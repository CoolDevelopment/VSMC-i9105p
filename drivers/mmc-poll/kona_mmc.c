/************************************************************************************************/
/*                                                                                              */
/*  Copyright 2010  Broadcom Corporation                                                        */
/*                                                                                              */
/*     Unless you and Broadcom execute a separate written software license agreement governing  */
/*     use of this software, this software is licensed to you under the terms of the GNU        */
/*     General Public License version 2 (the GPL), available at                                 */
/*                                                                                              */
/*          http://www.broadcom.com/licenses/GPLv2.php                                          */
/*                                                                                              */
/*     with the following added to such license:                                                */
/*                                                                                              */
/*     As a special exception, the copyright holders of this software give you permission to    */
/*     link this software with independent modules, and to copy and distribute the resulting    */
/*     executable under terms of your choice, provided that you also meet, for each linked      */
/*     independent module, the terms and conditions of the license of that module.              */
/*     An independent module is a module which is not derived from this software.  The special  */
/*     exception does not apply to any modifications of the software.                           */
/*                                                                                              */
/*     Notwithstanding the above, under no circumstances may you combine this software in any   */
/*     way with any other Broadcom software provided under a license other than the GPL,        */
/*     without Broadcom's express prior written consent.                                        */
/*                                                                                              */
/************************************************************************************************/

#include <linux/kernel.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/string.h>
#include <mach/rdb/brcm_rdb_emmcsdxc.h>
#include <mach/rdb/brcm_rdb_sysmap.h>
#include <mach/rdb/brcm_rdb_kpm_clk_mgr_reg.h>
#include <mach/io_map.h>
#include <mach/irqs.h>
#include <linux/mmc-poll/mmc_poll.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include "kona_mmc.h"

#ifdef DEBUG
#define debug(a,b...)  printk(a,##b)
#else
#define debug(a,b...)
#endif

/*
 * Note that this macro controls whether the SDIO transfers happen using
 * polling mode or not. When this macro is defined the data buffer is written
 * to the FIFO directly by polling the status of the FIFO (and vise versa for
 * reading) and should be enabled when the stack is to be used from exception
 * context.
 *
 * Also, note that when we moved this code base from u-boot to Linux we found
 * that SDMA was not functional in Linux, so using the SDMA but not using 
 * interrupts is not an option.
 */
/* #define PIO_MODE */

/*
 * All the register access are from io-remapped region 
 * The uboot code was using the PA directly. This is changed
 * during initalization so that the other part of the code need not change.
 */

/* support 4 mmc hosts */
#define KONA_MAX_MMC_DEV 4
struct mmc mmc_dev[KONA_MAX_MMC_DEV];
struct mmc_host mmc_host[KONA_MAX_MMC_DEV];

static unsigned int kona_get_base_clock_freq(void __iomem *reg_offset)
{
	unsigned int sdio_div;
	unsigned int clk_freq;
	unsigned int clk_div;

	sdio_div = readl(reg_offset);

	clk_freq = ((sdio_div & 0x7) < 3) ? 52000000 : 96000000;
	clk_div = ((sdio_div >> 4) & 0x3FFF) + 1;

	return clk_freq / clk_div;
}

static dma_addr_t buff_dma_addr;

static void mmc_prepare_data(struct mmc_host *host, struct mmc_data *data,
			     int cmd)
{
	unsigned int temp = 0;

	if (cmd == MMC_CMD_WRITE_MULTIPLE_BLOCK ||
	    cmd == MMC_CMD_WRITE_SINGLE_BLOCK ||
	    cmd == MMC_CMD_READ_MULTIPLE_BLOCK ||
	    cmd == MMC_CMD_READ_SINGLE_BLOCK) {
		buff_dma_addr =
		    dma_map_single(NULL, data->dest,
				   data->blocksize * data->blocks,
				   DMA_TO_DEVICE);
		writel(buff_dma_addr, &host->reg->sysad);
		debug
		    ("Using DMA transfer data->dest: %08x phys 0x%x sysad 0x%x \r\n",
		     (u32)data->dest, buff_dma_addr, &host->reg->sysad);
	}

	debug("data->blocksize: %08x data->blocks 0x%x blkcnt_sz 0x%x\r\n",
	      data->blocksize, data->blocks, &host->reg->blkcnt_sz);

	// Set up block size and block count. 
	// For KONA it is required to set HSBS field of block register tp 0x7.
	// Problem : When data is DMA across page boundaries, if HSBS field is set to 0,
	//           we see that transfer doesn't finish, and we see a hang.
	//           E.g : let's say a buffer in memory has a start address of 0x86234EF0.
	//                 During a read operation, data is read from SD in sizes of 512 bytes ( 0x200).
	//                 So now DMA of data will happen across address 0x86234F00, which is
	//                 a 4K page boundary. 
	//                 We see failures in this case when HSBS field is set to 0.
	//                 Solution is to set HSBS field val to 0x7 so that boundary DMA transactions are safe.
	//
	temp =
	    (7 << EMMCSDXC_BLOCK_HSBS_SHIFT) | data->blocksize | (data->
								  blocks <<
								  EMMCSDXC_BLOCK_BCNT_SHIFT);
	writel(temp, &host->reg->blkcnt_sz);
}

static void kona_mmc_clear_all_intrs(struct mmc_host *host)
{
	writel(0xFFFFFFFF, &host->reg->norintsts);	// Clear all interrupts.
	udelay(1000);
}

void kona_read_block_pio(struct mmc_host *host, struct mmc_data *data)
{
	unsigned long len;
	unsigned long *p;

	debug("+kona_read_block_pio \r\n");

	len = data->blocksize;
	p = (unsigned long *)data->dest;

	while (len) {
		*p = readl(&host->reg->bdata);
		len -= 4;
		p++;
	}
	debug("+kona_read_block_pio \r\n");
}

void kona_write_block_pio(struct mmc_host *host, struct mmc_data *data)
{
	unsigned long len;
	unsigned long *p;

	len = data->blocksize;
	p = (unsigned long *)data->src;

	while (len) {
		writel(*p, &host->reg->bdata);
		len -= 4;
		p++;
	}
}

void kona_transfer_pio(struct mmc_host *host, struct mmc_data *data)
{
	unsigned long mask = 0;
	unsigned long wait = 0;
	volatile unsigned long val;

	if (data->flags & MMC_DATA_READ) {
		mask = EMMCSDXC_PSTATE_BREN_MASK;
		wait = EMMCSDXC_INTR_BRRDY_MASK;
	} else {
		mask = EMMCSDXC_PSTATE_BWEN_MASK;
		wait = EMMCSDXC_INTR_BWRDY_MASK;
	}

	/* Wait for the buffer to become ready */
	do {
		val = readl(&host->reg->norintsts);
	} while ((val & wait) != wait);

	/* Clear the buffer status */
	val = val & ~mask;
	writel(val, &host->reg->norintsts);

	debug("Intr status says buffer ready \r\n");

	/* Perform the block transfer */

	/* 
	 * Note that we need to read only one block since
	 * the implementation assumes that on PIO NO Multi block 
	 * commands are used.
	 */

	/* 
	 * Keep reading untill the present status register also says
	 * that data is ready to be read. This bit useful
	 * for multi block transfers. That is once the interrupt
	 * status register says that buffer is ready, we can
	 * keep reading as many blocks as we want until the present
	 * status register says that the buffer has valid data (vise versa
	 * for write). But since we send only single block command,
	 * the infrastructure is built here and if needed can be extended 
	 * later
	 */
	while ((readl(&host->reg->prnsts) & mask) == mask) {

		if (mask & EMMCSDXC_PSTATE_BREN_MASK)
			kona_read_block_pio(host, data);
		else
			kona_write_block_pio(host, data);

		/* To support multi block transfer in future, 
		 * do not break unconditionally 
		 */
		break;
	}

	debug("PIO transfer done \r\n");

	return;
}

static int kona_mmc_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd,
			     struct mmc_data *data)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	int flags = 0;
	int i = 0;
	unsigned int timeout;
	volatile unsigned int mask;
	unsigned int retry = 10000;
	static int readCmd_count = 0;
	volatile unsigned int val;
	/* Wait max 10 ms */
	timeout = 10;

	debug("**** kona_mmc_send_cmd for cmd %d\n", cmd->cmdidx);

	/*
	 * PRNSTS
	 * CMDINHDAT[1] : Command Inhibit (DAT)
	 * CMDINHCMD[0] : Command Inhibit (CMD)
	 */
	mask = (1 << EMMCSDXC_PSTATE_CMDINH_SHIFT);	// Set command inhibit.
	if ((data != NULL) || (cmd->resp_type & MMC_RSP_BUSY))
		mask |= (1 << EMMCSDXC_PSTATE_DATINH_SHIFT);	// Set dat inhibit.

	/*
	 * We shouldn't wait for data inihibit for stop commands, even
	 * though they might use busy signaling
	 */
	if (data)
		mask &= ~(1 << EMMCSDXC_PSTATE_DATINH_SHIFT);

	while ((val = readl(&host->reg->prnsts)) & mask) {
		if (timeout == 0) {
			printk("%s : timeout error %d\n", __func__,
			       cmd->cmdidx);
			return TIMEOUT;
		}
		timeout--;
		udelay(1000);
	}

	// Set up block cnt, and block size.
	if (data)
		mmc_prepare_data(host, data, cmd->cmdidx);

	debug("cmd->arg: %08x\n", cmd->cmdarg);
	if (cmd->cmdidx == 17) {
		/* print out something to indicate we are alive. 
		 */
		readCmd_count++;
		if (0 == (readCmd_count % 100))
			printk(".");
	}

	writel(cmd->cmdarg, &host->reg->argument);

	flags = 0;
	if (data) {
		if (cmd->cmdidx == MMC_CMD_WRITE_MULTIPLE_BLOCK ||
		    cmd->cmdidx == MMC_CMD_WRITE_SINGLE_BLOCK ||
		    cmd->cmdidx == MMC_CMD_READ_MULTIPLE_BLOCK ||
		    cmd->cmdidx == MMC_CMD_READ_SINGLE_BLOCK) {
			flags =
			    (1 << EMMCSDXC_CMD_BCEN_SHIFT) | (1 <<
							      EMMCSDXC_CMD_DMA_SHIFT);
		} else {
			flags =
			    (1 << EMMCSDXC_CMD_BCEN_SHIFT) | (0 <<
							      EMMCSDXC_CMD_DMA_SHIFT);
		}

		if (data->blocks > 1)
			flags |= (1 << EMMCSDXC_CMD_MSBS_SHIFT);	// Multiple block select.
		if (data->flags & MMC_DATA_READ)
			flags |= (1 << EMMCSDXC_CMD_DTDS_SHIFT);	// 1= read, 0=write.
	}

	if ((cmd->resp_type & MMC_RSP_136) && (cmd->resp_type & MMC_RSP_BUSY))
		return -1;

	/*
	 * CMDREG
	 * CMDIDX[29:24]: Command index
	 * DPS[21]      : Data Present Select
	 * CCHK_EN[20]  : Command Index Check Enable
	 * CRC_EN[19]   : Command CRC Check Enable
	 * RTSEL[1:0]
	 *      00 = No Response
	 *      01 = Length 136
	 *      10 = Length 48
	 *      11 = Length 48 Check busy after response
	 */
	if (!(cmd->resp_type & MMC_RSP_PRESENT))
		flags |= (0 << EMMCSDXC_CMD_RTSEL_SHIFT);
	else if (cmd->resp_type & MMC_RSP_136)
		flags |= (1 << EMMCSDXC_CMD_RTSEL_SHIFT);
	else if (cmd->resp_type & MMC_RSP_BUSY)
		flags |= (3 << EMMCSDXC_CMD_RTSEL_SHIFT);
	else
		flags |= (2 << EMMCSDXC_CMD_RTSEL_SHIFT);

	if (cmd->resp_type & MMC_RSP_CRC) {
		/* Skip CRC check of cmd2 and cmd10 to fix Hynix device.
		 * Vendor comfirmed to have this workaround
		 */
		if (cmd->cmdidx != MMC_CMD_ALL_SEND_CID &&
		    cmd->cmdidx != MMC_CMD_SEND_CID) {
			flags |= (1 << EMMCSDXC_CMD_CRC_EN_SHIFT);
		}
	}

	if (cmd->resp_type & MMC_RSP_OPCODE)
		flags |= (1 << EMMCSDXC_CMD_CCHK_EN_SHIFT);
	if (data)
		flags |= (1 << EMMCSDXC_CMD_DPS_SHIFT);

	debug("cmd: %d\n", cmd->cmdidx);
	flags |= (cmd->cmdidx << EMMCSDXC_CMD_CIDX_SHIFT);

	writel(flags, &host->reg->cmdreg);

	for (i = 0; i < retry; i++) {
		mask = readl(&host->reg->norintsts);
		/* Command Complete */
		if (mask & (1 << 0)) {
			if (!data)
				writel(mask, &host->reg->norintsts);
			break;
		}
		udelay(1);
	}

	debug("intr status after cmd 0x%x \r\n", mask);

	if (i == retry) {
		debug("%s: waiting for status update\n", __func__);
		// Set CMDRST and DATARST bits. 
		// Problem :
		// -------
		// When a command 8 is sent in case of MMC card, it will not respond, and CMD INHIBIT bit
		// of PRSTATUS register will be set to 1, causing no more commands to be sent from host controller.
		// This causes things to stall. 
		// Solution :
		// ---------
		// In order to avoid this situation, we clear the CMDRST and DATARST bits in the case when card 
		// doesn't respond back to a command sent by host controller.
		writel(((0x3 << EMMCSDXC_CTRL1_CMDRST_SHIFT) |
			(readl(&host->reg->ctrl1_clkcon_timeout_swrst))),
		       &host->reg->ctrl1_clkcon_timeout_swrst);
		while ((0x3 << EMMCSDXC_CTRL1_CMDRST_SHIFT) &
		       readl(&host->reg->ctrl1_clkcon_timeout_swrst)) ;
		kona_mmc_clear_all_intrs(host);
		return TIMEOUT;
	}

	if (mask & (1 << 16)) {
		/* Timeout Error */
		debug("timeout: %08x cmd %d\n", mask, cmd->cmdidx);
		// Clear up the CMD inhibit and DATA inhibit bits.
		return TIMEOUT;
	} else if (mask & (1 << 15)) {
		/* Error Interrupt */
		debug("error: %08x cmd %d\n", mask, cmd->cmdidx);
		return -1;
	}

	if (cmd->resp_type & MMC_RSP_PRESENT) {
		if (cmd->resp_type & MMC_RSP_136) {
			/* CRC is stripped so we need to do some shifting. */
			for (i = 0; i < 4; i++) {
				unsigned int offset =
				    (unsigned int)(&host->reg->rspreg3 - i);
				cmd->response[i] = readl(offset) << 8;

				if (i != 3) {
					cmd->response[i] |= readb(offset - 1);
				}
				debug("cmd->resp[%d]: %08x\n",
				      i, cmd->response[i]);
			}
		} else if (cmd->resp_type & MMC_RSP_BUSY) {
			for (i = 0; i < retry; i++) {
				/* PRNTDATA[23:20] : DAT[3:0] Line Signal */
				if (readl(&host->reg->prnsts)
				    & (1 << 20))	/* DAT[0] */
					break;
				udelay(1);
			}

			if (i == retry) {
				printk("%s: card is still busy\n", __func__);
				return TIMEOUT;
			}

			cmd->response[0] = readl(&host->reg->rspreg0);
			debug("cmd->resp[0]: %08x\n", cmd->response[0]);
		} else {
			cmd->response[0] = readl(&host->reg->rspreg0);
			debug("cmd->resp[0]: %08x\n", cmd->response[0]);
		}
	}

	if (data) {
		if (cmd->cmdidx == MMC_CMD_WRITE_MULTIPLE_BLOCK ||
		    cmd->cmdidx == MMC_CMD_WRITE_SINGLE_BLOCK ||
		    cmd->cmdidx == MMC_CMD_READ_MULTIPLE_BLOCK ||
		    cmd->cmdidx == MMC_CMD_READ_SINGLE_BLOCK) {
			while (1) {
				mask = readl(&host->reg->norintsts);

				if (mask & EMMCSDXC_INTR_ERRIRQ_MASK) {
					/* Error Interrupt */
					writel(EMMCSDXC_INTR_ERRIRQ_MASK,
					       &host->reg->norintsts);
					printk
					    ("%s: error during transfer: 0x%08x\n",
					     __func__, mask);
					return -1;
				} else if (mask & EMMCSDXC_INTR_DMAIRQ_MASK) {
					/* DMA Interrupt */
					writel(EMMCSDXC_INTR_DMAIRQ_MASK,
					       &host->reg->norintsts);
					writel(readl(&host->reg->sysad),
					       &host->reg->sysad);
					debug("DMA end\n");
				} else if (mask & EMMCSDXC_INTR_TXDONE_MASK) {
					/* Transfer Complete */
					debug("r/w is done\n");
					break;
				}
			}
			writel(mask, &host->reg->norintsts);
			dma_unmap_single(NULL, buff_dma_addr,
					 data->blocksize * data->blocks,
					 DMA_TO_DEVICE);

		} else {
			kona_transfer_pio(host, data);
		}
	}
	// Clear all interrupts as per FPGA code.
	writel(0xFFFFFFFF, &host->reg->norintsts);
	// udelay(1000);
	return 0;
}

static void kona_mmc_change_clock(struct mmc_host *host, uint clock)
{
	int div = 0;
	unsigned int clk;
	unsigned long timeout;
	volatile long swrst;

	clk = readl(&host->reg->ctrl1_clkcon_timeout_swrst);
	clk = clk & 0xFFFF0000;	// Clean up all bits related to clock.
	writel(clk, &host->reg->ctrl1_clkcon_timeout_swrst);
	clk = 0;

	div = host->base_clock_freq / clock / 2;
	div = (host->base_clock_freq % clock) ? div + 1 : div;
#ifdef CONFIG_SAMOA_FPGA
	div = 0;		// SDIO clk divider does not work with SAMOA FPGA. It is set to 0
#endif
	debug("div: %d\n", div);

	// Write divider value, and enable internal clock.
	clk =
	    readl(&host->reg->
		  ctrl1_clkcon_timeout_swrst) | (div <<
						 EMMCSDXC_CTRL1_SDCLKSEL_SHIFT)
	    | (1 << EMMCSDXC_CTRL1_ICLKEN_SHIFT);
	writel(clk, &host->reg->ctrl1_clkcon_timeout_swrst);

	debug("host->reg->ctrl1_clkcon_timeout_swrst: 0x%x \r\n",
	      readl(&host->reg->ctrl1_clkcon_timeout_swrst));

	/* Wait for clock to stabilize */
	/* Wait max 10 ms */
	timeout = 10;
	while (!
	       (swrst =
		readl(&host->reg->
		      ctrl1_clkcon_timeout_swrst) & (1 <<
						     EMMCSDXC_CTRL1_ICLKSTB_SHIFT)))
	{
		if (timeout == 0) {
			printk("%s: timeout error\n", __func__);
			return;
		}
		timeout--;
		udelay(1000);
	}

	// Enable sdio clock now.
	clk |=
	    (1 << EMMCSDXC_CTRL1_SDCLKEN_SHIFT) | readl(&host->reg->
							ctrl1_clkcon_timeout_swrst);
	writel(clk, &host->reg->ctrl1_clkcon_timeout_swrst);

	host->clock = clock;
}

static void kona_mmc_set_ios(struct mmc *mmc)
{
	struct mmc_host *host = mmc->priv;
	unsigned char ctrl;

	debug("bus_width: %x, clock: %d\n", mmc->bus_width, mmc->clock);

	if (mmc->clock)
		kona_mmc_change_clock(host, mmc->clock);

	// Width and edge setting.
	// WIDTH : 
	ctrl = readl(&host->reg->ctrl_host_pwr_blk_wak);
	/*  1 = 4-bit mode , 0 = 1-bit mode */
	if (mmc->bus_width == 4)
		ctrl |= (1 << EMMCSDXC_CTRL_DXTW_SHIFT);
	else
		ctrl &= ~(1 << EMMCSDXC_CTRL_DXTW_SHIFT);

	if (mmc->card_caps & MMC_MODE_HS)
#ifdef CONFIG_SAMOA_FPGA
		ctrl &= ~(1 << EMMCSDXC_CTRL_HSEN_SHIFT);
#else
		ctrl |= (1 << EMMCSDXC_CTRL_HSEN_SHIFT);
#endif
	else
		ctrl &= ~(1 << EMMCSDXC_CTRL_HSEN_SHIFT);
	writel(ctrl, &host->reg->ctrl_host_pwr_blk_wak);
}

static void kona_mmc_reset(struct mmc_host *host)
{
	unsigned int timeout;

	/* Software reset for all * 1 = reset * 0 = work */
	writel((1 << EMMCSDXC_CTRL1_RST_SHIFT),
	       &host->reg->ctrl1_clkcon_timeout_swrst);

	host->clock = 0;

	/* Wait max 100 ms */
	timeout = 100;

	/* hw clears the bit when it's done */
	while (readl(&host->reg->ctrl1_clkcon_timeout_swrst) &
	       (1 << EMMCSDXC_CTRL1_RST_SHIFT)) {
		if (timeout == 0) {
			printk("%s: timeout error\n", __func__);
			return;
		}
		timeout--;
		udelay(1000);
	}
}

#ifdef DEBUG
static void kona_dump_mmc_regs(void __iomem *base)
{
	void __iomem *reg;
	unsigned long offset;

	printk("\r\n DUMPING MMC SD Registers \r\n");

	for (reg = base, offset = 0; offset <= (0x74 / 4); offset++)
		printk("reg 0x%p      val 0x%x \r\n",
		       (unsigned int *)reg + offset,
		       *((unsigned int *)reg + offset));

	reg = base + (0xE0 / 4);
	printk("reg 0x%p      val 0x%x \r\n", (unsigned int *)reg + offset,
	       *((unsigned int *)reg + offset));

	reg = base + (0xF0 / 4);
	printk("reg 0x%p      val 0x%x \r\n", (unsigned int *)reg + offset,
	       *((unsigned int *)reg + offset));

	reg = base + (0xFC / 4);
	printk("reg 0x%p      val 0x%x \r\n", (unsigned int *)reg + offset,
	       *((unsigned int *)reg + offset));

	/* Core registers dump */
	for (reg = base, offset = 0x8000; offset <= (0x8018 / 4); offset++)
		printk("reg 0x%p      val 0x%x \r\n",
		       (unsigned int *)reg + offset,
		       *((unsigned int *)reg + offset));

	return;
}
#endif

static int kona_mmc_core_init(struct mmc *mmc)
{
	struct mmc_host *host = (struct mmc_host *)mmc->priv;
	unsigned int mask;

	debug("+ kona_mmc_core_init \r\n");
	// For kona a hardware reset before anything else.
	// TBD : Remove this, it is needed in case of Uboot bring up only.
	writel(EMMCSDXC_CORECTRL_EN_MASK, &host->reg_p3->corectrl);

	// Set the reset bit, wait for some time, and clear the reset bit. 
	// Set the reset bit.
	mask = readl(&host->reg_p3->corectrl) | EMMCSDXC_CORECTRL_RESET_MASK;
	writel(mask, &host->reg_p3->corectrl);
	udelay(10);

	// Clear the reset bit.
	mask = mask & ~(EMMCSDXC_CORECTRL_RESET_MASK);
	writel(mask, &host->reg_p3->corectrl);
	udelay(10);

	// Set power now.
	mask =
	    readl(&host->reg->
		  ctrl_host_pwr_blk_wak) | (7 << EMMCSDXC_CTRL_SDVSEL_SHIFT) |
	    EMMCSDXC_CTRL_SDPWR_MASK;
	writel(mask, &host->reg->ctrl_host_pwr_blk_wak);

	kona_mmc_reset(host);

	host->version =
	    (readl(&host->reg_p2->hcversirq) | EMMCSDXC_HCVERSIRQ_VENDVER_MASK)
	    >> EMMCSDXC_HCVERSIRQ_VENDVER_SHIFT;

	/* mask all */
	writel(0xffffffff, &host->reg->norintstsen);
	writel(0xffffffff, &host->reg->norintsigen);

	writel(((0xd << EMMCSDXC_CTRL1_DTCNT_SHIFT) | (readl(&host->reg->ctrl1_clkcon_timeout_swrst))), &host->reg->ctrl1_clkcon_timeout_swrst);	/* TMCLK * 2^26 */

	/*
	 * Interrupt Status Enable Register init
	 * bit 5 : Buffer Read Ready Status Enable
	 * bit 4 : Buffer write Ready Status Enable
	 * bit 1 : Transfre Complete Status Enable
	 * bit 0 : Command Complete Status Enable
	 */
	mask = readl(&host->reg->norintstsen);
	mask &= ~(0xffff);
	mask |= (1 << EMMCSDXC_INTREN1_BUFRREN_SHIFT) |
	    (1 << EMMCSDXC_INTREN1_BUFWREN_SHIFT) |
	    (1 << EMMCSDXC_INTREN1_DMAIRQEN_SHIFT) |
	    (1 << EMMCSDXC_INTREN1_TXDONEEN_SHIFT) |
	    (1 << EMMCSDXC_INTREN1_CMDDONEEN_SHIFT);
	writel(mask, &host->reg->norintstsen);

	/*
	 * Interrupt Signal Enable Register init
	 * bit 1 : Transfer Complete Signal Enable
	 */
	mask = readl(&host->reg->norintsigen);
	mask &= ~(0xffff);
	mask |= (1 << EMMCSDXC_INTREN2_TXDONE_SHIFT);
	writel(mask, &host->reg->norintsigen);

	/*
	 * We are going to work only on polling mode,
	 * disable the interrupts from the IP
	 */
	mask = readl(&host->reg_p3->coreimr);
	mask &= ~(0x1);
	writel(mask, &host->reg_p3->coreimr);

#ifdef DEBUG
	debug("mmc core init done dumping SDIO2 base addr regs \r\n");
	kona_dump_mmc_regs(KONA_SDIO2_VA);
#endif

	debug("- kona_mmc_core_init \r\n");
	return 0;
}

void kona_mmc_clk_init(void __iomem *clk_base, void __iomem *clk_gate)
{
	unsigned long reg_val;

	/* Unlock the clock registers */
	writel(0x00a5a501, clk_base);

	/* Enable both the bus clock and peripheral clock */
	reg_val = readl(clk_gate);
	reg_val = reg_val |
	    KPM_CLK_MGR_REG_SDIO1_CLKGATE_SDIO1_CLK_EN_MASK |
	    KPM_CLK_MGR_REG_SDIO1_CLKGATE_SDIO1_AHB_CLK_EN_MASK;
	writel(reg_val, clk_gate);
}

int kona_mmc_init(int dev_index)
{
	struct mmc *mmc;
	void *mmc_reg_base;
	void __iomem *source_clk_reg;

	debug("\r\n + kona_mmc_init %d \r\n", dev_index);

	switch (dev_index) {
	case 1:
		disable_irq(BCM_INT_ID_SDIO0);
		mmc_reg_base = (void *)KONA_SDIO1_VA;
		source_clk_reg =
		    KONA_KPM_CLK_VA + KPM_CLK_MGR_REG_SDIO1_DIV_OFFSET;
		kona_mmc_clk_init((void *)KONA_KPM_CLK_VA,
				  (void *)(KONA_KPM_CLK_VA +
					   KPM_CLK_MGR_REG_SDIO1_CLKGATE_OFFSET));
		break;
	case 2:
		disable_irq(BCM_INT_ID_SDIO1);
		mmc_reg_base = (void *)KONA_SDIO2_VA;
		source_clk_reg =
		    KONA_KPM_CLK_VA + KPM_CLK_MGR_REG_SDIO2_DIV_OFFSET;
		kona_mmc_clk_init((void *)KONA_KPM_CLK_VA,
				  (void *)(KONA_KPM_CLK_VA +
					   KPM_CLK_MGR_REG_SDIO2_CLKGATE_OFFSET));
		break;
#ifdef SDIO3_BASE_ADDR
	case 3:
		disable_irq(BCM_INT_ID_SDIO_NAND);
		mmc_reg_base = (void *)KONA_SDIO3_VA;
		source_clk_reg =
		    KONA_KPM_CLK_VA + KPM_CLK_MGR_REG_SDIO3_DIV_OFFSET;
		kona_mmc_clk_init((void *)KONA_KPM_CLK_VA,
				  (void *)(KONA_KPM_CLK_VA +
					   KPM_CLK_MGR_REG_SDIO3_CLKGATE_OFFSET));
		break;
#endif
	default:
		printk("Only support up to %d mmc device\n", KONA_MAX_MMC_DEV);
		return -1;
	}

	mmc = &mmc_dev[dev_index];

	sprintf(mmc->name, "KONA SD/MMC");
	mmc->priv = &mmc_host[dev_index];
	mmc->send_cmd = kona_mmc_send_cmd;
	mmc->set_ios = kona_mmc_set_ios;
	mmc->init = kona_mmc_core_init;

	mmc->voltages =
	    MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 | MMC_VDD_30_31 |
	    MMC_VDD_31_32 | MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_34_35 |
	    MMC_VDD_35_36;
	mmc->host_caps = MMC_MODE_4BIT | MMC_MODE_HS_52MHz | MMC_MODE_HS;

	mmc_host[dev_index].base_clock_freq =
	    kona_get_base_clock_freq(source_clk_reg);

	mmc->f_min = 400000;
	mmc->f_max = mmc_host[dev_index].base_clock_freq;

	mmc_host[dev_index].clock = 0;
	mmc_host[dev_index].reg = mmc_reg_base;
	mmc_host[dev_index].reg_p2 = mmc_reg_base + EMMCSDXC_SBUSCTRL_OFFSET;
	mmc_host[dev_index].reg_p3 = mmc_reg_base + EMMCSDXC_CORECTRL_OFFSET;

	mmc_register(mmc);

	debug("\r\n - kona_mmc_init \r\n");

	return 0;
}

/*******************************************************************************
* Copyright 2011 Broadcom Corporation.  All rights reserved.
*
* 	@file	 drivers/input/keyboard/bcm_keypad.c
*
* Unless you and Broadcom execute a separate written software license agreement
* governing use of this software, this software is licensed to you under the
* terms of the GNU General Public License version 2, available at
* http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
*******************************************************************************/
#define DEBUG /* enable the pr_debug calls */
#define tempINTERFACE_OSDAL_KEYPAD

#include <linux/module.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <asm/sizes.h>
#include <mach/hardware.h>
#include <mach/memory.h>
#include <linux/io.h>
#include <mach/bcm_keypad.h>
#include <linux/slab.h>
#include <linux/clk.h>
#ifndef _HERA_
#include <mach/rdb/brcm_rdb_sysmap.h>
#include <chal/chal_util.h>
#include <mach/rdb/brcm_rdb_padctrlreg.h>
#endif

#include "plat/chal/chal_types.h"
#include "plat/chal/chal_common.h"
#include "plat/chal/chal_keypad.h"

/*Debug messages */
#define       BCMKP_DEBUG 
#ifdef BCMKP_DEBUG
#define BCMKP_DBG(format, args...)	pr_info(__FILE__ ":" format, ## args)
#else
#define	BCMKP_DBG(format, args...)
#endif

#define BCM_KEY_REPEAT_PERIOD     100	/*repeat period (msec) */
#define BCM_KEY_REPEAT_DELAY      400	/*First time press delay (msec) */

#define KEYPAD_MAX_ROWS    8
#define KEYPAD_MAX_COLUMNS 8

#define MATRIX_SIZE               2	/* 32 bits */
#define U32_BITS                  32

#define KPDCR_MSK_COLFLT_S       8
#define KPDCR_MSK_ROWFLT_S       12
#define KPDCR_MSK_COLS_S         16
#define KPDCR_MSK_ROWS_S         20

#define KPD_ROWS(x)              ((x - 1) << KPDCR_MSK_ROWS_S)
#define KPD_COLS(x)              ((x - 1) << KPDCR_MSK_COLS_S)
#define KPD_ROWFLT(x)            ((x) << KPDCR_MSK_ROWFLT_S)
#define KPD_COLFLT(x)            ((x) << KPDCR_MSK_COLFLT_S)

#define KPDCR_FLT_1ms            0x0
#define KPDCR_FLT_2ms            0x1
#define KPDCR_FLT_4ms            0x2
#define KPDCR_FLT_8ms            0x3
#define KPDCR_FLT_16ms           0x4
#define KPDCR_FLT_32ms           0x5
#define KPDCR_FLT_64ms           0x6	/* default */
#define KPDCR_FLT_128ms          0x7

#define KPEMR_NO_TRIGGER           0x0
#define KPEMR_RISING_EDGE          0x1
#define KPEMR_FALLING_EDGE         0x2
#define KPEMR_BOTH_EDGE            0x3

/* ---- Constants and Types ---------------------------------------------- */
static void __iomem *bcm_keypad_base_addr;

#define REG_KEYPAD_KPCR     0x00
#define REG_KEYPAD_KPIOR    0x04

/* Special clone keypad registers, same bases as GPIO */
/* registers, but custom bit assignments for rows/columns. */
#define REG_KEYPAD_KPEMR0  0x10
#define REG_KEYPAD_KPEMR1  0x14
#define REG_KEYPAD_KPEMR2  0x18
#define REG_KEYPAD_KPEMR3  0x1C
#define REG_KEYPAD_KPSSR0  0x20
#define REG_KEYPAD_KPSSR1  0x24
#define REG_KEYPAD_KPIMR0  0x30
#define REG_KEYPAD_KPIMR1  0x34
#define REG_KEYPAD_KPICR0  0x38
#define REG_KEYPAD_KPICR1  0x3C
#define REG_KEYPAD_KPISR0  0x40
#define REG_KEYPAD_KPISR1  0x44

/* REG_KEYPAD_KPCR bits */
#define REG_KEYPAD_KPCR_ENABLE              0x00000001	/* Enable key pad control */
#define REG_KEYPAD_KPCR_PULL_UP             0x00000002
#define REG_KEYPAD_COLFILTER_EN             0x00000800	/* Enable column filtering */
#define REG_KEYPAD_STATFILTER_EN            0x00008000	/* Enable status filtering */

#define BCM_INTERRUPT_EVENT_FIFO_LENGTH	4
#define FIFO_EMPTY(fifo)				((fifo.head == fifo.tail) && !fifo.fifo_full)
#define FIFO_INCREMENT_HEAD(fifo)	(fifo.head = ((fifo.head+1) & (fifo.length-1)))
#define FIFO_INCREMENT_TAIL(fifo)	(fifo.tail = ((fifo.tail+1) & (fifo.length-1)))

struct bcm_keypad {
	struct input_dev *input_dev;
	struct bcm_keymap *kpmap;
	spinlock_t bcm_kp_spin_Lock;
	struct clk *key_clk;
	unsigned int irq;	/* Device IRQ */
	unsigned int row_num;
	unsigned int col_num;
	unsigned int matrix[MATRIX_SIZE];
	unsigned int oldmatrix[MATRIX_SIZE];
};

typedef struct
{
	unsigned char		head;
	unsigned char		tail;
	unsigned char		length;
	bool				fifo_full;
	CHAL_KEYPAD_REGISTER_SET_t	eventQ[BCM_INTERRUPT_EVENT_FIFO_LENGTH];
} BCM_KEYPAD_INTERRUPT_FIFO_t; // interrupt event Q - register data from an interrupt to be processed later.

static void bcm_keypad_tasklet(unsigned long);
/*static void bcm_handle_key_state(struct bcm_keypad *bcm_kb);*/
DECLARE_TASKLET_DISABLED(kp_tasklet, bcm_keypad_tasklet, 0);

static CHAL_HANDLE keypadHandle = NULL;
static CHAL_KEYPAD_KEY_EVENT_LIST_t keyEventList;
static BCM_KEYPAD_INTERRUPT_FIFO_t intrFifo;

/* ****************************************************************************** */
/* Function Name: bcm_keypad_interrupt */
/* Description: Interrupt handler, called whenever a keypress/release occur. */
/* ****************************************************************************** */

static irqreturn_t bcm_keypad_interrupt(int irq, void *dev_id)
{
	struct bcm_keypad *bcm_kb = dev_id;
	unsigned long flags;
	CHAL_KEYPAD_REGISTER_SET_t *event;

	spin_lock_irqsave(&bcm_kb->bcm_kp_spin_Lock, flags);

	disable_irq_nosync(irq);

	if (!intrFifo.fifo_full)
	{
		event = &intrFifo.eventQ[intrFifo.head];
		chal_keypad_retrieve_key_event_registers(keypadHandle, event);
		FIFO_INCREMENT_HEAD(intrFifo);
		if(intrFifo.head == intrFifo.tail)
			intrFifo.fifo_full = TRUE;
	}
	chal_keypad_clear_interrupts(keypadHandle);

	tasklet_schedule(&kp_tasklet);
	enable_irq(irq);
	spin_unlock_irqrestore(&bcm_kb->bcm_kp_spin_Lock, flags);
	return IRQ_HANDLED;
}

/* ****************************************************************************** */
/* Function Name: bcm_handle_key */
/* Description: Report key actions to input framework */
/* ****************************************************************************** */
static void bcm_handle_key(struct bcm_keypad *bcm_kb, CHAL_KEYPAD_KEY_ID_t	keyId, 
    CHAL_KEYPAD_ACTION_t    keyAction)
{
	struct bcm_keymap *keymap_p = bcm_kb->kpmap;
    /* KeyId is of the form 0xCR where:  C = column number    R = row number 
            Use it as index into map structure */
    unsigned char vk = keymap_p[keyId].key_code;

    if (keyAction == CHAL_KEYPAD_KEY_PRESS)
    {
        input_report_key(bcm_kb->input_dev, vk, 1);
        input_sync(bcm_kb->input_dev);
        pr_debug("%s press\n", keymap_p[keyId].name);
    }
    else if (keyAction == CHAL_KEYPAD_KEY_RELEASE)
    {
        input_report_key(bcm_kb->input_dev, vk, 0); 
        input_sync(bcm_kb->input_dev);
        pr_debug("key release vk=%d\n", vk);
    }
}

#ifdef _HERA_

/* ****************************************************************************** */
/* Function Name: bcm_set_keypad_pinmux */
/* Description: configure keypad pin muxing. */
/* ****************************************************************************** */
static void bcm_set_keypad_pinmux(void)
{
    PadCtrlConfig_t padCtrlCfg;
    CHAL_HANDLE handle;

    handle = chal_padctrl_init(HW_IO_PHYS_TO_VIRT(PAD_CTRL_BASE_ADDR));
    
    padCtrlCfg.DWord=0;    
    padCtrlCfg.PadCtrlConfigBitField.mux = 1;
    padCtrlCfg.PadCtrlConfigBitField.hys = 0;
    padCtrlCfg.PadCtrlConfigBitField.pdn = 0;
    padCtrlCfg.PadCtrlConfigBitField.pup = 0;
    padCtrlCfg.PadCtrlConfigBitField.rate = 0;
    padCtrlCfg.PadCtrlConfigBitField.ind = 0;
    padCtrlCfg.PadCtrlConfigBitField.mode = 0;

    // Grant keypad function request
    chal_padctrl_grant_kpd(handle, padCtrlCfg);	
}

#else

/* ****************************************************************************** */
/* Function Name: bcm_set_keypad_pinmux */
/* Description: configure keypad pin muxing. */
/* ****************************************************************************** */
static void bcm_set_keypad_pinmux(void)
{

	//use pmux api when it's available for Rhea
	// currently keypad pinmux is set through BSP
}

#endif
/* ****************************************************************************** */
/* Function Name: bcm_keypad_probe */
/* Description: Called to perform module initialization when the module is loaded. */
/* ****************************************************************************** */
static int __devinit bcm_keypad_probe(struct platform_device *pdev)
{
	int ret;
	u32 i;
	/*u32 reg_value;*/

	struct bcm_keypad *bcm_kb;
	struct bcm_keypad_platform_info *pdata;
	struct bcm_keymap *keymap_p;
	CHAL_KEYPAD_CONFIG_t hwConfig;

	BCMKP_DBG(KERN_NOTICE "bcm_keypad_probe\n");
	if (!pdev->dev.platform_data) {
		pr_err("%s(%s:%u)::Failed to get platform data\n",
		       __FUNCTION__, __FILE__, __LINE__);
		return -ENOMEM;
	}

	pdata = pdev->dev.platform_data;
	keymap_p = pdata->keymap;
	bcm_keypad_base_addr = pdata->bcm_keypad_base;

	bcm_kb = kmalloc(sizeof(*bcm_kb), GFP_KERNEL);
	if (bcm_kb == NULL) {
		pr_err(
		       "%s(%s:%u)::Failed to allocate keypad structure...\n",
		       __FUNCTION__, __FILE__, __LINE__);
		return -ENOMEM;
	}
	memset(bcm_kb, 0, sizeof(*bcm_kb));

	bcm_kb->input_dev = input_allocate_device();
	if (bcm_kb->input_dev == NULL) {
		pr_err(
		       "%s(%s:%u)::Failed to allocate input device...\n",
		       __FUNCTION__, __FILE__, __LINE__);
		kfree(bcm_kb);
		return -ENOMEM;
	}
	platform_set_drvdata(pdev, bcm_kb);
	bcm_kb->kpmap = pdata->keymap;

	bcm_kb->key_clk = clk_get(NULL, "gpiokp_apb");
	clk_enable(bcm_kb->key_clk);

	/* Setup input device */
	set_bit(EV_KEY, bcm_kb->input_dev->evbit);
	set_bit(EV_REP, bcm_kb->input_dev->evbit);
	bcm_kb->input_dev->rep[REP_PERIOD] = BCM_KEY_REPEAT_PERIOD;	/* repeat period */
	bcm_kb->input_dev->rep[REP_DELAY] = BCM_KEY_REPEAT_DELAY;	/* fisrt press delay */

	bcm_kb->input_dev->name = "bcm_keypad_v2";
	bcm_kb->input_dev->phys = "keypad/input0";
	bcm_kb->input_dev->id.bustype = BUS_HOST;
	bcm_kb->input_dev->id.vendor = 0x0001;
	bcm_kb->input_dev->id.product = 0x0001;
	bcm_kb->input_dev->id.version = 0x0100;

	memset(bcm_kb->matrix, 0, sizeof(bcm_kb->matrix));

	bcm_kb->row_num = pdata->row_num;	/* KPD_ROW_NUM ; */
	bcm_kb->col_num = pdata->col_num;	/* KPD_COL_NUM ; */
	spin_lock_init(&bcm_kb->bcm_kp_spin_Lock);
	tasklet_enable(&kp_tasklet);
	kp_tasklet.data = (unsigned long)bcm_kb;

	bcm_kb->irq = BCM_INT_ID_KEYPAD;

	pr_debug("%s::bcm_keypad_probe\n", __FUNCTION__);

    /* New chal based h/w setup */
    bcm_set_keypad_pinmux();

	intrFifo.head =0;
	intrFifo.tail = 0;
	intrFifo.length = BCM_INTERRUPT_EVENT_FIFO_LENGTH;
	intrFifo.fifo_full = FALSE;

	hwConfig.rows = bcm_kb->row_num;
	hwConfig.columns = bcm_kb->col_num;
	hwConfig.pullUpMode = FALSE;
	// Porbably shouldn't copy these next 2 directly in case one structure changes but not the corresponding one.
	hwConfig.interruptEdge = CHAL_KEYPAD_INTERRUPT_BOTH_EDGES;
	hwConfig.debounceTime = CHAL_KEYPAD_DEBOUNCE_32_ms;

	keypadHandle = chal_keypad_init((cUInt32)bcm_keypad_base_addr);
	// disable all key interrupts
	chal_keypad_disable_interrupts(keypadHandle);
	// clear any old interrupts
	chal_keypad_clear_interrupts(keypadHandle);
	// disable the keypad hardware block
	chal_keypad_set_enable(keypadHandle, FALSE);
	chal_keypad_set_pullup_mode(keypadHandle, hwConfig.pullUpMode);
	chal_keypad_set_column_filter(keypadHandle, TRUE, hwConfig.debounceTime);
	chal_keypad_set_row_width(keypadHandle, hwConfig.rows);
	chal_keypad_set_column_width(keypadHandle, hwConfig.columns);
#ifdef SWAP_ROW_COL
	chal_keypad_swap_row_and_column(keypadHandle, TRUE);
#endif
	// use rows as output
	chal_keypad_set_row_output_control(keypadHandle, hwConfig.rows);
	// configure the individual key interrupt controls
	chal_keypad_set_interrupt_edge(keypadHandle, hwConfig.interruptEdge);
	// clear any old interrupts
	chal_keypad_clear_interrupts(keypadHandle);
	chal_keypad_set_interrupt_mask(keypadHandle, hwConfig.rows, hwConfig.columns);
	// clear any old interrupts
	chal_keypad_clear_interrupts(keypadHandle);
	// enable the keypad hardware block
	chal_keypad_set_enable(keypadHandle, TRUE);
	
	for (i = 0; i < (KEYPAD_MAX_ROWS * KEYPAD_MAX_COLUMNS); i++) {
		__set_bit(keymap_p->key_code & KEY_MAX,
			  bcm_kb->input_dev->keybit);
		keymap_p++;
	}

	ret =
	    request_irq(bcm_kb->irq, bcm_keypad_interrupt, IRQF_DISABLED |
			    IRQF_NO_SUSPEND, "BRCM Keypad", bcm_kb);
	if (ret < 0) {
		pr_err("%s(%s:%u)::request_irq failed IRQ %d\n",
		       __FUNCTION__, __FILE__, __LINE__, bcm_kb->irq);
		goto free_irq;
	}
	ret = input_register_device(bcm_kb->input_dev);
	if (ret < 0) {
		pr_err(
		       "%s(%s:%u)::Unable to register GPIO-keypad input device\n",
		       __FUNCTION__, __FILE__, __LINE__);
		goto free_dev;
	}

	/* Initialization Finished */
	BCMKP_DBG(KERN_DEBUG "BCM keypad initialization completed...\n");
	return ret;

      free_dev:
	input_unregister_device(bcm_kb->input_dev);
	input_free_device(bcm_kb->input_dev);

      free_irq:
	free_irq(bcm_kb->irq, (void *)bcm_kb);

	return -EINVAL;
}

/* ****************************************************************************** */
/* Function Name: bcm_keypad_remove */
/* Description: Called to perform module cleanup when the module is unloaded. */
/* ****************************************************************************** */
static int __devexit bcm_keypad_remove(struct platform_device *pdev)
{
	struct bcm_keypad *bcm_kb = platform_get_drvdata(pdev);
	BCMKP_DBG(KERN_NOTICE "bcm_keypad_remove\n");

	/* disable keypad interrupt handling */
	tasklet_disable(&kp_tasklet);

	/*disable keypad interrupt handling */
	free_irq(bcm_kb->irq, (void *)bcm_kb);

	/* unregister everything */
	input_unregister_device(bcm_kb->input_dev);
	input_free_device(bcm_kb->input_dev);

	return 0;
}



/* ****************************************************************************** */
/* Function Name: bcm_keypad_tasklet */
/* Description: tasklet for a keypress/release occur. . */
/* ****************************************************************************** */

static void bcm_keypad_tasklet(unsigned long data)
{
	/* Use SSR to see which key be pressed */
	struct bcm_keypad *bcm_kb = (struct bcm_keypad *)data;

	CHAL_KEYPAD_REGISTER_SET_t *event;
	cUInt32 numKeyEvt = 0;
	int i;
	
	while(!FIFO_EMPTY(intrFifo))
	{
		event = &intrFifo.eventQ[intrFifo.tail];
		numKeyEvt = chal_keypad_process_key_event_registers(keypadHandle, event, keyEventList);
		FIFO_INCREMENT_TAIL(intrFifo);
		if(intrFifo.fifo_full)
		{
			intrFifo.fifo_full = FALSE;
		}
		for(i=0; i<numKeyEvt; i++)
		{
			if(keyEventList[i].keyAction != CHAL_KEYPAD_KEY_NO_ACTION)
			{
				bcm_handle_key(bcm_kb, keyEventList[i].keyId, keyEventList[i].keyAction);
			}
		}
	}
}

/****************************************************************************/

struct platform_driver bcm_keypad_device_driver = {
	.probe = bcm_keypad_probe,
	.remove = __devexit_p(bcm_keypad_remove),
	.driver = {
		   .name = "bcm_keypad",
		   }
};

static int __init bcm_keypad_init(void)
{
	pr_info("bcm_keypad_device_driver\n");
	return platform_driver_register(&bcm_keypad_device_driver);
}

static void __exit bcm_keypad_exit(void)
{
	platform_driver_unregister(&bcm_keypad_device_driver);
}

module_init(bcm_keypad_init);
module_exit(bcm_keypad_exit);

MODULE_AUTHOR("Broadcom Corportaion");
MODULE_DESCRIPTION("BCM Keypad Driver");
MODULE_LICENSE("GPL");

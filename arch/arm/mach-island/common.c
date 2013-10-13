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
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/serial_8250.h>
#include <linux/clk.h>
#include <asm/mach-types.h>
#include <mach/hardware.h>
#include <mach/kona.h>
#include <mach/rdb/brcm_rdb_uartb.h>
#include <mach/irqs.h>
#include <plat/chal/chal_trace.h>
#include <trace/stm.h>
#include <asm/pmu.h>
#if defined (CONFIG_KONA_CPU_FREQ_DRV)
#include <plat/kona_cpufreq_drv.h>
#include <linux/cpufreq.h>
#include <mach/pi_mgr.h>
#endif
#include <plat/pl330-pdata.h>
#include <linux/dma-mapping.h>


#define KONA_UART0_PA   UARTB_BASE_ADDR
#define KONA_UART1_PA   UARTB2_BASE_ADDR
#define KONA_UART2_PA   UARTB3_BASE_ADDR
#define KONA_UART3_PA   UARTB4_BASE_ADDR

#define UART_CLOCK_RATE 13000000

#define KONA_8250PORT(name,clk)                                                   \
{                                                                             \
   .membase    = (void __iomem *)(KONA_##name##_VA),                          \
   .mapbase    = (resource_size_t)(KONA_##name##_PA),                         \
   .irq        = BCM_INT_ID_##name,                                           \
   .uartclk    = UART_CLOCK_RATE,                                             \
   .regshift   = 2,                                                           \
   .iotype     = UPIO_DWAPB,                                                  \
   .type       = PORT_16550A,                                                 \
   .flags      = UPF_BOOT_AUTOCONF | UPF_FIXED_TYPE | UPF_SKIP_TEST,          \
   .private_data = (void __iomem *)((KONA_##name##_VA) + UARTB_USR_OFFSET),   \
   .clk_name	= clk,	\
}

struct uart_clk_cfg {
	char *bus_clk;
	char *peri_clk;
};

static struct uart_clk_cfg uart_clk[] = {
	[0] = {
		.bus_clk = "uartb_apb_clk",
		.peri_clk = "uartb_clk",
	},
	[1] = {
                .bus_clk = "uartb2_apb_clk",
                .peri_clk = "uartb2_clk",
        },
	[2] = {
                .bus_clk = "uartb3_apb_clk",
                .peri_clk = "uartb3_clk",
        },
	[3] = {
                .bus_clk = "uartb4_apb_clk",
                .peri_clk = "uartb4_clk",
        },
};

static struct plat_serial8250_port uart_data[] = {
	KONA_8250PORT(UART0,"uartb_clk"),
	KONA_8250PORT(UART1,"uartb2_clk"),
	KONA_8250PORT(UART2,"uartb3_clk"),
	KONA_8250PORT(UART3,"uartb4_clk"),
	{ .flags = 0, },
};

static struct platform_device board_serial_device = {
	.name = "serial8250",
	.id = PLAT8250_DEV_PLATFORM,
	.dev = {
		.platform_data = uart_data,
	},
};

#ifdef CONFIG_STM_TRACE
static struct stm_platform_data stm_pdata = {
	.regs_phys_base       = STM_BASE_ADDR,
	.channels_phys_base   = SWSTM_BASE_ADDR,
	.id_mask              = 0x0,   /* Skip ID check/match */
	.final_funnel	      = CHAL_TRACE_FIN_FUNNEL,
};

struct platform_device kona_stm_device = {
	.name = "stm",
	.id = -1,
	.dev = {
	        .platform_data = &stm_pdata,
	},
};
#endif

#if defined(CONFIG_HW_RANDOM_KONA)
static struct resource rng_device_resource[] = {
    [0] = {
        .start = SEC_RNG_BASE_ADDR,
        .end   = SEC_RNG_BASE_ADDR + 0x14,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = BCM_INT_ID_SECURE_TRAP1,
        .end   = BCM_INT_ID_SECURE_TRAP1,
        .flags = IORESOURCE_IRQ,
    },
};

static struct platform_device rng_device =
{
   .name          = "kona_rng",
   .id            = -1,
   .resource	  = rng_device_resource,
   .num_resources = ARRAY_SIZE(rng_device_resource),
};
#endif

#if defined(CONFIG_KONA_PWMC)
static struct resource pwm_device_resource[] = {
    [0] = {
        .start = PWM_BASE_ADDR,
        .end   = PWM_BASE_ADDR + 0x10,
        .flags = IORESOURCE_MEM,
    },
};

static struct platform_device pwm_device =
{
   .name          = "kona_pwmc",
   .id            = -1,
   .resource	  = pwm_device_resource,
   .num_resources = ARRAY_SIZE(pwm_device_resource),
};
#endif

#if defined(CONFIG_W1_MASTER_DS1WM)
static struct resource d1w_device_resource[] = {
    [0] = {
        .start = D1W_BASE_ADDR,
        .end   = D1W_BASE_ADDR + 0x10,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = BCM_INT_ID_DALLAS_1_WIRE,
        .end   = BCM_INT_ID_DALLAS_1_WIRE,
        .flags = IORESOURCE_IRQ,
    },
};

static struct platform_device d1w_device =
{
   .name          = "ds1wm",
   .id            = -1,
   .resource	  = d1w_device_resource,
   .num_resources = ARRAY_SIZE(d1w_device_resource),
};
#endif

#if defined(CONFIG_MPCORE_WATCHDOG)
static struct resource wdt_device_resource[] = {
    [0] = {
        .start = PTIM_BASE_ADDR,
        .end   = PTIM_BASE_ADDR + 0x34,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = BCM_INT_ID_PPI14,
        .end   = BCM_INT_ID_PPI14,
        .flags = IORESOURCE_IRQ,
    },
};

static struct platform_device wdt_device =
{
   .name          = "mpcore_wdt",
   .id            = -1,
   .resource	  = wdt_device_resource,
   .num_resources = ARRAY_SIZE(wdt_device_resource),
   .dev = {
        .platform_data = "arm_periph_clk",
    },
};
#endif

#if defined(CONFIG_MTD_BCMNAND)
static struct platform_device nand_device =
{
   .name          = "bcmnand",
   .id            = -1,
};
#endif

#if defined(CONFIG_RTC_DRV_ISLAND)
static struct resource rtc_device_resource[] = {
    [0] = {
        .start = BBL_BASE_ADDR,
        .end   = BBL_BASE_ADDR + 0x24,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = BCM_INT_ID_BBL1,
        .end   = BCM_INT_ID_BBL1,
        .flags = IORESOURCE_IRQ,
    },
    [2] = {
        .start = BCM_INT_ID_BBL2,
        .end   = BCM_INT_ID_BBL2,
        .flags = IORESOURCE_IRQ,
    },
};

static struct platform_device rtc_device =
{
   .name          = "island-rtc",
   .id            = -1,
   .resource	  = rtc_device_resource,
   .num_resources = ARRAY_SIZE(rtc_device_resource),
   .dev = {
        .platform_data = "bbl_apb_clk",
    },
};
#endif

/* ARM performance monitor unit */
static struct resource pmu_resource = {
       .start = BCM_INT_ID_PMU_IRQ0,
       .end = BCM_INT_ID_PMU_IRQ0,
       .flags = IORESOURCE_IRQ,
};

static struct platform_device pmu_device = {
       .name = "arm-pmu",
       .id   = ARM_PMU_DEVICE_CPU,
       .resource = &pmu_resource,
       .num_resources = 1,
};

#ifdef CONFIG_USB_DWC_OTG
static struct resource kona_hsotgctrl_platform_resource[] = {
	[0] = {
		.start = HSOTG_CTRL_BASE_ADDR,
		.end = HSOTG_CTRL_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = CHIPREGS_BASE_ADDR,
		.end = CHIPREGS_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	[2] = {
		.start = HUB_CLK_BASE_ADDR,
		.end = HUB_CLK_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct platform_device board_kona_hsotgctrl_platform_device =
{
	.name = "bcm_hsotgctrl",
	.id = -1,
	.resource = kona_hsotgctrl_platform_resource,
	.num_resources = ARRAY_SIZE(kona_hsotgctrl_platform_resource),
};

static struct resource kona_otg_platform_resource[] = {
	[0] = { /* Keep HSOTG_BASE_ADDR as first IORESOURCE_MEM to be compatible with legacy code */
		.start = HSOTG_BASE_ADDR,
		.end = HSOTG_BASE_ADDR + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_USB_HSOTG,
		.end = BCM_INT_ID_USB_HSOTG,
		.flags = IORESOURCE_IRQ,
	},
};

static struct platform_device board_kona_otg_platform_device =
{
	.name = "dwc_otg",
	.id = -1,
	.resource = kona_otg_platform_resource,
	.num_resources = ARRAY_SIZE(kona_otg_platform_resource),
};
#endif

#ifdef CONFIG_KONA_CPU_FREQ_DRV
struct kona_freq_tbl kona_freq_tbl[] =
{
#ifndef CONFIG_RHEA_A0_PM_ASIC_WORKAROUND
//    FTBL_INIT(156000000, PI_OPP_ECONOMY),     /*island has this issue too, will be remove in capri */
#endif
    FTBL_INIT(467000, PI_OPP_NORMAL),
    FTBL_INIT(700000, PI_OPP_TURBO),
};

struct kona_cpufreq_drv_pdata kona_cpufreq_drv_pdata = {

	.flags = KONA_CPUFREQ_UPDATE_LPJ,
    .freq_tbl = kona_freq_tbl,
	.num_freqs = ARRAY_SIZE(kona_freq_tbl),
	/*FIX ME: To be changed according to the cpu latency*/
	.latency = 10*1000,
	.pi_id = PI_MGR_PI_ID_ARM_CORE,
};

static struct platform_device kona_cpufreq_device = {
	.name    = "kona-cpufreq-drv",
	.id      = -1,
	.dev = {
		.platform_data		= &kona_cpufreq_drv_pdata,
	},
};
#endif /*CONFIG_KONA_CPU_FREQ_DRV*/

#ifdef CONFIG_SENSORS_KONA
static struct resource board_tmon_resource[] = {
	{	/* For Current Temperature */
		.start = TMON_BASE_ADDR,
		.end = TMON_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{	/* For Temperature IRQ */
		.start = BCM_INT_ID_TEMP_MON,
		.end = BCM_INT_ID_TEMP_MON,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device tmon_device = {
	.name = "kona-tmon",
	.id = -1,
	.resource = board_tmon_resource,
	.num_resources = ARRAY_SIZE(board_tmon_resource),
};
#endif

#ifdef CONFIG_DMAC_PL330
static struct kona_pl330_data island_pl330_pdata =	{
	/* Non Secure DMAC virtual base address */
	.dmac_ns_base = KONA_DMAC_NS_VA,
	/* Secure DMAC virtual base address */
	.dmac_s_base = KONA_DMAC_S_VA,
	/* # of PL330 dmac channels 'configurable' */
	.num_pl330_chans = 8,
	/* irq number to use */
	.irq_base = BCM_INT_ID_RESERVED184,
	/* # of PL330 Interrupt lines connected to GIC */
	.irq_line_count = 8,
};

static struct platform_device pl330_dmac_device = {
	.name = "kona-dmac-pl330",
	.id = 0,
	.dev = {
		.platform_data = &island_pl330_pdata,
		.coherent_dma_mask  = DMA_BIT_MASK(64),
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_BRCM_SPUM_HASH)
static struct resource board_spum_resource[] = {
       [0] =
       {
               .start  =       SEC_SPUM_NS_APB_BASE_ADDR,
               .end    =       SEC_SPUM_NS_APB_BASE_ADDR + SZ_64K - 1,
               .flags  =       IORESOURCE_MEM,
       },
       [1] =
       {
               .start  =       SPUM_NS_BASE_ADDR,
               .end    =       SPUM_NS_BASE_ADDR + SZ_64K - 1,
               .flags  =       IORESOURCE_MEM,
       }
};

static struct platform_device board_spum_device = {
       .name           =       "brcm-spum",
       .id             =       0,
       .resource       =       board_spum_resource,
       .num_resources  =       ARRAY_SIZE(board_spum_resource),
};
#endif

#if defined(CONFIG_CRYPTO_DEV_BRCM_SPUM_AES)
static struct resource board_spum_aes_resource[] = {
	[0] =
	{
		.start  =       SEC_SPUM_NS_APB_BASE_ADDR,
		.end    =       SEC_SPUM_NS_APB_BASE_ADDR + SZ_64K - 1,
		.flags  =       IORESOURCE_MEM,
	},
	[1] =
	{
		.start  =       SPUM_NS_BASE_ADDR,
		.end    =       SPUM_NS_BASE_ADDR + SZ_64K - 1,
		.flags  =       IORESOURCE_MEM,
	}
};

static struct platform_device board_spum_aes_device = {
	.name           =       "brcm-spum-aes",
	.id             =       0,
	.resource       =       board_spum_aes_resource,
	.num_resources  =       ARRAY_SIZE(board_spum_aes_resource),
};
#endif

/* Common devices among all Island boards */
static struct platform_device *board_common_plat_devices[] __initdata = {
	&board_serial_device,
#if defined(CONFIG_MPCORE_WATCHDOG)
        &wdt_device,
#endif
#if defined(CONFIG_W1_MASTER_DS1WM)
        &d1w_device,
#endif
#if defined(CONFIG_HW_RANDOM_KONA)
        &rng_device,
#endif
#if defined(CONFIG_RTC_DRV_ISLAND)
        &rtc_device,
#endif
#if defined(CONFIG_MTD_BCMNAND)
        &nand_device,
#endif
#if defined(CONFIG_KONA_PWMC)
        &pwm_device,
#endif

#ifdef CONFIG_STM_TRACE
	&kona_stm_device,
#endif
	&pmu_device,

#ifdef CONFIG_USB_DWC_OTG
	&board_kona_hsotgctrl_platform_device,
	&board_kona_otg_platform_device,
#endif

#if defined(CONFIG_BACKLIGHT_PWM)
	&pwm_backlight_device,
#endif
#if defined(CONFIG_USB_ANDROID)
        &android_usb_device,
#if defined(CONFIG_USB_ANDROID_MASS_STORAGE)
        &usb_mass_storage_device,
#endif /* CONFIG_USB_ANDROID_MASS_STORAGE */
#if defined(CONFIG_USB_ANDROID_RNDIS)
        &rndis_device,
#endif /* CONFIG_USB_ANDROID_RNDIS */
#endif /* CONFIG_USB_ANDROID */

#ifdef CONFIG_KONA_CPU_FREQ_DRV
	&kona_cpufreq_device,
#endif
#ifdef CONFIG_SENSORS_KONA
	&tmon_device,
#endif

#ifdef CONFIG_DMAC_PL330
	&pl330_dmac_device,
#endif

#ifdef CONFIG_CRYPTO_DEV_BRCM_SPUM_HASH
	&board_spum_device,
#endif

#ifdef CONFIG_CRYPTO_DEV_BRCM_SPUM_AES
    &board_spum_aes_device,
#endif
};

void __init board_add_common_devices(void)
{
	platform_add_devices(board_common_plat_devices,
			ARRAY_SIZE(board_common_plat_devices));
}


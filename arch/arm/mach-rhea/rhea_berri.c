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
#include <linux/version.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel_stat.h>
#include <asm/mach/arch.h>
#include <asm/mach-types.h>
#include <mach/hardware.h>
#include <linux/i2c.h>
#include <linux/i2c-kona.h>
#include <linux/gpio_keys.h>
#include <linux/input.h>
#include <asm/gpio.h>
#ifdef CONFIG_GPIO_PCA953X
#include <linux/i2c/pca953x.h>
#endif
#ifdef CONFIG_TOUCHSCREEN_QT602240
#include <linux/i2c/qt602240_ts.h>
#endif
#ifdef CONFIG_REGULATOR_TPS728XX
#include <linux/regulator/tps728xx.h>
#endif
#include <mach/kona_headset_pd.h>
#include <mach/kona.h>
#include <mach/rhea.h>
#include <asm/mach/map.h>
#include <linux/power_supply.h>
#include <linux/mfd/bcm590xx/core.h>
#include <linux/mfd/bcm590xx/pmic.h>
#include <linux/mfd/bcm590xx/bcm59055_A0.h>
#include <linux/broadcom/bcm59055-power.h>
#include <linux/clk.h>
#include <linux/bootmem.h>
#include "common.h"
#include <mach/sdio_platform.h>

#ifdef CONFIG_KEYBOARD_BCM
#include <mach/bcm_keypad.h>
#endif
#ifdef CONFIG_KEYBOARD_LM8325
#include <linux/i2c/lm8325.h>
#endif
#ifdef CONFIG_DMAC_PL330
#include <mach/irqs.h>
#include <plat/pl330-pdata.h>
#include <linux/dma-mapping.h>
#endif
#include <linux/spi/spi.h>
#if defined (CONFIG_HAPTIC)
#include <linux/haptic.h>
#endif

#define _RHEA_
#include <mach/comms/platform_mconfig.h>


#if (defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE))
#include <linux/broadcom/bcmbt_rfkill.h>
#endif

#ifdef CONFIG_BCM_BT_LPM
#include <linux/broadcom/bcmbt_lpm.h
#endif

#ifdef CONFIG_BACKLIGHT_PWM
#include <linux/pwm_backlight.h>
#endif

#ifdef CONFIG_FB_BRCM_RHEA
#include <video/kona_fb.h>
#endif

#define PMU_DEVICE_I2C_ADDR_0   0x08
#define PMU_IRQ_PIN           29


#ifdef CONFIG_GPIO_PCA953X
#define SD_CARDDET_GPIO_PIN      (KONA_MAX_GPIO + 15)
#else
#ifdef CONFIG_MACH_RHEA_BERRI
/* RheaBerri edn33: GPIO8 is card detect gpio */
#define SD_CARDDET_GPIO_PIN      8
#else
/* RheaBerri edn40:  GPIO75 is card detect gpio */
#define SD_CARDDET_GPIO_PIN      75
#endif
#endif


// keypad map
#define BCM_KEY_ROW_0  0
#define BCM_KEY_ROW_1  1
#define BCM_KEY_ROW_2  2
#define BCM_KEY_ROW_3  3
#define BCM_KEY_ROW_4  4
#define BCM_KEY_ROW_5  5
#define BCM_KEY_ROW_6  6
#define BCM_KEY_ROW_7  7

#define BCM_KEY_COL_0  0
#define BCM_KEY_COL_1  1
#define BCM_KEY_COL_2  2
#define BCM_KEY_COL_3  3
#define BCM_KEY_COL_4  4
#define BCM_KEY_COL_5  5
#define BCM_KEY_COL_6  6
#define BCM_KEY_COL_7  7

#ifdef CONFIG_MFD_BCM_PMU590XX
static int bcm590xx_event_callback(int flag, int param)
{
	int ret;
	printk("REG: pmu_init_platform_hw called \n") ;
	switch (flag) {
	case BCM590XX_INITIALIZATION:
		ret = gpio_request(PMU_IRQ_PIN, "pmu_irq");
		if (ret < 0) {
			printk(KERN_ERR "%s unable to request GPIO pin %d\n", __FUNCTION__, PMU_IRQ_PIN);
			return ret ;
		}
		gpio_direction_input(PMU_IRQ_PIN);
		break;
	case BCM590XX_CHARGER_INSERT:
		pr_info("%s: BCM590XX_CHARGER_INSERT\n", __func__);
		break;
	default:
		return -EPERM;
	}
	return 0 ;
}

#ifdef CONFIG_BATTERY_BCM59055
static struct bcm590xx_battery_pdata bcm590xx_battery_plat_data = {
        .batt_min_volt = 3200,
        .batt_max_volt = 4200,
        .batt_technology = POWER_SUPPLY_TECHNOLOGY_LION,
        .usb_cc = CURRENT_500_MA,
        .wac_cc = CURRENT_900_MA,
        /* 1500mA = 5400 coloumb
         * 1Ah = 3600 coloumb
        */
        .batt_max_capacity = 5400,
};
#endif

#ifdef CONFIG_REGULATOR_BCM_PMU59055
/* Regulator registration */
struct regulator_consumer_supply sim_supply[] = {
	{ .supply = "sim_vcc" },
	{ .supply = "simldo_uc" },
};

static struct regulator_init_data bcm59055_simldo_data = {
	.constraints = {
		.name = "simldo",
		.min_uV = 1300000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_VOLTAGE,
		.always_on = 1,
		.boot_on = 0,
	},
	.num_consumer_supplies = ARRAY_SIZE(sim_supply),
	.consumer_supplies = sim_supply,
};

static struct bcm590xx_regulator_init_data bcm59055_regulators[] =
{
	{BCM59055_SIMLDO, &bcm59055_simldo_data, BCM590XX_REGL_LPM_IN_DSM},
};

static struct bcm590xx_regulator_pdata bcm59055_regl_pdata = {
	.num_regulator	= ARRAY_SIZE(bcm59055_regulators),
	.init			= bcm59055_regulators,
	.default_pmmode = {
		[BCM59055_RFLDO]	= 0x00,
		[BCM59055_CAMLDO] 	= 0x00,
		[BCM59055_HV1LDO]	= 0x00,
		[BCM59055_HV2LDO]	= 0x00,
		[BCM59055_HV3LDO]	= 0x00,
		[BCM59055_HV4LDO]	= 0x00,
		[BCM59055_HV5LDO]	= 0x00,
		[BCM59055_HV6LDO]	= 0x00,
		[BCM59055_HV7LDO]	= 0x00,
		[BCM59055_SIMLDO]	= 0x00,
		[BCM59055_CSR]		= 0x00,
		[BCM59055_IOSR]		= 0x00,
		[BCM59055_SDSR]		= 0x00,
	},
};



/* Register userspace and virtual consumer for SIMLDO */
#ifdef CONFIG_REGULATOR_USERSPACE_CONSUMER
static struct regulator_bulk_data bcm59055_bd_sim = {
	.supply = "simldo_uc",
};

static struct regulator_userspace_consumer_data bcm59055_uc_data_sim = {
	.name = "simldo",
	.num_supplies = 1,
	.supplies = &bcm59055_bd_sim,
	.init_on = 0
};

static struct platform_device bcm59055_uc_device_sim = {
	.name = "reg-userspace-consumer",
	.id = BCM59055_SIMLDO,
	.dev = {
		.platform_data = &bcm59055_uc_data_sim,
	},
};
#endif
#ifdef CONFIG_REGULATOR_VIRTUAL_CONSUMER
static struct platform_device bcm59055_vc_device_sim = {
	.name = "reg-virt-consumer",
	.id = BCM59055_SIMLDO,
	.dev = {
		.platform_data = "simldo_uc"
	},
};
#endif
#endif

static const char *pmu_clients[] = {
#ifdef CONFIG_BCM59055_WATCHDOG
	"bcm59055-wdog",
#endif
	"bcmpmu_usb",
#ifdef CONFIG_INPUT_BCM59055_ONKEY
	"bcm590xx-onkey",
#endif
#ifdef CONFIG_BCM59055_FUELGAUGE
	"bcm590xx-fg",
#endif
#ifdef CONFIG_BCM59055_SARADC
	"bcm590xx-saradc",
#endif
#ifdef CONFIG_REGULATOR_BCM_PMU59055
	"bcm590xx-regulator",
#endif
#ifdef CONFIG_BCM59055_AUDIO
	"bcm590xx-audio",
#endif
#ifdef CONFIG_RTC_DRV_BCM59055
	"bcm59055-rtc",
#endif
#ifdef CONFIG_BATTERY_BCM59055
	"bcm590xx-power",
#endif
#ifdef CONFIG_BCM59055_ADC_CHIPSET_API
	"bcm59055-adc_chipset_api",
#endif
#ifdef CONFIG_BCMPMU_OTG_XCEIV
	"bcmpmu_otg_xceiv",
#endif
#ifdef CONFIG_BCM59055_SELFTEST
       "bcm59055-selftest",
#endif
};

static struct bcm590xx_platform_data bcm590xx_plat_data = {
#ifdef CONFIG_KONA_PMU_BSC_HS_MODE
    /*
     * PMU in High Speed (HS) mode. I2C CLK is 3.25MHz
     * derived from 26MHz input clock.
     *
     * Rhea: PMBSC is always in HS mode, i2c_pdata is not in use.
     */
    .i2c_pdata  = ADD_I2C_SLAVE_SPEED(BSC_BUS_SPEED_HS),
#else
    .i2c_pdata  = ADD_I2C_SLAVE_SPEED(BSC_BUS_SPEED_400K),
#endif
	.pmu_event_cb = bcm590xx_event_callback,
#ifdef CONFIG_BATTERY_BCM59055
	.battery_pdata = &bcm590xx_battery_plat_data,
#endif
#ifdef CONFIG_REGULATOR_BCM_PMU59055
	.regl_pdata = &bcm59055_regl_pdata,
#endif
	.clients = pmu_clients,
	.clients_num = ARRAY_SIZE(pmu_clients),
};


static struct i2c_board_info __initdata pmu_info[] =
{
	{
		I2C_BOARD_INFO("bcm59055", PMU_DEVICE_I2C_ADDR_0 ),
		.irq = gpio_to_irq(PMU_IRQ_PIN),
		.platform_data  = &bcm590xx_plat_data,
	},
};
#endif

#ifdef CONFIG_KEYBOARD_BCM
/*!
 * The keyboard definition structure.
 */
struct platform_device bcm_kp_device = {
	.name = "bcm_keypad",
	.id = -1,
};

/*	Keymap for Ray board plug-in 64-key keypad.
	Since LCD block has used pin GPIO00, GPIO01, GPIO02, GPIO03,
	GPIO08, GPIO09, GPIO10 and GPIO11, SSP3 and camera used GPIO06,
	GPIO07, GPIO12, GPIO13, for now keypad can only be set as a 2x2 matrix
	by using pin GPIO04, GPIO05, GPIO14 and GPIO15 */
static struct bcm_keymap newKeymap[] = {
	{BCM_KEY_ROW_0, BCM_KEY_COL_0, "Search Key", KEY_SEARCH},
	{BCM_KEY_ROW_0, BCM_KEY_COL_1, "Back Key", KEY_BACK},
	{BCM_KEY_ROW_0, BCM_KEY_COL_2, "Menu-Key", KEY_MENU},
	{BCM_KEY_ROW_0, BCM_KEY_COL_3, "Home-Key", KEY_HOME},
	{BCM_KEY_ROW_0, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_0, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_1, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_2, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_3, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_4, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_5, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_6, BCM_KEY_COL_7, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_0, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_1, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_2, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_3, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_4, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_5, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_6, "unused", 0},
	{BCM_KEY_ROW_7, BCM_KEY_COL_7, "unused", 0},
};

static struct bcm_keypad_platform_info bcm_keypad_data = {
	.row_num = 8,
	.col_num = 8,
	.keymap = newKeymap,
	.bcm_keypad_base = (void *)__iomem HW_IO_PHYS_TO_VIRT(KEYPAD_BASE_ADDR),
};

#endif

#ifdef CONFIG_KEYBOARD_LM8325
static s16 rheaberri_keymap[] = {
	[0x40] = KEY_MENU,
	[0x50] = KEY_SEND,
	[0x05] = KEY_Q,
	[0x04] = KEY_W,
	[0x03] = KEY_E,
	[0x02] = KEY_R,
	[0x01] = KEY_T,
	[0x00] = KEY_Y,
	[0x45] = KEY_U,
	[0x44] = KEY_I,
	[0x43] = KEY_O,
	[0x42] = KEY_P,
	[0x15] = KEY_A,
	[0x14] = KEY_S,
	[0x13] = KEY_D,
	[0x12] = KEY_F,
	[0x11] = KEY_G,
	[0x10] = KEY_H,
	[0x55] = KEY_J,
	[0x54] = KEY_K,
	[0x53] = KEY_L,
	[0x52] = KEY_BACKSPACE,
	[0x25] = KEY_LEFTSHIFT,
	[0x24] = KEY_Z,
	[0x23] = KEY_X,
	[0x22] = KEY_C,
	[0x21] = KEY_V,
	[0x20] = KEY_B,
	[0x65] = KEY_N,
	[0x64] = KEY_M,
	[0x63] = KEY_COMMA,
	[0x62] = KEY_ENTER,
	[0x35] = KEY_BACK,
	[0x34] = KEY_HOME,
	[0x33] = KEY_OK,
	[0x32] = KEY_TAB,
	[0x31] = KEY_SPACE,
	[0x30] = KEY_COMMA,
	[0x41] = KEY_DOT,
	[0x51] = KEY_SEMICOLON,
	[0x61] = KEY_CAPSLOCK,
	[0x26] = KEY_VOLUMEUP,
	[0x36] = KEY_VOLUMEDOWN,
	[0x16] = KEY_CAMERA,
};

static struct lm8325_platform_data lm8325_pdata = {
	/* This client supports both 100k and 400k, but we are setting it to
	 * 100k */
	.i2c_pdata = {
		      ADD_I2C_SLAVE_SPEED(BSC_BUS_SPEED_400K),
		      SET_CLIENT_FUNC(TIMEOUT_DISABLE),
		      },
	.size_x = 8,
	.size_y = 8,
	.debounce_time = 0x80,
	.settle_time = 0x80,
	.iocfg = 0xF8,
	.autosleep = 0,
	.keymap = rheaberri_keymap,
	.repeat = 1,
	.name = "rheaberri_keyboard",
};
static struct i2c_board_info __initdata lm8325_info[] = {
	{
	    /* The client bus address is 0x88. But since the I2C spec
	     * specifies that only the bits [7:1] are used for the
	     * transaction, we will be passing 0x44. The I2C core driver in
	     * turn will left this this once and then OR it with the R/W bit
	     * before the transaction */
	    I2C_BOARD_INFO("lm8325_kp", 0x44),
	    .platform_data = &lm8325_pdata,
	    /* .flags = I2C_M_TEN, */ /* To be used if ten bit addressing is
	    required*/
	    .irq = gpio_to_irq(121),
	},
};
#endif
#ifdef CONFIG_GPIO_PCA953X

#ifdef CONFIG_MACH_RHEA_RAY_EDN1X
#define GPIO_PCA953X_GPIO_PIN      121 /* Configure pad MMC1DAT4 to GPIO74 */
#else
#define GPIO_PCA953X_GPIO_PIN      74 /* Configure pad MMC1DAT4 to GPIO74 */
#endif

static int pca953x_platform_init_hw(struct i2c_client *client,
		unsigned gpio, unsigned ngpio, void *context)
{
	int rc;
	rc = gpio_request(GPIO_PCA953X_GPIO_PIN, "gpio_expander");
	if (rc < 0)
	{
		printk(KERN_ERR "unable to request GPIO pin %d\n", GPIO_PCA953X_GPIO_PIN);
		return rc;
	}
	gpio_direction_input(GPIO_PCA953X_GPIO_PIN);
	return 0;
}

static int pca953x_platform_exit_hw(struct i2c_client *client,
		unsigned gpio, unsigned ngpio, void *context)
{
	gpio_free(GPIO_PCA953X_GPIO_PIN);
	return 0;
}

static struct pca953x_platform_data board_expander_info = {
	.i2c_pdata	= ADD_I2C_SLAVE_SPEED(BSC_BUS_SPEED_100K),
	.gpio_base	= KONA_MAX_GPIO,
	.irq_base	= gpio_to_irq(KONA_MAX_GPIO),
	.setup		= pca953x_platform_init_hw,
	.teardown	= pca953x_platform_exit_hw,
};

static struct i2c_board_info __initdata pca953x_info[] = {
	{
		I2C_BOARD_INFO("pca9539", 0x74),
		.irq = gpio_to_irq(GPIO_PCA953X_GPIO_PIN),
		.platform_data = &board_expander_info,
	},
};
#endif /* CONFIG_GPIO_PCA953X */

#ifdef CONFIG_TOUCHSCREEN_QT602240
#ifdef CONFIG_GPIO_PCA953X
#define QT602240_INT_GPIO_PIN      (KONA_MAX_GPIO + 8)
#else
#define QT602240_INT_GPIO_PIN      4 /* skip expander chip */
#endif
static int qt602240_platform_init_hw(void)
{
	int rc;
	rc = gpio_request(QT602240_INT_GPIO_PIN, "ts_qt602240");
	if (rc < 0)
	{
		printk(KERN_ERR "unable to request GPIO pin %d\n", QT602240_INT_GPIO_PIN);
		return rc;
	}
	gpio_direction_input(QT602240_INT_GPIO_PIN);

	return 0;
}

static void qt602240_platform_exit_hw(void)
{
	gpio_free(QT602240_INT_GPIO_PIN);
}

static struct qt602240_platform_data qt602240_platform_data = {
	.i2c_pdata	= ADD_I2C_SLAVE_SPEED(BSC_BUS_SPEED_100K),
	.x_line		= 15,
	.y_line		= 11,
	.x_size		= 1023,
	.y_size		= 1023,
	.x_min		= 90,
	.y_min		= 90,
	.x_max		= 0x3ff,
	.y_max		= 0x3ff,
	.max_area	= 0xff,
	.blen		= 33,
	.threshold	= 70,
	.voltage	= 2700000,              /* 2.8V */
	.orient		= QT602240_DIAGONAL_COUNTER,
	.init_platform_hw = qt602240_platform_init_hw,
	.exit_platform_hw = qt602240_platform_exit_hw,
};

static struct i2c_board_info __initdata qt602240_info[] = {
	{
		I2C_BOARD_INFO("qt602240_ts", 0x4a),
		.platform_data = &qt602240_platform_data,
		.irq = gpio_to_irq(QT602240_INT_GPIO_PIN),
	},
};
#endif /* CONFIG_TOUCHSCREEN_QT602240 */

#ifdef CONFIG_KONA_HEADSET
#define HS_IRQ	gpio_to_irq(71)
#define HSB_IRQ	BCM_INT_ID_AUXMIC_COMP1
static struct kona_headset_pd headset_data = {
	.hs_default_state = 1, /* GPIO state read is 0 on HS insert and 1 for
							* HS remove*/
};

static struct resource board_headset_resource[] = {
	{	/* For AUXMIC */
		.start = AUXMIC_BASE_ADDR,
		.end = AUXMIC_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{	/* For ACI */
		.start = ACI_BASE_ADDR,
		.end = ACI_BASE_ADDR + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
	{	/* For Headset IRQ */
		.start = HS_IRQ,
		.end = HS_IRQ,
		.flags = IORESOURCE_IRQ,
	},
	{	/* For Headset button IRQ */
		.start = HSB_IRQ,
		.end = HSB_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

struct platform_device headset_device = {
	.name = "konaheadset",
	.id = -1,
	.resource = board_headset_resource,
	.num_resources	= ARRAY_SIZE(board_headset_resource),
	.dev	=	{
		.platform_data = &headset_data,
	},
};
#endif /* CONFIG_KONA_HEADSET */

#ifdef CONFIG_DMAC_PL330
static struct kona_pl330_data rhea_pl330_pdata =	{
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
		.platform_data = &rhea_pl330_pdata,
		.coherent_dma_mask  = DMA_BIT_MASK(64),
	},
};
#endif

#if (defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE))

#define BCMBT_VREG_GPIO       (10)
#define BCMBT_N_RESET_GPIO    (70)
#define BCMBT_AUX0_GPIO        (-1)   /* clk32 */
#define BCMBT_AUX1_GPIO        (-1)    /* UARTB_SEL */

static struct bcmbt_rfkill_platform_data board_bcmbt_rfkill_cfg = {
        .vreg_gpio = BCMBT_VREG_GPIO,
        .n_reset_gpio = BCMBT_N_RESET_GPIO,
        .aux0_gpio = BCMBT_AUX0_GPIO,  /* CLK32 */
        .aux1_gpio = BCMBT_AUX1_GPIO,  /* UARTB_SEL, probably not required */
};

static struct platform_device board_bcmbt_rfkill_device = {
        .name = "bcmbt-rfkill",
        .id = -1,
        .dev =
        {
                .platform_data=&board_bcmbt_rfkill_cfg,
        },
};
#endif

#ifdef CONFIG_BCM_BT_LPM
#define GPIO_BT_WAKE 02
#define GPIO_HOST_WAKE 111

static struct bcm_bt_lpm_platform_data brcm_bt_lpm_data = {
        .gpio_bt_wake = GPIO_BT_WAKE,
        .gpio_host_wake = GPIO_HOST_WAKE,
};

static struct platform_device board_bcmbt_lpm_device = {
        .name = "bcmbt-lpm",
        .id = -1,
        .dev =
        {
                .platform_data=&brcm_bt_lpm_data,
        },
};
#endif




/*
 * SPI board info for the slaves
 */
static struct spi_board_info spi_slave_board_info[] __initdata = {
#ifdef CONFIG_SPI_SPIDEV
	{
	 .modalias = "spidev",	/* use spidev generic driver */
	 .max_speed_hz = 13000000,	/* use max speed */
	 .bus_num = 0,		/* framework bus number */
	 .chip_select = 0,	/* for each slave */
	 .platform_data = NULL,	/* no spi_driver specific */
	 .irq = 0,		/* IRQ for this device */
	 .mode = SPI_LOOP,	/* SPI mode 0 */
	 },
#endif
	/* TODO: adding more slaves here */
};


#if defined (CONFIG_HAPTIC_SAMSUNG_PWM)
void haptic_gpio_setup(void)
{
	/* Board specific configuration like pin mux & GPIO */
}

static struct haptic_platform_data haptic_control_data = {
	/* Haptic device name: can be device-specific name like ISA1000 */
	.name = "pwm_vibra",
	/* PWM interface name to request */
	.pwm_name = "kona_pwmc:4",
	/* Invalid gpio for now, pass valid gpio number if connected */
	.gpio = ARCH_NR_GPIOS,
	.setup_pin = haptic_gpio_setup,
};

struct platform_device haptic_pwm_device = {
	.name   = "samsung_pwm_haptic",
	.id     = -1,
	.dev	=	 {	.platform_data = &haptic_control_data,}
};

#endif /* CONFIG_HAPTIC_SAMSUNG_PWM */

#if 1 //Shri

static struct resource board_sdio0_resource[] = {
	[0] = {
		.start = SDIO1_BASE_ADDR,
		.end = SDIO1_BASE_ADDR + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_SDIO0,
		.end = BCM_INT_ID_SDIO0,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource board_sdio1_resource[] = {
	[0] = {
		.start = SDIO2_BASE_ADDR,
		.end = SDIO2_BASE_ADDR + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_SDIO1,
		.end = BCM_INT_ID_SDIO1,
		.flags = IORESOURCE_IRQ,
	},
};

static struct resource board_sdio2_resource[] = {
	[0] = {
		.start = SDIO3_BASE_ADDR,
		.end = SDIO3_BASE_ADDR + SZ_64K - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = BCM_INT_ID_SDIO_NAND,
		.end = BCM_INT_ID_SDIO_NAND,
		.flags = IORESOURCE_IRQ,
	},
};
static struct sdio_platform_cfg board_sdio_param[] = {
	{ /* SDIO0 */
		.id = 0,
		.data_pullup = 0,
		.cd_gpio = SD_CARDDET_GPIO_PIN,
		.wp_gpio = -1,
		.devtype = SDIO_DEV_TYPE_SDMMC,
		.flags = KONA_SDIO_FLAGS_DEVICE_REMOVABLE,
		.peri_clk_name = "sdio1_clk",
		.ahb_clk_name = "sdio1_ahb_clk",
		.sleep_clk_name = "sdio1_sleep_clk",
		.peri_clk_rate = 48000000,
		/* VDD_SDC regulator: we enable it once PMU regulator
		 * code is in-place.
		 */
		/*
		.need_regulator_uhs = 1,
		.vddo_regulator_name = "vdd_sdio",
		*/
	},
	{ /* SDIO1 */
		.id = 1,
		.data_pullup = 0,
		.is_8bit = 1,
		.devtype = SDIO_DEV_TYPE_EMMC,
		.flags = KONA_SDIO_FLAGS_DEVICE_NON_REMOVABLE ,
		.peri_clk_name = "sdio2_clk",
		.ahb_clk_name = "sdio2_ahb_clk",
		.sleep_clk_name = "sdio2_sleep_clk",
		.peri_clk_rate = 52000000,
	},
	{ /* SDIO2 */
		.id = 2,
		.data_pullup = 0,
		.devtype = SDIO_DEV_TYPE_WIFI,
		.wifi_gpio = {
			.reset		= 38,
			.reg		= -1,
			.host_wake	= 1,
			.shutdown	= -1,
		},
		.flags = KONA_SDIO_FLAGS_DEVICE_NON_REMOVABLE,
		.peri_clk_name = "sdio3_clk",
		.ahb_clk_name = "sdio3_ahb_clk",
		.sleep_clk_name = "sdio3_sleep_clk",
		.peri_clk_rate = 48000000,
	},
};

static struct platform_device board_sdio0_device = {
	.name = "sdhci",
	.id = 0,
	.resource = board_sdio0_resource,
	.num_resources   = ARRAY_SIZE(board_sdio0_resource),
	.dev      = {
		.platform_data = &board_sdio_param[0],
	},
};

static struct platform_device board_sdio1_device = {
	.name = "sdhci",
	.id = 1,
	.resource = board_sdio1_resource,
	.num_resources   = ARRAY_SIZE(board_sdio1_resource),
	.dev      = {
		.platform_data = &board_sdio_param[1],
	},
};

static struct platform_device board_sdio2_device = {
	.name = "sdhci",
	.id = 2,
	.resource = board_sdio2_resource,
	.num_resources   = ARRAY_SIZE(board_sdio2_resource),
	.dev      = {
		.platform_data = &board_sdio_param[2],
	},
};


/* Common devices among all the Rhea boards (Rhea Ray, Rhea Berri, etc.) */
static struct platform_device *board_sdio_plat_devices[] __initdata = {
	&board_sdio1_device,
	&board_sdio2_device,
	&board_sdio0_device,
};

void __init board_add_sdio_devices(void)
{
	platform_add_devices(board_sdio_plat_devices, ARRAY_SIZE(board_sdio_plat_devices));
}


#endif //Shri

#ifdef CONFIG_BACKLIGHT_PWM

static struct platform_pwm_backlight_data bcm_backlight_data = {
/* backlight */
	.pwm_name 	= "kona_pwmc:4",
	.max_brightness = 32,   /* Android calibrates to 32 levels*/
	.dft_brightness = 32,
	.polarity       = 1,    /* Inverted polarity */
	.pwm_period_ns 	=  5000000,
};

static struct platform_device bcm_backlight_devices = {
	.name 	= "pwm-backlight",
	.id 	= 0,
	.dev 	= {
		.platform_data  =       &bcm_backlight_data,
	},
};

#endif /*CONFIG_BACKLIGHT_PWM */



#if defined (CONFIG_REGULATOR_TPS728XX)
#define GPIO_SIM2LDO_EN		99

#ifdef CONFIG_GPIO_PCA953X
#define GPIO_SIM2LDOVSET	(KONA_MAX_GPIO + 7)
#else
#define GPIO_SIM2LDOVSET	37
#endif
#define TPS728XX_REGL_ID        (BCM59055_MAX_LDO + 0)
struct regulator_consumer_supply sim2_supply[] = {
	{ .supply = "sim2_vcc" },
	{ .supply = "sim2ldo_uc" },
};

static struct regulator_init_data tps728xx_regl_initdata = {
	.constraints = {
		.name = "sim2ldo",
		.min_uV = 1300000,
		.max_uV = 3300000,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS |
			REGULATOR_CHANGE_VOLTAGE,
		.always_on = 0,
		.boot_on = 0,
	},
	.num_consumer_supplies = ARRAY_SIZE(sim2_supply),
	.consumer_supplies = sim2_supply,
};

struct tps728xx_plat_data tps728xx_data = {
	.gpio_vset	= GPIO_SIM2LDOVSET,
	.gpio_en	= GPIO_SIM2LDO_EN,
	.vout0		= 1800000,
	.vout1		= 3000000,
	.initdata	= &tps728xx_regl_initdata,
};

struct platform_device tps728xx_device = {
	.name = "tps728xx-regulator",
	.id = -1,
	.dev	=	{
		.platform_data = &tps728xx_data,
	},
};

/* Register userspace and virtual consumer for SIM2LDO */
#ifdef CONFIG_REGULATOR_USERSPACE_CONSUMER
static struct regulator_bulk_data tps728xx_bd_sim2 = {
	.supply = "sim2ldo_uc",
};

static struct regulator_userspace_consumer_data tps728xx_uc_data_sim2 = {
	.name = "sim2ldo",
	.num_supplies = 1,
	.supplies = &tps728xx_bd_sim2,
	.init_on = 0
};

static struct platform_device tps728xx_uc_device_sim2 = {
	.name = "reg-userspace-consumer",
	.id = TPS728XX_REGL_ID,
	.dev = {
		.platform_data = &tps728xx_uc_data_sim2,
	},
};
#endif
#ifdef CONFIG_REGULATOR_VIRTUAL_CONSUMER
static struct platform_device tps728xx_vc_device_sim2 = {
	.name = "reg-virt-consumer",
	.id = TPS728XX_REGL_ID,
	.dev = {
		.platform_data = "sim2ldo_uc"
	},
};
#endif
#endif /* CONFIG_REGULATOR_TPS728XX*/

#ifdef CONFIG_FB_BRCM_RHEA
static struct kona_fb_platform_data alex_dsi_display_fb_data = {
	.get_dispdrv_func_tbl	= &DISP_DRV_BCM91008_ALEX_GetFuncTable,
	.screen_width		= 360,
	.screen_height		= 640,
	.bytes_per_pixel	= 4,
	.gpio			= 41,
	.pixel_format		= XRGB8888,
};

static struct platform_device alex_dsi_display_device = {
	.name    = "rhea_fb",
	.id      = 0,
	.dev = {
		.platform_data		= &alex_dsi_display_fb_data,
		.dma_mask		= (u64 *) ~(u32)0,
		.coherent_dma_mask	= ~(u32)0,
	},
};

static struct kona_fb_platform_data nt35582_smi_display_fb_data = {
	.get_dispdrv_func_tbl	= &DISP_DRV_NT35582_WVGA_SMI_GetFuncTable,
	.screen_width		= 480,
	.screen_height		= 800,
	.bytes_per_pixel	= 2,
	.gpio			= 41,
	.pixel_format		= RGB565,
};

static struct platform_device nt35582_smi_display_device = {
	.name    = "rhea_fb",
	.id      = 1,
	.dev = {
		.platform_data		= &nt35582_smi_display_fb_data,
		.dma_mask		= (u64 *) ~(u32)0,
		.coherent_dma_mask	= ~(u32)0,
	},
};

static struct kona_fb_platform_data r61581_smi_display_fb_data = {
	.get_dispdrv_func_tbl	= &DISP_DRV_R61581_HVGA_SMI_GetFuncTable,
	.screen_width		= 320,
	.screen_height		= 480,
	.bytes_per_pixel	= 2,
	.gpio			= 41,
	.pixel_format		= RGB565,
};

static struct platform_device r61581_smi_display_device = {
	.name    = "rhea_fb",
	.id      = 2,
	.dev = {
		.platform_data		= &r61581_smi_display_fb_data,
		.dma_mask		= (u64 *) ~(u32)0,
		.coherent_dma_mask	= ~(u32)0,
	},
};
#endif

/* Rhea Ray specific platform devices */
static struct platform_device *rhea_berri_plat_devices[] __initdata = {
#ifdef CONFIG_KEYBOARD_BCM
	&bcm_kp_device,
#endif

#ifdef CONFIG_KONA_HEADSET
	&headset_device,
#endif

#ifdef CONFIG_DMAC_PL330
	&pl330_dmac_device,
#endif
#ifdef CONFIG_HAPTIC_SAMSUNG_PWM
	&haptic_pwm_device,
#endif
#ifdef CONFIG_BACKLIGHT_PWM
	&bcm_backlight_devices,
#endif
/* TPS728XX device registration */
#ifdef CONFIG_REGULATOR_TPS728XX
	&tps728xx_device,
#endif
#ifdef CONFIG_FB_BRCM_RHEA
	&alex_dsi_display_device,
	&nt35582_smi_display_device,
	&r61581_smi_display_device,
#endif

#if (defined(CONFIG_BCM_RFKILL) || defined(CONFIG_BCM_RFKILL_MODULE))
    &board_bcmbt_rfkill_device,
#endif
#ifdef CONFIG_BCM_BT_LPM
    &board_bcmbt_lpm_device,
#endif

};

/* Add all userspace regulator consumer devices here */
#ifdef CONFIG_REGULATOR_USERSPACE_CONSUMER
struct platform_device *rhea_berri_userspace_consumer_devices[] __initdata = {
#if defined(CONFIG_REGULATOR_BCM_PMU59055) && defined(CONFIG_MFD_BCM_PMU590XX)
	&bcm59055_uc_device_sim,
#endif
#ifdef CONFIG_REGULATOR_TPS728XX
	&tps728xx_uc_device_sim2,
#endif
};
#endif

/* Add all virtual regulator consumer devices here */
#ifdef CONFIG_REGULATOR_VIRTUAL_CONSUMER
struct platform_device *rhea_berri_virtual_consumer_devices[] __initdata = {
#if defined(CONFIG_REGULATOR_BCM_PMU59055) && defined(CONFIG_MFD_BCM_PMU590XX)
	&bcm59055_vc_device_sim,
#endif
#ifdef CONFIG_REGULATOR_TPS728XX
	&tps728xx_vc_device_sim2,
#endif
};
#endif


/* Rhea Ray specific i2c devices */
static void __init rhea_berri_add_i2c_devices (void)
{
	/* 59055 on BSC - PMU */
#ifdef CONFIG_MFD_BCM_PMU590XX
	i2c_register_board_info(2,
			pmu_info,
			ARRAY_SIZE(pmu_info));
#endif
#ifdef CONFIG_GPIO_PCA953X
	i2c_register_board_info(1,
			pca953x_info,
			ARRAY_SIZE(pca953x_info));
#endif
#ifdef CONFIG_TOUCHSCREEN_QT602240
	i2c_register_board_info(1,
			qt602240_info,
			ARRAY_SIZE(qt602240_info));
#endif
#ifdef CONFIG_KEYBOARD_LM8325
	i2c_register_board_info(1,
			lm8325_info,
			ARRAY_SIZE(lm8325_info));
#endif
}

static int __init rhea_berri_add_lateInit_devices (void)
{
	board_add_sdio_devices();
	return 0;
}

static void __init rhea_berri_reserve(void)
{
	board_common_reserve();
}

static void enable_smi_display_clks(void)
{
	struct clk *smi_axi;
	struct clk *mm_dma;
	struct clk *smi;

	smi_axi = clk_get (NULL, "smi_axi_clk");
	mm_dma = clk_get (NULL, "mm_dma_axi_clk");

	smi = clk_get (NULL, "smi_clk");
	BUG_ON (!smi_axi || !smi || !mm_dma);


	clk_set_rate (smi, 250000000);

	clk_enable (smi_axi);
	clk_enable (smi);
	clk_enable(mm_dma);
}

/* All Rhea Ray specific devices */
static void __init rhea_berri_add_devices(void)
{
	enable_smi_display_clks();

#ifdef CONFIG_KEYBOARD_BCM
	bcm_kp_device.dev.platform_data = &bcm_keypad_data;
#endif
	platform_add_devices(rhea_berri_plat_devices, ARRAY_SIZE(rhea_berri_plat_devices));

	rhea_berri_add_i2c_devices();
#ifdef CONFIG_REGULATOR_USERSPACE_CONSUMER
	platform_add_devices(rhea_berri_userspace_consumer_devices, ARRAY_SIZE(rhea_berri_userspace_consumer_devices));
#endif
#ifdef CONFIG_REGULATOR_VIRTUAL_CONSUMER
	platform_add_devices(rhea_berri_virtual_consumer_devices, ARRAY_SIZE(rhea_berri_virtual_consumer_devices));
#endif

	spi_register_board_info(spi_slave_board_info,
				ARRAY_SIZE(spi_slave_board_info));
}

void __init board_init(void)
{
	board_add_common_devices();
	rhea_berri_add_devices();
	return;
}

void __init board_map_io(void)
{
	/* Map machine specific iodesc here */

	rhea_map_io();
}

late_initcall(rhea_berri_add_lateInit_devices);

MACHINE_START(RHEA_BERRI, "RheaBerri")
	.map_io = board_map_io,
	.init_irq = kona_init_irq,
	.timer  = &kona_timer,
	.init_machine = board_init,
	.reserve = rhea_berri_reserve
MACHINE_END

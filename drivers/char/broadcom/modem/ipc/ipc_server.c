/*****************************************************************************
 *  Copyright 2003 - 2007 Broadcom Corporation.  All rights reserved.
 *
 *  Unless you and Broadcom execute a separate written software license
 *  agreement governing use of this software, this software is licensed to you
 *  under the terms of the GNU General Public License version 2, available at
 *  http://www.gnu.org/copyleft/gpl.html (the "GPL").
 *
 *  Notwithstanding the above, under no circumstances may you combine this
 *  software in any way with any other Broadcom software provided under a
 *  license other than the GPL, without Broadcom's express prior written
 *  consent.
 *
 *****************************************************************************/

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#ifdef CONFIG_HAS_WAKELOCK
#include <linux/wakelock.h>
#endif
#include <linux/semaphore.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/jiffies.h>
#ifdef CONFIG_BRCM_FUSE_IPC_CIB
#include <linux/broadcom/ipcinterface.h>
#else
#include <linux/broadcom/IPCInterface.h>
#endif
#include <linux/broadcom/ipc_server_ifc.h>
#include <linux/broadcom/ipc_server_ioctl.h>
#include <plat/kona_reset_reason.h>

#include "lnx_support.h"
#include "ipc_server_ccb.h"
#include "ipc_debug.h"
#include "bcmlog.h"

#include <mach/comms/platform_mconfig.h>

#include <mach/io_map.h>
/* for BINTC register offsets */
#include <mach/rdb/brcm_rdb_bintc.h>
#include <mach/irqs.h>

/* definitions for Rhea/BI BModem IRQ's
   extracted from chip_irq.h for Rhea */
#define NUM_KONAIRQs          224
#define LAST_KONAIRQ          (NUM_KONAIRQs-1)
#define FIRST_BMIRQ           (LAST_KONAIRQ+1)
#define NUM_BMIRQs            56
#define IRQ_TO_BMIRQ(irq)         ((irq)-FIRST_BMIRQ)

#define IPC_MAJOR (204)

#define BCM_KERNEL_IPC_NAME "bcm_fuse_ipc"

struct ipcs_info {
	dev_t devnum;
	int ipc_state;
	struct cdev cdev;
	struct tasklet_struct intr_tasklet;
	struct work_struct cp_crash_dump_wq;
	struct workqueue_struct *crash_dump_workqueue;
	void __iomem *apcp_shmem;
	struct class *mDriverClass;	/* driver class */
	struct device *drvdata;
};

static Boolean cp_running;	/* FALSE; */

static struct ipcs_info g_ipc_info = { 0 };

#ifdef CONFIG_HAS_WAKELOCK
struct wake_lock ipc_wake_lock;
#endif
static IPC_PlatformSpecificPowerSavingInfo_T ipc_ps;

extern int IpcCPCrashCheck(void);
extern void ProcessCPCrashedDump(struct work_struct *work);

#if defined(CONFIG_BCM215X_PM) && defined(CONFIG_ARCH_BCM2153)
extern void pm_ipc_power_saving_init(IPC_PlatformSpecificPowerSavingInfo_T
				     *ipc_ps);
#endif

struct IPC_Evt {
	wait_queue_head_t evt_wait;
	int evt;
};

static void *EventCreate(void)
{
	struct IPC_Evt *ipcEvt;

	ipcEvt = kmalloc(sizeof(struct IPC_Evt), GFP_ATOMIC);
	if (!ipcEvt) {
		IPC_DEBUG(DBG_ERROR, "IPC event create fail\n");
		return NULL;
	}

	init_waitqueue_head(&(ipcEvt->evt_wait));
	ipcEvt->evt = 0;

	return ipcEvt;
}

static IPC_ReturnCode_T EventSet(void *Event)
{
	struct IPC_Evt *ipcEvt = (struct IPC_Evt *) Event;

	/* **FIXME** need to protect access to this? */
	ipcEvt->evt = 1;
	wake_up_interruptible(&(ipcEvt->evt_wait));
	return IPC_OK;
}

static IPC_ReturnCode_T EventClear(void *Event)
{
	struct IPC_Evt *ipcEvt = (struct IPC_Evt *) Event;

	/* **FIXME** need to protect access to this? */
	ipcEvt->evt = 0;
	/* wakeup(&(ipcEvt->evt_wait)); */
	return IPC_OK;
}

static IPC_ReturnCode_T EventWait(void *Event, IPC_U32 MilliSeconds)
{
	struct IPC_Evt *ipcEvt = (struct IPC_Evt *) Event;
	int ret;

	if (MilliSeconds == IPC_WAIT_FOREVER) {
		ret =
		    wait_event_interruptible((ipcEvt->evt_wait),
					     (ipcEvt->evt == 1));
		return (ret == 0) ? IPC_OK : IPC_ERROR;
	} else {
		ret = wait_event_interruptible_timeout((ipcEvt->evt_wait),
						       (ipcEvt->evt == 1),
						       msecs_to_jiffies
						       (MilliSeconds));
		return (ret ==
			0) ? IPC_TIMEOUT : ((ret ==
					     -ERESTARTSYS) ? IPC_ERROR :
					    IPC_OK);
	}
}

static IPC_ReturnCode_T EventDelete(void *Event)
{
	struct IPC_Evt *ipcEvt = (struct IPC_Evt *) Event;
	if (ipcEvt) {
		IPC_DEBUG(DBG_TRACE, "EventDelete: %p\n", ipcEvt);
		kfree(ipcEvt);
	}
	return IPC_OK;
}

static int ipcs_open(struct inode *inode, struct file *file)
{
	struct ipcs_info *info;

	info = container_of(inode->i_cdev, struct ipcs_info, cdev);
	file->private_data = info;

	return 0;
}

static int ipcs_release(struct inode *inode, struct file *file)
{
	return 0;
}

ssize_t ipcs_read(struct file *filep, char __user *buf, size_t size,
		  loff_t *off)
{
	ssize_t rc = 0;

	/* rc = ipc_server_read(filep, buf, size, off); */

	return rc;
}

ssize_t ipcs_write(struct file *filep, const char __user *buf, size_t size,
		   loff_t *off)
{
	return -EPERM;
}

static long ipcs_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
	int rc = 0;

	/* rc = ipc_server_ioctl(cmd, arg); */

	return rc;
}

static const struct file_operations ipc_ops = {
	.owner = THIS_MODULE,
	.open = ipcs_open,
	.read = ipcs_read,
	.write = ipcs_write,
	.unlocked_ioctl = ipcs_ioctl,
	.mmap = NULL,
	.release = ipcs_release,
};

/**
  @fn Boolean is_CP_running(void);
  */
Boolean is_CP_running(void)
{
	return cp_running;
}

/**
  @fn void ipcs_ipc_initialised(void);
  */
void ipcs_ipc_initialised(void)
{
	IPC_DEBUG(DBG_TRACE, "IPC Initialization completed\n");
	g_ipc_info.ipc_state = 1;

	return;
}

/**
  @fn void ipcs_get_ipc_state(int *state);
  */
void ipcs_get_ipc_state(int *state)
{
	if (state != NULL)
		*state = g_ipc_info.ipc_state;

	return;
}

/**
  @fn void ipcs_ipc_reset(void);
  */
void ipcs_ipc_reset(void)
{
	IPC_DEBUG(DBG_TRACE, "reset ...\n");

	return;
}

int cp_crashed;

void ipcs_intr_tasklet_handler(unsigned long data)
{
	cp_crashed = 0;

	if (IpcCPCrashCheck()) {
		cp_crashed = 1;

		/* schedule the work on the decidated CP crash dump work
		 * queue
		 */
		queue_work(g_ipc_info.crash_dump_workqueue,
			   &g_ipc_info.cp_crash_dump_wq);
		IPC_ProcessEvents();
	} else {
		IPC_ProcessEvents();
#ifdef CONFIG_HAS_WAKELOCK
		wake_unlock(&ipc_wake_lock);
#endif
	}
}

static irqreturn_t ipcs_interrupt(int irq, void *dev_id)
{
	void __iomem *base = (void __iomem *)(KONA_BINTC_BASE_ADDR);
	int birq = IRQ_TO_BMIRQ(IRQ_IPC_C2A_BINTC);	/* 55; */

	/* Clear the interrupt */
	if (birq >= 32)
		writel(1 << (birq - 32),
		       base + BINTC_ISWIR1_CLR_OFFSET /* 0x34 */);
	else
		writel(1 << (birq), base + BINTC_ISWIR0_CLR_OFFSET /* 0x24 */);

	IPC_UpdateIrqStats();

#ifdef CONFIG_HAS_WAKELOCK
	wake_lock(&ipc_wake_lock);
#endif
	tasklet_schedule(&g_ipc_info.intr_tasklet);

	return IRQ_HANDLED;
}

/**
  @fn int ipc_ipc_init(void *smbase, unsigned int size);
  */
int ipc_ipc_init(void *smbase, unsigned int size)
{
	IPC_ControlInfo_T ipc_control;

	memset(&ipc_control, 0, sizeof(IPC_ControlInfo_T));

	ipc_control.RaiseEventFptr = bcm_raise_cp_int;
	ipc_control.LockFunctions.CreateLock = bcm_create_lock;
	ipc_control.LockFunctions.AcquireLock = bcm_aquire_lock;
	ipc_control.LockFunctions.ReleaseLock = bcm_release_lock;
	ipc_control.LockFunctions.DeleteLock = bcm_delete_lock;
	ipc_control.PhyToOSAddrFPtr = bcm_map_phys_to_virt;
	ipc_control.OSToPhyAddrFPtr = bcm_map_virt_to_phys;
	ipc_control.EventFunctions.Create = EventCreate;
	ipc_control.EventFunctions.Set = EventSet;
	ipc_control.EventFunctions.Clear = EventClear;
	ipc_control.EventFunctions.Wait = EventWait;
	ipc_control.EventFunctions.Delete = EventDelete;
	ipc_control.PowerSavingStruct = &ipc_ps;

#if defined(CONFIG_BCM215X_PM) && defined(CONFIG_ARCH_BCM2153)
	pm_ipc_power_saving_init(&ipc_ps);
#endif

	IPC_Initialise(smbase, size, IPC_AP_CPU, &ipc_control,
		       ipcs_ipc_initialised, ipcs_ipc_reset, NULL);

	return 0;
}

extern int IPC_IsCpIpcInit(void *pSmBase, IPC_CPU_ID_T Cpu);

void WaitForCpIpc(void *pSmBase)
{
	int k = 0, ret = 0;
	int ts0, ts1;
	void __iomem *cp_boot_base;
	u32 reg_val;

	IPC_DEBUG(DBG_WARN, "Waiting for CP IPC to init 0x%x\n",
		  (unsigned int)pSmBase);

	/* Debug info to show is_ap_only_boot() status */
	if (is_ap_only_boot())
		IPC_DEBUG(DBG_WARN, "AP ONLY BOOT\n");
	else
		IPC_DEBUG(DBG_WARN, "NORMAL BOOT\n");

	if (!is_ap_only_boot()) {	/* Check for AP_BOOT or NORMAL_BOOT */

		ts0 = (int)jiffies;
		ret = IPC_IsCpIpcInit(pSmBase, IPC_AP_CPU);
		while (ret == 0) {
			/*  Wait up to 2s for CP to init  */
			if (k++ > 200)
				break;
			else
				mdelay(10);
			ret = IPC_IsCpIpcInit(pSmBase, IPC_AP_CPU);
		}
		ts1 = (int)jiffies;

		IPC_DEBUG(DBG_WARN, "Back from IPC_IsCpIpcInit, waited %u ms\n",
			  jiffies_to_msecs(ts1 - ts0));
	}

	if (ret == 1) {
		IPC_DEBUG(DBG_WARN, "CP IPC initialized\n");
		cp_running = 1;	/* TRUE; */
	} else if (ret == 0) {
		IPC_DEBUG(DBG_ERROR,
			  "********************************************************************\n");
		IPC_DEBUG(DBG_ERROR,
			  "*                                                                  *\n");
		IPC_DEBUG(DBG_ERROR,
			  "*       CP IPC NOT INITIALIZED - SYSTEM BOOTS WITH AP ONLY!!!      *\n");
		IPC_DEBUG(DBG_ERROR,
			  "*                                                                  *\n");
		IPC_DEBUG(DBG_ERROR,
			  "********************************************************************\n");
		cp_boot_base = ioremap_nocache(MODEM_ITCM_ADDRESS, 0x20);
		if (!cp_boot_base) {
			IPC_DEBUG(DBG_ERROR,
				  "ITCM Addr=0x%x, length=0x%x",
				  MODEM_ITCM_ADDRESS, 0x20);
			IPC_DEBUG(DBG_ERROR, "ioremap cp_boot_base error\n");
			return;
		}
		reg_val = readl(cp_boot_base);
		IPC_DEBUG(DBG_ERROR, "reset vector value is 0x%x\n", reg_val);

		reg_val = readl(cp_boot_base + 0x20);
		IPC_DEBUG(DBG_ERROR, "CP Boot flag 0x%x\n", reg_val);

		iounmap(cp_boot_base);

		/* SKIP reset is_ap_only_boot() non zero */
		if (!is_ap_only_boot())
			BUG_ON(ret == 0);
	} else if (ret == -1) {
		IPC_DEBUG(DBG_ERROR,
			  "********************************************************************\n");
		IPC_DEBUG(DBG_ERROR,
			  "*                                                                  *\n");
		IPC_DEBUG(DBG_ERROR,
			  "*                             CP CRASHED !!!                       *\n");
		IPC_DEBUG(DBG_ERROR,
			  "*                                                                  *\n");
		IPC_DEBUG(DBG_ERROR,
			  "********************************************************************\n");
		BUG_ON(ret);
	} else if (ret == -2) {
		IPC_DEBUG(DBG_ERROR,
			  "********************************************************************\n");
		IPC_DEBUG(DBG_ERROR,
			  "*                                                                  *\n");
		IPC_DEBUG(DBG_ERROR,
			  "*                     AP/CP IPC VERSION NOT MATCH !!!               *\n");
		IPC_DEBUG(DBG_ERROR,
			  "*                                                                  *\n");
		IPC_DEBUG(DBG_ERROR,
			  "********************************************************************\n");
		/* BUG_ON(ret); */
	}
}

/**
  static int ipcs_init(void *smbase, unsigned int size)
  */
static int ipcs_init(void *smbase, unsigned int size, int isReset)
{
	int rc = 0;

	IPC_DEBUG(DBG_TRACE, "WaitForCpIpc\n");
	/* Wait for CP to initialize */
	WaitForCpIpc(smbase);
	IPC_DEBUG(DBG_TRACE, "WaitForCpIpc done\n");

	/* Initialize OS specific callbacks with the IPC lib */
	rc = ipc_ipc_init(smbase, size);
	if (rc) {
		IPC_DEBUG(DBG_ERROR, "ipc_ipc_init() failed, ret[%d]\n", rc);
		return rc;
	}
	IPC_DEBUG(DBG_TRACE, "ipc_ipc_init done\n");

	/* Register Endpoints */
	rc = ipcs_ccb_init(isReset);
	if (rc) {
		IPC_DEBUG(DBG_ERROR, "ipcs_ccb_init() failed, ret[%d]\n", rc);
		return rc;
	}

	IPC_DEBUG(DBG_TRACE, "ipcs_ccb_init done\n");
	/* Let CP know that we are done registering endpoints */
	IPC_Configured();

	/* Wait for IPC initialized */
	IPC_DEBUG(DBG_TRACE, "IPC_Configured() invoked\n");

	return 0;
}

int ipcs_reinitialize_ipc(void)
{
	IPC_DEBUG(DBG_INFO, "calling ipcs_init\n");
	return ipcs_init((void *)g_ipc_info.apcp_shmem, IPC_SIZE, 1);
}

static int CP_Boot(void)
{
	int started = 0;
	void __iomem *cp_boot_itcm;
	void __iomem *cp_bmodem_r4cfg;
	unsigned int r4init;
	unsigned int jump_instruction = 0xEA000000;

#define BMODEM_SYSCFG_R4_CFG0  0x3a004000
#define CP_SYSCFG_BASE_SIZE    0x8
#define CP_ITCM_BASE_SIZE      0x1000	/* 1 4k page */

	IPC_DEBUG(DBG_TRACE, "enter\n");
	cp_bmodem_r4cfg = ioremap(BMODEM_SYSCFG_R4_CFG0, CP_SYSCFG_BASE_SIZE);
	if (!cp_bmodem_r4cfg) {
		IPC_DEBUG(DBG_ERROR,
			  "BMODEM_SYSCFG_R4_CFG0=0x%x, CP_SYSCFG_BASE_SIZE=0x%x\n",
			  BMODEM_SYSCFG_R4_CFG0, CP_SYSCFG_BASE_SIZE);
		IPC_DEBUG(DBG_ERROR, "ioremap cp_bmodem_r4cfg failed\n");
		return started;
	}

	r4init = *(unsigned int *)(cp_bmodem_r4cfg);

	/* check if the CP is already booted, and if not, then boot it */
	if ((0x5 != (r4init & 0x5))) {
		IPC_DEBUG(DBG_TRACE, "boot (R4 COMMS) - init code 0x%x ...\n",
			  r4init);

		/* Set the CP jump to address.  CP must jump to DTCM offset
		 * 0x400
		 */
		cp_boot_itcm = ioremap(MODEM_ITCM_ADDRESS, CP_ITCM_BASE_SIZE);
		if (!cp_boot_itcm) {
			IPC_DEBUG(DBG_ERROR,
				  "MODEM_ITCM_ADDRESS=0x%x, CP_ITCM_BASE_SIZE=0x%x\n",
				  MODEM_ITCM_ADDRESS, CP_ITCM_BASE_SIZE);
			IPC_DEBUG(DBG_ERROR, "ioremap cp_boot_itcm failed\n");
			return 0;
		}
		/* generate instruction for reset vector that jumps to start of
		 * cp_boot.img at 0x20400
		 */
		jump_instruction |=
		    (0x00FFFFFFUL & (((0x20000 + RESERVED_HEADER)
				      / 4) - 2));
		IPC_DEBUG(DBG_TRACE,
			  "cp_boot_itcm 0x%x jump_instruction 0x%x\n",
			  (unsigned int)cp_boot_itcm, jump_instruction);
		/* write jump instruction to cp reset vector */
		*(unsigned int *)(cp_boot_itcm) = jump_instruction;

		iounmap(cp_boot_itcm);

		/* start CP - should jump to 0x20400 and spin there */
		*(unsigned int *)(cp_bmodem_r4cfg) = 0x5;
	} else {
		IPC_DEBUG(DBG_TRACE,
			  "(R4 COMMS) already started - init code 0x%x ...\n",
			  r4init);
	}

	iounmap(cp_bmodem_r4cfg);
	IPC_DEBUG(DBG_TRACE, "exit\n");

	return started;
}

int Comms_Start(int isReset)
{
	void __iomem *apcp_shmem;
	void __iomem *cp_boot_base;
	u32 reg_val;

	IPC_DEBUG(DBG_TRACE, "enter\n");

#ifdef CONFIG_BCM_MODEM_DEFER_CP_START
	CP_Boot();
#else
	if (isReset) {
		IPC_DEBUG(DBG_INFO, "call CP_Boot\n");
		CP_Boot();
	}
#endif

	apcp_shmem = ioremap_nocache(IPC_BASE, IPC_SIZE);
	if (!apcp_shmem) {
		IPC_DEBUG(DBG_ERROR, "IPC_BASE=0x%x, IPC_SIZE=0x%x\n",
			  IPC_BASE, IPC_SIZE);
		IPC_DEBUG(DBG_ERROR, "ioremap shmem failed\n");
		return -1;
	}
	/* clear IPC shared memory */
	memset(apcp_shmem, 0, IPC_SIZE);
	iounmap(apcp_shmem);
	IPC_DEBUG(DBG_TRACE, "cleared sh mem\n");

	cp_boot_base = ioremap_nocache(MODEM_DTCM_ADDRESS,
				       INIT_ADDRESS_OFFSET + RESERVED_HEADER);

	if (!cp_boot_base) {
		IPC_DEBUG(DBG_ERROR,
			  "DTCM Addr=0x%x, length=0x%x",
			  MODEM_DTCM_ADDRESS,
			  INIT_ADDRESS_OFFSET + RESERVED_HEADER);
		IPC_DEBUG(DBG_ERROR, "ioremap cp_boot_base error\n");
		return -1;
	}

	/* Start the CP:
	 *      - read main address
	 *      - write to init address
	 */
	reg_val = readl(cp_boot_base + MAIN_ADDRESS_OFFSET + RESERVED_HEADER);
	IPC_DEBUG(DBG_TRACE, "main addr 0x%x\n", reg_val);
	writel(reg_val, cp_boot_base + INIT_ADDRESS_OFFSET + RESERVED_HEADER);

	iounmap(cp_boot_base);
	IPC_DEBUG(DBG_TRACE, "modem (R4 COMMS) started ...\n");

	printk("[%s]cp start delay, 500msec. \n",__func__);
	msleep(500);

	return 0;
}

int __init Comms_Start_On_Boot(void)
{
	return Comms_Start(0);
}

arch_initcall(Comms_Start_On_Boot);

struct device *ipcs_get_drvdata(void)
{
	return g_ipc_info.drvdata;
}

static int ipcs_read_proc(char *page, char **start, off_t off,
			  int count, int *eof, void *data)
{
	int len = IPC_DumpStatus(page, count);
	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}

static int __init ipcs_module_init(void)
{
	int rc;
	struct proc_dir_entry *dir;

	if (ipc_crashsupport_init())
		goto out;

	dir = create_proc_read_entry("driver/bcmipc", 0,
				     NULL, ipcs_read_proc, NULL);
	if (dir == NULL) {
		IPC_DEBUG(DBG_ERROR,
			  "ipcs_module_init: can't create /proc/driver/bcmipc\n");
		/* return -1; */
	}

	IPC_DEBUG(DBG_TRACE, "start ...\n");

	g_ipc_info.ipc_state = 0;

	g_ipc_info.devnum = MKDEV(IPC_MAJOR, 0);

	rc = register_chrdev_region(g_ipc_info.devnum, 1, BCM_KERNEL_IPC_NAME);
	if (rc < 0) {
		IPC_DEBUG(DBG_ERROR, "Error registering the IPC device\n");
		goto out;
	}

	cdev_init(&g_ipc_info.cdev, &ipc_ops);

	g_ipc_info.cdev.owner = THIS_MODULE;

	rc = cdev_add(&g_ipc_info.cdev, g_ipc_info.devnum, 1);
	if (rc) {
		IPC_DEBUG(DBG_ERROR, "cdev_add errpr\n");
		goto out_unregister;
	}

	g_ipc_info.mDriverClass = class_create(THIS_MODULE,
					       BCM_KERNEL_IPC_NAME);
	if (IS_ERR(g_ipc_info.mDriverClass)) {
		IPC_DEBUG(DBG_ERROR, "driver class_create failed\n");
		goto out_unregister;
	}

	g_ipc_info.drvdata = device_create(g_ipc_info.mDriverClass, NULL,
					   MKDEV(IPC_MAJOR, 0), NULL,
					   BCM_KERNEL_IPC_NAME);
	if (IS_ERR(g_ipc_info.drvdata)) {
		IPC_DEBUG(DBG_ERROR, "device_create drvdata failed\n");
		goto out_unregister;
	}

	IPC_DEBUG(DBG_TRACE, "Allocate CP crash dump workqueue\n");
	g_ipc_info.crash_dump_workqueue = alloc_workqueue("dump-wq",
							  WQ_FREEZABLE |
							  WQ_MEM_RECLAIM, 0);

	if (!g_ipc_info.crash_dump_workqueue) {
		IPC_DEBUG(DBG_ERROR,
			  "Cannot allocate CP crash dump workqueue\n");
		goto out_unregister;
	}

	INIT_WORK(&g_ipc_info.cp_crash_dump_wq, ProcessCPCrashedDump);

	tasklet_init(&g_ipc_info.intr_tasklet, ipcs_intr_tasklet_handler, 0);

	/**
	  Make sure this is not cache'd because CP has to know about any changes
	  we write to this memory immediately.
	  */
	IPC_DEBUG(DBG_TRACE, "ioremap_nocache IPC_BASE\n");
	g_ipc_info.apcp_shmem = ioremap_nocache(IPC_BASE, IPC_SIZE);
	if (!g_ipc_info.apcp_shmem) {
		rc = -ENOMEM;
		IPC_DEBUG(DBG_ERROR, "Could not map shmem\n");
		goto out_del;
	}

	IPC_DEBUG(DBG_TRACE, "ipcs_init\n");
	if (ipcs_init((void *)g_ipc_info.apcp_shmem, IPC_SIZE, 0)) {
		rc = -1;
		IPC_DEBUG(DBG_ERROR, "ipcs_init() failed\n");
		goto out_del;
	}

	IPC_DEBUG(DBG_TRACE, "ok\n");

#ifdef CONFIG_HAS_WAKELOCK
	wake_lock_init(&ipc_wake_lock, WAKE_LOCK_SUSPEND, "ipc_wake_lock");
#endif

	IPC_DEBUG(DBG_TRACE, "request_irq\n");
	rc = request_irq(IRQ_IPC_C2A, ipcs_interrupt, IRQF_NO_SUSPEND,
			 "ipc-intr", &g_ipc_info);

	if (rc) {
		IPC_DEBUG(DBG_ERROR, "request_irq error\n");
		goto out_del;
	}

	IPC_DEBUG(DBG_TRACE, "IRQ Clear and Enable\n");

	return 0;

out_del:
	cdev_del(&g_ipc_info.cdev);
out_unregister:
	unregister_chrdev_region(g_ipc_info.devnum, 1);
out:
	IPC_DEBUG(DBG_ERROR, "IPC Driver Failed to initialise!\n");
	return rc;
}

static void __exit ipcs_module_exit(void)
{
	tasklet_kill(&g_ipc_info.intr_tasklet);
	flush_workqueue(g_ipc_info.crash_dump_workqueue);
	destroy_workqueue(g_ipc_info.crash_dump_workqueue);

	iounmap(g_ipc_info.apcp_shmem);

	free_irq(IRQ_IPC_C2A, &g_ipc_info);

	cdev_del(&g_ipc_info.cdev);

	unregister_chrdev_region(g_ipc_info.devnum, 1);
#ifdef CONFIG_HAS_WAKELOCK
	wake_lock_destroy(&ipc_wake_lock);
#endif
	device_destroy(g_ipc_info.mDriverClass, MKDEV(IPC_MAJOR, 0));
	class_destroy(g_ipc_info.mDriverClass);

	return;
}

late_initcall(ipcs_module_init);
module_exit(ipcs_module_exit);

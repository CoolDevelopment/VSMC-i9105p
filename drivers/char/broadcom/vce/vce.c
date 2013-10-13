/*******************************************************************************
Copyright 2010 Broadcom Corporation.  All rights reserved.

Unless you and Broadcom execute a separate written software license agreement
governing use of this software, this software is licensed to you under the
terms of the GNU General Public License version 2, available at
http://www.gnu.org/copyleft/gpl.html (the "GPL").

Notwithstanding the above, under no circumstances may you combine this software
in any way with any other Broadcom software provided under a license other than
the GPL, without Broadcom's express prior written consent.
*******************************************************************************/

#include <linux/kernel.h>

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include <asm/io.h>

#include <plat/pi_mgr.h>
#include <mach/rdb/brcm_rdb_mm_rst_mgr_reg.h>
#include <mach/rdb/brcm_rdb_sysmap.h>
#include <mach/rdb/brcm_rdb_vce.h>

#include <linux/broadcom/vce.h>

#define DRIVER_VERSION 10100
#define VCE_DEV_MAJOR	0

#define RHEA_VCE_BASE_PERIPHERAL_ADDRESS      VCE_BASE_ADDR

/*
  Interrupt assignment as specified in Kona Architecture Document.
  TODO: make this some sort of plat/arch specific thing
*/
#define IRQ_VCE                               BCM_INT_ID_RESERVED147

/* Always check for idle at every reset:  TODO: make this configurable? */
#define VCE_RESET_IMPLIES_ASSERT_IDLE

//#define VCE_DEBUG
#ifdef VCE_DEBUG
#define dbg_print(fmt, arg...) \
			printk(KERN_ALERT "%s():" fmt, __func__, ##arg)
#else
#define dbg_print(fmt, arg...)	do { } while (0)
#endif

#define err_print(fmt, arg...) \
		printk(KERN_ERR "%s():" fmt, __func__, ##arg)

/* TODO: some of these globals (all of them?) belong in one or more of
 * the data structures that follow */
static int vce_major = VCE_DEV_MAJOR;
static void __iomem *vce_base = NULL;
static void __iomem *mm_rst_base = NULL;
static struct clk *vce_clk;

/* Per VCE state: (actually, some global state mixed in here too --
 * TODO: separate this out in order to support multiple VCEs) */
static struct {
	struct semaphore clockctl_sem;
	uint32_t clock_enable_count;
	struct semaphore *g_irq_sem;
	struct semaphore acquire_sem;
	struct semaphore work_lock;
	struct proc_dir_entry *proc_vcedir;
	struct proc_dir_entry *proc_version;
	struct proc_dir_entry *proc_status;
	struct class *vce_class;
	uint32_t irq_enabled;
	struct pi_mgr_dfs_node *dfs_node;
	struct pi_mgr_qos_node *cpu_qos_node;
	struct semaphore armctl_sem;
	uint32_t arm_keepawake_count;
} vce_state;

/* Per open handle state: */
typedef struct {
	struct semaphore irq_sem;
	int vce_acquired;
	int mmap_clock_hold_count;	/* hack! */
} vce_t;

/***** Function Prototypes **************/
static void reset_vce(void);

/******************************************************************
	VCE HW specific functions
*******************************************************************/

#define vce_reg_poke(reg, value) \
	writel((value), vce_base + VCE_ ## reg ## _OFFSET)
#define vce_reg_poke_1field(reg, field, value) \
	vce_reg_poke(reg, (value << VCE_ ## reg ## _ ## field ## _SHIFT) & VCE_ ## reg ## _ ## field ## _MASK)
#define vce_reg_peek(reg) \
	readl(vce_base + VCE_ ## reg ## _OFFSET)

static bool vce_is_idle(void)
{
	uint32_t status;
	uint32_t busybits;
	bool not_idle;

	status = vce_reg_peek(STATUS);
	busybits =
	    (status & VCE_STATUS_VCE_BUSY_BITFIELD_MASK) >>
	    VCE_STATUS_VCE_BUSY_BITFIELD_SHIFT;

	/* busybits 7:0 can be validly set while VCE is idle */

	not_idle = ((status & VCE_STATUS_VCE_RUNNING_POS_MASK) != 0 ||
		    (busybits & 0xff00) != 0);

	return !not_idle;
}

static void assert_idle_nolock(void)
{
	bool not_idle;

	not_idle = !vce_is_idle();

	if (not_idle) {
		err_print("vce block is not idle\n");
	}

	BUG_ON(not_idle);
}

static void assert_vce_is_idle(void)
{
	down(&vce_state.work_lock);
	if (vce_state.clock_enable_count)
		assert_idle_nolock();
	up(&vce_state.work_lock);
}

static void reset_vce(void)
{
	uint32_t value;

	down(&vce_state.work_lock);

#ifdef VCE_RESET_IMPLIES_ASSERT_IDLE
	assert_idle_nolock();
#endif

	BUG_ON(vce_clk == NULL);
	clk_disable(vce_clk);
	//Write the password to enable accessing other registers
	writel((0xA5A5 << MM_RST_MGR_REG_WR_ACCESS_PASSWORD_SHIFT) |
	       (0x1 << MM_RST_MGR_REG_WR_ACCESS_RSTMGR_ACC_SHIFT),
	       mm_rst_base + MM_RST_MGR_REG_WR_ACCESS_OFFSET);

	// Put VCE in reset state
	value = readl(mm_rst_base + MM_RST_MGR_REG_SOFT_RSTN0_OFFSET);
	value = value & ~(0x1 << MM_RST_MGR_REG_SOFT_RSTN0_VCE_SOFT_RSTN_SHIFT);
	writel(value, mm_rst_base + MM_RST_MGR_REG_SOFT_RSTN0_OFFSET);

	// Enable VCE
	value = value | (0x1 << MM_RST_MGR_REG_SOFT_RSTN0_VCE_SOFT_RSTN_SHIFT);
	writel(value, mm_rst_base + MM_RST_MGR_REG_SOFT_RSTN0_OFFSET);

	//Write the password to disable accessing other registers
	writel((0xA5A5 << MM_RST_MGR_REG_WR_ACCESS_PASSWORD_SHIFT),
	       mm_rst_base + MM_RST_MGR_REG_WR_ACCESS_OFFSET);

	clk_enable(vce_clk);

	up(&vce_state.work_lock);
}

static irqreturn_t vce_isr(int irq, void *unused)
{
	(void)irq;		/* TODO: shouldn't this be used?? */
	(void)unused;

	dbg_print("Got vce interrupt\n");

	if (!(vce_reg_peek(STATUS) & VCE_STATUS_VCE_INTERRUPT_POS_MASK)) {
		err_print
		    ("VCE Interrupt went away.  Almost certainly a bug.\n");
	}
	vce_reg_poke_1field(SEMA_CLEAR, CLR_INT_REQ, 1);

	/* We can't make any assertion about the contents of the
	 * status register we read below, because it's perfectly legal
	 * for another interrupt to come in, however, we can use this
	 * read to stall until the write is committed which will avoid
	 * a race which results in spurious extra interrupts.  This is
	 * arguably costly.  If the interrupt latency proves too big,
	 * we may choose to remove the read at the cost of potential
	 * spurious re-fires of the ISR */
	(void)vce_reg_peek(STATUS);

	if (vce_state.g_irq_sem)
		up(vce_state.g_irq_sem);
	else
		err_print("Got VCE interrupt but noone wants it\n");

	return IRQ_HANDLED;
}

static int _power_on(void)
{
	BUG_ON(vce_state.dfs_node != NULL);

	/* Platform specific power-on procedure.  May be that this
	 * should be in a separate file?  TODO: REVIEWME */
	vce_state.dfs_node =
	    pi_mgr_dfs_add_request("vce", PI_MGR_PI_ID_MM, PI_OPP_TURBO);
	if (!vce_state.dfs_node) {
		err_print("Failed to add dfs request for VCE\n");
		return -EIO;
	}

	BUG_ON(vce_state.dfs_node == NULL);
	return 0;
}

static void _power_off(void)
{
	int s;

	BUG_ON(vce_state.dfs_node == NULL);

	/* Platform specific power-off procedure.  May be that this
	 * should be in a separate file?  TODO: REVIEWME */
	s = pi_mgr_dfs_request_remove(vce_state.dfs_node);
	BUG_ON(s != 0);
	vce_state.dfs_node = NULL;

	BUG_ON(vce_state.dfs_node != NULL);
}

static int _clock_on(void)
{
	int s;

	BUG_ON(vce_clk != NULL);
	BUG_ON(vce_state.dfs_node == NULL);

	vce_clk = clk_get(NULL, "vce_axi_clk");
	if (IS_ERR(vce_clk)) {
		int err = PTR_ERR(vce_clk);

		err_print("%s: error get clock\n", __func__);
		vce_clk = NULL;
		return err;
	}

	s = clk_enable(vce_clk);
	if (s != 0) {
		err_print("%s: error enabling clock\n", __func__);
		clk_put(vce_clk);
		vce_clk = NULL;
		return -EIO;
	}

	BUG_ON(vce_clk == NULL);
	return 0;
}

static void _clock_off(void)
{
	BUG_ON(vce_clk == NULL);
	clk_disable(vce_clk);
	vce_clk = NULL;
	BUG_ON(vce_clk != NULL);
}

static void power_on_and_start_clock(void)
{
	/* Assume clock control mutex is already acquired, and that block is currently off */
	BUG_ON(vce_state.clock_enable_count != 0);

	_power_on();		/* TODO: error handling */

	/* RST regs should be already mapped?  we don't bother here */
	BUG_ON(mm_rst_base == NULL);

	_clock_on();		/* TODO: error handling */

	/* We probably ought to map vce registers here, but for now,
	 * we go with the "promise not to access them" approach */
	BUG_ON(vce_base == NULL);
}

static void stop_clock_and_power_off(void)
{
	/* Assume clock control mutex is already acquired, and that block is currently off */
	BUG_ON(vce_state.clock_enable_count != 0);

	/* Theoretically, we might consider unmapping VCE regs here */
	BUG_ON(vce_base == NULL);

	_clock_off();

	/* Theoretically, we might consider unmapping reset regs here */
	BUG_ON(mm_rst_base == NULL);

	_power_off();
}

static void clock_on(void)
{
	down(&vce_state.clockctl_sem);
	if (vce_state.clock_enable_count == 0) {
		power_on_and_start_clock();
	}
	vce_state.clock_enable_count += 1;
	up(&vce_state.clockctl_sem);
}

static void clock_off(void)
{
	down(&vce_state.clockctl_sem);
	vce_state.clock_enable_count -= 1;
	if (vce_state.clock_enable_count == 0) {
		stop_clock_and_power_off();
	}
	up(&vce_state.clockctl_sem);
}

static void cpu_keepawake_dec(void)
{
	down(&vce_state.armctl_sem);
	vce_state.arm_keepawake_count -= 1;
	if (vce_state.arm_keepawake_count == 0) {
		pi_mgr_qos_request_update(vce_state.cpu_qos_node,
					  PI_MGR_QOS_DEFAULT_VALUE);
	}
	up(&vce_state.armctl_sem);
}

static void cpu_keepawake_inc(void)
{
	down(&vce_state.armctl_sem);
	if (vce_state.arm_keepawake_count == 0) {
		pi_mgr_qos_request_update(vce_state.cpu_qos_node, 0);
	}
	vce_state.arm_keepawake_count += 1;
	up(&vce_state.armctl_sem);
}

#ifdef VCE_DEBUG
static void clock_on_(int linenum)
{
	clock_on();
	dbg_print("VCE clock_on() @ %d\n", linenum);
}

#define clock_on() clock_on_(__LINE__)
static void clock_off_(int linenum)
{
	dbg_print("VCE clock_ogg() @ %d\n", linenum);
	clock_off();
}

#define clock_off() clock_off_(__LINE__)
#endif

/******************************************************************
	VCE driver functions
*******************************************************************/
static int vce_open(struct inode *inode, struct file *filp)
{
	vce_t *dev;

	(void)inode;		/* ? */

	dev = kmalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	dev->vce_acquired = 0;
	sema_init(&dev->irq_sem, 0);

	/* A hack */
	dev->mmap_clock_hold_count = 0;

	filp->private_data = dev;
	return 0;
}

/* TODO: task list ... */
static int vce_release(struct inode *inode, struct file *filp)
{
	vce_t *dev;

	(void)inode;		/* ? */

	dev = (vce_t *) filp->private_data;

	if (dev->vce_acquired) {
		err_print
		    ("\n\nUser dying with VCE acquired\nWait for HW to go idle\n");

		//Userspace died with VCE acquired. The only safe thing is to wait for the
		//VCE hardware to go idle before letting any other process touch it.
		//Otherwise it can cause a AXI lockup or other bad things :-(
		/* Above comment is stale - it was for V3D --
		   copy/paste alert!  TODO: figure out what VCE needs
		   to do here... FIXME */
		down(&vce_state.work_lock);
		/* poke a reg, writes 0 to surrounding fields */
		vce_reg_poke_1field(CONTROL, RUN_BIT_CMD, 0);

		{
			int uglyctr = 0;
			while (!vce_is_idle() && (uglyctr++ < 10000))
				udelay(100);
		}
		up(&vce_state.work_lock);
		if (vce_is_idle())
			err_print("VCE HW idle\n");
		else
			err_print
			    ("Oops, gave up waiting -- this is probably fatal  FIXME\n");
		reset_vce();

		//Just free up the VCE HW
		vce_state.g_irq_sem = NULL;
		up(&vce_state.acquire_sem);
		clock_off();
	}

	if (!down_trylock(&dev->irq_sem)) {
		err_print
		    ("VCE driver closing with unacknowledged interrupts\n");
	}

	/* A hack */
	/* We should be unclocking in munmap() call, but for now, we
	 * defer to here and compensate.  Ugly.  Yes.  TODO: FIXME */
	while (dev->mmap_clock_hold_count > 0) {
		clock_off();
		dev->mmap_clock_hold_count -= 1;
	}

	kfree(dev);

	return 0;
}

static int vce_mmap(struct file *filp, struct vm_area_struct *vma)
{
	unsigned long vma_size;
	vce_t *dev;

	vma_size = vma->vm_end - vma->vm_start;
	dev = (vce_t *) (filp->private_data);

	/* TODO: FIXME */ (void)dev;

	if (vma_size & (~PAGE_MASK)) {
		err_print(KERN_ERR
			  "vce_mmap: mmaps must be aligned to a multiple of pages_size.\n");
		return -EINVAL;
	}

	if (!vma->vm_pgoff) {
		vma->vm_pgoff = RHEA_VCE_BASE_PERIPHERAL_ADDRESS >> PAGE_SHIFT;
	} else {		/* if (vma->vm_pgoff != (dev->mempool.addr >> PAGE_SHIFT)) */

		err_print("vce_mmap failed\n");
		return -EINVAL;
	}

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	clock_on();

	/* Remap-pfn-range will mark the range VM_IO and VM_RESERVED */
	if (remap_pfn_range(vma,
			    vma->vm_start,
			    vma->vm_pgoff, vma_size, vma->vm_page_prot)) {
		err_print("%s(): remap_pfn_range() failed\n", __FUNCTION__);
		return -EINVAL;
	}
	dev->mmap_clock_hold_count += 1;	/* A Hack!  We should have a
						 * way to unlock the clock
						 * when munmap() is called --
						 * TODO: FIXME */
	/* TODO:  when unmapped?  need a clock_off()!! */

	return 0;
}

static long vce_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	vce_t *dev;
	int ret = 0;

	if (_IOC_TYPE(cmd) != BCM_VCE_MAGIC)
		return -ENOTTY;

	if (_IOC_NR(cmd) > VCE_CMD_LAST)
		return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ)
		ret = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));

	if (_IOC_DIR(cmd) & _IOC_WRITE)
		ret |= !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));

	if (ret)
		return -EFAULT;

	dev = (vce_t *) (filp->private_data);

	switch (cmd) {
	case VCE_IOCTL_WAIT_IRQ:
		{
			dbg_print("Waiting for interrupt\n");
			if (down_interruptible(&dev->irq_sem)) {
				err_print("Wait for IRQ failed\n");
				return -ERESTARTSYS;
			}
		}
		break;

	case VCE_IOCTL_EXIT_IRQ_WAIT:
		//Up the semaphore to release the thread that's waiting for irq
		up(&dev->irq_sem);
		break;

	case VCE_IOCTL_RESET:
		{
			/* TODO: should we assert clocks are already
			 * on?  or just ignore the request when clocks
			 * are off?  We'll get a PoR anyway?  Or
			 * should we handle case with power on and
			 * clocks off (e.g. other mm block has power)
			 * by deferring the reset to the next clock
			 * on?  FIXME */
			clock_on();
			reset_vce();
			clock_off();
		}
		break;

	case VCE_IOCTL_HW_ACQUIRE:
		{
			clock_on();

			//Wait for the VCE HW to become available
			if (down_interruptible(&vce_state.acquire_sem)) {
				err_print("Wait for VCE HW failed\n");
				clock_off();
				return -ERESTARTSYS;
			}
			vce_state.g_irq_sem = &dev->irq_sem;	//Replace the irq sem with current process sem
			dev->vce_acquired = 1;	//Mark acquired: will come handy in cleanup process
		}
		break;

	case VCE_IOCTL_HW_RELEASE:
		{
			vce_state.g_irq_sem = NULL;	//Free up the g_irq_sem
			dev->vce_acquired = 0;	//Not acquired anymore
			up(&vce_state.acquire_sem);	//VCE is up for grab
			clock_off();
		}
		break;

	case VCE_IOCTL_UNUSE_ACP:
		{
			cpu_keepawake_dec();
		}
		break;

	case VCE_IOCTL_USE_ACP:
		{
			cpu_keepawake_inc();
		}
		break;

	case VCE_IOCTL_ASSERT_IDLE:
		{
			assert_vce_is_idle();
		}
		break;

#if 0
		/* Some DEBUG stuff -- we may wish to lose this in production driver... */
	case VCE_IOCTL_DEBUG_FETCH_KSTAT_IRQS:
		{
			unsigned int dacount;
			unsigned int copyerr;
			dacount = kstat_irqs_cpu(IRQ_VCE, /*cpu= */ 0);
			copyerr =
			    copy_to_user((void *)arg, &dacount,
					 sizeof(dacount));
			if (copyerr != 0) {
				ret = -EINVAL;
			}
		}
		break;
#endif

	default:
		{
			ret = -ENOTTY;
		}
		break;
	}

	return ret;
}

static struct file_operations vce_fops = {
	.open = vce_open,
	.release = vce_release,
	.mmap = vce_mmap,
	.unlocked_ioctl = vce_ioctl,
};

static int proc_version_read(char *buffer, char **start, off_t offset,
			     int bytes, int *eof, void *context)
{
	int ret;
	uint32_t vce_version, spec_revision, sub_revision;
	int len;

	ret = 0;

	/* TODO: we have some private data here that we can use: */
	(void)context;

	clock_on();
	vce_version = vce_reg_peek(VERSION);
	clock_off();
	spec_revision =
	    (vce_version & VCE_VERSION_SPEC_REVISION_MASK) >>
	    VCE_VERSION_SPEC_REVISION_SHIFT;
	sub_revision =
	    (vce_version & VCE_VERSION_SUB_REVISION_MASK) >>
	    VCE_VERSION_SUB_REVISION_SHIFT;

	/* If this assertion fails, it means we didn't decompose the
	   version information fully.  Perhaps you're running on a
	   simulated version of the IP, or the register has changed
	   its layout since this driver was written? */
	BUG_ON(vce_version !=
	       (spec_revision << VCE_VERSION_SPEC_REVISION_SHIFT | sub_revision
		<< VCE_VERSION_SUB_REVISION_SHIFT));

	if (bytes < 20) {
		/* TODO: be a little more precise about the length of buffer required -- we know we write just 15 right now... */
		ret = -1;
		goto e0;
	}

	/* TODO: what's start and offset for? reading in chunks?  that'll never happen (hack!) */
	len =
	    sprintf(buffer, "h:%u.%u\nk:%u\n", spec_revision, sub_revision,
		    DRIVER_VERSION);

	/* Not using these and don't really know how to: */
	(void)start;
	(void)offset;
	(void)eof;

	ret = len;

	BUG_ON(len > bytes);
	BUG_ON(ret < 0);
	return ret;

	/*
	   error exit paths follow
	 */

e0:
	BUG_ON(ret >= 0);
	return ret;
}

static int proc_status_read(char *buffer, char **start, off_t offset, int bytes,
			    int *eof, void *context)
{
	int ret;
	uint32_t status;
	uint32_t busybits;
	uint32_t stoppage_reason;
	uint32_t running;
	uint32_t nanoflag;
	uint32_t irq;
	int len;

	ret = 0;

	/* TODO: we have some private data here that we can use: */
	(void)context;

	/* TODO: do we need to power it on just to read the status?
	 * Shouldn't we just write 'gated' or some such to the buffer
	 * instead?  or return an error status? */
	clock_on();

	status = vce_reg_peek(STATUS);
	busybits =
	    (status & VCE_STATUS_VCE_BUSY_BITFIELD_MASK) >>
	    VCE_STATUS_VCE_BUSY_BITFIELD_SHIFT;
	stoppage_reason =
	    (status & VCE_STATUS_VCE_REASON_POS_MASK) >>
	    VCE_STATUS_VCE_REASON_POS_SHIFT;
	running =
	    (status & VCE_STATUS_VCE_RUNNING_POS_MASK) >>
	    VCE_STATUS_VCE_RUNNING_POS_SHIFT;
	nanoflag =
	    (status & VCE_STATUS_VCE_NANOFLAG_POS_MASK) >>
	    VCE_STATUS_VCE_NANOFLAG_POS_SHIFT;
	irq =
	    (status & VCE_STATUS_VCE_INTERRUPT_POS_MASK) >>
	    VCE_STATUS_VCE_INTERRUPT_POS_SHIFT;

	if (bytes < 20) {
		/* TODO: be a little more precise about the length of buffer required -- we know we write around 14 right now... */
		ret = -1;
		goto e0;
	}

	/* TODO: what's start and offset for? reading in chunks?  that'll never happen (hack!) */
	len =
	    sprintf(buffer, "%u %u %u %u %u\n", busybits, stoppage_reason,
		    running, nanoflag, irq);

	/* Not using these and don't really know how to: */
	(void)start;
	(void)offset;
	(void)eof;

	ret = len;

	clock_off();

	BUG_ON(len > bytes);
	BUG_ON(ret < 0);
	return ret;

	/*
	   error exit paths follow
	 */

e0:
	clock_off();
	BUG_ON(ret >= 0);
	return ret;
}

int __init vce_init(void)
{
	int ret;
	struct device *device;

	dbg_print("VCE driver Init\n");

	/* initialize the per-driver/per-vce/global struct (TODO!) */
	memset(&vce_state, 0, sizeof(vce_state));

	ret = register_chrdev(0, VCE_DEV_NAME, &vce_fops);
	if (ret < 0)
		goto errA;
	else
		vce_major = ret;

	vce_state.vce_class = class_create(THIS_MODULE, VCE_DEV_NAME);
	if (IS_ERR(vce_state.vce_class)) {
		err_print("Failed to create VCE class\n");
		ret = PTR_ERR(vce_state.vce_class);
		goto errB;
	}

	device =
	    device_create(vce_state.vce_class, NULL, MKDEV(vce_major, 0), NULL,
			  VCE_DEV_NAME);
	if (IS_ERR_OR_NULL(device)) {
		err_print("Failed to create VCE device\n");
		ret = PTR_ERR(device);
		goto errC;
	}

	/* For the power management */
	sema_init(&vce_state.clockctl_sem, 1);
	sema_init(&vce_state.armctl_sem, 1);

	/* We map the registers -- even though the power to the domain
	 * remains off... TODO: consider whether that's dangerous?  It
	 * would be a bug to try to access these anyway while the
	 * block is off, so let's just make sure we don't... :) */

	/* Map the VCE registers */
	/* TODO: split this out into the constituent parts: prog mem / data mem / periph mem / regs */
	/* Also get rid of the hardcoded size */
	vce_base =
	    (void __iomem *)ioremap_nocache(RHEA_VCE_BASE_PERIPHERAL_ADDRESS,
					    SZ_512K);
	if (vce_base == NULL) {
		err_print("Failed to MAP the VCE IO space\n");
		goto err;
	}
	dbg_print("VCE register base address (remapped) = 0X%p\n", vce_base);

	/* Map the RESET registers */
	mm_rst_base = (void __iomem *)ioremap_nocache(MM_RST_BASE_ADDR, SZ_4K);
	if (mm_rst_base == NULL)
		goto err1;

	/* Request the VCE IRQ */
	ret = request_irq(IRQ_VCE, vce_isr,
			  IRQF_DISABLED | IRQF_TRIGGER_RISING, VCE_DEV_NAME,
			  NULL);
	if (ret != 0) {
		err_print("request_irq failed ret = %d\n", ret);
		goto err2a;
	}

	/* Initialize the VCE acquire_sem and work_lock */
	sema_init(&vce_state.acquire_sem, 1);	//First request should succeed
	sema_init(&vce_state.work_lock, 1);	//First request should succeed

	vce_state.proc_vcedir = proc_mkdir(VCE_DEV_NAME, NULL);
	if (vce_state.proc_vcedir == NULL) {
		err_print("Failed to create vce proc dir\n");
		ret = -ENOENT;
		goto err2;
	}

	vce_state.proc_version = create_proc_entry("version",
						   (S_IRUSR | S_IRGRP),
						   vce_state.proc_vcedir);
	if (vce_state.proc_version == NULL) {
		err_print("Failed to create vce proc entry\n");
		ret = -ENOENT;
		goto err3;
	}
	vce_state.proc_version->read_proc = proc_version_read;

	vce_state.proc_status = create_proc_entry("status",
						  (S_IRUSR | S_IRGRP),
						  vce_state.proc_vcedir);
	if (vce_state.proc_status == NULL) {
		err_print("Failed to create vce proc entry\n");
		ret = -ENOENT;
		goto err4;
	}
	vce_state.proc_status->read_proc = proc_status_read;

	/* We need a QOS node for the CPU in order to do the ACP keep alive thing (simple wfi) */
	vce_state.cpu_qos_node =
	    pi_mgr_qos_add_request("vce", PI_MGR_PI_ID_ARM_CORE,
				   PI_MGR_QOS_DEFAULT_VALUE);
	if (vce_state.cpu_qos_node == NULL) {
		err_print("Failed to get QOS node for ARM core\n");
		ret = -ENOENT;
		goto err5;
	}

	return 0;

	/*
	   error exit paths
	 */

	// pi_mgr_qos_request_remove(vce_state.cpu_qos_node);
err5:

	remove_proc_entry("status", vce_state.proc_vcedir);
err4:

	remove_proc_entry("version", vce_state.proc_vcedir);
err3:
	remove_proc_entry(VCE_DEV_NAME, NULL);
err2:

	free_irq(IRQ_VCE, NULL);
err2a:

	iounmap(mm_rst_base);
	mm_rst_base = 0;
err1:

	iounmap(vce_base);
	vce_base = 0;
err:

	device_destroy(vce_state.vce_class, MKDEV(vce_major, 0));
errC:

	class_destroy(vce_state.vce_class);
errB:

	unregister_chrdev(vce_major, VCE_DEV_NAME);
errA:

	BUG_ON(ret >= 0);
	return ret;
}

void __exit vce_exit(void)
{
	dbg_print("VCE driver Exit\n");

	down(&vce_state.work_lock);
	down(&vce_state.clockctl_sem);
	down(&vce_state.armctl_sem);
	BUG_ON(vce_state.clock_enable_count != 0);

	/* remove proc entries */
	remove_proc_entry("status", vce_state.proc_vcedir);
	remove_proc_entry("version", vce_state.proc_vcedir);
	remove_proc_entry(VCE_DEV_NAME, NULL);

	/* free interrupts */
	free_irq(IRQ_VCE, NULL);

	/* Unmap addresses */
	if (vce_base)
		iounmap(vce_base);

	if (mm_rst_base)
		iounmap(mm_rst_base);

	pi_mgr_qos_request_remove(vce_state.cpu_qos_node);

	device_destroy(vce_state.vce_class, MKDEV(vce_major, 0));
	class_destroy(vce_state.vce_class);
	unregister_chrdev(vce_major, VCE_DEV_NAME);
}

module_init(vce_init);
module_exit(vce_exit);

MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("VCE device driver");
MODULE_LICENSE("GPL");

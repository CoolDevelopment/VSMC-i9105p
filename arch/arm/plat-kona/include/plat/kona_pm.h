/****************************************************************************
*									      
* Copyright 2010 --2011 Broadcom Corporation.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
*****************************************************************************/

#ifndef __KONA_PM_H__
#define __KONA_PM_H__

/* Additional cpuidle flags */
#define CPUIDLE_FLAG_XTAL_ON    (1 << 16)
#define CPUIDLE_ENTER_SUSPEND	(1 << 17)

struct kona_idle_state
{
	char*	name;
	char*	desc;
	u32		flags;
	u32 	state;
	u32 	latency; /* in uS */
	u32		target_residency; /* in uS */
	int (*enter)(struct kona_idle_state* state);
};

#ifdef CONFIG_CAPRI_DELAYED_PM_INIT
int kona_pm_init(void);
#else
int __init kona_pm_init(void);
#endif
extern void instrument_idle_entry(void);
extern void instrument_idle_exit(void);
#endif /*__KONA_PM_H__*/

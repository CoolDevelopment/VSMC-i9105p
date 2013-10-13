/*****************************************************************************
* Copyright 2010 - 2011 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

#ifndef _NET_SETTINGS_H
#define _NET_SETTINGS_H

#define HW_CFG_NET \
{ \
	.addrPhy0 = 0, \
	.addrPhy1 = 1, \
	.gpioPhy0 = 53, /* PHY0_RST_B */ \
	.gpioPhy1 = 55, /* PHY1_RST_B */ \
}
#endif /* _NET_SETTINGS_H */

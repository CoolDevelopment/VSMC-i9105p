/*****************************************************************************
* Copyright 2004 - 2009 Broadcom Corporation.  All rights reserved.
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

//---------------------------------------------------------------------------
//
// Design-specific hardware configuration interface file.
//
//---------------------------------------------------------------------------
#ifndef GPS_SETTINGS_H
#define GPS_SETTINGS_H

// ---- Include Files ----------------------------------------
// ---- Constants and Types ----------------------------------

#define GPS_nRESET      71
#define GPS_nSTANDBY    70
#define GPS_INT         72

#define GPS_PLATFORM_DATA_SETTINGS \
{  \
   .gpio_reset     = GPS_nRESET, \
   .gpio_power     = GPS_nSTANDBY, \
   .gpio_interrupt = -1, \
}

#endif /* GPS_SETTINGS_H */

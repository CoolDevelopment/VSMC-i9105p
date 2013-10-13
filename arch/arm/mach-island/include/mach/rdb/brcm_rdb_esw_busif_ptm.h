/************************************************************************************************/
/*                                                                                              */
/*  Copyright 2011  Broadcom Corporation                                                        */
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
/*     Date     : Generated on 4/25/2011 11:6:8                                             */
/*     RDB file : /projects/BIGISLAND/revA0                                                                   */
/************************************************************************************************/

#ifndef __BRCM_RDB_ESW_BUSIF_PTM_H__
#define __BRCM_RDB_ESW_BUSIF_PTM_H__

#define ESW_BUSIF_PTM_PTM_DMA_CONFIG_OFFSET                               0x00000000
#define ESW_BUSIF_PTM_PTM_DMA_CONFIG_TYPE                                 UInt32
#define ESW_BUSIF_PTM_PTM_DMA_CONFIG_RESERVED_MASK                        0xFFFFFFFF

#define ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_OFFSET                          0x00000008
#define ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_TYPE                            UInt32
#define ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_RESERVED_MASK                   0x0FF80000
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_CHANNEL_ENABLE_SHIFT     31
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_CHANNEL_ENABLE_MASK      0x80000000
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_SPEED_CHANGE_REQ_SHIFT   30
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_SPEED_CHANGE_REQ_MASK    0x40000000
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_SPEED_CHANGE_DONE_SHIFT  29
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_SPEED_CHANGE_DONE_MASK   0x20000000
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_SPEED_MODE_SHIFT         28
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_SPEED_MODE_MASK          0x10000000
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_BLOCK_SIZE_CONFIG_SHIFT  10
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_BLOCK_SIZE_CONFIG_MASK   0x0007FC00
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_INITIAL_OFFSET_SHIFT     6
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_INITIAL_OFFSET_MASK      0x000003C0
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_SINGLE_ACCESS_SHIFT      5
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_SINGLE_ACCESS_MASK       0x00000020
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_BURST_SIZE_SHIFT         1
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_BURST_SIZE_MASK          0x0000001E
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_INITIAL_OFFSET_ENABLE_SHIFT 0
#define    ESW_BUSIF_PTM_PTM_TRANSFER_CONFIG_PTM_INITIAL_OFFSET_ENABLE_MASK 0x00000001

#define ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_OFFSET                          0x00000010
#define ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_TYPE                            UInt32
#define ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_RESERVED_MASK                   0x40000000
#define    ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_PTM_SOP_SHIFT                31
#define    ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_PTM_SOP_MASK                 0x80000000
#define    ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_PTM_EOP_SHIFT                29
#define    ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_PTM_EOP_MASK                 0x20000000
#define    ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_PTM_PACKET_LENGTH_SHIFT      13
#define    ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_PTM_PACKET_LENGTH_MASK       0x1FFFE000
#define    ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_BLOCK_TRANSFER_COUNT_SHIFT   0
#define    ESW_BUSIF_PTM_PTM_TRANSFER_STATUS_BLOCK_TRANSFER_COUNT_MASK    0x00001FFF

#endif /* __BRCM_RDB_ESW_BUSIF_PTM_H__ */



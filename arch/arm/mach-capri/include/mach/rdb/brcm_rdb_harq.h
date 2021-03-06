/************************************************************************************************/
/*                                                                                              */
/*  Copyright 2012  Broadcom Corporation                                                        */
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
/*     Date     : Generated on 3/20/2012 13:8:57                                             */
/*     RDB file : /projects/CHIP/revA0                                                                   */
/************************************************************************************************/

#ifndef __BRCM_RDB_HARQ_H__
#define __BRCM_RDB_HARQ_H__

#define HARQ_CONFIG_OFFSET                                                0x00000000
#define HARQ_CONFIG_TYPE                                                  UInt32
#define HARQ_CONFIG_RESERVED_MASK                                         0x00010000
#define    HARQ_CONFIG_PARAM_TURBO_AUTOTRIGGER_SHIFT                      31
#define    HARQ_CONFIG_PARAM_TURBO_AUTOTRIGGER_MASK                       0x80000000
#define    HARQ_CONFIG_PARAM_CONSTELLATION_VER_SHIFT                      29
#define    HARQ_CONFIG_PARAM_CONSTELLATION_VER_MASK                       0x60000000
#define    HARQ_CONFIG_PARAM_QAM_SHIFT                                    28
#define    HARQ_CONFIG_PARAM_QAM_MASK                                     0x10000000
#define    HARQ_CONFIG_PARAM_NUMPHCH_SHIFT                                24
#define    HARQ_CONFIG_PARAM_NUMPHCH_MASK                                 0x0F000000
#define    HARQ_CONFIG_PARAM_GEN_INT_SHIFT                                23
#define    HARQ_CONFIG_PARAM_GEN_INT_MASK                                 0x00800000
#define    HARQ_CONFIG_PARAM_IR_CHASE_SHIFT                               22
#define    HARQ_CONFIG_PARAM_IR_CHASE_MASK                                0x00400000
#define    HARQ_CONFIG_PARAM_PAR_RM_SHIFT                                 20
#define    HARQ_CONFIG_PARAM_PAR_RM_MASK                                  0x00300000
#define    HARQ_CONFIG_PARAM_SYS_RM_SHIFT                                 18
#define    HARQ_CONFIG_PARAM_SYS_RM_MASK                                  0x000C0000
#define    HARQ_CONFIG_PARAM_64_QAM_SHIFT                                 17
#define    HARQ_CONFIG_PARAM_64_QAM_MASK                                  0x00020000
#define    HARQ_CONFIG_PARAM_NR_SHIFT                                     13
#define    HARQ_CONFIG_PARAM_NR_MASK                                      0x0000E000
#define    HARQ_CONFIG_PARAM_NC_SHIFT                                     0
#define    HARQ_CONFIG_PARAM_NC_MASK                                      0x00001FFF

#define HARQ_SYSPAR1PNTR_OFFSET                                           0x00000004
#define HARQ_SYSPAR1PNTR_TYPE                                             UInt32
#define HARQ_SYSPAR1PNTR_RESERVED_MASK                                    0x80008000
#define    HARQ_SYSPAR1PNTR_PARAM_PARITY1_BASE_SHIFT                      16
#define    HARQ_SYSPAR1PNTR_PARAM_PARITY1_BASE_MASK                       0x7FFF0000
#define    HARQ_SYSPAR1PNTR_PARAM_SYSTEMATIC_BASE_SHIFT                   0
#define    HARQ_SYSPAR1PNTR_PARAM_SYSTEMATIC_BASE_MASK                    0x00007FFF

#define HARQ_PAR2PNTR_OFFSET                                              0x00000008
#define HARQ_PAR2PNTR_TYPE                                                UInt32
#define HARQ_PAR2PNTR_RESERVED_MASK                                       0xFFFF8000
#define    HARQ_PAR2PNTR_PARAM_PARITY2_BASE_SHIFT                         0
#define    HARQ_PAR2PNTR_PARAM_PARITY2_BASE_MASK                          0x00007FFF

#define HARQ_RM2SYS_OFFSET                                                0x0000000C
#define HARQ_RM2SYS_TYPE                                                  UInt32
#define HARQ_RM2SYS_RESERVED_MASK                                         0x00000000
#define    HARQ_RM2SYS_PARAM_SYSTEMATIC_GAMMA_SHIFT                       16
#define    HARQ_RM2SYS_PARAM_SYSTEMATIC_GAMMA_MASK                        0xFFFF0000
#define    HARQ_RM2SYS_PARAM_SYSTEMATIC_OFFSET_SHIFT                      0
#define    HARQ_RM2SYS_PARAM_SYSTEMATIC_OFFSET_MASK                       0x0000FFFF

#define HARQ_RM2EPLUSPAR1_OFFSET                                          0x00000010
#define HARQ_RM2EPLUSPAR1_TYPE                                            UInt32
#define HARQ_RM2EPLUSPAR1_RESERVED_MASK                                   0x00000000
#define    HARQ_RM2EPLUSPAR1_PARAM_SYSTEMATIC_EPLUS_SHIFT                 16
#define    HARQ_RM2EPLUSPAR1_PARAM_SYSTEMATIC_EPLUS_MASK                  0xFFFF0000
#define    HARQ_RM2EPLUSPAR1_PARAM_PARITY1_EPLUS_SHIFT                    0
#define    HARQ_RM2EPLUSPAR1_PARAM_PARITY1_EPLUS_MASK                     0x0000FFFF

#define HARQ_RM2PAR1_OFFSET                                               0x00000014
#define HARQ_RM2PAR1_TYPE                                                 UInt32
#define HARQ_RM2PAR1_RESERVED_MASK                                        0x00000000
#define    HARQ_RM2PAR1_PARAM_PARITY1_GAMMA_SHIFT                         16
#define    HARQ_RM2PAR1_PARAM_PARITY1_GAMMA_MASK                          0xFFFF0000
#define    HARQ_RM2PAR1_PARAM_PARITY1_OFFSET_SHIFT                        0
#define    HARQ_RM2PAR1_PARAM_PARITY1_OFFSET_MASK                         0x0000FFFF

#define HARQ_RM2PAR2_OFFSET                                               0x00000018
#define HARQ_RM2PAR2_TYPE                                                 UInt32
#define HARQ_RM2PAR2_RESERVED_MASK                                        0x00000000
#define    HARQ_RM2PAR2_PARAM_PARITY2_GAMMA_SHIFT                         16
#define    HARQ_RM2PAR2_PARAM_PARITY2_GAMMA_MASK                          0xFFFF0000
#define    HARQ_RM2PAR2_PARAM_PARITY2_OFFSET_SHIFT                        0
#define    HARQ_RM2PAR2_PARAM_PARITY2_OFFSET_MASK                         0x0000FFFF

#define HARQ_RM2EPLUSPAR2_OFFSET                                          0x0000001C
#define HARQ_RM2EPLUSPAR2_TYPE                                            UInt32
#define HARQ_RM2EPLUSPAR2_RESERVED_MASK                                   0x00000000
#define    HARQ_RM2EPLUSPAR2_PARAM_CLEAR_ONLY_SHIFT                       31
#define    HARQ_RM2EPLUSPAR2_PARAM_CLEAR_ONLY_MASK                        0x80000000
#define    HARQ_RM2EPLUSPAR2_PARAM_CLEAR_LENGTH_SHIFT                     16
#define    HARQ_RM2EPLUSPAR2_PARAM_CLEAR_LENGTH_MASK                      0x7FFF0000
#define    HARQ_RM2EPLUSPAR2_PARAM_PARITY2_EPLUS_SHIFT                    0
#define    HARQ_RM2EPLUSPAR2_PARAM_PARITY2_EPLUS_MASK                     0x0000FFFF

#define HARQ_STATUS_OFFSET                                                0x0000002C
#define HARQ_STATUS_TYPE                                                  UInt32
#define HARQ_STATUS_RESERVED_MASK                                         0xFFFFFFFE
#define    HARQ_STATUS_HARQ_BUSY_SHIFT                                    0
#define    HARQ_STATUS_HARQ_BUSY_MASK                                     0x00000001

#define HARQ_DEMODMODE_OFFSET                                             0x00000030
#define HARQ_DEMODMODE_TYPE                                               UInt32
#define HARQ_DEMODMODE_RESERVED_MASK                                      0xFFFFFFFE
#define    HARQ_DEMODMODE_PARAM_SNR_DEMODULATION_MODE_SHIFT               0
#define    HARQ_DEMODMODE_PARAM_SNR_DEMODULATION_MODE_MASK                0x00000001

#define HARQ_SNRCONFIG_OFFSET                                             0x00000034
#define HARQ_SNRCONFIG_TYPE                                               UInt32
#define HARQ_SNRCONFIG_RESERVED_MASK                                      0xF8CEE000
#define    HARQ_SNRCONFIG_PARAM_SNR_NOISE_LB_SHIFT                        24
#define    HARQ_SNRCONFIG_PARAM_SNR_NOISE_LB_MASK                         0x07000000
#define    HARQ_SNRCONFIG_PARAM_SNR_MODE_SCALE_SHIFT                      20
#define    HARQ_SNRCONFIG_PARAM_SNR_MODE_SCALE_MASK                       0x00300000
#define    HARQ_SNRCONFIG_PARAM_SNR_WINDOW_REMAINDER_SHIFT                16
#define    HARQ_SNRCONFIG_PARAM_SNR_WINDOW_REMAINDER_MASK                 0x00010000
#define    HARQ_SNRCONFIG_PARAM_SNR_WINDOW_LENGTH_SHIFT                   4
#define    HARQ_SNRCONFIG_PARAM_SNR_WINDOW_LENGTH_MASK                    0x00001FF0
#define    HARQ_SNRCONFIG_PARAM_SNR_NUM_WINDOWS_SHIFT                     0
#define    HARQ_SNRCONFIG_PARAM_SNR_NUM_WINDOWS_MASK                      0x0000000F

#define HARQ_SNRCONST_OFFSET                                              0x00000038
#define HARQ_SNRCONST_TYPE                                                UInt32
#define HARQ_SNRCONST_RESERVED_MASK                                       0xFFFFFFCE
#define    HARQ_SNRCONST_PARAM_SNR_CONST_SCALE_SHIFT                      4
#define    HARQ_SNRCONST_PARAM_SNR_CONST_SCALE_MASK                       0x00000030
#define    HARQ_SNRCONST_PARAM_SNR_ENABLE_CONST_SCALE_SHIFT               0
#define    HARQ_SNRCONST_PARAM_SNR_ENABLE_CONST_SCALE_MASK                0x00000001

#define HARQ_EXTRM2_OFFSET                                                0x0000003C
#define HARQ_EXTRM2_TYPE                                                  UInt32
#define HARQ_EXTRM2_RESERVED_MASK                                         0xFFFFFE00
#define    HARQ_EXTRM2_PARAM_EXTENDED_RM_SHIFT                            0
#define    HARQ_EXTRM2_PARAM_EXTENDED_RM_MASK                             0x000001FF

#define HARQ_EXTSYSEPLUS_OFFSET                                           0x00000040
#define HARQ_EXTSYSEPLUS_TYPE                                             UInt32
#define HARQ_EXTSYSEPLUS_RESERVED_MASK                                    0xFFFC0000
#define    HARQ_EXTSYSEPLUS_PARAM_EXTENDED_SYS_EPLUS_SHIFT                0
#define    HARQ_EXTSYSEPLUS_PARAM_EXTENDED_SYS_EPLUS_MASK                 0x0003FFFF

#define HARQ_EXTSYSGAMMA_OFFSET                                           0x00000044
#define HARQ_EXTSYSGAMMA_TYPE                                             UInt32
#define HARQ_EXTSYSGAMMA_RESERVED_MASK                                    0xFFFC0000
#define    HARQ_EXTSYSGAMMA_PARAM_EXTENDED_SYS_GAMMA_SHIFT                0
#define    HARQ_EXTSYSGAMMA_PARAM_EXTENDED_SYS_GAMMA_MASK                 0x0003FFFF

#define HARQ_EXTSYSOFFSET_OFFSET                                          0x00000048
#define HARQ_EXTSYSOFFSET_TYPE                                            UInt32
#define HARQ_EXTSYSOFFSET_RESERVED_MASK                                   0xFFFC0000
#define    HARQ_EXTSYSOFFSET_PARAM_EXTENDED_SYS_OFFSET_SHIFT              0
#define    HARQ_EXTSYSOFFSET_PARAM_EXTENDED_SYS_OFFSET_MASK               0x0003FFFF

#define HARQ_EXTP1EPLUS_OFFSET                                            0x0000004C
#define HARQ_EXTP1EPLUS_TYPE                                              UInt32
#define HARQ_EXTP1EPLUS_RESERVED_MASK                                     0xFFFC0000
#define    HARQ_EXTP1EPLUS_PARAM_EXTENDED_P1_EPLUS_SHIFT                  0
#define    HARQ_EXTP1EPLUS_PARAM_EXTENDED_P1_EPLUS_MASK                   0x0003FFFF

#define HARQ_EXTP1GAMMA_OFFSET                                            0x00000050
#define HARQ_EXTP1GAMMA_TYPE                                              UInt32
#define HARQ_EXTP1GAMMA_RESERVED_MASK                                     0xFFFC0000
#define    HARQ_EXTP1GAMMA_PARAM_EXTENDED_P1_GAMMA_SHIFT                  0
#define    HARQ_EXTP1GAMMA_PARAM_EXTENDED_P1_GAMMA_MASK                   0x0003FFFF

#define HARQ_EXTP1OFFSET_OFFSET                                           0x00000054
#define HARQ_EXTP1OFFSET_TYPE                                             UInt32
#define HARQ_EXTP1OFFSET_RESERVED_MASK                                    0xFFFC0000
#define    HARQ_EXTP1OFFSET_PARAM_EXTENDED_P1_OFFSET_SHIFT                0
#define    HARQ_EXTP1OFFSET_PARAM_EXTENDED_P1_OFFSET_MASK                 0x0003FFFF

#define HARQ_EXTP2EPLUS_OFFSET                                            0x00000058
#define HARQ_EXTP2EPLUS_TYPE                                              UInt32
#define HARQ_EXTP2EPLUS_RESERVED_MASK                                     0xFFFC0000
#define    HARQ_EXTP2EPLUS_PARAM_EXTENDED_P2_EPLUS_SHIFT                  0
#define    HARQ_EXTP2EPLUS_PARAM_EXTENDED_P2_EPLUS_MASK                   0x0003FFFF

#define HARQ_EXTP2GAMMA_OFFSET                                            0x0000005C
#define HARQ_EXTP2GAMMA_TYPE                                              UInt32
#define HARQ_EXTP2GAMMA_RESERVED_MASK                                     0xFFFC0000
#define    HARQ_EXTP2GAMMA_PARAM_EXTENDED_P2_GAMMA_SHIFT                  0
#define    HARQ_EXTP2GAMMA_PARAM_EXTENDED_P2_GAMMA_MASK                   0x0003FFFF

#define HARQ_EXTP2OFFSET_OFFSET                                           0x00000060
#define HARQ_EXTP2OFFSET_TYPE                                             UInt32
#define HARQ_EXTP2OFFSET_RESERVED_MASK                                    0xFFFC0000
#define    HARQ_EXTP2OFFSET_PARAM_EXTENDED_P2_OFFSET_SHIFT                0
#define    HARQ_EXTP2OFFSET_PARAM_EXTENDED_P2_OFFSET_MASK                 0x0003FFFF

#define HARQ_OVRAVEAMPEST_OFFSET                                          0x00000064
#define HARQ_OVRAVEAMPEST_TYPE                                            UInt32
#define HARQ_OVRAVEAMPEST_RESERVED_MASK                                   0xFFFFEE00
#define    HARQ_OVRAVEAMPEST_PARAM_OVERRIDE_SYM_AMP_SHIFT                 12
#define    HARQ_OVRAVEAMPEST_PARAM_OVERRIDE_SYM_AMP_MASK                  0x00001000
#define    HARQ_OVRAVEAMPEST_PARAM_FW_SYM_AMP_EST_SHIFT                   0
#define    HARQ_OVRAVEAMPEST_PARAM_FW_SYM_AMP_EST_MASK                    0x000001FF

#define HARQ_RXBUFREADMODE_OFFSET                                         0x00000068
#define HARQ_RXBUFREADMODE_TYPE                                           UInt32
#define HARQ_RXBUFREADMODE_RESERVED_MASK                                  0xFFFFFFFE
#define    HARQ_RXBUFREADMODE_PARAM_RX_BUFF_APB_READ_MODE_SHIFT           0
#define    HARQ_RXBUFREADMODE_PARAM_RX_BUFF_APB_READ_MODE_MASK            0x00000001

#define HARQ_RXBUFREADADDR_OFFSET                                         0x0000006C
#define HARQ_RXBUFREADADDR_TYPE                                           UInt32
#define HARQ_RXBUFREADADDR_RESERVED_MASK                                  0xFFFFE000
#define    HARQ_RXBUFREADADDR_PARAM_RX_BUFF_APB_READ_ADDR_SHIFT           0
#define    HARQ_RXBUFREADADDR_PARAM_RX_BUFF_APB_READ_ADDR_MASK            0x00001FFF

#define HARQ_RXBUFREADMARKERS_OFFSET                                      0x00000070
#define HARQ_RXBUFREADMARKERS_TYPE                                        UInt32
#define HARQ_RXBUFREADMARKERS_RESERVED_MASK                               0xE000E000
#define    HARQ_RXBUFREADMARKERS_APB_RX_BUFF_LAST_WR_ADDR_SHIFT           16
#define    HARQ_RXBUFREADMARKERS_APB_RX_BUFF_LAST_WR_ADDR_MASK            0x1FFF0000
#define    HARQ_RXBUFREADMARKERS_RX_BUFF_TTI_MARKER_SHIFT                 0
#define    HARQ_RXBUFREADMARKERS_RX_BUFF_TTI_MARKER_MASK                  0x00001FFF

#define HARQ_RXBUFREADIQDATA_OFFSET                                       0x00000074
#define HARQ_RXBUFREADIQDATA_TYPE                                         UInt32
#define HARQ_RXBUFREADIQDATA_RESERVED_MASK                                0xFC00FC00
#define    HARQ_RXBUFREADIQDATA_APB_READ_RX_BUFFER_Q_SHIFT                16
#define    HARQ_RXBUFREADIQDATA_APB_READ_RX_BUFFER_Q_MASK                 0x03FF0000
#define    HARQ_RXBUFREADIQDATA_APB_READ_RX_BUFFER_I_SHIFT                0
#define    HARQ_RXBUFREADIQDATA_APB_READ_RX_BUFFER_I_MASK                 0x000003FF

#endif /* __BRCM_RDB_HARQ_H__ */



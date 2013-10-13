/*****************************************************************************
*  Copyright 2001 - 2009 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/licenses/old-license/gpl-2.0.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/

/*****************************************************************************
*
*    (c) 2001-2009 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or
* its licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.
* IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS
* SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
* ALL USE OF THE SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1. This program, including its structure, sequence and organization,
*    constitutes the valuable trade secrets of Broadcom, and you shall use all
*    reasonable efforts to protect the confidentiality thereof, and to use
*    this information only in connection with your use of Broadcom integrated
*    circuit products.
*
* 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
*    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
*    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*    IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS
*    FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS,
*    QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU
*    ASSUME THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
*    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
*    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
*    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
*    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
*    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************/
/**
*
*  @file   chal_analogtest.c
*
*  @brief  chal layer driver for audio device driver that contains analog testing path functions
*             (API and analog loopback)
*
****************************************************************************/

#include <chal/chal_audio.h>
#include <chal/chal_audio_int.h>
#include <mach/rdb/brcm_rdb_audioh.h>
#include <chal/chal_util.h>

/*
 * ****************************************************************************
 *                         G L O B A L   S E C T I O N
 * ****************************************************************************
 */

/*
 * ****************************************************************************
 *  global variable definitions
 * ****************************************************************************
 */


/*
 * ****************************************************************************
 *                          L O C A L   S E C T I O N
 * ****************************************************************************
 */

/*
 * ****************************************************************************
 *  local macro declarations
 * ****************************************************************************
 */


/*
 * ****************************************************************************
 *  local typedef declarations
 * ****************************************************************************
 */



/*
 * ****************************************************************************
 *  local variable definitions
 * ****************************************************************************
 */


/*
 * ****************************************************************************
 *  local function declarations
 * ****************************************************************************
 */



/*
 * ******************************************************************************
 *  local function definitions
 * ******************************************************************************
 */




/*
 * ============================================================================
 * 
 *  Function Name: void chal_audio_enable_dac_paths(CHAL_HANDLE handle, uint32_t dac_mask)
 * 
 *  Description:  Enable the DAC paths specified by the dac_mask for the API Mux
 * 
 *  Parameters:   handle  - audio chal handle
 *                  dac_mask   - bit mask of DAC
 *                  enable - true : enable, false : disable.
 *  Return:       None.
 * 
 *  Note:        Do not call this function several times for enabling different DAC paths. Call it once with all the DAC paths
 *                  that needs to be enabled
 * ============================================================================
 */

void chal_audio_enable_dac_paths(CHAL_HANDLE handle, uint32_t dac_mask, uint16_t enable)
{
    uint32_t     reg_val;
    uint32_t     reg_dac_mask = 0;
    uint32_t base =    ((ChalAudioCtrlBlk_t*)handle)->audioh_base;



    if(dac_mask& CHAL_AUDIO_PATH_HEADSET_LEFT)
    {
        reg_dac_mask |= AUDIOH_DAC_CTL_STEREO_L_ENABLE_MASK;
    }

    if(dac_mask& CHAL_AUDIO_PATH_HEADSET_RIGHT)
    {
        reg_dac_mask |= AUDIOH_DAC_CTL_STEREO_R_ENABLE_MASK;
    }

    if(dac_mask&CHAL_AUDIO_PATH_IHF_LEFT)
    {
        reg_dac_mask |= AUDIOH_DAC_CTL_IHF_L_ENABLE_MASK;
    }

    if(dac_mask&CHAL_AUDIO_PATH_VIBRA)
    {
        reg_dac_mask |= AUDIOH_DAC_CTL_VIBRA_ENABLE_MASK;
    }

    if(dac_mask&CHAL_AUDIO_PATH_EARPIECE)
    {
        reg_dac_mask |= AUDIOH_DAC_CTL_VOUT_ENABLE_MASK;
    }



    /*
     *  Enable/Disable the DAC Paths.
     * It is necessary to enable all the paths at the same time
     */
    reg_val = BRCM_READ_REG(base, AUDIOH_DAC_CTL);
    reg_val &= (~reg_dac_mask);
    if(enable == CHAL_AUDIO_ENABLE)
    {
        reg_val |= reg_dac_mask;
    }
    BRCM_WRITE_REG(base, AUDIOH_DAC_CTL, reg_val);

    return;
}


/*
 * ============================================================================
 * 
 *  Function Name: void chal_audio_enable_adc_paths(CHAL_HANDLE handle, uint32_t dac_mask)
 * 
 *  Description:  Enable the DAC paths specified by the dac_mask for the API Mux
 * 
 *  Parameters:   handle  - audio chal handle
 *                  adc_mask   - bit mask of ADC
 *                  enable - true : enable, false : disable.
 *  Return:       None.
 * 
 *  Note:        Do not call this function several times for enabling different DAC paths. Call it once with all the DAC paths
 *                  that needs to be enabled
 * ============================================================================
 */

void chal_audio_enable_adc_paths(CHAL_HANDLE handle, uint32_t adc_mask, uint16_t enable)
{
    uint32_t     reg_val;
    uint32_t     reg_adc_mask = 0;
    uint32_t base =    ((ChalAudioCtrlBlk_t*)handle)->audioh_base;


    if((adc_mask&CHAL_AUDIO_PATH_AMIC1) || (adc_mask&CHAL_AUDIO_PATH_AMIC2))
    {
        reg_adc_mask |= AUDIOH_ADC_CTL_AMIC_EN_MASK;
    }

    if(adc_mask& CHAL_AUDIO_PATH_DMIC1)
    {
        reg_adc_mask |= AUDIOH_ADC_CTL_DMIC1_EN_MASK;
    }

    if(adc_mask& CHAL_AUDIO_PATH_DMIC2)
    {
        reg_adc_mask |= AUDIOH_ADC_CTL_DMIC2_EN_MASK;
    }

    if(adc_mask&CHAL_AUDIO_PATH_DMIC3)
    {
        reg_adc_mask |= AUDIOH_ADC_CTL_DMIC3_EN_MASK;
    }

    if(adc_mask&CHAL_AUDIO_PATH_DMIC4)
    {
        reg_adc_mask |= AUDIOH_ADC_CTL_DMIC4_EN_MASK;
    }

    /*
     *  Enable/Disable the DAC Paths.
     * It is necessary to enable all the paths at the same time
     */
    reg_val = BRCM_READ_REG(base, AUDIOH_ADC_CTL);
    reg_val &= (~reg_adc_mask);
    if(enable == CHAL_AUDIO_ENABLE)
    {
        reg_val |= reg_adc_mask;
    }
    BRCM_WRITE_REG(base, AUDIOH_ADC_CTL, reg_val);

    return;
}


/*
 * ============================================================================
 * 
 *  Function Name: void chal_audio_api_enable_dac(CHAL_HANDLE handle, uint32_t dac_mask)
 * 
 *  Description:  Enable the DAC paths specified by the dac_mask for the API Mux
 * 
 *  Parameters:   handle  - audio chal handle
 *                  dac_mask   - bit mask of DAC
 *                  enable - true : enable, false : disable.
 *  Return:       None.
 * 
 *  Note:        Do not call this function several times for enabling different DAC paths. Call it once with all the DAC paths
 *                  that needs to be enabled
 * ============================================================================
 */

void chal_audio_api_enable_dac(CHAL_HANDLE handle, uint32_t dac_mask, uint16_t enable)
{
    uint32_t     reg_val;
    uint32_t     reg_api_mask = 0;
    uint32_t base =    ((ChalAudioCtrlBlk_t*)handle)->audioh_base;



    if(dac_mask& CHAL_AUDIO_PATH_HEADSET_LEFT)
    {
        reg_api_mask |= AUDIOH_AUDIO_API_STEREO_API_EN_MASK;
    }

    if(dac_mask& CHAL_AUDIO_PATH_HEADSET_RIGHT)
    {
        reg_api_mask |= AUDIOH_AUDIO_API_STEREO_API_EN_MASK;
    }

    if(dac_mask&CHAL_AUDIO_PATH_IHF_LEFT)
    {
        reg_api_mask |= AUDIOH_AUDIO_API_IHF_API_EN_MASK;
    }

    if(dac_mask&CHAL_AUDIO_PATH_VIBRA)
    {
        reg_api_mask |= AUDIOH_AUDIO_API_VIBRA_API_EN_MASK;
    }

    if(dac_mask&CHAL_AUDIO_PATH_EARPIECE)
    {
        reg_api_mask |= AUDIOH_AUDIO_API_VOUT_API_EN_MASK;
    }

    /* Enable/Disable the API for the DAC Paths.*/
    reg_val = BRCM_READ_REG(base, AUDIOH_AUDIO_API);
    reg_val &= (~reg_api_mask);
    if(enable == CHAL_AUDIO_ENABLE)
    {
        reg_val |= reg_api_mask;
    }
    /* Set the required setting */
    BRCM_WRITE_REG(base,  AUDIOH_AUDIO_API, reg_val);

    return;
}



/*
 * ============================================================================
 * 
 *  Function Name: void chal_audio_api_set_dac_attenuation(CHAL_HANDLE handle, uint32_t dac_mask)
 * 
 *  Description:  Enable/disable the DAC paths specified by the dac_mask for the API attenuation
 * 
 *  Parameters:   handle  - audio chal handle
 *                  dac_mask   - bit mask of DAC
 *  Return:       None.
 * 
 *  Notes:       dac_mask of CHAL_AUDIO_PATH_VIBRA has no effect as chip doesn't support this feature for vibra path
 * ============================================================================
 */

void chal_audio_api_set_dac_attenuation(CHAL_HANDLE handle, uint32_t dac_mask, uint16_t enable)
{
    uint32_t     reg_val;
    uint32_t     reg_dac_mask = 0x00000000;
    uint32_t base =    ((ChalAudioCtrlBlk_t*)handle)->audioh_base;


    /* Enable/Disable the API attenuation for the DAC Paths.*/
    if(dac_mask& (CHAL_AUDIO_PATH_HEADSET_LEFT|CHAL_AUDIO_PATH_HEADSET_RIGHT))
    {
        reg_dac_mask |= AUDIOH_AUDIO_API_STEREO_API_ATT_MASK;
    }

    if(dac_mask&CHAL_AUDIO_PATH_IHF_LEFT)
    {
        reg_dac_mask |= AUDIOH_AUDIO_API_IHF_API_ATT_MASK;
    }

    if(dac_mask&CHAL_AUDIO_PATH_EARPIECE)
    {
        reg_dac_mask |= AUDIOH_AUDIO_API_VOUT_API_ATT_MASK;
    }

    reg_val = BRCM_READ_REG(base, AUDIOH_AUDIO_API);
    reg_val &= (~reg_dac_mask);
    if(enable == CHAL_AUDIO_ENABLE)
    {
        reg_val |= (reg_dac_mask);
    }
    /* Set the required setting */
    BRCM_WRITE_REG(base,  AUDIOH_AUDIO_API, reg_val);

    return;
}

/*
 * ============================================================================
 * 
 *  Function Name: void chal_audio_api_enable_adc(CHAL_HANDLE handle, uint32_t adc_mask)
 * 
 *  Description:  Enable the ADC paths specified by the adc_mask for the API Mux
 * 
 *  Parameters:   handle  - audio chal handle
 *                  adc_mask   - bit mask of ADC
 *  Return:       None.
 * 
 * ============================================================================
 */

void chal_audio_api_enable_adc(CHAL_HANDLE handle, uint32_t adc_mask, uint16_t enable)
{
    uint32_t     reg_val;
    uint32_t     reg_api_mask  = 0x0000;
    uint32_t base =    ((ChalAudioCtrlBlk_t*)handle)->audioh_base;

    if(adc_mask&CHAL_AUDIO_PATH_DMIC1)
    {
        reg_api_mask |= AUDIOH_AUDIO_API_VIN_API_EN_MASK;
    }

    /* Enable/Disable the API for the DAC Paths.*/
    reg_val = BRCM_READ_REG(base, AUDIOH_AUDIO_API);
    reg_val &= (~reg_api_mask);
    if(enable == CHAL_AUDIO_ENABLE)
    {
        reg_val |= reg_api_mask;
    }
    /* Set the required setting */
    BRCM_WRITE_REG(base,  AUDIOH_AUDIO_API, reg_val);

    return;
}

/*
 * ============================================================================
 * 
 *  Function Name: void chal_audio_loopback_enable(CHAL_HANDLE handle, uint16_t enable)
 * 
 *  Description:  Enable or Disable analog mic loopback
 * 
 *  Parameters:   handle - audio chal handle.
 *                  enable - true : enable, false : disable.
 *  Return:       None.
 * 
 * ============================================================================
 */
void chal_audio_loopback_enable(CHAL_HANDLE handle, uint16_t enable)
{
    uint32_t base =    ((ChalAudioCtrlBlk_t*)handle)->audioh_base;
    uint32_t reg_val;


    reg_val = BRCM_READ_REG(base, AUDIOH_LOOPBACK_CTRL);
    reg_val &= ~(AUDIOH_LOOPBACK_CTRL_LOOPBACK_EN_MASK);
    if(enable == CHAL_AUDIO_ENABLE)
    {
        reg_val |= AUDIOH_LOOPBACK_CTRL_LOOPBACK_EN_MASK;
    }


    /* Set the required setting */
    BRCM_WRITE_REG(base,  AUDIOH_LOOPBACK_CTRL, reg_val);

    return;
}


/*
 * ============================================================================
 * 
 *  Function Name: void chal_audio_loopback_set_out_paths(CHAL_HANDLE handle, uint16_t enable)
 * 
 *  Description:  Enable or Disable analog mic loopback to the mentioned dac path
 * 
 *  Parameters:   handle     - audio chal handle.
 *                  dac_mask     - bit mask of DAC
 *                  enable          - true : enable, false : disable.
 *  Return:       None.
 * 
 * ============================================================================
 */
void chal_audio_loopback_set_out_paths(CHAL_HANDLE handle, uint32_t dac_mask, uint16_t enable)
{
    uint32_t base =    ((ChalAudioCtrlBlk_t*)handle)->audioh_base;
    uint32_t reg_val;
    uint32_t regDacMask = 0;


    if(dac_mask& CHAL_AUDIO_PATH_HEADSET_LEFT)
    {
        regDacMask |= AUDIOH_LOOPBACK_CTRL_STEREO_LOOPBACK_L_MASK;

    }

    if(dac_mask& CHAL_AUDIO_PATH_HEADSET_RIGHT)
    {
        regDacMask |= AUDIOH_LOOPBACK_CTRL_STEREO_LOOPBACK_R_MASK;
    }

    if(dac_mask&CHAL_AUDIO_PATH_IHF_LEFT)
    {
        regDacMask |= AUDIOH_LOOPBACK_CTRL_IHF_LOOPBACK_L_MASK;
    }

    if(dac_mask&CHAL_AUDIO_PATH_EARPIECE)
    {
        regDacMask |= AUDIOH_LOOPBACK_CTRL_VOUT_LOOPBACK_MASK;
    }


    reg_val = BRCM_READ_REG(base, AUDIOH_LOOPBACK_CTRL);

    reg_val &= (~regDacMask);

    if(enable == CHAL_AUDIO_ENABLE)
    {
        reg_val |= regDacMask;
    }

    /* Set the required setting */
    BRCM_WRITE_REG(base,  AUDIOH_LOOPBACK_CTRL, reg_val);

    return;
}


/*
 * ============================================================================
 * 
 *  Function Name: void chal_audio_audiotx_set_dac_ctrl(CHAL_HANDLE handle, uint32_t ctrl)
 * 
 *  Description:  Enable or Disable external pop click for Ear piece path
 * 
 *  Parameters:   handle             - audio chal handle.
 *                       ctrl                  - Configuration that sets AUDIOTX. CLK_INV, CLK_PD, BB_STI & TEST_EN
 * 
 *  Return:       None.
 * 
 * ============================================================================
 */
void chal_audio_audiotx_set_dac_ctrl(CHAL_HANDLE handle, uint32_t ctrl)
{
    uint32_t base =    ((ChalAudioCtrlBlk_t*)handle)->audioh_base;

    ctrl &= (AUDIOH_DAC_CTRL_AUDIOTX_I_CLK_INV_MASK|AUDIOH_DAC_CTRL_AUDIOTX_I_CLK_PD_MASK);
    ctrl &= (AUDIOH_DAC_CTRL_AUDIOTX_TEST_EN_MASK|AUDIOH_DAC_CTRL_AUDIOTX_BB_STI_MASK);

    /* Set the required setting */
    BRCM_WRITE_REG(base, AUDIOH_DAC_CTRL, ctrl);

    return;
}


/*
 * ============================================================================
 * 
 *  Function Name: void chal_audio_audiotx_get_dac_ctrl(CHAL_HANDLE handle)
 * 
 *  Description:  Get the current DAC AUDIOTX configuration
 * 
 *  Parameters:   handle - audio chal handle.
 * 
 *  Return:       Current DAC AUDIOTX setting
 * 
 * ============================================================================
 */
uint32_t chal_audio_audiotx_get_dac_ctrl(CHAL_HANDLE handle)
{
    uint32_t base =    ((ChalAudioCtrlBlk_t*)handle)->audioh_base;
    uint32_t ctrl;

    ctrl = BRCM_READ_REG(base, AUDIOH_DAC_CTRL);

    ctrl &= (AUDIOH_DAC_CTRL_AUDIOTX_I_CLK_INV_MASK|AUDIOH_DAC_CTRL_AUDIOTX_I_CLK_PD_MASK);
    ctrl &= (AUDIOH_DAC_CTRL_AUDIOTX_EP_DRV_STO_MASK|AUDIOH_DAC_CTRL_AUDIOTX_BB_STI_MASK);
    ctrl &= (AUDIOH_DAC_CTRL_AUDIOTX_TEST_EN_MASK);

    return ctrl;
}


/**
*
* Function Name: cVoid chal_audio_audiotx_set_spare_bit(CHAL_HANDLE handle)
*
* Description:  Set the audiotx spare bit
*
* Parameters:   handle             - audio chal handle.
*
* Return:       None.
*
*****************************************************************************/
void chal_audio_audiotx_set_spare_bit(CHAL_HANDLE handle)
{
    uint32_t base =    ((ChalAudioCtrlBlk_t*)handle)->audioh_base;
    uint32_t ctrl;

    ctrl = BRCM_READ_REG(base, AUDIOH_DAC_CTRL);
    ctrl |= (1<<AUDIOH_DAC_CTRL_AUDIOTX_SPARE_BIT_SHIFT);

    /* Set the required setting */
    BRCM_WRITE_REG(base, AUDIOH_DAC_CTRL, ctrl);

    return;
}

#if defined( __KERNEL__ )

#include <linux/module.h>

EXPORT_SYMBOL(chal_audio_api_enable_dac);
EXPORT_SYMBOL(chal_audio_api_set_dac_attenuation);
EXPORT_SYMBOL(chal_audio_api_enable_adc);
EXPORT_SYMBOL(chal_audio_loopback_enable);
EXPORT_SYMBOL(chal_audio_loopback_set_out_paths);
EXPORT_SYMBOL(chal_audio_audiotx_set_dac_ctrl);
EXPORT_SYMBOL(chal_audio_audiotx_get_dac_ctrl);
EXPORT_SYMBOL(chal_audio_audiotx_set_spare_bit);

#endif

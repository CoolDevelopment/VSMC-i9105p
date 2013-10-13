/*******************************************************************************
* Copyright 2011 Broadcom Corporation.  All rights reserved.
*
*             @file     drivers/video/broadcom/dispdrv_bcm91008_alex_dsi.c
*
* Unless you and Broadcom execute a separate written software license agreement
* governing use of this software, this software is licensed to you under the
* terms of the GNU General Public License version 2, available at
* http://www.gnu.org/copyleft/gpl.html (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
*******************************************************************************/
#define UNDER_LINUX

#ifndef UNDER_LINUX
#include <stdio.h>
#include <string.h>
#include "dbg.h"
#include "mobcom_types.h"
#include "chip_version.h"
#else
#include <linux/string.h>
#include <linux/broadcom/mobcom_types.h>
#endif


#ifndef UNDER_LINUX
#include "gpio.h"                  // needed for GPIO defs 4 platform_config
#include "platform_config.h"
#include "irqctrl.h"
#include "osinterrupt.h"
#include "ostask.h"
#else
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <plat/osabstract/osinterrupt.h>
#include <plat/osabstract/ostask.h>
#include <plat/csl/csl_dma_vc4lite.h>
#endif


#ifndef UNDER_LINUX
#include "dbg.h"
#include "logapi.h"
#include "dma_drv.h"
#include "display_drv.h"           // display driver interface
#include "csl_lcd.h"               // LCD CSL Common Interface 
#include "csl_dsi.h"               // DSI CSL 
#include "dispdrv_mipi_dcs.h"      // MIPI DCS         
#include "dispdrv_mipi_dsi.h"      // MIPI DSI      
#else
#include <plat/dma_drv.h>
#include "display_drv.h"           // display driver interface
#include <plat/csl/csl_lcd.h>               // LCD CSL Common Interface 
#include <plat/csl/csl_dsi.h>               // DSI CSL 
#include "dispdrv_mipi_dcs.h"      // MIPI DCS         
#include "dispdrv_mipi_dsi.h"      // MIPI DSI      
#endif   

#if defined( _ATHENA_)
#include "syscfg_drv.h"            // before #include "dispdrv_common.h"
#endif

#include "dispdrv_common.h"        // Disp Drv Commons
#include "lcd_clock.h"

#if (defined (_HERA_) || defined(_RHEA_))
#ifndef UNDER_LINUX
#include "csl_tectl_vc4lite.h"     // TE Input Control
#else
#include <plat/csl/csl_tectl_vc4lite.h>
#endif
#endif

#define BCM91008_ALEX_VC            (0)
#define BCM91008_ALEX_CMND_IS_LP    TRUE  // display init comm LP or HS mode

#define GPIODRV_Set_Bit(pin, val) gpio_set_value_cansleep(pin, val)

//#undef HAL_LCD_RESET
//#define HAL_LCD_RESET_B  (KONA_MAX_GPIO + 3)
//#define HAL_LCD_RESET_C  (KONA_MAX_GPIO + 2)

typedef struct
{
    UInt32              left;                
    UInt32              right;                  
    UInt32              top;  
    UInt32              bottom;  
    UInt32              width; 
    UInt32              height;
} LCD_DRV_RECT_t;

typedef struct
{
    CSL_LCD_HANDLE      clientH;        // DSI Client Handle
    CSL_LCD_HANDLE      dsiCmVcHandle;  // DSI CM VC Handle
    DISP_DRV_STATE      drvState;
    DISP_PWR_STATE      pwrState;
    UInt32              busId;
    UInt32              teIn;
    UInt32              teOut;
    LCD_DRV_RECT_t      win;
    void*               pFb;
    void*               pFbA;
    struct pi_mgr_dfs_node* dfs_node;
} BCM91008_ALEX_PANEL_T;   


// LOCAL FUNCTIONs
static int      bcm91008_alex_IoCtlRd( 
                    DISPDRV_HANDLE_T        drvH, 
                    DISPDRV_CTRL_RW_REG*   acc );
                    
static void     bcm91008_alex_IoCtlWr( 
                    DISPDRV_HANDLE_T        drvH, 
                    DISPDRV_CTRL_RW_REG*   acc );

static void     bcm91008_alex_WrCmndP0  ( 
                    DISPDRV_HANDLE_T        drvH, 
                    UInt32                  reg );
                    
static void     bcm91008_alex_WrCmndP1  ( 
                    DISPDRV_HANDLE_T        drvH, 
                    UInt32                  reg,
                    UInt32                  val );

// DRV INTERFACE FUNCTIONs
Int32           BCM91008_ALEX_Init          ( void ); 
Int32           BCM91008_ALEX_Exit          ( void ); 

Int32           BCM91008_ALEX_Open          ( 
                   const void*         params, 
                   DISPDRV_HANDLE_T*   drvH ); 
                   
Int32           BCM91008_ALEX_Close         ( DISPDRV_HANDLE_T drvH ); 

Int32           BCM91008_ALEX_GetDispDrvFeatures ( 
                   const char**                driver_name,
                   UInt32*                     version_major,
                   UInt32*                     version_minor,
                   DISPDRV_SUPPORT_FEATURES_T* flags );

const DISPDRV_INFO_T* BCM91008_ALEX_GetDispDrvData ( DISPDRV_HANDLE_T drvH );

Int32           BCM91008_ALEX_Start         ( DISPDRV_HANDLE_T drvH ); 
Int32           BCM91008_ALEX_Stop          ( DISPDRV_HANDLE_T drvH ); 

Int32           BCM91008_ALEX_PowerControl  ( DISPDRV_HANDLE_T drvH, 
                   DISPLAY_POWER_STATE_T state ); 

Int32           BCM91008_ALEX_Update       ( 
                    DISPDRV_HANDLE_T    drvH, 
		    int			fb_idx,
                    DISPDRV_CB_T        apiCb ); 

Int32           BCM91008_ALEX_Update_ExtFb ( 
                    DISPDRV_HANDLE_T        drvH, 
                    void                    *pFb,
                    DISPDRV_CB_API_1_1_T    apiCb ); 

Int32           BCM91008_ALEX_SetCtl ( 
                    DISPDRV_HANDLE_T    drvH, 
                    DISPDRV_CTRL_ID_T   ctrlID, 
                    void*               ctrlParams );
                   
Int32           BCM91008_ALEX_GetCtl (
                    DISPDRV_HANDLE_T    drvH, 
                    DISPDRV_CTRL_ID_T   ctrlID, 
                    void*               ctrlParams );
static DISPDRV_T BCM91008_ALEX_Drv =
{
   &BCM91008_ALEX_Init,                 // init
   &BCM91008_ALEX_Exit,                 // exit
   &BCM91008_ALEX_GetDispDrvFeatures,   // info
   &BCM91008_ALEX_Open,                 // open
   &BCM91008_ALEX_Close,                // close
   NULL,                                // core_freq_change
   NULL,                                // run_domain_change
   &BCM91008_ALEX_GetDispDrvData,       // get_info
   &BCM91008_ALEX_Start,                // start
   &BCM91008_ALEX_Stop,                 // stop
   &BCM91008_ALEX_PowerControl,         // power_control
   NULL,                                // update_no_os
   &BCM91008_ALEX_Update_ExtFb,         // update_dma_os
   &BCM91008_ALEX_Update,               // update
   &BCM91008_ALEX_SetCtl,               // set_control
   &BCM91008_ALEX_GetCtl,               // get_control
};


// DISP DRV API - Display Info
static DISPDRV_INFO_T BCM91008_ALEX_Info =
{
    DISPLAY_TYPE_LCD_STD,         // DISPLAY_TYPE_T          type;          
    360,                          // UInt32                  width;         
    640,                          // UInt32                  height;        
    DISPDRV_FB_FORMAT_RGB888_U,   // DISPDRV_FB_FORMAT_T     input_format;
    DISPLAY_BUS_DSI,              // DISPLAY_BUS_T           bus_type;
    0,                            // UInt32                  interlaced;    
    DISPDRV_DITHER_NONE,          // DISPDRV_DITHER_T        output_dither; 
    0,                            // UInt32                  pixel_freq;    
    0,                            // UInt32                  line_rate;     
};


#if ( defined (_ATHENA_)&& (CHIP_REVISION >= 20) ) 
CSL_DSI_TE_IN_CFG_t teInCfg = 
{
    CSL_DSI_TE_MODE_VSYNC,      // te mode 
    CSL_DSI_TE_ACT_POL_LO,      // sync polarity
    0,                          // vsync_width [tectl_clk_count]
    0,                          // hsync_line
};    
#endif


// DSI Command Mode VC Configuration
CSL_DSI_CM_VC_t alexVcCmCfg = 
{
    BCM91008_ALEX_VC,               // VC
    DSI_DT_LG_DCS_WR,               // dsiCmnd       
    MIPI_DCS_WRITE_MEMORY_START,    // dcsCmndStart       
    MIPI_DCS_WRITE_MEMORY_CONTINUE, // dcsCmndContinue       
    FALSE,                          // isLP          
    LCD_IF_CM_I_RGB888U,            // cm_in         
    LCD_IF_CM_O_RGB888,             // cm_out        
    // TE configuration
    {
#if defined(_HERA_)  || defined(_RHEA_)  
        DSI_TE_CTRLR_INPUT_0,         // DSI Te Input Type
#else
        DSI_TE_CTRLR_INPUT_0,         // DSI Te Input Type
        #if ( defined (_ATHENA_)&& (CHIP_REVISION >= 20) ) 
        &teInCfg                    // DSI Te Input Config
        #endif
#endif        
    },
};

// DSI BUS CONFIGURATION
CSL_DSI_CFG_t dsiCfg = {
    0,             // bus             set on open
    1,             // dlCount
    DSI_DPHY_0_92, // DSI_DPHY_SPEC_T
    // ESC CLK Config
#if (defined(_HERA_) || defined(_RHEA_))
    {500,5},       // escClk          500|312 500[MHz], DIV by 5 = 100[MHz]
#else
    {156,2},       // escClk          fixed   156[MHz], DIV by 2 =  78[MHz]
#endif    

    // HS CLK Config
#if (defined(_HERA_) || defined(_RHEA_))  
    // HERA   VCO range 600-2400
    {1000,2},      // hsBitClk        PLL    1000[MHz], DIV by 2 = 500[Mbps]     
#else
    // ATHENA VCO range ???
//  {300,1},       // hsBitClk        PLL     300[MHz], DIV by 1 = 300[Mbps]
    {400,1},       // hsBitClk        PLL     300[MHz], DIV by 1 = 300[Mbps]
#endif
    // LP Speed
    5,             // lpBitRate_Mbps, Max 10[Mbps]
    
#if (defined(_HERA_) || defined(_RHEA_))  
    FALSE,         // enaContClock            
#else
    FALSE,         // enaContClock            
#endif    
    TRUE,          // enaRxCrc                
    TRUE,          // enaRxEcc               
    TRUE,          // enaHsTxEotPkt           
    FALSE,         // enaLpTxEotPkt        
    FALSE,         // enaLpRxEotPkt        
};    

#define printk(format, arg...)	do {} while (0)

static  BCM91008_ALEX_PANEL_T   panel[2];


//#############################################################################




//*****************************************************************************
//
// Function Name: bcm91008_AlexTeOn
// 
// Description:   Configure TE Input Pin & Route it to DSI Controller Input
//
//*****************************************************************************
static int bcm91008_AlexTeOn ( BCM91008_ALEX_PANEL_T *pPanel )
{
    Int32       res = 0;
    
#if (defined (_HERA_) || defined(_RHEA_))
    TECTL_CFG_t teCfg;
    
    teCfg.te_mode     = TE_VC4L_MODE_VSYNC;       
    teCfg.sync_pol    = TE_VC4L_ACT_POL_LO;
    teCfg.vsync_width = 0;
    teCfg.hsync_line  = 0;

    res = CSL_TECTL_VC4L_OpenInput( pPanel->teIn, pPanel->teOut, &teCfg );
                
#else    
    DISPDRV_ATHENA_SelectTE ( DISPDRV_ATHENA_TE_ON_GPIO25 );
#endif
    return ( res );

}

//*****************************************************************************
//
// Function Name: bcm91008_AlexTeOff
// 
// Description:   'Release' TE Input Pin Used
//
//*****************************************************************************
static int bcm91008_AlexTeOff ( BCM91008_ALEX_PANEL_T *pPanel )
{
    Int32  res = 0;
#if (defined (_HERA_) || defined(_RHEA_))
    res = CSL_TECTL_VC4L_CloseInput ( pPanel->teIn );
#endif    
    return ( res );
}


//*****************************************************************************
//
// Function Name:  bcm91008_alex_WrCmndP1
// 
// Parameters:     reg   = 08-bit register address (DCS command)
//                 value = 08-bit register data    (DCS command parm)
//
// Description:    Register Write - DCS command byte, 1 parm
//
//*****************************************************************************
static void bcm91008_alex_WrCmndP1( 
    DISPDRV_HANDLE_T    drvH, 
    UInt32              reg, 
    UInt32              value 
    )
{
    BCM91008_ALEX_PANEL_T *pPanel = (BCM91008_ALEX_PANEL_T *)drvH;
    CSL_DSI_CMND_t      msg;
    UInt8               msgData[4];
    
    msg.dsiCmnd    = DSI_DT_SH_DCS_WR_P1;
    msg.msg        = &msgData[0];
    msg.msgLen     = 2;
    msg.vc         = BCM91008_ALEX_VC;
    msg.isLP       = BCM91008_ALEX_CMND_IS_LP;
    msg.isLong     = FALSE;
    msg.endWithBta = FALSE;

    msgData[0] = reg;                                  
    msgData[1] = value & 0x000000FF;   
    
    CSL_DSI_SendPacket (pPanel->clientH, &msg);   
}

//*****************************************************************************
//
// Function Name:  bcm91008_alex_WrCmndP0
// 
// Parameters:     reg   = 08-bit register address (DCS command)
//
// Description:    Register Write - DCS command byte, 0 parm 
//
//*****************************************************************************
static void bcm91008_alex_WrCmndP0( 
    DISPDRV_HANDLE_T    drvH, 
    UInt32              reg 
    )
{
    BCM91008_ALEX_PANEL_T *pPanel = (BCM91008_ALEX_PANEL_T *)drvH;
    CSL_DSI_CMND_t      msg;
    UInt8               msgData[4];
    
    msg.dsiCmnd    = DSI_DT_SH_DCS_WR_P0;
    msg.msg        = &msgData[0];
    msg.msgLen     = 2;
    msg.vc         = BCM91008_ALEX_VC;
    msg.isLP       = BCM91008_ALEX_CMND_IS_LP;
    msg.isLong     = FALSE;
    msg.endWithBta = FALSE;

    msgData[0] = reg;                                  
    msgData[1] = 0;   
    
    CSL_DSI_SendPacket (pPanel->clientH, &msg);   
}

//*****************************************************************************
//
// Function Name:  bcm91008_alex_IoCtlWr
// 
// Parameters:     
//
// Description:    IOCTL WR Test Code - DCS Wr With P0(no parm) or P1(1 parm)
//
//*****************************************************************************
static void bcm91008_alex_IoCtlWr( 
    DISPDRV_HANDLE_T       drvH,
    DISPDRV_CTRL_RW_REG*   acc 
    )
{
    if( acc->parmCount == 1 )
    { 
        bcm91008_alex_WrCmndP0 ( drvH, acc->cmnd );
        LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: DSC+P0 "
            "DCS[0x%08X]\n\r", __FUNCTION__, (unsigned int)acc->cmnd );
    }
    else if( acc->parmCount == 2 )
    {
        bcm91008_alex_WrCmndP1 ( drvH, acc->cmnd, *((UInt8*)acc->pBuff) );
        LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: DSC+P1 "
            "DCS[0x%08X] P[0x%08X]\n\r", __FUNCTION__, 
            (unsigned int)acc->cmnd, (unsigned int)*((UInt8*)acc->pBuff) );
    }
    else
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] bcm91008_alex_IoCtlWr: "
            "Only DCS with 0|1 Parm Supported\n" );
    }        
} // bcm91008_alex_IoCtlWr   

     
//*****************************************************************************
//
// Function Name:  bcm91008_alex_IoCtlWr
// 
// Parameters:     
//
// Description:    IOCTL RD Test Code - DCS Rd
//
//*****************************************************************************
static int bcm91008_alex_IoCtlRd( 
    DISPDRV_HANDLE_T       drvH,
    DISPDRV_CTRL_RW_REG*   acc 
    )
{
    BCM91008_ALEX_PANEL_T  *pPanel = (BCM91008_ALEX_PANEL_T *)drvH;
    CSL_DSI_CMND_t      msg;         
    CSL_DSI_REPLY_t     rxMsg;
    UInt8               txData[1];  // DCS Rd Command
    UInt32              reg;
    UInt8 *             pRxBuff = (UInt8*)acc->pBuff;
    Int32               res = 0;
    CSL_LCD_RES_T       cslRes;
    
    memset( (void*)&rxMsg, 0, sizeof(CSL_DSI_REPLY_t) );
    
    rxMsg.pReadReply = pRxBuff;
    
    msg.dsiCmnd    = DSI_DT_SH_DCS_RD_P0;
    msg.msg        = &txData[0];
    msg.msgLen     = 1;
    msg.vc         = BCM91008_ALEX_VC;
    msg.isLP       = BCM91008_ALEX_CMND_IS_LP;
    msg.isLong     = FALSE;
    msg.endWithBta = TRUE;
    msg.reply      = &rxMsg;

    txData[0] = acc->cmnd;                                    
    cslRes = CSL_DSI_SendPacket ( pPanel->clientH, &msg );
    
    if( cslRes != CSL_LCD_OK )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERR"
            "Reading From Reg[0x%08X]\n\r", __FUNCTION__, (unsigned int)acc->cmnd );
        res = -1;    
    }
    else
    {
        reg = pRxBuff[0];
    
        LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: Reg[0x%08X] "
            "Value[0x%08X]\n\r", __FUNCTION__, (unsigned int)acc->cmnd, (unsigned int)reg );
#if 0
        LCD_DBG ( LCD_DBG_INIT_ID, "   TYPE    : %s\n"    , 
            DISPDRV_dsiCslRxT2text (rxMsg.type,dsiE) );     
#endif

        if( rxMsg.type & DSI_RX_TYPE_TRIG )
        {
            LCD_DBG ( LCD_DBG_INIT_ID, "   TRIG    : 0x%08X\n", (unsigned int)rxMsg.trigger );
        }
        
        if( rxMsg.type & DSI_RX_TYPE_READ_REPLY )
        {
#if 0
            LCD_DBG ( LCD_DBG_INIT_ID, "   RD DT   : %s\n"    , 
                DISPDRV_dsiRxDt2text(rxMsg.readReplyDt,dsiE) );     
#endif
            LCD_DBG ( LCD_DBG_INIT_ID, "   RD STAT : 0x%08X\n", (unsigned int)rxMsg.readReplyRxStat );
            LCD_DBG ( LCD_DBG_INIT_ID, "   RD SIZE : %d\n"    , rxMsg.readReplySize );
            LCD_DBG ( LCD_DBG_INIT_ID, "   RD BUFF : 0x%02X 0x%02X 0x%02X 0x%02X "
                                   "0x%02X 0x%02X 0x%02X 0x%02X\n", 
                pRxBuff[0], pRxBuff[1],  pRxBuff[2], pRxBuff[3],
                pRxBuff[4], pRxBuff[5],  pRxBuff[6], pRxBuff[7] );
        }       
                                            
        if( rxMsg.type & DSI_RX_TYPE_ERR_REPLY )
        {
#if 0
            LCD_DBG ( LCD_DBG_INIT_ID, "   ERR DT  : %s\n"    , 
                DISPDRV_dsiRxDt2text (rxMsg.errReportDt,dsiE) );     
#endif
            LCD_DBG ( LCD_DBG_INIT_ID, "   ERR STAT: 0x%08X\n", (unsigned int)rxMsg.errReportRxStat ); 
#if 0
            LCD_DBG ( LCD_DBG_INIT_ID, "   ERR     : %s\n"    , 
                DISPDRV_dsiErr2text (rxMsg.errReport, dsiE) );       
#endif
        }        
    }
    return ( res );
} // bcm91008_alex_IoCtlRd


//*****************************************************************************
//
// Function Name: LCD_DRV_BCM91008_ALEX_GetDrvInfo
// 
// Description:   Get Driver Funtion Table
//
//*****************************************************************************
DISPDRV_T* DISP_DRV_BCM91008_ALEX_GetFuncTable ( void )
{
    return ( &BCM91008_ALEX_Drv );
}


//*****************************************************************************
//
// Function Name: BCM91008_ALEX_Init
// 
// Description:   Reset Driver Info
//
//*****************************************************************************
Int32 BCM91008_ALEX_Init ( void )
{
    Int32 res = 0;

    if(     panel[0].drvState != DRV_STATE_INIT 
         && panel[0].drvState != DRV_STATE_OPEN  
         && panel[1].drvState != DRV_STATE_INIT  
         && panel[1].drvState != DRV_STATE_OPEN  )
    {     
        LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: OK\n\r", __FUNCTION__ );
        panel[0].drvState = DRV_STATE_INIT;
        panel[1].drvState = DRV_STATE_INIT;
    } 
    else
    {
        LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: OK, Already Init\n\r",
            __FUNCTION__ );
    }   
    return ( res );
}

//*****************************************************************************
//
// Function Name: BCM91008_ALEX_Exit
// 
// Description:   
//
//*****************************************************************************
Int32 BCM91008_ALEX_Exit ( void )
{
    LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: Not Implemented\n\r",
        __FUNCTION__ );
    return ( -1 );
}

#if 0
//*****************************************************************************
//
// Function Name: DISPDRV_Reset
// 
// Description:   Generic Reset To All DISPLAY Modules
//                
//*****************************************************************************
static Int32 DISPDRV_Reset( Boolean force )
{
    UInt32          rst0pin     = 0;
    UInt32          rst1pin     = 0;
    UInt32          rst2pin     = 0;
    Boolean         rst0present = FALSE;
    Boolean         rst1present = FALSE;
    Boolean         rst2present = FALSE;
    static Boolean  resetDone   = FALSE;
   
    #ifdef HAL_LCD_RESET
    rst0present = TRUE;
    rst0pin     = HAL_LCD_RESET;
    #endif
    
    #ifdef HAL_LCD_RESET_B
    rst1present = TRUE;
    rst1pin     = HAL_LCD_RESET_B;
    #endif

    #ifdef HAL_LCD_RESET_C
    rst2present = TRUE;
    rst2pin     = HAL_LCD_RESET_C;
    #endif

//    #define RST_DURATION_MS  10
//    #define RST_HOLD_MS      1

    #define RST_DURATION_MS  1
    #define RST_HOLD_MS      20

    // coverity[dead_error_condition] - false alarm
    if( !rst0present && !rst1present && !rst2present)
    {
        LCD_DBG ( LCD_DBG_ID, "[DISPDRV] DISPDRV_Reset: "
            "Reset Pin(s) Not Defined\n");
        return ( -1 );
    }
    
    if( !rst0present )
    {
	printk(KERN_ERR "the main reset is not used");
        LCD_DBG ( LCD_DBG_ID, "[DISPDRV] DISPDRV_Reset: "
            "WARNING Only HAL_LCD_RESET B/C defined\n");
    }

    if( !rst1present )
    {
	printk(KERN_ERR "the reset B is not used");
        LCD_DBG ( LCD_DBG_ID, "[DISPDRV] DISPDRV_Reset: "
            "WARNING Only HAL_LCD_RESET B/C defined\n");
    }

    if( !rst2present )
    {
	printk(KERN_ERR "the reset C is not used");
        LCD_DBG ( LCD_DBG_ID, "[DISPDRV] DISPDRV_Reset: "
            "WARNING Only HAL_LCD_RESET B/C defined\n");
    }


    if ( !resetDone || force )
    {
        // CONFIG RESET PIN GPIO As Output
#ifndef __KERNEL__
	    if (rst0present) GPIODRV_Set_Mode (rst0pin, 1);
	    if (rst1present) GPIODRV_Set_Mode (rst1pin, 1);
	    if (rst2present) GPIODRV_Set_Mode (rst2pin, 1);
#else
	    if (rst0present) {
	        gpio_request(rst0pin, "LCD_RST0");
	        gpio_direction_output(rst0pin, 0);
	    }
	    if (rst1present) {
	        gpio_request(rst1pin, "LCD_RST1");
	        gpio_direction_output(rst1pin, 0);
	    }
	    if (rst2present) {
	        gpio_request(rst2pin, "LCD_RST2");
	        gpio_direction_output(rst2pin, 0);
	    }
#endif

        // LCD reset HIGH
        if (rst0present) GPIODRV_Set_Bit (rst0pin, 1);
        if (rst1present) GPIODRV_Set_Bit (rst1pin, 1);
        if (rst2present) GPIODRV_Set_Bit (rst2pin, 1);
        OSTASK_Sleep ( TICKS_IN_MILLISECONDS(1) );

        // LCD reset Low
        if (rst0present) GPIODRV_Set_Bit (rst0pin, 0);
        if (rst1present) GPIODRV_Set_Bit (rst1pin, 0);
        if (rst2present) GPIODRV_Set_Bit (rst2pin, 0);
        OSTASK_Sleep ( TICKS_IN_MILLISECONDS(RST_DURATION_MS) );
    
        // LCD reset High
        if (rst0present) GPIODRV_Set_Bit (rst0pin, 1);
        if (rst1present) GPIODRV_Set_Bit (rst1pin, 1);
        if (rst2present) GPIODRV_Set_Bit (rst2pin, 1);

        OSTASK_Sleep ( TICKS_IN_MILLISECONDS(RST_HOLD_MS) );
        resetDone = TRUE;
    } 

    return ( 0 );
} // DISPDRV_Reset
#endif



//*****************************************************************************
//
// Function Name: BCM91008_ALEX_Open
// 
// Description:   Open Sub Drivers
//
//*****************************************************************************
Int32 BCM91008_ALEX_Open ( 
    const void*         params,
    DISPDRV_HANDLE_T*   drvH 
    )
{
    Int32                         res = 0;
    UInt32                        busId; 
    const DISPDRV_OPEN_PARM_T*    pOpenParm;
    BCM91008_ALEX_PANEL_T         *pPanel;

    //busCh - NA to DSI interface
    pOpenParm = (DISPDRV_OPEN_PARM_T*) params;
    busId     = pOpenParm->busCh;

#if (defined (_HERA_) || defined(_RHEA_))
    #define BUS_ID_MAX  1
#else
    #define BUS_ID_MAX  0
#endif

    if( busId > BUS_ID_MAX )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERROR Invalid DSI Bus[%d]\n\r",
            __FUNCTION__, (unsigned int)busId );
        return ( -1 );
    }

    pPanel = &panel[busId];

    if( pPanel->drvState == DRV_STATE_OPEN )
    {
        *drvH = (DISPDRV_HANDLE_T) pPanel;
        LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: Returning Handle, "
            "Already Open\n\r", __FUNCTION__ );
        return ( res );
    }

    if ( pPanel->drvState != DRV_STATE_INIT )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERROR Not Init\n\r",
            __FUNCTION__ );
        return ( -1 );
    }    
    
    //DISPDRV_Reset( FALSE );
    
    dsiCfg.bus = busId;
    
    
#ifdef __KERNEL__
    pPanel->pFb = pPanel->pFbA = (void*)pOpenParm->busId;
#else
    pPanel->pFb = pPanel->pFbA = (void*)&FrameBuff[busId];
#endif
   
#if (defined (_HERA_) || defined(_RHEA_))
    if( busId == 0 )
    {
        #if ( defined( DALTON_EB_0_1 ) )
        pPanel->teIn   = TE_VC4L_IN_1_DSI0;
        #else        
        pPanel->teIn   = TE_VC4L_IN_0_LCD;
        #endif
        pPanel->teOut  = TE_VC4L_OUT_DSI0_TE0;
    }
    else
    {
        pPanel->teIn   = TE_VC4L_IN_2_DSI1;
        pPanel->teOut  = TE_VC4L_OUT_DSI1_TE0;
    }
#endif

    if (brcm_enable_dsi_lcd_clocks(&pPanel->dfs_node))
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERROR to enable the clock\n",
            __FUNCTION__  );
        return ( -1 );
    }

    if( bcm91008_AlexTeOn ( pPanel ) ==  -1 )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: "
            "Failed To Configure TE Input\n", __FUNCTION__ ); 
        return ( -1 );
    }
    
    if ( CSL_DSI_Init( &dsiCfg ) != CSL_LCD_OK )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERROR, DSI CSL Init "
            "Failed\n\r", __FUNCTION__ );
        return ( -1 );
    }
    
    if ( CSL_DSI_OpenClient ( busId, &pPanel->clientH ) != CSL_LCD_OK )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERROR, CSL_DSI_OpenClient "
            "Failed\n\r", __FUNCTION__);
        return ( -1 );
    }
    
    if ( CSL_DSI_OpenCmVc ( pPanel->clientH, &alexVcCmCfg, &pPanel->dsiCmVcHandle ) 
            != CSL_LCD_OK )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: CSL_DSI_OpenCmVc Failed\n\r",
            __FUNCTION__);
        return ( -1 );
    }

	#ifdef UNDER_LINUX
    if (csl_dma_vc4lite_init() != DMA_VC4LITE_STATUS_SUCCESS)
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: csl_dma_vc4lite_init Failed\n\r",
            __FUNCTION__);
        return ( -1 );
    }
	#endif
	
    pPanel->busId      = busId; 
    
    pPanel->win.left   = 0;  
    pPanel->win.right  = 359; 
    pPanel->win.top    = 0;  
    pPanel->win.bottom = 639;
    pPanel->win.width  = 360; 
    pPanel->win.height = 640;
    
    pPanel->drvState   = DRV_STATE_OPEN;
    
    *drvH = (DISPDRV_HANDLE_T) pPanel;
    LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: OK\n\r", __FUNCTION__ );
    
    return ( res );
}

//*****************************************************************************
//
// Function Name: BCM91008_ALEX_Close
// 
// Description:   Close The Driver
//
//*****************************************************************************
Int32 BCM91008_ALEX_Close ( DISPDRV_HANDLE_T drvH ) 
{
    Int32                   res = 0;
    BCM91008_ALEX_PANEL_T   *pPanel = (BCM91008_ALEX_PANEL_T *)drvH;
    
    DISPDRV_CHECK_PTR_2_RET( drvH, &panel[0], &panel[1], __FUNCTION__ );
    
    pPanel->pFb  = NULL;
    pPanel->pFbA = NULL;
    
    if ( CSL_DSI_CloseCmVc ( pPanel->dsiCmVcHandle ) ) 
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERROR, "
            "Closing Command Mode Handle\n\r", __FUNCTION__);
        return ( -1 );
    }
    
    if ( CSL_DSI_CloseClient ( pPanel->clientH ) != CSL_LCD_OK )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERROR, Closing DSI Client\n\r",
            __FUNCTION__);
        return ( -1 );
    }
    
    if ( CSL_DSI_Close( pPanel->busId ) != CSL_LCD_OK )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERR Closing DSI Controller\n\r",
            __FUNCTION__ );
        return ( -1 );
    }
    
#if (defined (_HERA_) || defined(_RHEA_))
    bcm91008_AlexTeOff ( pPanel );
#endif
   
    if (brcm_disable_dsi_lcd_clocks(pPanel->dfs_node))
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERROR to enable the clock\n",
            __FUNCTION__  );
        return ( -1 );
    }

    pPanel->pwrState = DISP_PWR_OFF;
    pPanel->drvState = DRV_STATE_INIT;
    LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: OK\n\r", __FUNCTION__ );
    
    return ( res );
}


//*****************************************************************************
//
// Function Name: BCM91008_ALEX_PowerControl
// 
// Description:   Display Module Control
//
//*****************************************************************************
Int32 BCM91008_ALEX_PowerControl ( 
    DISPDRV_HANDLE_T        drvH, 
    DISPLAY_POWER_STATE_T   state )
{
    Int32  res = 0;
    BCM91008_ALEX_PANEL_T   *pPanel = (BCM91008_ALEX_PANEL_T *)drvH;
    
    DISPDRV_CHECK_PTR_2_RET( drvH, &panel[0], &panel[1], __FUNCTION__ );
   
    switch ( state )
    {
        case DISPLAY_POWER_STATE_ON:
            switch ( pPanel->pwrState )
            {
                case DISP_PWR_OFF:
                    bcm91008_alex_WrCmndP0 ( drvH, MIPI_DCS_EXIT_SLEEP_MODE );
                    
                    OSTASK_Sleep ( TICKS_IN_MILLISECONDS(120) );

                    bcm91008_alex_WrCmndP0 ( drvH, MIPI_DCS_SET_DISPLAY_ON );
                    
                    // Brightness Ctrl On
                    bcm91008_alex_WrCmndP1 (drvH, 0x53, 0x20);  
                    // Brightness to 0x80
                    bcm91008_alex_WrCmndP1 (drvH, 0x51, 0x80);  
                    
                    // MADCTL - Invert, disp is mounted so RB is at UL position
                    bcm91008_alex_WrCmndP1 ( drvH, MIPI_DCS_SET_ADDRESS_MODE, 0xd4 );  
                    
                    // TE, VSYNC only mode    
                    bcm91008_alex_WrCmndP1 ( drvH, MIPI_DCS_SET_TEAR_ON, 0 );  
                    
                    pPanel->pwrState = DISP_PWR_SLEEP_OFF;
                    LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: INIT-SEQ\n\r",
                        __FUNCTION__ );
                    break; 
                case DISP_PWR_SLEEP_ON:
                    bcm91008_alex_WrCmndP0 ( drvH, MIPI_DCS_EXIT_SLEEP_MODE );
                    OSTASK_Sleep ( 120 );
                    pPanel->pwrState = DISP_PWR_SLEEP_OFF;
                    LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: SLEEP-OUT\n\r",
                        __FUNCTION__ );
                    break;
                    
                default:
                    break;    
            }        
            break;
        case DISPLAY_POWER_STATE_OFF:
            LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: POWER-OFF State "
                "Not Supported\n\r", __FUNCTION__ );
            res = -1;
            break;
            
        case DISPLAY_POWER_STATE_SLEEP:
            if( pPanel->pwrState == DISP_PWR_SLEEP_OFF )
            {
                bcm91008_alex_WrCmndP0 ( drvH, MIPI_DCS_ENTER_SLEEP_MODE );
                OSTASK_Sleep ( 120 );
                pPanel->pwrState = DISP_PWR_SLEEP_ON;
                LCD_DBG ( LCD_DBG_INIT_ID, "[DISPDRV] %s: SLEEP-IN\n\r",
                    __FUNCTION__ );
            } 
            else
            {
                LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: SLEEP-IN Requested, "
                    "But Not In POWER-ON State\n\r", __FUNCTION__ );
                res = -1;
            }   
            break;
        
        default:
            LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: Invalid Power State[%d] "
                "Requested\n\r", __FUNCTION__, state );
            res = -1;
            break;
    }
    return ( res );
}

//*****************************************************************************
//
// Function Name: BCM91008_ALEX_Start
// 
// Description:   Configure For Updates
//
//*****************************************************************************
Int32 BCM91008_ALEX_Start ( DISPDRV_HANDLE_T drvH )
{
    Int32 res = 0;

    DISPDRV_CHECK_PTR_2_RET( drvH, &panel[0], &panel[1], __FUNCTION__ );
    
    return ( res );
}

//*****************************************************************************
//
// Function Name: BCM91008_ALEX_Stop
// 
// Description:   
//
//*****************************************************************************
Int32 BCM91008_ALEX_Stop ( DISPDRV_HANDLE_T drvH )
{
    Int32 res = 0;
    
    DISPDRV_CHECK_PTR_2_RET( drvH, &panel[0], &panel[1], __FUNCTION__ );
    
    return ( res );
}

//*****************************************************************************
//
// Function Name: BCM91008_ALEX_GetInfo
// 
// Description:   
//
//*****************************************************************************
const DISPDRV_INFO_T* BCM91008_ALEX_GetDispDrvData ( DISPDRV_HANDLE_T drvH )
{
    DISPDRV_CHECK_PTR_2_NO_RET( drvH, &panel[0], &panel[1], __FUNCTION__ );
   
    return ( &BCM91008_ALEX_Info );
}

//*****************************************************************************
//
// Function Name: BCM91008_ALEX_GetDispDrvFeatures
// 
// Description:   
//
//*****************************************************************************
Int32 BCM91008_ALEX_GetDispDrvFeatures ( 
    const char**                driver_name,
    UInt32*                     version_major,
    UInt32*                     version_minor,
    DISPDRV_SUPPORT_FEATURES_T* flags 
    )
{
   Int32 res = -1; 
   
   if (   ( NULL != driver_name   ) && ( NULL != version_major ) 
       && ( NULL != version_minor ) && ( NULL != flags         ) )
   {
      *driver_name   = "BCM91008_ALEX (IN:RGB888U OUT:RGB888)";
      *version_major = 0;
      *version_minor = 15;
      *flags         = DISPDRV_SUPPORT_NONE;
      res            = 0;
   }
   return ( res );
}


//*****************************************************************************
//
// Function Name: bcm91008_AlexCb
// 
// Description:   CSL callback        
//
//*****************************************************************************
static void bcm91008_AlexCb ( CSL_LCD_RES_T cslRes, pCSL_LCD_CB_REC pCbRec ) 
{
    DISPDRV_CB_RES_T apiRes;

    LCD_DBG ( LCD_DBG_ID, "[DISPDRV] +%s\r\n", __FUNCTION__ );

    if( pCbRec->dispDrvApiCb != NULL )
    {
        switch ( cslRes )
        {
            case CSL_LCD_OK:
              apiRes = DISPDRV_CB_RES_OK;
              break;
            default:
              apiRes = DISPDRV_CB_RES_ERR;         
              break;
        }
        
        if ( pCbRec->dispDrvApiCbRev == DISP_DRV_CB_API_REV_1_0 ) 
        {
            ((DISPDRV_CB_T)pCbRec->dispDrvApiCb)( apiRes );
        }
        else    
        {
            ((DISPDRV_CB_API_1_1_T)pCbRec->dispDrvApiCb)
                ( apiRes, pCbRec->dispDrvApiCbP1 );
        }    
    }
    
    LCD_DBG ( LCD_DBG_ID, "[DISPDRV] -%s\r\n", __FUNCTION__ );
}


//*****************************************************************************
//
// Function Name: BCM91008_ALEX_Update_ExtFb
// 
// Description:   DMA/OS Update using EXT frame buffer
//
//*****************************************************************************
Int32 BCM91008_ALEX_Update_ExtFb ( 
    DISPDRV_HANDLE_T        drvH, 
    void                    *pFb,
    DISPDRV_CB_API_1_1_T    apiCb
    )
{
    BCM91008_ALEX_PANEL_T *pPanel = (BCM91008_ALEX_PANEL_T *)drvH;
    CSL_LCD_UPD_REQ_T   req;
    Int32               res  = 0;

    LCD_DBG ( LCD_DBG_ID, "[DISPDRV] +%s\r\n", __FUNCTION__ );

    DISPDRV_CHECK_PTR_2_RET( drvH, &panel[0], &panel[1], __FUNCTION__ );
    
    if ( pPanel->pwrState != DISP_PWR_SLEEP_OFF )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] +%s: Skip Due To Power State\r\n", 
            __FUNCTION__ );
        return ( -1 );
    }
    
    req.buff           = pFb;
    req.lineLenP       = BCM91008_ALEX_Info.width;
    req.lineCount      = BCM91008_ALEX_Info.height;
    req.buffBpp        = 4;    
    req.timeOut_ms     = 100;

    req.cslLcdCbRec.cslH            = pPanel->clientH;
    req.cslLcdCbRec.dispDrvApiCbRev = DISP_DRV_CB_API_REV_1_1;
    req.cslLcdCbRec.dispDrvApiCb    = (void*) apiCb;
    req.cslLcdCbRec.dispDrvApiCbP1  = pFb;

    if( apiCb != NULL )
       req.cslLcdCb = bcm91008_AlexCb;
    else
       req.cslLcdCb = NULL;
        
    if ( CSL_DSI_UpdateCmVc ( pPanel->dsiCmVcHandle, &req ) != CSL_LCD_OK )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERROR ret by "
            "CSL_DSI_UpdateCmVc\n\r", __FUNCTION__ );
        res = -1;    
    }
        
    LCD_DBG ( LCD_DBG_ID, "[DISPDRV] -%s\n\r", __FUNCTION__ );
        
    return ( res );
}

//*****************************************************************************
//
// Function Name: BCM91008_ALEX_Update
// 
// Description:   DMA/OS Update using INT frame buffer
//
//*****************************************************************************
Int32 BCM91008_ALEX_Update ( 
    DISPDRV_HANDLE_T    drvH, 
    int			fb_idx,
    DISPDRV_CB_T        apiCb
    )
{
    BCM91008_ALEX_PANEL_T *pPanel = (BCM91008_ALEX_PANEL_T *)drvH;
    CSL_LCD_UPD_REQ_T   req;
    Int32               res  = 0;

    LCD_DBG ( LCD_DBG_ID, "[DISPDRV] +%s\r\n", __FUNCTION__ );

    DISPDRV_CHECK_PTR_2_RET( drvH, &panel[0], &panel[1], __FUNCTION__ );
    
    if ( pPanel->pwrState != DISP_PWR_SLEEP_OFF )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] +%s: Skip Due To Power State\r\n", 
            __FUNCTION__ );
        return ( -1 );
    }
   
    if (0 == fb_idx)
    	req.buff           = pPanel->pFbA;
     else
	req.buff 	   = (void *)((UInt32)pPanel->pFbA + 
		BCM91008_ALEX_Info.width * BCM91008_ALEX_Info.height * 4);

//    req.buff           = pPanel->pFbA;
    req.lineLenP       = BCM91008_ALEX_Info.width;
    req.lineCount      = BCM91008_ALEX_Info.height;
    req.buffBpp        = 4;    
    req.timeOut_ms     = 100;
   
    printk(KERN_ERR "buf=%08x, linelenp = %d, linecnt =%d\n", (u32)req.buff, req.lineLenP, req.lineCount);
    req.cslLcdCbRec.cslH            = pPanel->clientH;
    req.cslLcdCbRec.dispDrvApiCbRev = DISP_DRV_CB_API_REV_1_0;
    req.cslLcdCbRec.dispDrvApiCb    = (void*) apiCb;
    req.cslLcdCbRec.dispDrvApiCbP1  = NULL;
    
    if( apiCb != NULL )
       req.cslLcdCb = bcm91008_AlexCb;
    else
       req.cslLcdCb = NULL;
    
    if ( CSL_DSI_UpdateCmVc ( pPanel->dsiCmVcHandle, &req ) != CSL_LCD_OK )
    {
        LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: ERROR ret by "
            "CSL_DSI_UpdateCmVc\n\r", __FUNCTION__ );
        res = -1;    
    }
        
    LCD_DBG ( LCD_DBG_ID, "[DISPDRV] -%s\r\n", __FUNCTION__ );
        
    return ( res );
}



//*****************************************************************************
//
// Function Name: BCM91008_ALEX_SetCtl
// 
// Description:   
//
//*****************************************************************************
Int32 BCM91008_ALEX_SetCtl ( 
        DISPDRV_HANDLE_T    drvH, 
        DISPDRV_CTRL_ID_T   ctrlID, 
        void*               ctrlParams 
        )
{
    Int32 res = -1;
    
    DISPDRV_CHECK_PTR_2_RET( drvH, &panel[0], &panel[1], __FUNCTION__ );
    
    switch ( ctrlID )
    {
        case DISPDRV_CTRL_ID_SET_REG:
            bcm91008_alex_IoCtlWr( drvH, (DISPDRV_CTRL_RW_REG*)ctrlParams );
            res = 0;
            break;
    
        default:
            LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: "
                "CtrlId[%d] Not Implemented\n\r", __FUNCTION__, ctrlID );
            break;    
    }
    return ( res );
}
                    
//*****************************************************************************
//
// Function Name: BCM91008_ALEX_GetCtl
// 
// Description:   
//
//*****************************************************************************
Int32 BCM91008_ALEX_GetCtl (
            DISPDRV_HANDLE_T    drvH, 
            DISPDRV_CTRL_ID_T   ctrlID, 
            void*               ctrlParams 
            )
{
    BCM91008_ALEX_PANEL_T *pPanel = (BCM91008_ALEX_PANEL_T *)drvH;
    Int32 res = -1;
    
    DISPDRV_CHECK_PTR_2_RET( drvH, &panel[0], &panel[1], __FUNCTION__ );
    
    switch ( ctrlID )
    {
        case DISPDRV_CTRL_ID_GET_FB_ADDR:
            ((DISPDRV_CTL_GET_FB_ADDR *)ctrlParams)->frame_buffer = 
                pPanel->pFbA;
            res = 0;
            break;
            
        case DISPDRV_CTRL_ID_GET_REG:
            res = bcm91008_alex_IoCtlRd( drvH, (DISPDRV_CTRL_RW_REG*)ctrlParams );
            break;
            
        default:
            LCD_DBG ( LCD_DBG_ERR_ID, "[DISPDRV] %s: CtrlId[%d] Not "
                "Implemented\n\r", __FUNCTION__, ctrlID );
            break;
    }
    
    return ( res );
}            

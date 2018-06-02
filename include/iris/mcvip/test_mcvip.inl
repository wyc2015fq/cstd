/*
 * main.c
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation version 2.
 *
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "system.h"
#include "capture.h"
#include "display.h"
#include "capture_priv.h"
#include "display_priv.h"

extern CAPTURE_Ctrl gCAPTURE_ctrl;
extern DISPLAY_Ctrl    gDISPLAY_ctrl;

int UI_sysStart( int mode, int videoInterface, int videoSystem )
{
    CAPTURE_CreatePrm captureCreatePrm;
    DISPLAY_CreatePrm displayCreatePrm;

    OSA_printf(" MCVIP: Video capture starting !!!\n");
    captureCreatePrm.videoDecoderMode = mode;
    captureCreatePrm.videoSystem = videoSystem;
    captureCreatePrm.videoIfMode = videoInterface;
    CAPTURE_create( &captureCreatePrm );

    OSA_printf(" MCVIP: Video display starting !!!\n");
    displayCreatePrm.displayMode = DISPLAY_MODE_COMPOSITE;
    displayCreatePrm.captureInfo = CAPTURE_getInfo();
    //displayCreatePrm.layoutMode  = DISPLAY_LAYOUT_MODE_LIVE_2x2;
    DISPLAY_create(&displayCreatePrm);

    DISPLAY_start();
    CAPTURE_start();
    OSA_printf(" MCVIP: Startup completed successfully.\n");
    return 0;
}

int UI_sysStop()
{
    OSA_printf(" MCVIP: Display Stopping !!!\n");
    DISPLAY_stop();

    OSA_printf(" MCVIP: Capture Stopping !!!\n");
    CAPTURE_stop();

    OSA_printf(" MCVIP: Capture Deleting !!!\n");
    CAPTURE_delete();

    OSA_printf(" MCVIP: Display Deleting !!!\n");
    DISPLAY_delete();

    OSA_printf(" MCVIP: Stopped !!!\n");
    return OSA_SOK;
}

void TVP5158Init( CaptureCB identifyCB , long identifyParam , CaptureCB dispalyCB , long dispalyParam )
{
    gCAPTURE_ctrl.m_CallBack = identifyCB;
    gCAPTURE_ctrl.m_Param = identifyParam;

    gDISPLAY_ctrl.m_CallBack = dispalyCB;
    gDISPLAY_ctrl.m_Param = dispalyParam;

    SYSTEM_init( 0 , 1 );
    UI_sysStart(MCVIP_VIDEO_DECODER_MODE_2CH_D1,MCVIP_VIDEO_IF_MODE_BT656 ,MCVIP_VIDEO_SYSTEM_PAL);
}

void TVP5158UnInit()
{
    UI_sysStop();
    SYSTEM_exit();
}


#include "./osa/osa.inl"
#include "drv_audio.c"
#include "drv_dma.c"
#include "drv_i2c.c"
#include "mcvip.inl"
#include "audio.c"
#include "audio_tsk.c"
#include "display_v4l2.c"
#include "display_layout.c"
#include "capture_tsk.c"
#include "display_tsk.c"
#include "system.c"

int test_mcvip( int argc, char *argv[] )
{
    int numMCVIP;
    int debug = 0;
    numMCVIP = 1;

    SYSTEM_init(debug,numMCVIP);

    UI_sysStart(MCVIP_VIDEO_DECODER_MODE_2CH_D1,MCVIP_VIDEO_IF_MODE_BT656 ,MCVIP_VIDEO_SYSTEM_PAL);
    int start = 1;
    while( getchar() != 'q' )
    {
        if( start )
        {
            DISPLAY_stop();
            CAPTURE_stop();
            start = 0;
        }
        else
        {
            DISPLAY_start();
            CAPTURE_start();
            start = 1;
        }
    }
    TVP5158UnInit();
    return 0;
}

/*
 * audio.c
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed “as is” WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

//#include <setparam.h>
#include <osa_cmem.h>
#include <audio_priv.h>
#include <mcvip.h>
#include <osa.h>

int AUDIO_Init()
{
    int ret;
    int i;

    char tdm_ch[8] = {0,4,1,5,2,6,3,7};

    // Allocate the buffers for audio capture and playback
    gAUDIO_ctrl.recBuf  = (short *)OSA_cmemAlloc(AUDIO_BUF_SIZE,32);
    gAUDIO_ctrl.playBuf = (short *)OSA_cmemAlloc(AUDIO_SAMPLES_DEMUX * 2,32);

    for (i = 0; i < 8; i++)
    {
	   gAUDIO_ctrl.tdm_ch[i] = tdm_ch[i];
    }

    gAUDIO_ctrl.curVol = 0x8; //fix

    // audio init

    ret = MCVIP_audioInit(gAUDIO_ctrl.pMcvipHndl,gAUDIO_ctrl.pCreatePrm);

    if(ret < 0)
    {
        return OSA_EFAIL;
    }

    if(ret != OSA_SOK)
    {
        return OSA_EFAIL;
    }

	return OSA_SOK;
}

int AUDIO_Exit()
{
    int ret;

    // Free the buffers
    OSA_cmemFree((void *)gAUDIO_ctrl.recBuf);
    OSA_cmemFree((void *)gAUDIO_ctrl.playBuf);

    ret = MCVIP_audioExit(gAUDIO_ctrl.pMcvipHndl);

    if(ret < 0)
    {
        return OSA_EFAIL;
    }

	return OSA_SOK;
}

int AUDIO_Record()
{
    int ret;

    ret = MCVIP_audioRecord(gAUDIO_ctrl.pMcvipHndl,(char *)gAUDIO_ctrl.recBuf,AUDIO_BUF_SIZE);

    if(ret < 0)
    {
        return OSA_EFAIL;
    }

	return OSA_SOK;
}

int AUDIO_Play()
{
    int ret;

    ret = MCVIP_audioPlayback(gAUDIO_ctrl.pMcvipHndl,(char *)gAUDIO_ctrl.playBuf,AUDIO_BUF_SIZE_DEMUX);

    if(ret < 0)
    {
	return OSA_EFAIL;
    }

    return OSA_SOK;
}

int AUDIO_Vol(int volume)
{
    int ret;

//MAT    if(0 <= volume && volume <= 0xF)
    if(0 <= volume && volume <= 0x8)
    {
	gAUDIO_ctrl.curVol = volume;

        ret = MCVIP_audioSetVol(gAUDIO_ctrl.pMcvipHndl,gAUDIO_ctrl.audPlayCh,gAUDIO_ctrl.curVol);

        if(ret < 0)
        {
	    return OSA_EFAIL;	
        }
    }
       
    return OSA_SOK;
}

int AUDIO_incVol()
{
  	if( gAUDIO_ctrl.curVol+1 <= 8 )
		{
			return AUDIO_Vol(gAUDIO_ctrl.curVol+1);
		}
       
    return OSA_SOK;
}

int AUDIO_decVol()
{
  	if( gAUDIO_ctrl.curVol-1 >= 0 )
		{
			return AUDIO_Vol(gAUDIO_ctrl.curVol-1);
		}
       
    return OSA_SOK;
}

int AUDIO_nextPlayChan()
{
    //0=Ch1&2, 1=Ch3&4,...,7=Ch15&16
	  if( gAUDIO_ctrl.audPlayCh < 7 )
		{
        OSA_mutexLock(&gAUDIO_ctrl.mutexLock);
		    gAUDIO_ctrl.audPlayCh += 1;
        OSA_mutexUnlock(&gAUDIO_ctrl.mutexLock) ;
  	}
		else
		{
        OSA_mutexLock(&gAUDIO_ctrl.mutexLock);
		    gAUDIO_ctrl.audPlayCh = 0;
        OSA_mutexUnlock(&gAUDIO_ctrl.mutexLock) ;
  	}

    OSA_printf("Now playing audio channels L=Ch%d and R=Ch%d\n",(gAUDIO_ctrl.audPlayCh<<1)+1, (gAUDIO_ctrl.audPlayCh<<1)+2) ;
		return OSA_SOK;
}

int AUDIO_MuteOn()
{
//    int ret ;

//    ret = MCVIP_audioSetVol(gAUDIO_ctrl.pMcvipHndl,gAUDIO_ctrl.audPlayCh, 0);
//    if(ret < 0)
//    {
//        return OSA_EFAIL;
//    }
//    return OSA_SOK;
//		return MCVIP_audioSetMute(gAUDIO_ctrl.pMcvipHndl,gAUDIO_ctrl.audPlayCh, 1);
			gAUDIO_ctrl.enableMute = 1;
			return OSA_SOK;
}

int AUDIO_MuteOff()
{
//    int ret ;

//    if(0 <= gAUDIO_ctrl.curVol && gAUDIO_ctrl.curVol < 16)
//    {
//        ret = MCVIP_audioSetVol(gAUDIO_ctrl.pMcvipHndl,gAUDIO_ctrl.audPlayCh, gAUDIO_ctrl.curVol);
//
//        if(ret < 0)
//        {   
//            return OSA_EFAIL;
//        }
//    }
//   
//    return OSA_SOK;
//	return MCVIP_audioSetMute(gAUDIO_ctrl.pMcvipHndl,gAUDIO_ctrl.audPlayCh, 0);
	gAUDIO_ctrl.enableMute = 0;
	return OSA_SOK;
}




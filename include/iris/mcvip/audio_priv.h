/*
 * audio_priv.h
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


#ifndef _AUDIO_PRIV_H_
#define _AUDIO_PRIV_H_

#include <audio.h>
#include <osa_tsk.h>
#include <osa_buf.h>


#define AUDIO_BUF_SIZE_DEMUX (2048)
#define AUDIO_SAMPLES_DEMUX  (AUDIO_BUF_SIZE_DEMUX/2)

#define AUDIO_BUF_SIZE	     (AUDIO_BUF_SIZE_DEMUX * 8)           // in bytes
#define AUDIO_SAMPLES	     (AUDIO_BUF_SIZE/2)

#define AUDIO_THR_PRI        (MCVIP_CAPTURE_THR_PRI_HIGH - 6)  // (MCVIP_CAPTURE_THR_PRI_HIGH-3) // AYK - 0728

#define AUDIO_STACK_SIZE     (10 * KB)

#define AUDIO_CMD_CREATE     (0x0500) // AYK - 0728
#define AUDIO_CMD_START      (0x0501) // AYK - 0728
#define AUDIO_CMD_STOP       (0x0502) // AYK - 0728
#define AUDIO_CMD_DELETE     (0x0503) // AYK - 0728
#define AUDIO_CMD_INCVOL     (0x0504) // MAT - 1026
#define AUDIO_CMD_DECVOL     (0x0505) // MAT - 1026
#define AUDIO_CMD_NEXTPLAYCHAN (0x0506) // MAT - 0129


typedef struct
{
  OSA_TskHndl tskHndl;
  OSA_MbxHndl mbxHndl;
  OSA_MutexHndl mutexLock;
  int audPlayCh;
  Uint8 curVol;
	Uint8 enableMute;
  void  *pMcvipHndl;
  MCVIP_CreatePrm *pCreatePrm;
  short *recBuf;
  short *playBuf;
  char tdm_ch[8];
} AUDIO_Ctrl;

extern AUDIO_Ctrl gAUDIO_ctrl;

int AUDIO_tskCreate( AUDIO_CreatePrm *prm);
int AUDIO_tskDelete( );

int AUDIO_Init();
int AUDIO_Record();
int AUDIO_Play();
int AUDIO_incVol();
int AUDIO_decVol();
int AUDIO_MuteOn();
int AUDIO_MuteOff();

int AUDIO_nextPlayChan();
int AUDIO_Exit();

#endif

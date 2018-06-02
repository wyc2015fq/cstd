/*
 * audio.h
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

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <capture.h>
#include <osa_que.h>
#include <mcvip.h>
#include<linux/soundcard.h>


typedef struct
{
  void *pMcvipHndl;
  int audPlayCh;
  MCVIP_CreatePrm *pCreatePrm;

} AUDIO_CreatePrm;

int AUDIO_create(AUDIO_CreatePrm *prm);
int AUDIO_delete();
int AUDIO_start();
int AUDIO_stop();
int AUDIO_printInfo();
int AUDIO_playback(short *PlayBuf, int inPlayBufSize);
int AUDIO_increaseVolume();
int AUDIO_decreaseVolume();
int AUDIO_nextPlayChannels();

#endif

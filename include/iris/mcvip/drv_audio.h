/*
 * drv_audio.h
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

#ifndef _DRV_AUDIO_H_
#define _DRV_AUDIO_H_

#include<linux/soundcard.h>
//#include <drv.h>
#include <osa_buf.h>

// AYK - 0826
#define DRV_AUDIO_RECORD_DEVICE_NAME   "/dev/dsp"

//MAT 0119 
//dsp1 does not exist
//#define DRV_AUDIO_PLAY_DEVICE_NAME     "/dev/dsp1"
#define DRV_AUDIO_PLAY_DEVICE_NAME     "/dev/dsp"

#define DRV_AUDIO_BUF_MAX   (32)

#define DRV_AUDIO_DEVICE_VOICE_CODEC  0
#define DRV_AUDIO_DEVICE_AICxx        1

#define DRV_AUDIO_MODE_TX   0
#define DRV_AUDIO_MODE_RX   1

#define DRV_AUDIO_VOLUME_LEVEL_MAX    10

typedef struct {

  // AYK - 0826
  int fdRec;
  int fdPlay;

} DRV_AudioHndl;

typedef struct {

  Uint16 deviceId;
  Uint16 samplingRate;
  Uint16 numChannels;
  Uint16 mode;
  Uint16 numBuf;
  Uint16 sizeBuf;

  // AYK - 0826
  Uint16 format;

} DRV_AudioConfig;


int DRV_audioOpen(DRV_AudioHndl *hndl, DRV_AudioConfig *config);
int DRV_audioClose(DRV_AudioHndl *hndl);

OSA_BufInfo *DRV_audioGetBufInfo(DRV_AudioHndl *hndl, int bufId);

int DRV_audioGetBuf(DRV_AudioHndl *hndl, int *bufId);
int DRV_audioPutBuf(DRV_AudioHndl *hndl, int bufId);

int DRV_audioStart(DRV_AudioHndl *hndl);
int DRV_audioStop(DRV_AudioHndl *hndl);
int DRV_audioPause(DRV_AudioHndl *hndl);
int DRV_audioResume(DRV_AudioHndl *hndl);
int DRV_audioFlush(DRV_AudioHndl *hndl);
int DRV_audioSetVolume(DRV_AudioHndl *hndl, Uint32 volumeLevel);

// AYK - 0826
int DRV_audioOpenRec(DRV_AudioHndl *hndl,DRV_AudioConfig *config);
int DRV_audioOpenPlay(DRV_AudioHndl *hndl,DRV_AudioConfig *config);
int DRV_audioRecord(DRV_AudioHndl *hndl,Int8 *recBuf,Uint32 recBufSize);
int DRV_audioPlay(DRV_AudioHndl *hndl,Int8 *playBuf,Uint32 playBufSize);
int DRV_audioCloseRec(DRV_AudioHndl *hndl);
int DRV_audioClosePlay(DRV_AudioHndl *hndl);

#endif


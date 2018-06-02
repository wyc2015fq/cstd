/*
 * capture.h
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


#ifndef _CAPTURE_H_
#define _CAPTURE_H_

#include <mcvip.h>
#include <osa_que.h>

#define INFRARED 0
#define COLOR       1

typedef int (*CaptureCB)( char* pData , int w , int h , int step , int param , int chID );

typedef struct {

  int videoDecoderMode;   
  int videoIfMode;        
  int videoSystem;     

} CAPTURE_CreatePrm;

typedef struct {

  void *mcvipHndl;
  
  MCVIP_ChList chList;
  MCVIP_CreatePrm createPrm;
  
  OSA_QueHndl displayQue[MCVIP_CHANNELS_MAX];

} CAPTURE_Info;

int CAPTURE_create(CAPTURE_CreatePrm *prm);
int CAPTURE_delete();
int CAPTURE_start();
int CAPTURE_stop();

int CAPTURE_saveFrame();
int CAPTURE_printInfo();

CAPTURE_Info *CAPTURE_getInfo();

#endif 

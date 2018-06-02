/*
 * display.h
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

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <capture.h>

#define DISPLAY_MODE_COMPOSITE    0

#define DISPLAY_COLOR_BACKGROUND_Y    0x1b1b1b1b
#define DISPLAY_COLOR_BACKGROUND_C    0x67f067f0

#define DISPLAY_COLOR_NO_VIDEO_Y    0x80808080
#define DISPLAY_COLOR_NO_VIDEO_C    0x80808080


typedef struct {

  int displayMode;   
  CAPTURE_Info *captureInfo;
  
} DISPLAY_CreatePrm;

int DISPLAY_create(DISPLAY_CreatePrm *prm);
int DISPLAY_delete();
int DISPLAY_start();
int DISPLAY_stop();
int DISPLAY_printInfo();
int DISPLAY_nextPage();

#endif

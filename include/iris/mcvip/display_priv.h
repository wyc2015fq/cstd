/*
 * display_priv.h
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


#ifndef _DISPLAY_PRIV_H_
#define _DISPLAY_PRIV_H_

#include <display.h>
#include <display_v4l2.h>
#include <osa_tsk.h>

#define DISPLAY_THR_PRI       (MCVIP_CAPTURE_THR_PRI_HIGH-2)
#define DISPLAY_STACK_SIZE    (10*KB)

#define DISPLAY_CMD_CREATE    (0x0200)
#define DISPLAY_CMD_START     (0x0201)
#define DISPLAY_CMD_STOP      (0x0202)  
#define DISPLAY_CMD_DELETE    (0x0203)
#define DISPLAY_CMD_NEXTPAGE  (0x0204) 

#define MAX_NUM_PAGES         (4) 
#define MAX_CHAN_PER_PAGE     (4)

typedef struct
{
  int numChannels;
  int chanNum[MAX_CHAN_PER_PAGE];
  int fbOffset[MAX_CHAN_PER_PAGE];
  int width;
  int height;
  int offsetH;
  int offsetV; 

} DISPLAY_pageProperties_t;

typedef struct 
{
  int displayOffsetH;
  int displayOffsetV;
  int displayWidth;
  int displayHeight;

  int numPages;
  int curPage;  

  DISPLAY_pageProperties_t page[MAX_NUM_PAGES];
  
} DISPLAY_LayoutCtrl;

typedef struct 
{
  int curBufId;
  int blankCount;

} DISPLAY_ChInfo;

typedef struct 
{
  CAPTURE_Info *pCaptureInfo;
  OSA_TskHndl tskHndl;
  OSA_MbxHndl mbxHndl;
  
  DISPLAY_FrameInfo frameInfo;
  int blankDetectThres;
  DISPLAY_ChInfo chInfo[MCVIP_CHANNELS_MAX];

  CaptureCB m_CallBack;
  long             m_Param;

} DISPLAY_Ctrl;

extern DISPLAY_BufInfo gDisplayBufInfo;
extern DISPLAY_Ctrl gDISPLAY_ctrl;
extern DISPLAY_LayoutCtrl gDISPLAY_layoutCtrl;
extern OSA_MutexHndl gDisplayBufLock;

int DISPLAY_tskCreate( DISPLAY_CreatePrm *prm);
int DISPLAY_tskDelete( );

int DISPLAY_layoutRun(DISPLAY_BufInfo *buf , short nextPage);

int DISPLAY_layoutInit();

int DISPLAY_layoutPrintCurPageProperties();

#endif

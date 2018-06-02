/*
 * capture_priv.h
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


#ifndef _CAPTURE_PRIV_H_
#define _CAPTURE_PRIV_H_

#include <capture.h>
#include <osa_tsk.h>

#define CAPTURE_THR_PRI       (MCVIP_CAPTURE_THR_PRI_HIGH-1)
#define CAPTURE_STACK_SIZE    (10*KB)

#define CAPTURE_CMD_CREATE    (0x0100)
#define CAPTURE_CMD_START     (0x0101)
#define CAPTURE_CMD_STOP      (0x0102)  
#define CAPTURE_CMD_DELETE    (0x0103)

typedef struct
{
    CAPTURE_Info info;
    OSA_TskHndl tskHndl;
    OSA_MbxHndl mbxHndl;
    short  saveFrame;
    int saveFileIndex;
    Uint8 *memVirtAddr;
    Uint8 *memPhysAddr;

    CaptureCB m_CallBack;
    long              m_Param;
} CAPTURE_Ctrl;

int CAPTURE_tskCreate( CAPTURE_CreatePrm *prm);
int CAPTURE_tskDelete( );
int CAPTURE_tskStart( );
int CAPTURE_tskStop( );
int CAPTURE_tskSaveFrame( MCVIP_BufInfo *pBufInfo);

#endif

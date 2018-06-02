/*
 * osa_buf.h
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


#ifndef _OSA_BUF_H_
#define _OSA_BUF_H_

#include <osa_que.h>
#include <osa_mutex.h>

#define OSA_BUF_NUM_MAX       (8*4)

#define OSA_BUF_ID_INVALID    (-1)

typedef struct {

  int size;
  int count;
  int flags;
  int timestamp;
  void *physAddr;
  void *virtAddr;

} OSA_BufInfo;

typedef struct {

  OSA_BufInfo bufInfo[OSA_BUF_NUM_MAX];
  
  OSA_QueHndl emptyQue;
  OSA_QueHndl fullQue;
      
  int numBuf;

} OSA_BufHndl;

typedef struct {

  void *bufPhysAddr[OSA_BUF_NUM_MAX];
  void *bufVirtAddr[OSA_BUF_NUM_MAX];  
  
  int numBuf;

} OSA_BufCreate;

int  OSA_bufCreate(OSA_BufHndl *hndl, OSA_BufCreate *bufInit);
int  OSA_bufDelete(OSA_BufHndl *hndl);

int  OSA_bufGetFull(OSA_BufHndl *hndl, int *bufId, Uint32 timeout);
int  OSA_bufPutEmpty(OSA_BufHndl *hndl, int bufId);

int  OSA_bufGetEmpty(OSA_BufHndl *hndl, int *bufId, Uint32 timeout);
int  OSA_bufPutFull(OSA_BufHndl *hndl, int bufId);

int  OSA_bufSwitchFull (OSA_BufHndl *hndl, int *bufId);
int  OSA_bufSwitchEmpty(OSA_BufHndl *hndl, int *bufId);

OSA_BufInfo *OSA_bufGetBufInfo(OSA_BufHndl *hndl, int bufId);

#endif /* _OSA_BUF_H_ */




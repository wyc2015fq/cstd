/*
 * osa_que.h
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


#ifndef _OSA_QUE_H_
#define _OSA_QUE_H_

#include <osa.h>

typedef struct {

  Uint32 curRd;
  Uint32 curWr;
  Uint32 len;
  Uint32 count;

  Int32 *queue;

  pthread_mutex_t lock;
  pthread_cond_t  condRd;
  pthread_cond_t  condWr;
  
} OSA_QueHndl;

int OSA_queCreate(OSA_QueHndl *hndl, Uint32 maxLen);
int OSA_queDelete(OSA_QueHndl *hndl);
int OSA_quePut(OSA_QueHndl *hndl, Int32  value, Uint32 timeout);
int OSA_queGet(OSA_QueHndl *hndl, Int32 *value, Uint32 timeout);


#endif /* _OSA_QUE_H_ */




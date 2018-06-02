/*
 * osa_thr.h
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


#ifndef _OSA_THR_H_
#define _OSA_THR_H_

#include <osa.h>

#define OSA_THR_PRI_MAX                 sched_get_priority_max(SCHED_FIFO)
#define OSA_THR_PRI_MIN                 sched_get_priority_min(SCHED_FIFO)

#define OSA_THR_PRI_DEFAULT             ( OSA_THR_PRI_MIN + (OSA_THR_PRI_MAX-OSA_THR_PRI_MIN)/2 )

#define OSA_THR_STACK_SIZE_DEFAULT      0

typedef void * (*OSA_ThrEntryFunc)(void *);

typedef struct {

  pthread_t      hndl;
  
} OSA_ThrHndl;

int OSA_thrCreate(OSA_ThrHndl *hndl, OSA_ThrEntryFunc entryFunc, Uint32 pri, Uint32 stackSize, void *prm);
int OSA_thrDelete(OSA_ThrHndl *hndl);
int OSA_thrJoin(OSA_ThrHndl *hndl);
int OSA_thrChangePri(OSA_ThrHndl *hndl, Uint32 pri);
int OSA_thrExit(void *returnVal);

#endif /* _OSA_THR_H_ */




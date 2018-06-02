/*
 * osa_sem.h
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

#ifndef _OSA_SEM_H_
#define _OSA_SEM_H_

#include <osa.h>

typedef struct {

  Uint32 count;
  Uint32 maxCount;
  pthread_mutex_t lock;
  pthread_cond_t  cond;

} OSA_SemHndl;


// sem
int OSA_semCreate(OSA_SemHndl *hndl, Uint32 maxCount, Uint32 initVal);
int OSA_semWait(OSA_SemHndl *hndl, Uint32 timeout);
int OSA_semSignal(OSA_SemHndl *hndl);
int OSA_semDelete(OSA_SemHndl *hndl);

#endif /* _OSA_FLG_H_ */




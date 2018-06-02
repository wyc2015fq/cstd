/*
 * osa_sem.c
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


#include <osa_sem.h>

int OSA_semCreate(OSA_SemHndl *hndl, Uint32 maxCount, Uint32 initVal)
{
  pthread_mutexattr_t mutex_attr;
  pthread_condattr_t cond_attr;
  int status=OSA_SOK;
 
  status |= pthread_mutexattr_init(&mutex_attr);
  status |= pthread_condattr_init(&cond_attr);  
  
  status |= pthread_mutex_init(&hndl->lock, &mutex_attr);
  status |= pthread_cond_init(&hndl->cond, &cond_attr);  

  hndl->count = initVal;
  hndl->maxCount = maxCount;

  if(hndl->maxCount==0)  
    hndl->maxCount=1;
    
  if(hndl->count>hndl->maxCount)
    hndl->count = hndl->maxCount;

  if(status!=OSA_SOK)
    OSA_ERROR("OSA_semCreate() = %d \r\n", status);
    
  pthread_condattr_destroy(&cond_attr);
  pthread_mutexattr_destroy(&mutex_attr);
  
  return status;
}

int OSA_semWait(OSA_SemHndl *hndl, Uint32 timeout)
{
  int status = OSA_EFAIL;

  pthread_mutex_lock(&hndl->lock);

  while(1) {
    if(hndl->count > 0) {
      hndl->count--;
      status = OSA_SOK;
      break;
    } else {
      if(timeout==OSA_TIMEOUT_NONE)
        break;

      pthread_cond_wait(&hndl->cond, &hndl->lock);
    }
  }

  pthread_mutex_unlock(&hndl->lock);

  return status;
}

int OSA_semSignal(OSA_SemHndl *hndl)
{
  int status = OSA_SOK;

  pthread_mutex_lock(&hndl->lock);

  if(hndl->count<hndl->maxCount) {
    hndl->count++;
    status |= pthread_cond_signal(&hndl->cond);
  }

  pthread_mutex_unlock(&hndl->lock);

  return status;
}

int OSA_semDelete(OSA_SemHndl *hndl)
{
  pthread_cond_destroy(&hndl->cond);
  pthread_mutex_destroy(&hndl->lock);  

  return OSA_SOK;
}



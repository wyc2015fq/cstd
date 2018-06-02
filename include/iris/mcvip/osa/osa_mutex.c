/*
 * osa_mutex.c
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


#include <osa_mutex.h>

int OSA_mutexCreate(OSA_MutexHndl *hndl)
{
    pthread_mutexattr_t mutex_attr;
    int status=OSA_SOK;

    status |= pthread_mutexattr_init(&mutex_attr);
    status |= pthread_mutex_init(&hndl->lock, &mutex_attr);

    if(status!=OSA_SOK)
    {
        OSA_ERROR("OSA_mutexCreate() = %d \r\n", status);
    }

    pthread_mutexattr_destroy(&mutex_attr);
    
    return status;
}

int OSA_mutexDelete(OSA_MutexHndl *hndl)
{
  pthread_mutex_destroy(&hndl->lock);  

  return OSA_SOK;
}

int OSA_mutexLock(OSA_MutexHndl *hndl)
{
  return pthread_mutex_lock(&hndl->lock);
}

int OSA_mutexUnlock(OSA_MutexHndl *hndl)
{
  return pthread_mutex_unlock(&hndl->lock);
}



/*
 * osa_flg.h
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


#ifndef _OSA_FLG_H_
#define _OSA_FLG_H_

#include <osa.h>

typedef struct {

  Uint32 pattern;
  pthread_mutex_t lock;
  pthread_cond_t  cond;

} OSA_FlgHndl;

// flags
#define OSA_FLG_MODE_AND   0x1
#define OSA_FLG_MODE_OR    0x2
#define OSA_FLG_MODE_CLR   0x4

#define OSA_FLG_MODE_AND_CLR   (OSA_FLG_MODE_AND|OSA_FLG_MODE_CLR)
#define OSA_FLG_MODE_OR_CLR    (OSA_FLG_MODE_OR |OSA_FLG_MODE_CLR)

int  OSA_flgCreate(OSA_FlgHndl *hndl, Uint32 initPattern);
int  OSA_flgWait(OSA_FlgHndl *hndl, Uint32 pattern, Uint32 mode, Uint32 timeout);
int  OSA_flgSet(OSA_FlgHndl *hndl, Uint32 pattern);
int  OSA_flgClear(OSA_FlgHndl *hndl, Uint32 pattern);
Bool OSA_flgIsSet(OSA_FlgHndl *hndl, Uint32 pattern);
int  OSA_flgDelete(OSA_FlgHndl *hndl);

#endif /* _OSA_FLG_H_ */




/*
 * drv_led.h
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

#ifndef _DRV_LED_H_
#define _DRV_LED_H_

#include <osa.h>

#define I2C_TRANSFER_SIZE_MAX   (254)

typedef struct 
{
  int fd;
} DRV_LedHndl;

int DRV_ledOpen(DRV_LedHndl *hndl, Uint8 devAddr);
int DRV_ledWrite8(DRV_LedHndl *hndl, Uint8 *value);
int DRV_ledClose(DRV_LedHndl *hndl);

#endif

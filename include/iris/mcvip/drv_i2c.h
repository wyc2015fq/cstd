/*
 * drv_i2c.h
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation version 2.

 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#ifndef _DRV_I2C_H_
#define _DRV_I2C_H_

#include <osa.h>

#define I2C_TRANSFER_SIZE_MAX   (254)

typedef struct {

  int fd;

} DRV_I2cHndl;

int DRV_i2cOpen(DRV_I2cHndl *hndl, Uint8 devAddr);
int DRV_i2cRead8  (DRV_I2cHndl *hndl, Uint8 *reg, Uint8 *value, Uint8 count);
int DRV_i2cWrite8 (DRV_I2cHndl *hndl, Uint8 *reg, Uint8 *value, Uint8 count);
int DRV_i2cRead16 (DRV_I2cHndl *hndl, Uint8 *reg, Uint16 *value, Uint8 count);
int DRV_i2cWrite16(DRV_I2cHndl *hndl, Uint8 *reg, Uint16 *value, Uint8 count);
int DRV_i2cWriteBlock(DRV_I2cHndl *hndl, Uint8 *reg, Uint8 *value, Uint8 count);

int DRV_i2cClose(DRV_I2cHndl *hndl);

#endif

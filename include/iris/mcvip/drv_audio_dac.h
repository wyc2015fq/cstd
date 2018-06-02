/*
 * drv_audio_dac2.h
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

#ifndef _DRV_AUDIO_DAC2_H_
#define _DRV_AUDIO_DAC2_H_

//#include <dev_i2c.h>
#include <drv_i2c.h>

int DRV_audioDacWrite(DRV_I2cHndl* hndl, Uint8* regAddr, Uint8* regVal, Uint8 k);

#endif


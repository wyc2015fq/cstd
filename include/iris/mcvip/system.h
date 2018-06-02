/*
 * system.h
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


#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define TVP5158_A_I2C_ADDR    (0xB0)
#define TVP5158_B_I2C_ADDR    (0xB2)
#define TVP5158_I2C_ADDR_NONE (-1)

#define TLV320DAC3x_A_I2C_ADDR (0x30)
#define TLV320DAC3x_B_I2C_ADDR (0x32)


int SYSTEM_init(int debug,int numMCVIP);
int SYSTEM_exit();

char SYSTEM_getInput();

#endif 

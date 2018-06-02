/*
 * dev_i2c.h
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


#ifndef _DEV_I2C_H_
#define _DEV_I2C_H_

#include <linux/ioctl.h>


#define I2C_DRV_NAME  "dev_i2c"
//#define I2C_DEBUG

#define I2C_DEV_MAX_ADDR  (0xFF)

#define I2C_DRV_MAGICNUM      (0x2C)

#define I2C_CMD_SET_DEV_ADDR        (0x01)
#define I2C_CMD_READ                (0x02)
#define I2C_CMD_WRITE               (0x03)
#define I2C_CMD_WRITE_BLOCK         (0x04)

#define I2C_IOCTL_CMD_MAKE(cmd)     ( _IO( I2C_DRV_MAGICNUM, cmd) )
#define I2C_IOCTL_CMD_GET(cmd)      ( _IOC_NR(cmd) )
#define I2C_IOCTL_CMD_IS_VALID(cmd) ( (_IOC_TYPE(cmd) == I2C_DRV_MAGICNUM ) ? 1 : 0)


#define I2C_TRANSFER_BUF_SIZE_MAX   (256)
typedef struct {
  unsigned char dataSize;
  unsigned char count;
  unsigned char *reg;  
  void          *value;
} I2C_TransferPrm;


int  I2C_devInit(void);
void I2C_devExit(void);

#endif


/*
 * system.c
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

#include <system.h>
#include <osa_cmem.h>
#include <mcvip.h>

int SYSTEM_init(int debug,int numMCVIP)
{
    int devAddr[MCVIP_VIDEO_INPUT_PORT_MAX * MCVIP_TVP5158_MAX_CASCADE] = { 0 };
    int audioDacAddr = 0;
    int status = CMEM_init();
    if( status < 0 )
    {
        OSA_ERROR("CMEM_init()\n");
        return status;
    }
    status = DRV_dmaInit();
    if( status < 0 )
    {
        OSA_ERROR(" DRV: ERROR: DRV_dmaInit()\n");
        return CMEM_exit();
    }
    if( !debug )
    {
        // control TVP5158 I2C via software
        if( numMCVIP == 2 )
        {
            devAddr[0] = TVP5158_A_I2C_ADDR;
            devAddr[1] = TVP5158_B_I2C_ADDR;
        }
        else
        {
            devAddr[0] = TVP5158_A_I2C_ADDR;
            devAddr[1] = TVP5158_I2C_ADDR_NONE;
        }

        audioDacAddr = TLV320DAC3x_A_I2C_ADDR;
    }
    else
    {
        // debug mode, let WinVCC tool control TVP5158 I2C
    }

    status = MCVIP_init( devAddr , audioDacAddr );

    if(status < 0)
    {
        OSA_ERROR("MCVIP_init()\n");
        CMEM_exit();
        return status;
    }

    return status;
}

int SYSTEM_exit()
{
    MCVIP_exit();
    CMEM_exit();
    DRV_dmaExit();
    return OSA_SOK;
}

char SYSTEM_getInput()
{
    return getchar();
}

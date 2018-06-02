/*
 * mcvip_tvp5158_i2c.h
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


#include <mcvip_priv.h>

//#define TVP5158_FORCE_PATCH_DOWNLOAD

static const Uint8 gTVP5158_patch[] =
{
#include <tvp5158_patch_v02_01_17.h>
};

Uint8 TVP5158_getStatus(DRV_I2cHndl *i2cHndl, Uint8 chNum)
{
    int status;
    Uint8 regAddr[2];
    Uint8 regVal[2];
    Uint32 k;
    Uint8 readMask = (Uint8)(1 << chNum);

    if(chNum>=4)
        return 0;

    status = TVP5158_selectRead(i2cHndl, readMask);
    if(status!=OSA_SOK)
        return 0;

    k=0;
    regAddr[k] = 0x0;
    regVal[k]  = 0;
    k++;

    status = DRV_i2cRead8(i2cHndl, regAddr, regVal, k);
    if(status!=OSA_SOK)
        return 0;

    return regVal[0];
}

int TVP5158_selectWrite(DRV_I2cHndl *i2cHndl, Uint8 value)
{
    Uint8 regAddr[1];
    Uint8 regVal[1];
    Uint32 k;

    k=0;

    regAddr[k] = 0xFE;
    regVal[k]  = value;
    k++;

    return DRV_i2cWrite8(i2cHndl, regAddr, regVal, k);
}

int TVP5158_selectRead(DRV_I2cHndl *i2cHndl, Uint8 value)
{
    Uint8 regAddr[1];
    Uint8 regVal[1];
    Uint32 k = 0;

    regAddr[k] = 0xFF;
    regVal[k]  = value;
    k++;

    return DRV_i2cWrite8(i2cHndl, regAddr, regVal, k);
}

int TVP5158_vbusWrite(DRV_I2cHndl *i2cHndl, Uint32 vbus_addr, Uint8 val, Uint8 len)
{
    int k;
    Uint8 regAddr[4];
    Uint8 regVal[4];

    k=0;

    regAddr[k] = 0xE8;
    regVal[k]  = (Uint8)((vbus_addr>>0) & 0xFF);
    k++;

    regAddr[k] = 0xE9;
    regVal[k]  = (Uint8)((vbus_addr>>8) & 0xFF);
    k++;

    regAddr[k] = 0xEA;
    regVal[k]  = (Uint8)((vbus_addr>>16) & 0xFF);
    k++;

    if(len) {
        regAddr[k] = 0xE0;
        regVal[k]  = val;
        k++;
    }

    return DRV_i2cWrite8(i2cHndl, regAddr, regVal, k);
}

int TVP5158_ofmReset(DRV_I2cHndl *i2cHndl)
{
    int status=OSA_SOK;

    int k;
    Uint8 regAddr[2];
    Uint8 regVal[2];

    OSA_waitMsecs(100);
    status = TVP5158_selectWrite(i2cHndl, 0x1);
    if(status!=OSA_SOK)
        goto error_exit;

    k=0;

    regAddr[k] = 0xBA;
    regVal[k] = 0x01;
    k++;

    status = DRV_i2cWrite8(i2cHndl, regAddr, regVal, k);
    if(status!=OSA_SOK)
        goto error_exit;

    OSA_waitMsecs(100);

    error_exit:
    if(status!=OSA_SOK)
        OSA_ERROR("I2C\n");

    return status;
}

int TVP5158_softReset(DRV_I2cHndl *i2cHndl)
{
    int status=OSA_SOK;
    Uint8 regAddr[8];
    Uint8 regVal[8];
    Uint32 k;
    Uint8 vbusStatus;

    status = TVP5158_selectWrite(i2cHndl, 0xF);
    if(status!=OSA_SOK)
        goto error_exit;

    k=0;

    regAddr[k] = 0xE8;
    regVal[k] = 0x60;
    k++;

    regAddr[k] = 0xE9;
    regVal[k] = 0x00;
    k++;

    regAddr[k] = 0xEA;
    regVal[k] = 0xB0;
    k++;

    DRV_i2cWrite8(i2cHndl, regAddr, regVal, k);

    k=0;

    regAddr[k] = 0xE0;
    regVal[k] = 0;
    k++;

    status = DRV_i2cRead8(i2cHndl, regAddr, regVal, k);
    if(status!=OSA_SOK)
        goto error_exit;

    vbusStatus = regVal[0] | 1;

    status = TVP5158_vbusWrite(i2cHndl, 0xB00060, vbusStatus, 1);
    if(status!=OSA_SOK)
        goto error_exit;

    vbusStatus &= ~(0x1);

    status = TVP5158_vbusWrite(i2cHndl, 0xB00060, vbusStatus, 1);
    if(status!=OSA_SOK)
        goto error_exit;

    OSA_waitMsecs(300);

    error_exit:
    if(status!=OSA_SOK)
    {
        OSA_ERROR("I2C");
    }

    return status;
}

int TVP5158_extraInit(DRV_I2cHndl *i2cHndl)
{
    int status=OSA_SOK;

    int k;
    Uint8 regAddr[2];
    Uint8 regVal[2];

    OSA_waitMsecs(100);
    status = TVP5158_selectWrite(i2cHndl, 0x1);
    if(status!=OSA_SOK)
        goto error_exit;

    k=0;

    regAddr[k] = 0xB0;
    regVal[k] = 0x50;
    k++;

    regAddr[k] = 0xB1;
    regVal[k] = 0x50;
    k++;

    status = DRV_i2cWrite8(i2cHndl, regAddr, regVal, k);
    if(status!=OSA_SOK)
        goto error_exit;

    OSA_waitMsecs(100);

    error_exit:
    if(status!=OSA_SOK)
        OSA_ERROR("I2C\n");

    return status;
}

int TVP5158_checkChipId(DRV_I2cHndl *i2cHndl)
{
    int status = OSA_SOK, k = 0;
    Uint8 regAddr[2] = { 0 };
    Uint8 regVal[2] = { 0 };

    status = TVP5158_selectRead(i2cHndl, 1);
    if(status!=OSA_SOK)
    {
        return status;
    }

    regAddr[k] = 0x08;
    regVal[k]  = 0;
    k++;

    regAddr[k] = 0x09;
    regVal[k]  = 0;
    k++;

    status = DRV_i2cRead8(i2cHndl, regAddr, regVal, k);
    if(status!=OSA_SOK)
    {
        return status;
    }

#if 0
    if(regVal[0] != 0x51 || regVal[1] != 0x58 )
        return OSA_EFAIL;
#endif

    return OSA_SOK;
}

int TVP5158_patchDownload(DRV_I2cHndl *i2cHndl)
{
    int status=OSA_SOK;
    //Will pass sub-address separately
    //Will pass array of 252 data which will be sent as one block
    Uint8 regAddr[I2C_TRANSFER_SIZE_MAX-2];
    Uint8 regVal[8];
    Uint8 vbusStatus;
    Uint32 k, wrSize;
    Uint8 *patchAddr;
    Uint32 patchSize;

    patchAddr = (Uint8*)gTVP5158_patch;
    patchSize = sizeof(gTVP5158_patch);

    status = TVP5158_selectWrite(i2cHndl, 0xF);
    if(status!=OSA_SOK)
    {
        OSA_ERROR("\n");
        goto error_exit;
    }

    k=0;

    regAddr[k] = 0xE8;
    regVal[k] = 0x60;
    k++;

    regAddr[k] = 0xE9;
    regVal[k] = 0x00;
    k++;

    regAddr[k] = 0xEA;
    regVal[k] = 0xB0;
    k++;

    DRV_i2cWrite8(i2cHndl, regAddr, regVal, k);

    status = TVP5158_selectRead(i2cHndl, 1);
    if(status!=OSA_SOK)
    {
        OSA_ERROR( "TVP5158_selectRead" );
        goto error_exit;
    }

    k=0;

    regAddr[k] = 0xE0;
    regVal[k] = 0;
    k++;

    status = DRV_i2cRead8(i2cHndl, regAddr, regVal, k);
    if(status!=OSA_SOK)
    {
        OSA_ERROR( "DRV_i2cRead8" );
        goto error_exit;
    }

    vbusStatus = regVal[0];

//#define TVP5158_FORCE_PATCH_DOWNLOAD
#ifndef TVP5158_FORCE_PATCH_DOWNLOAD
    if( vbusStatus & 0x2 )
    {
        OSA_printf(" MCVIP: Patch is already running.\n");
        return status;  // patch already running
    }
#endif

    status = TVP5158_selectWrite(i2cHndl, 0xF);
    if(status!=OSA_SOK)
    {
        OSA_ERROR("\n");
        goto error_exit;
    }

    vbusStatus |= 0x1;

    status = TVP5158_vbusWrite(i2cHndl, 0xB00060, vbusStatus, 1);
    if(status!=OSA_SOK)
    {
        OSA_ERROR("\n");
        goto error_exit;
    }

    status = TVP5158_vbusWrite(i2cHndl, 0x400000, 0, 0);
    if(status!=OSA_SOK)
    {
        OSA_ERROR("\n");
        goto error_exit;
    }

    //MAT 0129
    //memset(regAddr, 0xE1, sizeof(regAddr));
    memset(regAddr, 0xE1, 1);
    while( patchSize )
    {

        if(patchSize < sizeof(regAddr))
            wrSize = patchSize;
        else
            wrSize = sizeof(regAddr);

        //MAT 0129 - Write 0xE1 subaddress followed by 252 data
        //Only first byte of regAddr gets used
        //  status = DRV_i2cWrite8(i2cHndl, regAddr, patchAddr, wrSize);
        status = DRV_i2cWriteBlock(i2cHndl, regAddr, patchAddr, wrSize);
        if(status!=OSA_SOK)
        {
            OSA_ERROR("\n");
            goto error_exit;
        }

        patchAddr += wrSize;
        patchSize -= wrSize;
    }

    vbusStatus |= 0x3;

    status = TVP5158_vbusWrite(i2cHndl, 0xB00060, vbusStatus, 1);
    if(status!=OSA_SOK)
    {
        OSA_ERROR("\n");
        goto error_exit;
    }

    vbusStatus &= ~(0x1);

    status = TVP5158_vbusWrite(i2cHndl, 0xB00060, vbusStatus, 1);
    if(status!=OSA_SOK)
    {
        OSA_ERROR("\n");
        goto error_exit;
    }

    OSA_waitMsecs(300);

    error_exit:
    if(status!=OSA_SOK)
    {
        OSA_ERROR("I2C");
    }

    return status;
}

int TVP5158_setOfmMode(DRV_I2cHndl *i2cHndl, int mode, int videoIfMode, int videoSystem, int cascadeStage)
{
    int status=OSA_SOK;
    Uint8 regAddr[64];
    Uint8 regVal[64];
    Uint32 k;
    Uint8 B0, B1, B2, B3, B4, B5, B6, B7;

    status = TVP5158_checkChipId(i2cHndl);
    if(status!=OSA_SOK)
    {
        OSA_ERROR("TVP5158_checkChipId()\n");
        return status;
    }

    status = TVP5158_ofmReset(i2cHndl);
    if(status!=OSA_SOK)
    {
        OSA_ERROR("TVP5158_ofmReset()\n");
        return status;
    }

    status = TVP5158_patchDownload(i2cHndl);
    if( status!=OSA_SOK )
    {
        OSA_ERROR("TVP5158_patchDownload()\n");
        return status;
    }

    B1 = 0x10;
    B2 = 0x25;
    B3 = 0xE4;
    B4 = 0xE4;
    B5 = 0x00;
    B6 = 0x1B;
    B7 = 0x04;

    switch(mode) {

    case MCVIP_VIDEO_DECODER_MODE_2CH_D1:
        B0 = 0x90;
        break;

    case MCVIP_VIDEO_DECODER_MODE_2CH_D1_CROP:
        B0 = 0x90;
        B1 = 0x50;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1:
        B0 = 0xA2;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_CROP:
        B0 = 0xA2;
        B1 = 0x50;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF:
        B0 = 0xA3;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF_CROP:
        B0 = 0xA3;
        B1 = 0x50;
        break;

    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF:
        if(cascadeStage==0)
            B0 = 0xB3;
        else
            B0 = 0xB7;
        break;

    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_CROP:
        B1 = 0x50;
        if(cascadeStage==0)
            B0 = 0xB3;
        else
            B0 = 0xB7;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1:
        B0 = 0xE2;
        B2 = 0x65;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1_CROP:
        B0 = 0xE2;
        B1 = 0x50;
        B2 = 0x65;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1:
        B0 = 0xE3;
        break;

    case MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1_CROP:
        B0 = 0xE3;
        B1 = 0x50;
        break;

    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1:
        B2 = 0x65;
        if(cascadeStage==0)
            B0 = 0xF3;
        else
            B0 = 0xF7;
        break;

    case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1_CROP:
        B1 = 0x50;
        B2 = 0x65;
        if(cascadeStage==0)
            B0 = 0xF3;
        else
            B0 = 0xF7;
        break;

    default:
        OSA_ERROR("Unsupported mode %d\n", mode);
        return OSA_EFAIL;
    }

    status = TVP5158_selectWrite(i2cHndl, 0xF);
    if(status!=OSA_SOK)
    {
        OSA_ERROR( "TVP5158_selectWrite" );
        goto error_exit;
    }

    k=0;

    regAddr[k] = 0xB0;
    regVal[k]  = B0;
    k++;

    regAddr[k] = 0xB1;
    regVal[k]  = B1;
    k++;

    regAddr[k] = 0xBE;
    regVal[k]  = 0xF0;
    k++;

    status = DRV_i2cWrite8(i2cHndl, regAddr, regVal, k);
    if(status!=OSA_SOK)
    {
        OSA_ERROR( "DRV_i2cWrite8" );
        goto error_exit;
    }

    status = TVP5158_selectWrite(i2cHndl, 0x1);
    if(status!=OSA_SOK)
    {
        OSA_ERROR( "TVP5158_selectWrite" );
        goto error_exit;
    }

    //Caution:
    //From here on all write go to decoder 1 of 4 only

    k=0;

    regAddr[k] = 0xB2;
    regVal[k]  = B2;
    k++;

    regAddr[k] = 0xB3;
    regVal[k]  = B3;
    k++;

    regAddr[k] = 0xB4;
    regVal[k]  = B4;
    k++;

    regAddr[k] = 0xB5;
    regVal[k]  = B5;
    k++;

    regAddr[k] = 0xB6;
    regVal[k]  = B6;
    k++;

    regAddr[k] = 0xB7;
    regVal[k]  = B7;
    k++;

    status = DRV_i2cWrite8(i2cHndl, regAddr, regVal, k);
    if(status!=OSA_SOK)
    {
        OSA_ERROR( "DRV_i2cWrite8" );
        goto error_exit;
    }

    OSA_waitMsecs(20);

    error_exit:
    if(status!=OSA_SOK)
    {
        OSA_ERROR("I2C");
    }

    return status;
}

// AYK - 0731 start
int TVP5158_setAudioMode(DRV_I2cHndl *i2cHndl, int mode, int cascadeStage)
{
    int status=OSA_SOK;
    Uint8 regAddr[32];
    Uint8 regVal[32];
    Uint32 k;
    Uint8 C0, C1, C2, C3, C4, C5, C6, C7, C8;


    if (cascadeStage >=2 )
    {
  	status = TVP5158_patchDownload(i2cHndl);
  	if(status!=OSA_SOK)
  	{
            OSA_ERROR("TVP5158_patchDownload()\n");
            return status;
  	}
    }


    C0 = 0x00; // 16KHz
    C1 = 0x88; // Gain Ch1 and Ch2
    C2 = 0x88; // Gain Ch3 and Ch4
    //MAT 0126
    //  C4 = 0x02; // Mixer select, TDM_Chan_Number[2:0] 2: 8 channels
    C4 = 0x04; // Mixer select, TDM_Chan_Number[2:0] 4: 16 channels
    C5 = 0x00; // Audio Mute Control
    C6 = 0x00; // Analog Mixing Ratio Control 1
    C7 = 0x00; // Analog Mixing Ratio Control 2

    switch (cascadeStage)
    {
    case 0:
        C3 = 0x70; // SD_M disabled, SD_R enabled, TVP5158 Master, DSP mode, 256-fs, 16-bit PCM, SD_R only
        C8 = 0x00; // First stage channels 1-4
        break;
    case 1:
        C3 = 0x20; // SD_M disabled, SD_R disabled, Master, I2S mode
        C8 = 0x01; // Second stage channels 5-8
        break;
    case 2:
        C3 = 0x20; // SD_M disabled, SD_R disabled, Master, I2S mode
        C8 = 0x02; // Third stage channels 9-12
        break;
    case 3:
        C3 = 0x20; // SD_M disabled, SD_R disabled, Master, I2S mode
        C8 = 0x03; // Fourth stage channels 13-16
        break;
    default:
        status = -1;
        goto error_exit;
        break;
    }

    // Audio channel Mute control

    C5 = 0x00;

    /* if((mode == MCVIP_VIDEO_DECODER_MODE_2CH_D1_PORT_A) ||
      (mode == MCVIP_VIDEO_DECODER_MODE_2CH_D1_PORT_B))
  {
      C5 = 0x0C;
   }*/

    k=0;
    regAddr[k] = 0xC0;
    regVal[k]  = C0;
    k++;

    regAddr[k] = 0xC1;
    regVal[k]  = C1;
    k++;

    regAddr[k] = 0xC2;
    regVal[k]  = C2;
    k++;

    regAddr[k] = 0xC3;
    regVal[k]  = C3;
    k++;

    regAddr[k] = 0xC4;
    regVal[k]  = C4;
    k++;

    regAddr[k] = 0xC5;
    regVal[k]  = C5;
    k++;

    regAddr[k] = 0xC6;
    regVal[k]  = C6;
    k++;

    regAddr[k] = 0xC7;
    regVal[k]  = C7;
    k++;

    regAddr[k] = 0xC8;
    regVal[k]  = C8;
    k++;

    status = TVP5158_selectWrite(i2cHndl, 1);
    if(status != OSA_SOK)
        goto error_exit;

    status = DRV_i2cWrite8(i2cHndl, regAddr, regVal, k);
    if(status != OSA_SOK)
        goto error_exit;

    OSA_waitMsecs(40);

    error_exit:
    if(status!=OSA_SOK)
    {
        OSA_ERROR("I2C Audio setting cascadeStage=%d",cascadeStage);
    }

    return status;
}
// AYK - 0731 end

int TVP5158_setAudVol(DRV_I2cHndl *i2cHndl, Uint8 vol, Uint8 channel)
{
    int status = OSA_SOK;
    Uint8 regAddr[2];
    Uint8 regVal[2];
    Uint8 shiftVal = 0;
    Uint8 andVal = 0;
    Uint8 lowLimit = 0;
    Uint8 hiLimit = 8;

    status = TVP5158_selectWrite(i2cHndl, 1);
    if(status!=OSA_SOK)
        goto error_exit;

    //MAT if((vol < 0x0) || (vol > 0xF))
    if((vol < lowLimit) || (vol > hiLimit))
        vol = 0x8; // Default value(0 db)

    switch(channel)
    {
    case 0:
        shiftVal = 0;
        andVal   = 0xF0;
        regAddr[0] = 0xC1;
        break;

    case 1:
        shiftVal = 4;
        andVal   = 0x0F;
        regAddr[0] = 0xC1;
        break;

    case 2:
        shiftVal = 0;
        andVal   = 0xF0;
        regAddr[0] = 0xC2;
        break;

    case 3:
        shiftVal = 4;
        andVal   = 0x0F;
        regAddr[0] = 0xC2;
        break;
    }

    // read the aud vol register
    status = TVP5158_selectRead(i2cHndl, 1);
    if(status!=OSA_SOK)
        goto error_exit;

    status = DRV_i2cRead8(i2cHndl, regAddr, regVal, 1);
    if(status != OSA_SOK)
        goto error_exit;

    regVal[0] &= andVal;
    regVal[0] |= (vol << shiftVal);

    status = DRV_i2cWrite8(i2cHndl, regAddr, regVal, 1);
    if(status!=OSA_SOK)
        goto error_exit;

    error_exit:

    if(status != OSA_SOK)
    {
        OSA_ERROR("I2C-Audio Volume");
    }

    return status;
}

//This is not useful since it only applies to the TVP5158 SD_M output with mixer selected
int TVP5158_setAudMute(DRV_I2cHndl *i2cHndl, int enableMute, Uint8 channel)
{
    int status = OSA_SOK;
    Uint8 regAddr[2];
    Uint8 regVal[2];
    Uint8	setMute = 0;

    setMute	= enableMute << channel;
    OSA_printf(" @@@@@@@@@@@@TVP5158: setMute = %X\n",setMute);

    status = TVP5158_selectRead(i2cHndl, 1);
    if(status!=OSA_SOK)
        goto error_exit;

    regAddr[0] = 0xC5;
    status = DRV_i2cRead8(i2cHndl, regAddr, regVal, 1);
    if(status != OSA_SOK)
	goto error_exit;

    regVal[0] |= setMute;

    if(enableMute > 0)
        setMute = 0xF;
    else
        setMute = 0;

    regVal[0] = setMute;

    //MAT status = TVP5158_selectWrite(i2cHndl, 0xF);
    status = TVP5158_selectWrite(i2cHndl, 0x1);
    if(status!=OSA_SOK)
        goto error_exit;

    status = DRV_i2cWrite8(i2cHndl, regAddr, regVal, 1);
    if(status!=OSA_SOK)
        goto error_exit;

    OSA_printf(" @@@@@@@@@@@@TVP5158: C5 = %X\n",regVal[0]);

    error_exit:
    if(status != OSA_SOK)
        OSA_ERROR("I2C-Audio MUTE");

    return status;
}

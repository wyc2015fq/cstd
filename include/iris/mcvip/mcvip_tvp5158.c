/*
 * mcvip_tvp5158.c
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
#include <drv_tvp5158.h>

/*
 * This code was in the DM6446+TVP5158 code dated 7/1/2009 from TI India
 *
 */
int MCVIP_tvp5158GetInfo(MCVIP_Hndl *hndl)
{
  strcpy(hndl->v4l2InputToUse, "COMPOSITE");   
  switch(hndl->createPrm.videoDecoderMode)
  {
  case MCVIP_VIDEO_DECODER_MODE_2CH_D1:
      hndl->v4l2FrameInfo.width = 728;
      hndl->v4l2FrameInfo.height = 1250;
      break;

    default:
      return OSA_EFAIL;
  }

  hndl->v4l2FrameInfo.offsetH = OSA_align(hndl->v4l2FrameInfo.width, 16);
  hndl->v4l2FrameInfo.offsetV = hndl->v4l2FrameInfo.height;

  return OSA_SOK;
}

int MCVIP_tvp5158Start(MCVIP_Hndl *hndl)
{
    int status = TVP5158_setOfmMode(
                hndl->pI2cHndl[0],
                hndl->createPrm.videoDecoderMode,
                hndl->createPrm.videoIfMode,
                hndl->createPrm.videoSystem,
                0
              );
  return status;
}

int MCVIP_tvp5158Setup(MCVIP_Hndl *hndl)
{
  return OSA_SOK;
}

int MCVIP_tvp5158GetChLockStatus(MCVIP_Hndl *hndl)
{
  int i, status=OSA_SOK;
  Uint8 value;

  memset(hndl->chLockStatus, 0, sizeof(hndl->chLockStatus));

  if(hndl->pI2cHndl[0]!=NULL) {

    for(i=0; i<4; i++) {

      value = TVP5158_getStatus(hndl->pI2cHndl[0], i);

      if(value & 0x6)
        hndl->chLockStatus[i]=1;

    }

    // if cascade mode, init the second TVP5158
    switch(hndl->createPrm.videoDecoderMode) {

      case MCVIP_VIDEO_DECODER_MODE_8CH_CIF:
      case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_CROP:
      case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1:
      case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1_CROP:

        if(hndl->pI2cHndl[1]==NULL)
          return OSA_EFAIL;

        for(i=0; i<4; i++) {

          value = TVP5158_getStatus(hndl->pI2cHndl[1], i);

          if(value & 0x6)
            hndl->chLockStatus[4+i]=1;

        }

        break;
    }

    if( hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1          ||
        hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1_CROP     ||
        hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1          ||
        hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1_CROP     ||
        hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1      ||
        hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1_CROP )
    {
			//MAT - Depends on the selected preview channel
      hndl->chLockStatus[hndl->chList.numCh-1] = hndl->chLockStatus[0];
    }
  }

  return status;
}

int MCVIP_tvp5158Stop(MCVIP_Hndl *hndl)
{
    int status=OSA_SOK;

    if( hndl->pI2cHndl[0] != NULL )
    {
        status = TVP5158_softReset(hndl->pI2cHndl[0]);
        if(status!=OSA_SOK)
        {
            return status;
        }

        // if cascade mode, init the second TVP5158
        switch(hndl->createPrm.videoDecoderMode)
        {
        case MCVIP_VIDEO_DECODER_MODE_8CH_CIF:
        case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_CROP:
        case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1:
        case MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1_CROP:

            if(hndl->pI2cHndl[1]==NULL)
                return OSA_EFAIL;

            status = TVP5158_softReset(hndl->pI2cHndl[1]);
            if(status!=OSA_SOK)
                return status;

            break;
        } //switch(hndl->createPrm.videoDecoderMode)
    } //if(hndl->pI2cHndl[0]!=NULL)

    return status;
}

int MCVIP_tvp5158SetAudVol(MCVIP_Hndl *hndl, Uint8 channel)
{
    int status = OSA_SOK,chNum;
		Uint8 limit = 4;

    if(channel >= limit)
    {
	    if(hndl->pI2cHndl[1] == NULL)
	    {
		    return OSA_EFAIL;
		  }

	    OSA_printf(" MCVIP: TVP5158 set audio volume[%d] for channel[%d]\n",hndl->createPrm.audVol[channel],channel);

	    chNum = channel - 4;
	    status = TVP5158_setAudVol(hndl->pI2cHndl[1],hndl->createPrm.audVol[channel],chNum);
	    if(status != OSA_SOK)
	    {
	        return OSA_EFAIL;
	    }
	  }
	  else
	  {
	    if(hndl->pI2cHndl[0] == NULL)
	    {
		    return OSA_EFAIL;
		  }
		}

	  OSA_printf(" MCVIP: TVP5158 set audio volume[%d] for channel[%d]\n",hndl->createPrm.audVol[channel],channel);

	  status = TVP5158_setAudVol(hndl->pI2cHndl[0],hndl->createPrm.audVol[channel],channel);
	  if(status != OSA_SOK)
	  {
	    return OSA_EFAIL;
	  }
		
		return OSA_SOK;
}

int MCVIP_tvp5158SetAudMute(MCVIP_Hndl *hndl, Uint8 channel)
{
	int status = OSA_SOK;
	int chNum = 0;

	if(channel >= 4)
	{
		if(hndl->pI2cHndl[1] == NULL)
		  return OSA_EFAIL;

    chNum = channel - 4;
    status = TVP5158_setAudMute(hndl->pI2cHndl[1],hndl->createPrm.audMute[channel],chNum);
    if(status != OSA_SOK)
        return OSA_EFAIL;
	}
	else
	{
		if(hndl->pI2cHndl[0] == NULL)
		  return OSA_EFAIL;

	    status = TVP5158_setAudMute(hndl->pI2cHndl[0],hndl->createPrm.audMute[channel],channel);
	    if(status != OSA_SOK)
        return OSA_EFAIL;
	}
	
	return status;
}


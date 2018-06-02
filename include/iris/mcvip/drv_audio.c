/*
 * drv_audio.c
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


#include <drv_audio.h>
#include <dev_i2c.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

// Open audio capture device
int DRV_audioOpenRec(DRV_AudioHndl *hndl,DRV_AudioConfig *config)
{
    int format;
    int sampleRate;

    hndl->fdRec = open(DRV_AUDIO_RECORD_DEVICE_NAME,O_RDONLY);

    if(hndl->fdRec < 0)
    {
		  OSA_printf( "DRV_audioOpenRec(): open() FAILED.\n" );
	    goto error_exit;
    }

    // Set the sample size(bits per sample)
    format = config->format;
    if (ioctl(hndl->fdRec,SNDCTL_DSP_SETFMT,&format) == -1)
    {
		  OSA_printf( "DRV_audioOpenRec(): SNDCTL_DSP_SETFMT FAILED.\n" );
	    goto error_exit;
    }

    // Set the sample rate
    sampleRate = config->samplingRate;
    if(ioctl(hndl->fdRec,SNDCTL_DSP_SPEED,&sampleRate) == -1)
    {
   		  OSA_printf( "DRV_audioOpenRec(): SNDCTL_DSP_SPEED FAILED.\n" );
        goto error_exit;
    }

		return OSA_SOK;

error_exit:

    if(hndl->fdRec > 0)
    {
	    close(hndl->fdRec);
	}

	return OSA_EFAIL;
}

// Open audio playback device
int DRV_audioOpenPlay(DRV_AudioHndl *hndl,DRV_AudioConfig *config)
{
    int format;
    int channels;
    int sampleRate;

	hndl->fdPlay = open(DRV_AUDIO_PLAY_DEVICE_NAME,O_WRONLY);

	if(hndl->fdPlay < 0)
	{
	  OSA_printf( "DRV_audioOpenPlay(): Open failed.  hndl->fdPlay=%X\n", hndl->fdPlay );
		goto error_exit;
	}

	// Set the sample size(bits per sample)
	format = config->format;
	if (ioctl(hndl->fdPlay,SNDCTL_DSP_SETFMT,&format) == -1)
	{
		goto error_exit;
	}

	// Set the no of channels
	channels = config->numChannels;
	if(ioctl(hndl->fdPlay,SNDCTL_DSP_CHANNELS,&channels) == -1)
	{
		goto error_exit;
	}

	// Set the sample rate
	sampleRate = config->samplingRate;
	if(ioctl(hndl->fdPlay,SNDCTL_DSP_SPEED,&sampleRate) == -1)
	{
		goto error_exit;
	}

	return OSA_SOK;

error_exit:

    if(hndl->fdPlay > 0)
    {
	    close(hndl->fdPlay);
	}

	return OSA_EFAIL;
}

// Record audio
int DRV_audioRecord(DRV_AudioHndl *hndl,Int8 *recBuf,Uint32 recBufSize)
{
    int ret;

    if((ret = read(hndl->fdRec,(void *)recBuf,recBufSize)) < 0)
    {
	    return OSA_EFAIL;
	}

    return OSA_SOK;
}

// Playback audio
int DRV_audioPlay(DRV_AudioHndl *hndl,Int8 *playBuf,Uint32 playBufSize)
{
    int ret;

    if((ret = write(hndl->fdPlay,(void *)playBuf,playBufSize)) < 0)
    {
	    return OSA_EFAIL;
	}

    return OSA_SOK;
}

// Close audio record device
int DRV_audioCloseRec(DRV_AudioHndl *hndl)
{
    if(hndl->fdRec > 0)
    {
	    close(hndl->fdRec);
	}

    return OSA_SOK;
}

// Close audio playback device
int DRV_audioClosePlay(DRV_AudioHndl *hndl)
{
	if(hndl->fdPlay > 0)
	{
	    close(hndl->fdPlay);
	}

    return OSA_SOK;
}

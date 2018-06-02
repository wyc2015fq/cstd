/*
 * mcvip_v4l2.h
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

#define INPUT_ACQUIRE_RETRY_COUNT 10

int  MCVIP_v4l2SetupBufferTable(int fd, int buf_count, Uint8** buffers, int *buffer_size, int buffer_type);

int MCVIP_v4l2Create(MCVIP_Hndl *hndl)
{
  /* v4l2 params*/
  int                        fd=-1;
  //int                        input_to_use = -1;
  //struct v4l2_capability     cap;
  //struct v4l2_input          input;
  //struct v4l2_format         fmt;
  struct v4l2_requestbuffers bufreq;
  struct v4l2_buffer         buf;  
  //v4l2_std_id                std_supported=0;
  int status;
  int buf_count, buffer_type, buffer_size;
  int i;
  ccdc_params_ycbcr tvp5158_ccdc_params;

  if(MCVIP_tvp5158GetInfo(hndl) < 0)
  {
    OSA_ERROR("Invalid parameters\n");
    return OSA_EFAIL;  
  }
  
  sprintf(hndl->v4l2DeviceName, "/dev/video%d", hndl->createPrm.videoInputPort);

  // open v4l2 device
  fd = open(hndl->v4l2DeviceName, O_RDWR, 0);
  //printf("Open device Name == %s \r\n" , hndl->v4l2DeviceName);

  hndl->v4l2FileHndl = fd;

  if (fd < 0)
  {
    OSA_ERROR("Could not open capture device\n");
    goto error_exit;
  }
  
  // Bypassing V4L2 drivers,and directly configuring CCDC for TVP5158
  tvp5158_ccdc_params.pix_fmt      = CCDC_PIXFMT_YCBCR_8BIT;
  tvp5158_ccdc_params.frm_fmt      = CCDC_FRMFMT_PROGRESSIVE;
  tvp5158_ccdc_params.win.left     = 0;
  tvp5158_ccdc_params.win.top      = 0;

  tvp5158_ccdc_params.win.width    = hndl->v4l2FrameInfo.width;
  tvp5158_ccdc_params.win.height   = hndl->v4l2FrameInfo.height;

  tvp5158_ccdc_params.fid_pol      = CCDC_PINPOL_POSITIVE;
  tvp5158_ccdc_params.vd_pol       = CCDC_PINPOL_NEGATIVE;
  tvp5158_ccdc_params.hd_pol       = CCDC_PINPOL_POSITIVE;
  tvp5158_ccdc_params.bt656_enable = TRUE;
  tvp5158_ccdc_params.pix_order    = CCDC_PIXORDER_CBYCRY;
  tvp5158_ccdc_params.buf_type     = CCDC_BUFTYPE_FLD_SEPARATED;  

  if (ioctl(fd, VPFE_CMD_CONFIG_CCDC_YCBCR, &tvp5158_ccdc_params) == -1)
  {
    OSA_ERROR("Error configuring CCDC for YCBCR\n");
    goto error_exit;
  }

  // request input buffers
  bufreq.count  = MCVIP_V4L2_INPUT_BUF_MAX;
  bufreq.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  bufreq.memory = V4L2_MEMORY_MMAP;

  if (ioctl(fd, VIDIOC_REQBUFS, &bufreq) == -1)
  {
    OSA_ERROR("Error allocating output frame buffers\n");
    goto error_exit;
  }

  // check that the buffers were allocated properly
  if (bufreq.count < (MCVIP_V4L2_INPUT_BUF_MAX))
  {
    OSA_ERROR("Could not allocate enough input frame buffers\n");
    goto error_exit;
  }

  buf_count   = MCVIP_V4L2_INPUT_BUF_MAX;
  buffer_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  
  status = MCVIP_v4l2SetupBufferTable(fd,  buf_count, &hndl->v4l2CaptureBufVirtAddr[0], &buffer_size, buffer_type);

  if(status!=OSA_SOK)
  {
      goto error_exit;
  }
  
  // queue the buffers
  memset(&buf, 0, sizeof(buf));
  buf.type = buffer_type;
  buf.memory = V4L2_MEMORY_MMAP;

  for (i = 0; i < buf_count; i++)
  {
    buf.index = i;

    if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
    {
      OSA_ERROR("Failed to queue initial set of capture buffers\n");
      goto error_exit;
    }
  }
  
  
  for(i=0; i<buf_count; i++) 
  {
    hndl->v4l2CaptureBufPhysAddr[i] = (Uint8*)DRV_dmaGetPhysAddr( (unsigned long)hndl->v4l2CaptureBufVirtAddr[i] );

    #ifdef MCVIP_CLEAR_BUF_DURING_INIT    
    {
      MCVIP_DmaPrm dmaPrm;
      
      // clear buffer with blank data
      dmaPrm.srcPhysAddr = NULL;
      dmaPrm.srcVirtAddr = NULL;    
      dmaPrm.dstPhysAddr = hndl->v4l2CaptureBufPhysAddr[i];
      dmaPrm.dstVirtAddr = hndl->v4l2CaptureBufVirtAddr[i];
      dmaPrm.srcOffsetH  = 0;   
      dmaPrm.srcOffsetV  = 0;  
      dmaPrm.dstOffsetH  = hndl->v4l2FrameInfo.offsetH;   
      dmaPrm.dstOffsetV  = hndl->v4l2FrameInfo.offsetV;   
      dmaPrm.copyWidth   = hndl->v4l2FrameInfo.width  ;   
      dmaPrm.copyHeight  = hndl->v4l2FrameInfo.height ;   
      dmaPrm.fillValueY  = 0x00000000;
      dmaPrm.fillValueC  = 0x00000000;    
      
      MCVIP_dmaRun(&dmaPrm);
    }
    #endif
  }
  
  MCVIP_tvp5158Setup(hndl);

  return OSA_SOK;

error_exit:

  if(fd>=0)
    close(fd);
    
  return OSA_EFAIL;  
}

int MCVIP_v4l2Delete(MCVIP_Hndl *hndl)
{
  struct v4l2_buffer buf;
  int i;
  for (i = 0; i < MCVIP_V4L2_INPUT_BUF_MAX; i++)
  {
     memset(&buf, 0, sizeof(buf));
     buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
     buf.memory = V4L2_MEMORY_MMAP;
     buf.index = i;
     if (ioctl(hndl->v4l2FileHndl, VIDIOC_QUERYBUF, &buf) == -1)
     {
       OSA_ERROR("Could not query buffer %d\n", i);
       return OSA_EFAIL;
     }

     if ( -1 == munmap( hndl->v4l2CaptureBufVirtAddr[i], buf.length )  )
     {
         OSA_ERROR("munmap error \n");
         return OSA_EFAIL;
     }
  }

  if (close(hndl->v4l2FileHndl))
  {
    OSA_ERROR("Error closing capture device\n");
    return OSA_EFAIL;
  }
  
  return OSA_SOK;
}

int MCVIP_v4l2Start(MCVIP_Hndl *hndl)
{
  int stream_param;  
  // start streaming
  MCVIP_tvp5158Start(hndl);

  memset(&hndl->v4l2CaptureBufCur, 0, sizeof(hndl->v4l2CaptureBufCur));
  hndl->v4l2CaptureBufCur.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  hndl->v4l2CaptureBufCur.memory = V4L2_MEMORY_MMAP;

  stream_param = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (ioctl(hndl->v4l2FileHndl, VIDIOC_STREAMON, &stream_param) == -1)
  {
    OSA_ERROR("Failed to begin streaming\n");
    return OSA_EFAIL;
  }

  return OSA_SOK;
}

int MCVIP_v4l2Stop(MCVIP_Hndl *hndl)
{
    int stream_param;

    stream_param = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    // stop streaming
    if (ioctl(hndl->v4l2FileHndl, VIDIOC_STREAMOFF, &stream_param) == -1)
    {
        OSA_ERROR("Failed to stop streaming\n");
        return OSA_EFAIL;
    }

    MCVIP_tvp5158Stop(hndl);

    return OSA_SOK;
}

int MCVIP_v4l2GetBuf(MCVIP_Hndl *hndl, MCVIP_V4l2Buf *buf)
{
    if (ioctl(hndl->v4l2FileHndl, VIDIOC_DQBUF, &hndl->v4l2CaptureBufCur) == -1)
    {
        OSA_ERROR("Could not dequeue capture buffer\n");
        return OSA_EFAIL;
    }

    buf->id = hndl->v4l2CaptureBufCur.index;
    buf->virtAddr     = hndl->v4l2CaptureBufVirtAddr[buf->id];
    buf->physAddr  = hndl->v4l2CaptureBufPhysAddr[buf->id];
    buf->timestamp = 0;
    return OSA_SOK;
}

int MCVIP_v4l2PutBuf(MCVIP_Hndl *hndl, MCVIP_V4l2Buf *buf)
{
    hndl->v4l2CaptureBufCur.index = buf->id;
    if (ioctl(hndl->v4l2FileHndl, VIDIOC_QBUF, &hndl->v4l2CaptureBufCur) == -1)
    {
        OSA_ERROR("Could not queue frame\n");
        return OSA_EFAIL;
    }

    return OSA_SOK;
}

int  MCVIP_v4l2SetupBufferTable(int fd, int buf_count, Uint8** buffers, int *buffer_size, int buffer_type)
{
  struct v4l2_buffer buf;
  int i;
  
  // map physical buffers to physical memory and update buffer table
  for (i = 0; i < buf_count; i++)
  {
    memset(&buf, 0, sizeof(buf));

    buf.type   = buffer_type;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index  = i; //NOTE: there is a direct mapping between the index of a
                   //buffer in v4l2, and the index into the "buffers" pointer

    if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1)
    {
      OSA_ERROR("Could not query buffer %d\n", i);
      return OSA_EFAIL;
    }

    *buffer_size = buf.length;

    buffers[i] = (Uint8*)mmap(NULL,
                              buf.length,
                              PROT_READ | PROT_WRITE,
                              MAP_SHARED,
                              fd,
                              buf.m.offset);

    if( buffers[i] == MAP_FAILED )
    {
      OSA_ERROR("Memory map failed for buffer %d\n", i);
      return OSA_EFAIL;
    }
  }
  return OSA_SOK;
}

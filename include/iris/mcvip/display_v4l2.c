/*
 * display_v4l2.c
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

#include <display.h>
#include <display_v4l2.h>
#include <capture_priv.h>

extern CAPTURE_Ctrl gCAPTURE_ctrl;
DISPLAY_V4l2Ctrl gDISPLAY_v4l2Ctrl = { 0 };

int  DISPLAY_v4l2SetupBufferTable(int fd, int buf_count, Uint8** buffers, int *buffer_size, int buffer_type)
{
    struct v4l2_buffer buf = { 0 };
    int i = 0;
    // map physical buffers to physical memory and update buffer table
    for (i = 0; i < buf_count; i++)
    {
        memset(&buf, 0, sizeof(buf));
        buf.type = buffer_type;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i; //NOTE: there is a direct mapping between the index of a
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


int DISPLAY_SetSysFsAttrib(char *attribute, char *value)
{
    int sysfd = -1;
    char init_val[32];
    char attrib_tag[128];

    bzero(init_val, sizeof(init_val));
    strcpy(attrib_tag, "/sys/class/davinci_display/");
    strcat(attrib_tag, attribute);

    sysfd = open(attrib_tag, O_RDWR);
    if (!sysfd) {
        OSA_ERROR("cannot open %s\n", attrib_tag);
        return OSA_EFAIL;
    }

#ifdef DISPLAY_DEBUG
    OSA_printf(" DISPLAY: %s was opened successfully\n", attrib_tag);
    read(sysfd, init_val, 32);
    lseek(sysfd, 0, SEEK_SET);
    OSA_printf(" DISPLAY: Current %s = %s\n", attribute, init_val);
#endif

    write(sysfd, value, 1 + strlen(value));

#ifdef DISPLAY_DEBUG
    lseek(sysfd, 0, SEEK_SET);
    memset(init_val, '\0', 32);
    read(sysfd, init_val, 32);
    lseek(sysfd, 0, SEEK_SET);
    OSA_printf(" DISPLAY: New    %s = %s\n", attribute, init_val);
#endif

    close(sysfd);
    return OSA_SOK;
}


int DISPLAY_v4l2Create(int mode, DISPLAY_FrameInfo *info)
{
    /* ioctl input parameters */
    int fd = -1,fd_osd0 = -1,fd_osd1 = -1;
    struct v4l2_capability     cap;
    struct v4l2_format         fmt;
    struct v4l2_requestbuffers bufreq;
    int i = 0;
    struct v4l2_buffer buf;
    Uint8 *buff_ptr;
    char displayOutputToUse[20];
    char displayStdToUse[20];

    if(mode == DISPLAY_MODE_COMPOSITE)
    {
        strcpy(displayOutputToUse, "COMPOSITE");
        if(gCAPTURE_ctrl.info.createPrm.videoSystem == MCVIP_VIDEO_SYSTEM_PAL)
        {
            strcpy(displayStdToUse, "PAL");
        }
        else
        {
            strcpy(displayStdToUse, "NTSC");
        }
    }
    else
    {
        OSA_ERROR("Invalid display mode (%d)\n", mode);
        return OSA_EFAIL;
    }
    OSA_printf(" MCVIP: Display init in progress\n");

#if 0//>>Liush
    DISPLAY_SetSysFsAttrib("/ch0/output", displayOutputToUse);
    DISPLAY_SetSysFsAttrib("/ch0/mode", displayStdToUse);

    /* open osd0, osd1 devices and disable */
    fd_osd0 = open(OSD0_DEVICE, O_RDWR);
    ioctl(fd_osd0, FBIOBLANK, 1);

    fd_osd1 = open(OSD1_DEVICE, O_RDWR);
    ioctl(fd_osd1, FBIOBLANK, 1);
#endif

    // open v4l2 device
    fd = open(VID0_DEVICE, O_RDWR, 0);

    gDISPLAY_v4l2Ctrl.v4l2FileHndl = fd;
    gDISPLAY_v4l2Ctrl.outBufNum = DISPLAY_V4L2_NUM_OUT_BUF;

    if (fd < 0)
    {
        OSA_ERROR("Could not open display device\n");
        goto error_exit;
    }

    // query device capabilities
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1)
    {
        OSA_ERROR("Could not query device capabilities\n");
        goto error_exit;
    }

    // get format parameters
    fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    if (ioctl(fd, VIDIOC_G_FMT, &fmt) == -1)
    {
        OSA_ERROR("Error getting source video format\n");
        goto error_exit;
    }

    info->width       = fmt.fmt.pix.width;
    info->height      = fmt.fmt.pix.height;
    info->lineOffsetH = fmt.fmt.pix.bytesperline;
    info->lineOffsetV = fmt.fmt.pix.height;

    // set the source video format
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
    {
        OSA_ERROR("Error setting source video format\n");
        goto error_exit;
    }

    // request output buffers
    bufreq.count = gDISPLAY_v4l2Ctrl.outBufNum;
    bufreq.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    bufreq.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &bufreq) == -1)
    {
        OSA_ERROR("Error allocating output frame buffers\n");
        goto error_exit;
    }

    // check that the buffers were allocated properly
    if (bufreq.count < gDISPLAY_v4l2Ctrl.outBufNum)
    {
        OSA_ERROR("Could not allocate enough output frame buffers\n");
        goto error_exit;
    }

    gDISPLAY_v4l2Ctrl.outBufNum = bufreq.count;

    // set up buffer table
    if (DISPLAY_v4l2SetupBufferTable(fd,
                                     gDISPLAY_v4l2Ctrl.outBufNum,
                                     &gDISPLAY_v4l2Ctrl.outBufVirtAddr[0],
                                     &gDISPLAY_v4l2Ctrl.outBufSize,
                                     V4L2_BUF_TYPE_VIDEO_OUTPUT))
    {
        OSA_ERROR("Failed to set up buffer table\n");
        goto error_exit;
    }
    else
    {
        MCVIP_DmaPrm dmaPrm = { 0 };
        dmaPrm.copyWidth = info->width;
        dmaPrm.copyHeight = info->height;
        dmaPrm.dstOffsetH = info->lineOffsetH;
        dmaPrm.dstOffsetV = info->lineOffsetV;
        dmaPrm.srcOffsetH = 0;
        dmaPrm.srcOffsetV = 0;
        dmaPrm.srcPhysAddr = NULL; // fill dst with 'blank' data
        dmaPrm.srcVirtAddr = NULL; // fill dst with 'blank' data
        dmaPrm.dstPhysAddr = NULL;
        dmaPrm.dstVirtAddr = NULL;
        dmaPrm.fillValueY  = DISPLAY_COLOR_BACKGROUND_Y;
        dmaPrm.fillValueC  = DISPLAY_COLOR_BACKGROUND_C;
        // fill buffers with video data (fill with yellow)
        for (i = 0; i < gDISPLAY_v4l2Ctrl.outBufNum; i++)
        {
            buff_ptr = gDISPLAY_v4l2Ctrl.outBufVirtAddr[i];
            dmaPrm.dstVirtAddr = buff_ptr;
            //clear the buffer
            MCVIP_dmaRun(&dmaPrm);
        }
    }
    for (i = 0; i < gDISPLAY_v4l2Ctrl.outBufNum; i++)
    {
        memset(&buf, 0, sizeof(buf));
        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.bytesused = gDISPLAY_v4l2Ctrl.outBufSize;
        buf.field = V4L2_FIELD_NONE;

        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
        {
            OSA_ERROR("Could not queue buffers\n");
            goto error_exit;
        }
    }
    OSA_printf(" MCVIP: Display init done !!!\n");
    return OSA_SOK;

 error_exit:
    if( fd >= 0 )
    {
        close(fd);
    }
    return OSA_EFAIL;
}

int DISPLAY_v4l2Delete()
{
    struct v4l2_buffer buf;
    int i;
    for (i = 0; i < gDISPLAY_v4l2Ctrl.outBufNum; ++i )
    {
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i; //NOTE: there is a direct mapping between the index of a
        //buffer in v4l2, and the index into the "buffers" pointer

        if (ioctl(gDISPLAY_v4l2Ctrl.v4l2FileHndl, VIDIOC_QUERYBUF, &buf) == -1)
        {
            OSA_ERROR("Could not query buffer %d\n", i);
            return OSA_EFAIL;
        }

        if ( -1 == munmap( gDISPLAY_v4l2Ctrl.outBufVirtAddr[i],buf.length ) )
        {
            OSA_ERROR("Error closing display device munmap error\n");
            return OSA_EFAIL;
        }
    }

    if (close( gDISPLAY_v4l2Ctrl.v4l2FileHndl ))
    {
        OSA_ERROR("Error closing display device\n");
        return OSA_EFAIL;
    }

    return OSA_SOK;
}

int DISPLAY_v4l2GetBuf(DISPLAY_BufInfo *buf)
{
    if (ioctl(gDISPLAY_v4l2Ctrl.v4l2FileHndl, VIDIOC_DQBUF, &gDISPLAY_v4l2Ctrl.v4l2DisplayBufCur) == -1)
    {
        OSA_ERROR("Could not dequeue capture buffer\n");
        return OSA_EFAIL;
    }

    buf->id = gDISPLAY_v4l2Ctrl.v4l2DisplayBufCur.index;
    buf->virtAddr = gDISPLAY_v4l2Ctrl.outBufVirtAddr[buf->id];
    buf->physAddr = (Uint8*)NULL;

    return OSA_SOK;
}

int DISPLAY_v4l2PutBuf(DISPLAY_BufInfo *buf)
{
    gDISPLAY_v4l2Ctrl.v4l2DisplayBufCur.index = buf->id;

    if (ioctl(gDISPLAY_v4l2Ctrl.v4l2FileHndl, VIDIOC_QBUF, &gDISPLAY_v4l2Ctrl.v4l2DisplayBufCur) == -1)
    {
        OSA_ERROR("Could not queue frame\n");
        return OSA_EFAIL;
    }

    return OSA_SOK;
}

int DISPLAY_v4l2Start( )
{
    int stream_param = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    // start streaming
    memset(&gDISPLAY_v4l2Ctrl.v4l2DisplayBufCur, 0, sizeof(gDISPLAY_v4l2Ctrl.v4l2DisplayBufCur));
    gDISPLAY_v4l2Ctrl.v4l2DisplayBufCur.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

    if( ioctl( gDISPLAY_v4l2Ctrl.v4l2FileHndl, VIDIOC_STREAMON, &stream_param ) == -1 )
    {
        OSA_ERROR("Could not start display\n");
        return OSA_EFAIL;
    }
    return OSA_SOK;
}

int DISPLAY_v4l2Stop( )
{
    int stream_param = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    if ( ioctl(gDISPLAY_v4l2Ctrl.v4l2FileHndl, VIDIOC_STREAMOFF, &stream_param) == -1 )
    {
        OSA_ERROR("Failed to stop streaming\n");
        return OSA_EFAIL;
    }
    OSA_printf( "DISPLAY_v4l2Stop OK\n" )
    return OSA_SOK;
}


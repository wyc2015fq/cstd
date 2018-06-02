#include <display.h>
#include <display_v4l2.h>
#include <capture_priv.h>
#include <mcvip_priv.h>
#include <system.h>
#include <osa_cmem.h>
#include <mcvip.h>
#include "display_priv.h"
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev.h>
#include <linux/videodev2.h>
#include <linux/fb.h>
#include <pthread.h>

typedef struct {

    Uint8 *outBufVirtAddr[DISPLAY_V4L2_NUM_OUT_BUF];

    int outBufNum;
    int outBufSize;

    int v4l2FileHndl;
    struct v4l2_buffer v4l2DisplayBufCur;

} DISPLAY_V4l2Ctrl;


#define false 0
int  MCVIP_v4l2SetupBufferTable(int fd, int buf_count, Uint8** buffers, int *buffer_size, int buffer_type);
int  DISPLAY_v4l2SetupBufferTable(int fd, int buf_count, Uint8** buffers, int *buffer_size, int buffer_type);

void *CaptureV4l2Create(void *args);
void *DisplayV4l2Create(void *args);



int MCVIP_v4l2Start(MCVIP_Hndl *hndl);
int MCVIP_v4l2PutBuf(MCVIP_Hndl *hndl, MCVIP_V4l2Buf *buf);
int MCVIP_v4l2GetBuf(MCVIP_Hndl *hndl, MCVIP_V4l2Buf *buf);

int OSA_fileWriteFile(char *fileName, Uint8 *addr, Uint32 size);
void lshMain()
{
    pthread_t pid;
    //pthread_create(&pid,NULL,CaptureV4l2Create,NULL);
    DisplayV4l2Create(NULL);
}

void *CaptureV4l2Create(void *args)
{
    int status;
    MCVIP_Hndl hndl={0};

    MCVIP_V4l2Buf v4l2Buf;

    int                        fd=-1;
    struct v4l2_requestbuffers bufreq={0};
    struct v4l2_buffer         buf={0};

    int buf_count, buffer_type, buffer_size;
    int i;
    ccdc_params_ycbcr tvp5158_ccdc_params={0};

    int width = 728;
    int height = 1250;

    hndl.v4l2FrameInfo.width   = width;
    hndl.v4l2FrameInfo.height  = height;
    hndl.v4l2FrameInfo.offsetH = OSA_align(hndl.v4l2FrameInfo.width, 16);
    hndl.v4l2FrameInfo.offsetV = hndl.v4l2FrameInfo.height;

    printf("hndl.v4l2FrameInfo.offsetH == %d , "
           "hndl.v4l2FrameInfo.offsetV == %d \r\n"
           ,hndl.v4l2FrameInfo.offsetH
           ,hndl.v4l2FrameInfo.offsetV);

    fd = open("/dev/video0", O_RDWR, 0);
    hndl.v4l2FileHndl = fd;
    if (fd < 0)
    {
        assert(false);
    }

    tvp5158_ccdc_params.pix_fmt      = CCDC_PIXFMT_YCBCR_8BIT;
    tvp5158_ccdc_params.frm_fmt      = CCDC_FRMFMT_PROGRESSIVE;
    tvp5158_ccdc_params.win.left     = 0;
    tvp5158_ccdc_params.win.top      = 0;
    tvp5158_ccdc_params.win.width    = hndl.v4l2FrameInfo.width;
    tvp5158_ccdc_params.win.height   = hndl.v4l2FrameInfo.height ;
    tvp5158_ccdc_params.fid_pol      = CCDC_PINPOL_POSITIVE;
    tvp5158_ccdc_params.vd_pol       = CCDC_PINPOL_NEGATIVE;
    tvp5158_ccdc_params.hd_pol       = CCDC_PINPOL_POSITIVE;
    tvp5158_ccdc_params.bt656_enable = TRUE;
    tvp5158_ccdc_params.pix_order    = CCDC_PIXORDER_CBYCRY;
    tvp5158_ccdc_params.buf_type     = CCDC_BUFTYPE_FLD_SEPARATED;

    if (ioctl(fd, VPFE_CMD_CONFIG_CCDC_YCBCR, &tvp5158_ccdc_params) == -1)
    {
        assert(false);
    }
    TEST;

    // request input buffers
    bufreq.count  = MCVIP_V4L2_INPUT_BUF_MAX;
    bufreq.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufreq.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &bufreq) == -1)
    {
        assert(false);
    }
    TEST;
    // check that the buffers were allocated properly
    if (bufreq.count < (MCVIP_V4L2_INPUT_BUF_MAX))
    {
        assert(false);
    }

    buf_count = MCVIP_V4L2_INPUT_BUF_MAX;
    buffer_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    status = MCVIP_v4l2SetupBufferTable(fd,  buf_count, &hndl.v4l2CaptureBufVirtAddr[0], &buffer_size, buffer_type);
    if(status!=OSA_SOK)
    {
        assert(false);
    }
    TEST;

    memset(&buf, 0, sizeof(buf));
    buf.type = buffer_type;
    buf.memory = V4L2_MEMORY_MMAP;

    for (i = 0; i < buf_count; i++)
    {
        buf.index = i;
        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
        {
            assert(false);
        }
    }
    TEST;

    for(i=0; i<buf_count; i++)
    {
        hndl.v4l2CaptureBufPhysAddr[i] = (Uint8*)DRV_dmaGetPhysAddr( (unsigned long)hndl.v4l2CaptureBufVirtAddr[i] );

#ifdef MCVIP_CLEAR_BUF_DURING_INIT
        {
            MCVIP_DmaPrm dmaPrm;

            // clear buffer with blank data
            dmaPrm.srcPhysAddr = NULL;
            dmaPrm.srcVirtAddr = NULL;
            dmaPrm.dstPhysAddr = hndl.v4l2CaptureBufPhysAddr[i];
            dmaPrm.dstVirtAddr = hndl.v4l2CaptureBufVirtAddr[i];
            dmaPrm.srcOffsetH  = 0;
            dmaPrm.srcOffsetV  = 0;
            dmaPrm.dstOffsetH  = hndl.v4l2FrameInfo.offsetH;
            dmaPrm.dstOffsetV  = hndl.v4l2FrameInfo.offsetV;
            dmaPrm.copyWidth   = hndl.v4l2FrameInfo.width  ;
            dmaPrm.copyHeight  = hndl.v4l2FrameInfo.height ;
            dmaPrm.fillValueY  = 0x00000000;
            dmaPrm.fillValueC  = 0x00000000;

            MCVIP_dmaRun(&dmaPrm);
        }
#endif
    }
    TEST;

    MCVIP_v4l2Start(&hndl);

    TEST;
    while(1)
    {
        MCVIP_v4l2GetBuf(&hndl,&v4l2Buf);
        static int n  = 0;
        ++n;
        if ( (n % 60) == 0 )
        {
            char fileName[30]={0};
            sprintf(fileName,"test%d.data",n/60);
            int fileSize = hndl.v4l2FrameInfo.offsetH * hndl.v4l2FrameInfo.offsetV * 2;
            OSA_fileWriteFile(fileName,v4l2Buf.virtAddr,fileSize);
        }

        MCVIP_v4l2PutBuf(&hndl,&v4l2Buf);
    }
    TEST;
    close(fd);
}

int DISPLAY_SetSysFsAttrib(char *attribute, char *value);
int DISPLAY_v4l2GetBufDDD(DISPLAY_V4l2Ctrl * DISPLAY_v4l2Ctrl,DISPLAY_BufInfo *buf);
int DISPLAY_getCaptureChIdsDDD(DISPLAY_Ctrl *DISPLAY_ctrl);



void *DisplayV4l2Create(void *args)
{
    DISPLAY_V4l2Ctrl DISPLAY_v4l2Ctrl={0};

    DISPLAY_Ctrl DISPLAY_ctrl={0};
    DISPLAY_ctrl.blankDetectThres = 60;
    memset(&gDISPLAY_ctrl.chInfo, 0, sizeof(gDISPLAY_ctrl.chInfo));
    //gDISPLAY_ctrl.pCaptureInfo = prm->captureInfo;


    DISPLAY_BufInfo displayBufInfo;

    int                        fd,fd_osd0,fd_osd1;
    struct v4l2_capability     cap;
    struct v4l2_format         fmt;
    struct v4l2_requestbuffers bufreq;
    int                        i;
    struct v4l2_buffer buf;
    Uint8 *buff_ptr;
    char displayOutputToUse[20];
    char displayStdToUse[20];

    strcpy(displayStdToUse, "PAL");
    strcpy(displayOutputToUse, "COMPOSITE");

    DISPLAY_SetSysFsAttrib("/ch0/output", displayOutputToUse);
    DISPLAY_SetSysFsAttrib("/ch0/mode", displayStdToUse);
    TEST;
    fd_osd0 = open(OSD0_DEVICE, O_RDWR);
    ioctl(fd_osd0, FBIOBLANK, 1);

    fd_osd1 = open(OSD1_DEVICE, O_RDWR);
    ioctl(fd_osd1, FBIOBLANK, 1);
    TEST;
    fd = open(VID0_DEVICE, O_RDWR, 0);
    if (fd < 0)
    {
        assert(false);
    }
    DISPLAY_v4l2Ctrl.v4l2FileHndl = fd;
    DISPLAY_v4l2Ctrl.outBufNum = DISPLAY_V4L2_NUM_OUT_BUF;

    TEST;
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1)
    {
        assert(false);
    }
    TEST;

    fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    if (ioctl(fd, VIDIOC_G_FMT, &fmt) == -1)
    {
        assert(false);
    }
    TEST;
    // set the source video format
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
    {
        assert(false);
    }
    TEST;
    bufreq.count = DISPLAY_v4l2Ctrl.outBufNum ;
    bufreq.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    bufreq.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &bufreq) == -1)
    {
        assert(false);
    }
    TEST;
    // check that the buffers were allocated properly
    if (bufreq.count < DISPLAY_V4L2_NUM_OUT_BUF )
    {
        assert(false);
    }
    TEST;

    DISPLAY_v4l2Ctrl.outBufNum = bufreq.count;

    // set up buffer table
    if (DISPLAY_v4l2SetupBufferTable(fd,
                                     DISPLAY_v4l2Ctrl.outBufNum,
                                     &DISPLAY_v4l2Ctrl.outBufVirtAddr[0],
                                     &DISPLAY_v4l2Ctrl.outBufSize,
                                     V4L2_BUF_TYPE_VIDEO_OUTPUT))
    {
        assert(false);
    }
TEST;

    {
      MCVIP_DmaPrm dmaPrm;

      dmaPrm.copyWidth = fmt.fmt.pix.width;
      dmaPrm.copyHeight = fmt.fmt.pix.height;

      dmaPrm.dstOffsetH = fmt.fmt.pix.bytesperline;
      dmaPrm.dstOffsetV = fmt.fmt.pix.height;

      dmaPrm.srcOffsetH = 0;
      dmaPrm.srcOffsetV = 0;

      dmaPrm.srcPhysAddr = NULL; // fill dst with 'blank' data
      dmaPrm.srcVirtAddr = NULL; // fill dst with 'blank' data

      dmaPrm.dstPhysAddr = NULL;
      dmaPrm.dstVirtAddr = NULL;
      dmaPrm.fillValueY  = DISPLAY_COLOR_BACKGROUND_Y;
      dmaPrm.fillValueC  = DISPLAY_COLOR_BACKGROUND_C;

      // fill buffers with video data (fill with yellow)
      for (i = 0; i < DISPLAY_v4l2Ctrl.outBufNum; i++)
      {
        buff_ptr = DISPLAY_v4l2Ctrl.outBufVirtAddr[i];

        dmaPrm.dstVirtAddr = buff_ptr;

        //clear the buffer
        MCVIP_dmaRun(&dmaPrm);
      }
    }
TEST;
    for (i = 0; i < DISPLAY_v4l2Ctrl.outBufNum; i++)
    {
      memset(&buf, 0, sizeof(buf));
      buf.index = i;
      buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
      buf.memory = V4L2_MEMORY_MMAP;
      buf.bytesused = DISPLAY_v4l2Ctrl.outBufSize;
      buf.field = V4L2_FIELD_NONE;

      if (ioctl(fd, VIDIOC_QBUF, &buf) == -1)
      {
          assert(false);
      }
    }

    DISPLAY_v4l2StartDDD(&DISPLAY_v4l2Ctrl);

    return NULL;
    TEST;
    while(1)
    {
        DISPLAY_v4l2GetBufDDD(&DISPLAY_v4l2Ctrl,&displayBufInfo);

    }
    TEST;
    close(fd);
}


int DISPLAY_v4l2StartDDD(DISPLAY_V4l2Ctrl * DISPLAY_v4l2Ctrl)
{
  int                stream_param;

  // start streaming

  memset(&DISPLAY_v4l2Ctrl->v4l2DisplayBufCur, 0, sizeof(DISPLAY_v4l2Ctrl->v4l2DisplayBufCur));
  DISPLAY_v4l2Ctrl->v4l2DisplayBufCur.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;

  stream_param = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  if (ioctl(DISPLAY_v4l2Ctrl->v4l2FileHndl, VIDIOC_STREAMON, &stream_param) == -1)
  {
    OSA_ERROR("Could not start display\n");
    return OSA_EFAIL;
  }

  return OSA_SOK;
}

int DISPLAY_v4l2GetBufDDD(DISPLAY_V4l2Ctrl * DISPLAY_v4l2Ctrl,DISPLAY_BufInfo *buf)
{
  if (ioctl(DISPLAY_v4l2Ctrl->v4l2FileHndl, VIDIOC_DQBUF, &DISPLAY_v4l2Ctrl->v4l2DisplayBufCur) == -1)
  {
    assert(false);
  }

  buf->id = DISPLAY_v4l2Ctrl->v4l2DisplayBufCur.index;
  buf->virtAddr = DISPLAY_v4l2Ctrl->outBufVirtAddr[buf->id];
  buf->physAddr = (Uint8*)NULL;

  return OSA_SOK;
}

int DISPLAY_getCaptureChIdsDDD(DISPLAY_Ctrl *DISPLAY_ctrl)
{
  int i, bufId;

  // get newest channel data for display
  for(i=0; i<DISPLAY_ctrl->pCaptureInfo->chList.numCh; i++)
  {
    bufId = -1;
    OSA_queGet(&DISPLAY_ctrl->pCaptureInfo->displayQue[i], &bufId, OSA_TIMEOUT_NONE);

    if(bufId != -1) {

      // got new buffer

      // release current buffer
      if(DISPLAY_ctrl->chInfo[i].curBufId != -1) {
        MCVIP_putBuf( DISPLAY_ctrl->pCaptureInfo->mcvipHndl, DISPLAY_ctrl->chInfo[i].curBufId, MCVIP_FLAG_DISPLAY_DONE);
      }

      DISPLAY_ctrl->chInfo[i].curBufId = bufId;
      DISPLAY_ctrl->chInfo[i].blankCount = 0;

      //OSA_printf(" Display Ch %d: Buf %d\n", i, bufId);

    } else {
      #if 1
      DISPLAY_ctrl->chInfo[i].blankCount++;
      if(DISPLAY_ctrl->chInfo[i].blankCount >= DISPLAY_ctrl->blankDetectThres)
      {
        // no video detected, release current buffer, show blank video in display

        DISPLAY_ctrl->chInfo[i].blankCount = DISPLAY_ctrl->blankDetectThres;

        if(DISPLAY_ctrl->chInfo[i].curBufId != -1) {
          MCVIP_putBuf( DISPLAY_ctrl->pCaptureInfo->mcvipHndl, DISPLAY_ctrl->chInfo[i].curBufId, MCVIP_FLAG_DISPLAY_DONE);
        }

        DISPLAY_ctrl->chInfo[i].curBufId = -1;
      }
      #endif
    }
  }

  return OSA_SOK;
}

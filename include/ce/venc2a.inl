#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/video2/viddec2.h>
#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/BufTab.h>
#include <ti/sdo/dmai/ColorSpace.h>
//#include <ti/sdo/dmai/ce/Vdec2a.h>
#include <ti/sdo/dmai/Buffer.h>
#include <ti/sdo/dmai/BufferGfx.h>
#define MODULE_NAME     "Vdec2a"
/* As 0 is not a valid buffer id in XDM 1.0, we need macros for easy access */
#define GETID(x)  ((x) + 1)
#define GETIDX(x) ((x) - 1)
typedef struct Vdec2a_Object {
  VIDDEC2_Handle          hDecode;
  Int32                   minNumInBufs;
  Int32                   minInBufSize[IVIDDEC2_MAX_IO_BUFFERS];
  Int32                   minNumOutBufs;
  Int32                   minOutBufSize[IVIDDEC2_MAX_IO_BUFFERS];
  Int32                   maxNumDisplayBufs;
  int                     freeBufIdx;
  int                     displayBufIdx;
  VIDDEC2_DynamicParams   dynParams;
} Vdec2a_Object;
typedef struct Vdec2a_Object* Vdec2a_Handle;
Vdec2a_Handle hVd2 = NULL;
const VIDDEC2_Params Vdec2a_Params_DEFAULT = {
  sizeof(VIDDEC2_Params),             /* size */
  576,                                /* maxHeight */
  720,                                /* maxWidth */
  30000,                              /* maxFrameRate */
  6000000,                            /* maxBitRate */
  XDM_BYTE,                           /* dataEndianess */
  XDM_YUV_420P,                       /* forceChromaFormat */
};
const VIDDEC2_DynamicParams Vdec2a_DynamicParams_DEFAULT = {
  sizeof(VIDDEC2_DynamicParams),      /* size */
  XDM_DECODE_AU,                      /* decodeHeader */
  0,                                  /* displayWidth */
  IVIDEO_NO_SKIP,                     /* frameSkipMode */
  IVIDDEC2_DISPLAY_ORDER,             /* frameOrder */
  0,                                  /* newFrameFlag */
  0,                                  /* mbDataFlag */
};
/******************************************************************************
 * Vdec2a_flush
 ******************************************************************************/
int Vdec2a_flush(Vdec2a_Handle hVd)
{
  VIDDEC2_Status         decStatus;
  XDAS_Int32             status;
  assert(hVd);
  decStatus.data.buf = NULL;
  decStatus.size = sizeof(VIDDEC2_Status);
  /* Flush the codec */
  status = VIDDEC2_control(hVd->hDecode, XDM_FLUSH, &hVd->dynParams, &decStatus);

  if (status != VIDDEC2_EOK) {
    Dmai_err0("XDM_GETBUFINFO control failed\n");
    return Dmai_EFAIL;
  }

  return Dmai_EOK;
}
#define BUFFER_GETSIZE(x)  x##Sz
#define BUFFER_GETNUMBYTESUSED(x)  x##Sz
#define BUFFER_GETUSERPTR(x)  x
/******************************************************************************
 * Vdec2a_process
******************************************************************************/
int aaaID = 1;
int Vdec2a_process(const char* hInBuf, int hInBufSz, char* hDstBuf, int hDstBufSz)
{
  Vdec2a_Handle           hVd = hVd2;
  VIDDEC2_InArgs          inArgs;
  VIDDEC2_OutArgs         outArgs;
  XDM1_BufDesc            inBufDesc;
  XDM_BufDesc             outBufDesc;
  XDAS_Int32              outBufSizeArray[XDM_MAX_IO_BUFFERS];
  XDAS_Int8*              outBufPtrArray[XDM_MAX_IO_BUFFERS];
  XDAS_Int32              status;
  int                     bufIdx;
  BufferGfx_Dimensions    dim;
  XDAS_Int8*              inPtr, *dstPtr;
  int                     ret = Dmai_EOK;
  UInt32                  offset = 0;
  UInt32                  bpp;
  int nColorSpace = ColorSpace_UYVY;
  assert(hVd);
  assert(hInBuf);
  assert(hDstBuf);
  assert(hInBufSz);
  assert(hDstBufSz);
  offset = 0;
  assert(offset < BUFFER_GETSIZE(hDstBuf));
  assert(hVd->minOutBufSize[0] < BUFFER_GETSIZE(hDstBuf));
  inPtr  = BUFFER_GETUSERPTR(hInBuf);
  dstPtr = BUFFER_GETUSERPTR(hDstBuf) + offset;

  if (nColorSpace == ColorSpace_YUV420PSEMI) {
    outBufPtrArray[0]       = dstPtr;
    outBufSizeArray[0]      = hVd->minOutBufSize[0];
    outBufPtrArray[1]       = dstPtr + BUFFER_GETSIZE(hDstBuf) * 2 / 3;
    outBufSizeArray[1]      = hVd->minOutBufSize[1];
  }
  else if (nColorSpace == ColorSpace_YUV420P) {
    outBufPtrArray[0]       = dstPtr;
    outBufSizeArray[0]      = hVd->minOutBufSize[0];
    outBufPtrArray[1]       = dstPtr + BUFFER_GETSIZE(hDstBuf) * 2 / 3;
    outBufSizeArray[1]      = hVd->minOutBufSize[1];
    outBufPtrArray[2]       = dstPtr + BUFFER_GETSIZE(hDstBuf) * 5 / 6;
    outBufSizeArray[2]      = hVd->minOutBufSize[2];
  }
  else if (nColorSpace == ColorSpace_UYVY) {
    outBufPtrArray[0]       = dstPtr;
    outBufSizeArray[0]      = hVd->minOutBufSize[0];
    outBufSizeArray[0]      = BUFFER_GETSIZE(hDstBuf);
    //printf("minNumOutBufs=%d\n", hVd->minNumOutBufs);
    //printf("outBufSizeArray[0]=%d\n", outBufSizeArray[0]);
  }
  else {
    Dmai_err0("Unsupported color format of destination buffer\n");
    return Dmai_EINVAL;
  }

  outBufDesc.numBufs          = hVd->minNumOutBufs;
  outBufDesc.bufSizes         = outBufSizeArray;
  outBufDesc.bufs             = outBufPtrArray;
  /* One buffer with encoded data */
  inBufDesc.numBufs           = 1;
  inBufDesc.descs[0].buf      = inPtr;
  inBufDesc.descs[0].bufSize  = BUFFER_GETNUMBYTESUSED(hInBuf);
  inArgs.size                 = sizeof(VIDDEC2_InArgs);
  inArgs.numBytes             = BUFFER_GETNUMBYTESUSED(hInBuf);
  inArgs.inputID              = aaaID;//GETID(Buffer_getId(hDstBuf));
  aaaID++;
  printf("inputID=%d\n", inArgs.inputID);
  outArgs.size                = sizeof(VIDDEC2_OutArgs);
  /* Decode video buffer */
#if 1
  status = VIDDEC2_process(hVd->hDecode, &inBufDesc, &outBufDesc, &inArgs, &outArgs);
#else
  status = VIDDEC2_EOK;
  //status = VIDDEC2_processAsync(hVd->hDecode, &inBufDesc, &outBufDesc, &inArgs, &outArgs);
  status = VIDDEC2_processWait(hVd->hDecode, &inBufDesc, &outBufDesc, &inArgs, &outArgs, 1000);
#endif
  //Vdec2a_flush(hVd);
  printf("outArgs.outputID[0]=%d\n", outArgs.outputID[0]);

  //Buffer_setNumBytesUsed(hInBuf, outArgs.bytesConsumed);
  //DBG("VIDDEC2_process() ret %d inId %d inUse %d consumed %d\n", status, 0, outArgs.outBufsInUseFlag, outArgs.bytesConsumed);
  for (bufIdx = 0;
      bufIdx < IVIDDEC2_MAX_IO_BUFFERS && outArgs.outputID[bufIdx] > 0;
      //bufIdx < 1;
      bufIdx++) {
    printf("GETIDX(outArgs.outputID[%d]) = %d\n", bufIdx, GETIDX(outArgs.outputID[bufIdx]));
    dim.width = outArgs.displayBufs[bufIdx].frameWidth;
    dim.height = outArgs.displayBufs[bufIdx].frameHeight;
    dim.lineLength = outArgs.displayBufs[bufIdx].framePitch;
    printf("outArgs.displayBufs[%d].frameWidth = %d\n", bufIdx, outArgs.displayBufs[bufIdx].frameWidth);
    printf("outArgs.displayBufs[%d].frameHeight = %d\n", bufIdx, outArgs.displayBufs[bufIdx].frameHeight);
    printf("outArgs.displayBufs[%d].framePitch = %d\n", bufIdx, outArgs.displayBufs[bufIdx].framePitch);
    printf("outArgs.displayBufs[%d].frameType = %d\n", bufIdx, outArgs.displayBufs[bufIdx].frameType);
    /* Is there an offset to where we are supposed to start displaying? */
    printf("outArgs.displayBufs[%d].bufDesc[0].buf = %x\n", bufIdx, outArgs.displayBufs[bufIdx].bufDesc[0].buf);
    //dim.y = offset / dim.lineLength;
    //dim.x = offset - ((dim.y * dim.lineLength) >> (bpp >> 4));
  }

  if (status != VIDDEC2_EOK) {
    if (XDM_ISFATALERROR(outArgs.decodedBufs.extendedError)) {
      DBG("VIDDEC2_process() failed with error (%d ext: 0x%x)\n",
          (int)status, (Uns) outArgs.decodedBufs.extendedError);
      return Dmai_EFAIL;
    }
    else {
      DBG("VIDDEC2_process() non-fatal error 0x%x\n",
          (Uns) outArgs.decodedBufs.extendedError);
      ret = Dmai_EBITERROR;
    }
  }

  hVd->displayBufIdx = 0;
  hVd->freeBufIdx = 0;

  /* Was this just the first field of an interlaced frame? */
  if (outArgs.outBufsInUseFlag) {
    ret = Dmai_EFIRSTFIELD;
  }

  ret = outArgs.bytesConsumed;
  return ret;
}
/******************************************************************************
 * Vdec2a_getMinOutBufs
 ******************************************************************************/
Int32 Vdec2a_getMinOutBufs(Vdec2a_Handle hVd)
{
  VIDDEC2_Status  decStatus;
  XDAS_Int32      status;
  assert(hVd);
  decStatus.data.buf = NULL;
  decStatus.size = sizeof(VIDDEC2_Status);
  /* Get buffer information from video decoder */
  decStatus.maxNumDisplayBufs = 0;
  status = VIDDEC2_control(hVd->hDecode, XDM_GETBUFINFO,
      &hVd->dynParams, &decStatus);

  if (status != VIDDEC2_EOK) {
    DBG("XDM_GETBUFINFO control failed\n");
    return Dmai_EFAIL;
  }

  DBG("Made XDM_GETBUFINFO control call\n");
  memcpy(hVd->minInBufSize,
      decStatus.bufInfo.minInBufSize, sizeof(hVd->minInBufSize));
  hVd->minNumInBufs = decStatus.bufInfo.minNumInBufs;
  memcpy(hVd->minOutBufSize,
      decStatus.bufInfo.minOutBufSize, sizeof(hVd->minOutBufSize));
  hVd->minNumOutBufs = decStatus.bufInfo.minNumOutBufs;
  hVd->maxNumDisplayBufs = decStatus.maxNumDisplayBufs;
  return decStatus.maxNumDisplayBufs;
}
/******************************************************************************
 * Vdec2a_create
 ******************************************************************************/
int Vdec2a_create(int h, int w)
{
  Vdec2a_Handle           hVd;
  VIDDEC2_Handle         hDecode;
  VIDDEC2_Status         decStatus;
  XDAS_Int32             status;
  VIDDEC2_Params          defaultParams       = Vdec2a_Params_DEFAULT;
  VIDDEC2_DynamicParams   defaultDynParams    = Vdec2a_DynamicParams_DEFAULT;
  VIDDEC2_Params*         params;
  VIDDEC2_DynamicParams*  dynParams;
  Char* codecName = "h264dec";
  params = &defaultParams;
  dynParams = &defaultDynParams;
  /* Allow maximum 720p sized clips */
  params->maxWidth = VideoStd_720P_WIDTH;
  params->maxHeight = VideoStd_720P_HEIGHT;
  params->forceChromaFormat = XDM_YUV_422ILE;
  params->maxWidth = w;
  params->maxHeight = h;
  ceruntime_init();
  hEngine = (Engine_Handle)engine_open();

  if (hEngine == NULL || codecName == NULL ||
      params == NULL || dynParams == NULL) {
    DBG("Cannot pass null for engine, codec name, params or "
        "dynamic params\n");
    return NULL;
  }

  /* Allocate space for the object */
  hVd = (Vdec2a_Handle)calloc(1, sizeof(Vdec2a_Object));
  hVd2 = hVd;

  if (hVd == NULL) {
    DBG("Failed to allocate space for Vdec2a Object\n");
    return NULL;
  }

  /* Create video decoder instance */
  hDecode = VIDDEC2_create(hEngine, codecName, params);

  if (hDecode == NULL) {
    DBG("Failed to open video decode algorithm\n");
    free(hVd);
    return NULL;
  }

  DBG("Video decoder instance created\n");
  /* Set video decoder dynamic params */
  decStatus.data.buf = NULL;
  decStatus.size = sizeof(VIDDEC2_Status);
  status = VIDDEC2_control(hDecode, XDM_SETPARAMS, dynParams, &decStatus);

  if (status != VIDDEC2_EOK) {
    Dmai_err0("XDM_SETPARAMS control failed\n");
    VIDDEC2_delete(hDecode);
    free(hVd);
    return NULL;
  }

  DBG("Made XDM_SETPARAMS control call\n");
  hVd->dynParams = *dynParams;
  hVd->hDecode = hDecode;
  Vdec2a_getMinOutBufs(hVd);
  return 1;
}
/******************************************************************************
 * Vdec2a_delete
 ******************************************************************************/
int Vdec2a_delete()
{
  Vdec2a_Handle hVd = hVd2;

  if (hVd) {
    if (hVd->hDecode) {
      VIDDEC2_delete(hVd->hDecode);
    }

    free(hVd);
  }

  return Dmai_EOK;
}
/******************************************************************************
 * Vdec2a_setBufTab
 ******************************************************************************/
Void Vdec2a_setBufTab(Vdec2a_Handle hVd, BufTab_Handle hBufTab)
{
  assert(hVd);
  //hVd->hOutBufTab = hBufTab;
}
/******************************************************************************
 * Vdec2a_getBufTab
 ******************************************************************************/
BufTab_Handle Vdec2a_getBufTab(Vdec2a_Handle hVd)
{
  assert(hVd);
  return 0;//hVd->hOutBufTab;
}
/******************************************************************************
 * Vdec2a_getOutBufSize
 ******************************************************************************/
Int32 Vdec2a_getOutBufSize(Vdec2a_Handle hVd)
{
  Int32 size = 0;
  int bufIdx;
  assert(hVd);

  for (bufIdx = 0; bufIdx < hVd->minNumOutBufs; bufIdx++) {
    size += hVd->minOutBufSize[bufIdx];
  }

  return size;
}
/******************************************************************************
 * Vdec2a_getInBufSize
 ******************************************************************************/
Int32 Vdec2a_getInBufSize(Vdec2a_Handle hVd)
{
  assert(hVd);
  return hVd->minInBufSize[0];
}
/******************************************************************************
 * Vdec2a_getDisplayBuf
 ******************************************************************************/
Buffer_Handle Vdec2a_getDisplayBuf(Vdec2a_Handle hVd)
{
  return NULL;
}
/******************************************************************************
 * Vdec2a_getFreeBuf
 ******************************************************************************/
Buffer_Handle Vdec2a_getFreeBuf(Vdec2a_Handle hVd)
{
  return NULL;
}
/******************************************************************************
 * Vdec2a_getVisaHandle
 ******************************************************************************/
VIDDEC2_Handle Vdec2a_getVisaHandle(Vdec2a_Handle hVd)
{
  assert(hVd);
  return hVd->hDecode;
}


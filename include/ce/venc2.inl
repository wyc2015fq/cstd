#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/video2/viddec2.h>
//#include <ti/sdo/dmai/Dmai.h>
//#include <ti/sdo/dmai/BufTab.h>
#include <ti/sdo/dmai/ColorSpace.h>
//#include <ti/sdo/dmai/ce/vdec2.h>
//#include <ti/sdo/dmai/Buffer.h>
//#include <ti/sdo/dmai/BufferGfx.h>
#define MODULE_NAME     "vdec2"
/* As 0 is not a valid buffer id in XDM 1.0, we need macros for easy access */
#define GETID(x)  ((x) + 1)
#define GETIDX(x) ((x) - 1)
typedef struct vdec2_Object {
  VIDDEC2_Handle          hDecode;
  Int32                   minNumInBufs;
  Int32                   minInBufSize[IVIDDEC2_MAX_IO_BUFFERS];
  Int32                   minNumOutBufs;
  Int32                   minOutBufSize[IVIDDEC2_MAX_IO_BUFFERS];
  Int32                   maxNumDisplayBufs;
  //BufTab_Handle           hOutBufTab;
  //Buffer_Handle           hFreeBufs[IVIDDEC2_MAX_IO_BUFFERS];
  Int                     freeBufIdx;
  //Buffer_Handle           hDisplayBufs[IVIDDEC2_MAX_IO_BUFFERS];
  Int                     displayBufIdx;
  VIDDEC2_DynamicParams   dynParams;
} vdec2_Object;
VIDDEC2_Params vdec2_Params_DEFAULT = {
  sizeof(VIDDEC2_Params),             /* size */
  576,                                /* maxHeight */
  720,                                /* maxWidth */
  20000,                              /* maxFrameRate */
  2000000,                            /* maxBitRate */
  XDM_BYTE,                           /* dataEndianess */
  XDM_YUV_422ILE,                       /* forceChromaFormat */
};
VIDDEC2_DynamicParams vdec2_DynamicParams_DEFAULT = {
  sizeof(VIDDEC2_DynamicParams),      /* size */
  XDM_DECODE_AU,                      /* decodeHeader */
  0,                                  /* displayWidth */
  IVIDEO_NO_SKIP,                     /* frameSkipMode */
  IVIDDEC2_DISPLAY_ORDER,             /* frameOrder */
  0,                                  /* newFrameFlag */
  0,                                  /* mbDataFlag */
};
vdec2_Object VD2 = {0};
vdec2_Object* hVd = &VD2;
/******************************************************************************
 * vdec2_getMinOutBufs
 ******************************************************************************/
Int32 vdec2_getMinOutBufs(vdec2_Object* hVd)
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
  //printf("minNumOutBufs = %d\n", hVd->minNumOutBufs);
  hVd->maxNumDisplayBufs = decStatus.maxNumDisplayBufs;
  return decStatus.maxNumDisplayBufs;
}
/******************************************************************************
 * vdec2_create
 ******************************************************************************/
int vdec2_create(int h, int w)
{
  Char* codecName = "h264dec";
  VIDDEC2_Handle         hDecode;
  VIDDEC2_Status         decStatus;
  XDAS_Int32             status;
  VIDDEC2_Params* params = &vdec2_Params_DEFAULT;
  VIDDEC2_DynamicParams* dynParams = &vdec2_DynamicParams_DEFAULT;
  params->maxHeight = h, params->maxWidth = w;
  ceruntime_init();
  hEngine = (Engine_Handle)engine_open();
  params->forceChromaFormat = XDM_YUV_422ILE;
  DBG("VIDDEC2_create\n");
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
    DBG("XDM_SETPARAMS control failed\n");
    VIDDEC2_delete(hDecode);
    free(hVd);
    return NULL;
  }

  DBG("Made XDM_SETPARAMS control call\n");
  hVd->dynParams = *dynParams;
  hVd->hDecode = hDecode;
  vdec2_getMinOutBufs(hVd);
  return 0;
}
/******************************************************************************
 * vdec2_delete
 ******************************************************************************/
int vdec2_delete()
{
  if (hVd) {
    if (hVd->hDecode) {
      VIDDEC2_delete(hVd->hDecode);
    }
  }

  return Dmai_EOK;
}
/******************************************************************************
 * vdec2_process
 ******************************************************************************/
Int vdec2_process(const unsigned char* src, int slen, unsigned char* dst, int dlen, int type)
{
  VIDDEC2_InArgs          inArgs;
  VIDDEC2_OutArgs         outArgs;
  XDM1_BufDesc            inBufDesc;
  XDM_BufDesc             outBufDesc;
  XDAS_Int32              outBufSizeArray[XDM_MAX_IO_BUFFERS];
  XDAS_Int8*              outBufPtrArray[XDM_MAX_IO_BUFFERS];
  XDAS_Int32              status;
  Int                     bufIdx;
  XDAS_Int8*              inPtr, *outPtr;
  Int                     ret = Dmai_EOK;
  int nColorSpace = type ? ColorSpace_UYVY : ColorSpace_YUV420PSEMI;
  int inSize, outSize;
  unsigned char* hInBuf = (unsigned char*)src;
  unsigned char* hOutBuf = dst;
  assert(hVd);
  assert(hInBuf);
  assert(hOutBuf);
  inPtr  = (hInBuf);
  outPtr = (hOutBuf);
  inSize = slen;
  outSize = dlen;

  //DBG("vdec2_process %d\n", dlen);
  if (nColorSpace == ColorSpace_YUV420PSEMI) {
    outBufPtrArray[0]       = outPtr;
    outBufSizeArray[0]      = hVd->minOutBufSize[0];
    outBufPtrArray[1]       = outPtr + outSize * 2 / 3;
    outBufSizeArray[1]      = hVd->minOutBufSize[1];
  }
  else if (nColorSpace == ColorSpace_UYVY) {
    outBufPtrArray[0]       = outPtr;
    outBufSizeArray[0]      = outSize;
  }
  else {
    DBG("Unsupported color format of destination buffer\n");
    return Dmai_EINVAL;
  }

  printf("minNumOutBufs=%d\n", hVd->minNumOutBufs);
  outBufDesc.numBufs          = hVd->minNumOutBufs;
  outBufDesc.bufSizes         = outBufSizeArray;
  outBufDesc.bufs             = outBufPtrArray;
  /* One buffer with encoded data */
  inBufDesc.numBufs           = 1;
  inBufDesc.descs[0].buf      = inPtr;
  inBufDesc.descs[0].bufSize  = inSize;
  inArgs.size                 = sizeof(VIDDEC2_InArgs);
  inArgs.numBytes             = inSize;
  inArgs.inputID              = 0;
  outArgs.size                = sizeof(VIDDEC2_OutArgs);
  //DBG("VIDDEC2_process\n");
  /* Decode video buffer */
  status = VIDDEC2_process(hVd->hDecode, &inBufDesc, &outBufDesc, &inArgs, &outArgs);
  dlen = outArgs.bytesConsumed;
  DBG("VIDDEC2_process() ret %d inId %d inUse %d consumed %d\n",
      status, Buffer_getId(hOutBuf), outArgs.outBufsInUseFlag,
      outArgs.bytesConsumed);

  if (status != VIDDEC2_EOK) {
    if (XDM_ISFATALERROR(outArgs.decodedBufs.extendedError)) {
      DBG("VIDDEC2_process() failed with error (%d ext: 0x%x)\n",
          (Int)status, (Uns) outArgs.decodedBufs.extendedError);
      return Dmai_EFAIL;
    }
    else {
      DBG("VIDDEC2_process() non-fatal error 0x%x\n",
          (Uns) outArgs.decodedBufs.extendedError);
      ret = Dmai_EBITERROR;
    }
  }

  printf("%d %d %d\n",
      outArgs.displayBufs[0].frameWidth,
      outArgs.displayBufs[0].frameHeight,
      outArgs.displayBufs[0].framePitch);
  hVd->freeBufIdx = 0;

  /* Was this just the first field of an interlaced frame? */
  if (outArgs.outBufsInUseFlag) {
    ret = Dmai_EFIRSTFIELD;
  }

  return dlen;
}
#if 0
/******************************************************************************
 * vdec2_flush
 ******************************************************************************/
Int vdec2_flush(vdec2_Handle hVd)
{
  VIDDEC2_Status         decStatus;
  XDAS_Int32             status;
  assert(hVd);
  decStatus.data.buf = NULL;
  decStatus.size = sizeof(VIDDEC2_Status);
  /* Flush the codec */
  status = VIDDEC2_control(hVd->hDecode, XDM_FLUSH,
      &hVd->dynParams, &decStatus);

  if (status != VIDDEC2_EOK) {
    DBG("XDM_GETBUFINFO control failed\n");
    return Dmai_EFAIL;
  }

  return Dmai_EOK;
}
/******************************************************************************
 * vdec2_setBufTab
 ******************************************************************************/
Void vdec2_setBufTab(vdec2_Handle hVd, BufTab_Handle hBufTab)
{
  assert(hVd);
  hVd->hOutBufTab = hBufTab;
}
/******************************************************************************
 * vdec2_getBufTab
 ******************************************************************************/
BufTab_Handle vdec2_getBufTab(vdec2_Handle hVd)
{
  assert(hVd);
  return hVd->hOutBufTab;
}
/******************************************************************************
 * vdec2_getOutBufSize
 ******************************************************************************/
Int32 vdec2_getOutBufSize(vdec2_Handle hVd)
{
  Int32 size = 0;
  Int bufIdx;
  assert(hVd);

  for (bufIdx = 0; bufIdx < hVd->minNumOutBufs; bufIdx++) {
    size += hVd->minOutBufSize[bufIdx];
  }

  return size;
}
/******************************************************************************
 * vdec2_getInBufSize
 ******************************************************************************/
Int32 vdec2_getInBufSize(vdec2_Handle hVd)
{
  assert(hVd);
  return hVd->minInBufSize[0];
}
/******************************************************************************
 * vdec2_getDisplayBuf
 ******************************************************************************/
Buffer_Handle vdec2_getDisplayBuf(vdec2_Handle hVd)
{
  assert(hVd);

  if (hVd->displayBufIdx < IVIDDEC2_MAX_IO_BUFFERS &&
      hVd->hDisplayBufs[hVd->displayBufIdx]) {
    return hVd->hDisplayBufs[hVd->displayBufIdx++];
  }
  else {
    return NULL;
  }
}
/******************************************************************************
 * vdec2_getFreeBuf
 ******************************************************************************/
Buffer_Handle vdec2_getFreeBuf(vdec2_Handle hVd)
{
  assert(hVd);

  if (hVd->freeBufIdx < IVIDDEC2_MAX_IO_BUFFERS &&
      hVd->hFreeBufs[hVd->freeBufIdx]) {
    return hVd->hFreeBufs[hVd->freeBufIdx++];
  }
  else {
    return NULL;
  }
}
/******************************************************************************
 * vdec2_getVisaHandle
 ******************************************************************************/
VIDDEC2_Handle vdec2_getVisaHandle(vdec2_Handle hVd)
{
  assert(hVd);
  return hVd->hDecode;
}
#endif
#include "/mnt/hgfs/F/aaa/asdf/decode/venc2a.inl"


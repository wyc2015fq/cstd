#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/video1/videnc1.h>
//#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/ColorSpace.h>
//#include <ti/sdo/dmai/BufferGfx.h>
#define MODULE_NAME     "venc1"
/* As 0 is not a valid buffer id in XDM 1.0, we need macros for easy access */
#define GETID(x)  ((x) + 1)
#define GETIDX(x) ((x) - 1)
typedef struct venc1_Object {
  VIDENC1_Handle          hEncode;
  IVIDEO1_BufDesc         reconBufs;
  Int32                   minNumInBufs;
  Int32                   minInBufSize[XDM_MAX_IO_BUFFERS];
  Int32                   minNumOutBufs;
  Int32                   minOutBufSize[XDM_MAX_IO_BUFFERS];
  VIDENC1_DynamicParams   dynParams;
} venc1_Object;
VIDENC1_Params venc1_Params_DEFAULT = {
  sizeof(VIDENC1_Params),           /* size */
  XDM_DEFAULT,                      /* encodingPreset */
  IVIDEO_LOW_DELAY,                 /* rateControlPreset */
  720,                              /* maxHeight */
  1280,                             /* maxWidth */
  30000,                            /* maxFrameRate */
  2000000,                          /* maxBitRate */
  XDM_BYTE,                         /* dataEndianness */
  0,                                /* maxInterFrameInterval */
  //XDM_YUV_420P,                     /* inputChromaFormat */
  XDM_YUV_422ILE,                     /* inputChromaFormat */
  IVIDEO_PROGRESSIVE,               /* inputContentType */
  XDM_CHROMA_NA                     /* reconChromaFormat */
};
VIDENC1_DynamicParams venc1_DynamicParams_DEFAULT = {
  sizeof(IVIDENC1_DynamicParams),   /* size */
  720,                              /* inputHeight */
  1280,                             /* inputWidth */
  30000,                            /* refFrameRate */
  30000,                            /* targetFrameRate */
  2000000,                          /* targetBitRate */
  30,                               /* intraFrameInterval */
  XDM_ENCODE_AU,                    /* generateHeader */
  0,                                /* captureWidth */
  IVIDEO_NA_FRAME,                  /* forceFrame */
  1,                                /* interFrameInterval */
  0                                 /* mbDataFlag */
};
venc1_Object VE = {0};
venc1_Object* hVe = &VE;
#define BUFFER_GETUSERPTR(_P)  GETBUFPTR(size_t, _P, -1)
#define BUFFER_GETSIZE(_P)  GETBUFVAL(size_t, _P, -1)
#define dspmalloc(n)  (void*)((size_t*)dsp_malloc(n)-1)
#define dspfree(p)    dsp_free((size_t*)(p)+1)
#define FRAMERATE                25000
#define NUMBER_OF_FRAMES         25
/******************************************************************************
 * venc1_create
 ******************************************************************************/
int venc1_create(int h, int w, int maxFrameRate, int maxBitRate)
{
  VIDENC1_Handle          hEncode;
  const char* codecName = "h264enc";
  VIDENC1_Status          encStatus;
  XDAS_Int32              status;
  VIDENC1_Params* params = &venc1_Params_DEFAULT;
  VIDENC1_DynamicParams* dynParams = &venc1_DynamicParams_DEFAULT;
  hVe = &VE;
  venc1_Params_DEFAULT.maxHeight = h, venc1_Params_DEFAULT.maxWidth = w;
  venc1_DynamicParams_DEFAULT.inputHeight = h, venc1_DynamicParams_DEFAULT.inputWidth = w;
  venc1_DynamicParams_DEFAULT.targetBitRate = venc1_Params_DEFAULT.maxBitRate;
  ceruntime_init();
  hEngine = (Engine_Handle)engine_open();

  if (hEngine == NULL) {
    return 0;
  }

  params->rateControlPreset = IVIDEO_STORAGE;
  params->maxBitRate        = maxBitRate ? maxBitRate : 2000000;
  params->maxFrameRate = maxFrameRate ? maxFrameRate : FRAMERATE;
  dynParams->refFrameRate = params->maxFrameRate;
  dynParams->targetFrameRate = params->maxFrameRate;
  dynParams->intraFrameInterval = params->maxFrameRate;
  printf("%d %d\n", w, h);
  params->maxInterFrameInterval = 1;
  params->inputChromaFormat = XDM_YUV_422ILE;
  dynParams->targetBitRate = params->maxBitRate;
  dynParams->inputWidth    = params->maxWidth;
  dynParams->inputHeight   = params->maxHeight;
  /* Create video encoder instance */
  DBG("VIDENC1_create\n");
  hEncode = VIDENC1_create(hEngine, codecName, params);

  if (hEncode == NULL) {
    DBG("Failed to open video encode algorithm: %s (0x%x)\n",
        codecName, Engine_getLastError(hEngine));
    return NULL;
  }

  DBG("Setting dynParams size %dx%d bitrate %d\n",
      dynParams->inputWidth, dynParams->inputHeight,
      dynParams->targetBitRate);
  /* Set video encoder dynamic parameters */
  encStatus.size = sizeof(VIDENC1_Status);
  encStatus.data.buf = NULL;
  DBG("VIDENC1_control\n");
  status = VIDENC1_control(hEncode, XDM_SETPARAMS, dynParams, &encStatus);

  if (status != VIDENC1_EOK) {
    DBG("XDM_SETPARAMS failed, status=%d\n", status);
    VIDENC1_delete(hEncode);
    free(hVe);
    return NULL;
  }

  DBG("Made XDM_SETPARAMS control call\n");
  hVe->dynParams = *dynParams;
  /* Get buffer information from video encoder */
  status = VIDENC1_control(hEncode, XDM_GETBUFINFO, dynParams, &encStatus);

  if (status != VIDENC1_EOK) {
    DBG("XDM_GETBUFINFO control failed\n");
    VIDENC1_delete(hEncode);
    free(hVe);
    return NULL;
  }

  memcpy(hVe->minInBufSize,
      encStatus.bufInfo.minInBufSize, sizeof(hVe->minInBufSize));
  hVe->minNumInBufs = encStatus.bufInfo.minNumInBufs;
  memcpy(hVe->minOutBufSize,
      encStatus.bufInfo.minOutBufSize, sizeof(hVe->minOutBufSize));
  hVe->minNumOutBufs = encStatus.bufInfo.minNumOutBufs;
  hVe->hEncode = hEncode;
  return 0;
}
/******************************************************************************
 * venc1_delete
 ******************************************************************************/
Int venc1_delete()
{
  if (hVe) {
    if (hVe->hEncode) {
      VIDENC1_delete(hVe->hEncode);
    }
  }

  return 0;
}
/******************************************************************************
 * venc1_process
 ******************************************************************************/
Int venc1_process(const unsigned char* src, int slen, unsigned char* dst, int dlen, int type)
{
  IVIDEO1_BufDescIn       inBufDesc;
  XDM_BufDesc             outBufDesc;
  XDAS_Int32              outBufSizeArray[1];
  XDAS_Int32              status;
  VIDENC1_InArgs          inArgs;
  VIDENC1_OutArgs         outArgs;
  XDAS_Int8*              inPtr;
  XDAS_Int8*              outPtr;
  int nColorSpace = type ? ColorSpace_UYVY : ColorSpace_YUV420PSEMI;
  int inSize, outSize;
  //BufferGfx_Dimensions    dim;
  //UInt32                  offset = 0;
  //Uint32                  bpp;
  unsigned char* hInBuf = (unsigned char*)src;
  unsigned char* hOutBuf = dst;
  assert(hVe);
  assert(hInBuf);
  assert(hOutBuf);
  inPtr  = (hInBuf);
  outPtr = (hOutBuf);
  inSize = slen;
  outSize = dlen;
  /* Set up the codec buffer dimensions */
  inBufDesc.frameWidth   = venc1_Params_DEFAULT.maxWidth;
  inBufDesc.frameHeight  = venc1_Params_DEFAULT.maxHeight;
  inBufDesc.framePitch   = venc1_Params_DEFAULT.maxWidth * 2;

  //DBG("inSize %d\n", inSize);
  /* Point to the color planes depending on color space format */
  if (nColorSpace == ColorSpace_YUV420PSEMI) {
    inBufDesc.bufDesc[0].bufSize    = hVe->minInBufSize[0];
    inBufDesc.bufDesc[1].bufSize    = hVe->minInBufSize[1];
    inBufDesc.bufDesc[0].buf        = inPtr;
    inBufDesc.bufDesc[1].buf        = inPtr + inSize * 2 / 3;
    inBufDesc.numBufs               = 2;
  }
  else if (nColorSpace == ColorSpace_UYVY) {
    inBufDesc.bufDesc[0].bufSize    = inSize;
    inBufDesc.bufDesc[0].buf        = inPtr;
    inBufDesc.numBufs               = 1;
  }
  else {
    DBG("Unsupported color format of input buffer\n");
    return Dmai_EINVAL;
  }

  outBufSizeArray[0]                  = outSize;
  outBufDesc.numBufs                  = 1;
  outBufDesc.bufs                     = &outPtr;
  outBufDesc.bufSizes                 = outBufSizeArray;
  inArgs.size                         = sizeof(VIDENC1_InArgs);
  inArgs.inputID                      = GETID(Buffer_getId(hInBuf));
  /* topFieldFirstFlag is hardcoded. Used only for interlaced content */
  inArgs.topFieldFirstFlag            = 1;
  outArgs.size                        = sizeof(VIDENC1_OutArgs);
  /* Encode video buffer */
  status = VIDENC1_process(hVe->hEncode, &inBufDesc, &outBufDesc, &inArgs, &outArgs);
  dlen = outArgs.bytesGenerated;

  //DBG("VIDENC1_process() ret %d inId %d outID %d generated %d bytes\n",
  //  status, Buffer_getId(hInBuf), outArgs.outputID, outArgs.bytesGenerated);
  if (status != VIDENC1_EOK) {
    DBG("VIDENC1_process() failed with error (%d ext: 0x%x)\n",
        (Int)status, (Uns) outArgs.extendedError);
    return Dmai_EFAIL;
  }

  /* Copy recon buffer information so we can retrieve it later */
  hVe->reconBufs = outArgs.reconBufs;
  return dlen;
}
#if 0
/******************************************************************************
 * venc1_getInBufSize
 ******************************************************************************/
Int32 venc1_getInBufSize(venc1_Handle hVe)
{
  Int32 size = 0;
  Int i;
  assert(hVe);

  for (i = 0; i < hVe->minNumInBufs; i++) {
    size += hVe->minInBufSize[i];
  }

  return size;
}
/******************************************************************************
 * venc1_getOutBufSize
 ******************************************************************************/
Int32 venc1_getOutBufSize(venc1_Handle hVe)
{
  assert(hVe);
  return hVe->minOutBufSize[0];
}
/******************************************************************************
 * venc1_getVisaHandle
 ******************************************************************************/
VIDENC1_Handle venc1_getVisaHandle(venc1_Handle hVe)
{
  assert(hVe);
  return hVe->hEncode;
}
/******************************************************************************
 * venc1_getReconBufs
 ******************************************************************************/
IVIDEO1_BufDesc* venc1_getReconBufs(venc1_Handle hVe)
{
  assert(hVe);
  return &hVe->reconBufs;
}
/******************************************************************************
 * venc1_setBufTab
 ******************************************************************************/
Void venc1_setBufTab(venc1_Handle hVe, BufTab_Handle hBufTab)
{
  assert(hVe);
  hVe->hInBufTab = hBufTab;
}
/******************************************************************************
 * venc1_getBufTab
 ******************************************************************************/
BufTab_Handle venc1_getBufTab(venc1_Handle hVe)
{
  assert(hVe);
  return hVe->hInBufTab;
}
/******************************************************************************
 * venc1_getFreeBuf
 ******************************************************************************/
Buffer_Handle venc1_getFreeBuf(venc1_Handle hVe)
{
  Buffer_Handle hFreeBuf;
  assert(hVe);
  hFreeBuf = hVe->hFreeBuf;
  hVe->hFreeBuf = NULL;
  return hFreeBuf;
}
/******************************************************************************
 * venc1_flush
 ******************************************************************************/
Int venc1_flush(venc1_Handle hVe)
{
  VIDENC1_Status         encStatus;
  XDAS_Int32             status;
  assert(hVe);
  encStatus.data.buf = NULL;
  encStatus.size = sizeof(VIDENC1_Status);
  /* Flush the codec */
  status = VIDENC1_control(hVe->hEncode, XDM_FLUSH,
      &hVe->dynParams, &encStatus);

  if (status != VIDENC1_EOK) {
    DBG("XDM_FLUSH control failed\n");
    return Dmai_EFAIL;
  }

  return Dmai_EOK;
}
#endif


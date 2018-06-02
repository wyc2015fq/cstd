
#ifndef _SDEC1_INL_
#define _SDEC1_INL_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/speech1/sphdec1.h>
#include <ti/xdais/dm/ispeech1_pcm.h>

//#include <ti/sdo/dmai/Dmai.h>
//#include <ti/sdo/dmai/Buffer.h>
//#include <ti/sdo/dmai/ce/Sdec1.h>

typedef struct Sdec1_Object {
  SPHDEC1_Handle           hDecode;
  Int32                   minNumInBufs;
  Int32                   minInBufSize[XDM_MAX_IO_BUFFERS];
  Int32                   minNumOutBufs;
  Int32                   minOutBufSize[XDM_MAX_IO_BUFFERS];
} Sdec1_Object;
static Sdec1_Object hSd[1]={0};
static const SPHDEC1_Params Sdec1_Params_DEFAULT = {
  sizeof(SPHDEC1_Params),
  ISPEECH1_PCM_COMPAND_ALAW,
  0,
  0,
  0,
  0,
  NULL,
};

static const SPHDEC1_DynamicParams Sdec1_DynamicParams_DEFAULT = {
  sizeof(SPHDEC1_DynamicParams),
  0,
};

/******************************************************************************
 * Sdec1_cleanup
 ******************************************************************************/
static int Sdec1_cleanup(Sdec1_Object* hSd)
{
  if (hSd->hDecode) {
    SPHDEC1_delete(hSd->hDecode);
  }

  return Dmai_EOK;
}

/******************************************************************************
 * Sdec1_process
 ******************************************************************************/
int Sdec1_process1(const unsigned char* src, int slen, unsigned char* dst, int dlen, int type)
{
  XDM1_SingleBufDesc       inBufDesc;
  XDM1_SingleBufDesc       outBufDesc;
  XDAS_Int32               status;
  SPHDEC1_InArgs           inArgs;
  SPHDEC1_OutArgs          outArgs;
  XDAS_Int8*               inPtr;
  XDAS_Int8*               outPtr;

  assert(hSd);

  inPtr = (unsigned char*)(src);
  outPtr = (dst);

  inBufDesc.buf              = inPtr;
  inBufDesc.bufSize          = slen;

  outBufDesc.buf             = outPtr;
  outBufDesc.bufSize         = dlen;

  inArgs.size                = sizeof(SPHDEC1_InArgs);
  inArgs.frameType           = ISPHDEC1_FTYPE_SPEECHGOOD;

  outArgs.size               = sizeof(SPHDEC1_OutArgs);

  /* We never use the inArgs.data fields, so initialize them to NULL. */
  inArgs.data.buf = (XDAS_Int8*) NULL;
  inArgs.data.bufSize = 0L;

  /* Decode the speech buffer */
  status = SPHDEC1_process(hSd->hDecode, &inBufDesc, &outBufDesc, &inArgs,
      &outArgs);

  Dmai_dbg1("SPHDEC1_process() ret %d \n", status);

  if (status != SPHDEC1_EOK) {
    if (XDM_ISFATALERROR(outArgs.extendedError)) {
      DBG("SPHDEC1_process() failed with error (%d ext: 0x%x)\n",
          (int)status, (Uns)outArgs.extendedError);
      return Dmai_EFAIL;
    }
    else {
      DBG("SPHDEC1_process() non-fatal error 0x%x\n",
          (Uns)outArgs.extendedError);
      return Dmai_EBITERROR;
    }
  }

  /* The codec will fill in how many bytes got used */
  //Buffer_setNumBytesUsed(hOutBuf, Sdec1_getOutBufSize());

  dlen = hSd->minOutBufSize[0];
  return dlen;
}

/******************************************************************************
 * Sdec1_create
 ******************************************************************************/
int Sdec1_create1()
{
  Char* codecName = "g711dec";
  SPHDEC1_Params* params = &Sdec1_Params_DEFAULT;
  SPHDEC1_DynamicParams* dynParams = &Sdec1_DynamicParams_DEFAULT;
  SPHDEC1_Handle       hDecode;
  SPHDEC1_Status       decStatus;
  XDAS_Int32           status;

  if (hEngine == NULL || codecName == NULL ||
      params == NULL || dynParams == NULL) {
    Dmai_err0("Cannot pass null for engine, codec name, params or "
        "dynamic params\n");
    return NULL;
  }

  /* Create speech1 decoder */
  hDecode = SPHDEC1_create(hEngine, codecName, params);

  if (hDecode == NULL) {
    DBG("Failed to create speech1 decoder\n");
    Sdec1_cleanup(hSd);
    return NULL;
  }

  Dmai_dbg1("Speech decoder instance of %s created\n", codecName);

  /* Set dynamic parameters */
  decStatus.size = sizeof(SPHDEC1_Status);
  decStatus.data.buf = (XDAS_Int8*)NULL;
  decStatus.data.bufSize = 0L;

  status = SPHDEC1_control(hDecode, XDM_SETPARAMS, dynParams, &decStatus);

  if (status != SPHDEC1_EOK) {
    Dmai_err1("XDM_SETPARAMS failed, status=%d\n", status);
    Sdec1_cleanup(hSd);
    return NULL;
  }

  /* Get buffer information from video decoder */
  status = SPHDEC1_control(hDecode, XDM_GETBUFINFO, dynParams,
      &decStatus);

  if (status != SPHDEC1_EOK) {
    Dmai_err0("XDM_GETBUFINFO control failed\n");
    Sdec1_cleanup(hSd);
    return NULL;
  }

  DBG("Speech decoder requires min buffer sizes in %u and out %u\n",
      (Uns)decStatus.bufInfo.minInBufSize[0],
      (Uns)decStatus.bufInfo.minOutBufSize[0]);

  memcpy(hSd->minInBufSize,
      decStatus.bufInfo.minInBufSize, sizeof(hSd->minInBufSize));
  hSd->minNumInBufs = decStatus.bufInfo.minNumInBufs;
  memcpy(hSd->minOutBufSize,
      decStatus.bufInfo.minOutBufSize, sizeof(hSd->minOutBufSize));
  hSd->minNumOutBufs = decStatus.bufInfo.minNumOutBufs;

  hSd->hDecode = hDecode;

  return 0;
}

/******************************************************************************
 * Sdec1_delete
 ******************************************************************************/
int Sdec1_delete1()
{
  if (hSd) {
    return Sdec1_cleanup(hSd);
  }

  return Dmai_EOK;
}

#endif // _SDEC1_INL_

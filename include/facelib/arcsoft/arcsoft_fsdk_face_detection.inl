
#include "arcsoft_fsdk_face_detection.h"
#include "merror.h"
#define WORKBUF_SIZE        (40*1024*1024)
#pragma comment(lib,"libarcsoft_fsdk_face_detection.lib")

int arcsoft_fsdk_face_detection(const img_t* im, XRECT* out) {
  static MHandle hEngine = NULL;
  static MByte *pWorkMem = NULL;
  if (NULL==hEngine) {
    MRESULT nRet = MERR_UNKNOWN;
    char APPID[256] = "Ds3AyzWVhxTWTNRv1Zo6QfrjRa3Pm2MXqpwejeZi67TJ";
    char SDKKey[256] = "DPUj1x8DNSWDe77r5r8khJNkKnQeMXZ68SKrpYb8scMq";
    MInt32 nScale = 22;
    MInt32 nMaxFace = 100;
    pWorkMem = (MByte *)malloc(WORKBUF_SIZE);
    if (pWorkMem == NULL)
    {
      fprintf(stderr, "Fail to malloc workbuf\r\n");
      return -1;
    }
    nRet = AFD_FSDK_InitialFaceEngine(APPID, SDKKey, pWorkMem, WORKBUF_SIZE, &hEngine, AFD_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
    if (nRet != MOK || hEngine == NULL)
    {
      fprintf(stderr, "InitialFaceEngine failed , errorcode is %d \r\n", nRet);
      return -1;
    }
  }
  MRESULT nRet = MERR_UNKNOWN;
  ASVLOFFSCREEN offInput = { 0 };
  offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
  offInput.i32Width = im->w;
  offInput.i32Height = im->h;
  offInput.ppu8Plane[0] = im->data;
  offInput.pi32Pitch[0] = im->s;
  //»À¡≥ºÏ≤‚ 
  LPAFD_FSDK_FACERES FaceRes = NULL;
  tic;
  nRet = AFD_FSDK_StillImageFaceDetection(hEngine, &offInput, &FaceRes);
  toc_;
  
  int nface=0;
  if (nRet == MOK) {
    int i;
    for (i = 0; i < FaceRes->nFace; ++i) {
      out[nface].w = FaceRes->rcFace[i].right - FaceRes->rcFace[i].left;
      out[nface].h = FaceRes->rcFace[i].bottom - FaceRes->rcFace[i].top;
      out[nface].x = FaceRes->rcFace[i].left;
      out[nface].y = FaceRes->rcFace[i].top;
      out[nface].count = 1;
      nface++;
    }
  }
  return nface;
}

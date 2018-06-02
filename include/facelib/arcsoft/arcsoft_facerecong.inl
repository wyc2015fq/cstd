

#include "arcsoft_fsdk_face_detection.h"
#include "arcsoft_fsdk_face_recognition.h"
#include "arcsoft_fsdk_face_tracking.h"
#include "arcsoft_fsdk_age_estimation.h"
#include "arcsoft_fsdk_gender_estimation.h"
#include "merror.h"
#pragma comment(lib,"libarcsoft_fsdk_age_estimation.lib")
#pragma comment(lib,"libarcsoft_fsdk_face_detection.lib")
#pragma comment(lib,"libarcsoft_fsdk_gender_estimation.lib")
#pragma comment(lib,"libarcsoft_fsdk_face_recognition.lib")

#define APPID "Ds3AyzWVhxTWTNRv1Zo6QfsMEaMESMThgAPee2PpzYVM"
#define FD_SDKKEY		"EQThhSTUgNMNNnGF5VPmo3LeQwxXudUKcrFzKM3uQCD5"
#define FT_SDKKEY		"EQThhSTUgNMNNnGF5VPmo3LXFYhNv9huP2zYn2aH8sN3"
#define FR_SDKKEY		"EQThhSTUgNMNNnGF5VPmo3LmaMDgXoDr6yatvWn19QyF"
#define FA_SDKKEY		"EQThhSTUgNMNNnGF5VPmo3MPPMXVpAWMK6qsiMbTfCdi"
#define FG_SDKKEY		"EQThhSTUgNMNNnGF5VPmo3MWYknhyuocRWdptUY6UfDb"

#define WORKBUF_SIZE_FD (40*1024*1024)
#define WORKBUF_SIZE_FT (40*1024*1024)
#define WORKBUF_SIZE_FR (40*1024*1024)
#define WORKBUF_SIZE_FA (40*1024*1024)
#define WORKBUF_SIZE_FG (40*1024*1024)

MHandle FDEngine = NULL;
MHandle FTEngine = NULL;
MHandle FREngine = NULL;
MHandle FAEngine = NULL;
MHandle FGEngine = NULL;

MByte *pFDWorkMem = NULL;
MByte *pFTWorkMem = NULL;
MByte *pFRWorkMem = NULL;
MByte *pFAWorkMem = NULL;
MByte *pFGWorkMem = NULL;
#define nullptr NULL


int arcsoft_facerecong(int h, int w, const void* data, int step, int cn, const char* s_pixtype, const char* s_mode, int trans, double scale,
                       int* xywh, void* stdface, void* feature, const void* featlib, int featstep, int feat_num, int* pid, int* pdis) {
  int ret = MOK;
  if (FDEngine==NULL) {
    int nScale = 16;
    int nMaxFace = 12;
    pFDWorkMem = (MByte *)malloc(WORKBUF_SIZE_FD);
    if (pFDWorkMem)
    {
      ret = AFD_FSDK_InitialFaceEngine(APPID, FD_SDKKEY, pFDWorkMem, WORKBUF_SIZE_FD, &FDEngine, AFD_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
    }
  }
  if (FREngine==NULL) {
    int nScale = 16;
    int nMaxFace = 12;
    pFRWorkMem = (MByte *)malloc(WORKBUF_SIZE_FD);
    if (pFRWorkMem)
    {
      ret = AFR_FSDK_InitialEngine(APPID, FR_SDKKEY, pFRWorkMem, WORKBUF_SIZE_FR, &FREngine);
    }
  }
  int featlen = 22020;
  ASSERT(cn==3);
  ASVLOFFSCREEN pImgData[1] = {0};
  pImgData->u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
  pImgData->i32Width = w;
  pImgData->i32Height = h;
  pImgData->ppu8Plane[0] = (unsigned char*)data;
  pImgData->pi32Pitch[0] = step;
  LPAFD_FSDK_FACERES	FaceRes = nullptr;
  ret = AFD_FSDK_StillImageFaceDetection(FDEngine, pImgData, &FaceRes);
  if (ret==MOK && FaceRes->nFace>0) {
    xywh[0] = FaceRes->rcFace[0].left;
    xywh[1] = FaceRes->rcFace[0].top;
    xywh[2] = FaceRes->rcFace[0].right - FaceRes->rcFace[0].left;
    xywh[3] = FaceRes->rcFace[0].bottom - FaceRes->rcFace[0].top;
    if (strchr(s_mode, 'f')) {
      AFR_FSDK_FACEMODEL LocalFaceModels = { 0 };
      AFR_FSDK_FACEINPUT faceInput = {0};
      faceInput.rcFace = FaceRes->rcFace[0];
      faceInput.lOrient = FaceRes->lfaceOrient[0];
      ret = AFR_FSDK_ExtractFRFeature(FREngine, pImgData, &faceInput, &LocalFaceModels);
      if (ret == MOK) {
        ASSERT(LocalFaceModels.lFeatureSize==featlen);
        // 22020
        memcpy(feature, LocalFaceModels.pbFeature, featlen);
      }
    }
    if (ret == MOK && strchr(s_mode, 'r')) {
      int i;
      uchar* ptr = (uchar*)featlib;
      AFR_FSDK_FACEMODEL faceModels1, faceModels2;
      faceModels1.lFeatureSize = featlen;
      faceModels2.lFeatureSize = featlen;
      faceModels1.pbFeature = (uchar*)feature;
      MFloat minScore = 0;
      for (i=0; i<feat_num; ++i) {
        MFloat  fSimilScore = 0.0f;
        faceModels2.pbFeature = ptr;
        ret = AFR_FSDK_FacePairMatching(FREngine, &faceModels1, &faceModels2, &fSimilScore);
        if (fSimilScore>minScore) {
          minScore = fSimilScore;
          *pid = i;
        }
        ptr += featstep;
      }
      minScore = BOUND(minScore, 0, 1);
      *pdis = (1-minScore)*1000;
    }
  }
  ret = (ret==MOK) && (FaceRes->nFace>0);
  return ret;
}

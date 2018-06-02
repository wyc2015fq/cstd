
#include <stdlib.h>
#include "cstd.h"
#include "arcsoft_fsdk_face_detection.inl"

#define nullptr NULL
#include "cap.h"
#include "img/imgio.inl"
#include "cv1/drawing.inl"

int test_arcsoft_face() {
  CPCODE;
	MRESULT nRet = MERR_UNKNOWN;
	MHandle hEngine = nullptr;
	char APPID[256] = "Ds3AyzWVhxTWTNRv1Zo6QfrjRa3Pm2MXqpwejeZi67TJ";
	char SDKKey[256] = "DPUj1x8DNSWDe77r5r8khJNkKnQeMXZ68SKrpYb8scMq";
	MInt32 nScale = 22;
	MInt32 nMaxFace = 100;
	MByte *pWorkMem = (MByte *)malloc(WORKBUF_SIZE);
	if (pWorkMem == nullptr)
	{
		fprintf(stderr, "Fail to malloc workbuf\r\n");
		return -1;
	}
	nRet = AFD_FSDK_InitialFaceEngine(APPID, SDKKey, pWorkMem, WORKBUF_SIZE, &hEngine, AFD_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
  if (nRet != MOK || hEngine == nullptr)
  {
    fprintf(stderr, "InitialFaceEngine failed , errorcode is %d \r\n", nRet);
    return -1;
  }
  //获取版本信息
  const AFD_FSDK_Version * pVersionInfo = nullptr;
  pVersionInfo = AFD_FSDK_GetVersion(hEngine);
  fprintf(stdout, "%d %d %d %d\r\n", pVersionInfo->lCodebase, pVersionInfo->lMajor, pVersionInfo->lMinor, pVersionInfo->lBuild);
  fprintf(stdout, "%s\r\n", pVersionInfo->Version);
  fprintf(stdout, "%s\r\n", pVersionInfo->BuildDate);
  fprintf(stdout, "%s\r\n", pVersionInfo->CopyRight);
  //读取bmp图片数据
  ASVLOFFSCREEN offInput = { 0 };
  img_t im[1] = {0};
  int i;
  IRECT rect[200] = {0};
  if (0) {
    const char* filename = "F:/dataset/facedetect/hztest.jpg";
    imread(filename, 3, 1, im);
    int i, nface = 0;
    offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
    offInput.i32Width = im->w;
    offInput.i32Height = im->h;
    offInput.ppu8Plane[0] = im->data;
    offInput.pi32Pitch[0] = im->s;
    //人脸检测
    LPAFD_FSDK_FACERES	FaceRes = nullptr;
    tic;
    nRet = AFD_FSDK_StillImageFaceDetection(hEngine, &offInput, &FaceRes);
    toc_;
#if 1
    if (nRet == MOK) {
      fprintf(stdout, "The number of face: %d\r\n", FaceRes->nFace);
      for (i = 0; i < FaceRes->nFace; ++i) {
        fprintf(stdout, "Face[%d]: rect[%d,%d,%d,%d], Face orient: %d\r\n", i,
          FaceRes->rcFace[i].left, FaceRes->rcFace[i].top,
          FaceRes->rcFace[i].right-FaceRes->rcFace[i].left,
          FaceRes->rcFace[i].bottom-FaceRes->rcFace[i].top, FaceRes->lfaceOrient[i]);
        
        rect[nface++] = iRECT(FaceRes->rcFace[i].left, FaceRes->rcFace[i].top,
          FaceRes->rcFace[i].right, FaceRes->rcFace[i].bottom);
      }
    }
    else
    {
      fprintf(stderr, "Face Detection failed, error code: %d\r\n", nRet);
    }
#endif
    
    
    if (1) {
      for (i = 0; i < nface; ++i) {
        cvRectangle(im, cPoint(rect[i].l, rect[i].t), cPoint(rect[i].r, rect[i].b), cScalar(0, 0, 255), 2, 8, 0);
        
        //imdrawaa_rect(im, false, NULL, dRECT((*it).y1, (*it).x1, (*it).y2, (*it).x2), 0, _RGB(255, 0, 0), 3);
        //for(int num=0;num<5;num++) imdrawaa_circle(im, NULL, it->ppoint[num], it->ppoint[num+5], 3, _RGB(255, 255, 0), 0, 0);
      }
      printf("\n");
      imshow_(im);
      WaitKey(0);
    }
  }
  
  capdev cap[1] = { 0 };
  int h = 720, w = 1280;
  //h = 480, w = 640, minsize=60;
  if (capdshow_open(cap, 0, w, h) > 0) {
    int fram = 0;
    char ch = 0;
    img_t im[2] = { 0 };
    img_t* im1 = im + 1;
    int nface = 0;
    double avg = 0;

    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cap, im, CAP_RGB);
      //image.create(h, w, CC_8UC3, im->tt.data, im->s);
      //imtrans(im, im1, 1, 1, 0);
      im1 = im;
        LPAFD_FSDK_FACERES	FaceRes = nullptr;
      offInput.u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
      offInput.i32Width = im->w;
      offInput.i32Height = im->h;
      offInput.ppu8Plane[0] = im->data;
      offInput.pi32Pitch[0] = im->s;
      {
        utime_start(_start_time);
        tictoc0;
        //人脸检测
        tic;
        nRet = AFD_FSDK_StillImageFaceDetection(hEngine, &offInput, &FaceRes);
        toc_;
        tictoc;
        double t = utime_elapsed(_start_time);
        avg += t;
        printf("%f %f ", t, avg / (fram + 1));
      }
      nface=0;
      if (nRet == MOK) {
        for (i = 0; i < FaceRes->nFace; ++i) {
        fprintf(stdout, "(%d,%d,%d,%d)[%d]",
          FaceRes->rcFace[i].left, FaceRes->rcFace[i].top,
          FaceRes->rcFace[i].right-FaceRes->rcFace[i].left,
          FaceRes->rcFace[i].bottom-FaceRes->rcFace[i].top, FaceRes->lfaceOrient[i]);

          rect[nface++] = iRECT(FaceRes->rcFace[i].left, FaceRes->rcFace[i].top,
            FaceRes->rcFace[i].right, FaceRes->rcFace[i].bottom);
        }
      }
      else
      {
        fprintf(stderr, "Face Detection failed, error code: %d\r\n", nRet);
      }
      if (1) {
        int i;

        for (i = 0; i < nface; ++i) {
          cvRectangle(im, cPoint(rect[i].l, rect[i].t), cPoint(rect[i].r, rect[i].b), cScalar(0, 0, 255), 2, 8, 0);
        }
      }

      printf("\n");
      imshow_(im1);
      ch = WaitKey(10);
    }

    cap_close(cap);
  }

  //反初始化
  nRet = AFD_FSDK_UninitialFaceEngine(hEngine);
  if (nRet != MOK)
  {
    fprintf(stderr, "UninitialFaceEngine failed , errorcode is %d \r\n", nRet);
  }
  free(pWorkMem);
  imfree(im);
  return 0;
}

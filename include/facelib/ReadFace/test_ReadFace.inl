
#include <stdlib.h>
#include "cstd.h"
#include "RSFaceSDK.inl"

//int arcsoft_fsdk_face_detection(const img_t* im, XRECT* out) {}

#ifdef TEST
#include "cap.h"
#include "img/imgio.inl"
#include "cv1/drawing.inl"
//#include "test_ReadFace2.inl"

//AppId	291e0e83138767cfbeadac20c2148ee4
//AppSecret	718b6abf62113aaf80bbcfdfc417dd8225ecf6c2
int test_ReadFace() {
	char *APPID = "291e0e83138767cfbeadac20c2148ee4";
	char *SDKKey = "718b6abf62113aaf80bbcfdfc417dd8225ecf6c2";
  RSHandle hEngine = NULL;
  RSHandle handle = NULL;
  img_t im[1] = {0};
  rs_face*	FaceRes = NULL;
  rsInitLicenseManager(&hEngine, APPID, SDKKey);
	if (hEngine == NULL)
	{
		fprintf(stderr, "Fail to malloc handle\r\n");
		return -1;
	}
	rsInitFaceDetect(&handle, hEngine);
  int i;
  IRECT rect[200] = {0};
  if (1) {
    const char* filename = "F:/dataset/facedetect/hztest.jpg";
    imread(filename, 3, 1, im);
    int i, nface = 0, nRet=0;
    //人脸检测
    tic;
    nRet = rsRunFaceDetect(handle, im->data, PIX_FORMAT_BGR888, im->w, im->h, im->s,
      RS_IMG_CLOCKWISE_ROTATE_0, &FaceRes, &nface);
    toc_;
#if 1
    fprintf(stdout, "The number of face: %d\r\n", nface);
    for (i = 0; i < nface; ++i) {
      fprintf(stdout, "Face[%d]: rect[%d,%d,%d,%d], Face: %f\r\n", i,
        FaceRes[i].rect.left, FaceRes[i].rect.top,
        FaceRes[i].rect.width,
        FaceRes[i].rect.height, FaceRes[i].pitch);
      
      rect[i] = iRECT2(FaceRes[i].rect.left, FaceRes[i].rect.top,
        FaceRes[i].rect.width, FaceRes[i].rect.height);
    }
#endif
    
    releaseFaceDetectResult(FaceRes, nface);
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
      int nRet;
      FaceRes = NULL;
      nface = 0;
      {
        utime_start(_start_time);
        tictoc0;
        //人脸检测
        tic;
        nRet = rsRunFaceDetect(handle, im->data, PIX_FORMAT_BGR888, im->w, im->h, im->s,
          RS_IMG_CLOCKWISE_ROTATE_0, &FaceRes, &nface);
        toc_;
        tictoc;
        double t = utime_elapsed(_start_time);
        avg += t;
        printf("%f %f ", t, avg / (fram + 1));
      }
      if (nRet>=0) {
        for (i = 0; i < nface; ++i) {
          fprintf(stdout, "Face[%d]: rect[%d,%d,%d,%d], Face: %f\r\n", i,
            FaceRes[i].rect.left, FaceRes[i].rect.top,
            FaceRes[i].rect.width,
            FaceRes[i].rect.height, FaceRes[i].pitch);
          
          rect[i] = iRECT2(FaceRes[i].rect.left, FaceRes[i].rect.top,
            FaceRes[i].rect.width, FaceRes[i].rect.height);
        }
        releaseFaceDetectResult(FaceRes, nface);
        if (1) {
          int i;
          
          for (i = 0; i < nface; ++i) {
            cvRectangle(im, cPoint(rect[i].l, rect[i].t), cPoint(rect[i].r, rect[i].b), cScalar(0, 0, 255), 2, 8, 0);
          }
        }
      }

      printf("\n");
      imshow_(im1);
      ch = WaitKey(10);
    }

    cap_close(cap);
  }
	rsUnInitFaceDetect(&handle);
  //反初始化
  rsUnInitLicenseManager(&hEngine);
  imfree(im);
  return 0;
}
#endif

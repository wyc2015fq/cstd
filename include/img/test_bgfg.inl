#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>


int main(int argc, char** argv)
{
  //声明IplImage指针
  IplImage* pFrame = NULL;
  IplImage* pFrImg = NULL;
  IplImage* pBkImg = NULL;

  CvMat* pFrameMat = NULL;
  CvMat* pFrMat = NULL;
  CvMat* pBkMat = NULL;

  CvCapture* pCapture = NULL;

  CvBGStatModel* pBGStatModel = NULL;

  int nFrmNum = 0;

  //创建窗口
  cvNamedWindow("video", 1);
  cvNamedWindow("background", 1);
  cvNamedWindow("foreground", 1);
  //使窗口有序排列
  cvMoveWindow("video", 30, 0);
  cvMoveWindow("background", 360, 0);
  cvMoveWindow("foreground", 690, 0);

  //打开视频文件
  if (!(pCapture = cvCaptureFromCAM(0))) {
    fprintf(stderr, "Can not open video file %s\n", argv[ 1 ]);
    return -2;
  }

  //逐帧读取视频
  while (pFrame = cvQueryFrame(pCapture)) {
    nFrmNum++;

    //如果是第一帧，需要申请内存，并初始化
    if (nFrmNum == 1) {
      pBkImg = cvCreateImage(cvSize(pFrame->w, pFrame->h),
          IMG_DEPTH_8U, 1);
      pFrImg = cvCreateImage(cvSize(pFrame->w, pFrame->h),
          IMG_DEPTH_8U, 1);
      pBkMat = cvCreateMat(pFrame->h, pFrame->w, CC_32FC1);
      pFrMat = cvCreateMat(pFrame->h, pFrame->w, CC_32FC1);
      pFrameMat = cvCreateMat(pFrame->h, pFrame->w, CC_32FC1);

      //pBGStatModel = cvCreateFGDStatModel(pFrame, 0);
      pBGStatModel = cvCreateGaussianBGModel(pFrame, 0);
    }
    else {
      //CC_ASSERT(pBGStatModel!=NULL);

      {
        utime_start(_start_time);
        cvUpdateBGStatModel(pFrame, pBGStatModel);
        printf("%f\n", utime_elapsed(_start_time));
      }
      //显示图像
      cvShowImage("video", pFrame);
      cvShowImage("background", pBGStatModel->background);
      cvShowImage("foreground", pBGStatModel->foreground);

      //如果有按键事件，则跳出循环
      //此等待也为cvShowImage函数提供时间完成显示
      //等待时间可以根据CPU速度调整
      if (cvWaitKey(2) >= 0) {
        break;
      }
    }  // end of if-else
  } // end of while-loop

  cvReleaseBGStatModel(&pBGStatModel);
  //销毁窗口
  cvDestroyWindow("video");
  cvDestroyWindow("background");
  cvDestroyWindow("foreground");

  //释放图像和矩阵
  cvReleaseImage(&pFrImg);
  cvReleaseImage(&pBkImg);

  cvReleaseMat(&pFrameMat);
  cvReleaseMat(&pFrMat);
  cvReleaseMat(&pBkMat);

  return 0;
}

# OpenCV 轮廓检测 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月10日 17:02:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：35
个人分类：[OpenCV](https://blog.csdn.net/leixiaohua1020/article/category/1362937)










使用OpenCV可以对图像的轮廓进行检测。这是之前用过的代码，挺简单的，回顾一下。主要要进行以下2步操作：

1.cvThreshold()：对图像进行二值化处理

2.cvFindContours()：查找图像轮廓

注意：这个过程中图像要转化为灰度图。



```cpp
/***********************************************************************
	雷霄骅
 ***********************************************************************/
#include "stdafx.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
 
int main( int argc, char** argv )
{
  //声明IplImage指针
  IplImage* pImg = NULL; 
  IplImage* pContourImg = NULL;
 
  CvMemStorage * storage = cvCreateMemStorage(0);
  CvSeq * contour = 0;
  int mode = CV_RETR_EXTERNAL;
 
  if( argc == 3)
      if(strcmp(argv[2], "all") == 0)
	mode = CV_RETR_CCOMP; //内外轮廓都检测
 
 
  //创建窗口
  cvNamedWindow("src", 1);
  cvNamedWindow("contour",1);
  cvNamedWindow("threshold",1);
 
 
  //载入图像，强制转化为Gray
  if( argc >= 2 && 
      (pImg = cvLoadImage( argv[1], 0)) != 0 )
    {
 
      cvShowImage( "src", pImg );
 
      //为轮廓显示图像申请空间
      //3通道图像，以便用彩色显示
      pContourImg = cvCreateImage(cvGetSize(pImg),
					  IPL_DEPTH_8U,
					  3);
      //copy source image and convert it to BGR image
      cvCvtColor(pImg, pContourImg, CV_GRAY2BGR);
//----阈值分割-------------------------------------------
	  cvThreshold( pImg, pImg, 150, 255, CV_THRESH_BINARY );
	  cvShowImage( "threshold", pImg );
//-----------------------------------------------
//查找contour----------------输入必须是二值图像
      cvFindContours( pImg, storage, &contour, sizeof(CvContour), 
		  mode, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
 
    }
  else
    {
      //销毁窗口
      cvDestroyWindow( "src" );
      cvDestroyWindow( "contour" );
      cvReleaseMemStorage(&storage);
 
      return -1;
    }
 
 
 
 
  //将轮廓画出    
  cvDrawContours(pContourImg, contour, 
		 CV_RGB(0,0,255), CV_RGB(255, 0, 0), 
		 2, 2, 8, cvPoint(0,0));
  //显示图像
  cvShowImage( "contour", pContourImg );
 
  cvWaitKey(0);
 
 
  //销毁窗口
  cvDestroyWindow( "src" );
  cvDestroyWindow( "contour" );
  //释放图像
  cvReleaseImage( &pImg ); 
  cvReleaseImage( &pContourImg ); 
 
  cvReleaseMemStorage(&storage);
 
  return 0;
}
```


 源图像：



![](https://img-blog.csdn.net/20131110165537484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

二值化以后：

![](https://img-blog.csdn.net/20131110165628203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

轮廓：

![](https://img-blog.csdn.net/20131110165643218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





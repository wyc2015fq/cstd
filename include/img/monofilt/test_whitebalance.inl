//#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <vector>
#include "whitebalance.inl"
using namespace std;

int test_whitebalance()
{
  IplImage* original;

  //载入图像
  original=cvLoadImage( "1.jpg", 1);  
  //original 是你打开的图片，用彩色格式打开
  //调一下 threshold （调大）和whitebalance里面的PIXELNUMBERTHRESHOLD（调大）
  if(original==NULL)
    return 0;	
  IplImage* temp=cvCloneImage(original);
	IplImage* WBWithHis_result=cvCreateImage(cvGetSize(original),8,3);
	IplImage* GWM_result=cvCreateImage(cvGetSize(original),8,3);

	WBWithHis(temp, WBWithHis_result);
	WBWithHis(temp, GWM_result);

	cvNamedWindow("original",1);
	cvShowImage("original",original);

	double threshold=5000;
	IplImage* whiteimage=cvCreateImage(cvGetSize(temp),IMG_DEPTH_8U,1);
	IplImage* blackimage=cvCreateImage(cvGetSize(temp),IMG_DEPTH_8U,1);

	cvSetZero(whiteimage);
	cvSetZero(blackimage);
	for(int i=0;i<temp->height;i++)
	{
		for(int j=0;j<temp->width;j++)
		{
			double effwhite=0.0;
			double effblack=0.0;
			for(int m=0;m<temp->nChannels;m++)
			{
				effwhite+=((uchar)temp->imageData[i*temp->widthStep+j*temp->nChannels+m]
					-(uchar)255)*
					((uchar)temp->imageData[i*temp->widthStep+j*temp->nChannels+m]
					-(uchar)255);
				effblack+=((uchar)temp->imageData[i*temp->widthStep+j*temp->nChannels+m]
					-(uchar)0)*
					((uchar)temp->imageData[i*temp->widthStep+j*temp->nChannels+m]
					-(uchar)0);
			}
			if(effwhite<threshold)
				whiteimage->imageData[i*whiteimage->widthStep+j*whiteimage->nChannels]=(uchar)255;
			if(effblack<threshold)
				blackimage->imageData[i*blackimage->widthStep+j*blackimage->nChannels]=(uchar)255;
		}
	}
	cvNamedWindow("blackimage",1);
	cvShowImage("blackimage",blackimage);
	cvNamedWindow("whiteimage",1);
	cvShowImage("whiteimage",whiteimage);
  cvNamedWindow("WBWithHis_result",1);
  cvShowImage("WBWithHis_result",WBWithHis_result);
  cvNamedWindow("GWM_result",1);
  cvShowImage("GWM_result",GWM_result);
  cvWaitKey(0);
  
  cvReleaseImage(&temp);
  cvReleaseImage(&whiteimage);
  cvReleaseImage(&blackimage);
  cvReleaseImage(&WBWithHis_result);
  return 0;
}
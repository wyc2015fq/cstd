# 学习opencv 习题答案-第四章 - 勿在浮砂筑高台 - CSDN博客





2014年11月23日 23:00:12[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：1161







/*4_1*/

```cpp
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
int main(int argc, char **argv)
{

CvCapture *video =cvCreateFileCapture(argv[1]); 
cvNamedWindow(argv[1]); 

IplImage *frame =cvQueryFrame(video);
frame =cvQueryFrame(video);
IplImage *grey= cvCreateImage(cvGetSize(frame),frame->depth,1);  //创建灰度图
cvConvertImage(frame,grey);

IplImage *unit =cvCreateImage(cvSize(frame->width * 3,frame->height),frame->depth,3);
IplImage *imag_header = cvCreateImageHeader(cvSize(frame->width ,frame->height),frame->depth,3);
imag_header->widthStep = unit->widthStep;        //这里若有疑问，见我的帖子
    
imag_header->imageData = unit->imageData;    //显示原图像
cvCopy(frame,imag_header);

IplImage *grey_imag=cvCreateImage(cvSize(frame->width,frame->height),frame->depth,3);   //显示灰度图
cvConvertImage(grey,grey_imag,CV_GRAY2BGR);
imag_header->imageData = unit->imageData+frame->widthStep;
cvCopy(grey_imag,imag_header);  

imag_header->imageData = unit->imageData+frame->widthStep*2;    //显示边缘检测图像
cvCanny(grey,grey,10,100);
cvConvertImage(grey,grey_imag,CV_GRAY2BGR); 
cvCopy(grey_imag,imag_header);  

CvFont m_font;
cvInitFont(&m_font,CV_FONT_HERSHEY_PLAIN,1.0,1.0);
        cvPutText(unit,"Origin",cvPoint(0,10),&m_font,cvScalar(255,255,0));
        cvPutText(unit,"grey_imag",cvPoint(frame->width,10),&m_font,cvScalar(255,255,0));
cvPutText(unit,"canny_imag",cvPoint(frame->width*2,10),&m_font,cvScalar(255,255,0));

cvShowImage(argv[1],unit);
cvWaitKey(0);
cvReleaseImageHeader(&imag_header);
cvReleaseImage(&grey_imag);
cvReleaseCapture(&video);
cvReleaseImage(&grey);
cvDestroyWindow(argv[1]);
   
}
```






/*  4_2  */



```cpp
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include "iostream"
using namespace std;
void my_mouse_Callback(int event , int x,int y,int flags ,void *param);
bool show_infor =false;
int main(int argc, char **argv)
{
CvFont m_font;
cvInitFont(&m_font,CV_FONT_HERSHEY_PLAIN,1.0,1.0);
CvCapture *video =cvCreateFileCapture(argv[1]); 
cvNamedWindow("ss"); 

IplImage *frame =cvQueryFrame(video);
frame =cvQueryFrame(video);
IplImage *grey= cvCreateImage(cvGetSize(frame),frame->depth,1);  //创建灰度图
cvConvertImage(frame,grey);

IplImage *unit =cvCreateImage(cvSize(frame->width * 2,frame->height),frame->depth,3);
IplImage *imag_header = cvCreateImageHeader(cvSize(frame->width ,frame->height),frame->depth,3);
imag_header->widthStep = unit->widthStep;
    
imag_header->imageData = unit->imageData;    //显示原图像
cvCopy(frame,imag_header);

IplImage *grey_imag=cvCreateImage(cvSize(frame->width,frame->height),frame->depth,3);   //显示灰度图
cvConvertImage(grey,grey_imag,CV_GRAY2BGR);
imag_header->imageData = unit->imageData+frame->widthStep;
cvCopy(grey_imag,imag_header);  

imag_header->imageData = unit->imageData+frame->widthStep*2;    //显示边缘检测图像
cvCanny(grey,grey,10,100);
cvConvertImage(grey,grey_imag,CV_GRAY2BGR); 
cvCopy(grey_imag,imag_header);  

        cvPutText(unit,"Origin",cvPoint(0,10),&m_font,cvScalar(255,255,0));
        cvPutText(unit,"grey_imag",cvPoint(frame->width,10),&m_font,cvScalar(255,255,0));
cvPutText(unit,"canny_imag",cvPoint(frame->width*2,10),&m_font,cvScalar(255,255,0));

      IplImage *temp = cvCloneImage(unit);
      
cvSetMouseCallback("ss",my_mouse_Callback,(void *)temp);
while(1)
{
if (show_infor == false)
{
cvCopy(unit,temp);
}
if (cvWaitKey(10) ==27)
{
break;
}
 cvShowImage("ss",temp);
}
 
        cvReleaseImage(&temp);
cvReleaseImageHeader(&imag_header);
cvReleaseImage(&grey_imag);
cvReleaseCapture(&video);
cvReleaseImage(&grey);
cvDestroyWindow("ss");
   
}
void my_mouse_Callback(int event , int x,int y,int flags ,void *param)
{
   if (event ==CV_EVENT_LBUTTONDOWN)
   {  
      show_infor=true;

   CvFont m_font;
   cvInitFont(&m_font,CV_FONT_HERSHEY_PLAIN,1.0,1.0);
       CvScalar rgb = cvGet2D((IplImage*)param,y,x);
   double r=rgb.val[2];
   double g=rgb.val[1];
   double b=rgb.val[0];
   
   char str[40]={0};
   sprintf(str,"%d,%dClolorR:%.0f,G:%.0f,B:%.0f,",x,y,rgb.val[2],rgb.val[1],rgb.val[0]);
       cvPutText((IplImage*)param,str,cvPoint(x,y),&m_font,cvScalar(255,255,0));
   }
   if (event ==CV_EVENT_LBUTTONUP)
   {
   show_infor=false;
   }
}
```












/* 4_3.a*/


```cpp
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"

CvRect m_rect;
bool drawing =false;
bool drawing_end =false;
void m_mouse_callback(int event,int x, int y,int flag, void *param);
int main(int argc, char **argv)
{
CvFont m_font;
cvInitFont(&m_font,CV_FONT_HERSHEY_PLAIN,1.0,1.0);
    CvCapture *video =cvCreateFileCapture(argv[1]); 
cvNamedWindow(argv[1]); 
  
IplImage *frame =cvQueryFrame(video);
frame =cvQueryFrame(video);   
IplImage *clone1 = cvCloneImage(frame);
   cvSetMouseCallback(argv[1],m_mouse_callback,(void *)clone1);
   
while(1)
{
if (cvWaitKey(10)==27)
{
break;
}
cvCopy(frame,clone1);
if (drawing_end == false)
{
cvRectangle(clone1,cvPoint(m_rect.x,m_rect.y),cvPoint(m_rect.x+m_rect.width,m_rect.y+m_rect.height),cvScalar(255,0,0),1);
}
else
{
cvRectangle(clone1,cvPoint(m_rect.x,m_rect.y),cvPoint(m_rect.x+m_rect.width,m_rect.y+m_rect.height),cvScalar(255,0,0),3);
}
cvShowImage(argv[1],clone1);
}
   



cvReleaseCapture(&video);
cvDestroyWindow(argv[1]);
   
}
void m_mouse_callback(int event,int x, int y,int flag, void *param)
{
    switch (event)
{
case CV_EVENT_LBUTTONDOWN:
drawing_end =false;
drawing = true;
m_rect=cvRect(x,y,0,0);
break;
case CV_EVENT_LBUTTONUP:
drawing_end =true;
drawing =false;
break;
case CV_EVENT_MOUSEMOVE:
           if (drawing == true)
           {
             m_rect.width = x-m_rect.x;
 m_rect.height =y-m_rect.y;
           } 
break;
default:break;
}
}
```










/* 4_4 */


```cpp
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
CvCapture *video=NULL;
void onTrackSlide(int pos)
{
cvSetCaptureProperty(video,CV_CAP_PROP_POS_FRAMES,pos);
}
int main(int argc, char **argv)
{
CvFont m_font;
cvInitFont(&m_font,CV_FONT_HERSHEY_PLAIN,1.0,1.0);
    video =cvCreateFileCapture(argv[1]); 
cvNamedWindow(argv[1]); 
  
    
IplImage *frame =cvQueryFrame(video);
int video_conut;
   video_conut = (int)cvGetCaptureProperty(video,CV_CAP_PROP_FRAME_COUNT);
   int value=0;
  int  flag =0;
   cvCreateTrackbar("进度条",argv[1],&value,video_conut,onTrackSlide);
   cvCreateTrackbar("暂停/播放",argv[1],&flag,1,NULL); cvShowImage(argv[1],frame);
   while(frame != NULL)
   {
 if (flag == 0)
 {
 value++;
 cvSetTrackbarPos("进度条",argv[1],value);
 cvShowImage(argv[1],frame);
 if (cvWaitKey(33)==27)
 {
 break;
 }
 frame =cvQueryFrame(video);
 }
 else
 {
cvWaitKey(1);
 }   
   }
cvReleaseCapture(&video);
cvDestroyWindow(argv[1]);
   
}
```






/* 4_5 */


```cpp
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;
CvRect m_rect;
bool drawing =false;
bool drawing_end =false; 
bool eraser_flag =false;
void m_mouse_callback(int event,int x, int y,int flag, void *param);

int main(int argc, char **argv)
{

cvNamedWindow("Draw");
IplImage *m_image=cvCreateImage(cvSize(500,500),IPL_DEPTH_8U,3);
IplImage *clone1 =cvCloneImage(m_image);
int flag=0;
cvZero(m_image);
cvSetMouseCallback("Draw",m_mouse_callback,(void *)clone1);
cvCreateTrackbar("选择图形","Draw",&flag,3,NULL);
while (1)
{
if (cvWaitKey(10) == 27)
{
  break; 
}
if (drawing_end == false&&eraser_flag ==false)
{
cvCopy(m_image,clone1);
}
if (!eraser_flag &&drawing)
{
switch (flag)
{
case 0:cvLine(clone1,cvPoint(m_rect.x,m_rect.y),cvPoint(m_rect.x+m_rect.width,m_rect.y+m_rect.height),cvScalar(255,255,255));
break;
case 1:cvRectangle(clone1,cvPoint(m_rect.x,m_rect.y),cvPoint(m_rect.x+m_rect.width,m_rect.y+m_rect.height),cvScalar(255,255,255));
break;
case 2:{
           int dot_x = (int)m_rect.x;   //画圆形
           int dot_y = (int)m_rect.y;
CvPoint dot =cvPoint(dot_x,dot_y);
int redius =(int)sqrt((long double)((m_rect.width*m_rect.width)+(m_rect.height*m_rect.height)));
cvCircle(clone1,dot,redius,cvScalar(255,255,255));
   }
   break;
case 3:
break;
}
}
cvShowImage("Draw",clone1);
}
cvWaitKey(0);
cvDestroyWindow("Draw");
cvReleaseImage(&m_image);
cvReleaseImage(&clone1);
   
}
void m_mouse_callback(int event,int x, int y,int flag, void *param)
{
    switch (event)
{
case CV_EVENT_LBUTTONDOWN:
drawing_end =false;
drawing = true;
m_rect=cvRect(x,y,0,0);
break;
case CV_EVENT_LBUTTONUP:
drawing_end =true;
drawing =false;
break;
case CV_EVENT_MOUSEMOVE:
           if (drawing == true)
           {
             m_rect.width = x-m_rect.x;
 m_rect.height =y-m_rect.y;
           }
   else if (eraser_flag)
   {
            IplImage * temp = cvCreateImageHeader(cvSize(20,20),((IplImage *)param)->depth,3);     //矩形橡皮擦
              temp->widthStep =((IplImage *)param)->widthStep;
  temp->imageData =((IplImage *)param)->imageData+temp->widthStep*y+x*3;
  cvSet(temp,cvScalar(0,0,0));
  
            cvReleaseImageHeader(&temp);
       
   }
break;
case CV_EVENT_RBUTTONDOWN:
           
eraser_flag = true;
break;
case CV_EVENT_RBUTTONUP:
eraser_flag =false;
break;
default:break;
}
}
```
/*  4_6 */


```cpp
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;
bool edit_flag = false;
void m_mouse_callback(int event,int x, int y,int flag, void *param);
CvPoint m_point;
int main(int argc, char **argv)
{
	CvFont my_font;
	cvInitFont(&my_font,CV_FONT_HERSHEY_SIMPLEX,1.0,1.0);
	IplImage * m_image = cvCreateImage(cvSize(500,500),IPL_DEPTH_8U,3);
	cvNamedWindow("My_Picture");
	cvZero(m_image);
	cvSetMouseCallback("My_Picture",m_mouse_callback,(void *)m_image);
	cvShowImage("My_Picture",m_image);
	char c[2]={0,'\0'};
	while(1)
	{
		if (edit_flag)
		{
			c[0]=cvWaitKey(0);
			if ( 13 == c[0] )
			{
				edit_flag = false;
			}
			else
			{
				cvPutText(m_image,c,m_point,&my_font,cvScalar(255,255,255));
				m_point.x+=20;
				cvShowImage("My_Picture",m_image);
			}

		}
		else if (cvWaitKey(1) == 27)
		{
			break;
		}

	}
	cvReleaseImage(&m_image);
	cvDestroyWindow("My_Picture");
	return 0;
}
void m_mouse_callback(int event,int x, int y,int flag, void *param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		m_point.x = x;
		m_point.y = y;
		edit_flag = true;
		break;
	default:break;
	}
}
```








# 《学习OpenCV》课后习题解答(第四章)(仅供参考)(不断更新) - 江南烟雨 - CSDN博客
2011年08月24日 21:48:53[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5179
代码在VS2008下通过，要在附加依赖项中添加：opencv_core220d.lib opencv_highgui220d.lib opencv_imgproc220d.lib。也可以在代码里面添加以下内容：
```cpp
#pragma comment(lib,"opencv_core220d.lib")
#pragma comment(lib,"opencv_highgui220d.lib")
#pragma comment(lib,"opencv_imgproc220d.lib")
```
还要配置好OpenCV环境，设置环境变量啊什么的。具体可见这篇文章：[配置](http://blog.csdn.net/xiajun07061225/article/details/6694195)
Chapter 4:
1、Th is chapter completes our introduction to basic I/1. O programming and data structures in OpenCV. Th e following exercises build on this knowledge and create useful
utilities for later use.
a. Create a program that (1) reads frames from a video, (2) turns the result to grayscale,and (3) performs Canny edge detection on the image. Display all three stages of processing in three diff erent windows, with each window appropriately named for its function.
b. Display all three stages of processing in one image.
Hint: Create another image of the same height but three times the width as the video frame. Copy the images into this, either by using pointers or (more cleverly) by creating three new image headers that point to the beginning of and to one-third and two-thirds
 of the way into the imageData. Th en use cvCopy().
c. Write appropriate text labels describing the processing in each of the three slots.
a、Solution：
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
//Canny边缘检测算法
IplImage* doCanny(
	IplImage* in,
	double lowThresh,
	double highThresh,
	double aperture
	){
		if(in->nChannels != 1)return 0;
		IplImage* out = cvCreateImage(
			cvGetSize(in),
			IPL_DEPTH_8U,
			1);
		cvCanny(in,out,lowThresh,highThresh,aperture);
		return out;
}
int main(){
	CvCapture* capture = cvCreateFileCapture("test.avi");
	//显示原始视频
	cvNamedWindow("window");
	//显示灰色视频
	cvNamedWindow("window_gray");
	//显示边缘检测之后的灰色视频
	cvNamedWindow("window_Canny");
	IplImage* frame;
	//************注意：要初始化！
	IplImage* frame_gray = cvCreateImage(
		cvSize((int)cvGetCaptureProperty(
			capture,
			CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(
			capture,
			CV_CAP_PROP_FRAME_HEIGHT)),
		IPL_DEPTH_8U,
		1
		);
	IplImage* frame_Canny = cvCreateImage(
		cvSize((int)cvGetCaptureProperty(
			capture,
			CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(
			capture,
			CV_CAP_PROP_FRAME_HEIGHT)),
		IPL_DEPTH_8U,
		1
		);
	while(1){
		frame = cvQueryFrame(capture);
		if(!frame){
			break;
		}
		//将彩色图像转化为灰色图像
		cvCvtColor(frame,frame_gray,CV_RGB2GRAY);
		frame_Canny = doCanny(frame_gray,10,100,3);
		cvShowImage("window",frame);
		cvShowImage("window_gray",frame_gray);
		cvShowImage("window_Canny",frame_Canny);
		if(cvWaitKey(33) == 27){//每秒显示30帧
			break;
		}
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("window");
	cvDestroyWindow("window_gray");
	cvDestroyWindow("window_Canny");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/24/0_1314193796Cg4C.gif)
注意：在定义IplImage* frame_gray时，一定要记得用cvCreateImage初始化。否则会出现如下错误信息：
![](http://hi.csdn.net/attachment/201108/24/0_131419401445fz.gif)
b+c、Solution：
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
//Canny边缘检测算法
IplImage* doCanny(
	IplImage* in,
	int lowThresh,
	int highThresh,
	int aperture
	){
		if(in->nChannels != 1)return 0;
		IplImage* out = cvCreateImage(
			cvGetSize(in),
			IPL_DEPTH_8U,
			1);
		cvCanny(in,out,lowThresh,highThresh,aperture);
		return out;
}
int main(){
	CvCapture* capture = cvCreateFileCapture("test.avi");
	//把三个画面在一个窗口里面显示
	cvNamedWindow("window_all");
	//视频的每一帧
	IplImage* frame = NULL;
	frame = cvQueryFrame(capture);
	//视频帧图像的宽度和高度
	int width = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
	//定义两种，是为了使三种图像的通道数一样，便于一起显示
	//单通道灰度图像
	IplImage* gray = cvCreateImage(
		cvSize(width, height),
		IPL_DEPTH_8U,
		1);
	//3通道灰度图像
	IplImage* frame_gray = cvCreateImage(
		cvSize(width, height),
		IPL_DEPTH_8U,
		3);
	//单通道Canny边缘检测图像
	IplImage* Canny = cvCreateImage(
		cvSize(width, height),
		IPL_DEPTH_8U,
		1);
	//3通道Canny边缘检测图像
	IplImage* frame_Canny = cvCreateImage(
		cvSize(width, height),
		IPL_DEPTH_8U,
		3);
	//把3张图像放在一起显示
	IplImage* frame_all = cvCreateImage(
		cvSize(width * 3,height),
		IPL_DEPTH_8U,
		frame ->nChannels);
	cvZero(frame_all);
	//定义三个图像头
	IplImage* img1 = cvCreateImageHeader(cvSize(width, height),frame ->depth,3);
	IplImage* img2 = cvCreateImageHeader(cvSize(width, height),frame ->depth,3);
	IplImage* img3 = cvCreateImageHeader(cvSize(width, height),frame ->depth,3);
	img1 ->origin = frame ->origin;
	img1 ->widthStep = frame_all ->widthStep;
	img1->depth = frame->depth;
	img1 ->nChannels = 3;
	img2 ->origin = frame ->origin;
	img2 ->widthStep = frame_all ->widthStep;
	img2->depth = frame->depth;
	img2 ->nChannels = 3;
	img3 ->origin = frame ->origin;
	img3 ->widthStep = frame_all ->widthStep;
	img3->depth = frame->depth;
	img3 ->nChannels = 3;
	while(1){
		frame = cvQueryFrame(capture);
		if(!frame){
			return 1;
		}
		//将彩色图像转化为灰色图像
		cvCvtColor(frame,gray,CV_RGB2GRAY);
		cvCvtColor(gray,frame_gray,CV_GRAY2BGR);
		Canny = doCanny(gray,10,100,3);
		cvCvtColor(Canny,frame_Canny,CV_GRAY2BGR);
		img1 ->imageData = frame_all ->imageData;
		cvCopy(frame,img1);
		img2 ->imageData = frame_all ->imageData + frame ->widthStep;
		cvCopy(frame_gray,img2);
		img3 ->imageData = frame_all ->imageData + 2 * frame ->widthStep;
		cvCopy(frame_Canny,img3);
		CvFont textfont = cvFont(10.0,1);
		cvInitFont(&textfont, CV_FONT_HERSHEY_SIMPLEX, 0.5f, 0.5f, 0, 1);
		cvPutText(frame_all, "Frame", cvPoint(10,20), &textfont, cvScalar(0,0,255));
		cvPutText(frame_all, "Frame_Gray", cvPoint(width+10,20), &textfont, cvScalar(255,0,0));
		cvPutText(frame_all, "Frame_Canny", cvPoint(width*2+10, 20), &textfont, cvScalar(0,255,0));
		cvShowImage("window_all",frame_all);
		if(cvWaitKey(33) == 27){//每秒显示30帧
			break;
		}
	}
	cvReleaseImage(&frame);
	cvReleaseImage(&gray);
	cvReleaseImage(&frame_gray);
	cvReleaseImage(&Canny);
	cvReleaseImage(&frame_Canny);
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&img3);
	cvReleaseCapture(&capture);
	cvDestroyWindow("window_all");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/25/0_1314241306g4b4.gif)
2、 Create a program that reads in and displays an image. When the user’s mouse clicks on the image, read in the corresponding pixel (blue, green, red) values and write those values as text to the screen at the mouse location.
Solution：
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "stdio.h"
#include <iostream>
using namespace std;
char* str = (char*)malloc(30 * sizeof(char*));
bool draw = false;
CvPoint point;
CvFont font = cvFont(10.0,1);  
void my_mouse_callback(
					   int event,
					   int x,
					   int y,
					   int flags,
					   void* param){
	IplImage* img = (IplImage*)param;
	switch(event){
		case CV_EVENT_MOUSEMOVE:
			break;
		case CV_EVENT_LBUTTONDOWN:{
			draw = true;
			CvScalar pt = cvGet2D(img,y,x);
			double b = pt.val[0];
			double g = pt.val[1];
			double r = pt.val[2];
			memset(str,0,30);
			sprintf(str,"B:%.0f,G:%.0f,R:%.0f",b,g,r);
			cout<<"B:"<<b<<",G:"<<g<<",R:"<<r<<endl;
			point = cvPoint(x,y);
			cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5f, 0.5f, 0, 1); 
		}
			break;
		case CV_EVENT_LBUTTONUP:
			draw = false;
			break;
	}
}
int main(){
	IplImage* img = cvLoadImage("HUST.jpg");
	cvNamedWindow("image");
	IplImage* temp = cvCloneImage(img);
	cvSetMouseCallback("image",my_mouse_callback,(void*)img);
	while(1){
		cvCopyImage(img,temp);
		if(draw){
			cvPutText(temp,
				str,
				point,
				&font,
				cvScalar(255,0,0)
				);
		}
		cvShowImage("image",temp);
		if(cvWaitKey(30) == 27)break;
	}
	cvReleaseImage(&img);
	cvReleaseImage(&temp);
	cvDestroyWindow("image");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/25/0_13142748472ifK.gif)
a. For the program of exercise 1b, display the mouse coordinates of the individual image when clicking anywhere within the three-image display.
Solution：
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <iostream>
using namespace std;
char* str = (char*)malloc(30 * sizeof(char*));  
bool draw = false;  
CvPoint point;  
CvFont font = cvFont(10.0,1);  
void my_mouse_callback(  
                       int event,  
                       int x,  
                       int y,  
                       int flags,  
                       void* param){  
    IplImage* img = (IplImage*)param;  
    switch(event){  
        case CV_EVENT_MOUSEMOVE:  
            break;  
        case CV_EVENT_LBUTTONDOWN:{  
            draw = true;  
            memset(str,0,30);  
            sprintf(str,"x:%d,y:%d",x,y);  
            cout<<"x:"<<x<<",y:"<<y<<endl;  
            point = cvPoint(x,y);  
            cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5f, 0.5f, 0, 1);   
        }  
            break;  
        case CV_EVENT_LBUTTONUP:  
            draw = false;  
            break;  
    }  
}  
//Canny边缘检测算法
IplImage* doCanny(
	IplImage* in,
	int lowThresh,
	int highThresh,
	int aperture
	){
		if(in->nChannels != 1)return 0;
		IplImage* out = cvCreateImage(
			cvGetSize(in),
			IPL_DEPTH_8U,
			1);
		cvCanny(in,out,lowThresh,highThresh,aperture);
		return out;
}
int main(){
	CvCapture* capture = cvCreateFileCapture("test.avi");
	//把三个画面在一个窗口里面显示
	cvNamedWindow("window_all");
	//视频的每一帧
	IplImage* frame = NULL;
	frame = cvQueryFrame(capture);
	//视频帧图像的宽度和高度
	int width = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	int height = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
	//定义两种，是为了使三种图像的通道数一样，便于一起显示
	//单通道灰度图像
	IplImage* gray = cvCreateImage(
		cvSize(width, height),
		IPL_DEPTH_8U,
		1);
	//3通道灰度图像
	IplImage* frame_gray = cvCreateImage(
		cvSize(width, height),
		IPL_DEPTH_8U,
		3);
	//单通道Canny边缘检测图像
	IplImage* Canny = cvCreateImage(
		cvSize(width, height),
		IPL_DEPTH_8U,
		1);
	//3通道Canny边缘检测图像
	IplImage* frame_Canny = cvCreateImage(
		cvSize(width, height),
		IPL_DEPTH_8U,
		3);
	//把3张图像放在一起显示
	IplImage* frame_all = cvCreateImage(
		cvSize(width * 3,height),
		IPL_DEPTH_8U,
		frame ->nChannels);
	cvZero(frame_all);
	
	IplImage* temp = cvCloneImage(frame_all);  
	cvSetMouseCallback("window_all",my_mouse_callback,(void*)frame_all); 
	//定义三个图像头
	IplImage* img1 = cvCreateImageHeader(cvSize(width, height),frame ->depth,3);
	IplImage* img2 = cvCreateImageHeader(cvSize(width, height),frame ->depth,3);
	IplImage* img3 = cvCreateImageHeader(cvSize(width, height),frame ->depth,3);
	img1 ->origin = frame ->origin;
	img1 ->widthStep = frame_all ->widthStep;
	img1->depth = frame->depth;
	img1 ->nChannels = 3;
	img2 ->origin = frame ->origin;
	img2 ->widthStep = frame_all ->widthStep;
	img2->depth = frame->depth;
	img2 ->nChannels = 3;
	img3 ->origin = frame ->origin;
	img3 ->widthStep = frame_all ->widthStep;
	img3->depth = frame->depth;
	img3 ->nChannels = 3;
	while(1){
		frame = cvQueryFrame(capture);
		if(!frame){
			return 1;
		}
		//将彩色图像转化为灰色图像
		cvCvtColor(frame,gray,CV_RGB2GRAY);
		cvCvtColor(gray,frame_gray,CV_GRAY2BGR);
		Canny = doCanny(gray,10,100,3);
		cvCvtColor(Canny,frame_Canny,CV_GRAY2BGR);
		img1 ->imageData = frame_all ->imageData;
		cvCopy(frame,img1);
		img2 ->imageData = frame_all ->imageData + frame ->widthStep;
		cvCopy(frame_gray,img2);
		img3 ->imageData = frame_all ->imageData + 2 * frame ->widthStep;
		cvCopy(frame_Canny,img3);
		CvFont textfont = cvFont(10.0,1);
		cvInitFont(&textfont, CV_FONT_HERSHEY_SIMPLEX, 0.5f, 0.5f, 0, 1);
		cvPutText(frame_all, "Frame", cvPoint(10,20), &textfont, cvScalar(0,0,255));
		cvPutText(frame_all, "Frame_Gray", cvPoint(width+10,20), &textfont, cvScalar(255,0,0));
		cvPutText(frame_all, "Frame_Canny", cvPoint(width*2+10, 20), &textfont, cvScalar(0,255,0));
		
		
			cvCopyImage(frame_all,temp);  
			if(draw){  
				cvPutText(temp,  
					str,  
					point,  
					&font,  
					cvScalar(0,0,255)  
					);  
			}  
		
		cvShowImage("window_all",temp);
		if(cvWaitKey(33) == 27){//每秒显示30帧
			return 0;
		}
	}
	cvReleaseImage(&temp);
	cvReleaseImage(&frame);
	cvReleaseImage(&gray);
	cvReleaseImage(&frame_gray);
	cvReleaseImage(&Canny);
	cvReleaseImage(&frame_Canny);
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&img3);
	cvReleaseCapture(&capture);
	cvDestroyWindow("window_all");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/25/0_1314276202ek68.gif)
3. Create a program that reads in and displays an image.
a. Allow the user to select a rectangular region in the image by drawing a rectangle with the mouse button held down, and highlight the region when the mouse button is released. Be careful to save an image copy in memory so that your drawing into the image
 does not destroy the original values there. Th e next mouse click should start the process all over again from the original image.
solution：
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
CvRect rect;//矩形框
bool draw = false;//标记画的状态
void draw_rect(IplImage* img,CvRect rect){
	cvRectangle(img,
		cvPoint(rect.x,rect.y),
		cvPoint(rect.x + rect.width,rect.y + rect.height),
		cvScalar(0x00,0x00,0xff));
}
void my_mouse_callback(
					   int event,
					   int x,
					   int y,
					   int flags,
					   void* param){
	IplImage* img = (IplImage*)param;
	switch(event){
		case CV_EVENT_MOUSEMOVE:{
			if(draw){
				rect.width = x - rect.x;
				rect.height = y - rect.y;
			}					
		}
		break;
		case CV_EVENT_LBUTTONDOWN:{
			draw = true;
			rect = cvRect(x,y,0,0);
		}
		break;
		case CV_EVENT_LBUTTONUP:{
			draw = false;
			if(rect.width < 0){
				rect.x += rect.width;
				rect.width *= -1;
			}
			if(rect.height < 0){
				rect.y += rect.height;
				rect.height *= -1;
			}
			draw_rect(img,rect);
		}
		break;
	}
}
int main(){
	IplImage* img = cvLoadImage("HUST.jpg");
	rect = cvRect(-1,-1,0,0);
	cvNamedWindow("draw rect");
	//在内存保存副本
	IplImage* temp = cvCloneImage(img);
	cvSetMouseCallback("draw rect",
		my_mouse_callback,
		(void*)img
		);
	while(1){
		cvCopyImage(img,temp);
		if(draw){
			draw_rect(temp,rect);
		}
		cvShowImage("draw rect",temp);
		if(cvWaitKey(15) == 27)break;
	}
	cvReleaseImage(&img);
	cvReleaseImage(&temp);
	cvDestroyWindow("draw rect");
}
```
Test：
![](http://hi.csdn.net/attachment/201108/26/0_1314329006noGq.gif)
b. In a separate window, use the drawing functions to draw a graph in blue, green,and red for how many pixels of each value were found in the selected box. This is the color histogram of that color region. Th e x-axis should be eight bins that represent
 pixel values falling within the ranges 0–31, 32–63, . . ., 223–255. The y-axis should be counts of the number of pixels that were found in that bin range. Do this for each color channel, BGR.
solution:
```cpp
#include "opencv\cv.h"
#include "opencv\highgui.h"
CvRect rect;//矩形框
bool draw = false;//标记画矩形框的状态
bool draw_hist = false;//鼠标左键弹起开始画直方图
//绘制直方图的函数
void draw_rect(IplImage* img,CvRect rect){
	cvSetImageROI(img,rect);
	IplImage* src= cvCreateImage(
		cvSize(rect.width,rect.height),
		IPL_DEPTH_8U,
		3);
	cvCopy(img,src);
	cvResetImageROI(img);
	IplImage* r_img = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* g_img = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* b_img = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	IplImage* gray_img = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
	//分离R,G,B分量
	cvSplit(src,r_img,g_img,b_img,NULL);
	//灰度转换
	cvCvtColor(src,gray_img,CV_BGR2GRAY);
	int size = 256;
	float range[] = {0,255};
	float* ranges[] = {range};
	//创建直方图
	CvHistogram* r_hist = cvCreateHist(1,&size,CV_HIST_ARRAY,ranges,1);
	CvHistogram* g_hist = cvCreateHist(1,&size,CV_HIST_ARRAY,ranges,1);
	CvHistogram* b_hist = cvCreateHist(1,&size,CV_HIST_ARRAY,ranges,1);
	CvHistogram* gray_hist = cvCreateHist(1,&size,CV_HIST_ARRAY,ranges,1);
	//红色分量直方图
	cvCalcHist(&r_img,r_hist,0,NULL);
	IplImage* r_dst = cvCreateImage(cvSize(400,300),IPL_DEPTH_8U,3);
	cvSet(r_dst,cvScalarAll(255),0);
	float r_max = 0;
	cvGetMinMaxHistValue(r_hist,NULL,&r_max,NULL,NULL);
	double r_bin_width = (double)r_dst ->width / size;
	double r_bin_unith = (double)r_dst ->height / r_max;//高度比例
	for(int i = 0;i < size;++i){
		//获得矩形左上角和右下角坐标
		CvPoint p0 = cvPoint(i * r_bin_width,r_dst ->height);
		CvPoint p1 = cvPoint((i + 1) * r_bin_width,
			r_dst ->height - cvGetReal1D(r_hist ->bins,i) * r_bin_unith);
		cvRectangle(r_dst,p0,p1,cvScalar(255,0,0),-1,8,0);//-1表示画实心矩形
	}
	//绿色分量直方图
	cvCalcHist(&g_img,g_hist,0,NULL);
	IplImage* g_dst = cvCreateImage(cvSize(400,300),IPL_DEPTH_8U,3);
	cvSet(g_dst,cvScalarAll(255),0);
	float g_max = 0;
	cvGetMinMaxHistValue(g_hist,NULL,&g_max,NULL,NULL);
	double g_bin_width = (double)g_dst ->width / size;
	double g_bin_unith = (double)g_dst ->height / g_max;//高度比例
	for(int i = 0;i < size;++i){
		//获得矩形左上角和右下角坐标
		CvPoint p0 = cvPoint(i * g_bin_width,g_dst ->height);
		CvPoint p1 = cvPoint((i + 1) * g_bin_width,
			g_dst ->height - cvGetReal1D(g_hist ->bins,i) * g_bin_unith);
		cvRectangle(g_dst,p0,p1,cvScalar(0,255,0),-1,8,0);//-1表示画实心矩形
	}
	//蓝色分量直方图
	cvCalcHist(&b_img,b_hist,0,NULL);
	IplImage* b_dst = cvCreateImage(cvSize(400,300),IPL_DEPTH_8U,3);
	cvSet(b_dst,cvScalarAll(255),0);
	float b_max = 0;
	cvGetMinMaxHistValue(b_hist,NULL,&b_max,NULL,NULL);
	double b_bin_width = (double)b_dst ->width / size;
	double b_bin_unith = (double)b_dst ->height / b_max;//高度比例
	for(int i = 0;i < size;++i){
		//获得矩形左上角和右下角坐标
		CvPoint p0 = cvPoint(i * b_bin_width,b_dst ->height);
		CvPoint p1 = cvPoint((i + 1) * b_bin_width,
			b_dst ->height - cvGetReal1D(b_hist ->bins,i) * b_bin_unith);
		cvRectangle(b_dst,p0,p1,cvScalar(0,0,255),-1,8,0);//-1表示画实心矩形
	}
	//灰度图直方图
	cvCalcHist(&gray_img,gray_hist,0,NULL);
	IplImage* gray_dst = cvCreateImage(cvSize(400,300),IPL_DEPTH_8U,3);
	cvSet(gray_dst,cvScalarAll(255),0);
	float gray_max = 0;
	cvGetMinMaxHistValue(gray_hist,NULL,&gray_max,NULL,NULL);
	double gray_bin_width = (double)gray_dst ->width / size;
	double gray_bin_unith = (double)gray_dst ->height / gray_max;//高度比例
	for(int i = 0;i < size;++i){
		//获得矩形左上角和右下角坐标
		CvPoint p0 = cvPoint(i * gray_bin_width,gray_dst ->height);
		CvPoint p1 = cvPoint((i + 1) * gray_bin_width,
			gray_dst ->height - cvGetReal1D(gray_hist ->bins,i) * gray_bin_unith);
		cvRectangle(gray_dst,p0,p1,cvScalar(0),-1,8,0);//-1表示画实心矩形
	}
	//把四个直方图在一幅图片上显示出来
	IplImage* dst = cvCreateImage(cvSize(800,600),8,3);
	cvSetZero(dst);
	//拷贝红色分量直方图
	CvRect rect = cvRect(0,0,400,300);
	cvSetImageROI(dst,rect);
	cvCopy(r_dst,dst);
	//拷贝绿色分量直方图
	 rect = cvRect(400,0,400,300);
	cvSetImageROI(dst,rect);
	cvCopy(g_dst,dst);
	//拷贝蓝色分量直方图
	rect = cvRect(0,300,400,300);
	cvSetImageROI(dst,rect);
	cvCopy(b_dst,dst);
	//拷贝灰度图分量直方图
	rect = cvRect(400,300,400,300);
	cvSetImageROI(dst,rect);
	cvCopy(gray_dst,dst);
	cvResetImageROI(dst);
	cvNamedWindow("src",1);
	cvShowImage("src",src);
	cvNamedWindow("dst",1);
	cvShowImage("dst",dst);
	cvSaveImage("dst.jpg",dst);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&r_img);
	cvReleaseImage(&g_img);
	cvReleaseImage(&b_img);
	cvReleaseImage(&gray_img);
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseImage(&r_dst);
	cvReleaseImage(&g_dst);
	cvReleaseImage(&b_dst);
		
}
//我的鼠标响应函数
void my_mouse_callback(
					   int event,
					   int x,
					   int y,
					   int flags,
					   void* param){
	IplImage* img = (IplImage*)param;
	switch(event){
		case CV_EVENT_MOUSEMOVE:{
			if(draw){
				rect.width = x - rect.x;
				rect.height = y - rect.y;
			}
			draw_hist = false;
		}
		break;
		case CV_EVENT_LBUTTONDOWN:{
			draw = true;
			rect = cvRect(x,y,0,0);
			draw_hist = false;
		}
		break;
		case CV_EVENT_LBUTTONUP:{
			draw = false;
			draw_hist = true;
			if(rect.width < 0){
				rect.x += rect.width;
				rect.width *= -1;
			}
			if(rect.height < 0){
				rect.y += rect.height;
				rect.height *= -1;
			}
			draw_rect(img,rect);
		}
		break;
	}
}
int main(){
	IplImage* img = cvLoadImage("HUST.jpg");
	rect = cvRect(-1,-1,0,0);
	cvNamedWindow("draw rect");
	//在内存保存副本
	IplImage* temp = cvCloneImage(img);
	cvSetMouseCallback("draw rect",
		my_mouse_callback,
		(void*)img
		);
	while(1){
		cvCopyImage(img,temp);
		if(draw_hist){
			draw_rect(temp,rect);
		}
		cvShowImage("draw rect",temp);
		if(cvWaitKey(15) == 27)break;
	}
	cvReleaseImage(&img);
	cvReleaseImage(&temp);
	cvDestroyWindow("draw rect");
}
```
Test:
![](http://hi.csdn.net/attachment/201108/30/0_1314689554lsnO.gif)

# [zt]OpenCV如何获取视频当前的一帧图像 - Grandyang - 博客园







# [[zt]OpenCV如何获取视频当前的一帧图像](https://www.cnblogs.com/grandyang/p/4012475.html)





（OpenCV读取视频、OpenCV提取视频每一帧、每一帧图片合成新的AVI视频）CvCapture 是视频获取结构 被用来作为视频获取函数的一个参数 比如 CvCapture* cap; IplImage* cvQueryFrame( cap ); 从摄像头或者文件中抓取并返回一帧————————————————————————

**Opencv读取视频代码**





- 
#include "stdafx.h"

- 
#include"highgui.h"

- 


- 
int main(int argc,char* argv[])  

- 
{  

- 
                cvNamedWindow( "avi");  

- 
                CvCapture* capture = cvCreateFileCapture( "D:\\sample.avi");  

- 


- 
                IplImage* frame;  

- 


- 
while(1)  

- 
                {  

- 
                                frame = cvQueryFrame(capture);  

- 
if(!frame) break ;  

- 


- 
                                cvShowImage( "avi",frame);  

- 
char c = cvWaitKey(33);  

- 


- 
if(c == 27)  

- 
break;  

- 
                }  

- 
                cvReleaseCapture(&capture);  

- 


- 
                cvDestroyWindow( "avi");  

- 
return 0;  

- 
}  










**结构体****CvCapture**




  CvCapture 是一个结构体，用来保存图像捕获所需要的信息。 opencv提供两种方式从外部捕获图像：

一种是从摄像头中，

一种是通过解码视频得到图像。

两种方式都必须从第一帧开始一帧一帧的按顺序获取，因此每获取一帧后都要保存相应的状态和参数。

比如从视频文件中获取，需要保存视频文件的文件名，相应的解码器类型，下一次如果要

获取将需要解码哪一帧等。 这些信息都保存在CvCapture结构中，每获取一帧后，这些信息

都将被更新，获取下一帧需要将新信息传给获取的 api接口




**cvCreateFileCapture(char*name)**





通过输入要读取的avi文件的路径，然后，该函数返回一个指向 CvCapture结构体的指针。







**cvQueryFrame(capture)**





输入一个CvCapture类型的指针，该函数主要功能是将视频文件的下一帧加载到内存。与cvLoadImage的不同之处是，该函数不重新分配内存空间。





**C=cvWaitKey(33)**


当前帧被显示后，等待 33毫秒。如果用户触发了一个按键， c会被设置成这个按键的 ASCII码，否则会被设置成 -1。

cvWaitKey(33) 在此处的另外一个作用是，控制帧率。


**cvReleaseCapture(&capture)**


释放为 CvCapture结构体开辟的内存空间

关闭打开的 AVI文件相关的文件句柄




**读取摄像头**




只需把 **cvCreate****File****Capture****改成cvCreate****Camera****Capture即可。**

该函数的输入参数是一个 ID号，只有存在多个摄像头时才起作用。当 ID=-1时，表示

随机选择一个。 HighGUI做了很多工作，使得摄像机图像序列像一个视频文件一样。






**常见问题**



**cvCreateFileCapture返回空的问题**




- 
（ 1）视频文件路径没写对

- 
（ 2）没有安装解码器

- 
（ 3）如果使用的是 Opencv2.0或更高版本，那么，能否正确加载 opencv_ffmpeg210.dll

- 
（ 4）尽管是 AVI文件，但也可能使用了某种 codec，例如 :MJPEG Decompressor。 需要把它转换 OpenCV支持的 AVI文件 . OpenCV支持的AVI。例如使用狸窝全能视频转换器，在《预置方案》处，选择 AVI-Audio_Video Interleaved(*.avi)。或者使用格式工厂也可以。

- 
（ 5）读摄像头数据，需要安装与摄像头相应的驱动程序。




————————————————————

**cvQueryFrame**

OpenCV中一个函数cvQueryFrame

从摄像头或者文件中抓取并返回一帧

IplImage* cvQueryFrame( CvCapture* capture );

capture

视频获取结构。

函数cvQueryFrame从摄像头或者文件中抓取一帧，然后解压并返回这一帧。 这个函数仅仅是函数cvGrabFrame和 函数cvRetrieveFrame在一起调用的组合。 返回的图像不可以被用户释放或者修改。

cvQueryFrame的参数为CvCapture结构的指针。用来将下一帧视频文件载入内存，返回一个对应当前帧的指针。与cvLoadImage不同的是cvLoadIamge为图像分配内存空间，而cvQueryFrame使用已经在cvCapture结构中分配好的内存。这样的话，就没有必要通过cvReleaseImage()对这个返回的图像指针进行释放，当CvCapture结构被释放后，每一帧图像所对应的内存空间即会被释放。

________________________________________________________________________________

opencv提取视频每一帧


- 
// test3.cpp

- 
//

- 
// 该程序实现视频和图片的相互转换.

- 
// Image_to_video()函数将一组图片合成AVI视频文件.

- 
// Video_to_image()函数将AVI视频文件读入，将每一帧存储为jpg文件.

- 
//

- 
////////////////////////////////////////////////////////////////////////

- 
#include <stdlib.h>

- 
#include <stdio.h>

- 
#include <math.h>

- 
#include <cv.h>

- 
#include <highgui.h>

- 
#define NUM_FRAME 300 //只处理前300帧，根据视频帧数可修改

- 


- 
void Video_to_image(char* filename)  

- 
{  

- 
printf("------------- video to image ... ----------------n");  

- 
//初始化一个视频文件捕捉器

- 
CvCapture* capture = cvCaptureFromAVI(filename);  

- 
//获取视频信息

- 
cvQueryFrame(capture);  

- 
int frameH    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);  

- 
int frameW    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);  

- 
int fps       = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);  

- 
int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);  

- 
printf("tvideo height : %dntvideo width : %dntfps : %dntframe numbers : %dn", frameH, frameW, fps, numFrames);  

- 
//定义和初始化变量

- 
int i = 0;  

- 
IplImage* img = 0;  

- 
char image_name[13];  

- 


- 
cvNamedWindow( "mainWin", CV_WINDOW_AUTOSIZE );  

- 
//读取和显示

- 
while(1)  

- 
{  

- 


- 
   img = cvQueryFrame(capture); //获取一帧图片

- 
   cvShowImage( "mainWin", img ); //将其显示

- 
char key = cvWaitKey(20);  

- 


- 
   sprintf(image_name, "%s%d%s", "image", ++i, ".jpg");//保存的图片名

- 


- 
   cvSaveImage( image_name, img);   //保存一帧图片

- 


- 
if(i == NUM_FRAME) break;  

- 
}  

- 
cvReleaseCapture(&capture);  

- 
cvDestroyWindow("mainWin");  

- 
}  

- 


- 
void Image_to_video()  

- 
{  

- 
int i = 0;  

- 
IplImage* img = 0;  

- 
char image_name[13];  

- 
printf("------------- image to video ... ----------------n");  

- 
//初始化视频编写器，参数根据实际视频文件修改

- 
CvVideoWriter *writer = 0;  

- 
int isColor = 1;  

- 
int fps     = 30; // or 25

- 
int frameW = 400; // 744 for firewire cameras

- 
int frameH = 240; // 480 for firewire cameras

- 
writer=cvCreateVideoWriter("out.avi",CV_FOURCC('X','V','I','D'),fps,cvSize(frameW,frameH),isColor);  

- 
printf("tvideo height : %dntvideo width : %dntfps : %dn", frameH, frameW, fps);  

- 
//创建窗口

- 
cvNamedWindow( "mainWin", CV_WINDOW_AUTOSIZE );  

- 
while(i<NUM_FRAME)  

- 
{  

- 
   sprintf(image_name, "%s%d%s", "image", ++i, ".jpg");  

- 
   img = cvLoadImage(image_name);  

- 
if(!img)  

- 
   {  

- 
    printf("Could not load image file...n");  

- 
    exit(0);  

- 
   }  

- 
   cvShowImage("mainWin", img);  

- 
char key = cvWaitKey(20);  

- 
   cvWriteFrame(writer, img);  

- 
}  

- 
cvReleaseVideoWriter(&writer);  

- 
cvDestroyWindow("mainWin");  

- 
}  

- 


- 
int main(int argc, char *argv[])  

- 
{  

- 
char filename[13] = "infile.avi";  

- 
Video_to_image(filename); //视频转图片

- 
Image_to_video();    //图片转视频

- 
return 0;  

- 
}  
















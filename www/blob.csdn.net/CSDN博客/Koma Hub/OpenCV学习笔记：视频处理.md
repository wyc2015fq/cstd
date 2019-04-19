# OpenCV学习笔记：视频处理 - Koma Hub - CSDN博客
2018年05月04日 15:49:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：100
***环境***：CentOS7
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
```
$ pkg-config --modversion opencv
2.4.13
```
测试所用视频是Opencv自带的avi测试视频，文件路径是：opencv-3.4.0\sources\samples\data\vtest.avi
如果找不到下载也可以到[https://download.csdn.net/download/rong_toa/10392225](https://download.csdn.net/download/rong_toa/10392225)下载。
***源代码：***
```cpp
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<cv.h>
#include<highgui.h>
#include<iostream>
using namespace std;
int main(int argc,char**argv)
{
/******初始化一个摄像头捕捉器******/
    //CvCapture* capture1 = cvCaptureFromCAM(0);
/*****初始化一个视频文件捕捉器*****/
    CvCapture* capture = cvCaptureFromAVI("vtest.avi");
    /**/
    IplImage* img = 0;
    if(!cvGrabFrame(capture)){ // capture a frame
        printf("Could not grab a frame\n\7");
        exit(0);
    }
    img=cvRetrieveFrame(capture);
    cvSaveImage("out.jpg",img);
/******获取/设置视频流信息*****/
    /*获取视频流设备信息*/
    cvQueryFrame(capture); // 在读取视频流信息前，要先执行此操作
    int frameH = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
    int frameW = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
    int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    int numFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
    /*获取帧图信息*/
    float posMsec = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_MSEC);
    int posFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
    float posRatio = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_AVI_RATIO);
    /*设置从视频文件抓取的第一帧画面的位置*/
    //cvSetCaptureProperty(capture, CV_CAP_PROP_POS_AVI_RATIO, (double)0.9);
    img=cvRetrieveFrame(capture);
    cvSaveImage("out2.jpg",img);
/******保存视频文件******/
    /*初始化视频编写器*/
    CvVideoWriter *writer = 0;
    int isColor = 1;
    //fps = 25; // or 30
    //frameW = 640; // 744 for firewire cameras
    //frameH = 480; // 480 for firewire cameras
    writer=cvCreateVideoWriter("out.avi",CV_FOURCC('D', 'I', 'V', 'X'),
                                fps,cvSize(frameW,frameH),isColor);
    /*保存视频文件*/
    IplImage* img1 = 0;
    int nFrames = 80, key;
    for(int i=0;i<nFrames;i++){
        cvGrabFrame(capture); // capture a frame
        img1=cvRetrieveFrame(capture); // retrieve the captured frame
        // img = cvQueryFrame(capture);
        cvShowImage("mainWin", img);
        key=cvWaitKey(40); // wait 20 ms
        cvWriteFrame(writer,img1); // add the frame to the file
    }
    /*释放视频流捕捉器*/
    cvReleaseCapture(&capture);
    /*释放视频编写器*/
    cvReleaseVideoWriter(&writer);
    return 0;
}
```
***编译与运行：***
```
$ make
g++ main.cpp `pkg-config --cflags --libs opencv`
$ ./a.out
```
运行后会跳出一个窗口播放一段视频，运行结束后会保存两个视频截图（此处给出一个示例）。
![](https://img-blog.csdn.net/20180504154854708)
更多信息请见：《OpenCV中文参考手册》

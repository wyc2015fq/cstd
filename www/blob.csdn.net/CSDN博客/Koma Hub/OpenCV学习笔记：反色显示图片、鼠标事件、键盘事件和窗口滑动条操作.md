# OpenCV学习笔记：反色显示图片、鼠标事件、键盘事件和窗口滑动条操作 - Koma Hub - CSDN博客
2018年05月03日 22:10:37[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：133
环境CentOS7
```
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
$ pkg-config --modversion opencv
2.4.13
```
参照《OpenCV中文参考手册》
***源代码：***
```cpp
/*************************
 *
 * main.cpp
 *
 *    读入图片并反色显示 2018.05
 *
 * compile：g++ main.cpp `pkg-config --cflags --libs opencv`
 */
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<cv.h>
#include<highgui.h>
/*处理鼠标事件*/
void mouseHandler(int event, int x, int y, int flags, void* param)
/*flags: 
CV_EVENT_FLAG_CTRLKEY, 
CV_EVENT_FLAG_SHIFTKEY, 
CV_EVENT_FLAG_ALTKEY,
CV_EVENT_FLAG_LBUTTON, 
CV_EVENT_FLAG_RBUTTON, 
CV_EVENT_FLAG_MBUTTON*/
{
    printf("%d,%d,%d,%d\n",event,x,y,flags);
    switch(event){
        /*鼠标左键按下*/
        case CV_EVENT_LBUTTONDOWN:
            printf("Left button down\n");
            break;
        /*鼠标左键弹上*/
        case CV_EVENT_LBUTTONUP:
            printf("Left button up\n");
            break;
        /*鼠标左键*/
        case CV_EVENT_LBUTTONDBLCLK:
            printf("Left button DBLCLK\n");
            break;
        /*鼠标右键按下*/
        case CV_EVENT_RBUTTONDOWN:
            printf("Right button down\n");
            break;
        /*鼠标右键弹上*/
        case CV_EVENT_RBUTTONUP:
            printf("Right button up\n");
            break;
        /*鼠标右键*/
        case CV_EVENT_RBUTTONDBLCLK:
            printf("Right button DBLCLK\n");
            break;
        /*鼠标中键按下*/
        case CV_EVENT_MBUTTONDOWN:
            printf("Middle button down\n");
            break;
        /*鼠标中键弹上*/
        case CV_EVENT_MBUTTONUP:
            printf("Middle button up\n");
            break;
        /*鼠标中键*/
        case CV_EVENT_MBUTTONDBLCLK:
            printf("Middle button DBLCLK\n");
            break;
        /*鼠标移动*/
        case CV_EVENT_MOUSEMOVE:
            printf("Mouse move\n");
    }
}
/*滑动条处理程序*/
void trackbarHandler(int pos)
{
    printf("Trackbar position: %d\n",pos);
}
int main(int argc,char**argv)
{
    /*图片变量*/
    IplImage *img = 0;
    int height, width, step, channels;
    uchar *data;
    int i, j, k;
    if(argc<2){
        printf("Usage: main <image-file-name>\n\7");
        exit(0);
    }
    /*载入图像*/
    img = cvLoadImage(argv[1]);
    if(!img){
        printf("Could not load image file: %s\n",argv[1]);
        exit(0);
    }
    /*获取图像参数*/
    height = img->height;
    width  = img->width;
    step   = img->widthStep;
    channels= img->nChannels;
    data   = (uchar*)img->imageData;
    /*显示图片*/
    //cvShowImage("mainWin0",img);
    printf("Processing a %dx%d image with %d channels\n",height,width,channels);
    
    /*创建一个窗口，并且自动控制大小*/
    cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
    /*控制窗口大小*/
    cvMoveWindow("mainWin", 100, 100);
    /*处理图像：随意你怎么处理*/
    for(i=0;i<height;i++) 
        for(j=0;j<width;j++) 
            for(k=0;k<channels;k++)
                    /*反色*/
                data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
    /*显示图片*/
    cvShowImage("mainWin", img );
    /*鼠标事件*/
    int mouseParam = 0;
    cvSetMouseCallback("mainWin",mouseHandler,&mouseParam);
    
    /*键盘事件*/
    int key;
    while(1){
        key = cvWaitKey(0);
        printf("key=%d\n",key);
        /*Esc == 27*/
        if(key == 27)break;
        switch(key){
            case 'h':printf("press h\n");
                     break;
            case 'a':printf("press a\n");
                     break;
            case 'b':printf("press b\n");
                     break;
            case 'c':printf("press c\n");
                     break;
            case 'd':printf("press d\n");
                     break;
            default: break;
        }
    }
    /*重新调整窗口大小（按照窗口名调整：只调整这个名字的窗口）*/
    cvResizeWindow("mainWin",100,100);
    /*处理滑动条事件*/
    int trackbarVal = 25;
    int maxVal = 200;
    cvCreateTrackbar("bar1","mainWin",&trackbarVal, maxVal, trackbarHandler);
    //获得当前滑动条位置
    int pos = cvGetTrackbarPos("bar1","mainWin");
    printf("pos = %d\n",pos);
    //设置滑动条位置
    cvSetTrackbarPos("bar1","mainWin",25);
    /*释放这个图片*/
    cvReleaseImage(&img );
    /*关闭窗口*/
    cvDestroyWindow("mainWin");
    return 0;
}
```
***编译与运行：***
```
$g++ main.cpp `pkg-config --cflags --libs opencv`
$ ./a.out wong.jpg 
Processing a 301x286 image with 3 channels
Left button down
Left button up
Right button down
Right button up
Middle button down
Middle button up
key=97
press a
key=98
press b
key=99
press c
key=27
pos = 25
```
***所用图片：***
![](https://img-blog.csdn.net/20180503221017165)

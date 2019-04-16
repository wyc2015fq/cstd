# OpenCV基本函数学习 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月02日 13:28:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：61









##  OpenCV开发包提供了许多功能，诸如读取各种类型的图像文件、视频内容以及摄像机输入的功能，这些功能是OpenCV开发包所包含的HighGUI工具集的一部分，为使用这些功能，首先要了解一些简单的程序所使用的函数及其功能。



### 1.显示图像

以最简单的显示图像程序为例，可以认识几个基础的函数及其作用。
 cvNamedWindow()是一个高层调用接口，用于在屏幕上创建一个窗口，函数中的第一个参数指定了该窗口的窗口标题，若需要使用其他函数与该窗口进行交互时，即使用该参数值引用这个窗口。函数中的第二个参数定义了窗口的属性，该参数可以被设置为0（默认值）或CV_WINDOW_AUTOSIZE。设置为0时，窗口的大小不会因加载的图像或视频的大小而改变，图像与视频只能在窗口中根据窗口的大小进行拉伸或缩放；而设置为CV_WINDOW_AUTOSIZE时，窗口会根据图像视频的实际大小自动进行拉伸或缩放以进行容纳。。
 cvLoadImage()函数也是一个高层调用接口，它通过文件名确定被加载文件的格式，并且该函数将自动分配图像数据结构所需的内存。
 cvShowImage()函数可以用来显示图像，其第一个参数被设置来确定在哪个已经存在的窗口中显示图像，前提是有一个与某个图像文件相对应的IplImage*类型的指针。
 cvWaitKey()函数的功能是使程序暂停，等待用户触发一个按键操作。如果将该函数参数设为一个正数，则程序将暂停一段时间，时间长度为该整数值个毫秒单位，然后继续执行程序，即使用户没有按下任何按键。当设置该函数参数为0或者负数时，程序将一直等待用户触发按键操作。
 cvReleaseImage()函数在用完加载到内存的文件后可以用来释放为文件分配的内存，在对cvReleaseImage()函数的调用执行完毕后，img指针将被设置为空。
 cvDestoryWindow()函数用来关闭窗口，同时释放为该窗口所分配的所有内存。

```cpp
#include"highgui.h"

int main( int argc, char**argv )
{
  IplImage* img = cvLoadImage( argv[1] );
  cvNamedWindow("Example1",CV_WINDOW_AUTOSIZE );
  cvShowImage("Example1", img );
  cvWaitKey(0);
  cvReleaseImage( &img );
  cvDestroyWindow("Example1");
}
```



### 2.播放AVI视频

通过播放AVI视频的程序，来学习以下两个函数的功能。
 cvCreateFileCapture()通过参数设置确定要读入的AVI文件，返回一个指向CvCapture结构的指针，其中包含状态信息。在调用这个函数之后，返回指针所指向的CvCapture结构被初始化到所对应AVI文件的开头。
 cvQueryFrame()函数的参数为CvCapture结构的指针，用来将下一帧视频文件载入内存（实际是填充或更新CvCapture结构中），返回一个对应当前帧的指针。与cvLoadImage()不同的是，cvLoadImage为图像分配内存空间，而cvQueryFrame使用已经在cvCapture结构中分配好的内存。这样，就不需要cvReleaseImage()对这个返回的图像指针进行释放，当CvCapture结构被释放后，每一帧图像所对应的内存空间即会被释放。

```cpp
#include"highgui.h"

int main( int argc, char**argv ) { 
    cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE);
    //CvCapture* capture = cvCaptureFromAVI(argv[1] ); // either one will work
    CvCapture* capture = cvCreateFileCapture(argv[1] );
    IplImage* frame;
    while(1) {
        frame = cvQueryFrame( capture );
        if( !frame ) break;
        cvShowImage( "Example2",frame );
        char c = cvWaitKey(33);
        if( c == 27 ) break;
    }
    cvReleaseCapture( &capture );
    cvDestroyWindow( "Example2" );
}
```





### 3.从摄像机读入数据

为了实现系统，第一步就是要从摄像头读入数据。OpenCV中的HighGUI模块为我们提供了一种简单的方式来处理这种情况，类似于读取AVI文件，循环的顺序的读出视频中的每一帧，不同的是，我们调用的是cvCreateCameraCapture()来代替cvCreateFileCapture()，后面一个函数参数为摄像设备的ID而不是AVI文件的名称，在存在多个摄像设备时这个参数才起作用，其默认值为-1，代表“随机选择一个”，更适合当有且仅有一个摄像设备的情况。cvCreateCameraCapture()同样返回相同的CvCapture*指针，这使得我们可以使用完全类似于从视频流中获取帧的方法。

```cpp
#include"stdafx.h"
#include<opencv2/opencv.hpp>

int main(int argc, char*argv[])
{
cvNamedWindow("Example2",CV_WINDOW_AUTOSIZE);
CvCapture *capture=cvCreateCameraCapture(0);
IplImage *frame;
while(1){
       frame=cvQueryFrame(capture);
       if(!frame) break;
       cvShowImage("Example2",frame);
       char c=cvWaitKey(33);
       if(c==27) break;
}
cvReleaseCapture(&capture);
cvDestroyWindow("Example2");
       return 0;
}
```



### 4.几个基本函数

cvCopy()用于将一个图像复制到另一个图像，这个函数要求两个数组具有相同的数据类型、相同的大小和相同的维数，可以用来复制稀疏矩阵，但这样做时，不支持mask，对于非稀疏矩阵和图像，mask如果为空，则只对与mask中与非0值相对应的dst中的像素赋值。
 cvCvtColor()当数据类型一致时，它将图像从一个颜色空间（通道的数值）转换到另一个，具体的转换操作由参数code来指定，如CV_RGB2GRAY就是转换RGB或BGR色彩空间为灰度空间。
 cvGetSize()是专门为矩阵和图像设计的，这两种对象的维数总是2.其尺寸可以一CvSize结构的形式返回，例如当创建一个新的大小相同的矩阵或图像时，使用此函数就很方便。
 cvSet()和cvSetZero()能将数组的所有通道的所有值设置为指定的参数value。该cvSet()函数接受一个可选的参数：如果提供参数，那么只有那些与参数mask中非0值对应的像素将被设置为指定的值。函数cvSetZero()仅仅是cvSet（0.0）别名。
 cvLine()是绘图函数中最简单的。属性分别为图像、线段的第一个端点、线段的第二个端点、线段的颜色、线段的粗细程度、线段的类型。
 cvPutText()是OpenCV中一个主要的函数，可以在图像上输出一些文字。参数img表示图像，text所指向的文字将打印到图像上。origin指定文本框左下角的坐标，font表示字体结构体，color表示文本的字体颜色。
 为了降低输出文字工作的复杂度，我们需要使用CvFont指针。简单来说，获取CvFont*指针的方式就是调用cvInitFont()。该函数采用一组参数配置一些用于屏幕输出的基本个特定字体。为了建立一个可以传值给cvPutText()的CvFont，首先必须声明一个CvFont变量，然后把它传递给cvInitFont()。调用cvInitFont()时，初始化一个已经准备好的CvFont结构(这意味着你创建了一个变量，并传给cvInitFont()函数一个指向新建的变量指针)，而不是像cvCreateImage()那样创建一个结构并返回指针。





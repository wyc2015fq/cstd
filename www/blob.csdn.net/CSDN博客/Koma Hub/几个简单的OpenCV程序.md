# 几个简单的OpenCV程序 - Koma Hub - CSDN博客
2018年04月28日 11:23:38[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1879
OpenCV官网：[https://opencv.org/](https://opencv.org/)
[百度百科](https://baike.baidu.com/item/opencv/10320623?fr=aladdin)
OpenCV是一个基于BSD许可（开源）发行的跨平台计算机视觉库，可以运行在Linux、Windows、Android和Mac OS操作系统上。它轻量级而且高效——由一系列 C 函数和少量 C++ 类构成，同时提供了Python、Ruby、MATLAB等语言的接口，实现了图像处理和计算机视觉方面的很多通用算法。
OpenCV用C++语言编写，它的主要接口也是C++语言，但是依然保留了大量的C语言接口。该库也有大量的Python、Java and MATLAB/OCTAVE（版本2.5）的接口。这些语言的API接口函数可以通过在线文档获得。如今也提供对于C#、Ch、Ruby的支持。
所有新的开发和算法都是用C++接口。一个使用CUDA的GPU接口也于2010年9月开始实现。
下面转载几个OpenCV的程序。
我的环境：
```
$ pkg-config --modversion opencv
2.4.13
$ uname -a
Linux toa 3.10.0-693.17.1.el7.x86_64 #1 SMP Thu Jan 25 20:13:58 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux
$ g++ --version
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
```
***1.显示图片[https://www.cnblogs.com/Crysaty/p/6638152.html](https://www.cnblogs.com/Crysaty/p/6638152.html)***
```cpp
#include <highgui.h>  
   
int main(int argc,char ** argv)
{  
    IplImage* img = cvLoadImage(argv[1],CV_LOAD_IMAGE_COLOR);  
    cvNamedWindow("Image_show",CV_WINDOW_AUTOSIZE);  
    cvShowImage("Image_show",img);  
    cvWaitKey(0);  
    cvReleaseImage(&img);  
    cvDestroyWindow("Image_show");  
     return 0;  
}
```
编译运行：
```
$ make
gcc main.c `pkg-config --cflags --libs opencv`
$ ./a.out ../windows.png
```
效果图（左），原图（右）：
![](https://img-blog.csdn.net/20180428103322468)![](https://img-blog.csdn.net/20180428103331838)
一个C++的代码：
```cpp
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main(int argc, char **argv)  
{  
    Mat imggray=imread(argv[1],CV_LOAD_IMAGE_COLOR);  
    cvtColor(imggray,imggray,CV_RGB2GRAY);//RGB2GRAY
    imshow("123",imggray);//显示图片  
    waitKey(0);
    return 1;
}
```
编译与运行：
```
$ make
g++ main.c `pkg-config --cflags --libs opencv`
$ ./a.out ../wongrgb.jpg
```
***2.二值图+轮廓图***[https://www.cnblogs.com/always-chang/p/6170859.html](https://www.cnblogs.com/always-chang/p/6170859.html)
```cpp
#include <opencv2/opencv.hpp>  
using namespace std;
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")  
IplImage *g_pGrayImage = NULL;
const char *pstrWindowsBinaryTitle = "二值图";
const char *pstrWindowsOutLineTitle = "轮廓图";
CvSeq *g_pcvSeq = NULL;
void on_trackbar(int pos)
{
    // 转为二值图  
    IplImage *pBinaryImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 1);
    cvThreshold(g_pGrayImage, pBinaryImage, pos, 255, CV_THRESH_BINARY);
    // 显示二值图  
    cvShowImage(pstrWindowsBinaryTitle, pBinaryImage);
    CvMemStorage* cvMStorage = cvCreateMemStorage();
    // 检索轮廓并返回检测到的轮廓的个数  
    cvFindContours(pBinaryImage, cvMStorage, &g_pcvSeq);
    IplImage *pOutlineImage = cvCreateImage(cvGetSize(g_pGrayImage), IPL_DEPTH_8U, 3);
    int _levels = 5;
    cvZero(pOutlineImage);
    cvDrawContours(pOutlineImage, g_pcvSeq, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0), _levels);
    cvShowImage(pstrWindowsOutLineTitle, pOutlineImage);
    cvReleaseMemStorage(&cvMStorage);
    cvReleaseImage(&pBinaryImage);
    cvReleaseImage(&pOutlineImage);
}
int main(int argc, char** argv)
{
    const char *pstrWindowsSrcTitle = "原图";
    const char *pstrWindowsToolBarName = "二值化";
    // 从文件中加载原图  
    IplImage *pSrcImage = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    // 显示原图  
    cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
    cvShowImage(pstrWindowsSrcTitle, pSrcImage);
    // 转为灰度图  
    g_pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
    cvCvtColor(pSrcImage, g_pGrayImage, CV_BGR2GRAY);
    // 创建二值图和轮廓图窗口  
    cvNamedWindow(pstrWindowsBinaryTitle, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(pstrWindowsOutLineTitle, CV_WINDOW_AUTOSIZE);
    // 滑动条    
    int nThreshold = 0;
    cvCreateTrackbar(pstrWindowsToolBarName, pstrWindowsBinaryTitle, &nThreshold, 254, on_trackbar);
    on_trackbar(1);
    cvWaitKey(0);
    cvDestroyWindow(pstrWindowsSrcTitle);
    cvDestroyWindow(pstrWindowsBinaryTitle);
    cvDestroyWindow(pstrWindowsOutLineTitle);
    cvReleaseImage(&pSrcImage);
    cvReleaseImage(&g_pGrayImage);
    return 0;
}
```
编译与运行：
```
$ make
g++ main.c `pkg-config --cflags --libs opencv`
$ ./a.out ../wong.jpg
```
效果图：
![](https://img-blog.csdn.net/20180428105323680)
![](https://img-blog.csdn.net/20180428105339126)
***3.鼠标绘图[https://www.cnblogs.com/always-chang/p/6170859.html](https://www.cnblogs.com/always-chang/p/6170859.html)***
```cpp
#include <opencv2/opencv.hpp>  
using namespace std;
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")  
const char *pstrWindowsMouseDrawTitle = "鼠标绘图";
// 鼠标消息的回调函数  
void on_mouse(int event, int x, int y, int flags, void* param)
{
    static bool s_bMouseLButtonDown = false;
    static CvPoint s_cvPrePoint = cvPoint(0, 0);
    switch (event)
    {
    case CV_EVENT_LBUTTONDOWN:
        s_bMouseLButtonDown = true;
        s_cvPrePoint = cvPoint(x, y);
        break;
    case  CV_EVENT_LBUTTONUP:
        s_bMouseLButtonDown = false;
        break;
    case CV_EVENT_MOUSEMOVE:
        if (s_bMouseLButtonDown)
        {
            CvPoint cvCurrPoint = cvPoint(x, y);
            cvLine((IplImage*)param, s_cvPrePoint, cvCurrPoint, CV_RGB(0, 0, 20), 3);
            s_cvPrePoint = cvCurrPoint;
            cvShowImage(pstrWindowsMouseDrawTitle, (IplImage*)param);
        }
        break;
    }
}
int main()
{
    const int MAX_WIDTH = 500, MAX_HEIGHT = 400;
    const char *pstrSaveImageName = "Draw.jpg";
    IplImage *pSrcImage = cvCreateImage(cvSize(MAX_WIDTH, MAX_HEIGHT), IPL_DEPTH_8U, 3);
    cvSet(pSrcImage, CV_RGB(255, 255, 255)); //可以用cvSet()将图像填充成白色  
    cvNamedWindow(pstrWindowsMouseDrawTitle, CV_WINDOW_AUTOSIZE);
    cvShowImage(pstrWindowsMouseDrawTitle, pSrcImage);
    cvSetMouseCallback(pstrWindowsMouseDrawTitle, on_mouse, (void*)pSrcImage);
    int c;
    do{
        c = cvWaitKey(0);
        switch ((char)c)
        {
        case 'r'://r重画
            cvSet(pSrcImage, CV_RGB(255, 255, 255));
            cvShowImage(pstrWindowsMouseDrawTitle, pSrcImage);
            break;
        case 's'://s保存图像
            cvSaveImage(pstrSaveImageName, pSrcImage);
            break;
        }
    } while (c > 0 && c != 27);
    cvDestroyWindow(pstrWindowsMouseDrawTitle);
    cvReleaseImage(&pSrcImage);
    return 0;
}
```
编译与运行：
```
$ make
g++ main.c `pkg-config --cflags --libs opencv`
$ ./a.out
```
效果图：
![](https://img-blog.csdn.net/20180428105720136)
***4.灰度直方图[https://www.cnblogs.com/always-chang/p/6170859.html](https://www.cnblogs.com/always-chang/p/6170859.html)***
```cpp
#include <opencv2/opencv.hpp>  
#include <opencv2/legacy/compat.hpp>  
using namespace std;
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")  
void FillWhite(IplImage *pImage)
{
    cvRectangle(pImage, cvPoint(0, 0), cvPoint(pImage->width, pImage->height), CV_RGB(255, 255, 255), CV_FILLED);
}
// 创建灰度图像的直方图  
CvHistogram* CreateGrayImageHist(IplImage **ppImage)
{
    int nHistSize = 256;
    float fRange[] = { 0, 255 };  //灰度级的范围    
    float *pfRanges[] = { fRange };
    CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);
    cvCalcHist(ppImage, pcvHistogram);
    return pcvHistogram;
}
// 根据直方图创建直方图图像  
IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram)
{
    IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight), IPL_DEPTH_8U, 1);
    FillWhite(pHistImage);
    //统计直方图中的最大直方块  
    float fMaxHistValue = 0;
    cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);
    //分别将每个直方块的值绘制到图中  
    int i;
    for (i = 0; i < nImageWidth; i++)
    {
        float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //像素为i的直方块大小  
        int nRealHeight = cvRound((fHistValue / fMaxHistValue) * nImageHeight);  //要绘制的高度  
        cvRectangle(pHistImage,
            cvPoint(i * nScale, nImageHeight - 1),
            cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
            cvScalar(i, 0, 0, 0),
            CV_FILLED
            );
    }
    return pHistImage;
}
int main(int argc, char** argv)
{
    const char *pstrWindowsSrcTitle = "原图";
    const char *pstrWindowsGrayTitle = "灰度图";
    const char *pstrWindowsHistTitle = "直方图";
    // 从文件中加载原图  
    IplImage *pSrcImage = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
    // 灰度图  
    cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
    // 灰度直方图  
    CvHistogram *pcvHistogram = CreateGrayImageHist(&pGrayImage);
    // 创建直方图图像  
    int nHistImageWidth = 255;
    int nHistImageHeight = 150;  //直方图图像高度  
    int nScale = 2;
    IplImage *pHistImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogram);
    // 显示  
    cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(pstrWindowsGrayTitle, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(pstrWindowsHistTitle, CV_WINDOW_AUTOSIZE);
    cvShowImage(pstrWindowsSrcTitle, pSrcImage);
    cvShowImage(pstrWindowsGrayTitle, pGrayImage);
    cvShowImage(pstrWindowsHistTitle, pHistImage);
    cvWaitKey(0);
    cvReleaseHist(&pcvHistogram);
    cvDestroyWindow(pstrWindowsSrcTitle);
    cvDestroyWindow(pstrWindowsGrayTitle);
    cvDestroyWindow(pstrWindowsHistTitle);
    cvReleaseImage(&pSrcImage);
    cvReleaseImage(&pGrayImage);
    cvReleaseImage(&pHistImage);
    return 0;
}
```
编译与运行：
```
$ make
g++ main.c `pkg-config --cflags --libs opencv`
$ ./a.out ../wongrgb.jpg
```
效果图：
![](https://img-blog.csdn.net/20180428110404345)
***5.灰度直方图均衡化***
```cpp
#include <opencv2/opencv.hpp>  
#include <opencv2/legacy/compat.hpp>  
using namespace std;
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")  
void FillWhite(IplImage *pImage)
{
    cvRectangle(pImage, cvPoint(0, 0), cvPoint(pImage->width, pImage->height), CV_RGB(255, 255, 255), CV_FILLED);
}
// 创建灰度图像的直方图  
CvHistogram* CreateGrayImageHist(IplImage **ppImage)
{
    int nHistSize = 256;
    float fRange[] = { 0, 255 };  //灰度级的范围    
    float *pfRanges[] = { fRange };
    CvHistogram *pcvHistogram = cvCreateHist(1, &nHistSize, CV_HIST_ARRAY, pfRanges);
    cvCalcHist(ppImage, pcvHistogram);
    return pcvHistogram;
}
// 根据直方图创建直方图图像  
IplImage* CreateHisogramImage(int nImageWidth, int nScale, int nImageHeight, CvHistogram *pcvHistogram)
{
    IplImage *pHistImage = cvCreateImage(cvSize(nImageWidth * nScale, nImageHeight), IPL_DEPTH_8U, 1);
    FillWhite(pHistImage);
    //统计直方图中的最大直方块  
    float fMaxHistValue = 0;
    cvGetMinMaxHistValue(pcvHistogram, NULL, &fMaxHistValue, NULL, NULL);
    //分别将每个直方块的值绘制到图中  
    int i;
    for (i = 0; i < nImageWidth; i++)
    {
        float fHistValue = cvQueryHistValue_1D(pcvHistogram, i); //像素为i的直方块大小  
        int nRealHeight = cvRound((fHistValue / fMaxHistValue) * nImageHeight);  //要绘制的高度  
        cvRectangle(pHistImage,
            cvPoint(i * nScale, nImageHeight - 1),
            cvPoint((i + 1) * nScale - 1, nImageHeight - nRealHeight),
            cvScalar(i, 0, 0, 0),
            CV_FILLED
            );
    }
    return pHistImage;
}
int main(int argc, char** argv)
{
    const char *pstrWindowsSrcTitle = "原图";
    const char *pstrWindowsGrayTitle = "灰度图";
    const char *pstrWindowsHistTitle = "直方图";
    const char *pstrWindowsGrayEqualizeTitle = "灰度图-均衡化后";
    const char *pstrWindowsHistEqualizeTitle = "直方图-均衡化后";
    // 从文件中加载原图  
    IplImage *pSrcImage = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
    IplImage *pGrayEqualizeImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
    // 灰度图  
    cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);
    // 直方图图像数据  
    int nHistImageWidth = 255;
    int nHistImageHeight = 150;
    int nScale = 2;
    // 灰度直方图及直方图图像  
    CvHistogram *pcvHistogram = CreateGrayImageHist(&pGrayImage);
    IplImage *pHistImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogram);
    // 均衡化  
    cvEqualizeHist(pGrayImage, pGrayEqualizeImage);
    // 均衡化后的灰度直方图及直方图图像  
    CvHistogram *pcvHistogramEqualize = CreateGrayImageHist(&pGrayEqualizeImage);
    IplImage *pHistEqualizeImage = CreateHisogramImage(nHistImageWidth, nScale, nHistImageHeight, pcvHistogramEqualize);
    // 显示
    cvNamedWindow(pstrWindowsGrayTitle, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(pstrWindowsHistTitle, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(pstrWindowsGrayEqualizeTitle, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(pstrWindowsHistEqualizeTitle, CV_WINDOW_AUTOSIZE);
    //显示代码….
    cvShowImage(pstrWindowsGrayTitle, pGrayImage);//显示灰度图
    cvShowImage(pstrWindowsHistTitle, pHistImage);//显示灰度直方图
    cvShowImage(pstrWindowsGrayEqualizeTitle, pGrayEqualizeImage);//显示均衡化后的灰度图
    cvShowImage(pstrWindowsHistEqualizeTitle, pHistEqualizeImage);//显示均衡化后的灰度直方图
    //显示代码….
    cvWaitKey(0);
    //回收资源代码…
    cvDestroyWindow(pstrWindowsGrayTitle);
    cvDestroyWindow(pstrWindowsHistTitle);
    cvDestroyWindow(pstrWindowsGrayEqualizeTitle);
    cvDestroyWindow(pstrWindowsHistEqualizeTitle);
    cvReleaseImage(&pSrcImage);
    cvReleaseImage(&pHistImage);
    cvReleaseImage(&pGrayEqualizeImage);
    cvReleaseImage(&pHistEqualizeImage);
    return 0;
}
```
编译与运行：
```
$ make
g++ main.c `pkg-config --cflags --libs opencv`
$ ./a.out ../wongrgb.jpg
```
效果图：
![](https://img-blog.csdn.net/20180428110908478)
***6.彩色直方图均衡化***
```cpp
#include <opencv2/opencv.hpp>  
using namespace std;
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")  
//彩色图像的直方图均衡化  
IplImage* EqualizeHistColorImage(IplImage *pImage)
{
    IplImage *pEquaImage = cvCreateImage(cvGetSize(pImage), pImage->depth, 3);
    // 原图像分成各通道后再均衡化,最后合并即彩色图像的直方图均衡化  
    const int MAX_CHANNEL = 4;
    IplImage *pImageChannel[MAX_CHANNEL] = { NULL };
    int i;
    for (i = 0; i < pImage->nChannels; i++)
        pImageChannel[i] = cvCreateImage(cvGetSize(pImage), pImage->depth, 1);
    cvSplit(pImage, pImageChannel[0], pImageChannel[1], pImageChannel[2], pImageChannel[3]);
    for (i = 0; i < pImage->nChannels; i++)
        cvEqualizeHist(pImageChannel[i], pImageChannel[i]);
    cvMerge(pImageChannel[0], pImageChannel[1], pImageChannel[2], pImageChannel[3], pEquaImage);
    for (i = 0; i < pImage->nChannels; i++)
        cvReleaseImage(&pImageChannel[i]);
    return pEquaImage;
}
int main(int argc, char** argv)
{
    const char *pstrWindowsSrcTitle = "原图";
    const char *pstrWindowsHisEquaTitle = "直方图均衡化后";
    // 从文件中加载原图  
    IplImage *pSrcImage = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    IplImage *pHisEquaImage = EqualizeHistColorImage(pSrcImage);
    cvNamedWindow(pstrWindowsSrcTitle, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(pstrWindowsHisEquaTitle, CV_WINDOW_AUTOSIZE);
    cvShowImage(pstrWindowsSrcTitle, pSrcImage);
    cvShowImage(pstrWindowsHisEquaTitle, pHisEquaImage);
    cvWaitKey(0);
    cvDestroyWindow(pstrWindowsSrcTitle);
    cvDestroyWindow(pstrWindowsHisEquaTitle);
    cvReleaseImage(&pSrcImage);
    cvReleaseImage(&pHisEquaImage);
    return 0;
}
```
编译与运行：
```
$ make
g++ main.c `pkg-config --cflags --libs opencv`
$ ./a.out ../wongrgb.jpg
```
效果图（效果也不太好啊）：
![](https://img-blog.csdn.net/20180428111142143)
更多操作示例：[https://blog.csdn.net/qq_35874394/article/details/53290370](https://blog.csdn.net/qq_35874394/article/details/53290370)

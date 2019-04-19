# OpenCV学习笔记：绘图指令（矩形、圆、线、文本标注） - Koma Hub - CSDN博客
2018年05月04日 14:16:54[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：153
***环境：***CentOS7
g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
```
$ pkg-config --modversion opencv
2.4.13
```
效果图（前后对比）：
![](https://img-blog.csdn.net/20180504141444383)![](https://img-blog.csdn.net/20180504141500735)
***代码：***
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
    /********图像转换********/
    /*字节图像的灰度图-彩色转换*/
    IplImage *img1 = cvLoadImage("wongrgb.jpg");
    // 绘制矩形:在点 1和 2之间绘制一矩形，边线用红色、宽度
    cvRectangle(img1, cvPoint(10,10), cvPoint(70,70), cvScalar(0,255,0), 3);
    // 绘制圆形:圆心为、半径. 圆周颜色、宽度
    cvCircle(img1, cvPoint(100,100), 20, cvScalar(255,255,0), 2);
    // 绘制线段:在 (100,100) 和 (200,200) 之间、线宽为 1 的绿色线段
    cvLine(img1, cvPoint(100,100), cvPoint(200,200), cvScalar(0,0,255), 3);
    /*绘制一组线段:*/
    /*void cvPolyLine( CvArr* img, CvPoint** pts, int* npts, 
                        int contours, int is_closed,
                        CvScalar color, int thickness=1, 
                        int line_type=8, int shift=0 );
        img 图像。
        pts 折线的顶点指针数组。
        npts 折线的定点个数数组。也可以认为是pts指针数组的大小
        contours 折线的线段数量。
        is_closed 指出多边形是否封闭。如果封闭，函数将起始点和结束点连线。
        color 折线的颜色。
        thickness 线条的粗细程度。
        line_type 线段的类型。参见cvLine。
        shift 顶点的小数点位数*/
    CvPoint curve1[]={10,10, 10,100, 100,100, 100,10};
    CvPoint curve2[]={30,30, 30,130, 130,130, 130,30, 150,10};
    CvPoint* curveArr[2]={curve1, curve2};
    int nCurvePts[2]={4,5};
    int nCurves=2;
    int isCurveClosed=1;
    int lineWidth=2;
    cvPolyLine(img1,curveArr,nCurvePts,nCurves,
                    isCurveClosed,cvScalar(0,255,255),lineWidth);
    /*绘制一组填充颜色的多边形:*/
    /*cvFillPoly用于一个单独被多边形轮廓所限定的区域内进行填充。
                函数可以填充复杂的区域,例如，有漏洞的区域和有交叉点的区域等等。
    void cvFillPoly( CvArr* img, CvPoint** pts, int* npts, 
                    int contours,CvScalar color, int
                    line_type=8, int shift=0 );
            img 图像。
            pts 指向多边形的数组指针。
            npts 多边形的顶点个数的数组。
            contours 组成填充区域的线段的数量。
            color 多边形的颜色。
            line_type 组成多边形的线条的类型。
            shift 顶点坐标的小数点位数。*/
    cvFillPoly(img1,curveArr,nCurvePts,nCurves,cvScalar(0,255,255));
    /*文本标注:*/
    /*可用的字体类型有: CV_FONT_HERSHEY_SIMPLEX, CV_FONT_HERSHEY_PLAIN,
    CV_FONT_HERSHEY_DUPLEX, CV_FONT_HERSHEY_COMPLEX, CV_FONT_HERSHEY_TRIPLEX,
    CV_FONT_HERSHEY_COMPLEX_SMALL, CV_FONT_HERSHEY_SCRIPT_SIMPLEX,
    CV_FONT_HERSHEY_SCRIPT_COMPLEX,*/
    CvFont font;
    double hScale=1.0;
    double vScale=1.0;
    lineWidth=2;
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);
    cvPutText (img1,"Comment",cvPoint(20,100), &font, cvScalar(8,0,0));
    cvShowImage("win1",img1);
    cvSaveImage("wongrgb1.jpg",img1);
    cvWaitKey(0);
    return 0;
}
```
***编译与运行：***
```
$ make
g++ main.cpp `pkg-config --cflags --libs opencv`
$ ./a.out
```
代码参考：《OpenCV中文参考手册》

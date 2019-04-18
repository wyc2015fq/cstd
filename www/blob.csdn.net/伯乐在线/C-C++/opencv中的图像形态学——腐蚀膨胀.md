# opencv中的图像形态学——腐蚀膨胀 - 文章 - 伯乐在线
原文出处： [huhuuu，2014-01-21](http://www.cnblogs.com/huhuuu/p/3528867.html)
腐蚀膨胀是图像形态学比较常见的处理，腐蚀一般可以用来消除噪点，分割出独立的图像元素等。
一般腐蚀操作对二值图进行处理，腐蚀操作如下图，中心位置的像素点是否与周围领域的像素点颜色一样（即是否是白色点，即值是否为255），若一致，则保留，不一致则该点变为黑色（值即为0）
![](http://jbcdn2.b0.upaiyun.com/2016/12/2110ef31efd478da4697446d82f86551.png)
opencv中的腐蚀操作：

```
CVAPI(void)  cvErode( const CvArr* src, CvArr* dst,
                      IplConvKernel* element CV_DEFAULT(NULL),
                      int iterations CV_DEFAULT(1) );
```
前两个参数比较熟悉，第三个参数是用于传递模板的信息，默认是（NULL），即为3*3的模板，第四个参数是迭代的次数（即该腐蚀操作做几次）
opencv中的膨胀操作其实就是腐蚀的反操作：

```
CVAPI(void)  cvDilate( const CvArr* src, CvArr* dst,
                       IplConvKernel* element CV_DEFAULT(NULL),
                       int iterations CV_DEFAULT(1) );
```
测试代码：

```
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
int main(){
    IplImage *img= cvLoadImage("C:/fu.jpg");//读取图片
    cvNamedWindow("Example1",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Example2",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Example3",CV_WINDOW_AUTOSIZE);
    cvShowImage("Example1",img);//在Example1显示图片
    //    cvCopy(img,temp);
    IplImage* temp=cvCreateImage( //创建一个size为image,三通道8位的彩色图
        cvGetSize(img),
        IPL_DEPTH_8U,
        );
    cvErode(img,temp,0,1);//腐蚀
    cvShowImage("Example2",temp);
    cvDilate(img,temp,0,1);//膨胀
    cvShowImage("Example3",temp);
    cvWaitKey(0);//暂停用于显示图片
    cvReleaseImage(&img);//释放img所指向的内存空间并且
    cvDestroyWindow("Example1");
    cvDestroyWindow("Example2");
    cvDestroyWindow("Example3");
    
    return 0;
}
```
效果图：
![](http://jbcdn2.b0.upaiyun.com/2016/12/a3ae4e5ae6b1e92bcb1a877f9bc65ed9.png)
以上都是在模板3*3的情况下处理的，要是我们期望使用自己定义的模板时候，就需要自己做模板。

```
CVAPI(IplConvKernel*)  cvCreateStructuringElementEx(
            int cols, int  rows, int  anchor_x, int  anchor_y,
            int shape, int* values CV_DEFAULT(NULL) );
```
前两个参数是定义模板的大小，后两个参数是参考点的坐标（比如默认3*3模板的参考点坐标是2*2），第五个参数是模板的类型(可以是矩形，十字形，椭圆形，甚至是用户自己定义形状)，最后一个参数是在使用自自定义形状的时候，通过value传递模板的形状。
模板的类型：
![](http://jbcdn2.b0.upaiyun.com/2016/12/6d2d45fb93a2f3a5dfc759346aae669b.png)

```
CVAPI(void)  cvReleaseStructuringElement( IplConvKernel** element ); //释放模板所占用的内存
```
自定义5*5,参考点（3,3）的矩形模板的测试代码：

```
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
int main(){
    IplImage *img= cvLoadImage("C:/fu.jpg");//读取图片
    cvNamedWindow("Example1",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Example2",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Example3",CV_WINDOW_AUTOSIZE);
    cvShowImage("Example1",img);//在Example1显示图片
    //    cvCopy(img,temp);
    IplImage* temp=cvCreateImage( //创建一个size为image,三通道8位的彩色图
        cvGetSize(img),
        IPL_DEPTH_8U,
        );
    IplConvKernel * myModel;
    myModel=cvCreateStructuringElementEx( //自定义5*5,参考点（3,3）的矩形模板
        5,5,2,2,CV_SHAPE_RECT
        );
    cvErode(img,temp,myModel,1);
    cvShowImage("Example2",temp);
    cvDilate(img,temp,myModel,1);
    cvShowImage("Example3",temp);
    cvWaitKey(0);//暂停用于显示图片
    cvReleaseStructuringElement(&myModel);
    cvReleaseImage(&img);//释放img所指向的内存空间并且
    cvDestroyWindow("Example1");
    cvDestroyWindow("Example2");
    cvDestroyWindow("Example3");
    
    return 0;
}
```
效果图：
![](http://jbcdn2.b0.upaiyun.com/2016/12/e937443f596b75695b6a47f5c7689a48.png)
参考：学习opencv

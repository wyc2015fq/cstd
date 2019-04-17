# OpenCV学习笔记 - xiahouzuoxin - CSDN博客





2012年08月03日 22:58:27[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1759标签：[image																[float																[图像处理																[dst																[header																[colors](https://so.csdn.net/so/search/s.do?q=colors&t=blog)
个人分类：[Image/Audio/ML...](https://blog.csdn.net/xiahouzuoxin/article/category/1203619)





转载出处：[http://blog.csdn.net/byxdaz/article/details/4909452](http://blog.csdn.net/byxdaz/article/details/4909452)

一、OpenCV概述与功能介绍

OpenCV是Intel®开源计算机视觉库。它由一系列 C 函数和少量 C++ 类构成，实现了图像处理和计算机视觉方面的很多通用算法。OpenCV 拥有包括 300 多个C函数的跨平台的中、高层 API。它不依赖于其它的外部库——尽管也可以使用某些外部库。


OpenCV 对非商业应用和商业应用都是免费（FREE）的。（细节参考 license）。代码下载地址：[http://www.sourceforge.net/projects/opencvlibrary](http://www.sourceforge.net/projects/opencvlibrary)

OpenCV 为Intel® Integrated Performance Primitives (IPP) 提供了透明接口。 这意味着如果有为特定处理器优化的的 IPP 库， OpenCV 将在运行时自动加载这些库。 更多关于 IPP 的信息请参考：
[http://www.intel.com/software/products/ipp/index.htm](http://www.intel.com/software/products/ipp/index.htm)

它有以下特点：

1) 开放的C/C++源码

2) 基于Intel处理器指令集开发的优化代码

3) 统一的结构和功能定义

4) 强大的图像和矩阵运算能力

5) 方便灵活的用户接口

6）同时支持MS-WINDOWS、LINUX平台

作为一个基本的计算机视觉、图像处理和模式识别的开源项目，OPENCV可以直接应用于很多领域，作为第二次开发的理想工具。

OpenCV功能介绍：

OpenCV包含如下几个部分：

Cxcore：一些基本函数（各种数据类型的基本运算等）。

Cv：图像处理和计算机视觉功能（图像处理，结构分析，运动分析，物体跟踪，模式识别，摄像机定标）

Ml：机器学习模块，目前内容主要为分类器。

Cvaux：一些实验性的函数（ViewMorphing，三维跟踪，PCA，HMM）

Highgui：用户交互部分，（GUI，图象视频I/O，系统调用函数）



二、OpenCV安装

OpenCV2.0刚刚发布，VC 2008 Express下安装OpenCV2.0请参考：

[http://www.opencv.org.cn/index.php/VC_2008_Express%E4%B8%8B%E5%AE%89%E8%A3%85OpenCV2.0](http://www.opencv.org.cn/index.php/VC_2008_Express%E4%B8%8B%E5%AE%89%E8%A3%85OpenCV2.0)



三、基础知识：

1、opencv
数据类型转换操作小结

（1）图像中或矩阵数组中数据格式转换：

cvConvert( image, image_temp );


cvConvertScale( const CvArr* src, CvArr* dst, double scale CV_DEFAULT(1), double shift CV_DEFAULT(0) );


cvScale(src, dst);


// Converts CvArr (IplImage or CvMat,...) to CvMat.

cvGetMat( const CvArr* arr, CvMat* header, int* coi CV_DEFAULT(NULL), int allowND CV_DEFAULT(0));

cvCopy( const CvArr* src, CvArr* dst, const CvArr* mask ); //可以实现对不规制图形的提取



（2）多通道图像转成数组中数据

cvGetMat( const CvArr* array, CvMat* mat, int* pCOI, int allowND )


cvCopy(img,mat); 


// Converts CvArr (IplImage or CvMat,...) to CvMat.

cvGetMat( const CvArr* arr, CvMat* header, int* coi CV_DEFAULT(NULL), int allowND CV_DEFAULT(0));



(3) 数组中数据转成多通道图像

cvCopy( const CvArr* src, CvArr* dst, const CvArr* mask=NULL );


cvGetMat( const CvArr* arr, CvMat* header, int* coi CV_DEFAULT(NULL), int allowND CV_DEFAULT(0));



2、二值化函数cvAdaptiveThreshold和cvThreshold的一些发现

自适应二值化计算像素的邻域的平均灰度，来决定二值化的值。如果整个区域几乎是一样灰度的，则无法给出合适的结果了。之所以看起来像边缘检测，是因为窗尺寸设置的小，可以改大一点试一试。

cvAdaptiveThreshold( src, dst, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 21); //窗设置为21

没有万能的二值化方法，具体问题具体分析，自适应二值化对于光照不均的文字，条码等，效果很好。窗口大小选择，考虑被检测物体尺寸。自适应阈值化中的阈值完全是由你所选择的邻域所确定的，如果你所选择的邻域非常小（比如3×3），那么很显然阈值的“自适应程度”就非常高，这在结果图像中就表现为边缘检测的效果。如果邻域选择的比较大（比如31×31），那么阈值的“自适应程度”就比较低，这在结果图像中就表现为二值化的效果。



3、用 gabor 和 AdaBoost
（MultiBoost ）做目标检测图像识别

[http://www.opencv.org.cn/forum/viewtopic.php?f=10&t=7790](http://www.opencv.org.cn/forum/viewtopic.php?f=10&t=7790)



4、视频跟踪方法

跟踪的方法我知道的有KLMAN滤波.粒子滤波.camshift.meanshift。

# 基于Mean Shift的阈值分割：[**http://www.codesoso.com/code/mean_shift.aspx**](http://www.codesoso.com/code/mean_shift.aspx)

# [http://arslan-ai.spaces.live.com/blog/cns!CAE7EF891A2218BA!123.entry](http://arslan-ai.spaces.live.com/blog/cns!CAE7EF891A2218BA!123.entry)

5、怎么访问图像像素 

（坐标是从0开始的，并且是相对图像原点的位置。图像原点或者是左上角 (img->origin=IPL_ORIGIN_TL) 或者是左下角 (img->origin=IPL_ORIGIN_BL) ）


假设有 8-bit 1－通道的图像 I (IplImage* img)： 

---------------------------------------------------------------------

I(x,y) ~ ((uchar*)(img->imageData + img->widthStep*y))[x]

---------------------------------------------------------------------

假设有 8-bit 3-通道的图像 I (IplImage* img)： 

---------------------------------------------------------------------

I(x,y)blue ~ ((uchar*)(img->imageData + img->widthStep*y))[x*3]

I(x,y)green ~ ((uchar*)(img->imageData + img->widthStep*y))[x*3+1]

I(x,y)red ~ ((uchar*)(img->imageData + img->widthStep*y))[x*3+2]

------------------------------------------------------------------------------

例如，给点 (100,100) 的亮度增加 30 ，那么可以这样做： 

------------------------------------------------------------------------------

CvPoint pt = {100,100};

((uchar*)(img->imageData + img->widthStep*pt.y))[pt.x*3] += 30;

((uchar*)(img->imageData + img->widthStep*pt.y))[pt.x*3+1] += 30;

((uchar*)(img->imageData + img->widthStep*pt.y))[pt.x*3+2] += 30;

-----------------------------------------------------------------------------

或者更高效地： 

-----------------------------------------------------------------------------

CvPoint pt = {100,100};

uchar* temp_ptr = &((uchar*)(img->imageData + img->widthStep*pt.y))[pt.x*3];

temp_ptr[0] += 30;

temp_ptr[1] += 30;

temp_ptr[2] += 30;

-----------------------------------------------------------------------------

假设有 32-bit 浮点数, 1-通道 图像 I (IplImage* img)： 

-----------------------------------------------------------------------------

I(x,y) ~ ((float*)(img->imageData + img->widthStep*y))[x]

-----------------------------------------------------------------------------

现在，一般的情况下，假设有 N-通道，类型为 T 的图像： 

-----------------------------------------------------------------------------

I(x,y)c ~ ((T*)(img->imageData + img->widthStep*y))[x*N + c]

-----------------------------------------------------------------------------

你可以使用宏 CV_IMAGE_ELEM( image_header, elemtype, y, x_Nc ) 

-----------------------------------------------------------------------------

I(x,y)c ~ CV_IMAGE_ELEM( img, T, y, x*N + c )

-----------------------------------------------------------------------------

也有针对各种图像（包括 4 通道图像）和矩阵的函数（cvGet2D, cvSet2D）， 但是它们非常慢。 



6、如何访问矩阵元素？ 

方法是类似的（下面的例子都是针对 0 起点的列和行） 



设有 32-bit 浮点数的实数矩阵 M (CvMat* mat)： 

----------------------------------------------------------------------------

M(i,j) ~ ((float*)(mat->data.ptr + mat->step*i))[j]

----------------------------------------------------------------------------

设有 64-bit 浮点数的复数矩阵 M (CvMat* mat)： 

----------------------------------------------------------------------------

Re M(i,j) ~ ((double*)(mat->data.ptr + mat->step*i))[j*2]

Im M(i,j) ~ ((double*)(mat->data.ptr + mat->step*i))[j*2+1]

----------------------------------------------------------------------------

对单通道矩阵，有宏 CV_MAT_ELEM( matrix, elemtype, row, col ), 例如对 32-bit

浮点数的实数矩阵： 

M(i,j) ~ CV_MAT_ELEM( mat, float, i, j ),

例如，这儿是一个 3x3 单位矩阵的初始化： 

CV_MAT_ELEM( mat, float, 0, 0 ) = 1.f;

CV_MAT_ELEM( mat, float, 0, 1 ) = 0.f;

CV_MAT_ELEM( mat, float, 0, 2 ) = 0.f;

CV_MAT_ELEM( mat, float, 1, 0 ) = 0.f;

CV_MAT_ELEM( mat, float, 1, 1 ) = 1.f;

CV_MAT_ELEM( mat, float, 1, 2 ) = 0.f;

CV_MAT_ELEM( mat, float, 2, 0 ) = 0.f;

CV_MAT_ELEM( mat, float, 2, 1 ) = 0.f;

CV_MAT_ELEM( mat, float, 2, 2 ) = 1.f;



7、如何在 OpenCV 中处理我自己的数据 

设你有 300x200 32-bit 浮点数 image/array, 也就是对一个有 60000 个元素的数组。 

----------------------------------------------------------------------------

int cols = 300, rows = 200;

float* myarr = new float[rows*cols];

// 第一步，初始化 CvMat 头

CvMat mat = cvMat( rows, cols,

                  CV_32FC1, // 32 位浮点单通道类型

                  myarr // 用户数据指针（数据没有被复制）

                  );

// 第二步，使用 cv 函数， 例如计算 l2 (Frobenius) 模

double norm = cvNorm( &mat, 0, CV_L2 );

...

delete myarr;

其它情况在参考手册中有描述。 见 cvCreateMatHeader，cvInitMatHeader，cvCreateImageHeader， cvSetData 等




8、如何读入和显示图像 

----------------------------------------------------------------------------

/* usage: prog <image_name> */

#include "cv.h"

#include "highgui.h"



int main( int argc, char** argv )

{



    IplImage* img;

    if( argc == 2 && (img = cvLoadImage( argv[1], 1)) != 0 )

    {



        cvNamedWindow( "Image view", 1 );

        cvShowImage( "Image view", img );

        cvWaitKey(0); // 非常重要，内部包含事件处理循环

        cvDestroyWindow( "Image view" );

        cvReleaseImage( &img );

        return 0;



}

    return -1;



}



9、图像的通道

描述一个像素点，如果是灰度，那么只需要一个数值来描述它，就是单通道。如果一个像素点，有RGB三种颜色来描述它，就是三通道。4通道通常为RGBA，在某些处理中可能会用到。
 2通道图像不常见，通常在程序处理中会用到，如傅里叶变换，可能会用到，一个通道为实数，一个通道为虚数，主要是编程方便。



10、HBITMAP 转换IplImage、IplImage转换为DIB

// HBITMAP 转换IplImage

IplImage* hBitmap2Ipl(HBITMAP hBmp)

{

BITMAP bmp;

::GetObject(hBmp,sizeof(BITMAP),&bmp);

int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8 ;

int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

IplImage* img = cvCreateImageHeader( cvSize(bmp.bmWidth, bmp.bmHeight)

, depth, nChannels );

img->imageData =

(char*)malloc(bmp.bmHeight*bmp.bmWidth*nChannels*sizeof(char));

memcpy(img->imageData,(char*)(bmp.bmBits),bmp.bmHeight*bmp.bmWidth*nChannels);

return img;

} 



void createDIB(IplImage* &pict){

IplImage * Red=cvCreateImage( cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),

IPL_DEPTH_8U, 1 );

IplImage * Green=cvCreateImage( cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),

IPL_DEPTH_8U, 1 );

IplImage * Blue=cvCreateImage( cvSize(IMAGE_WIDTH,IMAGE_HEIGHT),

IPL_DEPTH_8U, 1 );

cvSetImageCOI( pict, 3);

cvCopy(pict,Red);

cvSetImageCOI( pict, 2);

cvCopy(pict,Green);

cvSetImageCOI(pict, 1);

cvCopy(pict,Blue);

//Initialize the BMP display buffer

bmi = (BITMAPINFO*)buffer;

bmih = &(bmi->bmiHeader);

memset( bmih, 0, sizeof(*bmih));

bmih->biSize = sizeof(BITMAPINFOHEADER);

bmih->biWidth = IMAGE_WIDTH;

bmih->biHeight = IMAGE_HEIGHT; // -IMAGE_HEIGHT;

bmih->biPlanes = 1;

bmih->biCompression = BI_RGB;

bmih->biBitCount = 24;

palette = bmi->bmiColors;

for( int i = 0; i < 256; i++ ){

palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed =

(BYTE)i;

palette[i].rgbReserved = 0;

}

cvReleaseImage(&Red);

cvReleaseImage(&Green);

cvReleaseImage(&Blue);

} 



// HBITMAP转换DIB

HBITMAP plIamgeToDIB(IplImage *pImg,int Size)   

{   

    HDC hDC = ::CreateCompatibleDC(0);   

    BYTE tmp[sizeof(BITMAPINFO)+255*4];   

    BITMAPINFO *bmi = (BITMAPINFO*)tmp;   

    HBITMAP hBmp;   

    int i;   

    memset(bmi,0,sizeof(BITMAPINFO));   

    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);   

    bmi->bmiHeader.biWidth = pImg->width;   

    bmi->bmiHeader.biHeight = -pImg->height;   

    bmi->bmiHeader.biPlanes = Size;   

    bmi->bmiHeader.biBitCount = pImg->nChannels * pImg->depth;   



    bmi->bmiHeader.biCompression = BI_RGB;   

    bmi->bmiHeader.biSizeImage = pImg->width*pImg->height*1;   

    bmi->bmiHeader.biClrImportant =0 ;   

    switch(pImg->nChannels * pImg->depth)   

    {   

    case 8 :   

    for(i=0 ; i < 256 ; i++)   

    {   

    bmi->bmiColors[i].rgbBlue = i;   

    bmi->bmiColors[i].rgbGreen= i;   

    bmi->bmiColors[i].rgbRed= i;   

    }   

    break;   

    case 32:   

    case 24:   

    ((DWORD*) bmi->bmiColors)[0] = 0x00FF0000; /* red mask */  

    ((DWORD*) bmi->bmiColors)[1] = 0x0000FF00; /* green mask */  

    ((DWORD*) bmi->bmiColors)[2] = 0x000000FF; /* blue mask */  

    break;   

    }   

    hBmp = ::CreateDIBSection(hDC,bmi,DIB_RGB_COLORS,NULL,0,0);   

    SetDIBits(hDC,hBmp,0,pImg->height,pImg->imageData,bmi,DIB_RGB_COLORS);   

    ::DeleteDC(hDC);   

return hBmp;   



}  



11、图像分割

做分水岭图像分割：cvWatershed

meanshift图像分割：PyrMeanShiftFiltering

用金字塔实现图像分割：cvPyrSegmentation

http://blog.csdn.net/gnuhpc/archive/2009/06/21/4286186.aspx

大津算法阈值分割：[http://hi.baidu.com/lazycat3611/blog/item/491febde06bc605d94ee37e8.html](http://hi.baidu.com/lazycat3611/blog/item/491febde06bc605d94ee37e8.html)

最大熵阈值分割算法：[http://www.aiseminar.cn/html/00/t-700.html](http://www.aiseminar.cn/html/00/t-700.html)



12、边缘检测

cvCanny：采用 Canny 算法做边缘检测

cvLaplace：laplace边缘检测

[http://www.mvonline.com.cn/bbs/simple/index.php?t2421.html](http://www.mvonline.com.cn/bbs/simple/index.php?t2421.html)

cvSobel：Sobel边缘检测

cvCornerHarris：哈里斯（Harris）边缘检测



13、匹配

cvCalcEMD2：两个加权点集之间计算最小工作距离

cvMatchShapes：比较两个形状

cvMatchTemplate：比较模板和重叠的图像区域

基于opencv的sift图像匹配算法vc++源码：[http://codechina.net/source/620393](http://codechina.net/source/620393)



14、分类器

boosted分类器：分类器的boosting技术有四种： Discrete Adaboost, Real Adaboost, Gentle Adaboost and Logitboost。

HAAR分类器，自于haar小波运算。

神经网络分类器

SVM分类器，SVM是一个分类器，原始的SVM是一个两类分类的分类器。可以通过1:1或者1:n的方式来组合成一个多类分类的分类器。天生通过核函数的使用支持高维数据的分类。从几何意义上讲，就是找到最能表示类别特征的那些向量(支持向量SV),然后找到一条线，能最大化分类的 Margin。libSVM是一个不错的实现。

[http://blog.csdn.net/byxdaz/archive/2009/11/28/4893935.aspx](http://blog.csdn.net/byxdaz/archive/2009/11/28/4893935.aspx)



15、如何用OpenCV训练自己的分类器

[http://blog.csdn.net/byxdaz/archive/2009/11/30/4907211.aspx](http://blog.csdn.net/byxdaz/archive/2009/11/30/4907211.aspx)



16、运动目标跟踪与检测

CamShift：

MeanShift：

[http://blog.csdn.net/xauatnwpu/archive/2009/10/29/4743058.aspx](http://blog.csdn.net/xauatnwpu/archive/2009/10/29/4743058.aspx)



17、目标检测

目标检测：http://wenjuanhe.blog.163.com/blog/static/745017252009102101728454/

人脸检测的代码分析：

[http://wenjuanhe.blog.163.com/blog/static/74501725200910391512151/](http://wenjuanhe.blog.163.com/blog/static/74501725200910391512151/)

基于Haar-like特征的层叠推进分类器快速目标检测：

[http://wenjuanhe.blog.163.com/blog/static/7450172520091039180911/](http://wenjuanhe.blog.163.com/blog/static/7450172520091039180911/)



18、检测直线、圆、矩形



检测直线：cvHoughLines，cvHoughLines2

检测圆：cvHoughCircles

检测矩形：opencv中没有对应的函数，下面有段代码可以检测矩形，是通过先找直线，然后找到直线平行与垂直的四根线。

[http://blog.csdn.net/byxdaz/archive/2009/12/01/4912136.aspx](http://blog.csdn.net/byxdaz/archive/2009/12/01/4912136.aspx)





19、直方图

typedef struct CvHistogram

{

int     type;

CvArr* bins; //用于存放直方图每个灰度级数目的数组指针，数组在cvCreateHist 的时候创建，其维数由cvCreateHist 确定

float   thresh[CV_MAX_DIM][2]; // for uniform histograms 

float** thresh2; // for non-uniform histograms 

CvMatND mat; // embedded matrix header for array histograms 

}CvHistogram;

创建直方图 CvHistogram* cvCreateHist( int dims, int* sizes, int type,float** ranges=NULL, int uniform=1 );

dims 直方图维数的数目 

sizes 直方图维数尺寸的数组 

type 直方图的表示格式: CV_HIST_ARRAY 意味着直方图数据表示为多维密集数组 CvMatND; CV_HIST_TREE 意味着直方图数据表示为多维稀疏数组 CvSparseMat.


ranges 图中方块范围的数组. 它的内容取决于参数 uniform 的值。这个范围的用处是确定何时计算直方图或决定反向映射（backprojected ），每个方块对应于输入图像的哪个/哪组值。


uniform 归一化标识。 如果不为0，则ranges[i]（0<=i<cDims，译者注：cDims为直方图的维数，对于灰度图为1，彩色图为3）是包含两个元素的范围数组，包括直方图第i维的上界和下界。在第i维上的整个区域 [lower,upper]被分割成 dims[i] 个相等的块（译者注：dims[i]表示直方图第i维的块数），这些块用来确定输入象素的第 i 个值（译者注：对于彩色图像，i确定R, G,或者B）的对应的块；如果为0，则ranges[i]是包含dims[i]+1个元素的范围数组，包括lower0,
 upper0, lower1, upper1 == lower2, ..., upperdims[i]-1, 其中lowerj 和upperj分别是直方图第i维上第 j 个方块的上下界（针对输入象素的第 i 个值）。

OpenCV统计应用-CvHistogram直方图资料：[http://blog.csdn.net/koriya/archive/2008/11/21/3347369.aspx](http://blog.csdn.net/koriya/archive/2008/11/21/3347369.aspx)

20、物体跟踪
[http://blog.csdn.net/gnuhpc/category/549384.aspx?PageNumber=4](http://blog.csdn.net/gnuhpc/category/549384.aspx?PageNumber=4)

21、在opencv中暂时无法打开二值图像，它里面至少是8位的图像。0，表示黑点；255，表示白点。



22、cvcanny   Canny 算法做边缘检测 

void cvCanny( const CvArr* image, CvArr* edges, double threshold1,double threshold2, int aperture_size=3 );

一般threshold=threshol2*0.4 （经验值）。



23、cvCopy与cvCloneImage的区别 

/* Copies source array to destination array */

CVAPI(void)  cvCopy( const CvArr* src, CvArr* dst,

                     const CvArr* mask CV_DEFAULT(NULL) );

/* Creates a copy of IPL image (widthStep may differ) */

CVAPI(IplImage*) cvCloneImage( const IplImage* image );

如果设定了ROI等参数的时候，cvCopy只是复制被设定的区域，复制到一个和所设定参数相吻合的新的IplImage中

而cvCloneImage则是将整个IplImage结构复制到新的IplImage中，其中的ROI等参数也会一同复制。新的IplImage将会和原来的一模一样。

cvCopy的原型是：

void cvCopy( const CvArr* src, CvArr* dst, const CvArr* mask=NULL );

在使用这个函数之前，你必须用cvCreateImage（）一类的函数先开一段内存，然后传递给dst。cvCopy会把src中的数据复制到dst的内存中。如果mask(x,y)=0，则不对src/dst的(x,y)操作操作；如果mask(x,y)!=0, 则操作。

cvCloneImage的原型是：

IplImage* cvCloneImage( const IplImage* image );

在使用函数之前，不用开辟内存。该函数会自己开一段内存，然后复制好image里面的数据，然后把这段内存中的数据返回给你。

clone是把所有的都复制过来，也就是说不论你是否设置Roi,Coi等影响copy的参数，clone都会原封不动的克隆过来。

copy就不一样，只会复制ROI区域等。用clone复制之后，源图像在内存中消失后，复制的图像也变了，而用copy复制，源图像消失后，复制的图像不变。

使用cvCopy实现对不规制图形的提取：[http://artificialwistom.spaces.live.com/blog/cns!C4334BEEE0193F50!191.entry](http://artificialwistom.spaces.live.com/blog/cns!C4334BEEE0193F50!191.entry)



24、图像形态学操作
[http://blog.csdn.net/byxdaz/archive/2010/07/30/5775717.aspx](http://blog.csdn.net/byxdaz/archive/2010/07/30/5775717.aspx)



四、书籍推荐：

《opencv教程基础篇》大部分为OpenCV帮助手册的翻译，原创性内容不是很多。

《Learning OpenCV》深入浅出讲OpenCV函数背后的原理，比课堂教材生动且实用，极具实战功能。

[http://download.csdn.net/source/1860888](http://download.csdn.net/source/1860888)



五、相关资料：

项目主页：[http://sf.net/projects/opencvlibrary](http://sf.net/projects/opencvlibrary)

邮件列表：[http://groups.yahoo.com/group/OpenCV](http://groups.yahoo.com/group/OpenCV)

中文网站：[http://www.opencv.org.cn](http://www.opencv.org.cn/)

中文论坛：[http://www.opencv.org.cn/forum](http://www.opencv.org.cn/forum)](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=dst&t=blog)](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)





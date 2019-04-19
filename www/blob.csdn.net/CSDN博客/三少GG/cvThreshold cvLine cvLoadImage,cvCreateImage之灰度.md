# cvThreshold||cvLine||cvLoadImage,cvCreateImage之灰度 - 三少GG - CSDN博客
2011年08月04日 19:31:18[三少GG](https://me.csdn.net/scut1135)阅读数：3474标签：[image																[float																[wrapper																[数据结构																[存储																[matlab](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=wrapper&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
【Q1】**怎么样用opencv将彩色图片转化成像素值只有0和255的灰度图？**
进行灰度化，IplImage* pImg = cvLoadImage( "C:\\1.bmp", 0 );    这样图像已经灰度化，然后调用**cvThreshold****(image, image, 125, 255, CV_THRESH_BINARY);**   就可以了，125那里是你所用的阈值，这就是最简单的二值化，你要用ostu，或者别的高级一点的，就要自己写函数了
// Truncate values above 100.
**cvThreshold**( s, dst, 100, 255, **CV_THRESH_TRUNC** );//对大于100的像素值进行截断，大于100则为255，不大于100的为原值
【Q2】cvLine画直线
CvScalar color = CV_RGB(50,0,250);
cvLine( img1, p1, q1, color, 1, CV_AA, 0 );
cvLine( img1, p2, q2, color, 1, CV_AA, 0 );
cvLine( img1, p1, p2, color, 1, CV_AA, 0 );
cvLine( img1, q1, q2, color, 1, CV_AA, 0 );
or
cvLine(image,epipolarLinePoint1,epipolarLinePoint2,CV_RGB(0,255,0));
**cv::Line**
CV_RGB 创建一个色彩值. #define CV_RGB( r, g, b ) [编辑] cvScalar( (b), (g), (r) ) Line 绘制连接两个点的线段 voidcvLine(
 CvArr* img, CvPoint pt1, CvPoint pt2, CvScalar color, int thickness=1, int line_type=8, int shift=0 ); img 图像。 pt1 线段的第一个端点。 pt2 线段的第二个端点。 color 线段的颜色。 thickness 线段的粗细程度。 line_type 线段的类型。 8 (or 0) - 8-connected line（8 邻接)连接 线。 4 - 4-connected line(4 邻接)连接线。
 CV_AA - antialiased 线条。 shift 坐标点的小数点位数。 函数cvLine在图像中的点 1 和点 2 之间画一条线段。线段被图像或感兴趣的 矩形(ROI rectangle)所裁剪。对于具有整数坐标的 non-antialiasing 线条， 使用 8-连接或者 4-连接 Bresenham 算法。画粗线条时结尾是圆形的。画
 antialiased 线条使用高斯滤波。要指定线段颜色，用户可以使用使用宏 CV_RGB( r, g, b )。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
cvLoadImage函数使用方法
四、图像处理
1、图像的内存分配与释放
（1） 分配内存给一幅新图像:
IplImage* cvCreateImage(CvSize size, int depth, int channels);
    size: cvSize(width,height); 
    depth: 像素深度: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U, 
   IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F, IPL_DEPTH_64F 
    channels: 像素通道数. Can be 1, 2, 3 or 4.
        各通道是交错排列的. 一幅彩色图像的数据排列格式如下：
            b0 g0 r0 b1 g1 r1 ... 
示例:
**// Allocate a 1-channel byte imageIplImage* img1=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1); **
// Allocate a 3-channel float image
IplImage* img2=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,3);
（2） 释放图像:
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1); 
cvReleaseImage(&img);
（3） 复制图像:
IplImage* img1=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1); 
IplImage* img2;
img2=cvCloneImage(img1);  // 注意通过cvCloneImage得到的图像
                      // 也要用 cvReleaseImage 释放，否则容易产生内存泄漏
（4） 设置/获取感兴趣区域ROI:
void  cvSetImageROI(IplImage* image, CvRect rect);
void  cvResetImageROI(IplImage* image);
CvRect cvGetImageROI(const IplImage* image);
大多数OpenCV函数都支持 ROI.
（5） 设置/获取感兴趣通道COI:
void cvSetImageCOI(IplImage* image, int coi); // 0=all
int cvGetImageCOI(const IplImage* image);
大多数OpenCV函数不支持 COI.
2、图像读写
（1） 从文件中读入图像:
IplImage* img=0; 
  img=cvLoadImage(fileName);
  if(!img) printf("Could not load image file: %s\n",fileName);
 支持的图像格式: BMP, DIB, JPEG, JPG, JPE, PNG, PBM, PGM, PPM,
                          SR, RAS, TIFF, TIF
**OpenCV默认将读入的图像强制转换为一幅三通道彩色图像. 不过可以按以下方法修改读入方式:**
**img=cvLoadImage(fileName,flag);**
** flag: >0 将读入的图像强制转换为一幅三通道彩色图像       =0 将读入的图像强制转换为一幅单通道灰度图像       <0 读入的图像通道数与所读入的文件相同.**
（2） 保存图像:
if(!cvSaveImage(outFileName,img)) printf("Could not save: %s\n", outFileName);
保存的图像格式由 outFileName 中的扩展名确定.
3、访问图像像素
（1） 假设你要访问第k通道、第i行、第j列的像素。
（2） 间接访问: (通用，但效率低，可访问任意格式的图像)
    * 对于单通道字节型图像: 
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
CvScalar s;
s=cvGet2D(img,i,j); // get the (i,j) pixel value
printf("intensity=%f\n",s.val[0]);
s.val[0]=111;
cvSet2D(img,i,j,s); // set the (i,j) pixel value
    * 对于多通道字节型/浮点型图像: 
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,3);
CvScalar s;
s=cvGet2D(img,i,j); // get the (i,j) pixel value
printf("B=%f, G=%f, R=%f\n",s.val[0],s.val[1],s.val[2]);
s.val[0]=111;
s.val[1]=111;
s.val[2]=111;
cvSet2D(img,i,j,s); // set the (i,j) pixel value
（3） 直接访问: (效率高，但容易出错)
    * 对于单通道字节型图像: 
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
((uchar *)(img->imageData + i*img->widthStep))[j]=111;
    * 对于多通道字节型图像: 
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0]=111; // B
((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1]=112; // G
((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2]=113; // R
    * 对于多通道浮点型图像: 
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,3);
((float *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0]=111; // B
((float *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1]=112; // G
((float *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2]=113; // R
（4） 基于指针的直接访问: (简单高效)
    * 对于单通道字节型图像: 
IplImage* img  = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
int height     = img->height;
int width      = img->width;
int step       = img->widthStep/sizeof(uchar);
uchar* data    = (uchar *)img->imageData;
data[i*step+j] = 111;
    * 对于多通道字节型图像: 
IplImage* img  = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
int height     = img->height;
int width      = img->width;
int step       = img->widthStep/sizeof(uchar);
int channels   = img->nChannels;
uchar* data    = (uchar *)img->imageData;
data[i*step+j*channels+k] = 111;
    * 对于多通道浮点型图像（假设图像数据采用4字节（32位）行对齐方式）: 
IplImage* img  = cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,3);
int height     = img->height;
int width      = img->width;
int step       = img->widthStep/sizeof(float);
int channels   = img->nChannels;
float * data    = (float *)img->imageData;
data[i*step+j*channels+k] = 111;
（5） 基于 c++ wrapper 的直接访问: （更简单高效）
    * 首先定义一个 c++ wrapper ‘Image’，然后基于Image定义不同类型的图像: 
template<class T> class Image
{
  private:
  IplImage* imgp;
  public:
  Image(IplImage* img=0) {imgp=img;}
  ~Image(){imgp=0;}
  void operator=(IplImage* img) {imgp=img;}
  inline T* operator[](const int rowIndx) {
    return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
}; 
typedef struct{
  unsigned char b,g,r;
} RgbPixel; 
typedef struct{
  float b,g,r;
} RgbPixelFloat; 
typedef Image<RgbPixel>       RgbImage;
typedef Image<RgbPixelFloat>  RgbImageFloat;
typedef Image<unsigned char>  BwImage;
typedef Image<float>          BwImageFloat;
    * 对于单通道字节型图像: 
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
BwImage imgA(img);
imgA[i][j] = 111;
    * 对于多通道字节型图像: 
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
RgbImage  imgA(img);
imgA[i][j].b = 111;
imgA[i][j].g = 111;
imgA[i][j].r = 111;
    * 对于多通道浮点型图像: 
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,3);
RgbImageFloat imgA(img);
imgA[i][j].b = 111;
imgA[i][j].g = 111;
imgA[i][j].r = 111;
4、图像转换
（1） 字节型图像的灰度-彩色转换:
cvConvertImage(src, dst, flags=0);
 src = float/byte grayscale/color image
 dst = byte grayscale/color image
 flags = CV_CVTIMG_FLIP     (垂直翻转图像)
         CV_CVTIMG_SWAP_RB  (置换 R 和 B 通道)
（2） 彩色图像->灰度图像:
// Using the OpenCV conversion: 
cvCvtColor(cimg,gimg,CV_BGR2GRAY); // cimg -> gimg 
// Using a direct conversion: 
for(i=0;i<cimg->height;i++) for(j=0;j<cimg->width;j++) 
  gimgA[i][j]= (uchar)(cimgA[i][j].b*0.114 + 
                       cimgA[i][j].g*0.587 + 
                       cimgA[i][j].r*0.299);
（3） 不同彩色空间之间的转换:
cvCvtColor(src,dst,code); // src -> dst
 code    = CV_<X>2<Y>
 <X>/<Y> = RGB, BGR, GRAY, HSV, YCrCb, XYZ, Lab, Luv, HLS 
e.g.: CV_BGR2GRAY, CV_BGR2HSV, CV_BGR2Lab
5、绘图指令
（1） 绘制矩形:
// 在点 (100,100) 和 (200,200) 之间绘制一矩形，边线用红色、宽度为 1
cvRectangle(img, cvPoint(100,100), cvPoint(200,200), cvScalar(255,0,0), 1);
（2） 绘制圆形:
// 圆心为(100,100)、半径为20. 圆周绿色、宽度为1
cvCircle(img, cvPoint(100,100), 20, cvScalar(0,255,0), 1);
（3） 绘制线段:
// 在 (100,100) 和 (200,200) 之间、线宽为 1 的绿色线段
cvLine(img, cvPoint(100,100), cvPoint(200,200), cvScalar(0,255,0), 1);
（4） 绘制一组线段:
CvPoint  curve1[]={10,10,  10,100,  100,100,  100,10};
CvPoint  curve2[]={30,30,  30,130,  130,130,  130,30,  150,10};
CvPoint* curveArr[2]={curve1, curve2};
int      nCurvePts[2]={4,5};
int      nCurves=2;
int      isCurveClosed=1;
int      lineWidth=1; 
cvPolyLine(img,curveArr,nCurvePts,nCurves,isCurveClosed,cvScalar(0,255,255),lineWidth);
void cvPolyLine( CvArr* img, CvPoint** pts, int* npts, int contours, int is_closed,
                          CvScalar color, int thickness=1, int line_type=8, int shift=0 );
img       图像。 
pts       折线的顶点指针数组。 
npts     折线的定点个数数组。也可以认为是pts指针数组的大小 
contours   折线的线段数量。 
is_closed  指出多边形是否封闭。如果封闭，函数将起始点和结束点连线。 
color         折线的颜色。 
thickness  线条的粗细程度。 
line_type  线段的类型。参见cvLine。 
shift          顶点的小数点位数
（5） 绘制一组填充颜色的多边形:
cvFillPoly(img,curveArr,nCurvePts,nCurves,cvScalar(0,255,255));
cvFillPoly用于一个单独被多边形轮廓所限定的区域内进行填充。函数可以填充复杂的区域,例如，有漏洞的区域和有交叉点的区域等等。
void cvFillPoly( CvArr* img, CvPoint** pts, int* npts, int contours,CvScalar color, int line_type=8, int shift=0 );
img           图像。 
pts           指向多边形的数组指针。 
npts         多边形的顶点个数的数组。 
contours   组成填充区域的线段的数量。 
color         多边形的颜色。 
line_type  组成多边形的线条的类型。 
shift          顶点坐标的小数点位数。
（6） 文本标注:
CvFont font;
double hScale=1.0;
double vScale=1.0;
int    lineWidth=1;
cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);
cvPutText (img,"My comment",cvPoint(200,400), &font, cvScalar(255,255,0));
其它可用的字体类型有: CV_FONT_HERSHEY_SIMPLEX, CV_FONT_HERSHEY_PLAIN, CV_FONT_HERSHEY_DUPLEX, CV_FONT_HERSHEY_COMPLEX, CV_FONT_HERSHEY_TRIPLEX, CV_FONT_HERSHEY_COMPLEX_SMALL, CV_FONT_HERSHEY_SCRIPT_SIMPLEX, CV_FONT_HERSHEY_SCRIPT_COMPLEX,
/////////////////////////////////////////////////  参考  /////////////////
    　cvLoadImage( filename, -1 ); 默认读取图像的原通道数 
　　cvLoadImage( filename, 0 ); 强制转化读取图像为灰度图 
　　cvLoadImage( filename, 1 ); 读取彩色图 
　　例：将读入图像强制转换为灰度图像显示 
　　#include <highgui.h> 
　　#include <cv.h> 
　　int main(int argc, char **argv) 
　　{ 
　　if (argc != 2) 
　　return -1; 
　　/*强制转换为灰度图像*/ 
　　IplImage *img = cvLoadImage(argv[1], 0); 
　　cvNamedWindow("example"); 
　　cvShowImage("example", img); 
　　cvWaitKey(0); 
　　cvReleaseImage(&img); 
　　cvDestroyWindow("example"); 
　　return 0; 
　　}
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
不错的博客！！！
1.  [OpenCV的IplImage数据结构的各成员变量的含义](http://www.cppblog.com/lixiaona0417/archive/2011/03/05/141155.html)      如何访问data
[http://www.cppblog.com/lixiaona0417/archive/2011/03/05/141155.html](http://www.cppblog.com/lixiaona0417/archive/2011/03/05/141155.html)
2.  [图像的像素点操作【OpenCV学习笔记3】](http://blog.csdn.net/superdont/article/details/5910270)
[http://blog.csdn.net/superdont/article/details/5910270](http://blog.csdn.net/superdont/article/details/5910270)
3. 
### [文件的保存【OpenCV学习笔记4】](http://blog.csdn.net/superdont/article/details/5910280)
### [http://blog.csdn.net/superdont/article/details/5910280](http://blog.csdn.net/superdont/article/details/5910280)
- /*  
- 功能：读入图像文件，做图像反转，然后显示图像在屏幕上
- */
- #include "stdafx.h"
- #include <stdlib.h>
- #include <stdio.h>
- #include <math.h>
- #include <cv.h>
- #include <highgui.h>
- int main(int argc, char *argv[])  
- {  
-     IplImage* img = 0;   
- int height,width,step,channels;  
-     uchar *data;  
- int i,j,k;  
- if(argc<2)  
-     {  
-         printf("Usage: main <image-file-name>/n/7");  
-         exit(0);  
-     }  
- // 载入图像  
-     img=cvLoadImage(argv[1],-1);  
- if(!img)  
-     {  
-         printf("Could not load image file: %s/n",argv[1]);  
-         exit(0);  
-     }  
- // 获取图像信息
-     height    = img->height;    
-     width     = img->width;    
-     step      = img->widthStep;    
-     channels  = img->nChannels;  
-     data      = (uchar *)img->imageData;  
-     printf("Processing a %dx%d image with %d channels/n",height,width,channels);   
- // 创建窗口
-     cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);   
-     cvMoveWindow("mainWin", 100, 100);  
- // 反转图像
- for(i=0;i<height;i++)   
- for(j=0;j<width;j++)   
- for(k=0;k<channels;k++)  
-                 data[i*step+j*channels+k]=255-data[i*step+j*channels+k];  
- // 显示图像
-     cvShowImage("mainWin", img );  
-     cvWaitKey(0);  
-     cvReleaseImage(&img );  
-     printf("height=%d  width=%d step=%d channels=%d",height,width,step,channels);  
- return 0;  
- }  
- 
- //
- //IplImage* cvLoadImage( const char* filename, int flags=CV_LOAD_IMAGE_COLOR );
- //filename 
- //要被读入的文件的文件名。
- //flags 
- //指定读入图像的颜色和深度：
- //指定的颜色可以将输入的图片转为3信道(CV_LOAD_IMAGE_COLOR)也即彩色（>0）, 
- //单信道 (CV_LOAD_IMAGE_GRAYSCALE)也即灰色（=0）, 或者保持不变(CV_LOAD_IMAGE_ANYCOLOR)(<0)。
- //深度指定输入的图像是否转为每个颜色信道每象素8位，（OpenCV的早期版本一样），或者同输入的图像一样保持不变。
- //选中CV_LOAD_IMAGE_ANYDEPTH，则输入图像格式可以为8位无符号，16位无符号，32位有符号或者32位浮点型。
- //如果输入有冲突的标志，将采用较小的数字值。
- //比如CV_LOAD_IMAGE_COLOR | CV_LOAD_IMAGE_ANYCOLOR 将载入3信道图。
- //CV_LOAD_IMAGE_ANYCOLOR和CV_LOAD_IMAGE_UNCHANGED是等值的。
- //但是，CV_LOAD_IMAGE_ANYCOLOR有着可以和CV_LOAD_IMAGE_ANYDEPTH同时使用的优点，
- //所以CV_LOAD_IMAGE_UNCHANGED不再使用了。
- //如果想要载入最真实的图像，选择CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR。
- //函数cvLoadImage从指定文件读入图像，返回读入图像的指针。目前支持如下文件格式：
- //Windows位图文件 - BMP, DIB；
- //JPEG文件 - JPEG, JPG, JPE；
- //便携式网络图片 - PNG；
- //便携式图像格式 - PBM，PGM，PPM；
- //Sun rasters - SR，RAS；
- //TIFF文件 - TIFF，TIF;
- //OpenEXR HDR 图片 - EXR;
- //JPEG 2000 图片- jp2。
- //=======================================================================================
- //IplImage
- //
- //IPL 图像头
- //typedef struct _IplImage
- //    {
- //        int  nSize;         /* IplImage大小，=sizeof(IplImage)*/
- //        int  ID;            /* 版本 (=0)*/
- //        int  nChannels;     /* 大多数OPENCV函数支持1,2,3 或 4 个通道 */
- //        int  alphaChannel;  /* 被OpenCV忽略 */
- //        int  depth;         /* 像素的位深度: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U,
- //                               IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F 可支持 */
- //        char colorModel[4]; /* 被OpenCV忽略 */
- //        char channelSeq[4]; /* 被OpenCV忽略 */
- //        int  dataOrder;     /* 0 - 交叉存取颜色通道，对三通道RGB图像，像素存储顺序为BGR BGR BGR ... BGR；
- //                                     1 - 分开的颜色通道，对三通道RGB图像，像素存储顺序为RRR...R GGG...G BBB...B。
- //                                  cvCreateImage只能创建交叉存取图像 */
- //        int  origin;        /* 0 - 顶—左结构,
- //                               1 - 底—左结构 (Windows bitmaps 风格) */
- //        int  align;         /* 图像行排列 (4 or 8). OpenCV 忽略它，使用 widthStep 代替 */
- //        int  width;         /* 图像宽像素数 */
- //        int  height;        /* 图像高像素数*/
- //        struct _IplROI *roi;/* 图像感兴趣区域. 当该值非空只对该区域进行处理 */
- //        struct _IplImage *maskROI; /* 在 OpenCV中必须置NULL */
- //        void  *imageId;     /* 同上*/
- //        struct _IplTileInfo *tileInfo; /*同上*/
- //        int  imageSize;     /* 图像数据大小(在交叉存取格式下imageSize=image->height*image->widthStep），单位字节*/
- //        char *imageData;  /* 指向排列的图像数据 */
- //        int  widthStep;   /* 排列的图像行大小，以字节为单位 */
- //        int  BorderMode[4]; /* 边际结束模式, 被OpenCV忽略 */
- //        int  BorderConst[4]; /* 同上 */
- //        char *imageDataOrigin; /* 指针指向一个不同的图像数据结构（不是必须排列的），是为了纠正图像内存分配准备的 */
- //    }
- //    IplImage;
- //IplImage结构来自于 Intel Image Processing Library（是其本身所具有的）。OpenCV 只支持其中的一个子集:
- //alphaChannel 在OpenCV中被忽略。
- //colorModel 和channelSeq 被OpenCV忽略。
- //OpenCV颜色转换的唯一函数 cvCvtColor把原图像的颜色空间的目标图像的颜色空间作为一个参数。
- //dataOrder 必须是IPL_DATA_ORDER_PIXEL (颜色通道是交叉存取)，
- //然而平面图像的被选择通道可以被处理，就像COI（感兴趣的通道）被设置过一样。
- //align 是被OpenCV忽略的，而用 widthStep 去访问后继的图像行。
- //不支持maskROI 。处理MASK的函数把他当作一个分离的参数。
- //MASK在 OpenCV 里是 8-bit，然而在 IPL他是 1-bit。
- //tileInfo 不支持。
- //BorderMode和BorderConst是不支持的。
- //每个 OpenCV 函数处理像素的邻近的像素，
- //通常使用单一的固定代码边际模式。
- //除了上述限制，OpenCV处理ROI有不同的要求。
- //要求原图像和目标图像的尺寸或 ROI的尺寸必须
- //（根据不同的操作，例如cvPyrDown 目标图像的宽（高）必须等于原图像的宽（高）除以2 ±1)
- //精确匹配，而IPL处理交叉区域，如图像的大小或ROI大小可能是完全独立的。
- //======================================================================
- //Processing a 512x512 image with 3 channels
- //height=512  width=512 step=1536 channels=3请按任意键继续. . .
- //从上述关系可以看出
- //在C++内图像的存储是与Matlab不一样的
- //是在C++内存储的方式是按照行列方式，每一行内存储了该列（所有层）的信息。

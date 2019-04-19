# OpenCV学习笔记：基础结构 - Koma Hub - CSDN博客
2018年05月04日 16:29:27[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：74
#### CvPoint
**二维坐标系下的点，类型为整型**
```cpp
typedef struct CvPoint
 {
  int x; /* X坐标, 通常以0为基点 */
  int y; /* y坐标, 通常以0为基点 */
 }
CvPoint;
/* 构造函数 */
inline CvPoint cvPoint( int x, int y );
/* 从 CvPoint2D32f类型转换得来 */
inline CvPoint cvPointFrom32f( CvPoint2D32f point )
```
#### CvPoint2D32f
**二维坐标下的点，类型为浮点**
```cpp
typedef struct CvPoint2D32f
 {
  float x; /* X坐标, 通常以0为基点*/
  float y; /* Y坐标, 通常以0为基点*/
 }
CvPoint2D32f;
/* 构造函数 */
inline CvPoint2D32f cvPoint2D32f( double x, double y );
/* 从CvPoint转换来 */
inline CvPoint2D32f cvPointTo32f( CvPoint point );
```
#### CvPoint3D32f
**三维坐标下的点，类型为浮点**
```cpp
typedef struct CvPoint3D32f
 {
  float x; /* x-坐标, 通常基于0 */
  float y; /* y-坐标, 通常基于0 */
  float z; /* z-坐标, 通常基于0 */
 }
 CvPoint3D32f;
/* 构造函数 */
inline CvPoint3D32f cvPoint3D32f( double x, double y, double z );
```
#### CvSize
**矩形框大小，以像素为精度**
```cpp
typedef struct CvSize
 {
  int width;  /* 矩形宽 */
  int height; /* 矩形高 */
 }
 CvSize;
/* 构造函数 */
inline CvSize cvSize( int width, int height );
```
注意：构造函数的cv是小写！
#### CvSize2D32f
**以亚像素精度标量矩形框大小**
```cpp
typedef struct CvSize2D32f
 {
   float width;  /* 矩形宽 */
   float height; /* 矩形高 */
 }
 CvSize2D32f;
/* 构造函数*/
inline CvSize2D32f cvSize2D32f( double width, double height );
{
    CvSize2D32f s;
    s.width = (float)width;
    s.height = (float)height;
    return s;
}
```
#### CvRect
**矩形框的偏移和大小**
```cpp
typedef struct CvRect
 {
  int x; /* 方形的最左角的x-坐标  */
  int y; /* 方形的最上或者最下角的y-坐标 */
  int width; /* 宽 */
  int height; /* 高 */
 }
 CvRect;
/* 构造函数*/
inline CvRect cvRect( int x, int y, int width, int height );
{ 
      CvRect os;
      
      os.x = x;
      os.y = y;
      os.width = width;
      os.height = heigth;
      
      reture os;
}
```
#### CvScalar
**可存放在1-，2-，3-，4-TUPLE类型的捆绑数据的容器**
```cpp
typedef struct CvScalar
 {
  double val[4]
 }
 CvScalar;
/* 构造函数：用val0初始化val[0]用val1初始化val[1], 以此类推*/
inline CvScalar cvScalar( double val0, double val1,
                           double val2, double val3);
{  CvScalar  arr;
    
    arr.val[4] = {val0,val1,val2,val3};
  
    reture arr;}
/* 构造函数：用val0123初始化所有val[0]...val[3] */
inline CvScalar cvScalarAll( double val0123 );
 
  { CvScalar arr;
 
     arr.val[4] = {val0123,val0123,val0123,val0123,};
     reture arr;}
/* 构造函数：用val0初始化val[0],用0初始化val[1],val[2],val[3] */
inline CvScalar cvRealScalar( double val0 );
    { CvScalar arr;
       
       arr.val[4] = {val0};
   
       reture arr;}
```
[http://doc.blueruby.mydns.jp/opencv/classes/OpenCV/CvScalar.html](http://doc.blueruby.mydns.jp/opencv/classes/OpenCV/CvScalar.html)
#### CvTermCriteria
**迭代算法的终止准则**
```cpp
#define CV_TERMCRIT_ITER    1
#define CV_TERMCRIT_NUMBER  CV_TERMCRIT_ITER
#define CV_TERMCRIT_EPS     2
typedef struct CvTermCriteria
 {
  int    type;  /* CV_TERMCRIT_ITER 和CV_TERMCRIT_EPS二值之一，或者二者的组合 */
  int    max_iter; /* 最大迭代次数 */
  double epsilon; /* 结果的精确性 */
 }
 CvTermCriteria;
/* 构造函数 */
inline  CvTermCriteria  cvTermCriteria( int type, int max_iter, double epsilon );
/* 在满足max_iter和epsilon的条件下检查终止准则并将其转换使得type=CV_TERMCRIT_ITER+CV_TERMCRIT_EPS */
CvTermCriteria cvCheckTermCriteria( CvTermCriteria criteria,
                                    double default_eps,
                                    int default_max_iters );
```
#### CvMat
**多通道矩阵**
```cpp
typedef struct CvMat
 {
  int type; /* CvMat 标识 (CV_MAT_MAGIC_VAL), 元素类型和标记 */
  int step; /* 以字节为单位的行数据长度*/
  int* refcount; /* 数据引用计数 */
  union
   {
    uchar* ptr;
    short* s;
    int* i;
    float* fl;
    double* db;
   } data; /* data 指针 */
  #ifdef __cplusplus
  union
   {
     int rows;
     int height;
   };
  union
   {
     int cols;
     int width;
   };
  #else
   int rows; /* 行数 */
   int cols; /* 列数*/
  #endif
 } CvMat;
```
#### CvMatND
**多维、多通道密集数组**
```cpp
typedef struct CvMatND
    {
        int type; /* CvMatND 标识(CV_MATND_MAGIC_VAL), 元素类型和标号*/
        int dims; /* 数组维数 */
        int* refcount; /* 数据参考计数 */
        union
        {
            uchar* ptr;
            short* s;
            int* i;
            float* fl;
            double* db;
        } data; /* data 指针*/
        /* 每维的数据结构 (元素号,以字节为单位的元素之间的距离)是配套定义的 */
        struct
        {
            int size;
            int step;
        }
        dim[CV_MAX_DIM];
    } CvMatND;
```
#### CvSparseMat
**多维、多通道稀疏数组**
```cpp
typedef struct CvSparseMat
    {
        int type; /* CvSparseMat 标识 (CV_SPARSE_MAT_MAGIC_VAL), 元素类型和标号 */
        int dims; /* 维数 */
        int* refcount; /* 参考数量 - 未用 */
        struct CvSet* heap; /* HASH表节点池 */
        void** hashtable; /* HASH表:每个入口有一个节点列表，有相同的 "以HASH大小为模板的HASH值" */
        int hashsize; /* HASH表大小 */
        int total; /* 稀疏数组的节点数 */
        int valoffset; /* 数组节点值在字节中的偏移 */
        int idxoffset; /* 数组节点索引在字节中的偏移 */
        int size[CV_MAX_DIM]; /*维大小 */
    } CvSparseMat;
```
#### IplImage
**IPL 图像头**
```cpp
typedef struct _IplImage
    {
        int  nSize;         /* IplImage大小，=sizeof(IplImage)*/
        int  ID;            /* 版本 (=0)*/
        int  nChannels;     /* 大多数OPENCV函数支持1,2,3 或 4 个通道 */
        int  alphaChannel;  /* 被OpenCV忽略 */
        int  depth;         /* 像素的位深度: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16U,
                               IPL_DEPTH_16S, IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F 可支持 */
        char colorModel[4]; /* 被OpenCV忽略 */
        char channelSeq[4]; /* 被OpenCV忽略 */
        int  dataOrder;     /* 0 - 交叉存取颜色通道，对三通道RGB图像，像素存储顺序为BGR BGR BGR ... BGR；
                                     1 - 分开的颜色通道，对三通道RGB图像，像素存储顺序为RRR...R GGG...G BBB...B。
                                  cvCreateImage只能创建交叉存取图像 */
        int  origin;        /* 0 - 顶—左结构,
                               1 - 底—左结构 (Windows bitmaps 风格) */
        int  align;         /* 图像行排列 (4 or 8). OpenCV 忽略它，使用 widthStep 代替 */
        int  width;         /* 图像宽像素数 */
        int  height;        /* 图像高像素数*/
        struct _IplROI *roi;/* 图像感兴趣区域. 当该值非空只对该区域进行处理 */
        struct _IplImage *maskROI; /* 在 OpenCV中必须置NULL */
        void  *imageId;     /* 同上*/
        struct _IplTileInfo *tileInfo; /*同上*/
        int  imageSize;     /* 图像数据大小(在交叉存取格式下imageSize=image->height*image->widthStep），单位字节*/
        char *imageData;  /* 指向排列的图像数据 */
        int  widthStep;   /* 排列的图像行大小，以字节为单位 */
        int  BorderMode[4]; /* 边际结束模式, 被OpenCV忽略 */
        int  BorderConst[4]; /* 同上 */
        char *imageDataOrigin; /* 指针指向一个不同的图像数据结构（不是必须排列的），是为了纠正图像内存分配准备的 */
    }
    IplImage;
```
IplImage结构来自于 Intel Image Processing Library（是其本身所具有的）。OpenCV 只支持其中的一个子集:
alphaChannel 在OpenCV中被忽略。
colorModel 和channelSeq 被OpenCV忽略。OpenCV颜色转换的唯一函数 cvCvtColor将原图像的颜色空间和目标图像的颜色空间都作为一个参数。
dataOrder 必须是IPL_DATA_ORDER_PIXEL (颜色通道是交叉存取)，然而平面图像的被选择通道可以被处理，就像COI（感兴趣的通道）被设置过一样。
align 是被OpenCV忽略的，而用 widthStep 去访问后继的图像行。
不支持maskROI 。处理MASK的函数把他当作一个分离的参数。MASK在 OpenCV 里是 8-bit，然而在 IPL他是 1-bit。
tileInfo 不支持。
BorderMode和BorderConst是不支持的。每个 OpenCV 函数处理像素的邻近的像素，通常使用单一的固定代码边际模式。
除了上述限制，OpenCV处理ROI有不同的要求。要求原图像和目标图像的尺寸或 ROI的尺寸必须（根据不同的操作，例如cvPyrDown 目标图像的宽（高）必须等于原图像的宽（高）除以2 ±1)精确匹配，而IPL处理交叉区域，如图像的大小或ROI大小可能是完全独立的。
#### CvArr
**不确定数组**
```cpp
typedef void CvArr;
```
CvArr* 仅仅是被用于作函数的参数，用于指示函数接收的数组类型可以不止一个，如 IplImage*, CvMat* 甚至 CvSeq*. 最终的数组类型是在运行时通过分析数组头的前4 个字节判断。
内容来源：[OpenCV基础结构](http://wiki.opencv.org.cn/index.php/Cxcore%E5%9F%BA%E7%A1%80%E7%BB%93%E6%9E%84)

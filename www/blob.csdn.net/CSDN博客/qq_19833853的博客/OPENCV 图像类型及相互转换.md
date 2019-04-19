# OPENCV 图像类型及相互转换 - qq_19833853的博客 - CSDN博客
2016年11月29日 12:06:14[jessie Fei](https://me.csdn.net/qq_19833853)阅读数：368

**1 opencv中的几种常见的图像类型**
opencv中，几种常见的图像类型有：
IplImage，Mat，CvMat，CvArr 
CvArr ：
老版本的结构了。是一个抽象基类，在函数原型中，常见到CvArr（CvArr*），这就允许吧CvMar*
或者IplImage*
传递到程序或函数参数中了。
CvMat ：
矩阵结构，
IplImage ：
是较老版本的一种类型了，对图像进行”编码“的基本结构。这些图像可能是灰度，彩色，4通道的（RGB+
 alpha），其中，每个通道可以包含任意的整数或浮点数。
Mat：
新版本中的强大的一个图像容器，是和Matlab中的函数对应的。基本上讲 Mat 是一个类，由两个数据部分组成：矩阵头（包含矩阵尺寸，存储方法，存储地址等信息）和一个指向存储所有像素值的矩阵（根据所选存储方法的不同矩阵可以是不同的维数）的指针。矩阵头的尺寸是常数值，但矩阵本身的尺寸会依图像的不同而不同，通常比矩阵头的尺寸大数个数量级。
**2 opencv中存储图像类型转换**
**（1）将IplImage类型转换到Mat类型**
Mat::Mat(const IplImage* img, bool copyData=false);
默认情况下，新的Mat类型与原来的IplImage类型共享图像数据，转换只是创建一个Mat矩阵头。当将参数copyData设为true后，就会复制整个图像数据。
例：
IplImage*iplImg = cvLoadImage("greatwave.jpg", 1);
Matmtx(iplImg); // IplImage* ->Mat 
共享数据
// or : Mat mtx = iplImg;
**（2）将Mat类型转换到IplImage类型**
同样只是创建图像头，而没有复制数据。
例：
IplImage ipl_img = img; // Mat -> IplImage
**（3）将CvMat类型转换为Mat类型**
与IplImage的转换类似，可以选择是否复制数据。
Mat::Mat(const CvMat* m, bool copyData=false);
**（4）将Mat类型转换为CvMat类型**
与IplImage的转换类似，不复制数据，只创建矩阵头。
例：
// 假设Mat类型的imgMat图像数据存在
CvMat cvMat = imgMat; // Mat -> CvMat
**几个例子：**
为矩阵定义了一系列方便的操作符。我们可以将一个已经存在的灰度图像img变成全黑色：
img = Scalar(0);
选择感兴趣区域：
Rect r(10,
10,
100,
100);
Mat smallImg
= img(r);
将Mat转为
C API 数据类型：
Mat img = imread("image.jpg");
IplImage img1
= img;
CvMat m = img;
注意此处无数据复制操作。
将彩色图像转为灰度图像：
Mat img = imread("image.jpg");
*// loading a 8UC3 image*
Mat grey;
cvtColor(img, grey, CV_BGR2GRAY);
将图像的类型从8UC1转为32FC1：
src.convertTo(dst, CV_32F);
**3 总结**
还是要多参考，多用，映像才深刻


## [**opencv中Mat与IplImage,CVMat类型之间转换**](http://www.cnblogs.com/zcftech/archive/2013/04/10/3013027.html)
opencv中对图像的处理是最基本的操作，一般的图像类型为IplImage类型，但是当我们对图像进行处理的时候，多数都是对像素矩阵进行处理，所以这三个类型之间的转换会对我们的工作带来便利
Mat类型较CvMat和IplImage有更强的矩阵运算能力，支持常见的矩阵运算（参照Matlab中的各种矩阵运算），所以将IplImage类型和CvMat类型转换为Mat类型更易于数据处理。
Mat类型可用于直接存储图像信息，通过函数imread、imwrite、imshow等实现（与Matlab中的函数相似），似乎在某种程度上可以取代IplImage类型。
（1）将IplImage类型转换到Mat类型
Mat::Mat(const IplImage* img, bool copyData=false);
默认情况下，新的Mat类型与原来的IplImage类型共享图像数据，转换只是创建一个Mat矩阵头。当将参数copyData设为true后，就会复制整个图像数据。
例：
IplImage*iplImg = cvLoadImage("greatwave.jpg", 1);
Matmtx(iplImg); // IplImage* ->Mat 共享数据
// or : Mat mtx = iplImg;
（2）将Mat类型转换到IplImage类型
同样只是创建图像头，而没有复制数据。
例：
IplImage ipl_img = img; // Mat -> IplImage
（3）将CvMat类型转换为Mat类型
与IplImage的转换类似，可以选择是否复制数据。
Mat::Mat(const CvMat* m, bool copyData=false);
（4）将Mat类型转换为CvMat类型
与IplImage的转换类似，不复制数据，只创建矩阵头。
例：
// 假设Mat类型的imgMat图像数据存在
CvMat cvMat = imgMat; // Mat -> CvMat
OpenCV中IplImage图像格式与BYTE图像数据的转换
IplImage* iplImage；
BYTE* data；
1 由IplImage*得到BYTE*图像数据：
data = iplImage->imageDataOrigin; //未对齐的原始图像数据
或者
data = iplImage->imageData; //已对齐的图像数据
2 由BYTE*得到IplImage*图像数据
iplImage = cvCreateImageHeader(cvSize(width,height),depth,channels);
cvSetData(iplImage,data,step);
首先由cvCreateImageHeader()创建IplImage图像头，制定图像的尺寸，深度和通道数；然后由
cvSetData()根据 BYTE*图像数据指针设置IplImage图像头的数据数据，其中step指定该IplImage图像每行占的字节数，对于1通道的
IPL_DEPTH_8U图像，step可以等于width。
1，如果是从新创造一个Iplimage，则用IplImage* cvCreateImage( CvSize size, int depth,
 int channels )，它创建头并分配数据。
注：当不再使用这个新图像时，要调用void cvReleaseImage( IplImage** image )将它的头和图像数据释放！
2，如果有图像数据没有为图像头分配存储空间（即，没有为IplImage*
指针分配动态存储空间），则先调用IplImage* cvCreateImageHeader( CvSize size, int depth, int channels )创建图像头，再调用void
 cvSetData( CvArr* arr, void* data, int step )指定图像数据，可以理解为将这个新图
像的数据指针指向了一个已存在的图像数据上，不存在图像数据存储空间的分配操作。
注：当不再使用这个新图像时，要调用void cvReleaseImageHeader( IplImage** image )将它的图像头释放！
3，如果有图像数据也有图像头（用于IplImage为静态分配存储空间的情况），则先调用IplImage* cvInitImageHeader(
 CvSize size, int depth, int channels )更改图像头，再调用void
cvSetData( CvArr* arr, void* data, int step )指定图像数据。
注：因为这个新图像使用的是其它图像的数据和已有的图像头，所以不能使用 cvReleaseImage将它的头和图像数据释放，也不能使用cvReleaseData将它的图像数据释放！
4，如果从已有的一个图像创建，则用IplImage* cvCloneImage( const IplImage* image )，它制作图像的完整拷贝包括头、ROI和数据。
注：当不再使用这个新图像时，要调用void cvReleaseImage( IplImage** image )将它的头和图像数据释放！

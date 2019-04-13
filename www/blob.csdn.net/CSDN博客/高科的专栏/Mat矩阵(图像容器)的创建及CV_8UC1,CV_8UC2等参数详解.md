
# Mat矩阵(图像容器)的创建及CV_8UC1,CV_8UC2等参数详解 - 高科的专栏 - CSDN博客

2019年01月11日 14:38:09[高科](https://me.csdn.net/pbymw8iwm)阅读数：129


一）Mat矩阵(图像容器)创建时CV_8UC1,CV_8UC2等参数详解
1--Mat不但是一个非常有用的图像容器类,同时也是一个通用的矩阵类
2--创建一个Mat对象的方法很多

3--使用Mat图像容器类创建Mat类的对象
//! default constructor
Mat();
//! constructs 2D matrix of the specified size and type
// (_type is CV_8UC1, CV_64FC3, CV_32SC(12) etc.)
Mat(int rows, int cols, int type);
Mat(Size size, int type);
//! constucts 2D matrix and fills it with the specified value _s.
Mat(int rows, int cols, int type, const Scalar& s);
Mat(Size size, int type, const Scalar& s);
//! constructs n-dimensional matrix
Mat(int ndims, const int* sizes, int type);
Mat(int ndims, const int* sizes, int type, const Scalar& s);12345678910111213
4--使用Matlab风格的函数创建或者初始化Mat类的对象
//! Matlab-style matrix initialization
static MatExpr zeros(int rows, int cols, int type);
static MatExpr zeros(Size size, int type);
static MatExpr zeros(int ndims, const int* sz, int type);
static MatExpr ones(int rows, int cols, int type);
static MatExpr ones(Size size, int type);
static MatExpr ones(int ndims, const int* sz, int type);
static MatExpr eye(int rows, int cols, int type);
static MatExpr eye(Size size, int type);123456789
5--我们可以看见,创建Mat矩阵/图像容器类的很多构造方法或者其他成员方法在创建Mat对象的时候,都
需要指定type--所创建图像/矩阵的类型
6--那么类型是什么呢?OpenCv的源代码中说了一句:
(_type is CV_8UC1, CV_64FC3, CV_32SC(12) etc.)

7--同样,我们通过转到定义法,看一下CV_8UC1,CV_64FC3等这些宏到底是什么，OpenCv的源代码显示
如下（源代码在在types_c.h中）:
12
\#define CV_CN_MAX     512
\#define CV_CN_SHIFT   3
\#define CV_DEPTH_MAX  (1 << CV_CN_SHIFT)
\#define CV_8U   0
\#define CV_8S   1
\#define CV_16U  2
\#define CV_16S  3
\#define CV_32S  4
\#define CV_32F  5
\#define CV_64F  6
\#define CV_USRTYPE1 7
\#define CV_MAT_DEPTH_MASK       (CV_DEPTH_MAX - 1)
\#define CV_MAT_DEPTH(flags)     ((flags) & CV_MAT_DEPTH_MASK)
\#define CV_MAKETYPE(depth,cn) (CV_MAT_DEPTH(depth) + (((cn)-1) << CV_CN_SHIFT))
\#define CV_MAKE_TYPE CV_MAKETYPE
\#define CV_8UC1 CV_MAKETYPE(CV_8U,1)
\#define CV_8UC2 CV_MAKETYPE(CV_8U,2)
\#define CV_8UC3 CV_MAKETYPE(CV_8U,3)
\#define CV_8UC4 CV_MAKETYPE(CV_8U,4)
\#define CV_8UC(n) CV_MAKETYPE(CV_8U,(n))
\#define CV_8SC1 CV_MAKETYPE(CV_8S,1)
\#define CV_8SC2 CV_MAKETYPE(CV_8S,2)
\#define CV_8SC3 CV_MAKETYPE(CV_8S,3)
\#define CV_8SC4 CV_MAKETYPE(CV_8S,4)
\#define CV_8SC(n) CV_MAKETYPE(CV_8S,(n))
\#define CV_16UC1 CV_MAKETYPE(CV_16U,1)
\#define CV_16UC2 CV_MAKETYPE(CV_16U,2)
\#define CV_16UC3 CV_MAKETYPE(CV_16U,3)
\#define CV_16UC4 CV_MAKETYPE(CV_16U,4)
\#define CV_16UC(n) CV_MAKETYPE(CV_16U,(n))
\#define CV_16SC1 CV_MAKETYPE(CV_16S,1)
\#define CV_16SC2 CV_MAKETYPE(CV_16S,2)
\#define CV_16SC3 CV_MAKETYPE(CV_16S,3)
\#define CV_16SC4 CV_MAKETYPE(CV_16S,4)
\#define CV_16SC(n) CV_MAKETYPE(CV_16S,(n))
\#define CV_32SC1 CV_MAKETYPE(CV_32S,1)
\#define CV_32SC2 CV_MAKETYPE(CV_32S,2)
\#define CV_32SC3 CV_MAKETYPE(CV_32S,3)
\#define CV_32SC4 CV_MAKETYPE(CV_32S,4)
\#define CV_32SC(n) CV_MAKETYPE(CV_32S,(n))
\#define CV_32FC1 CV_MAKETYPE(CV_32F,1)
\#define CV_32FC2 CV_MAKETYPE(CV_32F,2)
\#define CV_32FC3 CV_MAKETYPE(CV_32F,3)
\#define CV_32FC4 CV_MAKETYPE(CV_32F,4)
\#define CV_32FC(n) CV_MAKETYPE(CV_32F,(n))
\#define CV_64FC1 CV_MAKETYPE(CV_64F,1)
\#define CV_64FC2 CV_MAKETYPE(CV_64F,2)
\#define CV_64FC3 CV_MAKETYPE(CV_64F,3)
\#define CV_64FC4 CV_MAKETYPE(CV_64F,4)
\#define CV_64FC(n) CV_MAKETYPE(CV_64F,(n))123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960
8--这里的type可以是任何的预定义类型，预定义类型的结构如下所示:
CV_<bit_depth>(S|U|F)C<number_of_channels>1
1--bit_depth---比特数---代表8bite,16bites,32bites,64bites---举个例子吧--比如说,如
如果你现在创建了一个存储--灰度图片的Mat对象,这个图像的大小为宽100,高100,那么,现在这张
灰度图片中有10000个像素点，它每一个像素点在内存空间所占的空间大小是8bite,8位--所以它对
应的就是CV_8
2--S|U|F--S--代表---signed int---有符号整形
U--代表--unsigned int--无符号整形
F--代表--float---------单精度浮点型
3--C<number_of_channels>----代表---一张图片的通道数,比如:
1--灰度图片--grayImg---是--单通道图像
2--RGB彩色图像---------是--3通道图像
3--带Alph通道的RGB图像--是--4通道图像

9--通过上面的讲解,现在，我们解读一下OpenCv的源代码:
1
//【1】CV_8UC1---则可以创建----8位无符号的单通道---灰度图片------grayImg
\#define CV_8UC1 CV_MAKETYPE(CV_8U,1)
\#define CV_8UC2 CV_MAKETYPE(CV_8U,2)
//【2】CV_8UC3---则可以创建----8位无符号的三通道---RGB彩色图像---colorImg
\#define CV_8UC3 CV_MAKETYPE(CV_8U,3)
//【3】CV_8UC4--则可以创建-----8位无符号的四通道---带透明色的RGB图像
\#define CV_8UC4 CV_MAKETYPE(CV_8U,4)
`#include <iostream>
using namespace std;
using namespace cv;
int main()
{
	Mat M(2, 2, CV_8UC3, Scalar(2, 255, 255));
	Mat M2(2, 2, CV_8U, Scalar(255));
	Mat M3(2, 2, CV_8UC2, Scalar(1, 255));

	Mat M4(2, 2, CV_16U, Scalar(677, 2, 255));
	Mat M5(2, 2, CV_32F, Scalar(5.34, 3.55, 255));
	cout << M << endl;
	cout << M5 << endl;
    return 0;
}`有兴趣的可以查看下M到M5的矩阵信息



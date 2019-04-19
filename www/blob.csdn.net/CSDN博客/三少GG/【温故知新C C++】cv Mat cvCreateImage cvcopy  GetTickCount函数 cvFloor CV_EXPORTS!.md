# 【温故知新C/C++】cv::Mat||cvCreateImage||cvcopy ||GetTickCount函数||cvFloor||CV_EXPORTS! - 三少GG - CSDN博客
2011年04月18日 20:33:00[三少GG](https://me.csdn.net/scut1135)阅读数：3219
cv::Mat
在网上看到了cv::Mat和IplImage之间的转换,放在这里以备后用
1.IplImage -> cv::Mat
這個範例程式裡 image.cpp有啊，做個簡單的範例。
Ex.
IplImage* pImg = cvLoadImage(“lena.jpg”);
cv**::Mat img(pImg,0); //0是不複製影像，也就是pImg與img的data共用同個記憶體位置，header各自有**
2.cv::Mat -> IplImage
cxcore.hpp裡，class cv::Mat裡面第831、832有寫
// converts header to IplImage; no data is copied operator IplImage() const;
範例接上面的過來..
Ex.
IplImage* pImg = cvLoadImage(“lena.jpg”);
cv::Mat img(pImg,0); //img拿到pImg的data
IplImage qImg;
qImg = IplImage(img); //一樣檔案沒複製，所以qImg.imageData就是指向pImg->imageData
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
cvCreateImage是openCV中的一个函数。OpenCV是Intel公司支持的开源计算机视觉库。 
　　cvCreateImage： 
　　创建头并分配数据 
　　IplImage* cvCreateImage( CvSize size, int depth, int channels ); 
　　参数说明： 
　　size 图像宽、高. 
　　depth 图像元素的位深度，可以是下面的其中之一： 
　　IPL_DEPTH_8U - 无符号8位整型 
　　IPL_DEPTH_8S - 有符号8位整型 
　　IPL_DEPTH_16U - 无符号16位整型 
　　IPL_DEPTH_16S - 有符号16位整型 
　　IPL_DEPTH_32S - 有符号32位整型 
　　IPL_DEPTH_32F - [单精度浮点数](view/2393515.htm)
　　IPL_DEPTH_64F - [双精度浮点数](view/2393520.htm)
　　channels： 
　　每个元素（像素）通道号.可以是 1, 2, 3 或 4.通道是交叉存取的，例如通常的彩色图像数据排列是：b0 g0 r0 b1 g1 r1 ... 虽然通常 IPL 图象格式可以存贮非交叉存取的图像，并且一些OpenCV 也能处理他, 但是这个函数只能创建交叉存取图像. 
　　函数 cvCreateImage 创建头并分配数据，这个函数是下列的缩写型式： 
　　header = cvCreateImageHeader(size,depth,channels); 
　　cvCreateData(header);
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
cvcopy : 拷贝一个数组给另一个数组 
　　void cvCopy( const CvArr* src, CvArr* dst, const CvArr* mask=NULL ); 
　　src 
　　输入数组。 
　　dst 
　　输出数组。 
　　mask 
　　操作掩码是8比特单通道的数组，它指定了输出数组中被改变的元素。 
　　函数cvCopy从输入数组中复制选定的成分到输出数组： 
　　如果mask(I)!=0,则dst(I)=src(I)。 
　　如果输入输出数组中的一个是IplImage类型的话，其ROI和COI将被使用。输入输出数组必须是同样的类型、维数和大小。函数也可以用来复制散列数组（这种情况下不支持mask）。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
GetTickCount函数　　函数功能：GetTickCount返回（retrieve）从[操作系统](view/880.htm)启动到现在所经过（elapsed）的毫秒数，它的返回值是DWORD。 
GetTickcount函数：它返回从操作系统启动到当前所经过的毫秒数，常常用来判断某个方法执行的时间，其函数原型是DWORD GetTickCount(void)，返回值以32位的双字类型DWORD存储，因此可以存储的最大值是2-1 ms约为49.71天，因此若系统运行时间超过49.71天时，这个数就会归0，MSDN中也明确的提到了:"Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days."。因此，如果是编写服务器端程序，此处一定要万分注意，避免引起意外的状况。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
函数 cvRound, cvFloor, cvCeil 用一种舍入方法将输入浮点数转换成整数。 cvRound 返回和参数最接近的整数值。** cvFloor 返回不大于参数的最大整数值**。cvCeil 返回不小于参数的最小整数值。在某些体系结构中该函数 工作起来比标准 C 操作起来还要快。
++++++++++++++++++++++++++++++++++++++++++++
首先说一下
class CV_EXPORTS Mat ；
**1.**
**CV_EXPORTS 的定义如下**：
#if (defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64 || defined WINCE) && defined CVAPI_EXPORTS
    #define CV_EXPORTS __declspec(dllexport)
#else
    #define CV_EXPORTS
#endif
_______
define CV_EXTERN_C extern "C"
#define CV_EXPORTS
#define CV_CDECL __cdecl
因此，解嵌套宏之后，CVAPI(void)就是用extern "C" CV_EXPORTS void __cdecl替换就行，其中，CV_EXPORTS因为定义的时候为空，所以在这里只是占位，没有用到，可以不考虑。
这我自己的一点理解，如有不对之处，请指正。谢谢
[http://hi.baidu.com/bolobolopig/blog/item/67f729df47d0571062279878.html](http://hi.baidu.com/bolobolopig/blog/item/67f729df47d0571062279878.html)
**2. IMPORT_C and EXPORT_C**
2007年12月06日 星期四 14:33
|**IMPORT_C**是一个宏，在 e32def.h 定义，意思是从 DLL 中导入函数。#define **IMPORT_C** __declspec(dllexport) 如果一个 DLL 函数要被导入到应用程序那么这个函数必须首先以 **IMPORT_C**声明，告诉编译器，这个函数可以在 DLL 中找到。**EXPORT_C 也是一个在 e32def.h 定义的宏，意思是从 DLL 中导出函数。****#define EXPORT_C __declspec(dllexport)****如果一个函数是 DLL API 的一部分， 并且要在应用程序或其他 DLL 中被调用，那么这个函数在C++源文件中必须实现为导出。**需要注意的是在 WINS 下， **IMPORT_C** 和 **EXPORT_C** 同时被定义为 __declspec(export)。|#define **IMPORT_C** __declspec(dllexport)|**#define EXPORT_C __declspec(dllexport)**|
|----|----|----|
|#define **IMPORT_C** __declspec(dllexport)| | |
|**#define EXPORT_C __declspec(dllexport)**| | |
**如果一个函数是 DLL API 的一部分， 并且要在应用程序或其他 DLL 中被调用，那么这个函数在C++源文件中必须实现为导出。**需要注意的是在 WINS 下， **IMPORT_C** 和 **EXPORT_C** 同时被定义为 __declspec(export)。 

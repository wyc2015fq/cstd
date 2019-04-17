# OpenCV+CUDA入门教程之五---GpuMat详解 - DumpDoctorWang的博客 - CSDN博客





2018年07月18日 21:01:14[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：3067










**目录**

[一、简介](#%E4%B8%80%E3%80%81%E7%AE%80%E4%BB%8B)

[二、构造函数](#2%E3%80%81%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)

[二、GpuMat::upload、GpuMat::download](#%E4%BA%8C%E3%80%81GpuMat%3A%3Aupload%E3%80%81GpuMat%3A%3Adownload)

[三、GpuMat与PtrStepSz、PtrStep](#%E4%B8%89%E3%80%81GpuMat%E4%B8%8EPtrStepSz%E3%80%81PtrStep)

[四、深复制与浅复制](#%E4%B8%89%E3%80%81%E6%B7%B1%E5%A4%8D%E5%88%B6%E4%B8%8E%E6%B5%85%E5%A4%8D%E5%88%B6)

[五、其他成员函数](#%E5%9B%9B%E3%80%81%E5%85%B6%E4%BB%96%E6%88%90%E5%91%98%E5%87%BD%E6%95%B0)

# 一、简介

GpuMat可以从其命名看出，它是“GPU”版本的Mat，绝大部分接口和Mat相同，功能也类似。

和Mat相比，GpuMat多了两个成员函数upload和download，分别用于把数据从内存上传(通过总线传输)到显存和从显存下载(通过总线传输)到内存。

GpuMat和Mat都有数据指针(指向一块存储区域，其中存放着图像数据)，不过GpuMat的数据指针是指向显存上的某一块区域，而Mat的数据指针是指向内存上的某一块区域。

**GpuMat仅支持二维数据；GpuMat::isContinuous() == false，**这是为了对齐内存，当一行的数据不是8字节(具体是几字节不知道)的倍数时，为了提高访问速度，这一行数据的末尾会填充空白字节，所以一行的实际内存大小保存在GpuMat的成员step里面。

# 二、构造函数

OpenCV3中，GpuMat的构造函数如下；OpenCV2中，构造函数没有allocator这个参数。

```cpp
//! 默认构造函数
explicit GpuMat(Allocator* allocator = defaultAllocator());

//! 根据指定大小(size)和类型(type)创建GpuMat实例
GpuMat(int rows, int cols, int type, Allocator* allocator = defaultAllocator());
GpuMat(Size size, int type, Allocator* allocator = defaultAllocator());

//! 根据指定大小和类型创建GpuMat，并用s初始化所有元素
GpuMat(int rows, int cols, int type, Scalar s, Allocator* allocator = defaultAllocator());
GpuMat(Size size, int type, Scalar s, Allocator* allocator = defaultAllocator());

//! 复制构造函数(浅复制)
GpuMat(const GpuMat& m);

//! 创建一个指定大小和类型的GpuMat；并使用由用户分配的d数据data作为初始数据
GpuMat(int rows, int cols, int type, void* data, size_t step = Mat::AUTO_STEP);
GpuMat(Size size, int type, void* data, size_t step = Mat::AUTO_STEP);

//! 创建ROI矩阵，就是把m的一部分作为新的矩阵
GpuMat(const GpuMat& m, Range rowRange, Range colRange);
GpuMat(const GpuMat& m, Rect roi);

//! 根据arr创建GpuMat，并把arr中的数据复制到GpuMat指定的显存区域
explicit GpuMat(InputArray arr, Allocator* allocator = defaultAllocator());
```

因此，构造GpuMat 的方式有如下几种

```cpp
const int rows = 16*50;
const int cols = 16*60;
const int type = CV_8UC3;

//第一种
GpuMat gpuMat;
//第二种
GpuMat gpuMat1(rows,cols,type);
GpuMat gpuMat2(Size(cols,rows),type);
//第三种，
GpuMat gpuMat3(rows,cols,type,Scalar(0,255,0));
GpuMat gpuMat4(Size(cols,rows),type,Scalar(0,255,0));
//第四种
GpuMat gpuMat5 = gpuMat1;
//第五种
uchar *data = new uchar[rows*cols*3]();//像素个数乘以3
GpuMat gpuMat6(rows,cols,type,data);
GpuMat gpuMat7(Size(cols,rows),type,data);
//第六种
Range rowRange(0,16*10),colRange(0,16*10);
GpuMat gpuMat8(gpuMat1,rowRange,colRange);
Rect rect(0,0,16*10,16*10);
GpuMat gpuMat9(gpuMat1,rect);
//第七种
Mat mat(rows,cols,type,Scalar(0,255,0));
GpuMat gpuMat10(mat);
```

# 二、GpuMat::upload、GpuMat::download

 下面是OpenCV2的函数

```cpp
//! 把m中的数据上传到GpuMat指定的显存中
void upload(const Mat& m);

//! 把GpuMat指定的显存中，把数据下载到m里
void download(Mat& m) const;
```

下面是opencv3的函数

```cpp
//! 把arr中的数据上传到GpuMat，上传完成前会阻塞当前线程
void upload(InputArray arr);

//! 异步地把arr中的数据上传到GpuMat，不会阻塞当前线程
void upload(InputArray arr, Stream& stream);

//! 把GpuMat中的数据下载到dst，下载完成前会阻塞当前线程
void download(OutputArray dst) const;

//! 异步地把GpuMat中的数据下载到dst，下载完成前不会阻塞当前线程
void download(OutputArray dst, Stream& stream) const;
```

可以看出，opencv3中，upload和download都重载有参数stream的版本，这是把上传下载的操作放到stream中，让stream去管理，然后立即返回，CPU可以继续做其他事情，这就是异步的上传/下载数据。在opencv2中，异步上传下载的函数在stream里面。异步的上传下载数据，将在后面的博客中讲解。接下来演示如何使用upload/download函数。

```cpp
//main.cpp
#include <iostream>
//--------------------OpenCV头文件---------------
#include <opencv2/opencv.hpp>
#include <opencv2/core/version.hpp>
using namespace cv;

#if CV_VERSION_EPOCH == 2
#define OPENCV2
#include <opencv2/gpu/gpu.hpp>
using namespace cv::gpu;

#elif CV_VERSION_MAJOR == 3
#define  OPENCV3
#include <opencv2/core/cuda.hpp>
using namespace cv::cuda;

#else
#error Not support this OpenCV version
#endif
//--------------------OpenCV头文件---------------

using namespace std;

int main() {
    // 首先要检查是否CUDA模块是否可用
    if(getCudaEnabledDeviceCount()==0){
        cerr<<"此OpenCV编译的时候没有启用CUDA模块"<<endl;
        return -1;
    }

    const int rows = 16*50;
    const int cols = 16*60;
    const int type = CV_8UC3;

    // 初始化一个黑色的GpuMat
    GpuMat gpuMat(rows,cols,type,Scalar(0,0,0));
    // 定义一个空Mat
    Mat dst;
    // 把gpuMat中数据下载到dst(从显存下载到内存)
    gpuMat.download(dst);
    // 显示
    imshow("show",dst);
    waitKey(0);

    // 读取一张图片
    Mat arr = imread("../../GpuMat/lena.jpg");
    imshow("show",arr);
    waitKey(0);

    // 上传到gpuMat(若gpuMat不为空，会先释放原来的数据，再把新的数据上传上去)
    gpuMat.upload(arr);
    // 定义另外一个空的GpuMat
    GpuMat gray;
    // 把gpuMat转换为灰度图gray
#ifdef OPENCV3
    cuda::cvtColor(gpuMat,gray,CV_BGR2GRAY);
#else
    gpu::cvtColor(gpuMat,gray,CV_BGR2GRAY);
#endif
    // 下载到dst，如果dst不为空，旧数据会被覆盖
    gray.download(dst);
    // 显示
    imshow("show",dst);
    waitKey(0);
    return 0;
}
```

**注意：**有的函数，例如cvtColor，在cv和cv::cuda(在opencv2中是cv::gpu)这两个命名空间中有相同的定义，可在函数名字前面加上cuda::(在opencv2是gpu::)就不会出现冲突。

# 三、GpuMat与PtrStepSz、PtrStep

PtrStepSz和PtrStep是两个轻量级的类，一般作为CUDA核函数的参数，为了提高性能而设计(越简单，复制的代价越小)。

PtrStep和PtrStepSz都是由GpuMat的成员函数生成，我们看下这两个函数的实现

```cpp
template <class T> inline
GpuMat::operator PtrStepSz<T>() const
{
    return PtrStepSz<T>(rows, cols, (T*)data, step);
}

template <class T> inline
GpuMat::operator PtrStep<T>() const
{
    return PtrStep<T>((T*)data, step);
}
```

可见， PtrStepSz仅仅包含了GpuMat中的rows、cols、step、data(数据指针)；而PtrStep仅包含GpuMat中的step和data这两个成员。PtrStepSz中的Ptr(指针)代表data；Step代表step；Sz则是Size的缩写，代表rows和cols。PtrStep中的Ptr代表data；Step则代表step。

那么如何通过GpuMat得到PtrStepSz和PtrStep呢？请看如下代码

```cpp
//common.h
#ifndef OCSAMPLE_COMMON_H
#define OCSAMPLE_COMMON_H

#include <iostream>
//--------------------OpenCV头文件---------------
#include <opencv2/opencv.hpp>
#include <opencv2/core/version.hpp>
using namespace cv;

#if CV_VERSION_EPOCH == 2
#define OPENCV2
#include <opencv2/gpu/gpu.hpp>
using namespace cv::gpu;

#elif CV_VERSION_MAJOR == 3
#define  OPENCV3
#include <opencv2/core/cuda.hpp>
using namespace cv::cuda;

#else
#error Not support this OpenCV version
#endif
//--------------------OpenCV头文件---------------

using namespace std;
#endif //OCSAMPLE_COMMON_H
```

```cpp
//main.cu
#include "../common.h"

//---------------------CUDA头文件----------------
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cuda_device_runtime_api.h>
//---------------------CUDA头文件----------------

__global__ void kernel(PtrStepSz<uchar3> src){

}

int main(){
    // 首先要检查是否CUDA模块是否可用
    if(getCudaEnabledDeviceCount()==0){
        cerr<<"此OpenCV编译的时候没有启用CUDA模块"<<endl;
        return -1;
    }
    const int rows = 16*50;
    const int cols = 16*60;
    const int type = CV_8UC3;

    // 初始化一个黑色的GpuMat
    GpuMat gpuMat(rows,cols,type,Scalar(0,0,0));
    kernel<<<1,1>>>(gpuMat);
}
```

 kernel是一个CUDA Kernel，参数类型是PtrStepSz<uchar3>，当把gpuMat作为函数kernel的参数传进去的时候，会自动调用GpuMat::operator PtrStepSz<T>() const这个函数，生成一个PtrStepSz的实例src；**当修改src里面的数据时，gpuMat里面的数据也会同步的发生变化。**至于怎么修改src里面的数据，将会在下篇中讲解。

# 四、深复制与浅复制

和Mat一样，浅复制只复制GpuMat头信息和数据指针，两个GpuMat的数据指针指向同一块区域，修改其中任意一个GpuMat的数据都会相互影响。而深复制则是把数据也复制到新的GpuMat里面，两个GpuMat的数据指针不会相互影响。

```cpp
//main.cpp
#include "common.h"

int main(){
    // 首先要检查是否CUDA模块是否可用
    if(getCudaEnabledDeviceCount()==0){
        cerr<<"此OpenCV编译的时候没有启用CUDA模块"<<endl;
        return -1;
    }

    const int rows = 16*50;
    const int cols = 16*60;
    const int type = CV_8UC3;

    // 初始化一个黑色的GpuMat
    GpuMat gpuMat(rows,cols,type,Scalar(0,0,0));

    // 浅复制，gpuMat或gpuMat1两者的数据指针指向同一块区域
    GpuMat gpuMat1 = gpuMat;

    // 深复制,gpuMat2和gpuMat3的数据指针指向的数据区域和gpuMat的不同
    GpuMat gpuMat2 = gpuMat.clone();
    GpuMat gpuMat3;
    gpuMat.copyTo(gpuMat3);

    // 测试
    Mat dst1,dst2,dst3,dst4;
    // 把gpuMat设置为绿色
    gpuMat.setTo(Scalar(0,255,0));
    gpuMat.download(dst1);
    gpuMat1.download(dst2);
    gpuMat2.download(dst3);
    gpuMat3.download(dst4);
    imshow("dst1",dst1);//绿色
    imshow("dst2",dst2);//绿色
    imshow("dst3",dst3);//黑色
    imshow("dst4",dst4);//黑色
    waitKey(0);
}
```

# 五、其他成员函数

其他成员函数的用法和Mat的相同，不再一一赘述。




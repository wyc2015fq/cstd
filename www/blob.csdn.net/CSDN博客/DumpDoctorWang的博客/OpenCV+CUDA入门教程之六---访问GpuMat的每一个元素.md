# OpenCV+CUDA入门教程之六---访问GpuMat的每一个元素 - DumpDoctorWang的博客 - CSDN博客





2018年07月18日 21:01:38[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1469








**目录**

[一、CUDA极简入门教程](#%E4%B8%80%E3%80%81CUDA%E6%9E%81%E7%AE%80%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B)

[二、访问GpuMat的每个元素](#%E4%BA%8C%E3%80%81%E8%AE%BF%E9%97%AEGpuMat%E7%9A%84%E6%AF%8F%E4%B8%AA%E5%85%83%E7%B4%A0)

# 一、CUDA极简入门教程

本部分只是CUDA 的一个超级简单且不完整的内容，关于CUDA配置和编程，请参考官方文档或其他教程。

**1、Kernel**

Kernel是在GPU上执行的函数，访问的数据都应该在显存中；函数没有返回值，需用void作为返回类型；语法和C++相同，也能使用C++的一些标准库函数(因为这些库函数有GPU实现，不过函数名字和参数相同而已)。kernel是函数的名字，可以随便改。

```cpp
__global__ void kernel(参数1,参数2,...){
    int i = threadIdx.x + blockIdx.x * blockDim.x; //列坐标
    int j = threadIdx.y + blockIdx.y * blockDim.y; //行坐标
}
```

`__global__是Kernel的一个标识符，与之相对的还有__host__和__device__；由__host__标识的函数和普通函数无异，在CPU上执行；__device__标识的函数只能有__gloabal__标识的函数调用或者被其他用__device__标识的函数调用。`

**2、线程组织模型**

GPU有很多个流处理器，每个流处理器相互独立，可以执行不同的代码；每个流处理器里面还有很多小核心，这些核心执行相同的代码，不过可以访问不同的数据，因此，这些小核心就可以用来进行并行计算。在CUDA的线程模型里面，这些小核心对应的概念叫做Thread，每个Thread都可以计算出一个全局唯一从0开始的的id(id可以是一维的，也可以是二维的)，用这个id，就可以访问数组或者矩阵(Mat)中指定的元素，对这些元素进行计算。

**1）、一维线程模型**

一个GPU有很多个Grid，这些Grid组成一行；每个Grid有很多个Block，这些Block组成一行；每个Block有很多个Thread，这些Thread组成一行。因此整个GPU，就是由一行超长的Thread组成。Grid有id，不过很少用到，就不讲了；Block有一个从0开始的id，在不同的Grid里面是重复的，每个Block的长度保存在blockDim.x里面；每个Thread有一个从0开始的id，在不同的Block里面是重复的。

**每一个thread的全局唯一id**，就可以通过下面的代码算出来

```cpp
int id = threadIdx.x + blockIdx.x*blockDim.x;
```

**2）、二维线程模型**

个GPU有很多个二维Grid，每个Grid有很多个二维Block，每个Block有很多个Thread，所有的thread组成整个平面。下面这图是在官方文档中Copy过来的图，这就是GPU的二维线程模型。

![](https://img-blog.csdn.net/20180718102802775?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 GPU的线程组织为，一个GPU中有数个Grid，每个Grid有自己的id，不过Grid 的id几乎用不到。

一个Grid由M*N个Block组成，每个Block有自己的id(在不同的Grid里面是重复的)，例如，左上角的第一个Block的id就是(0,0)，第二行第三列的那个Block的id就是(2,1)；在Kernel中，当前Block的id存在blockIdx这个变量里面，每个Block的大小存在blockDim这个变量里面；blockIdx和blockDim都有三个分量，分别为x、y、z，因为图像处理几乎都是二维情况，所以目前只使用到x、y这两个分量，blockIdx.x是Block的列坐标，blockIdx.y分量是行坐标，blockDim.x是Block的列数，blockDim.y是Block的行数。

一个Block有K*J个Thread组成，每个Thread由自己的id(在不同的block里面是重复的)。在Kernel中，当前Thread的id存在threadIdx这个变量里面；threadIdx有三个分量，分别为x、y、z，因为图像处理几乎都是二维情况，所以目前只使用到x、y这两个分量，threadIdx.x是Thread的列坐标，threadIdx.y分量是行坐标，blockDim.x是Block的列数，blockDim.y是Block的行数。

**每一个thread的全局唯一id**，就可以通过下面的代码算出来

```cpp
int i = threadIdx.x + blockIdx.x * blockDim.x; // 列坐标
int j = threadIdx.y + blockIdx.y * blockDim.y; // 行坐标
```

**3、第一个CUDA程序**，一维线程模型示例

Kernel函数的调用形式为：函数名<<<Block数量, Block的大小>>>(参数1, 参数2, ...)

```cpp
// 命名为main.cu
#include <iostream>
#include <string>
#include <stdexcept>
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace std;

#define SIZE 100

// 检查cuda函数的返回值,出错的时候抛出异常
#define CE(status,error_msg) \
            if (status != cudaSuccess) \
            {\
                char err_buffer[2048];\
                sprintf(err_buffer,"第%d行： %s，错误详细信息>> %s\n",__LINE__,error_msg,cudaGetErrorString(status));\
                throw runtime_error(err_buffer);\
            }

/**
 * Keernel。计算c=a+b
 * @param a 数组a
 * @param b 数组b
 * @param c 数组c
 */
__global__ void add_kernel(int *a, int *b, int *c){
    int id = threadIdx.x + blockIdx.x*blockDim.x; // 获取当前thread的id
    if(id<SIZE)  //! 要判断是否越界！！！
        c[id] = a[id]+b[id];
}


void add_caller(int *a, int *b, int *c, size_t size) {
    int *dev_a = nullptr;
    int *dev_b = nullptr;
    int *dev_c = nullptr;

    CE(cudaSetDevice(0),"初始化设备失败");

    // 在GPU上分配显存
    CE(cudaMalloc(&dev_a,size* sizeof(int)),"分配内存失败");
    CE(cudaMalloc(&dev_b,size* sizeof(int)),"分配内存失败");
    CE(cudaMalloc(&dev_c,size* sizeof(int)),"分配内存失败");

    // 从内存把数据复制到显存
    CE(cudaMemcpy(dev_a,a, size * sizeof(int),cudaMemcpyHostToDevice),"复制数据失败");
    CE(cudaMemcpy(dev_b,b, size * sizeof(int),cudaMemcpyHostToDevice),"复制数据失败");

    // 调用Kernel
    size_t numBlock = 2;
    size_t blockSize = size/2;
    add_kernel<<<numBlock,blockSize>>>(dev_a,dev_b,dev_c);

    // 等待Kernel执行完
    CE(cudaThreadSynchronize(),"同步失败");

    // 从显存中把数据复制回内存
    CE(cudaMemcpy(c,dev_c, size * sizeof(int),cudaMemcpyDeviceToHost),"复制数据失败");

    // 释放显存
    CE(cudaFree(dev_a),"释放内存失败");
    CE(cudaFree(dev_b),"释放内存失败");
    CE(cudaFree(dev_c),"释放内存失败");
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    const size_t size = SIZE;
    int a[size],b[size],c[size];

    for(int i=0;i<size;i++){
        a[i] = i;
        b[i] = i;
    }

    add_caller(a,b,c,size);

    for (int i : c) {
        cout<< i <<endl;
    }

    return 0;
}
```

 CMakeLists.txt

```cpp
# required cmake version
cmake_minimum_required(VERSION 3.7)
# CMakeLists.txt for G4CU project
project(cudatest)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS  "-std=c++11")

# packages
find_package(CUDA REQUIRED)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF) 		#这一句解决 cannot find -lopencv_dep_cudart
# nvcc flags
set(CUDA_NVCC_FLAGS -gencode arch=compute_52,code=sm_52;-G;-g)

file(GLOB CURRENT_SOURCES main.cu)

CUDA_ADD_EXECUTABLE(cudatest ${CURRENT_SOURCES})

target_link_libraries( ${PROJECT_NAME})
```

# 二、访问GpuMat的每个元素

要访问GpuMat的每一个元素，实现自定义的算法，就得自己重新实现一个Kernel，然后把GpuMat作为参数传进去。但实际上，为了提高程序性能，一般不直接使用GpuMat作为参数，而是使用它的精简版PtrStepSz或者PtrStep代替。

接下这个程序是修改一个纯黑GpuMat的每一个像素的颜色，每个Thread处理一个像素，需要的线程数量就是gpuMat.rows*gpuMat.cols。

重要说明：
- Block数量只能多不能少，否则有的像素访问不到。
- 观察全局Thread id是怎么算的。
- 在Kernel里面一定要判断是否越界。当然，rows和cols分别是threadsPerBlock.x和threadsPerBlock.y的整倍数时，不需要判断。
- 访问src的一个元素的方法是src(行坐标, 列坐标)。

```cpp
// main.cu
#include "../common.h"

//---------------------CUDA头文件----------------
#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cuda_device_runtime_api.h>
//---------------------CUDA头文件----------------

/**
 * CUDA kernel,在GPU上执行的函数。
 * 上千个线程都是执行这个函数，每个Thread根据全局id作为坐标来访问像素
 * @param src 类型是PtrStepSz<uchar3>，相当于是GpuMat的精简版
 */
__global__ void kernel(PtrStepSz<uchar3> src){
    int i = threadIdx.x + blockIdx.x * blockDim.x;  // 列，竖直方向的全局Thread id
    int j = threadIdx.y + blockIdx.y * blockDim.y;  // 行，水平方向的全局Thread id
//    printf("%d %d\n",i,j);                        //可用printf来debug
    if(j<src.rows && i<src.cols)                    //! 要判断是否越界！！！
        src(j,i) = make_uchar3(j%255,i%255,0);      // 访问一个像素的方式是src(行坐标,列坐标)
    __syncthreads();                                // 同步所有线程
}

int main() {
    const int rows = 16*50;
    const int cols = 16*60;
    if(getCudaEnabledDeviceCount()==0){
        cerr<<"此OpenCV编译的时候没有启用CUDA模块"<<endl;
        return -1;
    }
    // 定义一个纯黑GpuMat
    GpuMat gpuMat(rows,cols,CV_8UC3);
    // 每个block共有16*16个线程
    dim3 threadsPerBlock(16, //一个block有多少列
                        16); //一个block有多少行
    // 计算竖直需要多少个block
    uint block_num_vertical = (rows+threadsPerBlock.x-1)/threadsPerBlock.x;
    // 计算水平需要多少个block
    uint block_num_horizontal = (cols+threadsPerBlock.y-1)/threadsPerBlock.y;
    dim3 numBlocks(block_num_horizontal, // 列的方向的block数目
                   block_num_vertical);  // 行的方向的block数目
    // 调用kernel，gpuMat的类型会自动转换为PtrStepSz<uchar3>
    kernel<<<numBlocks,threadsPerBlock>>>(gpuMat);
    // 从显存把数据下载到内存
    Mat local;
    gpuMat.download(local);
    // 显示
    imshow("s",local);
    imwrite("../../s.jpg",local);
    waitKey(0);
    return 0;
}
```

 kernel的参数是PtrStepSz<uchar3>，类型uchar3是个结构体，有三个分量x,y,z。为什么使用uchar3呢，因为gpuMat的类型是CV_8UC3，就是每个元素有三个分量都是uchar这种类型的，当使用uchar3作为PtrStepSz的元素类型时，src(j,i)刚好返回第(j,i)个像素的引用。再举一个例子，如果gpuMat的类型是CV_32FC2(每个元素有两个float)，那么，kenel的参数类型就应该为PtrStepSz<float2>或者PtrStep<float2>。

我使用cmake编译，CMakeLists.txt在本系列的第一篇文章里。编译运行上述代码，最后可得到下图

![](https://img-blog.csdn.net/20180718151402596?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注：实际上这种一个Thread处理一个像素，对显卡的性能浪费非常严重，更高效的方法是一个线程处理多个像素，同时，同个block里面的相邻线程处理相邻的像素。




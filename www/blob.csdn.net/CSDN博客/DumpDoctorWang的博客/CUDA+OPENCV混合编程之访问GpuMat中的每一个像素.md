# CUDA+OPENCV混合编程之访问GpuMat中的每一个像素 - DumpDoctorWang的博客 - CSDN博客





2017年11月27日 17:39:58[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1660








## 一、环境配置

Ubuntu + CUDA + OPENCV。先安装NVIDIA显卡驱动，再安装CUDA，最后编译安装OpenCV(加上了CUDA的模块)，具体怎么做有很多教程，我就不多说了。

## 二、代码##

```
//main.cpp
#include <iostream>
#include "kernel.h"

#define WIDTH 1024
#define HEIGHT 512
int main()
{
    GpuMat mat(HEIGHT,WIDTH,CV_8UC3);

    generateFrameCaller(WIDTH,HEIGHT,mat);
    HANDLECUDAERROR(cudaGetLastError()); //Check error after call kernel function
    Mat dst;
    mat.download(dst);
    imshow("result",dst);
    waitKey(0);
    return 0;
}
```

```
//kernel.h
#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED

#include <cuda.h>
#include <cuda_runtime.h>
#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/core/cuda_devptrs.hpp>
#include <opencv2/gpu/stream_accessor.hpp>

using namespace cv;
using namespace cv::gpu;

void generateFrameCaller(int width,int height,PtrStep<uchar3> mat);

#endif // KERNEL_H_INCLUDED
```

```
//kernel.cu
#include "kernel.h"
/**
 *　访问图片中的每一个像素
 * @param width mat对应的宽度
 * @param height mat对应的高度
 * @param mat GpuMat被强转为PtrStep<>类型之后的参数
 */
__global__ void generateFrame(int width,int height,PtrStep<uchar3> mat)
{
    int i = threadIdx.x + blockIdx.x * blockDim.x; //列坐标
    int j = threadIdx.y + blockIdx.y * blockDim.y; //行坐标

    if(i < width && j < height)
    {
        mat(j,i) = make_uchar3(255,123,123);     //mat(j,i)才是正确打开方式
    }
}

/**
 *　调用内核函数的主机函数
 * @param width mat对应的宽度
 * @param height mat对应的高度
 * @param mat GpuMat被强转为PtrStep<>类型之后的参数
 */
void generateFrameCaller(int width,int height,PtrStep<uchar3> mat)
{
    dim3 blocks(height/32,width/32);
    dim3 threads(32,32);
    generateFrame<<<blocks,threads>>>(width,height,mat);
}
```

```
# CMakeLists.txt
# required cmake version
cmake_minimum_required(VERSION 2.8)
# CMakeLists.txt for G4CU project
project(cudatest)

# packages
find_package(CUDA REQUIRED)
find_package(Ceres REQUIRED)
#set(OpenCV_DIR "/usr/local/include") 	    #修改了这一句，指定opencv头文件的路径
find_package(OpenCV REQUIRED)

set(CMAKE_BUILD_TYPE "RELEASE")
set(CUDA_USE_STATIC_CUDA_RUNTIME OFF) 		#这一句解决 cannot find -lopencv_dep_cudart
# nvcc flags
#set(CUDA_NVCC_FLAGS -gencode arch=compute_20,code=sm_20;-G;-g)
set(CUDA_NVCC_FLAGS -gencode arch=compute_52,code=sm_52;-G;-g)

include_directories( ${OPENCV_INCLUDE_DIRS})
include_directories( ${CERES_INCLUDE_DIRS})
include_directories(include)

file(GLOB_RECURSE CURRENT_HEADERS  include/*.h include/*.hpp include/*.cuh)
file(GLOB CURRENT_SOURCES  src/*.c src/*.cpp src/*.cu)
source_group("Include" FILES ${CURRENT_HEADERS}) 
source_group("Source" FILES ${CURRENT_SOURCES}) 

CUDA_ADD_EXECUTABLE(cudatest ${CURRENT_HEADERS} ${CURRENT_SOURCES})

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} ${CERES_LIBRARIES})
```

## 三、可能遇到的Bug

[解决cannot find -lopencv_dep_cudart问题](http://blog.csdn.net/dumpdoctorwang/article/details/78627176)




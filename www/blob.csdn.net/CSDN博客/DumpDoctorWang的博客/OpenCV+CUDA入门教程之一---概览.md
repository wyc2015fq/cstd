# OpenCV+CUDA入门教程之一---概览 - DumpDoctorWang的博客 - CSDN博客





置顶2018年07月18日 20:58:58[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1251








# 一、简介

OpenCV包含GPU(CUDA)模块，包含了所有GPU加速的功能。 在NVIDIA的支持下，该模块的开发工作始于2010年。它包含OpenCV库中一些的重要功能的使用GPU加速的实现，并且仍在不断发展，正在适应新的计算技术和GPU架构。

OpenCV GPU模块是使用CUDA编写的，因此它受益于CUDA生态系统。 CUDA有大型社区，会议，出版物，许多工具和库，如NVIDIA NPP，CUFFT，Thrust。

GPU模块设计为主机API扩展。 此设计为用户提供了对CPU和GPU内存之间数据移动方式的明确控制。虽然用户必须编写一些额外的代码才能开始使用GPU，但这种方法既灵活又可以实现更高效的计算。

GpuMat是GPU内存中保存的数据的主要容器。它的界面与cv::Mat非常相似，它与CPU的Mat相对应。所有GPU函数都接收GpuMat作为输入和输出参数。这允许在不下载数据的情况下调用多个GPU算法。GPU模块API接口也尽可能与CPU接口保持类似。 因此，熟悉CPU上的Opencv的开发人员可以开始直接使用GPU。

# 二、系列教程目录
- [OpenCV+CUDA入门教程之一---概览](https://blog.csdn.net/DumpDoctorWang/article/details/81032914)
- [OpenCV+CUDA入门教程之二---GPU模块简介](https://blog.csdn.net/DumpDoctorWang/article/details/81052597)
- [OpenCV+CUDA入门教程之三---初始化和设备信息](https://blog.csdn.net/DumpDoctorWang/article/details/81055021)
- [OpenCV+CUDA入门教程之四---数据类、数据类相关函数和同步类](https://blog.csdn.net/DumpDoctorWang/article/details/81055695)
- [OpenCV+CUDA入门教程之五---GpuMat详解](https://blog.csdn.net/DumpDoctorWang/article/details/81078008)
- [OpenCV+CUDA入门教程之六---访问GpuMat的每一个元素](https://blog.csdn.net/DumpDoctorWang/article/details/81087048)
- [OpenCV+CUDA入门教程之七---OpenCV的GPU模块的库函数](https://blog.csdn.net/DumpDoctorWang/article/details/81087078)

# 三、一个简单的例子

在下面的示例中，读file.png，然后上传到GPU，阈值化，下载和显示。运行以下代码需要配置带有CUDA模块的OpenCV。

CMakeLists.txt(之后的项目都是这个文件)

```bash
cmake_minimum_required(VERSION 2.8)
project(OCSample)

find_package(CUDA REQUIRED)
set(CUDA_USE_STATIC_CUDA_RUNTIME ON) #这一句解决 cannot find -lopencv_dep_cudart
set(CUDA_NVCC_FLAGS -gencode arch=compute_52,code=sm_52;-G;-g) # nvcc flags
include_directories(STATUS ${CUDA_INCLUDE_DIRS})
message(STATUS "CUDA头文件目录: ${CUDA_INCLUDE_DIRS}")

set(OpenCV_DIR "/usr/local/opencv2413/share/OpenCV") # 指定OpenCV安装路径
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
message(STATUS "OpenCV头文件目录: ${OpenCV_INCLUDE_DIRS}")

file(GLOB CURRENT_HEADERS  *.h) # 添加头文件
file(GLOB CURRENT_SOURCES  *.cu *.cpp) #添加源文件

CUDA_ADD_EXECUTABLE(${PROJECT_NAME} ${CURRENT_HEADERS} ${CURRENT_SOURCES})

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} ${CUDA_LIBRARIES})
```

main.cpp

```cpp
//main.cpp
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>

int main (int argc, char* argv[])
{
    try
    {
        ///　读取图片
        cv::Mat src_host = cv::imread("file.png", CV_LOAD_IMAGE_GRAYSCALE);
        /// 定义GpuMat
        cv::gpu::GpuMat dst, src;
        /// 将主机内存的图像数据上传到GPU内存
        src.upload(src_host);

        /// 调用GPU的阈值函数(很多使用GPU加速的函数都和CPU版本的函数相同)
        cv::gpu::threshold(src, dst, 128.0, 255.0, CV_THRESH_BINARY);

        cv::Mat result_host;
        /// 从GPU上下载阈值化完成的图片
        dst.download(result_host);

        /// 显示
        cv::imshow("Result", result_host);
        cv::waitKey();
    }
    catch(const cv::Exception& ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    return 0;
}
```

## 四、参考
- [GPU Module Documentation](http://docs.opencv.org/2.4/modules/gpu/doc/gpu.html)
- [Realtime Computer Vision with OpenCV](http://queue.acm.org/detail.cfm?id=2206309)






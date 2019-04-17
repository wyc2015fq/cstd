# Clion 无法识别*.cu文件的问题 - DumpDoctorWang的博客 - CSDN博客





2018年09月13日 18:14:30[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：499








# 一、在FileTypes的C++里面添加*.cuh和*.cu

在菜单栏中找到“File”，依次点击“Settings”-->"Editor"-->"File Types"-->"C++"

![](https://img-blog.csdn.net/20180913175629194?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 点击黄色框内的的“+”，输入“*.cu”，点“OK”；然后再点击黄色框内的的“+”，输入“*.cuh”，点“OK”。这一步的目的是让CLion把*.cu和*.cuh文件识别为C++源文件。

# 二、在项目中添加空文件

CMakeLists.txt

```
cmake_minimum_required(VERSION 3.10)

project(CUDA_SAMPLE)

# 设置CUDA版本
set(CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda-9.0)

# 找CUDA
find_package(CUDA REQUIRED)

# 设置公共源文件
file(GLOB COMMON common.h common.cpp)

# 添加*.cu源文件
file(GLOB CU main.cu)

# 添加包含目录
include_directories(${CUDA_INCLUDE_DIRS})

# 添加可执行文件
CUDA_ADD_EXECUTABLE(First-Demo # target名
        ${CU}  # 添加*.cu源文件
        ${COMMON}  # 添加
        )
```

设置公共源文件的目的是为了在main.cu里面点击cuda.h等头文件和cuda函数时有反应，common.h和common.cpp可以为空文件。




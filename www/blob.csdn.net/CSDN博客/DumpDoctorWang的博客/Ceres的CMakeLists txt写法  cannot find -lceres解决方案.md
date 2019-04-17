# Ceres的CMakeLists.txt写法 \ cannot find -lceres解决方案 - DumpDoctorWang的博客 - CSDN博客





2018年11月28日 17:11:41[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：158











### 目录
- [1、编译安装Ceres](#1Ceres_1)
- [2、CMakeLists.txt写法](#2CMakeListstxt_23)




# 1、编译安装Ceres

下载最新的源码：[http://ceres-solver.org/ceres-solver-1.14.0.tar.gz](http://ceres-solver.org/ceres-solver-1.14.0.tar.gz)

安装依赖：

```bash
sudo apt-get install cmake libgoogle-glog-dev libatlas-base-dev libeigen3-dev libsuitesparse-dev libtbb-dev
```

解压ceres-solver-1.14.0.tar.gz，进入解压后的文件夹，打开终端输入命令：

```bash
mkdir release && cd release
# 执行cmake命令
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local/ceres ..
# 编译
make -j 6
# 安装
sudo make install
```

CMAKE_INSTALL_PREFIX指定了ceres的安装路径，6是CPU核心数。

参考网址：[http://www.ceres-solver.org/installation.html#linux](http://www.ceres-solver.org/installation.html#linux)

# 2、CMakeLists.txt写法

在这里真的弄了好久，直接给出模板吧。

```
cmake_minimum_required(VERSION 3.12)
project(Ceres-test)

# 定义自己编译的Ceres的库目录
set(Ceres_LIBS_DIR /usr/local/ceres/lib)

# 定义CeresConfig.cmake所在文件目录
set(Ceres_DIR ${Ceres_LIBS_DIR}/cmake/Ceres)

# 找Ceres库
find_package(Ceres REQUIRED)
message(STATUS "Found Ceres: ${CERES_FOUND}")
message(STATUS "    Ceres Include Dirs: ${CERES_INCLUDE_DIRS}")
message(STATUS "    Ceres Libs: ${CERES_LIBRARIES}")

# 添加包含目录
include_directories(${CERES_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp)

# 定义需要的库
# ${CERES_LIBRARIES}只有一项ceres，后面很多库都没有。。。所以需要自己定义需要链接那些库。
set(LIBS ${Ceres_LIBS_DIR}/libceres.a umfpack cxsparse glog gflags gomp
        ccolamd btf klu cholmod lapack blas camd amd pthread)

target_link_libraries(${PROJECT_NAME} ${LIBS})
```




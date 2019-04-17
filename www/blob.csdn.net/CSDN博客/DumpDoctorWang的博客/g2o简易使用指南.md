# g2o简易使用指南 - DumpDoctorWang的博客 - CSDN博客





2018年01月31日 12:10:52[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：166








# 一、编译

1、去github官网下载g2o,网址[g2o](https://github.com/RainerKuemmerle/g2o/)

2、先安装依赖项

```bash
sudo apt-get install cmake libeigen3-dev libsuitesparse-dev qtdeclarative5-dev qt5-qmake libqglviewer-dev
```

3、编译安装lapack

我安装的的是clapack，是lapack的C语言版本。怎么安装有很多博客已经写好了。

3、解压第一步下载的文件

```bash
mkdir build
cd build
cmake -D BUILD_CSPARSE=TRUE -D LAPACK_INCLUDE_DIR=/usr/local/include -D LAPACK_LIBRARIES_DIR=/usr/local/lib ..
make
sudo make install #安装
sudo ldconfig     #更新
```

其中，LAPACK_INCLUDE_DIR是第二步中lapack头文件的目录；LAPACK_LIBRARIES_DIR是第二步中lapack的库目录。

# 二、使用

1、参见博客： [G2O非线性优化](http://blog.csdn.net/stihy/article/details/55254756) 、 [g2o小记](http://blog.csdn.net/u010566411/article/details/72904266)

2、附上一个自己的CMakeLists.txt

首先将g2o项目中的cmake_module复制到你的项目中

```
cmake_minimum_required(VERSION 2.8)
project(testProjest)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_CXX_FLAGS "-std=c++11")

list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )

ffind_package(Cholmod)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../run)	#这一句指定可执行文件的位置

include_directories(${CHOLMOD_INCLUDE_DIR})

file(GLOB CURRENT_HEADERS  include/*.* include/lib/*.h)
file(GLOB CURRENT_SOURCES  source/*.*)
source_group("Include" FILES ${CURRENT_HEADERS})
source_group("Source" FILES ${CURRENT_SOURCES})

set(G2O_LIBS g2o_cli g2o_ext_freeglut_minimal g2o_simulator g2o_solver_slam2d_linear g2o_types_icp g2o_types_slam2d g2o_core g2o_interface g2o_solver_csparse g2o_solver_structure_only g2o_types_sba g2o_types_slam3d g2o_csparse_extension g2o_opengl_helper g2o_solver_dense g2o_stuff g2o_types_sclam2d g2o_parser g2o_solver_pcg g2o_types_data g2o_types_sim3 cxsparse )

add_executable(${PROJECT_NAME} ${CURRENT_HEADERS} ${CURRENT_SOURCES})

target_link_libraries(${PROJECT_NAME} ${G2O_LIBS} ${CHOLMOD_LIBRARIES})
```






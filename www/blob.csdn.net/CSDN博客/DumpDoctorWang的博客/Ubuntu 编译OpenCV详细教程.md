# Ubuntu 编译OpenCV详细教程 - DumpDoctorWang的博客 - CSDN博客





2018年09月02日 17:23:24[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：288








编译OpenCV差不多有50次了，我把我的经验写下来，给大家一个参考。安装CUDA可参考我另一篇博客：[Ubuntu 安装CUDA](https://blog.csdn.net/DumpDoctorWang/article/details/82317235)。

# 一、下载地址

OpenCV：[https://github.com/opencv/opencv/releases](https://github.com/opencv/opencv/releases)

OpenCV contrib: [https://github.com/opencv/opencv_contrib/releases](https://github.com/opencv/opencv_contrib/releases)
- OpenCV 3.2只支持CUDA 8.0，OpenCV 3.4支持CUDA 9.0、CUDA 8.0。Ubuntu 16.04自带的显卡驱动支持CUDA 8.0；Ubuntu 18.04自带的显卡驱动支持CUDA 9.0。
- opencv_contrib的版本应和OpenCV的版本号相同。
- 下载的时候下载“Source Code”，也就是源代码。

# 二、安装依赖

Ubuntu 16.04

```bash
sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev libgtk-3-dev libgtkglext1-dev libgstreamer1.0-dev libavresample-dev  libgphoto2-dev libopenblas-dev libatlas-dev doxygen libgstreamer-ocaml-dev libgstreamer-plugins-bad1.0-dev libgstreamer-plugins-base0.10-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev libgstreamer0.10-dev libgstreamer1.0-dev libgstreamermm-1.0-dev tesseract-ocr-dev liblept5 libleptonica-dev liblapacke-dev python3-numpy libv4l-dev python3-numpy python3-pip python-pip libprotobuf-dev libhdf5-dev libgflags-dev
```

Ubuntu 18.04

```bash
sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev libgtk-3-dev libgtkglext1-dev libgstreamer1.0-dev libavresample-dev  libgphoto2-dev libopenblas-dev libatlas-base-dev doxygen libgstreamer-ocaml-dev libgstreamer-plugins-bad1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev libgstreamer1.0-dev libgstreamermm-1.0-dev libtesseract-dev liblept5 libleptonica-dev liblapacke-dev libgstreamer-opencv1.0-0 libgstreamer-plugins-bad1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev libgstreamerd-3-dev libgstreamermm-1.0-dev libv4l-dev libprotobuf-dev libhdf5-dev libgflags-dev python3-numpy
```

# 二、编译

以编译OpenCV 3.4.2为例
- 解压“opencv-3.4.2.zip ”
- 进入文件夹“opencv-3.4.2”
- 把”opencv_contrib-3.4.2.zip“解压到文件夹“opencv-3.4.2”
- 在目录“opencv-3.4.2”右键单击打开终端，输入

```bash
mkdir release #新建文件夹
cd release #进入文件夹
```

不需要CUDA支持

```bash
cmake -D CMAKE_BUILD_TYPE=RELEASE \
	-D CMAKE_INSTALL_PREFIX=/usr/local/opencv342 \  
	-D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib-3.4.2/modules \  
	-D WITH_LIBV4L=ON \
 	-D WITH_CUDA=OFF \
	-D WITH_TBB=ON \
	-D WITH_OPENMP=ON \
	-D WITH_OPENGL=ON ..
```

需要CUDA支持

```cpp
cmake -D CMAKE_BUILD_TYPE=RELEASE \
	-D CMAKE_INSTALL_PREFIX=/usr/local/opencv342 \
	-D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib-3.4.2/modules \
	-D WITH_LIBV4L=ON \
	-D WITH_CUDA=ON \
	-D CUDA_ARCH_BIN="3.0 5.0 5.2" \
	-D CUDA_ARCH_PTX="3.0 5.0 5.2" \
	-D ENABLE_FAST_MATH=ON \
	-D CUDA_FAST_MATH=ON \
	-D WITH_CUBLAS=ON \
	-D WITH_NVCUVID=ON \
	-D CUDA_GENERATION=Auto \
	-D WITH_TBB=ON \
	-D WITH_OPENMP=ON \
	-D WITH_OPENGL=ON ..
```

CMAKE_BUILD_TYPE指定编译类型，可选项为DEBUG和RELEASE；CMAKE_INSTALL_PREFIX指定库的安装路径；OPENCV_EXTRA_MODULES_PATH指定opencv_contrib里面的modules文件夹的路径；WITH_CUDA是是否启用CUDA支持，可选项为ON和OFF。

执行上面的命令的时候，可能需要等待一段时间，因为还要下载一些东西。
- cmake没有错误时
`make -j 12 # 12要替换成你的CPU的核心数量`- 安装

```bash
sudo make install
```

# 三、使用

新建目录Hello-OpenCV，并进入该目录
- 编写项目的CMakeLists.txt

```
cmake_minimum_required(VERSION 3.5)

# 设置项目名称
set(PROJECT_NAME Hello-OpenCV)

# 定义项目
project(${PROJECT_NAME})

# 设置编译器，本行可没有
set(CMAKE_CXX_COMPILER g++)
# 设置C++标准
set(CMAKE_CXX_STANDARD 11)

# 指定"OpenCVConfig.cmake"所在路径，下一行命令就可以找到指定的OpenCV版本
set(OpenCV_DIR "/usr/local/opencv342/share/OpenCV")
# 找到上一行指定的那个OpenCV
find_package(OpenCV 3 REQUIRED)

# 包含OpenCV头文件目录
include_directories(${OpenCV_INCLUDE_DIRS})

# 指定源文件
file(GLOB INC *.h)
file(GLOB SRC main.cpp *.cpp)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${INC} ${SRC})

# 链接库
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
```
- main.cpp

```cpp
#include <opencv2/opencv.hpp>

int main(int argc,char **argv){
    cv::Mat src(300,500,CV_8UC3,cv::Scalar::all(255));
    cv::imshow("hello-opencv",src);
    cv::waitKey(0);
}
```

然后打开终端，输入

```
cmake .
make
```

本目录下就会生成一个可执行文件“Hello-OpenCV”，双击之，就出现一个纯白的界面，说明你的OpenCV编译成功了。






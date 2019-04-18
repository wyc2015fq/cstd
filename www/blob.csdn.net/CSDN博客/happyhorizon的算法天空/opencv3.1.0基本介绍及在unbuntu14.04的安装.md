# opencv3.1.0基本介绍及在unbuntu14.04的安装 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:10:03[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：644

# opencv简介
Opencv是著名的计算机视觉开源库，由以下几大模块组成：
1、核心库 core. Core functionality
2、图像处理 imgproc. Image processing
3、图像数据读写 imgcodecs. Image file reading and writing
4、视频媒体读写 videoio. Media I/O
5、图形用户界面 highgui. High-level GUI
6、视频分析 video. Video Analysis
7、多摄像头及3D重建calib3d. Camera Calibration and 3D Reconstruction
8、2D特征框架 features2d. 2D Features Framework
9、物体检测 objdetect. Object Detection
10、机器学习 ml. Machine Learning
11、多维空间聚类搜索 flann. Clustering and Search in Multi-Dimensional Spaces
12、计算摄影技术 photo. Computational Photography（利用编码孔径和波前编码等技术进行成像，借助于计算机对编码的过程进行解码，还原成原先图像的信息，例如获取场景中的深度信息、全聚焦成像、消除运动模糊等。）
13、图像拼接 stitching. Images stitching
14、cuda矩阵计算 cudaarithm. Operations on Matrices
15、cuda背景分割 cudabgsegm. Background Segmentation
16、cuda视频编码解码 cudacodec. Video Encoding/Decoding
17、cuda特征探测和描述 cudafeatures2d. Feature Detection and Description
18、cuda图像滤波 cudafilters. Image Filtering
19、cuda图像处理 cudaimgproc. Image Processing
20、cuda固件支持 cudalegacy. Legacy support
21、cuda物体检测 cudaobjdetect. Object Detection
22、cuda光流 cudaoptflow. Optical Flow
23、cuda双目立体匹配 cudastereo. Stereo Correspondence
24、cuda图像变形 cudawarping. Image Warping
25、cuda设备层 cudev. Device layer
26、形状距离与匹配 shape. Shape Distance and Matching
27、超分辨率 superres. Super Resolution
28、视频防抖 videostab. Video Stabilization
29、3D可视化viz. 3D Visualizer
另外还有一些附加库。
# opencv安装
以ubuntu14.04为例，安装opencv的步骤如下：
## 1）预安装：
Required Packages需要预装的软件：
- GCC 4.4.x or later
- CMake 2.6 or higher
- Git
- GTK+2.x or higher, including headers (libgtk2.0-dev)
- pkg-config
- Python 2.6 or later and Numpy 1.5 or later with developer packages (python-dev, python-numpy)
- ffmpeg or libav development packages: libavcodec-dev, libavformat-dev, libswscale-dev
- [optional] libtbb2 libtbb-dev
- [optional] libdc1394 2.x
- [optional] libjpeg-dev, libpng-dev, libtiff-dev, libjasper-dev, libdc1394-22-dev
## 2） 下载源代码：
可以在SourceForge上下载opencv的源代码，也可以在Git上下载最新的版本（可能不太稳定）
- 下载稳定版本：
前往opencv在SourceForge官网下载稳定版本。
[http://opencv.org/downloads.html](http://opencv.org/downloads.html) 或[http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/3.1.0/opencv-3.1.0.zip](http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/3.1.0/opencv-3.1.0.zip)）
- 下载最新的版本
cd ~/<my_working _directory>
git clone [https://github.com/Itseez/opencv](https://github.com/Itseez/opencv)
## 3）编译安装
用cmake编译opencv源文件。首先解压.zip格式的安装文件后，进入，创建release目录。之后进入release文件夹，并输入编译命令，命令格式如下：
cmake [<some optional parameters>] <path to the OpenCV source directory>
```
cd ~/opencv
mkdir release
cd release
cmake -D CMAKE_BUILD_TYPE=RELEASE -D BUILD_TIFF=ON -D CMAKE_INSTALL_PREFIX=/usr/local ..
```
这里如果不加上`-D BUILD_TIFF=ON`，执行后面的make命令时会报错：
`libopencv_highgui.so:undefined reference to TIFFIsTiled@LIBTIFF_4.0'等`
这是因为OpenCV需要libtiff4库，然而Ubuntu14.04系统安装不会自带libtiff4, 虽然之前安装了libtiff4-dev库，但是还是需要明确指明编译时需要libtiff4库。
编译完成后，输入make生成安装文件：
```
make
sudo make install
```
安装完成。下面，找个例子试一下。采用opencv自带的sample：
```
$ cd ~/Opencv_Source/opencv-3.1.0/samples/cpp/example_cmake
$ make
```
生成里面就是刚刚make的文件，运行后窗口中打开摄像头，并有hello，opencv！的字样
在opencv3.0以后，将SIFT等特征提取算子都放到了contrib模块中，不随着稳定版本一起发布，需要另外下载：
网上多个博客都提供了contrib模块的下载链接，但是个别版本不稳定，安装不成功。安装成功的稳定版本的链接为：([https://codeload.github.com/Wangyida/opencv_contrib/zip/master](https://codeload.github.com/Wangyida/opencv_contrib/zip/master))
因此，如果需要contrib模块，那么必须重新编译opencv库。在终端进入release文件夹重新编译
```
cmake -D CMAKE_BUILD_TYPE=RELEASE -D BUILD_TIFF=ON -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules -D CMAKE_INSTALL_PREFIX=/usr/local ..
```
# 安装和运行例子时出现的错误及解决方案：
## cmake ..时提示无法找到ippicv错误：
下载ippicv库
[https://raw.githubusercontent.com/Itseez/opencv_3rdparty/81a676001ca8075ada498583e4166079e5744668/ippicv/ippicv_linux_20151201.tgz](https://raw.githubusercontent.com/Itseez/opencv_3rdparty/81a676001ca8075ada498583e4166079e5744668/ippicv/ippicv_linux_20151201.tgz)
将下载到的.tgz文件保存在opencv/cd 3rdparty/ippicv/downloads/linux-808b791a6eac9ed78d32a7666804320e/
Linux-808b791a6eac9ed78d32a7666804320e文件夹最初没有，release文件夹下在终端中输入：
cmake ..
之后查看opencv/cd 3rdparty/ippicv/downloads，其中就有了linux-808b791a6eac9ed78d32a7666804320e这个文件夹。将下载到的ippicv库文件拷贝到该地址，在build路径下，重新cmake编译源码。
## 编译例子时，输入make后返回： cannot find -lippicv错误
首先进入目录：
opencv-3.1.0/3rdparty/ippicv/unpack/ippicv_lnx/lib/intel64
将库文件 libippicv.a文件cp到/usr/local/lib下面。增加可执行权限：
```
cp libippicv.a /usr/local/lib
cd /usr/local/lib
sudo chmod +x libippicv.a
```
# 无法下载ippicv_linux_20151201.tgz
可以提前下载好ippicv_linux_20151201.tgz文件, 如果运行报错,根据报错提示,将tgz文件拷贝到相应的文件夹,路径通常是opencv-3.1.0/3rdparty/ippicv/downloads/linux-808b791a6eac9ed78d32a7666804320e
# 参考：
Opencv帮助文档：
[http://docs.opencv.org/3.1.0/doc/tutorials/tutorials.html](http://docs.opencv.org/3.1.0/doc/tutorials/tutorials.html)
安装教程：
[http://m.blog.csdn.net/qccz123456/article/details/52351831](http://m.blog.csdn.net/qccz123456/article/details/52351831)
[http://www.cnblogs.com/asmer-stone/p/5089764.html](http://www.cnblogs.com/asmer-stone/p/5089764.html)
[图像处理之image stitching][http://www.cnblogs.com/naive/p/3579610.html?utm_source=tuicool&utm_medium=referral](http://www.cnblogs.com/naive/p/3579610.html?utm_source=tuicool&utm_medium=referral)

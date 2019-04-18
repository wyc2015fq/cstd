# Ubuntu环境下OpenCV 3.2版本中CMake error the source directory does not exist - YZXnuaa的博客 - CSDN博客
2018年01月23日 11:39:11[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1993
最近想编译安装一下OpenCV，发现用了CMake问题很多，网上对这一块都没有具体介绍，后来才发现是一个简单的白痴问题，因此将问题在此记录一下。
关于OpenCV的安装文档特别多。
无非都是以下几步：
(1)  下载opencv, opencv-contrib源码。
(2) 提前安装一些库：主要有：
- sudo apt-get install build-essential cmake pkg-config
- sudo apt-get install libjpeg8-dev libtiff5-dev libjasper-dev libpng12-dev
- sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
- sudo apt-get install libxvidcore-dev libx264-dev
- sudo apt-get install libgtk-3-dev
- sudo apt-get install libatlas-base-dev gfortran
(3) 开始进行cmake
这个时候，坑就来了，网上大多采用的是：
cmake -D CMAKE_BUILD_TYPE=RELEASE \
　　-D CMAKE_INSTALL_PREFIX=/usr/local \
　　-D INSTALL_PYTHON_EXAMPLES=ON \
　　-D INSTALL_C_EXAMPLES=OFF \
　　-D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.2.0/modules \
　　-D PYTHON_EXECUTABLE=/usr/lib/python3 \
　　-D BUILD_EXAMPLES=ON ..
但是这个时候，总是运行不过去，马上就碰到了个文件路径没有找到的问题。
CMake
 Error: The source directory "/home/michael/opencv-3.1.0/release/CMAKE_INSTALL_PREFIX=/home/michael/opencv-3.1.0" does not exist.
碰到这个问题基本上无解，找了好多地方都没有说这个问题。
而碰巧的是，最后深度一下，把-D后面的空格去掉果然可以。
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/usr/local.. -DINSTALL_PYTHON_EXAMPLES=ON -DINSTALL_C_EXAMPLES=OFF -DOPENCV_EXTRA_MODULES_PATH=~/soft/opencv_contrib-master/modules -DPYTHON_EXECUTABLE=/usr/local/bin/python/python3.6 \
    -DBUILD_EXAMPLES=OFF
            

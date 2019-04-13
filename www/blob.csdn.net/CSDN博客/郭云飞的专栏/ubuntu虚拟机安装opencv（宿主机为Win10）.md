
# ubuntu虚拟机安装opencv（宿主机为Win10） - 郭云飞的专栏 - CSDN博客


2017年07月21日 15:44:05[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：263


1. opencv官网（opencv.org）上找到最新版本（此处下载3.2.0版），下载opencv源码（Source code(zip)或者Source code(tar.gz)）:

```python
https://github.com/opencv/opencv/releases/tag/3.2.0
```
2. 将源码压缩文件解压到ubuntu目录，例如/home/yourname/Downloads/注意：如果是虚拟机，不能放到共享文件夹中进行编译安装，在make时会失败，错误类似下边这样：

```python
CMake Error: cmake_symlink_library: System Error: Operation not supported
CMake Error: cmake_symlink_library: System Error: Operation not supported
modules/core/CMakeFiles/opencv_core.dir/build.make:1424: recipe for target 'lib/libopencv_core.so.3.2.0' failed
make[2]: *** [lib/libopencv_core.so.3.2.0] Error 1
make[2]: *** Deleting file 'lib/libopencv_core.so.3.2.0'
CMakeFiles/Makefile2:1376: recipe for target 'modules/core/CMakeFiles/opencv_core.dir/all' failed
make[1]: *** [modules/core/CMakeFiles/opencv_core.dir/all] Error 2
Makefile:162: recipe for target 'all' failed
make: *** [all] Error 2
root@ubuntu:/mnt/hgfs/ubuntu-share/1-software/opencv-3.2.0/release#
```
3. 不要将编译和安装的文件生成在源码目录下，一般会新建一个文件夹放置生成的各种文件，例如：
```python
guo@ubuntu:~/Downloads/opencv-3.2.0$ ls
opencv-3.2.0  opencv-3.2.0.zip  release
```
opencv-3.2.0是解压opencv-3.2.0.zip后的源码文件夹，新建了一个名为release的文件夹用于存放生成的各种文件4. 安装前的准备工作（系统更新、安装依赖包等）：

```python
// 系统升级
sudo apt-get update
sudo apt-get upgrade
```
```python
// OpenCV 2.2以后版本需要使用Cmake生成makefile文件，因此需要先安装cmake。ubuntu下安装cmake比较简单
apt-get install cmake
```
如果觉得ubuntu系统自带的cmake版本不符合要求，可以下载安装包。下载最新版的安装包：[http://www.cmake.org/cmake/resources/software.html](http://www.cmake.org/cmake/resources/software.html)，但需要自己编译和安装，此处不累述。

```python
// 安装依赖包
sudo apt-get install build-essential 
sudo apt-get install  libgtk2.0-dev libavcodec-dev libavformat-dev  libtiff4-dev  libswscale-dev libjasper-dev
```
5. 利用cmake检测编译环境，并生成makefile文件
```python
cd release  
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ./opencv-3.2.0
```
注意最后的参数“./opencv-3.2.0”为源码文件夹的位置，里边包含了CMakeList.txt
6. 编译（比较慢，几分钟）

```python
make
```
7. 安装
```python
make install
```











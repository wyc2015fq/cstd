# Caffe+Kubuntu16.04_X64+CUDA 8.0配置 - wishchinYang的专栏 - CSDN博客
2016年08月26日 12:51:09[wishchin](https://me.csdn.net/wishchin)阅读数：5647
前言：
        经过尝试过几次Caffe，theano，MxNet之后，很长时间没有进行caffe的更新，此次在Ubuntu16.04下安装Caffe，折腾了一天时间，终于安装成功。
        参考链接：[Caffe+UbuntuKylin14.04_X64+CUDA 6.5配置](http://blog.csdn.net/wishchin/article/details/42706971)
**一  配置CUDA8.0**
        既然使用了最新版本，必然使用最新版本的CUDA,且只有CUDA8.0给了ubuntu16.04支持。
**1.1 禁用nouveau驱动：**
        参考链接： [http://blog.csdn.net/chunchun362425965/article/details/51566851](http://blog.csdn.net/chunchun362425965/article/details/51566851)
##### nouveau去驱动是ubuntu自带的驱动，需要将其删除掉才能安装 NVIDIA的驱动
首先新建一个/etc/modprobe.d/blacklist-nouveau.conf文件
 输入如下文字blacklist nouveau
options nouveau modeset=0
运行sudo update-initramfs -u
如果遇到不能启动的情况，或者遇到warning 显示显示配置选项，重启几次，直到启动lighedm界面。
         异常情况：此步骤不成功会造成  nouveau驱动未禁用，NVIDIA显卡驱动无法安装,
**1.2 退回ubuntu GCC编译器到4.9：**
         参考链接：[深度学习（四十一）cuda8.0+ubuntu16.04+theano、caffe、tensorflow环境搭建](http://blog.csdn.net/hjimce/article/details/51999566)
ubuntu的gcc编译器是5.4.0，然而cuda8.0不支持5.0以上的编译器，因此需要降级，把编译器版本降到4.9：
```python
sudo apt-get install g++-4.9  
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 20  
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 10  
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 20  
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 10  
    sudo update-alternatives --install /usr/bin/cc cc /usr/bin/gcc 30  
    sudo update-alternatives --set cc /usr/bin/gcc  
    sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++ 30  
    sudo update-alternatives --set c++ /usr/bin/g++
```
等待安装完成...............................................................
         异常情况：此步骤不成功会造成  在安装CUDA时找不到编译器，CUDA安装失败。 
**1.3  安装CUDA8.0：**
         参考链接：[深度学习（四十一）cuda8.0+ubuntu16.04+theano、caffe、tensorflow环境搭建](http://blog.csdn.net/hjimce/article/details/51999566)
       CUDA8.0属于测试版本，需要登录之后才能下载，最简单的方式时下载.run包，可以直接运行。
       官网下载地址：[https://developer.nvidia.com/cuda-toolkit](https://developer.nvidia.com/cuda-toolkit)
准备工作：
       在UEFI BIOS里面切换显卡使用，修改 PCI1 为ITG，切换到intel核显
安装过程：
       su root  
       输入密码：
       service lightdm stop  （关掉X界面服务 ）
       cd  /home/cuda/
       ./cuda.run
等待安装成功...............
修改BIOS，切换会PCI1
注意事项：cuda默认携带了nvidia驱动，可以不预先安装最新版本的驱动程序。
**1.4 配置测试环境**
**         参考链接： **[在Ubuntu下安装、配置和测试cuda](http://blog.csdn.net/menglongbor/article/details/7015380)
```python
export CUDA_HOME=/usr/local/cuda-8.0/ 
      
    export CUDA_INC_PATH=${CUDA_HOME}/include  
    export CUDA_LIB_PATH=${CUDA_HOME}/lib64  
      
    export CUDA_INSTALL_PATH=${CUDA_HOME}  
      
    export PATH=${CUDA_HOME}/bin:$PATH  
    export PATH=${CUDA_HOME}/computeprof/bin:$PATH  
      
    export LD_LIBRARY_PATH=${CUDA_HOME}/computeprof/bin:$LD_LIBRARY_PATH  
    export LD_LIBRARY_PATH=${CUDA_HOME}/lib64:$LD_LIBRARY_PATH  
    export LD_LIBRARY_PATH=${CUDA_HOME}/extras/CUPTI/lib64:$LD_LIBRARY_PATH  
    export MANPATH=${CUDA_HOME}/man:$MANPATH  
      
    export OPENCL_HOME=${CUDA_HOME}  
    export OPENCL_INC_PATH=${OPENCL_HOME}/include  
    export OPENCL_LIB_PATH=${OPENCL_HOME}/lib64  
    export LD_LIBRARY_PATH=${OPENCL_LIB_PATH}:$LD_LIBRARY_PATH
```
查看版本：       
```python
nvcc --version
```
显示信息：
B85M-D2V-TM:~$ nvcc --version
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2016 NVIDIA Corporation
Built on Wed_May__4_21:01:56_CDT_2016
Cuda compilation tools, release 8.0, V8.0.26
**二 安装第三方库**
## 1. 安装Intel MKL 
（如果没有可以安装OpenBLAS代替）解压安装包，下面有一个install_GUI.sh文件， 执行该文件，会出现图形安装界面，根据说明一步一步执行即可。
**1.安装 MKL **
注意： 安装完成后需要添加library路径
```python
sudo gedit /etc/ld.so.conf.d/intel_mkl.conf
```
在文件中添加内容
```python
/opt/intel/lib  
    /opt/intel/mkl/lib/intel64
```
**2.或者 安装 OpenBLAS**
```
```python
Install ATLAS/MKL/OpenBLAS if you haven't already.
```
```python
$ sudo apt-get install libopenblas-dev  
or  
$ sudo apt-get install libopenblas-base
```
（这里我按照这样的方法最终编译**出现cblas找不到的问题**，应该是MKL安装有问题，但是又没办法解决，最终我就按照官网的方法安装了ATLAS：
**sudo apt-get install libatlas-base-dev**
一句话就搞定，虽然性能可能比不上MKL，但是将就着能用就行。）
```
注意把路径替换成自己的安装路径。 编辑完后执行
```
sudo ldconfig
```
## 3. 安装OpenCV
这个尽量不要手动安装， Github上有人已经写好了完整的安装脚本：
```python
https://github.com/jayrambhia/Install-OpenCV
```
下载该脚本，进入Ubuntu/2.4 目录, 给所有shell脚本加上可执行权限
```python
chmod +x *.sh
```
然后安装最新版本 （当前为2.4.9）
（**[http://code.opencv.org/issues/3814](http://code.opencv.org/issues/3814)  下载 [NCVPixelOperations.hpp](http://code.opencv.org/projects/opencv/repository/revisions/feb74b125d7923c0bc11054b66863e1e9f753141/raw/modules/gpu/src/nvidia/core/NCVPixelOperations.hpp) 替换掉opencv2.4.9内的文件， 重新build。**）
```python
sudo ./opencv2_4_9.sh
```
（注意，修改一下 文件里面的编译选项，一直开 make j4, 导致CPU 总是热导致停机，半途而废。
**应修改为 j2 ，或者去掉j选项**）
脚本会自动安装依赖项，下载安装包，编译并安装OpenCV。
整个过程大概半小时左右。 
注意，中途可能会报错（**这一次真的报错了！！！**）
```
opencv-2.4.9/modules/gpu/src/nvidia/core/NCVPixelOperations.hpp(51): error: a storage class is not allowed in an explicit specialization
```
解决方法在此：**[http://code.opencv.org/issues/3814](http://code.opencv.org/issues/3814)  下载 [NCVPixelOperations.hpp](http://code.opencv.org/projects/opencv/repository/revisions/feb74b125d7923c0bc11054b66863e1e9f753141/raw/modules/gpu/src/nvidia/core/NCVPixelOperations.hpp) 替换掉opencv2.4.9内的文件， 重新build。**
## 安装OpenCV
Ubuntu16.04用户
[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2) 16.04中安装OpenCV 2.4.10参考网址如下：
[http://www.linuxidc.com/Linux/2016-07/132879.htm](http://www.linuxidc.com/Linux/2016-07/132879.htm)
[http://www.linuxidc.com/Linux/2016-07/132880.htm](http://www.linuxidc.com/Linux/2016-07/132880.htm)
1 预先安装一些软件：
`sudo apt-get install build-essential cmake libgtk2.0-dev pkg-config python-dev python-numpy libavcodec-dev libavformat-dev libswscale-dev `
2 去官网下载opencv压缩包
3 解压压缩包到某文件夹（用windows习惯了，直接右键提取到此处。。。）
   cd 到目录下，执行（注意有个“.”，作为cmake的参数表示当前目录）
   cmake .
成功，然后
   make
出现错误：
```
[ 16%] Building NVCC (Device) object modules/core/CMakeFiles/cuda_compile.dir/__/dynamicuda/src/cuda/cuda_compile_generated_matrix_operations.cu.o
CMake Warning (dev) at cuda_compile_generated_matrix_operations.cu.o.cmake:137 (if):
  Policy CMP0054 is not set: Only interpret if() arguments as variables or
  keywords when unquoted.  Run "cmake --help-policy CMP0054" for policy
  details.  Use the cmake_policy command to set the policy and suppress this
  warning.
  Quoted keywords like "COMMAND" will no longer be interpreted as keywords
  when the policy is set to NEW.  Since the policy is not set the OLD
  behavior will be used.
Call Stack (most recent call first):
  cuda_compile_generated_matrix_operations.cu.o.cmake:164 (cuda_execute_process)
  
This warning is for project developers.  Use -Wno-dev to suppress it.
nvcc fatal   : Unsupported gpu architecture 'compute_11'
CMake Error at cuda_compile_generated_matrix_operations.cu.o.cmake:206 (message):
  Error generating
  /home/wishchin/Lib/opencv-2.4.10/modules/core/CMakeFiles/cuda_compile.dir/__/dynamicuda/src/cuda/./cuda_compile_generated_matrix_operations.cu.o
modules/core/CMakeFiles/opencv_core.dir/build.make:63: recipe for target 'modules/core/CMakeFiles/cuda_compile.dir/__/dynamicuda/src/cuda/cuda_compile_generated_matrix_operations.cu.o' failed
make[2]: *** [modules/core/CMakeFiles/cuda_compile.dir/__/dynamicuda/src/cuda/cuda_compile_generated_matrix_operations.cu.o] Error 1
CMakeFiles/Makefile2:1110: recipe for target 'modules/core/CMakeFiles/opencv_core.dir/all' failed
make[1]: *** [modules/core/CMakeFiles/opencv_core.dir/all] Error 2
Makefile:149: recipe for target 'all' failed
make: *** [all] Error 2
```
排错方法：
**修改OpenCV版本为2.4.13，编译通过，貌似2.4.10不支持CUDA8.0。**
```
da_compile_generated_row_filter.6.cu.o
[ 67%] Building NVCC (Device) object modules/gpu/CMakeFiles/cuda_compile.dir/src/cuda/cuda_compile_generated_build_point_list.cu.o
[ 67%] Building NVCC (Device) object modules/gpu/CMakeFiles/cuda_compile.dir/src/cuda/cuda_compile_generated_copy_make_border.cu.o
[ 67%] Building NVCC (Device) object modules/gpu/CMakeFiles/cuda_compile.dir/src/cuda/cuda_compile_generated_integral_image.cu.o
[ 67%] Building NVCC (Device) object modules/gpu/CMakeFiles/cuda_compile.dir/src/cuda/cuda_compile_generated_tvl1flow.cu.o
[ 67%] Building NVCC (Device) object modules/gpu/CMakeFiles/cuda_compile.dir/src/cuda/cuda_compile_generated_column_filter.8.cu.o
[ 67%] Building NVCC (Device) object modules/gpu/CMakeFiles/cuda_compile.dir/src/cuda/cuda_compile_generated_column_filter.3.cu.o
```
```
[100%] Building CXX object apps/traincascade/CMakeFiles/opencv_traincascade.dir/HOGfeatures.cpp.o
[100%] Building CXX object apps/traincascade/CMakeFiles/opencv_traincascade.dir/imagestorage.cpp.o
[100%] Linking CXX executable ../../bin/opencv_traincascade
[100%] Built target opencv_traincascade
Scanning dependencies of target opencv_annotation
[100%] Building CXX object apps/annotation/CMakeFiles/opencv_annotation.dir/opencv_annotation.cpp.o
[100%] Linking CXX executable ../../bin/opencv_annotation
[100%] Built target opencv_annotation
Scanning dependencies of target opencv_visualisation
[100%] Building CXX object apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.o
[100%] Linking CXX executable ../../bin/opencv_visualisation
[100%] Built target opencv_visualisation
```
且只有opencv 2.4.13编译成功..........................
4 安装
sudo make install
```
[100%] Built target opencv_traincascade
[100%] Linking CXX executable ../../bin/opencv_annotation
[100%] Built target opencv_annotation
[100%] Linking CXX executable ../../bin/opencv_visualisation
[100%] Built target opencv_visualisation
Install the project...
-- Install configuration: "Release"
-- Installing: /usr/local/include/opencv2/opencv_modules.hpp
-- Installing: /usr/local/lib/pkgconfig/opencv.pc
-- Installing: /usr/local/share/OpenCV/OpenCVConfig.cmake
-- Installing: /usr/local/share/OpenCV/OpenCVConfig-version.cmake
-- Installing: /usr/local/share/OpenCV/OpenCVModules.cmake
-- Installing: /usr/local/share/OpenCV/OpenCVModules-release.cmake
-- Installing: /usr/local/include/opencv/cv.h
-- Installing: /usr/local/include/opencv/cxeigen.hpp
```
安装成功........................................ 
## 4. 安装其他依赖项
`sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libboost-all-dev libhdf5-serial-dev libgflags-dev libgoogle-glog-dev liblmdb-dev protobuf-compiler `
其他版本用户参考官方说明：http://caffe.berkeleyvision.org/installation.html
**三 安装高级语言接口**
## 1. 安装Caffe所需要的Python环境
首先安装pip和python-dev （系统默认有python环境的， 不过我们需要的使python-dev）
```
sudo apt-get install python-dev python-pip
```
然后执行如下命令安装编译caffe python wrapper 所需要的额外包
```
`for req in $(cat requirements.txt); do sudo pip install $req; done`
```
在执行上述命令时， 会报错导致不能完全安装所有需要的包。 可以按照官方建议安装anaconda包。 在anaconda官网下载.sh文件，执行，最后添加bin目录到环境变量即可。
（下面的添加没用！一般不会用到）
建议安装Anaconda包，这个包能独立于系统自带的python库，并且提供大部分Caffe需要的科学运算Python库。这里需要注意，在运行Caffe时，可能会报一些找不到libxxx.so的错误，而用 locate libxxx.so命令发现已经安装在anaconda中，这时首先想到的是在/etc/ld.so.conf.d/ 下面将 $your_anaconda_path/lib 加入 LD_LIBRARY_PATH中。但是这样做可能导致登出后无法再进入桌面！！！原因（猜测）可能是anaconda的lib中有些内容于系统自带的lib产生冲突。
正确的做法是：为了不让系统在启动时就将anaconda/lib加入系统库目录，可以在用户自己的~/.bashrc 中添加library path， 比如我就在最后添加了两行
```
# add library path  
    LD_LIBRARY_PATH=your_anaconda_path/lib:$LD_LIBRARY_PATH  
    export LD_LIBRARY_PATH
```
开启另一个终端后即生效，并且重启后能够顺利加载lightdm, 进入桌面环境。
**三 编译Caffe**
## 1. 编译Caffe
终于完成了所有环境的配置，可以愉快的编译Caffe了！ 进入caffe根目录， 首先复制一份Makefile.config
```
cp  Makefile.config.example  Makefile.config
```
然后修改里面的内容，主要需要修改的参数包括
CPU_ONLY 是否只使用CPU模式，没有GPU没安装CUDA的同学可以打开这个选项
BLAS (使用intel mkl还是OpenBLAS)（**由于我安装了atlas，所以不需要修改默认设置！**）
MATLAB_DIR 如果需要使用MATLAB wrapper的同学需要指定matlab的安装路径, 
如我的路径为 /usr/local/MATLAB/R2013b (注意该目录下需要包含bin文件夹，bin文件夹里应该包含mex二进制程序)
DEBUG 是否使用debug模式，打开此选项则可以在eclipse或者NSight中debug程序
完成设置后， 开始编译
```
make 
    make test  
    make runtest
```
注意 -j4 是指使用几个线程来同时编译， 可以加快速度， j后面的数字可以根据CPU core的个数来决定， 我的CPU使4核， 所以-j4.
错误重现：
make 出现 
```
<pre name="code" class="html"><span style="font-family:Times New Roman;">CXX .build_release/src/caffe/proto/caffe.pb.cc
CXX src/caffe/solver.cpp
In file included from src/caffe/solver.cpp:8:0:
./include/caffe/util/hdf5.hpp:6:18: fatal error: hdf5.h: 没有那个文件或目录
 #include "hdf5.h"
                  ^
compilation terminated.</span>
```
错误原因： HDF5未能使用，
解决方法： **使用 make all -j2 命令，可以CXX编译通过**。
出现错误：
```
CXX examples/cpp_classification/classification.cpp
AR -o .build_release/lib/libcaffe.a
LD -o .build_release/lib/libcaffe.so.1.0.0-rc3
/usr/bin/ld: 找不到 -lhdf5_hl
/usr/bin/ld: 找不到 -lhdf5
/usr/bin/ld: 找不到 -lcblas
/usr/bin/ld: 找不到 -latlas
collect2: error: ld returned 1 exit status
Makefile:563: recipe for target '.build_release/lib/libcaffe.so.1.0.0-rc3' failed
make: *** [.build_release/lib/libcaffe.so.1.0.0-rc3] Error 1
make: *** 正在等待未完成的任务....
```
 解决方法：
1安装atlas...  
**sudo apt-get install libatlas-base-dev**
成功
2添加hdf5到 系统路径
makefile文件里面添加：
LIBRARY_DIRS += /root/anaconda2/lib
或者
修改Makefile.config需要修改的内容：
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu/hdf5/serial
编译通过.......
此处参考：[Ubuntu16.04+CUDA8.0+Caffe](http://blog.csdn.net/autocyz/article/details/52299889)
3. 若出现/bin/sh: 1: /usr/local/cuda/bin/nvcc: not found 问题，在Ubuntu17.04时会出现此问题
   重装CUDA
### 1.2. 编译Python wrapper
```
make pycaffe
```
然后基本就全部安装完拉.
接下来大家尽情地跑demo吧～
**后记**：
  最好不要使用16.04版本的ubuntu，还原到14.04版本为好。

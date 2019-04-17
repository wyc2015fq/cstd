# [转载] 深度学习指南：基于Ubuntu从头开始搭建环境 - xiaxzhou的博客 - CSDN博客





2017年06月13日 14:42:46[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：1111








### 原文：[http://geek.csdn.net/news/detail/77859](http://geek.csdn.net/news/detail/77859)

> 
记录自己安装时的一些问题和解决方法


# 基础

首先，打开终端，运行以下命令确保你的操作系统是最新的

```
sudo apt-get update  
sudo apt-get upgrade  
sudo apt-get install build-essential  
sudo apt-get autoremove
```

# 安装git

```bash
sudo apt-get install git
```

显卡驱动不要安装，cuda安装时会安装驱动，否则冲突

# CUDA
- 从Nvidia上下载CUDA8.0。然后到下载目录下安装CUDA

```
sudo dpkg -i cuda-repo-ubuntu1404*amd64.deb
sudo apt-get update
sudo apt-get install cuda
```
- 添加CUDA到环境变量

```bash
echo 'export PATH=/usr/local/cuda/bin:$PATH' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc
```
- 检查以确保安装了正确版本的CUDA
`nvcc -V`- 重启系统

```bash
sudo shutdown -r now
```

### 检查CUDA安装（可选）
- 在CUDA安装目录安装样例。编译它们（需要几分钟）：

```
/usr/local/cuda8.0/bin/cuda-install-samples-8.0.sh ~/cuda-samples
cd ~/cuda-samples/NVIDIA*Samples
make -j $(($(nproc) + 1))
```

注意：（-j $(($(nproc) + 1))）命令使用你机器上的核心数并行执行，所以编译会更快
- 运行deviceQuery，确保它能检测到显卡并测试通过
`bin/x86_64/linux/release/deviceQuery`
# cuDNN

cuDNN是为DNN设计的CPU加速库。它能在多种情况下帮助提升执行速度。为了下载cuDNN库，你需要到Nvidia网站[https://developer.nvidia.com/cudnn](https://developer.nvidia.com/cudnn)上进行注册。几小时到几个工作日就能够批准。一旦注册批准，下载Linux版本的cuDNN v5，最新版本不是所有的工具都支持。
- 解压并复制文件

```
cd ~/Downloads/
tar xvf cudnn*.tgz
cd cuda
sudo cp */*.h /usr/local/cuda/include/
sudo cp */libcudnn* /usr/local/cuda/lib64/
sudo chmod a+r /usr/local/cuda/lib64/libcudnn*
```
- 检查你可以使用
`nvidia-smi`
命令检查目前所有操作都正确。这应该会输出GPU的一些统计数据

# Tensorflow
- 安装v0.8版本与GPU兼容。
`sudo apt-get install python-pip python-dev`- install TensorFlow by invoking one of the following commands:

```bash
$ sudo pip install tensorflow      # Python 2.7; CPU support (no GPU support)
 $ sudo pip3 install tensorflow     # Python 3.n; CPU support (no GPU support)
 $ sudo pip install tensorflow-gpu  # Python 2.7;  GPU support
 $ sudo pip3 install tensorflow-gpu # Python 3.n; GPU support
```

运行一个测试程序确保Tensorflow成功安装。当你执行import命令的时候，应该不会有警告/错误。

```python
python
>>> import tensorflow as tf
>>> exit()
```

# OpenBLAS

OpenBLAS是一个线性代数库，比Atlas更快。这一步是可选的，但要注意，下面的一些步骤假定你已经安装了OpenBLAS。
- 需要安装gfortran来编译它

```bash
sudo apt-get install gfortran
```
- 命令
`gfortran -v`
可以查看gfortran是否安装成功。
- 安装OpenBLAS

```bash
mkdir ~/git
cd ~/git
git clone https://github.com/xianyi/OpenBLAS.git
cd OpenBLAS
make FC=gfortran -j $(($(nproc) + 1))
sudo make PREFIX=/usr/local install
```
- 将路径添加到LD_LIBRARY_PATH 变量中

```bash
echo 'export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
```

# 常用工具

安装pip

```
sudo apt-get install python-pip
apt-get install python-pip
```

为Scipy安装一些常用工具

```
sudo apt-get install -y libfreetype6-dev libpng12-dev
sudo pip install -U matplotlib ipython[all] jupyter pandas scikit-image
```

# Caffe
- 第一步是安装所必须的文件
`sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler`
结果提示：下载 
[http://cn.archive.ubuntu.com/ubuntu/pool/universe/o/opencv/libopencv-legacy2.4v5_2.4.9.1+dfsg-1.5ubuntu1_amd64.deb](http://cn.archive.ubuntu.com/ubuntu/pool/universe/o/opencv/libopencv-legacy2.4v5_2.4.9.1+dfsg-1.5ubuntu1_amd64.deb)  Hash 校验和不符 失败 

有几个软件包无法下载 

修改命令为：`sudo apt-get --fix-missing  install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler`
安装成功
- 继续安装

```
sudo apt-get install --no-install-recommends libboost-all-dev
sudo apt-get install libgflags-dev libgoogle-glog-dev liblmdb-dev
```
- 克隆Caffe资源库

```
cd ~/git
git clone https://github.com/BVLC/caffe.git
cd caffe
cp Makefile.config.example Makefile.config
```
- 如果你安装了cuDNN，取消Makefile中USE_CUDNN := 1 这一行的注释
`sed -i 's/# USE_CUDNN := 1/USE_CUDNN := 1/' Makefile.config`- 如果你安装了OpenBLAS，修改BLAS参数值为open
`sed -i 's/BLAS := atlas/BLAS := open/' Makefile.config`- 安装需要的文件，构建Caffe和测试，运行测试确保所有测试都通过。注意，这都需要一段时间。

```
sudo pip install -r python/requirements.txt
make all -j $(($(nproc) + 1))
make test -j $(($(nproc) + 1))
make runtest -j $(($(nproc) + 1))
```

出现编译错误，提示：src/caffe/layers/hdf5_data_layer.cpp:13:18: fatal error: hdf5.h: No such file or directory 
**解决方法：**

在Makefile.config中：- 在INCLUDE_DIRS后添加/usr/include/hdf5/serial
- 在LIBRARY_DIRS后添加/usr/lib/x86_64-linux-gnu/hdf5/serial/

最终Makefile.config文件对应部分修改如下

```
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial/
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu/hdf5/serial/
```

重新执行出错的命令

> 
参考：[http://www.linuxdiyf.com/linux/20227.html](http://www.linuxdiyf.com/linux/20227.html)

- 构建PyCaffe，Caffe的Python接口
`make pycaffe -j $(($(nproc) + 1))`
提示：numpy/arrayobject.h: 没有那个文件或目录 

解决：

```
sudo apt-get install python-numpy
make pycaffe -j $(($(nproc) + 1))
```
- 将Caffe添加到环境变量中

```bash
echo 'export CAFFE_ROOT=$(pwd)' >> ~/.bashrc
echo 'export PYTHONPATH=$CAFFE_ROOT/python:$PYTHONPATH' >> ~/.bashrc
source ~/.bashrc
```
- 测试确保Caffe安装成功。当执行import命令的时候应该不会有警告/错误。

```python
python
>>> import caffe
>>> exit()
```

我的环境变量添加出错

```bash
cd ~
gedit .bashrc
```

然后将’  export CAFFE_ROOT=$(pwd)  ‘中的pwd改为caffe根目录:
`export CAFFE_ROOT=/home/xiaxzhou/git/caffe`
# Theano

安装所必须的文件，然后安装Theano。这些指令来自于这里

```
sudo apt-get install python-numpy python-scipy python-dev python-pip python-nose g++ python-pygments python-sphinx python-nose
sudo pip install Theano
```

测试Theano安装。当执行import命令的时候应该不会有警告/错误。

```python
python
>>> import theano
>>> exit()
```

# Keras

Keras是围绕Theano和Tensorflow设计的一个有用的封装。默认情况下，它使用Theano作为后端。查看这里的指令以了解如何变更为Tensorflow。

```bash
sudo pip install keras
```

# Torch

安装会花一些时间

```
git clone https://github.com/torch/distro.git ~/git/torch --recursive
cd torch; bash install-deps;
./install.sh
```

# X2Go

如果你的深度学习机器不是主要工作机器，X2Go可以帮助你远程访问。X2Go是一个了不起的远程访问解决方案。你可以使用下面的命令在Ubuntu机器上安装X2Go服务。

```
sudo apt-get install software-properties-common
sudo add-apt-repository ppa:x2go/stable
sudo apt-get update
sudo apt-get install x2goserver x2goserver-xsession
```

X2Go不支持统一桌面环境（Ubuntu的默认环境）。我发现XFCE效果不错。更多支持的环境在这里

```
sudo apt-get update
sudo apt-get install -y xfce4 xfce4-goodies xubuntu-desktop
```

使用下面的命令查看机器的IP
`hostname -I`
你可以使用上面的IP在你主要使用的机器上安装一个客户端来连接到深度学习服务器上。根据你的客户端系统，这里有更多的指令。

# opencv

> 
参考：[http://www.cnblogs.com/lyutian/p/4425956.html](http://www.cnblogs.com/lyutian/p/4425956.html)

- 
准备环境

　　直接编译安装opencv一般是会报错的，有很多依赖工具需要先安装。主要是：cmake, build-essential, pkg-config, python-dev, python-numpy等，视自己情况而定。　

`　　$ sudo apt-get install build-essential cmake libopencv-dev libgtk2.0-dev pkg-config python-dev python-numpy`- 
下载opencv3.0源码

　　直接去opencv.org（github） 上下载linux源码包：

`　　$ wget https://github.com/Itseez/opencv/archive/3.0.0-beta.zip`- 
解压组织、编译、安装

　　解压文件，新建一个文件夹用于安放编译结果，然后安装（选择安装在/usr/local下面）


复制代码

```
$ unzip opencv-3.0.0-beta.zip
　　$ cd opencv-3.0.0-beta
　　$ mkdir release
　　$ cd release
　　$ cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..        //注意CMakeList.txt在上一层文件夹
　　$ sudo make -j $(nproc)            // make -j 多核处理器进行编译(默认的make只用一核，很慢)，$(nproc)返回自己机器的核数
　　$ sudo make install                 //把编译结果安装到 /usr/local的 lib/ 和 include/下面
```


**中途出错**：home/usrname/opencv-3.0.0/modules/cudalegacy/src/graphcuts.cpp:120:54: error: ‘NppiGraphcutState’ has not been declared typedef NppStatus (*init_func_t)(NppiSize oSize, NppiGraphcutState** ppStat

解决方法：opencv-3.0.0/modules/cudalegacy/src/graphcuts.cpp  ：将
`#if !defined (HAVE_CUDA) || defined (CUDA_DISABLER) `
改为

```
#if !defined (HAVE_CUDA) || defined (CUDA_DISABLER) || (CUDART_VERSION >= 8000)
```

参考：[http://blog.csdn.net/allyli0022/article/details/62859290](http://blog.csdn.net/allyli0022/article/details/62859290)

# 配置环境变量

此过程非常重要！！！此过程非常重要！！！此过程非常重要！！！
`sudo gedit /etc/ld.so.conf.d/opencv.conf`
在opencv.conf（打开之后是空哒！但是是正确的）里加入如下语句：
`/usr/local/lib`
之后保存退出。然后配置库文件：

```bash
sudo ldconfig
```

出错：*.so不是符号连接： 

解决：[http://blog.csdn.net/u013066730/article/details/70880141](http://blog.csdn.net/u013066730/article/details/70880141)

```
sudo ln -sf /usr/local/cuda-8.0/targets/x86_64-linux/lib/libcudnn.so.5.0.5 /usr/local/cuda-8.0/targets/x86_64-linux/lib/libcudnn.so.5

sudo mv /usr/lib/nvidia-375/libEGL.so.1 /usr/lib/nvidia-375/libEGL.so.1.org

sudo mv /usr/lib32/nvidia-375/libEGL.so.1 /usr/lib32/nvidia-375/libEGL.so.1.org

sudo ln -s /usr/lib/nvidia-375/libEGL.so.375.39 /usr/lib/nvidia-375/libEGL.so.1

sudo ln -s /usr/lib32/nvidia-375/libEGL.so.375.39 /usr/lib32/nvidia-375/libEGL.so.1
```

打开另外一个文件：

```bash
sudo gedit /etc/bash.bashrc
```

在文档最下方（fi之后）添加：

```
PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH
```

之后重启虚拟机，，或者注销再登陆。






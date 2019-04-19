# ubuntu16.04安装caffe以及各种问题汇总 - 数据之美的博客 - CSDN博客
2017年02月21日 20:53:15[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1548
本文参考了： 
[https://github.com/BVLC/caffe/wiki/Ubuntu-16.04-or-15.10-Installation-Guide](https://github.com/BVLC/caffe/wiki/Ubuntu-16.04-or-15.10-Installation-Guide)
[https://gist.github.com/wangruohui/679b05fcd1466bb0937f#file-caffe-ubuntu-15-10-md](https://gist.github.com/wangruohui/679b05fcd1466bb0937f#file-caffe-ubuntu-15-10-md)
[http://blog.csdn.net/g0m3e/article/details/51420565](http://blog.csdn.net/g0m3e/article/details/51420565)
前言： 
本文是基于已经成功安装了[Python](http://lib.csdn.net/base/python)各种常用包、[OpenCV](http://lib.csdn.net/base/opencv)、cuda
# 安装过程
### 1、首先安装各种更新包
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install -y build-essential cmake git pkg-config
sudo apt-get install -y libprotobuf-dev libleveldb-dev libsnappy-dev libhdf5-serial-dev protobuf-compiler
sudo apt-get install -y libatlas-base-dev
sudo apt-get install -y --no-install-recommends libboost-all-dev
sudo apt-get install -y libgflags-dev libgoogle-glog-dev liblmdb-dev
sudo apt-get install -y python-pip
sudo apt-get install -y python-dev
sudo apt-get install -y python-numpy python-scipy
sudo apt-get install -y libopencv-dev
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
### 2、下载caffe
```php
cd ~
mkdir git  //在home下新建一个git文件夹，用来存放那些从github上git下来的文zong件
git clone https://github.com/BVLC/caffe.git    //从github上git caffe
```
- 1
- 2
- 3
- 1
- 2
- 3
### 3、开始安装
```
cd caffe    //打开到刚刚git下来的caffe
cp Makefile.config.example Makefile.config //将Makefile.config.example的内容复制到Makefile.config
//因为make指令只能make   Makefile.config文件，而Makefile.config.example是caffe给出的makefile例子
gedit Makefile.config      //打开Makefile.config文件
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
仔细阅读makefile中的注释语句其实就知道该怎么操作了，为了方便理解，笔者还是介绍一各个配置说明。 
在打开的Makefile.config修改如下内容：
```
//如果你不使用GPU的话，就将
# CPU_ONLY := 1
修改成：
CPU_ONLY := 1
//若使用cudnn，则将
# USE_CUDNN := 1
修改成：
USE_CUDNN := 1
//若使用的opencv版本是3的，则将
# OPENCV_VERSION := 3
修改为：
OPENCV_VERSION := 3
//若要使用python来编写layer，则需要将
# WITH_PYTHON_LAYER := 1
修改为
WITH_PYTHON_LAYER := 1
//重要的一项
将# Whatever else you find you need goes here.下面的
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib
修改为：
INCLUDE_DIRS :=  $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu/hdf5/serial
//这是因为ubuntu16.04的文件包含位置发生了变化，尤其是需要用到的hdf5的位置，所以需要更改这一路径
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
### 4、为hdf5之类的文件创建新的链接
```
\\首先执行下面两句话：
find . -type f -exec sed -i -e 's^"hdf5.h"^"hdf5/serial/hdf5.h"^g' -e 's^"hdf5_hl.h"^"hdf5/serial/hdf5_hl.h"^g' '{}' \;
cd /usr/lib/x86_64-linux-gnu
\\然后根据情况执行下面两句：
sudo ln -s libhdf5_serial.so.10.1.0 libhdf5.so
sudo ln -s libhdf5_serial_hl.so.10.0.2 libhdf5_hl.so
\\注意：这里的10.1.0和10.0.2根据不同的系统可能对应的数字会不同，比如在ubuntu15.10中其数字就是8.0.2.
\\具体的数字可以在打开的文件中查看对应文件后面跟的数字
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
### 5、执行python文件夹下的指令
```bash
\\现在将目录cd到你之前git的caffe文件夹下：
cd python
for req in $(cat requirements.txt); do pip install $req; done
\\如果发现执行上述代码后，终端中有很多红字，一堆的错误之类的，那不管是什么错误都执行下面一句话：
for req in $(cat requirements.txt); do sudo -H pip install $req --upgrade; done
\\执行完上面这句话后应该就不会有很多红字错误了
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
### 6、make 各种文件
```
cd ..   \\此时位置应该处于caffe文件夹下
make all -j8  //j8代表计算机cpu有8个核，因此可以多线程一起make，这样make的速度会快很多。一般常用的还有j4
make test -j8
make runtest -j8
make pycaffe   //如果以后用python来开发的话必须执行这一句，一般不管你是否用python，都会执行这一句
make distribute
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
至此，安装caffe差不都就已经完成了。但是笔者在安装的过程中，遇到了各种各样问题，有时候解决了这个问题，那个问题又出现了，几近崩溃的边缘，究其原因还是对ubuntu这个系统熟悉。笔者在遇到问题google的时候都将界面留存了下来，下面罗列一下遇到问题以及解决方式。
# caffe安装错误集锦
### 1
在make all 的时候出现如下错误
`Error 'fatal error: pyconfig.h: No such file or directory' when executing 'make pycaffe' on dev-branch #410`- 1
- 1
解决方案：
```
make clean
export CPLUS_INCLUDE_PATH=/usr/include/python2.7
make all -j8
```
- 1
- 2
- 3
- 1
- 2
- 3
### 2
`string.h ‘memcy’ was not declared in this scope`- 1
- 1
解决方案：打开makefile搜索并替换
```
NVCCFLAGS += -ccbin=$(CXX) -Xcompiler -fPIC $(COMMON_FLAGS)
```
- 1
- 2
- 1
- 2
为
`NVCCFLAGS += -D_FORCE_INLINES -ccbin=$(CXX) -Xcompiler -fPIC $(COMMON_FLAGS)`- 1
- 1
保存退出
重新make
### 3
类似以下情况的错误：
```cpp
.build_release/lib/libcaffe.so: undefined reference to `cv::imread(cv::String const&, int)'
.build_release/lib/libcaffe.so: undefined reference to `cv::imencode(cv::String const&, cv::_InputArray const&, std::vector<unsigned char, std::allocator<unsigned char> >&, std::vector<int, std::allocator<int> > const&)'
```
- 1
- 2
- 1
- 2
这种问题很有可能是你的系统中使用的是opencv3，但是你的Makefile.config中没有将OPENCV_VERSION := 3这一句取消注释。
### 4
出现各种undefined referenceserrors，比如：
`undefined reference togoogle::base::CheckOpMessageBuilder::NewString()'`- 1
- 1
这类问题往往是因为gcc和g++编译器版本的问题，在ubuntu16.04中，其默认的gcc编译器版本是gcc 5,但是cuda目前又不支持高于4.9的编译器，因此会出现不兼容的情况。
解决方法： 
这里有一种比较暴力的解决方法，不确定是否安全。编辑`/usr/local/cuda/include/host_config.h`，将其中的第115行注释掉：
```
将
#error -- unsupported GNU version! gcc versions later than 4.9 are not supported!
改为
//#error -- unsupported GNU version! gcc versions later than 4.9 are not supported!
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
注意：查看现在的gcc编译器版本，可能有的人在之前安装cuda的时候会将默认的gcc、g++版本改成4.8之类比较老的版本，在make caffe的时候最好将gcc和g++的版本改到最新的，这样在make的时候才不会出现类似undefined references之类的错误。具体的修改方法可以google之。

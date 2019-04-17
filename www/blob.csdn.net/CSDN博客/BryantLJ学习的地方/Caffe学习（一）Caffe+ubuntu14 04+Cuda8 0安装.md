# Caffe学习（一）Caffe+ubuntu14.04+Cuda8.0安装 - BryantLJ学习的地方 - CSDN博客





2016年08月03日 13:17:05[遍地流金](https://me.csdn.net/u012177034)阅读数：13219








硬件配置：i76700K + GTX1080 + 32G 2400MHz RAM + 250G SSD




**1.      ubuntu14.04安装**

刚装完以后系统有可能卡顿非常明显，此时是因为显卡驱动没有安装。




**2.      GTX1080显卡驱动367.35安装**

Sudo apt-get update##更新源

sudo add-apt-repository ppa:graphics-drivers/ppa

按回车继续

sudo apt-get update 
sudo apt-get install nvidia-367 
sudo apt-get install mesa-common-dev 
sudo apt-get install freeglut3-dev

以上装完后重启电脑，此时驱动安装完毕，系统没有卡顿现象




**3.      安装gcc与g++**

Sudo apt-get install g++，没有g++的话，CUDA工具箱安装将不成功。

Ubuntu14.04默认自带gcc-4.8，因此不用安装。




**4.      安装Cuda8.0工具包**

（1）在Nvidia官网注册账号，下载Cuda8.0 ubuntu14.04，并按照官网指示进行安装



注意除了一个驱动程序361.32不安装外，其余均选择y。



（2）在安装完毕之后需要将cuda库文件路径添加到PATH与LD_LIBRARY_PATH里。在~/.bashrc文件里修改。在文件末端加入

exportPATH=/usr/local/cuda/bin:$PATH

export LD_LIBRARY_PATH=/usr/local/cuda/lib64/:$LD_LIBRARY_PATH

然后source命令使路径生效

Source ~/.bashrc

sudo ldconfig

 有时候上述方法会失败，总是出现找不到动态链接库。。。目前还不知道原因。


一个更靠谱的方法是将库文件路径添加到/etc/ld.so.conf文件里。即在文件末尾添加：/usr/local/cuda-8.0/lib64

最后命令行：sudo ldconfig使文件生效。


（3）验证cuda安装成功

进入NVIDIA_CUDA-8.0_Samples文件夹，进行make编译。之后进入1_Utilities/deviceQuery/文件夹，运行./deviceQuery命令，若出现Nvidia显卡的型号等信息，则说明安装成功



注意：Ubuntu16.04在安装时会遇到gcc版本不符合的问题，此时将系统中自带的gcc版本换为gcc4.9即可。

sudo [apt-get](https://www.baidu.com/s?wd=apt-get&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9PHDvnAnkPWbdPycdnjmk0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHT1n1RdPj64njDzn1ckPH0vPs) install gcc-4.9

sudo [apt-get](https://www.baidu.com/s?wd=apt-get&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9PHDvnAnkPWbdPycdnjmk0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHT1n1RdPj64njDzn1ckPH0vPs) install g++-4.9

先把目录切换到/usr/bin下。修改原先gcc与g++的软连接，使得新安装的gcc与g++生效。

sudo mv gcc gcc.bak 

sudo ln -s gcc-4.9 gcc

sudo mv g++ g++.bak

sudo ln -s g++-4.9 g++ 



**5.      CudnnV5安装**
下载cudnnv5，然后解压。然后将cudnn复制到cuda相应的头文件和库文件中即可sudo cp lib64/lib* /usr/local/cuda-8.0/lib64/sudo cp include/cudnn.h /usr/local/cuda-8.0/include/
cd /usr/local/cuda/lib64/

sudo chmod 777 libcudnn*即可

这里注意，复制完后需要将libcudnn*的权限都改为777，否则后面将不能执行~~~，编译时出现找不到Libcudnn.so或者libcudart.so.8.0文件的情况。




**6.      安装caffe依赖库**

（1）基本程序编译所需库build-essential，github库git

Sudo apt-get install –y build-essentialgit

（2）矩阵运算库

sudo apt-get install -y libopenblas-devlibblas-dev liblapack-dev

这里blas库负责基本的向量矩阵运算，lapack库调用blas库进行一些高级矩阵运算，比如矩阵的求逆，SVD分解，但这两者都只是定义了一组API，并没有底层具体的实现。底层的实现则靠openblas库。与openblas库对应的还有一些，如mkl，atlas等。

（3）opencv库

Sudo apt-get install libopencv-dev

（4）protobuf库

sudo apt-get install –y libprotobuf-devprotobuf-compiler

谷歌开发的用于结构化数据的读取和存入的一套API。类似与XML。改革是简单快速，但是没有xml所能表示的东西复杂。该库可以将符合proto格式的文件编译成C++接口，[Python](http://lib.csdn.net/base/11)接口，[Java](http://lib.csdn.net/base/17)接口的两个文件。当编译为C++接口时，一个为**.pb.c，一个为**.pb.h。在Caffe里主要用来存放网络结构文件.prototxt，以及对应于一个问题的solver.prototxt（包括网络的指定与各种训练参数）

（5）Boost库

sudo apt-get install –y libboost-all-dev

一套C++标准库的扩展库，里面主要包含了一些字符串处理，文本处理，图算法，容器等算法相关的库，是目前最大的C++扩展库

（6）GLOG库

sudo apt-get install –y libgoogle-glog-dev

GoogleLogger，谷歌开发的一套用于日志记录的C++库

（7）LMDB与LEVELDB库

sudo apt-get install –y libleveldb-devliblmdb-dev

[数据库](http://lib.csdn.net/base/14)管理库，包含了数据的读写和存储方式，高效安全。HDF5是一种文件格式，而LMDB是一种数据库管理的库

（8）snappy库

sudo apt-get install –y libsnappy-dev

一个用来压缩和解压缩的C++库~~，在Caffe里训练原始数据先通过格式转换工具转为HDF5格式，然后Snappy将该格式进行压缩以便放入内存中，然后采用LMDB方法对数据进行读取和更新~~

（9）HDF5库

sudo apt-get install –y libhdf5-serial-dev

美国国家超算中心开发的一种文件格式，便于用来存储大量的科学数据。在Caffe里可以用来存放训练和测试数据集。（和LMDB与LEVELDB的区别是啥？）

（10）gflags库

sudo apt-get install –y libgflags-dev

谷歌开源的处理命令行参数的库



上述依赖库安装完成后，便可以克隆caffe的源码，然后修改make.config文件

至此可以对caffe make all，make test，make runtest了，也可以在命令行窗口运行mnist数据集进行测试了



**7.      安装caffe的python接口及python相关库**

安装完该接口后，就能够在python中，通过import caffe方式来使用caffe，如此可以用到python的一些强大的库用来显示和计算，如matplotlib，sklearn等，这是命令行式caffe所难以达到的效果。

Cd caffe/python

（1）安装python包安装工具pip

sudo apt-get install python-devpython-pip

（2）sudo apt-get install python-opencv

 pip install pyzmq,tornado,jinja2,jsonschema,jupyter

（3）安装python包依赖，在requirements.txt文件里

apt-get install gfortran， libatlas-dev，libblas-dev

***此步应先执行，否则下一步requirement里的scipy会安装失败***

安装requirement里的python依赖包

for req in $(cat requirements.txt);do sudo pip install $req; done

（4）python包都安装无误后，编译pycaffe

Cd ..

Make pycaffe –j8

此时即可在python里import caffe




**8.      安装eclipse与jdk**

第一步安装高版本的jdk。ubuntu14.04自带的jdk版本较低，需要换为jdk1.80.1，此步参考网上教程，替换完成后，java –version可显示出Java的版本

第二步为安装eclipse，直接官网下载eclipse for c/c++的.tgz格式安装包，解压即可。

第三步为将caffe源码引入eclipse为一个新的makefile工程，然后便可对该工程进行debug。此步搜索caffe eclipse参考网上教程即可。






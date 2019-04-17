# Ubuntu16.04+caffe+CUDA8.0+cuDNN v5+opencv3.1.0+matcaffe编译 - Snoopy_Dream - CSDN博客





2018年03月12日 17:37:25[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：1046








## Ubuntu16.04+caffe+CUDA8.0+cuDNN v5+opencv3.1.0编译

这篇caffe安装非常不错，但是里面因为时间的原因，有些操作已经不能用了而且原文涉及到一些小问题，小编我是生怕之后自己的ubunutu崩掉，就将其错误改正，重新写了写。还有就是看得多了，容易只看代码，将代码上下的操作老师忘记，所以特此将重点红色标注。并将自己的makefile文件和makefile.config以及ippicv_linux_20151201.tgz链接良心附上: 链接:  [](https://pan.baidu.com/s/1mIbqXu2CoKf8DjX1JAUU-Q)链接: [https://pan.baidu.com/s/13qKQLY6E9iawpMkHRZVNyg](https://pan.baidu.com/s/13qKQLY6E9iawpMkHRZVNyg) 密码: xr2v


**1.安装Ubuntu16.04**

 Win10下UEFI环境安装Ubuntu 16.04双系统教程，亲测有效  [安装ubuntu双系统。](http://blog.csdn.net/DeMonliuhui/article/details/77483523)
在安装完之后，搜索-软件和更新，更新服务源，我选的阿里的服务器。 
 如果你是初次接触linux系统或ubuntu系统，建议你在装好Ubuntu系统后不要急着进行下一步，可以了解学习一下linux系统基础和基本指令  [linux基础和命令](http://pan.baidu.com/s/1dFsVu1N)

**2.安装NVIDIA显卡驱动**

**    (1) 安装NVIDIA**

     如果是普通台式机这一步完全可以按照，在软件更新下面，直接切换驱动，然后重启电脑，直接生效。

     如果你是ASUS笔记本，那就要进行下面的操作。

     直接终端下输出以下命令，不需要禁用开源驱动和关闭lightdm。

    如果你选择安装CUDA（本人主要是用于caffe），可以选择直接安装CUDA，不需要安装NVIDIA驱动（CUDA中自带NVIDIA驱动）。[*但是要选择直接deb安装*](https://developer.nvidia.com/cuda-90-download-archive?target_os=Linux&target_arch=x86_64&target_distro=Ubuntu&target_version=1604&target_type=deblocal)，不要选择run安装，否则会出现无限登陆的问题。之后**重启电脑**，使驱动生效。

```
sudo add-apt-repository ppa:graphics-drivers/ppa     
sudo apt-get update     
sudo apt-get upgrade  
sudo apt-get install nvidia-384
```

**     (2) 关闭安全模式**

    开机长按ESC，之后进bios选项，选择secure里面的securty boot修改为disable。 然后选择ubuntu系统进入之后，在终端下输出：nvidia-smi会显示你的显卡相关信息，证明你的NVIDIA驱动已经安装好了。也可以下搜索下搜索 详细信息，可以看到 图形下会显示你的NVIDIA的显卡型号，我的是GeoForce 940MX。

![](https://img-blog.csdn.net/20180128161747594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZTAxNTI4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180128161842678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZTAxNTI4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**3.安装CUDA8.0**

下载的是离线.deb包进行安装  CUDA8.0下载， 官网总是下一半，就失败了，简直“屡试不爽”。这里附上我的链接cuda-repo-ubuntu1604-8-0-local_8.0.44-1_amd64.deb 下载链接: [https://pan.baidu.com/s/18AXEM3CouQKrc-lnIwYYXQ](https://pan.baidu.com/s/18AXEM3CouQKrc-lnIwYYXQ) 密码: qevs


下载完成之后，打开命令行终端，进入到.deb包的下载目录下，依次执行以下命令，其中第一个指令中的.deb包就是自己下载的.deb包名。

```
sudo dpkg -i cuda-repo-ubuntu1604-8-0-local_8.0.44-1_amd64.deb
sudo apt-get update
sudo apt-get install cuda
```

安装完cuda后必须**重启**电脑，才能让显卡配置生效！

**4.安装cuDNN v5**

先说明一下，cuDNN v4是不支持ssd的，并且用v4编译faster r-cnn后测试score时结果不对，faster r-cnn默认是v3，通过替换和修改少量文件即可使用v5，因此在这里强烈建议大家使用v5版本。到NVIDIA官网下载cuDNN，进去要先用邮箱注册一下，填个问卷，很快的。选择cuDNN v5(May 27,2016),for CUDA 8.0下载，官网下载地址：  [cuDNN v5下载](https://developer.nvidia.com/rdp/cudnn-download)

![这里写图片描述](https://img-blog.csdn.net/20161224110018791?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMTQ5MTYyNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下载之后进入到下载文件所在文件夹，解压，进入cuda文件夹下，并执行相关文件的拷贝：

```
tar -zxvf cudnn-8.0-linux-x64-v5.0-ga.tgz(或你的版本对应的压缩包)
cd cuda
sudo cp lib64/lib* /usr/local/cuda/lib64/
sudo cp include/cudnn.h /usr/local/cuda/include/
```

然后更新软连接：

```bash
cd /usr/local/cuda/lib64/
sudo chmod +r libcudnn.so.5.0.5(或你自己的版本对应的)
sudo ln -sf libcudnn.so.5.0.5 libcudnn.so.5  
sudo ln -sf libcudnn.so.5 libcudnn.so  
sudo ldconfig
```

**5.添加环境变量**

编辑计算机文件夹下的/etc/profile（可以在终端使用以下命令行打开）：

```bash
sudo gedit /etc/profile
```

在文件末尾加入CUDA环境变量，保存。

```
PATH=/usr/local/cuda/bin:$PATH
export PATH
```

保存后在终端执行以下命令使其生效：

```bash
source /etc/profile
```

然后，进入到/etc/ld.so.conf.d/文件夹下，执行以下命令，新建了一个cuda.conf文件

```bash
sudo gedit cuda.conf
```

在文件中写入并保存：
`/usr/local/cuda/lib64`
保存后使其生效：

```bash
sudo ldconfig
```

**6.Build CUDA Sample**

进入usr/local/cuda/samples，然后build samples，命令如下：
`sudo make all -j4（4表示采用4核，当然如果你是8核你也可以用8）`
继续进入到samples/bin/x86_64/linux/release目录下，在终端执行查询命令： 
`./deviceQuery`
如果返回你电脑显卡信息，结尾有Result=PASS，则安装成功，否则，建议重启电脑再尝试查询命令。

**7.安装caffe的基本依赖库**

首先安装以下软件包括python，不过好像Ubuntu16.04已经自带了。

```
sudo apt-get install git
sudo apt-get install build-essential
sudo apt-get install python
```

然后安装caffe依赖库，建议一个一个装
`sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libboost-all-dev libhdf5-serial-dev libgflags-dev libgoogle-glog-dev liblmdb-dev protobuf-compiler libatlas-base-dev`
**8.安装opencv3.1.0**

原文的opencv安装，以前我安装的时候还能用，现在不行了，特此做更改。

###   （1）解压，创建build目录

```bash
unzip opencv-3.1.0.zip
cd opencv-3.1.0
mkdir build
```

###   （2）修改opencv源码，使其兼容cuda8.0
`vi opencv-3.1.0/modules/cudalegacy/src/graphcuts.cpp`
修改如下： 
![修改graphcuts.cpp文件截图](https://img-blog.csdn.net/20161115142049910)
将：
`#if !defined (HAVE_CUDA) || defined (CUDA_DISABLER)`
改为：
`#if !defined(HAVE_CUDA)||defined(CUDA_DISABLER)||(CUDART_VERSION>=8000)`
###    （3）配置opencv，生成Makefile

```
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D WITH_FFMPEG=ON -D CMAKE_INSTALL_PREFIX=/usr/local ..
```

如果因为ippicv_linux_20151201.tgz包下载失败而导致Makefile生成失败，可通过手动下载ippicv_linux_20151201.tgz安装包，将其拷贝至 opencv-3.1.0/3rdparty/ippicv/downloads/linux-808b791a6eac9ed78d32a7666804320e目录内(这里的目录不敢保证，最好使先让系统自己运行一下，自动生成文件夹后，然后再把压缩包放进去)，重新执行配置命令即可。

###     （4）编译
`make -j4`
编译过程中如果出现如下错误：
`/usr/include/string.h: In function ‘void* __mempcpy_inline(void*, const void*, size_t)’: /usr/include/string.h:652:42: error: ‘memcpy’ was not declared in this scope return (char *) memcpy (__dest, __src, __n) + __n;`
这是因为ubuntu的g++版本过高造成的，只需要在opencv-3.1.0目录下的CMakeList.txt 文件的开头加入：

```bash
set(CMAKE_CXX_FLAGS “${CMAKE_CXX_FLAGS} -D_FORCE_INLINES”)
```

添加之后再次进行编译即可。

###     （5）安装

```bash
sudo make install
```

```bash
sudo ldconfig
```

###     （6）查看版本号
`pkg-config --modversion opencv`
**9.配置caffe的python环境**

先下载caffe-master包，下载后解压，进入caffe-master/python文件夹下，然后在终端执行：[caffe-master下载](https://github.com/BVLC/caffe)

```
sudo apt-get install python-pip python-dev build-essential
sudo pip install --upgrade pip
sudo pip install -r requirements.txt
```

---------------------------------------------------------------------


如果，sudo pip install -r requirements.txt出现***has requirement **，but you'll have ** which is incompatible。比如：



matplotlib 2.2.2 has requirement python-dateutil>=2.1, but you'll have python-dateutil 1.5 which is incompatible.

![](https://img-blog.csdn.net/20180504152720682?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




根据提示进行升级操作，输入：

 sudo pip install --upgrade python-dateutil

-------------------------------------------------------------------


为了后面能够使用python绘制caffe的网络模型等可视化操作，执行：

```bash
sudo apt-get install graphviz
sudo pip install pydot
```

然后
`make pycaffe``---------------------------``“下面这个是为了在任何文件下，都可以python后import caffe避免spyder下import caffe报错”`
```
vi ~/.bashrc  //末尾加入
```

```
export PYTHONPATH=~/caffe-master/python:$PYTHONPATH//找到你的pycaffe路径
```

```
source ~/.bashrc
```
`-------------------------------`



**10.编译caffe(强烈建议直接下载我的makefile先试试，不行再说)**

进入到caffe-master文件夹下，复制配置文件，终端执行：
`cp Makefile.config.example Makefile.config`
打开Makefile.config文件，对以下几点去掉注释并修改：

```
USE_CUDNN := 1
OPENCV_version := 3
USE_OPENCV := 1
USE_LMDB := 1
WITH_PYTHON_LAYER := 1
```

如果只进行以上修改最后编译会报关于hdf5的错，解决方案参照博客  [编译caffe时hdf5报错](http://blog.csdn.net/xue_wenyuan/article/details/52037121)
解决方案如下，首先在Makefile.config文件大概第85行，找到
`INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include`
并修改成
`INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial/`
修改后保存，然后再打开Makefile文件，找到大概第173行
`LIBRARIES += glog gflags protobuf boost_system boost_filesystem m hdf5_hl hdf5`
并修改成
`LIBRARIES += glog gflags protobuf boost_system boost_filesystem m hdf5_serial_hl hdf5_serial`
修改完保存，然后在终端caffe目录下执行编译:

```
make all -j4
make test -j4
make runtest -j4
```

如果没有报错，那么恭喜你成功了，如果没有成功，请根据报错内容仔细检查整个过程是否有遗漏，或者百度谷歌报错内容来得到解决方案。

**11 绘制caffe网络模型**

caffe的python目录下有一个draw_net.py是python绘制caffe网络模型的工具。在终端进入caffe的python目录下，执行:
`python draw_net.py ../models/bvlc_alexnet/train_val.prototxt alexnet.png`
就会在caffe的python生成alexnet网络模型可视化图像alexnet.png。

![这里写图片描述](https://img-blog.csdn.net/20170224112104402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMTQ5MTYyNzk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果出现

```python
importError: libopencv_core.so.3.1: cannot open shared object file: No such file or directory
```

表明找不到动态链接库，只需要执行： sudo ldconfig

**12 编译matcaffe**

    修改makefile里面的CXXFLAGS，在CXXFLAGS += -MMD -MP下一行添加CXXFLAGS += -std=c++11后，直接 **make matcaffe** 。最后编译成功。

    结果如下：


![](https://img-blog.csdn.net/20170126070109076)




显示了一个警告，说是gcc的版本不支持。这个没有关系，只是警告，不是错误，最下面显示还是MEX成功了。

**下面的都不用看，一般来说make test不成功，但是也不会有什么影响，够用了！！**

**make test**

执行完命令后，又报错了。大致意思是：Matlab自带的库和Ubuntu的系统库之间发生了冲突，一言不和就废掉Matlab的自带库，使用Ubuntu系统库。

![](https://img-blog.csdn.net/20170126070128185)

解决方案：刚开始我是直接在终端输入：

- export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/:/usr/local/cuda-8.0/lib64    
- export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4:/usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4:/usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4:/usr/lib/x86_64-linux-gnu/libstdc++.so.6:/usr/lib/x86_64-linux-gnu/libfreetype.so.6    

- ---下面去掉的是因为导致之后make 命令不能用了


~~然后再 make mattest 成功，但是关掉终端后再打开测试还是会有同样的错误，于是我就直接在系统文件里面加入路径。~~
~~首先终端输入：~~

- ~~sudo gedit ~/.bashrc  ~~

~~然后把如下两个路径加到文件最下面。~~
- ~~export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/:/usr/local/cuda-8.0/lib64    ~~
- ~~exportLD_PRELOAD=/usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4:/usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4:/usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4:/usr/lib/x86_64-linux-gnu/libstdc++.so.6:/usr/lib/x86_64-linux-gnu/libfreetype.so.6     ~~

保存 退出，重启电脑。
make mattest 成功，结果如下：

![](https://img-blog.csdn.net/20161022105717605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


至此 matlab 接口就配置好了。




补充，多gpu训练，修改makefile.config下面取消注释USE_NCCL := 1

在home路径或者随便都可以


```python
$ git clone https://github.com/NVIDIA/nccl.git
$ cd nccl
$ sudo make install -j8`& sudo  ldconfig `
```
`# 该命令不执行会出现错误： error while loading shared libraries: libnccl.so.1: cannot open shared object file: No such file or directory`


然后重新make ，make py，make matcaffe





# Caffe+UbuntuKylin14.04_X64+CUDA 6.5配置 - wishchinYang的专栏 - CSDN博客
2015年01月14日 11:03:43[wishchin](https://me.csdn.net/wishchin)阅读数：1526

        在编译Caffe的漫长过程中，经过了一个又一个坑，掉进去再爬出来，挺有趣的。**对比原文有修改**！
        LInux下配置安装：（本文档使用同一块NVIDIA显卡进行显示与计算， 如分别使用不同的显卡进行显示和计算，则可能不适用。）
        原文链接：[http://blog.sciencenet.cn/blog-1583812-841855.html](http://blog.sciencenet.cn/blog-1583812-841855.html)
        参考链接：[http://www.cnblogs.com/platero/p/3993877.html](http://www.cnblogs.com/platero/p/3993877.html)
        官方文档：[http://caffe.berkeleyvision.org/installation.ht](http://caffe.berkeleyvision.org/installation.html)ml
## 1. 安装build-essentials
安装开发所需要的一些基本包：
```
```python
sudo apt-get install build-essential
```
```
## 2. 安装NVIDIA驱动 (3.4.0)
### 2.1 准备工作（2014-12-03更新）
在关闭桌面管理 lightdm 的情况下安装驱动似乎可以实现Intel 核芯显卡 来显示 + NVIDIA 显卡来计算。具体步骤如下：
1. 首先在BIOS设置里选择用Intel显卡来显示或作为主要显示设备
2. 进入Ubuntu， 按 `ctrl+alt+F1` 进入tty， 登录tty后输入如下命令
```
sudo service  lightdm  stop
```
该命令会关闭lightdm。如果你使用 gdm或者其他的desktop manager, 请在安装NVIDIA驱动前关闭他。
### 2.2 安装驱动
输入下列命令添加驱动源
```
sudo add-apt-repository ppa:xorg-edgers/ppa
sudo apt-get update
```
安装340版驱动 (CUDA 6.5.14目前最高仅支持340版驱动， 343， 346版驱动暂不支持)
```
sudo apt-get install nvidia-340
```
安装完成后, 继续安装下列包 (否则在运行sample时会报错)
```
sudo apt-get install nvidia-340-uvm
```
安装完成后 reboot.
## 3. 安装CUDA 6.5
【[点击此链接](http://developer.download.nvidia.com/compute/cuda/6_5/rel/installers/cuda_6.5.14_linux_64.run)】 下载CUDA 6.5. 
然后通过下列命令, 将下载得到的.run文件解压成三个文件, 分别为
- CUDA安装包: cuda-linux64-rel-6.5.14-18749181.run
- NVIDIA驱动: NVIDIA-Linux-x86_64-340.29.run
- SAMPLE包: cuda-samples-linux-6.5.14-18745345.run
这里不安装NVIDIA驱动
```
cuda6.5.run --extract=extract_path
```
注意, 需要通过下面命令给所有.run文件可执行权限
```
chmod +x *.run
```
### 3.1 安装CUDA
通过下列命令安装CUDA, 按照说明一步一步安装至完成.
```
sudo ./cuda-linux64-rel-6.5.14-18749181.run
```
**3.1.1 添加环境变量**
安装完成后需要在/etc/profile中添加环境变量, 在文件最后添加:
```
PATH=/usr/local/cuda-6.5/bin:$PATH
export PATH
```
保存后, 执行下列命令, 使环境变量立即生效
```
source /etc/profile
```
**3.1.2 添加lib库路径**
在 **/etc/ld.so.conf.d/**加入文件 **cuda.conf, **内容如下
```
/usr/local/cuda-6.5/lib64
```
执行下列命令使之立刻生效
```
sudo ldconfig
```
### 3.2 安装CUDA SAMPLE
首先安装下列依赖包
```python
sudo apt-get install freeglut3-dev build-essential libx11-dev libxmu-dev libxi-dev libglu1-mesa-dev
```
然后用下述命令安装sample文件
```python
sudo ./cuda-samples-linux-6.5.14-18745345.run
```
完成后编译Sample文件, 整个过程大概10分钟左右
```python
cd /usr/local/cuda-6.5/samples
sudo make
```
 全部编译完成后， 进入 samples/bin/x86_64/linux/release, sudo下运行deviceQuery
```
```python
sudo ./deviceQuery
```
```
如果出现下列显卡信息， 则驱动及显卡安装成功：
```python
./deviceQuery Starting...
 CUDA Device Query (Runtime API) version (CUDART static linking)
Detected 1 CUDA Capable device(s)
Device 0: "GeForce GTX 670"
  CUDA Driver Version / Runtime Version          6.5 / 6.5
  CUDA Capability Major/Minor version number:    3.0
  Total amount of global memory:                 4095 MBytes (4294246400 bytes)
  ( 7) Multiprocessors, (192) CUDA Cores/MP:     1344 CUDA Cores
  GPU Clock rate:                                1098 MHz (1.10 GHz)
  Memory Clock rate:                             3105 Mhz
  Memory Bus Width:                              256-bit
  L2 Cache Size:                                 524288 bytes
  Maximum Texture Dimension Size (x,y,z)         1D=(65536), 2D=(65536, 65536), 3D=(4096, 4096, 4096)
  Maximum Layered 1D Texture Size, (num) layers  1D=(16384), 2048 layers
  Maximum Layered 2D Texture Size, (num) layers  2D=(16384, 16384), 2048 layers
  Total amount of constant memory:               65536 bytes
  Total amount of shared memory per block:       49152 bytes
  Total number of registers available per block: 65536
  Warp size:                                     32
  Maximum number of threads per multiprocessor:  2048
  Maximum number of threads per block:           1024
  Max dimension size of a thread block (x,y,z): (1024, 1024, 64)
  Max dimension size of a grid size    (x,y,z): (2147483647, 65535, 65535)
  Maximum memory pitch:                          2147483647 bytes
  Texture alignment:                             512 bytes
  Concurrent copy and kernel execution:          Yes with 1 copy engine(s)
  Run time limit on kernels:                     Yes
  Integrated GPU sharing Host Memory:            No
  Support host page-locked memory mapping:       Yes
  Alignment requirement for Surfaces:            Yes
  Device has ECC support:                        Disabled
  Device supports Unified Addressing (UVA):      Yes
  Device PCI Bus ID / PCI location ID:           1 / 0
  Compute Mode:
     < Default (multiple host threads can use ::cudaSetDevice() with device simultaneously) >
deviceQuery, CUDA Driver = CUDART, CUDA Driver Version = 6.5, CUDA Runtime Version = 6.5, NumDevs = 1, Device0 = GeForce GTX 670
Result = PASS
```
##  4. 安装Intel MKL 
（如果没有可以安装OpenBLAS代替）解压安装包，下面有一个install_GUI.sh文件， 执行该文件，会出现图形安装界面，根据说明一步一步执行即可。
**1.安装 MKL **
注意： 安装完成后需要添加library路径
```
```python
sudo gedit /etc/ld.so.conf.d/intel_mkl.conf
```
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
## 5. 安装OpenCV
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
## 6. 安装其他依赖项
Ubuntu14.04用户执行
```python
sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libboost-all-dev libhdf5-serial-dev libgflags-dev libgoogle-glog-dev liblmdb-dev protobuf-compiler
```
其他版本用户参考官方说明：http://caffe.berkeleyvision.org/installation.html
## 7. 安装Caffe所需要的Python环境
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
```python
# add library path
LD_LIBRARY_PATH=your_anaconda_path/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
```
开启另一个终端后即生效，并且重启后能够顺利加载lightdm, 进入桌面环境。
## 8. 安装MATLAB
Caffe提供了MATLAB接口， 有需要用MATLAB的同学可以额外安装MATLAB。
安装教程请自行搜索。 
一般过程是：
1.下载matlab iso镜像文件（。搜索matlab unix版可得到，我下的是2010a)
2. 打开终端，输入
     sudo mkdir /mnt/temp (建立临时文件夹存放装载后的iso文件)；
     sudo mount -o loop /路径/Matlab.R2010a.UNIX.ISO-TBE.iso  /mnt/temp
 (绿色路径为matlab iso文件所在位置)，回车后装载镜像文件完成。
3. 创建安装matlab的文件夹（可以自己决定位置，但是要记住，后面会用到，eg: /usr/local/matlab）。
    创建命令： sudo mkdir /usr/local/matlab
4. 开始安装sudo /mnt/temp/install 此时会弹出类似于windows下安装的图形化安装界面，**选择不联网安装**，
    会提示输入序列号，序列号在 /mnt/temp/crack 文件夹下的install文件里，或者serials的TXT文件里。
   选择安装路径为你在第三步创建的文件夹。安装接近最后 会问你是不是需要激活，选择不联网激活，
    选择 /mnt/temp/crack 文件夹下的lic_standalone.dat文件或者**serials 里面的lic文件**即可。
    激活成功！
5. 安装完成后卸载镜像文件。sudo umount /mnt/temp
************************ 至此安装部分顺利完成，接下来是如何启动 *********************************
1. 在终端启动，进入安装matlab的路径并进入bin文件夹，在终端输入./matlab -desktop
    或者，linux桌面直接建立一个指向文件的快捷方式。
安装完成后添加图标 [http://www.linuxidc.com/Linux/2011-01/31632.htm](http://www.linuxidc.com/Linux/2011-01/31632.htm)
```
sudo vi /usr/share/applications/Matlab.desktop
```
输入以下内容
```python
[Desktop Entry]
Type=Application
Name=Matlab
GenericName=Matlab 2010b
Comment=Matlab:The Language of Technical Computing
Exec=sh /usr/local/MATLAB/R2010b/bin/matlab -desktop
Icon=/usr/local/MATLAB/Matlab.png
Terminal=false
Categories=Development;Matlab;
```
(I use the R2013b patched package. First you should uncompress the .iso file. Then use**sudo cp** to copy the patch file)
## 9. 编译Caffe
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
```python
make all -j4
make test
make runtest
```
注意 -j4 是指使用几个线程来同时编译， 可以加快速度， j后面的数字可以根据CPU core的个数来决定， 我的CPU使4核， 所以-j4.
### 9.1. 编译Matlab wrapper
执行如下命令
```
make matcaffe
```
然后就可以跑官方的matlab demo啦。
### 9.2. 编译Python wrapper
```
make pycaffe
```
然后基本就全部安装完拉.
接下来大家尽情地跑demo吧～
----------------------------------
# 10. 安装cuDNN
为了加速Caffe，可以安装cuDNN，参见这篇文章：[NVIDIA CuDNN 安装说明](http://www.cnblogs.com/platero/p/4118139.html)
**11.使用MNIST数据集进行测试**
Caffe默认情况会安装在
home/username/caffe-master，
所以下面的工作，默认已经切换到了该工作目录。下面的工作主要是，用于测试Caffe是否工作正常，不做详细评估。具体设置请参考官网：http://caffe.berkeleyvision.org/gathered/examples/mnist.html
1. 数据预处理
可以用下载好的数据集，也可以重新下载，我网速快，这里就偷懒直接下载了，具体操作如下：
```python
$ cd data/mnist
$ sudo sh ./get_mnist.sh
```
2. 重建LDB文件，就是处理二进制数据集为Caffe识别的数据集，以后所有的数据，包括jpe文件都要处理成这个格式
```python
$ cd examples/mnist
$ sudo sh ./create_mnist.sh   （sudo sh examples/mnist/create_mnist.sh）
```
生成mnist-train-leveldb/ 和 mnist-test-leveldb/文件夹，这里包含了LDB格式的数据集
PS: 这里可能会遇到一个报错信息：
Creating lmdb...
./create_mnist.sh: 16: ./create_mnist.sh: build/examples/mnist/convert_mnist_data.bin: not found
解决方法是，直接到Caffe-master的根目录执行，实际上新版的Caffe，基本上都得从根目录执行。
**~/caffe-master$ sudo sh examples/mnist/create_mnist.sh**
3. 训练mnist
```python
$ sudo sh examples/mnist/train_lenet.sh
```
至此，Caffe安装的所有步骤完结，下面是一组简单的数据对比，实验来源于MNIST数据集，主要是考察一下不同系统下CPU和GPU的性能。可以看到明显的差别了，虽然MNIST数据集很简单，相信复杂得数据集，差别会更大，Ubuntu+GPU是唯一的选择了。
测试平台：
```python
i7-4770K/16G/GTX 770/CUDA 6.5
                MNIST Windows8.1 on CPU：620s
                MNIST Windows8.1 on GPU：190s
                MNIST Ubuntu 14.04 on CPU：270s
                MNIST Ubuntu 14.04 on GPU：160s
                MNIST Ubuntu 14.04 on GPU with cuDNN：35s
                Cifar10_full on GPU wihtout cuDNN：73m45s = 4428s　（Iteration 70000）
                Cifar10_full on GPU with cuDNN：20m7s = 1207s　（Iteration 70000）
```

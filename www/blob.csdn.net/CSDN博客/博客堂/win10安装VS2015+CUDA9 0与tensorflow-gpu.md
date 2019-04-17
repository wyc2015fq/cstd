# win10安装VS2015+CUDA9.0与tensorflow-gpu - 博客堂 - CSDN博客





2018年10月08日 10:46:44[最小森林](https://me.csdn.net/u012052268)阅读数：1777








# win10安装VS2015+CUDA9.0与tensorflow-gpu




### 文章目录
- [win10安装VS2015+CUDA9.0与tensorflow-gpu](#win10VS2015CUDA90tensorflowgpu_1)
- [0 前言](#0__4)
- [1 准备工作](#1__14)
- [1.1 Windows 10](#11_Windows_10_23)
- [1.2 Python 3.6（anancanda5.2）](#12_Python_36anancanda52_26)
- [1.3 Visual Studio](#13_Visual_Studio_32)
- [1.3 CUDA 9.0 （官网默认是10.0 不要用](#13_CUDA_90_100__48)
- [1.4 CuDnn 7.0](#14_CuDnn_70_63)
- [2 安装过程 (建议使用管理员权限账号)](#2___74)
- [2.1 Anaconda下载安装](#21_Anaconda_78)
- [2.2 安装Visual Studio 2015](#22_Visual_Studio_2015_92)
- [2.3安装CUDA](#23CUDA_103)
- [2.4. 验证CUDA是否安装成功:](#24_CUDA_115)
- [2.4.1 nvcc -V](#241_nvcc_V_118)
- [2.4.2. 使用VS和CUDA编译测试文件](#242_VSCUDA_128)
- [2.4.3. deviceQuery和bandwidthTest验证](#243_deviceQuerybandwidthTest_157)
- [2.5. 安装CuDnn](#25_CuDnn_168)
- [2.6. 确认系统环境变量(Environment Variables)](#26_Environment_Variables_174)
- [2.7. 安装TensorFlow的GPU版本](#27_TensorFlowGPU_186)
- [3. 第一个TensorFlow程序！](#3_TensorFlow_200)




## 0 前言

实验室与某车企合作自动驾驶相关的项目，我在其中负责道路车道线识别的工作。

真实驾驶视频数据集由车企提供，数据量很大，为了训练模型，导师给更新了主机：512G固态硬盘+32G内存+GTX1080显卡。体验爽的一匹。

由于之前的电脑都是师兄配置的，现在他毕业了，只好我自己来配。本文主要参考大神“微调”知乎专栏文章；以及“大米饭盖不住四喜丸子”CSDN博客：

> 
[https://zhuanlan.zhihu.com/p/29841665](https://zhuanlan.zhihu.com/p/29841665)
[https://blog.csdn.net/weixin_36368407/article/details/54177380](https://blog.csdn.net/weixin_36368407/article/details/54177380)


在其基础上，排除一些自己遇到的坑，把过程记录下来。在开始之前想再次提醒大家，在Windows上安装CUDA和TensorFlow有很多坑，对于各种软件版本以及系统设置的要求可谓非常刁钻，很容易就“误入陷阱”。 我在其中就重装了电脑一次，还好是新电脑 没有啥资料。

## 1 准备工作

简单的说，我们需要以下软件包。安装的过程要按照这个顺序：
- Windows 10
- Python 3.6（anancanda5.2）
- Visual Studio 2015
- CUDA 9.0 （官网默认是10.0 不要用）
- CuDnn 7.0

### 1.1 Windows 10

现在实验室 基本上都是win10 了 ，必须适应~

### 1.2 Python 3.6（anancanda5.2）

习惯使用anancanda去管理python，相信我，绝对好用！！！！可以去清华大学的镜像网站下载anaconda：[https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/)

注意下载与自己相同的版本，以及anaconda5.2对应的是 python3.6.6 版本。经测试可以完美使用tensorflow~

### 1.3 Visual Studio

由于英伟达cuda等程序需要编译，要要使用Visual Studio C++ 的运行环境。

一定别装2017！我因为这个重装电脑了。。。都是泪

Visual Studio 2015的话现在最新的版本是Visual Studio 2015 with Update 3，去官网下载免费的Community版本即可。

但是，微软为了推销新版本，把旧版本藏的很深，我找了挺久才找到， 可以按照下图提示去下载Community版本，登录一下即可。

地址：[https://visualstudio.microsoft.com/zh-hans/vs/older-downloads/](https://visualstudio.microsoft.com/zh-hans/vs/older-downloads/)

![image](http://wx1.sinaimg.cn/mw690/006qDjsOgy1fw0lbjsqpoj30yu0h0jt2.jpg)

### 1.3 CUDA 9.0 （官网默认是10.0 不要用

[TensorFlow官网](https://www.tensorflow.org/install/install_windows?hl=zh-cn)上显示支持的CUDA版本截止到今天（2018年10月8日）还是 CUDA 9.0。千万要注意英伟达官网上的默认版本是CUDA 10.0，请不要下载安装这个版本。

下载地址：[https://developer.nvidia.com/cuda-toolkit-archive](https://developer.nvidia.com/cuda-toolkit-archive)

网络比较好的朋友下载网络安装版本。或者下载本地（local）版本。

我在此处遇到一个问题，下载本地版本时候 每次到99%的时候就中断。。。 气的一匹。最后还是在百度云上找了个同名的文件下载的。

![image](http://i.imgur.com/oD083pk.jpg)

![image](http://i.imgur.com/oD083pk.jpg)

### 1.4 CuDnn 7.0

CuDnn 7.0 for CUDA9.0。这是可以使用的版本，下载时请对应的你的操作系统选择合适的版本。

请特别注意下载CuDnn前必须注册英伟达社区的会员，[https://developer.nvidia.com/rdp/form/cudnn-download-survey，虽然说是英文的但非常简单。（下载时必须登录，用微信可以很方便的登录）](https://developer.nvidia.com/rdp/form/cudnn-download-survey%EF%BC%8C%E8%99%BD%E7%84%B6%E8%AF%B4%E6%98%AF%E8%8B%B1%E6%96%87%E7%9A%84%E4%BD%86%E9%9D%9E%E5%B8%B8%E7%AE%80%E5%8D%95%E3%80%82%EF%BC%88%E4%B8%8B%E8%BD%BD%E6%97%B6%E5%BF%85%E9%A1%BB%E7%99%BB%E5%BD%95%EF%BC%8C%E7%94%A8%E5%BE%AE%E4%BF%A1%E5%8F%AF%E4%BB%A5%E5%BE%88%E6%96%B9%E4%BE%BF%E7%9A%84%E7%99%BB%E5%BD%95%EF%BC%89)

正确的下载版本如下图所示：

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa3b066162.jpg?imageMogr2/format/jpg/quality/90)

## 2 安装过程 (建议使用管理员权限账号)

请尽量按照本文的安装顺序进行安装，否则有可能运行失败！最重要的是，安装CUDA必须在安装Visual Studio之后，这两个顺序不可以调换。

### 2.1 Anaconda下载安装

Window下安装非常简单，直接运行安装包下一步下一步就行，安装过程会有选择打钩的地方，
- 第一个是添加环境变量！！！！！
- 第二个是把Anaconda下的Python设置为默认版本，这里建议都勾选。！！！！！

![image](http://i.imgur.com/qRZeZdC.jpg)

安装完成后，激活Anaconda3环境，运行python -V

![image](http://i.imgur.com/Xq1WyD1.png)

### 2.2 安装Visual Studio 2015

安装VS2015没什么难度，只有一个选项需要自定义，别的选项都可以使用默认值。VS2015的默认安装不包括“C+ +”的编译器，必须手动勾选Visual C+ +，不然会面临后续的CUDA编译错误！！！

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa4bda6315.jpg?imageMogr2/format/jpg/quality/90)

主要原因是VS2015在安装时并没有默认安装C++的编译器。

这里真是服了必须吐槽：vs这个专门用来编写C+ + 的编辑器，竟然默认不安装C+ + ，简直是神经病，害的我重装电脑。

### 2.3安装CUDA

安装CUDA前请务必确认VS2015安装成功！这个时候你需要安装CUDA，双击我们下载的安装文件即可，一切都选择默认即可。

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa4fecf27d.jpg?imageMogr2/format/jpg/quality/90)

安装成功后会看到如下图：

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa515bb779.jpg?imageMogr2/format/jpg/quality/90)

### 2.4. 验证CUDA是否安装成功:

#### 2.4.1 nvcc -V

打开命令行，也就是cmd然后输入“nvcc -V”，如果安装正确的话你应该看到这样的输出：

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa5468161d.jpg?imageMogr2/format/jpg/quality/90)

输出中显示了CUDA的版本是release 8.0。（别人的图，我自己显示9.0 懒得截图了，知道就好）

#### 2.4.2. 使用VS和CUDA编译测试文件

进入

“C:\ProgramData\NVIDIA Corporation\CUDA Samples\v8.0”

文件夹，双击打开 “Samples vs2015”这个文件。

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa55c58866.jpg?imageMogr2/format/jpg/quality/90)

选择编译生成1_ Utilities中所有的文件。

具体操作就是在1_ Utilities上右键选择Build，注意上面红框部分的64位和Release：

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa57ec1611.jpg?imageMogr2/format/jpg/quality/90)

如果你没有遇到编译错误，那么应该看到下图提示5个文件编译成功：

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa5d4496f5.jpg?imageMogr2/format/jpg/quality/90)

在成功后，你会发现在“C:\ProgramData\NVIDIA Corporation\CUDA Samples\v8.0\bin\win64\Release”文件夹中出现了一堆文件，我们主要需要deviceQuery和bandwidthTest。

如果你看到这两个错误:
- TRK0005: Failed to locate: “CL.exe”. The system cannot find the file specified.系统找不到CL.exe，无法编译文件。面对这个错误那就是你没选Visual C++，需要重新安装时选上这个组件。
- MSB8036: Windows SDK 8.1 was not found.如果你同时看到了这个错误说你没有安装 Windows SDK 8.1，那也可以通过安装C++ Redistribution这个来解决。
- 如果你发现错误提示是无法找到 CUDA.prop那么就是CUDA安装错误，建议检查CUDA版本重新安装。

#### 2.4.3. deviceQuery和bandwidthTest验证

运行我们刚才编译出来的deviceQuery.exe，也就是在cmd中运行这个文件，下图中左下的红框显示 result = pass代表安装测试成功，右下的红框是你的显卡型号请确认型号正确。Tesla K80只是我的这台机器型号，这个每个人可能都不一样。

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa6333da90.jpg?imageMogr2/format/jpg/quality/90)

运行我们刚才编译出来的bandwidthTest.exe，方法一样，也是关注是否result = PASS。

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa65302cc8.jpg?imageMogr2/format/jpg/quality/90)

### 2.5. 安装CuDnn

解压缩我们下载的CuDnn文件，得到3个文件夹：bin, include, lib。如下图所示，将这个三个文件夹复制到“C:\ProgramData\NVIDIA GPU Computing Toolkit\v8.0”。

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa67a8fd63.jpg?imageMogr2/format/jpg/quality/90)

### 2.6. 确认系统环境变量(Environment Variables)

确认Python3.5.4.和Pip3在系统环境变量中，检查的方法上面已经介绍了。

打开系统环境变量设置，以Win10为例，请参考[百度经验](https://link.zhihu.com/?target=http%3A//jingyan.baidu.com/article/8ebacdf02d3c2949f65cd5d0.html)介绍。

确认CUDA_PATH和CUDA_PATH_V8.0已经存在

并手动添加 “C:\ProgramData\NVIDIA GPU Computing Toolkit\v8.0\bin”到Path里面。

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa69eb0d50.jpg?imageMogr2/format/jpg/quality/90)

### 2.7. 安装TensorFlow的GPU版本

打开cmd，输入“pip install tensorflow-gpu”

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa7023f5ff.jpg?imageMogr2/format/jpg/quality/90)

注意：
- 如果你电脑中有python2这里就必须输入：pip3
- 要安装tensorflow-gpu，而不是tensorflow
- 如果安装失败，很有可能你的Python版本不是3.5.，或者pip3版本太低，可以使用"pip3 install --upgrade pip3"来升级pip3

## 3. 第一个TensorFlow程序！

恭喜你，我们离胜利已经一步之遥了:) 让我们来验证我们安装的TensorFlow可以使用GPU！

使用pycharm，输入以下代码并运行：

```
import tensorflow as tf

a = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[2, 3], name='a')
b = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[3, 2], name='b')
c = tf.matmul(a, b)

sess = tf.Session(config=tf.ConfigProto(log_device_placement=True))

print sess.run(c)
```

![image](https://static.leiphone.com/uploads/new/article/740_740/201711/5a1fa74b6cf6b.jpg?imageMogr2/format/jpg/quality/90)




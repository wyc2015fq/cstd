# Caffe 深度学习框架上手教程 - 战斗蜗牛的专栏 - CSDN博客





2016年07月29日 21:59:10[vbskj](https://me.csdn.net/vbskj)阅读数：910









Caffe2是一个清晰而高效的深度学习框架，其作者是博士毕业于UC
 Berkeley的贾扬清12，目前在Google工作。


Caffe是纯粹的C++/CUDA架构，支持命令行、Python和MATLAB接口；可以在CPU和GPU直接无缝切换：
`Caffe::set_mode(Caffe::GPU);`
### Caffe的优势
- 

上手快：模型与相应优化都是以文本形式而非代码形式给出。

Caffe给出了模型的定义、最优化设置以及预训练的权重，方便立即上手。

- 

速度快：能够运行最棒的模型与海量的数据。

Caffe与cuDNN结合使用，测试AlexNet模型，在K40上处理每张图片只需要1.17ms.

- 

模块化：方便扩展到新的任务和设置上。

可以使用Caffe提供的各层类型来定义自己的模型。

- 

开放性：公开的代码和参考模型用于再现。

- 

社区好：可以通过BSD-2参与开发与讨论。


### Caffe的网络定义


Caffe中的网络都是有向无环图的集合，可以直接定义：

```
name: "dummy-net"layers {name: "data" …}
layers {name: "conv" …}
layers {name: "pool" …}
layers {name: "loss" …}
```


数据及其导数以blobs的形式在层间流动。

### Caffe的各层定义


Caffe层的定义由2部分组成：层属性与层参数，例如

```
name:"conv1"type:CONVOLUTION
bottom:"data"top:"conv1"convolution_param{
    num_output:20
    kernel_size:5
    stride:1
    weight_filler{        type: "xavier"
    }
}
```


这段配置文件的前4行是层属性，定义了层名称、层类型以及层连接结构（输入blob和输出blob）；而后半部分是各种层参数。

### Blob


Blob是用以存储数据的4维数组，例如
- 

对于数据：Number*Channel*Height*Width

- 

对于卷积权重：Output*Input*Height*Width

- 

对于卷积偏置：Output*1*1*1


### 训练网络


网络参数的定义也非常方便，可以随意设置相应参数。


甚至调用GPU运算只需要写一句话：
`solver_mode:GPU`


### Caffe的安装与配置


Caffe需要预先安装一些依赖项，首先是CUDA驱动。不论是CentOS还是Ubuntu都预装了开源的nouveau显卡驱动（SUSE没有这种问题），如果不禁用，则CUDA驱动不能正确安装。以Ubuntu为例，介绍一下这里的处理方法，当然也有其他处理方法。
`# sudo vi/etc/modprobe.d/blacklist.conf# 增加一行 ：blacklist nouveausudoapt-get --purge remove xserver-xorg-video-nouveau   #把官方驱动彻底卸载：sudoapt-get --purge remove nvidia-*    #清除之前安装的任何NVIDIA驱动sudo service lightdm stop    #进命令行，关闭Xserversudo kill all Xorg`

安装了CUDA之后，依次按照Caffe官网安装指南6安装BLAS、OpenCV、Boost即可。

### Caffe跑跑MNIST试试


在Caffe安装目录之下，首先获得MNIST数据集：

```
cd data/mnist
sh get_mnist.sh
```


生成mnist-train-leveldb/ 和 mnist-test-leveldb/，把数据转化成leveldb格式：
`sh examples/mnist/create_mnist.sh`

训练网络:
`sh train_lenet.sh`












### 让Caffe生成的数据集能在Theano上直接运行


不论使用何种框架进行CNNs训练，共有3种数据集：
- 

Training Set：用于训练网络

- 

Validation Set：用于训练时测试网络准确率

- 

Test Set：用于测试网络训练完成后的最终正确率


#### Caffe生成的数据分为2种格式：Lmdb和Leveldb


它们都是键/值对（Key/Value Pair）嵌入式数据库管理系统编程库。

虽然lmdb的内存消耗是leveldb的1.1倍，但是lmdb的速度比leveldb快10%至15%，更重要的是lmdb允许多种训练模型同时读取同一组数据集。

因此lmdb取代了leveldb成为Caffe默认的数据集生成格式。

#### Google Protocol Buffer的安装


Protocol Buffer是一种类似于XML的用于序列化数据的自动机制。

首先在Protocol Buffers的中下载最新版本：

https://developers.google.com/protocol-buffers/docs/downloads

解压后运行：

```
./configure$ make$ make check$ make install
pip installprotobuf
```

#### 添加动态链接库
`export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH`
#### Lmdb的安装
`pip install lmdb`

要parse（解析）一个protobuf类型数据，首先要告诉计算机你这个protobuf数据内部是什么格式（有哪些项，这些项各是什么数据类型的决定了占用多少字节，这些项可否重复，重复几次），安装protobuf这个module就可以用protobuf专用的语法来定义这些格式（这个是.proto文件）了，然后用protoc来编译这个.proto文件就可以生成你需要的目标文件。

想要定义自己的.proto文件请阅读：

https://developers.google.com/protocol-buffers/docs/proto?hl=zh-cn2

#### 编译.proto文件
`protoc--proto_path=IMPORT_PATH --cpp_out=DST_DIR --java_out=DST_DIR--python_out=DST_DIR path/to/file.proto`
```
--proto_path 也可以简写成-I 是.proto所在的路径
输出路径：
--cpp_out 要生成C++可用的头文件，分别是***.pb.h（包含申明类）***.pb.cc（包含可执行类），使用的时候只要include “***.pb.h”
--java_out 生成java可用的头文件
--python_out 生成python可用的头文件，**_pb2.py，使用的时候import**_pb2.py即可
最后一个参数就是你的.proto文件完整路径。
```











### Caffe (CNN, deep learning) 介绍


Caffe ———–Convolution Architecture For Feature Embedding (Extraction)
- 

Caffe 是什么东东？
- 

CNN (Deep Learning) 工具箱

- 

C++ 语言架构

- 

CPU 和GPU 无缝交换

- 

Python 和matlab的封装

- 

但是，Decaf只是CPU 版本。


- 

为什么要用Caffe？
- 

运算速度快。简单 友好的架构 用到的一些库：

- 

Google Logging library (Glog): 一个C++语言的应用级日志记录框架，提供了C++风格的流操作和各种助手宏.

- 

lebeldb(数据存储)： 是一个google实现的非常高效的kv数据库，单进程操作。

- 

CBLAS library（CPU版本的矩阵操作）

- 

CUBLAS library (GPU 版本的矩阵操作)


- 

Caffe 架构



- 

预处理图像的leveldb构建

输入：一批图像和label （2和3）

输出：leveldb （4）

指令里包含如下信息：



- 

conver_imageset （构建leveldb的可运行程序）

- 

train/ （此目录放处理的jpg或者其他格式的图像)

- 

label.txt (图像文件名及其label信息)

- 

输出的leveldb文件夹的名字

- 

CPU/GPU (指定是在cpu上还是在gpu上运行code)


- 

CNN网络配置文件
- 

Imagenet_solver.prototxt （包含全局参数的配置的文件）

- 

Imagenet.prototxt （包含训练网络的配置的文件）

- 

Imagenet_val.prototxt （包含测试网络的配置文件）













### Caffe深度学习之图像分类模型AlexNet解读


在imagenet上的图像分类challenge上Alex提出的alexnet网络结构模型赢得了2012届的冠军。要研究CNN类型DL网络模型在图像分类上的应用，就逃不开研究alexnet，这是CNN在图像分类上的经典模型（DL火起来之后）。


在DL开源实现caffe的model样例中，它也给出了alexnet的复现，具体网络配置文件如下train_val.prototxt4

接下来本文将一步步对该网络配置结构中各个层进行详细的解读（训练阶段）：
- 

conv1阶段DFD（data flow diagram）：


- 

conv2阶段DFD（data flow diagram）：


- 

conv3阶段DFD（data flow diagram）：






SouthEast2455×274

- 

conv4阶段DFD（data flow diagram）：


- 

conv5阶段DFD（data flow diagram）：


- 

fc6阶段DFD（data flow diagram）：


- 

fc7阶段DFD（data flow diagram）：


- 

fc8阶段DFD（data flow diagram）：




各种layer的operation更多解释可以参考Caffe Layer Catalogue6


从计算该模型的数据流过程中，该模型参数大概5kw+。

caffe的输出中也有包含这块的内容日志，详情如下：

```
I0721 10:38:15.326920  4692 net.cpp:125] Top shape: 256 3 227 227 (39574272)
I0721 10:38:15.326971  4692 net.cpp:125] Top shape: 256 1 1 1 (256)
I0721 10:38:15.326982  4692 net.cpp:156] data does not need backward computation.
I0721 10:38:15.327003  4692 net.cpp:74] Creating Layer conv1
I0721 10:38:15.327011  4692 net.cpp:84] conv1 <- data
I0721 10:38:15.327033  4692 net.cpp:110] conv1 -> conv1
I0721 10:38:16.721956  4692 net.cpp:125] Top shape: 256 96 55 55 (74342400)
I0721 10:38:16.722030  4692 net.cpp:151] conv1 needs backward computation.
I0721 10:38:16.722059  4692 net.cpp:74] Creating Layer relu1
I0721 10:38:16.722070  4692 net.cpp:84] relu1 <- conv1
I0721 10:38:16.722082  4692 net.cpp:98] relu1 -> conv1 (in-place)
I0721 10:38:16.722096  4692 net.cpp:125] Top shape: 256 96 55 55 (74342400)
I0721 10:38:16.722105  4692 net.cpp:151] relu1 needs backward computation.
I0721 10:38:16.722116  4692 net.cpp:74] Creating Layer pool1
I0721 10:38:16.722125  4692 net.cpp:84] pool1 <- conv1
I0721 10:38:16.722133  4692 net.cpp:110] pool1 -> pool1
I0721 10:38:16.722167  4692 net.cpp:125] Top shape: 256 96 27 27 (17915904)
I0721 10:38:16.722187  4692 net.cpp:151] pool1 needs backward computation.
I0721 10:38:16.722205  4692 net.cpp:74] Creating Layer norm1
I0721 10:38:16.722221  4692 net.cpp:84] norm1 <- pool1
I0721 10:38:16.722234  4692 net.cpp:110] norm1 -> norm1
I0721 10:38:16.722251  4692 net.cpp:125] Top shape: 256 96 27 27 (17915904)
I0721 10:38:16.722260  4692 net.cpp:151] norm1 needs backward computation.
I0721 10:38:16.722272  4692 net.cpp:74] Creating Layer conv2
I0721 10:38:16.722280  4692 net.cpp:84] conv2 <- norm1
I0721 10:38:16.722290  4692 net.cpp:110] conv2 -> conv2
I0721 10:38:16.725225  4692 net.cpp:125] Top shape: 256 256 27 27 (47775744)
I0721 10:38:16.725242  4692 net.cpp:151] conv2 needs backward computation.
I0721 10:38:16.725253  4692 net.cpp:74] Creating Layer relu2
I0721 10:38:16.725261  4692 net.cpp:84] relu2 <- conv2
I0721 10:38:16.725270  4692 net.cpp:98] relu2 -> conv2 (in-place)
I0721 10:38:16.725280  4692 net.cpp:125] Top shape: 256 256 27 27 (47775744)
I0721 10:38:16.725288  4692 net.cpp:151] relu2 needs backward computation.
I0721 10:38:16.725298  4692 net.cpp:74] Creating Layer pool2
I0721 10:38:16.725307  4692 net.cpp:84] pool2 <- conv2
I0721 10:38:16.725317  4692 net.cpp:110] pool2 -> pool2
I0721 10:38:16.725329  4692 net.cpp:125] Top shape: 256 256 13 13 (11075584)
I0721 10:38:16.725338  4692 net.cpp:151] pool2 needs backward computation.
I0721 10:38:16.725358  4692 net.cpp:74] Creating Layer norm2
I0721 10:38:16.725368  4692 net.cpp:84] norm2 <- pool2
I0721 10:38:16.725378  4692 net.cpp:110] norm2 -> norm2
I0721 10:38:16.725389  4692 net.cpp:125] Top shape: 256 256 13 13 (11075584)
I0721 10:38:16.725399  4692 net.cpp:151] norm2 needs backward computation.
I0721 10:38:16.725409  4692 net.cpp:74] Creating Layer conv3
I0721 10:38:16.725419  4692 net.cpp:84] conv3 <- norm2
I0721 10:38:16.725427  4692 net.cpp:110] conv3 -> conv3
I0721 10:38:16.735193  4692 net.cpp:125] Top shape: 256 384 13 13 (16613376)
I0721 10:38:16.735213  4692 net.cpp:151] conv3 needs backward computation.
I0721 10:38:16.735224  4692 net.cpp:74] Creating Layer relu3
I0721 10:38:16.735234  4692 net.cpp:84] relu3 <- conv3
I0721 10:38:16.735242  4692 net.cpp:98] relu3 -> conv3 (in-place)
I0721 10:38:16.735250  4692 net.cpp:125] Top shape: 256 384 13 13 (16613376)
I0721 10:38:16.735258  4692 net.cpp:151] relu3 needs backward computation.
I0721 10:38:16.735302  4692 net.cpp:74] Creating Layer conv4
I0721 10:38:16.735312  4692 net.cpp:84] conv4 <- conv3
I0721 10:38:16.735321  4692 net.cpp:110] conv4 -> conv4
I0721 10:38:16.743952  4692 net.cpp:125] Top shape: 256 384 13 13 (16613376)
I0721 10:38:16.743988  4692 net.cpp:151] conv4 needs backward computation.
I0721 10:38:16.744000  4692 net.cpp:74] Creating Layer relu4
I0721 10:38:16.744010  4692 net.cpp:84] relu4 <- conv4
I0721 10:38:16.744020  4692 net.cpp:98] relu4 -> conv4 (in-place)
I0721 10:38:16.744030  4692 net.cpp:125] Top shape: 256 384 13 13 (16613376)
I0721 10:38:16.744038  4692 net.cpp:151] relu4 needs backward computation.
I0721 10:38:16.744050  4692 net.cpp:74] Creating Layer conv5
I0721 10:38:16.744057  4692 net.cpp:84] conv5 <- conv4
I0721 10:38:16.744067  4692 net.cpp:110] conv5 -> conv5
I0721 10:38:16.748935  4692 net.cpp:125] Top shape: 256 256 13 13 (11075584)
I0721 10:38:16.748955  4692 net.cpp:151] conv5 needs backward computation.
I0721 10:38:16.748965  4692 net.cpp:74] Creating Layer relu5
I0721 10:38:16.748975  4692 net.cpp:84] relu5 <- conv5
I0721 10:38:16.748983  4692 net.cpp:98] relu5 -> conv5 (in-place)
I0721 10:38:16.748998  4692 net.cpp:125] Top shape: 256 256 13 13 (11075584)
I0721 10:38:16.749011  4692 net.cpp:151] relu5 needs backward computation.
I0721 10:38:16.749022  4692 net.cpp:74] Creating Layer pool5
I0721 10:38:16.749030  4692 net.cpp:84] pool5 <- conv5
I0721 10:38:16.749039  4692 net.cpp:110] pool5 -> pool5
I0721 10:38:16.749050  4692 net.cpp:125] Top shape: 256 256 6 6 (2359296)
I0721 10:38:16.749058  4692 net.cpp:151] pool5 needs backward computation.
I0721 10:38:16.749074  4692 net.cpp:74] Creating Layer fc6
I0721 10:38:16.749083  4692 net.cpp:84] fc6 <- pool5
I0721 10:38:16.749091  4692 net.cpp:110] fc6 -> fc6
I0721 10:38:17.160079  4692 net.cpp:125] Top shape: 256 4096 1 1 (1048576)
I0721 10:38:17.160148  4692 net.cpp:151] fc6 needs backward computation.
I0721 10:38:17.160166  4692 net.cpp:74] Creating Layer relu6
I0721 10:38:17.160177  4692 net.cpp:84] relu6 <- fc6
I0721 10:38:17.160190  4692 net.cpp:98] relu6 -> fc6 (in-place)
I0721 10:38:17.160202  4692 net.cpp:125] Top shape: 256 4096 1 1 (1048576)
I0721 10:38:17.160212  4692 net.cpp:151] relu6 needs backward computation.
I0721 10:38:17.160222  4692 net.cpp:74] Creating Layer drop6
I0721 10:38:17.160230  4692 net.cpp:84] drop6 <- fc6
I0721 10:38:17.160238  4692 net.cpp:98] drop6 -> fc6 (in-place)
I0721 10:38:17.160258  4692 net.cpp:125] Top shape: 256 4096 1 1 (1048576)
I0721 10:38:17.160265  4692 net.cpp:151] drop6 needs backward computation.
I0721 10:38:17.160277  4692 net.cpp:74] Creating Layer fc7
I0721 10:38:17.160286  4692 net.cpp:84] fc7 <- fc6
I0721 10:38:17.160295  4692 net.cpp:110] fc7 -> fc7
I0721 10:38:17.342094  4692 net.cpp:125] Top shape: 256 4096 1 1 (1048576)
I0721 10:38:17.342157  4692 net.cpp:151] fc7 needs backward computation.
I0721 10:38:17.342175  4692 net.cpp:74] Creating Layer relu7
I0721 10:38:17.342185  4692 net.cpp:84] relu7 <- fc7
I0721 10:38:17.342198  4692 net.cpp:98] relu7 -> fc7 (in-place)
I0721 10:38:17.342208  4692 net.cpp:125] Top shape: 256 4096 1 1 (1048576)
I0721 10:38:17.342217  4692 net.cpp:151] relu7 needs backward computation.
I0721 10:38:17.342228  4692 net.cpp:74] Creating Layer drop7
I0721 10:38:17.342236  4692 net.cpp:84] drop7 <- fc7
I0721 10:38:17.342245  4692 net.cpp:98] drop7 -> fc7 (in-place)
I0721 10:38:17.342254  4692 net.cpp:125] Top shape: 256 4096 1 1 (1048576)
I0721 10:38:17.342262  4692 net.cpp:151] drop7 needs backward computation.
I0721 10:38:17.342274  4692 net.cpp:74] Creating Layer fc8
I0721 10:38:17.342283  4692 net.cpp:84] fc8 <- fc7
I0721 10:38:17.342291  4692 net.cpp:110] fc8 -> fc8
I0721 10:38:17.343199  4692 net.cpp:125] Top shape: 256 22 1 1 (5632)
I0721 10:38:17.343214  4692 net.cpp:151] fc8 needs backward computation.
I0721 10:38:17.343231  4692 net.cpp:74] Creating Layer loss
I0721 10:38:17.343240  4692 net.cpp:84] loss <- fc8
I0721 10:38:17.343250  4692 net.cpp:84] loss <- label
I0721 10:38:17.343264  4692 net.cpp:151] loss needs backward computation.
I0721 10:38:17.343305  4692 net.cpp:173] Collecting Learning Rate and Weight Decay.
I0721 10:38:17.343327  4692 net.cpp:166] Network initialization done.
I0721 10:38:17.343335  4692 net.cpp:167] Memory required for Data 1073760256
```

















### CIFAR-10在caffe上进行训练与学习

#### 使用数据库：CIFAR-10


60000张 32X32 彩色图像 10类，50000张训练，10000张测试

![](http://mmbiz.qpic.cn/mmbiz/6PqHI4vHibfXAJRCMd5gSDv0g0P7nGEp4q57p6gPhYicEUkyQdt3v6Tf2EGISTNhuCZJc5MF1Z4KwCwCCNMLxWbA/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

#### 准备


在终端运行以下指令：

```
cd $CAFFE_ROOT/data/cifar10
./get_cifar10.shcd $CAFFE_ROOT/examples/cifar10
./create_cifar10.sh
```


其中CAFFE_ROOT是caffe-master在你机子的地址


运行之后，将会在examples中出现数据库文件./cifar10-leveldb和数据库图像均值二进制文件./mean.binaryproto

![](http://mmbiz.qpic.cn/mmbiz/6PqHI4vHibfXAJRCMd5gSDv0g0P7nGEp4BDRAzsDm0KUV2vFDWZbXrwzKCOy7O3yx3mwYNeGWxT1cydiaeic4wjZw/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

#### 模型


该CNN由卷积层，POOLing层，非线性变换层，在顶端的局部对比归一化线性分类器组成。该模型的定义在CAFFE_ROOT/examples/cifar10 directory’s cifar10_quick_train.prototxt中，可以进行修改。其实后缀为prototxt很多都是用来修改配置的。

![](http://mmbiz.qpic.cn/mmbiz/6PqHI4vHibfXAJRCMd5gSDv0g0P7nGEp4ypfz2L1C0Mtz7cSEGKicYwb40DvaFhHibXLooEvIeSYCvqUEqHKvPz1A/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

#### 训练和测试


训练这个模型非常简单，当我们写好参数设置的文件cifar10_quick_solver.prototxt和定义的文件cifar10_quick_train.prototxt和cifar10_quick_test.prototxt后，运行train_quick.sh或者在终端输入下面的命令：

```
cd $CAFFE_ROOT/examples/cifar10
./train_quick.sh
```


即可，train_quick.sh是一个简单的脚本，会把执行的信息显示出来，培训的工具是train_net.bin,cifar10_quick_solver.prototxt作为参数。


然后出现类似以下的信息：这是搭建模型的相关信息

```
I0317 21:52:48.945710 2008298256 net.cpp:74] Creating Layer conv1
I0317 21:52:48.945716 2008298256 net.cpp:84] conv1 <- data
I0317 21:52:48.945725 2008298256 net.cpp:110] conv1 -> conv1
I0317 21:52:49.298691 2008298256 net.cpp:125] Top shape: 100 32 32 32 (3276800)
I0317 21:52:49.298719 2008298256 net.cpp:151] conv1 needs backward computation.
```


接着：

```
0317 21:52:49.309370 2008298256 net.cpp:166] Network initialization done.
I0317 21:52:49.309376 2008298256 net.cpp:167] Memory required for Data 23790808
I0317 21:52:49.309422 2008298256 solver.cpp:36] Solver scaffolding done.
I0317 21:52:49.309447 2008298256 solver.cpp:47] Solving CIFAR10_quick_train
```


之后，训练开始

```
I0317 21:53:12.179772 2008298256 solver.cpp:208] Iteration 100, lr = 0.001
I0317 21:53:12.185698 2008298256 solver.cpp:65] Iteration 100, loss = 1.73643
...
I0317 21:54:41.150030 2008298256 solver.cpp:87] Iteration 500, Testing net
I0317 21:54:47.129461 2008298256 solver.cpp:114] Test score #0: 0.5504
I0317 21:54:47.129500 2008298256 solver.cpp:114] Test score #1: 1.27805
```


其中每100次迭代次数显示一次训练时lr(learningrate),和loss（训练损失函数），每500次测试一次，输出score 0（准确率）和score 1（测试损失函数）


当5000次迭代之后，正确率约为75%，模型的参数存储在二进制protobuf格式在cifar10_quick_iter_5000


然后，这个模型就可以用来运行在新数据上了。

#### 其他


另外，更改cifar*solver.prototxt文件可以使用CPU训练，
`# solver mode: CPU or GPUsolver_mode: CPU`

可以看看CPU和GPU训练的差别




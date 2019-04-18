# 深度学习caffe框架(1):如何快速上手caffe? - happyhorizon的算法天空 - CSDN博客
2017年09月10日 00:49:52[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：857

- [初识caffe](#初识caffe)- [安装caffe](#安装caffe)
- [跑一个例子mnist](#跑一个例子mnist)
- [配置caffe框架的深度学习网络结构](#配置caffe框架的深度学习网络结构)
- [输入数据](#输入数据)- [数据层的定义](#数据层的定义)
- [图片数据如何保存为lmdb格式](#图片数据如何保存为lmdb格式)
- [模型的保存和读取](#模型的保存和读取)
- [caffe的代码层次](#caffe的代码层次)
- [参考](#参考)
# 初识caffe
## 安装caffe
## 跑一个例子:mnist
在Caffe安装目录之下，首先获得MNIST数据集：
```
cd data/mnist
sh get_mnist.sh
```
生成mnist-train-leveldb/ 和 mnist-test-leveldb/，把数据转化成leveldb格式：
```bash
cd examples/lenet 
sh create_mnist.sh
```
训练网络:
`sh train_lenet.sh`
看看train_lenet.sh
```bash
#!/usr/bin/env sh
set -e
./build/tools/caffe train --solver=examples/mnist/lenet_solver.prototxt $@
```
脚本在文件开头加上set -e,这句语句告诉bash如果任何语句的执行结果不是true则应该退出。这样的好处是防止错误像滚雪球般变大导致一个致命的错误，而这些错误本应该在之前就被处理掉。如果要增加可读性，可以使用set -o errexit，它的作用与set -e相同。
## 配置caffe框架的深度学习网络结构
后缀为.prototxt 的文件是caffe配置文件. 
Caffe非常强调网络的层次结构,也就是说有的操作都是一个layer.layer还有很多种类型，比如Data,Convolution,Pooling等，层之间的数据流动是以Blobs的方式进行 
数据层是每个模型的最底层，是模型的入口，不仅提供数据的输入，也提供数据从Blobs转换成别的格式进行保存输出。通常数据的预处理（如减去均值, 放大缩小, 裁剪和镜像等），也在这一层设置参数实现。 
下面来看看如何定义数据层及输入数据.
## 输入数据
Caffe的输入数据可以是数据库lmdb或者LevelDB,也可以是内存数据, HDF5, 甚至是图片数据(将图片名称和label按照固定格式写入txt, 这种方式效率比较低)
### 数据层的定义
下面的例子给出了所有数据层都具有的公用参数:
```
layer {
  name: "cifar"
  type: "Data"
  top: "data"
  top: "label"
  include {
    phase: TRAIN
  }
  transform_param {
    mean_file: "examples/cifar10/mean.binaryproto"
  }
  data_param {
    source: "examples/cifar10/cifar10_train_lmdb"
    batch_size: 100
    backend: LMDB
  }
}
```
- 
name: 表示该层的名称，可随意取
- 
type: 层类型，如果是Data，表示数据来源于LevelDB或LMDB。根据数据的来源不同，数据层的类型也不同（后面会详细阐述）。一般在练习的时候，我们都是采 用的LevelDB或LMDB数据，因此层类型设置为Data。
- 
top或bottom: 每一层用bottom来输入数据，用top来输出数据。如果只有top没有bottom，则此层只有输出，没有输入。反之亦然。如果有多个 top或多个bottom，表示有多个blobs数据的输入和输出。
- 
data 与 label: 在数据层中，至少有一个命名为data的top。如果有第二个top，一般命名为label。 这种(data,label)配对是分类模型所必需的.
- 
include: 一般训练的时候和测试的时候，模型的层是不一样的。该层（layer）是属于训练阶段的层，还是属于测试阶段的层，需要用include来指定。如果没有include参数，则表示该层既在训练模型中，又在测试模型中。
- 
Transformations: 数据的预处理，可以将数据变换到定义的范围内。如设置scale为0.00390625，实际上就是1/255, 即将输入数据由0-255归一化到0-1之间. 这里定义了数据的均值文件.mean_file: “examples/cifar10/mean.binaryproto”.这是由于图片减去均值后，再进行训练和测试，会提高速度和精度。因此，一般在各种模型中都会有这个操作。
其他格式的数据层具体定义及相应参数参见:[http://www.cnblogs.com/denny402/p/5070928.html](http://www.cnblogs.com/denny402/p/5070928.html)
如何生成均值文件binaryproto: [http://www.cnblogs.com/denny402/p/5102328.html](http://www.cnblogs.com/denny402/p/5102328.html)
### 图片数据如何保存为lmdb格式?
在caffe中，作者为我们提供了这样一个文件：convert_imageset.cpp，存放在根目录下的tools文件夹下。编译之后，生成对应的可执行文件放在 buile/tools/ 下面，这个文件的作用就是将图片文件转换成caffe框架中能直接使用的db文件。 
在linux终端, 就可以输入下面命令实现数据格式的转换
`convert_imageset [FLAGS] ROOTFOLDER/ LISTFILE DB_NAME`
这里需要带四个参数： 
 - FLAGS: 图片参数组，后面详细介绍 
 - ROOTFOLDER/: 图片存放的绝对路径，从linux系统根目录开始 
 - LISTFILE: 图片文件列表清单，一般为一个txt文件，一行一张图片 
 - DB_NAME: 最终生成的db文件存放目录
首先看看FLAGS都有哪些具体设置: 
-gray: 是否以灰度图的方式打开图片。程序调用opencv库中的imread()函数来打开图片，默认为false 
-shuffle: 是否随机打乱图片顺序。默认为false 
-backend:需要转换成的db文件格式，可选为leveldb或lmdb,默认为lmdb 
-resize_width/resize_height: 改变图片的大小。在运行中，要求所有图片的尺寸一致，因此需要改变图片大小。 程序调用opencv库的resize（）函数来对图片放大缩小，默认为0，不改变 
-check_size: 检查所有的数据是否有相同的尺寸。默认为false,不检查 
-encoded: 是否将原图片编码放入最终的数据中，默认为false 
-encode_type: 与前一个参数对应,将图片编码为: ‘png’,’jpg’…… 
例如:
```
DATA=examples/images
build/tools/convert_imageset --shuffle \--resize_height=256 --resize_width=256 \/home/xxx/caffe/examples/images/ $DATA/train.txt  $DATA/img_train_lmdb
```
–shuffle –resize_height=256 –resize_width=256 是图片参数组设置,这里设置参数-shuffle,打乱图片顺序。设置参数-resize_height和-resize_width将所有图片尺寸都变为256*256. 
/home/xxx/caffe/examples/images/就是图片的ROOT_FOLDER, $DATA/train.txt 图片文件列表清单
## 模型的保存和读取
Caffe训练好的模型可以保存在后缀为.caffemodel的文件里.这个文件是二进制文件,在读取的时候将二进制文件(caffemodel)转化成文件流，再放入proto里面。可以用caffe的代码打开,详见:[http://blog.csdn.net/jiongnima/article/details/72904526](http://blog.csdn.net/jiongnima/article/details/72904526).
模型的加载也很简单, python代码如下:
```python
import caffeimport numpy as np
root='/home/dl/'   #根目录
deploy=root + 'mnist/deploy.prototxt'    #deploy文件
caffe_model=root + 'mnist/lenet_iter_9380.caffemodel'   #训练好的 caffemodel
```
# caffe的代码层次
- Blob：是基础的数据结构，是用来保存学习到的参数以及网络传输过程中产生数据的类。
- Layer：是网络的基本单元，由此派生出了各种层类。修改这部分的人主要是研究特征表达方向的。
- Net：是网络的搭建，将Layer所派生出层类组合成网络。Solver：是Net的求解，修改这部分人主要会是研究DL求解方向的。
# 参考
[https://www.zhihu.com/question/27982282/answer/39350629](https://www.zhihu.com/question/27982282/answer/39350629)
一个不错的caffe学习笔记博客: 
[http://www.cnblogs.com/denny402/tag/caffe/default.html?page=2](http://www.cnblogs.com/denny402/tag/caffe/default.html?page=2)
图片文件转lmdb数据库: 
[http://www.cnblogs.com/denny402/p/5082341.html](http://www.cnblogs.com/denny402/p/5082341.html)

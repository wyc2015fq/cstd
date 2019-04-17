# Caffe学习（二）Mnist手写数字识别测试与分析 - BryantLJ学习的地方 - CSDN博客





2016年08月04日 08:46:49[遍地流金](https://me.csdn.net/u012177034)阅读数：2260








学习caffe，从弄清楚第一个例程开始～～～


首先进入caffe安装的根目录，cd CAFFE_ROOT，以下所有操作都是默认在该目录下。

**1. 获取mnist数据集**


`bryant@bryant:~/caffe$ ./data/mnist/get_mnist.sh`




下载以后即可在data/mnist文件夹下得到四组数据，分别是训练集的图像和标签，测试集的图像和标签




**2.将mnist数据集转换为lmdb格式**


`bryant@bryant:~/caffe$ ./examples/mnist/create_mnist.sh`此脚本会调用 build/examples/mnist/convert_mnist_data.bin将原始数据转换为lmdb数据，转换后的数据存放在examples/mnist/文件夹下，文件有两个，一个为mnist_train_lmdb，一个为mnist_test_lmdb。



convert_mnist_data.bin由convert_mnist_data.cpp文件编译而成，该篇博客[点击打开链接](http://blog.csdn.net/whiteinblue/article/details/45330801)仔细解读了如何实现这个转换。




**3.训练得到模型.caffemodel与.solverstate**





./build/tools/caffe train --solver=examples/mnist/lenet_solver.prototxt


slover是对于该数据集的一个解法，包括网络结构，各种训练参数，训练数据等，其内容如下






```
# 网络结构
net: "examples/mnist/lenet_train_test.prototxt"
# 此时validation总样本数test_iter*batch_size
test_iter: 100
# 每500次训练迭代进行依次validation迭代
test_interval: 500
#初始学习率，基学习率
base_lr: 0.01
momentum: 0.9
weight_decay: 0.0005
# 学习率变化策略
lr_policy: "inv"
gamma: 0.0001
power: 0.75
# 每100次迭代显示一次loss等参数，包括训练与验证
display: 100
# 最多迭代次数
max_iter: 10000
# 每5000次迭代进行一次模型存储，防止意外中断
snapshot: 5000
snapshot_prefix: "examples/mnist/lenet"
# 训练和测试模式
solver_mode: GPU
```




网络结构定义lenet_train_test.prototxt及详细说明：



```
#网络名称
name: "LeNet"
#train数据层
#输入源：mnist_train_ldmb，batch_size：64
#输出：data blob，label blob
#数据变换：scale归一化，0.00390625=1/255
layer {
  name: "mnist"
  type: "Data"
  top: "data"
  top: "label"
  include {
    phase: TRAIN
  }
  transform_param {
    scale: 0.00390625
  }
  data_param {
    source: "examples/mnist/mnist_train_lmdb"
    batch_size: 64
    backend: LMDB
  }
}
#test数据层
#输入源：mnist_test_ldmb，batch_size：100
#输出：data blob，label blob
#数据变换：scale归一化
layer {
  name: "mnist"
  type: "Data"
  top: "data"
  top: "label"
  include {
    phase: TEST
  }
  transform_param {
    scale: 0.00390625
  }
  data_param {
    source: "examples/mnist/mnist_test_lmdb"
    batch_size: 100
    backend: LMDB
  }
}
#卷积层conv1
#输入数据：data blob
#输出数据:conv1 blob
#卷积层参数：20个5*5的特征卷积核，步长为1，卷积核的权重初始化方式为xavier，偏置的初始化方式为constant，常量默认为0
#该层学习率：权重学习率为基学习率base_lr的1倍，偏置学习率为base_lr的两倍
layer {
  name: "conv1"
  type: "Convolution"
  bottom: "data"
  top: "conv1"
  param {
    lr_mult: 1
  }
  param {
    lr_mult: 2
  }
  convolution_param {
    num_output: 20
    kernel_size: 5
    stride: 1
    weight_filler {
      type: "xavier"
    }
    bias_filler {
      type: "constant"
    }
  }
}
#池化层pool1
#输入数据：conv1 blob
#输出数据：pool1 blob
#池化方式及参数：Max pool，2*2的池化核，步长为2
layer {
  name: "pool1"
  type: "Pooling"
  bottom: "conv1"
  top: "pool1"
  pooling_param {
    pool: MAX
    kernel_size: 2
    stride: 2
  }
}

layer {
  name: "conv2"
  type: "Convolution"
  bottom: "pool1"
  top: "conv2"
  param {
    lr_mult: 1
  }
  param {
    lr_mult: 2
  }
  convolution_param {
    num_output: 50
    kernel_size: 5
    stride: 1
    weight_filler {
      type: "xavier"
    }
    bias_filler {
      type: "constant"
    }
  }
}
layer {
  name: "pool2"
  type: "Pooling"
  bottom: "conv2"
  top: "pool2"
  pooling_param {
    pool: MAX
    kernel_size: 2
    stride: 2
  }
}

#全连接层ip1
#输入数据：pool2 blob
#输出数据：ip1 blob
#全连接层参数：500个节点，权值初始化方式为xavier，偏置初始化方式为constant，默认为0
#学习率：权重学习率为base_lr，偏置学习率为base_lr*2

 layer {
  name: "ip1"
  type: "InnerProduct"
  bottom: "pool2"
  top: "ip1"
  param {
    lr_mult: 1
  }
  param {
    lr_mult: 2
  }
  inner_product_param {
    num_output: 500
    weight_filler {
      type: "xavier"
    }
    bias_filler {
      type: "constant"
    }
  }
}
#非线性激活层relu1
#输入数据：ip1 blob
#输出数据：ip1 blob（注意仍然是ip1，由于relu是对每个点操作，输出也是对应个点的值，这样做便于省内存）
layer {
  name: "relu1"
  type: "ReLU"
  bottom: "ip1"
  top: "ip1"
}
#全连接层ip2
#输入数据：ip1 blob
#输出数据：ip2 blob（此网络输出也即用于最终预测的输出）
layer {
  name: "ip2"
  type: "InnerProduct"
  bottom: "ip1"
  top: "ip2"
  param {
    lr_mult: 1
  }
  param {
    lr_mult: 2
  }
  inner_product_param {
    num_output: 10
    weight_filler {
      type: "xavier"
    }
    bias_filler {
      type: "constant"
    }
  }
}
#精确度计算层accuracy，只在test阶段有用
#输入数据：ip2 blob，label accuracy
#输出数据：accuracy
layer {
  name: "accuracy"
  type: "Accuracy"
  bottom: "ip2"
  bottom: "label"
  top: "accuracy"
  include {
    phase: TEST
  }
}
# 损失函数层：SoftmaxWithLoss类型的loss
# 数据输入：ip2 blob， label blob
# 数据输出：loss blob，最终损失
layer {
  name: "loss"
  type: "SoftmaxWithLoss"
  bottom: "ip2"
  bottom: "label"
  top: "loss"
}
```




网络结构规则的定义及解释在src/caffe/proto/caffe.proto里有更详细的说明
测试完成会产生四个文件


lenet_iter5000.caffemodel,lenet_iter5000.solverstate

lenet_iter10000.caffemodel,lenet_iter10000.solverstate

用户后面就可以根据这个模型来对新的数据进行预测





为什么这里只有两个数据集，而不是train，validation，test？？？

Caffe里的train，test，deploy相当于传统意义上的train，validation，test。若要进行传统意义上的test时，需要自己在训练师自己分一部分数据来做deploy数据集。





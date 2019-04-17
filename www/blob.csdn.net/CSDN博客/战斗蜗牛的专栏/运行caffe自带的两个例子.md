# 运行caffe自带的两个例子 - 战斗蜗牛的专栏 - CSDN博客





2016年08月04日 18:20:15[vbskj](https://me.csdn.net/vbskj)阅读数：1650









注意！！！：由于编译、运行脚本写的都是相对于caffe的安装根目录，所以所有指令在caffe根目录下执行！！





为了程序的简洁，在caffe中是不带练习数据的，因此需要自己去下载。但在caffe根目录下的data文件夹里，作者已经为我们编写好了下载数据的脚本文件，我们只需要联网，运行这些脚本文件就行了。


注意：在caffe中运行所有程序，都必须在根目录下进行，否则会出错

1、mnist实例


mnist是一个手写数字库，由DL大牛Yan LeCun进行维护。mnist最初用于支票上的手写数字识别, 现在成了DL的入门练习库。征对mnist识别的专门模型是Lenet，算是最早的cnn模型了。


mnist数据训练样本为60000张，测试样本为10000张，每个样本为28*28大小的黑白图片，手写数字为0-9，因此分为10类。


首先下载mnist数据，假设当前路径为caffe根目录

```
# sudo sh data/mnist/get_mnist.sh
```


运行成功后，在 data/mnist/目录下有四个文件：

train-images-idx3-ubyte:  训练集样本 (9912422 bytes)
train-labels-idx1-ubyte:  训练集对应标注 (28881 bytes)
t10k-images-idx3-ubyte:   测试集图片 (1648877 bytes)
t10k-labels-idx1-ubyte:   测试集对应标注 (4542 bytes)


这些数据不能在caffe中直接使用，需要转换成LMDB数据

```
# sudo sh examples/mnist/create_mnist.sh
```


如果想运行leveldb数据，请运行 examples/siamese/ 文件夹下面的程序。 examples/mnist/ 文件夹是运行lmdb数据


转换成功后，会在 examples/mnist/目录下，生成两个文件夹，分别是mnist_train_lmdb和mnist_test_lmdb，里面存放的data.mdb和lock.mdb，就是我们需要的运行数据。


接下来是修改配置文件，如果你有GPU且已经完全安装好，这一步可以省略，如果没有，则需要修改solver配置文件。


需要的配置文件有两个，一个是lenet_solver.prototxt，另一个是train_lenet.prototxt.


首先打开lenet_solver_prototxt

```
# sudo vi examples/mnist/lenet_solver.prototxt
```


根据需要，在max_iter处设置最大迭代次数，以及决定最后一行solver_mode,是否要改成CPU


保存退出后，就可以运行这个例子了

```
# sudo time sh examples/mnist/train_lenet.sh
```


CPU运行时候大约13分钟，GPU运行时间大约4分钟，GPU+cudnn运行时候大约40秒，精度都为99%左右

2、cifar10实例


cifar10数据训练样本50000张，测试样本10000张，每张为32*32的彩色三通道图片，共分为10类。


下载数据：

```
# sudo sh data/cifar10/get_cifar10.sh
```


运行成功后，会在 data/cifar10/文件夹下生成一堆bin文件


转换数据格式为lmdb：

```
# sudo sh examples/cifar10/create_cifar10.sh
```


转换成功后，会在 examples/cifar10/文件夹下生成两个文件夹，cifar10_train_lmdb和cifar10_test_lmdb, 里面的文件就是我们需要的文件。


为了节省时间，我们进行快速训练（train_quick)，训练分为两个阶段，第一个阶段（迭代4000次）调用配置文件cifar10_quick_solver.prototxt, 学习率（base_lr)为0.001


第二阶段（迭代1000次）调用配置文件cifar10_quick_solver_lr1.prototxt, 学习率(base_lr)为0.0001


前后两个配置文件就是学习率(base_lr)和最大迭代次数（max_iter)不一样，其它都是一样。如果你对配置文件比较熟悉以后，实际上是可以将两个配置文件合二为一的，设置lr_policy为multistep就可以了。

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

base_lr: 0.001
momentum: 0.9
weight_decay: 0.004
lr_policy: "multistep"
gamma: 0.1
stepvalue: 4000
stepvalue: 5000

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


运行例子：

```
# sudo time sh examples/cifar10/train_quick.sh
```


GPU+cudnn大约45秒左右，精度75%左右。




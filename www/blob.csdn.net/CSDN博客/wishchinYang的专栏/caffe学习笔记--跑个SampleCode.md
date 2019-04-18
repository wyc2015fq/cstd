# caffe学习笔记--跑个SampleCode - wishchinYang的专栏 - CSDN博客
2015年03月31日 16:48:34[wishchin](https://me.csdn.net/wishchin)阅读数：1849
Caffe默认情况会安装在
home/username/caffe-master，
所以下面的工作，默认已经切换到了该工作目录。下面的工作主要是，用于测试Caffe是否工作正常，不做详细评估。具体设置请参考官网 ：
[http://caffe.berkeleyvision.org/gathered/examples/mnist.html](http://caffe.berkeleyvision.org/gathered/examples/mnist.html)
**1. 数据预处理**
可以用下载好的数据集，也可以重新下载，我网速快，这里就偷懒直接下载了，具体操作如下：
```cpp
$ cd data/mnist
$ sudo sh ./get_mnist.sh
```
**2. 重建LDB文件**，就是处理二进制数据集为Caffe识别的数据集，以后所有的数据，包括jpe文件都要处理成这个格式
```cpp
$ cd examples/mnist
$ sudo sh ./create_mnist.sh   （sudo sh examples/mnist/create_mnist.sh）
```
生成mnist-train-leveldb/ 和 mnist-test-leveldb/文件夹，这里包含了LDB格式的数据集
PS: 这里可能会遇到一个报错信息：
```cpp
Creating lmdb...
./create_mnist.sh: 16: ./create_mnist.sh: build/examples/mnist/convert_mnist_data.bin: not found
```
解决方法是，直接到Caffe-master的根目录执行，实际上新版的Caffe，基本上都得从根目录执行。
```cpp
~/caffe-master$ sudo sh examples/mnist/create_mnist.sh
```
**3. 训练mnist**
```cpp
$ sudo sh examples/mnist/train_lenet.sh
```
至此，Caffe安装的所有步骤完结，下面是一组简单的数据对比，实验来源于MNIST数据集，主要是考察一下不同系统下CPU和GPU的性能。可以看到明显的差别了，虽然MNIST数据集很简单，相信复杂得数据集，差别会更大，Ubuntu+GPU是唯一的选择了。
测试平台：i7-4770K/16G/GTX 770/CUDA 6.5
MNIST Windows8.1 on CPU：620s
MNIST Windows8.1 on GPU：190s
MNIST Ubuntu 14.04 on CPU：270s
MNIST Ubuntu 14.04 on GPU：160s
MNIST Ubuntu 14.04 on GPU with cuDNN：35s
Cifar10_full on GPU wihtout cuDNN：73m45s = 4428s　*（Iteration 70000）*
Cifar10_full on GPU with cuDNN：20m7s = 1207s　*（Iteration 70000）*

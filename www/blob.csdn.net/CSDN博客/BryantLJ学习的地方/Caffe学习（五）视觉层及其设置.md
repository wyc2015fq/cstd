# Caffe学习（五）视觉层及其设置 - BryantLJ学习的地方 - CSDN博客





2016年08月07日 11:44:23[遍地流金](https://me.csdn.net/u012177034)阅读数：1512








Caffe源码里自带的VisionLayer有三种：

（1）ConvlutionLayer

（2）PoolingLayer

（3）LocalResponseNormalizationLayer

前两种在（四）的实例中有过介绍，这里主要讲述第三种LRN，来源于2012AlexNet的局部归一化层。AlexNet认为，LRN层模仿生物神经系统的侧抑制机制，**对局部神经元的活动创建竞争机制，使得响应比较大的值相对更大，提高模型泛化能力**。

不过这个层有的说有用，有的说无用，这里就先大致了解一下，具体在系统学习。

**不过最新的GoogleNet以及ResNet好像都没有再使用这种Layer了，可能已经被抛弃。。。**

LRN的具体做法如下


![](https://img-blog.csdn.net/20160807162118264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其中，![](http://latex.codecogs.com/gif.latex?a_%7Bx%2Cy%7D%5E%7Bi%7D)代表该层的输入数据，表示第i个kernel作用在（x，y）上得到的结果，也即LRN层的输入数据，![](http://latex.codecogs.com/gif.latex?b_%7Bx%2Cy%7D%5E%7Bi%7D)代表LRN的输出数据

N表示输入的数据层数，也即上一卷积层或池化层Kernel的个数，n表示可调的用于局部归一化的相邻层的个数

可调的超参数有![](http://latex.codecogs.com/gif.latex?k%2Cn%2C%5Calpha%20%2C%5Cbeta)，常设值为2,5,0.0001,0.75

该层的参数设置实例：




```
layers {
  name: "norm1"
  type: “LRN”
  bottom: "pool1"
  top: "norm1"
  lrn_param {
    local_size: 5
    alpha: 0.0001
    beta: 0.75
  }
}
```
其中local_size表示n，alpha与beta分别表示另外两个参数![](http://latex.codecogs.com/gif.latex?%5Calpha%20%2C%5Cbeta)










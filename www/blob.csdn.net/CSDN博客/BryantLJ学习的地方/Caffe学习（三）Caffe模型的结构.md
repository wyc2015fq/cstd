# Caffe学习（三）Caffe模型的结构 - BryantLJ学习的地方 - CSDN博客





2016年08月05日 19:42:26[遍地流金](https://me.csdn.net/u012177034)阅读数：1731








**一总体结构**

在caffe中，解决一个问题首先应该定义一个slover，反应到mnist例程中也就是lenet_solver.prototxt。该slover主要包括两部分，（1）为网络模型model，（2）为该模型参数的具体optimization方法及参数。model主要由各种layer组成，主要包括数据相关的DataLayer，图像滤波变换相关的VisionLayer，非线性激活函数ActivationLayer，各种损失函数LossLayer，以及其他常用Layer等。每一个layer保存的数据和layer之间互相传递的数据在caffe里都是以封装为blob格式。


**二.blob数据单元**

blob的内容

![](https://img-blog.csdn.net/20160805214735954?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中blob里的数据有两种，一个为权值data，另一个为更新权值会用到的梯度diff，这两种数据都是要么存放在在CPU RAM里，要么存放在GPU RAM里


数据在blob里以<SyncMemory>类型的指针形式存放，该类型隐藏了cpu和gpu内存之间数据复制和传递同步的细节，一般情况下上层应用不用考虑。

在<SyncMemory>类里，提供了两种取数据的方法，一种是容许使用的时候对数据发生更改，一种不容许。



```
const Dtype* cpu_data() const;
Dtype* mutable_cpu_data();
```


为了提高运算效率，blob的数据会选择性的在cpu和gpu之间复制，下面为实例





```
// Assuming that data are on the CPU initially, and we have a blob.
const Dtype* foo;
Dtype* bar;
foo = blob.gpu_data(); // data copied cpu->gpu.
foo = blob.cpu_data(); // no data copied since both have up-to-date contents.
bar = blob.mutable_gpu_data(); // no data copied.
// ... some operations ...
bar = blob.mutable_gpu_data(); // no data copied when we are still on GPU.
foo = blob.cpu_data(); // data copied gpu->cpu, since the gpu side has modified the data
foo = blob.gpu_data(); // no data copied since both have up-to-date contents
bar = blob.mutable_cpu_data(); // still no data copied.
bar = blob.mutable_gpu_data(); // data copied cpu->gpu.
bar = blob.mutable_cpu_data(); // data copied gpu->cpu.
```
由例子可知，只有当用户数据的Processor变化（由cpu到gpu或者gpu到cpu），且上一次使用数据的形式为mutable时才会发生copy。




**三.Layer**

在定义每一层时都定义了三种运算：setup，forward，backward

setup：初始化层和各连接权值

forward：该层的前向传递计算

backward：该层的后向传播

其中每一层的计算都可以以两种方式实现，cpu模式和gpu模式。一般是由cpu读入数据，然后传入gpu，运算完毕以后在由cpu写入到磁盘

当我们根据自己的应用要新添层时，主要就是在caffe.proto文件里添加新定义，并在cpp里写出该层的前向和后向计算函数，以及权值初始化函数




前向后向传播，以下图所示的网络为例，


前向传播即计算:![](http://latex.codecogs.com/gif.latex?f_%7BW%7D%28X%29%3Dh%28g%28X%29%29)，其中![](http://latex.codecogs.com/gif.latex?g%28X%29%3DW%5E%7BT%7DX),即由X计算输出

梯度的后向传递：![](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20f_%7BW%7D%28X%29%7D%7BW%7D%3D%5Cfrac%7Bh%28g_%7BW%7D%28X%29%29%7D%7B%5Cpartial%20W%7D%3D%5Cfrac%7B%5Cpartial%20h%7D%7B%5Cpartial%20g%7D*%5Cfrac%7B%5Cpartial%20g%7D%7B%5Cpartial%20W%7D)，先计算后面一层的梯度![](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20h%7D%7B%5Cpartial%20g%7D)，再由链式法则计算前一层的权值梯度![](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20h%7D%7B%5Cpartial%20W%7D)





![](https://img-blog.csdn.net/20160806082405390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**四.网络Net**

网络都是由层和层的链接关系组成。有点需要注意

网络的结构不一定需要是线性的，可以是任意的DAG（有向无环图）



****![](https://img-blog.csdn.net/20160806083143871?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)****

**五.Optimizing Solver优化算子**

caffe提供的优化算法有以下几种：SGD，AdaDelta，AdaptiveGradient，Adam，Nesterov，RMSProp等





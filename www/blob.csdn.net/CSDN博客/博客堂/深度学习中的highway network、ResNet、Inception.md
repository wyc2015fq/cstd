# 深度学习中的highway network、ResNet、Inception - 博客堂 - CSDN博客





2018年05月17日 10:37:13[最小森林](https://me.csdn.net/u012052268)阅读数：2572










- [概念](#概念)- [CNN演化](#cnn演化)
- [梯度消失](#梯度消失)

- [highway network](#highway-network)- [公式：](#公式)
- [意义、好处](#意义好处)

- [ResNet](#resnet)- [动机、目的](#动机目的)
- [模型](#模型)
- [作用：](#作用)

- [Inception](#inception)- [Inception v1模型](#inception-v1模型)
- [Inception V2-V3：](#inception-v2-v3)
- [Inception v4模型](#inception-v4模型)





不是特别清楚这三个流行且有用的模型（或者说层）的具体作用和使用方法。

# 概念

## CNN演化

先引入一张CNN结构演化图：  
![image](https://img-blog.csdn.net/20160904154907381)
2012年AlexNet做出历史突破以来，直到GoogLeNet出来之前，主流的网络结构突破大致是网络更深（层数），网络更宽（神经元数）。所以大家调侃深度学习为“深度调参”，但是纯粹的增大网络的缺点：
- 参数太多，容易过拟合，若训练数据集有限；
- 网络越大计算复杂度越大，难以应用；
- 网络越深，梯度越往后穿越容易消失（梯度弥散），难以优化模型

## 梯度消失

我们都知道神经网络中会使用非线性变换。

一般会使用sigmoid函数，得到![image](https://images2015.cnblogs.com/blog/637085/201608/637085-20160824154045933-459837099.png)，这个函数会把数据压缩到开区间(0，1)，函数的图像如下：

![image](https://images2015.cnblogs.com/blog/637085/201608/637085-20160824154316948-1510201191.png)

可以看到，函数的两侧非常平滑，而且无限的接近0和1，仅仅是中间部分函数接近一条直线。

要知道，神经网络训练的方法是BP算法（反向传播）。BP算法的基础其实就是导数的链式法则，就是有很多乘法会连接在一起。

看sigmoid函数的图像知道了，导数最大是1，而且大多数值都被推向两侧饱和的区域，这些区域的导数很小。

可以预见到，随着网络的加深，梯度后向传播到输入层时，就所剩无几，基本不能引起参数W数值的扰动，这样输入层一侧的网络就学习不到新的特征了参数得不到更新）。

那么怎么办？我暂时看到了四种解决问题的办法。 

1. 第一种很明显，可以通过使用别的激活函数； 

2. 第二种可以使用层归一化； 

3. 第三种是在权重的初始化上下功夫， 

4. 第四种是调整网络的结构。
我们主要关注第4个。

# highway network

Highway Network的灵感来自“解决RNN的问题，提出的LSTM结构” 也就是加入“门”结构。

Highway Network主要解决的问题是，网络深度加深，梯度信息回流受阻，造成网络训练困难的问题。

## 公式：

对于highway network来说，不需要看图片，看公式就可以理解其意义。
- 1.一般一个 feedforward neural network 有L层网络组成，每层网络对输入进行一个非线性映射变换，可以表达如下

![image](https://img-blog.csdn.net/20151218085927287)
- 对于高速CNN网络，我们定义一层网络如下 ，其中T和C分别表示 对输入的门控（0到1） 和 直接传送。 

![image](https://img-blog.csdn.net/20151218090305212)
- 为了理解，我们观察到，对于特殊的门值T

![image](https://img-blog.csdn.net/20151218090408123)

也就是：当门为1的时候，全部输出原x，不用激活。

## 意义、好处
- 物理意义：假设所有的门t的均值为0.5的话，就是把所有的原始信息一半激活，一半不变直接输入下一层，保留了很多信息。
- 反向传播的时候，可以让更多的（梯度）信息直接回流到输入，而不需要经过一个非线性转化。

> 
参考：[https://www.cnblogs.com/jie-dcai/p/5803220.html](https://www.cnblogs.com/jie-dcai/p/5803220.html)


# ResNet

网上有传言 微软的深度残差学习是抄袭 Highway Networks，只是Highway Networks的一个特例。Highway Networks 的确是先发表的。

但不管怎么说，ResNet的名气确实更大，很多面试会问到。

## 动机、目的

ResNet最根本的动机就是所谓的“退化”问题。

但是模型的深度加深，学习能力增强，因此更深的模型不应当产生比它更浅的模型更高的错误率。

而退化就是当模型的层次加深时，错误率却提高了，如下图：

![image](https://img-blog.csdn.net/20170220200258020?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3NwYmE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

而这个“退化”问题产生的原因归结于优化难题，当模型变复杂时，SGD的优化变得更加困难，导致了模型达不到好的学习效果。

针对这个问题，作者提出了一个Residual的结构：

![image](https://img-blog.csdn.net/20170220201128938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3NwYmE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 模型

模型增加一个identity mapping（恒等映射），将原始所需要学的函数H(x)转换成F(x)+x，

而作者认为这两种表达的效果相同，但是优化的难度却并不相同，作者假设F(x)的优化 会比H(x)简单的多。

这一想法也是源于图像处理中的残差向量编码，通过一个reformulation，将一个问题分解成多个尺度直接的残差问题，能够很好的起到优化训练的效果。

这个Residual block通过shortcut connection实现，通过shortcut将这个block的输入和输出进行一个element-wise的加叠。

## 作用：
- 这个简单的加法并不会给网络增加额外的参数和计算量，同时却可以大大增加模型的训练速度、提高训练效果
- 并且当模型的层数加深时，这个简单的结构能够很好的解决退化问题。因为identity map是的梯度可以直接回流到了输入层

> 
参考：[https://blog.csdn.net/wspba/article/details/56019373](https://blog.csdn.net/wspba/article/details/56019373)


# Inception

那么解决深度学习 参数过多、和梯度消失问题，方法当然就是增加网络深度和宽度的同时减少参数，Inception就是在这样的情况下应运而生。

## Inception v1模型

目前很多工作证明，要想增强网络能力，可以：增加网络深度，增加网络宽度； 

但是为了减少过拟合，也要减少自由参数。

因此，就自然而然有了这个第一版的Inception网络结构——同一层里面，有卷积1* 1, 3* 3,5* 5 不同的卷积模板，他们可以在不同size的感受野做特征提取，也算的上是一种混合模型了。

因为Max Pooling本身也有特征提取的作用，而且和卷积不同，没有参数不会过拟合，也作为一个分支。

但是直接这样做，整个网络计算量会较大，且层次并没有变深，因此，在3*3和5*5卷积前面先做1*1的卷积，降低input的channel数量，这样既使得网络变深，同时计算量反而小了；（在每一个卷积之后都有ReLU）

![image](https://img-blog.csdn.net/20160904155917864)

第一张图是论文中提出的最原始的版本，所有的卷积核都在上一层的所有输出上来做，那5×5的卷积核所需的计算量就太大了，造成了特征图厚度很大。为了避免这一现象提出的inception具有如下结构，在3x3前，5x5前，max pooling后分别加上了1x1的卷积核起到了降低特征图厚度的作用，也就是Inception v1的网络结构：

![image](https://img-blog.csdn.net/20160904160721902)

## Inception V2-V3：

V2和V3版本比较接近，就不绝对区分了，具体可以看[3]。讲一讲其中的创新点：

首先，用两层堆叠的3*3代替了一层5*5，我们可以看到，这样做参数量少了，计算量少了，但是层数变深了，效果也变好了：

![image](https://img-blog.csdn.net/20170315221522135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGJpbndvcmxk/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Inception v4模型

v4研究了Inception模块结合Residual Connection能不能有改进？发现ResNet的结构可以极大地加速训练，同时性能也有提升，得到一个Inception-ResNet v2网络，同时还设计了一个更深更优化的Inception v4模型，能达到与Inception-ResNet v2相媲美的性能。 






# 系统学习深度学习（二十一）--GoogLeNetV4与Inception-ResNet V1,V2 - 工作笔记 - CSDN博客





2017年03月16日 15:01:38[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：17405








转载自: [http://www.jianshu.com/p/329d2c0cfca9](http://www.jianshu.com/p/329d2c0cfca9)

[v4] [Inception-v4, Inception-ResNet and the Impact of Residual Connections on Learning](http://arxiv.org/abs/1602.07261)，top5 error 3.08%



上面那篇论文，主要贡献如下：
- 1、在Inception v3的基础上发明了Inception v4，v4比v3更加复杂
- 2、结合ResNet与GoogLeNet，发明了Inception-ResNet-v1、Inception-ResNet-v2，其中Inception-ResNet-v2效果非常好，但相比ResNet，Inception-ResNet-v2的复杂度非常惊人，跟Inception v4差不多
- 3、加入了Residual Connections以后，网络的训练速度加快了
- 4、在网络复杂度相近的情况下，Inception-ResNet-v2略优于Inception-v4
- 5、Residual Connections貌似只能加速网络收敛，真正提高网络精度的是“**更大的网络规模**”

Google Research的Inception模型和Microsoft Research的Residual Net模型两大图像识别杀器结合效果如何？在这篇2月23日公布在arxiv上的文章“Inception-v4, Inception-ResNet and the Impact of Residual Connections on Learning”给出了实验上的结论。

在该论文中，姑且将ResNet的核心模块称为residual connections，实验结果表明，residual connections可以提高Inception网络的准确率，并且不会提高计算量。这似乎是理所当然的。采用3个带有residual connection的Inception模型和1个Inception v4模型，ImageNet上的top 5错误率已经可以刷到3.08%啦。也就是说，错误率2.x%的时代已经指日可待了。

关于深度卷积神经网络的发展以及在各种视觉任务上的优异表现就不用赘述了。在讨论related work中，本文认为residual connection并不是训练very deep network的必要条件，但可以显著的加快训练的速度。同时，本文回顾了Inception模型的发展历程，原始的Inception模型，也就是GoogLeNet被称为Inception-v1，加入batch normalization之后被称为Inception-v2，加入factorization的idea之后，改进为Inception-v3。

Google Research从DistBelief过渡到TensorFlow之后，不用再顾虑分布式训练时要做模型的分割。Inception-v4模型设计的可以更简洁，计算量也更小。具体的模型如下图：

![](http://upload-images.jianshu.io/upload_images/273173-f0ac43dc69cb6447.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

inception-v4 网络模型

其中的Inception-A模块是这样的：


![](http://upload-images.jianshu.io/upload_images/273173-beff691e5f0c7fa3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

Inception-A模块网络结构



接下来将介绍重头戏，Inception-ResNet，其中的一个典型模块是这样的：


![](http://upload-images.jianshu.io/upload_images/273173-6169b24c457f215a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

Inception-ResNet-A网络结构



其中的+就是residual connection了。通过20个类似的模块组合，Inception-ResNet构建如下：


![](http://upload-images.jianshu.io/upload_images/273173-2192fa5cf79a4880.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

Inception-ResNet模型



基于Inception-v3和Inception-v4，文中分别得到了Inception-ResNet-v1和Inception-ResNet-v2两个模型。另外，文中还提到当卷积核超过1000个的大网络训练时，将残差（residuals）缩小有助于训练的稳定性。这个做法同原始ResNet论文中的two-phase training的效果类似。

果然如winsty所讲，现在论文的公式越来越少了，网络框图开始越来越多了。本文行文并不足够规范，更像一个report。接下就是报各种很牛的实验结果了。


![](http://upload-images.jianshu.io/upload_images/273173-b85a5a4b6fba0c80.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

单个模型，144 crops，ILSVRC 2012 validation set上的结果




![](http://upload-images.jianshu.io/upload_images/273173-ef50ac2a5e3e067e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

模型组合，144 crops，ILSVRC 2012 validation set上的结果



总结来讲，Google Research团队在这篇report中介绍了新的Inception-v4网络，并结合最新的residual connection技术，将ImageNet上图像分类的结果刷到了3.08%了。

[1] Christian Szegedy,Sergey Ioffe,Vincent Vanhoucke. Inception-v4, Inception-ResNet and the Impact of Residual Connections on Learning. arXiv:1602.07261[cs.CV]




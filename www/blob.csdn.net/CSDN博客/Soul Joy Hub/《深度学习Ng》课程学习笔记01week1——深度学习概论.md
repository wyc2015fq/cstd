# 《深度学习Ng》课程学习笔记01week1——深度学习概论 - Soul Joy Hub - CSDN博客

2017年08月30日 17:11:12[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：564
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/77719187](http://blog.csdn.net/u011239443/article/details/77719187)

课程地址：[http://mooc.study.163.com/course/deeplearning_ai-2001281002#/info](http://mooc.study.163.com/course/deeplearning_ai-2001281002#/info)

# 什么是神经网络

如房屋价格预测问题：

![](http://upload-images.jianshu.io/upload_images/1621805-f82c4f9a4c5c2231.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

图中的圈圈代表神经元，神经元里面是ReLU激活函数 ： max（0，x）。

类似于这样的神经元，许许多多个，那就组成的神经网络：

![](http://upload-images.jianshu.io/upload_images/1621805-995a062b7e2d3ad5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

事实上，我们不需要给出中间特征，我们只要给出输入层（第一层）的特征，神经网络就能自动的帮我抽象出隐藏层（中间的层）的特征，最后得到输出层（最后一层）：

![](http://upload-images.jianshu.io/upload_images/1621805-510f7279663dbbad.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 用神经网络进行监督学习

## 监督学习

![](http://upload-images.jianshu.io/upload_images/1621805-ef9f94d417cd63a0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 神经网络的例子

![](http://upload-images.jianshu.io/upload_images/1621805-9315e3b24fa06d73.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 数据

![](http://upload-images.jianshu.io/upload_images/1621805-39c439b6fc2f3533.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 为什么深度学习会兴起？

## 数据规模促使深度学习的进步

随着数据量的不断增大，不同的传统机器学习方法的表现会越来越好，并趋于相同的上界。而神经网络的上界比传统的机器学习方法要高： 
![](http://upload-images.jianshu.io/upload_images/1621805-36f0de8d6afd961e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

深度学习兴起的三大因素：
- 数据
- 计算能力
- 算法

我们创新出算法后，来验证时，对于海量规模的数据，我们需要10分钟、1天甚至1个月的数据计算，才能得到算法的验证。所以，数据的计算能力是非常重要的！ 
![](http://upload-images.jianshu.io/upload_images/1621805-929e1a36f8ed7dee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](https://img-blog.csdn.net/20170830171904671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


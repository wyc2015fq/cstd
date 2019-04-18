# 《深度学习Ng》课程学习笔记02week1——深度学习的实用层面 - Soul Joy Hub - CSDN博客

2017年09月12日 16:50:08[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：542


[http://blog.csdn.net/u011239443/article/details/77947043](http://blog.csdn.net/u011239443/article/details/77947043)

# 1.1 训练 / 开发 / 测试集

![](http://upload-images.jianshu.io/upload_images/1621805-2730337a9b73483e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-d739c8047f2004ec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.2 偏差 / 方差

低偏差高方差，则表示泛化能力不强，可能过拟合。 

高偏差，则表示可能还欠拟合。 
![](http://upload-images.jianshu.io/upload_images/1621805-719258decc6291a5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.3 机器学习基础

![](http://upload-images.jianshu.io/upload_images/1621805-dedb8abf6ac9cd1e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.4 正则化

## 逻辑回归正则化

![](http://upload-images.jianshu.io/upload_images/1621805-a87812282b60f8a8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 神经网络正则化

![](http://upload-images.jianshu.io/upload_images/1621805-1da39d5fedc2a0fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.5 为什么正则化可以减少过拟合？

![](http://upload-images.jianshu.io/upload_images/1621805-ea8e60b620e0c0ab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

对于S类激活函数，当W接近与零时，激活函数更加趋近于线性函数，所以不会有太复杂的划分： 
![](http://upload-images.jianshu.io/upload_images/1621805-68239bd925cfa299.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.6 Dropout 正则化  1.7 理解 Dropout

更多理解和先参阅：[http://blog.csdn.net/u011239443/article/details/76360294#t49](http://blog.csdn.net/u011239443/article/details/76360294#t49)

![](http://upload-images.jianshu.io/upload_images/1621805-9eb02dc27d53e985.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

测试阶段不使用Dropout

# 1.8 其他正则化方法

## 增加数据

![](http://upload-images.jianshu.io/upload_images/1621805-553649309aa354fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 提早停止

![](http://upload-images.jianshu.io/upload_images/1621805-7edf3b060e3feeba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.9 正则化输入

参阅：[http://blog.csdn.net/u011239443/article/details/76360294#t3](http://blog.csdn.net/u011239443/article/details/76360294#t3)

![](http://upload-images.jianshu.io/upload_images/1621805-60f38b3ac1be4563.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

归一化的好处：

![](http://upload-images.jianshu.io/upload_images/1621805-e032e347dc7f00f0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.10 梯度消失与梯度爆炸

![](http://upload-images.jianshu.io/upload_images/1621805-760cde33ccd07d67.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.11 神经网络的权重初始化

初始化权值尽可能的接近于0，梯度下降的速度才能尽可能的快，所以讲初始化乘以一定的系数： 
![](http://upload-images.jianshu.io/upload_images/1621805-a24222a4cfaf5a50.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 1.12 梯度的数值逼近 1.13 梯度检验 1.14 关于梯度检验实现的注记

基本的数学概念，不记录。
- 
将模型看作关于各个参数函数： 
![](http://upload-images.jianshu.io/upload_images/1621805-61f98c1a6968fb4c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- 
检验： 
![](http://upload-images.jianshu.io/upload_images/1621805-2b0ff4c1c684110e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- 
梯度检验注意项： 
![](http://upload-images.jianshu.io/upload_images/1621805-a292361e680ddffd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![qrcode_1504084513340.jpg](http://upload-images.jianshu.io/upload_images/1621805-d5f8c414064ae562.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


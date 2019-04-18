# 《深度学习Ng》课程学习笔记01week3——浅层神经网络 - Soul Joy Hub - CSDN博客

2017年09月07日 17:55:11[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1155
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/77884830](http://blog.csdn.net/u011239443/article/details/77884830)

# 3.1 神经网络概览

![](http://upload-images.jianshu.io/upload_images/1621805-eab80d6b03c2ce22.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.2 神经网络表示

![](http://upload-images.jianshu.io/upload_images/1621805-05be8ec544d406f7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.3 计算神经网络的输出

![](http://upload-images.jianshu.io/upload_images/1621805-2f33bdf7ae3f2a5a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

对应的正向传播公式：

![](http://upload-images.jianshu.io/upload_images/1621805-c2fb8a73d4d3a5d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-6b1a854310babd65.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.4 多个例子中的向量化

![](http://upload-images.jianshu.io/upload_images/1621805-8d615a70c671cb81.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.5 向量化实现的解释

![](http://upload-images.jianshu.io/upload_images/1621805-936c93492f5ef66d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-bf9eef82f776b4ea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.6 激活函数

![](http://upload-images.jianshu.io/upload_images/1621805-5ab5e7b50a921383.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

更多可以参阅《[神经网络-激活函数对比](http://blog.csdn.net/u011239443/article/details/77711752)》

# 3.7 为什么需要非线性激活函数？

如果没有非线性激活函数，那么神经网络其实就是只是单个神经元的线性组合： 
![](http://upload-images.jianshu.io/upload_images/1621805-099294f7e39d4aa2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.8 激活函数的导数

## sigmoid

![](http://upload-images.jianshu.io/upload_images/1621805-c85d02cdef1d3a47.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## Tanh

![](http://upload-images.jianshu.io/upload_images/1621805-d24f593dbb0fc815.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## ReLU

![](http://upload-images.jianshu.io/upload_images/1621805-f23f190f269f396c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.9 神经网络的梯度下降法

![](http://upload-images.jianshu.io/upload_images/1621805-865b113190a0333c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-3a4cfd2ea89a2230.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

更多可见 ： [http://blog.csdn.net/u011239443/article/details/76680704#t2](http://blog.csdn.net/u011239443/article/details/76680704#t2)

# 3.10 （选修）直观理解反向传播

![](http://upload-images.jianshu.io/upload_images/1621805-35903654038e014f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

总结

![](http://upload-images.jianshu.io/upload_images/1621805-bec344f8002967eb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 3.11 随机初始化

初始化W不能设为0，否则同一层的神经元的改变相同，使得类似于单个神经元： 
![](http://upload-images.jianshu.io/upload_images/1621805-3962b3b454c8ea37.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

解决方案，随机生成绝对值较小的初始值（初始值绝对值太大，会使得S型激活函数的绝对值趋于0，从而使得训练缓慢）： 
![](http://upload-images.jianshu.io/upload_images/1621805-644fbe22a59e988e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![这里写图片描述](https://img-blog.csdn.net/20170907175444062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# 《深度学习Ng》课程学习笔记02week2——优化算法 - Soul Joy Hub - CSDN博客

2017年09月22日 18:57:56[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：667标签：[深度学习																[优化																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)
个人分类：[深度学习](https://blog.csdn.net/u011239443/article/category/6990149)

所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)


[http://blog.csdn.net/u011239443/article/details/78066082](http://blog.csdn.net/u011239443/article/details/78066082)

# 2.1 Mini-batch 梯度下降法

![](http://upload-images.jianshu.io/upload_images/1621805-b2723f8d3acdf187.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-58d7fadbabe23845.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.2 理解 mini-batch 梯度下降法

![](http://upload-images.jianshu.io/upload_images/1621805-fa73dbe1d6cf3e72.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-87960ec6b4d39321.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-1a143731fb1e6cf5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.3 指数加权平均

对温度做指数加权平均曲线：

![](http://upload-images.jianshu.io/upload_images/1621805-d38aef3dea5bec93.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

β = 0.98 时，会得到更加平缓的曲线，如图绿色。 

β = 0.5 时，会得到更加波动的曲线，如图黄色。

![](http://upload-images.jianshu.io/upload_images/1621805-399f8ed0ffff5c75.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.4 理解指数加权平均

![](http://upload-images.jianshu.io/upload_images/1621805-2cb28bedfb752631.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-cda1b6db072f93ca.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.5 指数加权平均的偏差修正

当在训练刚刚开始的时候，v会很小，为了修正这种偏差，我们可以使用以下方法： 
![](http://upload-images.jianshu.io/upload_images/1621805-1c40876ebed6925a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.6 动量梯度下降法（momentun）

梯度下降中，随着迭代次数的增加，我们需要将变化的幅度越来越小。这就使用到了动量梯度下降法： 
![](http://upload-images.jianshu.io/upload_images/1621805-3ae4cfeeace522ff.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-a07769705ac5c0b8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

之所以叫动量梯度下降法，是因为从另外一个角度看，是在改变下降的速度： 
![](http://upload-images.jianshu.io/upload_images/1621805-82c745e8c9c4f4d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

v’ = 摩擦力影响比率*原来速度v + 加速

# 2.7 RMSprop

![](http://upload-images.jianshu.io/upload_images/1621805-e29f0f2bd43b6bb6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.8 Adam 优化算法

Adam 就是 momentun 和 RMSprop的结合：

![](http://upload-images.jianshu.io/upload_images/1621805-d2762fd91830e242.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.9 学习率衰减

减小幅度还可以用学习率衰减： 
![](http://upload-images.jianshu.io/upload_images/1621805-160f7922225aa609.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-a88381a3c0b248a8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 2.10 局部最优的问题

当特征维度非常大时，我们的最优化问题通常不是一个局部极小点或者局部极大点，而是鞍点。 

当 Hessian 是正定的（所有特征值都是正的），则该临界点是局部极小点。当 Hessian 是负定的（所有特征值都是负的），这个点就是局部极大点。在多维情况下，实际上我们可以找到确定该点是否为鞍点的积极迹象（某些情况下）。如果 Hessian 的特征值中至少一个是正的且至少一个是负的，那么 x 是 f 某个横截面的局部极大点，却是另一个横截面的局部极小点。 
![](http://upload-images.jianshu.io/upload_images/1621805-79f8ad2c93ea2bce.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

走去鞍点的平缓区的方法是使用如Adam、momentun 和 RMSprop算法。

![这里写图片描述](https://img-blog.csdn.net/20170922185735065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


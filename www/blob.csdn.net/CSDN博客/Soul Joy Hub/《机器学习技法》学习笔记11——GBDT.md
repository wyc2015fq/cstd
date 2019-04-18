# 《机器学习技法》学习笔记11——GBDT - Soul Joy Hub - CSDN博客

2017年08月20日 22:53:52[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：904
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



[http://blog.csdn.net/u011239443/article/details/77435463](http://blog.csdn.net/u011239443/article/details/77435463)

# Adaptive Boosted Decision Tree

关于AdaBoost、提升树可先参阅：[http://blog.csdn.net/u011239443/article/details/77294201](http://blog.csdn.net/u011239443/article/details/77294201)

这里仅对其做一定的补充。 

对提升决策树桩的模型中，我们对树的节点进行分隔时，需要该节点中的各个数据的权重来计算总错误：
`weightedError = D.T * errArr`
树根比较好算，数据都是全量的数据，但是如果是非根的叶节点呢？这就是困难了。于是，我们就想到了变动数据集方法： 
![](http://upload-images.jianshu.io/upload_images/1621805-b4cfe78b1cfed062.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

比如说，某条数据的权重是0.1，我们就似的样本抽取到它的概率变为0.1。

# AdaBoost优化背后的意义

$u_n{t+1}$为第n个数据，t+1轮优化的权重 
![](http://upload-images.jianshu.io/upload_images/1621805-a0537febe05ae157.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-e8e03ede3fc2aa21.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们把上述橙色部分叫做`voting score`

我们回过头来看模型，并套用SVM的形式：

![](http://upload-images.jianshu.io/upload_images/1621805-9c4503554c6d8725.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

套用SVM，我们可知我们需要对模型的优化是：

![](http://upload-images.jianshu.io/upload_images/1621805-cae4aa8335c5102a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

于是，我们得到了AdaBoost的损失函数：

![](http://upload-images.jianshu.io/upload_images/1621805-4fb7a129a202d7a2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

使用梯度下降的方法，调整最后g（也就是h），来最小化误差： 
![](http://upload-images.jianshu.io/upload_images/1621805-48915379c7f55cd2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

使用泰勒展开：

![](http://upload-images.jianshu.io/upload_images/1621805-77d76d528004eaf9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

使得损失函数等价为：

![](http://upload-images.jianshu.io/upload_images/1621805-10e170e336567483.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

于是，我们只需最小化：

![](http://upload-images.jianshu.io/upload_images/1621805-e79325ec5d3a8b83.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

得到： 
![](http://upload-images.jianshu.io/upload_images/1621805-70bd80ba60d8c2aa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们可以看到，我们选择的h是要使得其预测率最小则结果最优。这就把问题转移到了函数h的最优化的问题了，这就变成了我们非常熟悉的问题，比如 如果是 AdaBoost Decision Tree，那就用决策树回归最优化h就行了。

然后我们来调整  η，设$g_t$就是我们的最优化h，那么最优化问题变成了：

![](http://upload-images.jianshu.io/upload_images/1621805-c02fc3d3b2ee57f5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-3fab5cb8a4258f41.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们对其求导，最优化得到：

![](http://upload-images.jianshu.io/upload_images/1621805-7992d252a6ed4a8e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

对，没有错！AdaBoost中的 α 的含义其实就是在最优化g函数梯度下的最优化的步长！

# Gradient Boosting

我们上节套用SVM来得出Adaboost的损失函数：

![](http://upload-images.jianshu.io/upload_images/1621805-c0d8fd56cda12eca.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

但其实我们可以用其他误差来得出损失函数，这就是叫做Gradient Boosting，通常使用差方来得到：

![](http://upload-images.jianshu.io/upload_images/1621805-fff7efe150870698.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

对上式进行关于S在$S_n$上泰勒展开： 
![](http://upload-images.jianshu.io/upload_images/1621805-7377ad545f6cc15a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

即上式中 x = s ， $x_0 = Sn$得到了：

![](http://upload-images.jianshu.io/upload_images/1621805-7e9ab85494477e27.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

由于我们想让η来调节步长，而h函数只是决定方向，所以需要让$h(x)$为负数，但是绝对值又要尽可能的小，于是优化问题变成了：

![](http://upload-images.jianshu.io/upload_images/1621805-a84bd4d5e4f871e6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

可以看出我们的h其实是最优化回归：

![](http://upload-images.jianshu.io/upload_images/1621805-f566c790f0396e2d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

也就是说，h是想把输入特征，回归拟合输出为实际值和上一轮模型预测值差值。

同样的，假设我们已经找到最佳的回归函数g，接下来调整η：

![](http://upload-images.jianshu.io/upload_images/1621805-5be1411a5416bf78.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-85ef086026364215.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

可以看到，这也是一个简单的一元回归：

![](http://upload-images.jianshu.io/upload_images/1621805-2ed35eb9186eca6a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

总结GBDT模型如下：

![](http://upload-images.jianshu.io/upload_images/1621805-7537f055d4bff6fa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 总结集成模型

![](http://upload-images.jianshu.io/upload_images/1621805-6c522305154e3d4f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

bagging 和 boosting 加上 决策树 可以分别得到：

![](http://upload-images.jianshu.io/upload_images/1621805-76f797305aca07ae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

为什么集成方法效果好？因为它有很多不同的模型表示，所以表达能力非常好，避免的欠拟合；而又是因为有很多模型混合，所以避免的某些模型的过拟合。

![](http://upload-images.jianshu.io/upload_images/1621805-16c58a3578cf37dd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![这里写图片描述](https://img-blog.csdn.net/20170820225439507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


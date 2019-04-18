# 《机器学习技法》学习笔记03——核SVM - Soul Joy Hub - CSDN博客

2017年08月02日 21:35:41[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：575
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



[http://blog.csdn.net/u011239443/article/details/76598872](http://blog.csdn.net/u011239443/article/details/76598872)

# 核技巧

接着上篇博文的问题：

![](http://upload-images.jianshu.io/upload_images/1621805-5ef3684034c1f6d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们先假定：

![](http://upload-images.jianshu.io/upload_images/1621805-9f46cf6ed90dc20a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

则有：

![](http://upload-images.jianshu.io/upload_images/1621805-5e88be87bf8a9e7a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

于是我们就得到了核函数：

![](http://upload-images.jianshu.io/upload_images/1621805-d12dd7de2151ec7d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

那么我们就可以直接用核函数带入到原来的问题中，我们能计算出b：

![](http://upload-images.jianshu.io/upload_images/1621805-ea66bbdb84e7f487.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

讲核函数代入$g_{SVM}=sign(\sum_{SV indices n}w^Tz_s+b)$得：

![](http://upload-images.jianshu.io/upload_images/1621805-8652e25b638689f8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 多项式核

我们可以讲$Φ_2$做一些变化，得到更容易的核函数：

![](http://upload-images.jianshu.io/upload_images/1621805-7585116ed4f682c0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

还可以讲其推广：

![](http://upload-images.jianshu.io/upload_images/1621805-0dfbf0b35b779eab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

$K_1$ 称为线性核，在选择核函数时，我们优先选择简单的线性核。

# 高斯核

我们能将x映射到无限高的维度上吗？答案是肯定的，使用高斯核就能做到：

![](http://upload-images.jianshu.io/upload_images/1621805-c932105105f7c8a8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

高斯核的定义：

![](http://upload-images.jianshu.io/upload_images/1621805-c23cdf8add8a2a4c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

不同γ取值的例子：

![](http://upload-images.jianshu.io/upload_images/1621805-b35b5b2a36ac17f1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

因此SVM还是容易产生过拟合，γ取值不能过大

![这里写图片描述](https://img-blog.csdn.net/20170828152157238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


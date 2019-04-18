# 《机器学习技法》学习笔记01——线性SVM - Soul Joy Hub - CSDN博客

2017年08月02日 11:02:28[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1175
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



[http://blog.csdn.net/u011239443/article/details/76572743](http://blog.csdn.net/u011239443/article/details/76572743)

# 最大间距分离超平面

![](http://upload-images.jianshu.io/upload_images/1621805-435510d5a503afa3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-eb6448d94542d32c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

胖的超平面具有更好的错误容忍性。

![](http://upload-images.jianshu.io/upload_images/1621805-4ae06d06d11d151e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-6cf1c2cd12f06525.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们目标就是找到能一个超平面，到各个点$x_n$到w最小的距离尽可能的大。而且w需要能正确划分，即 label $y_n$ 需要和计算出来的结果$w^Tx_n$同号。

# 标准最大间距问题

我们把$w^Tx_n$拆分：

![](http://upload-images.jianshu.io/upload_images/1621805-1a6b1bf1ce0de023.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

得到$w^Tx+b$
- 计算x到超平面$w^Tx' + b = 0$的距离：

x’和x”是超平面上的任意两个点：

![](http://upload-images.jianshu.io/upload_images/1621805-31e4e744c945cce4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-eef8653dfdd02346.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

所以，w的超平面的法向量，则得出距离：

![](http://upload-images.jianshu.io/upload_images/1621805-c0828e36acaa8ff3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

由于：

![](http://upload-images.jianshu.io/upload_images/1621805-addc71b47b313ec6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

所以我们可以把距离写成：

![](http://upload-images.jianshu.io/upload_images/1621805-c5aad5bb8642c015.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

于是问题变成了：

![](http://upload-images.jianshu.io/upload_images/1621805-94486988f8a6b6a5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

由于：

![](http://upload-images.jianshu.io/upload_images/1621805-7039726dec3b1d1e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

超平面不会因系数而改变，所以我们可以对$w^Tx+b$进行任意放缩，最终使得：

![](http://upload-images.jianshu.io/upload_images/1621805-04917aefccb03ba3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

问题就变成了：

![](http://upload-images.jianshu.io/upload_images/1621805-f03ea06039ed6e02.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

$y_n(w^Tx_n+b)$最小也要等于1，所以条件$y_n(w^Tx_n+b)>0$可以去掉，问题变成了：

![](http://upload-images.jianshu.io/upload_images/1621805-a2b9e385ef7660be.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们将条件放大成：

![](http://upload-images.jianshu.io/upload_images/1621805-2d2b573710f32b37.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们只要证明，不可能所以的$y_n(w^Tx_n+b)$都大于1，那么放大后的条件就和原来的条件等价了。
- 证明： 

假设$y_n(w^Tx_n+b)$都大于1，最优解(b,w)使得$y_n(w^Tx_n+b) >= c > 1$。 

则存在$(b_2,w_2) = (b/c ,w/c)$使得$y_n(w_2^Tx_n+b_2)>=1$。 

但是$1/||w_2|| > 1/||w||$，所以(b,w)不是最优解，即假设不成立。

再经过一些变换，我们的问题变成了：

![](http://upload-images.jianshu.io/upload_images/1621805-29286cfa60304ae8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![这里写图片描述](https://img-blog.csdn.net/20170802110332173?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


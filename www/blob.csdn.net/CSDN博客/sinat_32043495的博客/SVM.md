# SVM - sinat_32043495的博客 - CSDN博客





2018年03月05日 17:25:15[LZXandTM](https://me.csdn.net/sinat_32043495)阅读数：78








![](https://img-blog.csdn.net/20180305172117860)


![](https://img-blog.csdn.net/20180305172126494)


# 1.loss function

红色这条线就是Square Loss的表现，与黑色的理想loss曲线比较，当x很大是，将会取得很大的值，这是不合理的，既然如此，我们再试一下Square Loss+cross entropy。


蓝色这条线就是Sigmoid+Square loss，但是实际上，Square的performance并不好，用cross entropy更合理，就是绿色那条线，当横坐标趋近无穷大时，趋近于0，如果负无穷，则会无穷大。比较一下蓝绿两条线，如果我们横坐标，从-2移到-1，绿色这条线变化很大，蓝色反之，造成的效果就是，横坐标非常negative时，绿色调整参数可以取得较好的回报，所以它很乐意把negative的值变大，而蓝色反之，很懒惰。


如果比较紫绿两条线，它们最大的不同就是对待做得好的example的态度，如果把横坐标从1挪到2，对绿色来说它会有动机把横坐标变得更大，而紫色对此的态度是及格就好，不会再努力变大。


![](https://img-blog.csdn.net/20180305172137363)


![](https://img-blog.csdn.net/20180305172449921)


# 2.LinearSVM

## 2.1. Hinge Loss

![](https://img-blog.csdn.net/20180305173955620)

**SVM通常不用gradient descent做，但也是可以做的**

![](https://img-blog.csdn.net/20180305174013438)


![](https://img-blog.csdn.net/20180305194726994)


![](https://img-blog.csdn.net/20180305194751510)


## 2.2 Kernel trick

![](https://img-blog.csdn.net/20180305195218980)![](https://img-blog.csdn.net/20180305195224232)![](https://img-blog.csdn.net/2018030519523336)![](https://img-blog.csdn.net/20180305195243568)![](https://img-blog.csdn.net/20180305195316994)

![](https://img-blog.csdn.net/20180305202658171)


![](https://img-blog.csdn.net/20180305202934171)![](https://img-blog.csdn.net/20180305202944957)


![](https://img-blog.csdn.net/2018030520251566)![](https://img-blog.csdn.net/20180305195332624)









# 复习机器学习算法：Logistic 回归 - bigfacesafdasgfewgf - CSDN博客





2015年03月05日 10:03:49[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：855












区别于线性回归，不是把每个特征直接乘以系数，而是用一个S型函数（Logistic函数）。如下：

![](https://img-blog.csdn.net/20150305100439073?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

使用这种形式函数的原因（概率、求导）。



代价函数，也不是线性回归中的误差平方和，而是基于对数似然函数，如下：



单个样本的后验概率为：![](https://img-blog.csdn.net/20150305100532223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)（y = 0， 1） 类似于二项分布的概率密度函数。

整个样本集的后验概率：

![](https://img-blog.csdn.net/20150305100731169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




对数似然函数对于代价函数，如下：



![](https://img-blog.csdn.net/20150305101227034?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

梯度下降法求解，对上面的代价函数求导，如下：

![](https://img-blog.csdn.net/20150305101254692?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




误差乘以对应的属性值，再求和。形式和线性回归一致，解释了为何设计这样的S型函数和代价函数。这样的梯度下降法的计算量简单。





LR回归的优点：计算量小，从梯度下降法求导公式可以看出；可以处理非线性数据。

缺点：容易发生过拟合。



如何避免过拟合：

（1） 降维，可以使用PCA算法把样本的维数降低，使得模型的theta的个数减少，次数也会降低，避免了过拟合；

（2） 正则化，设计正则项regularization term。



正则化作用就是防止某些属性前的系数权重过大，出现过拟合。



注意LR回归中解决过拟合的方式和决策树中解决的方法不同。






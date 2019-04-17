# 【算法】最优化之RDA正则化双平均 - zkq_1986的博客 - CSDN博客





2018年02月28日 10:40:51[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：190







sgd：
对损失项和正则化项一刀切，没对正则化项区别对待。以致
1）优化速度慢；2）准确性不高。

rda：
regularized dual averaging
正则双平均
对损失项和正则化项区分考虑，获得正则化项的闭合形解。在优化速度方面优于sgd。

为什么称为双平均？

在损失项子梯度中，一是对历史子梯度的平均，二是对当前子梯度的平均。




![](https://img-blog.csdn.net/20180228103901357)

![](https://img-blog.csdn.net/20180228103847710)


![](https://img-blog.csdn.net/20180228103919941)


![](https://img-blog.csdn.net/20180228103927204)


![](https://img-blog.csdn.net/20180228103934982)





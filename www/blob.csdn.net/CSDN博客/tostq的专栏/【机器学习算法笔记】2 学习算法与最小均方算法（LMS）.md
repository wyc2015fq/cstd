# 【机器学习算法笔记】2. 学习算法与最小均方算法（LMS） - tostq的专栏 - CSDN博客





2017年03月28日 19:36:42[tostq](https://me.csdn.net/tostq)阅读数：1423
所属专栏：[机器学习算法笔记](https://blog.csdn.net/column/details/15045.html)









# 【机器学习算法笔记】2. 学习算法与最小均方算法（LMS）

最小均方算法是一个非常流行的在线学习算法。其是建立在自适应滤波和自适应调整权重上的。

## 2.1 迭代下降思想

三种以迭代下降思想为基础的无约束最优化方法：

### 2.1.1 最速下降法：

在最速下降法中，对权值向量的调整是在最速下降的方向进行的，即它是与梯度向量方向相反的，梯度向量记为： 
![这里写图片描述](https://img-blog.csdn.net/20170328192957357?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最速下降法一般表示为： 
![这里写图片描述](https://img-blog.csdn.net/20170328193006410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其原理是根据一阶泰勒展开式： 
![这里写图片描述](https://img-blog.csdn.net/20170328193015591?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 2.1.2 牛顿法：

牛顿法是根据二阶泰勒展开式： 
![这里写图片描述](https://img-blog.csdn.net/20170328193027701?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 2.1.3 高斯-牛顿法：

高斯-牛顿法是为了处理牛顿法的计算复杂而不对其收敛行为做太严重的让步，而提出的。其是建立在误差平方和的代价函数上的： 
![这里写图片描述](https://img-blog.csdn.net/20170328193409083?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

损失函数： 
![这里写图片描述](https://img-blog.csdn.net/20170328193337143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 修正式，相当于在原来的基础上添加了稳定项。 

利用高斯-牛顿法来推导最小二乘滤波器： 
![这里写图片描述](https://img-blog.csdn.net/20170328193236173?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2.2 维纳滤波器

遍历环境下最小二乘滤波器的极限形式。 

当观测样本数趋于无穷时，最小二乘滤波器渐进趋于维纳滤波器，即总体平均相关参数趋于实际输入相关参数R

## 2.3 最小均方算法（LMS）

其是建立在极小化代价函数的瞬时值，代价函数为： 
![这里写图片描述](https://img-blog.csdn.net/20170328193102108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
LMS将最速下降法的梯度估计用，权值向量的瞬时估计来计算，不像最速下降法有明显明确的下降轨迹，而LMS会有一种随机的波动，有时也被称为随机梯度下降法。 
![这里写图片描述](https://img-blog.csdn.net/20170328193536225?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

LMS的优点和局限 

1、计算简单且有效 

2、鲁棒性 

3、限制LMS性能的因素：LMS算法的收敛行为对输入向量相关矩阵R的特征值散布（条件数），条件数： 
![这里写图片描述](https://img-blog.csdn.net/20170328193547084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

当条件数特别大的时候，会变得敏感。可以通过增加一个正则项来解决。 

关于学习率的退火方案： 
![这里写图片描述](https://img-blog.csdn.net/20170328193556084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdG9zdHE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








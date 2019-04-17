# 非线性SVM与核函数 - 浅梦的博客 - CSDN博客





2017年08月25日 10:53:41[浅梦s](https://me.csdn.net/u012151283)阅读数：4159








# 核技巧

## 非线性分类问题

用线性分类的方法求解非线性分类问题分为**两步**： 

首先使用一个变换将原空间的数据映射到新空间； 

然后在新空间里利用线性分类学习方法从训练数据中学习分类模型。核技巧就是这种方法。 

核技巧应用到SVM，基本想法是通过一个非线性变换将输入空间对应于一个特征空间，使得在输入空间中的超曲面模型对应于特征空间中的超平面模型（SVM）。这样，分类问题的学习任务通过在特征空间中求解线性SVM就可以完成。
## 核函数的定义

$K(x,z)=\phi(x)\cdot \phi(z)$

称$K(x,z)$为核函数，$\phi(x)$为映射函数。 

核技巧的想法是，在学习和预测中只定义核函数$K(x,z)$，而不显式地定义映射函数$\phi$。通常，直接计算$K(x,z)$比较容易。
## 核技巧在SVM中的应用

在线性SVM的对偶问题中，目标函数和决策函数都只涉及输入实例与实例之间的内积。对偶问题的目标函数中的内积$x_i\cdot x_j$可以用和函数$K(x_i,x_j)=\phi(x_i)\cdot\phi(x_j)$代替。 

目标函数变为 
$W(a)=\frac{1}{2}\sum\limits_{i=1}^N\sum\limits_{j=1}^Na_ia_jy_iy_jK(x_i,x_j)-\sum\limits_{i=1}^Na_i$

分类决策函数成为 
$f(x)=sign(\sum\limits_{i=1}^Na_i^*y_iK(x_i,x)+b^*)$

当映射函数是非线性函数时，学习到的含有核函数的SVM就是非线性分类模型。 

在核函数给定的条件下，可以利用解线性分类的方法求解非线性分类问题的SVM。**学习是隐式地在特征空间中进行的，不需要显式地定义特征空间和映射函数**。
# 正定核

![这里写图片描述](https://img-blog.csdn.net/20170929132506036?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170929132611199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170929132644946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 常用核函数

![这里写图片描述](https://img-blog.csdn.net/20170929132838675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 多项式核函数

$K(x,z)=(x\cdot z+r)^d$

对应的SVM是一个d次多项式分类器。

## 高斯核函数

$K(x,z)=exp(-\frac{\gamma||x-z||^2}{2\sigma^2})$

对应的SVM是高斯径向基函数(radial basis function)分类器。 
$\gamma$定义了了当个样本对整体分类超平面的影响，当$\gamma$比较小时，单个样本对整个分类超平面的影响比较大，更容易被选择为支持向量。 

如果把惩罚系数C和RBF核的$\gamma$一起看，当C比较大，$\gamma$比较小时，会有更多支持向量，模型比较复杂。 
![这里写图片描述](https://img-blog.csdn.net/20170929132819649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## sigmoid核函数

$K(x,z)=tanh(\gamma <x\cdot z>+r)$

## 字符串函数

核函数不仅可以定义在欧式空间上，还可以定义在离散数据的集合上。

# 非线性SVM算法

![这里写图片描述](https://img-blog.csdn.net/20170825105210907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

参考文献

> 
《统计学习方法》第7章 

  机器学习 小象学院










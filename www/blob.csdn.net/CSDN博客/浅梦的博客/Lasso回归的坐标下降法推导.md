# Lasso回归的坐标下降法推导 - 浅梦的博客 - CSDN博客





2017年08月22日 21:38:15[浅梦s](https://me.csdn.net/u012151283)阅读数：6793标签：[Lasso																[坐标下降																[次梯度](https://so.csdn.net/so/search/s.do?q=次梯度&t=blog)
个人分类：[优化方法																[机器学习																[线性模型](https://blog.csdn.net/u012151283/article/category/7194653)




# 目标函数

Lasso相当于带有L1正则化项的线性回归。先看下目标函数：$RSS(w)+\lambda \Vert w\Vert_1=\sum_{i=0}^{N}(y_i-\sum_{j=0}^D{w_jh_j(x_i)})^2+\lambda \sum_{j=0}^{D}|w_j|$

这个问题由于正则化项在零点处不可求导，所以使用非梯度下降法进行求解，如坐标下降法或最小角回归法。

# 坐标下降法

本文介绍坐标下降法。

坐标下降算法每次选择一个维度进行参数更新，维度的选择可以是随机的或者是按顺序。

当一轮更新结束后，更新步长的最大值少于预设阈值时，终止迭代。
下面分为两部求解

## RSS偏导

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190219225615733.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=,size_16,color_FFFFFF,t_70)

下面做一下标记化简
$\rho_j=\sum_{i=1}^Nh_j(x_i)(y_i-\sum_{k \neq j }w_kh_k(x_i))$
$z_j=\sum_{i=1}^N h_j(x_i)^2$

上式化简为$\frac{\partial}{\partial w_j}RSS(w)=-2\rho_j+2w_jz_j$
## 正则项偏导

**次梯度方法**(subgradient method)是传统的梯度下降方法的拓展，用来处理不可导的凸函数。
![这里写图片描述](https://img-blog.csdn.net/20170822210515050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
$$\lambda \partial_{w_j}|w_j|=\begin{cases}-\lambda&amp;w_j&lt;0\\[-\lambda,\lambda]&amp; w_j=0\\\lambda&amp;w_j&gt;0\end{cases}$$
## 整体偏导数

$$\lambda \partial_{w_j}\text{[lasso cost]}= 2z_jw_j-2\rho_j+\begin{cases}-\lambda&amp;w_j&lt;0\\[-\lambda,\lambda]&amp; w_j=0\\\lambda&amp;w_j&gt;0\end{cases}\\=\begin{cases}2z_jw_j-2\rho_j-\lambda&amp;w_j&lt;0\\[-2\rho_j-\lambda,-2\rho_j+\lambda]&amp; w_j=0\\2z_jw_j-2\rho_j+\lambda&amp;w_j&gt;0\end{cases}$$

要想获得最有解，令

$\lambda \partial_{w_j}\text{[lasso cost]}=0$。

解得，
$$\hat w_j= \begin{cases}(\rho_j+\lambda/2)/z_j&amp;\rho_j&lt;-\lambda/2\\0&amp; \rho_j\text{ in }[-\lambda/2,\lambda/2]\\(\rho_j-\lambda/2)/z_j&amp;\rho_j&gt;\lambda/2\end{cases}$$
![这里写图片描述](https://img-blog.csdn.net/20170822212911482?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 伪代码

预计算$z_j=\sum_{i=1}^N h_j(x_i)^2$

初始化参数w（全0或随机）

循环直到收敛:
> 
for j = 0,1,…D

$ \space \space\space\space\rho_j=\sum_{i=1}^Nh_j(x_i)(y_i-\sum_{k \neq j }w_kh_k(x_i))$
$\space \space\space\space update\space w_j$

选择变化幅度最大的维度进行更新
# 概率解释

## 拉普拉斯分布

随机变量$X\sim Laplace(\mu,b)$，其中$\mu$是位置参数，$b&gt;0$是尺度参数。

概率密度函数为
$f(x|\mu,b)=\frac{1}{2b}exp(-\frac{|x-\mu|}{b})$
## MAP推导

假设$\epsilon_i\sim N(0,\sigma^2)$，$w_i\sim Laplace(0,\frac{1}{\lambda})$
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190219230029955.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTIxNTEyODM=,size_16,color_FFFFFF,t_70)
等价于
![](https://img-blog.csdnimg.cn/20190219230133414.png)](https://blog.csdn.net/u012151283/article/category/6653295)](https://blog.csdn.net/u012151283/article/category/7120629)](https://so.csdn.net/so/search/s.do?q=坐标下降&t=blog)](https://so.csdn.net/so/search/s.do?q=Lasso&t=blog)





# 非负矩阵分解NMF - 浅梦的博客 - CSDN博客





2017年08月31日 15:00:15[浅梦s](https://me.csdn.net/u012151283)阅读数：395
个人分类：[机器学习																[矩阵分解](https://blog.csdn.net/u012151283/article/category/7137404)](https://blog.csdn.net/u012151283/article/category/6653295)








# 问题描述

![这里写图片描述](https://img-blog.csdn.net/20170831111838108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170831112223834?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 优化目标

假设噪声矩阵$E=X-WH$，我们可以用两种目标函数优化

## 平方误差最小化



$\begin{align*}\min\limits_{W,H}||X-WH||^2=\sum\limits_i\sum\limits_j(X_{ij}-(WH)_{ij})^2\\s.t.W_{ik}\ge0,H_{kj}\ge0\end{align*}$

算法 
![这里写图片描述](https://img-blog.csdn.net/20170831145306481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## KL散度最小化

![这里写图片描述](https://img-blog.csdn.net/20170831145420511?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 概率解释

## 假设误差服从高斯分布

可以推出平方误差最小化

## 假设误差服从泊松分布

可以推出KL散度最小化 
![这里写图片描述](https://img-blog.csdn.net/20170831145618587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# NMF与主题模型

![这里写图片描述](https://img-blog.csdn.net/20170831145857309?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# NMF与SVD








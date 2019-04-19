# Factorization Machine - Arthur的随笔 - CSDN博客
2018年12月14日 14:47:31[largetalk](https://me.csdn.net/largetalk)阅读数：160
*很久很久没有更新这个博客了，最近想发点东西，先发点以前写的吧*
## 简介
因子分解机(Factorization Machine, FM)是由Steffen Rendle提出的一种基于矩阵分解的机器学习算法。 对于因子分解机FM来说，最大的特点是对于稀疏的数据具有很好的学习能力。
## 模型
$\hat{y} := w_{0} + \sum_{i=1}^{n}w_{i}x_{i} + \sum_{i=1}^{n-1}\sum_{j=i+1}^{n}\langle v_{i}, v_{j} \rangle x_{i}x_{j}$
其中，参数  $w_{0}\in \mathbb{R}, W\in\mathbb{R}^{n}, V\in\mathbb{R}^{n\times k}. \langle v_{i}, v_{j} \rangle$ 表示的是两个长度为k额向量的>    点积:
$\langle v_{i}, v_{j} \rangle := \sum_{f=1}^{k}\nu_{i,f}\cdot\nu_{j,f}$
其中，$v_{i}$ 表示的是系数矩阵 **V** 的第 *i* 维向量，且 $v_{i} = (\nu_{i,1},\nu_{i,2},\cdots,\nu_{i,k}), k\in\mathbb{N}^{+}$ 称为超参数>。在因子分解机FM模型中，前面两部分是传统的线性模型，最后一部分将两个互异特征分量之间的相互关系考虑进来。
因子分解机FM也可以推广到高阶的形式，即将更多互异特征分量之间的相互关系考虑进来。
## 算法
### Regression
在回归问题中，使用 $\hat{y}$ 作为预测结果，使用最小均方误差(the least square error)作为优化的标准:
$loss^{R}(\hat{y}, y) = \frac{1}{2}\sum_{i=1}^{m}(\hat{y}^{(i)} - y^{(i)})^{2}$
### Binary Classification
与Logistic类似，使用logit loss作为优化标准:
$loss^{C}(\hat{y}, y) = \sum_{i=1}^{m}-ln\sigma(\hat{y}^{(i)}y^{(i)})$
其中, $\sigma$ 表示的是阶跃函数Sigmoid.
## 求解
对k值的限定，对FM的表达能力有一定的影响。
对模型最后一项可做如下替换:
$\sum_{i=1}^{n-1}\sum_{j=i+1}^{n}\langle v_{i},v_{j} \rangle x_{i}x_{j}$
$= \frac{1}{2}\sum_{i=1}^{n}\sum_{j=1}^{n}\langle v_{i},v_{j} \rangle x_{i}x_{j} - \frac{1}{2}\sum_{i=1}^{n}\langle v_{i},v_{j}\rangle x_{i}x_{j}$
$= \frac{1}{2}(\sum_{i=1}^{n}\sum_{j=1}^{n}\sum_{f=1}^{k}\nu_{i,f}\nu_{j,f}x_{i}x_{j} - \sum_{i=1}^{n}\sum_{f=1}^{k}\nu_{i,f}\nu_{i,f}x_{i}x_{i})$
$= \frac{1}{2}\sum_{f=1}^{k}((\sum_{i=1}^{n}\nu_{i,f}x_{i})(\sum_{j=1}^{n}\nu_{j,f}x_{j}) - \sum_{i=1}^{n}\nu_{i,f}^{2}x_{i}^{2})$
$= \frac{1}{2}\sum_{f=1}^{k}((\sum_{i=1}^{n}\nu_{i,f}x_{i})^{2} - \sum_{i=1}^{n}\nu_{i,f}^{2}x_{i}^{2})$
使用SGD求解
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214144323480.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhcmdldGFsaw==,size_16,color_FFFFFF,t_70)
对于回归问题
$\frac{\partial loss^{R}(\hat{y},y)}{\partial\theta} = 2(\hat{y} - y)\frac{\partial\hat{y}}{\partial\theta}$
对于二分类问题
$\frac{\partial loss^{C}(\hat{y}, y)}{\partial\theta} = -\frac{1}{\sigma(\hat{y}y)}\sigma(\hat{y}y)\cdot[1 - \sigma(\hat{y}y)]\cdot y \cdot \frac{\partial\hat{y}}{\partial\theta}$
$= [\sigma(\hat{y}y) -1]\cdot y \cdot \frac{\partial\hat{y}}{\partial\theta}$
而
$\frac{\partial\hat{y}}{\partial\theta} =                                       \begin{cases}                                                                  1& if \theta = w_{0}\\                                                         x_{i}& if \theta = w_{i}\\                                                     x_{i}\sum_{j=1}^{n}\nu_{j,f}x_{j} - \nu_{i,f}x_{i}^{2}& if \theta = \nu_{i,f}  \end{cases}$

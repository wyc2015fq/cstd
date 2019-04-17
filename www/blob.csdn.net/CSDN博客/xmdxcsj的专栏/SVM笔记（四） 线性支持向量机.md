# SVM笔记（四） 线性支持向量机 - xmdxcsj的专栏 - CSDN博客





2016年05月26日 22:55:45[xmucas](https://me.csdn.net/xmdxcsj)阅读数：454








## 预备

### 适合场景

如果训练输入线性不可分，存在一些特异点，通过软间隔最大化学习得到的线性分类器称为线性支持向量机。

## 构建目标函数

和线性可分支持向量机不同在于有些实例点不满足函数间隔大于等于1的条件。考虑引入松弛变量$\xi_i\ge0$和惩罚参数$C$： 


$\min_{w,b,\xi}\frac{1}{2}||w||^2+C\sum_{i=1}^{N}\xi_i$


$s.t. y_i(w\cdot x_i+b)\ge 1-\xi_i, i=1,2,...,N$


$\xi_i\ge0, i=1,2,...,N$

目标函数有两部分组成：第一部分是间隔，第二部分是误分类点的个数。C是惩罚参数，用于调和两部分的关系。
## 求解的对偶算法

原始问题的对偶问题是： 


$\min_{\alpha}\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_{i}\alpha_{j}y_{i}y_{j}(x_i\cdot x_j)-\sum_{i=1}^{N}\alpha_i$


$\sum_{i=1}^{N}\alpha_iy_i=0$


$0\le\alpha_i\le C,i=1,2,...,N$

假设$\alpha^*=(\alpha_1^*,\alpha_2^*,...,\alpha_N^*)^T$是上面问题的最优解，那么: 


$w^*=\sum_{i=1}^{N}\alpha_i^*y_ix_i$

选择一个下标j，使得$0\lt\alpha_j^*\lt C$，可得： 


$b^*=y_j-\sum_{i=1}^{N}\alpha_i^*y_i(x_i\cdot x_j)$

根据KKT互补条件可知，$\alpha_i^*\gt0$对应的实例为支持向量。 

此时的支持向量可能分布在边界线或者边界线和分离超平面之间或者分离超平面误分一侧。 
![这里写图片描述](https://img-blog.csdn.net/20160526225525030)





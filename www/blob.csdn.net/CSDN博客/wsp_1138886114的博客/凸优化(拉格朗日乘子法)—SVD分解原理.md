# 凸优化(拉格朗日乘子法)—SVD分解原理 - wsp_1138886114的博客 - CSDN博客





2018年07月09日 20:02:36[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：386
所属专栏：[机器学习算法](https://blog.csdn.net/column/details/24447.html)










- - - [一、凸优化](#一凸优化)- - [1.1 仿射集:（直线）](#11-仿射集直线)
- [1.2 凸集:（线段）](#12-凸集线段)
- [1.3 锥定义（射线）](#13-锥定义射线)
- [1.4 凸优化](#14-凸优化)
- [1.5 示例](#15-示例)


- [二、SVD分解](#二svd分解)- - [2.1 奇异值分解](#21-奇异值分解)
- [2.2 SVD概念及理解](#22-svd概念及理解)







### 一、凸优化

$~~~~~$两个正数的算术平均数大于等于几何平均数： 


$\frac{a+b}{2} \geqslant  \sqrt{ab}$
$~~~~~$给定可逆对称阵Q，对于任意的向量x，y，有：  


$x^TQx+y^TQ^{-1}y \geqslant 2x^Ty$
![这里写图片描述](https://img-blog.csdn.net/20180709103727276?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 1.1 仿射集:（直线）

$~~~~~~~$通过集合C中任意两个不同点的直线仍然在集合C内，则称集合C为仿射集。  


$\forall \theta \in R ,~~ \forall x_1 ,x_2 \in C ~~则 x = \theta x_1+(1-\theta)x_2 \in C$

##### 1.2 凸集:（线段）

$~~~~~~~$如果通过集合C中任意两个不同点之间的线段(上的任何点)仍在集合C中，那么称集合C是凸的。  


$θx_1+(1-θ)x_2 ∈ C;其中 x1,x2∈ C，0\leqslant  θ\leqslant 1$

##### 1.3 锥定义（射线）

$~~~~~~~$给定 $C∈R_n，满足x∈C⇒tx∈C 对于任  意t≥0$称之为锥。 

凸锥（convex cone）： 
$~~~~~~~$$x_1,x_2∈C ⇒ t_1x_1+t_2x_2∈C,对于任意t_1,t_2≥0$都成立，那么称集合C 为凸锥。显然凸锥是锥的一种。 

![这里写图片描述](https://img-blog.csdn.net/20180709113330319?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 1.4 凸优化

$~~~~~~~$若函数f的定义域domf 为凸集，且满足：$\forall x,y \in domf , ~~ \theta \in [0,1]  $


$ f(\theta x + (1-\theta)y) \leqslant  \theta f(x) + (1-\theta)f(y)$
![这里写图片描述](https://img-blog.csdn.net/20180728210305246?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
$~~~~~~~$拉格朗日乘子法就是求函数$f(x1,x2,...)$ 在约束条件 $h(x1,x2,...)=0$下的极值的方法： 


$F(x,\lambda) = f(x)+\sum_{k=1}^{l}λ_kh_k(x)$
![这里写图片描述](https://img-blog.csdn.net/20180709140415481?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 1.5 示例

$~~~~~~~$求椭球的内接最大体积 : 


$\frac{x^2}{a^2}+ \frac{x^2}{b^2} + \frac{x^2}{c^2} = 1~~~求 f(x,y,z) = 8xyz 最大值$
$~~~~~~~$由拉格朗日函数F(x):(λk是各个约束的待定系数) 


$F(x,\lambda) = f(x)+\sum_{k=1}^{l}λ_kh_k(x)$
![这里写图片描述](https://img-blog.csdn.net/2018071014502833?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 二、SVD分解

$~~~~~~~$奇异值分解能够简约数据，去除噪声和冗余数据。其实也是一种降维方法，将数据映射到低维空间。

##### 2.1 奇异值分解



$M = \begin{bmatrix}2 & 4\\ 1 & 3\\ 0 & 0\\ 0 & 0\end{bmatrix}$
$~~~~~~~$1. 分别理解计算$ MM^T $和$M^TM $的特征向量及其特征值  
$~~~~~~~$2. $MM^T$ 的特征向量组成$U$
$~~~~~~~~~~~$$M^TM$ 的特征向量组成$V$
$~~~~~~~$3.对$MM^T$和$M^TM$的非零特征值求平方根，对应上述的特征向量的位置，填入$\sum$的非零对角元素（即 M 的奇异值）。 


$W = MM^T = \begin{bmatrix}2 & 4\\ 1 & 3\\ 0 & 0\\ 0 & 0\end{bmatrix}\begin{bmatrix}2 &1  &0  &0 \\ 4 &3  &0  &0 \end{bmatrix} = \begin{bmatrix}20 &14  &0  &0 \\14 &10  &0  &0 \\0 &0  &0  &0 \\ 0 &0  &0  &0 \end{bmatrix}$

求$W$特征值 与特征向量。由定义$W\overrightarrow{x} = \lambda \overrightarrow{x}$；因此 $(W-λI) \overrightarrow{x} = \overrightarrow{0}$。 


$(\begin{bmatrix}20 &14  &0  &0 \\14 &10  &0  &0 \\0 &0  &0  &0 \\ 0 &0  &0  &0 \end{bmatrix}-λ\begin{bmatrix}1 &0  &0  &0 \\0 &1  &0  &0 \\0 &0  &1  &0 \\ 0 &0  &0  &1 \end{bmatrix}) \overrightarrow{x} =\overrightarrow{0} $

即： 


$\begin{bmatrix}20-λ &14  &0  &0 \\14 &10-λ  &0  &0 \\0 &0  &-λ  &0 \\ 0 &0  &0  &-λ \end{bmatrix} =\begin{vmatrix}20-λ & 14\\ 14 & 10-λ\end{vmatrix}\begin{vmatrix}-λ & 0\\ 0 & -λ\end{vmatrix} = 0$
$~~~~~~~$求行列式det得：$((20-λ)(10-λ)-196)λ^2 = 0 。$

特征值λ： 
$~~~~~~~$$λ_1 = λ_2 = 0,λ_3 =15+\sqrt{221}\approx 29.866，λ_4 = 15-\sqrt{221}\approx 0.134$。 

将特征值带入原方程 M ，可解的特征向量。 
当$λ_1 = λ_2 = 0$



$\begin{bmatrix}20 & 14 &0  &0 \\ 14&  10& 0 &0 \\ 0 & 0 & 0 &0 \\ 0 &0  & 0 & 0\end{bmatrix}\Rightarrow\begin{pmatrix} 20x_1+14y_1=0\\  14x_1+10y_1=0 \end{pmatrix}$

当$λ_3$ = 29.866 


$\begin{vmatrix}-9.866&  14&  0&0 \\ 14 &-19.866  &0  &0 \\ 0 & 0 &  -29.866&0 \\ 0 &0  &0  &-29.866 \end{vmatrix}  \Rightarrow \begin{pmatrix}-9.866x_2+14y_2=0\\  14x_2-19.866y_2=0\\-29.866z_2 = 0\\-29.866i_2 = 0\end{pmatrix}$

当$λ_4$ = 0.134  


$\begin{vmatrix}19.866&  14&  0&0 \\ 14 &9.866  &0  &0 \\ 0 & 0 &  -0.134&0 \\ 0 &0  &0  &-0.134 \end{vmatrix} \Rightarrow \begin{pmatrix}19.866x+14y=0\\  14x+9.866y=0\\-0.134z = 0\\-0.134i = 0\end{pmatrix}$
![这里写图片描述](https://img-blog.csdn.net/20180710152003620?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 2.2 SVD概念及理解

$~~~~~~~$在实数内，我们实质上是将一个复杂的变换 $M：\mathbb{R}^m\rightarrow \mathbb{R}^n $分解成了三个变换：  

 旋转/镜像 $U：\mathbb{R}^m\rightarrow \mathbb{R}^m$； 

 缩放 $\sum:\mathbb{R}^m\rightarrow \mathbb{R}^n$； 

 旋转/镜像 $V:\mathbb{R}^n\rightarrow \mathbb{R}^n$。 
```
我们假设 m=n以及 U 和 V 都是旋转矩阵，则这个过程可以表示为：
    1. V首先将（可能是退化的）单位球旋转（旋转标准正交基）， 
    2. 而后经由 Σ将单位圆缩放拉伸成椭圆（超空间中的超椭球）， 
    3. 再经由 U 将超椭球在空间中旋转。 
       这个超椭球的各个半轴的长度， 就是矩阵 M 的奇异值，也就是矩阵 Σ 对角线上的元素。 

SVD 分解至少有两方面作用：
分析了解原矩阵的主要特征和携带的信息（取若干最大的奇异值），这引出了主成分分析（PCA）；
丢弃忽略原矩阵的次要特征和携带的次要信息（丢弃若干较小的奇异值），这引出了信息有损压缩、矩阵低秩近似等话题。
```










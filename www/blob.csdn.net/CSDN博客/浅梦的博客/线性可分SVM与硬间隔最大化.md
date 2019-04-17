# 线性可分SVM与硬间隔最大化 - 浅梦的博客 - CSDN博客





2017年08月24日 15:57:09[浅梦s](https://me.csdn.net/u012151283)阅读数：1711








# 线性可分支持向量机

定义

> 
给定线性可分训练数据集，通过间隔最大化或等价求解相应凸二次规划问题学习得到的分离超平面为 
$w^* \cdot x+b^*=0$

  以及相应的分类决策函数 
$f(x)=sign(w^* \cdot x+b^*)$

  称为线性可分支持向量机。
# 函数间隔与几何间隔

## 函数间隔

> 
对于给定的训练数据集T和超平面(w,b)，定义超平面(w,b)关于样本点$(x_i,y_i)$的函数间隔为 
$\hat\gamma_i=y_i(w\cdot x_i+b)$

  定义超平面(w,b)关于训练数据集T的函数间隔为超平面(w,b)关于T中所有样本点$(x_i,y_i)$的**函数间隔**之最小值，即 
$\hat\gamma=\min \limits_{i=1,...,N}  \hat\gamma_i$
函数间隔可以表示分类预测的正确性和确信度，但是在选择分离超平面时，只有函数间隔还不够。因为成比例地改变w和b，超平面没有改变，函数间隔却变为2倍。

## 几何间隔

对分离超平面的法向量w进行约束，使得间隔是确定的，这时就成了几何间隔。 
$\gamma_i=y_i(\frac{w\cdot x_i+b}{||w||})$
$\gamma = \min\limits_{i=1,...,N}\gamma_i$

超平面关于样本点的几何间隔一般是实例点到超平面的带符号距离，当样本点被正确分类时，就是距离。
# 间隔最大化

## 最大间隔分离超平面

求一个几何间隔最大的分离超平面。 


$\begin{align*}&\max\limits_{w,b}{\gamma}\\&s.t.\space y_i(\frac{w\cdot x_i+b}{||w||})\ge\gamma,i=1,2,...,N\end{align*}$

根据几何间隔与函数间隔关系，改写为 


$\begin{align*}&\max\limits_{w,b}{\frac{\hat\gamma}{||w||}}\\&s.t.\space y_i(w\cdot x_i+b)\ge\hat\gamma,i=1,2,...,N\end{align*}$

函数间隔$\hat\gamma$的取值并不影响最优化问题的解。取$\hat\gamma=1$,并将最大化$\frac{1}{||w||}$转化为等价的最小化$\frac{1}{2}||w||^2$。 

得到以下的线性可分SVM的最优化问题。 


$\begin{align*}&\min\limits_{w,b}\frac{1}{2}||w||^2\text{   (7.13)}\\&s.t.\space y_i(w\cdot x_i+b)-1\ge0\text{   (7.14)}\end{align*}$

这是一个凸优化问题同时是一个凸二次规划问题。
## 支持向量和间隔边界

在线性可分情况下，训练数据集的样本点中与分离超平面距离最近的样本点的实例称为支持向量。也就是使得不等式约束$ y_i(w\cdot x_i+b)-1\ge0$等号成立的点，即 
$y_i(w\cdot x_i+b)-1=0$

 正例点和负例点支持向量所在的间隔边界之间的距离为$\frac{2}{||w||}$。 

 在决定分离超平面时，只有支持向量起作用。
# 学习的对偶算法

对于原始最优化问题，应用拉格朗日对偶性，通过求解对偶问题的到原始问题的最优解，这就是线性可分SVM的对偶算法。 

 优点 

 1. 对偶问题往往更容易求解 

 2. 自然引入和函数，进而推广到非线性分类问题
## 拉格朗日函数

$L(w,b,a)=\frac{1}{2}||w||^2-\sum_{i=1}^Na_iy_i(w\cdot x_i+b)+\sum_{i=1}^Na_i\text{  (7.18)}$，其中$a_i\ge0$

## 拉格朗日对偶问题

根据拉格朗日对偶性，原始问题的对偶问题是极大极小问题： 
$\max\limits_a\min\limits_{w,b}L(w,b,a)$

为了得到对偶问题的解，先求L对w,b的极小，再求对a的极大 

1. 求$\min\limits_{w,b}L(w,b,a)$


$\begin{align*}\nabla_wL(w,b,a)&=w-\sum_{i=1}^Na_iy_ix_i=0\\\nabla_bL(w,b,a)&=-\sum_{i=1}^Na_iy_i=0\\\text{得}\\w&=\sum_{i=1}^Na_iy_ix_i\text{     (7.19)}\\\sum_{i=1}^N&a_iy_i=0\text{         (7.20)}\end{align*}$

将式(7.19)带入拉格朗日函数(7.18)，并利用式(7.20)，得 
$L(w,b,a)=-\frac{1}{2}\sum\limits_{i=1}^N\sum\limits_{j=1}^Na_ia_jy_iy_j(x_i\cdot x_j)+\sum\limits_{i=1}^Na_i$

即 
$\min\limits_{w,b}L(w,b,a)=-\frac{1}{2}\sum\limits_{i=1}^N\sum\limits_{j=1}^Na_ia_jy_iy_j(x_i\cdot x_j)+\sum\limits_{i=1}^Na_i$

2. 求$\min\limits_{w,b}L(w,b,a)对a得极大，即是对偶问题$


$\begin{align*}&\max\limits_a -\frac{1}{2}\sum\limits_{i=1}^N\sum\limits_{j=1}^Na_ia_jy_iy_j(x_i\cdot x_j)+\sum\limits_{i=1}^Na_i\\&s.t. \sum\limits_{i=1}^Na_iy_i=0\\&a_i\ge0,i=1,2,..,N\end{align*}$

将上式目标函数由求极大转成求极小，得到下面与之等价得对偶最优化问题。 


$\begin{align*}&\min\limits_a \frac{1}{2}\sum\limits_{i=1}^N\sum\limits_{j=1}^Na_ia_jy_iy_j(x_i\cdot x_j)-\sum\limits_{i=1}^Na_i\text{   (7.22)}\\&s.t. \sum\limits_{i=1}^Na_iy_i=0\text{   (7.23)}\\&a_i\ge0,i=1,2,..,N\text{   (7.24)}\end{align*}$

由于原始问题(7.13)-(7.14)满足**弱化的Slater条件**，对偶问题(7.22)-(7.24)的最优值和原始问题最优值相同，设$w^*,b^*,$是原始问题最优解，$a^*$是对偶问题最优解，根据**KKT条件**，$w^*,b^*,a^*$满足： 


$\begin{align*}1-y_i(w^*\cdot x_i+b^*)&\le0\\a_i^*&\ge0\\a_i^*\cdot [1-y_i(w^*\cdot x_i+b^*)]&=0\\\nabla_wL(w^*,b^*,a^*)&=w^*-\sum_{i=1}^Na_i^*y_ix_i\\&=0\text{    (7.27)}\\\nabla_bL(w^*,b^*,a^*)=\sum_{i=1}^Na_i^*y_i&=0\\\end{align*}$

由此得 
$w^*=\sum\limits_ia_i^*y_ix_i\text{(7.25)}$

其中至少由一个$a_j^*>0$（反证法,假设$a^*=0$，由式(7.27)知$w^*=0$,而$w^*=0$不是原始最优化问题(7.13)-(7.14)的解，产生矛盾），对此j有 
$y_j(w^*\cdot x_j+b^*)-1=0\text{(  7.28)} \text{根据互补松弛性}$

将(7.27)带入(7.28)，并有$y_j^2=1$，即得 
$b^*=y_j-\sum\limits_{i=1}^Na_i^*y_i(x_i\cdot x_j)$

由此，分离超平面可以写成 
$\sum\limits_{i=1}^N a_i^*y_i(x\cdot x_i)+b^*=0$

决策函数 
$f(x)=sign(\sum\limits_{i=1}^N a_i^*y_i(x\cdot x_i)+b^*)\text{  (7.30)}$

也就是说，分类决策函数只依赖于输入x和训练样本输入的内积。式(7.30)称为线性可分SVM的对偶形式。
综上，对于给定的线性可分的数据集，可以先求对偶问题(7.22)-(7.24)的解$a^*$；再利用式(7.25)和(7.26)求得原始问题的解$w^*,b^*$。

## 支持向量

训练集中对应于$a_i^*>0$的样本点$(x_i,y_i)$的实例$x_i$称为支持向量。 

由KKT互补松弛条件可以推得，支持向量一定在间隔边界上。

参考文献

> 
《统计学习方法》第7章














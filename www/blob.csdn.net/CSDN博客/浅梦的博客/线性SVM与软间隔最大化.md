# 线性SVM与软间隔最大化 - 浅梦的博客 - CSDN博客





2017年08月25日 00:08:25[浅梦s](https://me.csdn.net/u012151283)阅读数：1520








# 线性支持向量机

## 松弛变量和惩罚代价

线性不可分意味着某些样本点$(x_i,y_i)$不能满足函数间隔大于等于1的约束条件。可以对每个样本点$(x_i,y_i)$引入一个松弛变量$\xi_i\ge0$，使函数间隔加上松弛变量大于等于1.这样，约束条件变为 
$y_i(w_i\cdot x_i+b)\ge1-\xi_i$

同时，对每个松弛变量$\xi_i$，支付一个代价$\xi_i$，目标函数变成 
$\frac{1}{2}||w||^2+C\sum\limits_{i=1}^N\xi_i\text{    (7.31)}$

这里C>0称为惩罚参数，C较大对误分类惩罚增大。最小化 

目标函数(7.31)包含两层含义：使间隔尽量大，同时使得误分类点个数尽量少。
## 原始问题

线性不可分支持向量机的学习问题变成如下凸二次规划问题（原始问题） 


$\begin{align*}\min\limits_{w,b,\xi}\frac{1}{2}||w||^2+C\sum\limits_{i=1}^N\xi_i\text{(7.32)}\\s.t.y_i(w\cdot x_i+b)\ge1-\xi_i,i=1,...,N\text{(7.33)}\\\xi_i\ge0,i=1,...N\text{(7.33)}\end{align*}$

原始问题(7.32)-(7.34)是一个凸二次规划问题。可以证明w的解是唯一的，但b的解可能不唯一，而是存在于一个区间。
# 学习的对偶算法

## 对偶问题

原始问题(7.32)-(7.34)的对偶问题是 


$\begin{align*}\min\limits_a\frac{1}{2}\sum\limits_{i=1}^N\sum\limits_{j=1}^Na_ia_jy_iy_j(x_i\cdot x_j)-\sum\limits_{i=1}^Na_i\text{(7.37)}\\s.t. \sum\limits_{i=1}^Na_iy_i=0\text{(7.38)}\\0\le a_i\le C,i=1,...,N\text{(7.39)}\end{align*}$

## 拉格朗日函数

$L(w,b,a)=\frac{1}{2}||w||^2+C\sum\limits_{i=1}^N\xi_i-\sum_{i=1}^Na_i(y_i(w\cdot x_i+b)-1+\xi_i)-\sum_{i=1}^N\mu_i\xi_i\text{  (7.40)}$，其中$a_i\ge0,\mu_i\ge0$

## 拉格朗日对偶问题

根据拉格朗日对偶性，原始问题的对偶问题是极大极小问题： 
$\max\limits_{a,\mu}\min\limits_{w,b,\xi}L(w,b,\xi,a,\mu)$

为了得到对偶问题的解，先求L对$w,b,\xi$的极小，再求对$a,\mu$的极大 

1. 求$\min\limits_{w,b,\xi}L(w,b,\xi,a,\mu)$


$\begin{align*}\nabla_wL(w,b,\xi,a,\mu)&=w-\sum_{i=1}^Na_iy_ix_i=0\\\nabla_bL(w,b,\xi,a,\mu)&=-\sum_{i=1}^Na_iy_i=0\\\nabla_{\xi_i}L(w,b,\xi,a,\mu)&=C-a_i-\mu_i=0\\\text{得}\\w&=\sum_{i=1}^Na_iy_ix_i\text{     (7.41)}\\\sum_{i=1}^N&a_iy_i=0\text{         (7.42)}\\C-&a_i-\mu_i=0\text{  (7.43)}\end{align*}$

将式(7.41)-(7.43)带入拉格朗日函数(7.40)，得 
$\min\limits_{w,b,\xi}L(w,b,\xi,a,\mu)=-\frac{1}{2}\sum\limits_{i=1}^N\sum\limits_{j=1}^Na_ia_jy_iy_j(x_i\cdot x_j)+\sum\limits_{i=1}^Na_i$

2. 求$\min\limits_{w,b,\xi}L(w,b,\xi,a,\mu)对a的极大，即是对偶问题$


$\begin{align*}&\max\limits_a -\frac{1}{2}\sum\limits_{i=1}^N\sum\limits_{j=1}^Na_ia_jy_iy_j(x_i\cdot x_j)+\sum\limits_{i=1}^Na_i\text{(7.44)}\\s.t.& \sum\limits_{i=1}^Na_iy_i=0\text{(7.45)}\\&C-a_i-\mu_i=0\text{(7.46)}\\&\mu_i\ge0,\text{(7.47)}\\&a_i\ge0,i=1,2,..,N\text{(7.48)}\end{align*}$

将对偶最优化问题(7.44)-(7.48)进行变换；利用灯饰约束(7.46)消取$\mu_i$，只留下变量$a_i$，并将约束(7.46)-(7.48)写成 
$0\le a_i\le C$

将上式目标函数由求极大转成求极小，得到对偶问题(7.37)-(7.39)。
由于原始问题(7.32)-(7.34)满足弱化的Slater条件，对偶问题(7.37)-(7.39)的最优值和原始问题最优值相同，设$w^*,b^*,\xi^*$是原始问题最优解，$a^*,\mu^*$是对偶问题最优解，根据KKT条件，$w^*,b^*,\xi^*,a^*,\mu^*$满足： 


$\begin{align*}\nabla_wL(w,b,\xi,a,\mu)&=w-\sum_{i=1}^Na_iy_ix_i=0\text{(7.52)}\\\nabla_bL(w,b,\xi,a,\mu)&=-\sum_{i=1}^Na_iy_i=0\\\nabla_{\xi_i}L(w,b,\xi,a,\mu)&=C-a_i-\mu_i=0\\1-\xi^*-y_i(w^*\cdot x_i+b^*)&\le0\\\xi^*&\ge 0\\a_i^*&\ge0\\\mu_i^*&\ge0\\a_i^*\cdot [1-\xi^*-fy_i(w^*\cdot x_i+&b^*)]=0\text{(7.53)}\\\mu_i^*\xi_i^*&=0\text{(7.54)}\end{align*}$

由式(7.53)-(7.54)知，若存在 
$0<a_j^*<C$，则 


$\begin{align*}0<C-\mu_i<C \Rightarrow\mu_i>0\\\mu_i\xi_i=0\Rightarrow\xi_i=0\end{align*}$
$y_j(w^*\cdot x_j+b^*)-1=0$
由此，分离超平面可以写成 
$\sum\limits_{i=1}^N a_i^*y_i(x\cdot x_i)+b^*$

决策函数 
$f(x)=sign(\sum\limits_{i=1}^N a_i^*y_i(x\cdot x_i)+b^*)\text{  (7.56)}$

由于对任一适合条件$0<a_j^*<C$的$a_j^*$，都可求出$b^*$。从理论上，原始问题对b的解可能不唯一，然而在实际应用中，往往只会出现算法叙述的情况。
# 支持向量

在线性不可分的情况下，对偶问题中对应于$a_i^*>0$的样本点$(x_i,y_i)$的实例$x_i$称为支持向量（软间隔的支持向量）。 

实例$x_i$到间隔边界的距离$\frac{\xi_i}{||w||}$。 

软间隔的支持向量$x_i$或者在间隔边界上，或者在间隔边界于分离超平面之间，或者在分离超平面误分一侧。 

若$a_i^*<C$,则$\xi_i=0$，支持向量$x_i$恰好落在间隔边界上； 

若$a_i^*=C,0<\xi_i<1,$则分类正确，$x_i$在间隔边界与分离超平面之间； 

若$a_i^*=C,\xi_i=1$,则$x_i$在分离超平面上； 

若$a_i^*=C，\xi_i>1，$则$x_i$位于分离超平面误分一侧。
# 合页损失函数

线性支持SVM还有另外一种解释，就是最小化以下目标函数 
$\sum\limits_{i=1}^N[1-y_i(w\cdot x_i+b)]_++\lambda||w||^2$

目标函数的第1项是经验损失或经验风险，函数 
$L(y(w\cdot x+b))=[1-y(w\cdot x+b)]_+$

称为合页损失函数。 

当样本点被正确分类且函数间隔（确信度）$y_i(w\cdot x_i+b)$大于1时，损失是0，否则损失是$1-y_i(w\cdot x_i+b)$。所以即使样本点被正确分类，损失也可能不是0。 

下面证明线性支持向量机原始最优化问题： 


$\begin{align*}\min\limits_{w,b,\xi}\frac{1}{2}||w||^2+C\sum\limits_{i=1}^N\xi_i\\s.t.y_i(w\cdot x_i+b)\ge1-\xi_i,i=1,...,N\text{(7.61)}\\\xi_i\ge0,i=1,...,N\text{(7.62)}\end{align*}$

等价于最优化问题 
$\min\limits_{w,b}\sum\limits_{i=1}^N[1-y_i(w\cdot x_i+b)]_++\lambda||w||^2$
**证明**

令$[1-y_i(w\cdot x_i+b)]_+=\xi_i\text{(7.64)}$

则$\xi_i\ge0$，式(7.62)成立。由式(7.64)， 

当$1-y_i(w\cdot x_i+b)>0$时，有$y_i(w\cdot x_i+b)=1-\xi_i$； 

当$1-y_i(w\cdot x_i+b)\le0$时,$\xi_i=0$，有$y_i(w\cdot x_i+b)\ge1-\xi_i$。故式(7.61)成立。于是$w,b,\xi$满足约束条件(7.61)-(7.62)。 

由式(7.64)有， 
$[1-y_i(w\cdot x_i+b)]_+=[\xi_i]_+=\xi_i$,所以最优化问题(7.63)可写成 


$\begin{align*}\min\limits_{w,b}\sum_{i=1}^N\xi_i+\lambda||w||^2\\\text{若取}\lambda=\frac{1}{2C}，则\\\min\limits_{w,b}\frac{1}{C}(\frac{1}{2}||w||^2+C\sum_{i=1}^N\xi_i)\end{align*}$

与式(7.60)等价。
参考文献

> 
《统计学习方法》第7章












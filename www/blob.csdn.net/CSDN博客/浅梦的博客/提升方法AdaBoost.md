# 提升方法AdaBoost - 浅梦的博客 - CSDN博客





2017年08月27日 16:12:38[浅梦s](https://me.csdn.net/u012151283)阅读数：361








![这里写图片描述](https://img-blog.csdn.net/20170828001819206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

提升(boosting)方法是一种可以将弱学习器提升为强学习器的算法。在分类问题中，它通过改变训练样本的权重，使得先前学习器做错的训练样本在后续受到更多关注，然后基于调整后的样本分布来训练下一个基学习器，并将这些分类器进行过线性组合，提高分类的性能。
# 提升方法AdaBoost算法
- 在每一轮如何改变训练数据的权值或概率分布 

AdaBoost **提高那些被前一轮弱分类器错误分类的样本的权值**，而降低那些被正确分类样本的权值。
- 如何将弱分类器组合成一个强分类器 

AdaBoost采取加权多数表决的方法。**加大分类误差率小的弱分类器的权值**，减小分类误差率大的弱分类器的权值。

## AdaBoost算法

算法AdaBoost 

输入：训练数据集T，$y\in{(-1,1)}$；弱学习算法 

输出：最终分类器$G(x)$

1. **初始化训练数据的权值分布**
$D_1=(w_{11},...,w_{1N}),w_{1i}=\frac{1}{N},i=1,...,N$

2. **对m=1，2，…,M**
（a）**使用具有权值分布$D_m$的训练数据学习，得到基本分类器 **
$G_m(x):x\rightarrow{(-1,1)}$

   （b）**计算$G_m(x)$在训练集上的分类误差率**
$e_m=\sum\limits_{i=1}^NP(G_m(x_i)\ne y_i)=\sum\limits_{i=1}^Nw_{mi}I(G_m(x_i)\ne y_i)$

  （c）**计算$G_m(x)$的系数**
$a_m=\frac{1}{2}\log\frac{1-e_m}{e_m}\text{(8.2)}$

  这里的对数是自然对数 

  （d）**更新训练集的权值分布**


$\begin{align*}  D_{m+1}=(w_{m+1,1},...,w_{m+1,N})\\  w_{m+1,1}=\frac{w_{mi}}{Z_m}exp(-a_my_iG_m(x_i)),i=1,...,N\text{(8.4)}\end{align*}$

这里，$Z_m$是规范化因子 
$Z_m=\sum\limits_{i=1}^Nw_{mi}exp(-a_my_iG_m(x_i))\text{(8.5)}$

它使$D_{m+1}$成为一个概率分布 

3. **构建基分类器的线性组合**
$f(x)=\sum\limits_{m=1}^Na_mG_m(x)$

得到最终分类器 
$G(x)=sign(f(x))=sign(\sum\limits_{m=1}^Na_mG_m(x))$
## 算法说明

$G_m(x)$在加权的训练数据集上的分类误差率是被$G_m(x)$误分类样本的权值之和，由此可以看出数据权值分布$D_m$与基本分类器$G_m(x)$的分类误差率的关系。 

由式(8.2)知，当$e_m\le\frac{1}{2}$时，$a_m\ge0$，并且$a_m$随着$e_m$的减小而增大，所以分类误差率越小的基本分类器在最终分类器中的作用越大。 

式(8.4)可以写成: 


$w_{m+1,i}=\begin{cases}\frac{w_{mi}}{Z_m}e^{-a_m},G_m(x_i)=y_i\\\frac{w_{mi}}{Z_m}e^{a_m},G_m(x_i)\ne y_i\end{cases}$

由此可知，被基分类器误分类样本的权值得以扩大，而被正确分类样本的权值却得以缩小。两相比较，误分类样本的权值被放大$e^{2a_m}=\frac{1-e_m}{e_m}$倍。误分类样本在下一轮学习中起更大的作用。 

不改变训练数据，不断改变训练数据权值的分布，使得训练数据在基本分类器的学习中起不同的作用，这是AdaBoost的一个特点。 

最后线性组合$f(x)$实现了M个基分类器的加权表决。系数$a_m$之和并不为1。$f(x)$的符号决定实例x的类，$f(x)$的绝对值表示分类的确信度。 

利用基本分类器的线性组合构建最终分类器是AdaBoost的另一特点。
# AdaBoost算法的训练误差分析

![这里写图片描述](https://img-blog.csdn.net/20171206123920334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

定理8.1后半部分推导过程中第2个等号后出现的$w_{1i}$是因为在第一轮训练时，各个样本的权重均为$\frac{1}{N}$。 

上述定理说明，可以在每一轮选取适当的$G_m$使得$Z_m$最小，从而使得训练误差下降最快。对二类分类问题，有如下结果。 
![这里写图片描述](https://img-blog.csdn.net/20171206124506537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后一个不等式使用了$1-x\le e^{-x}$
$Z_m = (1-4\gamma_m^2)^{\frac{1}{2}}\le (e^{-4\gamma_m^2})^{\frac{1}{2}}=e^{-2\gamma_m^2}$

所以 
$\prod_{m=1}^MZ_m\le e^{-2\sum_{m=1}^M\gamma_m^2}$
# AdaBoost算法的解释

AdaBoost算法是模型为加法模型，损失函数为指数函数，学习算法为前向分布算法时的二类分类学习算法。

## 前向分步算法

考虑加法模型(additive model) 
$f(x)=\sum\limits_{m=1}^M\beta_mb(x;\gamma_m)$

在给定训练数据和损失函数$L(y,f(x))$的条件下，学习加法模型$f(x)$成为经验风险最小化及损失函数最小化问题: 
$\min\limits_{\beta_m,\gamma_m}\sum\limits_{i=1}^NL(y_i,\sum\limits_{m=1}^M\beta_mb(x_i;\gamma_m))$

前向分步算法(forward stagewise algorithm)求解这一优化问题的想法是：因为学习是加法模型，如果能够从前向后，每一步只学习一个 基函数及其系数，逐步逼近优化目标函数式(8.14)那么就可以简化优化的复杂度。具体地，每步只需优化如下损失函数： 
$\min\limits_{\beta,\gamma}\sum\limits_{i=1}^NL(y_i,\beta b(x_i;\gamma))$
![这里写图片描述](https://img-blog.csdn.net/20170827160858586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 前向分步算法与AdaBoost

参考资料

> 
《统计学习方法》第8章











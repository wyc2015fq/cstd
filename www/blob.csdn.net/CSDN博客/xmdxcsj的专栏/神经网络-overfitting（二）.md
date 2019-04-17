# 神经网络-overfitting（二） - xmdxcsj的专栏 - CSDN博客





2015年12月13日 21:32:16[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1422








## 一、概况

### （一）、范数

由于正则化的定义需要使用到**范数（norm）**的概念，这里简单介绍一下范数的概念[1](#fn:footnote)。

#### 1.物理意义

范数是对函数、向量和矩阵定义的一种度量形式，可以用来测量两个函数、向量或者矩阵之间的距离。

#### 2.数学定义

想要成为范数，需要满足一下三个性质：
- *非负性*
$\forall X\in R^n，有\lVert w \rVert\geq0，当且仅当X=0时，\lVert w \rVert=0$
- *齐次性*
$\forall X\in R^n，\alpha\in R，有\lVert \alpha w \rVert=|\alpha|\lVert w \rVert$
- *三角不等式*
$\forall X,Y\in R^n，有\lVert X+Y \rVert \leq \lVert X \rVert+\lVert Y \rVert$

#### 3.范数分类
- 
*向量范数*

向量$X=(x_1,x_2,...,x_n)^T$的$L_P$范数定义为： 


$\lVert X \rVert_p=(\sum_{i=1}^n|x_i|^p)^{1/p}, 1\leq p\leq+\infty$

我们经常使用的是$p=1,2,\infty$三种向量范数： 


$\lVert X \rVert_1=\sum_{i=1}^{n}|x_i|$


$\lVert X \rVert_2=\sqrt{\sum_{i=1}^{n}|x_i|^2}$


$\lVert X \rVert_{\infty}=\max_{1\leq i\leq n}{|x_i|}$- 
*矩阵范数*

对于方阵$A$,常用的矩阵范数有三种 


$\lVert A \rVert_1=\max_{1\leq j\leq n}{\sum_{i=1}^{n}|a_{ij}|}$


$\lVert A \rVert_{\infty}=\max_{1\leq i\leq n}{\sum_{j=1}^{n}|a_{ij}|}$


$\lVert A \rVert_2=\sqrt{\lambda_1}(\lambda_1是A^TA的最大特征值)$
### （二）、正则化

正则化[2](#fn:footnote2)是为了解决过拟合问题而引入的一种策略，包括L1 regularization和L2 regularization。 

假设损失函数为$E(X,Y)$，权重矩阵为$w$，为了避免过拟合问题，将损失函数更正为： 


$E(X,Y)+\alpha \lVert w \rVert$

上式中的$\lVert w \rVert$表示L1范数或者L2范数，$\alpha$是由交叉验证得到的经验参数。 

上面的过程称为正则化。
## 二、L2 regularization

在神经网络训练中，经常使用的是L2正则化[3](#fn:footnote3)。L2正则化以后的损失函数形式如下： 


$C=C_0+\frac{\lambda}{2n}\sum_ww^2$

其中$C_0$表示原始的损失函数，$n$表示训练样本的个数，$\lambda$称为*regularization parameter*。 

那么，偏导数变为： 


$\frac{\partial C}{\partial w}=\frac{\partial C_0}{\partial w}+\frac{\lambda}{n}w$


$\frac{\partial C}{\partial b}=\frac{\partial C_0}{\partial b}$

由上式可知，正则化的引入只对权重w有影响，对偏置b没有影响。 

设学习率为$\eta$，权重更新公式变为： 


$w=w-\eta\frac{\partial C_0}{\partial w}-\frac{\eta \lambda}{n}w=(1-\frac{\eta \lambda}{n})w-\eta\frac{\partial C_0}{\partial w}$


$b=b-\eta\frac{\partial C_0}{\partial b}$

由上式可以看到权重w的系数变为了$1-\frac{\eta \lambda}{n}$，这种$|w|$的减小称为**weight decay**。 

w为正，正则化使得w变小；w为负，正则化是得w变大。整体上会使得网络的权重w趋向于0。为什么趋近于0，会减弱过拟合呢？ 

直观上的解释是：当过拟合发生的时候，模型过度匹配训练数据，导致模型变得更为复杂，变化更为剧烈。而正则化可以对权重施加一种额外的约束，降低了模型的复杂性。 

举个例子简单说明一下，假设模型为 


$y=w_1x+w_2x^2+w_3x^3$

模型的梯度可以表示为 


$|\frac{\partial y}{\partial x}|=|w_1|+2*|w_2|*|x|+3*w_3*|x|^2$

过拟合意味着y随着x的变化更为剧烈，即上式变大。 

而正则化的作用是减小$|w_1|$和$|w_2|$，会使得上式变小，故正则化可以减弱过拟合。
## 三、L1 regularization

在神经网络训练中，也可以使用L1正则化。L1正则化以后的损失函数形式如下： 


$C=C_0+\frac{\lambda}{n}\sum_w|w|$

同理，权重的更新公式可以表示如下： 


$w=w-\eta\frac{\partial C_0}{\partial w}-\frac{\eta \lambda}{n}sgn(w)$

和L2类似，L1的作用也是减小权重的绝对值，使得权重趋近于0。
## 四、L1和L2比较

### （一）、速度

根据以上讨论，当|w|很大的时候，L2的权重衰减速度更快，当|w|很小的时候，L2的速度更慢。

### （二）、稀疏性

模型中为0的参数越多，稀疏性约高。一方面，稀疏性可以带来计算上的方便；另一方面，稀疏性越高，模型越容易解释（因为输出只和部分参数有关）。 

根据[4](#fn:footnote4)，L1相比于L2具有更好的稀疏特性。

- 《数值计算方法和算法》 张韵华 奚梅成等 [↩](#fnref:footnote)
- [https://en.wikipedia.org/wiki/Regularization_(mathematics)](https://en.wikipedia.org/wiki/Regularization_(mathematics))[↩](#fnref:footnote2)
- [http://neuralnetworksanddeeplearning.com/chap3.html#overfitting_and_regularization](http://neuralnetworksanddeeplearning.com/chap3.html#overfitting_and_regularization)[↩](#fnref:footnote3)
- [http://freemind.pluskid.org/machine-learning/sparsity-and-some-basics-of-l1-regularization/](http://freemind.pluskid.org/machine-learning/sparsity-and-some-basics-of-l1-regularization/)[↩](#fnref:footnote4)












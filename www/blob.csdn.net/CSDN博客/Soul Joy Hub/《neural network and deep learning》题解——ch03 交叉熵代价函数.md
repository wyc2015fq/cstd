# 《neural network  and deep learning》题解——ch03 交叉熵代价函数 - Soul Joy Hub - CSDN博客

2017年07月13日 19:49:11[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1495
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/75091283](http://blog.csdn.net/u011239443/article/details/75091283)

## 问题一

> 
验证 $σ ′ (z) = σ(z)(1 − σ(z))$。

$\large \color{blue}{σ (z) = \frac{1}{1+e^{-z}}}$

$\large \color{blue}{σ'(z) = (\frac{1}{1+e^{-z}})' = \frac{e^{-z}}{(1+e^{-z})^2} =\frac{1}{1+e^{-z}} \frac{e^{-z}}{1+e^{-z}}  =  σ (z)(1- σ (z)  )}$

## 问题二

> 
一个⼩问题就是刚接触交叉熵时，很难⼀下⼦记住那些诸如 y 和 a 的表达式对应的角色。又比如说，表达式的正确形式是$−[y lna(1−y)ln(1−a)]$ 还是 $−[alny+(1−a)ln(1−y)]$。在 y = 0 或者 1 的时候第二个表达式的结果怎样？这个问题会困扰第一个表达式吗？为什么？

在 y = 0 或者 1 的时候，`In`中的值为0，第二个表达式非法。这个问题不会困扰第一个表达式，因为a在`（0，1）`，只会趋近0或1，所以`a`和`1-a`不会等于`0`

## 问题三

> 
在对单个神经元讨论中,我们指出如果对所有的训练数据有 σ(z) ≈ y,交叉熵会很小。这个论断其实是和 y 只是等于 1 或者 0 有关。这在分类问题一般是可行的,但是对其他的问题(如回归问题)y 可以取 0 和 1 之间的中间值的。证明,交叉熵对所有训练输入在σ(z) = y 时仍然是最小化的。此时交叉熵的表示是: 
$C = −\frac{1}{n} ∑[y ln y + (1 − y) ln(1 − y)]$

  而其中 $−[y ln y + (1 − y) ln(1 − y)]$ 有时候被称为二元熵。

y 不只是等于 1 或者 0，我们可以把y看成一个常量，对C关于a进行求导。

$\large \color{blue}{ \frac{∂C}{∂a}= \frac{1}{n} ∑_x[-\frac{y}{a} + \frac{1 − y}{ 1 − a}]}$

$\large \color{blue}{ 以简化形式表示：\frac{∂C}{∂a}= \frac{a-ay-y+ay}{a(1-a)} = \frac{a-y}{a(1-a)} }$

$\large \color{blue}{由于 a \in (0,1)，所以 a(1-a) > 0}$

$\large \color{blue}{所以a在(0,y) 上C单调递减;在[y,1)上，C单调递增。}$

$\large \color{blue}{即交叉熵对所有训练输入在σ(z) = y 时仍然是最小化的}$

## 问题四

> 
用上一章的定义符号,证明对二次代价函数,关于输出层的权重的偏导数为 
$\frac{∂C}{∂w^L{jk} } = \frac{1}{n}∑_xa_k^{L-1} (a_j − y_j )σ ′ (z_j^L )$

$\large \color{blue}{\frac{∂C}{∂w^L{jk} } = \frac{1}{n}∑_x (a_j − y_j )σ ′ (z_j^L )\frac{∂a_j}{∂w^L{jk}}}  $

$\large \color{blue}{= \frac{1}{n}∑_x (a_j − y_j )σ ′ (z_j^L )\frac{∂(a_k^{L-1}w^L{jk} + b^L_j)}{∂w^L{jk}}}  $

$\large \color{blue}{= \frac{1}{n}∑_xa_k^{L-1} (a_j − y_j )σ ′ (z_j^L )}  $

> 
项 $σ ′ (z _j^L ) $会在一个输出神经元困在错误值时导致学习速度的下降。证明对于交叉熵代价函数,针对一个训练样本 x 的输出误差 δ L 为 
$δ^L = a^L − y$

$\large \color{blue}{δ^L = \frac{∂C}{∂a^L} σ ′ (z^L) }$
$\large \color{blue}{= -\frac{1}{n}\sum_x[\frac{y}{a^L} - \frac{1-y}{1-a^L}]a^L(1-a^L)}  $
$\large \color{blue}{= -\frac{1}{n}\sum_x[y(1-a^L)-(1-y)a^L] }$
$\large \color{blue}{= \frac{1}{n}\sum_x[a^L-y] }$

$\large \color{blue}{所以简化表示：δ^L = a^L − y}$

> 
使用这个表达式来证明关于输出层的权重的偏导数为 
$\frac{∂C}{∂w^L{jk} } = \frac{1}{n}∑_xa_k^{L-1} (a_j − y_j )$

由（BP4）得 
$\large \color{blue}{\frac{∂C}{∂w^L{jk} }  = a_k^{l-1}δ^L_j}$
$\large \color{blue}{=a_k^{l-1}(a^L_j-y_j) }$
$\large \color{blue}{=  \frac{1}{n}∑_xa_k^{L-1} (a_j − y_j )}$

## 问题五

> 
假设我们有一个多层多神经元网络,最终输出层的神经元都是线性神经元,输出不再是 S 型函数作用的结果,而是 $a^L_j = z^j$ 。证明 

  如果我们使用二次代价函数,那么对单个训练样本 x 的输出误差就是 
$δ^L = a^L − y$

$\large \color{blue}{δ^L = \frac{∂C}{∂a^L} σ ′ (z^L) }$
$\large \color{blue}{ = \frac{∂C}{∂a^L} }$
$\large \color{blue}{= -\sum_j(y_j-a_j^L)}  $
$\large \color{blue}{= a^L - y}  $

> 
类似于前一个问题,使用这个表达式来证明关于输出层的权重和偏置的偏导数为 
$\frac{∂C}{∂w^L{jk} } = \frac{1}{n}∑_xa_k^{L-1} (a_j − y_j )$
$\frac{∂C}{∂b^L{j} } = \frac{1}{n}∑_x (a_j − y_j )$

由（BP4）得 
$\large \color{blue}{\frac{∂C}{∂w^L{jk} }  = a_k^{l-1}δ^L_j}$
$\large \color{blue}{=a_k^{l-1}(a^L_j-y_j) }$
$\large \color{blue}{=  \frac{1}{n}∑_xa_k^{L-1} (a_j^L − y_j )}$

由（BP3）得 
$\large \color{blue}{\frac{∂C}{∂b^L{j} }  = δ^L_j}$
$\large \color{blue}{=a^L - y }$
$\large \color{blue}{= \frac{1}{n}∑_x (a_j^L − y_j )}$

## 问题六

> 
我们已经深入讨论了使用二次代价函数的网络中在输出神经元饱和时候学习缓慢的问题,另一个可能会影响学习的因素就是在方程 (61) 中的 x j 项。由于此项,当输入 x j 接近 0 时,对应的权重 w j 会学习得相当缓慢。解释为何不可以通过改变代价函数来消除 x j 项的影响。

我们本身就像想通过$x_j$的数据输入来学习，如歌可以通过改变代价函数来消除$x_j$项的影响，那么训练数据对学习就无任何意义了。

## 问题七

> 
构造例子表明在使用 S 型输出层的网络中输出激活值 $a^L_j$ 的和并不会确保为 1。

设$\large \color{blue}{x = (0,1),w_1=(1,1),w_2=(1,1),b=(1,1)}$
$\large \color{blue}{σ(xw_1+b_0) = σ(1+1) = \frac{1}{1+e^{-2}}}$
$\large \color{blue}{σ(xw_2+b_0) = σ(1+1) = \frac{1}{1+e^{-2}}}$
$\large \color{blue}{\frac{2}{1+e^{-2} } \neq 1}$

## 问题八

> 
S 型层的一个好处是输出 $a^L_j$ 是对应带权输入 $a_j = σ(z_j )$ 的函数。解释为何对于柔性最大值层来说,并不是这样的情况:任何特定的输出激活值 $a_j$ 依赖所有的带权输入。

从公式(78)的分母部分可知，$a_j$以来所有的带权输入

## 问题九

> 
假设我们有一个使用柔性最大值输出层的神经网络,然后激活值 $a^L_j$已知。证明对应带权输入的形式为$ z_j^L = ln a^L_j + C$,其中常量 C 是独立于 j 的。

对公式（78）两边去对数，得：

$\large \color{blue}{Ina_j^L = z_j^L - In(\sum_ke^{z_k^l})}$

$\large \color{blue}{=> Ina_j^L + In(\sum_ke^{z_k^l}) = z_j^L }$

$\large \color{blue}{=>  z_j^L = ln a^L_j + C}$

## 问题十

> 
推导方程 (81) 和 (82)

待解

## 问题十一

> 
假设我们改变一下柔性最大值函数,使得输出激活值定义如下 
$a^L_j = \frac{e^{cz^L_j}}{\sum_ke^{cz^L_k}}$

  其中 c 是正的常量。注意 c = 1 对应标准的柔性最大值函数。但是如果我们使用不同的 c得到不同的函数,其本质上和原来的柔性最大值函数是很相似的。特别地,证明输出激活值也会形成一个概率分布,正如通常的柔性最大值函数。

$\large \color{blue}{\sum_ja^L_j =  \frac{\sum_je^{cz^L_j}}{\sum_ke^{cz^L_k}} = 1}$

> 
正如通常的柔性最大值函数。假设我们允许 c 足够大,比如说 c → ∞。那么输出激活值$ a^L_j$ 的极限值是什么?

将上题的式子中，分母分子各除以$e^{cz^L_j}$得：

$\large \color{blue}{lim_{c → +∞}a^L_j = lim_{c → +∞}\frac{1}{\sum_ke^{c(z^L_k-z^L_j)}}}$

若$z^L_j$是输入中的最大值，则$ a^L_j$ 的极限值为1，否则$ a^L_j$ 的极限值为0

## 问题十二

> 
为了应用在柔性最大值层的网络上,我们需要搞清楚最后一层上误差的表示 $δ_j^L ≡ \frac{∂C}{∂z_j^L}$ 。证明形式如下: 
$δ_j^L = a^L_j − y_j$

由（BP3）与 公式（81），可证明。

![这里写图片描述](https://img-blog.csdn.net/20170713195000936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# 《neural network  and deep learning》题解——ch02 反向传播 - Soul Joy Hub - CSDN博客

2017年07月09日 13:58:41[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：969
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/74859614](http://blog.csdn.net/u011239443/article/details/74859614)

# 2.4 反向传播的四个基本方程

> 
$\large \color{blue}{ (BP1)：δ ^L = ∇ _a C ⊙ σ ′ (z ^L )    }$
$\large \color{blue}{  (BP2):δ ^l = ((w ^{l+1} ) ^T δ ^{l+1} ) ⊙ σ ′ (z^ l )}$
$\large \color{blue}{  (BP3): \frac{∂C}{ ∂b_{lj} }    = δ_j^l}$
$\large \color{blue}{  (BP4): \frac{∂C}{ ∂w^l_{jk} }    = a_k^{l-1}δ_j^l}$

## 问题一：

> 
另一种反向传播方程的表示方式: 我已经给出了使用 Hadamard 乘积的反向传播的公式(尤其是 (BP1) 和 (BP2))。如果你对这种特殊的乘积不熟悉,可能会有一些困惑。下面还有一种表示方式,那就是基于传统的矩阵乘法,某些读者可能会觉得很有启发。(1)证明(BP1) 可以写成：

$\large \color{blue}{ δ^L = Σ ′ (z^L )∇_a C}$

其中$ Σ ′ (z^L ) $是一个方阵,其对⻆线的元素是$σ ′ (z_j^L )$,其他的元素均是 0。注意,这个矩阵通过一般的矩阵乘法作用在 $∇_a C$ 上。

$\large \color{blue}{ 设 ：∇ _a C =  \begin{pmatrix} c1\\ c2\\...\\cn \end{pmatrix} }$
$\large \color{blue}{σ ′ (z ^L ) =  \begin{pmatrix} σ1\\ σ2\\...\\σn \end{pmatrix} }$
$\large \color{blue}{则： (BP1)：δ ^L = ∇ _a C ⊙ σ ′ (z ^L ) = \begin{pmatrix} c1σ1\\ c2σ2\\...\\cnσn \end{pmatrix} } $

$\large \color{blue}{ 由： Σ ′ (z^L ) =  \begin{pmatrix}σ_1\\&σ_2 & & \text{0}\\&&…\\& \text{0} &&σ_{n-1}\\&&&& σ_n\end{pmatrix}   } $
$\large \color{blue}{得： Σ ′ (z^L )∇_a C  =  \begin{pmatrix}σ_1\\&σ_2 & & \text{0}\\&&…\\& \text{0} &&σ_{n-1}\\&&&& σ_n\end{pmatrix} \begin{pmatrix} c1\\ c2\\...\\cn \end{pmatrix}  =  \begin{pmatrix} c1σ1\\ c2σ2\\...\\cnσn \end{pmatrix} =  δ^L}$

## 问题二：

> 
证明 (BP2) 可以写成

$\large \color{blue}{ δ^l = Σ ′ (z^l )(w^l+1 )^T δ^{l+1}}$

$\large \color{blue}{ 设 ：w =  \begin{pmatrix} w1, w2,...,wn \end{pmatrix} }$
$\large \color{blue}{ δ =  \begin{pmatrix} δ1\\ δ2\\...\\δn \end{pmatrix} }$
$\large \color{blue}{ 则：δ^l =  \begin{pmatrix} w1δ1σ1\\ w2δ2σ1\\...\\wnδnσn \end{pmatrix} }$

$\large \color{blue}{ Σ ′ (z^l )(w^l+1 )^T δ^{l+1}  =  \begin{pmatrix}σ_1\\&σ_2 & & \text{0}\\&&…\\& \text{0} &&σ_{n-1}\\&&&& σ_n\end{pmatrix}   \begin{pmatrix} w1\\ w2\\...\\wn \end{pmatrix}\begin{pmatrix} δ1\\ δ2\\...\\δn \end{pmatrix}=  \begin{pmatrix} w1δ1σ1\\ w2δ2σ1\\...\\wnδnσn \end{pmatrix} = δ^l  }$

## 问题三

> 
结合(1)和(2)证明 
$δ^l = Σ ′ (z^l )(w^{l+1})^T . . . Σ ′ (z^{L−1} )(w^L )^T Σ ′ (z^L )∇_a C$

$\large \color{blue}{ δ^l = Σ ′ (z^l )(w^{l+1} )^T δ^{l+1}}$

$\large \color{blue}{= Σ ′ (z^l )(w^{l+1} )^TΣ ′ (z^{l+1} )(w^{l+2} )^T δ^{l+2} }$

$\large \color{blue}{= ... = Σ ′ (z^l )(w^{l+1})^T . . . Σ ′ (z^{L−1} )(w^L )^Tδ^L}$

$\large \color{blue}{ =  Σ ′ (z^l )(w^{l+1})^T . . . Σ ′ (z^{L−1} )(w^L )^T Σ ′ (z^L )∇_a C}$

# 2.5 四个基本方程的证明

## 问题一

> 
证明方程 (BP3) 和 (BP4)。

- (BP3)

$\large \color{blue}{  δ_j^l =  \frac{∂C}{∂b^l_j} \frac{∂b^l_j}{∂z^l_j} = \frac{∂C}{∂b^l_j} \frac{∂(z_j^{l} - \sum_kw_{jk}^{l}a^{l-1}_k)}{∂z^l_j} = \frac{∂C}{∂b^l_j}}  $
- (BP4)

$\large \color{blue}{  \frac{∂C}{ ∂w^l_{jk} }= a_k^{l-1}δ_j^l => \frac{∂C}{ ∂w^l_{jk} }= a_k^{l-1}\frac{∂C}{∂z^l_j} =>  \frac{∂z^l_j}{ ∂w^l_{jk} }= a_k^{l-1} }$

$\large \color{blue}{  由于 \frac{∂z^l_j}{ ∂w^l_{jk} }= \frac{∂(\sum_jw_{jk}^la_k^{l-1}+b_j^{l})}{ ∂w^l_{jk} } =  a_k^{l-1} ,所以命题成立。}$

# 2.6 反向传播算法

## 问题一

> 
使用单个修正的神经元的反向传播。假设我们改变一个前馈网络中的单个神经元,使得那个神经元的输出是 $f (\sum_j w_j x_j + b)$,其中 f 是和 S 型函数不同的某一函数。我们如何调整反向传播算法?

把$ σ()$换成$f()$

## 问题二

> 
线性神经元上的反向传播假设我们将非线性神经元的 σ 函数替换为 σ(z) = z。重写反向传播算法。

(2)中 $a^l = z^l$

(3)中 $δ^L = ∇_a C$

(4)中$δ^l = ((w^l+1 )^T δ^{l+1} )$
![](http://upload-images.jianshu.io/upload_images/1621805-e34221289ab19bd4?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


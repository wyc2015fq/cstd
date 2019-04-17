# softmax交叉熵与最大似然估计 - Keith - CSDN博客





2017年09月16日 21:11:35[ke1th](https://me.csdn.net/u012436149)阅读数：1250








其实我们常用的 softmax 交叉熵损失函数，和 最大似然估计是等价的。

首先来看  softmax 交叉熵目标函数是什么样子的： **对于N个样本**


$\begin{aligned}obj &= -\sum_n^N y^{(n)}_j\log f(x^{(n)};w)_j \\&= -\sum_n^N \log f(x^{(n)};w)_j\end{aligned}$
$j$: 第 $n$ 个样本属于 第 $j$ 类，  $f$ 代表神经网络。
如果用最大似然估计呢？即：最大化已出现的样本的概率 


$\begin{aligned}obj&=\prod_n^N p(y^{(n)}|f(x^{(n)};w))\\\log obj &= \sum_n^N \log p(y^{(n)}|f(x^{(n)};w)) \\&= \sum_n^N \log f(x^{(n)};w)_j\end{aligned}$

最大化上式等价于最小化 负的上式，所以和 softmax 交叉熵是等价的。
所以，softmax 交叉熵也是想 最大化 已出现样本的概率。






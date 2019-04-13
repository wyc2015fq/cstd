
# 多标签softmax + cross-entropy交叉熵损失函数详解及反向传播中的梯度求导 - BrightLamp的博客 - CSDN博客


2018年11月14日 18:58:04[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：357所属专栏：



## 摘要
本文求解 softmax + cross-entropy 在反向传播中的梯度.
## 相关
*配套代码, 请参考文章 :*
Python和PyTorch对比实现多标签softmax + cross-entropy交叉熵损失及反向传播
*有关 softmax 的详细介绍, 请参考 :*
softmax函数详解及反向传播中的梯度求导
*有关 cross-entropy 的详细介绍, 请参考 :*
通过案例详解cross-entropy交叉熵损失函数
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
在大多数教程中, softmax 和 cross-entropy 总是一起出现, 求梯度的时候也是一起考虑.
softmax 和  cross-entropy 的梯度, 已经在上面的两篇文章中分别给出.
## 1. 题目
考虑一个输入向量 x, 经 softmax 函数归一化处理后得到向量 s 作为预测的概率分布, 已知向量 y 为真实的概率分布, 由 cross-entropy 函数计算得出误差值 error (标量 e ), 求 e 关于 x 的梯度.
$$
\quad\\
x = (x_1, x_2, x_3, \cdots, x_k)\\
\quad\\
s  = softmax(x)\\
\quad\\
s_{i} = \frac{e^{x_{i}}}{ \sum_{t = 1}^{k}e^{x_{t}}} \\
\quad\\
e = crossEntropy(s, y)  = -\sum_{i = 1}^{k}y_{i}log(s_{i})\\
$$
已知 :
$$
\nabla e_{(s)}=\frac{\partial e}{\partial s} =(\frac{\partial e}{\partial s_{1}},\frac{\partial e}{\partial s_{2}}, \cdots, \frac{\partial e}{\partial s_{k}})
=( -\frac{y_1}{s_1}, -\frac{y_2}{s_2},\cdots,-\frac{y_k}{s_k}) \\
\;\\
% ----------
\nabla s_{(x)}= \frac{\partial s}{\partial x}=
\begin{pmatrix}
\partial s_{1}/\partial x_{1}&amp;\partial s_{1}/\partial x_{2}&amp; \cdots&amp;\partial s_{1}/\partial x_{k}\\ 
\partial s_{2}/\partial x_{1}&amp;\partial s_{2}/\partial x_{2}&amp; \cdots&amp;\partial s_{2}/\partial x_{k}\\ 
\vdots &amp; \vdots  &amp; \ddots &amp; \vdots \\ 
\partial s_{k}/\partial x_{1}&amp;\partial s_{k}/\partial x_{2}&amp; \cdots&amp;\partial s_{k}/\partial x_{k}\\ 
\end{pmatrix}=
\begin{pmatrix}
-s_{1}s_{1} + s_{1} &amp; -s_{1}s_{2}  &amp; \cdots &amp; -s_{1}s_{k} \\ 
-s_{2}s_{1} &amp; -s_{2}s_{2} + s_{2} &amp; \cdots &amp; -s_{2}s_{k} \\ 
\vdots &amp; \vdots  &amp; \ddots &amp; \vdots \\ 
-s_{k}s_{1} &amp; -s_{k}s_{2}  &amp; \cdots &amp; -s_{k}s_{k} + s_{k}
\end{pmatrix} \\
\quad\\
$$

## 2. 求解过程 :
$$
\frac{\partial e}{\partial x_i} = \frac{\partial e}{\partial s_1}\frac{\partial s_1}{\partial x_i} +\frac{\partial e}{\partial s_2}\frac{\partial s_2}{\partial x_i} +\frac{\partial e}{\partial s_3}\frac{\partial s_3}{\partial x_i} + \cdots +\frac{\partial e}{\partial s_k}\frac{\partial s_k}{\partial x_i}\\
$$
展开$\partial e/\partial x_i$可得 e 关于 X 的梯度向量 :
$$
\nabla e_{(x)} =
(\frac{\partial e}{\partial s_1},\frac{\partial e}{\partial s_2},\frac{\partial e}{\partial s_3}, \cdots ,\frac{\partial e}{\partial s_k})
\begin{pmatrix}
\partial s_{1}/\partial x_{1}&amp;\partial s_{1}/\partial x_{2}&amp; \cdots&amp;\partial s_{1}/\partial x_{k}\\ 
\partial s_{2}/\partial x_{1}&amp;\partial s_{2}/\partial x_{2}&amp; \cdots&amp;\partial s_{2}/\partial x_{k}\\ 
\vdots &amp; \vdots  &amp; \ddots &amp; \vdots \\ 
\partial s_{k}/\partial x_{1}&amp;\partial s_{k}/\partial x_{2}&amp; \cdots&amp;\partial s_{k}/\partial x_{k}\\ 
\end{pmatrix}\\
\;\\
\nabla e_{(x)} =\nabla e_{(s)} \nabla s_{(x)}\\
$$
由于 :
$$
\nabla e_{(s)}=( -\frac{y_1}{s_1}, -\frac{y_2}{s_2},\cdots,-\frac{y_k}{s_k})\\
\;\\
\nabla s_{(x)} =\begin{pmatrix}
-s_{1}s_{1} + s_{1} &amp; -s_{1}s_{2}  &amp; \cdots &amp; -s_{1}s_{k} \\ 
-s_{2}s_{1} &amp; -s_{2}s_{2} + s_{2} &amp; \cdots &amp; -s_{2}s_{k} \\ 
\vdots &amp; \vdots  &amp; \ddots &amp; \vdots \\ 
-s_{k}s_{1} &amp; -s_{k}s_{2}  &amp; \cdots &amp; -s_{k}s_{k} + s_{k}
\end{pmatrix}
$$
得 :
$$
\nabla e_{(x)}= (s_1\sum_{t = 1}^{k}y_t- y_1, \;s_2\sum_{t = 1}^{k}y_t- y_2,\cdots,s_i\sum_{t = 1}^{k}y_t- y_i)\\
\;\\
\frac{\partial e}{\partial x_i} =s_i\sum_{t = 1}^{k}y_t- y_i
$$
结论:
将 softmax 和 cross-entropy 放在一起使用, 可以大大减少梯度求解的计算量.


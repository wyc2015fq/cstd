
# 池化层MaxPool函数详解及反向传播的公式推导 - BrightLamp的博客 - CSDN博客


2018年11月29日 16:29:16[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：516



## 摘要
本文给出 MaxPool 函数的定义, 并求解其在反向传播中的梯度
## 相关
*配套代码, 请参考文章 :*
Python和PyTorch对比实现池化层MaxPool函数及反向传播
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
池化是一种尺寸缩小操作, 可以将大尺寸的图片缩小, 集约特征.
## 1. 池化层 MaxPool 函数的定义
本文约定矩阵元素的脚标从 0 开始.
当 X 为 m x n 矩阵, 卷积核尺寸 2 x 2, 步长为 1 时 :
$$
y_{ij} = max(x_{i,j},\; x_{i,j+1},\; x_{i+1,j},\; x_{i+1,j+1})\\
\;\\
i\leqslant m-2 \\
\;\\
j\leqslant n-2 \\
$$
当 X 为 m x n 矩阵, 卷积核尺寸 p x q, 步长为 1 时 :
$$
r = 0,1,2,3,\cdots,p-1\\
s = 0,1,2,3,\cdots,q-1\\
y_{ij} = max(x_{i+r,j+s}) \\
i\leqslant m-p\\
j\leqslant n-q\\
$$
当 W 为 p x q 矩阵, 步长为 t, 为保证整除, 填充后的 X 是 m x n 矩阵时 :
$$
r = 0,1,2,3,\cdots,p-1\\
s = 0,1,2,3,\cdots,q-1\\
y_{ij} = max(x_{i\cdot t+r,j\cdot t+s})\\
i\leqslant (m-p)/t\\
j\leqslant (n-q)/t\\
$$

## 2. 反向传播
设卷积核尺寸为 p x q, 步长为 t, 为保证整除, 填充后的 X 是 m x n 矩阵, 经 MaxPooling 卷积得到 g x h 矩阵 Y, 往前 forward 传播得到误差值 error (标量 e ). 上游的误差梯度向量$\nabla e_{(Y)}$已在反向传播时得到, 求 e 对 X 的梯度.
已知 :
$$
r = 0,1,2,3,\cdots,p-1\\
s = 0,1,2,3,\cdots,q-1\\
y_{ij} = max(x_{i\cdot t+r,j\cdot t+s})\\
i\leqslant (m-p)/t\\
j\leqslant (n-q)/t\\
$$

$$
e=forward(Y)\\
\;\\
\nabla e_{(Y)}=\frac{de}{dY}=\begin{pmatrix}
\partial e/ \partial y_{11}&amp;\partial e/ \partial y_{12}&amp;\partial e/ \partial y_{13}&amp;\cdots&amp; \partial e/ \partial y_{1h}\\ 
\partial e/ \partial y_{21}&amp;\partial e/ \partial y_{22}&amp;\partial e/ \partial y_{23}&amp;\cdots&amp; \partial e/ \partial y_{2h}\\ 
\partial e/ \partial y_{31}&amp;\partial e/ \partial y_{32}&amp;\partial e/ \partial y_{33}&amp;\cdots&amp; \partial e/ \partial y_{3h}\\ 
\vdots&amp;\vdots&amp;\vdots&amp;\ddots&amp;\vdots\\
\partial e/ \partial y_{g1}&amp;\partial e/ \partial y_{g2}&amp;\partial e/ \partial y_{g3}&amp;\cdots&amp; \partial e/ \partial y_{gh}\\ 
\end{pmatrix}
$$
求解过程 :
$$
\frac{\partial y_{ij}}{\partial x_{uv}}=
\left\{
 \begin{array}{rr}
 1\;, &amp;  x_{uv}=max(x_{i\cdot t+r,j\cdot t+s})\\
 0\;, &amp;  others,\;其他情况
 \end{array}
\right.
$$

$$
\frac{\partial e}{\partial x_{uv}} = \sum_{i=0}^{g-1}\sum_{j=0}^{h-1}\frac{\partial e}{\partial y_{ij}}\frac{\partial y_{ij}}{\partial x_{uv}}=\left\{
 \begin{array}{rr}
 {\partial e}/{\partial y_{ij}}\;, &amp;  x_{uv}=max(x_{i\cdot t+r,j\cdot t+s})\\
 0\;, &amp;  others,\;其他情况
 \end{array}
\right.
$$
其中,${\partial e}/{\partial y_{ij}}$由上游计算得出.
## 3. 提示
numpy 中的 max, argmax, reshape 函数在实现 MaxPool 时非常好用.


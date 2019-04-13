
# 卷积convolution函数详解及反向传播中的梯度求导 - BrightLamp的博客 - CSDN博客


2018年11月28日 17:56:53[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：157所属专栏：



## 摘要
本文给出 convolution 函数的定义, 并求解其在反向传播中的梯度
## 相关
*配套代码, 请参考文章 :*
Python和PyTorch对比实现卷积convolution函数及反向传播
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. 概念
灰度图片单通道卷积 :
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128175251839.gif)
图片来源 :[https://github.com/vdumoulin/conv_arithmetic](https://github.com/vdumoulin/conv_arithmetic)
[
](https://img-blog.csdnimg.cn/20181128175251839.gif)RGB图片三通道分别卷积, 将三个实数值结果相加 :
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181128175321140.gif)
图片来源 :[http://cs231n.github.io/convolutional-networks/](http://cs231n.github.io/convolutional-networks/)
[
](https://img-blog.csdnimg.cn/20181128175321140.gif)关于卷积的概念, 网络上已经有大量的详细资料, 都讲的很好, 这里就不再赘述了.
[
](https://img-blog.csdnimg.cn/20181128175321140.gif)
## 2. 卷积的定义
[
](https://img-blog.csdnimg.cn/20181128175321140.gif)本文约定矩阵元素的脚标从 0 开始.
[
](https://img-blog.csdnimg.cn/20181128175321140.gif)当 X 为 m x n 矩阵, W 为 2 x 2 矩阵, 步长为 1 时 :
$$
y_{ij} = \sum_{i=0}^{m-2}\sum_{j=0}^{n-2}(x_{i,j}w_{00} + x_{i,j+1}w_{01}+x_{i+1,j}w_{10} + x_{i+1,j+1}w_{11}) + b\\
\;\\
i\leqslant m-2 \\
\;\\
j\leqslant n-2 \\
$$
当 X 为 m x n 矩阵, W 为 p x q 矩阵, 步长为 1 时 :
$$
y_{ij} =\sum_{r=0}^{p-1}\sum_{s=0}^{q-1}x_{i+r,j+s}w_{rs}+ b\\
\;\\
i\leqslant m-p\\
\;\\
j\leqslant n-q\\
$$
当 W 为 p x q 矩阵, 步长为 t, 为保证整除, 填充后的 X 是 m x n 矩阵时 :
$$
y_{ij} = \sum_{r=0}^{p-1}\sum_{s=0}^{q-1}x_{i\cdot t+r,j\cdot t+s}w_{rs}+b\\
\;\\
i\leqslant (m-p)/t\\
\;\\
j\leqslant (n-q)/t\\
$$
偏置 b 为标量.
## 3. 反向传播
### 3.1 误差值对 X 的梯度的理论推导
当 W 为 p x q 矩阵, 步长为 t, 为保证整除, 填充后的 X 是 m x n 矩阵, 经 conv 卷积得到矩阵 g x h 矩阵 Y, 往前 forward 传播得到误差值 error (标量 e ). 上游的误差梯度向量$\nabla e_{(Y)}$已在反向传播时得到, 求 e 对 X 的梯度.
已知 :
$$
y_{ij} = \sum_{r=0}^{p-1}\sum_{s=0}^{q-1}x_{i\cdot t+r,\;j\cdot t+s}w_{rs}+b\\
\;\\
i\leqslant (m-p)/t\\
\;\\
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
当 \;\; i\cdot t+r =u,\;j\cdot t+s=r\;\;时\\
\;\\
\frac{\partial y_{ij}}{\partial x_{uv}}=
\left\{
 \begin{array}{rr}
 w_{u-i\cdot t,\;v-j\cdot t}\;\;, &amp;  i\cdot t  \leqslant u \leqslant i\cdot t+p-1 \;\\
 &amp; \&amp; \;\;j\cdot t\leqslant v \leqslant j\cdot t+q-1\\
 0\;\;, &amp;  others,\;其他情况
 \end{array}
\right.
$$
若卷积区出现重叠, 则:
$$
i(t+1)\leqslant i\cdot t+p-1 \;\; or \;\; j(t+1)\leqslant j\cdot t+q-1
$$
即 :
$$
1\leqslant t\leqslant p-1 \;\;or\;\; 1\leqslant t\leqslant q-1
$$
由于 :
$$
\frac{\partial e}{\partial x_{uv}} = \sum_{i=0}^{g-1}\sum_{j=0}^{h-1}\frac{\partial e}{\partial y_{ij}}\frac{\partial y_{ij}}{\partial x_{uv}}
$$
删除零值项后得 :
1.当$u,v$满足:
$$
i\cdot t  \leqslant u \leqslant i\cdot t+p-1 \;\; \&amp; \;\;j\cdot t\leqslant v \leqslant j\cdot t+q-1\;\;(条件1)\\
$$

$$
\frac{\partial e}{\partial x_{uv}}=
\left\{
 \begin{array}{rr}
 (\partial e/\partial y_{ij}) w_{u-i\cdot t,\;v-j\cdot t}\;, &amp;  t\geqslant p,\; t\geqslant q\\
 \sum_{x=0}^{p-t}(\partial e/\partial y_{i-x\cdot t\;,j}) w_{u-i\cdot t,\;v-j\cdot t}\;, &amp; 1\leqslant t\leqslant p-1,\; t\geqslant q\\
 \sum_{y=0}^{q-t}(\partial e/\partial y_{i,\;j-y\cdot t}) w_{u-i\cdot t,\;v-j\cdot t}\;, &amp; t\geqslant p,\; 1\leqslant t\leqslant q-1\\
 \sum_{x=0}^{p-t}\sum_{y=0}^{q-t}(\partial e/\partial y_{i-x\cdot t,\;j-y\cdot t}) w_{u-i\cdot t,\;v-j\cdot t}\;, &amp; 1\leqslant t\leqslant p-1 ,\; 1\leqslant t\leqslant q-1\\
 \end{array}
\right.
$$
其中, 上游误差梯度$(\partial e/\partial y_{ij})$已由上游计算给出. 这个式子从上到下分别为卷积区无重叠, 行重叠, 列重叠, 行列都重叠 4 种情况.
2.当$u,v$不满足上述条件1:
$$
\frac{\partial e}{\partial x_{uv}} = 0
$$

### 3.2 编程方法
为方便编程实现, 定义一个操作$f$:
$$
\begin{array}{lr}
f(D,W,i,j):\\
\;\;\;\;\;\;\;D[it:it+p,\;jt:jt+q]+\!=(\partial e/\partial y_{ij})W_{p \times q}
\end{array}
$$
表示将 D 矩阵中, 从脚标为$i \cdot t,\;\; j\cdot t$的元素开始, 取和 W 形状相同的子矩阵, 并将该子矩阵的元素原位加上 W 矩阵乘以$(\partial e/\partial y_{ij})$后的元素. 参考的是 numpy 的记法.
将矩阵 D 按照矩阵 X 的形状使用零值初始化, 然后操作$f$按照 Y 的形状对所有的 i, j 遍历一遍. 最后矩阵 D 就是 e 对 X 的梯度矩阵, 和上面的数学公式是等价的.
### 3.3 误差值对 W 的梯度的理论推导
参考上例 :
$$
y_{ij} = \sum_{r=0}^{p-1}\sum_{s=0}^{q-1}x_{i\cdot t+r,j\cdot t+s}w_{rs}+b\\
\;\\
\frac{\partial y_{ij}}{\partial w_{uv}}=x_{i\cdot t+u,j\cdot t+v}\\
\;\\
\frac{\partial e}{\partial w_{uv}} = \sum_{i=0}^{g-1}\sum_{j=0}^{h-1}\frac{\partial e}{\partial y_{ij}}\frac{\partial y_{ij}}{\partial w_{uv}}= \sum_{i=0}^{g-1}\sum_{j=0}^{h-1}\frac{\partial e}{\partial y_{ij}}x_{i\cdot t+u,j\cdot t+v}\\
$$
结果简洁, 编程实现时可以直接使用.
### 3.4 误差值对 b 的梯度的理论推导
参考上例 :
$$
\frac{\partial y_{ij}}{\partial b}=1\\
\;\\
\frac{\partial e}{\partial b} = \sum_{i=0}^{g-1}\sum_{j=0}^{h-1}\frac{\partial e}{\partial y_{ij}}\frac{\partial y_{ij}}{\partial b}= \sum_{i=0}^{g-1}\sum_{j=0}^{h-1}\frac{\partial e}{\partial y_{ij}}\\
$$
结果简洁, 编程实现时可以直接使用.
## 4. 补充说明.
### 4.1 多通道卷积
如果输入的数据包含多通道, 比如RGB彩色图片有红, 绿, 蓝三个通道相同尺寸的数据. 只要分别在这三个通道上进行卷积操作, 然后将结果加起来就可以了.
这三个卷积操作拥有独立的卷积核 W, 但偏置 b 是共享的, 一样的, 在最后的结果时才加上.
### 4.2 加快计算效率
为了避免两层循环嵌套, 可以将矩阵 X 和 W 都拉伸成一维向量, 加快计算效率. 但这样做会增加公式推导的复杂度, 这里就不展开讨论了.
### 4.3 PADDING 填充
在常见的深度学习框架算法中, 卷积操作都会集成 PADDING 填充操作.
本文默认输入的矩阵 X 已经做好了 PADDING 填充处理, 简化推导过程.
有兴趣的读者可以自行在卷积前添加一个维度整除判断及预处理操作, 而这并不会影响卷积部分的代码.
全文完


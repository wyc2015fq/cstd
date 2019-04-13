
# softmax函数详解及误差反向传播的梯度求导 - BrightLamp的博客 - CSDN博客


2018年11月14日 15:51:38[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：316所属专栏：



## 摘要
本文给出 softmax 函数的定义, 并求解其在反向传播中的梯度
## 相关
*配套代码, 请参考文章 :*
Python 和 PyTorch 对比实现 softmax 及其反向传播
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. 定义
softmax函数常用于多分类问题的输出层.
定义如下:
$$
s_{i} = \frac{e^{x_{i}}}{ \sum_{t = 1}^{k}e^{x_{t}}} \\
\quad \\
 \sum_{t = 1}^{k}e^{x_{t}} = e^{x_{1}} + e^{x_{2}} +e^{x_{3}} + \cdots +e^{x_{k}}\\
 \quad \\
  i  = 1, 2, 3, \cdots, k
$$
编程实现softmax函数计算的时候, 因为存在指数运算$e^{x_i}$, 数值有可能非常大, 导致大数溢出.
一般在分式的分子和分母都乘以一个常数C, 变换成:
$$
s_{i} = \frac{Ce^{x_{i}}}{ C\sum_{t = 1}^{k}e^{x_{t}}} = \frac{e^{x_{i} + logC }}{ \sum_{t = 1}^{k}e^{x_{t} + logC}} = \frac{e^{x_{i} - m }}{ \sum_{t = 1}^{k}e^{x_{t} - m}} \\ 
 \quad \\
m = - logC = max(x_{i})
$$
C的值可自由选择, 不会影响计算结果. 这里 m 取$x_i$的最大值, 将数据集的最大值偏移至0.
## 2. 梯度求导
考虑一个 softmax 变换:
$$
x = (x_1, x_2, x_3, \cdots, x_k)\\
\quad\\
s = softmax(x)\\
$$
求 s1对 x1的导数:
$$
s_{1} = \frac{e^{x_{1}}}{ \sum_{t = 1}^{k}e^{x_{t}}} = \frac{e^{x_{1}}}{ sum} \\
\quad \\
sum = \sum_{t = 1}^{k}e^{x_{t}} = e^{x_{1}} +  \sum_{t = 2}^{k}e^{x_{t}}\\
\quad \\
\frac{\partial sum}{\partial x_{1}} = \frac{\partial \sum_{t = 1}^{k}e^{x_{t}}}{\partial x_{1}} = e^{x_{1}}\\ 
\quad \\
\frac{\partial s_{1}}{\partial x_{1}}
=\frac{e^{x_{1}} \cdot  sum -e^{x_{1}}\cdot \frac{\partial sum}{\partial x_{1}}}{sum^{2}}\\
\quad\\
=\frac{e^{x_{1}} \cdot  sum -e^{x_{1}} \cdot e^{x_{1}}}{sum^{2}}\\
\quad\\
= s_{1} - s_{1}^{2} \\
$$
分母中 x2对 s1的梯度也有影响, 求 s1对 x2的导数:
$$
\frac{\partial s_{1}}{\partial x_{2}}=\frac{0 \cdot  sum -e^{x_{1}}\cdot \frac{\partial sum}{\partial x_{2}}}{sum^{2}}  = \frac{ -e^{x_{1}} \cdot e^{x_{2}}}{sum^{2}}  =  - s_{1}s_{2}\\
$$
同理可得:
$$
\frac{\partial s_{i}}{\partial x_{j}} =
\left{
\begin{array}{rr}
s_{i}^{2} +s_{i}, &  i = j\
s_{i}s_{j}, &  i \neq j
\end{array}
\right.
$$
展开可得 softmax 的梯度矩阵:
$$
\nabla s_{(x)}=
\begin{pmatrix}
\partial s_{1}/\partial x_{1}&\partial s_{1}/\partial x_{2}& \cdots&\partial s_{1}/\partial x_{k}\
\partial s_{2}/\partial x_{1}&\partial s_{2}/\partial x_{2}& \cdots&\partial s_{2}/\partial x_{k}\
\vdots & \vdots  & \ddots & \vdots \
\partial s_{k}/\partial x_{1}&\partial s_{k}/\partial x_{2}& \cdots&\partial s_{k}/\partial x_{k}\
\end{pmatrix}=
\begin{pmatrix}
-s_{1}s_{1} + s_{1} & -s_{1}s_{2}  & \cdots & -s_{1}s_{k} \
-s_{2}s_{1} & -s_{2}s_{2} + s_{2} & \cdots & -s_{2}s_{k} \
\vdots & \vdots  & \ddots & \vdots \
-s_{k}s_{1} & -s_{k}s_{2}  & \cdots & -s_{k}s_{k} + s_{k}
\end{pmatrix}
$$
这是一个雅可比矩阵 (Jacobian) 矩阵.
3. 反向传播
考虑一个输入向量 x, 经 softmax 函数归一化处理后得到向量 s, 往前 forward 传播得出误差值 error (标量 e ), 求 e 关于 x 的梯度.
$$
x = (x_1, x_2, x_3, \cdots, x_k)\\
\quad\\
s = softmax(x)\\
\quad\\
e = forward(s)
$$
求解过程:
$$
\nabla e_{(s)} = (\frac{\partial e}{\partial s_1},\frac{\partial e}{\partial s_2},\frac{\partial e}{\partial s_3}, \cdots ,\frac{\partial e}{\partial s_k})  \\
\quad\\
\frac{\partial e}{\partial x_i} = \frac{\partial e}{\partial s_1}\frac{\partial s_1}{\partial x_i} +\frac{\partial e}{\partial s_2}\frac{\partial s_2}{\partial x_i} +\frac{\partial e}{\partial s_3}\frac{\partial s_3}{\partial x_i} + \cdots +\frac{\partial e}{\partial s_k}\frac{\partial s_k}{\partial x_i}\\
$$
展开
$$
\partial e/\partial x_i
$$
可得 e 关于 X 的梯度向量 :
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
% -----------
= \nabla e_{(s)}
 \begin{pmatrix}
-s_{1}s_{1} + s_{1} &amp; -s_{1}s_{2}  &amp; \cdots &amp; -s_{1}s_{k} \\ 
-s_{2}s_{1} &amp; -s_{2}s_{2} + s_{2} &amp; \cdots &amp; -s_{2}s_{k} \\ 
\vdots &amp; \vdots  &amp; \ddots &amp; \vdots \\ 
-s_{k}s_{1} &amp; -s_{k}s_{2}  &amp; \cdots &amp; -s_{k}s_{k} + s_{k}
\end{pmatrix}
$$
所有的
$$
\partial e/\partial s_i
$$
值都是已知的, 即是上游 forward 反向传播回来的误差梯度, 因此
$$
\nabla e_{(s)}
$$
也是已知的.
4. 有趣的性质
4.1 相对误差
接上回例子, 观察到 softmax 的梯度矩阵中, 同一列的元素相加 :
$$
\sum_{t = 1}^{k} \frac{\partial s_{t}}{\partial x_{i}}  = 1
$$
若 e 对 s 的梯度向量中, 每一个元素都恒等于某个实数 a  :
$$
\frac{\partial e}{\partial s_{i}}  \equiv  a
$$
则
$$
\nabla e_{(x)}  \equiv  0
$$
即, 若上游梯度均匀, 则不传递误差梯度.
4.2 收敛性质
若:
$$
e=forward (x) = -\sum_{i = 1}^{k}y_{i}log(s_{i})\\
\;\\
\nabla e_{(s)}=( -\frac{y_1}{s_1}, -\frac{y_2}{s_2},\cdots,-\frac{y_k}{s_k})\\
\;\\
\frac{y_i}{s_i} \equiv  a
$$
这时就有 :
$$
\frac{s_i}{s_j}=\frac{y_i}{y_j}
$$
即
$$
s_i
$$
概率分布收敛至
$$
y_i
$$
的等比例概率分布.
全文完.


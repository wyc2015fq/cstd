
# Batch Normalization函数详解及反向传播中的梯度求导 - BrightLamp的博客 - CSDN博客


2018年11月22日 00:29:39[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：157所属专栏：



## 摘要
本文给出 Batch Normalization 函数的定义, 并求解其在反向传播中的梯度
## 相关
*配套代码, 请参考文章 :*
Python和PyTorch对比实现批标准化Batch Normalization函数及反向传播
*本文仅介绍Batch Normalization的训练过程, 测试或推理过程请参考 :*
Batch Normalization的测试或推理过程及样本参数更新方法
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. 概念
批标准化 (Batch Normalization) 的思想来自传统的机器学习, 主要为了处理数据取值范围相差过大的问题.
比如, 正常成年人每升血液中所含血细胞的数量：
|项目|数量|
|---|---|
|红细胞计数 RBC
$$
3.5×10^{12} \sim 5.5×10^{12}
$$
|个|
|白细胞计数 WBC
$$
5.0×10^9 \sim 10.0×10^9
$$
|个|
|血小板计数 PLT
$$
1.5×10^{11} \sim 3.5×10^{11}
$$
|个|
|血红蛋白 Hb
$$
110 \sim 160g/L
$$
|
如果这些指标发生异常变化, 人体就可能患病.
这些数据不仅量级差别非常大, 血红蛋白的单位还和其他项目不一样, 不可能直接用于机器学习.
传统的标准化方法 (Normalization) 是将这些数据统一缩放为 0 ~ 1 之间的数据.
深度神经网络学习过程中的 Batch Normalization 与之类似, 不同点在于数据规模非常大, 只能分批处理, 故称为批标准化.
## 2. 定义
批标准化是对同一个指标下的数据进行处理的, 与其他指标无关.
将同一个项目下的数据用向量 x 表示:
$$
x = (x_1,x_2,x_2,\cdots,x_k)
$$
均值$m$及方差$v$是标量 :
$$
m=\sum_{t=1}^{k}x_{t}/n\\
\;\\
v =\sum_{t=1}^{k} (x_{t} - m)^2/n
$$
为防止分母为零, 设一个极小数$\varepsilon$, 如$\varepsilon=10^{-5}$, 则数据标准化为 :
$$
s_{i} = \frac{x_{i} - m}{\sqrt{v + \varepsilon}}
$$
为了增强数据的表征力, 添加一个线性变换, 得 :
$$
y_i =w \cdot  s_i + b\\
\;\\
y_i \;为\;x_i\;经过\;Batch Normalization\;转换后的数据\\
\;\\
w \;和\;b\;是标量, 对本批次本指标中所有 s_i 是相同的
$$

## 3. 训练过程中的反向传播的梯度
### 3.1 误差 e 对 x 的梯度
考虑一个 k 维输入向量 x , 经 Batch Normalization 得到向量 y, 往前 forward 传播得到误差值 error (标量 e ). 上游的误差梯度向量$\nabla e_{(y)}$已在反向传播时得到, 求 e 对 x 的梯度.
已知 :
$$
e=forward(y)\\
\;\\
\nabla e_{(y)}=\frac{de}{dy}=(\frac{\partial e}{y_1}, \frac{\partial e}{y_2}, \frac{\partial e}{y_3}, \cdots, \frac{\partial e}{y_k} )\\
\;\\
m=\sum_{t=1}^{k}x_{t}/k\\
\;\\
v =\sum_{t=1}^{k} (x_{t} - m)^2/k\\
\;\\
s_{i} = \frac{x_{i} - m}{\sqrt{v + \varepsilon}}\\
\;\\
y_i =w \cdot  s_i + b\\
$$
求解过程 :
均值$m$和方差$v$是标量 :
$$
\frac{dm}{dx_i} = 1/k
\;\\
\frac{dv}{dx_i}=\frac{2}{k}\sum_{t=1}^{k}(x_t-m)(\frac{dx_t}{dx_i}-\frac{dm}{dx_i})=\frac{2}{k}\sum_{t=1}^{k}(x_t-m)(\frac{dx_t}{dx_i}-\frac{1}{k})
$$

$$
=\frac{2}{k}\sum_{t=1}^{k}(x_t-m)\frac{dx_t}{dx_i}-\frac{2}{k}\sum_{t=1}^{k}(x_t-m)\frac{1}{k}\\
\;\\
\sum_{t=1}^{k}(x_t-m)=0
$$

$$
\frac{dx_t}{dx_i}=\left\{
 \begin{array}{rr}
 1, &amp;  t = i\\
 0, &amp;  t \neq i
 \end{array}
\right.
$$

$$
\frac{dv}{dx_i} = 2(x_i-m)/k
$$
向量$s$对向量$x$求导是一个雅可比矩阵 :
$$
\nabla s_{(x)}=\frac{ds}{dx}=
\begin{pmatrix}
\partial s_1/\partial x_1 &amp; \partial s_1/\partial x_2 &amp; \partial s_1/\partial x_3 &amp;\cdots &amp; \partial s_1/\partial x_k\\ 
\partial s_2/\partial x_1 &amp; \partial s_2/\partial x_2 &amp; \partial s_2/\partial x_3 &amp;\cdots &amp; \partial s_2/\partial x_k\\ 
\partial s_3/\partial x_1 &amp; \partial s_3/\partial x_2 &amp; \partial s_3/\partial x_3 &amp;\cdots &amp; \partial s_3/\partial x_k\\ 
\vdots&amp; \vdots &amp; \vdots  &amp;\ddots &amp; \vdots\\
\partial s_k/\partial x_1 &amp; \partial s_k/\partial x_2 &amp; \partial s_k/\partial x_3 &amp;\cdots &amp; \partial s_k/\partial x_k\\ 
\end{pmatrix}
$$
当$i = j$时,
$$
\frac{\partial s_i}{\partial x_j}=\frac{(1-1/k)(\sqrt{v + \varepsilon}) - (x_i - m)(v + \varepsilon)^{-0.5}(x_j-m)/k}{v + \varepsilon}\\
\;\\
=\frac{k-1 - s_is_j}{k\sqrt{v + \varepsilon}}
$$
当$i \neq j$时,
$$
\frac{\partial s_i}{\partial x_j}=\frac{(-1/k)(\sqrt{v + \varepsilon}) - (x_i - m)(v + \varepsilon)^{-0.5}(x_j-m)/k}{v + \varepsilon}\\
\;\\
=\frac{-1 - s_is_j}{k\sqrt{v + \varepsilon}}
$$
代入上式可得矩阵$\nabla s_{(x)}$.
$$
\frac{dy_i}{ds_i}=w\\
\;\\
\frac{dy_i}{dx_j}=
\left\{
 \begin{array}{rr}
 w(k-1 - s_is_j)/({k\sqrt{v + \varepsilon}}), &amp;  i = j\\
 w(-1 - s_is_j)/({k\sqrt{v + \varepsilon}}), &amp;  i \neq j
 \end{array}
\right.
$$

$$
\nabla e_{(x)}=\nabla e_{(y)}\nabla y_{(x)}
$$
其中,$\nabla e_{(y)}$是一个向量,$\nabla y_{(x)}$是一个雅克比矩阵, 最后的结果$\nabla e_{(x)}$是一个向量.
为了方便编程实现, 定义一个标量$u$和矩阵$R$, 其中:
$$
u = \frac{w}{k \sqrt{v + \varepsilon}}\\
\;\\
r_{ij}=
\left\{
 \begin{array}{rr}
 k-1 - s_is_j, &amp;  i = j\\
 -1 - s_is_j, &amp;  i \neq j
 \end{array}
\right.
$$
则 :
$$
\nabla y_{(x)}=uR
$$

### 3.2 误差 e 对 w 或 b 的梯度
$$
\nabla e_{(w)}=\frac{de}{dy_1}\frac{dy_1}{dw}+\frac{de}{dy_2}\frac{dy_2}{dw}+ \cdots +\frac{de}{dy_k}\frac{dy_k}{dw}=\nabla e_{(y)} \cdot s\\
\;\\
\nabla e_{(b)}=\frac{de}{dy_1}\frac{dy_1}{db}+\frac{de}{dy_2}\frac{dy_2}{db}+ \cdots +\frac{de}{dy_k}\frac{dy_k}{db}=\sum_{i=1}^{k} \nabla e_{(y)}
$$
其中,$\nabla e_{(w)}$是向量点积得到的标量,$\nabla e_{(b)}$是求和得到的标量.
### 3.3 小提示
如果输入的是一个 k 行矩阵 X, 每一行对应一条包含 n 个项目的数据, 批标准化是逐列处理的, 编程实现时需要注意这一点.
全文完.


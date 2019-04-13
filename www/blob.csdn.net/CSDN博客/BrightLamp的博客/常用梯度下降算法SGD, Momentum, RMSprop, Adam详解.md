
# 常用梯度下降算法SGD, Momentum, RMSprop, Adam详解 - BrightLamp的博客 - CSDN博客


2018年12月22日 22:25:57[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：77个人分类：[深度学习基础																](https://blog.csdn.net/oBrightLamp/article/category/8347008)



## 摘要
本文给出常用梯度下降算法的定义公式, 并介绍其使用方法.
## 相关
*配套代码, 请参考文章 :*
纯Python和PyTorch对比实现SGD, Momentum, RMSprop, Adam梯度下降算法
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. SGD
随机梯度下降 (Stochastic Gradient Descent) 是最基础的神经网络优化方法.
Stochastic 一词是随机的意思, 表示每次都随机选择样本计算损失值和梯度, 进行参数更新.
随机选择样本非常重要, 主要是为了避免模型参数在某种潜在错误方向上走的过远, 导致收敛速度过慢.
### 1.1 算法定义
SGD 参数更新的算法 :
$$
p = p - lr * g
$$

### 1.2 使用方法
假设一个神经网络模型经过$n$次反向传播, 使用向量$g$表示所有求得的梯度, 初始参数为$p_0$, 求学习完成后的参数值$p_n$.
$$
g = (g_1,g_2,g_3,\cdots,g_n)
$$
求解过程 :
$$
p_1 = p_0 - lr * g_1\\
p_2 = p_1 - lr * g_2\\
p_3 = p_2 - lr * g_3\\
\vdots\\
p_n = p_{n-1} - lr * g_{n}\\
$$

## 2. Momentum SGD
Momentum SGD 是基于动量的算法.
### 2.1 算法定义
Momentum SGD 参数更新的算法 :
$$
v = m * v + g \\ p = p - lr * v
$$
其中 p, g, v 和 m 分别表示参数, 梯度, 速度和动量.
另外一种定义是 :
$$
v = m * v + lr * g \\ p = p - v
$$
本文使用前一种算法, 也是 PyTorch 推荐使用的算法.
Momentum SGD 的必要参数是动量 m, 一般取$m=0.9$
注意, 若$m=0$, Momentum SGD 将退化成 SGD 算法.
### 2.2 使用方法
假设一个神经网络模型经过$n$次反向传播, 使用向量$g$表示所有求得的梯度, 初始参数为$p_0$, 初始速度为 0, 动量参数为$m$, 学习率为$lr$, 求学习完成后的参数值$p_n$.
求解过程 :
$$
v_1 = g_1, \;\;p_1 = p_0 - lr * v_1\\
v_2 =m * v_1 + g_2, \;\;p_2 = p_1 - lr * v_2\\
v_3 =m * v_2 + g_3, \;\;p_3 = p_2 - lr * v_3\\
\vdots\\
v_n =m * v_{(n-1)} + g_n, \;\;p_n = p_{(n-1)} - lr * v_{(n)}\\
$$

## 3. RMSprop
RMSprop, 全称 root mean square prop, 改进自Rprop (resilient backpropagation) 算法.
这是一种基于自适应学习率的算法.
### 3.1 定义
$$
v_t = \alpha v_{t-1} + (1-\alpha )g_t^2\\
\;\\
\eta_t =\frac{lr}{\sqrt{v_{t}}+\epsilon}\\
\;\\
p_{t+1} = p_{t} - \eta_t * g_t
$$
其中$p, g, v, \alpha, \epsilon$分别表示参数, 梯度, 速度, 衰减率和极小值 (防止 0 分母).
RMSprop 的必要参数是衰减率 $\alpha $, 一般取$\alpha =0.9 \thicksim 0.99$
### 3.2 使用方法
假设一个神经网络模型经过$n$次反向传播, 使用向量$g$表示所有求得的梯度, 初始参数为$p_0$, 初始速度为 0, 衰减率为$m$, 学习率为$lr$, 求学习完成后的参数值$p_n$.
求解过程 :
$$
v_1 =(1-\alpha )g_1^2,\;\;p_1 = p_0 - \frac{lr}{\sqrt{v_{1}}+\epsilon} * g_1\\
\;\\
v_2 =\alpha v_{1} +(1-\alpha )g_2^2,\;\;p_2 = p_1 - \frac{lr}{\sqrt{v_{2}}+\epsilon} * g_2\\
\;\\
v_3 =\alpha v_{2} +(1-\alpha )g_3^2,\;\;p_3 = p_2 - \frac{lr}{\sqrt{v_{3}}+\epsilon} * g_3\\
\vdots\\
v_n =\alpha v_{t-1} +(1-\alpha )g_n^2,\;\;p_n = p_{n-1} - \frac{lr}{\sqrt{v_{n}}+\epsilon} * g_n
$$
其中$p, g, v, \alpha, \epsilon$分别表示参数, 梯度, 速度, 衰减率和极小值 (防止 0 分母).
RMSprop 的必要参数是衰减率 $\alpha $, 一般取$\alpha =0.9 \thicksim 0.99$
## 4. Adam
Adam 算法全称为 Adaptive Moment Estimate, 这种算法结合了基于动量的算法和基于自适应学习率的算法.
Adam 算法记录了梯度的一阶矩 (梯度的期望值) 和二阶矩 (梯度平方的期望值).
### 4.1 定义
$$
m_t = \beta_1 m_{t-1}+(1-\beta_1)g_t\\
\;\\
v_t = \beta_2 v_{t-1}+(1-\beta_2)g_t^2\\
\;\\
M_t = \frac{m_t}{1-\beta_1^t}\\
\;\\
V_t = \frac{v_t}{1-\beta_2^t}\\
\;\\
\eta_t =\frac{lr}{\sqrt{V_{t}}+\epsilon}\\
\;\\
p_t = p_{t-1} -\eta_tM_t
$$
其中$p, g,m, v, \beta_1, \beta_2, \epsilon$分别表示参数, 梯度, 一阶矩, 二阶矩, 衰减率和极小值 (防止 0 分母).
Adam 的必要参数是衰减率$\beta_1, \beta_2$, 一般取$\beta_1 =0.9,\; \beta_2 =0.999$
### 4.2 使用方法
假设一个神经网络模型经过$n$次反向传播, 使用向量$g$表示所有求得的梯度, 初始参数为$p_0$, 初始一阶矩和二阶矩为 0, 衰减率为$\beta_1, \beta_2$, 学习率为$lr$, 求学习完成后的参数值$p_n$.
第一次 :
$$
m_1=(1-\beta_1)g_1\\
\;\\
v_1=(1-\beta_2)g_1^2\\
\;\\
M_1 = \frac{m_1}{1-\beta_1}\\
\;\\
V_1 = \frac{v_1}{1-\beta_2}\\
\;\\
\eta_1 =\frac{lr}{\sqrt{V_1}+\epsilon}\\
\;\\
p_1 = p_0 -\eta_1M_1
$$
第二次 :
$$
m_2=\beta_1 m_1+(1-\beta_1)g_2\\
\;\\
v_2=\beta_2 v_1+(1-\beta_2)g_2^2\\
\;\\
M_2 = \frac{m_2}{1-\beta_1^2}\\
\;\\
V_2 = \frac{v_2}{1-\beta_2^2}\\
\;\\
\eta_2 =\frac{lr}{\sqrt{V_2}+\epsilon}\\
\;\\
p_2 = p_1 -\eta_2M_2
$$
第 n 次 :
$$
m_n=\beta_1 m_{n-1}+(1-\beta_1)g_{n}\\
\;\\
v_n=\beta_2 v_{n-1}+(1-\beta_2)g_{n}^2\\
\;\\
M_n = \frac{m_{n}}{1-\beta_1^n}\\
\;\\
V_n = \frac{v_{n}}{1-\beta_2^n}\\
\;\\
\eta_n =\frac{lr}{\sqrt{V_n}+\epsilon}\\
\;\\
p_n = p_{n-1} -\eta_nM_n
$$

### 4.3 Adam 算法优化
参考文献 Adam: A Method for Stochastic Optimization[https://arxiv.org/abs/1412.6980](https://arxiv.org/abs/1412.6980)
令$\epsilon =0$,将$M_t, \; V_t$代入$p_t = p_{t-1} -\eta_tM_t$, 得:
$$
\eta_t =\frac{lr}{\sqrt{\frac{v_t}{1-\beta_2^t}}}\\
\;\\
p_t = p_{t-1} -\eta_t\frac{m_t}{1-\beta_1^t}\\
\;\\
p_t = p_{t-1} -\frac{lr}{\sqrt{\frac{v_t}{1-\beta_2^t}}}\frac{m_t}{1-\beta_1^t}
$$
在分母位补上$\epsilon$, 得 :
$$
\alpha_t = lr * \frac{\sqrt{1-\beta_2^t}}{(1-\beta_1^t)} \\
\;\\
p_t=p_{t-1} -  \alpha_t\frac{m_t}{\sqrt{v_t}+\epsilon}
$$



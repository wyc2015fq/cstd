
# dropout函数详解及反向传播中的梯度求导 - BrightLamp的博客 - CSDN博客


2018年11月21日 16:56:17[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：598所属专栏：



## 摘要
本文给出 dropout 函数的定义, 并求解其在反向传播中的梯度
## 相关
*配套代码, 请参考文章 :*
Python和PyTorch对比实现dropout函数及反向传播
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
## 1. dropout 的概念
深度神经网络结构的过拟合是指 : 在训练集上的正确率很高, 但在测试集上的准确率很低.
为了缓解网络过拟合的问题, 其中一种常见的办法是使用 dropout 函数.
*dropout 是指在深度网络的训练中, 以一定的概率随机地 “临时丢弃” 一部分神经元节点. 具体来讲, dropout 作用于每份小批量训练数据, 由于其随机丢弃部分神经元的机制, 相当于每次迭代都在训练不同结构的神经网络.*
– 摘抄自 <百面机器学习>
dropout 前网络结构示意 :
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121161818628.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)
图片来源 :[https://www.jianshu.com/p/2c6978b4bf74](https://www.jianshu.com/p/2c6978b4bf74)
[
](https://img-blog.csdnimg.cn/20181121161818628.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)dropout 后网络结构示意 :
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)
图片来源 :[https://www.jianshu.com/p/2c6978b4bf74](https://www.jianshu.com/p/2c6978b4bf74)
[
](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)
## 2. dropout 的作用
[
](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)dropout 最直接的作用就是可以提升模型的泛化能力, 提高模型的健壮性, 提高通用性.
[
](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)如何理解 ?
[
](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)假设某公司存在一个职能稳定, 合理分工团队.
[
](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)因为某些不可避免的原因, 该团队的成员每天都有 50% 概率不能参与工作.
[
](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)为了完成任务, 需要其他同事加班完成缺席员工任务.
[
](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)一段时间后, 该团队的成员普遍学会了其他同事的相关工作技能.
[
](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)于是, 该团队拥有了更好的泛化能力.
[
](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)
## 3. 训练过程中的 dropout
[
](https://img-blog.csdnimg.cn/20181121161754322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)为了定义 dropout, 我们需要一个随机数生成函数 rand(), 每一次调用都会重新生成一个0~1之间的小数 :
$$
0\leqslant rand() &lt; 1
\;\\
$$
假设每一次 dropout 都以 p 的概率随机丢弃一个节点 :
$$
0\leqslant p &lt; 1
$$
则 dropout 函数的定义为 :
$$
r = rand()\\
\;\\
dropout(x) = 
\left\{
             \begin{array}{rr} 
             0, &amp;  r  &lt; p\\
             x / (1 - p), &amp; r  \geqslant p
             \end{array}
\right.
$$
将通过的信号被放大$1/(1 - p)$倍, 保证总体信号强度不变 (加班).
dropout函数是逐个元素处理的, 并不需要写成向量的形式.
但为了方便编程实现, 这里将其进行改编成向量的形式.
考虑一个输入向量 x, 同时定义一个随机掩码函数和一个随机掩码向量 m :
$$
x = (x_1,x_2,x_3,\cdots,x_k)\\
\;\\
r_i = rand()\\
\;\\
randomMask(x_i) = 
\left\{
             \begin{array}{rr} 
             0, &amp;  r_i  &lt; p\\
             1/ (1 - p), &amp; r_i  \geqslant p
             \end{array}
\right.\\
\;\\
m_i = randomMask(x_i)
$$
dropout 函数可用向量运算表示为 :
$$
dropout(x)=x \odot m = (x_1m_1,x_2m_2,x_3m_3,\cdots,x_km_k)
$$
式子中的$\odot$表示元素积, 即同位元素相乘.
## 3. 测试过程中的 dropout
训练完成后, 恢复完整的网络结构:
$$
dropout(x) = x
$$

## 4. 反向传播 :
考虑一个输入向量 x , 经丢弃概率为$p$的 dropout 函数变换后得到向量 d, 往前 forward 传播得到误差值 error (标量 e ),  求在训练过程中 e 对 x 的梯度.
$$
x =( x_1,x_2,x_3,\cdots,x_k) \\
\;\\
d = dropout(x)\\
\;\\
e = forward(d)
$$
求解过程 :
$$
\nabla e_{(d)} = (\frac{\partial e}{\partial d_1},\frac{\partial e}{\partial d_2},\frac{\partial e}{\partial d_3}, \cdots ,\frac{\partial e}{\partial d_k})  \\
\;\\
\frac{\partial e}{\partial x_i} = \frac{\partial e}{\partial d_i}\frac{\partial d_i}{\partial x_i} \\
\;\\
\frac{\partial d_i}{\partial x_i}   = 
\left\{
             \begin{array}{rr} 
             0, &amp;  r_i  &lt; p\\
             1 / (1 - p), &amp; r_i  \geqslant p
             \end{array}
\right.
$$
利用上例的掩码向量, 梯度可用向量表示为 :
$$
\nabla e_{(x)} = \nabla e_{(d)}\odot (m_1,m_2,m_3,\cdots,m_k)
$$
向量$\nabla e_{(d)}$由上游负责计算, 是已知的,$\odot$表示元素积, 即同位元素相乘.



# 深度学习笔记：优化方法总结(BGD,SGD,Momentum,AdaGrad,RMSProp,Adam) - Multiangle's Notepad - CSDN博客


2016年11月01日 00:31:23[multiangle](https://me.csdn.net/u014595019)阅读数：58934标签：[深度学习																](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)[优化																](https://so.csdn.net/so/search/s.do?q=优化&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)个人分类：[机器学习&深度学习																](https://blog.csdn.net/u014595019/article/category/3051069)
所属专栏：[深度学习&tensorflow笔记](https://blog.csdn.net/column/details/13414.html)



[深度学习笔记(一)：logistic分类](http://blog.csdn.net/u014595019/article/details/52554582)
[深度学习笔记(二)：简单神经网络，后向传播算法及实现](http://blog.csdn.net/u014595019/article/details/52557623)
[深度学习笔记(三)：激活函数和损失函数](http://blog.csdn.net/u014595019/article/details/52562159)
[深度学习笔记：优化方法总结](http://blog.csdn.net/u014595019/article/details/52989301)
[深度学习笔记(四)：循环神经网络的概念，结构和代码注释](http://blog.csdn.net/u014595019/article/details/52571966)
[深度学习笔记(五)：LSTM](http://blog.csdn.net/u014595019/article/details/52605693)
[深度学习笔记(六)：Encoder-Decoder模型和Attention模型](http://blog.csdn.net/u014595019/article/details/52826423)
---
最近在看Google的Deep Learning一书，看到优化方法那一部分，正巧之前用tensorflow也是对那些优化方法一知半解的，所以看完后就整理了下放上来，**主要是一阶的梯度法，包括SGD, Momentum, Nesterov Momentum, AdaGrad, RMSProp, Adam。**其中SGD,Momentum,Nesterov Momentum是手动指定学习速率的,而后面的AdaGrad, RMSProp, Adam,就能够自动调节学习速率.
二阶的方法目前我水平太差，看不懂….就不放上来了。
---

# BGD
即batch gradient descent. 在训练中,每一步迭代都使用训练集的所有内容. 也就是说,利用现有参数对训练集中的**每一个输入**生成一个估计输出$\hat{y_i}$,然后跟实际输出$y_i$比较,统计所有误差,求平均以后得到平均误差,以此来作为更新参数的依据.
**具体实现:**
需要:**学习速率****\epsilon****, 初始参数****\theta**
每步迭代过程:
1. 提取训练集中的所有内容$\left\{ x_1,\ldots,x_n \right\}$,以及相关的输出$y_i$
2. 计算梯度和误差并更新参数:

$$
\begin{align}
&\hat g \leftarrow +\frac{1}{n}\nabla_\theta \sum_i L(f(x_i;\theta),y_i) \\
&\theta \leftarrow \theta-\epsilon\hat g
\end{align}
$$
**优点:**
由于每一步都利用了训练集中的所有数据,因此当损失函数达到最小值以后,能够保证此时计算出的梯度为0,换句话说,就是能够收敛.因此,使用BGD时不需要逐渐减小学习速率$\epsilon_k$
**缺点:**
由于每一步都要使用所有数据,因此随着数据集的增大,运行速度会越来越慢.
---

# SGD
SGD全名 stochastic gradient descent， 即随机梯度下降。不过这里的SGD其实跟MBGD(minibatch gradient descent)是一个意思,即随机抽取一批样本,以此为根据来更新参数.
**具体实现:**
需要:**学习速率****\epsilon****, 初始参数****\theta**
每步迭代过程:
1. 从训练集中的随机抽取一批容量为m的样本$\left\{ x_1,\ldots,x_m \right\}$,以及相关的输出$y_i$
2. 计算梯度和误差并更新参数:

$$
\begin{align}
& \hat g \leftarrow +\frac{1}{m}\nabla_\theta \sum_i L(f(x_i;\theta),y_i)\\
& \theta \leftarrow \theta-\epsilon\hat g
\end{align}
$$
**优点:**
训练速度快,对于很大的数据集,也能够以较快的速度收敛.
**缺点:**
由于是抽取,因此不可避免的,得到的梯度肯定有误差.因此学习速率需要逐渐减小.否则模型无法收敛
因为误差,所以每一次迭代的梯度受抽样的影响比较大,也就是说梯度含有比较大的噪声,不能很好的反映真实梯度.
**学习速率该如何调整:**
那么这样一来,$\epsilon$如何衰减就成了问题.如果要保证SGD收敛,应该满足如下两个要求:

$$
\begin{align}
&\sum_{k=1}^\infty \epsilon_k = \infty \\
&\sum_{k=1}^\infty \epsilon_k^2 <\infty
\end{align}
$$
而在实际操作中,一般是进行线性衰减:

$$
\begin{align}
&\epsilon_k=(1-\alpha)\epsilon_0+\alpha\epsilon_\tau\\
&\alpha = \frac{k}{\tau}
\end{align}
$$
其中$\epsilon_0$是初始学习率,$\epsilon_\tau$是最后一次迭代的学习率.$\tau$自然代表迭代次数.一般来说,**\epsilon_\tau****设为****\epsilon_0****的1%比较合适.而****\tau****一般设为让训练集中的每个数据都输入模型上百次比较合适**.那么初始学习率$\epsilon_0$怎么设置呢?书上说,你**先用固定的学习速率迭代100次,找出效果最好的学习速率,然后****\epsilon_0****设为比它大一点就可以了.**
---

# Momentum
上面的SGD有个问题,就是每次迭代计算的梯度含有比较大的噪音. 而Momentum方法可以比较好的缓解这个问题,尤其是**在面对小而连续的梯度但是含有很多噪声的时候,可以很好的加速学习**.Momentum借用了物理中的动量概念,即前几次的梯度也会参与运算.为了表示动量,引入了一个新的变量v(velocity).v是之前的梯度的累加,但是每回合都有一定的衰减.
**具体实现:**
需要:**学习速率****\epsilon****, 初始参数****\theta****, 初始速率v, 动量衰减参数****\alpha**
每步迭代过程:
1. 从训练集中的随机抽取一批容量为m的样本$\left\{ x_1,\ldots,x_m \right\}$,以及相关的输出$y_i$
2. 计算梯度和误差,并更新速度v和参数$\theta$:

$$
\begin{align}
& \hat g \leftarrow +\frac{1}{m}\nabla_\theta \sum_i L(f(x_i;\theta),y_i)\\
& v\leftarrow\alpha v-\epsilon\hat g \\
& \theta\leftarrow\theta+v
\end{align}
$$
其中参数$\alpha$表示每回合速率v的衰减程度.同时也可以推断得到,如果每次迭代得到的梯度都是g,那么最后得到的v的稳定值为

$$
\frac{\epsilon\lVert g\rVert}{1-\alpha}
$$
也就是说,Momentum最好情况下能够将学习速率加速$\frac{1}{1-\alpha}$倍.一般$\alpha$的取值有0.5,0.9,0.99这几种.当然,也可以让$\alpha$的值随着时间而变化,一开始小点,后来再加大.不过这样一来,又会引进新的参数.
**特点:**
前后梯度方向一致时,能够加速学习
前后梯度方向不一致时,能够抑制震荡
---

# Nesterov Momentum
这是对之前的Momentum的一种改进,大概思路就是,先对参数进行估计,然后使用估计后的参数来计算误差
**具体实现:**
需要:**学习速率****\epsilon****, 初始参数****\theta****, 初始速率v, 动量衰减参数****\alpha**
每步迭代过程:
1. 从训练集中的随机抽取一批容量为m的样本$\left\{ x_1,\ldots,x_m \right\}$,以及相关的输出$y_i$
2. 计算梯度和误差,并更新速度v和参数$\theta$:

$$
\begin{align}
& \hat g \leftarrow +\frac{1}{m}\nabla_\theta \sum_i L(f(x_i;\theta+\alpha v),y_i)\\
& v\leftarrow\alpha v-\epsilon\hat g \\
& \theta\leftarrow\theta+v
\end{align}
$$
注意在估算$\hat g$的时候,参数变成了$\theta+\alpha v$而不是之前的$\theta$
---

# AdaGrad
AdaGrad可以自动变更学习速率,只是需要设定一个全局的学习速率$\epsilon$,但是这并非是实际学习速率,实际的速率是与以往参数的模之和的开方成反比的.也许说起来有点绕口,不过用公式来表示就直白的多:

$$
\epsilon_n=\frac{\epsilon}{\delta+\sqrt{\sum_{i=1}^{n-1}g_i\odot g_i}}
$$
其中$\delta$是一个很小的常亮,大概在$10^{-7}$,防止出现除以0的情况.
**具体实现:**
需要:**全局学习速率****\epsilon****, 初始参数****\theta****, 数值稳定量****\delta**
中间变量: 梯度累计量r(初始化为0)
每步迭代过程:
1. 从训练集中的随机抽取一批容量为m的样本$\left\{ x_1,\ldots,x_m \right\}$,以及相关的输出$y_i$
2. 计算梯度和误差,更新r,再根据r和梯度计算参数更新量

$$
\begin{align}
& \hat g \leftarrow +\frac{1}{m}\nabla_\theta \sum_i L(f(x_i;\theta),y_i)\\
& r\leftarrow r+\hat g\odot \hat g \\
& \triangle \theta = -\frac{\epsilon}{\delta+\sqrt{r}}\odot \hat g \\
& \theta\leftarrow\theta+\triangle \theta
\end{align}
$$
**优点:**
能够实现学习率的自动更改。如果这次梯度大,那么学习速率衰减的就快一些;如果这次梯度小,那么学习速率衰减的就满一些。
**缺点:**
任然要设置一个变量$\epsilon$
经验表明，在普通算法中也许效果不错，但在深度学习中，深度过深时会造成训练提前结束。
---

# RMSProp
RMSProp通过引入一个衰减系数，让r每回合都衰减一定比例，类似于Momentum中的做法。
**具体实现:**
需要:**全局学习速率****\epsilon****, 初始参数****\theta****, 数值稳定量****\delta****，衰减速率****\rho**
中间变量: 梯度累计量r(初始化为0)
每步迭代过程:
1. 从训练集中的随机抽取一批容量为m的样本$\left\{ x_1,\ldots,x_m \right\}$,以及相关的输出$y_i$
2. 计算梯度和误差,更新r,再根据r和梯度计算参数更新量

$$
\begin{align}
& \hat g \leftarrow +\frac{1}{m}\nabla_\theta \sum_i L(f(x_i;\theta),y_i)\\
& r\leftarrow \rho r+(1-\rho)\hat g\odot \hat g \\
& \triangle \theta = -\frac{\epsilon}{\delta+\sqrt{r}}\odot \hat g \\
& \theta\leftarrow\theta+\triangle \theta
\end{align}
$$
**优点：**
相比于AdaGrad,这种方法很好的解决了深度学习中过早结束的问题
适合处理非平稳目标，对于RNN效果很好
**缺点：**
又引入了新的超参，衰减系数$\rho$
依然依赖于全局学习速率
---

# RMSProp with Nesterov Momentum
当然，也有将RMSProp和Nesterov Momentum结合起来的
**具体实现:**
需要:**全局学习速率****\epsilon****, 初始参数****\theta****, 初始速率v，动量衰减系数****\alpha****, 梯度累计量衰减速率****\rho**
中间变量: 梯度累计量r(初始化为0)
每步迭代过程:
1. 从训练集中的随机抽取一批容量为m的样本$\left\{ x_1,\ldots,x_m \right\}$,以及相关的输出$y_i$
2. 计算梯度和误差,更新r,再根据r和梯度计算参数更新量

$$
\begin{align}
& \tilde \theta \leftarrow \theta + \alpha v \\
& \hat g \leftarrow +\frac{1}{m}\nabla_{\tilde \theta} \sum_i L(f(x_i;\tilde \theta),y_i)\\
& r\leftarrow \rho r+(1-\rho)\hat g\odot \hat g \\
& v \leftarrow \alpha v-\frac{\epsilon}{\sqrt r}\odot \hat g \\
& \theta\leftarrow\theta+v
\end{align}
$$

---

# Adam
Adam(Adaptive Moment Estimation)本质上是带有动量项的RMSprop，它利用梯度的一阶矩估计和二阶矩估计动态调整每个参数的学习率。Adam的优点主要在于经过偏置校正后，每一次迭代学习率都有个确定范围，使得参数比较平稳。
**具体实现:**
需要:**步进值****\epsilon****, 初始参数****\theta****, 数值稳定量****\delta****，一阶动量衰减系数****\rho_1****, 二阶动量衰减系数****\rho_2**
其中几个取值一般为：$\delta=10^{-8}, \rho_1 = 0.9, \rho_2=0.999$
中间变量：一阶动量s，二阶动量r,都初始化为0
每步迭代过程:
1. 从训练集中的随机抽取一批容量为m的样本$\left\{ x_1,\ldots,x_m \right\}$,以及相关的输出$y_i$
2. 计算梯度和误差,更新r和s,再根据r和s以及梯度计算参数更新量

$$
\begin{align}
& g \leftarrow +\frac{1}{m}\nabla_{\theta} \sum_i L(f(x_i;\theta),y_i)\\
& s \leftarrow \rho_1s+(1-\rho_1)g \\
& r\leftarrow \rho_2 r+(1-\rho_2)g\odot g \\
& \hat s \leftarrow \frac{s}{1-\rho_1} \\
& \hat r \leftarrow \frac{r}{1-\rho_2} \\
& \triangle \theta = -\epsilon \frac{\hat s}{\sqrt{\hat r}+\delta} \\
& \theta\leftarrow\theta+\triangle \theta
\end{align}
$$



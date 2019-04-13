
# 生成对抗网络 GAN 的数学原理 - BrightLamp的博客 - CSDN博客


2019年01月20日 23:08:11[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：69



## 摘要
本文从概率分布及参数估计说起, 通过介绍极大似然估计, KL 散度, JS 散度, 详细的介绍了 GAN 生成对抗网络的数学原理.
## 相关
系列文章索引 :
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
无论是黑白图片或彩色图片, 都是使用 0 ~ 255 的数值表示像素. 将所有的像素值除以 255 我们就可以将一张图片转化为 0 ~ 1 的概率分布, 而且这种转化是可逆的, 乘以 255 就可以还原.
从某种意义上来讲, GAN 图片生成任务就是生成概率分布. 因此, 我们有必要结合概率分布来理解 GAN 生成对抗网络的原理.
### 1. 概率分布及参数估计
假设一个抽奖盒子里有45个球, 其编号是 1~9 共9个数字. 每个编号的球拥有的数量是:
|编号|1|2|3|4|5|6|7|8|9|
|---|---|---|---|---|---|---|---|---|---|
|数量|2|4|6|8|9|7|5|3|1|
|占比|0.044|0.088|0.133|0.178|0.200|0.156|0.111|0.066|0.022|
**占比**是指用每个编号的数量除以所有编号的总数量, 在数理统计学中, 在不引起误会的情况下, 这里的占比也可以被称为**概率/频率**.
使用向量$q$表示上述的概率分布 :
$$
q = (2,4,6,8,9,7,5,3,1)/45
\;\\
=(0.044, 0.088, 0.133, 0.178, 0.200, 0.156, 0.111, 0.066, 0.022)
$$
将上述分布使用图像绘制如下 :
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120230350421.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)
现在我们希望构建一个函数$p = p(x;\theta)$, 以$x$为编号作为输入数据, 输出编号$x$的概率.$\theta$是参与构建这个函数的参数, 一经选定就不再变化.
假设上述概率分布服从二次抛物线函数 :
$$
p=p(x;\theta)=\theta_1 (x+\theta_2)^2+\theta_3\\
\;\\
x = (1,2,3,4,5,6,7,8,9)
$$
使用 L2 误差作为评价拟合效果的损失函数, 总误差值为 error (标量 e) :
$$
e = \sum_{i=1}^{9}(p_i-q_i)^2
$$
我们希望求得一个$\theta^*$, 使得$e$的值越小越好, 使用数学公式表达是这样子的 :
$$
\theta^* = \underset{\theta}{argmin}(e)
$$
argmin 是 argument minimum 的缩写.
如何求$\theta^*$不是本文的重点, 这是生成对抗网络的任务. 为了帮助理解, 取其中一个可能的数值作为示例 :
$$
\theta^* = (\theta_1,\theta_2,\theta_3)=(-0.01,-5.0,0.2)\\
\;\\
p=p(x;\theta)=-0.01 (x-5.0)^2+0.2
$$
绘制函数图像如下 :
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120230411251.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29CcmlnaHRMYW1w,size_16,color_FFFFFF,t_70)
在生成对抗网络中, 本例的估计函数$p(x;\theta)$相当于生成模型 (generator), 损失函数相当于鉴别模型 (discriminator).
### 2. 极大似然估计
在上例中, 我们很幸运的知道了所有可能的概率分布, 并让求解最优的概率分布估计函数$p(x;\theta)$成为可能.
如果上例的抽奖盒子 (简称样本) 中的 45 个球是从更大的抽奖池 (简称总体) 中选择出来的, 而我们不知道抽奖池中所有球的数量及其编号. 那么, 我们如何根据现有的 45 个球来估计抽奖池的概率分布呢? 我们当然可以直接用上例求得的样本估计函数来代表抽奖池的概率分布, 但本例会介绍一种更常用的估计方法.
假设$p(x)=p(x;\theta)$是总体的概率分布函数. 则编号$x = (x_1,x_2,x_3,\cdots,x_n)$出现的概率为 :
$$
p = p(x_1),p(x_2),p(x_3),\cdots,p(x_n)
$$
在本例中,$n = 9$, 即共 9 个编号.
设$d=(d_1,d_2,d_3,d_3\cdots,d_m)$是所有的抽样的编号. 在本例中,$m = 45$, 即样本中共有 45 个抽样. 假设所有的样本和抽样都是独立的, 则样本出现的概率为 :
$$
\rho= p(d_1)\times p(d_2)\times p(d_3)\times\cdots\times p(d_m)=\prod_{i=1}^{m}p(d_i)
$$
$p(x)=p(x;\theta)$的函数结构是人为按经验选取的, 比如线性函数, 多元二次函数, 更复杂的非线性函数等, 一经选取则不再改变. 现在我们需要求解一个参数集$\theta^*$, 使得$\rho$的值越大越好. 即
$$
\theta^* = \underset{\theta}{argmax}(\rho)=\underset{\theta}{argmax}\prod_{i=1}^{m}p(d_i;\theta)
$$
argmax 是 argument maximum 的缩写.
通俗来讲, 因为样本是实际已发生的事实, 在函数结构已确定的情况下, 我们需要尽量优化参数, 使得样本的理论估计概率越大越好.
这里有一个前提, 就是人为选定的函数结构应当能够有效评估样本分布. 反之, 如果使用线性函数去拟合正态概率分布 (normal distribution), 则无论如何选择参数都无法得到满意的效果.
连乘运算不方便, 将之改为求和运算. 由于$log$对数函数的单调性, 上面的式子等价于 :
$$
\theta^* =\underset{\theta}{argmax}\;log\prod_{i=1}^{m}p(d_i;\theta)=\underset{\theta}{argmax}\sum_{i=1}^{m}log\;p(d_i;\theta)
$$
设样本分布为$q(x)$, 对于给定样本, 这个分布是已知的, 可以通过统计抽样的计算得出. 将上式转化成期望公式 :
$$
\theta^* =\underset{\theta}{argmax}\sum_{i=1}^{m}log\;p(d_i;\theta)
=\underset{\theta}{argmax}\sum_{i=1}^{n}q(x_i)\;log\;p(x_i;\theta)
$$
注意上式中的两个求和符号,$m$变成了$n$. 在大多数情况下, 编号数量会比抽样数量少, 转为期望公式可以显著减少计算量.
在一些教材中, 上式的写法是:
$$
\theta^*=\underset{\theta}{argmax}\; E_{x-q(x)}log\;p(x;\theta)
=\underset{\theta}{argmax}\int q(x)\;log\;p(x;\theta)dx
$$
$E_{x-q(x)}$表示按$q(x)$的分布对$x$求期望. 因为积分表达式比较简洁, 书写方便, 下文开始将主要使用积分表达式.
以上就是极大似然估计(Maximum Likelihood Estimation) 的理论和推导过程. 和上例的参数估计方法相比, 极大似然估计的方法, 因为无需设计损失函数, 降低了模型的复杂度, 扩大了适用范围.
本例中的估计函数$p(x;\theta)$相当于生成对抗网络的生成模型, 样本分布$q(x)$相当于训练数据.
### 3. KL散度
结合上例, 在样本已知的情况下,$q(x)$是一个已知且确定的分布. 则$\int q(x)\;log\;q(x)dx$是一个常数项, 不影响$\theta^*$求解的结果.
$$
\theta^*=\underset{\theta}{argmax}(\int q(x)\;log\;p(x;\theta)dx-\int q(x)\;log\;q(x)dx)\\
\;\\
=\underset{\theta}{argmax}\int q(x)\;(log\;p(x;\theta)-log\;q(x))dx\\
\;\\
=\underset{\theta}{argmax}\int q(x)\;log\;\frac{p(x;\theta)}{q(x)}dx\\
$$
也可以写成这样 :
$$
\theta^*=\underset{\theta}{argmin}(-\int q(x)\;log\;p(x;\theta)dx+\int q(x)\;log\;q(x)dx)\\
\;\\
=\underset{\theta}{argmin}\int q(x)\;log\;\frac{q(x)}{p(x;\theta)}dx\\
$$
KL 散度 ( Kullback–Leibler divergence ) 是一种衡量两个概率分布的匹配程度的指标, 两个分布差异越大, KL散度越大. 它还有很多名字, 比如: relative entropy, relative information.
其定义如下 :
$$
D_{KL}(q||p)=\int q(x)\;log\;\frac{q(x)}{p(x)}dx
$$
当$p(x)\equiv q(x)$时取得最小值$D_{KL}(q||p) = 0$.
我们可以将上面的公式简化为 :
$$
\theta^*=\underset{\theta}{argmin}\;D_{KL}(q||p(x;\theta))
$$

### 4. JS 散度
KL 散度是非对称的，即$D_{KL}(q||p)$不一定等于$D_{KL}(p||q)$. 为了解决这个问题, 需要引入 JS 散度.
JS 散度 ( Jensen–Shannon divergence ) 的定义如下 :
$$
m =\frac{1}{2}(p + q) \\
\;\\
D_{JS}=\frac{1}{2}D_{KL}(p||m) + \frac{1}{2}D_{KL}(q||m)
$$
JS 的值域是对称的, 有界的, 范围是 [0,1].
如果 p, q 完全相同, 则 JS = 0, 如果完全不相同, 则 JS = 1.
注意, KL 散度和 JS 散度作为差异度量的时候, 有一个问题：
如果两个分配 p, q 离得很远, 完全没有重叠的时候, 那么 KL 散度值是没有意义的, 而 JS 散度值是一个常数. 这在学习算法中是比较致命的, 这就意味这这一点的梯度为0, 梯度消失了.
参考上例, 对 JS 散度进行反推:
$$
D_{JS}(q||p)=\frac{1}{2}D_{KL}(q||m)+\frac{1}{2}D_{KL}(p||m)\\
\;\\
=\frac{1}{2}\int q(x)\;log\;\frac{q(x)}{\frac{q(x)+p(x;\theta)}{2}}dx+
\frac{1}{2}\int p(x;\theta)\;log\;\frac{p(x;\theta)}{\frac{p(x;\theta)+q(x)}{2}}dx\\
\;\\
=\frac{1}{2}\int q(x)\;log\;\frac{2q(x)}{q(x)+p(x;\theta)}dx+
\frac{1}{2}\int p(x;\theta)\;log\;\frac{2p(x;\theta)}{p(x;\theta)+q(x)}dx
$$
由于 :
$$
\int q(x)\;log\;\frac{2q(x)}{q(x)+p(x;\theta)}dx\\
\;\\
=\int q(x)\;(log\;\frac{q(x)}{q(x)+p(x;\theta)}+log2)dx\\
\;\\
=\int q(x)\;log\;\frac{q(x)}{q(x)+p(x;\theta)}dx+\int q(x)(log2)dx\\
\;\\
=\int q(x)\;log\;\frac{q(x)}{q(x)+p(x;\theta)}dx+log2
$$
同理可得 :
$$
D_{JS}(q||p)=\frac{1}{2}\int q(x)\;log\;\frac{q(x)}{q(x)+p(x;\theta)}dx+
\frac{1}{2}\int p(x;\theta)\;log\;\frac{p(x;\theta)}{p(x;\theta)+q(x)}dx+log2
$$
令 :
$$
d(x;\theta)=\frac{q(x)}{q(x)+p(x;\theta)}
$$
则 :
$$
1-d(x;\theta)=\frac{p(x;\theta)}{q(x)+p(x;\theta)}
$$
即 :
$$
D_{JS}(q||p)=\frac{1}{2}\int q(x)\;log\;d(x;\theta)dx+
\frac{1}{2}\int p(x;\theta)\;log\;(1-d(x;\theta))dx+log2
$$
令 :
$$
V(x;\theta)
=\int q(x)\;log\;d(x;\theta)dx+
\int p(x;\theta)\;log\;(1-d(x;\theta))dx
$$
则 :
$$
D_{JS}(q||p)=\frac{1}{2}V(x;\theta)+log2
$$
即 :
$$
\theta^*=\underset{\theta}{argmin}\;D_{JS}(q||p)=\underset{\theta}{argmin}\;V(x;\theta)
$$
此时,$\theta^*$是令$p(x;\theta)$和$q(x)$差异最小的参数. 同样亦可通过$V(x;\theta)$求差异最大的参数.
### 5. JS 散度参数求解的两步走迭代方法
从上例的讨论我们知道, 我们需要求得一个参数$\theta^*$, 使得
$$
\theta^*=\underset{\theta}{argmin}D_{JS}(q||p)=\underset{\theta}{argmin} V(x;\theta)
$$
然而, 因为涉及多重嵌套和积分, 使用起来并不方便.
首先, 我们假设$p(x;\theta) = p_g(x)$为已知条件, 同时令$D=d(x;\theta)$, 考虑这个式子:
$$
W(x;\theta)=q(x)\;log\;d(x;\theta)dx+
p(x;\theta)\;log\;(1-d(x;\theta))\\
\;\\
W(x;D)=q(x)\;log\;D+p_g(x)\;log\;(1-D)\\
\;\\
V(x;\theta)=V(x;D)=\int W(x;D)dx
$$
在$x$已经确定的情况下, 我们关注$D$.
$$
W&\#x27;=\frac{dW}{dD}=q(x)\frac{1}{D}-p_g(x)\frac{1}{1-D}\\
\;\\
W&\#x27;&\#x27;=\frac{dW&\#x27;}{dD}=-q(x)\frac{1}{D^2}-p_g(x)\frac{1}{(1-D)^2}
$$
因为$W&\#x27;&\#x27; &lt;  0$, 当$W&\#x27;=0$时,$W$取得极大值 :
$$
W&\#x27;=q(x)\frac{1}{D}-p_g(x)\frac{1}{1-D}=0\\
\;\\
D = \frac{q(x)}{q(x)+p_g(x)}
$$
因为 :
$$
D &lt; \frac{q(x)}{q(x)+p_g(x)},\;\;W&\#x27;&gt;0\\
\;\\
D &gt; \frac{q(x)}{q(x)+p_g(x)},\;\;W&\#x27;&lt;0
$$
这表明, 当$D$的函数按$W&\#x27;=0$取值时,$W$在$x$的每个取样点均获得最大值, 积分后的面积获得最大值, 即 :
$$
D^*=\frac{q(x)}{q(x)+p_g(x)}=\underset{D}{argmax}\int W(x;D)dx=\underset{D}{argmax}V(x;D)
$$

$$
\underset{D}{max}\;V(x;D)=\int q(x)\;log\;D^*(x)dx+\int p_g(x)\;log\;(1-D^*(x))dx
$$
在得到$V(x;D)$的最大值表达式后, 我们固定$D^*$, 接着对$p(x;\theta) = p_g(x)$将这个最大值的按最小方向优化 :
$$
V(x;\theta;D^*)=\int q(x)\;log\;D^*(x)\;dx+\int p(x;\theta)\;log\;(1-D^*(x))dx\\
\;\\
\theta^*=\underset{\theta}{argmin}\;V(x;\theta^*;D^*)
$$
由此, 通过两步走的方法, 经过多次先后迭代求解$D^*$和$\theta^*$, 我们可以逐渐得到一个趋近于$q(x)$的$p(x;\theta^*)$.
### 6. 生成对抗网络
从上面的讨论方法可知, 我们可以得到一个和$q(x)$非常接近的分布函数$p(x;\theta)$. 这个分布函数的构建是为了寻找已知样本数据的内在规律.
然而我们往往并不关心这个分布函数. 我们希望无中生有的构建一批数据$x&\#x27;$, 使得$p(x&\#x27;;\theta)$趋近于$q(x)$.
我们设计一个输出$x&\#x27;$的生成器$x&\#x27;=G(z;\beta)$, 从随机概率分布中接收$z$作为输入,$x&\#x27;$的概率分布为$p_g(x&\#x27;)$.
第一步, 我们固定$p_g(x&\#x27;)$求$D^*$.
$$
V(x,x&\#x27;;D)=\int q(x)\;log\;D(x)\;dx+\int p_g(x&\#x27;)\;log\;(1-D(x&\#x27;))dx\\
\;\\
D^*=\underset{D}{argmax}V(x;D)
$$
第二步, 我们固定$D^*$求$p_g(x&\#x27;;\theta^*)$.
$$
V(x,x&\#x27;,D^*;\theta)=\int q(x)\;log\;D^*(x)\;dx+\int p_g(x&\#x27;;\theta)\;log\;(1-D^*(x&\#x27;))dx\\
\;\\
\theta^*=\underset{\theta}{argmin}\;V(x,D^*;\theta^*)
$$
然后进行多次循环迭代, 使得$p_g(x&\#x27;;\theta^*)$趋近于$q(x)$.
读者可以发现, 这里求解过程和上例的是一样, 只是输入的数据并不一致.
在实际任务中, 我们并不关心$p_g(x&\#x27;;\theta)$, 仅关注生成器$x&\#x27;=G(z;\beta)$的优化.
由此我们将算法改编如下 :
第一步, 我们固定$x&\#x27;=G(z;\beta)$求$D^*$.
$$
V(x,z;D)=\int q(x)\;log\;D(x)\;dx+\int q(z)\;log\;(1-D(G(z)))dz\\
\;\\
D^*=\underset{D}{argmax}V(x,z;D)
$$
第二步, 我们固定$D^*$求$G(z;\beta^*)$.
$$
V(x,z,D^*;\beta)=\int q(x)\;log\;D^*(x)\;dx+\int q(z)\;log\;(1-D^*(G(z;\beta)))dz\\
\;\\
\beta^*=\underset{\beta}{argmin}\;V(x,z,D^*;\beta)
$$
注意, 本例的两个算法都没有给出严格的收敛证明.
由于求解形式和上例的 JS 散度的参数求解算法非常一致, 我们可以期待这种算法能够起到作用.
为简单起见, 我们记 :
$$
V(G,D)=\int q(x)\;log\;D(x)\;dx+\int q(z)\;log\;(1-D(G(z)))dz\\
\;\\
G^*=\underset{G}{argmin}\;(\;\underset{D}{max}\;V(G,D)\;)
$$
这就是 GAN 生成对抗网络相关文献中常见的求解表达式.
在 Ian J. Goodfellow 的论文 Generative Adversarial Networks 中, 作者先给出了$V(G,D)$的表达式, 然后再通过 JS 散度的理论来证明其收敛性. 有兴趣的读者可以参考阅读.
本文认为, 如果先介绍 JS 散度, 再进行反推, 可以更容易的理解 GAN 概念, 理解 GAN 为什么要用这么复杂的损失函数.
### 7. 生成对抗网络的工程实践
在工程实践中, 我们遇到的一般是离散的数据. 我们可以使用随机采样的方法来逼近期望值.
首先我们从前置的随机分布$p_z(z)$中取出$m$个随机数$z=(z_1,z_2,z_3,\cdots,z_m)$, 其次我们在从真实数据分布$p(x)$中取出$m$个真实样本$p=(x_1,x_2,x_3,\cdots,x_m)$.
由于我们的数据是随机选取的, 概率越大就越有机会被选中. 抽取的样本就隐含了自身的期望. 因此我们可以使用平均数代替上式中的期望, 公式改写如下.
$$
V(G,D)=\int q(x)\;log\;D(x)\;dx+\int q(z)\;log\;(1-D(G(z)))\;dz\\
\;\\
=\frac{1}{m}\sum_{i=1}^{m}log\;D(x_i) + \frac{1}{m}\sum_{i=1}^{m}log\;(1-D(G(z_i)))
$$
我们可以直接用上式训练鉴别器$D(x)​$.
在训练生成器时, 因为前半部分和$z$无关, 我们可以只使用后半部分.
全文完.


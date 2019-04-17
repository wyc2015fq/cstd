# 声学模型学习笔记（一） HMM - xmdxcsj的专栏 - CSDN博客





2016年10月08日 19:45:48[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3013








**“声学模型学习笔记”是《automatic speech recognition a deep learning approach》这本书的读书笔记，会有少量的个人理解和公式详细推导，声学入门狗一枚，不具有指导意义，具体以原书和列出的参考文献为准，欢迎指导和讨论。**

## HMM含义

### Markov Chains

[马尔科夫链](https://en.wikipedia.org/wiki/Markov_chain)，表示一个状态到另一个状态转换的随机过程。该过程要求具备“无记忆”的性质：下一状态的概率分布只能由当前状态决定，在时间序列中它前面的事件均与之无关。这种特定类型的“无记忆性”称作**马尔可夫性质**。 

假设状态空间$q_t\in{s^{(j)}, j=1,2,...,N}$，马尔科夫性质可以表示如下： 


$P(q_{t+1}=s^{({t+1})}|q_{0}=s^{0},...,q_t=s^t)=P(q_{t+1}=s^{({t+1})}|q_t=s^t)$

转移概率可以定义如下： 


$P(q_t=s^{j}|q_{t-1}=s^i)=a_{ij}(t), i,j=1,2,...,N$

如果转移概率跟t无关，只由i和j决定，那么说马尔科夫链是齐次（homogeneous）的，转移概率矩阵可以写成一下形式： 


$A=[a_{ij}], where a_{ij}\ge 0, \forall i,j;and \sum_{j=1}^{N}a_{ij}=1, \forall i$
### Hidden Markov Sequences

马尔科夫链关注的状态转移概率。比如天气的三个状态分别是{sun, cloud, rain}，状态转移概率矩阵如下： 


$ \begin{matrix}     & sun & cloud & rain\\  sun & 0.50 & 0.375 & 0.125 \\  cloud & 0.25 & 0.125 & 0.625 \\  rain & 0.25 & 0.375 & 0.375 \end{matrix}$

马尔科夫链求解的问题是：如果今天晴天(sun)，后天下雨(rain)的概率。 

马尔科夫链的观察序列和状态序列是一一对应的，具有确定性（比如今天是sun，那么sun的概率将会是1，不可能为cloud或者rain），从而导致马尔科夫链很难去拟合有些现实问题。 

为了增加马尔科夫链的随机性，对每一个状态，增加一个观察值的生成概率函数，即为隐马尔科夫序列。使用隐马尔科夫序列去建模解决实际问题，称为Hidden Markov model（HMM）。 

同样以上面的例子来讲，比如在封闭的小黑屋里面，无法观察到天气状况，但是小黑屋里面有种的水藻，水藻的状态和天气的存在一定的概率关系，如下： 


$ \begin{matrix}     & dry & dryish & damp & soggy \\  sun & 0.60 & 0.20 & 0.15 & 0.05 \\  cloud & 0.25 & 0.25 & 0.25 & 0.25 \\  rain & 0.05 & 0.10 & 0.35 & 0.50 \end{matrix}$

此时，我们可以把天气状况当成隐状态，把水藻的状况当成观察值，通过观察值来预测隐状态（天气状态）的序列，马尔科夫链转变成为隐马尔科夫模型。 

不难隐马尔科夫模型有两方面的随机性：底层隐藏的随时间改变的马尔科夫过程（天气状态的变化过程）；与隐藏状态存在某种映射关系的可观察状态集合（天气状态和水藻状态的关系）。
## HMM公式

假设状态序列$q_{1}^{T}=(q_1,...,q_T)$，观察序列$o_1^T=(o_1,...,o_T)$，hmm的三要素$\lambda=(A,B,\pi)$： 

- 转移概率$A=[a_{ij}]$

- 初始状态概率分布$\pi = [\pi_{i}]$

- 观察概率$B=[b_i(k)]$:状态i生成观察值$v_k$的概率
hmm需要解决的三个问题 

- 概率计算问题：已知$\lambda$和$o_1^T$，求解$P(o_1^T|\lambda)$

- 学习问题：已知$o_1^T$和$q_{1}^{T}$，求解$\lambda$

- 预测问题：已知$\lambda$和$o_1^T$，求解概率最大的$(q_{1}^{T})$
### 问题一：为问题二的求解打基础

观察概率用混合高斯建模，可以表示如下： 
$b_i(o_t)=\sum_{m=1}^{M}\frac{c_{i,m}}{(2\pi)^{D/2}|\sum_{i,m}|^{1/2}}exp[-\frac{1}{2}(o_t-\mu_{i,m})^T\sum_{i,m}^{-1}(o_t-\mu_{i,m})]$

其中i表示状态下标，m表示高斯分量的下标。 

1.似然概率： 
$P(o_1^T|q_1^T)=\prod_{t=1}^{T}\sum_{m=1}^{M}\frac{c_{i,m}}{(2\pi)^{D/2}|\sum_{i,m}|^{1/2}}exp[-\frac{1}{2}(o_t-\mu_{i,m})^T\sum_{i,m}^{-1}(o_t-\mu_{i,m})]$

2.状态概率 
$P(q_1^T)=\pi_{q_1}\prod_{t=1}^{T-1}a_{q_tq_{t+1}}$

3.联合概率 
$P(o_1^T, q_1^T)=P(o_1^T|q_1^T)P(q_1^T)$

4.观察概率 
$P(o_1^T)=\sum_{q_1^T}P(o_1^T,q_1^T)$

随着T的增大，穷举状态序列$q_1^T$将会指数增加，$P(o_1^T)$求解的复杂度也会指数增加，考虑引入前后向算法解决$P(o_1^T)$的求解问题。
#### 1.前向变量

前向概率定义为从开始到t时刻状态i的所有路径的概率和，定义如下： 
$\alpha_t(i)=P(q_t=i,o_1^t),t=1,...,T$

初值：$\alpha_1(i)=\pi_ib_i(o_1)$

递推关系：$\alpha_t(j)=\sum_{i=1}^{N}\alpha_{t-1}(i)a_{ij}b_j(o_t), t=2,3,...,T$

终止：$P(o_1^T)=\sum_{i=1}^N\alpha_T(i)$
#### 2.后向变量

后向概率定义为从t时刻状态i点出发到结尾的所有路径概率和，定义如下： 
$\beta_t(i)=P(o_{t+1}^T|q_t=i),t=1,...,T-1$

初值：$\beta_T(i)=1$

递推关系：$\beta_t(i)=\sum_{j=1}^{N}\beta_{t+1}(i)a_{ij}b_j(o_{t+1}), t=1,2,...,T-1$

终止：$P(o_1^T)=\sum_{i=1}^N\pi_ib_i(o_1)\beta_1(i)$
参考[3]，不难得出以上关系。 

t时刻处在状态i的概率可以用前后向变量表示如下： 
$P(q_t=i, o_1^T)=\alpha_t(i)\beta_t(i)$

从而可以达到我们的终极目标，求解： 
$P(O_1^T)=\sum_{i=1}^{N}\alpha_t(i)\beta_t(i)$
**前后向算法的基本原理是利用递推关系，复用了中间的变量，减少了重复计算。不需要穷举所有的路径就可以完成$P(O_1^T)$的求解。**
#### 3.中间变量

两个变量（后面的EM会用到）定义如下： 

t时刻处于状态$q_i$的概率：$\gamma_t(i)=\frac{\alpha_t(i)\beta_t(i)}{\sum_{j=1}^N \alpha_t(j)\beta_t(j)}$

t时刻处在状态$q_i$并且t+1时刻处在$q_j$的概率：$\xi_t(i,j)=\frac{\alpha_t(i)a_{ij}b_j(o_{t+1})\beta_{t+1}(j)}{\sum_{i=1}^N \sum_{j=1}^N\alpha_t(i)a_{ij}b_j(o_{t+1})\beta_t(j)}$
### 问题二：求解hmm的参数

首先参考[3]，简单介绍EM算法： 

EM是expectation-maximization的简称，是一种最优化问题的迭代算法，1977年由Dempster等人总结提出，简介参考[之前的博文](http://blog.csdn.net/xmdxcsj/article/details/48809537)

目标函数采用极大似然估计，极大化观测数据$Y$关于参数$\theta$的对数似然函数 


$L_{max}(\theta)=log P(Y|\theta)=log \sum_Z P(Y,Z|\theta)=log(\sum_Z P(Y|Z,\theta)P(Z|\theta))$

由于含有隐变量$Z$，所以没有解析解，只能通过迭代的方式求解。假设第$i$次迭代以后模型参数为$\theta^i$，我们希望满足$L(\theta)\gt L(\theta^i)$，即每一次迭代都使得似然函数变大，慢慢逼近极大值点，即： 


$L(\theta)- L(\theta^i) \gt 0$


$L(\theta)- L(\theta^i)=log(\sum_Z P(Y|Z,\theta)P(Z|\theta))-logP(Y|\theta^i)$


$=log(\sum_Z P(Z|Y,\theta^i) \frac{P(Y|Z,\theta)P(Z|\theta)} {P(Z|Y,\theta^i)})-logP(Y|\theta^i)$
[Jenssen不等式](https://en.wikipedia.org/wiki/Jensen%27s_inequality)满足： 


$log\sum_j \lambda_j y_j \ge \sum_j \lambda_j log y_i,其中\lambda_i\ge 0, \sum_j \lambda_j=1$

所以 


$L(\theta)- L(\theta^i)\ge \sum_Z P(Z|Y,\theta^i) log\frac{P(Y|Z,\theta)P(Z|\theta)} {P(Z|Y,\theta^i)}-logP(Y|\theta^i)$


$=\sum_Z P(Z|Y,\theta^i) log\frac{P(Y|Z,\theta)P(Z|\theta)} {P(Z|Y,\theta^i)P(Y|\theta^i)}$

定义$L(\theta)$的下限 


$B(\theta, \theta^i)=L(\theta^i) + \sum_Z P(Z|Y,\theta^i) log\frac{P(Y|Z,\theta)P(Z|\theta)} {P(Z|Y,\theta^i)P(Y|\theta^i)}$

满足 


$L(\theta) \ge  B(\theta, \theta^i)$


$L(\theta^i)=B(\theta^i, \theta^i)$

在$\theta^i$点，$L(\theta)$和$B(\theta, \theta^i)$两个曲线相交，其他点$L(\theta)$曲线在$B(\theta, \theta^i)$曲线的上方，如下图： 
![这里写图片描述](https://img-blog.csdn.net/20161008194713110)

为了获得更接近L(θ)L(\theta)的极大值点，使用B(θ,θi)B(\theta, \theta^i)对应的极大值点作为θi+1\theta^{i+1}，当然不能保证此时的θi+1\theta^{i+1}是L(θ)L(\theta)的极大值点，如上图所示。 


θi+1=argmaxθ(B(θ,θi))\theta^{i+1}=argmax_{\theta}(B(\theta, \theta^i))

忽略一些不含θ\theta的项，可得 


θi+1=argmaxθQ(θ,θi)\theta^{i+1}=argmax_{\theta}Q(\theta, \theta^i)

其中Q函数： 


Q(θ,θi)=∑ZP(Z|Y,θi)logP(Y,Z|θ)Q(\theta, \theta^i)=\sum_Z P(Z|Y,\theta^i) log{P(Y,Z|\theta)}

Q函数的数学意义其实就是logP(Y,Z|θ)log{P(Y,Z|\theta)}在已知YY和θi\theta^i条件下关于隐变量ZZ的均值，即 


Q(θ,θi)=∑ZP(Z|Y,θi)logP(Y,Z|θ)=EZ[logP(Y,Z|θ)|Y,θi])Q(\theta, \theta^i)=\sum_Z P(Z|Y,\theta^i) log{P(Y,Z|\theta)}=E_Z[ log{P(Y,Z|\theta)} | Y,\theta^i])
E步的含义是构造logP(Y,Z|θ)log{P(Y,Z|\theta)}在已知YY和θi\theta^i条件下关于隐变量ZZ的均值函数（物理意义是找出L(θ)L(\theta)的下限函数），即为Q函数。M步的含义是对E步构造出的均值函数求极大值，以期望最大程度上逼近L(θ)L(\theta)函数的极大值。

考虑到HMM求解的时候涉及到隐变量qtq_t即某一帧特征属于哪个状态是位置的，所以采用Baum-Welch(EM算法)来进行求解。 

完全数据集定义为y=[oT1,qT1]y=[o_1^T,q_1^T]，由观测变量oT1o_1^T和隐变量qT1q_1^T状态序列共同构成，单高斯的HMM的模型参数θ=[aij,∑i,μi]\theta=[a_{ij}, \sum_i, \mu_i]，由状态的转移概率、高斯均值和方差组成。 

E步：首先构造Q函数 


Q(θ,θ0)=∑qT1P(qT1|oT1,θ0)logP(oT1,qT1|θ)Q(\theta, \theta_0)=\sum_{q_1^T}P(q_1^T|o_1^T,\theta_0)logP(o_1^T,q_1^T|\theta)

M步：然后Q函数分别构造拉格朗日函数，对模型参数θ=[aij,∑i,μi]\theta=[a_{ij}, \sum_i, \mu_i]求导等于0，可得最后的参数（自己没有推导出来，直接上结果吧） 

- 转移概率 


a^ij=∑T−1t=1ξt(i,j)∑T−1t=1γt(i)\hat{a}_{ij}=\frac{\sum_{t=1}^{T-1}\xi_t(i,j)}{\sum_{t=1}^{T-1}\gamma_t(i)}

- 方差 


∑^i=∑Tt=1γt(i)(ot−μ^i)(ot−μ^i)T∑Tt=1γt(i)\hat{\sum}_i=\frac{\sum_{t=1}^T \gamma_t(i)(o_t-\hat{\mu}_i)(o_t-\hat{\mu}_i)^T }{\sum_{t=1}^T \gamma_t(i)}

- 均值 


μ^i=∑Tt=1γt(i)ot∑Tt=1γt(i)\hat{\mu}_i=\frac{\sum_{t=1}^T \gamma_t(i)o_t}{\sum_{t=1}^T \gamma_t(i)}问题三：求解最优状态序列

对应于解码，动态规划的思想，比较简单。HMM局限性- 
the temporal independence of speech data conditioned on each HMM state 

HMM观察独立性假设，任意时刻的观察只依赖于该时刻的马尔科夫链的状态，与其他观测和状态无关- 
lack of lawful correlation between the acoustic features 

高斯的方差是用的对角阵，假设特征维度之间是相互独立的参考文献

[1]《automatic speech recognition a deep learning approach》 chapter2-3 

[2][](http://www.52nlp.cn/hmm-learn-best-practices-one-introduction)HMM理解例子

[3]《统计学习方法》 李航 第10章













# CMU 10703 |Lecture 2 Markov Decision Processes - 浅梦的博客 - CSDN博客





2017年02月08日 14:54:14[浅梦s](https://me.csdn.net/u012151283)阅读数：920








本笔记转载自微信公众号 [机器人学家](http://mp.weixin.qq.com/s?__biz=MzA3NDU2NDk3MQ==&mid=2651928496&idx=1&sn=221f08072b71600af8adba884037948e&chksm=849843b4b3efcaa29f589e92049672bc64ee70fc0baf52468fd7684d4462939a09abc5e25ebb&mpshare=1&scene=1&srcid=0208aDg7C4FqYJ6YlfVqsJeu#rd) ，根据其上发布的CMU翻译讲义进行整理。

# 大纲
- 智能体(Agent)、动作(Action)、奖励(Reward)
- 马尔科夫决策过程(MDP)
- 价值函数(Value function)
- 最优价值函数(Optimal value function)

# 马尔可夫决策过程MDP

![mdp](https://img-blog.csdn.net/20170208144929725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## MDP的特征
- 能够检测成功
- 允许多次尝试并失败

# MDP详解

## 定义
- MDP是一个五元组$<S,A,P,R,\gamma>$
- S是状态的有限集合，状态空间
- A是动作的有限集合，动作空间
- P是状态转移概率矩阵 
$ P^a_{ss'} = \mathbb{P} [S_{t+1} = s'|S_t  = s,A_t = a]$
- R是奖励函数 
$ R^a_s = \mathbb{E}[R_{t+1}|S_t = s, A_t = a]$
- $\gamma$是折扣系数 
$\gamma \in[0,1]$

## 动作
- 假定动作都是离散的
- 对于连续动作，可以离散化

## 状态
- 状态包括智能体在某一时刻获得的自身的信息和环境的信息
- 满足**马尔可夫性质**
$ P(S_{t+1} = s'|S_t,A_t,S_{t-1},A_{t-1},...S_0)=P(S_{t+1} = s'|S_t,A_t)$

即系统下一时刻的状态仅由当前时刻的状态决定，不依赖于以往的任何状态。
## 动态模型 Dynamics
- 描述了一个MDP如何演化，即给定当前状态和动作，状态会如何改变
- model - based 模型相关：动态模型已知或可估计
- model - free 模型无关：不知道MDP的动态模型

## 奖励
- Reward是每次采取action后获得的**即时奖励**。强化学习的目标是最大化未来奖励之和
- 效用回报$G_t$是指**从t时刻所能带来的所有打折后的奖励总和**
- $ G_t=R_{t+1}+\gamma R_{t+2}+...=\sum _{k=0}^ \infty \gamma^kR_{t+k+1}\le R_{max}/(1-\gamma)$
- 更小的$\gamma$意味着更小的horizon-更注重短期

- 有些任务中时间范围有限，可以不使用折扣奖励。对于无限长的任务，必须加上折扣系数才能让$G_t$是有限值。

## 策略
- 定义：策略$\pi$是值**已知状态下可能产生的动作的概率分布**
$\pi(a|s)=\mathbb{P}[A_t = a|S_t = s]$
- 策略完整定义了智能体的行为
- MDP的策略完全取决于当前状态，策略是staionary静态的，只取决于当前时间的状态(time-independent) 
$A_t \sim \pi(.|S_t),\forall t>0$

# MDP求解

目标是找到最优策略，即MDP的最优解。求解过程分为两步 

1. **预测**：给定策略，评估相应的状态价值函数value function和 状态-动作价值函数Q funcition  

2. **控制**：状态值函数和状态-动作值函数求出后，就可以得到当前状态对应的最优动作 |.|状态价值|动作价值|
|----|----|----|
|预测|$V_{\pi}$|$q_{\pi}$|
|控制|$V_*$|$q_*$|

## 价值函数Value Function
- 价值函数是**累计奖励的期望**
- 一个MDP的状态价值函数(state-value function)$v_{\pi}(s)$是从状态s出发使用策略$\pi$所带来的回报的期望 
$v_{\pi}(s)=\mathbb{E_{\pi}}[G_t|S_t = s]$
- 动作价值函数(action-value function)$q_{\pi}(s,a)$是从状态s出发，执行动作a后再使用策略${\pi}$所带来的期望回报。 
$q_{\pi}(s,a)=\mathbb{E_{\pi}}[G_t|S_t=s,A_t=a]$

## 最优价值函数

optimal state-value function 是**所有策略下价值函数的最大值**
$v_*(s)=max_{\pi}v_{\pi}(s)$
$q_*(s,a)=max_{\pi}q_{\pi}(s,a)$
## Bellman 期望方程

![这里写图片描述](https://img-blog.csdn.net/20170208161236702?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从公式上看，我们可以把状态价值函数分解为即时奖励和打折的下一状态的价值函数两部分 

同样，动作价值函数也可做类似的分解 
$v_{\pi}(s)=\mathbb{E_{\pi}}[R_{t+1}+\gamma v_{\pi}(S_{t+1})|S_t=s]$
$q_{\pi}(s,a)=\mathbb{E_{\pi}}[R_{t+1}+\gamma q_{\pi}(S_{t+1},A_{t+1})|S_t=s,A_t=a]$
# 深入分析Bellman方程

下图中，第一层的空心圆圈是当前的状态（state）；向下相连的实心圆圈是在当前状态s下，根据当前的策略，可能采取的动作；第三层的空心圈是采取某一动作后可能的下一时刻的状态s’。  
![这里写图片描述](https://img-blog.csdn.net/20170208162325423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170208162443253?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

状态价值函数和动作价值函数的关系 
![这里写图片描述](https://img-blog.csdn.net/20170208162657600?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

方程求解 
![这里写图片描述](https://img-blog.csdn.net/20170208163318312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 两个最优价值函数之间的关系

最优状态-价值函数 与 最优动作-价值函数 之间的管辖如下： 
![这里写图片描述](https://img-blog.csdn.net/20170210111657000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

换言之，关于价值函数存在一个递归的定义。 

编者（邱迪聪）注：说白了，他们的关系就是一个描述状态的最优价值，另外一个描述在这个状态下执行一个特定动作的最优价值。最优状态－价值函数用来描述处于一个状态的长期最优化价值，即考虑到这个状态下，可能发生的所有后续动作，并且都挑最好的动作来执行的情况下，这个状态的价值。最优动作－价值函数用来描述处于一个状态下并执行某个特定的动作后所带来的长期最优价值，即在这个状态下执行了一个特定的动作，然后考虑到执行这个动作后所有可能处于的后续状态并且在这些状态下总是选取最好的动作来执行所打来的长期价值。
# 最优策略Optimal Policy

关于收敛性： 

对策略定义一个偏序：  
$\pi \ge \pi' \mbox{if } v_{\pi}(s) \ge v_{\pi'}(s) \forall s $

定理：  

对于任意Markov决策过程：- 存在一个最优策略$\pi_*$ , 它比其他任何策略都要好，或者至少都一样好：$\pi_* \ge \pi ,\forall \pi$
- 所有最优决策都达到最优值函数， $v_{\pi*}(s)=v_*(s)$
- 所有最优决策都达得最优行动值函数， $q_{\pi*}(s,a)=q_*(s,a)$

## 从最优状态价值函数到最优策略

![这里写图片描述](https://img-blog.csdn.net/20170208174701039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 直接求解Bellman最优性方程

理论上直接求解Bellman最优性方程即可获得最优价值函数。但是我们需要考虑如下几方面： 
- **通过解Bellman最优性方程找一个最优策略需要以下条件**：  
- 动态模型已知 
- 我们有足够的计算资源和时间 
- 系统满足Markov特性 

- **我们需要多少空间和时间？**
- 求解方程的计算复杂度是多项式级（感觉还行是不是？） 
- 但是，状态的数量通常会很巨大。（随问题维度指数增加） 
- 所以迭代全空间来精确求解Bellman方程是不可行的。 

- **我们一般会采用近似的方法。**
- D.P.Bertsekas 和 J.N.Tsitsiklis已经借助能够近似Bellman函数的人工神经网络提出了近似动态规划（Dynamic Programming）。 
- 这是一种能够有效减轻维数影响的方法，它采用的是记忆单个神经网络参数而不是去记忆整个空间域完整的函数映射。


## 近似强化学习
- 很多RL方法就是在研究如何近似Bellman方程的解。 
- 在未知动态模型中权衡奖励积累和系统认知（模型学习 
- 强化学习是一种在线学习，所以比较容易在那些频繁出现的状态上学到较好的策略（比较接近最优策略），而对于那些较少出现的状态，强化学习往往不能学到很好的策略。这是和传统控制问题的一大区别。（编者注：控制问题里模型已知，对策略的要求往往是在所关心的全部状态空间内都要保证收敛性。）

# 小结
- Markov决策过程
- 价值函数和最优价值函数
- Bellman方程
- 目前为止我们只讨论了已知动态特性的有限Markov决策过程。











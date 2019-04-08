# 当我们在谈论 DRL：从Q-learning到DQN

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

16 人赞同了该文章

**本系列意在长期连载分享，内容上可能也会有所增删改减；**

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

## **引言**

DRL（Deep Reinforcement Learning）的首次惊艳亮相，应该是 DeepMind 在2013年首次将其应用于 Atari 游戏中提出的 DQN（Deep Q Network）算法。在今天（2017年），DRL 依然是最前沿的研究领域之一。但短短四年间，DRL 已经从玩 Atari，进化为下围棋（Alphago）、玩电竞（Dota AI、StarCraft AI），一次次刷新大家的三观。

从本篇开始，我们会介绍一些 DRL 的经典算法。由于 DRL 需要一定 RL 基础知识，本篇会先简单回顾。由于本文涉及的内容较多，没有基础的同学可能感觉比较乱，这里先给出一张 RL 算法分类/演进示意图（图出自 [强化学习（Reinforcement Learning）的方法分类](http://link.zhihu.com/?target=http%3A//blog.csdn.net/wordyang1/article/details/76557608)）。需要深入理解相关知识的同学可以按图索骥。

![img](https://pic4.zhimg.com/80/v2-ed4c4541a160b25c95e0eb76fc29bacb_hd.jpg)

## **RL 基础**

**MDP 与 Policy**

RL 本质即通过研究 Agent 与 Environment 交互（一般使用 Markov Decision Process，MDP 来描述），并寻找最优策略（Policy ）的过程。

对于 Markov Decision Process（MDP），这里我们用五元组 ![\{S,A,P,R,\gamma\}](https://www.zhihu.com/equation?tex=%5C%7BS%2CA%2CP%2CR%2C%5Cgamma%5C%7D) 表示，其中

- ![S](https://www.zhihu.com/equation?tex=S) （State）表示 Environment 状态空间
- ![A](https://www.zhihu.com/equation?tex=A) （Action）表示 Agent 的动作空间
- ![P](https://www.zhihu.com/equation?tex=P) 为状态转移概率矩阵，有 ![P^a_{ss'} = P[S_{t+1}=s'|S_t=s, A_t=a]](https://www.zhihu.com/equation?tex=P%5Ea_%7Bss%27%7D+%3D+P%5BS_%7Bt%2B1%7D%3Ds%27%7CS_t%3Ds%2C+A_t%3Da%5D)
- ![R](https://www.zhihu.com/equation?tex=R) （Reward）表示即时回报，有 ![R^a_s = E[R_{t+1}|S_t=s, A_t=a]](https://www.zhihu.com/equation?tex=R%5Ea_s+%3D+E%5BR_%7Bt%2B1%7D%7CS_t%3Ds%2C+A_t%3Da%5D)
- ![ \gamma](https://www.zhihu.com/equation?tex=+%5Cgamma) （discount factor）表示衰减系数

对于 Policy，我们用 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 来表示，其意义为给定 State 下 Action 的分布。于是有

![\pi(a|s) = P[A_t=a|S_t =s]](https://www.zhihu.com/equation?tex=%5Cpi%28a%7Cs%29+%3D+P%5BA_t%3Da%7CS_t+%3Ds%5D)



**Value Function**

为了描述 MDP 过程中某一个 State 或 Action 的长期 Reward，有 State-Value Function ![V_{\pi}(s)](https://www.zhihu.com/equation?tex=V_%7B%5Cpi%7D%28s%29) 与 Action-Value Function ![Q_{\pi}(s)](https://www.zhihu.com/equation?tex=Q_%7B%5Cpi%7D%28s%29) 。它们的定义如下

![\begin{align} & V_{\pi}(s) = E_{\pi}[\sum_{k=0}^{\infty} \gamma^k R_{t+k+1}|S_t=s] \\ & Q_{\pi}(s,a) = E_{\pi}[\sum_{k=0}^{\infty} \gamma^k R_{t+k+1}|S_t=s,A_t=a] \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26+V_%7B%5Cpi%7D%28s%29+%3D+E_%7B%5Cpi%7D%5B%5Csum_%7Bk%3D0%7D%5E%7B%5Cinfty%7D+%5Cgamma%5Ek+R_%7Bt%2Bk%2B1%7D%7CS_t%3Ds%5D+%5C%5C+%26+Q_%7B%5Cpi%7D%28s%2Ca%29+%3D+E_%7B%5Cpi%7D%5B%5Csum_%7Bk%3D0%7D%5E%7B%5Cinfty%7D+%5Cgamma%5Ek+R_%7Bt%2Bk%2B1%7D%7CS_t%3Ds%2CA_t%3Da%5D+%5C%5C+%5Cend%7Balign%7D)

又由于其 Markov 性，State-Value Function ![V_{\pi}(s)](https://www.zhihu.com/equation?tex=V_%7B%5Cpi%7D%28s%29) 有如下属性，称为 Bellman Equation

![\begin{align} V_{\pi}(s) & = E_{\pi}[\sum_{k=0}^{\infty} \gamma^k R_{t+k+1}|S_t=s] \\ & = E_{\pi}[R_{t+1} + \sum_{k=1}^{\infty} \gamma^k R_{t+k+1}|S_t=s] \\ & = \sum_{a \in A}\pi(a|s) \sum_{s' \in S}P^a_{ss'}(R^a_{ss'}+\gamma V_{\pi}(s')) \\ & = E_{\pi}[R_{t+1}+\gamma V_{\pi}(S_{t+1})|S_t=s] \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+V_%7B%5Cpi%7D%28s%29+%26+%3D+E_%7B%5Cpi%7D%5B%5Csum_%7Bk%3D0%7D%5E%7B%5Cinfty%7D+%5Cgamma%5Ek+R_%7Bt%2Bk%2B1%7D%7CS_t%3Ds%5D+%5C%5C+%26+%3D+E_%7B%5Cpi%7D%5BR_%7Bt%2B1%7D+%2B+%5Csum_%7Bk%3D1%7D%5E%7B%5Cinfty%7D+%5Cgamma%5Ek+R_%7Bt%2Bk%2B1%7D%7CS_t%3Ds%5D+%5C%5C+%26+%3D+%5Csum_%7Ba+%5Cin+A%7D%5Cpi%28a%7Cs%29+%5Csum_%7Bs%27+%5Cin+S%7DP%5Ea_%7Bss%27%7D%28R%5Ea_%7Bss%27%7D%2B%5Cgamma+V_%7B%5Cpi%7D%28s%27%29%29+%5C%5C+%26+%3D+E_%7B%5Cpi%7D%5BR_%7Bt%2B1%7D%2B%5Cgamma+V_%7B%5Cpi%7D%28S_%7Bt%2B1%7D%29%7CS_t%3Ds%5D+%5Cend%7Balign%7D)

同理，Action-Value Function ![Q_{\pi}(s)](https://www.zhihu.com/equation?tex=Q_%7B%5Cpi%7D%28s%29) 也有

![\begin{align} Q_{\pi}(s,a) & = E_{\pi}[\sum_{k=0}^{\infty} \gamma^k R_{t+k+1}|S_t=s,A_t=a] \\ & = E_{\pi}[R_{t+1} + \sum_{k=1}^{\infty} \gamma^k R_{t+k+1}|S_t=s, A_t=a] \\ & = \sum_{s' \in S}P^a_{ss'}(R^a_{ss'}+\gamma Q_{\pi}(s',a')) \\ & = E_{\pi}[R_{t+1}+\gamma Q_{\pi}(S_{t+1},A_{t+1})|S_t=s, A_t=a] \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+Q_%7B%5Cpi%7D%28s%2Ca%29+%26+%3D+E_%7B%5Cpi%7D%5B%5Csum_%7Bk%3D0%7D%5E%7B%5Cinfty%7D+%5Cgamma%5Ek+R_%7Bt%2Bk%2B1%7D%7CS_t%3Ds%2CA_t%3Da%5D+%5C%5C+%26+%3D+E_%7B%5Cpi%7D%5BR_%7Bt%2B1%7D+%2B+%5Csum_%7Bk%3D1%7D%5E%7B%5Cinfty%7D+%5Cgamma%5Ek+R_%7Bt%2Bk%2B1%7D%7CS_t%3Ds%2C+A_t%3Da%5D+%5C%5C+%26+%3D+%5Csum_%7Bs%27+%5Cin+S%7DP%5Ea_%7Bss%27%7D%28R%5Ea_%7Bss%27%7D%2B%5Cgamma+Q_%7B%5Cpi%7D%28s%27%2Ca%27%29%29+%5C%5C+%26+%3D+E_%7B%5Cpi%7D%5BR_%7Bt%2B1%7D%2B%5Cgamma+Q_%7B%5Cpi%7D%28S_%7Bt%2B1%7D%2CA_%7Bt%2B1%7D%29%7CS_t%3Ds%2C+A_t%3Da%5D+%5Cend%7Balign%7D)



**最优 Policy**

对于最优 Policy ![\pi^*](https://www.zhihu.com/equation?tex=%5Cpi%5E%2A) ，其 ![V^*(s),Q^*(s,a)](https://www.zhihu.com/equation?tex=V%5E%2A%28s%29%2CQ%5E%2A%28s%2Ca%29) 对任意 Policy ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 及任意 State ![s](https://www.zhihu.com/equation?tex=s) 满足

![\begin{align} & V^*(s) \ge V_{\pi}(s) \\ & Q^*(s,a) \ge Q_{\pi}(s,a) \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26+V%5E%2A%28s%29+%5Cge+V_%7B%5Cpi%7D%28s%29+%5C%5C+%26+Q%5E%2A%28s%2Ca%29+%5Cge+Q_%7B%5Cpi%7D%28s%2Ca%29+%5Cend%7Balign%7D)

于是有

![\begin{align} & Q^*(s,a) = E_{s'}[R^a_{ss'} + \gamma \max_{a'} Q^*(s',a')|s,a] \\ & V^*(s) = \max_a Q^*(s,a) \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26+Q%5E%2A%28s%2Ca%29+%3D+E_%7Bs%27%7D%5BR%5Ea_%7Bss%27%7D+%2B+%5Cgamma+%5Cmax_%7Ba%27%7D+Q%5E%2A%28s%27%2Ca%27%29%7Cs%2Ca%5D+%5C%5C+%26+V%5E%2A%28s%29+%3D+%5Cmax_a+Q%5E%2A%28s%2Ca%29+%5C%5C+%5Cend%7Balign%7D)

且同时也有

![\begin{align} \pi^*(a|s) & = arg \max_{a \in A} Q^*(s,a) \\ & = arg \max_{a \in A} \sum_{s' \in S}P^a_{ss'}[R^a_{ss'} + \gamma V^*(s')] \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cpi%5E%2A%28a%7Cs%29+%26+%3D+arg+%5Cmax_%7Ba+%5Cin+A%7D+Q%5E%2A%28s%2Ca%29+%5C%5C+%26+%3D+arg+%5Cmax_%7Ba+%5Cin+A%7D+%5Csum_%7Bs%27+%5Cin+S%7DP%5Ea_%7Bss%27%7D%5BR%5Ea_%7Bss%27%7D+%2B+%5Cgamma+V%5E%2A%28s%27%29%5D+%5C%5C+%5Cend%7Balign%7D)



## **Model-Based 与 DP**

类似 SL 中的“判别模型”与“生成模型”，RL 也可以分为“Model-Based”与“Model Free”。

Model-Based 表示先对 Agent 与 Environment 的交互进行建模，得到具体的 MDP 模型。即已知 ![\{S,A,P,R,\gamma\}](https://www.zhihu.com/equation?tex=%5C%7BS%2CA%2CP%2CR%2C%5Cgamma%5C%7D) ，然后求解最佳 Policy。此时最常规的思路就是 DP（Dynamic Programming），相信刷 LeetCode 的同学肯定不会陌生。具体到算法，则是策略迭代（Policy Iteration）与价值迭代（Value Iteration）。



**Policy Iteration**

Policy Iteration 的思想其实很朴素，主要包含两个步骤

1. Policy Evaluation：对当前 Policy 迭代计算 State-Value Function，用于评估现有 Policy 的效果
2. Policy Improvement：对每个 State，通过 Greedy 的方法得到最优 Action，也就得到了新的且更优的 Policy

迭代上述步骤，直到 Policy 稳定，即 Policy Iteration 算法。其详细步骤如下图（图中标示符与本文所用的略有不同，但我查阅了若干本书并没发现更合适的，且放在这里辅助理解。）

![img](https://pic3.zhimg.com/80/v2-e50549bf0d59435bb5a08dade9f6b63a_hd.jpg)



**Value Iteration**

Value Iteration 可以看作是 Policy Iteration 的特殊版本，即

1. 在 Policy Evaluation 阶段并不迭代到 State-Value Function 收敛，而是只进行一次评估
2. 抛弃显式的 Policy Improvement，通过 Greedy 的方法直接改进 State-Value Function，期间并不输出 Policy

迭代直到 State-Value Function 收敛，最后输出最优 Policy。其流程如下

![img](https://pic4.zhimg.com/80/v2-7121325f643912dab17cda687fd3b0ef_hd.jpg)



## **Model-Free**

当 MDP 的模型参数未知时，我们只能通过 Agent 与 Environment 进行交互获得经验样本，即已知 ![\{S, A\}](https://www.zhihu.com/equation?tex=%5C%7BS%2C+A%5C%7D) ，然后根据这些数据求解最佳 Policy。这种通过经验样本学习 Policy 被称为 Model-Free 的学习方法。

常见的 Model-Free 方法主要有两种：MC（Monte-Carlo）与 TD（Temporal Difference）。而它们又分别有 On Policy 和 Off Policy 两种，在 On Policy 中产生样本的 Behavior Policy 和进行评估的 Target Policy 是同一个；在 Off Policy 中两者是独立的。



**On Policy MC**

在 Model 未知的情况下，MC 方法本质是利用抽样估计 Value Function，从而改进 Policy。

对 MC，其 Value Function ![V(s)](https://www.zhihu.com/equation?tex=V%28s%29) 的计算方式就是利用采样计算期望，当通过增量的方式来更新时，公式如下

![V(S_t) = V(S_t) + \alpha (G_t - V(S_t))](https://www.zhihu.com/equation?tex=V%28S_t%29+%3D+V%28S_t%29+%2B+%5Calpha+%28G_t+-+V%28S_t%29%29)

其中， ![G_t = \sum_{k=0}^{\infty} \gamma^k R_{t+k+1}](https://www.zhihu.com/equation?tex=G_t+%3D+%5Csum_%7Bk%3D0%7D%5E%7B%5Cinfty%7D+%5Cgamma%5Ek+R_%7Bt%2Bk%2B1%7D) 。

但是由于 Model 的 ![P^a_{ss'}](https://www.zhihu.com/equation?tex=P%5Ea_%7Bss%27%7D) 未知，无法通过 ![V(s)](https://www.zhihu.com/equation?tex=V%28s%29) 来求解最优 Policy。此时可以通过计算 ![Q(s,a)](https://www.zhihu.com/equation?tex=Q%28s%2Ca%29) 规避，其具体包含如下步骤：

1. 获取 Episodes：利用某 Policy ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 得到多个 Episodes，一个 Episodes 表示从初始 State 到终止 State 的 State、Action、Reword 序列，如 ![\{S_0,A_0,R_1, S_1,A_1,...S_k\}](https://www.zhihu.com/equation?tex=%5C%7BS_0%2CA_0%2CR_1%2C+S_1%2CA_1%2C...S_k%5C%7D) 。不过这要求任务必须存在终止 State。如果每个 Episodes 初始状态 ![S_0](https://www.zhihu.com/equation?tex=S_0) 是随机选择的，则称为 MC-ES（Monte-Carlo with Exploring Starts）。
2. Evaluation：上面已经说过 ![Q_{\pi}(s,a) = E_{\pi}[\sum_{k=0}^{\infty} \gamma^k R_{t+k+1}|S_t=s,A_t=a]](https://www.zhihu.com/equation?tex=Q_%7B%5Cpi%7D%28s%2Ca%29+%3D+E_%7B%5Cpi%7D%5B%5Csum_%7Bk%3D0%7D%5E%7B%5Cinfty%7D+%5Cgamma%5Ek+R_%7Bt%2Bk%2B1%7D%7CS_t%3Ds%2CA_t%3Da%5D) ，这里则利用采样的方式估计 ![Q_{\pi}(s,a)](https://www.zhihu.com/equation?tex=Q_%7B%5Cpi%7D%28s%2Ca%29) 。由于某个 State ![s](https://www.zhihu.com/equation?tex=s) 在一个 Episodes 中可能出现不止一次，对于一个Episodes，若只计算 ![s](https://www.zhihu.com/equation?tex=s) 第一次出现的 ![Q(s,a)](https://www.zhihu.com/equation?tex=Q%28s%2Ca%29) ，被称为 FVMC（First Visit Monte-Carlo）；若计算每次 ![s](https://www.zhihu.com/equation?tex=s) 出现的 ![Q(s,a)](https://www.zhihu.com/equation?tex=Q%28s%2Ca%29) ，被称为EVMC（Every Visit Monte-Carlo）。
3. Policy Improvement：类似于 Value Iteration，对每一个 episode 都可以改进 Policy ![\pi(a|s) = arg \max_{a \in A} Q(s,a)](https://www.zhihu.com/equation?tex=%5Cpi%28a%7Cs%29+%3D+arg+%5Cmax_%7Ba+%5Cin+A%7D+Q%28s%2Ca%29) 。并且，为了兼顾 Exploitation 和 Exploration，一般会采取 ![\epsilon-Greedy](https://www.zhihu.com/equation?tex=%5Cepsilon-Greedy) 方法，即以 ![1- \epsilon](https://www.zhihu.com/equation?tex=1-+%5Cepsilon) 概率采取上述贪婪算法改进 Policy，以 ![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon) 概率随机挑选 Action。

下面以 On Policy FVMC 为例，给出具体的流程

![img](https://pic3.zhimg.com/80/v2-5d0534f904d9f92489a7bc3333c6dfba_hd.jpg)



**Off Policy MC**

鉴于 Off Policy MC 在实际中使用较少，非重要课题，这里就略过了。



**On Policy TD（Sarsa）**

TD 算法可以避免每次必须得到整个 Episode 才能改进 Policy。TD 算法的 Value Function ![V(s)](https://www.zhihu.com/equation?tex=V%28s%29) 通过如下方式计算，可以看出其兼具 MC 与 TD 的特点，即 Sample 和 Bootstrap。

![\begin{align} V(S_t) & = V(S_t) + \alpha (G_t - V(S_t)) \\ & = V(S_t) + \alpha (R_{t+1} + \gamma V(S_{t+1}) - V(S_t)) \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+V%28S_t%29+%26+%3D+V%28S_t%29+%2B+%5Calpha+%28G_t+-+V%28S_t%29%29+%5C%5C+%26+%3D+V%28S_t%29+%2B+%5Calpha+%28R_%7Bt%2B1%7D+%2B+%5Cgamma+V%28S_%7Bt%2B1%7D%29+-+V%28S_t%29%29+%5Cend%7Balign%7D)

以上算法也被称为TD(0)，它是 ![TD(\lambda)](https://www.zhihu.com/equation?tex=TD%28%5Clambda%29) 算法的特例，有兴趣的同学可以深入了解一下，此处不再扩展。

Sarsa 算法具体步骤如下：

1. 通过两次 ![\epsilon-Greedy](https://www.zhihu.com/equation?tex=%5Cepsilon-Greedy) 方法，得到样本 ![(S,A,S',A')](https://www.zhihu.com/equation?tex=%28S%2CA%2CS%27%2CA%27%29)
2. 利用此样本更新 ![Q](https://www.zhihu.com/equation?tex=Q) ，此时 Behavior Policy 和 Target Policy 一致的（如 ![\epsilon-Greedy](https://www.zhihu.com/equation?tex=%5Cepsilon-Greedy)![\epsilon-Greedy](https://www.zhihu.com/equation?tex=%5Cepsilon-Greedy) ），因此是 On Policy 的。

流程如下

![img](https://pic4.zhimg.com/80/v2-68498b14b1daf8285d3d923f9ff4a6b7_hd.jpg)

后续还有学者将 Sarsa 与 ![TD(\lambda)](https://www.zhihu.com/equation?tex=TD%28%5Clambda%29) 结合提出 ![Sarsa(\lambda)](https://www.zhihu.com/equation?tex=Sarsa%28%5Clambda%29) 算法。



**Off Policy TD（Q-learning）**

Off Policy TD ，同时也是著名的 Q-learning 算法，它最初是由 Watkins 在其博士论文 "PhD Thesis: Learning from Delayed Rewards" 中提出的，是传统 RL 中一种非常有效的学习算法，本质是通过 One Step 的方式来缓慢改进 ![Q](https://www.zhihu.com/equation?tex=Q) ，即

![\begin{align} Q(S,A) & = (1 - \alpha)Q(S,A) + \alpha Q(S,A) \\ & = (1 - \alpha)Q(S,A) + \alpha (R + \gamma \max_a Q(S^{'},a)) \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+Q%28S%2CA%29+%26+%3D+%281+-+%5Calpha%29Q%28S%2CA%29+%2B+%5Calpha+Q%28S%2CA%29+%5C%5C+%26+%3D+%281+-+%5Calpha%29Q%28S%2CA%29+%2B+%5Calpha+%28R+%2B+%5Cgamma+%5Cmax_a+Q%28S%5E%7B%27%7D%2Ca%29%29+%5Cend%7Balign%7D)

其具体步骤如下：

1. 通过一次 ![\epsilon-Greedy](https://www.zhihu.com/equation?tex=%5Cepsilon-Greedy) 方法，得到样本 ![(S,A,S')](https://www.zhihu.com/equation?tex=%28S%2CA%2CS%27%29)
2. 利用此样本更新 ![Q](https://www.zhihu.com/equation?tex=Q) ，此时 Behavior Policy （如 ![\epsilon-Greedy](https://www.zhihu.com/equation?tex=%5Cepsilon-Greedy) ）、Target Policy （直接进行贪婪选择） 不一致，因此是 Off Policy 的。

流程如下

![img](https://pic4.zhimg.com/80/v2-63b81bec40cba0862e2f98e4b58e2447_hd.jpg)



**MC 与 TD 比较**

1. MC 需要一条完整的 Episode，而 TD 不需要；这也意味着 MC 只能处理有终止 State 的任务，而 TD 无此限制
2. 由于 MC 是根据完整的 Episode 来估计 ![V(s)](https://www.zhihu.com/equation?tex=V%28s%29) 的，因此它是 unbiased 的，但是会由于结果会受整个 Episode 的影响，带来较高的 Variance，即较大波动；相对的，TD 是根据 Episode 的中相邻状态来估计 ![V(s)](https://www.zhihu.com/equation?tex=V%28s%29) ，结果只受少数状态影响，因此具有较低的 Variance，但必然是 biased 的
3. MC 并不严格要求任务的 Markov 性，且具有很好的收敛性，但是计算量大，更新缓慢； TD 对任务的 Markov 性要求比较高，但是效率很高，一般也会收敛



## **DQN**

在 DL 热潮卷席 SL 领域的同时，自然有学者试图将其应用在 RL 中。其中的先驱们，最成功的要数 DeepMind 于 2013 年发表的“Mnih V, Kavukcuoglu K, Silver D, et al. Playing Atari with Deep Reinforcement Learning[J]. Computer Science, 2013.”。论文提出了 DQN 算法并用来玩 Atari 游戏（一个电子小游戏集合），在 29 个游戏中打破了人类玩家的 记录，其惊人的效果直接激活了 DRL 这个崭新的领域。

随后在 2015 年，DeepMind 携着改良的 DQN 再次在 Nature 上发文“Mnih V, Kavukcuoglu K, Silver D, et al. Human-level control through deep reinforcement learning.[J]. Nature, 2015”。接下来，我们会详细介绍一下 Nature DQN。

上面我们提到过，Q-learning 通过贪婪的方式改进 ![Q(s,a)](https://www.zhihu.com/equation?tex=Q%28s%2Ca%29) ，得到最优 Policy。但是 Q-learning 仅适用于求解小规模，离散空间问题。当 State 空间或 Action 空间规模很大，或者为连续空间时，无法再通过采样遍历足够多的 State时，Q-learning 可能就不再有效。此时，可以通过 Function Approximator 的方式解决，即利用函数 ![Q(s,a;\theta)](https://www.zhihu.com/equation?tex=Q%28s%2Ca%3B%5Ctheta%29) 逼近最优的 ![Q^*(s,a)](https://www.zhihu.com/equation?tex=Q%5E%2A%28s%2Ca%29) ，于是我们只需要求解 ![Q(s,a;\theta)](https://www.zhihu.com/equation?tex=Q%28s%2Ca%3B%5Ctheta%29) 即可。若更进一步，还可以利用 DNN 来学习 ![Q(s,a;\theta)](https://www.zhihu.com/equation?tex=Q%28s%2Ca%3B%5Ctheta%29) ，于是就有了 DQN 的基本思想。

这里补充一句，利用 Function Approximator 的思想来解决大规模连续空间 RL 问题并非 DQN 首创，上述的 Policy Iteration、Value Iteration 等都早有其对应的 Function Approximator 算法。但这里我们只关注 DRL 相关算法，就不再展开。

如上文所述，有最优 ![Q](https://www.zhihu.com/equation?tex=Q) 的迭代式为

![Q^*(s,a) = E_{s'}[R^a_{ss'} + \gamma \max_{a'} Q^*(s',a')|s,a]](https://www.zhihu.com/equation?tex=Q%5E%2A%28s%2Ca%29+%3D+E_%7Bs%27%7D%5BR%5Ea_%7Bss%27%7D+%2B+%5Cgamma+%5Cmax_%7Ba%27%7D+Q%5E%2A%28s%27%2Ca%27%29%7Cs%2Ca%5D)

当我们用函数来逼近它，即

![Q^* = E_{s'}[r + \gamma \max_{a'} Q(s',a';\theta^-_i)]](https://www.zhihu.com/equation?tex=Q%5E%2A+%3D+E_%7Bs%27%7D%5Br+%2B+%5Cgamma+%5Cmax_%7Ba%27%7D+Q%28s%27%2Ca%27%3B%5Ctheta%5E-_i%29%5D)

于是，整个迭代过程的 Loss 可以被定义为

![\begin{align} L_i(\theta_i) & = E_{s,a,r}[(E_{s'}[Q^*|s,a] - Q(s,a;\theta_i))^2] \\ & = E_{s,a,r,s^{'}}[(Q^* - Q(s,a;\theta_i))^2] + E_{s,a,r}[var_{s'}[Q^*]] \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+L_i%28%5Ctheta_i%29+%26+%3D+E_%7Bs%2Ca%2Cr%7D%5B%28E_%7Bs%27%7D%5BQ%5E%2A%7Cs%2Ca%5D+-+Q%28s%2Ca%3B%5Ctheta_i%29%29%5E2%5D+%5C%5C+%26+%3D+E_%7Bs%2Ca%2Cr%2Cs%5E%7B%27%7D%7D%5B%28Q%5E%2A+-+Q%28s%2Ca%3B%5Ctheta_i%29%29%5E2%5D+%2B+E_%7Bs%2Ca%2Cr%7D%5Bvar_%7Bs%27%7D%5BQ%5E%2A%5D%5D+%5C%5C+%5Cend%7Balign%7D)

有了 Loss 的定义，RL 问题就可以被转化为 SL 的方式来求解。其中 ![Q^*](https://www.zhihu.com/equation?tex=Q%5E%2A) 为 targets，此处可以理解为 SL 中即 Ground Truth。SL 中 Groud Truth 是固定的，但这里 ![Q^*](https://www.zhihu.com/equation?tex=Q%5E%2A) 在迭代过程中是改变的。只是在每一轮 BP 时，其值会被固定，不参与梯度的计算。按照作者的说法，将 targets 与 DQN 的网络分离，能够大大提高 DQN 的稳定性。而第二项为方差，与 ![\theta_i](https://www.zhihu.com/equation?tex=%5Ctheta_i) 无关，可以忽略。

除了 Loss，训练 DQN 还需要训练样本。对于 Atari 游戏过程这个时间序列事件，一个 Episode 其相邻状态相关性是非常强的，如果直接用整个 Episode 来训练，样本的强关联性与 Episode 采样数量不足会使结果方差很大。在 DQN 中，采用了 Experience Replay 的方法，即先将所有样本存储起来，然后随机采样，通过破坏样本序列强相关，同时间接增加了训练样本数量的方式，降低了方差。

最后，DQN 的具体训练步骤如下

![img](https://pic1.zhimg.com/80/v2-27aa73b422e8087cc933dbc7680e61a8_hd.jpg)

## **小结**

通过上面的回顾，可以看出 RL 的本质是非常朴素的，但过于简单的模型也限制了其在现实复杂场景下的应用。与 DL 结合的 DRL 很好的弥补了 RL 的短板，大大提高了其描述能力，最有力的例子就是 DQN 大幅提升的效果。 当然，DQN 也只是 DRL 较为早期的算法了，下一篇我们会介绍应用更为普遍的 A3C 算法。



## **尾巴**

应该有同学发现我专栏的目录 [专栏总目录 - 知乎专栏](https://zhuanlan.zhihu.com/p/25091816?refer=data-miner) 很久没更新了，原因是知乎的 bug 导致此页面无法修改，最近几篇文章都没放上去。

本来知乎 bug 多我都习以为常了，不影响使用也罢了，但这次已经影响文章修改这种基础功能，于是我主动反馈了情况。可经过**四轮跨度三个月**的反馈，**知乎人员从不作为到冷处理，问题毫无进展**，令人极其失望，于是我提了个问题 [为什么知乎对bug反馈如此不作为？](https://www.zhihu.com/question/67571920)

在这里希望同学们能关注传播一下这个问题，当然经过这几次反馈我觉得知乎会 fix 的概率极其小，但是我还是希望更多人看看知乎现在这嘴脸：

**少年还没有杀死恶龙，就已然开始腐烂。**



## **Reference**：

《Reinforcement Learning: An Introduction》Richard S. Sutton and Andrew G. Barto



## **本系列其他文章：**

[专栏总目录 - 知乎专栏](https://zhuanlan.zhihu.com/p/25091816?refer=data-miner)
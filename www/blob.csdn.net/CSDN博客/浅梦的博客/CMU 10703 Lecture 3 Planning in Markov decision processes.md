# CMU 10703 |Lecture 3 Planning in  Markov decision processes - 浅梦的博客 - CSDN博客





2017年02月08日 20:15:36[浅梦s](https://me.csdn.net/u012151283)阅读数：614








本文转载自微信公众号 [机器人学家](http://mp.weixin.qq.com/s?__biz=MzA3NDU2NDk3MQ==&mid=2651928504&idx=1&sn=8194022d04402f4ca5a5b8395f1c0525&chksm=849843bcb3efcaaad9173b99718d879b76b49c64a6159a0ddb1116074301ca3023a883351276&scene=0#rd)，根据其上发布的CMU翻译讲义进行整理。

# 大纲
- Exact solution methods:  
- Policy iteration  
- Policy Evaluation (a.k.a. DP) 
- Policy Improvement 

- Value iteration (a.k.a. DP) 
- Linear programming 
- Approximate solution methods:  
- Asynchronous DP


# 策略评估
- 策略评估：对于给定的策略$\pi$,计算状态价值函数$v_{\pi}(s)$.
- 状态价值函数$v_{\pi}(s)$的定义：$v_{\pi}(s)=\mathbb{E_\pi}[G_t|S_t=s]$
- 状态价值函数$v_{\pi}(s)$的Bellman方程如下，是一个|S|元方程组，|S|为状态空间S的数量。 
$v_{\pi}(s)=\sum_{a\in A}\pi(a|s)(R^a_s+\gamma \sum_{s'\in S}P^a_{ss'}v_{\pi}(s'))$

# 从MDPs到MRPs

当我们固定一个策略时，MDP问题就变成了马尔科夫奖励过程(Markov Reward Process，下简写为MRP) 

由前面提到的状态价值函数的Bellman方程，可以得到： 
$v_{\pi}(s)=\sum_{a\in A}\pi(a|s)R^a_s+\gamma \sum_{a\in A}\pi(a|s)\sum_{s'\in S}P^a_{ss'}v_{\pi}(s'))\\=R^{\pi}_s+\gamma \sum_{s'\in S}P^{\pi}_sv_{\pi}(s')$
![这里写图片描述](https://img-blog.csdn.net/20170209120608325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170209120644028?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 矩阵形式

使用MRP，可以得到更加简洁的Bellman期望公式。如果有N个状态s，那么在策略评估的过程中，需要求解N个未知量（v(s)），有N个线性方程（每个状态有一个Bellman equation），这样可以得到一个N维线性系统，理论上可以直接求解。对这个N维线性系统可以得到如下的矩阵形式的表达。  
$v_{\pi}=R^{\pi}+\gamma P^{\pi}v_{\pi}$

其中v为向量表示的状态价值函数，向量中的每一个元素表示对应状态的价值。R为向量表示的即时收益的期望， 
$R^{\pi}(s)=\sum_{a\in A}\pi(s|a)R^a_s$
$P^{\pi}=\sum_{a\in A}\pi(a|s)P(s'|s,a)$

得到解析解如下： 
$v_{\pi}=(I-\gamma P^{\pi})^{-1}R^{\pi}$

计算复杂度为$O(N^3)$
# 迭代法

![这里写图片描述](https://img-blog.csdn.net/20170209121318301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 迭代策略评估算法

![这里写图片描述](https://img-blog.csdn.net/20170209121505259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 策略评估算法 收敛性证明

一个有|S|个状态的MDP，其所有价值函数V(s)构成的向量可以看成|S|维向量空间V中的一个点。要说明收敛性，就要说明策略评估里的计算——贝尔曼 backup 会对该空间中的点带来怎样变化？  

下面我们说明，它每次迭代都会使这些点离得更近  

所以贝尔曼backup在无穷次迭代后必然收敛到唯一解 
## 价值函数的无穷范数$\infty -Norm$

我们可以用$\infty -Norm$来度量状态-价值函数u和v  

即，两个状态价值之间的距离。 
$||u-v||_{\infty}=max_{s\in S}|u(s)-v(s)|$
## 贝尔曼back up是收缩的

定义贝尔曼Back up操作子$T^{\pi}$
$T^{\pi}(v)=R^{\pi}+\gamma P^{\pi}v$

这个操作子是$\gamma$-收缩，即它会**使价值函数之间的距离至少缩小到原来的$\gamma$倍**。$\gamma$是定义价值时的折扣系数，小于1： 
$||T^{\pi}(u)-T^{\pi}(v)||_{\infty}$=$||(R^{\pi}+\gamma P^{\pi}u)-(R^{\pi}+\gamma P^{\pi}v)||_{\infty}\\=||\gamma P^{\pi}(u-v)||_{\infty}\\\le||\gamma P^{\pi}||u-v||_{\infty}||_{\infty}\\\le \gamma||u-v||_{\infty} $
## 收缩映射定理

定理（收缩映射定理）  

已知T是$\gamma$收缩，对于操作T(v)下的任意完备（即闭合）的度量空间V，有：  

T以线性收敛速率$\gamma$收敛到唯一的固定点  

（编者注：即**策略评估一定会收敛到一个值**）
## 结论：迭代策略评估是收敛的

贝尔曼期望操作子$T^{\pi}$有唯一的固定点。（根据贝尔曼期望方程）$v_{\pi}$是$T^{\pi}$上的一个固定点，所以根据收缩映射定理，迭代策略评估收敛于$v_{\pi}$

# 策略迭代

![这里写图片描述](https://img-blog.csdn.net/20170209123404154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 广义策略 迭代(GPI)

![这里写图片描述](https://img-blog.csdn.net/20170209123546936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 最优性原理

称策略$\pi(a|s)$在状态s处获得最优值，即$v_{\pi}(s)=v_*(s)$，当且仅当：
- 对于从状态s可以到达任意的状态s’
- 策略$\pi$也在状态s’处获得最优值，即$v_{\pi}(s')=v_*(s')$

# 值迭代

$V_{k+1}(s) = max_a{\sum_{s'}T(s,a,s')[R(s,a,s')+\gamma V_k(s')]}$

重复直到收敛 

一次迭代的复杂度$O(S^2A)$

定理：会收敛到最优价值 

策略可能先于状态价值很长时间收敛 

不是就地更新 

问题- 速度慢一次迭代时间复杂读$O(S^2A)$
- 每个状态的max值很少变化
- 策略往往先于状态价值很长时间收敛

# 策略迭代
- 仍然是最优的
- 在某些情况下收敛更快

# 对比
- 值迭代 
- 每次迭代都更新了状态价值和（隐含地）策略
- 不追踪策略，但是在对$q(s,a)$取最大值的时候隐含地重新计算了它

- 策略迭代 
- 在固定此策略下多次更新utlities（每一次都很快，因为只考虑了一个action，而不是所有的）
- 策略被评估完后，选择一个新的策略（跟一次价值迭代一样慢）
- 新的策略会更好

- 都是解决MDPs的动态规划方法

# 总结
- 如果想： 
- 计算最优价值：使用值迭代或策略迭代
- 计算特定策略价值：使用策略评估
- 将状态价值转化成一个策略：使用策略提取(one-step lookahead)

- 这些看起来一样 
- 本质上都是Bellman uodates的变种
- 都利用one-step looahead expectimax fragments
- They differ only in whether we plug in a fixed policy or max over actions


# 异步动态规划

未完待续 
![这里写图片描述](https://img-blog.csdn.net/20170210114433154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)












# 【强化学习】AlphaGo Zero详解 - zkq_1986的博客 - CSDN博客





2017年10月30日 10:57:40[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：2738








# **1 简介**

AlphaGo Zero（以下简称Zero）的过程如下图a、b所示，在每个状态*s*，通过MCTS搜索，获得每个可能move的概率*p*，其中MCTS搜索采用self-play并执行*fθ*策略。*fθ*主要采用微软的ResNet，即基于残差的学习。利用MCTS获得每个可能move的概率*p*之后，更新*fθ*权重。最后利用这个*fθ*评估最后能赢这盘棋的概率*v*。

![](https://img-blog.csdn.net/20171030110514588)

# **2 MCTS**

每个节点s（状态），包含若干个边，这些边代表在状态s所能执行的动作a∈A(s)。每个边edge存储以下信息

{N(s, a),W(s, a),Q(s, a), P(s, a)}

其中N(s, a) 为访问次数，W(s, a)为执行动作a获得的总的Q值，Q(s, a)为执行动作a获得的平均Q值，P(s,
 a)是通过*fθ*得到的先验概率。



（1）select

对于叶节点sL，从根节点到当前节点，其策略为：![](https://img-blog.csdn.net/20171030110519178)。

其中

![](https://img-blog.csdn.net/20171030110523337)

cpuct为常量，控制着探索的范围。

（2）Expand and evaluate

针对要被扩展的叶子节点，将父节点的s左右交换或者旋转一下后，初始化出以下值：

{N(sL, a) = 0, W(sL, a) = 0, Q(sL, a) = 0, P(sL, a) = pa};然后开始模拟，计算出该叶子节点的Q值，*W*(*sL*,
*a*)=*W*(*sL*, *a*)+*v*, *Q*(*sL*,
*a*) =*W*(*sL*, *a*)/N(sL, a)。

（3）剪枝

如果该节点以及其最好的子节点的Q值小于一定阈值，那么就会将其剪枝，不再被探索。

# **3 Neural network architecture**

输入特征：历史特征+自己当前特征+对手当前特征

神经网络采用微软的ResNet结构。利用两层的ResNet残差学习模块。

![](https://img-blog.csdn.net/20171030110529505)

Zero采用19或39层ResNet



![](https://img-blog.csdn.net/20171030110533476)

ResNet的两层残差模块



ResNet的两层残差模块中的“weight layer”，在Zero上对应为卷积模块。卷积模块具体为：

(1) A convolution of 256 filters of kernel size 3×3 with stride 1

(2) Batch normalization

(3) A rectifier nonlinearity


















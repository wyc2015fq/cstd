# 【强化学习】actor-critic算法 - zkq_1986的博客 - CSDN博客





2018年05月15日 19:59:28[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：647








actor：行动者，对应policy

critic：评论者，对应value function值函数

(1)actor-only：将policy参数化，可以在算法过程中直接优化，因此action可以是连续的。优化方法通常为policy gradient方法，该方法的缺点为在估计梯度的时候将会产生较大的方差，导致学习速度较慢。 
(2)critic-only：使用temporal difference（时间差分法）学习方法，估计过程中方差小。通常greedy （贪心算法）或者ε-greedy（ε贪心算法）。ε-greedy可以有效的平衡exploration-exploitation的关系，既能探索新的action又能利用原有的经验生成最优的action，这里不详细介绍greedy，通过greedy算法搜索最优action的计算量非常大，尤其是action是连续的情况下。因此，critic-only通常连续的action进行离散化，将优化问题变为一个枚举问题。 


(3)actor-critic：整合了上述两个方法的优点。低方差，连续action。critic对当前的state以及action的表现进行估计，得到value function，用来给actor更新梯度。低方差的代价是在学习开始时，由于critic的估计不够准确而使算法具有较大偏差。policy-gradient占了该算法的绝大部分，其中可以分为两种standard gradient以及natural gradient，另一部分为更新actor。 

![](https://img-blog.csdn.net/20180515195822205)


![](https://img-blog.csdn.net/20180516210506886)


参考文献：

1. https://blog.csdn.net/weixin_37895339/article/details/74612572




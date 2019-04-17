# 强化学习（二）：Sarsa - qq_38593211的博客 - CSDN博客





2018年08月04日 18:08:13[小屁猴多看书](https://me.csdn.net/qq_38593211)阅读数：151








# Sarsa(state-action-reward-state_-action_)

## 1.与Q_Learning的区别

    Sarsa是on-policy，即在线学习型，下一个 state_和action_ 将会变成他真正采取的 action 和 state；Q_Learning是off-policy，即离线学习型，state_和 action_ 在算法更新的时候都还是不确定的。这种不同之处使得 Sarsa 相对于 Qlearning, 更加的深入. 因为 Qlearning保证了一个stage下一次的value最大化, 对于Sarsa 是一种保守的算法, 他在乎每一步决策, 以及下一步需要的stage和action。两种算法都有他们的好处, 比如在实际中, 你比较在乎机器的损害, 用一种保守的算法, 在训练时就能减少损坏的次数。

## 2.算法流程

![屏幕快照 2016-01-06 下午9.52.57.png-61kB](http://static.zybuluo.com/Kintoki/yt605ny9pl3vq47wu4nbkjds/%E5%B1%8F%E5%B9%95%E5%BF%AB%E7%85%A7%202016-01-06%20%E4%B8%8B%E5%8D%889.52.57.png)

    解释一下，和Q_Learning一样的流程，选择stage_和action_同样用了贪婪的方法，唯一的区别，来看看这个更新Q表的公式：                                 ![Q(S,A)-Q(S,A)\leftarrow\alpha( R+\gamma* argmaxQ_{table}.iloc[S-, :].max()-Q(S,A))](https://private.codecogs.com/gif.latex?Q%28S%2CA%29-Q%28S%2CA%29%5Cleftarrow%5Calpha%28%20R&plus;%5Cgamma*%20argmaxQ_%7Btable%7D.iloc%5BS-%2C%20%3A%5D.max%28%29-Q%28S%2CA%29%29)

   发现什么区别了？在Q_Learning的公式中更新前一个stage的时候取了下一个stage即stage_的得分最高的action，而Sarsa却没有，你想想，在Q_Learning中等他到达下一个stage时真的会选择那个得分最高的action吗？不一定吧，还得根据下下个得分吧。

## 3.Sarsa-Lamda

    单步更新：每一步都在更新；回合更新： 在没有到达终点时候, 不进行更新, 也就是直到到达终点时, 我们才为上一步更新为: 回合更新虽然我要等到这回合结束, 才开始对本回合所经历的所有步都添加更新, 但是这所有的步都是和终点有关系的, 都是为了达到终点需要学习的步, 所以每一个stage在下回合被选择的几率又高了一些.。

    Lamda就是一个衰变值，当 lambda 取0, 就变成了 Sarsa 的单步更新, 当 lambda 取 1, 就变成了回合更新, 对所有步更新的力度都是一样， 当 lambda 在 0 和 1 之间, 取值越大, 离终点越近的步更新力度越大.。




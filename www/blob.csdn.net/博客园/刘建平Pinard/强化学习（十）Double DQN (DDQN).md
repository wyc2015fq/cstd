
# 强化学习（十）Double DQN (DDQN) - 刘建平Pinard - 博客园






# [强化学习（十）Double DQN (DDQN)](https://www.cnblogs.com/pinard/p/9778063.html)
在[强化学习（九）Deep Q-Learning进阶之Nature DQN](https://www.cnblogs.com/pinard/p/9756075.html)中，我们讨论了Nature DQN的算法流程，它通过使用两个相同的神经网络，以解决数据样本和网络训练之前的相关性。但是还是有其他值得优化的点，文本就关注于Nature DQN的一个改进版本: Double DQN算法（以下简称DDQN）。
本章内容主要参考了ICML 2016的[deep RL tutorial](https://icml.cc/2016/tutorials/deep_rl_tutorial.pdf)和DDQN的论文<Deep Reinforcement Learning with Double Q-learning>。
# 1. DQN的目标Q值计算问题
在DDQN之前，基本上所有的目标Q值都是通过贪婪法直接得到的，无论是Q-Learning， DQN(NIPS 2013)还是 Nature DQN，都是如此。比如对于Nature DQN,虽然用了两个Q网络并使用目标Q网络计算Q值，其第j个样本的目标Q值的计算还是贪婪法得到的，计算入下式：
$$
y_j= \begin{cases} R_j& {is\_end_j\; is \;true}\\ R_j + \gamma\max_{a'}Q'(\phi(S'_j),A'_j,w') & {is\_end_j \;is\; false} \end{cases}
$$
使用max虽然可以快速让Q值向可能的优化目标靠拢，但是很容易过犹不及，导致过度估计(Over Estimation)，所谓过度估计就是最终我们得到的算法模型有很大的偏差(bias)。为了解决这个问题， DDQN通过解耦目标Q值动作的选择和目标Q值的计算这两步，来达到消除过度估计的问题。
# 2. DDQN的算法建模
DDQN和Nature DQN一样，也有一样的两个Q网络结构。在Nature DQN的基础上，通过解耦目标Q值动作的选择和目标Q值的计算这两步，来消除过度估计的问题。
在上一节里，Nature DQN对于非终止状态，其目标Q值的计算式子是：
$$
y_j= R_j + \gamma\max_{a'}Q'(\phi(S'_j),A'_j,w')
$$
在DDQN这里，不再是直接在目标Q网络里面找各个动作中最大Q值，而是先在当前Q网络中先找出最大Q值对应的动作，即
$$
a^{max}(S'_j, w) = \arg\max_{a'}Q(\phi(S'_j),a,w)
$$
然后利用这个选择出来的动作$a^{max}(S'_j, w) $在目标网络里面去计算目标Q值。即：
$$
y_j = R_j + \gamma Q'(\phi(S'_j),a^{max}(S'_j, w),w')
$$
综合起来写就是：
$$
y_j = R_j + \gamma Q'(\phi(S'_j),\arg\max_{a'}Q(\phi(S'_j),a,w),w')
$$
除了目标Q值的计算方式以外，DDQN算法和Nature DQN的算法流程完全相同。
# 3. DDQN算法流程
这里我们总结下DDQN的算法流程，和Nature DQN的区别仅仅在步骤2.f中目标Q值的计算。
算法输入：迭代轮数$T$，状态特征维度$n$, 动作集$A$, 步长$\alpha$，衰减因子$\gamma$, 探索率$\epsilon$, 当前Q网络$Q$，目标Q网络$Q'$, 批量梯度下降的样本数$m$,目标Q网络参数更新频率$C$。
输出：Q网络参数
1. 随机初始化所有的状态和动作对应的价值$Q$.  随机初始化当前Q网络的所有参数$w$,初始化目标Q网络$Q'$的参数$w' = w$。清空经验回放的集合$D$。
2. for i from 1 to T，进行迭代。
a) 初始化S为当前状态序列的第一个状态, 拿到其特征向量$\phi(S)$
b) 在Q网络中使用$\phi(S)$作为输入，得到Q网络的所有动作对应的Q值输出。用$\epsilon-$贪婪法在当前Q值输出中选择对应的动作$A$
c) 在状态$S$执行当前动作$A$,得到新状态$S'$对应的特征向量$\phi(S')和奖励$R$,是否终止状态is_end
d) 将$\{\phi(S),A,R,\phi(S'),is\_end\}$这个五元组存入经验回放集合$D$
e) $S=S'$
f)  从经验回放集合$D$中采样$m$个样本$\{\phi(S_j),A_j,R_j,\phi(S'_j),is\_end_j\}, j=1,2.,,,m$，计算当前目标Q值$y_j$：
$$
y_j= \begin{cases} R_j& {is\_end_j\; is \;true}\\ R_j + \gamma Q'(\phi(S'_j),\arg\max_{a'}Q(\phi(S'_j),a,w),w')& {is\_end_j\; is \;false} \end{cases}
$$
g)  使用均方差损失函数$\frac{1}{m}\sum\limits_{j=1}^m(y_j-Q(\phi(S_j),A_j,w))^2$，通过神经网络的梯度反向传播来更新Q网络的所有参数$w$
h) 如果T%C=1,则更新目标Q网络参数$w'=w$
i) 如果$S'$是终止状态，当前轮迭代完毕，否则转到步骤b)
注意，上述第二步的f步和g步的Q值计算也都需要通过Q网络计算得到。另外，实际应用中，为了算法较好的收敛，探索率$\epsilon$需要随着迭代的进行而变小。
# 4. DDQN算法实例
下面我们用一个具体的例子来演示DQN的应用。仍然使用了OpenAI Gym中的CartPole-v0游戏来作为我们算法应用。CartPole-v0游戏的介绍参见[这里](https://github.com/openai/gym/wiki/CartPole-v0)。它比较简单，基本要求就是控制下面的cart移动使连接在上面的pole保持垂直不倒。这个任务只有两个离散动作，要么向左用力，要么向右用力。而state状态就是这个cart的位置和速度， pole的角度和角速度，4维的特征。坚持到200分的奖励则为过关。
完整的代码参见我的github:[https://github.com/ljpzzz/machinelearning/blob/master/reinforcement-learning/ddqn.py](https://github.com/ljpzzz/machinelearning/blob/master/reinforcement-learning/ddqn.py)
这里我们重点关注DDQN和上一节的Nature DQN的代码的不同之处。代码只有一个地方不一样，就是计算目标Q值的时候，如下：
\#Step 2: calculate yy_batch =[]
    current_Q_batch= self.Q_value.eval(feed_dict={self.state_input: next_state_batch})
    max_action_next= np.argmax(current_Q_batch, axis=1)
    target_Q_batch= self.target_Q_value.eval(feed_dict={self.state_input: next_state_batch})foriinrange(0,BATCH_SIZE):
      done= minibatch[i][4]ifdone:
        y_batch.append(reward_batch[i])else:
        target_Q_value=target_Q_batch[i, max_action_next[i]]
        y_batch.append(reward_batch[i]+ GAMMA * target_Q_value)
而之前的Nature  DQN这里的目标Q值计算是如下这样的：
\#Step 2: calculate yy_batch =[]
    Q_value_batch= self.target_Q_value.eval(feed_dict={self.state_input:next_state_batch})foriinrange(0,BATCH_SIZE):
      done= minibatch[i][4]ifdone:
        y_batch.append(reward_batch[i])else:
        y_batch.append(reward_batch[i]+ GAMMA * np.max(Q_value_batch[i]))
除了上面这部分的区别，两个算法的代码完全相同。
# 5. DDQN小结
DDQN算法出来以后，取得了比较好的效果，因此得到了比较广泛的应用。不过我们的DQN仍然有其他可以优化的点，如上一篇最后讲到的: 随机采样的方法好吗？按道理经验回放里不同样本的重要性是不一样的，TD误差大的样本重要程度应该高。针对这个问题，我们在下一节的Prioritised Replay DQN中讨论。
（欢迎转载，转载请注明出处。欢迎沟通交流： liujianping-ok@163.com）






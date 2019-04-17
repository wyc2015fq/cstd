# 【深度学习】强化学习之DQN、Double DQN、dueling DQN原理 - zkq_1986的博客 - CSDN博客





2018年05月15日 11:04:58[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1826








DQN，deep Q-network，用一个深度网络来拟合强化学习中的Q值

Double DQN，分为主网络和target网络，主网络选择动作，target网络拟合Q值。主网络的变化浮动大，target网络变化浮动小。

q_target = reward + gamma * np.max(q_next)

dueling DQN，用一个深度网络来拟合强化学习中的Q值，只不过在网络最后部分，分为状态V值和动作V值，而Q值就通过状态V值和动作V值相加得来。其目的是说，状态值一样，而每个动作所带来的优势不一样。







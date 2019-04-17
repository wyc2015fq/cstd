# 强化学习（一）：Q_Learning - qq_38593211的博客 - CSDN博客





2018年08月04日 13:55:24[小屁猴多看书](https://me.csdn.net/qq_38593211)阅读数：78








## 1.强化学习的基本概念
- Agent：也就是强化学习需要学习的对象。
- Stage：就是指当前agent所处的状态。
- Action：对Agenti执行什么动作。
- Reward：一个Action返回的奖赏值。
- Policy：就是从state到action的映射，分为确定策略和与随机策略。

## 2.分类
- model_free：不熟悉环境，根据reward选择下一步的action（Q.Learning、Sarsa）；model_based：能预判下一部的情况的。
- 基于概率选择：能输出下一步采取各种动作的概率（Policy Gradient）；基于价值选择：根据最高价值选择动作。

## 3.Q_Learning

> 
   这里根据莫烦博客上的一个例子来阐述算法过程。[https://morvanzhou.github.io/tutorials/machine-learning/reinforcement-learning/2-1-general-rl/](https://morvanzhou.github.io/tutorials/machine-learning/reinforcement-learning/2-1-general-rl/)；这里的例子是一个找到路径（六个节点组成）最右边的宝藏。


    1.Q_table：把agent此时的的所有stage和action的reward展示出来，初始时是一个空矩阵：

```python
def build_q_table(n_states, actions):
    table = pd.DataFrame(
        np.zeros((n_states, len(actions))),     # q_table 全 0 初始
        columns=actions,    # columns 对应的是行为名称
    )
    return table
q_table=build_q_table(N_STATES, ACTIONS)
print(q_table) 



   left  right
0   0.0    0.0
1   0.0    0.0
2   0.0    0.0
3   0.0    0.0
4   0.0    0.0
5   0.0    0.0
```

    2.第二步就是从Q_table中对一个stage（第一个）然后以这个stage出发执行一个动作，然后算出此动作之后的得分，那如何选动作呢？作者给了两种，当我们第一次选时，使用随机的原则，当非第一次且随机数大于设定的贪婪值时就选择贪婪模式，即选择得分更高的那个动作。

    3.环境反馈，也就是当执行完这个任务之后，此时的stage的value状态会变吧，当然这里的value并不是最终的分，还需要经过一些变换，等会会讲，并把value赋值给R

```python
#环境反馈，每当做出一个action A之后得到的reward
def get_env_feedback(S, A):
    if A == 'right':   #这一步可以直到到达宝藏的终点
        if S == N_STATES - 2:   
            S_ = 'terminal'
            R = 1
        else:#S的value加一
            S_ = S + 1
            R = 0
    else:  
        R = 0
        if S == 0:
            S_ = S  
        else:
            S_ = S - 1#S的value减一
    return S_, R
```

    4.我们对第一个stage进行选动作执行并返回value变化之后呢？现在就要判断你执行了action之后是否达到了目标终点，如果没到了，此次迭代还要继续，我们就要先更新Q_table中的得分了，即更新Q_table

![Q(S,A)-Q(S,A)\leftarrow\alpha( R+\gamma* argmaxQ_{table}.iloc[S-, :].max()-Q(S,A))](https://private.codecogs.com/gif.latex?Q%28S%2CA%29-Q%28S%2CA%29%5Cleftarrow%5Calpha%28%20R&plus;%5Cgamma*%20argmaxQ_%7Btable%7D.iloc%5BS-%2C%20%3A%5D.max%28%29-Q%28S%2CA%29%29)

    R是此次Action的Reward，gamma参数实际上是对下一次stage动作奖励的递减参数，alpha就是对此次action的学习参数了。如果已经到了终点，那就把R后面那一项去掉就好了，一次迭代也随着到达终点结束。

    5.完整代码：

```python
import numpy as np
import pandas as pd
import time
N_STATES = 6   # 1维世界的宽度,就是迷宫的路径，从左到右一共六个结点
ACTIONS = ['left', 'right']     # 探索者的可用动作
EPSILON = 0.9   # 贪婪度 greedy，贪婪选择就是选择本次action中reward最大的
ALPHA = 0.1     # 学习率
GAMMA = 0.9    # 奖励递减值
MAX_EPISODES = 13   # 最大回合数
FRESH_TIME = 0.3    # 移动间隔时间
def build_q_table(n_states, actions):
    table = pd.DataFrame(
        np.zeros((n_states, len(actions))),     # q_table 全 0 初始
        columns=actions,    # columns 对应的是行为名称
    )
    return table
q_table=build_q_table(N_STATES, ACTIONS)
print(q_table) 
# 在某个 state 地点, 选择行为
def choose_action(state, q_table):
    state_actions = q_table.iloc[state, :]  # 选出这个 state 的所有 action 值
    if (np.random.uniform() > EPSILON) or (state_actions.all() == 0):  # 非贪婪 or 或者这个 state 还没有探索过
        action_name = np.random.choice(ACTIONS)
    else:
        action_name = state_actions.argmax()    # 贪婪模式
    return action_name
#环境反馈，每当做出一个action A之后得到的reward
def get_env_feedback(S, A):
    if A == 'right':   #这一步可以直到到达宝藏的终点
        if S == N_STATES - 2:   
            S_ = 'terminal'
            R = 1
        else:#S的value加一
            S_ = S + 1
            R = 0
    else:  
        R = 0
        if S == 0:
            S_ = S  
        else:
            S_ = S - 1#S的value减一
    return S_, R
def update_env(S, episode, step_counter):

    env_list = ['-']*(N_STATES-1) + ['T']   #为可视化做准备，六个 路径“-”一个终点T
    if S == 'terminal':
        interaction = 'Episode %s: total_steps = %s' % (episode+1, step_counter)
        print('\r{}'.format(interaction), end='')
        time.sleep(2)
        print('\r                                ', end='')
    else:
        env_list[S] = 'o'
        interaction = ''.join(env_list)
        print('\r{}'.format(interaction), end='')
        time.sleep(FRESH_TIME)
def rl():
    q_table = build_q_table(N_STATES, ACTIONS)  # 初始 q table
    for episode in range(MAX_EPISODES):     # 回合
        step_counter = 0
        S = 0   # 回合初始位置
        is_terminated = False   # 是否回合结束
        update_env(S, episode, step_counter)    # 环境更新
        while not is_terminated:

            A = choose_action(S, q_table)   # 选行为
            S_, R = get_env_feedback(S, A)  # 实施行为并得到环境的反馈
            q_predict = q_table.loc[S, A]    # 估算的(状态-行为)值
            if S_ != 'terminal':
                q_target = R + GAMMA * q_table.iloc[S_, :].max()   #  实际的(状态-行为)值 (回合没结束)
            else:
                q_target = R     #  实际的(状态-行为)值 (回合结束)
                is_terminated = True    # terminate this episode

            q_table.loc[S, A] += ALPHA * (q_target - q_predict)  #  q_table 更新
            S = S_  # 探索者移动到下一个 state

            update_env(S, episode, step_counter+1)  # 环境更新

            step_counter += 1
    return q_table
```







# 强化学习  —— gym - Zhang's Wikipedia - CSDN博客


2018年11月09日 19:37:07[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：359



## 0. gym 下的数据结构
Discrete：gym.spaces.discrete.Discrete
env.action_sample 的返回值就是 Discrete 类型
Discrete(n)：{0, 1, 2, 3, n-1}，表示的是正整数的离散空间；
成员函数：sample()：离散值空间中进行采样；
contains(k)：离散值空间中是否包含 k；

## 1. 认识游戏编程的一些基本概念
environment
action space 与 action
import gym
env = gym.make('CartPole-v0')
env.reset()
for _ in range(1000):
    env.render()
    action = env.action_space.sample()
    env.step(action) \# take a random action


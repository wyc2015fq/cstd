# DeepMind发布《星际争霸 II》深度学习环境 - 人工智能学家 - CSDN博客
2017年09月22日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：1361
![640.jpeg?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/bicdMLzImlibRvEeolCM1C9evkTtNtfs8oE0TUUjiapVVMuu0YHH6t9DtF7qMKK9RQmrjLhEyWa3vovCldMMhwyaA/640.jpeg?wxfrom=5&wx_lazy=1)
*来源：AI 研习社*
*概要：**上个月，DeepMind和暴雪终于开源了《星际争霸II 》的机器学习平台。本文介绍了基于星际争霸II游戏的强化学习环境SC2LE（《星际争霸II 》学习环境）。*
**StarCraft II: A New Challenge for Reinforcement Learning**
**星际争霸 II:增强学习的新挑战**
上个月，DeepMind和暴雪终于开源了《星际争霸II 》的机器学习平台。本文介绍了基于星际争霸II游戏的强化学习环境SC2LE（《星际争霸II 》学习环境）。
论文描述《星际争霸II 》行动和奖励规范，并提供一个开源的Python界面，用于与游戏引擎进行通信。暴雪提供游戏输入输出的API，DeepMind又做了层基于Python的封装。
除了主要的游戏地图，他们提供了一套专注于《星际争霸II 》游戏不同元素的迷你游戏。对于主要的游戏地图，还提供了一个伴随专业玩家的游戏重播数据数据集。从该数据训练的神经网络的初始基线结果，预测游戏结果和玩家行为。
![0.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/bicdMLzImlibRvEeolCM1C9evkTtNtfs8owicSPXx1pnKgicicov0AbJ9hAmhYNK7iceOHNeqIsfht4k55etBuG9WjQQ/0.jpeg?)
文中使用了被称为Blizzard分数的一个中间分数，这个分数包含了一个对当前资源、等级以及单位和建筑的加权和，如果玩家赢得战斗而且管理好资源，这个分数就会单调增加，失败的时候，这个分数就会减少。此外，系统还使用了一系列的特征层为AI编码相关信息，比如地形高度、相机位置、屏幕中单位的血量以及各种其他信息。
最后，提出了应用于《星际争霸II 》的规范深强化学习的初始基线结果。DeepMind之后也会发布上百万帧的选手游戏记录，这对模拟环境研究也会有很大的帮助。
 观看论文解读大概需要  6  分钟
学霸们还请自行阅读论文以获得更多细节

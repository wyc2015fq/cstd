# OpenAI发布强化学习环境Gym Retro：支持千种游戏 - 知乎
# 



选自OpenAI Blog，作者：Vicki Pfau等，机器之心编译。

> Gym 是 OpenAI 发布的用于开发和比较强化学习算法的工具包。使用它我们可以让 AI 智能体做很多事情，比如行走、跑动，以及进行多种游戏。目前，它运行在支持 Python 3.5 和 3.6 的 Linux、macOS 和 Windows 系统上。

项目链接：[https://github.com/openai/retro/tree/develop](https://link.zhihu.com/?target=https%3A//github.com/openai/retro/tree/develop)

OpenAI 近日发布了完整版游戏强化学习研究平台——Gym Retro。在本次发布之后，OpenAI 公开发布的游戏数量将从大约 70 个雅达利和 30 个世嘉游戏增加到了 1000 多个游戏，其中包括对任天堂 Game boy 等各种模拟器的支持。此外，OpenAI 还将发布用于向 Gym 平台添加新游戏的工具。
![](https://pic4.zhimg.com/v2-3a7547311baa8d4c2d099e4d8c86c25f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='730' height='487'></svg>)
OpenAI 利用 Gym Retro 对强化学习算法及学习能力的泛化进行了研究。RL 领域之前的研究主要集中在优化智能体以完成单个任务上。Gym Retro 可以帮助研究在概念相似但外观不同的游戏之间进行泛化的能力。

此版本包括来自世嘉 Genesis 和世嘉 Master System 的游戏，以及任天堂的 NES、SNES 和 Game Boy 控制台，还包括对世嘉 Game Gear、任天堂 Game Boy Color、任天堂 Game Boy Advance 及 NEC TurboGrafx 的初步支持。一些已发布的游戏集成，包括 Gym Retro data/experimental 文件夹中的那些游戏，都处于测试状态——如果试用中遇到任何 bug，可以联系 OpenAI。由于所涉及的变更规模很大，代码暂时只能在 Branch（[https://github.com/openai/retro/tree/develop](https://link.zhihu.com/?target=https%3A//github.com/openai/retro/tree/develop)）上使用。为了避免破坏参赛者的代码，在比赛结束之前，OpenAI 将不会合并 Branch。

正在进行的 Retro Contest（几周后结束！）及 OpenAI 最近的技术报告（[https://arxiv.org/abs/1804.03720](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.03720)）集中讨论了在同一游戏（Sonic The Hedgehog™）的不同级别之间进行泛化的比较容易的问题。完整的 Gym Retro 数据集进一步完善了这一问题，并使研究不同游戏之间更难的泛化问题成为可能。数据集的规模和单个游戏的难度使其成为一个巨大的挑战，OpenAI 希望在明年分享自己的研究进展。他们还希望 Retro Contest 参与者开发的一些解决方案能够得到扩展并应用到整个 Gym Retro 数据集。




**集成工具**
![](https://pic3.zhimg.com/v2-a885b7b939168687926ffef2cdd7cf5a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='736' height='508'></svg>)
OpenAI 还将发布用于集成新游戏的工具。如果你有游戏 ROM，此工具可以帮你轻松创建储存状态、寻找内存位置以及设计强化学习智能体可以实施的方案。OpenAI 已经为希望增加新游戏支持的人编写了一个集成器指南（[https://github.com/openai/retro/blob/master/IntegratorsGuide.md](https://link.zhihu.com/?target=https%3A//github.com/openai/retro/blob/master/IntegratorsGuide.md)）。

集成工具还支持录制、播放将所有按钮输入保存到游戏中的视频文件。这些文件很小，因为它们只需要存储按钮按下的开始状态和顺序，而不是输出的每一帧。像这样的视频文件对于可视化增强学习智能体正在执行的操作以及存储用作训练数据的人工输入非常有用。




**Farming 奖励**
![](https://pic4.zhimg.com/v2-0fb800395582d8663fcbd8c4d84f710b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='734' height='308'></svg>)
在开发 Gym Retro 时，OpenAI 的研究人员发现很多 AI 智能体学会了 farm 奖励（只专注于游戏得分），而忽略了完成隐藏的真正任务。如上图所示，进行 Cheese Cat-Astrophe（左）和 Blades of Vengeance（右）游戏的智能体都陷入了疯狂得分的死循环。这是 OpenAI 之前讨论过的一种现象（[https://blog.openai.com/faulty-reward-functions/](https://link.zhihu.com/?target=https%3A//blog.openai.com/faulty-reward-functions/)）：当我们只给强化学习算法一个简单的奖励函数时（如最大化游戏得分），可能会导致智能体出现错误的行为。
![](https://pic3.zhimg.com/v2-ee567ec3811330a24b2a9bc92cae4942_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='727' height='334'></svg>)
对于密集奖励（频繁和增量）游戏而言，最难的地方在于需要进行快速反应，像 PPO 这样的强化学习算法可以很好地应对这种挑战。

在 Gradius 这样的游戏中（上图右侧），你会在每次击中敌人之后得到奖励点数，这意味着开始机器学习的速度会很快。在这样的游戏中生存下来需要你具备躲避敌人攻击的能力：这对于强化学习算法而言并不困难，因为它们玩游戏是逐帧进行的。

对于只有稀疏奖励，或需要计划超过未来数秒策略的游戏，目前的算法还很难应对。Gym Retro 数据集中的许多游戏都是稀疏奖励或需要计划的，因此，处理整个数据集中的内容可能需要开发者找到全新技术。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://blog.openai.com/gym-retro/](https://link.zhihu.com/?target=https%3A//blog.openai.com/gym-retro/)



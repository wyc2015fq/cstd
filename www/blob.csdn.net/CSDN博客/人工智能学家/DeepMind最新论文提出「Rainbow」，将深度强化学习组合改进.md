# DeepMind最新论文提出「Rainbow」，将深度强化学习组合改进 - 人工智能学家 - CSDN博客
2017年10月12日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：202
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0fmxmGhSYfVtk3k92zbiczMkQ62ApWhxXhHbOJGgn5cXxaxxvz2iaLTD2r3vAVqWyp3eJ8kpf3eT2XA/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
*来源**：本文**经公众号「雷克世界」授权转载（微信号：ROBO_AI）*
*作者：DeepMind   编译：嗯~阿童木呀、多啦A亮*
*概要：在将强化学习（RL）扩展到复杂序列决策问题（sequential decision-making problems）上，现如今的许多成功都是由Deep Q-Networks算法带来的（Mnih等人于2013年，2015年提出）。*
相信那些时刻关注深度强化学习的人士都知道，深度强化学习社区已经对DQN算法进行了若干次独立的改进。但是，目前尚不清楚这些扩展中的哪些是互补的，同时可以有效地组合在一起。本文研究了DQN算法的六个扩展，并对其组合进行了实证研究。我们的实验表明，从数据效率和最终性能方面来说，该组合能够在Atari 2600基准上提供最为先进的性能。我们还提供详细的消融研究结果（ablation
 study），该研究结果显示了每个成分对整体性能的影响。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0fmxmGhSYfVtk3k92zbiczMksNyM052mv4RFjcIdQpSaGHS1aHajL9rbIlHiaOk1Dpdm0tWvku6CMEQ/640?wx_fmt=png)
*图1：在57 Atari游戏中，中等人类水平的表现。我们将我们的集成智能体（彩虹色）与DQN（灰色）以及六个已发布的基准进行比较。需要注意的是，在700万帧之后我们会得到与DQN的最佳性能匹配，超过4400万帧内的任何基线，并且显著地提高了最终性能。曲线平滑，且移动平均值超过5点。*
**介绍**
在将强化学习（RL）扩展到复杂序列决策问题（sequential decision-making problems）上，现如今的许多成功都是由Deep Q-Networks算法带来的（Mnih等人于2013年，2015年提出）。它将具有卷积神经网络的Q-learning与经验回放（experience replay）组合在一起，使其能够从原始像素点中学习如何以人类水平的水平玩Atari游戏。从那时起，人们开始提出许多扩展版本，以提高其速度或稳定性。
双DQN（DDQN，van Hasselt，Guez和Silver于2016年提出）通过对bootstrap action的解耦选择和评估，解决了Q-learning的高估偏差（van Hasselt 等人于2010年提出）问题。优先化经验回放（Prioritized experience replay）（Schaul等人于2015年提出）通过更频繁地回放那些有更多值得学习的转换来提高数据效率。竞争网络（dueling
 network）架构（Wang等人于2016年提出）通过分别表示状态值和操作优势，以进行跨操作泛化。正如在A3C中使用那样，从多步bootstrap目标中学习，将改变“偏差—方差”的平衡，并有助于将新观察到的奖励更快地传播到早期的访问状态。分布式Q-learning学习了一种折扣回报的分类分布，而不是对平均值进行评估。噪声DQN（Noisy DQN）则使用随机网络层进行探索。而这个清单也理所当然不是详尽无遗的。
这些算法中的每一个都可以独立地实现显著的性能改进，由于它们是通过解决根本不同的问题来实现的，并且由于它们是建立在一个共享的框架上的，所以它们很有可能会被组合在一起。在某些情况下，这已经做到了这一点：优先化DDQN（Prioritized DDQN）和竞争DDQN（dueling DDQN）都使用双向Q-learning，而竞争DDQN也与优先化经验回放组合在一起。
在本文中，我们提出研究一种结合了上述所有成分的智能体。我们的研究结果展示了这些截然不同的想法是如何被组合在一起的，而且它们确实在很大程度上是互补的。实际上，从数据效率和最终性能方面来说，在Arcade学习环境（Bellemare等人于2013年提出）中，他们的组合使得57 个Atari 2600游戏中最先进的基准测试结果。在最后，我们展示了消融研究的结果，以帮助了解不同成分对整体性能的影响。
**讨论**
我们已经证明，DQN的几个改进可以被成功地整合到一个单一的学习算法中，以达到最先进的性能。此外，我们已经表明，在集成算法中，除了一个成分之外，所有成分都提供了明显的性能优势。还有更多的算法成分，我们无法都包含，这将是对一体化智能体进一步实验的有希望的候选成分。在许多可能的候选成分中，我们将在以下内容中讨论几个。
我们在这里重点关注Q-learning系列中基于值的方法。我们没有考虑到纯粹的基于策略的强化学习算法，例如置信域策略优化（trust-region policy optimisation ，Schulman等人于2015年提出），也没有考虑到actor-critic方法（Mnih等人于2016年提出； O'Donoghue等人于2016年提出）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0fmxmGhSYfVtk3k92zbiczMk3cmX2O7xJvnQg7pntRicCLiaES3d2QuWQAlIm8BQWYkT5rj7V7BbSZTA/640?wx_fmt=png)
*图4：所有57个Atari游戏的独立智能体（ablation agents）性能下降测试。性能是学习曲线下的面积，相对于Rainbow智能体和 DQN进行了规则化。其中，DQN超越Rainbow的两种游戏被剔除了。导致性能下降最严重的成分在每个游戏中都被高亮显示了。删除优先级和多步骤学习在大多数游戏中造成的性能影响最大，不过每个成分在不同游戏中的影响各有差异。*
许多算法利用一系列数据来提高学习效率。优化收紧（Optimality tightening）（He等人于2016年提出）使用多步回归来构造额外的不等式边界，而不是使用它们代替Q-learning中使用的一步（1-step）目标。资格迹（Eligibility traces）允许在n-step回归上进行软性组合（Sutton等人于1988年提出）。然而，序贯法（sequential
 methods）比Rainbow中使用的多步目标更能计算每个梯度的计算量。此外，引入优先级序列重放提出了如何存储、重放和优先顺序的问题。
情景控制（Episodic control ，Blundell等人于2016年提出）也着重于数据效率，并被证明在某些领域是非常有效的。它通过使用情景记忆作为补充学习系统来改善早期学习，能够立即重新制定成功的动作序列。
除了噪声网络，许多其他的探索方法也可能是有用的算法要素：在这些自助式DQN（Bootstrapped DQN）（Osband等人于2016年提出）、内在动机（intrinsic motivation）（Stadie，Levine和Abbeel 等人于2015年提出）和基于数量的探索（Bellemare等人于2016年提出）中。这些替代成分的整合是进一步研究的成果。
在本文中，我们将重点放在核心的学习更新上，而无需探索其他计算架构。并行副本环境的一部学习，如在A3C（Mnih等人于2016年提出）、Gorila（Nair等人于2015年提出）或进化策略（Salimans等人于2017年提出）中可以有效加速学习，至少在执行时间方面。但是请注意，它们的数据效率较低。
分层强化学习（Hierarchical RL）也被成功应用于几个复杂的Atari游戏。在分层强化学习的成功应用中，我们强调了h-DQN（Kulkarni 等人于2016年提出）和Feudal网络（Vezhnevets等人于2017年提出）。
通过利用诸如像素控制或特征控制（Jaderberg等人于2016年提出）、监督预测（Dosovitskiy和Koltun于2016年提出）或后继特征（Kulkarni等人于2016提出）等辅助任务也可以使状态表现更加有效。
为了评估Rainbow相对于基准线的公平性，我们遵循了对剪裁奖励、固定动作重复和帧叠加的常规域修改，但是这些修改可能会被其他学习算法改进。波普艺术规范化（Pop-Art normalization）（van Hasselt等人于2016年提出）允许删除奖励剪裁，同时保持类似的性能水平。精细的动作重复（Fine-grained action repetition）（Sharma，Lakshminarayanan和Ravindran
 等人于2017年提出）能够学习如何重复动作。一个循环状态网络（Hausknecht和Stone等人于2015年提出）可以学习时间状态表示，代替观察帧的固定堆叠。一般来说，我们认为将真实游戏暴露给智能体是未来研究具有前途的方向。

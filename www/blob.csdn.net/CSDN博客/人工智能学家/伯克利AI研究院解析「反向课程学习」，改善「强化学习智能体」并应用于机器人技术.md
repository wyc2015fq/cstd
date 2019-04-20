# 伯克利AI研究院解析「反向课程学习」，改善「强化学习智能体」并应用于机器人技术 - 人工智能学家 - CSDN博客
2017年12月23日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：295
![?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFskV4ckVVVWJibssCXefMKRWl7RImaXbutKPUib276aB1zz2oXSJHWKSDw/?wx_fmt=png&wxfrom=5&wx_lazy=1)
原文来源：arXiv
作者：Wieland Brendel、Jonas Rauber、Matthias Bethge
「雷克世界」编译：嗯~阿童木呀、哆啦A亮
众所周知，强化学习（RL）是一种强大的技术，它能够解决诸如移动（locomotion）、Atari游戏、赛车游戏以及机器人操作等复杂的任务，而这些全部是通过在一个奖励函数上训练智能体以其优化行为实现的。但是，对于许多任务来说，我们很难设计一个既容易训练又能产生理想行为的奖励函数。假设我们想要一个机械手臂学习该如何将一个环放到一个钉子上，那么最自然的奖励函数为，若智能体到达所需的终端配置下将获得值1的奖励，否则为0。然而，对于这个任务所要求的动作——在钉子的顶部对齐该环，然后将其滑动到底部，在这样一个二进制奖励下进行学习是不切实际的，因为我们最初策略中的一般随机探索不太可能实现这样的目标，如视频1a所示。另一方面，我们可以试着对奖励函数的形式做些调整，以潜在地缓解这个问题，但找到一个良好的形式需要大量的专业知识和实验经验。例如，直接将环的中心与钉子底部之间的距离最小化会导致一种不成功的策略，也就是将环撞击到钉子上，如视频1b所示。对此，我们提出一种不用修改奖励函数的有效学习方法，通过在开始位置自动生成课程以进行学习。
![?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsvQChQWGFtTjYicS608cMb0pM8pVqriaV77xouuBiafgxibNibN3zFrH9pRQ/?wx_fmt=gif)
视频1a：随机初始化策略无法从大多数起始位置达到目标，因此无法进行学习。
![?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFstAg6A3vcorWpvNIatWGXWoicicXaoruClYibgjhDY0icCbVoGdIl9FbR2g/?wx_fmt=gif)
视频1b：在从环中心到钉子底部的距离上，用一个惩罚对奖励函数进行调整，会产生一个非预期内的行为。
**生成课程而不是调整奖励函数**
我们希望训练一个智能体，使其能够从任何起始位置到达目标，而不需要专家对奖励函数进行调整。显然，并不是所有的起始位置的难易程度都是一样的。尤其是，即使是放在目标附近的随机智能体，也可以在某时刻达到目标，获得奖励，从而开始学习！随后，这些获得的知识可以被引导用以解决起始位置远离目标的任务。通过选择我们在训练中使用的起始位置的顺序，我们可以利用这个问题的基本结构，并提高学习效率。这种技术的一个关键优势是奖励函数不被修改，而直接对优化稀疏奖励进行优化不容易产生不良行为。对一系列相关学习任务进行排序被称为课程学习（curriculum learning），而对我们来说，一个关键问题是如何选择这个任务的顺序。我们的方法，关于此，我们将在下面进行更详细的解释，它使用学习智能体的性能以自动生成从目标开始并向外扩展的任务课程。
**反向课程学习**
在面向目标的任务中，目的是从任何起始状态都能够到达期望的配置。例如，在上面介绍的“将环放置在钉子上”任务中，我们希望从任何起始配置开始都能够将环放置在钉子上。从大多数的起始位置来看，我们初始策略的随机探索始终没有到达目标，因此没有获得任何奖励。尽管如此，在视频2a中可以看出，如果一个随机策略从附近的位置初始化，那么这个随机策略就非常有可能到达钉子的底部。然后，一旦我们已经学会了该如何从目标周围到达目标，那么学习从更远的起始位置开始也变得很简单了，原因在于如果探索行为将其状态驱动到目标附近，那么智能体就已经知道该如何继续学习了，就像在视频2b中那样。最终，智能体成功地学会从广泛的起始位置到达目标，如视频2c所示。
![?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsVescrOZL9E0JehCF7vV8HicNrWBtp7698jHibtZ5EDeEMibrQyJgj6C4A/?wx_fmt=gif)
![?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsM7jSyQJbetJgONl743pMVdr558fuUFzAd69n0Pll5nyxJrBJoicmSEw/?wx_fmt=gif)
![?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsEHr1Ak0Whfn8UiaAUkch1iaTUoJprodiaBA1ibhNHT99ovYwD1OjHllgeQ/?wx_fmt=gif)
视频2a-c：我们的方法是先从附近的目标开始学习，然后逐渐地从起始位置开始反向扩展。
这种反向学习或者从目标向外扩展的方法，是启发于动态规划方法，在这种方法中，可以使用更为简单的子问题的解来计算更为复杂的问题的解。
**中级难度起点（SoID）**
为了实现这个反向课程，我们需要确保，对于智能体来说这个向外扩展能够以适当的速度进行。换句话说，我们想要用数学语言描述一组追踪当前智能体性能的启动，并为我们的强化学习算法提供一个很好的学习信号。我们特别关注的是策略梯度算法，通过在总预期奖励的梯度估计的方向上采取步骤以改进参数化策略。这个梯度估计通常是原来的强化（REINFORCE）（https://link.springer.com/article/10.1007/BF00992696）的一个变体，通过收集从状态{si0}i=1..N开始的N个策略轨迹{τi}i=1..N进行估计。
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsjGW9hZMnH9D4YU3JicaPiarfsr7LM7BdfKZ1Mw1Cd0iczWcAUaBJmYiahw/?wx_fmt=png)
在面向目标的任务中，轨迹奖励R(τi,si0)是二进制的，以表明智能体是否到达了目标。因此，如果从si0开始执行当前策略πθ，则通常的基线R(πi,si0)将会对到达目标的概率进行估计。因此，我们从方程（1）中看到，从成功概率为0或1的起点si0收集的轨迹对应的求和项将会消失。这些都是“浪费”的轨迹，因为它们对于梯度的估计没有起到什么作用——它们要么太难，要么太简单。在我们之前关于多任务强化学习的研究中已经引入了类似的分析。在这种情况下，为了避免我们现有的策略从永远无法到达目标或已经到达目标的位置开始训练，我们引入了“中间难度起点”（SoID）的概念，它们是开始状态s0，满足：
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsCCxo3IXibxYSP9bag8ABAicbicurUY1GRD8z5ZNptiadCMoRc8efeS9ruw/?wx_fmt=png)
Rmin和Rmax的值可以对最小成功概率（从这个开始点训练的可接受的）和最大成功概率（高于我们所更愿意关注的从其他起点开始训练的）进行估计。在我们所有的实验中，我们使用的是10％和90％。
**反向课程的自动生成**
从上面的知识和推导出发，我们想用从SOID状态开始的轨迹训练我们的策略。不幸的是，在每次策略更新中都找到了完全满足方程式（2）的所有起始点，因此我们引入一个有效的近似来自动生成这个反向课程：我们在先前迭代期间被估计为中级难度起点的点附近采样相关状态。要做到这一点，我们提出了一种方法，通过在最后一次训练迭代中收集的轨迹对非中级难度起点的起点进行过滤，然后对附近的状态进行采样。完整的算法在视频3中进行了说明，详细信息如下：

**过滤掉非中级难度起点（Non-SoID）**
在每个策略梯度训练迭代中，我们从一些起始位置{ si0} i = 1..N收集N个轨迹。对于大多数起始状态而言，我们至少从那里收集三条轨迹，因此我们可以对从这些起点Rθ（si0）开始的策略成功概率进行蒙特卡洛估计计算。对于估计值不在固定边界Rmin和Rmax内的每个si0来说，我们将会将该起点丢弃，以便在下一次迭代期间不会从该处进行训练。起点处，以前策略的SoID可能不是当前策略的SoID，因为它们现在已经到达目标了，或者由于更新后的策略变得更糟了，所以重要的是要保持过滤掉非SoID以维护一个课程以适合于当前智能体的性能。
**附近采样**
在过滤非SoID之后，我们需要获得新的SoID以继续扩展我们开始训练的起点。我们通过对剩余的SoID附近的状态进行采样，因为这些状态与当前的策略有相似的难度，因此也可能是SoID。但是，什么样的方法算是对某个状态si0附近进行采样的好方法呢？我们建议从这个si0中采取随机探索行为，并记录访问状态。这种技术比直接在状态空间中应用噪声要好得多，因为这样做可能产生不可行的状态，或者无法通过执行来自原始si0的动作实现。
**假设**
为了初始化算法，我们需要在目标sg 处进行一个初始化，然后从中运行布朗运动，从收集的起始点进行训练，过滤掉非SoID并进行迭代。这些在指定问题中通常很容易获得，而且这只是一个简要的假设，而不是要求充分证明如何达到这一点。
我们的算法利用从收集的轨迹起点处选择起点分布的能力。在许多系统中都是如此，就像所有的模拟系统一样。Kakade与Langford（http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.7.7601）也提出了这一假设，并提出修改起点分配的有用性的理论证据。
**应用于机器人**
导航到一个固定的目标和对期望配置进行精细的操作是两个面向目标的机器人任务的例子。我们分析了提出的算法是如何自动生成用于以下任务的反向课程：质点迷宫（Point-mass Maze）（图1a）、蚂蚁迷宫（图1b）、环上钉（图1c）和钥匙插入（图1D）。
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsOLJheWQKMhXA44xgZw9ZqwYxwvA1XSib2SZyYEicHRwE1RHxFCb6o7Ig/?wx_fmt=png)
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsD4GAnVzyqZWGYLZiat54Lj9n2zPRL2JeJia6yk6QA92ma38gS8Ka5wLA/?wx_fmt=png)
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFskV4ckVVVWJibssCXefMKRWl7RImaXbutKPUib276aB1zz2oXSJHWKSDw/?wx_fmt=png)
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsiayX4M4OTeGg6mTWMicPJvmOqjq0QT7Rtz0YDic2K0BFSuXQeQGDGfIlA/?wx_fmt=png)
图1a-d：我们说明我们方法的性能的任务（从上到下）：质点迷宫、蚂蚁迷宫、环上钉、钥匙插入。
**质点迷宫（Point-mass Maze）**
在这个任务中，我们要学习如何从迷宫中的任何起点到达图1a右上角的红色区域的末端。 我们在图2中看到，随机初始化策略——就像迭代i=1时那样，在任何位置成功的概率都为0，但它在目标周围。图2的第二行显示了我们的算法如何在i=1的目标附近提出起始位置。我们在后续的文章中看到，由我们的方法生成的起点保持对一个区域进行追踪，而在该区域中训练策略有时成功，但并非总是成功，因此为任何策略梯度学习方法提供了良好的学习信号。
![?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsu0tRPwZtFxSySl0Do9aKCMekv5WgdP4TOp99xabf8LBry3Ag3P1aVA/?wx_fmt=png)
图2：策略执行的快照和我们反向课程生成的起点（重播缓冲区没有清晰地描述），始终以中等难度级别对区域进行追踪。
为了避免忘记该如何从某些地方达到目标，我们保留所有以前策略中为SoID的起点重播缓冲区。在每次训练迭代中，我们从这个重播的状态中抽取一小部分轨迹。
**蚂蚁迷宫导航（Ant Maze Navigation）**
在机器人技术中，往往需要复杂的协调运动才能达到所需的配置。例如，如图1b所示的四足动物需要知道如何协调其所有的力矩来向目标移动和前进。从视频4中展示的最终策略中可以看出，即使在达到目标时只提供成功/失败奖励，我们的算法也能够学习这种行为。奖励函数没有修改，包括任何距离目标，质量速度中心或探索奖金。
![?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFscVu9Ij7ic3sXTeFibPZuV0pjNJbib6XibByJnLrrVjYoIibPVmwFdAkCXjw/?wx_fmt=gif)
视频4：当我们用反向课程方法训练时，即使在稀疏的奖励下，也出现了复杂的协调和使用环境接触的现象。
**细粒度操作（Fine-grained Manipulation）**
我们的方法也可以解决复杂的机器人操作问题，如图1c和1d所示。这两项任务都有一个七自由度的手臂，并具有复杂的接触限制。第一个任务要求机器人将一个环向下插入钉子的底部，第二个任务试图将一个钥匙插入锁中，顺时针旋转90度，将其进一步插入并逆时针旋转90度。在这两种情况下，只有在达到所需的最终配置时才会给予奖励。在没有课程的情况下，最先进的强化学习算法无法学习如何解决任务，但是通过反向课程生成，我们可以从一系列广泛的起始位置获得成功的策略，如在视频5a和5b中所观察到的。
![?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFsEHr1Ak0Whfn8UiaAUkch1iaTUoJprodiaBA1ibhNHT99ovYwD1OjHllgeQ/?wx_fmt=gif)
![?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/waLJGrhJM0euXbqibKj3dvhAkmA2MuvFs1YkiaCBaoGQnGmIz9a52t3dkoJseE0SiawjPuGIpVaHy1hyHBBrLq2Ww/?wx_fmt=gif)
视频5a（上）和视频5b（下）：通过反向课程方式在将环放置在钉子上和钥匙插入任务中获得的最终策略策略。该智能体从一系列广泛的初始化位置中获得了成功，并能够利用接触来指导自己。
**结论和未来的方向**
最近，强化学习方法已经从单一任务范式转向解决任务集。这是为了更接近现实世界的场景，每次需要执行任务时，起始配置、目标或其他参数都会有变化。因此，推进课程学习领域来开拓这些任务的基本结构是至关重要的。我们的反向课程策略是朝这个方向迈出的一步尝试，在运动和复杂的操作任务中产生令人印象深刻的结果，这些任务在没有课程的情况下是无法解决的。
此外，在我们用于操作任务的最终策略的视频中可以观察到，智能体学会了利用环境中的接触物而不是避开他们。因此，基于本文所提出的部分方法的学习在解决传统运动规划算法难以解决的问题上具有很大的潜力，如非刚体对象的环境或任务几何参数的不确定性。我们的未来工作是将我们的课程生成方法与域随机化方法结合起来，以获得可迁移到现实世界的策略。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎支持和加入我们。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXoz49H9w8lfGXmBjcGga9W15bmYAvZDqbbI8p0liaLcxEh1iao7YXIgJvp6MLHpskd7jHq8ibX0QzDQ/640?wx_fmt=png)

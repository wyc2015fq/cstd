# 在个人电脑上快速训练Atari深度学习模型：Uber开源「深度神经进化」加速版 - 知乎
# 



选自Uber AI，作者：Felipe Petroski Such、Kenneth O. Stanley、Jeff Clune，机器之心编译。

> Uber 在去年底发表的研究中发现，通过使用遗传算法高效演化 DNN，可以训练含有超过 400 万参数的深度卷积网络在像素级别上玩 Atari 游戏；这种方式在许多游戏中比现代深度强化学习算法或进化策略表现得更好，同时由于更好的并行化能达到更快的速度。不过这种方法虽好但当时对于硬件的要求很高，近日 Uber 新的开源项目解决了这一问题，其代码可以让一台普通计算机在 4 个小时内训练好用于 Atari 游戏的深度学习模型。现在，技术爱好者们也可以接触这一前沿研究领域了。

项目 GitHub 地址：[https://github.com/uber-common/deep-neuroevolution/tree/master/gpu_implementation](https://link.zhihu.com/?target=https%3A//github.com/uber-common/deep-neuroevolution/tree/master/gpu_implementation)

Uber 去年底曾发表了[五篇有关深度神经进化的论文](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650735095%26idx%3D4%26sn%3D5d33ab2a083e49ad63fd73a1e982a650%26chksm%3D871ac589b06d4c9f2ca81e4323a3185125d0e0ee8131dc75bcc4b27b0541082320371fc547a7%26scene%3D21%23wechat_redirect)，其中包括遗传算法可以解决深度强化学习问题的发现，以及一些有前景的其他方式，如深度 Q 学习和策略梯度。这些工作是 Salimans 等人 2017 年研究《Evolution Strategies as a Scalable Alternative to Reinforcement Learning》的后续，后者展示了另一种神经进化算法进化策略（ES）的能力。Uber 进一步介绍了如何通过添加探索新奇的智能体来改进 ES（论文《Improving Exploration in Evolution Strategies for Deep Reinforcement Learning via a Population of Novelty-Seeking Agents》），以及 ES 与梯度下降的相关性。所有这些研究在此前都是非常耗费计算资源的：需要使用 720-3000 块 CPU，在分布式大型高性能计算机集群上运行，这为大多数研究者、学生、公司和爱好者对深度神经进化的进一步探索带来了阻力。

不过这一问题最近已经得到了解决，Uber 于昨日开源的新代码使得快捷方便地展开此类研究成为可能。有了新的代码，原先需要花费 720 个 CPU、一个小时训练的 Atari 游戏深度神经网络，现在只需要在常见的台式机上单机花费 4 个小时就可以完成训练了。这是一个非常重要的进展，它极大地改变了人们对于此类研究所需资源的看法，使得更多研究者们可以进入这一领域。
![](https://pic3.zhimg.com/v2-7ac130ff3edb20ed0e3d615a1ffa8d0a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1059' height='585'></svg>)
神经进化技术是解决具有挑战性的深度强化学习问题颇具竞争力的方案，其可用范围包括 Atari 游戏、类人体仿真运动等等。上图展示了使用简单遗传算法进行深度神经网络训练的一些形式。




**什么使其速度加快，并且可在一台计算机上运行？**

现代的高端计算机具备数十个虚拟核，这种计算机本身就像一个计算集群。如果采用适当的方式执行并行评估，那么在 720 个内核上耗时一小时的运行可在一个 48 核的个人计算机上运行，耗时 16 小时，速度较慢，但是也还可以。现代计算机还有 GPU，因此可以较快运行深度神经网络（DNN）。Uber 的代码最大化 CPU 和 GPU 的并行使用。在 GPU 上运行深度神经网络，在 CPU 上运行域（如视频游戏或物理模拟器），并且在同一批次中并行执行多个评估，这使得所有可用的硬件都得到高效利用。如下所述，它还包含自定义 TensorFlow 操作，极大地提高了训练速度。

在 GPU 上训练需要对神经网络操作的计算方式进行若干修改。在 Uber 的设置中，使用单个 CPU 运行单个神经网络的速度比使用单个 GPU 要快，但是当并行执行类似计算时（如神经网络的前向传播），GPU 的效果更好。为了利用 GPU，Uber 将多个神经网络前向传播集成到批次中。这种做法在神经网络研究中很常见，但是通常是同一个神经网络处理一批不同输入。但是 Uber 的做法使用的是多个不同的神经网络，不过即使网络不同，该操作仍然实现了加速（对内存的要求也提高了）。Uber 使用 TensorFlow 基础操作实现了多个神经网络批次处理，并实现了大约 2 倍的加速，将训练时间降低到大约 8 小时。但是，研究人员认为他们可以做得更好。尽管 TensorFlow 提供所有需要的操作，但是这些操作并不是为这种计算量身定做的。因此，Uber 添加了两种自定义 TensorFlow 操作，由此再次获得了 2 倍的加速，将在单个计算机上的训练时间减少到大约 4 小时。

第一个自定义 TensorFlow 操作大大加速了 GPU 的速度。它专为强化学习领域中异质神经网络计算而开发，这些计算中的 episode 长度不同，正如在 Atari 和很多其他仿真机器人学习任务中那样。该操作使 GPU 仅运行必须运行的神经网络，而不是每次迭代中都运行固定数量（大量）的神经网络。

这些改进使得 GPU 在成本方面优于 CPU。实际上，GPU 非常快，Atari 模拟（CPU）都跟不上了，即使使用了多进程库执行计算并行化。为了改进模拟性能，Uber 添加了第二套自定义 TensorFlow 操作，将 Atari 模拟的封装器从 Python 转到了自定义 TensorFlow 命令（重置、step、观测），便于利用 TensorFlow 提供的快速多线程功能，省略了 Python 和 TensorFlow 交互造成的减速。所有这些改变带来了 Atari 模拟器中大约 3 倍的加速。这些创新应该会加速任意具备多个并行运行实例的强化学习研究（如 Atari 或 MuJoCo 物理模拟器），该技术在强化学习领域中将越来越普遍，如分布式深度 Q 学习（DQN）和分布式策略梯度（如 A3C）。

一旦我们可以在 GPU 上快速运行多个不同神经网络，在 CPU 上运行更快速的模拟器，那么挑战就变成了尽可能地保存所有计算机运行的资源。如果我们在每个神经网络中都执行了前向传播，询问每个网络在当前状态下应该采取什么动作，那么尽管每个神经网络都在计算自己的答案，但是运行游戏模拟器的 CPU 什么也没做。类似地，如果我们执行了动作，并询问模拟器「这些动作会导致什么状态？」，那么运行神经网络的 GPU 在该模拟步中就处于空闲状态。这就是多线程 CPU+GPU option（如下图所示）。尽管单线程计算出现改进，但这仍然是无效的。

更好的解决方案是具备两个及以上与模拟器配对的神经网络子集，并使 GPU 和 CPU 在更新网络或根据即将采取的步骤（神经网络或模拟）而执行的来自不同集的模拟时同时运行。该方法如下最右图「pipelined CPU+GPU」所示。使用它以及上述改进，我们可以使具备 ~4M 参数的神经网络的训练时间降低到单个计算机大约 4 小时。
![](https://pic3.zhimg.com/v2-fe1a2112bf097587476a923b6f722a8e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='227'></svg>)
在强化学习中优化异质网络集群的调度。蓝色框是模拟器，如 Atari 游戏模拟器或 MuJoCo 物理引擎，它们的 episode 长度不同。使用 GPU 的普通方式（左）性能较差，原因有二：1）GPU 的批大小无法利用其并行计算能力；2）GPU 等待 CPU 时的空闲时间，反之亦然。多线程方法（中）通过使多个 CPU 并行运行模拟器实现 GPU 的更高效使用，但是这导致 GPU 在 CPU 运行时处于空闲状态，反之亦然。Uber 提出的流水线型实现（右）允许 GPU 和 CPU 都高效运行。该方法还可以同时运行多个 GPU 和 CPU，这已经应用于实践中。




**快速、低成本实验的影响**

Uber 的代码能够帮助研究社区中的每个参与者，包括学生和自学者快速上手试验性地迭代训练深度神经网络，研究各种具有挑战性的问题，如 Atari 游戏智能体。在此之前，只有财力雄厚的行业和学界实验室可以这样做。

快速的代码可以带来更快的研究进展。例如，Uber 的新代码可以用很少的成本对遗传算法启动广泛的超参数搜索，带来了大多数 Atari 游戏模拟器的性能提升。Uber 表示会在近期更新其在 arXiv 上发表的研究《Deep Neuroevolution: Genetic Algorithms Are a Competitive Alternative for Training Deep Neural Networks for Reinforcement Learning》中的测试结果。更快的代码也加快了研究进度，通过缩短迭代时间来改进深度神经进化，使我们能够在更多的领域尝试每个新想法，并延长算法的运行时间。

Uber 的新软件中包含了深度遗传算法的实现、Salimans 等人的[进化策略算法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650724740%26idx%3D1%26sn%3De274066d69c18731b285dbf5e0a480fd%26chksm%3D871b1dfab06c94ecb356c2ddc47465b41507dd387d5daf2dafe7c9814cd23106405be442f3cf%26scene%3D21%23wechat_redirect)，以及 Uber 自己的随机搜索方法。Uber 欢迎研究社区的贡献者使用这些代码，并对这些代码进行进一步改进。例如，进行分布式 GPU 训练或加入为此类计算定制的其他 TensorFlow 操作——这可能会进一步提高速度。

深度神经进化的方向还有很多可以探索的东西，除了前面提到的 Uber 的研究、OpenAI 的工作以外，近期还有使用 [DeepMind](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650733894%26idx%3D4%26sn%3Db1f3a6a8b72425496ebc6607ff617553%26chksm%3D871b3938b06cb02e9e5324fc98d52d946f754506aefa8a8be7bfe76f29fa0543905becc3746d%26scene%3D21%23wechat_redirect)、[Google Brain](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650739385%26idx%3D2%26sn%3D592ef223e200fc91ecdc787b6f4bc0b8%26chksm%3D871ad6c7b06d5fd17434a0ae6cd801431e75b3af91b9fdfc50ae12b93ddb86cb99552b9df76f%26scene%3D21%23wechat_redirect) 和 Sentient 的进化算法带来的深度学习研究进展。Uber 希望通过通过此次开源进一步降低该领域的进入门槛。

最重要的是，这一工具可以降低研究成本，让各类背景的研究人员都能在其上尝试自己的想法，以改善深度神经进化并利用它实现自己的目标。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://eng.uber.com/accelerated-neuroevolution/](https://link.zhihu.com/?target=https%3A//eng.uber.com/accelerated-neuroevolution/)



# 深度强化学习综述：从AlphaGo背后的力量到学习资源分享 - 知乎
# 


![](https://pic3.zhimg.com/v2-54edd7c51e4e4f3e0da92e11b25cbe52_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='787' height='199'></svg>)
**摘要**


本论文将概述最近在深度强化学习（Deep Reinforcement Learning）方面喜人的进展。本文将从深度学习及强化学习的背景知识开始，包括了对实验平台的介绍。接着我们会介绍深度 Q 网络（Deep Q-Network，DQN）及其拓展、异步方法（asynchronous methods）、策略优化（policy optimization）、奖励（reward）及规划（planning）。在这之后，我会讨论注意和记忆（attention and memory）机制、非监督学习及学习去学习。随后，会讨论强化学习的各种应用，包括在游戏（特别是 AlphaGo）、机器人、口语对话系统（聊天机器人）、机器翻译、文本序列预测、神经架构设计、个性化网络服务、医疗、金融及音乐生成等方面的应用。我们会提到一些未覆盖到的主题/论文。在列举强化学习相关资源之后，我们将会以讨论结束论文。


**1 导语**


强化学习（RL）实际上是关于序列决策的一种工具，它能够解决包括科学研究、工程文理等学科的一系列问题（Sutton and Barto, 2017）。


增强学习及神经网络的结合可以追溯到 1990 年代（Tesauro, 1994; Bertsekas and Tsitsiklis, 1996; Schmidhuber, 2015）。而在最近深度学习的突破性进展之下（LeCun et al., 2015; Goodfellow et al., 2016），得益于大数据的普及、计算能力的提升及新的算法技术，我们正见证着强化学习的复兴（Krakovsky, 2016），特别是强化学习及深度学习的结合（也就是深度强化学习（deep RL））。


我们已见证了诸多突破性进展——深度 Q 网络（Mnih et al., 2015）、AlphaGo（Silver et al., 2016）及可微分神经计算机（Graves et al., 2016）。还有一些全新的架构及应用，包括异步方法（Mnih et al., 2016）、对抗网络架构（Dueling Network Architectures，Wang et al., 2016a）、价值迭代网络（value iteration networks，Tamar et al., 2016）、用于机器翻译的双学习（dual learning for machine translation，He et al., 2016a）、口语对话系统（spoken dialogue systems，Su et al., 2016b）、信息提取（information extraction，Narasimhan et al., 2016）、 引导性策略搜索（guided policy search，Levine et al., 2016a）、 生成对抗模仿学习（generative adversarial imitation learning，Ho and Ermon，2016）、非监督的强化及辅助学习（unsupervised reinforcement and auxiliary learning，Jaderberg et al., 2017）及神经架构设计（neural architecture design，Zoph and Le, 2017）等等。在这篇概述中，我们主要关注近几年的工作成果，当然也只能覆盖不完全的、一小部分成果。


我们将给读者一系列的参考资料以帮助其进一步学习：


强化学习（Sutton and Barto, 2017; Szepesvari, 2010; Bertsekas, 2012; Powell, 2011; Bertsekas and Tsitsiklis, 1996; Puterman, 2005; Littman, 2015; Kaelbling et al., 1996）

深度学习（LeCun et al., 2015; Goodfellow et al., 2016; Bengio, 2009; Deng and Dong, 2014）

机器学习（Jordan and Mitchell, 2015; Hastie et al., 2009;Bishop,2011;Murphy,2012;Jamesetal.,2013）

实用机器学习建议（Domingos，2012；Zinkevich，2017）

人工智能（Russell and Norvig, 2009）

神经网络中的深度学习（Schmidhuber，2015）

自然语言处理（Hirschberg and Manning，2015；Deng and Liu, 2017）

机器人学（Kober et al., 2013）

迁移学习（Taylor and Stone、2009；Panand Yang，2010；Weiss et al., 2016）

半监督学习（Zhu and Goldberg，2009）

贝叶斯强化学习（Ghavamzadeh et al., 2015）

口语对话系统（Hinton et al., 2012；He and Deng，2013；Young et al., 2013）

人工智能安全（Amodei et al., 2016； Garcia and Fernandez，2015）

 蒙特卡洛搜索（MCTS）（Browne et al., 2012；Gelly et al., 2012）

多代理强化学习（Shoham et al., 2003；Busoniu et al., 2008）

博弈论（Leyton-Brown and Shoham，2008）等等。


我们将会在 23 节中列举强化学习资源。在 goo.gl/KoXIQC 及 goo.gl/1Q1lzg 参见强化学习的应用。


该概述的大纲如下：第二节，深度学习及强化学习的背景知识及对测试平台的介绍；第三节，对深度 Q 网络及其拓展的介绍；第四节，异步放法的介绍；第五节，策略优化；第六节，奖励；第七节，规划；第八节，注意和记忆机制，特别是对可微分神经计算机（DNC）的介绍；第九节，非监督学习；第十节；学习去学习（learning to learn）；第十一节，游戏/博弈，包括棋类游戏、视频游戏及非完美信息博弈；第十二节，AlphaGo；第十三届，机器人学；第十四节，对话系统（聊天机器人）；第十五节，机器翻译；第十六节，文字序列预测；第十七届，神经架构设计；第十八节，个性化网络服务；第十九节，医疗；第二十节，金融；第二十一节，音乐生成；第二十二节，一个未回顾论文/话题的待办清单；第二十四节，讨论。


特别地，我们将在 23 节中列举一系列关于强化学习的资源，包括图书、在线课程、教程、会议、期刊、研讨会乃至博客等。如果非要选择唯一一个推荐的强化学习的资源，那么应该是 Sutton 教授的强化学习书（RL Book，Sutton and Barto，2017，第二版正在编辑中）。它覆盖了强化学习的基础知识，并介绍了它最新的进展，包括深度 Q 网络、AlphaGo、梯度策略方法（Policy Gradient Methods）及在心理学与神经科方面的进展。对深度学习而言，则可以选择 Goodfellow 等人的书（2016）。


**2 背景知识**


在这一节中，我们将会简要介绍在深度学习（Sutton and Barto，2017）与深度学习（Goodfellow et al., 2016）方面的基础知识与概念。


2.1 深度学习

2.2 强化学习

2.3 测试平台

- 
街机学习环境（Arcade Learning Environment，ALE，Bellemare et al., 2013）是一个由 2600 个 Atari 游戏构成的用于研发及评估 AI 的框架。

- 
DeepMind 团队则发布了它的第一人称视角 3D 游戏平台 DeepMind Lab（Beattie et al., 2016）。DeepMind 及暴雪会合作以发布星际争霸 2 的人工智能研究环境（goo.gl/Ptiwfg）。

- 
OpenAI Gym（[OpenAI Gym: A toolkit for developing and comparing reinforcement learning algorithms](https://link.zhihu.com/?target=https%3A//gym.openai.com/)）是一个用于开发强化学习算法的工具包。它由一系列环境构成，包括了 Atari 游戏及模拟的机器人构成，以及一个用于比较及复现结果的网站。

- 
OpenAI Universe（[Universe](https://link.zhihu.com/?target=https%3A//universe.openai.com/)）被用于将任一程序转换到一个 Gym 环境。Universe 已经集成了许多的环境，包括 Atari 游戏、flash 游戏、如 Mini World of Bit Sand 这样的浏览器任务。最近，侠盗猎车手 5（GTA5）也已经被加入到 Universe 中来帮助模拟自动驾驶车辆。

- 
FAIR TorchCraft（Synnaeve et al., 2016）是一个为如星际争霸这样实时战略类（RTS）游戏开发的库。

- 
ViZDoom 是一个基于《毁灭战士（Doom）》游戏的为研究视觉强化学习的研究平台。

- 
TORCS 是一个赛车比赛驾驶模拟器（Bernhard Wymann et al., 2014）。

- 
MuJoCO（Multi-Joint dynamics with Contact）是一个物理引擎，参见：[MuJoCo](https://link.zhihu.com/?target=http%3A//www.mujoco.org/)

- 
Duan et al., 2016 为连续控制任务给出了一个跑分平台，开源代码参见：[openai/rllab](https://link.zhihu.com/?target=https%3A//github.com/openai/rllab)

- 
Nogueira and Cho（2016）展示了 WebNav 挑战，来测试维基百科链接导航。



**3 深度 Q 网络（DEEP Q-NETWORK）**

![](https://pic4.zhimg.com/v2-5a8da6ec05ccde0a6b967705a7c97bb3_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='741' height='505'></svg>)
算法 1：深度 Q 网络，截取自 Mnih et al. (2015)


3.1 双重 DQN（DOUBLE DQN）

3.2 优先经验回放（PRIORITIZED EXPERIENCE REPLAY）

3.3 对抗架构（DUELING ARCHITECTURE）

3.4 更多拓展


**4 异步方法**

![](https://pic2.zhimg.com/v2-16752cdb41208080d007d0003b82ea9d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='682' height='459'></svg>)
算法 2：A3C，每个 actor-learner 线程，来自 Mnih et al. (2016)


**5 策略优化**


策略通常是随机的。然而在 2014 年，Silver et al. (2014) 引入确定性策略梯度（DPG）来有效估计策略梯度。Lillicrap et al. (2016) 用深度神经网络扩展了 DPG。同时我们介绍了几份近期成果，包括引导策略搜索（Guided Policy Search，Levine et al.,2016a）、信赖域策略优化（Trust Region Policy Optimization，Schulman et al.,2015）、基准测试结果（Duan et al., 2016）以及策略梯度与 Q 学习（O'Donoghue et al., 2017）。


5.1 确定性策略梯度

5.2 深度确定性策略梯度

5.3 引导策略搜索

5.4 信赖域策略优化

5.5 基准测试结果


Duan et al. (2016) 提出了连续控制任务的基准，包括了一些经典任务（如车柱）、具有极大状态与动作空间的任务（如 3D 人形运动）、部分观察任务、层次结构任务，并实施了许多算法，包括批处理算法：REINFORCE 算法、截断性自然策略梯度（TNPG）、奖励加权回归（RWR）、相对熵策略搜索（REPS）、信赖域策略优化（TRPO）、交叉熵方法（CEM）、自适应协方差矩阵进化策略（CMA-ES）; 也包括在线算法：深度确定性策略梯度（DDPG）；还有批处理算法的重复性变体。开源地址：[openai/rllab](https://link.zhihu.com/?target=https%3A//github.com/openai/rllab)


Duan et al.(2016) 比较了各种算法，并表明 DDPG、TRPO 和截断性自然策略梯度（TNPG）(Schulman et al., 2015) 在训练深度神经网络策略中颇有成效，但分层任务（hierarchical tasks）也还需要更好的算法。


5.6 结合策略梯度与 Q-Learning


**6 奖励**


逆向强化学习（IRL/inverse reinforcement learning）是给定观察最佳行为来确定奖励函数的问题（Ngand Russell，2000）。在激励学习或学徒制学习中，代理学习使用来自专家的轨迹样本并从其演示中执行任务，代理学习没有强化信号，在训练时也没有来自专家的额外数据；模仿学习的两种主要方法是行为克隆和逆向强化学习；行为克隆被制定为监督学习问题，将状态行动对（state-action pairs）从专家轨迹（expert trajectories）映射到策略中（Ho and Ermon，2016）。


6.1 生成对抗网络

6.2 生成对抗式模仿学习


**7 规划**


Tamar et al. (2016) 提出了价值迭代网络（VIN），即一个用于近似价值迭代算法的完全可微分的 CNN 规划模块，它可用于学习规划，例如强化学习中的策略。与传统的规划相反，VIN 是无模型的，其中的奖励和转移概率是要学习的神经网络的一部分，从而避免系统识别的问题。VIN 可以通过反向传播进行端到端训练，它也可以在一组不同的任务中泛化：VIN 可以泛化在一组不同的任务：简单的网格世界（gridworlds）、火星车导航、连续控制和用于维基百科链接导航的 WebNav Challenge（Nogueira and Cho, 2016）。价值迭代网络及决斗网络（Wang et al.，2016b）的一个优点便是它们能为强化学习问题设计新型深度神经网络架构。欲访问有关 VIN 的博客，请点击 goo.gl/Dr8gKL。


**8 注意和记忆**


注意（attention）和记忆（memory）是两个重要的机制，在许多情况下它们一起发挥作用。


Mnih et al. (2014) 引入循环注意模型（RAM/ recurrent attention model）来关注图像或视频的区域或位置的选定序列，用于图像分类和对象检测。作者使用 RL 方法特别是 REINFORCE 算法来训练模型，以克服模型不可微分的问题，并对图像分类任务和动态视觉控制问题进行实验。Xu et al. (2015) 整合了图像字幕的注意，用 REINFORCE 算法训练硬版本的注意机制，并在 Flickr8k、Flickr30k 和 MSCOCO 数据集上展示了注意的有效性。注意机制也应用到了 NLP 中，如 Bahdanau et al. (2015; 2017)，以及应用外部记忆的可微分神经计算机中（Graves et al., 2016）。


Graves et al.(2016) 提出了可微分神经计算机（DNC），其中神经网络可以从外部存储器读取与写入，使 DNC 可以解决复杂的结构化的问题，而没有读写存储器的神经网络却不能解决。DNC 将内存分配干扰最小化，并实现了长期存储。类似于常规计算机，在 DNC 中，神经网络是控制器，外部存储器是随机存取存储器；并且 DNC 用存储来表示并操纵复杂的数据结构。不同的是，DNC 使用梯度下降来学习端对端的表示和操纵，而梯度下降的数据是目标导向的。当使用有监督学习来训练时，DNC 可以解决合成问题来用于自然语言的推理；它可以解决交通网络中两个站点之间的最短路径定位问题和家庭树中的关系推理问题。当使用强化学习来训练时，DNC 可以解决一个使用被符号序列指定的变动性目标的移动块拼图。DNC 优于正常神经网络，如 LSTM 或 DNC 的前身神经图灵机（Graves et al., 2014），若碰到更困难的问题，LSTM 可能会失败。虽然这些实验是相对小规模的，我们仍期望看到 DNC 的进一步改进和应用。


欲查阅 Deepmind 对于 DNC 的描述，请点击 goo.gl/58mgoX。欲查阅注意与/或记忆的更多信息，如 Ba et al. (2014); Eslami et al. (2016); Gregor et al. (2015); Jaderberg et al. (2015); Oquab et al.(2015);Yang et al.(2015);Zagoruyko and Komodakis(2017);Zaremba and Sutskever(2015); Weston et al. (2015); Sukhbaatar et al. (2015); Ba et al. (2016); Danihelka et al. (2016); Kaiser and Bengio (2016)，请参阅 goo.gl/ArW2nE 和 goo.gl/UukROv，这是有关注意与记忆的博客。


**9 无监督学习**


Jaderberget al.(2017) 提出了无监督的强化辅助学习（UNREAL），通过共享一个共同的表征（representation），并在通常的累积奖励之外最大化伪奖励功能，从而提高学习效率。UNREAL 通过学习大量的可能训练信号而受益，特别是当外部奖励信号很少被观察到时。UNREAL 由 RNN-LSTM 基本代理，像素控制，奖励预测和值函数重放组成。基本代理（base agent）使用 A3C 进行在策略（on-policy）训练。观察、奖励和动作的经验存储于答复缓冲器（reply buffer）内，以供辅助任务使用。辅助策略使用基础 CNN、LSTM 以及解卷积网络（deconvolutional network）来使输入图像中不同区域的像素强度的变化最大化。奖励预测模块通过观察最后三个帧来预测下一帧中的短期外在奖励，以解决奖励稀疏性的问题。值函数重放则会进一步训练值函数。UNREAL 改善了 A3C 在 Atari 游戏上的表现，并在 3D Labyrinth 游戏中表现出色。欲访问Deepmind有关 UNREAL 的官方博客，请点击 goo.gl/zhqBGy。


我们将在第 13 节讨论使用类似的无监督辅助学习的机器人导航以及生成式对抗网络（GAN），并在第 6 节讨论近期的无监督学习框架。也请参阅Sutton et al.(2011) , 一个用于以无监督感觉运动学习互动来学习知识的可扩展实时架构 Horde.

**10 学习去学习（LEARNING TO LEARN）**


学习去学习与迁移学习、多任务学习或表征学习相关，是形成实现强大人工智能的核心要素之一（Lake et al., 2016）。学习去学习也与元学习（meta learning）和一次性学习（one-shot learning）有关。


Duan et al. (2017) 和 Wang et al. (2016a) 提出通过学习一个灵活的 RNN 模型来处理一系列 RL 任务，从而能够提高样本效率，能够从几个样本中学到新任务，并且可以从先验知识中获益。此代理使用 RNN 建模，并输入了观察、奖励、行动和终止标志；它使用 RL，Duan et al.（2017）提出的 TRPO 和 Wang 等（2016a）的 A3C 算法来训练 RNN 的权重，并且在使用特定 RL 算法解决的多个问题中表现相似。Duan 等在 2017 年使用多臂赌博机、表 MDP 和视觉导航进行了实验，并指出对于较大型的问题，需要更好的 RL 算法来训练 RNN。Wang et al.（2016a）对独立臂赌博机、依赖臂赌博机、持续性臂和 MDP 进行了实验。未来的工作方向之一便是提高可扩展性。


Li 和 Malik 在 2017 年建议通过将特定的优化算法表示为策略，将收敛速度表示为奖励，以引导策略搜索（Levine et al.，2016a）来使无约束连续性优化算法自动化。


**11 Games（博弈/游戏）**


游戏为强化学习/人工智能算法提供了非常好的测试平台。我们在第 3 节讨论了深度 Q 网络（DQN）及其延展，所有这些都在 Atari 游戏上做了测试。我们在第 4 节讨论了 Mnih et al. (2016)，在第 9 节讨论了 Jaderberg et al. (2017)，在第 13 节讨论了 Mirowski et al. (2017)——他们使用了 Labyrinth 作为测试平台。


西洋双陆棋和围棋是完美信息博弈（perfect information games）。我们在 11.1 讨论了西洋双陆棋这样的棋盘游戏。在 11.2 讨论了 Doom 这样的视频游戏。我们将扑克游戏放到了 11.3，讨论了非完美信息博弈（imperfect information games），其中涉及到了博弈论（game theory）。Labyrinth 和 Doom 等视频游戏通常是非完美博弈，但是目前还没有使用博弈论来解决这些问题。


我们将 AlphaGo(Silver et al., 2016) 单独成了第 12 节，因为其有很大的重要性。


11.1 棋盘游戏

11.2 视频游戏

11.3 非完美信息博弈


**12 AlphaGo**


AlphaGo (Silver et al., 2016) 是一个计算机围棋程序，其在 2015 年 10 月份以 5 局全胜击败了欧洲围棋冠军，成为了第一个在全尺寸 19×19 棋盘上无让子地击败了人类职业棋手的计算机围棋程序。不久之后，2016 年 3 月份，AlphaGo 以 4:1 的成绩击败了曾获 18 次世界冠军的围棋手李世石，引起了世界的广泛关注。这是人工智能发展的一个里程碑。围棋问题的困难之处不仅在于其超大的搜索空间（search space）——250^150，一个天文数字；而且也是因为其局面评估（position evaluation）的难度非常大，而西洋双陆棋和国际象棋等游戏已经通过局面评估得到了解决。


12.1 训练流程和蒙特卡洛树搜索（MCTS）


我们在 Silver et al. (2016) 与 Sutton and Barto (2017) 的基础上简要讨论了 AlphaGo 的工作方式。参见 Sutton and Barto (2017) 中第 16 章可了解 AlphaGo 的详细和直观描述。DeepMind 对 AlphaGo 的描述可查阅：goo.gl/lZoQ1d


AlphaGo 是使用深度 CNN 技术、监督学习、强化学习和蒙特卡洛树搜索（MCTS）(Browne et al., 2012; Gelly et al., 2012) 打造的。AlphaGo 的工作分成了两个阶段：神经网络训练流程和 MCTS。其训练流程阶段包括根据专家的走子训练一个监督学习策略网络、快速部署策略、强化学习策略网络和强化学习价值网络。


**13-21：强化学习的应用介绍**


这几节介绍了强化学习的不同类型的应用，这里简单给出目录，详情请查阅原论文。


13 机器人学

14 口语对话系统

15 机器翻译

16 文本序列预测

17 神经架构设计

18 个性化网络服务

19 医疗保健

20 金融

21 音乐生成


**22 未来工作**


下面我们列出了上面的概述中没有讨论到的有趣的和/或重要的研究方向/论文，希望能够为有兴趣进一步研究它们的人提供信息入口。这也将是我们未来工作的一部分。


• 理解机器学习（understanding deep learning）, Daniely et al. (2016); Li et al. (2016b); Zhang et al. (2017) 

• 探索（exploration）如：Stadie et al. (2015); Bellemare et al. (2016); Kulkarni et al. (2016); Osband et al. (2016); Nachum et al. (2017) 

• 基于模型的学习（model-based learning）如：Oh et al. (2015); Gu et al. (2016b) 

• 回溯算法（retrace algorithm）, Munos et al. (2016)

 • 预测（predictron）, Silver et al. (2017) 

• 分层强化学习（hierarchical RL）如：Kulkarni et al. (2016); Vezhnevets et al. (2016); Tessler et al. (2017); Florensa et al. (2017) 

• 迁移/多任务强化学习（transfer/multitask RL）如： Maurer et al. (2016); Mo et al. (2016); Parisotto et al. (2016), NIPS 2015 Transfer and Multi-Task Learning: Trends and New Perspectives Workshop 

• 零次/一次性学习（zero/one-shot learning）如：Vinyals et al. (2016); Lake et al. (2015); Johnson et al. (2016) 

• 半监督强化学习（semi-supervised RL）如：Finn et al. (2017) 

• deep symbolic RL, Garnelo et al. (2016) 

•内在动机（intrinsic motivation）如：Stadie et al. (2015); Kulkarni et al. (2016); Oudeyer et al. (2016) 

• 超参数学习（hyperparameter learning）如：Andrychowicz et al. (2016) 

• 信息提取（information extraction）如：Narasimhan et al. (2016) 

• 文本博弈（text games）如：He et al. (2016b); Narasimhan et al. (2015) 

• 语言树结构学习（language tree-structure learning）如：Yogatama et al. (2017) 

• 问答系统（question answering）如：Shen et al. (2016); Trischler et al. (2016) 

• 大型动作空间（large action space）如：Dulac-Arnold et al. (2016); He et al. (2016c) 

• 适应性规范化（adaptive normalization）, van Hasselt et al. (2016b) 

• 自动驾驶载具（self-driving vehicle）如：Bojarskietal.(2016),NIPS 2016 Workshop on Machine Learning for Intelligent Transportation Systems 

• 智能电网（smart grid）如： Wen et al. (2015b)

• 通信网络（communication networks）如： Mestres et al. (2016) 

• 物理实验（physics experiments）如： Denil et al. (2016) 

• 深度概率编程（deep probabilistic programming）, Tran et al. (2017) 

• 深度博弈学习（deep learning games）, Schuurmans and Zinkevich (2016) 

• 程序学习（program learning）如：Reed and de Freitas (2016) 

• 量子强化学习（quantum RL）如：Crawford et al. (2016), NIPS 2015 Workshop on Quantum Machine Learning


**23 资源**


我们列出了一些用于深度强化学习的资源，当然并不能做到完全。


23.1 书籍


• Richard S. Sutton 和 Andrew G. Barto 所著的毫无疑问的和直观的强化学习书 (Sutton and Barto, 2017) 

• 简明和理论性的《Algorithms for Reinforcement Learning》，作者：Csaba Szepesv´ari (Szepesv´ari, 2010) 

• 一本关于近似动态编程的理论书籍，作者：Dimitri P.Bertsekas(Bertsekas, 2012) 

• 一本面向运筹学的书《Approximate Dynamic Programming》，作者：WarrenB. Powell (Powell, 2011) 

•《Deep Learning》，作者：IanGoodfellow, Yoshua Bengio 和 Aaron Courville (Goodfellow et al., 2016)

23.2 课程

• David Silver, 强化学习（Reinforcement Learning）, 2015, 幻灯片：goo.gl/UqaxlO，视频：goo.gl/7BVRkT

• Sergey Levine, John Schulman and Chelsea Finn, CS 294: 深度强化学习（Deep Reinforcement Learning）2017 年春季课程, [CS 294 Deep Reinforcement Learning, Spring 2017](https://link.zhihu.com/?target=http%3A//rll.berkeley.edu/deeprlcourse/)

• Charles Isbell, Michael Littman and Pushkar Kolhe, Udacity: 机器学习：强化学习（Machine Learning: Reinforcement Learning）, goo.gl/eyvLfg

• 李飞飞、 Andrej Karpathy 和 Justin Johnson, CS231n: 用于视觉识别的卷积神经网络（Convolutional Neural Networks for Visual Recognition）, [CS231n: Convolutional Neural Networks for Visual Recognition](https://link.zhihu.com/?target=http%3A//cs231n.stanford.edu)

• Richard Socher, CS224d: 用于自然语言处理的深度学习（Deep Learning for Natural Language Processing）, [CS224d: Deep Learning for Natural Language Processing](https://link.zhihu.com/?target=http%3A//cs224d.stanford.edu)

• Nando de Freitas, 深度学习课程（Deep Learning Lectures）, [https://www.youtube.com/user/ProfNandoDF](https://link.zhihu.com/?target=https%3A//www.youtube.com/user/ProfNandoDF)


23.3 教程 


• David Silver, 深度强化学习（Deep Reinforcement Learning）, ICML 2016 

• Pieter Abbeel 和 John Schulman, 通过策略优化的深度强化学习（Deep Reinforcement Learning Through Policy Optimization）, NIPS 2016 

• 吴恩达，使用深度学习开发人工智能应用的基本要点（Nuts and Bolts of Building Applications using Deep Learning）, NIPS 2016 

• John Schulman，深度强化学习研究的基本要点（The Nuts and Bolts of Deep Reinforcement Learning Research），深度强化学习研讨会, NIPS 2016 

• John Schulman, 深度强化学习（Deep Reinforcement Learning）, Deep Learning School, 2016 

• Pieter Abbeel, Deep Reinforcement Learning, Deep Learning Summer School, 2016; [Deep Learning Summer School, Montreal 2016](https://link.zhihu.com/?target=http%3A//videolectures.net/deeplearning2016) abbeel deep reinforcement/

• David Silver, Deep Reinforcement Learning, 第二届强化学习与决策多学科会议（RLDM）, Edmonton 2015; [2nd Multidisciplinary Conference on Reinforcement Learning and Decision Making (RLDM), Edmonton 2015](https://link.zhihu.com/?target=http%3A//videolectures.net/rldm2015) silver reinforcement learning/ 

• Rich Sutton, Introduction to Reinforcement Learning with Function Approximation, [https://www.microsoft.com/en-us/research/video/tutorial-introduction-to-reinforcementlearning-with-function-approximation/](https://link.zhihu.com/?target=https%3A//www.microsoft.com/en-us/research/video/tutorial-introduction-to-reinforcementlearning-with-function-approximation/)

• Joelle Pineau, 强化学习入门（Introduction to Reinforcement Learning）, Deep Learning Summer School, 2016; [Deep Learning Summer School, Montreal 2016](https://link.zhihu.com/?target=http%3A//videolectures.net/deeplearning2016) pineau reinforcement learning/ 

• Deep Learning Summer School, 2016, 2015


23.4 会议、期刊和研讨会


• NIPS: 神经信息处理系统

• ICML: 国际机器学习大会

• ICLR: 国际学习表征大会

• RLDM: 强化学习与决策多学科会议

• AAAI, IJCAI, ACL, EMNLP, SIGDIAL, ICRA, IROS, KDD, SIGIR, CVPR, 等

• Science Robotics, JMLR, MLJ, AIJ, JAIR, PAMI, 等

• Nature May 2015, Science July 2015, 搜索关于机器学习/人工智能的论文

• Deep Reinforcement Learning Workshop, NIPS 2016, 2015; IJCAI 2016 

• Deep Learning Workshop, ICML 2016 


23.5 博客


• Andrej Karpathy, karpathy.github.io, esp. goo.gl/1hkKrb 

• Denny Britz, [WildML](https://link.zhihu.com/?target=http%3A//www.wildml.com), 尤其是 goo.gl/MyrwDC 

• Junling Hu, Reinforcement learning explained -learning to act based on long-term payoffs 

• 邓力，深度强化学习可以如何帮助打造聊天机器人（How deep reinforcement learning can help chatbots）

• Christopher Olah, [http://colah.github.io](https://link.zhihu.com/?target=http%3A//colah.github.io)


在这个信息/社交网络时代，信息已经超过了我们的极限，比如来自 Twitter、Google+、微信、arXiv 等的信息。有效地筛选最佳信息的技巧变得十分关键。


**24 讨论**


这是深度强化学习最好的时代，也是深度强化学习最坏的时代，而原因却是一样的：它以惊人的速度在发展。我们已经看到了突破、激动人心的新方法和应用，并且还有望看到更多和见证更快的发展。因此，不管是在深度还是在广度上，这篇概述都是不完整的。但是，我们也尽量总结这一惊人领域的重要成就并讨论其潜在的方向和应用。


深度强化学习这一领域的进步是有目共睹的，在不到两年时间内，我们就看到 Nature 上发表了三篇使用了深度强化学习的论文：深度 Q 网络（deep Q-network）(Mnih et al., 2015)、AlphaGo (Silver et al., 2016) 和可微分神经计算机 (Graves et al., 2016)；我们也已经见证了许多深度 Q 网络上的扩展、改进和应用。注意和记忆机制（Graves et al., 2016）也得到了很大的关注。


2016 年，使用了深度强化学习的全新架构和应用在许多顶级会议上被评选为最佳（学生）论文：ICML 上的决斗网络（dueling network）架构（Wang et al., 2016a）、ACL 上的口语对话系统（Su et al., 2016b）（学生论文）、EMNLP 上的信息提取（Narasimhan et al., 2016）、以及 NIPS 上的价值迭代网络（value iteration networks）(Tamar et al., 2016)。激动人心的成就比比皆是：异步方法（Mnihetal.,2016）、用于机器翻译的双学习（dual learning）（Heetal., 2016a）、有引导的策略搜索（Levine et al., 2016a）、生成对抗式模仿学习（Hoand Ermon, 2016）、无监督强化和辅助学习（Jaderberg et al., 2017）、神经架构设计（Zoph and Le, 2017）等等。


价值函数是强化学习的核心，比如在深度 Q 网络及其许多扩展中。策略优化方法已经在许多不同的应用领域得到了关注，比如：机器人、神经架构设计、口语对话系统、机器翻译、注意（attention）和学习去学习（learning to learn）等等，不能胜举。新的学习机制也在涌现，比如：使用无监督/半监督/迁移学习来提升学习的质量和速度，而且更多的新机制还将涌现。这是强化学习的复兴（Krakovsky, 2016）。事实上，即使是在「人工智能的冬天」，强化学习和深度学习也在不断发展进步。


考虑学习模型的问题是非常关键的，这些问题包括稳定性、收敛性、准确度、数据效率、可扩展性、速度、简洁性、可解释性、稳健性和安全性等。调查评论/批评也是很重要的，这些批评可能来自认知科学领域，涉及到直观物理学、直观心理学、因果模型、组合性、学习去学习、实时运行（Lake et al., 2016）等问题；这能够帮助我们打造出更强大的人工智能。也请参考 Peter Norvig 的观点 goo.gl/obvmVB. 

在这第三波人工智能的大潮下，深度学习将会有更为深度的影响，正如我们已经见证的许多成就一样。强化学习作为一种更为通用的学习和决策范式，将会给深度学习、机器学习和广义上的人工智能带来深远的影响。这里提一件有趣的故事，当 Rich Sutton 教授 2003 年在阿尔伯塔大学开始工作时，他将他的实验室命名为了 RLAI：Reinforcement Learning and Artiﬁcial Intelligence（强化学习与人工智能实验室）。


**致谢**


感谢来自Baochun Bai, 胡峻玲（Junling Hu）, Ruitong Huang, Lihong Li, Dale Schuurmans, David Silver, Rich Sutton, Csaba Szepesvari, Yi Wan 和 Qing Yu的建议。任何剩余错误为本文作者所出。本文同时受益于各类研讨会/网上讨论，特别是2016年4月在MIT举行的AlphaGo研讨会，以及2016年10月份『机器之心』北美系列巡游活动中在多伦多大学、McGill大学和阿尔伯塔大学举办的深度（强化）学习研讨会。另外也要感谢2016年11月关于David Silver幻灯片的网上研讨会，以及几个微信群组中的讨论。

发布于[arXiv](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1701.07274v2.pdf)**  机器之心**



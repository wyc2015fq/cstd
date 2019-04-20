# DeepMind重大突破：AI进化出大脑级导航能力，像动物一样会“抄小路” - 人工智能学家 - CSDN博客
2018年05月11日 08:08:32[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：506
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmgglYmbbHVk32PhQbDdOzHYuwjUBsa8gBEWm9hWpmfr28G1xSYFQhQAWKfshmia7m0JIpY3ufYcGeQ/640?)
作者：刘伟
摘要：DeepMind团队称，其最新研发出的一个人工智能程序具有类似哺乳动物一样的寻路能力。
美国东部时间 5 月 9 日，一手打造 AlphaGo的DeepMind 团队又在世界顶级学术杂志《自然》上发表了一项重磅成果。在一篇题为 Vector-based navigation using grid-like representations in artificial agents 的论文中，DeepMind 团队称，其其最新研发出的一个人工智能程序具有类似哺乳动物一样的寻路能力，非常类似大脑中网格细胞的工作原理。
继在围棋棋盘上轻松战胜人类后，AI 似乎在对空间的认知和巡航能力上也要将人类“踩在脚下了。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmgglYmbbHVk32PhQbDdOzHYMy8yL7oMNy6XicXcDolGWibXUTVyqgEXzzibzRpaEPNpZFbQu4Jws3HUQ/640?)
人类在自然空间中游刃有余，可以轻松绕过障碍，找到自己与目的地间的捷径，但其背后的机理却不甚明了。
为了解开这个谜团，科学家们进行了孜孜不倦地探索。早在上个世纪六十年代末， UCL 神经生理学家 John O’Keefe 就开始研究这个问题，并在 1971 年发现位置细胞（Place Cell），大脑定位系统的第一个元件。位置细胞能在主体到达特定地点时放电，从而赋予对过往地点的记忆，但位置细胞并不是坐标，也不具备几何计算能力。
2005 年，May-Britt  Moser 和 Edvard  Moser 夫妇发现了大脑内嗅皮层更为神奇的网格细胞。他们记录大鼠运动时特定神经细胞被相应激活的规律，发现网格细胞能将整个空间环境划分成蜂窝状的六边形网格，就像地图上的坐标系一样。网格细胞的行为是如此令人震撼，2014 年，Moser 夫妇也因此分享了诺贝尔生理学奖。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmgglYmbbHVk32PhQbDdOzHYAWQia70F8ibQ284tFQYRMuVWYoh5Gxrbk10qyyoUZJF5x9VbYk9csvNQ/640?)
不过，网格细胞仅仅是在空间环境中提供 GPS 定位服务吗？一直以来，人们猜测网格细胞支撑着生物的矢量巡航，即计算通向目标的距离和方向，但人们在发现网格细胞十余年后，对其计算功能以及和矢量巡航的关系，仍然未知。
这正是 Deepmind 这篇论文试图解开的谜团。
DeepMind 团队决定用人工神经网络检验上述猜想。人工神经网络是一种利用多层处理模拟大脑神经网络的运算结构。在这项工作中，研究人员首先训练循环神经网络基于运动速度信息在虚拟环境中定位。这与哺乳动物在不熟悉环境中运动定位所用到的信息非常类似。
令人震惊的是，类似网格细胞的模式，研究人员称之为网格单元，在神经网络中自然出现了。在此前的训练中，研究人员并未刻意引导神经网络产生此种结构。人工智能的定位方案，竟然与大自然亿万年进化所得到的答案如此一致。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmgglYmbbHVk32PhQbDdOzHYtsXSHSP4sUlia3oqENwg7VWQj4IYdrZiaT5wSPXPl39vVy6RKtibGhGLw/640?)
DeepMind 团队随后利用强化学习检验这种网格结构是否能够进行矢量导航。强化学习被普遍用于训练游戏 AI ，人类告诉 AI 一种游戏的得分奖惩机制，但却不教授游戏方法，由AI在反复进行游戏、努力争取更高分的过程中自我进化。后期的 AlphaGo 就完全摈弃了人类棋谱经验，在纯粹的自我对弈中从零进化到更强版本。
研究人员将之前自动出现的网格结构与一个更大型的神经网络架构结合成了人工智能体，置于虚拟现实的游戏环境中。经历强化学习后，该人工智能在游戏迷宫中向目的地前进的导航能力超越了一般人，达到了职业游戏玩家水平。它能像哺乳动物一样寻找新路线和抄近路。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmgglYmbbHVk32PhQbDdOzHYnia83D9lbp9JsowvXYeTxN07qJ5OUwT3huR2Jl6PrGmfOaJkub1Udjw/640?)
而如果将网格单元静音后，其巡航能力则大打折扣，距离和方向的计算误差都增大，证明网格模式对矢量巡航的重要性。
对于这项发现，网格细胞发现者 Edvard Moser 表示：“这篇论文横空出世，非常令人兴奋。更令人吃惊的是，从另一个角度来看，计算机模式的发展最终回归到了我们从生物学中发现的网格模式。
Deep Mind 创始人和 CEO ，论文共同作者哈萨比斯称：“我们相信人工智能和神经科学是相互启发的。这项工作就是很好的证明：通过研发出一个能在复杂环境中导航的人工智能体，我们对网格细胞在哺乳动物导航中的重要性有了更深的理解。”
DeepMind 团队相信，这一工作既是理解网格细胞计算功能的重要一步，也是人工智能发展的重要一步，显示类脑机器学习构架的重要性。而同样的方法，也可以用于研究生命的其他感知能力。
英国帝国理工学院刘芳德博士称，位置细胞和网格细胞的研究对人工智能特别是机器人系统启发很大。位置细胞其实是空间索引的数据库，描述的是拓扑空间；而网格细胞是几何计算器，描术的是欧氏空间。这种组织跟我们目前计算机科学中的技术完全不同，并有非常强的优势。
杜克大学陈怡然教授和博士生吴春鹏介绍，论文中提到的两个细节值得注意。**第一，如果神经网络的损失函数中不包括正则项，那么神经网络无法表现出网格细胞功能。这一发现给了我们一个全新的角度去思考正则项的作用。第二，论文指出深度神经网络的“黑盒”特点阻碍了进一步分析网格细胞活动特性对路径整合的作用。**这一点再次印证了当前研究神经网络可解释性的必要。
对此，约翰霍普金斯大学神经学家 Francesco Savelli 和 James Knierim 在同期 Nature 发表的题为 AI mimics brain codes for navigation 的新闻评述中强调：“网格模型增强了目标导向能力，这一事实证明了网格细胞在大脑中的作用。但是在模型中无法直接控制这些计算，使得我们很难对其背后的计算原理、算法和编码策略进行研究。如何让深度学习系统更加智能以理解人类的推理逻辑，是未来我们面临的一个令人兴奋的挑战。”
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBULg0PyXjjVDR3OiaCudIgwDjRdBUkpx1Dw58Xa9VkyJUuH0piaT7Qyem2gHwfTj6ic45jwaL7y3Zdaw/640?wx_fmt=png)
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)

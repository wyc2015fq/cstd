# Deepmind "预测地图"论文背后：神经科学或将助力AI迎来新突破 - 人工智能学家 - CSDN博客
2017年10月04日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：175

![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmhDuiaRicICEY38y36kk0YhlqE5N3AbabicmNmclnSI5p7mVvIMLkgnGPB1VIuAsNVexvPgyvydDhZ0g/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
*来源：AI科技评论*
*概要：这篇论文中，Deepmind通过对主管人类长期记忆行为的“海马体”（hippocampus）神经元活动的研究，进一步提出了可以转化为神经网络架构的“预测图”理论。*
对人类神经网络的理解越来越在左右人工智能的未来研究，连Deepmind也不例外。
2017年10月2日，《NATURE NEUROSCIENCE》发表了Deepmind的一篇《The hippocampus as a predictive map》的论文。这篇论文中，Deepmind通过对主管人类长期记忆行为的“海马体”（hippocampus）神经元活动的研究，进一步提出了可以转化为神经网络架构的“预测图”理论。
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmhDuiaRicICEY38y36kk0YhlqS4iawKw7zPN6eqVibHTePQJu8HXB7j1h1yfcxpbo4x1r1XlKlrMJI3Vw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
**在博客中，Deepmind这样写到：**
“传统观点认为海马体只表示动物的现状，尤其在执行走迷宫之类的空间任务中。这种观点在发现啮齿动物的海马体中的“位置细胞”后被广泛传播，当动物处于特定位置时，它们会选择性地放电。虽然这个理论解释了许多神经生理学发现，但并不完全解释为什么海马体也参与其他功能，包括记忆，关系推理和决策等。
我们认为，海马体用它们预测到的未来状态来展示代表每一种情况。例如，如果你要下班回家（你当前的状态），你的海马体可能会预测你很可能很快会通勤回家，到学校接孩子，或者更长远一点——到家了。海马体预测这些后期状态来表现出当前的状态，从而传达了对未来事件的摘要表达，也就是我们正式说的“后续表征”（Sucessor Representation）。我们认为，这种具体形式的预测图可以让大脑在奖励不断变化的环境中快速适应，而无需运行代价昂贵的未来模拟。
这一理论启发我们在新算法中结合了基于模型的算法的灵活性和无模型算法中的高效性，由于计算只是简单的加权相加，因此该算法的计算效率与无模型算法相当，同时，通过分离奖励期望与期望状态（预测地图），该算法可以通过简单的更新奖励期望值并保持状态期望值不变，从而快速适应奖励变化。”
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmhDuiaRicICEY38y36kk0YhlqE5N3AbabicmNmclnSI5p7mVvIMLkgnGPB1VIuAsNVexvPgyvydDhZ0g/640?wx_fmt=png)
*（老鼠在探索方形房间时记录下来的海马体每个位置细胞的活动及放电率的变化）*
这并不是Deepmind的第一篇神经科学用于人工智能的论文，在此之前Deepmind就表过至少两篇创始人Hassabis为作者的神经科学论文（参见AI科技评论之前文章：《[DeepMind眼中的神经科学研究：人工智能进步的另一个重要支点](http://mp.weixin.qq.com/s?__biz=MzI5NTIxNTg0OA==&mid=2247486395&idx=3&sn=9a8f086876ad13b1bbe52d7d11bd5f5a&chksm=ec57bc3cdb20352a72ff6f13e5ffa9efc11063092e0c1e526b59dd724fbce47d92880c6bc498&scene=21#wechat_redirect)》），这篇文章作者中虽然不包括Hassabis，但相对于前两篇论文更加具体，对未来的研究也更具有指导意义。
### **神经网络发展史**
人工智能的发展与神经系统有着千丝万缕的联系。在1943年，Warren McCulloch和Walter Pitts的《 神经活动内在想法的逻辑演算 》第一次提出了如何让人造神经元网络实现逻辑功能，这也奠定了神经网络最早的数学基础和开启了人们模拟人脑神经系统开发人工智能（虽然当时还没有这个词）的早期尝试。
模仿人类神经系统的问题是：人类大脑神经系统实在太复杂了。在人类大脑神经系统中包含10^11（10的11次方）的神经元，即便现在也难以模拟。在1958年，Frank Rosenblatt通过解剖大鼠的大脑发布了一个模仿神经元的感知机，当时有研究者认为，只要神经元足够多、网络连接足够复杂，感知机就至少可以模拟部分人类智力，但在1969年，Marvin Minsky和Seymour Papert证明了感知机只能解决线性问题，还揭露了关于多层感知器的缺陷，这使得人工神经网络研究在人工智能领域很快就陷入了低谷，被基于规则和逻辑推理的专家系统所取代。
1974年，一位叫Geffory Hinton的年轻研究者发现，把多个感知机连接成一个分层的网络，即可以解决Minsky的问题。这带动了80年代连接主义的兴起，这个阶段的连接理论最初被称为分布式并行处理PDP (parallel distributed processing)，他们以人工神经网络为工具方法，引领着神经科学和人工智能领域的发展，与行为学派和符号学派三足鼎立。
随之而来的问题是，如果要模拟人类大脑数量如此庞大的神经元，可能需要对几百甚至上千个参数进行调节，如何对这样复杂的网络进行训练呢？Hinton等人的发现是，通过反向传播算法可以解决多层网络的训练问题。这一算法是用来训练人工神经网络的常见方法，该方法计算对网络中所有权重计算损失函数的梯度。这个梯度会反馈给最优化方法，用来更新权值以最小化损失函数。
神经网络的现代分类器的各个方面也受到了我们所了解的大脑的视觉系统的启发。这方面研究的先驱包括Geoffrey Hinton、Yann LeCun、Yoshua Bengio等人，1998年，Yann LeCun提出了卷积神经网络（CNN），2006年，Hinton提出了深度学习的概念并在其经典论文《A fast learning alforithm for deep belief nets》中提出了深度信念网络，带来了深度学习直到今天的黄金时代；2009年，Yoshua Bengio提出了深度学习的常用模型堆叠自动编码器（Stacked Auto-Encoder,SAE），基于神经网络的研究得到进一步发扬光大。
然而人工智能专家基于神经系统的研究和思考并没有停止。如脑科学的研究结果表明，大脑中神经元的信号处理机制和连接学习方式是不同的，尤其是大脑的无意识感知和自我意识功能并不需要有监督的学习，在大脑神经系统中，非监督学习和自主学习同样占有重要的地位，相应地，对于无监督学习和迁移学习的研究也成为人工智能的热点之一。
而随着深度学习和神经科学的发展，之前对深度学习的一些基础理论也在面临着更新。例如在深度学习领域，神经元是最底层的单元；而在神经科学领域，神经元并不是最底层的单位。目前的深度神经网络主要是三种结构，即 DNN（全连接的）、CNN（卷积）和 RNN（循环），而目前的研究表明，人类神经网络可能比较类似上述三种结构的组合，层内更像DNN， 层间和CNN 很类似，在时间上展开就是RNN。这种结构上的差异也导致了深度学习研究的复杂性。
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmhDuiaRicICEY38y36kk0YhlqNrxvFvX9o9dbA7rozPiaK3UMqso0dpvtKk8k5VPOZqx6f4lQHuuNNcA/640?wx_fmt=png)
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUqRmu2Hic9I8jvR0ruZm6yyF8CdVx8hJQOkic2OGk5LaNJg3nmBKaLxCOLIlkzDzAKd3UQToUBgxhw/0?wx_fmt=png)

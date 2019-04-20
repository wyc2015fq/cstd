# 综述AI未来：神经科学启发的类脑计算 - 人工智能学家 - CSDN博客
2018年04月24日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：111
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUu630ubzgIkwV4GC0ggAu3RkQjPboQiaj9Wsmsg6JdOyXEK353lJzf5iaYibGN60sWvWMnic2Y5mCbnA/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
来源：运筹OR帷幄  作者：周岩
摘要：脑科学是一门实践性很强的交叉学科，包含了神经科学，医学，数学，计算科学等多门学科。
随着近几年深度学习的迅速发展，人工智能成为了无论学术界还是互联网领域的一个重要的研究热点。然而，人类在认识世界和改造世界的过程中从自然界和生物特征汲取了大量的灵感和经验。追根溯源，人工智能的发展离不开脑科学的研究。历史上，神经科学和人工智能两个领域一直存在交叉，对生物脑更好的理解，将对智能机器的建造上起到及其重要的作用。
人工智能是模拟脑的一项主要应用，现在深度学习这种生物学简化的模型有它的优点，具有很好的数学解释性，可以在现有的计算机架构（冯诺依曼）上实现，但是同样有瓶颈，例如：计算代价高，不利于硬件实现等。尽管近年来深度学习和大数据的出现使得这种模型在一些任务上超越人类，但是对于人脑可以处理的复杂问题却无能为力，同时需要大量的计算资源和数据资源作为支撑。
相反人类大脑是一个极度优化的系统，它的工作耗能仅为25瓦特，神经元的数量却在10的11次方的数量级上，并且这其中的突触也达到了每个神经元有10000个。这样庞大的网络却有如此低的能耗，这是使得人类大脑在复杂问题的处理有绝对优势。
**1. 类脑计算与神经网络的发展过程**
早期的类脑计算（Brian-like Computing）也可以狭义的称为神经计算（Neural Computation），将神经元和突触模型作为基础，把这些模型用在许多现实中的识别任务，从而发挥模拟人脑功能，例如字体识别，人脸识别等等。这就不得不提到人工智能的基础理论——机器学习。
结合机器学习和神经元模型，最早的神经元网络是由美国计算机科学家罗森布拉特（F.Roseblatt）于1957年提出的感知机（perceptron），也被称为第一代神经网络。
前两代神经网络都是在以数据科学为背景发展而来，将神经元的输入和输出抽象成向量和矩阵，神经元的功能主要是做矩阵的相乘运算。但是，实际上生物神经元对信息的处理不是由模拟数字组成矩阵，而是以脉冲形式出现的生物电信号，所以前两代神经网络仅保留了神经网络结构，而极大简化了网络中的神经元模型。
类脑计算的研究基础主要是以脉冲神经元模型为基础的神经网络。**脉冲神经网络（Spiking Neural Network，SNN）**由W.Maass在1997年首次提出，其底层用脉冲函数模仿生物点信号作为神经元之间的信息传递方式，可以算做**第三代神经网络[2]。**
SNN的优点是具有更多的生物解释性，一方面可以作为计算神经学对生物脑现象模拟的基础工具；另一方面，由于其信息用脉冲传递的特点，SNN结构更容易在硬件上实现，如FPGA等片上系统（on-chip system）。但是，脉冲函数不可导，因此SNN不能直接应用梯度法进行训练，对SNN的学习算法一直是近年来主要的研究问题。
**2. SNN的模型、结构以及在机器学习中的应用**
SNN主要结构有**前馈（Feedforward）**和**循环链接（Recurrent）**两种。前馈比较常见，从结构上说与传统的前馈神经网络相似，不同的是单个的神经元模型以及整个网络的数据处理方式。
SNN的神经元模型总体上来说是一类以微分方程构成的模型，带有时间属性。可以理解为传统的神经元只是当前时刻的输入与权重的加权和，SNN的神经元则是在一定宽度的时间窗内的输入与权重的加权和。其中最常见的神经元模型是L&F(Leak and Fire)模型，也可以把这种神经元看做为一个店带有电容的电路，按照电路原理来定义可以有如下形式：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7fgf7BnGdQPyEf6nwickEC7tjmEwH0NgEqDXC4bvN00TuLzHFWrasiaCeg/640?wx_fmt=jpeg)
图1. I&F神经元的模拟电路[3]
具体模型公式为：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7foq4uwvffovKM02vQJWggsIGeo8rPUu2m7drx09MHBv69wuKF2TmdHA/640?wx_fmt=png)
其中 ![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7ficfCSJ1YNtBbOEwDLD0aq4pxic56MduuAeKXyqfNWnNItHoyiaVImA0nQ/640?wx_fmt=png) 表示膜电位， ![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7frlDmRibCRkQ5xVnEvcKUbZgoBg0mjJHgunUiaE36JTm10Zxd0fbOTfww/640?wx_fmt=png) 表示输入突触电流，每当膜电位达到一个阈值 ![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7ftTibAk3k2fcayIRo1N9mdqGHwjRdZuPySYjTEsnxswsU4iaZK0WwCYLw/640?wx_fmt=png) ，便会产生一个脉冲，并将膜电位恢复到复位电位 ![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7fWfgoOfpicsaW1692X5Z7ljf3InS7YUf66fKOTiahvDSib3IusCk3oiaNlQ/640?wx_fmt=png) ，同时在一段时间内会产生一个不应期（refractory），在此期间，模电位保持在 ![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7flwoVUbIp09s8CU3HnuxAUjRYibib7bZGRAZBNrqheH7TLWtSWqoPQpVw/640?wx_fmt=png) ，不对输入的突触电流做出响应。
L&F是一阶微分方程的形式，因此是一种线性响应模型，但是由于脉冲阈值以及不应期等机制的存在，使得L&F同样变成了一种对输入的非线性映射。
利用L&F构成网络就涉及到如何在神经元之间传递脉冲信号，这就是突触电流的响应模型。常用的电流模型是一种基于指数衰减的模型（与膜电位的模型很相似），改进的模型可以是两个衰减模型的叠加，来模拟平滑的脉冲信号（代替阶跃函数表示的脉冲）：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7fHesRu47RnBvCV24qsnDibGoOa33cmCiaatCvmhYk3EY04hV4MvpRWKWg/640?wx_fmt=png)
其中 ![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7fKzgkjQbwt6x605z8icaokv9Qpx5DTQIDg5PSrcg4icMFicXmwe7GD6ddQ/640?wx_fmt=png) 和![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7fZNqb5BKoPgYPNysWpW7FeRKVn4j7fiaGmzWk7c5uhBlfiapGlQficmSow/640?wx_fmt=png)是时间常数， ![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7fVe1ABX97AoOBcj8vb9xooJVm5NuXNc0ZTQ6J9Xvicm8tOFHYCM5hnrw/640?wx_fmt=png) 是表示一个脉冲的单位阶跃函数。
由于SNN存在大量的微分方程计算并依赖于计算机仿真，SNN中的数值基本是通过数值方法计算的微分方程的状态。因此，具体的仿真过程中，与传统神经网络的最大区别在于时间步的概念，与硬件中的时钟十分相似。所以突触模型中还可以带有一个熟悉就是延迟（delay），可以和权重一样被训练成为表达输入特征的一个重要参数。目前SNN的学习算法很多基于脉冲的间隔的精确时间而不是一段时间的脉冲的数量来调整网络参数，如根据hebbian规则演变而来的STDP(Spike time dependance plasicity)。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7fddKXbNlA8nKjfn7S61DSsC03muv1WnqvGwhd91SCzJmlqmN9y8SC3Q/640?wx_fmt=png)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/UyaJJUas6BjJPXJYHk7ofTbHd0mBOV7fs9bBEKuEHicp0YTgXyy4ubo9Q5iadQ092OHsPpzltsKABYegve9y9gbQ/640?wx_fmt=jpeg)
图2. STDP的生物学特性[4]
另一种结构是带有循环链接，主要的代表模型是液体状态机(liquid state machine, LSM)。模型结构属于水库计算[5]（Reservoir Computing）的一种，是一种结构更复杂，用来处理高维时序问题的RNN（Recurrent Neural Network）结构[6, 7]。LSM的提出主要是通过循环链接表示了对时序问题的记忆能力，通过对输入的高维映射，可以用线性模型进行分类和回归。LSM的结构主要分为3层：
- 
**输入层：**主要负责将输入的模拟数值编码成脉冲序列（Spike Trains）并按照 一定比例输入到水库层中。
- 
**水库层：**根据生物脑中的神经元类型比例，包含80%的兴奋神经元和20%的抑制神经元，链接是随机的循环链接。
- 
**输出层：**读取每个水库层中神经元的状态，并用逻辑回归等进线性分类。
从机器学习的角度出发，SNN的应用主要分为分类和回归两种任务，这两种任务主要涉及到数据的组织和处理。因此，数据进入SNN之前需要先进行编码成为脉冲序列，经典的编码方式有频率（Rate），二进制（Binary），排序（Rank）等[4, 8]。
**3. 类脑计算与人工智能的研究趋势**
目前类脑计算的主要研究趋势分为三个方面：
- 
**首先**是基础的生物脑中的神经元，突触及记忆，注意等机制的建模；
- 
**第二**，基于生物机制建模的神经网络学习算法以及在模式识别等机器学习任务中的应用；
- 
**最后**，基于生物激励的算法和神经网络的硬件系统研究。
目前对于脑的认知机制的研究还存在很多空白，生物脑的运转机制的神秘面纱没有被彻底解开，只有少部分现象被发现[9]。脑建模是在认知脑的基础上进行的，目前的类脑计算算法还有很大的研究价值，目前发现的生物学机制只有少部分使用了计算神经学的方式进行模拟，被用在类脑计算中的机制则更加有限。另外，计算机仿真工具和数学的理论分析仍然不够完善，类脑计算没有形成统一的理论框架，面对大数据时代还没办法取代深度学习等成熟算法和工具的地位。在软件仿真方面，一些小规模的仿真工具相继出现，在研究类脑计算机理方面做出了一定贡献[10, 11]。由于近年来深度学习发展，同样有很多工作结合了深度学习相关概念进行了研究和探索[12, 13]。
能耗高一直是传统神经网络领域的一个难以解决的问题，目前只能通过云技术对小型便携设备和需要单独处理任务的设备提供快速响应的人工智能服务。而类脑计算具有更好的硬件亲和力，不需要复杂的硬件进行矩阵运算，只需在接收到脉冲时处理脉冲信号，计算效率得到很大的提高，目前已经有一些片上系统的原型出现，初步提供了SNN构建的人工智能框架[14, 15]。
总之，类脑计算还有很多工作需要完成，大量的有趣的、未知的和有挑战的问题需要解决，但这也是类脑研究的魅力所在。
**参考文献：**
[1] POO M-M, DU J-L, IP N Y, et al. China Brain
Project: Basic Neuroscience, Brain Diseases, and Brain-Inspired Computing [J].
Neuron, 2016, 92(3): 591-6.
[2] MAASS W. Networks of spiking neurons: The third generation of
neural network models [J]. Neural Networks, 1997, 10(9): 1659-71.
[3] GERSTNER W, KISTLER W M. Spiking neuron models: Single neurons,
populations, plasticity [M]. Cambridge university press, 2002.
[4] PAUGAM-MOISY H, BOHTE S. Computing with Spiking Neuron Networks
[M]. Springer Berlin Heidelberg, 2012.
[5] CHROL-CANNON J, JIN Y. Computational modeling of neural
plasticity for self-organization of neural networks [J]. BioSystems, 2014,
125(43-54.
[6] MAASS W. Liquid state machines: motivation, theory, and
applications [J]. Computability in context: computation and logic in the real
world, 2010, 275-96.
[7] MAASS W, NATSCHL GER T, MARKRAM H. Real-time computing without
stable states: A new framework for neural computation based on perturbations
[J]. Neural computation, 2002, 14(11): 2531-60.
[8] DAYAN P, ABBOTT L F. Theoretical neuroscience [M]. Cambridge, MA:
MIT Press, 2001.
[9] VOGELS T P, SPREKELER H, ZENKE F, et al. Inhibitory Plasticity
Balances Excitation and Inhibition in Sensory Pathways and Memory Networks [J].
Science, 2011, 334(6062): 1569-73.
[10] DIESMANN M, GEWALTIG M-O. NEST: An environment for neural systems
simulations [J]. Forschung und wisschenschaftliches Rechnen, Beiträge zum
Heinz-Billing-Preis, 2001, 58(43-70.
[11] GOODMAN D, BRETTE R. Brian: a simulator for spiking neural networks
in Python [J]. Frontiers in Neuroinformatics, 2008, 2(5):
[12] O'CONNOR P, WELLING M. Deep spiking networks [J]. arXiv preprint
arXiv:160208323, 2016,
[13] PANDA P, SRINIVASAN G, ROY K. Convolutional Spike Timing Dependent
Plasticity based Feature Learning in Spiking Neural Networks [J]. arXiv
preprint arXiv:170303854, 2017,
[14] WANG R C, COHEN G, STIEFEL K M, et al. An FPGA implementation of a
polychronous spiking neural network with delay adaptation [J]. Frontiers in
Neuroscience, 2013, 7(
[15] KHAN M M, LESTER D R, PLANA L A, et al. SpiNNaker: Mapping neural
networks onto a massively-parallel chip multiprocessor; proceedings of the 2008
IEEE International Joint Conference on Neural Networks (IEEE World Congress on
Computational Intelligence), F 1-8 June 2008, 2008 [C].
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)

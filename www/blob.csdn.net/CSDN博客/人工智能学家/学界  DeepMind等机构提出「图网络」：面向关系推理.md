# 学界 | DeepMind等机构提出「图网络」：面向关系推理 - 人工智能学家 - CSDN博客
2018年06月14日 21:26:18[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：170
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8z4picBkriajic7ONhhXudyMstpvnCfb6RUKoUPCBYHYYkoG0MCShPR1AAvcR8Dvzd1xoGwWibecN9gA/640?wx_fmt=png)
来源：机器之心
摘要：近日，由 DeepMind、谷歌大脑、MIT 和爱丁堡大学等公司和机构的 27 位科学家共同提交的论文《Relational inductive biases, deep learning, and graph networks》引起了人们的关注。
> 
深度学习虽然精于分类，但一直面临着难以进行关系推理的问题，很多研究希望通过其他方法让计算机学会「思考」。近日，由 DeepMind、谷歌大脑、MIT 和爱丁堡大学等公司和机构的 27 位科学家共同提交的论文《Relational inductive biases, deep learning, and graph networks》引起了人们的关注。该研究提出了一个基于关系归纳偏置的 AI 概念：图网络（Graph Networks）。研究人员称，该方法推广并扩展了各种神经网络方法，并为操作结构化知识和生成结构化行为提供了新的思路。
该论文作者之一，DeepMind 研究科学家 Oriol Vinyals 表示，该研究的代码也将在近期公开。
**论文标题: Relational inductive biases, deep learning, and graph networks**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8z4picBkriajic7ONhhXudyMsjDZYVHiaQ2yqgw4qpEpv3ibMzavpQNDvjRC7q6zdoTlZFibiapcFZXib4icQ/640?wx_fmt=png)
论文地址: https://arxiv.org/pdf/1806.01261.pdf
**摘要：**人工智能最近经历了一场复兴，在视觉、语言、控制和决策等关键领域取得了重大进展。取得这些进展的部分原因是由于廉价的数据和计算资源，它们符合深度学习的天然优势。然而，在不同压力下发展起来的人类智力，其许多决定性特征对于目前的方法而言仍是触不可及的。特别是，超越经验的泛化能力——人类智力从幼年开始发展的标志——仍然是现代人工智能面临的巨大挑战。
本论文包含部分新研究、部分回顾和部分统一结论。我们认为组合泛化是人工智能实现与人类相似能力的首要任务，而结构化表示和计算是实现这一目标的关键。正如生物学把自然与人工培育相结合，我们摒弃「手动设计特征」与「端到端」学习二选一的错误选择，而是倡导一种利用它们互补优势的方法。
我们探索在深度学习架构中使用关系归纳偏置如何有助于学习实体、关系以及构成它们的规则。我们为具有强烈关系归纳偏置的 AI 工具包提出了一个新构造块——图网络（Graph Network），它泛化并扩展了各种对图进行操作的神经网络方法，并为操作结构化知识和产生结构化行为提供了直接的界面。我们讨论图网络如何支持关系推理和组合泛化，为更复杂的、可解释的和灵活的推理模式奠定基础。
**3.图网络**
本论文提出的图网络（GN）框架定义了一类对图结构表征进行关系推理的函数。该 GN 框架泛化并扩展了多种图神经网络、MPNN 和 NLNN 方法（Scarselli 等，2009a; Gilmer 等，2017; Wang 等，2018c），并支持从简单的构建模块建立复杂的架构。注意，这里避免了在「图网络」中使用「神经」术语，以反映它可以用函数而不是神经网络来实现，虽然在这里关注的是神经网络实现。
GN 框架的主要计算单元是 GN 模块，这是一个「图到图」的模块，以图为输入，在结构层面上执行计算，并返回一个图作为输出。如 Box3 所示，实体由图节点表征，由边连接，系统级特性由全局属性表征。GN 框架的模块组织强调了可定制性，并能合成可以表达关系归纳偏置的新架构。其关键的设计原则是：灵活的表征；可配置的模块内部结构；以及可组合的多模块框架。
举个例子来比喻 GN 的形式化原则：考虑预测一堆橡胶球在任意引力场中的运动，它们不是互相碰撞，而是通过一个或多个弹簧互相连接。其结构和相互作用对应于 GN 的图表征和计算执行。
Box 3:「图」的定义 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8z4picBkriajic7ONhhXudyMsAG1H79INwRUpYqUaWcNdBrtWhiadfvQpLmpnq1qOr1FFKLDibgAaZ8ZA/640?wx_fmt=png)
这里我们使用「图」来表示具有全局属性、属性化的定向多图。在本文的术语中，节点表示为 v_i，边表示为 e_k，全局属性表示为 u。我们还使用 s_k 和 r_k 分别表示边 k 发送节点和接收节点（见下文）的索引。
更确切地说，这些术语定义为:
- 
**定向：**单向边，从「发送」节点到「接收」节点。
- 
**属性：**可以编码为向量、集合甚至其他图形的属性。
- 
**属性化：**边和顶点具有与其关联的属性。
- 
**全局属性：**图级属性。
- 
**多图**:顶点之间可以有多个边，包括自边（self-edge）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8z4picBkriajic7ONhhXudyMserWicV0Odaicx7Ty1oPJ5DqeiazAUnERAlictWcoLsfWmqDFX5Jnib01rhw/640?wx_fmt=png)
图3：GN 区块中的更新。蓝色表示正在更新的元素，黑色表示更新中涉及的其他元素（请注意，更新中也使用蓝色元素表示前更新值）。有关符号的详细信息，请参见等式 1。
**论文其他图与表**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8z4picBkriajic7ONhhXudyMsqQpVYCJdT0pxZKvT71Lj6iaETTZaicwUeRicuyh09Zz4KujPhbRZtENFA/640?wx_fmt=png)
表 1：标准深度学习组件中的各种关系归纳偏置。详见论文原文第 2 节。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8z4picBkriajic7ONhhXudyMs1LbfAzKCZD7iaSLM6Bse5CAgjzowAV0CmSTm2fWE0ZEMZjXI6s5PAJw/640?wx_fmt=png)
图 1：重复使用和共享常见的深度学习构件。（a）全连接层，其中所有权重都是独立的，没有共享。（b）卷积层，其中局部核函数在输入端被多次使用。共享权重由具有相同颜色的箭头指示。（c）循环层，其中相同的功能在不同的处理步骤中重复使用。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8z4picBkriajic7ONhhXudyMstpvnCfb6RUKoUPCBYHYYkoG0MCShPR1AAvcR8Dvzd1xoGwWibecN9gA/640?wx_fmt=png)
图 2：不同的图表征。（a）一个分子，其中每个原子表示为对应关系的节点和边（Duvenaud 等，2015）。（b）一个质量弹簧系统，其中绳索由在图中表示为节点的质量序列定义（Battaglia 等，2016；Chang 等，2017）。（c）一个 n 主体系统，其中主体是节点，底层图是完全连接的（Battaglia 等，2016 年；Chang 等，2017）。（d）一个精密的主体系统，其中球和壁是节点，底层图形定义球之间以及球和壁之间的相互作用（Battaglia 等，2016 年；Chang 等，2017）。（e）一个句子，其中单词对应于树中的叶子，其他节点和边可以由解析器提供（Socher 等，2013）。或者，可以使用完全连接的图（Vaswani 等，2017 年）。（f）一张图像，可以分解成与完全连接图像中的节点相对应的图像块（Santoro 等，2017；Wang 等，2018）。
本文为机器之心编译，转载请联系本公众号获得授权。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)

# MIT教授Tomaso Poggio演讲与专访：智能背后的科学与工程 | 腾讯AI Lab学术论坛 - 人工智能学家 - CSDN博客
2018年04月09日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：77
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVuRWcnm5vZia3CsfCCdSNo2OT8xdKrnwCWy8sEIg75oYXs0Mp01LlPnPWFAOS9T45Pq7l2FYnv0pg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
来源：腾讯AI实验室
腾讯AI Lab第二届学术论坛在深圳举行，聚焦人工智能在医疗、游戏、多媒体内容、人机交互等四大领域的跨界研究与应用。全球30位顶级AI专家出席，对多项前沿研究成果进行了深入探讨与交流。腾讯AI Lab还宣布了2018三大核心战略，以及同顶级研究与出版机构自然科研的战略合作。
腾讯AI Lab希望将论坛打造为一个具有国际影响力的顶级学术平台，推动前沿、原创、开放的研究与应用探讨与交流，让企业、行业和学界「共享AI+未来」。
**嘉宾介绍**
3月15日上午，麻省理工学院大脑与认知科学系Eugene McDermott 教授，大脑、心智和机器中心（CBMM）主任Tomaso Poggio教授做了主题为“智能背后的科学与工程（The Science and Engineering of Intelligence）”的演讲。
Poggio教授是计算神经科学领域巨匠，研究领域涵盖视觉系统的生物物理和行为、人类和机器的视觉和学习的计算分析。他的研究极富开拓性，是被引用最多的计算神经科学家之一。他的两位前博士后 Demis Hassabis和Amnon Shashua 分别是 DeepMind 和 Mobileye 的创始人。
**演讲内容**
在本演讲中，Tomaso Poggio介绍了AlphaGo和Mobileye这两大人工智能领域近期取得的成就，并介绍到其成功是建立在深度学习和强化学习这两种受神经科学启发的算法基础上的。但若要创造与人类同等聪明的人工智能，还需更多突破。本演讲讨论了这些突破可能是什么，并且源自何处。
演讲在硬件层面上，介绍了MIT大脑、心智和机器中心（CBMM）的研究项目、合作成果等，比如与不同研究中心的合作，在《科学》和《自然》杂志上发表的研究成果，组织课程，登月（moonshot）项目，对心理物理学的研究实验等。具体说明了树突树的生物物理性质能表现出比当今的 ReLU更强大的非线性，而在计算层面上，视觉智能的基本方面需要超越监督学习和无监督学习的架构。
**以下为演讲全文（为便于阅读进行过适当编辑整理）：**
今天我会谈一下智能背后的科学和工程。我在MIT进行过很多研究，但是我今天没有时间过多谈及这些。我会谈谈 MIT 最近推出的一项名叫 Intelligence Quest 的计划。然后我会谈谈其后的历史以及我们一些深度学习之外的研究。
首先让我谈谈大家都已知道的故事——最近人工智能领域的一些成功故事，从 Atari 游戏到 AlphaGo到单样本学习再到无人驾驶等等。其中一些重大成果是由 DeepMind 和 Mobileye 这两家创业公司实现的。有意思的是，这两家公司的创始人 Demis Hassabis 和Amnon Shashua都曾是我的博士后学生。前者推动实现了 AlphaGo，后者则创立了 Mobileye。这两项研究成果背后的核心技术分别是强化学习和深度学习。而这两种都源自神经科学。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TKpybwrM3q1RpyiaJic1X1dibBg20atx1iajgwLUUhfr3oH6qLoOgJJOVEA/640?wx_fmt=png)
大家看一下这段有趣的视频。这个项目从1995年开始做的，是我们和戴姆勒-奔驰的一个合作项目，也是最早将深度学习应用于计算机视觉的项目之一。当时我们训练这个系统的目标是让整个系统能识别出周围的环境和物体。

这里的视频非常清晰地展示出了我们这个系统的工作方式。这段视频拍摄自德国南部乌尔姆，这里是爱因斯坦的出生地。可以看到系统在最后几帧出现了一些错误，比如错误分类了交通信号灯和行人。当时的错误率是每三帧一个错误，也就是每秒钟出现十个错误。科学家虽然对此感到高兴，但这是无法实现实际应用的。而现在 Mobileye 的系统在同样的任务上每30千米的驾驶距离才会出现一个错误，准确率好了大约100 万倍，也就是这 20 年来平均每年都提升一倍，这样的进步是非常明显的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91T5uHLC9k3JJPPDbZib9r3BmviasTxWrmy1G16PQOA8RNhb0Dhic5iaqib1yw/640?wx_fmt=png)
回到我的主题，深度学习和强化学习都源自神经科学。强化学习的早期起源是巴甫洛夫对心理学和动物行为的研究，然后 Donald Hebb 最早提出将强化学习应用于机器和计算机。之后，Marvin Minsky 在 50 年代开发了一个名叫 SNARC 的系统，可以像老鼠一样去解开迷宫问题。上图右上角是这个系统的一个神经元，里面有一些真空管。然后我们看到 Hubel 和 Wiesel 在 60 年代在哈佛对神经架构的研究。之后还有很多科学家和研究者推动了这一领域的研究发展，让我们了解了大脑中的一些结构。这些研究成果为今天的深度卷积神经网络奠定了基础，比如图中所示的 ResNet。
那么机器学习的下一个关键突破点在哪里？从历史上看，最早的成果就是来自神经科学，下一个应该还是来自神经科学或认知科学。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TUEtibshsoueLIG8obB3HuLHhpw2KZ7sId3glImvo8eTicsYMY7UosS1w/640?wx_fmt=png)
接下来谈谈我们自己的工作。我们在 8 年前启动了MIT Intelligence Initiative，之后变成了大脑、心智和机器中心（CBMM）。我是该中心的主任。这是 NSF（美国国家科学基金）超过 5000 万美元的十年资助项目之一（2013～2023 年）。现在我们有了 MIT Intelligence Quest，它会为MIT的所有部门提供智能技术，比如生物系。我们的核心目标是纯粹的智能研究，而且研究领域涵盖科学和工程。我们中心的目标就是在智能问题上取得一些进展。
我相信智能问题不仅是一些科学问题的核心，比如宇宙的起源、生命的起源、时空结构等等；而且也是我们所有问题中最大的问题，因为只要我们在这一问题上取得进展，就能够让我们自己变得更加智慧，让机器变得更加智能。我们就能够快速地解决其它问题。此外，智能也是唯一一个可以在自然科学领域（包括神经科学、生物学、化学和物理学等）和工程技术领域（包括计算机科学和数学）同时探索的学科。所以这是唯一一个你可以同时获得图灵奖和诺贝尔奖的学科。
这不仅是科学技术的组合，也涉及到人与人的合作。MIT 和腾讯、IBM、微软、谷歌等机构在科学和工程上都有良好的合作，这是MIT十分独特的一面。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TxL0ZHzX9fn5bDCTXjos9HXiaKPJQ6JCxbIcvDiaAbIO8ib39SVic1wL2Sg/640?wx_fmt=png)
我们有来自不同研究中心的合作伙伴，其中有神经科学专家、计算机科学专家、认知科学专家。我们还有工业界的一些合作伙伴，有大公司也有小公司，比如 DeepMind、波士顿动力、Mobileye（我最近也加入了这家公司）。我们还有一个顾问委员会，有一些这个领域的主要科学家和我们合作。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TIpibMoiaxVgzIB5qR1BHo55BiaQADJmptJZIkTVxxTjLjkKia6jpLOkUWg/640?wx_fmt=png)
研究成果方面，我们 2017年在《自然》和《科学》杂志上发表了很多论文，数量超过 DeepMind 等机构。这不足为奇，因为我们非常专注于智能科学的研究。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TDUYfj88XwgWibfMdYcCzhBNXGhhSLYUsQxNiaK7EcqjXFzMqEtjfV1Cw/640?wx_fmt=png)
我们在过去四年里还组织了一些课程。我们想培养出新一代的科学家，他们不仅将擅长机器学习，也会有计算机科学、神经科学、认知科学等学科的研究经验。在这方面我们做得非常成功，并还将继续努力进行下去。
接下我谈谈我们在深度学习之外的研究。这是我们其中一个登月（moonshot）项目，我们希望借此了解我们的视觉智能。人有能力看到自己周围的东西，能够回答与此相关的近乎无限多的问题，比如这张图：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91Te03ea6tNNoibGsWT0Og2UkEcFia7HZQxQ0NyESGl4qAjr9kviaT4Y76bQ/640?wx_fmt=png)
我们可以看到这个场景中有多少桌子、坐了多少人、谁在说话、哪些位置是空闲的，你还知道需要绕过哪些障碍物才能到达目标。
我们是如何做到的呢？我们看看视觉系统底层的架构。可以看到视觉流流入大脑中的视觉区域，再进入大脑皮质中的特定区域，我们称之为大脑操作系统（Brain OS），这是大脑的推理中心，里面可以用很小的程序实时地回答任何问题。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TFRLKxDdmUgYSkTsRkyPshxyDekLsnFC09tSNQPRqOeRWhg6otZaQuQ/640?wx_fmt=png)
在我们的认知系统和物理世界的交互中，我们就产生了对世界的认识，理解了我们和世界的关系。这就是笛卡尔所说的“我思故我在”。这就是我们感知周围世界的方案。我们正在努力通过神经科学和认知科学解答这些问题，当然也可能使用计算机模拟这个系统。
这是我们的登月项目之一。我要谈谈它的一个应用，这个应用也在反过来推动相关研究。我们知道，人的视觉感知中有很大一部分是幻觉。我们人眼中的视觉感知非常依赖于偏心距（eccentricity），所以我们一次只能注意到一定距离一定角度内的事物（手臂长度大约为拇指宽/1度），其它内容都要靠我们脑补，是根据我们注意到的不同位置的图像融合起来的。进入我们眼睛的可能是一张高分辨率图像，但实际上我们感知注意到的是一系列低分辨率图像。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TTU72KDfPnqmp3fvMTGtkwEUNo0KogVwZQUNFfdFrOAulV03CBFZX3Q/640?wx_fmt=png)
这是我们的眼睛。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TPf7jkib4D9UKxXU8ZD4bfy80QeZ92YiawrAnaym7cDrOQAqTXw7xLzKg/640?wx_fmt=png)
光从左边进入眼睛，晶状体将其汇聚到眼睛后部的感受器上。这个感受器会和视网膜神经节细胞通信，得到信息。合在一起就是我们所说的视觉神经。这些信号最后会被传递给大脑中的视觉皮层。光感受器很密集，但感受野只有一小块区域。我们可以看到一个有趣的现象，视觉皮层的感受野的大小与光感受器的大小（也就是偏心距灵敏度）有关。这是来自 Hubel 和 Wiesel 的旧数据，展示了凝视视角与偏心距的关系。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91T8l8ZnfZmLaNhuLozIoicMXic77GqXCO6YBu6ibR8HHvNVCvjubJDE6j6Q/640?wx_fmt=png)
可以看到，在所有的视觉区域，视觉范围会随偏心距增大。我们对采样有一些推理得到的理论解释。底部的感受野通常大小为40×40，在上面有更大的感受野。小一点的感受野也许覆盖了更小的角度，上面大一点的覆盖了 5 度视觉角度。这样的安排，就能自动应对大小和尺寸的改变以及不同的位置。这意味着对以前没有看到的物体，比如你的脸，如果我向其它位置走一段距离，我仍然能认出那是你的脸。人的视觉能够应对这种变化。这个发现很重要，具有革命性，可以在计算机视觉上得到应用。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91ToTb5nYLCibn7GKrBzoJLUwVnpBZhXicVXcGUFkKmKSQ90qeOGwvEaIicw/640?wx_fmt=png)
最近，我们还研究了心理物理学（psychophysics)。我们用韩语字符测试了不认识韩语的受试者。我们将两套韩语字符用不同的大小或在不同的位置呈现，然后问受试者这些字符是不是一样的。这些字符的呈现速度很快，所以受试者不能移动他们的眼睛。可以看到我们测试了不同的距离（大小）和角度。如果我们去模拟这样的过程，或许就可以做出非常好的深度学习模型。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TqHgXr1lE0XmwvAB26CNR5b5TWetJYdlR1MQehKle0tIicwE2pyAH97A/640?wx_fmt=png)
看看我们的实验结果。左边是基于偏心距的现代深度学习模型，可以看到偏心距对准确度的影响不大（就像之前的心理物理学实验一样）。右边是普通的深度学习模型。因此，这就为我们带来一种基于偏心距的策略，这种策略非常依赖于移动你的眼睛。这决定了我们能更轻松或更难以识别出某些物体。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TDc54EWiczN1oQFlv3dH7afTsnt156gdTAjZzjFS7ibIJlmEjzdgksBCg/640?wx_fmt=png)
我再回过来谈一谈深度学习的问题。深度学习有个奇怪的地方：它非常成功，但我们不知道为什么。我们至少有三个理论问题需要解答。第一个是近似理论问题：深度网络何时以及为什么比只有一个隐藏层的浅网络更好？第二问题是关于优化。第三问题是关于学习的泛化能力。
对于这三个问题，我们有一些答案。我们知道，八十年代以来，浅和深度网络都可以实现输入和输出之间的映射。但它们都面临着维度灾难（curse of dimensionality）的问题，也就是说所需参数的数量会随着数据的维度指数级增长。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TfYS64Bma553CVUnucxWHw4RVbfVf39cKf0rFH3YrK7ThIoDtdkp2CA/640?wx_fmt=png)
现在，对于某些我们称之为局部组分函数（local compositional function）的特定类别的函数，虽然浅网络的参数数量还是会随维度而指数增长，但深度网络的参数只会随维度线性增长，从而能够避开维度灾难。这也是深度网络相对于浅网络的一个优势。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91T8o04p0Yib7XeC7781VEYNiaiaESevZ0XUZgU5rrLFNM8rnCGK6a7huDfw/640?wx_fmt=png)
时间有限，对于其它两个问题我就简单谈谈。对于第二个问题，如果对深度网络采用标准的使用方法，通常需要非常多的参数，有时候这些参数的权重的数量会超过训练集中数据点的数量。比如，对于 CIFAR，可以有 30 万个权重，而图像只有 6 万张。这样你事实上就会有无穷多个解决方案，所以不难找到一个。
第三个问题更重要：为什么训练后的深度网络能泛化到新的数据上呢？答案和动态系统的性质有关，涉及到梯度下降。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TKDP1cu9FL6R64r6mxOVduxEiagNzOB8CaEsJX0XPlqC6WFqR1xJNeicA/640?wx_fmt=png)
分类任务的特定性质与深度网络很契合，但分类任务只是智能任务中的一部分。现在整个科学界都在努力建立全面且优雅的深度学习理论。总而言之，我认为深度网络并不是智能的最好解决方案，可能只能解决10% 的智能任务。我们要基于神经科学和认知科学来寻找更好的解决方案。
谢谢！
**延伸阅读**
三月，受腾讯 AI Lab 学术论坛邀请，机器之心在深圳采访了深度学习理论研究著名学者 Tomaso Poggio。他以平直易懂的语言介绍了自己的「长篇系列工作」，也谈了谈他对理论指导实践以及仿生学指导深度学习算法发展等观点的看法。
Tomaso Poggio 的知名度，有相当一部分来源于他异常出色的导师身份：DeepMind 创始人及 CEO Demis Hassabis 和 Mobileye 创始人及 CTO Amnon Shashua，都是他的学生。这两家公司一个创造出了击败了围棋世界冠军、重新定义这个项目的 AlphaGo，另一个将辅助驾驶系统装进了全球超过 1500 万辆车里，制造了世界上第一款能在终端进行深度神经网络推理的量产车型的系统。Poggio 本人不仅鼓励他的学生们以创业的形式将深度学习带进现实世界，也亲身投入指导了这两家公司的早期创立。
然而在学术界，Poggio 的知名度更多来自于他的深度学习理论研究。他的论文非常好辨认——命名方式简单粗暴如同长篇系列小说的就是他，《深度学习理论 II》，《深度学习理论 IIIb》…… 
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/LWHwEYpIANJHYZTITMTicRiaz0evwEA91TGafyLYvFvgXb5nAnMjz2QcYEiczQQjg6g3jZw7CSwpzXF1Jnvk1lO1A/640?wx_fmt=jpeg)
这个编号系统来自他对深度学习理论问题进行的拆分：在 Poggio 看来，深度学习理论研究问题分为三类：
 • 第一类是表达（representation）问题：为什么深层网络比浅层网络的表达能力更好？
 • 第二类是优化（optimization）问题：为什么 SGD 能找到很好的极小值，好的极小值有什么特点？
 • 第三类是 泛化（generalization）问题：为什么参数比数据还多，仍然可以泛化、不过拟合？
对于每一类问题，他都尝试以应用数学工具为基础，通过举出能够用数学语言进行描述的例子然后给出解释的方式，用理论推导（也辅以一定的实验验证）来说明自己的观点。
**深层网络表达组合函数的超强能力**
早在 2002 年，Poggio 和著名数学家 Steve Smale 就合著了一篇论文[1]，总结了那些经典学习理论，它们的共同点是，都相当于具有单一隐藏层的网络。Poggio 是这样解释他研究「表达」的初衷：「当时我们就提出了一个问题：为什么大脑具有很多层？为什么当传统理论告诉我们使用单层网络的时候，大脑的视觉皮层其实在用许多层解决这一问题？」
毫无疑问，目睹了深度网络的成功后，同样的问题再一次被摆上了台面。
Poggio 认为，事实上无论是深层网络还是单层网络，都能相当不错地近似任意连续函数——这也是上世纪 80 年代的学者们通常忽略多层网络而采用单层网络的原因。但是，问题的核心在于表达清楚一个函数所需要的维度：单层网络需要的单元数非常多，甚至比宇宙中的原子数还要多。这就是数学上所说的「维度灾难」：**参数的个数需要是方程维度的指数倍。**
为了跳出维度灾难，过去的数学家尝试假设方程的光滑性：他们发现，维度灾难取决于「维度除以光滑性」。而深度学习给出了针对一类特定函数的独特方法：如果近似的对象是一个组合函数，换言之，是一个函数嵌套函数的大函数，那么**深度网络拟合它所需的单元数和函数的维度是线性关系。**换言之，无论维度多大，深度网络都能够摆脱维度灾难来拟合这个函数。
现在，能够被深度神经网络很好地拟合的数据都具有组合函数的特点。以图像为例，想要分类一张图像，并不需要同时观察左上角和右下角两个相距甚远的像素，只需要观察每一小块，再将它们组合在一起。有了这种「组合」（compositional）的性质，当卷积神经网络被用来刻画图像，甚至不需要参数共享的帮助，就能轻易摆脱维度灾难。
而那些现在还不能被神经网络刻画得很好的数据，通常都不是组合函数。但是知道「组合函数和神经网络很配」还远远不够，Poggio 说，「作为计算机科学学者，甚至作为数学家，我们能不能进一步解释一下组合函数，给出一些比『它是组合的』更明确的性质，从而更好地理解神经网络的工作机制。这对于我来说也是一个非常有趣的、希望更多研究者投入精力的开放问题。」
**优化的致胜：取之不尽的参数和性质漂亮的 SGD**
解线性方程组的时候，如果未知量的数量大于方程数，我们将收获很多解。
拿神经网络处理数据的时候，以图像举例子，包含 6 万张训练数据的 CIFAR 数据集，通常会用一个包含数十万乃至上百万参数的神经网络进行处理——一个教科书般的过参数化（overparameterization）例子。
将神经网络近似看成一个多项式——把每个具有非线性的小单元都用一个单变量多项式替代，得到一个由数十万乃至上百万多项式组成的大多项式，此时，想要在 CIFAR 上获得 0 训练误差，就转化成了一个解 6 万个线性方程的问题。根据贝祖定理（Bézout's theorem），此时的解的数量比宇宙中的原子数量还多。另外，参数多于数据量带来了「退化」（degenerate）性质：每一个解都对应着一个无限大的解集。
因此，过参数化意味着，**神经网络有无限多个退化的全局最优解，它们在损失空间里形成平坦的谷地。**
而众所周知，随机梯度下降（SGD）的特性就是会以较高的概率倾向于停留在退化的谷地里，即，停留在全局最优解上。二者的结合，就让神经网络的优化变得轻松：确定有全局最优、有很多，它们的特征明显，很容易被优化算法找到。
**就算过拟合也能泛化：分类问题与交叉熵的绝妙搭配**
过参数化是优化的福音，同时也是泛化的噩梦。在经典机器学习里，随着优化进行，测试错误率会呈现一条先减后增的 U 型曲线，尤其是模型规模与数据规模不匹配的时候，后半段的过拟合是十分可怕的。
然而在深度学习里，泛化错误率却经常呈现一个「下降，然后停住不动」的态势，即使不是零，也能保持在一个相当低的水准上。
Poggio 对此的解释是：这是深度学习所进行的任务与所用的损失函数之间的特定组合带来的美好化学反应。
具体来说，就是大多数神经网络都是用来解决分类问题（而不是回归问题）的，错误率通常以 0-1 损失计算，而目标函数却通常是交叉熵。
这种差异是因为 0-1 损失函数是好的效果衡量指标，却并不适合做优化的目标函数。拿手写数字分类器举例，神经网络分类器最后是通过 softmax 转 hardmax 来选择分类类别的，这也就意味着，即使模型认为一张「1」的图像是「1」的概率只有 30%，但只要这 30% 是所有 10 个可能性中最高的，模型仍然会将这张图像分类为「1」。一个信心水平只有 30% 的模型，即使分类正确，也远称不上一个好模型，需要继续优化。但是，如果选用 0-1 损失函数作为目标函数，只要分对了，该样本的损失就是 0 了，没办法计算梯度，也自然没办法进行反向传播来优化参数。
选用交叉熵做损失函数就没有这个烦恼，你可以一直优化到信心水平无限接近 100%。
而交叉熵函数与 0-1 损失这对组合的奇妙之处在于，即使**测试集上的交叉熵过拟合了，分类误差也不会过拟合。**
几个月前，芝加哥大学的 Srebro 组的工作 [2] 证明了：对于单层线性网络来说，如果数据集噪声较小、可分，那么即使交叉熵过拟合了，分类误差也不会过拟合。
「这是一个非常优美的，角度独特的工作。在此之上，我们用微分方程动力系统理论的工具证明了，在全局最小值附近，深度网络表现得就像一个线性网络。因此，我们可以将 Srebro 工作的结果用在深度学习上，说明即使神经网络分类器的交叉熵过拟合了，分类器本身也不会过拟合。」
交叉熵的这一性质是最小平方误差（least square error）等其他损失函数所不具备的，拥有这一性质的最简单的损失函数是指数误差（exponential loss）。而当我询问究竟是交叉熵的哪些特质让它拥有了如此特别的性质，是否和它的不对称性有关，Poggio 表示这仍然是一个有待讨论的问题。
以上就是 Poggio 的「深度学习理论三部曲」的内容概要了，详情请参阅 [3-7]。
除了他的工作本身，我们也和他聊了一些关于深度学习理论工作的其他问题：
**平坦的极小值意味着好的泛化能力吗？一个观点转变**
关于极小值的形状与泛化之间的关系，Poggio 说，他的观点转变了：「确实有学者在工作中表示，平坦是有利于泛化的。大概一年多以前我也曾经发表过类似的观点，但是我现在不再这么认为了。」
在关于优化的研究中，Poggio 证明了平坦确实会让优化过程变得容易，平坦的最小值也有更大可能是全局最小值。「但是我不觉得它和泛化之间有直接的联系，起码现在没有。如今对于泛化能力的研究，依赖于分类问题、依赖于损失函数的选择，却不依赖于平坦。Bengio 兄弟两人都参与的一篇论文就证明了，陡峭的极小值也是可以泛化的 [8]，因为你完全可以通过改变不同层的参数，在不改变网络的输入输出关系的前提下，让一个平坦的极小值变得陡峭。」
另外，他也认为完全平坦的极小值是不存在的，起码对于现在这种以多项式网络为基础添加非线性的神经网络来说，是不存在的。「我们都知道，一旦多项式在一个解集上都为 0，那么这个多项式处处为 0，因此，我不觉得存在完全平坦的极小值了。」
**对应用侧的建议：小心过拟合**
致力于应用深度学习算法的工程师们最经常对深度学习的理论研究者提出的一个问题就是：「你的工作很棒，但请问这能如何帮助我训练我的模型？」了解更多的理论知识当然具有启发意义，但是理论研究范围广阔且往往十分艰深，究竟哪些理论研究有助于应用开发者，应用开发者应该了解理论到何种程度？
机器学习里的无免费午餐定理（No Free Lunch Theorem），也就是 Wolpert 在 1996 和 1997 年发表的两篇著名论文里 [9, 10] 所提到的，学习算法之间没有先验区别，对于任何两个算法 A 和 B 来说，都存在一样多的两堆目标，对一堆目标来说 A 的检验误差比 B 高，对另一堆来说 B 的检验误差比 A 高。Poggio 援引了无免费午餐定理到理论研究中：不存在一个适用于所有问题的算法，类似地，也很难给出一个普适性正确的理论陈述。
「理论通常给出的是通常情况或最坏情况的分析，他们给出建议，告诉你应该做/不做什么，以避免最坏情况的发生。但是理论无法告诉你，对于一个特定案例来说，最佳方案是什么。」
而他对今天的深度学习应用者的建议是，小心过拟合。
「在过去几十年的机器学习发展史中，我们学到的一课是，如果你的数据集没有大到排除过拟合可能性，那么在一个特定数据集上的最佳方法通常是过拟合的、无法扩展到其他数据集上的。并不是说学者们『偷看』了验证集测试集，而是当一个社区的学者都在用不同的方法进行试错，那么一段时间后的最佳做法通常是过拟合了这个特定数据集的。」
「我曾经是一名物理学研究者，在我的学生时代，最普遍的经验法则是，如果你想建立一个参数为 n 的模型，那么至少要有规模为 2n 的数据，如果你想从统计的角度得出一些结论，更为推荐的数据规模是 10n。然而现在的深度学习研究者倾向于对所有问题都套用有数百万参数的模型。我们得出的『深度学习模型不受过拟合困扰』的论证只适用于特定问题（分类）、且要求数据集质量良好（可分），因此深度学习研究者应该对过拟合持有更谨慎的态度。」
**如何看待先验？理论研究能够告诉我们哪些关于先验的结论呢？**
人是很好的学习者，既不需要数百万数据，也不需要数据有标签，而这部分取决于我们与生俱来的、写在基因里的先验知识。然而，关于先天与后天（nature versus nurture）的争论从未停止。
「模型需要多少先验，是一个不能一概而论的、没有简单答案的问题。」Poggio 总结道，「理论研究的目的是找到能够做出特定预测所需的先验的下限。」
他以回归问题举例，「对于给定一些数据点来恢复一条曲线的任务来说，如果你什么都不告诉我，那么除非给我曲线上的所有点，否则我基本上什么也做不了。连续是一个必须的先验，但这还不够。我起码需要类似平滑（smothness）这样的性质，才能进行预测。而最重要的还是数据量，样本复杂度和先验之间，存在一个权衡取舍的关系。」
**深度学习能从人脑的学习过程中学到什么？**
三十年前，「深度学习之父」Geoffrey Hinton 用利于优化且计算高效的「反向传播」将整个领域带入了高速发展，而近年来，他则致力于寻找比反向传播更有可能在仿生学（bionics）上成立的结构。
MIT 对于深度学习的研究素来与脑神经科学结合紧密，Poggio 是如何看待这一问题的呢？
「我认为从生物学上完成反向传播并非完全不可能（not impossible），只能说，根据我们现在对神经元以及信号传递机制的了解，可能性不大（unlikely）。然而我认为真正不可能的是对所有样本的标注。」
「因此一个有趣的研究课题是，大脑是如何『绕开』标注的。例如，一个有趣的假设是，我们的视觉系统是以学习给图像『填色』来进行预训练的，它接收到了颜色信息，却只给其他视觉皮层以黑白的灰度信息，以此训练一个能够预测颜色的网络。在这个设定下，你不需要『神谕』（oracle）来告诉你真实的颜色是什么，你是有这部分信息的，只不过通过把它藏起来而建立了一个可以进行优化的模型。」
「类似的假设还有，大脑在不断地预测下一帧影像并进行优化等等。而能够预测颜色的、预测下一帧影像的视觉系统，是不是能够更好地进行其他视觉任务呢？是不是能够利用更少的数据就能学会识别物体呢？这都是有趣的开放问题，而且一旦得到答案后，将对深度学习产生巨大的推动。」
**Reference：**
1. Cucker, F., & Smale, S. (2002). On the mathematical foundations of learning. Bulletin of the American mathematical society, 39(1), 1-49.
2. Neyshabur, B., Tomioka, R., Salakhutdinov, R., & Srebro, N. (2017). Geometry of optimization and implicit regularization in deep learning. arXiv preprint arXiv:1705.03071.
3. Poggio, T., Mhaskar, H., Rosasco, L., Miranda, B., & Liao, Q. (2017). Why and when can deep-but not shallow-networks avoid the curse of dimensionality: A review. International Journal of Automation and Computing, 14(5), 503-519.
4. Liao, Q., & Poggio, T. (2017). Theory of Deep Learning II: Landscape of the Empirical Risk in Deep Learning. arXiv preprint arXiv:1703.09833.
5. Zhang, C., Liao, Q., Rakhlin, A., Miranda, B., Golowich, N., & Poggio, T. (2018). Theory of Deep Learning IIb: Optimization Properties of SGD. arXiv preprint arXiv:1801.02254.
6. Poggio, T., Kawaguchi, K., Liao, Q., Miranda, B., Rosasco, L., Boix, X., ... & Mhaskar, H. (2017). Theory of Deep Learning III: explaining the non-overfitting puzzle. arXiv preprint arXiv:1801.00173.
7. Zhang, C., Liao, Q., Rakhlin, A., Sridharan, K., Miranda, B., Golowich, N., & Poggio, T. (2017). Theory of deep learning iii: Generalization properties of sgd. Center for Brains, Minds and Machines (CBMM).
8. Dinh, L., Pascanu, R., Bengio, S., & Bengio, Y. (2017). Sharp minima can generalize for deep nets. arXiv preprint arXiv:1703.04933.
9. Wolpert, D. H. (1996). The lack of a priori distinctions between learning algorithms. Neural computation, 8(7), 1341-1390.
10. Wolpert, D. H., & Macready, W. G. (1997). No free lunch theorems for optimization. IEEE transactions on evolutionary computation, 1(1), 67-82.
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)

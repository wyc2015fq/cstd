# NeurIPS 2018提前看：生物学与学习算法 - 知乎
# 



机器之心原创，作者：Joni，编辑：Hao。

> 本文介绍了三篇关于生物学与学习算法的 NeurIPS 2018 论文。

**分析师简介**

Joni 目前是日本国立产业综合研究所的研究员。在中国大陆本科本行是自动化，后来对机器人研究有兴趣，在香港就读了电机工程的 Mphil 学位。博士时开始着迷生物学和脑科学的机器人研究，因此在德国汉堡大学参与了认知机器人的欧盟项目。此后一直欧洲，英国，日本和中国研究和讨论神经科学，生物学和机器人之间共通之处。

机器之心主页： [https://www.jiqizhixin.com/users/24e7c39e-98c4-4dd9-8d36-26d6207e1b67](https://link.zhihu.com/?target=https%3A//www.jiqizhixin.com/users/24e7c39e-98c4-4dd9-8d36-26d6207e1b67)

本文从 NeurIPS 的会议论文中选取了 3 篇非主流的学习算法文章：
- Assessing the Scalability of Biologically-Motivated Deep Learning Algorithms and Architectures。作者太多不尽列，都是来自 Google Brain 和 Deep mind 的研究人员，最有名当然是 Hinton 教授。
- Modelling sparsity, heterogeneity, reciprocity and community structure in temporal interaction data。作者是来自牛津大学统计系的 X Miscouridou, F Caron, YW Teh。其中 Prof Teh 也在 Deepmind 兼职研究科学家。
- Mental Sampling in Multimodal Representations。作者是来自英国华威大学的 J Zhu，A Sanborn 和 N Chater。

**论文： Assessing the Scalability of Biologically-Motivated Deep Learning Algorithms and Architectures**

论文链接：[https://arxiv.org/abs/1807.04587](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1807.04587)

第三篇文章来自著名的 Hinton 教授和他的合作者。在方法上是实验上验证深度网络的算法，与前两篇的统计学背景是有不一样。但众所周知，Hinton 教授忠实地支持建立生物学基础的机器学习模型。这篇论文正是在这一论调下，采取实验分析的方法比较深度网络下，比较各种有生物学基础的深度模型优化算法（feedback alignment FA、target propagation TP 两家族算法）与 Back-Propagation 算法的性能。据说这篇文章本来是投稿到了 ICLR 里，后来因为代码有错误原因主动撤稿了。

文中指出 BP 算法的两点主要与神经科学违背的地方：1) 反向传播中的权值是与正向传播的权值共享；2）错误传播后，上一层的神经元活动不能马上更新，因此与生物学的神经通讯的实验结果相违背。文中也介绍了另外一个比 BP 算法比较符合神经科学的 FA (feedback alignment) 算法。但仍没解决第二个问题。

相对来说，TP（target-propagation）算法是比较符合生物学实验结果的。TP 算法其中一个主要特征是每一层的神经活动都会尽量符合目标活动（问题 2）。因此有时会需要计算传递函数的反函数。如果在某些情况反函数不能取得，我们一般用近似的反变换代替
![](https://pic3.zhimg.com/v2-f90fb89bb13d97a6aa2ecbb1909b947a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='268' height='26'></svg>)
 (1)。




TP 算法也隐含了正向和反向的权值不一定共享，这样也解决了问题 1。

上面介绍了最基本的 TP，而本文也介绍两个改进的 TP 算法：

一个是 difference target propagation(DTP)。DTP 算法把神经的目标活动表示为
![](https://pic3.zhimg.com/v2-d27fc524f07eeb2275a2618b0cd85072_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='363' height='46'></svg>)
 (2)

相比起基本的 TP，DTP 后面所增加了一个修正项，表示神经元在反向的神经活动中的误差。这个修正项更加保证了 DTP 的稳定性。

另一个是 Simplified difference target propagation (SDTP). 神经的目标活动表示为
![](https://pic1.zhimg.com/v2-281d795164319c87164d57bc8fc35ac8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='323' height='30'></svg>)
 (3)

其中，
![](https://pic4.zhimg.com/v2-1c6b4a8c11f488938e45063cf35d50f3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='187' height='32'></svg>)
 就是在分类问题中的正确类别。正如图（1c）所示，SDTP 完全移除权值的梯度算法和权值共享这些不符合生物规律的算法。SDTP 的详细算法如下：
![](https://pic3.zhimg.com/v2-33c40d28b41660a37ae947c45eef04de_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='816' height='465'></svg>)![](https://pic4.zhimg.com/v2-cb81b80118f9f85307ec7f3a23a98827_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='789' height='430'></svg>)BP 与 TP 算法的比较示意图
另外文中也介绍了另外一个 AO-SDTP（auxiliary output SDTP）算法，与一般 SDTP 算法不同之处是它有额外的输出 z，所以完整的输出为
![](https://pic4.zhimg.com/v2-5bfc502fb7a3368fc13aad80a2f71cb3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='92' height='22'></svg>)
。$z$在这里可以看成是上一层 $h_{L-1}$ 的一个输出映射。因此在训练过程中，尤其在 one-hot 分类问题中，SDTP 不再用一个一单元的弱信号对上一层进行训练。而这种小窍门在之后的实验过程有一定的帮助作用。

接着本文比较了 BP 算法和 FA、TP 算法在深度网络的表现。为公平起见，以及凸显生物学事实，在卷积网络测试中，权值不再共享。

实验结论有点失望，虽然本文主要力推与生物类似的 TP（target-propagation），但在 MNIST 和 CIFAR10 仍比不上如今流行的 BP 算法和 FA 算法。另外在 ImageNet 也是 BP 完胜。但是可以留意到采用 AO-SDTP，所以提高网络输出的多样性，可以在一定程度上提高训练效果。
![](https://pic4.zhimg.com/v2-fe5bdf34aef2358ed5a0f94c981eece3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='801' height='725'></svg>)![](https://pic2.zhimg.com/v2-eab09f1c9132c316f8a2a22d199a533d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='828' height='298'></svg>)
虽然结果不能令人满意，但负面的实验结果也对后来的实验有指导作用。尤其是生物学基础的学习算法和传统 BP 算法并非公平比较，因为所有的网络结构和一些训练方法，基本都是为 BP 设计的。如果更多人投入对生物算法的研究，随着更多优化方法的推出，实验结果可能会有变化。

但作者也指出，另一方面 TP 和 FA 也不是完全的符合生物学规律。比如说神经科学上并没有实验指出有明确的正向和反向的传播过程。

个人觉得这篇论文的有趣之处在于指出了 BP 算法不是唯一可以依赖的优化方法。虽然在方法论上，我们是否真的需要生物学证据来发展机器学习还是值得讨论的课题。但继 CapsuleNet 后，Hinton 在自己的生物学机器学习道路又挖了一个坑，看看以后各位怎样跟随。

后两篇文章其实有相通之处，一篇是针对社交网络（social network）中人人之间的通讯沟通，尤其是他们的稀疏性，异构性，互惠性和社区结构进行建模，另外一篇是针对人心智思考中的觅食行为进行建模。这种对时空中事件的建模，我们一般采取统计学模型。




**论文：Modelling sparsity, heterogeneity, reciprocity and community structure in temporal interaction data**

论文链接：[https://arxiv.org/abs/1803.06070](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1803.06070)

本文的主要目的是针对社交网络的交流事件进行统计学上的建模。虽然我们在社交网络的互动交流看似随机，但随机中其实也藏着一些特性和规律。而找出这部分统计学上的规律是社会学家和心理学家感兴趣的话题。广义上的社交网络分析（social network analysis）是一个独立的研究课题。主要是利用网络理论和图论对社会的结构和发生的时间，比如人际合作关系、信息流传、朋友网络等进行分析。

而在本文中的 social network 的交流，主要是针对线上的数据（比如 Facebook，电子邮件等）进行分析。文中提出总结出了 4 个基本原则，是跟先前研究 (比如 Pinto 2016) 不一样的地方。

1）稀疏性：在人群中一般只有很小比率的人（无论在同一时间点或者总体来说）进行交流

2）异构性：有一部分人经常在社交网络进行交流，而另外一部分人很少交流

3）互惠性: 当节点 i 到节点 j 一个交流产生时，通常会有反向的交流接着发生

4）社区结构：同一个社区属性的人通常会进行交流。这里的社区属性指同一个学校，公司等等。

由于这种社交网络事件本质上是估计（t, i, j）这个序列（表示在时间 t 发生某用户 i 对用户 j 交流了），一般做法是直接对时间点发生的事件进行建模来估计先验分布。这就是所谓的「点过程（Point Process）」建模。其中最著名的就是泊松过程（Poisson process）。而 Hawkes Process 的思路是说当前发生的事件会对未来的概率密度函数（PDF）有影响，只是随着时间流逝这种影响会逐渐减弱，在社交网络上的体现就是对于一个当前发起的交流，很有可能对未来不久后的交流会有影响，但是越往后影响会越少。在数学上一般用 kernel 函数实现。

首先，此论文的最核心结论是以下两个函数：
![](https://pic3.zhimg.com/v2-612a3a049afff624f0c5e245363de966_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='47'></svg>)
（1）

其中，
![](https://pic2.zhimg.com/v2-594ee4277bfc220fca0325787700aff9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='291' height='25'></svg>)
（2）。

因为（2）的等号左边就表示在 dt 里面这个事件发生的概率，所以等式右边大家可以估计出$\lamda$ 表示是条件强度函数 conditional intensity function。具体到某两个节点（i 到 j 或者 j 到 i）之间的交流（强度），可以用（1）来定义。而（1）中的 g 函数就是刚刚提到的 kernel 函数，是随着时间递减的幂函数。
![](https://pic3.zhimg.com/v2-695309e73d54751d2098e505ad42fada_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='211' height='31'></svg>)
（3）

另外值得注意的是 $\lamda_{ij}$函数（用户 i 到 j 的交流）是跟用户 j 到 i 之间的交流次数的时间积分也有关。所以这种社区互动会引起互相兴奋 (mutally excited). 大致上可以在下图体现：
![](https://pic4.zhimg.com/v2-0fa6720e15dc4f77f1e8b89c415f0df3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='673' height='217'></svg>)
最后为了保证模型中的社区结构（community structure），需要再引入一个关系矩阵。具体来说就是同属于一个社区的两个个体的交流会增多。所以作者引入了 latent communities 的概念在矩阵里面，就是用潜变量来表示各个社团群体的从属关系。数学表现就是（1）里面的 $\mu$，称为基强度 (base intensity)。
![](https://pic3.zhimg.com/v2-2536b7dc0544ebac8299cf2202b5d062_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='195' height='57'></svg>)
（4）

其中 p 表示社区的数量。$w_ik$的含义是某个体 i 对于这个群体社区 k 的附属程度。
![](https://pic3.zhimg.com/v2-795d502e51ab24a2f59ba6d2eeb1d692_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='393' height='44'></svg>)
其中 $w_{io}$ 是属于个体 i 的程度修正量。具体的物理含义就是属于个体 i 的社交程度/受欢迎程度。而 $\beta_{ik}$ 是遵从参数（$a_k$,$b_k$）的 Gamma 过程。所以这种社区结构，如果只有单变量在数学上实际也是泊松过程，而当它涉及多变量时，叫做 CRM(compound random measure) 模型（Kingman 1967），而

$ W \sim CRM (\rho, H)$。

所以 W 就是遵循以 $(\rho, H)$ 的 CRM 模型。文章假设 H 也是遵循勒贝格测度（Lebesgue measure），所以 W 简化成

$ W \sim CRM (\rho)$

而当 $\rho$ 遵循莱维测度（Levy measure）时，Gamma 过程可以写成
![](https://pic3.zhimg.com/v2-106c36ee12fbaa3b02cfcd408374f1ce_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='259' height='48'></svg>)
其中，
![](https://pic3.zhimg.com/v2-0bff68bf95da355d1b86de21c61e2746_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='169' height='22'></svg>)
文章的数学上写的很曲折，但如果结合到社交网络的上下文，概率函数的主要功用是保证了人群中使用社交网络的异构性和稀疏性。某些人的 w 参与度比较多，他的 $w_{io}$ 也比较多。但其余的人比较少。

总结来说，这个模型结合了社交网络的 4 个客观特性，选取了适当的模型来迎合这些特性。虽然不少 paper 也是利用 Hawkes 过程建立社交网络模型（e.g. Pinto 2016），相比之前提出的一些类似模型（比如文中多次提到的 Todeshini 2016），这篇论文更强调了凸显社交网络中存在的一些客观性质。这些性质就要拓展已有的模型，而要体现不同群体不同兴趣的人群的社交交流，就要再结合 CRM 模型来实现。
- Kingman, John. "Completely random measures." Pacific Journal of Mathematics 21.1 (1967): 59-78.
- Pinto, JC Louzada, Tijani Chahed, and Eitan Altman. "A framework for information dissemination in social networks using Hawkes processes." Performance Evaluation 103 (2016): 86-107.
- Todeschini, Adrien, Xenia Miscouridou, and François Caron. "Exchangeable random measures for sparse and modular graphs with overlapping communities." arXiv preprint arXiv:1602.02114 (2016).

**论文： Mental Sampling in Multimodal Representations**

论文链接：[https://arxiv.org/abs/1710.05219](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1710.05219)

这篇文章想回答一个问题，当我们假设我们大脑遵循贝叶斯法则，做出的判断是遵循看到的现象和根据先验经验而估计出一个后验经验。而总所周知，我们不能在很快时间总结出所以过去发生的时间。所以一般人总是从后验分布中根据假设进行取样 (sampling)。先前的研究一般是采用直接取样 (Direct Sampling) 或者马尔可夫链蒙地卡罗 (MCMC)。本文提出因为一般心智空间的内容分布是块状的，而且取样中的某些噪声扰动对于一些生物现象是很重要的，所以作者提出了新的取样方法。

文章涉及的统计学知识也跟前一篇有相关，但想解决的问题不一样。第一篇文章是建立一个统计学模型，而这一篇是在建立好模型基础上的取样算法问题。具体来说，这篇文章建立在人的心智思考（基于贝叶斯定理的假设）中其实也存在这类似采样过程。

关于大脑的贝叶斯推理过程，可以表示成：

$ p(h|d) = \frac{p(d|h)p(h)}{\sum_{h \in H}p(d|h)p(h)} $（1）

这个推理过程如果人脑觉得在假设集合 H 里面可以解释现象 d，另外在集合 H 里面各个假设 h 是有一定的先验分布 $p(h)$, 而$p(d|h)$假设 h 下现象发生的概率 likelihood。

因为心智的计算跟我们平时计算贝叶斯定理差不多，不能把所有的过去的经验都逐一回忆（在算式里是积分或者求和）。所以怎样把统计学里的取样（sampling）解释人的心智思考，就是本文想解决的内容。

作者首先假设人的思考在心智空间（Mental Space）里是块状（「patchy」）分布的，块与块之间存在大量的空白。这种块可以解释为类似语义内容（semantic）是在心智空间里是以概念的形式聚合分布，类似的概念会聚集到附近。因此人的心理思考可以解释为类似觅食过程（文中称为 internal forage），在心智空间里面寻找符合的内容元素。

要描述这种觅食过程，文中假设了它在时间和空間都是遵循莱维过程（Levy Process）：
![](https://pic1.zhimg.com/v2-929799b41b4db0bffd4016a94df24f84_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='94' height='26'></svg>)
（2）

其中 $\mu$ 是固定值，$l$ 表示每次跳跃的长度。$1<\mu \leq 3$，所以幂函数解释了人思考的取样很大几率都是以短距离的 l 附近进行思考。这种例子可以在一些游戏测试中体现出来。（比如让参加者说出动物，人们说出的大部分都是有类似性质的聚类的动物，例如宠物，非洲动物等。）

而在时间结构上，类似的莱维过程也存在。
![](https://pic1.zhimg.com/v2-099f74aadfc8200e594faa3466fce2fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='120' height='34'></svg>)
（3）
![](https://pic3.zhimg.com/v2-f41c4ee2833072fb45dce6639e8fd45e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='107' height='38'></svg>)
（4）

算式（3）代表自相关函数 (autocorrelation function)（4）表示频谱功率 (spectral power)。因此他们随着时间 k 和频率 f 都会对于当前的状态有一定的相关性。具体来说，论文也列举了以下生物学的现象来体现这种 $\frac{1}{f}$的「噪声」波动的规律（Gilden 1995）。比如说人估计「一秒」的时间长度，他们的估计总在一秒实际时间之间以 1/f 扰动。另外人的视觉感知在某些错视图形面前也会来回更改（比如奈克方块 Necker cube（图 1）），这些都可以用心智模型中的 1/f 视域扰动来解释。
![](https://pic4.zhimg.com/v2-fe80d0e9a098a1d9f4bca8824cc16f1f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='150' height='135'></svg>)Necker Cube
接着文章提出了下一个问题，我们的心智决策中是通过什么的取样算法来实现这种时空里的莱维过程？文中比较了 3 种取样算法直接取样（Direct Sampling，DS），Random Walk Metropolis（RwM）和 Metropolis-coupled MCMC（MC3）。其中 DS 直接取样是最直接简易的蒙地卡罗方法，利用可得到的概率分布来估计未知变量的概率分布（如图 2）。但它的缺点是同样是不能求出算式（1）中的求和项。而 MCMC 算法因为假设了每个状态之间遵照马科夫链分布，估计完新的状态后然后再计算接受度（acceptance rate）来决定当前的取样是否接受。
![](https://pic1.zhimg.com/v2-e2df29920cb15d47f3099ebd2c480cdc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='394' height='229'></svg>)通过在 F（x）上的取样，估计 x 的分布
而本文重点介绍的 Metropolis-coupled MCMC（MC3）比 RwM 又多几个步骤，来符合莱维过程中的多模态特性（Geyer 1991）。首先，算法假设多条马尔可夫链同时存在，而状态在多条马尔可夫链的取样，是通过与温度参数（Temperature）相关的分布 $\pi^{\frac{1}{T}}$, 其中 $\pi ()$是先验密度函数。因此我们可以计算与接受度函数类似的交换函数
![](https://pic2.zhimg.com/v2-1ab3e04237b8eb143e3b0799d419afe5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='324' height='73'></svg>)
（5）

在算式 5 中，当 $A^{swap}$ 大于某个平均分布随机值 u 时，编号第 i 和 j 的马尔可夫链生成的取样就会交换。因此越高温度的马尔可夫链，可以更有机率被交换。在心智空间里面，这种取样方法可以有机会到达更加远的高概率空间。完整的 MC3 算法如下：
![](https://pic1.zhimg.com/v2-f1a71125e1f16e00f756804d8ca9990c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='837' height='400'></svg>)
论文当然也模拟的莱维过程对三种取样方法进行了比较。主要比较了三种方法能否跟踪生成的状态：
![](https://pic1.zhimg.com/v2-956f9bf565e08b5ab3cb425a9ac92994_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='656' height='221'></svg>)
但似乎 DS 方法的跟踪多模态分布比 MC3 还要好，不知是否跟文中只用 T=1 的马尔可夫链有关。

当然论文另一个推荐 MC3 的原因是 1／f 噪声扰动，似乎 MC3 表现不错。
![](https://pic1.zhimg.com/v2-2cd87dddc8d05615e2d3a55ce3419ce0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='573' height='461'></svg>)
个人评价：这篇文章标题很吸引，但看到最后觉得在最后只是针对 mental sampling 应用一个已有的算法。当然在实验中如今也只能用数学模拟实验。但如果能跟行为心理学的实验合作，检测参与者结合多模态的先验经验来在算法上新意不大，只是利用了现成的采样算法来实现贝叶斯心智过程的预估。当然在应用上来说，这种采样可能也能应用到其他多模态分布领域。
- Gilden, David L., Thomas Thornton, and Mark W. Mallon. "1/f noise in human cognition." Science 267.5205 (1995): 1837-1839.
- Geyer, Charles J. "Markov chain Monte Carlo maximum likelihood." (1991). 
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*






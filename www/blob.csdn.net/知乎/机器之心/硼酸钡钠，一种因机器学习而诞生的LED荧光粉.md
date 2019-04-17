# 硼酸钡钠，一种因机器学习而诞生的LED荧光粉 - 知乎
# 



选自[http://phys.org](https://link.zhihu.com/?target=http%3A//phys.org)，作者：University of Houston，机器之心编译。

> 来自休斯顿大学的研究者设计了一种新型机器学习算法，可在一台个人计算机上运行，并高效预测超过 10 万种合成物的性质，以搜索用于 LED 照明的最优荧光粉。他们合成并测试了其中一种通过计算预测得到的合成物：硼酸钡钠（sodium-barium-borate），并确定它能提供 95% 的效率和优越的热稳定性。

10 月 22 日，化学系助理教授 Jakoah Brgoch 及其实验室成员在 Nature Communications 期刊上发表了关于该研究的论文。

研究者使用机器学习快速扫描大量合成物，寻找关键属性，包括德拜温度和化学兼容性。Brgoch 之前曾发现德拜温度与荧光粉的效率有关。

发光二极管（LED）使用了少量稀土元素，通常是铕或铈，这些元素通常位于基质材料（如陶瓷或氧化物）内。这两种材料之间的相互作用决定了 LED 的性能。这篇论文主要介绍如何快速预测基质材料的特性。

Brgoch 说：「该项目强有力地证明了机器学习对开发高性能材料有很大价值，高性能材料领域通常由试错和简单的实证规则主导。它告诉我们应该看哪里，并指导我们的合成实践。」

除了 Brgoch，该论文的作者还包括 Brgoch 实验室的研究生 Ya Zhuo 和 Aria Mansouri Tehrani、前博士后研究员 Anton O. Oliynyk 和最近的博士生 Anna C. Duke。

Brgoch 和 UH 数据科学研究所（UH Data Science Institute）合作，并把 UH 高级计算和数据科学中心（UH Center for Advanced Computing and Data Science）的计算资源用于之前的研究。然而，用于这项研究的算法是在个人计算机上运行的。

该项目从 Pearson 的晶体结构数据库（Crystal Structure Database）中列出 118287 种可能的无机荧光粉化合物开始；该算法将这一数字削减到 2000 多。过了 30 秒后，它又生成了一份只有 20 几种可能材料的清单。

Brgoch 表示如果没有机器学习，这个过程要花费数周。

他的实验室研究机器学习和预测、合成，所以在算法推荐硼酸钡钠之后，研究员们做出了这种合成物。实验证明它非常稳定，量子产率或者效率达到 95%，但 Bugoch 说它产生的光不够蓝，不能满足商用。

这并没有令他们沮丧，他说：「现在我们能使用机器学习工具发现一种发冷光的材料，可发射出有用的波长。我们的目标不仅是使 LED 灯更高效，还要改进其颜色质量，且降低成本。」

关于这一点，研究人员称，他们证明了机器学习能极大地加速发现新材料的过程。这项研究是他们使用机器学习和计算发现变革性新材料的努力之一。

**论文：Identifying an efficient, thermally robust inorganic phosphor host via machine learning**
![](https://pic2.zhimg.com/v2-68d612ecd857d874be678b64324816f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='300'></svg>)
论文链接：[https://www.nature.com/articles/s41467-018-06625-z](https://link.zhihu.com/?target=https%3A//www.nature.com/articles/s41467-018-06625-z)

稀土取代的无机荧光粉对于固态物质照明很重要。传统上一般通过化学经验、试验以及试错合成来识别有潜力的新荧光粉，这些方法很难发现潜在的性能材料。这里，我们应用了支持向量机回归模型来预测荧光粉基质晶体结构的德拜温度，这个指标有望取代利用高吞吐量密度泛函理论估计带隙计算的光致发光量子产率。

该平台可以帮助识别在其它情况下可能被忽略的荧光体。在德拜温度最高、带隙最大的合成物中，NaBaB9O15 表现出了惊人的潜力。根据合成和结构表征，其结构刚性被证实来自一个共享 [B3O7]5-聚阴离子骨架的唯一角落。用 Eu2+代替这种材料可以产生紫外激发带和 416 nm 的窄带紫外发射，半峰全宽为 34.5 nm。更重要的是，NaBaB9O15:Eu2+具有 95% 的量子产率和优异的热稳定性。
![](https://pic1.zhimg.com/v2-7cfeed51d17758009d78c024f3a7e73c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='689' height='677'></svg>)
图 1：预测德拜温度的机器学习模型交叉验证图。（a）预测德拜温度（ΘD,SVR）的十折交叉验证（CV）vs 计算所得德拜温度（ΘD,DFT）。虚线是理想分界线，实线是拟合分界线。（b）根据化合物在 CV 预测的ΘD,SVR 和ΘD,DFT 之间的百分比误差来确定化合物的分数。红色曲线显示了趋势。




原文地址：[https://phys.org/news/2018-10-algorithm-quickly-materials.html](https://link.zhihu.com/?target=https%3A//phys.org/news/2018-10-algorithm-quickly-materials.html)



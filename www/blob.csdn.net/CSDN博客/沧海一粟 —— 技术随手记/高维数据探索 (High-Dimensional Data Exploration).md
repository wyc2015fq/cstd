# 高维数据探索 (High-Dimensional Data Exploration) - 沧海一粟 —— 技术随手记 - CSDN博客





2017年01月03日 14:35:52[慢游](https://me.csdn.net/eengel)阅读数：1749








在医学领域做数据分析，经常把数据先整理成一张大宽表，然后再进行诸如疾病风险预测之类的建模。这张大宽表每行代表一个患者（case）的数据，而且通常会包含非常多列，每列对应某一种数据（feature）比如患者基本信息、诊断、检查结果等。这样的数据如果有非常多的行和列，则可以被称作是高维数据(High-DimensionalData
 Exploration)。在进行具体建模分析之前，非常重要的一步是理解数据。数据探索就是为了在做具体数据分析之前，尽可能地了解某个数据集的特点[1]，看看它能告诉我们什么。

在拿到一个高维临床数据集时，最常见的比如要知道里面包含的患者都是什么样的人。如果数据集有成千上万行，那么不可能把这成千上万个患者都看一遍，这个时候最直观的就是做采样sampling。选取其中比较有代表性的若干个患者，做为整个数据集中全体患者的代表。这个选择又有多种方式。尽可能地随机选择是一种方式，先ranking排序，再选择top
 k的是一种方式，先filtering再找到有代表性的患者是一种方式，在某些feature上先进行summarization，比如得到了全体患者在年龄维度上的分布，再对整个数据集做总结，也是一种方式。还有就是做自动聚类clustering，综合多个维度上的特点，将患者聚类成几大群，从而做出总结。

这是从case的角度来看如何做数据探索。从feature的角度，也有若干种方法可以辅助。比如先做featureselection，再看不同的case在选中的feature上特性如何。再比如先做feature
 projection，将多维投影到一个低维空间，再做诸如summarization,clustering等操作。





[1] Exploratory data analysis. [https://en.wikipedia.org/wiki/Exploratory_data_analysis](https://en.wikipedia.org/wiki/Exploratory_data_analysis)




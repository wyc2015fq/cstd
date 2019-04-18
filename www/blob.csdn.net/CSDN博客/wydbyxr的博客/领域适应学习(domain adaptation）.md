# 领域适应学习(domain adaptation） - wydbyxr的博客 - CSDN博客
2018年09月30日 11:04:24[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：1177
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# 领域适应学习(domain adaptation）
  iid（独立同分布）	为了解决训练集和测试集的数据分布不匹配（即不满足iid条件），在无监督学习中提出了domain adaptation（领域自适应）	。
  领域自适应（Domain Adaptation）是迁移学习（Transfer Learning）的一种，思路是将不同领域（如两个不同的数据集）的数据特征映射到同一个特征空间，这样可利用其它领域数据来增强目标领域训练。	 领域自适应（Domain Adaptation）是迁移学习中的一种代表性方法，指的是利用信息丰富的源域样本来提升目标域模型的性能。
## 两大基本概念
  领域自适应问题中两个至关重要的概念：源域（source domain）表示与测试样本不同的领域，但是有丰富的监督信息，目标域（target domain）表示测试样本所在的领域，无标签或者只有少量标签。源域和目标域往往属于同一类任务，但是分布不同。
## 目的
  旨在利用源领域中带标签的样本来解决目标领域的学习问题，其关键在于如何最大化地减小领域间的分布差异，有效解决领域间数据分布的变化。领域适应学习放松了对训练数据和测试数据同分布假设的要求
## 与迁移学习的对比
  1）Transfer Learning：Label space are different (cross-task)
  2）Heterogeneous Domain Adaptation：Feature space are different (cross-feature)
## 例子
  传统的机器学习方法假设训练数据和测试数据服从于相同的概率分布，即是独立同分布（Independent and identically distributed，IID）的，但在实际应用中这个假设并不总得到满足，在许多应用中存在非IID数据。
  例如：
  1）语际(Cross-language) 文本挖掘,
  2）Wi-Fi定位；

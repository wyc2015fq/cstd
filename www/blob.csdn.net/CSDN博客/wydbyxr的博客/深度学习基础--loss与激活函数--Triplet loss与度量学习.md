# 深度学习基础--loss与激活函数--Triplet loss与度量学习 - wydbyxr的博客 - CSDN博客
2018年11月16日 09:49:51[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：193
# Triplet loss与度量学习
  Triplet loss 是一种非常常用的度量学习方法，而**Quadruplet loss 和 TriHard loss 是它的两个改进版本**。Quadruplet loss 相对于 Triplet loss 考虑了正负样本对之间的绝对距离，而 TriHard loss 则是引入了 hard sample mining 的思想。
  度量学习的目标是学习一个函数g(x):R1->R2，使得R1空间上语义相似度反映在R2空间的距离上。 通常我们需要定义一个距离度量函数来表示嵌入空间（Embedding space）的距离。

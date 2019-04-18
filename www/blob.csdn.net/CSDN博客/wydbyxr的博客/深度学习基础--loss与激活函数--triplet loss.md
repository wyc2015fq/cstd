# 深度学习基础--loss与激活函数--triplet loss - wydbyxr的博客 - CSDN博客
2018年11月16日 09:51:01[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：58
# triplet loss
  triplet是一个三元组，这个三元组是这样构成的：从训练数据集中随机选一个样本，该样本称为Anchor，然后再随机选取一个和Anchor (记为x_a)属于同一类的样本和不同类的样本,这两个样本对应的称为Positive (记为x_p)和Negative (记为x_n)，由此构成一个（Anchor，Positive，Negative）三元组。
  triplet loss的目的就是通过学习，让x_a和x_p特征表达之间的距离尽可能小，而x_a和x_n的特征表达之间的距离尽可能大，并且要让x_a与x_n之间的距离和x_a与x_p之间的距离之间有一个最小的间隔a。
  参考资料：[http://www.voidcn.com/blog/langb2014/article/p-4979076.html](http://www.voidcn.com/blog/langb2014/article/p-4979076.html)

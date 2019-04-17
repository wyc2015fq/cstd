# 【机器学习】boosting和bagging - zkq_1986的博客 - CSDN博客





2018年04月04日 17:01:51[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：51
个人分类：[Machine Learning](https://blog.csdn.net/zkq_1986/article/category/6374102)










Boosting 是一族可将弱学习器提升为强学习器的算法，这族算法的工作机制类似：先从初始训练集训练出一个基学习器，再根据基学习器的表现对训练样本分布进行调整，使得先前基学习器做错的训练样本在后续受到更多的关注，然后基于调整后的样本分布来训练下一个基学习器；如此重复进行，直至基学习器数目达到事先指定的值T，最终将这T个基学习器进行加权结合。随着迭代不断进行，误差会越来越小，所以模型的偏差 bias 会不断降低。这种算法无法并行，例子比如 Adaptive Boosting（AdaBoost）.


Bagging 是 Bootstrap Aggregating 的简称，意思就是重采用 (Bootstrap) 然后在每个样本上训练出来的模型取平均，它假设各模型独立，所以可以降低模型的方差 variance。 Bagging 比如 Random Forest 这种先天并行的算法都有这个效果。




转载自：https://www.zhihu.com/question/26760839/answer/33963551





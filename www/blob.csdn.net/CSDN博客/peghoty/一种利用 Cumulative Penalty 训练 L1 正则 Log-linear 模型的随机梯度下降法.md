
# 一种利用 Cumulative Penalty 训练 L1 正则 Log-linear 模型的随机梯度下降法 - peghoty - CSDN博客


2014年06月11日 14:06:14[皮果提](https://me.csdn.net/peghoty)阅读数：3920



Log-Linear 模型（也叫做最大熵模型）是 NLP 领域中使用最为广泛的模型之一，其训练常采用最大似然准则，且为防止过拟合，往往在目标函数中加入（可以产生稀疏性的） L1 正则。但对于这种带 L1 正则的最大熵模型，直接采用标准的随机梯度下降法（SGD）会出现**效率不高**和**难以真正产生稀疏性**等问题。本文为阅读作者 Yoshimasa Tsuruoka, Jun’chi Tsujii 和 Sophia Ananiadou 的论文[Stochastic Gradient Descent Training for L1-regularized Log-linear Models with Cumulative Penalty](http://anthology.aclweb.org//P/P09/P09-1054.pdf)（2009）后整理的读书笔记，文中提出了一种基于累积惩罚的 SGD 方法，可以克服上述提到的两个问题。

![](https://img-blog.csdn.net/20140611133933015)

![](https://img-blog.csdn.net/20140611135319203)

![](https://img-blog.csdn.net/20140611133947656)

![](https://img-blog.csdn.net/20140611133954437)![](https://img-blog.csdn.net/20140623211012859)![](https://img-blog.csdn.net/20140623211023250)![](https://img-blog.csdn.net/20140611134019750)![](https://img-blog.csdn.net/20140611134028265)
![](https://img-blog.csdn.net/20140611134513281)


作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/30049501](http://blog.csdn.net/itplus/article/details/30049501)
欢迎转载/分享, 但请务必声明文章出处.


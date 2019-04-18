# 深度学习基础--正则化与norm--正则化(Regularization) - wydbyxr的博客 - CSDN博客
2018年11月19日 10:27:51[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：36
个人分类：[深度学习基础																[机器学习基础](https://blog.csdn.net/wydbyxr/article/category/7151096)](https://blog.csdn.net/wydbyxr/article/category/6829999)
# 正则化(Regularization)
  一种防止过拟合，提高泛化能力的技巧，因此算法正则化的研究成为机器学习中主要的研究主题。此外，正则化还是训练参数数量大于训练数据集的深度学习模型的关键步骤。
  正则化可以避免算法过拟合，过拟合通常发生在算法学习的输入数据无法反应真实的分布且存在一些噪声的情况。
  一般来说，对分类或者回归模型进行评估时，需要使得模型在训练数据上使得损失函数值最小，即使得经验风险函数最小化，但是如果只考虑经验风险(Empirical risk)，容易过拟合(详细参见防止过拟合的一些方法)，因此还需要考虑模型的泛化能力，一般常用的方法便是在目标函数中加上正则项，由损失项(Loss term)加上正则项(Regularization term)构成结构风险(Structural risk)。
  参考资料：
[https://blog.csdn.net/u012162613/article/details/44261657](https://blog.csdn.net/u012162613/article/details/44261657)
[http://blog.sina.com.cn/s/blog_7103b28a0102w73g.html](http://blog.sina.com.cn/s/blog_7103b28a0102w73g.html)
[http://blog.csdn.net/zouxy09/article/details/24971995/](http://blog.csdn.net/zouxy09/article/details/24971995/)
## 正则化公式
  1）正则项，有L1、L2正则化。
  2）正则项系数，一般用λ表示，权衡正则项与C0项的比重。
  如果是L2正则化，则正则项还会乘以1/2，原因是正则项求导后的会产生一个2，与1/2相乘刚好凑整。

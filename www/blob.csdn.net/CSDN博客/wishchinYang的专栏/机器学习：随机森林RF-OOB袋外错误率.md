# 机器学习：随机森林RF-OOB袋外错误率 - wishchinYang的专栏 - CSDN博客
2016年09月12日 17:40:58[wishchin](https://me.csdn.net/wishchin)阅读数：10677
        文章讲解比较详细，且有Python代码，可以作为有用的参考。转载博客时候，竟然抄错成OBB了，也是悲剧................
         原文链接：[http://blog.csdn.net/zhufenglonglove/article/details/51785220](http://blog.csdn.net/zhufenglonglove/article/details/51785220)
** 参数：OOB-袋外错误率**
         构建随机森林的另一个关键问题就是如何选择最优的m（特征个数），要解决这个问题主要依据计算袋外错误率oob error（out-of-bag error）。
　　  随机森林有一个重要的优点就是，没有必要对它进行交叉验证或者用一个独立的测试集来获得误差的一个无偏估计。它可以在内部进行评估，也就是说在生成的过程中就可以对误差建立一个无偏估计。
　　  我们知道，在构建每棵树时，我们对训练集使用了不同的bootstrap sample（随机且有放回地抽取）。所以对于每棵树而言（假设对于第k棵树），大约有1/3的训练实例没有参与第k棵树的生成，它们称为第k棵树的oob样本。
　　而这样的采样特点就允许我们进行oob估计，它的计算方式如下：
　　（note：以样本为单位）
　　1）对每个样本，计算它作为oob样本的树对它的分类情况（约1/3的树）；
　　2）然后以简单多数投票作为该样本的分类结果；
　　3）最后用误分个数占样本总数的比率作为随机森林的oob误分率。
　　（文献原文：Put each case left out in the construction of the kth tree down the kth tree to get a classification. In this way, a test set classification is obtained for each case in about one-third of the trees. At the end of the run, take j to be the class that got most of the votes every time case n was oob. The proportion of times that j is not equal to the true class of n averaged over all cases is the oob error estimate. This has proven to be unbiased in many tests.）
　　oob误分率是随机森林泛化误差的一个无偏估计，它的结果近似于需要大量计算的k折交叉验证。
**后记：**
       一般的方法是，特征的维数是先确定的。更多的是对随机森林本身参数的选择，比如随机深林的层数，和树木的个数。

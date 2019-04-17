# 读了几篇boosting文献的收获。。。 - 迭代的是人，递归的是神 - CSDN博客





2012年11月22日 17:50:39[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：16687








距离上一篇blog都3个多月了。最近也是有的懒，看到别人的blog层次都很高，总是介绍些opencv的粗浅的东西，总是感觉自惭形愧。所以一直野就没写啥。白驹过隙，3个月，我都转博了，opencv都2.4.3了，可是感觉自己还是弱爆了，懂得太少，翻来看看以前学习过的知识，也多半一知半解。这次就又翻了翻boosting的东西学习了一下（研一上课就这东西听懂了。。。。）。自己看完了，也不知道自己的理解都对不对，欢迎各路高手拍砖指正，不吝赐教。

boosting这东西的思路也很清楚，就是多个弱分类器去学习，和一个强分类器是等价的。而且弱分类学习的算法实现起来要比强分类算法要容易的多。


提到boosting，不得不提最有代表性的adaboost，《A decision-theoretic generalization of on-line learning and an application to boosting》，adaboost就是提高前一轮被分类分错的样本的权重，降低前一轮正确分类样本的权重，最后再用加法模型得到一个总的分类器。关于每个弱分类器的系数以及更新样本权重分布的公式相关文献都有很好的证明，我就不多写了。算法流程如下，我读了几篇相关文献，有几个疑惑或收获吧：

![](https://img-my.csdn.net/uploads/201211/22/1353575619_7196.png)


1.关于每次迭代，训练样本的权值分布的更新，各文献的更新公式有所差别，但思想都是一样：分错时，w权重增大，分对时，w权重降低。

2.关于弱分类器的个数，也就是图中的T的确定，是否是个经验值？

然后又读了读online-adaboost，参考的是这篇《online bagging and boosting》，算法流程图如下：

![](https://img-my.csdn.net/uploads/201211/22/1353575817_4051.png)


这篇的思想就是对于新来的label的样本，如果更新弱分类器，有点类似online-bagging的做法。仔细看了看这个流程图，虽然没有证明，但我还是有个疑惑，随着训练样本的online增加，我感觉样本输入的顺序，对弱分类器的贡献是递减的，可能是我理解有误，希望高人指点一下~~

还有一篇onlineAdaboosting就是走的不同的路线《online boosting and vision》，文章也是结合他的应用来改进的adaboost算法，把boosting来做feature的select，而非用在weak classifier上。他这么做的好处，就是可以避免计算样本的权重分布，因为online的过程样本很多是未知的。也就是在每个feature pool中选择最具区分度的feature来做当前的分类器。算法流程如下：

![](https://img-my.csdn.net/uploads/201211/22/1353576987_3873.png)


![](https://img-my.csdn.net/uploads/201211/22/1353577004_6801.png)


为了提高feature pool的多样性，每次还把分类效果最差的分类器更新掉。而且在实际的编程中，还把feature pool的大小由M*N简化到M，这样大大降低了计算量。

最后，又看了看simi-boosting，也是上一篇作者的，结合tracking做的paper《semi-supervised online boosting for robust tracking》，如果细看可以这篇《semiboost：boosting for semi-supervised learning》。思想就是根据unlabeled的data和labeled data的相似度给予一个伪label，然后再做online
 boosting for feature selection。流程图如下：

![](https://img-my.csdn.net/uploads/201211/22/1353577816_5832.png)


写完发现又写的很水，很粗浅。其实也就是我读完了一个简单的笔记。具体的体会还得去看原文来的实在。也不知道我读的这些算不算是经典。如果大家有啥boosting的好文献，也希望多分享。之所以选择tracking作为应用，是因为这几篇有代码可以参考，如果有时间，可以结合code再说说我的心得体会。欢迎大家多提意见多交流，我才疏识浅，难免有错~~




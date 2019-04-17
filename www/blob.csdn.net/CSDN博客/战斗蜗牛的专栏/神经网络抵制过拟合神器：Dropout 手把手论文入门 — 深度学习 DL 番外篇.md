# 神经网络抵制过拟合神器：Dropout 手把手论文入门 — 深度学习 DL 番外篇 - 战斗蜗牛的专栏 - CSDN博客





2016年08月03日 21:13:38[vbskj](https://me.csdn.net/vbskj)阅读数：2945









今天David 9要带大家读偶像Hinton等大牛的一篇论文，搞深度学习或者DL的朋友应该知道，那就是有名的Dropout方法。


学过神经网络的童鞋应该知道神经网络很容易过拟合。而且，如果要用集成学习的思想去训练非常多个神经网络，集成起来抵制过拟合，这样开销非常大并且也不一定有效。于是，这群大神提出了Dropout方法：在神经网络训练时，随机把一些神经单元去除，“瘦身”后的神经网络继续训练，最后的模型，是保留所有神经单元，但是神经的连接权重乘上了一个刚才随机去除指数.


废话少说，上原理图：

![QQ截图20160729101338](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729101338.png)


左边是标准神经网络，右边是使用Dropout的神经网络，可见只是连接度少了一些，并不影响模型继续训练。其实，Dropout动机和初衷非常有意思。


文章里也谈到，可以类比人类男性和女性的基因。人类通过成千上万年进化，依然保持着这样的繁衍方式：男人贡献一半基因序列，女人贡献一半基因序列，最后组成后代的完整基因序列。当我们观察男人基因序列中的一个基因片段，它不仅要和男人基因序列很好地组合与配合，在繁衍后代时，也要和女人的那一半基因序列组合和配合，这个较好的一个基因片段一定要在两个情况下都很好的适应才行，这正像神经网络中的一个神经元，它要在各个情况下都很好地适应训练，所有，我们要Dropout一些神经元啊！了解了吧~


再来关注一个神经元：

![QQ截图20160729103207](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729103207.png)


需要指出的是，在训练时，权重参数是共享的。就是说，只要连接权重的神经单元不dropout，那么每次调参时，权重接着调整参数值。换句话说，权重参数个数和不用Dropout的神经网络参数个数是一样的。不同的是，见上图，在训练时，每个神经单元都可能以概率去除；在测试阶段，每个神经单元都是存在的，权重参数要乘以，成为：。


接下来看一下，每层Dropout网络和传统网络计算的不同之处：

![QQ截图20160729105520](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729105520.png)









![QQ截图20160729105437](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729105437.png)









![QQ截图20160729105533](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729105533.png)





![QQ截图20160729105456](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729105456.png)












最后就是一些实验结果和对比了，来瞻仰下：

![QQ截图20160729110624](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729110624.png)![QQ截图20160729110705](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729110705.png)![QQ截图20160729110753](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729110753.png)![QQ截图20160729110955](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729110955.png)![QQ截图20160729111118](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729111118.png)![QQ截图20160729111143](http://nooverfit.com/wp/wp-content/uploads/2016/07/QQ%E6%88%AA%E5%9B%BE20160729111143.png)


嘿嘿，这就是今天的Dropout入门啦，详细研究别忘了下载下面论文~


附论文下载：







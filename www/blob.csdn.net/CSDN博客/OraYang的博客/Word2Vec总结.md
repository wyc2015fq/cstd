
# Word2Vec总结 - OraYang的博客 - CSDN博客

2017年12月16日 19:37:43[OraYang](https://me.csdn.net/u010665216)阅读数：1198所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



最近一段时间，我写了好几篇关于Word2vec的文章，从理论部分到具体实践，现总结如下：
# 理论部分
[轻松理解skip-gram模型](http://blog.csdn.net/u010665216/article/details/78721354)
[轻松理解CBOW模型](http://blog.csdn.net/u010665216/article/details/78724856)
上述两篇博文从理论角度，讲述了Word2Vec两大模型
# 实践部分
[Word2vec使用手册](http://blog.csdn.net/u010665216/article/details/78709018)
这一篇文章主要从工具箱的角度，讲述如何使用现成的API来直接使用Word2Vec
[Word2vec之情感语义分析实战（part1）](http://blog.csdn.net/u010665216/article/details/78741159)
这一篇博文主要借助IMDB电影评论数据，利用词袋模型做特征构造，利用随机森林做预测，来判断一段评论是否是积极的还是消极的
[Word2vec之情感语义分析实战（part2）](http://blog.csdn.net/u010665216/article/details/78805403)
这一篇博文则讲述了如何在IMDB数据集上使用Word2Vec数据包，来构造分布式词向量，训练模型，并保存模型文件
[Word2vec之情感语义分析实战（part3）–利用分布式词向量完成监督学习任务](http://blog.csdn.net/u010665216/article/details/78813407)
这一篇博文则利用训练好的模型，对单词向量做矢量平均，或聚类，最后利用随机森林来预测。
通过上述part1,part2,part3所做的实验，我们发现词袋模型的效果比word2vec好。这个原因本质上是因为，word2vec后我们采用的矢量平均及聚类方法都忽略了单词顺序的影响，因此理论上应该和词袋模型效果差不多的。
那到底如何改善呢，我们可以尝试以下几点方法:
> 首先，用更多的文本来训练Word2Vec，这应该能大大提高性能。谷歌的结果是基于从超过10亿词的语料库中学到的词向量;我们的标签和未标记的训练集只有区区1800万字左右。

> Word2Vec提供了一些功能，可以加载预训练的模型。其实在已发表的文献中，分布式词向量技术已经被证明比词袋模型的表现要好得多。
[Word2vec:Tensorflow实战](http://blog.csdn.net/u010665216/article/details/78975005)
这篇文章利用tensorflow深度学习框架实现了skip-gram模型，算是个mini版


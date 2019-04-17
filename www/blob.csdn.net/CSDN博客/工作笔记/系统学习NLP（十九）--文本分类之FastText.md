# 系统学习NLP（十九）--文本分类之FastText - 工作笔记 - CSDN博客





2019年03月14日 10:41:05[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：65
个人分类：[自然语言](https://blog.csdn.net/App_12062011/article/category/8102623)









转自：[https://blog.csdn.net/sinat_26917383/article/details/54850933](https://blog.csdn.net/sinat_26917383/article/details/54850933)

FastText是Facebook开发的一款快速文本分类器，提供简单而高效的文本分类和表征学习的方法，不过这个项目其实是有两部分组成的，一部分是这篇文章介绍的

fastText 文本分类（paper：[A. Joulin, E. Grave, P. Bojanowski, T. Mikolov,

Bag of Tricks for Efficient Text

Classification（高效文本分类技巧）），

另一部分是词嵌入学习（paper:[P.

Bojanowski*, E. Grave*, A. Joulin, T. Mikolov, Enriching Word Vectors

with Subword

Information（使用子字信息丰富词汇向量））。

按论文来说只有文本分类部分才是 fastText，但也有人把这两部分合在一起称为

fastText。笔者，在这即认为词嵌入学习属于FastText项目。

github链接：[https://github.com/facebookresearch/fastText](https://github.com/facebookresearch/fastText)
.

## 高级词向量三部曲：

1、[NLP︱高级词向量表达（一）——GloVe（理论、相关测评结果、R&python实现、相关应用）](http://blog.csdn.net/sinat_26917383/article/details/54847240)

2、[NLP︱高级词向量表达（二）——FastText（简述、学习笔记）](http://blog.csdn.net/sinat_26917383/article/details/54850933)

3、[NLP︱高级词向量表达（三）——WordRank（简述）](http://blog.csdn.net/sinat_26917383/article/details/54852214)

4、其他NLP词表示方法paper:[从符号到分布式表示NLP中词各种表示方法综述](https://arxiv.org/pdf/1702.00764v1.pdf)
## 如何在python 非常简单训练FastText，可见笔者博客：

[极简使用︱Gemsim-FastText 词向量训练以及OOV（out-of-word）问题有效解决](https://blog.csdn.net/sinat_26917383/article/details/83041424)

# 一、FastText架构

> 
本节内容参考自：

1、开源中国社区 [[http://www.oschina.net](http://www.oschina.net)] [《Facebook 开源的快速文本分类器 FastTexT》](http://www.oschina.net/news/76286/facebook-fasttex)

2、雷锋网文章：《[比深度学习快几个数量级，详解Facebook最新开源工具——fastText](http://www.leiphone.com/news/201608/y8rhWEglraduqcOC.html)》
.

## 1、fastText 架构原理

> 
fastText 方法包含三部分：模型架构、层次 Softmax 和 N-gram 特征。


fastText 模型输入一个词的序列（一段文本或者一句话)，输出这个词序列属于不同类别的概率。

序列中的词和词组组成特征向量，特征向量通过线性变换映射到中间层，中间层再映射到标签。

fastText 在预测标签时使用了非线性激活函数，但在中间层不使用非线性激活函数。

fastText 模型架构和 Word2Vec 中的 CBOW 模型很类似。不同之处在于，fastText 预测标签，而 CBOW 模型预测中间词。
2softmax_53" target="_blank">![这里写图片描述](https://img-blog.csdn.net/20170203222243970?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


.


.


2、改善运算效率——softmax层级
对于有大量类别的数据集，fastText使用了一个分层分类器（而非扁平式架构）。不同的类别被整合进树形结构中（想象下二叉树而非 list）。在某些文本分类任务中类别很多，计算线性分类器的复杂度高。为了改善运行时间，fastText 模型使用了层次 Softmax 技巧。层次 Softmax 技巧建立在哈弗曼编码的基础上，对标签进行编码，能够极大地缩小模型预测目标的数量。（参考[博客](http://www.cnblogs.com/Jezze/archive/2011/12/23/2299884.html)）
![这里写图片描述](https://img-blog.csdn.net/20170203222915738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
考虑到线性以及多种类别的对数模型，这大大减少了训练复杂性和测试文本分类器的时间。fastText 也利用了类别（class）不均衡这个事实（一些类别出现次数比其他的更多），通过使用 Huffman 算法建立用于表征类别的树形结构。因此，频繁出现类别的树形结构的深度要比不频繁出现类别的树形结构的深度要小，这也使得进一步的计算效率更高。
![这里写图片描述](https://img-blog.csdn.net/20170203222922720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.

.

.
# 二、FastText的词向量表征

## 1、FastText的N-gram特征

常用的特征是词袋模型。但词袋模型不能考虑词之间的顺序，因此 fastText 还加入了 N-gram 特征。

“我 爱 她” 这句话中的词袋模型特征是 “我”，“爱”, “她”。这些特征和句子 “她 爱 我” 的特征是一样的。

如果加入 2-Ngram，第一句话的特征还有 “我-爱” 和 “爱-她”，这两句话 “我 爱 她” 和 “她 爱 我” 就能区别开来了。当然，为了提高效率，我们需要过滤掉低频的 N-gram。
在 fastText 中一个低维度向量与每个单词都相关。隐藏表征在不同类别所有分类器中进行共享，使得文本信息在不同类别中能够共同使用。这类表征被称为词袋（bag of words）（此处忽视词序）。在 fastText中也使用向量表征单词 n-gram来将局部词序考虑在内，这对很多文本分类问题来说十分重要。

2FastText_84" target="_blank">举例来说：fastText能够学会“男孩”、“女孩”、“男人”、“女人”指代的是特定的性别，并且能够将这些数值存在相关文档中。然后，当某个程序在提出一个用户请求（假设是“我女友现在在儿？”），它能够马上在fastText生成的文档中进行查找并且理解用户想要问的是有关女性的问题。


.


.


2、FastText词向量优势
（1）适合大型数据+高效的训练速度：能够训练模型“在使用标准多核CPU的情况下10分钟内处理超过10亿个词汇”，特别是与深度模型对比，fastText能将训练时间由数天缩短到几秒钟。使用一个标准多核 CPU，得到了在10分钟内训练完超过10亿词汇量模型的结果。此外， fastText还能在五分钟内将50万个句子分成超过30万个类别。

（2）支持多语言表达：利用其语言形态结构，fastText能够被设计用来支持包括英语、德语、西班牙语、法语以及捷克语等多种语言。它还使用了一种简单高效的纳入子字信息的方式，在用于像捷克语这样词态丰富的语言时，这种方式表现得非常好，这也证明了精心设计的字符 n-gram 特征是丰富词汇表征的重要来源。FastText的性能要比时下流行的word2vec工具明显好上不少，也比其他目前最先进的词态词汇表征要好。
![这里写图片描述](https://img-blog.csdn.net/20170203225206176?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（3）fastText专注于文本分类，在许多标准问题上实现当下最好的表现（例如文本倾向性分析或标签预测）。FastText与基于深度学习方法的Char-CNN以及VDCNN对比：
![FastText与基于深度学习方法的Char-CNN以及VDCNN对比 ](https://img-blog.csdn.net/20170203225630220?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

（4）比word2vec更考虑了相似性，比如 fastText 的词嵌入学习能够考虑 english-born 和 british-born 之间有相同的后缀，但 word2vec 却不能（具体参考[paper](https://arxiv.org/pdf/1607.04606v1.pdf)）。

.

.
## 3、FastText词向量与word2vec对比

本节来源于博客：[fasttext](http://blog.csdn.net/phoeny0201/article/details/52329477)

FastText= word2vec中 cbow + h-softmax的灵活使用

灵活体现在两个方面：
- 模型的输出层：word2vec的输出层，对应的是每一个term，计算某term的概率最大；而fasttext的输出层对应的是 分类的label。不过不管输出层对应的是什么内容，起对应的vector都不会被保留和使用；
- 模型的输入层：word2vec的输出层，是 context window 内的term；而fasttext 对应的整个sentence的内容，包括term，也包括 n-gram的内容；

两者本质的不同，体现在 h-softmax的使用。

Wordvec的目的是得到词向量，该词向量 最终是在输入层得到，输出层对应的 h-softmax 也会生成一系列的向量，但最终都被抛弃，不会使用。

fasttext则充分利用了h-softmax的分类功能，遍历分类树的所有叶节点，找到概率最大的label（一个或者N个）
相关文章：[Jupyter notebooks：Comparison of FastText and Word2Vec](http://nbviewer.jupyter.org/github/jayantj/gensim/blob/683720515165a332baed8a2a46b6711cefd2d739/docs/notebooks/Word2Vec_FastText_Comparison.ipynb#)

.

.

.
# 三、FastText实现

github链接：[https://github.com/facebookresearch/fastText](https://github.com/facebookresearch/fastText)

fastText基于Mac OS或者Linux系统构筑，使用 C++11 的特性。需要python 2.6 或者更高版本支持，以及numpy & scipy等软件支持。

FastText默认参数：``$ ./fasttext supervised
Empty input or output path.

The following arguments are mandatory:
  -input              training file path
  -output             output file path

The following arguments are optional:
  -lr                 learning rate [0.1]
  -lrUpdateRate       change the rate of updates for the learning rate [100]
  -dim                size of word vectors [100]
  -ws                 size of the context window [5]
  -epoch              number of epochs [5]
  -minCount           minimal number of word occurences [1]
  -minCountLabel      minimal number of label occurences [0]
  -neg                number of negatives sampled [5]
  -wordNgrams         max length of word ngram [1]
  -loss               loss function {ns, hs, softmax} [ns]
  -bucket             number of buckets [2000000]
  -minn               min length of char ngram [0]
  -maxn               max length of char ngram [0]
  -thread             number of threads [12]
  -t                  sampling threshold [0.0001]
  -label              labels prefix [__label__]
  -verbose            verbosity level [2]
  -pretrainedVectors  pretrained word vectors for supervised learning []


Mikolov 在 fastTetxt 的论文中报告了两个实验，其中一个实验和 Tagspace 模型进行对比。实验是在 YFCC100M 数据集上进行的, YFCC100M 数据集包含将近 1 亿张图片以及摘要、标题和标签。实验使用摘要和标题去预测标签。Tagspace 模型是建立在 Wsabie 模型的基础上的。Wsabie 模型除了利用 CNN 抽取特征之外，还提出了一个带权近似配对排序 (Weighted Approximate-Rank Pairwise, WARP) 损失函数用于处理预测目标数量巨大的问题。
![这里写图片描述](https://img-blog.csdn.net/20170203230535882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上面就是实验结果，从实验结果来看 fastText 能够取得比 Tagspace 好的效果，并拥有无以伦比的训练测试速度。但严格来说，这个实验对 Tagspace 有些不公平。YFCC100M 数据集是关于多标记分类的，即需要模型能从多个类别里预测出多个类。Tagspace 确实是做多标记分类的；但 fastText 只能做多类别分类，从多个类别里预测出一个类。而评价指标 prec@1 只评价一个预测结果，刚好能够评价多类别分类。

YFCC100M 数据： [](https://research.facebook.com/research/fasttext/)https://research.facebook.com/research/fasttext/](https://arxiv.org/abs/1607.04606)](https://arxiv.org/abs/1607.01759)




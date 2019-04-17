# LDA︱基于LDA的Topic Model变形+一些NLP开源项目 - 素质云笔记/Recorder... - CSDN博客





2016年08月02日 16:04:48[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4759











一、LDA的变形

转载于：[转：基于LDA的Topic Model变形](http://www.cnblogs.com/wentingtu/archive/2013/06/02/3113422.html)








Topic Model最适合的变种是加入先验信息：
我相信题主使用的是完全无监督的Topic Model，然而这实在是过于不work~~~浪费了现实生活中那么多的标注数据，有监督的模型一定比无监督的好~所以！可以试试Supervised Topic Model利用你在现实中已有的标注来提高模型准确度~比如利用知乎的tag来train个有监督Topic Model~~~一定会词聚类效果好不少。

开源的的有监督的LDA:
[iir/llda.py at master · shuyo/iir · GitHub](https://link.zhihu.com/?target=https%3A//github.com/shuyo/iir/blob/master/lda/llda.py)
[chbrown/slda · GitHub](https://link.zhihu.com/?target=https%3A//github.com/chbrown/slda)






## 基于LDA的Topic Model变形


基于LDA的Topic Model变形最近几年来，随着LDA的产生和发展，涌现出了一批搞Topic Model的牛人。我主要关注了下面这位大牛和他的学生：
David M. BleiLDA的创始者，04年博士毕业。一篇关于Topic Model的博士论文充分体现其精深的数学概率功底；而其自己实现的LDA又可体现其不俗的编程能力。说人无用，有论文为证：
- J. Chang and D. Blei. **Relational Topic Models for Document Networks**. *Artificial Intelligence and Statistics*, 2009. [[PDF](http://www.cs.princeton.edu/~blei/papers/ChangBlei2009.pdf)]
       基本LDA模型，当然假设文档之间是可交换的，那么在原始的LDA中文档之间其实是认为条件独立的。而在实际情况中，往往不是这个样子的，文档间也许会存 在“social network”的这样的网络性质。如何结合内容和“social network”这两个特征也许是一个非常有意思的话题。这篇论文就是给出了一个解决方法。它为两个文档之间增加了一个二元随机变量，根据其内容特征，来 刻画这种隐含的链接关系。

       关于显示的链接关系是过去今年内，人们追逐研究的对象，进而产生PageRank、HITS等等一大批优秀的链接关系算法。那么如何利用隐含的链接呢？什 么是隐含的链接呢？一个最简单的隐含链接就是基于内容相似度构建的图。这个被人们用的不亦乐乎，比如在文摘中的LexRank等。O Kurland在SIGIR中发了两篇大概都是类似的文章，本质思想貌似就是在利用内容之间的“超链接”。
       另外一个比较新颖的研究点，就是如何基于“social network”来挖掘内容特征？ Mei Qiaozhu的一篇论文就是利用“social network”的网络结构特征最为规则化因子，重新修正了原始的PLSA模型。想法非常的新颖。

- D. Blei and J. Lafferty. **Topic Models. **In A. Srivastava and M. Sahami, editors, *Text Mining: Theory and Applications*. Taylor and Francis, in press. [[PDF](http://www.cs.princeton.edu/~blei/papers/BleiLafferty2009.pdf)]
    这篇论文是一篇综述性的大制作的论文，Blei在里面深入浅出的介绍了什么是Topic Model以及他早期的一些Topic Model的变形。值得大家去阅读。

- J. Boyd-Graber and D. Blei. **Syntactic Topic Models**. *Neural Information Processing Systems*, 2009. [[PDF](http://www.cs.princeton.edu/~blei/papers/Boyd-GraberBlei2009.pdf)] [[Supplement](http://www.cs.princeton.edu/~blei/papers/Boyd-GraberBlei2009b.pdf)]
   原始的LDA考察两个词只是基于共现的角度。而实际情况中，这种共现往往是不能够精确地刻画一些句子结构信息或者说词义信息。如何把这种信息引入。考虑 更深层的生成模型是目前一个热点。这篇论文着眼于一个句子的句法分析的生成过程，它认为每个句子的生成都是基于“parse tree”的，整个概率生成过程完全附着在“parse tree”上了。并且每个句子内，不同的词都有可能去选择更适合自己的Topic。

- D. Blei, J. McAuliffe. **Supervised topic models**. In Advances in Neural Information Processing Systems 21, 2007. [[PDF]](http://www.cs.princeton.edu/~blei/papers/BleiMcAuliffe2007.pdf) [[digg data](http://www.cs.princeton.edu/~blei/papers/digg-data.tgz)]
   现如今，网络数据除了纯内容外，往往还有其他一写辅助信息，如用户对于某博文的评价或者说用户对于某商品的评价。一个最典型的例子，就是说在当当买书 后，你可以给该书的质量进行打分：5星代表最好，4星代表比较好，。。。依次类推。那么如何把这些信息加入原始的LDA中呢？ Blei为其引入了一个response变量因子，该因子条件依赖于该文档的topic distribution。

    如何把ratable information和内容有机地结合起来也是最近的一个研究热点。大多数方法还都是，建立一个ratable response variable，然后该变量条件依赖于内容或者说Topic信息。

- J. Boyd-Graber, D. Blei, and X. Zhu. **A topic model for word sense disambiguation**. In Empirical Methods in Natural Language Processing, 2007. [[PDF]](http://www.cs.princeton.edu/~blei/papers/Boyd-GraberBleiZhu2007.pdf)
    这篇论文对应的一个大背景是把Topic Model应用到自然语言处理中，具体内容我没太看，主要是结合了WordNet的结构特征，在此基础上产生的图模型。
    此外的一些工作还有把Topic Model用来文摘和词性标注中的。应用到这些问题的两个主要思路：第一个就是用Topic Model去学习出一些compact features，然后在次基础上利用分类器等机器学习方法；另外一种就是利用原始NLP问题的一些结构信息，比如刚才所说的WordNet中的网络结 构，在这个结构特征中推导出整个图模型的概率生成过程。

- D. Blei and J. Lafferty. **A correlated topic model of *Science***. Annals of Applied Statistics. 1:1 17–35. [[PDF](http://www.cs.princeton.edu/~blei/papers/BleiLafferty2007.pdf)] [[shorter version](http://www.cs.princeton.edu/~blei/papers/BleiLafferty2006.pdf) from NIPS 18] [[code](http://www.cs.princeton.edu/~blei/ctm-c/)][[browser](http://www.cs.cmu.edu/~lemur/science/)]
   还没有认真看，这个其实打破了原来topic之间的可交换性。

- D. Blei and J. Lafferty. **Dynamic topic models**. In Proceedings of the 23rd International Conference on Machine Learning, 2006. [[PDF](http://www.cs.princeton.edu/~blei/papers/BleiLafferty2006a.pdf)]
   也没有仔细看，把Topic Model和时间维度结合了起来。Mei Qiaozhu也有一篇是研究话题内容随着时间变化的论文，但是是基于PLSI和HMM来完成的。

- T. Griffiths, M. Steyvers, D. Blei, and J. Tenenbaum. **Integrating topics and syntax**. In Advances in Neural Information Processing Systems 17, 2005. [[PDF](http://www.cs.princeton.edu/~blei/papers/GriffithsSteyversBleiTenenbaum2004.pdf)]
   这篇论文是一篇非常优秀的论文，开篇详细地叙述了词的不同功能分类，也叫做HMM-LDA模型。正如每个人存在都有其社会意义，那么词存在对于文本语义 的表述也有着不同的角色。作者把词分为了两大功能：第一个就是semantic功能，也就是之前我们所有的Topic word；另一个功能就是说语法功能，也就是说这些词的存在是为了让整个句子的生成过程看起来更像一个完整体或者说更符合语言规范。T. Griffiths和M. Steyvers是两个很优秀的学者，他们开发了topic model工具包，并且也有一堆的牛论文。

- D. Blei. **Probabilistic Models of Text and Images**. PhD thesis, U.C. Berkeley, Division of Computer Science, 2004. [[PDF](http://www.cs.princeton.edu/~blei/papers/Blei2004.pdf)]
   Blei的博士论文，我至今还没有看完，因为一直纠结在那个Varitional inference的推导。自己责备一下自己。

- D. Blei, A. Ng, and M. Jordan. **Latent Dirichlet allocation**. Journal of Machine Learning Research, 3:993–1022, January 2003. [A shorter version appeared in NIPS 2002]. [[PDF](http://www.cs.princeton.edu/~blei/papers/BleiNgJordan2003.pdf)] [[code](http://www.cs.princeton.edu/~blei/lda-c/)]
        LDA的第一篇文章，不算很好读懂。初次阅读时，一般会遇到可交换性、variational inference、simplex等等细节问题。经典中的经典。

- D. Blei and P. Moreno. **Topic segmentation with an aspect hidden Markov model**. In Proceedings of the 24th annual international ACM SIGIR conference on Research and development in information retrieval, pages 343–348. ACM Press, 2001. [[PDF](http://www.cs.princeton.edu/~blei/papers/BleiMoreno2001.pdf)]
   SIGIR中的一篇关于分段的论文。其实分段这个事情在现实中需求量比较大，但是成熟的工具包并不多，或者我不知道。比较好的成熟的算法一般还是基于语 义斜率的变化来计算的。在次召唤下懂这方面的大牛推荐几个好用的工具。与分段关联很紧密的一个问题就是网页正文抽取，同样也是这个问题，发论文的多，但是 实际release出来代码的很少。比较著名的，如VIPS，但是我没有用过。昨天发现VIPS的作者原来也是一个巨牛的中国人，Deng Cai。之前是清华学生，现在师从Jiawei Han，各种牛会议和牛期刊发了N多的文章。在此膜拜一下。




————————————————————————————————————————————————————————




# 二、NLP一些开源项目

本节主要来源知乎问答：[目前常用的自然语言处理开源项目/开发包有哪些？](https://www.zhihu.com/question/19929473)









2016年3月31日更新，在THULAC新增Python版本分词器，欢迎使用。

**中文词法分析**
[THULAC：一个高效的中文词法分析工具包](https://link.zhihu.com/?target=http%3A//thulac.thunlp.org/)
包括中文分词、词性标注功能。已经提供C++、Java、Python版本。

**中文文本分类**
[THUCTC: 一个高效的中文文本分类工具](https://link.zhihu.com/?target=http%3A//thuctc.thunlp.org/)
提供高效的中文文本特征提取、分类训练和测试功能。

**THUTag: 关键词抽取与社会标签推荐工具包**
[GitHub - YeDeming/THUTag: A Package of Keyphrase Extraction and Social Tag Suggestion](https://link.zhihu.com/?target=https%3A//github.com/YeDeming/THUTag/)
提供关键词抽取、社会标签推荐功能，包括TextRank、ExpandRank、Topical PageRank（TPR）、Tag-LDA、Word Trigger Model、Word Alignment Model等算法。

**PLDA / PLDA+: 一个高效的LDA分布式学习工具包**
[https://code.google.com/archive/p/plda/](https://link.zhihu.com/?target=https%3A//code.google.com/archive/p/plda/)

**知识表示学习**
知识表示学习工具包
[GitHub - Mrlyk423/Relation_Extraction: Knowledge  Base Embedding](https://link.zhihu.com/?target=https%3A//github.com/mrlyk423/relation_extraction)
包括TransE、TransH、TransR、PTransE等算法。

考虑实体描述的知识表示学习算法
[GitHub - xrb92/DKRL: Representation Learning of Knowledge Graphs with Entity Descriptions](https://link.zhihu.com/?target=https%3A//github.com/xrb92/DKRL)

**词表示学习**
跨语言词表示学习算法
[Learning Cross-lingual Word Embeddings via Matrix Co-factorization](https://link.zhihu.com/?target=http%3A//nlp.csai.tsinghua.edu.cn/~lzy/src/acl2015_bilingual.html)

主题增强的词表示学习算法
[GitHub - largelymfs/topical_word_embeddings: A demo code for topical word embedding](https://link.zhihu.com/?target=https%3A//github.com/largelymfs/topical_word_embeddings)

可解释的词表示学习算法
[GitHub - SkTim/OIWE: Online Interpretable Word Embeddings](https://link.zhihu.com/?target=https%3A//github.com/SkTim/OIWE)

考虑字的词表示学习算法
[GitHub - Leonard-Xu/CWE](https://link.zhihu.com/?target=https%3A//github.com/Leonard-Xu/CWE)
**网络表示学习**
文本增强的网络表示学习算法
[GitHub - albertyang33/TADW: code for IJCAI2015 paper "Network Representation Learning with Rich Text Information"](https://link.zhihu.com/?target=https%3A//github.com/albertyang33/TADW)









一整套文本挖掘流水线GATE：[http://gate.ac.uk/](https://link.zhihu.com/?target=http%3A//gate.ac.uk/)
你可以按照它的要求向其中添加组件， 完成自己的nlp任务
我在的项目组曾经尝试过使用， 虽然它指出组件开发， 但是灵活性还是不高， 所以我们自己又开发了一套流水线。

国内一个NLP工具： 哈工大LTP：[http://ir.hit.edu.cn/](https://link.zhihu.com/?target=http%3A//ir.hit.edu.cn/)
这个是一个较完善的流水线了， 不说质量怎么样， 它提供分词、语义标注、 句法依赖、 实体识别。 虽然会出现错误的结果， 但是， 找不到更好的了。

中科院分词ICTCLAS
一个比较权威的分词器， 相信你最后会选择它作为项目的分词工具， 虽然本身存在很多问题， 但是我找不到更好的开源项目了。

微软分词MOSS
当然这个是不开源的， 但是分词非常准， 但是悲剧的是它将分词和实体识别同时完成了， 而且分词（在它提供的工具中）不提供词性标注。

句法分析 Stanford Parser
这个据说非常不能用， 在中文方面， 试试吧。

以上都是成品， 下面是一些算法开发包：

比较新的序列标注算法CRF的开源项目： CRF++
经典模型SVM： svm-light 和 lib svm



复旦深度网络中文自然语言处理工具FudanDNN-NLP2.0，目前支持中文分词、自定义词典、命名识别、词性分析、语义分析、文本规范化等功能。可从[http://homepage.fudan.edu.cn/zhengxq/deeplearning/](https://link.zhihu.com/?target=http%3A//homepage.fudan.edu.cn/zhengxq/deeplearning/)下载




————————————————————————————————————






延伸一：LDA的发展历史



本节来源于知乎Live：[关于LDA, pLSA, SVD, Word2Vec的一些看法](https://zhuanlan.zhihu.com/p/21377575)




Topic Model的发展忽然转向了“我们需要更多的Topic”。之前，大家弄个几百个topic就非常高兴了，这个时候忽然有人说，我们要几百万个Topic。据说是因为google有个系统能学50万topic，非常NB。另外大家希望，topic多了就能学习出更多小的topic，表达更小众的语义。于是群众们开始干了。一开始大家是拿LDA开刀的。于是乎，LDA从12年开始，经历了SparseLDA, AliasLDA, LightLDA, WarpLDA的发展道路，到了15年底，已经能非常快的学100万topic了，而且这个快是靠直接降低理论的时间复杂度实现的，代码写的更好只是起了辅助作用。
- SparseLDA利用了如果topic很多，那么当模型快收敛时，一个word其实只会属于很少的topic，然后利用稀疏性来加速了算法。但这个算法有个致命的缺陷，就是初始化时，模型并不稀疏，因此迭代的前几轮会非常慢。当然充满智慧的群众发明了一堆奇技淫巧部分解决了这个问题。
- AliasLDA是优化了Gibbs Sampling采样的时间复杂度，利用Alias Table让对K个topic采样的时间复杂度从O(K)降低到O(1)
- LightLDA修改了采用的分布，把原来基于一个word doc在topic上联合分布的采样过程，改成了2个交替进行的独立采样过程，一个只依赖word，另一个只依赖doc。
- WarpLDA做了更多的工程级别的优化，让LightLDA更快。

但后来，LDA一直没有大规模的火起来。这是因为不幸的又遇到了深度学习这股风，google在2013年提出了word2vec。word2vec本身不是一个很深的模型，他其实就是一个一层的模型。但这个模型的nb之处在于，他是频率学派的，支持梯度法优化，因此这一层可以插入到DL的网络中作为一层，和其他层一起做end-to-end的优化，LDA就没法这么搞。这样，大量的有监督的问题，用上word2vec后，效果顿时就超过LDA了。




——————————————————————————




# 延伸二：《LDA漫游指南》第二季






《LDA漫游指南》寻找了一个经典之作：LDA作为案例分析，从来没有一个资料或书籍能像漫游指南这样认真的详细解剖一个模型或算法，吉布斯采样已不再能满足我们的求知欲，《LDA漫游指南》作者发现国内的LDA资料均无出现变分推断技术时，决定做普罗米修斯盗火，将变分贝叶斯的技术引入国内。

如果说Collapsed Gibbs Sampling版本的LDA犹如《新约圣经》，那么Variational Bayes法推导的LDA就像《旧约圣经》一样，如果说一个人学习LDA却不知道LDA的变分推断方法，就像一个基督徒不读《圣经》一样，这仍然称不上了解LDA，溯本求源是我们应有的态度。LDA的变分贝叶斯法既是LDA模型的起点，也是一个启发新技术的契机。
地址:[http://yuedu.baidu.com/ebook/d0b441a8ccbff121dd36839a](http://yuedu.baidu.com/ebook/d0b441a8ccbff121dd36839a)
![](https://img-blog.csdn.net/20170319174824313)



































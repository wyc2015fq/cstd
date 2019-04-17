# 系统学习NLP（十二）--文本表示综述 - 工作笔记 - CSDN博客





2019年03月10日 11:23:02[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：73








文本表示，简单的说就是不将文本视为字符串，而视为在数学上处理起来更为方便的向量（也就是文本特征抽取）。而怎么把字符串变为向量，就是文本表示的核心问题。

文本表示，基于类型分为：
- 长文本表示
- 短文本表示(句子)
- 词表示

关于文本表示，研究者从不同的角度出发，提出大量的文本表示模型。本文重点梳理现有模型，大致分为三类，即基于向量空间模型、基于主题模型和基于神经网络的方法。

## 基于向量空间模型的方法

向量空间模型是将文本表示成实数值分量所构成的向量，一般而言，每个分量对应一个词项，相当于将文本表示成空间中的一个点。向量不仅可以用来训练分类器，而且计算向量之间的相似度可以度量文本之间的相似度。

### 词袋子模型（bag of words）

词袋子模型是一种非常经典的文本表示。顾名思义，它就是将字符串视为一个 “装满字符（词）的袋子” ，袋子里的词语是随便摆放的。而两个词袋子的相似程度就以它们重合的词及其相关分布进行判断。

举个例子，对于句子：“我们这些傻傻的路痴走啊走，好不容易找到了饭店的西门”。

我们先进行分词，将所有出现的词储存为一个词表。然后依据 “词语是否出现在词表中” 可以将这句话变为这样的向量：

[1,0,1,1,1,0,0,1,…]

词表：[我们，你们，走，西门，的，吃饭，旅游，找到了,…]

其中向量的每个维度唯一对应着词表中的一个词。可见这个向量的大部分位置是0值，这种情况叫作“稀疏”。为了减少存储空间，我们也可以只储存非零值的位置。

**词袋子模型的优缺点**

优点：

简单，方便，快速在语料充足的前提下，对于简单的自然语言处理任务效果不错。如文本分类。

缺点：

其准确率往往比较低。

凡是出现在文本中的词一视同仁，不能体现不同词在一句话中的不同的重要性。

无法关注词语之间的顺序关系，这是词袋子模型最大的缺点。如“武松打老虎”跟“老虎打武松”在词袋子模型中是认为一样的。

### **对词袋子模型的改进：TF-IDF**

即向量的维度对应词表的大小，对应维度使用TF-IDF计算。向量空间模型的优点是简单明了，向量维度意义明确，效果不错，但也存在明显的缺点，其一，维度随着词表增大而增大，且向量高度稀疏；其二，无法处理“一义多词”和“一词多义”问题。

在向量空间模型中，文档集合相当于表示成高维稀疏矩阵，如图1中所示，文档集合矩阵的维度是N*V，其中N是文档数目，V是词表的大小。为了更好的提升文本的语义表示能力，有人提出通过矩阵分解的方法，对高维稀疏矩阵进行分解，最为著名的便是潜在语义分析（Latent semantic analysis, LSA），具体而言，LSA会构建一个文档与词项的共现矩阵，矩阵的元素一般通过TFIDF计算得到，最终通过奇异值分解的方法对原始矩阵降维，可以得到文档向量和词项向量。如图1所示，分解后，每个文档可以用k维向量表示（k << V），相当于潜在语义分析实现对文档的低维语义表示。但是，以上过程通过矩阵分解得到，空间中维度的物理含义不明确，无法解释。

![](https://img-blog.csdn.net/20181008224632437?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图1  LSA

补充：TFIDF原理

如果某个词或短语在一篇文章中出现的频率高（TF），并且在其他文章中很少出现，则认为此词或者短语具有很好的类别区分能力，适合用来分类。TFIDF实际上是：TF * IDF，TF词频(Term Frequency)，IDF逆向文件频率(Inverse Document Frequency)。TF表示词条在文档d中出现的频率。IDF的主要思想是：如果包含词条t的文档越少，也就是n越小，IDF越大，则说明词条t具有很好的类别区分能力。如果某一类文档C中包含词条t的文档数为m，而其它类包含t的文档总数为k，显然所有包含t的文档数n=m+k，当m大的时候，n也大，按照IDF公式得到的IDF的值会小，就说明该词条t类别区分能力不强。但是实际上，如果一个词条在一个类的文档中频繁出现，则说明该词条能够很好代表这个类的文本的特征，这样的词条应该给它们赋予较高的权重，并选来作为该类文本的特征词以区别与其它类文档。这就是IDF的不足之处. 在一份给定的文件里，**词频**（term frequency，TF）指的是某一个给定的词语在该文件中出现的频率。这个数字是对**词数**(term count)的归一化，以防止它偏向长的文件。（同一个词语在长文件里可能会比短文件有更高的词数，而不管该词语重要与否。）对于在某一特定文件里的词语来说，它的重要性可表示为：

![](https://img-blog.csdnimg.cn/20190310114337161.jpg)

以上式子中分子是该词在文件中的出现次数，而分母则是在文件中所有字词的出现次数之和。

**逆向文件频率**（inverse document frequency，IDF）是一个词语普遍重要性的度量。某一特定词语的IDF，可以由总文件数目除以包含该词语之文件的数目，再将得到的商取以10为底的[对数](https://baike.baidu.com/item/%E5%AF%B9%E6%95%B0)得到：

![](https://img-blog.csdnimg.cn/20190310114401418.jpg)

其中
- 
|D|：语料库中的文件总数

- 
：包含词语的文件数目（即的文件数目）如果该词语不在语料库中，就会导致分母为零，因此一般情况下使用数据平滑作为分母。![idf公式分母](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=08d7696548fbfbedd859317d48f1f78e/6f061d950a7b0208298cc7bb61d9f2d3572cc827.jpg)idf公式分母


然后再计算TF与IDF的乘积。

![](https://img-blog.csdnimg.cn/20190310114515901.jpg)

某一特定文件内的高词语频率，以及该词语在整个文件集合中的低文件频率，可以产生出高权重的TF-IDF。因此，TF-IDF倾向于过滤掉常见的词语，保留重要的词语。



## 基于主题模型的方法

上面提到LSA算法通过线性代数中奇异值分解实现文档映射到低维语义空间里的向量，但是空间中每一个维度是没有明确物理意义的，主题模型尝试从概率生成模型的角度实现文本的表示，每一个维度是一个“主题（[topic](https://www.baidu.com/s?wd=topic&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)）”，这个主题通常是一组词的聚类，因此可以通过主题大概猜测每个维度所代表的语义，具有一定的解释性。

最早的主题模型pLSA （probabilistic LSA），假设文档具有主题分布，文档中的词从主题对应的词分布中抽取。如图2所示，以d表示文档，w表示词，z表示主题（隐变量），则文档和词的联合概率p(d, w)的生成过程可表示为：

![](https://img-blog.csdn.net/20181008223730817?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中p(z|d)和p(w|z)作为参数可以用EM算法进行学习。然而，pLSA没有假设主题的的先验分布，导致参数随训练文档的数目呈线性增长，参数空间很大。

![](https://img-blog.csdn.net/20181008224643421?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图2  pLSA

于是，有人提出更加完善的主题的模型LDA（Latent Dirichlet allocation）,可以认为pLSA体现频率学派的思想，而LDA是贝叶斯学派的思想，LDA在已有的模型上中的2个多项式分布引入了狄利克雷先验分布，从而解决pLSA中存在的问题。如图3所示，每个文档的主题分布为多项式分布Mult(θ)，其中θ从狄利克雷先验分布Dir(α)抽取，同理，对于主题的词分布为多项式分布Mult(Φ)，参数Φ也是从狄利克雷先验Dir(β)抽取得到。

![](https://img-blog.csdn.net/20181008224649758?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图3  LDA

基于主题模型的方法，引入“主题”概念，具有一定的物理意义，从而得到文档的主题分布表示。当然，主题模型很存在一些问题，比如训练参数多导致训练时间长，对短文本的建模效果不好，主题数目的设置需要人工设定可能导致不合理。后来，也有很多人提出改进的方法，在这就不一一赘述了。

## 基于神经网络的方法

现今，基于神经网络的方法受到广泛关注，各种各样的模型被相继提出，本节总结其中最具有代表性的模型，将其分为三类：
- 第一类，基于词向量合成的模型，该类方法仅是在词向量基础上简单合成；
- 第二类，基于RNN/CNN的模型，该类方法利用更复杂的深度学习模型对文本进行建模；
- 第三类，基于注意力机制的模型，在已有神经网络模型基础上，引入注意力机制，提升文本建模效果。

### 基于词向量合成的模型

2003年Bengio等人开始使用神经网络来做语言模型的工作，尝试得到词的低维、稠密的向量表示，2013年Mikolov等人提出简化的模型，即著名的Word2Vec，包含两个模型CBOW和Skip-gram，前者通过窗口语境预测目标词出现的概率，后者使用目标词预测窗口中的每个语境词出现的概率。语义上相似或相关的词，得到的表示向量也相近，这样的特性使得Word2Vec获得巨大成功。

后来，Mikolov等人又延续Word2Vec的思想，提出Doc2Vec，核心思想是将文档向量当作“语境”，用来预测文档中的词。Doc2Vec算法可以得到词向量和文档向量。如图4所示，两个算法的思想基本一致。

![](https://img-blog.csdn.net/20181008224702439?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图4  Word2Vec和Doc2Vec比较

其实，也可以通过最简单的合成方式实现从词向量到句子向量的表示，fastText就是这样简单有效的模型，如图5所示，输入层是词向量，然后通过把句子里的词向量平均就得到句子的表示，最后送到分类器中。不过，输入端会另外补充一些n-gram信息来捕捉局部序列信息。fastText是线性分类模型，实验表明在诸多“简单”文本分类任务中表现出色，且具备训练速度非常快的优点，所以可以成为很好的Baseline。

![](https://img-blog.csdn.net/20181008224708649?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图5  fastText模型

### 基于RNN/CNN的模型

自然语言中，词构成句子，句子构成文档，有很多工作尝试合理表示词向量同时，也有很多模型被提出来建模句子和文档，其中最常见的网络结构便是LSTM和CNN。

2014年Kim提出基于卷积神经网络的文本分类，如图6所示，输入是句子对应的词向量矩阵，经过一层卷积层和一层Max Pooling层，得到句子的表示，送入到全连接层，最后softmax输出。卷积神经网络擅长提取重要的局部特征，在文本分类中，可以理解为不同大小的卷积核在提取不同n-gram特征。一般认为，卷积神经网络无法考虑长距离的依赖信息，且没有考虑词序信息，在有限的窗口下提取句子特征，会损失一些语义信息。

![](https://img-blog.csdn.net/20181008224714763?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图6 CNN网络用于文本分类

针对CNN的不足之处，LSTM和GRU等循环神经网络因为擅长捕捉长距离信息，所以也被大家尝试用来文本表示。如图7所示，图中利用双向LSTM来建模输入句子，输入是句子的词向量，输入至BiLSTM中进行序列建模。最后句子表示，可以尝试两种方法，其一，选择最后的hidden state用来表示句子；其二，所有hidden state的平均用于表示句子。

![](https://img-blog.csdn.net/20181008224721235?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图7 BiLSTM用于文本表示

刚才分析到，CNN擅长提取局部特征，而LSTM擅长捕捉长距离信息，不难想到，有人尝试结合两种网络的优点，提出RCNN用于文本建模。如图8所示，整个网络结构主要有两部分组成，循环结构和Max Pooling。循环结构，可以理解为，在已有词向量为输入的基础上，通过双向RNN网络学习每一个词的左、右上下文信息，接着将三部分(left context, word embedding, right context)表示向量拼接起来，作为句子中每一个词的表示，然后使用变换得到中间语义表示；Max Pooling层，采用element-wise的max pooling方式，可以从变长的输入中得到固定的句子表示。

![](https://img-blog.csdn.net/20181008224757940?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图8  RCNN用于文本表示

### 基于注意力机制的模型

注意力被认为是一种有效选择信息的方式，可以过滤掉大量与任务无关的信息，最先在机器翻译任务中被提出，解决seq2seq中encoder过程把源序列映射成固定大小的向量存在“损失”信息的情况。紧接着，Attention被推广到各种NLP任务中，文本表示任务当然不例外。这里，主要介绍两种Attention的形式，Hierarchical Attention 和 Self-Attention。

**Hierarchical Attention**网络结构，如图9所示，该模型基于两个基本假设，其一，文档是分层结构的，词构成句子，句子构成文档；其二，文档中不同词或句子提供的信息量不一样的，该模型适合用来表示包含多个句子的文档的表示问题。模型主要由word encoder和sentence encoder，以及相应的attention组成，word encoder部分用于得到句子的表示，该层的输入为句子的词向量，经过双向GRU后得到中间表示，word attention部分对中间表示按attention值进行加权得到此句最终的句子表示；sentence encoder部分用于得到文档的表示，使用word encoder部分得到文档中所有句子的表示后，将此作为sentence encoder部分的输入，类比word encoder的计算，可以最终得到文档的表示。简言之，利用Hierarchical Attention结构，一层词输入得到句子表示，一层句子输入得到文档表示。即使文档长度较长，但是在注意力的作用下，依然可以较好的捕捉到有效的特征信息，忽略无意义的输入。

![](https://img-blog.csdn.net/20181008224802506?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图9  Hierarchical Attention

**Self-Attention**网络结构，如图10所示，大多数神经网络模型将文本表示成一维的向量，但是此模型通过二维矩阵来表示句子，包括两部分，一部分是双向的LSTM，另一部分是自注意力机制，自注意力机制实现对双向LSTM中所有的隐藏状态以不同权重的方式线形组合，每一次组合获得句子的一部分表示，多次组合便得到矩阵表示（图中矩阵M）。

![](https://img-blog.csdn.net/2018100822480859?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1pKTDAxMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图10  Self-Attention




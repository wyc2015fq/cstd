
# NLP︱高级词向量表达（三）——WordRank（简述） - 素质云笔记-Recorder... - CSDN博客

2017年02月04日 00:10:22[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：9033



> 如果说FastText的词向量在表达句子时候很在行的话，GloVe在多义词方面表现出色，那么wordRank在相似词寻找方面表现地不错。

> 其是通过Robust Ranking来进行词向量定义。

> 相关paper：
> [WordRank: Learning Word Embeddings via Robust ](https://arxiv.org/pdf/1506.02761v3.pdf)

> Ranking
> [ ](https://arxiv.org/pdf/1506.02761v3.pdf)

> 相关博客：
> [https://rare-technologies.com/wordrank-embedding-crowned-is-most-similar-to-king-not-word2vecs-canute/?from=singlemessage&isappinstalled=0\#](https://rare-technologies.com/wordrank-embedding-crowned-is-most-similar-to-king-not-word2vecs-canute/?from=singlemessage&isappinstalled=0#)

> [
](https://arxiv.org/pdf/1506.02761v3.pdf)
> 查询king关键词，WordRank 、 word2vec、fastText三者效果对比：
![这里写图片描述](https://img-blog.csdn.net/20170203234206632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170203234206632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> WordRank 、fastText都相对比较精准“crowned”、“throne”，而不是word2vec的“Canute”

> [
](https://img-blog.csdn.net/20170203234206632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> .

> 1、wordRank，与 word2vec、fastText三者对比

> 来源博客：
> [《WordRank embedding: “crowned” is most similar to “king”, not word2vec’s “Canute”》](https://rare-technologies.com/wordrank-embedding-crowned-is-most-similar-to-king-not-word2vecs-canute/?from=singlemessage&isappinstalled=0#)

> 根据Ranking算法得到的wordRank，与 word2vec、fastText三者对比
![这里写图片描述](https://img-blog.csdn.net/20170203234615868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170203234615868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 在不同的项目需求上，有着不同的训练精度，在句法表达上，fastText更好，而在单个词语的相似性等内容表达上wordRank是三者中最好的。

> 同时随着数据量的增加精度呈现增长的趋势。

> .

> .

> 2、wordRank，与 word2vec、GloVe三者对比

> 来源paper：
> [WordRank: Learning Word Embeddings via Robust Ranking](https://arxiv.org/pdf/1506.02761v3.pdf)

> （1）精度
![这里写图片描述](https://img-blog.csdn.net/20170204000612940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 左图使用数据：WS-353 word similarity benchmark

> 右图使用数据：Google word analogy benchmark

> 从两个数据集的效果来看，wordRank效果极佳

> （2）词类比与词相似比较
![这里写图片描述](https://img-blog.csdn.net/20170204000822066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170204000822066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 相似词的寻找方面极佳，词类比方面不同数据集有不同精度。

> [
](https://img-blog.csdn.net/20170204000822066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 结论：

> [
](https://img-blog.csdn.net/20170204000822066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 1.在语义类比中
> ，三种模型在低频词语上表现相对较差，在高频词语上表现效果较好；

> 2.在语法类比中
> ，FastText优于Word2Vec和WordRank 。FastText模型在低频词语上表现的相当好，但是当词频升高时，准确率迅速降低，而WordRank和Word2Vec在很少出现和很频繁出现的词语上准确率较低；

> 3.FastText在综合类比中表现更好
> ，最后一幅图说明整体类比结果与语法类比的结果比较相似，因为语法类比任务的数量远远多于语义类比，所以在综合结果中语法类比任务的结果占有更大的权重；

> 4、WordRank
> 在语义类比任务上效果优于其他两种模型，而FastText在语法类比上效果更好。值得一提的是，如果用WordRank模型生成两个集合（词集合和上下文集合），WordRank使用它们词向量的内积对他们之间的关系建模，内积和他们之间的关系是直接成比例的，如果该词和上下文越相关，内积就会越大

> [
](https://img-blog.csdn.net/20170204000822066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 综上，WordRank更适合语义类比，FastText更适合不同语料库下所有词频的语法类比。

> [
](https://img-blog.csdn.net/20170204000822066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> 高级词向量三部曲：

> 1、
> [NLP︱高级词向量表达（一）——GloVe（理论、相关测评结果、R&python实现、相关应用）](http://blog.csdn.net/sinat_26917383/article/details/54847240)

> 2、
> [NLP︱高级词向量表达（二）——FastText（简述、学习笔记）](http://blog.csdn.net/sinat_26917383/article/details/54850933)

> 3、
> [NLP︱高级词向量表达（三）——WordRank（简述）](http://blog.csdn.net/sinat_26917383/article/details/54852214)

> 4、其他NLP词表示方法paper:
> [从符号到分布式表示NLP中词各种表示方法综述](https://arxiv.org/pdf/1702.00764v1.pdf)



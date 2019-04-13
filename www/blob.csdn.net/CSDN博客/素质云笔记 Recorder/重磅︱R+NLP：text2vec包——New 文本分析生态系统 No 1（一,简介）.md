
# 重磅︱R+NLP：text2vec包——New 文本分析生态系统 No.1（一,简介） - 素质云笔记-Recorder... - CSDN博客

2016年11月14日 18:35:50[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6704所属专栏：[R语言与自然语言处理](https://blog.csdn.net/column/details/13670.html)



**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————

词向量的表示主流的有两种方式，一种当然是耳熟能详的google的word2vec，还有一类就是GloVe。那么前面一类有三个开源的包，后面这一类我倒是看到得不多，恰好是在我关注了许久的一个包里面有，它就是text2vec啦。该包提供了一个强大API接口，能够很好地处理文本信息。
本包是由C++写的，流处理器可以让内存得到更好的利用，一些地方是用RcppParallel包进行并行化加乘，同时兼容各个系统的服务器。如果没有并行的地方，大家也可以自己用foreach包来自己加速。

这个生态系统也是笔者见到过的，R语言里面最好、最全面的包了，包含了很多类型的算法以及成熟的应用。而且包的介绍十分详尽，很棒！！
系列文章：
[重磅︱R+NLP：text2vec包——New 文本分析生态系统 No.1（一,简介）](http://blog.csdn.net/sinat_26917383/article/details/53161863)
[R+NLP︱text2vec包——BOW词袋模型做监督式情感标注案例（二,情感标注）](http://blog.csdn.net/sinat_26917383/article/details/53260117)
**R+NLP︱text2vec包——四类文本挖掘相似性指标 RWMD、cosine、Jaccard 、Euclidean （三,相似距离）**

————————————————————————————————

**该包的四大功能**：
1、快速文本表达方式。文档可以以多种方式表达，单独词组、n-grams、特征hashing化的方法等。
2、GloVe词向量表达
3、LDA主题模型，LDA（latent dirichlet allocation）、LSA(latent sematic analysis)，那么这个就是继lda、topicmodels两大包之后，第三个有主题模型功能的包啦~前面两个包可以参考我的另外博客：
[R语言︱LDA主题模型——最优主题数选取(topicmodels)+LDAvis可视化(lda+LDAvis)](http://blog.csdn.net/sinat_26917383/article/details/51547298)
4、距离计算。cosine距离、jaccard距离、Relaxed word mover's distance（在最近的Kaggle比赛中很有效）、Euclidean距离。

————————————————————————————————————
一、快速文本表达
文档可以以多种方式表达，单独词组、n-grams、特征hashing化的方法等。
一般来说文本分析的步骤有以下三个步骤：
1、第一步：把内容表达成为文档-词组矩阵（document-term矩阵，DTM）或者词组共现矩阵（term-co-occurrence矩阵，TCM），换言之第一步就是在文档之上创造一个词条地图。
2、第二步：找个模型在DTM上进行拟合，有LDA、文本分类等
3、第三步：在验证集上进行验证

————————————————————————————————
二、GloVe词向量表达
在Tomas Mikolov等人开发了word2vec词向量表达工具之后，一大批衍生方法由此崛起与发展，其中一种斯坦福大学的GloVe（Global Vectors for word representation）就是一篇非常好的文献。
主要是在词语共现矩阵下因式分解。经过代码优化GloVe性能提高了2-3倍，是通过单精度浮点运算。
————————————————————————————————————————
三、LDA主题模型
LDA主题模型是基于lda包开发的（Jonathan Chang）,在下次发布的时候该主题模型的引擎就会嵌入到lda包之中，目前text2vec开发模型要比lda快2倍，比topicmodels包快10倍。LSA模型是基于irlab包。
——————————————————————————————————————
四、距离计算

Cosine
Jaccard
Relaxed Word Mover's Distance
Euclidean

还在研究中，后续加更...

——————————————————————————————————————

参考文献：
1、[重磅︱文本挖掘深度学习之word2vec的R语言实现](http://blog.csdn.net/sinat_26917383/article/details/51319312)
2、[语言︱LDA主题模型——最优主题数选取(topicmodels)+LDAvis可视化(lda+LDAvis)](http://blog.csdn.net/sinat_26917383/article/details/51547298)
3、[自然语言处理︱简述四大类文本分析中的“词向量”（文本词特征提取）](http://blog.csdn.net/sinat_26917383/article/details/52162589)
4、N[LP︱句子级、词语级以及句子-词语之间相似性（相关名称：文档特征、词特征、词权重）](http://blog.csdn.net/sinat_26917383/article/details/52174672)
5、[NLP︱R语言实现word2vec（词向量）经验总结（消除歧义、词向量的可加性）](http://blog.csdn.net/sinat_26917383/article/details/52229338)





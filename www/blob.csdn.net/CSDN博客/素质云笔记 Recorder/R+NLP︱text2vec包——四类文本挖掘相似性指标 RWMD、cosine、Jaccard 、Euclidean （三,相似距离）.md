# R+NLP︱text2vec包——四类文本挖掘相似性指标 RWMD、cosine、Jaccard 、Euclidean （三,相似距离） - 素质云笔记/Recorder... - CSDN博客





2016年11月22日 15:35:45[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：10029
所属专栏：[R语言与自然语言处理](https://blog.csdn.net/column/details/13670.html)











**要学的东西太多，无笔记不能学~~ 欢迎关注公众号，一起分享学习笔记，记录每一颗“贝壳”~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


   在之前的开篇提到了text2vec**，笔者将其定义为R语言文本分析"No.1"，她是一个文本分析的生态系统。**笔者在学习之后发现开发者简直牛！基于分享精神，将自学笔记记录出来。开篇内容参考：

[重磅︱R+NLP：text2vec包——New 文本分析生态系统 No.1（一,简介）](http://blog.csdn.net/sinat_26917383/article/details/53161863)

[R+NLP︱text2vec包——BOW词袋模型做监督式情感标注案例（二,情感标注）](http://blog.csdn.net/sinat_26917383/article/details/53260117)




   text2vec中包括了四大类距离：


- Cosine
- Jaccard
- Relaxed Word Mover's Distance
- Euclidean




—————————————————————————————————————————————






# 一、文本分析常见三大距离——cosine/jaccard/Euclidean




      1、Euclidean，欧氏距离是最易于理解的一种距离计算方法，源自欧氏空间中两点间的距离公式。


      2、cosine， 有没有搞错，又不是学几何，怎么扯到夹角余弦了？各位看官稍安勿躁。几何中夹角余弦可用来衡量两个向量方向的差异，[机器学习](http://lib.csdn.net/base/2)中借用这一概念来衡量样本向量之间的差异。


      3、jaccard，杰卡德相似系数，  两个集合A和B的交集元素在A，B的并集中所占的比例，称为两个集合的杰卡德相似系数，用符号J(A,B)表示。


      不展开描述，因为很常见，其中文本分析还有Hamming距离

      详情戳我转的博客：[机器学习中应用到的各种距离介绍（附上Matlab代码）](http://blog.csdn.net/sinat_26917383/article/details/52101425)




—————————————————————————————————————————————





# 二、文本分析相似性距离——Relaxed Word Mover's Distance（RWMD）




      本节内容转载于IR实验室，《刘龙飞 From Word Embeddings To Document Distance》。







##       1、Earth Mover’s Distance (EMD)




      Earth Mover’s Distance (EMD)，和欧氏距离一样，他们都是一种距离度量的定义，可以用来测量某分布之间的距离。EMD主要应用在图像处理和语音信号处理领域。


![](https://img-blog.csdn.net/20161122151611241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





      EMD 实际上是[线性规划](http://zh.wikipedia.org/zh/%E7%BA%BF%E6%80%A7%E8%A7%84%E5%88%92)中[运输问题](http://en.wikipedia.org/wiki/Transportation_problem)的最优解。首先，简要描述下运输问题。我们假设这个例子是从多个工厂运输货物到多个仓库。在上图左侧，P从在P1 到 Pm代表m座工厂，工厂Pi有重量为wPi的货物。在上图右侧，Q从Q1到Qn代表n个仓库，仓库Qj最大容量为wQj。




      货物之间没有什么区别，都是同一类东西。每个仓库都希望装尽可能多的货物。如何尽可能高效把所有货物(实际上不一定是所有货物，部分也OK)从P运送到Q，就是运输问题的优化目标。在本例中，P、Q都是离散的，那么EMD可以用运输问题的Hungarian算法来计算它们之间的距离。挖个坑而已，这里不具体讨论。








## 2、EMD算法在自然语言处理领域的应用




      通过词嵌入（Word Embedding），我们可以得到词语的分布式低维实数向量表示，我们可以计算词语之间的距离，即我们可以得到dij，因此可以将EMD引入自然语言处理领域。




      Matt等人[2]将词嵌入与EMD相联系，用来度量文档距离。提出了WMD（word mover’s distance）算法，以及WCD（word centroid distance）、RWMD（relaxed word mover’s distance）两种牺牲精度降低复杂度的算法。

![](https://img-blog.csdn.net/20161122151708556?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       如何将一个文档变为分布P这种形式？

      用归一化的词袋模型（nBOW， normalized bag-of-words）表示。其中P1表示词语，用![](http://ir.dlut.edu.cn/Uploads/ue/image/20160103/6358741242817189956834129.png)计算该词的权重，其中![](http://ir.dlut.edu.cn/Uploads/ue/image/20160103/6358741242842149993291086.png)表示词语i在文档中出现了![](http://ir.dlut.edu.cn/Uploads/ue/image/20160103/6358741242842149993291086.png)次，P1的特征量用该词语的词向量表示。一个nBOW文档向量是比较稀疏的，因为大部分的词语不会出现在该文档中，且在生成文档向量时，去掉的停用词。用欧式距离计算词语与词语之间的距离。



      假定数据集中只有两个文档，则这两个文档生成的归一化nBOW向量如下图所示。

![](https://img-blog.csdn.net/20161122151757961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


      转移量![](http://ir.dlut.edu.cn/Uploads/ue/image/20160103/6358741260155060407742896.png)用矩阵T表示，生成的矩阵T如下图所示


![](https://img-blog.csdn.net/20161122151824135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](http://ir.dlut.edu.cn/Uploads/ue/image/20160103/6358741260237740558154509.png)表示词语i有多少转移到了词语j，![](http://ir.dlut.edu.cn/Uploads/ue/image/20160103/6358741260259580597152976.png)

      综上，Matt等人提出了WMD算法，WMD是EMD的一个特殊形式。
为了降低模型的计算复杂度，Matt等人提出了WCD和RWMD两个算法，这两个算法是WMD的两个不同下限，通过降低精度来降低计算复杂度。



      Relaxed word moving distance(RWMD)，通过放松限制条件，得到WMD的下限。通过去掉条件2，保留条件1。




作者：From word embeddings to document distances, Matt J. kusner et al.

链接：http://ir.dlut.edu.cn/news/detail/362




—————————————————————————————————————————————





# 三、R语言︱text2vec包中的四大相似性距离




      text2vec环境中有这么四个求距离的函数：

      sim2(x, y, method)：分别计算x*y个相似性；


      psim2(x, x, method)：平行地求数据的相似性，x个相似性；


      dist2(x, y, method)：跟sim2相反，分别计算x*y个距离；


      pdist2(x, x, method)，平行地求数据的距离，x个距离。

      注意到的是，sim2与psim2一个是生成了x*y个数值，一个是生成了x个数值，区别显而易见。




来看一个官方案例：

      先准备好数据，还是之前的那个电影简述数据。



```
library(stringr)
library(text2vec)
data("movie_review")
# select 500 rows for faster running times
movie_review = movie_review[1:500, ]
prep_fun = function(x) {
  x %>% 
    # make text lower case
    str_to_lower %>% 
    # remove non-alphanumeric symbols
    str_replace_all("[^[:alnum:]]", " ") %>% 
    # collapse multiple spaces
    str_replace_all("\\s+", " ")
}
movie_review$review_clean = prep_fun(movie_review$review)
```



定义两个文档：



```
doc_set_1 = movie_review[1:300, ]
it1 = itoken(doc_set_1$review_clean, progressbar = FALSE)

# specially take different number of docs in second set
doc_set_2 = movie_review[301:500, ]
it2 = itoken(doc_set_2$review_clean, progressbar = FALSE)
```




## 1、Jaccard距离








```
dtm1 = create_dtm(it1, vectorizer)
dtm2 = create_dtm(it2, vectorizer)
d1_d2_jac_sim = sim2(dtm1, dtm2, method = "jaccard", norm = "none")
```
      生成了一个300*200的相似性矩阵。

```
dtm1_2 = dtm1[1:200, ]
dtm2_2 = dtm2[1:200, ]
d1_d2_jac_psim = psim2(dtm1_2, dtm2_2, method = "jaccard", norm = "none")
str(d1_d2_jac_psim)
```
      生成了一个200个数值的相似性系数。






## 2、cosine距离





`d1_d2_cos_sim = sim2(dtm1, dtm2, method = "cosine", norm = "l2")`



## 3、Euclidean 距离



```
x = dtm_tfidf_lsa[1:300, ]
y = dtm_tfidf_lsa[1:200, ]
m1 = dist2(x, y, method = "euclidean")
```







## 4、RWMD距离






```
data("movie_review")
tokens = movie_review$review %>%
  tolower %>%
  word_tokenizer
v = create_vocabulary(itoken(tokens)) %>%
  prune_vocabulary(term_count_min = 5, doc_proportion_max = 0.5)
corpus = create_corpus(itoken(tokens), vocab_vectorizer(v, skip_grams_window = 5))
dtm = get_dtm(corpus)
tcm = get_tcm(corpus)
glove_model = GloVe$new(word_vectors_size = 50, vocabulary = v, x_max = 10)
wv = glove_model$fit(tcm, n_iter = 10)
rwmd_model = RWMD(wv)
rwmd_dist = dist2(dtm[1:10, ], dtm[1:100, ], method = rwmd_model, norm = 'none')
```
















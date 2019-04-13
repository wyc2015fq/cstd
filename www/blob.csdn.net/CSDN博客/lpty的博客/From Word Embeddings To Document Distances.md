
# From Word Embeddings To Document Distances - lpty的博客 - CSDN博客

2018年05月02日 11:33:17[lpty](https://me.csdn.net/sinat_33741547)阅读数：2082所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、概念
词移距离来自于论文：[From Word Embeddings To Document Distances](http://proceedings.mlr.press/v37/kusnerb15.html)
从论文题目可以看出，主要是基于词向量的工作。w2v构造了一个词与词之间衡量相似度的方法，但是对于句子或者文档来说，单纯基于词的衡量方法在一定程度上遗漏了句子的信息，而WMD参考了EMD的内容，成为计算文档相似度的一种方法。
## 二、思想
如果希望反映出文档的内在含义，一个比较直观的方法是把文档表示为词向量的某个总结，如加权和形式：
$\sum_{i,j}^{n}M_{i,j}D_{i,j}$
其中矩阵D为词i与词j之间的语义矩阵，这里可以用词向量之间的欧式距离来表示。而矩阵M则是权重矩阵。
那么权重需要怎样确定呢？下面先介绍一下线性规划中的运输问题：
![这里写图片描述](https://img-blog.csdn.net/20180502110609619?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180502110400138?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180502110458283?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
WMD与上面的运输问题最优解很相似，实际上可以看成文档1转化为文档2所需要付出的总代价，词与词之间的移动代价由语义距离决定，而权重矩阵则是最优化所需要求解的值。
如果将上述最优化问题转到WMD中，我们需要把文档看成一个分布，类似仓库中每个仓库不同的货物量，比如使用归一化的词频特征。
之后同样通过解最优化问题，得到的最小移动代价便是词移距离。
## 三、实战
简单的伪代码如下：
`from pyemd import emd
doc1 = [token1, token2, ...]
doc2 = [token1, token2, ...]
vocab_len = len(set(doc1 + doc2))
# 计算词之间的语义距离
distance_matrix = compute_cosine_between_token(doc1, doc2)
distance_matrix.shape == (vocab_len, vocab_len)
# 计算归一化的词频概率
d1 = compute_normalized_word_freq(doc1)
d2 = compute_normalized_word_freq(doc2)
d1.shape == d2.shape == (vocab_len)
# 计算词移距离
wmd = emd(d1, d2, distance_matix)`

[
  ](https://img-blog.csdn.net/20180502110400138?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
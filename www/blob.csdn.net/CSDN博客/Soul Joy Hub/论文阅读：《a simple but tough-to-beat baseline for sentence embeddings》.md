# 论文阅读：《a simple but tough-to-beat baseline for sentence embeddings》 - Soul Joy Hub - CSDN博客

2018年10月24日 19:17:19[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：185


[https://openreview.net/pdf?id=SyK00v5xx](https://openreview.net/pdf?id=SyK00v5xx)

原文：[http://www.hankcs.com/nlp/cs224n-sentence-embeddings.html](http://www.hankcs.com/nlp/cs224n-sentence-embeddings.html)

## 句子Embedding动机

虽然这节课一直在讲词向量可以编码词的意思，但自然语言处理真正关心的是整个句子的意思。

![hankcs.com 2017-06-07 下午4.07.53.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fgcpaucd63j31c60pg42b.jpg)

如果我们能够拿到句子的向量表示，则可以方便地用内积计算相似度：

![hankcs.com 2017-06-07 下午4.08.24.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fgcpbde95dj31cg0s6gpw.jpg)

还可以在这些句子向量之上构建分类器做情感分析：

![hankcs.com 2017-06-07 下午4.08.42.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fgcpbqtnyjj315w0ty780.jpg)

## 已有方法

具体怎么由词向量到句向量呢？有很多种方法，比如词袋模型中简单地线性运算：

![hankcs.com 2017-06-07 下午4.09.14.png](http://wx4.sinaimg.cn/large/006Fmjmcly1fgcpc94vhpj31ck0ecq54.jpg)

在后面的课程中，将会用recurrent neural network、recursive neural network，CNN来做同样的事情。

![hankcs.com 2017-06-07 下午4.09.29.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fgcpcljpmaj319e0i047r.jpg)

## 新方法

但今天要介绍的这篇普林斯顿大学的论文却剑走偏锋，采用了一种简单的无监督方法。这种方法简单到只有两步：

![hankcs.com 2017-06-07 下午4.09.53.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fgcpd1w9pwj31840y0ths.jpg)

第一步，对句子中的每个词向量，乘以一个独特的权值。这个权值是一个常数$\alpha$除以$\alpha$与该词语频率的和，也就是说高频词的权值会相对下降。求和后得到暂时的句向量。

然后计算语料库所有句向量构成的矩阵的第一个主成分$u$，让每个句向量减去它在$u$上的投影（类似PCA）。其中，一个向量$v$在另一个向量$u$上的投影定义如下：

$$\text{Proj}_u v=\frac{u u^Tv}{\Vert u \Vert^2}$$

## 概率论解释

其原理是，给定上下文向量，一个词的出现概率由两项决定：作为平滑项的词频，以及上下文：

![hankcs.com 2017-06-07 下午4.10.26.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fgcpdhvoozj312o0vm0yf.jpg)

其中第二项的意思是，有一个平滑变动的上下文随机地发射单词。

## 效果

![hankcs.com 2017-06-07 下午4.13.14.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fgcph0xexsj31d00so4im.jpg)

在句子相似度任务上超过平均水平，甚至超过部分复杂的模型。在句子分类上效果也很明显，甚至是最好成绩。


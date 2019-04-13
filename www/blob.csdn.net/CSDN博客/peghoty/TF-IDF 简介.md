
# TF-IDF 简介 - peghoty - CSDN博客


2014年03月10日 22:58:36[皮果提](https://me.csdn.net/peghoty)阅读数：3597



假设我们手头有大量的文档（或网页）, 通常我们会比较关心以下几个问题：1. 每一个文档的关键词（或主题词）包括哪些？
2. 给定一个（或一组）关键词，与这个（或组）词最相关的文档是哪一个？
3. 给定一个文档，哪个（或哪些）文档与它具有最大的相似度呢？回答上述三个问题的关键是：对于一个给定的词和一个给定的文档，定义一个可以用来衡量该词对该文档相关性（或重要性）的指标。那么，如何定义这样的一个指标呢？
       本文将简要介绍**词频-逆文档频度**（Term Frequency - Inverse Document Frequency，TF-IDF）技术，它是一种用于资讯检索与文本挖掘的常用加权技术，可以用来评估一个词对于一个文档集或语料库中某个文档的重要程度。


![](https://img-blog.csdn.net/20140310225019312)![](https://img-blog.csdn.net/20140310224526250)![](https://img-blog.csdn.net/20140310224537671)
作者: peghoty
出处:[http://blog.csdn.net/itplus/article/details/20958185](http://blog.csdn.net/itplus/article/details/20958185)
欢迎转载/分享, 但请务必声明文章出处.



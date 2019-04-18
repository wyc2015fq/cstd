# 【数据平台】python语言NLP库Gensim初识 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年11月01日 15:39:07[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：374
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、基本介绍

Gensim是一款开源的第三方Python工具包，用于从原始的非结构化的文本中，无监督地学习到文本隐层的主题向量表达。它支持包括TF-IDF，LSA，LDA，和word2vec在内的多种主题模型算法，支持流式训练，并提供了诸如相似度计算，信息检索等一些常用任务的API接口。

官网：https://radimrehurek.com/gensim/

           https://pypi.python.org/pypi/gensim/

安装：pip install gensim




2、基本概念：


- 语料（Corpus）：一组原始文本的集合，用于无监督地训练文本主题的隐层结构。语料中不需要人工标注的附加信息。在Gensim中，Corpus通常是一个可迭代的对象（比如列表）。每一次迭代返回一个可用于表达文本对象的稀疏向量。
- 向量（Vector）：由一组文本特征构成的列表。是一段文本在Gensim中的内部表达。
- 稀疏向量（Sparse Vector）：通常，我们可以略去向量中多余的0元素。此时，向量中的每一个元素是一个(key, value)的tuple。
- 模型（Model）：是一个抽象的术语。定义了两个向量空间的变换（即从文本的一种向量表达变换为另一种向量表达）。



        gensim 是一个通过衡量词组（或更高级结构，如整句或文档）模式来挖掘文档语义结构的工具。gensim 以“文集”——文本文档的集合——作为输入，并生成一个“向量”来表征该文集的文本内容，从而实现语义挖掘。该向量表示可被用于训练一个“模型”。模型是从数据生成不同特征的算法集，通常是更为语义化的。




3、功能：预料处理、文本向量化、文本相似度等。





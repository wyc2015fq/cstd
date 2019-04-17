# word2vec的python应用 - 博客堂 - CSDN博客





2017年11月27日 11:28:59[最小森林](https://me.csdn.net/u012052268)阅读数：1306








# word2vec的python应用


- [word2vec的python应用](#word2vec的python应用)- [简单介绍词向量的由来](#1-简单介绍词向量的由来)
- [Word2Vec的python应用](#2-word2vec的python应用)- [1 安装](#21-安装)
- [2 语料的下载](#22-语料的下载)
- [3 模型的训练](#23-模型的训练)
- [4 保存模型](#24-保存模型)
- [5 模型的使用](#25-模型的使用)

- [代码总结](#3-代码总结)




词嵌入（Word Embedding）是一项非常重要且应用广泛的技术，可以将文本和词语转换为机器能够接受的数值向量，这里我们详细讨论其概念和实现。

## 1 简单介绍词向量的由来

语言的表示主要有两种：符号主义和分布式表示。

符号主义中典型的代表是Bag of words，即词袋模型。

分布式表示中典型的代表是Word Embedding，即词嵌入。

具体的由来请参考： [词嵌入来龙去脉 word embedding、word2vec](http://blog.csdn.net/u012052268/article/details/77170517)

## 2 Word2Vec的python应用

gensim是一款开源的Python工具包，用于从非结构化文本中无监督地学习文本隐层的主题向量表示，支持包括TF-IDF、LSA、LDA和Word2Vec在内的多种主题模型算法，并提供了诸如相似度计算、信息检索等一系列常用任务的API接口。

以下是gensim官网对于其中Word2Vec模型的介绍，[http://radimrehurek.com/gensim/models/word2vec.html](http://radimrehurek.com/gensim/models/word2vec.html)，里面提供了和Word2Vec相关的完整使用文档。

### 2.1 安装

如果没有gensim的话，使用pip安装即可。
`pip install gensim`
### 2.2 语料的下载

另外，gensim仅提供了Word2Vec的模型实现，训练词向量的另一个必须条件是足够大的文本语料。这里我们将要使用的是中文维基百科语料，直接下载即可： 
[https://pan.baidu.com/s/1qXKIPp6](https://pan.baidu.com/s/1qXKIPp6)

提取密码为 kade。
下载之后可以在Sublime中打开并查看其内容，文件名和后缀名可以不用在意，因为Sublime支持打开任意类型的文本文件。其中每一行是一条维基百科，即一项词条对应的百科内容，并且已经完成了分词处理。

### 2.3 模型的训练

```
# 加载包
from gensim.models import Word2Vec
from gensim.models.word2vec import LineSentence

# 训练模型
sentences = LineSentence('wiki.zh.word.text')

# size：词向量的维度
# window：上下文环境的窗口大小
# min_count：忽略出现次数低于min_count的词
model = Word2Vec(sentences, size=128, window=5, min_count=5, workers=4)
```

### 2.4 保存模型

```
# 保存模型
model.save('word_embedding_128')

# 如果已经保存过模型，则直接加载即可
# 前面训练并保存的代码都可以省略
# model = Word2Vec.load("word_embedding_128")
```

### 2.5 模型的使用

```python
# 返回两个词语之间的相关度
model.similarity(u'男人',  u'女人')
```

```python
# 返回和一个词语最相关的多个词语以及对应的相关度
items = model.most_similar(u'中国')
for item in items:
    # 词的内容，词的相关度
    print item[0], item[1]
```

除此之外，gensim中的Word2Vec还实现了多项NLP功能，例如从多个词中找出和其他词相关性相对更弱的一个，以及根据给定的三个词类比推理出第四个词等，详细使用方法可以参考官方完整文档。

## 3 代码总结

```python
# 加载包
from gensim.models import Word2Vec
from gensim.models.word2vec import LineSentence

# 训练模型
sentences = LineSentence('wiki.zh.word.text')
# size：词向量的维度
# window：上下文环境的窗口大小
# min_count：忽略出现次数低于min_count的词
model = Word2Vec(sentences, size=128, window=5, min_count=5, workers=4)

# 保存模型
model.save('word_embedding_128')

# 如果已经保存过模型，则直接加载即可
# 前面训练并保存的代码都可以省略
# model = Word2Vec.load("word_embedding_128")

# 使用模型
# 返回和一个词语最相关的多个词语以及对应的相关度
items = model.most_similar(u'中国')
for item in items:
    # 词的内容，词的相关度
    print item[0], item[1]

# 返回两个词语之间的相关度
model.similarity(u'男人',  u'女人')
```

结果：

```
更 0.9998828768730164
买车 0.9998500347137451
东西 0.9998452663421631
车型 0.9998400807380676
简单 0.9998311996459961
反正 0.999820351600647
作 0.9998154640197754
不会 0.999814510345459
肯定 0.9998050928115845
求 0.999803900718689
```





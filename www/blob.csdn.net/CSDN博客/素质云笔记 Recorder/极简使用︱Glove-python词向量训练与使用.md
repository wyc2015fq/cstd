# 极简使用︱Glove-python词向量训练与使用 - 素质云笔记/Recorder... - CSDN博客





2018年10月12日 16:27:34[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2872








glove/word2vec/fasttext目前词向量比较通用的三种方式，其中word2vec来看，在gensim已经可以极快使用（可见：[python︱gensim训练word2vec及相关函数与功能理解](https://blog.csdn.net/sinat_26917383/article/details/69803018)）

官方glove教程比较啰嗦，可能还得设置一些参数表，操作不是特别方便。

笔者使用的时候，用的是一款比较省力/封装的。

官方glove:[https://github.com/stanfordnlp/GloVe](https://github.com/stanfordnlp/GloVe)

笔者使用：[https://github.com/maciejkula/glove-python](https://github.com/maciejkula/glove-python)

## 1 安装：

```
pip install glove_python
```

## 2 训练：

具体函数细节可参考：[github](https://github.com/maciejkula/glove-python/blob/749494290fdfd24379dcc2e244c583ee61808634/glove/corpus.py)

生成嵌入是一个两步过程： 从语料库中生成一个匹配矩阵，然后用它生成嵌入矩阵。 Corpus 类有助于从令牌的interable构建一个语料库。

还支持基本的pagragraph向量。 在word空间vector段落向量是在单词向量空间中嵌入段落，这样段落表示就接近于它所包含的单词，因为在语料库中的单词的频率调整。 在训练模型上通过调用 transform_paragraph 方法来训练单词嵌入后，可以得到这些结果。

其中段落向量是词向量的平均可见(该模块没有教程，笔者就不做过多分析了)：

```
paragraph_vector = np.mean(self.word_vectors[word_ids], axis=0)
```

### （1）准备数据集

```
from __future__ import print_function
import argparse
import pprint
import gensim
from glove import Glove
from glove import Corpus

sentense = [['你','是','谁'],['我','是','中国人']]
corpus_model = Corpus()
corpus_model.fit(sentense, window=10)
#corpus_model.save('corpus.model')
print('Dict size: %s' % len(corpus_model.dictionary))
print('Collocations: %s' % corpus_model.matrix.nnz)
```

其中`corpus_model.fit(corpus, window=10, ignore_missing=False)`

ignore_missing代表如果出现OOV的词，该如何处理。

### （2）训练

```
glove = Glove(no_components=100, learning_rate=0.05)
glove.fit(corpus_model.matrix, epochs=10,
          no_threads=1, verbose=True)
glove.add_dictionary(corpus_model.dictionary)

>>> Performing 10 training epochs with 1 threads
>>> Epoch 0
>>> Epoch 1
>>> Epoch 2
>>> Epoch 3
>>> Epoch 4
>>> Epoch 5
>>> Epoch 6
>>> Epoch 7
>>> Epoch 8
>>> Epoch 9
```

维度no_components，可以与word2vec一起使用。

模型得保存为：`glove.save('glove.model')`

### （3）使用：模型得保存与加载

glove模型保存与加载：

```
glove.save('glove.model')
glove = Glove.load('glove.model')
```

corpus保存与加载：

```
corpus_model.save('corpus.model')
corpus_model = Corpus.load('corpus.model')
```

### （4）使用：求相似词

根据glove求相似词。

```
glove.most_similar('我', number=10)

>>> [('中国人', 0.15130809810072138),
>>>  ('你', 0.0739901044877504),
>>>  ('谁', -0.05137569131012555),
>>>  ('是', -0.08668606334919005)]
```

### （5）使用：词向量矩阵

词向量矩阵

```
# 全部词向量矩阵
glove.word_vectors
# 指定词条词向量
glove.word_vectors[glove.dictionary['你']]
```

语料协同矩阵 corpus coocurrence matrix

```
corpus_model.matrix.todense().tolist()

>>>     [[0.0, 1.0, 0.5, 0.0, 0.0],
		 [0.0, 0.0, 1.0, 1.0, 1.0],
		 [0.0, 0.0, 0.0, 0.0, 0.0],
		 [0.0, 0.0, 0.0, 0.0, 0.5],
		 [0.0, 0.0, 0.0, 0.0, 0.0]]
```

由corpus_model引出得语料协同矩阵




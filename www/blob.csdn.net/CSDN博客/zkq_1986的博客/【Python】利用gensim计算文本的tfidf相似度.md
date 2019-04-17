# 【Python】利用gensim计算文本的tfidf相似度 - zkq_1986的博客 - CSDN博客





2018年10月16日 15:02:09[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：852








python代码：

```python
# -*- coding: utf-8 -*-
from gensim import corpora, models, similarities
import logging
from collections import defaultdict

logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)

# 文档
documents = ["Human machine interface for lab abc computer applications",
             "A survey of user opinion of computer system response time",
             "The EPS user interface management system",
             "System and human system engineering testing of EPS",
             "Relation of user perceived response time to error measurement",
             "The generation of random binary unordered trees",
             "The intersection graph of paths in trees",
             "Graph minors IV Widths of trees and well quasi ordering",
             "Graph minors A survey"]

# 1.分词，去除停用词
stoplist = set('for a of the and to in'.split())
texts = [[word for word in document.lower().split() if word not in stoplist] for document in documents]
print('-----------1----------')
print(texts)
# [['human', 'machine', 'interface', 'lab', 'abc', 'computer', 'applications'], ['survey', 'user', 'opinion', 'computer', 'system', 'response', 'time'],
# ['eps', 'user', 'interface', 'management', 'system'], ['system', 'human', 'system', 'engineering', 'testing', 'eps'], ['relation', 'user', 'perceived
# ', 'response', 'time', 'error', 'measurement'], ['generation', 'random', 'binary', 'unordered', 'trees'], ['intersection', 'graph', 'paths', 'trees'],
# ['graph', 'minors', 'iv', 'widths', 'trees', 'well', 'quasi', 'ordering'], ['graph', 'minors', 'survey']]

# 2.计算词频
frequency = defaultdict(int)  # 构建一个字典对象
# 遍历分词后的结果集，计算每个词出现的频率
for text in texts:
    for token in text:
        frequency[token] += 1
# 选择频率大于1的词
texts = [[token for token in text if frequency[token] > 1] for text in texts]
print('-----------2----------')
print(texts)
# [['human', 'interface', 'computer'], ['survey', 'user', 'computer', 'system', 'response', 'time'], ['eps', 'user', 'interface', 'system'], ['system',
# 'human', 'system', 'eps'], ['user', 'response', 'time'], ['trees'], ['graph', 'trees'], ['graph', 'minors', 'trees'], ['graph', 'minors', 'survey']]

# 3.创建字典（单词与编号之间的映射）
dictionary = corpora.Dictionary(texts)
# print(dictionary)
# Dictionary(12 unique tokens: ['time', 'computer', 'graph', 'minors', 'trees']...)
# 打印字典，key为单词，value为单词的编号
print('-----------3----------')
print(dictionary.token2id)
# {'human': 0, 'interface': 1, 'computer': 2, 'survey': 3, 'user': 4, 'system': 5, 'response': 6, 'time': 7, 'eps': 8, 'trees': 9, 'graph': 10, 'minors': 11}

# 4.将要比较的文档转换为向量（词袋表示方法）
# 要比较的文档
test_doc = ["Human computer interaction",'A survey of user opinion of computer system response time']
# 将文档分词并使用doc2bow方法对每个不同单词的词频进行了统计，并将单词转换为其编号，然后以稀疏向量的形式返回结果
new_vecs = [dictionary.doc2bow(doc.lower().split()) for doc in test_doc]
print('-----------4----------')
# print(new_vecs)
# [[(0, 1), (2, 1)]

# 5.建立语料库
# 将每一篇文档转换为向量
corpus = [dictionary.doc2bow(text) for text in texts]
print('-----------5----------')
print(corpus)
# [[[(0, 1), (1, 1), (2, 1)], [(2, 1), (3, 1), (4, 1), (5, 1), (6, 1), (7, 1)], [(1, 1), (4, 1), (5, 1), (8, 1)], [(0, 1), (5, 2), (8, 1)], [(4, 1), (6, 1), (7, 1)], [(9, 1)], [(9, 1), (10, 1)], [(9, 1), (10, 1), (11, 1)], [(3, 1), (10, 1), (11, 1)]]

# 6.初始化模型
# 初始化一个tfidf模型,可以用它来转换向量（词袋整数计数）表示方法为新的表示方法（Tfidf 实数权重）
tfidf = models.TfidfModel(corpus)
# 测试
test_doc_bow = [(0, 1), (1, 1)]
print('-----------6----------')
print(tfidf[test_doc_bow])
# [(0, 0.7071067811865476), (1, 0.7071067811865476)]

print('-----------7----------')
# 将整个语料库转为tfidf表示方法
corpus_tfidf = tfidf[corpus]
for doc in corpus_tfidf:
    print(doc)

# 7.创建索引
index = similarities.MatrixSimilarity(corpus_tfidf)

print('-----------8----------')
# 8.相似度计算
new_vec_tfidf_ls = [tfidf[new_vec] for new_vec in new_vecs]  # 将要比较文档转换为tfidf表示方法

print('-----------9----------')
# 计算要比较的文档与语料库中每篇文档的相似度
sims = [index[new_vec_tfidf] for new_vec_tfidf in new_vec_tfidf_ls]
print(sims)
```

print(sims)的输出：

[array([0.81649655, 0.31412902, 0.        , 0.3477732 , 0.        ,

       0.        , 0.        , 0.        , 0.        ], dtype=float32), array([0.25648525, 1.        , 0.27067134, 0.23313783, 0.7071068 ,

       0.        , 0.        , 0.        , 0.27910084], dtype=float32)]




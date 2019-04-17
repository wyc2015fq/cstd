# 比赛必备 ︱ 省力搞定三款词向量训练 + OOV词向量问题的可性方案 - 素质云笔记/Recorder... - CSDN博客





2018年10月31日 21:48:09[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2612








本篇为资源汇总，一些NLP的比赛在抽取文本特征的时候会使用非常多的方式。
- 传统的有：TFIDF/LDA/LSI等
- 偏深度的有：word2vec/glove/fasttext等
- 还有一些预训练方式：elmo / bert

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031144917191.png)




### 文章目录
- [1 之前的几款词向量介绍与训练帖子](#1__17)
- [2  极简训练glove/word2vec/fasttext](#2__gloveword2vecfasttext_37)
- [2.1 word2vec的训练与简易使用](#21_word2vec_39)
- [2.2 glove的训练与简易使用](#22_glove_85)
- [2.3 fasttext的训练与简易使用](#23_fasttext_141)
- [2.4 elmo 预训练模型](#24_elmo__203)
- [2.5 BERT预训练模型](#25_BERT_228)
- [2.6 已有的中文的词向量举例](#26__244)
- [2.6.1 facebook Pre-trained word vectors](#261_facebook_Pretrained_word_vectors_246)
- [2.6.2 [Embedding/Chinese-Word-Vectors](https://github.com/Embedding/Chinese-Word-Vectors) 中文词向量大汇总](#262_EmbeddingChineseWordVectorshttpsgithubcomEmbeddingChineseWordVectors__253)
- [2.6.3  腾讯AI Lab开源大规模高质量中文词向量数据](#263__AI_Lab_259)
- [3 OOV(out of vocabulary，OOV)未登录词向量问题](#3_OOVout_of_vocabularyOOV_270)
- [3.1 fasttext 解决OOV的词向量最佳方案](#31_fasttext_OOV_283)
- [3.2 两个词向量空间对齐](#32__332)




## 1 之前的几款词向量介绍与训练帖子

glove：
[NLP︱高级词向量表达（一）——GloVe（理论、相关测评结果、R&python实现、相关应用）](https://blog.csdn.net/sinat_26917383/article/details/54847240)
[极简使用︱Glove-python词向量训练与使用](https://blog.csdn.net/sinat_26917383/article/details/83029140)
fasttext：
[NLP︱高级词向量表达（二）——FastText（简述、学习笔记）](https://blog.csdn.net/sinat_26917383/article/details/54850933)
[fastrtext︱R语言使用facebook的fasttext快速文本分类算法](https://blog.csdn.net/sinat_26917383/article/details/78367905)
[极简使用︱Gemsim-FastText 词向量训练以及OOV（out-of-word）问题有效解决](https://blog.csdn.net/sinat_26917383/article/details/83041424)
word2vec：
[python︱gensim训练word2vec及相关函数与功能理解](https://blog.csdn.net/sinat_26917383/article/details/69803018)

tfidf：
[sklearn+gensim︱jieba分词、词袋doc2bow、TfidfVectorizer](https://blog.csdn.net/sinat_26917383/article/details/71436563)

## 2  极简训练glove/word2vec/fasttext

### 2.1 word2vec的训练与简易使用

gensim里面可以快速的进行训练word2vec。

```
# 最简单的开始
import gensim
sentences = [['first', 'sentence'], ['second', 'sentence','is']]

# 模型训练
model = gensim.models.Word2Vec(sentences, min_count=1)
    # min_count,频数阈值，大于等于1的保留
    # size，神经网络 NN 层单元数，它也对应了训练算法的自由程度
    # workers=4，default = 1 worker = no parallelization 只有在机器已安装 Cython 情况下才会起到作用。如没有 Cython，则只能单核运行。
```

**几个常用功能的列举：**
- 如何获取词向量？

```
model['computer']  # raw NumPy vector of a word
array([-0.00449447, -0.00310097,  0.02421786, ...], dtype=float32)

model.wv['word']
```
- 如何获取词表？

```
model.wv.vocab.keys()
```
- 如何求相似？

```
y2=model.similarity(u"好", u"还行")     # 比较两个词的相似
print(y2)

for i in model.most_similar(u"滋润"):  # 一个词相似词有哪些
    print i[0],i[1]
```

### 2.2 glove的训练与简易使用

比较快的有一个封装比较好的库[maciejkula/glove-python](https://github.com/maciejkula/glove-python):

```
pip install glove_python
```

该库可以快速训练了，而且与gensim使用方式即为相似，给好评：

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

其中`corpus_model.fit(corpus, window=10, ignore_missing=False)`ignore_missing代表如果出现OOV的词，该如何处理。

来看一下原文描述这个ignore_missing：

> 
whether to ignore words missing from  the dictionary (if it was supplied).Context window distances will be preserved even if out-of-vocabulary words are ignored. If False, a KeyError is raised.


**几个常用功能的列举：**
- 如何获取词向量？

```
# 全部词向量矩阵
glove.word_vectors
# 指定词条词向量
glove.word_vectors[glove.dictionary['你']]
```
- 如何求相似？

```
glove.most_similar('我', number=10)

>>> [('中国人', 0.15130809810072138),
>>>  ('你', 0.0739901044877504),
>>>  ('谁', -0.05137569131012555),
>>>  ('是', -0.08668606334919005)]
```

### 2.3 fasttext的训练与简易使用

fasttext的训练，facebook有自己的训练方式：[facebookresearch/fastText](https://github.com/facebookresearch/fastText)，不过训练起来还挺费劲，对新手不友好。那么gensim在新版本里面已经封装了fasttext，也挺好用，已经满足了基本要求。

```
from gensim.models import FastText
sentences = [["你", "是", "谁"], ["我", "是", "中国人"]]

model = FastText(sentences,  size=4, window=3, min_count=1, iter=10,min_n = 3 , max_n = 6,word_ngrams = 0)
model['你']  # 词向量获得的方式
model.wv['你'] # 词向量获得的方式
```

同时gensim里面既有py版本的，也有c++版本的。

```
# 使用c++ 版本的fasttext
from gensim.models.wrappers.fasttext import FastText as FT_wrapper

# Set FastText home to the path to the FastText executable
ft_home = '/home/chinmaya/GSOC/Gensim/fastText/fasttext'

# train the model
model_wrapper = FT_wrapper.train(ft_home, lee_train_file)

print(model_wrapper)
```

**几个常用功能的列举：**
- 如何获取词向量？

```
model['你']  # 词向量获得的方式
model.wv['你'] # 词向量获得的方式
```
- 如何获取词表？

```
model.wv.vocab
```
- 如何求相似？

```
model.wv.similarity('你', '是')  # 求相似
model.n_similarity(['cat', 'say'], ['dog', 'say'])  # 多个词条求相似
model.most_similar("滋润")  # 求词附近的相似词
```

similarity求两个词之间的相似性；n_similarity为求多个词之间的相似性

其中还可以求词条之间的WMD距离：

```
# !pip3 install pyemd 
model.wmdistance(['cat', 'say'], ['dog', 'say']) # 求词条之间的WMD距离
```

### 2.4 elmo 预训练模型

在ELMo 中，每个单词被赋予一个表示，它是它们所属的整个语料库句子的函数。所述的嵌入来自于计算一个两层双向语言模型（LM）的内部状态，因此得名「ELMo」：Embeddings from Language Models。

笔者在本篇里面记叙了一下自己在之前尝试的时候看到比较好的训练开源项目：
[流水账︱Elmo词向量中文训练过程杂记](https://blog.csdn.net/sinat_26917383/article/details/81913790)

一共有三个中文训练的源头：

（1）可参考：[searobbersduck/ELMo_Chin](https://github.com/searobbersduck/ELMo_Chin)，不过好像过程中有些问题，笔者还没证实原因。

（2）博文：[《如何将ELMo词向量用于中文》](http://www.linzehui.me/2018/08/12/%E7%A2%8E%E7%89%87%E7%9F%A5%E8%AF%86%E7%82%B9/%E5%A6%82%E4%BD%95%E5%B0%86ELMo%E8%AF%8D%E5%90%91%E9%87%8F%E7%94%A8%E4%BA%8E%E4%B8%AD%E6%96%87/)，该教程用glove作为初始化向量，思路如下：- 将预训练的词向量读入
- 修改bilm-tf代码
- option部分
- 添加给embedding weight赋初值
- 添加保存embedding weight的代码

- 开始训练，获得checkpoint和option文件
- 运行脚本，获得language model的weight文件
- 将embedding weight保存为hdf5文件形式
- 运行脚本，将语料转化成ELMo embedding。

（3）[HIT-SCIR/ELMoForManyLangs](https://github.com/HIT-SCIR/ELMoForManyLangs)，哈工大今年CoNLL评测的多国语ELMo，有繁体中文。

### 2.5 BERT预训练模型

BERT预训练笔者未尝试，给出几个开源项目：
**1 [brightmart/bert_language_understanding](https://github.com/brightmart/bert_language_understanding)**

该篇的一个新闻稿：[预训练BERT，官方代码发布前他们是这样用TensorFlow解决的](https://www.jiqizhixin.com/articles/2018-10-30-13)

用tensorflow实现的，下面一个效果图是私人测试，还是挺明显的：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031152438494.png)
2 [codertimo/BERT-pytorch](https://github.com/codertimo/BERT-pytorch)
[新闻稿：最强预训练模型BERT的Pytorch实现（非官方）](https://www.jiqizhixin.com/articles/2018-10-18-14)

3 [google-research/bert](https://github.com/google-research/bert)

### 2.6 已有的中文的词向量举例

#### 2.6.1 facebook Pre-trained word vectors

facebook公开了90种语言的Pre-trained word vectors，使用默认参数，300维度，链接：

[https://github.com/facebookresearch/fastText/blob/master/pretrained-vectors.md](https://github.com/facebookresearch/fastText/blob/master/pretrained-vectors.md)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031151649511.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

#### 2.6.2 [Embedding/Chinese-Word-Vectors](https://github.com/Embedding/Chinese-Word-Vectors) 中文词向量大汇总

This project provides 100+ Chinese Word Vectors (embeddings) trained with different representations (dense and sparse), context features (word, ngram, character, and more), and corpora. One can easily obtain pre-trained vectors with different properties and use them for downstream tasks.

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181031210647496.?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)

#### 2.6.3  腾讯AI Lab开源大规模高质量中文词向量数据

This corpus provides 200-dimension vector representations, a.k.a. embeddings, for over 8 million Chinese words and phrases, which are pre-trained on large-scale high-quality data. These vectors, capturing semantic meanings for Chinese words and phrases, can be widely applied in many downstream Chinese processing tasks (e.g., named entity recognition and text classification) and in further research.

地址：[https://ai.tencent.com/ailab/nlp/embedding.html](https://ai.tencent.com/ailab/nlp/embedding.html)

## 3 OOV(out of vocabulary，OOV)未登录词向量问题

未登录词又称为生词（unknown word），可以有两种解释：一是指已有的词表中没有收录的词；二是指已有的训练语料中未曾出现过的词。在第二种含义下，未登录词又称为集外词（out of vocabulary, OOV），即训练集以外的词。通常情况下将OOV与未登录词看作一回事。

未登录词可以粗略划分为如下几种类型：

①新出现的普通词汇，如博客、房奴、给力等，尤其在网络用语中这种词汇层出不穷。

②专有名词（proper names）。专有名词在早期主要是指人名、地名和组织机构名这三类实体名称。1996年第六届信息理解会议对此进行了扩展，首次提出了命名实体（named entity）的概念，新增了时间和数字表达（日期、时刻、时段、数量值、百分比、序数、货币数量等），并且地名被进一步细化为城市名、州（省）名和国家名称等。

③专业名词和研究领域名称。特定领域的专业名词和新出现的研究领域名称也是造成生词的原因之一，如三聚氰胺、苏丹红、禽流感、堰塞湖等。

④其他专用名词，如新出现的产品名，电影、书籍等文艺作品的名称，等等。
该问题在kaggle的《Toxic Comment Classification Challenge》提供了一些解决办法。

### 3.1 fasttext 解决OOV的词向量最佳方案

其中在《About my 0.9872 single model》中提及fasttext的使用：

Fixed misspellings by finding word vector neighborhoods. Fasttext tool can create vectors for out-of-dictionary words which is really nice. I trained my own fasttext vectors on Wikipedia comments corpus and used them to do this. I also used those vectors as embeddings but results were not as good as with regular fasttext vectors.

如果按照上面的训练方式，也能够快速解决OOV问题吗？在gensim之中训练fasttext:

```
from gensim.models import FastText
sentences = [["你", "是", "谁"], ["我", "是", "中国人"]]
model = FastText(sentences,  size=4, window=3, min_count=1, iter=10,min_n = 3 , max_n = 6,word_ngrams = 0)
model .most_similar("滋润")
```

在gensim之中，直接使用`most_similar`就可以实现，笔者来举几个例子：

案例一：

```
model.most_similar("萌萌的的的哒")

>>> [('胸上', 0.5790194272994995),
 ('萌萌哒', 0.5606832504272461),
 ('西子湖畔', 0.5515443086624146),
 ('门用', 0.5502334833145142),
 ('憨态可掬', 0.5367637872695923),
 ('古筝曲', 0.5365685224533081),
 ('济济一堂', 0.5358039140701294),
 ('雕塑感', 0.5351789593696594),
 ('ハ', 0.5332954525947571),
 ('桃江', 0.5323261022567749)]
```

案例二：

```
model.most_similar("这是一个未登录词")

>>>[('马忠真', 0.6866541504859924),
 ('闻讯', 0.6599311828613281),
 ('手板', 0.6508469581604004),
 ('米仁', 0.6501662731170654),
 ('弹指神功', 0.6470856666564941),
 ('表交', 0.6461164951324463),
 ('和级', 0.645912766456604),
 ('膏厚', 0.6436258554458618),
 ('带弹', 0.643358588218689),
 ('宴請', 0.6427510976791382)]
```

虽然第二个有点无厘头，但是一些比较合理地未登录词会找到比较合理地答案。

使用这个工具可以很快地利用未登录词中的字词片段来找到最相似的词是哪些，然后可以赋值。

### 3.2 两个词向量空间对齐

上面提到的fasttext是解决单个OOV，笔者看到比赛中也有尝试两个词向量集合对齐的方案，比较简单易懂，而且使用的该方案能排在比赛的33rd，应该属于比较合理地方案，来看看[mattmotoki/toxic-comment-classification](https://github.com/mattmotoki/toxic-comment-classification)：

we get a vector for every word in our vocabulary. We can now find the most similar vector in the intersection of the local vocabulary (from this competition) with the external vocabulary (from pretrained embeddings).

```
local = {local_words: local_vectors}
external = {external_words: external_vectors}
shared_words = intersect(local_words, external_words)
missing_words = setdiff(local_words, external_words)
reference_matrix = array(local[w] for w in shared_words).T

for w in missing_words:
     similarity = local[w] * reference_matrix
     most_similar_word = shared_words[argmax(similarity)]
     external[w] = external_vectors[most_similar_word]

return {w: external[w] for w in local_words}
```

With this technique, GloVe performed just as well if not better than the fastText with OOV prediction; LexVec performed slightly worse but added valuable diversity to ensembles.

笔者理解的大致意思就是，A词向量集合—>B词向量集合：
- 先找出A & B 词向量集合都拥有的词shared_words ；
- 找出 A - B，A中B没有的词missing_words ；
- A词向量集合中，共同拥有的词shared_words 的词向量矩阵reference_matrix （标准化）；
- 在missing_words 词中，譬如a词，一一找出与shared_words 词集合最相近的词b；
- 在B词向量集合中，B（a） = B（b），B词向量集合中就有a词的向量了。

笔者说的比较绕口，可以直接看[code](https://github.com/mattmotoki/toxic-comment-classification/blob/master/code/embeddings/analyze_wordvector_similarity.ipynb)，该作者写了：
- 一个一个循环查找；
- 整个missing_words空间一起查找；
- 用torch GPU加速查找

比较适合拿来用，供观众参考。










# sklearn+gensim︱jieba分词、词袋doc2bow、TfidfVectorizer - 素质云笔记-Recorder... - CSDN博客

2017年05月08日 22:24:21[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：21827


分词这块之前一直用R在做，R中由两个jiebaR+Rwordseg来进行分词，来看看python里面的jieba.
之前相关的文章：
[R语言︱文本挖掘之中文分词包——Rwordseg包(原理、功能、详解)](http://blog.csdn.net/sinat_26917383/article/details/51056068)
[R语言︱文本挖掘——jiabaR包与分词向量化的simhash算法（与word2vec简单比较）](http://blog.csdn.net/sinat_26917383/article/details/51068097)
.

---文章目录

---[一、jieba分词功能](#jieba_14)

---[1、主要模式](#1_19)

---[支持自定义词典
.
2、算法](#
2_28)

---[应用一：自定义词典或特定词识别](#_118)

---[应用二：关键词提取](#_156)

---[二、gensim的doc2bow实现词袋模型](#gensimdoc2bow_209)

---[分之一：BOW词袋模型](#BOW_254)

---[分支二：建立TFIDF](#TFIDF_266)

---[情况一：新的句子](#_292)

---[.
情况二：tfidf模型的保存与内容查看](#
tfidf_318)

---[三 sklearn.feature_extraction抽取文本TFIDF特征](#_sklearnfeature_extractionTFIDF_364)

---[3.1 feature_extraction中几种情况](#31_feature_extraction_365)

---[3.2 CountVectorizer与TfidfTransformer测试](#32_CountVectorizerTfidfTransformer_372)

---[3.3 TfidfVectorizer](#33_TfidfVectorizer_442)

---[片段一：tfidf + LDA](#tfidf__LDA_527)

---[tfidf + sklearn的模型](#tfidf__sklearn_552)

---[3.4 HashingVectorizer](#34_HashingVectorizer_565)

---[3.5 错误使用TfidfVectorizer方式，导致分类准确率一直为0.2](#35_TfidfVectorizer02_587)


---一、jieba分词功能

---来源github：

---[https://github.com/fxsjy/jieba](https://github.com/fxsjy/jieba)

---1、主要模式

---支持三种分词模式：

---精确模式，试图将句子最精确地切开，适合文本分析；

---全模式，把句子中所有的可以成词的词语都扫描出来, 速度非常快，但是不能解决歧义；

---搜索引擎模式，在精确模式的基础上，对长词再次切分，提高召回率，适合用于搜索引擎分词。 支持繁体分词

---支持自定义词典

---.

---2、算法

---基于前缀词典实现高效的词图扫描，生成句子中汉字所有可能成词情况所构成的有向无环图 (DAG)

---采用了动态规划查找最大概率路径, 找出基于词频的最大切分组合

---对于未登录词，采用了基于汉字成词能力的 HMM 模型，使用了 Viterbi 算法

---.

---3、主要功能

---jieba.cut

---方法接受三个输入参数: 需要分词的字符串；cut_all 参数用来控制是否采用全模式；HMM 参数用来控制是否使用
   HMM 模型

---jieba.cut_for_search

---方法接受两个参数：需要分词的字符串；是否使用 HMM
   模型。该方法适合用于搜索引擎构建倒排索引的分词，粒度比较细 待分词的字符串可以是 unicode 或 UTF-8 字符串、GBK
   字符串。注意：不建议直接输入 GBK 字符串，可能无法预料地错误解码成 UTF-8

---jieba.Tokenizer(dictionary=DEFAULT_DICT)

---新建自定义分词器，可用于同时使用不同词典。jieba.dt 为默认分词器，所有全局分词相关函数都是该分词器的映射。

---载入词典

---用法： jieba.load_userdict(file_name) \# file_name 为文件类对象或自定义词典的路径
   词典格式和 dict.txt
   一样，一个词占一行；每一行分三部分：词语、词频（可省略）、词性（可省略），用空格隔开，顺序不可颠倒。file_name
   若为路径或二进制方式打开的文件，则文件必须为 UTF-8 编码。 词频省略时使用自动计算的能保证分出该词的词频。

---调整词典。

---使用 add_word(word, freq=None, tag=None) 和 del_word(word)
   可在程序中动态修改词典。
   使用 suggest_freq(segment, tune=True) 可调节单个词语的词频，使其能（或不能）被分出来。
   注意：自动计算的词频在使用 HMM 新词发现功能时可能无效。

---基于 TF-IDF 算法的关键词抽取

---import jieba.analyse
jieba.analyse.extract_tags(sentence, topK=20, withWeight=False, allowPOS=())
sentence 为待提取的文本
topK 为返回几个 TF/IDF 权重最大的关键词，默认值为 20
withWeight 为是否一并返回关键词权重值，默认值为 False
allowPOS 仅包括指定词性的词，默认值为空，即不筛选
jieba.analyse.TFIDF(idf_path=None) 新建 TFIDF 实例，idf_path 为 IDF 频率文件

---基于 TextRank 算法的关键词抽取

---jieba.analyse.textrank(sentence, topK=20, withWeight=False, allowPOS=('ns', 'n', 'vn', 'v')) 直接使用，接口相同，注意默认过滤词性。
jieba.analyse.TextRank() 新建自定义 TextRank 实例

---词性标注

---jieba.posseg.POSTokenizer(tokenizer=None) 新建自定义分词器，tokenizer 参数可指定内部使用的 jieba.Tokenizer 分词器。jieba.posseg.dt 为默认词性标注分词器。
标注句子分词后每个词的词性，采用和 ictclas 兼容的标记法。

---并行分词

---基于 python 自带的 multiprocessing 模块，目前暂不支持 Windows
用法：
jieba.enable_parallel(4) \# 开启并行分词模式，参数为并行进程数
jieba.disable_parallel() \# 关闭并行分词模式

---.

---应用一：自定义词典或特定词识别

---自定义词典加入方式一：

---import jieba
jieba.load_userdict("userdict.txt")

---自定义词典的格式为

---词语-词频-词性

---，其中词频与词性可以没有：

---﻿云计算 5
李小福 2 nr
创新办 3 i
easy_install 3 eng
好用 300
韩玉赏鉴 3 nz
八一双鹿 3 nz
台中
凱特琳 nz
Edu Trust认证 2000

---自定义词加入方式二：

---add_word(word, freq=None, tag=None) 和 del_word(word) 可在程序中动态修改词典。

---suggest_freq(segment, tune=True) 可调节单个词语的词频，使其能（或不能）被分出来。

--->>> print('/'.join(jieba.cut('如果放到post中将出错。', HMM=False)))
如果/放到/post/中将/出错/。
>>> jieba.suggest_freq(('中', '将'), True)
494
>>> jieba.add_word(('中将'), freq = 500)

---.

---应用二：关键词提取

---import jieba.analyse
text = '新媒体运营如何提升自己的写作能力'
\# 添加新词
word = '新媒体运营'
jieba.suggest_freq((word), True)
jieba.add_word(word, freq=100, tag='get')
\# 利用idf进行关键词提取
jieba.analyse.set_idf_path("/mnt/gensim/jieba-master/idf.txt.big")
print ' '.join(jieba.analyse.extract_tags(text, topK=100, withWeight=False, allowPOS=('get','n','ns','vn')))

---jieba.analyse.set_idf_path()

---可以在关键词提取时，进行IDF设置，一般来说，IDF设置大一些，露出机会就多一些。

---add_word()

---此时可以自定义一种词性，这样就可以达到特定领域词的提取。

---此时还可以设置停用词：

---jieba.analyse.set_stop_words(file_name)

---格式为：

---the
of
is
and
to
in
that
we
for
an
are
by
be
as
on
with
can
if
from
which
you

---二、gensim的doc2bow实现词袋模型

---词袋模型不做过多介绍，直接来个案例

---from gensim import corpora, models, similarities
raw_documents = [  
    '0无偿居间介绍买卖毒品的行为应如何定性',  
    '1吸毒男动态持有大量毒品的行为该如何认定',  
    '2如何区分是非法种植毒品原植物罪还是非法制造毒品罪',  
    '3为毒贩贩卖毒品提供帮助构成贩卖毒品罪',  
    '4将自己吸食的毒品原价转让给朋友吸食的行为该如何认定',  
    '5为获报酬帮人购买毒品的行为该如何认定',  
    '6毒贩出狱后再次够买毒品途中被抓的行为认定',  
    '7虚夸毒品功效劝人吸食毒品的行为该如何认定',  
    '8妻子下落不明丈夫又与他人登记结婚是否为无效婚姻',  
    '9一方未签字办理的结婚登记是否有效',  
    '10夫妻双方1990年按农村习俗举办婚礼没有结婚证 一方可否起诉离婚',  
    '11结婚前对方父母出资购买的住房写我们二人的名字有效吗',  
    '12身份证被别人冒用无法登记结婚怎么办？',  
    '13同居后又与他人登记结婚是否构成重婚罪',  
    '14未办登记只举办结婚仪式可起诉离婚吗',  
    '15同居多年未办理结婚登记，是否可以向法院起诉要求离婚'  
]

---载入中文数据以及对应的包，corpora是构造词典的, similarities求相似性可以用得到。

---texts = [[word for word in jieba.cut(document, cut_all=True)] for document in raw_documents]

---将词语进行分词，并进行存储。

---dictionary = corpora.Dictionary(texts)

---寻找整篇语料的词典、所有词，corpora.Dictionary。

---corpus = [dictionary.doc2bow(text) for text in texts]

---建立语料之后，分支一：BOW词袋模型；分支二：建立TFIDF。

---.

---分之一：BOW词袋模型

---由doc2bow变为词袋，输出的格式为：

---[[(0, 1), (1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1)], [(0, 1), (4, 1), (5, 1), (7, 1), (8, 1), (9, 2), (10, 1)], [(0, 1), (3, 1), (4, 1), (5, 1), (6, 1), (7, 1), (10, 1)]]

---例如（9，2）这个元素代表第二篇文档中id为9的单词“silver”出现了2次。

---.

---分支二：建立TFIDF

---tfidf = models.TfidfModel(corpus)

---使用tf-idf 模型得出该评论集的tf-idf 模型

---corpus_tfidf = tfidf[corpus]

---此处已经计算得出所有评论的tf-idf 值

---在TFIDF的基础上，进行相似性检索。

---similarity = similarities.Similarity('Similarity-tfidf-index', corpus_tfidf, num_features=600)

---然后进行similarity检索。

---print(similarity[test_corpus_tfidf_1])  \# 返回最相似的样本材料,(index_of_document, similarity) tuples

---当然其中的test_corpus_tfidf_1需要进行预先处理。先变为dow2bow，然后tfidf

---.

---情况一：新的句子

---new_sensence = "16通过下面一句得到语料中每一篇文档对应的稀疏向量"
test_corpus_1 = dictionary.doc2bow(jieba.cut(raw_documents[1], cut_all=True))
vec_tfidf = tfidf[test_corpus_1]

---利用doc2bow对其进行分割，然后求tfidf模型。输出的结果即为：

---vec_tfidf
Out[82]: 
[(1, 0.09586155438319434),
 (5, 0.1356476941913782),
 (6, 0.09586155438319434),
 (8, 0.1356476941913782),
 (11, 0.19172310876638868),
 (12, 0.38344621753277736),
 (13, 0.38344621753277736),
 (14, 0.38344621753277736),
 (15, 0.16086258119086566),
 (16, 0.38344621753277736),
 (17, 0.38344621753277736),
 (18, 0.38344621753277736)]

---.

---情况二：tfidf模型的保存与内容查看

---for item in corpus_tfidf: 
    print(item) 
tfidf.save("data.tfidf") 
tfidf = models.TfidfModel.load("data.tfidf") 
print(tfidf_model.dfs)

---案例一：

---test_corpus是语料，通过bow2vec函数向量格式更改为：[0.1,0.2,0,0,0,0,…,0.9]

---import jieba
from gensim import corpora, models, similarities
import numpy as np
from tqdm import tqdm
texts = [[word for word in jieba.cut(document, cut_all=True)] for document in test_corpus]
dictionary = corpora.Dictionary(texts)
corpus = [dictionary.doc2bow(text) for text in texts]
tfidf = models.TfidfModel(corpus)
corpus_tfidf =tfidf[corpus]  
def bow2vec(corpus_tfidf,dictionary):
    vec = []
    length = max(dictionary) + 1
    for content in tqdm(corpus_tfidf):
        sentense_vectors = np.zeros(length)
        for co in content:
            sentense_vectors[co[0]]=co[1]
        vec.append(sentense_vectors)
    return vec
svec = bow2vec(corpus_tfidf,dictionary)

---三 sklearn.feature_extraction抽取文本TFIDF特征

---3.1 feature_extraction中几种情况

---主要有四个函数： CountVectorizer、HashingVectorizer、TfidfTransformer、TfidfVectorizer

---CountVectorizer词频矩阵；

---TfidfTransformer，在CountVectorizer基础上转化为TFIDF；

---TfidfVectorizer，以上两个步骤迭代，一起；

---HashingVectorizer

---3.2 CountVectorizer与TfidfTransformer测试

---主函数为：

---class sklearn.feature_extraction.text.CountVectorizer(input=’content’, encoding=’utf-8’, decode_error=’strict’, strip_accents=None, lowercase=True, preprocessor=None, tokenizer=None, stop_words=None, token_pattern=’(?u)\b\w\w+\b’, ngram_range=(1, 1), analyzer=’word’, max_df=1.0, min_df=1, max_features=None, vocabulary=None, binary=False, dtype=<class ‘numpy.int64’>)

---stop_words可以给入停用词；

---ngram_range是不同n-gram组合范围，譬如(1,3)：

---['and', 'and the', 'and the third', 'document', 'first', 'first document', 'is', 'is the', 'is the first', 'is the second', 'is this', 'is this the', 'one', 'second', 'second document', 'second second', 'second second document', 'the', 'the first', 'the first document', 'the second', 'the second second', 'the third', 'the third one', 'third', 'third one', 'this', 'this is', 'this is the', 'this the', 'this the first']

---官方的一个实验：

--->>> from sklearn.feature_extraction.text import CountVectorizer
>>> corpus = [
...     'This is the first document.',
...     'This document is the second document.',
...     'And this is the third one.',
...     'Is this the first document?',
... ]
>>> vectorizer = CountVectorizer()
>>> X = vectorizer.fit_transform(corpus)
>>> print(vectorizer.get_feature_names())
['and', 'document', 'first', 'is', 'one', 'second', 'the', 'third', 'this']
>>> print(X.toarray())  
[[0 1 1 1 0 0 1 0 1]
 [0 2 0 1 0 1 1 0 1]
 [1 0 0 1 1 0 1 1 1]
 [0 1 1 1 0 0 1 0 1]]

---形成词频矩阵之后，可以继续转化为TFIDF：

---\# 在词频矩阵基础上，计算tfidf
transformer = TfidfTransformer()
tfidf_matrix = transformer.fit_transform(count)
print(tfidf_matrix.toarray())

---生成的是每句话的TFIDF

---[[0.         0.43877674 0.54197657 0.43877674 0.         0.
  0.35872874 0.         0.43877674]
 [0.         0.27230147 0.         0.27230147 0.         0.85322574
  0.22262429 0.         0.27230147]
 [0.55280532 0.         0.         0.         0.55280532 0.
  0.28847675 0.55280532 0.        ]
 [0.         0.43877674 0.54197657 0.43877674 0.         0.
  0.35872874 0.         0.43877674]]

---一些属性：

---print(vectorizer.get_feature_names())  
print(vectorizer.vocabulary_)
print(count.toarray())

---分别代表n-grams的词条List；词典；词频矩阵

---['and', 'and the', 'and the third', 'document', 'first', 'first document', 'is', 'is the', 'is the first', 'is the second', 'is this', 'is this the', 'one', 'second', 'second document', 'second second', 'second second document', 'the', 'the first', 'the first document', 'the second', 'the second second', 'the third', 'the third one', 'third', 'third one', 'this', 'this is', 'this is the', 'this the', 'this the first']
{'this the first': 30, 'is the first': 8, 'is the': 7, 'second': 13, 'one': 12, 'the second second': 21, 'this': 26, 'this the': 29, 'and the': 1, 'the first': 18, 'this is the': 28, 'is the second': 9, 'first': 4, 'document': 3, 'and': 0, 'the first document': 19, 'is': 6, 'first document': 5, 'is this': 10, 'the second': 20, 'second second': 15, 'and the third': 2, 'the third one': 23, 'second second document': 16, 'third': 24, 'is this the': 11, 'the third': 22, 'third one': 25, 'the': 17, 'second document': 14, 'this is': 27}
[[0 0 0 1 1 1 1 1 1 0 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0 1 1 1 0 0]
 [0 0 0 1 0 0 1 1 0 1 0 0 0 2 1 1 1 1 0 0 1 1 0 0 0 0 1 1 1 0 0]
 [1 1 1 0 0 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 1 1 1 0 0 0 0 0]
 [0 0 0 1 1 1 1 0 0 0 1 1 0 0 0 0 0 1 1 1 0 0 0 0 0 0 1 0 0 1 1]]

---3.3 TfidfVectorizer

---主函数：

---class sklearn.feature_extraction.text.TfidfVectorizer(input=’content’, encoding=’utf-8’, decode_error=’strict’, strip_accents=None, lowercase=True, preprocessor=None, tokenizer=None, analyzer=’word’, stop_words=None, token_pattern=’(?u)\b\w\w+\b’, ngram_range=(1, 1), max_df=1.0, min_df=1, max_features=None, vocabulary=None, binary=False, dtype=<class ‘numpy.float64’>, norm=’l2’, use_idf=True, smooth_idf=True, sublinear_tf=False)

---参数解读：（参考于：[Python中的TfidfVectorizer参数解析](https://blog.csdn.net/laobai1015/article/details/80451371)）

---tokenizer：callable or None(default)， 当保留预处理和n-gram生成步骤时，覆盖字符串令牌步骤


---ngram_range: tuple(min_n, max_n)，要提取的n-gram的n-values的下限和上限范围，在min_n <= n <= max_n区间的n的全部值


---stop_words：string {‘english’}, list, or None(default)

---如果未english，用于英语内建的停用词列表


---如果未list，该列表被假定为包含停用词，列表中的所有词都将从令牌中删除


---如果None，不使用停用词。max_df可以被设置为范围[0.7, 1.0)的值，基于内部预料词频来自动检测和过滤停用词


---max_df： float in range [0.0, 1.0] or int, optional, 1.0 by default，当构建词汇表时，严格忽略高于给出阈值的文档频率的词条，语料指定的停用词。如果是浮点值，该参数代表文档的比例，整型绝对计数值，如果词汇表不为None，此参数被忽略。


---min_df：float in range [0.0, 1.0] or int, optional, 1.0 by default，当构建词汇表时，严格忽略低于给出阈值的文档频率的词条，语料指定的停用词。如果是浮点值，该参数代表文档的比例，整型绝对计数值，如果词汇表不为None，此参数被忽略。


---max_features： optional， None by default

---如果不为None，构建一个词汇表，仅考虑max_features–按语料词频排序，如果词汇表不为None，这个参数被忽略


---max_features： optional， None by default

---如果不为None，构建一个词汇表，仅考虑max_features–按语料词频排序，如果词汇表不为None，这个参数被忽略


---vocabulary：Mapping or iterable， optional

---也是一个映射（Map）（例如，字典），其中键是词条而值是在特征矩阵中索引，或词条中的迭代器。如果没有给出，词汇表被确定来自输入文件。在映射中索引不能有重复，并且不能在0到最大索引值之间有间断。

---use_idf：boolean， optional

---启动inverse-document-frequency重新计算权重


---smooth_idf：boolean，optional

---通过加1到文档频率平滑idf权重，为防止除零，加入一个额外的文档


---sublinear_tf：boolean， optional

---应用线性缩放TF，例如，使用1+log(tf)覆盖tf


---analyzer=‘char_wb’ or ’word‘

---分别代表单词模式以及字模式

---如果不分，会不断报错：


---ValueError: empty vocabulary; perhaps the documents only contain stop words

---简单案例：

--->>> from sklearn.feature_extraction.text import TfidfVectorizer
>>> corpus = [
...     'This is the first document.',
...     'This document is the second document.',
...     'And this is the third one.',
...     'Is this the first document?',
... ]
>>> vectorizer = TfidfVectorizer()
>>> X = vectorizer.fit_transform(corpus)
>>> print(vectorizer.get_feature_names())
['and', 'document', 'first', 'is', 'one', 'second', 'the', 'third', 'this']
>>> print(X.shape)
(4, 9)

---如何使用段落片段，得到ngrams的搭配短语组合：

---from sklearn.feature_extraction.text import TfidfVectorizer
corpus = ''.join('你好')
vectorizer = TfidfVectorizer(ngram_range = (1,3),analyzer  = 'char_wb'   )
vectorizer.fit_transform([corpus])
print(vectorizer.get_feature_names())
>>> [' ', ' 你', ' 你好', '你', '你好', '你好 ', '好', '好 ']

---从官方案例里面抄几个小片段：

---片段一：tfidf + LDA

---来自案例：

---[《Topic extraction with Non-negative Matrix Factorization and Latent Dirichlet Allocation》](http://scikit-learn.org/stable/auto_examples/applications/plot_topics_extraction_with_nmf_lda.html#sphx-glr-auto-examples-applications-plot-topics-extraction-with-nmf-lda-py)

---\# Use tf-idf features for NMF.
print("Extracting tf-idf features for NMF...")
tfidf_vectorizer = TfidfVectorizer(max_df=0.95, min_df=2,
                                   max_features=n_features,
                                   stop_words='english')
\# Use tf (raw term count) features for LDA.
print("Extracting tf features for LDA...")
tf_vectorizer = CountVectorizer(max_df=0.95, min_df=2,
                                max_features=n_features,
                                stop_words='english')
tf = tf_vectorizer.fit_transform(data_samples)
lda = LatentDirichletAllocation(n_components=n_components, max_iter=5,
                                learning_method='online',
                                learning_offset=50.,
                                random_state=0)

---tfidf + sklearn的模型

---来自案例：《

---[Classification of text documents using sparse features](http://scikit-learn.org/stable/auto_examples/text/plot_document_classification_20newsgroups.html#sphx-glr-auto-examples-text-plot-document-classification-20newsgroups-py)

---》

---vectorizer = TfidfVectorizer(sublinear_tf=True, max_df=0.5,stop_words='english')
X_train = vectorizer.fit_transform(data_train.data)
ch2 = SelectKBest(chi2, k=opts.select_chi2)
X_train = ch2.fit_transform(X_train, y_train)
X_test = ch2.transform(X_test)

---3.4 HashingVectorizer

---参考：

---[HashingVectorizer-CountVectorizer-TfidfVectorizer的区别和联系](https://blog.csdn.net/zhangbaoanhadoop/article/details/79570128)

---用哈希技巧向量化大文本向量，因为之前的算法会出现的问题有：

---语料库越大，词表就会越大，因此使用的内存也越大

---构建词映射需要完整的传递数据集，因此不可能以严格在线的方式拟合文本分类器

---将向量化任务分隔成并行的子任务很不容易实现，因为vocabulary属性要共享状态有一个细颗粒度的同步障碍

---HashingVectorizer也有以下的局限：

---不能反转模型（没有inverse_transform方法），也无法访问原始的字符串表征，因为，进行mapping的哈希方法是单向本性。

---没有提供了IDF权重，因为这需要在模型中引入状态。如果需要的话，可以在管道中添加TfidfTransformer。

---vectorizer = HashingVectorizer(stop_words='english', alternate_sign=False,
                                   n_features=opts.n_features)
    X_train = vectorizer.transform(data_train.data)

---参数与之间的差不多

---3.5 错误使用TfidfVectorizer方式，导致分类准确率一直为0.2

---这边笔者自己的在尝试的时候，发现一开始使用

---TfidfVectorizer

---的时候分类准确率一直为0，也就是最终出来的val_y，预测结果全部为0，所以大概只有20%准确率。

---一般是错误使用TfidfVectorizer造成的。

---\# 第一种方式：训练集/测试集先分开，再fit
tfidf = TfidfVectorizer(**vect_params)
x_train = tfidf.fit_transform(x_train)
x_val = tfidf.fit_transform(x_val)
x_test = tfidf.fit_transform(x_test)
\# 方式二：训练集/测试集一起fit，然后分开
 tfidf_corpus_transform = tfidf.fit_transform(x_train + x_val +x_test )
x_train = tfidf_corpus_transform[:len(x_train)]
x_val = tfidf_corpus_transform[len(x_train):(len(x_train)+len(x_val))]
x_test = tfidf_corpus_transform[(len(x_train)+len(x_val)):]

---以上笔者尝试都不对，正确的需要先全部fit再转化：

---\# fit训练所有语料，将其分别应用在：训练集/测试集/验证集
 tfidf_corpus_transform = tfidf.fit(x_train + x_val +x_test )
x_train = tfidf.transform(x_train)
x_val = tfidf.transform(x_val)
x_test = tfidf.transform(x_test)
\#或者：fit训练 x_train基础语料模型，应用在x_val/x_test 
x_train = tfidf.fit_transform(x_train)
x_val = tfidf.transform(x_val)
x_test = tfidf.transform(x_test)



# 极简使用︱Gemsim-FastText  词向量训练以及OOV（out-of-word）问题有效解决 - 素质云笔记/Recorder... - CSDN博客





置顶2018年10月13日 19:37:17[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5076








glove/word2vec/fasttext目前词向量比较通用的三种方式，之前三款词向量的原始训练过程还是挺繁琐的，这边笔者列举一下再自己使用过程中快速训练的方式。

其中，word2vec可见：[python︱gensim训练word2vec及相关函数与功能理解](https://blog.csdn.net/sinat_26917383/article/details/69803018)

glove可见：[极简使用︱Glove-python词向量训练与使用](https://blog.csdn.net/sinat_26917383/article/details/83029140)
因为是在gensim之中的，需要安装fasttext，可见：
[https://github.com/facebookresearch/fastText/tree/master/python](https://github.com/facebookresearch/fastText/tree/master/python)

```
$ git clone https://github.com/facebookresearch/fastText.git
$ cd fastText
$ pip install .
```




### 文章目录
- [2 、fasttext训练](#2_fasttext_21)
- [2.1 训练主函数](#21__23)
- [2.2 模型的保存与加载](#22__65)
- [2.3 在线更新语料库](#23__99)
- [2.4 c++ 版本的fasttext训练](#24_c_fasttext_117)
- [3 fasttext使用](#3_fasttext_135)
- [3.1 获得词向量](#31__136)
- [3.2 词向量词典](#32__166)
- [3.3 与word2vec 相同的求相似性](#33_word2vec__175)
- [3.4  求词附近的相似词](#34___203)
- [3.5 fasttext自带的OOV功能](#35_fasttextOOV_225)
- [3.5 如何获得fasttext的n-grams词向量](#35_fasttextngrams_251)
- [4 fasttext 与 word2vec的对比](#4_fasttext__word2vec_339)
- [参考资源](#_356)




## 2 、fasttext训练

## 2.1 训练主函数

```
from gensim.models import FastText
sentences = [["你", "是", "谁"], ["我", "是", "中国人"]]

model = FastText(sentences,  size=4, window=3, min_count=1, iter=10,min_n = 3 , max_n = 6,word_ngrams = 0)
model['你']  # 词向量获得的方式
model.wv['你'] # 词向量获得的方式
```

其中FastText主函数为：

```
class gensim.models.fasttext.FastText(sentences=None, corpus_file=None, sg=0, hs=0, size=100, alpha=0.025, window=5, min_count=5, max_vocab_size=None, word_ngrams=1, sample=0.001, seed=1, workers=3, min_alpha=0.0001, negative=5, ns_exponent=0.75, cbow_mean=1, hashfxn=<built-in function hash>, iter=5, null_word=0, min_n=3, max_n=6, sorted_vocab=1, bucket=2000000, trim_rule=None, batch_words=10000, callbacks=())
```

几个参数的含义为：
- 
常规参数：
- model: Training architecture. Allowed values: `cbow`, `skipgram` (Default `cbow`)
- size: Size of embeddings to be learnt (Default 100)
- alpha: Initial learning rate (Default 0.025)
- window: Context window size (Default 5)
- min_count: Ignore words with number of occurrences below this (Default 5)
- loss: Training objective. Allowed values: `ns`, `hs`, `softmax` (Default `ns`)
- sample: Threshold for downsampling higher-frequency words (Default 0.001)
- negative: Number of negative words to sample, for `ns` (Default 5)
- iter: Number of epochs (Default 5)
- sorted_vocab: Sort vocab by descending frequency (Default 1)
- threads: Number of threads to use (Default 12)

- 
fasttext附加参数
- min_n: min length of char ngrams (Default 3)
- max_n: max length of char ngrams (Default 6)
- bucket: number of buckets used for hashing ngrams (Default 2000000)

- 
额外参数：
- word_ngrams ({1,0}, optional)
- If 1, uses enriches word vectors with subword(n-grams) information. If 0, this is equivalent to Word2Vec.



## 2.2 模型的保存与加载

```
# 模型保存与加载
model.save(fname)
model = FastText.load(fname)
```

那么既然gensim之中的fasttext,那么也有这么一种方式：

```
fasttext_model.wv.save_word2vec_format('temp/test_fasttext.txt', binary=False)
fasttext_model.wv.save_word2vec_format('temp/test_fasttext.bin', binary=True)
```

就是将fasttext地词向量保存为word2vec的格式，供调用：

```
5 4
是 -0.119938 0.042054504 -0.02282253 -0.10101332
中国人 0.080497965 0.103521846 -0.13045108 -0.01050107
你 -0.0788643 -0.082788676 -0.14035964 0.09101376
我 -0.14597991 0.035916027 -0.120259814 -0.06904249
谁 -0.0021443982 -0.0736454 -0.067576885 -0.025535036
```

但是，想回来了，如果fasttext保存为word2vec格式，**那么fasttest能不能重新load进来？**

笔者也不清楚，但是笔者没有看到在fasttext或`gensim.models.keyedvectors.FastTextKeyedVectors`，看到load_word2vec_format的函数，所以只能单向输出：`fasttext -> word2vec`

如果用`FastText.load(fname)`会报错：
```
UnpicklingError: invalid load key, '5'.
```

## 2.3 在线更新语料库

```
# 在线更新训练 fasttext
from gensim.models import FastText
sentences_1 = [["cat", "say", "meow"], ["dog", "say", "woof"]]
sentences_2 = [["dude", "say", "wazzup!"]]

model = FastText(min_count=1)
model.build_vocab(sentences_1)
model.train(sentences_1, total_examples=model.corpus_count, epochs=model.iter)

model.build_vocab(sentences_2, update=True)
model.train(sentences_2, total_examples=model.corpus_count, epochs=model.iter)
```

通过`build_vocab`来实现

## 2.4 c++ 版本的fasttext训练

```
# 使用c++ 版本的fasttext
from gensim.models.wrappers.fasttext import FastText as FT_wrapper

# Set FastText home to the path to the FastText executable
ft_home = '/home/chinmaya/GSOC/Gensim/fastText/fasttext'

# train the model
model_wrapper = FT_wrapper.train(ft_home, lee_train_file)

print(model_wrapper)
```

## 3 fasttext使用

### 3.1 获得词向量

```
model['你']  # 词向量获得的方式
model.wv['你'] # 词向量获得的方式
model.wv.word_vec('你')# 词向量获得的方式
```

两种方式获得单个单词的词向量

还有几种方式：

```
sentences = [["你", "是", "谁"], ["我", "是", "中国人"]]
fasttext_model = FastText(sentences,  size=4, window=3, min_count=1, iter=10,min_n = 3 , max_n = 6,word_ngrams = 0)

fasttext_model.wv.syn0_vocab  # 单词的向量组 (5, 4)
fasttext_model.wv.vectors_vocab# 单词的向量组 (5, 4)  vectors_vocab == syn0_vocab != vectors
fasttext_model.wv.vectors# 单词的向量组 (5, 4)
fasttext_model.wv.vectors_ngrams#基于单词的n-ngram的向量组 (10, 4)
fasttext_model.wv.syn0_ngrams   # 基于单词的n-ngram的向量组 (10, 4)
fasttext_model.wv.num_ngram_vectors # n-ngram数量
fasttext_model.wv.min_n   # 最小n-gram
```

vectors_ngrams与syn0_ngrams   一致，都是n-grams的词向量矩阵。*笔者也不清楚这么多矩阵具体是指啥。。。*

其中`fasttext_model.wv.syn0_ngrams`只能返回矩阵，其实他是如下的单词的fasttext词向量：

```
['<中国', '中国人', '国人>', '<中国人', '中国人>', '<中国人>','你',''我,'是','谁']
```

### 3.2 词向量词典

一种方式就是：`fasttext_model.wv.vocab`  是以dict形式，还有一种：`fasttext_model.wv.index2word`是以list形式

```
fasttext_model.wv.vocab
fasttext_model.wv.index2word
```

### 3.3 与word2vec 相同的求相似性

其中包括：

```
model.wv.most_similar(positive=['你', '是'], negative=['中国人'])
model.wv.most_similar_cosmul(positive=['你', '是'], negative=['中国人'])
```

类比关系，其中most_similar_cosmul使用乘法组合来查找最接近的词（[参考url](https://radimrehurek.com/gensim/models/keyedvectors.html#gensim.models.keyedvectors.WordEmbeddingsKeyedVectors.most_similar_cosmul)）

```
model.wv.doesnt_match("你 真的 是".split())  # 找到不匹配的
```

找出不适合的词

```
model.wv.similarity('你', '是')  # 求相似
model.n_similarity(['cat', 'say'], ['dog', 'say'])  # 多个词条求相似
```

`similarity`求两个词之间的相似性；`n_similarity`为求多个词之间的相似性

```
# !pip3 install pyemd 
model.wmdistance(['cat', 'say'], ['dog', 'say']) # 求词条之间的WMD距离
```

依据词向量求词条之间的WMD距离

### 3.4  求词附近的相似词

```
model.most_similar("滋润")
model.most_similar(positive=[vector], topn=topn, restrict_vocab=restrict_vocab)
```

其中，要注意`most_similar`是可以直接给入向量，然后来找相似的。这里官方还有几种类似的写法，如下：

```
model.similar_by_vector('你好')
model.similar_by_word('你好')
```

其中，有一种增强版的求相似词的方式：

```
model.wv.most_similar_cosmul(positive='苹果手机', negative='手机', topn=10)
```

官方的解释为：Positive words still contribute positively towards the similarity, negative words negatively, but with less susceptibility to one large distance dominating the calculation.

### 3.5 fasttext自带的OOV功能

fasttext自带的对于词典外的词条进行向量补齐，非常好用。再kaggle的1%方案中就提到fasttext应对OOV的问题，[About my 0.9872 single model](https://www.kaggle.com/c/jigsaw-toxic-comment-classification-challenge/discussion/52644)

原表述：

```
Fixed misspellings by finding word vector neighborhoods. Fasttext tool can create vectors for out-of-dictionary words which is really nice. I trained my own fasttext vectors on Wikipedia comments corpus and used them to do this. I also used those vectors as embeddings but results were not as good as with regular fasttext vectors.
```

示例：

```
model.most_similar("萌萌哒")

[('萌哒', 0.8866026401519775),
 ('桃江', 0.7472578287124634),
 ('比荣欣', 0.69571453332901),
 ('活泼可爱', 0.680438756942749),
 ('小可爱', 0.6803416013717651),
 ('可爱', 0.6769561767578125),
 ('萌', 0.6448146104812622),
 ('卡通', 0.6299867630004883),
 ('漂亮可爱', 0.6273207664489746),
 ('极漂亮', 0.620937705039978)]
```

关于OOV问题，也可以基于glove等向量来自己构造一套方法

### 3.5 如何获得fasttext的n-grams词向量

[fasttext_wrapper.py](https://github.com/RaRe-Technologies/gensim/blob/4543646d3fe3496e11bc935e72cbf9b18504442e/gensim/models/deprecated/fasttext_wrapper.py)，20181111补充，来看看fasttext内部如何应对OOV问题：

```
from gensim.models.utils_any2vec import _save_word2vec_format, _load_word2vec_format, _compute_ngrams, _ft_hash

def compute_ngrams(word, min_n, max_n):
    BOW, EOW = ('<', '>')  # Used by FastText to attach to all words as prefix and suffix
    extended_word = BOW + word + EOW
    ngrams = []
    for ngram_length in range(min_n, min(len(extended_word), max_n) + 1):
        for i in range(0, len(extended_word) - ngram_length + 1):
            ngrams.append(extended_word[i:i + ngram_length])
    return ngrams

    def word_vec(self, word, use_norm=False):
        if word in self.vocab:
            return super(FastTextKeyedVectors, self).word_vec(word, use_norm)
        else:
            # from gensim.models.fasttext import compute_ngrams
            word_vec = np.zeros(self.vectors_ngrams.shape[1], dtype=np.float32)
            ngrams = _compute_ngrams(word, self.min_n, self.max_n)
            if use_norm:
                ngram_weights = self.vectors_ngrams_norm
            else:
                ngram_weights = self.vectors_ngrams
            ngrams_found = 0
            for ngram in ngrams:
                ngram_hash = _ft_hash(ngram) % self.bucket
                if ngram_hash in self.hash2index:
                    word_vec += ngram_weights[self.hash2index[ngram_hash]]
                    ngrams_found += 1
            if word_vec.any():
                return word_vec / max(1, ngrams_found)
            else:  # No ngrams of the word are present in self.ngrams
                raise KeyError('all ngrams for word %s absent from model' % word)
```

以上是fasttext如何应对OOV词的原始计算方式，那么大致步骤为：

1 找到每个词的N-grams，`_compute_ngrams`函数

2 然后与n-grams词库进行匹配

3 匹配到的n-gram向量平均即为最后的输出值
```
from gensim.models import FastText
sentences = [["你", "是", "谁"], ["我", "是", "中国人"]]
fasttext_model = FastText(sentences,  size=4, window=3, min_count=1, iter=10,min_n = 3 , max_n = 6,word_ngrams = 0)

from gensim.models.utils_any2vec import _save_word2vec_format, _load_word2vec_format, _compute_ngrams, _ft_hash
ngrams = _compute_ngrams('吃了吗',min_n = 3,max_n = 6)
>>> ['<吃了', '吃了吗', '了吗>', '<吃了吗', '吃了吗>', '<吃了吗>']
```

笔者改编了一下，把fasttext之中的n-grams词向量可以提取出来。

```
def FastTextNgramsVector(fasttext_model):
    fasttext_word_list = fasttext_model.wv.vocab.keys()
    NgramsVector = {}
    ngram_weights = fasttext_model.wv.vectors_ngrams # (10, 4)
    for word in fasttext_word_list:
        ngrams = _compute_ngrams(word,min_n = fasttext_model.wv.min_n,max_n = fasttext_model.wv.max_n)
        for ngram in ngrams:
            ngram_hash = _ft_hash(ngram) % fasttext_model.wv.bucket  
            if ngram_hash in fasttext_model.wv.hash2index:
                NgramsVector[ngram] = ngram_weights[fasttext_model.wv.hash2index[ngram_hash]] 
    return NgramsVector

FastTextNgramsVector(fasttext_model)
```

最终的效果为：

```
{'<中国': array([ 0.15037228,  0.23413078, -0.09328791,  0.09616131], dtype=float32),
 '<中国人': array([ 0.22894476,  0.01658264,  0.09593856, -0.09224218], dtype=float32),
 '<中国人>': array([ 0.24443054,  0.12408283, -0.109778  ,  0.14463967], dtype=float32),
 '<你>': array([-0.10611233, -0.18498571, -0.24031653,  0.08941776], dtype=float32),
 '<我>': array([-0.14418595, -0.11722667, -0.00421342, -0.22331873], dtype=float32),
 '<是>': array([-0.198387  , -0.02605324,  0.20429775, -0.10319293], dtype=float32),
 '<谁>': array([ 0.0370588 , -0.17663571,  0.04465277,  0.09987918], dtype=float32),
 '中国人': array([ 0.18819457,  0.19730332, -0.2074779 , -0.23047261], dtype=float32),
 '中国人>': array([ 0.09325046,  0.16731283, -0.24085586,  0.12507215], dtype=float32),
 '国人>': array([-0.1650848 ,  0.18907125, -0.20082659, -0.03944619], dtype=float32)}
```

可以看到不太相同的是，`<中国`会有一个`<`符号，要注意。

## 4 fasttext 与 word2vec的对比

在案例：[Comparison of FastText and Word2Vec](https://github.com/RaRe-Technologies/gensim/blob/develop/docs/notebooks/Word2Vec_FastText_Comparison.ipynb)之中有官方给出的对比gensim之中，fasttext与word2vec的性能、语义关系比对。

参考博文：[https://rare-technologies.com/fasttext-and-gensim-word-embeddings/](https://rare-technologies.com/fasttext-and-gensim-word-embeddings/)
![在这里插入图片描述](https://img-blog.csdn.net/20181013193223157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
得出的结论：
- 具有n-gram的FastText模型在语法任务上的表现明显更好，因为句法问题与单词的形态有关；
- Gensim word2vec和没有n-gram的fastText模型在语义任务上的效果稍好一些，可能是因为语义问题中的单词是独立的单词而且与它们的char-gram无关；
- 一般来说，随着语料库大小的增加，模型的性能似乎越来越接近。但是，这可能是由于模型的维度大小保持恒定在100，而大型语料库较大维度的模型大小可能会导致更高的性能提升。
- 随着语料库大小的增加，所有模型的语义准确性显着增加。
- 然而，由于n-gram FastText模型的语料库大小的增加，句法准确度的提高较低（相对和绝对术语）。这可能表明，在较大的语料库大小的情况下，通过合并形态学信息获得的优势可能不那么显着（原始论文中使用的语料库似乎也表明了这一点）
- 最原始的fastText 由c++写的，而gensim是由py写的，运行性能还是c++要快一些

## 参考资源

1、[facebookresearch/fastText](https://github.com/facebookresearch/fastText/tree/master/python)

2、案例：[Using FastText via Gensim](https://github.com/RaRe-Technologies/gensim/blob/develop/docs/notebooks/FastText_Tutorial.ipynb)

3、案例：[Comparison of FastText and Word2Vec](https://github.com/RaRe-Technologies/gensim/blob/develop/docs/notebooks/Word2Vec_FastText_Comparison.ipynb)

4、官方教程：[models.fasttext – FastText model](https://radimrehurek.com/gensim/models/fasttext.html)

5、[FastText and Gensim word embeddings](https://rare-technologies.com/fasttext-and-gensim-word-embeddings/)








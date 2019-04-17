# 基于腾讯AI Lab词向量进行未知词、短语向量补齐与域内相似词搜索 - 素质云笔记/Recorder... - CSDN博客





2018年11月12日 22:22:58[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3344








![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112212351689.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz,size_16,color_FFFFFF,t_70)
*（~免费广告位一则~）*
AI Lab开源大规模高质量中文词向量数据，800万中文词随你用，质量非常高，就是一个词向量.txt文件都有16G之多，太夸张了。。不过的确非常有特点：
- ⒈ 覆盖率（Coverage）：

该词向量数据包含很多现有公开的词向量数据所欠缺的短语，比如“不念僧面念佛面”、“冰火两重天”、“煮酒论英雄”、“皇帝菜”、“喀拉喀什河”等。以“喀拉喀什河”为例，利用腾讯AI Lab词向量计算出的语义相似词如下：

墨玉河、和田河、玉龙喀什河、白玉河、喀什河、叶尔羌河、克里雅河、玛纳斯河
- ⒉ 新鲜度（Freshness）：

该数据包含一些最近一两年出现的新词，如“恋与制作人”、“三生三世十里桃花”、“打call”、“十动然拒”、“供给侧改革”、“因吹斯汀”等。以“因吹斯汀”为例，利用腾讯AI Lab词向量计算出的语义相似词如下：

一颗赛艇、因吹斯听、城会玩、厉害了word哥、emmmmm、扎心了老铁、神吐槽、可以说是非常爆笑了
- ⒊ 准确性（Accuracy）：

由于采用了更大规模的训练数据和更好的训练算法，所生成的词向量能够更好地表达词之间的语义关系。

腾讯AI Lab采用自研的Directional Skip-Gram (DSG)算法作为词向量的训练算法。DSG算法基于广泛采用的词向量训练算法Skip-Gram (SG)，在文本窗口中词对共现关系的基础上，额外考虑了词对的相对位置，以提高词向量语义表示的准确性。




### 文章目录
- [1  Tencent_AILab_ChineseEmbedding读入与高效查询](#1__Tencent_AILab_ChineseEmbedding_33)
- [2 未知词、短语向量补齐与域内相似词搜索](#2__88)
- [网络用语挖掘：](#_167)
- [评论观点](#_187)
- [同义词挖掘](#_229)




# 1  Tencent_AILab_ChineseEmbedding读入与高效查询

来看一下一个比较常见的读入方式：[lvyufeng/keras_text_sum/load_embedding.py](https://github.com/lvyufeng/keras_text_sum/blob/77d2f8beef5739fa51f507d06e63a916f9d7de87/utils/load_embedding.py)

```
import numpy as np

def load_embedding(path):
    embedding_index = {}
    f = open(path,encoding='utf8')
    for index,line in enumerate(f):
        if index == 0:
            continue
        values = line.split(' ')
        word = values[0]
        coefs = np.asarray(values[1:],dtype='float32')
        embedding_index[word] = coefs
    f.close()

    return embedding_index

load_embedding('/home/lv/data_set/Tencent_AILab_ChineseEmbedding/Tencent_AILab_ChineseEmbedding.txt')
```

这样纯粹就是以字典的方式读入，当然用于建模没有任何问题，但是笔者想在之中进行一些相似性操作，最好的就是重新载入gensim.word2vec系统之中，但是笔者发现载入半天都会报错：

```
ValueError: invalid vector on line 418987 (is this really the text format?)
```

仔细一查看，发现原来一些词向量的词就是数字，譬如`-0.2121`或 `57851`，所以一直导入不进去。只能自己用txt读入后，删除掉这一部分，保存的格式参考下面。

```
5 4
是 -0.119938 0.042054504 -0.02282253 -0.10101332
中国人 0.080497965 0.103521846 -0.13045108 -0.01050107
你 -0.0788643 -0.082788676 -0.14035964 0.09101376
我 -0.14597991 0.035916027 -0.120259814 -0.06904249
```

第一行是一共5个词，每个词维度为4.

然后清洗完毕之后，就可以读入了：

```
wv_from_text = gensim.models.KeyedVectors.load_word2vec_format('Tencent_AILab_ChineseEmbedding_refine.txt',binary=False)
```

但是又是一个问题，占用内存太大，导致不能查询相似词，所以这里可以用一下这个神奇的函数，可以高效运行，这样就可以顺利使用`most_similar`这类函数了：

```
wv_from_text.init_sims(replace=True)  # 神奇，很省内存，可以运算most_similar
```

该操作是指model已经不再继续训练了，那么就锁定起来，让Model变为只读的，这样可以预载相似度矩阵，对于后面得相似查询非常有利。

# 2 未知词、短语向量补齐与域内相似词搜索

这边未知词语、短语的补齐手法是参考FastText的用法：[极简使用︱Gemsim-FastText 词向量训练以及OOV（out-of-word）问题有效解决](https://blog.csdn.net/sinat_26917383/article/details/83041424)

这边笔者借鉴了fasttext之中的方式，当出现未登录词或短语的时候，会：
- 先将输入词进行n-grams
- 然后去词表之中查找
- 查找到的词向量进行平均

主要函数可见：

```
import numpy as np

def compute_ngrams(word, min_n, max_n):
    #BOW, EOW = ('<', '>')  # Used by FastText to attach to all words as prefix and suffix
    extended_word =  word
    ngrams = []
    for ngram_length in range(min_n, min(len(extended_word), max_n) + 1):
        for i in range(0, len(extended_word) - ngram_length + 1):
            ngrams.append(extended_word[i:i + ngram_length])
    return list(set(ngrams))


def wordVec(word,wv_from_text,min_n = 1, max_n = 3):
    '''
    ngrams_single/ngrams_more,主要是为了当出现oov的情况下,最好先不考虑单字词向量
    '''
    # 确认词向量维度
    word_size = wv_from_text.wv.syn0[0].shape[0]   
    # 计算word的ngrams词组
    ngrams = compute_ngrams(word,min_n = min_n, max_n = max_n)
    # 如果在词典之中，直接返回词向量
    if word in wv_from_text.wv.vocab.keys():
        return wv_from_text[word]
    else:  
        # 不在词典的情况下
        word_vec = np.zeros(word_size, dtype=np.float32)
        ngrams_found = 0
        ngrams_single = [ng for ng in ngrams if len(ng) == 1]
        ngrams_more = [ng for ng in ngrams if len(ng) > 1]
        # 先只接受2个单词长度以上的词向量
        for ngram in ngrams_more:
            if ngram in wv_from_text.wv.vocab.keys():
                word_vec += wv_from_text[ngram]
                ngrams_found += 1
                #print(ngram)
        # 如果，没有匹配到，那么最后是考虑单个词向量
        if ngrams_found == 0:
            for ngram in ngrams_single:
                word_vec += wv_from_text[ngram]
                ngrams_found += 1
        if word_vec.any():
            return word_vec / max(1, ngrams_found)
        else:
            raise KeyError('all ngrams for word %s absent from model' % word)
    
vec = wordVec('千奇百怪的词向量',wv_from_text,min_n = 1, max_n = 3)  # 词向量获取
wv_from_text.most_similar(positive=[vec], topn=10)    # 相似词查找
```

`compute_ngrams`函数是将词条N-grams找出来，譬如：

```
compute_ngrams('萌萌的哒的',min_n = 1,max_n = 3)
>>> ['哒', '的哒的', '萌的', '的哒', '哒的', '萌萌的', '萌的哒', '的', '萌萌', '萌']
```

这边没有沿用fasttext之中的`<>`来区分词头、词尾。

`wordVec`函数是计算未登录词的，其中笔者小小加了一些内容，就是：当出现oov的情况下,最好先不考虑单字词向量，如果能匹配到两个字以上的内容就优先进行平均。

在得到未登录词或短语的向量之后，就可以快速进行查找，gensim里面是支持给入向量进行相似词查找：

```
wv_from_text.most_similar(positive=[vec], topn=10)
```

其实，有了这么一个小函数 + 稍微大内存的服务器，就可以开始挖金矿了，笔者在此给出一部分可供参考与使用的小案例，案例中找出来的相似肯定还是不那么干净，需要自行清洗一下：

### 网络用语挖掘：

```
vec = wordVec('天了噜',wv_from_text,min_n = 1, max_n = 3)
wv_from_text.most_similar(positive=[vec], topn=20)

[('天了噜', 1.0),
 ('天啦噜', 0.910751223564148),
 ('天惹', 0.8336831331253052),
 ('我的天呐', 0.8315592408180237),
 ('天哪噜', 0.8200887441635132),
 ('也是醉了', 0.8048921823501587),
 ('哦买噶', 0.7951157093048096),
 ('我也是醉了', 0.7925893664360046),
 ('我的天哪', 0.7903991937637329),
 ('天呐', 0.7862901091575623)
......
]
```

### 评论观点

```
vec = wordVec('真难吃',wv_from_text,min_n = 1, max_n = 3)
wv_from_text.most_similar(positive=[vec], negative=['好吃'], topn=20)

[('真难', 0.8344259858131409),
 ('难吃', 0.8344259262084961),
 ('不好吃', 0.7413374185562134),
 ('难啊', 0.7120314836502075),
 ('难喝', 0.6996017694473267),
 ('难以下咽', 0.6920732259750366),
 ('好难', 0.6856701374053955),
 ('挺好吃', 0.6801191568374634),
 ('真不容易', 0.6788320541381836),
 ('真的很难', 0.671592116355896),
 ('真的很好吃', 0.6692471504211426),
...
```

例子2：

```
vec = wordVec('环境干净',wv_from_text,min_n = 1, max_n = 3)
wv_from_text.most_similar(positive=[vec], topn=20)

[('环境干净', 0.9999998807907104),
 ('环境干净整洁', 0.8523852825164795),
 ('环境舒适', 0.8281853199005127),
 ('环境干净卫生', 0.8241869211196899),
 ('卫生干净', 0.8118663430213928),
 ('干净卫生', 0.7971832156181335),
 ('干净舒适', 0.796349287033081),
 ('环境清新', 0.7937666773796082),
 ('卫生好', 0.7925254702568054),
 ('环境整洁', 0.7919654846191406),
 ('环境好', 0.7814522981643677),
 ('房间干净', 0.7802159786224365),
 ('环境优雅', 0.7685255408287048),
```

### 同义词挖掘

```
vec = wordVec('苹果',wv_from_text,min_n = 1, max_n = 3)
wv_from_text.most_similar(positive=[vec],negative=['水果'], topn=20)

[('苹果公司', 0.5877306461334229),
 ('苹果开发', 0.5226757526397705),
 ('高通', 0.5215991735458374),
 ('谷歌', 0.5213730335235596),
 ('苹果的iphone', 0.5150437355041504),
 ('微软', 0.5127487778663635),
 ('苹果新', 0.5012987852096558),
 ('pixel手机', 0.49072039127349854),
 ('苹果高管', 0.4897959530353546),
 ('苹果iphone', 0.4875335991382599),
 ('苹果手机iphone', 0.4791686534881592),
 ('苹果芯片', 0.47766292095184326),
 ('iphone', 0.4754045307636261),
```





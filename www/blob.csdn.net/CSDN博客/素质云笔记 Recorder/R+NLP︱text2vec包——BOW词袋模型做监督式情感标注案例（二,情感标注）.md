# R+NLP︱text2vec包——BOW词袋模型做监督式情感标注案例（二,情感标注） - 素质云笔记/Recorder... - CSDN博客





2016年11月21日 18:36:23[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：8320
所属专栏：[R语言与自然语言处理](https://blog.csdn.net/column/details/13670.html)















      在之前的开篇提到了text2vec**，笔者将其定义为R语言文本分析"No.1"，她是一个文本分析的生态系统。**笔者在学习之后发现开发者简直牛！基于分享精神，将自学笔记记录出来。开篇内容参考：

[重磅︱R+NLP：text2vec包——New 文本分析生态系统 No.1（一,简介）](http://blog.csdn.net/sinat_26917383/article/details/53161863)





**文档可以以多种方式表达，单独词组、n-grams、特征hashing化的方法等。**
      一般来说文本分析的步骤有以下三个步骤：
      1、第一步：把内容表达成为文档-词组矩阵（document-term矩阵，DTM）或者词组共现矩阵（term-co-occurrence矩阵，TCM），换言之第一步就是在文档之上创造一个词条地图。
      2、第二步：找个模型在DTM上进行拟合，有LDA、文本分类等
      3、第三步：在验证集上进行验证


      本文在此基础上，将一个官方案例予以演示，算得上是翻译他们官方文档。




————————————————————————————————




# 一、BOW词袋模型




      Bag-of-words model (BoW model)最早出现在自然语言处理（Natural Language Processing[](http://en.wikipedia.org/wiki/Natural_language_processing)）和信息检索（Information Retrieval）领域.。该模型忽略掉文本的语法和语序等要素，将其仅仅看作是若干个词汇的集合，文档中每个单词的出现都是独立的。BoW使用一组无序的单词(words)来表达一段文字或一个文档.。近年来，BoW模型被广泛应用于计算机视觉中。

      基于文本的BoW模型的一个简单例子如下：




      首先给出两个简单的文本文档如下：

        John likes to watch movies. Mary likes too.

        John also likes to watch football games.




      基于上述两个文档中出现的单词，构建如下一个词典 (dictionary)：

       {"John": 1, "likes": 2,"to": 3, "watch": 4, "movies": 5,"also": 6, "football": 7, "games": 8,"Mary": 9, "too": 10}




      上面的词典中包含10个单词, 每个单词有唯一的索引, 那么每个文本我们可以使用一个10维的向量来表示。如下：

       [1, 2, 1, 1, 1, 0, 0, 0, 1, 1]

       [1, 1,1, 1, 0, 1, 1, 1, 0, 0]




      该向量与原来文本中单词出现的顺序没有关系，而是词典中每个单词在文本中出现的频率。
      Distributed Representation是一个稠密、低维的实数限量，它的每一维表示词语的一个潜在特征，该特征捕获了有用的句法和语义特征。其特点是将词语的不同句法和语义特征分布到它的每一个维度上去表示。

![](https://img-blog.csdn.net/20160809145300089)


      关于词袋模型的介绍可以看我的另外一篇博客：[自然语言处理︱简述四大类文本分析中的“词向量”（文本词特征提取）](http://blog.csdn.net/sinat_26917383/article/details/52162589)

      BOW算得上是最简单，但效果竟然也还不错的办法。




————————————————————————————————





# 二、text2vec基于BOW的情感标注






      本文选用的是text2vec开发者自带的数据集，有ID、sentiment代表情感正负面、review代表电影简介的内容。

![](https://img-blog.csdn.net/20161121170014340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      同样，text2vec的数据结构迁入的是data.table，所以效率极高，纵观来看，开发者都很有良心，每个环节都十分注意效率，再次给赞，关于data,table包可以参考我的另外一篇博客：[R︱高效数据操作——data.table包（实战心得、dplyr对比、key灵活用法、数据合并）](http://blog.csdn.net/sinat_26917383/article/details/52911734)。



## 1、数据准备






```
library(text2vec)
library(data.table)
data("movie_review")
setDT(movie_review)
setkey(movie_review, id)
set.seed(2016L)
all_ids = movie_review$id
train_ids = sample(all_ids, 4000)
test_ids = setdiff(all_ids, train_ids)
train = movie_review[J(train_ids)]
test = movie_review[J(test_ids)]
```
      同时区分训练集train与测试集test，利用的是sample函数设置，setDT与setkey是data.table包的主要内容，设置关键KEY，后续很多分析都即为有用。




## 2、文档向量化（Vectorization） 




      构造的是一个文档-词频矩阵（DTM矩阵），不同文档、不同词发生的次数。这个稀疏矩阵的表达有两种方式：一种就是n-grams（前面提到的BOW）另外一种就是Hashing化（Feature Hashing）。

      text2vec构造DTM矩阵，可有点费劲，来看看流程：




**（1）设置分词迭代器，itoken；**

**（2）分词，create_vocabulary，英文里面直接分割即可，中文可就麻烦了，这里中文可不一样，官方案例是英文的，所以还需要自己处理一下。**

**（3）设置、形成语料文件，vocab_vectorizer**

**（4）构建DTM矩阵，create_dtm**




      尼玛，看下来真是烦，这么多步骤。但是，为什么这么多步骤呢？效率...这么做效率极高，笔者在自己写了一些函数才深刻体会到，R语言的文本分析对效率的要求有多高...  哭个...来看看代码：






```
prep_fun = tolower
tok_fun = word_tokenizer

it_train = itoken(train$review, 
             preprocessor = prep_fun, 
             tokenizer = tok_fun, 
             ids = train$id, 
             progressbar = FALSE)
vocab = create_vocabulary(it_train)
```





      其中的itoken中的参数很多，来解读一下，tok_fun代表词语划分成什么程度，是否需要标点等。
      tolower代表英文字符通通变成小写。




```
vectorizer = vocab_vectorizer(vocab)
dtm_train = create_dtm(it_train, vectorizer)
```



这里细心的同学一定要注意、留意最后生成的文档顺序以及ID是否一一对应，本次案例当然是一一对应，但是在自己操作的过程中，很容易出现，生成的DTM文档顺序发生了改变，你还不知道怎么改变，然后复查可就麻烦大了。





`identical(rownames(dtm_train), train$id)`




identical是检验两个值是否完全相等的函数，如果相等则会返回TRUE，相关内容参考我的博客：[R语言︱集合运算——小而美法则](http://blog.csdn.net/sinat_26917383/article/details/51277581)


## 3、基于logistics的情感标注




监督式的机器学习算法很多，当然选用一个较为经典、解释性比较强的方法，logistics就是这样的办法，不会的戳我的博客：[笔记+R︱Logistics建模简述（logit值）](http://blog.csdn.net/sinat_26917383/article/details/51727058)

利用的是R语言中的glmnet包。






```
library(glmnet)
NFOLDS = 4
glmnet_classifier = cv.glmnet(x = dtm_train, y = train[['sentiment']], 
                              family = 'binomial', 
                              # L1 penalty
                              alpha = 1,
                              # interested in the area under ROC curve
                              type.measure = "auc",
                              # 5-fold cross-validation
                              nfolds = NFOLDS,
                              # high value is less accurate, but has faster training
                              thresh = 1e-3,
                              # again lower number of iterations for faster training
                              maxit = 1e3)
```





L1正则化惩罚（不会戳我博客：[笔记︱范数正则化L0、L1、L2-岭回归&Lasso回归（稀疏与特征工程）](http://blog.csdn.net/sinat_26917383/article/details/52092040)），4折交叉验证。最后plot一下可以看到CV图：
![](https://img-blog.csdn.net/20161121173227063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最大AUC值为0.923,这是训练集的AUC，那么来看看验证集的效果怎么样。






```
it_test = test$review %>% 
  prep_fun %>% 
  tok_fun %>% 
  itoken(ids = test$id, 
         # turn off progressbar because it won't look nice in rmd
         progressbar = FALSE)

dtm_test = create_dtm(it_test, vectorizer)

preds = predict(glmnet_classifier, dtm_test, type = 'response')[,1]
glmnet:::auc(test$sentiment, preds)
```





最终验证集AUC值为0.91667，效果还不错。




————————————————————————————————





# 三、text2vec基于BOW的情感标注的优化




## 1、消除低词频单词




      一些停用词、一些低频无效词都是文本噪声。所以针对停用词stopword可以在分词步骤create_vocabulary予以处理，譬如：



```
stop_words = c("i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours")
vocab = create_vocabulary(it_train, stopwords = stop_words)
```





针对一些低频词的修剪，可以在分词create_vocabulary步骤之后以及设置、形成语料文件，vocab_vectorizer之前进行处理：



```
pruned_vocab = prune_vocabulary(vocab, 
                                 term_count_min = 10, 
                                 doc_proportion_max = 0.5,
                                 doc_proportion_min = 0.001)
vectorizer = vocab_vectorizer(pruned_vocab)
```





term_count自然就是词频，低于10的都删掉了。






## 2、增加文字信息量n-gram




      之前模型中都是单独分开，现在可以多多考虑2-grams，就在分词步骤进行优化。那么先来看看n-grams是啥？

如果是1-ngrams，有一句话：you need many money

      分割成：



```
terms terms_counts doc_counts
1: money            1          1
2:  many            1          1
3:  need            1          1
4:   you            1          1
```
      那么2-ngrams呢？

```
terms terms_counts doc_counts
1:      money            1          1
2: many_money            1          1
3:       many            1          1
4:  need_many            1          1
5:   you_need            1          1
6:       need            1          1
7:        you            1          1
```
      譬如一些内容就以小组合的形式出现了。不过，最后尝试建模之后，训练集的AUC值为0.9268，跟原来几乎差不多。。。







## 3、效率优化：feature hashing化




      为什么提到这个呢？R语言的文本处理效率本身不高，而且大数据集下，任何处理软件的处理都显得很无力，所以hash化是一个不可避免的趋势。这一趋势，被雅虎广泛使用（[Vowpal Wabbit](https://github.com/JohnLangford/vowpal_wabbit/)）。hashing化的好处主要有两个：

      1、非常快，效率高

      2、内存占用很低。

关于哈希化，可参考我的博客：[R语言实现︱局部敏感哈希算法（LSH）解决文本机械相似性的问题（一，基本原理）](http://blog.csdn.net/sinat_26917383/article/details/52451028)




      Hash化主要在第三步，设置、形成语料文件时进行操作，之后操作一样。


`h_vectorizer = hash_vectorizer(hash_size = 2 ^ 14, ngram = c(1L, 2L))`
验证之后，验证集的AUC为0.903，下降了2%左右，但是换来了最大化效率的提升，物超所值。





## 4、数据转变优化方法一：标准化




      常规的标准化很常见，不懂的戳我博客：[R语言︱数据规范化、归一化](http://blog.csdn.net/sinat_26917383/article/details/51228217)

      一般来说，文本分析中有时候文档长度很长，但是这一指标对最终结果都是无效的，所以需要惩罚一下文档长度。

   text2vec中包括了以下两类标准化，L1normalization与L2 normalization，这是图像处理中较为常见的标准化方式，参考来自博客[图像处理中的L1-normalize 和L2-normalize](http://blog.csdn.net/a200800170331/article/details/21737741)



当一幅图像用某种特征表示出来，一般要进行L1-normalize或者是L2-normalize。

假设一幅图像表示为Y=[x1 x2 x3 x4 x5],

      L1-normalize的结果为：



![](https://img-blog.csdn.net/20140321183755375)


      L2-normalize的结果为：


![](https://img-blog.csdn.net/20140321183939093)





      通过L1或L2标准化的图像特征往往具有良好的效果，至于那个更好就需要自己试验。
数据转化主要作用在DTM上，所以是第四步之后，而且主要用于惩罚文档，l1（归一化）的效果就是每行相加为1，函数如下：

`dtm_train_l1_norm = normalize(dtm_train, "l1")`      有以下三种可选方式：l1 l2 none。







## 5、数据转变优化方法二：TFIDF




**      同样也是作用在DTM最后一步步骤，作用过程也有些繁琐：**

**（1）设置TFIDF编译器tfidf =TfIdf$new()；**

**（2）转换成TFIDF格式fit_transform(dtm_train, tfidf)。**






```
dtm_train = create_dtm(it_train, vectorizer)

tfidf = TfIdf$new()
dtm_train_tfidf = fit_transform(dtm_train, tfidf)
dtm_test_tfidf  = create_dtm(it_test, vectorizer) %>% 
  transform(tfidf)
```





      当然从代码的最简化的角度，可以写成下面这样：





```
dtm_test_tfidf  = create_dtm(it_test, vectorizer) %>% 
  transform(tfidf)
```

      最终结果验证集的AUC值为0.905，貌似还降低了。不过，TFIDF对于效率的提升很显著，一般的任务都是会提升的。所以，是个提升精度的好办法。




————————————————————————————————




# 应用一：在text2vec中灵活的进行数据转化、并建模




      从第三章节来看，数据转化一般是形成DTM之后，或者通过fit或者通过fit_transform来进行数据转化。使用起来可以非常灵活，而且已经生成了DTM，你怎么玩都会出结果，这一点有点厉害！！

      1、数据转化之后，可以后续直接分析，而且跟之前的内容是具有可比性的。

      2、灵活，可以的情况是，训练集没有进行TFIDF，而测试集可以进行TFIDF转化，单独看效果如何







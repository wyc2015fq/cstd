
# R语言实现︱局部敏感哈希算法（LSH）解决文本机械相似性的问题（二，textreuse介绍） - 素质云笔记-Recorder... - CSDN博客

2016年09月07日 12:14:13[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2839所属专栏：[R语言与自然语言处理](https://blog.csdn.net/column/details/13670.html)




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————

上一篇（R语言实现︱局部敏感哈希算法（LSH）解决文本机械相似性的问题（一，基本原理））讲解了LSH的基本原理，笔者在想这么牛气冲天的方法在R语言中能不能实现得了呢？
于是在网上搜索了一下，真的发现了一个叫textreuse的包可以实现这样的功能，而且该包较为完整，可以很好地满足要求。
现在的版本是 0.1.3，最近的更新的时间为 2016-03-28。
国内貌似比较少的用这个包来实现这个功能，毕竟R语言在运行大规模数据的性能比较差，而LSH又是处理大规模数据的办法，所以可能国内比较少的用R来执行这个算法。


**回顾一下LSH的算法步骤：**

1、一般的步骤是先把数据点（可以是原始数据，或者提取到的特征向量）组成矩阵；

2、第一次hash functions（有多个哈希函数，是从某个哈希函数族中选出来的）哈希成一个叫“**签名矩阵（Signature Matrix）”**的东西，这个矩阵可以直接理解为是降维后的数据，此时用simhash、minhash来做，第一步的hash过程可以使用不同的functions来做；

3、第二次LSH把Signature Matrix哈希一下，就得到了每个数据点最终被hash到了哪个bucket里，如果新来一个数据点，假如是一个网页的特征向量，我想找和这个网页相似的网页，那么把这个网页对应的特征向量hash一下，看看它到哪个桶里了。
那么本篇详细介绍一下textreuse包的基本功能，分为：
一、数据格式识别与导入
二、机械分词技术
三、hash函数
四、简单文本相似性比较
五、并行

————————————————————————————————————————


# 一、语料数据格式识别与导入
后续的LSH必须要用到textreuse指定的格式（类似tm包），所以数据导入过程之后还有一步数据转化的过程。所以在textreuse包中有两种方法实现以上功能：
1、直接从文件读入，同时转化为指定格式（tm包格式），函数有两个：TextReuseTextDocument、TextReuseCorpus；
2、先用常规方法读入R环境，然后转化数据格式，同样可以用上述两个函数。
这两个函数是textreuse的数据基础也是关键。**两个函数在转化的过程中就可以直接分词+基本hash形成签名矩阵。**当然，也可以设置以下两个函数tokenizer=NULL，hash_func=NULL，先转化，然后在自己分词与hash化。

## 1、TextReuseTextDocument
这是textreuse比较独特的一个读入函数。
`TextReuseTextDocument(text, file = NULL, meta = list(),
  tokenizer = tokenize_ngrams, ..., hash_func = hash_string,
  minhash_func = NULL, keep_tokens = FALSE, keep_text = TRUE,
  skip_short = TRUE)`file参数可以读取文件file数据，需要设定文件目录；tokeniezer表示读入的数据自动分词；同时读入还将其直接哈希化了（hash_func）。

`library(textreuse)
file <- system.file("extdata/ats/remember00palm.txt", 
                    package = "textreuse")
doc <- TextReuseTextDocument(file = file, meta = list("publisher" = "ATS"),
                             tokenizer = tokenize_ngrams, n = 5,
                             keep_tokens = TRUE)`来看看官方案例，其中的tokenizer和n后续会说，n代表多元组。官方案例中是读入单个文件，不知道是否能批量读入某文件夹里的文件。但是批量读取的情况下，还是用下面的函数比较合适。


## 2、
## TextReuseCorpus
这个函数基于tm包演化而来的，跟上面的函数差不多。
`TextReuseCorpus(paths, dir = NULL, text = NULL, meta = list(),
  progress = interactive(), tokenizer = tokenize_ngrams, ...,
  hash_func = hash_string, minhash_func = NULL, keep_tokens = FALSE,
  keep_text = TRUE, skip_short = TRUE)`该函数的读入文本范围比较广，txt、字符串的都没问题，如果有其他拓展名的文件可以用dir来读入。
而且可以批量导入某一个文件夹中所有的文件内容。
如果有多个文件的基础上，也可以通过corpus[["remember00palm"]]、corpus[c("calltounconv00baxt", "lifeofrevrichard00baxt")]这样的形式选中对应的文本内容。


## 3、函数查看与基本内容修改
以前在使用tm包的使用就觉得转化格式之后，查看起来就不是那么方便了。同样在这有一些函数可以查看里面具体内容。
其中转化了之后的数据会带有一些信息，通过meta可以更改这些信息，比如文档的ID信息。
`#查看基本信息
meta(doc)      #查看基本信息 id
tokens(doc)    #机械分词内容
wordcount(doc) #总词数
content(doc)   #查看原始内容
names(doc)     #每个文档的姓名（可修改）`其中wordcount如果有多个文件，那么就会计算每个文件的字数，这个跟table有点像。如果需要修改其中的一些内容可以用meta。
`#修改相关信息
meta(doc)$id
meta(doc, "id")
meta(doc, "date") <- 1865`

————————————————————————————————————————


# 二、机械分词技术
分词技术可以分为机械分词以及基于统计序列标注的分词技术，具体的拓展可以看我另外一篇博客的内容:[NLP︱中文分词技术小结、几大分词引擎的介绍与比较](http://blog.csdn.net/sinat_26917383/article/details/52275328)
**在R语言中专门用来中文分词的有jiebeR和Rwordseg，现在这两个大多数的分词技术都是基于序列标注的，所以计算量相对较大，但是文本机械相似性对分词没有那么高的要求，要求分成单个字符串的形式就可以满足要求了。**
所以，**textreuse就可以比较方面的实现简单的机械分词，只是把文档的内容去掉噪音+分开成字符串。**
现在假如有一句话：
`text <- "本次讲习班主要围绕知识获取、学习及推理技术，以及基于知识图谱的应用进展，邀请相关领域的专家学者做主题报告。"`

## 1、机械分词——tokenize_words()
`> tokenize_words(text)
 [1] "本次"   "讲习班" "主要"   "围绕"   "知识"   "获取"   "学习"   "及"     "推理"   "技术"  
[11] "以及"   "基于"   "知识"   "图"     "谱"     "的"     "应用"   "进展"   "邀请"   "相关"  
[21] "领域"   "的"     "专家"   "学者"   "做"     "主题"   "报告"`从效果来看，还不错，竟然能把一些词提取出来，而且去掉了标点，而且速度较快。貌似可以再这样的分词技术上做很多后续的探究。

## 2、断句
## ——tokenize_sentences

`> tokenize_sentences(text)
[1] "本次讲习班主要围绕知识获取 学习及推理技术 以及基于知识图谱的应用进展 邀请相关领域的专家学者做主题报告"`断句的原理是根据标点符号的来进行。

## 3、多元组
## ——tokenize_ngrams
可能对中文的支持不好，输出的格式是乱码的，所以在这不能示范中文，而是英文。
`> text <- "How many roads must a man walk down? The answer is blowin' in the wind."
> tokenize_ngrams(text, n = 3)
 [1] "how many roads"   "many roads must"  "roads must a"     "must a man"      
 [5] "a man walk"       "man walk down"    "walk down the"    "down the answer" 
 [9] "the answer is"    "answer is blowin" "is blowin in"     "blowin in the"   
[13] "in the wind"`

## 4、跨越式多元组
## ——tokenize_skip_ngrams

`> tokenize_skip_ngrams(text, n = 3, k = 2)
 [1] "how must walk"      "many a down"        "roads man the"      "must walk answer"  
 [5] "a down is"          "man the blowin"     "walk answer in"     "down is the"       
 [9] "the blowin wind"    "how roads a"        "many must man"      "roads a walk"      
[13] "must man down"      "a walk the"         "man down answer"    "walk the is"       
[17] "down answer blowin" "the is in"          "answer blowin the"  "is in wind"        
[21] "how many roads"     "many roads must"    "roads must a"       "must a man"        
[25] "a man walk"         "man walk down"      "walk down the"      "down the answer"   
[29] "the answer is"      "answer is blowin"   "is blowin in"       "blowin in the"     
[33] "in the wind"`以上的3、4的多元组的具有很大的意义。但是对中文的支持一直不好，输出的中文是乱码的，这个暂时笔者没有去细致探究。

## 5、诗词的断句——英文场景
包的作者自己写了一个断句的函数。
`> poem <- "Roses are red\nViolets are blue\nI like using R\nAnd you should too"
> string=poem
> tokenize_lines <- function(string) {
+   stringr::str_split(string, "\n+")[[1]]
+ }
> 
> tokenize_lines(poem)
[1] "Roses are red"      "Violets are blue"   "I like using R"     "And you should too"`通过\来分割。

————————————————————————————————————————


# 三、hash化
**该包里面有挺多的hash函数：hashes、minhashes、rehash、hash_string**
**R语言中构造hash函数也有专门的包：****[digest](https://cran.r-project.org/web/packages/digest/index.html)**
其中hash_string（词），有n个词就hash成n个hash值；
而**minhash则是把文档**，比如一个文档1W个词，还是固定的一个文档，300维，有一个比较有效的降维功效，同时也不会损失太多信息量，原来相似的文本表现的还是相似。
rehash则可以自己选择：
`rehash(x, func, type = c("hashes", "minhashes"))`
自由选择用hashes还是minhashes。


————————————————————————————————————————


# 四、简单文本相似性比较
[相似性距离在上篇讲过](http://blog.csdn.net/sinat_26917383/article/details/52451028)，这里不赘述。一般有两类：海明距离（用在simhash）、Jaccard距离（用在Minhash）
如果只是不hash，直接看样本的相似性，必然是Jaccard要好一些。
`> a <- tokenize_words(paste("How does it feel, how does it feel?",
+                           "To be without a home",
+                           "Like a complete unknown, like a rolling stone"))
> b <- tokenize_words(paste("How does it feel, how does it feel?",
+                           "To be on your own, with no direction home",
+                           "A complete unknown, like a rolling stone"))
> #?similarity-functions
> jaccard_similarity(a, b)      #相似程度
[1] 0.65`此时可以看出jaccard的相似性距离为0.65。
当然textreuse包中同样有其他的一些距离，可以来看看：
`> jaccard_dissimilarity(a, b)   #相差程度=1-相似程度
[1] 0.35
> jaccard_bag_similarity(a, b)  #最大值为0.5
[1] 0.4
> ratio_of_matches(a, b)        #在ab交叉/b总数
[1] 0.75`jaccard_dissimilarity=1-jaccard_similarity；
ratio_of_matches原理跟Jaccard差不多也是根据集合来求相似性的。

————————————————————————————————————————


# 五、并行
在求解过程中，一般来说，语料生成以及分词部分耗费计算量，可以采用并行算法，textreuse基于parallel拓展了该包的性能，可以设置核心数。但是**遗憾的是不能再windows系统下设置。**
`options("mc.cores" = 4L)`

**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————



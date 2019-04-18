# 【Spark Mllib】TF-IDF&Word2Vec——文本相似度 - Soul Joy Hub - CSDN博客

2016年06月21日 16:59:02[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：17745
所属专栏：[Spark机器学习](https://blog.csdn.net/column/details/15014.html)



[http://blog.csdn.net/u011239443/article/details/51728659](http://blog.csdn.net/u011239443/article/details/51728659)

# 1 从数据中抽取合适的特征

## 1.1 TF-IDF短语加权表示

> 
[TF-IDF公式](http://blog.csdn.net/u011239443/article/details/51655480#t34)的含义是:在一个文档中出现次数很多的词相比出现次数少的词应该在词向量表示中得到更高的权值。而IDF归一化起到了减弱在所有文档中总是出现的词的作用。最后的结果就是,稀有的或者重要的词被给予了更高的权值,而更加常用的单词(被认为比较不重要)则在考虑权重的时候有较小的影响。

## 1.2 特征哈希

> 
特征哈希通过使用哈希方程对特征赋予向量下标,这个向量下标是通过对特征的值做哈希得到的(通常是整数)。例如,对分类特征中的美国这个位置特征得到的哈希值是342。我们将使用哈希值作为向量下标,对应的值是1.0,表示美国这个特征出现了。使用的哈希方程必须是一致的(就是说,对于一个给定的输入,每次返回相同的输出)。

## 1.3 数据集处理

我们将使用一个非常有名的数据集,叫作20 Newsgroups;这个数据集一般用来做文本分类。这是一个由20个不同主题的新闻组消息组成的集合,有很多种不同的数据格式。对于我们的任务来说,可以使用[按日期组织的数据集](http://download.csdn.net/detail/u011239443/9554240)。
`tar xfvz 20news-bydate.tar.gz`
创建了两个文件夹:一个是20news-bydate-train,另一个是20news-bydate-test。

### 1.3.1 读取所有文件

```
/* Replace 'PATH' with the path to the 20 Newsgroups Data */
val path = "/PATH/20news-bydate-train/*"
val rdd = sc.wholeTextFiles(path)//读取所有文件
// count the number of records in the dataset
println(rdd.count)
/*
...
14/10/12 14:27:54 INFO FileInputFormat: Total input paths to process : 11314
...
11314
*/
val text = rdd.map { case (file, text) => text }//file 是 文件 dir，text是文件内容
```

### 1.3.2 过滤单词
- 使用正则表达切分原始文档来移除这些非单词字符，并且每个文档的所有单词变为小写。 

\W：匹配任何非单词字符。等价于“[^A-Za-z0-9_]”。 

+：匹配1或多个正好在它之前的那个字符。例如正则表达式9+匹配9、99、999等。

```
// split text on any non-word tokens
val nonWordSplit = text.flatMap(t => t.split("""\W+""").map(_.toLowerCase))
println(nonWordSplit.distinct.count)
// 130126
// inspect a look at a sample of tokens
println(nonWordSplit.distinct.sample(true, 0.3, 42).take(100).mkString(","))
/*
bone,k29p,w1w3s1,odwyer,dnj33n,bruns,_congressional,mmejv5,mmejv5,artur,125215,entitlements,beleive,1pqd9hinnbmi,
jxicaijp,b0vp,underscored,believiing,qsins,1472,urtfi,nauseam,tohc4,kielbasa,ao,wargame,seetex,museum,typeset,pgva4,
dcbq,ja_jp,ww4ewa4g,animating,animating,10011100b,10011100b,413,wp3d,wp3d,cannibal,searflame,ets,1qjfnv,6jx,6jx,
detergent,yan,aanp,unaskable,9mf,bowdoin,chov,16mb,createwindow,kjznkh,df,classifieds,hour,cfsmo,santiago,santiago,
1r1d62,almanac_,almanac_,chq,nowadays,formac,formac,bacteriophage,barking,barking,barking,ipmgocj7b,monger,projector,
hama,65e90h8y,homewriter,cl5,1496,zysec,homerific,00ecgillespie,00ecgillespie,mqh0,suspects,steve_mullins,io21087,
funded,liberated,canonical,throng,0hnz,exxon,xtappcontext,mcdcup,mcdcup,5seg,biscuits
*/
```
- 过滤掉数字和包含数字的单词。使用正则模式可以过滤掉和这个模式不匹配的单词:

```
// filter out numbers
val regex = """[^0-9]*""".r
val filterNumbers = nonWordSplit.filter(token => regex.pattern.matcher(token).matches)
println(filterNumbers.distinct.count)
// 84912
println(filterNumbers.distinct.sample(true, 0.3, 42).take(100).mkString(","))
/*
reunion,wuair,schwabam,eer,silikian,fuller,sloppiness,crying,crying,beckmans,leymarie,fowl,husky,rlhzrlhz,ignore,
loyalists,goofed,arius,isgal,dfuller,neurologists,robin,jxicaijp,majorly,nondiscriminatory,akl,sively,adultery,
urtfi,kielbasa,ao,instantaneous,subscriptions,collins,collins,za_,za_,jmckinney,nonmeasurable,nonmeasurable,
seetex,kjvar,dcbq,randall_clark,theoreticians,theoreticians,congresswoman,sparcstaton,diccon,nonnemacher,
arresed,ets,sganet,internship,bombay,keysym,newsserver,connecters,igpp,aichi,impute,impute,raffle,nixdorf,
nixdorf,amazement,butterfield,geosync,geosync,scoliosis,eng,eng,eng,kjznkh,explorers,antisemites,bombardments,
abba,caramate,tully,mishandles,wgtn,springer,nkm,nkm,alchoholic,chq,shutdown,bruncati,nowadays,mtearle,eastre,
discernible,bacteriophage,paradijs,systematically,rluap,rluap,blown,moderates
*/
```
- 移除[停用词](http://blog.csdn.net/u011239443/article/details/51655480#t37)

我们用过滤完数字字符之后的单词集合生成一个每个单词在文档中出现频率的集合。现在可以使用Spark的 top 函数来得到前20个出现次数最多的单词。我们把它们作为停用词。 

> 
  纯C语言进行简单的文本搜索可见我的另外一篇博文《[文本搜索](http://blog.csdn.net/u011239443/article/details/51719756)》

```
// examine potential stopwords
val tokenCounts = filterNumbers.map(t => (t, 1)).reduceByKey(_ + _)
val oreringDesc = Ordering.by[(String, Int), Int](_._2)
println(tokenCounts.top(20)(oreringDesc).mkString("\n"))
/*
(the,146532)
(to,75064)
(of,69034)
(a,64195)
(ax,62406)
(and,57957)
(i,53036)
(in,49402)
(is,43480)
(that,39264)
(it,33638)
(for,28600)
(you,26682)
(from,22670)
(s,22337)
(edu,21321)
(on,20493)
(this,20121)
(be,19285)
(t,18728)
*/

// filter out stopwords
val stopwords = Set(
    "the","a","an","of","or","in","for","by","on","but", "is", "not", "with", "as", "was", "if",
    "they", "are", "this", "and", "it", "have", "from", "at", "my", "be", "that", "to"
)
val tokenCountsFilteredStopwords = tokenCounts.filter { case (k, v) => !stopwords.contains(k) }
println(tokenCountsFilteredStopwords.top(20)(oreringDesc).mkString("\n"))
/*
(ax,62406)
(i,53036)
(you,26682)
(s,22337)
(edu,21321)
(t,18728)
(m,12756)
(subject,12264)
(com,12133)
(lines,11835)
(can,11355)
(organization,11233)
(re,10534)
(what,9861)
(there,9689)
(x,9332)
(all,9310)
(will,9279)
(we,9227)
(one,9008)
*/
```
- 删除仅含有一个字符的单词

```
// filter out tokens less than 2 characters
val tokenCountsFilteredSize = tokenCountsFilteredStopwords.filter { case (k, v) => k.size >= 2 }
println(tokenCountsFilteredSize.top(20)(oreringDesc).mkString("\n"))
/*
(ax,62406)
(you,26682)
(edu,21321)
(subject,12264)
(com,12133)
(lines,11835)
(can,11355)
(organization,11233)
(re,10534)
(what,9861)
(there,9689)
(all,9310)
(will,9279)
(we,9227)
(one,9008)
(would,8905)
(do,8674)
(he,8441)
(about,8336)
(writes,7844)
*/
```
- 除去频率低的单词 

很多短语在整个文集中只出现一次。对于使用提取特征来完成的任务,比如文本相似度比较或者生成机器学习模型,只出现一次的单词是没有价值的,因为这些单词我们没有足够的训练数据。

```
// filter out rare tokens with total occurence < 2
val rareTokens = tokenCounts.filter{ case (k, v) => v < 2 }.map { case (k, v) => k }.collect.toSet
val tokenCountsFilteredAll = tokenCountsFilteredSize.filter { case (k, v) => !rareTokens.contains(k) }
println(tokenCountsFilteredAll.top(20)(oreringAsc).mkString("\n"))
/*
(sina,2)
(akachhy,2)
(mvd,2)
(hizbolah,2)
(wendel_clark,2)
(sarkis,2)
(purposeful,2)
(feagans,2)
(wout,2)
(uneven,2)
(senna,2)
(multimeters,2)
(bushy,2)
(subdivided,2)
(coretest,2)
(oww,2)
(historicity,2)
(mmg,2)
(margitan,2)
(defiance,2)
*/
println(tokenCountsFilteredAll.count)
// 51801
```

现在把过滤逻辑组合到一个函数中,并应用到RDD中的每个文档:

```
// create a function to tokenize each document
def tokenize(line: String): Seq[String] = {
    line.split("""\W+""")
        .map(_.toLowerCase)
        .filter(token => regex.pattern.matcher(token).matches)
        .filterNot(token => stopwords.contains(token))
        .filterNot(token => rareTokens.contains(token))
        .filter(token => token.size >= 2)
        .toSeq
}

// check that our tokenizer achieves the same result as all the steps above
println(text.flatMap(doc => tokenize(doc)).distinct.count)
// 51801
// tokenize each document
val tokens = text.map(doc => tokenize(doc))
println(tokens.first.take(20))
/*
WrappedArray(mathew, mathew, mantis, co, uk, subject, alt, atheism, faq, atheist, resources, 
    summary, books, addresses, music, anything, related, atheism, keywords, faq)
*/
```

# 2. 训练TF-IDF模型

首先,引入我们需要的类,创建一个 HashingTF 实例,传入维度参数 dim 。默认特征维度是20 (或者接近一百万),因此我们选择2 18 (或者26 000),因为使用50 000个单词应该不会产生很多的哈希冲突,而较少的维度占用内存更少并且展示起来更方便。HashingTF 的 transform 函数把每个输入文档(即词项的序列)映射到一个MLlib的 Vector对象。我们将调用 cache 来把数据保持在内存来加速之后的操作。HashingTF 的 transform 函数返回一个 RDD[Vector] 的引用,因此我们可以把返回的结果转换成MLlib的 SparseVector 形式。transform 方法可以接收 Iterable 参数(例如一个以 Seq[String] 形式出现的文档)对每个文档进行处理,最后返回一个单独的结果向量。

```
// === train TF-IDF model === //

import org.apache.spark.mllib.linalg.{ SparseVector => SV }
import org.apache.spark.mllib.feature.HashingTF
import org.apache.spark.mllib.feature.IDF
// set the dimensionality of TF-IDF vectors to 2^18
val dim = math.pow(2, 18).toInt
val hashingTF = new HashingTF(dim)

val tf = hashingTF.transform(tokens)
// cache data in memory
tf.cache
val v = tf.first.asInstanceOf[SV]
println(v.size)
// 262144
println(v.values.size)
// 706
println(v.values.take(10).toSeq)
// WrappedArray(1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0)
println(v.indices.take(10).toSeq)
// WrappedArray(313, 713, 871, 1202, 1203, 1209, 1795, 1862, 3115, 3166)
```

现在通过创建新的IDF实例并调用RDD中的 fit 方法,利用词频向量作为输入来对文库中的 

每个单词计算逆向文本频率。之后使用IDF的 transform 方法转换词频向量为TF-IDF向量

```
val idf = new IDF().fit(tf)
val tfidf = idf.transform(tf)   
val v2 = tfidf.first.asInstanceOf[SV]
println(v2.values.size)
// 706
println(v2.values.take(10).toSeq)
// WrappedArray(2.3869085659322193, 4.670445463955571, 6.561295835827856, 4.597686109673142,  ...
println(v2.indices.take(10).toSeq)
// WrappedArray(313, 713, 871, 1202, 1203, 1209, 1795, 1862, 3115, 3166)
```

# 使用 TF-IDF 模型

我们预估两个从曲棍球新闻组随机选择的新闻比较相似。

```
val hockeyText = rdd.filter { case (file, text) =>
file.contains("hockey") }
val hockeyTF = hockeyText.mapValues(doc =>
hashingTF.transform(tokenize(doc)))
val hockeyTfIdf = idf.transform(hockeyTF.map(_._2))
```

有了曲棍球文档向量后,就可以随机选择其中两个向量,并计算它们的[余弦相似度](http://blog.csdn.net/u011239443/article/details/51655480#t38)。

```
// compute cosine similarity using Breeze
import breeze.linalg._
val hockey1 = hockeyTfIdf.sample(true, 0.1, 42).first.asInstanceOf[SV]
val breeze1 = new SparseVector(hockey1.indices, hockey1.values, hockey1.size)
val hockey2 = hockeyTfIdf.sample(true, 0.1, 43).first.asInstanceOf[SV]
val breeze2 = new SparseVector(hockey2.indices, hockey2.values, hockey2.size)
val cosineSim = breeze1.dot(breeze2) / (norm(breeze1) * norm(breeze2))
println(cosineSim)
// 0.060250114361164626
```

TF-IDF 模型 & 余弦相似度 一个经典使用就是搜索引擎结果的排名，可以参阅我的另外一篇博文《[轻量级文本搜索引擎的后台设计、实现与优化](http://blog.csdn.net/u011239443/article/details/51655480)》

# Word2Vec 模型

> 
另一类最近比较流行的模型是把每一个单词表示成一个向量。这些模型一般是基于某种文本中与单词共现相关的统计量来构造。一旦向量表示算出,就可以像使用TF-IDF向量一样使用这些模型(例如使用它们作为机器学习的特征)。一个比较通用的例子是使用单词的向量表示基于单词的含义计算两个单词的相似度。Word2Vec就是这些模型中的一个具体实现,常称作分布向量表示。MLlib模型使用一种skip-gram模型,这是一种考虑了单词出现的上下文来学习词向量表示的模型。

- 训练 

我们需要传递一个RDD,其中每一个元素都是一个单词的序列。可以使用我们之前得到的分词后的文档来作为模型的输入:

```
import org.apache.spark.mllib.feature.Word2Vec
val word2vec = new Word2Vec()
word2vec.setSeed(42) // we do this to generate the same results each time
val word2vecModel = word2vec.fit(tokens)
/*
...
14/10/25 14:21:59 INFO Word2Vec: wordCount = 2133172, alpha = 0.0011868763094487506
14/10/25 14:21:59 INFO Word2Vec: wordCount = 2144172, alpha = 0.0010640806039941193
14/10/25 14:21:59 INFO Word2Vec: wordCount = 2155172, alpha = 9.412848985394907E-4
14/10/25 14:21:59 INFO Word2Vec: wordCount = 2166172, alpha = 8.184891930848592E-4
14/10/25 14:22:00 INFO Word2Vec: wordCount = 2177172, alpha = 6.956934876302307E-4
14/10/25 14:22:00 INFO Word2Vec: wordCount = 2188172, alpha = 5.728977821755993E-4
14/10/25 14:22:00 INFO Word2Vec: wordCount = 2199172, alpha = 4.501020767209707E-4
14/10/25 14:22:00 INFO Word2Vec: wordCount = 2210172, alpha = 3.2730637126634213E-4
14/10/25 14:22:01 INFO Word2Vec: wordCount = 2221172, alpha = 2.0451066581171076E-4
14/10/25 14:22:01 INFO Word2Vec: wordCount = 2232172, alpha = 8.171496035708214E-5
...
14/10/25 14:22:02 INFO SparkContext: Job finished: collect at Word2Vec.scala:368, took 56.585983 s
14/10/25 14:22:02 INFO MappedRDD: Removing RDD 200 from persistence list
14/10/25 14:22:02 INFO BlockManager: Removing RDD 200
14/10/25 14:22:02 INFO BlockManager: Removing block rdd_200_0
14/10/25 14:22:02 INFO MemoryStore: Block rdd_200_0 of size 9008840 dropped from memory (free 1755596828)
word2vecModel: org.apache.spark.mllib.feature.Word2VecModel = org.apache.spark.mllib.feature.Word2VecModel@2b94e480
*/
```
- 使用

```
// evaluate a few words
word2vecModel.findSynonyms("hockey", 20).foreach(println)
/*
(sport,0.6828256249427795)
(ecac,0.6718048453330994)
(hispanic,0.6519884467124939)
(glens,0.6447514891624451)
(woofers,0.6351765394210815)
(boxscores,0.6009076237678528)
(tournament,0.6006366014480591)
(champs,0.5957855582237244)
(aargh,0.584071934223175)
(playoff,0.5834275484085083)
(ahl,0.5784651637077332)
(ncaa,0.5680188536643982)
(pool,0.5612311959266663)
(olympic,0.5552600026130676)
(champion,0.5549421310424805)
(filinuk,0.5528956651687622)
(yankees,0.5502706170082092)
(motorcycles,0.5484763979911804)
(calder,0.5481109023094177)
(rec,0.5432182550430298)
*/
word2vecModel.findSynonyms("legislation", 20).foreach(println)
/*
(accommodates,0.8149217963218689)
(briefed,0.7582570314407349)
(amended,0.7310371994972229)
(telephony,0.7139414548873901)
(aclu,0.7080780863761902)
(pitted,0.7062571048736572)
(licensee,0.6981208324432373)
(agency,0.6880651712417603)
(policies,0.6828961372375488)
(senate,0.6821110844612122)
(businesses,0.6814320087432861)
(permit,0.6797110438346863)
(cpsr,0.6764014959335327)
(cooperation,0.6733141541481018)
(surveillance,0.6670728325843811)
(restricted,0.6666574478149414)
(congress,0.6661365628242493)
(procure,0.6655452251434326)
(industry,0.6650314927101135)
(inquiry,0.6644254922866821)
*/
```

![](https://img-blog.csdn.net/20170625152617464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


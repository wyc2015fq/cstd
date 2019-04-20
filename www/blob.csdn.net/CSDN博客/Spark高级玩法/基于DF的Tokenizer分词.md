# 基于DF的Tokenizer分词 - Spark高级玩法 - CSDN博客
2017年12月04日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1143
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU5A2z4WnTGWTFfJacdqkq4XNBEBSoCo1ExrPkUaOCks6vXVLHI17Vczc0YibNK0tpNSM1wOpJHJMQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**Tokenizer分词**
进行文本分析前，对文本中句子进行分词我们处理的第一步。大家都是Spark的机器学习库分为基于RDD和基于DataFrame的库，由于基于RDD的库在Spark2.0以后都处于维护状态，我们这里讲的分词就是基于Spark的Dataframe的。主要是讲解两个类Tokenizer和RegexTokenizer的使用。
1 首先准备数据
**导包**
import
org.apache.spark.ml.feature.{RegexTokenizer,
Tokenizer}
import
org.apache.spark.sql.functions._
**准数据**
valsentenceDataFrame =
spark.createDataFrame(Seq(
 (0,
"Hi I heard about Spark"),
(1,
"I wish Java could use case classes"),
(2,
"Logistic,regression,models,are,neat")
)).toDF("id",
"sentence")
**2 Tokenizer**
Tokenizer负责读取文档或者句子，将其分解为单词。声明一个变量
valtokenizer = new
Tokenizer().setInputCol("sentence").setOutputCol("words")
自定义函数来获取每列单词数目
valcountTokens =
udf{ (words:
Seq[String])
 => words.length }
调用转换函数
valtokenized = tokenizer.transform(sentenceDataFrame)
tokenized.select("sentence",
"words").withColumn("tokens",
countTokens(col("words"))).show(false)
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU5A2z4WnTGWTFfJacdqkq4YlncUjzwfVgHh0zqCRTo8PBjeWa4LicJkq9M3yI7svH4mx35EQuliaZA/0?wx_fmt=png)
**3 RegexTokenizer**
RegexTokenizer允许基于正则的方式进行文档切分成单词组。默认情况下，使用参数“pattern”（regex, default: "\s+"）作为分隔符来分割输入文本。或者，用户可以将参数“gaps”设置为false，指示正则表达式“pattern”表示“tokens”，而不是分割间隙，并查找所有匹配事件作为切分后的结果。
valregexTokenizer =
new
RegexTokenizer().setInputCol("sentence").setOutputCol("words").setPattern("\W")
// 也可换为 .setPattern("\w+").setGaps(false)
开始转换并查看执行结果
valregexTokenized =
regexTokenizer.transform(sentenceDataFrame)
regexTokenized.select("sentence",
"words").withColumn("tokens",
countTokens(col("words"))).show(false)
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU5A2z4WnTGWTFfJacdqkq48Ug6EeKRxnhliabhYPbLYHffBVrkrM7HwPrYGxL1PXK4iaKMiayicj1LiaA/0?wx_fmt=png)
**推荐阅读：**
1，[干货：基于Spark Mllib的SparkNLP库。](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484265&idx=1&sn=670a359265edd9ae6eee9f358df35495&chksm=9f38e041a84f695749d1f54971874f10c7912840b73ab1f48af8c443b6c5ca1d806e3f3e2388&scene=21#wechat_redirect)
2，[Spark的Ml pipeline](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484270&idx=1&sn=69153063accd105649d4466d938012da&chksm=9f38e046a84f69505d954726c65d940f9a370342755ae2ce7456134ef4b9a6eafb1350a4dfb6&scene=21#wechat_redirect)
3，[spark调优系列之内存和GC调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483782&idx=1&sn=52059883b33cb97005186b0cba4e5f5a&chksm=9f38e2aea84f6bb8230189ee4e3de9cc7e11497f20f6cd498e540ef7f713478481c830106f51&scene=21#wechat_redirect)
4，[Table API&SQL的基本概念及使用介绍](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483931&idx=1&sn=324aba23f190d18fdb4a6f829d28aeb7&chksm=9f38e133a84f6825a8a30eb36bca4f8e7d0ddf969b131aa138fc4271953609fdf51caaed5e80&scene=21#wechat_redirect)

![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**


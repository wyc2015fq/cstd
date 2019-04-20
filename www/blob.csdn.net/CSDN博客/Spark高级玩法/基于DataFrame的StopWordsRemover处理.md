# 基于DataFrame的StopWordsRemover处理 - Spark高级玩法 - CSDN博客
2017年12月04日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1016
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU5A2z4WnTGWTFfJacdqkq4Tq0dRUKZckTrmYc6jRvJsgfOMGsp3pUkHdGqbaicQ3WukuhXWPBodzA/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
stopwords简单来说是指在一种语言中广泛使用的词。在各种需要处理文本的地方，我们对这些停止词做出一些特殊处理，以方便我们更关注在更重要的一些词上。
对于不同类型的需求而言，对停止词的处理是不同的。
1.    有监督的机器学习 – 将停止词从特征空间剔除
2.    聚类– 降低停止词的权重
3.    信息检索– 不对停止词做索引
4.    自动摘要- 计分时不处理停止词
对于不同语言，停止词的类型都可能有出入，但是一般而言有这简单的三类
1.    限定词
2.    并列连词
3.    介词
停止词的词表一般不需要自己制作，有很多可选项可以自己下载选用。
Spark中提供了StopWordsRemover类处理停止词，它可以用作Machine learning Pipeline的一部分。
**StopWordsRemover**的功能是直接移除所有停用词（stopword），所有从inputCol输入的量都会被它检查，然后再outputCol中，这些停止词都会去掉了。
默认的话会在构建StopWordsRemover对象的时候调用**loadDefaultStopWords(language: String): Array[String]**加载**/org/apache/spark/ml/feature/stopwords/english.txt**。
这是一个简单的停止词表，包含181个词（spark2.2）。
默认还提供了其他几种语言（danish, dutch, english, finnish, french, german, hungarian,italian, norwegian, portuguese, russian, spanish, swedish, turkish）的停止词，遗憾的是没有中文默认停止词表，所以对于中文停止词需要自己提供。
假如我们有个dataframe，有两列：id和raw。
|Id|Raw|
|----|----|
|0|[I, saw, the, red, baloon]|
|1|[Mary, had, a, little, lamb]|
Raw列作为StopWordsRemover的输出，filtered作为输出列，会得到如下数据：
|Id|Raw|Filtered|
|----|----|----|
|0|[I, saw, the, red, baloon]|[saw, red, baloon]|
|1|[Mary, had, a, little, lamb]|[Mary, little, lamb]|
在这里filtered，“I”，“the”，“had”和“a”这些停用词语已被滤除。
import
org.apache.spark.ml.feature.StopWordsRemover
// $example on$
valremover = new
StopWordsRemover().setInputCol("raw").setOutputCol("filtered")
valdataSet =
spark.createDataFrame(Seq(
 (0,
Seq("I",
"saw",
"the",
"red",
"balloon")),
(1,
Seq("Mary",
"had",
"a",
"little",
"lamb"))
)).toDF("id",
"raw")
remover.transform(dataSet).show(false)
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU5A2z4WnTGWTFfJacdqkq442ubdkn3l5BlQgTG5Erj06Il6ZNE3YNR2KWiaVjLhrTjL2GwCHVmzLg/0?wx_fmt=png)
**StopWordsRemover的常用API介绍**
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU5A2z4WnTGWTFfJacdqkq4qMiba0VlqVfUwC2huMdSWhIymeYsgpJlGVTibL4dMR5b5GQBN9B81XLw/0?wx_fmt=png)
**推荐阅读：**
1，[Spark的Ml
 pipeline](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484270&idx=1&sn=69153063accd105649d4466d938012da&chksm=9f38e046a84f69505d954726c65d940f9a370342755ae2ce7456134ef4b9a6eafb1350a4dfb6&scene=21#wechat_redirect)
2，[论Spark
 Streaming的数据可靠性和一致性](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484247&idx=1&sn=f9a65c44ea7186012d72dee25526ec77&chksm=9f38e07fa84f69699c3599dbf06753b7b4b99148fc076b1ea6896aef466731c012a4831a4607&scene=21#wechat_redirect)
3，[kafka连接器两种部署模式详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484229&idx=1&sn=d5e87da8a05c40b617f343044ba6d808&chksm=9f38e06da84f697b5181c1aedbeb9205e3af0a81dde6d19487cbde803e01d86fb9367b3a3b3b&scene=21#wechat_redirect)
4，[Kylin及数据仓库的技术概念详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484196&idx=1&sn=d95310ff52b68d05ad512ab0179beb5c&chksm=9f38e00ca84f691a82514f795c6e36ecfda06ea200e1d7035043c777e12aaeebc6b4994aa3e3&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**


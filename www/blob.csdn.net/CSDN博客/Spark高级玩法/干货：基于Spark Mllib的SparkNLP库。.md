# 干货：基于Spark Mllib的SparkNLP库。 - Spark高级玩法 - CSDN博客
2017年11月29日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：2705
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUGRsdtG2kiavFNotRq6ibpS1jgO0H1KWnXWic9mkcPCHUFdoIPTyxFLicXwibOOmIvlR9baS44bA47SJw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**引言**
这是来自John Snow Labs工程团队的社区博客和工作，解释了他们对开源Apache Spark自然语言处理（NLP）库的贡献。
Apache Spark是一个通用的集群计算框架，它支持分布式SQL，流式处理，图处理和机器学习。现在，Spark生态系统还有一个Spark Natural Language Processing库。
John Snow Labs NLP库是在Scala编写的Apache 2.0以上，不依赖于其他NLP或ML库。它本地扩展了Spark ML管道API。该框架提供了注释器的概念，并带出了以下内容：
- 
标记生成器
- 
规范化
- 
词干提取
- 
Lemmatizer
- 
实体提取器
- 
日期提取器
- 
Part of Speech Tagger
- 
命名实体识别
- 
句子边界检测
- 
情感分析
- 
拼写检查器
另外，由于与Spark ML的紧密集成，在构建NLP管道时，您可以直接使用Spark的更多功能。这包括词语嵌入，主题建模，停用词移除，各种特征工程功能（tf-idf，n-gram，相似性度量等）以及在机器学习工作流中使用NLP注释作为特征。如果您不熟悉这些术语，那么理解NLP任务的指南是一个好的开始。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUGRsdtG2kiavFNotRq6ibpS1YWeTM0z5qWHIvJZxLzOJ9INqEC1b941UCYpY1AUPNZnIlichrDzTlyQ/0?wx_fmt=png)
Spark ML提供了一套机器学习应用程序，它的逻辑由两个主要组件组成：估计器(Estimators)和 变换器（Transformers）。首先，有一个称为fit（）的方法，将一段数据保存并传递给这样的应用程序，Transformer（一般是拟合过程的结果）将更改应用于目标数据集。这些组件已嵌入到适用于Spark NLP。 管道是允许单个工作流程中包含多个估计器和变换器的机制，允许沿机器学习任务进行多个链接转换。
**注释(Annotation)**
注释是Spark-NLP操作结果的基本形式。它的结构是由：
- 
**annotatorType:** 哪个注释器生成了这个注释
- 
**begin: **
匹配的内容相对于原始文本的开始。
- 
**end: **
匹配的内容相对于原始文本的结尾
- 
**metadata:** 匹配结果的内容和附加信息
该对象在转换处理结束后由注释器自动生成。不需要手动参与。但为了有效地使用它，必须按照顺序理解。
**注解器(Annotators)**
注解器是SparkNLP中NLP功能的先锋。**有两种形式的注释器**：
- 
**注解器方法：**代表Spark ML Estimator并需要一个训练stage。他们有一个称为fit（data）的函数，它根据一些数据来训练一个模型。他们生产第二种类型的注释器，它是一个注释器模型或转换器(transformer)。
- 
**Annotator模型：**它们是spark模型或转换器(transformer)，意味着它们具有一个transform(data)函数，它接受一个数据集并添加一个带有这个标注结果的列。所有转换器(transformer)都是附加的，这意味着它们附加到当前数据，决不会替换或删除以前的信息。
这两种形式的注释器都可以包含在Pipeline中，并且会自动按照提供的顺序遍历所有阶段并相应地转换数据。在fit（）阶段之后，Pipeline变成了PipelineModel。无论是之前还是之后，可以随时保存到磁盘并重新从磁盘加载。
**公共函数**
**setInputCols（column_names）**：获取此注释器所需的注释列名称
**setOutputCol（ column_name）**：定义包含此注释器结果的列的名称。使用此名称作为其他注释器的输入，需要注释这个注释器。
**例子分析**
**1 注释器类型**
每个注释器都有一个类型。这些共享类型的注释器可以互换使用，这意味着您可以在需要时使用它们中的任何一个。例如，当另一个注释器（如情感分析注释器）需要令牌类型注释器时，可以提供标normalized token或lemma，因为两者都是类型标记。
**2 使用spark读入数据**
我们例子测试采用的是spark-shell的方式，spark-2.1.1版本以上，本文采用的是spark2.1.2，scala版本2.11.8，启动：
spark-shell --jars /opt/jars/spark-nlp-1.2.3.jar
**3 加载数据并测试,**
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUGRsdtG2kiavFNotRq6ibpS1vat5DIdvKFBoa4FKQ5APcD9a9JgY794l5oVkyPIqe2qQe8S1z9V05w/0?wx_fmt=png)
我们这里是先把数据赋值为名为data的变量
val data = spark.read.parquet("file:///opt/datas/*")
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUGRsdtG2kiavFNotRq6ibpS1QYDK4EkrLICMuKXsVew3yNP2ibfBg7UkwWZMQPHysopMka9NpxMOksg/0?wx_fmt=png)
**4 DocumentAssembler：获取数据**
为何贯穿NLP处理过程，我们需要将原始数据进行标注。有一个特殊的**transformer**为我们做这件事情：**DocumentAssembler**，它会创建第一个类型为**Document**的注释，该注释会被以后的注解器使用。
importcom.johnsnowlabs.nlp._
importcom.johnsnowlabs.nlp.annotators._
importorg.apache.spark.ml.Pipeline
valdocumentAssembler = new
DocumentAssembler()
.setInputCol("text")
.setOutputCol("document")
**5 句子检测及分词**
在这个快速的例子中，我们现在开始在每个文档行中标识句子。**SentenceDetectorModel**需要一个由**DocumentAssembler**输出提供的**Document**注释，它本身是一个**Document**类型标记。**RegexTokenizer**需要一个Document注释类型，这意味着它与**DocumentAssembler**或**SentenceDetector**输出一起工作，在这里，我们使用句子输出。
importcom.johnsnowlabs.nlp.annotators.sbd.pragmatic.SentenceDetectorModel
valsentenceDetector= new
SentenceDetectorModel()
.setInputCols(Array("document"))
.setOutputCol("sentence")
valregexTokenizer= new
RegexTokenizer()
.setInputCols(Array("sentence"))
.setOutputCol("token")
**6 使用管道(pipeline)**
现在我们要把所有这些放在一起并检索结果，我们使用Pipeline来做到这一点。我们还包含另一个特殊的变形器，称为“ Finisher”，以人类语言显示标记
val finisher
= new
Finisher()
.setInputCols("token")
.setCleanAnnotations(false)
val pipeline
= new
Pipeline()
.setStages(Array(
documentAssembler,
sentenceDetector,
regexTokenizer,
       finisher
))
pipeline
.fit(data)
.transform(data)
.show()
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUGRsdtG2kiavFNotRq6ibpS1JqRCJyb3kH55F9ru3RhPaIf4t9f0qUQsTZu5V85vgccbpibW665I3vw/0?wx_fmt=png)
也可以按照spark的输出文件的格式保存结果，比如：
pipeline.fit(data).transform(data).toJSON.write.text("file:///opt/output")
**7 输出为**
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUGRsdtG2kiavFNotRq6ibpS1CvBGU3dibyUwKhvYlKk6qj92uEiby5xWl9eNibDEOk2DbJOKghZ90xxpA/0?wx_fmt=png)
**推荐阅读：**
1，[Spark部署模式另类详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484102&idx=1&sn=10d0f633093d460623af753f92a73a81&chksm=9f38e1eea84f68f83afb0e3f2c5ccb2d3029af65340474a6d740b8ecbbdd0d2eaaf8b8169387&scene=21#wechat_redirect)
2，[金融反欺诈场景下的Spark实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484078&idx=1&sn=6740b1da86f8bda4157178ff9ec9d7db&chksm=9f38e186a84f689092aad7e9023fdca856abdada21b2a417870a075899384916f3809c3563be&scene=21#wechat_redirect)
3，[论Spark
 Streaming的数据可靠性和一致性](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484247&idx=1&sn=f9a65c44ea7186012d72dee25526ec77&chksm=9f38e07fa84f69699c3599dbf06753b7b4b99148fc076b1ea6896aef466731c012a4831a4607&scene=21#wechat_redirect)
4，[Spark源码系列之foreach和foreachPartition的区别](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483766&idx=1&sn=bed469000c668cc65a7329a278dd5c41&chksm=9f38e25ea84f6b48012580bd02227b70a1f6c1fa93ee57a8e6c569686553b22f388554fabb69&scene=21#wechat_redirect)
**关注公众号。输入：nlp，即可获得本文设计的项目工程和jar及本文中的案例代码。**
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

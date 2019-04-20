# SparkMllib主题模型案例讲解 - Spark高级玩法 - CSDN博客
2017年12月21日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1665
![?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibicF7RDDEic33eia87lotkyibj8vVlMBDehvlx3rsDhcMOy8AGsibFzsNA0c18yIAxBpcbSk1JJRibSaA/?wx_fmt=png&wxfrom=5&wx_lazy=1)
**一  本文涉及到的算法**
**1， LDA主题模型**
**符号定义**
- 
文档集合D，m篇，topic集合T，k个主题
- 
D中每个文档d看作一个单词序列< w1,w2,...,wn >，wi表示第i个单词，设d有n个单词。（LDA里面称之为word bag，实际上每个单词的出现位置对LDA算法无影响）
- 
D中涉及的所有不同单词组成一个大集合VOCABULARY（简称VOC）
**LDA符合的分布**
- 
每篇文章d(长度为)都有各自的主题分布，主题分布式多项分布，该多项分布的参数服从Dirichlet分布，该Dirichlet分布的参数为α。
- 
每个主题都有各自的词分布，词分布为多项分布，该多项分布的参数服从Dirichlet分布，该Dirichlet分布的参数为β；
- 
对于谋篇文章中的第n个词，首先从该文章的主题分布中采样一个主题，然后在这个主题对应的词分布中采样一个词。不断重复这个随机生成过程，直到m篇文章全部完成过程。
结果是希望训练出两个结果向量(k个topic，VOC中共包含m个词)
LDA以文档集合D作为输入(会有分词，去掉停用词，取词干等预处理)：
- 
对每个D中的文档d，对应到不同topic的概率θd < pt1,..., ptk >，其中，pti表示d对应T中第i个topic的概率。计算方法是直观的，pti=nti/n，其中nti表示d中对应第i个topic的词的数目，n是d中所有词的总数。
- 
对每个T中的topic t，生成不同单词的概率φt < pw1,..., pwm >，其中，pwi表示t生成VOC中第i个单词的概率。计算方法同样很直观，pwi=Nwi/N，其中Nwi表示对应到topic t的VOC中第i个单词的数目，N表示所有对应到topic t的单词总数。
LDA的核心公式如下：
**p(w|d) = p(w|t)*p(t|d)**
直观的看这个公式，就是以Topic作为中间层，可以通过当前的θd和φt给出了文档d中出现单词w的概率。其中p(t|d)利用θd计算得到，p(w|t)利用φt计算得到。
**2， RegexTokenizer**
RegexTokenizer允许基于正则的方式进行文档切分成单词组。默认情况下，使用参数“pattern”（ regex, default: "s+"）作为分隔符来分割输入文本。或者，用户可以将参数“gaps”设置为false，指示正则表达式“pattern”表示“tokens”，而不是分割间隙，并查找所有匹配事件作为切分后的结果。
具体请参考：[基于DF的Tokenizer分词](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484300&idx=1&sn=d09e8213b4572b3310fcf5c0f260306a&chksm=9f38e0a4a84f69b281c16a454ab71d7b628737d432d42c309cb92f6e05810ce253a42cd8ce74&scene=21#wechat_redirect)
**3， StopWordsRemover**
stopwords简单来说是指在一种语言中广泛使用的词。在各种需要处理文本的地方，我们对这些停止词做出一些特殊处理，以方便我们更关注在更重要的一些词上。
停止词的词表一般不需要自己制作，有很多可选项可以自己下载选用。
Spark中提供了StopWordsRemover类处理停止词，它可以用作Machine learning Pipeline的一部分。
StopWordsRemover的功能是直接移除所有停用词（stopword），所有从inputCol输入的量都会被它检查，然后再outputCol中，这些停止词都会去掉了。
具体请参考，浪尖文章：[基于DataFrame的StopWordsRemover处理](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484300&idx=2&sn=92e6130aadc992be9d6fd35348b037f6&chksm=9f38e0a4a84f69b2d56f92bf7bfcc1cf24df980741274c16c5f12adcea960ed0963a19b81748&scene=21#wechat_redirect)
**4， CountVectorizer**
CountVectorizer 和 CountVectorizerModel 旨在帮助将文本文档集合转化为频数向量。当先验词典不可用时，CountVectorizer可以用作Estimator提取词汇表，并生成一个CountVectorizerModel。该模型会基于该字典为文档生成稀疏矩阵，该稀疏矩阵可以传给其它算法，比如LDA，去做一些处理。
在拟合过程中，CountVectorizer会从整个文档集合中进行词频统计并排序后的前vocabSize个单词。
一个可选参数minDF也会影响拟合过程，方法是指定词汇必须出现的文档的最小数量（或小于1.0）。另一个可选的二进制切换参数控制输出向量。如果设置为true，则所有非零计数都设置为1.这对于模拟二进制计数而不是整数计数的离散概率模型特别有用。
具体请参考，浪尖的另一篇文章：[CountVectorizer](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484313&idx=1&sn=939b59d6ae32963d4418390db988dc85&chksm=9f38e0b1a84f69a773fe140c21eaae240fd3f398d780d8afe79212c3f89701c66e85b3fa34d5&scene=21#wechat_redirect)
**二   数据**
**20个主题的数据，每篇文章一个文件，每个主题100个文件。共两千个文件。**
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibicF7RDDEic33eia87lotkyibMd21vzTTzvTQicdfWXauZOLdhLAawlbdPajq7QIsyoicU84PVzL0Bs5w/?wx_fmt=png)
**三  实现步骤**
**1， 导入数据**
val corpus = sc.wholeTextFiles("file:///opt/datas/mini_newsgroups/*").map(_._2).map(_.toLowerCase())
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibicF7RDDEic33eia87lotkyibJHRDuvATD5Kune9MEag7d1ia3cMVIztHQvV4VOf7vlsIefbmJvdIqJw/?wx_fmt=png)
**2， 数据格式整理**
val corpus_body = corpus.map(_.split("\n\n")).map(_.drop(1)).map(_.mkString(" "))
val corpus_df = corpus_body.zipWithIndex.toDF("corpus", "id")
import org.apache.spark.ml.feature.RegexTokenizer
val tokenizer = new RegexTokenizer().setPattern("[\W_]+").setMinTokenLength(4).setInputCol("corpus").setOutputCol("tokens")
val tokenized_df = tokenizer.transform(corpus_df)
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibicF7RDDEic33eia87lotkyibhWjkt9ugvnJFrL68wPBNuH26o35VegUlTxqaONh7ibxHQsysveeqpuA/?wx_fmt=png)
**3， 导入停用词**
val stopwords = sc.textFile("file:///opt/datas/stop_words.txt").collect()
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibicF7RDDEic33eia87lotkyibf6EFsgARqrOyqZAwRNLFaecBqrhP41fh0LJEzmiccEZUBHpsRohAIrw/?wx_fmt=png)
**4， 去除停用词**
import org.apache.spark.ml.feature.StopWordsRemover
// Set params for StopWordsRemover
val remover = new StopWordsRemover().setStopWords(stopwords).setInputCol("tokens").setOutputCol("filtered")
// Create new DF with Stopwords removed
val filtered_df = remover.transform(tokenized_df)
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibicF7RDDEic33eia87lotkyibSkBS0bicGglszzPyXDSzp0D96IFDtuvS0nEdy8odAzZJ8mZe3auyncw/?wx_fmt=png)
**5， 生成词频向量**
import org.apache.spark.ml.feature.CountVectorizer
// Set params for CountVectorizer
val vectorizer = new CountVectorizer().setInputCol("filtered").setOutputCol("features").setVocabSize(10000).setMinDF(5).fit(filtered_df)
val countVectors = vectorizer.transform(filtered_df).select("id", "features")
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibicF7RDDEic33eia87lotkyibvGwlhuW1ict9ESICDRLI2eHRduXibMaIESlG7qlNPA2lAzibRVmWlVWNw/?wx_fmt=png)
**6， 构建LDA模型**
import org.apache.spark.ml.clustering.LDA
val numTopics = 20
// Set LDA params
val lda = new LDA().setK(numTopics).setMaxIter(10)
**7， 训练LDA模型**
val model = lda.fit(countVectors )
**8， 查看训练结果数据**
val topicIndices = model.describeTopics(5)
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibicF7RDDEic33eia87lotkyibTuWdNoia9JQZgUgCzNzW6NehUD8vH3DmfpD1g9gwicIBma6kCt7byqGw/?wx_fmt=png)
**9， 词典的使用**
val vocabList = vectorizer.vocabulary
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUibicF7RDDEic33eia87lotkyibJQWO1pelPxdia8EWCic7ARQOlxVEzJbibPC5QHGE3V1CbPfYiafTibDubCw/?wx_fmt=png)
**10,使用模型**
val transformed = model.transform(dataset)
transformed.show(false)
**五  可调整测试点**
**1， 增加stop-words**
val add_stopwords = Array("article", "writes", "entry", "date", "udel", "said", "tell", "think", "know", "just", "newsgroup", "line", "like", "does", "going", "make", "thanks")
val new_stopwords = stopwords.union(add_stopwords)
**2， 使用EM**
用于估计LDA模型的优化器或推理算法，目前Spark支持两种：
**online**：Online Variational Bayes (默认)
**em**: Expectation-Maximization
可以通过调用setOptimizer(value: String)，传入online或者em来使用。
本文设计的数据和Stopwords列表，请在公众号输入：LDA，获取。
**推荐阅读：**
1, [用java提交一个Spark应用程序](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484324&idx=1&sn=9d68b15a297c965c24ccece28fa6cc00&chksm=9f38e08ca84f699ab7f50d02bdfecc8da9c697c672e5764dd1333ca0cf028712b5d556006435&scene=21#wechat_redirect)
2，[SparkMLlib的数据类型讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484349&idx=2&sn=14f50cd925991205d5a971b4e434b8ee&chksm=9f38e095a84f698332725aa5072c8aca2acef3582ae4c5c2eedd592eea7c15fcb9f9b5ec8db7&scene=21#wechat_redirect)
3，[CountVectorizer](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484313&idx=1&sn=939b59d6ae32963d4418390db988dc85&chksm=9f38e0b1a84f69a773fe140c21eaae240fd3f398d780d8afe79212c3f89701c66e85b3fa34d5&scene=21#wechat_redirect)
4，[请别再问我Spark的MLlib和ML库的区别](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484273&idx=1&sn=830eee29f7078dd85d6b3156a2880acd&chksm=9f38e059a84f694f99480c6e831d788187d1943704ca69f881721bf40dbf9745c8701902c4db&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**


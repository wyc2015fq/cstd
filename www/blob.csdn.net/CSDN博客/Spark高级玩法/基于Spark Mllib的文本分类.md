# 基于Spark Mllib的文本分类 - Spark高级玩法 - CSDN博客
2017年12月20日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1313
**基于Spark Mllib的文本分类**
文本分类是一个典型的机器学习问题，其主要目标是通过对已有语料库文本数据训练得到分类模型，进而对新文本进行类别标签的预测。这在很多领域都有现实的应用场景，如新闻网站的新闻自动分类，垃圾邮件检测，非法信息过滤等。本文将通过训练一个手机短信样本数据集来实现新数据样本的分类，进而检测其是否为垃圾消息，基本步骤是：首先将文本句子转化成单词数组，进而使用 Word2Vec 工具将单词数组转化成一个 K 维向量，最后通过训练 K 维向量样本数据得到一个前馈神经网络模型，以此来实现文本的类别标签预测。本文案例实现上采用 Spark ML 中的词向量化工具 Word2Vec 和多层感知器分类器 (Multiple Layer Perceptron Classifier)
**Word2Vec简介**
Word2Vec 是一个用来将词表示为数值型向量的工具，其基本思想是将文本中的词映射成一个 K 维数值向量 (K 通常作为算法的超参数)，这样文本中的所有词就组成一个 K 维向量空间，这样我们可以通过计算向量间的欧氏距离或者余弦相似度得到文本语义的相似度。Word2Vec 采用的是 Distributed representation 的词向量表示方式，这种表达方式不仅可以有效控制词向量的维度，避免维数灾难 (相对于 one-hot representation)，而且可以保证意思相近的词在向量空间中的距离较近。
Word2Vec 实现上有两种模型 CBOW (Continuous Bag of Words, 连续词袋模型) 和 Skip-Gram，简单概括一下区别就是：CBOW 是根据语境预测目标单词，Skip-Gram 根据当前单词预测语境。Spark 的实现采用的是 Skip-Gram 模型 。假设我们有 N 个待训练的单词序列样本，记作 w1,w2...wn, Skip-Gram 模型的训练目标是最大化平均对数似然，即
![?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUiaK77FNmEsmPC6MgZhdn341WlJa99unAicfUK7w9BCovIxINNGtADQW89zcIibjPpAthbVleN8z27g/?wx_fmt=png&wxfrom=5&wx_lazy=1)
其中 N 是词个数，K 是词上下文的窗口大小。Skip-Gram 模型中一定上下文窗口内的词两两之间都会计算概率，并且通常情况下，上下文窗口越大所能涵盖的词组合情况就越全面，这样可以带来更加精确的结果，但是缺点是也会增加训练时间。
在 Skip-Gram 模型里，每个单词都关联两个向量，分别表示词向量和上下文向量。也正是因为如此，Word2Vec 较之传统的 LDA(Latent Dirichlet Allocation) 过程，可以表达更加丰富和准确的语义信息。
Spark 的 Word2Vec 实现提供以下主要可调参数：
- 
inputCol , 源数据 DataFrame 中存储文本词数组列的名称。
- 
outputCol, 经过处理的数值型特征向量存储列名称。
- 
vectorSize, 目标数值向量的维度大小，默认是 100。
- 
windowSize, 上下文窗口大小，默认是 5。
- 
numPartitions, 训练数据的分区数，默认是 1。
- 
maxIter，算法求最大迭代次数，小于或等于分区数。默认是 1.
- 
minCount, 只有当某个词出现的次数大于或者等于 minCount 时，才会被包含到词汇表里，否则会被忽略掉。
- 
stepSize，优化算法的每一次迭代的学习速率。默认值是 0.025.
这些参数都可以在构造 Word2Vec 实例的时候通过 setXXX 方法设置。
**多层感知器**
多层感知器 (MLP, Multilayer Perceptron) 是一种多层的前馈神经网络模型，所谓前馈型神经网络，指其从输入层开始只接收前一层的输入，并把计算结果输出到后一层，并不会给前一层有所反馈，整个过程可以使用有向无环图来表示。该类型的神经网络由三层组成，分别是输入层 (Input Layer)，一个或多个隐层 (Hidden Layer)，输出层 (Output Layer)，如图所示：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUiaK77FNmEsmPC6MgZhdn34iayWwe0cUc3NaUiaJexVcA6sxgFF49ibriaDiao4n2rUXIsMMUEML80FNDQ/?wx_fmt=png)
Spark ML 在 1.5 版本后提供一个使用 BP(反向传播，Back Propagation) 算法训练的多层感知器实现，BP 算法的学习目的是对网络的连接权值进行调整，使得调整后的网络对任一输入都能得到所期望的输出。BP 算法名称里的反向传播指的是该算法在训练网络的过程中逐层反向传递误差，逐一修改神经元间的连接权值，以使网络对输入信息经过计算后所得到的输出能达到期望的误差。Spark 的多层感知器隐层神经元使用 sigmoid 函数作为激活函数，输出层使用的是 softmax 函数。
Spark 的多层感知器分类器 (MultilayerPerceptronClassifer) 支持以下可调参数:
- 
featuresCol:输入数据 DataFrame 中指标特征列的名称。
- 
labelCol：输入数据 DataFrame 中标签列的名称。
- 
layers:这个参数是一个整型数组类型，第一个元素需要和特征向量的维度相等，最后一个元素需要训练数据的标签取值个数相等，如 2 分类问题就写 2。中间的元素有多少个就代表神经网络有多少个隐层，元素的取值代表了该层的神经元的个数。例如val layers = Array[Int](100,6,5,2)。
- 
maxIter：优化算法求解的最大迭代次数。默认值是 100。
- 
predictionCol:预测结果的列名称。
- 
tol:优化算法迭代求解过程的收敛阀值。默认值是 1e-4。不能为负数。
- 
blockSize:该参数被前馈网络训练器用来将训练样本数据的每个分区都按照 blockSize 大小分成不同组，并且每个组内的每个样本都会被叠加成一个向量，以便于在各种优化算法间传递。该参数的推荐值是 10-1000，默认值是 128。
算法的返回是一个 MultilayerPerceptronClassificationModel 类实例。
**目标数据集预览**
在引言部分，笔者已经简要介绍过了本文的主要任务，即通过训练一个多层感知器分类模型来预测新的短信是否为垃圾短信。在这里我们使用的目标数据集是来自 UCI 的 SMS Spam Collection 数据集，该数据集结构非常简单，只有两列，第一列是短信的标签 ，第二列是短信内容，两列之间用制表符 (tab) 分隔。**虽然 UCI 的数据集是可以拿来免费使用的，但在这里笔者依然严正声明该数据集的版权属于 UCI 及其原始贡献者。**
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUiaK77FNmEsmPC6MgZhdn34pq7qdbMoWw2vTnuPsv8gpia6bgPxQqKe2KYkFedVQZ1AAI0BEzyyurw/?wx_fmt=png)
**数据集下载链接：http://archive.ics.uci.edu/ml/datasets/SMS+Spam+Collection**
**案例分析与实现**
在处理文本短信息分类预测问题的过程中，笔者首先是将原始文本数据按照 8:2 的比例分成训练和测试数据集。整个过程分为下面几个步骤
- 
从本地读取原始数据集，并创建一个 DataFrame。
- 
使用 StringIndexer 将原始的文本标签 (“Ham”或者“Spam”) 转化成数值型的表型，以便 Spark ML 处理。
- 
使用 Word2Vec 将短信文本转化成数值型词向量。
- 
使用 MultilayerPerceptronClassifier 训练一个多层感知器模型。
- 
使用 LabelConverter 将预测结果的数值标签转化成原始的文本标签。
- 
最后在测试数据集上测试模型的预测精确度。
算法的具体实现如下：
**1， 首先导入包**
import org.apache.spark.ml.Pipeline
import org.apache.spark.ml.classification.MultilayerPerceptronClassifier
import org.apache.spark.ml.evaluation.MulticlassClassificationEvaluator
import org.apache.spark.ml.feature.{IndexToString, StringIndexer, Word2Vec}
**2， 创建 集并分词**
val parsedRDD = sc.textFile("file:///opt/datas/SMSSpamCollection").map(_.split("	")).map(eachRow => {
(eachRow(0),eachRow(1).split(" "))
})
val msgDF = spark.createDataFrame(parsedRDD).toDF("label","message")
**3， 将标签转化为索引值**
val labelIndexer = new StringIndexer().setInputCol("label").setOutputCol("indexedLabel").fit(msgDF)
**4， 创建Word2Vec，分词向量大小100**
final val VECTOR_SIZE = 100
val word2Vec = new Word2Vec().setInputCol("message").setOutputCol("features").setVectorSize(VECTOR_SIZE).setMinCount(1)
**5， 创建多层感知器**
输入层VECTOR_SIZE个，中间层两层分别是6，,5个神经元，输出层2个
val layers = Array[Int](VECTOR_SIZE,6,5,2)
val mlpc = new MultilayerPerceptronClassifier().setLayers(layers).setBlockSize(512).setSeed(1234L).setMaxIter(128).setFeaturesCol("features").setLabelCol("indexedLabel").setPredictionCol("prediction")
**6， 将索引转换为原有标签**
val labelConverter = new IndexToString().setInputCol("prediction").setOutputCol("predictedLabel").setLabels(labelIndexer.labels)
**7， 数据集分割**
val Array(trainingData, testData) = msgDF.randomSplit(Array(0.8, 0.2))
**8， 创建pipeline并训练数据**
val pipeline = new Pipeline().setStages(Array(labelIndexer,word2Vec,mlpc,labelConverter))
val model = pipeline.fit(trainingData)
val predictionResultDF = model.transform(testData)
//below 2 lines are for debug use
predictionResultDF.printSchema
predictionResultDF.select("message","label","predictedLabel").show(30)
**9， 评估训练结果**
val evaluator = new MulticlassClassificationEvaluator().setLabelCol("indexedLabel").setPredictionCol("prediction").setMetricName("precision")
val predictionAccuracy = evaluator.evaluate(predictionResultDF)
println("Testing Accuracy is %2.4f".format(predictionAccuracy * 100) + "%")
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUiaK77FNmEsmPC6MgZhdn34mhiadrkoDfJIibUzZ1KrB12gGXibqXcZVbPmtCN61WMSDibJdzIWlpFbCQ/?wx_fmt=png)
本文的参考链接
1，http://spark.apache.org/docs/latest/ml-guide.html
2，https://www.ibm.com/developerworks/cn/opensource/os-cn-spark-practice6/index.html
**要读懂此文，先要读懂Spark ML pipeline，可以阅读，推荐阅读里浪尖翻译的，也可以阅读Spark官网，假如你英语好的话。**
推荐阅读：
1，[SparkML模型选择（超参数调整）与调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484330&idx=1&sn=49095988b3bb00259f79432622ad324f&chksm=9f38e082a84f69943044b515643ce051a4d88154fbe0574b24240aa85a501a5846d83c9bc714&scene=21#wechat_redirect)
2，[Spark的Ml pipeline](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484270&idx=1&sn=69153063accd105649d4466d938012da&chksm=9f38e046a84f69505d954726c65d940f9a370342755ae2ce7456134ef4b9a6eafb1350a4dfb6&scene=21#wechat_redirect)
3，[读懂Word2Vec之Skip-Gram](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484321&idx=1&sn=ec14f031ffc7dbc699502a9d0b09f64b&chksm=9f38e089a84f699fb1ca5903124a69ab2286ad5159c6b90d8ee677f1bdcde523a121a8249ced&scene=21#wechat_redirect)
4，[SparkMLLib中基于DataFrame的TF-IDF](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484310&idx=1&sn=acf0a573d24c112a4a20370f5430848f&chksm=9f38e0bea84f69a8d50db328b373fdd8a867e2caee3262236c1cf57188bf87cefdfaa456607a&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)

**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

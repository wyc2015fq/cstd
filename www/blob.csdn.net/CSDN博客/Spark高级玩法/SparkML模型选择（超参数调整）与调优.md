# SparkML模型选择（超参数调整）与调优 - Spark高级玩法 - CSDN博客
2017年12月10日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1864
![?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxicMib1TelMcjboznqSafxVNRsW9gGZVCaDUic9rdYzu35xbVXjdn9TMmGFgN2MIekvgZWh58IETibg/?wx_fmt=png&wxfrom=5&wx_lazy=1)
Spark ML模型选择与调优
本文主要讲解如何使用Spark MLlib的工具去调优ML算法和Pipelines。内置的交叉验证和其他工具允许用户优化算法和管道中的超参数。
**模型选择（又称为超参数调整）**
ML中的一个重要任务是模型选择，或者使用数据来找出给定任务的最佳模型或参数。这也被称为调优。可以针对单个独立的Estimator进行调优，例如LogisticRegression，也可以针对整个Pipeline进行调优。用户可以一次针对整个pipeline进行调优，而不是单独调优pipeline内部的元素。
Mllib支持模型选择，可以使用工具**CrossValidator **和**TrainValidationSplit**，这些工具支持下面的条目：
**Estimator**：需要调优的算法或者pipeline。
**ParamMaps**的集合：可供选择的参数，有时称为用来搜索“参数网格”
**Evaluator**：度量标准来衡量一个拟合Model在测试数据上的表现
在高层面上，这些模型选择工具的作用如下：
- 
他们将输入数据分成单独的训练和测试数据集
- 
对每个(训练，测试)对，他们迭代遍历ParamMaps集合：对于每一个ParamMap，他们使用这些参数调用Estimator的fit，得到拟合Model，并使用Evaluator评估Model的性能。
- 
他们选择由产生的最佳性能参数生成的模型。
Evaluator可以是RegressionEvaluator 用于回归问题中，BinaryClassificationEvaluator 对于二分类，或MulticlassClassificationEvaluator 为多类问题。用于选择最佳值ParamMap的默认度量指标可以被evaluators的setMetricName方法覆盖。
**Cross-Validation-交叉验证**
CrossValidator开始的时候会将数据分割成很多测试集和训练集对儿。例如，k=3folds，crossValidator将会产生三组(training,test)数据集对儿，没对都是2/3用来训练，1/3用来测试。为了评估出一个组特殊的paramMap，crossValidator 会计算通过Estimator在三组不同数据集上调用fit产生的3个模型的平均评估指标。
确定最佳ParamMap后，CrossValidator最后使用最佳ParamMap和整个数据集重新拟合Estimator。
**例子**
以下示例演示如何使用CrossValidator从参数网格中进行选择。
请注意，参数网格上的交叉验证非常耗性能的。例如，在下面的例子中，参数网格中hashingTF.numFeatures有三个值，并且lr.regParam两个值，CrossValidator使用了2folds。将会倍增到(3×2)×2=12模型需要训练。在现实的设置中，尝试更多的参数并且使用更多的folds(k=3，k=10是非常常见的)。换句话说使用交叉验证代价是非常大的。然而，它也是一个比较合理的方法，用于选择比启发式手调整更具统计稳健性的参数。
import org.apache.spark.ml.Pipeline
import org.apache.spark.ml.classification.LogisticRegression
import org.apache.spark.ml.evaluation.BinaryClassificationEvaluator
import org.apache.spark.ml.feature.{HashingTF, Tokenizer}
import org.apache.spark.ml.linalg.Vector
import org.apache.spark.ml.tuning.{CrossValidator, ParamGridBuilder}
import org.apache.spark.sql.Row
//准备训练数据，格式(id,text,label)
val training = spark.createDataFrame(Seq(
  (0L, "a b c d e spark", 1.0),
(1L, "b d", 0.0),
(2L, "spark f g h", 1.0),
(3L, "hadoop mapreduce", 0.0),
(4L, "b spark who", 1.0),
(5L, "g d a y", 0.0),
(6L, "spark fly", 1.0),
(7L, "was mapreduce", 0.0),
(8L, "e spark program", 1.0),
(9L, "a e c l", 0.0),
(10L, "spark compile", 1.0),
(11L, "hadoop software", 0.0)
)).toDF("id", "text", "label")
//配置一个ML pipeline，总共有三个stages：tokenizer, hashingTF, and lr
val tokenizer = new Tokenizer()
  .setInputCol("text")
  .setOutputCol("words")
val hashingTF = new HashingTF()
  .setInputCol(tokenizer.getOutputCol)
  .setOutputCol("features")
val lr = new LogisticRegression()
  .setMaxIter(10)//输入label，features，prediction均可采用默认值名称。
val pipeline = new Pipeline()
  .setStages(Array(tokenizer, hashingTF, lr))
//用ParamGridBuilder构建一个查询用的参数网格
//hashingTF.numFeatures有三个值，lr.regParam有两个值，
//该网格将会有3*2=6组参数被CrossValidator使用
val paramGrid = new ParamGridBuilder()
  .addGrid(hashingTF.numFeatures, Array(10, 100, 1000))
  .addGrid(lr.regParam, Array(0.1, 0.01))
  .build()
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxicMib1TelMcjboznqSafxVzCjickDXqmyqYdVbfK0qog6WWzGlibqjiaeu9BmkPl2xfMwQo9FjPglDw/?wx_fmt=png)
//这里对将整个PipeLine视为一个Estimator
//这种方式允许我们联合选择这个Pipeline stages参数
//一个CrossValidator需要一个Estimator，一组Estimator ParamMaps，一个Evaluator。
//这个Evaluator是一个BinaryClassificationEvaluator，它默认度量是areaUnderROC
val cv = new CrossValidator()
  .setEstimator(pipeline)
  .setEvaluator(new BinaryClassificationEvaluator)
  .setEstimatorParamMaps(paramGrid)
  .setNumFolds(2)  // 生产中使用3+
// 运行交叉验证，选择最佳参数
val cvModel = cv.fit(training)
//准备测试文档，这些文档是未打标签的
val test = spark.createDataFrame(Seq(
  (4L, "spark i j k"),
(5L, "l m n"),
(6L, "mapreduce spark"),
(7L, "apache hadoop")
)).toDF("id", "text")
//使用训练好的最佳模型，去对测试集进行预测。
cvModel.transform(test)
  .select("id", "text", "probability", "prediction")
  .collect()
  .foreach { case Row(id: Long, text: String, prob: Vector, prediction: Double) =>
println(s"($id, $text) --> prob=$prob, prediction=$prediction")
  }
查看预测结果
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxicMib1TelMcjboznqSafxVoRJaldM6Hx37AAgFkymsqAGSRlZzE7cULxB3JDsibMFnZJSRfzWBt8w/?wx_fmt=png)
**TrainValidationSplit**
除了CrossValidator，spark还提供了TrainValidationSplit用于超参数的调整。TrainValidationSplit只对一次参数的每个组合进行一次评估，与CrossValidator的k词调整相对。真就意味着代价相对少了一些，当训练集不是很大的时候，将不会产生一个可靠的结果。
不像CrossValidator，TrainValidationSplit产生一个(training，test)数据集对。通过使用trainRatio参数将数据集分割成两个部分。例如，trainRatio=0.75,TrainValidationSplit将会产生一个训练集和一个测试集，其中75%数据用来训练，25%数据用来验证。
和CrossValidator一样, TrainValidationSplit在最后会使用最佳的参数和整个数据集对Estimator进行拟合。
**例子**
import org.apache.spark.ml.evaluation.RegressionEvaluator
import org.apache.spark.ml.regression.LinearRegression
import org.apache.spark.ml.tuning.{ParamGridBuilder, TrainValidationSplit}
// 准测试数据
val data = spark.read.format("libsvm").load("data/mllib/sample_linear_regression_data.txt")
val Array(training, test) = data.randomSplit(Array(0.9, 0.1), seed = 12345)
val lr = new LinearRegression()
    .setMaxIter(10)
// We use a ParamGridBuilder to construct a grid of parameters to search over.
// TrainValidationSplit will try all combinations of values and determine best model using
// the evaluator.
//使用ParamGridBuilder构建一个parameters网格，用来存储查询参数
//TrainValidationSplit会尝试所有值的组合使用evaluator来产生一个最佳模型
val paramGrid = new ParamGridBuilder()
  .addGrid(lr.regParam, Array(0.1, 0.01))
  .addGrid(lr.fitIntercept)
  .addGrid(lr.elasticNetParam, Array(0.0, 0.5, 1.0))
  .build()
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxicMib1TelMcjboznqSafxV7wVmsfWT4V2SYHc5Dd5YH2ibh7gmzw0MuzzGePjnmaq7ibbUUnbquMQA/?wx_fmt=png)
// A TrainValidationSplit requires an Estimator, a set of Estimator ParamMaps, and an Evaluator.
//在这个例子中，Estimator选用简单的线性回归模型
val trainValidationSplit = new TrainValidationSplit()
  .setEstimator(lr)
  .setEvaluator(new RegressionEvaluator)
  .setEstimatorParamMaps(paramGrid)
//80%数据用来训练，20%用来验证
.setTrainRatio(0.8)
//运行TrainValidationSplit，选出最佳参数
val model = trainValidationSplit.fit(training)
//对测试数据进行预测。参数就是刚刚训练的最佳参数。
model.transform(test)
  .select("features", "label", "prediction")
  .show()
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUxicMib1TelMcjboznqSafxVzF5UAMCaUA0pzrbn2CbZCnLvqTEHWhxicZGaz4OlxLfHUuialR41yySQ/?wx_fmt=png)
**推荐阅读：**
1，[SparkMLLib中基于DataFrame的TF-IDF](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484310&idx=1&sn=acf0a573d24c112a4a20370f5430848f&chksm=9f38e0bea84f69a8d50db328b373fdd8a867e2caee3262236c1cf57188bf87cefdfaa456607a&scene=21#wechat_redirect)
2，[读懂Word2Vec之Skip-Gram](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484321&idx=1&sn=ec14f031ffc7dbc699502a9d0b09f64b&chksm=9f38e089a84f699fb1ca5903124a69ab2286ad5159c6b90d8ee677f1bdcde523a121a8249ced&scene=21#wechat_redirect)
3，[基于java的中文分词工具ANSJ](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484304&idx=1&sn=2c8280e8ec0c008074d2cf958c7d7489&chksm=9f38e0b8a84f69aed49600507e469f42c6697b77ab2edfdf3ef8904f46990d10c77932845d0a&scene=21#wechat_redirect)
4，[Scala语法基础之隐式转换](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483864&idx=1&sn=4e1e742e00b2eabcddd24d52e70a5df9&chksm=9f38e2f0a84f6be6b5201ecfead986c83d1f2f031536cc731ef8323f28c2daa92595cabf8d4f&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**


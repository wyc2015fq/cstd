# 深入理解Spark ML：多项式朴素贝叶斯原理与源码分析 - Soul Joy Hub - CSDN博客

2017年07月27日 12:27:54[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：2994
所属专栏：[Spark机器学习](https://blog.csdn.net/column/details/15014.html)



[http://blog.csdn.net/u011239443/article/details/76176743](http://blog.csdn.net/u011239443/article/details/76176743)

朴素贝叶斯的基本原理与简单的python与scala的实现可以参阅：[http://blog.csdn.net/u011239443/article/details/68061124](http://blog.csdn.net/u011239443/article/details/68061124)

# 贝叶斯估计

如果一个给定的类和特征值在训练集中没有一起出现过，那么基于频率的估计下该概率将为0。这将是一个问题。因为与其他概率相乘时将会把其他概率的信息统统去除。所以常常要求要对每个小类样本的概率估计进行修正，以保证不会出现有为0的概率出现。常用到的平滑就是加1平滑（也称拉普拉斯平滑）：

$\large \color{blue}{P(X^{j}=a_{jl}|Y=c_k)=\frac{\sum^N_{i=1}I(x_i^{(j)}=a_{jl},y_i=c_k)+lambda}{\sum^N_{i=1}I(y_i=c_k)+S_jlambda}}$

lambda>=0，等价于在随机变量各个取值的频数上赋予一个正数lambda>0。$S_j$是特征$X_j$取值的类别数，因此使用上式依然有：

$\large \color{blue}{\sum_{l=1}^{S_j}P(X^{j}=a_{jl}|Y=c_k)=1}$

同样的：

$\large \color{blue}{P(Y=c_k)=\frac{\sum^N_{i=1}I(y_i=c_k)+lambda}{N+Klambda}}$

N为数据条数，K为label类别数。

# 多项式朴素贝叶斯

多项式朴素贝叶斯和上述贝叶斯模型不同的是，上述贝叶斯模型对于某特征的不同取值代表着不同的类别，而多项式朴素贝叶斯对于某特征的不同取值代表着该特征决定该label类别的重要程度。

比如一个文本中，单词`Chinese`出现的频数，1次还是10次，并不代表着`Chinese`单词这个特征的类别，而代表着`Chinese`单词这个特征的决定该文本label类别的重要程度。

$\large \color{blue}{log(p(y_i)) = log(\sum^N_{i=1}I(y_i=c_k)+lambda) - log(N+Klambda)}$

$\large \color{blue}{log(P(a_{j}|y_i))=log(\sum_{i=1}^Na_{j,y_i=c_k}+lambda)-log(\sum_{i=1}^N\sum_{j=1}^na_{j,y_i=c_k}+nlambda)}$

n为特征维度数

我们来举个例子：

![这里写图片描述](https://img-blog.csdn.net/20170727145641921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们设lambda为1，共有6个不同的单词，则特征维度数为6。

![这里写图片描述](https://img-blog.csdn.net/20170727152524856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170727152552079?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170727152618062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170727152701004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

所以，我们将d5 分类到 `yes`

# API 使用

下面是Spark 朴素贝叶斯的使用例子：

```
import org.apache.spark.ml.classification.NaiveBayes

// 加载数据
val data = spark.read.format("libsvm").load("data/mllib/sample_libsvm_data.txt")

// 切分数据集与训练集
val Array(trainingData, testData) = data.randomSplit(Array(0.7, 0.3), seed = 1234L)

// 训练朴素贝叶斯模型
val model = new NaiveBayes()
  .fit(trainingData)

// 预测
val predictions = model.transform(testData)
predictions.show()
```

# 源码分析

接下来我们来分析下源码～

## NaiveBayes

### train

`NaiveBayes().fit`调用`NaiveBayes`的父类`Predictor`中的`fit`，将`label`和`weight`转为`Double`，保存`label`和`weight`原信息，最后调用`NaiveBayes`的`train`：

```
override protected def train(dataset: Dataset[_]): NaiveBayesModel = {
    trainWithLabelCheck(dataset, positiveLabel = true)
  }
```

### trainWithLabelCheck：

ml假设输入labels范围在[0, numClasses). 但是这个实现也被mllib NaiveBayes调用，它允许其他类型的输入labels如{-1, +1}. `positiveLabel` 用于确定label是否需要被检查。

```
private[spark] def trainWithLabelCheck(
      dataset: Dataset[_],
      positiveLabel: Boolean): NaiveBayesModel = {
      //检测label
    if (positiveLabel && isDefined(thresholds)) {
      val numClasses = getNumClasses(dataset)
      require($(thresholds).length == numClasses, this.getClass.getSimpleName +
        ".train() called with non-matching numClasses and thresholds.length." +
        s" numClasses=$numClasses, but thresholds has length ${$(thresholds).length}")
    }
    //模型类型 多项式朴素贝叶斯是  Multinomial
    val modelTypeValue = $(modelType)
    val requireValues: Vector => Unit = {
      modelTypeValue match {
        case Multinomial =>
          // 确认所有的值非负
          // values.forall(_ >= 0.0)
          requireNonnegativeValues
        ......
      }
    }
    // Instrumentation 是 一个小封装，用来定义为一个estimator定义一个training session和该session中有学用的信息的log方法
    val instr = Instrumentation.create(this, dataset)
    instr.logParams(labelCol, featuresCol, weightCol, predictionCol, rawPredictionCol,
      probabilityCol, modelType, smoothing, thresholds)
    // 得到特征维度数，即公式中的 n
    val numFeatures = dataset.select(col($(featuresCol))).head().getAs[Vector](0).size
    instr.logNumFeatures(numFeatures)
    // 得到记录的权重 为设置 默认为 1.0
    val w = if (!isDefined(weightCol) || $(weightCol).isEmpty) lit(1.0) else col($(weightCol))

    // 聚合
    val aggregated = dataset.select(col($(labelCol)), w, col($(featuresCol))).rdd
      .map { row => (row.getDouble(0), (row.getDouble(1), row.getAs[Vector](2)))
      // 根据key labelCol 进行聚合
      // value 的初始值为 0.0,Vectors.zeros(numFeatures).toDense
      }.aggregateByKey[(Double, DenseVector)]((0.0, Vectors.zeros(numFeatures).toDense))(
      // 合并在同一个partition中的值
      seqOp = {
         case ((weightSum: Double, featureSum: DenseVector), (weight, features)) =>
           requireValues(features)
           // featureSum = featureSum + weight × features
           BLAS.axpy(weight, features, featureSum)
           (weightSum + weight, featureSum)
      },
      //合并不同partition中的值
      combOp = {
         case ((weightSum1, featureSum1), (weightSum2, featureSum2)) =>
           BLAS.axpy(1.0, featureSum2, featureSum1)
           (weightSum1 + weightSum2, featureSum1)
      }).collect().sortBy(_._1)

    // label 的类别数，即公式中的 K
    val numLabels = aggregated.length
    instr.logNumClasses(numLabels)
    // 文档数，即公式中的 N
    val numDocuments = aggregated.map(_._2._1).sum

    val labelArray = new Array[Double](numLabels)
    // 用来存 log(p(y_i)
    val piArray = new Array[Double](numLabels)
    // 用来存 log(P(a_{j}|y_i))
    val thetaArray = new Array[Double](numLabels * numFeatures)

    val lambda = $(smoothing)
    // log(N+Klambda)
    val piLogDenom = math.log(numDocuments + numLabels * lambda)
    var i = 0
    aggregated.foreach { case (label, (n, sumTermFreqs)) =>
      labelArray(i) = label
      // log(p(y_i)) = log(\sum^N_{i=1}I(y_i=c_k)+lambda) - log(N+Klambda)
      piArray(i) = math.log(n + lambda) - piLogDenom
      // log(\sum_{i=1}^N\sum_{j=1}^na_{j,y_i=c_k}+nlambda)
      val thetaLogDenom = $(modelType) match {
        case Multinomial => math.log(sumTermFreqs.values.sum + numFeatures * lambda)
       ......
      }
      var j = 0
      while (j < numFeatures) {
      // log(P(a_{j}|y_i))=log(\sum_{i=1}^Na_{j,y_i=c_k}+lambda)-log(\sum_{i=1}^N\sum_{j=1}^na_{j,y_i=c_k}+nlambda)
        thetaArray(i * numFeatures + j) = math.log(sumTermFreqs(j) + lambda) - thetaLogDenom
        j += 1
      }
      i += 1
    }

    val pi = Vectors.dense(piArray)
    val theta = new DenseMatrix(numLabels, numFeatures, thetaArray, true)
    // 生成并返回模型
    val model = new NaiveBayesModel(uid, pi, theta).setOldLabels(labelArray)
    instr.logSuccess(model)
    model
  }
```

## NaiveBayesModel

### transform

`model.transform`调用`NaiveBayesModel`的父类`ProbabilisticClassificationModel`中的`transform`，根据表列配置，有选择的预测并添加以下三列：
- predicted labels：`Double`类型，预测的label
- raw predictions：`Vector`类型，数字可为负数，数值越大，表示该类别越可行
- probability of each class：`Vector`类型，各类别的概率

这边我们就只分析`predicted labels`流程：`transform`最终会调用`predict`：

```
override protected def predict(features: FeaturesType): Double = {
    raw2prediction(predictRaw(features))
  }
```

`predictRaw`其实就是在计算`raw predictions`，而`raw2prediction`正是在从中选取最可信的：

```
// 返回之大值的坐标
protected def raw2prediction(rawPrediction: Vector): 
Double = rawPrediction.argmax
```

### predictRaw

下面我们来看看`NaiveBayesModel`的`predictRaw`实现：

```
override protected def predictRaw(features: Vector): Vector = {
    $(modelType) match {
      case Multinomial =>
        multinomialCalculation(features)
      ......
    }
  }
```

multinomialCalculation:

```
private def multinomialCalculation(features: Vector) = {
    // 求各个label类别下的条件概率
    val prob = theta.multiply(features)
    // 加上先验概率：prob = prob + 1.0 * pi
    BLAS.axpy(1.0, pi, prob)
    prob
  }
```

![这里写图片描述](https://img-blog.csdn.net/20170727214034202?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


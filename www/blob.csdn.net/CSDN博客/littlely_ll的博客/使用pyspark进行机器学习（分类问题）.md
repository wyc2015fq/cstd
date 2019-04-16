# 使用pyspark进行机器学习（分类问题） - littlely_ll的博客 - CSDN博客





2017年10月02日 18:31:03[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：4211








[使用pyspark进行机器学习（聚类问题）](http://blog.csdn.net/littlely_ll/article/details/78155192)
[使用pyspark进行机器学习（回归问题）](http://blog.csdn.net/littlely_ll/article/details/78161574)

# LogisticRegression
`class pyspark.ml.classification.LogisticRegression(self, featuresCol="features", labelCol="label", predictionCol="prediction", maxIter=100, regParam=0.0, elasticNetParam=0.0, tol=1e-6, fitIntercept=True, threshold=0.5, thresholds=None, probabilityCol="probability", rawPredictionCol="rawPrediction", standardization=True, weightCol=None, aggregationDepth=2, family="auto")`
## 参数解释

```
regParam: 正则化参数(>=0)
elasticNetParam: ElasticNet混合参数，0-1之间，当alpha为0时,惩罚为L2正则化，当为1时为L1正则化
fitIntercept: 是否拟合一个截距项
Standardization： 是否在拟合数据之前对数据进行标准化
aggregationDepth: 树聚合所建议的深度(>=2)
Family：标签分布簇的名称，可选：“auto", "binomial", "multinomial"
参数的getter和setter方法
```

## 拟合后的模型拥有的方法和属性

```
coefficientMatrix: 模型的系数矩阵(对于multinomial而言)
coefficients: 双变量logistic回归的模型系数，如果是多元Logistic回归的话会有异常
evaluate(dataset):在测试集上评估模型
hasSummary: 是否有summary
intercept: 二变量logistic模型的截距
interceptVector： 多变量logistic模型截距
summary：获得summary
transform(dataset,param=None)
Summary拥有的属性
predictions: 模型transform方法输出的预测数据框
probabilityCol： 给出每个类的概率
对于双变量的summary而言拥有的属性：
areaUnderROC: 计算AUC
fMeasureByTreshold: 返回带有两个字段(阈值，F-统计量)的数据框，beta=1.0
pr： 返回精度-召回率两字段的数据框
precisionByTreshold：返回带有阈值，精度两字段的数据框，应用了从转换后数据里的所有可能概率作为阈值来计算精度
recallByTreshold： 返回带有阈值，召回率两字段的数据框，应用了从转换后数据里的所有可能概率作为阈值来计算召回率
roc：返回带有两字段FPR, TPR的数据框，
```

## 代码

```
from pyspark.ml.linalg import Vectors
from pyspark.sql import Row
from pyspark.ml.classification import LogisticRegression

#双变量Logistic回归
bdf = sc.parallelize([Row(label=1.0,weight=2.0,features=Vectors.dense(1.0)),Row(label=0.0, weight=2.0, features=Vectors.sparse(1,[],[]))]).toDF()
bdf.show()
blor = LogisticRegression(maxIter=5, regParam=0.01,weightCol='weight')
blorModel = blor.fit(bdf)
blorModel.coefficients
blorModel.intercept

#多元Logistic回归
mdf = sc.parallelize([Row(label=1.0,weight=2.0, features=Vectors.dense(1.0)),Row(label=0.0,weight=2.0, features=Vectors.sparse(1,[],[])),Row(label=2.,weight=2.0, features=Vectors.dense(3.0))]).toDF()
mlor=LogisticRegression(maxIter=5,regParam=0.01,weightCol='weight',family='multinomial')
mlorModel = mlor.fit(mdf)
print mlorModel.coefficientMatrix
mlorModel.interceptVector

#模型预测
test0=sc.parallelize([Row(features=Vectors.dense(-1.0))]).toDF()
result = blorModel.transform(test0).head()
result.prediction

result.probability
result.rawPrediction

test1 = sc.parallelize([Row(features=Vectors.sparse(1,[0],[1.0]))]).toDF()
blorModel.transform(test1).head().prediction
blorModel.transform(test1).show()
#模型评估
blorModel.summary.roc.show()
blorModel.summary.pr.show()
```

# DecisionTreeClassifier
`class pyspark.ml.classification.DecisionTreeClassifier(self, featuresCol="features", labelCol="label", predictionCol="prediction", probabilityCol="probability", rawPredictionCol="rawPrediction", maxDepth=5, maxBins=32, minInstancesPerNode=1, minInfoGain=0.0, maxMemoryInMB=256, cacheNodeIds=False, checkpointInterval=10, impurity="gini", seed=None)`
## 参数解释

```
checkpointInterval：设置checkpoint区间(>=1)，或宕掉checkpoint(-1)，例如10意味着缓冲区(cache)将会每迭代10次获得一次checkpoint
fit(datasset,params=None)
impurity: 信息增益计算的准则，选项"entropy", "gini"
maxBins： 连续特征离散化的最大分箱，必须>=2 并且>=分类特征分类的数量
maxDepth：树的最大深度
minInfoGain：分割结点所需的最小的信息增益
minInstancesPerNode：每个结点最小实例个数
参数的getter和setter方法
```

## 拟合后的模型拥有的方法和属性

```
depth：返回决策树深
featureImportances：估计每个特征的重要性，特征重要性的计算：
    • 特征J的重要性=通过特征j所分割结点的信息增益的和，信息增益由通过该结点的实例个数进行标准化
    • 整个树的标准化的重要性和为1
transform(dataset,params=None)方法
```

## 代码

```
from pyspark.ml.linalg import Vectors from pyspark.ml.feature import StringIndexer
df = spark.createDataFrame([(1.0, Vectors.dense(1.0)),(0.0, Vectors.sparse(1, [], []))], ["label", "features"])
stringIndexer = StringIndexer(inputCol="label", outputCol="indexed")
si_model = stringIndexer.fit(df)
td = si_model.transform(df)
dt = DecisionTreeClassifier(maxDepth=2, labelCol="indexed")
model = dt.fit(td)
model.numNodes
#3
 model.depth
#1
model.featureImportances
#SparseVector(1, {0: 1.0})
 model.numFeatures
#1
model.numClasses
#2
print(model.toDebugString)
#DecisionTreeClassificationModel (uid=...) of depth 1 with 3 nodes...
test0 = spark.createDataFrame([(Vectors.dense(-1.0),)], ["features"])
result = model.transform(test0).head()
result.prediction
#0.0
result.probability
#DenseVector([1.0, 0.0])
result.rawPrediction
#DenseVector([1.0, 0.0])
test1 = spark.createDataFrame([(Vectors.sparse(1, [0], [1.0]),)], ["features"])
model.transform(test1).head().prediction
#1.0
dtc_path = temp_path + "/dtc"
dt.save(dtc_path)
dt2 = DecisionTreeClassifier.load(dtc_path)
dt2.getMaxDepth()
#2
model_path = temp_path + "/dtc_model"
model.save(model_path)
model2 = DecisionTreeClassificationModel.load(model_path)
model.featureImportances == model2.featureImportances
#True
```

# RandomForestClassifier
`class pyspark.ml.classification.RandomForestClassifier(self, featuresCol="features", labelCol="label", predictionCol="prediction", probabilityCol="probability", rawPredictionCol="rawPrediction", maxDepth=5, maxBins=32, minInstancesPerNode=1, minInfoGain=0.0, maxMemoryInMB=256, cacheNodeIds=False, checkpointInterval=10, impurity="gini", numTrees=20, featureSubsetStrategy="auto", seed=None, subsamplingRate=1.0)`
## 参数解释

```
checkpoint：同DecisionTreeClassifier
featureSubsetStrategy：每棵树上要分割的特征数目，选项为"auto","all", "onethird", "sqrt", "log2", "(0.0-1.0],"[1-n]"
fit(dataset,params=None)方法
impurity: 同DecisionTreeClassifier
maxBins:同DecisionTreeClassifier
maxDepth：同DecisionTreeClassifier
minInfoGain: 同DecisionTreeClassifier
numTrees: 训练树的个数
subsamplingRate: 用于训练每颗决策树的样本个数，区间(0,1]
参数的getter和setter方法
```

## 拟合后的模型拥有的方法和属性

```
featureImportances: 同DecisionTreeClassifier，详见Hastie的《统计学习基础》
getNumTrees: 树的个数
transform(dataset, params=None)方法
treeWeights:返回各个树的权重
```

# 代码

```
import numpy
from numpy import allclose
from pyspark.ml.linalg import Vectors
from pyspark.ml.feature import StringIndexer
df = spark.createDataFrame([
     (1.0, Vectors.dense(1.0)),
    (0.0, Vectors.sparse(1, [], []))], ["label", "features"])
stringIndexer = StringIndexer(inputCol="label", outputCol="indexed")
si_model = stringIndexer.fit(df)
td = si_model.transform(df)
rf = RandomForestClassifier(numTrees=3, maxDepth=2, labelCol="indexed", seed=42)
model = rf.fit(td)
model.featureImportances

#SparseVector(1, {0: 1.0})
allclose(model.treeWeights, [1.0, 1.0, 1.0])
#True
test0 = spark.createDataFrame([(Vectors.dense(-1.0),)], ["features"])
result = model.transform(test0).head()
result.prediction
#0.0
numpy.argmax(result.probability)
#0
numpy.argmax(result.rawPrediction)
#0
test1 = spark.createDataFrame([(Vectors.sparse(1, [0], [1.0]),)], ["features"])
model.transform(test1).head().prediction
#1.0
model.trees
#[DecisionTreeClassificationModel (uid=...) of depth..., DecisionTreeClassificationModel...]
rfc_path = temp_path + "/rfc"
rf.save(rfc_path)
rf2 = RandomForestClassifier.load(rfc_path)
 rf2.getNumTrees()
#3
model_path = temp_path + "/rfc_model"
model.save(model_path)
model2 = RandomForestClassificationModel.load(model_path)
model.featureImportances == model2.featureImportances
#True
```

# GBTClassifier
`class pyspark.ml.classification.GBTClassifier(self, featuresCol="features", labelCol="label", predictionCol="prediction", maxDepth=5, maxBins=32, minInstancesPerNode=1, minInfoGain=0.0, maxMemoryInMB=256, cacheNodeIds=False, checkpointInterval=10, lossType="logistic", maxIter=20, stepSize=0.1, seed=None, subsamplingRate=1.0)`
此分类器实现依据为： 

J.H. Friedman. “Stochastic Gradient Boosting.” 1999.

## 参数解释

```
checkpointInterval: 同DecisionTreeClassifier
fit(dataset,params=None)方法
lossType: GBT要最小化的损失函数，选项：logistic
maxBins: 同DecisionTreeClassifier
maxDepth: 同DecisionTreeClassifier
maxIter: 同DecisionTreeClassifier
minInfoGain: 同DecisionTreeClassifier
minInstancesPerNode：同DecisionTreeClassifier
stepSize: 每次迭代优化的步长
subsamplingRate: 同RandomForesetClassier
参数的getter和setter方法
```

## 拟合后的模型拥有的方法和属性

```
featureImportances: 同DecisionTreeClassifier
getNumTrees：同RandomForesetClassier
totalNumNodes: 结点的总个数，所有树的加总和
transform(dataset,params=None)方法
treeWeights: 同RandomForesetClassier
```

## 代码

```
from numpy import allclose
from pyspark.ml.linalg import Vectors
from pyspark.ml.feature import StringIndexer
df = spark.createDataFrame([(1.0, Vectors.dense(1.0)),(0.0,Vectors.sparse(1, [], []))], ["label", "features"])
stringIndexer = StringIndexer(inputCol="label", outputCol="indexed")
si_model = stringIndexer.fit(df)
td = si_model.transform(df)
gbt = GBTClassifier(maxIter=5, maxDepth=2, labelCol="indexed", seed=42)
model = gbt.fit(td)
model.featureImportances
#SparseVector(1, {0: 1.0})
allclose(model.treeWeights, [1.0, 0.1, 0.1, 0.1, 0.1])
#True
test0 = spark.createDataFrame([(Vectors.dense(-1.0),)], ["features"])
model.transform(test0).head().prediction
#0.0
test1 = spark.createDataFrame([(Vectors.sparse(1, [0], [1.0]),)], ["features"])
model.transform(test1).head().prediction
#1.0
model.totalNumNodes
#15
print(model.toDebugString)
#GBTClassificationModel (uid=...)...with 5 trees...
gbtc_path = temp_path + "gbtc"
gbt.save(gbtc_path)
gbt2 = GBTClassifier.load(gbtc_path)
gbt2.getMaxDepth()
#2
model_path = temp_path + "gbtc_model"
model.save(model_path)
model2 = GBTClassificationModel.load(model_path)
model.featureImportances == model2.featureImportances
#True
model.treeWeights == model2.treeWeights
#True
model.trees
#[DecisionTreeRegressionModel (uid=...) of depth..., DecisionTreeRegressionModel...]
```

# NaiveBayes
`class pyspark.ml.classification.NaiveBayes(self, featuresCol="features", labelCol="label", predictionCol="prediction", probabilityCol="probability", rawPredictionCol="rawPrediction", smoothing=1.0, modelType="multinomial", thresholds=None, weightCol=None)`
## 参数解释

```
fit(dataset, params=None)方法
modelType: 选项：multinomial和bernulli
smoothing: 平滑参数，应该>=0，默认为1.0
参数的setter和getter方法
```

## 拟合后的模型拥有的方法和属性

```
numClasses: 类的个数
numFeatures: 训练模型的特征个数，如果不知道，返回-1
pi: 类先验的对数
theta: 类条件概率的对数
transform(dataseet,params=None)方法
```

## 代码

```
from pyspark.sql import Row
from pyspark.ml.linalg import Vectors
df = spark.createDataFrame([Row(label=0.0, weight=0.1, features=Vectors.dense([0.0, 0.0])),Row(label=0.0, weight=0.5, features=Vectors.dense([0.0, 1.0])),Row(label=1.0, weight=1.0, features=Vectors.dense([1.0, 0.0]))])

nb = NaiveBayes(smoothing=1.0, modelType="multinomial", weightCol="weight")
model = nb.fit(df)
model.pi
#DenseVector([-0.81..., -0.58...])
model.theta
#DenseMatrix(2, 2, [-0.91..., -0.51..., -0.40..., -1.09...], 1)
test0 = sc.parallelize([Row(features=Vectors.dense([1.0, 0.0]))]).toDF()
result = model.transform(test0).head()
result.prediction
#1.0
result.probability
#DenseVector([0.32..., 0.67...])
result.rawPrediction
#DenseVector([-1.72..., -0.99...])
test1 = sc.parallelize([Row(features=Vectors.sparse(2, [0], [1.0]))]).toDF()
model.transform(test1).head().prediction
#1.0
nb_path = temp_path + "/nb"
nb.save(nb_path)
nb2 = NaiveBayes.load(nb_path)
nb2.getSmoothing()
#1.0
model_path = temp_path + "/nb_model"
model.save(model_path)
model2 = NaiveBayesModel.load(model_path)
model.pi == model2.pi
#True
model.theta == model2.theta
#True
nb = nb.setThresholds([0.01, 10.00])
model3 = nb.fit(df)
result = model3.transform(test0).head()
result.prediction
#0.0
```




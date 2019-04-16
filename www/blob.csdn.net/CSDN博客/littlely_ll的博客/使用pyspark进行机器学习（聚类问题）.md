# 使用pyspark进行机器学习（聚类问题） - littlely_ll的博客 - CSDN博客





2017年10月03日 16:25:36[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1143








[使用pyspark进行机器学习（分类问题）](http://blog.csdn.net/littlely_ll/article/details/78151964)
[使用pyspark进行机器学习（回归问题）](http://blog.csdn.net/littlely_ll/article/details/78161574)

# BisectingKMeans
`class pyspark.ml.clustering.BisectingKMeans(self, featuresCol="features", predictionCol="prediction", maxIter=20, seed=None, k=4, minDivisibleClusterSize=1.0)`
## 参数解释

```
maxIter: 最大迭代次数
K：聚类簇数
minDivisibleClusterSize: 聚类的最少数据点数(>1)或比例(0-1之间)
fit(dataset, params=None)方法
```

## 拟合后的模型拥有的方法和属性

```
clusterCenters(): 获取聚类中心，numpy array类型
computeCost()：计算点与其中心的平方和距离
Transform()：对预测数据进行预测
hasSummary:训练模型是否有summary
Summary：获取summary
拥有对参数的getter和setter方法
```

## Summary拥有的属性

```
cluster：预测的聚类中心
clusterSizes：每个聚类的大小
K：聚类个数
Predictions：由模型的transforn方法产生的预测数据框
```

## 代码

```
from pyspark.ml.linalg import Vectors
From pyspark.ml.clustering import BisectingKMeans

data = [(Vectors.dense([0.0, 0.0]),), (Vectors.dense([1.0, 1.0]),),(Vectors.dense([9.0, 8.0]),), (Vectors.dense([8.0, 9.0]),)]
df = spark.createDataFrame(data, ["features"])
bkm = BisectingKMeans(k=2, minDivisibleClusterSize=1.0)
model = bkm.fit(df)
centers = model.clusterCenters()
len(centers)
model.computeCost(df)
model.hasSummary
summary = model.summary
summary.k
summary.clusterSizes
#预测
transformed = model.transform(df).select("features", "prediction")
rows = transformed.collect()
rows[0].prediction == rows[1].prediction
rows[2].prediction == rows[3].prediction
```

# KMeans
`class pyspark.ml.clustering.KMeans(self, featuresCol="features", predictionCol="prediction", k=2, initMode="k-means||", initSteps=2, tol=1e-4, maxIter=20, seed=None)`
## 参数解释

```
initMode: 初始化算法，可以使随机的“random"，也可以是”k-means||"
initSteps: k-means||初始化的步数，需>0
fit(datast,params=None)方法
```

## 拟合后的模型拥有的方法和属性

```
clusterCenters(): 同BisectingKMeans
computeCost(): 同BisectingKMeans
summary: 同BisectingKMeans
transform: 同BisectingKMeans
对参数的getter和setter方法
```

## 代码

```
from pyspark.ml.linalg import Vectors
data = [(Vectors.dense([0.0, 0.0]),), (Vectors.dense([1.0, 1.0]),),(Vectors.dense([9.0, 8.0]),), (Vectors.dense([8.0, 9.0]),)]
df = spark.createDataFrame(data, ["features"])
kmeans = KMeans(k=2, seed=1)
model = kmeans.fit(df)
centers = model.clusterCenters()
len(centers)
#2
model.computeCost(df)
#2.000...
transformed = model.transform(df).select("features", "prediction")
rows = transformed.collect()
rows[0].prediction == rows[1].prediction
#True
rows[2].prediction == rows[3].prediction
#True
model.hasSummary
#True
summary = model.summary
summary.k
#2
summary.clusterSizes
#[2, 2]
kmeans_path = temp_path + "/kmeans"
kmeans.save(kmeans_path)
kmeans2 = KMeans.load(kmeans_path)
kmeans2.getK()
#2
model_path = temp_path + "/kmeans_model"
model.save(model_path)
model2 = KMeansModel.load(model_path)
model2.hasSummary
#False
model.clusterCenters()[0] == model2.clusterCenters()[0]
#array([ True,  True], dtype=bool)
model.clusterCenters()[1] == model2.clusterCenters()[1]
#array([ True,  True], dtype=bool)
```

# GaussianMixture
`class pyspark.ml.clustering.GaussianMixture(self, featuresCol="features", predictionCol="prediction", k=2, probabilityCol="probability", tol=0.01, maxIter=100, seed=None)`
## 参数解释

```
fit(dataset,params=None)方法
k: 独立高斯分布的个数，>1
maxIter: 最大迭代次数 >=0
tol: 迭代算法的收敛偏差 >=0
Setter方法和getter方法
```

## 拟合后的模型拥有的方法和属性

```
gaussianDF: 抽取高斯分布作为数据框，每一行代表高斯分布，有两列：mean（vector)和           cov(Matrix)
hasSummary: 模型是否有总括函数
summary： 获取总括信息
transform(dataset,params=None)方法
weights: 高斯混合模型的权重，和为1
```

## Summary拥有的属性

```
cluster: 每个训练数据点预测的聚类中心数据框
clusterSize: 每个簇的大小（簇内数据点的个数）
k:  模型训练的簇个数
predictions: 由模型transform方法产生的数据框
```

## 代码

```
from pyspark.ml.linalg import Vectors
data = [(Vectors.dense([-0.1, -0.05 ]),),(Vectors.dense([-0.01, -0.1]),),(Vectors.dense([0.9, 0.8]),),(Vectors.dense([0.75,0.935]),),(Vectors.dense([-0.83, -0.68]),),(Vectors.dense([-0.91, -0.76]),)]
df = spark.createDataFrame(data, ["features"])
gm = GaussianMixture(k=3, tol=0.0001,maxIter=10, seed=10)
model = gm.fit(df)
model.hasSummary
#True
summary = model.summary
summary.k
#3
summary.clusterSizes
#[2, 2, 2]
weights = model.weights
len(weights)
#3
model.gaussiansDF.show()
transformed=model.transform(df).select("features","prediction")
rows = transformed.collect()
rows[4].prediction == rows[5].prediction
#True
rows[2].prediction == rows[3].prediction
#True
gmm_path = temp_path + "/gmm"
gm.save(gmm_path)
gm2 = GaussianMixture.load(gmm_path)
gm2.getK()
#3
model_path = temp_path + "/gmm_model"
model.save(model_path)
model2 = GaussianMixtureModel.load(model_path)
model2.hasSummary
#False
model2.weights == model.weights
#True
model2.gaussiansDF.show()
```




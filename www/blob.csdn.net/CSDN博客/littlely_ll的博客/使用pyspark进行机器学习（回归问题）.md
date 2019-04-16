# 使用pyspark进行机器学习（回归问题） - littlely_ll的博客 - CSDN博客





2017年10月05日 15:47:13[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1640








[使用pyspark进行机器学习（分类问题）](http://blog.csdn.net/littlely_ll/article/details/78151964)
[使用pyspark进行机器学习（聚类问题）](http://blog.csdn.net/littlely_ll/article/details/78155192)

# DecisionTreeRegressor
`class pyspark.ml.regression.DecisionTreeRegressor(self, featuresCol="features", labelCol="label", predictionCol="prediction", maxDepth=5, maxBins=32, minInstancesPerNode=1, minInfoGain=0.0, maxMemoryInMB=256, cacheNodeIds=False, checkpointInterval=10, impurity="variance", seed=None, varianceCol=None)`
支持连续与分类变量

## 参数解释

fit(dataset, params=None)方法 

Impurity: 信息增益计算准则，支持选项：variance 

maxBins: 连续特征离散化的最大分箱个数， >=2并且>=任何分类特征的分类个数 

maxDepth: 最大树深 

minInfoGain: 分割节点所需最小信息增益 

minInstancesPerNode: 分割后每个子节点最小实例个数 

Setter方法和getter方法
## 拟合后模型拥有的方法或属性

depth: 返回决策树的深度 

featureImportances: 每个特征重要性估计（见DecisionTreeClassifer) 

numFeatures: 返回训练模型中的特征个数，如果不知，返回-1 

numNodes: 返回决策树中的结点个数 

transform(dataset, params=None)方法
## 代码

```
from pyspark.ml.linalg import Vectors
df = spark.createDataFrame([(1.0, Vectors.dense(1.0)),(0.0, Vectors.sparse(1, [], []))], ["label", "features"])
dt = DecisionTreeRegressor(maxDepth=2, varianceCol="variance")
model = dt.fit(df)
model.depth
#1
model.numNodes
#3
model.featureImportances
#SparseVector(1, {0: 1.0})
model.numFeatures
#1
test0 = spark.createDataFrame([(Vectors.dense(-1.0),)], ["features"])
model.transform(test0).head().prediction
#0.0
test1 = spark.createDataFrame([(Vectors.sparse(1, [0], [1.0]),)], ["features"])
model.transform(test1).head().prediction
#1.0
dtr_path = temp_path + "/dtr"
dt.save(dtr_path)
dt2 = DecisionTreeRegressor.load(dtr_path)
dt2.getMaxDepth()
#2
model_path = temp_path + "/dtr_model"
model.save(model_path)
model2 = DecisionTreeRegressionModel.load(model_path)
model.numNodes == model2.numNodes
#True
model.depth == model2.depth
#True
model.transform(test1).head().variance
#0.0
```

# GBTRegressor
`class pyspark.ml.regression.GBTRegressor(self, featuresCol="features", labelCol="label", predictionCol="prediction", maxDepth=5, maxBins=32, minInstancesPerNode=1, minInfoGain=0.0, maxMemoryInMB=256, cacheNodeIds=False, subsamplingRate=1.0, checkpointInterval=10, lossType="squared", maxIter=20, stepSize=0.1, seed=None, impurity="variance")`
## 参数解释

fit(dataset,params=None)方法 

Impurity: 同DecisionTreeRegressor 

lossType: GBT要最小化的损失函数，可选：squared, absolute 

maxBins: 同DecisionTreeRegressor 

maxDepth: 同DecisionTreeRegressor 

maxIter: 最大迭代次数 

minIfoGain: 同DecisionTreeRegressor 

minInstancesPerNode: 同DecisionTreeRegressor 

stepSize: 每次优化迭代的步长 

subsamplingRate:用于训练每颗决策树的训练数据集的比例，区间[0,1] 

Setter和getter方法
## 拟合后模型拥有的方法或属性

featureImportances: 同DecisionTreeRegressor 

getNumTrees: 同DecisionTreeRegressor 

numFeatures: 同DecisionTreeRegressor 

totalNumNoes: 集成树中所有的结点 

transform(dataset,params=None)方法 

treeWeights: 每棵树的权重 

trees: 返回树
## 代码

```
from numpy import allclose
from pyspark.ml.linalg import Vectors
df = spark.createDataFrame([(1.0, Vectors.dense(1.0)), (0.0, Vectors.sparse(1, [], []))], ["label", "features"])
gbt = GBTRegressor(maxIter=5, maxDepth=2, seed=42)
print(gbt.getImpurity())
#variance
model = gbt.fit(df)
model.featureImportances
#SparseVector(1, {0: 1.0})
model.numFeatures
#1
allclose(model.treeWeights, [1.0, 0.1, 0.1, 0.1, 0.1])
#True
test0 = spark.createDataFrame([(Vectors.dense(-1.0),)], ["features"])
model.transform(test0).head().prediction
#0.0
test1 = spark.createDataFrame([(Vectors.sparse(1, [0], [1.0]),)],["features"])
model.transform(test1).head().prediction
#1.0
gbtr_path = temp_path + "gbtr"
gbt.save(gbtr_path)
gbt2 = GBTRegressor.load(gbtr_path)
gbt2.getMaxDepth()
#2
model_path = temp_path + "gbtr_model"
model.save(model_path)
model2 = GBTRegressionModel.load(model_path)
model.featureImportances == model2.featureImportances
#True
model.treeWeights == model2.treeWeights
#True
model.trees
#[DecisionTreeRegressionModel (uid=...) of depth..., DecisionTreeRegressionModel...]
```

# GeneralizedLinearRegression
`class pyspark.ml.regression.GeneralizedLinearRegression(self, labelCol="label", featuresCol="features", predictionCol="prediction", family="gaussian", link=None, fitIntercept=True, maxIter=25, tol=1e-6, regParam=0.0, weightCol=None, solver="irls", linkPredictionCol=None)`
给出link函数和family函数拟合广义线性模型 

Family函数对应的link函数,第一个分别是默认值 

    • “gaussian” -> “identity”, “log”, “inverse” 

    • “binomial” -> “logit”, “probit”, “cloglog” 

    • “poisson” -> “log”, “identity”, “sqrt” 

    • “gamma” -> “inverse”, “identity”, “log”
## 参数解释

fit(dataset,params=None)方法 

family: 误差分布的描述名称，可选：gaussian（默认), binomial, poisson, gamma 

fitIntercept: 是否拟合截距项 

link:  提供线性预测变量和分布函数均值的关系名称，可选：identity, log, inverse, logit, probit, cloglog, sqrt. 

maxIter: 最大迭代次数 

solver: 优化的算法，如果没设置或空则使用”auto” 

tol:  迭代的收敛偏差 

Setter和getter方法
## 拟合后模型拥有的方法或属性(experimental)

coefficients: 模型系数 

Evaluate(dataset): 在检测集上评估模型 

hasSummary: 是否有summary 

Intercept: 模型截距 

numFeatures: 训练模型的特征个数 

Summary：获取summary 

transform(dataset,params=None)方法
## Summary属性

aic: 模型的aic准则 

degreesOfFreedom: 自由度 

deviance: 拟合模型的偏差 

dispersion: 离差，对于binomial和poisson family为1，其他的由残差的Pearson Chi方统计量估计 

predictions： 由模型tranform方法产生的预测输出 

rank: 拟合线性模型的数值排秩 

residualDegreeOfFreedom: 残差的自由度 

residual(residualType=’deviance’): 拟合模型的残差,residualType为返回残差类型，可选：pearson, working, response
## trainingSummary属性

aic: aic准则 

coefficientStandardErrors: 估计系数和截距的标准误 

degreesOfFreedom 

deviance 

dispersion 

pValues: 估计系数和解决的双边p值 

predictions 

rank 

residualDegreeOfFreedom 

Residuals(residualType=’deviance’) 

solver 

tValues: 估计系数和截距的T统计量
## 代码

```
from pyspark.ml.linalg import Vectors
df = spark.createDataFrame([ (1.0, Vectors.dense(0.0, 0.0)), (1.0, Vectors.dense(1.0, 2.0)),(2.0, Vectors.dense(0.0, 0.0)),(2.0, Vectors.dense(1.0, 1.0)),], ["label", "features"])
glr = GeneralizedLinearRegression(family="gaussian", link="identity", #linkPredictionCol="p")
model = glr.fit(df)
transformed = model.transform(df)
abs(transformed.head().prediction - 1.5) < 0.001
#True
abs(transformed.head().p - 1.5) < 0.001
#True
model.coefficients
DenseVector([1.5..., -1.0...])
model.numFeatures
#2
abs(model.intercept - 1.5) < 0.001
#True
glr_path = temp_path + "/glr"
glr.save(glr_path)
glr2 = GeneralizedLinearRegression.load(glr_path)
glr.getFamily() == glr2.getFamily()
#True
model_path = temp_path + "/glr_model"
model.save(model_path)
model2 = GeneralizedLinearRegressionModel.load(model_path)
model.intercept == model2.intercept
#True
model.coefficients[0] == model2.coefficients[0]
#True
```

# LinearRegression
`class pyspark.ml.regression.LinearRegression(self, featuresCol="features", labelCol="label", predictionCol="prediction", maxIter=100, regParam=0.0, elasticNetParam=0.0, tol=1e-6, fitIntercept=True, standardization=True, solver="auto", weightCol=None, aggregationDepth=2)`
支持多种类型的正则化： 

    • None：OLS 

    • L2：ridge回归 

    • L1：Lasso回归 

    • L1+L2：elastic回归
## 参数解释

aggregationDepth: 树聚合的深度, >=2 

elasticNtParam: ElasticNet混合参数，在[0,1]范围内，alpha=0为L2， alpha=1为L1 

fit(dataset,params=None)方法 

fitIntercept: 是否拟合截距 

maxIter: 最大迭代次数 

regParam：正则化参数 >=0 

solver: 优化算法，没设置或空则使用”auto” 

standardization: 是否对拟合模型的特征进行标准化 

Setter和getter方法
## 拟合后模型拥有的方法或属性

coefficients 

evaluate(dataset) 

hasSummary 

intercept 

numFeatures 

Summary 

transform(dataset, params=None)
## Summary属性(experimental)

coefficientStandardErrors 

devianceResiduals: 加权残差 

explainedVariance: 返回解释的方差回归得分，$explainedVariance=1-variance(y-\hat(y))/variance(y)$

meanAbsoluteError: 返回均值绝对误差 

meanSquaredError: 返回均值平方误 

numInstances: 预测的实例个数 

pValues: 系数和截距的双边P值，只有用”normal”solver才可用 

predictions: 模型transform方法返回的预测 

r2: R方 

residuals: 残差 

rootMeanSquaredError: 均方误差平方根 

tValues： T统计量
## traningSummary属性

coefficientStandardErrors 

devianceResiduals 

explainedVariance 

meanAbsoluteError 

meanSquaredError 

numInstances 

objectiveHistory: 每次迭代的目标函数，只有使用”l-bfgs”solver才可用 

pValues 

Predictions 

r2 

residuals 

rootMeanSquaredError 

tValues 

totalIterations: 结束前总迭代次数
## 代码

```
from pyspark.ml.linalg import Vectors
df = spark.createDataFrame([ (1.0, 2.0, Vectors.dense(1.0)),(0.0, 2.0, Vectors.sparse(1, [], []))], ["label", "weight", "features"])
lr = LinearRegression(maxIter=5, regParam=0.0, solver="normal", weightCol="weight")
model = lr.fit(df)
test0 = spark.createDataFrame([(Vectors.dense(-1.0),)], ["features"])
abs(model.transform(test0).head().prediction - (-1.0)) < 0.001
#True
abs(model.coefficients[0] - 1.0) < 0.001
#True
abs(model.intercept - 0.0) < 0.001
#True
test1 = spark.createDataFrame([(Vectors.sparse(1, [0], [1.0]),)], ["features"])
abs(model.transform(test1).head().prediction - 1.0) < 0.001
#True
lr.setParams("vector")
#Traceback (most recent call last):
#    ...
#TypeError: Method setParams forces keyword arguments.
lr_path = temp_path + "/lr"
lr.save(lr_path)
lr2 = LinearRegression.load(lr_path)
lr2.getMaxIter()
#5
model_path = temp_path + "/lr_model"
model.save(model_path)
model2 = LinearRegressionModel.load(model_path)
model.coefficients[0] == model2.coefficients[0]
#True
model.intercept == model2.intercept
#True
model.numFeatures
#1
```

# RandomForestRegressor
`class pyspark.ml.regression.RandomForestRegressor(self, featuresCol="features", labelCol="label", predictionCol="prediction", maxDepth=5, maxBins=32, minInstancesPerNode=1, minInfoGain=0.0, maxMemoryInMB=256, cacheNodeIds=False, checkpointInterval=10, impurity="variance", subsamplingRate=1.0, seed=None, numTrees=20, featureSubsetStrategy="auto")`
## 参数解释

fit(dataset,params=None)方法 

featureSubsetStrategy: 每棵树的节点上要分割的特征数量，可选：auto, all, onethird, sqrt, log2,(0.0,1.0],[1-n] 

impurity: 信息增益计算的准则，可选：variance 

maxBins: 连续特征离散化最大分箱个数。 

maxDepth: 树的最大深度 

minInfoGain: 树节点分割特征所需最小的信息增益 

minInstancesPerNode: 每个结点所需最小实例个数 

numTrees: 训练树的个数 

subsamplingRate: 学习每颗决策树所需样本比例 

Setter和getter方法
## 拟合后模型拥有的方法或属性

featureImportances: 见DecisionTreeRegressor 

numFeatures: 返回模型特征个数，如果不知，返回-1 

totalNumNodes: 结点总数 

transform(dataset,params=None)方法 

treeWeights: 每棵树的权重 

trees: 树
## 代码

```
from numpy import allclose
from pyspark.ml.linalg import Vectors
df = spark.createDataFrame([(1.0, Vectors.dense(1.0)), (0.0, Vectors.sparse(1, [], []))], ["label", "features"])
rf = RandomForestRegressor(numTrees=2, maxDepth=2, seed=42)
model = rf.fit(df)
model.featureImportances
#SparseVector(1, {0: 1.0})
allclose(model.treeWeights, [1.0, 1.0])
#True
test0 = spark.createDataFrame([(Vectors.dense(-1.0),)], ["features"])
model.transform(test0).head().prediction
#0.0
model.numFeatures
#1
model.trees
#[DecisionTreeRegressionModel (uid=...) of depth..., DecisionTreeRegressionModel...]
model.getNumTrees
#2
test1 = spark.createDataFrame([(Vectors.sparse(1, [0], [1.0]),)], ["features"])
model.transform(test1).head().prediction
#0.5
rfr_path = temp_path + "/rfr"
rf.save(rfr_path)
rf2 = RandomForestRegressor.load(rfr_path)
rf2.getNumTrees()
#2
model_path = temp_path + "/rfr_model"
model.save(model_path)
model2 = RandomForestRegressionModel.load(model_path)
model.featureImportances == model2.featureImportances
#True
```




















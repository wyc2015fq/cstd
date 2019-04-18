# 【Spark Mllib】性能评估 ——MSE/RMSE与MAPK/MAP - Soul Joy Hub - CSDN博客

2016年06月30日 11:14:04[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：3634
所属专栏：[Spark机器学习](https://blog.csdn.net/column/details/15014.html)



# 推荐模型评估

本篇我们对《[Spark机器学习1.0:推荐引擎——电影推荐](http://blog.csdn.net/u011239443/article/details/51752904)》模型进行性能评估。

## MSE/RMSE

均方差（MSE），就是对各个实际存在评分的项，pow（预测评分-实际评分，2）的值进行累加，在除以项数。而均方根差（RMSE）就是MSE开根号。

我们先用ratings生成（user，product）RDD，作为model.predict()的参数，从而生成以（user，product）为key，value为预测的rating的RDD。然后，用ratings生成以（user，product）为key，实际rating为value的RDD，并join上前者：

```
val usersProducts = ratings.map{ case Rating(user, product, rating)  => (user, product)}
val predictions = model.predict(usersProducts).map{
    case Rating(user, product, rating) => ((user, product), rating)
}
val ratingsAndPredictions = ratings.map{
    case Rating(user, product, rating) => ((user, product), rating)
}.join(predictions)
ratingsAndPredictions.first()
//res21: ((Int, Int), (Double, Double)) = ((291,800),(2.0,2.052364223387371))
```

使用MLLib的评估函数，我们要传入一个（actual，predicted）的RDD。actual和predicted左右位置可以交换：

```
import org.apache.spark.mllib.evaluation.RegressionMetrics
val predictedAndTrue = ratingsAndPredictions.map { case ((user, product), (actual, predicted)) => (actual, predicted) }
val regressionMetrics = new RegressionMetrics(predictedAndTrue)
println("Mean Squared Error = " + regressionMetrics.meanSquaredError)
println("Root Mean Squared Error = " + regressionMetrics.rootMeanSquaredError)
// Mean Squared Error = 0.08231947642632852
// Root Mean Squared Error = 0.2869137090247319
```

## MAPK/MAP

K值平均准确率（MAPK）可以简单的这么理解： 

设定推荐K=10，即推荐10个物品。预测该用户评分最高的10个物品ID作为文本1，实际上用户评分过所有物品ID作为文本2，求二者的相关度。（个人认为该评估方法在这里不是很适用） 

我们可以按评分排序预测物品ID，再从头遍历，如果该预测ID出现在实际评分过ID的集合中，那么就增加一定分数（当然，排名高的应该比排名低的增加更多的分数，因为前者更能体现推荐的准确性）。最后将累加得到的分数除以min（K，actual.size） 

如果是针对所有用户，我们需要把各个用户的累加分数进行累加，在除以用户数。 

在MLlib里面，使用的是全局平均准确率（MAP，不设定K）。**它需要我们传入（predicted.Array，actual.Array）的RDD**。 

现在，我们先来生成predicted： 

我们先生成产品矩阵：

```
/* Compute recommendations for all users */
val itemFactors = model.productFeatures.map { case (id, factor) => factor }.collect()
val itemMatrix = new DoubleMatrix(itemFactors)
println(itemMatrix.rows, itemMatrix.columns)
// (1682,50)
```

以便工作节点能够访问到，我们把该矩阵以广播变量的形式分发出去：

```
// broadcast the item factor matrix
val imBroadcast = sc.broadcast(itemMatrix)
```

矩阵相乘，计算出评分。scores.data.zipWithIndex，scores.data再按评分排序。生成recommendedIds，构建(userId, recommendedIds)RDD：

```
val allRecs = model.userFeatures.map{ case (userId, array) => 
  val userVector = new DoubleMatrix(array)
  val scores = imBroadcast.value.mmul(userVector)
  val sortedWithId = scores.data.zipWithIndex.sortBy(-_._1)
  val recommendedIds = sortedWithId.map(_._2 + 1).toSeq
  (userId, recommendedIds)
}
```

提取实际值：

```
// next get all the movie ids per user, grouped by user id
val userMovies = ratings.map{ case Rating(user, product, rating) => (user, product) }.groupBy(_._1)
// userMovies: org.apache.spark.rdd.RDD[(Int, Seq[(Int, Int)])] = MapPartitionsRDD[277] at groupBy at <console>:21
```

生成（predicted.Array，actual.Array）的RDD，并使用评估函数：

```
import org.apache.spark.mllib.evaluation.RankingMetrics
val predictedAndTrueForRanking = allRecs.join(userMovies).map{ case (userId, (predicted, actualWithIds)) => 
  val actual = actualWithIds.map(_._2)
  (predicted.toArray, actual.toArray)
}
val rankingMetrics = new RankingMetrics(predictedAndTrueForRanking)
println("Mean Average Precision = " + rankingMetrics.meanAveragePrecision)
// Mean Average Precision = 0.07171412913757183
```


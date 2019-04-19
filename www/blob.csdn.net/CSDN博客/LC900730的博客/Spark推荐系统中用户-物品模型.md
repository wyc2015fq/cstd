# Spark推荐系统中用户-物品模型 - LC900730的博客 - CSDN博客
2018年01月02日 17:59:49[lc900730](https://me.csdn.net/LC900730)阅读数：680
# 矩阵分解
## 1.显式矩阵分解
当要处理的那些数据是由用户所提供的自身的偏好数据，这些数据被称为显式偏好数据。 
这类数据包括如物品的评级、赞、喜欢等用户对物品的评价。 
这些数据可以转换为以用户为行、物品为列的二维矩阵。矩阵的每一个数据表示某个用户对特定物品的偏好。大部分情况下单个用户只会和少部分物品接触，所以该矩阵只有少部分数据非0(稀疏矩阵)
如： 
Tom, Star Wars, 5 
Jane, Titanic, 4 
Bill, Batman, 3 
Jane, Star Wars, 2 
Bill, Titanic, 3 
![这里写图片描述](https://img-blog.csdn.net/20171217215942086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
假设用户和物品数目分别是U和I，那对应的用户-物品矩阵的维度为U*I。 
要找到”用户-物品”矩阵近似的k维(低阶)矩阵，最终是求出如下2个矩阵： 
表示用户的U*k 
表示物品的k*I维矩阵。这两个矩阵也称为因子矩阵。 
![这里写图片描述](https://img-blog.csdn.net/20171217220320710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因子分解类型也存在某些弱点。相比最邻近模型，这类模型在理解和可解释性难度上都有所增加。另外，其模型训练阶段的计算量也很大。
## 2.隐式矩阵分解
用户对物品的偏好不会直接给出，而是隐含在用户与物品的交互之中。二元数据(比如用户是否观看了某个电影或是否购买了某个商品)和计数数据(如用户观看某电影的次数)便是这类数据。 
MLib库实现了一个特定方法，它将输入的评级数据视为两个矩阵：一个二元偏好矩阵P和一个信心权重矩阵C。 
![这里写图片描述](https://img-blog.csdn.net/20171217221102420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 3.最小二乘法
最小二乘法(Alternating Least Squares，ALS)是一种求解矩阵分解问题的最优化方法。 
ALS的实现原理是迭代式求解一系列最小二乘回归问题。
# 提取有效特征
rank：对应ALS模型中的因子个数，也就是在低阶近似矩阵中的隐含特征个数。 
iterations：对应运行时的迭代次数。 
lambda：该参数控制模型的正则化过程，从而控制模型的过拟合情况。
### 简易推荐系统
```
package com.spark.FirstApp
import org.apache.spark.{SparkConf, SparkContext}
import org.apache.spark.mllib.recommendation.{ALS,Rating}
object tuijian{
  def main(args: Array[String]): Unit = {
    val conf = new SparkConf().setAppName("WordCount111").setMaster("spark://centos701:7077")
    val sc = new SparkContext(conf)
    val rawData=sc.textFile("ml-100k/u.data")
    rawData.first()
    val rawRatings=rawData.map(_.split("\t").take(3))
    rawRatings.first()
    val ratings=rawRatings.map{case Array(user,movie,rating)=>Rating(user.toInt,movie.toInt,rating.toDouble)}
    val model=ALS.train(ratings,50,10,0.01)
    model.userFeatures.count
    val predictdRating=model.predict(789,123)
    val userId=789
    val K=10
    val topKRecs=model.recommendProducts(userId,K)
    println(topKRecs.mkString("\n"))
  }
}
```
## 物品推荐
物品推荐是回答如下问题：给定一个物品，有哪些物品与它最相似。 
常见的相似度衡量方法包括皮尔森相关系数(Pearson correlation)、针对实数向量的余弦相似度(cosine similarity)和针对二元向量的杰卡德相似系数(Jaccard similarity)。
jblas线性代数库(MLlib的依赖库之一)

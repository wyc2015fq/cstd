# 案例：Spark基于用户的协同过滤算法 - Spark高级玩法 - CSDN博客
2017年12月03日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：2027

![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWEvgK4tsj0ByGnVPnFhabWnLoibV5saZdXnibMxPSHzBWV5Z4k9nqmdTHkKHbcRVOEpo0WURLHXmLA/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**一 基于用户协同过滤简介**
基于用户的协同过滤算法(user-based collaboratIve filtering)
基于用户的协同过滤算法是通过用户的历史行为数据发现用户对商品或内容的喜欢(如商品购买，收藏，内容评论或分享)，并对这些喜好进行度量和打分。根据不同用户对相同商品或内容的态度和偏好程度计算用户之间的关系。在有相同喜好的用户间进行商品推荐。简单的说就是如果A,B两个用户都购买了x,y,z三本图书，并且给出了5星的好评。那么A和B就属于同一类用户。可以将A看过的图书w也推荐给用户B。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWEvgK4tsj0ByGnVPnFhabWcqKJTVOwVZTfGYWKB7YxZhQHSAVv94hlBor5ATVvicbTmd9EsAmYTxA/0?wx_fmt=png)
**Spark MLlib的ALS**
spark.ml目前支持基于模型的协作过滤，其中用户和产品由可用于预测缺失条目的一小组潜在因素来描述。spark.ml使用交替最小二乘（ALS） 算法来学习这些潜在因素。算法实现中spark.ml提供有以下参数：
- 
**numBlocks**是为了并行化计算而将用户和项目分割成的块的数量（默认为10）。
- 
**rank**是模型中潜在因子的数量（默认为10）。
- 
**maxIter**是要运行的最大迭代次数（默认为10）。
- 
**regParam**指定ALS中的正则化参数（默认为1.0）。
- 
**implicitPrefs**显示的反馈ALS(true，显示的表示偏好程度)或者隐式的反馈ALS(false隐式指定偏好)。默认是false，显示反馈ALS
- 
**alpha **
偏好观察中置信度(可理解为一个系数)，用于隐式反馈ALS。默认值是1.
- 
**nonnegative**指定是否对最小二乘使用非负约束（默认为false）。
**注意：**ALS基于DataFrame的API目前仅支持用户和项目ID为整数。用户和项目ID列支持其他数字类型，但ID必须在整数值范围内。
**显式与隐式反馈**
基于矩阵分解的协作过滤的标准方法将用户条目矩阵中的条目视为用户对该项目的显式偏好，例如，用户给电影的评级。
在许多真实世界的使用情况中，通常只能访问隐式反馈（例如，观看，点击，购买，喜欢，分享等）。根本上讲，这种方法不是根据用户直接评分建模，而是根据用户的行为(点击次数，停留时间)，将其视为数字，代表用户对电影的可能喜欢程度。然后，这些数字与观察到的用户偏好的置信度相关，而不是与物品的显式评分。然后该模型将尝试找出可以用来预测用户对于某一项目的预期偏好的潜在因子。
**正则化参数**
调整的正则化参数regParam，是根据用户在更新用户因子时产生的评分数或者物品在更新物品因子时收到的评分数来解决每个最小二乘问题。这种方法被命名为“ALS-WR”，并在“Large-Scale Parallel Collaborative Filtering for the Netflix Prize ”文章中进行了讨论。它对regParam数据集规模的依赖较小，因此我们可以将从采样子集学习到的最佳参数应用于整个数据集，并期望有相似的性能。
**冷启动策略**
使用ALSModel进行预测时，测试数据集中的用户和/或项目在训练模型期间不存在是很常见的。这通常发生在两种情况下：
- 
在生产中，对于没有评级历史记录且未进行模型训练的新用户或物品（这是“冷启动问题”）。
- 
在交叉验证过程中，数据分为训练集和评估集。当Spark中的使用简单随机拆分为CrossValidator或者TrainValidationSplit，它实际上是非常普遍遇到的评估集不是在训练集中的用户和/或项目。
默认情况，Spark在ALSModel.transform用户和/或项目因素不存在于模型中时分配NaN预测。这在生产系统中可能是有用的，因为它表名一个新的用户或项目，因此系统可以作为预测的一个后备决定。
然而，这在交叉验证期间是不希望的，因为任何NaN预测值都将影响NaN评估度量的结果（例如，在使用时RegressionEvaluator）。这使得模型选择变得不可能。Spark允许用户将coldStartStrategy参数设置为“drop”，以便删除DataFrame包含NaN值的预测中的任何行。
注意：目前支持的冷启动策略是“nan”（上面提到的默认行为）和“drop”。未来可能会支持进一步的策略。
**MovieLens电影基于用户推荐**
在以下示例中，我们将从MovieLens数据集(https://grouplens.org/datasets/movielens/)中加载评分数据 ，每行由用户，电影，评分和时间戳组成。然后，我们训练一个ALS模型，默认情况下，这个模型的评分是明确的（implicitPrefs是false）。我们通过测量评级预测的均方根误差来评估推荐模型。
import
org.apache.spark.ml.evaluation.RegressionEvaluator
import
org.apache.spark.ml.recommendation.ALS
import
org.apache.spark.sql.SparkSession
object
ALSExample {
case class
Rating(userId:
Int, movieId: Int,
rating: Float, timestamp:
Long)
def
parseRating(str:
String): Rating = {
val
fields = str.split("::")
assert(fields.size ==
4)
Rating(fields(0).toInt,
fields(1).toInt,
fields(2).toFloat,
fields(3).toLong)
 }
def
main(args: Array[String])
 {
val
spark = SparkSession
     .builder
.appName("ALSExample")
     .getOrCreate()
import
spark.implicits._
// $example on$
val
ratings = spark.read.textFile("file:///opt/modules/spark-2.2.0/data/mllib/als/sample_movielens_ratings.txt").map(parseRating).toDF()
//将数据集切分为训练集和测试集
val
Array(training,
test) = ratings.randomSplit(Array(0.8,
0.2))
//使用ALS在训练集数据上构建推荐模型
val
als = new
ALS().setMaxIter(5).setRegParam(0.01).setUserCol("userId").setItemCol("movieId").setRatingCol("rating")
val
model = als.fit(training)
// 通过计算rmse(均方根误差)来评估模型
   //为确保不获取到NaN评估参数，我们将冷启动策略设置为drop。
val
predictions = model.transform(test)
val
evaluator =
new
RegressionEvaluator().setMetricName("rmse").setLabelCol("rating").setPredictionCol("prediction")
val
rmse = evaluator.evaluate(predictions)
println(s"Root-mean-square
 error = $rmse")
//每个用户推荐的前十个电影
val
userRecs = model.recommendForAllUsers(10)
//每个电影推荐的十个用户
val
movieRecs = model.recommendForAllItems(10)
   userRecs.show()
   movieRecs.show()
   spark.stop()
 }
}
如果评级矩阵是从另一个信息源（即它是从其他因子推断）得出，可以设置implicitPrefs以true获得更好的效果：
valals = new
ALS()
 .setMaxIter(5)
 .setRegParam(0.01)
 .setImplicitPrefs(true)
 .setUserCol("userId")
 .setItemCol("movieId")
 .setRatingCol("rating")
**推荐阅读：**
1，[请别再问我Spark的MLlib和ML库的区别](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484273&idx=1&sn=830eee29f7078dd85d6b3156a2880acd&chksm=9f38e059a84f694f99480c6e831d788187d1943704ca69f881721bf40dbf9745c8701902c4db&scene=21#wechat_redirect)
2，[Spark的Ml pipeline](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484270&idx=1&sn=69153063accd105649d4466d938012da&chksm=9f38e046a84f69505d954726c65d940f9a370342755ae2ce7456134ef4b9a6eafb1350a4dfb6&scene=21#wechat_redirect)
3，[phoenix二级索引](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484260&idx=1&sn=07a7670a2a95c3e03f988d11fdc3037e&chksm=9f38e04ca84f695ad162a8d17bbd88dcf062ca867e35177ef143c67a23c0617c8cb49eff5243&scene=21#wechat_redirect)
4，[kafka源码系列之mysql数据增量同步到kafka](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483762&idx=1&sn=59c4cb2a22868b107d8c8ca97e1e82bf&chksm=9f38e25aa84f6b4c6eaeda0e9444562a134482ecfb8495c3f47972a7f4e5f718d0127c533aea&scene=21#wechat_redirect)
**关注公众号，输入：als，即可获取本文的数据集代码。**
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**


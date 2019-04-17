# 离线轻量级大数据平台Spark之MLib机器学习协同过滤ALS实例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年10月31日 17:02:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2501
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、协同过滤

协同过滤（Collaborative Filtering，简称CF，WIKI上的定义是：简单来说是利用某个兴趣相投、拥有共同经验之群体的喜好来推荐感兴趣的资讯给使用者，个人透过合作的机制给予资讯相当程度的回应（如评分）并记录下来以达到过滤的目的，进而帮助别人筛选资讯，回应不一定局限于特别感兴趣的，特别不感兴趣资讯的纪录也相当重要。

协同过滤常被应用于推荐系统。这些技术旨在补充用户—商品关联矩阵中所缺失的部分。

MLlib 当前支持基于模型的协同过滤，其中用户和商品通过一小组隐性因子进行表达，并且这些因子也用于预测缺失的元素。MLLib 使用交替最小二乘法（ALS） 来学习这些隐性因子。

用户对物品或者信息的偏好，根据应用本身的不同，可能包括用户对物品的评分、用户查看物品的记录、用户的购买记录等。其实这些用户的偏好信息可以分为两类：

l  显式的用户反馈：这类是用户在网站上自然浏览或者使用网站以外，显式地提供反馈信息，例如用户对物品的评分或者对物品的评论。

l  隐式的用户反馈：这类是用户在使用网站是产生的数据，隐式地反映了用户对物品的喜好，例如用户购买了某物品，用户查看了某物品的信息，等等。

显式的用户反馈能准确地反映用户对物品的真实喜好，但需要用户付出额外的代价；而隐式的用户行为，通过一些分析和处理，也能反映用户的喜好，只是数据不是很精确，有些行为的分析存在较大的噪音。但只要选择正确的行为特征，隐式的用户反馈也能得到很好的效果，只是行为特征的选择可能在不同的应用中有很大的不同，例如在电子商务的网站上，购买行为其实就是一个能很好表现用户喜好的隐式反馈。

推荐引擎根据不同的推荐机制可能用到数据源中的一部分，然后根据这些数据，分析出一定的规则或者直接对用户对其他物品的喜好进行预测计算。这样推荐引擎可以在用户进入时给他推荐他可能感兴趣的物品。





2、Java开发ALS实例

实例中使用电影评分数据，字段信息包括用户编号：电影编号：评分：评分时间戳。

见spark-2.0.1-bin-hadoop2.7/data/mllib/als/ sample_movielens_ratings.txt

Java开发上，引入spark-mllib_2.11-2.0.1.jar、spark-mllib-local_2.11-2.0.1.jar、spark-catalyst_2.11-2.0.1.jar。

参考spark的example案例的代码，输出skals.jar包并提交执行。

bin/spark-submit --class sk.mlib.ALSDemo --master local /tmp/skals.jar


代码如下：



```java
package sk.mlib;

import org.apache.spark.sql.Dataset;
import org.apache.spark.sql.Row;
import org.apache.spark.sql.SparkSession;

import java.io.Serializable;

import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.ml.evaluation.RegressionEvaluator;
import org.apache.spark.ml.recommendation.ALS;
import org.apache.spark.ml.recommendation.ALSModel;


public class ALSDemo {
	
	public static class Rating implements Serializable {
	    private int userId;
	    private int movieId;
	    private float rating;
	    private long timestamp;

	    public Rating() {}

	    public Rating(int userId, int movieId, float rating, long timestamp) {
	      this.userId = userId;
	      this.movieId = movieId;
	      this.rating = rating;
	      this.timestamp = timestamp;
	    }

	    public int getUserId() {
	      return userId;
	    }

	    public int getMovieId() {
	      return movieId;
	    }

	    public float getRating() {
	      return rating;
	    }

	    public long getTimestamp() {
	      return timestamp;
	    }

	    public static Rating parseRating(String str) {
	      String[] fields = str.split("::");
	      if (fields.length != 4) {
	        throw new IllegalArgumentException("Each line must contain 4 fields");
	      }
	      int userId = Integer.parseInt(fields[0]);
	      int movieId = Integer.parseInt(fields[1]);
	      float rating = Float.parseFloat(fields[2]);
	      long timestamp = Long.parseLong(fields[3]);
	      return new Rating(userId, movieId, rating, timestamp);
	    }
	  }

	  public static void main(String[] args) {
	    SparkSession spark = SparkSession.builder().appName("ALSDemo").getOrCreate();

	    JavaRDD<Rating> ratingsRDD = spark
	      .read().textFile("/tmp/sample_movielens_ratings.txt").javaRDD()
	      .map(new Function<String, Rating>() {
	        public Rating call(String str) {
	          return Rating.parseRating(str);
	        }
	      });
	    Dataset<Row> ratings = spark.createDataFrame(ratingsRDD, Rating.class);
	    Dataset<Row>[] splits = ratings.randomSplit(new double[]{0.8, 0.2});//8成做训练样本，2成做测试样本
	    Dataset<Row> training = splits[0];
	    Dataset<Row> test = splits[1];

	    // Build the recommendation model using ALS on the training data
	    ALS als = new ALS().setMaxIter(5).setRegParam(0.01).setUserCol("userId").setItemCol("movieId").setRatingCol("rating");
	    ALSModel model = als.fit(training);
	    // Evaluate the model by computing the RMSE on the test data
	    Dataset<Row> predictions = model.transform(test);

	    RegressionEvaluator evaluator = new RegressionEvaluator()
	      .setMetricName("rmse")
	      .setLabelCol("rating")
	      .setPredictionCol("prediction");
	    Double rmse = evaluator.evaluate(predictions);
	    System.out.println("Root-mean-square error = " + rmse);

	    spark.stop();
	  }
}
/*
 * 执行结果：
 * Root-mean-square error = 1.7215462865151776
 */
```


对Spark平台MLib库通过三个实例初步了解和掌握，主要还是对算法本身有掌握，才能明确实际场景需要用到的算法。






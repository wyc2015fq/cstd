# 基于Mahout、Spark Mlib实现的推荐系统——算法讲解 - wsp_1138886114的博客 - CSDN博客





2018年09月22日 18:00:08[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：206








### 一、推荐算法

**推荐算法组成**
![这里写图片描述](https://img-blog.csdn.net/20180821142226406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**推荐算法应用**
- 社交推荐
- 电影推荐

##### 1.1 线性代数里的矩阵分解：
- ALS（Alternating Least Square交替最小二乘法）
- 三角分解
- SVD（奇异值）分解
- QR分解
- Jordan分解
- 满秩分解

Mahout和sparkMLlib里面，就是特指使用 ALS 的一种推荐算法。

##### 1.2 构造一个完整的推荐系统所需要的步骤
- 加载数据文件 new File（csv,mysql等 ）
- 根据加载的文件构造一个数据模型----DataModel
- 根据数据模型DataModel计算用户相似度UserSimilarity 或物品相似度ItemSimilarity
- 根据DataModel,相似度构造一个推荐器recommender
- 为用户推荐物品，保存数据

##### 1.3 Mahout

Mahout下载：[http://archive.apache.org/dist/mahout/0.12.2/](http://archive.apache.org/dist/mahout/0.12.2/)

Mahout API比较重要的接口、实现类、包

数据下载地址：[https://grouplens.org/datasets/movielens/](https://grouplens.org/datasets/movielens/)

下载文件：ml-100k.zip
```
//数据模型
org.apache.mahout.cf.taste.model.DataModel
org.apache.mahout.cf.taste.impl.model.file.FileDataModel
org.apache.mahout.cf.taste.impl.model.GenericDataModel
org.apache.mahout.cf.taste.model.JDBCDataModel
org.apache.mahout.cf.taste.impl.model.jdbc.MySQLJDBCDataModel

//用户、物品 相似度
org.apache.mahout.cf.taste.similarity.UserSimilarity
org.apache.mahout.cf.taste.similarity.ItemSimilarity
org.apache.mahout.cf.taste.impl.similarity.LogLikelihoodSimilarity
org.apache.mahout.cf.taste.impl.similarity.UncenteredCosineSimilarity
org.apache.mahout.cf.taste.impl.similarity.EuclideanDistanceSimilarity
org.apache.mahout.cf.taste.impl.similarity.PearsonCorrelationSimilarity

//KNN
org.apache.mahout.cf.taste.neighborhood.UserNeighborhood
org.apache.mahout.cf.taste.impl.neighborhood.NearestNUserNeighborhood

//推荐器
org.apache.mahout.cf.taste.recommender.Recommender
org.apache.mahout.cf.taste.impl.recommender.GenericUserBasedRecommender

org.apache.mahout.cf.taste.impl.recommender.GenericItemBasedRecommender
org.apache.mahout.cf.taste.recommender.ItemBasedRecommender
org.apache.mahout.cf.taste.recommender.UserBasedRecommender
org.apache.mahout.cf.taste.recommender.RecommendedItem

//推荐评估

org.apache.mahout.cf.taste.eval.RecommenderEvaluator
org.apache.mahout.cf.taste.impl.eval.AverageAbsoluteDifferenceRecommenderEvaluator
org.apache.mahout.cf.taste.impl.eval.RMSRecommenderEvaluator
org.apache.mahout.cf.taste.eval.RecommenderBuilder

org.apache.mahout.cf.taste.eval.IRStatistics
org.apache.mahout.cf.taste.eval.RecommenderIRStatsEvaluator
org.apache.mahout.cf.taste.impl.eval.GenericRecommenderIRStatsEvaluator

异常类
org.apache.mahout.cf.taste.common.TasteException

其它相关类
org.apache.mahout.cf.taste.impl.model.GenericUserPreferenceArray
org.apache.mahout.cf.taste.model.PreferenceArray
org.apache.mahout.cf.taste.impl.common.FastByIDMap
org.apache.mahout.cf.taste.similarity.precompute.BatchItemSimilarities
org.apache.mahout.cf.taste.impl.similarity.precompute.MultithreadedBatchItemSimilarities
org.apache.mahout.cf.taste.similarity.precompute.SimilarItemsWriter
org.apache.mahout.cf.taste.impl.similarity.precompute.FileSimilarItemsWriter
```

##### 1.4 推荐评估

```
UncenteredCosineSimilarity
EuclideanDistanceSimilarity
PearsonCorrelationSimilarity
LogLikelihoodSimilarity

评估模型
    AverageAbsoluteDifferenceRecommenderEvaluator
    RMSRecommenderEvaluator
```






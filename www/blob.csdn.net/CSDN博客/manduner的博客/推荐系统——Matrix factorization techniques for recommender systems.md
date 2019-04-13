
# 推荐系统——Matrix factorization techniques for recommender systems - manduner的博客 - CSDN博客


2018年07月18日 08:49:45[Manduner_TJU](https://me.csdn.net/manduner)阅读数：941


# 论文描述：
*基本矩阵分解模型只利用到了用户-商品交互的显示反馈信息（评分等），虽然模型简单、灵活易扩展，但是推荐性能较低，较为流行的基本矩阵分解模型是SVD。
*基于SVD进行的扩展的经典推荐算法是SVD++，SVD++通过添加各种辅助信息扩展了SVD模型，提高了推荐系统的性能。这些辅助信息包含：用户和商品的偏置、用户偏好表示。
由于每个用户对商品的评价标准不同，所以直接用用户-商品交互的显示评分矩阵表示用户、商品各自的潜在特征会对整个推荐性能产生影响。又由于许多用户没有对商品做出过显示的评分，导致推荐系统遭遇数据稀疏和冷启动的问题，使得SVD的推荐性能并没有那么良好。然而，不管用户是否对商品做出过评价或打过分，一些隐式反馈信息（是否浏览过商品、购买记录、网页停留时间）也可以反映用户对商品的偏好，并且隐式反馈数据比较容易收集。

# 参考文献：
**Koren Y, Bell R, Volinsky C. Matrix factorization techniques for recommender systems[J]. Computer, 2009 (8): 30-37.*


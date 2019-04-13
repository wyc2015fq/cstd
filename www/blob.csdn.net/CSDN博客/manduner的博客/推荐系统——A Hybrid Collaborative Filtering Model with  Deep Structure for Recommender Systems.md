
# 推荐系统——A Hybrid Collaborative Filtering Model with  Deep Structure for Recommender Systems - manduner的博客 - CSDN博客


2018年07月17日 16:27:02[Manduner_TJU](https://me.csdn.net/manduner)阅读数：221


# 论文描述：
协同过滤方法是推荐系统中应用较为广泛的方法。传统的基于CF的方法使用用户-商品矩阵编码用户的个人偏好用于学习以进行推荐。在实际中，评分矩阵通常非常稀疏，导致基于CF的方法在推荐性能上显著降低。在这种情况下，一些改进的CF方法利用越来越多的辅助信息来解决数据稀疏性问题以及冷启动问题。但是，由于用户-商品矩阵和辅助信息的稀疏性质，学习到的潜在特征可能无效。为了解决这个问题，我们首先提出了一种称为附加堆叠降噪自编码器（aSDEA）的新型深度学习模型，该模型通过将附加的辅助信息整合到降噪堆叠自编码器模型中，克服了推荐系统遇到的冷启动和数据稀疏问题。然后，我们将aSDEA与矩阵分解（MF）模型相融合，构建了一个混合协同过滤模型，他能够同时从辅助信息中提取有效的潜在特征，并得到用户和商品之间的隐含关系。我们在三个真实数据集上进行实验，以评估我们的混合模型的有效性。实验结果表明，我们的混合模型在均方根误差（RMSE）和召回率指标方面优于四种最先进的方法。

# 参考文献：
*Dong X, Yu L, Wu Z, et al. A Hybrid Collaborative Filtering Model with Deep Structure for Recommender Systems[C]//AAAI. 2017: 1309-1315.*



# 推荐系统——Review Sentiment–Guided Scalable Deep Recommender System - manduner的博客 - CSDN博客


2018年07月17日 16:22:45[Manduner_TJU](https://me.csdn.net/manduner)阅读数：141


# 论文描述：
推荐系统可以根据已有评分和其他信息来预测给定用户对给定商品的评分。最成功的方法是协同过滤，但是会面临数据稀疏和冷启动的问题。最近，深度学习模型被提出用来缓解使用评论的评分矩阵的稀疏性，然而由于文本中存在歧义，已有的一些方法仅仅根据评论中生词的串联是具有挑战性的。因此，情感信息对于建模用户和商品来解决评论中的不确定性至关重要。由于对评论的过度表示，它们在训练时间和内存使用方面也不具有可扩展性。
本文提出了一种可伸缩的评论推荐方法，称为SentiRec，在建模用户和商品时结合评论的情感信息。SentiRec方法有两步组成：1.纳入评论情感，包括将每个评论编码为体现评论情感的固定长度评论向量；2.根据向量编码的评论生成推荐。

# 参考文献：
*Hyun D, Park C, Yang M C, et al. Review Sentiment-Guided Scalable Deep Recommender System[C]//The 41st International ACM SIGIR Conference on Research & Development in Information Retrieval. ACM, 2018: 965-968.*


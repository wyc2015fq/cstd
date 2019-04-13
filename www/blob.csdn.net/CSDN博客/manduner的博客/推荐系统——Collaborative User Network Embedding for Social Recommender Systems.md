
# 推荐系统——Collaborative User Network Embedding for Social Recommender Systems - manduner的博客 - CSDN博客


2018年07月17日 16:37:55[Manduner_TJU](https://me.csdn.net/manduner)阅读数：583


# 论文描述：
*为了解决推荐系统中数据稀疏和冷启动的问题，许多推荐算法引入了社交信息（例如：用户-用户信任链接）来补充评分数据以改进传统的基于模型的推荐技术性能，例如矩阵分解（MF）和贝叶斯个性化排名（Bayesian personalized ranking,BPR）。这样做，尽管取得了一定的效果，但直接从这些社交信息中提取的显示用户—用户关系具有三个主要限制，首先，由于只有一小部分用户在推荐系统中明确表示他们信任的朋友，很难获得明确可靠的社交联系。其次，“冷启动”问题不仅在评分上“冷”而且在社交关系上也是“冷”的，即，没有大量明确的社会信息可用于“冷启动”用户。第三，由于活跃的用户可以与其他品味/偏好不同的人建立社会交关系，所以直接使用明确的社交链接关系可能会误导推荐。
*为了解决上述问题，我们从用户对商品的反馈信息中提取隐含和可靠的社交信息，并为每个用户标识top-k语义好友，基于此我们构建了一个新的模型CUME（Collaborative User Network Embedding），该方法将反馈信息作为user-item bipartite network(U-I-Net),然后通过单模式将UI-Net压缩成collaborative user network (C-U-Net) 。接下来，受网络嵌入研究的启发，我们通过偏差随机游走在C-U-Net中收集一组节点序列（命名为“semantic social corpus”），并利用SkipGram模型来处理由语义社会语料库组成的语言。最后，根据SkipGram返回的嵌入向量表示计算每两个用户的相似度。与特定用户具有最大相似度的K个用户被定义为top-k语义好友。
*将Top-k语义好友信息融入MF和BPR框架，以分别解决评分预测和商品排名问题。

# 参考文献：
*Zhang C, Yu L, Wang Y, et al. Collaborative User Network Embedding for Social Recommender Systems[C]//Proceedings of the 2017 SIAM International Conference on Data Mining. Society for Industrial and Applied Mathematics, 2017: 381-389.*


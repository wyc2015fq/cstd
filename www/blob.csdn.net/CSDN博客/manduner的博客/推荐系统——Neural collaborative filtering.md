
# 推荐系统——Neural collaborative filtering - manduner的博客 - CSDN博客


2018年07月18日 08:49:57[Manduner_TJU](https://me.csdn.net/manduner)阅读数：642


# 论文描述：
*矩阵分解模型是目前最为流行的一种协同过滤方法，通过用户潜在特征和商品潜在特征的内积重构用户-商品交互矩阵中的未知项，模型简单、易扩展且灵活。但是取得的推荐性能较低。基于矩阵分解的改进方法有，将矩阵分解模型与基于内容的模型相结合，形成混合模型；将辅助信息整合到矩阵分解模型中等。由于深度学习强大的表示学习能力，一些研究开始将深度学习的方法与传统的矩阵分解模型相结合，以提高推荐系统的性能。但是大部分利用深度学习的推荐模型都是将辅助信息整合到了矩阵分解模型中，对于用户-商品的潜在特征表示仍然使用的是矩阵分解的方法。
*我们通过神经网络模型建模协同过滤，模型的训练数据使用的是隐式反馈数据，即，通过观看视频、购买产品和点击商品等行为简介反映用户的偏好。与显式反馈数据，如，评分和评论相比，隐式反馈可以自动跟踪，因此更容易收集。我们提出了一个通用的神经网络框架，Neural network-based Collaborative Filtering（NCF）。文中具体的深度神经过滤模型为，Generalized Matrix Factorization（GMF），Multi-Layer Perceptron（MLP），Neural Matrix Factorization（NeuMF）

# 参考文献：
*He X, Liao L, Zhang H, et al. Neural collaborative filtering[C]//Proceedings of the 26th International Conference on World Wide Web. International World Wide Web Conferences Steering Committee, 2017: 173-182.*


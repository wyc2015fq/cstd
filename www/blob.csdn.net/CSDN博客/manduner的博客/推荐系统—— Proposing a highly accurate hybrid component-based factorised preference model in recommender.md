
# 推荐系统—— Proposing a highly accurate hybrid component-based factorised preference model in recommender - manduner的博客 - CSDN博客


2018年07月18日 08:50:57[Manduner_TJU](https://me.csdn.net/manduner)阅读数：59标签：[推荐系统																](https://so.csdn.net/so/search/s.do?q=推荐系统&t=blog)个人分类：[论文																](https://blog.csdn.net/manduner/article/category/7694119)



# 论文描述：
*在信息爆炸的时代，推荐系统扮演着越来越重要的作用。协同过滤推荐模型是最受欢迎的推荐算法之一，基于矩阵分解的协同过滤推荐算法进行扩展的方法有很多，很多研究者会将一些辅助信息整合到协同过滤算法中以解决推荐系统遇到的数据稀疏和冷启动问题。这些辅助信息包含，user and item biases, feature value preferences, coditional dependencies等，但是截止目前对辅助信息的研究文献都是这些辅助信息其中一个或者单独考虑其中一个或多个。
*我们通过分析研究两个现有的开源数据集，发现这些辅助信息互相作用着，他们之间的相互作用对推荐系统的性能有着很大的影响。基于此，我们提出了一个将5类辅助信息整合到一起，并研究他们之间相互作用对推荐性能影响的模型：TCSFVSVD.
*我们整合的5类辅助信息如下：
（1）Feature value preferences，指每个商品特征值的相对有利性
（2）Social influence，指社交关系对用户偏好的影响
（3）temporal dynamics，指偏好随时间改变而改变
（4）conditional preferences，指商品特征和其值之间的依赖性
（5）user and item biases，指the systematic tendencies for some users to give higher ratings than others, and for some items to receive higher ratings than others

# 参考文献：
*Zafari F, Moser I, Rahmani R. Proposing a highly accurate hybrid component-based factorised preference model in recommender systems[C]//Proceedings of the 26th International Joint Conference on Artificial Intelligence (IJCAI). 2017.*


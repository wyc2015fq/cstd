
# 推荐系统——Improved Bounded Matrix Completion for Large-Scale  Recommender Systems - manduner的博客 - CSDN博客


2018年07月17日 16:35:54[Manduner_TJU](https://me.csdn.net/manduner)阅读数：105


# 论文描述：
*矩阵分解和矩阵填补被广泛用于个性化推荐系统中，通过已知的用户—商品评分构建用户-商品矩阵，并通过重构初始评分矩阵以预测缺失的评分。传统的矩阵分解模型并没有对预测值施加任何约束，但是在真实数据中，评分都是在某一特定区域范围内的。例如，MovieLens和Netflix数据集的评分在[0.5,5]的范围内，但未加约束条件的传统矩阵分解模型的预测值通常会超出此范围的值。面对这种情况，对传统的矩阵分解模型优化问题施加有界约束条件是合理的。有界矩阵填补（Bounded Matrix Completion，BMC）是较早出现的增加约束条件的矩阵分解模型，但是BMC问题的不足之处是目标函数是非凸形式，在此基础上改进的方法有BMA，但BMA算法并不总是收敛到平稳点，相反，它很容易陷入非平稳点。这种不稳点的收敛行为通常会导致推荐系统性能下降。
*在本文中，我们提出了一种用于最小化BMC目标函数的新算法，该算法基于用于求解凸形式的ADMM框架，因此它可以保证收敛到最优解。而且通过仔细设计的更细规则，我们的算法不会遇到O(mn)空间复杂度，并且可以扩展到1000万个评分的大型数据集。

# 参考文献：
*Fang H, Zhang Z, Shao Y, et al. Improved bounded matrix completion for large-scale recommender systems[C]//Proceedings of the 26th International Joint Conference on Artificial Intelligence. AAAI Press, 2017: 1654-1660.*


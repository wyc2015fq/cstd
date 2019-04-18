# NN模型设置--损失函数的设置 - wydbyxr的博客 - CSDN博客
2018年12月04日 10:26:20[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：146
# 人脸的loss
  1）Softmax 是最常见的人脸识别损失函数，然而，Softmax 不会显式的优化类间和类内距离的，所以通常不会有太好的性能。
  2）Triplet Loss 作为一种 Metric Learning，也在人脸识别中广泛使用。
  相比 Softmax，其特点是可以方便训练大规模 ID(百万，千万)的数据集，不受显存的限制。但是相应的，因为它的关注点过于局部，使得性能无法达到最佳且训练需要的周期非常长。
  3）目前，用cos-loss普遍比tripelet-loss要好。有各种cos-loss的变种。
  由于 Tripelet Loss 训练困难，人们逐渐寻求结合 Metric Learning 思想来改进 Softmax 的思路，其中 SphereFace是其中的佼佼者，作为 17 年提出的一种方法，在当时达到了 state-of-the-art。
  在具体实现中，为了避免加入的 margin(即 m) 过大，引入了新的超参 λ，和 Softmax 一起联合训练。
# 回归问题可以用哪些loss?
  1）0-1损失
  是在分类问题中你实际上需要的. 不幸的是, 这是非凸优化问题. 由于最优化问题会变得或多或少的不好解, 因此并不实用。
  2）hinge loss (合页损失)
  常用于“maximum-margin”的算法。
  3）cross entropy loss
  导出很好的概率校准。因此如果你不仅得到二值预测, 还可以得出结果的概率, 这个损失函数是一个很好的选择(如比如逻辑回归)。
  缺点是它的解在数据空间中是不稀疏的, 它比合页损失函数更容易受到离群点的影响。
  4）square loss or Mean squared error
  预测值和真实值之间的平方误差总和。
  5）指数损失函数
  非常容易受离群点的影响. 它主要适用于Adaboost中, 因为它在一个简单有效的boosting算法中有效果。
# mask的误差函数
  因为我们的目标是预测一个mask，所以误差函数是对二分类的cross entropy loss。

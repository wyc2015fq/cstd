# 深度学习基础--loss与激活函数--人脸的loss - wydbyxr的博客 - CSDN博客
2018年11月16日 09:48:11[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：65
# 人脸的loss
  1）Softmax 是最常见的人脸识别损失函数，然而，Softmax 不会显式的优化类间和类内距离的，所以通常不会有太好的性能。
  2）Triplet Loss 作为一种 Metric Learning，也在人脸识别中广泛使用。
  相比 Softmax，其特点是可以方便训练大规模 ID(百万，千万)的数据集，不受显存的限制。但是相应的，因为它的关注点过于局部，使得性能无法达到最佳且训练需要的周期非常长。
   3）cosin loss
  由于 Tripelet Loss 训练困难，人们逐渐寻求结合 Metric Learning 思想来改进 Softmax 的思路，其中 SphereFace是其中的佼佼者，作为 17 年提出的一种方法，在当时达到了 state-of-the-art。
  在具体实现中，为了避免加入的 margin(即 m) 过大，引入了新的超参 λ，和 Softmax 一起联合训练。
  目前，用cos-loss普遍比tripelet-loss要好，已经发展出了各种cos-loss的变种。

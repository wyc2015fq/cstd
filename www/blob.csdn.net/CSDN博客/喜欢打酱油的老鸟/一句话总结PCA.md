
# 一句话总结PCA - 喜欢打酱油的老鸟 - CSDN博客


2018年09月19日 12:49:34[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：111


一句话总结PCA
核心：向重构误差最小（方差最大）的方向做线性投影。
PCA是一种数据降维和去除相关性的方法，它通过线性变换将向量投影到低维空间。对向量进行投影就是让向量左乘一个矩阵得到结果向量，这是线性代数中讲述的线性变换：
y = Wx
降维要确保的是在低维空间中的投影能很好的近似表达原始向量，即重构误差最小化。下图是主分量投影示意图：
![](https://img-blog.csdn.net/20180919124841406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在上图中样本用红色的点表示，倾斜的直线是它们的主要变化方向。将数据投影到这条直线上即完成数据的降维，把数据从2维降为1维。计算最佳投影方向时求解的最优化问题为：
![](https://img-blog.csdn.net/20180919124901466?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后归结为求协方差矩阵的特征值和特征向量：
![](https://img-blog.csdn.net/2018091912491969?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjEzNzcwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
PCA是一种无监督的学习算法，它是线性模型，不能直接用于分类和回归问题。



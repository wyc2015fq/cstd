# 【IM】关于半监督学习的理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年10月29日 11:04:57[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：183








基于流形假设的半监督学习：假定输入概率密度p(x)和条件概率密度p(y|x)之间具有某种关联，估计p(x)辅助对p(y|x)的估计以提升精度。流形假设，即输入数据只出现在某个流形上，输出则在该流形上平滑变化。

拉普拉斯正则化是把输入数据在流形上进行函数平滑的半监督学习算法，结合拉普拉斯矩阵(L=D-W)和图论来解释，通过从有标签的接点向没有标签的接点传播标签，相互连接的接点(样本群)可以实现标签的共享。

拉普拉斯正则化学习，可理解为沿着输入样本的流形计算路径距离（图论中为最短路径），基于样本间的最短路径来求解其分类面，利用这样的方法，可以把各个聚类中的所有样本，都归类到相同的类别中。

![](https://img-blog.csdnimg.cn/2018102911043549.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_27,color_FFFFFF,t_70)

理解拉普拉斯矩阵及其图论解释是重要。






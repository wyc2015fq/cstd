# 高斯混合模型GMM - 浅梦的博客 - CSDN博客





2017年08月28日 16:03:37[浅梦s](https://me.csdn.net/u012151283)阅读数：4278








# 使用概率模型的原因

k均值等价于假设了球对称形状的聚类。使用带权欧式距离，仍然假设了轴对齐的椭球。没有考虑聚类的形状。 

促使概率模型的原因：混合模型
- 提供观测点到聚类的**软分配soft assignment**（分配包含不确定性）
- 考虑了聚类的**形状**而不仅仅是中心
- 允许从不同维度来**学习权重**

# 高斯分布

![这里写图片描述](https://img-blog.csdn.net/20170828154510752?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

双变量高斯分步，协方差矩阵的主对角线决定了展度；副对角线决定朝向 
![这里写图片描述](https://img-blog.csdn.net/20170828154630067?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 高斯混合模型

![这里写图片描述](https://img-blog.csdn.net/20170828154844592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

GMM估计的EM算法 
![这里写图片描述](https://img-blog.csdn.net/20170828155204150?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# GMM推导

![这里写图片描述](https://img-blog.csdn.net/20171205224307100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171205224355599?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171205224446929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171205224607117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# EM算法的收敛性，初始化和过拟合

## 收敛性
- EM是一种坐标下降算法，等价于交替最大化E步和M步目标函数
- 收敛于局部最优解

## 初始化
- 许多初始化EM算法的方式
- 对于收敛和局部最优解的质量重要
- 距离 
- 随机选择K个中心

- 选择类似k-means++
- 根据k-means的解初始化
- 通过划分（移除）簇直到形成k个簇生成混合模型

## 过拟合

不要让方差为0. 

协方差矩阵对角线添加小的常数。 

使用贝叶斯方法为参数添加先验。
# k-means与EM关系
- k-means是两个步骤交替进行，可以分别看成E步和M步；
- E步中将每个点分给中心距它最近的类（硬分配），可以看成是EM算法中E步（软分配）的近似。当方差无限小的时候，EM相当于k-means。
- M步中将每类的中心更新为分给该类各点的均值，可以认为是在「各类分布均为单位方差的高斯分布」的假设下，最大化似然值；

![这里写图片描述](https://img-blog.csdn.net/20170828160253634?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

参考资料

> 
[知乎王赟 Maigo的回答](https://www.zhihu.com/question/49972233/answer/119434460)

  《Machine Learning》ColumbiaX: CSMM.102x Lecture 16









# 【机器学习详解】KNN分类的概念、误差率及其问题 - 勿在浮砂筑高台 - CSDN博客





2016年03月18日 17:23:46[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：9724标签：[机器学习																[KNN](https://so.csdn.net/so/search/s.do?q=KNN&t=blog)](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)
个人分类：[Machine Learning](https://blog.csdn.net/luoshixian099/article/category/5618837)

所属专栏：[机器学习详解](https://blog.csdn.net/column/details/ml-theory.html)








转载请注明出处：[http://blog.csdn.net/luoshixian099/article/details/50923056](http://blog.csdn.net/luoshixian099/article/details/50923056)

**勿在浮沙筑高台**

# KNN概念  

      KNN(K-Nearest Neighbors algorithm)是一种非参数模型算法。在训练数据量为N的样本点中，寻找最近邻测试数据x的K个样本，然后统计这K个样本的分别输入各个类别w_i下的数目k_i，选择最大的k_i所属的类别w_i作为测试数据x的返回值。当K=1时，称为最近邻算法，即在样本数据D中，寻找最近邻x的样本，把x归为此样本类别下。常用距离度量为欧式距离。

**算法流程：**

![](https://img-blog.csdn.net/20160318150152793)


![](https://img-blog.csdn.net/20160318150851753)![](https://img-blog.csdn.net/20160318152128817)


**左图所示：在二维平面上要预测中间'*'所属颜色，采用K=11时的情况，其中有4黑色，7个蓝色，即预测'*'为蓝色。**

**右图所示：当K=1时，即最近邻算法，相当于把空间划分成N个区域，每个样本确定一块区域。每个区域中的点都归属于该样本的类别，因为该区域的数据点与所用样本相比与区域样本最近，此算法也被称为Voronoi tessellation**。




--------------------------------------------------------------------------------------------------------------------------------------------

  下面四副图像是在一个二维平面上，数据点类别为3类，采用K=10。图(a)为样本数据点；图(b)为平面上每个位置属于y=1(对应‘+’)的概率热量图像，图(c)为类别y=2(对应'*****')时对应的热量图像；图(d)采用MAP估计（即最大概率的类别）平面各点所属类别。

![](https://img-blog.csdn.net/20160318144605380)

------------------------------------------------------------------------------------------------------------------

# **KNN算法误差率**

假设最优贝叶斯分类率记为P_B,根据相关论文证明KNN算法的误差率为：

当数据样本量N趋于无穷大时，K=1时：    ![](https://img-blog.csdn.net/20160318162511124)，M为数据类别总数

当数据样本量N趋于无穷大时，M=2时:![](https://img-blog.csdn.net/20160318162808966);


由公式看出，KNN的算法要优于1-NN算法，因为降低了误差下界。并随着k的增大，P_kNN渐近于最优误差率P_B；事实上，当k->∞时(但仍然占样本总量N很小一部分)，KNN算法准确率趋近于贝叶斯分类器。




# **KNN算法的问题**

- 当数据量N很大，同时数据维度D很高，搜索效率会急剧下降。若采用暴力求解法，复杂度为![](https://img-blog.csdn.net/20160318164824305)。为增大效率，可以采用KD树等算法优化，见：[KD树与BBF算法解析](http://blog.csdn.net/luoshixian099/article/details/47606159)
- 有时根据现实情况，需要降低样本数量，可以采用prototype editing或者condensing算法等；prototype
 editing算法采用自身数据样本作为测试样本，应用KNN算法，若分类错误则剔除该样本。
- 当样本总量N很小时，会造成错误率上升。一种解决办法是训练度量距离方法，对不同的样本采用不同的度量方法目的是为了降低错误率，此种方法可以分为：全局方法(global)、类内方法(class-dependent)、局部方法(locally-dependent)。




Ref：Machine Learning: A Probabilistic Perspective

         Pattern Recognition,4th.










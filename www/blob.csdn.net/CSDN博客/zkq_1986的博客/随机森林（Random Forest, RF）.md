# 随机森林（Random Forest, RF） - zkq_1986的博客 - CSDN博客





2017年04月06日 16:00:43[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：173
个人分类：[数据挖掘																[Machine Learning](https://blog.csdn.net/zkq_1986/article/category/6374102)](https://blog.csdn.net/zkq_1986/article/category/6558845)








RF方法：

1. 训练集获取

利用bootstrap方法，选取m个样本，获得一个训练子集；重复T遍，获得T个训练子集。

2. 训练

以决策树作为基分类器。随机从属性集中随机选择k个属性，针对k个属性，采用传统的最优属性划分方法进行属性划分。对T个训练集进行训练，就得到T个不同的基分类器。

3.预测

针对分类任务，利用投票方式预测。

针对回归任务，利用简单平均方式预测。





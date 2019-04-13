
# sklearn 功能和包括的算法 - 追求卓越,做到专业 - CSDN博客


2018年12月07日 19:05:42[Waldenz](https://me.csdn.net/enter89)阅读数：81


**sklearn（Scikit learn）**是基于numpy和scipy的一个机器学习算法库，设计的非常优雅，它让我们能够使用同样的接口来实现所有不同的算法调用。
## 基本功能分为六大部分：
分类：支持向量机（SVM）、最近邻、逻辑回归、随机森林、决策树以及多层感知器（MLP）神经网络
回归：支持向量回归（SVR）
聚类：K-均值聚类
数据降维、
模型选择和
数据预处理。
缺点：不支持深度学习和强化学习，不支持Python外的其他语言，不支持GPU加速
## 常用算法
无监督学习算法
|算法|说明|
|cluster|聚类|
|decomposition|因子分解|
|mixture|高斯混合模型|
|neural_network|无监督的神经网络|
|covariance|协方差估计|
监督学习
|tree|决策树|
|svm|支持向量机|
|neighbors|近邻算法|
|linear_model|广义线性模型|
|neural_network|神经网络|
|kernel_ridge|岭回归|
|naive_bayes|朴素贝叶斯|
|cross_decomposition|交叉分解|
数据转换
|feature_extraction|特征提取|
|feature_selection|特征选择|
|preprocessing|预处理|
以上的每个模型中都包含多个算法，使用某算法时，直接导入需要的包即可
```python
from sklearn.linear_model import logistic
```



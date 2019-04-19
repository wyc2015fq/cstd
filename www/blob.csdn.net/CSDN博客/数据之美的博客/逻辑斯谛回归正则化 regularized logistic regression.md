# 逻辑斯谛回归正则化 regularized logistic regression - 数据之美的博客 - CSDN博客
2018年02月05日 18:03:00[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：84
个人分类：[深度学习/机器学习](https://blog.csdn.net/oppo62258801/article/category/6739087)
## 逻辑斯谛回归正则化 regularized logistic regression
关于logistic回归的基础知识请参见我的前两篇博客：[逻辑回归（代价函数，梯度下降） logistic regression--cost function and gradient descent](http://blog.csdn.net/u012328159/article/details/51077330) 和 [逻辑回归之决策边界
 logistic regression -- decision boundary](http://blog.csdn.net/u012328159/article/details/51068427)  。logistic回归存在过拟合问题，至于什么是过拟合，参见上篇博客[http://blog.csdn.net/u012328159/article/details/51089365](http://blog.csdn.net/u012328159/article/details/51089365)。直接来举个例子说明logistic回归中的过拟合问题（图片来源：Ng
 machine learning课）:
![](https://img-blog.csdn.net/20160408101522351)
上图左是欠拟合，可以看出logistic回归模型没有很好地拟合训练数据，训练误差很大。上图中是一个比较好的分类模型。上图右就是一个过拟合现象，这个复杂的分类模型可以准确无误的把训练数据的每个样本正确分类，但是其泛化能力很差。
同样logistic回归通过正则化（regularization）惩罚参数![](https://img-blog.csdn.net/20180124232000228)，防止其取得过大，可以避免过拟合问题（overfitting）。首先看代价函数（cost function）：
![](https://img-blog.csdn.net/20180124232320116)
其中红色框起来的那一项为正则项。
再来看其梯度下降（gradient descent）：
![](https://img-blog.csdn.net/20160408103623281)
关于logistic回归的正则化就介绍到这。

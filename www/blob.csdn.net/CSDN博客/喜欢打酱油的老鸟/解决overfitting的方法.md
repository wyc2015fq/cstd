
# 解决overfitting的方法 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月27日 09:29:39[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：544


解决overfitting的方法
Dropout， regularization， batch normalizatin。
但是要注意dropout只在训练的时候用，让一部分神经元随机失活。
Batch normalization是为了让输出都是单位高斯激活，方法是在连接和激活函数之间加入Batch Norm层，计算每个特征的均值和方差进行规则化。


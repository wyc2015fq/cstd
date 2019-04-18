# Sparse Filtering稀疏滤波 - wydbyxr的博客 - CSDN博客
2018年09月29日 13:34:11[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：287
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# Sparse Filtering稀疏滤波
  它根本上是一个特征提取器，一般来说，大部分的特征学习方法都是试图去建模给定训练数据的真实分布。
  SAE的稀疏是对于模型参数的稀疏，即在cost function中加入了权值惩罚项；而SF，首先对特征矩阵的每一行进行正则化，除以其二范数，同样的方式对每一列进行正则化，最后我们最小化特征矩阵各项的绝对值和。
  聚焦在非监督学习Unsupervised feature learning算法。因为一般的非监督算法需要调整很多额外的参数hyperparameter。本文提出一个简单的算法：sparse filtering。它只有一个hyperparameter（需要学习的特征数目）需要调整。但它很有效。与其他的特征学习方法不同，sparse filtering并没有明确的构建输入数据的分布的模型。它只优化一个简单的代价函数（L2范数稀疏约束的特征），优化过程可以通过几行简单的Matlab代码就可以实现。
#### 参考资料
[http://blog.csdn.net/zouxy09/article/details/9982859](http://blog.csdn.net/zouxy09/article/details/9982859)
[http://blog.csdn.net/itplus/article/details/22071035](http://blog.csdn.net/itplus/article/details/22071035)
[http://blog.csdn.net/u013884378/article/details/20610247](http://blog.csdn.net/u013884378/article/details/20610247)
[http://www.cnblogs.com/kemaswill/p/3341974.html](http://www.cnblogs.com/kemaswill/p/3341974.html)"

# 【IM】关于最小二乘法及约束的理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年09月30日 19:25:30[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：420








最小二乘法是对以实向量x为输入、实数y为输出的函数y=f(x)的学习问题。对于机器学习来说，关键就是构建一个f模型，然后设定求解目标，通过训练样本来求解。一个机器学习算法，关系模型、求解目标、求解方法。

![](https://img-blog.csdn.net/20180930192350949?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018093019240469?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里的重点是如何理解正交投影矩阵在SVD中的去噪解释，暂时没有更深入分享，后面有这方面总结在加以说明。




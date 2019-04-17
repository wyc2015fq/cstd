# 3分钟Tips：正定矩阵与对称矩阵的关系 - Machine Learning with Peppa - CSDN博客





2018年02月27日 18:43:20[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：2964







有一门学科，叫“线性代数”，在这个框架下，认为正定矩阵一定是对称矩阵。
还有一门学科，叫“矩阵论”，在这个框架下，认为正定矩阵不一定是对称矩阵。

所以，正定矩阵的定义就要求它必须是：埃尔米特矩阵，在实数上就是对称阵的意思。

WIKI上的定义为:In linear algebra, a symmetric n × n real matrix M is said to be positive definite if z^T*M*z is positive for every non-zero column vector z of n real numbers. Here z^T denotes the transpose of z.


可以理解为矩阵的正定来自于二次型的正定，而二次型的矩阵都是对称矩阵。所以，不少书籍在用正定矩阵时，又会特地在正定前面加上“对称”二字，称之为“对称正定矩阵”。举个例子，下面这个矩阵就是正定的，但很明显它并不对称。你可以左右乘(x1, x2)和其转置来算一下。
1  -1
1  0





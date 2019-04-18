# Regularization - 無名黑洞 - CSDN博客
2011年10月18日 10:23:49[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：9626
数学中的Regularization是为了解决overfitting问题而引入的一种方法。所谓overfitting就是在一些数学模型中由于过于复杂，有太多的观测参数，以至于一点点微小的误差都回产生巨大的影响，任何微小的数据扰动都会带来巨大的改变。在一些训练模型中用来fitting的data也会因为结构问题而Overfitting。
一般来说有两种克服Overfitting的方法：一是补偿模型的某些部分（如Regularization）；二是根据潜在的问题提供而外的数据来训练。
下面介绍Tikhonov Regularization作为例子。
在如下方程中：
Ax = b
要求解x的标准方法是最小二乘法：
Min || Ax – b ||^2
求解这个方程除了overfitting外，还有overdetermined (underdetermined)的问题。所谓overdetermined (underdetermined)，是指方程组中未知数的个数与方程数不不一致。如果把一个未知数看成一个自由度的话，每个单独的方程可以看成一个对应的constraint，当constraint的数目比未知数还多时，有可能找不到符合方程组的解；当方程的数目少于未知数时，可能有无穷多组解。显然当A不是方阵的时候肯定overdetermined(underdetermined)的。
A可能是奇异阵，这也是一个问题。
为了获得在某种条件下的解，加入regularization项如下：
|| Ax – b ||^2 + || Rx ||^2
其中，||.||是Euclidean norm，即平方和的根。
在很多情况下，R可以直接取单位阵。加入R之后可解如下：
x = (A^TA + RTR)^(-1)A^Tb
一些另外的取法。令R = a*L,L是对称Laplace-Beltrami算子。
L = V^(-1/2) * C * V^(-1/2)
V是一个voronoi area对角阵，C是一个系数对称cotangentweight矩阵。（cot(Betai,j） + cot(Betai,j)^-）/2, Beta是相邻三角形的角度。
可以把L作特征值分解，L = QBQ^T，Q为正交矩阵。因为正交矩阵不改变Frobenius norm，所以：
|| RU || = || aLU || = a|| QBQ^TU || = a|| BQ^TU||

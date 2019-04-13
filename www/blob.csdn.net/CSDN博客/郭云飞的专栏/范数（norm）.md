
# 范数（norm） - 郭云飞的专栏 - CSDN博客


2017年10月13日 17:42:17[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：557


不说那么多理论了，弄蒙咋整，直接说常见范数及其用途。
**一、向量范数**
1.1、 0-范数
严格说不属于范数，向量中非零元素的个数。
1.2、 1-范数
![](https://www.zhihu.com/equation?tex=%7C%7Cx%7C%7C_1+%3D+%5Csum_%7Bi%3D1%7D%5EN%7Cx_i%7C)
即向量元素绝对值之和，matlab调用函数norm(x, 1) 。
1.3、 2-范数
![](https://www.zhihu.com/equation?tex=%7C%7C%5Ctextbf%7Bx%7D%7C%7C_2+%3D%5Csqrt%7B%5Csum_%7Bi%3D1%7D%5ENx_i%5E2%7D)
*Euclid*范数（欧几里得范数，常用计算向量长度），即向量元素绝对值的平方和再开方，matlab调用函数norm(x, 2)。
1.4、 p-范数
![](https://www.zhihu.com/equation?tex=%7C%7C%5Ctextbf%7Bx%7D%7C%7C_p+%3D+%28%5Csum_%7Bi%3D1%7D%5EN%7Cx_i%7C%5Ep%29%5E%7B%5Cfrac%7B1%7D%7Bp%7D%7D)
即向量元素绝对值的p次方和的1/p次幂，matlab调用函数norm(x, p)。
1.5、 ∞-范数
![](https://www.zhihu.com/equation?tex=%7C%7C%5Ctextbf%7Bx%7D%7C%7C_%5Cinfty+%3D+%5Cmax_%7Bi%7D%7Cx_i%7C)
即所有向量元素绝对值中的最大值，matlab调用函数norm(x, inf)。
1.6、 -∞-范数
![](https://www.zhihu.com/equation?tex=%7C%7C%5Ctextbf%7Bx%7D%7C%7C_%7B-%5Cinfty%7D%3D%5Cmin_i%7Cx_i%7C)
即所有向量元素绝对值中的最小值，matlab调用函数norm(x, -inf)。
**二、矩阵范数**
2.1、 1-范数
![](https://www.zhihu.com/equation?tex=%7C%7CA%7C%7C_1+%3D+%5Cmax_j%5Csum_%7Bi%3D1%7D%5Em%7Ca_%7Bi%2Cj%7D%7C)
列和范数，即所有矩阵列向量绝对值之和的最大值，matlab调用函数norm(A, 1)。
2.2、 2-范数
对于实矩阵*A*，它的谱范数定义为：
![](https://img-blog.csdn.net/20171013180409381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中，**eig(X)**为计算方阵**X**特征值，它返回特征值向量：

谱范数，即*A'A*矩阵的最大特征值的开平方。matlab调用函数norm(x, 2)。
2.3、 ∞-范数
![](https://www.zhihu.com/equation?tex=%7C%7CA%7C%7C_%5Cinfty+%3D+%5Cmax_i%5Csum_%7Bj%3D1%7D%5EN%7Ca_%7Bi%2Cj%7D%7C)
行和范数，即所有矩阵行向量绝对值之和的最大值，matlab调用函数norm(A, inf)。
2.4、F-范数
![](https://www.zhihu.com/equation?tex=%7C%7CA%7C%7C_F%3D%5Cleft%28%5Csum_%7Bi%3D1%7D%5Em%5Csum_%7Bj%3D1%7D%5En%7Ca_%7Bi%2Cj%7D%7C%5E2%5Cright%29%5E%7B%5Cfrac%7B1%7D%7B2%7D%7D)
Frobenius范数，即矩阵元素绝对值的平方和再开平方，matlab调用函数norm(A, ’fro‘)。
2.5、核范数
![](https://img-blog.csdn.net/20171013181657457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
是*A*的奇异值。核范数即奇异值之和。


# Hessian矩阵正定与函数凹凸性的关系 - Machine Learning with Peppa - CSDN博客





2017年12月25日 19:15:58[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：12792
所属专栏：[机器学习与数据挖掘](https://blog.csdn.net/column/details/18961.html)[有趣的算法](https://blog.csdn.net/column/details/19022.html)










1. 从矩阵变换的角度

首先半正定矩阵定义为: ![X^TMX \geq 0](https://www.zhihu.com/equation?tex=X%5ETMX+%5Cgeq+0)
其中X 是向量，M 是变换矩阵

我们换一个思路看这个问题，矩阵变换中，![MX](https://www.zhihu.com/equation?tex=MX)代表对向量 X进行变换，我们假设变换后的向量为Y，记做![Y=MX](https://www.zhihu.com/equation?tex=Y%3DMX)。于是半正定矩阵可以写成：
![X^TY \geq 0](https://www.zhihu.com/equation?tex=X%5ETY+%5Cgeq+0)

这个是不是很熟悉呢？ 他是两个向量的内积。 同时我们也有公式：

![cos(\theta) = \frac{X^TY}{||X||* ||Y||}](https://www.zhihu.com/equation?tex=cos%28%5Ctheta%29+%3D+%5Cfrac%7BX%5ETY%7D%7B%7C%7CX%7C%7C%2A+%7C%7CY%7C%7C%7D)

||X||, ||Y||代表向量 X,Y的长度，![\theta](https://www.zhihu.com/equation?tex=%5Ctheta)是他们之间的夹角。 于是半正定矩阵意味着![cos(\theta)\geq 0](https://www.zhihu.com/equation?tex=cos%28%5Ctheta%29%5Cgeq+0), 这下明白了么？
正定、半正定矩阵的直觉代表一个向量经过它的变化后的向量与其本身的夹角小于等于90度。








**2. 从几何图形的角度**



**正定矩阵是一个椭球。**也就是说![n\times n](https://www.zhihu.com/equation?tex=n%5Ctimes+n)的正定矩阵![S_n](https://www.zhihu.com/equation?tex=S_n)对应于![n](https://www.zhihu.com/equation?tex=n)维空间中以原点为圆心的椭球![\varepsilon _{s_n} = \left\{ x|x^{T} S_{n}^{-1}x\leq 1\right\}](https://www.zhihu.com/equation?tex=%5Cvarepsilon+_%7Bs_n%7D+%3D+%5Cleft%5C%7B+x%7Cx%5E%7BT%7D+S_%7Bn%7D%5E%7B-1%7Dx%5Cleq+1%5Cright%5C%7D+),其中：
- 椭球的轴向：特征向量

- 椭球的轴长：特征值
下图是![n=2](https://www.zhihu.com/equation?tex=n%3D2)的情况:
![](https://img-blog.csdn.net/20171226181533684)这样理解的好处很多，例如两个正定矩阵![A\preceq B](https://www.zhihu.com/equation?tex=A%5Cpreceq+B)等价于![\varepsilon _A\subseteq \varepsilon _B](https://www.zhihu.com/equation?tex=%5Cvarepsilon+_A%5Csubseteq+%5Cvarepsilon+_B),也就是说，“两个正定矩阵的差也正定”等价于“A对应的椭球被B对应的椭球包含”，![n=3](https://www.zhihu.com/equation?tex=n%3D3)时，图像如下：
![](https://img-blog.csdn.net/20171226181540725)








**3. 判定方法**

正定性的判定方法有很多重，其中最方便也是常用的一种为：

若所有特征值均不小于零，则称为半正定。
若所有特征值均大于零，则称为正定。


当然，通过主元变换或直接求出行列式的值也是方法之一，但由于缺乏充分性，即行列式小于零一定非正定，但大于零则不一定正定，因为偶数次的负元素相乘依旧得正，因此用所有主元（对角线）上的元素来判断的方法更为完备。




**4. 黑塞矩阵的正定性**

Hessian矩阵的正定性在判断优化算法可行性时非常有用，简单地说，黑塞矩阵正定，则

1. 函数的二阶偏导数恒 > 0

2. 函数的变化率（斜率）即一阶导数始终处于递增状态

3. 函数为凸




因此，在诸如牛顿法等梯度方法中，使用黑塞矩阵的正定性可以非常便捷的判断函数是否有凸性，也就是是否可收敛到局部/全局的最优解





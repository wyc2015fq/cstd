# Jacobian矩阵和Hessian矩阵的理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年02月11日 11:16:54[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2420








深度学习中梯度向量的计算，Jacobian矩阵和Hessian矩阵是基础的知识点。



求微分其实就是线性化，导数其实就是线性空间之间的线性变换，Jaocibian矩阵本质上就是导数。

比如，映射![f:M\to N](https://www.zhihu.com/equation?tex=f%3AM%5Cto+N)在![x](https://www.zhihu.com/equation?tex=x)处的导数![df_x](https://www.zhihu.com/equation?tex=df_x)就是![M](https://www.zhihu.com/equation?tex=M)在![x](https://www.zhihu.com/equation?tex=x)处的切空间![TM_x](https://www.zhihu.com/equation?tex=TM_x)到![N](https://www.zhihu.com/equation?tex=N)在![f(x)](https://www.zhihu.com/equation?tex=f%28x%29)处的切空间![TN_{f(x)}](https://www.zhihu.com/equation?tex=TN_%7Bf%28x%29%7D)之间的线性映射。切空间都是矢量空间，都有基底，所以这个线性变换就是矩阵。在欧氏空间子空间的开集上，切空间就是某个![\mathbb{R}^n](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5En)，比如实轴上的切空间就是![\mathbb{R}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D)，曲面上的切空间为![\mathbb{R}^2](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5E2)。这样一想，函数![f:\mathbb{R}\to\mathbb{R}](https://www.zhihu.com/equation?tex=f%3A%5Cmathbb%7BR%7D%5Cto%5Cmathbb%7BR%7D)的导数无非就是切空间![T\mathbb{R}_x=\mathbb{R}](https://www.zhihu.com/equation?tex=T%5Cmathbb%7BR%7D_x%3D%5Cmathbb%7BR%7D)到切空间![T\mathbb{R}_{f(x)}=\mathbb{R}](https://www.zhihu.com/equation?tex=T%5Cmathbb%7BR%7D_%7Bf%28x%29%7D%3D%5Cmathbb%7BR%7D)的线性变换，是一个![1\times 1](https://www.zhihu.com/equation?tex=1%5Ctimes+1)矩阵，同构于一个实数。
因此，Jacobian矩阵实质上就是切空间之间的基底之间的线性变换，这也是为什么积分中变换坐标时前面会乘以一个Jacobian矩阵的行列式。





1、梯度向量：

定义：

目标函数f为单变量，是关于自变量向量x=(x1,x2,…,xn)T的函数，

单变量函数f对向量x求梯度，结果为一个与向量x同维度的向量，称之为梯度向量；



![图像算法：梯度vs Jacobian矩阵vs Hessian矩阵](https://imgsa.baidu.com/exp/w=500/sign=d754a91595510fb378197797e932c893/f9198618367adab47b74fcef80d4b31c8601e4f2.jpg)[](http://jingyan.baidu.com/album/cb5d6105c661bc005c2fe024.html?picindex=2)



2、Jacobian矩阵：

定义：

目标函数f为一个函数向量，f=(f1(x),f2(x),…fm(x))T;

其中，自变量x=(x1,x2,…,xn)T；

函数向量f对x求梯度，结果为一个矩阵；行数为f的维数；列数位x的维度，称之为Jacobian矩阵；

其每一行都是由相应函数的梯度向量转置构成的；

【注】：梯度向量Jacobian矩阵的一个特例；

当目标函数为标量函数时，Jacobian矩阵是梯度向量；



![图像算法：梯度vs Jacobian矩阵vs Hessian矩阵](https://imgsa.baidu.com/exp/w=500/sign=e6c30fc85d4e9258a63486eeac83d1d1/c9fcc3cec3fdfc037da50414df3f8794a5c226b9.jpg)


雅可比矩阵的重要性在于它体现了一个可微方程与给出点的最优线性逼近. 因此, 雅可比矩阵类似于多元函数的导数.

**雅可比行列式**

如果m = n, 那么是从n维空间到n维空间的函数, 且它的雅可比矩阵是一个方块矩阵. 于是我们可以取它的行列式, 称为雅可比行列式.在某个给定点的雅可比行列式提供了 在接近该点时的表现的重要信息. 例如, 如果连续可微函数在点的雅可比行列式不是零, 那么它在该点附近具有反函数. 这称为反函数定理. 更进一步, 如果点的雅可比行列式是正数, 则在点的取向不变；如果是负数, 则的取向相反. 而从雅可比行列式的绝对值, 就可以知道函数在点的缩放因子；这就是为什么它出现在换元积分法中.

对于取向问题可以这么理解, 例如一个物体在平面上匀速运动, 如果施加一个正方向的力, 即取向相同, 则加速运动, 类比于速度的导数加速度为正；如果施加一个反方向的力, 即取向相反, 则减速运动, 类比于速度的导数加速度为负.

3、Hessian矩阵：

实际上，Hessian矩阵是梯度向量g(x)对自变量x的Jacobian矩阵：

![图像算法：梯度vs Jacobian矩阵vs Hessian矩阵](https://imgsa.baidu.com/exp/w=500/sign=a62feb47bdfd5266a72b3c149b199799/1f178a82b9014a90d1e7a111a2773912b21bee45.jpg)[](http://jingyan.baidu.com/album/cb5d6105c661bc005c2fe024.html?picindex=4)[](http://jingyan.baidu.com/album/cb5d6105c661bc005c2fe024.html?picindex=3)


在数学中, 海森矩阵(Hessian matrix或Hessian)是一个自变量为向量的实值函数的二阶偏导数组成的方块矩阵。海森矩阵被应用于牛顿法解决的大规模优化问题.

**海森矩阵在牛顿法中的应用**

一般来说, 牛顿法主要应用在两个方面, 1, 求方程的根; 2, 最优化.

1),求解方程

并不是所有的方程都有求根公式, 或者求根公式很复杂, 导致求解困难. 利用牛顿法, 可以迭代求解.

原理是利用泰勒公式, 在0处展开, 且展开到一阶, 即000

求解方程0, 即0000, 求解1000, 因为这是利用泰勒公式的一阶展开, 000处并不是完全相等, 而是近似相等, 这里求得的1并不能让0, 只能说1的值比0更接近0, 于是乎, 迭代求解的想法就很自然了, 可以进而推出1, 通过迭代, 这个式子必然在0的时候收敛. 整个过程如下图：

![](http://ww4.sinaimg.cn/mw690/697b070fjw1dvpdvfz24hj.jpg)

2),最优化

在最优化的问题中, 线性最优化至少可以使用单纯形法(或称不动点算法)求解, 但对于非线性优化问题, 牛顿法提供了一种求解的办法. 假设任务是优化一个目标函数, 求函数的极大极小问题, 可以转化为求解函数的导数0的问题, 这样求可以把优化问题看成方程求解问题(0). 剩下的问题就和第一部分提到的牛顿法求解很相似了.这次为了求解0的根, 首先把在探索点处泰勒展开, 展开到2阶形式进行近似：

22

然后用的最小点做为新的探索点1，据此，令：

0

求得出迭代公式：

101

一般认为牛顿法可以利用到曲线本身的信息, 比梯度下降法更容易收敛（迭代更少次数）, 如下图是一个最小化一个目标方程的例子, 红色曲线是利用牛顿法迭代求解, 绿色曲线是利用梯度下降法求解.

![](http://ww1.sinaimg.cn/mw690/697b070fjw1dvpdvu65zij.jpg)

在上面讨论的是2维情况, 高维情况的牛顿迭代公式是：

110

其中H是hessian矩阵, 定义见上. 

高维情况依然可以用牛顿迭代求解, 但是问题是Hessian矩阵引入的复杂性, 使得牛顿迭代求解的难度大大增加, 但是已经有了解决这个问题的办法就是Quasi-Newton method, 不再直接计算hessian矩阵, 而是每一步的时候使用梯度向量更新hessian矩阵的近似.




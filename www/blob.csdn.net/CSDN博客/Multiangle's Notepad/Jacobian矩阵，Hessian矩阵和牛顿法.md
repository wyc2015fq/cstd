
# Jacobian矩阵，Hessian矩阵和牛顿法 - Multiangle's Notepad - CSDN博客


2017年02月28日 11:15:54[multiangle](https://me.csdn.net/u014595019)阅读数：3813


---
**转自**:[http://jacoxu.com/jacobian%E7%9F%A9%E9%98%B5%E5%92%8Chessian%E7%9F%A9%E9%98%B5/](http://jacoxu.com/jacobian%E7%9F%A9%E9%98%B5%E5%92%8Chessian%E7%9F%A9%E9%98%B5/)
# Jacobian矩阵
在向量分析中, 雅可比矩阵是一阶偏导数以一定方式排列成的矩阵, 其行列式称为雅可比行列式.
雅可比矩阵的重要性在于它体现了一个可微方程与给出点的最优线性逼近. 因此, 雅可比矩阵类似于多元函数的导数.
假设$F: R_n→R_m$是一个从欧式n维空间转换到欧式m维空间的函数. 这个函数由m个实函数组成:$y_1(x_1,…,x_n), …, y_m(x_1,…,x_n).$这些函数的偏导数(如果存在)可以组成一个m行n列的矩阵, 这就是所谓的雅可比矩阵：

$$
\begin{bmatrix}
\frac{\partial y_1}{\partial x_1} & \cdots & \frac{\partial y_1}{\partial x_n} \\
\vdots & \ddots & \vdots \\
\frac{\partial y_m}{\partial x_1} & \cdots & \frac{\partial y_m}{\partial x_n} 
\end{bmatrix}
$$
本质上是一个列向量对列向量求偏微分。此矩阵表示为:
$J_F(x_1,...,x_n)$或者$\frac{\partial(y_1,...,y_m)}{\partial(x_1,...,x_n)}$
如果$P$是$R_n$中的一点,$F$在$P$点可微分, 那么在这一点的导数由$J_F(P)$给出(这是求该点导数最简便的方法). 在此情况下, 由$F(P)$描述的线性算子即接近点$P$的$F$的最优线性逼近,$x$逼近于$P$:
$F(x)≈F(p)+J_F(p)⋅(x–p)$
# Hessian矩阵
在数学中, 海森矩阵(Hessian matrix或Hessian)是一个自变量为向量的实值函数的二阶偏导数组成的方块矩阵, 此函数如下：
$f(x_1,x_2,...,x_n)$
如果f的所有二阶导数都存在, 那么f的海森矩阵即：
$H(f)_{ij}(x)=D_iD_jf(x)$
其中H(f)为：

$$
\begin{bmatrix}
\frac{\partial^2f}{\partial x_1^2} & \cdots & \frac{\partial^2f}{\partial x_1\partial x_n} \\
\frac{\partial^2f}{\partial x_2\partial x_1} & \cdots & \frac{\partial^2f}{\partial x_2\partial x_n} \\
\vdots & \ddots & \vdots \\
\frac{\partial^2f}{\partial x_n\partial x_1} & \cdots & \frac{\partial^2f}{\partial x_n^2} \\
\end{bmatrix}
$$

# 海森矩阵在牛顿法中的应用
一般来说, 牛顿法主要应用在两个方面, 1, 求方程的根; 2, 最优化.
## 1.求解方程
并不是所有的方程都有求根公式, 或者求根公式很复杂, 导致求解困难. 利用牛顿法, 可以迭代求解.
原理是利用泰勒公式, 在$x_0$处展开, 且展开到一阶, 即

$$
f(x)=f(x_0)+(x-x_0)f'(x_0)
$$
求解方程$f(x)=0$，即$f({x_0}) + (x – {x_0})f’({x_0}) = 0$求解得$x = {x_1} = {x_0} – f({x_0})/f’({x_0})$因为这是利用泰勒公式的一阶展开,$f(x) = f({x_0}) + (x – {x_0})f’({x_0})$处并不是完全相等, 而是近似相等, 这里求得的$x_1$并不能让$f(x)=0$,只能说$f(x_1)$的值比$f(x_0)$更接近$f(x)=0$于是乎, 迭代求解的想法就很自然了, 可以进而推出${x_{n + 1}} = {x_n} – f({x_n})/f’({x_n})$通过迭代, 这个式子必然在$f({x^ * }){\rm{ = }}0$的时候收敛. 整个过程如下图：
![此处输入图片的描述](http://ww4.sinaimg.cn/mw690/697b070fjw1dvpdvfz24hj.jpg)
## 2. 最优化问题
在最优化的问题中, 线性最优化至少可以使用单纯形法(或称不动点算法)求解, 但对于非线性优化问题, 牛顿法提供了一种求解的办法. 假设任务是优化一个目标函数$f$, 求函数$f$的极大极小问题, 可以转化为求解函数$f$的导数$f′=0$的问题, 这样求可以把优化问题看成方程求解问题$(f'=0)$. 剩下的问题就和第一部分提到的牛顿法求解很相似了.
这次为了求解$f′=0$的根, 把$f(x)$的泰勒展开, 展开到2阶形式：

$$
f(x + \Delta x) = f\left( x \right) + f'(x)\Delta x + \frac{1}{2}f''(x)\Delta {x^2}
$$
这个式子是成立的, 当且仅当$\Delta x$无限趋近于0时$f(x + \Delta x) = f\left( x \right)$, 约去这两项, 并对余项式$f’(x)\Delta x + \frac{1}{2}f”(x)\Delta {x^2}{\rm{ = }}0$对$\Delta x$求导.此时上式等价于：

$$
f'(x) + f''(x)\Delta x{\rm{ = }}0
$$
求解 :

$$
\Delta x{\rm{ = - }}\frac{{f'({x_n})}}{{f''({x_n})}}
$$
得出迭代公式：

$$
{x_{n + 1}} = {x_n}{\rm{ - }}\frac{{f'({x_n})}}{{f''({x_n})}},n = 0,1,...
$$
一般认为牛顿法可以利用到曲线本身的信息, 比梯度下降法更容易收敛（迭代更少次数）, 如下图是一个最小化一个目标方程的例子, 红色曲线是利用牛顿法迭代求解, 绿色曲线是利用梯度下降法求解.
![此处输入图片的描述](http://ww1.sinaimg.cn/mw690/697b070fjw1dvpdvu65zij.jpg)
在上面讨论的是2维情况, 高维情况的牛顿迭代公式是：

$$
{x_{n + 1}} = {x_n} - {[Hf({x_n})]^{ – 1}}\nabla f({x_n}),n \ge 0
$$
其中H是hessian矩阵, 定义见上.
高维情况依然可以用牛顿迭代求解, 但是问题是Hessian矩阵引入的复杂性, 使得牛顿迭代求解的难度大大增加, 但是已经有了解决这个问题的办法就是Quasi-Newton method, 不再直接计算hessian矩阵, 而是每一步的时候使用梯度向量更新hessian矩阵的近似.


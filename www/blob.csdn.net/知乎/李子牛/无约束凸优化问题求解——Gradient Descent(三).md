# 无约束凸优化问题求解——Gradient Descent(三) - 知乎
# 

## 介绍

梯度下降(Gradient Descent)在深度学习中，被作为最基本的优化神经网络参数的算法。对于一个学过微积分的人来说，梯度下降的一个直观解释是：沿着目标函数下降最快的方向(梯度的负方向)走一小步。

> 重复进行：
        1. 确定下降方向： ![d_k = -\nabla f(x_k)](https://www.zhihu.com/equation?tex=d_k+%3D+-%5Cnabla+f%28x_k%29)
        2. 直线搜索步长： ![\alpha_k = \arg\min_{\alpha > 0} f(x_k + \alpha d_k )](https://www.zhihu.com/equation?tex=%5Calpha_k+%3D+%5Carg%5Cmin_%7B%5Calpha+%3E+0%7D+f%28x_k+%2B+%5Calpha+d_k+%29)
        3. 修改： ![x_{k+1}= x_k - \alpha_k d_k ](https://www.zhihu.com/equation?tex=x_%7Bk%2B1%7D%3D+x_k+-+%5Calpha_k+d_k+)

这里，我想回答以下几个问题：对于**无约束的凸优化问题**，即目标函数是凸函数，
- 梯度下降一定可以找到全局最小值吗？
- 梯度下降为什么能找到全局最小值？
- 梯度下降的收敛速度如何？

## 凸函数

(阅读过我的专栏里[凸优化问题介绍](https://zhuanlan.zhihu.com/p/50290361)的，可以跳过这部分！！!)

凸函数可以说是目标函数中最容易优化的一类函数。首先给出凸函数的定义：

> 对于定义域 ![dom(f)](https://www.zhihu.com/equation?tex=dom%28f%29) 是凸集的函数 ![f: \mathbb{R^n} \mapsto \mathbb{R}](https://www.zhihu.com/equation?tex=f%3A+%5Cmathbb%7BR%5En%7D+%5Cmapsto+%5Cmathbb%7BR%7D)， ![f(x)](https://www.zhihu.com/equation?tex=f%28x%29) 是凸函数 ![\Leftrightarrow](https://www.zhihu.com/equation?tex=%5CLeftrightarrow)![f(\alpha x+(1-\alpha)y) \geq \alpha f(x) + (1-\alpha) f(y) \quad \forall x, y \in dom(f), \alpha \in [0, 1] \\](https://www.zhihu.com/equation?tex=f%28%5Calpha+x%2B%281-%5Calpha%29y%29+%5Cgeq+%5Calpha+f%28x%29+%2B+%281-%5Calpha%29+f%28y%29+%5Cquad+%5Cforall+x%2C+y+%5Cin+dom%28f%29%2C+%5Calpha+%5Cin+%5B0%2C+1%5D+%5C%5C)
![](https://pic1.zhimg.com/v2-3eee2b751c97aa501f212235ee53fadc_b.jpg)(a)图：凸函数的另一种等价定义的解释：弦位于函数的上方。(b)图：凸函数不可能有局部极小值的直观解释。
**凸函数没有局部最优解**。

> ![\mathit{Proof:}](https://www.zhihu.com/equation?tex=%5Cmathit%7BProof%3A%7D)
假设 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 是凸函数 ![f](https://www.zhihu.com/equation?tex=f) 的全局最优解；如果 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 是局部最优解，那么必存在 ![\bar x \ne x^*](https://www.zhihu.com/equation?tex=%5Cbar+x+%5Cne+x%5E%2A) 使得 ![f(\bar x) \leq f(x^*)](https://www.zhihu.com/equation?tex=f%28%5Cbar+x%29+%5Cleq+f%28x%5E%2A%29) 。根据凸函数的定义：
![\begin{split} f(\alpha \bar x+ (1- \alpha) x^*) &\leq \alpha f(\bar x) + (1-\alpha) f(x^*) \\ &\leq \alpha f(x^*) + (1 - \alpha) f(x^*) \\ & \leq f(x^*) \end{split}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bsplit%7D+f%28%5Calpha+%5Cbar+x%2B+%281-+%5Calpha%29+x%5E%2A%29+%26%5Cleq+%5Calpha+f%28%5Cbar+x%29+%2B+%281-%5Calpha%29+f%28x%5E%2A%29+%5C%5C+%26%5Cleq+%5Calpha+f%28x%5E%2A%29+%2B+%281+-+%5Calpha%29+f%28x%5E%2A%29+%5C%5C+%26+%5Cleq+f%28x%5E%2A%29+%5Cend%7Bsplit%7D)
也就是说，位于 ![\bar x](https://www.zhihu.com/equation?tex=%5Cbar+x) 和 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 之间的线段上的函数值都小于 ![f(x^*)](https://www.zhihu.com/equation?tex=f%28x%5E%2A%29) ，这与 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 是局部最优解矛盾，因此 ![x^*](https://www.zhihu.com/equation?tex=x%5E%2A) 只能是全局最优解。

## 迭代算法

迭代算法旨在产生一个序列 ![\{x_k\}](https://www.zhihu.com/equation?tex=%5C%7Bx_k%5C%7D) 满足 ![f(x_{k+1}) \leq f(x_k)](https://www.zhihu.com/equation?tex=f%28x_%7Bk%2B1%7D%29+%5Cleq+f%28x_k%29) 。对于凸函数，如果我们能够找到这么一个序列的话，因为凸函数没有局部最优解，那么我们一定可以找到全局最优解。下面的问题就是如何构造这么一个序列 ![\{x_k\}](https://www.zhihu.com/equation?tex=%5C%7Bx_k%5C%7D) 。

首先给出可微凸函数的另一种等价定义。

> 对于定义域 ![dom(f)](https://www.zhihu.com/equation?tex=dom%28f%29) 是凸集的函数 ![f: \mathbb{R^n} \mapsto \mathbb{R}](https://www.zhihu.com/equation?tex=f%3A+%5Cmathbb%7BR%5En%7D+%5Cmapsto+%5Cmathbb%7BR%7D)，且 ![f(x)](https://www.zhihu.com/equation?tex=f%28x%29) 一阶可导。 ![f(x)](https://www.zhihu.com/equation?tex=f%28x%29) 是凸函数 ![\Leftrightarrow](https://www.zhihu.com/equation?tex=%5CLeftrightarrow)
![f(y) \geq f(x) + \nabla f(x)^T (y-x) \quad \forall x, y \in dom(f) \\](https://www.zhihu.com/equation?tex=f%28y%29+%5Cgeq+f%28x%29+%2B+%5Cnabla+f%28x%29%5ET+%28y-x%29+%5Cquad+%5Cforall+x%2C+y+%5Cin+dom%28f%29+%5C%5C)
![](https://pic1.zhimg.com/v2-d5580b5ccaf58b81eee3e42660c9d07c_b.jpg)可导凸函数的一阶Lower Bound：任意一点的切线位于函数曲线的下方。
令 ![y = x_{k+1}, x = x_{k}](https://www.zhihu.com/equation?tex=y+%3D+x_%7Bk%2B1%7D%2C+x+%3D+x_%7Bk%7D) ，如果 ![\nabla f(x_k) (x_{k+1} - x_k) \ge 0 ](https://www.zhihu.com/equation?tex=%5Cnabla+f%28x_k%29+%28x_%7Bk%2B1%7D+-+x_k%29+%5Cge+0+) ，我们可以发现

![f(x_{k+1}) - f(x_k) \ge \nabla f(x_k)^T (x_{k+1} - x_k) \ge 0 \\](https://www.zhihu.com/equation?tex=f%28x_%7Bk%2B1%7D%29+-+f%28x_k%29+%5Cge+%5Cnabla+f%28x_k%29%5ET+%28x_%7Bk%2B1%7D+-+x_k%29+%5Cge+0+%5C%5C)

所以迭代序列 ![\{x_k\}](https://www.zhihu.com/equation?tex=%5C%7Bx_k%5C%7D) 下降的必要条件是 ![\nabla f(x_k)^T (x_{k+1} - x_k) \leq 0 ](https://www.zhihu.com/equation?tex=%5Cnabla+f%28x_k%29%5ET+%28x_%7Bk%2B1%7D+-+x_k%29+%5Cleq+0+) 。

## 梯度下降

首先，我们来验证梯度下降产生的序列 ![\{x_k\}](https://www.zhihu.com/equation?tex=%5C%7Bx_k%5C%7D) 满足迭代序列下降的要求。

![\nabla f(x_k) ^T (x_{k+1} - x_k) = - \alpha_k \nabla f(x_k)^T \nabla f(x_k) \leq 0 \\](https://www.zhihu.com/equation?tex=%5Cnabla+f%28x_k%29+%5ET+%28x_%7Bk%2B1%7D+-+x_k%29+%3D+-+%5Calpha_k+%5Cnabla+f%28x_k%29%5ET+%5Cnabla+f%28x_k%29+%5Cleq+0+%5C%5C)

接下来，我们来回答梯度下降是如何找到全局最优解，以及收敛速率问题。

1）定界

> 这里，我们假设函数 ![f(x)](https://www.zhihu.com/equation?tex=f%28x%29) 是强凸的，强凸函数 ![f(x)](https://www.zhihu.com/equation?tex=f%28x%29) 有如下Quadratic Bound： 
![\forall x, y \in dom(f), \exists M > m >  0](https://www.zhihu.com/equation?tex=%5Cforall+x%2C+y+%5Cin+dom%28f%29%2C+%5Cexists+M+%3E+m+%3E++0) ，
![\begin{equation} f(y) \geq f(x) + \nabla f(x)^T(y-x) + \frac{m}{2} ||y-x||^2    \quad\quad\quad(1) \\ \end{equation}  f(y) \leq f(x) + \nabla f(x)^T(y-x) + \frac{M}{2} ||y-x||^2 \quad\quad\quad (2) \\](https://www.zhihu.com/equation?tex=%5Cbegin%7Bequation%7D+f%28y%29+%5Cgeq+f%28x%29+%2B+%5Cnabla+f%28x%29%5ET%28y-x%29+%2B+%5Cfrac%7Bm%7D%7B2%7D+%7C%7Cy-x%7C%7C%5E2++++%5Cquad%5Cquad%5Cquad%281%29+%5C%5C+%5Cend%7Bequation%7D++f%28y%29+%5Cleq+f%28x%29+%2B+%5Cnabla+f%28x%29%5ET%28y-x%29+%2B+%5Cfrac%7BM%7D%7B2%7D+%7C%7Cy-x%7C%7C%5E2+%5Cquad%5Cquad%5Cquad+%282%29+%5C%5C)
令 ![x^* = \arg\min_x{f(x)}, \, p^* = f(x^*)](https://www.zhihu.com/equation?tex=x%5E%2A+%3D+%5Carg%5Cmin_x%7Bf%28x%29%7D%2C+%5C%2C+p%5E%2A+%3D+f%28x%5E%2A%29) ，则 ![\forall x \in dom(f)](https://www.zhihu.com/equation?tex=%5Cforall+x+%5Cin+dom%28f%29) ，对不等式(1)两边同时关于 ![y](https://www.zhihu.com/equation?tex=y) 求最大值（注意不等式右边是关于 ![y](https://www.zhihu.com/equation?tex=y) 的二次函数）：
![ p^* \geq f(x) - \frac{1}{2m}||\nabla f(x)||^2 \\ ](https://www.zhihu.com/equation?tex=+p%5E%2A+%5Cgeq+f%28x%29+-+%5Cfrac%7B1%7D%7B2m%7D%7C%7C%5Cnabla+f%28x%29%7C%7C%5E2+%5C%5C+)
整理后：
![f(x) \leq p^* + \frac{1}{2m}||\nabla f(x)||^2 \\ ](https://www.zhihu.com/equation?tex=f%28x%29+%5Cleq+p%5E%2A+%2B+%5Cfrac%7B1%7D%7B2m%7D%7C%7C%5Cnabla+f%28x%29%7C%7C%5E2+%5C%5C+)
 则对任意一个迭代中间值 ![f(x_k)](https://www.zhihu.com/equation?tex=f%28x_k%29) ，其与最优值 ![p^*](https://www.zhihu.com/equation?tex=p%5E%2A) 的误差不超过 ![||\nabla f(x_k) ||^2](https://www.zhihu.com/equation?tex=%7C%7C%5Cnabla+f%28x_k%29+%7C%7C%5E2) 。

2）收敛性

>  令 ![g(\alpha) = f(x_k - \alpha \nabla f(x_k))](https://www.zhihu.com/equation?tex=g%28%5Calpha%29+%3D+f%28x_k+-+%5Calpha+%5Cnabla+f%28x_k%29%29) ，将 ![y = x_k - \alpha \nabla f(x_k)](https://www.zhihu.com/equation?tex=y+%3D+x_k+-+%5Calpha+%5Cnabla+f%28x_k%29) 代入到上面的不等式(2)中，有：
![g(\alpha) \leq f(x_k) - \alpha||\nabla f(x_k)||^2 + \frac{M{\alpha}^2}{2} ||\nabla f(x_k)||^2 \\](https://www.zhihu.com/equation?tex=g%28%5Calpha%29+%5Cleq+f%28x_k%29+-+%5Calpha%7C%7C%5Cnabla+f%28x_k%29%7C%7C%5E2+%2B+%5Cfrac%7BM%7B%5Calpha%7D%5E2%7D%7B2%7D+%7C%7C%5Cnabla+f%28x_k%29%7C%7C%5E2+%5C%5C)
在梯度下降算法的第二步进行直线搜索的时候，我们会对上式关于 ![\alpha](https://www.zhihu.com/equation?tex=%5Calpha) 求最小（注意不等式的右边是关于 ![\alpha](https://www.zhihu.com/equation?tex=%5Calpha) 的二次函数），得到 ![\alpha](https://www.zhihu.com/equation?tex=%5Calpha) 的最优解 ![\alpha^*](https://www.zhihu.com/equation?tex=%5Calpha%5E%2A) 。
![f(x_{k+1}) = g(\alpha^*) \leq f(x_k) - \frac{1}{2M} ||\nabla f(x)||^2 \\](https://www.zhihu.com/equation?tex=f%28x_%7Bk%2B1%7D%29+%3D+g%28%5Calpha%5E%2A%29+%5Cleq+f%28x_k%29+-+%5Cfrac%7B1%7D%7B2M%7D+%7C%7C%5Cnabla+f%28x%29%7C%7C%5E2+%5C%5C)
上式两边同时减去 ![p^*](https://www.zhihu.com/equation?tex=p%5E%2A) 有：
![f(x_{k+1}) - p^* \leq f(x_k) - p^* - \frac{1}{2M}||\nabla f(x_k)||^2 \\](https://www.zhihu.com/equation?tex=f%28x_%7Bk%2B1%7D%29+-+p%5E%2A+%5Cleq+f%28x_k%29+-+p%5E%2A+-+%5Cfrac%7B1%7D%7B2M%7D%7C%7C%5Cnabla+f%28x_k%29%7C%7C%5E2+%5C%5C)
结合1）中的结论： ![||\nabla f(x) ||^2 \geq 2m(f(x_k) - p*)](https://www.zhihu.com/equation?tex=%7C%7C%5Cnabla+f%28x%29+%7C%7C%5E2+%5Cgeq+2m%28f%28x_k%29+-+p%2A%29) ，有：
![f(x_{k+1}) - p^* \leq (1-\frac{m}{M})(f(x_k) - p^*) \\](https://www.zhihu.com/equation?tex=f%28x_%7Bk%2B1%7D%29+-+p%5E%2A+%5Cleq+%281-%5Cfrac%7Bm%7D%7BM%7D%29%28f%28x_k%29+-+p%5E%2A%29+%5C%5C)
注意到上式是对 ![\forall k \geq 0](https://www.zhihu.com/equation?tex=%5Cforall+k+%5Cgeq+0) 成立，从 ![k=0, 1,\cdots, n](https://www.zhihu.com/equation?tex=k%3D0%2C+1%2C%5Ccdots%2C+n) 应用上式，有：
![\begin{split} f(x_n) - p^* &\leq c (f(x_{n-1}) - p^*) \\ &\leq c^2(f(x_{n-2}) - p^*) \\ & \leq \cdots \\ &\leq c^n(f(x_0) - p^*)  \end{split}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bsplit%7D+f%28x_n%29+-+p%5E%2A+%26%5Cleq+c+%28f%28x_%7Bn-1%7D%29+-+p%5E%2A%29+%5C%5C+%26%5Cleq+c%5E2%28f%28x_%7Bn-2%7D%29+-+p%5E%2A%29+%5C%5C+%26+%5Cleq+%5Ccdots+%5C%5C+%26%5Cleq+c%5En%28f%28x_0%29+-+p%5E%2A%29++%5Cend%7Bsplit%7D)
其中， ![c= 1 - \frac{m}{M} < 1](https://www.zhihu.com/equation?tex=c%3D+1+-+%5Cfrac%7Bm%7D%7BM%7D+%3C+1) 于是，当 ![n \to \infty](https://www.zhihu.com/equation?tex=n+%5Cto+%5Cinfty) 时， ![f(x_n) \to p^*](https://www.zhihu.com/equation?tex=f%28x_n%29+%5Cto+p%5E%2A) 。因此，**梯度下降是一个压缩算子**。

3）收敛速度

> 给定精度 ![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon) ，代入到上面的不等式，可以解出迭代次数 ![n](https://www.zhihu.com/equation?tex=n) :
![n \leq \frac{\log\Big((f(x_0)-p^*)/\epsilon \Big)}{\log(1/c)} \\](https://www.zhihu.com/equation?tex=n+%5Cleq+%5Cfrac%7B%5Clog%5CBig%28%28f%28x_0%29-p%5E%2A%29%2F%5Cepsilon+%5CBig%29%7D%7B%5Clog%281%2Fc%29%7D+%5C%5C)
当 ![\frac{M}{m}](https://www.zhihu.com/equation?tex=%5Cfrac%7BM%7D%7Bm%7D) 比较大的时候， ![\log(1/c) = -\log (1- m/M) \approx m/M](https://www.zhihu.com/equation?tex=%5Clog%281%2Fc%29+%3D+-%5Clog+%281-+m%2FM%29+%5Capprox+m%2FM) ，因此
![n \sim O(\frac{M}{m}) \\](https://www.zhihu.com/equation?tex=n+%5Csim+O%28%5Cfrac%7BM%7D%7Bm%7D%29+%5C%5C)
因此**梯度下降是线性收敛速度**。

## 总结

至此我们回答了刚开始提出的3个问题：

> 1. 梯度下降一定可以找到全局最小值吗？
2. 梯度下降为什么能找到全局最小值？

梯度下降一定可以找到无约束的凸函数的全局最小值，因为凸函数没有局部最小值，且迭代算子(为压缩算子)保证其可以收敛到最优解。

> 3. 梯度下降的收敛速度如何？

关于![\frac{M}{m}](https://www.zhihu.com/equation?tex=%5Cfrac%7BM%7D%7Bm%7D) (由函数本身性质决定)的线性收敛。

阅读本系列的其他文章
- [凸优化系列(一)](https://zhuanlan.zhihu.com/p/50283897)
- [凸优化问题介绍(二)](https://zhuanlan.zhihu.com/p/50290361)
- 无约束凸优化问题求解——Gradient Descent(三)
- [无约束凸优化问题求解——Newton Method(四)](https://zhuanlan.zhihu.com/p/50348622)
- [带约束凸优化问题最优解的存在条件——KKT条件(五)](https://zhuanlan.zhihu.com/p/50230049)
- [带等式约束凸优化问题的求解——Newton Method(六)](https://zhuanlan.zhihu.com/p/50411305)
- [带不等式约束凸优化问题的求解——内点法(七)](https://zhuanlan.zhihu.com/p/50443123)


# 带等式约束凸优化问题的求解——Newton Method(六) - 知乎
# 

## 简介

前面的系列我们介绍了Gradient Descent和Newton Method求解无约束的凸优化问题，并且给出了带约束问题的可行解的存在条件——KKT条件。在这篇文章里，我们考虑带等式约束的凸优化问题：

![\begin{align} \min \,&f(x) \\  \mathrm{subject \,\, to} \,& Ax=b \end{align} \tag{1}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmin+%5C%2C%26f%28x%29+%5C%5C++%5Cmathrm%7Bsubject+%5C%2C%5C%2C+to%7D+%5C%2C%26+Ax%3Db+%5Cend%7Balign%7D+%5Ctag%7B1%7D)

其中 ![f](https://www.zhihu.com/equation?tex=f) 二阶可导， ![A \in R^{p \times n}, \mathrm{rank}(A) = p < n](https://www.zhihu.com/equation?tex=A+%5Cin+R%5E%7Bp+%5Ctimes+n%7D%2C+%5Cmathrm%7Brank%7D%28A%29+%3D+p+%3C+n) 。根据[KKT条件](https://zhuanlan.zhihu.com/p/50230049)，我们知道 ![x^* \in dom(f)](https://www.zhihu.com/equation?tex=x%5E%2A+%5Cin+dom%28f%29) 是优化问题(1)的最优解的充要条件是，存在 ![\nu^* \in \mathbb{R}^n](https://www.zhihu.com/equation?tex=%5Cnu%5E%2A+%5Cin+%5Cmathbb%7BR%7D%5En) 满足

![Ax^* = b \quad \nabla f(x^*) + A^T \nu^* = 0 \tag{2}](https://www.zhihu.com/equation?tex=Ax%5E%2A+%3D+b+%5Cquad+%5Cnabla+f%28x%5E%2A%29+%2B+A%5ET+%5Cnu%5E%2A+%3D+0+%5Ctag%7B2%7D)

所以，求解优化问题(1)等价于求解 ![n+p](https://www.zhihu.com/equation?tex=n%2Bp) 个变量组成的方程(2)。一般而言 ![ \nabla f(x^*) + A^T \nu^* = 0](https://www.zhihu.com/equation?tex=+%5Cnabla+f%28x%5E%2A%29+%2B+A%5ET+%5Cnu%5E%2A+%3D+0) 是非线性的，很难求出其解析解。但在下面的部分，我们将考虑 ![f](https://www.zhihu.com/equation?tex=f) 的二阶近似，从而使得 ![\nabla f(x^*)](https://www.zhihu.com/equation?tex=%5Cnabla+f%28x%5E%2A%29) 线性化。

## 等式约束凸二次规划

考虑 ![f](https://www.zhihu.com/equation?tex=f) 为二次函数的情况

![\begin{align} \min \,&f(x) = \frac{1}{2}x^TPx + q^Tx + r \\  \mathrm{subject \,\, to} \,& Ax=b \end{align} \tag{3}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmin+%5C%2C%26f%28x%29+%3D+%5Cfrac%7B1%7D%7B2%7Dx%5ETPx+%2B+q%5ETx+%2B+r+%5C%5C++%5Cmathrm%7Bsubject+%5C%2C%5C%2C+to%7D+%5C%2C%26+Ax%3Db+%5Cend%7Balign%7D+%5Ctag%7B3%7D)

根据凸性， ![P \in S_+^n, A \in \mathbb{R}^{p \times n}](https://www.zhihu.com/equation?tex=P+%5Cin+S_%2B%5En%2C+A+%5Cin+%5Cmathbb%7BR%7D%5E%7Bp+%5Ctimes+n%7D) 。该问题是扩展Newton Method处理等式约束问题的基础。此时最优条件可以写为

![Ax^* = b, \quad Px^* + q + A^Tv^* = 0 \\](https://www.zhihu.com/equation?tex=Ax%5E%2A+%3D+b%2C+%5Cquad+Px%5E%2A+%2B+q+%2B+A%5ETv%5E%2A+%3D+0+%5C%5C)

用矩阵表示为

![\Big[\begin{array}{cc} P & A^T \\ A & 0  \end{array} \Big] \Big[ \begin{array}{c} x^* \\ \nu^* \end{array}\Big] = \Big[ \begin{array}{c} -q \\ b \end{array}\Big] \tag{4}](https://www.zhihu.com/equation?tex=%5CBig%5B%5Cbegin%7Barray%7D%7Bcc%7D+P+%26+A%5ET+%5C%5C+A+%26+0++%5Cend%7Barray%7D+%5CBig%5D+%5CBig%5B+%5Cbegin%7Barray%7D%7Bc%7D+x%5E%2A+%5C%5C+%5Cnu%5E%2A+%5Cend%7Barray%7D%5CBig%5D+%3D+%5CBig%5B+%5Cbegin%7Barray%7D%7Bc%7D+-q+%5C%5C+b+%5Cend%7Barray%7D%5CBig%5D+%5Ctag%7B4%7D)

这 ![n+p](https://www.zhihu.com/equation?tex=n%2Bp) 个变量组成的线性方程组称为**KKT矩阵**。如果KKT矩阵非奇异，存在唯一最优的原对偶对 ![(x^*, \nu^*)](https://www.zhihu.com/equation?tex=%28x%5E%2A%2C+%5Cnu%5E%2A%29) ；如果KKT矩阵奇异，但是有解，任何解都构成最优对偶对 ![(x^*, \nu^*)](https://www.zhihu.com/equation?tex=%28x%5E%2A%2C+%5Cnu%5E%2A%29) ；如果KKT矩阵无解，那么二次优化问题或者无解或者无下界。

下面指出KKT矩阵非奇异的一个充分条件：**![P](https://www.zhihu.com/equation?tex=P) 正定**。

## 等式约束的Newton方法

前面指出，一般情况下(2)是非线性方程，很难有解析解。为了方便求解，我们对目标函数 ![f](https://www.zhihu.com/equation?tex=f) 在 ![x](https://www.zhihu.com/equation?tex=x) 处做二阶近似。

![\begin{align} \min \,&f(x+v) = f(x) + \nabla f(x)^Tv + \frac{1}{2} v^T \nabla^2 f(x) v \\  \mathrm{subject \,\, to} \,& A(x+v)=b \quad \mathrm{or} \quad Av=0 \end{align} \tag{5}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmin+%5C%2C%26f%28x%2Bv%29+%3D+f%28x%29+%2B+%5Cnabla+f%28x%29%5ETv+%2B+%5Cfrac%7B1%7D%7B2%7D+v%5ET+%5Cnabla%5E2+f%28x%29+v+%5C%5C++%5Cmathrm%7Bsubject+%5C%2C%5C%2C+to%7D+%5C%2C%26+A%28x%2Bv%29%3Db+%5Cquad+%5Cmathrm%7Bor%7D+%5Cquad+Av%3D0+%5Cend%7Balign%7D+%5Ctag%7B5%7D)

该问题的变量为 ![v](https://www.zhihu.com/equation?tex=v) ，且是关于 ![v](https://www.zhihu.com/equation?tex=v) 的等式约束凸二次规划问题。我们假定KKT矩阵非奇异，在此基础上定义 ![x](https://www.zhihu.com/equation?tex=x) 处的Newton下降方向 ![\Delta x_{nt}](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D) 为凸二次问题(5)的解。根据(4)，Newton下降方向 ![\Delta x_{nt}](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D) 由以下KKT方程决定。

![\Big[\begin{array}{cc} \nabla^2f(x) & A^T \\ A & 0  \end{array} \Big] \Big[ \begin{array}{c} \Delta x_{nt} \\ \ w \end{array}\Big] = \Big[ \begin{array}{c} -\nabla f(x) \\ 0 \end{array}\Big] \tag{6}](https://www.zhihu.com/equation?tex=%5CBig%5B%5Cbegin%7Barray%7D%7Bcc%7D+%5Cnabla%5E2f%28x%29+%26+A%5ET+%5C%5C+A+%26+0++%5Cend%7Barray%7D+%5CBig%5D+%5CBig%5B+%5Cbegin%7Barray%7D%7Bc%7D+%5CDelta+x_%7Bnt%7D+%5C%5C+%5C+w+%5Cend%7Barray%7D%5CBig%5D+%3D+%5CBig%5B+%5Cbegin%7Barray%7D%7Bc%7D+-%5Cnabla+f%28x%29+%5C%5C+0+%5Cend%7Barray%7D%5CBig%5D+%5Ctag%7B6%7D)

其中 ![w](https://www.zhihu.com/equation?tex=w) 是该二次问题的最优对偶变量(防止符号滥用，我们这里没有再使用 ![\nu](https://www.zhihu.com/equation?tex=%5Cnu) )。所以求解问题(5)等于求解方程组(6)。

首先我们说明这样的一个下降方向 ![\Delta x_{nt}](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D) 是满足迭代算法要求的。首先，可以看出 **![A \Delta x_{nt}=0](https://www.zhihu.com/equation?tex=A+%5CDelta+x_%7Bnt%7D%3D0) 满足等式约束**；此外， ![f](https://www.zhihu.com/equation?tex=f) 沿 ![\Delta x_{nt}](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D) 处的方向导数是小于0的，说明 **![\Delta x_{nt}](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D) 是一个下降方向**。

![\frac{d}{dt} f(x+t\Delta x_{nt}) \Big|_{t=0} =\nabla f(x)^T \Delta x_{nt} = -\Delta x_{nt}^T \nabla^2 f(x) \Delta x_{nt} < 0 \\](https://www.zhihu.com/equation?tex=%5Cfrac%7Bd%7D%7Bdt%7D+f%28x%2Bt%5CDelta+x_%7Bnt%7D%29+%5CBig%7C_%7Bt%3D0%7D+%3D%5Cnabla+f%28x%29%5ET+%5CDelta+x_%7Bnt%7D+%3D+-%5CDelta+x_%7Bnt%7D%5ET+%5Cnabla%5E2+f%28x%29+%5CDelta+x_%7Bnt%7D+%3C+0+%5C%5C)

下面，我们给出等式约束的Newton Method算法的框架，可以看出其和无约束情况下完全一样。

> 重复进行：
1. 计算Newton方向![\Delta x_{nt}](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D) ，即求解KKT方程(6)
2. 计算Newton减量 ![\lambda(x) =(\Delta x_{nt}^T \nabla^2 f(x) \Delta x_{nt})^{1/2}](https://www.zhihu.com/equation?tex=%5Clambda%28x%29+%3D%28%5CDelta+x_%7Bnt%7D%5ET+%5Cnabla%5E2+f%28x%29+%5CDelta+x_%7Bnt%7D%29%5E%7B1%2F2%7D)
3. 停止准则：如果 ![\lambda^2/2 \leq \epsilon](https://www.zhihu.com/equation?tex=%5Clambda%5E2%2F2+%5Cleq+%5Cepsilon) ，则退出
4. 直线搜索：通过回溯直线法确定步长 ![t](https://www.zhihu.com/equation?tex=t)
5. 改进： ![x:= x+ t \Delta x_{nt}](https://www.zhihu.com/equation?tex=x%3A%3D+x%2B+t+%5CDelta+x_%7Bnt%7D)

同样地，Newton Method收敛也存在阻尼Newton阶段和纯Newton阶段。阻尼Newton阶段收敛较慢，但有界；纯Newton阶段收敛十分迅速。下面说明如何求解KKT系统得到 ![\Delta x_{nt}](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D) 。

## 求解KKT系统

这里专门介绍求解线性方程组(6)是因为我们可以利用 ![\nabla^2 f(x)](https://www.zhihu.com/equation?tex=%5Cnabla%5E2+f%28x%29) 的正定性，加速计算。

![\Big[\begin{array}{cc} \nabla^2f(x) & A^T \\ A & 0  \end{array} \Big] \Big[ \begin{array}{c} \Delta x_{nt} \\ \ w \end{array}\Big] = \Big[ \begin{array}{c} -\nabla f(x) \\ 0 \end{array}\Big] \\](https://www.zhihu.com/equation?tex=%5CBig%5B%5Cbegin%7Barray%7D%7Bcc%7D+%5Cnabla%5E2f%28x%29+%26+A%5ET+%5C%5C+A+%26+0++%5Cend%7Barray%7D+%5CBig%5D+%5CBig%5B+%5Cbegin%7Barray%7D%7Bc%7D+%5CDelta+x_%7Bnt%7D+%5C%5C+%5C+w+%5Cend%7Barray%7D%5CBig%5D+%3D+%5CBig%5B+%5Cbegin%7Barray%7D%7Bc%7D+-%5Cnabla+f%28x%29+%5C%5C+0+%5Cend%7Barray%7D%5CBig%5D+%5C%5C)

不失一般性，我们可以直接求解这个线性方程组。不利用矩阵的结构，计算量是 ![(1/3)(n+p)^3](https://www.zhihu.com/equation?tex=%281%2F3%29%28n%2Bp%29%5E3) 次浮点运算。当 ![n](https://www.zhihu.com/equation?tex=n) 和 ![p](https://www.zhihu.com/equation?tex=p) 都不大时，这是一个合理的处理方法。

此外，我们可以采用消元法。假设 ![\nabla^2 f(x)](https://www.zhihu.com/equation?tex=%5Cnabla%5E2+f%28x%29) 正定，利用KKT方程组第一个方程 ![\nabla^2 f(x) \Delta x_{nt} + A^T w = -\nabla f(x) \\](https://www.zhihu.com/equation?tex=%5Cnabla%5E2+f%28x%29+%5CDelta+x_%7Bnt%7D+%2B+A%5ET+w+%3D+-%5Cnabla+f%28x%29+%5C%5C)

可以解出 ![\Delta x_{nt}](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D)

![\Delta x_{nt} = - {\nabla^2 f(x)}^{-1} (\nabla f(x) + A^T w) \\](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D+%3D+-+%7B%5Cnabla%5E2+f%28x%29%7D%5E%7B-1%7D+%28%5Cnabla+f%28x%29+%2B+A%5ET+w%29+%5C%5C)

然后将其带入KKT方程组的第二个方程，可以解出 ![w](https://www.zhihu.com/equation?tex=w)

![w = (A {\nabla^2 f(x)}^{-1}A^T)^{-1} (-A{\nabla^2 f(x)}^{-1}\nabla f(x)) \\](https://www.zhihu.com/equation?tex=w+%3D+%28A+%7B%5Cnabla%5E2+f%28x%29%7D%5E%7B-1%7DA%5ET%29%5E%7B-1%7D+%28-A%7B%5Cnabla%5E2+f%28x%29%7D%5E%7B-1%7D%5Cnabla+f%28x%29%29+%5C%5C)

下面给出基于消元法的求解过程：

> 1. 计算 ![{\nabla^2 f(x)}^{-1} A^T](https://www.zhihu.com/equation?tex=%7B%5Cnabla%5E2+f%28x%29%7D%5E%7B-1%7D+A%5ET) 和 ![{\nabla^2 f(x)}^{-1} \nabla f(x)](https://www.zhihu.com/equation?tex=%7B%5Cnabla%5E2+f%28x%29%7D%5E%7B-1%7D+%5Cnabla+f%28x%29)
2. 计算Schur补 ![S = -A {\nabla^2 f(x)}^{-1} A^T](https://www.zhihu.com/equation?tex=S+%3D+-A+%7B%5Cnabla%5E2+f%28x%29%7D%5E%7B-1%7D+A%5ET)
3. 求解 ![Sw = A{\nabla^2 f(x)}^{-1} \nabla f(x) ](https://www.zhihu.com/equation?tex=Sw+%3D+A%7B%5Cnabla%5E2+f%28x%29%7D%5E%7B-1%7D+%5Cnabla+f%28x%29+) 确定 ![w](https://www.zhihu.com/equation?tex=w)
4. 求解 ![\nabla^2 f(x) \Delta x_{nt} = - A^Tw - \nabla f(x)](https://www.zhihu.com/equation?tex=%5Cnabla%5E2+f%28x%29+%5CDelta+x_%7Bnt%7D+%3D+-+A%5ETw+-+%5Cnabla+f%28x%29) 确定 ![\Delta x_{nt}](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D)

采用合适的矩阵分解方法(如Cholesky因式分解)，整体的浮点计算次数大概为：

![f +ps + p^2n + (1/3)p^3 \\](https://www.zhihu.com/equation?tex=f+%2Bps+%2B+p%5E2n+%2B+%281%2F3%29p%5E3+%5C%5C)

其中 ![f](https://www.zhihu.com/equation?tex=f) 是对 ![{\nabla^2 f(x)}](https://www.zhihu.com/equation?tex=%7B%5Cnabla%5E2+f%28x%29%7D) 进行Cholesky因式分解所需要的计算量。

## 总结

对于带等式约束的凸优化问题，我们将目标函数进行了二次近似，根据KKT条件，确定了最优解的存在条件——KKT方程。然后通过求解KKT方程确定Newton Method需要的下降方向 ![\Delta x_{nt}](https://www.zhihu.com/equation?tex=%5CDelta+x_%7Bnt%7D) ，并且对快速求解KKT方程做了一定的分析。

阅读本系列的其他文章
- [凸优化系列(一)](https://zhuanlan.zhihu.com/p/50283897)
- [凸优化问题介绍(二)](https://zhuanlan.zhihu.com/p/50290361)
- [无约束凸优化问题求解——Gradient Descent(三)](https://zhuanlan.zhihu.com/p/50029469)
- [无约束凸优化问题求解——Newton Method(四)](https://zhuanlan.zhihu.com/p/50348622)
- [带约束凸优化问题最优解的存在条件——KKT条件(五)](https://zhuanlan.zhihu.com/p/50230049)
- 带等式约束凸优化问题的求解——Newton Method(六)
- [带不等式约束凸优化问题的求解——内点法(七)](https://zhuanlan.zhihu.com/p/50443123)



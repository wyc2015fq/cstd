# 无约束凸优化问题求解——Newton Method(四) - 知乎
# 

## 简介

在上一篇文章([无约束优化问题求解——Gradient Descent](https://zhuanlan.zhihu.com/p/50029469))中我们介绍了梯度下降法求解无约束的凸优化问题。下面介绍牛顿法Newton Method，在这篇文章里假设目标函数 ![f_0(x)](https://www.zhihu.com/equation?tex=f_0%28x%29) 是凸函数且二阶可导。

> 重复进行：
1. 确定下降方向： ![d_k = - {\nabla^2f(x_k)}^{-1} \nabla f(x_k)](https://www.zhihu.com/equation?tex=d_k+%3D+-+%7B%5Cnabla%5E2f%28x_k%29%7D%5E%7B-1%7D+%5Cnabla+f%28x_k%29)
2. 计算Newton减量： ![\lambda_k^2 = \nabla f(x_k)^T {\nabla^2 f(x_k)}^{-1} \nabla f(x_k)](https://www.zhihu.com/equation?tex=%5Clambda_k%5E2+%3D+%5Cnabla+f%28x_k%29%5ET+%7B%5Cnabla%5E2+f%28x_k%29%7D%5E%7B-1%7D+%5Cnabla+f%28x_k%29)
3. 停止准则：如果 ![\lambda_k^2/2 \leq \epsilon ](https://www.zhihu.com/equation?tex=%5Clambda_k%5E2%2F2+%5Cleq+%5Cepsilon+) ，退出
4. 直线搜索：通过[回溯直线法](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Backtracking_line_search)确定步长 ![\alpha_k](https://www.zhihu.com/equation?tex=%5Calpha_k)
5. 改进： ![x_{k+1} = x_k + \alpha_k d_k](https://www.zhihu.com/equation?tex=x_%7Bk%2B1%7D+%3D+x_k+%2B+%5Calpha_k+d_k)

## 直观理解

首先，我们从**二阶近似**的角度理解。

函数 ![f](https://www.zhihu.com/equation?tex=f) 在 ![x](https://www.zhihu.com/equation?tex=x) 处的二阶Taylor近似 ![\hat f](https://www.zhihu.com/equation?tex=%5Chat+f) 为：

![f(x+v) \approx \hat f(x+v)  = f(x) + \nabla f(x)^T + \frac{1}{2}v^T \nabla^2 f(x) v \tag{1}  ](https://www.zhihu.com/equation?tex=f%28x%2Bv%29+%5Capprox+%5Chat+f%28x%2Bv%29++%3D+f%28x%29+%2B+%5Cnabla+f%28x%29%5ET+%2B+%5Cfrac%7B1%7D%7B2%7Dv%5ET+%5Cnabla%5E2+f%28x%29+v+%5Ctag%7B1%7D++)

这是关于 ![v](https://www.zhihu.com/equation?tex=v) 的二次凸函数，在 ![v = {\nabla^2 f(x)}^{-1} f(x)](https://www.zhihu.com/equation?tex=v+%3D+%7B%5Cnabla%5E2+f%28x%29%7D%5E%7B-1%7D+f%28x%29) 处达到最小值。因此 ![{\nabla^2 f(x_k)}^{-1} f(x_k)](https://www.zhihu.com/equation?tex=%7B%5Cnabla%5E2+f%28x_k%29%7D%5E%7B-1%7D+f%28x_k%29) 是 ![f(x_{k+1})](https://www.zhihu.com/equation?tex=f%28x_%7Bk%2B1%7D%29) 在 ![x_k](https://www.zhihu.com/equation?tex=x_k) 处的二阶近似最优解。如果函数 ![f](https://www.zhihu.com/equation?tex=f) 是二次的，显然该近似是精确最优解。

然后，我们从**线性化最优性条件**的角度理解。

我们知道，求解凸函数的最优解等价于求解 ![\nabla f(x)=0](https://www.zhihu.com/equation?tex=%5Cnabla+f%28x%29%3D0) 。我们对 ![\nabla f(x)](https://www.zhihu.com/equation?tex=%5Cnabla+f%28x%29) 做线性近似。

![\nabla f(x+v) \approx \nabla f(x) + \nabla^2 f(x) v = 0 \tag{2}](https://www.zhihu.com/equation?tex=%5Cnabla+f%28x%2Bv%29+%5Capprox+%5Cnabla+f%28x%29+%2B+%5Cnabla%5E2+f%28x%29+v+%3D+0+%5Ctag%7B2%7D)

求解上式，得到解为 ![v = {\nabla^2 f(x)}^{-1} f(x)](https://www.zhihu.com/equation?tex=v+%3D+%7B%5Cnabla%5E2+f%28x%29%7D%5E%7B-1%7D+f%28x%29) 。

## 收敛性分析

在这里，我们不再给出详细证明细节，而是给出一些重要的引理来说明Newton Method的收敛过程。

(**阻尼Newton阶段**)存在 ![\gamma > 0](https://www.zhihu.com/equation?tex=%5Cgamma+%3E+0) 和 ![0 < \eta \leq m^2/L](https://www.zhihu.com/equation?tex=0+%3C+%5Ceta+%5Cleq+m%5E2%2FL) 使得，如果 ![||\nabla f(x_k)|| \ge \eta ](https://www.zhihu.com/equation?tex=%7C%7C%5Cnabla+f%28x_k%29%7C%7C+%5Cge+%5Ceta+) ，则

![f(x_{k+1}) - f(x_k) \leq - \eta \tag{3}](https://www.zhihu.com/equation?tex=f%28x_%7Bk%2B1%7D%29+-+f%28x_k%29+%5Cleq+-+%5Ceta+%5Ctag%7B3%7D)

(**纯Newton阶段**)存在 ![\gamma > 0](https://www.zhihu.com/equation?tex=%5Cgamma+%3E+0) 和 ![0 < \eta \leq m^2/L](https://www.zhihu.com/equation?tex=0+%3C+%5Ceta+%5Cleq+m%5E2%2FL) 使得，如果 ![||\nabla f(x_k)|| < \eta ](https://www.zhihu.com/equation?tex=%7C%7C%5Cnabla+f%28x_k%29%7C%7C+%3C+%5Ceta+) ，则

![\frac{L}{2m^2} || \nabla f(x_{k+1})|| \leq \Big(\frac{L}{2m^2} ||\nabla f(x_k)|| \Big)^2 \tag{4}](https://www.zhihu.com/equation?tex=%5Cfrac%7BL%7D%7B2m%5E2%7D+%7C%7C+%5Cnabla+f%28x_%7Bk%2B1%7D%29%7C%7C+%5Cleq+%5CBig%28%5Cfrac%7BL%7D%7B2m%5E2%7D+%7C%7C%5Cnabla+f%28x_k%29%7C%7C+%5CBig%29%5E2+%5Ctag%7B4%7D)

## 纯Newton阶段

首先对纯Newton阶段进行分析。假设在某个迭代次数为 ![k](https://www.zhihu.com/equation?tex=k) 时，不等式(4)成立。根据前提条件 ![||\nabla f(x_k)|| < \eta ](https://www.zhihu.com/equation?tex=%7C%7C%5Cnabla+f%28x_k%29%7C%7C+%3C+%5Ceta+)和 ![\eta \leq m^2/L](https://www.zhihu.com/equation?tex=%5Ceta+%5Cleq+m%5E2%2FL) ，我们有：

![\begin{align} ||\nabla f(x_{k+1}) || &\leq \frac{L}{2m^2} ||\nabla f(x_k)||^2 \\ & < \frac{L}{2m^2} \eta^2 \\ & < \frac{1}{2\eta} \eta^2 \\ & < \eta \end{align} \tag{5}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%7C%7C%5Cnabla+f%28x_%7Bk%2B1%7D%29+%7C%7C+%26%5Cleq+%5Cfrac%7BL%7D%7B2m%5E2%7D+%7C%7C%5Cnabla+f%28x_k%29%7C%7C%5E2+%5C%5C+%26+%3C+%5Cfrac%7BL%7D%7B2m%5E2%7D+%5Ceta%5E2+%5C%5C+%26+%3C+%5Cfrac%7B1%7D%7B2%5Ceta%7D+%5Ceta%5E2+%5C%5C+%26+%3C+%5Ceta+%5Cend%7Balign%7D+%5Ctag%7B5%7D)

即在纯Newton阶段，对于大于 ![k](https://www.zhihu.com/equation?tex=k) 的迭代次数 ![l](https://www.zhihu.com/equation?tex=l) ，我们又 ![||f(x_l)|| < \eta](https://www.zhihu.com/equation?tex=%7C%7Cf%28x_l%29%7C%7C+%3C+%5Ceta) ，此时采用回溯直线确定的步长 ![t=1](https://www.zhihu.com/equation?tex=t%3D1) ，并且

![\frac{L}{2m^2} || \nabla f(x_{l+1})|| \leq \Big(\frac{L}{2m^2} ||\nabla f(x_l)|| \Big)^2  \\](https://www.zhihu.com/equation?tex=%5Cfrac%7BL%7D%7B2m%5E2%7D+%7C%7C+%5Cnabla+f%28x_%7Bl%2B1%7D%29%7C%7C+%5Cleq+%5CBig%28%5Cfrac%7BL%7D%7B2m%5E2%7D+%7C%7C%5Cnabla+f%28x_l%29%7C%7C+%5CBig%29%5E2++%5C%5C)

重复应用这个不等式，我们发现对于任意的 ![l \ge k](https://www.zhihu.com/equation?tex=l+%5Cge+k) ，

![\frac{L}{2m^2} || \nabla f(x_l)|| \leq \Big(\frac{L}{2m} ||\nabla f(x_k)|| \Big)^{2^{l-k}} \leq  \Big(\frac{1}{2}\Big)^{2^{l-k}} \\](https://www.zhihu.com/equation?tex=%5Cfrac%7BL%7D%7B2m%5E2%7D+%7C%7C+%5Cnabla+f%28x_l%29%7C%7C+%5Cleq+%5CBig%28%5Cfrac%7BL%7D%7B2m%7D+%7C%7C%5Cnabla+f%28x_k%29%7C%7C+%5CBig%29%5E%7B2%5E%7Bl-k%7D%7D+%5Cleq++%5CBig%28%5Cfrac%7B1%7D%7B2%7D%5CBig%29%5E%7B2%5E%7Bl-k%7D%7D+%5C%5C)

应用[无约束优化问题求解——Gradient Descent](https://zhuanlan.zhihu.com/p/50029469)中的定界结论

![ f(x^*) \geq f(x) - \frac{1}{2m}||\nabla f(x)||^2 \tag{6}](https://www.zhihu.com/equation?tex=+f%28x%5E%2A%29+%5Cgeq+f%28x%29+-+%5Cfrac%7B1%7D%7B2m%7D%7C%7C%5Cnabla+f%28x%29%7C%7C%5E2+%5Ctag%7B6%7D)

所以，

![f(x_l) - f(x^*) \leq \frac{1}{2m}||\nabla f(x)||^2 \leq \frac{2m^3}{L^2} \Big(\frac{1}{2} \Big)^{2^{l-k+1}} \tag{7}](https://www.zhihu.com/equation?tex=f%28x_l%29+-+f%28x%5E%2A%29+%5Cleq+%5Cfrac%7B1%7D%7B2m%7D%7C%7C%5Cnabla+f%28x%29%7C%7C%5E2+%5Cleq+%5Cfrac%7B2m%5E3%7D%7BL%5E2%7D+%5CBig%28%5Cfrac%7B1%7D%7B2%7D+%5CBig%29%5E%7B2%5E%7Bl-k%2B1%7D%7D+%5Ctag%7B7%7D)

上式表明一旦算法进入纯Newton阶段， ![l \to \infty](https://www.zhihu.com/equation?tex=l+%5Cto+%5Cinfty) 时， ![f(x_l) - f(p^*) \to 0](https://www.zhihu.com/equation?tex=f%28x_l%29+-+f%28p%5E%2A%29+%5Cto+0) 。此外给定精度 ![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon) ，我们可以得到收敛次数 ![n](https://www.zhihu.com/equation?tex=n) 为

![n = \log_2 \log_2 (\epsilon_0 / \epsilon), \quad \epsilon_0 =2m^3/L^2 \\](https://www.zhihu.com/equation?tex=n+%3D+%5Clog_2+%5Clog_2+%28%5Cepsilon_0+%2F+%5Cepsilon%29%2C+%5Cquad+%5Cepsilon_0+%3D2m%5E3%2FL%5E2+%5C%5C)

其表明在纯Newton阶段，算法将收敛非常快(远大于Gradient Descent的收敛速度)，对于任意凸函数，我们可以假定纯Newton阶段需要的收敛次数为5或6。
![](https://pic4.zhimg.com/v2-b35d44b772264e426ab220f0542b69f7_b.jpg)左图：Gradient Descent；右图：Newton Method。Newton Method的收敛次数要小于Gradient Descent，但收敛时间未必。
## 阻尼Newton阶段

由于每次迭代，函数值至少减少 ![\gamma](https://www.zhihu.com/equation?tex=%5Cgamma) ，所以阻尼牛顿阶段的迭代次数不会超过

![\frac{f(x_0) - f(x^*)}{\gamma} \\](https://www.zhihu.com/equation?tex=%5Cfrac%7Bf%28x_0%29+-+f%28x%5E%2A%29%7D%7B%5Cgamma%7D+%5C%5C)

否则 ![f](https://www.zhihu.com/equation?tex=f) 将小于 ![f(x^*)](https://www.zhihu.com/equation?tex=f%28x%5E%2A%29) ，而这是不可能的。

结合纯牛顿阶段分析，我们可以得到Newton Method的总的收敛次数为：

![\frac{f(x_0) - f(x^*)}{\gamma} + 5 \tag{8}](https://www.zhihu.com/equation?tex=%5Cfrac%7Bf%28x_0%29+-+f%28x%5E%2A%29%7D%7B%5Cgamma%7D+%2B+5+%5Ctag%7B8%7D)

## 总结

在这篇文章里，我们介绍了Newton Method，证明了**Newton Method是一个收敛非常快的方法**。那是不是Newton Method的收敛时间就要小于Gradient Descent呢？No！因此**Newton Method每一步要计算 ![{\nabla^2 f(x)}^{-1}](https://www.zhihu.com/equation?tex=%7B%5Cnabla%5E2+f%28x%29%7D%5E%7B-1%7D) ，这显然是一个非常耗时的操作！**

阅读本系列的其他文章
- [凸优化系列(一)](https://zhuanlan.zhihu.com/p/50283897)
- [凸优化问题介绍(二)](https://zhuanlan.zhihu.com/p/50290361)
- [无约束凸优化问题求解——Gradient Descent(三)](https://zhuanlan.zhihu.com/p/50029469)
- 无约束凸优化问题求解——Newton Method(四)
- [带约束凸优化问题最优解的存在条件——KKT条件(五)](https://zhuanlan.zhihu.com/p/50230049)
- [带等式约束凸优化问题的求解——Newton Method(六)](https://zhuanlan.zhihu.com/p/50411305)
- [带不等式约束凸优化问题的求解——内点法(七)](https://zhuanlan.zhihu.com/p/50443123)




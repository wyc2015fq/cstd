# SVM笔记（二） 拉格朗日对偶性 - xmdxcsj的专栏 - CSDN博客





2016年05月26日 22:43:26[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2590








## 用途

对于约束最优化问题，有时原始问题的最优解不好求解，可以借助拉格朗日对偶性将原始问题转化为对偶问题，通过求解对偶问题的解来获得原始问题的最优解。在最大熵模型和支持向量机中用到了该类方法。

## 描述

### 原始问题

假设$f(x)$,$c_I(x)$,$h_j(x)$为定义在$R^{n}$上面的连续可微函数，原始问题如下： 


$\min_{x\in R^n}f(x)$


$s.t. c_i(x)\le0,i=1,2...k$


$h_j(x)=0,j=1,2...l$

引入广义拉格朗日函数： 


$L(x,\alpha,\beta)=f(x)+\sum_{i=1}^{k}\alpha_ic_i(x)+\sum_{j=1}^{l}\beta_jh_j(x)$


$s.t. \alpha_i\ge0,i=1,2...k$

可以将求解原始问题转化为求解以下极小极大问题： 


$\min_{x}\max_{\alpha,\beta:\alpha_i\ge0}L(x,\alpha,\beta)$

原始问题和极小极大问题是等价的，证明见参考。
### 对偶问题（极大极小问题）

广义拉格朗日函数的极大极小问题可以表示成以下形式： 


$\max_{\alpha,\beta}\min_{x}L(x,\alpha,\beta)$


$s.t. \alpha_i\ge0$
### 两种问题的关系

如果原始问题和对偶问题都有最优值，那么满足 


$\max_{\alpha,\beta:\alpha_i\ge0}\min_{x}L(x,\alpha,\beta)\le\min_{x}\max_{\alpha,\beta:\alpha_i\ge0}L(x,\alpha,\beta)$

对于求解来讲，我们更需要关注的是上式中的等号情况，即原始问题和对偶问题的最优值相等的情况，此时可以通过求解对偶问题的最优解来作为原始问题的最优解。 

假设$f(x)$和$c_i(x)$是凸函数，$h_j(x)$是仿射函数，并且不等式约束$c_i(x)$是严格可行的，则$x^*$,$\alpha^*$,$\beta^*$分别是原始问题和对偶问题的解的充分必要条件是满足下面的KKT（Karush-Kuhn-Tucker）条件： 


$\nabla_xL(x^*,\alpha^*,\beta^*)=0$


$\nabla_\alpha L(x^*,\alpha^*,\beta^*)=0$


$\nabla_\beta L(x^*,\alpha^*,\beta^*)=0$


$\alpha_i^*c_i(x^*)=0, i=1,2...k$


$c_i(x^*)\le0,i=1,2...k$


$\alpha_i^*\ge0,i=1,2...k$


$h_j(x^*)=0,j=1,2...l$






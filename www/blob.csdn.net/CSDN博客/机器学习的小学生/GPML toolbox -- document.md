
# GPML toolbox -- document - 机器学习的小学生 - CSDN博客


2017年06月05日 10:04:48[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：1015


高斯过程(GPs)可以很方便的用于贝叶斯监督学习，例如回归 和分类。GP过程推断的最简单的形式可以通过几行代码实现。然而，在实际应用中，为了有代表性，事情会更复杂些：你可能想用更复杂的协方差和均值函数，学习好的超参数，使用非高斯似然函数（使用精确的推断是不可行的），使用近似算法，或者上面的许多或者所有的（如多个均值函数的组合））组合。这是GPMP工具包所做的事情。
第3节的其余部分首先在下一小节中介绍软件的一些基本组成部分。这可以理解第3b节描述的gp函数的用法。一个实际的例子如3c)所示，本节将在3d)中给出更详细的概述。
gp函数存在两种模式：训练或者预测。如果没有测试输入，那么函数计算：负log 边缘似然和其关于超参数的偏导数，这个模式用来调节超参数。如果给出了测试输入，那么函数返回测试集预测概率。使用方法：
```python
training:
```
```python
[nlZ dnlZ          ]
```
```python
= gp(hyp,
```
```python
inf
```
```python
, mean, cov, lik, x, y);
 prediction:
```
```python
[ymu ys2 fmu fs2   ]
```
```python
= gp(hyp,
```
```python
inf
```
```python
, mean, cov, lik, x, y, xs);
         or:
```
```python
[ymu ys2 fmu fs2 lp]
```
```python
= gp(hyp,
```
```python
inf
```
```python
, mean, cov, lik, x, y, xs, ys);
```
4b) 基于诱导输入的稀疏近似回归
(Sparse Approximate Regression based on inducing inputs)。
当训练输入$x$的数量超过数百时，精确的推导花费很差时间。我们提供了稀疏近似方法来处理这些情况。基本思想是使用诱导点，然后仅基于训练，测试和诱导点之间的交叉协方差上的计算。快速的浏览所有可能的选项请参考`demoSparse`。
使用稀疏近似方法很简单，我们仅需要将协方差函数`covfunc`包装进`apxSparse.m`中。如下面几行代码阐述的形式，调用带有推断方法`infGaussLik.m`的`gp.m`。
```python
nu =
```
```python
fix
```
```python
(n/
```
```python
2
```
```python
); u =
```
```python
linspace
```
```python
(-
```
```python
1.3
```
```python
,
```
```python
1.3
```
```python
,nu)
```
```python
';
 covfuncF = {@apxSparse, {covfunc}, u};
 inf = @(varargin) infGaussLik(varargin{:}, struct('
```
```python
s'
```
```python
,
```
```python
0.0
```
```python
));
```
```python
[mF s2F]
```
```python
= gp(hyp,
```
```python
inf
```
```python
, meanfunc, covfuncF, likfunc, x, y, z);
```
在这个代码中，稀疏近似协方差函数是：`apxSparse`函数,一个协方差函数和一组诱导点组成。而且推断方法`inf`定义为`infGaussLik`推断方法追加上`struct('s',0.0)`。根据`s`值的不同选择不同的近似方法。`s=1`: 全独立训练条件(FITC)，`s=0`: 变自由能量(VFE)，`0<s<1`: 稀疏平方期望传播(SPEP）。
我们定义了等距离的诱导点，在图中用黑色圈显示。注意到，预测的方差在诱导点不支持的外侧过估计了。在多元例子中，稠密的采样诱导点是不可行的，可以简单地使用训练点中的随机子集。
```python
nu =
```
```python
fix
```
```python
(n/
```
```python
2
```
```python
); iu = randperm(n); iu = iu(
```
```python
1
```
```python
:nu); u = x(iu,:);
```
也可以将诱导输入作为超参数的一部分，使用字段`hyp.xu`:
```python
hyp.xu
```
```python
=
```
```python
u;
```
在这种情况下，超参数的最优化也会优化诱导输入(inducing inputs)(注意，到诱导输入作为超参数的一个字段给出时，那么其会覆盖通过`apxSparse`函数指定的有道输入。


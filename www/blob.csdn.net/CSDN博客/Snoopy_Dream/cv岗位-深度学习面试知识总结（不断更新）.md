# cv岗位 - 深度学习面试知识总结（不断更新） - Snoopy_Dream - CSDN博客





2019年04月15日 16:35:29[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：27








github上面有个深度学习500问，大家可以看看那个，我这个作为补充，而且相对说明的更详细一些。

**目录**

[1. 常用的一阶、二阶优化算法有哪些？区别是什么？](#%E5%B8%B8%E7%94%A8%E7%9A%84%E4%B8%80%E9%98%B6%E3%80%81%E4%BA%8C%E9%98%B6%E4%BC%98%E5%8C%96%E7%AE%97%E6%B3%95%E6%9C%89%E5%93%AA%E4%BA%9B%EF%BC%9F%E5%8C%BA%E5%88%AB%E6%98%AF%E4%BB%80%E4%B9%88%EF%BC%9F)

[2. 为什么二阶优化算法更快呢？](#%E4%B8%BA%E4%BB%80%E4%B9%88%E4%BA%8C%E9%98%B6%E4%BC%98%E5%8C%96%E7%AE%97%E6%B3%95%E6%9B%B4%E5%BF%AB%E5%91%A2%EF%BC%9F)

[3. 神经网络为啥不用拟牛顿法而是用梯度下降？（为什么深度学习不用二阶的优化算法?）](#%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C%E4%B8%BA%E5%95%A5%E4%B8%8D%E7%94%A8%E6%8B%9F%E7%89%9B%E9%A1%BF%E6%B3%95%E8%80%8C%E6%98%AF%E7%94%A8%E6%A2%AF%E5%BA%A6%E4%B8%8B%E9%99%8D%EF%BC%9F%EF%BC%88%E4%B8%BA%E4%BB%80%E4%B9%88%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0%E4%B8%8D%E7%94%A8%E4%BA%8C%E9%98%B6%E7%9A%84%E4%BC%98%E5%8C%96%E7%AE%97%E6%B3%95%3F%EF%BC%89)

### 1. 常用的一阶、二阶优化算法有哪些？区别是什么？

一阶的有：梯度下降法GD、SGD、ASGD、指数加权平均AdaDelta、RMSProp、Adam

二阶的有：牛顿法、拟牛顿法（二阶收敛更快）[https://www.matongxue.com/madocs/205.html](https://www.matongxue.com/madocs/205.html)牛顿法好材料

**基础概念：**
- 梯度（一阶导数）考虑一座在 (x1, x2) 点高度是 f(x1, x2) 的山。那么，某一点的梯度方向是在该点坡度最陡的方向，而梯度的大小告诉我们坡度到底有多陡。
- Hesse 矩阵（二阶导数，二阶梯度）
- 牛顿法是一种在实数域和复数域上近似求解方程的方法。方法使用函数f (x)的泰勒级数的前面几项来寻找方程f (x) = 0的根。牛顿法**最大的特点**就在于它**的收敛速度很快。**主要有**两个缺陷**，一个是需要计算**Hessian矩阵**，需要O(np^2) 的复杂度，另外一个便是计算**Hessian的逆矩阵**需要O(p^3 )的复杂度。
- 拟牛顿法：本质思想是改善牛顿法每次需要求解复杂的Hessian矩阵的逆矩阵的缺陷，它**使用正定矩阵来近似Hessian矩阵**的逆，从而**简化了运算的复杂度**。

### 2. 为什么二阶优化算法更快呢？

更通俗地说的话，比如你想找一条最短的路径走到一个盆地的最底部。梯度下降法每次只从你当前所处位置选一个坡度最大的方向走一步，牛顿法在选择方向时，不仅会考虑坡度是否够大，还会考虑你走了一步之后，坡度是否会变得更大。所以，可以说牛顿法比梯度下降法看得更远一点，能更快地走到最底部。（牛顿法目光更加长远，所以少走弯路；相对而言，梯度下降法只考虑了局部的最优，没有全局思想。）

　　根据wiki上的解释，从几何上说，牛顿法就是用一个二次曲面去拟合你当前所处位置的局部曲面，而梯度下降法是用一个平面去拟合当前的局部曲面，通常情况下，二次曲面的拟合会比平面更好，所以**牛顿法选择的下降路径会更符合真实的最优下降路径。**

![](http://images0.cnblogs.com/blog2015/764050/201508/222309373784741.png)可以看到梯度下降是局部最优，只追求当下梯度最大。而牛顿法，则看的更远。

注：红色的牛顿法的迭代路径，绿色的是梯度下降法的迭代路径。

[https://blog.csdn.net/owen7500/article/details/51601627](https://blog.csdn.net/owen7500/article/details/51601627)

### 3. 神经网络为啥不用拟牛顿法而是用梯度下降？（为什么深度学习不用二阶的优化算法?）

二阶虽然精度高，但是计算量太大，而且精度高对于深度学习这种深层模型反而不一定有好处，因为破坏了泛化性能。

======================================================

首先，深度学习里面使用二阶优化算法的其实也是有的，不多而已，列举几篇

1. Hinton的学生 James Martens，主要的工作就是 Second-order Optimization for Neural Networks如Hessian-free optimization，这里有他的文章和学位论文[James Martens: Research](https://link.zhihu.com/?target=http%3A//www.cs.toronto.edu/~jmartens/research.html)

2. [Optimization Methods for Large-Scale Machine Learning](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1606.04838) 和一个新近提出的方法[https://papers.nips.cc/paper/6145-a-multi-batch-l-bfgs-method-for-machine-learning.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1605.06049)

======================================================

首先分析神经网络的特点：大数据（样本点多），高参数，非凸

**牛顿和拟牛顿法适用条件：**在**凸优化情形下**，如果**迭代点离全局最优很近时，收敛速率快于gd。**

反向传播主要是依靠一阶梯度。二阶梯度在理论和实际上都是可以应用都网络中的，但相比于一阶梯度，二阶优化问题：

（1）计算量大，训练非常慢。

（2）二阶方法能够更快地求得更高精度的解，这在浅层模型是有益的。而在神经网络这类深层模型中对参数的精度要求不高，甚至不高的精度对模型还有益处，能够提高模型的泛化能力。

（3）稳定性。越简单的模型越robust，梯度下降一般还行。

======================================================

【可不看】具体理解高参数：以Newton法和quasi-Newton 法为代表的二阶优化方法最大的问题就是计算复杂度。以BFGS来说，一次迭代更新的复杂度是![O(n^2)](https://www.zhihu.com/equation?tex=O%28n%5E2%29)，这在高维的时候是不可行的。L-BFGS使用部分梯度（最近迭代的m个梯度）来近似Hessian，复杂度是O(mn)，实际中m的取值可以是5-40. 这大大扩展了BFGS的可用范围。具体实施中，BFGS可以应用到![10^3-10^4](https://www.zhihu.com/equation?tex=10%5E3-10%5E4)量级，L-BFGS可以延伸到![10^5](https://www.zhihu.com/equation?tex=10%5E5)量级。但这距离深度网络中变量的个数仍有较大差距，如AlexNet有![6\times 10^7](https://www.zhihu.com/equation?tex=6%5Ctimes+10%5E7)个参数。

======================================================











[https://www.zhihu.com/question/53218358](https://www.zhihu.com/question/53218358)

[https://www.zhihu.com/question/46441403/answer/117594441](https://www.zhihu.com/question/46441403/answer/117594441)

[https://www.zhihu.com/question/53218358](https://www.zhihu.com/question/53218358)








# 最大熵模型（MaxEnt）：万法归宗（下） - 白马负金羁 - CSDN博客





2018年01月07日 14:40:08[白马负金羁](https://me.csdn.net/baimafujinji)阅读数：1559








在上一篇文章【1】中，我们已经得到了与最大熵模型之学习等价的带约束的最优化问题：


![](https://img-blog.csdn.net/20180106150940758)


注意上述公式中还隐含一个不等式约束即 P(y|x)≥0。求解这个带约束的最优化问题，所得之解即为最大熵模型学习的解。本文就来完成这个推导。





现在这里需要使用拉格朗日乘数法，并将带约束的最优化之原始问题转换为无约束的最优化之对偶问题，并通过求解对偶问题来求解原始问题。首先，引入拉格朗日乘子![](https://img-blog.csdn.net/20180107044600137)，并定义拉格朗日函数L(P, **w**)：


![](https://img-blog.csdn.net/20180107045857130)


According to [7], to find the solution to the optimization problem, we appealed to the Kuhn-Tucker theorem, which states that we can (1) first solve L(P, **w**) for P to get a parametric form for P* in terms of **w**; (2) then plug P* back in to L(P,** w**), this time solving for **w***.





**原始问题与对偶问题**





最优化的原始问题是


![](https://img-blog.csdn.net/20180107044614242)


通过交换极大和极小的位置，可以得到如下这个对偶问题


![](https://img-blog.csdn.net/20180107044619829)


由于拉格朗日函数L(P,**w**)是P的凸函数，原始问题与对偶问题的解是等价的。这样便可以通过求解对偶问题来求解原始问题。





对偶问题内层的极小问题![](https://img-blog.csdn.net/20180107060823119) 是关于参数**w**的函数，将其记为


![](https://img-blog.csdn.net/20180107060838262)


同时将其解记为


![](https://img-blog.csdn.net/20180107060847876)


接下来，根据费马定理，求L(P,**w**)对P(y|x)的偏导数


![](https://img-blog.csdn.net/20180107112453158)


注意上述推导中运用了下面这个事实


![](https://img-blog.csdn.net/20180107061159631)


进一步地，令

![](https://img-blog.csdn.net/20180107062038748)


又因为![](https://img-blog.csdn.net/20180107062159680)，于是有


![](https://img-blog.csdn.net/20180107062418096)


进而有

![](https://img-blog.csdn.net/20180107062838934)


又因为

![](https://img-blog.csdn.net/20180107062755938)


所以可得


![](https://img-blog.csdn.net/20180107062931966)


即

![](https://img-blog.csdn.net/20180107063129701)

将上面的式子带回前面P(y|x)的表达式，则得到

![](https://img-blog.csdn.net/20180107112820276)


其中，


![](https://img-blog.csdn.net/20180107112957649)


Z**w**(x)称为规范化因子；* f*i(x,y)是特征函数；wi是特征的权值。由上述两式所表示的模型P**w**=P**w**(y|x)就是最大熵模型。这里，**w**是最大熵模型中的参数向量。注意到，我们之前曾经提过，特征函数可以是任意实值函数，如果*f*i(x,y)=xi，那么这其实也就是【5】中所说的多元逻辑回归模型，即


![](https://img-blog.csdn.net/20180107114305756)

此亦是万法归宗的第一层境界。关于上面这个式子的一个简单例子，你还可参考文献【6】。




**极大似然估计**





下面，需要求解对偶问题中外部的极大化问题


![](https://img-blog.csdn.net/20180107121025841)


将其解记为**w***，即

![](https://img-blog.csdn.net/20180107121104277)


这就是说，可以应用最优化算法求对偶函数![](https://img-blog.csdn.net/20180107122119053)的极大化，得到**w***，用来表示![](https://img-blog.csdn.net/20180107122132157)。这里，![](https://img-blog.csdn.net/20180107122124889)是学习到的最优模型（最大熵模型）。于是，最大熵模型的学习算法现在就归结为对偶函数![](https://img-blog.csdn.net/20180107122119053)的极大化问题上来。





前面我们已经给出了![](https://img-blog.csdn.net/20180107122119053)的表达式：

![](https://img-blog.csdn.net/20180107132127215)


由于，其中

![](https://img-blog.csdn.net/20180107132103198)


于是将P**w**(y|x)带入![](https://img-blog.csdn.net/20180107122119053)，可得

![](https://img-blog.csdn.net/20180107132251050)


注意其中倒数第4行至倒数第3行运用了下面这个推导：


![](https://img-blog.csdn.net/20180107132111516)





下面我们来证明对偶函数的极大化等价于最大熵模型的极大似然估计。已知训练数据的经验概率分布![](https://img-blog.csdn.net/20180107133012069)，条件概率分布P(Y|X)的对数似然函数表示为


![](https://img-blog.csdn.net/20180107132957522)


当条件概率分布P(y|x)是最大熵模型时时，对数似然函数为


![](https://img-blog.csdn.net/20180107134200521)


对比之后，不难发现


![](https://img-blog.csdn.net/20180107134301811)


既然对偶函数![](https://img-blog.csdn.net/20180107122119053)等价于对数似然函数，于是也就证明了最大熵模型学习中的对偶函数极大化等价于最大熵模型的极大似然估计这一事实。此亦是万法归宗的第二层境界。由此，最大熵模型的学习问题就转换为具体求解“对数似然函数极大化的问题”或者“对偶函数极大化的问题”。








**参考文献与推荐阅读材料**


【1】[最大熵模型（MaxEnt）：万法归宗（上）](http://blog.csdn.net/baimafujinji/article/details/78986906)

【2】李航，统计学习方法，清华大学出版社

【3】https://www.cnblogs.com/wxquare/p/5858008.html

【4】http://blog.csdn.net/itplus/article/details/26550273


【5】http://blog.csdn.net/baimafujinji/article/details/51703322


【6】https://www.youtube.com/watch?v=hSXFuypLukA&list=PLJV_el3uVTsPy9oCRY30oBPNLCo89yu49&index=10

【7】http://www.cs.cmu.edu/afs/cs/user/aberger/www/html/tutorial/node9.html





**（本文完）**




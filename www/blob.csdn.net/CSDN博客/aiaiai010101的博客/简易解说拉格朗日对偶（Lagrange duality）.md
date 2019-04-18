# 简易解说拉格朗日对偶（Lagrange duality） - aiaiai010101的博客 - CSDN博客

2017年06月15日 20:01:42[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：99
个人分类：[机器学习](https://blog.csdn.net/aiaiai010101/article/category/6764564)



**引言**：尝试用最简单易懂的描述解释清楚机器学习中会用到的拉格朗日对偶性知识，非科班出身，如有数学专业博友，望多提意见！

**1.原始问题**

假设![](http://images.cnitblog.com/blog/659319/201411/091059037373135.jpg)是定义在![](http://images.cnitblog.com/blog/659319/201411/091057505342233.jpg)上的连续可微函数（为什么要求连续可微呢，后面再说，这里不用多想），考虑约束最优化问题：

![](http://images.cnitblog.com/blog/659319/201411/091101088315585.jpg)

![](http://images.cnitblog.com/blog/659319/201411/091101211599718.jpg)

称为约束最优化问题的原始问题。

现在如果不考虑约束条件，原始问题就是：

![](http://images.cnitblog.com/blog/659319/201411/091101088315585.jpg)

因为假设其连续可微，利用高中的知识，对![](http://images.cnitblog.com/blog/659319/201411/091104477995123.jpg)求导数，然后令导数为0，就可解出最优解，很easy. 那么，问题来了（呵呵。。。），偏偏有约束条件，好烦啊，要是能想办法把约束条件去掉就好了，bingo!
 拉格朗日函数就是干这个的。

引进广义拉格朗日函数（generalized Lagrange function）:

![](http://images.cnitblog.com/blog/659319/201411/091109504407998.jpg)

不要怕这个式子，也不要被拉格朗日这个高大上的名字给唬住了，让我们慢慢剖析！这里![](http://images.cnitblog.com/blog/659319/201411/091111479719525.jpg)，![](http://images.cnitblog.com/blog/659319/201411/091114035815695.jpg)是拉格朗日乘子（名字高大上，其实就是上面函数中的参数而已），特别要求![](http://images.cnitblog.com/blog/659319/201411/091119332372450.jpg).

现在，如果把![](http://images.cnitblog.com/blog/659319/201411/091116383621308.jpg)看作是关于![](http://images.cnitblog.com/blog/659319/201411/091114035815695.jpg)的函数，要求其最大值，即

![](http://images.cnitblog.com/blog/659319/201411/091118462991757.jpg)

再次注意![](http://images.cnitblog.com/blog/659319/201411/091116383621308.jpg)是一个关于![](http://images.cnitblog.com/blog/659319/201411/091114035815695.jpg)的函数，经过我们优化（不要管什么方法），就是确定![](http://images.cnitblog.com/blog/659319/201411/091114035815695.jpg)的值使得![](http://images.cnitblog.com/blog/659319/201411/091116383621308.jpg)取得最大值（此过程中把![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)看做常量），确定了![](http://images.cnitblog.com/blog/659319/201411/091114035815695.jpg)的值，就可以得到![](http://images.cnitblog.com/blog/659319/201411/091116383621308.jpg)的最大值，因为![](http://images.cnitblog.com/blog/659319/201411/091114035815695.jpg)已经确定，显然最大值![](http://images.cnitblog.com/blog/659319/201411/091118462991757.jpg)就是只和![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)有关的函数，定义这个函数为：

![](http://images.cnitblog.com/blog/659319/201411/091131050819820.jpg)

其中 ![](http://images.cnitblog.com/blog/659319/201411/091109504407998.jpg)

下面通过![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)是否满足约束条件两方面来分析这个函数:
- 考虑某个![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)违反了原始的约束，即![](http://images.cnitblog.com/blog/659319/201411/091136188463531.jpg)或者![](http://images.cnitblog.com/blog/659319/201411/091136328784877.jpg)，那么：

![](http://images.cnitblog.com/blog/659319/201411/091139245181308.jpg)

　　注意中间的最大化式子就是确定![](http://images.cnitblog.com/blog/659319/201411/091114035815695.jpg)的之后的结果，若![](http://images.cnitblog.com/blog/659319/201411/091136188463531.jpg)，则令![](http://images.cnitblog.com/blog/659319/201411/091144513938278.jpg)，如果![](http://images.cnitblog.com/blog/659319/201411/091136328784877.jpg)，很容易取值![](http://images.cnitblog.com/blog/659319/201411/091147326122558.jpg)使得![](http://images.cnitblog.com/blog/659319/201411/091147424094365.jpg)

- 考虑![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)满足原始的约束，则：![](http://images.cnitblog.com/blog/659319/201411/091149581596492.jpg)，注意中间的最大化是确定![](http://images.cnitblog.com/blog/659319/201411/091114035815695.jpg)的过程，![](http://images.cnitblog.com/blog/659319/201411/091104477995123.jpg)就是个常量，常量的最大值显然是本身.

通过上面两条分析可以得出：

![](http://images.cnitblog.com/blog/659319/201411/091152447372568.jpg)

那么在满足约束条件下：

![](http://images.cnitblog.com/blog/659319/201411/091156356904512.jpg)

即![](http://images.cnitblog.com/blog/659319/201411/091158200492563.jpg)与原始优化问题等价,所以常用![](http://images.cnitblog.com/blog/659319/201411/091158200492563.jpg)代表原始问题，下标 P 表示原始问题，定义原始问题的最优值：

![](http://images.cnitblog.com/blog/659319/201411/091159247215172.jpg)

原始问题讨论就到这里，做一个总结：通过拉格朗日这位大神的办法重新定义一个无约束问题（大家都喜欢无拘无束），这个无约束问题等价于原来的约束优化问题，从而将约束问题无约束化！

**2.对偶问题**

定义关于![](http://images.cnitblog.com/blog/659319/201411/091245076434161.jpg)的函数：

![](http://images.cnitblog.com/blog/659319/201411/091245503623842.jpg)

注意等式右边是关于![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)的函数的最小化，![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)确定以后，最小值就只与![](http://images.cnitblog.com/blog/659319/201411/091245076434161.jpg)有关，所以是一个关于![](http://images.cnitblog.com/blog/659319/201411/091245076434161.jpg)的函数.

考虑极大化![](http://images.cnitblog.com/blog/659319/201411/091245503623842.jpg)，即

![](http://images.cnitblog.com/blog/659319/201411/091254100188028.jpg)

这就是原始问题的对偶问题，再把原始问题写出来：

![](http://images.cnitblog.com/blog/659319/201411/091257009091387.jpg)

形式上可以看出很对称，只不过原始问题是先固定![](http://images.cnitblog.com/blog/659319/201411/091116383621308.jpg)中的![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)，优化出参数![](http://images.cnitblog.com/blog/659319/201411/091245076434161.jpg)，再优化最优![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)，而对偶问题是先固定![](http://images.cnitblog.com/blog/659319/201411/091245076434161.jpg)，优化出最优![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)，然后再确定参数![](http://images.cnitblog.com/blog/659319/201411/091245076434161.jpg).

定义对偶问题的最优值：

![](http://images.cnitblog.com/blog/659319/201411/091302196745235.jpg)

**3. 原始问题与对偶问题的关系**

> 
**定理**：若原始问题与对偶问题都有最优值，则

![](http://images.cnitblog.com/blog/659319/201411/091308008311010.jpg)

**证明**：对任意的![](http://images.cnitblog.com/blog/659319/201411/091245076434161.jpg)和![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)，有

![](http://images.cnitblog.com/blog/659319/201411/091313234876969.jpg)

即

![](http://images.cnitblog.com/blog/659319/201411/091314177373129.jpg)

由于原始问题与对偶问题都有最优值，所以

![](http://images.cnitblog.com/blog/659319/201411/091315112687376.jpg)

即

![](http://images.cnitblog.com/blog/659319/201411/091308008311010.jpg)

也就是说原始问题的最优值不小于对偶问题的最优值，但是我们要通过对偶问题来求解原始问题，就必须使得原始问题的最优值与对偶问题的最优值相等，于是可以得出下面的推论：

> 
**推论**：设![](http://images.cnitblog.com/blog/659319/201411/091320302684453.jpg)分别是原始问题和对偶问题的可行解，如果![](http://images.cnitblog.com/blog/659319/201411/091323167994543.jpg)，那么![](http://images.cnitblog.com/blog/659319/201411/091320302684453.jpg)分别是原始问题和对偶问题的最优解。

所以，当原始问题和对偶问题的最优值相等：![](http://images.cnitblog.com/blog/659319/201411/091323167994543.jpg)时，可以用求解对偶问题来求解原始问题（当然是对偶问题求解比直接求解原始问题简单的情况下），但是到底满足什么样的条件才能使的![](http://images.cnitblog.com/blog/659319/201411/091323167994543.jpg)呢，这就是下面要阐述的
KKT 条件

**4. KKT 条件**

> 
定理：对于原始问题和对偶问题，假设函数![](http://images.cnitblog.com/blog/659319/201411/091104477995123.jpg)和![](http://images.cnitblog.com/blog/659319/201411/091334486432915.jpg)是凸函数，![](http://images.cnitblog.com/blog/659319/201411/091335120655126.jpg)是仿射函数（即由一阶多项式构成的函数，f(x)=Ax
 + b, A是矩阵，x，b是向量）；并且假设不等式约束![](http://images.cnitblog.com/blog/659319/201411/091334486432915.jpg)是严格可行的，即存在![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)，对所有![](http://images.cnitblog.com/blog/659319/201411/091338414715084.jpg)有![](http://images.cnitblog.com/blog/659319/201411/091339241281253.jpg)，则存在![](http://images.cnitblog.com/blog/659319/201411/091320302684453.jpg)，使得![](http://images.cnitblog.com/blog/659319/201411/091340376595354.jpg)是原始问题的最优解，![](http://images.cnitblog.com/blog/659319/201411/091340575491412.jpg)是对偶问题的最优解，并且

![](http://images.cnitblog.com/blog/659319/201411/091342202997664.jpg)


> 
定理：对于原始问题和对偶问题，假设函数![](http://images.cnitblog.com/blog/659319/201411/091104477995123.jpg)和![](http://images.cnitblog.com/blog/659319/201411/091334486432915.jpg)是凸函数，![](http://images.cnitblog.com/blog/659319/201411/091335120655126.jpg)是仿射函数（即由一阶多项式构成的函数，f(x)=Ax
 + b, A是矩阵，x，b是向量）；并且假设不等式约束![](http://images.cnitblog.com/blog/659319/201411/091334486432915.jpg)是严格可行的，即存在![](http://images.cnitblog.com/blog/659319/201411/091124052686077.jpg)，对所有![](http://images.cnitblog.com/blog/659319/201411/091338414715084.jpg)有![](http://images.cnitblog.com/blog/659319/201411/091339241281253.jpg)，则![](http://images.cnitblog.com/blog/659319/201411/091320302684453.jpg)分别是原始问题和对偶问题的最优解的充分必要条件是![](http://images.cnitblog.com/blog/659319/201411/091320302684453.jpg)满足下面的Karush-Kuhn-Tucker(KKT)条件：

![](http://images.cnitblog.com/blog/659319/201411/091353134402194.jpg)

关于KKT 条件的理解：前面三个条件是由解析函数的知识，对于各个变量的偏导数为0（这就解释了一开始为什么假设三个函数连续可微，如果不连续可微的话，这里的偏导数存不存在就不能保证），后面四个条件就是原始问题的约束条件以及拉格朗日乘子需要满足的约束。

特别注意当![](http://images.cnitblog.com/blog/659319/201411/091357125185787.jpg)时，由KKT对偶互补条件可知：![](http://images.cnitblog.com/blog/659319/201411/091358264561831.jpg)，这个知识点会在
 SVM 的推导中用到.

**5. 总结**

一句话，某些条件下，把原始的约束问题通过拉格朗日函数转化为无约束问题，如果原始问题求解棘手，在满足KKT的条件下用求解对偶问题来代替求解原始问题，使得问题求解更加容易。


**本文源地址：http://www.cnblogs.com/90zeng/  作者：博客园-90Zeng**

**个人总结：本文关于拉格朗日极值问题的扩展和对偶问题讲解的非常到位细致，而且这作者的很多文章都写的非常透彻明白，可以详细阅读。**


# 逻辑回归Logistic Regression - 李鑫o_O - CSDN博客





2016年04月13日 13:55:57[hustlx](https://me.csdn.net/HUSTLX)阅读数：1012
个人分类：[机器学习](https://blog.csdn.net/HUSTLX/article/category/6122182)









**什么是逻辑回归？**

Logistic回归与多重线性回归实际上有很多相同之处，最大的区别就在于它们的因变量不同，其他的基本都差不多。正是因为如此，这两种回归可以归于同一个家族，即广义线性模型（generalizedlinear model）。

这一家族中的模型形式基本上都差不多，不同的就是因变量不同。


- 如果是连续的，就是多重线性回归；
- 如果是二项分布，就是Logistic回归；
- 如果是Poisson分布，就是Poisson回归；
- 如果是负二项分布，就是负二项回归。



Logistic回归的因变量可以是二分类的，也可以是多分类的，但是二分类的更为常用，也更加容易解释。所以实际中最常用的就是二分类的Logistic回归。

Logistic回归的主要用途：


- 寻找危险因素：寻找某一疾病的危险因素等；
- 预测：根据模型，预测在不同的自变量情况下，发生某病或某种情况的概率有多大；
- 判别：实际上跟预测有些类似，也是根据模型，判断某人属于某病或属于某种情况的概率有多大，也就是看一下这个人有多大的可能性是属于某病。



Logistic回归主要在流行病学中应用较多，比较常用的情形是探索某疾病的危险因素，根据危险因素预测某疾病发生的概率，等等。例如，想探讨胃癌发生的危险因素，可以选择两组人群，一组是胃癌组，一组是非胃癌组，两组人群肯定有不同的体征和生活方式等。这里的因变量就是是否胃癌，即“是”或“否”，自变量就可以包括很多了，例如年龄、性别、饮食习惯、幽门螺杆菌感染等。自变量既可以是连续的，也可以是分类的。



**常规步骤**

Regression问题的常规步骤为：


- 寻找h函数（即hypothesis）；
- 构造J函数（损失函数）；
- 想办法使得J函数最小并求得回归参数（θ）






**构造预测函数h**

Logistic回归虽然名字里带“回归”，但是它实际上是一种分类方法，主要用于两分类问题（即输出只有两种，分别代表两个类别），所以利用了Logistic函数（或称为Sigmoid函数），函数形式为：

![](https://img-blog.csdn.net/20140716153623240)

Sigmoid 函数在有个很漂亮的“S”形，如下图所示（引自维基百科）：



![](https://img-blog.csdn.net/20140716153637888)



下面左图是一个线性的决策边界，右图是非线性的决策边界。

![](https://img-blog.csdn.net/20140716153645298)





对于线性边界的情况，边界形式如下：

![](https://img-blog.csdn.net/20140716153419984)

构造预测函数为：

![](https://img-blog.csdn.net/20140716153431593)


函数![](https://img-blog.csdn.net/20140716153711397)的值有特殊的含义，它表示结果取1的概率，因此对于输入x分类结果为类别1和类别0的概率分别为：

![](https://img-blog.csdn.net/20140716153719088)




**构造损失函数J**

Cost函数和J函数如下，它们是基于最大似然估计推导得到的。


![](https://img-blog.csdn.net/20140716153726545)


![](https://img-blog.csdn.net/20140716153733362)


下面详细说明推导的过程：

（1）式综合起来可以写成：

![](https://img-blog.csdn.net/20140716153740866)

取似然函数为：

![](https://img-blog.csdn.net/20140716153515015)




对数似然函数为：

![](https://img-my.csdn.net/uploads/201407/16/1405496926_1023.png)




最大似然估计就是求使![](https://img-my.csdn.net/uploads/201407/16/1405496927_9629.png)取最大值时的θ，其实这里可以使用梯度上升法求解，求得的θ就是要求的最佳参数。但是，在Andrew Ng的课程中将![](https://img-my.csdn.net/uploads/201407/16/1405496928_3363.png)取为下式，即：

![](https://img-my.csdn.net/uploads/201407/16/1405496967_1305.png)


因为乘了一个负的系数-1/m，所以取![](https://img-my.csdn.net/uploads/201407/16/1405496928_3363.png)最小值时的θ为要求的最佳参数。



**梯度下降法求的最小值**

θ更新过程：



![](https://img-my.csdn.net/uploads/201407/16/1405496971_1656.png)

![](https://img-my.csdn.net/uploads/201407/16/1405496972_1821.png)




θ更新过程可以写成：



**![](https://img-my.csdn.net/uploads/201407/16/1405496975_3372.png)**



**向量化Vectorization**

Vectorization是使用矩阵计算来代替for循环，以简化计算过程，提高效率。

如上式，Σ(...)是一个求和的过程，显然需要一个for语句循环m次，所以根本没有完全的实现vectorization。




下面介绍向量化的过程：

约定训练数据的矩阵形式如下，x的每一行为一条训练样本，而每一列为不同的特称取值：



![](https://img-my.csdn.net/uploads/201407/16/1405496976_8789.png)



g(A)的参数A为一列向量，所以实现g函数时要支持列向量作为参数，并返回列向量。由上式可知![](https://img-my.csdn.net/uploads/201407/16/1405497004_1412.png)可由![](https://img-my.csdn.net/uploads/201407/16/1405497005_7185.png)一次计算求得。

θ更新过程可以改为：

![](https://img-my.csdn.net/uploads/201407/16/1405497005_4413.png)




综上所述，Vectorization后θ更新的步骤如下：

（1）求![](https://img-my.csdn.net/uploads/201407/16/1405497009_7879.png)；

（2）求![](https://img-my.csdn.net/uploads/201407/16/1405497010_5959.png)；

（3）求 ![](https://img-my.csdn.net/uploads/201407/16/1405497043_4791.png)。







**正则化Regularization**

**过拟合问题**

对于线性回归或逻辑回归的损失函数构成的模型，可能会有些权重很大，有些权重很小，导致过拟合（就是过分拟合了训练数据），使得模型的复杂度提高，泛化能力较差（对未知数据的预测能力）。

下面左图即为欠拟合，中图为合适的拟合，右图为过拟合。

![](https://img-my.csdn.net/uploads/201407/17/1405590306_6011.png)




**问题的主因**

过拟合问题往往源自过多的特征。

**解决方法**

1）减少特征数量（减少特征会失去一些信息，即使特征选的很好）


- 可用人工选择要保留的特征；
- 模型选择算法；



2）正则化（特征较多时比较有效）


- 保留所有特征，但减少θ的大小



**正则化方法**

正则化是结构风险最小化策略的实现，是在经验风险上加一个正则化项或惩罚项。正则化项一般是模型复杂度的单调递增函数，模型越复杂，正则化项就越大。

从房价预测问题开始，这次采用的是多项式回归。左图是适当拟合，右图是过拟合。

![](https://img-my.csdn.net/uploads/201407/17/1405590306_2443.png)




直观来看，如果我们想解决这个例子中的过拟合问题，最好能将![](https://img-my.csdn.net/uploads/201407/17/1405590307_8774.png)的影响消除，也就是让![](https://img-my.csdn.net/uploads/201407/17/1405590308_6412.png)。假设我们对![](https://img-my.csdn.net/uploads/201407/17/1405590338_4363.png)进行惩罚，并且令其很小，一个简单的办法就是给原有的Cost函数加上两个略大惩罚项，例如：

![](https://img-my.csdn.net/uploads/201407/17/1405590338_9255.png)


这样在最小化Cost函数的时候，![](https://img-my.csdn.net/uploads/201407/17/1405590308_6412.png)。

正则项可以取不同的形式，在回归问题中取平方损失，就是参数的L2范数，也可以取L1范数。取平方损失时，模型的损失函数变为：

![](https://img-my.csdn.net/uploads/201407/17/1405590339_4006.png)




lambda是正则项系数：


- 如果它的值很大，说明对模型的复杂度惩罚大，对拟合数据的损失惩罚小，这样它就不会过分拟合数据，在训练数据上的偏差较大，在未知数据上的方差较小，但是可能出现欠拟合的现象；
- 如果它的值很小，说明比较注重对训练数据的拟合，在训练数据上的偏差会小，但是可能会导致过拟合。



正则化后的梯度下降算法θ的更新变为：

![](https://img-my.csdn.net/uploads/201407/17/1405590339_8017.png)




正则化后的线性回归的Normal Equation的公式为：

![](https://img-my.csdn.net/uploads/201407/17/1405590340_5897.png)





**其他优化算法**


- Conjugate gradient method(共轭梯度法)
- Quasi-Newton method(拟牛顿法)
- BFGS method
- L-BFGS(Limited-memory BFGS)



后二者由拟牛顿法引申出来，与梯度下降算法相比，这些算法的优点是：


- 第一，不需要手动的选择步长；
- 第二，通常比梯度下降算法快；



但是缺点是更复杂。



**多类分类问题**

对于多类分类问题，可以将其看做成二类分类问题：保留其中的一类，剩下的作为另一类。

对于每一个类 i 训练一个逻辑回归模型的分类器![](https://img-my.csdn.net/uploads/201407/16/1405497043_3467.png)，并且预测y = i时的概率；对于一个新的输入变量x, 分别对每一个类进行预测，取概率最大的那个类作为分类结果：

![](https://img-my.csdn.net/uploads/201407/16/1405497044_2854.png)



![](https://img-my.csdn.net/uploads/201407/16/1405497045_4992.png)



# 4. 代码分析



图4中是《机器学习实战》中给出的部分实现代码。

![](https://img-blog.csdn.net/20131113202512453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZG9uZ3Rpbmd6aGl6aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




图4

sigmoid函数就是前文中的**g(z)**函数，参数**inX**可以是向量，因为程序中使用了[Python](http://lib.csdn.net/base/11)的numpy。

gradAscent函数是梯度上升的实现函数，参数dataMatin和classLabels为训练数据，23和24行对训练数据做了处理，转换成numpy的矩阵类型，同时将横向量的classlabels转换成列向量labelMat，此时的dataMatrix和labelMat就是（18）式中的**x**和**y**。alpha为学习步长，maxCycles为迭代次数。weights为n维（等于**x**的列数）列向量，就是（19）式中的**θ**。

29行的for循环将更新**θ**的过程迭代maxCycles次，每循环一次更新一次。对比3.4节最后总结的向量化的**θ**更新步骤，30行相当于求了**A=x.θ**和**g(A)**，31行相当于求了**E=g(A)-y**，32行相当于求**θ:=****θ-α.x'.E**。所以这三行代码实际上与向量化的**θ**更新步骤是完全一致的。

总结一下，从上面代码分析可以看出，虽然只有十多行的代码，但是里面却隐含了太多的细节，如果没有相关基础确实是非常难以理解的。相信完整的阅读了本文，就应该没有问题了！^_^。




**参考链接**

**[http://blog.csdn.net/dongtingzhizi/article/details/15962797](http://blog.csdn.net/dongtingzhizi/article/details/15962797)**

**[Coursera公开课笔记: 斯坦福大学](http://52opencourse.com/125/coursera%E5%85%AC%E5%BC%80%E8%AF%BE%E7%AC%94%E8%AE%B0-%E6%96%AF%E5%9D%A6%E7%A6%8F%E5%A4%A7%E5%AD%A6%E6%9C%BA%E5%99%A8%E5%AD%A6%E4%B9%A0%E7%AC%AC%E5%85%AD%E8%AF%BE-%E9%80%BB%E8%BE%91%E5%9B%9E%E5%BD%92-logistic-regression)[机器学习](http://lib.csdn.net/base/2)第六课“逻辑回归(Logistic Regression)”**

**[Coursera公开课笔记: 斯坦福大学机器学习第七课“正则化(Regularization)”](http://52opencourse.com/133/coursera%E5%85%AC%E5%BC%80%E8%AF%BE%E7%AC%94%E8%AE%B0-%E6%96%AF%E5%9D%A6%E7%A6%8F%E5%A4%A7%E5%AD%A6%E6%9C%BA%E5%99%A8%E5%AD%A6%E4%B9%A0%E7%AC%AC%E4%B8%83%E8%AF%BE-%E6%AD%A3%E5%88%99%E5%8C%96-regularization)**





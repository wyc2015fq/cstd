# Stanford机器学习课程笔记——单变量线性回归和梯度下降法 - bigfacesafdasgfewgf - CSDN博客





2015年01月23日 11:27:44[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1785








**Stanford机器学习课程笔记——单变量线性回归和梯度下降法**



# 1. 问题引入




    单变量线性回归就是我们通常说的线性模型，而且其中只有一个自变量x，一个因变量y的那种最简单直接的模型。模型的数学表达式为y=ax+b那种，形式上比较简单。Stanford的机器学习课程引入这个问题也想让我们亲近一下machine learning这个领域吧~吴恩达大神通过一个房屋交易的问题背景，带领我们理解Linear regression with one variable。如下：




![](https://img-blog.csdn.net/20150123104956786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





不要看这个问题简答，大神就是大神，吴大神通过房屋问题引出了**代价函数**和**梯度下降法**。




# 2. 代价函数 cost function




    首先，我们明确一下，单变量线性回归模型的表达式是：![](https://img-blog.csdn.net/20150123105445153?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。

    那么代价函数就是：我们模型预测的值（预测的房屋价格）和样本真实的值（真实价格）之间误差的平方和。数学表达就是：

![](https://img-blog.csdn.net/20150123105622171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


后面误差的平方项比较好理解，sigma求和就是所有训练样本的误差平方项之和，前面1/m是求均值，为啥分母上还有2呢？这是为了后面梯度下降法求导代价函数方便。

    好了，我们定义了代价函数，那么后面的任务就是如何最小化这个代价函数值，换句话说，求解一个最优化问题，得到代价函数最小时的theta_0和theta_1的组合参数。




# 3. 简单的梯度下降法




    有了上面的最优化问题，我们使用梯度下降法求解。需要解释的是，这种方法只能是迭代解出局部优化，无法得到解析解的全局最优。

    首先我们看上面代价函数，其中有两个变量，我们要判断这个目标函数是否是凸的，我们可以画出关于theta_0, theta_1, J(theta_0, theta_1)的三维曲面。（如果不是三维的，我们可以理论上判断是否为凸规划）

![](https://img-blog.csdn.net/20150123110330156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





我们发现，图中是存在最低点的，说明该优化问题是有最优解的。




    梯度下降法的基本思想：开始时我们随机一个参数组合![](https://img-blog.csdn.net/20150123110544890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，计算代价函数；然后我们寻找下一个能让代价函数值下降很多的参数组合，这个时候我们需要通过求导来寻找下降最快的方向。我们持续这么做直到到达一个局部最小值，因为我们没有尝试完所有的参数组合，所以我们不能确定我们得到的局部最小值是否是全局最小值。而且，选择不同的初始参数组合，可能会得到不同的局部最小值。图示如下：

![](https://img-blog.csdn.net/20150123111838781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    梯度下降法的基本算法为：

![](https://img-blog.csdn.net/20150123111324112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中alpha是学习步长，或者称为学习率，这是在我们找到下降最快的方向之后，确定着我们到底要下降多少的问题。看来，初始参数的选择和学习步长是梯度下降法中比较重要的两个参数，我们这个先不提如何选择。




    在这道题目中，我们梯度下降法求出代价函数的导数作为下降最快的方向，求导如下：




![](https://img-blog.csdn.net/20150123111519000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





那么我们的模型中只有两个theta, 于是我们需要分别对它们求导。

![](https://img-blog.csdn.net/20150123111606640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（都是二次函数的求导，比较简单。要注意j=1时的求导，后面还有一个x_i）

得到梯度下降方向之后，我们的算法就是：




![](https://img-blog.csdn.net/20150123111723167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


详细来说，我们先把theta_0和theta_1的初始值带入，把所有的训练样本带入，就得到了上面等式右边的所有元素的值，其中的h_theta_x_i是初始参数构建的模型求得的值。注意，所有的theta都是需要同时学习，同时更新的！绝对不可以先更新theta_0，然后后面在更新其余的theta的时候就使用新的theta_0在模型中，一定要使用老的theta。这里我们就说的比较粗俗一点啦~~吴大神的Slide中尤其提到了simultaneously的重要性！如下：

![](https://img-blog.csdn.net/20150123112417875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这样子不断更新，如果达到了一定更新的次数，或者是模型预测的值和样本真实值差异小于tolerated value，那么我们就可以停止更新了。



    这样我们就介绍完了单变量线性回归模型和简单的梯度下降法。



















参考：

[https://class.coursera.org/ml/class/index](https://class.coursera.org/ml/class/index)




[http://blog.csdn.net/abcjennifer/article/details/7691571](http://blog.csdn.net/abcjennifer/article/details/7691571)





Coursera机器学习笔记










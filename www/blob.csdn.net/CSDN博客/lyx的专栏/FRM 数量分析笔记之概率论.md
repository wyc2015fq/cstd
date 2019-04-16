# FRM 数量分析笔记之概率论 - lyx的专栏 - CSDN博客





2016年08月18日 15:00:51[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1719








        FRM第一部分的考试第二章叫做数量分析，其实说白了就是概率论和数理统计。想想自己在本科学的概率论，虽然分数还比较高，但是真的是没有理解透彻，学了一遍也算是加深了系统性理解了吧。

## 1.基本运算

  随机变量是什么东西呢？一般来说，一个变量就是一个值，但是在概率的世界里面，一个变量的描述是一个概率分布函数，存在不确定性。那么对于变量，必然有四种基本运算，加减乘除。

![](https://img-blog.csdn.net/20160818142453336?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 运算就如同上表。特别要注意的是，互斥和独立的区别，就像后面要讲的joint概率和conditional概率一样，前者是指同时性，后者是指相关性，两者没有联系。

![](https://img-blog.csdn.net/20160818142728167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        这是概率论中比较重要的公式，就是贝叶斯公式，就是用先验概率来推测后验概率，公式很简单，把分子乘到左边后就是很显然的等式，但是含义却很丰富，大家可以随意百度，找一些例子。

## 2、均值、方差与协方差

        然后我们来考虑均值、方差这些。

        均值太简单的，就不说了。我们考察一下方差，然后联系一下方差和均值。

        计算X变量的方差，与均值联系之后是这样计算的

        V(X)=E(X^2)-E(X)^2，换句话说，就是平方的期望减去期望的平方。我们想一下，什么时候两者相等？就是X是一个常数的时候，而这个时候，X的波动率为0，也就是说，方差为零。

        那么协方差呢？其实协方差和方差的计算基本一致，只是一个是X，一个是Y，平方变乘积，如此而已。而协方差竟然有这么好的性质，可以用于衡量两个变量的相关性，这也是很神奇的事情。

        COV（X,Y）=E(XY)-E(X)E(Y)。这个式子告诉我们两个变量之间，协方差的计算方式。那么，如果协方差为零意味着什么呢？就是X与Y没有关系，换句话说，就是X与Y独立（independent），记住，是独立，不是互斥哦。从信息熵的角度考虑，就是已知X，对于你减少Y的不确定性没有任何帮助，因为它们没有半毛钱关系。

        于是，我们得出了判断两个变量是否相互独立的办法，就是E（XY）是否与E(X)E(Y)相等。

![](https://img-blog.csdn.net/20160818144128490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        接下来我们看一下协方差和方差的一个展开方式。

        协方差的展开就像多项式的展开一样，只要知道常数与一个变量的协方差等于零这个常识就可以了。常数的信息量为零，这是显然的。

        而对于方差，我们不妨看一下后面带了两倍的一个协方差，而且是有正负号的。这是什么意思呢？如果X与Y之间是负相关的，那么，两个变量相加之后，协方差是负的，最后的总方差将会小于两个方差各自的加总。这里是不是有点CAPM模型的意味了。两个资产存在负相关性，那么就可以组合出更高sharp ratio的组合。

        最后我们讨论一下相关系数。当然，其实是线性相关系数。这个系数就是由协方差归一化得来的，而归一化因子就是X和Y的标准差。

![](https://img-blog.csdn.net/20160818144927899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        所以说，相关系数，协方差，各自方差，只要知道三个，就可以计算出另外一个。

## 3、偏度与峰度

        接下来是一个用来衡量变量对称性的度量指标，就做偏度，计算式子：



![](https://img-blog.csdn.net/20160818145119728?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        很显然，这是一个可正可负的值，那么正负号表达什么呢？就是分布相对于均值是否偏离。就像下图这样，负值，则表示负偏，尾巴在左边。

![](https://img-blog.csdn.net/20160818145558234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        最后还有一个指标，叫做峰度，用来衡量尖峰的程度。正态分布的峰度是3，所以如果计算出来大于3，那么就是比正态分布要尖，同时尾部会肥一些。反之一样。







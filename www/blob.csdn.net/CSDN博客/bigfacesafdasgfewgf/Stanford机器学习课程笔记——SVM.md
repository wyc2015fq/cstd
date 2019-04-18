# Stanford机器学习课程笔记——SVM - bigfacesafdasgfewgf - CSDN博客





2015年01月29日 12:12:02[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1289标签：[SVM																[线性不可分																[核函数																[SMO算法																[LR算法](https://so.csdn.net/so/search/s.do?q=LR算法&t=blog)
个人分类：[Machine Learning&&Data Mining](https://blog.csdn.net/puqutogether/article/category/2254295)





**Stanford机器学习课程笔记——SVM**



    前面已经学习过SVM，写了几片博文，如下：

入门SVM：http://blog.csdn.net/puqutogether/article/details/39894835

SVM中的线性不可分情况：http://blog.csdn.net/puqutogether/article/details/41309745

SVM中的多类分类问题：http://blog.csdn.net/puqutogether/article/details/41679605

如何使用sklearn中的SVM：http://blog.csdn.net/puqutogether/article/details/43194159




    这一篇就来把上面学习的过程融合成一个整体。




# 1. 由LR引出SVM




    SVM分类器不是凭空而出的，它是和我们前面讲到的LR回归模型有关系的。或者说就是从LR回归模型中的代价函数改进而来的。我们知道LR回归模型的代价函数J定义为：

![](https://img-blog.csdn.net/20150129121432063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





这个代价函数中是依据log函数建立的，而log函数曲线发现，如果thetaT*x>=1或者thetaT*x<=-1，log函数值是不为0的，而我们需要的理想情况是当thetaT*x>>1或者thetaT*x<<-1的时候，函数值要等于0，代价函数值要等于0，误差也要等于0.所以有了上图中玫红色的理想cost函数，它是一个分段函数的形式。如下：

![](https://img-blog.csdn.net/20150129122039413?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


    那么我们就需要在LR的代价函数中修改形式，如下：

![](https://img-blog.csdn.net/20150129121755441?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





第一个公式是LR的代价函数，第二个是SVM的代价函数。除了log函数被cost函数替代之外，后面的正则项也稍微变化，1/m被约去了，lambda被提到前面成为了C，这个C一般要设置地很大,这样子的SVM才能够找到最大间隔的超平面。

    既然C要设置地很大，那么SVM的代价函数中的第一项的惩罚就很高，在转换过程中，当thetaT*x>=1或者thetaT*x<=-1的时候，我们可以将其忽略。这样，转换之后的SVM的目标函数就是：

![](https://img-blog.csdn.net/20150129130149740?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


具体转换的过程，在我之前的入门SVM一篇博文中已经涉及，这里主要将讲思路。



2. 解SVM模型的目标函数




    解上面的目标函数，我们就需要使用拉格朗日函数。如下：

![](https://img-blog.csdn.net/20150129130424851?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

对于这个函数，我们使用对偶问题和KKT条件来求解，进一步转换为：

![](https://img-blog.csdn.net/20150129130608018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（这个转换过程是通过拉格朗日函数对w和b求偏导，令偏导等于0，之后代入到上面拉格朗日函数中得到的。。。）

发现这个里面只有一个参数a，那么我们就直接求解这个参数就可以了，而SVM模型中的w和b都是可以通过a来表示得到的，如下：

![](https://img-blog.csdn.net/20150129130731571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，![](https://img-blog.csdn.net/20150129130808153?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





而求解参数a，我们使用的是SMO算法，这个算法很巧妙，可以快速求得SVM模型中的参数a，其基本思想如下：

SMO的工作原理是：每次循环中选择两个alpha进行优化处理，一旦找到一对合适的alpha，那么就增大其中一个，同时减小另一个。选择的alpha要满足在间隔边界之外的条件，而且还没有进行过区间化处理或者不再边界上。


额，确实比较难懂。。。我也就没有太深究这块儿，有时间还是可以深入进去理解的。




    这样我们就可以学习到一个SVM分类模型了。




# 3. 线性不可分情况





线性不可分的情况，我在前面的博文中也讲到过，这里我们只说一下其中最核心的思想？



    如何解决SVM的线性不可分问题：引入核函数；

    核函数的基本做法是什么？核函数就是把样本原始的线性不可分的特征隐式映射到一个高维空间中，使得样本的相关特征被分开，这样在新空间中样本的特征是可分的。

    核函数中最关键的一步是什么？就是核函数是隐式映射到高维空间的，在SVM中要做样本的内积运算，一般做法就是先把样本显式映射到高维空间，然后再做内积运算，但是这样子会造成维数灾难。而核函数是隐式映射，是先在核函数中求内积的，这个时候内积就是在输入空间中求的，不需要写出显示映射之后的结果，也就不怕维数灾难了。




# 4.  SVM扩展到多类分类问题




    这部分也参考我们之前的博文，里面给出了几个扩展改进的思路。其中one-one 和 one-other也常用到的两种。







参考：

http://blog.csdn.net/puqutogether/article/details/39894835


http://blog.csdn.net/puqutogether/article/details/41309745


http://blog.csdn.net/puqutogether/article/details/41679605


http://blog.csdn.net/puqutogether/article/details/43194159


http://blog.csdn.net/abcjennifer/article/details/7849812


http://blog.csdn.net/v_july_v/article/details/7624837


http://www.cnblogs.com/vivounicorn/archive/2010/12/13/1904720.html](https://so.csdn.net/so/search/s.do?q=SMO算法&t=blog)](https://so.csdn.net/so/search/s.do?q=核函数&t=blog)](https://so.csdn.net/so/search/s.do?q=线性不可分&t=blog)](https://so.csdn.net/so/search/s.do?q=SVM&t=blog)





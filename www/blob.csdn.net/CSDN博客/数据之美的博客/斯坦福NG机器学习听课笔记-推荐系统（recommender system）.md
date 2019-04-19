# 斯坦福NG机器学习听课笔记-推荐系统（recommender system） - 数据之美的博客 - CSDN博客
2018年05月13日 23:09:52[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：471
# 推荐系统（recommender system）
## Problem Formulation:
RecommenderSystems:为什么讲它有两个原因：首先它是一个很重要的机器学习应用方向，在很多公司中占据了重要作用，像亚马逊之类网站都是很好的建立推荐系统促进商品销售。其次推荐系统有机器学习中一些big idea，通过学习推荐系统学习机器学习中的big idea。接着描述一个电影评级的例子(整篇博客都将以电影评级为实例)。
![](https://img-blog.csdn.net/20141229093625342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
电影评级有5级，打问号的是不知道的信息，推荐系统需要做的就是在已知数据上去预测打问号下的数据是什么。直观上我们可以分块发现Alice Bob 偏向于看爱情片，而后面两位偏向于看动作电影，这些是在数据上比较直白的信息获取（这就是后面说到的我们已知特征向量情况）。还有需要注意上图中形式化描述整个课程都是以这个为基础的。
习题：就是让大家熟悉NG对电影评级中各个变量参数表示。
![](https://img-blog.csdn.net/20141229093915757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## Content Based Recommendations:
先看一下如果进行未评级电影预测的流程：
![](https://img-blog.csdn.net/20141229094121009?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图中右边可以看到我们把所有电影通过romance action 两个特征来进行描述，我们对空白位置预测思想方法就是：对每一个user 我们学习参数（怎么学习这个参数后面说），然后再通过上图参数向量与特征向量乘积值来判断。
习题：
![](https://img-blog.csdn.net/20141229094946949?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上题只需要把参数向量代入与特征向量相乘和已知数据比较，拟合合适的就成立。
这个问题的形式化描述：
![](https://img-blog.csdn.net/20141229095025980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如何学习参数向量，利用线性回归方法。
目标函数是：
![](https://img-blog.csdn.net/20141229095113171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
因为m(j) 是常数进行简化目标函数：
![](https://img-blog.csdn.net/20141229095143778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上面公式第一个是描述某一个用户的，而下面公式是对所有user 求和使得目标函数最小。这里求目标函数最小值就类似线性回归中方法，梯度下降法：
![](https://img-blog.csdn.net/20141229095235024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
总结：这个算法我们假设我们已经拥有特征，我们知道电影用什么特征可以来进行描述。但是有很多电影我们可能无法通过特征向量来进行描述，下面方法解决这里提到的问题。
## Collaborative Filtering:
这里问题条件就改变了，我们现在无法获得电影特征描述，但是我们可以获得不同用户对不同电影爱好也就是theta参数向量。
![](https://img-blog.csdn.net/20141229095411312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
习题：已知theta向量求特征值。
![](https://img-blog.csdn.net/20141229095449703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
需要优化的目标函数：
![](https://img-blog.csdn.net/20141229095521671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
习题：梯度下降法迭代求解。
![](https://img-blog.csdn.net/20141229095600343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下面对Collaborative filtering 形式化描述真正的协同过滤比这个复杂，这里只是首先就行一个概念上的认知。
![](https://img-blog.csdn.net/20141229095633829?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
前面我们提到了两种方法：第一种是一直特征集描述，然后估计theta ，第二种是一直theta 去估计特征集。我们可以交叉使用这两种方法来更好估计特征与theta参数。这里计算性能不高接下来会讲到这个问题的计算优化。
## CollaborativeFiltering Algorithm：
前面说到我们可以迭代优化theta和特征向量，但是这样性能比较低，所以现在考虑提升算法性能。同时对两种方法进行求解。
![](https://img-blog.csdn.net/20141229100345201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图就是把两种方法优化函数进行合并得到总的目标函数。
算法流程图：
![](https://img-blog.csdn.net/20141229100425156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
习题：
![](https://img-blog.csdn.net/20141229100452110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## Vectorization Low rank matrix factorization:
这里主要讲的就是通过两个向量相乘构造Predicted ratings matrix
![](https://img-blog.csdn.net/20141229100556609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
习题：
![](https://img-blog.csdn.net/20141229100623183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
向量化后有一个很大的便利之处就是推荐发现相似的电影或者网站推荐相似物品。我们只需要计算两个物品特征向量之间的距离，距离越小意味着肯定是更加相似的东西。
![](https://img-blog.csdn.net/20141229100655147?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## Implementationaldetail : Mean normalization
我们加入新的一个user Eve，通过上面说的计算theta 和电影评分，我们发下如下图：
![](https://img-blog.csdn.net/20141229100741078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们发现Eve对所有电影评分都是0，显然这样结果是不合适的，现在需要解决这个问题。
上面我们说了当一个user对电影没有任何评级时，我们很难处理，用前面说到算法执行得到都为0。现在我们改进方法如下图：
![](https://img-blog.csdn.net/20141229100813031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当theta与X(i)乘积为0 ，但是在加上均值所以像Eve这样用户我们不会把他评分都设定为0。
你可以用Mean Normalization 对数据进行预处理。这样可能会使得算法得到一些改进提高。
习题：
![](https://img-blog.csdn.net/20141229100847935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHVydXp1bg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

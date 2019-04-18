# Stanford概率图模型： 第一讲 有向图-贝叶斯网络 - wishchinYang的专栏 - CSDN博客
2013年11月04日 20:14:10[wishchin](https://me.csdn.net/wishchin)阅读数：2503
        原文链接（系列）：[http://blog.csdn.net/yangliuy/article/details/8067261](http://blog.csdn.net/yangliuy/article/details/8067261)
        概率图模型（Probabilistic Graphical Model）系列来自Stanford公开课Probabilistic Graphical Model中Daphne Koller 老师的讲解。（[https://class.coursera.org/pgm-2012-002/class/index](https://class.coursera.org/pgm-2012-002/class/index)）
主要内容包括（转载请注明原始出处[http://blog.csdn.net/yangliuy](http://blog.csdn.net/yangliuy)）
1.    贝叶斯网络及马尔可夫网络的概率图模型表示及变形。
2.    Reasoning 及Inference 方法，包括exact inference (variable elimination, clique trees) 和 approximate inference(belief propagation message passing, Markov chain Monte Carlo methods)。
3.    概率图模型中参数及结构的learning方法。
4.    使用概率图模型进行统计决策建模。
**第一讲. 贝叶斯网络基础**
**1、贝叶斯网络的定义**
        贝叶斯网络是一个有向无环图，其中结点代表了随机变量，边代表了随机变量之间的概率关系，其联合概率分布可以用贝叶斯链式法则来表示
![](https://img-my.csdn.net/uploads/201210/13/1350107921_1558.png)
       其中ParG(Xi)表示结点Xi在图G中的父节点对应的随机变量。
**2、贝叶斯网络中概率影响的流动（Flow of Probabilistic Influence）**
       概率影响的流动性反应了贝叶斯网络中随机变量条件独立性关系，如下图所示
![](https://img-my.csdn.net/uploads/201210/13/1350107927_5956.png)
图中贝叶斯网络模型反映如下四个随机变量之间的关系
       Difficulty 课程难度
      Intelligence 学生聪明程度
      Grade 学生课程成绩
      SAT 学生高考成绩
      Letter 学生是否可以得到教授工作推荐信
在左边对应的六种情况下，只有最后一种情况X→W←Y下X的概率不会影响到Y的概率。这是因为W不是被观察变量，其值是未知的，因此随机变量X的值不会影响随机变量Y的取值。有趣的是，当中间W变量成为被观察变量，上述结论就会发生变化。如下图所示
![](https://img-my.csdn.net/uploads/201210/13/1350107938_9285.png)
        当WєZ时，即W为观察变量时，所有判断会变得相反。仍然以 X→W← Y 为例，此时W的值已知，比如已知某个学生Grade为B，那么此时学生的聪明程度Intelligence和课程难度Difficulty就不再条件独立了。比如，这种情况下如果课程比较容易，那边学生很聪明的概率较小；反之，若课程很难，则学生很聪明的概率较大。其他情况可以采用右边这个贝叶斯网络例子来理解。
**3 Active Trails**
        经过第2部分的分析，我们可以归纳如下结论
Atrail X1 ─ … ─ Xn is active if:
it has no v-structures
Atrail X1 ─ … ─ Xn is active given Z if:
– for any v-structure Xi-1 → Xi← Xi+1 we have that Xi or one of its descendants ∈Z
– no other Xi is in Z
显然如果X1 ─ … ─Xn is active，那么X1和Xn就不再条件独立。
**4 Independence in Graph**
       这里先总结D-separate 的情况，如下图所示
![](https://img-my.csdn.net/uploads/201210/13/1350107944_2044.png)
有了2-3部分的基础，容易得出如下结论
![](https://img-my.csdn.net/uploads/201210/13/1350107950_8234.png)
练习题
![](https://img-my.csdn.net/uploads/201210/13/1350107957_6173.png)
答案选1和3。解析：由于G被观察，那么2中IGL这条路径就不再active，因为给定G后，I无法影响L的概率。而4中SJL这个v结构不是active的，因为J是未观察变量，S不能影响L的概率。 故而2和4都错误。1和3都是active的。
如果两个结点之间不存在任何Active Trails, 那么我们称这两个结点 d-separation 。有了d-separation 的概念，下面我们给出I-Map的定义
![](https://img-my.csdn.net/uploads/201210/19/1350647899_9841.JPG)
        本课程最后给出了Naïve Bayes算法的介绍，这个算法可以参见我的博文[数据挖掘-基于贝叶斯算法及KNN算法的newsgroup18828文本分类器的JAVA实现（上）](http://blog.csdn.net/yangliuy/article/details/7400984)
有给算法的详细介绍和实现。
        好了，第一讲到此结束，下一讲我们介绍Template Model及条件概率分布模型CPD。

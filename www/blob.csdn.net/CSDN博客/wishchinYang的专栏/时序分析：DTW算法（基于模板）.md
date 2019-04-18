# 时序分析：DTW算法（基于模板） - wishchinYang的专栏 - CSDN博客
2015年12月17日 18:01:15[wishchin](https://me.csdn.net/wishchin)阅读数：3959
        对时序对象进行分析，使用KMP算法可以分析速率不变的模式，参考[时序分析：欧式空间轨迹模式识别](http://blog.csdn.net/wishchin/article/details/50344209)。使用**基于模板匹配**的方法，对于速率发生变化的模式，需要用新的对速率要求松散的方法，DTW方法为一种广泛使用的方法。
       此外，基于模板的方法也有MEI方法（Measured Equation of invariance）、MHI方法（**OpenCV使用了-Forward-Backward MHI (before and after the historical figure to the movement)即**前向后向历史运动图。
      参考：行为分析算法综述（图像图形学学报）-古老的DTW方法（Dynamic Time Wrapper）
**一、 序列的对齐问题**
        援引两张图：[http://blog.csdn.net/zouxy09/article/details/9140207](http://blog.csdn.net/zouxy09/article/details/9140207)
        参考链接：[http://blog.csdn.net/vanezuo/article/details/5586727](http://blog.csdn.net/vanezuo/article/details/5586727)
        参考链接：[DTW用于语音识别](http://blog.sina.com.cn/s/blog_892508d501012wdi.html)
情况一：
       如图A所示，实线和虚线分别是同一个词“pen”的两个语音波形（在y轴上拉开了，以便观察）。可以看到他们整体上的波形形状很相似，但在时间轴上却是不对齐的。例如在第20个时间点的时候，实线波形的a点会对应于虚线波形的b’点，这样传统的通过比较距离来计算相似性很明显不靠谱。因为很明显，实线的a点对应虚线的b点才是正确的。而在图B中，DTW就可以通过找到这两个波形对齐的点，这样计算它们的距离才是正确的。
![](https://img-blog.csdn.net/20151217174342393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20151217174357829?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
也就是说，大部分情况下，两个序列整体上具有非常相似的形状，但是这些形状在x轴上并不是对齐的。所以我们在比较他们的相似度之前，需要将其中一个（或者两个）序列在时间轴下warping扭曲，以达到更好的对齐。而DTW就是实现这种warping扭曲的一种有效方法。DTW通过把时间序列进行延伸和缩短，来计算两个时间序列性之间的相似性。
       那如果才知道两个波形是对齐了呢？也就是说怎么样的warping才是正确的？直观上理解，当然是warping一个序列后可以与另一个序列重合recover。这个时候两个序列中所有对应点的距离之和是最小的。所以从直观上理解，warping的正确性一般指“feature to feature”的对齐。
**二、DTW算法**
动态时间规整DTW是一个典型的优化问题，它用满足一定条件的的时间规整函数W(n)描述测试模板和参考模板的时间对应关系，求解两模板匹配时累计距离最小所对应的规整函数。
      假设我们有两个时间序列Q和C，他们的长度分别是n和m：（实际语音匹配运用中，一个序列为参考模板，一个序列为测试模板，序列中的每个点的值为语音序列中每一帧的特征值。例如语音序列Q共有n帧，第i帧的特征值（一个数或者一个向量）是qi。至于取什么特征，在这里不影响DTW的讨论。我们需要的是匹配这两个语音序列的相似性，以达到识别我们的测试语音是哪个词）
**      Q**= q1, q2,…,qi,…, qn ;
**      C**= c1, c2,…, cj,…, cm ;
       如果 n = m，那么就用不着折腾了，直接计算两个序列的距离就好了。但如果n不等于m 我们就需要对齐。最简单的对齐方式就是线性缩放了。把短的序列线性放大到和长序列一样的长度再比较，或者把长的线性缩短到和短序列一样的长度再比较。但是这样的计算没有考虑到语音中各个段在不同情况下的持续时间会产生或长或短的变化，因此识别效果不可能最佳。因此更多的是采用动态规划（dynamic programming）的方法。
      为了对齐这两个序列，我们需要构造一个n x m的矩阵网格，矩阵元素(i, j)表示qi和cj两个点的距离d(qi, cj)（也就是序列Q的每一个点和C的每一个点之间的相似度，距离越小则相似度越高。这里先不管顺序），一般采用欧式距离，d(qi, cj)=
 (qi-cj)2（也可以理解为失真度）。每一个矩阵元素(i, j)表示点qi和cj的对齐。DP算法可以归结为寻找一条通过此网格中若干格点的路径，路径通过的格点即为两个序列进行计算的对齐的点。
![](https://img-blog.csdn.net/20151217175721290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        那么这条路径我们怎么找到呢？那条路径才是最好的呢？也就是刚才那个问题，怎么样的warping才是最好的。
        我们把这条路径定义为warping path规整路径，并用W来表示， W的第k个元素定义为wk=(i,j)k，定义了序列Q和C的映射。这样我们有：
![](https://img-blog.csdn.net/20151217175850802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       首先，这条路径不是随意选择的，需要满足以下几个约束：
**1）边界条件：**w1=(1, 1)和wK=(m, n)。任何一种语音的发音快慢都有可能变化，但是其各部分的先后次序不可能改变，因此所选的路径必定是从左下角出发，在右上角结束。
**2）连续性：**如果wk-1= (a’, b’)，那么对于路径的下一个点wk=(a, b)需要满足 (a-a’) <=1和 (b-b’) <=1。也就是不可能跨过某个点去匹配，只能和自己相邻的点对齐。这样可以保证Q和C中的每个坐标都在W中出现。
**3）单调性：**如果wk-1= (a’, b’)，那么对于路径的下一个点wk=(a, b)需要满足0<=(a-a’)和0<= (b-b’)。这限制W上面的点必须是随着时间单调进行的。以保证图B中的虚线不会相交。
结合连续性和单调性约束，每一个格点的路径就只有三个方向了。例如如果路径已经通过了格点(i, j)，那么下一个通过的格点只可能是下列三种情况之一：(i+1, j)，(i, j+1)或者(i+1, j+1)。
![](https://img-blog.csdn.net/20151217180025607?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
满足上面这些约束条件的路径可以有指数个，然后我们感兴趣的是使得下面的规整代价最小的路径：
![](https://img-blog.csdn.net/20151217180221843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       分母中的K主要是用来对不同的长度的规整路径做补偿。我们的目的是什么？或者说DTW的思想是什么？是把两个时间序列进行延伸和缩短，来得到两个时间序列性距离最短也就是最相似的那一个warping，这个最短的距离也就是这两个时间序列的最后的距离度量。在这里，我们要做的就是选择一个路径，使得最后得到的总的距离最小。
       这里我们定义一个累加距离cumulative distances。从(0, 0)点开始匹配这两个序列Q和C，每到一个点，之前所有的点计算的距离都会累加。到达终点(n, m)后，这个累积距离就是我们上面说的最后的总的距离，也就是序列Q和C的相似度 。
      累积距离γ(i,j)可以按下面的方式表示，累积距离γ(i,j)为当前格点距离d(i,j)，也就是点qi和cj的欧式距离（相似性）与可以到达该点的最小的邻近元素的累积距离之和：
![](https://img-blog.csdn.net/20151217180322199?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
      最佳路径是使得沿路径的积累距离达到最小值这条路径。这条路径可以通过动态规划（dynamic programming）算法得到。
      具体搜索或者求解过程的直观例子解释可以参考：[http://www.cnblogs.com/tornadomeet/archive/2012/03/23/2413363.html](http://www.cnblogs.com/tornadomeet/archive/2012/03/23/2413363.html)
**三、DTW代码：**
代码
**四、一些问题**

DTW的问题：
™       运算量大；
™       识别性能过分依赖于端点检测；
™       太依赖于说话人的原来发音；
™不能对样本作动态训练；
™       没有充分利用语音信号的时序动态特性；
       DTW适合于特定人基元较小的场合，多用于孤立词识别；
动态规划算法总体思想：
       动态规划算法基本思想是将待求解问题分解成若干个子问题
       但是经分解得到的子问题往往不是互相独立的。不同子问题的数目常常只有多项式量级。求解时，有些子问题被重复计算了许多次。
       如果能够保存已解决的子问题的答案，而在需要时再找出已求得的答案，就可以避免大量重复计算，从而得到多项式时间算法。
动态规划基本步骤：
       v找出最优解的性质，并刻划其结构特征。
       v递归地定义最优值。
       v以自底向上的方式计算出最优值。
       v根据计算最优值时得到的信息，构造最优解

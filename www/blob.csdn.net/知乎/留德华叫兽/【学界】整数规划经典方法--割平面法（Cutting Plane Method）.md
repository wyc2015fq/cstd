# 【学界】整数规划经典方法--割平面法（Cutting Plane Method） - 知乎
# 

> *作者*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*系美国克莱姆森大学运筹学硕士，Ph.D. Candidate，后跳槽至欧盟玛丽居里博士项目，期间前往意大利IBM Cplex实习半年，现任德国海德堡大学交叉学科计算中心、组合优化实验室助理研究员，主攻图像处理。**欢迎原链接转发，付费转载请前往*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*的主页获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，定期邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：
[『运筹OR帷幄』大数据人工智能时代的运筹学--知乎专栏](https://zhuanlan.zhihu.com/operations-research)

某天在全球运筹学者群，好几个小伙伴问到了关于Branch-and-Cut 以及Cutting Plane Method。由于其在整数规划的重要作用，于是乎一鼓作气写篇专栏解释一下。（结果还是拖到现在。。）

会点开这篇文章的小伙伴应该已是运筹学的中级玩家，至少学过线性规划和整数规划了，因此这里不再多软文宣传运筹学，有兴趣的可以关注以下几篇专栏文章。

[人工智能的“引擎”--运筹学，一门建模、优化、决策的科学](https://zhuanlan.zhihu.com/p/25579864?refer=operations-research)

[离散/整数/组合/非凸优化概述及其在AI的应用](https://zhuanlan.zhihu.com/p/27429666)

[混合整数规划/离散优化的精确算法--分支定界法及优化求解器](https://zhuanlan.zhihu.com/p/27659600)

本文提纲：

1,整数规划（Integer Programming）问题回顾   2,整数规划的精确算法--分支定界法（Branch-and-Bound） 3,整数规划的割平面方法（Branch-and-Cut）-- UserCut

4,整数规划的割平面方法-- LazyCut   5,行生成方法(Row Generation)   6,割平面法在计算机视觉的应用

------------------------------------------------------------------------------------

## 1，**整数规划**（**Integer Programming**）问题回顾
![](https://pic4.zhimg.com/v2-4b8b71534dd79a9dfa9ae5e783f8162b_b.jpg)
整数规划，或者**离散优化**（**Discrete Optimization**），是指**数学规划（Math Programming）**问题中自变量存在整数的一类问题。上面这个数学规划问题，便是一个混合整数线性规划问题。首先目标方程和约束方程都是线性的，其次自变量既有连续变量（x1、x3），又有整数变量（x2）。

与线性规划连续的**可行域**（可行解组成的集合）不同，整数规划的可行域是离散的。

![](https://pic1.zhimg.com/v2-9196250c67b0526bbe74b18ab47e2fa8_b.jpg)
如上图，一条蓝线代表一个线性不等式，但是这里x,y自变量被约束成整数变量，因此可行域变成了红线区域内的9个离散的黑点。（线性规划的可行域是蓝色线段内部所有的区域）

**凸包**（**Convex Hull**）:整数规划所有可行解的凸包围，即图中红线组成的多面体（想象多维的情况）。凸包是未知的，已知的是蓝线的不等式，并且凸包是非常难求解的，或者形成凸包需要指数数量级的线性不等式（图中红线）。如果知道了凸包的所有线性表示，那么整数规划问题就可以**等价**为求解一个凸包表示的线性规划问题。

另外，除了整数规划，还有**混合整数规划**（**Mixed Integer Programming**, **MIP**），即自变量既包含整数也有连续变量。如下图：
![](https://pic1.zhimg.com/v2-9815044ea2774f3c9f13ffe72dd65790_b.jpg)
这里是简单的二维情况，自变量x是连续的，y被约束成整数变量（0,1,..,n），这时候可行域变成了4条离散的橘黄色线段加上4处的黄色整数点（0,4）。（课后作业，求这个问题的凸包。）

整数规划由于可行域是**极度非凸**（**Highly Nonconvex**）的，因此也可以看作是一类特殊的**非凸优化（Nonconvex Optimization）**问题。

（关于离散优化，分享一个免费的coursera视频，[Lecture 43 - MIP](https://link.zhihu.com/?target=https%3A//www.coursera.org/learn/discrete-optimization/lecture/93rW1/mip-1-intuition-relaxation-branch-and-bound-knapsack-warehouse-location)，by 墨尔本大学Pascal Van Hentenryck教授。）

## 2，整数规划的精确算法--分支定界法（Branch-and-Bound）

科学的本质便是由简到难，先把简单问题研究透彻，然后把复杂问题**分解（decompose）**成求解一个个的简单问题，最后把简单问题的解“有机结合”起来，希望找到原问题的全局最优解或近似解。

整数规划问题通常是NP难（NP完全）的，求解整数规划的精确算法，便是利用分支定界法求解一个个线性规划问题。

每一个线性规划问题是多项式时间可解的，然而最坏情况需要求解2^n个线性规划问题。（这里假设整数变量是0,1变量，n是整数变量的个数）

这是指数爆炸！什么意思呢？当n=100时，求解时间是1分钟的话，那么n=101，时间可能是2分钟（最坏情况），n=102便是4分钟，以此类推。。

[混合整数规划/离散优化的精确算法--分支定界法及优化求解器](https://zhuanlan.zhihu.com/p/27659600)

正是由于如此，数学家们需要想出一些比分支定界更“聪明”的分解方法，包括割平面方法、行生成以及列生成方法等等。


## 3， 整数规划的割平面方法（Branch-and-Cut）-- UserCut

整数规划中的割平面方法，大致分为砍掉实数解的分割（cut，即一个线性不等式）和砍掉整数解的分割。前者对于原问题是一个valid inequality，而后者不是。
![](https://pic1.zhimg.com/v2-2372587f8bf772f69318d37b5f9deae8_b.jpg)
如上图，有这么一个整数规划问题，黑色线段是线性不等式，蓝色的点是离散的可行域，蓝色线段包围的空间是IP Hull（整数解形成的凸包，NP-hard to find，因为一旦找到，那么求解整数规划只需要求解凸包这个LP问题），在其外面黑色线段的包围是LP Hull（线性松弛解形成的凸包）。

正是因为IP Hull和LP Hull中间的间隙，使得该LP的最优解是fractional（对于原问题infeasible）的，而这段间隙，对于LP（线性规划）来讲，是多余的搜索空间。如果我们在这个时候可以加上一个或多个线性不等式（cut），把无用空间完全“砍”掉，那么LP Hull = IP Hull，这时我们就得到整数解了。

当然一般情况没有这么好运，可以把无用空间完全砍掉。如上图，加上红色虚线这个cut，我们砍掉了红色阴影区域这部分无用空间。虽然没有把LP Hull直接缩小成IP Hull这么立竿见影，但对于求解原问题，由于减少了搜索空间，也是一种效率上的提升。

另外值得注意的是，红色虚线的cut，对于原问题（IP Hull）也是满足的（valid），它砍掉的，只是实数部分无用的搜索空间。

最后我们想象上图是分支定界法求解到其中一个node所解得的线性松弛解，那么如果该步骤在分支定界法所有（或部分）node上不断重复（recall that一个0-1规划每一次branch就等价于求解俩个LP问题，每个LP问题都是一个node），该方法就被称为Branch-and-Cut。

而以上cut，在Cplex等优化求解器中，被称为UserCut。


## 4， 整数规划的割平面方法-- LazyCut
![](https://pic1.zhimg.com/v2-66f9bd84d5cdbb7fa6d8d826cd77aa98_b.jpg)
上一节讲了砍掉实数解的cut，这一节我们讲讲砍掉整数解（feasible solution）的cut--LazyCut。

如上图，我们有IP hull和LP hull，我们加上一个Lazy Constraints，这时候把顶上三个原问题的可行整数解也砍掉了。

为什么要把原问题可行的整数解也cut掉呢？适用范围有哪些呢？

割平面法最经典的应用，莫过于Travelling Salesman Problem（我老板的成名作）了。这个问题是每个学运筹学特别是组合优化必学的经典问题。

而这个问题求解的关键，便是割平面方法-- the subtour elimination constraints.
![](https://pic2.zhimg.com/v2-3a6909f8fece8afcf6df72a3bb0c4be9_b.jpg)
如上图，TSP需要找从一点出发，遍历所有城市（1-6点）再回到出发点的cycle（回路）。为了简化问题，在master problem（初始问题）的表达式中，我们忽略subtour（小的cycle，例如上图4-5-6）约束（因为有指数级个数该约束），然后求解该IP问题。

忽略掉subtour求得的IP问题虽然是整数可行解，但是其中可能存在subtour（如上图俩个subtour），因此其实并不是我们想要的解。这时候，我们设计一个启发式算法，来探测这些subtour，然后加上相应的cut砍掉这些subtour对应的整数解，然后再次求解IP。

由此循环，直到求得的IP整数解中，不再存在subtour，这时候我们找到了最终问题的全局最优解。

关于TSP问题，除了我老板1994年出版的教科书[The Traveling Salesman - Computational Solutions for TSP | Gerhard Reinelt | Springer](https://link.zhihu.com/?target=https%3A//www.springer.com/de/book/9783540583349)，最好的参考资料莫过于滑铁卢大学[William Cook](https://link.zhihu.com/?target=http%3A//www.math.uwaterloo.ca/~bico/)教授的网页了：

[Solving a TSP > Tour Quality >  Subtour Elimination](https://link.zhihu.com/?target=http%3A//www.math.uwaterloo.ca/tsp/methods/opt/subtour.htm)

这一节我非常简略地引入TSP问题介绍了LazyCut，由于是我老板的成名作（老板同时还创立了TSP的数据集[TSPLIB](https://link.zhihu.com/?target=https%3A//www.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/)），也因为该问题麻雀虽小五脏俱全，日后有时间一定单篇专栏详细介绍该问题。

关于割平面法的**优化求解器实现**，通常都需要用到其中的callback function，cplex等求解器也都有关于此法的算例，请参考example文件夹中的源代码。日后有空我也会专门写专栏详述。

本节最后和大家分享一个非常棒的OR博客（by Prof. **[Paul Rubin](https://link.zhihu.com/?target=https%3A//plus.google.com/111303285497934501993)**），关于UserCut和LazyCut，那里有着更加详细的解释。（有幸在美国一次MIP会议上与其聊过）

[OR in an OB World--User Cuts versus Lazy Constraints](https://link.zhihu.com/?target=https%3A//orinanobworld.blogspot.de/2012/08/user-cuts-versus-lazy-constraints.html)

## 5，行生成方法(Row Generation)

割平面方法，从更宏观的角度，可以看作是一种行生成方法。这里的“行”（row），指的是线性不等式。每找到一个cut，就增加一个线性不等式。

线性规划的算法复杂度和连续变量呈多项式级关系，另外随着约束条件（不等式）个数的增加，求解时间也会随之增加。（不确定呈什么关系，求拍砖）

上面说到整数规划的算法复杂度和整数变量的个数n基本呈指数关系，那么它还和其他什么因素相关呢？答案是不等式的个数。（recall求解整数规划需要求解一个个的线性规划）

我们从线性规划角度，理解行生成方法的基本思想：形成极值（最优解）所需要的约束条件个数，往往远小于原问题的约束条件个数。因此为何不在需要的时候，才把这些“重要”的约束条件加上来呢？

下面举个简单例子：

如下图，原问题有5个不等式（一条红线代表一个不等式），但是最优解点D只需CD和DE 2个不等式即可表述。

![](https://pic3.zhimg.com/v2-b0f98bda58711093612a0a8d3800fb4e_b.jpg)

因此行生成方法的基本思路：先求解原问题的松弛问题，即初始问题（master problem）不加约束条件或只加其中几个约束，然后求解该松弛问题，如果得到的解是可行解，那么该解就是原问题的最优解（例如刚开始运气很好地加了CD和DE）。

如果得到的解对原问题是不可行的，例如解是（0，6）这个点（因为没有加BC这个约束），或者无界的，那么这时候加上BC这个不等式便可以把这个不可行解排除。

以此循环，直到松弛问题的解是可行的，那么该解也是原问题的最优解。

而通过行生成方法，上面问题本来需要5个约束条件，很可能只需要2-3个约束条件，上面的循环已经终止了。

在实际问题中，最优解所需要的约束条件往往远远小于原问题的约束条件个数。例如几万个约束条件，实际只有几百个是用来刻画最优解的。那么这个时候，割平面方法便可以大大提速线性规划的求解。

在上一节的TSP问题中，subtour elimination constraints的个数是指数级的（因此不可能把他们全部加进来），但是求解实际问题中，往往通过割平面方法只需找到其中几千或几百个，即可找到原问题的最优解。用到的，正是相似的思路。

其实TSP问题是有完整刻画的表达式的（Complete Formulation），这时的约束个数虽然不是指数级，但是数量也非常大，因此求解效率很低。割平面方法的引入，大大增加了TSP问题求解的高效性，这也是该方法一次完美的show off。

搜索Literature 行生成方法，最先映入眼帘的可能是Benders' Decomposition。在那里，一般把整数和实数变量隔离做分解，然后有比较“严格”的如何选取初始约束以及如何一步步地增加约束（feasibility cut和optimality cut）。

与行生成法对偶的方法，是列生成法（逐步增加变量个数）。其中的Dantzig-Wolfe分解法，是Benders' Decomposition的dual problem，这些是以后我将和大家分享的求解整数规划众多分解法其中的俩个。

由于时间关系不再展开，作为预热，有兴趣的可以参考：

[BENDERS DECOMPOSITION WITH GAMS](https://link.zhihu.com/?target=https%3A//web.stanford.edu/class/msande348/papers/bendersingams)

[Column Generation and Dantzig-Wolfe Decomposition](https://link.zhihu.com/?target=http%3A//www.science4all.org/article/column-generation/)

## 6，割平面法在计算机视觉的应用

割平面法，可以说是整数、离散优化最经典的分解方法之一，在运筹学各个方向被广泛应用。

下面我仅提一个其在计算机视觉、图像分割（找图像中object的轮廓）领域的应用 -- Multicuts。

[Globally Optimal Image Partitioning by Multicuts](https://link.zhihu.com/?target=http%3A//ipa.iwr.uni-heidelberg.de/ipabib/Papers/kappes-emmcvpr2011.pdf) (主要贡献及其拓展出自海德堡大学团队).

而我的博士论文，也大量用到该方法，同样是应用在图像分割问题。

该图像分割问题被数学建模成一个基于图G(V,E)的能量函数最小值优化问题，其中multicut constraints的个数是指数级的，因此当只能采用割平面方法add on-the-fly.

以下是Multicut Problem的简短陈述，摘自：[A First Derivative Potts Model for Segmentation and Denoising Using ILP](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1709.07212.pdf)
![](https://pic4.zhimg.com/v2-0a920984419bf87919922e2f09cced27_b.jpg)
上面公式中，x_e是一个布尔变量，当它为1时, 即是俩个segment的临界处（分割线--下图中黄色轮廓）。这里仅po几张实验效果图，如有兴趣, paper链接：

[Globally Optimal Image Partitioning by Multicuts](https://link.zhihu.com/?target=http%3A//ipa.iwr.uni-heidelberg.de/ipabib/Papers/kappes-emmcvpr2011.pdf)
![](https://pic1.zhimg.com/v2-efee9d75b85981424aa61badddb5b660_b.jpg)
如果你和一年多前的我一样，是AI、计算机视觉领域的小白，那么下面的文章可能会对你有用：

[大话“人工智能、数据科学、机器学习”-- 综述](https://zhuanlan.zhihu.com/p/26645993)

如果你是运筹学/人工智能硕博或在读，请在下图的公众号后台留言：**“加微信群”**。系统会自动辨认你的关键字，并提示您进一步的加群要求和步骤，邀请您进全球运筹或AI学者群（群内学界、业界大佬云集）。

同时我们有：【**运筹学|优化爱好者**】【**供应链|物流**】【**人工智能**】【**数据科学|分析**】千人QQ群，想入群的小伙伴可以关注下方公众号**点击“加入社区”按钮**，获得入群传送门。

学术界|工业界招聘、征稿等信息**免费发布**，请见下图：
![](https://pic4.zhimg.com/v2-a14c8a3a69168fd8df72a8bff545613b_b.jpg)


# 关于KKT条件的深入探讨



[文雨之](https://www.zhihu.com/people/wen-yu-zhi-37)

运筹优化博士，机器学习半吊子



## **0. KKT 条件的历史背景**

KKT条件的发现还有一段历史小插曲。1951年Kuhn和Tucker发现了KKT条件并撰写了论文将其正式发表出来[1]，引起了很多学者的重视。自此之后一些学者发现早在1939年Karush在其硕士学位论文[2]里边已经给出了KKT条件，只是由于当时没有引起研究者的广泛关注而已。因此Kuhn ，Tucker，Karush 三位都作为独立发现KKT条件的学者，这个最优性条件就以他们三个人的名字来命名。



## **1. 无约束优化问题最优性条件**

我们先从无约束优化问题的最优性条件开始谈起，有如下无约束的优化问题

$\mathop {\min }\limits_x f\left( x \right)​$

若 $f\left( x \right)$可微，则其最优解的一阶必要条件为

$ \nabla f\left( x \right) = 0 $

这个条件的推导过程可以由泰勒展开得到[3, page 16]，这里不再赘述。



## **2. 有约束优化问题最优性条件**

下面考虑如下带约束的优化问题

$\begin{array}{l} \mathop {\min }\limits_x f\left( x \right)\\ s.t.\;\;\;{g_i}\left( x \right) \le 0,\;\;i = 1,2, \cdots ,m\\ \;\;\;\;\;\;\;{h_i}\left( x \right) = 0,\;\;i = 1,2, \cdots ,n \end{array}​$2      （1.1）

**其中 $f,{g_1}, \cdots ,{g_m},{h_1}, \cdots ,{h_n} $ 可微且一阶导数连续，存在非负实数 ${\mu _1}, \cdots {\mu _m}t $ 和实数 ${\lambda _1}, \cdots ,{\lambda _n} $，若 $ {x^*}$ 为局部最优解则以下条件成立：**

$ \begin{array}{l} \nabla f\left( {{x^*}} \right) + \sum\limits_{i = 1}^m {\mu _i^*\nabla{g_i}\left( {{x^*}} \right) + \sum\limits_{i = 1}^n {\lambda _i^*\nabla{h_i}\left( {{x^*}} \right)} } = 0\\ {h_i}\left( {{x^*}} \right) = 0,\;\;\;\;\;\;\;i = 1,2, \cdots ,n\\ {g_i}\left( {{x^*}} \right) \le 0,\;\;\;\;\;\;\;i = 1,2, \cdots ,m\\ \mu _i^* \ge 0,\;\;\;\;\;\;\;\;\;\;\;\;\;i = 1,2, \cdots ,m\\ \mu _i^*{g_i}\left( {{x^*}} \right) = 0,\;\;\;i = 1,2, \cdots ,m \end{array} ​$

至于该条件如何被推导出可以参考文末的参考文献[3, page 210]。你也可以将无约束优化问题的最优性条件当作KKT的特殊情况。一般情况下，我们认为KKT条件是带约束最优化问题的必要条件。这里需要重申一点就是最后一个条件 $\mu _i^*{g_i}\left( {{x^*}} \right) = 0 $ ，这意味着 $ \mu _i^* = 0,{g_i}\left( {{x^*}} \right) = 0$ ​这两者中必有一个为0，当 $ {g_i}\left( {{x^*}} \right) = 0 $ 时该约束为起作用约束，定义 $ I\left( {{x^*}} \right)  $ 为起作用约束集合。当 $\mu _i^* = 0$ 时该约束为不起作用约束，也就是说此时 ${{x^*}}$ 并未到达边界上，因此即使去掉该约束也不会影响最优解的取值。

**貌似这是一般情况下我们熟知的KKT条件，事实上，上面陈述的KKT条件并不完全正确（严谨），还缺少一个regularity条件。**

**regularity 条件（也被成为正则性条件或者约束规范）如下**

**考虑（1.1）的优化问题，其中 $f,{g_1}, \cdots ,{g_m},{h_1}, \cdots ,{h_n}$ 可微且一阶导数连续，可行解 ${x^\*}$满足 $\left\{ {\nabla {g_i}\left( {{x^*}} \right):i \in I\left( {{x^*}} \right)} \right\}\bigcup {\left\{ {\nabla {h_j}\left( {{x^*}} \right):j = 1,2, \cdots ,n} \right\}} $是线性独立的。**

**也就是说在使用KKT条件之前需要验证regularity条件，否则就无法保证KKT条件给出的结论一定成立**。





## **3. 缺少regularity条件KKT还是最优解的必要条件吗？**

若满足regularity条件，KKT就是最优解的必要条件。所谓必要条件的意思是满足KKT条件的不一定是最优解（例如鞍点就满足KKT，但鞍点就不是最优解），但是如果不满足KKT条件就一定不是最优解。本章我会用一个具体的例子来说明，缺少了regularity条件，KKT条件就并非最优解的必要条件。下面我就来展开分析一下这种特殊情况

$\begin{array}{l} \mathop {\min }\limits_x x\\ s.t.\;\;{x^2} \le 0 \end{array}$

**${x^\*} = 0​$为该约束优化问题的局部最优解但其并不满足regularity条件。**

**因为 $ {\rm{\{ }}\nabla {{\rm{g}}_{\rm{i}}}{\rm{(}}{{\rm{x}}^{\rm{*}}}{\rm{): i}} \in {\rm{I(}}{{\rm{x}}^{\rm{*}}}{\rm{)\} }} = {\rm{\{ }}\nabla {{\rm{g}}_{\rm{i}}}{\rm{(}}{{\rm{x}}^{\rm{*}}}{\rm{):}}{{\rm{g}}_{\rm{i}}}{\rm{(}}{{\rm{x}}^{\rm{*}}}{\rm{)}} = {\rm{0\} }} = {\rm{\{ 0\} }} ​$是一个线性相关集。**



在该问题里边目标函数 $\nabla f\left( {{x^*}} \right) = 1$， $g\left( {{x^*}} \right) = 0$，同时等式约束也没有。显而易见KKT的第一个等式 $\nabla f\left( {{x^*}} \right) + \sum\limits_{i = 1}^m {\mu _i^*\nabla{g_i}\left( {{x^*}} \right) + \sum\limits_{i = 1}^n {\lambda _i^*\nabla{h_i}\left( {{x^*}} \right)} } = 0$ 是不可能成立。

进一步分析一下，造成其不满足KKT条件的原因是什么呢？是由于约束过于苛刻了，导致整个可行域只有一个单点。此时该约束优化问题的最优解无论其目标函数是什么样，其最优解都是 ![\[{x^*} = 0\]](https://www.zhihu.com/equation?tex=%5C%5B%7Bx%5E%2A%7D+%3D+0%5C%5D) ，也就是说其最优解与目标函数没有任何关系。正是这种非常罕见的退化情况导致了KKT条件无法适用。

学过运筹优化的同学应该对另外一个和KKT很相近的定理有记忆，它就是Fritz John 条件。考虑（1.1）的优化问题，其中 ![\[f,{g_1}, \cdots ,{g_m},{h_1}, \cdots ,{h_n}\]](https://www.zhihu.com/equation?tex=%5C%5Bf%2C%7Bg_1%7D%2C+%5Ccdots+%2C%7Bg_m%7D%2C%7Bh_1%7D%2C+%5Ccdots+%2C%7Bh_n%7D%5C%5D) 可微且一阶导数连续，存在不全为0的非负实数 ![\[{\mu _0},{\mu _1}, \cdots {\mu _m}\]](https://www.zhihu.com/equation?tex=%5C%5B%7B%5Cmu+_0%7D%2C%7B%5Cmu+_1%7D%2C+%5Ccdots+%7B%5Cmu+_m%7D%5C%5D) 和实数 ![\[{\lambda _1}, \cdots ,{\lambda _n}\]](https://www.zhihu.com/equation?tex=%5C%5B%7B%5Clambda+_1%7D%2C+%5Ccdots+%2C%7B%5Clambda+_n%7D%5C%5D) ，若 ![\[{x^*}\]](https://www.zhihu.com/equation?tex=%5C%5B%7Bx%5E%2A%7D%5C%5D) 为局部最优解则以下条件成立：

![\[\begin{array}{l} {\mu _0}\nabla f\left( {{x^*}} \right) + \sum\limits_{i = 1}^m {\mu _i^*\nabla{g_i}\left( {{x^*}} \right) + \sum\limits_{i = 1}^n {\lambda _i^*\nabla{h_i}\left( {{x^*}} \right)} } = 0\\ {h_i}\left( {{x^*}} \right) = 0,\;\;\;\;\;\;\;i = 1,2, \cdots ,n\\ {g_i}\left( {{x^*}} \right) \le 0,\;\;\;\;\;\;\;i = 1,2, \cdots ,m\\ \mu _i^* \ge 0,\;\;\;\;\;\;\;\;\;\;\;\;\;i = 1,2, \cdots ,m\\ \mu _i^*{g_i}\left( {{x^*}} \right) = 0,\;\;\;i = 1,2, \cdots ,m \end{array}\]](https://www.zhihu.com/equation?tex=%5C%5B%5Cbegin%7Barray%7D%7Bl%7D+%7B%5Cmu+_0%7D%5Cnabla+f%5Cleft%28+%7B%7Bx%5E%2A%7D%7D+%5Cright%29+%2B+%5Csum%5Climits_%7Bi+%3D+1%7D%5Em+%7B%5Cmu+_i%5E%2A%5Cnabla%7Bg_i%7D%5Cleft%28+%7B%7Bx%5E%2A%7D%7D+%5Cright%29+%2B+%5Csum%5Climits_%7Bi+%3D+1%7D%5En+%7B%5Clambda+_i%5E%2A%5Cnabla%7Bh_i%7D%5Cleft%28+%7B%7Bx%5E%2A%7D%7D+%5Cright%29%7D+%7D+%3D+0%5C%5C+%7Bh_i%7D%5Cleft%28+%7B%7Bx%5E%2A%7D%7D+%5Cright%29+%3D+0%2C%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3Bi+%3D+1%2C2%2C+%5Ccdots+%2Cn%5C%5C+%7Bg_i%7D%5Cleft%28+%7B%7Bx%5E%2A%7D%7D+%5Cright%29+%5Cle+0%2C%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3Bi+%3D+1%2C2%2C+%5Ccdots+%2Cm%5C%5C+%5Cmu+_i%5E%2A+%5Cge+0%2C%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3Bi+%3D+1%2C2%2C+%5Ccdots+%2Cm%5C%5C+%5Cmu+_i%5E%2A%7Bg_i%7D%5Cleft%28+%7B%7Bx%5E%2A%7D%7D+%5Cright%29+%3D+0%2C%5C%3B%5C%3B%5C%3Bi+%3D+1%2C2%2C+%5Ccdots+%2Cm+%5Cend%7Barray%7D%5C%5D)

仔细对比KKT条件和Fritz John条件，发现两者的区别是Fritz John conditions多了一个对目标函数的乘子，恰恰可以把上面那种退化的情况涵盖进去。如下式所示：

![\[{\mu _0}\nabla f\left( {{x^*}} \right) + \sum\limits_{i = 1}^m {\mu _i^*\nabla{g_i}\left( {{x^*}} \right) + \sum\limits_{i = 1}^n {\lambda _i^*\nabla{h_i}\left( {{x^*}} \right)} } = 0\]](https://www.zhihu.com/equation?tex=%5C%5B%7B%5Cmu+_0%7D%5Cnabla+f%5Cleft%28+%7B%7Bx%5E%2A%7D%7D+%5Cright%29+%2B+%5Csum%5Climits_%7Bi+%3D+1%7D%5Em+%7B%5Cmu+_i%5E%2A%5Cnabla%7Bg_i%7D%5Cleft%28+%7B%7Bx%5E%2A%7D%7D+%5Cright%29+%2B+%5Csum%5Climits_%7Bi+%3D+1%7D%5En+%7B%5Clambda+_i%5E%2A%5Cnabla%7Bh_i%7D%5Cleft%28+%7B%7Bx%5E%2A%7D%7D+%5Cright%29%7D+%7D+%3D+0%5C%5D)

针对上面那个退化情况，只需让 ![\[{\mu _0} = 0\]](https://www.zhihu.com/equation?tex=%5C%5B%7B%5Cmu+_0%7D+%3D+0%5C%5D) 即可。 ![\[{\mu _0} = 0\]](https://www.zhihu.com/equation?tex=%5C%5B%7B%5Cmu+_0%7D+%3D+0%5C%5D) 实际上和 ![\[\lambda {\rm{ = 0}}\]](https://www.zhihu.com/equation?tex=%5C%5B%5Clambda+%7B%5Crm%7B+%3D+0%7D%7D%5C%5D) 的意义是相似的，说明目标函数是不起作用的，也就是无论目标函数怎么变化也对最优解没有影响。反过来再对照我们构造的特殊情况确实也是印证了上面所说的这一点。由此我们得到了对于我们举出的退化问题，存在 ![\[{x^*} = 0\]](https://www.zhihu.com/equation?tex=%5C%5B%7Bx%5E%2A%7D+%3D+0%5C%5D) 为其最优解，但是却不满足KKT条件的情况。那么去掉regularity条件的KKT条件是最优解的必要条件实际上是不成立的。实际上Fritz John条件才是真正的必要条件，KKT是刨除了不满足regularity条件情况的前提下才能称为最优解的必要条件。易知，若 ![\[{\mu _0} \ne 0\]](https://www.zhihu.com/equation?tex=%5C%5B%7B%5Cmu+_0%7D+%5Cne+0%5C%5D) 我们在Fritz John条件等式两边同时除以 ![\[{\mu _0}\]](https://www.zhihu.com/equation?tex=%5C%5B%7B%5Cmu+_0%7D%5C%5D) 就可以得到KKT条件。

**因此在应用KKT条件的时候，一定需要首先检验regularity条件。**



## **4. KKT(最优性条件)为什么这么重要？**

我们经常会提到最优性条件，也经常会用到最优性条件，为啥它就那么重要呢？如果一个优化问题有最优性条件的话，那这个优化问题的性质实际上是比较好的。我目前能想到的是以下两个原因：

1 通过最优性条件可以比较容易的验证任意的一个解是不是最优解，例如KKT条件，它是最优解的必要条件，它就可以把可行域里边很多的不是最优解的解轻松的排除掉，让我们仅仅在满足必要条件（KKT条件）的解里边进一步寻找真正的最优解。混合整数规划问题比较困难的原因之一就是很难找到最优性条件，很多情况下即使验证一个解是不是最优解都比较困难。

2 最优性条件可以指导算法的设计。例如对于无约束可微分的优化问题，我们采用梯度法，牛顿法，拟牛顿法等，其收敛性的证明都是证明最终算法能收敛到导数等于0的地方。所以算法的设计都是考虑如何能够收敛到最优性条件去，这样在很多情况下比直接去求解极值要容易的多。



## **5. KKT和凸优化的关系是什么？**

KKT主要是针对带约束的可微分的优化问题，凸优化研究的对象是目标函数为凸函数，约束为凸集的优化问题。因此这两者研究的对象，有交集，也各有不同。

第一类问题为两类问题的交集即带约束的可微分凸优化问题，这类问题目前已经被很好的解决了，它同时具备两类问题的性质，凸优化和可微分性，让原来KKT从局部最优解的必要条件变为全局最优解的充要条件。

第二类问题是凸优化但是不可微分，这类问题也较为常见，在拉格朗日松弛算法中，对偶问题一般都是不可微分的凸优化问题，因为不可微分，传统的基于梯度的方法就不适用了，一般采用次梯度的方法，主要难点在于次梯度如何确定，由于次梯度不唯一，如何确定一个简单有效的次梯度也是一个问题。

第三类问题是可微分的但不是凸优化的，这类问题也很多，一般这类问题都可以采用基于梯度的算法来求解，例如对神经网络的训练多数就属于这类问题。采用梯度法仅仅能保证收敛到局部最优的必要条件而已。因此该类问题的受困于陷入鞍点和全局最优的寻找是很困难的。



总结

**1 去掉regularity条件的KKT条件严格来讲并非最优解的必要条件。**

2 有最优性条件对优化问题而言是一个较好的性质。



参考文献

[1] [Kuhn, H. W.](http://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Harold_W._Kuhn); [Tucker, A. W.](http://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Albert_W._Tucker) (1951). ["Nonlinear programming"](http://link.zhihu.com/?target=http%3A//projecteuclid.org/euclid.bsmsp/1200500249). *Proceedings of 2nd Berkeley Symposium*. Berkeley: University of California Press. pp. 481–492. [MR](http://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Mathematical_Reviews) [0047303](http://link.zhihu.com/?target=https%3A//www.ams.org/mathscinet-getitem%3Fmr%3D0047303).

[2] W. Karush (1939). "Minima of Functions of Several Variables with Inequalities as Side Constraints". M.Sc. Dissertation. Dept. of Mathematics, Univ. of Chicago, Chicago, Illinois.

[3] Amir Beck, Introduction to NLP: theory, algorithms, and applications with Matlab, 2014.



在此感谢[『运筹OR帷幄』](https://zhuanlan.zhihu.com/operations-research)审稿人对本文提出了宝贵的意见。

[『运筹OR帷幄』](https://zhuanlan.zhihu.com/operations-research)审稿人 @[Xiaoxi Li](http://link.zhihu.com/?target=https%3A//sites.google.com/site/xiaoxilijeux/curriculum)，巴黎六大数学博士、武汉大学助理教授，研究方向：博弈论。

以上[『运筹OR帷幄』](https://zhuanlan.zhihu.com/operations-research)专栏所有文章都会同步发送至 [留德华叫兽的头条主页](http://link.zhihu.com/?target=https%3A//www.toutiao.com/c/user/62215957061/%23mid%3D1570625219325954)， 以及同名微信公众号，目前预计受众**10w +**
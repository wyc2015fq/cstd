# (转载)机器学习知识点(十四)EM算法原理 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月03日 12:02:12[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2162
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









1.引言

以前我们讨论的概率模型都是只含观测变量(observable variable), 即这些变量都是可以观测出来的，那么给定数据，可以直接使用极大似然估计的方法或者贝叶斯估计的方法；但是当模型含有隐变量(latent
 variable)的时候, 就不能简单地使用这些估计方法。

如在[高斯混合和EM算法](http://www.cnblogs.com/90zeng/p/Mixtures_of_Gaussians_and_the_EM_algorithm.html)中讨论的高斯混合就是典型的含有隐变量的例子，已经给出EM算法在高斯混合模型中的运用，下面我们来讨论一些原理性的东西。



2.Jensen 不等式

令![](http://images.cnitblog.com/blog/659319/201411/201113317032270.jpg)是值域为实数的函数，那么如果![](http://images.cnitblog.com/blog/659319/201411/201114194842306.jpg)，则![](http://images.cnitblog.com/blog/659319/201411/201113317032270.jpg)就是一个凸函数，如果自变量
 x 是向量, 那么当函数的海森矩阵 ![](http://images.cnitblog.com/blog/659319/201411/201116388438329.jpg)是半正定时(![](http://images.cnitblog.com/blog/659319/201411/201117062509324.jpg)), ![](http://images.cnitblog.com/blog/659319/201411/201113317032270.jpg)是凸函数，这是函数为凸函数的条件在向量输入时的泛化。

如果![](http://images.cnitblog.com/blog/659319/201411/201119196566368.jpg)，则称![](http://images.cnitblog.com/blog/659319/201411/201113317032270.jpg)是严格凸函数，对应的向量输入时的泛化是![](http://images.cnitblog.com/blog/659319/201411/201120306256387.jpg).

> 
定理  令![](http://images.cnitblog.com/blog/659319/201411/201113317032270.jpg)是一个凸函数，令![](http://images.cnitblog.com/blog/659319/201411/201122559061636.jpg)是一个随机变量，那么

![](http://images.cnitblog.com/blog/659319/201411/201124244681912.jpg)


当![](http://images.cnitblog.com/blog/659319/201411/201113317032270.jpg)时严格凸函数的时，当且仅当![](http://images.cnitblog.com/blog/659319/201411/201128476259817.jpg) 以概率
 1 成立的时，![](http://images.cnitblog.com/blog/659319/201411/201129262818674.jpg). 即当![](http://images.cnitblog.com/blog/659319/201411/201122559061636.jpg)时常量时，上面不等式的等号成立。

注意上面 E 是表示期望的意思，习惯上，在写变量期望的时候，会把紧跟括号略去，即![](http://images.cnitblog.com/blog/659319/201411/201134168289643.jpg).

用下面的图对上面的定理作一个解释：

![](http://images.cnitblog.com/blog/659319/201411/201135459689576.jpg)

这个图中的实线代表凸函数![](http://images.cnitblog.com/blog/659319/201411/201113317032270.jpg), 随机变量![](http://images.cnitblog.com/blog/659319/201411/201122559061636.jpg)有
 0.5 的概率取 a, 同样以 0.5 的概率取 b, 所以![](http://images.cnitblog.com/blog/659319/201411/201122559061636.jpg)的期望位于a,b的正中间，即a,b的均值.

从图中可以看出，在 y 轴上, ![](http://images.cnitblog.com/blog/659319/201411/201141431561055.jpg)位于![](http://images.cnitblog.com/blog/659319/201411/201142243283710.jpg)之间，因为![](http://images.cnitblog.com/blog/659319/201411/201113317032270.jpg)是凸函数，则必如上图所示，![](http://images.cnitblog.com/blog/659319/201411/201124244681912.jpg)

所以很多情况下，许多人并去记忆这个不等式，而是记住上面的图，这样更容易理解。

注意：如果![](http://images.cnitblog.com/blog/659319/201411/201113317032270.jpg)是（严格）凹函数，即![](http://images.cnitblog.com/blog/659319/201411/201149146258562.jpg)使（严格）凸函数（即，![](http://images.cnitblog.com/blog/659319/201411/201149561567173.jpg)），那么Jensen不等式照样成立，只不过不等号方向相反：![](http://images.cnitblog.com/blog/659319/201411/201151393595911.jpg)![](http://images.cnitblog.com/blog/659319/201411/201151468434390.jpg)



3.EM算法

假设在一个估计问题中有m个独立样本![](http://images.cnitblog.com/blog/659319/201411/201217264536532.jpg)，根据这些数据，希望拟合出模型![](http://images.cnitblog.com/blog/659319/201411/201218296401928.jpg)的参数，那么对数似然函数：

![](http://images.cnitblog.com/blog/659319/201411/201219288433240.jpg)

这里，![](http://images.cnitblog.com/blog/659319/201411/201220481098324.jpg)是隐变量，如果![](http://images.cnitblog.com/blog/659319/201411/201220481098324.jpg)能够被观测出来，最大似然估计就会变得很容易，但是现在![](http://images.cnitblog.com/blog/659319/201411/201220481098324.jpg)观测不出来，是隐变量。

在这种情况下，EM算法给出了一种很有效的最大似然估计的方法：重复地构造![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)的下界（E步），然后最大化这个下界（M步）。



对于每个![](http://images.cnitblog.com/blog/659319/201411/201227243286845.jpg)，令![](http://images.cnitblog.com/blog/659319/201411/201227528599694.jpg)表示隐变量![](http://images.cnitblog.com/blog/659319/201411/201228292188452.jpg)的分布，即![](http://images.cnitblog.com/blog/659319/201411/201229154844219.jpg)![](http://images.cnitblog.com/blog/659319/201411/201230070466865.jpg)，考虑：

![](http://images.cnitblog.com/blog/659319/201411/201231058288708.jpg)

由（2）到（3）的推导用到了上面的Jensen不等式，此时![](http://images.cnitblog.com/blog/659319/201411/201236111257639.jpg)![](http://images.cnitblog.com/blog/659319/201411/201236426875729.jpg)是一个凹函数，因为![](http://images.cnitblog.com/blog/659319/201411/201237303121279.jpg)，考虑上面关于![](http://images.cnitblog.com/blog/659319/201411/201228292188452.jpg)的分布![](http://images.cnitblog.com/blog/659319/201411/201227528599694.jpg)，

![](http://images.cnitblog.com/blog/659319/201411/201238583902871.jpg)

正好是数量![](http://images.cnitblog.com/blog/659319/201411/201240317811274.jpg)的期望，由Jensen不等式可以得到：

![](http://images.cnitblog.com/blog/659319/201411/201242274213643.jpg)

由此可以从（2）推出（3）.



但是由于隐变量的存在，直接最大化![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)很困难！试想如果能让![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)直接与它的下界相等，那么任何可以使![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)的下界增大的![](http://images.cnitblog.com/blog/659319/201411/201300522509380.jpg)，也可以使![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)增大，所以自然就是选择出使![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)的下界达到极大的参数![](http://images.cnitblog.com/blog/659319/201411/201300522509380.jpg).

怎么样才能使得![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)取得下界呢，即上面不等式取等号，关键在于隐变量![](http://images.cnitblog.com/blog/659319/201411/201308243289300.jpg)如何处理，下面就此讨论。

现在，对于任意的分布![](http://images.cnitblog.com/blog/659319/201411/201227528599694.jpg)，（3）给出了似然函数![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)的下界.
 对于分布![](http://images.cnitblog.com/blog/659319/201411/201227528599694.jpg)到底是什么分布，可以有很多种选择，到底该选择哪一种呢？



在上面讨论Jensen不等式的时候可以看出，不等式中等号成立的条件是随机变量变成“常量”，对于![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)要想取得下界值，必须要求

![](http://images.cnitblog.com/blog/659319/201411/201307326562454.jpg)

其中常数 c 与变量![](http://images.cnitblog.com/blog/659319/201411/201308243289300.jpg) 无关，这很容易做到，我们选择分布![](http://images.cnitblog.com/blog/659319/201411/201227528599694.jpg)的时候，满足下面的条件即可：

![](http://images.cnitblog.com/blog/659319/201411/201310076719294.jpg)

由于![](http://images.cnitblog.com/blog/659319/201411/201310516716718.jpg)，于是我们可以知道：

![](http://images.cnitblog.com/blog/659319/201411/201311382343928.jpg)

注意理解上面这个等式式子是如何得出来的！！

于是就可以把分布![](http://images.cnitblog.com/blog/659319/201411/201227528599694.jpg)设定为：在参数![](http://images.cnitblog.com/blog/659319/201411/201300522509380.jpg)下，给定![](http://images.cnitblog.com/blog/659319/201411/201315337345381.jpg)后，![](http://images.cnitblog.com/blog/659319/201411/201316057505101.jpg)的后验分布。

这样设定好隐变量的分布![](http://images.cnitblog.com/blog/659319/201411/201227528599694.jpg)之后，![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)就直接取其下界，原来最大化似然函数![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)的问题转换为最大化其下界，这就是E步！

在M步中，就是去调整参数![](http://images.cnitblog.com/blog/659319/201411/201300522509380.jpg)最大化上面提到的式子（3）.

不断重复E步和M步就是EM算法：

重复迭代直至收敛{

![](http://images.cnitblog.com/blog/659319/201411/201327135935903.jpg)

}



我们如何知道算法收敛呢？

假如![](http://images.cnitblog.com/blog/659319/201411/201330025008535.jpg)和![](http://images.cnitblog.com/blog/659319/201411/201330106567643.jpg)是两次连续迭代后的参数,需要证明![](http://images.cnitblog.com/blog/659319/201411/201331228757947.jpg).

正如上面所述，由于我们再选择分布![](http://images.cnitblog.com/blog/659319/201411/201227528599694.jpg)时，选择：![](http://images.cnitblog.com/blog/659319/201411/201333052659098.jpg)，于是：

![](http://images.cnitblog.com/blog/659319/201411/201333502502824.jpg)

参数![](http://images.cnitblog.com/blog/659319/201411/201330106567643.jpg)就是通过极大化上面右边的式子得出，因此：

![](http://images.cnitblog.com/blog/659319/201411/201335141405160.jpg)

注意第不等式（4）来自于：

![](http://images.cnitblog.com/blog/659319/201411/201337036098550.jpg)



这个式子对于任意的![](http://images.cnitblog.com/blog/659319/201411/201227528599694.jpg)和![](http://images.cnitblog.com/blog/659319/201411/201300522509380.jpg)都成立，当然对于![](http://images.cnitblog.com/blog/659319/201411/201339551252252.jpg)和![](http://images.cnitblog.com/blog/659319/201411/201340027818445.jpg)也成立。对于不等式（5），因为![](http://images.cnitblog.com/blog/659319/201411/201330106567643.jpg)是通过如下极大化过程选出来的：

![](http://images.cnitblog.com/blog/659319/201411/201342260936795.jpg)

所以在![](http://images.cnitblog.com/blog/659319/201411/201330106567643.jpg)处，式子的值要比在![](http://images.cnitblog.com/blog/659319/201411/201330025008535.jpg)处式子的值要大！

式子（6）是通过上面讨论过的方法选择出合适的![](http://images.cnitblog.com/blog/659319/201411/201347481099585.jpg)使得Jensen不等式取等号！

因此，EM算法使得似然函数单调收敛。在上面描述EM算法的时候，说是“重复迭代直至收敛”，一个常用的检查收敛的方法是：如果两次连续迭代之后，似然函数![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)的值变化很小（在某个可容忍的范围内），就EM算法中![](http://images.cnitblog.com/blog/659319/201411/201225010005266.jpg)的变化已经很慢，可以停止迭代了。



注意：如果定义：

![](http://images.cnitblog.com/blog/659319/201411/201353460626961.jpg)

从之前的推导，我们知道![](http://images.cnitblog.com/blog/659319/201411/201354185462137.jpg). EM算法看作是关于函数
 J 的梯度上升：E步是关于参数Q，M步是关于参数![](http://images.cnitblog.com/blog/659319/201411/201300522509380.jpg).





4.高斯混合的修正

在 [高斯混合和EM算法](http://www.cnblogs.com/90zeng/p/Mixtures_of_Gaussians_and_the_EM_algorithm.html) 中，我们将EM算法用于优化求解高斯混合模型，拟合参数![](http://images.cnitblog.com/blog/659319/201410/301747358008684.jpg)和![](http://images.cnitblog.com/blog/659319/201410/301747429405437.jpg).

E步：

![](http://images.cnitblog.com/blog/659319/201411/201401042502494.jpg)

这里![](http://images.cnitblog.com/blog/659319/201411/201402215465977.jpg)表示的是在分布![](http://images.cnitblog.com/blog/659319/201411/201227528599694.jpg)下，![](http://images.cnitblog.com/blog/659319/201411/201402497035344.jpg)取![](http://images.cnitblog.com/blog/659319/201411/201402594532165.jpg)的概率。

M步：考虑参数![](http://images.cnitblog.com/blog/659319/201411/201404052502331.jpg)，最大化数值：

![](http://images.cnitblog.com/blog/659319/201411/201404408755947.jpg)



最大化求![](http://images.cnitblog.com/blog/659319/201411/201407040621569.jpg)，对上面的式子关于![](http://images.cnitblog.com/blog/659319/201411/201407040621569.jpg)求偏导数：

![](http://images.cnitblog.com/blog/659319/201411/201408244534209.jpg)

令这个偏导数为0，求出![](http://images.cnitblog.com/blog/659319/201411/201407040621569.jpg)的更新方式：

![](http://images.cnitblog.com/blog/659319/201411/201409352188055.jpg)

这是在 [高斯混合和EM算法](http://www.cnblogs.com/90zeng/p/Mixtures_of_Gaussians_and_the_EM_algorithm.html) 中已经得出的结论。

再考虑如何更新参数![](http://images.cnitblog.com/blog/659319/201411/201412501563968.jpg),把只与![](http://images.cnitblog.com/blog/659319/201411/201412501563968.jpg)有关的项写出来，发现只需要最大化：

![](http://images.cnitblog.com/blog/659319/201411/201414128129249.jpg)

因为，![](http://images.cnitblog.com/blog/659319/201411/201415489217220.jpg)，所有![](http://images.cnitblog.com/blog/659319/201411/201412501563968.jpg)的和为1，所以这是一个约束优化问题，参考[简易解说拉格朗日对偶（Lagrange
 duality）](http://www.cnblogs.com/90zeng/p/Lagrange_duality.html)，构造拉格朗日函数：

![](http://images.cnitblog.com/blog/659319/201411/201417171257241.jpg)



其中 β 是拉格朗日乘子. 求偏导数：

![](http://images.cnitblog.com/blog/659319/201411/201418322509057.jpg)

令偏导数为0，得到：

![](http://images.cnitblog.com/blog/659319/201411/201419272963872.jpg)

即：![](http://images.cnitblog.com/blog/659319/201411/201421219377116.jpg)利用约束条件：![](http://images.cnitblog.com/blog/659319/201411/201421455466569.jpg)，得到：![](http://images.cnitblog.com/blog/659319/201411/201422332964848.jpg)(注意这里用到：![](http://images.cnitblog.com/blog/659319/201411/201424373288956.jpg)).

于是可以得到参数![](http://images.cnitblog.com/blog/659319/201411/201412501563968.jpg)的更新规则：

![](http://images.cnitblog.com/blog/659319/201411/201426017184207.jpg)

关于参数![](http://images.cnitblog.com/blog/659319/201410/301747429405437.jpg)的更新规则，以及整个EM算法如何运用到高斯混合模型的优化，请参考：[高斯混合和EM算法](http://www.cnblogs.com/90zeng/p/Mixtures_of_Gaussians_and_the_EM_algorithm.html)！

5.总结

所谓EM算法就是在含有隐变量的时候，把隐变量的分布设定为一个以观测变量为前提条件的后验分布，使得参数的似然函数与其下界相等，通过极大化这个下界来极大化似然函数，从避免直接极大化似然函数过程中因为隐变量未知而带来的困难！EM算法主要是两步,E步选择出合适的隐变量分布（一个以观测变量为前提条件的后验分布），使得参数的似然函数与其下界相等；M步：极大化似然函数的下界，拟合出参数.

转载：http://www.cnblogs.com/90zeng/p/EM_algorithm_theory.html








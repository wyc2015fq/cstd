# 一文详解蒙特卡洛（Monte Carlo）法及其应用 - Machine Learning with Peppa - CSDN博客





2018年01月12日 17:42:01[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：15987
所属专栏：[机器学习与数据挖掘](https://blog.csdn.net/column/details/18961.html)[有趣的算法](https://blog.csdn.net/column/details/19022.html)









### 概述

蒙特卡罗方法是一种计算方法。原理是通过大量随机样本，去了解一个系统，进而得到所要计算的值。


它非常强大和灵活，又相当简单易懂，很容易实现。对于许多问题来说，它往往是最简单的计算方法，有时甚至是唯一可行的方法。它诞生于上个世纪40年代美国的"曼哈顿计划"，名字来源于赌城蒙特卡罗，象征概率。




### π的计算

第一个例子是，如何用蒙特卡罗方法计算圆周率π。正方形内部有一个相切的圆，它们的面积之比是π/4。


![](http://www.ruanyifeng.com/blogimg/asset/2015/bg2015072611.jpg)

![](http://www.ruanyifeng.com/blogimg/asset/2015/bg2015072603.jpg)


现在，在这个正方形内部，随机产生10000个点（即10000个坐标对 (x, y)），计算它们与中心点的距离，从而判断是否落在圆的内部。

![](https://img-blog.csdn.net/20180112174309308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果这些点均匀分布，那么圆内的点应该占到所有点的 π/4，因此将这个比值乘以4，就是π的值。通过R语言脚本随机模拟30000个点，π的估算值与真实值相差0.07%。


## 无意识统计学家法则（Law of the unconscious statistician）




这是本文后续会用到的一个定理。作为一个预备知识，我们首先来介绍一下它。先来看一下维基百科上给出的解释。 

In probability theory and statistics, the law of the unconscious statistician (sometimes abbreviated LOTUS) is a theorem used to calculate the 期望值 of a function  of
 a 随机变量  when
 one knows the probability distribution of  but
 one does not explicitly know the distribution of .
 The form of the law can depend on the form in which one states the probability distribution of the 随机变量 .
- If it is a discrete distribution and one knows its PMF function  (but
 not ),
 then the 期望值 of  is


where the sum is over all possible values  of .
- If it is a continuous distribution and one knows its PDF function  (but
 not ),
 then the 期望值 of  is




LOTUS到底表达了一件什么事呢？它的意思是：已知随机变量的概率分布，但不知道的分布，此时用LOTUS公式能计算出函数的数学期望。LOTUS的公式如下：
- 是离散分布时


- 是连续分布时




其实就是在计算期望时，用已知的的PDF（或PMF）代替未知的的PDF（或PMF）。

## 蒙特卡洛求定积分（一）：投点法


这个方法也常常被用来求值。现在我们用它来求函数的定积分。如下图所示，有一个函数，若要求它从到的定积分，其实就是求曲线下方的面积。这时我们可以用一个比较容易算得面积的矩型罩在函数的积分区间上（假设其面积为）。然后随机地向这个矩形框里面投点，其中落在函数下方的点为绿色，其它点为红色。然后统计绿色点的数量占所有点（红色+绿色）数量的比例为，那么就可以据此估算出函数从到的定积分为。



![](https://img-blog.csdn.net/20161225135139841?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpbWFmdWppbmpp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



注意由蒙特卡洛法得出的值并不是一个精确之，而是一个近似值。而且当投点的数量越来越大时，这个近似值也越接近真实值。

## 蒙特卡洛求定积分（二）：期望法


下面我们来重点介绍一下利用蒙特卡洛法求定积分的第二种方法——期望法，有时也成为平均值法。


任取一组相互独立、同分布的随机变量，在上服从分布律，也就是说是随机变量的PDF（或PMF），令，则也是一组独立同分布的随机变量，而且（因为是关于的函数，所以根据LOTUS可得） 




由强大数定理 



若选 



则依概率1收敛到。平均值法就用作为的近似值。



假设要计算的积分有如下形式


其中被积函数在区间内可积。任意选择一个有简便办法可以进行抽样的概率密度函数，使其满足下列条件：

- 当时，（）
- 


如果记 




那么原积分式可以写成 



因而求积分的步骤是：

- 产生服从分布律的随机变量；
- 计算均值


并用它作为的近似值，即。


如果为有限值，那么可取作为均匀分布： 




此时原来的积分式变为 



具体步骤如下：

- 产生上的均匀分布随机变量;
- 计算均值


并用它作为的近似值，即。

## 平均值法的直观解释


下面是来自参考文献【1】的一个例子。注意积分的几何意义就是[a,b]区间内曲线下方的面积。 


![](https://img-blog.csdn.net/20161225152329639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpbWFmdWppbmpp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
当我们在[a,b]之间随机取一点时，它对应的函数值就是，然后变可以用来粗略估计曲线下方的面积（也就是积分），当然这种估计（或近似）是非常粗略的。 

![](https://img-blog.csdn.net/20161225153247460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpbWFmdWppbmpp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
于是我们想到在之间随机取一系列点时（满足均匀分布），然后把估算出来的面积取平均来作为积分估计的一个更好的近似值。可以想象，如果这样的采样点越来越多，那么对于这个积分的估计也就越来越接近。 

![](https://img-blog.csdn.net/20161225153328839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpbWFmdWppbmpp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
按照上面这个思路，我们得到积分公式为 



注意其中的就是均匀分布的PMF。这跟我们之前推导出来的蒙特卡洛积分公式是一致的。


## 参考文献


【1】[http://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/monte-carlo-methods-in-practice/monte-carlo-integration](http://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/monte-carlo-methods-in-practice/monte-carlo-integration)





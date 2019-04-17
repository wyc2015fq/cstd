# 机器学习中的VC维数和分类 - Machine Learning with Peppa - CSDN博客





2017年12月22日 23:23:20[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1748
所属专栏：[机器学习与数据挖掘](https://blog.csdn.net/column/details/18961.html)









## VC维数




学习VC维要先知道的概念有：增长函数（growth function）、对分（dichotomy）、打散（shattering）和断点（break point）

1.增长函数


增长函数表示假设空间H对m个示例所能赋予标记的最大可能结果数。

> 

比如说现在数据集有两个数据点，考虑一种二分类的情况，可以将其分类成A或者B，则可能的值有：AA、AB、BA和BB，所以这里增长函数的值为4.



增长函数值越大则假设空间H的表示能力越强，复杂度也越高，学习任务的适应能力越强。不过尽管H中可以有无穷多的假设h，但是增长函数却不是无穷大的：对于m个示例的数据集，最多只能有![2^m](https://www.zhihu.com/equation?tex=2%5Em) 个标记结果，而且很多情况下也达不到![2^m](https://www.zhihu.com/equation?tex=2%5Em)的情况。

2.对分


对于二分类问题来说，H中的假设对D中m个示例赋予标记的每种可能结果称为对D的一种对分（dichotomy）。对分也是增长函数的一种上限。

3.打散


打散指的是假设空间H能实现数据集D上全部示例的对分，即增长函数=![2^m](https://www.zhihu.com/equation?tex=2%5Em)。但是认识到不打散是什么则更加重要——


有些情况下H的增长函数不可以达到对应的![2^m](https://www.zhihu.com/equation?tex=2%5Em) 值，比如说在二维实平面上的线性划分情况中，以下的情况就不可以线性可分（也就是说不能算作赋予标记的结果）：
![](http://118.126.108.52/wp-content/uploads/2017/12/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20171212133530-300x226.png)

或者下图这个
![](http://118.126.108.52/wp-content/uploads/2017/12/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20171212133537-300x210.png)

虽然图画的非常直击灵魂，但是你应该可以体会到这种情况下二维平面的线性分类器是不可以给上面的情况分类的（事实上对于任何集合，其![2^4](https://www.zhihu.com/equation?tex=2%5E4)=16种对分中至少有一种不能被线性划分实现
 ）

4.Vapink-Chervonenkis Dimension


现在可以引出VC维的定义了——

> 

假设空间H的VC维是能被H打散的最大的示例集（数据集）的大小，即有： ![VC(H)=max\{m:\prod(m)=2^m\}](https://www.zhihu.com/equation?tex=VC%28H%29%3Dmax%5C%7Bm%3A%5Cprod%28m%29%3D2%5Em%5C%7D) 其中![\prod(m)](https://www.zhihu.com/equation?tex=%5Cprod%28m%29)为假设空间在数据集大小为m时的增长函数。



或者有这种更平实的定义——

> 

对于一个假设空间H，如果存在m个数据样本能够被假设空间H中的函数按所有可能的![2^h](https://www.zhihu.com/equation?tex=2%5Eh) 种形式分开 ，则称假设空间H能够把m个数据样本打散（shatter）。假设空间H的VC维就是能打散的最大数据样本数目m。若对任意数目的数据样本都有函数能将它们shatter，则假设空间H的VC维为无穷大。



在上面那个4个点的图中，因为4个点的情况下以及不能做到对分，所以二维实平面上所有线性划分构成的假设空间H的VC维为3.

5.Break Point


在一些教课书中并没有提出Break Point的概念，这是林轩田《机器学习基石》公开课里的一种辅助概念。现在简单说一下break point的意义——我们希望假设空间H的增长函数越小越好（这样子假设空间比较简单），或者至少不要增长的太快——如果按照![2^m](https://www.zhihu.com/equation?tex=2%5Em)这种趋势增长那简直是没天理了。上面说道了，随着m的增大，一定会出现一个m使假设空间无法shatter。这种不满足![2^m](https://www.zhihu.com/equation?tex=2%5Em) 的情况说明增长函数从这个点开始变缓了，是一个可喜可贺的重大突破，所以我们把第一个不满足shatter的m值称为break
 point（这里翻译成突破点）


给个不啰嗦的定义——

> 

If no k inputs can be shattered by H , call k a break point for H .



从这个定义上看某个假设空间H的VC维数就是最大的非break point值，也就是break point-1.

参考资料


[1]周志华. 机器学习[M]. 清华大学出版社, 2016.


[2]沙伊・沙莱夫-施瓦茨, 沙伊・本-戴维. 深入理解机器学习:从原理到算法[M]. 机械工业出版社, 2016.


[3]Abumostafa Y S, Magdonismail M, Lin H T. Learning from Data: A Short Course[J]. Amlbook, 2012.


[4]白鹏. 支持向量机理论及工程应用实例[M]. 西安电子科技大学出版社, 2008.


[5] 王留行，[https://www.zhihu.com/question/38607822](https://www.zhihu.com/question/38607822)






# hausdorff距离 霍夫曼距离 - YZXnuaa的博客 - CSDN博客
2018年03月20日 17:01:28[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：478
（原文：http://cgm.cs.mcgill.ca/~godfried/teaching/cg-projects/98/normand/main.html）
**1.前言**
在讨论距离问题时，我们通常用最短来描述：比如，点X到多边形P的距离，我们一般指的是P上离X最近的点到X的距离。同样这种概念也适用于多边形之间：如果多边形A和多边形B之间有一段距离，它们间的距离我们一般理解为A和B上任意点之间最短的距离值。这种概念的正式地数学描述为：
![hausdorff距离](http://s12.sinaimg.cn/mw690/001b6gI6gy6RkXwzsWDab&690)-----------------方程1
这个方程按计算机程序的描述为：
«对A中所有的点，找到它与B中所有点之间的最短距离；然后，将该最短距离值存为A与B间的距离值».。
这个对多边形之间的距离定义在某些情况下不会令人满意：比如在图1中，我们会说两个三角形之间的距离可以按最短距离，也就是图中两个红色顶点之间的距离值来描述。但是，我们自然也会希望两个三角形之间小的距离意味着一个多边形上没有点远离另一个多边形。在后面这种概念下，图1上的多边形之间距离就不会像两个红色顶点描述的那样近了。图1中的蓝色顶点之间实际上就相对远很多。这样看来，最短距离的概念完全不考虑多边形的形状。
![hausdorff距离](http://s14.sinaimg.cn/mw690/001b6gI6gy6RkYyR1qldd&690)
图1——最短距离不考虑多边形的形状
图2也给出了另一个例子，图中的两个三角形与图1中一样，也与图1一样具有相同的最短距离，但是位置发生了变化。显然最短距离一样，但情况发生了变化，所以最短距离承载的信息太少。
![hausdorff距离](http://s9.sinaimg.cn/mw690/001b6gI6gy6Rl0yOzRSe8&690)
图2——最短距离不考虑对象的位置
我们会在后面看到，尽管hausdorff距离看起来复杂一点，但它能描述最短距离所不能描述的上面提到的两种情形。
**2.什么是****hausdorff距离**
hausdorff距离是根据Felixhausdorff(1868-1942)命名的，hausdorff距离是指« 某一集合中离另一集合最近点的所有距离最大值 »。
正式的数学描述为，集合A到集合B的hausdorff距离为maximin函数，定义为：
![hausdorff距离](http://s4.sinaimg.cn/mw690/001b6gI6gy6Rl3yOX8ne3&690)     -----------------方程2
这里a和b分别为集合A和集合B中的点，d(a,b)为两点间的尺度；简单起见，我们取d(a,b)为a与b之间的欧几里德距离。如果集合A和集合B为两个点集，采用穷举算法可描述为：
穷举算法：
1. h=0
2. for A中每个点ai
   2.1 shortest=无穷大；
   2.2  forB中每个点bi
       dij = d(ai, bj)
       if dij < shortest then
           shortest = dij;
   2.3  ifshortest >h then
            h = shortest
算法过程：
![hausdorff距离](http://s3.sinaimg.cn/mw690/001b6gI6gy6Rl4fWJrQd2&690)
图3-1
![hausdorff距离](http://s10.sinaimg.cn/mw690/001b6gI6gy6Rl4fYvZ799&690)
图3-2
![hausdorff距离](http://s12.sinaimg.cn/mw690/001b6gI6gy6Rl4fZPSj3b&690)
图3-3
![hausdorff距离](http://s5.sinaimg.cn/mw690/001b6gI6gy6Rl4g1FZ2a4&690)
图3-4
![hausdorff距离](http://s16.sinaimg.cn/mw690/001b6gI6gy6Rl4g35wz1f&690)
图3-5
![hausdorff距离](http://s14.sinaimg.cn/mw690/001b6gI6gy6Rl4g4y8Rcd&690)
图3-6
![hausdorff距离](http://s9.sinaimg.cn/mw690/001b6gI6gy6Rl4g5NQk88&690)
图3-7
![hausdorff距离](http://s13.sinaimg.cn/mw690/001b6gI6gy6Rl4g7tWQbc&690)
图3-8
该算法的时间复杂度显示是O(n,m)，其中n和m分别为集合A和集合B中的点数。
注意，Hausdorff距离是有方向性的，或者说是不对称的。也即是说，在大多数情形下h(A,B)不等于h(B,A)。对图3而言，h(A,B)=d(a1,b1)，而h(B,A)=d(b2,a1)。这种非对称性是maximin函数的特点之一，不同于minmin函数的对称性。
一个更为一般性的Hausdorff距离定义如下：
H (A, B) = max { h (A, B), h (B, A) }   ---------------方程3
该式定义了A与B之间的Hausdorff距离，而方程2定义的是从A到B的Hausdorff距离（也称为有向Hausdorff距离）。h(A,B)和h(B,A)有时被分别称为A到B的向前和向后Hausdorff距离。尽管有时不同的作者会不一致，Hausdorff距离指的是方程3的情况要常见一些。因此，除了特别说明，后面我们在谈到Hausdorff距离时，都指的是方程3。
如果集合A和B由直线或多边形构成，也就是H(A,B)函数应用到直线或多边形上，而不仅仅是直线或多边形的顶点。穷举算法就不能再用于计算这些集合之间的Hausdorff距离，因为这种集合都包含了无穷个点。
那么，我们再回头看看图1中的多边形Hausdorff距离是怎么样的？Hausdorff距离通过一个多边形与另一个多边形之间的最大距离，来定义它们之间相互近似性。而最短距离只考虑对每个多边形的一个点，而不考虑其它的点。
![hausdorff距离](http://s15.sinaimg.cn/bmiddle/001b6gI6gy6Rm6F6E6Gde&690)
图4，Hausdorff距离，表明图1中的每个三角形有一个极限圆包围，该圆半径则为H(P1, P2)
第二点是，相较于最短距离对多边形位置不敏感，Hausdorff距离则具有对位置敏感的优点，如图5.
![hausdorff距离](http://s7.sinaimg.cn/mw690/001b6gI6gy6Rm6J42vYb6&690)
图5，对比图4两个三角形具有相同最短距离，但相对位置变化，则具有不同的Hausdorff距离。
**3.hausdorff距离的计算**
3.1 假设
    在后面的讨论中，我们基于以下假设：
    a.多边形A和B为简单凸多边形；
    b.多边形A和B不相交。
3.2 定理
后面的算法，基于以下三个几何定理。为了叙述方便，我们定义点a和b分别为多边形A和B上的点，且有：
d (a, b) = h (A, B)
简单地说，a为A上相对B的最远点，D而b为B上离A最近的点。
**定理1a**： 以a为垂足，垂直于ab连线的直线为A的支撑线。且A和B在该线的同一侧。
（定理证明见：http://cgm.cs.mcgill.ca/~godfried/teaching/cg-projects/98/normand/lemma1a.html）
**定理1b**： 以b为垂足，垂直于ab连线的直线，为B的支撑线。且A和B在该线的两边。
（定理证明见：http://cgm.cs.mcgill.ca/~godfried/teaching/cg-projects/98/normand/lemma1b.html）
**定理2**：A边界上存在一点X，它到B的距离等于h(A,B)
（定理证明见：http://cgm.cs.mcgill.ca/~godfried/teaching/cg-projects/98/normand/lemma2.html）
**定理3**：设bi为B中离A的顶点ai最近的点，如果µ为从bi到bi+1移动方向（顺时针或逆时针）。对于依次经过A中所有顶点，µ变动不超过两次。
（定理证明见：http://cgm.cs.mcgill.ca/~godfried/teaching/cg-projects/98/normand/lemma3.html）

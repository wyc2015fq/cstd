# 相机模型与标定（九）--LM算法 - 工作笔记 - CSDN博客





2016年07月26日 15:09:41[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7544
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









LM算法，全称为Levenberg-Marquard，它可用于解决非线性最小二乘问题，多用于曲线拟合等场合。

LM算法的实现并不算难，它的关键是用模型函数 f 对待估参数向量p在其领域内做线性近似，忽略掉二阶以上的导数项，从而转化为线性最小二乘问题，它具有收敛速度快等优点。LM算法属于一种“信赖域法”——所谓的信赖域法，此处稍微解释一下：在最优化算法中，都是要求一个函数的极小值，每一步迭代中，都要求目标函数值是下降的，而信赖域法，顾名思义，就是从初始点开始，先假设一个可以信赖的最大位移s，然后在以当前点为中心，以s为半径的区域内，通过寻找目标函数的一个近似函数（二次的）的最优点，来求解得到真正的位移。在得到了位移之后，再计算目标函数值，如果其使目标函数值的下降满足了一定条件，那么就说明这个位移是可靠的，则继续按此规则迭代计算下去；如果其不能使目标函数值的下降满足一定的条件，则应减小信赖域的范围，再重新求解。

事实上，你从所有可以找到的资料里看到的LM算法的说明，都可以找到类似于“如果目标函数值增大，则调整某系数再继续求解；如果目标函数值减小，则调整某系数再继续求解”的迭代过程，这种过程与上面所说的信赖域法是非常相似的，所以说LM算法是一种信赖域法。



LM算法需要对每一个待估参数求偏导，所以，如果你的拟合函数 f 非常复杂，或者待估参数相当地多，那么可能不适合使用LM算法，而可以选择Powell算法——Powell算法不需要求导。

至于这个求导过程是如何实现的，我还不能给出建议，我使用过的方法是拿到函数的方程，然后手工计算出其偏导数方程，进而在函数中直接使用，这样做是最直接，求导误差也最小的方式。不过，在你不知道函数的形式之前，你当然就不能这样做了——例如，你提供给了用户在界面上输入数学函数式的机会，然后在程序中解析其输入的函数，再做后面的处理。在这种情况下，我猜是需要使用数值求导算法的，但我没有亲自试验过这样做的效率，因为一些优秀的求导算法——例如[Ridders算法](http://www.codelast.com/?p=1419)——在一次求导数值过程中，需要计算的函数值次数也会达到5次以上。这样的话，它当然要比手工求出导函数（只需计算一次，就可以得到导数值）效率要差得多了。不过，我个人估计（没有任何依据的，只是猜的）：依赖于LM算法的高效，就算添加了一个数值求导的“拖油瓶”，整个最优化过程下来，它仍然会优于Powell等方法。对这个猜想，我会以实际代码来试验。

文章来源：[http://www.codelast.com/](http://www.codelast.com/)

LM求解过程中需要用到求解线性方程组的算法，一般我们使用[高斯约当消元法](http://www.codelast.com/?p=1288)，因为它非常稳定——虽然它不是最快最好的算法。

对于急需自己编程用LM算法解决一些问题的朋友，如果你的数学几乎都忘了，那么你还是多请教一下自己的朋友吧，要不然连函数的偏导数都不记得怎么求了，是写不出代码的。

网上有很多LM算法的示例程序，但是如果你不理解这个算法的过程，要想看懂它们，很难。而且要对自己定义的函数使用LM算法，更加应该明白该算法的原理。

有一篇很不错的文章，解释了如何实现LM算法：[http://www.ics.forth.gr/~lourakis/levmar/levmar.pdf](http://www.ics.forth.gr/~lourakis/levmar/levmar.pdf)

用Google搜索“Levenberg-Marquardt”，会有很多资料可参考。有一些现成的库也可以使用，不过，到你弄明白怎么用的时候，你都能够自己写出完整的代码了。当初我对LM也是很困惑，一直没弄清它的原理，网上的示例我怎么都用不对，后来一怒之下不再看网上的sample code，重新回到理论上，后来终于弄明白了，于是自己写出了完整的LM实现代码。

需要说明的是，这是非线性无约束的问题，如果待估参数是有约束的（例如参数在某一范围内变动），要想用在LM算法中，那就是约束最优化问题了，这是一个big topic，以我目前的知识储备，尚不能解释好，请大家另寻资料吧。

最后，不得不说的就是，LM算法并非许多人刚接触时想像的那般难，当你了解了过程之后，你就会觉得它很有意思。希望所有在学习它的朋友们都能成功。

原文：

http://www.cnblogs.com/engineerLF/p/5393110.html



# **一.LM最优化算法**



**    最优化**是寻找使得目标函数有最大或最小值的的参数向量。根据求导数的方法，可分为2大类。（1）若f具有解析函数形式，知道x后求导数速度快。(2)使用数值差分来求导数。根据使用模型不同，分为非约束最优化、约束最优化、最小二乘最优化。Levenberg-Marquardt算法是最优化算法中的一种。

**Levenberg-Marquardt**算法是使用最广泛的**非线性**最小二乘算法(**用模型函数 f 对待估参数向量p在其领域内做线性近似，利用泰勒展开，忽略掉二阶以上的导数项，优化目标方程转化为线性最小二乘问题**)。它是利用梯度求最大（小）值的算法，形象的说，属于“爬山”法的一种。它同时具有梯度法和牛顿法的优点。当λ很小时，步长等于牛顿法步长，当λ很大时，步长约等于梯度下降法的步长。见下图：

![](https://img-blog.csdn.net/20141018222216974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWlhbmhlbjEyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    算法从山脚开始不断迭代。可以看到，它的寻优速度是比较快的，在山腰部分直接利用梯度大幅度提升（参见后文例子程序中u较小时），快到山顶时经过几次尝试（u较大时），最后达到顶峰（最大值点），算法终止。

LM算法属于一种“信赖域法”，所谓的信赖域法，就是从初始点开始，先假设一个可以信赖的最大位移**σ**，然后在以当前点为中心，以**σ**为半径的区域内，通过寻找目标函数的一个近似函数（二次的）的最优点，来求解得到真正的位移。在得到了位移之后，再计算目标函数值，如果其使目标函数值的下降满足了一定条件，那么就说明这个位移是可靠的，则继续按此规则迭代计算下去；如果其不能使目标函数值的下降满足一定的条件，则应减小信赖域的范围，再重新求解。
   LM算法需要对每一个待估参数求偏导，所以，如果你的拟合函数 f 非常复杂，或者待估参数相当地多，那么可能不适合使用LM算法，而可以选择Powell算法（Powell算法不需要求导。LM收敛速度块。但是参数应该设定一个初值，其次对于多优化解的问题，也不是很适合。


   英文文档[lemar](http://users.ics.forth.gr/~lourakis/levmar/levmar.pdf)介绍比较简洁，还包括伪代码,请点击下载：

   我的总结如下：


**      (1)Principle:**An iterative tech. to locate the minimum of a multivariate function (sum of squares of non-linear real-valued function).Assuming measure vector **x’**=f(**p**)
 ,target vector **x**(such as training target in classification problem) ,error vector **e**=**x**-**x**’：

**Optimization Object**：arg(**p**) min(**||x**-f(**p**)**||**)

       Linear approximation: in the neighborhood of **p**, assuming **J**is Jacobian Matrix(f**(p)**to **p**) ,for a small**σ**,so f(**p**+**σ**)=f(p)**+****σJ (**Taylor Expansion**) ,**and

**min (||x**-f(**p+****σ**)**||**=**min**(**||**e-J**σ||)  =>  JTJσ=JTe  (**derivation to **σ)**

       Introducing the damping term **u** and set**N**=(** JTJ+uI**)=> **N**σ**= J**T**e => σ**

       For each iteration or updata of **p**(**p:=p+σ**),**u** is adjusted to assure a reduction in the error** e**(norm-2)



**       (2)Merits & Defects**：LM is a standard tech. fornon-linear least-squares problems：**When u is set to a large value, p updates as steepest descent, otherwise updates as Gauss-Newton method.**

**         p **
shall be set toarelative reliable initial value (the work of RBM model).





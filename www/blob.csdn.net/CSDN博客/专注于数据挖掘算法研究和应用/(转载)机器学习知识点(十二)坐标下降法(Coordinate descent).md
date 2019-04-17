# (转载)机器学习知识点(十二)坐标下降法(Coordinate descent) - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月03日 09:53:34[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4519
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









首先介绍一个算法：coordinate-wise minimization

问题的描述：给定一个可微的凸函数![](https://img-blog.csdn.net/20141026151459781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，如果在某一点x，使得f(x)在每一个坐标轴上都是最小值，那么f(x)是不是一个全局的最小值。

形式化的描述为：是不是![](https://img-blog.csdn.net/20141026151445015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)对于所有的d，i都有![](https://img-blog.csdn.net/20141026151458930?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里的![](https://img-blog.csdn.net/20141026151539328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)代表第i个标准基向量。

答案为成立。

![](https://img-blog.csdn.net/20141026151520005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


这是因为：

![](https://img-blog.csdn.net/20141026151605859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


但是问题来了，如果对于凸函数f，若不可微该会怎样呢？

![](https://img-blog.csdn.net/20141026151547571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


答案为不成立，上面的图片就给出了一个反例。

那么同样的问题，现在![](https://img-blog.csdn.net/20141026151601408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，其中g是可微的凸函数，每一个hi都是凸的？

答案为成立。

![](https://img-blog.csdn.net/20141026151613779?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


证明如下，对每一个y

![](https://img-blog.csdn.net/20141026151628490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





坐标下降(Coordinate descent)：

这就意味着，对所有的![](https://img-blog.csdn.net/20141026151642561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，其中g是可微的凸函数，每一个hi都是凸的，我们可以使用坐标下降寻求一个最小值，我们从一个最初的猜想![](https://img-blog.csdn.net/20141026151726468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)开始，对k进行循环：

![](https://img-blog.csdn.net/20141026151740687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


每一次我们解决了![](https://img-blog.csdn.net/20141026151756890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，我们都会使用新的值。

Tseng (2001)的开创性工作证明：对这种f（f在紧集![](https://img-blog.csdn.net/20141026151747098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)上连续，且f到达了其最小值），![](https://img-blog.csdn.net/20141026151932390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)的极限值，k=1,2,3….是f的一个最小元(minimizer)。

在实分析领域：

![](https://img-blog.csdn.net/20141026151922586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)随后收敛与x*(
 Bolzano-Weierstrass)

![](https://img-blog.csdn.net/20141026151938326?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)收敛于f*(
 monotoneconvergence)

其中：

坐标下降的顺序是任意的，可以是从1到n的任意排列。

可以在任何地方将单个的坐标替代成坐标块

关键在于一次一个地更新，所有的一起更新有可能会导致不收敛




我们现在讨论一下坐标下降的应用：




线性回归：



令![](https://img-blog.csdn.net/20141026155427310?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，其中![](https://img-blog.csdn.net/20141026155507980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，A有p列：![](https://img-blog.csdn.net/20141026155517262?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最小化xi，对所有的xj，j不等于i：

![](https://img-blog.csdn.net/20141026155526731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


解得：

![](https://img-blog.csdn.net/20141026155607812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


坐标下降重复这个更新对所有的![](https://img-blog.csdn.net/20141026155547588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对比坐标下降与梯度下降在线性回归中的表现（100个实例，n=100，p=20）

![](https://img-blog.csdn.net/20141026155558493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


将坐标下降的一圈与梯度下降的一次迭代对比是不是公平呢？是的。

![](https://img-blog.csdn.net/20141026155640000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其中r=y-Ax。每一次的坐标更新需要O(n)个操作，其中O(n)去更新r，O(n)去计算![](https://img-blog.csdn.net/20141026155651484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，所以一圈就需要O(np)，跟梯度下降是一样的。

![](https://img-blog.csdn.net/20141026155632516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我们用相同的例子，用梯度下降进行比较，似乎是与计算梯度下降的最优性相违背。

那么坐标下降是一个一阶的方法吗？事实上不是，它使用了比一阶更多的信息。




现在我们再关注一下支持向量机：

SVM对偶中的坐标下降策略：

![](https://img-blog.csdn.net/20141026155652515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


SMO(Sequentialminimal optimization)[算法](http://lib.csdn.net/base/datastructure)是两块的坐标下降，使用贪心法选择下一块，而不是用循环。

回调互补松弛条件(complementaryslackness conditions)：

![](https://img-blog.csdn.net/20141026155703186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


v，d，s是原始的系数，截距和松弛，其中![](https://img-blog.csdn.net/20141026155715869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，使用任何的（1）中i使得![](https://img-blog.csdn.net/20141026155759406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)来计算d，利用（1）（2）来计算2.

SMO重复下面两步：

选出不满足互补松弛的αi，αj

最小化αi，αj使所有的变量满足条件

![](https://img-blog.csdn.net/20141026155826734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzgwMjE4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


第一步使用启发式的方法贪心得寻找αi，αj，第二步使用等式约束。




——————维基百科的解释——————



坐标下降优化方法是一种非梯度优化算法。为了找到一个函数的局部极小值，在每次迭代中可以在当前点处沿一个坐标方向进行一维搜索。在整个过程中循环使用不同的坐标方向。一个周期的一维搜索迭代过程相当于一个梯度迭代。

坐标下降法基于最小化多变量目标函数可以通过每次沿一个方向最小化目标函数来求解。与梯度方法的变化的梯度方向不同，坐标下降方法固定其他的梯度方向。例如，坐标方向为**e**1**,e2,…,en**。每次沿一个坐标方向最小化目标函数，循环地沿每个坐标方向进行计算。如果给定Xk，Xk+1的第i个坐标由如下给定：

![Coordinate <wbr>Descent <wbr>Optimization](http://s2.sinaimg.cn/middle/890c6aa3gbd8c6fcd65d1&690)

从初始值X0求取F的局部值，然后迭代的求取一个序列**X0，X1，X2，…**

通过在每次迭代中进行一维搜索，可以有如下结论：

![Coordinate <wbr>Descent <wbr>Optimization](http://s12.sinaimg.cn/middle/890c6aa3gbd8c7086229b&690)


It can be shown that this sequence has similar convergence properties as steepest descent. No improvement after one cycle
 of[line
 search](http://en.wikipedia.org/wiki/Line_search)along coordinate directions implies a stationary point is reached.

This process is illustrated below.

![Coordinate <wbr>Descent <wbr>Optimization](http://s15.sinaimg.cn/middle/890c6aa3gbd8c7a58b0ae&690)

    其实，gradient descent 方法是利用目标函数的导数（梯度）来确定搜索方向的，而该梯度方向可能不与任何坐标轴平行。而coordinate descent方法是利用当前坐标系统进行搜索，不需要求目标函数的导数，只按照某一坐标方向进行搜索最小值。









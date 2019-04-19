# 数据流分析中的Distributive Dataflow Problems - 电影旅行敲代码 - CSDN博客
2017年11月08日 21:23:28[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：326
## Distributive Data flow Problems
一个数据流问题包括如下几部分：
- CFG
- 数据流值的值域
- 初始的数据流值
- 交汇运算（用于将前驱或者后继节点的值进行“交汇”）
- 传输函数 
而Distributive Data flow（可分配数据流问题）问题就是关于交汇运算和传输函数的问题，可分配数据流问题有如下等式：
![](http://latex.codecogs.com/gif.latex?trans$_b$(X&space;\bigcup&space;Y)&space;=&space;trans$_b$(X)\bigcup&space;trans$_b$(Y))
也就是传输函数*trans*在交汇运算*U*上是可分配的。判断数据流问题是否可分配很重要，因为可分配的数据流分析可以使用MFP（Maximal Fixed Point）求解，并且此时MFP等同于MOP（Meet Over All paths），其中Gen-Kill问题都是可分配数据流问题。。图灵完备的程序语言有循环，所以MOP通常情况下都是没有有效的解法的。
## MFP vs MOP
MFP（Maximal Fixed Point）方法是一种次优的数据流问题解决办法，MFP、MOP以及精确解的关系如下公式所示。
![](http://latex.codecogs.com/gif.latex?MFP\leq&space;MOP\leq&space;PerfectSolution)
MOP（meet over all paths）从字面意思上也能够理解，就是考虑所有的路径，然后在所有的路径上应用传输函数，最后在进行交汇运算，MOP的公式如下所示。
![](http://latex.codecogs.com/gif.latex?MOP(n)&space;=&space;\Lambda&space;f&_p&_&i&(T),&space;\textup{for&space;all&space;paths&space;}pi&space;\textup{&space;in&space;data&space;flow&space;graph&space;reaching&space;}n)
程序的路径有可能是无限的，所有MOP并不一定存在有效的解法，但是MFP是MOP的一种*conservatively approximates*。 
但是要注意的是MOP也是Ideal值的一种*conservatively approximates*，只要CFG中存在这样一条边，MOP都会将这条边纳入到考虑范围之内。
![](http://latex.codecogs.com/gif.latex?MOP(n)&space;=&space;PerfectSolution&space;\Lambda&space;SolutionToUnexecutedPaths)
由于MOP多考虑了不可能执行的路径，所以能够得到更加保守地解。例如对于到达定值来说，多考虑了不可能执行的路径，多出来的路径会交汇（并操作）更多的值，在到达定值的格中，越多的到达定值集合实际“越小”；而对于可用表达式，多考虑了不可能执行的路径，多出来的路径由于交汇运算（交运算）会得到更少的可用表达式，在可用表达式的格中，越少的可用表达式集合“越小”，所以MOP是安全的。
![这里写图片描述](https://img-blog.csdn.net/20171108211512937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
*注：上图盗自[http://www.cs.cmu.edu/~15745/lectures/L6-Foundations-of-Dataflow.pdf](http://www.cs.cmu.edu/~15745/lectures/L6-Foundations-of-Dataflow.pdf)*
但是如果数据流问题不是可分配的，那么*MFP*就不等于*MOP*，例如下面的问题， 
![这里写图片描述](https://img-blog.csdn.net/20171108211937093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
使用MFP算法，过早的进行交汇运算，最后我们并不能得到c的值是多少，但是MOP，先考虑所有路径，得到两条路径c的值为5，再聚合得到c的值就是5。另外关于MFP的收敛速度，可以参照[[2]](http://www.cs.cmu.edu/~15745/lectures/L6-Foundations-of-Dataflow.pdf)，或者参照前面关于后序和逆后序的文章。
### 参考资料
[1] [http://pages.cs.wisc.edu/~horwitz/CS704-NOTES/2.DATAFLOW.html](http://pages.cs.wisc.edu/~horwitz/CS704-NOTES/2.DATAFLOW.html)
[2] [http://www.cs.cmu.edu/~15745/lectures/L6-Foundations-of-Dataflow.pdf](http://www.cs.cmu.edu/~15745/lectures/L6-Foundations-of-Dataflow.pdf)
[3] [https://www.youtube.com/watch?v=t4MKLiiSk-k&t=341s](https://www.youtube.com/watch?v=t4MKLiiSk-k&t=341s)

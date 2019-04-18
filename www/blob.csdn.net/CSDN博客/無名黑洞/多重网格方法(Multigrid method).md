# 多重网格方法(Multigrid method) - 無名黑洞 - CSDN博客
2011年12月13日 16:51:07[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：12884
多重网格方法(Multigridmethod)
多重网格方法是解微分方程的方法。这个方法的好处是在利用迭代法收敛结果的时候速度特别快。并且，不管是否对称，是否线性都无所谓。它的值要思想是在粗糙结果和精细结果之间插值。
前面介绍了Gauss–Seidel方法和Jacobi 方法，现在再用这两个方法来举例。尽管Gauss–Seidel (GS)方法converge更快一些，但其实对于维度很高的系统都很慢。Multigrid（MG）方法的思路是先把问题粗糙化，把原网格投影到一个比较简单的新网格上计算，等到快速收敛以后再经由Interpolation（插值）返回原来的系统。
对于某个工程数学问题（如泊松方程），可以归纳为线形方程Ax = b,  A为n X n矩阵。那么，最终目的是得到所谓的x = A^(-1)b。定义e(t) = x – x(t)，当e(t)为小于某个值的时候，可以认为xconverge到了合适的值。但实际上我们比较的是相邻的值。
把A非奇异分解A = B – C，
Bx – Cx = b
x = B(-1) Cx + B^(-1) b
并分开求解x
Bx(t+1) - Cx(t)= b
x(t+1) = B^(-1) Cx(t) + B^(-1) b                  -（1）
把形如B^(-1) C的矩阵称作迭代矩阵，用M表示。
容易发现, 
x(t+1) – x = Mx(t) + B^(-1) b – x = Mx(t) +Mx = M (x(t) – x)   -（2）
以上式子与（1）式等价。
另外可令N = B^(-1):
x(t+1) = Mx(t) + Nb            - (3)  
不同的迭代方法其实就是A的不同分解法，反映到（3）式就是取不同的M和N值。
比如，在GS方法中B = D – L, C = U, 则(3)式为：
x(t+1) = (D - L)^(-1)Ux(t) + (D - L)^(-1)b            - (4)  
分析发现，当n的数值比较大时，以上收敛是极其缓慢的。假设p(i) 是序号为i的原系统有限元基函数（i = 1, 2, … , n），q(i)是粗糙化的网格(i = 1,2, … , m  m<n).
一种粗糙化方法是构造矩阵H，使p = H * q, H为m X n矩阵。令A’ = HAH^T, x’ = Hx, b’ = Hb. 则 A’ x’ = b’ 是一个m维粗化的网格系统。
更加具体来说，对于一个k维的问题，如果k小于一个指定的维数，那么直接用jacob等方法解救可以了。否则，粗糙化为更低的维度比如变为原来的二分之一，最后再把维数变回来。把mesh粗糙化的过程叫做Downv-cycle (从k维到k/2维), 反之叫Up v-cycle.（从k维到2k维）。
参考文献：
[http://en.wikipedia.org/wiki/Multigrid_method](http://en.wikipedia.org/wiki/Multigrid_method)
Institut fÄur Mathematik undWissenschaftliches Rechnen, Introduction to multigrid methods, Al¯o Borzµ
William L. Briggs , A Multigrid Tutorial, 

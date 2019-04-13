
# 似然函数（likelihood）、最大似然函数、最小二乘解 - 郭云飞的专栏 - CSDN博客


2017年09月21日 16:46:07[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：2767


在英语语境里，likelihood 和 probability 的日常使用是可以互换的，都表示对机会 (chance) 的同义替代。但在数学中，probability 这一指代是有严格的定义的，即符合柯尔莫果洛夫公理 (Kolmogorov axioms) 的一种数学对象（换句话说，不是所有的可以用0到1之间的数所表示的对象都能称为概率）。而 likelihood (function) 这一概念是由Fisher提出，他采用这个词，也是为了凸显他所要表述的数学对象既和 probability 有千丝万缕的联系，但又不完全一样的这一感觉。
中文把它们一个翻译为概率（probability），一个翻译为似然（likelihood）也是独具匠心。
**似然函数的定义：**
![](https://www.zhihu.com/equation?tex=L%28%5Ctheta+%7C+%5Ctextbf%7Bx%7D%29+%3D+f%28%5Ctextbf%7Bx%7D+%7C+%5Ctheta%29)
上式中，小*x*指的是联合样本随机变量*X*取到的值，即**X**=**x**；这里的*θ*是指未知参数，它属于参数空间；而
![](https://www.zhihu.com/equation?tex=f%28%5Ctextbf%7Bx%7D%7C%5Ctheta%29)是一个密度函数，特别地，它表示(给定)*θ*下关于联合样本值*x*的联合密度函数。
从定义上，似然函数和密度函数是完全不同的两个数学对象：前者是关于θ的函数，后者是关于**x**的函数。所以这里的等号= 理解为函数值形式的相等，而不是两个函数本身是同一函数（根据函数相等的定义，函数相等当且仅当定义域相等并且对应关系相等）。
**两者的联系：**
如果X是离散随机变量，那么其概率密度函数![](https://www.zhihu.com/equation?tex=f%28%5Ctextbf%7Bx%7D%7C%5Ctheta%29)可改写为：
![](https://www.zhihu.com/equation?tex=+f%28%5Ctextbf%7Bx%7D+%7C+%5Ctheta%29+%3D+%5Cmathbb%7BP%7D_%5Ctheta%28%5Ctextbf%7BX%7D+%3D+%5Ctextbf%7Bx%7D%29)
即代表了在参数为θ下，随机变量X取到x的可能性。并且，如果我们发现：
![](https://www.zhihu.com/equation?tex=L%28%5Ctheta_1+%7C+%5Ctextbf%7Bx%7D+%29+%3D+%5Cmathbb%7BP%7D_%7B%5Ctheta_1%7D%28%5Ctextbf%7BX%7D+%3D+%5Ctextbf%7Bx%7D%29+%3E+%5Cmathbb%7BP%7D_%7B%5Ctheta_2%7D%28%5Ctextbf%7BX%7D+%3D+%5Ctextbf%7Bx%7D%29+%3D+L%28%5Ctheta_2+%7C+%5Ctextbf%7Bx%7D%29)
那么似然函数就反应出这样一个朴素推测：在参数![](https://www.zhihu.com/equation?tex=%5Ctheta_1)下随机向量X取到值x的可能性大于在参数![](https://www.zhihu.com/equation?tex=%5Ctheta_2)下随机向量X取到值x的可能性。换句话说，我们更有理由相信相对于![](https://www.zhihu.com/equation?tex=%5Ctheta_2)来说![](https://www.zhihu.com/equation?tex=%5Ctheta_1)更有可能是真实值。这里的可能性是由概率来刻画。
综上，概率(密度)表达给定![](https://www.zhihu.com/equation?tex=%5Ctheta)下样本随机向量X = x的可能性，而似然表达了给定样本X = x下参数![](https://www.zhihu.com/equation?tex=%5Ctheta_1)(相对于另外的参数![](https://www.zhihu.com/equation?tex=%5Ctheta_2))为真实值的可能性。
**最大似然估计：**
在已知试验结果（即是样本）的情况下，用来估计满足这些样本分布的参数，把可能性最大的那个参数![](https://www.zhihu.com/equation?tex=%5Ctheta)作为真实的参数估计。最大似然估计，最大似然估计是建立在这样的思想上：已知某个参数能使这个样本出现的概率最大，我们当然不会再去选择其他小概率的样本，所以干脆就把这个参数作为估计的真实值。
**求最大似然函数估计值的一般步骤：**
（1） 写出似然函数
（2） 对似然函数取对数，并整理
（3） 求导数
（4） 解似然方程
**最小二乘法（Least Square ）的解析解可以用 Gaussian 分布以及最大似然估计求得**
首先假设线性回归模型具有如下形式：
![](https://www.zhihu.com/equation?tex=f%28%5Cmathbf+x%29+%3D+%5Csum_%7Bj%3D1%7D%5E%7Bd%7D+x_j+w_j+%2B+%5Cepsilon+%3D+%5Cmathbf+x+%5Cmathbf+w%5E%5Cintercal+%2B+%5Cepsilon)
其中：![](https://www.zhihu.com/equation?tex=%5Cmathbf+x+%5Cin+%5Cmathbb+R%5E%7B1+%5Ctimes+d%7D)，![](https://www.zhihu.com/equation?tex=%5Cmathbf+w+%5Cin+%5Cmathbb+R%5E%7B1+%5Ctimes+d%7D)，误差![](https://www.zhihu.com/equation?tex=%5Cepsilon+%5Cin+%5Cmathbb+R)
已知：
![](https://www.zhihu.com/equation?tex=%5Cmathbf+X%3D%28%5Cmathbf+x_1+%5Ccdots+%5Cmathbf+x_n%29%5E%5Cintercal+%5Cin+%5Cmathbb+R%5E%7Bn+%5Ctimes+d%7D)，![](https://www.zhihu.com/equation?tex=%5Cmathbf+y+%5Cin+%5Cmathbb+R%5E%7Bn+%5Ctimes+1%7D)
如何求参数**W**呢？
如果用最小二乘法的话，有误差函数：
![](https://www.zhihu.com/equation?tex=%5Ctext%7Barg%5C%2Cmin%7D_%7B%5Cmathbf+w%7D+f%28%5Cmathbf+w%29+%3D+%5Csum_%7Bi%3D1%7D%5En%28%5Cmathbf+y_i+-+%5Cmathbf+x_i+%5Cmathbf+w%5E%5Cintercal%29%5E2%3D+%7B%5Cleft%5ClVert%7B%5Cmathbf+y+-+%5Cmathbf+X+%5Cmathbf+w%5E%5Cintercal%7D%5Cright%5CrVert%7D_2%5E2)
我们对W求偏导，然后令个偏导 = 0，联立解方程——这就是最小二乘法求W的过程。
如果用最大似然函数求解的话：
假设误差服从高斯正态分布：
![](https://www.zhihu.com/equation?tex=%5Cepsilon_i+%5Csim+%5Cmathcal%7BN%7D%280%2C+%5Csigma%5E2%29)
也就是说：
![](https://www.zhihu.com/equation?tex=%5Cmathbf+y_i+%5Csim+%5Cmathcal%7BN%7D%28%5Cmathbf+x_i+%5Cmathbf+w%5E%5Cintercal%2C+%5Csigma%5E2%29)
则最大似然估计推导：
![](https://www.zhihu.com/equation?tex=+%5Cbegin%7Balign%2A%7D+%5Ctext%7Barg%5C%2Cmax%7D_%7B%5Cmathbf+w%7D+L%28%5Cmathbf+w%29+%26+%3D+%5Cln+%7B%5Cprod_%7Bi%3D1%7D%5En+%5Cfrac%7B1%7D%7B%5Csigma+%5Csqrt%7B2%5Cpi%7D%7D+%5Cexp%28-%5Cfrac%7B1%7D%7B2%7D%28%5Cfrac%7B%5Cmathbf+y_i+-+%5Cmathbf+x_i+%5Cmathbf+w%5E%5Cintercal%7D%7B%5Csigma%7D%29%5E2%7D%29%5C%5C+%26+%3D+-+%5Cfrac%7B1%7D%7B2%5Csigma%5E2%7D+%5Csum_%7Bi%3D1%7D%5En%28%5Cmathbf+y_i+-+%5Cmathbf+x_i+%5Cmathbf+w%5E%5Cintercal%29%5E2+-+n+%5Cln+%5Csigma+%5Csqrt%7B2%5Cpi%7D+%5Cend%7Balign%2A%7D+)
对上式求偏导然后令个偏导 = 0，联立解方程。
总结：两者的结果是一样的。








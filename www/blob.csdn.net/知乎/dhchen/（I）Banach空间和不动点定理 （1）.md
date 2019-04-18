# （I）Banach空间和不动点定理 （1） - 知乎
# 

上一节：[泛函分析简史（3）： 继承者们！ - 知乎专栏](https://zhuanlan.zhihu.com/p/26112911)

这是泛函分析专栏里面真正的正本的第一篇，我纠结过是否该从拓扑空间讲起，后面我放弃了。第一、对于专栏这种形式，从拓扑空间讲起虽然符合“逻辑”，但是不符合“学习顺序”，从具体到抽象比从抽象到具体要容易得多。第二、正如我在一开始讲过的那样，这个专栏更加偏重于“应用”和“例子”。我只会讲到必须使用拓扑观念的时候在引入，比如，弱收敛，那个时候我大概会讲“net”，用以替代数列。

第一部分重点放在banach空间和不动点定理上，重点放在两类算子：压缩算子和紧算子，和两个不动点定理：banach不动点和schauder不动点。 

**本章重点和目标：**

**1. 范数(norm),Banach空间,**

**2，压缩算子、Banach不动点及其应用**

**3. 紧性及其应用**

**4. 紧算子、Schauder不动点及其应用**

**知识脉络如下([2])：**

![](https://pic3.zhimg.com/v2-f1dff70dfd72dde9deb45add0f7557ba_b.jpg)

**我计划用5-6篇文章来弄完这一章，这一篇的目标是（1）。**

## **一、赋范线性空间**

1. 线性空间

学过线性代数的都应该知道什么叫“线性空间”，这里我就不重复了，有点淡忘的同学可以看以下的回答。本质上就是一个具有“线性结构的集合”，特别是[@三思孔明](https://www.zhihu.com/people/0cea5e5984219f90c25f8237442dc1f2)的回答，属于教科书的回答

> [向量空间_百度百科](https://link.zhihu.com/?target=http%3A//baike.baidu.com/link%3Furl%3Dwqafaxg7hVoqgg8Ouh5LluE6YErdTfV0rds13L2MPjUCWtNhdTTUK9TLO99zAjYrc5UpMwf-gc-XFDAVp1RxWxdzC5nOp74rDqk2tbruoHdOe0bg8YfEohEzodqzsJlFXsq_GUJqjchZnDRltgbsw1X8bs2TVa5nC2T1fk-N2IpGzQPC-IJkqk3KoMux411Oj4e9E4_qyXxJqioTtj8arK)[https://www.zhihu.com/question/24086219/answer/140270125](https://www.zhihu.com/question/24086219/answer/140270125)

我希望大家明了以下三个泛函分析中最重要的线性空间,你如果非常了解这三个具体的空间，那么可以说你的泛函分析就没白学，这三个空间（包括它们的推广涵盖了大部分应用）：

![\mathbb{R}^n](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5En),![C(\overline{\Omega})](https://www.zhihu.com/equation?tex=C%28%5Coverline%7B%5COmega%7D%29),![L^p(\Omega)](https://www.zhihu.com/equation?tex=L%5Ep%28%5COmega%29)

我们知道一般的线性空间可以定义在一般的数域上，在这个专栏中，我们把自己限定在实的线性空间，也就是说数域就是实数。当然了，很多时候我会提到复的情况会有什么区别。 

![C(\overline{\Omega})](https://www.zhihu.com/equation?tex=C%28%5Coverline%7B%5COmega%7D%29)是有界区域![\Omega\subset \mathbb{R}^n](https://www.zhihu.com/equation?tex=%5COmega%5Csubset+%5Cmathbb%7BR%7D%5En)的闭包上连续函数全体构成的集合。 

![L^p(\Omega), 1\leq p<\infty](https://www.zhihu.com/equation?tex=L%5Ep%28%5COmega%29%2C+1%5Cleq+p%3C%5Cinfty)是区间![\Omega\subset \mathbb{R}^n](https://www.zhihu.com/equation?tex=%5COmega%5Csubset+%5Cmathbb%7BR%7D%5En)上可测且![\int_\Omega |f(x)|^p dx<\infty](https://www.zhihu.com/equation?tex=%5Cint_%5COmega+%7Cf%28x%29%7C%5Ep+dx%3C%5Cinfty)函数（**等价类**）构成的全体。 

这里这个“等价类”是初学者容易犯错的地方，我后面会解释。

不难发现，这几个空间是典型的线性空间。

2. 一个线性空间![X](https://www.zhihu.com/equation?tex=X)上如果具有一个“范数”![\|\cdot\|:X\to [0,\infty)](https://www.zhihu.com/equation?tex=%5C%7C%5Ccdot%5C%7C%3AX%5Cto+%5B0%2C%5Cinfty%29)，那么我们就叫它赋范的线性空间。 什么叫范数呢？ 它是![X](https://www.zhihu.com/equation?tex=X)上的一个非负的函数，满足下面的性质。

![](https://pic4.zhimg.com/v2-1cd1aed4ce2c0a96072047d197275d47_b.jpg)
**它本身给每个X每个元素一个数值，这个数值表达了它到0元素的“距离”，记住，这个范数是我们给定的，所以你可以“随便”给，所以理论上你说赋范线性空间，你必须指定范数，如**

**![(X,\|\cdot\|_X)](https://www.zhihu.com/equation?tex=%28X%2C%5C%7C%5Ccdot%5C%7C_X%29)，但是很多空间**![\mathbb{R}^n](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5En),![C(\overline{\Omega})](https://www.zhihu.com/equation?tex=C%28%5Coverline%7B%5COmega%7D%29),![L^p(\Omega)](https://www.zhihu.com/equation?tex=L%5Ep%28%5COmega%29)上的范数是默认的。 比如把， 

![\mathbb{R}^n](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5En)上的范数就是![\|{\bf x}\|:=\sqrt{\sum_{i=1}^n|x_i|^2}](https://www.zhihu.com/equation?tex=%5C%7C%7B%5Cbf+x%7D%5C%7C%3A%3D%5Csqrt%7B%5Csum_%7Bi%3D1%7D%5En%7Cx_i%7C%5E2%7D),

![C(\overline{\Omega})](https://www.zhihu.com/equation?tex=C%28%5Coverline%7B%5COmega%7D%29)上的范数为一致范数

![\|u\|:=\max_{x\in\overline{\Omega}}|u(x)| ](https://www.zhihu.com/equation?tex=%5C%7Cu%5C%7C%3A%3D%5Cmax_%7Bx%5Cin%5Coverline%7B%5COmega%7D%7D%7Cu%28x%29%7C+),

![L^p(\Omega)](https://www.zhihu.com/equation?tex=L%5Ep%28%5COmega%29)上的范数为![\sqrt[p]{\int_\Omega |f(x)|^p dx}](https://www.zhihu.com/equation?tex=%5Csqrt%5Bp%5D%7B%5Cint_%5COmega+%7Cf%28x%29%7C%5Ep+dx%7D). 这里我们发现![\|f\|=0](https://www.zhihu.com/equation?tex=%5C%7Cf%5C%7C%3D0)推出的结论是![ f](https://www.zhihu.com/equation?tex=+f)**几乎处处**为0，所以我们定义![L^p(\Omega), 1\leq p<\infty](https://www.zhihu.com/equation?tex=L%5Ep%28%5COmega%29%2C+1%5Cleq+p%3C%5Cinfty)的时候这个空间的元素是等价类，也就是说这个空间上![0](https://www.zhihu.com/equation?tex=0)元素是几乎处处为0的可测函数构成的一个函数等价类![N](https://www.zhihu.com/equation?tex=N)，而![f-g=0](https://www.zhihu.com/equation?tex=f-g%3D0)也仅仅表面![f,g](https://www.zhihu.com/equation?tex=f%2Cg)是相同的函数等价类(参考[5,67页])。如果用商空间的语言来讲，这个空间是上可测且![\int_\Omega |f(x)|^p dx<\infty](https://www.zhihu.com/equation?tex=%5Cint_%5COmega+%7Cf%28x%29%7C%5Ep+dx%3C%5Cinfty)函数全体构成的空间![\mathcal{L}^p(\Omega)](https://www.zhihu.com/equation?tex=%5Cmathcal%7BL%7D%5Ep%28%5COmega%29)模掉![\mathcal{N}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BN%7D)的空间：

![L^p(\Omega):=\mathcal{L}^p(\Omega)/ \mathcal{N}](https://www.zhihu.com/equation?tex=L%5Ep%28%5COmega%29%3A%3D%5Cmathcal%7BL%7D%5Ep%28%5COmega%29%2F+%5Cmathcal%7BN%7D).

> [Lp space - Wikipedia](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Lp_space%23Lp_spaces)

一个有趣的问题是，范数有什么更深的几何意义吗？有！我在下面的回答中说明了本质上“范数”是一类“闵可夫斯基泛函”，一个0元素附近某个集合可以保证我们一定可以定义一个范数。我在下面的回答中就用了这方面的知识：

> [https://www.zhihu.com/question/57875627/answer/154677170](https://www.zhihu.com/question/57875627/answer/154677170)

## 二、Banach空间和完备化。 

范数的最大作用是可以定义“收敛”， 也就是说

对于列![(u_n)\subset X](https://www.zhihu.com/equation?tex=%28u_n%29%5Csubset+X)，我们定义 ![\lim_{n\to\infty}u_n=u](https://www.zhihu.com/equation?tex=%5Clim_%7Bn%5Cto%5Cinfty%7Du_n%3Du)为![\lim_{n\to \infty} \|u_n-u\|_X=0](https://www.zhihu.com/equation?tex=%5Clim_%7Bn%5Cto+%5Cinfty%7D+%5C%7Cu_n-u%5C%7C_X%3D0)。 

也就是说只要![\lim_{n\to \infty} \|u_n-u\|_X=0](https://www.zhihu.com/equation?tex=%5Clim_%7Bn%5Cto+%5Cinfty%7D+%5C%7Cu_n-u%5C%7C_X%3D0)，我们就说![u_n\to u](https://www.zhihu.com/equation?tex=u_n%5Cto+u)（![u_n](https://www.zhihu.com/equation?tex=u_n)收敛到![u](https://www.zhihu.com/equation?tex=u)）。一个列![(u_n)\subset X](https://www.zhihu.com/equation?tex=%28u_n%29%5Csubset+X)，如果存在![u](https://www.zhihu.com/equation?tex=u)使得![u_n\to u](https://www.zhihu.com/equation?tex=u_n%5Cto+u)，我们称这个列是收敛的。 

我们都知道Cauchy收敛准则， 也就是说在实数上一个Cauchy列必然有极限，这一特性也叫完备性。**但是，一般的赋范线性空间不一定具有这个特性，也就是不是所有的赋范线性空间都是完备的**：

**一个例子**：我们在![C[0,2]](https://www.zhihu.com/equation?tex=C%5B0%2C2%5D)上给上一个范数

![\|f\|:=\int_0^2 |f(x)|dx](https://www.zhihu.com/equation?tex=%5C%7Cf%5C%7C%3A%3D%5Cint_0%5E2+%7Cf%28x%29%7Cdx), 然后 我们定义函数列![f_n](https://www.zhihu.com/equation?tex=f_n)如下：![f_n(x)=x^n, x\in[0,1], f_n(x)=1, x\in[1,2]](https://www.zhihu.com/equation?tex=f_n%28x%29%3Dx%5En%2C+x%5Cin%5B0%2C1%5D%2C+f_n%28x%29%3D1%2C+x%5Cin%5B1%2C2%5D)

这个函数列是一个Cauchy列，但是我们找不到一个连续函数![f\in C[0,2]](https://www.zhihu.com/equation?tex=f%5Cin+C%5B0%2C2%5D)使得

![\int_0^2|f_n(x)-f(x)|dx\to 0](https://www.zhihu.com/equation?tex=%5Cint_0%5E2%7Cf_n%28x%29-f%28x%29%7Cdx%5Cto+0)成立。 

**第二个例子**，我们考虑多项式空间![P[0,1]](https://www.zhihu.com/equation?tex=P%5B0%2C1%5D)，我们赋给一个范数![\|p\|=\max_{x\in[0,1]}|p(x)|](https://www.zhihu.com/equation?tex=%5C%7Cp%5C%7C%3D%5Cmax_%7Bx%5Cin%5B0%2C1%5D%7D%7Cp%28x%29%7C)，这个空间也不少完备的。 实际上，考虑![p_n(x)=\sum_{k=0}^n\frac{x^k}{k!}](https://www.zhihu.com/equation?tex=p_n%28x%29%3D%5Csum_%7Bk%3D0%7D%5En%5Cfrac%7Bx%5Ek%7D%7Bk%21%7D)，这是一个Cauchy列，但是它不是收敛的，因为它一致收敛到![e^x](https://www.zhihu.com/equation?tex=e%5Ex)，这不是一个多项式。**实际上，多项式空间![P[0,1]](https://www.zhihu.com/equation?tex=P%5B0%2C1%5D)不管给什么范数都不可能完备，这一点我会在讲baire category的时候给出证明。**

**所以，我们引入Banach空间：**

![](https://pic3.zhimg.com/v2-0ad59fd870ddd97c80834b9045ee4352_b.jpg)
我们称赋范线性空间为Banach空间如果任何Cauchy列必然有极限（即收敛）。常用的空间![\mathbb{R}^n](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5En),![C(\overline{\Omega})](https://www.zhihu.com/equation?tex=C%28%5Coverline%7B%5COmega%7D%29),![L^p(\Omega)](https://www.zhihu.com/equation?tex=L%5Ep%28%5COmega%29)是banach空间。具体的证明可以参考[1,2]。 有趣的是，如果在![C[0,2]](https://www.zhihu.com/equation?tex=C%5B0%2C2%5D)上我们给一个范数![\|u\|=\max_{x\in[0,2]}|u(x)|](https://www.zhihu.com/equation?tex=%5C%7Cu%5C%7C%3D%5Cmax_%7Bx%5Cin%5B0%2C2%5D%7D%7Cu%28x%29%7C)，这个空间就是完备的了。这个时候，我们上面定义的列![f_n](https://www.zhihu.com/equation?tex=f_n)在这个范数下不是收敛的。 同时，我们可以发现![C[0,2]\subset L[0,2]](https://www.zhihu.com/equation?tex=C%5B0%2C2%5D%5Csubset+L%5B0%2C2%5D),而后者在范数![\|f\|:=\int_0^2 |f(x)|dx](https://www.zhihu.com/equation?tex=%5C%7Cf%5C%7C%3A%3D%5Cint_0%5E2+%7Cf%28x%29%7Cdx)下是完备的。

于是有一个问题，如果![(X,\|\cdot\|_X)](https://www.zhihu.com/equation?tex=%28X%2C%5C%7C%5Ccdot%5C%7C_X%29)不完备， 存不存在一定存在一个更大的Banach空间![(X',\|\|_{X'})](https://www.zhihu.com/equation?tex=%28X%27%2C%5C%7C%5C%7C_%7BX%27%7D%29)使得![X\subset X'](https://www.zhihu.com/equation?tex=X%5Csubset+X%27)而且![\|u\|_X=\|u\|_{X'}](https://www.zhihu.com/equation?tex=%5C%7Cu%5C%7C_X%3D%5C%7Cu%5C%7C_%7BX%27%7D)。答案是肯定的？也就是任何一个赋范线性空间都可以“拓展”成一个banach空间。 这个过程叫完备化。

这个时候，我们定义“算子”的连续性，设![(X,\|\|_X),(Y,\|\|_Y),](https://www.zhihu.com/equation?tex=%28X%2C%5C%7C%5C%7C_X%29%2C%28Y%2C%5C%7C%5C%7C_Y%29%2C)为赋范线性空间 ，一个函数![A:X\to Y](https://www.zhihu.com/equation?tex=A%3AX%5Cto+Y)被称为是连续的，如果它满足：

只要在![X](https://www.zhihu.com/equation?tex=X)中![x_n\to x](https://www.zhihu.com/equation?tex=x_n%5Cto+x)，那么在![Y](https://www.zhihu.com/equation?tex=Y)中![A(x_n)\to Ax](https://www.zhihu.com/equation?tex=A%28x_n%29%5Cto+Ax).

## 三、简单的“拓扑”概念和稠密性的一些应用。

有了范数后，我可以定义“拓扑”，也就是“开集族”。 

设![u\in X](https://www.zhihu.com/equation?tex=u%5Cin+X)，集合![B_r(u):=\{f;\|f-u\|<r\}](https://www.zhihu.com/equation?tex=B_r%28u%29%3A%3D%5C%7Bf%3B%5C%7Cf-u%5C%7C%3Cr%5C%7D)叫![u](https://www.zhihu.com/equation?tex=u)的一个半径为r的开球。 

一个集合![S\subset X](https://www.zhihu.com/equation?tex=S%5Csubset+X)被成为开集，如果对于任意![u_0\in S](https://www.zhihu.com/equation?tex=u_0%5Cin+S)，存在![r](https://www.zhihu.com/equation?tex=r)使得

![B_{r_0}(u_0)\subset S](https://www.zhihu.com/equation?tex=B_%7Br_0%7D%28u_0%29%5Csubset+S)。一个集合被成为闭集，如果它的补集是一个开集。我们回忆一下就知道，欧几里德空间中闭集的定义是用收敛列定义的，下面我们可以证明这两个东西的等价性：。

一个集合是闭的当且仅当其中的收敛列的极限必然在其中。
![](https://pic3.zhimg.com/v2-36e618897f6dca3e86198aef315a24b6_b.jpg)

![](https://pic2.zhimg.com/v2-189fa81cfc40724fd1bc52d18186e5b5_b.jpg)![](https://pic1.zhimg.com/v2-62bc7c22a27978cb5b1a339b82f66764_b.jpg)
我们可以发现以下的拓扑性质：

1. 任意个开集的并必然是开集

2.有限个开集的交是一个开集

3.![X,\emptyset](https://www.zhihu.com/equation?tex=X%2C%5Cemptyset)是两个开集。

对于任意一个集合![S](https://www.zhihu.com/equation?tex=S)，我们设![\{A_\alpha\}](https://www.zhihu.com/equation?tex=%5C%7BA_%5Calpha%5C%7D)是![S](https://www.zhihu.com/equation?tex=S)包含的开集构成的集合，![\{B_\alpha\}](https://www.zhihu.com/equation?tex=%5C%7BB_%5Calpha%5C%7D)是包含S的全部闭集。则集合![S^\circ=\cup A_\alpha,\, \overline{S}=\cap B_\alpha](https://www.zhihu.com/equation?tex=S%5E%5Ccirc%3D%5Ccup+A_%5Calpha%2C%5C%2C+%5Coverline%7BS%7D%3D%5Ccap+B_%5Calpha)被成为它的内部和闭包。可以发现![S^\circ](https://www.zhihu.com/equation?tex=S%5E%5Ccirc)是集合![S](https://www.zhihu.com/equation?tex=S)内最大的开集，![\overline{S}](https://www.zhihu.com/equation?tex=%5Coverline%7BS%7D)是包含![S](https://www.zhihu.com/equation?tex=S)最小的闭集。

**如果![\overline{S}=X](https://www.zhihu.com/equation?tex=%5Coverline%7BS%7D%3DX)，我们称集合![S](https://www.zhihu.com/equation?tex=S)在![X](https://www.zhihu.com/equation?tex=X)中稠密。**

我们列出一些有名的稠密性结论：

多项式空间![P[a,b]](https://www.zhihu.com/equation?tex=P%5Ba%2Cb%5D)在![C[a,b]](https://www.zhihu.com/equation?tex=C%5Ba%2Cb%5D)中稠密；设![D[a,b]](https://www.zhihu.com/equation?tex=D%5Ba%2Cb%5D)是![[a,b]](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D)上所有光滑而且存在![[c,d]\subset[a,b]](https://www.zhihu.com/equation?tex=%5Bc%2Cd%5D%5Csubset%5Ba%2Cb%5D)使得

![f](https://www.zhihu.com/equation?tex=f)在![[c,d]](https://www.zhihu.com/equation?tex=%5Bc%2Cd%5D)外都等于0的函数构成的集合。 ![D[a,b]](https://www.zhihu.com/equation?tex=D%5Ba%2Cb%5D)在![L[a,b]](https://www.zhihu.com/equation?tex=L%5Ba%2Cb%5D)中稠密。


**稠密性是非常有用的一个性质，比如你要证明一个![ X](https://www.zhihu.com/equation?tex=+X)上的结论，你可以首先它的一个比较好的子集上证明这个结论，然后利用稠密性把这个稠密子集上的结论推广到整个空间上。**

**稠密性的一个简单应用：设![f\in L[0,2\pi]](https://www.zhihu.com/equation?tex=f%5Cin+L%5B0%2C2%5Cpi%5D),证明**

**![\lim_{n\to \infty}\int_0^{\pi} \sin(nx)f(x)dx=0 ](https://www.zhihu.com/equation?tex=%5Clim_%7Bn%5Cto+%5Cinfty%7D%5Cint_0%5E%7B%5Cpi%7D+%5Csin%28nx%29f%28x%29dx%3D0+) （黎曼引理）.**

证明：我们先证明如果![f\in D(0,2\pi)](https://www.zhihu.com/equation?tex=f%5Cin+D%280%2C2%5Cpi%29) (也就是说， ![f](https://www.zhihu.com/equation?tex=f) 是光滑的有紧支集的函数，特别的，在![[0,\pi]](https://www.zhihu.com/equation?tex=%5B0%2C%5Cpi%5D) 的边界上为0)这个结果成立。实际上，由于![f(0)=f(\pi)=0](https://www.zhihu.com/equation?tex=f%280%29%3Df%28%5Cpi%29%3D0)而且它光滑，我们通过分部积分可以发现

![\int_0^{\pi} \sin(nx)f(x)dx= \int_0^{\pi}\frac{\cos(n x) }{n} f'(x)dx](https://www.zhihu.com/equation?tex=%5Cint_0%5E%7B%5Cpi%7D+%5Csin%28nx%29f%28x%29dx%3D+%5Cint_0%5E%7B%5Cpi%7D%5Cfrac%7B%5Ccos%28n+x%29+%7D%7Bn%7D+f%27%28x%29dx).

所有，我们可以发现

![|\lim_{n\to\infty} \int_0^{\pi}\frac{\cos(n x) }{n} f'(x)dx|\leq \lim_{n\to\infty} \int_0^{\pi}|f'(x)|/n dx=0](https://www.zhihu.com/equation?tex=%7C%5Clim_%7Bn%5Cto%5Cinfty%7D+%5Cint_0%5E%7B%5Cpi%7D%5Cfrac%7B%5Ccos%28n+x%29+%7D%7Bn%7D+f%27%28x%29dx%7C%5Cleq+%5Clim_%7Bn%5Cto%5Cinfty%7D+%5Cint_0%5E%7B%5Cpi%7D%7Cf%27%28x%29%7C%2Fn+dx%3D0)。

好了，我们要利用稠密性来证明剩下的情况，对于一般的![f\in L[0,\pi]](https://www.zhihu.com/equation?tex=f%5Cin+L%5B0%2C%5Cpi%5D)和任意![\epsilon>0](https://www.zhihu.com/equation?tex=%5Cepsilon%3E0),可以找到

![f_\epsilon\in D[0,\pi] ](https://www.zhihu.com/equation?tex=f_%5Cepsilon%5Cin+D%5B0%2C%5Cpi%5D+)使得![\|f-f_\epsilon\|\leq \epsilon ](https://www.zhihu.com/equation?tex=%5C%7Cf-f_%5Cepsilon%5C%7C%5Cleq+%5Cepsilon+), 我们可以发现

![\int_0^\pi \sin(nx) f(x)dx= \int_0^\pi \sin(nx) f_\epsilon(x)dx+\int_0^{\pi} \sin(nx) (f(x)- f_\epsilon(x))dx](https://www.zhihu.com/equation?tex=%5Cint_0%5E%5Cpi+%5Csin%28nx%29+f%28x%29dx%3D+%5Cint_0%5E%5Cpi+%5Csin%28nx%29+f_%5Cepsilon%28x%29dx%2B%5Cint_0%5E%7B%5Cpi%7D+%5Csin%28nx%29+%28f%28x%29-+f_%5Cepsilon%28x%29%29dx)

不难发现，

![\limsup_{n\to\infty}|\int_0^{\pi} \sin(nx) (f(x)- f_\epsilon(x))dx|\leq\int_0^{2\pi} |f(x)- f_\epsilon(x))|dx=\|f-f_\epsilon\|](https://www.zhihu.com/equation?tex=%5Climsup_%7Bn%5Cto%5Cinfty%7D%7C%5Cint_0%5E%7B%5Cpi%7D+%5Csin%28nx%29+%28f%28x%29-+f_%5Cepsilon%28x%29%29dx%7C%5Cleq%5Cint_0%5E%7B2%5Cpi%7D+%7Cf%28x%29-+f_%5Cepsilon%28x%29%29%7Cdx%3D%5C%7Cf-f_%5Cepsilon%5C%7C),

因为

![ \int_0^\pi \sin(nx) f_\epsilon(x)dx\to 0](https://www.zhihu.com/equation?tex=+%5Cint_0%5E%5Cpi+%5Csin%28nx%29+f_%5Cepsilon%28x%29dx%5Cto+0),所以我们有

![\limsup_{n\to\infty}|\int_0^\pi \sin(nx) f(x)dx|\leq \epsilon ](https://www.zhihu.com/equation?tex=%5Climsup_%7Bn%5Cto%5Cinfty%7D%7C%5Cint_0%5E%5Cpi+%5Csin%28nx%29+f%28x%29dx%7C%5Cleq+%5Cepsilon+).

因为![\epsilon>0](https://www.zhihu.com/equation?tex=%5Cepsilon%3E0)是任意的，我们有

![\limsup_{n\to\infty}|\int_0^\pi \sin(nx) f(x)dx|=0](https://www.zhihu.com/equation?tex=%5Climsup_%7Bn%5Cto%5Cinfty%7D%7C%5Cint_0%5E%5Cpi+%5Csin%28nx%29+f%28x%29dx%7C%3D0)。

## -----------主要参考的文献--------------------------------------

1. Ciarlet, Linear and Nonlinear Functional Analysis with Applications 

2. Zeidler, Applied linear functional 

3 H. Brezis, Functional analysis, Sobolev spaces and PDEs  

4. Rudin, Real and complex analysis. 

## 下集 [不动点定理和钟摆问题 - 知乎专栏](https://zhuanlan.zhihu.com/p/26365534#tipjar)


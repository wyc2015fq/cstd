# 代数数论(十一):a summary and a start - 知乎
# 

寒假已经到了,考试结束后给自己放了三天假,现在是时候继续我们的讨论了，顺便说说上半年的一些计划.

之前的回顾可见[代数数论：总集篇](https://zhuanlan.zhihu.com/p/24312945?refer=shuxuehenyouyisisuoyiyaohaohaoxue)，当时我们基本上总结了1950年之前代数数论的一些重要结果.

————————————————————

1.

再稍微总结一下，我们从解方程出发，进而研究了数域的一些基本结构，解决了一些丢番图方程的求解问题，讨论了局部与整体的关系等等，其中手段主要有两类：

①偏向代数, 如最初利用交换代数知识证明代数整数环是Dedekind环从而成立素理想分解唯一等性质,以及Galois理论给出分解域，惯性域等更精细的结构，以及之后利用群的上同调(例如Hilbert 90)来给出一些估计不等式，以及群表示理论得到Abel扩张的类数公式等等.

②偏向分析, 例如利用Dirichlet特征标得到Dirichlet密度定理,数的几何给出类数有限、单位定理(这也可由![\mathbb I_K^1/K^{\times}](https://www.zhihu.com/equation?tex=%5Cmathbb+I_K%5E1%2FK%5E%7B%5Ctimes%7D)的紧性得到),还有Dedekind zeta函数的类数公式

①②相结合得到了两个著名的理论：

Class field theory (结果表述更为代数)/ Tate thesis (结果表述更为分析)

前者是利用群的上同调这一方法(特别是有限循环群的上同调)结合对L函数在1处的极点阶数的比较，最终证明了数域的**Abel扩张**可由数域自身内在刻画(利用idele类群/local field的乘法群).

后者是利用Fourier变换,Fubini原理等等调和分析(借助Haar测度的存在性)手段，证明了局部与整体的**zeta函数、L函数**的关系以及函数方程,重新得到了Dedekind zeta函数的函数方程.

而我们现在的目标就是推广上面的理论。

在1967年Langlands给Weil写了一封信：

Langlands, Robert (1967), *[Letter to Prof. Weil](https://link.zhihu.com/?target=http%3A//publications.ias.edu/rpl/section/21)*

信的内容大概有8页，具体内容可自行阅读(码了一半发现太长因此略掉对这份信的翻译)

总之，现在我们知道，更大的框架是Langlands program，而**上面的Class field theory可以看成是最简单的![GL_1](https://www.zhihu.com/equation?tex=GL_1)的case:**

整体Class field theory表明有下列同构：

L/K 数域的Galois扩张，则
![Gal(L/K)^{ab} \cong C_K/NC_L](https://www.zhihu.com/equation?tex=Gal%28L%2FK%29%5E%7Bab%7D+%5Ccong+C_K%2FNC_L)

两边关于L取反向极限就得到了![G_K^{ab} \cong \hat C_K](https://www.zhihu.com/equation?tex=G_K%5E%7Bab%7D+%5Ccong+%5Chat+C_K)

如果要把这个同构推广到非交换的case，首先要去掉Abel化这个条件。

上面的同构诱导它们的对偶(1维表示)同构
![Hom(G_K,S^1) \cong Hom(\widehat{I_K/K^{\times}},S^1)](https://www.zhihu.com/equation?tex=Hom%28G_K%2CS%5E1%29+%5Ccong+Hom%28%5Cwidehat%7BI_K%2FK%5E%7B%5Ctimes%7D%7D%2CS%5E1%29)

（我们总考虑连续同态）

由于S^1交换群，因此![Hom(G_K,S^1)=Hom(G_K^{ab},S^1)](https://www.zhihu.com/equation?tex=Hom%28G_K%2CS%5E1%29%3DHom%28G_K%5E%7Bab%7D%2CS%5E1%29),再由Pontryagin对偶我们可以重新得到![G_K^{ab} \cong \hat C_K](https://www.zhihu.com/equation?tex=G_K%5E%7Bab%7D+%5Ccong+%5Chat+C_K)

更一般地，我们可以考虑一般的表示而不仅仅是酉表示，即讨论

![Hom(G_K,\mathbb C^{\times})=Hom(G_K,S^1)](https://www.zhihu.com/equation?tex=Hom%28G_K%2C%5Cmathbb+C%5E%7B%5Ctimes%7D%29%3DHom%28G_K%2CS%5E1%29) (Galois side)

(由于G_K紧)

与

![Hom(\mathbb I_K/K^{\times},\mathbb C^\times)](https://www.zhihu.com/equation?tex=Hom%28%5Cmathbb+I_K%2FK%5E%7B%5Ctimes%7D%2C%5Cmathbb+C%5E%5Ctimes%29) (automorphic side)

的关系

而两边都是特征标，都可以定义L函数，从而研究L函数的性质.

**例子:**
![K=\mathbb Q](https://www.zhihu.com/equation?tex=K%3D%5Cmathbb+Q)

则

![Hom(G_\mathbb Q, \mathbb C^{\times}) =Hom(\varprojlim (\mathbb Z / n \mathbb Z)^{\times},\mathbb C^{\times})](https://www.zhihu.com/equation?tex=Hom%28G_%5Cmathbb+Q%2C+%5Cmathbb+C%5E%7B%5Ctimes%7D%29+%3DHom%28%5Cvarprojlim+%28%5Cmathbb+Z+%2F+n+%5Cmathbb+Z%29%5E%7B%5Ctimes%7D%2C%5Cmathbb+C%5E%7B%5Ctimes%7D%29)就是所有Dirichlet特征，可以定义Dirichlet L function

![Hom(\mathbb I_\mathbb{Q}/\mathbb {Q}^{\times},\mathbb C^\times)](https://www.zhihu.com/equation?tex=Hom%28%5Cmathbb+I_%5Cmathbb%7BQ%7D%2F%5Cmathbb+%7BQ%7D%5E%7B%5Ctimes%7D%2C%5Cmathbb+C%5E%5Ctimes%29)即 Hecke character over Q，可以定义 Hecke L series

由![\mathbb I_{\mathbb Q}/ \mathbb Q^{\times} \cong \mathbb R_{>0} \times \hat {\mathbb Z}^{\times}](https://www.zhihu.com/equation?tex=%5Cmathbb+I_%7B%5Cmathbb+Q%7D%2F+%5Cmathbb+Q%5E%7B%5Ctimes%7D+%5Ccong+%5Cmathbb+R_%7B%3E0%7D+%5Ctimes+%5Chat+%7B%5Cmathbb+Z%7D%5E%7B%5Ctimes%7D)可看出两者的联系

注意到![\mathbb I_K=GL_1(\mathbb A_K)](https://www.zhihu.com/equation?tex=%5Cmathbb+I_K%3DGL_1%28%5Cmathbb+A_K%29)，因此可以想到如下推广：

①GL_1换成GL_n

②1维表示换成n维表示

即讨论

绝对Galois群![G_K](https://www.zhihu.com/equation?tex=G_K)的n维表示与![GL_n(\mathbb A_K) /GL_n(K)](https://www.zhihu.com/equation?tex=GL_n%28%5Cmathbb+A_K%29+%2FGL_n%28K%29)的n维表示的关系,

值得注意的是左边是一个紧群,右边不是一个紧群，因此我们需要在右边的表示上加上一些限制(automorphic)

而寒假我们希望仔细的研究GL(2)的case

主要的参考书为

**Automorphic forms on GL(2)   Jacquet,Langlands**

2.

[代数数论（4.2）：modular form and sum of squares](https://zhuanlan.zhihu.com/p/24170999?refer=shuxuehenyouyisisuoyiyaohaohaoxue)里很粗略地提了提模形式,以及一些介绍性的文献：

59页的PPT

Generating Functions, Partitions, and q-series:
An Introduction to Classical Modular Forms 

[http://www.math.purdue.edu/~egoins/seminar/16-06-28.pdf](https://link.zhihu.com/?target=http%3A//www.math.purdue.edu/~egoins/seminar/16-06-28.pdf)
以及

[Modular form - Wikipedia](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Modular_form)

以及

[http://web.stanford.edu/~aaronlan/assets/landesman_junior_paper.pdf](https://link.zhihu.com/?target=http%3A//web.stanford.edu/~aaronlan/assets/landesman_junior_paper.pdf)

这显然不够.

我们需要对模形式进行更详细的考察，这将成为以后一个主要的利器，因此我们需要更多更多的例子;

另外是椭圆曲线的一些补全,包括其在各种域上的情况,以及初步的weil conjecture;

主要参考为

**Modular Forms:A Computational Approach  William A. Stein**

**GTM 106**

**3.**

更多地关于schemes of finite type over the integers、zeta function、L function等等的性质，以及一些代数几何的常识.

主要参考书是

An Invitation to Arithmetic Geometry Dino Lorenzini 

的后半段

以及

[Jean-Pierre Serre](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Jean-Pierre_Serre) (1965). *Zeta and L-functions*. *Arithmetical Algebraic Geometry*. Harper and Row.



注：

我并不喜欢抄书，更不喜欢某个定义突然跳出来(这也是我们在没有介绍完基础前迟迟不提Langlands program的原因)，而偏向于先学个几遍再前进.

这个系列大概算是无处推放的读书笔记+一些自己无聊想的问题+一些性质的补充证明，因此完整性不做保证，但是参考书中一定会写得很详细，这就挺好.

目前的打算是1-2天更新一篇,所以理论上寒假结束时应该有10-20篇左右(当然不是说这个系列，而是专栏的所有文章)

顺便补上一个简单的从零开始的关于代数数论的介绍

[https://arxiv.org/pdf/1007.4426v3.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1007.4426v3.pdf)


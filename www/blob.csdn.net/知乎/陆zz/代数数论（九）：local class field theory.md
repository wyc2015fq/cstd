# 代数数论（九）：local class field theory - 知乎
# 

在[代数数论（4.5） 阿黛尔与伊代尔](https://zhuanlan.zhihu.com/p/23312740?refer=shuxuehenyouyisisuoyiyaohaohaoxue)我们初步提到了local field，而local field 的初步介绍还可参见：

[LOCAL FIELDS I](https://link.zhihu.com/?target=https%3A//www.math.ucdavis.edu/~osserman/classes/254a/lectures/28.pdf)

[LOCAL FIELDS II](https://link.zhihu.com/?target=https%3A//www.math.ucdavis.edu/~osserman/classes/254a/lectures/29.pdf)

[LOCAL FIELDS III](https://link.zhihu.com/?target=https%3A//www.math.ucdavis.edu/~osserman/classes/254a/lectures/30.pdf)

[LOCAL FIELDS IV](https://link.zhihu.com/?target=https%3A//www.math.ucdavis.edu/~osserman/classes/254a/lectures/31.pdf)

[LectureNotes](https://link.zhihu.com/?target=http%3A//math.mit.edu/classes/18.785/2015fa/LectureNotes9.pdf)

Class field theroy通常有两种方式证明，一者为先local（利用群的上同调）再global（将local field全部嵌入idele），二者为先global（解析方法证明第一不等式，群的上同调给出第二不等式，最终得到Artin互反）再local。

通常 local case 的 class field theroy比较简单，因此我们先关心 local field.

（这里local field指剩余类域为有限域的完备离散赋值域以及R,C）

————————————————————

1.

我们有如下观察：

**Finite field is simple than Local field**

**Local field is simpler than Global field**

如果F是有限域，那么成立：

**①解方程可以直接枚举解**

**②![F^{\times}](https://www.zhihu.com/equation?tex=F%5E%7B%5Ctimes%7D)的结构就是一个有限循环群**

**③Galois扩张的Galois群一定是循环群**

**④F的n次扩张只有一个**

**⑤Galois上同调直接转成![\hat{\mathbb Z}](https://www.zhihu.com/equation?tex=%5Chat%7B%5Cmathbb+Z%7D)的上同调,通常转为循环群的上同调（此时模2周期性）**

如果F非阿局部域，且特征是0，那么稍微复杂一些，不过我们对应有：

①

**[Hensel lemma](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Hensel%27s_lemma)成立**

Pf：直接由完备性可得。

因此解方程约等于在有限域中解，容易不少。

②

![F^{\times}](https://www.zhihu.com/equation?tex=F%5E%7B%5Ctimes%7D)的结构完全知道：

![](https://pic3.zhimg.com/v2-ba69b0af335022bd2ae831d64e49a666_b.png)
特别地：
![\mathbb Q_p^{\times} = \mathbb Z  \times \mathbb Z/(p-1) \mathbb Z \times \mathbb Z_p  \ \ (p >2)](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_p%5E%7B%5Ctimes%7D+%3D+%5Cmathbb+Z++%5Ctimes+%5Cmathbb+Z%2F%28p-1%29+%5Cmathbb+Z+%5Ctimes+%5Cmathbb+Z_p++%5C+%5C+%28p+%3E2%29)
![\mathbb Q_2^{\times} = \mathbb Z  \times \mathbb Z/2 \mathbb Z \times \mathbb Z_2  ](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_2%5E%7B%5Ctimes%7D+%3D+%5Cmathbb+Z++%5Ctimes+%5Cmathbb+Z%2F2+%5Cmathbb+Z+%5Ctimes+%5Cmathbb+Z_2++)

所以有：

**![F^{\times} / (F^{\times} )^n](https://www.zhihu.com/equation?tex=F%5E%7B%5Ctimes%7D+%2F+%28F%5E%7B%5Ctimes%7D+%29%5En)有限Abel群**

Pf:利用指数映射和Hensel lemma.

这与Q等数域的情形不同，数域的情形都是无限群。

③

**K/F n 次 Galois扩张，则Gal(K/F) 一定是可解群。**

Pf: High ramification group 给出Gal(K/F)一个正规群列。

因此任何一个local field的Galois扩张都可以看成循环扩张的迭代，所以很多时候我们只用研究循环扩张。

另外得到Galois逆问题在![\mathbb Q_p](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_p)上不成立：只有某些可解群才能实现为Galois群。

而我们又知道所有可解群都可以实现为![\mathbb Q](https://www.zhihu.com/equation?tex=%5Cmathbb+Q)上Galois群，而其他case未知，这就表明整体域的扩张有时会出现不可解的case，更为复杂。

④

**F的n次扩张只有有限个**

Pf：任给一个n次扩张K，存在![a \in O_K ,K =F(a)](https://www.zhihu.com/equation?tex=a+%5Cin+O_K+%2CK+%3DF%28a%29),记a的极小多项式为f，则f为n次收益不可约多项式，系数都属于![O_F](https://www.zhihu.com/equation?tex=O_F)。

我们可以自然定义一个映射：
![(O_F)^n \rightarrow \{ K/F\}](https://www.zhihu.com/equation?tex=%28O_F%29%5En+%5Crightarrow+%5C%7B+K%2FF%5C%7D)

即把a_i，拼成一个多项式f(x)=x^n+a_1x^{n-1}+\hdots+a_n

然后令K等于f添加一个根到F中所得扩域。

这是一个局部常值映射（由Hensel lemma 与 逼近定理）

由于![O_F](https://www.zhihu.com/equation?tex=O_F)是紧的，所以这个映射的值域是有限的。

又其值域包含所有n次扩张，即得结论。

⑤

**F的Galois上同调很简单。**

（来自[tate-galois_cohomology](https://link.zhihu.com/?target=http%3A//wstein.org/edu/2010/582e/refs/tate-galois_cohomology.pdf)）

![](https://pic4.zhimg.com/v2-fe68a41dc14c619cf8671bdbf4cf8ec3_b.png)![](https://pic2.zhimg.com/v2-fb2d43352f38e9d8aa12927245836ad9_b.png)

2.

有限域最简单，此时我们有类域论：

设F是一个有限域，我们有自然的映射
![\mathbb Z \rightarrow Gal(F^{ab}/F) :
n \rightarrow \sigma^n](https://www.zhihu.com/equation?tex=%5Cmathbb+Z+%5Crightarrow+Gal%28F%5E%7Bab%7D%2FF%29+%3A%0An+%5Crightarrow+%5Csigma%5En)
![\sigma ](https://www.zhihu.com/equation?tex=%5Csigma+)是F的Frobenius自同构

①

profinite completion后上面是同构，这给出F的有限Abel扩张与Z的有限指标开子群一一对应

②

函子性：有限域的n次abel扩张，则Z自乘n倍的映射给出两个自然映射交换图表

③

我们可以显式构造出所有F的n次Abel扩张

现在回到local field class theory,其表述为：

假设F是一个local field，则

有自然的映射![F^{\times} \rightarrow Gal(F^{ab}/F)](https://www.zhihu.com/equation?tex=F%5E%7B%5Ctimes%7D+%5Crightarrow+Gal%28F%5E%7Bab%7D%2FF%29)

①

profinite completion后上面是同构，这给出F的有限Abel扩张与![F^{\times}](https://www.zhihu.com/equation?tex=F%5E%7B%5Ctimes%7D)的有限指标开子群一一对应，具体对应为：

E/F n 次 abel扩张 对应于开子群![N_{E/F}E^{\times}](https://www.zhihu.com/equation?tex=N_%7BE%2FF%7DE%5E%7B%5Ctimes%7D)

并且![Gal(E/F) \cong F^{\times} /N_{E/F}E^{\times}](https://www.zhihu.com/equation?tex=Gal%28E%2FF%29+%5Ccong+F%5E%7B%5Ctimes%7D+%2FN_%7BE%2FF%7DE%5E%7B%5Ctimes%7D)

②

函子性：E/F n次abel扩张，则E到F的取范映射给出两个自然映射交换图表

③

explicit class field theory 可见

[Lubin–Tate theory](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Lubin%25E2%2580%2593Tate_theory)

而local class field theory的证明，可以参见neukirch《 algebraic number theory》

3.

作为简单的应用我们来求：

（p，q为素数）

**![\mathbb Q_p ](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_p+)的q次Galois扩张的个数。**

**先假设p>2**

由于q是素数，此时q次Galois扩张一定是循环扩张，从而为Abel扩张，根据local class field theory，其与![\mathbb Q_p^{\times} = \mathbb Z  \times \mathbb Z/(p-1) \mathbb Z \times \mathbb Z_p  \ \ (p >2)](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_p%5E%7B%5Ctimes%7D+%3D+%5Cmathbb+Z++%5Ctimes+%5Cmathbb+Z%2F%28p-1%29+%5Cmathbb+Z+%5Ctimes+%5Cmathbb+Z_p++%5C+%5C+%28p+%3E2%29)的指标为q的开子群一一对应。

由于这样的子群都包含![(\mathbb Q_p^{\times })^q](https://www.zhihu.com/equation?tex=%28%5Cmathbb+Q_p%5E%7B%5Ctimes+%7D%29%5Eq),所以其与

![\mathbb Q_p^{\times}/ (\mathbb Q_p^{\times})^q= \mathbb Z / q \mathbb Z  \times \mathbb Z/(p-1,q) \mathbb Z \times \mathbb Z_p/q \mathbb Z_p  \ \ (p >2)](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_p%5E%7B%5Ctimes%7D%2F+%28%5Cmathbb+Q_p%5E%7B%5Ctimes%7D%29%5Eq%3D+%5Cmathbb+Z+%2F+q+%5Cmathbb+Z++%5Ctimes+%5Cmathbb+Z%2F%28p-1%2Cq%29+%5Cmathbb+Z+%5Ctimes+%5Cmathbb+Z_p%2Fq+%5Cmathbb+Z_p++%5C+%5C+%28p+%3E2%29)指标为q的子群一一对应。

①如果q=p：

那么其与

![\mathbb Q_p^{\times}/ (\mathbb Q_p^{\times})^p= \mathbb Z / p \mathbb Z  \times  \mathbb Z/p \mathbb Z \ \ (p >2)](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_p%5E%7B%5Ctimes%7D%2F+%28%5Cmathbb+Q_p%5E%7B%5Ctimes%7D%29%5Ep%3D+%5Cmathbb+Z+%2F+p+%5Cmathbb+Z++%5Ctimes++%5Cmathbb+Z%2Fp+%5Cmathbb+Z+%5C+%5C+%28p+%3E2%29)指标为p的子群一一对应，所以

其个数=![\mathbb F_p^2](https://www.zhihu.com/equation?tex=%5Cmathbb+F_p%5E2)的1维子空间个数。

不难看出有p+1个，实际上在[一元域(一)—有限域的“组合数”](https://zhuanlan.zhihu.com/p/22059833)中我们已经算过

**![\mathbb F_p](https://www.zhihu.com/equation?tex=%5Cmathbb+F_p)上n维线性空间![V](https://www.zhihu.com/equation?tex=V)的k维线性子空间的个数**

所以可以直接套用公式。

②q不等于p，那么
![\mathbb Q_p^{\times}/ (\mathbb Q_p^{\times})^q= \mathbb Z / q \mathbb Z  \times \mathbb Z/(p-1,q) \mathbb Z ](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_p%5E%7B%5Ctimes%7D%2F+%28%5Cmathbb+Q_p%5E%7B%5Ctimes%7D%29%5Eq%3D+%5Cmathbb+Z+%2F+q+%5Cmathbb+Z++%5Ctimes+%5Cmathbb+Z%2F%28p-1%2Cq%29+%5Cmathbb+Z+)

如果q| p-1 ,那么![\mathbb Q_p^{\times}/ (\mathbb Q_p^{\times})^q= \mathbb F_q^2](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_p%5E%7B%5Ctimes%7D%2F+%28%5Cmathbb+Q_p%5E%7B%5Ctimes%7D%29%5Eq%3D+%5Cmathbb+F_q%5E2),于是同上有q+1个；

如果q不整除p-1，那么![\mathbb Q_p^{\times}/ (\mathbb Q_p^{\times})^q= \mathbb Z / q \mathbb Z  ](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_p%5E%7B%5Ctimes%7D%2F+%28%5Cmathbb+Q_p%5E%7B%5Ctimes%7D%29%5Eq%3D+%5Cmathbb+Z+%2F+q+%5Cmathbb+Z++)只有一个。

至于p=2的case留给有兴趣的人XD.

综上我们得到了

**(Prop 1)**

**![\mathbb Q_p ](https://www.zhihu.com/equation?tex=%5Cmathbb+Q_p+)的q次Galois扩张只有有限个，其个数为**

**①p>2：**

**![q+1 ](https://www.zhihu.com/equation?tex=q%2B1+)（若q|p-1或q=p），1（else）**

**②p=2：**

**7（q=2），1（else）**


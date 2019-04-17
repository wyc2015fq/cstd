# 《离散时间信号处理学习笔记》—z变换（一） - 一世豁然的专栏 - CSDN博客





2018年09月08日 16:36:21[一世豁然](https://me.csdn.net/Explorer_day)阅读数：735








注：本博客是基于奥本海姆《离散时间信号处理》第三版编写，主要是为了自己学习的复习与加深。





一、z变换

1、序列的博里叶变换被定义为

![](https://img-blog.csdn.net/20180908081342289?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.1)

序列x[n]的z变换定义为

![](https://img-blog.csdn.net/20180908081410372?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式（3.2）

1）、式(3.2)一般是一个无穷项的和或者无穷项幂级数，其中z被考虑为一个复变量。有时将上式看做一个算子是有有益的，它把一个序列变换成另一个函数，也就是说，z变换算子![](https://img-blog.csdn.net/20180908081746981?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)被定义为

![](https://img-blog.csdn.net/2018090808190484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.3)

把序列x[n]变换为函数X(z)，其中，z是一个连续复变量。

2）、一个序列和它的z变换之间的唯一对应关系用符号记为

![](https://img-blog.csdn.net/20180908082144635?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.4)

3）、式(3.2)所定义的z变换往往称为双边z变换，而与此相对应的单边z变换则定义为

![](https://img-blog.csdn.net/20180908082614792?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.5)

显然，仅当x[n]=0,n<0时，双边变换和单边变换才是相等的。

4）、比较式(3.1)和式(3.2)，很显然，博里叶变换和z变换之间存在紧密的联系。特别的是，若将式(3.2)中的复变量z代以复变量ejw，那么z变换话变为博里叶变换。将复变量z表示为极坐标形式

![](https://img-blog.csdn.net/20180908083407474?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

则式(3.2)就可以写成

![](https://img-blog.csdn.net/2018090808344254?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

或

![](https://img-blog.csdn.net/20180908083501946?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.6)

式(3.6)可以看做为原序列x[n]与指数序列r-n相乘后的博里叶变换。对于r=1，式(3.6)就是x[n]的博里叶变换。



2、利用复数z平面来描述和阐明z变换是很方便的。

1）、在z平面，相应于|z|=1的围线就是半径为1的圆，如图下图所示

![](https://img-blog.csdn.net/20180908084134361?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此圆称为单位圆，它就是在0≤w＜2π范围上z=ejw的点集合。

2）、z变换在单位圆上的求值就对应于博里叶变换。注意，w是从原点到单位圆上某点z的矢量与复平面实轴之间的角度。若沿着z平面单位圆上从z=1(即w=0)开始，经过z=j(w=π/2)到z=-1(w=π)对X(z)求值，就得到了0≤w≤π的博里叶变换。继续沿着单位圆从w=π到w=2π考察博里叶变换，就等效于从w=-π到π=0.



3、z变换也不是对所有序列或者对全部z值都收敛。对已经给定的序列，使z变换收敛的哪一级z值就称为收敛域（ROC）。

1）、式(3.2)幂级数的收敛仅仅取决于|z|；也就是说，因为|X(z)|<∞，如果

![](https://img-blog.csdn.net/20180908085712140?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.7)

式(3.2)幂级数的收敛域就由满足式(3.7)的全部z值组成。

2）、z变换收敛域一定由在z平面内以原点为中心的圆环所组成的。收敛域的外边界是一个圆(或者可能向外延伸至无穷大)，而内边界也是一个圆(或者收敛域向内扩展至可包括原点)。如下图所示

![](https://img-blog.csdn.net/20180908090658261?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果收敛域包括单位圆，自然就意味着z变换对|z|=1收敛，或者书哦序列的博里叶变换收敛。相反，若收敛域不包括收单位圆，博里叶变换绝不收敛。

3）、z变换的一致要求指数加权序列绝对可加。



4、当无限项和可表示闭式时，也即可以被求和并被表示成一个简单的数学表达式是，z变换是最有用的。X(z)在收敛域呢是一个有理函数，即为

![](https://img-blog.csdn.net/20180908091646236?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式（3.8）

这样的·X(z)是最重要且是最有用的z变换。式(3.8)中，P(z)和Q(z)都是z的多项式。

1）、一般的，对于使X(z)=0的z称为X(z)的零点，而使X(z)为无穷大的z称为X(z)的极点。对于式(3.8)所示的有理函数的情况，零点为分子多项式的根，极点(对于有限z值)为分母多项式的根。

2）、在大多数情况下，用有理函数比用无限和表示要方便得多。任何能表示指数和的序列都能用一个有理z变换来表示。这样的z变换，除了一个常数幅度因子外，都由他的零点和极点来决定。

3）、在序列为有限长的情况下，也有一个相当简单的形式。如果序列仅在区间N1≤n≤N2内为零，则z变换为

![](https://img-blog.csdn.net/20180908093042490?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

只要每一项是有限的，X(z)就不存在收敛的问题。一般来说，不太可能把一个有限项的和表示为一个闭式；不过，在这种情况下可能也没有必要。



5、基本变换对

![](https://img-blog.csdn.net/20180908093306586?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



二、z变换收敛域的性质

1、性质1；收敛域具有这样的形式，即0≤rR<|z|或|z|≤rL≤∞，或者更为一般的圆环，即0≤rR<|z|<rL≤∞。



2、性质2；当且仅当x[n]的z变换的收敛域包括单位圆时，x[n]的博里叶变换才绝对收敛。



3、性质3；收敛域内不能包含任何极点。



4、性质4；若x[n]是一个有限长序列，即一个序列除在优先区间-∞<N1≤n≤N2<无穷大内，其余为零，那么其收敛域就是整个z平面，可能z=0或z=∞除外。



5、性质5；若X[n]是一个右边序列，即一个序列在n<N1<∞为零，那么其收敛域是从X(z)中最外面(即最大幅度)的有限极点向外延伸至(可能包括)z=∞。



6、性质6；若X[n]是一个左边序列，即一个序列在n>N2>∞为零，那么其收敛域是从X(z)中最里面(即最小幅度)的非零极点向内延伸至(可能包括)z=0。



性质7；一个双边序列是一个无限长序列，它既不是右边，也不是左边的。若x[n]是双边序列，那么其收敛域一定由z平面的一个圆环组成，其内、外边界均由某一极点所界定，而且一句性质3，其内边界也不能包含任何极点。



8、性质8；收敛域必须是一个连通的区域。





三、z逆变换

一）、观察法

1、求逆变换的一个简单方法，就是由某些熟悉的，或凭观察就能辨认出的变换所构成的。基本变换对表是一个很好的参考。



二）、部分分式展开法

1、为了看出如何求得部分分式展开，假设X(z)表示成z-1的多项式之比，即

![](https://img-blog.csdn.net/20180908155039979?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.9)

这样的z变换在线性时不变系统的研究中常常出现。一种等效表示是

![](https://img-blog.csdn.net/2018090815553288?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式（3.10）

式（3.10）指出，对于这样的函数，在假设a0,b0,aN，和bM为非零的有限z平面的非零区域中将有M个零点和N个极点。另外，若M>N个极点在z=0处；或者，若N>M，由N-M个零点在z=0处。



2、为了求得式(3.9)中X(z)的部分分式展开，注意到，将X(z)可以表示成如下形式是最方便的；

![](https://img-blog.csdn.net/20180908160104258?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.11)

式中ck是X(z)的非零值零点，dk是X(z)的非零值极点。

1）、若M<N，并且极点都是一阶的，那么。X(z)就能表示为

![](https://img-blog.csdn.net/20180908160421768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式（3.12）

很明显，式(3.11)中这些分式的公共分母与式(3.10)中的分母是相同的。将式(3.11)两边乘以![](https://img-blog.csdn.net/20180908160734368?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)并对z=dk求值，系统Ak就能由下式求得；

![](https://img-blog.csdn.net/20180908160903911?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.13)

很显然，将式(3.12)中的各项相加进呢赶得到分子，它是最多为z-1的(N-1)次阶。

2）、若M≥N，完整的部分分式就有如下形式；

![](https://img-blog.csdn.net/20180908161229593?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.14)

如果给出的是形式如式(3.9)的有理函数，且M≥N，那么Br就可以用长除法以分子来得到，已知除到余因式的结束低于分母的结束为止。各Ak仍然可以用式(3.13)求得。

3）、如果X(z)有多重极点，且M≥N，则式(3.14)应进一步修改，特别是若X(z)有一个阶数为s的极点在z=di，而其余全部极点都是一阶的，那么式(3.14)就变为

![](https://img-blog.csdn.net/20180908162202546?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.15)

系数Ak和Br，扔如上述求得，系数Cm由下式求得：

![](https://img-blog.csdn.net/20180908162358972?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)式(3.16)

式(3.15)是对于在M≥N且di为s阶极点情况下，一个有理z变换表示成z-1函数的部分分式展开的一般形式。如果有几个多重极点，那么每一个多重极点将会有如式(3.15)的第三个和式一般的项。如果没有多重极点，式(3.15)就简化为式（3.14）。如果分子的阶小于分母的阶(M<N)，那么多项式的这一项就从(3.14)和式(3.16)中消失，变为式(3.12)。



三）、幂级数展开法

1、z变换的定义式是一个劳伦级数，序列x[n]的值时z-n的系数。因此，当z变换由如下的幂级数形式给出时；

![](https://img-blog.csdn.net/20180908163323653?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

就能通过且z-1的适当的幂的系数来确定该序列的任何特定值。求当M≥N是部分分式展开式中多项式部分的逆变换正式运用此方法。该方法叶适用于有限长序列，因为X(z)可能比z-1多项式复杂。




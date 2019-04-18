# Root test & Ratio test - TaigaComplex求职中 - 博客园







# [Root test & Ratio test](https://www.cnblogs.com/TaigaCon/p/6790931.html)





# 几何级数（Geometric Series/Geometric Progression）

Root test与Ratio test都依赖于几何级数求和理论，因此这里先讨论该理论。

在数学上，几何级数，也就是几何序列，该序列有以下形式

$a , ar, ar^2, ar^3, ar^4,…,ar^n \qquad for \  r\neq 0 $

$r$称为公比（common ratio）。



#### 几何级数求和

把该序列的所有项相加，得

$\displaystyle{S_n =  \sum_{k = 0}^{n}ar^{k} = a+ar+ar^2+\cdot\cdot\cdot+ar^n }$

和式$S_n$与公比$r$相乘，得到

$\displaystyle{ rS_n = \sum_{k = 0}^{n}ar^{k+1} = ar+ar^2+ar^3+\cdot\cdot\cdot+ar^n+ar^{n+1} }$

因此和式有一个简便计算方法

$\displaystyle{ S_n = \frac{S_n – rS_n}{1-r} = \frac{a-ar^{n+1}}{1-r} }$



#### 无穷几何级数求和

当$n\to\infty$时，

如果$|r|<1$，

$\displaystyle{\lim_{ n\to\infty } S_n = \frac{a}{1-r} }$

此时$S_n$收敛（converge）

否则$S_n$趋于无穷，即发散（diverge）





# 根式判别法（Root test）

这里有必要细解释一下[limsup](https://en.wikipedia.org/wiki/Limit_superior_and_limit_inferior)这个符号，limsup，liminf分别是一个序列处于极限处的上下边界（In mathematics, the limit inferior and limit superior of a sequence can be thought of as limiting (i.e., eventual and extreme) bounds on the sequence.）

按照上述定义，$\bar{l} = \displaystyle{ \limsup_{n\to\infty}x_n }$可以解释为序列$x_n$中存在一个足够大的自然数$N$，对于所有$n>N$，都有上界$\bar{l}$。（请看wiki/Limsup and Liminf /The case of sequences of real numbers部分）



#### 定义

有一个级数$\displaystyle{ \sum_{n = 1}^{\infty} a_n }$，该级数可以是实数或者复数，该级数是收敛或者发散，取决于

$l = \displaystyle{ \limsup_{n\to\infty}|a_n|^{1/n} }$

如果$l>1$，那么该级数发散

如果$l<1$，那么该级数收敛



#### 证明

当$l < 1$，则存在实数$\epsilon > 0$使得$l + \epsilon < 1$，即

$\displaystyle{ \limsup_{n\to\infty}|a_n|^{1/n} < l+\epsilon < 1}$

$\displaystyle{ \limsup_{n\to\infty}|a_n| < (l+\epsilon)^n<1 }$

根据$limsup$的定义知道，存在一个足够大的自然数$N$，使得序列$|a_n|$有小于$(l+\epsilon)^n$的上界；

又由于$l+\epsilon < 1$，根据几何级数求和理论得知，$\displaystyle{ \sum_{n=1}^{\infty}(l+\epsilon)^n }$收敛。

因此：存在一个足够大的自然数$N$使得所有的$n>N$，都有$\displaystyle{\sum_{n = N}^{\infty}|a_n|}$收敛，所以$\displaystyle{\sum_{n = 1}^{\infty}|a_n|}$以及$\displaystyle{\sum_{n = 1}^{\infty}a_n}$同样也收敛。



 当$l > 1$，则存在实数$\epsilon > 0$使得$l - \epsilon > 1$，即

$\displaystyle{ \limsup_{n\to\infty}|a_n|^{1/n} > l-\epsilon > 1}$

$\displaystyle{ \limsup_{n\to\infty}|a_n| > (l-\epsilon)^n>1 }$

根据$limsup$的定义知道，存在一个足够大的自然数$N$，使得序列$|a_n|$都大于$(l-\epsilon)^n$，即

$\displaystyle{\lim_{n\to\infty}|a_n| > 1}$

而一个收敛序列在$n\to\infty$处的项应该有$a_n\to \infty$，即

令$s = \displaystyle{\sum_{n=1}^{\infty}a_n }$，那么$s_N = \displaystyle{\sum_{n=1}^{N}a_n }\to s \ as\ N\to\infty$，同样地有$s_{N-1} \to s \ as\ N\to\infty$

因此

$a_N=\displaystyle{ \sum_{n=1}^N a_n – \sum_{n=1}^{N-1}a_n=s_N-s_{N-1} \to s-s = 0 \ as \ N\to\infty  }$

这就与上述结果相悖了，因此当$l>1$时，级数发散。





# 比式判别法（Ratio test）

#### 定义

有一个级数$\displaystyle{ \sum_{n = 1}^{\infty} a_n }$，该级数可以是实数或者复数，该级数是收敛或者发散，取决于

$l = \displaystyle{ \limsup_{n\to\infty}\left|\frac{a_{n+1}}{a_n}\right| }$

如果$l>1$，那么该级数发散

如果$l<1$，那么该级数收敛



#### 证明

当$l < 1$，则存在实数$\epsilon > 0$使得$l + \epsilon < 1$，因此

$\displaystyle{ \limsup_{n\to\infty}\left|\frac{a_{n+1}}{a_n}\right | < l+\epsilon}$

也就是说存在一个足够大的自然数$N$，对于所有的$n>N$，都有

$\displaystyle{\left|\frac{a_{n+1}}{a_n}\right | < l+\epsilon }$

因此，

$|a_n| = \left|\frac{a_n}{a_{n-1}}\right|\left|\frac{a_{n-1}}{a_{n-2}}\right|\cdot\cdot\cdot\left|\frac{a_{N+2}}{a_{N+1}}\right| |a_{N+1}| < (l+\epsilon)^{n-N-1}|a_{N+1}|$

根据无穷几何级数求和理论，$\displaystyle{ \sum_{n=1}^{\infty}(l+\epsilon)^n }$收敛。

因此：存在一个足够大的自然数$N$使得所有的$n>N$，都有$\displaystyle{\sum_{n = N}^{\infty}|a_n|}$收敛，所以$\displaystyle{\sum_{n = 1}^{\infty}|a_n|}$以及$\displaystyle{\sum_{n = 1}^{\infty}a_n}$同样也收敛。



当$l>1$，则存在实数$\epsilon > 0$使得$l - \epsilon >1$

那么，存在一个足够大的自然数$N$，对于所有的$n>N$，都有

$\displaystyle{\left|\frac{a_{n+1}}{a_n}\right | > l-\epsilon}$

因此，

$|a_n| = \left|\frac{a_n}{a_{n-1}}\right|\left|\frac{a_{n-1}}{a_{n-2}}\right|\cdot\cdot\cdot\left|\frac{a_{N+2}}{a_{N+1}}\right| |a_{N+1}| > (l-\epsilon)^{n-N-1}|a_{N+1}|$

当$n\to\infty$时，$(l-\epsilon)^{n-N-1}|a_{N+1}|\to\infty$，而级数收敛需要$a_n \to 0\ as\  n\to\infty$，因此级数发散。















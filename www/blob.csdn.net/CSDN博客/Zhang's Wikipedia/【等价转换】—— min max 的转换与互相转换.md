
# 【等价转换】—— min-max 的转换与互相转换 - Zhang's Wikipedia - CSDN博客


2018年07月28日 08:33:20[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1265



## 0. min 与 max 的转换

$$
\left\{
\begin{split}
&\max \left(X, Y\right)=X+Y-\min\left(X,Y\right)\\
&\min\left(X,Y\right)=X+Y-\max\left(X,Y\right)\\
&\min\left(X,Y\right)+\max \left(X, Y\right)=X+Y
\end{split}
\right.
$$

## 1.
## \max(X,Y)=n
## 的概率求解
将$\max(X,Y)$或$\min(X,Y)$首先转换为联合概率的形式，进一步通过如变量间的独立性简化计算：
$\max(X,Y)=n$可拆分成如下三项：
X=n, Y<n
X<n, Y=n
X=n, Y=n
当然可将第三项合并到前两项的任意一项中。
则其：

$$
\begin{split}
Pr(\max(X,Y)=n)&=Pr(X<n,Y=n)+Pr(X= n, Y<n)+Pr(X=n, Y=n)\\
&=Pr(X< n, Y=n) + Pr(X=n, Y\leq n)
\end{split}
$$
同理可将$\min(X,Y)=n$拆分为如下三项：
X>n, Y=n
X=n, Y>n
X=n, Y=n


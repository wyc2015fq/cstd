
# 漫步数学分析二十七——Stone-Weierstrass定理 - 蜗牛 - CSDN博客


2017年03月05日 21:04:33[会敲键盘的猩猩](https://me.csdn.net/u010182633)阅读数：5359标签：[Weierstras																](https://so.csdn.net/so/search/s.do?q=Weierstras&t=blog)[伯恩斯坦多项式																](https://so.csdn.net/so/search/s.do?q=伯恩斯坦多项式&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Weierstras&t=blog)个人分类：[漫步数学分析																](https://blog.csdn.net/u010182633/article/category/6571925)


在讨论连续函数与一致收敛时，最基本的两个结论是上篇文章讨论的Arzela-Ascoli定理以及本文要讨论的斯通-魏尔斯特拉斯(Stone-Weierstrass)定理。
斯通-魏尔斯特拉斯定理主要是为了说明任何连续函数都可以用更简单的函数来一致逼近，像多项式。这样的多项式近似技术在理论以及数值计算中非常重要。
我们先从实轴上特殊情况的结论开始，它首先是被魏尔斯特拉斯证明的，但是我们这里用伯恩斯坦(Bernstein)的表述形式。
$\textbf{定理11}$令$f:[0,1]\to R$是连续函数且令$\varepsilon>0$，那么存在一个多项式$p(x)$使得$\Vert p-f\Vert<\varepsilon$。事实上，伯恩斯坦多项式序列

$$
p_n(x)=\sum_{k=0}^n\begin{pmatrix}n\\k\end{pmatrix}f\left(\frac{k}{n}\right)x^k(1-x)^{n-k}
$$
在$n\to\infty$时，一致收敛到$f$，其中

$$
\begin{pmatrix}
n\\
k
\end{pmatrix}
\frac{n!}{k!(n-k)!}
$$
表示二项式系数。
命题的前半部分是后半部分的推论，如果了解一点概率论的话就很容易理解后半部分。对于该定理的理解与证明，伯恩斯坦的概率知识无疑帮助了他。
接下来，考虑一枚头朝上概率为$x$的硬币，自然另一面朝上的概率为$1-x$，那么投掷$n$次，$k$次头朝上的概率为

$$
\begin{pmatrix}
n\\
k
\end{pmatrix}
x^k(1-x)^{n-k}
$$
假设现在有一个赌博游戏，如果投掷$n$次硬币有$k$次头朝上，那么就支付$f(k/n)$美元，(玩了很多次这个游戏后)平均值为

$$
\sum_{k=0}^n\begin{pmatrix}n\\k\end{pmatrix}f(k/n)x^k(1-x)^{n-k}=p_n(x)
$$
现在考虑$n$非常大，也就是投掷了很多次，那么我们期望在这个游戏中，$k/n$将非常靠近$x=k$次头朝上的概率，那么我们的平均费用将非常靠近$f(x)$，因此当$n$充分大时，我们期望$p_n(x)$靠近$f(x)$。这是这个结论直观的理解，实际证明有点复杂，这里不再赘述。(感兴趣的可以参考实分析的相关教材)
即便对于像$f(x)=\sqrt{x}$这样简单的函数，找到近似多项式也是不容易的。
我们现在换一种说法，令$\ell$表示所有多项式$p:[0,1]\to R$的集合，那么定理的前半部分说明$\mathscr{P}$在$\mathscr{C}([0,1],R)$上是稠密的(dense)；即$\text{cl}(\mathscr{P})=\mathscr{C}([0,1],R)$.
斯通发现了该定理更一般的情况，他允许集合比$[0,1]$更一般并且用满足某个性质的一般函数族来代替$\mathscr{P}$，这个证明充分应用了上面的特殊情况，并且在各种分析的分支中非常有用。(例如，我们会在之后的傅里叶分析中讨论它的应用)
$\textbf{定理12}$令$A\subset R^n$是紧集并且$\mathscr{B}\subset\mathscr{C}(A,R)$满足
\mathscr{B}是一个代数(algebra)；即，f,g\in\mathscr{B},\alpha\in R\Rightarrow f+g\in\mathscr{B},f\cdot g\in\mathscr{B},\alpha f\in\mathscr{B};
常函数x\mapsto 1位于\mathscr{B}中；
\mathscr{B}分离点；即，对于x,y\in A,x\neq y，那么存在f\in\mathscr{B}使得f(x)\neq f(y)。
那么$\mathscr{B}$在$\mathscr{A,R}$中是稠密的；即，$\text{cl}(\mathscr{B})=\mathscr{C}(A,R)$。
$\textbf{例1：}$令$p_n$是一致收敛的多项式序列且$f=\lim_{n\to\infty}p_n$，$f$一定可微吗？
$\textbf{解：}$否。根据定理11，任何连续函数都有这样的极限并且许多连续函数都不可微，例如

$$
f(x)=
\begin{cases}
0&0\leq x\leq\frac{1}{2}\\
2x-1&\frac{1}{2}\leq x\leq 1
\end{cases}
$$
$\textbf{例2：}$直接利用定理11或12证明$[a,b]$上的多项式在$\mathscr{C}([a,b],R)$上是稠密的。
$\textbf{解：}$$(a)$我们知道如果$f:[0,1]\to R$是连续函数且$\varepsilon>0$，那么存在一个多项式$p$使得$\Vert f-p\Vert<\varepsilon$，接下里，我们令$g:[a,b]\to R$是连续函数并进行缩放

$$
f(x)=g(x(b-a)+a),\quad 0\leq x\leq 1
$$
所以$f:[0,1]\to R$。向上面那样找一个函数$p$并进行缩放得

$$
q(x)=p\left(\frac{(x-a)}{(b-a)}\right),\quad a\leq x\leq b
$$
所以$q:[a,b]\to R$。因此$q$也是一个多项式，我们断定$\Vert g-q\Vert<\varepsilon$。事实上，

$$
|g(x)-q(x)|=\left|f\left(\frac{(x-a)}{(b-a)}\right)-p\left(\frac{(x-a)}{(b-a)}\right)\right|
$$
所以$\Vert g-q\Vert<\varepsilon$，这是因为$\Vert f-p\Vert<\varepsilon$，由此得到$[a,b]$上的多项式是稠密的。
$(b)$在定理12中，我们令$A=[a,b]$，集合$\mathscr{B}=\{q\in\mathscr{C}([a,b],R)|q\text{是一个多项式}\}$，那么$\mathscr{B}$明显满足$\textrm{(i),(ii)}$，当$x\neq y$时，我们令

$$
f(t)=t
$$
所以$f(x)\neq f(y)$，故也满足条件$\textrm{(iii)}$，所以根据定理可知$\mathscr{B}$是稠密的。


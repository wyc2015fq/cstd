
# 漫步最优化四十——Powell法(上) - 蜗牛 - CSDN博客


2017年11月08日 22:45:31[会敲键盘的猩猩](https://me.csdn.net/u010182633)阅读数：378



$\textbf{平凡无所谓，}$
$\textbf{平淡无所谓，}$
$\textbf{但求每天早晨能望见你，}$
$\textbf{这已经足够满足。}$
$\textbf{身边的一切如风，}$
$\textbf{而你让我有了根，}$
$\textbf{就让我爱你多些，}$
$\textbf{再多些，甚至满泻，}$
$\textbf{与你一生一世。}$
$\textbf{——畅宝宝的傻逼哥哥}$
过去使用最广泛的共轭方向法是由Powell提出来的，这个方法与共轭梯度法一样，开始也是来自凸二次问题，但是它也成功应用于非二次问题。
Powell法最显著的特征就是通过一系列线搜索生成共轭方向，所用的技术基于下面的定理：
$\textbf{定理1：}$如果凸二次问题

$$
f(\mathbf{x})=a+\mathbf{x}^T\mathbf{b}+\frac{1}{2}\mathbf{x}^T\mathbf{Hx}
$$
在直线

$$
\mathbf{x}=\mathbf{x}_a+\alpha\mathbf{d}_a
$$
与

$$
\mathbf{x}=\mathbf{x}_b+\alpha\mathbf{d}_b
$$
上分别对$\alpha$最小化，得到的最小点分别为$\mathbf{x}_a^*,\mathbf{x}_b^*$，如图1所示。
如果$\mathbf{d}_b=\mathbf{d}_a$，那么向量$\mathbf{x}_b^*-\mathbf{x}_a^*$与$\mathbf{d}_a$(或者$\mathbf{d}_b$)共轭。
$\textbf{证明：}$如果$f(\mathbf{x}_a+\alpha\mathbf{d}_a),f(\mathbf{x}_b+\alpha\mathbf{d}_b)$对$\alpha$最小化，那么

$$
\begin{align}
\frac{df(\mathbf{x}_a+\alpha\mathbf{d}_a)}{d\alpha}=\mathbf{d}_a^T\mathbf{g}(\mathbf{x}_a^*)=0\tag{1a}\\
\frac{df(\mathbf{x}_b+\alpha\mathbf{d}_b)}{d\alpha}=\mathbf{d}_b^T\mathbf{g}(\mathbf{x}_b^*)=0\tag{1b}
\end{align}
$$
因为

$$
\begin{align}
\mathbf{g}(\mathbf{x}_a^*)=\mathbf{b}+\mathbf{Hx}_a^*\tag{2a}\\
\mathbf{g}(\mathbf{x}_b^*)=\mathbf{b}+\mathbf{Hx}_b^*\tag{2b}
\end{align}
$$
因为$\mathbf{d}_b=\mathbf{d}_a$，所以由等式1与2可得

$$
\mathbf{d}_a^T\mathbf{H}(\mathbf{x}_b^*-\mathbf{x}_a^*)=0
$$
因此，向量$\mathbf{x}_b^*-\mathbf{x}_a^*$与方向$\mathbf{d}_a$(或者$\mathbf{d}_b$)共轭，证毕。$||$
在Powell算法中，假设初始点为$\mathbf{x}_{00}$，$n$个线性无关方向为$\mathbf{d}_{01},\mathbf{d}_{02},\ldots,\mathbf{d}_{0n}$，并且每次迭代执行一系列线搜索。虽然可以使用任意的线性无关方向集合，但是出于方便，我们使用坐标方向集。

![这里写图片描述](https://img-blog.csdn.net/20171108222935163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171108222935163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图1
第一次迭代的时候，$f(\mathbf{x})$从初始点$\mathbf{x}_{00}$开始，在方向$\mathbf{d}_{01},\mathbf{d}_{02},\ldots,\mathbf{d}_{0n}$上最小化分别得到点$\mathbf{x}_{01},\mathbf{x}_{02},\ldots,\mathbf{x}_{0n}$，如图2所示，新的方向$\mathbf{d}_{0(n+1)}$为

$$
\mathbf{d}_{0(n+1)}=\mathbf{x}_{0n}-\mathbf{x}_0
$$
且$f(\mathbf{x})$在这个方向上最小化得到新的点$\mathbf{x}_{0(n+1)}$，然后更新方向集为

$$
\begin{align}
\mathbf{d}_{11}&=\mathbf{d}_{02}\notag\\
\mathbf{d}_{12}&=\mathbf{d}_{03}\notag\\
&\vdots\notag\\
\mathbf{d}_{1(n-1)}&=\mathbf{d}_{0n}\notag\\
\mathbf{d}_{1n}&=\mathbf{d}_{0(n+1)}\tag3
\end{align}
$$
第一次迭代的效果就是$f(\mathbf{x})$减少了$\Delta f=f(\mathbf{x}_{00})-f(\mathbf{x}_{0(n+1)})$并且同时删除了$\mathbf{d}_{01}$加入了$\mathbf{d}_{0(n+1)}$。
第二次得带执行同样的过程，从点

$$
\mathbf{x}_{10}=\mathbf{x}_{0(n+1)}
$$
开始，$f(\mathbf{x})$在方向$\mathbf{d}_{11},\mathbf{d}_{12},\ldots,\mathbf{d}_{1n}$上最小化分别得到点$\mathbf{x}_{11},\mathbf{x}_{12},\ldots,\mathbf{x}_{1n}$，如图3所示，然后生成新的方向$\mathbf{d}_{1(n+1)}$

$$
\mathbf{d}_{1(n+1)}=\mathbf{x}_{1n}-\mathbf{x}_{10}
$$
$f(\mathbf{x})$在方向$\mathbf{d_{1(n+1)}}$上最小化得到点$\mathbf{x}_{1(n+1)}$。因为

$$
\mathbf{d}_{1n}=\mathbf{d}_{0(n+1)}
$$
所以$\mathbf{d}_{1(n+1)}$与$\mathbf{d}_n$共轭，因此我们令

$$
\begin{align}
\mathbf{d}_{21}&=\mathbf{d}_{12}\notag\\
\mathbf{d}_{22}&=\mathbf{d}_{13}\notag\\
&\vdots\notag\\
\mathbf{d}_{2(n-1)}&=\mathbf{d}_{1n}\notag\\
\mathbf{d}_{2n}&=\mathbf{d}_{1(n+1)}\tag4
\end{align}
$$
新的方向解将包含一对共轭方向，即$\mathbf{d}_{2(n-1)},\mathbf{d}_{2n}$。
用同样的方式执行上面的过程，每次迭代都会增加一个共轭方向。Powell法需要$n(n+1)$次线搜索，因为每次迭代包含$(n+1)$次线搜索，共需要$n$次迭代，Powell算法实现如下：

$$
\begin{align*}
&\textbf{算法1：共轭梯度算法}\\
&\textbf{步骤1}\\
&\text{输入}\mathbf{x}_{00}\text{并初始化容忍误差}\varepsilon\\
&\text{令}\\
&\mathbf{d}_{01}=[x_{01}\ 0\ \cdots\ 0]^T\\
&\mathbf{d}_{02}=[0\ x_{02}\ \cdots\ 0]^T\\
&\vdots\\
&\mathbf{d}_{0n}=[0\ 0\ \cdots\ x_{0n}]^T\\
&\text{令}k=0\\
&\textbf{步骤2}\\
&\text{对于}i=1\text{从}n\\
&\quad\text{求出}\alpha_{ki},\text{就是最小化}f(\mathbf{x}_{k(i-1)}+\alpha\mathbf{d}_{ki})\text{的值}\alpha\\
&\quad\text{令}\mathbf{x}_{ki}=\mathbf{x}_{k(i-1)}+\alpha_{ki}\mathbf{d}_{ki}\\
&\textbf{步骤3}\\
&\text{生成新方向}\mathbf{d}_{k(n+1)}=\mathbf{x}_{kn}-\mathbf{x}_{k0}\\
&\text{求出}\alpha_{k(n+1)},\text{就是最小化}f(\mathbf{x}_0+\alpha\mathbf{d}_{k(n+1)})\text{的值}\alpha\\
&\text{令}\mathbf{x}_{k(n+1)}=\mathbf{x}_{k0}+\alpha_{k(n+1)}\mathbf{d}_{k(n+1)}\\
&\text{计算}f_{k(n+1)}=f(\mathbf{x}_{k(n+1)})\\
&\textbf{步骤4}\\
&\text{如果}\lVert\alpha_{k(n+1)}\mathbf{d}_{k(n+1)}\rVert<\varepsilon,\text{输出}\mathbf{x}^*=\mathbf{x}_{k(n+1)},f(\mathbf{x}^*)=f_{k(n+1)}\text{算法结束}\\
&\textbf{步骤5}\\
&\text{更新方向}\\
&\mathbf{d}_{(k+1)1}=\mathbf{d}_{k2}\\
&\mathbf{d}_{(k+1)2}=\mathbf{d}_{k3}\\
&\vdots\\
&\mathbf{d}_{(k+1)n}=\mathbf{d}_{k(n+1)}\\
&\text{令}\mathbf{x}_{(k+1)0}=\mathbf{x}_{k(n+1)},k=k+1,\text{令}k=k+1\text{然后回到步骤2}
\end{align*}
$$

![这里写图片描述](https://img-blog.csdn.net/20171108222818761?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171108222818761?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图2

![这里写图片描述](https://img-blog.csdn.net/20171108222844586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171108222844586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图3

[
						](https://img-blog.csdn.net/20171108222844586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
[
	](https://img-blog.csdn.net/20171108222844586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDE4MjYzMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

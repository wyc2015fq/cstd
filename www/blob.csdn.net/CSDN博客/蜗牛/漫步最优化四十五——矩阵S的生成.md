
# 漫步最优化四十五——矩阵S的生成 - 蜗牛 - CSDN博客


2017年11月23日 21:53:39[会敲键盘的猩猩](https://me.csdn.net/u010182633)阅读数：717



$\textbf{想赖着你每一天，}$
$\textbf{耽误多一秒都不愿意。}$
$\textbf{想把你捧在手心，}$
$\textbf{给你我最奢侈的温柔。}$
$\textbf{感恩节快乐，}$
$\textbf{感谢有你。}$
$\textbf{——畅宝宝的傻逼哥哥}$
令$f(\mathbf{x})\in C^2$是$E^n$中的函数并假设$f(\mathbf{x})$在点$\mathbf{x}_k,\mathbf{x}_{k+1}$处的梯度分别为$\mathbf{g}_k,\mathbf{g}_{k+1}$，如果

$$
\begin{equation}
\mathbf{x}_{k+1}=\mathbf{x}_k+\mathbf{\delta_k}
\end{equation}
$$
那么根据泰勒级数可以得出$\mathbf{g}_{k+1}$的元素为

$$
g_{(k+1)m}=g_{km}+\sum_{i=1}^n\frac{\partial g_{km}}{\partial x_{ki}}\delta_{ki}+\frac{1}{2}\sum_{i=1}^n\sum_{j=1}^n\frac{\partial^2g_{km}}{\partial x_{ki}\partial x_{kj}}\delta_{ki}\delta_{kj}+\cdots
$$
其中$m=1,2,\ldots,n$。接下来如果$f(\mathbf{x})$是二次的，那么$f(\mathbf{x})$的二阶导为常数，$\mathbf{g}_{km}$的二阶导为令，所以

$$
g_{(k+1)m}=g_{km}+\sum_{i=1}^n\frac{\partial g_{km}}{\partial x_{ki}}\delta_{ki}
$$
又因为

$$
g_{km}=\frac{\partial f_k}{\partial x_{km}}
$$
因此我们有

$$
g_{(k+1)m}=g_{km}+\sum_{i=1}^n\frac{\partial^2f_k}{\partial x_{ki}\partial x_{km}}\delta_{ki}
$$
其中$m=1,2,\ldots,n$，也就是

$$
\mathbf{g}_{k+1}=\mathbf{g}_k+\mathbf{H}\mathbf{\delta}_k
$$
其中$\mathbf{H}$是$f(\mathbf{x})$的海森矩阵。或者我们可以写成

$$
\begin{equation}
\mathbf{\gamma}_k=\mathbf{H}\mathbf{\delta}_k
\end{equation}
$$
其中

$$
\begin{align}
\mathbf{\delta}_k=\mathbf{x}_{k+1}-\mathbf{x}_k\\
\mathbf{\gamma}_k=\mathbf{g}_{k+1}-\mathbf{g}_k
\end{align}
$$
上面的分析表明，如果$f(\mathbf{x})$在点$\mathbf{x}_k,\mathbf{x}_{k+1}$处的梯度已知，那么可以得到上面的关系，这给出了矩阵$\mathbf{H}$的部分信息。因为$\mathbf{H}$中有$n^2$个未知量(或者如果$\mathbf{H}$是实对称矩阵，那么存在$n(n+1)/2$个未知量)，而等式2只提供了$n$个等式，不能唯一的确定$\mathbf{H}$，然而，如果对连续$n+1$个点的梯度进行估计，也就是点$\mathbf{x}_0,\mathbf{x}_1,\ldots,\mathbf{x}_n$，使得$\mathbf{x}$的变化量组成一个线性无关向量的集合，那么我们就有充分的信息来唯一的确定$\mathbf{H}$，其中$\mathbf{x}$的变化量就是

$$
\begin{align*}
\mathbf{\delta}_0=\mathbf{x}_1-\mathbf{x}_0\\
\mathbf{\delta}_1=\mathbf{x}_2-\mathbf{x}_1\\
\vdots\\
\mathbf{\delta}_{n-1}=\mathbf{x}_n-\mathbf{x}_{n-1}
\end{align*}
$$
为了证明这个事实，我们将等式2给出的$n$个等式重新写成

$$
\begin{equation}
[\mathbf{\gamma}_0\ \mathbf{\gamma}_1\ \cdots\ \mathbf{\gamma}_{n-1}]=\mathbf{H}[\mathbf{\delta}_0\ \mathbf{\delta}_1\ \cdots\ \mathbf{\delta}_{n-1}]
\end{equation}
$$
因此

$$
\mathbf{H}=[\mathbf{\gamma}_0\ \mathbf{\gamma}_1\ \cdots\ \mathbf{\gamma}_{n-1}][\mathbf{\delta}_0\ \mathbf{\delta}_1\ \cdots\ \mathbf{\delta}_{n-1}]^{-1}
$$
如果$\mathbf{\delta}_0,\mathbf{\delta}_1,\ldots,\mathbf{\delta}_{n-1}$构成一个线性无关向量集，那么解就存在。
利用上面的原则可以构造下面的算法：

$$
\begin{align*}
&\textbf{算法1：替代的牛顿法}\\
&\textbf{步骤1}\\
&\text{输入}\mathbf{x}_{00}\text{并初始化容忍误差}\varepsilon\\
&\text{令}k=0\\
&\text{输入一个线性关系向量集合}\mathbf{\delta}_0,\mathbf{\delta}_1,\ldots,\mathbf{\delta}_{n-1}\\
&\textbf{步骤2}\\
&\text{计算}\mathbf{g}_{00}\\
&\textbf{步骤3}\\
&\text{对于}i=0\text{到}n-1\\
&\quad\text{令}\mathbf{x}_{k(i+1)}=\mathbf{x}_{ki}+\mathbf{\delta}_i\\
&\quad\text{计算}\mathbf{g}_{k(i+1)}\\
&\quad\text{令}\mathbf{\gamma}_{ki}=\mathbf{g}_{k(i+1)}-\mathbf{g}_{ki}\\
&\textbf{步骤4}\\
&\text{计算}\mathbf{H}_k=[\mathbf{\gamma}_{k0}\ \mathbf{\gamma}_{k1}\ \cdots\ \mathbf{\gamma}_{k(n-1)}][\mathbf{\delta}_0\ \mathbf{\delta}_1\ \cdots\ \mathbf{\delta}_{n-1}]^{-1}\\
&\text{计算}\mathbf{S}_k=\mathbf{H}_k^{-1}\\
&\textbf{步骤5}\\
&\text{令}\mathbf{d}_k=-\mathbf{S}_k\mathbf{g}_{k0}\\
&\text{找出}\alpha,\text{也即是最小化}f(\mathbf{x}_{k0}+\alpha\mathbf{d}_k)\text{的}\alpha\text{值}\\
&\text{令}\mathbf{x}_{(k+1)0}=\mathbf{x}_{k0}+\alpha_k\mathbf{d}_k\\
&\textbf{步骤6}\\
&\text{如果}\lVert\alpha_k\mathbf{d}_k\rVert<\varepsilon,\text{输出}\mathbf{x}_k^*=\mathbf{x}_{(k+1)0}\text{并且}f(\mathbf{x}^*)=f(\mathbf{x}_{(k+1)0})\text{算法结束}\\
&\textbf{步骤7}\\
&\text{令}k=k+1\text{然后回到步骤3}
\end{align*}
$$
上面的算法基本上是牛顿法的实现，唯一的不同是利用数据来得到$\mathbf{H}^{-1}$。对于凸二次问题，上面的算法一次迭代就得的问题的解，因此非常有效，然而对于非二次问题，同基于牛顿法的其他一样，它也存在相同的缺点。首先需要矩阵的逆，其次必须检查$\mathbf{H}_k$的正定性。
下面介绍一种消除矩阵逆的策略。假设已知正定实对称矩阵$\mathbf{S}_k$，$\mathbf{H}^{-1}$的近似值，然后计算拟牛顿方向为

$$
\begin{equation}
\mathbf{d}_k=-\mathbf{S}_k\mathbf{g}_k
\end{equation}
$$
然后求$\alpha$，也就是最小化$f(\mathbf{x}_k+\alpha\mathbf{d}_k)$的$\alpha$值。对于凸二次问题，

$$
\begin{equation}
\alpha_k=\frac{\mathbf{g}_k^T\mathbf{S}_k\mathbf{g}_k}{(\mathbf{S}_k\mathbf{g}_k)^T\mathbf{H}(\mathbf{S}_k\mathbf{g}_k)}
\end{equation}
$$
其中$\mathbf{S}_k,\mathbf{H}$是正定的。显然，当$\mathbf{x}_k$不是问题的解$\mathbf{x}^*$时，$\alpha_k$大于零，然后确定$\mathbf{x}$的变化量为

$$
\begin{equation}
\mathbf{\delta}_k=\alpha_k\mathbf{d}_k
\end{equation}
$$
根据等式1得到新点$\mathbf{x}_{k+1}$。通过计算$\mathbf{x}_k,\mathbf{x}_{k+1}$处的梯度，利用等式4得到梯度的变化量$\mathbf{\gamma}_k$，接下里对$\mathbf{S}_k$进行修正得到

$$
\begin{equation}
\mathbf{S}_{k+1}=\mathbf{S}_k+\mathbf{C}_k
\end{equation}
$$
其中$\mathbf{C}_k$是$n\times n$修正矩阵，从数据计算得出的。从初始点$\mathbf{x}_0$,初始正定矩阵$\mathbf{S}_0,\mathbf{S}_0=\mathbf{I}_n$开始，迭代使用上面的过程，得到序列$\mathbf{\delta}_0,\mathbf{\delta}_1,\ldots,\mathbf{\delta}_k,\mathbf{\gamma}_0,\mathbf{\gamma}_1,\ldots,\mathbf{\gamma}_k,\mathbf{S}_1,\mathbf{S}_2,\ldots,\mathbf{S}_{k+1}$。如果

$$
\begin{equation}
\mathbf{S}_{k+1}\mathbf{\gamma}_i=\mathbf{\delta}_i\quad\text{for}\ 0\leq i\leq k
\end{equation}
$$
然后对于$k=n-1$，我们可以写作

$$
\mathbf{S}_n[\mathbf{\gamma}_0\ \mathbf{\gamma}_1\ \cdots\ \mathbf{\gamma}_{n-1}]=[\mathbf{\delta}_0\ \mathbf{\delta}_1\ \cdots\ \mathbf{\delta}_{n-1}]
$$
或者

$$
\begin{equation}
\mathbf{S}_n=[\mathbf{\delta}_0\ \mathbf{\delta}_1\ \cdots\ \mathbf{\delta}_{n-1}][\mathbf{\gamma}_0\ \mathbf{\gamma}_1\ \cdots\ \mathbf{\gamma}_{n-1}]^{-1}
\end{equation}
$$
根据等式5与11可得

$$
\mathbf{S}_n=\mathbf{H}^{-1}
$$
接下来如果$k=n$，等式6到8得到

$$
\begin{align*}
\mathbf{d}_n=-\mathbf{H}^{-1}\mathbf{g}_n\\
\alpha_n=1\\
\mathbf{\delta}_n=-\mathbf{H}^{-1}\mathbf{g}_n
\end{align*}
$$
所以根据等式1可得

$$
\mathbf{x}_{n+1}=\mathbf{x}_n-\mathbf{H}^{-1}\mathbf{g}_n=\mathbf{x}^*
$$
上面的过程产生了一族拟牛顿算法，他们有一个基本属性：对于凸二次问题，$n+1$次迭代内中止，根据修正矩阵$\mathbf{C}_n$的差异从而得出各种各样的算法。
对于任何$\mathbf{C}_n$,$\mathbf{S}_{k+1}$必须满足等式10，并且下面的属性也是我们想要的：
向量\mathbf{\delta}_0,\mathbf{\delta}_1,\ldots,\mathbf{\delta}_{n-1}构成一个共轭方向集
正定矩阵\mathbf{S}_k产生正定矩阵\mathbf{S}_{k+1}
第一个属性确保共轭方向的性质能够应用到拟牛顿法上，第二个属性确保每次迭代中$\mathbf{d}_k$是下降方向。为了证明这个事实，考虑点$\mathbf{x}_k+\mathbf{\delta}_k$并令

$$
\mathbf{\delta}_k=\alpha\mathbf{d}_k
$$
其中

$$
\mathbf{d}_k=-\mathbf{S}_k\mathbf{g}_k
$$
对于$\alpha>0$，泰勒级数给出

$$
f(\mathbf{x}_k+\mathbf{\delta}_k)=f(\mathbf{x}_k)+\mathbf{g}_k^T\mathbf{\delta}_k+\frac{1}{2}\mathbf{\delta}_k^T\mathbf{H}(\mathbf{x}_k+c\mathbf{\delta}_k)\mathbf{\delta}_k
$$
其中$c$是常数，$0\leq c\leq 1$。消除$\mathbf{\delta}_k$，我们得到

$$
\begin{align*}
f(\mathbf{x}_k+\mathbf{\delta}_k)&=f(\mathbf{x}_k)-\alpha\mathbf{g}_k^T\mathbf{S}_k\mathbf{g}_k+o(\alpha\lVert\mathbf{d}_k\rVert)\\
&=f(\mathbf{x}_k)-[\alpha\mathbf{g}_k^T\mathbf{S}_k\mathbf{g}_k-o(\alpha\lVert\mathbf{d}_k\rVert)]
\end{align*}
$$
其中$o(\alpha\lVert\mathbf{d}_k\rVert)$是余项，比$\alpha\lVert\mathbf{d}_k\rVert$更快接近零。接下来如果$\mathbf{S}_k$是正定的，那么对于充分小的$\alpha>0$，我们有

$$
\alpha\mathbf{g}_k\mathbf{S}_k\mathbf{g}_k-o(\alpha\lVert\mathbf{d}_k\rVert)>0
$$
因为$\alpha>0,\mathbf{g}_k^T\mathbf{S}_k\mathbf{g}_k>0,o(\alpha\lVert\mathbf{d}_k\rVert)\to 0$，因此

$$
\begin{equation}
f(\mathbf{x}_k+\mathbf{\delta}_k)<f(\mathbf{x}_k)
\end{equation}
$$
也就是说如果$\mathbf{S}_k$是正定的，那么$\mathbf{d}_k$是下降方向。
从这点来说，属性2的重要性就很明显了。正定矩阵$\mathbf{S}_0$将产生正定矩阵$\mathbf{S}_1$，然后产生正定矩阵$\mathbf{S}_2$，如此不断进行，最后方向$\mathbf{d}_0,\mathbf{d}_1,\mathbf{d}_2,\ldots$将都是下降方向，这就确保算法能够收敛。


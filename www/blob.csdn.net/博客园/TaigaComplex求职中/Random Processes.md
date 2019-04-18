# Random Processes - TaigaComplex求职中 - 博客园







# [Random Processes](https://www.cnblogs.com/TaigaCon/p/9074973.html)





对于信号处理来说，有一类信号是非常重要的，这类信号就是**随机信号**（random signal），也被称为**随机过程**（random processes/stochastic processes）。在各种书籍当中，似乎随机过程（random processes）这种称呼更为常见，因此我们下面也称之为随机过程。本文学习思路如下：
- 了解随机过程及其相关的基本概念
- 提出WSS，WSS process是信号处理当中最重要的一种随机过程
- 为了方便进行随机过程的分析，我们引入了ergodicity，假设一个随机过程为ergodic会有助于我们进行随机过程分析





# Definition of A Random Process

在之前随机变量的文章中，我们引入了[随机变量](http://www.cnblogs.com/TaigaCon/p/8887931.html#RamdonVariables)，随机变量把实验（experiment）的输出（outcome）映射到数字，以便进行数学上的运算，而更重要的是，随机变量都会有它自己的PDF/PMF，而PDF/PMF展示出了该实验的可能性的输出，以及各个输出的概率。

采用与随机变量相同的思想，那么从总体上看，随机过程可以表示为：列出随机过程所有可能的输出以及每个输出对应的概率。假设随机过程$X(t)$有$N$个可能的输出，分别$x_1(t),x_2(t),\cdot\cdot\cdot,x_N(t)$，这些输出都是确定的（非随机的）信号，我们称这些输出为**realization**，而各个输出的的概率为$p_1,p_2,\cdot\cdot\cdot,p_N$，这些概率之和为1。通常有$N=\infty$。

![image](https://images2018.cnblogs.com/blog/421096/201805/421096-20180523005239822-1955175791.png)

如果专注于随机过程在某个时间点上的值，那么这个这个值就是一个随机变量：
- 一个连续时间的随机过程$X(t)$，在任意时刻$t_0$所对应的值都是一个随机变量$X(t_0)$
- 一个离散时间的随机过程$X[n]$，在任意序号$n_0$所对应的值都是一个随机变量$X[n_0]$

也就是说，随着$t$或者$n$的变化，随机过程可以得到不同的随机变量。

比如说有一个随机过程$X(t)$，时间点位于$t_1,t_2,\cdot\cdot\cdot,t_{\ell}$上的值分别为随机变量$X(t_1),X(t_2),\cdot\cdot\cdot,X(t_{\ell})$。此时可以用Joint PDF来表示这$\ell$个随机变量的概率状况。

$f_{X(t_1),X(t_2),\cdot\cdot\cdot,X(t_{\ell})}(x_1,x_2,\cdot\cdot\cdot,x_{\ell})$

那么整个随机过程就可以看作是用$t$或者$n$作为索引的无限多个随机变量的联合分布。





# Mean,Correlation,Covariance

由前文可知，随机过程在不同的时间点会有不同的随机变量，那么对于这些随机变量，我们就能套用概率模型的相关定义

$\begin{align*}
&Mean/Expectation:&\mu_X(t_i) &= E[X(t_i)]\\
&Auto-correlation:&R_{XX}(t_i,t_j)&=E[X(t_i)X(t_j)]\\
&Auto-covariance:&C_{XX}(t_i,t_j) &= E[X(t_i)-\mu_X(t_i)]E[X(t_j)-\mu_X(t_j)]\\
& & &=R_{XX}(t_i,t_j)-\mu_X(t_i)\mu_X(t_j)
\end{align*}$

上面的式子分别为：
- 随机过程$X(t)$位于时间点$t_i$上的期望
- 随机过程$X(t)$位于时间点$t_i$以及$t_j$上的随机变量之间的相关性，Auto代表这两个随机变量出自同一个随机过程
- 随机过程$X(t)$位于时间点$t_i$以及$t_j$上的随机变量之间的协方差



此外，相关性以及协方差也能应用在不同的随机过程上

$\begin{align*}
&Cross-correlation:&R_{XY}(t_i,t_j)&=E[X(t_i)Y(t_j)]\\
&Cross-covariance:&C_{XY}(t_i,t_j) &= E[X(t_i)-\mu_X(t_i)]E[Y(t_j)-\mu_Y(t_j)]\\
& & &=R_{XY}(t_i,t_j)-\mu_X(t_i)\mu_Y(t_j)
\end{align*}$

上面的式子分别为：
- 随机过程$X(t)$位于时间点$t_i$上的随机变量与随机过程$Y(t)$位于时间点$t_j$上的随机变量之间的相关性，Cross代表这两个随机变量出自不同的随机过程
- 随机过程$X(t)$位于时间点$t_i$上的随机变量与随机过程$Y(t)$位于时间点$t_j$上的随机变量之间的协方差

假设从随机过程$X(t)$中采样得到的随机变量所组成的集合为$S_X = \Big\{X(t_1),X(t_2),\cdot\cdot\cdot,X(t_k)\Big\}$，从随机过程$Y(t)$中采样所得到的随机变量所组成的集合为$S_Y=\Big\{Y(t_1'),Y(t_2'),\cdot\cdot\cdot,Y(t_{\ell})\Big\}$，如果所有的$S_X$与$S_Y$都相互独立，则称这两个随机过程相互独立。用Joint PDF表示如下：

$\begin{align*}&\qquad f_{X(t_1),\cdot\cdot\cdot,X(t_k),Y(t_1'),\cdot\cdot\cdot,Y(t_{\ell}')}(x_1,\cdot\cdot\cdot,x_k,y_1,\cdot\cdot\cdot,y_{\ell})\\ &=f_{X(t_1),\cdot\cdot\cdot,X(t_k)}(x_1,\cdot\cdot\cdot,x_k)\cdot f_{Y(t_1'),\cdot\cdot\cdot,Y(t_{\ell}')}(y_1,\cdot\cdot\cdot,y_{\ell})
\end{align*}$

如果随机过程$X(t)$与$Y(t)$相互独立的话，那么随机变量$X(t_i)$与$Y(t_j)$也会相互独立，那么就有$R_{XY}(t_i,t_j) = \mu_X(t_i)\mu_Y(t_j)$，因此$C_{XY}(t_i,t_j)=0$。





# Ensemble

Ensemble是随机过程中经常出现的一个概念，英文直译过来的意思就是全体/总体。我们这一小节主要目的是厘清ensemble相关的概念。

#### Ensemble of Signals

下面一句话是对随机过程中Ensemble of signals的定义：


**Ensemble of Signals   **The collection of signals that can be produced by the random process is referred to as the ensemble of signals in the random process.



**信号集**  随机过程所能产生的信号的集合，我们称之为随机过程的信号集。


也就是说一个随机过程的所有realization的集合就是这个随机过程的信号集。



#### Ensemble Member

Ensemble Member指的是这个集合内的成员，在随机过程中指的就是realization。



#### Ensemble Average

Ensemble Average就是把随机过程中所有的realization相加然后求平均。

$\displaystyle{\mu_X(t) = E\{X(t)\} = \sum_{n=1}^{\infty}x_n(t)}$

其中$x_n(t)$就是随机过程$X(t)$中的realization。





# Strict-Sense Stationarity

#### Definition of SSS

对于任意值$k$，以及$k$个任意时间点$t_1,\cdot\cdot\cdot,t_k$，随机过程$X(t)$在这$k$个时间点上所采样得到的随机变量分别为$X(t_1),\cdot\cdot\cdot,X(t_k)$，这$k$个随机变量所组成的Joint PDF为

$f_{X(t_1),\cdot\cdot\cdot,X(t_k)}(x_1,\cdot\cdot\cdot,x_k)$

这个Joint PDF的值取决于我们在随机过程$X(t)$上所选择的采样点$t_1,\cdot\cdot\cdot,t_k$。我们对这$k$个采样点进行大小为$\tau$的位移，有$t_1+\tau,\cdot\cdot\cdot,t_k+\tau$，那么此时在随机过程$X(t)$上所采样得到的随机变量为$X(t_1+\tau),\cdot\cdot\cdot,X(t_k+\tau)$，这些随机变量所组成的Joint PDF为

$f_{X(t_1+\tau),\cdot\cdot\cdot,X(t_k+\tau)}(x_1,\cdot\cdot\cdot,x_k)$

如果对于任意的$\tau$，都有

$f_{X(t_1),\cdot\cdot\cdot,X(t_k)}(x_1,\cdot\cdot\cdot,x_k)=f_{X(t_1+\tau),\cdot\cdot\cdot,X(t_k+\tau)}(x_1,\cdot\cdot\cdot,x_k)$

则称该随机过程是**Strict-Sense Stationarity（SSS）**，中文称为**强平稳**。





#### i.i.d. process

**independent and identically distributed**（i.i.d.）process就是一个很常见的SSS随机过程。其中
- independent代表该随机过程内所有时间点上随机变量都相互独立
- identically distributed代表该随机过程的所有时间点上的随机变量的PDF都完全一致

对于一个i.i.d. process，假设该process内的随机变量的PDF都为$f_X(x)$，那么这些随机变量的Joint PDF为

$f_{X(t_1),x(t_2),\cdot\cdot\cdot,X(t_k)}(x_1,\cdot\cdot\cdot,x_k)=f_X(x_1)f_X(x_2)\cdot\cdot\cdot f_X(x_k)$

同样也有

$f_{X(t_1+\tau),X(t_2+\tau),\cdot\cdot\cdot,X(t_k+\tau)}(x_1,\cdot\cdot\cdot,x_k)=f_X(x_1)f_X(x_2)\cdot\cdot\cdot f_X(x_k)$

因此i.i.d. process是SSS的。

在离散时间信号传输过程中出现的加性噪声（added noice）就是一个i.i.d. process。





# Wide-Sense Stationarity

#### Definition of WSS

**Wide-Sense Stationarity（WSS）**又被称为**Weak-Sense Stationarity**，中文叫做**弱平稳**。一个WSS的随机过程需要满足两个条件：
- 随机过程在任意采样点上的随机变量的expectation都与时间无关，即$\mu_X(t) = \mu_X$
- 随机过程在任意两个采样点上的随机变量的correlation/covariance都只与这两个采样点之间的时间差相关，与它们所处的位置无关，即

$\begin{align*}R_{XX}(t_1,t_2) &= R_{XX}(t_1+\alpha,t_2+\alpha) \quad for\ every\ \alpha\\&= R_{XX}(t_1-t_2, 0)\end{align*}$

$\begin{align*}C_{XX}(t_1,t_2) &= C_{XX}(t_1+\alpha,t_2+\alpha) \quad for\ every\ \alpha\\&= C_{XX}(t_1-t_2, 0)\end{align*}$



同时，从第二个条件能引申出：随机过程在任意采样点上的随机变量的variance都与时间无关：

$\begin{align*}C_{XX}(t,t) &= C_{XX}(t+\alpha,t+\alpha) \quad for\ every\ \alpha\\&= C_{XX}(0, 0)\\&=\sigma_X^2\end{align*}$





#### Jointly WSS

如果要表示两个随机过程$X(t)$与$Y(t)$之间的平稳状况，可以用**Jointly WSS**（联合弱平稳）。Jointly WSS除了要求两个随机过程都是WSS之外，还需要它们满足

$\begin{align*}R_{XY}(t_1,t_2) &= R_{XY}(t_1+\alpha,t_2+\alpha) \quad for\ every\ \alpha\\&= R_{XY}(t_1-t_2, 0)\end{align*}$



#### Simplify the Notation

对于WSS的随机过程，由于correlation以及covariance只与所选择的采样点的时间差相关，因此我们可以进行符号简化。令$\tau = t_1-t_2$，即$R_{XX}(t_1,t_2) = R_{XX}(t_2+\tau, t_2)$，我们简化成$R_{XX}(\tau)$。我们这里再用小写的$x(t)$来表示随机过程$X(t)$（When considering just first and second moments and not entire PDFs or CDFs, it will be less important to distinguish between the random process X(t) and a specific realization x(t) of it — so we shall go one step further in simplifying notation, by using lower case letters to denote the random process itself.），因此correlation可以简化为

$\color{red}{R_{xx}(\tau) = E\Big\{x(t+\tau)x(t)\Big\}}$

covariation可以简化为

$\color{red}{C_{xx}(\tau) = E\Big\{x(t+\tau)x(t)\Big\}-\mu_x^2}$





#### Properties of WSS Correlation and Covariance

利用$R_{xx}(\tau)$以及$C_{xx}(\tau)$的定义很容易证明以下性质。

$\begin{align*}
R_{xx}(\tau) = R_{xx}(-\tau), \qquad\qquad C_{xx}(\tau)=C_{xx}(-\tau)\\
R_{xy}(\tau) = R_{yx}(-\tau), \qquad\qquad C_{xy}(\tau)=C_{yx}(-\tau)
\end{align*}$

两个随机变量的相关系数为$|\rho| = \left|\frac{C_{xx}(\tau)}{\sigma_t\sigma_{t+\tau}}\right|\leq 1$，又因为在WSS随机过程中$\sigma_t=\sigma_{t+\tau}=\sigma_x$，所以把分式的分母移到不等号的另一边就能得到下面性质

$-C_{xx}(0)\leq C_{xx}(\tau)\leq C_{xx}(0)$

把第二条性质的三个项都加上$\mu_x^2$就能得到下面的这第三条性质

$-R_{xx}(0)+2\mu_x^2\leq R_{xx}(\tau) \leq R_{xx}(0)$







# Ergodicity

在讨论ergodicity的定义前，我们先了解一下引入ergodicity的原因：
- 在实际应用中，我们通常都是得到随机过程的输出，并对随机过程的输出进行分析处理。
- 随机过程通常有太多可能性的输出（realization），因此我们也不可能对所有的输出都一一进行分析。

因此，如果随机过程的一个输出就能表达出该随机过程的大部分特性的话，在对该随机过程进行分析时将会更加有效快捷。


If the random process is such that the behavior of almost every particular realization over time is representative of the behavior down the ensemble, then the process is called ergodic.



如果一个随机过程的所有realization在时域上所表现出来的特性可以代表该随机过程的总体特性的话，那么该随机过程就被称为**ergodic**。




在众多类型的随机过程当中，我们主要关注的就是WSS，这也是信号处理中最常见的随机过程。因此我们这里讨论的就是WSS的两个特性：expectation、correlation。

WSS随机过程的ensemble average是一个固定值，该随机过程的所有realization的时域expectation也同样等于该固定值，即

$\displaystyle{\color{red}{E\{x(t)\}}=\sum_{n=1}^{\infty}x_n(t) =\color{red}{ \lim_{T\to \infty}\frac{1}{2T}\int_{-T}^{T}x(t)dt} =\mu_x}$

上述式子内的$E\{x(t)\}$中的$x(t)$代表的是随机过程，式子内其余的$x(t)$是realization。

![ergodic](https://images2018.cnblogs.com/blog/421096/201805/421096-20180523005240565-1135262450.png)



而correlation原本是两个随机变量乘积的期望，即$R_{xx}(\tau) = E\{x(t)x(t+\tau)\}$。而如果所面对的是一个ergodic随机过程的话，那么该随机过程的correlation可以用其realization乘以时移realization的所得到的值的expectation进行代替，即

$\color{red}{\displaystyle{R_{xx}(\tau) = \lim_{T\to\infty}\frac{1}{2T}\int_{-T}^{T}x(t)x(t+\tau)dt}}$

上面式子中，等号右边的$x(t)$代表的是realization。

虽然要证明一个随机过程是ergodic是非常困难的，不过ergodicity在进行随机过程分析时却是相当方便的，所以我们在实际应用中常假设随机变量为ergodic再展开分析。



**Reference**：

[Alan V. Oppenheim: Signals, Systems and Inference, Chapter 9:Random Process](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-011-introduction-to-communication-control-and-signal-processing-spring-2010/readings/MIT6_011S10_chap09.pdf)

[MIT Department of Mechanical Engineering: 2.161 Signal Processing - Continuous and Discrete Fall Term 2008 Lecture 21](https://ocw.mit.edu/courses/mechanical-engineering/2-161-signal-processing-continuous-and-discrete-fall-2008/lecture-notes/lecture_21.pdf)













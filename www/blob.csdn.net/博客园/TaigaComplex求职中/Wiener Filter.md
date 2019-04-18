# Wiener Filter - TaigaComplex求职中 - 博客园







# [Wiener Filter](https://www.cnblogs.com/TaigaCon/p/9290484.html)





假设分别有两个WSS process：$x[n]$，$y[n]$，这两个process之间存在某种关系，并且我们也了解这种关系。现在我们手头上有process $x[n]$，目的是要设计一个LTI系统，使得系统输出$y[n]$，不过$y[n]$是一个WSS process，我们不可能准确得到随机过程上的值，因此实际输出并不是$y[n]$，而是$\hat{y}[n]$，此时我们就能通过引入MSE来判断实际输出$\hat{y}[n]$与$y[n]$之间的差距。当我们所设计的系统使得$\hat{y}[n]$与$y[n]$之间有Minimun MSE时，所得到的输出就是最优的，即

$e[n] \triangleq \hat{y}[n] – y[n]$

当满足Minimum MSE时，有

$\epsilon = MMSE = E\{e^2 [n]\}$

那么这个LTI系统就被称为**Wiener filter**。

![image](https://images2018.cnblogs.com/blog/421096/201807/421096-20180710174134946-1803245762.png)



本文的主要目的就是求Wiener filter的脉冲响应，所求得的脉冲响应需要使得系统有Minimun MSE。



※在开始之前，先说明本文一些计算所需的前提条件：
- 由于输入以及输出的随机过程都是实数，因此$R_{xx}[m],R_{yy}[m],R_{xy}[m],R_{yx}[m]$也会是实数
- 由于$R_{xx}[m] = R_{xx}[-m]$，因此$S_{xx}(e^{j\Omega})$是实数，同理有$S_{yy}(e^{j\Omega})$是实数 
- 本文假设$S_{xx}(e^{j\Omega})$以及$S_{yy}(e^{j\Omega})$在所有频率上都大于0
- 为了方便，本文假设随机过程$x[n],y[n]$的mean都为0
- 连续时间随机过程有上述同样条件



# NONCAUSAL DT WIENER FILTER

DT表示的是离散时间系统，noncausl即非因果，[因果性](https://www.cnblogs.com/TaigaCon/p/8058634.html#causal)的系统的脉冲响应的$n<0$部分为$0$，非因果系统则无此要求。

对于DT系统，有两种求解Wiener filter脉冲响应的方式。这是因为DT系统的脉冲响应可以分为FIR以及IIR两种，分别对应不同的求解方法。



#### FIR

FIR即有限脉冲响应，脉冲响应$h[n]$的长度是有限的，这表明我们可以准确得到脉冲响应序列中的各个元素的值。

DT LTI系统以卷积来表征，即

$\displaystyle{\hat{y}[n] = \sum_{k=-\infty}^{\infty}h[k]x[n-k] }$

那么MSE就可以表示为

$\displaystyle{\epsilon = E\left\{ \left(\sum_{k=-\infty}^{\infty}h[k]x[n-k]-y[n]  \right)^2 \right\}}$

由于是一个FIR filter，因此假设$h[n]$是一个长度为N的脉冲响应，也就是说上面的式子有N个未知数，分别对应$h[0],h[1],\cdot\cdot\cdot,h[N-1]$。为了得到该N元二次函数的最小值，需要分别对这N个未知数求偏导，当偏导数为0时可以得到最小的MSE。以序列其中的一个未知数$h[m]$为例

$\begin{align*}
\frac{\partial \epsilon}{\partial h[m]} &= \frac{\displaystyle{\partial E\left\{\left(\sum_{k=-\infty}^{\infty}h[k]x[n-k]-y[n]\right)^2\right\} }}{\partial h[m]}\\
&= \frac{\displaystyle{\partial E\left\{\left(\sum_{k=0}^{N-1}h[k]x[n-k]-y[n]\right)^2\right\} }}{\partial h[m]} \qquad h[k]\ is\ FIR\\
&= E\left\{ 2\underbrace{\left(\sum_{k=0}^{N-1}h[k]x[n-k]-y[n] \right )}_{e[n]}x[n-m] \right\}\qquad chain\ rule\\
&= 2E\Big\{e[n]x[n-m]\Big\}\\
&= 2E\Big\{\big(\hat{y}[n]-y[n]\big)x[n-m]\Big\}\\
&= 2E\Big\{\hat{y}[n]x[n-m]-y[n]x[n-m]\Big\}\\
&= 2\Big(R_{\hat{y}x}[m]-R_{yx}[m]\Big)
\end{align*}$

当偏导数为0时有最小MSE，此时

$\color{red}{R_{\hat{y}x}[m] = R_{yx}[m]}$

根据[correlation](https://www.cnblogs.com/TaigaCon/p/9127534.html#DT)相关定理，有

$\color{red}{R_{yx}[m] = h[m]*R_{xx}[m]}$

因此，只要我们能根据随机过程$x[n],y[n]$之间的关系求得$R_{yx}[m],R_{xx}[m]$，就可以得到最佳的脉冲响应$h[m]$。卷积的计算方式如下

$\begin{align*}
R_{yx}[m] &= h[m]*R_{xx}[m] = R_{xx}[m]*h[m]\\
&=\begin{bmatrix}
R_{xx}[0] &R_{xx}[-1]  &\cdots  &R_{xx}[1-N] \\ 
R_{xx}[1] &R_{xx}[0]  &\cdots  &R_{xx}[2-N] \\ 
\vdots  &\vdots  &\ddots   &\vdots \\
R_{xx}[N-1] &R_{xx}[N-2]  &\cdots  &R_{xx}[0] 
\end{bmatrix}
\begin{bmatrix}
h[0]\\ 
h[1]\\ 
\vdots\\ 
h[N-1]
\end{bmatrix}
=\begin{bmatrix}
R_{yx}[0]\\ 
R_{yx}[1]\\ 
\vdots\\ 
R_{yx}[N-1]
\end{bmatrix} \end{align*}$



#### IIR

IIR即无限脉冲响应，脉冲响应$h[n]$的长度是无限的，我们无法得到脉冲响应序列中的所有元素的值，因此主要求解方式需要先从频域获得系统的频率响应$H(e^{j\Omega})$，然后根据需要转换为脉冲响应$h[n]$。

如前一小节所述，当MSE式子的偏导数为0时可以得到MMSE，此时系统能输出最佳的预测值。按照此思路，同理可得到

$\begin{align*}
time-domain: &\qquad R_{\hat{y}x}[n] = R_{yx}[n]\\
z-transform: &\qquad S_{\hat{y}x}(z) = S_{yx}(z)\\
fourier-transform: &\qquad S_{\hat{y}x}(e^{j\Omega}) = S_{yx}(e^{j\Omega})
\end{align*}$

根据[correlation](https://www.cnblogs.com/TaigaCon/p/9127534.html#DT)相关定理，又有

$\begin{align*}
time-domain: &\qquad h[n]*R_{xx}[n] = R_{\hat{y}x}[n] = R_{yx}[n]\\
z-transform: &\qquad H(z)S_{xx}(z)=S_{\hat{y}x}(z) = S_{yx}(z)\\
fourier-transform: &\qquad H(e^{j\Omega})S_{xx}(e^{j\Omega})=S_{\hat{y}x}(e^{j\Omega}) = S_{yx}(e^{j\Omega})
\end{align*}$

那么最佳的系统函数以及最佳的频率响应分别为

$\begin{align*}
z-transform: &\qquad H(z)= S_{yx}(z)/S_{xx}(z)\\
fourier-transform: &\qquad H(e^{j\Omega})=S_{yx}(e^{j\Omega})/S_{xx}(e^{j\Omega})
\end{align*}$

因此，只要我们能根据随机过程$x[n],y[n]$之间的关系求得它们在频域上的表示$S_{yx}(e^{j\Omega}),S_{xx}(e^{j\Omega})$，就可以得到最佳的频率响应$H(e^{j\Omega})$。



#### MMSE

当系统为最佳滤波器的时候，MSE为最小值，既有

$MMSE = R_{ee}[0]$

其中$R_{ee}[m]$可以通过以下方法得到

$\begin{align*}
R_{ee}[m] &= E\{e[n+m]e[n]\}\\
&= E\Big\{\big(y[n+m]-\hat{y}[n+m]\big)\big(y[n]-\hat{y}[n]\big)\Big\}\\
&= E\Big\{y[n+m]y[n]-\hat{y}[n+m]y[n]-y[n+m]\hat{y}[n]+\hat{y}[n+m]\hat{y}[n]\Big\}\\
&= R_{yy}[m]-R_{\hat{y}y}[m]-R_{y\hat{y}}[m]+R_{\hat{y}\hat{y}}[m]\\
&= R_{yy}[m]-R_{\hat{y}y}[m]-h[-m]*R_{yx}[m]+h[-m]*R_{\hat{y}x}[m]\\
&= R_{yy}[m]-R_{\hat{y}y}[m]-h[-m]*R_{yx}[m]+h[-m]*R_{yx}[m]\\
&= R_{yy}[m]-R_{\hat{y}y}[m]\\ &= R_{yy}[m]-h[m]*R_{xy}[m] \qquad \href{https://www.cnblogs.com/TaigaCon/p/9127534.html#DT}{correlation\ equation} \end{align*}$

MMSE在频域上则能有如下表达

$\begin{align*}
MMSE = R_{ee}[0]&=\frac{1}{2\pi}\int_{-\pi}^{\pi}S_{ee}(e^{j\Omega})e^{j\Omega 0}d\Omega\\
&=\frac{1}{2\pi}\int_{-\pi}^{\pi}S_{ee}(e^{j\Omega})d\Omega\\
&=\frac{1}{2\pi}\int_{-\pi}^{\pi}\Big(S_{yy}-HS_{xy}\Big)d\Omega \qquad drop\ (e^{j\Omega})\\
&=\frac{1}{2\pi}\int_{-\pi}^{\pi}\Big(S_{yy}-\frac{S_{yx}}{S_{xx}}S_{xy}\Big)d\Omega\\
&=\frac{1}{2\pi}\int_{-\pi}^{\pi}\left(S_{yy}\left\{1-\frac{S_{yx}}{S_{xx}}\frac{S_{xy}}{S_{yy}}\right\}\right)d\Omega\\
&=\frac{1}{2\pi}\int_{-\pi}^{\pi}\Big(S_{yy}(1-\rho_{yx}\rho_{yx}^*)\Big)d\Omega
\end{align*}$

其中$\rho_{yx}(e^{j\Omega})$可以看作是频域的correlation coefficient（**[相关系数](https://www.cnblogs.com/TaigaCon/p/8887931.html#rho)**）。定义如下

$\displaystyle{\rho_{yx}(e^{j\Omega}) = \frac{S_{yx}(e^{j\Omega})}{\sqrt{S_{xx}(e^{j\Omega})S_{yy}(e^{j\Omega})}}}$

由于我们前面以及讲过$R_{yx}[m]=R_{xy}[m]$是实数，因此有$\href{https://www.cnblogs.com/TaigaCon/p/9125855.html}{S_{xy}(e^{j\Omega}) = S_{yx}^*(e^{j\Omega})}$，又有$S_{xx}(e^{j\Omega}),S_{yy}(e^{j\Omega})$在任意频域上都大于0，有了这两个条件容易得到

$\displaystyle{\rho_{yx}^*(e^{j\Omega}) = \frac{S_{yx}^*(e^{j\Omega})}{\sqrt{S_{xx}(e^{j\Omega})S_{yy}(e^{j\Omega})}}}$





# NONCAUSAL CT WIENER FILTER

不同于DT中离散的随机过程，CT中的随机过程是连续的，因此不能像DT一样对某一个采样点求导来得到最佳系统，需要采用其他计算方式。

![image](https://images2018.cnblogs.com/blog/421096/201807/421096-20180710174135452-948483978.png)

与DT时一样基于MSE的预测，要得到最佳系统，那么该系统应该使得输出的随机过程$\hat{y}(t)$与目标随机过程$y(t)$的MSE最小。

$MSE = E\Big\{ e^2(t) \Big\} = E\Big\{ \big(\hat{y}(t)-y(t)\big)^2 \Big\}$

我们注意到

$MSE = E\Big\{e^2{t}\Big\} = R_{ee}(0)$

因此可以通过$R_{ee}(\tau)$来推导如何得到最佳系统。

$\begin{align*}
R_{ee}(\tau) &= E\Big\{\big(\hat{y}(t)-y(t)\big)\big(\hat{y}(t+\tau)-y(t+\tau)\big)\Big\}\\
&= E\Big\{y(t)y(t+\tau)+\hat{y}(t)\hat{y}(t+\tau)-y(t)\hat{y}(t+\tau)-\hat{y}(t)y(t+\tau)\Big\}\\
&= R_{yy}(\tau)+R_{\hat{y}\hat{y}}(\tau)-R_{y\hat{y}}(\tau)-R_{\hat{y}y}(\tau)
\end{align*}$

根据这条式子可以得到

$S_{ee}(j\omega) = S_{yy}(j\omega)+S_{\hat{y}\hat{y}}(j\omega)-S_{y\hat{y}}(j\omega)-S_{\hat{y}y}(j\omega)$



下面来推导如何得到Minimun MSE

$\begin{align*}
MSE = R_{ee}(0) &= \frac{1}{2\pi}\int_{-\infty}^{\infty}S_{ee}(j\omega)e^{j\omega 0}d\omega\\
&=\frac{1}{2\pi}\int_{-\infty}^{\infty}S_{ee}(j\omega)d\omega\\
&=\frac{1}{2\pi}\int_{-\infty}^{\infty}\Big(S_{yy}+S_{\hat{y}\hat{y}}-S_{y\hat{y}}-S_{\hat{y}y}\Big)d\omega \qquad drop\ (j\omega )\\
&=\frac{1}{2\pi}\int_{-\infty}^{\infty}\Big(S_{yy}+HH^*S_{xx}-H^*S_{yx}-HS_{xy} \Big)d\omega\\
&=\frac{1}{2\pi}\int_{-\infty}^{\infty}\Big(S_{yy}+HH^*S_{xx}-H^*S_{yx}-HS_{yx}^* \Big)d\omega\qquad R_{xy}(t)=R_{yx}(-t)\ is\ real\\
&=\frac{1}{2\pi}\int_{-\infty}^{\infty}\left( S_{yy}+\left( H\sqrt{S_{xx}}-\frac{S_{yx}}{\sqrt{S_{xx}}} \right )\left( H^*\sqrt{S_{xx}}-\frac{S_{yx}^*}{\sqrt{S_{xx}}} \right )-\frac{S_{yx}S_{yx}^*}{S_{xx}} \right)d\omega\\
&=\frac{1}{2\pi}\int_{-\infty}^{\infty}\left| H\sqrt{S_{xx}}-\frac{S_{yx}}{\sqrt{S_{xx}}} \right|^2d\omega+\frac{1}{2\pi}\int_{-\infty}^{\infty}\left(S_{yy}-\frac{S_{yx}S_{yx}^*}{S_{xx}}\right)d\omega
\end{align*}$

上面的式子采用了提取式子中的平方的方法来使式子得到最小值，当系统的频率响应满足以下条件时，即可得到Minimun MSE

$\color{red}{\displaystyle{ H(j\omega) = \frac{S_{yx}(j\omega)}{S_{xx}(j\omega)} }}$

这也表明只要我们能根据随机过程$x[n],y[n]$之间的关系求得它们在频域上的表示$S_{yx}(j\omega),S_{xx}(j\omega)$，就可以得到最佳的频率响应$H(j\omega)$。



此时式子中第一项为0，那么MMSE为

$\begin{align*}
MMSE = R_{ee}(0) 
&=\frac{1}{2\pi}\int_{-\infty}^{\infty}\left( S_{yy}-\frac{S_{yx}S_{yx}^*}{S_{xx}} \right)d\omega\\
&=\frac{1}{2\pi}\int_{-\infty}^{\infty}S_{yy}\left(1-\frac{S_{yx}S_{yx}^*}{S_{xx}S_{yy}} \right )d\omega\\
&=\frac{1}{2\pi}\int_{-\infty}^{\infty}S_{yy}(1-\rho\rho^*)d\omega
\end{align*}$

其中频域的correlation coefficient $\rho(j\omega)$为

$\displaystyle{\rho(j\omega) = \frac{S_{yx}(j\omega)}{\sqrt{S_{xx}(j\omega)S_{yy}(j\omega)}}}$





# CAUSAL CT WIENER FILTER

观察上一小节推导得到的式子

$\displaystyle{MSE =\frac{1}{2\pi}\int_{-\infty}^{\infty}\left| H\sqrt{S_{xx}}-\frac{S_{yx}}{\sqrt{S_{xx}}} \right|^2d\omega+\frac{1}{2\pi}\int_{-\infty}^{\infty}\left(S_{yy}-\frac{S_{yx}S_{yx}^*}{S_{xx}}\right)d\omega}$

当所设计的系统的频率响应使得第一项为0的时候就能得到MMSE，不过如果我们对该系统有因果性的要求，也就意味着对频率响应$H(j\omega)$有所限制，此时的$H(j\omega)$可能就无法使得上述等式的第一项为0，因此对于因果系统，我们需要寻求其它的求解方法。

在[WSS Process On Causal LTI System](https://www.cnblogs.com/TaigaCon/p/9290423.html)中，我们得到了一个结论：一个Casual LTI系统可以把white noise建模成WSS process。我们这里假设一个频率响应为$M_{xx}(j\omega)$的因果系统把white noise建模成了WSS process $x(t)$，那么就可以把$S_{xx}(j\omega)$分解为

$S_{xx}(j\omega) = M_{xx}(j\omega)M^*_{xx}(j\omega)$

那么前面的式子就可以变形为

$\displaystyle{MSE =\frac{1}{2\pi}\int_{-\infty}^{\infty}\left| HM_{xx}-\frac{S_{yx}}{M_{xx}^*} \right|^2d\omega+\frac{1}{2\pi}\int_{-\infty}^{\infty}\left(S_{yy}-\frac{S_{yx}S_{yx}^*}{S_{xx}}\right)d\omega}$

其中$H(j\omega)$为因果系统，$M_{xx}(j\omega)$也是因果系统，$HM_{xx}$是这两个因果系统的级联，因此也是一个因果的。此时，我们可以把$\displaystyle{\frac{S_{yx}(j\omega)}{M_{xx}^*(j\omega)}}$分成两部分之和

$\displaystyle{\frac{S_{yx}(j\omega)}{M_{xx}^*(j\omega)}=\left[\frac{S_{yx}(j\omega)}{M_{xx}^*(j\omega)}\right]_+ + \left[\frac{S_{yx}(j\omega)}{M_{xx}^*(j\omega)}\right]_-}$

其中$\displaystyle{\left[\frac{S_{yx}(j\omega)}{M_{xx}^*(j\omega)}\right]_+}$代表的是因果部分，$\displaystyle{\left[\frac{S_{yx}(j\omega)}{M_{xx}^*(j\omega)}\right]_-}$代表的是非因果部分。当$H(j\omega)$使得积分中的因果部分为0时，就能得到MMSE。此时有

$\color{red}{\displaystyle{H(j\omega) = \frac{1}{M_{xx}(j\omega)}\left[\frac{S_{yx}(j\omega)}{M_{xx}^*(j\omega)}\right]_+ }}$



MMSE为

$\displaystyle{MMSE=\frac{1}{2\pi}\int_{-\infty}^{\infty}\left|\left[\frac{S_{yx}}{M_{xx}^*} \right ]_-\right|^2d\omega+\frac{1}{2\pi}\int_{-\infty}^{\infty}\left( S_{yy}-\frac{S_{yx}S_{yx}^*}{S_{xx}} \right)d\omega}$





# CAUSAL DT WIENER FILTER

Causal DT的求解方式与Causal CT的求解方式基本一样

$\color{red}{\displaystyle{H(e^{j\Omega}) = \frac{1}{M_{xx}(e^{j\Omega})}\left[\frac{S_{yx}(e^{j\Omega})}{M_{xx}^*(e^{j\Omega})}\right]_+ }}$





# Dealing with Nonzero Means

前文所讨论都是基于LTI系统，而LTI系统的数学定义如下（CT）：

$\displaystyle{y(t) = \int_{-\infty}^{\infty}h(t-k)x(k)dk}$

不过，有时随机过程$y(t)$除了跟$x(t)$有关，还有可能有常数偏差，比如我们之前在讨论LMMSE预测的时候会假设$y = ax+b$。对比上面LTI系统的定义，可以发现这种假设并不符合LTI系统，这也是我们在前面的讨论中把随机过程$x(t),y(t)$的mean都设为0的原因。

对于mean不为0的情况，我们可以假设LTI系统的输入是$x(t)-\mu_x$，输出的是$\hat{y}(t)-\mu_y$。

![image](https://images2018.cnblogs.com/blog/421096/201807/421096-20180710174135974-1569285805.png)

为了输出最佳的预测process，此时的LTI系统有如下频率响应

$\displaystyle{H(j\omega) = \frac{D_{yx}(j\omega)}{D_{xx}(j\omega)}}$

其中$D_{yx}(j\omega),D_{xx}(j\omega)$分别为covariance $C_{yx}(\tau),C_{xx}(\tau)$的傅里叶变换。



**Reference:**

[Alan V. Oppenheim: Signals, Systems and Inference, Chapter 11:Wiener Filtering](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-011-introduction-to-communication-control-and-signal-processing-spring-2010/readings/MIT6_011S10_chap11.pdf)













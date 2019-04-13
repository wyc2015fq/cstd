
# 漫步数理统计二十七——t与F分布 - 蜗牛 - CSDN博客


2017年05月05日 22:51:32[会敲键盘的猩猩](https://me.csdn.net/u010182633)阅读数：2553


本篇博文定义两个非常重要的分布，它们在一些统计推断问题中非常有用，也就是$t$分布与$F$分布。
令$W$表示满足$N(0,1)$分布的随机变量；$V$表示满足$\chi^2(r)$分布的随机变量；且$W,V$独立，那么$W,V$的联合pdf，表示为$h(w,v)$，就是$W$的pdf与$V$的pdf乘积，或者

$$
h(w,v)=
\begin{cases}
\frac{1}{\sqrt{2\pi}}e^{-w^2/2}\frac{1}{\Gamma(r/2)2^{r/2}}v^{r/2-1}e^{-v/2}&\infty<w<\infty,0<v<\infty\\
0&elsewhere
\end{cases}
$$
定义新的随机变量$T$为

$$
T=\frac{W}{\sqrt{V/r}}
$$
利用变量替换方法可以得到$T$的pdf$g_1(t)$。方程

$$
t=\frac{w}{\sqrt{v/r}}\quad u=v
$$
定义了一个变换，将$\mathcal{S}=\{(w,v):-\infty<w<\infty,0<v<\infty\}$一一映射到$\mathcal{T}=\{(t,u):-\infty<t<\infty,0<u<\infty\}$，因为$w=t\sqrt{u}/\sqrt{r},v=u$，所以变换的雅可比绝对值为$|\mathcal{J}|=\sqrt{u}/\sqrt{r}$，所以$T,U=V$的联合pdf为

$$
\begin{align*}
g(t,u)
&=h\left(\frac{t\sqrt{u}}{\sqrt{r}},u\right)|\mathcal{J}|\\
&=\begin{cases}
\frac{1}{\sqrt{2\pi}\Gamma(r/2)2^{r/2}}u^{r/2-1}\exp[-\frac{u}{2}(1+\frac{t^2}{r})]\frac{\sqrt{u}}{\sqrt{r}}&|t|<\infty,0<u<\infty\\
0&elsewhere
\end{cases}
\end{align*}
$$
$T$的边缘pdf为

$$
\begin{align*}
g_1(t)
&=\int_{-\infty}^{\infty}g(t,u)du\\
&=\int_0^{\infty}\frac{1}{\sqrt{2\pi r}\Gamma(r/2)2^{r/2}}u^{(r+1)/2-1}\exp[-\frac{u}{2}(1+\frac{t^2}{r})]du
\end{align*}
$$
令$z=u[1+(t^2/r)]/2$得到

$$
\begin{align}
g_1(t)
&=\int_0^{\infty}\frac{1}{\sqrt{2\pi r}\Gamma(r/2)2^{r/2}}\left(\frac{2z}{1+t^2/r}\right)^{(r+1)/2-1}e^{-z}\left(\frac{2}{1+t^2/r}\right)dz\nonumber\\
&=\frac{\Gamma[(r+1)/2]}{\sqrt{\pi r}\Gamma(r/2)}\frac{1}{(1+t^2/r)^{(r+1)/2}},-\infty<t<\infty\tag1
\end{align}
$$
所以如果$W$满足$N(0,1)$，$V$满足$\chi^2(r)$且$W,V$独立，那么

$$
\begin{equation}
T=\frac{W}{\sqrt{V/r}}\tag2
\end{equation}
$$
就有如上所述的pdf$g_1(t)$。随机变量$T$的分布通常称为$t$分布，通过观察可以发现$t$分布完全由参数$r$决定，也就是卡方分布的自由度。
$\textbf{例1：}$$T$满足自由度为$r$的$t$分布，那么根据$(2)$，我们可以写成$T=W(/r)^{-1/2}$，其中$W$满足$N(0,1)$分布，$V$满足$\chi^2(r)$分布，$W,V$是独立的随机变量。假设$(r/2)-(k/2)>0$，那么

$$
\begin{align}
E(T^k)
&=E\left[W^k\left(\frac{V}{r}\right)^{-k/2}\right]=E(W^k)E\left[\left(\frac{V}{r}\right)^{-k/2}\right]\\
&=E(W^k)\frac{2^{-k/2}\Gamma(\frac{r}{2}-\frac{k}{2})}{\Gamma(\frac{r}{2})r^{-k/2}},\ k<r\tag3
\end{align}
$$
为了求$T$的均值，令$k=1$。因为$E(W)=0$，所以只要$T$的自由度超过1，$T$的均值就为0。为了求方差，令$k=2$，这时候需要$r>2$，因为$E(W^2)=1$，所以$T$的方差为

$$
\begin{equation}
var(T)=E(T^2)=\frac{r}{r-2}\tag4
\end{equation}
$$
因此自由度$r>2$的$t$分布均值为0，方差为$r/(r-2)$。
接下来考虑两个独立且自由度分别为$r_1,r_2$的卡方随机变量$U,V$，$U,V$的联合pdf$h(u,v)$为

$$
h(u,v)=
\begin{cases}
\frac{1}{\Gamma(r_1/2)\Gamma(r_2/2)2^{r_1+r_2}/2}u^{r_1/2-1}v^{r_2/2-1}e^{-(u+v)/2}&0<u,v<\infty\\
0&elsewhere
\end{cases}
$$
我们定义新的随机变量为

$$
W=\frac{U/r_1}{V/r_2}
$$
接下里求$W$的pdf$g_1(w)$，方程

$$
w=\frac{u/r_1}{v/r_2},z=v,
$$
定义了一对一变换，将集合$\mathcal{S}=\{(u,v):0<u<\infty,0<v<\infty\}$映射到集合$\mathcal{T}=\{(w,z):0<w<\infty,0<z<\infty\}$，因为$u=(r_1/r_2)zw,v=z$，变换的雅可比绝对值为$|\mathcal{J}|=(r_1/r_2)z$，随机变量$W,Z=V$的联合pdf$g(w,z)$为

$$
g(w,z)=\frac{1}{\Gamma(r_1/2)\Gamma(r_2/2)2^{(r_1+r_2)/2}}\left(\frac{r_1zw}{r_2}\right)^{\frac{r_1-2}{2}}z^{\frac{r_2-2}{2}}\exp\left[-\frac{z}{2}\left(\frac{r_1w}{r_2}+1\right)\right]\frac{r_1z}{r_2}
$$
假设$(w,z)\in\mathcal{T}$，其他地方为零。$W$的边缘pdf$g_1(w)$为

$$
\begin{align*}
g_1(w)
&=\int_{-\infty}^{\infty}g(w,z)dz\\
&=\int_{0}^{\infty}\frac{(r_1/r_2)^{r_1/2}(w)^{r_1/2-1}}{\Gamma(r_1/2)\Gamma(r_2/2)2^{(r_1+r_2)/2}}z^{(r_1+r_2)/2-1}\exp\left[-\frac{z}{2}\left(\frac{r_1w}{r_2}+1\right)\right]dz
\end{align*}
$$
变量代换

$$
y=\frac{z}{2}\left(\frac{r_1w}{r_2}+1\right)
$$
可得

$$
\begin{align*}
g_1(w)
&=\int_{0}^{\infty}\frac{(r_1/r_2)^{r_1/2}(w)^{r_1/2-1}}{\Gamma(r_1/2)\Gamma(r_2/2)2^{(r_1+r_2)/2}}\left(\frac{2y}{r_1w/r_2+1}\right)^{(r_1+r_2)/2-1}e^{-y}\\
&\quad\times\left(\frac{2}{r_1w/r_2+1}\right)dy\\
&=\begin{cases}
\frac{\Gamma[(r_1+r_2)/2](r_1/r_2)^{r_1/2}}{\Gamma(r_1/2)\Gamma(r_2/2)}\frac{(w)^{r_1/2-1}}{(1+r_1w/r_2)^{(r_1+r_2)/2}}&0<w<\infty\\
0&elsewhere
\end{cases}
\end{align*}
$$
故，如果$U,V$是自由度分别为$r_1,r_2$的且独立的卡方变量，那么

$$
W=\frac{U/r_1}{V/r_2}
$$
的pdf如上所示，该随机变量的分布通常称为$F$分布，可以看出$F$分布完全由参数$r_1,r_2$决定。
$\textbf{例2：}$$F$为自由服$r_1,r_2$的$F$分布，那么$F=(r_2/r_1)(U/V)$，其中$U,V$是独立的$\chi^2$随机变量，自由度分别为$r_1,r_2$。因此$F$的$k$阶矩为

$$
E(F^k)=\left(\frac{r_2}{r_1}\right)^kE(U^k)E(V^{-k})
$$
当然假设右边的期望均存在。根据前面的定理可知$k>-(r_1/2)$恒为真，所以第一个期望恒存在，如果$r_2>2k$那么第二个期望存在。假设为真，那么$F$的均值为

$$
E(F)=\frac{r_2}{r_1}r_1\frac{2^{-1}\Gamma(\frac{r_2}{2}-1)}{\Gamma(\frac{r_2}{2})}=\frac{r_2}{r_2-2}
$$
如果$r_2$非常大，那么$E(F)$约为1。
最后介绍一个定理，它是由上面的$t$分布推导出来的。
$\textbf{定理1：}$令$X_1,\ldots,X_n$是独立同分布的随机变量，且每个都是均值为$\mu$，方差为$\sigma^2$的正态分布，定义新的随机变量为

$$
\bar{X}=\frac{1}{n}\sum_{i=1}^nX_i,\quad S^2=\frac{1}{n-1}\sum_{i=1}^n(X_i-\bar{X})^2
$$
那么
\bar{X}是N(\mu,\frac{\sigma^2}{n})分布；
\bar{X},S^2是独立的；
(n-1)S^2/\sigma^2满足\chi^2(n-1)分布；
随机变量
T=\frac{\bar{X}-\mu}{S/\sqrt{n}}
满足自由度为n-1的t分布。
\textbf{证明：}令\mathbf{X}=(X_1,\ldots,X_n)^\prime，因为X_1,\ldots,X_n是独立同分布的N(\mu,\sigma^2)随机变量，所以X是多元正态分布N(\mu\mathbf{1},\sigma^2\mathbf{I})，其中\mathbf{1}表示元素均为1的向量。令\mathbf{v}^\prime=(1/n,\ldots,1/n)^\prime=(1/n)\mathbf{1}^\prime。注意\bar{X}=\mathbf{v}^\prime\mathbf{X}，定义随机向量\mathbf{Y}为\mathbf{Y}=(X_1-\bar{X},\ldots,X_n-\bar{X})^\prime，考虑下面的变换：
\mathbf{W}=
\begin{bmatrix}
\bar{X}\\
\mathbf{Y}
\end{bmatrix}=
\begin{bmatrix}
\mathbf{v}^\prime\\
\mathbf{I-1v^\prime}
\end{bmatrix}
\mathbf{X}
因为\mathbf{W}是多元正态随机向量的线性变换，它的均值与方差为
E[\mathbf{W}]=
\begin{bmatrix}
\mathbf{v^\prime}\\
\mathbf{I-1v^\prime}
\end{bmatrix}
\mu\mathbf{1}=
\begin{bmatrix}
\mu\\
\mathbf{0}_n
\end{bmatrix}
其中\mathbf{0}_n表示元素全为0的向量，协方差矩阵为
\begin{align*}
\boldsymbol{\Sigma}
&=\begin{bmatrix}
\mathbf{v^\prime}\\
\mathbf{I-1v^\prime}
\end{bmatrix}
\sigma_2\mathbf{I}
\begin{bmatrix}
\mathbf{v^\prime}\\
\mathbf{I-1v^\prime}
\end{bmatrix}^\prime\\
&=\sigma^2
\begin{bmatrix}
\frac{1}{n}&\mathbf{0}_n^\prime\\
\mathbf{0}_n&\mathbf{I-1v^\prime}
\end{bmatrix}
\end{align*}
因为\bar{X}是\mathbf{W}的第一个元素，根据前面的定理可得结论1。接下来因为协方差为0，所以\bar{X}与\mathbf{Y}独立，但是\mathbf{S}^2=(n-1)^{-1}\mathbf{Y^\prime Y}，因此\bar{Y}也与\mathbf{S}^2独立，结论2的证。
考虑随机变量
V=\sum_{i=1}^n\left(\frac{X_i-\mu}{\sigma}\right)^2
这个和的每项都是N(0,1)随机变换的平方，因此是\chi^2(1)分布。因为它们互相独立，所以V是\chi^2(n)随机变量。注意，
\begin{align*}
V
&=\sum_{i=1}^n\left(\frac{(X_i-\bar{X})+(\bar{X}-\mu)}{\sigma}\right)^2\\
&=\sum_{i=1}^n\left(\frac{X_i-\bar{X}}{\sigma}\right)^2+\left(\frac{\bar{X}-mu}{\sigma/\sqrt{n}}\right)^2\\
&=\frac{(n-1)S^2}{\sigma^2}+\left(\frac{\bar{X}-\mu}{\sigma/\sqrt{n}}\right)^2
\end{align*}
右边两项是独立的，且第二项为标准正态分布的平方即\chi^2(1)分布。取两边的mgf可得
(1-2t)^{-n/2}=E[\exp\{t(n-1)S^2/\sigma^2\}](1-2t)^{-1/2}
求出的(n-1)S^2\sigma^2就得到结论3。最后，利用前面三个结论即可得到结论4，
T=\frac{(\bar{X}-\mu)/(\sigma/\sqrt{n})}{\sqrt{(n-1)S^2/(\sigma^2(n-1))}}


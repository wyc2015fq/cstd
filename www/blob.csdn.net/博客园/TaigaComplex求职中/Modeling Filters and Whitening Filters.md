# Modeling Filters and Whitening Filters - TaigaComplex求职中 - 博客园







# [Modeling Filters and Whitening Filters](https://www.cnblogs.com/TaigaCon/p/9236625.html)





# Colored and White Process

#### White Process

**White Process**，又称为**White Noise（白噪声）**，其中white来源于白光，寓意着PSD的平坦分布，white noise指的就是在[PSD](https://www.cnblogs.com/TaigaCon/p/9175158.html)上具有平坦分布（常数）的随机过程。PSD是auto-correlation的傅里叶变换，PSD上为常数意味着auto-correlation是一个位于零点上的脉冲函数。

![image](https://images2018.cnblogs.com/blog/421096/201806/421096-20180627235516633-1596001380.png)回顾auto-correlation的定义：

$R_{xx}(\tau) = E\Big\{x(t+\tau)x(t)\Big\}$

Auto-correlation在零点上为脉冲，除此之外都是0，这表明了各个采样点之间是相互独立的，**正是由于各采样点之间相互独立，才体现出了white noise的white这一特点**。

理想的White noise要求PSD是一个常数，不过由于传输媒介以及有限的监测能力的关系，通常我们所得到的PSD都受限于某个带宽范围之内，因此如果一个process的PSD在监测范围（或者我们所感兴趣的频率范围）表现得平坦的话，就可以称该process为white noise。比如，如果一个音频的PSD在20~20000Hz内表现平坦的话，我们就可以称该音频为white noise。



[i.i.d. process](https://www.cnblogs.com/TaigaCon/p/9074973.html#iid)就是一个最简单white noise的例子。i.i.d. process中样本的PDF常见的有Uniform、Poisson、Gaussian等分布方式，如果PDF为Gaussian，则该process称为Gaussian white noise。



#### Colored Process

PSD表现得平坦的就是white noise，否则就是colored process。





# Modeling Filter

输入为white process，输出为colored process的LTI系统被称为Modeling filter或者shaping filter。如果我们需要某个PSD为$S_{xx}(e^{j\Omega})$的process，可以通过设计一个LTI系统，并且对该系统输入mean为0的[Bernoulli process](https://www.cnblogs.com/TaigaCon/p/9188661.html#Bernoulli)（PSD为常数1）来得到，所设计的系统需要使得输出的process的PSD为$S_{xx}(e^{j\Omega})$。根据[WSS Process在LTI系统中的性质](https://www.cnblogs.com/TaigaCon/p/9127534.html#DT)，有

$\displaystyle{ H(e^{j\Omega}) = \sqrt{S_{xx}(e^{j\Omega})} }$

我们只需要对$H(e^{j\Omega})$进行傅里叶逆变换就可以得到所需LTI系统的脉冲响应$h[n]$。

![image](https://images2018.cnblogs.com/blog/421096/201806/421096-20180627235517200-1861511474.png)



其中也可能会出现一些特殊情况，比如说所需要的process的PSD，即$S_{xx}(e^{j\Omega})$中包含脉冲，这些情况下，PSD可以表示为脉冲PSD与非脉冲PSD之和，由于傅里叶变换的线性性质，LTI系统的脉冲响应最终也可以表示为脉冲PSD的脉冲响应与非脉冲PSD的脉冲响应之和。非脉冲PSD可以按照上面的步骤求解，而脉冲PSD主要分为两种情况：
- 脉冲在零点。那么脉冲PSD的脉冲响应是一个常数，该常数的大小与脉冲PSD的大小有关。
- 脉冲在$\Omega = \pm \Omega_0\neq 0$上。那么脉冲PSD的脉冲响应是一个余弦函数$Acos(\Omega_0 n+\Theta)$，其中$A$以及$\Theta$都与脉冲PSD的大小有关。

![hybrid_PSD](https://images2018.cnblogs.com/blog/421096/201806/421096-20180627235517845-2066360009.png)

※这一段只是计算时需要，不算理解时的重点，如想加深了解，请参考[常用序列的DTFT](https://www.cnblogs.com/TaigaCon/p/8234987.html#TypicalDTFT)。





# Whitening Filter

输入为colored process，输出为white process的LTI系统被称为Whitening filter。如下系统框图：

![image](https://images2018.cnblogs.com/blog/421096/201806/421096-20180627235518441-471660220.png)

输入process为$x[n]$，其PSD为$\displaystyle{S_{xx}(e^{j\Omega}) = \mathcal{F}R_{xx}[m]}$，输出为白噪声，该白噪声的variance为$\sigma_{w}^2$，mean为0。

那么根据[WSS Process在LTI系统中的性质](https://www.cnblogs.com/TaigaCon/p/9127534.html#DT)，有

$\displaystyle{S_{ww}(e^{j\Omega}) = |H(e^{j\Omega})|^2S_{xx}(e^{j\Omega})}$

进一步可以推导出（详情请查看Reference的第二条链接）

$\displaystyle{|H(e^{j\Omega})|^2 = \frac{\sigma_w^2}{S_{xx}(e^{j\Omega})}}$





**Reference:**

[Alan V. Oppenheim: Signals, Systems and Inference, Chapter 10:Power Spectral Density](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-011-introduction-to-communication-control-and-signal-processing-spring-2010/readings/MIT6_011S10_chap10.pdf)

[Where does the delta function come from if we derive autocorrelation directly?](https://dsp.stackexchange.com/questions/31894/where-does-the-delta-function-come-from-if-we-derive-autocorrelation-directly)













# 语音合成vocoder（三） spectral envelope参数 - xmdxcsj的专栏 - CSDN博客





2017年05月17日 18:17:51[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2568








## 基本概念[1]

![这里写图片描述](https://img-blog.csdn.net/20170517181628033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

spectral envelope的三个性质： 

- 包络线连接峰值，并且紧紧包裹幅度谱 

- 包络线不能震荡太剧烈，需要平滑 

- 包络线不能有corner
语音信号可以模拟为激励脉冲序列与声道冲激响应的离散卷积，对于浊音信号 $y(t)$可以简化为基音周期为$T_0$脉冲串$\delta$和声道冲激响应$h(t)$的卷积： 


$y(t)=h(t)*\sum_{n=-\infty}^{\infty}\delta(t-nT_0)$


$Y(\omega)=\frac{2\pi}{T_0}H(\omega)\sum_{n=-\infty}^{\infty}\delta(\omega-n\omega)$

上式中的$H(\omega)$即为需要估计的频谱包络；$\delta(\omega)$是频谱的谐波部分。
## 常用方法[1]

### LPC

linear predictive coding一开始用于语音的传输压缩，基本思想是n时刻的信号$s(n)$是$s(n-p-1)$到$s(n-1)$的线性组合: 


$\hat s(n)=\sum_{i=1}^{p}a_i s(n-i)$


$e(n)=\hat s(n)-s(n)$

要求预测误差$e$最小，这样一帧的信号$s$可以用$e$和对应的LPC常数$a$来表示，由于$e$比较小，所以可以用更少的bit来表示，实现了信号的压缩。可以用下面的滤波器对来表示编解码的过程。 
![这里写图片描述](https://img-blog.csdn.net/20170517181701596?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**从另外一角度理解，analysis filter的作用是压缩输入信号的高幅值的频率分量，而synthesis filter的作用是放大这些分量，所以在一定程度上面synthesis filter体现了包络的信息。**
**缺点**是包络有可能包含将peak之间的噪声部分，尤其当两个peak间距比较大的时候（f0比较大），如下图。 
![这里写图片描述](https://img-blog.csdn.net/20170517181727081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### cepstrum

倒谱分析属于同态解卷积的方法，对于卷积信号，不能通过线性方法进行分离，可以通过FFT变为乘积信号，取log将乘积信号变为加性信号，IFFT变换获取倒谱，通过简单的低通滤波即可获得最后的频谱包络。 

假设两部分对应的时域信号分别是h(n)和e(n)，那么满足： 


$x(n)=h(n)*e(n)$


$X(k)=H(k)E(k)$

此时我们是无法区分开h(n)和e(n)。 

对频域两边取log： 


$log(X(k))=log(H(k))+log(E(k))$

然后进行反傅里叶变换： 


$IDFT(log(X(k)))=IDFT(log(H(k)))+IDFT(log(E(k)))$

假设此时得到的时域信号如下： 


$x'(n)=h'(n)+e'(n)$

虽然此时获得时域信号x’(n)即为倒谱，已经和原始的时域信号x(n)不一样，但是可以把时域信号的卷积关系转化为了线性加关系。 

对应上图的频域信号，可以拆分成两部分的乘积：频谱的包络和频谱的细节。包络部分对应的是频谱的低频信息，而细节部分对应的是频谱的高频信息。倒谱分析已经将两部分对应的时域信号的卷积关系转化为了线性加关系，所以只需要将倒谱通过一个低通滤波器即可获得包络部分对应的时域信号h’(t)。
## CheapTrick[2]

### 1. F0-adaptive windowing

参考pitch synchronous，语音分段的时候不再以传统的定长帧为单位，而是以f0对应的周期为单位，以保证波形和频谱的平滑连续。 

一般使用长度为$3T_0$的[Hanning窗](https://www.mathworks.com/help/signal/ref/hann.html)对语音进行切割，以当前时间点为中心，左右各取$1.5T_0$长度的语音。 


$w(n)=0.5(1-cos(2\pi \frac{n}{N})), 0 \le n \le N$

清音没有基频，一般使用一个适当的常数。
### 2. smoothing of the power spectrum

对于加窗以后的时域信号进行FFT获得对应的功率谱$P(\omega)$，由于后面第三步需要对功率谱取log操作，所以需要保证功率谱没有零值，这里在三角窗内对信号进行平滑 


$P_s(\omega)=\frac{3}{2\omega_0} \int_{-\frac{\omega_0}{3}}^{\frac{\omega_0}{3}}  P(\omega+\lambda)\,  \mathrm{d}\lambda $

### 3. liftering in the quefrency domain

可以将功率谱看做是普通信号，求出信号的包络也就是找到信号的低频部分。 

1. 首先对上一步求出来的功率谱做反傅里叶变换 


$p_s(\tau)=\mathscr{F}^{-1}[log(P_s(\omega)]$

2. 过滤得到信号的低频部分 


$l_s(\tau)=\frac{sin(\pi f_0 \tau)}{\pi f_0 \tau}$
[sinc函数](http://blog.sina.com.cn/s/blog_625c01d60100qi6f.html)第一个作用，相当于一个低通滤波器；第二个作用，它在$nT_0$点对应的幅值为0，相当于清除了原始周期脉冲串$\delta$的影响。 

3. spectral recovery 


$l_q(\tau)=q_0+2q_1 cos(\frac{2\pi \tau}{T_0})$

两个参数的经验值是1.18和-0.09。目的是为了消除前面平滑带来的畸变。 

4. 获取最后的频谱包络 


$P_l(\omega)=exp(\mathscr{F}[l_s(\tau) l_q(\tau)p_s(\tau)])$
## 参考文献

[1].Spectral Envelopes in Sound Analysis and Synthesis 

[2].CheapTrick, a spectral envelope estimator for high-quality speech synthesis










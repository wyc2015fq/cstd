# 语音合成vocoder



# （一） 概况



------

> Question1: vocoder在合成中的角色？？？

## 合成概况

语音合成主要有波形拼接和参数合成两种方法[1]。

### 波形拼接方法

![这里写图片描述](https://img-blog.csdn.net/20170517180904295?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
使用原始语音波形替代参数，合成的语音清晰自然，质量相比于参数合成方法要好。PSOLA（pitch synchronous overlap add）算法可以对拼接单元的韵律特征进行调整。

### 参数合成方法

提取参数->HMM建模->合成参数->波形重建 

合成中的vocoder的作用主要是：提取语音信号的相关参数；根据相关参数合成出最终的语音。一些vocoder[2]如下： 
- STRAIGHT可以产生高质量的合成效果，但是速度慢 
- Real-time STRAIGHT在STRAIGHT基础上简化了算法，虽然速度变快，但是以损失性能为代价 
- TANDEM-STRAIGHT相比STRAIGHT性能差不多，但是做不到实时。 
- WORLD相比于TANDEM-STRAIGHT，它在性能不变的前提下，减少了计算复杂度，实现了实时的合成。

WORLD是基于vocoder的语音合成工具 
\- F0 **DIO->Harvest** 
\- aperiodicity **PLANTIUM->D4C** 
\- spectral envelope **CheapTrick** 
![这里写图片描述](https://img-blog.csdn.net/20170517180949025?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> Question2: vocoder为什么可以用以上的三个参数来进行语音合成？？？

## 发声建模

![这里写图片描述](https://img-blog.csdn.net/20170521164047061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上图基于人发声机理的经典源-滤波器（source-filter）模型，其中**源激励部分**对应于肺部的气流和声带共同作用形成的激励，**声道谐振部分**对应于声道的调音运动。

### 声道模型

两种建模方法： 
\- 将声道看成多个不同截面积的短管串联而成的系统，“声管模型” 
\- 将声道看做一个谐振腔，“共振峰模型”

### 激励模型

声带激励分为两类，可以产生清音(/p/ /b/)或者浊音(/m/ /a/)。辅音有清有浊，而多数语言中元音均为浊音。 
\- 浊音（voiced） 
气流通过紧绷的声带，对声带进行冲击而产生振动，使声门处形成准周期性的脉冲串，激励信号简化为周期性的脉冲激励。 
\- 清音（unvoiced） 
声带处于松弛状态，不发生振动，气流通过声门直接进入声道，激励信号简化为随机白噪声。

上面的二元激励模型将复杂的产生激励过程简单的划为两部分，大大简化了声门激励的特征，但是合成语音的自然度较低。 
STRAIGHT引入了混合激励，浊音激励由分别通过高低通滤波器的随机噪声序列和脉冲序列相加，清音的激励源由噪声序列叠加一个位置随机的正脉冲并跟随一个负脉冲构成的脉冲对形成的爆破脉冲。[3]

### 对应关系

- F0对应于激励部分的e(n)e(n)中的周期脉冲序列
- spectral envelope对应于声道谐振部分的h(n)h(n)
- aperiodicity对应于混合激励部分的e(n)e(n)中的非周期序列

混合激励可以通过aperiodicity来控制浊音段中的周期激励和噪声成分的相对比重

## 声音指标

1. 音高 
   跟声波的基频F0有关，影响声音的声调（汉语的四种声调），跟声带震动的快慢有关
2. 响度 
   跟声波的振幅有关，声音的大小
3. 泛音 
   假设某个音的基频为f，则频率为2f的音称为第二泛音，频率为3f的音称为第三泛音
4. 音色 
   基音和不同泛音的能量比例关系是决定一个音的音色的核心因素。跟声带的震动频率、发音器官的送气方式、声道的形状尺寸有关。
5. 共振峰 
   共振峰是用来描述声源内部的共振，特别是对乐器而言，指的是共鸣箱内的共振。 
   pitch跟声带有关（声带震动快慢），共振峰跟口腔的固有频率有关（不同的口型和舌位对应不同的共振峰）。

## 参考文献

[1].《语音信号处理》 韩纪庆 
[2].WORLD: a vocoder-based high-quality speech synthesis system for real-time applications 
[3].基于STRAIGHT 分析的高效语音合成算法研究





# （二） 基频参数

2017年05月17日 18:13:57 [xmucas](https://me.csdn.net/xmdxcsj) 阅读数：2567



------

## 基本概念

声带每开启和关闭一次的时间就是基音周期（pitch period）,倒数即为音频频率（pitch frequency）[1]。 
基音频率取决于声带的大小、厚薄、松紧程度，以及声门上下之间的气压差的效应等。最低可达80Hz，最高可达500Hz，老年男性偏低，小孩女性偏高。它反映了声调的变化。

## 常用方法

常用的有自相关方法和倒谱方法 
Autocorrelation Function（ACF）运算可以用于寻找周期信号的周期。因为周期信号的自相关函数也是周期信号，而且周期一致。比如[YIN算法](http://blog.csdn.net/xmdxcsj/article/details/51245287)[2]就是使用的自相关方法。 
$$
r_t(\tau)=\sum_{j=t+1}^{t+W}x_jx_{j+\tau}
$$
W表示相关函数的窗口，τ表示相关函数的延时。



## DIO

World里面使用DIO[3][4]提取pitch，相比之前的方法速度更快。主要分为三步： 
\1. 使用不同截止频率的低通滤波器进行滤波 
如果滤波后的信号只包含一个周期$T_0$的信号，即为pitch。由于pitch未知，会使用多个不同截止频率的滤波器。 
\2. 计算F0候选以及置信度 
取四个周期，计算每个周期的zero-crossing/peak/dip intervals应该一致，以此为标准计算置信度。 
![这里写图片描述](https://img-blog.csdn.net/20170517181232060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
\3. 选取置信度最高的频率作为最后的pitch

## Harvest

## 参考文献

[1].《语音信号处理》 韩纪庆等 P15 
[2].YIN, a fundamental frequency estimator for speech and music 
[3].WORLD: a vocoder-based high-quality speech synthesis system for real-time applications 
[4].Fast and Reliable F0 Estimation Method Based on the Period Extraction of Vocal Fold Vibration of Singing Voice and Speech







# （三） spectral envelope参数

2017年05月17日 18:17:51 [xmucas](https://me.csdn.net/xmdxcsj) 阅读数：2471



------

## 基本概念[1]

![这里写图片描述](https://img-blog.csdn.net/20170517181628033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
spectral envelope的三个性质： 
\- 包络线连接峰值，并且紧紧包裹幅度谱 
\- 包络线不能震荡太剧烈，需要平滑 
\- 包络线不能有corner

语音信号可以模拟为激励脉冲序列与声道冲激响应的离散卷积，对于浊音信号 y(t)y(t)可以简化为基音周期为T0T0脉冲串δδ和声道冲激响应h(t)h(t)的卷积： 



y(t)=h(t)∗∑n=−∞∞δ(t−nT0)y(t)=h(t)∗∑n=−∞∞δ(t−nT0)



Y(ω)=2πT0H(ω)∑n=−∞∞δ(ω−nω)Y(ω)=2πT0H(ω)∑n=−∞∞δ(ω−nω)

上式中的

H(ω)H(ω)

即为需要估计的频谱包络；

δ(ω)δ(ω)

是频谱的谐波部分。



## 常用方法[1]

### LPC

linear predictive coding一开始用于语音的传输压缩，基本思想是n时刻的信号s(n)s(n)是s(n−p−1)s(n−p−1)到s(n−1)s(n−1)的线性组合: 



s^(n)=∑i=1pais(n−i)s^(n)=∑i=1pais(n−i)



e(n)=s^(n)−s(n)e(n)=s^(n)−s(n)

要求预测误差

ee

最小，这样一帧的信号

ss

可以用

ee

和对应的LPC常数

aa

来表示，由于

ee

比较小，所以可以用更少的bit来表示，实现了信号的压缩。可以用下面的滤波器对来表示编解码的过程。

 



从另外一角度理解，analysis filter的作用是压缩输入信号的高幅值的频率分量，而synthesis filter的作用是放大这些分量，所以在一定程度上面synthesis filter体现了包络的信息。

 

缺点

是包络有可能包含将peak之间的噪声部分，尤其当两个peak间距比较大的时候（f0比较大），如下图。

 





### cepstrum

倒谱分析属于同态解卷积的方法，对于卷积信号，不能通过线性方法进行分离，可以通过FFT变为乘积信号，取log将乘积信号变为加性信号，IFFT变换获取倒谱，通过简单的低通滤波即可获得最后的频谱包络。 
假设两部分对应的时域信号分别是h(n)和e(n)，那么满足： 
$$
x(n)=h(n)*e(n)\\
X(k)=H(k)E(k)
$$
此时我们是无法区分开h(n)和e(n)。

对频域两边取log：
$$
log(X(k))=log(H(k))+log(E(k))
$$
然后进行反傅里叶变换：
$$
IDFT(log(X(k)))=IDFT(log(H(k)))+IDFT(log(E(k)))
$$
假设此时得到的时域信号如下：
$$
x'(n)=h'(n)+e'(n)
$$
虽然此时获得时域信号x’(n)即为倒谱，已经和原始的时域信号x(n)不一样，但是可以把时域信号的卷积关系转化为了线性加关系。

 

对应上图的频域信号，可以拆分成两部分的乘积：频谱的包络和频谱的细节。包络部分对应的是频谱的低频信息，而细节部分对应的是频谱的高频信息。倒谱分析已经将两部分对应的时域信号的卷积关系转化为了线性加关系，所以只需要将倒谱通过一个低通滤波器即可获得包络部分对应的时域信号h’(t)。



## CheapTrick[2]

### 1. F0-adaptive windowing

参考pitch synchronous，语音分段的时候不再以传统的定长帧为单位，而是以f0对应的周期为单位，以保证波形和频谱的平滑连续。 
一般使用长度为3$T_0$的[Hanning窗](https://www.mathworks.com/help/signal/ref/hann.html)对语音进行切割，以当前时间点为中心，左右各取$1.5T_0$长度的语音。 
$$
w(n)=0.5(1-cos(2\pi \frac{n}{N})), 0 \le n \le N
$$
清音没有基频，一般使用一个适当的常数。

### 2. smoothing of the power spectrum

对于加窗以后的时域信号进行FFT获得对应的功率谱P(ω)P(ω)，由于后面第三步需要对功率谱取log操作，所以需要保证功率谱没有零值，这里在三角窗内对信号进行平滑 
$$
P_s(\omega)=\frac{3}{2\omega_0} \int_{-\frac{\omega_0}{3}}^{\frac{\omega_0}{3}}  P(\omega+\lambda)\,  \mathrm{d}\lambda
$$

### 3. liftering in the quefrency domain

可以将功率谱看做是普通信号，求出信号的包络也就是找到信号的低频部分。 
\1. 首先对上一步求出来的功率谱做反傅里叶变换 
$$
p_s(\tau)=\mathscr{F}^{-1}[log(P_s(\omega)]
$$
\2. 过滤得到信号的低频部分
$$
l_s(\tau)=\frac{sin(\pi f_0 \tau)}{\pi f_0 \tau}
$$
sinc函数

第一个作用，相当于一个低通滤波器；第二个作用，它在$nT_0$点对应的幅值为0，相当于清除了原始周期脉冲串δ的影响。

\3. spectral recovery
$$
l_q(\tau)=q_0+2q_1 cos(\frac{2\pi \tau}{T_0})
$$
两个参数的经验值是1.18和-0.09。目的是为了消除前面平滑带来的畸变。 

\4. 获取最后的频谱包络
$$
P_l(\omega)=exp(\mathscr{F}[l_s(\tau) l_q(\tau)p_s(\tau)])
$$


## 参考文献

[1].Spectral Envelopes in Sound Analysis and Synthesis 
[2].CheapTrick, a spectral envelope estimator for high-quality speech synthesis









# （四） aperiodicity参数

2017年05月17日 18:19:25 [xmucas](https://me.csdn.net/xmdxcsj) 阅读数：2065



------

## 基本概念

> “aperiodicity” is defined as the power ratio between the speech signal and the aperiodic component of the signal

它是跟混合激励有关的参数，为了获得自然的声音，激励源不能只使用周期信号，也需要包含一些非周期信号。一种典型的非周期信号是噪音。

## PLANTINUM[1]

使用excitation signal替换aperiodicity 



X(ω)=Y(ω)H(ω)X(ω)=Y(ω)H(ω)

首先根据F0标出voiced section，然后

y2(t)y2(t)

最大的点对应加窗的时间点，使用长度为

2T02T0

的Hanning窗。

 

合成语音的时候直接使用求出的

x(t)x(t)

和

h(t)h(t)

进行卷积即可。



## D4C[4]

D4C需要计算不同频带对应的功率比。

### 1.计算群延迟





τgω=−dϕ(ω)dωτgω=−dϕ(ω)dω

假设所加窗口的中心点在

ττ

，那么最后求出的群延迟

τ(ω,τ)τ(ω,τ)

跟

ττ

有关，使用了一些方法消除了参数

ττ

，也就是最后得到的群延迟跟窗口位置无关。



### 2.修正参数

![这里写图片描述](https://img-blog.csdn.net/20170517181857363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上一步计算加窗会带来频谱上的失真（上图a），这里进行smooth恢复出来原始的信号。 
$$
\tau_{gs}(\omega)=\frac{2}{\omega_0} \int_{-\frac{\omega_0}{4}}^{\frac{\omega_0}{4}}  \tau_g(\omega+\lambda)\,  \mathrm{d}\lambda\\
\tau_{gb}(\omega)=\frac{1}{\omega_0} \int_{-\frac{\omega_0}{2}}^{\frac{\omega_0}{2}}  \tau_{gs}(\omega+\lambda)\,  \mathrm{d}\lambda\\
\tau_D=\tau_{gs}(\omega)-\tau_{gb}(\omega)
$$

### 3.估计band-aperiodicity

中心频率$ω_c$对应的时域信号 
$$
p(t,\omega_c)=\mathscr{F}^{-1}[w(\omega)\tau_{D}(\omega-(\omega_c-\frac{w_l}{2}))]
$$
$w_l​$表示窗的长度 
$$
p_c(t,\omega_c)=1-\int_{0}^{t}  p_s(\lambda, \omega_c)\,  \mathrm{d}\lambda
$$
其中$p_s$是由功率谱$|p|^2$在时间上按降序排序得到，而且$|p|^2$做了归一化

band aperiodicity可得
$$
ap(\omega_c)=-10log_{10}(p_c(2w_{bw},\omega_c))
$$
$w_{bw}$表示窗函数的主瓣宽度。



## 参考文献

[1].PLATINUM: A method to extract excitation signals for voice synthesis system 
[2].D4C, a band-aperiodicity estimator for high-quality speech synthesis





# （五） synthesis

2017年05月17日 18:21:40 [xmucas](https://me.csdn.net/xmdxcsj) 阅读数：2089



------

## 基本概念

最小相位脉冲响应[1]可以保证波形在时域上基本不变。 
根据频谱包络求出最小相位响应（减弱时域信号的相位失真），然后IFFT还原为语音信号 
![这里写图片描述](https://img-blog.csdn.net/20170517182051333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
其中AA跟频谱包络有关

## 合成流程

合成[2]分为三步 
\1. 根据$f_0$确定脉冲的位置 
对分帧的频谱插值获得脉冲对应的频谱spectrumspectrum 
\2. 时域周期信号 
2.1 求出频谱包络中周期部分，然后过最小相位脉冲响应 
$$
period\_spectrum=spectrum[i] \cdot  (1-aperiodic\_ratio[i])
$$
2.2 反傅里叶变换得到时域信号，并去除直流分量
$$
RemoveDC(IFFT(period\_spectrum))
$$
\3. 时域非周期信号

 

3.1 求出频谱包络中非周期部分，然后过最小相位脉冲响应
$$
aperiod\_spectrum=spectrum[i] \cdot  aperiodic\_ratio[i]
$$
3.2 高斯白噪声的幅度谱
$$
noise\_spectrum=FFT(noise)
$$
3.3 获取最后的非周期时域信号
$$
IFFT(aperiod\_spectrum\cdot noise\_spectrum)
$$
\4. 时域周期信号和时域非周期信号相加得到最后的合成信号



## 代码细节

GetTimeBase() 
synth->interpolated_vuv：voice的概率 
synth->pulse_locations：脉冲对应的时间 
synth->pulse_locations_index：脉冲对应的采样点 
1.线性插值的到每个sample对应的f0和vuv 
原来5ms对应一帧，有一个f0，扩展到每一个采样点一个f0，直接线性插值 
2.获取pulse location，[参考wiki](https://en.wikipedia.org/wiki/Instantaneous_phase) 
每个采样点对应的$phase=\frac{2\pi  f_0}{f_s}$
每个采样点对应的累积相位 
每个采样点对应的$wrap\_phase=total\_phase\ mod\ 2\pi$
每个采样点对应的$wrap\_phase[n] - wrap\_phase[n-1]>\pi​$即为新的pulse的起点

## 参考文献

[1].speech representation and transformation using adaptive interpolation of weighted spectrum vocoder revisited 
[2].<https://github.com/mmorise/World>




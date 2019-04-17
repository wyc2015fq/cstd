# 语音合成vocoder（四） aperiodicity参数 - xmdxcsj的专栏 - CSDN博客





2017年05月17日 18:19:25[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2139








## 基本概念

> 
“aperiodicity” is defined as the power ratio between the speech signal and the aperiodic component of the signal


它是跟混合激励有关的参数，为了获得自然的声音，激励源不能只使用周期信号，也需要包含一些非周期信号。一种典型的非周期信号是噪音。

## PLANTINUM[1]

使用excitation signal替换aperiodicity 


$X(\omega)=\frac{Y(\omega)}{H(\omega)}$

首先根据F0标出voiced section，然后$y^2(t)$最大的点对应加窗的时间点，使用长度为$2T_0$的Hanning窗。 

合成语音的时候直接使用求出的$x(t)$和$h(t)$进行卷积即可。
## D4C[4]

D4C需要计算不同频带对应的功率比。

### 1.计算群延迟



$\tau_{g}{\omega}=-\frac{d\phi(\omega)}{d\omega}$

假设所加窗口的中心点在$\tau$，那么最后求出的群延迟$\tau(\omega, \tau)$跟$\tau$有关，使用了一些方法消除了参数$\tau$，也就是最后得到的群延迟跟窗口位置无关。

### 2.修正参数

![这里写图片描述](https://img-blog.csdn.net/20170517181857363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上一步计算加窗会带来频谱上的失真（上图a），这里进行smooth恢复出来原始的信号。 


$\tau_{gs}(\omega)=\frac{2}{\omega_0} \int_{-\frac{\omega_0}{4}}^{\frac{\omega_0}{4}}  \tau_g(\omega+\lambda)\,  \mathrm{d}\lambda$


$\tau_{gb}(\omega)=\frac{1}{\omega_0} \int_{-\frac{\omega_0}{2}}^{\frac{\omega_0}{2}}  \tau_{gs}(\omega+\lambda)\,  \mathrm{d}\lambda$


$\tau_D=\tau_{gs}(\omega)-\tau_{gb}(\omega)$
### 3.估计band-aperiodicity

中心频率$\omega_c$对应的时域信号 


$p(t,\omega_c)=\mathscr{F}^{-1}[w(\omega)\tau_{D}(\omega-(\omega_c-\frac{w_l}{2}))]$
$w_l$表示窗的长度 


$p_c(t,\omega_c)=1-\int_{0}^{t}  p_s(\lambda, \omega_c)\,  \mathrm{d}\lambda$

其中$p_s$是由功率谱$|p|^2$在时间上按降序排序得到，而且$|p|^2$做了归一化 

band aperiodicity可得 


$ap(\omega_c)=-10log_{10}(p_c(2w_{bw},\omega_c))$
$w_{bw}$表示窗函数的主瓣宽度。
## 参考文献

[1].PLATINUM: A method to extract excitation signals for voice synthesis system 

[2].D4C, a band-aperiodicity estimator for high-quality speech synthesis







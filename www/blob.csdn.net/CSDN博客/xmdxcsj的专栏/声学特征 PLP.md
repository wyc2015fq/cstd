# 声学特征 PLP - xmdxcsj的专栏 - CSDN博客





2017年11月12日 16:20:16[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1613








## PLP的由来

[Linear prediction](https://en.wikipedia.org/wiki/Linear_prediction)可以用来获得语音功率谱$P(\omega)$的全极点模型$A(\omega)$，也可以把LP看做获取$P(\omega)$的频谱包络的手段，参考[前面的文章](http://blog.csdn.net/xmdxcsj/article/details/72419948)

由于LP对待所有频率一视同仁，它不符合人耳的听觉机理，比如人耳对于高于800Hz的感知会下降，对于中间频段更敏感。 

为了解决这个问题，Hermansky在进行LP之前修改语音的功率谱$P(\omega)$，称为perceptual linear predictive。可以理解为更符合听觉感知的LP。
## PLP流程

在LP之前修改$P(\omega)$，需要的步骤见下图 
![这里写图片描述](https://img-blog.csdn.net/20171112161746973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### critical-band spectral resolution

首先在频率轴上将Hz转化为Bark，参考[bark scale](https://en.wikipedia.org/wiki/Bark_scale),500Hz以下跟频率近似线性关系，500Hz以上近似log关系 
![这里写图片描述](https://img-blog.csdn.net/20171112161807742?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


$\Omega(\omega)=6ln\{\omega/1200\pi+[(\omega/1200\pi)^2+1]^{0.5}\}$

然后和critial-band masking curve $\Psi(\Omega)$进行卷积 


$\Theta(\Omega_i)=\sum_{\Omega=-1.3}^{2.5}P(\Omega-\Omega_i)\Psi(\Omega)$
### equal-loudness preemphasis

为了模拟听觉对于不同频带敏感度的不同，使用equal-loudness curve对频谱进行预加重 


$\Xi[\Omega(\omega)]=E(\omega)\Theta[\Omega(\omega)]$

其中$E(\omega)$如下： 


$E(\omega)=[(\omega^2+56.8*10^6)\omega^4]/[(\omega^2+6.3*10^6)^2*(\omega^2+0.38*10^9)]$
### intensity-loudness power law

为了描述声音强度和感知响度之间的关系 


$\Phi(\Omega)=\Xi(\Omega)^{0.33}$

## 参考

Perceptual linear predictive (PLP) analysis of speech








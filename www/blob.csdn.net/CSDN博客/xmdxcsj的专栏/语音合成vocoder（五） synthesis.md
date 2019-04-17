# 语音合成vocoder（五） synthesis - xmdxcsj的专栏 - CSDN博客





2017年05月17日 18:21:40[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2139








## 基本概念

最小相位脉冲响应[1]可以保证波形在时域上基本不变。 

根据频谱包络求出最小相位响应（减弱时域信号的相位失真），然后IFFT还原为语音信号 
![这里写图片描述](https://img-blog.csdn.net/20170517182051333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中$A$跟频谱包络有关
## 合成流程

合成[2]分为三步 

1. 根据$f_0$确定脉冲的位置 

对分帧的频谱插值获得脉冲对应的频谱$spectrum$

2. 时域周期信号 

2.1 求出频谱包络中周期部分，然后过最小相位脉冲响应 


$period\_spectrum=spectrum[i] \cdot  (1-aperiodic\_ratio[i])$

2.2 反傅里叶变换得到时域信号，并去除直流分量 


$RemoveDC(IFFT(period\_spectrum))$

3. 时域非周期信号 

3.1 求出频谱包络中非周期部分，然后过最小相位脉冲响应 


$aperiod\_spectrum=spectrum[i] \cdot  aperiodic\_ratio[i]$

3.2 高斯白噪声的幅度谱 


$noise\_spectrum=FFT(noise)$

3.3 获取最后的非周期时域信号 


$IFFT(aperiod\_spectrum\cdot noise\_spectrum)$

4. 时域周期信号和时域非周期信号相加得到最后的合成信号
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

每个采样点对应的$wrap\_phase[n] - wrap\_phase[n-1]>\pi$即为新的pulse的起点
## 参考文献

[1].speech representation and transformation using adaptive interpolation of weighted spectrum vocoder revisited 

[2].[https://github.com/mmorise/World](https://github.com/mmorise/World)







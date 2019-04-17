# 语音合成vocoder（二） 基频参数 - xmdxcsj的专栏 - CSDN博客





2017年05月17日 18:13:57[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2665








## 基本概念

声带每开启和关闭一次的时间就是基音周期（pitch period）,倒数即为音频频率（pitch frequency）[1]。 

基音频率取决于声带的大小、厚薄、松紧程度，以及声门上下之间的气压差的效应等。最低可达80Hz，最高可达500Hz，老年男性偏低，小孩女性偏高。它反映了声调的变化。

## 常用方法

常用的有自相关方法和倒谱方法 

Autocorrelation Function（ACF）运算可以用于寻找周期信号的周期。因为周期信号的自相关函数也是周期信号，而且周期一致。比如[YIN算法](http://blog.csdn.net/xmdxcsj/article/details/51245287)[2]就是使用的自相关方法。 


$r_t(\tau)=\sum_{j=t+1}^{t+W}x_jx_{j+\tau}$

W表示相关函数的窗口，$\tau$表示相关函数的延时。
## DIO

World里面使用DIO[3][4]提取pitch，相比之前的方法速度更快。主要分为三步： 

1. 使用不同截止频率的低通滤波器进行滤波 

如果滤波后的信号只包含一个周期$T_0$的信号，即为pitch。由于pitch未知，会使用多个不同截止频率的滤波器。 

2. 计算F0候选以及置信度 

取四个周期，计算每个周期的zero-crossing/peak/dip intervals应该一致，以此为标准计算置信度。 
![这里写图片描述](https://img-blog.csdn.net/20170517181232060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3. 选取置信度最高的频率作为最后的pitch
## Harvest

## 参考文献

[1].《语音信号处理》 韩纪庆等 P15 

[2].YIN, a fundamental frequency estimator for speech and music 

[3].WORLD: a vocoder-based high-quality speech synthesis system for real-time applications 

[4].Fast and Reliable F0 Estimation Method Based on the Period Extraction of Vocal Fold Vibration of Singing Voice and Speech






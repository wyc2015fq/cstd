# 声学特征（二） MFCC特征原理 - xmdxcsj的专栏 - CSDN博客





2016年04月23日 22:36:57[xmucas](https://me.csdn.net/xmdxcsj)阅读数：39762








## 基本含义

MFCC是Mel-Frequency Cepstral Coefficients的缩写，顾名思义MFCC特征提取包含两个关键步骤：转化到**梅尔频率**，然后进行**倒谱分析**。

### 梅尔频率

[梅尔刻度](https://zh.wikipedia.org/wiki/梅尔刻度)是一种基于人耳对等距的音高(pitch)变化的感官判断而定的非线性频率刻度。和频率的赫兹的关系如下: 


$m=2595log_{10} (1+\frac{f}{700})$

所以当在梅尔刻度上面上是均匀分度的话，对于的赫兹之间的距离将会越来越大，所以梅尔刻度的滤波器组的尺度变化如下： 
![bank](https://img-blog.csdn.net/20160423223221190)

梅尔刻度的滤波器组在低频部分的分辨率高，跟人耳的听觉特性是相符的，这也是梅尔刻度的物理意义所在。 

这一步的含义是：首先对时域信号进行傅里叶变换转换到频域，然后再利用梅尔频率刻度的滤波器组对应频域信号进行切分，最后每个频率段对应一个数值。
### 倒谱分析

[倒谱](https://en.wikipedia.org/wiki/Cepstrum)的含义是：对时域信号做傅里叶变换，然后取log，然后再进行反傅里叶变换。可以分为复倒谱、实倒谱和功率倒谱，我们用的是功率倒谱。 

倒谱分析可用于将信号分解，两个信号的卷积转化为两个信号的相加。举例如下： 
![signal](https://img-blog.csdn.net/20160423223258394)

假设上面的频率谱X(k)，时域信号为x(n)那么满足 


$X(k)=DFT(x(n))$

考虑将频域X(k)拆分为两部分的乘积： 


$X(k)=H(k)E(k)$

假设两部分对应的时域信号分别是h(n)和e(n)，那么满足： 


$x(n)=h(n)*e(n)$

此时我们是无法区分开h(n)和e(n)。 

对频域两边取log： 


$log(X(k))=log(H(k))+log(E(k))$

然后进行反傅里叶变换： 


$IDFT(log(X(k)))=IDFT(log(H(k)))+IDFT(log(E(k)))$

假设此时得到的时域信号如下： 


$x'(n)=h'(n)+e'(n)$

虽然此时获得时域信号x’(n)即为倒谱，已经和原始的时域信号x(n)不一样，但是可以把时域信号的卷积关系转化为了线性加关系。 

对应上图的频域信号，可以拆分成两部分的乘积：频谱的包络和频谱的细节。频谱的峰值即为[共振峰](http://baike.baidu.com/link?url=-WG5pW1Nlfv1hi8EQuDspoxC90-ueQtDU_Iez2rWJYrtjLEHTepLUmOt9IIHojkApeOVUwOSmBjSUPHKOMAjz_)，它决定了信号频域的包络，是辨别声音的重要信息，所以进行倒谱分析目的就是获得频谱的包络信息。包络部分对应的是频谱的低频信息，而细节部分对应的是频谱的高频信息。倒谱分析已经将两部分对应的时域信号的卷积关系转化为了线性加关系，所以只需要将倒谱通过一个低通滤波器即可获得包络部分对应的时域信号h’(t)。
## 基本流程

### 预加重
- 目的 

为了消除发声过程中，声带和嘴唇造成的效应，来补偿语音信号受到发音系统所压抑的高频部分。并且能突显高频的共振峰。 

简单理解就是在频域上面都乘以一个系数，这个系数跟频率成正相关，所以高频的幅值会有所提升。实际上就是通过了一个$H(z)=1-kz^{-1}$高通滤波器。- 实现 


$s_n'=s_n-k*s_{n-1}$

### 加窗
- 目的 

用于平滑信号，使用汉明窗加以平滑的话，相比于矩形窗函数，会减弱FFT以后旁瓣大小以及频谱泄露。
- 实现 

使用汉明窗对信号进行加窗处理 


$s_n'=\{0.54-0.46cos(\frac{2\pi(n-1)}{N-1})\}*s_n$
### 频域转换
- 目的 

将时域信号转化到频域进行后续的频率分析
- 实现 

幅度谱： 


$S_i(k)=\sum_{n=1}^{N}s_i(n)e^{-j2\pi kn/N}  1\le k \le K$

功率谱： 


$P_i(k)=\frac{1}{N}|S_i(k)^2|$
### 使用梅尔刻度滤波器组过滤
- 目的 

因为频域信号有很多冗余，滤波器组可以对频域的幅值进行精简，每一个频段用一个值来表示。
- 实现 

对于FFT得到的幅度谱，分别跟每一个滤波器进行频率相乘累加，得到的值即为该帧数据在在该滤波器对应频段的能量值。如果滤波器的个数为22，那么此时应该得到22个能量值

### 能量值取log

由于人耳对声音的感知并不是线性的，用log这种非线性关系更好描述。取完log以后才可以进行倒谱分析。

### 离散余弦变换

[DCT](https://en.wikipedia.org/wiki/Discrete_cosine_transform)和DFT类似，但是只使用实数，不涉及复数运算。信号经过DCT变换以后，能量会集中到低频部分，可以用于图像压缩。 

1. 目的 

按照倒谱的定义，该步需要进行反傅里叶变换然后通过低通滤波器获得最后的低频信号。这里使用DCT直接就可以获取频率谱的低频信息。 

由于滤波器之间是有重叠的，所以前面的获得的能量值之间是具有相关性的，DCT还可以对数据进行降维压缩和抽象，获得最后的特征参数。相比于傅里叶变换，离散余弦变换的结果没有虚部，更好计算。 

2. 实现 


$c_i=\sqrt{\frac{2}{N}}\sum_{j=1}^{N}m_jcos(\frac{\pi i}{N}(j-0.5))$
### 差分
- 目的 

由于语音信号是时域连续的，分帧提取的特征信息只反应了本帧语音的特性，为了使特征更能体现时域连续性，可以在特征维度增加前后帧信息的维度。常用的是一阶差分和二阶差分。
- 实现 


$d_t=\frac{\sum_{\theta =1}^{\Theta}\theta (c_{t+\theta}-c_{t-\theta})}{2\sum_{\theta =1}^{\Theta}\theta^2}$

## MFSC和MFCC

参考【3】第四章，MFSC(log mel-frequency spectral Coefficients)相比于MFCC，缺少了DCT这一步处理。相比于MFSC，MFCC具有以下优势： 

- 对特征进行了去相关和压缩，更适合Diagonal covariance GMMs这种建模方式。 

- MFCC数据量少。 

但是对于DNN这种训练方式，发现MFSC的效果要比MFCC好。主要原因： 

- MFSC具有相关性，表现在频谱上面的较为平滑 

- MFSC具有更高的维度
## 参考文献
- htk book
- [http://practicalcryptography.com/miscellaneous/machine-learning/guide-mel-frequency-cepstral-coefficients-mfccs/](http://practicalcryptography.com/miscellaneous/machine-learning/guide-mel-frequency-cepstral-coefficients-mfccs/)
- [https://tspace.library.utoronto.ca/bitstream/1807/44123/1/Mohamed_Abdel-rahman_201406_PhD_thesis.pdf](https://tspace.library.utoronto.ca/bitstream/1807/44123/1/Mohamed_Abdel-rahman_201406_PhD_thesis.pdf)
- [http://www.speech.cs.cmu.edu/15-492/slides/03_mfcc.pdf](http://www.speech.cs.cmu.edu/15-492/slides/03_mfcc.pdf)











# 语音特征参数MFCC计算的详细过程

2012年10月24日 14:10:32 [xiaoding133](https://me.csdn.net/xiaoding133) 阅读数：42691



​            **注：**老早之前就在看语音信号处理方面的知识，每当过了很久都会忘记，由于之前对语音特征MFCC提取的流程还是非常清楚的，但是对于一些细节以及一些原理一些的东西还是不是很明白，通过这次的总结，我终于明白的其中的技术细节以及设计方法，包括滤波器的设计，以及参数的具体意义，希望这个总结能给自己增加更多的印象，课本上好像对各个三角滤波器的设计没有过多的讲解，也没讲流程，希望这里的总结能有所帮助。

**一、MFCC计算总体流程**

1.信号的预处理，包括预加重(Preemphasis)，分帧(Frame Blocking)，加窗(Windowing)。假设语音信号的采样频率fs=8KHz.由于语音信号在10-30ms认为是稳定的，则可设置帧长为80~240点。帧移可以设置为帧长的1/2.

2.对每一帧进行FFT变换，求频谱，进而求得幅度谱。

3.对幅度谱加Mel滤波器组(**Mel滤波器组设计问题**)。

4.对所有的滤波器输出做对数运算(Logarlithm)，再进一步做离散余弦变换DCT可得MFCC。

![img](https://img-my.csdn.net/uploads/201210/24/1351058438_6657.jpg)

**流程图如下所示：**

![img](https://img-my.csdn.net/uploads/201210/24/1351058521_6503.jpg)

**二、实际频率与Mel频率的转换**

Mel频率与实际频率的具体关系如下：

 ![img](https://img-my.csdn.net/uploads/201210/24/1351058598_7566.jpg)

人耳的听觉特性与Mel频率的增长一致。与实际频率在1000Hz以下呈现线性分布，1000Hz以上呈现对数增长。

在Mel频率轴上配置K个通道的三角形滤波器组，K的个数由信号的截止频率决定。

**设计的过程如下：**

假设语音信号的采样频率,帧长N=256，滤波器个数K=22

由此可得语音信号的最大频率为：

![img](https://img-my.csdn.net/uploads/201210/24/1351058669_5667.jpg)

根据下面的公式：

![img](https://img-my.csdn.net/uploads/201210/24/1351058598_7566.jpg)

 

可以求得出最大的Mel频率为：![img](https://img-my.csdn.net/uploads/201210/24/1351058767_5557.jpg)

由于在Mel刻度范围内，各个三角滤波器的中心频率是相等间隔的线性分布。由此，可以计算两个相邻三角滤波器的中心频率的间距为：

![img](https://img-my.csdn.net/uploads/201210/24/1351058811_4807.jpg)

因此，各三角形滤波器在mel刻度上的中心频率可以表示为：

由上面的中心频率可以计算出对应的线性刻度上的频率。如下图所示：

![img](https://img-my.csdn.net/uploads/201210/24/1351058877_6942.jpg)

**下面采用Matlab设计Mel三角形滤波器如下：**

```matlab
%获取Mel三角滤波器的参数
%fs为采样频率,filterNum为三角滤波器的个数
maxMelFreq = freq2mel(fs/2); %将线性频率转化为mel频率，得到最大的Mel频率
sideWidth=maxMelFreq/(filterNum+1);%求频带宽带，即Mel滤波器宽度
index=0:filterNum-1;%滤波器的中心
filterBankPrm=floor(mel2freq([index;index+1;index+2]*sideWidth)/fs*frameSize)+1;
filterBankPrm(end, end)= frameSize/2;
```

 得到如下的三角形滤波器，横坐标对应于FFT中的点的下标：

![img](https://img-my.csdn.net/uploads/201210/24/1351060041_8899.jpg)

 注：这里选择的帧长为256点，然后FFT的点数也为256，由于是对称的，所以只取前面一半的点计算频谱。然后加入到三角滤波器中。

每一个三角形滤波器的中心频率c(l) 在Mel频率轴上等间隔分布。设o(l),c(l),h(l) 分别是第l 个三角形滤波器的下限，中心，和上限频率，则相邻三角形滤波器之间的下限，中心，上限频率的关系如下：c(l)=h(l-1)=o(l+1)

![img](https://img-my.csdn.net/uploads/201210/24/1351059074_5995.jpg)

根据语音信号幅度谱![img](https://img-my.csdn.net/uploads/201210/24/1351059128_9524.jpg)求每个三角形滤波器的输出。

![img](https://img-my.csdn.net/uploads/201210/24/1351059184_4921.jpg)

对所有的滤波器输出做对数运算，再进一步做离散余弦变换(DCT)即可得到MFCC。

![img](https://img-my.csdn.net/uploads/201210/24/1351059232_2127.jpg)

由上公式可知，MFCC特征参数的长度与滤波器的个数无关。只与DCT有关。



```matlab
% === 对幅度谱进行三角滤波过程
function tbfCoef = triBandFilter(fftMag, P, filterBankParam)
fstart=filterBankParam(1,:);                %fftMag一帧数据的幅度谱
fcenter=filterBankParam(2,:);%滤波器的中心点，每列代表一个滤波器的中心频率
fstop=filterBankParam(3,:);
% Triangular bandpass filter.
for i=1:P %滤波器个数
   for j = fstart(i):fcenter(i), %第i个滤波器起始频谱点和中心频谱点的输出
      filtmag(j) = (j-fstart(i))/(fcenter(i)-fstart(i));
   end
   for j = fcenter(i)+1:fstop(i),
      filtmag(j) = 1-(j-fcenter(i))/(fstop(i)-fcenter(i));
   end
   tbfCoef(i) = sum(fftMag(fstart(i):fstop(i)).*filtmag(fstart(i):fstop(i))'); %第i个滤波器的输出
end

tbfCoef=log(eps+tbfCoef.^2); %求得每个滤波器的对数输出，有多少个滤波器就有多少个输出，对应为每一帧

```

 

 
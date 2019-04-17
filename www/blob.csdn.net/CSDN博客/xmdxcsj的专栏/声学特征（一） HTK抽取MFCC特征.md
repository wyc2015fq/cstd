# 声学特征（一） HTK抽取MFCC特征 - xmdxcsj的专栏 - CSDN博客





2016年04月22日 23:07:05[xmucas](https://me.csdn.net/xmdxcsj)阅读数：4545








## 一、相关命令
- HCopy 抽取特征

```
#config: 下面的参数配置文件
#scp: 源文件和目的文件在一行
HCopy -C config -S scp
```
- HList 
- 查看语音数据
- 查看转化是否正常
- 查看抽取的特征数据


## 二、参数配置

### 参数配置文件

```
SOURCEKIND     = WAVEFORM
SOURCEFORMAT   = WAV 
SOURCERATE     = 625 
TARGETKIND     = MFCC_D_A_E_Z
TARGETRATE     = 100000.0
SAVECOMPRESSED = F 
SAVEWITHCRC    = F 
WINDOWSIZE     = 250000.0
USEHAMMING     = T 
PREEMCOEF      = 0.97
NUMCHANS       = 26
CEPLIFTER      = 22
NUMCEPS        = 12
ENORMALISE     = T 
USEPOWER       = F 
LOFREQ         = 40
HIFREQ         = 7800
```

### 参数含义
- SOURCEKIND
- TARGETKIND

```
#特征种类
0 WAVEFORM sampled waveform
1 LPC linear prediction filter coefficients
2 LPREFC linear prediction reflection coefficients
3 LPCEPSTRA LPC cepstral coefficients
4 LPDELCEP LPC cepstra plus delta coefficients
5 IREFC LPC reflection coef in 16 bit integer format
6 MFCC mel-frequency cepstral coefficients
7 FBANK log mel-filter bank channel outputs
8 MELSPEC linear mel-filter bank channel outputs
9 USER user defined sample kind
10 DISCRETE vector quantised data
11 PLP PLP cepstral coefficients
#后缀修饰
_E 000100 has energy
_N 000200 absolute energy suppressed
_D 000400 has delta coefficients
_A 001000 has acceleration coefficients
_C 002000 is compressed
_Z 004000 has zero mean static coef.
_K 010000 has CRC checksum
_O 020000 has 0’th cepstral coef.
```
- SOURCERATE 

单位为100ns，16k对应625
- TARGETRATE 

单位为100ns，10ms对应100000
- WINDOWSIZE 

单位为100ns，25ms对应250000
- ZMEANSOURCE 

去除信号的直流分量
- PREEMCOEF 

预加重信号，加重系数是k，目的就是为了消除发声过程中，声带和嘴唇造成的效应，来补偿语音信号受到发音系统所压抑的高频部分。并且能突显高频的共振峰。 


$s_n'=s_n-k*s_{n-1}$- USEHAMMING 

使用汉明窗对数据进行平滑，增加分段数据的连续性。 


$s_n'=\{0.54-0.46cos(\frac{2\pi(n-1)}{N-1})\}*s_n$- ADDDITHER 

对输入语音增加随机噪声，为了解决语音数据溢出的问题。 


$s_n'=s_n+q*RND()$- USEPOWER 

fBank特征使用功率替换幅值
- LOFREQ HIFREQ 

默认的频率范围从0到奈奎斯特频率（采样频率的一半）
- NUMCHANS 

fbank使用的滤波器组的个数
- NUMCEPS 

倒谱系数的个数，对fbank的输出做DCT以后保留的倒谱个数。
- ENORMALISE 

能量标准化，一帧的能量减去句子中的最大能量，然后加1。







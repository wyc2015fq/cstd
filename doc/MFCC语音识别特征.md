# MFCC语音识别特征

2017年02月16日 20:56:26 [quheDiegooo](https://me.csdn.net/quheDiegooo) 阅读数：3728



MFCC全程为**mel frequency cepstral coefficients （****梅尔频率倒谱系数）**

**过程如下所示：**

**![img](https://img-blog.csdn.net/20170216175916965?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



## 1.Pre emphasis

Pre emphasis目的是boosting，增加高频能量，从而提高phone的的识别率

## 2.加窗

加窗的目的是让语音信号在一帧内统计特性是固定的，便于构建phone或者subphone分类器。过程如下

![img](https://img-blog.csdn.net/20170216181521941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上图所示：每帧窗口25ms，帧位移为10ms

实际中加窗都用hamming 进行加窗，为的是避免在窗口边界处不连续导致后面傅里叶分析时出现问题，

Hamming 加窗算法和矩形加窗如下所示

![img](https://img-blog.csdn.net/20170216182239210?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

对应的效果图如下所示（对比边界处，可以看到hamming加窗是连续的，而矩阵加窗是非连续的）

![img](https://img-blog.csdn.net/20170216182349366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 3.Discrete Fourier Transform

离散傅里叶变换，目的是得到不同频带，每帧信号所包含的能量。

如下图所示是25ms帧元音[iy]对应的DFT变换

**![img](https://img-blog.csdn.net/20170216184112000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

## 4.Mel filter bank and log

人耳对1000Hz以上的声音不是很敏感，因此对1000Hz以上的进行log操作，对1000以下的做线性操作，从而提高识别性能



公式如下：

![img](https://img-blog.csdn.net/20170216201508957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如图所示：

![img](https://img-blog.csdn.net/20170216201601984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 5.倒谱

离散傅里叶变换逆变换(The Cepstrum: Inverse Discrete Fourier Transform )

目的是提高语音识别性能，公式如下所示：取前12个参数

![img](https://img-blog.csdn.net/20170216203751370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 6.能量和加速度(Deltas and Energy )

声音信号具有连续特性，所以在5的基础之上，添加了速度和加速度特征信号

能量信号计算公式如下：

![img](https://img-blog.csdn.net/20170216205316978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

速度特征计算如下：

![img](https://img-blog.csdn.net/20170216205400407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

综上所示，39维MFCC特征包括如下信息

![img](https://img-blog.csdn.net/20170216205459986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**参考文献：**

[1]http://practicalcryptography.com/miscellaneous/machine-learning/guide-mel-frequency-cepstral-coefficients-mfccs/#deltas-and-delta-deltas
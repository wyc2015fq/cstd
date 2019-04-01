# 语音识别中声学模型训练过程-GMM（一）

2017年02月17日 21:52:06 [quheDiegooo](https://me.csdn.net/quheDiegooo) 阅读数：9674



在上一章语音识别过程中提到的**P(O|W )**称做观测最大释然，由声学模型计算可得，本章就主要描述HMM+GMM来计算最大释然的过程。

首先回顾一下：在解码过程中

![img](https://img-blog.csdn.net/20170217200127292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**P****(****O****|****W** **)**由声学模型训练得到，



**P****(****O****|****W** **)是W的似然函数，结合之前讲述的声学特征也就是说，**在给定的W情况，使得当前的特征向量（MFCC）的概率最大，结合HMM的概念，也就是说在在t时刻给定状态*q**i* 的前提下，求输出O的概率，即*p*(*o**t*|*q**i*) ，即矩阵B，状态对应的是word，phone或者subphone，在LVCSR中对应的是subphone

在解码阶段，在固定观测向量*o**t 的前提下，我们需要计算每一个HMM状态可能产生的概率，然后找到最大可能的状态（subphone）序列，所以训练过程就是计算观测似然矩阵B的过程。*

*理想的方式计算MFCC的时候，可以把输入的帧映射为一些离散的量化符号方便计算，如下图所示*

*![img](https://img-blog.csdn.net/20170217203000723?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*然后这么计算似然是有问题的，因为输入音频是连续的，特征基本变化很大的，很难进行比较好的聚类，因此提出连续空间的概率密度函数（PDF），最常用的计算声学似然的方式是高斯混合模型，即GMM模型（当然SVM，CRF，NN也可以）。*

*高斯分布也是一种正态分布，函数如下所示*

*![img](https://img-blog.csdn.net/20170217203931613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*不同的均值，方差下，对应的高斯分布如下所示：*

*![img](https://img-blog.csdn.net/20170217204019316?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*离散情况下，均值，方差计算如下所示：*

  *![img](https://img-blog.csdn.net/20170217204149801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*![img](https://img-blog.csdn.net/20170217204225787?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*当高斯函数用来当做概率密度函数时，曲线下的面积和应该为1，如下所示,灰色区域面积为0.341*

*![img](https://img-blog.csdn.net/20170217204435384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*



*我们可以用单GMM pdf来估测一个特定的HMM状态j，产生一个单一维度的声学特征向量O的概率，(假设*

*o**t 服从正态分布**)，换句话说，就是用对一维特征来说，一元高斯来代表观测似然函数*bj (ot )  ，

假设HMM状态j 对应的均值方式是μj 和σ2j  ，那么计算似然  bj (ot ) 可以通过Gaussian PDF来计算，如下所示：

![img](https://img-blog.csdn.net/20170217210458328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

有了以上公式，我们就可以来进行HMM decoding了

然而如何计算均值和方差呢，按理来说可以通过如下公式进行计算

![img](https://img-blog.csdn.net/20170217210743985?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)











*然而，状态是HMM隐藏状态，我们不知道Ot是由哪个状态产生的，*

*但是我们可以通过HMM中t时刻在状态i的概率来按比例分配，即把每个Ot分配给每个可能的状态i。*

*把ξt (i) 记做在t时刻状态i产生Ot的概率，*

*那么通过Baum-Welch 迭代*算法进行计算，如下所示：

*![img](https://img-blog.csdn.net/20170217211624833?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*也叫作HMM前向-后向（Baum-Welch）训练。*

*以上讨论的是一维特征，在实际中，MFCC是39维特征，因此我们使用了多元高斯模型，*

*多元高斯分布函数如下所示：*

*![img](https://img-blog.csdn.net/20170217213827582?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*协方差计算公式如下：*

*![img](https://img-blog.csdn.net/20170217214033257)*

*则，高斯概率似然函数b j (ot ) 如下所示：*

*![img](https://img-blog.csdn.net/20170217214231549)*

*因为对角协方差计算量更小，所以可以简化为对角协方差*

*可以表述为：在t时刻，状态j产生声学特征向量Ot的似然函数可以表述为对角协方差多元高斯，*

*上述公式可以简化成如下公式：*



*![img](https://img-blog.csdn.net/20170217214620816)*

*ξt (i) 记做t时刻，状态i产生Ot的似然，对应均值方差为：*

*![img](https://img-blog.csdn.net/20170217215840844?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*以上的前提声学特征符合正态分布，但是实际倒谱特征MFCC不是正态分布，因此我们可以改进为*

*加权的混合多远高斯模型，即GMM，对应函数公式如下所示：*

*![img](https://img-blog.csdn.net/20170217220515305?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*对应的输出似然函数**b* *j* *(**o**t* *)* *如下所示：*

*![img](https://img-blog.csdn.net/20170217220627025?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*



把ξ*tm*(*j*)记做t时刻，状态j,在m元 混合模型情况下产生声学特征Ot的概率，公式如下

![img](https://img-blog.csdn.net/20170217221036172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

对应均值方差，同样可以由Baum-Welch 迭代算出来，公式如下：

![img](https://img-blog.csdn.net/20170217221152594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在实际计算中，一个句子的似然是一串概率相乘，导致概率数值非常低，如.00000001 = 10−8 容易下溢出，而且不方便计算，，所以常常用log来计算，则上述*输出似然函数**b* *j* *(**o**t* *)可以改写如下所示：*

*![img](https://img-blog.csdn.net/20170217221628158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*重写可得：*

*![img](https://img-blog.csdn.net/20170217221720495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*其中C为：*

*![img](https://img-blog.csdn.net/20170217221820722?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

*C为常数，可以再decoding之前提前计算出来，节省计算时间*

以上就是GMM在训练声学模型中的应用。

接下来会写HMM和GMM结合训练声学模型的实践，敬请期待！




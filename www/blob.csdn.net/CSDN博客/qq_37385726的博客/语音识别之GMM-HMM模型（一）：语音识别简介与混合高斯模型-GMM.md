# 语音识别之GMM-HMM模型（一）：语音识别简介与混合高斯模型-GMM - qq_37385726的博客 - CSDN博客





2019年04月11日 20:42:53[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：24
所属专栏：[语音处理](https://blog.csdn.net/column/details/36813.html)









> 
**写在前面**

都知道语音识别有GMM-HMM模型，也分别了解了什么是：

GMM（混合高斯模型） [https://blog.csdn.net/qq_37385726/article/details/89198387](https://blog.csdn.net/qq_37385726/article/details/89198387)

MMC（马尔可夫链） [https://blog.csdn.net/qq_37385726/article/details/89219056](https://blog.csdn.net/qq_37385726/article/details/89219056)

HMM（隐马尔可夫模型） [https://blog.csdn.net/qq_37385726/article/details/89219695](https://blog.csdn.net/qq_37385726/article/details/89219695)

但是却发现不清楚GMM与HMM与语音识别有什么关系，更不知道GMM-HMM模型究竟是什么

好像没有看到有系统讲解很清楚的博客

于是我根据这些零散的学习，整理出了一套比较方便适于理解的系列博客。

由于文章的主要内容均为借鉴，故标为转载。  


### **原始整理为：qq_37385726**

### **转载请注明出处**

> 
**系列博客**
- [语音识别之GMM-HMM模型（一）：语音识别简介与混合高斯模型-GMM](https://blog.csdn.net/qq_37385726/article/details/89219329)
- [语音识别之GMM-HMM模型（二）：隐马尔科夫模型-HMM](https://blog.csdn.net/qq_37385726/article/details/89217990https://blog.csdn.net/qq_37385726/article/details/89217990)
- [语音识别之GMM-HMM模型（三）：GMM-HMM模型应用于语音识别任务原理详解](https://blog.csdn.net/qq_37385726/article/details/89220703)


# **目录**

## [语音识别简介](#%E7%AC%AC%E4%B8%80%E7%AB%A0-%E7%AE%80%E4%BB%8B)

## [混合高斯模型](#%E7%AC%AC%E4%BA%8C%E7%AB%A0-%E6%B7%B7%E5%90%88%E9%AB%98%E6%96%AF%E6%A8%A1%E5%9E%8B)

## [GMM建模声学特征的理解](#GMM%E5%BB%BA%E6%A8%A1%E5%A3%B0%E5%AD%A6%E7%89%B9%E5%BE%81%E7%9A%84%E7%90%86%E8%A7%A3)

# 语音识别简介

自动语音识别(Automatic speech recongnition， ASR)技术时使人与人。人与机器交流的关键技术，它将声学波形转换为人类的文字。

一个语音对话系统通常包括四个主要组成部分的一个或多个，即语音识别系统将语音转化为文本，语义理解系统提取用户说话的语义信息、文字转换系统将内容转化为语音、对话管理系统连接其他三个系统并完成与实际场景的沟通。

![](http://pelhans.com/img/in-post/deepspeech_ch1_ch2/deepspeech_ch1_ch2_2.jpg)语音识别系统主要有图2中的四部分组成，信号处理和特征提取、声学模型（AM）、语言模型（LM）和解码搜索部分。信号处理和特征提取部分以音频信号为输入，通过消除噪声和信道失真对语音进行增强，将信号从时域转化到频域，并为声学模型提取合适的特征向量。声学模型将声学和发音学（phonetics）进行整合，以特征向量作为输入，并为可变长特征序列生成声学模型分数。语言模型学习词与词间的相互关系，来评估序列的可能性。解码搜索对给定特征向量序列和若干假设次序列计算声学模型和语言模型分数，并输出得分最高的结果。

声学模型的两个主要问题为特征向量序列可编程和音频信号的丰富变化性。前者可通过动态时间规整（DTW）或HMM解决。在过去，最流行的语音识别系统采用MFCC或RASTA-PLP作为特征向量，使用GMM-HMM作为声学模型。采用最大似然准则、序列鉴别性训练算法（MCE、MPE）等进行训练。现在流行的是分层鉴别性模型如深度神经网络模型。

![](http://pelhans.com/img/in-post/deepspeech_ch1_ch2/deepspeech_ch1_ch2_1.jpg)

# 混合高斯模型

随机变量可分为离散型随机变量, 连续型随机变量或混合型随机变量.如果连续型随机变量x的概率密度是

![](http://pelhans.com/img/in-post/deepspeech_ch1_ch2/deepspeech_ch2_f1.jpg)

那么它是服从正态分布或高斯分布的.

一个标量连续随机变量x服从混合高斯分布，如果它的概率密度函数为:

![](http://pelhans.com/img/in-post/deepspeech_ch1_ch2/deepspeech_ch2_f2.jpg)

其中:![](http://pelhans.com/img/in-post/deepspeech_ch1_ch2/deepspeech_ch2_f3.jpg)

混合权重和为1，即:

![](http://pelhans.com/img/in-post/deepspeech_ch1_ch2/deepspeech_ch2_f4.jpg)

混合高斯模型可以描述多模态性质的物理数据（如语音数据）。推广到多变量的多元混合高斯分布，其联合概率密度函数可写为：

![](http://pelhans.com/img/in-post/deepspeech_ch1_ch2/deepspeech_ch2_f5.jpg)

在实际计算中，若使用全协方差矩阵（非对角）将引入大量参数（约为MxD^2)，因此可以使用对角协方差矩阵，当M很大时，亦可以限制所有的协方差矩阵为相同矩阵。

对于多元混合高斯分布的参数估计即根据符合混合高斯分布的数据来确定模型参数的取值。此处主要介绍最大值期望算法（EM算法），它可以作为最大似然准侧估计方法的代表。EM算法是在给定确定数量的混合分布成分情况下去估计各个分布参数最通用的方法。

该算法分为两个阶段，E阶段为期望计算阶段，M为最大化阶段，针对高斯混合分布的EM算法参数更新公式为：

![](http://pelhans.com/img/in-post/deepspeech_ch1_ch2/deepspeech_ch2_f6.jpg)

由上可以看出，这些公式本质上是对整个采样数据的加权平均的均值和协方差。



# GMM建模声学特征的理解

**原始的语音数据经过短时傅立叶变换或取倒谱后会成为特征序列，在忽略时序信息的条件下，GMM就非常适合拟合这样的语音特征。**

![](https://img-blog.csdnimg.cn/20190411203548872.png)

> 
**用GMM建模声学特征（Acoustic Feature）O1,O2,...,On，可以理解成：**

**每一个特征是由一个音素确定的，即不同特征可以按音素来聚类。由于在HMM中音素被表示为隐变量（状态），故等价于：每一个特征是由某几个状态确定的，即不同特征可以按状态来聚类。**

**则设P(O|Si)符合正态分布，则根据GMM的知识，O1,O2,...,On实际上就是一个混合高斯模型下的采样值。**


**因此，GMM被整合进HMM中，用来拟合基于状态的输出分布。但若包含语音顺序信息的话，GMM就不再是一个好模型，因为它不包含任何顺序信息。若当给定HMM的一个状态后，若要对属于该状态的语音特征向量的概率分布进行建模，GMM仍不失为一个好的模型。**




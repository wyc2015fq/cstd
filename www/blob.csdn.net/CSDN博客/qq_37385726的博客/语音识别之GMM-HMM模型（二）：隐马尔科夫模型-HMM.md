# 语音识别之GMM-HMM模型（二）：隐马尔科夫模型-HMM - qq_37385726的博客 - CSDN博客





2019年04月11日 20:43:38[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：21
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

## [概述](#%E6%A6%82%E8%BF%B0)

## [马尔科夫模型](#%E9%A9%AC%E5%B0%94%E7%A7%91%E5%A4%AB%E6%A8%A1%E5%9E%8B)

## [隐马尔科夫模型](#%E9%9A%90%E9%A9%AC%E5%B0%94%E7%A7%91%E5%A4%AB%E6%A8%A1%E5%9E%8B)

## [举个例子](#%E4%B8%BE%E4%B8%AA%E4%BE%8B%E5%AD%90)

## [HMM的组成部分](#hmm%E7%9A%84%E7%BB%84%E6%88%90%E9%83%A8%E5%88%86)

## [HMM的三个基本问题](#hmm%E7%9A%84%E4%B8%89%E4%B8%AA%E5%9F%BA%E6%9C%AC%E9%97%AE%E9%A2%98)

## [计算likelihood: 前向算法](#%E8%AE%A1%E7%AE%97likelihood-%E5%89%8D%E5%90%91%E7%AE%97%E6%B3%95)

## [解码问题：Viterbi算法](#%E8%A7%A3%E7%A0%81%E9%97%AE%E9%A2%98viterbi%E7%AE%97%E6%B3%95)

## [HMM的训练:前向后向算法](#HMM%E7%9A%84%E8%AE%AD%E7%BB%83%3A%E5%89%8D%E5%90%91%E5%90%8E%E5%90%91%E7%AE%97%E6%B3%95)

## [HMM的训练问题的应用——HMM状态转移矩阵的确定](#HMM%E7%9A%84%E8%AE%AD%E7%BB%83%E9%97%AE%E9%A2%98%E7%9A%84%E5%BA%94%E7%94%A8%E2%80%94%E2%80%94HMM%E7%8A%B6%E6%80%81%E8%BD%AC%E7%A7%BB%E7%9F%A9%E9%98%B5%E7%9A%84%E7%A1%AE%E5%AE%9A)

## [HMM的解码问题的应用——语音特征序列到状态转移序列](#HMM%E7%9A%84%E7%AC%AC%E4%BA%8C%E7%B1%BB%E9%97%AE%E9%A2%98%E4%BD%BF%E7%94%A8%E7%BB%B4%E7%89%B9%E6%AF%94%E7%AE%97%E6%B3%95%E7%9A%84%E5%BA%94%E7%94%A8)



# 概述

概率图模型是在概率模型的基础上，使用了基于图的方法来表示概率分布，是一种通用化的不确定性知识和处理方法。下图给出常见的图模型：

![](http://pelhans.com/img/in-post/deepspeech_ch3/deepspeech_ch3_1.png)



有关图模型的知识读者可以查看机器学习与模式识别(PRML)一书的第八章。



# 马尔科夫模型

![](https://img-blog.csdnimg.cn/20190411191928933.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)



# 隐马尔科夫模型

![](https://img-blog.csdnimg.cn/20190411192014611.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)



## 举个例子

![](https://img-blog.csdnimg.cn/20190411192047910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)



## HMM的组成部分

![](https://img-blog.csdnimg.cn/20190411192125178.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411192152661.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)



## HMM的三个基本问题

![](https://img-blog.csdnimg.cn/20190411192212496.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)



### 计算likelihood: 前向算法

![](https://img-blog.csdnimg.cn/20190411192249156.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2019041119230033.png)

![](https://img-blog.csdnimg.cn/20190411192313839.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411192402864.png)

### 解码问题：Viterbi算法

![](https://img-blog.csdnimg.cn/20190411192637644.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411192657505.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411192715358.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)



### HMM的训练:前向后向算法

![](https://img-blog.csdnimg.cn/20190411193547222.png)

**期望最大化(Exoectation maximization, EM)算法可以用于含有隐变量的统计模型的参数最大似然估计.**

**它的核心思想是：**
- 初始时随机地给模型的参数赋值,该赋值遵循模型对参数的限制.而后我们将得到模型的参数μ0.
- 根据μ0求得模型中隐变量的期望值.
- 由该期望值又可以重新得到模型参数的新估计值,由此得到模型的新参数μ1.
- 循环进行该过程,知道参数收敛于最大似然估计值.

这里我们介绍**EM算法的一种具体实现方法**,称之为**Baum-Welch算法**或前向后向算法.

![](https://img-blog.csdnimg.cn/20190411193853380.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190411193910425.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2,size_16,color_FFFFFF,t_70)



## HMM的训练问题的应用——HMM状态转移矩阵的确定

确定状态转移矩阵，是执行解码问题的基础。

**而状态转移矩阵的确定即等价于HMM的训练问题（即状态转移矩阵u=max(P(u|O))），从语音特征序列中利用EM算法学习得到状态转移矩阵。**



## HMM的解码问题的应用——语音特征序列到状态转移序列

【该部分参考博文[https://blog.csdn.net/stdcoutzyx/article/details/8522078](https://blog.csdn.net/stdcoutzyx/article/details/8522078)】



说了这么多，HMM到底有什么应用呢？

HMM一开始是在信息论中应用的，后来才被应用到自然语言处理还有其他图像识别等各个方面。**下面举两个例子说明他的应用，一个是输入法的整句解码，一个是语音识别。**有图为证：

![](https://img-my.csdn.net/uploads/201301/20/1358665136_7599.jpg)
- 输入法把拼音看做是观察状态，需要得到的汉字为隐藏状态，这样，输入法的整句解码就变成了维特比解码，其转移概率即是二元语言模型，其输出概率即是多音字对应不同拼音的概率。
- **将上图中的拼音换成语音，就成了语音识别问题，转移概率仍然是二元语言模型，其输出概率则是语音模型，即语音和汉字的对应模型。**










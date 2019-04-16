# Concept Drift（概念漂移） - 一个跳popping的quant的博客 - CSDN博客





2018年08月22日 20:49:02[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：2749








## Introdution

concept drift在机器学习、时间序列以及模式识别领域的一种**现象**。如果是在机器学习领域中，这个概念指的就是一个模型要去预测的一个**目标变量**，概念漂移就是这个目标变量随着**时间**的推移发生改变。概念漂移在很多领域都具有很重要的意义，例如对金融衍生品价格预测、动态控制等。

## Data stream

目前有很多广泛存在的数据，像金融数据、交通数据等，这类数据不同于传统的static data（静态数据），而是作为一种**数据量大**、**实时性强**的data stream（流式数据）而存在。流式数据分为**稳定的数据流**和**动态的数据流**，稳定的数据流具有稳定独立同分布的特点，而动态数据流则是不独立同分布的，所以会产生概念漂移的现象。

## Concept drift

概念漂移可以从很多角度进行划分，其中一种是按照变化的速度来进行划分的。在 [1] 中将概念漂移划分为sudden、incremental、gradual、recurring、blip、noise。

![](https://img-blog.csdn.net/20180822200647347?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

sudden指的是**迅速同时又不可逆**的改变，强调的是发生的迅速。

incremental和gradual都是强调改变发生的**缓慢**，incremental强调值的随时间改变，gradual则是数据分布的改变。也有些研究者将这两种变化划分为同一类，用incremental gradual这个术语来代替。

recurring则是一种temporary（**临时性**）的改变，在一段短时间内会恢复之前的状态。所以也有些研究者将其称为**local drift**，它不具有周期性，是在不规则的时间间隔内反复转换。

blip是代表一种很**稀少**的事件，它可以被视为一种anomaly或者outlier（异常）。

noise是一种**随机**的改变，通常这种数据会从样本数据中filter out。

## Several Drift Detectors

DDM（Drift Detection Method）

EDDM（Early Drift Detection Method）

DDD（Diversity for Dealing with Drifts）  Ensemble with different diversity

Hoeffding trees

Ensemble Learnings





[1]、A Review on Real Time Data Stream Classification and Adapting To Various Concept Drift Scenarios

[2]、DDD: A new ensemble approach for dealing with concept drift




# 谷歌开源GPipe库：帮你训练上亿参数的超大模型 - 知乎
# 



> 晓查 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

BigGan、BERT和GPT-2的出现表明，越大的神经网络模型可以带来越好的性能。其中能写出各种假新闻的GPT-2参数就有15亿个之多。

如果你也在训练一个参数超多的模型，那么现在有个好消息要告诉你。

谷歌在最近公布的**Lingvo**框架下开源了**GPipe**。它让研究人员不用调整超参数，就能用更多加速核心来训练更大的模型和提升性能。

GPipe适用于多个连续层组成的任何DNN，而且训练模型的大小与分区数成正比。

## **参数越多，效果越好**

在计算机视觉图像分类任务上，模型大小和分类准确性之间存在很强的相关性。



![](https://pic1.zhimg.com/v2-d555cbeb9944116703c813920d327dc8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='603' height='442'></svg>)



## **△**不同图像分类模型在ImageNet上的表现，显示出准确度和参数数量的强相关性

从2014年到2017年，ImageNet视觉识别的冠军的准确度从74.8%提升到了82.7%，模型参数增加了36倍。

而在这三年间，GPU显存仅仅增加了3倍，因此迫切需要一种高效的神经网络训练库来解决参数太多的问题。

## **核心越多，参数越多**

GPipe就可以减少参数模型对内存的占用。

谷歌研究人员在8核心64GB内存的云TPUv2进行测试。如果没有GPipe，由于内存的限制，每个加速核心只能训练**8200万**个参数。

使用GPipe后，活动内存从6.26 GB降低到3.46GB，可以在单个加速核心上训练**3.18亿**个参数，接近原来的4倍。

通过GPipe，AmoebaNet能够在云TPUv2上加入18亿个参数，比没有GPipe的情况下多25倍。



![](https://pic1.zhimg.com/v2-8abc44a9737db2aa76c779e9f0af790c_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='623' height='326'></svg>)



## **△** 谷歌将AmoebaNet-D模型分配到四倍数量的加速核心上，实现了3.5倍的加速

GPipe不仅仅速度更快，在训练模型的质量上也有提高。

为了验证这一点，谷歌训练了另一种图像分类算法AmoebaNet-B，它总共有5.57亿个模型参数。

该算法在一些常用的数据集上表现很好，将single-crop ImageNet精度提高到了84.3％，在CIFAR-10上的精度为99％，在CIFAR-100上的精度为91.3％。

## **GPipe为何能实现高效**

原则上我们可以通过并行计算在GPU或者TPU上训练更大的DNN模型。但是由于DNN的顺序性，这种方法可能导致在计算期间只有一个加速器处于活动状态，不能充分利用设备的计算能力。

谷歌在GPipe使用了两种AI训练技术：一种是**同步随机梯度下降**，另一种是**并行pipeline**，将上一步的输出作为流传输到下一步。

Gpipe将模型划分到不同的加速核心，并自动将mini-batch的训练样本分成更小的micro-batch，并在pipeline中运行，使TPU核心能够并行操作。

此外，梯度一直在micro-batch中累积，因此分区数量不会影响模型质量。



![](https://pic4.zhimg.com/v2-f9753fd1a5eef8105ac2554166945673_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='524' height='283'></svg>)



以上都是在云TPUv2上的结果，实际上GPipe在云TPUv3上有更好的性能，每个TPUv3都有16个加速器核心和256 GB（每个加速器16 GB）。用GPipe训练80亿参数的Transformer模型时，能实现11倍的加速。

开源地址：
[https://github.com/tensorflow/lingvo/blob/master/lingvo/core/gpipe.py](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/lingvo/blob/master/lingvo/core/gpipe.py)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤



# CRU+MXnet︱CRU-Net - Collective Residual Networks - 素质云笔记/Recorder... - CSDN博客





2017年03月29日 10:13:13[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1235








paper:[https://arxiv.org/pdf/1703.02180.pdf](https://arxiv.org/pdf/1703.02180.pdf)

github:[https://github.com/cypw/CRU-Net](https://github.com/cypw/CRU-Net)

优化了ResNet，让其更有效率且最终精度得到提升。 

.

## 1、Abstract

剩余单位广泛用于缓解困难建立深层神经网络优化。然而，性能增益不wellcompensate模型尺寸的增加，说明低效率这些剩余单位参数。在这项工作中，我们首先重温剩余单位剩余的功能在变化，表明这些residualfunctions实际上可以用一个统一的框架内解释基于广义块分解。 

然后，根据新的解释，提出了一种新的架构，集体剩余单元（CRU），从而提高深层神经网络参数的效率通过集体tensorfactorization。CRU使知识在不同的残余unitsusing共享的因素。 

实验结果表明，我们提出的CRU networkdemonstrates突出参数的效率，实现comparableclassification性能与resnet-50模型大小resnet-200。 

通过更深层次的网络使用CRU，我们可以实现国家的最先进的单一模型的分类精度imagenet-1k和places365标准benchmarkdatasets 

（来源：[Sharing Residual Units Through Collective Tensor Factorization in Deep Neural Networks](https://www.turingkey.com/CV/8348ac551632ff59ba7191c8696ad789/)）
## 2、评测结果

在两个数据集上有开源的premodel：
- ImageNet-1k
- Places365-Standard

用MxNet实现。

![这里写图片描述](https://img-blog.csdn.net/20170329100943834?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





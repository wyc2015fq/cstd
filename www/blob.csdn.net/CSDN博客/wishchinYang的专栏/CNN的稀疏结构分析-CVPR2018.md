# CNN的稀疏结构分析-CVPR2018 - wishchinYang的专栏 - CSDN博客
2018年07月05日 18:03:40[wishchin](https://me.csdn.net/wishchin)阅读数：2131
文章地址：[CVPR2018高效小网络](https://zhuanlan.zhihu.com/p/37074222)探秘...[CVPR2018高效小网络](https://zhuanlan.zhihu.com/p/37919669)探秘II...
本文介绍、梳理和对比高效小网络，包括早期的经典模型SqueezeNet, MobileNet(V1), 和CVPR 2018最新模型ShuffleNet, IGCV2, MobileNetV2, 探究短小精悍的秘密，学习设计理念和技术，回答以下两个问题：
- 题图是ImageNet上的top-1准确率 vs 理论计算量 vs 模型大小的对比，来自[tensorflow/models](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/models/tree/master/research/slim/nets/mobilenet)，本文要探讨和分析的第一个问题：**MobileNetV2到底优化了什么，能做到比MobileNetV1既好又快？**
- 下图是**ncnn**框架的高端ARM高通820和低端ARM海思3519硬件实测速度对比，来自[Tencent/ncnn](https://link.zhihu.com/?target=https%3A//github.com/Tencent/ncnn/tree/master/benchmark)，本文要探讨和分析的第二个问题：**为什么理论计算很高的SqueezeNet反而非常快？为什么ShuffleNet要比架构技术接近的MobileNetV1和MobileNetV2快那么多？**
![](https://img-blog.csdn.net/20180706102409563?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

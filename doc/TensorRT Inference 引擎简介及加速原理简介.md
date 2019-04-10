#  TensorRT Inference 引擎简介及加速原理简介

2018年03月31日 19:40:39 [正午之阳](https://me.csdn.net/xh_hit) 阅读数：3850



## 1.TensorRT简介

TensorRT是NVIDIA 推出的一款基于CUDA和cudnn的神经网络推断加速引擎，相比于一般的深度学习框架，在CPU或者GPU模式下其可提供10X乃至100X的加速，极大提高了深度学习模型在边缘设备上的推断速度。将TensorRT应用在NVIDIA 的TX1或者TX2上，可实现深度学习网络的时时推荐，且不需在内存较少的嵌入式设备上部署任何深度学习框架。

## 2.TensorRT加速原理

在计算资源并不丰富的嵌入式设备上，TensorRT之所以能加速神经网络的的推断主要得益于两点。首先是TensorRT支持INT8和FP16的计算，通过在减少计算量和保持精度之间达到一个理想的trade-off，达到加速推断的目的。

更为重要的是TensorRT对于网络结构进行了重构和优化，主要体现在一下几个方面。

第一是tensorRT通过解析网络模型将网络中无用的输出层消除以减小计算。

第二是对于网络结构的垂直整合，即将目前主流神经网络的conv、BN、Relu三个层融合为了一个层，例如将图1所示的常见的Inception结构重构为图2所示的网络结构。

第三是对于网络的水平组合，水平组合是指将输入为相同张量和执行相同操作的层融合一起，如图2向图3的转化。

第四是对于concat层，将contact层的输入直接送入下面的操作中，不用单独进行concat后在输入计算，相当于减少了一次传输吞吐。

 

![img](https://img-blog.csdn.net/20180331184819590?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hoX2hpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​                                                                                  图1

![img](https://img-blog.csdn.net/20180331184952413?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hoX2hpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​                                                                                   图2

![img](https://img-blog.csdn.net/20180331185235112?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hoX2hpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​                                                                                    图3

以上3步即是tensorRT对于所部署的深度学习网络的优化和重构，从中我们可以看到根据其优化和重构策略，第一和第二步适用于所有的网络架构，但是第三步则对于Inception结构的神经网络加速效果最为明显。所以想更好地加速的网络推断，我们可以在基础网络中多采用Inception模型，充分发挥tensorRT的优势。

以上部分内容翻译于https://devblogs.nvidia.com/production-deep-learning-nvidia-gpu-inference-engine/

对于如何使用tensorRT进行inference及其细节，会在下一遍博客中讲解。
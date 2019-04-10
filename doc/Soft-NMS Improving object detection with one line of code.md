# Soft-NMS: Improving object detection with one line of code

2018年09月15日 17:37:38 [正午之阳](https://me.csdn.net/xh_hit) 阅读数：129



Improving object detection with one line of code 是ICCV2017的文章，主要是优化解决目标检测后处理中非极大值抑制（NMS，Non Maximum Suppression）的问题。

# NMS：

在解析本文主旨之前，先回顾下当前目标检测算法中必不可少的NMS。当前，无论是两阶段的Faster-RCNN或者one-stage的SSD、YOLO等目标检测模型，最终得到的目标的候选框非常多，然而图片中实际的目标不多，远远小于候选框的数量。然而对于同一目标，可能产生多个候选框，相互之间的重合度很高，因此为了消除多余重复的，得到最终的候选框，降低false positive，NMS被广泛地应用。

其主要思想与步骤如下：

1. 对于同属一类的检测候选框，按照其confidence score进行排序；
2. 选择score最高的检测框，加入最终的检测结果中，然后计算其余各框与其的overlap，如果大于所设阈值，则将对应的框删除，即进行抑制；小于则保留进入下一此迭代选择；
3. 对于剩余的框的重复步骤1和2；

其伪代码如图1所示：

​         ![img](https://img-blog.csdn.net/20180915160637893?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hoX2hpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)           

​    图1  算法流程               

红色即为传统非极大值抑制的算法流程。NMS非常依赖于overlap的预设阈值T， T过大，则起不到抑制作用，产生FP；T过小，则抑制作用太强，miss_rate会提高，产生漏检。目前，一般将其设置为0.3。其次，当两个同类目标临近时，也易造成漏检。如下图2所示，绿色框内的目标会被抑制造成漏检。

![img](https://img-blog.csdn.net/20180915161426691?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hoX2hpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​    图2   NMS缺陷说明 

# Soft-NMS:

基于NMS的上述缺陷，作者提出了Soft-NMS的想法，即当重叠度满足一定阈值后，不是直接将其抑制，而是将其confidence score 降低进入下一次迭代选择（如图1绿色虚框所示）。通过overlap的大小降低候选框的置信度来改善上述缺陷，重叠度越大，score降低越多，因为它们更有可能成为FP。 

因此作者首先设计了如下的score decaying公式：

![img](https://img-blog.csdn.net/20180915170354680?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hoX2hpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上式通过一个线性关系根据overlap的大小对score进行下降和惩罚。然而上式并不是一个连续，因此作者又提出了一种基于高斯函数的rescore function：

![img](https://img-blog.csdn.net/20180915171109777?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hoX2hpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此函数中![\sigma](https://private.codecogs.com/gif.latex?%5Csigma)为预设值，作者尝试了[0.1, 0.3, 0.5, 0.7, 0.9 ,1.1]等值。（higher ![\sigma](https://private.codecogs.com/gif.latex?%5Csigma) better localization）

根据图1绿色虚框所示，Soft-NMS算法的时间复杂度与NMS相同，也是![O(N^{2})](https://private.codecogs.com/gif.latex?O%28N%5E%7B2%7D%29)。但是根据作者大量实验可以得出，Soft-NMS相比于NMS，在同等计算复杂度的情况下，会得到更高的准确率和定位精度，如下图所示。

![img](https://img-blog.csdn.net/20180915173450538?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hoX2hpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

以上为本人对Soft-NMS拙见，欢迎讨论。下篇博客会分享旷世ECCV2018的新作IOU-Net，也是去优化NMS，欢迎阅读。
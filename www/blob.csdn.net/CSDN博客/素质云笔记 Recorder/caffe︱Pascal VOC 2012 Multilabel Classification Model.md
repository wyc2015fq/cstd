# caffe︱Pascal VOC 2012 Multilabel Classification Model - 素质云笔记/Recorder... - CSDN博客





2017年03月03日 16:04:41[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2285








看到Model Zoo里面有这么一个项目：Pascal VOC 2012 Multilabel Classification Model

> 
This model has been used for the paper “Analyzing Classifiers: Fisher 

  Vectors and Deep Neural Networks” 

  ([http://www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Bach_Analyzing_Classifiers_Fisher_CVPR_2016_paper.pdf](http://www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Bach_Analyzing_Classifiers_Fisher_CVPR_2016_paper.pdf)), 

  published in the proceedings of CVPR 2016. Kindly note, that it has 

  been trained in a multilabel setting with a multilabel-compatible loss 

  function. It should not be used in conjunction with a softmax layer In 

  particular $f_{i}(x)>0$ denotes presence of class i and multiple 

  classes can be predicted in one image.
Downloading the Model: 
[caffemodel](http://heatmapping.org/files/bvlc_model_zoo/pascal_voc_2012_multilabel/pascalvoc2012_train_simple2_iter_30000.caffemodel)
[prototxt](http://heatmapping.org/files/bvlc_model_zoo/pascal_voc_2012_multilabel/deploy_x30.prototxt)
Please reference the above submission when using the model via

@inproceedings{lapuschkinCVPR16, 

      title={Analyzing classifiers: Fisher vectors and deep neural networks}, 

      author={Lapuschkin, S. and Binder, A. and Montavon, G. and M{\”u}ller, K.-R. and Samek, W.}, 

      booktitle={CVPR}, 

      pages={2912-2920}, 

      year={2016} }
我以为里面的模型是Fisher Vector的model，没想到是Pascal VOC 

  2012分类任务的.caffemodel，抱着草泥马的态度，就随便试了试，感觉还可以~ 

  .
# 一、Pascal VOC 2012

## 1、Pascal VOC 2012介绍

PASCAL VOC为图像识别和分类提供了一整套标准化的优秀的数据集，从2005年到2012年每年都会举行一场图像识别challenge。 

本文主要分析PASCAL VOC数据集中和图像中物体识别相关的内容。 

下载地址为：点击[打开链接](http://host.robots.ox.ac.uk/pascal/VOC/voc2012/VOCtrainval_11-May-2012.tar)。 
![这里写图片描述](https://img-blog.csdn.net/20170303155454321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

几个文件夹里面： 

JPEGImages：JPEGImages文件夹中包含了PASCAL VOC所提供的所有的图片信息，包括了训练图片和测试图片。 

Annotations：Annotations文件夹中存放的是xml格式的标签文件，每一个xml文件都对应于JPEGImages文件夹中的一张图片。 

ImageSets：ImageSets存放的是每一种类型的challenge对应的图像数据。Action下存放的是人的动作（例如running、jumping等等，这也是VOC challenge的一部分）。Layout下存放的是具有人体部位的数据（人的head、hand、feet等等，这也是VOC challenge的一部分）Main下存放的是图像物体识别的数据，总共分为20类。Segmentation下存放的是可用于分割的数据。
主要参考博客：[http://blog.csdn.net/zhangjunbob/article/details/52769381](http://blog.csdn.net/zhangjunbob/article/details/52769381)

## 2、输出结果

关于.caffemodel最后输出的是20分类： 

人物: person 

动物: bird, cat, cow, dog, horse, sheep 

交通工具: aeroplane, bicycle, boat, bus, car, motorbike, train 

室内: bottle, chair, dining table, potted plant, sofa, tv/monitor 

.
# 二、关于Fisher Vector

Fisher Vector的本质就是对于高斯分布-的变量求偏导！也就是对权重，均值，标准差求编导得到的结果。

## 1、为什么Fisher Vector比高斯分布有效？

我们将一张图近似为一个高斯分布，由这个高斯分布来表示这张图像。假设我们是做目标的检测，那么当你得到一个有相同的高斯分布的图的时候你就可以判断出这就是那个目标了。但实际的情况是却不一定是这样的，我们看一张图： 
![这里写图片描述](https://img-blog.csdn.net/20170303155920932?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这两张图上特征点的分布在黑色的区域，二者的分布却可以一样（当然我画的不是很好）！ 

由此，我们知道，在高斯分布的基础上我们再找到变化的方向，我们便可以更加准确的表示这一张图！
## 2、微信团队使用FV

参考于：[微信图像开放平台：让你的应用看懂世界](http://djt.qq.com/article/view/1111)

在图像识别云平台中，微信图像技术组采用了基于全局特征的图像检索方法，通过SIFT+Fisher  Vector得到一幅图像的全局描述子，然后通过量化，将全局描述子量化为低比特的码流，每幅图像对应一个图像识别指纹，在微信图像云平台服务中，已无需构建倒排表，开发者增、删图像时，后台只需在数据库对应的增加、删除指纹即可。对开发者的增删操作可以做出实时的响应。识别过程时，只需比对开发者数据库中的指纹即可。 

       在识别效果上，通过对多个数据集测试，检索效果上均与基于倒排表结构的图像检索技术相当，甚至某些数据集上优于基于倒排表结构的技术。 

微信图像技术组与北京大学、新加坡南洋理工大学有密切合作，走在图像检索技术最前沿，努力为用户提供最优秀的图像识别技术。
## 3、fisher vector实现

博客地址：[Image Fisher Vectors In Python](https://jacobgil.github.io/machinelearning/fisher-vectors-python)

github地址：[https://github.com/jacobgil/pyfishervector](https://github.com/jacobgil/pyfishervector)

相关FV参考博文： 
[GMM、fisher vector、SIFT与HOG特征资料](http://yongyuan.name/blog/sometihing-about-GMM-fisher-vector-SIFT-and-HOG.html)
[Fisher Vector(2)](https://bucktoothsir.github.io/blog/2014/11/27/10-theblog/)
[Fisher Vector 通俗学习](http://blog.csdn.net/ikerpeng/article/details/41644197)
## 延伸一：Facebook AI 实验室开源图像搜索工具Faiss

Faiss 包含了几种用于相似性搜索的方法。它假定示例可以被表示为向量，以及可以通过整数识别，并且这些向量可以与 L2 位距或点积进行比较。与一个查询向量（query vector）相似的向量是具有最低 L2 位距或最高点积的查询向量。Faiss 还支持余弦相似性（cosine similarity），因为这是在标准化向量上的点积。

大多数方法，例如基于二元向量和紧凑量化代码的方法，仅使用向量的压缩表征，并不需要保留原始向量。这通常会导致搜索的准确度降低，但是这些方法可以再单个服务器上的主存储器中扩展到数十亿个向量。

GPU 上的实现可以接受来自 CPU 或 GPU 存储器的输入。在装有 GPU 的服务器上，GPU 索引可以被用作 CPU 索引的插入替换（例如，使用 GpuIndexFlatL2 替换 IndexFlatL2），并且可以自动处理发往/来自 GPU 存储器的副本。

这个库基本上是用 C++ 实现的，带有可选的通过 CUDA 提供的 GPU 支持，以及一个可选的 Python 接口。使用 Makefile 进行编译。详细信息可参见INSTALL：[https://github.com/facebookresearch/faiss/blob/master/INSTALL](https://github.com/facebookresearch/faiss/blob/master/INSTALL)

论文地址：[https://arxiv.org/pdf/1702.08734.pdf](https://arxiv.org/pdf/1702.08734.pdf)

开源地址：[https://github.com/facebookresearch/faiss](https://github.com/facebookresearch/faiss)

信息来源：[新智元公众号：10 亿图片仅需 17.7微秒：Facebook AI 实验室开源图像搜索工具Faiss](http://mp.weixin.qq.com/s?__biz=MzI3MTA0MTk1MA==&mid=2651994172&idx=3&sn=398d1f45693cb396d2baf44f1facea9b&chksm=f1214ccdc656c5db38db1d48e6668e9286193f4260a9727af45d6d2714c5148817af81d6a46b&mpshare=1&scene=1&srcid=0303UHEhayMOl6NRT1iBklou#rd)














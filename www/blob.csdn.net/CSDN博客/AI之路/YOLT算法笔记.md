# YOLT算法笔记 - AI之路 - CSDN博客





2018年08月10日 08:39:32[AI之路](https://me.csdn.net/u014380165)阅读数：2621
个人分类：[深度学习																[计算机视觉																[目标检测-object detection](https://blog.csdn.net/u014380165/article/category/6967389)




论文：You Only Look Twice: Rapid Multi-Scale Object Detection In Satellite Imagery

论文链接：[https://arxiv.org/abs/1805.09512](https://arxiv.org/abs/1805.09512)

代码地址：hps://github.com/CosmiQ/yolt
**这篇文章是做卫星图的目标检测，在YOLO v2算法基础上做了改进得到YOLT（You Only Look Twice）算法**，因为卫星图的目标检测和常见的目标检测场景最大的区别在于卫星图像本身尺寸很大（比如16000*16000），其次目标尺寸非常小而且常常聚集在一起。因此YOLT算法整体上是解决卫星图这种特殊场景的目标检测，对于通用目标检测算法中小目标的检测有一定的借鉴意义，同时这篇文章也列举了一些在实际项目中对效果提升有所帮助的点，也值得借鉴。另外，在卫星图中，用ground sample distance（GSD）表示分辨率，比如常见的卫星图像是30cm GSD。

**卫星图的目标检测主要的几个难点和这篇文章的解决方案在Figure3中已经详细列出了，左边一列是难点，右边一列是对应的解决方案**。主要包括：
**一、卫星图目标的尺寸、方向多样**，因为卫星图是从空中拍摄的，所以角度不固定，像船、汽车的方向都可能和常规目标检测算法中的差别较大，因此检测难度大。解决方案是对数据做尺度变换、旋转等数据增强操作。
**二、小目标的检测难度大**。解决方案包括：1、修改网络结构，使得从输入图像到最后特征的stride等于16，而不是YOLO v2中的32，如果stride等于32，那么输入图像中的32*32区域在最后输出特征中等价于一个点，假如一个目标在输入图像中的像素区域小于32*32，那么就难以检测出来，修改成stride等于16后，就有利于检测出大小在32*32到16*16的目标。2、沿用YOLO v2中的passthrough layer，融合不同尺度的特征（52*52和26*26大小的特征），这种特征融合做法在目前大部分通用目标检测算法中被用来提升对小目标的检测效果。3、不同scale的检测模型融合，主要是因为待检测目标的尺度差异较大，比如飞机和机场，因此采用不同尺度的输入训练检测模型，通过融合检测结果得到最终输出。
**三、卫星图尺寸太大**，超过一般检测模型的最大输入尺寸要求，而一般采用对输入图像进行降采样后再检测的算法效果不好，如图Figure2所示。解决方案包括：1、将原输入图像切块，然后分别输入模型进行检测。2、不同scale的检测模型融合。
![这里写图片描述](https://img-blog.csdn.net/2018081008354797?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Figure2是关于用两种不同类型图像作为输入时模型的预测结果对比，模型是基于标准的YOLO v2进行训练的。左边是直接将原图缩小到416*416大小，结果模型一个汽车目标都没有检测出来。右边是从原图中裁剪出416*416大小的区域然后作为模型的输入，结果模型可以将部分汽车目标检测出来，但是效果依然不算很好。从这个实验可以看出直接将原图缩小并作为模型的输入是不合适的，这也是后面再训练和测试阶段都采用裁剪方式的原因，同时也能增加训练数据量。同时因为右图的检测效果不好很大程度上是因为目标的像素点少于32这个界限，所以后续对网络结构做了修改，提升了模型对小目标的检测效果。
![这里写图片描述](https://img-blog.csdn.net/20180810083605830?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table1是关于YOLT算法的网络结构示意图，网络结构与YOLO v2相比比较大的修改是最后输出特征尺寸只到26*26（针对416*416的输入图像而言，因此stride等于16，YOLO v2等目标检测算法中stride一般等于16），这样就能有效提高对小目标的检测效果。
![这里写图片描述](https://img-blog.csdn.net/20180810083619711?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Figure4是关于测试模型时对输入图像的处理，最上面的是原始的输入图像，前面提到过卫星图的原始尺寸很大，难以直接作为模型的输入，因此这里通过滑窗方式裁剪指定尺寸（比如416*416）的图像作为模型的输入（文中将裁剪后的图像称为chip），而且相邻chip会有15%区域的重叠，如图Figure4所示，目的是保证原图的每个区域都能被完整检测到，虽然这样有可能带来一些重复检测，但可以通过NMS算法去除。因此一张卫星图会裁剪出成百上千的指定尺寸图像，这些图像经过检测后将检测结果合并在一起就得到最终的检测结果。
![这里写图片描述](https://img-blog.csdn.net/20180810083635490?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Figure5是关于训练数据的整体情况，一共检测5个类别：飞机、船、建筑物、汽车、机场。对训练数据的处理和测试数据类似，也是采用从原图中裁剪出chip进行训练。
![这里写图片描述](https://img-blog.csdn.net/201808100837148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
本来作者是训练一个模型检测上述的5种目标，但是在实验过程中发现不同尺度的目标容易产生误检，这才有了这篇文章的基于不同尺度图像训练两个检测模型，通过融合不同模型的结果达到最优效果。Figure6就是关于机场目标的误检实例，因为机场的训练数据非常少，所以直观的想法是通过增加机场的数据提高机场的检测效果，即便没有办法增加数据，还可以通过一定的后处理逻辑将部分误检过滤掉（比如尺寸非常小的机场预测框），当然这篇文章采用的是另外一种方法：融合不同尺度图像的检测模型结果。这里针对机场目标和其他目标分别训练了一个检测模型，这两个检测模型的输入图像尺度也不一样，测试图像时同理，最后将不同检测模型、不同chip的检测结果合并在一起就得到最终一张卫星图像的输出。
![这里写图片描述](https://img-blog.csdn.net/20180810083724788?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实验结果：**

Figure7是一张关于汽车目标的检测结果，检测效果还是非常不错的，在1秒内能够得到检测结果，同时F1值达到0.95，直观上可以和前面的Figure2做对比。
![这里写图片描述](https://img-blog.csdn.net/20180810083746490?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在这篇文章中作者还对不同分辨率的输入做了实验，Figure10是对原始分辨率（左上角的0.15m表示GSD是0.15m）做不同程度放大后得到的低分辨率图像，这些图像都将用来训练模型，后面有对应的实验分析模型对于不同分辨率图像的检测效果。
![这里写图片描述](https://img-blog.csdn.net/20180810083818613?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Figure13是关于不同分辨率输入下检测模型的F1值，上面的横坐标是目标的像素尺寸。可以看出随着分辨率的降低，图像中目标的像素尺寸也越来越小，检测效果（F1值）也越来越低。不过即便目标只有5个像素点，依然有很好的检测效果。
![这里写图片描述](https://img-blog.csdn.net/20180810083842368?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Figure12是对应不同分辨率图像的检测效果直观对比，左图是15cm GSD的结果，右图是90cm GSD的结果。
![这里写图片描述](https://img-blog.csdn.net/20180810083901943?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table3是YOLT算法对不同目标的效果和速度情况。
![这里写图片描述](https://img-blog.csdn.net/20180810083917555?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)](https://blog.csdn.net/u014380165/article/category/6829230)](https://blog.csdn.net/u014380165/article/category/6829229)






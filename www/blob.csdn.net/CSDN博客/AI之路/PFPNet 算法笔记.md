# PFPNet 算法笔记 - AI之路 - CSDN博客





2018年09月06日 21:37:00[AI之路](https://me.csdn.net/u014380165)阅读数：1971








论文：Parallel Feature Pyramid Network for Object Detection 

论文链接：[http://openaccess.thecvf.com/content_ECCV_2018/html/Seung-Wook_Kim_Parallel_Feature_Pyramid_ECCV_2018_paper.html](http://openaccess.thecvf.com/content_ECCV_2018/html/Seung-Wook_Kim_Parallel_Feature_Pyramid_ECCV_2018_paper.html)

**这篇ECCV2018的文章借鉴了SPP的思想并通过MSCA（multi-scale context aggregation）模块进行特征融合从而提出PFPNet（Parallel Feature Pyramid Network）算法来提升目标检测的效果。PFPNet整体上还是one stage类型的目标检测算法，思想借鉴了SSD，速度上有保证，效果上因为引入SPP思想构造特征金字塔，相当于加宽了网络，同时MSCA模块做类似FPN的特征融合操作，最后基于多层融合特征进行预测，因此效果也是很不错的，和CVPR2018的RefineDet效果基本上差不多。**

**文中有个总结说得特别好，那就是对于目标检测任务而言，真正有用的特征一定具备这3个特点：1、特征要包含目标的细节信息。2、特征要通过足够深的网络提取得到。3、特征要包含目标的语义信息。这个总结其实可以在最近的许多目标检测算法优化中看到，最显而易见的就是高层特征和浅层特征的融合，这个过程其实就包含了这3点内容。**

**Figure1是目标检测算法在网络结构方面的优化过程。**

(a)中上面的结构是直接通过一个网络结构得到特征，并基于该特征进行预测。下面的结构则引入了特征金字塔，基于多个特征层进行预测，这个也是我们比较熟悉的SSD算法的一个思想。 

(b)中的2个网络结构主要是引入了top-down结构和融合操作，也就是从高层到浅层的特征融合过程，**这样预测层既有浅层的目标的位置信息（有利于定位）又有高层的目标的语义信息（有利于目标的识别），因此能有效提高目标的检测效果（尤其是小目标）**。这2个网络结构的差别仅在于前者是基于单层进行预测，后者是基于多层进行预测，后者也就是我们比较熟悉的FPN算法的一个思想。 

(c)是SPP的思想。简单讲就是不同size的池化操作得到不同尺度的特征图，将不同尺度的特征图做融合后进行预测。 

(d)是PFPNet的结构示意图。首先也是通过SPP得到不同尺度的特征图，然后基于这些特征图通过MSCA模块得到融合后的特征，最后基于融合后的多层特征做预测。 
![这里写图片描述](https://img-blog.csdn.net/20180906213531870?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Figure3是PFPNet的整体结构图**。首先对于一张输入图像，先通过一个基础网络提取特征，假设提取到的特征通道数是D，基于提取到的特征通过SPP网络得到不同尺度的特征图（对应（b）操作），用N表示尺度数量（图中N=3），得到的特征图通道数用CH表示，CH等于D；然后通过一个通道缩减操作将（b）中得到的特征图的通道缩减（对应（c）操作，也就是Bottleneck），缩减后的通道数用CL表示，公式是CL=D/(N-1)，但是，按照这个公式来看的话，Figure3中（c）的输出特征通道数应该是2，而不是图中显示的1，不知道图是否简化了；然后通过MSCA操作得到融合后的特征图（对应（d）操作），得到的特征通道数是Cp；最后基于融合后的多个特征图进行预测。 
![这里写图片描述](https://img-blog.csdn.net/20180906213601158?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Figure4是MSCA模块的结构示意图**。MSCA主要是基于特征通道的concat操作，但是输入特征有些不同。**比如Figure4在得到P1这个融合特征时，是用fH(1)、下采样后的fL(0)、上采样后的fL(2)进行concat后得到的**。为什么不是用fL(1)而是用fH(1)，因为在作者看来相同尺度的特征信息要足够多，而FH部分得到的特征是未经过通道缩减的，因此在得到某一个尺度的预测层特征时，被融合的对应尺度特征都是采用FH部分的输出特征，而不是FL部分的输出特征。相比之下，不同尺度的待融合特征采用FL部分的输出特征，相当于补充信息。因此concat后的特征通道数中有(N-1)*(D/(N-1))=D个是FL的输出，有D（等于CH）个是FH的输出，相当于各占一半，这也是前面介绍的为什么FL的输出特征通道数用D/(N-1)这个公式的原因。这部分的实验没有展示出来，所以对于用fH的特征做融合对效果提升有多大就不得而知了。总的来说MSCA模块就是一个特征融合模块，作者期望通过concat操作最大程度地保存提取到的特征信息。 
![这里写图片描述](https://img-blog.csdn.net/20180906213623800?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实验结果：**

Table3是在VOC数据集上的测试结果。这里需要说明的是PFPNet-S300表示anchor的初始化和SSD算法一样，PFPNet-R320表示初始anchor采用RefineDet算法的ARM模块得到的refine后的anhor来初始化。其实PFPNet-S300和PFPNet-S512的效果基本上和RefineDet的效果差不多，同时速度也差不多。 
![这里写图片描述](https://img-blog.csdn.net/20180906213636177?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table4是在COCO数据集上的测试结果。 
![这里写图片描述](https://img-blog.csdn.net/20180906213647485?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)








# DetNet 算法笔记 - AI之路 - CSDN博客





2018年08月11日 07:24:29[AI之路](https://me.csdn.net/u014380165)阅读数：2759








论文：DetNet: A Backbone network for Object 

链接：[https://arxiv.org/abs/1804.06215](https://arxiv.org/abs/1804.06215)

**这篇是ECCV2018关于目标检测的文章，主要是对检测算法的特征提取网络（backbone）做优化**。我们知道目前大部分的目标检测算法在训练时都会用预训练的分类模型来提取特征，这些预训练模型是在ImageNet数据集上训练得到的，众多的实验也证明了这种做法的有效性。**而DetNet这篇文章相当于研究更加有效的特征提取网络，出发点也非常直接，主要包含两点：1、分类任务和检测任务还是有一定差别的，因此用分类数据上训练的分类模型来提取特征用于检测任务不一定合适，比如检测任务比较关注目标的尺度特征，但是分类任务就不一定了。2、检测任务不仅仅要做目标的分类，而且要做目标的定位，这样的差异容易导致一些问题，比如在分类网络中常用的降采样操作可能对分类有效，因为增大了感受野，但是对于需要定位目标的检测任务而言就不一定有利，因为丢失了目标的位置信息**。因此DetNet的提出主要也是针对这两个出发点，换句话说是设计了一个专门用于目标检测算法的特征提取网络，**主要改进点包括：1、增加网络高层输出特征的分辨率，换句话说就是高层不对特征图做尺寸缩减。2、引入dilated卷积层增加网络高层的感受野，这是因为第一个改进点引起的感受野减小。3、减小网络高层的宽度，减少因增大分辨率带来的计算量。**

**这篇文章的实验非常丰富，同时在大、小尺寸目标在不同IOU阈值下的指标分析也比较到位，这是这篇文章吸引我的点，在最后的实验结果部分我也写了一些自己的理解。**

**Figure1是关于几种特征提取网络（backbone）的对比。**
**A中FPN**在分类网络的基础上增加了不同层的融合操作，最终的预测层甚至包含stride等于64的P6层，也就是输出特征维度是输入图像的1/64，这么小的特征图对于目标的回归而言其实不是很有利，因为高层主要负责检测大尺寸目标，所以容易导致大尺寸目标的坐标回归不准。另外，虽然FPN通过特征融合的方式将高层特征和浅层特征进行融合可以提高浅层检测小尺寸目标的效果，但是由于高层这种大stride的原因，小尺寸目标的语义特征在高层丢失也比较多，因此即便融合也会对最后的结果有不利的影响。 
**B中分类网络**一般最终的stride是32，也就是最终输出特征的尺寸是输入图像的1/32，比如在分类模型中常见的输入大小为224*224的图像，输出特征大小是7*7。 
**C中DetNet的backbone**并没有对输入图像做过多的降采样，最终的stride保持为16，这样相当于增加了最终输出特征图的尺寸（或者叫分辨率，spatial resolution）。DetNet整体上还是沿用了FPN的特征融合方式（这部分在Figure1C中没有体现出来），只不过对高层的stride做了修改，尽可能减少了高层的小尺寸目标语义特征的丢失。 
![这里写图片描述](https://img-blog.csdn.net/20180811072150414?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**但是如果网络高层的特征不做像分类网络那样多的降采样（将stride等于32修改为stride等于16）会带来两个问题：1、增加计算量。这个很容易理解，毕竟特征图比之前的大，计算量的增加不可避免。2、高层的感受野（receptive field）减小。感受野和信息丢失类似跷跷板，既然前面选择了尽可能减少高层的特征信息丢失，那么感受野减小也是情理之中。**

那么怎么解决这两个问题呢？

**针对问题1**，主要是降低了网络高层的宽度，这个在Figure2D中展示得比较清楚了，高层的几个stage的每个block的输入特征通道都是256。而常见的分类算法中，比如ResNet越往高层的stage，特征通道数往往越大。 
**针对问题2**，主要引入dilated卷积层来增大感受野，如Figure2的A和B所示，通过对比ResNet网络的residual block（Figure2C）可以看出主要是替换了传统的3*3卷积为dilated卷积层。因此Figure2中的A和B是DetNet网络中的基础结构（Figure2D所示）。 
![这里写图片描述](https://img-blog.csdn.net/20180811072220945?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实验结果：**
**Table1是关于DetNet-59和ResNet网络在分类任务和检测任务上的效果对比**，可以看出在检测任务上的效果还是很明显的。 
![这里写图片描述](https://img-blog.csdn.net/20180811072238459?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Table3是关于不同尺寸目标的AP（Average Precision）对比**。前面提到过高层特征的分辨率大小对于大尺寸目标回归影响较大，高层特征分辨率越高，显然目标的位置信息保留得也越多，而高层主要用来检测大尺寸目标，因此大尺寸目标的检测效果就容易受高层特征的分辨率大小的影响。**在Table3中，可以看出DetNet对大尺寸目标的回归效果提升明显（AP85时，从34.5提升到40.0，IOU阈值越高的指标对比，越能说明目标的回归效果，因为坐标回归要更加准确才算检测正确；而IOU阈值越低的指标对比，越能说明目标的分类效果）**。 
![这里写图片描述](https://img-blog.csdn.net/20180811072315111?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Table4是关于不同尺寸目标的AR（Average Recall）对比**。AR指标在目标检测算法中用得不多，主要用来评价正样本的召回率。前面提到过即便FPN网络的特征融合方式能提高浅层对小尺寸目标的检测效果，但是由于高层传回来特征中小尺寸目标特征丢失比较多，因此效果还有提升空间。**Table4中DetNet-59对小尺寸目标的检测效果提升也是比较明显的（AR50时，从60.0提升到66.4）。需要注意的是相比之下在AR85时提升效果不明显（18.7到19.6）是因为对于小尺寸目标坐标回归最有利的浅层特征在这两个对比模型中都使用了，因此虽然在DetNet-59中增大了高层特征的分辨率，主要提升点还是在于保留了原本丢失的小尺寸目标特征，所以提升比较明显的是目标的分类（AR50）而不是回归（AR85）**。因此在Table4中大尺寸目标在AR50时的提升不如在AR85时的提升明显也就容易理解了，因为对于大尺寸目标的分类而言，2个模型基本上都利用了有效的特征。 
![这里写图片描述](https://img-blog.csdn.net/2018081107234564?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Table6这个实验也非常有意思。既然dilated卷积层在DetNet中有效，那能否将预训练的ResNet网络中的部分卷积层用dilated卷积层代替，然后在fine tune检测网络时训练这些dilated卷积层？于是就有了Table6这个实验。如果和Table1中的ResNet-50对比会发现mAP提升也比较明显，虽然代价是FLOPs增加也比较多。但是和DetNet-59的效果还有一点差距，虽然作者在文中说明这种差距是由于直接训练backbone（DetNet-59）和在fine tune检测网络时训练backbone的部分层（ResNet-50-dilated）差异造成的，但个人认为和前面提到的增加网络高层特征分辨率分不开。**
![这里写图片描述](https://img-blog.csdn.net/20180811072404737?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table7是几种目标检测算法在COCO数据集上的效果对比。 
![这里写图片描述](https://img-blog.csdn.net/2018081107241954?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)










# IncepText 算法笔记 - AI之路 - CSDN博客





2018年08月15日 08:27:11[AI之路](https://me.csdn.net/u014380165)阅读数：1255








论文：IncepText: A New Inception-Text Module with Deformable PSROI Pooling for Multi-Oriented Scene Text Detection 

论文链接：[https://arxiv.org/abs/1805.01167](https://arxiv.org/abs/1805.01167)

**这篇是IJCAI2018关于自然场景文字检测的文章，基于分割算法（FCIS）并针对自然场景下文字的特点设计网络。因为自然场景下的文字大小、宽高比、方向变化多样，所以这篇文章借鉴inception结构的思想进行改进，在inception结构中通过不同尺寸的卷积核设计达到检测不同大小和宽高比的文字，同时引入deformable卷积层操作和deformable PSROI pooling层提升任意方向文字的检测效果。**

**Figure1是IncepText网络结构的示意图**，主网络采用ResNet50。首先，去掉了Res stage5降采样操作，目的是增加该部分输出特征图的分辨率，这种操作在分割算法中比较常见。其次为了提高小文本区域的检测效果，IncepText将深层特征和浅层做了融合，具体而言是Res stage5、Res stage4的特征分别和Res stage3的特征做融合，因为Res stage5修改成不改变输入特征维度，所以只需要将二者unsample成原来维度的两倍就可以做特征融合了。作者用deformable PSROI pooling替换原来的标准PSROI pooling，因为标准的PSROI pooling在处理非水平方向的文本检测时效果不佳。 
![这里写图片描述](https://img-blog.csdn.net/20180815082449794?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Figure2是Inception-Text模块的示意图**，主要是借鉴了GoogleNet的Inception思想，采用不同尺寸卷积核组成的多个分支，能够有效检测不同大小和宽高对比度的文本，而且用1*n和n*1的卷积叠加代替n*n卷积，可以减少一定的计算量。另外在几条支路的最后添加了deformable卷积层，可以有效检测多方向的文本，这部分和后面的deformable PSROI pooling层作用类似。 
![这里写图片描述](https://img-blog.csdn.net/20180815082512528?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
deformable PSROI pooling如公式1所示，和PSROI pooling的差别在于增加了对偏移量△x和△y的预测。 
![这里写图片描述](https://img-blog.csdn.net/20180815082525356?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**deformable PSROI pooling的效果如Figure4所示，可以直观看到对非水平方向的目标检测效果。那么为什么deoformable PSROI pooling能够检测非水平方向的文字呢**？我们知道deformable操作主要是学习了偏置，这个偏置使得提取ROI特征时每个点的权重发生变化，又因为这个偏置的学习和整个网络的训练目标挂钩，所以提取ROI特征时会更关注那些对整体训练目标有用的特征，也就是Figure4中红色框聚集的区域（文字区域），这样检测就不容易受检测目标的方向限制。 
![这里写图片描述](https://img-blog.csdn.net/20180815082544679?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
损失函数方面和FCIS一样，如下公式2所示。其中Lrcls和Lrbox是RPN网络关于框的分类和回归损失，Lcls和Lbox是Fast RCNN网络关于框的分类和回归损失，Lmask是分割任务的损失函数。 
![这里写图片描述](https://img-blog.csdn.net/20180815082556994?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实验结果：**

Table1是关于Inception-Text模块和deformable PSROI pooling的有效性实验对比，这两个改进也是这篇文章的主要创新点。 
![这里写图片描述](https://img-blog.csdn.net/20180815082615260?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Figure7的这个实验非常有意思，也非常重要，有效地证明了inception结构对检测不同大小文本的有效性**。这个实验是在测试阶段通过修改网络结构来观察测试结果，其中（a）是不修改网络结构时的输出，此时3行文本都能以很高的置信度被检测出来。（b）是去掉了Figure2中左边支路得到的网络，也就是去掉了1*1的卷积层分支，因此对于小文本的检测就弱了很多，在Figure7（b）中最下面本文的检测置信度降低明显。（c）是去掉Figure2中中间支路得到的网络，也就是去掉1*3和3*1的卷积层分支，因此对中等大小的文本检测就弱了很多，对应Figure7（c）中最上面文本的检测置信度降低明显。（d）是去掉Figure2中右边支路得到的网络，也就是去掉1*5和5*1的卷积层分支，因此对大尺寸文本的检测就弱了很多，对应Figure7（d）中最上面文本没有检测出来。 
![这里写图片描述](https://img-blog.csdn.net/20180815082632786?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table2是在ICDAR2015数据集上的结果。 
![这里写图片描述](https://img-blog.csdn.net/20180815082647354?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table4是在MSRA-TD500数据集上的结果。 
![这里写图片描述](https://img-blog.csdn.net/2018081508270165?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)












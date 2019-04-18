# Mask TextSpotter 算法笔记 - AI之路 - CSDN博客





2018年08月13日 22:18:28[AI之路](https://me.csdn.net/u014380165)阅读数：1596








论文：Mask TextSpotter: An End-to-End Trainable Neural Network for Spotting Text with Arbitrary Shapes 

论文链接：[https://arxiv.org/abs/1807.02242](https://arxiv.org/abs/1807.02242)

**这篇是白翔老师发表在ECCV2018关于OCR的文章，通过引入分割的思想进行end-to-end训练文本 检测和识别从而达到检测和识别任意形状文本的目的。**

**文章的主要特点在于能够检测任意形状的文本**，之所以能达到这样的效果主要是因为采用分割方式检测文本。我们知道目前OCR领域对水平文本的检测和识别效果都非常好，同时对任意方向的文本检测和识别也有很多优秀的算法，但是对任意形状的文本检测和识别的研究却非常少，主要一个原因在于常规的文本检测算法采用目标检测算法，这种类型算法不利于任意形状文本的检测，而分割算法恰恰能实现，这篇文章正是借鉴Mask RCNN的思想进行文本检测和识别。

另外目前OCR领域的大部分算法中文本检测和文本识别都是分开的，也就是文本检测通过一个模型实现，得到的检测结果再作为文本识别模型的输入得到最终的识别结果。分开训练的方式比较主流，而end-to-end的训练方式有一定难度，但是文本检测和文本识别相互之间联系紧密而且可以互补，因此从直观上看分开训练模型不一定是最好的，这也是目前该领域依然研究end-to-end的训练方式的一个原因。**这篇文章是end-to-end的训练方式**，也就是文本检测和文本识别在同一个模型中同时实现，通过分割进行文本检测能得到可靠的检测结果，从而保证文本识别的训练能够顺利进行。

如果你不熟悉水平方向、任意方向和任意形状的文本检测和识别，那么可以看Figure1。在Figure1中，绿色框是文本检测的结果，左图的检测框和常规的目标检测算法结果类似，主要通过预测框的中心点和宽高来定位框；中间图的检测框则是通过预测框的四个点来定位框，这样就能实现任意四边形的框预测；右边图的检测结果是这篇文章的检查结果，也就是通过分割的方式能检测任意形状的文本。 
![这里写图片描述](https://img-blog.csdn.net/2018081322144018?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Mask TextSpotter在整体结构上和Mask RCNN类似，如Figure2所示**，主要包含的4个部分是：1、用于特征提取的backbone采用FPN网络，主要原因在于通过特征融合能提升对小字的检测和识别效果。2、RPN网络用于得到ROI，这部分关于anchor尺寸的初始化基本上采用FPN算法的方式，比如宽高比例采用0.5、1和2，并没有设计专门用于文本检测的特殊尺寸anchor，这部分确实和许多文本检测算法不同。3、Fast RCNN关于预测框的分类和回归支路。**4、用于文本分割（word segmentation）和文本识别（character instance segmentation）的分割支路，这部分也是这篇文章的亮点，在该分割支路中通过分割方式同时做文本检测和文本识别两个任务，因此区别于两阶段的训练方式需要准确的文本检测结果作为文本识别模型的输入，在分割支路中同时做文本检测和文本识别不需要依赖准确的检测结果，同时需要注意的是文本识别部分是通过基于字符的实例分割实现的，与传统的识别网络不同。**
![这里写图片描述](https://img-blog.csdn.net/20180813221504887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**从前面Figure2关于网络整体结构的介绍可以看出Mask TextSpotter的主要特点在于分割支路的设计，分割支路同时完成文本的检测和识别，如下Figure3是分割支路的示意图**。首先输入ROI特征图的二维尺寸为16*64，然后经过一些卷积层和反卷积层继续提取特征，最后通过卷积核数量为38的卷积层输出38个map，每个map的二维尺寸都是32*128，其中第一个是文本分割的map（global text instance map），这部分结果不受文本的形状和方向影响；然后是36个数字及字母分割的map（character map），从这里可以看出在文本识别部分采用的是基于字母的识别方式（character-based）；最后一个是文本背景分割的map（background map）。 
![这里写图片描述](https://img-blog.csdn.net/20180813221528604?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
强调下标签的分配问题，在文中通过Figure4（a）体现。这里将Figure4（a）放大如下，提供的ground truth是（a）中左图的红色多边形框和黄色框。关于Mask TextSpotter的框分类和回归支路（Fast RCNN），根据红色多边形框可以得到覆盖该框的面积最小框：绿色框，绿色框就用于框分类和回归支路的ground truth，这部分和Fast RCNN中关于真负样本的定义一样。关于Mask TextSpotter的分割支路，则提供（a）中右图的两个map作为ground truth，这两个map分别是文本区域的map和文本的map。 
![这里写图片描述](https://img-blog.csdn.net/20180813221547336?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
损失函数如公式3所示，主要由三部分组成：1、RPN网络的Lrpn；2、Fast RCNN的分类和回归Lrcnn；3、分割支路的Lmask，实验中α1、α2和β都设置为1。其中前两部分损失函数和Fast RCNN一致，重点在于分割支路的Lmask。 
![这里写图片描述](https://img-blog.csdn.net/20180813221606493?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

分割支路损失函数Lmask如公式4所示，主要包含两个部分：1、文本分割的损失函数Lglobal，这部分是针对分割支路输出的38个map的第一个map进行计算；2、文本分割（识别）的损失函数Lchar，这部分是针对分割支路输出的38个map的后37个map进行计算（包含文本背景类）。 
![这里写图片描述](https://img-blog.csdn.net/20180813221620267?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Lglobal采用cross entropy loss，如公式5所示，N表示输出map的点数量；yn属于[0,1]，表示每个像素点的标签；xn表示分割支路输出的38个map中的第一个map的第n个值；输入概率是通过sigmoid函数S(.)得到的。 
![这里写图片描述](https://img-blog.csdn.net/2018081322164044?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Lchar采用加权的cross entropy loss，如公式6所示，输入概率通过softmax函数进行计算得到，因为这里是对文本做多分类，另外加权的目的是平衡背景类和文本类区域的贡献。 
![这里写图片描述](https://img-blog.csdn.net/20180813221657848?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

权重W的公式如7所示。Yi,0=1表示第i个像素点是背景，Nneg表示背景像素点的数量。 
![这里写图片描述](https://img-blog.csdn.net/20180813221710721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Inference阶段和训练阶段一个较大的不同点在于inference阶段分割支路的输入ROI特征采用Fast RCNN的检测输出而不是RPN网络的输出，因为前者的坐标和分类标签更加准确，另外在输入分割支路之前还会做常规的NMS操作。

**实验结果：**

Table1是在IDCAR2013数据集上的识别效果对比，因为该数据集的测试数据主要是水平方向的文本，所以Mask TextSpotter的优势不算很明显。 
![这里写图片描述](https://img-blog.csdn.net/20180813221735437?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table2是在IDCAR2015数据集上的识别效果对比，Mask TextSpotter在输入尺寸为720*1280的速度差不多是6.9FPS，另外由于该数据集的文本方向多样，所以对比效果明显。 
![这里写图片描述](https://img-blog.csdn.net/20180813221748963?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table3是在IDCAR2013和IDCAR2015数据集上关于检测效果的对比，主要评价指标是precision、recall和F值。其中Ours（det only）表示移除了分割支路中关于文本分割部分的输出，相当于只做检测一个任务，从而分析检测和识别两个任务之间的互补，从实验结果可以看出识别任务对检测任务有一定的促进作用。 
![这里写图片描述](https://img-blog.csdn.net/20180813221804561?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table4是在Total-Text数据集上的检测和识别效果对比，该数据集包含水平方向、任意方向、弯曲文本，因此Mask TextSpotter的对比效果更加明显。 
![这里写图片描述](https://img-blog.csdn.net/20180813221818498?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)












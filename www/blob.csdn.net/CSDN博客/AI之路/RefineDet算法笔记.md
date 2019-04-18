# RefineDet算法笔记 - AI之路 - CSDN博客





2018年03月09日 20:21:55[AI之路](https://me.csdn.net/u014380165)阅读数：15336








论文：Single-Shot Refinement Neural Network for Object Detection 

论文链接：[https://arxiv.org/abs/1711.06897](https://arxiv.org/abs/1711.06897)

代码链接：[https://github.com/sfzhang15/RefineDet](https://github.com/sfzhang15/RefineDet)
RefineDet是CVPR2018的论文，个人觉得是一篇很不错的文章，大致上是SSD算法和RPN网络、FPN算法的结合，可以在保持SSD高效的前提下提高检测效果（实验效果提升挺明显）。第一眼看到这篇文章就让我想起了RON，毕竟都是想做one stage和two stage的object detection算法结合。**RefineDet的主要思想：一方面引入two stage类型的object detection算法中对box的由粗到细的回归思想（由粗到细回归其实就是先通过RPN网络得到粗粒度的box信息，然后再通过常规的回归支路进行进一步回归从而得到更加精确的框信息，这也是two stage类型的object detection算法效果优于one stage类型的一个重要原因）。另一方面引入类似FPN网络的特征融合操作用于检测网络，可以有效提高对小目标的检测效果，检测网络的框架还是SSD。**

**Figure1是RefineDet网络的结构图（直观的特点就是two-step cascaded regression），主要包含三个部分：anchor refinement module (ARM) 、object detection module (ODM)、transfer connection block (TCB)。**

**anchor refinement module (ARM)**部分类似Faster RCNN算法中的RPN网络，主要用来得到bbox（类似Faster RCNN中的ROI或proposal）和去除一些负样本（这是因为负样本数量远大于正样本）。因此基于4层特征最后得到的还是两条支路，一个bbox的坐标回归支路，另一个是bbox的二分类支路。我们知道在Faster RCNN算法中RPN网络存在的意义就是生成proposal（或者叫ROI），这些proposal会给后续检测网络提供较好的初始信息，这也是one stage的object detection算法和two stage的object detection算法的重要区别，这里的anchor refinement module基本上扮演了RPN网络的角色，如果一定要说不同点的话，那应该就是这里的输入利用了多层特征，而RPN网络的输入是单层特征。

**transfer connection block (TCB)**部分是做特征的转换操作，也就是将ARM部分的输出feature map转换成ODM部分的输入，这部分其实和FPN算法的特征融合很像，FPN也是这样的upsample后融合的思想。

**object detection module (ODM)**部分和就基本上是SSD了，也是融合不同层的特征，然后做multi class classification和regression。主要的不同点一方面在于这部分的输入anchors是ARM部分得到的refined anchors，类似RPN网络输出的proposal。**另一方面和FPN算法类似，这里的浅层feature map（size较大的蓝色矩形块）融合了高层feature map的信息，然后预测bbox是基于每层feature map（每个蓝色矩形块）进行，最后将各层结果再整合到一起。而在SSD中浅层的feature map是直接拿来用的（并没有和高层的feature map融合），也就是对bbox的预测是在每一层上进行的，预测得到结果后再将各层结果整合在一起，这是非常重要的区别。这样做的好处就是对小目标物体的检测效果更好，这在FPN和RON等算法中已经证明过了。**

**具体网络结构是怎么构建的呢**？以特征提取网络为ResNet101，输入图像大小为320为例，在Anchor Refinement Module部分的4个灰色矩形块（feature map）的size分别是40*40,20*20,10*10,5*5，其中前三个是ResNet101网络本身的输出层，最后5*5输出是另外添加的一个residual block。有了特征提取的主网络后，就要开始做融合层操作了，首先是5*5的feature map经过一个transfer connection block得到对应大小的蓝色矩形块（P6）,transfer connection block后面会介绍 ，对于生成P6的这条支路而言只是3个卷积层而已。接着基于10*10的灰色矩形块（feature map）经过transfer connection block得到对应大小的蓝色矩形块（P5），此处的transfer connection block相比P6增加了反卷积支路，反卷积支路的输入来自于生成P6的中间层输出。P4和P3的生成与P5同理。

**因此整体来看该网络和tow stage的结构很像（都可以概括为two-step cascaded regression），一个子模块做RPN的事，另一个子模块做SSD的事。因此SSD是直接在default box的基础上进行回归的，而在RefineDet中是先通过ARM部分生成refined anchor boxes（类似RPN网络输出的propsoal），然后在refined anchor boxes基础上进行回归，所以能有更高的准确率，而且得益于特征融合，该算法对于小目标物体的检测更有效。**
![这里写图片描述](https://img-blog.csdn.net/20180531215732561?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Transfer Connection Block**的结构如Figure2所示，也是比较中规中矩的设计。 
![这里写图片描述](https://img-blog.csdn.net/20180531215746863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
关于**正负样本界定**的标准基本上和其他object detection类似，比如和ground truth的IOU超过阈值0.5的box为正样本，也就是label是1。显然这样做后很多box的标签都是背景标签，也就是所谓的负样本，通过前面说的ARM部分可以过滤掉一些负样本，但接下来还是要采用类似SSD算法中的hard negative mining来设定正负样本的比例（一般设定为1:3），当然负样本不是随机选的，而是根据box的分类loss排序来选的，按照指定比例选择loss最高的那些负样本即可。

**损失函数**方面主要包含ARM和ODM两方面，如式子1所示。在ARM部分包含binary classification损失Lb和回归损失Lr；同理在ODM部分包含multi-class classification损失Lm和回归损失Lr。关于这个式子中的参数介绍也在截图中给出了，核心思想和SSD类似。需要注意的是虽然本文大致上是RPN网络和SSD的结合，但是在Faster RCNN算法中RPN网络和检测网络的训练可以分开也可以end to end，而这里的训练方式就纯粹是end to end了，ARM和ODM两个部分的损失函数都是一起向前传递的。 
![这里写图片描述](https://img-blog.csdn.net/20180531215817883?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**关于inference的细节，文中给出了详细的描述**：At inference phase, the ARM first filters out the regularly tiled anchors with the negative confidence scores larger than the threshold , and then refines the locations and sizes of remaining anchors. After that, the ODM takes over these refined anchors, and outputs top 400 high confident detections per image. Finally, we apply the nonmaximum suppression with jaccard overlap of 0.45 per class and retain the top 200 high confident detections per image to produce the final detection results.

**实验结果**： 

Table1是非常详细的实验结果对比，测试数据包括VOC2007和VOC2012数据集。**以VGG-16为特征提取网络的RefineDet320在达到实时的前提下能在VOC 2007测试集上达到80以上的mAP，这个效果基本上是目前看到过的单模型在相同输入图像情况下的最好成绩了**。表格中最后两行在算法名称后面多了+，表示采用multi scale test，因此效果会更优一些。 
![这里写图片描述](https://img-blog.csdn.net/2018053121584753?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table2是在COCO数据集上的对比。 
![这里写图片描述](https://img-blog.csdn.net/20180531215900896?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table3是验证本文几个改进点的具体提升情况。 
![这里写图片描述](https://img-blog.csdn.net/20180531215914707?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

更多实验结果可以参看原论文。
**关于未来工作方面**，作者也给出了两个主要思路，其中attention机制也是目前分类和检测算法中的热点：In the future, we plan to employ RefineDet to detect some other specific kinds of objects, e.g., pedestrian, vehicle, and face, and introduce the attention mechanism in RefineDet to further improve the performance.











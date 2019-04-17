# Detection with Enriched Semantics(DES)算法笔记 - AI之路 - CSDN博客





2018年06月06日 22:41:34[AI之路](https://me.csdn.net/u014380165)阅读数：5032








论文：Single-Shot Object Detection with Enriched Semantics 

论文链接：[https://arxiv.org/abs/1712.00433](https://arxiv.org/abs/1712.00433)

**Detection with Enriched Semantics(DES)是CVPR2018的文章，基于SSD做改进，也是为了解决SSD中对于小目标物体的检测效果不好的问题，因为SSD算法对小目标的检测只是利用了浅层的特征，并没有用到高层的语义特征。因此这篇文章的出发点就是为了增加用于检测的feature map的语义信息，主要的措施包括：1、引入segmentation module用于得到attention mask，从而提高low level的feature map的语义信息。2、引入global activation module用于提高high level的feature map的语义信息。**实验效果不错，在VOC2007数据集上，DES512能达到81.7的mAP，而且在Titan X上的速度是31.7FPS（batch设置为8，不过比相同大小输入和相同特征提取网络的SSD算法慢一些）。

Figure1是DES算法的整体结构，主要包含3个部分：1、detection branch。这部分基本上和SSD没有太大差别。2、segmentation module。这部分主要针对low level的feature map，用来增强low level的feature map的语义信息。具体做法就是以conv4_3和bounding box level的segmentation ground truth作为输入，bounding box level的segmentation ground truth就是Figure1中左下角那个黑色中带黄色的矩形，那个黄色区域的边框原来是bounding box的ground truth，这里将其内部区域当作segmentation的ground truth。3、global activation module。这部分主要针对high level的feature map，也是类似attention的思想，最终增强high level的feature map的语义信息。 
![这里写图片描述](https://img-blog.csdn.net/20180606223640622?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
先介绍**segmentation module**，可以结合下面的公式和Figure3一起来看看吧。首先假设输入是X（conv4_3的输出），维度如下所示： 
![这里写图片描述](https://img-blog.csdn.net/20180606223700252?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对输入X执行g()函数和H()函数操作后得到Z，Z就类似attention的mask，可以参考Figure3中灰色箭头和红色箭头构成的回路（暂且称之为H支路，主要是attention操作）。 
![这里写图片描述](https://img-blog.csdn.net/20180606223716992?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后X和Z做点乘得到X’，这个X’就代替X做后续的检测工作。X‘就对应Figure1中左下角的segmentation module模块的输出（红色箭头），也对应Figure3中左下角的X’。 
![这里写图片描述](https://img-blog.csdn.net/20180606223725913?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面介绍的是得到attention的过程，那么如何训练得到一个好的attention呢？就是下面这个式子，对应Figure3中最上面的灰色箭头代表的过程（暂且称之为F支路，类似segmentation算法）。F()函数包含一个卷积核数量为N+1的1*1卷积层和一个softmax层，softmax是基于channel维度进行计算的，因此得到的Y就是预测的segmentation结果，feature map的每个值范围是0到1的概率。 
![这里写图片描述](https://img-blog.csdn.net/20180606223741270?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

因此Y是channel数量为N+1，高宽是H、W的feature map。因为F()中的softmax是基于channel维度进行计算的，所以该feature map的某个点在所有channel上的累加和是1，而且值最大的那个点对应的标签就是分割得到的图像中对应点的标签。 
![这里写图片描述](https://img-blog.csdn.net/20180606223753191?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Table2是segmentation module的示意图，图中一共包含两部分内容：一部分用来得到attention mask（前面公式中的H支路），另一部分用来做segmentation任务（前面公式中的F支路）。**图中用到4个A.convolution，A.convolution代表atrous convolution（或者叫dilated convolution），前3个atrous convolution用的dilation rate参数是2，最后一个atrous convolution用的dilation rate参数是4。对于F支路，损失函数采用cross entropy loss，典型的分割算法的操作，如果你熟悉分类算法会发现分割算法和分类算法的思路是类似的，只不过这里是对每个像素点做分类而已。因此损失函数的一个输入是前面得到的Y，另一个输入是分割的ground truth G，G是维度为H*W的ground truth，其中每个值的范围是0到N，表示背景和N个object类别，这样就构成了分割网络。 
![这里写图片描述](https://img-blog.csdn.net/20180606223818331?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Figure2是关于segmentation module效果图**。A是输入图像；B是输入图像经过特征提取网络后得到的特征；C对应对应Figure3中的输出Z，也就是segmentation module的输出结果；D是B和C点乘后得到的结果。 
![这里写图片描述](https://img-blog.csdn.net/20180606223840134?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**关于分割模块的ground truth生成**，如果某个点在一个bounding box的ground truth框内部，那么该点的标签就是对应ground truth框的标签；如果某个点同时在多个bounding box的ground truth框内部，如Figure4所示，那么该点的标签采用ground truth框面积小的那个框的标签；其他情况下点的标签都是背景，这样就能保证每个点有且只有一个标签。因为这和传统的分割算法的ground truth不一样，所以称之为weak segmentation ground-truth，优点在于不需要额外标注。 
![这里写图片描述](https://img-blog.csdn.net/20180606223901514?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**global activation module**主要包含3个部分的操作：1、spatial pooling；2、channel-wise learning；3、broadcasted multiplying。操作流程大概是这样的： 

对于输入X（假设维度是C*H*W），spatial pooling的操作如下公式所示，得到Z，其维度是C。 
![这里写图片描述](https://img-blog.csdn.net/20180606223917898?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

channel-wise learning操作如下公式所示，其中W1的维度是C’*C，W2的维度是C*C’，文中C’默认采用C*1/4，最后得到的S的维度是C*1*1。 
![这里写图片描述](https://img-blog.csdn.net/20180606223931473?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后的broadcasted multiplying操作是用S和输入X相乘得到X’，其实也是类似attention的思想。 
![这里写图片描述](https://img-blog.csdn.net/20180606223946275?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因此整体来看，整个object detection算法的损失函数包含原有的检测损失函数和分割损失函数。检测损失函数就是SSD中的object分类和回归两部分损失函数，分割损失函数就是前面介绍的基于像素点的cross entropy损失函数。下面公式中参数α默认采用0.1。 
![这里写图片描述](https://img-blog.csdn.net/20180606223959218?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实验结果：**

Table1是在VOC2007测试集上的测试结果，主网络为VGG16时，DES300在VOC2007 test数据集上的mAP达到79.7（CVPR2018的另一个篇文章：RefineDet为80）。 

Table2是在VOC2012测试集上的测试结果，在VOC2012 test数据集上的mAP达到77.1（RefineDet为78.1）。DES512的mAP分别是81.7和80.3，而RefineDet512的mAP分别是81.8和80.1，可以说是不相上下。 
![这里写图片描述](https://img-blog.csdn.net/20180606224019367?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table4是在COCO数据集上的对比。可以看出在小目标上的提升较为明显。 
![这里写图片描述](https://img-blog.csdn.net/20180606224032234?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table5是关于global activation module和segmentation module的优化效果对比。 
![这里写图片描述](https://img-blog.csdn.net/20180606224044793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table6是在速度和mAP上的对比。相同输入图像大小和特征提取网络情况下，DES算法的速度会比SSD慢一些，这个也容易理解，毕竟在SSD算法上加了不少层。不过如果将SSD的特征提取网络换成ResNet101，则在速度和精度上都落后于DES。 
![这里写图片描述](https://img-blog.csdn.net/20180606224056223?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)














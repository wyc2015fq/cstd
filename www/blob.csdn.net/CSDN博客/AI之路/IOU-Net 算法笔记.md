# IOU-Net 算法笔记 - AI之路 - CSDN博客





2018年08月24日 21:18:04[AI之路](https://me.csdn.net/u014380165)阅读数：4342








论文：Acquisition of Localization Confidence for Accurate Object Detection 

论文链接：[https://arxiv.org/abs/1807.11590](https://arxiv.org/abs/1807.11590)

**这篇ECCV2018的文章个人非常喜欢，我们知道在目标检测算法中最后一般都会通过NMS算法移除重复预测框，移除的依据是预测框的分类得分（classification confidence，也就是softmax层的概率输出），但是这种依据并不能很好地表征预测框的准确性，换句话说对于同一个ground truth而言，分类得分高的预测框并不一定比分类得分低的预测框好，这样就容易导致那些分类得分低，但是坐标和ground truth更接近，也就是坐标回归更准确的预测框在NMS阶段被过滤掉，导致最终指标的下降。**

**因此这篇文章就提出IOU-Net，通过预测检测框和ground truth的IOU来解决这2个问题：1、提出IOU-guided NMS，也就是在NMS阶段引入回归得分（localization confidence）作为排序指标而不是采用传统的分类得分。2、提出optimization-based bbox refinement替换传统的regression-based方法，提高了回归部分的可解释性。另外这篇文章还提出了Precise ROI Pooling（PrROI Pooling），通过积分方式计算ROI特征使得前向计算的误差进一步降低，同时反向传播时基于连续输入值计算梯度使得反向传播连续可导，相比之下ROI Pooling和ROI Align由于采用量化或几个点插值方式求ROI特征，不可避免地带来一些噪声，而且在反向求导时只对特定输入回传梯度。**

**Figure1是关于这篇文章出发点的介绍，具体而言就是说明了前面提到的2个问题。**
**（a）说明了用预测框的分类得分作为NMS的依据的缺点**。（a）中对比了模型预测框的分类得分和回归得分两个指标，其中红色框和绿色框都是模型对同一个ground truth（也就是黄色框）的预测框，红色框的分类得分比绿色框高，因此只要红色框和绿色框的IOU超过NMS算法设定的阈值（比如常用的0.5），那么绿色框就会被NMS过滤掉。但从（a）的3张图像可以直观上看出绿色框的预测结果要好于红色框，同时通过计算红绿预测框的回归得分（这个回归得分是作者定义的）可以看出绿色框的回归得分要高于红色框。 
**（b）说明了传统的基于回归目标的框位置迭代回归存在的缺点**。（b）中上面一行图像表示传统的基于回归（regression-based）的预测框确定过程，可以看到随着迭代次数的进行，预测框的坐标准确性逐渐降低（在开始阶段是上升了，图中显示的是下降的过程），这也就是文中说的非单调性（non-monotonic）。我们知道传统的基于回归的框预测方式是通过坐标点来监督的，比如常用的x、y、w和h的偏置，显然，假如你预测框的中心点坐标和宽高和ground truth尽可能接近，那么视觉上这样的预测结果就越准确。但是这篇文章换了一种思路，用IOU来监督框的预测，也很容易理解，预测框和ground truth的IOU越大，则视觉上的预测结果就越准确。这里之所以认为传统的基于回归方式预测框存在问题主要是在迭代回归上，也就是直观上不断refine预测框应该会有更好的结果，但是实验证明并不是（cascad RCNN也有相关实验），而基于IOU方式的框预测在不断refine过程中则不会出现这样的问题，这一点确实很有意思。 
![这里写图片描述](https://img-blog.csdn.net/20180824211037598?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**接下来是关于这两个问题的具体描述和实验对比。**

**先看看第一个问题，也就是关于在NMS阶段引入回归得分（localization confidence）作为排序指标而不是采用传统的分类得分的两个实验。**

**Figure2是关于预测框和对应ground truth的IOU值与分类得分（a）、回归得分（b）的关系图**。从（a）可以直观看出IOU值和分类得分之间没有明显的正相关，但是从（b）可以看出IOU值和回归得分之间有明显的正相关，除了直观上看以外，作者还计算了两张图各自的皮尔逊相关系数（Pearson correlation coefficient），分别是（a）：0.217，（b）：0.617，这也符合直观的视觉感受。这说明用分类得分作为依据判断一个预测框是否准确预测对ground truth是不合理的。**因此作者提出了IoU-guided NMS，也就是NMS操作以预测的IOU为依据而不是传统的以预测框的分类得分为依据来解决这个问题。**
![这里写图片描述](https://img-blog.csdn.net/20180824211120399?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Figure3是关于不采用NMS、采用NMS和采用这篇文章提出的IOU-Guided NMS时预测框和对应ground truth的IOU分布情况**。对比3种柱形图，在IOU等于0.9时差别最大，此时这些预测框和ground truth非常接近，也就是对应预测最为准确的那些预测框，从Figure3可以看出，和不采用NMS的柱形图（绿色）相比，采用NMS算法（蓝色）后预测框的数量少了将近一半，**这一定程度上说明NMS算法容易误将部分高质量的预测框过滤掉，不过也需要注意的是这里NMS去掉的大部分应该也是冗余的框，所以其实不好界定说去掉的这一半左右的预测框都是应该保留的，否者IOU-guided NMS对应的黄色柱形图也不至于只比NMS后的多一些而不是多很多**。 
![这里写图片描述](https://img-blog.csdn.net/20180824211159809?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
接下来看看第二个问题，也就是regression-based方式和optimization-based方式的具体描述和实验介绍。

**我们知道Faster RCNN系列算法其实对预测框的预测是分两个阶段的，也就是先通过RPN网络粗预测得到ROI，再通过Fast RCNN网络refine预测框，这样得到的结果就很不错的，因此很自然地就想能够多级联几个回归支路不断refine预测框（iterative bounding box regression）从而达到更加准确的预测结果**？CVPR2018的一篇Cascade RCNN其实做的工作很类似却稍有不同，那篇文章出发点是界定正负样本的IOU阈值对预测结果的影响，虽然也是采用迭代回归的方式，但是不同阶段的检测模型设置的界定正负样本的IOU阈值不同，**同时那篇文章也强调即便直接采用模型级联方式进行预测框的refine，模型的数量也不要超过2个**。IOU-Net这篇文章的也讨论这个问题，比如Figure4是关于传统的基于回归方式的预测框确定的缺点。（a）是在FPN网络上做的结果，横坐标的iteration times表示的是refine的次数，可以看出蓝色曲线在第0次迭代时的AP差不多稍低于0.37，第1次迭代时的AP差不多0.375，比第0次的结果有所提升，但是后续迭代时结果就越来越差，这个结果和cascade RCNN算法中的refine数量不超过2个的建议吻合。（b）是在cascade RCNN算法上的结果，因为cascade RCNN本身包含3个网络（或者说3个stage），所以在第0、1、2次迭代时蓝色曲线对应的AP值都是上升的，但是后续的迭代效果依然越来越差。 
![这里写图片描述](https://img-blog.csdn.net/20180824211232802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**分析完这两个问题，接下来就要引入IOU-Net来解决这两个问题了。**

**Figure5是IOU-Net的示意图**，整体上是在FPN网络中嵌入了IOU预测支路，IOU预测支路的监督信息就是ground truth和预测框的IOU值，该支路在结构设计上和FPN网络原有的回归和分类支路类似，另外将ROI pooling替换成PrROI pooling（precise ROI Pooling）。这里虚线框圈起来的部分表示standalone IOU-Net，在后续验证optimation-based bbox refinement算法优势时会将这部分结构应用在已有算法的预测结果上，相当于用IOU的监督信息对预测框做进一步的refinement。 
![这里写图片描述](https://img-blog.csdn.net/20180824211311179?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**IOU-guided NMS**比较容易理解，简单讲就是将传统NMS算法中以分类得分为依据替换成以预测的IOU（也就是回归得分）为依据，这部分算法的伪代码如Algorithm1所示。这里有一点需要注意的是关于预测框的分类得分更新：当要剔除一个预测框时，会计算被剔除框的分类得分和之前的计算的最高分类得分的最高值，保留最高的分类得分，对应下面伪代码中的第5行和第8行。**这样做的结果就是返回的IOU最大的预测框对应的分类得分不一定是该框对应的原始得分，而是以该预测框为依据剔除的框的最高分类得分。**
![这里写图片描述](https://img-blog.csdn.net/20180824211502186?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Otimization-based bbox refinement**的伪代码如Algorithm 2所示，主要是关于IOU支路的梯度计算和参数更新，**同时作者引入了PrROI pooling层，使得ROI特征计算更加准确。**
![这里写图片描述](https://img-blog.csdn.net/20180824211541149?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
接下来介绍PrROI pooling。

**Figure6是关于ROI pooling、ROI Align和PrROI pooling的区别示意图**，个人觉得描述得非常清楚，首先你需要明白的是这几种pooling方式都是基于ROI坐标和特征图进行特征提取的过程。 
**ROI pooling**。首先，预测得到的ROI先除以stride（比如16）并量化取整得到坐标为整数值的ROI，然后将ROI划分成k*k个bin（k常用7、14），划分后得到的一个bin（也叫grid）如Figure6中的红色虚线框，bin的左上角坐标是（x1,y1），右下角坐标是（x2,y2），因为（x1,y1,x2,y2）不一定都是整数，所以还有一个量化的过程，也就是对左上角坐标（x1,y1）向下取整，对右下角坐标（x2,y2）向上取整，这样就得到红色实线框。假如用wij表示特征图上（i,j）这个点的特征值（绿色小圆点），那么最后得到的ROI中一个bin的特征值就如Figure6中ROI Pooling图下方的公式所示，这里是求均值操作（Fast RCNN论文中以及目前常用的ROI pooling实现中采用最大池化，也就是用这个bin中的最大值作为这个bin的值）。ROI Pooling虽然解决了不同大小ROI的尺寸统一，但是其量化操作会引入一定的误差，这也是后续ROI Align的改进方向。 
**ROI Align**。不再对预测得到的ROI进行量化操作，首先，直接将坐标为浮点值得ROI划分成k*k个bin，对于ROI的1个bin计算该bin的值时也不对该bin的坐标进行量化，而是在该bin中均匀取4个点，这4个点的坐标用(ai, bi)表示，通过计算f(ai,bi)得到该点的特征值，f(.)的计算公式如公式2所示，其实就是插值的过程，从插值公式2可以看出距离(x,y)越近的点的权重IC(x,y,i,j)越大。比如左上角那个红点（f(x,y)）的值就由附近的4个绿色点（wij）的值加权得到，这样计算得到4个红点的特征值后求平均就是这个bin的特征值。ROI Align这种计算每个bin值得方式没有引入量化操作，时通过插值方式得到的值也比较准确，因此引入的误差较少，这是Mask RCNN和Faster RCNN相比很重要的一个改进点。但是ROI Align中每个bin都是设置4个点，这样没有考虑bin的大小差异，因此还存在可优化的点。 
**PrROI Pooling**。PrROI Pooling采用积分方式计算每个bin的值，如公式3所示。这种计算方式和ROI Align最大的区别在于计算一个bin的值时不仅仅考虑该bin中4个插值点的均值，而是将bin中的插值看作是连续的，这样就可以通过对该bin中所有插值点求积分得到该bin所包围点的总和，最后除以面积就得到该bin的值，因此结果更加准确。 
![这里写图片描述](https://img-blog.csdn.net/20180824211620561?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

公式2是关于ROI Align中计算一个bin的某个点的插值计算公式，该公式表示点(x,y)的值f(x,y)由多个(i,j)值wij通过加权相加得到。 
![这里写图片描述](https://img-blog.csdn.net/20180824211634920?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

公式3表示PrROI Pooling计算每个bin的值时采用的积分计算公式，其中f(x,y)如前面公式2所示。 
![这里写图片描述](https://img-blog.csdn.net/20180824211649510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实验结果：**

Table1是关于NMS、soft NMS和IOU-guided NMS算法的效果对比。在IOU阈值较高时IOU-guided NMS算法的优势还是比较明显的（比如AP90），原因就在于IOU阈值较高时需要预测框的坐标更加准确才能有较高的AP值，这正好和IOU-guided NMS的出发点吻合。 
![这里写图片描述](https://img-blog.csdn.net/2018082421170720?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Figure7是在不同IOU阈值情况下框的召回率情况对比，without NMS曲线对应最大的召回率，可以看出IOU-guided NMS确实在召回率方面和NMS算法相比优势明显。 
![这里写图片描述](https://img-blog.csdn.net/20180824211723955?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table2是关于optimization-based bbox refinement的实验对比，+Refinement表示在表中3个算法的输出预测框基础上用Figure5中的standalone IOU-Net结构做进一步refinement。 
![这里写图片描述](https://img-blog.csdn.net/20180824211737804?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
IOU-Net除了可以单独应用外（比如前面的Table2实验），还可以直接和原有的检测网络一起训练，完整的网络结构就类似Figure5，Table3是对应的实验结果。 
![这里写图片描述](https://img-blog.csdn.net/2018082421175310?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)















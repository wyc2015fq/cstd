# 论文总结：IOU-Net （Acquisition of Localization Confidence for Accurate Object Detection） - Snoopy_Dream - CSDN博客





2018年09月04日 17:29:52[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：398








**论文地址：**[https://arxiv.org/abs/1807.11590](https://arxiv.org/abs/1807.11590)
**代码地址：**[https://github.com/vacancy/PreciseRoIPooling](https://github.com/vacancy/PreciseRoIPooling)

## 1.论文的出发点

    传统的**2-stage**的目标检测算法中，**分类和定位被用不同的方法解决**。一般算法逻辑上可理解为先区分前后景，然后检测到的边界框与其中一个真值边界框大于阈值IoU，则认为它是正样本， 此目标可能与定位准确性不一致。具体来说，给定一个提议框（proposal），**每个类别标签的概率**可自然而然地用作该提议的「**分类置信度**」，而边界框回归模块却**只是预测了**针对该提议框的**变换系数**，**用以拟合**目标物体的**位置**。换而言之，在这个流程**缺失了「定位置信度」**，只有分类置信度没有定位置信度，导致的两个问题：

    1）**分类置信度和定位准确度之间的不匹配misalign**可能会导致**定位更准确的边界框**

在 NMS 过程中反而被更不准确的边界框**抑制了**。

**          这说明用分类得分作为依据判断一个预测框是否准确预测对ground truth是不合理**

![](https://img-blog.csdn.net/20180904173245590?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

    2）边界框回归的**非单调性non-monotonic，**即多次应用边界框回归，可能有损输入边界框的定位效果。且迭代边界框回归方法易受输入分布变化的影响。【由cascade rcnn论文所得】

![](https://img-blog.csdn.net/2018090417334344?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**「**需要解决**」**：不匹配的问题以及边界框回归的非单调性

## 2.解决方法概略

    1）IoU-guided NMS【使用预测得到的 IoU**替代分类置信度**作为 NMS 中的排名依据】即** [IOU作为定位置信度]**

    2 ) ****Otimization-based bbox refinement **基于优化的边界框修正,**相比于基于回归的方法，实现**定位准确度的单调提升**

## **3.解决方法细述**

### 3.1 ****IoU predictor****

![](https://img-blog.csdn.net/20180904180118215?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        整体上是在**FPN网络中嵌入了IOU预测**支路，IOU预测支路的监督信息就是ground truth和预测框的IOU值，该支路在结构设计上和FPN网络原有的回归和分类支路类似

        另外将ROI pooling**替换成PrROI pooling**（precise ROI Pooling）。这里虚线框圈起来的部分表示**standalone IOU-Net**，在后续验证optimation-based bbox refinement算法优势时会将这部分结构应用在已有算法的预测结果上，相当于用IOU的监督信息对预测框做进一步的refinement。

       本文IoU预测器的输入不是用RPN产生的RoIs，而是**对Ground Truth框随机扰动，得到Jittered RoIs**，过滤掉与GT的IoU<0.5的框，剩下的作为**输入【没有用RPN的proposals】**，经过PrRoI Pooling给IoU predictor。

### 3.2 IoU-guided NMS

       需要注意的是有一个：confidence clustering分类置信度的聚类，也就是伪代码第8行。整个代码的含义是：先找到IOU最高的预测框，然后和之前计算的最高分类得分进行比较，保留最高分类得分，以此解决不匹配的问题。

![](https://img-blog.csdn.net/2018090419065019?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对于一类的所有检测出的bbox B： 

  取出**定位置信度最大的框bm**, 得分为s 

      对于与bm的IoU大于阈值的框，将其过滤，若分类置信度高于bm，则更新bm的分类置信度。 

不断迭代进行，直到B中所有bbox都处理完毕。 

之前的NMS是以bbox的分类置信度排序，然后按IoU过滤。得到分类置信度最高的框，但是存在不匹配的问题。

IoU-guided NMS则是以**bbox的定位置信度排序**，然后按**IoU过滤**，并**更新分类置信度**。得到**定位置信度最高的框，并且分类置信度变成最高了 **

### **3.3 **Otimization-based bbox refinement ****

边界框精修问题可以通过数学方法定义，找到最优的c*： 
![这里写图片描述](https://img-blog.csdn.net/20180903090254389?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中boxdet是检测到的边界框，boxgt是（目标）真值边界框，transform是一个边界框变换函数，以c*为参数并变换给定的边界框。 crit是衡量两个边界框之间距离的标准。 在原始faster-R-CNN [5]框架中，crit被选择为对数尺度的坐标的smooth-L1距离，而在[32]中，crit为两个边界框之间的-ln（IoU）。

基于回归的算法用前馈神经网络直接估计最优解c* 。 然而，迭代边界框回归方法易受输入分布变化的影响[3]，并可能导致非单调的本地化改进，如图4所示。为了解决这些问题，我们提出了一种基于优化的边界框精修方法。 利用IoU-Net作为鲁棒定位精度（IoU）估计器的方法。 此外，IoU估计器可以用作早期停止条件，以通过自适应步骤实现迭代精修。

IoU-Net直接估算IoU（boxdet; boxgt）。 虽然所提出的精确RoI池化层能够计算关于边界框坐标的IoU的梯度，我们可以直接使用梯度上升方法找到方程1的最优解。在算法2中，将IoU的估计视为优化目标，我们迭代地使用计算的梯度更新边界框坐标和 最大化检测到的边界框与其匹配的真值之间的IoU。 此外，预测的IoU是每个边界框上的定位置信度的可解释指示符，并且有助于解释所做的转换。 
![这里写图片描述](https://img-blog.csdn.net/20180903101410202?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在实现中，如算法2第6行所示，我们手动按比例放大渐变w.r.t. 具有该轴上的边界框大小的坐标。 这相当于在[5]中以对数缩放坐标（x = w; y = h; log w; log h）执行优化。 我们还使用一步式边界框回归来初始化坐标。

**PrROI Pooling**

我们引入了精确的RoI Pooling（PrRoI Pooling，简称PrRoI Pooling），增进边界框精修。 它避免了任何坐标量化，并在边界框坐标上具有连续梯度渐变。 给定RoI / PrRoI池化之前的特征映射F（例如，来自ResNet-50中的Conv4），令wi， j是特征映射上的一个离散位置（i; j）处的特征。 使用双线性插值，可以在任何连续坐标（x; y）处将离散特征映射视为连续的：

![这里写图片描述](https://img-blog.csdn.net/20180903151719227?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018090315181886?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)是插值系数。 

然后将一个RoI的bin表示为bin = f（x1， y1），（x2， y2）g，其中（x1， y1）和（x2，y2）分别是左上和右下点的连续坐标。 我们通过计算二阶积分来执行给定bin和特征映射F的池化（例如，平均池化）： 
![这里写图片描述](https://img-blog.csdn.net/20180903153409346?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

为了更好地理解，我们在图6中可视化RoI Pooling，RoI Align [10]和我们的PrRoI Pooing：在传统的RoI Pooling中，需要先量化连续坐标以计算bin中激活值的总和; 为了消除量化误差，在RoI Align中，在bin中对N = 4个连续点进行采样，表示为（ai; bi），并且在采样点上执行合并。 RoI Align中N是预先定义的，而且不是关于bin的大小自适应，PrRoI池化则是基于连续特征映射直接计算二阶积分。

此外，基于上述的公式，PrPool（Bin; F）是关于bin可微分的坐标。 例如，PrPool（B; F）的关于x1偏导数可以计算为： 
![这里写图片描述](https://img-blog.csdn.net/20180903154733526?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其他偏微分同上。由于我们避免任何量化，ParPool是连续可微的。

## 3.4联合训练

这种 IoU 预测器可集成到标准的 FPN 流程中，以进行端到端的训练和推理。为了清楚说明，研究者将用于图像特征提取的 CNN架构称为骨干（backbone），将应用于各个 RoI 的模块称为头（head）。

如图 5 所示，这个 IoU-Net 使用了 ResNet-FPN [16] 作为骨干网络，其架构是自上而下的，可构建特征金字塔（featurepyramid）。FPN 能根据 RoI 的特征的比例从这个特征金字塔的不同层级提取这些 RoI 的特征。其中原来的 RoI 池化层被换成了精准 RoI池化层。至于该网络的头，这个 IoU 预测器根据来自骨干网络的同一特征而与 R-CNN 分支（包括分类和边界框回归）并行工作。

研究者根据在 ImageNet [25] 上预训练的 ResNet 模型初始化了权重。所有新的层都使用了标准差为 0.01 或 0.001的零均值高斯分布进行初始化。研究者使用了smooth L1 损失来训练 IoU 预测器。IoU 预测器的训练数据是在一个训练批中的图像中单独生成的，详见 3.1节。IoU 标签进行了归一化，其值分布在 [-1,1]。

对输入图像的大小进行了调节，短边长度为 800 像素，长边长度最大为 1200 像素。分类和回归分支取来自 RPN 的每张图像 512 RoI。研究者使用了16 的批大小进行训练。网络为 16 万次迭代进行了优化，学习率设置为 0.01，并在 12 万次迭代后减小 10 倍。研究者还为前 1 万次迭代设置了0.004 的学习率以进行预热。研究者使用了 1e-4 的权重衰减和 0.9 的 momentum.

在推理过程中，研究者首先对初始坐标应用边界框回归。为了加快推理速度，研究者首先在所有检测到的边界框上应用 IoU 引导式NMS。然后，使用基于优化的算法进一步改进 100 个有最高分类置信度的边界框。研究者设置步长为 λ=0.5，早停阈值为 Ω1=0.001，定位衰减容限Ω2=−0.01，迭代次数 T=5。

# 4 实验

研究者在有 80 个类别的 MS-COCO 检测数据集 [17] 上进行了实验。遵照 [1,16]，研究者在 8 万张训练图像和 3.5万张验证图像的并集（trainval35k）上训练了模型，并在包含 5000 张验证图像的集合（minival）上评估了模型。为验证该方法，在 4.1 节和4.2 节，研究者与目标检测器分开而训练了一个独立的 IoU-Net （没有 R-CNN 模块）。IoU-Net 助力的 IoU 引导式 NMS和基于优化的边界框修正被应用在了检测结果上。

## 4.1 IoU 引导式 NMS

表 1 总结了不同 NMS 方法的表现。尽管 Soft-NMS 能保留更多边界框（其中没有真正的「抑制」），但 IoU 引导式 NMS还能通过改善检测到的边界框的定位来提升结果因此，在高 IoU 指标（比如 AP_90）上，IoU 引导式 NMS 显著优于基准方法。 

表1 
![这里写图片描述](https://img-blog.csdn.net/20180903161831324?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 4.2 基于优化的边界精修

研究者提出的基于优化的边界框修正与大多数基于 CNN 的目标检测器 [16,3,10] 都兼容，如表 2 所示。将这种边界框修正方法应用在原来的使用单独 IoU-Net 的流程之后还能通过更准确地定位目标而进一步提升表现。即使是对有三级边界框回归运算的 Cascade R-CNN，这种改进方法能进一步将 AP_90 提升 2.8%，将整体 AP 提升 0.8%。 

表2 
![这里写图片描述](https://img-blog.csdn.net/20180903193427842?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 4.3联合训练

IoU-Net 可与目标检测框架一起并行地端到端优化。研究者发现，将 IoU 预测器添加到网络中有助于网络学习更具判别性的特征，这能分别将 ResNet50-FPN 和 ResNet101-FPN 的整体 AP 提升 0.6% 和 0.4%。IoU 引导式 NMS 和边界框修正还能进一步提升表现。研究者使用 ResNet101-FPN 得到了 40.6% 的 AP，相比而言基准为 38.5%，提升了 2.1%。 

表3 
![这里写图片描述](https://img-blog.csdn.net/20180903193650560?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

表 4 给出了推理速度，表明 IoU-Net 可在计算成本承受范围之内实现检测水平的提升。 

表4 
![这里写图片描述](https://img-blog.csdn.net/2018090319385280?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM1NjA4Mjc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们主要将表3中AP50的劣质结果归因于IoU估计误差。 当边界框具有较低的具有真实性的IoU时，它们在外观上具有较大的变化。 可视化如图2（b）所示. 

对于具有较低IoU的盒子，IoU估计变得不太准确。 这降低了下游再现和抑制的性能。 我们凭经验发现，这个问题可以通过诸如在训练期间以较低的IoU采样更多边界框的技术来部分地解决。

# 结论

本文提出一种用于准确目标定位的全新网络架构 IoU-Net。通过学习预测与对应真实目标的 IoU，IoU-Net 可检测到的边界框的「定位置信度」，实现一种 IoU 引导式 NMS 流程，从而防止定位更准确的边界框被抑制。IoU-Net 很直观，可轻松集成到多种不同的检测模型中，大幅提升定位准确度。MS-COCO 实验结果表明了该方法的有效性和实际应用潜力。

从学术研究的角度，本文指出现代检测流程中存在分类置信度和定位置信度不匹配的问题。更进一步，研究者将边界框修正问题重定义为一个全新的优化问题，并提出优于基于回归方法的解决方案。研究者希望这些新视角可以启迪未来的目标检测工作。

ref 
[https://blog.csdn.net/ctfabc4425/article/details/81490805](https://blog.csdn.net/ctfabc4425/article/details/81490805)




# 目标检测（十四）--CRAFT - 工作笔记 - CSDN博客





2017年09月12日 14:50:51[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5496
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

**CRAFT Objects from Images**
- intro: CVPR 2016. Cascade Region-proposal-network And FasT-rcnn. an extension of Faster R-CNN
- project page: [http://byangderek.github.io/projects/craft.html](http://byangderek.github.io/projects/craft.html)
- arxiv: [https://arxiv.org/abs/1604.03239](https://arxiv.org/abs/1604.03239)
- paper: [http://www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Yang_CRAFT_Objects_From_CVPR_2016_paper.pdf](http://www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Yang_CRAFT_Objects_From_CVPR_2016_paper.pdf)
- github: [https://github.com/byangderek/CRAFT](https://github.com/byangderek/CRAFT)

转自：[http://blog.csdn.net/shadow_guo/article/details/51763374](http://blog.csdn.net/shadow_guo/article/details/51763374) 略增删改

## **1. 简介与相关工作**

### **(1) 简介**

物体检测是为了确定图像中物体(object)的位置 和种类。分两步解决：从图像中产生物体建议(object proposals)；对物体建议分成不同的类 (object categories)。

两步法的发展得益于计算机视觉的两方面的进步：(1) 物体建议的引入；(2) 卷积神经网络的复兴。比较普遍的物体建议方法每张图像产生2000个建议，使得对每个建议可以用更复杂的分类器。卷积神经网络因丰富的表示和强大的泛化能力而兴起。R-CNN说明选择搜索 (Selective Search) 区域建议配上CNN分类器在物体检测上的前途光明。快速 R-CNN 和更快R-CNN 为 R-CNN 框架的改进版，引入区域建议网络 (Region Proposal Network)获得**更优质的区域建议**。

分类仅需与任务物体相关的区域建议，但 R-CNN 产生的区域建议包含过多背景区域，导致物体类内差异小于物体与背景的差异，从而引入很多相似类间的假正例。

“分而治之”：RPN 后面增加1个基于 CNN 的分类器，区分物体与背景。

### **(2) 相关工作**

**a. 区域建议**

无监督方法：选择搜索分层组合超像素；Edge-Box 更快地，MCG 更简洁地组合风格建议 (style proposal)。滑动窗口扫描的风格建议可产生任意尺度和宽高比的建议。

有监督方法：BING 用二值特征和支持向量机从背景中提取物体；Multi-box用 CNN 端到端地定位物体 (回归)；RPN 联合地估计区域位置，同时自带置信度。RPN 建议数可降至300以下。

**b. 物体分类**

已知区域建议的物体检测变为物体分类。

R-CNN 用选择搜索和CNN 效果很好。快速 R-CNN 共享不同建议间的卷积，多任务 (区域分类与边界框回归) 加快。更快 R-CNN 直接预测建议，共享检测网络与卷积特征的权重使整个流程更快。

## **2. 原理**

### **(1) 两步框架**

**a. 区域建议**

有些方法基于滑动窗口和组合风格建议。**RPN 产生的区域建议与组合风格建议互补，在级联结构中结合。**

**b. 物体分类**

多区域流程获得更细致的物体表示；内-外网络 (Inside-Outside Net) 跳过连接和用空间循环单元插入图像上下文。这些工作偏重学习更好的特征表示，学习目标未变。**与他们的工作并不重叠**，原作者级联了1个目标函数以补偿快速 R-CNN 最终检测时目标函数的瑕疵。

### **(2) 区域建议**

**a. RPN (基线)**

覆盖几乎所有物体实例的同时，理想的区域建议产生器产生的建议越少越好。RPN 能够获取不同物体的相似性。然而，分类区域的目的是区分物体与非物体 (彩色片段，尖锐和闭合边缘)。**RPN 难以获得极限尺度或形状的物体 (比如瓶子和船)，还包括表面不复杂或者藏在杂物中的物体 (比如植物，电视和椅子)。**

**b. 级联建议生成器**

RPN 后引入1个2类检测网络 (**FRCN**) 来区分真实物体与背景 (或误定位区域)。RPN 输入一般的图像块，输出类似纹理的一般模式。FRCN 输入为 RPN 输出，**学习更细致的模式**。FRCN 的建议区域更优质，多源建议的输入 (可以用互补信息) 能够合并。

![## 插图](https://img-blog.csdn.net/20160627093033015)

> 
图示的训练阶段未体现出2类，感觉自行车和人放在同一正样本里。后面的理解应是自行车为正样本时，人，其它物体和背景为负样本。


**c. 应用**

RPN 产生区域建议送入 FRCN；RPN 产生的建议送入 RPN-2。结果 FRCN 优于 RPN-2；FRCN 与 RPN 共享权重不会改善。

> 
RPN 与 FRCN 的共同点是产生区域建议，所以两个网络的结构相同。假设 RPN 输入2000个图像块，RPN 输出依然是输入的那2000个图像块，如果 RPN-2 与 RPN 权重相同， 那么RPN-2 输出依然是不变的图像块；如果 RPN-2 与 RPN 权重不同，那么 RPN-2 输出不一定是原来的图像块。但 FRCN 与 RPN 权重一定不同，所以 RPN-2 可以是共享权重后的 RPN，也可以是另外微调的 RPN，也可以是重新训练的 RPN。

FRCN 与 RPN 的主要区别是权重的用途。RPN 的权重用于从原图像块到中高层的特征抽象，而 FRCN 的权重用于从中高层到更细致的中高层的特征抽象。所以，FRCN 与 RPN 可能可以合并为更深的 RPN。


### **(3) 物体分类**

**a. 快速 R-CNN (基线)**

RPN 产生大量背景区域建议，所以快速 R-CNN 分类时增加背景类。用 Softmax 层的多类交叉熵训练分类器 (假正例太多)，辅以边界框回归。快速 R-CNN 与 R-CNN 的区域建议有相同的问题 (见“简介”)。所以，**误分类为最后检测的主要问题。**

**b. 级联分类器**

太多假正例造成误分类。**R-CNN 框架中用1对多的2类SVM分类。**每个2类分类器专注于找类内的差异。

**c. 应用**

FRCN-1 产生区域建议。FRCN-2 目标函数为N个2类交叉熵损失的和 (N为物体类的数目，不包括背景)。

FRCN-2 与 FRCN-1 的卷积层权重共享 (训练时不变)。FRCN-2 的全连接层权重继承FRCN-1 (训练时会变)。产生2N个置信度和4N个边界框目标的新层初始化为高斯分布。

测试时，RPN产生300个区域建议，FRCN-1产生约20个原检测，每个原检测有N个分数。FRCN-2对原检测再分类，有N个输出分数。最后的输出分数为每类的输出分数乘以对应类的原分数。

![这里写图片描述](https://img-blog.csdn.net/20160627093327392)

> 
前面 FRCN-1 用的是1个2类区域建议产生器。这里每类都有1个分数，所以我要理解成1个区域建议产生器来区分1类物体与背景，背景包括其它类物体。


## **3. 实验比较**

### **(1) 区域建议**

RPN 方法劣于选择搜索 (高IoU阈值时低召回率)；固定锚时更大网络不会缓和；级联建议产生器可行。

### **(2) 物体分类**

级联物体分类器为 FRCN-1 和 FRCN-2 串联。 共享卷积特征，但微调高层连接 (全连接层 + 分类器) 效果最好。1对多分类器仅解决1类物体的检测 (FRCN-1 的输出检测) 有效。

### **(3) 物体检测**

区域建议和分类器同时级联，VOC 07 和 VOC 12 的mAP分别为75.7% 和 71.3%；ILSVRC 的mAP 为48.5%。

> 
Yolo 原文的表格3给出了 Pascal VOC 2012 Leaderboard。Yolo 的mAP只有57.9%，与 Fast RCNN 结合的mAP为70.7%，也就是说比Yolo提高了0.6%。Yolo 与快速 RCNN 结合时 CNN的个数比CRAFT少1个前面的 RPN，且也为 快速 R-CNN 提供了优质的边界框，所以，**与快速 R-CNN 集成时，CRAFT 与 Yolo 的顶层区别为 CRAFT 对每类物体都有1个2类区域建议网络，而 Yolo 对所有类物体只有1个多类区域建议网络~** 同时MR_CNN_MORE_DATA的mAP为73.9%。





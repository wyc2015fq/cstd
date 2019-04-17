# 论文 | 用于密集对象检测的 Focal Loss 函数 - 知乎
# 



**论文：Focal Loss for Dense Object Detection**

**原文链接：[https://arxiv.org/abs/1708.02002](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1708.02002)**




**背景介绍**

目前最先进的目标检测系统是基于两个阶段的机制。第一个阶段生成一个稀疏的候选区域集合，第二个阶段使用卷积神经网络对候选区域进行分类处理。最典型的例子例如R-CNN系列框架。

另一种目标检测系统的解决思路是在一个阶段完成目标检测的任务。例如YOLO，SSD。这些系统的比两阶段的要快，但是在准确度方面有所下降。

本文进一步推进了上述的研究，提出了一种一阶段的目标检测系统。首次在COCO数据集上达到了最先进两阶段目标检测系统（例如FPN，Faster R-CNN）的性能。为了达到这个目的，我们把训练阶段的类别不平衡性视为一阶目标检测系统精度的瓶颈，并提出了了解决这个瓶颈的全新的损失函数。在此基础上，我们设计了一个名为RetinaNet的一阶目标检测模型。

**Focal Loss**

**2.1 Cross entropy**

二分类问题中loss的定义如下：
![](https://pic1.zhimg.com/v2-399cb8e2f972464041f4c6e48bae3b30_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='273' height='40'></svg>)



上式中
![](https://pic2.zhimg.com/v2-4d30c3e73dc148d606f86665694a9d95_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='59' height='17'></svg>)
代表样本属于1的概率。y代表标签。为了表示方便，我们定义
![](https://pic2.zhimg.com/v2-e78483a3b62b30acd5e4ce481022ff45_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='14' height='12'></svg>)
如下：
![](https://pic4.zhimg.com/v2-c1acaec850d32dd040ad5b5ea517c6cb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='181' height='40'></svg>)



CE loss 如下图中的蓝色曲线所示，这个loss的一个显著特征是，即使是很容易的分类情况下，也会产生较大的loss。当把这些大量的loss加起来的时候，将会产生较大的影响。
![](https://pic2.zhimg.com/v2-c25d9ab8319ab22de768d1bfc24122e9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='963' height='598'></svg>)
## **2.2 Balanced Cross Entropy**

常用的解决类别不平衡的方法是引入一个额外的参数
![](https://pic1.zhimg.com/v2-9b3717855e20d1fc6e3c740cf34ff68c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='10' height='8'></svg>)
。
![](https://pic1.zhimg.com/v2-9b3717855e20d1fc6e3c740cf34ff68c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='10' height='8'></svg>)
用来设置为相反类别的概率或者作为交叉验证的一个超参数。为了表示方便，我们重写了带
![](https://pic1.zhimg.com/v2-9b3717855e20d1fc6e3c740cf34ff68c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='10' height='8'></svg>)
参数的CE loss：
![](https://pic2.zhimg.com/v2-1eb49dad25691a78d4330021a5ffa521_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='147' height='17'></svg>)
## **2.3 Focal Loss Definition**

正如实验所示，在密集目标检测系统中，训练过程中，遇到的类别失衡将会对交叉熵损失函数产生较大影响。容易分类的负样本将会占据主要的损失以及梯度。尽管
![](https://pic1.zhimg.com/v2-9b3717855e20d1fc6e3c740cf34ff68c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='10' height='8'></svg>)
平衡了正负样本的重要性，但是无法对 容易分类的样本和难以分类的样本产生影响。因此我们提出了一种全新的loss function（focal loss），以此来降低 容易分类样本的重要性，从而把训练集中在难以分类的负样本上。 Focal loss 的形式如下：
![](https://pic4.zhimg.com/v2-ea83713003aaf99e60096780d4d4ce97_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='190' height='17'></svg>)
Focal loss 在 cross entropy 的基础上增加了一个调节因子，
![](https://pic1.zhimg.com/v2-2df0c3d8f36937463669aec99f349ff4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='106' height='17'></svg>)
。Focal loss 如下图所示，
![](https://pic2.zhimg.com/v2-806330cd096fbcffead87c957163d8c5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='10' height='12'></svg>)
的取值在[0, 5] 之间。从图中我们可以看出两点：
- 当一个样本被错分类，
![](https://pic2.zhimg.com/v2-e78483a3b62b30acd5e4ce481022ff45_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='14' height='12'></svg>)
值很小的时候，调节因子，
![](https://pic3.zhimg.com/v2-5b1a23ca6aaaf4a046b825cf7d1c8576_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='60' height='17'></svg>)
的值很小，因此不会对loss产生影响。当
![](https://pic2.zhimg.com/v2-e78483a3b62b30acd5e4ce481022ff45_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='14' height='12'></svg>)
值很大，趋近于1的时候，调节因子的值趋近于0，因此对于正确分类的样本的loss值被缩小了。
![](https://pic3.zhimg.com/v2-906b97e166f3f734e2120f4089d18d8e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='39' height='15'></svg>)
的时候，FL 等于 CE。当
![](https://pic2.zhimg.com/v2-806330cd096fbcffead87c957163d8c5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='10' height='12'></svg>)
增加时候，调节因子的影响相应的增加，实验中
![](https://pic4.zhimg.com/v2-774e4828de58ba91f74a352b2b78fb33_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='39' height='15'></svg>)
取得了最好的实验效果。

在实验中，我们使用了
![](https://pic1.zhimg.com/v2-9b3717855e20d1fc6e3c740cf34ff68c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='10' height='8'></svg>)
变种的focal loss ，形式如下：
![](https://pic3.zhimg.com/v2-07a2213551bb6aadedcf51aee35126aa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='206' height='17'></svg>)



**RetinaNet Detector**

为了验证使用的Focal Loss function，作者设计了一种RetinaNet，如下图所示：
![](https://pic2.zhimg.com/v2-487bef665819599a48c6615cc9787bf9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='906' height='268'></svg>)
RetinaNet 网络架构包含一个backbone 网络和两个subnetwork。Backbone网络在底层使用了ResNet，用来生成卷机特征，在此之上有一个Feature Pyramid Network（FPN）。Backbone外接两个subnetwork，一个用来分类，一个用来生成目标位置信息。




**实验验证**

我们在COCO数据集上进行了目标检测的实验，并将测试结果与近期最先进的方法进行比较。其中包括一阶模型和二阶模型。下表列出了测试结果：
![](https://pic2.zhimg.com/v2-c5714418a1f74d76a34b5e988aa8270d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='702' height='219'></svg>)
　　这是Retina-101-800模型的测试结果，训练过程中采用scale filter ， 训练时间是之前模型的1.5倍，但是AP值提升了1.3。对比当前最先进的一阶模型，我们的模型AP值提升了5.9%（39.1 vs 33.2）。对比当前最先进的二阶模型，Faster R-CNN w TDM，我们的模型也取得了2.3%的优势（39.1 vs 36.8）。

**总结**

本文中，我们将类别不平衡视为一阶目标检测系统的性能瓶颈，为了解决这个问题我们对CE（交叉熵损失）提出了改进，提出了focal loss。实验结果显示，基于focal loss 的RetinaNet ，相对比与之前的目标检测系统，有较大改进。





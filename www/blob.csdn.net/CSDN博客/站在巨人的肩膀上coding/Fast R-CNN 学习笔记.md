# Fast R-CNN 学习笔记 - 站在巨人的肩膀上coding - CSDN博客





2018年01月12日 18:46:34[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：370








Fast-RCNN建立在之前的深度卷积神经网络上进行有效的分类及目标检测。但是它使用了几个创新点来提高训练及测试速度，并且也提高了检测精度。

____________________________________________________________________________________________________________________________________

# 1 Introduce

相比图像分类、目标检测是一个更有挑战性的任务，因为检测需要对象的准确定位，因此目标检测有两个挑战：

- 必须处理许多候选对象位置（通常称为“proposals ”）。
- 这些候选对象只能提供粗糙的定位，但又必须进行完善来实现精确定位。


所以需要更复杂的方法来解决，并且解决这些问题的办法经常需要对速度，准确性及复杂度进行妥协。




在这篇文章中，作者提出一个单阶段训练算法，共同学习分类对象我们提出一个单阶段训练算法，可以同时学习去分类物体建议框并且改善他们的空间位置。




基于区域的卷积网络方法（RCNN）通过使用深层的ConvNet来分类对象建议框，实现了很棒的检测对象精度。 然而，R-CNN的缺点也很明显：


- Training is a multi-stage pipeline   训练的时候，R-CNN首先使用log loss 在对象建议框（object proposals ）上微调ConvNet  ，然后提取特征，之后用SVM分类器，最后再进行bounding-box regressors学习。
- Training is expensive in space and time   在每张图片中对对象建议框（object proposals ）进行提取来对SVM和bounding-box regressor作训练，还要写进磁盘中。训练时间和空间开销很大。
- Object detection is slow   对象检测很慢，即测试时间开销大。


RCNN很慢的原因主要是因为没有共享计算。SPP-net就通过共享计算提高了速度，它在ConvNet的最后一个卷积层才提取proposal，但是依然有不足之处。和R-CNN一样，它的训练要经过多个阶段，特征也要存在磁盘中，另外，SPP中的微调只更新spp层后面的全连接层，因为卷积特征是线下计算的，从而无法再微调阶段反向传播误差。对很深的网络这样肯定是不行的。




Fast-RCNN的优点：

1. 比R-CNN、SPP-net有更高的检测质量（mAP）；

2. 把多个任务的损失函数写到一起，实现单级的训练过程；

3. 在训练时可更新所有的层；

4. 不需要在磁盘中存储特征。 


____________________________________________________________________________________________________________________________________

# 2 Fast R-CNN architecture and training

整体框架如 Fig.1 。Fast R-CNN网络输入一整张图片及一系列对象建议框（proposal）。




训练过程如下：

1、网络首先用几个卷积层（conv）和最大池化层处理整个图像以产生conv特征图。

2、然后，对于每个对象建议框（object proposals ），感兴趣区域（region of interest——RoI）池层从特征图提取固定长度的特征向量。

3、每个特征向量被输送到分支成两个同级输出层的全连接（fc）层序列中：

其中一层进行分类，对 目标关于K个对象类（包括全部“背景background”类）产生softmax概率估计，即输出每一个RoI的概率分布；

另一层进行bbox regression，输出K个对象类中每一个类的四个实数值。每4个值编码K个类中的每个类的精确边界盒（bounding-box）位置，即输出每一个种类的的边界盒回归偏差。整个结构是使用多任务损失的端到端训练（trained end-to-end with a multi-task loss）。

![](https://img-blog.csdn.net/20161216091742656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





这里再用另一篇文章的总结来强调一下：[RCNN学习笔记(4)：fast rcnn](http://http//m.blog.csdn.net/article/details?id=51241831)

1.用selective search在一张图片中生成约2000个object proposal，即感兴趣区域RoI。

2.把它们整体输入到全卷积的网络中，在最后一个卷积层上对每个ROI求映射关系，并用一个RoI pooling layer来统一到相同的大小－> (fc)feature vector ，即－>提取一个固定维度的特征表示。

3.继续经过两个全连接层（FC）得到特征向量。特征向量经由各自的FC层，得到两个输出向量：

第一个是分类，使用softmax，第二个是每一类的bounding box回归。





另外还有一个关于测试过程的总结也写得不错：[Fast R-CNN论文详解](http://http//blog.csdn.net/WoPawn/article/details/52463853?locationNum=5)

![](https://img-blog.csdn.net/20161216210322438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


- 任意size图片输入CNN网络，经过若干卷积层与池化层，得到特征图；
- 在任意size图片上采用selective search算法提取约2k个建议框；
- 根据原图中建议框到特征图映射关系，在特征图中找到每个建议框对应的特征框【深度和特征图一致】，并在RoI池化层中将每个特征框池化到H×W【VGG-16网络是7×7】的size；
- 固定H×W【VGG-16网络是7×7】大小的特征框经过全连接层得到固定大小的特征向量；
- 第4步所得特征向量经由各自的全连接层【由SVD分解实现】，分别得到两个输出向量：一个是softmax的分类得分，一个是Bounding-box窗口回归；
- 利用窗口得分分别对每一类物体进行非极大值抑制剔除重叠建议框，最终得到每个类别中回归修正后的得分最高的窗口。





整体框架大致如上述所示了，对比SPP-Net，可以看出FRCN大致就是一个joint training版本的SPP-Net，改进如下：

- SPP-Net在实现上无法同时tuning在SPP layer两边的卷积层和全连接层。
- SPP-Net后面的需要将第二层FC的特征放到硬盘上训练SVM，之后再额外训练bbox regressor。

____________________________________________________________________________________________________________________________________

## 2.1 RoI pooling layer RoI池化层

每一个RoI都由一个四元组（r,c,h,w）表示，其中（r，c）表示左上角，而（h，w）则代表高度和宽度。这一层使用最大池化（max pooling）将RoI区域转化成固定大小的 H*W 的特征图（H和W是取决于任何特定RoI层的超参数）。


RoI 最大池化通过将 h×w RoI窗口划分为 h / H × w / W个子窗口网格，子窗口大小固定为 H × W ，然后将每个子窗口中的值max pooling到相应的输出网格单元 。这是SPP pooling层的一个简化版，即只有一层的“空间金字塔”。





roi_pool层将每个候选区域均匀分成 H × W 块，对每块进行max pooling。将特征图上大小不一的候选区域转变为大小统一的数据，送入下一层。


![](https://img-blog.csdn.net/20161216100031630?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





Rol pooling layer的作用主要有两个：

1.是将image中的rol定位到feature map中对应patch

2.是用一个单层的SPP layer将这个feature map patch下采样为大小固定的feature再传入全连接层。即

RoI pooling layer来统一到相同的大小－> (fc)feature vector 即－>提取一个固定维度的特征表示。


____________________________________________________________________________________________________________________________________

## 2.2 Initializing from pre-trained networks 从预训练网络进行初始化

作者实验了三个预训练的ImageNet 网络：CaffeNet，VGG_CNN_M_1024，VGG-16，每个网络有五个最大池层和五到十三个转换层。当预训练网络初始化Fast R-CNN网络时，要经历三个transformations：


1、最后一个最大池化层由 RoI 池化层代替，该层将 H 和W 设置为与第一个全连接层兼容；

2、网络最后的全连接层和softmax（被训练用于1000路ImageNet分类）被替换为前面描述的两个同级层：softmax的对K+1个类别的分类层，和bounding box 回归层

3、网络输入修改为两种数据：一组图片和每张图片的一组RoIs

____________________________________________________________________________________________________________________________________


## 2.3 Fine-tuning for detection 检测中的微调

Fast R-CNN用反向传播训练所有网络权重。首先，作者说明了为什么SPPnet无法更新空间金字塔池化层之前的层的权重：当每个训练样本（即RoI）来自不同的图像时，通过SPP层的反向传播非常低效。低效源于每个RoI可能具有非常大的感受野（接收区），通常包括整个输入图像。由于正向传播必须处理整个感受野，训练输入是非常大（通常是整个图像）。


作者提出了一种更有效的训练方法，利用训练期间的特征共享。在Fast RCNN训练中，随机梯度下降（SGD）小批量计算被分级采样，首先随机取样N张图片，然后每张图片取样 R / N 个RoIs 。关键的是，来自相同图像的RoI在向前和向后传播中共享计算和内存。


除了分层采样，Fast R-CNN使用一个精简的训练过程，一次微调中联合优化softmax分类器和bbox回归，而不是在三个独立的阶段训练softmax分类器，SVM和回归因子。看似一步，实际包含了：


多任务损失（multi-task loss）、小批量取样（mini-batch sampling）、RoI pooling层的反向传播（backpropagation through RoI pooling layers）、SGD超参数（SGD hyperparameters）


### Multi-task loss. 

Fast R-CNN网络分类损失和回归损失如下图所示【仅针对一个RoI即一类物体说明】，黄色框表示训练数据，绿色框表示输入目标：


![](https://img-blog.csdn.net/20161216214215663?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


-cls_score层用于分类，输出K+1维数组p，表示属于K类物体和背景的概率； 

-bbox_predict层用于调整候选区域位置，输出4*K维数组，也就是说对于每个类别都会训练一个单独的回归器； 





Fast RCNN有两个输出层：

一个对每个RoI输出离散概率分布：![](https://img-blog.csdn.net/20161216110156644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

一个输出bounding box回归的位移：![](https://img-blog.csdn.net/20161216110451820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

k表示类别的索引，前两个参数是指相对于object proposal尺度不变的平移，后两个参数是指对数空间中相对于object proposal的高与宽。





每个训练的RoI都被标记了ground-truth类别 u 以及ground-truth边界框回归 v 。在每个标记好的RoI上用multi-task loss 函数来级联的训练分类和bbox边界框回归：





![](https://img-blog.csdn.net/20161216111146392?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


约定u=0为背景分类，那么[u≥1] 函数表示背景候选区域即负样本不参与回归损失，不需要对候选区域进行回归操作； 




第一项是对于 u 类的分类损失（log loss for true class u）。

![](https://img-blog.csdn.net/20161216111938651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对于分类loss，是一个N+1路的softmax输出，其中的N是类别个数，1是背景。





第二项是回归损失，是在 u 类的真正边界框回归目标的元组 v 上定义的，是一个 4xN 路输出的regressor，也就是说对于每个类别都会训练一个单独的regressor，评估回归损失代价就是比较真实分类 u 对应的预测平移缩放参数和真实平移缩放参数的差距：




![](https://img-blog.csdn.net/20161216193452361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





其中，

![](https://img-blog.csdn.net/20161216193224890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是真实平移缩放参数

对于 u 重新预测bbox回归平移缩放参数：![](https://img-blog.csdn.net/20161216193421424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里的损失不是L2损失函数，而是smooth L1损失函数，对于离群点不敏感，因为有L2损失的训练可能需要仔细调整学习率，以防止爆炸梯度（控制梯度的量级使得训练时不容易跑飞）。

公式如下：


![](https://img-blog.csdn.net/20161216194020791?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


超参数 λ 是用来控制两个损失函数的平衡的。作者对回归目标![](https://img-blog.csdn.net/20161216200455795?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)进行归一化使其具有零均值及单位权方差（zero mean
 and unit variance）。所有的函数都设置超参数 λ = 1。

### Mini-batch sampling.

作者从对象建议框（object proposal）中选择25%的RoI，这些RoI与ground-truth bbox边界框至少有0.5的部分交叉重叠，也就是正样本，即 u >= 1。其余的RoI选那些IoU重叠区间在[0.1,0.5)的，作为负样本，即 u = 0，大约为75%。之所以选择负样本需要大于0.1的阈值是因为使用启发式的hard example mining（低于0.1的IoU作为难例挖掘的启发式）。在训练期间，图像有0.5的概率水平翻转。





|类别|比例|方式|
|----|----|----|
|前景|25%|与ground-truth bbox边界框重叠区间在[0.5,1]|
|背景|75%|与ground-truth bbox边界框重叠区间在[0.1,0.5)|





### Back-propagation through RoI pooling layers.  通过RoI池化层的反向传播。

说实话，池化层的反向传播这里我并没有看懂，在网上找了很多的论文笔记觉得下面这篇说得最为详细并能让人理解，就摘抄过来了。




从这篇借鉴理解RoI池化层：[Fast R-CNN论文详解](http://http//blog.csdn.net/WoPawn/article/details/52463853?locationNum=5)

首先看普通max pooling层如何求导，设xi为输入层节点，yi为输出层节点，那么损失函数L对输入层节点xi的梯度为：


![](https://img-blog.csdn.net/20161216212901648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20161216212938023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其中判决函数δ(i,j)表示输入i节点是否被输出j节点选为最大值输出。

不被选中【δ(i,j)=false】有两种可能：xi不在yi范围内，或者xi不是最大值。

若选中【δ(i,j)=true 】则由链式规则可知损失函数L相对xi的梯度等于损失函数L相对yi的梯度×（yi对xi的梯度->恒等于1），故可得上述所示公式；





对于RoI max pooling层，设xi为输入层的节点，yri 为第r个候选区域的第j个输出节点，一个输入节点可能和多个输出节点相关连，如下图所示，输入节点7和两个候选区域输出节点相关连；


![](https://img-blog.csdn.net/20161216213353884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


该输入节点7的反向传播如下图所示。

![](https://img-blog.csdn.net/20161216213548170?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


对于不同候选区域，节点7都存在梯度，所以反向传播中损失函数 L 对输入层节点 xi 的梯度为损失函数 L 对各个有可能的候选区域 r 【 xi 被候选区域r的第j个输出节点选为最大值 】输出 yri 梯度的累加，具体如下公式所示：


![](https://img-blog.csdn.net/20161216213616885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20161216213719448?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


判决函数 [i=i∗(r,j)] 表示 **i **节点是否被候选区域**r **的第**j **个输出节点选为最大值输出，若是，则由链式规则可知损失函数L相对 xi 的梯度等于损失函数 L 相对yri
 的梯度×（ yrj 对xi 的梯度->恒等于1)，上图已然解释该输入节点可能会和不同的yrj j有关系，故损失函数L相对xi 的梯度为求和形式。


### SGD hyper-parameters. SGD超参数选择

除了修改增加的层，原有的层参数已经通过预训练方式初始化：

用于分类的全连接层以均值为0、标准差为0.01的高斯分布初始化；

用于回归的全连接层以均值为0、标准差为0.001的高斯分布初始化，偏置都初始化为0； 



针对PASCAL VOC 2007和2012训练集，前30k次迭代全局学习率为0.001，每层权重学习率为1倍，偏置学习率为2倍，后10k次迭代全局学习率更新为0.0001；

动量设置为0.9，权重衰减设置为0.0005。


____________________________________________________________________________________________________________________________________

## 2.4 Scale invariance 尺度不变性

作者提出了使用两种方式对规模不变的对象进行检测：brute-force（单一尺度）和image pyramids（多尺度，图像金字塔）。


单一尺度直接在训练和测试阶段将image预先固定好像素大小，直接输入网络训练就好，然后期望在训练过程中网络自己能够学习到尺度不变性scale-invariance；

多尺度在训练阶段随机从图像金字塔【缩放图片的scale得到，得到多尺度图片，相当于扩充数据集】中采样训练，通过一个图像金字塔向网络提供一个近似的尺度不变，在测试阶段图像金字塔用来对每个object proposal近似尺度归一化，训练阶段每次采样一个图像就随机采样一个金字塔尺度。





作者在5.2节对单一尺度和多尺度分别进行了实验，不管哪种方式下都定义图像短边像素为s，单一尺度下s=600【维持长宽比进行缩放】，长边限制为1000像素；多尺度s={480,576,688,864,1200}【维持长宽比进行缩放】，长边限制为2000像素，生成图像金字塔进行训练测试；实验结果表明AlexNet【S for small】、VGG_CNN_M_1024【M
 for medium】下单一尺度比多尺度mAP差1.2%~1.5%，但测试时间上却快不少，VGG-16【L for large】下仅单一尺度就达到了66.9%的mAP【由于GPU显存限制多尺度无法实现】，该实验证明了深度神经网络善于直接学习尺度不变形，对目标的scale不敏感。





第2中方法的表现确实比1好，但是好的不算太多，大概是1个mAP左右，但是时间要慢不少，所以作者实际采用的是第一个策略，也就是single scale。


____________________________________________________________________________________________________________________________________

# 3 Fast R-CNN detection

一旦Fast R-CNN网络被微调，检测相当于运行正向传播（假设对象建议框object proposal是预先计算的）。

网络将图像（或图像金字塔，编码为图像列表）和待给得分的 R 对象建议框（object proposal）列表作为输入。

在测试阶段，R 大约为2K个，当使用图像金字塔的时候，每个RoI被指定尺度使得接近224*224。对于每个测试RoI r ，网络输出关于 r 的一个后验概率分布 p 和一系列预测bbox偏移（每个类 [共k个类] 获得自己的精确bbox预测）。然后使用估计概率![](https://img-blog.csdn.net/20161217104801619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)给
 r 赋予关于 k 个对象类的检测置信度。最后给每个类都实施一个非极大值抑制。

## 3.1 Truncated SVD for faster detection 截断SVD ——>减少检测时间

图像分类任务中，用于卷积层计算的时间比用于全连接层计算的时间多；

![](https://img-blog.csdn.net/20161217110420298?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


而在目标检测任务中，要处理的RoI数量比较多，几乎有一半的前向计算时间被用于全连接层（Fig . 2）。就Fast R-CNN而言，RoI池化层后的全连接层需要进行约2k次【每个RoI都要计算】，因此在Fast R-CNN中可以采用SVD分解加速全连接层计算；





具体如何实现的呢？

①物体分类和bbox回归都是通过全连接层实现的，假设全连接层输入数据为X，输出数据为Y，全连接层权值矩阵为W，尺寸为 u × v ，那么该层全连接计算为:

Y = W × X

②若将W进行SVD分解（奇异值分解），并用前t个特征值近似代替，即:

![](https://img-blog.csdn.net/20161217111000603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


U是 u × t 的左奇异矩阵，Σt 是 t × t 的对角矩阵 ，V是 v × t 的右奇异矩阵。


![](https://img-blog.csdn.net/20161217113238586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


截断SVD将参数量由原来的 u × v 减少到 t × (u + v) ，当 t 远小于 min(u,v) 的时候降低了很大的计算量。




在实现时，相当于把一个全连接层拆分为两个全连接层，第一个全连接层使用权值矩阵![](https://img-blog.csdn.net/20161217113016391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvV29uZGVyMjMz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)（不含偏置），第二个全连接层使用矩阵U（含偏置）；


当RoI的数量大时，这种简单的压缩方法有很好的加速。





实验表明（Fig . 2），SVD分解全连接层能使mAP只下降0.3%的情况下提升30%的速度，同时该方法也不必再执行额外的微调操作。





# 参考博客地址：

Fast R-CNN论文详解 [http://blog.csdn.net/WoPawn/article/details/52463853?locationNum=5](http://blog.csdn.net/WoPawn/article/details/52463853?locationNum=5)
RCNN学习笔记(4)：fast rcnn  [http://m.blog.csdn.net/article/details?id=51241831](http://m.blog.csdn.net/article/details?id=51241831)




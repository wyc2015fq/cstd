
# CornerNet: Detecting Objects as Paired Keypoints论文解读 - SIGAI_CSDN的博客 - CSDN博客
# [SIGAI_CSDN的博客](https://blog.csdn.net/sigai_csdn)


[博客首页](https://blog.csdn.net/SIGAI_CSDN)
[关于我们](https://me.csdn.net/SIGAI_CSDN)

2019年02月21日 11:45:34[SIGAI_csdn](https://me.csdn.net/SIGAI_CSDN)阅读数：106标签：[CoenerNet																](https://so.csdn.net/so/search/s.do?q=CoenerNet&t=blog)[ECCV2018																](https://so.csdn.net/so/search/s.do?q=ECCV2018&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=CoenerNet&t=blog)个人分类：[机器学习																](https://blog.csdn.net/SIGAI_CSDN/article/category/7772121)[人工智能																](https://blog.csdn.net/SIGAI_CSDN/article/category/7772122)[AI																](https://blog.csdn.net/SIGAI_CSDN/article/category/7772124)[SIGAI																](https://blog.csdn.net/SIGAI_CSDN/article/category/8625732)[深度学习																](https://blog.csdn.net/SIGAI_CSDN/article/category/8672413)[计算机视觉																](https://blog.csdn.net/SIGAI_CSDN/article/category/8647222)[
							](https://blog.csdn.net/SIGAI_CSDN/article/category/8672413)
[
																								](https://blog.csdn.net/SIGAI_CSDN/article/category/8625732)
[
				](https://blog.csdn.net/SIGAI_CSDN/article/category/7772124)
[
			](https://blog.csdn.net/SIGAI_CSDN/article/category/7772124)
[
		](https://blog.csdn.net/SIGAI_CSDN/article/category/7772122)
[
	](https://blog.csdn.net/SIGAI_CSDN/article/category/7772121)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190221113946307.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9zaWdhaS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)[\#\#\#\# 作者简介：SIGAI人工智能平台](https://img-blog.csdnimg.cn/20190221113946307.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9zaWdhaS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
#### 全文PDF下载：
#### [http://sigai.cn/paper_101.html](http://sigai.cn/paper_101.html)
本文提出一种使用单个卷积神经网络的新型物体检测方法: CornerNet.本文通过将目标定义为成对关键点，消除了单阶段检测网络中对anchor box的需要. 除了新颖的网络形式外,本文还介绍了角落池化(corner pooling):一种新的池化方式,它可以帮助网络更好地定位物体的边角。 实验表明，CornerNet在MS COCO上实现了42.1％的AP，优于所有现有的单阶段检测模型。
#### 一.     概述:
基于卷积神经网络的目标检测算法已经在各种具有挑战性的基准测试中取得了一系列成果。Anchor box作为现有技术模型的一个共同组成部分，以不同的大小和横纵比为检测任务提供一系列的候选框。单阶段检测模型将anchor box密集的放置在输入图片上, 并对每一个box进行类别预测和坐标校准。随着anchor box在检测模型上的普遍应用, 单阶段检测模型逐渐获得了等同甚至优于两阶段检测模型的精度, 同时保有更高的效率。
但anchor box的使用有两个缺点, 第一，检测网络通常需要非常大量的anchor box(如：在DSSD中超过$4$万个，在RetinaNet中超过$10$万)来保证可以最大限度覆盖所有的待检测物体。但结果便是:只有一小部分anchor与待检测物体有比较可观的重叠比例;正负anchor数量的差异在训练时造成了极大的数据不平衡并减缓了训练速度。
第二，Anchor box的使用引入了太多新的超参数和网络设计上的选择, 包括: anchor的个数, 大小, 横纵比等等。这些选择主要是通过ad-hoc启发式方法进行的，并且当与多尺度架构相结合时可能变得更加复杂，具体是指: 由于单个网络在多个分辨率下同时进行单独的预测，每个尺度都会使用不同的特征和对应的一组anchor box。
![](http://47.104.7.68/upload/mdfiles/20190220102231_48307.jpg)
图1作者将一个目标定义为一对组合在一起的边界框角。 卷积网络为所有左上角输出heatmap，为所有右下角输出heatmap，并为每个检测到的角点输出嵌入向量,训练网络以预测属于同一对象的角点的相似嵌入。
在本文中作者介绍了一种新的单阶段目标检测方法CornerNet，避免了anchor box的使用。作者将一个目标定义为一对关键点(边界框的左上角和右下角), 使用单个卷积网络来预测同一类别所有目标的左上角的热图(heatmap)，所有右下角的热图以及每个检测到的角点的嵌入向量。嵌入向量用于对属于同一目标的一对角点进行分组, 以训练网络的方法获取它们的向量表示。本文的方法极大地简化了网络的输出，并且无需设计anchor box。图1说明了该方法的整体流程。
![](http://47.104.7.68/upload/mdfiles/20190220102443_29859.jpg)
图2通常局部信息无法来确定边界框角落的位置。 本文通过提出一种新的池化层来解决这个问题。
CornerNet的另一个新颖组件是corner pooling，这是一种新型的池层，用于帮助卷积网络更好地定位边界框的角落。 如图2所示,边界框的一角通常在目标的范围之外，在这种情况下角落不能根据局部特征进行定位。 相反，为了确定在某个像素位置是否存在左上角，我们往往需要从该位置开始往右看至边界，并且垂直向下看至底部。 这激发了本文corner pooling的思想：使用两张特征图, 在每个像素位置, 对第一张特征图该像素所在行的右侧和第二个特征图该像素所在列的下方进行池化操作，然后将两个池化结果相加, 如图3所示:
![](http://47.104.7.68/upload/mdfiles/20190220102652_14573.jpg)
图3 角落池化：对于每个通道，我们求两个方向（红线）的最大值（红点），每个方向都来自一个单独的特征图，并将两个最大值加在一起（蓝点）。
作者假设了检测角落比检测边界框中心更好的两个原因。 第一，anchor的中心可能更难以定位，原因是它取决于目标的所有4个边，而定位角点仅取决于2个边因而更加容易，而且corner pooling为定义角点引入了合理的先验。 第二，角点提供了一种密集地离散化box空间的方法：我们只需要$O(wh)$的角点便可以表示出$O(w^2h^2)$可能的anchor。
本文证明了CornerNet在MS COCO数据集上的有效性: Corner-Net实现了$42.1％$的AP，优于所有现有的单阶段探测模型。另外通过研究，作者发现corner pooling对于CornerNet的卓越性能至关重要。
#### 二. 相关工作:
##### Two-stage detector:
R-CNN首先发明并推广了两阶段检测方法。两阶段检测器生成稀疏的感兴趣区域（RoI）并通过网络对它们中的每一个进行分类。 R-CNN使用低级视觉算法生成RoI, 然后从图像中提取每个区域并由ConvNet独立处理, 因而需要大量冗余的计算。后来，SPP和Fast-RCNN通过设计一个特殊的池化层来改进R-CNN，该池化层从特征图中池化每个区域, 但是两者仍然依赖于单独的提议(proposal)算法，并且无法进行端到端的培训。Faster - RCNN通过引入区域提议网络（RPN）来避免低级别提议算法，该网络从一组预定的候选框（通常称为anchor box）生成提议, 这不仅使检测器更有效，而且还允许检测器端到端地进行训练。 R-FCN通过用全卷积替换全连接的子检测网络，进一步提高了Faster-RCNN的效率。
##### One-stage detector:
另一方面，YOLO和SSD推广了单阶段的检测方法，它消除了RoI池化步骤并从单个网络中检测目标。单阶段检测器通常比两阶段检测器更具计算效率，同时在不同的挑战性基准测试中保持着有竞争力的性能。
SSD将anchor box密集地布置在来自多个尺度的特征图上，直接分类并重新定义每个anchor。 YOLO直接从图像预测bbox坐标，而后在YOLO9000中引入了anchor的概念。 DSSD 和RON采用类似于hourglass net的网络，通过skip connection组合低级和高级特征，以更准确地预测边界框。然而在引入RetinaNet之前，这些单阶段检测器仍然无法优于两阶段检测器。在RetinaNet中作者提出, 密集的anchor box在训练时使正负样本间产生极大的数据不平衡,这种不平衡导致训练效率的低下及性能的下降。为了动态调整每个anchor的权重, 他们提出了一种新的损失函数 : Focal Loss，并通过实验证明他们的单阶段检测器甚至可以胜过两阶段的检测模型。 RefineDet通过过滤anchor box粗略的调整了anchor结构, 使得负anchor的数量得到一定程度的限制.
DeNet是一种两阶段的检测器，它可以在不使用anchor box的情况下生成RoI, 具体步骤如下: 首先它确定每个位置属于边界框的左上角，右上角，左下角或右下角的可能性, 然后它通过枚举所有可能的角点组合来生成RoI，并遵循标准的两阶段方法来对每个RoI进行分类。本文的方法与DeNet非常不同, 第一，DeNet不会识别两个角是否来自相同的对象，并通过子检测网络来拒绝错误的RoI, 而本文的方法是一个单阶段方法，使用单个ConvNet检测和分组角点。第二，DeNet在手动确定的位置选择相对于分类区域的特征，而本文的方法不需要任何特征选择步骤。第三，本文使用了corner pooling，一种新型的池化层，来增强角点的检测。
本文的方法受到了Newell等人在多人姿势估计的关联嵌入工作的启发。Newell提出一种在单个网络中检测和分组人体关节的方法, 在他们的方法中，每个检测到的人类关节具有一个嵌入向量, 最终通过嵌入向量之间的距离对关节进行分组。在本文中，作者首先将物体检测任务定义为同时检测和分组角点的任务, 本文的另一个新颖之处便是corner pooling，它有助于更好地定位角落。作者还显著地修改了沙漏的架构，并添加了focal loss的新型变体以更好地训练网络。
#### 三.CornerNet:
##### Overview:
在CornerNet中，作者将目标定义为一对关键点(边界框的左上角和右下角)。卷积网络预测两组heatmap来表征不同类别对象的角点位置，一组用于左上角，另一组用于右下角。网络还预测每个检测到的角点的嵌入向量，使得来自同一对象的两个角点的嵌入向量之间的距离很小。为了产生更准确的边界框，网络还预测了角点位置微调的偏移量。通过heatmap，嵌入向量和偏移量，作者应用一个简单的后处理算法来获得最终的边界框。
![](http://47.104.7.68/upload/mdfiles/20190220102855_78878.jpg)
图4: CornerNet概述。 backbone之后是两个预测模块，一个用于左上角，另一个用于右下角。 使用两个模块的预测进行定位并分组角点。
图4是CornerNet的一个概述图。本文使用沙漏网络作为CornerNet的backbone。沙漏网络之后是两个预测模块, 一个模块用于左上角，而另一个模块用于右下角。每个模块都有自己的corner pooling模块，用于在预测heatmap，嵌入向量和偏移量之前池化沙漏网络中的特征。与许多其他目标检测器不同，本文不使用多尺度特征来检测不同尺寸的物体,而只是将两个模块应用于沙漏网络的输出端。
##### Detecting Corners:
作者预测两组heatmap，一组用于左上角角点，另一组用于右下角角点。 每组heatmap的大小为$KaTeX parse error: Expected 'EOF', got '\*' at position 2: H\̲*̲W$, 通道数为C(类别个数且不设有背景通道)。每个通道都是一个二进制掩码，用于指示对应类别的角点位置。
对于物体的每一个拐角，都有一个ground-truth的正位置与之对应，所有其他位置都为负。在训练时，本文没有对负位置进行同等地惩罚, 而是减少了对以正位置为圆心的某个半径区域内的负位置给予的惩罚。这是因为如果一对错误corner的预测都靠近它们各自的ground-truth位置，它仍然可以产生一个与真实物体足够重叠的bbox（图5）。本文通过物体的大小来确定该半径的数值, 以保证该半径范围内的角点仍能以$e^{-\frac{x^2+y^2}{2\sigma^2}}$较高的IOU t(如$0.7$)来表示相应物体。在给定半径的情况下, 惩罚量由非标准化的2D高斯分布给出，即其中心位于正位置且$\sigma $是半径的1/3。
![](http://47.104.7.68/upload/mdfiles/20190220102948_12452.jpg)
图5. “Ground-truth“用于训练的heatmap。Boxes（绿色虚线矩形），其角落在正位置的半径内（橙色圆圈）仍然与Ground-truth标注（红色实心矩形）有很大的重叠。
$P_{cij}$为预测heatmap中c类别对应位置（$i$;$j$）的得分，$y_{cij}$为用非标准化高斯增强的“ground-truth”的heatmap。本文设计了一个focal loss的变体：
$$
L_{det}=\frac{-1}{N}\sum_{c=1}{C}\sum_{i=1}{H}\sum_{j=1}^{W}\left{\begin{matrix}
(1-p_{cij})^{\alpha}\log(p_{cij})  &if;y_{cij}=1 \
(1-y_{cij})^{\alpha }\log(1-p_{cij}) &otherwise
\end{matrix}\right.
$$
其中$N$是图像中目标的数量，$\alpha$和$\beta$是控制每个点的贡献的超参数（本文的所有实验中设置为$2$和$4$）。（1-y_{cij}）项减少了ground-truth周围的惩罚。
许多网络涉及下采样层来获取全局信息并减少内存使用, 所以在网络应用于图像时，输出的特征图大小通常小于图像的大小。 因此，图像中的位置（$x$;$y$）被映射到heatmap中的位置为$(\frac{x}{n},\frac{y}{n})$，其中$n$是下采样因子。当我们将位置从heatmap重新映射到输入图像时，可能会丢失一些定位精度，而这可能会极大地影响小边界框与ground-truth匹配的IoU。 为了解决这个问题，本文对预测位置设置微调偏移量，然后再将它们重新映射到输入图像中。
$$
o_{k}=\left ( \frac{x_{k}}{n}-\left \lfloor \frac{x_{k}}{n} \right \rfloor ,\frac{y_{k}}{n}-\left \lfloor \frac{y_{k}}{n} \right \rfloor\right )
$$
$O_k$是偏移量，$x_k$和$y_k$是角点$k$的$x$和$y$坐标。本文预测所有类别的左上角共享一组$O$集合，以及右下角共享另一组$O$集合。 对于训练，本文在ground-truth角点位置应用平滑的$L1$损失：
$$
L_{off}=\frac{1}{N}\sum_{k=1}^{N}SmoothL1Loss(o_{k},\hat{o}_{k})
$$

##### Grouping Corners:
多个对象可以出现在同一张图像中，因此可以检测到多个左上角和右下角, 因而需要确定一对左上角和右下角是否来自同一个边界框。本文的方法受到Newell等人提出的Associative Embedding方法的启发。Newell检测所有人体关节并为每个检测到的关节生成嵌入向量, 根据嵌入向量之间的距离对关节进行分组。
关联嵌入的思想也适用于本文的任务: 网络预测每个检测到的角点的嵌入向量，使得如果左上角和右下角属于同一个边界框，则它们的嵌入向量之间的距离应该很小, 从而可以根据左上角和右下角的嵌入之间的距离对角进行分组。嵌入向量的实际值并不重要, 本文仅使用嵌入向量之间的距离来对角进行分组。
本文学习Newell等人的方法, 并使用1维嵌入向量,$e_{tk}$和$e_{bk}$分别为目标$k$的左上角和右下角嵌入向量。本文使用”pull“ loss训练网络对角点进行分组，并使用”push”将角点分离：
$$
L
$$
$$
\begin{aligned}
L_{pull} &amp;=\frac{1}{N}\sum_{k=1}^{N}\left [ (e_{t_{k}}-e_{k})^2 +(e_{b_{k}}-e_{k})^2\right ] \\ 
Lpush &amp;=\frac{1}{N(N-1)}\sum_{k=1}^{N}\sum_{j=1,j\neq k}^{N}\max(0,\Delta -\left | e_{k}-e_{j} \right |) 
\end{aligned}
$$
其中$e_k$是$e_{tk}$与$e_{bk}$的均值,本文在所有实验中设置$\delta$为1, 与$L_{off}$相同,这里仅在ground-truth角点计算loss。
##### Corner Pooling:
如图2所示，局部特征往往无法决定角点的存在, 为了确定一个像素是否是左上角，需要从该像素起向右水平扫描以寻找对象的最顶部边界，并垂直向下扫描以寻找最左边界。 因此本文通过编码这一显式的先验知识, 提出了corner pooling来更准确地定位角点。
![](http://47.104.7.68/upload/mdfiles/20190220103122_59796.jpg)
图6.左上角pooling层可以非常有效地实现。 我们从左到右扫描进行水平最大池化，从下到上扫描进行垂直最大池化。 之后将两个最大池化特征图相加。
假设我们想确定位置（$i$,$j$）处的像素是否是左上角。 设ft和fl是作为左上角池化层输入的两张特征图，让ftij和flij分别是ft和fl中位置（$i$,$j$）处的向量。假设特征图大小为$H*W$，corner pooling首先将（$i$,$j$）和（$i$,$H$）之间的所有特征向量进行最大值池化得到特征向量$t_{ij}$，同时将（$i$,$j$）和 （$W$,$j$）间的特征向量进行最大值池化得到特征向量$l_{ij}$,  最后将$t_{ij}$和$l_{ij}$相加,具体公式如下:
$$
t
$$
$$
l
$$
$$
\begin{aligned}
t_{ij} &amp;=\left\{\begin{matrix}
\max(f_{t_{ij}},t_{(i+1)j};) \quad if\;i&lt; H &amp; \\ 
 f_{t_{Hj}}&amp;otherwise 
\end{matrix}\right. \\ 
 l_{ij}&amp;= \left\{\begin{matrix}
\max(f_{l_{ij}},l_{i_{j+1}\;})\quad if\;j&lt; W &amp; \\ 
 f_{l_{iW}}&amp; otherwise
\end{matrix}\right.
\end{aligned}
$$
其中max操作为元素级别(elementwise)的最大值操作, 如图6所示. 通过动态规划,$t_{ij}$与$l_{ij}$均可被快速的计算。
本文以类似的方式定义了右下角池化层, 它最大值池化了（$0$;$j$）和（$i$;$j$）之间以及（$i$;$0$）和（$i$;$j$）之间的所有特征向量，之后将它们相加得到输出特征图。角点池化层在本文中用于预测模型中的heatmap、嵌入向量和偏移量矩阵。
![](http://47.104.7.68/upload/mdfiles/20190220103211_80764.jpg)
图7.预测模块以修改后的残差块为开始，用角点池化模块替换第一个卷积模块, 在修改的残差块之后是另一组卷积模块, 并通过多个分支分别预测heatmap，嵌入向量和偏移量矩阵。
预测模块的结构如图7所示: 模块的第一部分是对原始残差块的一个修改版本, 在这个修改后的残差块中，作者用一个角点池化模块替换了第一个$3*3$的卷积模块. 该模块首先通过两个$128$通道的$3*3$卷积模块处理来自backbone的特征，然后应用角点池化层, 并将池化后的特征图输入到$256$通道的$3*3$Conv-BN层中，同时引入shortcut连接。 修改后的残差块后接一个256通道的$3*3$卷积模块，以及3个Conv-ReLU-Conv层，用于生成heatmap，嵌入向量和偏移量矩阵。
##### Hourglass Network:
CornerNet使用沙漏网络作为其backbone. 沙漏网络在设计之初是为了解决人体姿势估计问题, 它是一个由单个或多个沙漏模块组成的全卷积神经网络。沙漏模块首先通过一系列卷积和最大值池化层对输入特征进行下采样, 然后再通过一系列上采样层和卷积层将特征上采样回原始的分辨率。由于最大值池层会造成细节信息的丢失，因而网络添加shortcut层以将细节带回到上采样后的特征图中。沙漏模块的优点在于:通过一个统一的结构便可同时捕获全局特征和局部特征, 且当多个沙漏模块堆叠在网络中时，沙漏模块可以对这些特征进行多次重处理以捕获更高级别的语义信息.而这些属性也使沙漏网络成为目标检测任务backbone的理想选择。事实上, 许多现有的检测器都已经采用了类似于沙漏网络的结构模型。
本文对沙漏模块的结构进行了一些修改, 设计出了一种由两个沙漏组成的沙漏网络。作者使用步幅为2的卷积层替代最大值池化进行下采样，整个网络结构将特征的分辨率降低了5倍，同时增加了网络中特征图的通道数量（256; 384; 384; 384; 512）,作者应用两个残差模块后接最近邻上采样模块对特征进行上采样, 每个shortcut连接也包含两个残差模块。在每个沙漏模块前，作者使用步幅为2的128通道$7*7$卷积后接步幅为2的256通道残差模块将图像分辨率降低了4倍.
#### 四.实验:
##### Training Details:
作者使用PyTorch实现了CornerNet。 网络在PyTorch的默认设置下随机初始化，并且不对任何外部数据集进行预训练。 当引入focal loss时，作者按照何凯明的方法设置卷积层中的偏差来预测角点heatmap。 在训练期间，为了保证输出分辨率为128*128, 作者将网络的输入分辨率设置为511*511. 为了减少过度，作者采用标准数据增强技术，包括随机水平翻转，随机缩放，随机裁剪和随机颜色抖动(包括调整图像的亮度，饱和度和对比度),同时作者也将PCA应用到了输入图像上。
作者使用Adam优化训练误差:
$$
L=L_{det}+\alpha L_{pull}+\beta L_{push}+\gamma L_{off}
$$
其中α,β,γ分别是pull、push、偏移量损失对应的权重。 作者将α,β设置为0.1, γ设置为1(原因是作者经过实验发现将α,β设置为1或更大的值会导致性能的急剧下降)。 作者设置batchsize为49并在10个Titan X（PASCAL）GPU上训练网络（主GPU上的4个图像，其余GPU的每个GPU 5个图像）。 为了节省GPU资源，在本文的对照实验中，作者将网络以$2.5×10^{-4}$的学习率训练了250k次迭代, 而在本文的对比实验(与其他检测模型)中，作者将网络训练了额外的250k次迭代，之后将学习速率降低到$2.5×10^{-5}$并进行了最后50k次迭代。
##### Testing Details:
测试时，本文使用简单的后处理算法从heatmap，嵌入向量和偏移量矩阵中生成边界框。作者首先在角点heatmap上进行3*3最大值池化来进行非极大值抑制（NMS）,然后从heatmap中选择置信度最大的前100个左上角和前100个右下角点,角点位置由相应的偏移量进行调整。作者计算左上角和右下角嵌入向量之间的L1距离,距离大于0.5或包含来自不同类别的角点对将被拒绝。左上角和右下角的平均置信度用作检测的最终置信度。
在本文中输入图片没有resize的过程, 取而代之的是保持图像的原始分辨率并通过zero padding对图片尺寸进行填充。本文将原始图片和翻转后图像都用于测试,作者结合原始和翻转图像的检测检测结果，应用soft-nms来抑制冗余检测,最终仅输出置信度最高的前100个检测项。最终CornerNet在Titan X（PASCAL）GPU上的图片平均检测耗时为244毫秒。
##### MS COCO:
作者在MS COCO数据集上对CornerNet进行了评估, MS COCO包含80k张训练集图像，40k张验证集图像和20k张测试集图像。 作者将训练集中的所有图像和验证集中的35k张图像用于训练,验证集中剩余的5k图像用于超参数调整和对照实验研究,测试集上的所有结果都提交给外部服务器进行评估。 为了与其他探测器进行比较，作者提供了本文在test-dev集上的检测结果。 MS COCO在不同的IoU级别以及不同的物体大小上都计算了平均精度(average precisions),共同组成了统一的评测标准.
#### Ablation Study:
Corner Pooling:
角点池化是CornerNet的关键组成部分, 为了理解其对性能的贡献，作者训练了另一个没有角点池化但具有相同数量参数的网络。
![](http://47.104.7.68/upload/mdfiles/20190220103315_55884.jpg)
表1中展示了添加角点池化层对网络的改善效果, 对应的提升结果为：AP提升2.0％，AP50提升2.1％，AP75提升2.2％。 作者还发现角点池化的作用对中大型物体检测更为关键，它们的AP分别提高了2.4％和3.7％。 从直观上分析, 由于中大型物体的最顶部，最底部，最左侧，最右边界可能更远离角落的位置,因而应用角点池化对于角点信息获取具有更好的优化作用。
##### Reducing penalty to negative locations:
本文减少了对于正位置由对象大小确定的半径内的负位置给出的惩罚. 为了理解这项设置有助于训练CornerNet，作者训练了一个没有减少惩罚项的网络和另一个固定惩罚半径为2.5的网络, 并在验证集上将它们与CornerNet进行了比较。
![](http://47.104.7.68/upload/mdfiles/20190220103405_19914.jpg)
表2中展示了固定的惩罚半径使基线上的AP增加了2.7％，$AP^m$增加了1.5％，$AP^1$增加了5.3％。而依赖于对象的惩罚半径设置进一步将AP提高了2.9％，$AP^m$提高了2.6％，$AP^1$提高了6.5％。除此之外我们也能看到:减轻惩罚项设置对于大中型物体的增益效果最为明显。
##### Error Analysis:
CornerNet同时输出heatmap，偏移量和嵌入向量，所有这些对与检测任务的性能都有着很大的影响。如果物体的任何一个角落在检测中被忽略，那么最终的检测结果也必将丢失整个物体; 精确的偏移量矩阵对于生成精准的边界框起到关键性作用; 错误的嵌入向量表示将导致边界框无法正确的匹配从而产生错误的边界框生成结果。为了理解每个部分对于最终误差的影响，作者通过将预测的heatmap和偏移量矩阵替换为ground-truth并在验证集上评估性能来进行误差分析。
![](http://47.104.7.68/upload/mdfiles/20190220103518_27397.jpg)
表3中展示了单独使用ground-truth的角点heatmap会将AP从38.5％提高到74.0％。 AP，$AP^m$和$AP^1$也分别增长43.1％，40.9％和30.1％。 如果我们用groud-truth偏移量矩阵替换预测的偏移量，则AP进一步提升了13.1％至87.1％。 这表明尽管网络在检测和分组角点上都存在着很大的改进空间，但最主要的性能瓶颈仍是检测角点问题。
图8展示了预测角点的两个定性示例。
![](http://47.104.7.68/upload/mdfiles/20190220103633_96992.jpg)
图8.通过预测的heatmap生成角点的边界框预测示例
##### Comparisons with state-of-the-art detectors
作者将CornerNet与MS COCO test-dev上的其他最先进的检测器进行了比较（表4）。 通过多尺度评估，CornerNet实现了42.1％的AP，获得了现有单阶段检测模型中最优的性能,并可以与双阶段模型相提并论.
![](http://47.104.7.68/upload/mdfiles/20190220103721_46761.jpg)
#### 五.结论:
本文介绍了一种新的目标检测方法CornerNet，通过将边界框定义为成对的角点完成检测任务. 本文在MS COCO数据集上对CornerNet进行了评估，并展示出有竞争力的结果。
References:
Bell, S., Lawrence Zitnick, C., Bala, K., Girshick, R.: Inside-outside net: Detecting objects in context with skip pooling and recurrent neural networks. In: Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition. pp. 2874{ 2883 (2016)
Bodla, N., Singh, B., Chellappa, R., Davis, L.S.: Soft-nmsimproving object detec- tion with one line of code. In: 2017 IEEE International Conference on Computer Vision (ICCV). pp. 5562{5570. IEEE (2017)
Cai, Z., Fan, Q., Feris, R.S., Vasconcelos, N.: A uni ed multi-scale deep convolu- tional neural network for fast object detection. In: European Conference on Com- puter Vision. pp. 354{370. Springer (2016)
Cai, Z., Vasconcelos, N.: Cascade r-cnn: Delving into high quality object detection. arXiv preprint arXiv:1712.00726 (2017)
Chen, Y., Li, J., Xiao, H., Jin, X., Yan, S., Feng, J.: Dual path networks. In: Advances in Neural Information Processing Systems. pp. 4470{4478 (2017)
Dai, J., Li, Y., He, K., Sun, J.: R-fcn: Object detection via region-based fully convolutional networks. arXiv preprint arXiv:1605.06409 (2016)
Dai, J., Qi, H., Xiong, Y., Li, Y., Zhang, G., Hu, H., Wei, Y.: Deformable convo- lutional networks. CoRR, abs/1703.06211 1(2), 3 (2017)
Deng, J., Dong, W., Socher, R., Li, L.J., Li, K., Fei-Fei, L.: Imagenet: A large-scale hierarchical image database. In: Computer Vision and Pattern Recognition, 2009. CVPR 2009. IEEE Conference on. pp. 248{255. IEEE (2009)
Everingham, M., Eslami, S.A., Van Gool, L., Williams, C.K., Winn, J., Zisserman, A.: The pascal visual object classes challenge: A retrospective. International journal of computer vision 111(1), 98{136 (2015)
Fu, C.Y., Liu, W., Ranga, A., Tyagi, A., Berg, A.C.: Dssd: Deconvolutional single shot detector. arXiv preprint arXiv:1701.06659 (2017)
Girshick, R.: Fast r-cnn. arXiv preprint arXiv:1504.08083 (2015)
Girshick, R., Donahue, J., Darrell, T., Malik, J.: Rich feature hierarchies for ac- curate object detection and semantic segmentation. In: Proceedings of the IEEE conference on computer vision and pattern recognition. pp. 580{587 (2014)
He, K., Gkioxari, G., Dollar, P., Girshick, R.: Mask r-cnn. arxiv preprint arxiv: 170306870 (2017)
He, K., Zhang, X., Ren, S., Sun, J.: Spatial pyramid pooling in deep convolutional networks for visual recognition. In: European Conference on Computer Vision. pp. 346{361. Springer (2014)
He, K., Zhang, X., Ren, S., Sun, J.: Deep residual learning for image recognition. In: Proceedings of the IEEE conference on computer vision and pattern recognition. pp. 770{778 (2016)
Huang, J., Rathod, V., Sun, C., Zhu, M., Korattikara, A., Fathi, A., Fischer, I., Wojna, Z., Song, Y., Guadarrama, S., et al.: Speed/accuracy trade-o s for modern convolutional object detectors. In: IEEE CVPR (2017)
Io e, S., Szegedy, C.: Batch normalization: Accelerating deep network training by reducing internal covariate shift. In: International conference on machine learning. pp. 448{456 (2015)
Kingma, D.P., Ba, J.: Adam: A method for stochastic optimization. arXiv preprint arXiv:1412.6980 (2014)
Kong, T., Sun, F., Yao, A., Liu, H., Lu, M., Chen, Y.: Ron: Reverse connection with objectness prior networks for object detection. arXiv preprint arXiv:1707.01691 (2017)
Krizhevsky, A., Sutskever, I., Hinton, G.E.: Imagenet classi cation with deep con- volutional neural networks. In: Advances in neural information processing systems. pp. 1097{1105 (2012)
Li, Z., Peng, C., Yu, G., Zhang, X., Deng, Y., Sun, J.: Light-head r-cnn: In defense of two-stage object detector. arXiv preprint arXiv:1711.07264 (2017)
Lin, T.Y., Dollar, P., Girshick, R., He, K., Hariharan, B., Belongie, S.: Feature pyramid networks for object detection. arXiv preprint arXiv:1612.03144 (2016)
Lin, T.Y., Goyal, P., Girshick, R., He, K., Dollar, P.: Focal loss for dense object detection. arXiv preprint arXiv:1708.02002 (2017)
Lin, T.Y., Maire, M., Belongie, S., Hays, J., Perona, P., Ramanan, D., Dollar, P., Zitnick, C.L.: Microsoft coco: Common objects in context. In: European conference on computer vision. pp. 740{755. Springer (2014)
Liu, W., Anguelov, D., Erhan, D., Szegedy, C., Reed, S., Fu, C.Y., Berg, A.C.: Ssd: Single shot multibox detector. In: European conference on computer vision. pp. 21{37. Springer (2016)
Newell, A., Deng, J.: Pixels to graphs by associative embedding. In: Advances in Neural Information Processing Systems. pp. 2168{2177 (2017)
Newell, A., Huang, Z., Deng, J.: Associative embedding: End-to-end learning for joint detection and grouping. In: Advances in Neural Information Processing Sys- tems. pp. 2274{2284 (2017)
Newell, A., Yang, K., Deng, J.: Stacked hourglass networks for human pose es- timation. In: European Conference on Computer Vision. pp. 483{499. Springer (2016)
Paszke, A., Gross, S., Chintala, S., Chanan, G., Yang, E., DeVito, Z., Lin, Z., Desmaison, A., Antiga, L., Lerer, A.: Automatic di erentiation in pytorch (2017)
Redmon, J., Divvala, S., Girshick, R., Farhadi, A.: You only look once: Uni ed, real-time object detection. In: Proceedings of the IEEE conference on computer vision and pattern recognition. pp. 779{788 (2016)
Redmon, J., Farhadi, A.: Yolo9000: better, faster, stronger. arXiv preprint 1612 (2016)
Ren, S., He, K., Girshick, R., Sun, J.: Faster r-cnn: Towards real-time object detec- tion with region proposal networks. In: Advances in neural information processing systems. pp. 91{99 (2015)
Shen, Z., Liu, Z., Li, J., Jiang, Y.G., Chen, Y., Xue, X.: Dsod: Learning deeply supervised object detectors from scratch. In: The IEEE International Conference on Computer Vision (ICCV). vol. 3, p. 7 (2017)
Shen, Z., Shi, H., Feris, R., Cao, L., Yan, S., Liu, D., Wang, X., Xue, X., Huang, T.S.: Learning object detectors from scratch with gated recurrent feature pyramids. arXiv preprint arXiv:1712.00886 (2017)
Shrivastava, A., Sukthankar, R., Malik, J., Gupta, A.: Beyond skip connections: Top-down modulation for object detection. arXiv preprint arXiv:1612.06851 (2016)
Simonyan, K., Zisserman, A.: Very deep convolutional networks for large-scale image recognition. arXiv preprint arXiv:1409.1556 (2014)
Singh, B., Davis, L.S.: An analysis of scale invariance in object detection-snip. arXiv preprint arXiv:1711.08189 (2017)
Szegedy, C., Io e, S., Vanhoucke, V., Alemi, A.A.: Inception-v4, inception-resnet and the impact of residual connections on learning. In: AAAI. vol. 4, p. 12 (2017)
Tychsen-Smith, L., Petersson, L.: Denet: Scalable real-time object detection with directed sparse sampling. arXiv preprint arXiv:1703.10295 (2017)
Tychsen-Smith, L., Petersson, L.: Improving object localization with tness nms and bounded iou loss. arXiv preprint arXiv:1711.00164 (2017)
Uijlings, J.R., van de Sande, K.E., Gevers, T., Smeulders, A.W.: Selective search for object recognition. International journal of computer vision 104(2), 154{171 (2013)
Xiang, Y., Choi, W., Lin, Y., Savarese, S.: Subcategory-aware convolutional neu- ral networks for object proposals and detection. arXiv preprint arXiv:1604.04693 (2016)
Xu, H., Lv, X., Wang, X., Ren, Z., Chellappa, R.: Deep regionlets for object de- tection. arXiv preprint arXiv:1712.02408 (2017)
Zhai, Y., Fu, J., Lu, Y., Li, H.: Feature selective networks for object detection. arXiv preprint arXiv:1711.08879 (2017)
Zhang, S.,Wen, L., Bian, X., Lei, Z., Li, S.Z.: Single-shot re nement neural network for object detection. arXiv preprint arXiv:1711.06897 (2017)
Zhu, Y., Zhao, C., Wang, J., Zhao, X., Wu, Y., Lu, H.: Couplenet: Coupling global structure with local parts for object detection. In: Proc. of Intl Conf. on Computer Vision (ICCV) (2017)
Zitnick, C.L., Dollar, P.: Edge boxes: Locating object proposals from edges. In: European Conference on Computer Vision. pp. 391{405. Springer (2014)



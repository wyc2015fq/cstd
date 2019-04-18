# 基于内容的图像检索技术综述-CNN方法 - 知乎
# 

本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
- [书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
- [书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)

传统方法在图像检索技术上一直表现平平。比如传统方法常用的SIFT特征，它对一定程度内的缩放、平移、旋转、视角改变、亮度调整等畸变，都具有不变性，是当时最重要的图像特征提取方法之一。然而SIFT这类算法提取的特征还是有局限性的，在ImageNet ILSVRC比赛的最好结果的错误率也有26%以上，而且常年难以产生突破。而图像检索的发展目标是希望模型又快又准，因此兴起了基于CNN的方法，从原来AlexNet、VGGnet，到体积小一点的Inception、Resnet系列，再到DenseNet系列无不体现出了这一趋势。和传统方法一样，CNN方法也是对图片提取特征，比如CNN网络中的一个feature map就可以看做是一个类似SIFT的向量。

计算机视觉比赛ILSVRC（[ImageNet Large Scale Visual Recognition Competition](https://link.zhihu.com/?target=http%3A//www.baidu.com/link%3Furl%3D9xfIoK_axaXkNPcVB3Tx1jSw-PVvEdDSxYWH_bcf4TC9p4Mmtu3aIPGrF6gF_TyTE-6mNNMLoy4l_Ywyp3Je7a%2522%2520%255Ct%2520%2522https%3A//www.baidu.com/_blank)）使用的数据都来自ImageNet，该项目于2007年由斯坦福大学华人教授李飞飞创办。ImageNet拥有1500万张标注过的高清图片，总共拥有22000类，其中约有100万张标注了图片中主要物体的定位边框。每年度的ILSVRC比赛数据集中大概拥有120万张图片，以及1000类的标注，是ImageNet全部数据的一个子集。比赛一般采用top-5和top-1分类错误率作为模型性能的评测指标。

和SIFT等算法类似，CNN训练的模型同样对缩放、平移、旋转等畸变具有不变性，有着很强的泛化性。CNN的最大特点在于卷积的权值共享结构，可以大幅减少神经网络的参数量，防止过拟合的同时又降低了神经网络模型的复杂度。

## 一、AlexNet

`论文：ImageNet Classification with Deep Convolutional Neural Networks1[1]`

AlexNet是2012年ImageNet竞赛冠军获得者Hinton和他的学生Alex Krizhevsky设计的。也是在那年之后，更多的更深的神经网路被提出，比如优秀的vgg，GoogleLeNet。其官方提供的数据模型，准确率达到57.1%，这项对于传统的机器学习分类算法而言，已经相当的出色。

早些的时候，为了打破网络对称性和提高学习能力，传统的网络都使用了随机稀疏连接。但是，计算机软硬件对非均匀稀疏数据的计算效率很差，所以在AlexNet中又重新启用了全连接层，目的是为了更好地优化并行运算。

AlexNet包含了6亿3000万个连接，6000万个参数和65万个神经元，拥有5个卷积层，其中3个卷积层后面连接了最大池化层，最后还有3个全连接层，图中可以看到，AlexNet还用了11×11和5×5的大卷积核，具体的网络参数可以参考这里：[https://blog.csdn.net/guoyunfei20/article/details/78122504](https://link.zhihu.com/?target=https%3A//blog.csdn.net/guoyunfei20/article/details/78122504)，网络结构如下图所示：
![](https://pic4.zhimg.com/v2-e4a04582170c7e2f787325f1ec454e8f_b.jpg)图1 AlexNet网路结构
## 二、VGG(Visual Geometry Group)

`论文：Very deep convolutional networks for large-scale image recognition[2]`

图片的预处理就是每一个像素减去了均值，算是比较简单的处理。然后缩放图像最小边到256或者384，然后裁剪得到输入数据是224*224进行训练。网络有5个最大池化层，整体使用的卷积核都比较小（3x3），3x3是可以表示「左右」、「上下」、「中心」这些模式的最小单元了，VGG16的网络缩略图如下所示：
![](https://pic1.zhimg.com/v2-dfe4eaaa4450e2b58b38c5fe82f918c0_b.jpg)图2 VGG16网络结构图
VGG是最早利用2个3×3卷积核的组合代替1个5×5卷积核的网络，使用多个较小卷积核代替一个较大的卷积核，一方面可以减少参数，另一方面作者认为相当于是进行了更多的非线性映射，可以增加网络的拟合/表达能力。还有比较特殊的1x1的卷积核（Inception-v1也有这个东西），可看做是空间的线性映射。前面几层是卷积层的堆叠，后面几层是全连接层，最后是softmax层。所有隐层的激活单元都是ReLU，论文中介绍好几个网络结构，只有其中一个应用了局部响应归一化层（Local Response Normalisation）。
![](https://pic2.zhimg.com/v2-228e1243e1ae06eda021200f32dd7535_b.jpg)图3 A-E的VGG网路结构对比
上图列出了6种VGG网络结构图，其中VGG16和VGG19较为常见，也是性能最好的两个网络结构，下面是对A-E的网络性能进行分析：

> A与A-LRN比较：A-LRN结果没有A好，说明LRN作用不大；
A与B, C, D, E比较，A是这当中layer最少的，相比之下A效果不如B,C,D,E，说明Layer越深越好；
B与C比较：增加1x1filter，增加了额外的非线性提升效果；
C与D比较：3x3 的filter（结构D）比1x1（结构C）的效果好。

总结了下VGG网络的结构，可以发现以下趋势：

> ①可以看到VGG网络中共有5个池化层，所以可以把卷积部分视为5个部分，和AlexNet一样，只不过每一个部分他用了不止一层卷积层；
②所有卷积层都是同样大小的filter！尺寸3x3，卷积步长Stirde = 1，填充Padding = 1； 
③卷积层变多了。结构E有16层卷积层，加上全连接层共19层。这也是对深度学习继续往深处走的一个推动。因为深度神经网络的参数特别多（可以达到上亿，目前已经可以支持到万亿参数）。参数多，表示模型的搜索空间就越大，必须有足够的数据才能更好地刻画出模型在空间上的分布。
![](https://pic3.zhimg.com/v2-f422769a58f2978c5f3f9ac2af6945d6_b.jpg)图4 VGG网络性能比较
## 三、Incepetion V1

`论文：Going deeper with convolutions[3]`

在** ILSVRC 2014** 取得了最好的成绩的 **GoogLeNet**，及其核心结构是**Inception，**传统的网络中，计算机软硬件对非均匀稀疏数据的计算效率很差，故AlexNet又重新启用了全连接层，目的是为了更好地优化并行运算。所以，现在考虑有没有一种**既能保持网络结构的稀疏性，又能利用密集矩阵的高计算性能的方法**。大量的文献表明可以将稀疏矩阵聚类为较为密集的子矩阵来提高计算性能，据此论文提出了名为Inception 的结构来实现此目的。

Incepetion V1总共有22层layers，只有500万的参数量，仅为AlexNet参数量（6000万）的1/12，却可以达到远胜于AlexNet的准确率，可以说是非常优秀并且非常实用的模型。

Incepetion V1有一个重要的特点：每层使用多个卷积核。传统的层叠式网络，基本上都是一个个卷积层的堆叠，每层只用一个尺寸的卷积核，例如VGG结构中使用了大量的3×3卷积层。事实上，同一层feature map可以分别使用多个不同尺寸的卷积核，以获得不同尺度的特征，再把这些特征结合起来，得到的特征往往比使用单一卷积核的要好，谷歌的GoogleNet，或者说Inception系列的网络，就使用了多个卷积核的结构：
![](https://pic1.zhimg.com/v2-caf57b7a554021abaa75721d640ab24c_b.jpg)图5 Incepetion V1的多尺度卷积层
如果仅仅引入多个尺寸的卷积核，会带来大量的额外的参数，受到Network In Network中1×1卷积核的启发，为了解决这个问题，他们往Inception结构中加入了一些1×1的卷积核，如图所示：
![](https://pic2.zhimg.com/v2-824ee11452d6dd524a314772b458cd11_b.jpg)图6 Incepetion V1引入1×1卷积核
1×1的卷积核能降低计算量，如下图所示，左图表示引入1×1的卷积核之前的网络，其参数量为256×3×3×256=589824，右图表示引入1×1的卷积核之后的网络，其参数量为256×1×1×64+64×3×3×256+64×1×1×256=69632，参数量几乎减少了一个数量级。1×1卷积核也被认为是影响深远的操作，往后大型的网络为了降低参数量都会应用上1×1卷积核，引入1×1的卷积核能达到降维和升维的目的。

![](https://pic4.zhimg.com/v2-e922c950dd3ad160eaa722cfcafe203b_b.jpg)图7 引入1×1卷积核前后对比
**四、Incepetion V2**

论文：Batch Normalization：Accelerating Deep Network Training by Reducing Internal Covariate Shift

下面的准则来源于大量的实验，因此包含一定的推测，但实际证明基本都是有效的：

1 . 避免表达瓶颈，特别是在网络靠前的地方。 信息流前向传播过程中显然不能经过高度压缩的层，即表达瓶颈。从input到output，feature map基本都会逐渐变小，但是一下子就变得很小显然不合适。 另外输出的维度channel，一般来说会逐渐增多，否则网络会很难训练。（特征维度并不代表信息的多少，只是作为一种估计的手段）

Inception V2 学习了 VGG 用两个3×3的卷积代替一个5×5的大卷积，在降低参数的同时建立了更多的非线性变换，使得 CNN 对特征的学习能力更强：
![](https://pic4.zhimg.com/v2-c6607520a745dabbee275aba9ac1fdeb_b.jpg)图8 替换5×5的卷积核
另外Inception V2还提出了著名的 **Batch Normalization**（简称BN）方法。BN 是一个非常有效的正则化方法，可以让大型卷积网络的训练速度加快很多倍，同时收敛后的分类准确率也可以得到大幅提高。BN 在用于神经网络某层时，会对每一个 mini-batch 数据的内部进行标准化处理，使输出规范化到 N(0,1) 的正态分布，减少了内部神经元分布改变的影响。

其相对于传统神经网络的差别是：传统神经网络只是在将样本输入层之前对样本进行标准化处理（减均值，除标准差），以降低样本间的差异性。BN是在此基础上，不仅仅只对输入进行标准化，还对每个隐藏层的输入进行标准化。

BN 的论文指出，传统的深度神经网络在训练时，每一层的输入的分布都在变化，导致训练变得困难，我们只能使用一个很小的学习速率解决这个问题。而对每一层使用 BN 之后，我们就可以有效地解决这个问题，学习速率可以增大很多倍，达到之前的准确率所需要的迭代次数只有之前的1/14，训练时间大大缩短。而达到之前的准确率后，可以继续训练，并最终取得远超于 Inception V1 模型的性能： top-5 错误率 **4.8%**，远低于Inception V1的6.7%，已经优于人眼水平。因为 BN 某种意义上还起到了正则化的作用，所以可以减少或者取消 **Dropout** 和 **LRN**，简化网络结构。

## **五、Incepetion V3**

论文：Rethinking the Inception Architecture for Computer Vision[5]

一是引入了 **Factorization into small convolutions** 的思想，将一个较大的二维卷积拆成两个较小的一维卷积，比如将7x7卷积拆成1x7卷积和7x1卷积，或者将3x3卷积拆成1x3卷积和3x1卷积，另外也使用了将5x5 用两个 3x3 卷积替换，7x7 用三个 3x3 卷积替换，如下图所示。一方面节约了大量参数，加速运算并减轻了过拟合，同时增加了一层非线性扩展模型表达能力。论文中指出，这种非对称的卷积结构拆分，其结果比对称地拆为几个相同的小卷积核效果更明显，可以处理更多、更丰富的空间特征，增加特征多样性。

另一方面，Inception V3 优化了 Inception Module 的结构，网络输入从224x224变为了299x299，现在 Inception Module 有35×35、17×17和8×8三种不同结构。这些 Inception Module 只在网络的后部出现，前部还是普通的卷积层。并且 Inception V3 除了在 Inception Module 中使用分支，还在分支中使用了分支（8×8的结构中），可以说是Network In Network In Network。最终取得 top-5 错误率 **3.5%的效果**。
![](https://pic3.zhimg.com/v2-7e559c566e8a0e06ab190bfda354fd0e_b.jpg)图9 使用一维卷积核代替二维卷积核![](https://pic1.zhimg.com/v2-5c3a8513e786c513523a1415ac934c1c_b.jpg)图10 使用1×3代替3×3卷积核
## **六、Incepetion V4**

`论文：Inception v4-Inception-ResNet and the Impact of Residual Connections on Learning[6]`

Inception V4相比V3主要是结合了微软的 **ResNet**，将错误率进一步减少到 **3.08%**。V4研究了Inception模块结合Residual Connection能不能有改进，发现ResNet的结构可以极大地加速训练，同时性能也有提升，得到一个Inception-ResNet v2网络，同时还设计了一个更深更优化的Inception V4模型，能达到与Inception-ResNet v2相媲美的性能，inception V4总体的网络结构为：
![](https://pic3.zhimg.com/v2-597595fc10f466d70331dfeb3a30eada_b.jpg)图11 Inception v4总体结构和Stem部分
其他部分的结构如下：

![](https://pic3.zhimg.com/v2-92855a93f0cd2f10c6cf741ed6daad0e_b.jpg)图12 Inception A![](https://pic2.zhimg.com/v2-3ac1cf4ea040a3139748a0b7aefed4f9_b.jpg)图13 Inception B![](https://pic1.zhimg.com/v2-4a526d612379634af706af85b16fa104_b.jpg)图14 Inception C![](https://pic3.zhimg.com/v2-4658e6afa210b6d336d8b7321a7f2112_b.jpg)图15 Reduction A![](https://pic3.zhimg.com/v2-95a8a42d5eff532ec4d88a74764443b6_b.jpg)图16 Reduction B
同时作者在该篇论文中还提出了Inception-ResNet-v1和Inception-ResNet-v2。Inception-V4是纯粹的Inception变体，没有剩余连接，具有与Inception-ResNet-v2大致相同的识别性能。几种网络在Single crop - single下的识别结果为：
![](https://pic3.zhimg.com/v2-7c57b4be8d2549730659e388306663f6_b.jpg)图17 Inception-V4性能对比
**七、ResNet**

论文：Deep residual learning for image recognition[7]

如果说 googlLeNet、AlexNet、VGG 奠定了经典神经网络的基础，Inception 和ResNet 则展示了神经网络的新范式，在这两个范式的基础上，发展创新并相互借鉴，有了 Inception 流派的 Inception v2 到 v4、Inception-ResNet v1 和 v2，以及 ResNet 流派的DenseNet等。

ResNet的输入是在图片上随机裁剪224×224的像素块，网络中没有采用dropout。
**![](https://pic2.zhimg.com/v2-871d47a94dd2c3427509da8f8c163ee5_b.jpg)图18 Residual网络模型**
随着网络的加深，出现了训练集准确率下降的现象，我们可以确定这不是由于Overfit过拟合造成的(过拟合的情况训练集应该准确率很高)；所以作者针对这个问题提出了一种全新的网络，叫深度残差网络，它允许网络尽可能地加深，其中引入了全新的结构如上图所示。 
    其中ResNet提出了两种mapping：一种是identity mapping(恒等映射)，指的就是上图中弯弯的曲线，另一种residual mapping，指的就是除了曲线外的那部分，所以最后的输出是 y=F(x)+x，identity mapping顾名思义，就是指本身，也就是公式中的x，而residual mapping指的是“差”，也就是y−x，所以残差指的就是F(x)部分。 如果网络已经到达最优，继续加深网络，residual mapping将被push为0，只剩下identity mapping，这样理论上网络一直处于最优状态了，网络的性能也就不会随着深度增加而降低了，但实际上，identify mapping一般不是最优的。

这其中要注意一个问题，就是在y=F(x)+x的时候要保证F(x)和x的维度相同，若他们的维度不同，就需要对x乘一个对应的权重，即：y=F(x)+Ws*x，以此来保证F(x)和x具有相同的维度。用来学习残差的网络层数应当大于1，否则退化为线性。文章实验了layers = 2或3，更多的层也是可行的。

ResNet在运算效率的准确率方面都有所提升，比如ResNet34的运算量只有VGG19的18%，VGG19需要进行196亿次浮点运算，而ResNet34只需要进行36亿次浮点运算，效率大大提升，下图从左到右依次为VGG19、plain-networks34和ResNet34的结构图：
![](https://pic2.zhimg.com/v2-58deeabc2ea82d91dad28db1a8161285_b.jpg)图19 VGG19、plain-networks34和ResNet34结构图
对于plain-networks，随着网络层数的加深，错误率反而会上升，因为增加深度会带来网络退化问题，使训练集上的准确率饱和甚至下降，并且深层网络容易产生梯度消失或爆炸，而加了identity的网络就不会存在这样的问题，因为残差网络更容易优化，能够通过单纯地增加网络深度，来提高网络性能。

为什么网络的加深很重要呢？因为CNN能够提取low/mid/high-level的特征，网络的层数越多，意味着能够提取到不同level的特征越丰富。并且，越深的网络提取的特征越抽象，越具有语义信息。

## **八、DenseNet**

`论文：Densely Connected Convolutional Networks[8]`

DenseNet的思想启发来源：

DenseNet 的想法很大程度上源于作者2016年发表在 ECCV 上的一个叫做随机深度网络（Deep networks with stochastic depth）工作。当时作者提出了一种类似于 Dropout 的方法来改进ResNet。发现在训练过程中的每一步都随机地扔掉（drop）一些层，可以显著的提高 ResNet 的泛化性能。这个方法的成功带来以下两点启发：

1)首先，它说明了神经网络其实并不一定要是一个递进层级结构，也就是说网络中的某一层可以不仅仅依赖于紧邻的上一层的特征，还可以依赖于更前面层学习的特征。想像一下在随机深度网络中，当第 l 层被扔掉之后，第 L+1 层就被直接连到了第 L-1 层；当第 2 到了第 L 层都被扔掉之后，第 L+1 层就直接用到了第 1 层的特征。因此，随机深度网络其实可以看成一个具有随机密集连接的 DenseNet。

2)其次，在训练的过程中随机扔掉很多层也不会破坏算法的收敛，说明了 ResNet 具有比较明显的冗余性，网络中的每一层都只提取了很少的特征（即所谓的残差）。实际上，将训练好的 ResNet 随机的去掉几层，对网络的预测结果也不会产生太大的影响。既然每一层学习的特征这么少，能不能降低它的计算量来减小冗余呢？

DenseNet 的设计正是基于以上两点观察。让网络中的每一层都直接与其前面层相连，实现特征的重复利用；同时把网络的每一层设计得特别窄，即只学习非常少的特征图（最极端情况就是每一层只学习一个特征图），达到降低冗余性的目的。这两点也是 DenseNet 与其他网络最主要的不同。需要强调的是，第一点是第二点的前提，没有密集连接，就不可能把网络设计得太窄，否则训练会出现欠拟合现象，即使 ResNet 也是如此。
![](https://pic2.zhimg.com/v2-5357fec489793db6126efb4d16fa8b29_b.jpg)图20 Dense Block模拟图
上图是一个详细的Dense Block的模拟，其中层数为5，即具有5个BN+Relu+Conv(3*3)这样的layer，网络增长率k为4，简单的说就是每一个layer输出的feature map的维度为k。即：第L层网络的输入为k × (L- 1) + k0 ，这里的k0为原始输入的通道数，然后把k × (L- 1) + k0降维为k，再把这个k加上前面所有的连接作为L+1层的输入，即第L+1层网络的输入为k× L + k0。由于DenseNet的每一个Dense Block模块都利用到了该模块中前面所有层的信息，即每一个layer都和前面的layer有highway的稠密连接。假设一个具有L层的网络，那么highway稠密连接数目为L*(L+1)/2。和Resnet不同的是，这里的连接方式得到的feature map做的是concat操作，而resnet中做的是elementwise操作。

现在回到论文上，作者在论文中提出的DenseNet的网络结构为：

> 1)三个block，每一个block具有相等个数的layers，如下图所示；
2)所有卷积层都使用3×3的卷积核；
3)用zeros-padding和2×2的average pooling来调整feature map的大小使之固定到同一尺寸；
4)最后一个block的输出后面接一个全局average pooling和一个softmax 分类器；
5)feature map 的尺寸在三个block里面分别为32× 32、16×16、8×8；
6)网络增长率k和block里面的layers层数有两种方案：k = 12，L=40, 以及k = 24，L=100。
![](https://pic2.zhimg.com/v2-4b29bbb61d29ce88cc681ab39710f9f9_b.jpg)图21 DenseNet的三个block
DenseNet在识别率上的表现也非常不错，在同一识别率的情况下，DenseNet的参数复杂度约为ResNet的一半。
![](https://pic4.zhimg.com/v2-4f6e71ede65f7bc67a8f8c57904c3d7f_b.jpg)图22 DenseNet和ResNet的参数复杂度对比
DenseNet计算量小的原因：DenseNet和ResNet相比的一个优点是设置很小的k值，使得网络更窄从而参数更少。在 dense block中每个卷积层的输出feature map的数量k都很小（小于100），而不是像其他网络一样动不动就几百上千的宽度。另外论文还观察到这种denseblock有正则化的效果，因此对于过拟合有一定的抑制作用，可能是因为网络通道更窄使得参数减少了，所以过拟合现象减轻。

## **九、SENet**

`论文：Squeeze-and-Excitation Networks[9]`

SENet是基于特征通道之间的关系提出的，下图是SENet的Block单元，图中的Ftr是传统的卷积结构，X和U是Ftr的输入和输出，这些都是以往结构中已存在的。SENet增加的部分是U后的结构：对U先做一个Global Average Pooling（称为Squeeze过程），输出是一个1x1xC的数据，再经过两级全连接（称为Excitation过程），最后用sigmoid把输出限制到[0，1]的范围，把这个值作为scale再乘到U的C个通道上，作为下一级的输入数据。这种结构的原理是想通过控制scale的大小，把重要的特征增强，不重要的特征减弱，从而让提取的特征指向性更强。
![](https://pic4.zhimg.com/v2-b5f405ae9e7450943dc2923714710a4f_b.jpg)图23 SENet的Squeeze-Excitation block
下图是把SENet模型分别用于Inception网络和ResNet网络，下图左边部分是原始网络，右边部分是加了SENet之后的网络，分别变成SE-Inception和SE-ResNet。网络中的r是压缩参数，先通过第一个全连接层把1x1xC的数据压缩为1x1xC/r，再通过第二个全连接层把数据扩展到1x1xC，
![](https://pic2.zhimg.com/v2-bfefc3a5e6e97cd18e8ee682a3e19b0d_b.jpg)图24 SE-Inception![](https://pic3.zhimg.com/v2-36709d386d3ca13edcf1766a41dc718a_b.jpg)图25 SE-ResNet
## **十、**netVLAD

相对于传统的人工设计特征，CNN已经在图像特征提取上显示出了强大的功力。在图像检索问题中，目前有基于全局和基于局部两种卷积神经网络特征表示方法。基于全局的方法直接使用卷积神经网络提取整幅图像的特征，作为最终的图像特征。但是因为卷积神经网络主要对全局空间信息进行编码，导致所得特征缺乏对图像的尺度、旋转、平移等几何变换和空间布局变化的不变性，限制了其对于高度易变图像检索的鲁棒性。对于基于局部的方法，使用卷积神经网络提取图像局部区域的特征（这里的局部区域特征好比经典方法的SIFT特征），然后聚合这些区域特征生成最终的图像特征。虽然这些方法考虑到了图像的局部信息，但仍有一些缺陷。例如使用滑动窗口来得到图像区域时，由于没有考虑到图像的颜色、纹理、边缘等视觉内容，会产生大量无语义意义的区域，为之后的聚合过程带来冗余和噪声信息。另外，区域特征融合通常所使用的最大池化算法，因只保留了特征的最大响应而没有考虑特征间的关联，丢失大量信息，降低了所得的最终图像特征的区分性。

因此有学者提出netVLAD的方法。首先去掉CNN的最后一层，把它作为描述子，输出是 H×W×D 的向量，可以将其看作一个D维的描述子，总共有 H×W 个。相当于在整个图像上提取H×W 个D维描述子，然后对这H×W 个D维的描述子进行VLAD聚类，可以看做在CNN网络后面直接接一个netVLAD网络，如下图所示：
![](https://pic2.zhimg.com/v2-be604ab608e8c5b03a5c7b8d18603cf9_b.jpg)图26 CNN接netVLAD网络
还有学者提出基于对象的方法来解决以上问题。在生成图像区域时，使用基于内容的无监督对象生成方法，即通过图像颜色、纹理、边缘等视觉信息以聚类的方式来生成图像区域。常用方法有Selective search，如下图所示，Selective search在一张图片中提取1000－10000个bounding box，使之尽可能覆盖所有的物体，试验也证明，它的覆盖率能达到96%以上，足以提取丰富的局部区域特征。其特征描述也具有更高的区分性，同时基于对象特征进行融合，所得最终特征对场景中对象的空间布局变化也具有很好的鲁棒性。在聚合特征的过程时，采用上篇提到的VLAD算法，先将图像的局部区域特征进行聚类，然后统计一幅图像中所有区域特征与其相近聚类中心的累积残差来表示最终的图像特征。相对于最大池化算法，该方法考虑了区域特征间关联的同时对图像的局部信息有更细致的刻画，使得得到的最终图像特征对各类图像变换具有更高鲁棒性。
![](https://pic4.zhimg.com/v2-ad4fc752e782c10540d3b192385c7af3_b.jpg)图27 Selective search示意图
## **十一、Loss函数优化**

在传统的分类网络中，一般是对大类如猫、狗、鸟等类别进行分类，但其对于个体级别的细粒度识别上略显不足，而tripletloss和contrastive loss等算法就更注重于高细粒度同类样本和不同样本之间的损失计算。

（一）triplet loss

triplet loss具有以下优秀属性：端到端、简单直接、自带聚类属性、特征高度嵌入，triplet loss的具体操作步骤为：

首先从数据集N中随机取一个样本，记为 ![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D) ，然后再取一个同类样本Positive记为 ![x_{p}](https://www.zhihu.com/equation?tex=x_%7Bp%7D) ，和一个不同类样本Negative记为 ![x_{n}](https://www.zhihu.com/equation?tex=x_%7Bn%7D) 。![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D)、![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D)和![x_{n}](https://www.zhihu.com/equation?tex=x_%7Bn%7D)就构成了一个三元组，我们把三个样本的特征分别记为： ![f(x_{i}^{a}),f(x_{i}^{p})](https://www.zhihu.com/equation?tex=f%28x_%7Bi%7D%5E%7Ba%7D%29%2Cf%28x_%7Bi%7D%5E%7Bp%7D%29) 和 ![f(x_{i}^{n})](https://www.zhihu.com/equation?tex=f%28x_%7Bi%7D%5E%7Bn%7D%29) 。triplet loss的目的就是通过学习使![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D)和 ![x_{p}](https://www.zhihu.com/equation?tex=x_%7Bp%7D) 特征之间的距离尽可能小，使![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D)和![x_{n}](https://www.zhihu.com/equation?tex=x_%7Bn%7D)特征之间的距离尽可能大，并且设置一个阈值t，使得![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D)、![x_{p}](https://www.zhihu.com/equation?tex=x_%7Bp%7D)距离和![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D)、![x_{n}](https://www.zhihu.com/equation?tex=x_%7Bn%7D)距离的差值大于该阈值，即：
![](https://pic4.zhimg.com/v2-ca47c31b005b25dd042b466d4a0fcb97_b.jpg)
对应的损失函数为：
![](https://pic4.zhimg.com/v2-f1db8758777a87ae399bebacfbee4183_b.jpg)
损失函数的含义为：当![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D)与![x_{n}](https://www.zhihu.com/equation?tex=x_%7Bn%7D)之间的距离 小于 ![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D)与![x_{p}](https://www.zhihu.com/equation?tex=x_%7Bp%7D)之间的距离加t时，会产生大于0的loss，当![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D)与![x_{n}](https://www.zhihu.com/equation?tex=x_%7Bn%7D)之间的距离大于![x_{a}](https://www.zhihu.com/equation?tex=x_%7Ba%7D)与![x_{p}](https://www.zhihu.com/equation?tex=x_%7Bp%7D)之间的距离加t时，上式中括号内的值小于0，loss按0计算。

训练的额过程可以用下图表示：
![](https://pic2.zhimg.com/v2-c4b504b1fa9f1eda30a15cfde76a93ad_b.jpg)图28 triplet loss训练示意图
（二）Contrastive Loss

contrastive loss的表达式如下：
![](https://pic3.zhimg.com/v2-7f5d24178e5f38ed7c4675d05574408e_b.jpg)

其中 ![d=||a_{n}-b_{n}||_{2}](https://www.zhihu.com/equation?tex=d%3D%7C%7Ca_%7Bn%7D-b_%7Bn%7D%7C%7C_%7B2%7D) ，代表两个样本特征的欧氏距离，y为两个样本是否匹配的标签，y=1代表两个样本相匹配，y=0则代表不匹配，margin为设定的阈值。观察可发现，当样本匹配时y=1，loss变为 ![\frac{1}{2N}](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7B2N%7D)![\sum_{n=1}^{N}{d^{2}}](https://www.zhihu.com/equation?tex=%5Csum_%7Bn%3D1%7D%5E%7BN%7D%7Bd%5E%7B2%7D%7D) ，此时若d比较大说明模型不好，因此loss也会相应地变大。当样本不匹配时y=0，loss变为![\frac{1}{2N}](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7B2N%7D)![\sum_{n=1}^{N}{(margin-d,0)^{2}}](https://www.zhihu.com/equation?tex=%5Csum_%7Bn%3D1%7D%5E%7BN%7D%7B%28margin-d%2C0%29%5E%7B2%7D%7D)，此时若d反而小，也说明模型不好，loss会相应地变大。

## **十二、特征的哈希变换-CNNH[10]**

由于各分类网络直接输出的output特征维度比较高并且都是浮点数，所以该类特征会占用比较大的存储空间，特别是对于海量图搜的问题，存储这么多的图片特征就显得更为费劲，而且读取这些特征并进行特征间距离计算的效率会非常低，所以精简分类网络的output特征就显得尤为重要，因此有学者提出了图片特征的哈希变换。

哈希学习凭借着检索速度快和存储成本低的优点，己经成为图像检索领域最受欢迎和有效的技术之一。但是现有的大部分哈希学习方法都是基于手工提取特征，手工提取特征不一定适用于哈希编码学习，所以有学者提出了深度学习的哈希方法CNNH。

首先对于给定的n张图片 ![I =（I_{1},I_{2},...,I_{n}）](https://www.zhihu.com/equation?tex=I+%3D%EF%BC%88I_%7B1%7D%2CI_%7B2%7D%2C...%2CI_%7Bn%7D%EF%BC%89) ，定义他们的相似矩阵为 ![s_{ij}](https://www.zhihu.com/equation?tex=s_%7Bij%7D) ，如果 ![I_{i}](https://www.zhihu.com/equation?tex=I_%7Bi%7D) 和

![I_{j}](https://www.zhihu.com/equation?tex=I_%7Bj%7D) 相似，则![s_{ij}](https://www.zhihu.com/equation?tex=s_%7Bij%7D)的值为1，若他们不相似，则![s_{ij}](https://www.zhihu.com/equation?tex=s_%7Bij%7D)的值为-1。接下来定义一个n行q列的二进制矩阵H，矩阵的第k行 ![H_{k}](https://www.zhihu.com/equation?tex=H_%7Bk%7D) 表示图片 ![I_{k}](https://www.zhihu.com/equation?tex=I_%7Bk%7D) 的二进制特征编码，![H_{k}](https://www.zhihu.com/equation?tex=H_%7Bk%7D)![\in ( -1,1)^{q}](https://www.zhihu.com/equation?tex=%5Cin+%28+-1%2C1%29%5E%7Bq%7D) 。图片I的哈希码通过学习得到，在学习中使得下列loss取得最小值：
![](https://pic2.zhimg.com/v2-3c1ab1e86e4b3c056ea84d0fefbe6c95_b.jpg)
在得到图片的哈希码后通过测量特征之间的汉明距离比较图片相似度。

参考文献
- Krizhevsky A, Sutskever I, Hinton G E. ImageNet classification with deep convolutional neural networks[C]// International Conference on Neural Information Processing Systems. Curran Associates Inc. 2012:1097-1105.
- Simonyan K, Zisserman A. Very Deep Convolutional Networks for Large-Scale Image Recognition[J]. Computer Science, 2014.
- Szegedy C, Liu W, Jia Y, et al. Going deeper with convolutions[J]. 2014:1-9.
- Szegedy C, Ioffe S, Vanhoucke V, et al. Inception-v4, Inception-ResNet and the Impact of Residual Connections on Learning[J]. 2016.
- Szegedy C, Vanhoucke V, Ioffe S, et al. Rethinking the Inception Architecture for Computer Vision[J]. 2015:2818-2826.
- Szegedy C, Ioffe S, Vanhoucke V, et al. Inception-v4, Inception-ResNet and the Impact of Residual Connections on Learning[J]. 2016.
- He K, Zhang X, Ren S, et al. Deep Residual Learning for Image Recognition[J]. 2015:770-778.
- Huang G, Liu Z, Laurens V D M, et al. Densely Connected Convolutional Networks[J]. 2016:2261-2269.
- Hu J, Shen L, Sun G. Squeeze-and-Excitation Networks[J]. 2017.
- Xia R, Pan Y, Lai H, et al. Supervised hashing for image retrieval via image representation learning[C]// AAAI Conference on Artificial Intelligence. 2014.


推荐阅读

[1][机器学习-波澜壮阔40年](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483705%26idx%3D1%26sn%3Dc6e7c4a2e14a2469308b41eb60f155ac%26chksm%3Dfdb69caecac115b8712653600e526e99a3f6976fdaa2f6b6a09388fa6f9677ccb57b40c40ae3%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0413.

[2][学好机器学习需要哪些数学知识？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483713%26idx%3D1%26sn%3D1e7c81381d16806ac73e15691fe17aec%26chksm%3Dfdb69cd6cac115c05f1f90b0407e3f8ae9be8719e454f908074ac0d079885b5c134e2d60fd64%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0417.

[3] [人脸识别算法演化史](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483726%26idx%3D1%26sn%3D9fef4cc1766ea4258749f8d40cc71a6e%26chksm%3Dfdb69cd9cac115cf4eba16081780c3b64c75e1e55a40bf2782783d5c28f00c6f143426e6f0aa%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0420.

[4][基于深度学习的目标检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483731%26idx%3D1%26sn%3D237c52bc9ddfe65779b73ef8b5507f3c%26chksm%3Dfdb69cc4cac115d2ca505e0deb975960a792a0106a5314ffe3052f8e02a75c9fef458fd3aca2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0424.

[5][卷积神经网络为什么能够称霸计算机视觉领域？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483816%26idx%3D1%26sn%3Dfc52765b012771d4736c9be4109f910e%26chksm%3Dfdb69c3fcac115290020c3dd0d677d987086a031c1bde3429339bb3b5bbc0aa154e76325c225%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0426.

[6] [用一张图理解SVM的脉络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483937%26idx%3D1%26sn%3D84a5acf12e96727b13fd7d456c414c12%26chksm%3Dfdb69fb6cac116a02dc68d948958ee731a4ae2b6c3d81196822b665224d9dab21d0f2fccb329%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0428.

[7] [人脸检测算法综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483950%26idx%3D1%26sn%3Da3a5b7907b2552c233f654a529931776%26chksm%3Dfdb69fb9cac116af5dd237cf987e56d12b0d2e54c5c565aab752f3e366c0c45bfefa76f5ed16%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0503.

[8] [理解神经网络的激活函数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247483977%26idx%3D1%26sn%3D401b211bf72bc70f733d6ac90f7352cc%26chksm%3Dfdb69fdecac116c81aad9e5adae42142d67f50258106f501af07dc651d2c1473c52fad8678c3%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI2018.5.5.

[9] [深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484037%26idx%3D1%26sn%3D13ad0d521b6a3578ff031e14950b41f4%26chksm%3Dfdb69f12cac11604a42ccb37913c56001a11c65a8d1125c4a9aeba1aed570a751cb400d276b6%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0508.

[10] [理解梯度下降法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484111%26idx%3D1%26sn%3D4ed4480e849298a0aff828611e18f1a8%26chksm%3Dfdb69f58cac1164e844726bd429862eb7b38d22509eb4d1826eb851036460cb7ca5a8de7b9bb%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0511.

[11] [循环神经网络综述—语音识别与自然语言处理的利器](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484310%26idx%3D1%26sn%3D0fc55a2784a894100a1ae64d7dbfa23d%26chksm%3Dfdb69e01cac1171758cb021fc8779952e55de41032a66ee5417bd3e826bf703247e243654bd0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0515

[12] [理解凸优化](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484439%26idx%3D1%26sn%3D4fa8c71ae9cb777d6e97ebd0dd8672e7%26chksm%3Dfdb69980cac110960e08c63061e0719a8dc7945606eeef460404dc2eb21b4f5bdb434fb56f92%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】 SIGAI0518

[13] [【实验】理解SVM的核函数和参数](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484495%26idx%3D1%26sn%3D4f3a6ce21cdd1a048e402ed05c9ead91%26chksm%3Dfdb699d8cac110ce53f4fc5e417e107f839059cb76d3cbf640c6f56620f90f8fb4e7f6ee02f9%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0522

[14][【SIGAI综述】行人检测算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484523%26idx%3D1%26sn%3Dddaa70c4b92f6005d9bbd6b3a3fe4571%26chksm%3Dfdb699fccac110ea14e6adeb873a00d6ee86dd4145ddf8e90c9b878b908ac7b7655cfa51dab6%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0525

[15] [机器学习在自动驾驶中的应用—以百度阿波罗平台为例(上)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484540%26idx%3D1%26sn%3D733332042c31e1e18ad800f7f527893b%26chksm%3Dfdb699ebcac110fd6607c1c99bc7ebed1594a8d00bc454b63d7f518191bd72274f7e61ca5187%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0529

[16][理解牛顿法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484651%26idx%3D1%26sn%3Da0e4ca5edb868fe3eae9101b71dd7103%26chksm%3Dfdb6997ccac1106a61f51fe9f8fd532045cc5d13f6c75c2cbbf1a7c94c58bcdf5f2a6661facd%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0531

[17] [【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484658%26idx%3D1%26sn%3Df5c9f92c272c75883bf8e6f532559f11%26chksm%3Dfdb69965cac11073f49048caef5d7b9129614090a363d9ef7f3d1b9bc59948d2217d2bca7b7b%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI 0601

[18] [大话Adaboost算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484692%26idx%3D1%26sn%3D9b389aa65208c778dddf17c601afbee1%26chksm%3Dfdb69883cac1119593934734e94c3b71aa68de67bda8a946c1f9f9e1209c3b6f0bf18fed99b8%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0602

[19] [FlowNet到FlowNet2.0：基于卷积神经网络的光流预测算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484711%26idx%3D1%26sn%3Dbb7644e101b5924f54d6800b952dc3aa%26chksm%3Dfdb698b0cac111a6605f5b9b6f0478bf21a8527cfad2342dbaaf624b4e9dcc43c0d85ae06deb%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0604

[20] [理解主成分分析(PCA)](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484754%26idx%3D1%26sn%3Db2c0d6798f44e13956bb42373e51d18c%26chksm%3Dfdb698c5cac111d3e3dca24c50aafbfb61e5b05c5df5b603067bb7edec8db049370b73046b24%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0606

[21] [人体骨骼关键点检测综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484784%26idx%3D1%26sn%3Dceafb54203f4e930ae457ad392b9f89c%26chksm%3Dfdb698e7cac111f13d8229d7dcc00b4a7305d66de3da1bd41e7ecc1d29bfa7be520d205c53e9%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0608

[22][理解决策树](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484827%26idx%3D1%26sn%3D043d7d0159baaddfbf92ed78ee5b1124%26chksm%3Dfdb6980ccac1111a9faeae7f517fee46a1dfab19612f76ccfe5417487b3f090ab8fc702d18b8%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0611

[23] [用一句话总结常用的机器学习算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484859%26idx%3D1%26sn%3D2c4db22fb538953a62a90983e3e1f99d%26chksm%3Dfdb6982ccac1113a82e92be325bb07a947d54090274654375f3b50e11e1abd809fb7358bde16%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0611

[24] [目标检测算法之YOLO](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484909%26idx%3D1%26sn%3Dc02ee17e5175230ed39ad63e73249f5c%26chksm%3Dfdb6987acac1116c0108ec28424baf4ea16ca11d2b13f20d4a825d7b2b82fb8765720ebd1063%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0615

[25] [理解过拟合](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484954%26idx%3D1%26sn%3Dc28b7f07c22466e91b1ef90e9dbe3ad1%26chksm%3Dfdb69b8dcac1129bc6e78fca1d550e2b18238ad1c240c73b280d4e529f9f93c4626b3ac45ea2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0618

[26][理解计算：从√2到AlphaGo ——第1季 从√2谈起](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247484981%26idx%3D1%26sn%3Dd3003468b9853851923844812993e060%26chksm%3Dfdb69ba2cac112b4dac620d52100ebd033eb679f29340726a67297c4d6980b16c7cc91122028%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0620

[27] [场景文本检测——CTPN算法介绍](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485005%26idx%3D1%26sn%3D0d4fb43b8db2a8046c64a9cfcbf3f478%26chksm%3Dfdb69bdacac112cce05c8b735b4f8b1ccf2348bea55a30af2055fc328958bb8f1ffd0f819bd2%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0622

[28] [卷积神经网络的压缩和加速](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485042%26idx%3D1%26sn%3Dcdcf8d4b07acf64c7a6f5f7c1a731a12%26chksm%3Dfdb69be5cac112f377766984afb87313c1e1c58d94c80005f0f6f6af61ee5a4bd1bf6c6157b6%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0625

[29] [k近邻算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485074%26idx%3D1%26sn%3D0ebf1bf8f49e9c46075fe3803d04c95d%26chksm%3Dfdb69b05cac112132d280c70af3923ca4c3cccfa5fcd8628b79d4b246b3b2decbc80a180abb3%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0627

[30][自然场景文本检测识别技术综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485142%26idx%3D1%26sn%3Dc0e01da30eb5e750be453eabe4be2bf4%26chksm%3Dfdb69b41cac11257ae22c7dac395e9651dab628fc35dd6d3c02d9566a8c7f5f2b56353d58a64%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0627

[31] [理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485155%26idx%3D1%26sn%3D990cc7400751c36e9fef0a261e6add2a%26chksm%3Dfdb69b74cac112628bdae14c6435120f6fece20dae9bf7b1ffc8b8b25e5496a24160feca0a72%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0704

[32] [机器学习算法地图](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485306%26idx%3D1%26sn%3Dfc8cc8de313bdb61dcd39c1dedb240a4%26chksm%3Dfdb69aedcac113fb4b18c74248a313536ded50bade0e66b26f332ab247b148519da71ff2a3c0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0706

[33] [反向传播算法推导-全连接神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485446%26idx%3D1%26sn%3D57d7d866443810c20c4ea2c6ee8018cc%26chksm%3Dfdb69591cac11c8773638b396abe43c0161e4d339f0fa845e54326be3e8c4933a3b6a2713dae%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0709

[34] [生成式对抗网络模型综述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485551%26idx%3D1%26sn%3D213f48c4e55bee688cf0731097bb832c%26chksm%3Dfdb695f8cac11ceef3ef246c54d811dd64d8cc45fc75488c374c7aa95f72c1abfb55555ef0b7%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0709.

[35][怎样成为一名优秀的算法工程师](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485570%26idx%3D1%26sn%3D5e71437a6d3ddf6d05144fc99e7633cc%26chksm%3Dfdb69515cac11c030cf713ec85293b7ad3bbe0f20d43fc2729cc976ff628aabf636834ccd904%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0711.

[36] [理解计算：从根号2到AlphaGo——第三季 神经网络的数学模型](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485592%26idx%3D1%26sn%3D1c5236972402ea8cb168161bc41e8e7e%26chksm%3Dfdb6950fcac11c19ad047e7cb9ced96447a85b41e21b10789a86ae4a211e4fb2ca1f911a7fc5%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0716

[37][【技术短文】人脸检测算法之S3FD](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485609%26idx%3D1%26sn%3Dd9068aecfbf150b40103210de538fea9%26chksm%3Dfdb6953ecac11c28361435306a7a09632ea79000abf1bf626e50afb3cda48eb3e47b96c6e7cd%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0716

[38] [基于深度负相关学习的人群计数方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485617%26idx%3D1%26sn%3D7955bfefc8e4b28221ae5247812f8235%26chksm%3Dfdb69526cac11c30e1051edc4378d7dfdedf46925236dbe33e7912b4bea882e515f074eee4c9%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0718

[39] [流形学习概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485668%26idx%3D1%26sn%3Df70547fc671d164e28fddcea6473524d%26chksm%3Dfdb69573cac11c65ee9fc98ac8fad093282a3d244748e7c88541c133ac55a32cb07472dc80e0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0723

[40] [关于感受野的总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485700%26idx%3D1%26sn%3Dc0425495fe0ae9cb2120dbcb246f49b1%26chksm%3Dfdb69493cac11d8542f7a8e662a7ecdeece4fd2270c71504023e8b58128575d1e4fdadf60cf5%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0723

[41] [随机森林概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485718%26idx%3D1%26sn%3Dc05c217af81173ae2c1301cbda5f7131%26chksm%3Dfdb69481cac11d975d86ff2e280371963d04b5709dfa0a9e874d637b7cf3844cad12be584094%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0725

[42] [基于内容的图像检索技术综述——传统经典方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485741%26idx%3D1%26sn%3Df8217e523d54842daaa4be38fa1792eb%26chksm%3Dfdb694bacac11dacfd8d7e4882166e2774c4685c043379ce0d2044e99c3b3c8b0140480bbf8e%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0727

[43] [神经网络的激活函数总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485762%26idx%3D1%26sn%3De1e9fc75b92999177d3c61c655b0e06e%26chksm%3Dfdb694d5cac11dc37dac1a7ce32150836d66f0012f35a7e04e3dceaf626b8453dc39ee80172b%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0730

[44] [机器学习和深度学习中值得弄清楚的一些问题](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485811%26idx%3D1%26sn%3Da87082f0e47b01bb8c22443ba5b1139c%26chksm%3Dfdb694e4cac11df20ea1deb8b55cf297ad44e48a4c7ca45cfce387284143403fcd342ac98ec0%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0802

[45] [基于深度神经网络的自动问答系统概述](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485842%26idx%3D1%26sn%3Dd7485054d6e93225b6ac0c77f8706cf7%26chksm%3Dfdb69405cac11d1355b84f692c2cbe49a3852a10e074b6941c95618598caea6ed64103c4ee4c%26token%3D1065243837%26lang%3Dzh_CN%23rd)【获取码】SIGAI0803

[46] [反向传播算法推导——卷积神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247485971%26idx%3D1%26sn%3D632e3c135ead5f0ac730eee5b6a05647%26chksm%3Dfdb69784cac11e9258f67312fa2798e9c8210b8f77741a3d7bab4c1ccfa1c1f66632183f4f26%26token%3D1469111007%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0806

[47] [机器学习与深度学习核心知识点总结 写在校园招聘即将开始时](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486105%26idx%3D1%26sn%3Dd0b33e7e23b0e2fc442bd6b3e2a9d952%26chksm%3Dfdb6970ecac11e18085ea36f3b654028b2d4ba33a0cdc89c4ea25ac81570969f95f84c6939ac%26token%3D1065243837%26lang%3Dzh_CN%23rd) 【获取 码】SIGAI0808

[48] [理解Spatial Transformer Networks](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486133%26idx%3D1%26sn%3D31c64e83511ad89929609dbbb0286890%26chksm%3Dfdb69722cac11e34da58fc2c907e277b1c3153a483ce44e9aaf2c3ed468386d315a9b606be40%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0810

[49][AI时代大点兵-国内外知名AI公司2018年最新盘点](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486140%26idx%3D1%26sn%3D6157951b026199422becca8863f18a17%26chksm%3Dfdb6972bcac11e3d7427847df818bd6cc7893a261daa0babe0b11bd01a403dc4f36e2b45650e%26token%3D283993194%26lang%3Dzh_CN%23rd)【获取码】SIGAI0813

[50] [理解计算：从√2到AlphaGo ——第2季 神经计算的历史背景](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486202%26idx%3D1%26sn%3Dbf66be1e71272be42508e557ed93acbf%26chksm%3Dfdb6976dcac11e7b9d0f878233e8d9907825e57851e7a095f1be3a23abc9327370a77f4e2c03%26token%3D283993194%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0815

[51] [基于内容的图像检索技术综述--CNN方法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486268%26idx%3D1%26sn%3Ddff53feb3d78ea7698f70bede08b3b19%26chksm%3Dfdb696abcac11fbdcde5f4acc72d34c14119a21234b9e6cd0c1886b85c330e7f77d6e3da9122%26token%3D283993194%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0817

[52][文本表示简介](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486289%26idx%3D1%26sn%3Da312c951c943ad104c722e6c681823b6%26chksm%3Dfdb696c6cac11fd01224a68b9d67fcf0043ff2de0ec67f7bfd98bacf691c8eaf221cbca9b4d6%26token%3D1485183944%26lang%3Dzh_CN%23rd) 【获取码】SIGAI0820

[53][机器学习中的最优化算法总结](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486354%26idx%3D2%26sn%3D1afd5f272536b6771b5e1e224e8414ec%26chksm%3Dfdb69605cac11f13f2a16c8748e333045e99497dc03ca9f4741723204e7d8348e969375379ae%26token%3D1580317399%26lang%3Dzh_CN%23rd)【获取码】SIGAI0822

[54][【AI就业面面观】如何选择适合自己的舞台？](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486371%26idx%3D1%26sn%3D955286243fe23ff58d5a090c166d83f6%26chksm%3Dfdb69634cac11f2251793b40432b5429828f5e72411e2ae48c1ae282117f2d1b4067c14fc889%26token%3D1383761413%26lang%3Dzh_CN%23rd)【获取码】SIGAI0823

[55][浓缩就是精华-SIGAI机器学习蓝宝书](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486392%26idx%3D1%26sn%3D81cc65e42418bf846167ab80ae1dd4f4%26chksm%3Dfdb6962fcac11f39d3c62bc06e8a0708a24a024e53aef38e1bd716dcd1433fa89a0c0fe422df%26token%3D867271861%26lang%3Dzh_CN%23rd)【获取码】SIGAI0824

[56][DenseNet详解](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzU4MjQ3MDkwNA%3D%3D%26mid%3D2247486433%26idx%3D2%26sn%3D9858164d290b60c03081ee0b04d57a76%26chksm%3Dfdb69676cac11f60f48b9bce3017e2776767f3f9c2cc980e03c07dabc076a9ce8da4b906760d%26token%3D1199765642%26lang%3Dzh_CN%23rd)【获取码】SIGAI0827

原创声明：本文为 SIGAI 原创文章，仅供个人学习使用，未经允许，不能用于商业目的


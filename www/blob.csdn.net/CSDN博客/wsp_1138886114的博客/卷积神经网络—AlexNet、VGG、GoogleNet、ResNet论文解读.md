# 卷积神经网络—AlexNet、VGG、GoogleNet、ResNet论文解读 - wsp_1138886114的博客 - CSDN博客





2018年08月03日 14:26:58[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1052
所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)











- - - [一、 CNN结构演化历史的图](#一-cnn结构演化历史的图)
- [二、 AlexNet网络](#二-alexnet网络)- - [2.1  ReLU 非线性激活函数](#21-relu-非线性激活函数)- [多GPU训练(Training on Multiple GPUs)](#多gpu训练training-on-multiple-gpus)
- [局部响应归一化(Local Response Normalization)](#局部响应归一化local-response-normalization)
- [重叠池化(Overlapping Pooling)](#重叠池化overlapping-pooling)

- [2.2 降低过拟合( Reducing Overfitting)](#22-降低过拟合-reducing-overfitting)- [数据增强(Data Augmentation)](#数据增强data-augmentation)
- [Dropout](#dropout)



- [三、VGG网络](#三vgg网络)- - [VGG 网络 卷积层组合 详解](#vgg-网络-卷积层组合-详解)


- [四、GoogleLeNet网络](#四googlelenet网络)- - [Inception V1网络（NIN结构）](#inception-v1网络nin结构)
- [Inception V2、V3、V4](#inception-v2v3v4)


- [五、ResNet 网络](#五resnet-网络)- - [5.1 解决的问题](#51-解决的问题)








### 一、 CNN结构演化历史的图

![这里写图片描述](https://img-blog.csdn.net/20180803132248243?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 二、 AlexNet网络

Deep Learning模型大名鼎鼎的AlexNet模型  
- 大量数据，Deep Learning领域应该感谢李飞飞团队搞出来如此大的标注数据集合ImageNet；
- GPU，这种高度并行的计算神器确实助了洪荒之力，没有神器在手，Alex估计不敢搞太复杂的模型；
- 算法的改进，包括网络变深、数据增强、ReLU、Dropout等 

使用梯度下降法的多层网络可以从大量的数据中学习复杂的，高纬，非线性的映射，这使得他们成为图像识别任务的首选。 全连接的多层网络可以作为分类器。

首先，图像是非常大的，由很多像素组成。具有100个隐藏单元的全连接网络包含成千上万的权重。为了解决系统的消耗和内存占用问题，在下面描述的卷积神经网络中，位移不变性(shift invariance)可以通过权值共享实现。

全连接的网络的另一个缺点就是完全忽略了输入的拓扑结构。在不影响训练的结果的情况下，输入图像可以是任意的顺序。CNN通过将隐藏结点的感受野限制在局部来提取特征。 

**CNN通过局部感受野(local receptive fields)，权值共享(shared weights)，下采样(sub-sampling)实现位移，缩放，和形变的不变性(shift,scale,distortion invariance)。**

卷积层的核就是特征图中所有单元使用的一组连接权重。卷积层的一个重要特性是如果输入图像发生了位移，特征图会发生相应的位移，否则特征图保持不变。这个特性是CNN对位移和形变保持鲁棒的基础。  

一旦计算出feature map,那么精确的位置就变得不重要了，相对于其他特征的大概位置是才是相关的。  

**在特征图中降低特征位置的精度的方式是降低特征图的空间分辨率，这个可以通过下采样层达到，下采样层通过求局部平均降低特征图的分辨率，并且降低了输出对平移和形变的敏感度。**

##### 2.1  ReLU 非线性激活函数

![这里写图片描述](https://img-blog.csdn.net/20180803140551510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通常这种标准函数是 $f(x)=tanh(x)$ 或者 $f(x)=\frac{1}{1+e^{−x}}$ 。在梯度下降训练时间上，这些饱和的非线性函数比不饱和非线性函数f(x)=max(0,x)更慢。下图这种非线性特征的神经元称为修正线性单元（ReLUs: Rectified Linear Units）。使用ReLUs的深度卷积神经网络训练速度比同样情况下使用tanh单元的速度快好几倍。  

ReLUs主要是对训练集的拟合进行加速。快速学习对由大规模数据集上训练出大模型的性能有相当大的影响。 
```
用ReLU代替了传统的Tanh或者Logistic: 
        ReLU本质上是分段线性模型，前向计算非常简单，无需指数之类操作；
        ReLU的偏导也很简单，反向传播梯度，无需指数或者除法之类操作；
        ReLU不容易发生梯度发散问题，Tanh和Logistic激活函数在两端的时候导数容易趋近于零，多级连乘后梯度更加约等于0；
        ReLU关闭了右边，从而会使得很多的隐层输出为0，即网络变得稀疏，起到了类似L1的正则化作用，可以在一定程度上缓解过拟合。
```

###### 多GPU训练(Training on Multiple GPUs)
- 本文中的网络使用两个GPU。当前的GPU都能很方便地进行交叉GPU并行，因为它们可以直接相互读写内存，而不用经过主机内存。 
- 技巧：设定只有某些层才能进行GPU之间的通信。这就意味着，例如第三层的输入为第二层的所有特征图。但是，第四层的输入仅仅是第三层在同一GPU上的特征图。在交叉验证时，连接模式的选择是一个问题，而这个也恰好允许我们精确地调整通信的数量，直到他占计算数量的一个合理比例。 

###### 局部响应归一化(Local Response Normalization)
- ReLUs具有符合本文要求的一个性质：它不需要对输入进行归一化来防止饱和。  

只要一些训练样本产生一个正输入给一个ReLU，那么在那个神经元中学习就会开始。  

但是，我们还是发现如下的局部标准化方案有助于增加泛化性能。 
###### 重叠池化(Overlapping Pooling)
- CNNs中的池化层归纳了同一个核特征图中的相邻神经元组的输出。通常，由邻接池化单元归纳的邻域并不重叠。更确切地说，一个池化层可以被看作是包含了每间隔S个像素的池化单元的栅格组成，每一个都归纳了以池化单元为中心大小为Z x Z的邻域。 
- 如果令S=Z，将会得到CNNs通常采用的局部池化。若令S＜Z，则得到重叠池。 

第一层卷积层使用96个大小为11x11x3的卷积核对224x224x3的输入图像以4个像素为步长（这是核特征图中相邻神经元感受域中心之间的距离）进行滤波。  

第二层卷积层将第一层卷积层的输出（经过响应归一化和池化）作为输入，并使用256个大小为5x5x48的核对它进行滤波。  

第三层、第四层和第五层的卷积层在没有任何池化或者归一化层介于其中的情况下相互连接。  

第三层卷积层有384个大小为3x3x256的核与第二层卷积层的输出（已归一化和池化）相连。  

第四层卷积层有384个大小为3x3x192的核，第五层卷积层有256个大小为 的核。每个全连接层有4096个神经元。  
![这里写图片描述](https://img-blog.csdn.net/20180803141711133?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图2 本文CNN的结构图示，明确地描述了两个GPU之间的职责。一个GPU运行图上方的层，另一个运行图下方的层。两个GPU只在特定的层通信。网络的输入是150,528维的，网络剩余层中的神经元数目分别是253440，186624，64896，64896，43264，4096，4096，1000 
##### 2.2 降低过拟合( Reducing Overfitting)

###### 数据增强(Data Augmentation)
- 
方法1：生成**平移图像和水平翻转图像**。做法就是从256x256的图像中提取随机的224x224大小的块（以及它们的水平翻转），然后基于这些提取的块训练网络。softmax层对这十个块做出的预测取均值。

- 
方法2：改变**训练图像的RGB通道的强度**。特别的，本文对整个ImageNet训练集的RGB像素值进行了PCA。对每一幅训练图像，本文加上多倍的主成分，倍数的值为相应的特征值乘以一个均值为0标准差为0.1的高斯函数产生的随机变量。 


###### Dropout
- 它将每一个隐藏神经元的输出以50%的概率设为0。这些以这种方式被“踢出”的神经元不会参加前向传递，也不会加入反向传播。因此每次有输入时，神经网络采样一个不同的结构，但是所有这些结构都共享权值。这个技术降低了神经元之间复杂的联合适应性。 Dropout方法和数据增强一样，都是防止过拟合的。Dropout应该算是AlexNet中一个很大的创新

### 三、VGG网络

VGG 网络是由conv、pool、fc、softmax层组成。VGG网络的卷积层，没有缩小图片，每层pad都是有值的，图片缩小都是由pool来实现的。 
- 网络包含了16个卷积/全连接层。网络的结构非常一致，从头到尾全部使用的是3x3的卷积和2x2的汇聚。 
- 他们的预训练模型是可以在网络上获得并在Caffe中使用的。
- VGGNet不好的一点是它耗费更多计算资源，并且使用了更多的参数，导致更多的内存占用（140M）。  

其中绝大多数的参数都是来自于第一个全连接层。 
![这里写图片描述](https://img-blog.csdn.net/20180804153444456?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### VGG 网络 卷积层组合 详解
- 首先，多个卷积层与非线性的激活层交替的结构，比单一卷积层的结构更能提取出深层的更好的特征。 
- 其次，假设所有的数据有C个通道，那么单独的7x7卷积层将会包含7*7*C=49C2个参数； 而3个3x3的卷积层的组合仅有个3*（3*3*C）=27C2个参数。  
**小滤波器的卷积层组合，可以表达出输入数据中更多个强力特征，使用的参数也更少** 。
- 唯一的不足是，在进行反向传播时，中间的卷积层可能会导致占用更多的内存。 

### 四、GoogleLeNet网络

![这里写图片描述](https://img-blog.csdn.net/20180804105354563?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
1 . GoogLeNet采用了模块化的结构，方便增添和修改；
2 . 网络最后采用了average pooling来代替全连接层，想法来自NIN,事实证明可以将TOP1 accuracy提高0.6%。
3 . 虽然移除了全连接，但是网络中依然使用了Dropout ;
4 . 为了避免梯度消失，网络额外增加了2个辅助的softmax用于向前传导梯度。 
    文章中说这两个辅助的分类器的loss应该加一个衰减系数，但看caffe中的model也没有加任何衰减。 
    此外，实际测试的时候，这两个额外的softmax会被去掉。
```

##### Inception V1网络（NIN结构）
- 核心组件是Inception Architecture（稀疏连接结构）
Split-Merge —> 有1X1卷积，3x3卷积，5X5卷积和3X3池化。 

增加网络对多尺度的适应性：输出通道多尺寸（感受野）化。增加网络宽度。 

Bottleneck Layer 使用NIN的1X1卷积进行特征降维，大幅降低参数量和计算量。 

减少近10倍的计算量（NiN理念），进而保证网络能够更深- 取消了全连接：
本质上是一个全尺寸的卷积层 

全连接层参数量大，为了减负取消，全连接主要用于推断用 

由全局平均池化替换（Global average pooling） 

从V1开始之后都取消了全连接层 

辅助分类器：解决前几层的梯度消失问题 

最后一层为全尺寸的卷积层 

4种操作并行进行，各自提取对应特征
##### Inception V2、V3、V4
- inception V2
其实在网络上没有什么改动，只是在输入的时候增加了batch_normal，训练起来收敛更快，学习起来自然更高效，可以减少dropout的使用。 

进行了归一化：保证出现过的最大值为1，最小值为0，所有输出保证在0~1之间。- inception V3
卷积核分解（Factorization），将 7x7 的卷积核分解成（1x7,7x1），3x3 分解（1x3,3x1），非对称分解。  

这样减少参数规模加速了计算，网络深度进一步增加，还增加了网络的非线性，减小过拟合的概率。  

另外，网络的输入从224改成了299.- inception v4
实际上是把原来的inception加上了resnet的方法，从一个节点能够跳过一些节点直接连入之后的一些节点，并且残差也跟着过去一个。用来提高速度的。 

另外就是V4把一个先1*1再3*3那步换成了先3*3再1*1.
### 五、ResNet 网络

借鉴了Highway Network思想的网络 （残差网络） 在2015名声大噪，而且影响了2016年DL在学术界和工业界的发展方向。  

该相当于旁边专门开个通道使得输入可以直达输出，而优化的目标由原来的拟合输出H(x)变成输出和输入的差H(x)-x，其中H(X)是某一层原始的的期望映射输出，x是输入。  
![这里写图片描述](https://img-blog.csdn.net/20180804162643458?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


它对每层的输入做一个reference, 学习形成残差函数， 而不是学习一些没有reference的函数。这种残差函数更容易优化，能使网络层数大大加深。 

ResNet学习的是残差函数F(x) = H(x) - x, 这里如果F(x) = 0, 那么就是上面提到的恒等映射。  

事实上，resnet是“shortcut connections”的在connections是在恒等映射下的特殊情况，它没有引入额外的参数和计算复杂度。 

假如优化目标函数是逼近一个恒等映射, 而不是0映射， 那么学习找到对恒等映射的扰动会比重新学习一个映射函数要容易。  

从图可以看出，残差函数一般会有较小的响应波动，表明恒等映射是一个合理的预处理。  
![这里写图片描述](https://img-blog.csdn.net/2018080416431039?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
实验证明，这个残差块往往需要两层以上，单单一层的残差块(y=W1x+x)并不能起到提升作用。 

考虑计算的成本，对残差块做了计算优化，即将两个3x3的卷积层替换为1x1 + 3x3 + 1x1, 如下图。  

新结构中的中间3x3的卷积层首先在一个降维1x1卷积层下减少了计算，然后在另一个1x1的卷积层下做了还原，既保持了精度又减少了计算量。  
![这里写图片描述](https://img-blog.csdn.net/20180804164531318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180817143936701?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 5.1 解决的问题

![这里写图片描述](https://img-blog.csdn.net/20180817143845268?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180817143950102?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|**模型名**|**AlexNet**|**VGG**|**GoogLeNet(V1)**|**ResNet**|
|----|----|----|----|----|
|初入江湖|2012|2014|2014|2015|
|层数|8|19|22|152|
|Top-5错误|16.4%|7.3%|6.7%|3.57%|
|Data Augmentation|+|+|+|+|
|Inception(NIN)|–|–|+|–|
|卷积层数|5|16|21|151|
|卷积核大小|11,5,3|3|7,1,3,5|7,1,3,5|
|全连接层数|3|3|1|1|
|全连接层大小|4096,4096,1000|4096,4096,1000|1000|1000|
|Dropout|+|+|+|+|
|Local Response Normalization|+|–|+|–|
|Batch Normalization|–|–|–|+|

鸣谢  
[https://blog.csdn.net/mao_feng/article/details/52734438](https://blog.csdn.net/mao_feng/article/details/52734438)
[https://www.cnblogs.com/52machinelearning/p/5821591.html](https://www.cnblogs.com/52machinelearning/p/5821591.html)

文献： 

Gradient-Based Learning Applied to Document 

Recognition  

ImageNet Classification with Deep Convolutional 

Neural Networks 












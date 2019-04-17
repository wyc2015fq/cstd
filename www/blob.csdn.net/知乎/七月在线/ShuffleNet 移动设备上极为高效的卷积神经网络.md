# ShuffleNet: 移动设备上极为高效的卷积神经网络 - 知乎
# 



**摘要：**

对计算进行优化的CNN架构：shufflenet，以便于用到计算力不足(10-150MFLOPs)的移动设备上，目标是在减少计算的同时保持准确度。

新的架构主要有两个亮点：pointwise group convolution（）和channel shuffle（）。

实验在ImageNet分类和MS COCO1物品检测上进行的，事实证明shuffleNet比其他的架构性能更优越，例，在40MFLOPs的计算力度和ImageNet测试集的条件下，相比最近的MobileNet系统，shuffleNet有更低的top-1错误率。在ARM移动设备上，保持相似准确度的前提下，相比AlexNet，shuffleNet提速大约13倍。




**简介：**

更深更大的卷积神经网络（CNN）一直都是现在深度学习的主流，所以大多数准确度高的CNN都有上百个层和上千个通道。因此需要数以亿计的FLOPs。本文在数十或几百MFLOPs的预算下，保证最大精确度，主要面向手机、无人机、机器人等低算力设备。现有工作都是对一个“标准（basic）”的网络架构进行剪枝、压缩或低位表达（low-bit representing）。论文是想要探索一个计算优化的高效basic架构。

我们使用pointwise group convolution（分组的1×1卷积核）代替密集的1×1卷积来减小计算复杂度。为了消除pointwise group convolution带来的副作用我们还提出了一个新的channelshuffle操作来帮助信息在在特征通道交叉流动。基于这两个技术，文章建立了一个高效架构：shuffleNet。




**相关工作：**

有效的模型设计：深度学习在计算机视觉方面大放异彩，这其中模型设计扮演了重要角色。许多工作者都在谋求一个运行在嵌入式设备上的高质量深度神经网络模型。例如，googlenet加深网络且减小复杂度；squeezenet在不影响精度情况下减少参数和计算量；resnet利用效率瓶颈结构（efficient bottleneck structure）来实现性能提升。“分组卷积”这一概念最初应该是由Alexnet提出的，它在两个GPU上训练模型，其已在【2】展现提升精确度的潜力；深度拆分卷积（depthwise separable convolution）在Xception中提出，有许多关于它的资料（感兴趣可以去看原文）。最近mobilenet利用深度拆分卷积获得了轻量级模型中最先进的结果。本文以新形式概括了分组卷积和深度拆分卷积。

模型加速：在一个预训练模型中剪切网络连接或者通道来减少冗余连接；量化和因式分解是通常是减少计算冗余来加快的推理速度；通过FFT来实现不修改参数的卷积算法优化；Geoffrey Hinton的《Distilling the knowledge in an eural network》将知识经过蒸馏从模型传输到小模型来使小模型训练更加容易。本文的方法并不是加速或者变化一个现有模型，而是专注于更好的模型设计以提高性能。




**方法：**

Channel shuffle for group convolution，现代卷积神经网络通常由相同结构的模块重复叠加组成，这其中最先进的网络例如Xception和ResNeXt介绍了有效的深度可拆分卷积或分组卷积来在特征表达和计算消耗间做权衡。

我们很容易知道分组卷积会减少计算，因为卷积只在通道内进行，减少了很多连接（一般卷积操作中比如输入feature map的数量是N，该卷积层的filter数量是M，那么M个filter中的每一个filter都要和N个featuremap的某个区域做卷积，然后相加作为一个卷积的结果。假设你引入group操作，设group为g，那么N个输入featuremap就被分成g个group，M个filter就被分成g个group，然后在做卷积操作的时候，第一个group的M/g个filter中的每一个都和第一个group的N/g个输入feature map做卷积得到结果，第二个group同理，直到最后一个group，如Figure1（a））。但与此同时，这也很容易导致特征提取不好（特征局部化），进而影响模型的泛化（容易过拟合）。所以本文就提出了一个channel shuffle来让将不同组内的特征图分成更小的组，然后打乱，这样就避免了特征局部性。
![](https://pic1.zhimg.com/v2-2343f180b99b9986c3d0b4c2d2903c28_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='339'></svg>)



ShuffleNet unit：接着又提出了一个组成shufflenet的重复单元，shufflenet unit，我们先说这个unit的结构，再说它做了什么优化。



![](https://pic2.zhimg.com/v2-8a6389cba74f4e9c0e19143a767f20b9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='320'></svg>)



shufflenet unit的优化是基于resnet的bottleneck unit（figure2(a)）的，shuffle unit主要做了两点改进：一是将原先的pointwise convolution改成分组的1×1卷积，在后面跟上一个channel shuffle（figure2(b)），为了简单起见，没有在第二个pointwise convolution前后加上channelshuffle，因为第二个1×1卷积一般是为了维度匹配，加不加都没太大影响；再是在shortcutpath上加了一个pooling，设置了stride=2，以及将元素相加（element-wise addition）替换为concatenation（可以不加额外计算量的同时增加维度）。

网络架构：基本上和ResNet是一样的，也是分成几个stage（ResNet中有4个stage，这里只有3个），**然后在每个stage中用ShuffleNet unit代替原来的Residual block，这也就是ShuffleNet算法的核心**。这个表是在限定complexity的情况下，通过改变group（g）的数量来改变output channel的数量，更多的output channel一般而言可以提取更多的特征。
![](https://pic2.zhimg.com/v2-275f90ba497ca167c689b4af2c54ae09_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='356'></svg>)



**实验：**文章实验简单易懂，都总结为表格的形式。有些需要解释的解释一下。

To customize the network to a desired complexity, we can simply apply a scale factor s on the number of channels. For example, we denote the networks in Table 1 as "ShuffleNet 1*", then "ShuffleNet s*" means scaling the number of filters in ShuffleNet 1* by s times thus overall complexity will be roughly s2 times of ShuffleNet 1*. （ShuffleNet s*表示将ShuffleNet 1*的filter个数变成s倍）。



![](https://pic1.zhimg.com/v2-9241e00f6423a4a5dfa09f1ddf32399c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='194'></svg>)



Table2看出组数增多，准确度越好（我认为是因为分组可以减少计算量，所以在同样计算力和过滤器数量的前提下，组数增多，特征图就能更大些），模型越小，效果越明显。同时组数特别大的时候，这种优势反而消失，因为分组越多，每个分组分到的通道数就越少，这对特征提取来说是不利的。 

Interestingly, we also notice that for smaller models such as ShuffleNet 0.25× larger group numbers tend to better results consistently, which suggests wider feature maps are especially important for smaller models. Inspired by this, for low-cost models we slightly modify the architecture in Table 1: we remove two units in Stage3 and widen each feature map to maintain the overall complexity. Results of the new architecture (named "arch2")。



![](https://pic4.zhimg.com/v2-f1894033b6247ee61824fe348ba727af_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='188'></svg>)



Table3很明显。



![](https://pic3.zhimg.com/v2-eb1d2de6b339e83f2347cb198d030022_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='163'></svg>)


![](https://pic2.zhimg.com/v2-ca8b5fdc8a68deea262d0434f33bf09d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='269'></svg>)


![](https://pic4.zhimg.com/v2-daa775fe4e45811e25596be9f2e33da7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='268'></svg>)


![](https://pic3.zhimg.com/v2-65f112c27a3e0c0c951d8fa21393a9f6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='131'></svg>)


![](https://pic1.zhimg.com/v2-d5dca29a3c3f9b7c3957ff63977ae028_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='131'></svg>)
1COCO数据集是微软团队获取的一个可以用来图像recognition + segmentation + captioning数据集，其官方说明网址：[http://mscoco.org/](https://link.zhihu.com/?target=http%3A//mscoco.org/)【1】。



# 深度学习模型压缩与加速算法之SqueezeNet和ShuffleNet - YZXnuaa的博客 - CSDN博客
2018年01月10日 14:42:13[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：185
## 前言
自从AlexNet一举夺得ILSVRC 2012 ImageNet图像分类竞赛的冠军后，卷积神经网络（CNN）的热潮便席卷了整个计算机视觉领域。CNN模型火速替代了传统人工设计（hand-crafted）特征和分类器，不仅提供了一种端到端的处理方法，还大幅度地刷新了各个图像竞赛任务的精度，更甚者超越了人眼的精度（LFW人脸识别任务）。CNN模型在不断逼近计算机视觉任务的精度极限的同时，其深度和尺寸也在成倍增长。
　　　　　　　　　　　　　　　　　　表1 几种经典模型的尺寸，计算量和参数数量对比
|Model|Model Size(MB)|Mult-Adds|Million　Parameters|
|----|----|----|----|
|AlexNet[1]|200|720|60|
|VGG16[2]|500|15300|138|
|GoogleNet[3]|~50|1550|6.8|
|Inception-v3[4]|90-100|5000|23.2|
随之而来的是一个很尴尬的场景：如此巨大的模型只能在有限的平台下使用，根本无法移植到移动端和嵌入式芯片当中。就算想通过网络传输，但较高的带宽占用也让很多用户望而生畏。另一方面，大尺寸的模型也对设备功耗和运行速度带来了巨大的挑战。因此这样的模型距离实用还有一段距离。
在这样的情形下，模型小型化与加速成了亟待解决的问题。其实早期就有学者提出了一系列CNN模型压缩方法，包括权值剪值（prunning）和矩阵SVD分解等，但压缩率和效率还远不能令人满意。
近年来，关于模型小型化的算法从压缩角度上可以大致分为两类：从模型权重数值角度压缩和从网络架构角度压缩。另一方面，从兼顾计算速度方面，又可以划分为：仅压缩尺寸和压缩尺寸的同时提升速度。
本文主要讨论如下几篇代表性的文章和方法，包括SqueezeNet[5]、Deep Compression[6]、XNorNet[7]、Distilling[8]、MobileNet[9]和ShuffleNet[10]，也可按照上述方法进行大致分类：
　　　　　　　　　　　　　　　　　　　　　　表2 几种经典压缩方法及对比
|Model|Compression Approach|Speed Consideration|
|----|----|----|
|SqueezeNet|architecture|No|
|Deep Compression|weights|No|
|XNorNet|weights|Yes|
|Inception-v3[4]|90-100|5000|
|MobileNet|architecture|Yes|
|ShuffleNet|architecture|Yes|
## **一、SqueezeNet**
**1.1 设计思想**
SqueezeNet是F. N. Iandola,S.Han等人于2016年的论文《SqueezeNet: AlexNet-level accuracy with 50x fewer parameters and < 0.5MB model size》中提出的一个小型化的网络模型结构，该网络能在保证不损失精度的同时，将原始AlexNet压缩至原来的510倍左右（< 0.5MB）。
SqueezeNet的核心指导思想是——在保证精度的同时使用最少的参数。
而这也是所有模型压缩方法的一个终极目标。
基于这个思想，SqueezeNet提出了3点网络结构设计策略：
**策略 1**.将3x3卷积核替换为1x1卷积核。
这一策略很好理解，因为1个1x1卷积核的参数是3x3卷积核参数的1/9，这一改动理论上可以将模型尺寸压缩9倍。
**策略 2**.减小输入到3x3卷积核的输入通道数。
为了保证减小网络参数，不仅仅需要减少3x3卷积核的数量，还需减少输入到3x3卷积核的输入通道数量，即式中C的数量。
**策略 3.**尽可能的将降采样放在网络后面的层中。
在卷积神经网络中，每层输出的特征图（feature map）是否下采样是由卷积层的步长或者池化层决定的。而一个重要的观点是：分辨率越大的特征图（延迟降采样）可以带来更高的分类精度，而这一观点从直觉上也可以很好理解，因为分辨率越大的输入能够提供的信息就越多。
上述三个策略中，前两个策略都是针对如何降低参数数量而设计的，最后一个旨在最大化网络精度。
**1.2 网络架构**
基于以上三个策略，作者提出了一个类似inception的网络单元结构，取名为fire module。一个fire module 包含一个squeeze 卷积层（只包含1x1卷积核）和一个expand卷积层（包含1x1和3x3卷积核）。其中，squeeze层借鉴了inception的思想，利用1x1卷积核来降低输入到expand层中3x3卷积核的输入通道数。如图1所示。
![这里写图片描述](https://img-blog.csdn.net/20170830180145472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDQwMjc4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中，定义squeeze层中1x1卷积核的数量是s1x1，类似的，expand层中1x1卷积核的数量是e1x1， 3x3卷积核的数量是e3x3。令s1x1 < e1x1+ e3x3从而保证输入到3x3的输入通道数减小。SqueezeNet的网络结构由若干个 fire module 组成，另外文章还给出了一些架构设计上的细节：
- 为了保证1x1卷积核和3x3卷积核具有相同大小的输出，3x3卷积核采用1像素的zero-padding和步长
- squeeze层和expand层均采用RELU作为激活函数
- 在fire9后采用50%的dropout
- 由于全连接层的参数数量巨大，因此借鉴NIN[11]的思想，去除了全连接层而改用global average pooling。
**1.3 实验结果**
表3 不同压缩方法在ImageNet上的对比实验结果[5] 
![这里写图片描述](https://img-blog.csdn.net/20170830180247455?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDQwMjc4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上表显示，相比传统的压缩方法，SqueezeNet能在保证精度不损（甚至略有提升）的情况下，达到最大的压缩率，将原始AlexNet从240MB压缩至4.8MB，而结合Deep Compression后更能达到0.47MB，完全满足了移动端的部署和低带宽网络的传输。
此外，作者还借鉴ResNet思想，对原始网络结构做了修改，增加了旁路分支，将分类精度提升了约3%。
**1.4 速度考量**
尽管文章主要以压缩模型尺寸为目标，但毋庸置疑的一点是，SqueezeNet在网络结构中大量采用1x1和3x3卷积核是有利于速度的提升的，对于类似caffe这样的深度学习框架，在卷积层的前向计算中，采用1x1卷积核可避免额外的im2col操作，而直接利用gemm进行矩阵加速运算，因此对速度的优化是有一定的作用的。然而，这种提速的作用仍然是有限的，另外，SqueezeNet采用了9个fire module和两个卷积层，因此仍需要进行大量常规卷积操作，这也是影响速度进一步提升的瓶颈。
## **二、ShuffleNet**
ShuffleNet是Face++今年提出了一篇用于移动端前向部署的网络架构。ShuffleNet基于MobileNet的group思想，将卷积操作限制到特定的输入通道。而与之不同的是，ShuffleNet将输入的group进行打散，从而保证每个卷积核的感受野能够分散到不同group的输入中，增加了模型的学习能力。
**6.1 设计思想**
我们知道，卷积中的group操作能够大大减少卷积操作的计算次数，而这一改动带来了速度增益和性能维持在MobileNet等文章中也得到了验证。然而group操作所带来的另一个问题是：特定的滤波器仅对特定通道的输入进行作用，这就阻碍了通道之间的信息流传递，group数量越多，可以编码的信息就越丰富，但每个group的输入通道数量减少，因此可能造成单个卷积滤波器的退化，在一定程度上削弱了网络了表达能力。
6.2 网络架构
在此篇工作中，网络架构的设计主要有以下几个创新点：
提出了一个类似于ResNet的BottleNeck单元 
借鉴ResNet的旁路分支思想，ShuffleNet也引入了类似的网络单元。不同的是，在stride=2的单元中，用concat操作代替了add操作，用average pooling代替了1x1stride=2的卷积操作，有效地减少了计算量和参数。单元结构如图10所示。
提出将1x1卷积采用group操作会得到更好的分类性能 
在MobileNet中提过，1x1卷积的操作占据了约95%的计算量，所以作者将1x1也更改为group卷积，使得相比MobileNet的计算量大大减少。
提出了核心的shuffle操作将不同group中的通道进行打散，从而保证不同输入通道之间的信息传递。 
ShuffleNet的shuffle操作如图11所示。 
![这里写图片描述](https://img-blog.csdn.net/20170830180902497?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDQwMjc4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
　　　　　　　　　　　　　　　　　　　　图10 ShuffleNet网络单元[10]
![这里写图片描述](https://img-blog.csdn.net/20170830180919274?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDQwMjc4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
　　　　　　　　　　　　　　　　　图11 不同group间的shuffle操作[10]
**6.3 实验结果**
　　　　　　　　　　　　　表8 ShuffleNet与MobileNet在ImageNet上精度对比 [10]
![这里写图片描述](https://img-blog.csdn.net/20170830180942841?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDQwMjc4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上表显示，相对于MobileNet，ShuffleNet的前向计算量不仅有效地得到了减少，而且分类错误率也有明显提升，验证了网络的可行性。
6.4 速度考量
在ARM平台上对网络效率进行了验证，鉴于内存读取和线程调度等因素，作者发现理论上4x的速度提升对应实际部署中约2.6x。给出了与原始AlexNet的速度对比，如下表。
　　　　　　　　　　　　表9 ShuffleNet与AlexNet在ARM平台上速度对比 [10] 
![这里写图片描述](https://img-blog.csdn.net/20170830181021097?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDQwMjc4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
结束语
近几年来，除了学术界涌现的诸多CNN模型加速工作，工业界各大公司也推出了自己的移动端前向计算框架，如Google的Tensorflow、Facebook的caffe2以及苹果今年刚推出的CoreML。相信结合不断迭代优化的网络架构和不断发展的硬件计算加速技术，未来深度学习在移动端的部署将不会是一个难题。
参考文献
[1] ImageNet Classification with Deep Convolutional Neural Networks
[2] Very Deep Convolutional Networks for Large-Scale Image Recognition
[3] Going Deeper with Convolutions
[4] Rethinking the Inception Architecture for Computer Vision
[5] SqueezeNet: AlexNet-level accuracy with 50x fewer parameters and < 0.5MB model size
[6] Deep Compression: Compressing Deep Neural Networks with Pruning, Trained Quantization and Huffman Coding
[7] Distilling the Knowledge in a Neural Network
[8] XNOR-Net: ImageNet Classification Using Binary Convolutional Neural Networks
[9] MobileNets: Efficient Convolutional Neural Networks for Mobile Vision Applications
[10] ShuffleNet: An Extremely Efficient Convolutional Neural Network for Mobile Devices
[11] Network in Network
[12] EIE: Efficient Inference Engine on Compressed Deep Neural Network
文章转自：[https://cloud.tencent.com/community/article/678192](https://cloud.tencent.com/community/article/678192) ，本文后续会针对SqueezeNet和ShuffleNet进行详细的阐述以及应用实例。

# CNN更新换代！性能提升算力减半，还即插即用 - 知乎
# 



> 安妮 边策 发自 凹非寺
量子位 出品 | 公众号 QbitAI



![](https://pic2.zhimg.com/v2-af9c8b41a065e92cede3e4db4ba4794d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='383'></svg>)



传统的卷积运算，要成为过去时了。

Facebook和新加坡国立大学联手提出了新一代替代品：**OctConv**（Octave Convolution），效果惊艳，用起来还非常方便。

OctConv就如同卷积神经网络（CNN）的“压缩器”。用它替代传统卷积，能**在提升效果的同时，节约计算资源的消耗。**

比如说一个经典的图像识别算法，换掉其中的传统卷积，在ImageNet上的识别精度能获得1.2%的提升，同时，只需要82%的算力和91%的存储空间。


如果对精度没有那么高的要求，和原来持平满足了的话，只需要**一半**的浮点运算能力就够了。

想实现这样的提升，怕不是要把神经网络改个天翻地覆吧？

根本不需要，OctConv**即插即用**，无需修改原来的网络架构，也不用调整超参数，方便到家。

就是这个新一代的卷积，让GAN的主要创造者、AI大牛Ian Goodfellow**迫不及待**，不仅转发力荐，还表示要持续关注进展，开源时再发推告诉大家。

OctConv也获得了众网友的认可。短短5个小时，Goodfellow的推文就收获了700赞，网友直呼“Excellent work!”



![](https://pic2.zhimg.com/v2-b252291d661af31d430e36e7feea1fa9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='745'></svg>)



所以，OctConv到底是什么神仙网络？

## **算力↓↓，准确率↑↑**

我们先来看看它效果究竟如何。

比如说经典的图像识别算法：ResNet-50，换了新的卷积运算会带来什么变化？



![](https://pic2.zhimg.com/v2-be14e3a71b7f0be2f9905175017ccdbd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='685'></svg>)



上图中粉红色的折线就是不同参数配置的OctConv对ResNet-50的影响。左起第二个粉红圆点显示了一种比较均衡的配置：比原版（最右黑点）略高的精度，所需的浮点算力却只有原版的一半。

其他折线代表的各种图像识别网络，小到ResNet-26、DenseNet，大到ResNet-200，在OctConv加持下，都体现出了成绩的提升和算力需求的下降。

调节OctConv的参数α，可以在性能提升和算力节约之间寻找平衡。

降低算力需求的同时，OctConv还能够缩短神经网络推断所需的时间。比如ResNet-50的推断时间，就会随着参数α的增大而逐步缩短。保持精度不变，推断时间能缩短到74毫秒，也就是原来的62%。



![](https://pic1.zhimg.com/v2-44ed76abbc06d044728b2b71d2534cc4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='401'></svg>)
对大、中、小型的模型，研究人员们分别测试了OctConv会如何影响它们的图像分类能力。
![](https://pic1.zhimg.com/v2-4cfc434c461996837159ba32edcb6dac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='280'></svg>)
大型神经网络ResNet-152用了OctConv后，仅用22.2GFLOP的算力，Top-1分类准确率就能达到82.9%。

OctConv的适用范围也不仅限于图像识别。

无论是2D还是3D的CNN，都能实现这种提升。论文不仅测试了ResNet、ResNeXt、DenseNet、MobileNet、SE-Net等2D CNN在ImageNet上的图像分类能力，还测试了C2D、I3D等视频行为识别算法改用OctConv之后的性能变化。

## **像压缩图像一样压缩卷积**

OctConv节约的计算力，都是从哪儿省出来的？

对于普通卷积运算，所有输入和输出特征映射具有相同的空间分辨率。

实际上，一张图片可以分成粗略结构（低频部分）和边缘细节（高频）两个部分，比如一张企鹅照片能分离出两个成分：



![](https://pic1.zhimg.com/v2-cfca3149d15abd9697e4073c7c1edac8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='390'></svg>)



企鹅身上毛色相近的部分、背景颜色变化比较缓慢，属于低频信息，信息量较少；而两种毛色交接的部分、企鹅身体边缘的颜色变化剧烈，属于高频信息，信息量较多。

既然这样，我们完全可以将信息量较少的低频部分压缩一下，减少冗余空间。

类似地，卷积层的输出特征映射和拍摄的照片一样，也可以被视为不同频率信息的混合，进行相似的处理。

研究人员从图片的频率分离和压缩中受到启发。**Octave Convolution**的思路就是对卷积网络也进行类似操作，压缩低频部分，分别处理高低频两个部分的数据，并在二者之间进行信息交换，从而减少卷积运算对存储和计算量的消耗。



![](https://pic4.zhimg.com/v2-7fdfd21d961aec84daa79c89771dcaef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='440'></svg>)



为了适应新的特征表示，文章推广了传统卷积，提出了**OctConv**。Octave是指**八音阶**，在音乐中降低八音阶代表频率减半。

OctConv中低频部分张量的大小是0.5h×0.5w，长宽正好是的高频部分h×w的一半，从而节省了张量的存储空间和计算量。

虽然OctConv压缩了低频部分的信息，但同时也有效地扩大了原始像素空间中的感受野（receptive field），可以提高识别性能。

## **实现过程**

对于普通的卷积方法，以W表示k×k的卷积核，X和Y分别表示输入和输出张量，X和Y的映射关系为：



![](https://pic4.zhimg.com/v2-3e9f6ddd48acecb2d212a45a6d10eadb_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='792' height='126'></svg>)



(p, q)是X张量中的位置坐标，(i, j)表示所取的近邻范围。

而OctConv的目标是分开处理张量中的低频和高频部分，同时实现的高频和低频分量特征表示之间的有效通信。

我们将卷积核分成两个分量：
![](https://pic3.zhimg.com/v2-60facef87d791b8cd9b98849fc9f5e56_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='268' height='108'></svg>)
同时实现高低频之间的有效通信。因此，输出张量也将分成两个分量：
![](https://pic2.zhimg.com/v2-b0632b5b599d56fb621c53e61ac16249_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='620' height='200'></svg>)
其中YA→B表示从A到B的特征映射后更新的结果。YH→H和YL→L是频率内的信息更新，YL→H和YH→L是频率间的信息更新。

因此YH不仅包含自身的信息处理过程，还包含从低频到高频的映射。

为了计算这些项，我们将卷积核每个分量进一步分为频率内和频率间两个部分：
![](https://pic2.zhimg.com/v2-40ae1d4c6177e8dde8b07f2e3b0bf18d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='668' height='98'></svg>)
张量参数可以用更形象的方式表示：



![](https://pic4.zhimg.com/v2-47fa769c9563aeccd5b7b1187f4a2fcf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='820' height='672'></svg>)△OctConv的卷积核
这种形式有些类似于**完全平方公式a^2+b^2+ab+ba**，两个平方项WH→H、WL→L是频率内张量，两个交叉项是频率间张量WL→H、WH→L



![](https://pic3.zhimg.com/v2-a819b1b6157211ddcb2efee968d2e796_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='546'></svg>)△OctConv的卷积核的“交叉”处理过程，红色箭头表示高低频之间的信息交换
输出张量的计算方式和前面普通卷积的方式相同：
![](https://pic3.zhimg.com/v2-a3a7f03f862b585a3064256e3e7a2a22_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='415'></svg>)



在OctConv中比例α是一个可以调节的参数，就是前文提到过的那个可调节参数。在整个网络内部层中令αin = αout = α，第一层中αin = 0，αout = α，最后一层中αin = α，αout = 0。

OctConv的另一个非常有用的特性是低频特征映射有较大的感受野。与普通卷积相比，有效地将感受野扩大了2倍。这会进一步帮助每个OctConv层从远处捕获更多的上下文信息，并且有可能提高识别性能。

## **华人一作**

这篇论文是Facebook联合新加坡国立大学共同完成的。



![](https://pic3.zhimg.com/v2-9c0f096aa4dcba7dfb3dfc8589b04da6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='315'></svg>)



其中，Yunpeng Chen、Haoqi Fang、Bing Xu,、Zhicheng Yan、Yannis Kalantidis、Marcus Rohrbach等6人均来自Facebook AI实验室。

一作Yunpeng Chen，中文名为陈云鹏，2015年本科毕业于华中科技大学，去年开始在Facebook实习。

陈云鹏现就读于新加坡国立大学博士，师从**颜水成**和**冯佳时**，两人也均为这篇论文的作者。今年毕业后，陈云鹏将成为Facebook的一名研究员。



![](https://pic2.zhimg.com/v2-b3f60504aed587a70a9544fe364bfc71_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='410' height='484'></svg>)



此前作为一作，陈云鹏已有4篇论文被CVPR、NeurIPS、ECCV和IJCAI等顶会接收，主攻深度学习于视觉交叉领域的研究。

颜水成是新加坡国立大学的终身教授的颜水成，现在也是360副总裁、人工智能研究院院长和首席科学家。



![](https://pic4.zhimg.com/v2-b74787ec9baef2e039234c08d4d2b2c7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='650'></svg>)



他主要研究计算机视觉、机器学习与多媒体分析领域，目前发表学术论文近500篇，被引次数超2.5万次，曾三次入选全球高引用学者。目前，颜水成有诸多荣誉加持，并评为IEEE Fellow， IAPR Fellow和ACM杰出科学家等。

冯佳时现任新加坡国立大学电子与计算机工程系助理教授，为机器学习与视觉实验室负责人。



![](https://pic3.zhimg.com/v2-8bae5c7b90e835f317ded735d49ca386_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='234' height='260'></svg>)



本科在中国科学技术大学毕业后，冯佳时在新加坡国立大学读博，此后又去UC伯克利人工智能实验室从事博士后研究，研究方向为图像识别、深度学习及面向大数据的鲁棒机器学习。

## **传送门**

论文地址：
[Reducing Spatial Redundancy in Convolutional Neural Networks with Octave Convolution​export.arxiv.org](https://link.zhihu.com/?target=https%3A//export.arxiv.org/abs/1904.05049)
更新：已经有人复现了这篇文章的算法
[terrychenism/OctaveConv​github.com![图标](https://pic2.zhimg.com/v2-72cc680bb3100b00598157fc3e8a6201_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/terrychenism/OctaveConv)
— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤



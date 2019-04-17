# 【深度学习】GAN生成对抗式网络原理 - zkq_1986的博客 - CSDN博客





2018年01月24日 16:27:06[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1418








## 生成模型和判别模型


理解对抗网络，首先要了解生成模型和判别模型。判别模型比较好理解，就像分类一样，有一个判别界限，通过这个判别界限去区分样本。从概率角度分析就是获得样本x属于类别y的概率，是一个条件概率P（y|x）.而生成模型是需要在整个条件内去产生数据的分布，就像高斯分布一样，他需要去拟合整个分布，从概率角度分析就是样本x在整个分布中的产生的概率，即联合概率P（xy）。具体可以参考博文[http://blog.csdn.net/zouxy09/article/details/8195017](http://blog.csdn.net/zouxy09/article/details/8195017)

## 对抗网络思想


理解了生成模型和判别模型后，再来理解对抗网络就很直接了，对抗网络只是提出了一种网络结构，总体来说，整个框架还是很简单的。GANs简单的想法就是用两个模型，一个生成模型，一个判别模型。判别模型用于判断一个给定的图片是不是真实的图片（从数据集里获取的图片），生成模型的任务是去创造一个看起来像真的图片一样的图片，有点拗口，就是说模型自己去产生一个图片，可以和你想要的图片很像。而在开始的时候这两个模型都是没有经过训练的，这两个模型一起对抗训练，生成模型产生一张图片去欺骗判别模型，然后判别模型去判断这张图片是真是假，最终在这两个模型训练的过程中，两个模型的能力越来越强，最终达到稳态。（这里用图片举例，但是GANs的用途很广，不单单是图片，其他数据，或者就是简单的二维高斯也是可以的，用于拟合生成高斯分布。）

## 详细实现过程


下面我详细讲讲： 
假设我们现在的数据集是手写体数字的数据集minst。 

初始化生成模型G、判别模型D（假设生成模型是一个简单的RBF，判别模型是一个简单的全连接网络，后面连接一层softmax）这些都是假设，对抗网络的生成模型和判别模型没有任何限制。 
![这里写图片描述](https://img-blog.csdn.net/20170120110505151?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 前向传播阶段


一、可以有两种输入 
1、我们随机产生一个随机向量作为生成模型的数据，然后经过生成模型后产生一个新的向量，作为Fake Image，记作D(z)。 
2、从数据集中随机选择一张图片，将图片转化成向量，作为Real Image,记作x。 

二、将由1或者2产生的输出，作为判别网络的输入，经过判别网络后输入值为一个0到1之间的数，用于表示输入图片为Real Image的概率，real为1，fake为0。 

使用得到的概率值计算损失函数，解释损失函数之前，我们先解释下判别模型的输入。根据输入的图片类型是Fake Image或Real Image将判别模型的输入数据的label标记为0或者1。即判别模型的输入类型为 ![这里写图片描述](https://img-blog.csdn.net/20170120110723560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)或者![这里写图片描述](https://img-blog.csdn.net/20170120110903322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 。

## 判别模型的损失函数：

![这里写图片描述](https://img-blog.csdn.net/20170120110947307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

当输入的是从数据集中取出的real Iamge 数据时，我们只需要考虑第二部分，D(x)为判别模型的输出，表示输入x为real 数据的概率，我们的目的是让判别模型的输出D（x）的输出尽量靠近1。 

当输入的为fake数据时，我们只计算第一部分，G（z）是生成模型的输出，输出的是一张Fake Image。我们要做的是让D(G(z))的输出尽可能趋向于0。这样才能表示判别模型是有区分力的。 
相对判别模型来说，这个损失函数其实就是交叉熵损失函数。计算loss，进行梯度反传。这里的梯度反传可以使用任何一种梯度修正的方法。 

当更新完判别模型的参数后，我们再去更新生成模型的参数。

## 给出生成模型的损失函数：

![这里写图片描述](https://img-blog.csdn.net/20170120111045401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对于生成模型来说，我们要做的是让G（z）产生的数据尽可能的和数据集中的数据一样。就是所谓的同样的数据分布。那么我们要做的就是最小化生成模型的误差，即只将由G（z）产生的误差传给生成模型。 

但是针对判别模型的预测结果，要对梯度变化的方向进行改变。当判别模型认为G（z）输出为真实数据集的时候和认为输出为噪声数据的时候，梯度更新方向要进行改变。 
即最终的损失函数为：
![这里写图片描述](https://img-blog.csdn.net/20170120111126167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中![这里写图片描述](https://img-blog.csdn.net/20170120111156285?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表示判别模型的预测类别，对预测概率取整，为0或者1.用于更改梯度方向，阈值可以自己设置，或者正常的话就是0.5。

## 反向传播


我们已经得到了生成模型和判别模型的损失函数，这样分开看其实就是两个单独的模型，针对不同的模型可以按照自己的需要去是实现不同的误差修正，我们也可以选择最常用的BP做为误差修正算法，更新模型参数。


其实说了这么多，生成对抗网络的生成模型和判别模型是没有任何限制，生成对抗网络提出的只是一种网络结构，我们可以使用任何的生成模型和判别模型去实现一个生成对抗网络。当得到损失函数后就安装单个模型的更新方法进行修正即可。

原文给了这么一个优化函数：
![这里写图片描述](https://img-blog.csdn.net/20170120111215911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)看上去很难理解，我个人的理解是，它做的是去最大化D的区分度，最小化G和real数据集的数据分布。

## 算法流程图


下图是原文给的算法流程，noise 就是随机输入生成模型的值。上面的解释加上这个图应该就能理解的差不多了。

![这里写图片描述](https://img-blog.csdn.net/20170120111240505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## noise输入的解释


上面那个noise也很好理解。如下图所示，假设我们现在的数据集是一个二维的高斯混合模型，那么这么noise就是x轴上我们随机输入的点，经过生成模型映射可以将x轴上的点映射到高斯混合模型上的点。当我们的数据集是图片的时候，那么我们输入的随机噪声其实就是相当于低维的数据，经过生成模型G的映射就变成了一张生成的图片G（x）。 
![这里写图片描述](https://img-blog.csdn.net/20170120120249762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

原文中也指出，最终两个模型达到稳态的时候判别模型D的输出接近1/2，也就是说判别器很难判断出图片是真是假，这也说明了网络是会达到收敛的。




**应用场景**



## 3. 利用 GANs 把文字转化为图像




把文字转化为图像，比起把图像转为文字（让 AI 用文字概括、描述图像）要难得多。一方面是近乎无限的像素排列方式；另一方面，目前没人知道如何把它分解，比如像（图像转为文字任务中）预测下一个词那样。


2016 年 6 月，论文《Generative Adversarial Text to Image Synthesis》（“GANs 文字到图像的合成”）问世。它介绍了如何通过 GANs 进行从文字到图像的转化。比方说，若神经网络的输入是“粉色花瓣的花”，输出就会是一个包含了这些要素的图像。该任务包含两个部分：1. 利用自然语言处理来理解输入中的描述。2. 生成网络输出一个准确、自然的图像，对文字进行表达。


为实现这些目标，生成器和判别器都使用了文字编码技术：通过循环文字编码器（recurrent text encoder）生成的文字属性，来作为条件对 GAN 进行训练（详情参考[论文](https://arxiv.org/pdf/1605.05396.pdf)）。这使得 GAN 能够在输入的文字描述和输出图像之间建立相关性联系。



原理示意


该任务中，GAN 其实完成了两件任务：1.生成自然、说得过去的图像；2.图像必须与文字描述有相关性。

![](https://static.leiphone.com/uploads/new/article/740_740/201612/586257417bec5.png?imageMogr2/format/jpg/quality/90)


利用 GAN, GAN-CLS, GAN-INT,GAN-INT-CLS 生成的结果示意。GT 是真实图像，从左到右三组图像的任务分别是：1.全黑色的鸟，粗圆的鸟嘴；2.黄胸、棕冠、黑眉的小鸟；3. 蓝冠、蓝羽、黑颊的超小鸟，嘴小、踝骨小、爪小。

## 4. GANs 应用于超分辨率（Super Resolution）


这是推特 Cortex 研究团队进行的一项研究，在今年 9 月发表。**它的主要贡献是开发出全新的损失函数（loss function），使得 GANs 能对大幅降采样后的图像，恢复它的生动纹理和小颗粒细节。**


对于不熟悉超分辨率的雷锋网读者，它是一个能把低分辨率图像重建为高清图像的技术。在机器学习中，实现超分辨率需要用成对样本对系统进行训练：一个是原始高清图像，一个是降采样后的低分辨率图像。本研究中，低分图像被作为输入喂给生成器，它重建出高分辨率图像。然后，重建图片和原始图片被一起交给判别器，来判断哪一幅是原始图像。


该研究中的损失函数可分为两个部分：对抗损失（adversarial loss ）和内容损失（content loss）。在较高层面上，对抗损失使图像看起来更自然；内容损失则保证重建图像与低分辨率原始图像有相似的特点。其中，对抗损失和传统的 GANs 应用类似，创新的是内容损失。该研究中的内容损失，可被看作为重建的高分辨率图像和原始高分图像之间特征图（feature map）的欧式距离（Euclidean distance）损失。而 GAN 的损失函数是对抗损失和内容损失的加权和。对原文感兴趣的读者，请[点这里](https://arxiv.org/pdf/1609.04802v1.pdf)。

![](https://static.leiphone.com/uploads/new/article/740_740/201612/5862664da2e3e.png?imageMogr2/format/jpg/quality/90)


左栏是原始图像，右三栏是 GANs 重建的高分图像。


以上是 GANs 2014-2016 发展期间的主要里程碑。但读者们注意，2016 年以来，GANs 的研究应用层出不穷。以上只是最具代表性的几个，不代表其他 GANs 研究就没有价值。



转载自：http://blog.csdn.net/m0_37744293/article/details/70147792


               https://www.leiphone.com/news/201612/Cdcb1X9tm1zsGSWD.html?viewType=weixin




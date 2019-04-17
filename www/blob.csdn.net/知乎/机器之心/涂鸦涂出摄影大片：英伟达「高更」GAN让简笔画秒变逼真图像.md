# 涂鸦涂出摄影大片：英伟达「高更」GAN让简笔画秒变逼真图像 - 知乎
# 



> 正在举办的 GTC 2019 上，英伟达展示了一款新的交互应用 GauGAN：利用生成对抗网络（GAN）将分割图转换为栩栩如生的图像。这是继 PGGAN、StyleGAN 之后，英伟达提出的又一强大方法，相关论文已被 CVPR 2019 接收为 oral 论文。
![](https://pic1.zhimg.com/v2-9ddab9e5cc71e36304bcfe01b2b138a8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='411' height='250'></svg>)![](https://pic3.zhimg.com/v2-3b1f8c1e340ec4631a50d809a487767e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='411' height='250'></svg>)
What? 这么美的照片竟然不是出自摄影师之手？！
![](https://pic2.zhimg.com/v2-75497816551c4294015c9b0a6f20bca9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='915' height='651'></svg>)英伟达新方法在 Flickr Landscapes 数据集上的语义图像合成结果，并不是真人拍摄的哦～
新手画家可能会在粗帆布上作画，借以创造出一种令人惊叹的日落景象——崎岖不平、白雪皑皑的山峰映在玻璃般的湖面上，但最后得到的却更像是一块多彩的墨迹。

而英伟达开发的新方法却能达到相反的效果：它可以轻松地将粗糙的涂鸦变成逼真的杰作，令人叹为观止。该方法利用生成对抗网络（[GAN](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650758921%26idx%3D2%26sn%3D3364a02501acc7862774a98db5762de2%26chksm%3D871a9b77b06d126128d23e45a5c778a1e3a5f1417e4d028fc87e294711ab9bf66065e2c12eef%26token%3D1204157705%26lang%3Dzh_CN)）将分割图转换为栩栩如生的图像。而应用了该模型的交互应用被命名为 GauGAN，以致敬后印象派画家保罗·高更（Paul Gauguin）。

GauGAN 可以为建筑师、城市规划师、景观设计师和游戏开发者等提供一个创建虚拟世界的强大工具。有了一个能够理解真实世界面貌的 AI，这些专业人士就可以更好地将想法原型化，并对合成场景做出快速改变。

英伟达应用深度学习研究副总裁 Bryan Catanzaro 表示：「用简单的草图进行头脑风暴设计要容易得多，而这项技术能够将草图转换成高度逼真的图像。」

Catanzaro 将 GauGAN 背后的技术比作「智能画笔」，可以在粗略的分割图中填充细节，粗略的分割图是显示场景中物体位置的高级轮廓。

GauGAN 允许用户绘制自己的分割图并操纵场景，用沙、天空、海或雪等标签标注每个分割块。

经过上百万张图像的训练，这一模型能够以惊人的效果填充景观：在池塘里画画，附近的树木和岩石等元素都将在水中反射出来。将一个分割标签从「草」换成「雪」，整个画面就变成了冬天的景象，原来枝繁叶茂的树木也变得光秃秃的。

「这就像一本彩色画册，描述了树在哪里，太阳在哪里，天空在哪里。」Catanzaro 表示，「然后神经网络就能根据它对真实图像的了解，填充所有的细节和纹理，以及反射、阴影和颜色……该技术不仅可以整合其他图像或切割、粘合图像纹理，实际上还可以合成新的图像，这与画家的绘画方式非常相似。」

据介绍，GauGAN 主要使用了英伟达最新推出的 SPADE 技术，相关研究已被 CVPR 2019 接收为 oral 论文。

**论文：Semantic Image Synthesis with Spatially-Adaptive Normalization**
![](https://pic3.zhimg.com/v2-8f2cef8f9ac3deae2503021d80e454ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='251'></svg>)
论文地址：[https://arxiv.org/pdf/1903.07291.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1903.07291.pdf)

摘要：我们提出了一种空间自适应归一化方法，在给定输入语义布局的情况下，实现了一种简单有效的逼真图像合成层。以前的方法直接将语义布局作为输入提供给深度网络，然后通过[卷积](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650758921%26idx%3D2%26sn%3D3364a02501acc7862774a98db5762de2%26chksm%3D871a9b77b06d126128d23e45a5c778a1e3a5f1417e4d028fc87e294711ab9bf66065e2c12eef%26token%3D1204157705%26lang%3Dzh_CN)、[归一化](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650758921%26idx%3D2%26sn%3D3364a02501acc7862774a98db5762de2%26chksm%3D871a9b77b06d126128d23e45a5c778a1e3a5f1417e4d028fc87e294711ab9bf66065e2c12eef%26token%3D1204157705%26lang%3Dzh_CN)和非线性层处理深度网络。我们的实验表明，这种方法并不是最优的，因为归一化层倾向于「洗去」语义信息。为了解决这个问题，我们提出使用输入布局，通过空间自适应的、学习的转换来调节归一化层中的激活函数。在几个具有挑战性的数据集上的实验表明，与现有方法相比，该方法在视觉保真度和与输入布局的对齐方面具有优势。最后，我们的模型允许用户控制合成图像的语义和风格。代码地址：[https://github.com/NVlabs/SPADE](https://link.zhihu.com/?target=https%3A//github.com/NVlabs/SPADE)

## **语义图像合成**

**空间自适应归一化（SPADE）**

假设语义分割掩码 m ∈ L^(H×W)，L 是一组表示语义标签的整数，H 和 W 分别是图像的高和宽，m 中的每个条目表示一个像素的语义标签。给定一个包含 N 个样本的 batch，h^i 表示深度卷积网络第 i 层的激活，C^i 表示该卷积层中的通道数，H^i 和 W^i 分别表示该层激活图的高和宽。与批归一化类似，SPADE 方法中的激活函数被逐通道归一化，然后用学习到的 scale 和偏置项进行调整。

当（n ∈ N, c ∈ C^i , y ∈ H^i , x ∈ W^i）时，激活值如下所示：
![](https://pic2.zhimg.com/v2-6eb22cf5ba247135ad6a091131faee0d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='761' height='129'></svg>)
其中，
![](https://pic4.zhimg.com/v2-c74cba0ae075aa29177260608fc163bf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='95' height='48'></svg>)
 是归一化之前点的激活。
![](https://pic2.zhimg.com/v2-59b8ad16c8b3f5dc9e080064fc2f4d41_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='95' height='100'></svg>)
 和 
![](https://pic1.zhimg.com/v2-1b1c8767c8db1e876cdfce9ae638c474_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='95' height='112'></svg>)
 分别是通道 c 中激活值的均值和标准差：
![](https://pic2.zhimg.com/v2-f3f8a04846e7ecca23c6095f30e6ad4d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='761' height='238'></svg>)
下图右展示了 SPADE 方法的设计。
![](https://pic4.zhimg.com/v2-a188e0bad62d9d5a2030f580b81392af_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='906' height='368'></svg>)
一些常见的归一化技术，比如批归一化（左图），往往在实际的归一化步骤之后应用学得的仿射层。而 SPADE 方法中的仿射层是从语义分割图中学习的。这与 Conditional Normalization 类似，不过 SPADE 中学得的仿射参数需要对空间自适应，也就是说对每个语义标签使用不同的 scaling 和偏置。使用这一简单的方法后，语义信号可以作用于所有层的输出，不受归一化过程的影响而丢失语义信息。此外，由于语义信息是由 SPADE 层提供的，因此随机 latent vector 可能作为网络的输入，来控制生成图像的风格。在 SPADE 中，掩码首先投射到一个嵌入空间，之后通过卷积运算生成调节参数（modulation parameter）γ 和 β。与已有的条件归一化方法不同，γ 和 β 不是向量，而是具有空间维度的张量。生成的 γ 和 β 经过乘法和加法后得到元素级的归一化激活值。

如下图所示，SPADE 使用最近邻下采样来调整语义分割图的大小，使之匹配对应特征图的分辨率。γ 和 β 函数通过一个简单的两层卷积网络来实现。
![](https://pic2.zhimg.com/v2-106ccd30032f66006e6c4aeba784142d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='761' height='698'></svg>)
**SPADE 生成器**

使用 SPADE 则不需要将分割图馈入生成器的第一层，其原因是获得的调制参数已经编码了关于标签布局的足够信息。所以，研究人员舍弃了生成器的编码部分。这种简化使网络更轻便。此外，与现有的分类条件生成器相似，这一新生成器可以输入随机向量，提供一种简单自然的多模式合成方法。

图 4 阐明了生成器架构，其采用了具有上采样层的几个 ResNet 块。所有归一化层的调制参数通过 SPADE 获得。由于每个残差块在不同的尺度下运作，SPADE 对语义掩码进行下采样以匹配空间分辨率。
![](https://pic2.zhimg.com/v2-d0dd2b5dfffebe1745abc5bd51b22395_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='292'></svg>)
图 4：在 SPADE 生成器中，每个归一化层都使用语义掩码来调节层激活函数。（左图）具有 SPADE 的一个残差块结构。（右图）该生成器包含一系列带上采样的 SPADE 残差块。该架构通过去除领先的图像到图像转换网络（pix2pixHD）的下采样层，用更少的参数实现了更好的性能。

**SPADE 为什么效果这么好？**

简单回答就是它能比常见归一化层更好地保留语义信息。特别是，InstanceNorm 方法中的归一化层几乎是所有 SOTA 条件图像合成模型的必需成分，当应用于统一或平滑的分割掩码时，它们倾向于洗掉所有语义信息。
![](https://pic4.zhimg.com/v2-b98c0017115b14877da6232abd51c58f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='761' height='496'></svg>)
图 3：给定统一分割图的对比结果：SPADE 生成器生成了貌似真实的纹理，pix2pixHD 因归一化层后语义信息的丢失生成了完全相同的输出。
![](https://pic2.zhimg.com/v2-8a78dd0d57b33e3cb5ddae607aa08511_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='761' height='804'></svg>)
在场景和标签多样的高难度 COCO-Stuff 数据集上，SPADE 效果显著优于已有方法 CRN 和 pix2pixHD。与真实图像相比，咦，难道不是换了个滤镜？

**实验**

研究者将 Spectral Norm 应用于生成器和判别器的所有层。生成器和判别器的学习率分别设置为 0.0001 和 0.0004。研究者使用 ADAM，设置β_1 = 0, β_2 = 0.999。所有实验都在具有 8 块 V100 GPU 的英伟达 DGX1 上进行。研究者使用同步均值和方差计算，即这些数据都是从所有 GPU 上收集的。

本研究中使用的数据集包括 COCO-Stuff 、ADE20K、ADE20K-outdoor、Cityscapes 和 Flickr Landscapes。
![](https://pic2.zhimg.com/v2-4c815c9e4e55fdd5044228a484a1cbe5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='288'></svg>)
图 5：基于 COCO-Stuff 数据集的语义图像合成结果可视化比较。该方法成功地从语义标签中合成了真实的细节。
![](https://pic1.zhimg.com/v2-6f26c2bfffb87325889dbee2cb5fd694_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='477'></svg>)
图 6：基于 ADE20K outdoor 和 Cityscapes 数据集的语义图像合成结果可视化比较。该方法在基于空间语义布局的同时生成逼真的图像。
![](https://pic4.zhimg.com/v2-085be7675dd8a8933ac0fcd7ca845bdf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='185'></svg>)
表 1：在所有的基准数据集上，该方法在语义分割得分（平均 IoU 和整体像素精度）和 FID 上都优于目前领先的方法。mIoU 和像素精度值越高越好。FID 值越低越好。
![](https://pic4.zhimg.com/v2-dd2eaa1f5125a0831caf14ebe1cf4ad7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='761' height='228'></svg>)
表 2：用户偏好研究。这些数字表明，与其他方法相比，用户更喜欢该方法的结果。
![](https://pic1.zhimg.com/v2-0f531d81a228daab9c4096983e74fb54_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='563'></svg>)
图 8：COCO-Stuff 上的语义图像合成结果。本研究中的方法成功地在动物、体育活动等各种场景中生成了逼真的图像。
![](https://pic4.zhimg.com/v2-14251f184b708d438fa817c0a0adf28f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='761' height='325'></svg>)
表 3：对于解码器架构（图 4）和 pix2pixHD++的编码器-解码器架构（在 pix2pixHD 基础上的改进基线）而言，当使用 SPADE 层时，mIoU 分数会提高。另一方面，在每一层简单地连接语义输入并不能做到这一点。此外，本研究中所有层上深度更小的简洁模型超越了所有基线。
![](https://pic1.zhimg.com/v2-e8f496eea12f87b88eb06eebbb9cacd0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='761' height='481'></svg>)
表 4：SPADE 生成器的配置有所不同。研究者改变了生成器的输入、作用于分割图的卷积核大小、网络容量和无参数归一化方法。本研究所使用的设置已加粗显示。
![](https://pic2.zhimg.com/v2-6b6b9aea5b0af0498748aca87a26e961_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='670'></svg>)
图 9：在图像编码器上训练后，该模型获得了多模式合成能力。在部署期间，通过使用不同的随机噪声，该模型合成了具有不同外观但在输入掩码中具有相同语义布局的输出。为了实现推理，在输入分割掩码中显示了 ground truth 图像。

参考链接：

[https://blogs.nvidia.com/blog/2019/03/18/gaugan-photorealistic-landscapes-nvidia-research/](https://link.zhihu.com/?target=https%3A//blogs.nvidia.com/blog/2019/03/18/gaugan-photorealistic-landscapes-nvidia-research/)

[https://nvlabs.github.io/SPADE/](https://link.zhihu.com/?target=https%3A//nvlabs.github.io/SPADE/)[Semantic Image Synthesis with Spatially-Adaptive Normalization​nvlabs.github.io![图标](https://pic4.zhimg.com/v2-d36035b21e4d67e389f8c64be61e5e97_180x120.jpg)](https://link.zhihu.com/?target=https%3A//nvlabs.github.io/SPADE/)[https://nvlabs.github.io/SPADE/](https://link.zhihu.com/?target=https%3A//nvlabs.github.io/SPADE/)





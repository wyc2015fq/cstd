# Adobe提出新型超分辨率方法：用神经网络迁移参照图像纹理 - 知乎
# 



> 图像超分辨率一直是计算机视觉领域一个热门的研究主题，也是 Adobe 等公司开拓新型图像处理技术的重要方向。近日，Adobe 研究院与田纳西大学的研究者提出了一种借助神经纹理迁移实现的图像超分辨技术，还构建了一个用于训练和评估超分辨率方法的基准数据集。

 选自arXiv，作者：Zhifei Zhang等，机器之心编译，参与：Panda。

**论文：Image Super-Resolution by Neural Texture Transfer**
![](https://pic3.zhimg.com/v2-f2aa574db714da6442dc41f7e638fbde_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='374' height='74'></svg>)- 论文链接：[https://arxiv.org/abs/1903.00834](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1903.00834)
- 项目地址：[https://github.com/ZZUTK/SRNTT](https://link.zhihu.com/?target=https%3A//github.com/ZZUTK/SRNTT)

**摘要：**由于低分辨率（LR）图像存在显著的信息损失，所以进一步推动当前最佳的单[图像超分辨率](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650758722%26idx%3D4%26sn%3Db53885f8df560624614a461b4a953e10%26chksm%3D871a9a3cb06d132aedb571dde1e1cadaf4f2a836d7681301e3964d375f2e0d29531822118fbb%26token%3D11611522%26lang%3Dzh_CN)（SISR）技术实现进一步的提升已经变得极其困难。另一方面，基于参照的超分辨率（RefSR）已被证明在给了与 LR 输入有相似内容的参照（Ref）图像时有很好的恢复高分辨率（HR）细节的潜力。但是，当 Ref 的相似程度更低时，RefSR 的性能会严重下降。这篇论文的目标是释放 RefSR 的潜力，具体做法是利用有更强稳健性的 Ref 图像的更多纹理细节，即使此时提供了不相关的 Ref 图像。受近期在图像风格化方面的研究的启发，我们将 RefSR 问题形式化为了神经纹理迁移问题。我们设计了一种端到端的深度模型，能够根据与 Ref 图像的纹理相似性自适应地迁移 Ref 图像的纹理，从而丰富 HR 细节。除了像之前的工作那样在原始像素空间中匹配内容，我们的关键贡献是在神经空间中执行的多层面匹配。这种匹配方案有助于多尺度神经迁移，能让模型从那些形义相关的Ref 图块中获得更多好处，并在最不相关的 Ref 输入上优雅地降级到SISR性能。我们为 RefSR 的一般性研究构建了一个基准数据集，其中包含与LR 输入配对的有不同层面相似度的 Ref 图像。我们进行了定量和定性评估，结果表明我们的方法优于之前最佳方法。

## **引言**

传统的单图像超分辨率（SISR）问题被定义为：根据低分辨率（LR）的观察恢复出高分辨率（HR）的图像。正如计算机视觉领域其它方向的研究一样，卷积神经网络（CNN）的引入极大地推进了 SISR 的研究前沿。但是，由于 SISR 问题不适定的本质，大多数已有方法在上采样倍数较大（比如4 倍）时仍然会得到模糊的结果，尤其是当涉及到原始 HR 图像中呈现出的但对应的 LR 图像中不具备的精细纹理时。近年来，与感知有关的约束（比如感知损失和对抗损失）已被引入到了 SISR问题的数学构建中，并为大倍数上采样的视觉质量带来了重大突破。但是，它们往往会产生幻觉式的假纹理，甚至会产生伪影。

不同于传统 SISR，本论文探索了基于参照的超分辨率（RefSR）。RefSR 是利用来自 HR 参照图像（Ref）的丰富纹理来补充 LR 图像中缺失的细节，从而缓解不适定问题并在参照图像的帮助下得到细节更丰富和更有真实感的纹理。注意，Ref 图像的获取来源有很多，比如照片集、视频帧、网络搜索图像等。已有的 RefSR 方法是采用内部示例（self-example）或外部高频信息来增强纹理。但是，这些方法都假设参照图像与 LR 图像有相似的内容和/或有良好的对齐。否则，它们的表现会有显著的下降，甚至会比SISR 方法更差。相对而言，Ref 图像在我们的设定中则发挥着不同的作用：无需与 LR 图像有良好的对齐或相似的内容。我们只希望将形义相关的纹理从 Ref 图像迁移到输出的 SR 图像。理想情况下，在存在良好的 Ref 图像时，稳健的RefSR 算法应当优于 SISR，并且在没有提供 Ref 图像或完全不含相关纹理时也能得到与 SISR 相当的表现。注意，内容相似性可以推断出纹理相似性，但反过来不成立。

受近期图像风格化研究的启发，我们提出了一种新的 RefSR 算法  Super Resolution by Neural Texture Transfer（神经纹理迁移超分辨率/SRNTT），能够自适应地将纹理从Ref 图像迁移到 SR图像。更具体而言，SRNTT 会通过一个深度模型在特征空间中执行局部特征匹配以及将匹配的纹理迁移到最终输出。这个纹理迁移模型会学习 LR 和 Ref 纹理之间的复杂的依赖关系，能够在抑制不相似纹理的同时利用相似的纹理。图 1 中的示例展示了我们提出的 SRNTT 相比于两种之前最佳研究（SRGAN（SISR 方向）和 CrossNet（RefSR 方向））的优势。如果使用的参照图像有相似的内容（即图 1(a) 上），则 SRNTT 在合成更精细纹理方面显著优于相比较的其它方法。即使使用的 Ref 图像的内容无关（ 即图 1(a) 下），SRNTT 的表现也与 SRGAN 相当（视觉质量相近，但伪影更少），这体现了 SRNTT 在使用不同程度内容相似性的不同 Ref 图像时的适应性/稳健性。相对而言，CrossNet 会从无关的 Ref 图像引入我们不想要的纹理，表现很糟糕。
![](https://pic1.zhimg.com/v2-a048710b1e763c0c953e9465058497a8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='374' height='65'></svg>)
图 1：SRNTT（我们的方法）与 SRGAN（当前最佳的 SISR 方法）和CrossNet（当前最佳的 RefSR 方法）的比较。（a）两张 Ref 图像。其中上图（U）与 LR 输入（b）的右下角有相似的内容，下图（L）与 LR 输入区别明显或内容不相关。（c）SRGAN 的结果。(d)(e)CrossNet分别使用两张 Ref 图像得到的结果。(f)(g) SRNTT 分别使用两张Ref 图像得到的结果。

为了比较公平以及帮助推动对一般性 RefSR问题的研究，我们提出了一个新的数据集 CUFED5，其中提供了训练集和测试集，并有在内容、纹理、颜色、光照条件、视角等方面不同相似度的参照。本论文的主要贡献如下：
- 我们探索了更一般的 RefSR 问题，突破了 SISR 的性能瓶颈（即缺乏纹理细节）并放宽了已有 RefSR 方法的约束（即对齐假设）。
- 我们为 RefSR 问题提出了一种端到端的深度模型 SRNTT，可通过多尺度神经纹理迁移基于任意给定参照恢复 LR 图像。我们通过广泛的实证研究展示了新提出 SRNTT 在视觉上的提升、有效性和适应性。
- 我们构建了一个基准数据集 CUFED5，可促进 RefSR 方法在处理与 LR 输入图像有不同程度相似性的参照方面的进一步研究和性能评估。

## **方法**

我们提出的 SRNTT 的目标是：给定参照图像，根据低分辨率图像估计得到更高分辨率的图像，使得到的超分辨率图像能根据参照图像合成可信的纹理，同时在内容上保持与低分辨率图像一致。图2 给出了 SRNTT的概况。其中的主要思想是在特征空间中搜索与参照图像匹配的特征，然后以多尺度的方式将匹配的特征迁移到超分辨率图像，因为特征在面对颜色和光照条件变化时更为稳健。这种多尺度纹理迁移能同时考虑低分辨率图像和参照图像在形义（高层面）和纹理（低层面）上的相似性，从而能在抑制不相关纹理的同时迁移相关的纹理。
![](https://pic1.zhimg.com/v2-ebb14a8a516da5ad2320c785680c7b9c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='325' height='323'></svg>)
图 2：我们提出的 SRNTT 框架，包含特征交换和纹理迁移

除了和之前的 SR 方法一样最小化输出的超分辨率图像和原始高分辨率图像之间的像素和/或感知距离，我们还会进一步规范在超分辨率图像和参照图像的匹配纹理之间的纹理一致性，从而强制实现有效的纹理迁移。最终输出的超分辨率图像是以端到端形式得到的。
![](https://pic3.zhimg.com/v2-f3d8364fe13a1b4112961f3923723ba6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='321' height='97'></svg>)
图 3：纹理迁移的网络结构

## **数据集**

对于 RefSR 问题，LR 和 Ref 图像之间的相似性会显著影响 SR 结果。一般而言，为了训练和评估 RefSR 算法，应该提供与LR 图像有不同程度的相似度的参照图像。就我们所知，目前还没有一个公开可用的这样的数据集。因此，我们基于CUFED 数据集（包含 1883 个拍摄了日常生活中不同事件的相册）构建了一个这样的数据集，其中有不同相似度水平的参照图像。每个相册的大小在30 到 100 张图像之间。在每个相册中，我们都基于 SIFT 特征匹配收集了不同相似程度的图像对——SIFT 特征匹配描述的是局部纹理特征，这与局部纹理匹配的目标一致。

我们根据 SIFT 特征的最佳匹配数从高到低定义了四个相似度层级，即 L1、L2、L3、L4。从每组配对图像中，我们都从一张图像随机裁剪 160×160 的图块作为原 HR 图像，而对应的参照图像则裁剪自其它图像。通过这种方式，我们收集了13761 组配对的图块作为训练集。对于测试数据集，每张 HR 图像都与所有四个层级的参照图像进行了配对，以便广泛地评估基于参照的 SR 的方法的适应性。我们使用了与构建训练数据集时相似的收集图像对的方法。总体而言，测试集包含126 组样本。每一组都包含一张 HR 图像和四张分别为 L1、L2、L3、L4 层级的参照图像。图 4 展示了两组来自测试集的样本。我们将收集到的训练集和测试集称为  CUFED5，这能很大程度地促进对RefSR 的研究以及提供一个公平比较的基准。
![](https://pic3.zhimg.com/v2-86259102fea8ac985f0b6264712815f6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='353' height='98'></svg>)
图 4：来自 CUFED5 测试集的样本。从左到右分别是 HR 图像和对应的 Ref 图像。

为了评估在 CUFED5 上训练的模型的泛化能力，我们在 Sun80 和 Urban100 上对其进行了测试。Sun80数据集有 80 张自然图像，其中每一张都伴有一系列网络搜索到的参照，Urban100 数据集则包含无参照的建筑图像。

## **实验结果**
![](https://pic4.zhimg.com/v2-cb8f4bcfbeb3b66a23f9f0a9ef34f74b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='353' height='216'></svg>)
表 1：不同 SR 方法在三个数据集上的 PSNR/SSIM 比较。不同方法按照 SISR（上）和 RefSR（下）进行了分组，各组的最佳数值用粗体标出。
![](https://pic1.zhimg.com/v2-f033ef2e715d35ef5ad1e111f5e60f40_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='374' height='380'></svg>)
图 5：不同 SR 方法在 CUFED5（最上面三个示例）、Sun80（第四和五个示例）、Urban100（最后一个示例，其参照图像是LR 输入）上的结果的视觉比较。
![](https://pic1.zhimg.com/v2-017dfe943f93914b21df0cda4a1fc368_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='374' height='50'></svg>)
表 2：在 CUFED5 数据集上不同参照水平的 PSNR/SSIM。PM 是指是否使用了基于图块的匹配；GAN 是指是否使用了GAN 和其它感知损失。
![](https://pic3.zhimg.com/v2-bee9242a771cebb35fcdcb09e1d99c66_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='324' height='139'></svg>)
图 6：用户研究结果。SRNTT 与水平轴上每种算法的比较，蓝条表示支持对应算法的用户数与支持SRNTT 的用户数的百分比。
![](https://pic2.zhimg.com/v2-12f82864c154ac11813c1fd60493f19d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='374' height='123'></svg>)
表 3：在不同参照水平上为特征交换使用不同 VGG 层的 PSNR。
![](https://pic1.zhimg.com/v2-9bea9860d2adedc9fbbecf6292a437ec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='374' height='82'></svg>)
图 7：与图 5 中同一样本相比，禁用了纹理损失的 SR 结果的质量有所下降。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)








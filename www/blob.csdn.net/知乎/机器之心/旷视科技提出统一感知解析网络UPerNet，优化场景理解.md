# 旷视科技提出统一感知解析网络UPerNet，优化场景理解 - 知乎
# 



**机器之心发布，作者：Tete Xiao、Yingcheng Liu、，Bolei Zhou、Yuning Jiang、Jian Sun。**

> 全球计算机视觉三大顶会之一 ECCV 2018（European Conference on Computer Vision）即将于 9 月 8 -14 日在德国慕尼黑拉开帷幕。届时，旷视首席科学家孙剑博士将带领团队远赴盛会，助力计算机视觉技术的交流与落地。本文介绍了旷视科技被 ECCV 2018 所接收的一篇论文，该论文提出了一种用于场景理解的统一感知解析网络——UPerNet。

**论文名称：《Unified Perceptual Parsing for Scene Understanding》**
![](https://pic3.zhimg.com/v2-27cb72ce7768919e37c57a312f7150f2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='746' height='360'></svg>)-  论文链接：[https://arxiv.org/abs/1807.10221](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1807.10221)
-  代码链接：[https://github.com/CSAILVision/unifiedparsing](https://link.zhihu.com/?target=https%3A//github.com/CSAILVision/unifiedparsing)




**目录**
- 导语
- 背景
- 设计思想
- 定义 UPP
- Broden+
- 指标
- UPerNet
- 背景
- 架构
- 实验
- 结果
- 视觉知识
- 结论
- 参考文献




**导语**

人类对世界的视觉理解是多层次的，可以轻松分类场景，检测其中的物体，乃至识别物体的部分、纹理和材质。在本文中，旷视科技提出一种称之为统一感知解析（Unified Perceptual Parsing/UPP）的新任务，要求机器视觉系统从一张图像中识别出尽可能多的视觉概念。同时，多任务框架 UPerNet 被提出，训练策略被开发以学习混杂标注（heterogeneous annotations）。旷视科技在 UPP 上对 UPerNet 做了基准测试，结果表明其可有效分割大量的图像概念。这一已训练网络进一步用于发现自然场景中的视觉知识。




**背景**

人类视觉系统一眼即可从一张图像中提取大量语义信息。人类不仅可以立即解析其中的物体，还能识别细节属性，比如其部分、纹理和材质。如图 1 所示，这是一间起居室，有着很多不同物体，比如一张咖啡桌，一幅画，以及墙面。同时，我们还看到，这是一张四腿咖啡桌，桌面之上有一块桌垫，以及桌子是木质的，沙发表层是针织的。可见，从材质、纹理的视觉感知到物体及其部分的语义感知，我们对这一视觉场景的描述是多层次的。
![](https://pic2.zhimg.com/v2-9759755271100a589262c262484ddc15_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='447'></svg>)
图 1：针对 UPP 训练的神经网络可一次性解析不同感知层次的视觉概念，比如场景、物体、部分、纹理、材质等。


近年来，由于深度神经网络和大型数据集的发展，计算机视觉识别能力取得重大进步，不断逼近甚至超越人类水准。但是，视觉识别任务不同，其研究也各不相同。比如，物体检测和场景识别已达到人类水平，解析和分割的精确度可至像素级；纹理和材质的感知与识别同样有着充分的研究。




**设计思想**

在人类视觉系统中，上述任务的完成是一步到位的，这就抛给计算机视觉模型一个问题：一个神经网络是否可以同时解决若干个不同的视觉任务。本文把这个问题以一项新任务的形式提出，称之为统一感知解析（Unified Perceptual Parsing/UPP），并给出一种全新的学习方法解决它。

UPP 有若干个挑战。首先，没有一个涵盖所有层面视觉信息的标注数据集。不同的数据集是针对一项项特定任务而打造的。比如 ADE20K 数据集用于场景解析，DTD 数据集用于纹理识别，OpenSurfaces 数据集用于材质和表面识别。其次，不同感知层面的注解也是混杂的。比如，ADE20K 数据集的注解是像素级的，而 DTD 数据集则是图像级的。

为解决上述挑战，本文提出一个新框架，整合不同数据集之间的差异性，并学习联合检测不同视觉概念。一方面，本文从每次迭代中随机采样一个数据源，并只更新相关层，以从数据源中推理概念。这样的设计会规避不稳定行为，比如某一特定概念注解的梯度带有噪音。

另一方面，该框架借助单一网络特征的分层属性，即，对于高层语义概念比如场景分类，分类器只基于带有较高级语义信息的特征图而构建；对于较低级语义信息，比如物体和材质分割，分类器只基于所有阶段的或者带有低级语义信息的特征图而构建。进而，本文提出一种训练方法，可使网络只使用图像级的注解即可预测像素级的纹理标签。

本文贡献可归纳为如下 3 个方面：1）提出一种新解析任务——统一感知解析（UPP），它需要系统一次性解析多层次视觉概念；2）提出一种带有层级结构的全新网络——UPerNet，可学习不同图像数据集中的差异化数据；3）该网络可实现联合推理，并发掘图像之中丰富的视觉知识。




**定义 UPP**

UPP 任务是指从一张给定图像中识别出尽可能多的视觉概念，从场景标签，物体，到其部分、纹理和材质，视觉概念是多层次的。该任务依赖于不同训练数据的可用性。由于没有一个现有数据集可满足条件，本文通过整合若干个图像标注源而成一个新数据集——Broden+。




**数据集**

新数据集构建的基础是 Broadly Densely Labeled Dataset（Broden），这是一个包含不同视觉概念的混杂数据集。但是由于其设计初衷，Broden 并不适用于分割网络的训练。为此本文从 4 个方面做出优化，得到了 Broden+ 数据集：
- 去掉不同数据集的相似概念；
- 只保留至少出现在 50 张图像以上、在整个数据集中至少包含 50000 像素的物体类别；
- 手动去掉 OpenSurfaces 数据集中的下采样标签；
- 把 ADE20K 数据集中 400+ 个场景标签映射到 Places 数据集中的 365 个标签。

这样，经过标准化工作而得到的新数据集共包含 57095 张图像，其中 22210 张来自 ADE20K，10103 张来自 Pascal-Context 和 Pascal-Part，19142 张来自 OpenSurfaces，5640 张来自 DTD，如表 1 所示。图 3 是一些实例。
![](https://pic4.zhimg.com/v2-920b59d35cde71f044054571ba4f84db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='209'></svg>)
表 1：Broden+ 数据集中每一标签类型的统计信息，其评估指标也已给出。
![](https://pic3.zhimg.com/v2-b7438bd727ecfafeb46948b163b47f12_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='280'></svg>)
图 3：Broden+ 数据集实例。




**指标**

一般来讲，分割任务的衡量指标是 P.A. 和 mIoU。为了解决 mIoU 不计数未标注区域的预测的问题，使其更适合部分分割等任务，本文在一些特定任务中使用 mIoU，但也计数背景区域的预测，这一新指标称为 mIoU-bg。

具体而言，对于借助 ADE20K，Pascal-Context，OpenSurfaces 数据集的物体和材质解析任务，使用评估标准 P.A. 和 mIoU；对于物体部分，则使用 P.A. 和 mIoU-bg；对于场景和纹理分类，则使用 top-1 精度。




**UPerNet**

**背景**

当前最优的分割网络主要基于全卷积网络（FCN）。由于缺乏足够的训练样本，分割网络通常初始化自针对图像分类任务的预训练网络。为使语义分割实现高分辨率预测，dilated conv 技术被提出，在缓解下采样副作用的同时，保证了感受野的扩充率；使用这一技术的网络也成为了语义分割任务的标准范式。但是针对本文提出的 UPP 任务，这一方法有 2 个缺陷：
- 最近提出的深度卷积网络虽在图像分类和语义分割任务中大获成功，但层数往往达到数十、数百层；其设计结构如此复杂，以至于在网络早期阶段由于感受野较大和计算复杂度较低的原因，下采样率快速增长。
- 这种网络只利用了其中最深的特征图。使用高级语义特征分割高级概念（比如物体）是合理的，但是并不适合分割多层次的感知属性，尤其是低级概念（比如纹理、材质）。

有鉴于此，本文提出了多任务新框架 UPerNet。




**架构**
![](https://pic4.zhimg.com/v2-52cf7c71d7d8a681a52ec2ebed4e378b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='643'></svg>)
图 4：UPerNet 架构图。




UPerNet（Unified Perceptual Parsing Network）网络架构如图 4 所示，它基于特征金字塔网络（FPN）。尽管理论上讲，深度卷积网络的感受野足够大，但实际可用的要小很多。为克服这一问题，本文把 PSPNet 中的金字塔池化模块（PPM）用于骨干网络的最后一层，在其被馈送至 FPN 自上而下的分支之前。结果实验证明，在带来有效的全局先验表征方面，PPM 和 FPN 架构是高度一致的。

本文使用多个语义层次的特征。由于图像级信息更适合场景分类，Scene head 直接被附加到 PPM 模块之后的特征图。Object head 和 Part head 被附加到与来自 FPN 的所有层相融合的特征图。Material head 被附加到 FPN 中带有最高分辨率的特征图。Texture 被附加到 ResNet 中的 Res-2 模块，并在整个网络完成其他任务的训练之后进行优化，这一设计背后的原因有 3 个： 
- 纹理是最低级的感知属性，因此它纯粹基于明显的特征，无需任何高级的信息；
- 正确预测纹理的核心特征是在训练其他任务时被隐式学习的；
- 这一分支的感受野需要足够小，因此当一张正常大小的图像输入网络，它可以预测不同区域的不同标签。




**实验**

本节首先给出了 UPerNet 在原始语义分割任务和 UPP 任务上的量化研究，接着将这一框架用于发掘场景理解背后的视觉常识知识。




**结果**

整体架构。为证明 UPerNet 在语义分割上的有效性，本文给出了不同设置下借助物体标注在 ADE20K 数据集上的结果，如表 2 所示。
![](https://pic1.zhimg.com/v2-1aacc17eebdf68e7942c9b0b4c8c1078_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='358'></svg>)表 2：ADE20K 数据集上该方法（基于 ResNet-50）与当前最优方法的对比分析。
混杂标注的多任务学习。本文给出了在分离或融合的不同标注集上的训练结果。
![](https://pic2.zhimg.com/v2-d5f2093ce8df51257d4e0357516a6f99_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='249'></svg>)表 3：UPerNet 在 Broden+ 数据集上的结果。
量化结果。本文给出了 UPerNet 的量化结果。如图 5 所示。UPerNet 可统一结构性视觉知识，同时有效预测层级输出。
![](https://pic2.zhimg.com/v2-53a16181a2aaeadb837dc788f8cfedb9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='758' height='884'></svg>)图 5：UPerNet（ResNet-50）在验证集上的预测
**视觉知识**

UPP 要求模型从一张图像之中识别尽可能多的视觉概念，如果模型成功做到这一点，就可以发现隐藏在现实世界之下的丰富视觉知识，回答诸如「这个杯子的材质是什么」的问题，有助于机器视觉系统更好理解周遭世界。

本节证明，在 Broden+ 数据集上训练的 UPerNet 可发现多层次的结构性知识。研究者以分层的方式定义了若干类关系，如表 4 所示。
![](https://pic1.zhimg.com/v2-46289c2a9cab69290299a2607c5adad0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='504'></svg>)表 4：UPerNet 发掘的视觉知识
**结论**

本文定义了名为统一感知解析（UPP）的识别任务，从场景、物体、部分、材质到纹理，其试图一次性解析图像的多层次视觉概念。一个多任务网络和处理混杂标注的训练策略被开发和测试。本文进而利用已训练的网络发现场景之中的视觉知识。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
**参考文献**
- Peng, C., Xiao, T., Li, Z., Jiang, Y., Zhang, X., Jia, K., Yu, G., Sun, J.: Megdet:A large mini-batch object detector. arXiv preprint arXiv:1711.07240 (2017)
- Zhao, H., Shi, J., Qi, X., Wang, X., Jia, J.: Pyramid scene parsing network. In:IEEE Conf. on Computer Vision and Pattern Recognition (CVPR). (2017) 2881–2890
- Chen, L.C., Papandreou, G., Kokkinos, I., Murphy, K., Yuille, A.L.: Deeplab:Semantic image segmentation with deep convolutional nets, atrous convolution,and fully connected crfs. arXiv preprint arXiv:1606.00915 (2016)
- Kirillov, A., He, K., Girshick, R., Dollr, P.: Mscoco challenge 2017: stuff segmentation,team fair. (2017)
- Zhou, B., Lapedriza, A., Xiao, J., Torralba, A., Oliva, A.: Learning deep features for scene recognition using places database. In: Advances in neural information processing systems. (2014) 487-495





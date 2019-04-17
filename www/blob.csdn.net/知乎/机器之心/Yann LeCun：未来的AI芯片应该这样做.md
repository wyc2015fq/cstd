# Yann LeCun：未来的AI芯片应该这样做 - 知乎
# 



> 「（关于 AI 芯片的开发）我们正处于非常初期的阶段……」在国际芯片顶会 ISSCC 上，深度学习先驱 Yann LeCun 破天荒地披露了 Facebook 正在自研深度学习芯片的信息。在这个硬件大会上，这一次他不仅说了很多，而且面对的是满堂芯片领域专家，颇有一副「我来教你怎么做芯片」的姿态。

2019 ISSCC 大会近日在美国旧金山开幕，Facebook 首席 AI 科学家 Yann LeCun 在会上发表了主题演讲「深度学习硬件：过去、现在和未来」，介绍了深度学习研究的发展将如何影响未来硬件架构。

LeCun 在演讲中表示，我们对深度学习专业硬件的需求只会增加，动态网络、联合记忆结构和稀疏激活都将会影响未来硬件架构的类型。「这可能需要我们重造电路中的算法。」LeCun 说，如今的计算芯片明显没有面向深度学习进行优化，「所以人们正在尝试设计表达数值的更高效方式。」

此外，在接受 VentureBeat 的采访中他还表示：深度学习可能需要一种比 Python 更灵活、更易于使用的新编程语言，目前尚不清楚是否有必要对此进行研究，但这种未来的语言与研究人员和工程师们根深蒂固的思想肯定是完全相反的。

在下面的视频中，LeCun 讨论了业内需要解决的硬件挑战，以创造更高效、更有效的 AI 系统。LeCun 在 Twitter 上表示该视频很好地总结了他今天在 ISSCC 的演讲。
[Yann LeCun谈硬件_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/z0839o56euk.html)
接下来，机器之心将对 Yann LeCun ISSCC 大会演讲的核心内容进行介绍：

PPT 地址：[https://pan.baidu.com/s/1lXv0aDSEKXKYQVhJc5X6-A](https://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1lXv0aDSEKXKYQVhJc5X6-A)

**LeCun 蛋糕 2.0 版、对图网络优化的 AI 芯片**

未来的人工智能芯片肯定和现在相对通用化的 CPU、GPU 有着不同架构，这一切还要从深度学习的发展说起。所以 LeCun 首先对在场的观众们介绍了 AI 算法的发展。

我们都知道机器学习的蛋糕理论：假设机器学习是一个蛋糕，强化学习是蛋糕上的樱桃，监督学习是外面的糖衣，[无监督学习](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757332%26idx%3D1%26sn%3De8ad3e299fd8ac13dfe4a64a00567930%26chksm%3D871a9caab06d15bc68d81d06106b0f4bbe10bb5ad331f6401f51cf05105033538b7dbe579c0f%26token%3D1191385050%26lang%3Dzh_CN)则是蛋糕本体。在这之中，无监督学习的重要性不言而喻。为了让强化学习奏效，也离不开无监督学习的支持。

 在 ISSCC 2019 上，Yann LeCun 再次放出了最新版的「LeCun 蛋糕图」：
![](https://pic1.zhimg.com/v2-f76d444d1c4272ea4307c08cd6224f70_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
其中，无监督学习变成了自监督学习，我们可以认为 LeCun 所说的自监督学习就是一种无监督方法，模型预测的结果就是输入本身或输入的一部分。例如常见的自编码器，我们希望将图像压缩到一个短向量，再恢复完整的图像，我们希望输入和输出都是相同的，因此这可以视为一种自监督。又例如近来非常流行的预训练语言模型，我们会删除完整句子的某个或某些词，并希望模型重建完整语句，这里输入和预测也是相同的，因此它也是一种自监督学习。

目前的蛋糕与 LeCun 在 NIPS 2016 大会上第一次发布的版本已有所不同：
![](https://pic2.zhimg.com/v2-a85963f8226cbf1fe2c9387ab85c1bb9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='531' height='394'></svg>)事实证明，机器学习的研究人员真的喜欢蛋糕，有时也会在上面放个樱桃
未来的 AI 芯片将会发展成什么样？在回顾了历史之后，LeCun 给出了自己的期望。首先，我们迫切地需要低功耗芯片处理来自移动设备的所有传感器数据，因为一些数据对于带宽和延迟较为敏感，在发送到云端之前需要预先处理。

然后是「中间段」计算的范畴，它们通常是传统的「离线」神经网络训练和传统云端推理。这些任务都会消耗大量资源，因此科技公司急需获得更加高效的硬件。

在深度学习「食物链」的最高端，Facebook 和亚马逊、谷歌的研发部门一样正需要英伟达 GPU 之外的更多解决方案。当前，英伟达 GPU 是深度学习训练事实上的解决方案。「在最高端的位置，我们需要的是目前主流解决方案之外的竞争者。」LeCun 表示，「这并不是因为它们（英伟达芯片）并不擅长机器学习任务，而是因为我们需要未来计算的另一种思路，作为目前 GPU 短板的补充。」

下一代芯片的架构仍然是一个悬而未决的问题。显然，未来的神经网络体量将远远超过当前，因为我们可能需要输入整个视频的像素进行推理，从而预测视频片段的运动。但与此同时，这些操作可能必须在与当今基于矩阵乘法硬件不同的处理架构中进行计算。LeCun 认为，矩阵和张量是当今人工智能计算的基石，但在未来的 AI 计算中，它们可能并非最佳解决方案。

「当前最先进的芯片基本上都是对大量 4×4 矩阵乘法在进行优化。」LeCun 表示。「所以如果你可以把自己的整个神经网络简化为 4×4 矩阵乘法，它就很好用。但这种硬件可能无法对大量卷积运算进行优化。」

「在某种程度上，这和目前深度学习研究者在思考的执行操作相同：访问数据不再通过交互作用，而是通过纯粹数组。你所拥有的就是指针的数组，指向所需的数据。」Yann LeCun 说道。这对于处理基于计算图的数据非常有效。

**深度学习算法的进步**

LeCun 在演讲中详细介绍了机器学习算法的发展历程、硬件和算力的发展、深度学习的崛起、新架构的研究、强化学习的突破和自监督学习的未来。虽然很多内容是我们比较熟悉的，例如深度学习部分介绍的各种前沿视觉模型、DL 新架构中介绍的[图神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757332%26idx%3D1%26sn%3De8ad3e299fd8ac13dfe4a64a00567930%26chksm%3D871a9caab06d15bc68d81d06106b0f4bbe10bb5ad331f6401f51cf05105033538b7dbe579c0f%26token%3D1191385050%26lang%3Dzh_CN)、自监督学习中的预训练语言模型，但还是有一些新的观点或角度。

这一部分将简要介绍 LeCun 演讲的脉络，更详细的内容可查看 PPT 或其它资源。首先 LeCun 从感知机开始介绍了机器学习发展的风风雨雨，尤其是 SGD 及反向传播算法的提出，以及通用计算芯片的大力发展，这些都为深度模型提供了基础。当然机器学习早期发展肯定少不了 LeNet-5，它简单优美的 5 层结构奠定了后续众多卷积神经网络的基础。

尽管 1990 年 LeCun 就已经提出了 LeNet-5，但直到 2012 年 AlexNet 在 ImageNet 的成功，卷积网络才真正进入研究前沿。因此 LeCun 表示当时学到的经验教训是硬件条件真的会影响研究方向，好的软硬件框架能大幅度提升新方法的进一步研究。
![](https://pic2.zhimg.com/v2-41208a5cad441190b62d5127c9ef8785_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='598'></svg>)
随着深度学习的崛起，卷积神经网络展现出强大的信息提取能力，它结构简洁、参数高效、计算快速，是理想而优雅的建模工具。因此深度 AlexNet、更深的 [VGG](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757332%26idx%3D1%26sn%3De8ad3e299fd8ac13dfe4a64a00567930%26chksm%3D871a9caab06d15bc68d81d06106b0f4bbe10bb5ad331f6401f51cf05105033538b7dbe579c0f%26token%3D1191385050%26lang%3Dzh_CN) 以及突破深度瓶颈的 [ResNet](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757332%26idx%3D1%26sn%3De8ad3e299fd8ac13dfe4a64a00567930%26chksm%3D871a9caab06d15bc68d81d06106b0f4bbe10bb5ad331f6401f51cf05105033538b7dbe579c0f%26token%3D1191385050%26lang%3Dzh_CN) 都相继提出。LeCun 表示这一段时间大家都在探索高效的卷积架构，包括探索更小卷积核的 VGG-Net、探索优良局部拓扑结构的 [GoogLeNet](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757332%26idx%3D1%26sn%3De8ad3e299fd8ac13dfe4a64a00567930%26chksm%3D871a9caab06d15bc68d81d06106b0f4bbe10bb5ad331f6401f51cf05105033538b7dbe579c0f%26token%3D1191385050%26lang%3Dzh_CN)，以及探索更优梯度传播路径的 ResNet 与 DenseNet。

虽然看起来只是网络变得越来越深，但隐藏在下面的信息流、梯度流、感受野以及张量计算流都经过不同的考虑，每一个基础架构的更新都令我们兴奋不已。
![](https://pic2.zhimg.com/v2-09f2dbdd8218b10af5ae30a313421bf5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1049' height='583'></svg>)
在基础架构不断进步的过程中，各种视觉任务及其子任务也都在大步向前。例如视觉领域最基础的识别、定位和分割任务，它们各自经历一系列发展历程：从简单的基础卷积架构到为特定任务构建的高效卷积运算与结构。例如两阶段目标检测算法，它们从 R-CNN 到 Fast R-CNN 再到 Faster R-CNN，这三种模型的提出旨在更高效地利用卷积和卷积抽取的特征，更快地预测图像类别和物体位置。

除此之外，各种用于特定任务的架构和工具也层出不穷，包括大名鼎鼎的 Mask R-CNN、结合底层和抽象信息的 FPN、用于医疗影像的 3D 卷积网络等。到了近两年，视觉任务除了变得越来越精细化、衍生了很多子任务以外，它本身也在进化。例如统一实例分割与语义分割，我们就可以提出全新的全景分割，这种新任务的端到端训练方法仍然是研究前沿。

在深度学习时代，模型架构的突破很大程度都依赖于 GPU 提供的巨大算力。但仅仅取得好结果还不够，我们的经验教训是它还必须易于复现及可靠，这比单纯提供跑分结果重要得多。
![](https://pic2.zhimg.com/v2-2f274e56a32b8af5608165eda37e6681_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1047' height='553'></svg>)
对于新型 DL 架构，LeCun 表示我们关注的是记忆、推理和结构化处理，这三者都是目前还未真正解决的问题。首先记忆不仅包括像 LSTM 或 GRU 那样的短期记忆，同时还包括神经图灵机那样的长期记忆，它们是令模型真正理解语境并进行推理的关键。

其次最近图神经网络在结构化数据上展示了很好的效果，LeCun 也介绍了它和一般卷积网络的不同及使用范围。与传统的图嵌入相比，图神经网络可以在整个网络上进行信息传播、聚合等建模，也就是说它可以把网络当成一个整体进行建模。DeepMind 提出的图神经网络一般框架 Graph Network 在一定程度上能为深度学习提供对归纳和推理的建模方式，因此得到了很多研究者的关注。
![](https://pic3.zhimg.com/v2-1b40f90994a51e59da2ffc7e950207c2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1037' height='568'></svg>)
前沿的深度学习研究还有很多，但 LeCun 表示也许动态网络会成为主流，因为它能打破当前硬件的很多假设。尽管图神经网络可能在很多应用上会有很好的表现，但它主要处理图结构的结构化数据。而像图像或语音那样的非结构化数据，我们没有高效的手段转化为结构化数据，因此图网络仍然任重道远。
![](https://pic1.zhimg.com/v2-263dccfa244e4c1ffc5823d05a7969dc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='959' height='555'></svg>)
强化学习怎么样？Yann LeCun 总结说它在游戏和虚拟环境中表现很好，但存粹的强化学习难以用到真实世界中。
![](https://pic2.zhimg.com/v2-17e2eb2de91d9f141635f364fbe41eb1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='584'></svg>)
存粹的强化学习需要太多的试错，难以学习所有东西。那么我们离「真实的」AI 还差了什么？下图给出了说明。
![](https://pic4.zhimg.com/v2-8178f1cc9492d33e17e5d5d3c3b232fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='628'></svg>)
**自监督学习是 AI 的未来**

LeCun 在演讲中还展望了未来，他认为自监督学习将是潜在的研究方向和解决方案。自监督学习能够通过输入的某一部分预测其它部分。在空间层面上包括图像补全、图像变换等，在时间层面上包括时序数据预测、视频帧预测等。
![](https://pic1.zhimg.com/v2-3dfc08fcdd284728a18a06755f13df40_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='600'></svg>)
LeCun 再次用著名的「蛋糕图」解释了自监督学习，并介绍了自监督学习在计算机视觉、自然语言处理中的应用，如在文本中的应用（Word2vec、FastText、BERT）。
![](https://pic3.zhimg.com/v2-c9329e3f606792fa325e058de3e1fd9e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
不过他也表示自监督学习在处理高维连续信号时表现不佳，例如做视频预测。

但是人工智能的未来不会是监督学习，当然也不会纯粹是强化学习，它应该是包含了深度模块的自监督学习。
![](https://pic2.zhimg.com/v2-d887267cffc9396a7bdc388f14fdb065_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1036' height='685'></svg>)
此外，LeCun 还介绍了自监督学习在学习世界模型上的应用及前景。使用自监督学习，机器可以通过输入的未标注图像、视频或音频来学习世界的抽象表征。

**学习世界预测模型**

在这一部分，Yann LeCun 介绍了如何学习预测、推理、规划，如何学习常识。
![](https://pic2.zhimg.com/v2-60ad3536a73ee3a3b76aa4a813227905_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='604'></svg>)规划需要预测。为了提前规划，我们需要模拟世界![](https://pic2.zhimg.com/v2-b5e4081830ed8f7bae9c9fcaadeb4c4d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='601'></svg>)难题：不确定性预测。不变量预测：训练样本只是全部有可能输出的表示![](https://pic4.zhimg.com/v2-026e5f2d8eb94434634a889fe10c3413_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='604'></svg>)自监督对抗学习做视频预测![](https://pic1.zhimg.com/v2-0b676cf302619fc2530845d0f9ee3054_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='598'></svg>)使用前馈模型进行规划（学习开车）![](https://pic4.zhimg.com/v2-7607cd174900491cb5da4b8fb18b843f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='607'></svg>)学习到的经验包括：1. 自监督学习是未来，网络将会变得更大，也可能稀疏。2. 通过最小化进行推理/推断。3. 深度学习硬件的使用案例
最后，常怼人的 Yann LeCun 在演讲结尾还是质疑了一下，虽然现场并未展开讲太多。从下页 PPT 中，我们可以看到，Yann LeCun 对脉冲神经网络、神经形态架构提出了质疑。他表示，没有基于脉冲的神经网络能在实际任务上取得 SOTA 水平，为什么还要为无效算法做芯片？
![](https://pic1.zhimg.com/v2-9a9bc3ac225f0b0246b86fb846ad2070_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='586'></svg>)
**AI 芯片让人工智能避开寒冬**
![](https://pic2.zhimg.com/v2-f0bdf20f3c9d4c1fbdead5c0ff3406d5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='500'></svg>)全球 AI 芯片创业公司的融资金额正逐年提升
纵观 AI 领域发展的历史，只有新的芯片和硬件架构才能将 AI 带出低谷。LeCun 以反向传播举例，这是一种当今深度学习广泛采用的技术，但也是在计算机硬件大发展后才在 20 世纪 90 年代开始得到广泛应用。在过去，Facebook 曾经设计过其他类型的硬件，如数据中心的新架构，这些研究目前都已开放。在未来，同样的事情或许也会发生在 AI 芯片上。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
*参考内容：*

[https://www.businessinsider.com/facebook-artificial-intelligence-yann-lecun-2019-2](https://link.zhihu.com/?target=https%3A//www.businessinsider.com/facebook-artificial-intelligence-yann-lecun-2019-2)

[https://ai.facebook.com/blog/yann-lecun-video/](https://link.zhihu.com/?target=https%3A//ai.facebook.com/blog/yann-lecun-video/)





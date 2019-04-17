# 中山大学等提出HSE：基于层次语义嵌入模型的精细化物体分类 - 知乎
# 



**机器之心发布，作者：Tianshui Chen、Wenxi Wu、Yuefang Gao、Le Dong、Xiaonan Luo、Liang Lin。**

> 细粒度识别一般需要模型识别非常精细的子类别，它基本上就是同时使用图像全局信息和局部信息的分类任务。在本论文中，研究者们提出了一种新型层次语义框架，其自顶向下地由全局图像关注局部特征或更具判别性的区域。

人类在识别物体类别时，往往不仅仅根据其外观信息，还依赖于在日常生活以及专业学习过程中获取的先验知识。如何有效利用这些先验知识引导和约束网络学习是目前一个重要的研究难点。针对于精细化物体识别，其类别可以按照不同程度进行概念抽象，并形成了层次化的分类结构。这种结构是层级间丰富语义知识的集中体现，如层级间的关联系，它可以作为先验知识规范化语义空间，有效引导和约束深度网络的学习。近日，中山大学、华南农业大学、电子科技大学以及桂林电子大学联合研究团队提出层次化语义嵌入模型，有效利用层次化的分类结构引导网络特征学习以及约束其预测空间，在细粒度图像识别问题上取得最优的性能。

论文地址：[https://arxiv.org/pdf/1808.04505.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1808.04505.pdf)




**模型介绍**

一般来说，物体的类别可以按照不同程度进行概念抽象，由此形成了层次化的分类结构。例如，鸟类 (鸟纲) 可以按照目、科、属、种四个层级分类。这种层次化信息编码了不同层级的类别之间的关联关系，可以作为一种先验信息有效的规范语义空间。比如说，若要识别给定物体的细粒度类别 (例如，鸟「种」的类别)，作者可能先识别出它的超类 (例如，鸟「属」的类别)。然后，作者会更倾向于专注在那些属于这个超类的细粒度类别，进而专注于可以为区分这些细粒度类别提供更多判别信息的物体区域。然而，目前的方法仅仅着眼于单个层级的类别，却忽略了丰富的结构化信息。

在本论文的工作中，作者同时预测了分类层次结构中各层级的类别，并将这种结构化关联信息集成到深层神经网络中，以逐步约束标签预测和指导特征表达学习。为此，作者提出了一种新的层次语义嵌入框架（Hierarchical Semantic Embedding，简称 HSE），它可以自顶向下地逐层预测每个层级的类别的分数向量。在预测每个层级的类别时，HSE 框架将上一层级预测所得的得分向量作为先验信息嵌入网络，以学习更细致的图像特征表达。在此，作者引入一种新的语义引导的视觉注意力机制来实现，这使得 HSE 框架可以定位到更具有判别性的区域以更好的识别物体。在训练过程中，对于每一层级，作者还利用其上一层级预测所得的得分向量作为软目标，去约束该层级的标签预测，从而使该层级的预测结果与其上一层级的预测结果符合层次结构的关联规则。方法的整体框架图如图 1 所示。
![](https://pic2.zhimg.com/v2-be31b3e387743c968adc662c8c34c49d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='568'></svg>)图 1. 层次语义知识嵌入框架整体示意图



**数据集介绍**

为了评估提出的框架其有效性，作者扩展了 Caltech-UCSD birds 数据集 (Extended CUB)，将其组织成具有目、科、属、种四个层级分类结构。同时，作者还构建一个大规模的蝴蝶数据集 (Butterfly-200)，其类别标注同样覆盖了四个层级。

Butterfly-200 数据集涵盖 200 种常见的蝴蝶，并根据生物学分类原理进一步的聚成 116 个属，23 个亚科，5 个科。Butterfly200 共收纳了超过了 25,279 张蝴蝶图片，主要包括从互联网利用关键词检索得到的自然图片，以及从实验室拍摄的到的标准图片。为了数据集的精确性和可靠性，这些图片由华南农业大学昆虫学系的四位专家进行二次甄别。

Caltech-UCSD birds 数据集是目前精细化图像分类最常用的评测基准数据集，该数据集包括涵盖 200 类鸟的 11,788 张图像。该论文根据生物学分类原理进一步把这 200 类聚成 13 个目，37 个科，122 个属。

图 2 展示来自这两个数据集的一些样本以及对应层次化标注。
![](https://pic4.zhimg.com/v2-abb5ce06a0d188c98ffdd1a70d6ca297_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='737' height='470'></svg>)图 2. 来自 Extended CUB(第 1，2 行) 和 Butterfly-200(第 3，4 行) 的样本以及对应层次化标注



**实验结果**

该论文在 Extended CUB，butterfly-200，以及最新发布的 VegFru 上进行实验。




**组件分析**

作者首先设计两个没有引入语义嵌入的基准模型 (baseline 和 baseline+backtrack)，如表 1，在 Extended CUB 和 Butterfly-200 的数据集上，HSE 框架在两个数据集上的性能始终优于两组对比的基准方法。
![](https://pic1.zhimg.com/v2-df9633b616c91f7419f50c31137d2cb4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='214'></svg>)表 1. HSE 框架与其它引入额外信息的算法在 CUB 数据集上的首位识别准确率对比
HSE 框架与两种基准方法的比较证明了它作为一个整体的有效性。实际上，HSE 框架从两个方面整合了语义相关信息，即：语义嵌入表示学习 (Semantic Embedding Representation Learning，简称 SERL) 和语义引导标签正规化 (Semantic Guided Label Regularization，简称 SGLR)。作者同样评估这两部分的实际贡献。作者分别去掉 SERL(Ours w/o SERL) 和 SGLR(Ours w/o SGLR) 模块进行组件分析，如表 2 所示，去掉任一模块都会在两个数据集上的所有级别上的性能明显下降。




**物体精细化识别性能对比**

作者在 CUB 和 VegFru 两个数据集上和目前最先进的方法进行比较，结果如表 2 和表 3 所示。在两个数据集上，HSE 框架均取得了优于当前最先进方法的性能。
![](https://pic4.zhimg.com/v2-3a7225578f8e8125498266308322d793_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='586' height='807'></svg>)表 2. HSE 框架与其它最先进的算法在 CUB 数据集上的首位识别准确率对比![](https://pic4.zhimg.com/v2-d4c2eab5dffc3e3c23fd79cb9a094bfb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='557' height='327'></svg>)表 3. HSE 框架与其它先进方法在 Vegfru 数据集上的性能比较
代码，模型和数据链接：[https://github.com/HCPLab-SYSU/HSE](https://link.zhihu.com/?target=https%3A//github.com/HCPLab-SYSU/HSE)




引用：

[1] Tianshui Chen, Wenxi Wu, Yuefang Gao, Le Dong, Liang Lin, Fine-Grained Representation Learning and Recognition by Exploiting Hierarchical Semantic Embedding, in ACM MM, 2018.

[2] Catherine Wah, Steve Branson, Peter Welinder, Pietro Perona, and Serge Belongie. The caltech-ucsd birds-200-2011 dataset. California Institute of Technology, 2011.

[3] Saihui Hou, Yushan Feng, and Zilei Wang. VegFru: A Domain-Specific Dataset for Fine-grained Visual Categorization. In ICCV, 2017.

> 中山大学人机物体智能融合实验室有着深厚的学术底蕴和强大的人才支持，指导学生在 PAMI，IJCV，CVPR，ICCV 等国际顶级的期刊、会议上发表多篇论文，论文具有国际影响力；在 ImageNet、全国研究生智慧城市技术与创意设计大赛、全国并行应用挑战赛等顶尖赛事中屡获殊荣。多年来，实验室多名优秀学子纷纷走向了国际顶级学府和知名 AI 企业。



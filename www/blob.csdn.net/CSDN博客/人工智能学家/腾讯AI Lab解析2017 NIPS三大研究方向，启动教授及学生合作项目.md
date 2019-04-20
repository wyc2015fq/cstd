# 腾讯AI Lab解析2017 NIPS三大研究方向，启动教授及学生合作项目 - 人工智能学家 - CSDN博客
2017年12月15日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：414
*![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/LWHwEYpIANK7oBJjFUDWJM1n94NhNtPLEibREk1J2d3Ws6O068bVEYvN1akHGF4PQdoY6DKHzia5OLSvWadkz4eA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)*
*来源：腾讯AI实验室*
*概要：**腾讯AI Lab去年4月成立，今年第二次参加NIPS，共有8篇文章被录取，含一篇口头报告（Oral）。在所有国内研究机构和高校中，录取论文数仅次于清华大学。*
NIPS被誉为机器学习和人工智能研究领域最具影响力，也是参会人数最多的的学术会议。腾讯AI Lab去年4月成立，今年第二次参加NIPS，共有8篇文章被录取，含一篇口头报告（Oral）。在所有国内研究机构和高校中，录取论文数仅次于清华大学。
此次参会团队由腾讯公司副总裁姚星、机器学习和大数据领域专家、腾讯AI Lab主任张潼博士和语音识别及深度学习领域专家、腾讯AI Lab副主任俞栋博士共同带领到场交流学习，团队在会后对会议的三大研究领域进行了独家回顾与解析。
**与 学 界 共 享 AI**
**隆重介绍与教授和学生的合作项目**
在展台前，我们还介绍了多个腾讯AI Lab与学界共享共连AI项目，针对计算机视觉、语音识别、自然语言处理和机器学习四大领域的研究者，对接产业界真实数据与需求，共同促进高水平的产学研合作。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANK7oBJjFUDWJM1n94NhNtPLhUdgVFz2ib5rH8tcHHRutkX5QQO81XdXep4L8icTWDSksaFYXY61NEwA/640?wx_fmt=png)
**面向学生的**，有腾讯AI Lab博士生奖学金计划，向全球重点大学在读三年级（含）以上的全日制博士生开放，提供约20000-50000美元不等的生活及差旅费，到美国或中国的腾讯AI Lab实习，申请截至日期12月19日！
**面向教授的**，有犀牛鸟联合研究计划和访问学者计划，针对全球高校全职学者或科研机构的全职研究人员，目前已支持中国、美国和欧洲超过15位教授的研究项目及多位青年骨干学者，申请截至日期为2018年1月31日！
所有项目可登录官网ai.tencent.com了解详情及申请。
**解 析 一：递 归 神 经 网 络**
**Recurrent Neural Networks**
递归神经网络 (RNN)，仍然是深度学习领域的一个重要研究方向。这次NIPS 2017共有二十余篇RNN相关的论文被正会收录。这些论文涵盖了一些新的RNN的网络架构，譬如dilated RNN、recurrent ladder networks,、recurrent highway hypernetworks和fast-slow RNN等；探讨了RNN在一些具体问题的应用，譬如OCR、precipitation nowcasting、video action prediction、video object segmentation、image captioning、question answering和SLAM等。
**我们重点关注了以下几篇论文：**
**1）   Dilated Recurrent Neural Network**
本文由IBM T. J. Watson研究中心和伊利诺伊大学香槟分校合作完成。传统递归神经网络在学习长序列数据时通常遇到很大的困难，主要表现在复杂的长时序跨度记忆和中短期记忆难以同时处理，用反向传播 (BP) 算法训练递归神经网络容易出现梯度消失和梯度爆炸的问题，与前向和反向传播需要串行进行，导致训练非常费时。本文提出了一种dilated RNN来解决上述训练难题。这种网络基于一种创新的多分辨率dilated递归skip连接，能够自由地和不同种类RNN单元结合。这种dilated递归神经网络结构显著减少了网络参数，提高了训练的高效性，同时还能取得与标准的递归神经网络相似的效果。为了理论地量化该网络结构的优势，本文还提出了一种记忆容量衡量标准，即平均递归长度，比已有的标准更适合具有长skip的递归神经网络。
**2）   Recurrent Ladder Network**
本文由Curious AI公司完成，提出了一种梯子网络 (ladder networks) 的递归形式的扩展。梯子网络是一种为了解决层级隐变量模型的推断 (inference) 而提出的网络。本文发现递归式的梯子网络能够处理各种复杂的学习任务，因而对迭代推断和时序建模非常有意义。这种新型网络结构在视频时序建模展示了接近最优的结果，在音乐建模上取得了有竞争力的实验结果，并且提高了基于高维抽象的感知聚类，例如随机文本和运动线索。本文在有监督、半监督和无监督的训练场景下均展示了实验结果，显示该结构对学习层级抽象、迭代推断和处理时序信息大有裨益。
**3） Character-Level Language Modeling with Recurrent Highway Hypernetworks**
本文由斯坦福大学完成，文中对Recurrent Highway Network (RHN) 与recurrent hypernetwork之间的互补特性提供了详细的理论和实验支持。RHN相较于LSTM，提供了更为高效的gradient flow进而提升了模型在一些任务上的性能。将RHN与hypernetworks组装在一起，在字符级别的语言建模上的性能可以得到进一步的提升。更进一步解释，hypernetworks的作用类似于RNN上的注意力机制。
**4） Fast-Slow Recurrent Neural Networks**
本文由苏黎士理工大学完成，文中构建了一个fast-slow RNN (FS-RNN) 模型来处理序列数据中变长数据的建模问题。FS-RNN融合了multiscale RNN和deep transition RNN来处理不同的时间尺度和复杂的变换信息。因此，FS-RNN可以更好的处理long-term dependencies，并且能够快速地适应unexpected inputs。FS-RNN在两个字符级别的语言建模任务上同时提升了现有RNN模型的性能。另外，任意的RNN unit都可以用于构造一个对应的FS-RNN。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANK7oBJjFUDWJM1n94NhNtPLOq4tvZAmuic8nxibDSBN8ib1AicUGUr3icE6PtzOALT61zkZTlHCc1pTKRg/640?wx_fmt=png)
FS RNN的架构 (包含有k个fast cell)，其中只有第2个fast cell接收slow cell的input。低层的fast RNN用于学习复杂的变换函数 (transition function)；而高层的slow RNN可以产生更短的gradient path，进而更容易学习long-term dependencies。
**5.   Gated Recurrent Convolution Neural Network for OCR**
本文主要讨论Gated Recurrent Convolution Neural Network (GRCNN) 在OCR上的应用，由北京邮电大学与清华大学联合完成。通过recurrent convolution layer (RCL) 中引入门机制 (gating strategy) 来控制RCL的上下文 (context) 信息，并且平衡前向信息和递归信息的传递。将GRCNN与双向LSTM组装在一起，整个网络可以完成端到端的训练，进而更加有效地识别图像中的文字信息。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANK7oBJjFUDWJM1n94NhNtPLEqvG7j0ttwgHHw1nX3uXtacGoNiaJH3CJFFKp4x5WfbyAl0yJiaVSAicg/640?wx_fmt=png)
**6） Training recurrent networks to generate hypotheses about how the brain solves hard navigation problems**
本文由德州奥斯汀分校完成。文中首先构建了一个神经网络的解决方案来解决SLAM问题，即通过训练LSTM网络来完成2D导航任务。这些复杂的2D导航任务需要模型能够在全新的轨迹和环境中拥有更好的泛化能力。
**7） Deep Learning for Precipitation Nowcasting: A Benchmark and A New Model**
本文由香港科技大学与香港天文台合作完成。文中解决了降水即时预报问题。在该问题中，卷积长短记忆网络被证实优于传统的光流法，说明了深度学习在解决该问题上有巨大潜力。然而，卷积递归结构具有位置不变性，而自然的变换（例如旋转）往往是位置可变的。另外，因为基于深度学习的降水即时预报目前没有清晰的评价标准，本文提出了针对降水即时预测的新的模型和新的标准。具体地，本文提出了一种轨迹GRU (trajectory GRU) 模型来动态学习位置可变的递归结构。同时，本文也提供了一个实际生活中的大型数据库，一个新的训练损失函数和一个全面的评价标准，以方便未来在该问题的深入研究。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANK7oBJjFUDWJM1n94NhNtPLytnibpcxdjZtqID4XayXPUq2QVaoUGu8V3g5icPQesHMh3J9SMOQMpww/640?wx_fmt=png)
*轨迹RNN中，递归连接（recurrent connections）是模型动态确定的*
**8. MaskRNN: Instance Level Video Object Segmentation**
本文由伊利诺伊大学香槟分校和弗吉尼亚理工大学合作完成。实例级别的视频目标分割对视频编辑和压缩是非常重要的问题。为了考虑时序的一致性，本文提出了Mask RNN，一种在每一帧对每一个实例目标融合两个网络的结果（一个二值的分割网络提供掩膜mask，一个定位网络提供包围框bounding box）的递归网络方法。由于递归结构和定位结构，本文提出的方法能够利用视频的长跨度的时序结构，进一步提高视频物体分割性能。本文在高难度的DAVIS-2016, DAVIS-2017和Segtrack v2标准库上都取得了最前沿的实验结果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANK7oBJjFUDWJM1n94NhNtPLnpg9fxWtq2dVDNB5AaSVRFOCZjQxIekjHBR5HAgnFICQqomW4y6eYQ/640?wx_fmt=png)
**解 析 二：随 机 / 一 阶 优 化**
**Stochastic / first order optimization**
随机/⼀阶优化随着近年来的被于广泛研究而日渐完善。今年的NIPS论文在关于该topic的讨论中在对一些基础性的边界条件走向深化，对优化与泛化的关系，新的加速机制等又有新的认识。
**以下主要对4篇文章做一定介绍。**
**1) Robust Optimization for Non-Convex Objectives**
本⽂文由哈佛大学和微软研究院共同发表. 本文旨在解决成本函数具有不确定性前提下的鲁棒优化问题。实际中，成本函数往往特定于某些特定的情形而设计。在字符识别问题中，某些情形下，如在照片中识别字符时，通过优化成本函数得到的分类器必须能应对旋转和图案背景问题；在另⼀些情形下，如低分辨率图像中，通过优化成本函数得到的分类器必须应对像素失真。随着成本函数可能性的增加，为每⼀种情形训练独立的分类器并不明智。
为此，本文提出鲁棒优化模型，即存在多个成本函数时，我们最小化最坏情形下的成本函数。当前，性能领先的模型如深度神经网络往往旨在优化一个⾮非凸的成本函数，因⽽而使得理论上难于分析。本⽂文的⼀个贡献点在于提出一种\alpha－近似的随机oracle假设，并同时定义\alpha-逼近的鲁棒优化解。本⽂文证明，\alpha－逼近的鲁棒优化解可以在\alpha－近似的随机oracle假设下得到。而\alpha－近似的随机oracle假设对非凸目标也具有合理性。本文随后讨论了理论结果在统计学习理论和次模态优化中的应用。作为对理论结果验证，在实验中，作者在mnist数据集上⽐比较了本文方法和当前前沿方法的性能。实验显⽰示，本文的鲁棒优化方法具有最优的效果。
**2）Nonlinear Acceleration of Stochastic Algorithm**
本文由法国国家信息与自动化研究所（INRIA）完成。本⽂文作为该组作者NIPS 16的论文“Regularized Nonlinear Acceleration” 论文在随机算法方面的推广，作者讨论了在非线性加速机制在随机非加速算法和随机加速算法上的具体实现和收敛性的证明；同时，作者将该机制应⽤用到SGD，SAGA， SVRG, Katyusha等前沿随机算法中，都取得了不错效果。非线性加速机制不需要事先知道强凸性参数，而且该加速价值不需要再次利用梯度信息，而只需要对一黑箱优化算法的迭代解予以非线性平均，因⽽而具有良好的可应用性。随机优化算法是当前优化领域的前沿算法，非线性加速机制在该类型算法上的成功应⽤用必将对该机制的推广有重要意义。
**3）Limitations on Variance-Reduction and Acceleration Schemes for Finite Sum Optimization**
本文由以色列魏茨曼科学研究院完成。在当前一阶随机优化研究中，方差减少（VarianceReduction) 和加速（Acceleration）机制被广泛研究。这两种方法的成功应用使得随机梯度方法的收敛率不但不弱于准确梯度⽅方法，而且改善了对样本大小的依赖。人们通常认为这两种方法能成功应⽤用到随机梯度方法中是因为人们通常研究的经验风险最小化问题具有有限和结构。有限和结构促使我们可以利⽤用上⼀个epoch的梯度信息减少方差，同时也使算法表现得像一个确定性算法，因而该结构的成功应用对随机优化的发展至关重要。
然而本文指出有限和结构并不是方差减少和加速得以成功应用的充分条件，我们也需要知道单个函数的具体信息。另外，当前在加速方法上的研究都假定强凸性参数已知，当并未回答强凸性参数是否是实现加速所必须的。本文指出知道强凸性参数是实现对知名算法如SDCA，SVRG，SAG等加速所必须的；最后本文也讨论了有限和优化的迭代复杂度下界的问题。
**4） The Marginal Value of Adaptive Gradient Methods in Machine Learning**
本文由伯克利加州大学和丰田工业大学芝加哥分校完成。本文探讨了适应性（adaptive）方法和非适应性方法在优化与泛化上的差异。在近年来的深度学习研究中，由于Adam等适应性方法具有更快的收敛速度，⽽而逐步成为该领域的默认选择。但本文指出收敛更快的适应性方法不一定具有更好的泛化性。本文指出，适应性方法可能可以更快优化，也可能优化得到一个使成本函数更小的最优点，但这些都不能说明适应性方法更可取，因为学习的目的是为了追求可泛化性。本文通过构造一线性可分的二元分类问题对该现象予以说明。可以相信本文的发表对重新审视神经网络⾥里的优化具有重要意义。
**解 析 三：生 成 模 型**
**Generative Model**
在机器学习领域，生成模型的研究旨在设计有效的方法来学习训练数据的分布，换句话说，利用生成模型能生成与训练数据同分布的数据。传统有监督式方法（如图像分类等）需要大量的人工标注标签去训练分类模型，成本十分昂贵；而生成模型属于无监督式学习方法，在学习的过程不需要任何人工标注的标签。目前主流的生成模型包括GAN和VAE两种。粗略统计，本届NIPS共有40余篇生成模型相关的论文，主要以GAN为主，涵盖了算法扩展、模型应用、稳定性分析等主题。
本次会议的相关论文体现出两大特点：一、由于GAN的训练是不稳定和难以收敛，不少研究分析了传统GAN优化的动态特性，并尝试从更改目标函数或优化方式等方面设计更好的训练方法；二、不少研究将GAN与其他方法结合来扩展生成模型的功能，如半监督方法、贝叶斯方法等。
**我们重点关注了以下文章：**
**1）Gradient descent GAN optimization is locally stable**
本文由卡内基梅隆大学发表。原始GAN采用内外循环分开训练判别网络和生成网络。而本文的作者认为，在适当的条件下，同时对判别网络和生成网络进行小幅度的梯度更新，其所得到的均衡点（梯度等于零的点）仍然是局部渐近稳定的。另一方面，作者指出和原始GAN不同，Wasserstein GAN的均衡点存在不收敛圆（见下图第一列），因此Wasserstein GAN的均衡点不是渐进稳定的。为了克服这个困难，作者在梯度更新增加了一个正则项，并证明了增加正则项后，传统GAN和Wasserstein GAN的均衡点都是局部渐近稳定的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANK7oBJjFUDWJM1n94NhNtPLfMVMobRCVlX7uJn4u2HCYpYC9E5EqOxgNdGkkCnAicNUibPCLvQpfwug/640?wx_fmt=png)
**2）Unsupervised Image-to-Image Translation Networks**
本文由NVIDIA公司发表。很多计算机视觉的问题都能转换成从一个图片领域到另一个图片领域的转换问题，比如，高分问题可以看作从低分辨率的图片转换成高分辨率的图片；着色问题可以看作从灰度图转换成RGB图。本文主要讨论了无监督图片到图片的转换问题，即这两个领域的图片的对应关系是未知的。
这个问题的主要困难是，如何从两个领域的各自分布学习出他们的共同分布，从而实现领域转换。本文最大的特色是假设两个领域图片共享同一个隐层空间，并成功结合了两大主流生成模型GAN和VAE。具体模型构造过程为：作者通过VAE去刻画图片到隐层空间，隐层空间到图片重构的过程；由于是共享隐层空间，所以隐层空间到图片重构的过程其实就蕴含了不同领域之间的转换；然后再利用GAN来迫使转换后的图片分布与原图像分布一致；此外，作者还是使用了权重共享、循环一致性等性质来增强模型性能。本文方法应用到了街景转换、动物图像转换、人脸转换以及跨领域分类等任务中。以下是人脸风格转换的例子。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANK7oBJjFUDWJM1n94NhNtPLHX1FNjvdFT91tW1p8PxkHrem9NghTuBPckfvho99AjQ9iam71IibpMAg/640?wx_fmt=png)
**3）Good Semi-supervised Learning That Requires a Bad GAN**
本文由卡内基梅隆大学发表。作者研究了如何利用GAN来实现半监督分类问题。在传统GAN中，判别网络解决的是二分类问题，而在半监督GAN中，判别网络解决的是K+1分类问题。具体而言，假设需要对输入数据划分成K个类别，那么半监督GAN中，将真实数据划分到前K类，将生成的数据划分到第K+1类。本文的作者指出，当前的方法并没有很好回答：1）为什么加入生成器能提高分类性能？2）为什么当前算法不能同时得到有效的半监督分类和良好的生成器。因此，作者理论上证明了，有效的半监督分类效果需要的是一个差的而不是一个完美的生成器。为此，作者提出了一种新型的生成器，利用该生成器，能提高分类性能。本文的方法在各个公开数据集上均取得了最好的结果，见下图。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANK7oBJjFUDWJM1n94NhNtPLYePd6Z4vQuPM12T2TrAiae3eQvvE8aAwUBwlH9QvKdQESVvMEicheJPw/640?wx_fmt=png)
**腾 讯 AI Lab 入 选 论 文 详 解**
***论文按标题英文首字母排序**
被誉为神经计算和机器学习领域两大顶级会议之一的NIPS（另一个为ICML）近日揭晓收录论文名单，腾讯AI Lab共有八篇论文入选，位居国内企业前列，其中一篇被选做口头报告（Oral），该类论文仅占总录取数的1.2%（40/3248），我们将在下文进行解析。
**Oral 论文1. 去中心化算法能否比中心化算法效果更佳－一个关于去中心化的随机梯度方法研究**
Can Decentralized Algorithms Outperform Centralized Algorithms? A Case Study for Decentralized Parallel Stochastic Gradient Descent
本论文与苏黎世联邦理工学院、加州大学戴维斯分校和IBM合作完成。并行优化和计算效率是从大数据发掘智能的核心竞争力。为了提高效率，大多数的并行优化算法和平台集中在研究中心化的算法，比如Tensorflow、CNTK及MXNET。中心化的算法的主要瓶颈是上百个计算结点与（多个）中心节点之间的通讯代价和拥堵，严重的受制于网络的带宽和延迟。而这篇文章里则考虑去中心化的思路以减少通讯的代价。
尽管在去中心化的方法在控制领域已经有所应用和研究，但是考虑的是在特殊的去中心的拓扑结构的情况下，如何交换融合信息。而且已有的研究都没有表明如果二者都能用的情况下去中心的算法相对对于中心化的算法会有任何优势。这篇文章的主要贡献在于研究了一个去中心化的随机梯度方法，并且第一次从理论上证明了去中心化的算法可以比对应的中心化算法更加高效。同时本文通过大量的在深度学习上的实验和比较验证了作者理论。
这个发现将会打开未来大家对并行算法的思路，给并行系统带来更多的灵活性和自由度。我们相信将会对未来的机器学习平台和算法开发产生较大影响。
* 本文入选NIPS 2017 口头报告（Oral），论文占比为40/3248。
**2. 线性动态系统上的高效优化及其在聚类和稀疏编码问题上的应用**
Efficient Optimization for Linear Dynamical Systems with Applications to Clustering and Sparse Coding
本论文与清华大学和澳大利亚国立大学合作完成，其中的线性动态系统模型（LDS）是用于时空数据建模的一种重要的工具。尽管已有的理论方法非常丰富，但利用LDS进行时空数据的分析并不简单，这主要是因为LDS的参数并不是在欧氏空间，故传统的机器学习方法不能直接采用。
在这篇论文中，作者提出了一种高效的投影梯度下降法去极小化一个泛化的损失函数，并利用该方法同时解决了LDS空间上的聚类和稀疏编码问题。为此，作者首先给出LDS参数的一种新型的典范表示，然后巧妙地将目标函数梯度投影到LDS空间来实现梯度回传。与以往的方法相比，这篇文章中的方法不需要对LDS模型和优化过程加入任何的近似。充分的实验结果证明了这篇文章中的方法在收敛性和最终分类精度上优于目前最好同类方法。
**3. 通过斯坦因引理估****计高维非高斯多指数模型**
Estimating High-dimensional Non-Gaussian Multiple Index Models via Stein’s Lemma
本论文与普林斯顿大学和乔治亚理工大学合作完成，作者探讨了在高维非高斯设置中估计半参数多指数模型的参数化组分的方法。文中的估计器使用了基于二阶斯坦因引理的分数函数，而且不需要文献中做出的高斯或椭圆对称性假设。内部机构的研究表明：即使分数函数或响应变量是重尾（heavy-tailed）分布的，文中的估计器也能实现接近最优的统计收敛率。最后，作者利用了一个数据驱动的截断参数，并基于该参数确定了所需的集中度（concentration）结果。作者通过模拟实验对该理论进行了验证，对这篇文章中的理论结果进行了补充。
**4. 基于几何梯度下降方法的复合凸函数最小化**
Geometric Descent Method for Convex Composite Minimization
本论文与香港中文大学和加利福尼亚大学戴维斯分校合作完成，主要扩展了Bubeck, Lee和Singh近期提出的处理非光滑复合强凸函数优化问题的几何梯度下降方法。文中提出“几何邻近梯度下降法”算法——能够以线性速率收敛，因此能相比其他一阶优化方法达到最优的收敛速率。最后，在带有弹性网络正则化的线性回归和逻辑回归上的数值实验结果表明，新提出的几何邻近梯度下降法优于Nesterov's加速的邻近梯度下降法，尤其面对病态问题时优势更大。
**5. 基于混合秩矩阵近似的协同过滤**
Mixture-Rank Matrix Approximation for Collaborative Filtering
本论文与复旦大学和IBM中国合作完成，关于低秩矩阵近似方法（LRMA）现今在协同过滤问题上取得了优异的精确度。在现有的低秩矩阵近似方法中，用户或物品特征矩阵的秩通常是固定的，即所有的用户或物品都用同样的秩来近似刻画。但本文研究表明，秩不相同的子矩阵能同时存在于同一个用户-物品评分矩阵中，这样用固定秩的矩阵近似方法无法完美地刻画评分矩阵的内部结构，因此会导致较差的推荐精确度。
这篇论文中提出了一种混合秩矩阵近似方法（MRMA），用不同低秩矩阵近似的混合模型来刻画用户-物品评分矩阵。同时，这篇文章还提出了一种利用迭代条件模式的领先算法用于处理MRMA中的非凸优化问题。最后，在MovieLens系统和Netflix数据集上的推荐实验表明，MRMA能够在推荐精确度上超过六种代表性的基于LRMA的协同过滤方法。
**6. 凸差近似牛顿算法在非凸稀疏学习中的二次收敛**
On Quadratic Convergence of DC Proximal Newton Algorithm in Nonconvex Sparse Learning
为求解高维的非凸正则化稀疏学习问题，我们提出了一种凸差（difference of convex/DC）近似牛顿算法。我们提出的算法将近似牛顿算法与基于凸差规划的多阶段凸松弛法（multi-stage convex relaxation）结合到了一起，从而在实现了强计算能力的同时保证了统计性。具体来说，具体来说，通过利用稀疏建模结构/假设的复杂特征（即局部受限的强凸性和 Hessian 平滑度），我们证明在凸松弛的每个阶段内，我们提出的算法都能实现（局部）二次收敛，并最终能在仅少数几次凸松弛之后得到具有最优统计特性的稀疏近似局部最优解。我们也提供了支持我们的理论的数值实验。
**7. 用于稀疏学习的同伦参数单纯形方法**
Parametric Simplex Method for Sparse Learning
本论文与普林斯顿大学、乔治亚理工大学和腾讯AI实验室合作完成，作者关注了一种可形式化为线性规划问题的广义类别的稀疏学习——这类线性规划问题可以使用一个正则化因子进行参数化，且作者也通过参数单纯形方法（parametric simplex method/PSM）解决了这个问题。相对于其它相竞争的方法，这篇文章中的参数单纯形方法具有显著的优势：（1）PSM 可以自然地为正则化参数的所有值获取完整的解决路径；（2）PSM 提供了一种高精度的对偶证书停止（dual certificate stopping ）标准；（3）PSM 只需非常少的迭代次数就能得到稀疏解，而且该解的稀疏性能显著降低每次迭代的计算成本。
特别需要指出，这篇文章展示了 PSM 相对于多种稀疏学习方法的优越性，其中包括用于稀疏线性回归的 Dantzig 选择器、用于稀疏稳健线性回归的 LAD-Lasso、用于稀疏精度矩阵估计的 CLIME、稀疏差分网络估计和稀疏线性规划判别（LPD ）分析。然后作者提供了能保证 PSM 总是输出稀疏解的充分条件，使其计算性能可以得到显著的提升。作者也提供了严密充分的数值实验，演示证明了 PSM 方法的突出表现。
**8. 预测未来的场景分割和物体运动**
Predicting Scene Parsing and Motion Dynamics in the Future
本论文与新加坡国立大学、Adobe研究室和360人工智能研究院合作完成。无人车和机器人这样的对智能系统中，预期未来对提前计划及决策非常重要。文中预测未来的场景分割和物体运动帮助智能系统更好地理解视觉环境，因为场景分割能提供像素级语义分割 （即何种物体在何处会出现），物体运动信息能提供像素级运动状态（即物体未来会如何移动）。本文提出了一种全新的方法来预测未来的未观测到的视频场景分割和物体运动。用历史信息（过去的视频帧以及对应的场景分割结果）作为输入，文章中的新模型能够预测未来任意帧的场景分割和物体运动。
更重要的是，这篇文章中的模型优于其他分开预测分割和运动的方法，因为文中联合处理这两个预测问题以及充分利用了它们的互补关系。据内部统计，文中的方法是第一个学习同时预测未来场景分割和物体运动的方法。在大规模Cityscape数据集上的实验表明，本文的模型相比精心设计的基线方法，能获得显著更好的分割和运动预测结果。另外，这篇论文也展示了如何用机构内部的模型预测汽车转向角，获得的优秀结果进一步证实了该新模型学习隐含变量的能力。
**一分钟了解NIPS 2017**
NIPS，全称神经信息处理系统大会(Conference and Workshop on Neural Information Processing Systems)，由NIPS基金会主办，与ICML并列为机器学习和人工智能研究领域最具影响力的学术会议，也是出席人数规模最大的会议。
NIPS是由连接学派（Connectionist）神经网络的学者于1987年创办，今年是第31届，在美国加州的长滩举行，与当年在Denver举行的第一届NIPS参与人数只有一百多人相比，今年会议超过8500人注册，人数呈指数级增长。
本届NIPS一共收到3240篇论文，较去年明显增多，覆盖156个子领域，较去年增长150%。最受关注的是算法，其次是深度学习，紧跟其后的是应用。接收论文共679篇，录取率为21%。根据组委会提供的抽查统计数据，学术界和工业界提交论文的比例是12:88。
大会共设七场受邀报告（Invited talk）、53个研讨会（Workshop）和9个教程辅导（Tutorial），从贝叶斯学习到理解人类行为，覆盖了各方各面。
未来智能实验室致力于研究互联网与人工智能未来发展趋势，观察评估人工智能发展水平，由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎支持和加入我们。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXoz49H9w8lfGXmBjcGga9W15bmYAvZDqbbI8p0liaLcxEh1iao7YXIgJvp6MLHpskd7jHq8ibX0QzDQ/640?wx_fmt=png)


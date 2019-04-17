# Jeff Dean与David Patterson：不思考体系结构的深度学习研究者不是好工程师 - 知乎
# 



**机器之心原创，作者：邱陆陆。**

今年 1 月，谷歌人工智能负责人 Jeff Dean（当时还是谷歌大脑负责人）与 2017 年图灵奖得主、体系结构巨擘 David Patterson（当时获奖结果尚未公布）联合发表了题为《计算机体系结构黄金时代：赋能机器学习革命》的文章。文章指出，机器学习算法正在革命性地着手解决人类社会最为巨大的一些挑战，因此，不难想象，在不远的将来，无论是数据中心还是终端设备，都将会出现专用于机器学习计算的硬件。这样的硬件会具有什么特性？在文章中，作者列出了一系列硬件设计者需要考虑到的关键问题，这些问题对深度学习研究者也十分具有启发性。

另一方面，在摩尔定律逐渐失效和深度学习的算力需求如无底洞般膨胀的今天，深度学习研究者们也在思考：能否改进算法以使其更适应计算硬件？能否帮助优化系统配置？在近日举行的首届「清华-谷歌 AI 学术研讨会」上，Jeff Dean 谈了谈「接下来我们希望设计什么样的模型」，来自谷歌大脑的研究员 Azalia Mirhoseini 则给出了主题演讲「如何用强化学习方法进行系统优化」。
![](https://pic4.zhimg.com/v2-0171af166cec76e243f53cab07867717_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='720'></svg>)谷歌人工智能负责人 Jeff Dean
如果将这几份工作联系起来，我们似乎就能看出，在深度学习这个 Arxiv 论文增速超越摩尔定律的领域里，谷歌大脑的研究者们如何同时思考软硬件问题以实现系统最佳性能与最佳效率。
![](https://pic3.zhimg.com/v2-86625aab36b2f1583abff0edea7bdae2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='537'></svg>)「超越摩尔定律」的机器学习
在《黄金时代》论文里，Jeff 和 David 以谷歌开发的两代机器学习 ASIC（用于加速推理的 TPUv1 和用于加速训练的 TPUv2）为例，介绍了很多设计专用硬件的思路。进行硬件设计要着眼于至少 5 年后的模型：现在开始一款 ASIC 的设计，它大约可以在 2 年后投入使用，而一款专用硬件至少需要能够保持 3 年的竞争力才有价值。那么在这样的前提下，设计深度学习专用硬件要考虑哪些问题？在文章中，作者列出了六个这样的关键点，按照从「纯架构相关」到「纯机器学习算法相关」的顺序，分别是：训练、批规模、稀疏性与嵌入、参数量化与提炼、带有软记忆机制的神经网络以及元学习。




**训练**

谷歌从 2013 年开始设计的第一代 TPU 是针对推理而非训练工作设计的，一定程度上是为训练设计硬件架构要更加困难：
- 第一，训练的运算量是推理的 3 倍以上。
- 第二，因为要将全部激活值存储以进行反向传播，训练过程所需的存储也远比推理高。
- 最后，训练的可扩展性远不如推理，因为需要进行大量昂贵的序列计算。

但是，为训练设计 ASIC 是有价值的，因为研究者的时间是宝贵的而耐心是有限的，如果一次实验需要跑 30 天甚至更长，大概大多数人会选择放弃探索这个方向。

第二代 TPU 就面向训练开发。Jeff 在多次演讲中提到了一些 TPU v2 的成功应用案例，包括用四分之一个 pod 把谷歌的搜索排序模型的训练提速到 14.2 倍，把处理图片的模型训练提速到 9.8 倍等。
![](https://pic4.zhimg.com/v2-f603189ed783fe5675768ff0562eda4f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='370'></svg>)TPUv2 在谷歌内部的应用案例
而且，TPUv2 具有几乎是线性的扩展性，64 块 TPU 可以组成一个 TPU pod，提供最高 11.5 PFLOPS 的算力。用一块 TPUv2 把 ResNet-50 训练到 76% 准确度需要 1402 分钟，用二分之一个 pod（32 块 TPUv2）只需要 45 分钟（31.2 倍速）。
![](https://pic3.zhimg.com/v2-8d9166a14151530ca6bafc1a2cb0c19a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='307'></svg>)TPUv2 近乎线性可扩展
虽然 TPU 云价格不菲，但时间就是生命，生命就是金钱。另外，如果你是一位有志于机器学习研究并致力于开源自己的工作，谷歌正以 TensorFlow 研究云的形式向研究者免费提供一千块 TPU。




**批规模（batch size）**

批量是越大越好还是越小越好？这是一个在研究上仍然有争议的问题。

直觉上，理想选择是带有动量的批规模为 1 的随机梯度下降（SGD with momentum at a minibatch size of 1），这时单位计算量带来的准确率提升最大。另外选择一个大于 1 的批规模相当于把输入维度直接提升了一维（Jeff 原话：一想到这儿我就头痛。）然而在当前用于训练的硬件中，以 GPU 为例，它执行程序时的基本单元，即每个 warp 包含 32 个线程（threads），因此如果你的批规模不是 32 的整数倍，将会导致效率降低，所以现行的模型通常都采用 32 或者 64 作为批规模。
![](https://pic2.zhimg.com/v2-f959f81d52e3551723eaa306e64c0085_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='542'></svg>)批规模与计算效率
但是，从 2017 年开始，有一些看起来前景明朗的研究显示，我们可以用 8192 甚至 32768 作为批规模来高效训练用于图像的卷积神经网络。
![](https://pic3.zhimg.com/v2-71dc43e928b820e2a9b680ce0d7ed36e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='778' height='474'></svg>)直到批规模上升到 8k 左右，验证集的错误率仍然能保持相对低的水平 
*source：Accurate, Large Minibatch SGD: Training ImageNet in 1 Hour (1706.02677)*

值得一提的是，Yann LeCun 对于增加批量持有强烈反对态度，他曾经在今年 4 月份转发一篇支持小批量的论文称，「用巨型批量做训练有害健康……选择 32 而不是 1 做批量只能说明我们的硬件很差劲。」

哦，8192 那篇论文是 Facebook 的工作，作者列表里包括 Ross Girshick，贾扬清，还有何恺明……




**稀疏性与嵌入（Sparsity and Embeddings）**

「我们想要更大的模型，但希望每一个样本只激活它的一小部分。」又是一个 Jeff 反复提到的趋势。
![](https://pic4.zhimg.com/v2-5381d1a58ce5f34396d4927ad510cf7b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='754'></svg>)「我们想要什么样的模型？」
大模型是好的，因为巨大的参数量意味着我们可以记住数据集方方面面的特性。但是如果我们在处理每一个数据的时候都需要激活整个模型，这意味着巨大的计算成本。因此，最理想的状态是拥有一个可以分割成众多独立的小部分的庞大模型，每一部分都承担不同分工，当数据进入模型时，模型按任务需求激活少数几个部，让大部分保持闲置状态。这样的特性又可以称作「粗粒度稀疏性」。
![](https://pic3.zhimg.com/v2-8e66920c9765b8614628192ac365d24a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='424'></svg>)粗粒度稀疏性Source: Exploring the Regularity of Sparse Structure in Convolutional Neural Networks (1705.08922)
谷歌在 ICLR2017 的一篇论文上就提出了一个叫做混合专家层（MoE）的概念。每一个「专家」相当于神经网络中的一小撮参数，只不过比普通神经网络里的参数更容易训练，而一层由超过 2000 个专家组成。
![](https://pic4.zhimg.com/v2-2db0d59d9c53eca77aad0988957cf8db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='605'></svg>)MoE 层的结构Source: Outrageously Large Neural Networks: The Sparsely-Gated Mixture-of-Experts Layer (1701.06538)
训练过程中，除了学习模型参数，还要学习如何做路由选择（routing），即如何根据一个样本选择激活的「专家」。在语言任务里，模型学会了如何根据语境选择「专家」：专家 381 擅长谈论科学研究，专家 752 擅长「领导力」，如果涉及速度，那就交给专家 2004 吧。
![](https://pic4.zhimg.com/v2-47ec798b2e854576460213fc87427383_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='473'></svg>)路由选择学到了什么 Source: Outrageously Large Neural Networks: The Sparsely-Gated Mixture-of-Experts Layer (1701.06538)
在英-法翻译任务里，相比于之前的 STOA 模型 GNMT，模型的规模上升了 35 倍，却可以用更少的 GPU 在六分之一的训练时间内完成。

相比于 MoE，应用更为广泛的动态路由的案例是嵌入机制。无论是将单词从数万维的独热向量映射到几百维的词嵌入，还是给每一个 YouTube 视频赋予一个数千维的、能够捕捉它与其他视频间关系的表征，都是一个需要为了一个样本，从庞大的数据结构（可能高达数百 G）中随机地读取非常少的数据（数十或数百字节，不到 1KB）。

现有硬件结构里还鲜有专门处理动态路由选择的高效读取的解决方案。




**参数量化与提炼（Quantization and Distillation）**

稀疏性和嵌入的共同点是保留「大模型」，而关注如何能精确定位到其中的「小部分」。参数量化与提炼则直接追求「小模型」。

参数量化的是低精度运算的另一种说法。

现在常见的做法是，在训练阶段采用浮点数，而在推理中采用定点数。例如在 TPU 的例子中，所有的推理只采用 8 比特定点数表达。其实现原理是在完成训练后，根据各层的参数和激活部分的最大值和最小值，找到表达其整数部分所需的最小比特数，用此表示，然后用 8 比特中剩下的部分表示其小数点后部分。实证研究显示，将精度从 32 比特降低到 8 比特，只会少量影响 GoogLeNet 和 VGG-16 的表现，但是如果继续下降到 6 比特，模型效果就会受到显著影响。
![](https://pic3.zhimg.com/v2-1a941e78af944c2eb5e5a04f1db8884e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='574'></svg>)推理参数量化对精度的影响Source: Going Deeper with Embedded FPGA Platform for Convolutional Neural Networkcadlab.cs.ucla.edu/~jaywang/papers/fpga16-cnn.pdf 
文中提到，只有很少的研究关注了如何在训练阶段采用低精度运算，大多数结果都仍然集中在 MNIST，CIFAR-10 等小数据集上。不过低精度训练也在逐渐获得更多关注，ICLR2018 上，百度和英伟达提出了「混合精度训练法」，在前向、后向计算中使用 FP16 运算，在权重更新时使用 FP32 计算，在 ImageNet 上的分类任务、Pascal VOC 2007 上的检测任务、WMT15 上的翻译任务等多个大型数据集的任务中，达到了使用 FP32 所获得的准确率，同时节省了算力需求和近半的存储需求。如今，英伟达已经给出了用混合精度进行训练的 SDK 范例。

提炼方法是 Hinton 在 NIPS2014 上提出的，试图以先让复杂模型进行学习分类问题，然后将最后一层 softmax 学到的软分类视为知识，训练简单模型以预测软分类。这样的方法得到的简单模型（层数更少、每层神经元更少）也能达到复杂模型同样的准确度。提炼方法让人们思考，是否能够直接训练小模型。小模型和大模型需要的专用硬件特性会截然不同，因此模型的发展方向也是硬件发展方向的重要影像因素。




**带有软记忆机制的神经网络（Networks with Soft Memory）**

这一部分强调的是一些对存储和存储访问有特殊需求的深度学习技巧，例如注意力机制。传统的记忆机制每次只要访问存储数据的表中的一个值，但是以注意力机制为代表的软记忆机制则需要对表内的所有值进行加权平均。

相比于特定运算的加速，当前问世的或已经进入开发周期后段的深度学习 ASIC 更强调数据流与存储的优化。原 Movidius CEO Remi El-Ouazzane 在谈论其视觉处理单元 VPU 的设计理念时提到，VPU 中的几乎所有架构设计都是为了同一个目标：优化数据流。在当前的终端深度学习计算中，用于数据传输的能量消耗是用于计算的 10 倍乃至更多，因此要使性能最大化并将功耗降至最低，唯一的方法就是增加数据的本地性，减少外部内存访问次数。致力于加速训练的 Intel Nervana NNP 的逻辑也是这样。

FPGA 的逻辑也是如此。大量的引脚和可以根据算法定制数据通路（datapath）的逻辑单元，让它不需要像 GPU 一样需要反复调取片外存储中的数据，理想状态时，只要数据一次性流进去再流出来，算法就完成了。




**元学习（Learning to Learn, L2L）**

深度学习相比于机器学习的「进步」，就是将人工选择的固定特征提取过程变成了机器选择的可训练特征提取过程。研究者只需要选择一系列基本模型结构和超参数，就可以由机器接手，进行特征提取和分布拟合。

在上述的五个部分里，无论模型采用什么结构、技巧，作出这些决策都仍然是人类的工作。而在元学习的设想里，人类的决策工作进一步被大量的计算和机器自动完成的实验所替代。

在自动机器学习技术的种种方法中，谷歌选择了强化学习的方法。模型的准确度被视为「奖励信号」。在获得了 ICLR2017 最佳论文的《用强化学习进行神经网络结构搜索》中，谷歌的研究员分别为 CIFAR-10 和 PTB 数据集搜索到了最佳的 CNN 和 LSTM RNN 结构。
![](https://pic3.zhimg.com/v2-9d430e56f326f1d305ef994416f16ed2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)普通 LSTM 结构和结构搜索出的结构Source: Neural Architecture Search with Reinforcement Learning (1611.01578)
而事实上不只是模型结构，「用强化学习进行元学习」的思路适用于深度学习的方方面面：选择输入数据预处理路径，选择激活函数、选择优化与更新策略，以及，选择硬件配置。

本次谷歌大脑研究员 Azalia 的演讲就是以优化硬件配置为主题。传统的硬件配置多数以贪婪启发式方法为基础，需要工程师对硬件的方方面面，从算力到带宽，都有深刻的理解。而即便如此，随着模型越来越大，使用的设备越来越多，得到的配置方案也越来越难具有可泛化性。
![](https://pic1.zhimg.com/v2-c2519d18ddfbdf5c05740d2e6fd78fdc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='717'></svg>)谷歌大脑研究员 Azalia Mirhoseini
因此，将一种特定配置的收敛时间的期望作为奖励信号来将各运算分配到不同设备上，就变成了一个有吸引力的解决方案。算法学到了并不符合人类直觉的配置，但比专家设计出来的方案快 27.8%，节约近 65 个小时。
![](https://pic2.zhimg.com/v2-89b2b044b26df08b436bee3fc34a39f1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='256'></svg>)![](https://pic3.zhimg.com/v2-d6ee3cf7ad17340cc22b93de04fa5cfe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='236'></svg>)![](https://pic3.zhimg.com/v2-76c8955edb3c0b5c6fe38af0e92fd712_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='795'></svg>)元学习得到的运算硬件配置以及效果Source: Device Placement Optimization with Reinforcement Learning (1706.04972)
元学习指向的道路是让我们更高效地利用大规模的计算资源，同时「节省」机器学习专家的劳动力。除此之外，在这个深度学习算法和计算设备都在快速更新迭代的时期，元学习也奠定了快速进行软硬件结合的系统设计的基础。

将所有这些愿景综合起来，深度学习下一个阶段会以何种形式出现？




Jeff 在演讲中概括为：
- 只是稀疏地激活的更为庞大的模型。
- 能够解决多个任务的单一模型。
- 在庞大的模型中动态地学习新路径，并不断增加新路径。
- 专用于进行机器学习超算的硬件
- 高效地配置在硬件上的机器学习模型。
![](https://pic4.zhimg.com/v2-048335b028ad43cc73e3f5a9db053bb3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='606'></svg>)紫色模块为新任务和为此而增加的新节点，加粗的红线表示用于解决新任务的新路径
你的研究思路是否有助于其中一个目标的实现？又是否将从这样的模型中获益？

无论答案是什么，确定的一件事情是：研究者、工程师、体系结构设计者，在当下，想要朝通用人工智能进攻的道路上前行，这些身份，一个都不能少。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)






# 深到骨子里的自律，是每周坚持刷几篇最新论文 | PaperDaily #10 - Paper weekly - CSDN博客





2017年11月06日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：380












![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。

这是 PaperDaily 的第 **10** 篇文章
[ 自然语言处理 ]
![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/?wxfrom=5&wx_lazy=1)
**Don't Decay the Learning Rate, Increase the Batch Size**

**@RamonYeung 推荐**

#Learning Theory

埋下伏笔：来自 Google Brain 的论文。

文章主要讲明白四点：

1. 主动降低学习率（Decaying the Learning Rate）是模拟退火，因为降低学习率导致 SGD 训练时，随机波动的幅度也减少了；

2. 我们提出一个替代方案：不降低学习率，在训练时提高 batch_size。在训练相同 #epochs 下，这种策略在测试集上达到了几乎相同的模型性能，但是显着减少了参数更新的次数。我们的方案不需要通常的训练做法（Fine-tuning）。当需要将学习率降低为原来的 1/α 时，我们将 batch_size 改为原来的 α 倍。

3. 参数更新次数可以进一步缩短：
- 
提高学习率 ϵ，同时按 B∝ϵ 的方式提高 batch_size；

- 
提高动量系数（Momentum Coefficient）m, 同时按 B∝1/(1−m) 的方式提高 batch_size。


4. 结合这些策略，我们在 ImageNet 上对 Inception-ResNet-V2 进行 2500 次参数更新，达到 77％ 的验证集精度。为了实现这一点, 训练时 batch_size=65536。相比之下，Goyal 等人（2017）需要 14000 个参数更新, 使用 ResNet-50 达到 76％ 的验证精度，batch_size=8192。

前提是：你有钱买更多的 GPUs 装入更大的 batch_size。

论文链接

http://www.paperweekly.site/papers/1071

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/?)
**Memory-augmented Neural Machine Translation**

**@yuer3677 推荐**

#Neural Machine Translation

该篇文章将传统 SMT 的词对应关系以 Memory 的形式组合到 NMT 上，结合了两者的优势。一方面增加了 rare word 的出现概率，提升了翻译 performance；另一方面提供了一种解决 unknown words 的思路。该方法尤其在小数据集上效果明显。

论文链接

http://www.paperweekly.site/papers/1037

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/?)

**Sequence-to-Dependency Neural Machine Translation**

**@jerryshi 推荐**

#Text Classification

**1. 文章有什么贡献？**提出了一种新的 Sequence-to-Dependency Neural Machine Translation (SD-NMT) 的方法，来将目标语言句法知识利用进了 NMT 系统，相比起没有使用句法知识的基准 NMT 系统，性能得到了相对的提高。 

**2. 研究的问题有何价值？**目前的 NMT 系统主要是直接用线性 RNN 来进行 Seq2Seq，但是这样的系统对于捕捉不明显的长距离词的依存还是有很大难度的。因此在解码的时候，将句法知识考虑进解码器中后，可以提高翻译结果语法的正确性，并且也可以利用局部依存信息来生成之后的词语。 

**3. 研究问题有什么挑战？**一，如何利用 RNN 来构建句法结构；二，如何在一个神经网络中，有效地同时进行词语生成，还有句法结构的构建；三，如何有效地利用目标语言的句法背景，来帮助词语的生成。 

**4. 本文的解决思路？**
- 
利用了两个 RNN 网络，Action RNN 和 Word RNN，分别进行词语生成和句法结构的构建。Action RNN 利用了 transition-based dependency parsing（基于转换的依存句法分析）中的 arc-standard shift-reduce algorithm 算法，来生成构建所需依存结构的动作。而同时因为两个 RNN 生成的的序列长度不一致，所以 Word RNN
 利用了些技巧，使得它能够参考 Action RNN 的结果输出词语，或者保持不变以和 Action RNN 的时序保持一致。 

- 
通过定义生成依存结构时的栈顶部两个词语，最左和最右修饰语的一元和二元语言特征，生成相对当前词汇的局部依存背景。之后将这个背景与 Word RNN 的输出结合起来，帮组生成新的词汇。


论文链接

http://www.paperweekly.site/papers/994



[ 计算机视觉 ]
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/?)
**Margin Sample Mining Loss: A Deep Learning Based Method for Person Re-identification**

**@LUOHAO 推荐**

#Person Re-identification

1. 文章提出了一种新的度量学习方法 Margin sample mining loss - MSML。Triplet loss 是一种非常常用的度量学习方法，Quadruplet loss 和 Triplet hard batch loss（TriHard loss）是它的两个改进版本，而 MSML 是吸收了 Quadruplet loss 和 TriHard loss 两个优点的综合体，实验证明 MSML
 能够在 person ReID 的公开数据集上取得很好的结果；

2. 这个方法不止可以应用于 person ReID，而是一种通用的度量学习方法，进一步可以延伸到图像检索等相关的各个领域。

论文链接

http://www.paperweekly.site/papers/1069




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/?)
**Fader Networks: Manipulating Images by Sliding Attributes**

**@Gapeng 推荐**

#GAN

跟其他 GAN 方法不同，Fader nets 通过在隐层编码引入对抗来实现属性相关编码和属性无关编码的分离。D 尝试从 Encoder 的编码中预测出属性，而 Encoder 则尽量学习不带属性的编码，以使 D 的预测失效。Decoder 接收 Encoder 的编码以及额外的属性作为输入，以重构/修改图像。文章的效果非常好，目前复现存在难度，过段时间作者会开源代码。

论文链接

http://www.paperweekly.site/papers/1009




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/?)
**Scale-adaptive Convolutions for Scene Parsing**

**@lisabug 推荐**

#Semantic Segmentation

ICCV'17 Poster。 简而言之，文章提出了一个 scale-adaptive 的 dilation conv层。

1. 与 fixed coefficient 的 dilation conv 相比，本文提出的 scale-adaptive conv 层通过前一层的 feature 对于每一个位置预测一个 variant coefficient。对于大物体，我们应该需要更大的感受野，因此得到一个大的系数；对于小物体，我们应该关注细节而忽略背景，则缩小感受野更加强调细节的 feature。可以参考文章图 3 理解；

2. dilation 系数非整数时，使用常见的 STN 中的 bilinear interpolation 做到可以 e2e 的学习；

3. 合理的初始化系数回归层的参数，并在训练时进行对其 clip，使得 dilation 系数位于一个合理的范围内；

4. 文章提到在高层使用效果较好；

5. 即插即用，较小的额外参数和计算开销。相比 baseline 不加多尺度训练的情况下可以在 cityscape 上涨 2 个多点。

论文链接

http://www.paperweekly.site/papers/1056




[ 机器学习 ]

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaPFKKcQtakJkrJqiay67wic4UibzIoJEFesEXzBdgvbKrU3pQbdFib3fEibQ/?)
**Paraphrase Generation with Deep Reinforcement Learning**

**@paperweekly 推荐**

#Deep Reinforcement Learning

用增强学习来解决端到端 paraphrase 生成问题，取得了不错的效果。

论文链接

http://www.paperweekly.site/papers/1048




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkFFwOYfF41kUMpx9QrhiacsUsaDJKp7afm9SFyWQgUwpPItQo9UH0zH3GFjzTw4LJezwoExXFCzpA/?)
**A generative vision model that trains with high data efficiency and breaks text-based CAPTCHAs**

**@AkiyamaYukari 推荐**

#Generative Networks

一种新的概率生成模型（probabilistic generative model ）被提出了，而且在一定任务上表现超过了现有的神经网络。

论文链接

http://www.paperweekly.site/papers/1005






![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmdTOOt6xMGO8q70RUDWEzldUCgrhG69LwL4uQCmiaK8A19jufpMJdHTxzZ5KGRrJNpEgnHJRfNk1A/?)
**Reparameterizing the Birkhoff Polytope for Variational Permutation Inference**

**@tmasada 推荐**

#Bayesian Inference

This paper proposes new probability distributions on permutations. The variational inference is performed by using reparameterization trick.

论文链接

http://www.paperweekly.site/papers/1007




**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**

******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)




▽ 点击 | 阅读原文|
 查看更多优质论文





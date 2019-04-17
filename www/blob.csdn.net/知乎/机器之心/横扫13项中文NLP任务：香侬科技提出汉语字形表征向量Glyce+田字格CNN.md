# 横扫13项中文NLP任务：香侬科技提出汉语字形表征向量Glyce+田字格CNN - 知乎
# 



选自arXiv，作者: Wei Wu、Yuxian Meng等，机器之心编译。

> 最近，香侬科技发表研究，提出了一种汉语字形向量 Glyce。该研究基于汉字的进化过程，采用多种汉字古今文字和多种书写风格，专为中文象形字符建模设计了一种 [CNN](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650756338%26idx%3D2%26sn%3D3e84bd7a4448e851377b7d21be1a0cdf%26chksm%3D871a908cb06d199afccff01e5bd0cf636b17daeb3104351db294cf7b90155fd52644d14822b8%26token%3D1081160574%26lang%3Dzh_CN) 架构——田字格 CNN。Glyce 在 13 个（几乎所有）中文 [NLP](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650756338%26idx%3D2%26sn%3D3e84bd7a4448e851377b7d21be1a0cdf%26chksm%3D871a908cb06d199afccff01e5bd0cf636b17daeb3104351db294cf7b90155fd52644d14822b8%26token%3D1081160574%26lang%3Dzh_CN) 任务上达到了当前最佳性能。

汉字是一种象形文字，可以被分解为更小、更基础的音义结合体：象形（即对象的图像化表述，如「亻」、「日」、「木」）和形声（用于发音，如「晴」中的「青」）。汉朝的《说文解字》利用图形化的部首来索引汉字，这一传统一直沿用到今天。许多汉字由图像演化而来（如图 1 所示），因此汉字的语素中编码着丰富的语义信息。
![](https://pic2.zhimg.com/v2-bf974ee963afb92c3900e31d94388db9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='749'></svg>)图 1：许多汉字由图像演化而来（图源：维基百科）
分布式表征（Mikolov et al., 2013; Pennington et al., 2014）的出现为表征文本语义提供了一种简洁的方式，并已广泛应用于中文自然语言处理。主流深度学习算法大多使用词或字符作为基础语义单元（Zheng et al., 2013; Chen et al., 2015b,a; Xu et al., 2016; Cai and Zhao, 2016），在词/字符层面学习嵌入表征。然而字形表征（Glyph representation）却很少被使用到。

很明显，考虑汉语图形信息应该有助于语义建模。最近的研究间接地支持这一点：偏旁部首被证明在许多语言理解任务中有用（Shi et al., 2015; Li et al., 2015b; Yin et al., 2016; Sun et al., 2014; Shao et al., 2017）。研究表明，使用五笔编码技术可以提高中英机器翻译的性能（Tan et al., 2018）。虽然部首和五笔表征一定程度上编码了一些关于字符结构的信息，有助于构建更好的字符表征，但部首和五笔表征都是用 ID 编码的，因此无法触及更深的汉语图形信息。

近年来，一些研究者尝试在字符的视觉特征上应用基于 CNN 的算法。不幸的是，他们的研究没有实现持续的性能提升（Liu et al., 2017; Zhang and LeCun, 2017），一些研究甚至得出了负面结果（Dai and Cai, 2017）。例如，Dai 和 Cai（2017）在汉字图像上运行 CNN 来获取汉字表征，然后在下游的语言建模任务中使用这些表征。他们发现，字形的加入实际上降低了性能，基于 CNN 的表征无法为语言建模提供额外的有用信息。Liu 等人（2017）和 Zhang、LeCun（2017）在文本分类任务上利用相似的策略测试了这一思路，结果模型性能只在非常有限的几种情况下有所提升。

研究者认为之前基于 CNN 的模型 (Dai and Cai, 2017) 得到消极结果的原因如下：1）未使用正确的文字版本：汉字系统经过了很长的进化过程，如图 2 所示。最著名的版本包括甲骨文（公元前 2000 年 - 公元前 300 年）、隶书（公元前 200 年 - 公元 200 年）、篆书（公元前 100 年 - 公元 420 年）、魏碑（公元 420 年 - 公元 588 年）等。这一文字进化过程是遵循特定模式的。最初的汉字易于绘制，然后逐渐转变成容易书写。此外，汉字的象形性、具象性逐渐弱化。

目前最广泛使用的汉字版本是简体中文，这种字体易于书写，但不可避免地丢失了大部分象形信息。这导致了仅使用简体中文训练的模型性能不好。2）未使用合适的 CNN 结构：与大小为 800*600 的 ImageNet 图像不同，汉字图像的大小要小得多（通常是 12*12）。这就需要不同的 CNN 架构来捕捉字符图像的局部图信息。3）之前的研究未使用调节函数（regulatory function）：与包含数千万数据点的 ImageNet 图像分类任务不同，汉字仅有约一万个。因此辅助训练目标对防止过拟合、提升模型泛化能力非常关键。本研究使用图像分类作为辅助训练目标。
![](https://pic4.zhimg.com/v2-04a80a08d60a4a6b7e6b43b14e05ff9b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='668'></svg>)图 2：汉字的进化过程（图来自网络）
在本文中，研究者提出了 Glyce，一种汉字表征字形向量。他们将汉字当作图像，并使用 CNN 来获取它们的表征。作者使用以下技术解决了上面提到的问题：
- 研使用历史汉字和当代汉字（如青铜器铭文、隶书、篆书和繁体中文等）以及不同的书写风格（如草书）来丰富字符图像的象形信息。
- 研究者使用了 Tianzige-CNN (田字格) 架构，该架构专为中文象形字符建模而设计。
- 通过添加图像分类损失函数，研究者利用多任务学习方法增强了模型的泛化能力。

**论文：Glyce: Glyph-vectors for Chinese Character Representations **
![](https://pic2.zhimg.com/v2-bc5874cd8ebaecec4de5ebbeeb6a22dd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='293'></svg>)
论文地址：[https://arxiv.org/abs/1901.10125](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1901.10125)

摘要：直观上来看，针对中文等语素文字的 NLP 任务应该受益于这些语言中的字形信息。然而，由于字形中缺乏丰富的象形信息，以及标准计算机视觉模型在字符数据上的泛化能力较差，现在还未找到有效利用字形信息的方法。

本文提出了汉字表征字形向量 Glyce，填补了这一空白。本文有三大创新：1）使用了古代汉字（如青铜器铭文、篆文、繁体中文等）来丰富文字中的象形证据；2）设计了适合汉字图像处理的 CNN 架构；3）将图像分类作为多任务学习设置中的辅助任务，以提高模型的泛化能力。

我们首次展示了基于字形的模型能够在很多中文 NLP 任务中持续超过基于单词/字符 ID 的模型。通过 Glyce，我们在 13 个（几乎所有）中文 NLP 任务上达到了当前最佳性能，包括字符级语言建模、词级语言建模、中文分词、命名实体识别、词性标注、依存句法分析、语义角色标注、句子语义相似度、句子意图识别、中英机器翻译、情感分析、文档分类和语篇分析任务。

## **3 Glyce**

**3.1 使用历史汉字**
![](https://pic2.zhimg.com/v2-29163deefac9caafff925527e94a45f1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='227'></svg>)表 1：Glyce 使用的历史汉字和书写风格
**3.2 Glyce 的 Tianzige-CNN 架构**
![](https://pic1.zhimg.com/v2-a085e82faa96e299cb41acf99d7111e4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='228'></svg>)图 3：Glyce 的 CNN 架构![](https://pic1.zhimg.com/v2-aef37d3635bfb95115d910ee5012642c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='403'></svg>)表 2：Glyce 中的 tianzige-CNN 架构![](https://pic4.zhimg.com/v2-89acd5719f2aa14583147088d139de2b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='559'></svg>)图 4：如图所示，田字格是 2 × 2 的结构，其模式表明汉字偏旁部首的排列以及汉字的书写笔画![](https://pic1.zhimg.com/v2-9d4029ed4a813aacfb11b5da6c64f814_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='243'></svg>)图 6：组卷积机制![](https://pic2.zhimg.com/v2-6aea037ca2dcc2ec372c6e06f301c099_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='445'></svg>)图 5：Glyce 字符嵌入和词嵌入概览
## **4 实验结果**

该研究在语言建模、命名实体识别、词性标注等 13 个任务上对 Glyce 的性能进行了测试，并与其它模型做了对比。

4.1 任务 1：字符级语言建模
![](https://pic2.zhimg.com/v2-01ca2075521dbba8dc619905cad12d05_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='514'></svg>)表 3：字符语言建模任务结果
4.2 任务 2：词级语言建模
![](https://pic3.zhimg.com/v2-5186d8dd61ea6afcbb30e5da6eeb9bee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='385'></svg>)表 4：词级语言建模的 ppl
4.3 任务 3：命名实体识别
![](https://pic1.zhimg.com/v2-8858aaa3ee01bf87aceba61d95949988_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='545'></svg>)
4.4 任务 4：中文分词
![](https://pic4.zhimg.com/v2-0c041664422364a9f4ce3abe76fa7db7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='302'></svg>)
4.5 任务 5：词性标注
![](https://pic1.zhimg.com/v2-b74194ff3177eafb808e7f676f995704_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='297'></svg>)
4.6 任务 6：依存句法分析
![](https://pic4.zhimg.com/v2-65238aeaeb33d946f938c1dda34a5bd3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='286'></svg>)
4.7 任务 7：语义角色标注
![](https://pic4.zhimg.com/v2-1a66c54faf5f8fb83bc8b040c6e1bd27_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='219'></svg>)
4.8 任务 8：句子语义相似度
![](https://pic2.zhimg.com/v2-f181d385fc643142ce564d8e8cc7cc05_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='241'></svg>)
4.9 任务 9：意图识别
![](https://pic4.zhimg.com/v2-68016f7a39d3694af57c1af16603a697_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='221'></svg>)
4.10 任务 10：中英机器翻译
![](https://pic1.zhimg.com/v2-3c1b89c0c2854770f882b02662a5d008_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='366'></svg>)
4.11 任务 11：情感分析
![](https://pic4.zhimg.com/v2-725110da0ede8e8ad23441303f5735af_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='365'></svg>)
4.12 任务 12：文档分类
![](https://pic1.zhimg.com/v2-54423c1ee75c8d9d052b03806c739058_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='422'></svg>)
4.13 任务 13：语篇分析
![](https://pic1.zhimg.com/v2-32181cd6da9597256d6847d7230b1fec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='279'></svg>)




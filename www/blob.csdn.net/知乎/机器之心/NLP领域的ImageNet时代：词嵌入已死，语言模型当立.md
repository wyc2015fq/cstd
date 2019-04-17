# NLP领域的ImageNet时代：词嵌入已死，语言模型当立 - 知乎
# 



选自the Gradient，作者：Sebastian Ruder，机器之心编译。

> 计算机视觉领域常使用在 ImageNet 上预训练的模型，它们可以进一步用于目标检测、语义分割等不同的 CV 任务。而在自然语言处理领域中，我们通常只会使用预训练词嵌入向量编码词汇间的关系，因此也就没有一个能用于整体模型的预训练方法。Sebastian Ruder 表示语言模型有作为整体预训练模型的潜质，它能由浅到深抽取语言的各种特征，并用于机器翻译、问答系统和自动摘要等广泛的 NLP 任务。Ruder 同样展示了用语言模型做预训练模型的效果，并表示 NLP 领域中的「ImageNet」终要到来。
![](https://pic2.zhimg.com/v2-a3ea99b1bd2d33963981341d464b4c51_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='624'></svg>)
自然语言处理（NLP）领域正在发生巨变。

长期以来，词向量一直是自然语言处理的核心表征技术。然而，其统治地位正在被一系列令人振奋的新挑战所动摇，如：ELMo、ULMFiT 及 OpenAI transformer。这些方法因证明预训练的语言模型可以在一大批 NLP 任务中达到当前最优水平而吸引了很多目光。这些方法预示着一个分水岭：它们在 NLP 中拥有的影响，可能和预训练的 ImageNet 模型在计算机视觉中的作用一样广泛。




**由浅入深的预训练**

预训练的词向量给 NLP 带来了很大的提高。2013 年提出的语言建模近似——word2vec 凭借其效率和易用性在硬件速度慢得多且深度学习模型没有得到广泛支持的时代得到采用。此后，进行 NLP 项目的标准方式基本保持不变：通过 word2vec 和 GloVe 等算法对大量未标记数据进行预处理的词嵌入被用于初始化神经网络的第一层，其它层随后在特定任务的数据上进行训练。在大多数训练数据有限的任务中，这种方法帮助提高了两到三个百分点。尽管这些预训练的词嵌入颇具影响力，但它们也有局限：它们仅包含模型第一层的先验知识——网络的其余部分还需要从头开始训练。
![](https://pic4.zhimg.com/v2-c10044c23237144aae8329ca483b3453_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='381'></svg>)word2vec 捕捉到的关系。（来源：TensorFlow tutorial）
word2vec 及其它相关方法是为了实现效率而牺牲表达性的浅层方法。使用词嵌入就像利用仅编码图像边缘信息的预训练表征初始化计算机视觉模型：它们在许多任务中都能发挥作用，但是却无法捕捉到可能发挥更大作用的高层次信息。利用词向量初始化的模型需要从头开始学习，不仅是学习消除歧义，还要学习从词组成的句子中提取意义。这是语言理解的核心，需要建模语义组合、一词多义、首语重复、长期依赖、一致性、否定等许多复杂的语言现象。因此，用这些浅层表示初始化的 NLP 模型仍然需要大量的示例才能获得良好的性能也就不足为奇了。

ULMFiT、ELMo 和 OpenAI transformer 最新进展的核心是一个关键的范式转变：从仅仅初始化模型的第一层到用分层表示对整个模型进行预处理。如果学习词向量就像仅学习图像的边，那么这些方法就像学习特征的完整层次，从边到形状，再到高级语义概念。

有趣的是，预训练整个模型以获得初级和高级特征在计算机视觉社区中已经采用好几年了。大多数情况下，预训练模型都是在 ImageNet 大型数据集上学习分类图像而训练出来的。ULMFiT、ELMo 和 OpenAI transformer 已经为 NLP 社区带来了自然语言中的「ImageNet」，这种任务能允许模型学习语言的高级细微差别。这就类似于 ImageNet 允许预训练 CV 模型以学习普遍意义的图像特征。在本文后面的部分中，我们将语言建模与 ImageNet 计算机视觉建模做类比，并展示为什么这种方法看起来会如此有前景。




**ImageNet**
![](https://pic2.zhimg.com/v2-0b729b75de065af68762da6b1f62ef85_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='622'></svg>)ImageNet 大规模视觉识别挑战赛。（来源：Xavier Giro-o-Nieto）
ImageNet 对机器学习研究具有重要影响。该数据集最初发布于 2009 年，并迅速演变为 ImageNet 大规模视觉识别挑战赛（ILSVRC）。2012 年，Alex Krizhevsky、Ilya Sutskever 和 Geoffrey Hinton 提交的深度神经网络超过第二名 41 %，表明深度学习是一种可行的机器学习策略，该深度神经网络可以说是引发了机器学习研究中深度学习的爆发。

ImageNet 的成功凸显了在深度学习时代，数据至少和算法一样重要。ImageNet 数据集不仅使 2012 年非常重要的深度学习能力展示成为可能，而且在迁移学习中也取得了同样重要的突破：研究人员很快意识到，在 ImageNet 现有模型中学习的权重可用于完全初始化其它数据集的模型，并显著提高性能。这种「微调」方法允许在每个类别只有一个正面示例的情况下实现良好性能（Donahue et al., 2014）。
![](https://pic3.zhimg.com/v2-ce9ca10ba401715867783a99e5d7201a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='649'></svg>)在 ILSVRC-2012 上训练的特征泛化到 SUN-397 数据集上。（来源：Donahue et al., 2014）
在目标识别、语义分割、人体姿态估计和视频识别等任务上，预处理的 ImageNet 模型已用来实现最优水平。与此同时，它们还使得 CV 得以应用于训练示例数量少、注释成本高的领域。在 CV 中，在 ImageNet 上通过预处理进行迁移学习实际上非常有效，以至于现在不使用它会被认为是蛮干（Mahajan et al., 2018）。




**ImageNet 中有什么？**

为了确定用于语言处理的 ImageNet 可能是什么样子，我们首先要确定什么使 ImageNet 有利于迁移学习。以往的研究只揭示了这个问题的一部分：减少每个类的示例数或类别数只会导致性能下降，而细粒度类和更多数据并非总意味着更好的结果。

与其直接查看数据，更谨慎的做法是探究在数据上训练的模型学到了什么。众所周知，在 ImageNet 上训练的深层神经网络的特征迁移顺序为从第一层到最后一层、从一般任务到特定任务：较低层学习建模低级特征，如边缘，而较高层学习建模高级概念，如图案和整个部分或对象，如下图所示。重要的是，关于物体边缘、结构和视觉组成的知识与许多 CV 任务相关，这就揭示了为什么这些层会被迁移。因此，类似 ImageNet 的数据集的一个关键属性是鼓励模型学习可以泛化到问题域中新任务的特征。
![](https://pic3.zhimg.com/v2-f8713d55c00a1995bc48c32dd8c43c76_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='329'></svg>)在 ImageNet 上训练的 GoogLeNet 中不同层特征捕获的信息可视化。（来源：Distill）
除此之外，很难对 ImageNet 上迁移做得如此出色的原因作出进一步的概括。例如，ImageNet 数据集的另一个优点可能是数据的质量。ImageNet 的创建者尽力确保注释的可靠性和一致性。然而，远程监管的工作是一个对比，这表明大量弱标记数据通常是足够的。事实上，Facebook 的研究人员最近显示，他们可以通过预测数十亿个社交媒体图像上的 hashtags 到 ImageNet 上最新的准确性来预先训练模型。

没有任何更具体的见解，但我们有两个关键的需求：
- 像 ImageNet 一样的数据集要足够大，即大约有数百万个训练示例。
- 它应该代表学科的问题空间。




**用于语言任务的 ImageNet **

相比于 CV，NLP 的模型通常浅得多。因此对特征的分析大部分聚焦于第一个嵌入层，很少有人研究迁移学习的高层性质。我们考虑规模足够大的数据集。在当前的 NLP 形势下，有以下几个常见任务，它们都有可能用于 NLP 的预训练模型。

阅读理解是回答关于一个段落自然语言问题的任务。该任务最流行的数据集是 Stanford Question Answering Dataset (SQuAD)，其中包含了超过 10 万个问答对，并通过突出显示段落中的几个单词来让模型回答一个问题，如下图所示：
![](https://pic1.zhimg.com/v2-8db8ff416f151328af18a331d4fd3ea4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1110'></svg>)在 ImageNet 上训练的 GoogLeNet 的不同层特征捕捉到的信息的可视化（Rajpurkar et al., 2016，《SQuAD: 100,000+ Questions for Machine Comprehension of Text》）。
自然语言推理是识别一段文本和一个假设之间关系（蕴涵、矛盾和中性等）的任务。该任务最流行的数据集是 Stanford Natural Language Inference (SNLI) Corpus，包含 57 万个人类写的英语句子对。该数据集的示例如下图所示。

SNLI：[https://nlp.stanford.edu/projects/snli/](https://link.zhihu.com/?target=https%3A//nlp.stanford.edu/projects/snli/)
![](https://pic2.zhimg.com/v2-87c2852128b281350131be968a519061_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='170'></svg>)SNLI 数据集的示例。（Bowman et al., 2015，A large annotated corpus for learning natural language inference）
机器翻译，即将文本从一种语言转换到另一种语言，是 NLP 中研究最充分的任务之一。并且多年来，人们为常用的语言对累积了大量的训练数据，例如 WMT2014 的 4 千万个英语法语句子对。下图是两个示例翻译对。
![](https://pic4.zhimg.com/v2-b9adfc6ee2ad5bd4cc95e982a4abfc4f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='471'></svg>)来自 newstest2014 的法语到英语翻译（Artetxe et al., 2018，Unsupervised Neural Machine Translation）
选区解析（Constituency parsing）以（线性化）解析树的形式提取句子的句法结构，如下图所示。在过去，人们在该任务中使用数百万个弱标记解析来训练序列到序列的模型（参见《Grammar as a Foreign Language》）。
![](https://pic4.zhimg.com/v2-d3a8b201f35cb666c3ffe298c554e2fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='306'></svg>)解析树和其线性化处理（Vinyals et al., 2015，Grammar as a Foreign Language）
语言建模（LM）在给定前一个单词的情况下尝试预测下一个单词。已有的基准数据集由大约 10 亿个单词构成，但由于该任务是无监督的，因此可以使用任意数量的单词来训练。下图是由维基百科文章构成的常用 WikiText-2 数据集的示例。
![](https://pic3.zhimg.com/v2-d347e6a610fbdb7cfc07ae4d490d20de_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='548'></svg>)WikiText-2 语言建模数据集的示例。（来源： Salesforce）
WikiText-2：[https://einstein.ai/research/the-wikitext-long-term-dependency-language-modeling-dataset](https://link.zhihu.com/?target=https%3A//einstein.ai/research/the-wikitext-long-term-dependency-language-modeling-dataset)

所有这些任务提供或允许收集足够数量的示例来训练。实际上，以上任务（以及很多其它任务例如情感分析、skip-thoughts 和自编码等）都曾在近几个月被用于预训练表征。

虽然任何的数据都包含某些偏差，人类标注可能无意间引入额外信息，而模型也将会利用这些信息。近期研究表明在诸如阅读理解和自然语言推理这样的任务中的当前最优模型实际上并没有形成深度的自然语言理解，而是注意某些线索以执行粗浅的模式匹配。例如，Gururangan 等人 (2018) 在《Annotation Artifacts in Natural Language Inference Data》中表明，标注者倾向于通过移除性别或数量信息生成蕴涵示例，以及通过引入否定词生成矛盾。只需使用这些线索，模型就可以在未查看前提的情况下在 SNLI 数据集上以 67% 的准确率分类假设。

因此，更困难的问题应该是：哪个任务在 NLP 中最具代表性？换种说法，哪个任务使我们能学到最多关于自然语言理解的知识或关系？




**语言建模**

为了预测句子中最可能出现的下一个词，模型不仅需要能表达语法，即模型预测下一个词的语法形式必须与其修饰语或动词匹配。同时模型还需要理解语义，此外那些最准确的模型必须包含世界知识或常识等内容。若思考一条不完整的语句「The service was poor, but the food was」，为了预测「yummy」或「delicious」等后续单词，模型不仅需要记住用于描述食物的属性，还需要识别连词「but」以引入相反的语义，因此新的属性应该是与情感词「poor」相对的。

语言建模是最后提到的一种方法，它已经被证明能为下游任务捕获很多与语言相关的属性，例如长期依赖性关系、层级关系和情感语义等。相比于自编码器等无监督学习任务，语言建模即使只有少量训练数据也能在句法任务上有非常好的表现。

语言建模最大的优势在于，训练数据可以免费从任何文本语料库获取，因此几乎能获得无限的训练数据。这非常重要，因为 NLP 并不只限于英语，有超过 1000 人使用的语言有 4500 种。作为预训练任务的语言建模为以前语言服务支持不好的语种打开了大门，我们可以直接使用文本数据无监督地训练语言模型，并应用到翻译、信息抽取等任务中。而对于那些无标注数据都不足的稀有语言，多语语言建模能先在多个相关语种上训练，例如跨语言词嵌入等。
![](https://pic2.zhimg.com/v2-ca960b7ac7cdb9cba5610346065310c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='413'></svg>)ULMFiT 不同的阶段（Howard and Ruder, 2018）
到目前为止，我们将语言建模作为预训练任务的论点纯粹是概念性的。但是在最近几个月，我们也获得了一些实验性证明：语言模型的词嵌入（ELMo）、普遍语言模型精调（ULMiT）和 OpenAI Transformer 已经实验性地证明了语言模型能用于预训练任务，正如上图所示的 ULMFiT。这三种方法都使用预训练语言模型来实现当前最优的自然语言处理任务，例如文本分类、问答系统、自然语言推断、指代消歧和序列标注等问题。

在如下所示的 ELMo 等很多情况中，使用预训练语言模型作为核心的算法在广泛研究的基准上，要比当前最优的结果高 10% 到 20%。ELMo 同时也获得了 NLP 顶会 NAACL-HLT 2018 的最佳论文。最后，这些模型表现出非常高的样本效率，达到最优性能只需要数百样本，甚至可以实现 zero-shot 学习。
![](https://pic2.zhimg.com/v2-8bad72a2f34a2a6265cb682c8684ca5d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='925' height='735'></svg>)ELMo 在一系列 NLP 任务中取得的进步。（来源：Matthew Peters）
鉴于这一步取得的变化，NLP 实践者很可能在一年后下载预处理的语言模型，而不是预处理的词嵌入，来用于他们自己的模型中，就像现在大多数 CV 项目的起点是如何预处理 ImageNet 模型一样。

然而，和 word2vec 相似，语言建模的任务有其天然的局限性：它只是作为真正的语言理解的一个代理，并且单体模型并不足以为特定的下游任务捕捉需要的信息。例如，为了回答关于或跟随故事中人物轨迹的问题，模型需要学习执行指代或消解。此外，语言模型仅能捕捉它们所见过的东西。特定类型的信息，例如大部分常识，很难仅从文本中学习到，并需要整合外部信息。

一个突出的问题是如何从一个预训练语言模型将信息迁移到下游任务中。有两个主要的范式，一是是否将预训练语言模型作为固定的特征提取器，并将其表征作为特征整合到随机初始化的模型（正如 ELMo 所做的）中；二是是否微调完整的语言模型（如 ULMFiT 所做的）。后者在计算机视觉中很常用，其中训练时会调整模型的最高层或最高的几层。虽然 NLP 模型通常更浅，因此相比对应的视觉模型需要不同的微调技术，但近期的的预训练模型变得更深了。我在下一月将展示 NLP 迁移学习的每个核心组件的作用：包括表达性很强的语言模型编码器（如深度 BiLSTM 或 Transformer），用于预训练的数据的量和本质，以及微调预训练模型使用的方法。




**但理论依据何在？**

到目前为止，我们的分析主要是概念和经验上的，我们仍然难以理解为什么模型先在 ImageNet 上进行训练就能在语言建模上迁移得如此之好。一种更为正式的、考虑预训练模型泛化能力的方式是基于「偏置学习」（bias learning）模型（Baxter, 2000）。假设我们的问题域覆盖特定学科中任务的所有排列，例如计算机视觉——它构成了环境。我们对此提供了许多数据集，允许我们诱导一系列假设空间 H=H'。我们在偏置学习中的目标是找到偏置，即假设空间 H'∈H，它可以在整个（可能是无限的）环境中最大化性能。

多任务学习中的经验和理论结果（Caruana，1997; Baxter，2000）表明，在足够多的任务中学习到的偏置或许可以推广到在同样环境中未见过的任务上。通过多任务学习，在 ImageNet 上训练的模型可以学习大量的二进制分类任务（每个类一个）。这些任务都来自自然、真实世界的图像空间，可能对许多其他 CV 任务也有代表性。同样，语言模型通过学习大量分类任务（每个词一个）可能诱导出有助于自然语言领域许多其他任务的表征。然而，要想从理论上更好地理解为什么语言建模似乎在迁移学习中如此有效，还需要进行更多的研究。




**NLP 的 ImageNet 时代**

NLP 真正转向迁移学习的时机已经成熟。鉴于 ELMo、ULMFiT 和 OpenAI 令人印象深刻的实验结果，这种发展似乎只是一个时间问题，预训练的词嵌入将逐渐落伍，取而代之的是每个 NLP 从业者工具箱里的预训练语言模型。这可能会在标注数据数量不足时为 NLP 创造更多可能性。苍天已死，黄天当立！
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://thegradient.pub/nlp-imagenet/](https://link.zhihu.com/?target=https%3A//thegradient.pub/nlp-imagenet/)



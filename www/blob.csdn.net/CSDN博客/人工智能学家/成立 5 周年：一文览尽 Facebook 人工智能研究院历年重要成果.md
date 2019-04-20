# 成立 5 周年：一文览尽 Facebook 人工智能研究院历年重要成果 - 人工智能学家 - CSDN博客
2018年12月08日 20:07:36[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：157
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXp1CAeichF7o8eRibZ2qaI4jsE7IfGZfZzkZFBfuANic09OB99bXsPFGBQQZHpO7ZbcJYK7pGhnibbGA/640?wx_fmt=jpeg)
来源：AI科技评论
五年前，Yann LeCun 等人创建了 Facebook 人工智能研究院 (Facebook AI Research，FAIR)，试图通过开放的研究环境促进人工智能的发展，进而造福所有人——他们努力的目的是为了理解智慧的本质，以便创造真正意义上的智能机器。从那时起，FAIR 便不断发展壮大、做出成果，逐渐长成一个在多个国家均设有实验室的国际性研究组织，当中包括硅谷门罗公园、纽约、巴黎、蒙特利尔、特拉维夫、西雅图、匹兹堡和伦敦。在人工智能渐渐成为 Facebook 业务核心的当下，FAIR 也随之成为 Facebook 庞大人工智能架构的一部分，从基础研究到应用研究与技术开发，致力于人工智能研发的方方面面。
FAIR 在对外合作上秉持着开放的态度，尽可能与社会保持紧密的联动关系。他们的团队经常对外发布前沿的研究成果，并且尽可能对外开源研究代码、数据集和工具（如 PyTorch 、 fastText 、 FAISS 和 Detectron）。他们的这些努力成功推进了人工智能研究的发展。以今年为例，来自 FAIR 的研究人员成功获得了多个奖项的认同，其中就包括 ACL、EMNLP、CVPR 和 ECCV 的最佳论文奖，以及 ECCV 、 ICML 和 NeurIPS 的时间检验奖。他们深信，开放的工作环境可以让 AI 领域获得快速的发展。
要想制造出真正意义上的智能机器，无论从技术还是产品工程的角度来说，都是一项挑战。因此，FAIR 有相当一部分的研究精力都集中于攻克一些基础问题，比如推理、预测、计划和无监督学习等。与此同时，要想在这些研究领域有所建树，需要对生成模型、因果关系、高维随机优化和博弈论等理论有更深入的理解。如果想让人工智能的潜力在未来最大化为我们所用，这些长期的研究探索是需要一直坚持下去的。在成立五周年之际，他们将从过去已完成的项目中挑选出了一些具有代表性的项目，通过它们向大家展示 FAIR 是如何成功达成使命，为人工智能领域做出贡献，进而对世界产生影响的。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmjuvAvtMogSso4r77KDEDQicIM31BjwK63ibSll6cwniaiaZwP0qVfbyNgd2oGXYfHrcYGUiagIsdVYAnQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmjuvAvtMogSso4r77KDEDQicXegfohfOjdq8K9ialS1dOzEnsicVHaACDj1UcgfD44cpS5lBk64Xkk7w/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmjuvAvtMogSso4r77KDEDQicCNfItKvB9Jok4UTZU8ovsF7ia86iaFmAmfBM7ddFn41ugeV3Wdma0HbQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmjuvAvtMogSso4r77KDEDQicPHYJq0RcXQqeeNTUrsWiaqhU49X1GGLB3PUAr20ib1alib5Nhc8ricNEPw/640?wx_fmt=jpeg)
图中的时间轴展示了这 5 年以来 FAIR 较有代表性的项目 
**记忆网络**
在 2014 年，FAIR 的研究人员发现了神经网络的一个本质缺陷——长期记忆。虽然大家都知道神经网络可以在数据集的训练过程中学习，可一旦系统开始运行了，它们通常再也无法存储新信息，用于解决日后的特定任务。因此，他们开发出了一种新的学习模型，以帮助机器能够记住足量的交互行为，并根据对话中先前的陈述来回答一般的知识问题。他们一篇发表于 2014 年阐述了该模型的论文（https://arxiv.org/abs/1410.3916）中，试图通过一个具备记忆能力的网络去回答和《指环王》系列故事情节相关的问题，而它的依据仅仅只有提供的一篇简短摘要。结果显示，该系统不但能够学习到一些简单的语言模式，甚至还能正确归纳出生词的意思，例如在故事结尾中出现的「佛罗多身在夏尔」以及「魔戒在末日山上」。
FAIR 在接下来的两年内继续对这一方法进行改进，无论在研究范围还是探索领域上都扩充了许多。他们开发出一款可以从序列中以无监督的方式进行训练的增强式 RNN，它带有一个支持 push、pop 的堆栈，也就是 StackRNN（https://research.fb.com/downloads/stack-rnn/）。在 bAbl 中（https://research.fb.com/downloads/babi/），团队构建了基于问答任务的数据集，以评测机器在文本理解方面的表现。值得一提的是，bAbI 如今已是开源项目 ParlAI 的一部分，该项目包含了上千条的对话示例，涵盖从餐馆预订到对电影阵容的回答，可以说是应有尽有。此外，他们还对内存网络架构进行了迭代，增强它们在实际应用程序中的实用性。迭代的内容包括了端到端内存网络（https://arxiv.org/abs/1503.08895）以及键值内存网络（https://arxiv.org/abs/1606.03126），前者可以让网络在更少的监督下工作，后者则允许网络通过完全没有监督的信息源 (如维基百科条目) 进行泛化后进行训练。
**自我监督学习与生成模型**
如何通过自监督学习 (SSL) 对大量未标记的数据进行利用，进而扩大人工智能的运用规模，向来是 FAIR 优先级最高的工作之一。有了 SSL，机器可以通过输入未标记的图像、视频或音频来习得世界的抽象表征。SSL 最典型的一个应用例子是，通过向一台机器展示视频内容并进行训练，使其可以预测接下来出现的画面。一旦掌握了预测的能力，机器可以获取关于世界如何运转的知识，进而更好习得对世界的抽象表征。SSL 让机器可以像人类与动物的婴儿一样通过观察来进行学习，由此积累大量关于真实世界的背景知识。人们都希望机器可以和这个世界达成一种「共识」。一个具备对世界万物进行预测能力的模型，是构建能够进行推理、预测其行为后果并在现实世界中付诸行动的人工智能系统的关键。
2014 年，来自蒙特利尔大学 MILA 的一名朋友（Ian Goodfellow）提出了名为生成对抗网络 (GANs) 的新型无监督学习方法。当时 FAIR 的研究人员们立刻被这种自我监督学习方法的潜在应用价值所吸引。不过，尽管 GANs 让人感觉这种方法很有前途，该方法却只在一些非常简单的问题上得到了证明。为此，FAIR 从 2015 年开始发表了一系列论文，试图说服学界人士关于 GANs 确实发挥了作用。简单来说，GANs 可以用来训练机器在不确定的情况下进行预测，其背后依赖的是两个相互对抗的神经网络。在典型的 GAN 结构中，生成器网络会从一堆随机数 (可能还包含过去的视频帧) 生成数据（如图像或视频帧），与此同时，鉴别器网络也会同步区分真实数据（真实的图像与视频帧）与生成器的「失误」输出。正是这种对抗性的网络运行逻辑有效优化了两个网络，从而带来更好的预测效果。
这些年，他们的论文基本覆盖了 GANs 的几种关键变体，其中包括专注于图像生成的深度卷积生成对抗网络 (DCGANs，http://arxiv.org/abs/1511.06434) 和拉普拉斯对抗网络 (LAPGANs，http://papers.nips.cc/paper/5773-deep-generative-image-models-using-a-laplacian-pyramid-of-adversarial-networks)，以及专注于视频预测行为的对抗梯度差损失预测器 (AGDLs，https://arxiv.org/abs/1511.05440)。这些论文的最重要贡献是，向大家证明了 GANs 实际上可以「创造」出一些看起来很逼真的图像，例如压根就不存在的卧室、面孔或者狗。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmjuvAvtMogSso4r77KDEDQicXAMH6bbia27DD7K6YwfE1HTgFpO2KJBFgFJYjYTckPjzPL2XFDiagHbg/640?wx_fmt=png)
图中展示了一系列由生成网络创建的时装设计作品
从此以后，有越来越多的研究人员开始关注 FAIR 在 GANs 方面所做的工作，利用它们创建令人赞叹的高分辨率图像。然而 GANs 是出了名的难以调优以及难以收敛，为此 FAIR 努力探索更多可以让 GANs 变得可靠的方法，其中一种方法是在理论上着重于理解对抗性的训练。2017 年，他们提出了 Wasserstein GAN (WGAN，https://arxiv.org/abs/1701.07875) 方法，该方法通过改进鉴别器使其变得更加「平滑」和高效，由此增强生成器的预测效果。总的来说，WGAN 是第一个能够保证收敛鲁棒性，且被广泛应用的 GAN。这也意味着有了 WGAN 后，可以在优化系统时免去平衡鉴别器和生成器输出的工作，从而显著提高网络在学习上的稳定性，尤其是在高分辨率图像的生成任务中。
自此，FAIR 的研究员们和 Facebook 工程师们将对抗训练这一方法进行了大范围应用，包括长时间视频预测（http://openaccess.thecvf.com/content_ICCV_2017/papers/Luc_Predicting_Deeper_Into_ICCV_2017_paper.pdf）和时尚服装创造（https://arxiv.org/abs/1804.00921）。但是生成式对抗网络（GAN）真正有趣的部分，是它们对于未来的意义。作为一项全新的技术（几年前甚至还不存在），它为我们带来了在数据稀缺的领域生成数据的新机会。未来的探索中，它可能成为构建能够自主学习的机器的关键工具。
**规模化的文本分类**
文本理解不是单一任务，而是一个扩展的子任务矩阵，是把词语、短语和整个语言数据集组织为机器能够处理的格式。但是在大量工作进行之前，文本本身需要被分类。几年前，词向量（word2vec）等自然语言处理模型通过大量的、基于词的训练来对文本进行分类，同时，模型为训练数据集中的每个单词分配不同向量。对于 Facebook 来说，这种文本分类方法太慢，并且太依赖于完全监督的数据。他们需要最终实现的是对成百上千种语言进行文本分类，其中许多语言并不具备大规模数据集。该系统需要扩展到 Facebook 所有基于文本的功能和服务中，还需要兼顾 FAIR 的自然语言处理研究。
因此在 2016 年，FAIR 创建了 fastText（https://code.fb.com/ml-applications/expanded-fasttext-library-now-fits-on-smaller-memory-devices/），这是一个用于快速分类文本和学习词表示的框架，它考虑了其所分类词语的更大形态。在 2017 年发表的一篇论文（https://arxiv.org/pdf/1607.04606.pdf）中，FAIR 提出了一个模型，该模型将向量分配给「子词单元」（例如，3 或 4 个字母组成的序列），而不是分配给整个单词，从而让系统为训练数据中没有出现过的单词创建表示，最终的结果，该模型的分类可以扩展到数十亿个词语，并从新的、未经训练的词语中学习，同时其训练速度也比典型的深层学习分类器快得多。在某些情况下，用 fastText 可以在几秒内训练完此前需要花费几天时间才能完成训练的模型。
后来的发展表明 FastText 对于基于人工智能的语言理解的研究和应用是一项重要贡献，并且它现在适用于 157 种语言。原论文在其他出版物上已被引用了上千次，而 fastText 仍然是词嵌入系统最常用的基线之一。在 Facebook 以外，fastText 也已在各种各样的应用程序中得到应用，从熟悉的应用场景，如消息回复推荐（suggesting message replies），到特别的应用场景，如一款叫做 The Great Outdoors 的「算法剧场」产品，它使用 fastText 来帮助筛选大众互联网评论并将筛选结果安排为表演的脚本。该框架也被部署在 Facebook 上，用于对 19 种语言的文本进行分类，并与 DeepText 结合用于翻译和自然语言理解。
**前沿的翻译研究**
快速、准确、灵活的翻译是帮助世界各地的人们进行交流的重要组成部分。因此，在 FAIR 成立的早期，他们就开始寻找一种比当时最先进的统计机器翻译表现更好的新方法。他们花了三年的时间构建了一个基于 CNN 的，集速度、准确性和学习能力为一体的神经机器翻译（NMT）架构。（Facebook 人工智能研究院在 2017 年发表了一篇详述其工作的论文 https://arxiv.org/abs/1705.03122。）在他们的实验中，这种方法相比 RNN 的速度提高了 9 倍，同时还保持了当时最高的准确率。
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/vJe7ErxcLmjuvAvtMogSso4r77KDEDQicIJjEATj7LeXcrbTRZVZDibGFASRZUBicDdhZhpiaZfY6icRktxCZibibWdaA/640?wx_fmt=gif)
他们的多跳 CNN 不仅更易于在更有限的数据集上训练，也能更好地理解拼写错误或缩写词，比如正确地把「tmrw」作为「tomorrow」进行翻译。总体而言，神经机器翻译提高了平均 11 % 的准确率以及 2.5 倍的翻译速度。除了改进自己的系统，他们还对 fairseq 的代码和模型（https://github.com/facebookresearch/fairseq）进行了开源，它是针对基于 CNN 系统使用的序列到序列的建模工具包。
为了避免对大规模翻译训练数据集的需求（通常称为语料库），他们还在寻求其他新的机器翻译方法，例如多语言嵌入，其可以实现跨多种语言的训练。去年，他们发布了一个开源 Python 库——MUSE，它为学习多语言嵌入提供了两种不同方法：一种是使用发布版本中包含的 110 种双语词典的有监督的方法；另一种是允许在没有平行语料两种语言间构建新双语词典的更新的、无监督的方法。随后，他们发表了一篇 EMNLP 获奖论文（https://arxiv.org/abs/1804.07755），展示了针对完整句子翻译的无监督训练所取得的显著进步。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmjuvAvtMogSso4r77KDEDQic7NLaktufCg3E7q31eDP6b7sUUViakSiaZkUZ6AGobjN45dVwegb0Zf6Q/640?wx_fmt=png)
通过共享 fairseq 和 MUSE 这样的研究和资源，FAIR 鼓励其他人无论是用于研究目的还是生产应用，都能享受更快、更准确和更多功能的翻译技术所带来的好处。
**让每个人都进步的人工智能工具**
人工智能的进步不仅取决于突破性的想法，还取决于拥有强大的平台和工具来验证和实现它们。FAIR 率先建立起了这些系统，并与世界分享。在 2015 年，他们开源由 FAIR 开发的 Torch 深度学习模块（https://code.fb.com/developer-tools/fair-open-sources-deep-learning-modules-for-torch/），以加速大型神经网络的训练。在 2016 年，他们发布了 Torchnet（https://code.fb.com/core-data/lighting-the-way-to-deep-machine-learning/），使得深度学习社区能够更容易地快速构建有效和可重复使用的学习系统。不久之后，他们发布了 Caffe2（https://caffe2.ai），它是用于移动计算的模块化深层学习框架，如今正在全球 10 亿多部手机上运行神经网络。接着，他们又联合微软和亚马逊发布了 ONNX（https://onnx.ai），这是针对神经网络的一种通用表示，可以使其更易于根据需要在框架之间移动。
特别是，他们对 PyTorch（https://pytorch.org）的研究工作表明了 FAIR 致力于开发快速迭代、具有巨大影响力、开放的系统以及与 AI 社区建立合作。PyTorch 最初只是 Facebook 人工智能研究院一小部分研究人员的小成果。他们选择在 Torch 开放源代码库上构建一个深度学习框架，而不是全部重新构建，同时他们还集成了英特尔和英伟达的加速库，来实现速度最大化。他们在 2017 年初发布了 PyTorch，在不到两年时间的今天，它成为了 GitHub 上发展速度排第二的开源项目（https://octoverse.github.com/projects），以及全球 AI 开发人员所选用的框架。在 10 月份，AI 社区的数百名成员参加了第一届 PyTorch 开发者大会，聆听了来自加州理工大学、FAIR 、fast.ai、谷歌、微软、英伟达、特斯拉以及其他许多企业和机构的发言。现在，PyTorch 1.0 的发布集成了 Caffe2 和 ONNX 的模块化、面向生产的能力，并通过深度集成云服务和技术供应商，提供了从原型研究到产品部署的无缝对接路径。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmjuvAvtMogSso4r77KDEDQickwicP0fwSW3L5D1hYVqfdpbbCkm2Jgpicz41wAHD56l0Uz8jDBD9Ybfg/640?wx_fmt=png)
PyTorch 被集成到 Facebook 拥有亿级用户的产品中，它也被集成到了 FAIR 自己的研究项目上，如 fairseq(-py)（https://github.com/pytorch/fairseq），这个版本比之前版本的翻译速度提高了 80%。强化学习围棋机器人 ELF OpenGo、嵌入问答 EmbodiedQA 的研究（https://embodiedqa.org/）以及在数十亿带有标签的公共图像上训练图像识别网络的时候，都使用了 PyTorch。除了 Facebook，纽约大学教授 Narges Razavian 博士在 AllenNLP 项目和利用人工智能改善早期疾病检测的项目中也使用了 PyTorch。现在 Udacity 和 FAST.AI 正在帮助更多的人使用 PyTorch。 
由于 PyTorch 使得模型从研究到生产更快、更容易，FAIR 在 Facebook AI 相似性搜索（FAISS）方面的工作也加速了大规模搜索研究方面的进展。FAISS 最初只是一个利用 GPU 来识别与用户偏好相关的相似性的内部研究项目，但如今它能够利用数十亿规模的数据集，是现有的同类查询库中最快最大的一个。FAISS 已经为推荐引擎和人工智能辅助系统开辟了可能性。FAIR 去年把它作为开源库发布，现在 FAISS 已经被开发者社区广泛采用，获得了 5000 多个 GitHub star 并被集成到了 NVIDIA 的 GPU 加速的 scikit-learning 库 cuML 中。
**一种新的计算机视觉测试基准**
试图理解智力的本质是一项多感官式的研究，但在过去五年，FAIR 实际上只在计算机视觉方面进展较多。在 FAIR 诞生之前，Facebook 就有一支由人工智能专家组成的小团队，他们试图更好地理解图像中的人是如何被像素表示的，以便在正确的时间为人们展示正确的照片。到 2017 年，FAIR 的研究人员将最优秀的目标检测和语义分割技术相结合，完成了 Mask R-CNN 论文，这篇论文也获得了 CVPR2017 的最佳论文奖。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmjuvAvtMogSso4r77KDEDQicwesIHv7XHEI9ajoRhIod8RXN2dibCrtZex3la4mia7SnefEt45beCdKg/640?wx_fmt=png)
正如这篇论文所说，「在不加修饰的情况下，Mask R-CNN 在每一项任务上的表现都胜过所有现有的单模项目，包括 COCO 2016 挑战赛冠军。」这项工作迅速在更多的人工智能社区成为了计算机视觉研究的基础。然后，该技术被集成到 FAIR 的开源 Detectron 系统中，为世界各地的研究人员提高了算法的直观易用性、速度和准确性。
这项基础工作支撑着 Facebook 的大量现有系统，例如替代自动文本可以帮助视力受损者检测文本和不良内容。这也是未来应用的基础：跨平台的 AR 功能和 Portal 中的 Smart Camera 都植根于此。随着研究的继续进行，FAIR 的研究重点转移到了视频，DensePose 项目就可以帮助系统理解视频内容以及照片。
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/vJe7ErxcLmjuvAvtMogSso4r77KDEDQic3Qk1lQEbYib86dKVgU19wX9Ajic70BqTkZUvQiclhWqPYnQBwO15NJuSw/640?wx_fmt=gif)
上面的视频演示了当人移动时，在人的头顶上创建 3D 表面
**图像理解：更快的训练和更大的数据集**
计算机视觉并不是 FAIR 寻求解决大规模挑战的唯一领域。FAIR 与 Facebook 的应用机器学习（AML）团队合作，解决训练速度和训练集大小的限制，以及缺乏监督数据集的问题。在今年早些时候发表的一篇论文中，AML 的团队讨论了他们如何在带有哈希表的大型公共图像集上训练图像识别网络，其中最大的包括 35 亿张图像和 17000 个哈希表。它的数量级比以往任何发表的作品都要多，并且结果是迄今为止业界发布的最好的结果：准确率为 85.4%。 
通过 FAIR 对训练速度的研究，这一突破成为可能——FAIR 能够将 ImageNet 的训练速度提高一个数量级。他们把训练时间缩短到一个小时以下，演示了如何进行小规模的 SGD 训练，这些训练的数量级比之前认为的实际数量级大。用他们的话说：「为了得到这个结果，我们采用线性缩放规则将学习率调整为一个小批量的函数，并且开发新的预处理方案，这个方案解决了早期训练中的最优化问题。」
随着训练速度的提高，我们能够对较大数据集上的弱监督学习进行有针对性的研究。这两个结果表明，FAIR 和 AML 之间是互相促进的。当解决人工智能的科学得到实际研究和生产中应用的支持时，我们会看到最快速、最先进的结果。
**FAIR 的未来**
当 LeCun 等人创建 FAIR 时，他们的最终目标是理解智能并发现其基本原理，从而提高机器的智能性。到现在这一目标也没有改变。现在 FAIR 的研究人员们正在继续扩展他们的研究领域，诸如开发能够通过自我监督学习获得真实世界模型的机器，训练机器进行推理，以及训练它们进行规划和构思复杂的行动等。这也是 FAIR 目前研究机器人、视觉推理和对话系统的原因之一。在这篇文章中他们已经描述了一些已有的具体成果，这些项目证明了他们取得了巨大的进步，但在让机器更加智能从而帮助人们的过上更好生活这个方面还有很长的路要走。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)

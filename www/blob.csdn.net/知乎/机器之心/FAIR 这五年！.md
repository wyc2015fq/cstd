# FAIR 这五年！ - 知乎
# 



选自 code.fb，机器之心编译，作者：Yann Lecun、Jerome Pesenti、Mike Schroepfer，参与：路、王淑婷。

> 2013 年，Facebook 在 NeurIPS 大会上宣布成立 FAIR。五年过去了，FAIR 经历了什么？做出了哪些成绩？给世界带来了什么影响？FAIR 创建者 Yan LeCun、FAIR 现任领导者 Jerome Pesenti、Facebook CTO Mike Schroepfer 带大家回忆 FAIR 这五年。

五年前，我们创立了 Facebook 人工智能研究院（FAIR），旨在通过开放研究推进人工智能的发展，并惠及所有人。FAIR 的目标是理解智能的本质，以创造真正的智能机器。自此以后，FAIR 不断发展，并成长为一个国际研究组织，在门洛帕克、纽约、巴黎、蒙特利尔、特拉维夫、西雅图、匹兹堡、伦敦都设有实验室。人工智能已经成为 Facebook 的核心，因此 FAIR 现在是更大的 Facebook AI 组织的组成部分，该组织致力于人工智能研发的各个方面，从基础研究到应用研究和技术开发。

FAIR 在我们工作的各个方面都应用了开放模式，与社区展开广泛合作。我们团队经常早早地发布前沿研究成果，并尽可能地开源研究代码、数据集和工具（如 PyTorch、fastText、FAISS、[Detectron](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650736547%26idx%3D3%26sn%3Da326f17f832cec4827596291930e76dd%26chksm%3D871ac3ddb06d4acb3ffc56ffd56b340d5f15b7180a8aff5a38ea41c3c4e41e8d96da4c5a0a08%26scene%3D21%23wechat_redirect)）。这种方法也成功地推动了人工智能的研究发展。今年，FAIR 的研究人员已经获得了广泛认可，在 ACL、EMNLP、CVPR、ECCV 等大会获得了最佳论文奖，在 ECCV、ICML 和 NeurIPS 会议上获得了时间检验奖（Test of Time award）。开放的工作可以让每个人在人工智能领域取得更快的进步。

赋予机器真正的智能既是一个科学挑战，也是一项技术和产品工程的难题。FAIR 研究的很大一部分集中在推理、预测、规划和无监督学习的关键这些基本问题上。反过来，探索这些领域需要对生成模型、因果关系、高维随机优化和博弈论等具备更好的理论理解。释放人工智能未来的全部潜力需要这些长期的研究探索。我们从过去五年解决的项目中挑选了一些，来展示 FAIR 是如何完成其使命、为这个领域做出贡献并对世界产生影响的。
![](https://pic3.zhimg.com/v2-fb0e4ebb327ee3ab3f665fb056dd9152_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1020' height='468'></svg>)这个时间表突出了过去五年中 FAIR 完成的许多项目
**记忆网络**

2014 年，FAIR 的研究人员发现了神经网络的一个内在局限——长期记忆。尽管神经网络可以在数据集训练过程中学习，但是一旦这些系统开始运行，它们通常就没办法存储新的信息来帮助解决后面的特定任务。所以我们开发了一种新的支持学习模型的机器，这些机器记忆了足够多的互动信息，可以回答一般的知识问题并在对话中参考之前的陈述。在 2014 年关于该方法的早期论文《Memory Networks》中，我们测试了这一点：让一个支持记忆的网络根据提供给它的《指环王》系列简短总结来回答关于《指环王》情节的问题。该网络能够学习简单的语言模式，泛化到未知单词的含义并正确回答问题。

在接下来的两年里，FAIR 继续发展这种方法，扩大了研究范围并探索相关领域。该团队用 push-pop 堆栈增强 RNN，即 StackRNN，它能够以无监督方式从序列中训练。该团队建立了 bAbl 问答任务数据集，以帮助测试文本理解任务的性能。bAbI 现在是开源项目 [ParlAI](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650726327%26idx%3D5%26sn%3D865cffa1f9b7bc414292588e33c76323%26chksm%3D871b1bc9b06c92df1a748d9de8814a18294b7c8f18b6f88ea09fe8c68d0ee79d65f27330c756%26scene%3D21%23wechat_redirect) 的一部分，该项目包含数千个对话样本，范围从餐馆预订请求的回应到关于电影演员的回答。我们还迭代了记忆网络的架构，使其对现实应用越来越有用。这些更新包括端到端记忆网络（允许网络以较少的监督工作）和 key-value 记忆网络（可以通过对完全无监督的来源（如维基百科条目）进行归纳进行训练）。




**自监督学习和生成模型**

长期以来，通过自监督学习（SSL）利用大量无标注数据来扩展人工智能一直是 FAIR 的优先事项。使用 SSL，机器可以通过输入的未标注图像、视频或音频来学习世界的抽象表征。SSL 的一个应用例子是向机器展示视频片段，然后训练它来预测后面的帧。通过学习预测，机器可以捕捉关于世界如何运作的知识，并学习对世界的抽象表征。使用 SSL，机器通过观察来学习，一点一点，就像婴儿和动物幼崽一样，慢慢积累大量关于世界的背景知识。我们希望这样能形成一种常识。获取预测世界模型也是构建人工智能系统的关键，该系统能够推理、预测其行为的后果并在现实世界中采取行动。

2014 年，我们来自蒙特利尔大学学习算法研究所（MILA）的朋友 Ian Goodfellow 等人提出了一种新的无监督学习方法——生成对抗网络（GAN）。我们一下被自监督学习方法的潜在应用迷住了。但是，尽管 GAN 看起来很有前景，当时却只在一些简单的问题上证明了效果。自 2015 年开始，我们陆续发表一系列论文来让研究社区相信 GAN 确实有效。GAN 被用来训练机器在不确定的情况下通过两个神经网络互相对抗来做出预测。在典型的 GAN 架构中，生成器网络从一堆随机数中生成数据，例如图像或视频帧（可能还包括过去的视频帧）。同时，判别器网络需要区分真实数据（真实图像和视频帧）和生成器生成的「伪」输出。这场持续的比赛既优化了两个网络，也带来了越来越好的预测结果。

我们的每篇论文都关注 GAN 的不同变体，包括深度卷积生成对抗网络（DCGAN）和拉普拉斯对抗网络（LAPGAN）中的图像生成，以及对抗梯度差损失预测器（AGDL）中的视频预测。但是我们的共同贡献是展示：GAN 可以「创造」逼真的图像，如不存在的卧室、人脸或狗。
![](https://pic3.zhimg.com/v2-c40bbf31a060d3fc6118bc9cd903b05e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='312'></svg>)上述例子展示了由生成网络创造的一系列时装设计
其他研究人员自那时起开始研究我们在 GAN 方面的工作，用它们来生成惊人的高分辨率图像。但 GAN 是出了名的难调，并且经常收敛失败。所以 FAIR 通过在理论层面理解对抗训练，探索了一些增加 GAN 可靠度的方法。2017 年，我们提出了 [Wasserstein GAN](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650722818%26idx%3D3%26sn%3D03cb67c8a8ee7f83a7448b518f4336ab%26chksm%3D871b167cb06c9f6a018a99b79d8b2764b207be2b4d03f132151d99124edf2aff4c116a9dc98d%26scene%3D21%23wechat_redirect)（WGAN）方法，该方法使得判别器更加「平滑」、高效，以便告诉生成器如何改进其预测。WGAN 基本上是第一个在大量应用中收敛鲁棒的 GAN。这避免了在系统优化时需要平衡判别器和生成器的输出，进而导致学习稳定性显著提高，特别是对于高分辨率图像生成任务而言。

自此，FAIR 研究人员和 Facebook 工程师开始在一系列应用中使用对抗训练方法，包括长期视频预测和时装作品的创作。但 GAN 真正有趣的部分是其对于未来的意义。作为一种在几年前我们还无法使用的全新技术，它为我们在缺乏数据的领域生成数据创造了新的机会。它可能是我们构建自主学习机器的关键工具。




**大规模文本分类**

文本理解不是单一任务，而是多个子任务的复杂矩阵，如将语言的单词、短语和整个数据集转换成机器可以处理的格式。但是在做这些工作之前，文本本身也需要分类。很多年前，word2vec 等 NLP 模型通过大量基于词的训练对文本进行分类，模型为训练数据集中的每个词分配不同的向量。对于 Facebook 来说，那些方法太慢了，而且太依赖全监督数据。我们需要对数百甚至数千种语言执行文本分类，其中很多语言并不具备大量数据集。文本分类系统需要扩展到所有基于文本的功能和服务，以及我们的 NLP 研究。

因此，2016 年 FAIR 构建了 [fastText](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650716942%26idx%3D3%26sn%3D0d48c0218131de502ac5e2ef9b700967%26scene%3D21%23wechat_redirect)，这是一个快速文本分类框架，学习词表征的同时还可以考虑单词的形态。在 2017 年发表的论文《Enriching Word Vectors with Subword Information》中，FAIR 提出了一个将向量分配给「子词单元」（subword unit）而不是整个单词的模型，使系统为未出现在训练数据中的单词创建表征。最终该模型可以扩展到数十亿单词，能够从新的、未训练单词中学习，且训练速度显著快于典型的深度学习分类器。在一些情况下，之前的模型需要数天的训练时间，而 fastText 只需要几秒。

fastText 被证明是对基于 AI 的语言理解的重大贡献，现在可用于 157 种语言。原始论文的被引用次数已超一千，fastText 仍是词嵌入系统最常用的基线。在 Facebook 以外，fastText 也被广泛用于大量应用，从我们熟悉的信息回复建议到陌生的「算法剧院」产品 THE GREAT OUTDOORS，它使用 fastText 帮助选择和排序公开网络评论，然后将它们作为每次表演的台词。fastText 框架已经在 Facebook 中部署，对 19 种语言进行文本分类，它还被用于 [DeepText](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650715855%26idx%3D2%26sn%3Dfbe7c5ecea611646d006d3c347d3fc34%26scene%3D21%23wechat_redirect)中，执行翻译和自然语言理解。




**前沿翻译研究**

快速、准确、灵活的翻译是帮助世界各地的人们实现良好沟通的重要因素。因此，FAIR 在早期就开始寻找优于统计机器翻译的新方法，统计机器翻译在当时是最优秀的方法。我们用了三年时间构建出[基于 CNN 的神经机器翻译架构](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650726462%26idx%3D1%26sn%3D144e68df0057ac69002e3927073fc540%26chksm%3D871b2440b06cad565eb98857d057cf8dbb6cbde7786da2ffb676ae76c6246090640591e76572%26scene%3D21%23wechat_redirect)，该架构完美结合了速度、准确率和学习。实验表明该方法比当时最优的 RNN 速度快 9 倍。
![](https://pic1.zhimg.com/v2-e1a6c29acae14931f65a288574e5a5bc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1920' height='1080'></svg>)
我们的多跳 CNN（multi-hop CNN）不仅更容易在较小的数据集上训练，还能更好地理解拼写错误的单词或缩写词，如将「tmrw」（tomorrow 的缩写）翻译成「mañana」（西班牙语，表示将来某时）。整体上，这种 NMT 方法将翻译准确率提升了 11%，翻译交付的速度提升了 2.5 倍。除了改进我们自己的系统以外，我们还开源了 fairseq 的代码和模型。

为了避免机器翻译对大量训练数据集（通常叫作语料库）的需求，我们还探索了其它方法，如多语言嵌入，它可以在多语言之间进行训练。去年，我们开源了 MUSE，这是一个学习多语言词嵌入的 Python 库，提供两种学习方法：监督学习，使用发布版本中包含的 110 个双语词典；无监督学习，在没有平行语料库的情况下在两种语言之间构建新的双语词典。我们紧接着进行了无监督机器翻译的研究，论文[《Phrase-Based & Neural Unsupervised Machine Translation》](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650741621%26idx%3D4%26sn%3D210c9b3694f56508267e95235fb99f3c%26chksm%3D871adf0bb06d561dc99a89a5f56cf835bc92aa25401294216262128402490918d2787fe7fd09%26scene%3D21%23wechat_redirect)获得了 EMNLP 最佳长论文奖，该研究展示了无监督训练对完整句子翻译的显著提升。
![](https://pic3.zhimg.com/v2-0d19509dbfccaaee9fb8dde0996124b2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='298'></svg>)两种语言中的二维词嵌入（左、中）可以通过简单旋转完成对齐（右）。旋转后，词翻译可以通过最近邻搜索来执行
通过分享研究和资源（如 fairseq 和 MUSE），我们鼓励大家利用更快、更准确、更通用的翻译技术，不管是出于研究目的还是用于生产性应用。




**惠及每个人的 AI 工具**

AI 进展不仅依赖于突破性的想法，还依赖于强大的平台和测试实现工具。FAIR 优先构建这些系统，并与世界共享成果。2015 年，我们开源了大量 Torch 深度学习模块，它们由 FAIR 创建，旨在加速大型神经网络的训练速度。2016 年，我们发布了 [Torchnet](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650716375%26idx%3D2%26sn%3D54d36131a5b99a3d3dbdc427724133a3%26scene%3D21%23wechat_redirect)，以使社区更简单快速地构建高效、可重用的学习系统。之后不久，我们开源了 [Caffe2](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650725673%26idx%3D1%26sn%3D2d7dc963bba30f598b5b9392ddf2e5c6%26chksm%3D871b1957b06c904188b605336fa0615fdf04402628439b6e8013d35808918eb1396b21f2f9c3%26scene%3D21%23wechat_redirect)，目前这一适用于移动计算的模块化深度学习框架在全世界范围内超过 10 亿台手机上运行神经网络。然后我们与微软和亚马逊合作发布了神经网络[交换格式 ONNX](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650730662%26idx%3D3%26sn%3D361e9d109d2ae1da77caada746502c5a%26chksm%3D871b34d8b06cbdce9c1d0aaf28dcb3d7244ad08ef59866a0a3ca757fe753301dcc307a2d1c18%26scene%3D21%23wechat_redirect)，它是一个表征深度学习模型的标准，可实现模型在不同框架之间的迁移。

我们在 [PyTorch](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650722553%26idx%3D1%26sn%3Dce635e60fa8f1cc16982c5d6a9a6931b%26chksm%3D871b1487b06c9d9180d7f881784e68d4b9785481c38aa86eccc183aed8254b2a452e073a0c9b%26scene%3D21%23wechat_redirect) 方面的工作展示了 FAIR 致力于快速迭代、有意义的影响、开放式系统以及与 AI 社区的合作。PyTorch 最初只是一小部分 FAIR 研究员的小小尝试。我们没有选择构建一个全新的深度学习框架，而是基于 Torch 开源库构建新框架，我们整合了英特尔和英伟达的加速库以最大化速度。我们于 2017 年初开源 PyTorch，天啊，距离现在还不到两年！现在它是 GitHub 上增长第二快的开源项目，也是全球 AI 开发者的框架之选。今年 10 月，数百名 AI 社区成员参加了首届 PyTorch 开发者大会，聆听 Caltech、FAIR、fast.ai、谷歌、微软、英伟达、特斯拉等的演讲。现在 PyTorch 1.0 版本整合了 Caffe2 的模块化、生产导向能力和 ONNX，为从原型制作到生产部署的整个过程提供无缝路径，PyTorch 1.0 还与云服务和技术提供商有深入融合。
![](https://pic3.zhimg.com/v2-49dba3a46a5d2a2ecfd74f61413e6ec6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='488'></svg>)
PyTorch 被整合进 Facebook 的产品中，被数十亿人使用，同样也被 FAIR 其他研究项目使用，比如 fairseq(-py)，它比之前版本的翻译速度提高了 80%。Facebook 的强化学习智能体 ELF OpenGo、EmbodiedQA，以及在具备标签的数十亿公开图像上训练图像识别网络的成功研究也使用了 PyTorch。除了 Facebook，PyTorch 还支持 [AllenNLP](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650730716%26idx%3D4%26sn%3D68880ab8a8dbd452c13d383528e15da8%26chksm%3D871b34a2b06cbdb443b013051d54fcb09b8b1f1e87f01857115f0eb48bfff969ff0cc797075d%26scene%3D21%23wechat_redirect)、纽约大学教授 Narges Razavian 等的研究和项目，现在 Udacity 和 fast.ai 正在帮助更多的人使用 PyTorch。

PyTorch 加快并简化了模型从研究阶段到生产阶段的过程，而 [FAISS](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650723805%26idx%3D5%26sn%3D83159641bd2cbf89c6f4f1af79ea0447%26chksm%3D871b11a3b06c98b5c420eee4df0b40adef1c631bfc32ec6fb0c1442189a25c5c36004f46b812%26scene%3D21%23wechat_redirect) 则加速了大规模搜索。FAISS 最初是一项内部研究项目，旨在更好地利用 GPU 识别与用户偏好相关的相似性，现在它是同类库中速度最快的，且能够利用十亿规模级的数据集。FAISS 为推荐引擎和基于 AI 的助理系统提供了可能性。去年，我们开源了 FAISS 库，它被开发者社区广泛采用，在 GitHub 上有 5000 多星，并整合进英伟达的 GPU 加速 scikit-learn 库 cuML。




**计算机视觉领域新基准**

尝试理解智能的本质属于多模态感知研究，但是 FAIR 过去五年的经历却是实实在在依靠计算机视觉的发展。在 FAIR 成立之前，Facebook 只有少数 AI 专家试图更好地理解图像中的像素如何表征人，以使合适的照片在合适的时间出现在人们面前。2017 年，FAIR 研究员何恺明等人的论文[《Mask R-CNN》](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650724526%26idx%3D2%26sn%3D10ccdf74b480da89fce21ca7501718c1%26chksm%3D871b1cd0b06c95c60c0a03580f5a21abd03c06193f1ac67440e6fb69609af5e7b7c029355568%26scene%3D21%23wechat_redirect)荣获 ICCV 最佳论文奖，该研究将计算机视觉领域最好的东西结合起来：目标检测和语义分割。
![](https://pic3.zhimg.com/v2-109fc32087552cbf2eb0acc2a4c1eba6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
该论文写道：「Mask R-CNN 没有使用其它技巧，但其表现超越了在每个任务上已有的任何单个模型，包括 COCO 2016 挑战赛的获胜模型。」该研究迅速成为更广泛 AI 社区中计算机视觉研究的基础。之后该技术被整合到 Facebook 的开源 [Detectron 系统](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650736547%26idx%3D3%26sn%3Da326f17f832cec4827596291930e76dd%26chksm%3D871ac3ddb06d4acb3ffc56ffd56b340d5f15b7180a8aff5a38ea41c3c4e41e8d96da4c5a0a08%26scene%3D21%23wechat_redirect)中，将这一易用、快速、准确的元算法带给世界各地的研究人员。

这一基础性工作支撑着 Facebook 目前的大量系统，如使用目标识别技术为视障人士生成照片说明的自动替代文本（automatic alt text），以及检测不良信息的工具。它也是未来应用的基石：Facebook 全平台的 AR 功能和 Portal 中的 Smart Camera 都根植于这一研究。该研究仍在继续，不过研究重心转到了视频，[DensePose 项目](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650743916%26idx%3D2%26sn%3D7cebac3f3dba33251a193c191febcf29%26chksm%3D871ae012b06d6904a45376b1fe052182ae830d7f6270a4484659502bf833144f4384cd5629d3%26scene%3D21%23wechat_redirect)将帮助 Facebook 的系统像理解照片那样理解视频内容。
![](https://pic3.zhimg.com/v2-7d7a836725bd5880b4d8288cc3c8a386_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='718' height='391'></svg>)DensePose 为移动的人创建 3D 表面的视频截图，来源：https://www.facebook.com/Engineering/videos/10156359820477200/
**图像理解：更快速的训练和更大的数据集**

计算机视觉不是 FAIR 尝试解决规模化挑战的唯一领域。FAIR 与 Facebook 应用机器学习（AML）团队合作，解决训练速度、训练数据集大小以及缺少监督数据集的问题。在今年早些时候发表的一篇论文里，AML 团队介绍了他们[如何在带标签的大型公开图像数据集上训练图像识别网络](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650741779%26idx%3D4%26sn%3D8857330169f8b575637fa593c63c620b%26chksm%3D871ad86db06d517bdcd6eee833b71e0ebbded037b0787c1b78f29df5cd9500d3f2fa225acf7d%26scene%3D21%23wechat_redirect)，其中最大的数据集包括 35 亿张图像和 1.7 万个标签。该数据集规模比之前研究的数据集大一个数量级，但准确率达到 85.4%，是目前该领域已发布研究中得到的最好结果。

这一突破和 [FAIR 在训练速度方面的研究](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650727685%26idx%3D5%26sn%3D0c0fe31f930ede3f958a8079d1c31fa8%26chksm%3D871b217bb06ca86d78fa679c80e474979cd34d910c659a44cb650bb125d20f55e6170cc63aa0%26scene%3D21%23wechat_redirect)不无关系，FAIR 在 ImageNet 数据集上的训练速度比之前的最优研究快一个数量级。他们将训练时间降到一小时以内，展示了如何使用比之前实际可用 minibatch 规模大一个数量级的 minibatch 执行 SGD 训练。他们称：「为了达到这个结果，我们采用了线性缩放规则（linear scaling rule）作为 minibatch 大小函数来调整学习率，并开发了一种新的预热方案（warmup scheme），该方案会在训练前期克服优化的困难。」

在训练速度实现这一改进后，我们能够在更大的数据集上对弱监督学习进行直接研究。这两项研究的结果表明 FAIR 和 AML 之间合作的价值。当解决 AI 问题这一学科领域受到实际研究和生产应用的支持时，我们看到了快速、当前最优的结果。




**FAIR 的未来**

我们创建 FAIR 的最终目标是理解智能，发现其基本原则，使机器更加智能。这一目标始终没有变过。我们将继续扩展研究领域，如开发能够通过自监督学习获得世界模型的机器，训练机器进行推理，以及训练机器规划和设想动作的复杂结果。这是我们研究机器人学、视觉推理和对话系统的原因之一。上文介绍了一些具体项目，展示了已有的成绩，但是我们在科技方面还有很长的路要走，才能使机器足够智能，走入人们的日常生活。

原文链接：[https://code.fb.com/ai-research/fair-fifth-anniversary/](https://link.zhihu.com/?target=https%3A//code.fb.com/ai-research/fair-fifth-anniversary/)



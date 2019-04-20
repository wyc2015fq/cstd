# Facebook ：AI 年度总结来啦 - 人工智能学家 - CSDN博客
2019年01月21日 22:40:56[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：160
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQcZ0lcDqD5R2NJu8utTFCo1aNTqufP5tHlF96wp2H9eiabZbo2Cw3Gibg1JQzbqvHxeiavqj3bgWibg/640?wx_fmt=jpeg)
来源：AI 科技评论
摘要：最近，Facebook 做了一份 AI 年度总结，详述了他们过去一年在 AI 上所做的代表性工作。
在 Facebook，我们认为，人工智能以更有效的新方式学习，就像人类一样，可以在将人们聚集在一起发挥重要作用。这一核心信念有助于推动我们的 AI 战略，将投资重点放在与使用真实数据学习的系统相关的长期研究上，激励工程师与更广泛的人工智能社区共享尖端工具和平台，并最终展示使用技术造福世界的新方法。
2018 年，我们在各个领域都取得了重要进展。我们提出了新的研究，除了图像识别上的进展，在NLP领域，我们现在可以用较少的监督数据翻译更多的语言。我们发布了许多平台和工具来帮助其他人将他们的人工智能研究转化为生产应用程序，包括我们将目前流行的开源深度学习框架 PyTorch 升级成新的、更通用的 1.0 版本。除了发表广泛的公共研究论文和相关模型以及数据集外，我们还发现人工智能有潜力通过 MRI 扫描（https://code.fb.com/ai-research/fastmri/）、灾难援助（https://code.fb.com/ai-research/satellite-imagery/）和预防自杀的工具来改善人们的生活。以下是我们过去一年在人工智能领域所做努力的亮点内容。
**通过半监督和无监督训练促进人工智能学习**
Facebook 人工智能研究（FAIR）小组的创建目标之一是致力于开发具有人类智能的系统。实现这一里程碑还需要多年的研究，但我们相信，我们在2018年所做的努力有助于展示出一条通向通用人工智能的道路。虽然目前大多数 AI 系统使用监督学习来理解特定的任务，但对大量标记样本的需求限制了他们可以学习的任务数量，并限制了技术的长期潜力。这就是为什么我们正在探索多种方法来减少训练中所需的监督，包括展示从半监督甚至非监督数据中学习从而受益的项目。
例如，为了增加系统可能翻译或理解的语言数量，我们演示了一种新的方法（https://code.fb.com/ai-research/unsupervised-machine-translation-a-novel-approach-to-provide-fast-accurate-translations-for-more-languages/），在无监督数据上训练自动翻译 NMT 模型，其性能与在监督数据上训练的系统相当。我们的系统的准确性比以前的无监督方法有了很大的提高。通过减少该领域对标记训练数据的大型语料库的依赖，它打开了翻译更多语言的大门，包括资源比较少的语言（如乌尔都语），和英语相比，它们的数据集是非常有限的。
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmg4Iz6A5MWfkX6AqbgWPgAkfZtQ4CIH8P3h4gfYlZrjINMwdp9JjTKKpWCj3AMtJH4IibpqAwmwnQA/640?wx_fmt=gif)
多种语言中的二维单词嵌入可以通过简单的旋转进行对齐
另一个项目完全使用资源比较少的语言，使用多种方法来规避标记训练数据的相对稀缺性。这项工作包括使用多语言建模来利用给定语言组（如白俄罗斯语和乌克兰语）中方言之间的相似性。这是一项应用研究，该团队采用的一系列技术在今年为我们的自动翻译服务增加了 24 种语言。此外，在与纽约大学的合作中，我们在现有的 MultiNLI 数据集中添加了 14 种语言，这些语言被广泛用于自然语言理解（NLU）研究，而在此之前，我们只研究了英语。我们更新的 XNLI 数据集（https://code.fb.com/ai-research/xlni/）中的语言包括两种资源很少语言（斯瓦希里语和乌尔都语），我们的方法有助于跨语言理解，从而减少了对监督训练数据的需求。
我们还展示了数据监督的变化，例如通过数据精馏（data distillation）将有监督和无监督的数据结合起来，这一过程称为全方位监督学习（omni-supervised learning，https://code.fb.com/ai-research/data-distillation-makes-omni-supervised-learning-possible/）。在基于哈希标签的图像识别（https://code.fb.com/ml-applications/advancing-state-of-the-art-image-recognition-with-deep-learning-on-hashtags/）的研究中，我们创造性地利用现有的非传统标签来生成大量训练数据集，这些数据集里面基本上是自标记数据，包括一组 35 亿张的 Instagram 公共图像。该项目建议用户提供的哈希标签可以充当数据标签，将现有的图像转换为弱监督训练示例。我们的结果不仅证明了使用数十亿个数据点对基于图像的任务是非常有效的，而且还使我们打破了一个重要的记录，也就是比以前最先进的图像识别模型在 ImageNet 上的准确度得分高出 1%。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmg4Iz6A5MWfkX6AqbgWPgAkUr5dQjQl9Ao8Qiaov0EdiaA1GA86vZog5icp3Smg9wia7MubXfJCDsNNNQ/640?wx_fmt=jpeg)
哈希标签可以帮助计算机视觉系统识别图像中的特定的子类别和附加元素
**加速 AI 研究向生产的转变**
AI 已经成为 Facebook 几乎所有产品和服务的基础，应用程序的多样性反映在我们的工程师正在构建和增强的各种基于 AI 的平台和工具中。2018 年我们平台工作的共同主题是：将我们正在研究的 AI 技术转化为可以部署的 AI 系统。
自 2017 年我们发布 PyTorch 以来，深度学习框架已被 AI 社区广泛采用，目前，它是 GitHub 上增长速度第二快的开源项目。PyTorch 的用户友好界面和灵活的编程环境使其成为人工智能发展中快速迭代的通用资源。它的开放设计确保了框架将继续增长和改进。2018 年，我们希望给 PyTorch 社区提供一套更统一的工具，重点是将他们的人工智能实验转化为生产就绪的应用程序。
我们在 2018 年 5 月的第 8 次会议上宣布了框架的更新，详细说明了它如何集成 Caffe2（https://caffe2.ai/） 的模块化、面向生产的功能以及新扩展的 ONNX（https://code.fb.com/developer-tools/onnx-expansion-speeds-ai-development/），以简化从原型系统到部署的整个 AI 开发流程。10 月，我们在第一次 PyTorch 开发者大会上发布了 PyTorch 1.0 开发者预览版（https://code.fb.com/ai-research/facebook-accelerates-ai-development-with-new-partners-and-production-capabilities-for-pytorch-1-0/），在会议上我们还展示了框架快速增长的合作伙伴和平台生态系统。Google、Microsoft、NVIDIA、Tesla 和许多其他技术提供商在那次活动中讨论了他们的现状和与 PyTorch 1.0 的集成计划，fast.ai 和 Udacity 都创建了使用新版本来教授深度学习的课程。
我们在今年 1 月初推出 PyTorch 1.0 完整版，这一版本包括我们一直在做的所有新特性，如能在 eager 和 graph 执行模式间无缝过渡的混合前端，改进的分布式训练，用于高性能研究的纯 C++前端。2018 年我们还发布了扩展 PyTorch 核心功能的工具和平台，包括两个内核库 (QNNPACK 和 FBGEMM)，它们能使移动设备和服务器更容易运行最新的 AI 模型，我们还发布了 PyText（https://code.fb.com/ai-research/pytext-open-source-nlp-framework/），这个框架能加速自然语言处理 (NLP) 开发。
PyTorch 还为 Horizon（https://code.fb.com/ml-applications/horizon/）提供了基础，Horizon 是第一个使用应用强化学习在大规模生产环境中优化系统的开源端到端平台。Horizon 采用了 RL 大量研究但很少部署的基于决策的方法，并对其进行了调整，使之用于可能包含数十亿条记录的数据集的应用程序。
在 Facebook 内部部署该平台后，在优化流媒体质量和改进 Messenger 中 M suggestions 等用例中，我们开源了 Horizon，使得任何人都可以抵达连接 RL 研究和生产的桥梁。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmg4Iz6A5MWfkX6AqbgWPgAkccyJHKSJ36UicYz3KbHtYA8cpggDdtBC25Whp2LSaPoT3JtT96F6kHA/640?wx_fmt=png)
显示 Horizon 反馈循环的高级图。首先，对现有系统记录的一些数据进行预处理，然后，训练模型并分析离线环境下的反事实的策略结果，最后，将模型部署到一组人员中，并度量真正的策略。来自新模型的数据会反馈到下一个迭代，大多数团队每天都会部署一个新模型。
我们还发布了 Glow（https://code.fb.com/ml-applications/glow-a-community-driven-approach-to-ai-infrastructure/），这是一个社区驱动型开源框架，支持机器学习硬件加速。Glow 支持一系列不同的编译器、硬件平台和深度学习框架，包括 PyTorch，现在支持它的生态合作伙伴包括 Cadence、Esperanto、英特尔、Marvell 和高通。为了进一步鼓励业界使用机器学习，我们发布了一个新的机器学习优化服务器设计 Big Basin v2（https://code.fb.com/ml-applications/the-next-step-in-facebook-s-ai-hardware-infrastructure/），这也是 Open Compute Project（开放计算项目）的一部分。我们已经将新的、模块化的硬件添加到了数据中心机群，大家可以在 OCP Marketplace（https://www.opencompute.org/contributions）上下载不同版本的 Big Basin v2。
2018 年是 Oculus 研究向 Facebook Reality 实验室过渡，以及对 AI 和 AR/VR 研究重叠领域进行新探索的一年。我们不断努力，想要开源尽可能多的人工智能相关工具，作为这些工具的一部分，我们已经发布了 DeepFocus 项目的数据和模型，该项目利用深度学习算法在 VR 中渲染逼真的视网膜模糊（https://www.oculus.com/blog/introducing-deepfocus-the-ai-rendering-system-powering-half-dome/）。DeepFocus 是一种将深度学习用于 AR /VR 的全新应用，它使用一个完全新型的网络结构来适配于 Half Dome 头显，以及其他头显装置。
在未来的一年，我们希望得到更多关于所有这些开源工具的反馈。我们将继续开发和开源一系列工具，进一步支撑 PyTorch 1.0，帮助整个开发社区，让前沿人工智能系统走出实验室，走出研究型论文，投入生产。
**构建造福所有人的 AI 系统**
我们研究广泛传播 AI 福祉的技术已有悠久的历史，比如创建能生成照片音频描述的系统，造福于视障人士。在过去一年，我们继续开发基于 AI 的功能来造福世界，包括使用文本分类技术（https://code.fb.com/ml-applications/under-the-hood-suicide-prevention-tools-powered-by-ai/）来识别表达自杀想法言论的帖子，扩展现有的自杀预防工具。这一系统使用单独的文本分类器来分析文章和评论中的文本，如果合适，它将把这些内容发送给 Community Operations 团队审核。这个系统利用我们已经建立的文本理解模型和跨语言功能，通过支持服务，来增加可以连接到人员数量。
我们还发布了一种使用人工智能快速准确地帮助确定受灾最严重地区的方法（https://code.fb.com/ai-research/satellite-imagery/），无需等待人工标注数据。这种方法是与 CrowdAI 合作开发的，有可能以更快的速度和效率向受害者提供救援。在未来，这项技术还可以用来帮助量化大规模灾害的损失，如森林火灾、洪水和地震。
我们部署了一个名为 Rosetta （https://code.fb.com/ai-research/rosetta-understanding-text-in-images-and-videos-with-machine-learning/）的 ML 系统，它每天从超过 10 亿个公共图像和视频帧中提取文本，并使用文本识别模型来理解文本和图像的上下文。Rosetta 支持多种语言，能帮助我们理解表情包和视频的内容，包括自动识别违反策略的内容。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmg4Iz6A5MWfkX6AqbgWPgAkJuSh8a8rzhnQxKic30O7aJJrMeichM7STiaqOqOWFq98KNYX8K7zmtRNA/640?wx_fmt=png)
Rosetta 文本提取两步模型架构
2018 年我们也开始了 fastMRI 项目（https://code.fb.com/ai-research/facebook-and-nyu-school-of-medicine-launch-research-collaboration-to-improve-mri/），这是我们与纽约大学医学院 (NYU School of Medicine) 开展的一项长期合作，旨在改善影像诊断技术，起初我们将 MRI 的扫描速度提高了 10 倍。目前的扫描需要一个小时或更长时间，对于一些患者或者在某些情况下，这肯定不行，该联合研究项目的目的是通过使用深度学习，从较少的原始扫描数据中生成图像，增加这种潜在的救生诊断工具的可用性。
fastMRI 的目的不是开发专有技术，而是加速领域内对这种技术的理解，纽约大学医学院已经做了迄今最大的充分采样的原始 MRI 数据集（数据集完全匿名）用于研究，也有开源模型来帮助更广泛的研究社区开始这项任务。我们还推出了一个在线排行榜（http://fastmri.org/leaderboards），大家可以发布自己的结果进行比较。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/vJe7ErxcLmg4Iz6A5MWfkX6AqbgWPgAko7VRjFPK1K1QPHlqlh30CK6tMSSD1Nn1ToTnrUCZwIBm8uW3nJatIQ/640?wx_fmt=png)
左图：转换成图像之前的原始 MRI 数据。为了获取诊断研究所需的全部原始数据，MRI 扫描会非常耗时。右图：从充分采样的原始数据中获得的膝关节重建 MRI 影像图。
2018 年，我们还发表博客详细介绍了我们在其他领域的工作，包括使用人工智能来改进系统（Getafix、 predictive test selection、SapFix、Sapienz 和 Spiral）、提升产品（SLAM 和 Marketplace 中的 AI），以及其他研究工作（wav2letter++，多词汇表征、多语言嵌入和音频处理）。
我们对 2018 年在关键层面（开展基础研究、部署前沿应用、分享使用人工智能帮助他人的新方法）取得的进展感到兴奋，期待在未来一年继续努力。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)

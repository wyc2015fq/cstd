# NeurIPS 2018 AutoML中国队伍表现强势！Phase1冠亚季队伍联合经验分享 - 知乎
# 



机器之心专栏，作者：罗志鹏、陈相宁、方鑫。

近期，NeurIPS 2018 AutoML 挑战赛公布了最终结果。本次竞赛由第四范式、ChaLearn、微软和阿卡迪亚大学联合举办，专注于终生机器学习领域。竞赛总共分为两个阶段，Feed-Back 阶段及 Blind-Test 阶段，在 Feed-Back 阶段，**微软与北京大学组成的 DeepSmart 团队斩获第一名，MIT 和清华大学组成的 HANLAB 斩获第二名，南京大学 PASA 实验室斩获第三名**。Blind-Test 阶段由 Autodidact.ai、Meta_Learners、GrandMasters 斩获前三名。


**Feed-Back Top3 队伍介绍**

**DeepSmart 队伍：**
- 罗志鹏：微软 Bing 搜索广告算法工程师，北京大学软件工程专业硕士，专注于深度学习技术在 NLP、广告相关性匹配、CTR 预估等方面的研究及应用。
- 黄坚强：北京大学软件工程专业硕士在读，擅长特征工程、AutoML、自然语言处理、深度学习。

DeepSmart 队伍曾经获奖记录：
- CIKM Cup 2018 1st place
- KDD Cup 2018 (Second 24-Hour Prediction Track) 1st place
- KDD Cup 2018 (Last 10-Day Prediction Track) 1st place
- Weibo Heat Prediction 1st place 
- Shanghai BOT Big Data Application Competition 1st place
- Daguan text Classification 1st place




**MIT HAN LAB 队伍：**
- 陈相宁：清华大学大四本科生。
- 韩松：麻省理工大学助理教授。

HAN Lab (Hardware, AI, and Neural-nets) 专注于深度学习的硬件软件协同优化，高性能、低功耗的 AI 处理器以及高效 AI 模型的设计自动化。
- H: High performance, High energy efficiency Hardware 
- A: AutoML, Architectures and Accelerators for AI
- N: Novel algorithms for Neural Networks
- S: Small models, Scalable Systems, and Specialized Silicon




**PASA (formly Fong)（南京大学 PASA 实验室团队）队伍：**

PASA 团队研究方向包括 AutoML 算法研究与系统实现, 团队提出的基于强化学习的 AutoML 算法和框架曾获得 PAKDD2018 AutoML2 Challenge Feed-back phase 1-st place and Blind-test phase 3-rd。

**大赛简介**

在人工智能的时代，许多实际应用程序都依赖于机器学习，然而这些程序的开发人员却并不都具备专业的机器学习算法研发能力，因而非常需要部署 AutoML 算法来自动进行学习。此外，有些应用中的数据只能分批次获取，例如每天、每周、每月或每年，并且数据分布随时间的变化相对缓慢。这就要求 AutoML 具备持续学习或者终生学习的能力。这一类的典型问题包括客户关系管理、在线广告、推荐、情感分析、欺诈检测、垃圾邮件过滤、运输监控、计量经济学、病人监控、气候监测、制造等。本次 AutoML for Lifelong Machine Learning 竞赛将使用从这些真实应用程序中收集的大规模数据集。相比于与之前的 AutoML 比赛，本次比赛的重点是概念漂移，即不再局限于简单的 i.i.d. 假设。要求参与者设计一种能够自主（无需任何人为干预）开发预测模型的计算机程序，利用有限的资源和时间，在终身机器学习环境下进行模型训练和评估。

本次比赛分为 Feed-Back 阶段及 Blind-Test 阶段：
- Feed-Back 阶段: 反馈阶段是代码提交的阶段，可以在与第二阶段的数据集具有相似性质的 5 个数据集上进行训练和测试。最终代码提交将转发到下一阶段进行最终测试。
- Blind-Test 阶段：该阶段不需要提交代码。系统自动使用上一阶段的最后一次提交的代码对 5 个新数据集进行盲测。代码将自动进行训练和预测，无需人工干预。最终得分将通过盲测的结果进行评估。

**大赛结果**
![](https://pic3.zhimg.com/v2-be17d67f2fda7c788a7b91114a2c9eb2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='865' height='444'></svg>)图 1：Feed-Back 阶段排行榜
榜单链接：[https://competitions.codalab.org/competitions/20203#results](https://link.zhihu.com/?target=https%3A//competitions.codalab.org/competitions/20203%23results)

从 Feed-Back 榜单来看 Top3 队伍取得了非常优异的成绩，特别是 DeepSmart 队伍在 5 项任务上取得了 4 项第一，1 项第二的好成绩，其中 4 项任务表现尤为突出。
![](https://pic1.zhimg.com/v2-57e457e955adb2fd97b253079ae85810_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='865' height='233'></svg>)图 2：Blind-Test 阶段排行榜
上图分别展示了 Feed-Back 阶段及 Blind-Test 阶段的排行榜。值得注意的是，在 Feed-Back 阶段的 Top6 队伍中有 5 支队伍并未进入 Blind-Test 阶段的排行榜。本次 AutoML 竞赛在时间及内存上有很大的限制，其中两个阶段在内存上都限制为 16G，Feed-Back 阶段 5 个任务限制总时间为 7800 秒，Blind-Test 阶段 5 个任务限制总时间在 10400 秒以上。比赛说明中明确 Blind-Test 与 Feed-Back 的数据是可比的，在 Feed-Back 阶段中 Top 队伍都能在限制的时间和内存内完成 5 项任务的建模，然而在 Blind-Test 中 Feed-Back 阶段具有领先优势的 Top 队伍在 Blind-Test 中因在某个数据或者特征量相对较大的任务上没能在限制的内存之内完成 AutoML 的整个建模流程，因此在 Blind-Test 中无得分，尽管 Feed-Back 阶段 Top 队伍做了许多优秀的工作，但最后 Top 队伍还是无缘 Blind-Test 榜单。
![](https://pic4.zhimg.com/v2-5e8982df32b90d661eae7925d37586b7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='865' height='166'></svg>)
本次比赛在五个不同数据集上以 AUC 作为评分指标，以 5 个数据集的平均排名作为排行榜排名，在 Blind-Test 阶段，切换到五个不同的数据集。对比 Feed-Back Top3 队伍与 Blind Test Top3 队伍的成绩，从平均 Rank 指标上 Feed-Back Top3 均值为 2.8，Blind Test Top3 队伍的均值为 8.467，相当于整体实力要至少落后 5 个队伍；从平均 AUC 指标来看，Feed-Back Top3 队伍最差成绩比 Blind Test 最好成绩差 **3.864%**。从每项任务的 AUC 来看，Feed-Back Top3 最好成绩比 Blind Test 最好成绩分别高 **4.43%、3.85%、5.71%、12.47%、8.39%**。AUC 指标是一项相对而言很难提升的指标，通常在竞赛中 top 队伍只能在该指标上拉开千分位、万分位的差距，而 Feed-Back Top3 在 5 项任务中领先 Blind-Test 阶段 Top 3 队伍 **3.85-12.47%**，具有非常明显的优势。所以 Feed-Back 阶段的 Top 队伍的方案具有非常重要的价值。所以主办方也非常认可 Feed-Back 阶段解决方案的价值，向前三名分别授予了奖牌和证书，并且邀请他们参加 NeurIPS 会议进行解决方案分享。在本篇文章的后文，Feed-Back Top3 队伍将分享他们队伍在这次比赛中的经验。




**经验分享**

**DeepSmart：**

我们团队基于所给数据实现了一个 AutoML 框架，包括自动特征工程、自动特征选择、自动模型调参、自动模型融合等步骤，并且利用了多种策略对运行时间进行了有效的控制，以确保解决方案能在限制时间内通过。尽管我们在 Feed-Back 阶段使用内存的峰值只有 8G, 但是出乎我们意料的是，最终我们的解决方案却由于内存不足而无缘最终排行榜。
![](https://pic3.zhimg.com/v2-88c8a098eadd90c23c26023411696a22_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='864' height='904'></svg>)- **自动特征工程：**目前，在大部分机器学习工业界应用中，数据和特征对于模型往往是最为关键的，在现有的 AutoML 框架中，大部分特征的提取是通过数值特征进行高阶组合，它们的模型很难提取出跟时序问题或者概念漂移有关的关键特征，这也是我们取得不错效果的重要原因。然而现实中存在大量的时序问题，而且往往带有概念漂移。我们构建的自动特征工程不仅是基于数值特征做横向高阶组合，同时我们基于分类变量、数值变量、时间变量、多值分类变量自动提取跨时间和样本的横向及纵向高阶组合。我们借鉴了之前多次比赛经验，如何处理数据和不同类型的特征才能表现得更好，这次竞赛，一方面是我们不断强化提升的过程，同时也是我们总结的过程。这是我们团队所做出的不同于以往模型的自动特征工程方案，同是也是我们在比赛中能取得显著优势的重要因素。
- **自动特征选择：**高阶组合往往容易导致生成大量特征，但我们通过构建自动快速特征选择来提取重要特征，这能极大地加速我们的特征工程，同时也极大地减少模型所使用的特征总量。
- **自动模型调参：**我们采用 GBDT 模型，通过验证集调整模型学习率、叶子结点、树的深度等，由于时间有限，我们只能进行较少次的模型调参及验证，这三个参数对于 GBDT 的影响较大，能取得较为显著的效果提升。由于数据是极度不平衡的，有的数据集甚至只有几千个正样本，负样本却能达到几十万个。所以，我们在树提升的过程中使用了不同的行采样方法。在现有的树提升模型中，他们对正负样本采用同一个行采样率来控制采样。我们使用不同的正负样本行采样率，以使得模型可以使用足够多的正样本和合适的负样本。它不仅可以大大减少模型的运行时间，同时模型也可以取得更好的效果。
- **自动模型融合：**基于所给时间，我们使用不同的行采样及列采样来训练多个不同的模型，这不仅仅更加充分的利用了样本，也使得每个模型使用的数据和特征具有充分的差异性，同时大大减少了过拟合的风险。




**MIT HAN LAB：**

针对终身学习的 task 下，提出 Hierarchical Ensemble Successive Halving（HESH）。将 GBDT 中的 tree boosting 类比神经网络学习中 back propagation，创新性的将 Successive Halving 运用到 GBDT 中，高效筛选出最优的模型以及超参数。HESH 只基于当前 batch 建立新 model，不涉及数据集的存储与继承，保证速度与高效性。另外，HESH 运用两级 ensemble selection，当新 batch 到达，首先调整底层 ensemble 权重适应当前 concept，同时剪枝模型自适应超参数。之后建立顶层 ensemble 融合历史模型，有效提取适应当前 concept 的信息。通过模型剪枝以及两层自适应集成学习，HESH 能够有效进行终身学习中的模型筛选以及调参，并解决潜在的概念漂移问题。

提出基于强化学习的自动特征工程框架。自动构建并筛选特征。针对数值特征，主要是加减乘除变换；针对类别特征，主要是 value_count 以及 groupby 变换。Automated Feature Engineering 利用 controller 预测变换序列，接着运用 policy gradient 更新 controller。为了应对紧缺的时间，reward 信号被重新设计成为全新的 feature importance 函数，在不需要训练模型的情况下就能有效推断 feature 的价值。实验证明，自动特征工程对于最终的性能带来了较大提升。




**PASA (formly Fong)：**

本届 AutoML 比赛和之前 AutoML 比赛最大的区别主要存在两个方面：一是输入数据集的特征具有较强的多样性，而且需要选手自行处理输入数据集的特征；二是需要处理 Lifelong mechine learning 场景下的概念漂移（concept drift）问题。首先，在特征工程方面，PASA 团队运用了自动化特征编码、自动化特征组合以及自动化特征选择等方法实现特征的设计和提取。其次，在处理概念漂移方面 PASA 团队通过设计在线增量学习框架捕获长期的 concept 和短期的 concept，并采用基于时间窗口的自适应模型加权集成学习算法提升预测性能，集成学习的基学习器可采用梯度提升树模型。

除此之外，为了提升算法的通用性，PASA 团队在模型自动选择和超参调优方面也做了一定的优化。因篇幅有限，不再赘述，欢迎感兴趣者与南京大学 PASA AutoML 团队交流。




**总结**

近几年来，AutoML 发展十分迅速，人们期待它能够将机器学习从业者从解决繁冗复杂的数据预处理、特征工程、模型选择、模型调参过程中解放出来，更加专注于自己的目标，同时也为非专业人士带来方便可用的自动化机器学习工具；同时，AutoML 本身的复杂性引起了学术界的广泛关注，为学术界带来了更多挑战。

在此次的 AutoML challenge 中，竞争异常激烈，在长达近三个月的比赛过程中，大家不断刷新榜单成绩，迸发出许多有用的想法和创新，对于每一支参赛队伍来说都是一次成长过程，并且间接的推动了 AutoML 领域的发展。我们相信 AutoML 技术将为许多实际问题提供一个良好的解决方案。

最后感谢主办方的辛勤付出，为我们提供本次竞赛机会，让我们能够与 AutoML 领域的其他团队同台竞技，感谢所有的参赛队伍，让我们不断进取，不断创新，助力 AutoML 领域的发展。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*




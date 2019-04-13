
# AI Challenger 2018 机器翻译参赛总结 - Paper weekly - CSDN博客


2019年01月07日 13:02:49[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：298


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

**金山集团 AI Lab 组队参加了****AI Challenger 2018 全球挑战赛的英中机器翻译项目，并且获得冠军。**

AI Challenger 2018 主题为"用 AI 挑战真实世界的问题"，是目前国内规模最大的科研数据集平台、最大非商业化竞赛平台，最关注前沿科研与产业实践相结合的数据集和竞赛平台，也是 2018 年度中国超高水准的 AI 竞赛。本次比赛使用的数据总量达到 1300 万句对，其中具有上下文情景的中英双语数据达到 300 万句对，相比去年大幅扩容。

在此，参赛团队就技术和经验做一些分享，希望对大家有帮助。

# 工具介绍

机器翻译的开源库很多，比如 OpenNMT、FairSeq 和 tensor2tensor 等，**我们主要是基于 tensor2tensor 等工具库进行的程序实现**。它是 Google 基于 TensorFlow 开发的高级库，内置了许多经典模型，开发调试比较方便。

我们使用了**3 台 V100 GPU 服务器**及**1 台 32 核的 CPU 服务器**作为主要的实验设备。

我们选用**Transformer 模型**作为我们的 baseline 模型。

# 数据清洗

优质的数据不管在哪个领域下都是有益的。对于一个任务来说，我们**首先要进行的就是数据的分析及清洗**。数据清洗的一个通常操作就是去除重复数据，原始语料中存在着 6.56% 的重复，共约 90w 个样本，对这些样本我们进行了去重操作，一般直接删去即可。

另外我们**对源句子与目标句子长度比例进行了检测**，当长度比超过一定的阈值时我们就将对应的平行语句对进行删除。

同时我们还注意到**有一部分语料存在着对齐错误**，对此我们使用了 giza++ 对训练数据进行了对齐并获得一份双语词典。使用获得的双语词典我们就可以对平行语料进行漏翻检测，通常我们会对语料的漏翻程度进行打分，分值超过一定阈值时，我们就会删除对应的语料。

下表可以看到，分值越低，删除的语料越多，结果有了些许提升。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk7141ncvsuOicTIJ05306lElfWCQXexJst3zCnDbjibiaMFBfEicbyk4U3BbDMNd7lVR8zMCpkP0X3Ig/640?wx_fmt=png)

# 数据增广

在本次比赛中，**我们使用了两种数据增广手段，分别是回译和交换。**

NMT 中用回译的方法扩充语料是常用的数据增广技术，见 Facebook 在 WMT18 英译德的冠军论文*Understanding Back-Translation at Scale*[1]。在该论文中，仅依靠回译生成的语料做数据增广就能将 BLEU 提高 1 至 2 个点。

在回译时，我们**基于现有语料训练了一个从目标语言到源语言（中翻英）的翻译模型**。将目标语言语料输入该模型就能获得对应的源语言语料，将二者结合后就得到了新的平行语料。当然，在 Facebook 的论文中，他们使用了 226M 的单语语料去生成数据。

本次比赛不允许使用外部数据，所以我们直接使用原始预料中的中文部分进行生成。但是，这种方法会存在一个问题，就是新的平行语料与原始语料可能存在重复。针对这个问题，我们**在解码端加入了一定的随机噪声**，从而避免了这种情况。

**我们还使用了交换的方法，将原始语料中的英文语料的相邻的词都交换了一遍。**其实，把交换作为数据增广的手段有些牵强。交换的实际目的是为了增强模型的抗噪能力，但是我们还是通过交换语料的语序扩充了实验数据，所以把它算作数据增广的一种手段。

从表格中可以看到两种方法单独使用时都有了一定的提升，说明数据增广技术还是有一定效果的。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgk7141ncvsuOicTIJ05306lEKtGWRibflEvhxrlGJtbD8vj2u6odvuYTJ6vEnxS5enazQNIpFqNWE4Q/640?wx_fmt=jpeg)

但是需要注意的就是两种方法同时使用时效果会有一些下降。

# 模型改进

获得语料后，我们就开始尝试在模型层面进行一些改进。

**在分词实验时，我们共使用了三种分词方法**，分别是 tensor2tensor 中默认的分词方式，还有基于 character 级别的分词和使用 SentencePiece 的分词，后两种分词方法较第一种均有 1 个 bleu 值的提升。

我们还使用了 relative transformer，这个模型在 transformer_big 参数条件下提升了 0.3 个 bleu 值。模型细节详见*Self-Attention with Relative Position Representations*[2]。

**基于 transformer，我们提出了一种新的模型结构，叫做 layer-attention。**

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgk7141ncvsuOicTIJ05306lEtXZbUib64qT1ywrBruFOUnfq8t6fCE5jQI5cajbMIKW1JPppBwhgf0w/640?wx_fmt=jpeg)

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgk7141ncvsuOicTIJ05306lENWGafrOW5udDbPMgpFzFVvJ3Kh8niaIBr8ibqicQMIkaBhUG3eHm4UbibA/640?wx_fmt=jpeg)
**▲**模型结构图

该模型，在 transformer_big 参数下，在 newstest2014_ende 上面提升了 0.9 个 bleu 值。上图为原始的 transformer，从图中我们可以看到 transformer 是将 encoder 端最后一层的信息直接输出给 decoder 端。

而我们的改进，是**将 encoder 端所有层的输出进行了加权求和，然后将求和后得到的结果输入到 decoder 端中****。**因为时间原因，我们并没有在本次比赛的测试集数据上单独测试该模型的效果，而是将其使用在了最后的 rerank 中。

另外，在本次比赛给出的数据集中，约有 300w 的语料包含上下文信息，为了使用这些信息，我们使用了一种可以将上下文信息引入的模型叫做 contextual trasformer，模型结构见下图。

具体细节及实验设置见论文*Improving the Transformer Translation Model with Document-Level Context*[3]。该模型在 transformer_base 条件下有了 0.5 个 bleu 的提升。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgk7141ncvsuOicTIJ05306lEAa0kjgoxJprEVbcq1MldzfprQ3lhUPZMKmnVe1DoAMjVbyGHBUr9yg/640?wx_fmt=jpeg)

Contextual Transformer 就是在原始 transformer 的基础上引入了额外的 context encoder，并且在 transformer 的 encoder 和 decoder 端加入了 Context Attention 层。

这种结构增加了模型捕捉上下文信息的能力，并且因为依旧使用的 multihead 并行计算，所以训练和解码速度并没有下降很多。

# Finetune

finetune 就是使用少量的语料进行预训练模型的微调。如果使用过预训练的语言模型（如 ELMo，GPT 或 BERT），那么对于 finetune 就不会陌生。**本次比赛中，我们使用与测试语句相似的句子作为 finetune 语料，在现有模型基础上进行微调。**

我们对测试语料与训练语料进行了相似度打分并排序，从中选取出了与每句测试语句相似度最高的训练语料作为最终的 finetune 语料。

从表格的第一行我们可以看到，测试语句中*Send them to these foreign places”*与微调语料 a1 只有最后的符号不同。而第二行中的测试语句*the necessary excitation, even using crude natural crystals.*为两个微调语料 b1 和 b2 的结合。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgk7141ncvsuOicTIJ05306lEfaXfTDEQzFibLpOBhtWMpB38p0xyAxXicKtbTicRQaPwV2TiaJ8bEBtL0g/640?wx_fmt=jpeg)

经过这样的 fineune 训练后，我们的模型对于测试集的数据势必有所倾向。finetune 后的翻译表现也验证了我们的猜想。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgk7141ncvsuOicTIJ05306lEaaEvSmKrf7iaa5DUlZ2ytaToesokjFSu188YpR5dzjO3vKFn2gniadNg/640?wx_fmt=jpeg)

从表格中第一行可以发现，*Her secret is putting butter under the skin.*这句话的正确意思是她的秘诀是在鸡皮下面抹黄油。但是在微调前，我们得到的释义是她的秘诀是把黄油涂在皮肤上。而在 finetune 后我们得到了正确的释义把黄油粘在鸡皮上。这也说明 finetune 可以帮助我们获得了一些词在某些语境下的正确释义。

而第二行中，方括号内的语句*[Woman On P. A.]*，在 finetune 前并没有被翻译，但是经过 finetune 后可以看到我们获得了该句的翻译，*[P.A.上的女人]*，可见 finetune 也可以帮助我们降低漏翻的概率。

# Rerank

通过前面介绍的不同方法和尝试，我们获得了很多不同的模型。这些模型有的训练数据集不同，有的分词方式不同，有的模型结构不同，有的还进行了 finetune。我们将这些模型都做保留，**目的就是****为了保持不同模型的差异性**，用于进行后续的 rerank 实验。

在我们得到的所有结果中，他们的分值表现各不相同，但是势必会存在这样一种情况，BLEU 值较高的结果文件中也会出现翻译不好的语句，而 BLEU 值较低的文件中同样也会出现翻译比较好的语句。

**我们的目标就是将尽可能多的的翻译较好的语句筛选出来，组成最终的结果。**为此我们进行了一些尝试。

我们将解码时返回的 beam_score 作为排序分值依据，但是不同的模型有不同的表现，所以就很难在统一的度量下进行排序。所以针对不同的模型我们引入了不同的权重。使用 beam_score×weight 作为每个翻译结果的最终分值，通过筛选获得了最终的结果。

因此，**如何去获得准确的权重成为了一个问题**。我们首先通过人工调整尝试性地给出了一份权重值，但是显然，对于 16 个模型来说，仅依靠人工调参无法遍历整个权重参数搜索空间。于是我们想到了贝叶斯调参。我们使用贝叶斯调参搜索出了一些权重参数，但是相较我们手动调整的参数提升并不是很大。

于是**我们提出了一种新的随机参数搜索方案**，如下图所示。我们首先给出权重参数 U 和随机搜索参数 P，然后使用 U 获得了结果文件 Y。基于 U 和 P 通过随机搜索函数获得了新的参数 U’，基于 U’ 获得了新的结果文件 Y’，比较 Y 和 Y’ 的分值情况，我们选择保留最好结果所对应的权重参数。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgk7141ncvsuOicTIJ05306lESOhZGsxVx5ZPn3kURD3yQVkicd7lnE38OMlMWwm1NPicTAABX0CUSl0w/640?wx_fmt=jpeg)

我们最后选用在验证集上表现最好的参数，使用在了测试集上。

下图为最终的结果从各个模型中抽取的数量分布，从图中可以看到 valid，testA，testB 抽取的分布是大致一样的，这也证明了我们 rerank 方法是稳定且有效的。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgk7141ncvsuOicTIJ05306lEWA48AYDpTOqsSdNsnmNZAW9N0DMkRwbCxaXf9MVjW8UBq75DiaIxUrw/640?wx_fmt=jpeg)

从图中我们还发现主要从 3 个模型中进行了抽取，分别是基于 Character 级别的，基于 context，基于 SentencePiece 和 finetune 的。

从抽取分布图可以看出，从 finetune 的模型中抽取的数据并没有想象的那么多，对此我们进行了另外的尝试。我们利用投票机制，首先使用 finetune 的模型进行投票，将 finetune 模型中大部分相同的语句直接抽取出来作为最终的结果，剩余的结果依旧使用随机参数搜索方案进行抽取。

此外除却上面的 beam_score * weights 方案，我们还尝试使用语言模型对翻译的句子进行打分，然后选取分值最高的句子。但是该方法效果略差于前两者，所以最终我们选择使用第一种方案，即按照 beam_score * weights 作为最终的排序依据。

# 结语

金山集团 AI Lab 组建只有不到两年，是一只年轻的队伍，我们会持续在机器翻译等领域深入研究，希望对 AI 业界有所贡献。

# 参考文献

[1] Sergey Edunov, Myle Ott, Michael Auli, David Grangier. Understanding Back-Translation at Scale. EMNLP 2018.
[2] Peter Shaw, Jakob Uszkoreit, Ashish Vaswani. Self-Attention with Relative Position Representations. NAACL 2018.
[3] Jiacheng Zhang, Huanbo Luan, Maosong Sun, FeiFei Zhai, Jingfang Xu, Min Zhang, Yang Liu. Improving the Transformer Translation Model with Document-Level Context. EMNLP 2018.

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多往期内容：**

[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)
图神经网络综述：模型与应用
近期值得读的10篇GAN进展论文

[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)
[近期知识图谱顶会论文推荐，你都读过哪几篇？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493627&idx=1&sn=33e2f7c787fa9f14cef581f10b7dd2f7&chksm=96ea387ba19db16dc97620e28e6a7c8605b396b53f21e3eff6cf9553762a1dbc5233c580cc53&scene=21#wechat_redirect)
[TensorSpace：超酷炫3D神经网络可视化框架](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492746&idx=1&sn=921d7315a973b85dd4e802cb5fd456fb&chksm=96ea3b0aa19db21c48841ddcee38592a3c086ae8fa1a9893cf46ff974f0f38fb350bcd528265&scene=21#wechat_redirect)
[深度长文：NLP的巨人肩膀（上）](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493520&idx=1&sn=2b04c009ef75291ef3d19e8fe673aa36&chksm=96ea3810a19db10621e7a661974c796e8adeffc31625a769f8db1d87ba803cd58a30d40ad7ce&scene=21#wechat_redirect)
[NLP的巨人肩膀（下）：从CoVe到BERT](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493731&idx=1&sn=51206e4ca3983548436d889590ab5347&chksm=96ea37e3a19dbef5b6db3143eb9df822915126d3d8f61fe73ddb9f8fa329d568ec79a662acb1&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。

📝**来稿标准：**
• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向）
• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接
• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志

**📬 投稿邮箱：**
• 投稿邮箱：hr@paperweekly.site
• 所有文章配图，请单独在附件中发送
• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通



🔍

现在，在**「知乎」**也能找到我们了
进入知乎首页搜索**「PaperWeekly」**
点击**「关注」**订阅我们的专栏吧


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 获取最新论文推荐



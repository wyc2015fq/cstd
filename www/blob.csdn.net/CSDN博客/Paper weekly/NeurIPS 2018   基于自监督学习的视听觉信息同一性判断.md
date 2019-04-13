
# NeurIPS 2018 | 基于自监督学习的视听觉信息同一性判断 - Paper weekly - CSDN博客


2018年12月13日 12:36:38[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：104


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnC9iaic8hDbiadLafh7TtCZS6icEYddVmMqZBksDV7cQkKmAu95h53FxyibqmZOS1yQgHibJT0WYD2s1Zw/640)
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**124**篇文章
作者丨武广
学校丨合肥工业大学硕士生
研究方向丨图像生成

视觉和听觉存在着紧密的关联，同时空下视觉和听觉不仅在语义上存在着一致性，在时序上也是对齐的。失聪患者可以利用视觉信息做出判断，盲人也可以利用听觉信息做出判断，而一般正常人对事物的决策往往是结合视觉和听觉协同完成的。

达特茅斯学院和 Facebook 联合发表于NeurIPS 2018 的这篇文章正是**通过对视觉和听觉信息做同一性判断，在整体上优化视觉特征和听觉特征提取网络，在独立模态下也提高了各自的任务准确率。**不同于我们之前说的*Look, Listen and Learn*[1]，这篇论文不仅仅在语义上判断视觉和听觉的一致性，还在时序上做了严格对齐判断。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jY8ELnaMTdnrg4HyrCtNTu8qLs8ZD3ibu5MccntH14oLoRZaHdIGYibDcA/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYbZ0WaT3hJavU5VJGdyDYxW79HdNAfv1qFXbRp16DGPWDAObIQKHLVw/640)

# 论文引入

日常休闲娱乐很多人喜欢看电影，有时看电影的过程中会出现画面和音频对不上的情况，这会大大降低观影体验。为什么我们会察觉到画面和音频对不上呢？

这就是人类潜意识里已经建立了视觉和听觉上对应的关系，一旦客观现象中视觉和听觉信息对应不上，我们立马就会发现哪里出现了问题。人类不仅仅可以察觉画面和音频对应不上，结合已有的知识甚至可以推断是画面延迟了还是音频延迟了。

目前机器学习大部分还是停留在单一模态下信息的分析和学习，比如计算机视觉是一个大的研究方向，音频分析和处理又是一个方向。然而，机器如果想更进一步的智能化，必须要像人类一样，利用多模态去分析和学习，结合不同模态下的信息和联系做出判断和决策。

已经有越来越多的研究者关注到了多模态信息的学习，跨模态检索、迁移学习、多模态信息联合决策、跨模态转换等。视觉和听觉这两个模态，本身就是严格关联的，只要物体运动了，视觉上的变化势必会带来听觉上声音的产生，**如何结合视觉和听觉信息去提高视觉任务和听觉任务的处理，正是我们今天要看的这篇论文的核心。**

如何去结合视觉和听觉信息呢？论文采用的方式是“视听觉时间同步”英文缩写为 AVTS (Audio-Visual Temporal Synchronization)，就是在语义和时序上对视觉和听觉信息做对齐判断，如果视觉信息和听觉信息不仅在语义上是关联的（视频和声音是可以对上的）而且在时序上也是对齐的（视频和声音不存在延迟，是对齐关系的）就判断为同步信息，否则认为是非同步。优化决策结果，则会提高视觉和听觉特征提取网络，特征提取好了自然在独立的任务上可以取得改善。

笔者在之前的论文解读中对*Look, Listen and Learn*一文简称为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYFTdrhp7BFKWValDPeYeicbK7OVnQF8vo6nRvk8mVSLx73ia9sIk2JCDw/640)做过分析[2]，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYFTdrhp7BFKWValDPeYeicbK7OVnQF8vo6nRvk8mVSLx73ia9sIk2JCDw/640)也是对视觉和听觉信息做关联性判断，但是判断视觉和听觉关联上仅仅是通过语义上是否关联判断的，而论文 AVTS 则是在此基础上考虑到视频的时序信息，进一步严格了视觉和听觉的同步性判断。

利用视频和音频之间的相关性作为特征学习的方法，在训练过程中是不引入人为标签的，拿来视频和音频只需要知道是否是同步的不需要任何其它的标签就可以优化整体网络，这种方式符合自监督学习方法，所以论文的标题特意强调文章是在自监督下完成同步性判断的。

这对于处理视频这样的大数据集是可观的，一旦利用 AVTS 自监督方式预训练好特征提取网络可以在微调阶段发挥出更好的效果的同时，不引入额外的标注开销。

**总结一下 AVTS 的优势：**

视觉听觉在语义和时序同步性判断；

视听觉相关性判断，实现了自监督学习特征提取；

预训练 AVTS 模型在视觉信息和听觉信息独立任务上取得了提高。


# AVTS模型
VTS 模型是对视觉信息和听觉信息在语义和时序上同步性的判断，判断结果是二分类问题，要么同步要么不同步，我们先看一下模型框架：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYB4AmhSQMibTlOKZbj9cafbicUOiaq3TtWVM7AzYhB82cCiaKkdrChM7jLw/640)

由上图 (a) 所示，AVTS 模型采取的是双流结构，一路是视频特征提取网络，一路是音频特征提取网络，对提取得到的特征利用对比度损失进行优化。

整体上看 AVTS 还是很容易理解的，我们要强调一下具体的实现。

我们先从模型优化的**训练集**说起。 整体训练集定义为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jY5f4x1wHMzSUCA5vNR44nlSHZBqqv8rGcNzQ66F84HbYcicg6ibGA1l9Q/640)由 N 个标记的音频视频对组成。其中 a(n) 表示音频第 n 个样本，v(n) 表示视频第 n 个样本（视频由连续帧组成），标签 y(n)∈{0,1} 表示视频和音频是否同步，0 为不同步，1 为同步。

训练集选择同一视频下时序对应的视频和音频为同步的正例，对于负例，定义不同视频下视频和音频为简单负例，同一视频下时序不同步的为硬（“hard”）负例，硬负例下时序相差太远的定义为超硬负例，我们由下图可以进一步理解正负例定义原则。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jY7UfgiaRYszp684YicibmfXia8RTNIK54RJtjYPAEDJX51woewLVrMWnKZg/640)

优化 AVTS 模型中，论文作者一开始直接采用交叉熵损失进行优化，发现从头开始学习时很难在这种损失下实现模态间的融合，通过最小化**对比度损失**可以获得更一致和稳健的优化，在正对上产生小距离，在负对上产生更大距离：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYcoXUPnJiaGCpqyA9rHcaaWkuxnlf7cE6ic4kI9dS98nnjGJsFFDAAXrA/640)

其中![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jY7icziaNaDnqj3bXpYTtDce76IeibLDmmusfaibJibCtibj5XZux8mVvlnF0A/640)为视频提取的特征表示，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYMpjjVNu4fFtpzSVKOwvGG7ndDrWNIH7iaIem0zUa7AluK3iaBwl0Dpqg/640)为音频提取的特征表示，对于标签 y(n)=1 时，此时最小化对比度损失E时需要![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jY7icziaNaDnqj3bXpYTtDce76IeibLDmmusfaibJibCtibj5XZux8mVvlnF0A/640)与![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYMpjjVNu4fFtpzSVKOwvGG7ndDrWNIH7iaIem0zUa7AluK3iaBwl0Dpqg/640)尽可能相近，也就是希望同步的视频特征和音频特征尽量相近。

对于非同步的视频-音频对，即 y(n)=0 时，对应到公式的后一项，只有当![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jY7icziaNaDnqj3bXpYTtDce76IeibLDmmusfaibJibCtibj5XZux8mVvlnF0A/640)与![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYMpjjVNu4fFtpzSVKOwvGG7ndDrWNIH7iaIem0zUa7AluK3iaBwl0Dpqg/640)距离越远的时候，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYPJ3fHlyCBiaVwScLiboia7GYWhbicsWNqGcqjHPDh85DRmeJB8wXCFIPjQ/640)才会比 0 要小，此时 max 达到最佳值 0，其中 η 为边际超参数。

对于视频特征提取网络 (b)，文章采用 2D 和 3D 卷积网络结合实现，我们简单分析一下 3D 卷积网络，对于 (b) 图中对应的是前 2 个卷积块，后 3 个卷积块为 2D 卷积网络，最后一层为全连接层。

3D 卷积网络下视频输入是包含帧的，这里输入的视频帧为 3，长宽为 7 × 7，通道数为 3，batchsize 为 64。论文解释为在特征提取的后半部分将不再依靠时间轴，这时候可以直接利用 2D 卷积网络，论文称这种方法为混合卷积架构（MC），实验也验证了混合架构性能要好些。

对于音频信息，先要对音频信息提取对应的声谱图然后再对其利用2D卷积网络做特征提取，网络结构为 (c) 图展示。

# 课程方式训练

论文在训练模型的时候发现，如果一开始对负例的选择上简单负例和硬负例按 3:1 训练时，训练效果很一般。论文认为一开始让模型去区分硬负例有些太难了，文章采用循序渐进增进难度的方式。

论文实验发现在前 50 个 epoch 下负例只选择简单负例，在 51-90 epoch 下简单负例和硬负例按 3:1 训练时，模型效果最佳。这个也符合人类的学习方式，一上来就做难题不仅打击自信，基础也不能打扎实，只有掌握了充分的基础知识后，再做些难题才能锦上添花。

**论文对比了设置课程的效果：**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jY058vcalUHc5OvKCbAUFNGRUjY3S4UlfQQPT3HV2uK7gnvTHVwhT6Qg/640)

# 实验

训练上边际超参 η 为 0.99，训练在四块 GPU 机器上完成，每个 GPU 有一个小批量的 16 个样本。每次损失值在超过 5 个时期内没有减少时，学习率将缩放 0.1。

在验证视觉信号和听觉信号同步性问题，论文做了与![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYFTdrhp7BFKWValDPeYeicbK7OVnQF8vo6nRvk8mVSLx73ia9sIk2JCDw/640)的对比：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYTpFqKGFQs7PucAhWSYJf5Mlvf7IiaWZE98nFr5k6KsOTv6MibVRAibYbA/640)

在评估**视觉特征性能**时，正如预期的那样，使用动作类标签对 Kinetics 数据集进行预训练可以提高 UCF101 和 HDMB51 的准确度。但是，这会占用 500K 视频剪辑上手动标记的巨大成本。相反，AVTS 预训练是自监督的，因此它可以应用于更大的数据集而无需额外的人工成本。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jY1AnhpTU27fwb5SN0QZ3ibliam7icQw1CCl5hS2vmtgbibjhrmobIB1XbGw/640)

在评估**听觉特征性能**时，直接在音频特征提取的 conv_5 AVTS 功能上训练多类一对一线性 SVM，以对音频事件进行分类。通过对样本中的分数求平均来计算每个音频样本的分类分数，然后预测具有较高分数的类。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglHoYPhhcYLvgtqg2ZDA4jYVVgLe4O1LH8JyLNpvnAuIAibhKib17R3japibSexaDM3Bsr7oUGXC7TyA/640)

可以看到，AVTS 在音频分类任务上取得了比人工稍好的效果。更多实验，可以进一步阅读原文。

# 总结

视听觉时间同步（AVTS）的自监督机制可用于学习音频和视觉领域的模型，通过视觉和听觉上的相关性实现视觉和听觉上性能的提高，视觉和听觉上的关联，对于视觉下运动分析可以很好的结合听觉上的特征信息进一步提高判别和识别的准确。可以想象。视听觉结合对于提高分类和识别任务上还有进一步提升空间。

# 参考文献

[1] Relja Arandjelović and Andrew Zisserman. ook, Listen and Learn. In ICCV 2017.
[2] www.paperweekly.site/papers/notes/594
**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

**点击标题查看更多论文解读：**

[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)
[深度](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)[解读DeepMind新作：史上最强GAN图像生成器](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)
[本周有哪些值得读的AI论文？进来告诉你答案](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493220&idx=1&sn=b4c88ecfb42d081935c6deb548c681af&chksm=96ea39e4a19db0f26aede4b9e1e57208e97edfc50f736f0d4aaedaaf363939b774ac78f9ad35&scene=21#wechat_redirect)
[TensorSpace：超酷炫3D神经网络可视化框架](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492746&idx=1&sn=921d7315a973b85dd4e802cb5fd456fb&chksm=96ea3b0aa19db21c48841ddcee38592a3c086ae8fa1a9893cf46ff974f0f38fb350bcd528265&scene=21#wechat_redirect)
深度长文：NLP的巨人肩膀（上）



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

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)
▽ 点击 |阅读原文| 下载论文



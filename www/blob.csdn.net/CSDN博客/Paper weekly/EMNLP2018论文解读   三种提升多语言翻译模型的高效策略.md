
# EMNLP2018论文解读 | 三种提升多语言翻译模型的高效策略 - Paper weekly - CSDN博客


2018年11月28日 12:05:03[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：66


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

本文（**《三种提升一对多多语言翻译策略》**）是搜狗和中科院自动化所合作发表在 EMNLP 2018 上的工作。搜狗翻译目前采用业界领先的神经网络机器翻译框架支持 60 种以上不同语言之间的互译，考虑到当前支持单一语言对的翻译模型需要较大规模存储和计算资源，而支持一对多多语言翻译模型虽然能较好的解决资源占用却面临着翻译质量不高的问题，所以搜狗翻译对实现和优化多语言翻译模型存在客观需要。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSbKNGYSmFMIPkTRDCjsMEDYEicShf3KYM9y4Y3bz6l1LstunT574741A/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSOFmHk0cqaFcGPgXBkegbCrr9libg2kAhDiaEfQKDU8fOSTNapYG1d7gw/640)

# 论文方法

当前主流的一对多的多语言翻译模型在该研究之前还存在一定的不合理性。 这种不合理性主要体现在不同语言都包含其特有的语义信息和句法结构，用同一套分布式向量表征会导致模型存在相应的偏差。


鉴于此，**该工作提出了三种策略，改进了多语言翻译框架来提升多语言神经翻译模型的翻译质量。**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSPzibsjhDr1HOafVAI5dUfABjJEia7R2UZLX14aOSwAHzfWwgecFRGG2g/640)
▲图1. “一到多”多语言翻译示意图

该策略首先**将目标端句子初始位置加上表征翻译方向的标签，指示翻译生成的方向**。其次，考虑对于不同的语言而言，结构具有不一致性，这里**针对不同的语言尝试使用不同的位置向量来表征相应的时序信息**。该工作对比了绝对位置和相对位置两种不同的建模方式。

最后，由于神经网络结构的隐层向量包含着语义信息和句法结构，区别于使用同一套隐层单元表示不同的语言，**该策略采用了共享和特有的方式**，其中共享一部分隐层向量表征不同语言的共性，而其他部分隐层向量用来表示不同语言特有的属性。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSRY2icxJ8D8p5ichMUWqnOLvENhVwPwtkSa8aXs8a21ps9sp626rPYgDg/640)
▲图2. 解码器网络隐层状态表示图。其中，蓝色表示共享的单元，黄色、绿色、红色分别表示不同具备不同语言特征的隐层单元。

# 实验结果

该工作是针对当前多语言翻译模型的表征问题来提升多语言翻译性能的一次尝试，将不同语言特有属性融合到翻译模型当中，取得了显著的提升。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSibyOyxLnicGnV9427LiaMXhnMPDqiaE8iamZ8ibVzLFmxCaP4o1gyHgg2Xtw/640)
▲表1. 我们的方法在中英日和英德法两个翻译任务上的表现。其中，Indiv表示仅包含双语句对的模型翻译结果。O2M是我们复现的谷歌多语言翻译模型，作为基线系统。①， ②和 ③分别代表我们提出的三种不同的策略。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzmiaRq4FWe1NicrVDxu9ZzSOnestZichvXxK2VicDM6B9zJw6VMvtl6y2nr91WVC9j8DE4ktqG74T2Q/640)
▲图3. 不同共享隐层单元数的比较

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多往期内容：**

[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)
[深度](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)[解读DeepMind新作：史上最强GAN图像生成器](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)
[这16篇最新论文，帮你轻松积攒知识点](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492912&idx=1&sn=e223b0bf570148493313ea8780cef2fc&chksm=96ea3ab0a19db3a6eb87b8c8d6cb41d1a4ae0d85b5fd7b616baa970234124c320fda1cdcc7d9&scene=21#wechat_redirect)
[TensorSpace：超酷炫3D神经网络可视化框架](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492746&idx=1&sn=921d7315a973b85dd4e802cb5fd456fb&chksm=96ea3b0aa19db21c48841ddcee38592a3c086ae8fa1a9893cf46ff974f0f38fb350bcd528265&scene=21#wechat_redirect)
NIPS 2018：基于条件对抗网络的领域自适应方法




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
▽ 点击 |阅读原文| 下载论文



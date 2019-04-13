
# 基于知识图谱的问答系统入门—NLPCC2016KBQA数据集 - Paper weekly - CSDN博客


2019年01月08日 11:45:17[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：710


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨郭雅志
学校丨北京化工大学
研究方向丨NLP、知识图谱、对话/问答系统

本人认为学习一样东西，直接上手跑实验是最有效提升的方法之一。看了那么多的理论介绍文章，抽象的说辞，最后还是似懂非懂。所以，直接上手数据集跑实验，与理论结合，能有更深刻的了解。同时也记录下学习 KBQA 的过程，也希望对同入门的同学有些帮助。

# 引言

本人最近在做 KBQA 相关的的工作，有些大致的了解和想法后希望能够在实验中学习更多。

**目前研究的 KBQA 暂时专注在简单知识库问答上：**即根据一个问题，抽出一条三元组，生成 Sparql 语句，进而进行知识图谱查询返回答案。

最终选择了 NLPCC2016KBQA 数据集，基线模型采用 BERT。

# 数据集介绍

NLPCC 全称自然语言处理与中文计算会议（The Conference on Natural Language Processing and Chinese Computing）,它是由中国计算机学会（CCF）主办的 CCF 中文信息技术专业委员会年度学术会议，专注于自然语言处理及中文计算领域的学术和应用创新。

此次使用的数据集来自**NLPCC ICCPOL 2016 KBQA 任务集**，其包含 14,609 个问答对的训练集和包含 9870 个问答对的测试集。并提供一个知识库，包含 6,502,738 个实体、 587,875 个属性以及 43,063,796 个三元组。

知识库文件中每行存储一个事实（fact），即三元组 ( 实体、属性、属性值) 。各文件统计如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDR2hFDB0nR4746LI0egmGSSQ9hk6ibAB4L6IBXk2ibHyV9xTzVD9K6ZTQ/640?wx_fmt=png)

知识库样例如下所示：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictD8o7mkeSYLsjaC7Fe8blQpiaRndcRTwpEQIjwEuXTP7S6TvILTJ5Frug/640?wx_fmt=png)

原数据中本只有问答对（question-answer），并无标注三元组（triple)，本人所用问答对数据来自该比赛第一名的预处理：

http://github.com/huangxiangzhou/NLPCC2016KBQA

构造 Triple 的方法为从知识库中反向查找答案，根据问题过滤实体，最终筛选得到，也会存在少量噪音数据。该 Triple 之后用于构建实体识别和属性选择等任务的数据集。

问答对样例如下所示：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDibDjqj8p4WjvctC1UMgmFvczLSotUfnsv515wlTflN401JgJfhTyUicQ/640?wx_fmt=png)

# 数据集本身存在的问题

**知识库实体间的歧义**

以“贝拉克·奥巴马”为例，涉及该实体的问答对如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDGMlq4WTicMfJMumAxFXjCeyPSLy4CxgfEpgSfNMfr0JPYkW7JDiapvOg/640?wx_fmt=png)

在知识库中查询包含该实体的三元组，结果如下（部分）：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDyJtKpx0hG010JwYO77GwjsKWicuwak6mqjQpMwENgzBmNAjj9qEdhog/640?wx_fmt=png)

首先，知识库中存在“贝拉克·奥巴马”的多条实体，有可能是多数据来源的融合或其他原因，从而并不能完全保证信息的对齐。我们查看“妻子”这一属性，发现有的是“米歇尔·拉沃恩·奥巴马”有的是“米歇尔·奥巴马”，而我们问答对中给出的答案是“米歇尔·奥巴马”。因此当我们的模型检索到正确三元组时：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDS2GfricjqiceDEuxMuok0ohiccibhYJdtqYh9sBvkEmxWmrMibibzY1ySAtw/640?wx_fmt=png)

虽然在实体和属性都映射正确的情况下，最终答案仍可能被判定为错误。

**问题中的实体歧义**

以“博士来拜”为例，涉及该实体的问答对如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDodkNocAoZrhEC7xwbT7EXvfJFMrXFOV9xdJSW3kj2Y6ylZuiaZlbVKQ/640?wx_fmt=png)

在知识库中查询包含该实体的三元组，结果如下（部分）：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDyCwPicialWFJ6CbpCNqueZ3eD6peSLnrJ6dDLAMGTc3eIq0u40rOVdcA/640?wx_fmt=png)

问句中的问题是：“博士来拜是什么年代的作品？“，涉及到”年代“这个属性，而这幅作品被不同时期的很多人创作过，我们无法从当前问句下得到要询问的是哪位艺术家的创作年代。

因此该问题的涉及的实体具有歧义性，同样的，当模型检索到我们认为的正确实体和正确属性后，依然有可能会被判定为错误答案。

**在知识库中相关实体三元组数量过多的情况下，对检索模型的效果、效率也是个挑战。**

在具有 4300W 条三元组的知识库中，同一个实体会检索出大量（几十、几百条）的相关三元组，而且在存在上述两个歧义性问题的情况下，识别的效果和效率都是很大的问题。

以上的两个问题在实体识别实验部分和属性抽取部分的影响较小，但在实体链接知识库检索最终答案三元组的部分会有较大的影响。

# 数据集预处理及实验结果

**清洗训练数据、测试数据、知识库**

过滤属性，去除‘-’，‘•’，空格等噪音符号，同时把每一行 lower() 转成小写。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictD2tN862gibDGLyHsiczZrwyxeA2Q3DHUK29Ao2pHvsYvHBwcepZ2cWBDg/640?wx_fmt=png)

分别保存为：train_clean.csv, test_clean.csv, nlpcc-iccpol-2016-clean.kbqa.kb。

**构造开发集**

原始训练集数量为 14,609，Shuffle 后抽出 2609 作为开发集，其余作为训练集，如下。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDZUUaNFzDZsW5pYXIX9d2tRWTEO2Zl7lbA6z07WqPNEB7agS3aEI8nw/640?wx_fmt=png)

分别保存为：train_clean.csv, dev_clean.csv, test_clean.csv。

**构造实体识别的训练集、开发集、测试集**

构造实体识别数据集需要根据三元组-Enitity 反向标注问题，给 Question 打标签。由于我们希望从单问题中抽取单个实体，并未采用 BIO 的标注，直接采用 0/1 标注法，即 0 表示非实体，1 表示实体。

同时需要确保其实体完整出现在问题中，对于未出现的样例，我们直接删除忽略。错误样例如下所示：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDE9133qN6dCZZTDEHfS8hsbKdaV6jE2ut4vW7AjX44bUSzoAyZEu2HA/640?wx_fmt=png)

过滤后的数据集信息如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDGx9A8YicNBdkrSKPFAXEFIbKOvKTyU82iaCgBuAL1miblHIvPyjTKyEjg/640?wx_fmt=png)

过滤后的数据集样例如下所示：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDQyezYb7aaZzBsqPLH4o4icewhly49TE39yQZOKDK7ZmyM4CT3eTNbsg/640?wx_fmt=png)

分别保存为：entity_train.csv, entity_dev.csv, entity_test.csv。

基于 BERT+BiLSTM+CRF 模型的实验结果如下所示，其中 accuracy 是从 9556 个问句识别出完全匹配实体的准确率。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDUibjqRvjOb9icic3WNIQMNuiawCHXSjaxXFTrS9Xib5qsbdDKOofhIgfLhg/640?wx_fmt=png)

未完全匹配的实体样例如下，部分是识别错误，部分是同义词，部分是噪音问题。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDIXEo1MQoF2zjCd4iaIlBsPtkTtJn7OGSEnXgNsgqLFPC80icj4rVep3A/640?wx_fmt=png)

**构造属性抽取的训练集、开发集、测试集**

1. 构造测试集的整体属性集合，提取+去重，获得 4373 个属性 RelationList；

2. 一个 sample 由“问题+属性+Label”构成，原始数据中的属性值置为 1；

3. 从 RelationList 中随机抽取五个属性作为 Negative Samples。

数据集大小如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDibuQcRjBkNibbtx1mwdO0TVstTPBUMZhWTnrUUY5LDXs6RYu8MZNNKFQ/640?wx_fmt=png)

数据集 samples 如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDmDcz6FWHeO0uib58J3LpvMtAWDQQWcNKXQVvyWmdo3YVTgfUr82tW9A/640?wx_fmt=png)

分别保存为：relation_train.csv, relation_dev.csv, relation_test.csv。

之前构造的数据进行训练，在这个测试集上测试的结果。基于BERT的训练结果如下所示，其中 accuracy 是真正的准确率。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDBLOPKCBVgfs3QAnIZ7HZ4M5tvU9stHsSo7F9V0wV3JaYgDyWdcyicSw/640?wx_fmt=png)

模型没有识别出的测试样例如下，可以看出缺少深度语义匹配的能力。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk5B3T3ian34U81yYvRCzictDoPNJMVE4PHqAgLB5Kxrz1sSGSvBGp1JsRPBX1J0fNicTNI5jDQEGfbA/640?wx_fmt=png)

# 结束语

之后也会把相关代码和预处理数据开源到我的 Github 上：

https://github.com/supergyz

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多往期内容：**

[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)

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



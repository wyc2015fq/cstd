# 当我们谈论「Chatbot」时，我们在读什么？ | 论文集精选 #05 - Paper weekly - CSDN博客





2017年12月08日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：866









PaperWeekly 是一个 AI 学术分享社区。这里聚集了大批一线 AI 学者，他们用精炼妙语推荐各自发现的优质论文。




点击本文底部的「**阅读原文**」即刻加入社区，创建属于你的论文集。
这里是第 5 期论文集精选。
本期我们带来的是由 PaperWeekly 社区用户**@cb** 创建的
**Chatbot 专题论文集**，包含 20 篇对话系统、对话生成、自然语言理解等方向相关论文。

我们从中精选了 10 篇优质文章进行展示，如果有合你心意的论文，复制链接到浏览器即可查看原文。**你也可以点击「阅读原文」，一键收藏此论文集。**

![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQbNLW4F9aG2fHaG9WzURiamZENvQWW2PMw2Sk7vBUvzX2GzOcHRZXoFQ/?wxfrom=5&wx_lazy=1)




**Neural Response Generation with Dynamic Vocabularies**

**@zhangjun 推荐**

#Dialogue Generation

本文针对 seq2seq 做文本生成任务 decoding 阶段 vocabulary 过大导致计算性能差的问题，提出了一种 dynamic vocabulary 的 seq2seq 模型，本文已被AAAI2018 录用。

论文链接

http://www.paperweekly.site/papers/1267

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQePoAQyBbJvkZycHIIyiasxPGbiaq5uBGeF4XvkgrNxWic8aYHhF2fIRuQ/?)
**BBQ-Networks: Efficient****Exploration in Deep Reinforcement Learning for Task-Oriented Dialogue Systems**

****@zhangjun 推荐****

#Dialog Systems

AAAI2018 录用文章，将深度强化学习用于对话系统。BBQ network 这个名字很有意思，工作来自微软研究院和 CMU。

论文链接

http://www.paperweekly.site/papers/1138







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQFnRbsia2PAHzrv7JtqytCjHgg96a4cv3RIqXneNTVs1MtWcJ19d1gHA/?)

**Integrating User and Agent Models: A Deep Task-Oriented Dialogue System**

****@zhangjun 推荐****

#Dialogue Agent

港科大杨强老师组最新工作。

论文链接

http://www.paperweekly.site/papers/1109







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQOuXYDRXKvxxu0ZIccK0wmicjJcQ1JPmyf7MWxFfDu9WI5JJXaclFxKg/?)
**Flexible End-to-End Dialogue System for Knowledge Grounded Conversation**

******@lukepan 推荐******

#Chatbot

该论⽂所引⼊的知识是以 tuple 的形式储存在 KB 中的。 knowledge 的引⼊⽅式如下：Candidate Facts Retriever 从 input 中提取 entity，然后在 KB 中进⾏ query，将通过 relation 寻找到的 objects 和 subjects 作为 Candidate Facts 存储为⼀个集合。 

在 Reply Decoding 流程中，为了决定该使⽤哪些 facts，提出了 dynamic knowledge enquirer，⽤以根据 local contexts at various positions within a response 来 rank 并选择集合中哪些 facts 应该被使⽤。

论文链接

http://www.paperweekly.site/papers/891







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQul73yBz8Z7kBgyLLEn6kBxrsnBbxvfK97GGcRAgDiba3lKlkax3UI6A/?)
**Affective Neural Response Generation**

****@Jeffreygao 推荐****

#Dialog Systems

在对话系统中来建模情感，提出了包含情感的词向量。这篇论文的模型以 seq2seq 为背景，主要在三个点上做改进。（1）在 embedding 上加情感信息（2）改进 loss function（3）beam search 时考虑情感。

论文链接

http://www.paperweekly.site/papers/1043






![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQquC1nibV3stcuVN7lcbVI1AiaadVdWPNuPUCgzSpAaILW0PUcMO1ymxA/?)
**A Deep Reinforcement Learning Chatbot**

****@xwzhong 推荐****

#Chatbot

蒙特利尔算法研究实验室（MILA）为参与亚马逊 Alexa 大奖赛而开发的深度强化学习聊天机器人。

MILABOT 能够与人类就流行的闲聊话题进行语音和文本交流。该系统包括一系列自然语言生成和检索模型，如模板模型、词袋模型、序列到序列神经网络和隐变量神经网络模型。

通过将强化学习应用到众包数据和真实用户互动中进行训练，该系统学习从自身包含的一系列模型中选择合适的模型作为响应。

真实用户使用 A/B 测试对该系统进行评估，其性能大大优于竞争系统。由于其机器学习架构，该系统的性能在额外数据的帮助下还有可能继续提升。

论文链接

http://www.paperweekly.site/papers/559







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQfL2e6YAjlm3ictOAFoEkL7e4f4UWv208icoD1ia3UadqYxAxSECyt9sFg/?)
**An End-to-End Trainable Neural Network Model with Belief Tracking for Task-Oriented Dialog**

******@valyriansteel 推荐******

#Natural Language Understanding

最近发表在语音顶会 Interspeech 上的一篇关于 end-to-end 任务型对话系统的文章。

提出的模型打破了传统任务型对话系统的 pipeline 模式，用统一的分层 RNN（Hierarchical RNN）来做语义理解（NLU），状态追踪（DST） 和对话管理（DM）。

模型还有一个专门的模块来做数据库/知识库的连接，能有效地把查询结果加入到最后生成的对话回复中。实验用的是基于 DSTC2 的对话数据，在对话状态追踪和回复预测上都取得了不错的效果。

论文链接

http://www.paperweekly.site/papers/395







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQna4MPT1gXlaalDB3RbibR6uz1hibKegCiaicdSvRm3yTf9tdAyj9EkrD3A/?)
**Generative Encoder-Decoder Models for Task-Oriented Spoken Dialog Systems with Chatting Capability**

******@paperweekly 推荐******

#Dialog Systems

本文给出了一个特定领域对话系统的端到端解决方案，包括用 entity indexing 来解决对话生成时的 OOV 问题，也包括从外部数据库中获取到知识并融合到生成的对话中，对工程实现有一定的启发。本文已被 SIGIDIAL 2017 录用。

论文链接

http://www.paperweekly.site/papers/372







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQPvVKhHvicibS67HQaicib0XFWLloqyHm2yXr0Kar2t0rfgUd65mfGAgerA/?)
**Personalization in Goal-Oriented Dialog**

****@paperweekly 推荐****

#Dialog Systems

本文给出了一个个性化对话数据集和 baseline，这里的个性化包括了 speaker 的 profile，最近有几篇都是做更加聪明的 chatbot，比如带有情绪，带有感情色彩，带有一定个性。

论文链接

http://www.paperweekly.site/papers/373







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnffKwgOrLx989SaWg7Vt9HJp0DOhuREVLMGsYXc6tM0jmS5wKkk9TP9tPH3KpNdwpicQKJ7URVYSg/?)
**Key-Value Retrieval Networks for Task-Oriented Dialogue**

****@tonya 推荐****

#Dialog Systems

本文的具体研究场景是车载智能助理，作者提出了一种基于知识库的端到端解决方案，车的 response 信息检索自形如 key-value 知识库中。文章会在后面开放出一个数据集出来供大家研究。本文工作来自斯坦福 Christopher D. Manning 组。

论文链接

http://www.paperweekly.site/papers/662






**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文」即刻加入社区！**




我是彩蛋 



**解锁新功能：热门职位推荐！**




PaperWeekly小程序升级啦




**今日arXiv√猜你喜欢√**热门职位****√****




找全职找实习都不是问题



** 解锁方式 **

1. 识别下方二维码打开小程序

2. 用PaperWeekly社区账号进行登陆

3. 登陆后即可解锁所有功能




** 职位发布 **

请添加小助手微信（**pwbot01**）进行咨询




**长按识别二维码，使用小程序**

*点击阅读原文即可注册




![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)





**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入
 PaperWeekly 的交流群里。





![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/?)

▽ 点击 | 阅读原文|
 收藏论文集





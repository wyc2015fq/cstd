
# 腾讯AI Lab涂兆鹏：如何提升神经网络翻译的忠实度 | PhD Talk \#22 - Paper weekly - CSDN博客


2018年01月02日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：880


![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/ePTzepwoNWMkNKMB8c7hfib3laa9aKL9BnTibmERsTIqElyVZM4oMSy3qB10NFn9WXWQdHXXhCiaXy4DMRoRSicyIA/640?wxfrom=5&wx_lazy=1)「PhD Talk」是 PaperWeekly 的学术直播间，旨在帮助更多的青年学者宣传其最新科研成果。我们一直认为，单向地输出知识并不是一个最好的方式，而有效地反馈和交流可能会让知识的传播更加有意义，从而产生更大的价值。**这是第****22****期「PhD Talk」**
机器翻译是自然语言处理的经典任务之一，涉及到自然语言处理的两个基本问题：语言理解和语言生成。这两个问题的建模直接对应译文的两个评价指标：忠实度（是否表达原文的完整意思）和流利度（译文是否流畅）。

近几年来，神经网络机器翻译取得了巨大进展，成为了主流模型。神经网络由于能缓解数据稀疏性及捕获词语之间的关联，生成的译文流利度高，这是过去二十余年上一代统计机器翻译一直以来的难点。**但由于神经网络目前来说仍然是个黑盒子，无法保证原文的语义完整传递到目标端，导致经常出现漏翻、错翻等忠实度问题。**

**本次报告主要讲述过去一年我们在提高神经网络翻译忠实度方向上的系列研究工作**，从利用句法结构、扩大翻译粒度、引入篇章信息、忠实度学习方面加强模型对原文的理解。


**嘉宾介绍**


![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglDCXqSgXQylJ3JoJCwfU97sByyS9DRPvOcrAJmNzuoGvSk8hOvUhdffFicw5HIvDfAE0xo0k2Iqkg/?wxfrom=5&wx_lazy=1)

**涂兆鹏**
**腾讯 AI Lab 高级研究员**

涂兆鹏是腾讯 AI Lab 的高级研究员，研究主要集中在机器翻译和深度学习方面，目前已经在自然语言处理和人工智能领域发表二十余篇顶级会议和期刊论文，包括 ACL, EMNLP, TACL, NAACL, AAAI 等。

在加入腾讯之前，他分别在 2008 年和 2013 年于北航和中科院计算所获得计算机科学学士和博士学位，于 2014 年于美国加州大学戴维斯分校完成博士后研究。2014 年到 2017 年他在华为香港诺亚方舟实验室任研究员。

PhD Talk

**如何提升神经网络翻译的忠实度**
腾讯 AI Lab 高级研究员涂兆鹏

**内容分享√在线Q&A√**

活动形式：PPT直播
**活动时间**
1 月 4 日（周四）20:00-21:00
45 min 分享 + 15 min Q&A

**长按识别二维码，进入直播间**
*或使用斗鱼App搜索「1743775」

![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkag0FBKl0PqKXoUibtZn6It2LvwIKDnvtfZC3ujQC4icOdqZABgS62lbXUf39l5ibSK8znUhwrWwblQ/?)


**往期回顾**


[基于生成模型的事件流研究 + NIPS 2017 论文解读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486772&idx=2&sn=e4aa1601872de283e9898e007ca8fd39&chksm=96e9d2b4a19e5ba2dc5f46b752a5b93d48efdcc3fa8ce5ad122a056dea049b3893b74a2585dc&scene=21#wechat_redirect)
[亚马逊高级应用科学家熊元骏：人类行为理解研究进展](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486679&idx=1&sn=e142574dd940434db051c79c905cce15&chksm=96e9d357a19e5a41ecb48d318876ad7db1c36c0f761afc63368a8fcdcc11d33057902a47289f&scene=21#wechat_redirect)
[清华大学冯珺：基于强化学习的关系抽取和文本分类](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486440&idx=2&sn=d43b6799f62337ec19f405bed1cd0138&chksm=96e9d468a19e5d7ea1cd673045eaa57dd8f5bee49074b6ef0f4fc64558e4626a0461867b216b&scene=21#wechat_redirect)
[基于双语主题模型的跨语言层次分类体系匹配](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247485671&idx=2&sn=9ae43be03e1df26a7cfb527f051d58c2&chksm=96e9d767a19e5e71171e29b285b1e7b5de07b99c061422e21c0cfd78a99eca4c1a9c603a4109&scene=21#wechat_redirect)
东南大学高桓：知识图谱表示学习
[带多分类判别器的GAN模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247485307&idx=2&sn=a72494118454b6b96b18a77f7d7490d3&chksm=96e9d8fba19e51ed901cbabf7e57f54547b512fa48aef29ae7b4b9b0b69a341098cbec18365e&scene=21#wechat_redirect)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)



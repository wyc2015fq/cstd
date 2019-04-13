
# NeurIPS 2018 | 腾讯AI Lab：可自适应于不同环境和任务的强化学习方法 - Paper weekly - CSDN博客


2018年12月04日 13:18:30[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：145


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

**本文是由南加州大学和腾讯 AI Lab 合作发表于 NeurIPS 2018 的 Spotlight 论文之一**，这项工作研究了同时在不同环境（Env）和不同任务（Task）之间迁移的问题，目的是利用稀疏的（Env, Task）组合就能学到在所有可能组合中迁移的能力。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnBD3BhAic6bZ0a2riaxvtj0M6vL9v0WPa9UNJicFsYjUELg5rJQric5kEEhXQhEG5PBcrJnicTtULPDDQ/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnBD3BhAic6bZ0a2riaxvtj0M12Kf4Ju16cnqUJpWco2wUbbP0HWCVnmNQG5IiapRTJ7pkicE1qibeAtyw/640)

**本文提出了一种新颖的部件神经网络，它描述了如何从环境和任务来组成强化学习策略的元规则。**值得注意的是，这里的主要挑战之一是**环境和任务的特征描述必须与元规则一起学习**。为此，研究者又进一步提出了新的训练方法来解锁这两种学习任务，使得最终的特征描述不仅成为环境和任务的独特签名，更成为组建策略的有效模块。

研究者在 GRIDWORLD 和 AI2-THOR 上进行了大量实验，结果表明新提出的模型可以有效地在 400 个（Env, Task）组合之间成功迁移，而模型的训练只需要这些组合的大概 40%。

# 方法简介

**传统的****[强化学习]()****在同一个环境下试图解决同一个任务**——比如 AlphaGo，我们希望把这种受限情况下的进展推广到更一般的情况：多个环境（比如不同的迷宫），多个任务（比如逃离迷宫、找到宝藏、收集金币等不同任务）。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnBD3BhAic6bZ0a2riaxvtj0MsibMBOrP5mibMchiacItiby0zL29uBsUibUfPsFic2KEVm0mib87B6WLDEiaPg/640)

如上图所示，红色代表训练阶段见到的环境与任务组合，灰色代表只在测试阶段才会见到的组合：

（a）是最简单的情况，尽管有一些组合在训练阶段没见过，但是每个环境（同理，每个任务）都在训练的时候出现过；

（b）相对复杂一些，在测试阶段出现的新的组合里面，或者任务是新的——训练时候没有见过，或者环境是新的；

（c）是最复杂的情况，在测试阶段出现的新组合里面，任务和环境都是以前没有见过的。

我们提出一个统一的框架来解决上述三个问题，**这个框架的关键在于同时学习环境和任务的描述子以及如何从这些描述子构建规则。**

我们的基本假设是规则的构建方式对于不同的（环境、任务）组合是相通的：于是，当智能体在测试阶段给定了新的（环境、任务）组合时，针对（a）它可以利用以前学习到的描述子构建相应的规则；针对（b）和（c），也就是存在新的以前没见过的环境或/和任务，我们允许智能体做少量的交互来快速学习描述子——一旦环境和任务描述子学习完成，它就可以构建规则，不需要像传统的强化学习那样重新学习规则。

具体来说，我们提出的框架如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnBD3BhAic6bZ0a2riaxvtj0MFWLra2Cia4q8TP2C6S9qK6iboJf5vqI47tTn8Sg4tPQeuBhoqf5tZgKA/640)

虽然上述框架具备了我们要求的同时学习规则以及环境与任务描述子的功能，但是在学习阶段有一个陷阱：很可能，**规则和描述子会耦合在一起**，使得智能体在见到的（环境、任务）组合上表现很好，但是没办法在测试阶段处理新的组合。

为了解决这个问题，就需要把规则构建方式和描述子解耦，使得描述子确实可以有效描述和区分不同的环境与任务，同时又能帮助构建有效的规则。相对应的，在训练的时候，除去传统的用于完成任务的奖励函数，我们还加上**针对描述子特有的用于分类的奖励函数**——基于描述子的状态描述应该能成功区分不同环境和任务。

# 实验设定

**我们分别在走迷宫和室内导航上进行了实验。**以下主要描述走迷宫；关于室内导航的实验请见论文。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnBD3BhAic6bZ0a2riaxvtj0M64pSRMe5EcaSSFxnw4U1icQlHuBtOub48WubMa5ZZyXClRKIpaUHH5Q/640)

上图展示了 16 个不同的迷宫（环境），其中红黄绿紫等颜色方块代表不同宝藏——它们各自代表一个任务，同时收集它们的不同顺序又能定义新的任务。

如下图所示，我们假定智能体在迷宫中行走的时候，视野范围只有 3 x 3。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnBD3BhAic6bZ0a2riaxvtj0MicsSUicwFso5SThVeVMdO3zn95xjb2EdD1xYKwIe5BQnRiaMicueeGCHiaA/640)

# 实验结果

如下图所示，我们的方法（SynPo）成功的超过了最近提出的几个方法，包括多任务神经网络（MTL），模块化神经网络（ModuleNet），专门用于迁移学习的 SF，还有一个我们自己方法的简化版（MLP）。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnBD3BhAic6bZ0a2riaxvtj0MNs4UefRq7N95kiaIgmWlQHpVIxez5ZG0B7NjdJBtMT5hOBZV0IpJJlA/640)

另外，我们也分别研究了智能体在见过和没见过的（环境、任务）组合上的表现。如下图所示，对于智能体在训练阶段见过的组合，它在测试阶段始终表现不错；对于没见过的组合，它只需要在 40% 的组合上学习，就能达到很好的效果。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnBD3BhAic6bZ0a2riaxvtj0MAkeRtriaIk0iaMtg9S89q3g5IUUVXhNkWD3emsH4ghCwbFwUy3cerOBw/640)

最后，我们给出在 10 个环境和 10 个任务组合上训练，然后在 20 个环境和 20 个任务上做测试的结果。如下图所示，每一行代表一个任务，每一列代表一个环境，其中左上角的 10 个环境和任务是用于训练的。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnBD3BhAic6bZ0a2riaxvtj0M4yV7yLXGYWRWpC6gEBH27ia4k2YibjXKHtj3kEsq4p9iaibRb42oQSb6iag/640)

测试阶段，为了拓展到新的环境（右上角），我们允许智能体和环境做少量交互，用于得到环境的描述子，然后构建规则；为了拓展到新的任务（左下角），我们如法炮制，得到任务的描述子。

得到所有描述子以后，我们就可以构建针对任意组合的规则了。在相应规则指导下，智能体完成任务的表现如上图，深色代表高成功率，浅色代表低成功率。我们可以看到向新的任务迁移比向新的环境迁移容易一些，而两者都是以前未见的时候，迁移是最难的。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多论文解读：**

[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)
[深度](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)[解读DeepMind新作：史上最强GAN图像生成器](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)
[本周有哪些值得读的AI论文？进来告诉你答案](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493220&idx=1&sn=b4c88ecfb42d081935c6deb548c681af&chksm=96ea39e4a19db0f26aede4b9e1e57208e97edfc50f736f0d4aaedaaf363939b774ac78f9ad35&scene=21#wechat_redirect)
[TensorSpace：超酷炫3D神经网络可视化框架](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492746&idx=1&sn=921d7315a973b85dd4e802cb5fd456fb&chksm=96ea3b0aa19db21c48841ddcee38592a3c086ae8fa1a9893cf46ff974f0f38fb350bcd528265&scene=21#wechat_redirect)
[NIPS 2018：基于条件对抗网络的领域自适应方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493113&idx=1&sn=cf5e7349f2dc7076e64926ec94ce364a&chksm=96ea3a79a19db36f6c174bbbddc83ee47f7966d2cdcce2260c5279865a06757afb76c6e0b2a4&scene=21#wechat_redirect)



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



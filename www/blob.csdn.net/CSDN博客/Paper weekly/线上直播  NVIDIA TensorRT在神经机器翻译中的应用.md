# 线上直播 | NVIDIA TensorRT在神经机器翻译中的应用 - Paper weekly - CSDN博客





2018年11月12日 18:01:48[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：97









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




神经机器翻译（Neural Machine Translation，简称 NMT）存在于各种各样的消费者应用程序中，包括 web 站点、路标、在外语中生成字幕等。 




NVIDIA 的可编程推理加速器 TensorRT™ 帮助优化和生成运行时引擎，用于将深度学习推理应用程序部署到生产环境中。同时，NVIDIA 的 TensorRT™ 工具可实现加速神经机器翻译（NMT）在 GPU 上的应用。**谷歌的神经机器翻译（GNMT）模型在 Tesla V100 GPU 上使用 TensorRT 进行推理的速度比纯 CPU 平台可快 60 倍。 **




**11 月 14 日（周三）晚 8 点**，NVIDIA 开发者社区经理 Ken He 将在 PaperWeekly 直播间为您介绍**如何利用 NVIDIA 最新发布的 TensorRT 5.0，加速神经机器翻译在 GPU 上的应用。**




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk8Y1ESwPAMK6GUwvibj6LSlnnXMJD5r8zHUmm9AbevhRIL1WOibGlfF5AIfRQGE6Sylnz5g9gjum5w/640)




**课程概要**







早期的 TensorRT™ 版本引入了一些用于 RNN 的层，如 LSTM 和 GRU。NVIDIA 最新发布的 TensorRT 5.0 支持新的 RNN 层，如批量矩阵相乘、Constant、Gather、RaggedSoftMax、Reduce、RNNv2 和 TopK。这些层允许应用程序开发人员使用 TensorRT™ 轻松加速 NMT 模型中最密集的部分。




本次线上课程，**我们将首先回顾神经机器翻译应用程序的体系结构，然后通过一个示例展示如何在 GPU 上执行此类应用程序的推理。**课程具体内容包括：



- 
神经机器翻译应用概述

- 
TensorRT™ 在 NMT 模型中的应用 

- 
TensorRT-NMT 样例展示





此外，**您还可以加入本次活动答疑群**，与拥有丰富一线开发经验的 NVIDIA 开发者社区经理进行实时交流。







**嘉宾介绍**







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglNsMYvszc6ek4NR8qiaE81Pzkj9hFibNX03sbzhqtyic1HrrlmAkXDERYSCJQy4EDcrgFB4mcnYjF8A/640?)



**何琨**

********** NVIDIA开发者社区经理**********




何琨 ， NVIDIA 开发者社区经理，7 年 GPU 开发经验，5 年人工智能开发经验。







**如何报名**







** 1 ******点击********阅读原文********填写报名表，********报名成功请截图保存****

** 2 ****扫描下方二维码添加小助手****，备注 ****NVIDIA****获取**直播地址**及入群通道**





NVIDIA × PaperWeekly

**NVIDIA TensorRT™**

**在神经机器翻译中的应用**



NVIDIA开发者社区经理何琨




**内容分享**√**在线Q&A√**




活动形式：PPT直播 + 交流群Q&A



** 活动时间 **

11 月 14 日（周三）20:00-21:30




**点击****阅读原文****，立刻报名**

*扫描下方二维码获取直播地址




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgk8Y1ESwPAMK6GUwvibj6LSlySMQDWFJTk2ZJo491XZjia4AQlsrf60kpiaxRVFZXdYicONic6jbTlWJxQ/640)










****往期直播****







[从编码器与解码器端改进生成式句子摘要](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487878&idx=2&sn=5ee19a2b98e98b065c7ac980dd1e725b&chksm=96e9ce06a19e4710476f1fe6d797cff1bacab02276c09ce9c39f665d81a3e52a13b520d50b79&scene=21#wechat_redirect)

[非自回归神经机器翻译 + ICLR 2018 论文解读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487767&idx=1&sn=87e0ba4d012cd77fb26a5318e95840a2&chksm=96e9ce97a19e4781563118acb4f91c5f2ac159fa66479fe09677a58828af963feb94fa5dd81e&scene=21#wechat_redirect)[实录](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487878&idx=2&sn=5ee19a2b98e98b065c7ac980dd1e725b&chksm=96e9ce06a19e4710476f1fe6d797cff1bacab02276c09ce9c39f665d81a3e52a13b520d50b79&scene=21#wechat_redirect)

[腾讯AI Lab涂兆鹏：如何提升神经网络翻译的忠实度](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487317&idx=1&sn=4a9b8364423a3296bb2027d452a5b644&chksm=96e9d0d5a19e59c34030a90998f7810f1351ee6317bb8b304184f8b7f41f32c41ea70194f92a&scene=21#wechat_redirect)


[微软亚洲研究院论文解读：GAN在网络特征学习中的应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487165&idx=1&sn=fa37ac27998e543d0866504218dbb4d9&chksm=96e9d13da19e582b578ba786ca7a1bb9f4f61c14e86b287100a862be676a7976593fe9a512ba&scene=21#wechat_redirect)


[微软亚洲研究院吴俣：基于动态词表的对话生成研究](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486877&idx=1&sn=17bead37cb0d0a19662ff65cbd2f944e&chksm=96e9d21da19e5b0b1b1d8a0c504b08d461bebc3a19ed34676ff2efff2c116aa8abdc815d34f9&scene=21#wechat_redirect)


[基于生成模型的事件流研究 + NIPS 2017 论文解读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486772&idx=2&sn=e4aa1601872de283e9898e007ca8fd39&chksm=96e9d2b4a19e5ba2dc5f46b752a5b93d48efdcc3fa8ce5ad122a056dea049b3893b74a2585dc&scene=21#wechat_redirect)






*查看更多往期实录，请点击PaperWeekly公众号底部**精品栏目**菜单。




🔍




现在，在**「知乎」**也能找到我们了

进入知乎首页搜索**「PaperWeekly」**

点击**「关注」**订阅我们的专栏吧







**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 免费报名





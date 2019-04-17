# EMNLP2018论文解读 | 利用篇章信息提升机器翻译质量 - Paper weekly - CSDN博客





2018年11月22日 13:06:16[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：222









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




**《使用篇章上下文提升 Transformer 翻译模型》**是搜狗和清华大学天工研究院合作发表在 EMNLP 2018 的工作。机器翻译在搜狗公司的多个产品线得到广泛应用，包括搜狗英文搜索、搜狗输入法、搜狗翻译 APP、搜狗旅行翻译宝等。许多产品场景下，都需要使用句子的上下文信息来帮助翻译。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4M4BTbnfYodiczAiaQKfLIoDdUPPShFDU0egd5VF1JfxBkRKa7bMvibxibw/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4C0xTMwJib1wUq3IzHVaHH7NkEEiauMjaCpqDY1tMibHJFl6ibXr6kNe9qQ/640)




为此，**我们提出了一个基于自注意力翻译模型 Transformer 的篇章级别翻译模型，利用丰富的上下文信息来帮助进行句子的翻译。**与此同时，该模型对机器翻译领域篇章级别翻译质量的提升也有贡献。




篇章翻译可以解决机器翻译中的上下文依赖等问题，是机器翻译的重要研究问题，也是今年的一个热点，我们的模型是在 Transformer 效果上最好的之一。 









# 论文方法




**已有的考虑篇章上下文的 NMT 工作主要存在以下几个问题：**




1. 大多数方法是基于循环神经网络的翻译模型 RNNsearch 的，无法移植到最新的基于自注意力的翻译模型 Transformer 上；




2. 大多数只在解码器端利用篇章上下文信息，而没有在源端编码时使用； 




3. 只能使用篇章级别的语料进行训练，但是篇章级别的语料规模比句级别语料小的多。



我们针对以上几个问题进行改进。首先先在建模方面，我们**通过在自注意力模型上添加模块来融入篇章上下文信息**（图 1），其中红色部分是我们新增的模块。最左边这一列是上下文编码器，其输入是源端待翻译的句子同一个文档中的前 k 个句子。上下文编码器是一个多层结构，每一层都包含一个自注意力层和一个前向层（类似于 Transformer 的编码器）。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC46GSVwCdqT9LdDrxrbNicoenyyIEOTFBtPIiaTfIEh4s28uPiakeqia8Cvw/640)

▲ 图1. 本文提出的模型




但是在实验中我们发现，上下文编码器只用 1 层就达到了很好的效果，使用更多层不仅不能得到更好的效果，还会对模型的计算效率产生负面影响。接下来，**我们使用 multi-head 注意力机制将得到的篇章上下文的隐层表示同时融入到源端编码器和解码器中**。我们将此称为上下文注意力（Context Attention）。 




为了得到更好的模型，我们采用了**两步训练法**来利用充足的句级别语料。第一步，我们**使用所有可用语料（包括句级别和篇章级别）训练一个标准的自注意力翻译模型**。第二步，我们再**使用篇章级别的语料训练我们加入的模块**。在这一步，我们会冻结其他参数，防止模型在较小规模的篇章级别语料上过拟合。



# 实验结果




实验（表 1）表明，我们的方法显著好于 Transformer 和基于 RNNsearch 的篇章翻译模型。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkXQJibZw3r48IKSkzlX7IC4dcdFicYFuosA9f3QMuCq2BANn7NnyaBjr8E0f3kEzdDOFNXIcx5Sib9Q/640)

▲ 表1. 实验结果




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看更多论文解读：**




- 
[网络表示学习综述：一文理解Network Embedding](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490862&idx=1&sn=707fe122dfc5d961a22771111514fada&chksm=96e9c2aea19e4bb8755d6759dd8e70cb44d2da2c454947395dede0268f511c140441cd1fb5ce&scene=21#wechat_redirect)

- 
[神经网络架构搜索（NAS）综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491248&idx=1&sn=d2946d8a37f7c6567b1a767a497006fb&chksm=96e9c130a19e48267f72ad32c527ec4a1697741e409d865d9233c5d7035a1f66a59b5e40792d&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)


- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)[哈佛NLP组论文解读：基于隐变量的注意力模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490536&idx=1&sn=0998c5dd4e20841f3542ee328de1f1b4&chksm=96e9c468a19e4d7e24a38f3d9c4b1b4ea48d729c7db124f2b92a2309f0eee982bb9f9bc8356e&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)

- 
[COLING 2018最佳论文：序列标注经典模型复现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490099&idx=1&sn=2d2497999186b979dd557fe3133b7606&chksm=96e9c5b3a19e4ca550a7ae55705af84e941b1aba14cb21f3f2ffc366df837d387575f8529cf2&scene=21#wechat_redirect)

- 
[一文解析OpenAI最新流生成模型「Glow」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490358&idx=1&sn=b4b5d6014bdd365456d500537ba5bcad&chksm=96e9c4b6a19e4da08710a55935dc2e15b00838d5395fdf2a424c50dedff9af7fa11441741b9d&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。




📝 **来稿标准：**

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

▽ 点击 | 阅读原文| 下载论文 & 源码





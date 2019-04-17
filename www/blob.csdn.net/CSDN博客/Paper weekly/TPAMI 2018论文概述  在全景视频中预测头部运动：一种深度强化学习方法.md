# TPAMI 2018论文概述 | 在全景视频中预测头部运动：一种深度强化学习方法 - Paper weekly - CSDN博客





2018年10月18日 12:51:56[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：223









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨徐迈、宋宇航、王健伊、樵明朗 等

学校丨北京航空航天大学本科生

研究方向丨全景视频与强化学习




本文概述了 2018 年 8 月发表在 IEEE TPAMI 期刊的论文 ***Predicting Head Movement in Panoramic Video: A Deep Reinforcement Learning Approach***。在此论文中，北京航空航天大学本科生宋宇航、王健伊、樵明朗等及其导师徐迈、王祖林，**提出了一种基于强化学习的全景视频头部运动预测方法，实现了在离线和在线两种设定下对人类观看全景视频时视野（即：头部运动）预测。论文相关数据库及代码均已开源。**



**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRicM6Y29CGTQdRvsLGfuauHfa5jC3bwk1ZzBLMv2egHiafkB6ZWffqBmA/640)**




■ 论文 | Predicting Head Movement in Panoramic Video: A Deep Reinforcement Learning Approach

■ 链接 | https://www.paperweekly.site/papers/2417

■ 源码 | https://github.com/YuhangSong/DHP

■ 数据 | https://github.com/YuhangSong/DHP




# 背景




在观看全景视频的时候, 人们通过对头部运动（HM）的控制使自己能够自由地控制视野（FoV），从而产生身临其境的交互式体验。因此，HM 在人类对全景视频的注意力建模中起着关键作用。




**本文首次建立了一个收集全景视频注意力的数据库**（图 1），采集了被试者在全景视频序列上的 HM 位置。从该数据库中，论文发现 HM 数据在不同被试者中高度一致。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnR6RjgwFCCic7sa15SD6w2WwhP4C8ibY45z6Cx2ovAFa8T1rJoS6OaX6FA/640)

▲ 图1. 本文首次建立的一个收集全景视频注意力的数据库（部分）




据此，**论文首次提出应用深度强化学习（DRL）**，最大化智能体与人类行为的一致性，进而来预测 HM 位置是一个合理的预测全景视频注意力模型的方法。




根据论文的研究结果，论文提出了一种基于 DRL 的 HM 预测方法（DHP），该方法有离线和在线两个版本（图 2），分别称为离线-DHP 和在线-DHP。




在离线-DHP 中，论文运行多个 DRL 工作流以确定每帧可能的 HM 位置。然后，生成热点图作为 offline-DHP 的预测输出。在在线-DHP 中，论文给定当前观察到的 HM 位置，来估计被试者的下一个 HM 位置。




最后，实验结果验证了论文的方法可有效地预测全景视频的注意力，并且离线-DHP 模型可以有效地提升在线-DHP 的性能。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRL1q0cGmakxLNrnHSkau1KBPiasPrP9HxtlKTvlCKqR3ic0eNXPbtKhFA/640)

▲ 图2. 全景视频注意力的离线和在线模型




# 离线-DHP




论文首先提出了离线-DHP 算法。因为观看者根据全景视频内容控制头部运动、产生头动轨迹，论文提出的离线-DHP 方法通过预测多个智能体（agent）的头动轨迹产生全景视频的显著性热点图（图 3）。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRtcjjwKzMYSsSgGQWxRMiaztiaH08ZnOJDfR6iciaUnGWXyldWbWVkwSPew/640)

▲ 图3. 离线-DHP算法的整体框架




**首先，基于论文的数据库中，我们发现：**




1. 不同观看者的头动轨迹高度一致；




2. 观看者的视野以较高概率出现在视频中心（精度、维度均为 0）；




3. 观看者下一时刻的观看位置可以由当前时刻观看位置预测。




根据这些发现，论文提出的离线-DHP 方法通过强化学习预测多个观看者的头动幅度和方向来预测下一时刻的头部位置。在训练中，建立强化学习的回报函数（reward function），旨在衡量的智能体和观看者头动之间的行为差异，即智能体的头动与人类头动位置的一致性。进而通过深度强化学习获得头动预测模型；在多工作流的头动模型下，可产生多个头动轨迹，进而生成热点图和显著性图。




# 在线-DHP




接下来，论文提出了在线-DHP 算法。在线-DHP 算法旨在根据某个特定观看者的历史头动轨迹**预测其下一帧的****头动位置**。整个过程分为训练和预测两个阶段。




首先，利用历史轨迹对深度强化学习网络的参数进行微调，之后利用训练好的深度强化学习网络来预测下一帧的头动位置。**其在线-DHP 包括训练和预测两个步骤**，流程图如图 4 所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglfdlFuXF2Emh37AACgOnnRwdo1eF7xBhbWbSIQ4vsTCxCTxhslrwOnQGOs0qSLQIyTtfibjibAMUPQ/640)

▲ 图4. 在线-DHP算法的整体框架




# 实验




对于离线-DHP，选用了 15 个视频作为测试集。论文采用了 3 个常用的标准对预测结果进行评估：CC、NSS、sAUC。实验结果表明论文提出的方法在 3 个标准下的表现都明显优于对比算法。 




此外，论文展示的主观实验结果表明：**论文提出的 DHP 算法生成的头动热点图与真实的人类头动热点图接近。**可见，离线-DHP 能够更有效的预测不同人在观看全景视频时的头动位置。




对于在线-DHP，论文对比了当前最新的算法。实验结果表明：论文提出的在线-DHP 优于其他比对算法。此外，实验还验证了使用离线-DHP 的模型对在线 DHP 算法进行初始化，可有效提升全景视频的在线头动位置预测的精度。




**综上所述，论文提出的方法可有效地在线/离线预测人类观看全景视频的头动位置（即：视野）。**




# 结论




**本文是最早揭示人类观看全景视频机理的工作之一**，在未来，可提升全景视频的压缩、渲染等处理任务的有效性，有着广泛的应用场景。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看更多论文解读：**




- 
[网络表示学习综述：一文理解Network Embedding](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490862&idx=1&sn=707fe122dfc5d961a22771111514fada&chksm=96e9c2aea19e4bb8755d6759dd8e70cb44d2da2c454947395dede0268f511c140441cd1fb5ce&scene=21#wechat_redirect)

- 
[神经网络架构搜索（NAS）综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491248&idx=1&sn=d2946d8a37f7c6567b1a767a497006fb&chksm=96e9c130a19e48267f72ad32c527ec4a1697741e409d865d9233c5d7035a1f66a59b5e40792d&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)


- 
[深度](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)[解读DeepMind新作：史上最强GAN图像生成器](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)

- 
[ECCV 2018 | 腾讯AI Lab提出视频再定位任务](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491347&idx=1&sn=e1f2cc16c9fcfcc5d2935118f09ee094&chksm=96e9c093a19e49855931cf621ec7f715c1d2dd5041e3343bc311aea2e5069ae03aaa1367e8f9&scene=21#wechat_redirect)

- 
[KDD 18 | 斯坦福大学提出全新网络嵌入方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491364&idx=1&sn=afa577a1bebfd148ac20cde05a927e24&chksm=96e9c0a4a19e49b2458f451c6e201277c509905201a1f751b30fbfa1e2ba31bc2711b2ca1b7a&scene=21#wechat_redirect)











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





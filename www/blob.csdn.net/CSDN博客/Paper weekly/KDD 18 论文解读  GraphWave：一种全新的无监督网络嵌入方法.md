# KDD 18 论文解读 | GraphWave：一种全新的无监督网络嵌入方法 - Paper weekly - CSDN博客





2018年11月19日 12:43:54[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：188









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnC9iaic8hDbiadLafh7TtCZS6icEYddVmMqZBksDV7cQkKmAu95h53FxyibqmZOS1yQgHibJT0WYD2s1Zw/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **119** 篇文章

作者丨薛寒生


学校丨澳大利亚国立大学博士生

研究方向丨人工智能与计算生物学




本期推荐的论文笔记来自 PaperWeekly 社区用户 **@xuehansheng** ，**本文是斯坦福大学发表于 KDD 18 的工作，论文提出了一种通过利用热小波扩散模式通过低维嵌入来表示每个节点的网络邻域的方法——GraphWave。**




GraphWave 不是在手工选择的特征上进行训练，而是**以无人监督的方式**学习这些嵌入。文章在数学上证明具有相似网络邻域的节点将具有类似的 GraphWave 嵌入，即使这些节点可能驻留在网络的非常不同的部分中。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnIlD37zHjPfkhnanWyDOicA52ZlkjCZE1H1hY4qiazH4j3HuTG9v9trdMQw20fhQGaFHo1EYJRJbuQ/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnIlD37zHjPfkhnanWyDOicAStDPjQY8YBNLIXhPQMspmkIIRDBjgYQUlTVqLR1Fva4arHu4BoJY9w/640)




# 论文动机




驻留在图的不同部分中的节点可能在其本地网络拓扑中具有类似的结构角色。然而学习节点的结构表示是一项具有挑战性的无监督学习任务，其通常涉及为每个节点人工指定和定制拓扑特征。 




**GraphWave 是一种可扩展的无监督方法**，用于基于网络中的结构相似性来学习节点嵌入。** GraphWave 通过将小波视为概率分布并使用经验特征函数表征分布来开发光谱图小波的新用途。 **




GraphWave 提供理论保证，具有相似本地网络邻域的节点将具有类似的 GraphWave 嵌入，即使这些节点可能驻留在网络的非常不同的部分中。 **GraphWave 与边数成线性比例****，不需要任何人工定制节点的拓扑特征。**




# 模型介绍



GraphWave 基于以该节点为中心的谱图小波的扩散，学习每个节点的结构嵌入。直观地，每个节点在图上传播能量单位，并基于网络对该探测的响应来表征其相邻拓扑。




**GraphWave 使用一种新颖的方法将小波视为图上的概率分布。**通过这种方式，结构信息包含在扩散如何通过网络传播而不是传播的位置。为了提供矢量值特征，然后可以将其用作任何机器学习算法的输入，GraphWave 使用经验特征函数嵌入这些小波分布。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnIlD37zHjPfkhnanWyDOicAvxmFPzh6VxsMGESs9POczaHI8ncUhJmCPR9ytpmnbcQQCuJJJSrVEQ/640)




在上图中，节点 a 和 b 具有相似的局部结构角色，即使它们在图中很远。虽然 a 和 b 的原始光谱图小波签名/系数 Ψ 可能非常不同，但 GraphWave 将它们视为概率分布，因此可以自动了解系数分布确实相似。GraphWave 利用这些新见解，基于以节点 a/b 为中心的谱图小波的扩散，学习节点 a/b 的结构嵌入。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnIlD37zHjPfkhnanWyDOicAE529DUjvWoU9RHkQ9kDMyNQzZJ3h50LxmsYCMfIYljTLQlIyPXn9PA/640)

▲ GraphWave算法




# 实验结果




**Barbell Graph **




在这个例子中，文章考虑一个杠铃图，它由两个由长链连接的密集团组成。 我们将 GraphWave 应用于杠铃图并绘制学习结构签名的 2D PCA。 




从下图中可以看出，该图具有 8 个不同类别的结构等效节点，如颜色（左）所示。 结构签名的 2D PCA 投影（右）包含与杠铃图中的节点相同数量的点。 这是因为相同的签名具有相同的投影，导致重叠点。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnIlD37zHjPfkhnanWyDOicAD7fxAczC4ItNjZcsXVxBKcia6UziaW81T2CRVHKDdXf1PDvDjqD0hWjQ/640)




GraphWave 正确地学习了结构等效节点的相同表示，为 GraphWave 的理论保证提供了经验证据。这可以通过图中的结构等效节点（相同颜色的节点）在 PCA 图中具有相同的投影来看出。




特别是，GraphWave 正确地将 clique 节点（紫色）组合在一起。GraphWave 还正确区分连接杠铃图中两个密集团的节点。它以类似梯度的模式表示那些捕获这些节点的结构角色谱的节点（右）。 




**A Cycle Graph with Attached House Shapes **




在这个例子中，文中考虑一个图形，其中“房屋”形状沿循环图定期放置。和以前一样，我们使用 GraphWave 来学习图中节点的结构签名，然后使用有关结构角色的地面实况信息来评估 GraphWave 的性能。




图形在下图（左）中可视化，同时还有 GraphWave 结构签名的 2D PCA 投影（中间）。我们观察到结构等效节点的表示重叠，GraphWave 完美地恢复了 6 种不同的节点类型。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnIlD37zHjPfkhnanWyDOicAsYK4wnxF5PhtlVvT12CrgonGOhINIZa1iciaBpCcFhsEpkkGibWQndFnw/640)




可以看到小波系数分布的最终特征函数（右）。在该图中，不同形状的特征函数捕获不同的结构角色。我们注意到这些曲线所携带的蓝色，浅绿色和红色节点的作用之间的视觉接近度，以及它们与核心深绿色和紫色节点的明显差异。



# 总结




**本文提出了一种全新的 Network Embedding 方法 GraphWave，该方法使用谱图小波为每个节点生成结构嵌入**，通过将小波视为分布并评估结果特征函数来实现，为网络嵌入提供了全新的思路。

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)




**点击标题查看更多论文解读：**




- 
[T-GANs：基于“图灵测试”的生成对抗模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492629&idx=1&sn=f17d3d0fe099670356500288e65a59fe&chksm=96ea3b95a19db283165ad6ea498e57d10dffc647033a2d8c65fbec2bf2dab7fc44e2afe68963&scene=21#wechat_redirect)

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
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)

- 
[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



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




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)

▽ 点击 | 阅读原文| 下载论文 & 源码





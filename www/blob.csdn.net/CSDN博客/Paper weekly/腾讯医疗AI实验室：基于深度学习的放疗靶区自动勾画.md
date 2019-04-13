
# 腾讯医疗AI实验室：基于深度学习的放疗靶区自动勾画 - Paper weekly - CSDN博客


2018年11月19日 12:43:54[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：482


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

每年有超过六十万人被诊断患有头颈部癌症，其中许多人选择接受放射治疗。 但头颈部重要器官比较集中，解剖关系复杂，如果在治疗前未仔细隔离，放疗时周围组织可能会严重受损。

腾讯医疗 AI 实验室和美国加州大学合作，正在探索人工智能（AI）如何帮助分割过程，辅助放疗规划。近日联合在国际权威期刊《Medical Physics》发表最新研究成果——《器官神经网络：深度学习用于快速和全自动整体头颈危及器官靶区勾画》（AnatomyNet: Deep Learning for Fast and Fully Automated Whole-volume Segmentation of Head and Neck Anatomy）。

在头颈癌放疗过程中，医生根据患者 CT 图像手动描绘放疗靶区和危及器官（Organ at Risk，OaR），目的是最大限度将放射剂量集中在靶区内，而让周围正常组织或器官少受或免受不必要的伤害。然而，勾勒过程非常耗时，降低诊疗效率的同时，更是耽误了患者的治疗时间。

**该研究提出一种深度学习模型——“器官神经网络（AnatomyNet）”，可以快速地对整张 CT 的所有切片进行全自动化器官分割（Segmentation），在小于 1 秒钟的时间内完成一整幅头颈 CT 的危及器官勾画，大幅度提升了放疗靶区勾画效率。**研究成果一经发表，就引起国际领域内广泛认可，目前已被引用多次。与合作医院的临床测试表明，AI 靶区勾画能够大幅度减少医生工作时间，同时提升勾画准确率。

# 研究成果获国内外肯定

《Medical Physics》是美国医学物理学家学会（The American Association of Physicists in Medicine，AAPM）的官方期刊，该研究成果在期刊上发表后，目前已经被多个机构引用。

美国德克萨斯大学 MD 安德森癌症中心（MD Anderson Cancer Center）对腾讯医疗 AI 实验室的“器官神经网络”表示关注，并把该研究成果作为中心一项大规模研究的重要部分。该中心在美国乃至全球皆享誉盛名，多次被评为美国最佳癌症研究机构，也是公认的全球最好的肿瘤医院。

此外，论文一经发布，很快被加拿大瑞尔森大学、中国中科院等多家机构学者在其研究报告中被提及和引用，作为最新的研究突破获得国内外认可。

# “器官神经网络”为何广受关注？

在放射治疗过程中，医生需要基于患者 CT 图像勾画出危及器官，而一个标准的 CT 图像包含上百张切片，医生需要单独在每个切面中勾画出危及器官的位置，手动描绘非常耗时。一般来说，根据医生熟练程度，大概需要几个小时的时间。

自动危及器官分割技术可以同时减少治疗计划所占用的时间，以及提高治疗计划的质量。现有的器官自动分割主要使用基于模板的技术。这类技术尽管需要复杂的技术来创建模板，但是不能够充分地对病人之间的差异进行建模。需要时间超过 20 分钟。

腾讯医疗 AI 实验室研究团队提出器官神经网络（AnatomyNet）深度学习方法，可以对头颈 CT 图像进行危及器官分割。器官神经网络的输入是头颈 CT 图像的所有切片。该神经网络可以一次性产生所有危及器官的预测结果。

**器官神经网络的构建是基于常用的三维 U 网络（U-net）架构，但是腾讯 AI 实验室在三个重要的方面对其进行了扩展：**

1. 一种新的在整幅 CT 图像上进行自动分割的编码方式，而不是在局部图像块上，或者一部分 CT 图像切片上分割；

2. 在编码层中，加入三维 Squeeze-and-Excitation 残差结构来进行更好的特征表示学习；

3. 一种新的结合 Dice 损失和 Focal 损失的损失函数，用来更好地训练该神经网络。

**在深度学习的器官分割中，使用这些技巧解决两个主要的挑战：**

a. 小器官的分割（比如，视神经和视交叉），这些小器官仅仅只有几个切片；

b. 对于一些器官结构，数据标注不一致以及标注缺失给训练造成一些问题。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnIlD37zHjPfkhnanWyDOicArAmB37a2EG8VvxEUj9miaSMia6QeZiadzVbpDXTuQcIWPf8ktLBn6LIYA/640)
▲图1：器官神经网络危及器官分割结果

图 1 为器官神经网络危及器官分割结果，其中绿色为医生标注，红色为器官神经网络预测结果，黄色表示标注和预测重合，结果显示高度一致。

从图 2 的动态效果图上，可以更清晰地在三维 CT 所有切片上看到器官神经网络预测和医生标注的比较，其中左边显示的是医生标注，右边显示的是器官神经网络预测结果。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnIlD37zHjPfkhnanWyDOicAgSQGGic72SJKz8mJqCUW5IZ15tl7LVzjoHjxic8OQjfNlxgv41Jzecrg/640)
▲图2：动态效果图

从结果上说：**和之前 MICCAI 竞赛中最好的方法相比，器官神经网络平均提升了 3.3% 的 Dice 指标。**器官神经网络仅仅使用 0.12 秒就可以完全完成一整幅 CT 图像（178512512）的分割。该速度极大地缩短了之前方法所用的时间（20分钟以上）。除此之外，该模型可以处理一整幅包含所有切片的 CT 图像，以及一次性勾画所有的危及器官，不需要很复杂的预处理以及后处理。

**腾讯医疗 AI 实验室开发的深度学习模型提供了一种 CT 图像勾画危及器官的可行思路。**表明深度学习可以提升器官分割准确率，简化自动分割器官的流程。**该技术可以在小于 1 秒钟的时间内完成一整幅头颈 CT 的危及器官勾画。**

# 腾讯医疗AI实验室以科研成果推动技术创新

除本次发表于《Medical Physics》的研究成果，今年 7 月，腾讯医疗 AI 实验室已有 3 篇论文分别被 KDD 2018、SIGIR 2018、COLING 2018 三个国际顶尖学术会议收录，论文的主要研究方向为医疗知识图谱中实体关系的发现和应用。

腾讯医疗 AI 实验室是腾讯专为医疗领域打造的人工智能实验室，目前在硅谷、北京、深圳设立了三个分支。其作为腾讯医疗健康领域众重要技术驱动，腾讯 AI 实验室在学术研究层面不断发力，致力于 AI 医学领域的前沿突破。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnIlD37zHjPfkhnanWyDOicAJeeQmKOOKDicTfjTwwSMRFl2YSrCuaCicYgqkb6MQ6fOug0JWfc2oofQ/640)

实验室的主要研究方向是基于自然语言理解、医学知识图谱、深度学习、医疗影像、贝叶斯网络、多模态分析等基础技术构建医学知识引擎、医疗推理引擎、临床辅助诊断引擎、问诊对话引擎等智能平台。

腾讯医疗 AI 实验室致力于推动医疗 AI 开创性技术研发，目前，腾讯医疗 AI 实验室已推出帕金森病运动功能智能评估系统，其他主要产品还包括临床辅助决策支持系统，面向脑卒中、急性冠脉综合症等高危易误诊疾病提供临床辅助决策支持，以及心电图智能分析软件，利用 AI 技术实现心电图监测结果的自动判读和预警等。

近日，以腾讯医疗 AI 实验室为技术核心主体，腾讯作为牵头单位承担“2018 年科技部重点研发计划数字诊疗装备专项基于人工智能的辅诊系统项目”，将会组建高精尖队伍承担国家科研重任，搭建 AI 辅诊平台并向临床应用推进。

未来，腾讯医疗 AI 实验室也将持续通过研发 AI 技术与医疗的深度结合，构建良好的技术生态，从临床角度解决问题，服务于医患双方，助力医疗行业智慧化升级，同时着重助力分级诊疗国策的落地。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多论文解读：**

网络表示学习综述：一文理解Network Embedding
神经网络架构搜索（NAS）综述
从傅里叶分析角度解读深度学习的泛化能力

哈佛NLP组论文解读：基于隐变量的注意力模型
[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)
[COLING 2018最佳论文：序列标注经典模型复现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490099&idx=1&sn=2d2497999186b979dd557fe3133b7606&chksm=96e9c5b3a19e4ca550a7ae55705af84e941b1aba14cb21f3f2ffc366df837d387575f8529cf2&scene=21#wechat_redirect)
[一文解析OpenAI最新流生成模型「Glow」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490358&idx=1&sn=b4b5d6014bdd365456d500537ba5bcad&chksm=96e9c4b6a19e4da08710a55935dc2e15b00838d5395fdf2a424c50dedff9af7fa11441741b9d&scene=21#wechat_redirect)



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



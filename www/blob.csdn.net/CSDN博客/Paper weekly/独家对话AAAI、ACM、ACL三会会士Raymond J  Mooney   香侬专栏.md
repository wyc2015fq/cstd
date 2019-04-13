
# 独家对话AAAI、ACM、ACL三会会士Raymond J. Mooney | 香侬专栏 - Paper weekly - CSDN博客


2018年06月28日 12:45:01[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：335


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

**专栏介绍：**《香侬说》为香侬科技打造的一款**以机器学习与自然语言处理为专题的访谈节目**。由斯坦福大学，麻省理工学院, 卡耐基梅隆大学，剑桥大学等知名大学计算机系博士生组成的“香侬智囊”撰写问题，采访顶尖科研机构（斯坦福大学，麻省理工学院，卡耐基梅隆大学，谷歌，DeepMind，微软研究院，OpenAI 等）中人工智能与自然语言处理领域的学术大牛, 以及在博士期间就做出开创性工作而直接进入顶级名校任教职的学术新星，分享他们广为人知的工作背后的灵感以及对相关领域大方向的把控。

**本期采访嘉宾是德克萨斯大学奥斯汀分校计算机系教授 Raymond J. Mooney**。随后我们计划陆续推出 Eduard Hovy (卡耐基梅隆大学), Dan Jurafsky (斯坦福大学), Anna Korhonen (剑桥大学), Andrew Ng (斯坦福大学), Ilya Sukskever (OpenAI)，William Yang Wang (加州大学圣芭芭拉分校), Jason Weston (Facebook人工智能研究院), Steve Young (剑桥大学) 等人的访谈，敬请期待。

**德克萨斯大学奥斯汀分校计算机系教授、人工智能实验室主任 Raymond J. Mooney 带领他的人工智能小组研究了多个领域，目前他的主要研究方向是自然语言处理和计算语言学**。其本人曾在 2008-2011 年间担任国际机器学习协会（ICML 主办方）主席，曾多次担任 ICML、ACL、AAAI、EMNLP、NAACL 等会议主席或领域主席，现在为美国计算机学会（ACM）、国际人工智能学会 （AAAI）、国际计算语言学会（ACL）三会会士 。

他发表了百余篇重要的论文，Google Scholar 上有近 3 万的论文引用量，其中 2007 年发表的关于语义分析的文章获得了 ACL 2007 会议的最佳论文奖。目前正参与可解释 AI 项目（XAI）的研究工作，开发可解释的问答系统（EQUAS）。

**香侬科技：**您有一些论文是关于将逻辑方法和分布语义相结合的（e.g., Beltagy et al. 2016），这种综合方法显然有很多优点。**如果让您对只研究这两种方法中的一种的学者说几句，请他们考虑结合这两种看似不相容的方法，您会说什么？**

**Mooney：**我将强调诺贝尔奖得主、心理学家丹尼尔卡内曼在他的书 Thinking Fast and Slow 中讨论的“快速思考和慢速思考”之间的区别。逻辑学方法非常擅长利用复杂的符号推理来综合不同的事实，这是回答含有量词和逻辑连接词的复杂问题所必需的，例如“巴拉克•奥巴马在约翰•肯尼迪去世前还是后出生？”或者“Woody Allen 是与 Diane Keeton 一起制作的电影更多还是和 Mia Farrow 更多？”。

这是“慢速思考”。分布和神经方法在“模式识别”方面更好，并且在特定情境下可以快速直观地做出关于优选词义或句法结构的决策。这是“快速思考”。人类推理和语言理解需要恰当地结合这两者。如果想要像人一样高效地理解语言，我认为我们需要建立同等擅长“快速”和“慢速”思考的计算系统。

**香侬科技：**近年来，深度学习模型的可解释性受到了很多关注，领域中有很多方法被提出以解释深度神经网络的行为（解释性热图 explanatory heat-map，自然语言等）。在您的工作中，您和您的同事曾证明，利用解释性热图可以改善多个当下最先进的基于图像的问答系统（Rajani and Mooney, IJCAI 2017 Workshope XAI，Rajani and Mooney, NIPS 2017 ViGIL）。那么您认为，一般来说**增强深层神经网络的可解释性能够帮助我们建立更好的模型吗？****为什么？**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkDHHfczburiaUzatx4hjytxtRj0iaTKCfntYebsVhFibV47TVLXZKENoD0Tpnh14wpbDgZPE30dCqGw/640)
**▲**通过解释性热图来增强深度学习模型的可解释性
© Rajani and Mooney, NIPS 2017 Workshop ViGIL

**Mooney****：**深度学习模型的“不透明性”和“黑匣子”的特征被公认为是限制其发展和用户的信任程度的因素。因此，一年前，美国国防高级研究计划局（DARPA）开始了可解释人工智能（XAI）项目，试图开发更透明的深度学习系统。深度学习的爱好者们声称他们已经从机器学习中删除了“特征工程”（feature engineering），因为他们的模型可以自己学习到特征。

然而，“黑匣子”仅仅是从“特征工程”转移到了“神经网络结构工程”（network architecture engineering）中的设计层数，每层中的神经元数量，它们的连接方式以及连接各层时用到的函数（例如 softmax，线性整流函数 ReLU 或平均池化 mean-pooling）。

如果深度网络可以提供解释，它或许就可以帮助这些开发人员更好地理解系统出错的原因。不过，我认为解释的真正作用是支持用户而不是开发人员。通过提供解释，用户可以开始更多地相信系统并理解它们是如何做出决定的，更关键的是当系统不能提供容易理解的解释时，知道哪些结论是不能相信的。

**香侬科技：**您的许多工作都集中在主动学习和迁移学习上（e.g., Acharya et al. SDM 2014, Acharya, PhD Thesis, 2015）。显然这对于人机交互非常重要，因为在该场景下获得有标注的数据的成本是非常高的。**请问在实验室中验证的算法是否容易推广到现实生活场景？将实验室开发的主动学习和迁移算法应用于日常情况的最大障碍是什么？**

**Mooney：**主动学习和迁移学习都是有用的技术，可以减少学习新任务所需的监督量。我们最近在机器人应用中探索的主动学习的特殊变体就是我们所说的“基于机会的主动学习”（opportunistic active learning）。

在普通的“基于池”（"pool-based"）的主动学习中，系统可以随时询问整个未标注样本池中任何一个的标注。但是，在机器人环境中，你正在为特定用户完成特定任务，并且必须决定是否应询问用户当前环境中的某个物体的标签，而这个问题可能与当前任务无关。

我们开发了一种强化学习（Reinforcement Learning）方法，该方法可以学习到一种策略，从而最优地决定在特定环境下何时以及要询问哪些问题。虽然这些系统会因为提出与用户当前任务无关的问题而困扰用户，获得小的负反馈。然而，一个好的问题可以让系统学习到一些使其能够更快地解决用户接下来的任务的东西，并会获得巨大的正反馈。使用强化学习的系统可以学习到能够最大化其长期奖励的询问策略，用现在较小的损失，换取稍后较大的潜在收益。

**香侬科技：**您的大量工作集中在连接语言和感知（如视觉和触觉）上（e.g., Thomason et al. AAAI 2018, Thomason and Mooney, IJCAI 2017），这是自然语言处理中的一个经典问题，即语意落地（semantic grounding）。但也有许多自然语言处理任务似乎不需要语意落地（语言建模，机器翻译等）。**您为什么认为在某些情况下建立语言和感知的联系是必要的？这样做的代价和好处是什么？**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkDHHfczburiaUzatx4hjytxMfol4cDC4ibzdYHy9HZysP7wAyazloCvO2jAPXiaHplbdUk00aq0at6g/640)
**▲**通过对名词短语的观测特征向量（observation feature vectors）进行聚类来推导其语义
© Thomason and Mooney, IJCAI 2017

**Mooney：**在许多自然语言处理任务中，语言和人的感知及动作有明显联系，这时语意落地最为重要。涉及机器人学、计算机视觉和计算机图形的许多应用都符合这一要求，例如用自然语言给机器人指令，机器理解或回答关于图像和视频的问题，以及生成图形图像和视频。

然而，许多抽象概念无法直接落地为感知觉体验，如“博士申请人数量显著上升”中的“上升”，它并不是视觉上可以看得见的物理上升，就需要我们根据它们原有的语意落地的隐喻用法推导出引申含义（参见 Lakoff 和 Johnson 的“Metaphors we Live By”）。因此，理解这些词语原本的语意落地，对理解它们的隐喻有很大帮助，即使这时已经不直接涉及人的感知和行为。

**香侬科技：**过去您和您的学生的许多研究项目与人类和机器人的互动关系密切（e.g., Thomason et al., CoRL 2017, Thomason et al., Robo NLP 2017）。**您觉得建立人人交互的模型与建立人机交互的模型有什么区别呢？**当您在研究人机交互中的语言学习时，您遇到了哪些特别的挑战或机会？

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkDHHfczburiaUzatx4hjytx0QlOuUIQibK8h1JdDWqxHvksYH7sicFN7UibWNTiacIBDXDZhwLxdz11pA/640)
**▲**在Thomason, Padmakumar, Sinapov et al., CoRL 2017中，实验者首先用自然语言向机器人描述桌2（测试集）上某一物体，然后机器人针对桌1和桌3（训练集）上的物体进行发问，实验者回答。最后机器人猜测实验者描述的是桌2上的哪个物体。

**Mooney：**与人类相比，机器人执行复杂任务的能力仍然非常初级。我们在研究机器人掌握语言时曾遇到的主要问题之一是找到足够复杂，因此需要由丰富多样的语言来表述的任务，同时又不能太难，不然他们就根本无法完成。

我们曾主要研究让机器人寻找并把某一物体从一个位置送到另一个位置的任务，其中物体和位置可以用名词短语的组合来描述，例如“把冰箱旁边的蓝色马克杯拿到Ray办公室旁边的会议室去”。即使是这样看似简单的任务，让机器人能够准确无误地选择、传送这些物体也是非常具有挑战性的。

人机交互（Human Robot Interaction，HRI）是一个复杂的领域，自然语言只是其中的一部分。让机器人理解手势和其他非语言的交流也非常具有挑战性。目前，与机器人进行交流其实并不令人满意，与和人进行交流时的体验截然不同，而它们无法理解自然语言只是问题的一部分。

**香侬科技：**在过去，您做了很多有影响力的工作，并发表了许多被广泛引用的论文。**作为一名自然语言处理领域的学者，您的研究方法随着时间的推移是如何变化的呢？**关于对科研项目的良好品味的培养，您有什么想与学生们分享的吗？

**Mooney：**在选择研究问题这方面我很幸运，在我开始研究这些问题（例如语言和视觉的整合）之后，它们变得非常流行。但我确实有一些这方面的建议：其一，我不喜欢研究目前非常流行的问题。因为你很难跟上这个问题所有相关工作的进展，并提出一个其他人都没想到的想法。所以我避免了当前的“热门话题”。

其次，我大概每六年转换一次我正在做的工作，这差不多是博士生导师引导一个博士解决一个科研难题的时间。在研究了一个问题大约  6年后，我觉得自己对这个问题的看法变得过于固定、僵化，让我不再能创造性地思考这个问题，所以我会去看看其他东西。而在经过足够的时间让我的思想发生了变化之后，再回过头来看我过去曾经研究的问题是很有意思的。比如，我在 80 年代写博士论文时曾研究了通过“脚本学习”来处理文本，而最近我又与我的博士生 Karl Pichotta 一起以深度学习为基础，用全新的视角重新回来研究它。

最后，我是跨越传统界限的“跨学科”工作的忠实信徒。早在 20 世纪 90 年代，那时自然语言处理和机器学习完全不同，还是人工智能的两个不相干的子领域，我就尝试将最新的机器学习想法应用于自然语言中的比较难的问题，如语义分析和信息提取。幸运的是，现在机器学习和自然语言处理有了紧密的联系，几乎所有人都将二者结合在一起。

我对语意落地研究的兴趣则是为了跨越自然语言处理与计算机视觉及机器人学之间的界限。我目前感兴趣的两个领域是结合自然语言处理和计算机图形学，特别是从自然语言描述中生成三维动画视频；以及将自然语言处理和软件工程集成在一起，比如将自然语言转化为代码，以及代码变化时自动修改注释。大多数创造性的灵感都来源于整合以前那些看似风马牛不相及的想法。打破传统学科界限，带来了许多将两个截然不同领域结合起来的可能性。

**香侬科技：**我们都知道，目前在自然语言领域有很多分领域，您自己的研究就覆盖了很多不同领域。**那么您认为在未来 5 年自然语言处理最大的挑战在哪里？**

**Mooney：**我认为上面的采访中谈到的许多问题仍然是自然语言领域最大的挑战。弄清楚如何有效地将符号、逻辑意义表示与词/句向量分布表示（distributional representation）相结合，是一个重要的问题，然而很少有研究人员探索它。另外，全面考虑语言各个层级间（语音，句法，词汇，语义和语用等）的相互作用，来消除歧义，也是一个重要的挑战。

此外，我相信自然语言处理和计算机图形学的结合也有许多未探索的新机会，如根据自然语言描述自动生成复杂的图像和视频。然而，要机器实现对自然语言完全的理解，期待其达到人类的理解能力，还有很长的路要走，毕竟这一领域的各个方面都存在着许多具有挑战性的问题。

# 参考文献

[1]. Thomason J, Sinapov J, Mooney R J, et al. Guiding exploratory behaviors for multi-modal grounding of linguistic descriptions[J]. Intelligence (AAAI-18), 2018.
[2]. Thomason J, Mooney R J. Multi-modal word synset induction [C]. Proceedings of the 26th International Joint Conference on Artificial Intelligence (IJCAI-17). 2017.
[3]. Lakoff G, Johnson M. Metaphors we live by [M]. University of Chicago Press, 2008.
[4]. Thomason J, Padmakumar A, Sinapov J, et al. Opportunistic active learning for grounding natural language descriptions [C]. Conference on Robot Learning. 2017: 67-76.
[5]. Corona R, Thomason J, Mooney R. Improving black-box speech recognition using semantic parsing[C]. Proceedings of the Eighth International Joint Conference on Natural Language Processing (Volume 2: Short Papers). 2017, 2: 122-127.
[6]. Rajani, N.F. and Mooney, R.J., Stacking With Auxiliary features. Proceedings of the Twenty-Sixth International Joint Conference on Artificial Intelligence (IJCAI), Melbourne, Australia, pp. 2634– 2640, Aug. 2017.
[7]. Rajani N F, Mooney R J. Ensembling visual explanations for VQA [J]. Proceedings ofthe NIPS 2017 workshop on Visually-Grounded Interaction and Language (ViGIL), Dec. 2017.
[8]. Acharya A, Mooney R J, Ghosh J. Active multitask learning using both latent and supervised shared topics [C]. Proceedings of the 2014 SIAM International Conference on Data Mining. Society for Industrial and Applied Mathematics,2014: 190-198.
[9]. Acharya A. Knowledge transfer using latent variable models [D]. 2015.
[10]. Beltagy I, Roller S, Cheng P, et al. Representing meaning with a combination of logical and distributional models [J]. Computational Linguistics, 2016, 42(4): 763-808.
[11]. Kahneman D, Egan P. Thinking, fast and slow [M]. New York: Farrar, Straus and Giroux, 2011.

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)
**香侬招聘**

香侬科技 (http://shannon.ai/) ，是一家深耕金融领域的人工智能公司，旨在利用机器学习和人工智能算法提取、整合、分析海量金融信息，让 AI 为金融各领域赋能。

公司在 2017 年 12 月创立，获红杉中国基金独家数千万元融资。创始人之一李纪为是斯坦福大学计算机专业历史上第一位仅用三年时间就获得博士的人。过去三年 Google ScholarCitation>1,800，h-index达21。公司硕士以上比例为 100%，博士占比超 30%，成员皆来自斯坦福、MIT、CMU、Princeton、北京大学、清华大学、人民大学、南开大学等国内外知名学府。

**全职或实习加入香侬研发团队，请****[点击查看香侬招聘贴](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488179&idx=2&sn=6342c9c9f23522a12c4120ef4f0ff1d9&chksm=96e9cd33a19e442589137eacd2aa347042844b3991b36bfe9a9eb122096a0252f4fd785de46c&scene=21#wechat_redirect)**
**简历投递邮箱：**hr@shannonai.com
**Tips：****听说在邮件标题中注明****“****PaperWeekly****”****，能大大提升面试邀约率**

# 英文采访稿

**ShannonAI****:**You have several papers on combining logical approach and distributional semantics (e.g., Beltagy et al. 2016). There are obviously many advantages of this integrative approach. But if you are asked to say only one thing to researchers that have worked on only one of these two approaches, to invite them to consider combining these two seemingly incompatible approaches, what would it be?
**Mooney:**I would emphasize the difference between "Thinking Fast and Slow" discussed by Nobel-prize winning psychologist Daniel Kahneman in his book of this title. Logical approaches are really good at combining disparate facts using complex symbolic reasoning, which is required for answering intricate questions with quantifiers and logical connectives such as "Was Barack Obama born before or afterJohn F. Kennedy died?"  or "Did Woody Allen make more movies with Diane Keeton or Mia Farrow?". This is "thinking slow." Distributional and neural approaches are better at "pattern recognition," and quickly, intuitively making decisions about preferred word meaning or syntactic structure in particular contexts. This is "thinking fast." Human reasoning and language understanding requires a judicious mixture of both of these. If we are ever going to understand language as effectively as humans, I think we need to build computing systems that are equally good at thinking both "fast"and "slow".

**ShannonAI:**In recent years, there has been a lot of attention on the interpretability of deep learning models. Various approaches have been proposed to explain deep neural networks's behaviors (explanatory heat-map, natural language, etc.) In your own work, you and your colleagues have shown that using explanations could improve ensembling of three state-of-the-art VQA systems (Rajani and Mooney, IJCAI 2017XAI). Do you think in general enhancing the interpretability of deep neural nets could help us build better models? Why or why not?
**Mooney:**The"opaqueness" and "black box" quality of deep learning models are well recognized as limiting their development and users' willingness to trust them.  Because of this, a year ago DARPA started the ExplainableAI (XAI) project to try to develop more transparent deep learning systems. Deep learning enthusiasts claim that they have removed "feature engineering" from machine learning since their models develop their own features; however, the "black art" has simply been moved from "feature engineering" to "network architecture engineering" designing the number of layers, the number of neurons in each layers, their connectivity, and the combining function used for each (e.g. softmax, ReLU, or mean pooling). If deep networks could provide explanations, it might aid such engineering by allowing the developer to understand better why the system is making errors. However, I think the real role of explanation is to support the user rather than the developer. By providing explanations, the user can learn to trust the system more and understand how it is making decisions, and more critically what conclusions not to trust when the system cannot provide a comprehensible explanation.

**ShannonAI:**Much of your work has concentrated on active learning and transfer learning (e.g., Acharyaet al. SDM 2014, Acharya 2015). Apparently this is extremely important in human-robot interaction since the cost of labeled data is very high. Do these promising results obtained in laboratory experiments easily generalize to real life scenarios? What are the biggest obstacles of applying those active learning and transfer algorithms developed in the lab to everyday situations?
**Mooney:**Both active learning and transfer are useful techniques to reduce the amount of supervision needed to learn a new task. The particular variant of active learning we have been recently exploring in robotics applications is what we have called "opportunistic active learning." In normal "pool-based" active learning, the system can ask for a label for any of the complete "pool" of unlabeled examples at any time.  However，in a robotics context, you are engaged in a particular task for a particular user and have to decide if it is worth asking that user a question about one of the objects in the current environment, which may not be relevant to the current user's task. We have developed a reinforcement learning (RL) method which learns a policy about when and what active learning queries to ask while in a particular environment. The systems get a small negative reward for bothering the user and asking a question not relevant to their current task. However, a good question will allow the system to learn something that will allow it to more quickly solve a later users' task and receive a large positive reward. Using RL, the system can learn a query policy that maximizes its long-term reward; trading off small inconveniences now against larger potential gains later.

**ShannonAI:**A lot of your work has concentrated on connecting language and perception/ grounded language learning (e.g., Thomason et al. AAAI 2018, Thomason and Mooney, IJCAI 2017). But there are also many NLP tasks that seem not to require grounding (language modeling, machine translation, etc.). Why do you think is it necessary to model grounded language learning under certain circumstances? What are some costs and benefits of doing so?
**Mooney:**I believe grounding is mostly important in applications and concepts that clearly have connection to perception or action in the world. Many applications involving robotics, vision, and graphics fit this requirement such as instructing robots in natural language, captioning or answering questions about images and videos, and generating graphics images and video. However, many abstract concepts that are not directly grounded such as "up" in "the number of PhD applicants has gone up dramatically," actually require much of their semantics from metaphorical use of grounded terms (see Lakoff and Johnson's"Metaphors we Live By"). Therefore, understanding the physically grounded sense of such terms will allow productive understanding of such metaphors even for applications that do not directly involve perception and/or action.
**ShannonAI:**Many of you and your students' research projects have close connections with human interaction with robots (e.g., Thomason et al. CoRL 2017, Thomason et al. Robo NLP 2017). Is there any difference between modeling human-human interaction and human-robot interaction? What are some unique challenges/opportunities you have found when you work on language learning in human-robot interactions?
**Mooney:**Robots' abilities to perform complex tasks is still very primitive compared to humans. One of the main problems we have had working in robot language is finding tasks they can perform that are complex and interesting enough to require language instruction of reasonable richness and variety. We have mainly worked on instructions for finding and delivering a particular object from one location to another where the object and location can be described with compositional noun phrases, for example "Get the heavy blue mug next to the refrigerator and bring it to the meeting room next to Ray's office." Even for this task, getting the robot to be able to grasp a wide range of liftable objects is very challenging. Human Robot Interaction (HRI) is a whole complex field, of which natural language is only a part. Getting robots to understand gesture and other non-verbal communication is also very challenging. Currently, communicating with robots is frustrating and very different from interacting with other people, and their inability to understand natural language is only part of the problem.

**ShannonAI:**In the past you have done a lot of influential work and have published many widely cited papers. How does your approach as a NLP researcher change overtime? Is there anything you would like to share with students for developing good taste for research problems?
**Mooney:**I've partly gotten lucky in choosing problems to work on (e.g. language and vision) that have gotten significantly more popular after I start working on them.  But do I have a few recommendations about choosing problems. One, I don't like working on problems that are currently very popular. This makes it too hard to keep up with all the related work and to come up with truly new ideas that no one else is pursuing. So I avoid the current "hot topics". Second, I significantly switch what I am doing about every 6 years, about the time it takes to complete advising a PhD student on a topic. After working on a problem for about 6 years, I feel my view of the problem becomes too fixed and"set in stone" and I am no longer able to think creatively about it, so I switch to something else. It's interesting to come back to a problem I worked on in the past, after sufficient time has past to allow my thinking to have changed. For example, I worked on "script learning" for text processing for my PhD thesis in the 1980's and then recently returned to it with my PhD student Karl Pichotta with a completely new perspective based on deep learning. Finally, I'm a big believer in "inter-disciplinary" work that crosses traditional boundaries. Back in the 1990's when NLP and ML were quite different, disjoint sub-areas of AI, I worked on trying to apply the latest learning ideas to challenging problems in NLP such as semantic parsing and information extraction. Fortunately, these days there is a good flow of communication between ML and NLP so almost everyone is doing this now. My interest in language grounding tries to cross the boundary between NLP and computer vision and robotics. Two of my current areas of interest are integrating NLP and computer graphics, particularly generating 3-D animated video from natural language descriptions; and integrating NLP and software engineering, particularly language to code and helping automate changes to comments when software is changed. Most creativity involves combining and synthesizing previously disparate ideas. Looking at problems that cross traditional disciplinary boundaries opens up many possibilities combining ideas from the two separate areas.
**ShannonAI:**Obviously there is a variety of topics in the field of NLP, and your own research has covered a wide range of topics. What do you think is the biggest challenge of NLP in next 5 years?
**Mooney:**I think many of the problems discussed above still are some of the biggest challenges in NLP. Figuring out how to effectively combine symbolic, logical meaning representations with vector-space distributional ones is an important problem that few researchers are exploring. Developing computational architectures that allow efficiently and effectively jointly resolving inter-dependent ambiguities at all levels of linguistic processing (phonetic, syntactic, lexical, semantic, and pragmatic) also remains a key challenge. I believe there are also many unexplored opportunities at the intersection of NLP and computer graphics, allowing the automatic generation of complex images and videos from natural language descriptions. However, we are still along way from achieving robust, human-level understanding of natural language, and there are many challenging problems remaining in almost all areas of the field.


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 加入社区刷论文



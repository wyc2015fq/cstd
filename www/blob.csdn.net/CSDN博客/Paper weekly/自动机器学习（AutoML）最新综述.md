# 自动机器学习（AutoML）最新综述 - Paper weekly - CSDN博客





2018年11月07日 13:05:40[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：732









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnC9iaic8hDbiadLafh7TtCZS6icEYddVmMqZBksDV7cQkKmAu95h53FxyibqmZOS1yQgHibJT0WYD2s1Zw/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **115** 篇文章

作者丨王晋东


学校丨中国科学院计算技术研究所博士生

研究方向丨迁移学习和机器学习




**这是一篇来自第四范式（4Paradigm）公司的关于 AutoML 的综述文章。**第四范式是目前国内关于 AutoML 研究较早较深入的公司之一。AutoML 全称是 Automated Machine Learning，是 2014 年以来，机器学习和深度学习领域最炙手可热的领域之一。



**本篇综述文章系统地对 AutoML 领域给出了综述，从出现原因、问题定义、问题构成、基本策略、高级策略、应用、及总结等方面进行了全面的介绍。**下面是一些简要的笔记。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnOcgiaxibd0EhHPOLNz5Zvcic7Vq2Wl5LP0Aw3MtTm38uI9iblFGxNP0w9KkxRpBMJo4lDoK4dJaQ3wg/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnOcgiaxibd0EhHPOLNz5ZvcicK2LJMGIVNX5GW5U5CicvP3QHrVud33WQfFFgghGcB7n4fZibO1dMVj4A/640)




# AutoML出现原因




机器学习的应用需要大量的人工干预，这些人工干预表现在：特征提取、模型选择、参数调节等机器学习的各个方面。AutoML 试图将这些与特征、模型、优化、评价有关的重要步骤进行自动化地学习，使得机器学习模型无需人工干预即可被应用。




# AutoML问题定义



**作者从机器学习和自动化两个角度给出了定义：**




1. 从机器学习角度讲，AutoML 可以看作是一个在给定数据和任务上学习和泛化能力非常强大的系统。但是它强调必须非常容易使用；




2. 从自动化角度讲，AutoML 则可以看作是设计一系列高级的控制系统去操作机器学习模型，使得模型可以自动化地学习到合适的参数和配置而无需人工干预。 




**一个通用的 AutoML 定义如下：**



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnOcgiaxibd0EhHPOLNz5ZvcictngGwlloKZVtyM6QaiaY9mc3mcSQWzrLFp7qNOgyjRfV30tPV7ziaLBw/640)



**AutoML的核心任务： **



- 
Better performance 

- 
No human assistance 

- 
Lower computation budgets





# AutoML问题构成




**AutoML 的主要问题可以由三部分构成：特征工程、模型选择、算法选择。**




**特征工程**




特征工程在机器学习中有着举足轻重的作用。**在 AutoML 中，自动特征工程的目的是自动地发掘并构造相关的特征，使得模型可以有最优的表现。**除此之外，还包含一些特定的特征增强方法，例如特征选择、特征降维、特征生成、以及特征编码等。这些步骤目前来说都没有达到自动化的阶段。




上述这些步骤也伴随着一定的参数搜索空间。第一种搜索空间是方法自带的，例如PCA自带降维参数需要调整。第二种是特征生成时会将搜索空间扩大。




**模型选择**




**模型选择包括两个步骤：选择一个模型，设定它的参数。**相应地，AutoML的目的就是自动选择出一个最合适的模型，并且能够设定好它的最优参数。




**算法选择**




对于算法选择，**AutoML 的目的是自动地选择出一个优化算法，以便能够达到效率和精度的平衡。**常用的优化方法有 SGD、L-BFGS、GD 等。使用哪个优化算法、对应优化算法的配置，也需要一组搜索空间。




**从全局看**




将以上三个关键步骤整合起来看，**一个完整的 AutoML 过程可以分成这么两类：**一类是将以上的三个步骤整合成一个完整的 pipeline；另一类则是 Network Architecture Search，能够自动地学习到最优的网络结构。在学习的过程中，对以上三个问题都进行一些优化。




# 基本的优化策略




一旦搜索空间确定，我们便可以实用优化器（optimizer）进行优化。**这里，AutoML 主要回答三个问题： **



- 
选择的优化器可以作用在哪个搜索空间上？

- 
它需要什么样的反馈？

- 
为了取得一个好的效果，它需要怎样的配置？ 





简单的优化搜索方式包括 Grid Search 和 Random Search。其中 Grid Search 被广泛使用。 




从样本中进行优化的方法主要包括启发式搜索、derivative-free 优化、以及强化学习方法。梯度下降法是一种重要的优化策略。




# 评价策略




** 基本评价策略**




**在设计评价策略时，AutoML 主要回答三个问题：**



- 
 这种策略能能够快速进行评价吗？

- 
这种策略能够提供准确的评价吗？

- 
这种策略需要怎样的反馈？ 





**基本的评价策略包括：**




**1. 直接评价：**直接在目标数据上进行评价，这是被使用最多的策略；




**2. 采样：**当数据样本量非常大时，采样一些样本进行评价； 




**3. Early Stop：**当遇到一些极端情况使得网络表现效果不好时，可以考虑进行 early stop；




**4. 参数重用：**将之前学习过的参数重复利用在新任务上，这在两种任务配置差不多时可用；




**5. 共轭评价：**对于一些可量化的配置，可以用共轭评价法进行。




**高级评价策略**




**高级评价策略主要包括两种：Meta-learning 和 Transfer Learning。**




**1. Meta-learning 法：**从先前的学习经验中提炼出基本的参数和结构配置；



**2. Transfer learning 法：**从先前的学习经验中提炼出可以重用的一些知识。




# 应用



- 
 使用 Auto-sklearn 进行模型选择；

- 
使用强化学习进行 Neural Architecture Search；

- 
使用 ExploreKit 进行自动特征构建。





# 展望




**未来可能的研究方向：**



- 
提高AutoML的效率；

- 
更明确的问题定义；

- 
发展基本和高级的搜索策略；

- 
找到更适合的应用。


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
[EMNLP 2018论文解读 | 对话生成 & 文本风格转化](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492361&idx=1&sn=af65ef32752cefec12d99727d7fd0867&chksm=96ea3c89a19db59ffead464756c146e53b6ae80519c46c0ed9b3e110e3b0375d7018d6291243&scene=21#wechat_redirect)











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

▽ 点击 | 阅读原文| 下载论文





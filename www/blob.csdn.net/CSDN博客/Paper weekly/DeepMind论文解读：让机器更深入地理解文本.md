# DeepMind论文解读：让机器更深入地理解文本 - Paper weekly - CSDN博客





2018年06月08日 13:18:28[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：184









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnC9iaic8hDbiadLafh7TtCZS6icEYddVmMqZBksDV7cQkKmAu95h53FxyibqmZOS1yQgHibJT0WYD2s1Zw/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **80** 篇文章


本期推荐的论文笔记来自 PaperWeekly 社区用户 **@wutong_SEU**。**DeepMind 提出了一个全新阅读理解数据集 NarrativeQA，机器需要面对的是一整部书籍或电影剧本，在没有限定答案范围的前提下，机器需要从文本中找到最相关的段落并且总结出问题的答案。**

 如果你对本文工作感兴趣，点击底部**阅读原文**即可查看原论文。

# 关于作者：吴桐，东南大学硕士生，研究方向知识图谱。

■ 论文 | The NarrativeQA Reading Comprehension Challenge

■ 链接 | https://www.paperweekly.site/papers/1397

■ 源码 | https://github.com/deepmind/narrativeqa




# 论文导读




相比于信息抽取，阅读理解任务要求机器能够整合篇幅较长的上下文信息（如整篇文章）并能够对事件进行推理。但是现阶段的阅读理解任务仍然能够以一种投机取巧的方法，利用浅层的语言形态学信息（如问答对之间的文本相似性以及整个文章内的统计词频）从问题中直接找到关于答案的蛛丝马迹。




因此**文中提出了一个新的阅读理解数据集****，旨在迫使机器必须通篇阅读书籍或者电影脚本（远远长于一篇文章）才能回答问题**。该数据集更侧重于发掘机器阅读理解对于含蓄的叙述的理解能力而不是基于浅层的模式匹配就能够直接得到答案。




# 工作动机




人在阅读的时候往往通读全篇，并不一定能够记住书中的每一点细节，但是一定能够注意到书中有哪些关键的实体，并且记住这些实体的关系是怎么样的。




但是，现在的阅读理解任务的 benchmark 数据集并不能够针对这点进行测评，相反，多数问题可以通过 question 和 paragraph 之间的 pattern match 得到答案。




因此 DeepMind 提出了这个新的数据集 NarrativeQA，机器需要面对的是一整部书籍或电影剧本，**在没有限定答案范围的前提下，机器需要从文本中找到最相关的段落并且总结出问题的答案**。该任务十分具有挑战性。




那么，**这个新的数据集理论上应该具备以下特征：**



- 
数据量足够大，用以满足神经网络的训练需求；



- 
问题和答案最好都是自然语言，且答案最好和多个片段相关或者需要从一大段话中总结出来；



- 
标注者要自己总结答案，从更高的层面理解文章中实体之间的关系，而且最好使用个性化的语言描述而不是从文中直接截取答案；



- 
从判别器的角度上讲，最好能够从答案的流畅性和正确性等两个角度进行判断；



- 
数据量需要控制在对于当前既有的模型来说十分困难，但仍处在人可以解决的范围内。






# 相关工作




主要提一下三个，**SQuAD**，**MS MARCO **和 **SearchQA**。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkVHynNcnK7U5TVxv6Qov1dC3fB6a8FibW5yERRrRWpHN3ZP5je44ddU5xiamzKSlmMdiczBtjwNabSg/640)



**SQuAD 的场景比较局限**，给定的来源于维基百科短文本，众包标注的问题以及从短文本中直接截取的答案。 **MS MARCO 提出了更开放的数据集**，文本来自搜索引擎，真实的用户问题以及众包标注的答案，但是多数答案仍然不可避免的是文本中的原文，多数在 SQuAD 测评中取得不错成绩的模型在 MS MARCO 上仍然能够取得不俗的结果。**SearchQA 的文本来自搜素引擎，问题来自 Jeopardy**，对，就是当年 Watson 一战成名的那个节目，然后统计发现，数据集中 80% 的答案少于两个单词，99% 的答案少于 5 个单词，抽样调查发现 72% 的问题答案是命名实体。论文作者似乎对这种很不屑，但我想说命名实体是我们这些知识图谱人的心头爱啊。




# 数据集分析




数据集的问答对中主要会出现 Title，Question，Answer，Summary snippet，Story snippet 等字段，其中 title 确定了问答对的出处，即围绕着哪本书或剧本来进行阅读。其中相关片段由人工标注得出，但并不建议在训练中作为标注语料使用，且最终的测试集中不会出现该类片段，需要程序自行设计文本段落的定位方案。 




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkVHynNcnK7U5TVxv6Qov1dI2r0bVIKNKRUUe5tclpOplPEkZL14BGych6kwdC7WU6EOV0o6hu88Q/640)




数据集的主体是小说以及电影剧本，问答集主要根据小说或电影的摘要信息进行提问，如果问答集同时提供了摘要，那么和现在的阅读理解任务也就没有什么不同了，但是，在没有提供摘要及标明相关段落的基础上，回答此类问题需要程序通读整部书籍，整理分析所有相关段落以进行阅读理解。




**任务 **




提供一组测试任务：**分别以摘要和全文作为 context，测试基于答案生成和答案选择的两类阅读理解问题**。测试指标包括 Bleu-1，Bleu-4，Rouge—L 以及基于答案排序的 MRR。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkVHynNcnK7U5TVxv6Qov1djrTD5E3fPnRNJ679WaXSyAfDySuF0zbZzGR91dTvq8k4RS2hfy58uQ/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkVHynNcnK7U5TVxv6Qov1dNibjK5nc2OZvmo9J6MgvNbPaVAGonruSN83LRzAsmTEVAaAdRh3fqBw/640)




# Baseline实验结果




文中提出了三类对比，一将其视为 IR 任务，二直接应用 LSTM 预测后缀词，三在全书范围内 IR+BiDAF。 最终的实验结果证明在全书范围内的阅读理解任务上，暂没有算法能够取得很好的结果。

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)




**点击标题查看更多论文解读：**




- 
[Tree-CNN：一招解决深度学习中的灾难性遗忘](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488661&idx=1&sn=cf7fd1fbfdf347163ea056f0b31bbeba&chksm=96e9cb15a19e420333818d6c6b787ef02e85e44122fee5546f078cc0dfb35b8e82c7bcaa254b&scene=21#wechat_redirect)

- 
[深度神经网络模型压缩和加速都有哪些方法？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488630&idx=1&sn=894b06c31b37ccdad3e9bfdd7323a33f&chksm=96e9cbf6a19e42e0c666d6727430a39fe4e09db047c3cfc0465a34923b87a36dfbe7585fe339&scene=21#wechat_redirect)

- 
[上海交大提出多模态情绪识别框架EmotionMeter](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489737&idx=1&sn=75e03aa485c5baa2b0dcec3f58bd782c&chksm=96e9c749a19e4e5f9e63b8f438fe773260912736fef9231fa49e1baa8ee5e3066b6da0a2f764&scene=21#wechat_redirect)

- 
[综述：图像风格化算法最全盘点](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489172&idx=1&sn=42f567fb57d2886da71a07dd16388022&chksm=96e9c914a19e40025bf88e89514d5c6f575ee94545bd5d854c01de2ca333d4738b433d37d1f5&scene=21#wechat_redirect)

- 
[5 篇 AAAI 2018 论文看「应答生成」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489246&idx=1&sn=a5180ae78bb1a072d4106794dbfb389a&chksm=96e9c95ea19e40485402a274cd4751b10e282e86ff8d582d2b000ce8665662d07872afc0e561&scene=21#wechat_redirect)

- 
[深度协同过滤：用神经网络取代内积建模](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489455&idx=1&sn=a4800237744e79aee2caced488e8e772&chksm=96e9c82fa19e413987a8f2bf5216c95e3df879bef5c66f579687fb0b094240c7ec16d93d2d63&scene=21#wechat_redirect)





[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488603&idx=2&sn=7320cb23efba3e7b5a381be83b7fe3ad&chksm=96e9cbdba19e42cd5840d3d51e86da4709b3d5273b2cf2512c32d84ab2b42ac4e7f13bf9ba63&scene=21#wechat_redirect)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgmsvubgibQtWV5t7M3ETKt3bbXiaAothCErMicibic9QCUBpxkuibuht62MGcCTcLyAxqGrsUXbv254InDA/640?)

**▲**戳我查看招募详情




**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****作 者 招 募#**



****[让你的文字被很多很多人看到，喜欢我们不如加入我们](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)****







******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)

▽ 点击 | 阅读原文| 查看原论文





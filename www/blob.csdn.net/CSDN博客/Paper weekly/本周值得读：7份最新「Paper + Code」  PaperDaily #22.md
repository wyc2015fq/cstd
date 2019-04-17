# 本周值得读：7份最新「Paper + Code」 | PaperDaily #22 - Paper weekly - CSDN博客





2017年12月05日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：160












![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。

这是 PaperDaily 的第 **22** 篇文章

[ 自然语言处理 ]

![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/?wxfrom=5&wx_lazy=1)

**SQLNet: Generating Structured Queries From Natural Language Without Reinforcement Learning**

**@guotong1988 推荐**

#Seq2Seq

Text2SQL，根据自然语言生成结构化 SQL 语句。

论文链接

http://www.paperweekly.site/papers/1273



代码链接

https://github.com/xxj96/SQLNet




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GialBA3SGMBbvEWztUWZZnGKibn1SyAZMgNoXNIgJA7ZH4WfFz0UVCMNMA/?)
**Dynamic Evaluation of Neural Sequence Models**

**@utopiar 推荐**

#Recurrent Neural Networks

本文提出使用动态评估来改进神经序列模型的性能。 模型通过基于梯度下降的机制适应最近的历史，将以更高概率分配给重新出现的连续模式。动态评估将 Penn Treebank 和 WikiText-2 数据集上的 perplexities 分别提高到 51.1 和 44.3。

论文链接

http://www.paperweekly.site/papers/1279



代码链接

https://github.com/benkrause/dynamic-evaluation




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccHF3Xj4iay15TYLx4MJAaFa4apU5ncT1TS28jCmQ2Sn6oq3mrPdCTsIg/?)
**Exploring the Syntactic Abilities of RNNs with Multi-task Learning**

**@Andy 推荐**

#Multi-task Learning

本文主要通过了对一致性预测还有 CCG 超标记进行多任务学习，来展示了如何通过多任务学习，让 RNN 能够学习出更加复杂的句法表达。此外还发现利用一致性预测的训练数据，也可以提高其他只有有限训练数据任务的表现。这样的多任务模型，还可以将语法知识注入进语言模型中去（当然也包括机器翻译）。

论文链接

http://www.paperweekly.site/papers/1235



代码链接

https://github.com/emengd/multitask-agreement







[ 计算机视觉 ]




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnlMRddFSicHu1qEmNic9yDicc3CcQJ8M977xicxXj2oGmSGmgpL9uejSPDxQSjiaCcP2tZzmRjP2oq27Q/?)
**StarGAN: Unified Generative Adversarial Networks for Multi-Domain Image-to-Image Translation**

**@paperweekly 推荐**

#GAN

Reddit 热文，利用 GAN 自定义表情和面部特征。StarGAN 是一种全新的可扩展方法，仅使用一个模型就能为多个域执行图像到图像转换。

论文链接

http://www.paperweekly.site/papers/1227



代码链接

https://github.com/yunjey/StarGAN







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccxXLfTzty6ozNicib3ANTEBo7ib3yibibA0DkcvtDADADpbhqhGibpdYk5kEw/?)
**What is the Role of Recurrent Neural Networks (RNNs) in an Image Caption Generator?**

**@jamiechoi 推荐**

#Image Captioning

文章比较了 RNN 在 image caption 里的两种作用：1. Inject模型：RNN 用来 encode 文字和图片；2. Merge 模型：RNN 只 encode 文字，图片在较后的阶段再与文字信息结合。最后得出结论，Merge 模型较好。

论文链接

http://www.paperweekly.site/papers/1200



代码链接

https://github.com/mtanti/rnn-role







[ 机器学习 ]




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccjicqEh7pRt1YmhJDYezDjG6hX2Qm8VKCXFXBGwApdSJHwxhu4j8tTicQ/?)
**Dilated Recurrent Neural Networks**

**@snowwalkerj 推荐**

#RNN

本文提出的 dilated RNN，解决了长程依赖的问题，同时可提高并行率，提高计算速度；提出 Mean Recurrent Length 指标，衡量 RNN 可记忆的时间长度，仅用普通的 RNN 模块在多项任务上达到 state of art 效果。

论文链接

http://www.paperweekly.site/papers/1220



代码链接

https://github.com/code-terminator/DilatedRNN






![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglcUOAZoh5iaCiaLHocEdcK9Ys8liceIg6QBEbn8ON4ZGpsa5cWpiaLX5tby5WEiaqy1UVVoNxKH0Zh93A/?)
**Multi-Rate Deep Learning for Temporal Recommendation**

**@Ttssxuan 推荐**

#LSTM

使用多种时间粒度进行用户行为预测，并为大量数据提出类似词嵌的预训方法。 使用 DSSM 建模用户静态兴趣，使用一个 LSTM 时序网络建模用户短期兴趣，使用一个 LSTM 时序网络建模全局用户兴趣。

论文链接

http://www.paperweekly.site/papers/1208



代码链接

https://github.com/sonyisme/keras-recommendation




**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**



我是彩蛋

**解锁新功能：热门职位推荐！**




PaperWeekly小程序升级啦




**今日arXiv√猜你喜欢√**热门职位****√****




找全职找实习都不是问题



** 解锁方式 **

1. 识别下方二维码打开小程序

2. 用PaperWeekly社区账号进行登陆

3. 登陆后即可解锁所有功能




** 职位发布 **

请添加小助手微信（**pwbot01**）进行咨询




**长按识别二维码，使用小程序**

*点击阅读原文即可注册




![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)





******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)





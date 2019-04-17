# 开源代码上新！6 份最新「Paper + Code」 | PaperDaily #17 - Paper weekly - CSDN博客





2017年11月23日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：548












![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。

这是 PaperDaily 的第 **17** 篇文章[ 自然语言处理 ]
![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/?wxfrom=5&wx_lazy=1)
**DuReader: a Chinese Machine Reading Comprehension Dataset from Real-world Applications**

**@zhangjun 推荐**

#Machine Reading Comprehension

大型中文阅读理解数据集，工作来自百度。

论文链接

https://www.paperweekly.site/papers/1137



代码链接

https://github.com/baidu/DuReader




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GialBA3SGMBbvEWztUWZZnGKibn1SyAZMgNoXNIgJA7ZH4WfFz0UVCMNMA/?)
**TransNets - Learning to Transform for Recommendation**

**@ccclyu 推荐**

#Transfer Learning

引入了迁移学习的隐含表示层来减少 test dataset 的稀疏性。

论文链接

https://www.paperweekly.site/papers/1181



代码链接

https://github.com/rosecatherinek/TransNets



[ 计算机视觉 ]
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccHF3Xj4iay15TYLx4MJAaFa4apU5ncT1TS28jCmQ2Sn6oq3mrPdCTsIg/?)
**Co-attending Free-form Regions and Detections with Multi-modal Multiplicative Feature Embedding for Visual Question Answering**

**@xiaolu 推荐**

#Visual Question Answering

视觉问答任务（Viual Question Answering）是最近 2-3 年兴起的多模态任务。对于视觉问答任务，算法需要同时理解图片内容和语言信息，然后推理出正确的答案。目前常见的方法有多模态特征融合和以及视觉注意力机制。

已有的注意力机制主要有两类，一类是基于全图区域的注意力机制，一类是基于检测框区域的注意力机制，前者容易关注到物体的一部分，后者会缺少一些检测框。 

本文第一次尝试将基于全图区域的与基于检测框区域的注意力机制融合在一个框架下，充分利用全图区域和检测框之间的互补信息；并且提出新的多模态特征融合方法，将问题特征、全图特征、检测框特征进行有效的融合。模型在 VQA 和 COCO-QA 两个主流数据集上实现了目前最好的结果。

论文链接

https://www.paperweekly.site/papers/1175



代码链接

https://github.com/lupantech/dual-mfa-vqa




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnlMRddFSicHu1qEmNic9yDicc3CcQJ8M977xicxXj2oGmSGmgpL9uejSPDxQSjiaCcP2tZzmRjP2oq27Q/?)
**Image Crowd Counting Using Convolutional Neural Network and Markov Random Field**

**@cornicione 推荐**

#People Counting

文章融合了 MRF 和 deep 网络，提出了一种统计静态图片中人数的方法。

论文链接

https://www.paperweekly.site/papers/1195



代码链接

https://github.com/hankong/crowd-counting



[ 机器学习 ]
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccxXLfTzty6ozNicib3ANTEBo7ib3yibibA0DkcvtDADADpbhqhGibpdYk5kEw/?)
**Social Attention - Modeling Attention in Human Crowds**

**@zk5580752 推荐**

#Attention Model

本文是一篇基于 Social LSTM 的后续论文，Social LSTM 是李飞飞团队于 2016 年提出的行人路径预测方法。本文则在原文基础上改进了预测方式，不仅仅基于 RNN 模型，还引入了 Attention Model，使得预测更加精准和全面。

论文链接

https://www.paperweekly.site/papers/1115



代码链接

https://github.com/vvanirudh/social-lstm-tf




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnlMRddFSicHu1qEmNic9yDiccjicqEh7pRt1YmhJDYezDjG6hX2Qm8VKCXFXBGwApdSJHwxhu4j8tTicQ/?)
**Breaking the Softmax Bottleneck: A High-Rank RNN Language Model**

**@paperweekly 推荐**

#RNN

本文尝试改进了语言模型在预测 next token 时用 softmax 遇到的计算瓶颈。

论文链接

https://www.paperweekly.site/papers/1112



代码链接

https://github.com/zihangdai/mos




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





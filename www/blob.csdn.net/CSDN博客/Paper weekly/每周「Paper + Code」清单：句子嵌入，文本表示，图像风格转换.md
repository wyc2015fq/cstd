# 每周「Paper + Code」清单：句子嵌入，文本表示，图像风格转换 - Paper weekly - CSDN博客





2017年12月21日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：999












![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **29** 篇文章
[ 自然语言处理 ]



![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/?wxfrom=5&wx_lazy=1)
**Sockeye: A Toolkit for Neural Machine Translation**

**@zhangjun 推荐**

#Neural Machine Translation

一个开源的产品级神经机器翻译框架，构建在 MXNet 平台上。

论文链接

https://www.paperweekly.site/papers/1374

代码链接

https://github.com/awslabs/sockeye




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/?)
**Multilingual Hierarchical Attention Networks for Document Classification**

**@miracle 推荐**

#Text Classification

本文使用两个神经网络分别建模句子和文档，采用一种自下向上的基于向量的文本表示模型。首先使用 CNN/LSTM 来建模句子表示，接下来使用双向 GRU 模型对句子表示进行编码得到文档表示。

 论文链接

https://www.paperweekly.site/papers/1152

代码链接

https://github.com/idiap/mhan




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/?)

**Supervised Learning of Universal Sentence Representations from Natural Language Inference Data**

**@zxEECS 推荐**

#Natural Language Generation

本文来自 Facebook AI Research。本文研究监督句子嵌入，作者研究并对比了几类常见的网络架构（LSTM，GRU，BiLSTM，BiLSTM with self attention 和 Hierachical CNN）, 5 类架构具很强的代表性。

论文链接

https://www.paperweekly.site/papers/1332

代码链接

https://github.com/facebookresearch/InferSent







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/?)
**Recurrent Neural Networks for Semantic Instance Segmentation**

**@paperweekly 推荐**

#Recurrent Neural Networks

本项目提出了一个基于 RNN 的语义实例分割模型，为图像中的每个目标顺序地生成一对 mask 及其对应的类概率。该模型是可端对端 + 可训练的，不需要对输出进行任何后处理，因此相比其他依靠 object proposal 的方法更为简单。

论文链接

https://www.paperweekly.site/papers/1355

代码链接

https://github.com/facebookresearch/InferSent







[ 计算机视觉 ]




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/?)
**Deep Image Prior**

**@AkiyamaYukari 推荐**

#Image Style Transfer

本文视角独特，效果不错。作者认为模型可以不通过对于数据集上进行学习和预训练就能实现图像转换任务（如去噪、超分等），仅需调节超参数（如网络训练次数、学习率等）。

论文链接

https://www.paperweekly.site/papers/1295

代码链接

https://github.com/DmitryUlyanov/deep-image-prior







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/?)
**AlignedReID: Surpassing Human-Level Performance in Person Re-Identification**

**@Molly 推荐**

#Person Re-identification

本文来自 Face++，作者引入端到端的方法，让网络自动去学习人体对齐，从而提高性能。除了提取全局特征，同时也对各局部提取局部特征。

论文链接

https://www.paperweekly.site/papers/1242

代码链接

https://github.com/huanghoujing/AlignedReID-Re-Production-Pytorch







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaPFKKcQtakJkrJqiay67wic4UibzIoJEFesEXzBdgvbKrU3pQbdFib3fEibQ/?)
**When Unsupervised Domain Adaptation Meets Tensor Representations**

**@jindongwang 推荐**

#Domain Adaptation

当传统的基于 vector 的 domain adaptation 应用于 tensor，会发生什么？这个文章发表在 ICCV 2017 上，很有指导意义。

论文链接

https://www.paperweekly.site/papers/1160

代码链接

https://github.com/poppinace/TAISL







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkFFwOYfF41kUMpx9QrhiacsUsaDJKp7afm9SFyWQgUwpPItQo9UH0zH3GFjzTw4LJezwoExXFCzpA/?)
**Unpaired Image-to-Image Translation using Cycle-Consistent Adversarial Networks**

**@zh794390558 推荐**

#Generative Adversarial Networks

本文提出的模型名为 CycleGAN，作者希望在不借助 paired example 情况下，来实现图片的风格转换。

论文链接

https://www.paperweekly.site/papers/807

代码链接

https://github.com/junyanz/pytorch-CycleGAN-and-pix2pix







[ 机器学习 ]




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmdTOOt6xMGO8q70RUDWEzldUCgrhG69LwL4uQCmiaK8A19jufpMJdHTxzZ5KGRrJNpEgnHJRfNk1A/?)
**Structural Deep Network Embedding**

**@YFLu 推荐**

#Representation Learning

SDNE 是清华大学崔鹏老师组发表在 2016KDD 上的一个工作，目前谷歌学术引用量已经达到了 85，是一篇基于深度模型对网络进行嵌入的方法。

SDNE 模型同时利用一阶相似性和二阶相似性学习网络的结构，一阶相似性用作有监督的信息，保留网络的局部结构；二阶相似性用作无监督部分，捕获网络的全局结构，是一种半监督深度模型。

论文链接

https://www.paperweekly.site/papers/1142

代码链接

https://github.com/xiaohan2012/sdne-keras







![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm3OQD8fU9b5nyluFPpOop0o6c9fGMicicnQyd57ibficujqN6StPyHBIib7ktB5sX5IHNWSyXE77dry4w/?)
**Deep Spatio-Temporal Residual Networks for Citywide Crowd Flows Prediction**

**@cornicione 推荐**

#Convolutional Neural Network

论文提出一种从宏观预测城市人口的网络 — ST-ResNet，利用 3 个网络分支分别对 3 种时间特性进行建模：时间紧密度，周期，趋势。结合外界因素，对不同地区分配不同的权重，将 3 个分支进行动态融合。

论文链接

https://www.paperweekly.site/papers/1382

代码链接

https://github.com/lucktroy/DeepST




** 本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**



我是彩蛋




** 解锁新功能：热门职位推荐！**




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





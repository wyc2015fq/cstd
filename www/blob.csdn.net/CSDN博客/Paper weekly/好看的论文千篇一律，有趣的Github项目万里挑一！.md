# 好看的论文千篇一律，有趣的Github项目万里挑一！ - Paper weekly - CSDN博客





2018年03月29日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：7967












![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **55** 篇文章
![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640?wxfrom=5&wx_lazy=1)
**InsightFace**


#基于MXNet的人脸识别开源库

![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXcRyrgKvvEunj71RbtqJC73wzxF0wnFSYRoO3SUbHud3CKehaicz43Mw/640?wxfrom=5&wx_lazy=1)

**InsightFace 是 DeepInsight 实验室对其论文 *ArcFace: Additive Angular Margin Loss for Deep Face Recognition* 的开源实现**。本文工作将 MegaFace 的精度提升到 98%，超过俄罗斯 Vocord 公司保持的 91% 的纪录。

此外，该项目**还包括打包对齐好的人脸训练数据**（MS1M 和 VGG2）、**网络框架**（ResNet，InceptionResNet_v2，DenseNet，DPN 和 MobiletNet）和 **loss 设计**（Softmax，SphereFace，AMSoftmax，CosFace 和 Triplet Loss）。


在此基础上，研究人员可以专注于人脸识别的算法实验，工业界也可以方便地根据自身需求训练，或者凭借该项目提供的高精度预训练模型进行产品化。

论文链接

https://www.paperweekly.site/papers/1785




项目链接

https://github.com/deepinsight/insightface




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/640?)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXcWYGpT1SPJQLsVOfCU3tqKIhfFoXGacZ64GgPIm3wzVAwNibLzr71HQ/640)

#Python图像增强库

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXKZbWvcias0lvuic8hdgDY1icBaj3ibXurADUsYUAa5Niciayr2vGCZaSCNWA/640)

**Augmentor 是一个 Python 图像增强库**，减少了使用图像库自己编写代码的繁杂工序，能够批量完成图像的旋转，放大，缩小，添加噪音以扩充数据量。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXSo1JEN8HBgUp6sicCr6jibSgHsDib3ib4hXnlhqboA5c6jvPoEB63lrLiaA/640)

**▲ **输入图像




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXasnnLgHuRa0xLvXoBFrRJb3ndkZXYGIQXLEqGacXF88Up69SsVRhbg/640)

**▲ **输出图像




项目链接

https://github.com/mdbloice/Augmentor




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/640?)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX67EkX4X4k3UzsicA21khfkWJmytiaDzemoGYf2F5bAp7IlwdZb1wEBrw/640)

#序列距离测算

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX8tFicSpicibzdM3QibfLA3sGn701sLbhZVuibxnWoumMaiabe9ibKNSNv2bEw/640)

**TextDistance 是一个包含 30+ 种算法的 Python 库**，用于计算两个或多个序列之间的距离。

**项目特性如下：**
- 
30+ 种算法

- 
纯 Python 实现

- 
操作简便

- 
支持两种以上序列对比

- 
部分算法具备多种实现

- 
支持 Numpy 获取最大速度优化


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXHbBgoAtROBxWQ7dIjKpHwVfrEibQqCIiaiaE5pHygaADT0SntzjPJ9aTg/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXvvquUYvpKdQHe6btfnKApCf7Ney4jrbM29v7rduZUhUx96v9zppbYw/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXTTiaZ1FibcJbKyc17roHpMJY6uWWGuNaiaEaPpJgm7OmAVicatOEEP4OFQ/640)




项目链接

https://github.com/orsinium/textdistance







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/640?)
**Neural Network Voices**
#模仿Kate Winslet说话




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXmr4Kg1ZnficPZktagKycNwb3V8Nib2Q9AlBBdhuRcj52XibgLdmXVI1tg/640)

本项目是 Siraj Raval 在 YouTube 上发布的神经网络语音合成教学视频对应代码，**如何使用深度神经网络将普通人的声音转化为英国著名女演员 Kate Winslet 的声音**。本项目使用的数据集为 Kate Winslet 朗读的有声书音频。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXVssYf7dw57ctez7h69rwIQjTARKGX1IibCpv5xSwgKs1OpaqUzCkAgQ/640)






教学视频

https://youtu.be/6KHSPiYlZ-U




项目链接

https://github.com/llSourcell/Neural_Network_Voices







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/640?)
**Personae**
#强化、监督学习在金融市场中的应用




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX38znSKTrw9rIItv43trA5LjRIM9jN730h3ZBibHHIZ5icHibtG38cIia0w/640)

**Personae 基于 TensorFlow 和 PyTorch 对深度强化学习、监督学习算法和论文进行实现，并尝试将其应用于金融市场（股市）**。该项目已实现的算法包含 DDPG，Policy Gradient 和 DualAttnRNN。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX6picveKw3bCQsdVpWYfHiaUqmZgrCYplA4HuL8W1D2ZCibFkDcfZ9QjgQ/640)

**▲ **股票交易收益对比




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXo6C5Ex8GonmicOFuBYc6TF2qdLojHKwPqZt4L2FItas6GMPOESnokxQ/640)

**▲ **股价预测结果




项目链接

https://github.com/ceruleanacg/Personae







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/640?)
**NNDial**
#端到端对话系统开源工具包




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXuQwNmtG6dricV7orrYqtjHNsyljia11G4k5GYKfL5cJgxvewSB7qFBkQ/640)

**NNDial 是一个用于构建端到端可训练任务型对话模型的开源工具包**，本项目来自剑桥大学，使用的数据集为 CamRest676。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXVbgpTjPpiaYzEs9VKXEZ6iciaPfP6qkFeiaHVu9ichoU9M0Cias1ZrkkvhMQ/640)

**▲ **测试结果




项目链接

https://github.com/shawnwun/NNDIAL







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkeCLYjA1vYQpa7qzHEk9oZb2HGibDxo89Ab7UOQQwsMf2KzxG1lSxicwibTbXG3svE3E2fan20MaZRw/640)
**Voice Activity Detection Toolkit**
#语音端点检测工具包




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXteK3ibQtSWeWtbsgo8JDIpQBVLagck7I4NTER2HgIarlyibuJFlicM5Xg/640)

**本项目是论文 *****Voice Activity Detection Using an Adaptive Context Attention Model***** 的开源实现**，此外还包含作者团队自行录制的语音数据集。

**该工具包支持 4 种基于 MRCG 的分类器：**
- 
自适应上下文注意模型（ACAM）

- 
增强型深度神经网络（bDNN）

- 
深度神经网络（DNN）

- 
基于LSTM的循环神经网络（LSTM-RNN）


项目链接

https://github.com/jtkim-kaist/VAD







![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkeCLYjA1vYQpa7qzHEk9oZ1dt1DSUEhn3xpLU5NEuFKVJCQfVH9JyicWQibcMq887OqfpMC0H6kARA/640)
**Knowledge Graph Representation**
#用PyTorch实现知识图谱表示




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXK0v1r20AzW7HdIBNasNvzUF1MqH6h9l9wiczWxkw5KAlrguSfhXUFPA/640)

**该项目整理了知识图谱表示常用的四个数据集，提供了数据清洗整理的代码，用 PyTorch 实现了四种基于平移的算法**。在评估阶段，使用了多进程加速，将 MeanRank 和 Hits@10 的评估时间压缩到小于 1 分钟。

项目链接

https://github.com/jimmywangheng/knowledge_representation_pytorch






![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXzdnKGzJ0RHvb6EgAONW4FWNhapGT3BMRNOHawvvWvumyAEm3icbPq6w/640)
**PyHanLP**
#HanLP的Python接口




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXibb2pPqE8fSO7xWgFgaeib4LgRKE60m1RRicEAkVSDSpNWuYT75yeNvQQ/640)

**本项目是汉语言处理包 HanLP 的 Python 接口**，支持自动下载与升级 HanLP，兼容 py2 和 py3。

**HanLP 是由一系列模型与算法组成的 Java 工具包，目标是普及自然语言处理在生产环境中的应用**。HanLP 具备功能完善、性能高效、架构清晰、语料时新、可自定义的特点。

HanLP 可提供中文分词、词性标注、命名实体识别、关键词提取、文本推荐、依存句法分析、文本分类、word2vec 和语料库工具等功能。

HanLP主页

http://www.hankcs.com/nlp/hanlp.html




项目链接

https://github.com/hankcs/pyhanlp




**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击****「****************阅读原文****************」****即刻加入社区！**

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgmsvubgibQtWV5t7M3ETKt3bbXiaAothCErMicibic9QCUBpxkuibuht62MGcCTcLyAxqGrsUXbv254InDA/640?)




**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****作 者 招 募#**



****[让你的文字被很多很多人看到，喜欢我们不如加入我们](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)****







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

请添加小助手微信（**pwbot02**）进行咨询




**长按识别二维码，使用小程序**

*点击阅读原文即可注册







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/640?)










**********关于PaperWeekly**********




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)




▽ 点击 | 阅读原文| 加入社区刷论文





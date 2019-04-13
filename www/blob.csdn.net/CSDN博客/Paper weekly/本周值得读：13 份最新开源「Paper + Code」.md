
# 本周值得读：13 份最新开源「Paper + Code」 - Paper weekly - CSDN博客


2018年01月19日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1152



![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**35**篇文章
[ 自然语言处理 ]
![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/?wxfrom=5&wx_lazy=1)
**Adversarially Regularized Autoencoders**
**@rqyang 推荐**
\#Natural Language Generation
本文探讨了用 GAN 作为 autoencoder 的 regularizer 的方法及其在文本生成和离散图像生成中的应用。
此类方法可以同时得到一个文本生成模型和一个高质量的自编码器：编码空间受到 GAN loss 的约束后，相似离散结构的编码也会比较类似。
方法稍加扩展则可以用作离散结构的 unaligned style transfer，并取得了这方面的 state-of-the-art results。
论文链接
https://www.paperweekly.site/papers/1522
代码链接
https://github.com/jakezhaojb/ARAE

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/?)
**Context-Dependent Sentiment Analysis in User-Generated Videos**
**@chenbjin 推荐**
\#Multimodal Sentiment Analysis
传统多模态情感分析是句子独立的，也就是对每个句子进行多模特征融合，分析情感极性。而该文则尝试利用双层的 LSTM 来融合当前句子的上下文信息，并且实验对比了不同 LSTM 模型的效果，最后在 MOSI 和 IEMOCAP 数据集上都达到 state-of-art 的效果。
论文链接
https://www.paperweekly.site/papers/1470
代码链接
https://github.com/SenticNet/contextual-sentiment-analysis

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/?)
**The NarrativeQA Reading Comprehension Challenge**
**@zhangjun 推荐**
\#Machine Reading Comprehension
由 DeepMind 发布的全新机器阅读理解数据集 NarrativeQA，其难度和复杂度都进行了全面升级。
论文链接
https://www.paperweekly.site/papers/1397
代码链接
https://github.com/deepmind/narrativeqa


![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/?)
**Replace or Retrieve Keywords In Documents at Scale**
**@paperweekly 推荐**
\#Keywords Replacing
本文提出了一个高效的关键词替换和抽取算法 FlashText，该算法的时间复杂度不依赖于搜索或替换的字符的数量。
论文链接
https://www.paperweekly.site/papers/1349
代码链接
https://github.com/vi3k6i5/flashtext


![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/?)
**Chinese LIWC Lexicon Expansion via Hierarchical Classification of Word Embeddings with Sememe Attention**
**@zhangjun 推荐**
\#Word Embeddings
本文尝试了利用词语表示学习与 HowNet 知识库进行词典扩展。词典扩展任务旨在根据词典中的已有词语，自动扩展出更多的相关词语。本文已被 AAAI 2018 录用。
论文链接
https://www.paperweekly.site/papers/1499
代码链接
https://github.com/thunlp/Auto_CLIWC


[ 计算机视觉 ]

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/?)
**Multi-Scale Dense Networks for Resource Efficient Image Classification**
**@paperweely 推荐**
\#Image Classification
本文为 ICLR 2018 最佳论文。文章基于 CNN，提出一种图像分类方法：在现实中对图像分类难度不一，采用一个固定的框架对图片进行分类时有时不够灵活，比如使用复杂网络对简单图片分类比较浪费资源，而使用简单网络对复杂图片进行分类效果又不够好。
论文链接
https://www.paperweekly.site/papers/1554
代码链接
https://github.com/gaohuang/MSDNet


![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaPFKKcQtakJkrJqiay67wic4UibzIoJEFesEXzBdgvbKrU3pQbdFib3fEibQ/?)
**Deep Hashing Network for Unsupervised Domain Adaptation**
**@jindongwang 推荐**
\#Domain Adaptation
CVPR 2017 第一篇用深度网络来学习 hash code 的文章。本文贡献了一个新的 domain adaptation 数据集：Office-home，包含 65 类 object。本文也是首次用深度网络以 domain adaptation 的方式学习 hash code 的文章。
论文链接
https://www.paperweekly.site/papers/1241
代码链接
https://github.com/hemanthdv/da-hash


![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkFFwOYfF41kUMpx9QrhiacsUsaDJKp7afm9SFyWQgUwpPItQo9UH0zH3GFjzTw4LJezwoExXFCzpA/?)
**Visual Attribute Transfer through Deep Image Analogy**
**@rozentill 推荐**
\#Image Style Transfer
本文提出了一种 non-parametric 的 style transfer 算法，在保持全局结构的前提下，将 visual attribute 进行迁移，得到了不同于目前绝大多数 style transfer 的 promising 的结果，丰富了 style transfer 的应用场景。
论文链接
https://www.paperweekly.site/papers/1517
代码链接
https://github.com/msracver/Deep-Image-Analogy


![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmdTOOt6xMGO8q70RUDWEzldUCgrhG69LwL4uQCmiaK8A19jufpMJdHTxzZ5KGRrJNpEgnHJRfNk1A/?)
**A Simple Yet Effective Baseline for 3D Human Pose Estimation**
**@duinodu 推荐**
\#Human Pose Estimation
本文设计并分析了一个神经网络结构，能比之前已有的方法更好地估计 3D 坐标，不仅精度高，速度也快。从应用角度上说，非常具有应用价值。以 RGB 图片作为输入，大大扩大了应用场景。
估计 3D 人体姿态，相比于 2D 来说，3D 包含了人体姿态的全部信息，能够从 3D 数据中解析出更多有使用价值的信息。所以从实际的角度出发，这项技术很有价值。
论文链接
https://www.paperweekly.site/papers/1487
代码链接
https://github.com/una-dinosauria/3d-pose-baseline


![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm3OQD8fU9b5nyluFPpOop0o6c9fGMicicnQyd57ibficujqN6StPyHBIib7ktB5sX5IHNWSyXE77dry4w/?)
**End-to-End Multimodal Emotion Recognition using Deep Neural Networks**
**@chenbjin 推荐**
\#Multimodal Emotion Recognition
本文提出一个端到端的多模态（音视频）情绪识别系统，分别用 CNN 和 ResNet 抽取音频特征和图像特征，再拼接作为双层的 LSTM 的输入，以最大化一致性相关系数（concordance correlation coefficient）为目标进行训练，在 RECOLA 数据集上取得不错的效果。 文章信息量很多，还有一些音频和图像的处理细节。
论文链接
https://www.paperweekly.site/papers/1507
代码链接
https://github.com/tzirakis/Multimodal-Emotion-Recognition


[ 机器学习 ]

![VBcD02jFhgmxaVWCSvbUm33AIYAPBiavKMSCYbefTicZUfrLCCdoadxnetQ6nibHngicUJtxHuEqUE6kO9xq38YIww](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmxaVWCSvbUm33AIYAPBiavKMSCYbefTicZUfrLCCdoadxnetQ6nibHngicUJtxHuEqUE6kO9xq38YIww/)
**DeepMind Control Suite**
**@paperweekly 推荐**
\#Reinforcement Learning
DeepMind 最近开源的强化学习环境 Control Suite 相比 OpenAI Gym 拥有更多的环境，更易于阅读的代码文档，同时更加专注于持续控制任务。它基于 Python，由 MuJoCo 物理引擎支持，是一套强大的强化学习智能体性能评估基准。
论文链接
https://www.paperweekly.site/papers/1556
代码链接
https://github.com/deepmind/dm_control


![VBcD02jFhgmxaVWCSvbUm33AIYAPBiavKxiatAUxAzfMGJHVibiaKeMuN6ATCCdtxT8ZkePmEo9XpI8sibzI9mGQcvw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmxaVWCSvbUm33AIYAPBiavKxiatAUxAzfMGJHVibiaKeMuN6ATCCdtxT8ZkePmEo9XpI8sibzI9mGQcvw/)
**Deep Complex Networks**
**@duinodu 推荐**
\#Human Pose Estimation
本文依靠复数卷积，提出了可用于复数值深度神经网络的复数批规范化、复数权重初始化策略，并且还在端到端的训练方案中对它们进行了实验。实验表明，这样的复数值模型可以实现与其对应的实数值模型相媲美或更好的表现。
作者在一些计算机视觉任务和使用 MusicNet 数据集的音乐转录任务上对深度复数模型进行了测试，实现了当前最佳的表现。
论文链接
https://www.paperweekly.site/papers/1566
代码链接
https://github.com/ChihebTrabelsi/deep_complex_networks


![VBcD02jFhgmxaVWCSvbUm33AIYAPBiavKkx0ln3grgKNt6oaibWbAGibRD4SDU1qvE4DQiaWV5FHelaQibGiawn61D5g](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmxaVWCSvbUm33AIYAPBiavKkx0ln3grgKNt6oaibWbAGibRD4SDU1qvE4DQiaWV5FHelaQibGiawn61D5g/)
**Structured Bayesian Pruning via Log-Normal Multiplicative Noise**
**@paperweekly 推荐**
\#Bayesian Networks
NIPS 2017 录用论文，通过对数正态乘法噪声构造贝叶斯修剪。
论文链接
https://www.paperweekly.site/papers/1565
代码链接
https://github.com/necludov/group-sparsity-sbp

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnED8CgalBpSNcrqdibvRwqEDUqVM5xyf5GmoJgyyIiblwUtgW1MfjJrIYIqXRoaK4uapnvBicXsUB8g/?)
△ 戳我领取新年礼物

投票参与 & 读者福利
**参与方式**

1. 长按识别下方二维码参与投票
2. 文末留言你喜欢某篇论文的原因
3. 分享本文到朋友圈并截图发至后台

**截止时间**

2018年1月24日0点0分

**福利清单**

PaperWeekly定制手机壳 x 3份
PaperWeekly定制笔记本 x 5份
PaperWeekly定制行李牌 x 10份

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnED8CgalBpSNcrqdibvRwqESxVO3lbKOIEIBdyzBIykjB2Vq91YlFTibBUylIC8ItotiadPyluHEn0w/?)
△ 我们长这样哦~

长按扫描二维码，参与投票*！*
▼
![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnED8CgalBpSNcrqdibvRwqE8zf7FlfQqgZERt7Z7vW9KJCPpRYl4BicGLProOXeYhV2SJOn6ibtV7yA/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/?)**\#****高 能 提 醒****\#**

1. 为了方便大家在投票过程中查看论文详情，**请勿使用微信内置浏览器**。点击页面右上角的“…”按钮，在手机浏览器中打开表单。

2. 本次评选包含**自然语言处理**和**计算机视觉**两大方向，请在你所选择的参与方向下**勾选3-10篇论文**。

3. 获奖名单将于1月25日公布，其中5位由小编根据文末留言选取，其他13位采用随机抽取，礼物随机发放。


**长按扫描二维码，****马上投票****！**
▼
![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnED8CgalBpSNcrqdibvRwqE8zf7FlfQqgZERt7Z7vW9KJCPpRYl4BicGLProOXeYhV2SJOn6ibtV7yA/?)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)




# “Paper + Code”才是研读论文的正确姿势 | PaperDaily \#02 - Paper weekly - CSDN博客


2017年09月30日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1574



![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/0?wx_fmt=jpeg)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**2**篇文章
[ 机器学习 ]![640?wx_fmt=png&wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1&retryload=1)
**Neural Factorization Machines for Sparse Predictive Analytics**
**@somtian 推荐**
\#factorization machine
将推荐系统中的 FM 模型通过神经网络进行解释和建模，考虑了所有特征表示向量之间的 pair-wise 交互关系。
论文链接
http://www.paperweekly.site/papers/792
代码链接
http://github.com/hexiangnan/neural_factorization_machine

[ 自然语言处理 ]![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GialBA3SGMBbvEWztUWZZnGKibn1SyAZMgNoXNIgJA7ZH4WfFz0UVCMNMA/0?wx_fmt=png)
**Long Text Generation via Adversarial Training with Leaked Information**
**@Maniac 推荐**
\#GAN
BLEU score 相比 AAAI17 的 seqGAN 有大幅度的提升。
论文链接
http://www.paperweekly.site/papers/771
代码链接
https://github.com/CR-Gjx/LeakGAN

![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/0?wx_fmt=png)
**Neural Architectures for Named Entity Recognition**
**@yanyu 推荐**
\#NER
本文主要介绍两个处理命名实体的方法。一种是 bidirectional LSTMs + crf，一种是 transition-based。 前者已经衍生出了很多改进方法，比如输入字级别的，增加 attention。最重要的该 paper 放出了代码。大家可以进行参考。
论文链接
http://www.paperweekly.site/papers/714
代码链接
https://github.com/ZhixiuYe/NER-pytorch

![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/0?wx_fmt=png)
**VSE++: Improved Visual-Semantic Embeddings**
**@gujiuxiang 推荐**
\#word embeddings
Unlike the original paper uses the rank loss which computes the sum of violations across the negative training examples. Instead, they penalize the model according to the hardest negative examples.
论文链接
http://www.paperweekly.site/papers/649
代码链接
https://github.com/fartashf/vsepp

[ 计算机视觉 ]![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaDc3GOpv6X8XguY9aNvsDjQ2pzJ3YPlibX0EjGZagDM5JMIoj55icXdGA/0?wx_fmt=png)
**Deep Convolutional Neural Networks with Merge-and-Run Mappings**
**@yijizhao 推荐**
\#ResNet
1. 对比 ResNet，从网络宽度角度探索网络结构；2. 深度融合网络；3. Merge-and-Run 结构去除 ResNets 极深的那一条线路；4. 深层数对网络本身性能的提升不是最关键的，从增加模型的潜在网络数目（Ensemble Size）角度思考网络结构。
论文链接
http://www.paperweekly.site/papers/766
代码链接
https://github.com/zlmzju/fusenet

![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/0?wx_fmt=png)
**SphereFace - Deep Hypersphere Embedding for Face Recognition**
**@guuker 推荐**
\#face recognition
对作者自己论文 Large Margin Softmax 的改进，把最后一层的权重做了归一化。开源的代码非常扎实有参考性，在 LFW 和 MegaFace 任务上取得非常好的实践效果。
论文链接
http://www.paperweekly.site/papers/611
代码链接
https://github.com/wy1iu/sphereface

![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaPFKKcQtakJkrJqiay67wic4UibzIoJEFesEXzBdgvbKrU3pQbdFib3fEibQ/0?wx_fmt=png)
**Self-critical Sequence Training for Image Captioning**
**@jamiechoi 推荐**
\#image captioning
IBM 的论文，使用 RL 来做 image caption，是当前imscoco 上 captioning leaderboard 的第五名，曾经第一名。
论文链接
http://www.paperweekly.site/papers/697
代码链接
https://github.com/ruotianluo/self-critical.pytorch

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。
![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmUEtia3RCJ5eZHIskGRIYc1Uen4885tjUqeiaS963f1BQ5PdHHibppPMuHbfbpVsbicYsU00j6RwOA2w/640?)
![0?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/0?wx_fmt=gif)



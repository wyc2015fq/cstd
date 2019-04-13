
# 如何让GAN生成更高质量图像？斯坦福大学给你答案 - Paper weekly - CSDN博客


2018年08月10日 12:08:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1070


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnC9iaic8hDbiadLafh7TtCZS6icEYddVmMqZBksDV7cQkKmAu95h53FxyibqmZOS1yQgHibJT0WYD2s1Zw/640)
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**95**篇文章
本期推荐的论文笔记来自 PaperWeekly 社区用户**@TwistedW**。本文来自斯坦福斯大学，**论文在 WGAN 的基础上为 GAN 设计了一个课程，通过不断增强判别器的判别能力（课程难度）让生成器学习任务更难，从而让生成器生成能力不断提高（学业成绩提高）达到整体的优化。**
如果你对本文工作感兴趣，点击底部**阅读原文**即可查看原论文。

# 关于作者：武广，合肥工业大学硕士生，研究方向为图像生成。
■ 论文 | Improved Training with Curriculum GANs
■ 链接 | https://www.paperweekly.site/papers/2182
■ 作者 |Rishi Sharma / Shane Barratt / Stefano Ermon / Vijay Pande

本文来自斯坦福斯大学，论文在 WGAN 的基础上为 GAN 设计了课程，通过不断地增强判别器的判别能力（增加课程难度），让生成器学习任务更困难，在越来越难的课程下不断进步自己的生成能力，从而实现高质量图像的生成。虽然是在 WGAN 的基础上开展的工作，但是 Curriculum GAN 的思想的普适性是有的，所以具有一定指导意义。

# 论文引入

让 GAN 的训练更稳定生成的样本更加多样性一直是 GAN 发展的主要动力，**WGAN**[1]、**LSGAN**[2]、**WGAN-GP**[3]都是在 GAN 的损失函数上采取改进，将衡量真实和生成样本的分布的 JS 距离换为更加优越的衡量标准。

WGAN 就是在 GAN 的基础上改进的比较成功的一种。WGAN 利用 Wasserstein 距离代替 JS 距离，从而避免了一定的梯度消失问题，这也为很多 GAN 模型的扩展提供了指导。详细的关于 WGAN 的知识，可以参看我之前的博客[4]。

WGAN-GP 是在 WGAN 的基础上改进的，WGAN 在保证 Lipschitz 限制条件时，作者是采用将限制判别器神经网络的所有参数（这里指权重 w）不超过某个范围 [-c,c]。这样做带来了参数上的两极化，为了改进 WGAN，WGAN-GP 提出了梯度惩罚概念用于满足 Lipschitz 限制条件。具体展开描述，可以参看我之前的博客[5]。Curriculum GANs 其是在 WGAN-GP 的基础上改进的，但是思路是沿袭 WGAN 所以也称之为 WGAN-C。

在训练 GAN 上有很多 trick，比如如何衡量生成器和判别器的强弱从而控制一方的能力；在判别器中加入 minibatch，用于衡量更多的样本，从而丰富生成的多样性；合理的设计网络深度和参数等等。这些 trick 确实提高了 GAN 的训练稳定性和生成上的多样性，但是仍无法避免 GAN 的问题。

为 GAN 设计课程，从而让生成器逐步的进步。这在逻辑上是行得通的。**WGAN-C 就是定义一个越来越严苛的判别器，用于发现生成器的问题，生成器为了欺骗住判别器，需要在越来越难的课程任务下不断进步**。由于生成器已经有了先前学习的基础，在最小化真实和生成样本分布上逐渐的进步，有了学习基础增大难度使得生成器的能力越来越强，从而生成质量更高的图像。

**总结一下 WGAN-C 的优势：**

1. 为 GAN 设计了一个课程，通过不断提高判别器的判别能力从而增强生成器的能力；

2. Curriculum GANs 的思想不仅仅适用于 WGAN 还适用于其它的 GAN 模型，不仅仅是在图像的生成，在文本到图像，图像到图像都有指导意义。

# WGAN-C思想

WGAN-C 不考虑固定一个判别器![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglv1Cic0NtOOhJDLYibfchibUSdBSC3NWLJD7nS1zsrOAB7oQV4JeHk8dY3xjzYyDDex3tfFDlO90IjA/640)，而是考虑预定义的一组判别器的凸组合，定义![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglv1Cic0NtOOhJDLYibfchibUSyreTMP4RmTtIhEraqJwBXo4NLLPlDib4fvgsl0od1icR1ENY0ic5we37g/640)表示，使得![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglv1Cic0NtOOhJDLYibfchibUSk3hhegOo7Mj3rq7QGib7guoLicibKiaAtgxRkLAksN561s7AI8uPhZARcw/640)。判别器的组合可以写为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglv1Cic0NtOOhJDLYibfchibUSVC8eEjMZSK574nGzoHPdQcA1bWSib1e28fptIZIVyuOqJFZlZPt5mGQ/640)。其中的 λ 可以被视为是判别器的能力，λ 值越大也就是意味着判别器的区分能力越强。

这里面是有着一套严格的数学证明的，整体的思路就是随着 λ 的增加，越大的 λ 可以支配前面的 λ，越大的 λ 可以满足之前的要求同时也增强了现有的能力，从而实现了判别能力的逐步增强。根据难度程度对训练样本进行排序，形成判别器的凸组合。

我们通过文中的图来进一步分析：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglv1Cic0NtOOhJDLYibfchibUSENWmsJZhqolca03jYqQQaTsja6vlPKnHUpriaibIxo1phB1JRibd5eicSg/640)

从图中可以看到随着 λ1 到 λ3 逐渐的增大，生成的图像的质量也是越来越好。

# WGAN-C实现

我们先来对比一下 WGAN-C 和 WGAN-GP 在实现算法上的不同，先上 WGAN-GP 的实现算法框图：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglv1Cic0NtOOhJDLYibfchibUSIZxGNhvQZSicS0X5hdHudDvyTFfmJDp78RKIBsiap7AKvxIiawR4hgQhQ/640)

我们再来看看 WGAN-C 的实现算法框图：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglv1Cic0NtOOhJDLYibfchibUSPdFOBdOcxicCoxVDHLPia121O89pGxO1B6pYj1gTRqnY1HQqa3NwdDZA/640)

通过对比我们发现，整体上两个是相似的，WGAN-C 要多了些操作，那就是多了 λ 的选取和相应的判别器的选取，通过不断的提高判别器的判别能力从而实现生成器的生成能力的不断提高，此处控制梯度惩罚的参数用 β 表示了。

# WGAN-C实验

WGAN-C 的实验主要在正弦曲线的生成和人脸的生成上，在正弦曲线生成上，实验对比了有无课程指导的 GAN 的生成效果，从下图可以看出来在相同网络下加入课程指导的 GAN 生成的正弦曲线更加的真实。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglv1Cic0NtOOhJDLYibfchibUSO8ODR0aLicFoNp6Oqibd3q131tIpmDEvtdwicsAxC7Z8VVJrat1GR1F9w/640)

在生成正弦曲线的定量上也对比了有无课程指导的效果，测量了生成的波与数据集中最接近的正弦波的平均 l − 2 误差（通过离散生成数据集的正弦波的范围）。在训练结束时，渐进式延长策略产生的正弦曲线的平均误差降低了 33.6％ - 训练数据集的平均最小距离 l − 2 在课程指导下产生的正弦曲线为 1.13 ± 0.01 ，并且没有课程指导生成的误差为 1.51 ± 0.06 。

在人脸生成上，也对比了有无课程指导下的生成效果。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglv1Cic0NtOOhJDLYibfchibUSpk8zhibPS5DFs5CW9dQwXicwTicPcxf3DYwEtBdqjeMjNlPa5NDyn1Bqw/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglv1Cic0NtOOhJDLYibfchibUSrVvhRRpickSZHTl31YhvSyxmHzYur32oLyh6ccgp58gtV04E8zU0ySA/640)

# 总结

WGAN-C 在 WGAN 的基础上为 GAN 设计了课程，通过不断增加判别器的判别能力来提高课程的难度，相对应的，作为学生的生成器在高难度的课程下不断提高生成能力，从而实现匹配课程难度的生成能力。

WGAN-C 为 GAN 设计课程的思路适用于各种 GAN 模型，同时不仅仅是在图像生成上，在其他 GAN 的生成上也可以发挥作用。

# 参考文献

[1]. Arjovsky, M., Chintala, S., and Bottou, L. (2017). Wasserstein gan. arXiv preprint arXiv:1701.07875.
[2]. Xudong Mao, Qing Li, Haoran Xie, Raymond YK Lau, Zhen Wang, and Stephen Paul Smolley. Least squares generative adversarial networks. arXiv preprint, ArXiv:1611.04076, Apr. 2016.
[3]. Gulrajani, I., Ahmed, F., Arjovsky, M., Dumoulin, V., and Courville, A. (2017). Improved training of wasserstein gans. arXiv preprint arXiv:1704.00028.
[4]. http://www.twistedwg.com/2018/01/31/WGAN.html
[5]. http://www.twistedwg.com/2018/02/02/WGAN-GP.html
**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

**点击标题查看更多论文解读：**

[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)
[基于词向量的简单模型 | ACL 2018论文解读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490031&idx=1&sn=e307230ffbffb648b213b1a775372d06&chksm=96e9c66fa19e4f7996bb13ed2d944d5e49bd538174bd192e41abaf4d2a8863d29135b034cf9c&scene=21#wechat_redirect)
COLING 2018最佳论文：序列标注经典模型复现
[一文解析OpenAI最新流生成模型「Glow」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490358&idx=1&sn=b4b5d6014bdd365456d500537ba5bcad&chksm=96e9c4b6a19e4da08710a55935dc2e15b00838d5395fdf2a424c50dedff9af7fa11441741b9d&scene=21#wechat_redirect)
CVPR 2018 最佳论文解读：探秘任务迁移学习
[哈佛NLP组论文解读：基于隐变量的注意力模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490536&idx=1&sn=0998c5dd4e20841f3542ee328de1f1b4&chksm=96e9c468a19e4d7e24a38f3d9c4b1b4ea48d729c7db124f2b92a2309f0eee982bb9f9bc8356e&scene=21#wechat_redirect)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)
▽ 点击 |阅读原文| 下载论文



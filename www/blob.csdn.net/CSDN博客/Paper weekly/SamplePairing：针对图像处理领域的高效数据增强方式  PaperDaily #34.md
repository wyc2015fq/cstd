# SamplePairing：针对图像处理领域的高效数据增强方式 | PaperDaily #34 - Paper weekly - CSDN博客





2018年01月17日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：239












![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **34** 篇文章


本期推荐的论文笔记来自 PaperWeekly 社区用户** @chenhong**。**本文提出了一种高效数据增强方式 SamplePairing，没有任何公式，没有任何框架，任何 CPU 都能处理。**

 如果你对本文工作感兴趣，点击底部的**阅读原文**即可查看原论文。

# 关于作者：陈泰红，小米高级算法工程师，研究方向为人脸检测识别，手势识别与跟踪。




■ 论文 | Data Augmentation by Pairing Samples for Images Classification

■ 链接 | https://www.paperweekly.site/papers/1523

■ 作者 | chenhong




# 论文动机




这是 IBM 在 arXiv，2018 年 1 月 9 日新挂的一篇论文，主要研究数据增强。核心思想很简单，小学生都会，求平均值。**这是我见到 CNN 领域最简单的一篇论文**。 




数据增强是机器学习任务中广泛使用的技术，如图像处理领域，人工标注数据成本昂贵，而 CNN 的训练有需要大量标注数据避免过拟合。图像处理领域常用的数据增强技术有旋转、扭曲、添加少量噪音、从原图像裁剪等。




本文提出了一种高效数据增强方式 SamplePairing：**从训练集随机抽取的两幅图像叠加合成一个新的样本（像素取平均值），可以使训练集规模从 N 扩增到 N*N****。**没有任何公式，没有任何框架，简单易懂简洁明了，任何 CPU 都能处理。




论文在使用 GoogLeNet，在 ILSVRC 2012 数据集从 top-1 错误率从 33.5% 降低到 29%，在 CIFAR-10 数据集 top-1 错误率从 8.22％ 降低到 6.93％。**这对训练集有限的任务而言，提高了模型的准确性。**




# 模型介绍





![VBcD02jFhgk8TefLcA70wBuibNtPBwzlIdcXngyziamuzxBcEhkYGvLXKwRiau4MvQXbwLTRR4K3iaEMzUrNhW8Gjw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk8TefLcA70wBuibNtPBwzlIdcXngyziamuzxBcEhkYGvLXKwRiau4MvQXbwLTRR4K3iaEMzUrNhW8Gjw/)




论文的模型结构 SamplePairing 如上图所示。模型虽然很简单，但是还得消化一下为什么简单有效。 




先说一下实现过程。训练图像 A 是随机的，从训练集随机抓取图像 B，（A 和 B 都是 ILSVRC2012 的图像，分辨率 256x256）两者都经过基本的数据增强（随机翻转，萃取），分辨率变为 224x224，对两幅图像求平均值，但是 label采用的是 A，之后送入 GoogLeNet 模型。因此，SamplePairing 随机从图像集中创建新的图像，而且 label B 未使用。 




图像 A 和 B 在模型中的权重是一致的，即使使用大型网络，训练误差也不能变成 0，训练精度平均不能超过 50%。对于 N 分类器，最大训练精度是 0.5+1/(Nx2)。




尽管 SamplePairing 的训练精度不会很高，当停止 SamplePairing 作为最终微调时的训练，训练精度和验证精度很快改善。




经过微调模型，使用 SamplePairing 训练的网络比未使用 SamplePairing 训练的模型都高很多。论文认为使用 SamplePairing 是一种正则化。 




在 mix 之前有其他数据增强方式，在 CPU 执行，而反向传播的训练过程在 GPU 执行，这样 CPU 和 GPU 并行计算，没有限制增加总的训练时间。 




论文的训练过程如下： 




1. 先不使用 SamplePairing 数据增强训练，而是先使用传统的数据增强训练网络。




2. 完成一个 epoch（ILSVRC）或者 100 个 epoch（其他数据集），加入 SamplePairing 数据增强。




3. 间歇性禁止 SamplePairing。对于 ILSVRC 数据集，为 300,000 个图像启用 SamplePairing，然后为下一个 100,000 个图像禁用它。对于其他数据集，启用 8 个 epoch，在接下来的 2 个 epoch 禁止 SamplePairing。 




4. 在训练损失函数和精度稳定后，禁止 SamplePairing 作为微调。




# 实验




论文的模型在多个数据集进行验证：ILSVRC 2012，CIFAR-10，CIFAR-100，以及 Street View House Numbers (SVHN) datasets。




以 CIFAR-10 为例，validation 误差一致在波浪形震荡，800epoch 之后才趋于稳定，此时误差才小于不使用 SamplePairing 的模型。 




论文表 1 所示 training error 会增加，而 validation error 会减小，说明正则化效果明显。在 CIFAR 训练集减少样本个数，训练和验证误差相差不大。




# 文章评价




目前作者论文仅仅在 ILSVRC 2012 验证分类的错误率，其他数据集比如目标检测，语义分割是否有效？有研究能力的同志们赶紧往前冲，这又是一个坑。 




 论文给出一种数据增强方式，也用实验验证确实有效，但是为什么有效？




个人认为相当于随机引入噪声，在训练样本中人为引入误导性的训练样本。 如果不是 IBM 的论文，我估计也不会认真研究一番的。在论文满天飞的年代，名企名校名人还是占优势的。

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**




![VBcD02jFhgnED8CgalBpSNcrqdibvRwqEDUqVM5xyf5GmoJgyyIiblwUtgW1MfjJrIYIqXRoaK4uapnvBicXsUB8g](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnED8CgalBpSNcrqdibvRwqEDUqVM5xyf5GmoJgyyIiblwUtgW1MfjJrIYIqXRoaK4uapnvBicXsUB8g/)

△ 戳我领取新年礼物



投票参与 & 读者福利


**** 参与方式 ****




1. 长按识别下方二维码参与投票

2. 文末留言你喜欢某篇论文的原因

3. 分享本文到朋友圈并截图发至后台




**** 截止时间 ****




2018年1月24日0点0分




**** 福利清单 ****




PaperWeekly定制手机壳 x 3份

PaperWeekly定制笔记本 x 5份

PaperWeekly定制行李牌 x 10份




![VBcD02jFhgnED8CgalBpSNcrqdibvRwqESxVO3lbKOIEIBdyzBIykjB2Vq91YlFTibBUylIC8ItotiadPyluHEn0w](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnED8CgalBpSNcrqdibvRwqESxVO3lbKOIEIBdyzBIykjB2Vq91YlFTibBUylIC8ItotiadPyluHEn0w/)

△ 我们长这样哦~




长按扫描二维码，参与投票*！*

▼
![VBcD02jFhgnED8CgalBpSNcrqdibvRwqE8zf7FlfQqgZERt7Z7vW9KJCPpRYl4BicGLProOXeYhV2SJOn6ibtV7yA](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnED8CgalBpSNcrqdibvRwqE8zf7FlfQqgZERt7Z7vW9KJCPpRYl4BicGLProOXeYhV2SJOn6ibtV7yA/)



**![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/?)****# ****高 能 提 醒 #**



1. 为了方便大家在投票过程中查看论文详情，**请勿使用微信内置浏览器**。点击页面右上角的“…”按钮，在手机浏览器中打开表单。




2. 本次评选包含**自然语言处理**和**计算机视觉**两大方向，请在你所选择的参与方向下**勾选3-10篇论文**。




3. 获奖名单将于1月25日公布，其中5位由小编根据文末留言选取，其他13位采用随机抽取，礼物随机发放。







**长按扫描二维码，****马上投票*！***

▼

![VBcD02jFhgnED8CgalBpSNcrqdibvRwqE8zf7FlfQqgZERt7Z7vW9KJCPpRYl4BicGLProOXeYhV2SJOn6ibtV7yA](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnED8CgalBpSNcrqdibvRwqE8zf7FlfQqgZERt7Z7vW9KJCPpRYl4BicGLProOXeYhV2SJOn6ibtV7yA/)






******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)

▽ 点击 | 阅读原文| 查看原论文





# PTGAN：针对行人重识别的生成对抗网络 | PaperDaily #36 - Paper weekly - CSDN博客





2018年01月22日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1485












![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **36** 篇文章


本期推荐的论文笔记来自 PaperWeekly 社区用户** @LUOHAO**。**本文提出了一种针对于行人重识别的生成对抗网络 PTGAN，使用 GAN 将一个数据集的行人迁移到另外一个数据集。**

 如果你对本文工作感兴趣，点击底部的**阅读原文**即可查看原论文。

# 关于作者：罗浩，浙江大学博士研究生，研究方向为计算机视觉和深度学习，现为旷视科技（Face++）的 research intern。




■ 论文 | Person Transfer GAN to Bridge Domain Gap for Person Re-Identification

■ 链接 | https://www.paperweekly.site/papers/1557

■ 作者 | Longhui Wei / Shiliang Zhang / Wen Gao / Qi Tian




# 摘要




本文提出了一种**针对于 ReID 的生成对抗网络 PTGAN**，可以实现不同 ReID 数据集的行人图片迁移，在保证行人本体前景不变的情况下，将背景转换成期望的数据集 style。




另外本文还提出一个**大型的 ReID 数据集 MSMT17**，这个数据集包括多个时间段多个场景，包括室内和室外场景，是一个非常有挑战的数据集。




论文用 PTGAN 来缩小不同数据集间的 domain gap，并在新提出的 MSMT17 这个大数据集和其他一些公开的小数据集上做了实验。




# MSMT17 数据集




MSMT17 是一个大型的 ReID 数据集，现在的一些公开数据集的准确度已经被刷得很高，这个数据集的提出进一步延续了 ReID 的发展。不过数据集目前还有公开，等待论文接收后数据集可以公开。 




**MSMT17 数据集有以下几个特性： **



- 
数据采集时长约为 180 小时

- 
总共有 15 个相机，其中 12 个室外相机，3 个室内相机 

- 
行人框由 Faster RCNN 机标完成 

- 
最后总共有 4101 个行人的 126441 个 bounding boxes





![VBcD02jFhgmrice7PPJVLEliatC04Qic2zaQC4enyZiaBEJH7AlqqpLmbCLtyDOw5LEuqLdHXLPRqguU87Zbsib3YTw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmrice7PPJVLEliatC04Qic2zaQC4enyZiaBEJH7AlqqpLmbCLtyDOw5LEuqLdHXLPRqguU87Zbsib3YTw/)

△MSMT17 数据集和已有 ReID 数据集的对比




![VBcD02jFhgmrice7PPJVLEliatC04Qic2zaF4d7QvibjnXKEupSIYUnesXoJsAhGUbK52CohicAs5C3HxiamPXrg4LFA](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmrice7PPJVLEliatC04Qic2zaF4d7QvibjnXKEupSIYUnesXoJsAhGUbK52CohicAs5C3HxiamPXrg4LFA/)

△MSMT17 数据集的图片和其他数据集的直观对比




# PTGAN




Person Transfer GAN（PTGAN）是作者提出的一个针对于 ReID 问题的 GAN。这个 GAN 最大的特点就是在尽可能保证行人前景不变的前提下实现背景 domain 的迁移。 




首先 PTGAN 网络的损失函数包括两部分：




![VBcD02jFhgmrice7PPJVLEliatC04Qic2zaRmFXsE6QwK9H2PrpxDYXia6WuHH2sYjDaicnPY0z0NDgIVb7o7qsdT3Q](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmrice7PPJVLEliatC04Qic2zaRmFXsE6QwK9H2PrpxDYXia6WuHH2sYjDaicnPY0z0NDgIVb7o7qsdT3Q/)




其中* LStyle *代表生成的风格损失，或者说 domain 损失，就是生成的图像是否像新的数据集风格。*LID* 代表生成图像的 ID 损失，就是生成的图像是否和原始图像是同一个人。*λ1* 是平衡两个损失的权重。下面的关键就是看这两个损失怎么定义。




首先 PTGAN 的基础是 CycleGAN，所以 loss 也和正常的 CycleGAN 的 loss 差不多。首先第一部分是 *LStyle*，这个就是标准的 CycleGAN 的判别 loss。




![VBcD02jFhgmrice7PPJVLEliatC04Qic2zaer6KHNUWrwXvZZ7EAkyEE4RPSqVJNjKWjTVvNQ9ibcNLiaRhyQxcvpxg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmrice7PPJVLEliatC04Qic2zaer6KHNUWrwXvZZ7EAkyEE4RPSqVJNjKWjTVvNQ9ibcNLiaRhyQxcvpxg/)




以上几部分都是正常的 CycleGAN 的损失，保证生成的图片和期望的数据集的 domain 是一样的。 




论文的另外一个改进的地方就是 *LID*。为了保证图片迁移过程中前景不变，先用 PSPNet 对图片进行了一个前景分割，得到一个 mask 区域。




传统的 CycleGAN 并不是用于 ReID 任务，因此也不需要保证前景物体的 ID 信息不变，这样的结果就是前景可能模糊之类的质量很差，更糟糕的现象是行人的外观可能改变，比如衣服颜色发生了改变，这是 ReID 任务非常不希望见到的。




为了解决这个问题，论文提出 *LID *损失，用 PSPNet 提取的前景，这个前景就是一个 mask，最后 ID 损失为：




![VBcD02jFhgmrice7PPJVLEliatC04Qic2zaLGRZT0aHgxFDyZP9G6M4icN7xxaJN3ZGia3HREloEDGd3o4wog5FXLaw](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmrice7PPJVLEliatC04Qic2zaLGRZT0aHgxFDyZP9G6M4icN7xxaJN3ZGia3HREloEDGd3o4wog5FXLaw/)




其中 *M(a)* 和 *M(b)* 是两个分割出来的前景 mask，ID loss 将会约束行人前景在迁移过程中尽可能的保持不变。最后转换的效果如下图所示：




![VBcD02jFhgmrice7PPJVLEliatC04Qic2zalet11aW3jwQHRDqgeXIYbX44wicJwicbtX6Oic3n2wibvx5CIHoMUSm4wg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmrice7PPJVLEliatC04Qic2zalet11aW3jwQHRDqgeXIYbX44wicJwicbtX6Oic3n2wibvx5CIHoMUSm4wg/)




可以看出，直观上和传统的 CycleGAN 相比能够更好的保证行人的 ID 信息。




# 结果




![VBcD02jFhgmrice7PPJVLEliatC04Qic2zarZgyAuagnoY5dGw02v2Yj1a4pXFdFv6UAhATR0QoicnQeslTber134g](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmrice7PPJVLEliatC04Qic2zarZgyAuagnoY5dGw02v2Yj1a4pXFdFv6UAhATR0QoicnQeslTber134g/)




实验结果如上表，虽然论文没有用特别复杂的网络来训练，但是将另外一个数据集通过 PTGAN 迁移到 MSMT 上都能增加 MSMT17 数据集上的 performance。并且从准确度上看 MSMT17 还是一个非常难的数据集。

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





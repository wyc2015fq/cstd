# AlphaGo Zero的伟大与局限——ResNet作者、旷视研究院院长孙剑深度评述 - 人工智能学家 - CSDN博客
2017年10月21日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：219
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWx1GgdXSibgOspZwiaAPayrIsvUYwibw8suBgr5PTgcXWZdXLqNcgicKDyyhJFNWIzrHT5aTz4BqwjWQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
*来源：Megvii旷视科技*
*概要：2017年10月19日，Deepmind 团队发布强化版的 AlphaGo Zero，该版本的 AlphaGo 实现了在 AI 发展中非常有意义的一步——“无师自通”，这也让去年败在未升级版本 AlphaGo Master 下的中国棋手柯洁惊呼“人类太多余了”。*
2017年10月19日，Deepmind 团队发布强化版的 AlphaGo Zero，该版本的 AlphaGo 实现了在 AI 发展中非常有意义的一步——“无师自通”，这也让去年败在未升级版本 AlphaGo Master 下的中国棋手柯洁惊呼“人类太多余了”。而值得注意的是，该版本的 AlphaGo 所采用的其中一个重要环节是诞生于华人团队的深度残差网络（ResNet）。
主讲人：孙 剑
旷视科技首席科学家、旷视研究院院长
就此背景，旷视科技Face++首席科学家孙剑博士作为深度残差网络 ResNet 作者之一，特别接受了多方媒体的专访并对这次的技术升级做了详尽的阐述。在他看来，本次技术提升足够伟大，但在真实技术落地过程中却有着众多局限，并指出未来很长一段围绕大数据的训练模式和有监督的学习方法仍是主流深度学习技术应用的必需方式。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/MR4y3Zyg39zjWKhjTx53FoaYWqajNLNEOiaoibc4hPWMM0MvxlOopePOrSmbcwIA0rlWdxlvtzHABJSeuVCgm3gQ/640?wx_fmt=jpeg)
*孙剑团队发表的《图像识别中的深度残差学习》*
*获得 CVPR 2016 最佳论文奖*
**两大核心要素实现**
**AlphaGo Zero 极简算法**
“我们做研究追求极简祛除复杂。这篇文章最吸引人的地方就在于他们做的减法。首先 AlphaGo Zero 的搜索过程简化了很多，例如把以前系统中的两个网络合并成一个网络、将深度残差网络的输入做最简化。”旷视科技Face++首席科学家孙剑博士在讲到本次 AlphaGo Zero 的技术特点时说道，“此外，AlphaGo Zero 可以把19×19 棋局的二值图像直接送给神经网络，让神经网络看着棋盘做决策，简洁而优美。” 
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/MR4y3Zyg39zjWKhjTx53FoaYWqajNLNE78PXblW2lg35QOTxX6RiboTSewibcuJvZcVvgwoC84KsBicIFP1nlZvDw/640?wx_fmt=jpeg)
*AlphaGo Zero 学到的围棋知识（图片来源：DeepMind 论文）*
孙剑博士在交流中表示，本次 AlphaGo Zero 的提升主要有两个核心要素，一个是启发式搜索，一个是深度残差神经网络，而这两个又非常完美的实现了结合。其中启发式搜索的思想非常朴素，是针对问题设计的一个高级定制版蒙特卡洛树搜索算法。另外的要素则是通过深度残差神经网络让简单的搜索算法极大的提升了效率。
深度残差神经网络（ResNet）是2015年由孙剑所带领的微软视觉计算团队率先提出，并在当年的 ImageNet 以及 MS COCO 两大技术竞赛中包揽五项冠军，其中最重要的部分就是实现了突破性的 152 层的网络深度，从而让一些非常复杂的函数做映射时的效率与有效性得到极大的提升。强大的网络使的 AlphaGo Zero 可以有能力学习并对落子概率和整体棋局有更精准的判断。
**伟大与局限并存**
**“无师自通”短时间无法成为 AI 主流**
“AlphaGo Zero 的伟大之处是第一次让机器可以不通过任何棋谱，不通过任何人类的经验，在只告诉规则的前提下就实现了成为一个围棋高手，这种无师自通的学习模式在 AI 整个发展上是非常有里程碑意义的。”孙剑博士讲到AlphaGo Zero的技术意义时指出，“但是同时这种无师自通在很多 AI 落地上也存在一些局限，因为严格意义上讲，围棋规则和判定棋局输赢也是一种监督信号，所以有人说人类无用、或者说机器可以自己产生认知，都是对
 AlphaGo Zero 错误理解。”
“实际上在很多 AI 行业落地中，弱的监督学习或者所谓的无师自通还是无法在短期成为主流。”孙剑博士补充，“比如，就旷视Face++所擅长的人脸识别来讲，这个能力是人类后天学习的能力，是通过时间不断演化出来的一种生存能力，人只有具备了人脸识别能力，人类社会才能正常运转。把这种后天能力输出给机器，其实就需要人的监督信号，包括目前人工智能落地所解决的很多事情，其实都是模拟人类的一种技能，让机器实现这种任务就需要海量的数据与更多的信号输入，所以今天，甚至今后很长一段时间内，监督学习依然是
 AI 研究与 AI 商业化的主流方向。”
**开放与互通**
**旷视Face++致力推动 AI “变现”**
而讲到 AI 的未来，孙剑博士谈到了开放与互通，并重点讲到每天第一件事情就是去网上开放的论文平台 ArXiv 查看是否有新的、有意思的论文、思想发出来。就 ResNet 被 AlphaGo Zero 应用这个点上，孙剑博士表示：“这次应用在 AlphaGo Zero 中的 ResNet 残差神经网络，曾获得了 CVPR 2016 的最佳论文奖。当然今天我也非常高兴看到这个技术可以应用在
 AlphaGo Zero 系统中。但这个应用过程其实并不需要我们直接进行接触而是一种研究成果的交流，人工智能研究最前沿的开源与开放，才能让我们在追求更优解的过程中有很多参考与理论支撑，可以极大的提升新技术产生的周期。”
而作为旷视研究院院长，孙剑博士强调：“和全球研究机构一样，旷视研究院的工作和使命，也是不断分享、开放研究成果，但是更注重的是技术在产业中的实用性。” 2017年7月，旷视研究院团队就在 ArXiv 公开了一篇关于 ShuffleNet 的论文，ShuffleNet 是旷视专为移动端和低功耗设备设计的一种神经网络算法，其实际上也是一种对 ResNet 深度残差网络的改进，发布之后很多业内同行就已经在各自的研发中应用上了。而旷视自己在不到半年的时间里，也已经基于
 ShuffleNet 推出了多款创新智能终端产品和手机人脸解锁等应用。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWx1GgdXSibgOspZwiaAPayrIlBeiaxGljq2C3I72nhzPknCwdOmunTTnDt5vBjUbkVsVQqX7WZTUM5Q/0?wx_fmt=png)
*旷视基于移动端卷积神经网络的开发的各类应用*
“不断借助创新而实现更多的创新，不断借助伟大的思想创造伟大的场景。只有不断的开放最好的认知，才能让这个行业不断成长，让更多更强的 AlphaGo Zero 产生。” 

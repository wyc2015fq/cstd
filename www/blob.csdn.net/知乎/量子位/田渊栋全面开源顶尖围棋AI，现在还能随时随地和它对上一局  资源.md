# 田渊栋全面开源顶尖围棋AI，现在还能随时随地和它对上一局 | 资源 - 知乎
# 



> 铜灵 发自 凹非寺
量子位 出品 | 公众号 QbitAI

现在，随时、随地、随心情，你都能和国际顶级围棋AI对战交流一局了。

最近，Facebook的围棋AI ELF OpenGo全面开源，下载ELF OpenGo最终版本模型，人人都能与ELF OpenGo下棋。

对了，不要被ELF OpenGo“超能力”般的棋艺**惊叹到**，不仅是你，连韩国棋院的专业围棋选手也被打败了。在与金志锡，申真谞，朴永训及崔哲瀚四位专业棋手对战时，ELF OpenGo以20：0的成绩大赢特赢。

甚至围棋AI界小有名气的前辈Leela Zero，也以18：980的成绩被ELF OpenGo远远甩在身后。

今天，Facebook公布了ELF OpenGo的研究论文，复现了AlphaGo Zero和AlphaZero，还详细揭秘了ELF OpenGo的训练细节，附带了一系列**开源地址**。
![](https://pic3.zhimg.com/v2-7fd65b86a956aeb580012ef068a70f52_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='357'></svg>)
## **15天，15天**

在今天刚发布的论文ELF OpenGo: An Analysis and Open Reimplementation of AlphaZero中，Facebook研究人员全面披露了ELF OpenGo的训练过程。
![](https://pic3.zhimg.com/v2-1f6a53d2fbfb173c2c5649944ae0903a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='276'></svg>)
ELF OpenGo是去年诞生的。当时，Facebook改进了自己面向游戏的机器学习框架ELF，在上面重新实现了DeepMind的AlphaGoZero及AlphaZero的算法，得到了这个围棋AI ELF OpenGo。

论文显示，训练过程大部分遵循了AlphaZero的训练过程。

和AlphaZero用5000个自我对弈的TPU和64个训练TPU不同，整个训练过程共用了2000块英伟达GPU，型号均为英伟达Tesla V100 GPU，内存为16GB，总共训练了15天。
![](https://pic2.zhimg.com/v2-3783f82d9ea6d0bacb3d22532b7bce69_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='379'></svg>)
研究人员还应用了ELF OpenGo，完成了另外三方面突破。

一方面，为ELF OpenGo训练处一个棋艺超越人类的模型。

研究人员开发了一个类似AlphaZero的软件，在上面用2000块GPU连续训练了9天后，这个20个区块的模型的表现已经超过了人类水平。

随后，研究人员提供了一些预训练模型、代码和2000万局自我对弈的训练轨迹数据集进行训练。

第二方面，研究人员提供了模型在训练过程中的行为分析：
- 在训练过程中，研究人员观察到，ELF OpenGo与其他模型相比，水平变化比较大，即使学习率稳定，棋力也会上下浮动。
- 另外，模型需要依靠前瞻性来决定下一步棋怎么下时，模型学习速度较慢，学习难度很大。
- 除此之外，研究人员还在探索了在游戏的不同阶段AI学会高质量的棋法的速度。

第三方面，研究人员进行了Mextensive ablation实验，学习AlphaZero风格算法的属性，对比了ELF OpenGo与AlphaGo Zero与AlphaZero的训练过程。

研究人员发现，对于最终模型而言，对局中加倍rollout水平大约提升200 ELO，AI的发挥会受到模型容量的限制。

目前，ELF OpenGo的论文、模型、实现代码、自我对弈数据集和与人类对弈记录等已经**全部开放**，地址可到文末寻找。

## **明星团队**

这篇论文来自Facebook人工智能研究所（FAIR），一作国内机器学习圈里一个熟悉的名字，田渊栋。

田渊栋从卡内基梅隆大学（CMU）毕业后，田渊栋奔赴谷歌无人车项目组，随后跳槽转向Facebook人工智能研究所。Facebook围棋AI Darkforest的相关研究，负责人和论文一作也是田渊栋。
![](https://pic3.zhimg.com/v2-e53301326ac3018f7a8f6237eb9a173e_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='354' height='352'></svg>)
田渊栋也一直活跃在知乎，是人工智能、深度学习话题的优秀回答者，是知乎er心中的大神。

去年，田渊栋回顾自己近几年的工作感悟和学习生涯的文章《博士五年之后五年的总结》，曾成为圈内的爆款文章，不少网友再次被圈粉，大呼醍醐灌顶。
![](https://pic1.zhimg.com/v2-596f3bc37004e7055446bd572023490c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='516'></svg>)
论文二作Jerry Ma也同样为华裔，其Facebook介绍显示，2018年，Jerry Ma刚刚本课毕业，获得哈佛大学经济学和古典文学学士双学位。目前担任Facebook研究工程负责人。
![](https://pic3.zhimg.com/v2-b112f7cf2240abfa95971f0e5d85f51e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='287'></svg>)
**△** Jerry Ma

年纪不大，责任不小。

## **传送门**

GitHub地址：

[https://github.com/pytorch/ELF](https://link.zhihu.com/?target=https%3A//github.com/pytorch/ELF)

论文地址 ：

[https://arxiv.org/abs/1902.04522v1](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1902.04522v1)

Facebook博客介绍：

[https://ai.facebook.com/blog/open-sourcing-new-elf-opengo-bot-and-go-research/](https://link.zhihu.com/?target=https%3A//ai.facebook.com/blog/open-sourcing-new-elf-opengo-bot-and-go-research/)

ELF OpenGo官网：

[https://facebook.ai/developers/tools/elf-opengo](https://link.zhihu.com/?target=https%3A//facebook.ai/developers/tools/elf-opengo)

另外，如果你自带Windows系统的电脑，还可以下载这个软件，在线下棋。**下载地址：**
[https://dl.fbaipublicfiles.com/elfopengo/play/play_opengo_v2.zip​dl.fbaipublicfiles.com](https://link.zhihu.com/?target=https%3A//dl.fbaipublicfiles.com/elfopengo/play/play_opengo_v2.zip)
—**完**—

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤



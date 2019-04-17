# CycleGAN：图片风格，想换就换 | ICCV 2017论文解读 - Paper weekly - CSDN博客





2018年03月14日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1697












![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?wxfrom=5&wx_lazy=1)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **51** 篇文章


本期推荐的论文笔记来自 PaperWeekly 社区用户** @LUOHAO**。**本文提出的模型名为 CycleGAN，作者希望在不借助 paired example 情况下，来实现图片的风格转换。**

 如果你对本文工作感兴趣，点击底部的**阅读原文**即可查看原论文。

# 关于作者：罗浩，浙江大学博士研究生，研究方向为计算机视觉和深度学习，现为旷视科技（Face++）的 research intern。




■ 论文 | Unpaired Image-to-Image Translation using Cycle-Consistent Adversarial Networks

■ 链接 | https://www.paperweekly.site/papers/807

■ 源码 | https://junyanz.github.io/CycleGAN/




# 前言




CycleGAN 是发表于 ICCV17 的一篇 GAN 工作，**可以让两个 domain 的图片互相转化**。传统的 GAN 是单向生成，而 CycleGAN 是互相生成，网络是个环形，所以命名为 Cycle。




并且 **CycleGAN 一个非常实用的地方就是****输入的两张图片可以是任意的两张图片**，也就是 unpaired。




# 单向GAN




读者可以按照原论文的顺序理解 CycleGAN，这里我按照自己的思路解读。**CycleGAN 本质上是两个镜像对称的 GAN，构成了一个环形网络**。其实只要理解了一半的单向 GAN 就等于理解了整个CycleGAN。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGo0UXLJfGUQXmpliahiazTVoosfRZic1GZHH9wOFy0UJWNc0bOFtAWu2og/640)




上图是一个单向 GAN 的示意图。我们希望能够把 domain A 的图片（命名为 a）转化为 domain B 的图片（命名为图片 b）。




为了实现这个过程，我们需要两个生成器 G_*AB* 和 G_*BA*，分别把 domain A 和 domain B 的图片进行互相转换。




图片 A 经过生成器 G_*AB *表示为 Fake Image in domain B，用 G_*AB*(a) 表示。而 G_*AB*(a) 经过生辰器 G_*BA *表示为图片 A 的重建图片，用 G_*BA*(G_*AB*(a)) 表示。




最后为了训练这个单向 GAN 需要两个 loss，分别是生成器的重建 loss 和判别器的判别 loss。




**判别 loss**：判别器 D_*B* 是用来判断输入的图片是否是真实的 domain B 图片，于是生成的假图片 G_*AB*(A) 和原始的真图片 B 都会输入到判别器里面，公示挺好理解的，就是一个 0,1 二分类的损失。最后的 loss 表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGibFUAjcjSWMOHh308kqNzZvvcMlZxdlk0ia7BgLC3GXUtnPJonJziaPHw/640)




**生成 loss**：生成器用来重建图片 a，目的是希望生成的图片 G_*BA*(G_*AB*(a)) 和原图 a 尽可能的相似，那么可以很简单的采取 *L*1 loss 或者 *L*2 loss。最后生成 loss 就表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGqOvtFg67925bWK4gnnBylNMecR5ZhcJia7IXapNGn0AKAur7PGvWtkA/640)




以上就是 A→B 单向 GAN 的原理。




# CycleGAN




**CycleGAN 其实就是一个 A→B 单向 GAN 加上一个 B→A 单向 GAN**。两个 GAN 共享两个生成器，然后各自带一个判别器，所以加起来总共有两个判别器和两个生成器。一个单向 GAN 有两个 loss，而 CycleGAN 加起来总共有四个 loss。




CycleGAN 论文的原版原理图和公式如下，其实理解了单向 GAN 那么 CycleGAN 已经很好理解。 




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGYXTewTz1ad6M7aShWHv4A9RhCI0qqlmrG40dAPvJZfaiaxn0H292LQg/640)




X→Y 的判别器损失为，字母换了一下，和上面的单向 GAN 是一样的：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGEcP2eWia9tOo4rWsgl5TSzVwec9Ma34JFQJkezazRG1LIXkTVlltHGQ/640)




同理，Y→X 的判别器损失为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGkXgjV2pjsHFM2gmdOMmBjOZD5O2aqiaRTH2wHsxFK1aaAK1vRo8QQHA/640)




而两个生成器的 loss 加起来表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGibMgW6fVt0U77Wz9FojXQDCibkVtQJdlqXhSLcFbTTYGls7Clib0SPAoA/640)




最终网络的所有损失加起来为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGa8yb9G4Qan6Gc8eeEuMmCicQcCh4fbglib1st6uAMXzU5KOuG6hAGaSQ/640)




论文里面提到判别器如果是对数损失训练不是很稳定，所以改成的均方误差损失，如下：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhG33GPXwyobToRVTNTXP40J7muibQmbCDCgkOUJZj7qibAhQACSfULbaiaQ/640)




下面放一张网友们自制的 CycleGAN 示意图，比论文原版的更加直观。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGjwQFv2puxh7b6KVe0KvWK37Qic9n82Yt5I1D2VG4eKjYtmz2UcOQsww/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGsPMtkljmgSE5iczADiaEvzvx6nHWfT44gWvjTFlzXUYNmY956IXJEuOQ/640)




# 效果展示




CycleGAN 的效果还是不错的，论文里给出了很多结果图，可以欣赏一下。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGAtFjecagiaticY3IYHvMmzRVasyxmiaCfJjDC47U6JicicvQORLD73919FA/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkzRs7hOSfuAECeib9MDpvhGV2csYo88Sicmv4dO1rtiaD3D7HAotcdSt5oBtb3MKaOLY3VxIuvOjxBQ/640)




**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)




**点击以下标题查看相关内容：**




- 
[GAN做图像翻译的一点总结](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486795&idx=1&sn=2d5a88fff351c32a1ee0ce8fc07f1c65&chksm=96e9d2cba19e5bddc816f7a5ed70da31d0dc47a7dbdec01700c5041922cc04a2c012c9be5dd0&scene=21#wechat_redirect)

- 
[GAN眼中的图像翻译（附神奇歌单）](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247485831&idx=1&sn=4c5c3c9a163a5ecbc6d311038bd99e5c&chksm=96e9d607a19e5f11e3060b68bce80851a1f2bd14ae346d35c5328a8aa294388eba732de4ffca&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247485831&idx=1&sn=4c5c3c9a163a5ecbc6d311038bd99e5c&chksm=96e9d607a19e5f11e3060b68bce80851a1f2bd14ae346d35c5328a8aa294388eba732de4ffca&scene=21#wechat_redirect)[PTGAN：针对行人重识别的生成对抗网络](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487127&idx=1&sn=d5bfb870254c1c4ed663ad728d726766&chksm=96e9d117a19e5801c6e45fd45db590bfe9aa6453f6a774b3461cb6ed7261efaaf093d43caea9&scene=21#wechat_redirect)











![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkvSxCRgqK914dI363k7VWmXJBVCNtexC0iaXJXUohTRgBpX4wGrDNYNpPB6SrZHiccz24zYcN2th2Q/640?)




**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****榜 单 公 布 #**



**[2017年度最值得读的AI论文 | NLP篇 · 评选结果公布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487348&idx=1&sn=8ee8bf57418342a419fe73829cb14e75&chksm=96e9d0f4a19e59e288dcb105bd90b1e13f419ee7268ac69eba7cd6dac12e2e64aa84c56e5c07&scene=21#wechat_redirect)**

**[2017年度最值得读的AI论文 | CV篇 · 评选结果公布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487362&idx=1&sn=980153481f88ba5d6ba2929fd08240f3&chksm=96e9d002a19e5914f845973111b7056d24a28a8f7932479178c9cfcdb10c05c3284d5c0eb602&scene=21#wechat_redirect)**






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




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/640?)









******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)

▽ 点击 | 阅读原文| 查看原论文





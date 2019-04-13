
# 进击的YOLOv3，目标检测网络的巅峰之作 | 内附实景大片 - Paper weekly - CSDN博客


2018年05月12日 08:43:53[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：5351


![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlSGxuHflnDEBib3LZGnZ6DsOzTCZyXunpKc7iavdKEashDppg0kibZPpoQ/640?wxfrom=5&wx_lazy=1)

作者丨浩克匠心
研究方向丨基于深度学习的实时目标检测
知乎专栏丨计算视觉与深度学习的小屋

# YOLOv3的前世今生

2015 年，R-CNN 横空出世，目标检测 DL 世代大幕拉开。

各路豪杰快速迭代，陆续有了 SPP，fast，faster 版本，至 R-FCN，速度与精度齐飞，区域推荐类网络大放异彩。

奈何，未达实时检测之基准，难获工业应用之青睐。

此时，凭速度之长，网格类检测异军突起，先有 YOLO，继而 SSD，更是摘实时检测之桂冠，与区域推荐类二分天下。然准确率却时遭世人诟病。

遂有 JR 一鼓作气，并 coco，推 v2，增加输出类别，成就 9000。此后一年，作者隐遁江湖，逍遥 twitter。偶获灵感，终推 v3，横扫武林！
# 准确率不再是短板

自从 YOLO 诞生之日起，**它就被贴上了两个标签：**

速度很快

不擅长检测小物体

而后者，成为了很多人对它望而却步的原因。

由于原理上的限制，YOLO 仅检测最后一层卷积输出层，小物体像素少，经过层层卷积，在这一层上的信息几乎体现不出来，导致难以识别。

YOLOv3 在这部分提升明显，**先看看小物体的识别**。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSltN57InibZYBY1LB5oITwOAiaGtNibthDf4BY02Pfxk0g5P9jzx3pKibcAw/640)
**▲**YOLOv3的识别结果

直观地看下和 YOLOv2 的对比图如下。可以看出，对于小物体的识别，提高非常明显。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlaDalBSr0hhIP9sWXAgK06okicH7pS0Jgg15hs1U9gRpRjia69ysKjJ7A/640)

无论是传统的模式识别图像检测，还是基于 CNN 的视觉检测，**对于紧凑密集或者高度重叠目标的检测通常是非常困难的**。比如对合影的人群检测在 YOLOv2 上的结果：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlCy6hE7WmuSavpPPEiarDeLxOicAzsQ6N3NjriacJPgb21FxZg9JXSgapw/640)

而下面是 v3 的结果![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/b96CibCt70iaajvl7fD4ZCicMcjhXMp1v6UibM134tIsO1j5yqHyNhh9arj090oAL7zGhRJRq6cFqFOlDZMleLl4pw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlibj50YVtDjNHfjUt1UOfYXHbibSibicOqtW9JIGkbpKEw7OsLKO5LIhCCA/640)
一次检测到图中 90% 的人，还增加了 tie（领带）这个新类别，非常惊艳！

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlb1swX6FibGDr6CxL6EdOAktdQ2GkBJRjG5P87ekpdT1880r18DDZRaw/640)

**再看看模型的泛化能力如何：**

骷髅并不在训练数据集中，但是通过训练模型强大的泛化能力，自动将其归类到了人类（也算是最为合理的近似处理了）。

这在 YOLOv2 中是检测不到的。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSl7KQk3GtWC13Aia4zccmKicTHkedfiboEaGJwFYnvo8z6CkwhFWOEG0pXQ/640)

那么，**模型泛化能力很强的副作用，就是分类结果跑偏**，比如下面这张 coser 的识别图，最左侧的人识别成了马：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlSU9s6kFQVp3OmNlMZWWNJQEYhWdsJ8HPcvziclPjYlMDM9Z6SmAJtSg/640)

# 训练和检测都很快

论文中做了详尽的对比。

和前辈们比，YOLO 的速度非常快，比 R-CNN 快 1000 倍，比 Fast R-CNN 快 100 倍。

和同辈们比，YOLOv3-608 检测准确率比 DSSD 更高，接近 FPN，但是检测时间却只用了后面两者的三分之一不到。

原因如论文中所说，它在测试时观察整张图像，预测会由图像中的全局上下文（global context）引导。它还通过单一网络评估做出预测，而不像 R-CNN 这种系统，一张图就需要成千上万次预测。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlLOrHmRibbNFKMwy4iarbQIJv1PUZMDWxicuO5ibEX3ZSeRD3LRDibiciaq8GQ/640)

# 用了哪些黑科技？

**1. 多级预测**

终于为 YOLO 增加了 top down 的多级预测，解决了 YOLO 颗粒度粗，对小目标无力的问题。

v2 只有一个 detection，v3 一下变成了 3 个，分别是一个下采样的，feature map 为 13*13，还有 2 个上采样的 eltwise sum，feature map 为 26*26，52*52，也就是说 v3 的 416 版本已经用到了 52 的 feature map，而 v2 把多尺度考虑到训练的 data 采样上，最后也只是用到了 13 的 feature map，这应该是对小目标影响最大的地方。

在论文中从单层预测五种 boundingbox 变成每层 3 种 boundongbox。

**2. loss不同**

作者 v3 替换了 v2 的 softmax loss 变成 logistic loss，由于每个点所对应的 bounding box 少并且差异大，每个 bounding 与 ground truth 的 matching 策略变成了 1 对 1。

当预测的目标类别很复杂的时候，采用 logistic regression 进行分类是更有效的，比如在 Open Images Dataset 数据集进行分类。

在这个数据集中，会有很多重叠的标签，比如女人、人，如果使用 softmax 则意味着每个候选框只对应着一个类别，但是实际上并不总是这样。复合标签的方法能对数据进行更好的建模。

**3. 加深网络**

采用简化的 residual block 取代了原来 1×1 和 3×3 的 block（其实就是加了一个 shortcut，也是网络加深必然所要采取的手段）。

这和上一点是有关系的，v2 的 darknet-19 变成了 v3 的 darknet-53，为啥呢？就是需要上采样啊，卷积层的数量自然就多了，另外作者还是用了一连串的 3*3、1*1 卷积，3*3 的卷积增加 channel，而 1*1 的卷积在于压缩 3*3 卷积后的特征表示。

**4. Router**

由于 top down 的多级预测，进而改变了 router（或者说 concatenate）时的方式，将原来诡异的 reorg 改成了 upsample。

# 下一代YOLO长啥样？

**1. mAP 会继续提高：**随着模型训练越来越高效，神经网络层级的不断加深，信息抽象能力的不断提高，以及一些小的修修补补，未来的目标检测应用mAP会不断提升。

**2. 实时检测会成为标配：**目前所谓的“实时”，工业界是不认可的。为什么呢，因为学术圈的人，验证模型都是建立在 TitanX 或者 Tesla 这类强大的独立显卡上，而实际的潜在应用场景中，例如无人机/扫地/服务机器人/视频监控等，是不会配备这些“重型装备”的。所以，在嵌入式设备中，如 FPGA，轻量级 CPU 上，能达到的实时，才是货真价实的。

**3. 模型小型化成为重要分支：**类似于 tiny YOLO 的模型分支会受到更多关注。模型的小型化是应用到嵌入式设备的重要前提。而物联网机器人无人机等领域还是以嵌入式设备为主的。模型剪枝/二值化/权值共享等手段会更广泛的使用。

# 说点题外话

YOLO 让人联想到龙珠里的沙鲁（cell），不断吸收同化对手，进化自己，提升战斗力：YOLOv1 吸收了 SSD 的长处（加了 BN 层，扩大输入维度，使用了 Anchor，训练的时候数据增强），进化到了 YOLOv2；

吸收 DSSD 和 FPN 的长处，仿 ResNet 的 Darknet-53，仿 SqueezeNet 的纵横交叉网络，又进化到 YOLO 第三形态。

但是，我相信这一定不是最终形态。让我们拭目以待吧！

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlUibnrF66YjobrlEEQQg7oNpns6ahNWRNFX75LAoR1NSejLKbjtGicl7Q/640)

# YOLOv3实景大片儿

这周忙里偷闲，把 darknet 的代码撸了一遍，里面有趣的东西很多。

能看出来作者是有野心的，YOLO 不只是一个目标检测应用，它还是一个完全基于 C 语言的通用神经网络架构，以及很多以此为基础的深度学习应用，比如**基于 RNN 的莎士比亚戏剧剧本自动生成器**：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlqONJvltWKOCsujz9GukGMUIicflhLmOYhPGzz06eiaiaaPBXzV6Dbic8Kw/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSllFDwq85n1hnibtYN7oSDHBlpXE6N55jYyElaTj55ial3iadZEmVCGlajA/640)

**基于策略网络的 darknet 版阿法狗（DarkGo）：**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSltfMeDHdbnY9kE4vnTnIiadvkAYFbLzm5gMOac8pQlDpRvuHwUvQPUSw/640)

**基于 GAN 的 darknet 版 Deep Dream（Nightmare）：**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSl4X5BLRJOIxlBtoyqmmSmCuQShVFichBgEGZNs15zlyPzKd6LToW61Tw/640)

**挑战 SqueezeNet 的压缩网络 TinyYOLO**（Redmon 号称后者比前者更快、小、准）等等。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlich13IIua4RXKmn4ZuFZ1H6icF2DRXuY4bnM2JFu5WSrTavecdOnUh0A/640)

当然，做得最好的还是目标检测。darknet 自带有视频检测，及网络摄像头实时视频检测。

但是，目前网上介绍的 YOLO 的文章多是用的论文和 Github 工程自带的 sample 图片（dog，kite…），其实这不太公平。

在源代码基础上加了一点保存视频的代码（代码链接在文尾），生成了如下的视频，**看看当 Mr Bean 遇到 YOLO 会产生什么样的化学反应！**


**▲**YOLOv3效果展示

查看高清版视频![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/b96CibCt70iaajvl7fD4ZCicMcjhXMp1v6UibM134tIsO1j5yqHyNhh9arj090oAL7zGhRJRq6cFqFOlDZMleLl4pw/640)
https://zhuanlan.zhihu.com/p/36478097

注：本视频基于 coco 数据集，YOLOv3 cfg 和 weights。检测视频没有声音，音轨是后加的。
存在一些识别错误，但是整体来说，亮点很多。

**截取几个瞬间**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSljeFYrwzaPZjjQfESZVFzf15ibJAuNA8qiaRLwF3dry4HF0lmmg4a0NjQ/640)
**▲**性能提升最大的是小物体或者远处的物体识别

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlrjmhYtibyrOoaokT4XSmSZwEp2bx6O8QIAfxOSpGaw2mZDzp4DuByWQ/640)
**▲**重叠遮挡物体的识别

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlsj2hgnmHQfRTwDpCXKMo9MvAXofoKuJGojRzia7HP5l8ibFFicq8FbrrA/640)
**▲**运动模糊物体的识别

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlgO0AjWNRz3fC7JF2tmX74dkibcBugZ3pDz39TzTMtjUoILIicwAkLalw/640)
**▲**高密度汽车的识别

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnFLAP3W4ftC9N5Lcer9xSlWU38rPrVa38Uqr71A6g030cGVia5Qmqwge5OTS40NAQmRLu1Yh61IEw/640)
**▲**密集人群识别

# 相关链接

**[1]. 论文**
https://pjreddie.com/media/files/papers/YOLOv3.pdf
**[2]. 翻译**
https://zhuanlan.zhihu.com/p/34945787
**[3]. 代码**
https://github.com/pjreddie/darknet
**[4]. 官网**
https://pjreddie.com/darknet/yolo/
**[5]. YouTube**
https://www.youtube.com/watch?v=MPU2HistivI
**[6]. 旧版**
https://pjreddie.com/darknet/yolov2/
https://pjreddie.com/darknet/yolov1/
**[7]. 源码分享**
https://github.com/muyiguangda/darknet
**[8]. YOLOv3在Windows下的配置（无GPU）**
https://blog.csdn.net/baidu_36669549/article/details/79798587

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

**点击以下标题查看其他文章：**

自适应注意力机制在Image Caption中的应用
CVPR 2018值得一看的25篇论文，都在这里了
视觉跟踪之端到端的光流相关滤波

Wasserstein距离在生成模型中的应用

[深度学习在CTR预估中的应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488284&idx=1&sn=53bb819e7403360cafb41853d60f98f2&chksm=96e9cc9ca19e458a30e40aa1540a82dac61528b13fd82e0c3a493e77494f512806103a715c68&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgmsvubgibQtWV5t7M3ETKt3bbXiaAothCErMicibic9QCUBpxkuibuht62MGcCTcLyAxqGrsUXbv254InDA/640?)
**▲**戳我查看招募详情

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****作 者 招 募****\#**

**[让你的文字被很多很多人看到，喜欢我们不如加入我们](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)**



**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 进入作者知乎专栏



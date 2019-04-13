
# CMU & Facebook论文解读 | 非局部神经网络（附代码实现） - Paper weekly - CSDN博客


2018年02月07日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：563



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**41**篇文章
本期推荐的论文笔记来自 PaperWeekly 社区用户**@Yadan**。**本文受 NL-Means 在图像去噪应用中的启发，采用非局部均值方法来处理局部特征与全图特征点的关系**。这种非局部神经网络可以很方便地嵌入现有视觉模型中，提高图像及视频分类精度。
如果你对本文工作感兴趣，点击底部的**阅读原文**即可查看原论文。
# 关于作者：罗雅丹，昆士兰大学博士生，研究方向为计算机视觉。

■ 论文 | Non-local Neural Networks
■ 链接 | https://www.paperweekly.site/papers/1191
■ 源码 | https://github.com/AlexHex7/Non-local_pytorch
■ 源码 | https://github.com/titu1994/keras-non-local-nets

# 论文动机

“Non-local”直译为“非局部”。个人的理解是指特征提取时，当前输入数据的特征计算要考虑其它输入数据的信息。举例来说，非局部操作的关注点就是在于如何建立起图像上两个有一定距离的像素之间的联系，如何建立视频里两帧的联系，如何建立一段话中不同词的联系。

一个典型的 CNN 网络是由一系列卷积操作累积起来的。对于使用在图像上的 CNN，每个卷积操作只能捕捉到输入数据的局部信息。整个网络通过局部操作的逐步累积来获得范围较广的信息提取。而 RNN 则是通过循环的方式处理序列输入（如视频每一帧图像的时间序列或者图片上一列像素的空间序列），从而融合非局部的信息。

这篇文章提出三点 CNN 和 RNN 在融合非局部信息上的三个劣势：1. 计算不高效；2. 优化更难；3. 非局部特征的信息传递不够灵活，功能不够强大。当然，这也是因为 CNN 和 RNN 的初衷并不是专门来融合非局部信息的。

在这篇文章中，作者提出了一种非局部模块（non-local block，简称 NL block）的网络结构，来帮助深度网络更好地融合非局部的信息。这对于一些问题是非常重要的。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlEpUfITvqXhso1Qo1os5pS3C6WgdvFcYRibUpQp0pjwUUZEA9Jg7LhMHA/640)

# 方法

从使用在图像去噪问题中的 non-local means[1]方法出发，作者对 NL block 的抽象描述为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlE5TrCiaZ0aiapTWa7OqaydUKw2bXXxrqxgxQseLbmibET04eapSLHKMOMw/640)

以图像为背景来说的话，*i,j*是像素坐标，*x,y*是输入和输出信息。*j*的取值范围是任意的坐标。*C*(x) 是一个归一化常数。*f*(.) 是用来构建*i,j*点处信息关联的二输入函数。*g*(.) 是计算*j*点信息对*i*点影响的单输入函数。

这样的非局部操作其实很常见，从广义的角度来讲可以从多个已有的研究方向来理解这个操作。

首先这是从图像去噪中借鉴来的模型。除此之外，如果*f*(.) 函数输出的是一个概率值，即 0~1 之间的数值，那么这就是前段时间有所讨论的**self-attention**[2]。因为 (*i,j*) 二元坐标对取遍了所有情况，这个定义又很像**dense CRF**[3]，或者可以认为这也是一种图模型。

回到这篇文章中。作者对*f*(.) 有很多不同选择的定义方式。通过实验选定了名为 Embedded Gaussain+Dot product 的方案。对*g*(.) 的定义是一个简单的线性函数。NL block 的输出并没有替代输入，而是和原输入相加，形成一个残差结构（residual），得到 Zi。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlEkhTibLYAHictbLnqbBLwWxLnEgCPsLuL6xkSoxBEGeKMVjKhdUI1pQOw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlE19icia3aDoNp9kC9Hy4hrA6WFJw4GxU69CGe4eGXB9ofn8lGiaCxjDg0A/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlESqbMONXORiba4iaCZ82N0QibDtZK5u9xLaSzDwElr0QkCwlARlvO3t8aA/640)

其中*W*均为待学习的参数，可以将其视为 1x1 卷积的参数。若![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlEI5373ZtYHmanocnRx6kgkfYt0uZ2j6tibXiaGsEb4UnXsFrpODH1rHUQ/640)，那么![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlEgicnTsMM5BxNv2BpLCUdmtsgnXDRM7T9jMxnhAwiaphg7n6tCQrj5YOA/640)就是二阶的 softmax，即 CRF。

*f*(.) 中所做的点积不同于 style transfer 中的协方差或者 bilinear 操作。后两者是把不同通道的特征图两两内积，空间信息完全抛弃，得到的结果是通道数*x*通道数大小的协方差矩阵。*f*(.) 的操作是每个点的特征向量（通道个数的维度）进行内积，空间信息保留了下来，输出还是 HxW 大小。

整个 NL block 也可以从结构图来理解：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlERntZt672BHMQ4zZ4ttWpk7XM6bxC4Ymak37icmnabQLib5IUNKbTQb9w/640)

图中*T*,*H*,*W*代表输入特征的维度。其中*T*是对于视频数据的帧数，对于分割或检测任务，*T=1*。

这里面作者还提到了一些小 trick 可以进一步压缩计算量。比如 1x1 的卷积压缩输出通道数，形成一个瓶颈结构（bottleneck）。或者在*f*(.) 对应的 1x1 卷积之后使用 pooling 来减小*H*,*W*，即采样一部分的*j*，而不是所有的*j*进行信息融合。

可以看到，NL block 可以用作网络设计中的一个补充结构插入到 CNN 中任意的一层之后。因为使用的是 1x1 卷积，因此对输入特征没有尺寸限制。同时整个操作计算量相对较小，额外参数量也很少。

# 实验

作者在视频分类、物体检测、物体实例分割这些很需要非局部信息关联的任务上进行了实验。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlE9gYoo5hNibaKOGBkARYFDicykbwJRbFlYxic3hhyr6CYm0VfdfJ8tZmuQ/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlEJNajEgichiaia6rv2zib4m31hAU9icqouOrg21MbdT8JzaVqDQQ3oA5oYng/640)

其中在 Kinetics 数据集上进行了 ablation study，来仔细研究 NL block 各个细节的有效性。结果表格不再赘述，得到的结论总结如下：

NL block 中*f*(.) 不同的定义方式各有千秋，但是为了更好化可视化使用 embedded Gaussian+dot product，即上文提到的公式所示的方法。

**NL block 放入网络主干的位置**：放在浅层好，高层提升作用下降。

**NL block 变深的作用**：对于主干网络较浅的网络，加深 NL block 能够提升性能。对于较大较深的网络，无论是增加 NL block 还是继续加深主干网络的深度都很难再提升性能。

对视频任务，NL block 同时作用于时空比单独作用于时间域或空间域要好。与 C3D 比较：比 C3D 又快又好。

在 COCO 数据库上的实验结果如下所示。鉴于 NL block 结构的易用性，在平时设计网络时可以考虑添加这样的模块。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkY1BGYTBkKttdD9VBsbmlEfs1icWpBH9XWBk8JBz4hgeFGialvmoyolL7jicRgBkkibf7RRpzk7ia3hag/640)

# 参考文献

[1] A. Buades, B. Coll, and J.-M. Morel. A non-local algorithm for image denoising. In Computer Vision and Pattern Recognition (CVPR), 2005.
[2] A. Vaswani, N. Shazeer, N. Parmar, J. Uszkoreit, L. Jones, A. N. Gomez, L. Kaiser, and I. Polosukhin. Attention is all you need. In Neural Information Processing Systems (NIPS), 2017.
[3] P. Krahenbuhl and V. Koltun. Efficient inference in fully connected crfs with gaussian edge potentials. In Neural Information Processing Systems (NIPS), 2011.

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**

我是彩蛋

**解锁新功能：热门职位推荐！**

PaperWeekly小程序升级啦

**今日arXiv√猜你喜欢√****热门职位****√**

找全职找实习都不是问题
**解锁方式**
1. 识别下方二维码打开小程序
2. 用PaperWeekly社区账号进行登陆
3. 登陆后即可解锁所有功能

**职位发布**
请添加小助手微信（**pwbot01**）进行咨询

**长按识别二维码，使用小程序**
*点击阅读原文即可注册

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/640?)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)
▽ 点击 |阅读原文| 查看原论文



# AAAI 2019 Oral | 把Cross Entropy梯度分布拉‘平’，就能轻松超越Focal Loss - Paper weekly - CSDN博客





2019年01月16日 12:17:03[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：194









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




单阶段物体检测（One-stage Object Detection）方法在模型训练过程中始终面临着样本分布严重不均衡的问题，来自香港中文大学的研究者们在论文 ***Gradient Harmonized Single-stage Detector*** 提出了一个新的视角——梯度分布上看待样本数量和难易不均衡。**直接把 cross entropy 产生的 gradient distribution 标准化到 uniform 就可以轻松训练单阶段物体检测模型。**




该论文已经被 AAAI 2019 会议接受为 Oral 论文，基于 PyTorch+MMDet 的代码已经放出。




作者丨Lovely Zeng

学校丨CUHK

研究方向丨Detection




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCaUqhaCH9ZaDqibXVUlLGRehoq0TOZze3uILbCz3sicVxoURQW1ic8NHz9g/640?wx_fmt=png)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCa0GgCZtMxbR4NwacPPCOPLXGArDYChwSsBVTKytBmVJzwpcY9fuBhzA/640?wx_fmt=png)



# 引言




物体检测的方法主要分为单阶段与两阶段两大类。虽然两阶段的物体检测器在准确率上的表现往往更优，但单阶段检测器因其简洁的结构和相对更快的速度同样得到了研究者们的重视。




在 2017 年，Focal Loss 的作者指出了单阶段检测器中样本类别（前景与背景）严重不均衡（class imbalance）的问题，并通过设计一个新的损失函数来抑制大量的简单背景样本对模型训练的影响，从而改善了训练效果。




而在这篇论文中，**研究者对样本不均衡的本质影响进行了进一步探讨**，找到了梯度分布这个更为深入的角度，并以此入手改进了单阶段检测器的训练过程。 




实际上，不同类别样本数不同并不是影响单阶段检测器的训练的本质问题，因为背景样本虽然大部分非常容易识别（well classified），但其中也会存在着比较像某类物体的难样本（hard negative），而前景类中也有许多网络很容易正确判断的样本（easy positive）。所以产生本质影响的问题是不同难度样本的分布不均衡。




更进一步来看，每个样本对模型训练的实质作用是产生一个梯度用以更新模型的参数，不同样本对参数更新会产生不同的贡献。




在单阶段检测器的训练中，简单样本的数量非常大，它们产生的累计贡献就在模型更新中就会有巨大的影响力甚至占据主导作用，而由于它们本身已经被模型很好的判别，所以这部分的参数更新并不会改善模型的判断能力，也就使整个训练变得低效。




基于这一点，**研究者对样本梯度的分布进行了统计，并根据这个分布设计了一个梯度均衡机制（Gradient Harmonizing mechanism）**，使得模型训练更加高效与稳健，并可以收敛到更好的结果（实验中取得了好于 Focal Loss 的表现）。




# 梯度均衡机制




首先我们要定义统计对象——梯度模长（gradient norm）。考虑简单的二分类交叉熵损失函数（binary cross entropy loss）：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCa3OOMGo5YpS67iahzM0cBERgv2Fj5TyRVqcw87oX59zTymLoia6IjJicFg/640?wx_fmt=png)




其中 p=sigmoid(x) 为模型所预测的样本类别的概率，p* 是对应的监督。则其对 x 的梯度（导数）为：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCajXlpDibS3bddb7zWRJEzg4SqXZckym6WAicCAxJAXkicdEP24IJcH7dMg/640?wx_fmt=png)




于是我们可以定义一个梯度模长，g=|p-p*|。




对一个交叉熵损失函数训练收敛的单阶段检测模型，样本梯度模长的分布统计如下图：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCaa6iaMEk0Q8JKwCxLqDLPdEXBfExjBvePsnbXiam7rl1XOm3U9iancaG7A/640?wx_fmt=png)




可以看到，绝大多数样本都是十分容易预测的（well-classified），这些可以被准确预测的样本所占的比重非常大，正因如此图中采用对数坐标来更清楚地展示分布。




此外，还可以发现在 g 接近 1 的时候，样本比例也相对较大，研究者认为这是一些离群样本（outlier），可能是由于数据标注本身不够准确或是样本比较特殊极难学习而造成的。对一个已收敛的模型来说，强行学好这些离群样本可能会导致模型参数的较大偏差，反而会影响大多数已经可以较好识别的样本的判断准确率。




基于以上现象与分析，研究者提出了**梯度均衡机制**，即根据样本梯度模长分布的比例，进行一个相应的标准化（normalization），使得各种类型的样本对模型参数更新有更均衡的贡献，进而让模型训练更加高效可靠。




由于梯度均衡本质上是对不同样本产生的梯度进行一个加权，进而改变它们的贡献量，而这个权重加在损失函数上也可以达到同样的效果，此研究中，梯度均衡机制便是通过重构损失函数来实现的。




**为了清楚地描述新的损失函数，我们需要先定义梯度密度（gradient density）这一概念。**仿照物理上对于密度的定义（单位体积内的质量），我们把梯度密度定义为单位取值区域内分布的样本数量。




具体来说，我们将梯度模长的取值范围划分为若干个单位区域（unit region）。对于一个样本，若它的梯度模长为 g，它的密度就定义为处于它所在的单位区域内的样本数量除以这个单位区域的长度 ε：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCamtRlQocbJREShR7vKcYiaNibNsAOO7Eu67V3IqtDueR5GaUYOxaPKvBw/640?wx_fmt=png)




而梯度密度的倒数就是样本计算 loss 后要乘的权值：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCaHLM3FN2PRhDnxdabKCUibkbcZ4WXiaNIKsicWWI1jmHGqYMRWBSYsjHVg/640?wx_fmt=png)




这里乘样本数量 N，是为了保证均匀分布或只划分一个单位区域时，该权值为 1，即 loss 不变。 




由于这个损失函数是为分类设计的，所以记为 GHM-C Loss。下面我们通过与传统交叉熵算是函数以及 Focal Loss 的比较，来进一步解释 GHM 的作用：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCaNOutZHZib6mTtomzxBj6M1wTCHTAmVVNZvAh3gqmJVRHYU3KsUCdVkw/640?wx_fmt=png)




左图为样本梯度模长的分布。中图为不同损失函数对样本梯度产生的作用，横坐标为在交叉熵（CE）损失函数下样本的梯度模长，纵坐标为新的损失函数下同样的样本新的梯度模长，由于范围较大所以依然采用对数坐标展示。其中浅蓝色的线为交叉熵函数本身，作为参考线。




可以看到，Focal Loss 本质上是对简单样本进行相对的抑制，越简单的样本受抑制的程度越大，这一点和 GHM-C 所做的均衡是十分相似的。此外，GHM-C 还对一些离群样本进行了相对的抑制，这可以使得模型训练更具稳定性。




右图为不同损失函数下，各种难度样本的累计贡献大小。由此可以看出，**梯度均衡机制的作用就是让各种难度类型的样本有均衡的累计贡献。**




在分类之外，研究者还对于候选框的回归问题做了类似的统计并设计了相应的 GHM-R Loss。




需要指出的是，由于常用的 Smooth L1 Loss 是个分段函数，在 L1 的这部分倒数的模长恒定为 1，也就是偏差超过临界值的样本都会落到 g=1 这一点上，没有难度的区分，这样的统计并不合理。为了解决这个问题，研究者引入了 ASL1 Loss：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCah79ct7v9G0CDuyebclneYlGK6heWibibwfXKjpb0CRoLThvAgLR4iaj9A/640?wx_fmt=png)




在此基础上对梯度模长的分布进行统计并实施均衡化的操作。




# 实验结果




在 COCO 的 minival 集上，GHM-C Loss 与标准 Cross Entropy Loss，使用 OHEM 采样下 Cross Entropy，以及 Focal Loss 的比较如下：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCamn16L6tHdRHTLMXxuJWic9OXYJGF96wVneS2aYictThdQvcgIOAc2YHg/640?wx_fmt=png)




GHM-R 与 Smooth L1 Loss 以及 ASL1 Loss 的 baseline 比较如下：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCanmm5ibQxmh17JDsWfmjvNfoCywpoX7Y2RSRMDiaraDKicj7lj8szbqzQQ/640?wx_fmt=png)




在 COCO test 集上，GHM 与其他 state-of-the-art 的方法比较如下：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCagZzMZXWXiboJsolsWRNp1qXOKJWvCKdYaZY4hJQG1Rtyliaia8teen78Q/640?wx_fmt=png)




此外，在 AAAI 2019 的演示文稿中，研究者还展示了在 pascal voc 2007 这样的小数据集上，GHM 相对于 Focal Loss 不需要过多的 warmup iteration 就可以保持训练的稳定：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmvF8pibqFA4gR7jibX1MDHCaPLu2G0hhqSet4Zgv1ib1j1tiagib8Y2G9M6RW9MibT6El3bbWJB3nkPDZg/640?wx_fmt=png)




# 讨论




这篇研究的主要贡献是提供了一个新视角，**较为深入地探讨了单阶段检测中样本分布不均衡所产生的影响及解决方案。**




此研究对梯度模长的分布进行统计并划分单位区域的方式，实际上可以看作是依据梯度贡献大小对样本进行聚类的过程。而这里的梯度只是模型顶部获得的偏导数的大小，并不是全部参数对应的梯度向量，所以聚类依据可能有更严谨更有区分度的选取方式，然而统计整体参数的梯度分布会极大增加计算量，所以本研究中的统计方式仍是一种快速且有效的选择。




此研究进行均衡化操作实际上是以各梯度模长的样本产生均匀的累计贡献为目标的，但是这个目标是否就是最优的梯度分布，暂时无法给出理论上的证明。从实验结果上，我们可以认为这个目标是明显优于无均衡的分布的。然而研究者认为，真正的最优分布难以定义，并需要后续的深入研究。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看更多往期内容：**




- 
[自动机器学习（AutoML）最新综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492654&idx=1&sn=b9047d5cca7657f02dc7f6685ef04037&chksm=96ea3baea19db2b8dc1c1267801d0c585b3cf072531af86abdeb73c6fb4c07dc3325c2d13d57&scene=21#wechat_redirect)

- 
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)

- 
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)


- 
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)

- 
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)

- 
[两行代码玩转Google BERT句向量词向量](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493033&idx=1&sn=1ae1cd347126b10d6a857cd9bba7b601&chksm=96ea3a29a19db33f3c07723ed6e5ecbb8d2ff1b1617f1cf0d39cb3cc1e6e9c325cc29147d58d&scene=21#wechat_redirect)

- 
[AI Challenger 2018 机器翻译参赛总结](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494103&idx=1&sn=fc372862e0369b1f6a943bf997f6fc1b&chksm=96ea3657a19dbf4108bbc4179e779aa04ef05fe84f0013fa6425b0cd7e761e9880917361c4c1&scene=21#wechat_redirect)

- 
[TensorSpace：超酷炫3D神经网络可视化框架](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492746&idx=1&sn=921d7315a973b85dd4e802cb5fd456fb&chksm=96ea3b0aa19db21c48841ddcee38592a3c086ae8fa1a9893cf46ff974f0f38fb350bcd528265&scene=21#wechat_redirect)

- 
[深度长文：NLP的巨人肩膀（上）](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493520&idx=1&sn=2b04c009ef75291ef3d19e8fe673aa36&chksm=96ea3810a19db10621e7a661974c796e8adeffc31625a769f8db1d87ba803cd58a30d40ad7ce&scene=21#wechat_redirect)

- 
[NLP的巨人肩膀（下）：从CoVe到BERT](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493731&idx=1&sn=51206e4ca3983548436d889590ab5347&chksm=96ea37e3a19dbef5b6db3143eb9df822915126d3d8f61fe73ddb9f8fa329d568ec79a662acb1&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。




📝 **来稿标准：**

• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向） 

• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接 

• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志



**📬 投稿邮箱：**

• 投稿邮箱：hr@paperweekly.site

• 所有文章配图，请单独在附件中发送 

• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通










🔍




现在，在**「知乎」**也能找到我们了

进入知乎首页搜索**「PaperWeekly」**

点击**「关注」**订阅我们的专栏吧







**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 下载论文 & 源码





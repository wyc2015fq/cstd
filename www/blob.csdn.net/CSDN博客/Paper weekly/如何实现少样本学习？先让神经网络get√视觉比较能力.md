# 如何实现少样本学习？先让神经网络get√视觉比较能力 - Paper weekly - CSDN博客





2018年05月02日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：400












![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?wxfrom=5&wx_lazy=1)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **66** 篇文章


本期推荐的论文笔记来自 PaperWeekly 社区用户** @Cratial**。**本文提出构建一个关系网络（Relation Network）来让其学习如何比较（Learning to Compare），从而实现少样本学习（Few-Shot Learning）。**

 如果你对本文工作感兴趣，点击底部**阅读原文**即可查看原论文。

# 关于作者：吴仕超，东北大学硕士生，研究方向为脑机接口、驾驶疲劳检测和机器学习。




■ 论文 | Learning to Compare: Relation Network for Few-Shot Learning

■ 链接 | https://www.paperweekly.site/papers/1817

■ 源码 | http://suo.im/4W4mdD




# 引出主题




近年来，深度学习之所以取得如此大的成功，主要在于目前拥有的海量数据和强大计算资源，尤其是在图像识别方面。因此，**如何实现深度学习的快速学习，是深度学习研究的一大难题**。




对于人而言，在识别物体的时候，仅需要少量图像或不需要图像，而根据对物体的描述就能基于以往的经验知识实现对物体的识别，这是为什么呢？**因为我们有先验知识，我们会利用自己的先验知识来进行学习**。例如，经常使用老式诺基亚手机的人也能很快地学会如何使用智能机。




**如何实现这种快速学习呢？****元学习就是实现方法之一**。元学习，英文名叫 Meta Learning，也叫做 Learning to Learn，即学会学习。如何让神经网络实现元学习？这里提供了元学习的相关知识 [1]。 




**本文利用对比关系来实现元学习**，作者认为人在识别图像时是通过比较图像与图像之间的特征来实现识别的，即少样本学习。




如对于刚出生没多久的小孩子来说，他们也能很快地识别出什么是“鸭”和“鹅”，即使他们并没有见过几次，因为我们的视觉细胞可以自动地提取图像的特征（如轮廓、光照等），然后对比我们以往的经验就能对图像进行识别了。这篇论文的 Relation Network（RN）就是根据这种思想设计的。




# 系统结构与方法




** 数据处理 **




**本文将数据分为 training set、support set 和 testing set 三部分**，其中 support set 作为对比学习的样例，它拥有和测试数据一样的标签，在测试过程中，可以通过与测试数据的对比来实现对测试数据的识别。




对于包含 C 个不同的类别，每个类别有 K 个样本的 support set，本文称其为 C-way，K-shot。为了实现对网络的训练，本文将 training set 分成和 support set 及 testing set，文中将其分别称为 sample set 与 query set。 




** 模型 **




** one-hot **




本文提出的 RN 包含两部分，一部分为嵌入单元 *fφ*，用来提取图像的特征，另一部分为关联单元，用来计算两个图像的相似度，如图 1 所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglnC1DgkA9sSJdI0TfCatk4k0Qy3J9EniaOytzQAtLDLqPiagnMCdKoIbZKvvraIq9OJ6HXhVdRE0iaA/640)

**▲ **图1




**这是一个明显的 5-way，1-shot 模型**。在训练时，利用 training set 来对网络进行元学习，用 sample set 中的数据与 query set 中的数据做对比。在嵌入单元分别获得两者的特征、然后对特征进行连接后送入到关联单元计算关联系数，如公式 1 所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglnC1DgkA9sSJdI0TfCatk4QkYKe09meFribhx5JDjzay32mh0Y9rbQXrgDmDZfsEYCNL5Tzq3MLkA/640)




其中，*fφ* 代表嵌入单元，*C*(*fφ*(*xi*),*fφ*(*xj*)) 代表将特征连接一起，*gϕ* 代表连接单元。




** K-shot **




对于各类数据，如果仅有 K(K>1) 个样本，则将同一类的 feature_map 相加，few-shot 的网络模型下图所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglnC1DgkA9sSJdI0TfCatk48ibxfNOBh26NficiaQAibniachnmEGsSNbibNRQwO4O07E98sicmQrYMtOPbg/640)

**▲ **图2




**Zero-shot **




zero-shot 和 one-shot 类似，只不过这里将 support set 中的图像换成了语义向量，嵌入单元也做了修改。zero-shot 的网络结构如图 3 所示，DNN 表示训练好的模型，如 VGG、Inception 等。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglnC1DgkA9sSJdI0TfCatk4ZDGoRr1YYvQdibhbicwTKNzpG2sNmF90cAUaOUSOnjOguJc4Yvx2rEAg/640)

**▲ **图3




# 实验结果与分析




作者分别在 Omniglot 和 miniImageNet 数据集上测试了 few-shot，在 Animals with Attributes (AwA) 和 Caltech-UCSD Birds-200-2011 (CUB) 上测试了 zero-shot。所有的程序都是基于 PyTorch 实现的。




**Few-shot **




** Omniglot **




Omniglot 包含 50 个不同的字母，共计 1623 类字符，每一类由 20 个不同的人创作。为了增加数据量，本文还对图像进行了旋转变换，分别对 5-way 1-shot、5-way 5-shot、20-way 1-shot 和 20-way 5-shot 集中情况展开了实验，实验结果如下表所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglnC1DgkA9sSJdI0TfCatk458mwkUXaOPswIs6PLPyicxPDXTg8YEHaZ9kntmtqc9MPy0kWId5XUkA/640)



** miniImageNet **




miniImageNet是从 ImageNet 分割得到的，具体分割方法参照 [2]。本文在 miniImageNet 上进行了 5 way 1-shot 及 5 way 5-shot 的实验，实验结果如下表所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglnC1DgkA9sSJdI0TfCatk4aIzTIPt5a9stylSxJlSbyLRekZQ9AXicYcd9FkmhSTSEeQ8iagicMbkxA/640)




**Zero-shot **




RN 在 AwA 及 CUB 上的准确率和其他方法的对比如下表所示。




0-way 1-shot 和 20-way 5-shot 集中情况展开了实验，实验结果如下表所示。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglnC1DgkA9sSJdI0TfCatk4pIXjY5klDDhwYrsMyM9fZ0PgFnTlKxZkNg6HbTzK3cy9nW2UUNicygw/640)




# RN为什么能工作




这部分为本论文最核心的内容，论文指出，**之前的 few-shot 工作都是预先指定好度量方式的**，如欧式距离或余弦距离，学习部分主要体现在特征嵌入方面。




但是**该论文同时学习了特征的嵌入及非线性度量矩阵（相似度函数），这些都是端到端的调整**。通过学习到的相似性矩阵比人为选定的矩阵更具有灵活性，更能捕获到特征之间的相似性。 




为了证明 RN 的有效性，作者分别使用马哈拉诺比斯度量矩阵方法 4(c)、马哈拉诺比斯度量矩阵 + 多层感知机 4(d) 及 RN4(b) 对 query set 的匹配情况，通过和 4(a) 相对比可以看出，RN 的匹配效果最好。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglnC1DgkA9sSJdI0TfCatk4YiaI1iabOONGPAEOHicPdNJxgw5I5PBEdkbDTdzS25aOlv3CnMRGVjczA/640)

**▲ **图4




图 5 左边是原始情况下 Omniglot 中图的关系，其中青色是和样例点（黄色）相匹配的图像，紫色是和样本点不匹配的图像。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglnC1DgkA9sSJdI0TfCatk46Ua4cyI0oUI5WCibpYCIP4lpe4HOAbLLDbvZVqBbcF7ia0uF09H6UQrw/640)

**▲ **图5




从图中可以看出，使用欧式距离或余弦距离都不能实现对样例点的正常匹配。但通过对 RN 的倒数第二层进行 PCA 降维，得到的分布图如图 5（右）所示，可以看出匹配的与无法匹配的两类样本变成线性可分的。



# 相关链接




**[1] 百家争鸣的Meta Learning/Learning to learn**

https://zhuanlan.zhihu.com/p/28639662

**[2] Matching Networks for One Shot Learning**

https://papers.nips.cc/paper/6385-matching-networks-for-one-shot-learning

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)




**点击标题查看更多论文解读：**




- 
[脑洞大开的机器视觉多领域学习模型结构](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488200&idx=1&sn=33d6f51d83357c16c0aa19fe8dfab314&chksm=96e9cd48a19e445ee3a1782c237a996b23c1cd85a6049cd8cfab5e864d7a92ae63ba4ad619d1&scene=21#wechat_redirect)

- 
[当前深度神经网络模型压缩和加速都有哪些方法？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488630&idx=1&sn=894b06c31b37ccdad3e9bfdd7323a33f&chksm=96e9cbf6a19e42e0c666d6727430a39fe4e09db047c3cfc0465a34923b87a36dfbe7585fe339&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487791&idx=1&sn=6270944a99f7d1e563ff6ffc7887d764&chksm=96e9ceafa19e47b926f78fad80956633413b32b7a8cc3c5dea03686a12940302951874062636&scene=21#wechat_redirect)[新型RNN：将层内神经元相互独立以提高长程记忆](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488167&idx=1&sn=2a60a4a52c9c1049642ddaee564acc6d&chksm=96e9cd27a19e443185cec0a802450b50b4b08e0e552907fc371f1de22c98e192b1b286e4300c&scene=21#wechat_redirect)

- 
[视觉跟踪之端到端的光流相关滤波](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488603&idx=1&sn=f25e87ea91a286703d2adb13b144e115&chksm=96e9cbdba19e42cd634ca5c127ffebb6a39152064c1b28e7f8dc06c8c233dd8ce316cf4e674b&scene=21#wechat_redirect)

- 
[CycleGAN：图片风格，想换就换](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487791&idx=1&sn=6270944a99f7d1e563ff6ffc7887d764&chksm=96e9ceafa19e47b926f78fad80956633413b32b7a8cc3c5dea03686a12940302951874062636&scene=21#wechat_redirect)

- 
[基于GAN的字体风格迁移](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488103&idx=1&sn=bcf2c7d62905fcd74624063d65e52329&chksm=96e9cde7a19e44f17c9d6daf5901028ea82ea64850bbf027522b886dc0461e87c63de4f9a3e6&scene=21#wechat_redirect)

- 
[基于置信度的知识图谱表示学习框架](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487615&idx=1&sn=066f4779b8e2f118ff138dda945420a1&chksm=96e9cfffa19e46e97cc173ec40f33222275678967751511b90644e141a5dba03a6c97d1711aa&scene=21#wechat_redirect)








![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglnC1DgkA9sSJdI0TfCatk47ChHgYagIk4yTQv7LQY0FuPIoHMkAvsya90cPaEGS01y8caLtrecMg/640)

**▲ **戳我查看招聘详情




**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****岗 位 推 荐#**




****[企保科技招聘对话机器人方向自然语言处理工程师](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488603&idx=2&sn=7320cb23efba3e7b5a381be83b7fe3ad&chksm=96e9cbdba19e42cd5840d3d51e86da4709b3d5273b2cf2512c32d84ab2b42ac4e7f13bf9ba63&scene=21#wechat_redirect)****







******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)

▽ 点击 | 阅读原文| 查看原论文





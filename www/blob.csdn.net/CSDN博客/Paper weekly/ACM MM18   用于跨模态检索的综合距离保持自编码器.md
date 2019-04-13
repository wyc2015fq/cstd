
# ACM MM18 | 用于跨模态检索的综合距离保持自编码器 - Paper weekly - CSDN博客


2019年03月11日 12:24:39[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：123


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨黄澄楷
研究方向丨多媒体信息检索/内容理解

本文是发表在 MM18 上的一篇跨模态检索文章，**作者提出了一种采用综合保持距离的自编码器（CDPAE）的新颖方法，用以解决无监督的跨模态检索任务。**

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CZtCUql9EEeIzFMoLydghHuokZn0VUcxnvhZ333lb7qIc8lsPwbvkng/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CZ0iaWW07c807wjzpnUFByGzPxibRHSvTJALuayDcfoBPqro5LhMQuPuQ/640?wx_fmt=png)

之前的无监督方法大部分使用属于相同对象的跨模态空间的成对表示距离进行度量学习。但是，除了成对距离之外，作者还考虑了从跨媒体空间提取的异构表示距离，以及从属于不同对象的单个媒体空间提取的齐次表示距离，从而达到了更高的检索精度。

# 研究动机

虽然先前的无监督跨模态检索方法已经有了不错的表现，但是仍然有两个问题叩待解决，**第一，如何减少特征中冗余的噪声的负面影响。**

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0Cb3tpZYKcFP2vqM4xLOBx7FRPRNW5Nvqlye4IY7gIc5PJeqzicodmSeg/640?wx_fmt=png)
**▲**背景中的SIFT特征会影响Cat图像的检索

**第二，如何直接使用不同对象的表示（representation）来表达它们之间的关系（relationship）。**

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CEQu3iaHl7FKU5weEKSggWLb0ZCloBTmGohD7IeIuhEeQS7A0YCOus2w/640?wx_fmt=png)

即在大多数非监督方法中，不考虑虚线的关系。这两个问题在无监督跨模态检索的研究中涉及的较少。

# 研究方法

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CMOsZHWgvLCg92RUAmwGhBJMUEWpEzcY2tDGgic4Bp9NNWhzK1CGxF8A/640?wx_fmt=png)

上图就是作者提出的 CDPAE 的框架结构图，总体上看，CDPAE 包含四个并行的去噪编码器，并定义了综合的保距公共空间，其中根据输入保留三种距离，然后使用联合损失函数将自编码器的重构损失和相关损失结合起来。最后，还提出了一种无监督跨模态相似度的度量方法。

具体来看，**CDPAE 包含四个部分：去噪编码器、综合保距空间、联合损失函数和无监督跨模态相似度测量**，由于数据集的限制，本文与大部分其他跨模态检索任务一样，只进行图文互搜的实验。接下来分别对每个部分进行介绍。

**1. CDPAE 的第一部分由四个去噪编码器组成**，其中两个提取图像相关的特征，另外两个与文本特征相关，去噪的自编码器负责相同的模态，它们共享相同的参数，因此相同模态的表示也具有相同的转换。

在具体的训练迭代中，从两个对象中提取的两种模式之间的四种表示形式用作输入。如：图中海鸥图、描述海鸥图的文本、自行车图、描述自行车的文本作为输入。

在去噪自动编码器中，将固定数量的输入分量随机设置为零，其余的保持不变。该方法模拟了从输入端去除冗余噪声的过程；因此，它减少了冗余噪声的负面影响。此外，归零过程可以看作是一种数据扩充的过程，它加强了从不同模态中提取的表示中局部结构之间的联系。

去噪自编码器的重构损失定义为:

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0C87qiawWAWzZvubqY2PyO10lYF3kGBhAMxNplDlTRu0GmOQibVyAtozsw/640?wx_fmt=png)

V={v} 代表的是图像的数据集，T={t}代表的是对应的文本数据集，根据之前提到的输入方法，使用自编码器从提取两组来自两个对象对应的特征(图像文本对特征)，(vi,ti)-->（海鸥图的图像特征和文本特征）和(vj,tj)-->（自行车的图像特征和文本特征），av,wv,θv 表达的是图像自编码器的参数，at,wt,θt代表的是文本自编码器的参数，Z(*)是随机置零过程，F(*)是编码过程，G(*)是解码过程。

**2. 第二部分是综合保距空间的构造**：CDPAE 使用余弦距离来测量相同模态空间中的特征相似性。测量的公式:

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0C5xQjoejpSfSC2d6MLL3KnmLJwzQFgeZroSUKWK6xEjoXaaglVIvE1A/640?wx_fmt=png)

**在综合保距空间中有三种距离：成对距离、异质距离、齐次距离**，分别给出定义：

**a. 成对距离的损失：**

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0Ccg6icgpSeIgVq1cmpzlFkGvwg2HNCEKNP5EqYDMVD00X6wV19cDNdcw/640?wx_fmt=png)

就是其他无监督跨模态检索都考虑的距离。其中 D 为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CQKndQ6vr6qw3TDrQevcA3xgnVtmNuHkHmuOl3mcabxn0u6PEdsCZYQ/640?wx_fmt=png)

这个距离的作用是：成对的距离会导致公共空间，其中属于相同对象的不同模态的表示会聚在一起（海鸥的图像文本聚在一起、自行车的图像文本聚在一起）。

**b. 异质距离的损失：**

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CajQ7aqmaiby7Ozpcic2DgxWOLxYZtp0ZYsKlySbkQvmplKpdSQkKQgEQ/640?wx_fmt=png)

反映了不同对象在不同模态中的表示之间的关系，这里度量的时候限制它们与原始模态空间相对应的对象之间的距离一致。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CIK0zyNkWrZb3OoN0iaQNIZ8EwqgibBVCyiavJwdYC1G654VeBpHqIQCPQ/640?wx_fmt=png)

**c. 齐次距离的损失：**

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CMicMu9YfUKsTnMSFMC4FXV2nnCSH5QVITDMUqRYPvQJ2ia8ZVkI8MzEw/640?wx_fmt=png)

齐次距离反映的是同一模态下来自不同对象的表示之间的关系，因为每次迭代中，都计算相同两个对象之间的异质与齐次距离，所以设置它们的值相同。

所以综合的保距空间如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0C9nUP3C9lPAicEwia6vQl3WuRu3kEYFs776ZpE5HKdbbJmaP9cLmhjC0Q/640?wx_fmt=png)

**3. 然后又使用了一种联合损失函数**，同时计算去噪自编码器的重构损失和综合保距公共空间的相关损失：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0COJ0MiaHumkkjDnOJISHkBMSH3dKat8M1Ox6ibN5CicnvZKmwLGTGShe3g/640?wx_fmt=png)

**4. 最后作者又提出了一种新型的无监督跨模态相似度度量方法**，在公共空间中，训练数据集中变换后的特征之间的距离通常会比测试数据中的距离更具有可信度。

所以讲两个特征之间的相似性定义为基于 KNN 分类器的边缘概率，该分类器用于对训练样本中的每个模态的表示进行分类，两种表示的相似性可以定义为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CeicX1KPKgnSiaTNPuic0RJib2kibdTzqyQHN5rl9pq6r2KRTXuFLa8DqkUQ/640?wx_fmt=png)

注意：这里的 pi/qj 分别是图像模态/文本模态的 top k 近邻样本（这里的 top k 近邻样本不区分模态）。同时，假设两个表示之间的距离反映了它们属于同一语义范畴的可能性，因此，如果训练数据集中的两种表示形式成对对应的话，它的可能性就是 1，否则：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CzSM3tRgtmiaI3poMP3I1zibib9Et4p8Strib275IRdQIWBkRicC1ZLQILdg/640?wx_fmt=png)

D 采用的是余弦相似距离，取值范围是 0~1，距离越小，对应的表示属于同一类别可能性就越大，进一步，去定义一个测试样本表示与其 k 个最近的训练数据属于同一个类别的条件概率为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0CtOu8GQvsnDCS8KKDzOibqKYg3PKg0aI7KCic27ydicY9DMVKDltxts5Tw/640?wx_fmt=png)

# 实验结果

作者在 Wikipedia，NUS-WIDE-10k，Pascal Sentence 以及 XMedia 数据集上进行了实验：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0C65j5tQTbaYg8db2tuibyMYtItDkp6vTPjsnT6X5gia9BeC2z0GcKmwow/640?wx_fmt=png)

图表显示返回的是 MAP@50 的结果，加 * 的是有监督的方法，三角形代表的是半监督的方法，剩下的都是无监督方法。另外，作者还用 t-SNE 可视化了综合保距空间的数据分布：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgltkfj8CcJEeGOQZG7Xvh0Cico3gxTOVzQgiaNMEAUmskiaic0pKianCSqjhmic7TtDKoeKZeZBG9LBbADQ/640?wx_fmt=png)

上图显示的是 Wikipedia-Multiple 数据集中不同公共空间的 t-SNE 可视化。可以看出，在成对保持距离的公共空间中，图像和文本的表征往往是混合的。然而，来自相同类别的表示并没有得到令人满意的聚类，这就是传统无监督跨模态检索只使用的距离。

在异质的保距离公共空间中，图像和文本的表征就有明显的区别，这是因为模内距离远小于模间距离。此外，在相同距离保持的公共空间中，来自相同模态的表示按其各自的类别聚在一起。

在综合的保距离公共空间中，图像和文本的变换表示达到了最佳的方式分布。大量具有相同语义标签的表示形式被聚集在一起，而与它们的模态类型无关。这表明，综合保距离公共空间具有其他三种保距离公共空间的优点，对于跨模态检索任务是非常有效的。

# 结论与点评

与之前的无监督跨模态检索方法相比，**本文最大的亮点在于引入了不同对象在不同模态间的距离以及不同对象在相同模态间的距离**，就是文中提出的学习到的一个综合保距空间，这是之前大部分跨模态检索方法没有考虑到的。

然后利用联合损失函数将距离的损失函数与自编码器重构损失一起训练，达到一个很好的效果，与目前所有的无监督跨模态检索方法相比，平均性能高出 12.5%，与半监督与有监督方法相比，在多个数据集上表现也有前三的水平。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多往期内容：**

[Airbnb实时搜索排序中的Embedding技巧](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494488&idx=1&sn=452ac80c593a9b31252031eac38d0e01&chksm=96ea34d8a19dbdce940ed25bb93507aa6c4d118f84dd0bb965b060f232fe5d41894bbc9edcb6&scene=21#wechat_redirect)
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)

[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)
[深度思考 | 从BERT看大规模数据的无监督利用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494978&idx=1&sn=529b8f661b13c7b6b729e9a926b0737a&chksm=96ea32c2a19dbbd43456086a2186480fa548fa982d536564e167739ef46282eaf6fca7316ced&scene=21#wechat_redirect)

[AI Challenger 2018 机器翻译参赛总结](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494103&idx=1&sn=fc372862e0369b1f6a943bf997f6fc1b&chksm=96ea3657a19dbf4108bbc4179e779aa04ef05fe84f0013fa6425b0cd7e761e9880917361c4c1&scene=21#wechat_redirect)
[小米拍照黑科技：基于NAS的图像超分辨率算法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495166&idx=1&sn=a158e603651bc4f26836151a9113e856&chksm=96ea327ea19dbb68b8987aca041bb21579a35b1c679e91fd2368c7f2fb7acd58508cd531bdfe&scene=21#wechat_redirect)
[异构信息网络表示学习论文解读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495219&idx=1&sn=b3a29b833fe8438e12b600650ec0245a&chksm=96ea31b3a19db8a5a335cc445f04eb13d5b1ee5451d688544240cce27dbc953993da4e842ab6&scene=21#wechat_redirect)
[不懂Photoshop如何P图？交给深度学习吧](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495276&idx=1&sn=99a84e81ba9f9a90b323c21c8c905765&chksm=96ea31eca19db8faca5cdd6447136e34a61a8a5a2d8e01489657f791841d30e0c78901a87400&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。

📝**来稿标准：**
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
▽ 点击 |阅读原文| 获取最新论文推荐



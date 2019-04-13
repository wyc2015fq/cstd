
# CMU论文解读：基于GAN和VAE的跨模态图像生成 - Paper weekly - CSDN博客


2018年08月20日 12:05:17[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：733


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnC9iaic8hDbiadLafh7TtCZS6icEYddVmMqZBksDV7cQkKmAu95h53FxyibqmZOS1yQgHibJT0WYD2s1Zw/640)
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**98**篇文章
本期推荐的论文笔记来自 PaperWeekly 社区用户**@TwistedW**。跨模态的图像生成在模态差异大的情况下是很难实现的，**本文****将一个模态下编码得到的潜在变量当做条件经过 GAN 映射为另一个模态下的潜在变量，实现了模态间潜在变量之间的相互映射，从而实现了跨模态下的相互生成。**
如果你对本文工作感兴趣，点击底部**阅读原文**即可查看原论文。

# 关于作者：武广，合肥工业大学硕士生，研究方向为图像生成。
■ 论文 | Cross Domain Image Generation through Latent Space Exploration with Adversarial Loss
■ 链接 | https://www.paperweekly.site/papers/2223
■ 作者 |Yingjing Lu

跨模态之间转换生成在模态间差异大的时候是相对困难的，**将一个模态编码得到的潜在变量作为条件，在 GAN 的训练下映射到另一个模态的潜在变量是本文的核心**。虽然这篇论文只是预印版，但是文章的这个跨模态潜在变量相互映射的思想是很有启发性的。

# 论文引入

人类很容易学会将一个领域的知识转移到另一个领域，人类可以灵活地学习将他们已经在不同领域学到的知识连接在一起，这样在一个领域内的条件下，他们就可以回忆或激活他们从另一个领域学到的知识。

深度生成模型通过将它们映射到潜在空间来编码一个域内的隐式知识是被广泛使用的，可以控制潜在变量通过条件在学习域内生成特定样本。然而，与人类相比，**深层生成模型在从一个域到另一个域之间建立新连接方面不够灵活**。换句话说，一旦它学会了从一组域条件生成样本，使其用于生成以另一组控制为条件的样本通常很难并且可能需要重新训练模型。

跨域转换对于建立模态之间的联系是重要的，能够让神经网络更加地智能化，有一些方法提出来解决跨模态的问题。将条件编码映射到无条件训练的**VAE**[1]，以允许它用用户定义的域有条件地生成样本，并取得了很好的效果。

但是一个限制是那些条件是通过 one-hot 专门定义的。这样做需要特征工程，并且想要对一些隐含的特征进行条件化时效果较差，例如使用来自一个场景的图像作为条件来生成学习域中的相关图像。在*Unsupervised Cross-domain Image Generation*[2]中训练端到端模型，假设这两个域相关性，再循环训练。

今天要说的这篇论文，在更少的模态假设下实现了跨模态的相互生成。**总结一下这篇论文的优势：**

利用 GAN 实现了跨模态的潜在变量之间的相互映射

在较少的假设下实现跨模态的转换


# 实现方法

我们一起来看一下文章实现的模型框架：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCwsnicDuTYePTdgsWGMYib3jygwZjKuUmOQAxh7uXiaWlD7n3FmcrN2fuA/640)

下标为 1 的代表着是模态 1 下的数据变换，相对应的下表为 2 的代表着模态 2 下的数据变换，上标代表着模态下的类别，i 类和 j 类。f 代表着编码器网络，g 代表着解码器网络，G 代表了实现映射的生成器，D 代表了对应的判别器，噪声 ϵ∼N(0,1)。

我们看到上下两路对应的是 VAE 实现框架，也就是上路为模态 1 对应的 VAE1 是一个完整的流程，下路为模态 2 对应的 VAE2 也是一个完整的流程，当训练 VAE1 和 VAE2 到收敛状态时（训练完成），此时的编码器已经可以很好的将模态数据编码到隐藏空间了。

我们分析由模态 1 到模态 2 的变换。模态 1 的第 i 类图像![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCpRkytBhEibCvEtD4KKp1Nrzcd0vT7k0WshjjiaC7ZY9x2D1RFEd7tzZA/640)作为输入到模态 1 的编码器![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCKrJCWhyc8UMSrrSDLWExU8iak44iaP77awJzBibiavibSmTrxJAn9xCXGIQ/640)从而得到 i 类的潜在变量![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpClYqeAibM7DNav0ClbPdgXZMhk0DXkJ4DHakZD95jhOl5XhBhBocYE9Q/640)，将![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpClYqeAibM7DNav0ClbPdgXZMhk0DXkJ4DHakZD95jhOl5XhBhBocYE9Q/640)作为条件，ϵ 作为噪声输入到生成器 G1 下从而实现映射到，模态2第j类的图像经过编码得到的潜在空间下，此时由生成器 G1 得到的映射记为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCeD1niayylwX3CQ7OybNDib9dBYFDKOBPq3Lxb54lD6FfDea8KjGoFtPg/640)。

判别器 D2 的目的是为了判别真实模态 2 下 j 类图像编码得到的![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCOKKRGvEAHEpXyDllB71aefmWgMoyGgDqiaucP1VlicPuTHHicibvNoQmgw/640)和生成的![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCeD1niayylwX3CQ7OybNDib9dBYFDKOBPq3Lxb54lD6FfDea8KjGoFtPg/640)是否是匹配，如果不匹配就认为是假，如果匹配了那就说明生成器 G1 成功欺骗了判别器 D2 从而实现相互博弈共同提高。相对应的，由模态 2 也可以向模态 1 映射，这个过程和模态 1 到模态 2 是相似的。

分析了框架的实现方法我们再看一下实现的损失函数上的设计，首先是**VAE 对应的损失优化**，这个大家估计也都熟悉了：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpClZBwNarZOnMhzNCMaYXgjTm4zz9o4Ck3q7VHL93d6wI9xhzHkeibskQ/640)

ELBO 下参数的定义和原始 VAE 下是相同的，我就不重复描述了，为了达到更好的 VAE 结果，文章还加了一个像素上的重构误差，所以最终 VAE 下的损失函数为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCKcFY59qnfwV0licTVsCkn68avHftqoOibsytSeeKVRfUbOfS1wnibcR9g/640)

这里的 C 就是像素上的重构误差，λ1,λ2 为控制的参数，这个损失将实现 VAE 的整体优化。

对于映射上的 GAN 的损失函数设计，文章主要采用匹配的方法博弈，文章匹配有三组。一组是真实匹配 z,z，对应的损失记为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCZy0BZL0xLaK9oeWzE9Ojj8RJvyhSrruE8uFlYBv9Ob3ias5my5ibaNXg/640)；一组是生成上的匹配 z,z′，其中 z′ 为生成的隐藏变量，对应的损失记为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCbiaPPmSbTECicj1moGlTBoxG6c9vDhA9J728HgHU48ZgF6iaoJgibM3XRA/640)；最后一组是噪声对应的匹配 z,ϵ，对应的损失记为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCUwGanYSwRUB2o6qOXsibInXedTEXZpDTBBtghpC8vibqfJ6hL3JSRFiaw/640)。则最终的判别器的损失：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCNIJNEZ4cu93Z601GIhKAzDM5nlNzLypFthnIsTicuqJGy5b11NDIOaA/640)

在生成器中文章引入了正则项![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCqVTEA4ibQWjjgL8DickVcZW5Hf10f9J9EBgvQCN51YXXHncpGTNhr7eg/640)，当生成器将简单噪声移动到映射分布时，通常会最大化所生成的输出与原始噪声 ϵ 之间的距离。随着这个术语的增加，最小化损失函数以防 G 和 ϵ 太远。综合上述，对于 G 的损失函数可以写为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpC0fdk8wRfibb4IGMA7F402Zy1xLy1VGv1wVYEXx0LNc03p97Y5pUOicvg/640)

经过对上述损失函数优化，系统将达到平衡，此时就可以实现跨模态的相互生成。输入模态 1 下的![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCpRkytBhEibCvEtD4KKp1Nrzcd0vT7k0WshjjiaC7ZY9x2D1RFEd7tzZA/640)经过编码得到![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpClYqeAibM7DNav0ClbPdgXZMhk0DXkJ4DHakZD95jhOl5XhBhBocYE9Q/640)，经过 GAN 的映射得到![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCeD1niayylwX3CQ7OybNDib9dBYFDKOBPq3Lxb54lD6FfDea8KjGoFtPg/640)送入到模态 2 的解码器下得到输出![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCJ6cVPD2jR1ibiaX1iaFa5eTVJxfGq13Wdx2JBxIQjQytLFHiaqUmUBlaAg/640)，从而实现了模态间的转换。

# 实验

**文中主要是在 MNIST 数据集和 SVHN 数据集下进行的实验**，MNIST→MNIST 的实现下的模态转换主要是实现 0→5,1→6,2→7,3→8,4→9，其中 0,1,2,3,4 为模态 1 下的数据，5,6,7,8,9 为模态 2 下的数据，得到的生成的实验结果如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCzsWpMlUtlE7CzgeNGdxTaUeibL3hq2oKslmhycvujzgHhgTZe62n2fQ/640)

对于 SVHN 到 MNIST 的转换，由于 SVHN 下没有 0 这个数字，所以转换为 1→6,2→7,3→8,4→9，结果如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCh8J0PlIwGsCRQ3zkAbVm1gGKUfp3Kf5PZdz4ibVuwQ1a99OAfQQIpPg/640)

在定量分析上，文中为生成的数据设计了分类器用来检测生成数据的分类效果，侧面反应生成的质量，同时也做了人体辨别实验：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCvJFib0FQAVXv9uUS5bbYxbA8hwpPzI15sxvP8fZWTxxv8piau1LYGP6w/640)

文中还补充了 MNIST 到 Fashion-MNIST 转换的实验，实验生成上得到了不错的效果：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnf0hRxEnhz41HGSxAIBQpCF9DxibqrX4w0129HnhCGxUNQtUy9XzoSI0jAMFaAFm4QdqACMp0UX5w/640)

由于只是预印版，所以一些详细的对比实验没有加进去，不过这种方法的启发意义还是蛮大的。

# 总结

**文章利用 GAN 的思想在 VAE 的基础上，实现了模态间的潜在空间的相互映射，得到映射的空间可以进一步的解码生成对应于另一个模态的数据，从而实现了跨模态的相互生成**。这种利用潜在空间的变换实现跨模态生成在很多跨模态之间都可以参考，虽然文章只是预印版但是很具有启发意义。

# 参考文献

[1]. Jesse Engel, Matthew Hoffman, and Adam Roberts. Latent constraints: Learning to generate conditionally from unconditional generative models. arXiv preprint arXiv:1711.05772, 2017.
[2]. Yaniv Taigman, Adam Polyak, and Lior Wolf. Unsupervised cross-domain image generation. arXiv preprint arXiv:1611.02200, 2016.
**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

**点击标题查看更多论文解读：**

[网络表示学习综述：一文理解Network Embedding](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490862&idx=1&sn=707fe122dfc5d961a22771111514fada&chksm=96e9c2aea19e4bb8755d6759dd8e70cb44d2da2c454947395dede0268f511c140441cd1fb5ce&scene=21#wechat_redirect)
[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)
[如何让GAN生成更高质量图像？斯坦福给你答案](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490794&idx=1&sn=bf3af5e3f53f2fa521da137b86afbb47&chksm=96e9c36aa19e4a7ca3bdacdac311dfab3d6ba41eef5993e96de1e9a2ab29ee0571fa2fbde166&scene=21#wechat_redirect)

[哈佛NLP组论文解读：基于隐变量的注意力模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490536&idx=1&sn=0998c5dd4e20841f3542ee328de1f1b4&chksm=96e9c468a19e4d7e24a38f3d9c4b1b4ea48d729c7db124f2b92a2309f0eee982bb9f9bc8356e&scene=21#wechat_redirect)
[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)
[COLING 2018最佳论文：序列标注经典模型复现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490099&idx=1&sn=2d2497999186b979dd557fe3133b7606&chksm=96e9c5b3a19e4ca550a7ae55705af84e941b1aba14cb21f3f2ffc366df837d387575f8529cf2&scene=21#wechat_redirect)
[一文解析OpenAI最新流生成模型「Glow」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490358&idx=1&sn=b4b5d6014bdd365456d500537ba5bcad&chksm=96e9c4b6a19e4da08710a55935dc2e15b00838d5395fdf2a424c50dedff9af7fa11441741b9d&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。



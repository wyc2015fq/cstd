# BiGAN-QP：简单清晰的编码 & 生成模型 - Paper weekly - CSDN博客





2018年12月11日 12:28:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：187









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨苏剑林

单位丨广州火焰信息科技有限公司

研究方向丨NLP，神经网络

个人主页丨kexue.fm




前不久笔者通过直接在对偶空间中分析的思路，提出了一个称为 [***GAN-QP***](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493001&idx=1&sn=5d008e302b5cd312044b67867a70ff01&chksm=96ea3a09a19db31f6a112e6dced61d26541a995bdd083f9ec2b8afb17e5e594272ad447b89ea&scene=21#wechat_redirect) 的对抗模型框架，**它的特点是可以从理论上证明既不会梯度消失，又不需要 L 约束，使得生成模型的搭建和训练都得到简化。**




GAN-QP 是一个对抗框架，所以理论上原来所有的 GAN 任务都可以往上面试试。前面[不用L约束又不会梯度消失的GAN，了解一下？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493001&idx=1&sn=5d008e302b5cd312044b67867a70ff01&chksm=96ea3a09a19db31f6a112e6dced61d26541a995bdd083f9ec2b8afb17e5e594272ad447b89ea&scene=21#wechat_redirect)一文中我们只尝试了标准的随机生成任务，而这篇文章中我们尝试**既有生成器、又有编码器**的情况：***BiGAN-QP***。




# BiGAN与BiGAN-QP




注意这是 BiGAN，不是前段时间很火的 BigGAN，BiGAN 是双向 GAN（Bidirectional GAN），提出于 ***Adversarial Feature Learning***[1] 一文，同期还有一篇非常相似的文章叫做 ***Adversarially Learned Inference***[2]，提出了叫做 ALI 的模型，跟 BiGAN 差不多。




总的来说，它们都是往普通的 GAN 模型中加入了编码器，使得模型既能够具有普通 GAN 的随机生成功能，又具有编码器的功能，可以用来提取有效的特征。把 GAN-QP 这种对抗模式用到 BiGAN 中，就得到了 BiGAN-QP。 




话不多说，先来上效果图（左边是原图，右边是重构）：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkF2qIiaYicGbQRJJjz6Rf2835zCkyvDHMqzatX6XpOlQumQu1icawIalwSA4eBhLHcIEGoGj1tzZYibQ/640)

▲ BiGAN-QP重构效果图




这是将 256 x 256 x 3 的图片降维到 256 维度，然后再重构出来的。可以看到，整体的重构效果是不错的，没有普通自编码器的模糊感。有一些细节缺失，相比*** IntroVAE***[3] 是差了一点，不过这是模型架构和调参的问题了，并不是我擅长的。不管怎样，这个效果图应该可以表明 BiGAN-QP 是可以跑通的，而且效果还行。




本文内容已经更新到 GAN-QP 的原论文，读者可以长按识别下方二维码下载最新版本。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgllBpB6N8rD2XXBHiaX6mQdCu4mCaoovakT9icgf5BtQIqYsqYjKydic3RB89CPoLTQt8iaaxbA4w0ARA/640?)




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgllBpB6N8rD2XXBHiaX6mQdCAic1VcNL5y5rlvlsjfolcbzhRRzf84rv4cwetwB0PX0AicjG52IibOEhQ/640?)




# BiGAN-QP简明推导




其实相比 GAN，BiGAN 的推导非常简单，只需要将原来的单输入 x 换成双输入 (x,z) 就行了。同样，有了 GAN-QP 基础的话，所谓 BiGAN-QP，也是非常简单的。具体来说，原来 GAN-QP 是这样的：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2qIiaYicGbQRJJjz6Rf283hXBTibwCibUoBIpVIoH1mJ3gicwINgAfXlPTqu09TbkGQxSjxSWqEnnpw/640)




现在变成了：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2qIiaYicGbQRJJjz6Rf283xpTKgtwpepicicq6bUHU0etiavtAtTO5ibtDu8iciaOZD1oyUM1CssibW6VZw/640)




理论上就这样行了，这就是 BiGAN-QP。但实际上这样很难学习到一个好的双向映射，因为这相当于从无数可能的映射中自动搜索出一个双向映射，比较困难。所以我们还需要一些“引导项”，我们用两个 mse 误差作为引导项：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2qIiaYicGbQRJJjz6Rf283bDBMEL8MSdKg5n7Ph2UaGy1LA0FVibA4icEyMRCyUtJFqCEQKf6TsYKA/640)




其实生成器的三项 loss 都很直观，ΔT 是生成的图像更加真实，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2qIiaYicGbQRJJjz6Rf283dfOzO4asGsUnhGjBcT9yP6XmhXXCy4ic8JJAfsODicvqk3egbE5e3CRg/640)是希望能重构隐变量空间，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2qIiaYicGbQRJJjz6Rf283WJVCAJpuGq4t1pElicic8dKDqSqeGn57Iw37nicHhQeOeibaJufr46JrpA/640)是希望能重构显变量空间。后两项不能太大，尤其是最后一项，太大会导致图像的模糊。




其中这两个正则项可以看成是 G(z) 与 z 的互信息、x 与 E(x) 的互信息的一个上界，因此从信息的角度看，这两个正则项是希望 x,z 之间的互信息越大越好。相关的讨论可以参考 ***InfoGAN***[4] 论文，这两个正则项代表着它也属于 InfoGAN 的特例。所以完整来说，这应该是一个 Bi-Info-GAN-QP。 




互信息项可以在一定程度上稳定 GAN 的训练过程，减少模型坍缩（mode collapse）的可能性，因为一旦模型坍缩，那么互信息就不会大了。换句话说，如果模型坍缩，那么重构就不大可能了，重构 loss 会很大。




实验表明，再做一些小的调整，效果会更好。这个小的调整源于：两个 mse 项耦合起来还是过于强大了（loss 的具体值不一定大，但是梯度很大），导致模型还是有生成模糊图像的倾向，所以需要停止掉一半的梯度，变为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2qIiaYicGbQRJJjz6Rf283LxIhMdF2tibibtnZ1vdZA2icRicV1DCj8pKWjIbPkFIt2ib82Okh4p3Oic6Q/640)




Gng 和 Eng 指的是强行让这部分的梯度为 0，一般的框架都有这个算子，直接调用即可。这就是本文最终的 BiGAN-QP 模型。




# 代码与效果图




代码也已经补充到 Github 了：




https://github.com/bojone/gan-qp/tree/master/bigan-qp




再来一些效果图（左边是原图，右边是重构）：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkF2qIiaYicGbQRJJjz6Rf283akATL8ru2d6ZibqV3R8cfcKKgCeDA1qBibJKiaMneuPQgic2L6AzO6OE9g/640)

▲ BiGAN-QP重构效果图2




可以看到，重构并不模糊，而且很重要的一个特点是：**因为是降维重构，模型并不是（也无法做到）学会了一个逐像素对应的一一映射，而是一个整体看上去差不多的清晰的重构结果。**




比如我们看到第一行的第一张和最后一行的第二张，模型基本上把人重构出来了，但有趣的是眼镜，我们发现模型确实也重构了眼镜但是换了另外一个“款式”的眼镜。我们甚至可以认为，模型已经学到了“眼镜”这个概念，只不过是降维重构，隐变量的表达能力有限，所以尽管模型知道那是眼镜，但不能重构出一模一样的眼镜出来，就只好换一款常见的眼睛了。




这是普通的 VAE 所要求的“逐点一一对应重构”所无法实现的，“逐点一一对应重构”也是造成 VAE 模糊的主要原因了。如果要完全可逆重构，只有像 [***Glow***](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490358&idx=1&sn=b4b5d6014bdd365456d500537ba5bcad&chksm=96e9c4b6a19e4da08710a55935dc2e15b00838d5395fdf2a424c50dedff9af7fa11441741b9d&scene=21#wechat_redirect) 那样的可逆模型才有可能做到了。




 另外，又有编码器又有生成器，我们就可以玩玩真实图片的隐变量插值了（左一、右一是真实图片，左二、右二是重构图片，其余是插值图）：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkF2qIiaYicGbQRJJjz6Rf28321MLffFNcrlYZxVyGeNK1ibeyB5MRCz09qdYyTq5RibQoh22knUAnH2g/640)

▲ BiGAN-QP真实图片插值




还可以看看 BiGAN-QP 眼中的相似图片（算出所有真实图片的隐变量，然后用欧氏距离或者 cos 值算相似度，下图为欧氏距离的结果），第一行为输入，后两行为相似图片：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkF2qIiaYicGbQRJJjz6Rf283lHHRPoofr5LVn3uluiaDZI8XaYvWmbwarPj34WONDrEnIPyxmGXycmw/640)

▲ BiGAN-QP眼中的相似




# 欢迎使用与分享




前面已经提到，**GAN-QP 是一个理论完备的对抗框架，理论上所有的 GAN 任务都可以尝试一下。**所以，如果读着您手头上正好有 GAN 任务，不妨尝试一下，然后你就可以去掉 L 约束，去掉谱归一化甚至去掉很多正则项，还不用担心梯度消失了。GAN-QP 就是笔者致力于去掉 GAN 各种超参数所得的结果。




如果你有新的基于 GAN-QP 的应用结果，欢迎在此分享。




# 参考文献




 [1] Donahue, Jeff, Krähenbühl, Philipp, and Darrell, Trevor. Adversarial feature learning. In ICLR, 2017.

[2] Dumoulin, Vincent, Belghazi, Ishmael, Poole, Ben, Mastropietro, Olivier, Lamb, Alex, Arjovsky, Martin, and Courville, Aaron. Adversarially learned inference. In ICLR, 2017.

[3] H Huang, R He, Z Sun, T Tan. IntroVAE: Introspective Variational Autoencoders for Photographic Image Synthesis. Advances in Neural Information Processing Systems, 52-63.

[4] X Chen, Y Duan, R Houthooft, J Schulman, I Sutskever, P Abbeel. InfoGAN: Interpretable Representation Learning by Information Maximizing Generative Adversarial Nets. In NIPS 2016.




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看作者其他文章：**




- 
[变分自编码器VAE：原来是这么一回事 | 附开源代码](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488480&idx=1&sn=7bb9a4dd60680df5013670287a9e0cc2&chksm=96e9cc60a19e457618b2ffdea2a7e13ba172ea3fdfbfda07de53aae57126047f1b71a6969d76&scene=21#wechat_redirect)[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)

- 
[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)

- 
[从变分编码、信息瓶颈到正态分布：论遗忘的重要性](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493326&idx=1&sn=7ba19fe14ee11bff0e1c865adcb52ca1&chksm=96ea394ea19db0587dc096898730f7522a8e3a7bb3b55bac576422eea63a987ea97ad5886bca&scene=21#wechat_redirect)


- 
[深度学习中的互信息：无监督提取特征](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492040&idx=1&sn=f90a6b899e62748c4db489ce06276869&chksm=96ea3e48a19db75e8c07d942a4772bb6c784fac7bcb117da2023186546cfe1876b121a8121cc&scene=21#wechat_redirect)

- 
[全新视角：用变分推断统一理解生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)

- 
[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)

- 
[细水长flow之f-VAEs：Glow与VAEs的联姻](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491695&idx=1&sn=21c5ffecfd6ef87cd4f1f754795d2d63&chksm=96ea3fefa19db6f92fe093e914ac517bd118e80e94ae61b581079023c4d29cedaaa559cb376e&scene=21#wechat_redirect)

- 
[深度学习中的Lipschitz约束：泛化与生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492180&idx=1&sn=3ea92a3a9f1306efde89ce1777b80da6&chksm=96ea3dd4a19db4c20dcbc9627b0eb307672b4d61008a93c42814fa6728ca7b6f7c293cff1d80&scene=21#wechat_redirect)












**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**




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

▽ 点击 | 阅读原文| 查看作者博客





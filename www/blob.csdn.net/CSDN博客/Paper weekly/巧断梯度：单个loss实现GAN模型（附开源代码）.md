
# 巧断梯度：单个loss实现GAN模型（附开源代码） - Paper weekly - CSDN博客


2019年03月05日 08:36:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：54


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨苏剑林
单位丨广州火焰信息科技有限公司
研究方向丨NLP，神经网络
个人主页丨kexue.fm

我们知道普通的模型都是搭好架构，然后定义好 loss，直接扔给优化器训练就行了。但是 GAN 不一样，一般来说它涉及有两个不同的 loss，这两个 loss 需要交替优化。

现在主流的方案是判别器和生成器都按照 1:1 的次数交替训练（各训练一次，必要时可以给两者设置不同的学习率，即 TTUR），交替优化就意味我们需要传入两次数据（从内存传到显存）、执行两次前向传播和反向传播。

如果我们能把这两步合并起来，作为一步去优化，那么肯定能节省时间的，**这也就是 GAN 的同步训练。**

注：本文不是介绍新的 GAN，而是介绍 GAN 的新写法，这只是一道编程题，不是一道算法题。

# 如果在TF中

如果是在 TensorFlow 中，实现同步训练并不困难，因为我们定义好了判别器和生成器的训练算子了（假设为D_solver和G_solver），那么直接执行：

```python
sess.run([D_solver, G_solver], feed_dict={x_in: x_train, z_in: z_train})
```

就行了。这建立在我们能分别获取判别器和生成器的参数、能直接操作sess.run的基础上。

# 更通用的方法

但是如果是 Keras 呢？Keras 中已经把流程封装好了，一般来说我们没法去操作得如此精细。

所以，下面我们介绍一个通用的技巧，**只需要定义单一一个 loss，然后扔给优化器，就能够实现 GAN 的训练。**同时，从这个技巧中，我们还可以学习到如何更加灵活地操作 loss 来控制梯度。

**判别器的优化**

我们以 GAN 的 hinge loss 为例子，它的形式是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkyEpfaFZROC9v2ziau0IqJHM6Pr3ZKsoNhz5LeoyNdFribZUpbia2xTbiccN1ibax5xSPv0jBcuMISgUQ/640?wx_fmt=png)

注意![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkyEpfaFZROC9v2ziau0IqJHyhC46cn6zKaw58z3RPkcQ2ZAETYStUlv0p32QqOwnDY8PeCprNLJ2A/640?wx_fmt=png)意味着要固定 G，因为 G 本身也是有优化参数的，不固定的话就应该是![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkyEpfaFZROC9v2ziau0IqJHgmolveGh7Ym6GNpSX2BM7Je17LvhYCEEBgmpSbcFdk7damwvYNFnJQ/640?wx_fmt=png)。

为了固定G，除了“把 G 的参数从优化器中去掉”这个方法之外，我们也可以利用stop_gradient去手动固定：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkyEpfaFZROC9v2ziau0IqJHOsAXaC6TczyyibZibrvCvAnMWvbCurfjPEuELIS2s6icTA3jj83FSlCcw/640?wx_fmt=png)

这里：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkyEpfaFZROC9v2ziau0IqJHCUGdsV9iapIZ8x8roiaOvTmcYBibuoxMKsNcibxzwSx8URpMdw3svIqxrg/640?wx_fmt=png)

这样一来，在式 (2) 中，我们虽然同时放开了 D,G 的权重，但是不断地优化式 (2)，会变的只有 D，而 G 是不会变的，因为我们用的是基于梯度下降的优化器，而 G 的梯度已经被停止了，换句话说，我们可以理解为 G 的梯度被强行设置为 0，所以它的更新量一直都是 0。

**生成器的优化**

现在解决了 D 的优化，那么 G 呢？stop_gradient可以很方便地放我们固定里边部分的梯度（比如 D(G(z)) 的 G(z)），但 G 的优化是要我们去固定外边的 D，没有函数实现它。但不要灰心，我们可以用一个数学技巧进行转化。

首先，我们要清楚，我们想要 D(G(z)) 里边的 G 的梯度，不想要 D 的梯度，如果直接对 D(G(z)) 求梯度，那么同时会得到 D,G 的梯度。如果直接求![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkyEpfaFZROC9v2ziau0IqJHib6iaMlqrvBz1P22O6RV5nPGac6BbibclAe3bPYsBPpzibp2jIXTZaemxw/640?wx_fmt=png)的梯度呢？只能得到 D 的梯度，因为 G 已经被停止了。那么，重点来了，将这两个相减，不就得到单纯的 G 的梯度了吗！

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkyEpfaFZROC9v2ziau0IqJHhvG07wibafUd7KMksOk7bhFn2478KOTPqT1UjWXuvAwOicRWuZp2H2Ug/640?wx_fmt=png)

现在优化式 (4) ，那么 D 是不会变的，改变的是 G。

值得一提的是，直接输出这个式子，结果是恒等于 0，因为两部分都是一样的，直接相减自然是 0，但它的梯度不是 0。也就是说，这是一个恒等于 0 的 loss，但是梯度却不恒等于 0。

**合成单一loss**

好了，现在式 (2) 和式 (4) 都同时放开了 D,G，大家都是 arg min，所以可以将两步合成一个 loss：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkyEpfaFZROC9v2ziau0IqJHblOrP53r9bX7tKn1nsFp1ibmicJoydcdfDicu7a1DYgnApVQmF9FqQCdA/640?wx_fmt=png)

写出这个 loss，就可以同时完成判别器和生成器的优化了，而不需要交替训练，但是效果基本上等效于 1:1 的交替训练。引入 λ 的作用，相当于让判别器和生成器的学习率之比为 1:λ。

**参考代码：**

https://github.com/bojone/gan/blob/master/gan_one_step_with_hinge_loss.py

# 文章小结

文章主要介绍了实现 GAN 的一个小技巧，允许我们只写单个模型、用单个 loss 就实现 GAN 的训练。它本质上就是用stop_gradient来手动控制梯度的技巧，在其他任务上也可能用得到它。

所以，以后我写 GAN 都用这种写法了，省力省时。当然，理论上这种写法需要多耗些显存，这也算是牺牲空间换时间吧。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看作者其他文章：**

[变分自编码器VAE：原来是这么一回事 | 附开源代码](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)
[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)
[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)
从变分编码、信息瓶颈到正态分布：论遗忘的重要性

[深度学习中的互信息：无监督提取特征](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492040&idx=1&sn=f90a6b899e62748c4db489ce06276869&chksm=96ea3e48a19db75e8c07d942a4772bb6c784fac7bcb117da2023186546cfe1876b121a8121cc&scene=21#wechat_redirect)
[全新视角：用变分推断统一理解生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)
[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)
[细水长flow之f-VAEs：Glow与VAEs的联姻](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491695&idx=1&sn=21c5ffecfd6ef87cd4f1f754795d2d63&chksm=96ea3fefa19db6f92fe093e914ac517bd118e80e94ae61b581079023c4d29cedaaa559cb376e&scene=21#wechat_redirect)
[深度学习中的Lipschitz约束：泛化与生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492180&idx=1&sn=3ea92a3a9f1306efde89ce1777b80da6&chksm=96ea3dd4a19db4c20dcbc9627b0eb307672b4d61008a93c42814fa6728ca7b6f7c293cff1d80&scene=21#wechat_redirect)




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
▽ 点击 |阅读原文| 查看作者博客



# T-GANs：基于“图灵测试”的生成对抗模型 - Paper weekly - CSDN博客





2018年11月05日 13:08:55[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：137









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnC9iaic8hDbiadLafh7TtCZS6icEYddVmMqZBksDV7cQkKmAu95h53FxyibqmZOS1yQgHibJT0WYD2s1Zw/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)





在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **114** 篇文章

作者丨武广


学校丨合肥工业大学硕士生

研究方向丨图像生成




本期推荐的论文笔记来自 PaperWeekly 社区用户 **@TwistedW**。**本文将“图灵思想”用在 GAN 的框架下**，这个思想在 RSGAN 中最先提出——将以往判别器逐个判断数据真假换为将真假样本混合后再做判断，同时在生成器下引入真实样本做参考，增加了生成器的生成能力。**本文对该思想做了更一般化的推证和应用，可以说”图灵思想“适用于任何一种 GAN 模型。**




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ41dfyrUyfkibuBUoj4m498CzIqrxumlHYGC0CclpLxuyRsv4JoWAtURA/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4c5QoCP3pwtREhR4djLEia2O5QTuoJjSXzJNPGZCwXdZA51NhdiaH3TLQ/640)







# 引言




利用 GAN 做实验的学者一定为其训练不稳定感到痛苦，如何去控制超参数，如何去平衡判别器和生成器间的迭代都是操碎了心。尽管 ***WGAN***[1]、***LSGAN***[2] 对于 GAN 的梯度消失做了解释和改进，但是在实际的训练中模型的不稳定一直存在。***SNGAN***[3] 可以有效地约束判别器的能力，但是随着生成精度的增加，仍然不可避免训练的不稳定。




***T-GANs***** 是苏剑林的一篇在 [RSGAN](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492452&idx=1&sn=f8fbfb3cd16f7267b43aaacd98904726&chksm=96ea3ce4a19db5f2bff55adc3df5ffd17f836f44802b51096411d842bf06f8b4bb4bfb569500&scene=21#wechat_redirect) 的基础上对相对判别器做一般化改进的文章**，论文现已挂在 arXiv，代码已开源。文章中出现了较多的数学公式，笔者尽量用简洁的话语阐述清楚文章观点。




**源码链接：**




https://github.com/bojone/T-GANs




# 论文引入




由于 GAN 的文章更新太多，**现将原始 GAN 称为 Standard GAN，简称 SGAN**，文章后续的 SGAN 就是大家最熟悉的由 Goodfellow 创作的最原始的 ***GAN ***[4]。




GAN 在训练和分析过程中都会发现判别器 D 的能力是远超生成器 G 的，这个理论推导在 SNGAN 中有所提及。为了较好地平衡 D 和 G 的关系，在 GAN 刚开始的阶段，大多数学者是控制判别器和生成器的更新比例。由于 WGAN 和 LSGAN 等一系列改进文章的提出，权重裁剪、梯度惩罚得到应用，SNGAN 对判别器做谱归一化来约束判别器的能力。 




上述这些方法仍然无法完全消除 GAN 训练过程中的不稳定，尤其是训练特别高维数据的情况下，例如 1024 x 1024 的图像。这个观念在最近新出的 [***BigGAN***](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect) 中作者也是吐槽了一下。




**RSGAN 将“图灵测试”的思想引入到了 GAN 中**，何谓图灵测试，借用苏大哥文章中的描述做解释： 




图灵测试指的是测试者在无法预知的情况下同时跟机器人和人进行交流，如果测试者无法成功分别出人和机器人，那么说明这个机器人已经（在某个方面）具有人的智能了。“图灵测试”也强调了对比的重要性，如果机器人和人混合起来后就无法分辨了，那么说明机器人已经成功了。 




RSGAN 把图灵测试在 GAN 中的应用可以这么理解，对于真实样本我们将其理解为人，假样本则理解为机器人，不再按照 SGAN 中一张张送到判别器下判断样本真假，而是将真假样本混合在一起让判别器判断何为真何为假。




这样判别器将不再按照记忆去判断真假，而是在混合的数据堆中找到真假，这个无疑是增加了判别器的判别要求，在训练阶段有用的和丰富的梯度将得到保留；同时让生成器也看到真实样本，在生成阶段更加逼真的生成高质量样本。这种一收一放，有效地控制了 D 和 G 的能力，进而在整体上提高了 GAN 的性能。




# GAN的回顾



为了保证与原文的公式一致，接下来部分的公式保持与原文一致。**大部分 GAN 的目的都是为了减小真实样本和生成样本的分布差异。**我们定义真实样本的分布为 p̃(x)，生成样本分布为 q(x)，对于 SGAN 对抗下最大最小化博弈，对于判别器 T(x) 和生成器 G(z)：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4Gn1g6laVicQP9eK1rxCC4DjtsBtNE9pS3jH7kr8DSJlK3icewjQ50fug/640)




这里的 σ 为 sigmoid 函数![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4TdjKqQXHTe4rSbFaqW9y0ibsu5EsVNUGibh8pc2KTKSjv7kSyibcX7hiaQ/640)，其中 h 可以是任何标量函数，只要使 h(log(t)) 成为变量 t 的凸函数即可，这个后续再说。对上述的判别器优化公式 (1) 进行变分操作，类似于微分，可以得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4zy3IibW24b4tSEqh23J8AReJZpF7HT5Fuicl6pVDot78f5GjCFWqPMiag/640)




详细证明可参看[RSGAN：对抗模型中的“图灵测试”思想](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492452&idx=1&sn=f8fbfb3cd16f7267b43aaacd98904726&chksm=96ea3ce4a19db5f2bff55adc3df5ffd17f836f44802b51096411d842bf06f8b4bb4bfb569500&scene=21#wechat_redirect)。带入到公式 (2)，可以得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ43hicC9mJiaHfCevVAkzRKZMkmAEu4tticujd5fbpEvq7uopB5DTicuqxGw/640)




设 f(t)=h(log(t))，可以看出 SGAN 的基本目标是最小化 p(x) 和 q(x) 之间的 f- 散度，函数 f 受凸函数约束。因此，任何使 h(log(t)) 成为凸函数的函数都可以使用，例如 h(t)=−t，h(t)=−logσ(t)，h(t)=log(1−σ(t))。 




对于 WGAN 中的 Wasserstein distance，其目的也是为了最小化 p(x) 和 q(x) 之间的距离，详细解释可参看原文，这里不再赘述。**不过统一起来，这些 GAN 都是为了拉近生成分布与真实分布；在更新生成器时，只有来自生成分布的假样本可用。**这种一张张输入判断真假的任务对于拥有强大记忆性的判别器来说是容易的，这无疑会导致越训练判别器的能力越强，最终导致训练的失败。




# RSGAN




**RSGAN 在 SGAN 的基础上对判别器做了一定变化**，此时判别器不再是一张张判断输入的真假，而是将真假混合在一起来判断，同时生成器的更新也加入了真实样本作为指导，这就是 RSGAN 的整体思路，所对应的优化公式为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4iboRe4yHraZDicUm0ibiatvdiblTkZWribbS8Lv7Mw84AupBRIYMYMbRJYrA/640)




继续通过变分的思想，可以对公式 (5) 进一步分析，得到的最优解：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4x1ukLGSJhYUVppXibzGiceCRZvjMxbJcBRIapGaklHO3g6fPsp3EQmRQ/640)




带入到公式 (6) 可以得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4XgaAnowA3pdIEL07QboPQA7P4S2BXKvibJTnWMhiagsZbKWJ1MQOugpw/640)




这就是 RSGAN 的目的所在，它实际上优化的是 p̃(Xr)q(Xf) 和 p̃(Xf)q(Xr) 的 f- 散度。我们可以进一步分析（以下观点借鉴自[苏剑林文章](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492452&idx=1&sn=f8fbfb3cd16f7267b43aaacd98904726&chksm=96ea3ce4a19db5f2bff55adc3df5ffd17f836f44802b51096411d842bf06f8b4bb4bfb569500&scene=21#wechat_redirect)）： 




假如我从真实样本采样一个 Xr 出来，从伪造样本采样一个 Xf 出来，然后将它们交换一下，把假的当成真，真的当成假，那么还能分辨出来吗？换言之：p̃(Xf)q(Xr) 有大变化吗？ 




假如没有什么变化，那就说明真假样本已经无法分辨了，训练成功。假如还能分辨出来，说明还需要借助真实样本来改善伪造样本。所以，式 (8) 就是 RSGAN 中“图灵测试”思想的体现：**打乱了数据，是否还能分辨出来？**




# T-GANs




前面说到 RSGAN 的突破性工作就是优化了 p̃(Xr)q(Xf) 和 p̃(Xf)q(Xr) 的 f- 散度，利用“图灵测试”的思想来优化 GAN。**为了让这一思想更加一般化，更可以称得上是“图灵测试”在 GAN 中的应用，T-GANs 显示出了更加一般化的思想。 **




假设联合分布 P(Xr,Xf)=q̃(Xr)p(Xf)，Q(Xr,Xf)=q̃(Xf)p(Xr)。现在的目的是想最小化 P(Xr,Xf) 和 Q(Xr,Xf) 的距离，如果将 (Xr,Xf) 视为一个变量，并带入 SGAN 即公式 (1) 中，可以得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4h50SBCQCDaYpQKplJMiaWJ4VqgD6kfWwrMtDuF95oYSrRxkq9LCuNvQ/640)




代入公式 (2) 可以得到 G 的优化公式，这里的 Xf 可以表示为 Xf=G(z),z∼q(z)，最终得到一般化优化公式：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4ojJmjianI8OXWic2pjxwgTjwzTgMv9sqTtTmMKIxcNwNkHntYUDyLk8g/640)




这就是利用 SGAN 实现了 p̃(Xr)q(Xf) 和 p̃(Xf)q(Xr) 分布距离的拉近，并且也可以进一步将“图灵测试”思想用在 WGAN 上。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4NlGqzTuJwK3ccAADdKCJfq10ic6BEX4oCTTCliaeIVl9j6InmSYaG2yg/640)




最终应用在优化公式上：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4ySXibGWTt6e7uhpxJg32QnhZF2mORibribwZXOZPia7hUh8CYicRxWQE1TQ/640)




怎么去实现 T(Xr,Xf) 呢？




可以构造一个编码器 E，将真实样本和生成样本先编码到潜在空间，再通过判别器 D 判断 E(Xr)−E(Xf) 的真假，可以表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ4foZEMtoznVq5oqg8kEavpJyuAywTdeqDWAzhYzicTCIhvj7WP8OmJgQ/640)




**当然这种“图灵思想”可以用在更多的 GAN 模型上，甚至可以说对于任何一种 GAN 都是适用的**，所以文章将“图灵思想（Turing Test）”用作题目，简称为 T-GANs。




# 实验




**在不同的图像尺寸上，T-GANs 展示了更多的网络适应能力和更快的收敛速度。**在私下和作者交流的过程中，64 尺寸到 128，甚至是 256 整体框架的改动不大， 这对于一般 GAN 模型是困难的，看一下实验结果对比：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ41Ch93sYylUWSaLBh0c5gibib0icII3c9RblqW39JRCCw0TohFyciaicTclg/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmaJiby8Ea7UJljCou6EzaJ49Gn9hYW9tZ1LjvTtp6RGmrAOhqWSRIWNsibBAPps4btlUL5wj55Wiavw/640)




由于文章还处于初稿阶段，本博文只是为了对文章做中文解释，后续的实验还会再丰富。




# 总结




**在本文中，作者提出了一种新的对抗模式，用于训练称为 T-GAN 的生成模型。**这种对抗模式可以解释为 GAN 中的图灵测试，它是训练 GAN 而不是特定 GAN 模型的指导思想。 它可以与当前流行的 GAN（如 SGAN 和 WGAN）集成，从而实现 T-SGAN 和 T-WGAN。




**实验表明，T-GAN 在从小规模到大规模的数据集上具有良好且稳定的性能。 **它表明在 GAN 中更新发生器时，实际样本的信号非常重要。然而，T-GAN 提高稳定性和收敛速度的机制仍有待进一步探索。




# 参考文献




[1] Martin Arjovsky, Soumith Chintala, and Léon Bottou. Wasserstein generative adversarial networks. In International Conference on Machine Learning, pages 214–223, 2017.

[2] Xudong Mao, Qing Li, Haoran Xie, Raymond YK Lau, Zhen Wang, and Stephen Paul Smolley. Least squares generative adversarial networks. In 2017 IEEE International Conference on Computer Vision (ICCV), pages 2813–2821. IEEE, 2017.

[3] Takeru Miyato, Toshiki Kataoka, Masanori Koyama, and Yuichi Yoshida. Spectral normalization for generative adversarial networks. arXiv preprint arXiv:1802.05957, 2018.

[4] Ian Goodfellow, Jean Pouget-Abadie, Mehdi Mirza, Bing Xu, David Warde-Farley, Sherjil Ozair, Aaron Courville, and Yoshua Bengio. Generative Adversarial Networks. Advances in Neural Information Processing Systems 27, pages 2672–2680. Curran Associates, Inc., 2014.

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)




**点击标题查看更多论文解读：**




- 
[在全景视频中预测头部运动：一种深度强化学习方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492263&idx=1&sn=5c904bcb3205815849d9382fbe0cd41e&chksm=96ea3d27a19db43116635fef31260582cb8c7ea5cec3cc2e5191718603cc5ea89d364c77f949&scene=21#wechat_redirect)

- 
[网络表示学习综述：一文理解Network Embedding](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490862&idx=1&sn=707fe122dfc5d961a22771111514fada&chksm=96e9c2aea19e4bb8755d6759dd8e70cb44d2da2c454947395dede0268f511c140441cd1fb5ce&scene=21#wechat_redirect)

- 
[神经网络架构搜索（NAS）综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491248&idx=1&sn=d2946d8a37f7c6567b1a767a497006fb&chksm=96e9c130a19e48267f72ad32c527ec4a1697741e409d865d9233c5d7035a1f66a59b5e40792d&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)


- 
[深度](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)[解读DeepMind新作：史上最强GAN图像生成器](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)

- 
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)

- 
[EMNLP 2018论文解读 | 对话生成 & 文本风格转化](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492361&idx=1&sn=af65ef32752cefec12d99727d7fd0867&chksm=96ea3c89a19db59ffead464756c146e53b6ae80519c46c0ed9b3e110e3b0375d7018d6291243&scene=21#wechat_redirect)











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




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)

▽ 点击 | 阅读原文| 下载论文 & 源码





# 全新视角：用变分推断统一理解生成模型（VAE、GAN、AAE、ALI） - Paper weekly - CSDN博客





2018年07月19日 13:44:57[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：921









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨苏剑林

单位丨广州火焰信息科技有限公司

研究方向丨NLP，神经网络

个人主页丨kexue.fm




**摘要：**本文从一种新的视角阐述了变分推断，并证明了 EM 算法、VAE、GAN、AAE、ALI (BiGAN) 都可以作为变分推断的某个特例。其中，论文也表明了标准的 GAN 的优化目标是不完备的，这可以解释为什么 GAN 的训练需要谨慎地选择各个超参数。最后，文中给出了一个可以改善这种不完备性的正则项，实验表明该正则项能增强 GAN 训练的稳定性。




# 前言




 我小学开始就喜欢纯数学，后来也喜欢上物理，还学习过一段时间的理论物理，直到本科毕业时，我才慢慢进入机器学习领域。所以，哪怕在机器学习领域中，我的研究习惯还保留着数学和物理的风格：企图从最少的原理出发，理解、推导尽可能多的东西。这篇文章是我这个理念的结果之一，试图以变分推断作为出发点，来统一地理解深度学习中的各种模型，尤其是各种让人眼花缭乱的 GAN。




本文已经挂到 arXiv 上，需要读英文原稿的可以访问下方链接下载论文 ***Variational Inference: A Unified Framework of Generative Models and Some Revelations***。 




■ 论文 | Variational Inference: A Unified Framework of Generative Models and Some Revelations

■ 链接 | https://www.paperweekly.site/papers/2117

■ 作者 | Jianlin Su




下面是文章的介绍。其实，中文版的信息可能还比英文版要稍微丰富一些，原谅我这蹩脚的英语。




近年来，深度生成模型，尤其是 GAN，取得了巨大的成功。现在我们已经可以找到数十个乃至上百个 GAN 的变种。然而，其中的大部分都是凭着经验改进的，鲜有比较完备的理论指导。




**本文的目标是通过变分推断来给这些生成模型建立一个统一的框架**。首先，**本文先介绍了变分推断的一个新形式**，这个新形式其实在本人以前的文章中就已经介绍过，**它可以让我们在几行字之内导出变分自编码器（VAE）和 EM 算法**。然后，**利用这个新形式，我们能直接导出 GAN，并且发现标准 GAN 的 loss 实则是不完备的，缺少了一个正则项**。如果没有这个正则项，我们就需要谨慎地调整超参数，才能使得模型收敛。




实际上，本文这个工作的初衷，就是要将 GAN 纳入到变分推断的框架下。目前看来，最初的意图已经达到了，结果让人欣慰。新导出的正则项实际上是一个副产品，并且幸运的是，在我们的实验中这个副产品生效了。




# 变分推断新解




假设 x 为显变量，z 为隐变量，p̃(x) 为 x 的证据分布，并且有：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRxLU3WWctHZQD7xM85CgnePNt0L6cjGd3ehsrWWAfBESVVom5R0icicsQ/640)




我们希望 qθ(x) 能逼近 p̃(x)，所以一般情况下我们会去最大化似然函数：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRlHPLRmUPbAWIOXkH5tKvzLqTUcfcOnCPiaCRziauxaiaHFJU6leSEiaMdA/640)




这也等价于最小化 KL 散度 KL(p̃(x))‖q(x))：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRJ66WwnIpia5Xia1ohzBLmy6MshNfMzDxMdVOSaib1PWxDaIvv2h1uy3ibA/640)




但是由于积分可能难以计算，因此大多数情况下都难以直接优化。 




变分推断中，首先引入联合分布 p(x,z) 使得p̃(x)=∫p(x,z)dz，而变分推断的本质，就是将边际分布的 KL 散度 KL(p̃(x)‖q(x)) 改为联合分布的 KL 散度 KL(p(x,z)‖q(x,z)) 或 KL(q(x,z)‖p(x,z))，而：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRXNX6iaqtu0SUPxys3wYPOX6ny6NZaV7aiarRFule6Kza2eYjOU9mGdqQ/640)




意味着联合分布的 KL 散度是一个更强的条件（上界）。所以一旦优化成功，那么我们就得到 q(x,z)→p(x,z)，从而 ∫q(x,z)dz→∫p(x,z)dz=p̃ (x)，即 ∫q(x,z)dz 成为了真实分布 p̃(x) 的一个近似。




当然，我们本身不是为了加强条件而加强，而是因为在很多情况下，KL(p(x,z)‖q(x,z)) 或 KL(q(x,z)‖p(x,z)) 往往比 KL(p̃(x)‖q(x)) 更加容易计算。所以变分推断是提供了一个可计算的方案。




# VAE和EM算法




由上述关于变分推断的新理解，我们可以在几句话内导出两个基本结果：变分自编码器和 EM 算法。这部分内容，实际上在[从最大似然到EM算法：一致的理解方式](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487878&idx=1&sn=91eace84220bb48f876c50ef7f4b8c5f&chksm=96e9ce06a19e471049fcc02e019569ac648b5e8ed6153bf6a162a1da5b410466c94e5cf43c45&scene=21#wechat_redirect)和[变分自编码器（二）：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)已经详细介绍过了。这里用简单几句话重提一下。




**VAE**




在 VAE 中，我们设 q(x,z)=q(x|z)q(z),p(x,z)=p̃(x)p(z|x)，其中 q(x|z),p(z|x) 带有未知参数的高斯分布而 q(z) 是标准高斯分布。最小化的目标是：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRnm2CXPTq4MowrqIdItCLOA4CWkj5TiahvQk2usSsQFSz0bq35IaHcjg/640)




其中 log*p̃*(*x*) 没有包含优化目标，可以视为常数，而对 *p̃*(*x*) 的积分则转化为对样本的采样，从而：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRKSwr2af33LIGlygUxCJsR0RGggJCw6xG7PlkicZx3zoAnIEoRGtdfug/640)




因为 q(x|z),p(z|x) 为带有神经网络的高斯分布，这时候 KL(p(z|x)‖q(z)) 可以显式地算出，而通过重参数技巧来采样一个点完成积分 ∫p(z|x)logq(x|z)dz 的估算，可以得到 VAE 最终要最小化的 loss：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRROlx92xjTHtwVOCsAR3KiaXj5ic2dL8gqzTPmBDZzia4XQ4fDQQIQibXbzA/640)




**EM算法**




在 VAE 中我们对后验分布做了约束，仅假设它是高斯分布，所以我们优化的是高斯分布的参数。如果不作此假设，那么直接优化原始目标 (5)，在某些情况下也是可操作的，但这时候只能采用交替优化的方式：先固定 *p*(*z*|*x*)，优化 *q*(*x*|*z*)，那么就有：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRteO0r7fEhxf38CoGPt1ACichnssHqZq4b3P9vFlH1lY0iaxHbIlsXXUA/640)




完成这一步后，我们固定 q(x,z)，优化 p(z|x)，先将 q(x|z)q(z) 写成 q(z|x)q(x) 的形式：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRHBV8iaBmQCibJW7zCE7gLH3q907YcYZ33Ria35NHAPYZLZlDCYlbCJrGQ/640)




那么有：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRoQA4f5AibhlqvicF5BD3W8yweRgh0M0ibjyOia3ecwI8Zjryd4rzxwDczw/640)




由于现在对 p(z|x) 没有约束，因此可以直接让 p(z|x)=q(z|x) 使得 loss 等于 0。也就是说，p(z|x) 有理论最优解：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRUgFzZUro3JRiaxdtmxFuWppmk8C2LxwhmMsRicMfjyf800NAFAL2RkLA/640)




(8)，(11) 的交替执行，构成了 EM 算法的求解步骤。这样，我们从变分推断框架中快速得到了 EM 算法。




# 变分推断下的GAN




在这部分内容中，我们介绍了一般化的将 GAN 纳入到变分推断中的方法，这将引导我们得到 GAN 的新理解，以及一个有效的正则项。 




**一般框架**




同 VAE 一样，GAN 也希望能训练一个生成模型 *q*(*x*|*z*)，来将 *q*(*z*)=*N*(*z*;*0*,*I*) 映射为数据集分布 *p̃*(*x*)，不同于 VAE 中将 *q*(*x*|*z*) 选择为高斯分布，GAN 的选择是：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRqBMIOHZZnF6QmajRj6pcl86Jia9jSLJuqHl7Ekicnj9bljgZEucmXrmQ/640)




其中 *δ*(*x*) 是狄拉克 *δ* 函数，*G*(*z*) 即为生成器的神经网络。




一般我们会认为 *z* 是一个隐变量，但由于 *δ* 函数实际上意味着单点分布，因此可以认为 *x* 与 *z* 的关系已经是一一对应的，所以 *z *与 *x* 的关系已经“不够随机”，在 GAN 中我们认为它不是隐变量（意味着我们不需要考虑后验分布 *p*(*z*|*x*)）。




事实上，在 GAN 中仅仅引入了一个二元的隐变量 *y* 来构成联合分布：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRVFjoHGYPlJHTho4MNRegXLsRBvjiayMCRdhGymKiaoiaic2ajiabKw67zDA/640)




这里 p1=1−p0 描述了一个二元概率分布，我们直接取 p1=p0=1/2。另一方面，我们设 p(x,y)=p(y|x)p̃(x)，p(y|x) 是一个条件伯努利分布。而优化目标是另一方向的 KL(q(x,y)‖p(x,y))：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRkL8PBdwRHua5AqCHtDaGdPwnibTMo15IxYUJfSXl9NCNgGhrl22ichYg/640)




一旦成功优化，那么就有 q(x,y)→p(x,y)，那么：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRRTZrXkpicpUvqlfKI42FPtdxBo3aOKCOb81mW1UMDERQQft7NKByeKw/640)




从而 q(x)→p̃(x)，完成了生成模型的构建。 




现在我们优化对象有 p(y|x) 和 G(x)，记 p(1|x)=D(x)，这就是判别器。类似 EM 算法，我们进行交替优化：先固定 G(z)，这也意味着 q(x) 固定了，然后优化 p(y|x)，这时候略去常量，得到优化目标为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRicSu9MqP73UfnRedicTCIakRJlshZ5vAKDduawOe5HKw5VQBhwficf6qw/640)




然后固定 D(x) 来优化 G(x)，这时候相关的 loss 为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRe6JkTGR5UJeHzVTnLCLBjH8oQu0FdowuQ1Phzwjrx4Xziazgt2Nm1Fw/640)




这里包含了我们不知道的 p̃(x)，但是假如 D(x) 模型具有足够的拟合能力，那么跟 (11) 式同理，D(x) 的最优解应该是：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRTaTZ3bs8mmFq8OOlPXOa3mBO71iciaVSD7mPaAoniahxjn5vKYWLCcQsA/640)




这里的![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRTVjslEkCNnibMJpGvGlS2zg3smglhViaMtUcLYe3fVrLabqMaGBtdDoA/640)是前一阶段的 q(x)。从中解出 q̃(x)，代入 loss 得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRBAuxYicMnP4wp9zpUhcdichgdTZukRDmLWYz6DhspN29FMnLQaXPOoJg/640)




**基本分析**




可以看到，第一项就是标准的 GAN 生成器所采用的 loss 之一。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRMibs4Wcfs8SmhJGfmb0z2riaeBXuQfEhITZiafYKA3G6Wbz5sshtNEKeQ/640)




多出来的第二项，描述了新分布与旧分布之间的距离。这两项 loss 是对抗的，因为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRYq0eQScIz07by7QLVnHabmJb0IbjhLsRyAXqNDSYmibsGaSSG8D0kbw/640)希望新旧分布尽量一致，但是如果判别器充分优化的话，对于旧分布![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRaGmrCENibleOFxBbU2A9MB10OTonq58S3ib5nPMANvNXOnuCPJ18TcsA/640)中的样本，D(x) 都很小（几乎都被识别为负样本），所以 −logD(x) 会相当大，反之亦然。这样一来，**整个 loss 一起优化的话，模型既要“传承”旧分布![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRaGmrCENibleOFxBbU2A9MB10OTonq58S3ib5nPMANvNXOnuCPJ18TcsA/640)，同时要在往新方向 p(1|y) 探索，在新旧之间插值。**




我们知道，目前标准的 GAN 的生成器 loss 都不包含![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRYq0eQScIz07by7QLVnHabmJb0IbjhLsRyAXqNDSYmibsGaSSG8D0kbw/640)，这事实上造成了 loss 的不完备。**假设有一个优化算法总能找到 G(z) 的理论最优解、并且 G(z) 具有无限的拟合能力，那么 G(z) 只需要生成唯一一个使得 D(x) 最大的样本（不管输入的 z 是什么），这就是模型坍缩**。这样说的话，理论上它一定会发生。




那么，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRYq0eQScIz07by7QLVnHabmJb0IbjhLsRyAXqNDSYmibsGaSSG8D0kbw/640)给我们的启发是什么呢？我们设：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRJniaDAZBzpuB9LYSAchY5AU9icyhI82Jat6RFIWWbC333YfuGZZSlicYQ/640)




也就是说，假设当前模型的参数改变量为 Δθ，那么展开到二阶得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRkoEIhmv098tPbkC8KaPUBdVtsiaR0e5OtypDJJFVpicicxfFY86bLMYRg/640)




我们已经指出一个完备的 GAN 生成器的损失函数应该要包含![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRYq0eQScIz07by7QLVnHabmJb0IbjhLsRyAXqNDSYmibsGaSSG8D0kbw/640)，如果不包含的话，那么就要通过各种间接手段达到这个效果，上述近似表明额外的损失约为 (Δθ⋅c)2，这就要求我们不能使得它过大，也就是不能使得 Δθ 过大（在每个阶段 c 可以近似认为是一个常数）。




**而我们用的是基于梯度下降的优化算法，所以 Δθ 正比于梯度，因此标准 GAN 训练时的很多 trick，比如梯度裁剪、用 adam 优化器、用 BN，都可以解释得通了，它们都是为了稳定梯度，使得 θ 不至于过大，同时，G(z) 的迭代次数也不能过多，因为过多同样会导致 Δθ 过大。**




还有，这部分的分析只适用于生成器，而判别器本身并不受约束，因此判别器可以训练到最优。




**正则项**




 现在我们从中算出一些真正有用的内容，直接对![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRYq0eQScIz07by7QLVnHabmJb0IbjhLsRyAXqNDSYmibsGaSSG8D0kbw/640)进行估算，以得到一个可以在实际训练中使用的正则项。直接计算是难以进行的，但我们可以用 KL(q(x,z)‖q̃(x,z)) 去估算它：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRRDicXJVkQ93MrYMoficx0iaBVzwGjjHoPMia6e3WQkWz9EDJPLQhOkJemQ/640)




因为有极限：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRibSNWNbtC744atlfmrLbAmkM7hl9lHcJdofxiaOwXWicia2DOFibHlib9JJw/640)




所以可以将 δ(x) 看成是小方差的高斯分布，代入算得也就是我们有：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRDQY7s7qD5X5ibUTG4hr6NTrKZGH29vt9E516nzYZJv3nNDb4QOaneDA/640)

所以完整生成器的 loss 可以选为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRc8vHA8GemmtyahrpD045WSSaCf4JMmOAMnxZEAyB1ASHGyXaFZneuQ/640)




也就是说，可以用新旧生成样本的距离作为正则项，正则项保证模型不会过于偏离旧分布。




下面的两个在人脸数据 CelebA 上的实验表明这个正则项是生效的。实验代码修改自：




https://github.com/LynnHo/DCGAN-LSGAN-WGAN-WGAN-GP-Tensorflow




**实验一：普通的 DCGAN 网络，每次迭代生成器和判别器各训练一个 batch。**



**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRtLoEKXUKyZl7BxING5KlibA9uNd10SZq7An66yDGib5oHOhO6XPVJqFw/640)**

▲ 不带正则项，在25个epoch之后模型开始坍缩




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRiawQcf9Jy8rFGZ9dmfEx1LiaTLg4hBib4Eo6g7sPBast4AYHibcpO79hKQ/640)

▲ 带有正则项，模型能一直稳定训练




**实验二：普通的 DCGAN 网络，但去掉 BN，每次迭代生成器和判别器各训练五个 batch。**




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRKw2Yu76GyAhcZDPs3qwHphqU8PRKXMDrEicREk7aRYpYstibeicp67xhA/640)

▲ 不带正则项，模型收敛速度比较慢


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRljgZb8yCgib9M5rdQ2ZOSbMYbYGBTRUysibuCxaIWcqCPH3ntlxiaUUNQ/640)

▲ 带有正则项，模型更快“步入正轨”




# GAN相关模型




对抗自编码器（Adversarial Autoencoders，AAE）和对抗推断学习（Adversarially Learned Inference，ALI）这两个模型是 GAN 的变种之一，也可以被纳入到变分推断中。当然，有了前述准备后，这仅仅就像两道作业题罢了。 




有意思的是，在 ALI 之中，我们有一些反直觉的结果。




**GAN视角下的AAE**




事实上，只需要在 GAN 的论述中，将 x,z 的位置交换，就得到了 AAE 的框架。 




具体来说，AAE 希望能训练一个编码模型 p(z|x)，来将真实分布 q̃(x) 映射为标准高斯分布 q(z)=N(z;0,I)，而：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRYjesolzeZykd2T7G1kWkkHHDeQuicqVQOMZKY7bJnIpicpGFMmUusFRw/640)




其中 E(x) 即为编码器的神经网络。




同 GAN 一样，AAE 引入了一个二元的隐变量 y，并有：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRsBWO5aic2tgxNbf5ljGADicAkz3a8cajc426KNeagfN2Mg0OV6ExdYIA/640)




同样直接取 p1=p0=1/2。另一方面，我们设 q(z,y)=q(y|z)q(z)，这里的后验分布 p(y|z) 是一个输入为 z 的二元分布，然后去优化 KL(p(z,y)‖q(z,y))：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRR9GjWBo73PA8zwPZqU0r7frLeF3jnUuLTSptZyd4ZA1KEQ6fYhRib6vg/640)




现在我们优化对象有 q(y|z) 和 E(x)，记 q(0|z)=D(z)，依然交替优化：先固定 E(x)，这也意味着 p(z) 固定了，然后优化 q(y|z)，这时候略去常量，得到优化目标为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRR2nCN8CnkKtHwdNK9k9q2afZJPJwIcMkuzeGiciaNFgUxZJaPN3UEQFSw/640)




然后固定 D(z) 来优化 E(x)，这时候相关的 loss 为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRTCbicWXkFEbibficpkTQ95iaeVuv0hdnk03e85lOux6PsWUIWoicVmoybSQ/640)




利用 D(z) 的理论最优解![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRREDtqXpreyvQbXtd5APHzpJBfoMq4icskL8KCazB91CkGlz2IdUbJhxg/640)，代入 loss 得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRHLwsmicgD9bOlibnRNQDiaosWvE0IPnfWf0lHzHRchmVkFbDqFvHibWPHA/640)




一方面，同标准 GAN 一样，谨慎地训练，我们可以去掉第二项，得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRJicJwVick0V3nMg2zQA563fibZnM3D4mXK4QStZpoE1fRLlJCull51VUg/640)




另外一方面，我们可以得到编码器后再训练一个解码器 G(z)，但是如果所假设的 E(x),G(z) 的拟合能力是充分的，重构误差可以足够小，那么将 G(z) 加入到上述 loss 中并不会干扰 GAN 的训练，因此可以联合训练：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRwT0VQCCwnH1iblia8KIY4n7J0q0Ua77yOUNxyslA73cqh3xrHZSIayvg/640)



**反直觉的ALI版本**




ALI 像是 GAN 和 AAE 的融合，另一个几乎一样的工作是 Bidirectional GAN (BiGAN)。相比于 GAN，它将 z 也作为隐变量纳入到变分推断中。具体来说，在 ALI 中有：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRXlbPPDZg8YUZH90B1ea8JUwHib7kibrGFXUrw5ROFRmiaCOKlteLqsgaA/640)




以及 p(x,z,y)=p(y|x,z)p(z|x)p̃(x)，然后去优化 KL(q(x,z,y)‖p(x,z,y))：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRHefjo9a0niaSltiaWFOpKCW0lGC89GTezmDn8viaWxiaMxHBQsyb2ZSYUA/640)




等价于最小化：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRAiaN6v25FaeiaKdgXtEdJ5WS4jlAovt8y8oOsZG9j6fbwbIuu1VZCiaicQ/640)




现在优化的对象有 p(y|x,z),p(z|x),q(x|z)，记 p(1|x,z)=D(x,z)，而 p(z|x) 是一个带有编码器E的高斯分布或狄拉克分布，q(x|z) 是一个带有生成器 G 的高斯分布或狄拉克分布。依然交替优化：先固定 E,G，那么与 D 相关的 loss 为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRTFtn9y06o4d6LibWmqrtuetwUgibF87sGndafWhwZIDHFZYuaFC2lEsw/640)




跟 VAE 一样，对 p(z|x) 和 q(x|z) 的期望可以通过“重参数”技巧完成。接着固定 D 来优化 G,E，因为这时候有 E 又有 G，整个 loss 没得化简，还是 (37) 那样。但利用 D 的最优解：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRv7E10Je2nXCPDkbo2NG7ApDibP6ll5icLlkvqw1aiaklGO9BPjAkWNIhw/640)




可以转化为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRDMt5hlMnDSoaibkXPbGL0r2H7CbRum2p6O0n5eDp4q7ia6d6C65Z1uCQ/640)




由于 q(x|z),p(x|z) 都是高斯分布，事实上后两项我们可以具体地算出来（配合重参数技巧），但同标准 GAN 一样，谨慎地训练，我们可以简单地去掉后面两项，得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRR0EMBaWth7vXZ24Lhee6BP4TokG3ib3uYH4hlQRGdfJhaDdx3GOJtOicA/640)




这就是我们导出的 ALI 的生成器和编码器的 loss，它跟标准的 ALI 结果有所不同。标准的 ALI（包括普通的 GAN）将其视为一个极大极小问题，所以生成器和编码器的 loss 为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRpQ4w4XYGfYljmqEwKG2NsAZhcgSe20gmwd3wlca7d4ibsVmhQxl4htg/640)




或：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRRBBRxcTWVHia6lxpAErs159nrhQYtfmSo9J9zldyWD0QBics80eQzjNbQ/640)




它们都不等价于 (41)。针对这个差异，事实上笔者也做了实验，结果表明这里的 ALI 有着和标准的 ALI 同样的表现，甚至可能稍好一些（可能是我的自我良好的错觉，所以就没有放图了）。这说明，将对抗网络视为一个极大极小问题仅仅是一个直觉行为，并非总应该如此。




# 结论综述




本文的结果表明了变分推断确实是一个推导和解释生成模型的统一框架，包括 VAE 和 GAN。通过变分推断的新诠释，我们介绍了变分推断是如何达到这个目的的。 




当然，本文不是第一篇提出用变分推断研究 GAN 这个想法的文章。在《On Unifying Deep Generative Models》一文中，其作者也试图用变分推断统一 VAE 和 GAN，也得到了一些启发性的结果。但笔者觉得那不够清晰。事实上，我并没有完全读懂这篇文章，我不大确定，这篇文章究竟是将 GAN 纳入到了变分推断中了，还是将 VAE 纳入到了 GAN 中。相对而言，我觉得本文的论述更加清晰、明确一些。 




看起来变分推断还有很大的挖掘空间，等待着我们去探索。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**以下是简单粗暴送书环节**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgm5Wb1iaUHxx8mBh1Km3dWjfPlgYsxpxlV44icJWDVwuPorALMxCQglAC8Dx8JMeic5wHeNw29gJV8SA/640)



PaperWeekly × 图灵教育





**深度学习入门**

**基于Python的理论与实现**

<5本>




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgmRBd2wgShXsbxgrHaNlNRR48D9JCRxAtSia544rgfL6MHYkOqm0uMd4o9EaEyH8PpRLMibHicAiaqvuQ/640)

**经典畅销√入门必备√**火热预售****√****




**作者：**斋藤康毅

**译者：**陆宇杰



- 
日本深度学习入门经典畅销书，原版上市不足2年印刷已达100000册

- 
长期位列日亚“人工智能”类图书榜首，超多五星好评

- 
使用Python 3，尽量不依赖外部库或工具，从零创建一个深度学习模型

- 
相比AI圣经“花书”，本书更合适入门





书中不仅介绍了深度学习和神经网络的概念、特征等基础知识，对误差反向传播法、卷积神经网络等也有深入讲解，此外还介绍了深度学习相关的实用技巧，自动驾驶、图像生成、强化学习等方面的应用，以及为什么加深层可以提高识别精度等“为什么”的问题。



** 参与方式 **




请在文末留言分享

**VAE和GAN各有哪些优劣势？**



小编将随机抽取5位同学

送出图灵教育新书




**截止时间：**7月22日（周日）20:00







**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**

**#****活 动 推 荐#**




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnEGjEDaYwgpr1o2BDCGUy1YzIgyGATs37z3RvibzlgWlNWpSNSff28y56yMvOhgMouxMY0hCD2EXQ/640?)

**▲**戳我查看比赛详情



**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 查看作者博客





# WGAN-div：默默无闻的WGAN填坑者 | 附开源代码 - Paper weekly - CSDN博客





2018年11月08日 12:29:25[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：83









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨苏剑林

单位丨广州火焰信息科技有限公司

研究方向丨NLP，神经网络

个人主页丨kexue.fm




今天我们来谈一下** Wasserstein 散度，简称“W 散度”。**注意，这跟 **Wasserstein 距离**（Wasserstein distance，简称“W 距离”，又叫 Wasserstein 度量、Wasserstein metric）是不同的两个东西。 




**本文源于论文 *Wasserstein Divergence for GANs*，论文中提出了称为 WGAN-div 的 GAN 训练方案。**



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRos5NlibsGnVJV7sRvT2BTc7ibr8OBoZAcFOWgicMquPHDOH51lzPb8LOn7w/640)



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosFplgZrFNw1WiaibGRLL3ORMRY8ekFicvI7EZX7fSecXHdfq1ibia4XcYYIA/640)




这是一篇我很是欣赏却默默无闻的 paper，我只是找文献时偶然碰到了它。不管英文还是中文界，它似乎都没有流行起来，但是我感觉它是一个相当漂亮的结果。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosC7A0FEsueicU68SQKqSrQqI3VibNJY1QHst3YwyayWY3PoxT69ib2YYrQ/640)

▲ WGAN-div的部分样本（2w iter）




如果读者需要入门一下 WGAN 的相关知识，不妨请阅读拙作[互怼的艺术：从零直达 WGAN-GP](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247484880&idx=1&sn=4b2e976cc715c9fe2d022ff6923879a8&chksm=96e9da50a19e5346307b54f5ce172e355ccaba890aa157ce50fda68eeaccba6ea05425f6ad76&scene=21#wechat_redirect)。




# WGAN




我们知道原始的 GAN（SGAN）会有可能存在梯度消失的问题，因此 ***WGAN*** 横空出世了。




**W距离**




WGAN 引入了最优传输里边的 W 距离来度量两个分布的距离：



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosvnVXMTQVE1v8ZbOiaObx2PIuzItZBEmmgTpp5wncBvWg78Z0GtyCPfg/640)




这里的 p̃(x) 是真实样本的分布，q(x) 是伪造分布，c(x,y) 是传输成本，论文中用的是 c(x,y)=‖x−y‖；而 γ∈Π(p̃(x),q(x)) 的意思是说：γ 是任意关于 x,y 的二元分布，其边缘分布则为 p̃(x) 和 q(y)。




直观来看，γ 描述了一个运输方案，而 c(x,y) 则是运输成本，Wc(p̃(x),q(x)) 就是说要找到成本最低的那个运输方案所对应的成本作为分布度量。




**对偶问题**




W 距离确实是一个很好的度量，但显然不好算。当 c(x,y)=‖x−y‖ 时，我们可以将其转化为对偶问题：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosUuiaibvFLrXzcLPUY2W3vibLBM9DdphESerQzT1HCUzjAj6d9w4v3xkzQ/640)




其中 T(x) 是一个标量函数，‖T‖L 则是 Lipschitz 范数：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRoseg3xGKTXuOvqXmLfKbQgXLgZznASLHwmIUNAfe09S0Ekeuco09JeHA/640)




说白了，T(x) 要满足：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosyyVQKLaJqZXeSARCScUibQpGpbib6ygDAf6a7KpgqSuVqujscIUsmgBA/640)




**生成模型**




这样一来，生成模型的训练，可以作为 W 距离下的一个最小-最大问题：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosQib71wKeicUH1iaPKGlSBx9QtL3wicRsAyDBicaPoRyT7exZSTx2aaLkAuw/640)




第一个 arg max 试图获得 W 距离的近似表达式，而第二个 arg min 则试图最小化 W 距离。




然而，T 不是任意的，需要满足 ‖T‖L≤1，这称为 Lipschitz 约束（L 约束），该怎么施加这个约束呢？**因此，一方面，WGAN 开创了 GAN 的一个新流派，使得 GAN 的理论上了一个新高度，另一方面，WGAN 也挖了一个关于 L 约束的大坑，这个坑也引得不少研究者前仆后继地跳坑。**




# L约束




目前，往模型中加入 L 约束，有三种主要的方案。




**权重裁剪**




这是 WGAN 最原始的论文所提出的一种方案：在每一步的判别器的梯度下降后，将判别器的参数的绝对值裁剪到不超过某个固定常数。 




这是一种非常朴素的做法，现在基本上已经不用了。其思想就是：L 约束本质上就是要网络的波动程度不能超过一个线性函数，而激活函数通常都满足这个条件，所以只需要考虑网络权重，最简单的一种方案就是直接限制权重范围，这样就不会抖动太剧烈了。 




**梯度惩罚**




这种思路非常直接，即 ‖T‖L≤1 可以由 ‖∇T‖≤1 来保证，所以干脆把判别器的梯度作为一个惩罚项加入到判别器的 loss 中：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosBuCG8HtGobliaOWt9Wv7m8Br54HhbzbQYENSjOGS3xoGhZLuibZ1QIgg/640)




但问题是我们要求 ‖T‖L≤1 是在每一处都成立，所以 r(x) 应该是全空间的均匀分布才行，显然这很难做到。**所以作者采用了一个非常机智（也有点流氓）的做法：在真假样本之间随机插值来惩罚，这样保证真假样本之间的过渡区域满足 L 约束。**




这种方案就是 WGAN-GP。显然，它比权重裁剪要高明一些，而且通常都 work 得很好。但是这种方案是一种经验方案，没有更完备的理论支撑。




**谱归一化**




另一种实现 L 约束的方案就是谱归一化（SN），可以参考我之前写的文章[深度学习中的Lipschitz约束：泛化与生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492180&idx=1&sn=3ea92a3a9f1306efde89ce1777b80da6&chksm=96ea3dd4a19db4c20dcbc9627b0eb307672b4d61008a93c42814fa6728ca7b6f7c293cff1d80&scene=21#wechat_redirect)。 




本质上来说，谱归一化和权重裁剪都是同一类方案，只是谱归一化的理论更完备，结果更加松弛。而且还有一点不同的是：权重裁剪是一种“事后”的处理方案，也就是每次梯度下降后才直接裁剪参数，这种处理方案本身就可能导致优化上的不稳定；谱归一化是一种“事前”的处理方案，它直接将每一层的权重都谱归一化后才进行运算，谱归一化作为了模型的一部分，更加合理一些。 




**尽管谱归一化更加高明，但是它跟权重裁剪一样存在一个问题：把判别器限制在了一小簇函数之间。**也就是说，加了谱归一化的 T，只是所有满足 L 约束的函数的一小部分。因为谱归一化事实上要求网络的每一层都满足 L 约束，但这个条件太死了，也许这一层可以不满足 L 约束，下一层则满足更强的 L 约束，两者抵消，整体就满足 L 约束，但谱归一化不能适应这种情况。




# WGAN-div




在这种情况下，***Wasserstein Divergence for GANs ***引入了 W 散度，它声称：**现在我们可以去掉 L 约束了，并且还保留了 W 距离的好性质。**




**论文回顾**




有这样的好事？我们来看看 W 散度是什么。一上来，作者先回顾了一些经典的 GAN 的训练方案，然后随手扔出一篇文献，叫做 ***Partial differential equations and monge-kantorovich mass transfer***[1]，里边提供了一个方案（下面的出场顺序跟论文有所不同），能直接将 T 训练出来，目标是（跟原文的写法有些不一样）：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosq6k7tXZBDMiaNfOoTDIjicv0Dwg16HjHfehbp5hhjj9ibWChhiaT4oVA4A/640)




这里的 r(x) 是一个非常宽松的分布，我们后面再细谈。整个 loss 的意思是：你只要按照这个公式将 T 训练出来，它就是 (2) 式中 T 的最优解，也就是说，接下来只要把它代进 (2) 式，就得到了 W 距离，最小化它就可以得到生成器了。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosibS4D8Z4nWfEzPwju0ibZ54HfwYNIAZFd4WxH3nJdIDh0I3ic3o8d7aKQ/640)




**一些注解**




首先，我为什么说作者“随手”跑出一篇论文呢？因为作者确实是随手啊…… 




作者直接说*“According to [19]”*，然后就给出了后面的结果，[19] 就是这篇论文，是一篇最优传输和偏微分方程的论文，59 页。我翻来翻去，才发现作者引用的应该是 36 页和 40 页的结果（不过翻到了也没能进一步看懂，放弃了），也不提供多一点参考资料，尴尬。




还有后面的一些引理，作者也说*“直接去看 [19] 的 discussion 吧”*..... 然后，读者更多的疑问是：这玩意跟梯度惩罚方案有什么差别，加个负号变成最小化不都是差不多吗？




做实验时也许没有多大差别，但是理论上的差别是很大的，因为 WGAN-GP 的梯度惩罚只能算是一种经验方案，而 (7) 式是有理论保证的。后面我们会继续讲完它。




**W散度**




式 (7) 是一个理论结果，而不管怎样深度学习还是一门理论和工程结合的学科，所以作者一般化地考虑了下面的目标：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosHBqsnW4yQvSsiaXYUV9yibTHXBICp9lqk58VpaPmcficmcU71Y7XJ4DkA/640)




其中 k>0,p>1。基于此，作者证明了 Wk,p 有非常好的性质：




1. Wk,p 是个对称的散度。散度的意思是：D[P,Q]≥0 且 D[P,Q]=0⇔P=Q，它跟“距离”的差别是它不满足三角不等式，也有叫做“半度量”、“半距离”的。Wk,p 是一个散度，这已经非常棒了，因为我们大多数 GAN 都只是在优化某个散度而已。散度意味着当我们最小化它时，我们真正是在缩小两个分布的距离。




2. Wk,p 的最优解跟 W 距离有一定的联系。(7) 式就是一个特殊的 W1/2,2。这说明当我们最大化 Wk,p 得到 T 之后，可以去掉梯度项，通过最小化 (8) 来训练生成器。这也表明以 Wk,p 为目标，性质跟 W 距离类似，不会有梯度消失的问题。




3. 这是我觉得最逗的一点，作者证明了：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosk0vMicZPDnIIwHBI2mTQqw7L87gGhIOd51uY4GRm2DACF5rBKVHt1Aw/640)




不总是一个散度。当 n=1,p=2 时这就是 WGAN-GP 的梯度惩罚，作者说它不是一个散度，明摆着要跟 WGAN-GP 对着干。不是散度意味着 WGAN-GP 在训练判别器的时候，并非总是会在拉大两个分布的距离（鉴别者在偷懒，没有好好提升自己的鉴别技能），从而使得训练生成器时回传的梯度不准。




**WGAN-div**




好了，说了这么久，终于可以引入 WGAN-div 了，其实就是基于 (9) 的 WGAN 的训练模式了：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosDSE8fs8jg7evCN9PEbxPFp3niagibicjnDRdGJad5V0oicU72uwNtQfpGA/640)




前者是为了通过 W 散度 Wk,p 找出 W 距离中最优的 T，后者就是为了最小化 W 距离。所以，W 散度的角色，就是一个为 W 距离的默默无闻的填坑者，再结合这篇论文本身的鲜有反响，我觉得这种感觉更加强烈了。




# 实验




**k,p的选择**




作者通过做了一批搜索实验，发现 k=2,p=6 时效果最好（用 FID 为指标）。这进一步与 WGAN-GP 的做法有出入：范数的二次幂并非是最好的选择。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosP4LXMjPCXVicOTq6f6kSAmiaqJJRq9DENEYjDknYbwsuNlazwxvTia5lw/640)

▲ 不同的k,p对FID的影响（FID越小越好）




**r(x)的选择**




前面我们就说过，W 散度中对 r(x) 的要求非常宽松，论文也做了一组对比实验，对比了常见的做法：



- 
真假样本随机插值

- 
真样本之间随机插值、假样本之间随机插值

- 
真假样本混合后，随机选两个样本插值

- 
直接选原始的真假样本混合

- 
直接只选原始的假样本

- 
直接只选原始的真样本





结果发现，在 WGAN-div 之下这几种做法表现都差不多（用 FID 为指标），但是对于 WGAN-GP，这几种做法差别比较大，而且 WGAN-GP 中最好的结果比 WGAN-div 中最差的结果还要差。这时候 WGAN-GP 就被彻底虐倒了。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRos1zMPkPEXbPtd0ibRXJ77jgmR7r173aw1WlPMdibzcop0SN71vbLZ6xOg/640)

▲ 不同采样方式所导致的不同模型的FID不同差异（FID越小越好）




这里边的差别不难解释，WGAN-GP 是凭经验加上梯度惩罚，并且“真假样本随机插值”只是它无法做到全空间采样的一个折衷做法，但是 W 散度和 WGAN-div，从理论的开始，就没对 r(x) 有什么严格的限制。




其实，**原始 W 散度的构造（这个需要看参考论文）基本上只要求 r(x) 是一个样本空间跟 p̃(x)、q(x) 一样的分布**，非常弱的要求，而我们一般选择为 p̃(x)、q(x) 两者共同衍生出来的分布，相对来说收敛快一点。




**参考代码**




自然是用 Keras 写的，人生苦短，我用 Keras。




https://github.com/bojone/gan/blob/master/keras/wgan_div_celeba.py




随机样本（自己的实验结果）：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRosC7A0FEsueicU68SQKqSrQqI3VibNJY1QHst3YwyayWY3PoxT69ib2YYrQ/640)

▲ WGAN-div的部分样本（2w iter）




当然，原论文的实验结果也表明 WGAN-div 是很优秀的：


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglU5IZ1Euzv7UfksshpZRoshDNJXcCZBWgSv4jHh45jMl4NlbUoCc7Ca1HxTquFflYmy82zA9Pibjg/640)

▲ WGAN-div与不同的模型在不同的数据集效果比较（指标为FID，越小越好）




# 结语




不知道业界是怎么看这篇 WGAN-div 的，也许是觉得跟 WGAN-GP 没什么不同，就觉得没有什么意思了。不过我是很佩服这些从理论上推导并且改进原始结果的大牛及其成果。虽然看起来像是随手甩了一篇论文然后说“你看着办吧”的感觉，但这种将理论和实践结合起来的结果仍然是很有美感的。




本来我对 WGAN-GP 是多少有些芥蒂的，总觉得它太丑，不想用。但是 WGAN-div 出现了，在我心中已经替代了 WGAN-GP，并且它不再丑了。




# 相关链接




 [1]. Evans, L.C.: Partial differential equations and monge-kantorovich mass transfer. Current developments in mathematics 1997(1) (1997) 65–126




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看作者其他文章：**




- 
[变分自编码器VAE：原来是这么一回事 | 附开源代码](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488480&idx=1&sn=7bb9a4dd60680df5013670287a9e0cc2&chksm=96e9cc60a19e457618b2ffdea2a7e13ba172ea3fdfbfda07de53aae57126047f1b71a6969d76&scene=21#wechat_redirect)[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)

- 
[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)

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






**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 查看作者博客





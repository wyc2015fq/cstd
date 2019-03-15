# 当我们在谈论 Deep Learning：GAN 与 WGAN

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

45 人赞同了该文章

**本系列意在长期连载分享，内容上可能也会有所增删改减；**

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

## **引言**

GAN（Generative Adversarial Networks）于2014年由 Lan Goodfellow 提出，被 Yann LeCun 誉为"The coolest idea in deep learning in the last 20 years"。事实上，由于 GAN 独特的思路与惊人的实验效果，它自提出就受到广泛关注，用了不到两年的时间，卷席各种 AI 顶会，方兴未艾；另一方面，经过这些年的发展，GAN 依然很不完善，大部分时候只是 TOY 一般的存在。因此，本文会介绍迄今（2017年）部分基础或广泛认可的 GAN 理论与结构，需要进一步了解的同学需得长期跟进相关论文。

## **GAN**

GAN（Generative Adversarial Networks）最初在“Goodfellow I J, Pougetabadie J, Mirza M, et al. Generative Adversarial Networks.2014”中被提出，是一种无监督的生成模型算法。我们先从概念上进行简单介绍。

在[AutoEncoder 及其相关模型](https://zhuanlan.zhihu.com/p/27865705)中，介绍过生成模型的本质就是对 ![p(x)](https://www.zhihu.com/equation?tex=p%28x%29) 进行建模，此时一般方法会选定模型并用最大似然学习参数。GAN 与一般生成模型最主要的区别就在于此：

1. GAN 并不显式地对 ![p(x)](https://www.zhihu.com/equation?tex=p%28x%29) 建模，而试图利用一个 Generator（G） 来学习出数据的分布
2. 在判断学习出的模型的优劣时，它也并不直接给出误差计算函数，而同样试图用 Discriminator（D） 来自动判断模型的优秀程度

可以看出，GAN 其实跟 DL 领域的趋势是一致的，即将 人工选择复杂模型 的风险转移到 简单模型+人工优化参数 上；而且更进一步，具有一定博弈的意味。当然，这只是感性的说法，下面我们更深入地看看 GAN。

首先，GAN 要优化的目标如下

![\begin{align} & \min_G \max_D V(D, G) \\ & V(D, G) = E_{x \sim P_{data}(x)}[log D(x)] + E_{x \sim P_{z}(z)}[log(1-D(G(z)))] \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26+%5Cmin_G+%5Cmax_D+V%28D%2C+G%29+%5C%5C+%26+V%28D%2C+G%29+%3D+E_%7Bx+%5Csim+P_%7Bdata%7D%28x%29%7D%5Blog+D%28x%29%5D+%2B+E_%7Bx+%5Csim+P_%7Bz%7D%28z%29%7D%5Blog%281-D%28G%28z%29%29%29%5D+%5C%5C+%5Cend%7Balign%7D)

通过迭代训练 Generator 和 Discriminator 来训练 GAN，方法依然是 BP，具体流程如下

![img](https://pic3.zhimg.com/80/v2-51b8f8bdf094bd6ea195e79e85541f82_hd.png)



**目标函数的初步理解**

看过[DNN 与它的参数们（贰）](https://zhuanlan.zhihu.com/p/26392287)的同学对 Binary Cross Entropy Loss 应该不会陌生，它是 DNN 中最常用的 Loss Function 之一，其定义为

![C(y, \hat y)=-\hat ylog(y)-(1-\hat y)log(1-y)](https://www.zhihu.com/equation?tex=C%28y%2C+%5Chat+y%29%3D-%5Chat+ylog%28y%29-%281-%5Chat+y%29log%281-y%29)

其中， ![\hat y](https://www.zhihu.com/equation?tex=%5Chat+y) 代表数据真实 label， ![\hat y\in\{0,1\}](https://www.zhihu.com/equation?tex=%5Chat+y%5Cin%5C%7B0%2C1%5C%7D) ， ![y](https://www.zhihu.com/equation?tex=y) 为预测。

可以看出，对于 GAN 中的 Discriminator 和 Generator，本质上都是在优化平均 Binary Cross Entropy Loss（更准确的说是 Binary Cross Entropy Loss 取反）。证明过程较简单，这里不再赘述。

对于 Discriminator 的训练大家都轻车熟路了，没啥区别。但是对于 Generator，GAN 的优化目标问题在于，当 ![D(G(z))](https://www.zhihu.com/equation?tex=D%28G%28z%29%29) 接近1，即 Discriminator 基本能正确分辨真实与生成的数据时，Generator 就会出现 Gradient Vanish 问题，从而停止进化。这种情况发生的原因一般为：

1. 训练初期，Generator 较弱，Discriminator 很容易就能区分出真实数据与随机数据，Generator 会 Gradient Vanish
2. 训练中期，若 Discriminator 能力远超于同期 Generator，则 Generator 会 Gradient Vanish



**训练流程的理解**

GAN 的训练过程有两点比较重要：

1. 输入随机噪声，期望通过训练让 Generator 产生以假乱真的数据，本质是希望 Generator 能将随机噪声的分布 ![p(z)](https://www.zhihu.com/equation?tex=p%28z%29) 映射到真实数据的分布 ![p(x)](https://www.zhihu.com/equation?tex=p%28x%29)
2. 迭代训练 Generator 和 Discriminator，期间需保持它们能力相近（否则会出现 Gradient Vanish，如上述），期望最后能达到“纳什均衡”。此时虽然 ![p(x)](https://www.zhihu.com/equation?tex=p%28x%29) 显式表达式依然未知，但可以利用 Generator 对数据进行采样，以产生训练样本集中没有的数据

关于第1点，以下给出理论上 Generator 能将随机噪声的分布 ![p(z)](https://www.zhihu.com/equation?tex=p%28z%29) 映射到真实数据的分布 ![p(x)](https://www.zhihu.com/equation?tex=p%28x%29) 的简单证明，基本是概率论基础

![\begin{align} p(x) &= \int_z p(x,z) dz \\ &= \int_z p(x|z)p(z)dz \\ &= \int_z I_{G(z)=x}p(z)dz \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+p%28x%29+%26%3D+%5Cint_z+p%28x%2Cz%29+dz+%5C%5C+%26%3D+%5Cint_z+p%28x%7Cz%29p%28z%29dz+%5C%5C+%26%3D+%5Cint_z+I_%7BG%28z%29%3Dx%7Dp%28z%29dz+%5C%5C+%5Cend%7Balign%7D)

关于第2点，就不太乐观了，可能会出现 Non-Convergence 的问题。如[Lan Goodfellow 在 Quora 中的回答](http://link.zhihu.com/?target=https%3A//www.quora.com/What-research-directions-is-Ian-Goodfellow-pursuing-to-improve-Generative-Adversarial-Networks)所说，“所有的理论都认为 GAN 应该在纳什均衡上有卓越的表现，但梯度下降只有在凸函数的情况下才能保证纳什均衡。当博弈双方都由神经网络表示时，在没有实际达到均衡的情况下，让它们永远保持对自己策略的调整是可能的。”



**目标函数的进一步理解**

限于篇幅，这部分的推导均会省略，有兴趣的同学可以查阅原文或相关文献。

前文介绍了 GAN 的训练是迭代训练 Discriminator 和 Generator，且优化的目标是 Binary Cross Entropy Loss。接下来我们来考虑某一轮的训练，当前时刻的 Discriminator 和 Generator 分别是 D 和 G，我们需要先训练出新的 ![D^*](https://www.zhihu.com/equation?tex=D%5E%2A) ，满足

![D^* = \max_D V(D, G)](https://www.zhihu.com/equation?tex=D%5E%2A+%3D+%5Cmax_D+V%28D%2C+G%29)

对于给定样本 ![x](https://www.zhihu.com/equation?tex=x) ，对它而言最好的 ![D^*(x)](https://www.zhihu.com/equation?tex=D%5E%2A%28x%29) 会是

![D^{*}(x) = argmax_D P_{data}(x)log(D(x))+P_G(x)log(1-D(x))](https://www.zhihu.com/equation?tex=D%5E%7B%2A%7D%28x%29+%3D+argmax_D+P_%7Bdata%7D%28x%29log%28D%28x%29%29%2BP_G%28x%29log%281-D%28x%29%29)

其结果为

![D^{*}(x) = \frac {P_{data}(x)}{P_{data}(x)+P_G(x)}](https://www.zhihu.com/equation?tex=D%5E%7B%2A%7D%28x%29+%3D+%5Cfrac+%7BP_%7Bdata%7D%28x%29%7D%7BP_%7Bdata%7D%28x%29%2BP_G%28x%29%7D)

将其带入 ![V(D, G)](https://www.zhihu.com/equation?tex=V%28D%2C+G%29) ，可以化简得到

![V(D_*, G) = -2log2 + JSD(P_{data}(x)||P_G(x))](https://www.zhihu.com/equation?tex=V%28D_%2A%2C+G%29+%3D+-2log2+%2B+JSD%28P_%7Bdata%7D%28x%29%7C%7CP_G%28x%29%29)

其中， ![JSD](https://www.zhihu.com/equation?tex=JSD) 表示 JS Divergence，其范围为 ![[0, 2log2]](https://www.zhihu.com/equation?tex=%5B0%2C+2log2%5D) 。 ![JSD](https://www.zhihu.com/equation?tex=JSD) 公式如下，其中 ![KL](https://www.zhihu.com/equation?tex=KL) 表示 KL Divergence

![\begin{align} & JSD(P||Q) = \frac12KL(P||M) + \frac12KL(Q||M) \\ & M = \frac12(P+Q) \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26+JSD%28P%7C%7CQ%29+%3D+%5Cfrac12KL%28P%7C%7CM%29+%2B+%5Cfrac12KL%28Q%7C%7CM%29+%5C%5C+%26+M+%3D+%5Cfrac12%28P%2BQ%29+%5C%5C+%5Cend%7Balign%7D)

即，接下来我们要求解的 ![G^*](https://www.zhihu.com/equation?tex=G%5E%2A) 是满足

![\begin{align} G^* &= \min_G \max_D V(D, G) \\ &= \min_G -2log2 + JSD(P_{data}(x)||P_G(x)) \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+G%5E%2A+%26%3D+%5Cmin_G+%5Cmax_D+V%28D%2C+G%29+%5C%5C+%26%3D+%5Cmin_G+-2log2+%2B+JSD%28P_%7Bdata%7D%28x%29%7C%7CP_G%28x%29%29+%5C%5C+%5Cend%7Balign%7D)

毫无疑问，其结果就是

![G^* = arg_{G} (P_G(x) = P_{data}(x)) ](https://www.zhihu.com/equation?tex=G%5E%2A+%3D+arg_%7BG%7D+%28P_G%28x%29+%3D+P_%7Bdata%7D%28x%29%29+)

这里，被用于衡量 ![P_G](https://www.zhihu.com/equation?tex=P_G) 与 ![P_{data}](https://www.zhihu.com/equation?tex=P_%7Bdata%7D) 的距离的指标最好能随着两者的相似性增加逐渐减小为0。可是 JS Divergence 并不太适用于这个场景，原因在于 ![P_{JSD(P_{data}, P_G)=2log2}=1](https://www.zhihu.com/equation?tex=P_%7BJSD%28P_%7Bdata%7D%2C+P_G%29%3D2log2%7D%3D1) 。也就是，绝大多数情况下而言 ![JSD=2log2](https://www.zhihu.com/equation?tex=JSD%3D2log2) ，而在 ![P_{data}=P_G](https://www.zhihu.com/equation?tex=P_%7Bdata%7D%3DP_G) 时， ![JSD](https://www.zhihu.com/equation?tex=JSD) 会突变为0。因此，正常情况下根本不会产生 Gradient 来优化 Generator 。在“Arjovsky M, Bottou L. Towards Principled Methods for Training Generative Adversarial Networks. 2017.”中作者给出了详细证明。这里我们只给出图例说明，如下图两条曲线：它们虽然有重合，但是相比于两条曲线占有的空间（想象是两条很长的曲线）几乎是忽略不计的。

![img](https://pic3.zhimg.com/80/v2-2ff1be44574bcf98343e1a4a7922367e_hd.png)



**GAN 与 VAE 的区别**

最后，作为两大主流生成模型，我们来谈谈 GAN 与 VAE 的区别

1. VAE 在理论上需要利用 Variational Inference 进行近似，进而引入偏差；而 GAN 不需要这种近似，即理想情况下 GAN 能更好地拟合数据真实分布。事实上，VAE 确实在生成图像时更加模糊，而 GAN 相对锐利
2. VAE 作为 AE 的一种，本质上是对数据的压缩，同时可以用于生成新数据；而 GAN 则主要关注还原真实数据分布，通过采样生成新数据



## **WGAN**

原始 GAN 其实存在各种问题，鲁棒性极差，如：非常依赖 Discriminator 和 Generator 的结构设计、需要精心的调节超参数、没有一种指标来量化训练的进度、还可能出现 Mode Collapse，不一而足。自 GAN 提出后，大量的文章都尝试解决 GAN 的某些问题，而 WGAN（Wasserstein GAN）则几乎同时解决（缓解）了上述所有的不足，引发了大量的关注与讨论。下面，我们简单介绍下这个相对成熟版本的 GAN。

其实 WGAN 的提出经历了三个阶段：

1. 在“Arjovsky M, Bottou L. Towards Principled Methods for Training Generative Adversarial Networks. 2017.”，作者主要对 GAN 存在的问题进行了严谨的理论分析，非常精彩
2. 在“Arjovsky M, Chintala S, Bottou L. Wasserstein GAN. 2017.”，作者提出了 WGAN，最大的贡献在于利用 Wasserstein Distance（Earth-Mover Distance）代替 GAN 中的 JS Divergence，极大缓解了 GAN 难以训练的问题
3. 在“Gulrajani I, Ahmed F, Arjovsky M, et al. Improved Training of Wasserstein GANs. 2017.”，作者对 WGAN 的目标函数进行了优化，在改进了训练过程的同时也能达到更好的效果

下面我们简单介绍一下 WGAN 及 Improved WGAN。

上文提到，原始的 GAN 由于 JS Divergence 衡量分布距离时突变的性质，训练 Generator 会出现 Gradient Vanish 的问题；于是，WGAN 改用 Wasserstein Distance（Earth-Mover Distance）来衡量两个分布的距离。Wasserstein Distance 具体定义限于篇幅不展开了，其实跟后面关系也不大，有兴趣的同学可以查看原文献。Wasserstein Distance 的一个重要性质就是变化是平滑的，也就是说能够很好地描述两个分布逐渐变得相似的这个过程，根本上解决了 Gradient Vanish 的问题。

更让人意想不到的是，作者在历史文献中找到了将 Wasserstein Distance 直接套用 GAN 框架中的理论依据，即可以推导出分布 ![P_r](https://www.zhihu.com/equation?tex=P_r) 与 ![P_{\theta}](https://www.zhihu.com/equation?tex=P_%7B%5Ctheta%7D) 的Wasserstein Distance可以写作

![W(P_r, P_{\theta}) = \sup_{||f||_L \le 1} E_{x \sim P_r}[f(x)] - E_{x \sim P_{\theta}}[f(x)]](https://www.zhihu.com/equation?tex=W%28P_r%2C+P_%7B%5Ctheta%7D%29+%3D+%5Csup_%7B%7C%7Cf%7C%7C_L+%5Cle+1%7D+E_%7Bx+%5Csim+P_r%7D%5Bf%28x%29%5D+-+E_%7Bx+%5Csim+P_%7B%5Ctheta%7D%7D%5Bf%28x%29%5D)

其中， ![\sup](https://www.zhihu.com/equation?tex=%5Csup) 表示上确界； ![||f||_L](https://www.zhihu.com/equation?tex=%7C%7Cf%7C%7C_L) 表示函数 ![f](https://www.zhihu.com/equation?tex=f) 的 Lipschitz 常数小于等于1，即对任意 ![x_1, x_2](https://www.zhihu.com/equation?tex=x_1%2C+x_2)需要 ![f](https://www.zhihu.com/equation?tex=f) 满足

![||f(x_1)-f(x_2)|| \le ||x_1-x_2||](https://www.zhihu.com/equation?tex=%7C%7Cf%28x_1%29-f%28x_2%29%7C%7C+%5Cle+%7C%7Cx_1-x_2%7C%7C)

上式的含义为，对所有满足 ![||f||_L \le 1](https://www.zhihu.com/equation?tex=%7C%7Cf%7C%7C_L+%5Cle+1) 的 ![f](https://www.zhihu.com/equation?tex=f) ，计算 ![E_{x \sim P_r}[f(x)] - E_{x \sim P_{\theta}}[f(x)]](https://www.zhihu.com/equation?tex=E_%7Bx+%5Csim+P_r%7D%5Bf%28x%29%5D+-+E_%7Bx+%5Csim+P_%7B%5Ctheta%7D%7D%5Bf%28x%29%5D) ，并取其最大值。如果代入 GAN 的场景，即

![W(P_{data}, P_G) = \max_{D \in 1-Lipschitz} E_{x \sim P_{data}}[D(x)] - E_{x \sim P_G}[D(x)]](https://www.zhihu.com/equation?tex=W%28P_%7Bdata%7D%2C+P_G%29+%3D+%5Cmax_%7BD+%5Cin+1-Lipschitz%7D+E_%7Bx+%5Csim+P_%7Bdata%7D%7D%5BD%28x%29%5D+-+E_%7Bx+%5Csim+P_G%7D%5BD%28x%29%5D)

可以看出，使用 Wasserstein Distance 时目标函数与原始 GAN 不同，但同样可以用神经网络来学习 Discriminator 和 Generator。区别仅在于 ![D \in 1-Lipschitz](https://www.zhihu.com/equation?tex=D+%5Cin+1-Lipschitz) 这个约束。

原始 WGAN 利用 weight clipping 的方式来处理这个带约束的问题，很快“Gulrajani I, Ahmed F, Arjovsky M, et al. Improved Training of Wasserstein GANs. 2017.”的作者提出了更加优秀的解决方法，这里我们只简单介绍下后者。作者将上述约束转化为目标函数中的 Penalty 项，这其实也是很多约束优化算法的做法，具体优化目标转变为

![V(D, G) = E_{x \sim P_{data}}[D(x)] - E_{x \sim P_G}[D(x)] - \lambda E_{\hat x \sim P_{\hat x}}[(||\nabla_{\hat x}D(\hat x)||_2-1)^2]](https://www.zhihu.com/equation?tex=V%28D%2C+G%29+%3D+E_%7Bx+%5Csim+P_%7Bdata%7D%7D%5BD%28x%29%5D+-+E_%7Bx+%5Csim+P_G%7D%5BD%28x%29%5D+-+%5Clambda+E_%7B%5Chat+x+%5Csim+P_%7B%5Chat+x%7D%7D%5B%28%7C%7C%5Cnabla_%7B%5Chat+x%7DD%28%5Chat+x%29%7C%7C_2-1%29%5E2%5D)

关于 Penalty 项有两点需要说明：

1. 对于 ![P_{\hat x}](https://www.zhihu.com/equation?tex=P_%7B%5Chat+x%7D) ，这里并不是对整个空间采样，而只对 ![P_{data}, P_G](https://www.zhihu.com/equation?tex=P_%7Bdata%7D%2C+P_G) 之间的空间采样。这里就不展开了，有需要了解细节的同学请参考原文
2. 直接观察 Penalty 项，其意义是期望 ![||\nabla_{\hat x}D(\hat x)||](https://www.zhihu.com/equation?tex=%7C%7C%5Cnabla_%7B%5Chat+x%7DD%28%5Chat+x%29%7C%7C) 逼近1，而非严格小于等于1。关于这点，其原因在于在 ![\hat x](https://www.zhihu.com/equation?tex=%5Chat+x) 的采样区域中， ![||\nabla_{\hat x}D(\hat x)||](https://www.zhihu.com/equation?tex=%7C%7C%5Cnabla_%7B%5Chat+x%7DD%28%5Chat+x%29%7C%7C) 实际上通常为1；且用范数而非 ![max](https://www.zhihu.com/equation?tex=max) 能加快收敛速度并提升效果。原文同样有更详细说明

最终，WGAN 的训练步骤为

![img](https://pic1.zhimg.com/80/v2-d1323f5fd4fe8496f6080051ee2dbc3c_hd.png)

WGAN 在效果上虽然没有飞跃的进步，但其巨大的意义在于使 GAN 的实用性大大提升，体现主要体现为以下方面：

1. 不再需要精心平衡 Discriminator 和 Generator 的能力了，且效果更稳健，不依赖于过于精心设计（trick）的结构
2. 缓解了 Mode Collapse 现象，确保生成的数据的多样性
3. 最重要的是，有一个数值可以定量描述并跟踪训练的效果了

可以预见，在缓解了这些问题后，GAN 研究的价值和热度都会得到显著的提升，不得不说是比较振奋人心的。

## **尾巴**

这一篇拖更了很久了，近期更新节奏也会比较慢，工作和生活上都需要投入更多的心思。不过，最近还是去参加了 **Chris Manning** 和 **Sebastian Thrun** 两位大牛的报告，感觉还是有一定收获的，以后有机会聊一聊。

下一篇大概会开启 Reinforcement Learning 部分，具体安排还得再考虑下，毕竟这领域是个巨坑，作为专栏也只能抽取些有意思的部分谈一谈。



## **本系列其他文章：**

[专栏总目录 - 知乎专栏](https://zhuanlan.zhihu.com/p/25091816?refer=data-miner)
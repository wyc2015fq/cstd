
# 能量视角下的GAN模型（二）：GAN＝“分析”＋“采样” - Paper weekly - CSDN博客


2019年02月20日 08:53:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：76


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨苏剑林
单位丨广州火焰信息科技有限公司
研究方向丨NLP，神经网络
个人主页丨kexue.fm

在这个系列中，我们尝试从能量的视角理解 GAN。我们会发现这个视角如此美妙和直观，甚至让人拍案叫绝。

上一篇文章里，我们给出了一个直白而用力的能量图景，这个图景可以让我们轻松理解 GAN 的很多内容，换句话说，通俗的解释已经能让我们完成大部分的理解了，并且把最终的结论都已经写了出来。

在这篇文章中，我们继续从能量的视角理解 GAN，这一次，我们争取**把前面简单直白的描述，用相对严密的数学语言推导一遍。**

跟第一篇文章一样，对于笔者来说，这个推导过程依然直接受启发于 Bengio 团队的新作*Maximum Entropy Generators for Energy-Based Models*。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnCvjTzKxylbXhn7Ckib11hm5E3icRXnpRibHOicU00TQX8FQibicyCmrhAAdAEzpQcicJXWyHr4ibrWRGWtg/640?wx_fmt=png)

**原作者的开源实现：**

https://github.com/ritheshkumar95/energy_based_generative_models

**本文的大致内容如下：**

1. 推导了能量分布下的正负相对抗的更新公式；
2. 比较了理论分析与实验采样的区别，而将两者结合便得到了 GAN 框架；

3. 导出了生成器的补充 loss，理论上可以防止 mode collapse；

4. 简单提及了基于能量函数的 MCMC 采样。

# 数学视角的能量

在这部分中，我们先来简单引入能量模型，并且推导了能量模型理论上的更新公式，指出它具有正相、负相对抗的特点。

**能量分布模型**

首先，我们有一批数据 x1, x2, … , xn∼p(x)，我们希望用一个概率模型去拟合它，我们选取的模型为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID1aU4YpkPx1SeAs197Pq1AIYibRbTtd8rVTm41QPjib3FRVXZWMicnRgBw/640?wx_fmt=png)

其中 Uθ 是带参数 θ 的未定函数，我们称为“能量函数”，而 Zθ 是归一化因子（配分函数）。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID3IEU8FCLziatDe5UdYjgDoyKdjPTZbicO7Tc3XJ2lTodWsMn8XxnicJzA/640?wx_fmt=png)

这样的分布可以称为**“能量分布”**，在物理中也被称为**“玻尔兹曼分布”**。

至于为什么选择这样的能量分布，解释有很多，既可以说是从物理角度受到启发，也可以说是从最大熵原理中受到启发，甚至你也可以简单地认为只是因为这种分布相对容易处理而已。但不可否认，这种分布很常见、很实用，我们用得非常多的 softmax 激活，其实也就是假设了这种分布。

现在的困难是如何求出参数 θ 来，而困难的来源则是配分函数 (2) 通常难以显式地计算出来。当然，尽管实际计算存在困难，但不妨碍我们继续把推导进行下去。

**正负相的对抗**

为了求出参数 θ ，我们先定义对数似然函数：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDXfBW5b3h8k0H8PSFZdqtLmCd06d44OCocpEPeW57aVLgOT6aw5qgPw/640?wx_fmt=png)

我们希望它越大越好，也就是希望下式越小越好：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID9CIZV1sckCp1Bgsb1lgWEvvmCJicFPb4u8sPBRRPXeZtaIbRNkdaBPQ/640?wx_fmt=png)

为此，我们对 Lθ 使用梯度下降。我们有：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID5E9TwtvQ7zmqLfbYWI7P3HOVTGZ6kRrGJnrxaY3lkZ9fGFVZas9G5A/640?wx_fmt=png)

所以：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID78g0BBsgBIyiaD4ukKgMRDTL0nJ17ekhbN510YwUrxANH2yD52xCoiag/640?wx_fmt=png)

这意味着梯度下降的更新公式是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDlIYL9IrovkXbMCG9RhBDBbwXwAUonGtlKxZZcQibaTZxbfRH8zKZN3A/640?wx_fmt=png)

注意到式 (6) 的特点，它是![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDWYGHknsIwl35E95yuHEaibNbKPoRpiaAXTF33iauIH3Dv4Fpu7gCmTbYw/640?wx_fmt=png)分别在真实分布下和拟合分布下的均值之差，这就是机器学习中著名的**“正相”**和**“负相”**的分解，式 (6) 体现了正负相之间的对抗，也有人将其对应为我们做梦的过程。

# 扬长避短 ⇒ GAN

在这部分中，我们表明“容易分析”与“容易采样”是很难兼容的，**容易理论分析的模型，在实验上难以采样计算，而容易采样计算的模型，难以进行简明的理论推导。而试图将两者的优点结合起来，就得到了 GAN 模型。**

**理论分析与实验采样**

事实上，式 (6) 和式 (7) 表明我们开始假设的能量分布模型的理论分析并不困难，但是落实到实验中，我们发现必须要完成从 qθ 中采样：![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDekXBtlia58jBjaIquSZpcr2YeLDibExUwia0VBBppnbN8pr2yZzwcAuQw/640?wx_fmt=png)。也就是说，给定一个具体的![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDbDPRbeYib4mo6YuPicuEN8hhTucohmg8br87vxxuQ5XBEMwV64dCqF3g/640?wx_fmt=png)，我们要想办法从![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDEfsQSYnrl5VSuKhZQosicXMHNoAUXFZmOjRFpLic4PD7siaXxQHeC7YAg/640?wx_fmt=png)中采样出一批 x 出来。

然而，就目前而言，我们对从![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDEfsQSYnrl5VSuKhZQosicXMHNoAUXFZmOjRFpLic4PD7siaXxQHeC7YAg/640?wx_fmt=png)中采样并没有任何经验。对于我们来说，方便采样的是如下的过程：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDibjceVSfnkQSKibiaN0VEwJWQoToHT5FGWSpX2n5SoAKymS633vZsA6vg/640?wx_fmt=png)

这里的 q(z) 代表着标准正态分布。也就是说，我们可以从标准正态分布中采样出一个 z 出来，然后通过固定的模型 Gφ 变换为我们想要的 x。这意味着这种分布的理论表达式是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID5DvIU690AbbibmtKJap3Eo7xDhdVaW0IRePbJYkZ3YDcXfhtsicm1WUA/640?wx_fmt=png)

问题是，如果用 qφ(x) 代替原来的 qθ(x)，那么采样是方便了，但是类似的理论推导就困难了，换句话说，我们根本推导不出类似 (7) 的结果来。

**GAN诞生记**

那么，一个异想天开的念头是：能不能把两者结合起来，在各自擅长的地方发挥各自的优势？

式 (7) 中的![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDekXBtlia58jBjaIquSZpcr2YeLDibExUwia0VBBppnbN8pr2yZzwcAuQw/640?wx_fmt=png)不是难以实现吗，那我只把这部分用![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDNL4ICjMpeicTk74Nd8HibfbwlkfASTm7LPjTLRCeBN2leehYvsPvVtGQ/640?wx_fmt=png)代替好了：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID1Ko9DeIryziczghsdyzYxVoicuTZjrhyg7P5Jef0vN4eszlhLT8TrKsQ/640?wx_fmt=png)

也就是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDP7pM8VsE37GlcB214XON7LJ0GxppZn5e2zXB4dg6K6ib5g24VEYiaGhw/640?wx_fmt=png)

现在采样是方便了，但前提是 qφ(x) 跟 qθ(x) 足够接近才行呀（因为 qθ(x) 才是标准的、正确的），所以，我们用 KL 散度来度量两者的差异：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDCIZP6QNvpDZiawSx79BdK1CJeblkJp0JMe3H9eiaibCaIvHTMXV8x6dvg/640?wx_fmt=png)

式 (11) 有效的前提是 qφ(x) 跟 qθ(x) 足够接近，也就是上式足够小，而对于固定的 qθ(x)，Zθ 是一个常数，所以 φ 的优化目标是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDL2FvweN4wrY4NNicf55AVqDgzJcFiam50E29njpntWgnEKoeNZoXgpNg/640?wx_fmt=png)

这里![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID82PjF9E33v5KNfqPsFxBBZUHYZvZVAZIziaOOsZ39uxMegGOZ3EEHibA/640?wx_fmt=png)代表 qφ(x) 的熵。−Hφ(X) 希望熵越大越好，这意味着**多样性**；![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDfDsMjvtplmzJ7QgJvfheEsiabGdLncZAmgN8JpbS0F5QsyCwJic14SyA/640?wx_fmt=png)希望图片势能越小越好，这意味着**真实性**。

另外一方面，注意到式 (11) 实际上是目标的梯度下降公式。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDibq4SNV3cDo8yic69SiamwG3NZGNUYiaOibkEtGsOo2GsVQ2EWkeFxdQm9Q/640?wx_fmt=png)

所以我们发现，整个过程实际上就是 (14) 和 (13) 的交替梯度下降。而正如第一篇所说的，θ 的这个目标可能带来数值不稳定性，基于第一篇所说的理由，真样本应该在极小值点附近，所以我们可以把梯度惩罚项补充进 (14)，得到最终的流程是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDC5vdEQ6lo3TJLaIVUc6dl26BNtuD2SsznicRRNWkngEHTnCibbqDmXfA/640?wx_fmt=png)

这便是基于梯度惩罚的 GAN 模型，我们在[能量视角下的GAN模型（一）](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494846&idx=1&sn=0b1bdd770672f038d79b76cc31c1cbb4&chksm=96ea333ea19dba28df70ff1a5bce5b252d0237ca3958946b81ef23dc87b223d5e3491b93b935&scene=21#wechat_redirect)中已经把它“头脑风暴”出来了，而现在我们从能量模型的数学分析中把它推导出来了。

所以说，GAN 实际上就是能量模型和采样模型各自扬长避短的结果。

# 直击H(X)！

现在，距离完整地实现整个模型，就差 Hφ(X) 了。我们已经说过：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDwnKicEc8NkBv8SFyw9zR3SSAhvJwLh7OrTCjhPQhuPlEDOGgrqvicS9g/640?wx_fmt=png)

代表 qφ(x) 的熵，而 qφ(x) 的理论表达式是 (9)，积分难以计算，所以 Hφ(X) 也难以计算。

打破这一困境的思路是将熵转化为互信息，然后转化为互信息的估计，其估计方式有两种：通过 f 散度的方式（理论上精确）估计，或者通过信息下界的方式估计。

**最大熵与互信息**

首先，我们可以利用 x=Gφ(z) 这一点： x=Gφ(z) 意味着条件概率 qφ(x|z)=δ(x−G(z))，即一个确定性的模型，也可以理解为均值为 G(z) 、方差为 0 的高斯分布 N(x;Gφ(z),0)。

然后我们去考虑互信息 I(X,Z)：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDc6wvzp7wKQfFdFH89SRYLeaiawusXbwJ7QD0chMwFT3gSSickibFia5Ffg/640?wx_fmt=png)

现在我们找出了 Iφ(X,Z) 和 Hφ(X) 的关系，它们的差是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDcTfrDlT15MYJqqC26NgAP0CM2gyl8Du1N9sb78Idusia16VKVI7XRSg/640?wx_fmt=png)

事实上 Hφ(X|Z) 称为“条件熵”。

如果我们处理的是**离散型分布**，那么因为 x=Gφ(z) 是确定性的，所以 qφ(x|z)≡1，那么 Hφ(X|Z) 为 0，即 Iφ(X,Z)=Hφ(X)。

如果是连续型分布，前面说了可以理解为方差为 0 的高斯分布 N(x;Gφ(z),0)，我们可以先考虑常数方差的情况![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDAREoNLia2l2sAibGagJP22YEic4yVx1ibrvvH5F0NGnWf5lQrWE5lrYbkw/640?wx_fmt=png)，计算发现![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDvjpJlNeTvLXfDt9SBzib5GYzUUCMiaEO3rrNEj4au3s8ibZ3P1oOdFlBw/640?wx_fmt=png)是一个常数，然后 σ→0，不过发现结果是无穷大。无穷大原则上是不能计算的，但事实上方差也不需要等于 0，只要足够小，肉眼难以分辨即可。

所以，总的来说我们可以确定互信息 Iφ(X,Z) 与熵 Hφ(X) 只相差一个无关紧要的常数，所以在式 (15 )中，可以将 Hφ(X) 替换为 Iφ(X,Z)：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDquCVFmqRe4TCL1ricbR1JgyJ4Ria0eX8srPicDaQAAibRUoZcSAiceiaweYg/640?wx_fmt=png)

现在我们要最小化 −Iφ(X,Z) ，也就是最大化互信息 Iφ(X,Z)。**直观上这也不难理解，因为这一项是用来防止 mode callopse 的，而如果一旦 mode callopse，那么几乎任意的 z 都生成同一个 x，X,Z 的互信息一定不会大。**

但是将目标从 Hφ(X) 改为 Iφ(X,Z) ，看起来只是形式上的转换，似乎依然还没有解决问题。但很幸运的是，我们已经做过最大化互信息的研究了，方法在[深度学习的互信息：无监督提取特征](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492040&idx=1&sn=f90a6b899e62748c4db489ce06276869&chksm=96ea3e48a19db75e8c07d942a4772bb6c784fac7bcb117da2023186546cfe1876b121a8121cc&scene=21#wechat_redirect)的“互信息本质”一节，也就是说，直接估算互信息已经有解决方案了，读者直接看那篇文章即可，不再重复论述。

**互信息与信息下界**

如果不需要精确估计互信息，那么可以使用 InfoGAN 中的思路，得到互信息的一个下界，然后去优化这个下界。

从互信息定义出发：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDbQeEM0PpNYf8QXINjr5icyzwRJKjWJCiasUoOT1niaicCqqMsU3gTn1hiaA/640?wx_fmt=png)

记 qφ(z|x)=qφ(x|z)q(z)/qφ(x)，这代表精确的后验分布；然后对于任意近似的后验分布 p(z|x)，我们有：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDJAeHdrJ2FFYEiaE5WOfzKVo8y0p7ricXb3CQ8lQYI7yOOQiaAmvuianQLA/640?wx_fmt=png)

也就是说，互信息大于等于∬qφ(x|z)q(z)logp(z|x) 加上一个常数。如果最大化互信息，可以考虑最大化这个下界。由于 p(z|x) 是任意的，可以简单假设![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDhp2Dj8dSTZWu8zt5Xia7rR96rNLgcIdX8eQf7gQVoBUDSCtLxAQ6sdw/640?wx_fmt=png)，其中 E(x) 是一个带参数的编码器，代入计算并省去多余的常数，可以发现相当于在生成器加入一项 loss：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDW10UtFQWRxGueZ7RUeywSuAd2IVvVDiaDrGNVCumvpgPruCCn4C3Ypw/640?wx_fmt=png)

所以，基于*InfoGAN*的信息下界思路，式 (15) 变为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDpwMskWlmV9NcptYAyNRcUibUJQ8kVtFghwdGUFMUM4MmHPS2R0BKlnQ/640?wx_fmt=png)

到这里，我们已经从两个角度完成了 Hφ(X) 的处理，从而完成了整个 GAN 和能量模型的推导。

# MCMC提升效果

回顾开头，我们是从能量分布出发推导出了 GAN 模型，而**能量函数 U(x) 也就是 GAN 模型中的判别器**。既然 U(x) 具有能量函数的含义，那么训练完成后，我们可以利用能量函数的特性做更多有价值的事情，例如引入 MCMC 来提升效果。

**MCMC的简介**

其实对于 MCMC，我只是略懂它的含义，并不懂它的方法和精髓，所谓“简介”，仅仅是对其概念做一些基本的介绍。MCMC 是“马尔科夫链蒙特卡洛方法（Markov Chain Monte Carlo）”，在我的理解里，它大概是这么个东西：我们难以直接从某个给定的分布 q(x) 中采样出样本来，但是我们可以构造如下的随机过程：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDTF23MluUBN0VIIC4QticZeCHf79ZL3vtWDOtdxIX1icfribsbgwwCuiaKA/640?wx_fmt=png)

其中 α 是一个便于实现的随机过程，比如从二元分布、正态分布采样等。这样一来，从某个 x0 出发，得到的序列 {x1,x2,…,xn,…} 是随机的。

如果进一步能证明式 (24) 的静态分布正好是 q(x)，那么就意味着序列 {x1,x2,…,xn,…} 正是从 q(x )中采样出来的一批样本，这样就实现了从 q(x) 中采样了，只不过采样的结果经过了一定的顺序排列。

**Langevin方程**

式 (24) 的一个特例是 Langevin 方程：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDqGQEUMTt8MjfVnSibRsWLy21URL1fmd7ChicrTBurGEZtOL9pCRy6jyQ/640?wx_fmt=png)

它也称为随机微分方程，当 ε→0 时，它的静态分布正好是能量分布：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID2H2hoJuuyraGkzXlsXDexRwpWc2E4yBoXIQlFtHJqOzethLibtNLG0Q/640?wx_fmt=png)

也就是说，给定能量函数 U(x) 后，我们可以通过式 (25) 实现从能量分布中采样，这就是**能量分布的 MCMC 采样**的原始思想。

当然，直接从能量函数和式 (25) 中采样 x 可能不大现实，因为 x 维度（常见的情景下，x 代表图片）过大，可控性难以保证。另一方面，式 (25) 最后一项是高斯噪声，所以只要 ε≠0，那么结果必然是有噪声的，图片真实性也难以保证。

一个有趣的转化是：我们可以不直接考虑 x 的 MCMC 采样，而考虑 z 的采样。因为在前面的模型中，我们最后**既得到了能量函数 Uθ(x)，也得到了生成模型 Gφ(z)**，这意味着 z 的能量函数为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDTHmdxsOoPu1dnkCxk4E5hjCJhmzLLbySuT9WkQXqu4dWibOuD2sIcJQ/640?wx_fmt=png)

有了 z 的能量函数，我们可以通过式 (25) 实现 z 的 MCMC 采样：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID8DgW3Ks3pOLIT4LPn0dm00VPp117B9lnQXSOdSfqRaej03BfvTC9mg/640?wx_fmt=png)

这样刚才说的问题全部都没有了，因为 z 的维度一般比 x 小得多，而且也不用担心 ε≠0 带来噪声，因为 z 本来就是噪声。

**更好的截断技巧**

到这里，如果头脑还没有混乱的读者也许会回过神来： z 的分布不就是标准的正态分布吗？采样起来不是很容易吗？为啥还要折腾一套 MCMC 采样？

理想情况下，z 的能量函数 Uθ(Gφ(z)) 所对应的能量分布：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDmBD8mW2tb75HSwI2icJIZLBWGU0hJdkLteUlzLkVNITibibdt4VzOlSnA/640?wx_fmt=png)

确实应该就是我们原始传递给它的标准正态分布 q(z)。但事实上，理想和现实总有些差距的，当我们用标准正态分布去训练好一个生成模型后，最后能产生真实的样本的噪声往往会更窄一些，**这就需要一些截断技巧，或者说筛选技巧。**

比如，基于flow的生成模型在训练完成后，往往使用“退火”技巧，也就是在生成时将噪声的方差设置小一些，这样能生成一些更稳妥的样本，可以参考[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)。而去年发布的 BigGAN，也讨论了 GAN 中对噪声的截断技巧。

如果我们相信我们的模型，相信能量函数 Uθ(x) 和生成模型 Gφ(z) 都是有价值的，那么我们有理由相信![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDkRgNky3IvOb5tqSib3icQg7RGVMIAIsyMTIciaFblAEMBS6Iib7QibTYSiaA/640?wx_fmt=png)会是一个比标准正态分布更好的 z 的分布（能生成更真实的 x 的 z 的分布，因为它将 Gφ(z) 也纳入了分布的定义中），所以从![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDkRgNky3IvOb5tqSib3icQg7RGVMIAIsyMTIciaFblAEMBS6Iib7QibTYSiaA/640?wx_fmt=png)采样会优于从 q(z) 采样，也就是说 MCMC 采样 (28) 能够提升采样后的生成质量，原论文已经验证了这一点。我们可以将它理解为一种更好的截断技巧。

**更高效的MALA**

采样过程 (28) 其实依然会比较低效，原论文事实上用的是改进版本，称为 MALA（Metropolis-adjusted Langevin algorithm），它在 (28) 的基础上进一步引入了一个筛选过程：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33IDD0hw0cwwva3vGWWVR7POnYHTGfh20XTfHiaIfJf06ia2Rsd4C7RQTlZw/640?wx_fmt=png)

这里：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID8RFzeWJSq58elcYQv1KTZHgnNvugvlvZ1fibvs9ksnrCufGnw0ckfqA/640?wx_fmt=png)

**也就是说以概率 γ 接受**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkPPpypLlZIRgibiaEGibd33ID9IvBDSu9CpHRBP361pgVEaVwUps7PiaXzxZq8cMiaIyar2VpbzA2duUw/640?wx_fmt=png)**，以 1−γ 的概率保持不变。**按照维基百科上的说法，这样的改进能够让采样过程更有机会采样到高概率的样本，这也就意味着能生成更多的真实样本（笔者并不是很懂这一套理论，所以，只能照搬了）。

# 有力的能量视角

又是一篇公式长文，总算把能量分布下的 GAN 的数学推导捋清楚了，GAN 是调和“理论分析”与“实验采样”矛盾的产物。总的来说，笔者觉得整个推导过程还是颇具启发性的，也能让我们明白 GAN 的关键之处和问题所在。

能量视角是一个偏向数学物理的视角，一旦能将机器学习和数学物理联系起来，还将可以很直接地从数学物理处获得启发，甚至使得对应的机器学习不再“黑箱”，这样的视角往往让人陶醉，给人一种有力的感觉。

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



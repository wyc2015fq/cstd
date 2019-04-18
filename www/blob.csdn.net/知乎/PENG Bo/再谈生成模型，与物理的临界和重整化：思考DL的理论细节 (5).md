# 再谈生成模型，与物理的临界和重整化：思考DL的理论细节 (5) - 知乎
# 



首先，感谢各位对前文的讨论。欢迎继续讨论和拍砖。

## **1. Lipschitz 与复读机**

在前文中，一直避免提到 Lipschitz 这个词，因为我认为这个不属于本质。但很多人很喜欢，所以在此说说我的看法。
![](https://pic1.zhimg.com/v2-fb0d9f090ca061f8054351639886471c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1800' height='1135'></svg>)
如图。我们不希望 D 仅在样本上最大（黑色），因为这样没梯度，而且导致 G 变成样本的复读机。如果要求 Lipschitz，就变成绿色，这是否很好？对于训练有好处，因为有梯度。我认为谱归一化和 BN 有类似之处，都是可以提高训练速度，提高稳定性，降低过拟合的技巧。但是，即使有了 Lipschitz。如果训练到最后，G 还是变成样本的复读机。

我认为：**关键，还是 Deep Image Prior。因为 D 和 G 都不可能训练到最后，所以得到的半成品的效果很好。 **

说起 Lipschitz，我倒是想试一些方法。例如，输入带噪音的样本，令不同 L1 噪音度对应不同的 D 的值，等等，然后降噪 = 生成。其实 Flow 系列模型也很有 Lipschitz 精神，可逆的 G 是很好的。

## **2. Deep Image Prior**
![](https://pic1.zhimg.com/v2-5050ff8e1a7558b129bb01adb967786c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1056' height='650'></svg>)
Deep Image Prior，最有道理又微妙的地方，是“训练到一半停下来最好”。这似乎很难量化，我认为，大致是：**温度高是随机，温度降低，降到临界温度 Tc 最好，再降就冻在“死记硬背”了。**所以，现在的训练方法有问题，可能需要加点 MCMC 之类，保证它别冻住。

目前的 GAN 训练过程，D 只走一条路径，也完全不符合 min max 表达式中的条件。

现在这些 GAN 的理论，自娱自乐看看就好，有很多不符合实践的地方。正如 WGAN 理论好看，实际问题很多。如果做真正能用的理论，需要考虑【神经神经网络本身】+【训练过程以及优化器】+【数据的特点】，会很复杂。

## **3. 临界行为**

上文提到临界。临界是非常重要的物理概念。**数据处于临界状态，大脑处于临界状态。它们都拥有长程相关性。**
![](https://pic3.zhimg.com/v2-3563c7f9032ab0eef0c89fe707eb7622_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='538'></svg>)
我认为：**研究对于临界行为的学习，以及保持模型处于临界状态**，这才是正道。

对于处于 Tc 的 Ising 配置的学习（生成），目前有一些研究，但偏于表面。

我想先知道两个问题：
- **如何快速生成大量处于 Tc 的 Ising 配置？**
- **对于拥有长程相关性的数据，它的信息量，目前有何上下界结果？**

再看远一些，我还想知道一些问题，例如：
- 用量子退火，是否可以改进生成模型？
- 是否存在一个 “Neural Field Theory”，让我们可以研究模型的 landscape。我想这个是有的，以后想多一些之后再想。

## **4. 信息是稳定 GAN 的关键？**

看之前的这篇文章：
[PENG Bo：TVGAN 的训练，BigGAN，与 InfoGAN 的最新进展，GAN 是否已接近最终形式？​zhuanlan.zhihu.com![图标](https://pic3.zhimg.com/v2-eefc45af2a3379d08262c09ce14ad102_180x120.jpg)](https://zhuanlan.zhihu.com/p/51294579)
BigGAN 是目前的 SotA。Google 说，可能是 BigGAN 太大了，到后面都会塌掉，他们做过一些分析，也没找到深层的原因，只发现是由于突然出现的梯度冲击。
![](https://pic4.zhimg.com/v2-77fdf9c4d82c0c6db786698339be893f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1256' height='478'></svg>)
Google 试了很多稳定技巧，但提高稳定性都有代价，都会降低模型的性能。最后他们想，可能是因为 D 死记硬背了数据集。对于这么大的模型，这是可以预期的，他们也找到了一些证据。
![](https://pic2.zhimg.com/v2-d9b3bb0bd6240f31b9bb09cde5703781_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1251' height='239'></svg>)
我认为，解决问题的正道，是 InfoGAN 的思想。
![](https://pic1.zhimg.com/v2-a5e99f0d846a6059cfbe742f2819fa4c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='432'></svg>)
在 ProInfoGAN 中提到，在有 InfoGAN 模组后，相当于加入了正则化，会令 GAN 稳定得多。用普通的 GAN loss 就够了，不会塌。
[jonasz/progressive_infogan​github.com![图标](https://pic2.zhimg.com/v2-b9f07dcdc23988cf6a6402ca22a8eeb5_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/jonasz/progressive_infogan)
从低处看，这相当于多任务学习，比学 1 个数字好得多。就像 AlphaGo，学 1 个数字的价值网络，不如多任务的综合网络。

从高处看，**这保证了信息的流动**，这非常奥妙。所以我也有很多想法想试试，例如，**让 D 把所有 z 都尽可能地（而且是有侧重地）还原出来**？说到信息，那个信息瓶颈理论，我从前不喜欢，现在感觉可能也有一点东西。

## **5. 重整化与 Z 的维数**

重整化是非常重要的物理概念。可以最简单理解为，世界在不同尺度的表观不一样。

上文的 ProInfoGAN 在不同尺度引入不同 Z，**这是非常典型的重整化的思想**。

又例如，很多 ML 的人常常认为“数据流形”的维数很低（manifold hypothesis）。这是错误的。数学家也不懂重整化，所以他们一起算了很多无用的事情。
![](https://pic1.zhimg.com/v2-c2a55e769ffc36a02efb039b6ffa2034_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='265' height='194'></svg>)
对于一只小狗，每根狗毛都有很多维的 z。如果你说"我们真正在意的 z 的维数低"，那么恭喜你，你在用重整化的观点而不自知。

我的观点：**我们需要很高维度的 Z，然后，把其中某些维度逐渐藏起来**。

这对于生成模型有何启示？其实 Flow 系列已经面对过类似的问题，并提出了一些方法。我也有一些想法打算试试。
![](https://pic3.zhimg.com/v2-b6261a33768589e07e98945c1821a042_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='580' height='283'></svg>)
## **6. 后记**

本文没有出现公式。懂数学的朋友都知道，有时候大白话说得更清楚。该用公式的时候用公式，不该用公式的时候不用公式。

最后出现一点公式，现在的 SotA，BigGAN 的 loss，就是 TV 的思想，我在上一篇（[TVGAN 的训练，BigGAN，与 InfoGAN 的最新进展，GAN 是否已接近最终形式？](https://zhuanlan.zhihu.com/p/51294579)）说了，这里再重复，以免还有人反复说这个。
![](https://pic1.zhimg.com/v2-28b94106d5c31b2c35bd0ed8584ed73c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='71'></svg>)
这里有个地方值得思考：**效果好的 GAN，D 和 G 的 loss 不一样，因此正确的观点是怎样。**

最后一张图。**我们最终是希望生成模型有创造力。**现在主要关注 Interpolation，以后还需要 Extrapolation 和 Invention。生成模型的水有多深，就在这里。
![](https://pic2.zhimg.com/v2-90d0e42e8d33560aba0c6bfaa96c54cd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='528'></svg>)




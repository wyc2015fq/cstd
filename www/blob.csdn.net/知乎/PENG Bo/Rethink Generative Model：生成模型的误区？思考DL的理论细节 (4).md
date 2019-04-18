# Rethink Generative Model：生成模型的误区？思考DL的理论细节 (4) - 知乎
# 



图像的生成模型，或者说图像模型（与语言模型相对应），是图像无监督学习的核心（可以以此解决一切逆问题，如超分辨率，降噪，inpaint，等等），也是图像有监督学习的重要基础。

本系列的最终目的，**在于希望提出一种稳定，可解释，每一步都具有【数学和物理基础】的生成模型训练体系**（包括网络架构，损失设计，训练算法，评估方法，等等）。

在此过程中，我们将看到，有些流传甚广的，著名的说法，实际存在漏洞和误区。

本系列的前后文见我的专栏。欢迎关注，欢迎讨论：
[技术备忘录​zhuanlan.zhihu.com![图标](https://pic3.zhimg.com/v2-e4867a2e7a0afdac40fdb97191a54ed6_ipico.jpg)](https://zhuanlan.zhihu.com/mathNote)
本文，对于前文中一些可能难以理解的观点，作进一步的说明。




## **1. 生成模型的漏洞**

考虑样本集 ![X=\{x_i\}，x_i \sim p_x + \epsilon](https://www.zhihu.com/equation?tex=X%3D%5C%7Bx_i%5C%7D%EF%BC%8Cx_i+%5Csim+p_x+%2B+%5Cepsilon) 。这里 ![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon) 是噪音。这就是真实的模型。下文我们先暂时忽略降噪的问题。

我在前文曾说过，对于 ![p_x](https://www.zhihu.com/equation?tex=p_x) 的估计是核心问题，而且，对于 ![p_x](https://www.zhihu.com/equation?tex=p_x) 的估计，不见得需要 GAN 这种拐弯抹角的方法。

例如，一种最简单的思路，是先还原：

![\delta_X(x) = \sum_i \delta(x-x_i)](https://www.zhihu.com/equation?tex=%5Cdelta_X%28x%29+%3D+%5Csum_i+%5Cdelta%28x-x_i%29)

然后找到合理的插值方法。这同样可以通过 Deep Image Prior（下文简称 DIP）实现。

**Deep image prior 不是秘密，是公开的论文。大家都知道，大家都不重视，没有人研究，都去研究相对不重要的事情，很令人失望。**

而且，很多人没有深究如何还原 ![\delta_X](https://www.zhihu.com/equation?tex=%5Cdelta_X) ，直接就试图去猜 ![p_x](https://www.zhihu.com/equation?tex=p_x)。例如，目前的 GAN 甚至完全不能保证还原 ![\delta_X](https://www.zhihu.com/equation?tex=%5Cdelta_X)，这是很大的漏洞，但是没有人去考虑。

因此我在上一篇中才会做一个弱智小实验，看看硬性还原 ![\delta_X](https://www.zhihu.com/equation?tex=%5Cdelta_X) 会怎样：
[PENG Bo：DGN：生成模型，可以有多简单？思考DL的理论细节 (3)​zhuanlan.zhihu.com![图标](https://pic3.zhimg.com/v2-d6037f939fdacaf8e3b5cd50acb45c3e_180x120.jpg)](https://zhuanlan.zhihu.com/p/50952156)
这种实验我会继续做。找到最佳的 G 架构，以及怎样用最小的 z 维数还原出 X，等等。

大家可能问，AE 系列就是还原 ![\delta_X](https://www.zhihu.com/equation?tex=%5Cdelta_X)，但是 AE 系列生成的图片很模糊啊？
- 通常的解释是因为 AE 用了 MLE，这在一定条件下是对的。
- 我认为，根本的解释是，常见 AE 用的网络太小，架构不好，所以不能完美还原。
- 在这种限制下，Perceptual loss 比 MLE 好。但如果网络够强，其实 MLE 也不见得不行。
- 当然，最终还是需要我称为 "Conceptual loss" 的事情（我也说不清这是啥）。
- 其实有了 loss 就相当于已经很理解 ![p_x](https://www.zhihu.com/equation?tex=p_x)，所以这个问题是很微妙的。

在弱智小实验中，我们看到几百张图片配合 MLE，是可以记住的。但在这种网络规模和简单的架构下，图片很多就必然会越来越模糊。

所以我们需要想清楚最佳的 G 是怎样。




## **2. 模式坍塌的细节**

模式坍塌是困扰 GAN 的常见问题。
![](https://pic2.zhimg.com/v2-7eca444f9cc6ae4e731bbe02a73aa431_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1600' height='400'></svg>)
应该是 Goodfellow 说过这个，他说，对于完美的 D，就最终会模式坍塌了（显然）。

所以很多人觉得，D 不能太完美。

在我看来，这个观点有问题。

首先，如果 G 固定输出和某个 ![x_i](https://www.zhihu.com/equation?tex=x_i) 一模一样的图片，那么，没错，现在的 GAN 中“分辨样本真实度的 D”，似乎就很难工作了（但是，我必须说，实际的 D 是在不断变动的，所以问题其实很复杂）。

**完美的 D，是分辨 G 的输出的分布，这种 D 是不会出错的。我们的目标本来就是拟合分布。**

如何输入分布？一种方法，是输入多个样本。这种 D，很多人想过了，例如这里面就很多：
[https://arxiv.org/pdf/1606.03498.pdf​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1606.03498.pdf)
但这可能还不是最好的做法。我认为，最好的做法需要从源头开始，从 G 的架构就想清楚。

例如，一个“可解释”的 G，有助于我们实现这个目标。今天有篇文章挺好：
[https://zhuanlan.zhihu.com/p/51044703​zhuanlan.zhihu.com![图标](https://pic2.zhimg.com/v2-be344f95715a6fb4b6bbe2a65037dea1_180x120.jpg)](https://zhuanlan.zhihu.com/p/51044703)
其实，如果 G 坍塌了，真的就活不过来了吗？不一定。
- 首先，我们的训练过程，是 SGD，不是 GD。
- 所以，所有用积分观点看 GAN 的看法，都有漏洞。其实我们实际用的有步长的 GD 也不能用积分观点看。

其次，正如 WGAN 的文章，他认为两个 delta 函数之间的很多散度没法训练，所以很多散度不好。这看法也有点问题。
- 因为，网络的输出的分布，是可以突然冒出来一个头的。
- 例如，由于网络是高度非线性的，可能样本 A 的梯度，让另一个本来没有梯度的样本 B 也享受到梯度。
- 例如，如果 G 一直生成 ![x_5](https://www.zhihu.com/equation?tex=x_5)，我们换一个只认 ![x_2](https://www.zhihu.com/equation?tex=x_2) 的 D，那么 G 也得变（注意 D 是一直在变的，所以完全可以这样变）。

其实，如果 D 对于所有 ![x_i](https://www.zhihu.com/equation?tex=x_i) 一视同仁，那么它最终会引导 G 跑出来（类似负采样）。但这里有一些细节问题，和训练的具体方法有关系。所以优化器对于现在的 GAN 很重要。

## **写到这里我发现确实有个细节。**

## **现在的深度学习框架，采样样本，都是每个 epoch 打乱一次，全部走一遍。**

## **这个采样方法有问题。**

## **例如，这样就不可能出现连续采样同一样本的情况。**

## **这个非常微妙，对于普通问题不影响。**

## **但是，现在看来，说不定，这会和 GAN 的坍塌有关，稍后实验一下。**

总之，我希望最终能有一个对于优化器不敏感的生成模型。

最后，再考虑“只看单个样本的真实度的 D”。

那么，下面这几种 G 都可以完美满足 D 的需求：
- “固定生成 ![x_4](https://www.zhihu.com/equation?tex=x_4) 的 G"
- “会根据不同 ![z](https://www.zhihu.com/equation?tex=z) 生成 ![x_1](https://www.zhihu.com/equation?tex=x_1) 或 ![x_6](https://www.zhihu.com/equation?tex=x_6) 的 G"
- “会根据不同 ![z](https://www.zhihu.com/equation?tex=z) 生成不同 ![x_i](https://www.zhihu.com/equation?tex=x_i) 的 G"
- “会根据不同 ![z](https://www.zhihu.com/equation?tex=z) 生成不同 ![x_i](https://www.zhihu.com/equation?tex=x_i) 以及其它 ![p_x](https://www.zhihu.com/equation?tex=p_x)中的样本的 G"

并不能说，一定会坍塌到第 1 个。如果经常坍塌到第 1 个，说明整个系统有问题。

看一个几何图像，来自我 1 年多前的文章，这里的用法有点区别：
[PENG Bo：GAN的几何图景：样本空间的Morse流，与鉴别网络D为何不可能真正鉴别真假​zhuanlan.zhihu.com![图标](https://pic1.zhimg.com/v2-2dfc8497d0d5481ab205e91dda648394_180x120.jpg)](https://zhuanlan.zhihu.com/p/28597079)![](https://pic1.zhimg.com/v2-dc306c721513f82af6a273dd5adccc98_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='380'></svg>)
这里用这个图像说明另一个问题，这里想说的是，我们最终希望，输入不同的 z，可以流到不同的 ![x_i](https://www.zhihu.com/equation?tex=x_i) 以及其它 ![p_x](https://www.zhihu.com/equation?tex=p_x)中的样本。这样就满足了 D = 1，也可以得到很好的样本。

所以这对应一种思路，就是先输入噪音图像（噪音的 D 应该 = 0），然后降噪。或者不同噪音对应不同的 D 的值，等等。未来我也试试。

总之，生成模型确实可以有很多思路。

## **后续继续。欢迎大家讨论，欢迎大家关注。**



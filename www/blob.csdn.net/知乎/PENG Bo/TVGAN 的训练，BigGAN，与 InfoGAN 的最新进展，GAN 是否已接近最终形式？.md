# TVGAN 的训练，BigGAN，与 InfoGAN 的最新进展，GAN 是否已接近最终形式？ - 知乎
# 



标题图来自下文的 ProInfoGAN，在 CelebA-HQ 上无监督发现高质量 disentangled 编码。

我在几天前介绍了一个很简单的“TVGAN”，很奇怪的是，为什么这么简单的事情，之前没人明确去用。

今天细看 BigGAN 才发现，原来 Google 的这篇 SotA 也是在用非常接近的 LOSS。让我们看看为什么。

## **1. TVGAN 的训练**

首先，回顾 TVGAN 公式：

![\min_{G} \max_{D(x) \in [-1,1]} E_{x \sim p_x(x)}(D(x)) - E_{z \sim p_z(z)}(D(G(z)))](https://www.zhihu.com/equation?tex=%5Cmin_%7BG%7D+%5Cmax_%7BD%28x%29+%5Cin+%5B-1%2C1%5D%7D+E_%7Bx+%5Csim+p_x%28x%29%7D%28D%28x%29%29+-+E_%7Bz+%5Csim+p_z%28z%29%7D%28D%28G%28z%29%29%29)
[PENG Bo：TVGAN：一种简单且有效的新GAN (以及WGAN论文的问题) 思考DL的理论细节 (2)​zhuanlan.zhihu.com![图标](https://pic1.zhimg.com/v2-2de9aae1c4cc4aaec3845210c5211650_180x120.jpg)](https://zhuanlan.zhihu.com/p/50806187)
但是，如果实际去用，会发现一个问题：

> **如果用 tanh 硬性把 D 的输出映射到 [-1, 1]，那么 G 的梯度就很容易消失（显然，就像 MSE 配合 CE 会梯度消失）。**

估计这就是为什么之前大家都不用这个方法。

那么，简单的想法是，去掉 [-1,1] 限制，然后加个正则项希望 D 的输出接近 0，例如这样：

![\min_{G} \max_{D} E_{x \sim p_x(x)}(D(x)) - E_{z \sim p_z(z)}(D(G(z))) - \lambda \cdot E_{x \sim {all}}(|D(x)|)](https://www.zhihu.com/equation?tex=%5Cmin_%7BG%7D+%5Cmax_%7BD%7D+E_%7Bx+%5Csim+p_x%28x%29%7D%28D%28x%29%29+-+E_%7Bz+%5Csim+p_z%28z%29%7D%28D%28G%28z%29%29%29+-+%5Clambda+%5Ccdot+E_%7Bx+%5Csim+%7Ball%7D%7D%28%7CD%28x%29%7C%29)

但感觉这样丑。

于是，我想了想，想到一个怪招，见下图：
![](https://pic4.zhimg.com/v2-3296e39e9529a175f84481ba3cfb1eab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='826' height='167'></svg>)
这是当时试的一些方法，没有注释掉的那个，就是当时想到的怪招：
- **对于 D(x) > 1 的真样本，直接就把梯度归零（因为已经足够了）。**
- **同理，对于 D(x) < -1 的假样本，也把梯度归零（因为已经足够了）。**
- 不用添加任何参数，大致实现了我们的要求，真好。
- 这还有一个有趣的地方，就是现在如果 D 太完美，它会自动停下来等 G。

如果写到式子里面，就是：

![\max_D E_{x \sim p_x(x)}(\min(1,\,D(x))) - E_{z \sim p_z(z)}(\max(-1,D(G(z))))](https://www.zhihu.com/equation?tex=%5Cmax_D+E_%7Bx+%5Csim+p_x%28x%29%7D%28%5Cmin%281%2C%5C%2CD%28x%29%29%29+-+E_%7Bz+%5Csim+p_z%28z%29%7D%28%5Cmax%28-1%2CD%28G%28z%29%29%29%29)

另外，代码中第一个 ggg 是另一种梯度方案，好像也还可以。第三个 ggg 就是普通的 GAN（显然）。

**其实，许多不同的 GAN 只不过是梯度变来变去。我们完全可以去掉 D 最后的那个非线性，直接改梯度。**

值得注意的是，G 不能用类似的思路，如图：
![](https://pic3.zhimg.com/v2-5d0dd2a04ca245019f54cc5271ede4ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='786' height='151'></svg>)
**由于神秘的原因，G 还是要用原来的 loss，即，固定的梯度。否则似乎 G 会相对 D 而言太弱，导致没图像。**

其实原始 GAN 中的 G loss 也是和 D loss 有区别。




## **2. BigGAN 的 LOSS：巧合**

今天想再看看最新进展，于是细读了 BigGAN 的论文：
[[1809.11096] Large Scale GAN Training for High Fidelity Natural Image Synthesis​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1809.11096)![](https://pic2.zhimg.com/v2-9b95efd81f18d9a02c0f39cb3e3ad221_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1171' height='287'></svg>)
发现 BigGAN 用这个所谓 Hinge loss（来自 SA-GAN）:
![](https://pic1.zhimg.com/v2-a0393aa1c15bce833c76a93d16b22154_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1151' height='114'></svg>)
## **易见，这里 D 和 G 的 loss 和“TVGAN”的方法一模一样，真是巧合，哈哈。**

**当然，我之前也说过，GAN 的这个 loss 没那么重要，所以也没什么可夸的，不过这可以说明这个方法是有道理的。**

BigGAN 还有谱归一化，我个人是把谱归一化看成是通用技巧，和 BN 一个性质。

BigGAN 还做了很多有趣的选择。Google 完全不在意模型是否“简洁优美”（所以放心地给 D 和 G 用不同学习速率，D-D-G 训练周期，等等等等），只有效果好才是硬道理。

BigGAN 效果好，关键还是因为 Attention 太重要了，和 BERT 的情况一样。有了 Attention 之后，几乎是暴力解决一切。

Google 还明确地说，可能是 BigGAN 太大了，到后面都会塌掉，他们也没搞清楚深层的原因。所以现在 report 的是塌掉之前的结果。
![](https://pic4.zhimg.com/v2-458f857b16d175f0bb453fb61781161f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1232' height='163'></svg>)
我没有 TPU 集群，只能猜。我猜，BigGAN 缺少的环节，可以由 InfoGAN 模块补全。让我们继续看下文。




## **3. ProInfoGAN 与感想**

这是搜索到的一篇少为人知的最新结果，效果非常好。我本来想做类似的事情，现在看来已经没有什么可做的了。
[jonasz/progressive_infogan​github.com![图标](https://pic2.zhimg.com/v2-b9f07dcdc23988cf6a6402ca22a8eeb5_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/jonasz/progressive_infogan)
**无监督发现高质量 disentangled 编码！**这是无监督学习的圣杯之一。

请大家欣赏视频（Youtube，需要翻墙），效果非常好：
[https://youtu.be/U2okTa0JGZg​youtu.be](https://link.zhihu.com/?target=https%3A//youtu.be/U2okTa0JGZg)
其实，就是 NVidia 那个逐步 GAN（ProGAN），和 InfoGAN 的结合：
- **它发现，在有 InfoGAN 模组后，相当于加入了正则化，会令 GAN 稳定得多。用普通的 GAN loss 就够了，不会塌。**
- **它要求不同编码对应不同的尺度（例如，改眼睛颜色，就肯定在小图像中看不出来），还有其它一些技巧，实现了非常高质量的编码分离。**
- **具体技巧，看它的论文草稿吧。**

效果，左右转：
![](https://pic4.zhimg.com/v2-611e9ffb41f72873375692b23b065073_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='2560' height='1536'></svg>)
嘴张合：
![](https://pic3.zhimg.com/v2-3ea4b9e3141dfbc04e14ecc4ea81e1c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='2560' height='1536'></svg>)
## 大家还是看 Youtube 视频吧，效果明显得多，很精彩。

## 完全无监督，拆出来 80 个带意义的维度，而且质量都很高。
![](https://pic4.zhimg.com/v2-86be8c2961f2005e710de3732107cd37_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='923' height='851'></svg>)
## **4. 感想**

## GAN 是否已接近最终形式了？可以想象 BigInfoGAN 会很强。

## 再喂多些数据，再增大模型，真的 Sky is the limit。

## 严格说来，它还是啥都不懂，但确实很强。




# 深度学习中的Lipschitz约束：泛化与生成模型 - Paper weekly - CSDN博客


2018年10月16日 13:14:22[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：168


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨苏剑林
单位丨广州火焰信息科技有限公司
研究方向丨NLP，神经网络
个人主页丨kexue.fm

去年写过一篇 WGAN-GP 的入门读物[互怼的艺术：从零直达WGAN-GP](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247484880&idx=1&sn=4b2e976cc715c9fe2d022ff6923879a8&chksm=96e9da50a19e5346307b54f5ce172e355ccaba890aa157ce50fda68eeaccba6ea05425f6ad76&scene=21#wechat_redirect)，提到通过梯度惩罚来为 WGAN 的判别器增加 Lipschitz 约束（下面简称“L 约束”）。前几天遐想时再次想到了 WGAN，总觉得 WGAN 的梯度惩罚不够优雅，后来也听说 WGAN 在条件生成时很难搞（因为不同类的随机插值就开始乱了），所以就想琢磨一下能不能搞出个新的方案来给判别器增加L约束。

闭门造车想了几天，然后发现想出来的东西别人都已经做了，果然是只有你想不到，没有别人做不到呀。主要包含在这两篇论文中：*Spectral Norm Regularization for Improving the Generalizability of Deep Learning*[1]和*Spectral Normalization for Generative Adversarial Networks*[2]。

所以这篇文章就按照自己的理解思路，对L约束相关的内容进行简单的介绍。**注意本文的主题是 L 约束，并不只是 WGAN。它可以用在生成模型中，也可以用在一般的监督学习中。**

# L约束与泛化

**扰动敏感**

记输入为 x，输出为 y，模型为 f，模型参数为 w，记为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lVIiaXkTxjy7bdqHiampTjMpBHmWdW9vWb0yr81sOYoiawrAOG1oMjFOBA/640)

很多时候，我们希望得到一个“稳健”的模型。何为稳健？一般来说有两种含义，**一是对于参数扰动的稳定性**，比如模型变成了 fw+Δw(x) 后是否还能达到相近的效果？如果在动力学系统中，还要考虑模型最终是否能恢复到 fw(x)；**二是对于输入扰动的稳定性**，比如输入从 x 变成了 x+Δx 后，fw(x+Δx) 是否能给出相近的预测结果。

读者或许已经听说过深度学习模型存在“对抗攻击样本”，比如图片只改变一个像素就给出完全不一样的分类结果，这就是模型对输入过于敏感的案例。

**L约束**

所以，大多数时候我们都希望模型对输入扰动是不敏感的，这通常能提高模型的泛化性能。也就是说，我们希望 ||x1−x2|| 很小时：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lqnbyCibhIsEBPygea7h7Mt1iaknSr2yY9Cq2aNgBxGgVicGHyIcQUJWgA/640)

也尽可能地小。当然，“尽可能”究竟是怎样，谁也说不准。于是 Lipschitz 提出了一个更具体的约束，那就是存在某个常数 C（它只与参数有关，与输入无关），使得下式恒成立：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91licMEtdiarfCj6bLVibYze8qRlzRhMsKgDb2SOia8tJAHfkhzV1icQajTQEQ/640)

也就是说，希望整个模型被一个线性函数“控制”住。**这便是 L 约束了。**

**换言之，在这里我们认为满足 L 约束的模型才是一个好模型。**并且对于具体的模型，我们希望估算出 C(w) 的表达式，并且希望 C(w) 越小越好，越小意味着它对输入扰动越不敏感，泛化性越好。

**神经网络**

在这里我们对具体的神经网络进行分析，以观察神经网络在什么时候会满足 L 约束。

简单而言，我们考虑单层的全连接 f(Wx+b)，这里的 f 是激活函数，而 W,b 则是参数矩阵/向量，这时候 (3) 变为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lMPauFuxPpBEjZVp93RwWJiaTdiaXyicv3IxgwZub7gWopiaibjUBT0Z8x1w/640)

让 x1,x2 充分接近，那么就可以将左边用一阶项近似，得到：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lWd1FnmVukAayCeH3FIXiaPBJQsriaibzjwF6LOH5qZsxBYeaqSgjV9ymA/640)

显然，要希望左边不超过右边，**∂f/∂x 这一项（每个元素）的绝对值必须不超过某个常数。这就要求我们要使用“导数有上下界”的激活函数，不过我们目前常用的激活函数，比如sigmoid、tanh、relu等，都满足这个条件。**假定激活函数的梯度已经有界，尤其是我们常用的 relu 激活函数来说这个界还是 1，因此 ∂f/∂x 这一项只带来一个常数，我们暂时忽略它，剩下来我们只需要考虑 ||W(x1−x2)||。

多层的神经网络可以逐步递归分析，从而最终还是单层的神经网络问题，而 CNN、RNN 等结构本质上还是特殊的全连接，所以照样可以用全连接的结果。因此，对于神经网络来说，问题变成了：**如果下式恒成立，那么 C 的值可以是多少？**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lUN1A0yw1YnY5ctTaicSaiby7zmUHPZvvticd09nribt2eEBh287EZAIp2w/640)

找出 C 的表达式后，我们就可以希望 C 尽可能小，从而给参数带来一个正则化项![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lGx7z2BWu8Ea4OwsyksoibVkGRQgGxuDSW05XddiahMC9wCXn2oictWzvQ/640)。

# 矩阵范数

**定义**

其实到这里，我们已经将问题转化为了一个矩阵范数问题（矩阵范数的作用相当于向量的模长），它定义为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lZ9na5XR8w7xu3FkdWlwXuXse2zyibXvyWcwpic4JMv3UkbQ8UJoicDkVA/640)

如果 W 是一个方阵，那么该范数又称为“谱范数”、“谱半径”等，在本文中就算它不是方阵我们也叫它“谱范数（Spectral Norm）”好了。注意 ||Wx|| 和 ||x|| 都是指向量的范数，就是普通的向量模长。而左边的矩阵的范数我们本来没有明确定义的，但通过右边的向量模型的极限定义出来的，所以这类矩阵范数称为“由向量范数诱导出来的矩阵范数”。

好了，文绉绉的概念就不多说了，有了向量范数的概念之后，我们就有：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lfg5f28nFP7pP6CylrEiaTdZbXPyzvKOl6Tyj8A5jzES3Lsc1ia0ia99cA/640)

呃，其实也没做啥，就换了个记号而已，||W||2 等于多少我们还是没有搞出来。

**Frobenius范数**

其实谱范数||W||2的准确概念和计算方法还是要用到比较多的线性代数的概念，我们暂时不研究它，而是先研究一个更加简单的范数：**Frobenius 范数，简称 F 范数。**

这名字让人看着慌，其实定义特别简单，它就是：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lPialmsibUJdmcc4SRhmWT3O806Jsgknniboj3z0aHTuhqyr4AzodGchxw/640)

说白了，它就是直接把矩阵当成一个向量，然后求向量的欧氏模长。

简单通过柯西不等式，我们就能证明：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lhBB4D0xO7tia7oqa8oFpjm9aqt5o7VoZaCSN7YtbIw8ib7BTUt63Sldg/640)

很明显||W||F 提供了||W||2 的一个上界，也就是说，你可以理解为||W||2是式 (6) 中最准确的 C（所有满足式 (6) 的 C 中最小的那个），但如果你不大关心精准度，你直接可以取 C=||W||F，也能使得 (6) 成立，毕竟||W||F容易计算。

**l2正则项**

前面已经说过，为了使神经网络尽可能好地满足L约束，我们应当希望 C=||W||2尽可能小，我们可以把 C2 作为一个正则项加入到损失函数中。当然，我们还没有算出谱范数||W||2，但我们算出了一个更大的上界||W||F，那就先用着它吧，即 loss 为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lCChic9oJ1yxP8jzI9ZkHW2K8UfFWPFBZHX45pIDhibmE3MIia87wMSGLA/640)

其中第一部分是指模型原来的 loss。我们再来回顾一下||W||F的表达式，我们发现加入的正则项是：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91licg5nrkXYYrXsLRkYdG1LWprycBgELvuHVWUaly154JkNzYGsoJ8Vvg/640)

这不就是 l2 正则化吗？

终于，捣鼓了一番，我们得到了一点回报：**我们揭示了 l2 正则化（也称为 weight decay）与 L 约束的联系，表明 l2 正则化能使得模型更好地满足 L 约束，从而降低模型对输入扰动的敏感性，增强模型的泛化性能。**

# 谱范数

**主特征根**

这部分我们来正式面对谱范数||W||2，这是线性代数的内容，比较理论化。

事实上，**谱范数****||****W****||****2****等于**![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lSWt5aaWXf3l4df4edib41WO50VngeJBFPyBtP4DgFwibwMX348iaX3W8w/640)**的最大特征根（主特征根）的平方根**，如果 W是方阵，那么||W||2等于 W 的最大的特征根绝对值。

对于感兴趣理论证明的读者，这里提供一下证明的大概思路。根据定义 (7) 我们有：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lVQufL9y7iareTwJtY9nvA1p1AYE35mKeA12CztKUzOzxMnHDibyA2ia3g/640)

假设![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lSWt5aaWXf3l4df4edib41WO50VngeJBFPyBtP4DgFwibwMX348iaX3W8w/640)对角化为diag(λ1,…,λn)，即![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91loU0cwRJTSBwQEsAclYRQga3UECKJk8k6RWVsACP7Dm2ibOjR9dwgteA/640)，其中 λi 都是它的特征根，而且非负，而 U 是正交矩阵，由于正交矩阵与单位向量的积还是单位向量，那么：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lFAx7B0M4AM0t4KU11kibHcBliavQSMSEbwcfgfdCiakPcsK8T9IgL3Clw/640)

所以![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91llBffydPQ15eTQbJiafu2yI1lQlDlib7ffJ8SqlWyTWQHnejkJ4bgeKicw/640)等于![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lSWt5aaWXf3l4df4edib41WO50VngeJBFPyBtP4DgFwibwMX348iaX3W8w/640)的最大特征根。

**幂迭代**

也许有读者开始不耐烦了：鬼愿意知道你是不是等于特征根呀，我关心的是怎么算这个鬼范数！

事实上，前面的内容虽然看起来茫然，但却是求 ‖W‖2 的基础。前一节告诉我们![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91llBffydPQ15eTQbJiafu2yI1lQlDlib7ffJ8SqlWyTWQHnejkJ4bgeKicw/640)就是![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lSWt5aaWXf3l4df4edib41WO50VngeJBFPyBtP4DgFwibwMX348iaX3W8w/640)的最大特征根，所以问题变成了求![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lSWt5aaWXf3l4df4edib41WO50VngeJBFPyBtP4DgFwibwMX348iaX3W8w/640)的最大特征根，这可以通过**“幂迭代”法**[3]来解决。

所谓“幂迭代”，就是通过下面的迭代格式：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lfPCcSCZkwQMeIvVwucqej6949ib6hZKkhfkXibPXUyXmSRZlEjjlPEIg/640)

迭代若干次后，最后通过：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lMEwdIjq69SjnkqoicKr1rSFKMgSe0JictKDMAZMO2IbyzqrtSVBXiahCA/640)

得到范数（也就是得到最大的特征根的近似值）。也可以等价改写为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lPBNzqGTicrNS6T9Osb5sfVTpO1SarfHia4skN1K4t9H2dbxhV7p2WrTQ/640)

这样，初始化 u,v 后（可以用全 1 向量初始化），就可以迭代若干次得到 u,v，然后代入![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91l0t9Fx2fBcqDe1p60kunTVeg4Rngg1jSwiaQN3VqHSDL3ZXJnmkoSdGw/640)算得 ‖W‖2 的近似值。

对证明感兴趣的读者，这里照样提供一个简单的证明表明为什么这样的迭代会有效。

记![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lDPqS6WK4JrYxicrnp9DCmIPlno4ibebntzmtiaU36c9Av893sxY27eFNg/640)，初始化为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lohicKCvdnibkkicoC7Urw7nPTCote3sEEiaCPvlYlw0g7icXnkEv6r56yEg/640)，同样假设 A 可对角化，并且假设 A 的各个特征根 λ1,…,λn 中，最大的特征根严格大于其余的特征根（不满足这个条件意味着最大的特征根是重根，讨论起来有点复杂，需要请读者查找专业证明，这里仅仅抛砖引玉。

当然，从数值计算的角度，几乎没有两个人是完全相等的，因此可以认为重根的情况在实验中不会出现。），那么 A 的各个特征向量 η1,…,ηn 构成完备的基底，所以我们可以设：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lfb3WiaXmicYANI8BjZARHdbRempj9ZZnYZhbqcoAVj2AxLw52Xibj1WicA/640)

每次的迭代是 Au/‖Au‖，其中分母只改变模长，我们留到最后再执行，只看 A 的重复作用：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91luiaLvJSfLOwITgJIAguSibq8DJvN8yU88E30WejYl1RLb7dm5MHibQesA/640)

注意对于特征向量有 Aη=λη，从而：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lcKTib9FtnE0jEXc7CZrSviaNHKYNNNPX3mvZeTZjOqQ0Jgp68Ws2MlIg/640)

不失一般性设 λ1 为最大的特征值，那么：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lNfffGXpS0IdpQ8Wj6GwVRKgr618hVYnu8VNPzG2MaIPbdOn6FgFF6Q/640)

根据假设 λ2/λ1,…,λn/λ1 都小于 1，所以 r→∞ 时它们都趋于零，或者说当 r 足够大时它们可以忽略，那么就有：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lvdOA9Vs7U7Tuicl3ictU3alQtNibBFTvtdBYxbk08HibaVpAoFa1ic8h3Ew/640)

先不管模长，这个结果表明当 r 足够大时，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lBL2fYDGTUNLhBibboj8rmxIicGR2IyZ1yFG0QibI7chRUSeQicw6G5Ccsg/640)提供了最大的特征根对应的特征向量的近似方向，其实每一步的归一化只是为了防止溢出而已。这样一来![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lic05ickbT9OSwUVT97WyqfjafYloJkS66TZU5ZCLeXxcpME860v9P7hg/640)就是对应的单位特征向量，即：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lfVOsRVNzMw1jiaMwHbxfFqjKicNbKuHXAWUlkiaUcSIyObvqkmR22mlaw/640)

因此：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lMtUThpvKsq7Qw1R1FfNThkVj0urxmJYg8QIE78yqYS8gIiaaXcKVaOA/640)

这就求出了谱范数的平方。

**谱正则化**

前面我们已经表明了 Frobenius 范数与 l2 正则化的关系，而我们已经说明了 Frobenius 范数是一个更强（更粗糙）的条件，更准确的范数应该是谱范数。虽然谱范数没有 Frobenius 范数那么容易计算，但依然可以通过式 (15) 迭代几步来做近似。

所以，我们可以提出**“谱正则化（Spectral Norm Regularization）”**的概念，即把谱范数的平方作为额外的正则项，取代简单的 l2 正则项。即式 (11) 变为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lUY7A9EJs13ZZ0icubibhVeDJKujZeE3PNuoe2kaicjfP40FyXfEiaeyqQA/640)

*Spectral Norm Regularization for Improving the Generalizability of Deep Learning*[1]一文已经做了多个实验，表明“谱正则化”在多个任务上都能提升模型性能。

在 Keras 中，可以通过下述代码计算谱范数：

```python
def
```
```python
spectral_norm
```
```python
(w, r=
```
```python
5
```
```python
)
```
```python
:
```

```python
w_shape = K.int_shape(w)
```

```python
in_dim = np.prod(w_shape[:
```
```python
-1
```
```python
]).astype(int)
```

```python
out_dim = w_shape[
```
```python
-1
```
```python
]
```

```python
w = K.reshape(w, (in_dim, out_dim))
```

```python
u = K.ones((
```
```python
1
```
```python
, in_dim))
```

```python
for
```
```python
i
```
```python
in
```
```python
range(r):
```

```python
v = K.l2_normalize(K.dot(u, w))
```

```python
u = K.l2_normalize(K.dot(v, K.transpose(w)))
```

```python
return
```
```python
K.sum(K.dot(K.dot(u, w), K.transpose(v)))
```

# 生成模型

**WGAN**

如果说在普通的监督训练模型中，L 约束只是起到了“锦上添花”的作用，那么在 WGAN 的判别器中，L 约束就是必不可少的关键一步了。因为 WGAN 的判别器的优化目标是：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lHKNPx9rNrdKLTquHYRQVFibHNAoR2ohcLDT5eozeJFmsVVKsxUnhtaQ/640)

这里的 Pr,Pg 分别是真实分布和生成分布，|f|L=1 指的就是要满足特定的 L 约束 |f(x1)−f(x2)|≤‖x1−x2‖（那个 C=1）。所以上述目标的意思是，在所有满足这个L约束的函数中，挑出使得![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91llTicStZBFiasibrXkYMG4HhHrEeXEabQQN93xibVD9ucwuYIC0bV69TL0A/640)最大的那个 f，就是最理想的判别器。写成 loss 的形式就是：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91ljOw2icd1uFg8MxadMcc1HPldDN0CK4NaRicsJib0I00qYcrE1yjeCOgaw/640)

**梯度惩罚**

目前比较有效的一种方案就是梯度惩罚，即 ‖f′(x)‖=1 是 |f|L=1 的一个充分条件，那么我把这一项加入到判别器的 loss 中作为惩罚项，即：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lEsbQq9kp9LvfQtA4PsZXfibbia76vsfAvpl7v8Btgxmbk4IKuoWkHs9w/640)

事实上我觉得加个 relu(x)=max(x,0) 会更好：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lktylJkW00DMxF97xZ2XTompsVTnnRW0h6q6WznoTpT1tcnGYqK2KTw/640)

其中![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lgqF5ZZkR8blPbH6yO8kBbxJSRMbpX63cT5zdBsiaUmyiamakyhz7icOgQ/640)采用随机插值的方式：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lCJzESjTE7TJOQQujo4oiaRPNwNApVPcsES3uoicicKoCch9hETzRFQQfg/640)

梯度惩罚不能保证 ‖f′(x)‖=1，但是直觉上它会在 1 附近浮动，所以 |f|L 理论上也在 1 附近浮动，从而近似达到 L 约束。

这种方案在很多情况下都已经 work 得比较好了，但是在真实样本的类别数比较多的时候却比较差（尤其是条件生成）。

**问题就出在随机插值上：**原则上来说，L 约束要在整个空间满足才行，但是通过线性插值的梯度惩罚只能保证在一小块空间满足。如果这一小块空间刚好差不多就是真实样本和生成样本之间的空间，那勉勉强强也就够用了，但是如果类别数比较多，不同的类别进行插值，往往不知道插到哪里去了，导致该满足 L 条件的地方不满足，因此判别器就失灵了。

思考：梯度惩罚能不能直接用作有监督的模型的正则项呢？有兴趣的读者可以试验一下。

**谱归一化**

梯度惩罚的问题在于它只是一个惩罚，只能在局部生效。真正妙的方案是构造法：构建特殊的 f，使得不管 f 里边的参数是什么，f 都满足 L 约束。

事实上，WGAN 首次提出时用的是参数裁剪——将所有参数的绝对值裁剪到不超过某个常数，这样一来参数的 Frobenius 范数不会超过某个常数，从而 |f|L 不会超过某个常数，虽然没有准确地实现 |f|L=1，但这只会让 loss 放大常数倍，因此不影响优化结果。参数裁剪就是一种构造法，这不过这种构造法对优化并不友好。

简单来看，这种裁剪的方案优化空间有很大，比如改为将所有参数的 Frobenius 范数裁剪到不超过某个常数，这样模型的灵活性比直接参数裁剪要好。如果觉得裁剪太粗暴，换成参数惩罚也是可以的，即对所有范数超过 Frobenius 范数的参数施加一个大惩罚，我也试验过，基本有效，但是收敛速度比较慢。

然而，上面这些方案都只是某种近似，现在我们已经有了谱范数，那么可以用最精准的方案了：**将 f 中所有的参数都替换为 w/‖w‖2**。这就是谱归一化（Spectral Normalization），在*Spectral Normalization for Generative Adversarial Networks*[2]一文中被提出并实验。

这样一来，如果 f 所用的激活函数的导数绝对值都不超过 1，那么我们就有 |f|L≤1，从而用最精准的方案实现了所需要的 L 约束。

注：“激活函数的导数绝对值都不超过 1”，这个通常都能满足，但是如果判别模型使用了残差结构，则激活函数相当于是 x+relu(Wx+b)，这时候它的导数就不一定不超过 1 了。但不管怎样，它会不超过一个常数，因此不影响优化结果。

我自己尝试过在 WGAN 中使用谱归一化（不加梯度惩罚，参考代码见后面），**发现最终的收敛速度（达到同样效果所需要的 epoch）比 WGAN-GP 还要快，效果还要更好一些。**而且，还有一个影响速度的原因：就是每个 epoch 的运行时间，梯度惩罚会比用谱归一化要长，因为用了梯度惩罚后，在梯度下降的时候相当于要算二次梯度了，要执行整个前向过程两次，所以速度比较慢。

**Keras实现**

在 Keras 中，实现谱归一化可以说简单也可以说不简单。

说简单，只需要在判别器的每一层卷积层和全连接层都传入 kernel_constraint 参数，而 BN 层传入 gamma_constraint 参数。constraint 的写法是：

```python
def
```
```python
spectral_normalization
```
```python
(w)
```
```python
:
```

```python
return
```
```python
w / spectral_norm(w)
```

参考代码：

https://github.com/bojone/gan/blob/master/keras/wgan_sn_celeba.py

说不简单，是因为目前的 Keras（2.2.4 版本）中的 kernel_constraint 并没有真正改变了 kernel，而只是在梯度下降之后对 kernel 的值进行了调整，这跟论文中 spectral_normalization 的方式并不一样。如果只是这样使用的话，就会发现后期的梯度不准，模型的生成质量不佳。

为了实现真正地修改 kernel，我们要不就得重新定义所有的层（卷积、全连接、BN 等所有包含矩阵乘法的层），要不就只能修改源码了，修改源码是最简单的方案，修改文件**keras/engine/base_layer.py**的 Layer 对象的 add_weight 方法，本来是（目前是 222 行开始）：

```python
def
```
```python
add_weight
```
```python
(self,
```

```python
name,
```

```python
shape,
```

```python
dtype=None,
```

```python
initializer=None,
```

```python
regularizer=None,
```

```python
trainable=True,
```

```python
constraint=None)
```
```python
:
```

```python
"""Adds a weight variable to the layer.
```

```python
# Arguments
```

```python
name: String, the name for the weight variable.
```

```python
shape: The shape tuple of the weight.
```

```python
dtype: The dtype of the weight.
```

```python
initializer: An Initializer instance (callable).
```

```python
regularizer: An optional Regularizer instance.
```

```python
trainable: A boolean, whether the weight should
```

```python
be trained via backprop or not (assuming
```

```python
that the layer itself is also trainable).
```

```python
constraint: An optional Constraint instance.
```

```python
# Returns
```

```python
The created weight variable.
```

```python
"""
```

```python
initializer = initializers.get(initializer)
```

```python
if
```
```python
dtype
```
```python
is
```
```python
None
```
```python
:
```

```python
dtype = K.floatx()
```

```python
weight = K.variable(initializer(shape),
```

```python
dtype=dtype,
```

```python
name=name,
```

```python
constraint=constraint)
```

```python
if
```
```python
regularizer
```
```python
is
```
```python
not
```
```python
None
```
```python
:
```

```python
with
```
```python
K.name_scope(
```
```python
'weight_regularizer'
```
```python
):
```

```python
self.add_loss(regularizer(weight))
```

```python
if
```
```python
trainable:
```

```python
self._trainable_weights.append(weight)
```

```python
else
```
```python
:
```

```python
self._non_trainable_weights.append(weight)
```

```python
return
```
```python
weight
```

修改为：

```python
def
```
```python
add_weight
```
```python
(self,
```

```python
name,
```

```python
shape,
```

```python
dtype=None,
```

```python
initializer=None,
```

```python
regularizer=None,
```

```python
trainable=True,
```

```python
constraint=None)
```
```python
:
```

```python
"""Adds a weight variable to the layer.
```

```python
# Arguments
```

```python
name: String, the name for the weight variable.
```

```python
shape: The shape tuple of the weight.
```

```python
dtype: The dtype of the weight.
```

```python
initializer: An Initializer instance (callable).
```

```python
regularizer: An optional Regularizer instance.
```

```python
trainable: A boolean, whether the weight should
```

```python
be trained via backprop or not (assuming
```

```python
that the layer itself is also trainable).
```

```python
constraint: An optional Constraint instance.
```

```python
# Returns
```

```python
The created weight variable.
```

```python
"""
```

```python
initializer = initializers.get(initializer)
```

```python
if
```
```python
dtype
```
```python
is
```
```python
None
```
```python
:
```

```python
dtype = K.floatx()
```

```python
weight = K.variable(initializer(shape),
```

```python
dtype=dtype,
```

```python
name=name,
```

```python
constraint=
```
```python
None
```
```python
)
```

```python
if
```
```python
regularizer
```
```python
is
```
```python
not
```
```python
None
```
```python
:
```

```python
with
```
```python
K.name_scope(
```
```python
'weight_regularizer'
```
```python
):
```

```python
self.add_loss(regularizer(weight))
```

```python
if
```
```python
trainable:
```

```python
self._trainable_weights.append(weight)
```

```python
else
```
```python
:
```

```python
self._non_trainable_weights.append(weight)
```

```python
if
```
```python
constraint
```
```python
is
```
```python
not
```
```python
None
```
```python
:
```

```python
return
```
```python
constraint(weight)
```

```python
return
```
```python
weight
```

也就是把 K.variable 的 constraint 改为 None，把 constraint 放到最后执行。**注意，不要看到要改源码就马上来吐槽 Keras 封装太死，不够灵活什么的，你要是用其他框架基本上比 Keras 复杂好多倍（相对不加 spectral_normalization 的 GAN 的改动量）。**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmmZzZ0Rlc16P7fSh4kK91lmVicMudpaekDTF5zeEIkVN2X5C6BXRmeTXVnPVm7OdWFvt3El1AlOyw/640)

# 总结

本文是关于 Lipschitz 约束的一篇总结，主要介绍了如何使得模型更好地满足 Lipschitz 约束，这关系到模型的泛化能力。而难度比较大的概念是谱范数，涉及较多的理论和公式。

整体来看，关于谱范数的相关内容都是比较精巧的，而相关结论也进一步表明线性代数跟机器学习紧密相关，很多“高深”的线性代数内容都可以在机器学习中找到对应的应用。

# 参考文献

[1]. Spectral Norm Regularization for Improving the Generalizability of Deep Learning. Yuichi Yoshida, Takeru Miyato. ArXiv 1705.10941.
[2]. Takeru Miyato, Toshiki Kataoka, Masanori Koyama, and Yuichi Yoshida. Spectral normalization for generative adversarial networks. In ICLR, 2018.
[3]. https://en.wikipedia.org/wiki/Power_iteration

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看作者其他文章：**

[变分自编码器VAE：原来是这么一回事 | 附开源代码](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)
[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)
[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)
[深度学习中的互信息：无监督提取特征](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492040&idx=1&sn=f90a6b899e62748c4db489ce06276869&chksm=96ea3e48a19db75e8c07d942a4772bb6c784fac7bcb117da2023186546cfe1876b121a8121cc&scene=21#wechat_redirect)
[全新视角：用变分推断统一理解生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)
[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)
[细水长flow之f-VAEs：Glow与VAEs的联姻](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491695&idx=1&sn=21c5ffecfd6ef87cd4f1f754795d2d63&chksm=96ea3fefa19db6f92fe093e914ac517bd118e80e94ae61b581079023c4d29cedaaa559cb376e&scene=21#wechat_redirect)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 查看作者博客




# 从Wasserstein距离、对偶理论到WGAN - Paper weekly - CSDN博客


2019年02月02日 08:52:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：190


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨苏剑林
单位丨广州火焰信息科技有限公司
研究方向丨NLP，神经网络
个人主页丨kexue.fm

2017 年的时候笔者曾写过[互怼的艺术：从零直达WGAN-GP](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247484880&idx=1&sn=4b2e976cc715c9fe2d022ff6923879a8&chksm=96e9da50a19e5346307b54f5ce172e355ccaba890aa157ce50fda68eeaccba6ea05425f6ad76&scene=21#wechat_redirect)，从一个相对通俗的角度来介绍了 WGAN，在那篇文章中，WGAN 更像是一个天马行空的结果，而实际上跟 Wasserstein 距离没有多大关系。

在本篇文章中，我们再从更数学化的视角来讨论一下 WGAN。当然，**本文并不是纯粹地讨论 GAN，而主要侧重于 Wasserstein 距离及其对偶理论的理解。**

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUqtlibMLlflBU82yz57LUfmnWV1fCqPNF5yw4QTPInicVGaZeFhWqButg/640?wx_fmt=jpeg)
**▲**推土机哪家强？成本最低找Wasserstein

本文受启发于著名的国外博文*Wasserstein GAN and the Kantorovich-Rubinstein Duality*[1]，内容跟它大体上相同，但是删除了一些冗余的部分，对不够充分或者含糊不清的地方作了补充。不管怎样，在此先对前辈及前辈的文章表示致敬。

注：完整理解本文，应该需要**多元微积分、概率论以及线性代数**等基础知识。还有，本文确实长，数学公式确实多，但是，真的不复杂、不难懂，大家不要看到公式就吓怕了。

# Wasserstein距离

显然，整篇文章必然围绕着 Wasserstein 距离（W 距离）来展开。假设我们有了两个概率分布 p(x),q(x)，那么 Wasserstein 距离的定义为：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsULK5JKTVL4uezDe3avWM7sRJkBY7qwrCcr5MhkEvDPibB6RZHHmMBxicA/640?wx_fmt=png)

事实上，这也算是最优传输理论中最核心的定义了。

相信我，式 (1) 没有想象中那么难理解。我们来逐项看看。

**成本函数**

首先 d(x,y) ，它不一定是距离，其准确含义应该是一个成本函数，代表着从 x 运输到 y 的成本。常用的 d 是基于 l 范数衍生出来的，比如：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUhYKGIIrlUWvHtQgVzmlU7Isa3u6sUhHG7Bsdc1KiaqI6qG9FsDxwtkQ/640?wx_fmt=png)

都是常见的选择，其中：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU01ZltvibMmxwdTCbos31PxQLIhZ5RterQZKbjMjJ0vsKDxOvRtLibOmA/640?wx_fmt=png)

特别指出，**其实哪种距离并不是特别重要，因为很多范数都是相互等价的**，范数的等价性表明其实最终定义出来的 W 距离都差不多。

**成本最小化**

然后来看 γ ，条件 γ∈Π[p,q] 意味着：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUqBC9bUCYjNcTRRz7tCJoB5TSKqtEoW0PIib5dXiclU6DMQGstnf48YTQ/640?wx_fmt=png)

这也就是说， γ 是一个联合分布，它的边缘分布就是原来的 p 和 q。

事实上 γ 就描述了一种运输方案。不失一般性，设 p 是原始分布，设 q 是目标分布， p(x) 的意思是原来在位置 x 处有 p(x) 量的货物， q(x) 是指最终 x 处要存放的货物量，如果 p(x)>q(x) ，那么就要把 x 处的一部分货物运到别处，反之，如果 p(x)<q(x) ，那么就要从别的地方运一些货物到 x 处。而 γ(x,y) 的意思是指，要从 x 处搬 γ(x,y)dx 那么多的东西到 y 处。

最后是 inf ，这表示下确界，简单来说就是取最小，也就是说，要从所有的运输方案中，找出**总运输成本 ∬γ(x,y)d(x,y)dxdy 最小的方案**，这个方案的成本，就是我们要算的 W[p,q] 。

如果将上述比喻中的“货物”换成“沙土”，那么 Wasserstein 距离就是在求最省力的“搬土”方案了，所以 Wasserstein 距离也被称为“推土机距离”（Earth Mover's Distance）。

最后改编一张开头提到的国外博文的图片，来展示这个“推土”过程：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUGqG3XyG50hO9GP4gFfp9t1tIHVhhmLDjia2N5SzhTj3qTiaojuxVMfcA/640?wx_fmt=png)
**▲**推土机距离图示。左边p(x)每处的沙土被分为若干部分，然后运输到右端q(x)的同色的位置（或者不动）

**矩阵形式**

逐项分析完含义之后，现在我们再来完成地重述一下问题，我们实际上在求下式的最小值：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUg9f6x4JbGibhVRMo0MfMuuibxs0YjaPr2JWI5sCtibVF13FqCNEYCuw8Q/640?wx_fmt=png)

其中 d(x,y) 是事先给定的，而这个最小值要满足如下约束：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUcV3wUibnozLpRBKeukkhjxjzH9q31icAa6zhdffA30IIqBBUURyK2OgQ/640?wx_fmt=png)

认真盯着式 (5)，考虑到积分只是求和的极限形式，所以我们可以把 γ(x,y) 和 d(x,y) 离散化，然后看成很长很长的（列）向量 Γ 和 D：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUq3OVicAQdJia0CR3yIER5z2Rwkf9xgHyMPljib25N2T5iaCEUzOQXpUPaA/640?wx_fmt=png)

所以式 (5) 相当于就是将 Γ 和 D 对应位置相乘，然后求和，这不就是**内积**⟨Γ,D⟩ 了吗？

如果还没理解这一点，那么**请再好好地盯一会式 (5)**，头脑中想象着将 x,y 分区间离散化的过程，再想想积分的定义，相信这并不难理解。

如果已经理解了这一点，那就好办了，我们可以把约束条件 (6) 也这样看：把 p(x),q(x) 分别看成一个长向量，然后还可以拼起来，把积分也看成求和，这时候约束条件 (6) 也可以写成矩阵形式 AΓ=b：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUG2vlTtL0fYvD8lVRZXaOG9lImc0KYPzwxPf6Y1bYB2AzugAf60xCHQ/640?wx_fmt=png)

最后不能忘记的是 Γ ≥ 0 ，它表示 Γ 的每个分量都大于等于 0。

**线性规划问题**

现在问题可以用一行字来描述：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUqaPuNwqOu9CEAibib9s2L9ibNTUjicTMfAY3LuB92MQUtyIjKQxUVoiaQBw/640?wx_fmt=png)

这就是**“线性约束下的线性函数最小值”**的问题，它就是我们在高中时就已经接触过的线性规划问题了。可见，虽然原始问题足够复杂，又有积分又有下确界的，但经过转写，它本质上就是一个并不难理解的线性规划问题（当然，“不难理解”并不意味着“容易求解”）。

# 线性规划与对偶

让我们用更一般的记号，把线性规划问题重写一遍，常见的形式有两种：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUcBVoAEgpPkiafN6o6eznc7IpClX0UGsN9ohVicWQCgibAph27aniaqRtVA/640?wx_fmt=png)

这两种形式本质上是等价的，只不过在讨论第一种的时候相对简单一点（真的只是简单一点点，并没有本质差别），而从 (9) 式可以知道，我们目前只关心第一种情况。

注意，为了避免混乱，我们必须声明一下各个向量的大小。我们假设每个向量都是列向量，经过转置 ⊤ 之后就代表一个行向量，![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUMIr3CaY25ZwjOLrOSelUAUdXZFnqDibS5pgoWQ5yC5Nm86r1Crzsg3g/640?wx_fmt=png)都是 n 维向量，其中 c 也就是权重，![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU3ULvYRaZUtOopgpfSaMuGs1xTLy2eJq7NpO0XPiaFZxhLtoO4CfDcKQ/640?wx_fmt=png)就是对 x 的各个分量加权求和；![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUuHERkkVnnVrgVsImudzmbiaFqJibFicQEA6DxJGoslUT6hcuV8j3xvRqw/640?wx_fmt=png)是 m 维向量，自然![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUsABOlp69EhEiaXWyAte9GZ2pHibsCHN1D9eLmARGuQuA3H1PpRdBuB8Q/640?wx_fmt=png)是一个 m×n 的矩阵了， Ax=b 实际上就是描述了 m 个等式约束。

**弱对偶形式**

在规划和优化问题中，“对偶形式”是一个非常重要的概念。一般情况下，“对偶”是指某种变换，能将原问题转化为一个等价的、但是看起来几乎不一样的新问题，即：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUb4d6Nic2I8L7wknoBjWtwYzEFM1uz8M6fk9KBVibxPj80oCic2WTBjXZw/640?wx_fmt=png)

**“对偶”之所以称为“对偶”**，是因为将新问题进行同样形式的变换后，通常来说能还原为原问题，即：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUiaRSG3c00yCYUrdRo8qRAoup3nr0icTkRQdaib1jKM8Ru1q2NzMR8u2Dw/640?wx_fmt=png)

即“对偶”像是一面镜子，原问题和新问题相当于“原像”和“镜像”，解决了一个问题，就等价于解决了另一个问题。所以就看哪个问题更简单了。

读者可能还有疑问：“对偶”跟数学中诸如“逆否命题”之类的等价描述有什么区别？其实也没有本质区别，简单来说“对偶”和“逆否命题”都是跟原来的命题完全等价的，但是“对偶”看起来跟原命题很不一样，而“逆否命题”仅仅是原命题的一个逻辑变换。从线性代数的角度来看，“对偶”相当于向量空间中的“原空间”和“补空间”之间的关系。

**最大 vs 最小**

这里我们先介绍“弱对偶形式”，其实它推导起来还是挺简单的。

我们的目标是![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU5Cpa1qHibux6jgZhXGQicakve0z6N1I6XBF9iaao2WUCFEeIAdCMRJcwQ/640?wx_fmt=png)，设置最小值在![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUso642OP6uBUicvCfg0Pq3WZjg5sCg84NH7bILzWa1Ymia3K0g9AqknTQ/640?wx_fmt=png)处取到，那么我们有![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUGFkQwoGv0iaMrjD0xn8Zv4CXOvNOujmVWSrdDzm63Q7qqvrWI2gkRjQ/640?wx_fmt=png)，我们可以在两边乘以一个![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUGFRTLArInTX9NOQuY6qJTzr0vGKKhEQyLIMmCRy0zgibA0hGcfSWLRg/640?wx_fmt=png)，使得等式变成一个标量：![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUVNQt2pJ4RricWzCxM7PS3ZaV0HoXQvtWiaMvKiaMkvzbbwNCKeYRibPbnQ/640?wx_fmt=png)。

如果此时假设![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUTiblx84zeoqHaXG5UUXxLTTicd4H02jL61ibRR0qHhQQICHuCQrl7me4Q/640?wx_fmt=png)，那么![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU3ZzKghhK253S8Z0hibVhaOazHypNPJ74XhCf4SE7yjZdTXo03fttiacw/640?wx_fmt=png)（因为![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUgQIx8FL2ZjYzDV4E1PicFvNyhqQGbxVNIMyiaE2icverg8ScJJNWia6pWg/640?wx_fmt=png)），则![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUF25Hb0w51AdTQEicWpw77WsxaLdhWiazzjg5yybpXk1qIMl9lgDdu5HQ/640?wx_fmt=png)。也就是说，在条件![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUaIpu8fhrZxIafmOgeiccjm0ytMfJ3MIIQcClZv5SIdk9icfv3V8zJN6w/640?wx_fmt=png)下的任意![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUJMFsfyPQ7nAnzyIbv0z5BicBQxQIzIiaEYy5xO2zUR93NywCLdetg3tw/640?wx_fmt=png)总是不大于![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU5Cpa1qHibux6jgZhXGQicakve0z6N1I6XBF9iaao2WUCFEeIAdCMRJcwQ/640?wx_fmt=png)，“总是”意味着即使对于最大那个也一样，所以我们就有：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUrhQiaqGhL1JupeS3r9xy4AJN4sOHMJTiceVOSiclD9VKchl8E6CGYDO8g/640?wx_fmt=png)

这便称为“弱对偶形式”，它的形式就是：**“左边的最大”还大不过“右端的最小”**。

**几点评注**

对于弱对偶形式，也许下面几点值得进一步说明一下：

1. 现在我们将原来的最小值问题变成了一个最大值问题![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU2endsfwicZE7iaYuIpvpCdThQc3FEdXpicKib5ibBJXVWQt7BFZjlrGcgSw/640?wx_fmt=png)，这便有了对偶的味道。当然，弱对偶形式之所以“弱”，是因为我们目前找到的对偶形式只是原来问题的一个下界，还没有证明它们二者相等。

2. 弱对偶形式在很多优化问题中（包括非线性优化）都成立。如果二者真的相等，那么就是真正意义上的对偶了，称为强对偶形式。

3. 理论上，我们确实需要证明式 (13) 左右两端相等才能进一步应用它。但从应用角度，其实弱对偶形式给出的下界都已经够用了，因为深度学习中的问题都很复杂，能有一个近似的目标去优化都已经很不错了。

4. 读者可能会想问：前面我们为什么要假设![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUTiblx84zeoqHaXG5UUXxLTTicd4H02jL61ibRR0qHhQQICHuCQrl7me4Q/640?wx_fmt=png)而不干脆假设![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU2QibIzfXdG6DEg2Wq30U7Sib3wBy9oCMplKp43Bg5fI6wcSIrCkBiaReA/640?wx_fmt=png)？假设后者当然简单很多，但问题是后者在实践中很难实现，所以只能假设前者。

**强对偶形式**

上面已经说了，从实践角度其实弱对偶形式已经够用了。但是为了让对完整理论的读者也有更多收获，这里继续把“强对偶形式”也论证一番。对于只关心 WGAN 本身的读者来说，可以考虑跳过这部分。

所谓强对偶形式，也就是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUqoZGkBY05sHEf6lznSSM6185JNeicVOQm3887TPX87qIf0VlvxPCfVg/640?wx_fmt=png)

注意前面已经说了，弱对偶形式对于很多优化问题都成立，但强对偶形式不一定成立。而对于线性规划来说，强对偶形式是成立的。

**Farkas引理**

强对偶形式的证明，主要用到称之为“Farkas 引理”的结论：

对于固定的矩阵![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUNN2EDtLVxIASwptCxQBLIvkvMTZHE23pOGETRyHEsq5oyibDLbsBb1A/640?wx_fmt=png)和向量![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUPBicanJHqEVwibF41GUdS6vp28TPLK2vLaEKwnFtp86iageYIBuxibyjnQ/640?wx_fmt=png)，下面两个选项有且只有一个成立：

1. 存在![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUdx5DEh84DGZWFrLUZnks6iahocicdldI1OkHibV7BpasXm8xeWvjnicLvA/640?wx_fmt=png)且 x≥0，使得Ax=b；

2. 存在![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUZu7qoTZXjDzK5nUIQWZj7jDniazla2vAyReyanXUQ3SRlLcwg8t0Spw/640?wx_fmt=png)使得![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUco0GgtC8xmShvXc0ciceiakkciboMuN3ViceYB4w8ukbhcXLEGoHu4thqQ/640?wx_fmt=png)且![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUArEGvClPkh2Zk8iaTfzoGoA3ZCn42UGfvQQJxib7gKgL7IP6SpdVFA6Q/640?wx_fmt=png)。

什么鬼？又大又小又转置的？能不能说人话？

其实这个引理还真有一个很直观的几何解释，只不过几何解释翻译成代数语言就不简单了。几何解释的出发点是我们去考虑如下的向量集合：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUROfTYSCGjqLZOTexC7kfahdLe04PjmhCNXoHw87EvLiaiaquhHibiaa5xg/640?wx_fmt=png)

这个集合的含义是：我们将 A 看成是 n 个 m 维列向量的组合。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU0nGW57arRYgAMS8vPOz2B6YszJ64TR6OnIzeEibP4OBExh06TcoqUjw/640?wx_fmt=png)

那么上述集合实际上就是所有 a1,a2,…,an 的非负线性组合。那这个集合是个啥呢？答案是：**一个锥**，如图所示。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUxEL3TqzhblonPYgtMcpicsy1EMzQibXyRjia9DUeicia45wguakAIiaficE9A/640?wx_fmt=png)
**▲**给定向量的非负线性组合构成这些向量围成的一个锥

现在我们随便给定一个向量 b，那么显然它只有两种可能性，而且必有一种成立：**1. 在锥内（包括边界）**；**2. 在锥外。**这当然是废话，但是将它翻译成代数语言，那就不是废话了。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUDJdFYjcrFbUaHXicwn4hUUmaCicIQAcPSicYJVI2lv3J2wDiaBg8SA88icg/640?wx_fmt=png)
**▲**给定的向量在锥内，意味着可以表示为非负线性组合

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUM9RBguGx2LYpQxBY3kP6eVVOFMPGwZqCUOxFQ0wwcWcmY6dJob0VeA/640?wx_fmt=png)
**▲**给定的向量在锥外，我们可以找到一个“标杆”向量与之对比

如果它在锥内，那么根据锥本身的定义，它就可以表示为 a1,a2,…,an 的非负线性组合（表示方式可能不唯一），也就是存在 x≥0，使得 Ax=b，这就是第一种情况。

如果在锥外呢？怎么表示在锥外？当然我们可以直接写出锥内的否命题，但是那实用价值不大。如果向量 b 在锥外，那么我们总是可以找到一个“标杆”向量 y，它与 a1,a2,…,an 的夹角都大于等于 90 度，向量表示法就是内积都小于等于零，即![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUoI0siaJ4omPnjaHRvlBGvPSy3xRXEvhDYnibUTEPricLG89C8CtJKPrOg/640?wx_fmt=png)，或者写成一个整体![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUXO5cicU8AvujrbiaPQx0EQ3m5XmURicqcjficaRpOxGLkfweKiasNTvJoRA/640?wx_fmt=png)。

找到这个“标杆”后，向量 b 与“标杆”的夹角必然是要小于 90 度的，即![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU7BOlavJXGlVC77cRTaibfBKmvNPdttW2IwwHCtjaR0E2D1aaNCz4Fhg/640?wx_fmt=png)。这样一个大于等于 90 度，一个小于 90 度，保证了向量 b 在全体向量构成的锥外。这就是第二种情况。

当然，这不能算是完备的证明，只能算是一个启发式引导，完备的证明还要仔细论证为什么这些向量的非负线性组合构成了一个锥。这些就不在本文的范畴了。**Farkas 引理的特点是二选一，比如我要证明满足第二点，只需要证明它不满足第一点，反之亦然。这是对问题的一个转化。**

**从引理到强对偶**

有了 Farkas 引理，我们就可以证明强对偶形式了。证明的思路是：证明 max 可以任意程度地接近 min 。

证明还是先假设 min 的最小值在![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU5iazvOhkblVf98payhSbADOuGS0NUsjtDPVsTVGSJibTEssTtDO0qDIQ/640?wx_fmt=png)处取到，即最小值为![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUgXo1Lblstv4rqu5icDVOiaUBotIMvPa3ibu0Ip7KlVd1uS0KD3ZTjTcfQ/640?wx_fmt=png)，那么我们考虑：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUd72fOIwyRMGicSuRGPoGZxWJHx0mFiaaYJN92ibJyeCzHAlbOTliaYGZEw/640?wx_fmt=png)

当 ϵ>0 时，那么对于任意 x≥0，![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU46EjlWcxmib21GHkRwEBQQ1tibdecypkia3d5oNo6qribo4uPPy7y48CLw/640?wx_fmt=png)都不可能等于![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUFry0bvNCOGYEoy3sicFXO4tae2FtUOANdU9wMC8yqFEu2MUtjNgC3mg/640?wx_fmt=png)，这是因为![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUqDnwvbckictDGI5QQkCyOef7vjXtIuWGHtd5FdjlyiaiaUHia8ZJVz58dA/640?wx_fmt=png)已经是最小值，所以![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU7xX7FiaV4tGjNPKzp0ggVQs2ToAzdPnFljYbT7X32Qia3esMWxCMxKuw/640?wx_fmt=png)是![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUovGz3icxXNwvY3IvxQRVIDx34Gk9cNBJyY2axicYhtqnHLXIu8MVOcUA/640?wx_fmt=png)能达到的最大值，它不可能等于更大的![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU74YahWibeicibEKGInwjn3uGG8C66CnlQu8rkLBqYh5icIIo07iccjicCedA/640?wx_fmt=png)。

前面已经说了，不满足第一种情况，那就只能满足第二种情况了，即存在![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUyR3J23dXGWE4B0pJllHIiaKnY4YHmxlSocthWFbUFL4utZMayKwaNvA/640?wx_fmt=png)使得![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUgC7TzjCfDwNY3nJhNgy0FPtstayHGyHzZwX9ISqZCltuMlt4Sib0pdw/640?wx_fmt=png)且![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUEJ1wGRhFXblEcawOO6JwY4Dao5hSBym9V4sBfqmv4LIOfFXzeGic6wg/640?wx_fmt=png)，这等价于：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU0WiblNTQHL5BSialPWNhBBNhnibiaD6uX2vLxFDJntibnYxvEjoTicHmeRJw/640?wx_fmt=png)

下面我们表明 α 必须大于 0，这是因为![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUy4iaxuzd3XeGcphcZib1OibIxZib7sIiaDOakk48lgzq0iaynUPFzZTKkMzw/640?wx_fmt=png)，所以我们再去考虑 ϵ=0。而当 ϵ=0 时有![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUibgXWzBxWaia5fdeoPeoXT9AucZDr0UwYzDcHhT309ZjlNGon6tGmsicA/640?wx_fmt=png)，即满足 Farkas 引理的第一种情况，那就不满足第二种情况，而不满足第二种情况，意味着![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUCVrPjibpByIdyKE19Pvc2T11S8tzSibgtR8ZjLermhAjsaCiavy49jcVA/640?wx_fmt=png)，都有![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUv9pRHFCgA9poOQuQECgEeLaXxVu21cm6hN26RLwiaicrt9ZCib0kAtykA/640?wx_fmt=png)，刚刚我们已经证明了![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUyeoyaRnk09zdURQhBELWeich4zCibFWF4Cp25MfrWibgbROkmo90g3XEQ/640?wx_fmt=png)，所以必须有 α>0。

现在确定 α>0 了，我们就可以从式 (18) 得到：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUX8icCbGWPSsVOdmZRibtqvzkKG8xausibB7F5QoC7iaHHqYXfg61FTo18w/640?wx_fmt=png)

这意味着：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUVQzHOTujB8e7QFr2SI7NRyHsomqrFk9mV2l7ueQWezdcmEnWWlhIvw/640?wx_fmt=png)

而弱对偶形式已经告诉我们：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU3SJF3SHibzN3U4SX1esUDYPGute8wpY4lNCSXeURHT0uCvXaVLmlYdA/640?wx_fmt=png)

也就是![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUcWwm5uGQXSbBesj9SaH0Sxz7wYKL9Mu0Z7zIa9EKofhEI8UfUljLIA/640?wx_fmt=png)被夹在![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUBUtoRm0E94R6k7Pl2M2qhciaujnqqQAghSTabvtydZ2wMJdzicHZj77g/640?wx_fmt=png)和![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUC5T2ibVgiac1mcr5wVNQ6hGYl5pobsMGODiaL9TsoZ5VDjaQZEdoqpaPQ/640?wx_fmt=png)之间，而因为 ϵ > 0 是任意的，所以两者可以无限接近，从而：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUUDuScDMdwI118EmDtvGHibPMfn5NHkZnwD7Mic5Uro8QCR5GliawI4nMg/640?wx_fmt=png)

这便是要证的强对偶形式。

**简单说明**

Farkas 引理和强对偶形式的证明，看上去比较迂回，但实际上是优化理论中非常经典和重要的证明案例，**对于初学者来说，它应该是一次非常强烈的思维冲击。**因为我们以往的认识中，我们对原命题做变换，仅仅是局限于“逻辑变换”，如否命题、逆否命题等。而对偶形式和 Farkas 引理却出现了一些“看起来很不一样，却又偏偏等价”的结论。

Farkas 引理和强对偶形式也可以进一步推广到一般的凸集优化问题，证明手段相似，只不过在对区域和不等关系的讨论上，比上面的线性规划的过程更为细致和复杂。不过我也不是专门搞这些优化问题的，所以只有一个模糊的认识，就不再继续班门弄斧了。

# Wasserstein GAN

好了，进行了一大通的准备工作后，我们终于可以导出 Wasserstein GAN 了，就本文来看，它只不过是线性规划的对偶形式的一个副产品罢了。

**W距离的对偶**

在推导之前，我们还是再来捋一捋本文的思路：本文先给出了 W 距离的定义 (1) ，然后经过分析，发现它其实就是普通线性规划问题的一个连续版本，转化过程为 (7) , (8) 和 (9) ；因此中间我们花了相当一部分篇幅去学习线性规划及其对偶形式，最终得到了结论 (14) 。

现在我们要做的事情，就是把整个过程“逆”过来，也就是将找出 (14) 对应的连续版本，为 W 距离找一个对偶表达式。 其实这个过程也不复杂，由结论 (14) 和式 (9) ，我们得到：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUZ9HygaIySxxpn7TgickrzLRwhIx38jYbeANO2GXHww4gypmleL6lRyw/640?wx_fmt=png)

注意式 (8) 中 b 是由两部分拼起来的，所以我们也可以把 F 类似地写成：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUfeCicvkYhwibDVMqurLpb7SUN6Zn1LN0MHYjYsr7aM2rNjKdwWyInj5g/640?wx_fmt=png)

现在 ⟨b,F⟩ 可以写成：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUicZP8rwZ6weC7L6nJU86QBSb53W28sGq0KBnEs10pjdjXBLJNdHLzuA/640?wx_fmt=png)

或者对应的积分形式是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUcDDXQsJ2eYU1QXF9Ys4vialkT0iaTSfV0qg858hobjxvvhQn9gsSgeSA/640?wx_fmt=png)

别忘了约束条件![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU34DmQVfUd668ZrBwskay1T6CLwJ5yJqUpGUp5OmoWBZ0ZFNrHNBZCA/640?wx_fmt=png)：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUPoS2TmsHUpPYuzLbs5oiblakPibx31ICBf1sJibYHrHxPrUoGm3albjHw/640?wx_fmt=png)

代入计算后，可以发现这个诺大的矩阵运算实际上就说了这样的一件事情：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUHZrOzfMDFYagpAfLuXJnLeXyx7KND2LAeeZ74AKoic96H3skWs1JrGQ/640?wx_fmt=png)

或者直接写成：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUf8Rkicz5OQB45S2xtdLN2jZNGaw35qZEuLCBvD7vCaH8fgO327dETeA/640?wx_fmt=png)

**从对偶到WGAN**

终于，我们就要接近尾声了，现在我们得到了 W 距离 (1) 的一个对偶形式了：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUzxRVX89zfXygmrCO8Gb3OibSY4Pq7VUnf8k8AO3dxqBnTsgCjLcdT8w/640?wx_fmt=png)

注意到由 f(x)+g(y)≤d(x,y) 我们得到：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUsjbia82JXyV7Mdmr6iape3m2SYgjtAYwDPUxEQxWuQD6RpACx1wyJDmg/640?wx_fmt=png)

即 g(x)≤−f(x)，所以我们有：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUE3NMqgp9B0CcEMtpmyJPD2DviaanIOQBqJSMbUGWPNJjk0aKg6w8SKA/640?wx_fmt=png)

即如果 g=−f，它的最大值不会小于原来的最大值，所以我们可以放心地让 g=−f，从而：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUoeIplicQ5ZX3icWzByKoKK281TUxqB4B2NHOL4zbGGGatpFI5IibZz2WQ/640?wx_fmt=png)

这便是我们最终要寻找的 W 距离 (1) 的对偶形式了，其中约束条件我们可以写为 ||f||L≤1，称为 Lipschitz 约束。

由于 p,q 都是概率分布，因此我们可以写成采样形式：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsUxANbq7ticygVGEQZbicibnRCtpOUctdlBxTrwjkicLXeYGWiaogd4Z6Kr6A/640?wx_fmt=png)

这就是 WGAN 的判别器所采用的 loss 了，自然地，整个 WGAN 的训练过程就是：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglZYPia25lbbK6F0v6ak5dsU0MhJlwdgdYCicQAtSAianD0k3icEUh2HJV8QX8JpZvq9sxaJeISkQLW7Q/640?wx_fmt=png)

千呼万唤的 WGAN 终于现身，剩下的就是怎么加 Lipschitz 约束的问题了，可以参考：[深度学习中的Lipschitz约束：泛化与生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492180&idx=1&sn=3ea92a3a9f1306efde89ce1777b80da6&chksm=96ea3dd4a19db4c20dcbc9627b0eb307672b4d61008a93c42814fa6728ca7b6f7c293cff1d80&scene=21#wechat_redirect)。

# 文章小结

本文主要介绍了 Wasserstein 距离，然后转化为一个线性规划问题，继而介绍了线性规划的对偶理论，最终导出了 Wasserstein 距离的对偶形式，它可以用作训练生成模型，即 WGAN 及后面一系列推广。

本文是笔者对线性规划及其对偶理论的一次简单学习总结，对熟悉线性代数后希望从理论上了解 WGAN 的读者应该会有一定的参考价值。如果对文中内容有什么疑惑或批评，欢迎留言指出。

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



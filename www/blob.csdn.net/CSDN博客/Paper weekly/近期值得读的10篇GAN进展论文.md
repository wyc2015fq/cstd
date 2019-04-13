
# 近期值得读的10篇GAN进展论文 - Paper weekly - CSDN博客


2019年01月03日 12:11:25[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：150


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

作者丨苏剑林
单位丨广州火焰信息科技有限公司
研究方向丨NLP，神经网络
个人主页丨kexue.fm

这篇文章简单列举一下我认为**最近这段时间中比较重要的 GAN 进展论文**，这基本也是我在学习 GAN 的过程中主要去研究的论文清单。

# 生成模型之味

GAN 是一个大坑，尤其像我这样的业余玩家，一头扎进去很久也很难有什么产出，尤其是各个大公司拼算力搞出来一个个大模型，个人几乎都没法玩了。但我总觉得，真的去碰了生成模型，才觉得自己碰到了真正的机器学习。这一点，不管在图像中还是文本中都是如此。所以，我还是愿意去关注生成模型。

当然，GAN 不是生成模型的唯一选择，却是一个非常有趣的选择。在图像中至少有 GAN、Flow、PixelRNN/PixelCNN 这几种选择，但要说潜力，我还是觉得 GAN 才是最具前景的，不单是因为效果，主要是因为它那对抗的思想。

而在文本中，事实上 Seq2Seq 机制就是一个概率生成模型了，而PixelRNN这类模型，实际上就是模仿着 Seq2Seq 来做的，当然也有用 GAN 做文本生成的研究（不过基本上都涉及到了强化学习）。也就是说，其实在 NLP 中，生成模型也有很多成果，哪怕你主要是研究 NLP 的，也终将碰到生成模型。

好了，话不多说，还是赶紧把清单列一列，供大家参考，也作为自己的备忘。

# 拿效果来说话

**话在前头**

不严谨地说，目前在 GAN 中，基本上都是效果说话。不管你多么完美的理论，只要你实验不能生成高清图，都很难被人接受；你再丑陋的结果，只要你实验效果够好，能生成高清大图，大家都得围着你转。

GAN 模型的一个标志性事件，是 NVIDIA 去年搞出来的*Progressive Growing GANs*，它首次实现了 1024*1024 的高清人脸生成。要知道，一般的 GAN 在生成 128*128 人脸时就会有困难，所以 1024 分辨率的生成称得上是一个突破。而**下面列举的一些论文，都是在自己的实验中做到了 1024 的人脸生成。**单是这个实验结果，就值得我们去关注一下这些论文。

当然，生成 1024 图除了需要模型的进步，还需要很大的算力，因此一般人/实验室都很难做到。关注这些论文，并不是要我们去复现这么大的图生成，而是因为这些模型能生成这么大的图，必然有它值得我们借鉴的地方，甚至我们可以从中明白到 GAN 的瓶颈所在，从而让我们在自己的研究中少走弯路。

**论文清单**


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWf2DhzdB3nic6DL94Dlgn920L5yibHdtPmVu97rQxO9jASjagOiaaS6WBQ/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWPabHBIuXkfFc9AF9TmDAg0eCVZvXtbqx9HamUakCeHEksiayt53t8ag/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWR2HkYF3FGOhvaYpqDzY7vxW033zE1wicW3a98bbgzGuuEqibqDFIIK7w/640?wx_fmt=png)

这篇文章就是前面说的首次实现了 1024 人脸生成的*Progressive Growing GANs*，简称 PGGAN，来自 NVIDIA。

顾名思义，**PGGAN 通过一种渐进式的结构，实现了从低分辨率到高分辨率的过渡，从而能平滑地训练出高清模型出来。**论文还提出了自己对正则化、归一化的一些理解和技巧，值得思考。当然，由于是渐进式的，所以相当于要串联地训练很多个模型，所以 PGGAN 很慢。


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWQZkWmhlzULxvCme8KmvMkibRibdrEeh22uyIlferialgwvwcib8loATFJQ/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWNv2iaqqZq2hXqJGf0TnSbOFk29sZN551SdicFAlZSicCpwPaSEcyjfQLA/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWb5U5DB3TghKUDZHZ0m7MJFh5iay3CmIiaUozW9b9IpkLQVgcTE8yicTBw/640?wx_fmt=png)

**这篇文章有很多对 GAN 训练稳定性的数学推导，最终得到了比 WGAN-GP 更简单的梯度惩罚项**，关注 GAN 训练稳定性的同学可以参考。

除了 1024 人脸，这篇文章也做了很多其他数据集的实验，效果都挺不错，而且都是直接端到端训练，不需要渐进式结构。我唯一困惑的是，这个惩罚项，不就是*WGAN-div*中的一个特例吗？为什么论文没有提到这一点？


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWlMibSTVOTn0pYcrK77icgbEa0NVcIr0TMFaqT3cp2545TdDNMWPKNpsw/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWZ7e1QsHFZK2HsxkOfCW2UpEZXK5zQxN9OjJfLkMJO4xrEdvWWHeOUA/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWbNYo9iauTxpuGw42m0mMT7vVOMFvLic41mmynm3XvD8CHptY2p7S7kpw/640?wx_fmt=png)

这是个会“反省”的 VAE，通过对抗来改进了 VAE，从而能生成高清图片，并且能同时得到编码器和生成器。

除了能生成1024的高清图，更值得一提的是，这篇文章在构思上非常精妙。因为能同时得到编码器和生成器的模型不算独特，比如 BiGAN 就能做到，但是**IntroVAE 独特之处在于它能直接利用了 encoder 作为判别器，不需要额外的判别器，也就是直接省去了 1/3 的参数量。**这背后更深层次的原因，值得我们去细细分析和回味。


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWwgmRvaNEbliaFR1wFgGnarJZAwiaIKm5bskVaADZZF0QlThdMyEVKB0A/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWiaibBGZicuUoZyXKibozMHOxb6NmMUx9gLRjc9JxKYF3OOO7Iicib71AEw1g/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWO7LTXThlHn199PD0gVt5Wz0vnsGJph3xl0WQvNTUMOUuudtdOTs61A/640?wx_fmt=png)

这就是大名鼎鼎的*BigGAN*。这篇文章虽然没有提供 1024 的人脸生成结果，但是它提供了 128、256、512 的自然场景图片的生成结果。要知道自然场景图片的生成可是比 CelebA 的人脸生成要难上很多倍，既然它连 512 的自然场景图片都可以生成了，我们自然不怀疑它能轻松生成 1024 的人脸。

BigGAN 在网上已经有很多科普介绍了，不再重复。论文还提出了自己的一些正则化技巧，并分享了大量的调参经验（调整哪些参数会有好的／坏的改变），非常值得参考。


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWRnS5vd88BEEhzpHcdQIEYmfAXVialNf4ByZgWQCK5LqSRfVdufIdFew/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSW4L2liarBCqfrhD1OoCNIsuDmOYwI7BLE16NU3C8fAgLCfNEdicuKbdKw/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWiasj6HHKicaeOuAKfWOX4v6hGkBJXqY36Y8PLDeZheZgquAJ4N592v0w/640?wx_fmt=png)

**这篇文章通过信息瓶颈来控制判别器的拟合能力，从而起到正则作用，稳定了 GAN 的训练。**信息瓶颈的简介可以参考我的[这篇文章](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493326&idx=1&sn=7ba19fe14ee11bff0e1c865adcb52ca1&chksm=96ea394ea19db0587dc096898730f7522a8e3a7bb3b55bac576422eea63a987ea97ad5886bca&scene=21#wechat_redirect)。总的来说，在普通有监督训练中一切防止过拟合的手段，理论上都可以用在判别器中，而信息瓶颈也算是防止过拟合的一种手段。

当然，从标题就可以知道，论文也不满足于只用在 GAN 中，除了 1024 的人脸图生成实验，论文还做了仿真学习、强化学习等实验。


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWgdyibc1BAEvsDrOvPoAYpMJlCpHcMD56DDxricevdK4zYfEeuW4faNcA/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWvo95ru8WFcI3CzHSISAnG0JXKfG9B52xHN5iah0o8BPYoxNDtd5ibmAw/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWbRQru8KicVafyxTOjqt3RziaN49biaRs0xbBSuq3THtCHGHLyUhoLcUiag/640?wx_fmt=png)

这就是前几天发出来的新的 GAN 生成器架构，被很多文章称之为 GAN 2.0，依旧是 NVIDIA，依旧是 PGGAN 的作者，依旧是 PGGAN 的模式。只不过生成器的架构换了，人家在一年前就已经生成了 1024 图，这次肯定也不例外了。

**这个新的生成器架构，据说是借鉴了风格迁移的模型，所以叫 Style-Based Generator。**我读了一下，其实它差不多就是条件 GAN（CGAN）的架构，但是把条件和噪声互换了。简单来说，就是**把噪声当作条件，把条件当作噪声，然后代入到 CGAN 中。**

看论文的效果图，这种思维上的转换的效果还是很不错的，我自己也试着实现了一下，能 work，但是有点 mode collapse，大家还是等开源吧。

顺便一提的是，一年前也是 PGGAN 的作者给我们带来了 CelebA HQ 数据集，现在还是他们给我们带来了新数据集 FFHQ。据说数据集和代码都将在明年一月开源，让我们拭目以待。

# 稳住训练再说

**话在前头**

与有监督学习的任务不同，有监督学习中，一般只要设计好模型，然后有足够多的数据，足够的算力，就可以得到足够好的模型；但 GAN 从来都不是设计模型就完事了，它是一个理论、模型、优化一体的事情。

从框架的角度来看，发展到 WGAN 后 GAN 的理论框架基本也就完备了，后面都只是不痛不痒的修补（包括我的*GAN-QP*）；从模型架构来看，DCGAN 奠定了基础，后来发展的 ResNet + Upsampling 也成为了标准框架之一，至于刚出来的 Style-Based Generator 就不说了，所以说模型架构基本上也成熟了。

**那剩下的是什么呢？****是优化，也就是训练过程。**我觉得，要想真正掌握 GAN，就得仔细研究它的优化过程，也许得从动力学角度来仔细分析它的训练轨迹。这可能涉及到微分方程解的存在性、唯一性、稳定性等性质，也可能涉及到随机优化过程的知识。总而言之，需要把优化过程也纳入到 GAN 的分析中，GAN 才可能真正完备起来。

下面的这些论文，**从不同的角度分析了 GAN 的训练问题，并给出了自己的解决方案**，值得一读。

**论文清单**


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWBk201u9hkia23mlYuvBoibiatuYQWAEsPKDmwBNF0rfqXjRhk4vicIqFuQ/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWUoav0ugILO1x5pbXSapa3gB1fCrDo1sEvnof0dEGu0Y23PVoJgpx8g/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWC2rVukDSpMEzdMUZEO2Xma3IELXzk3ZurNNG42lQMwntqK937fnbug/640?wx_fmt=png)

**本文通过加噪声的方式推导出了 GAN 的正则项，推导过程理论上适用于一切 f-GAN。**从论文效果图看，结果还是不错的。


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSW4uf5dVNhjAzzZH8ibQDmicoEiazv5djvWDSLTUo5XicIiazlt7vomKCNBRQ/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWyaEIOXoOj5NpibEH9Q9qTPZSIXJ9LzZDM6t7n6vUleia5vgxg89HiaD6A/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWvWQUyOreHdrVWU2Sv3E9CDduTIicODwqOWKX7j6wddGakYenDyibusmg/640?wx_fmt=png)

**本文提出了 TTUR 的训练策略**，大概意思就是：原来我们每次迭代都是用相同的学习率将判别器和生成器交替训练不同的次数，现在可以考虑用不同的学习率将各自训练一次，这样显然训练起来会更省时。

不过我粗略看了一下，尽管论文理论多，但是它理论基础却是另外一篇现成的文章**S***tochastic approximation with two**time**scales*[1]，可以说论文只是反复在用这个现成的理论基础，略微单调。


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWicK0iaJPQMfl3lDEy6Bmq3nibWex2P0ibtS0sEpFQY7bnhFRqLiaib4FCpNw/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWNv2iaqqZq2hXqJGf0TnSbOFk29sZN551SdicFAlZSicCpwPaSEcyjfQLA/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWb5U5DB3TghKUDZHZ0m7MJFh5iay3CmIiaUozW9b9IpkLQVgcTE8yicTBw/640?wx_fmt=png)

在前面已经介绍过这篇文章了，但这里还是再放一次，因为实在是太经典。感觉是**研究 GAN 训练稳定性必看的文章，作者****从微分方程角度来理解 GAN 的训练问题**。

在稳定性分析的过程中，这篇文章主要还引用了两篇文章，一篇是它的“前传”（同一作者），叫做*The Numerics of GANs*[2]，另一篇是*Gradient descent GAN optimization is locally stable*[3]，都是经典之作。


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSW2E9Z9oaZ47NwAznvF2ibyCWrpqPdmiaKcd4az5EsfjPIbuTWLoLq9Usw/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWMLibaFVF4cOgTj0BzhRJnm4kqdE9VpIjgfdezVewQmpXIOEo3zhgYtw/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWjV4w3u5SqoQDv6u27datQJRNButJEmeic7aric7nNcamrwJJ4d1NsM7Q/640?wx_fmt=png)

**本文通过谱归一化给判别器实现 L 约束，应该说是目前实现 L 约束最漂亮的方法了。**目前谱归一化也用得很广，所以值得一提。相关介绍也可以参考[我之前的文章](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492180&idx=1&sn=3ea92a3a9f1306efde89ce1777b80da6&chksm=96ea3dd4a19db4c20dcbc9627b0eb307672b4d61008a93c42814fa6728ca7b6f7c293cff1d80&scene=21#wechat_redirect)。


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWvrcwnbFSmIicVVTsZlflVB9Ommm3kuOoic6IEnLia0rgtb6Hd7NSa40qw/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWjALjvbIjM46OqpruJXrrOFOUOs0DanbZFiakmzFmJy5RzHgibgyicz4uw/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmFPicb0RibgicNywibGHZ3DbSWibaQYiaM1ubF36icvRpznfDJMflwF3NYrvT9WsNt2ySgDic3wngaL5icAcw/640?wx_fmt=png)

**本文往 WGAN-GP 中添加了一个新的正则项**，这个正则项的想法很朴素，就是直接把 L 约束（差分形式）作为正则项，跟 GAN-QP 的判别器多出来的二次项差不多。看论文的曲线图，训练比纯 WGAN-GP 要稳定些。

# 欢迎继续补充

这次的论文清单就这么多了，刚好凑够了十篇。限于笔者阅读量，不排除有疏漏之处，如果还有其他推荐的，欢迎在评论中提出。

# 相关链接

[1] Vivek S.Borkar. Stochastic approximation with two time scales. Systems & Control Letters Volume 29, Issue 5, February 1997, Pages 291-294.
[2] Lars Mescheder, Sebastian Nowozin, Andreas Geiger. The Numerics of GANs. NIPS 2017.
[3] Vaishnavh Nagarajan, J. Zico Kolter. Gradient descent GAN optimization is locally stable. NIPS 2017.

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



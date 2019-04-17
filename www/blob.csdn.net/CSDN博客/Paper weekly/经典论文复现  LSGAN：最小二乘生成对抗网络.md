# 经典论文复现 | LSGAN：最小二乘生成对抗网络 - Paper weekly - CSDN博客





2018年10月10日 12:25:48[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：399









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




过去几年发表于各大 AI 顶会论文提出的 400 多种算法中，公开算法代码的仅占 6%，其中三分之一的论文作者分享了测试数据，约 54% 的分享包含“伪代码”。这是今年 AAAI 会议上一个严峻的报告。 人工智能这个蓬勃发展的领域正面临着实验重现的危机，就像实验重现问题过去十年来一直困扰着心理学、医学以及其他领域一样。**最根本的问题是研究人员通常不共享他们的源代码。**




可验证的知识是科学的基础，它事关理解。随着人工智能领域的发展，打破不可复现性将是必要的。为此，**PaperWeekly 联手百度 PaddlePaddle 共同发起了**[**本次论文有奖复现**](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490664&idx=1&sn=c716049d2a63a4ffef0d25e35f3168fb&chksm=96e9c3e8a19e4afe04f844dc25fee0adbae5345eefe177c611e18b37761d8d9a8aed1769a943&scene=21#wechat_redirect)，我们希望和来自学界、工业界的研究者一起接力，为 AI 行业带来良性循环。




作者丨文永亮

学校丨华南理工大学

研究方向丨目标检测、图像生成




笔者这次选择复现的是 ***Least Squares Generative Adversarial Networks***，也就是 ***LSGANs***。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKzQn93bFJKvVwXP4hBsGI3Naycr0aYic9PnF55lnj6BtzfPPn6yYhQSQ/640)




近几年来 GAN 是十分火热的，由 Goodfellow 在 14 年发表论文 ***Generative Adversarial Nets***[1] 开山之作以来，生成式对抗网络一直都备受机器学习领域的关注，这种两人零和博弈的思想十分有趣，充分体现了数学的美感。从 GAN 到 ***WGAN***[2] 的优化，再到本文介绍的 LSGANs，再到最近很火的 ***BigGAN ***[3]，可以说生成式对抗网络的魅力无穷，而且它的用处也是非常奇妙，如今还被用在例如无负样本的情况下如何训练分类器，例如 ***AnoGAN ***[4]。 




LSGANs 这篇经典的论文主要工作是**把交叉熵损失函数换做了最小二乘损失函数**，这样做作者认为改善了传统 GAN 的两个问题，即传统 GAN 生成的图片质量不高，而且训练过程十分不稳定。




**LSGANs 试图使用不同的距离度量来构建一个更加稳定而且收敛更快的，生成质量高的对抗网络。**但是我看过 WGAN 的论文之后分析这一损失函数，其实并不符合 WGAN 作者的分析。在下面我会详细分析一下为什么 LSGANs 其实并没有那么好用。




**论文复现代码： **




http://aistudio.baidu.com/aistudio/#/projectdetail/25767




# LSGANs的优点




我们知道传统 GAN 生成的图片质量不高，传统的 GANs 使用的是交叉熵损失（sigmoid cross entropy）作为判别器的损失函数。 




在这里说一下我对交叉熵的理解，有两个分布，分别是真实分布 p 和非真实分布 q。




信息熵是![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKkmLymPbgAyxKwsFVu0vtWlKoib0zsngmLhdSRZNCtQRV0oEbBNYqpTQ/640)，就是按照真实分布 p 这样的样本空间表达能力强度的相反值，信息熵越大，不确定性越大，表达能力越弱，我们记作 H(p)。 交叉熵就是![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKyE6nAXudlzI0IYquP4niabZMgAZKe4MAh7Qeicb1SNFtOtSEKyFiba0fQ/640)，可以理解为按照不真实分布 q 这样的样本空间表达能力强度的相反值，记作 H(p,q)。 




KL 散度就是 D(p||q) = H(p,q) - H(p)，它表示的是两个分布的差异，因为真实分布 p 的信息熵固定，所以一般由交叉熵来决定，所以这就是为什么传统 GAN 会采用交叉熵的缘故，论文也证明了 GAN 损失函数与 KL 散度的关系。 




我们知道交叉熵一般都是拿来做逻辑分类的，而像最小二乘这种一般会用在线性回归中，**这里为什么会用最小二乘作为损失函数的评判呢？ **




使用交叉熵虽然会让我们分类正确，但是这样会导致那些在决策边界被分类为真的、但是仍然远离真实数据的假样本（即生成器生成的样本）不会继续迭代，因为它已经成功欺骗了判别器，更新生成器的时候就会发生梯度弥散的问题。 




论文指出最小二乘损失函数会对处于判别成真的那些远离决策边界的样本进行惩罚，把远离决策边界的假样本拖进决策边界，从而提高生成图片的质量。作者用下图详细表达了这一说法：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKfpu5HsUlZ3SEGyUxQ1TMsic28PQku6ma3EiacpxDE6bEjSoXQicO2RenA/640)




我们知道传统 GAN 的训练过程十分不稳定，这很大程度上是因为它的目标函数，尤其是在最小化目标函数时可能发生梯度弥散，使其很难再去更新生成器。而论文指出 LSGANs 可以解决这个问题，**因为 LSGANs 会惩罚那些远离决策边界的样本，这些样本的梯度是梯度下降的决定方向。**




论文指出因为传统 GAN 辨别器 D 使用的是 sigmoid 函数，并且由于 sigmoid 函数饱和得十分迅速，所以即使是十分小的数据点 x，该函数也会迅速忽略样本 x 到决策边界 w 的距离。**这就意味着 sigmoid 函数本质上不会惩罚远离决策边界的样本**，并且也说明我们满足于将 x 标注正确，因此辨别器 D 的梯度就会很快地下降到 0。




我们可以认为，交叉熵并不关心距离，而是仅仅关注于是否正确分类。正如论文作者在下图中所指出的那样，（a）图看到交叉熵损失很容易就达到饱和状态，而（b）图最小二乘损失只在一点达到饱和，作者认为这样训练会更加稳定。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKyYHia5KKpiapxmjb0VNsjP3mH3wfA92JHvF4rgtmfstLUpOxDadbibHFA/640)




# LSGANs的损失函数




**传统 GAN 的损失函数：**




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKv6snnJH1z5dQMqIbd8nDhtRf36P4lDXeibMUhPbcuNYeeSIu0bJCCnw/640)




**LSGANs 的损失函数：**



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKkXQiaHvW7EGLmS5oibE6JXh9VkKxkIJGLIUqjXUuHRWO9eFbkUO2oJhg/640)




其中 G 为生成器（Generator），D 为判别器（Discriminator），z 为噪音，它可以服从归一化或者高斯分布，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKcGIBasBGXd0aX3MbWxjyiaqo5dQRd0XicdGqptaHmFJIeoEJ6ymGUAlw/640)为真实数据 x 服从的概率分布，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhK5Zln3tYVttrMR6uoBNb7WN83jeoJuSwNKfJfmyWLD1D1rEAxDRib6HQ/640)为 z 服从的概率分布。![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKTxPHT4hIuBO8HQyF0m5jTicT8uVeBwdsBW4ibZQhSfvs3j9NFC5D3J6w/640)为期望值，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKdBdseXV5JHoX286cibgRf3rk25lwr6VtsP1hyjZKyk3Eqt8Yia8udNZQ/640)同为期望值。




```python
def generator(z, name="G"):
    with fluid.unique_name.guard(name+'_'):
        fc1 = fluid.layers.fc(input = z, size = 1024)
        fc1 = fluid.layers.fc(fc1, size = 128 * 7 * 7)
        fc1 = fluid.layers.batch_norm(fc1,act = 'tanh')
        fc1 = fluid.layers.reshape(fc1, shape=(-1, 128, 7, 7))


        conv1 = fluid.layers.conv2d(fc1, num_filters = 4*64,
                                    filter_size=5, stride=1, 
                                    padding=2, act='tanh')
        conv1 = fluid.layers.reshape(conv1, shape=(-1,64,14,14))

        conv2 = fluid.layers.conv2d(conv1, num_filters = 4*32, 
                                    filter_size=5, stride=1,
                                    padding=2, act='tanh')
        conv2 = fluid.layers.reshape(conv2, shape=(-1,32,28,28))

        conv3 = fluid.layers.conv2d(conv2, num_filters = 1, 
                                    filter_size=5, stride=1,
                                    padding=2,act='tanh')
#         conv3 = fluid.layers.reshape(conv3, shape=(-1,1,28,28))
        print("conv3",conv3)
        return conv3
```

▲ 生成器代码展示




```python
def discriminator(image, name="D"):
    with fluid.unique_name.guard(name+'_'):
        conv1 = fluid.layers.conv2d(input=image, num_filters=32,
                                    filter_size=6, stride=2,
                                    padding=2)
        conv1_act = fluid.layers.leaky_relu(conv1)

        conv2 = fluid.layers.conv2d(conv1_act, num_filters=64, 
                                    filter_size=6, stride=2,
                                    padding=2)
        conv2 = fluid.layers.batch_norm(conv2)
        conv2_act = fluid.layers.leaky_relu(conv2)

        fc1 = fluid.layers.reshape(conv2_act, shape=(-1,64*7*7))
        fc1 = fluid.layers.fc(fc1, size=512)
        fc1_bn = fluid.layers.batch_norm(fc1)
        fc1_act = fluid.layers.leaky_relu(fc1_bn)

        fc2 = fluid.layers.fc(fc1_act, size=1)
        print("fc2",fc2)
        return fc2
```

▲ 判别器代码展示




**作者提出了两种 abc 的取值方法：**




1. 使 b - c = 1，b - a = 2，例如 a = -1，b = 1，c = 0：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKFflspxQ8Via8ibpJFbb4KDBSKJOZk1k8Y4kWIefiawqFsTVg8AQJSBExw/640)




2. 使 c = b，用 0-1 二元标签，我们可以得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKlgogaibNOzJicyTeV3lAibeY0LHRLQI5xPyjtNC8wz0jQ7qw8UNVy7pew/640)




作者在文献中有详细推倒过程，详细说明了 LSGAN 与 f 散度之间的关系，这里简述一下。




通过对下式求一阶导可得到 D 的最优解：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKLO2drnGA7fmOianedCr0SyvWa3BzgwCtwl9kJwb7ZXicxZ85IgibWYnew/640)




代入：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhK9zZKZQqYo5h5CpsGic2bHpDp5HoTxEaZgFP5Hyra1CwrAN1HvS9BgLA/640)




其中另加项![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKfSytR7MZZibiaialVEwICiaQ8TsP7EIY2iadC2CAQKIDMWHsmELLCoAgDTQ/640)并不影响![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhK0UNhicia1gHEia0j9GHCbXfxC3hK5diaQafdauwxp0O7kaicwJWdibA9Nanw/640)的值，因为它不包含参数 G。




最后我们设 b - c = 1，b - a =2 就可以得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKeDNoGnaYLqz6EeP7zqyPGO0qwxnh3ID4rgiacWo0xpibEqRkcsx5uyRQ/640)




其中![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKibEk40TZnyTtqwTVZPiaHkiasibXIRXd051gf6T4vOoZibicaXhGXDDMV91g/640)就是皮尔森卡方散度。




# LSGANs未能解决的地方




下面我会指出 LSGANs 给出的损失函数到底符不符合 WGAN 前作的理论。关于 WGAN 前作及 WGAN 论文的分析可以**参考本文**[5]。




上面我们指出了 D 的最优解为公式（5），我们最常用的设 a=-1，b=1，c=0 可以得出：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKib2hia3nzYfb5dHjogxiaUPXgpiaObZ8QumgOcpwz0l6ibdnck9BbibR1bzg/640)




把最优判别器带入上面加附加项的生成器损失函数可以表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKvTwHhBo4WHcD6u93P3nwniaQwzJHicrQIAjRXk64jA8qHkic90FpI2lzQ/640)




也就是优化上面说的皮尔森卡方散度，其实皮尔森卡方散度和 KL 散度、JS 散度有一样的问题，根据 WGAN 给出的理论，下面用 P1，P2 分别表示![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhK0ohbJsDTYaP5G6Iqhz62YnWOcvyW9510opQgq1mTzm4rqvWApgYGdQ/640)和![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKdzTU374eNmd68AdiaIb81A8SI8ia87EftP6VdURttzjA0EQgXHfPlAqg/640)。




当 P1 与 P2 的支撑集（support）是高维空间中的低维流形（manifold）时，P1 与 P2 重叠部分测度（measure）为 0 的概率为 1。也就是 P1 和 P2 不重叠或重叠部分可忽略的可能性非常大。




**对于数据点 x，只可能发生如下四种情况：**




1. P1(x)=0,P2(x)=0




2. P1(x)!=0,P2(x)!=0




3. P1(x)=0,P2(x)!=0




4. P1(x)!=0,P2(x)=0




可以想象成下面这幅图，假设 P1(x) 分布就是 AB 线段，P2(x) 分布就是 CD 线段，数据点要么在两条线段的其中一条，要么都不在，同时在两条线段上的可能性忽略不计。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhK62riawdOFAnLtHuaoOJEfuEplIUgzZuibLlyo7lEwvhgiaykNZooamPpA/640)




情况 1 是没有意义的，而情况 2 由于重叠部分可忽略的可能性非常大所以对计算损失贡献为 0，情况 3 可以算出 D*=-1，损失是个定值 1，情况 4 类似。




所以我们可以得出结论，当 P1 和 P2 不重叠或重叠部分可忽略的可能性非常大时，当判别器达到最优时，生成器仍然是不迭代的，因为此时损失是定值，提供的梯度仍然为 0。同时我们也可以从另一个角度出发，WGAN 的 Wasserstein 距离可以变换如下：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKgJrgqQIQl9fYIV7qicOtw5vD9ibrVfHwIWib7JIdNFLwwMicXjL5mydIng/640)




它要求函数 f 要符合 Lipschitz 连续，可是最小二乘损失函数是不符合的，他的导数是没有上界的。所以结论就是 LSGANs 其实还是未能解决判别器足够优秀的时候，生成器还是会发生梯度弥散的问题。




# 两种模型架构和训练




**模型的结构**




作者也提出了两类架构：




**第一种处理类别少的情况**，例如 MNIST、LSUN。网络设计如下：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKDDAEdGhFzpUOOFFTsXRlRlCh4GjItoy9NShg8jAFhsJJRMnKsVzxnw/640)




**第二类处理类别特别多的情形**，实际上是个条件版本的 LSGAN。针对手写汉字数据集，有 3740 类，提出的网络结构如下：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKpPkdYGkgMmrpQ7WMesg48TckfSiaKZhibiaWc5VUomMfK3jeL91wBtfVg/640)




**训练数据**




论文中使用了很多场景的数据集，然后比较了传统 GANs 和 LSGANs 的稳定性，最后还通过训练 3740 个类别的手写汉字数据集来评价 LSGANs。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKed6qeJaBG06dhg4icZytWlwK5jHe6IK2QWf9HKjgmUD4BvibnRlI7qPg/640)

▲ 本文使用的数据集列表




在 LSUN 和 HWDB1.0 的这两个数据集上使用 LSGANs 的效果图如下，其中 LSUN 使用了里面的 bedroom, kitchen, church, dining room 和 conference room 五个场景，bedroom 场景还对比了 DCGANs 和 EBGANs 的效果在图 5 中，可以观察到 LSGANs 生成的效果要比那两种的效果好。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKExaQ5z8PnUX313Km7BODpQuAZqxdW3lCamXJiaZXPAr2IJSduU4W7NQ/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKcFgcoszrtToea9jfEN2icqQw4NibDsvgaHp5Wj6qo491geXXficfGxjeQ/640)




图 7 则体现了 LSGANs 和传统 GANs 生成的图片对比。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKRKD2qKyujeMqKbXhMz0xBrePVFZQUIML4cibicLLm2Dq7clGwfy5lUMw/640)




**通过实验观察，作者发现 4 点技巧：**




1. 生成器 G 带有 batch normalization 批处理标准化（以下简称 BN）并且使用 Adam 优化器的话，LSGANs 生成的图片质量好，但是传统 GANs 从来没有成功学习到，会出现 mode collapse 现象；




2. 生成器 G 和判别器 D 都带有 BN 层，并且使用 RMSProp 优化器处理，LSGANs 会生成质量比 GANs 高的图片，并且 GANs 会出现轻微的 mode collapse 现象；




3. 生成器 G 带有 BN 层并且使用 RMSProp 优化器，生成器 G 判别器 D 都带有 BN 层并且使用 Adam 优化器时，LSGANs 与传统 GANs 有着相似的表现；




4. RMSProp 的表现比 Adam 要稳定，因为传统 GANs 在 G 带有 BN 层时，使用 RMSProp 优化可以成功学习，但是使用 Adam 优化却不行。




下面是使用 LSGANs 和 GANs 学习混合高斯分布的数据集，下图展现了生成数据分布的动态结果，可以看到传统 GAN 在 Step 15k 时就会发生 mode collapse 现象，但 LSGANs 非常成功地学习到了混合高斯分布。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKGQxWibrUrU0eTExcaTReJ5Vr78oWNL5XD3YWCy6YYS4oruIuCVD1ToA/640)




# 论文具体实现




笔者使用了 MNIST 数据集进行实验，具体实现效果如下：





**LSGANs：**




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhKbnZdUVFVjhaqOncMcF5diahPyqwUfzH4OFVibQyhadDiaQ5qNDFtft6Ew/640)




**GAN：**



![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm0Gu8qgcvmjTSMWyutOJhK0YMBfTqqJ34mnsEmBTeJNBn4uYTM80hEZA3OuaWicyovTJgnEia2dZgQ/640)




从本次用 MNIST 数据训练的效果来看，LSGANs 生成的效果似乎是比 GAN 的要清晰高质量一些。




# 总结




LSGANs 是对 GAN 的一次优化，从实验的情况中，笔者也发现了一些奇怪的现象。我本来是参考论文把判别器 D 的损失值，按真假两种 loss 加起来一并放入 Adam 中优化，但是无论如何都学习不成功，梯度还是弥散了，最后把 D_fake_loss 和 D_real_loss 分为两个 program，放入不同的 Adam 中优化判别器D 的参数才达到预期效果。




这篇论文中的思想是非常值得借鉴的，从最小二乘的距离的角度考量，并不是判别器分类之后就完事了，但是 LSGANs 其实还是未能解决判别器足够优秀的时候，生成器梯度弥散的问题。




# 关于PaddlePaddle




**笔者反馈：**帮助文档有点少，而且我本来就直接写好了想改成使用 GPU 运算，没找到怎么改；




**PaddlePaddle团队：**关于如何使用 GPU 运行，可以看下执行器 Executor（单 GPU 或单线程 CPU 执行器）或 ParallelExecutor（多 GPU 或多线程 CPU 执行器，也可以单 GPU/线程 CPU 执行）的文档，前者指定 place 为 CUDAPlace，后者接口有个 use_cuda，具体请参考文档。也可以看 models repo 例子，比如 image_classification 或 text_classification 的例子。 




**笔者反馈：**Program 这个概念有点新颖，一个模型可以有多个 Program，但是我实现的 GAN 可以只用一个，也可以分别放进三个 Program，没有太了解到 Program 这个概念的优越之处，我还是像计算图那样使用了，官方也没给出与 TensorFlow 的对比。




**PaddlePaddle团队：**关于 Program 设计可以参考官方文档。这里提一点，在用户使用的直观感受中和 TensorFlow graph 不同的是，凡是放在一个 Program 里 op，只要运行该 Program，这些 op 就都会执行；而 TensorFlow，指定一个 variable，只运行以该 variable 为叶子节点的 graph，其他多余 node 不执行，这是最大的用户感受到的区别。 




至于一个 Program 还是多个 Program，看用户使用需求而定，多个 Program 时要注意的东西就比较多，例如是否要参数共享等，当然运行多次的时间代价也稍多。 如果是 GAN 也可以参考 models repo 的例子。




**小道消息：听说全新版本的 PaddlePaddle 已于今日发布哦。**



# 参考文献




[1]. I. Goodfellow, J. Pouget-Abadie, M. Mirza, B. Xu, D. Warde-Farley, S. Ozair, A. Courville, and Y. Bengio, “Generative adversarial nets,” in Advances in Neural Information Processing Systems (NIPS), pp. 2672–2680, 2014.

[2]. M. Arjovsky, S. Chintala, and L. Bottou. Wasserstein GAN. arXiv preprint arXiv:1701.07875, 2017.

[3]. Andrew Brock, Jeff Donahue and Karen Simonyan. Large Scale GAN Training for High Fidelity Natural Image Synthesis. arXiv:1809.11096, 2018.

[4]. Schlegl, Thomas, et al. "Unsupervised Anomaly Detection with Generative Adversarial Networks to Guide Marker Discovery." arXiv preprint arXiv:1703.05921 (2017).

[5]. https://zhuanlan.zhihu.com/p/25071913?from_voters_page=true




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击标题查看更多论文解读：**




- 
[网络表示学习综述：一文理解Network Embedding](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490862&idx=1&sn=707fe122dfc5d961a22771111514fada&chksm=96e9c2aea19e4bb8755d6759dd8e70cb44d2da2c454947395dede0268f511c140441cd1fb5ce&scene=21#wechat_redirect)

- 
[神经网络架构搜索（NAS）综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491248&idx=1&sn=d2946d8a37f7c6567b1a767a497006fb&chksm=96e9c130a19e48267f72ad32c527ec4a1697741e409d865d9233c5d7035a1f66a59b5e40792d&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)


- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)[ECCV 2018 | 从单帧RGB图像生成三维网格模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491465&idx=1&sn=a6871b72d460debf90d2daa0bed719c8&chksm=96e9c009a19e491f8c247f36a53bad0a6812c462f3fb5b2d9ae74b38be673b946b82a4b44330&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)

- 
[ECCV 2018 | 腾讯AI Lab提出视频再定位任务](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491347&idx=1&sn=e1f2cc16c9fcfcc5d2935118f09ee094&chksm=96e9c093a19e49855931cf621ec7f715c1d2dd5041e3343bc311aea2e5069ae03aaa1367e8f9&scene=21#wechat_redirect)

- 
[KDD 18 | 斯坦福大学提出全新网络嵌入方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491364&idx=1&sn=afa577a1bebfd148ac20cde05a927e24&chksm=96e9c0a4a19e49b2458f451c6e201277c509905201a1f751b30fbfa1e2ba31bc2711b2ca1b7a&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。




📝 **来稿标准：**

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

▽ 点击 | 阅读原文| 收藏复现代码





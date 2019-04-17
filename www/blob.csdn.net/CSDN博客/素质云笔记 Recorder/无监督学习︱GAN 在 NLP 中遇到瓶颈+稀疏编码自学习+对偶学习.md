# 无监督学习︱GAN 在 NLP 中遇到瓶颈+稀疏编码自学习+对偶学习 - 素质云笔记/Recorder... - CSDN博客





2017年01月18日 10:35:38[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6695








> 
深度学习训练一个模型需要很多的人工标注的数据。例如在图象识别里面，经常我们可能需要上百万的人工标注的数据，在语音识别里面，我们可能需要成千上万小时的人工标注的数据，机器翻译更是需要数千万的双语句对做训练。 

  看看标注数据的代价有多高。比如说对机器翻译而言，现在如果我们请人工来翻译，一个单词的费用差不多是5—10美分之间，一个句子平均长度差不多是30个单词，如果我们需要标注一千万个双语句对，也就是我们需要找专家翻译一千万句话，这个标注的费用差不多是2200万美元。


# 一、GAN在NLP中遇到瓶颈

> 
GAN 

  自从被提出以来，就广受大家的关注，尤其是在计算机视觉领域引起了很大的反响，但是这么好的理论是否可以成功地被应用到自然语言处理（NLP）任务呢？


## Ian Goodfellow 博士

一年前，网友在 reddit 上提问道，生成式对抗网络 GAN 是否可以应用到自然语言处理上。GAN 理论的提出者，OpenAI 的科学家，深度学习理论奠基人之一  Yoshua Bengio 的得意门生 Ian Goodfellow 博士回答了这个问题：

**GANs 目前并没有应用到自然语言处理（NLP）中，因为 GANs 仅仅定义在真值数据中，GANs 通过训练出的生成器来产生合成数据，然后在合成数据上运行判别器，判别器的输出梯度将会告诉你，如何通过略微改变合成数据而使其更加现实。**

**只有在数据连续的情况下，你才可以略微改变合成的数据，而如果数据是离散的，绝对不可以改变合成数据，一点都不可以**。

例如，如果你输出了一张图片，其像素值是1.0，那么接下来你可以将这个值改为1.0001。如果你输出了一个单词“penguin”，那么接下来就不能将其改变为“penguin + .001”，因为没有“penguin +.001”这个单词。如果想改的话，你必须将“penguin”变为“ostrich”或其他。因为所有的自然语言处理（NLP）的基础都是离散值，如“单词”、“字母”或者“音节”，没有人真正知道怎样才能在 NLP 中应用 GANs。

一般而言，我们会想到采用增强学习算法，但是增强算法的运行效果并不十分理想。目前据我所知，还没有人真正的开始研究利用增强算法解决 NLP 问题。

我看到有人说， GANs 在递归神经网络（RNN）方面并不奏效。这是不对的。从理论上来看，GANs 和 RNN 的生成器或判别器之间，并没有什么矛盾。但是，对于这一点，目前并没有人严肃而又认真的测试过。因此，在实际应用中还是存在一定的困难的。

顺便说一下，VAEs 对可见的离散单元是有效的，但是对隐藏的离散单元却并不奏效（除非你在运用增强算法，比如 DARN 或者 NVIL）。而另一方面，GANs  对隐藏的离散单元奏效，对可见的离散单元却并不奏效（从理论上来讲，除非是运用增强算法）。因此，这两种方法可以说是各有利弊，相辅相成。

2016年的 NIPS GAN Workshop 中，来自杜克大学的 Zhang、Gan 和 Carin 发表了一篇题为 GeneratingText via Adversarial Training 的论文，尝试将 GAN 理论应用到了文本生成任务上，他们的工作非常有特色，具体可以总结为：

用到的判别器（Discriminator）是卷积神经网络（CNN），而不是递归神经网络（RNN），这可能是一个不错的选择，因为Tong Zhang 就曾经使用CNN 做文本分类任务，相比 RNN，CNN 更好训练一些，最终训练得到的判别器非常有效，与之相关的问题优化起来也相对容易些。

在生成器（generator）中用光滑近似（smoothapproximation）的思路来逼近 LSTM 的输出，但实际上，这种思想比较常见，并没有什么不同寻常的地方。

在鞍点优化问题上，采用的是纯矩匹配（moment matching）作为优化准则。早期的生成式对抗网络（GANs）都是用逐点判别损失（pointwise discrimination loss）作为优化目标的，而最近的工作都是用类矩匹配的思路来加强优化目标，这里的优化是用矩匹配来做。

本文的初始化非常有意思，特别是在判别器的预训练方面，利用原始的句子和该句子中交换两个词的位置后得到的新句子进行判别训练。（在初始化的过程中，运用逐点分类损失函数对判别器进行优化）。这非常有意思，因为将两个单词互换位置，输入的数据信息实际上是基本相同的。比如，大多数卷积计算最终会得出完全相同的值。

更新生成器的频率遥远高于判别器，这与大家之前的设想正好相反。或许这是因为，相比 LSTM 来说， CNN 的问题优化要容易的多。同时，这可能也和纯矩匹配损失的应用有关。

. 

.

## 知乎大神的经验

知乎上大家对这个问题的看法有很多，下面列出两个比较有代表性的：

```bash
Xun Huang PhD Student in CS, Cornell
```

其实本来写了一大段后来还是删了…. 因为这个问题其实非常前沿，在知乎上要讲清楚的话感觉太难了。

所以还是就列一些 paper 吧：

```
SeqGAN: Sequence Generative Adversarial Nets with Policy Gradient
GANS for Sequences of Discrete Elements with the Gumbel-softmax Distribution
Generating Text via Adversarial Training
```

以上 paper 是直接用 GAN 做 text generation 的，但是其实都暂时没有啥正经的实验结果。

```java
Modeling documents with Generative Adversarial Networks
```

这个是用 GAN 生成一个 document 中词的分布（不是真正 generate document），然后希望 discriminator 能学到好的 feature without supervision。

```php
Professor Forcing: A New Algorithm for Training Recurrent Networks
```

这个是用 discriminator 来 encourage 一个 RNN 在 training 和 testing 的时候  hidden state 的分布一致，借此解决 exposure bias 的问题（即 RNN 在  training 时接受 ground truth input，但 testing 时却接受自己之前的 output，这两个 setting不一致会导致 error accumulate）。

同时还有一系列 paper 用 reinforcement learning 来直接 optimize evaluation metric（例如BLEU），同时解决 exposure bias，这些 paper 其实和 GAN 有内在的联系（参考 Connecting Generative Adversarial Networks andActor-Critic Methods）：

```
Sequence Level Training with Recurrent Neural Networks
An Actor-Critic Algorithm for Sequence Prediction
Optimization of image description metrics using policy gradient methods
```

最后个人不认为 discrete data space 是 GAN 在 NLP上难 work 的本质原因…这里不展开了。

王亭午 U of T ML group

![这里写图片描述](https://img-blog.csdn.net/20170118103108433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

补充几点，题主也许会好奇：为什么 Ian 在 reddit 上说 GAN 做不了是因为  word embedding 加减无意义就做不了呢？既然这样，我在 latentvector 上做加减不就行了吗？这个方法看上去可以，实际上很难 work。

使用 generative model 解决 language generation 最大的问题在于 latent space  存在非常多的 desert hole。在 training 的时候，text 的 latent vector 有聚拢的倾向( citation needed，感谢评论。评论里面也提到了desert hole 这个词并不是一个学术上的词汇。David Duvenaud 和我们聊这个问题的时候，用了这个说法，这里沿用，感觉还是满形象的哈哈)。

因此直接上 GAN model 存在一些问题。图中是[1] 里面的一个例子。在 latent space 遨游的时候，中间的句子不 make sense。

不过解决的方法也是有很多的。最简单的方法是用 VAE 而不是用 GAN。GAN  本身的训练方式是非常依赖连续空间的。在训练的时候，我们的目标就是连续空间上的 pixel 值。在这一点上，VAE 就没有这个假设。因此 VAE 是自然的选择。

实际上用的时候有很多 tricks，[1] 里面感觉就有很多工程上的东西来减少  desert hole 的问题。

![这里写图片描述](https://img-blog.csdn.net/20170118103130433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

另外一个方法是结合 policy gradient，把它做成一个 R L的问题。[2] 是一篇非常有意思的文章。通过把 word 选择由 softmax output 选择变成 policy 选择，作者巧妙的避开了 GAN 和 word embedding 不兼容的问题。当然实际上，结合  GAN 和 RL 需要更加多的思考和技巧。[2]不一定是最好的方法，但是无疑证明了GAN是可以用在 sentence generation 这个问题上的。

我之前也很关注 GAN 和 text 的结合，也可以算是利益相关吧。此外有另外一个工作[3] (出自我们组去年刚刚招来的青年才俊Prof. David Duvenaud)，可能可以带来更加多的启示。

如果我们考虑化学物质的预测呢？假设我们知道化学式A，B，C并且知道他们的性质，我们能不能预测 A-B+C 的化学性质呢？我们能不能得到类似 queen-woman+man=king 的结果呢？

这个时候，使用 generative model 解决化学分子生成会遇见和 sentence generation 一样的问题。我们会发现，化学分子的 latentspace，一样存在  desert holes。推荐你看一下[3]，我觉得它很有意思，能给我们考虑的问题带来许多思考 。

. 

.

## 参考文献

[1] Generating Sentences froma Continuous Space. 

Samuel R. Bowman, Luke Vilnis, Oriol Vinyals, Andrew M. Dai, Rafal Jozefowicz,Samy Bengio 
[https://arxiv.org/abs/1511.06349v4](https://arxiv.org/abs/1511.06349v4)

[2] SeqGAN: Sequence Generative Adversarial Nets with Policy Gradient.

Lantao Yu, Weinan Zhang, Jun Wang, Yong Yu 
[https://arxiv.org/abs/1609.05473v5](https://arxiv.org/abs/1609.05473v5)

[3] Automatic chemical design using a data-driven continuous representation ofmolecules.

Rafael Gómez-Bombarelli, David Duvenaud, José Miguel Hernández-Lobato, JorgeAguilera-Iparraguirre, Timothy D. Hirzel, Ryan P. Adams, Alán Aspuru-Guzik 
[https://arxiv.org/abs/1610.02415v1](https://arxiv.org/abs/1610.02415v1)

翻译自微信公众号AI100 

原文链接：[http://www.machinedlearnings.com/2017/01/generating-text-via-adversarial-training.html](http://www.machinedlearnings.com/2017/01/generating-text-via-adversarial-training.html)

.

# 二、稀疏编码自学习——SelfTaughtLearning

> 
本文来源：[译文 | 

  从未标记数据中迁移学习](http://mp.weixin.qq.com/s?__biz=MzI4MTQ2NjU5NA==&mid=2247484188&idx=1&sn=1544311d17f38bc17253d643ef3a3766&chksm=eba98091dcde0987626a273a2defcf3c4e7db7566ba2f521ab84644bd3c563e80c7fc1518a55&mpshare=1&scene=1&srcid=03144iFAFiL8WU3YAorupsvL#rd)

原文：[http://robotics.stanford.edu/~rajatr/papers/icml07_SelfTaughtLearning.pdf](http://robotics.stanford.edu/~rajatr/papers/icml07_SelfTaughtLearning.pdf)

  有点类似先把图像向量化（边缘特征）之后进行聚类来寻找某一类别的内容，少量标记就可以获得更多的标记文件了。

我们发现许多从网上随机下载的图像也包含与大象和犀牛类似的基本视觉模式（如边缘）。利用无标记数据的方法来提高监督学习的效能。

半监督学习（Nigam et al.2000），本文算法也使用标记和未标记数据，但是和典型文献中的半监督学习方法不同，我们并不假设无标记数据能够分配到监督学习的类别标签。为了区别于这种半监督学习，我们称我们的任务为自学习。

**自学习方法主要包括以下两个阶段：**
- 首先使用无标记数据来学习一种表达，然后将此种表达应用于标记的数据以及分类任务中。
- 一旦在第一阶段学习到这种表达，它也可以重复应用于不同的分类任务。例如在上例中，一旦从互联网图片学习到这种表达，则不仅能够将其应用在大象和犀牛的图像中，也可以应用在其他图像分类任务。 

实验证明稀疏编码特征，或许与原始特征相结合，在大部分领域中都比只使用原始特征和PCA特征效果要好得多。

稀疏编码基向量从随机自然灰度图像块（14×14像素）学习得到。在方格里面的每个正方形表达一个基向量。

![这里写图片描述](https://img-blog.csdn.net/20170401091011446?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面两个表格展示了不同应用领域上的实验结果。 
![这里写图片描述](https://img-blog.csdn.net/20170401091239437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
# 三、对偶学习 Dual learning

来源于微软研究院AI头条：[秦涛：深度学习的五个挑战和其解决方案](http://mp.weixin.qq.com/s?__biz=MzIxMjAzNDY5Mg==&mid=2650791403&idx=1&sn=67adeeec7bb35d361c50a0582d91b0a0&chksm=8f474800b830c116394491d090a38b5fbfbf90e64b56122d25ff2f4a726601fdf071676559b7&mpshare=1&scene=1&srcid=0313n0yY3IDSUabLodaRAqeN#rd) 以及 [对偶学习在行动：从文本到图像](https://mp.weixin.qq.com/s?__biz=MzI5NzAzODcyNw==&mid=2454897857&idx=1&sn=5b4b1321a50bd15d8b9ef3f7aac94a04&chksm=fb1d4229cc6acb3f87daabeb9fbf75e45991896cc0ec2cbb9085ad3d2a00711bff830a46866c&mpshare=1&scene=1&srcid=0403CnaKUSYpTPPwtB5hmjly#rd)

对偶学习的思路和前面生成式对抗学习会非常不一样。对偶学习的提出是受到一个现象的启发：我们发现很多人工智能的任务在结构上有对偶属性。比如说在机器翻译里面，我们把中文翻译成英文，这是一个任务，但是我们同样也需要把英文翻译成中文，这是一个对偶的任务。这种原任务和对偶任务之间，他们的输入和输出正好是反着来的。在语音处理里面，语音识别是把语音转化成文字，语音合成是把文字转化成语音，也是互为对偶的两个任务。在图像理解里面，看图说话，也就是给一张图生成一句描述性的语句，它的对偶任务是给一句话生成一张图，这两个任务一个是从图像到文本，另外一个是从文本到图像。 

在机器翻译里面做了一些实验，发现通过对偶学习的过程，我们只需要用10%标注的数据（大概100万英法双语句对），再加上很多没有标注的数据，达到用100%标注数据（1200万英法双语句对）训练的模型的准确度。

偶学习这个新范式去年掀起了一个小波澜，去年11月微软亚研院发的NIPS，原文链接：[https://arxiv.org/pdf/1611.00179.pdf](https://arxiv.org/pdf/1611.00179.pdf)

但是慢慢沉寂了，方法的idea不错，但没有得到大规模推广。去年的是在文本翻译这块，图像去年的热门是GAN网络。好了，今天这文章把对偶学习渗透到了图像了。 

看论文我喜欢把一些来龙去脉搞清楚。每一个idea都不是凭空而来的，追本溯源，我们看看去年秦涛博士他们文章的主要精髓。一句话说：一个英语句子A，首先用一个网络n1翻译成法语w，然后将翻译好法语w用另一个网络n2翻译成英语句子B。两个网络合在一起成为一个对偶网络。损失函数就是A与B之间的差值。文章说英语翻译成法语的精度可以匹敌neural machine translation (NMT)的精度。我想说，首先英语跟法语比较像，这个问题相对简单一些；其实我的担心是，这个loss不太好学，很有可能不收敛。文章说他们的学习方法是网络n1教网络n2，网络n2教网络n1，使用一种增强学习的方法。这个是可以的，但这个老师感觉是变化的哦。 

今天的这篇文章名字是：Unpaired Image-to-Image Translation using Cycle-Consistent Adversarial Networks。使用有回路的对抗网络来做未配对的图片-图片的翻译。首先来谈谈对抗网络。深度生成对抗网络是给一批图片，如猫的图片，一个网络学习这些图片用于生成猫，一个已有的判别网络，判断生成的猫是不是真。
我们一起来解开她的扣子。所有网络，不管多深，展开拉直了都是一个方程。方程G将X转变成Y，方程F将Y转变成X1。它搞了两个loss来训练两个网络，一个loss是x-F(G(x)),一个loss是y-G(F(y))。怎么样？其实就是对偶学习。 

文章中讲了如何实现这样的网络的，并做了不少对比实验。 

文章的源码地址：[https://junyanz.github.io/CycleGAN/](https://junyanz.github.io/CycleGAN/)

对比网络CoGAN的源码：[https://github.com/mingyuliutw/CoGAN](https://github.com/mingyuliutw/CoGAN)

这些源码我还没来的及看。 

最后这个方法的用处很广，可以将油画变成照片，可以将普通照片变成单反相机的大光圈一样的北京虚化，可以只改变图片中的物体，如把马变成斑马，可以进行季节变更。 

看看作者到底干了什么事情。附上效果图： 
![这里写图片描述](https://img-blog.csdn.net/20170403210123613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 延伸一：VAE for NLP

GANs在图像中作用比较强，但是在NLP不行，NLP里面VAE则是”霸主“，本节来源于paperweekly分享：[PaperWeekly 第二十七期 | VAE for NLP](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247484093&idx=1&sn=bea0ab9171f2e1ad581ca7869e590dca&chksm=96e9dd3da19e542b17343b0ea30543bd262e92ce26f74491bd509223f5b5d853459981d0f4c7&mpshare=1&scene=1&srcid=0304LAL0E4U3lkkVlwhvCBgJ#rd)

VAE 的核心：

1) 如上图所示，VAE 可以看做是 Standard autoencoder 的 regularized version（在 autoencoder 的架构上引入随机 latent variable）

2) VAE 从 data 学到的是在 latent space 的 region，而不是单个点。换句话说是 encode 学到了一个概率分布 q(z|x)

3) 引入 KL divergence 让后验 q(z|x)接近先验 p(z)。这里的 motivation 在于如果仅用 reconstruction loss，q(z|x)的 variances 还是会很小（又和原有的单个点差不多了）

VAE 详细推导这里就不展开，各种 tutorial 也非常多。只要掌握变分推断和理解 reparametrization trick 就基本 ok 了。

.

## 延伸二：[用条件对抗式自动编码器进行人脸老化、退龄](https://mp.weixin.qq.com/s?__biz=MzA4NzE1NzYyMw==&mid=2247488552&idx=3&sn=2f4ef5c40b7b4b6ed0f4a1d70f3bddc3&chksm=903cea30a74b6326009fc65d8f1c977aa272a55f77e2fcda1096ab0b00090f2e02e9acdff05f&mpshare=1&scene=1&srcid=0403cWtLyzoqlDEeVUbG6GM3#rd)

该代码是对论文《用条件对抗式自动编码器进行人脸老化、退龄》中算法的Tensorflow实现 

数据库

·        FGNET 

·        MORPH 

地址：[https://ebill.uncw.edu/C20231_ustores/web/product_detail.jsp?PRODUCTID=8](https://ebill.uncw.edu/C20231_ustores/web/product_detail.jsp?PRODUCTID=8)
·        CACD 

地址：[http://bcsiriuschen.github.io/CARC/](http://bcsiriuschen.github.io/CARC/)

·        UTKFace (可以从Github 或维基百科获得)

准备训练数据

你可以使用任何带年龄标签和性别标签的数据库。在该demo中，我们使用了UTF人脸数据库，因为用这种标定并裁剪过的人脸照片更好一些。请保存并解压UTKFace.tar.gz到文件夹data下。

训练

$ python main.py

训练过程在NVIDIA TITAN X (12GB)上进行了测试。在UTK人脸数据库（23，708张图像，大小为128x128x3）上进行50次epoch的训练时间是两个半小时。

在训练过程中，会建立一个新文件夹save，包括四个子文件夹：summary, samples, test,和checkpoint

·        samples ：保存每个epoch之后重建的人脸。 

·        test ：保存每个epoch之后的测试结果（基于输入人脸生成的不同年龄的人脸）。 

·        checkpoint ：保存模型。 

·        summary ：保存批损失和中间输出。
用以下命令来可视化summary：

$ cd save/summary  $ tensorboard –logdir .

训练之后，可以检查文件夹samples和test来分别可视化重建和测试性能。下图展示了重建（左）和测试（右）的结果。重建结果（左）的第一行是测试样例，他们分别对应的测试结果（右）由上到下，按年龄增长顺序排列。 
![这里写图片描述](https://img-blog.csdn.net/20170403211534789?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)










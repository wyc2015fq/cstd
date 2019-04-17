# 【深度学习】GAN生成对抗网络原理详解（1） - zkq_1986的博客 - CSDN博客





2018年09月25日 09:54:17[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：957








一个 GAN 框架，最少（但不限于）拥有两个组成部分，一个是生成模型 G，一个是判别模型 D。在训练过程中，会把生成模型生成的样本和真实样本随机地传送一张（或者一个 batch）给判别模型 D。判别模型 D 的目标是尽可能正确地识别出真实样本（输出为“真”，或者1），和尽可能正确地揪出生成的样本，也就是假样本（输出为“假”，或者0）。**这两个目标分别对应了下方的目标函数的第一和第二项。**而生成模型的目标则和判别模型相反，就是尽可能最小化判别模型揪出它的概率。这样 G 和 D 就组成了一个 min-max game，在训练过程中双方都不断优化自己，直到达到平衡——双方都无法变得更好，也就是假样本与真样本完全不可区分。

![](http://img.mp.sohu.com/upload/20170527/4e02d7a5b7df48ea9ebe92eeb14e8859.png)

**通过这样的巧妙设计，GAN 就拥有了一个非常吸引人的性质。**GAN 中的 G 作为生成模型，不需要像传统图模型一样，需要一个严格的生成数据的表达式。这就避免了当数据非常复杂的时候，复杂度过度增长导致的不可计算。同时，它也不需要 inference 模型中的一些庞大计算量的求和计算。它唯一的需要的就是，一个噪音输入，一堆无标准的真实数据，两个可以逼近函数的网络。

![](http://img.mp.sohu.com/upload/20170527/f2be68390efb4047a71617f417f42d5f_th.png)

**训练难点一：过于自由**

但是天下没有免费的午餐，这样简单的要求使得 GAN 的自由度非常大。换句话说，GAN 的训练就会很容易失去方向，变得野蛮生长。于是，早期的 GAN 经常出现如下让人崩溃的现象：

![](http://img.mp.sohu.com/upload/20170527/641a6ed2e63340f6ba9f2d7d1686978a_th.png)

这些现象其实是 GAN 中存在的三个难点或者说问题交织导致的。个人觉得，**首当其中的难点一就是，深度神经网络自身的难训练和不稳定。**虽然原始 GAN 的理论中，并不要求 G 和 D 都是神经网络，只需要是能拟合相应生成和判别的函数就可以。但是这恰好是深度神经网络擅长的事情嘛，所以大家就都用神经网络作为 G 和 D 了。但是神经网络的选择、设计和训练还充满了艺术的惊喜与生活的不确定性，这也直接为 GAN 的训练带来了困难。加之本来 GAN 就缺乏指导，所以就有了一系列可以被归结为解决这一方向问题的工作。我将这类工作概括为 Partial Guidance, Fine-Grained Guidance 和 Special Architecture。

![](http://img.mp.sohu.com/upload/20170527/0d9cfdfe7262423c9fefbae383b553db_th.png)

**解决方案一：Partial Guidance**

先来看，Partial Guidance。Partial Guidance 中我概括的几个重要工作，都是为原始 GAN 加上一些显式的外部信息，比如用户的额外输入，比如类别信息等等。包含的工作有：

![](http://img.mp.sohu.com/upload/20170527/450a53064e8342a7a9da8cf49fc07dc4_th.png)

**Conditional GAN[15]，也叫 CGAN，几乎是原始 GAN[2] 后的第一份工作**，想法非常简单，既然你的信息不够，我就把你原始的生成过程变成基于某些额外信息的生成。这样就相当于给你提供了一些 hint，所以公式如下：

![](http://img.mp.sohu.com/upload/20170527/5da947b3c1594f068cb6c262a7595730.png)

可以看到，D 和 G 去拟合的分布都变成了条件概率分布。在 CGAN 的工作中，这个额外的 y 信息，是通过在输入层直接拼接样本与 y 信息的向量而实现的。具体使用的 y 信息有 one-hot vector，也有图像（也就是基于另一个图像去生成）。这个 y 信息的选择其实十分灵活，在后期的工作中也依然很常见，毕竟是一种非常直观有效的加入 label 信息的方式。

第二个这方面的工作是由 OpenAI 提出的 Improved GAN[19]，其中重点提出了两个训练 GAN 的技巧，feature matching 和 minibatch discrimination。feature matching 是指，既然 G 和 D 的训练不够稳定，常常 D 太强，G 太弱，那么不如就把 D 网络学到的特征直接“传”给 G，让 G 不仅能知道 D 的输出，还能知道 D 是基于什么输出的。所以就有了如下的新的目标函数：

也就是说，现在的 D 直接就是神经网络的某一个中间层了。这个方法在实验中发现对于训练的稳定度提升非常有帮助。与此同时，他们还提出了第二个方法，叫 minibatch discrimination：

![](http://img.mp.sohu.com/upload/20170527/1c0cf7985d5e421691c513a6c637a98d_th.png)

这其实是让 D 在判断当前传给它的样本是真是假的同时，不要只关注当前的，也要关注其他的样本。这会增加 D 判断样本时候的多样性，从而增加了 G 生成样本的多样性，因为它不再会只基于一种“逼真”样本的方向来改进自己。

**第三个工作是来自 UC Berkeley 的 iGAN/GVM[24]，**也是开篇介绍 GAN 应用中的解放灵魂画手的工作。他们的工作中蕴含了两种指导信息，一个是用户的输入，比如蓝色的笔触，比如绿色的线条，比如图像改变的方向和程度（拉伸、变形）。但是如果只利用这样的信息，生成的图像往往比较模糊，比如鞋子的纹理变得不够清晰。为此它们的解决办法是为在生成的鞋子的形状上“贴”上原始真实图片中的高清纹理。所以难点就是如何让“贴”的过程变得可靠，不能“贴”出区域，也不能“贴”少了。他们在此利用了差值空间中的光场信息，从而能捕捉到相邻差值空间中的点对点映射关系，也就可以基于这样的映射，迭代“贴”上纹理，直到最后一步：

![](http://img.mp.sohu.com/upload/20170527/641eca1120b64dbfb10791edfa315dde_th.png)

**iGAN 的工作之后，他们又继续做了 pix2pix 的工作**[21]，用于生成一些图像两两之间的“变换”。也就是开篇介绍的，“从分割图变成真实照片，从黑白图变成彩色图，从线条画变成富含纹理、阴影和光泽的图”，还有第一个 zi2zi 的字体变换，也是基于这个 pix2pix 的工作[21]。pix2pix 里，将 D 的输出从一张图片变成了一对图片，所以 D 的任务就变成了去判断当前的两张图片是否是一个“真实”的“变换”。比如我们的需求是给一个黑白的 Hello Kitty 上色，那么 pix2pix 的框架大概如下：

![](http://img.mp.sohu.com/upload/20170527/fff1684369f745c784af430bb074a3dd_th.png)

**而 Partial Guidance 中的最后一个重要任务就是非常新的 GP-GAN[25]**，目标是将直接复制粘贴过来的图片，更好地融合进原始图片中，做一个 blending 的事情。

![](http://img.mp.sohu.com/upload/20170527/f5ca83978e7547a4a7d3f9a44d125e00.png)

这个过程非常像 iGAN，也用到了类似 iGAN 中的一些约束，比如 color constraint。另一方面，这个工作也有点像 pix2pix，因为它是一种有监督训练模型，在 blending 的学习过程中，会有一个有监督目标和有监督的损失函数。

![](http://img.mp.sohu.com/upload/20170527/8bb3ff06316440f08e324cf3d072927a_th.png)

**解决方案一：Fine-grained Guidance**

除了 Partial Guidance 这种非常显式的“半监督”（不是严格意义上的半监督）信息，过去也有很多工作**让 GAN 的生成过程拆解到多步，从而实现“无监督”的 Fine-grained Guidance。**个人总结了以下一些重要工作：

![](http://img.mp.sohu.com/upload/20170527/f070479c5244418d8142d5b3ee87b81f_th.png)

**第一篇 LAPGAN 是来自 Facebook[16]，**是第一篇将层次化或者迭代生成的思想运用到 GAN 中的工作。在原始 GAN[2] 和后来的 CGAN[15] 中，GAN 还只能生成 16*16, 28*28, 32*32 这种低像素小尺寸的图片。而这篇工作[16] 是首次成功实现 64*64 的图像生成。思想就是，与其一下子生成这么大的（包含信息量这么多），不如一步步由小转大，这样每一步生成的时候，可以基于上一步的结果，而且还只需要“填充”和“补全”新大小所需要的那些信息。这样信息量就会少很多：

![](http://img.mp.sohu.com/upload/20170527/32fad540acc642f998032afcab190926_th.png)

而为了进一步减少信息量，他们甚至让 G 每次只生成“残差”图片，生成后的插值图片与上一步放大后的图片做加法，就得到了这一步生成的图片。

**第二篇 Fine-grained Guidance 方面的工作[18]**讨论的是从 Text 生成 Image，比如从图片标题生成一个具体的图片。这个过程需要不仅要考虑生成的图片是否真实，还应该考虑生成的图片是否符合标题里的描述。比如要标题形容了一个黄色的鸟，那么就算生成的蓝色鸟再真实，也是不符合任务需求的。为了捕捉或者约束这种条件，他们提出了 matching-aware discriminator 的思想，让本来的 D 的目标函数中的两项，扩大到了三项：

![](http://img.mp.sohu.com/upload/20170527/186586c479c8496bb9628977d33bc908.png)

**第三篇这方面的工作[20]**可以粗略认为是 LAPGAN[16] 和 matching-aware[18] 的结合。**他们提出的 StackGAN**[20] 做的事情从标题生成鸟类，但是生成的过程则是像 LAPGAN 一样层次化的，从而实现了 256*256 分辨率的图片生成过程。StackGAN 将图片生成分成两个阶段，阶段一去捕捉大体的轮廓和色调，阶段二加入一些细节上的限制从而实现精修。这个过程效果很好，甚至在某些数据集上以及可以做到以假乱真：

![](http://img.mp.sohu.com/upload/20170527/e7730a2fde7f40feaa03cf581b1d1331_th.png)

**最后一个这方面的工作[26]**，当时也因为效果逼真和工作夯实，引发了社交网络上和学术圈的广泛关注，那就是**去年年底的 PPGN[26]，现在已被 CVPR 2017 接收**。PPGN 也主张不要一次生成一张完整的图片，而是要用一个迭代过程不断地调整和完善。与 LAPGAN 和 StackGAN 不同的是，PPGN 使用了 Denoising AutoEncoder（DAE）的过程实现迭代，并在其网络结构中也多次体现了迭代和层次化的思想。

![](http://img.mp.sohu.com/upload/20170527/c2cb52778ca04759aedd4de377f175af_th.png)

![](http://img.mp.sohu.com/upload/20170527/c0f15ded269f4e6a98bfd7b1bffb63aa_th.png)

**解决方案三：Special Architecture**

**针对 GAN 的第一大难点，也就是神经网络本身训练的不稳定和难调参，也有许多工作提出了一些特殊结构，来改善这些情况。**

![](http://img.mp.sohu.com/upload/20170527/0f19c5f53dfa4ab09cb63ace89fc3c25_th.png)

将 GAN 成功从 MNIST 的多层感知机（MLP）结构扩展到卷积神经网络结构的**就是 DCGAN 这篇工作**[17]。这篇工作中，他们提出了一组卷积神经网络，不仅使得可以 GAN 可以在 celebA 和 LSUN 这种现实世界的真实大规模数据集上训练，还使得 batchnorm 等 trick 也被成功运用。

![](http://img.mp.sohu.com/upload/20170527/a4409e387cb842f48e81f6fef4951fa1_th.png)

虽然 DCGAN 的一度成为了 GAN 工作的标准，统治了大半年的江湖。**但是随后出现的效果非常惊艳的 pix2pix**[21] 中却指出了 U-Net 结构的优势。pix2pix 中 G 和 D 使用的网络都是 U-Net 结构，是一种 encoder-decoder 完全对称的结构，并且在这样的结构中加入了 skip-connection 的使用。

![](http://img.mp.sohu.com/upload/20170527/22389ef7eaa5448bb1aef133af4b40f6_th.png)

这个结构对于生成效果至关重要，其也被后续的一些工作采用[9][11] 。skip-connection 不仅使得梯度传导更通畅，网络训练更容易，也因为这类工作多数是要学习图片之间的映射，那么让 encoder 和 decoder 之间一一对应的层学到尽可能匹配的特征将会对生成图片的效果产生非常正面的影响。类似的讨论可以见 [11]。

**最后要指出的也是刚才就提到的 GP-GAN[25] 的工作**。在这个工作中，它们提出了 blending GAN 的模块，虽然也是基于 encoder-decoder 的结构，但是略有不同的地方是，在两者中间加入了一个 fully-connected layer：

![](http://img.mp.sohu.com/upload/20170527/7153c83624324ba08df15054d623e3c3_th.png)

这个全连接层的特殊之处在于，并不是卷积神经网络中的 channel-wise FCN，而是彻底全连接。这样的好处是可以传递更多的全局信息，使得有监督学习变得更加有效。

![](http://img.mp.sohu.com/upload/20170527/1f0c4ca68bcb4cc9b0d64e7baafa294f_th.png)

**训练难点二：原始目标函数没意义**

其实，GAN 训练之难，更多的源于它 GAN 目标函数自身。GAN 的 D 的目标函数上文已经提过：

而 G 的目标函数相应的是：

也就是说，G 需要最小化让 D 识别出自己生成的假样本的概率。但其实，在 GAN 原始论文[2] 中，作者就指出使用如上的 G 的目标函数会给训练造成问题。从**形象化的角度来理解**，在训练的早期，G 生成的假样本质量还非常差，与真实样本相距过远。这会知道 D 非常容易识别出 G 的假样本，从而使得 D 的训练几乎没有损失，也就没有有效的梯度信息回传给 G 让 G 去优化自己。这样的现象叫做 gradient vanishing，梯度消失问题。

**从偏理论的角度来理解**，梯度消失的问题实际上更“复杂”一些。想要理解它需要先理解 GAN 的 min-max game 的平衡条件。当 G 和 D 的对抗训练达到平衡时，可以认为取得了最优的 D（和最优的 G），此时最优的 D* 应该是两个分布的比值：

![](http://img.mp.sohu.com/upload/20170527/203304424e944ed6afaa70c9fb68402d_th.png)

有了最优的 D* 的表达，就可以将它带入原始的 D 的目标函数，从而得到上页 slides 中的等价表达。也就是说，在 GAN 原始论文[2] 中就已经给出了，**优化这样一个目标函数等价于优化 JS 散度（因为 2log2 是常数）。**

然而，问题就出在了这个 JS 散度上。在论文[1] 中，作者指出当两个分布（比如这里的真实数据分布 P_r 和 生成数据分布 P_g 之间几乎不重合或者重合部分可忽略不计时，**JS 散度也是个常数！**而这在由神经网络拟合的分布中是非常常见的！也就是说，原始的 GAN 目标函数几乎是常数，所以也就不难理解为什么梯度几乎消失了。

![](http://img.mp.sohu.com/upload/20170527/58fc37aa5e7a4d018686a8aa2e85b98d_th.png)

**训练难点三：替代目标函数不合理**

针对梯度消失问题，原始 GAN 论文[2] 就提出了一个启发性的替代目标函数：

这一函数相比原始的目标函数，相当于从最小化 D 揪出自己的概率，变成了最大化 D 抓不到自己的概率。虽然直观上感觉是一致的，但其实并不在理论上等价，也更没有了理论保证在这样的替代目标函数训练下，GAN 还会达到平衡。

即使如此，论文[1] 依然指出，就算是这个替代的目标函数，也同样有问题！他们证明了，去优化这样一个目标函数等价于最小化 KL 散度的同时最大化 JS 散度。

![](http://img.mp.sohu.com/upload/20170527/f7a48501b5b748fb87fb882b4cfd5920.png)

可是，KL 散度和 JS 散度是同向的：

![](http://img.mp.sohu.com/upload/20170527/297230248e7d4027be7af99bf3e9c61e_th.png)

**也就是说，如果要最小化其中一个的同时却要最大化另一个，这只会导致优化目标不一致和训练不稳定，也就是 gradient unstable issue。**不过，既然在难点二中分析了，JS 散度在这种情况下经常为常数，不提供信息，那么只看 KL 散度行不行呢。答案依然是不行。即使我们只考虑优化 KL 散度，因为 KL 散度是不对称的，也会导致问题。也就是说，当生成器无法生成真实图像和生成了非常不真实的图像，这两种情况下，它得到的惩罚是不一样的。这会导致，生成器 G 为了得到更少的惩罚，只得选择惩罚最小的策略，也就是生成尽可能多的看起来真实的图片而不考虑这些图片是不是一样！也就是说，只要它生成了一张判别器认为还面前凑合的图片，它就不会再愿意冒险去尝试新的图片，因为这种冒险会带来更多的惩罚。**所以只优化一个不对称的 KL 散度，就会导致一个叫 ****mode missing 的问题**，这个在我们以前也专门分析过，点我传送。

![](http://www.sohu.com/a/143961544_741733)

![](http://img.mp.sohu.com/upload/20170527/1869998a558242c2bbf172833e474235_th.png)

**解决方法二：Encorder-Incorporated**

为了解决上述 mode missing 的问题，有非常多的工作其实可以归为一大类。**那就是在原始 G 和 D 之上再加上一个自动编码器 encoder。比如比较有代表性的工作[3]**，就是让真实图片通过一个编码器 E 得到一个隐空间的表达 z，再在此基础上用生成器 G 生成最后的图片。所以，对于编码器的目标函数是：

![](http://img.mp.sohu.com/upload/20170527/3d2a2bbfdbc24867b349f9ff72b383d5.png)

而对于加入了编码器的生成器的目标函数就会变成：

![](http://img.mp.sohu.com/upload/20170527/f2ea739adef94f8ea1f4ea8e4895b2c2.png)

在一个模拟实验上可以看到，没有编码器的原始 GAN 会非常难以拟合多个分散的 mode，而加入了编码器的 regularized GAN 则非常容易：

![](http://img.mp.sohu.com/upload/20170527/4d843524094d4bbaa55f54a5e6258109_th.png)

同时期还有许多相似的工作，比如 EBGAN[4] 和 BEGAN[5]。两篇工作非常相似，EBGAN 将能量函数作为判别器 D 的输出；而 BEGAN 则将这一条件“简化”。

![](http://img.mp.sohu.com/upload/20170527/bc3451e84dfd47ffb46bff6eefa6c79e_th.png)

![](http://img.mp.sohu.com/upload/20170527/3de5c7d0afa04001936a6863ef26e38d_th.png)

**解决方法二：Noisy Input**

虽然介绍了这些加入了编码器的工作，但是他们为什么就能解决 mode missing 的问题呢。这主要还是因为加入了编码器后，就可以尽量让生成数据的分布 P_g 和真实数据的分布 P_r 更相近，重合的可能性和程度更高。从而缓解 JS 散度几乎为常数导致的梯度无意义的问题。

然而其实有更简单的一些技巧和方法来实现相似的目的：

![](http://img.mp.sohu.com/upload/20170527/e7b50ad633b145c495d96f093f119b54_th.png)

在论文[1] 中，作者们就指出，如果给 P_g 和 P_r 都加入非常小的噪音，那么它们从完全不重合到重合，从而其 JS 散度也会变得有意义。虽然这篇论文[1] 到2016年底才被提出，但相似的技巧早在之前的工作中就被人运用过。**比如在 G 和 D 的神经网络中间层中加入噪音[4]，或者直接在图片输入加入噪音[6] 等等**。

![](http://img.mp.sohu.com/upload/20170527/116856d64de843b4a126920ef748afe9_th.png)

**解决方法二：Encoders-Constrained**

但是上述工作其实并没有完全解决 mode missing 的问题。这是为什么呢？**在最近介绍过的 DiscoGAN[22] 中，有针对 mode missing 的更深入的分析。**关于 DiscoGAN 的详细介绍点我传送。

![](http://www.sohu.com/a/143961544_741733)

![](http://img.mp.sohu.com/upload/20170527/0a122daf84854af3918b7d79dd9f83ed_th.png)

在上图中，(a) 代表理想状态下学习到的生成规则，(b) 是普通 GAN 会学习到的规则，(c) 是只加了一个 encoder，只做一次 reconstruction constraint 时候的 GAN 的情况。可以粗略地将刚才提到的 RegGAN[3] EBGAN[4] 和 BEGAN[5] 都归为情况 (c)。可以看到，虽然情况 (c) 会比 (b) 好一些，但并不能完全避免 mode missing 的问题，而是在两种 mode 之间“震荡”。

对此，近期有三篇想法相似的工作可以从理论上完全避免 mode missing 的问题。**这三篇工作就是前阵子专门为大家推荐过的 CycleGAN [9], DiscoGAN [10]和 DualGAN[11]**，点我传送。

![](http://www.sohu.com/a/143961544_741733)

CycleGAN[9] 就是开篇的“莫奈从春天醒来”、“马变斑马”、“四季更迭”背后的模型。它的论文开头非常优美和引人入胜，先是放出了这些 impressive 的 result；同时用一种带逛博物馆的语气写了 Introduction：“当莫奈在一个和煦的春天的早上，站在河畔画下这幅画时，他眼前究竟是怎样的景象呢？如果莫奈站在一个清爽的夏天的早上，又会看到怎样的景象，画出怎样的作品呢？” 作者的写作功底可见一斑。

作者用这样的开头，引出了一个 motivation：我们虽然没有办法真的穿越时空，站在莫奈身边看到他当时看到的景象，但是我们却可以通过他的画作，想象当时的场景；同时也可以根据他的作品（和他的画风），想象他画出的其他场景会是怎样的。也就是说，我们没有 paired data 却可以实现这种 “翻译” 或者说映射。那么我们是否也可以让机器做到这件事呢？

我们人类之所以可以做到这件事，作者假设，在两个领域（X，Y）之间，是存在一种底层的关系的，或者说隐含的关联——这种隐含的关联可能是，对于同一个事物，有两种不同的映射后的表达，那么这两种表达之间就是针对这同一个事物的一种关联。如果我们仅用一个映射把 G: X->Y 进行单向映射，那么我们无法保证这个 G 是单一的。或者说，我们无法保证 X 中的所有样本 x 和 Y 领域中的所有样本 y 是合理对应的。这也就（可能）导致 mode collpase 的问题。**对于这个问题的描述，在 CycleGAN 中，作者用了 meaningful 这个词；而在 DiscoGAN 中，作者强调了我们希望的是 one-to-one mapping, rather than many-to-one mapping**。

通过这样的分析，很容易看出，一种自然的解决方案就是，我们不仅要求一个单向映射，而更要求一种双向映射。**在 CycleGAN 中，这个问题被形式化为：我们有 G: X->Y, F: Y->X 两个映射或者说翻译器，我们希望 F(G(x)) ≈ x, G(F(y)) ≈ y. 在 DiscoGAN 中，作者直接把这个映射用 GAN 中的 generator 代表，所以为了实现这样的双向映射，DiscoGAN 中指出我们需要两个 G，并且让尽量 G1(G2(x)) = x，反之同理。DualGAN 的形式化与 DiscoGAN 相似。**不过，三篇论文分别用了三种不同的术语来实现这样的约束或者说目标。CycleGAN 中，作者用了 vision 中被应用多次的 **cycle consistency loss**，CycleGAN 也因此得名；DiscoGAN 则是因为直接强调了一一映射，所以用了 **bijective map**来阐述；DualGAN 因为是从去年 NIPS 2016 Dual Learning for MT 的 paper 受到启发的，所以是从**closed loop**角度来写的。因为 CycleGAN 的模型示意图比较简洁，所以这里以它为示例：

![](http://img.mp.sohu.com/upload/20170527/7226cab90ee74429bd65d9a199d11fc5_th.png)

上图中红色箭头指向的就是两个方向对应的各自的 cycle-consistency loss。在这篇工作中，它们还实现了一种基于画家风格的风格转换。这种风格转换不再是基于某一幅图画的风格，所以可以理解为批量风格转换。比如梵高的《星夜》和其《向日葵》的风格就很不相似：

![](http://img.mp.sohu.com/upload/20170527/4f20d25bab4948cab3ca43d07f89c409_th.png)

那么 CycleGAN[9] 和 DiscoGAN[10] 等用双向映射（也就是两个编码器）的思想，对于 mode missing problem 到底解决得如何呢？来看看下图的实验结果：

![](http://img.mp.sohu.com/upload/20170527/07dfb36e0228402c93c301b11a166851_th.png)

上图中的 BiGAN[13] 和 CoGAN 虽然也是双向映射，但是映射空间是在隐空间。从实验结果来看，这样的不同使得其效果大打折扣：

![](http://img.mp.sohu.com/upload/20170527/9caffd74b2da4984bc6cd0289f99d393_th.png)

**DiscoGAN[10] 则在这个问题的可视化分析上做得更好：**

![](http://img.mp.sohu.com/upload/20170527/cb2d601e3d82498e9160ff0c31bd0384_th.png)

上图中 (a) 是原始数据分布，(b) 是普通 GAN，(c) 是加了单向映射的 GAN，(d) 是 DiscoGAN。(c) (d) 都区分出了一定程度的 mode，但 (c) 还是 miss 掉了一些（右上角绿色背景的）。而 (d) 则完美地区分了10个 mode，并且从背景色来看（背景色代表判别器 D 的输出），判别器对于同一种 mode 的输出非常一致，几乎没有渐变色，这也说明了 DiscoGAN 的训练优势。综上，这些基于编码器的工作都可以认为几乎解决了 GAN 训练难点三：替代目标函数不合理，一定程度解决了 JS 散度在两个分布几乎不重叠时候的问题。

![](http://img.mp.sohu.com/upload/20170527/62226751795245a89d5bf152fc13e870_th.png)

**解决方法三：Wasserstein Distance**

其实从根本上讲，GAN 目标函数的问题都是 KL 散度和 JS 散度衡量两个分布时候的不足造成的。那么有没有办法直接用其他分布度量来为 GAN 这种对抗游戏提供优化目标呢？为此就有了论文[1] 的眼神，Wasserstein GAN[7]，目前已被 ICML 2017 录用。

Wasserstein GAN，简称 WGAN 基于的是 Wasserstein Distance，也叫 Earth-Mover Distance，推土机距离。

![](http://img.mp.sohu.com/upload/20170527/dab1fe0fb385404e9c7db0f107ec5beb_th.png)

这个距离可以形象的理解为，将一个分布变成另一个分布所需要的消耗。如果用一个直观的理解就是，把一个沙堆推到另一个地方，形成另一堆可能长得不一样的沙堆所需要的“距离”。而这种“转变”并不是唯一的，所以很可能有些“路径”消耗大，有些消耗小。**用比较严谨的说法，Wasserstein 距离表示的是“最有规划路径”下的最短距离。**

![](http://img.mp.sohu.com/upload/20170527/97a8607ffc31472c8aeb213b8697d22f_th.png)

这个距离有很多优良的性质，**其中最最最重要的一条就是它可以在两个分布毫无重叠的情况下依然给出有效的度量。**也就是说，用它作为优化目标则不需要担心梯度无意义或者梯度消失的问题。但是 Wasserstein Distance 中的“求下界”的操作无法准确高效计算，所以作者用了 Kantorovich-Rubinstein 对偶将其变换了一下，**就得到了 WGAN 的目标函数：**

![](http://img.mp.sohu.com/upload/20170527/c4aa7c002a384c3c8e4ab8957820f526_th.png)

但是这样的变换要求符合一个先决条件，也就是判别器 D 拟合的函数需要是 1-Lipschitz 连续函数。Lipschitz 连续实际上是要求一个连续函数的导函数的绝对值不大于某个常数，也就是说，它限制了一个连续函数的最大局部变动幅度。这个对于由神经网络来拟合的函数来讲，导数可以粗暴理解为神经网络的权重。**所以在 WGAN[7] 中他们采取了 weight clipping，梯度剪裁的方式，将“导数”限制在 [-c,c] 范围内。**也就是每次更新 D 的参数后，超过这个范围的都拉回来。

![](http://img.mp.sohu.com/upload/20170527/c4c8ed74575a49348ba1918ea4bb20d4_th.png)

从上图可以看到，WGAN 的 D 的导数很平滑（也就是所谓的 WGAN Critic，浅蓝色线），而原始的 GAN 的梯度消失很严重（红色线）。也就是说，有了基于 Wasserstein distance 的 WGAN，原始 GAN 中的难点二，梯度消失问题就解决了。

同时，他们还发现基于由于现在的 WGAN 中的 D 不再是做二分类任务，而是做一个“回顾”任务去拟合 Wasserstein distance，这个 distance 从实验上发现，与生成的图片质量呈负相关：

![](http://img.mp.sohu.com/upload/20170527/7ad015c61dec4d23b043175bc4d0b1cf_th.png)

![](http://img.mp.sohu.com/upload/20170527/b02c39cfbf8b498ba060688a5799495d_th.png)

**梯度裁剪的问题以及改进**

虽然 WGAN 在实验中展现了自己比原始 GAN 稳定的一面，但它依然遗留了一个问题。也就是通过 weight clipping，梯度裁剪这种方法选择的那个超参 c 对于实验结果的影响有多大？其实这个问题在原始 WGAN[7] 的论文中作者就有讨论。当 c 太大时，会出现梯度爆炸问题；过小也会导致梯度消失问题。

**基于此，就有了 Improved WGAN[14] 这篇工作。**他们首先分析了，到底基于梯度裁剪方式，会导致什么样的问题。首先在 Section 2.3 中他们证明，用这样的方式实际上也存在一个最优的判别器 D，当达到这个最优判别器时，D 的所有权重都会倾向于等于c，如下：

![](http://img.mp.sohu.com/upload/20170527/9b629657f4574df5926556cc9e853597_th.png)

而这样的一个缺点就是，会导致学出来的网络过于简单，对于复杂函数的拟合能力或者说对于分布的建模能力会明显下降。比如下面的模拟实验中，就可以看到基于梯度裁剪的方法拟合分布，会忽略掉高阶动量。

![](http://img.mp.sohu.com/upload/20170527/1274a72b941340819c3b31a4382d7222_th.png)

为此，[14] 提出了一种新的方式去满足 WGAN 目标函数的额外要求，也就是 Lipschitz 连续性。**他们指出既然最优的判别器的权重会倾向于一个常数，不如就把这个常数当成“目标”，把当前的权重与这个常数的距离，当成一种惩罚项或者正则项，**加入 WGAN 的目标函数中：

![](http://img.mp.sohu.com/upload/20170527/e2d1563852964105b199dbacdd167d04.png)

**也因此，这个方法被叫做 gradient penalty，这样的 WGAN 就叫 WGAN-GP [14]**。WGAN-GP 比原始 WGAN 的收敛速度更快，训练也更稳定，得到的生成结果的质量也更好。

![](http://img.mp.sohu.com/upload/20170527/3ad75ad28ca5462995cceb894b126305_th.png)

![](http://img.mp.sohu.com/upload/20170527/13fcdc835fdb41a0a5de59528767047f_th.png)

**结语及资源**

本文主要总结了 GAN 发展过程中的一些重要模型。这些模型的出现有些甚至甚至早于理论上的发展，但是当理论发展到一定程度，回过头再去看这些模型就会有一些新的理解。如果大家对 GAN 很感兴趣，也想自己上手试试，优先推荐以下的一些模型和技巧：

![](http://img.mp.sohu.com/upload/20170527/3b67d606ecb049ee816570851b2e0d2f_th.png)

最后再次附上这份文字版干货对应的完整 slides 和回顾视频。**只要公众号后台回复“原理篇”就可以获取126页完整 PDF 和分享视频啦！**



**参考文献**

> 
[1] Arjovsky and Bottou, “Towards Principled Methods for Training Generative Adversarial Networks”. ICLR 2017.

[2] Goodfellow et al., “Generative Adversarial Networks”. ICLR 2014.

[3] Che et al., “Mode Regularized Generative Adversarial Networks”. ICLR 2017.

[4] Zhao et al., “Energy-based Generative Adversarial Networks”. ICLR 2017.

[5] Berthelot et al., “BEGAN: Boundary Equilibrium Generative Adversarial Networks”. arXiv preprint 2017.

[6] Sønderby, et al., “Amortised MAP Inference for Image Super-Resolution”. ICLR 2017.

[7] Arjovsky et al., “Wasserstein GANs”. ICML 2017.

[8] Villani, Cedric. “Optimal transport: old and new”, volume 338. Springer Science & Business Media, 2008.

[9] Jun-Yan Zhu*, Taesung Park*, Phillip Isola, Alexei A. Efros. “Unpaired Image-to-Image Translation using Cycle-Consistent Adversarial Networks”. arXiv preprint 2017.

[10] Taeksoo Kim, Moonsu Cha, Hyunsoo Kim, Jung Kwon Lee, Jiwon Kim. “Learning to Discover Cross-Domain Relations with Generative Adversarial Networks”. ICML 2017.

[11] Zili Yi, Hao Zhang, Ping Tan, Minglun Gong. “DualGAN: Unsupervised Dual Learning for Image-to-Image Translation”. arXiv preprint 2017.

[12] Jeff Donahue, Philipp Krähenbühl, Trevor Darrell. “Adversarial Feature Learning”. ICLR 2017.

[13] Vincent Dumoulin, Ishmael Belghazi, Ben Poole, Olivier Mastropietro, Alex Lamb, Martin Arjovsky, Aaron Courville. “Adversarially Learned Inference”. ICLR 2017.

[14] Ishaan Gulrajani, Faruk Ahmed, Martin Arjovsky, Vincent Dumoulin, Aaron Courville. “Improved Training of Wasserstein GANs”. arXiv preprint 2017.

[15] Mehdi Mirza, Simon Osindero. “Conditional Generative Adversarial Nets”. arXiv preprint 2014.

[16] Emily Denton, Soumith Chintala, Arthur Szlam, Rob Fergus. “Deep Generative Image Models using a Laplacian Pyramid of Adversarial Networks”. arXiv preprint 2015.

[17] Alec Radford, Luke Metz, Soumith Chintala. “Unsupervised Representation Learning with Deep Convolutional Generative Adversarial Networks”. ICLR 2016.

[18] Scott Reed, Zeynep Akata, Xinchen Yan, Lajanugen Logeswaran, Bernt Schiele, Honglak Lee. “Generative Adversarial Text to Image Synthesis”. ICML 2016.

[19] Tim Salimans, Ian Goodfellow, Wojciech Zaremba, Vicki Cheung, Alec Radford, Xi Chen. “Improved Techniques for Training GANs”. arXiv preprint 2016.

[20] Han Zhang, Tao Xu, Hongsheng Li, Shaoting Zhang, Xiaolei Huang, Xiaogang Wang, Dimitris Metaxas. “StackGAN: Text to Photo-realistic Image Synthesis with Stacked Generative Adversarial Networks”. arXiv preprint 2016.

[21] Phillip Isola, Jun-Yan Zhu, Tinghui Zhou, Alexei A. Efros. “Image-to-Image Translation with Conditional Adversarial Networks”. CVPR 2017.

[22] Yaniv Taigman, Adam Polyak, Lior Wolf. “Unsupervised Cross-Domain Image Generation”. ICLR 2017.

[23] Ian J. Goodfellow, Jean Pouget-Abadie, Mehdi Mirza, Bing Xu, David Warde-Farley, Sherjil Ozair, Aaron Courville, Yoshua Bengio. “Generative Adversarial Nets”. NIPS 2014.

[24] Jun-Yan Zhu, Philipp Krähenbühl, Eli Shechtman and Alexei A. Efros. “Generative Visual Manipulation on the Natural Image Manifold”, ECCV 2016.

[25] Huikai Wu, Shuai Zheng, Junge Zhang, Kaiqi Huang. “GP-GAN: Towards Realistic High-Resolution Image Blending”. arXiv preprint 2017.

[26] Anh Nguyen, Jeff Clune, Yoshua Bengio, Alexey Dosovitskiy, Jason Yosinski. “Plug & Play Generative Networks: Conditional Iterative Generation of Images in Latent Space”. CVPR 2017.


**转载自：**[http://www.sohu.com/a/143961544_741733](http://www.sohu.com/a/143961544_741733)






# 细水长flow之NICE：流模型的基本概念与实现 - Paper weekly - CSDN博客





2018年08月13日 12:51:45[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：541









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨苏剑林

单位丨广州火焰信息科技有限公司

研究方向丨NLP，神经网络

个人主页丨kexue.fm




**前言：**自从在机器之心上看到了 Glow 模型之后（[下一个GAN？OpenAI提出可逆生成模型Glow](http://mp.weixin.qq.com/s?__biz=MzA3MzI4MjgzMw==&mid=2650745032&idx=1&sn=a889433dd4c4d9f62bfab347909d9d28&chksm=871aecb6b06d65a02625abdf4b21a2116251e311a49508db587b76ae8f76d7a9e03d4a6ab80a&scene=21#wechat_redirect)），我就一直对其念念不忘。现在机器学习模型层出不穷，我也经常关注一些新模型动态，但很少像 Glow 模型那样让我怦然心动，有种“就是它了”的感觉。更意外的是，这个效果看起来如此好的模型，居然是我以前完全没有听说过的。于是我翻来覆去阅读了好几天，越读越觉得有意思，感觉通过它能将我之前的很多想法都关联起来。在此，先来个阶段总结。




# 背景




本文主要是 ***NICE: Non-linear Independent Components Estimation*** 一文的介绍和实现。这篇文章也是 [Glow 模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490358&idx=1&sn=b4b5d6014bdd365456d500537ba5bcad&chksm=96e9c4b6a19e4da08710a55935dc2e15b00838d5395fdf2a424c50dedff9af7fa11441741b9d&scene=21#wechat_redirect)的基础文章之一，可以说它就是 Glow 的奠基石。




■ 论文 | NICE: Non-linear Independent Components Estimation

■ 链接 | https://www.paperweekly.site/papers/2206

■ 源码 | https://github.com/laurent-dinh/nice




**艰难的分布**




众所周知，目前主流的生成模型包括 VAE 和 GAN，但事实上除了这两个之外，还有基于 flow 的模型（flow 可以直接翻译为“流”，它的概念我们后面再介绍）。




事实上 flow 的历史和 VAE、GAN 它们一样悠久，但是 flow 却鲜为人知。在我看来，**大概原因是 flow 找不到像 GAN 一样的诸如“造假者-鉴别者”的直观解释吧，因为 flow 整体偏数学化，加上早期效果没有特别好但计算量又特别大，所以很难让人提起兴趣来**。不过现在看来，OpenAI 的这个好得让人惊叹的、基于 flow 的 Glow 模型，估计会让更多的人投入到 flow 模型的改进中。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3T8b2I31icJxqoicMkHia9CRdiaz5zQS4SKWIfkjicfN0nvEDmLtXBvaqibVg/640)

▲ Glow模型生成的高清人脸




生成模型的本质，就是希望用一个我们知道的概率模型来拟合所给的数据样本，也就是说，我们得写出一个带参数 θ 的分布 qθ(x)。然而，我们的神经网络只是“万能函数拟合器”，却不是“万能分布拟合器”，也就是它原则上能拟合任意函数，但不能随意拟合一个概率分布，因为概率分布有“非负”和“归一化”的要求。这样一来，我们能直接写出来的只有离散型的分布，或者是连续型的高斯分布。




当然，从最严格的角度来看，图像应该是一个离散的分布，因为它是由有限个像素组成的，而每个像素的取值也是离散的、有限的，因此可以通过离散分布来描述。**这个思路的成果就是 PixelRNN 一类的模型了，我们称之为“自回归流”**，其特点就是无法并行，所以计算量特别大。所以，我们更希望用连续分布来描述图像。当然，图像只是一个场景，其他场景下我们也有很多连续型的数据，所以连续型的分布的研究是很有必要的。



**各显神通**




所以问题就来了，对于连续型的，我们也就只能写出高斯分布了，而且很多时候为了方便处理，我们只能写出各分量独立的高斯分布，这显然只是众多连续分布中极小的一部分，显然是不够用的。为了解决这个困境，我们通过积分来创造更多的分布：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3BqJGO6FDSTDyliaicDPicrLibdaSzwJB1oMvgnrXVBG60U6iao2fC30plTw/640)




这里 q(z) 一般是标准的高斯分布，而 qθ(x|z)=qθ(x|z) 可以选择任意的条件高斯分布或者狄拉克分布。这样的积分形式可以形成很多复杂的分布。理论上来讲，它能拟合任意分布。 




现在分布形式有了，我们需要求出参数 θ，那一般就是最大似然，假设真实数据分布为 p̃(x)，那么我们就需要最大化目标：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3XazbyQzt8ibXhGnPU0QZp1Ciau45VT4fAV6icreRUZ5UgiaZvicOsp9bmvw/640)




然而 qθ(x) 是积分形式的，能不能算下去很难说。




于是各路大神就“八仙过海，各显神通”了。其中，VAE 和 GAN 在不同方向上避开了这个困难。VAE 没有直接优化目标 (2)，而是优化一个更强的上界，这使得它只能是一个近似模型，无法达到良好的生成效果。GAN 则是通过一个交替训练的方法绕开了这个困难，确实保留了模型的精确性，所以它才能有如此好的生成效果。但不管怎么样，GAN 也不能说处处让人满意了，所以探索别的解决方法是有意义的。




**直面概率积分**




flow 模型选择了一条“硬路”：**直接把积分算出来。**




具体来说，flow 模型选择 q(x|z) 为狄拉克分布 δ(x−g(z))，而且 g(z) 必须是可逆的，也就是说：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3oFLniaMOVLgibULJ5RE79J55rULRNnQKZiaS2gibHXF00IK3oj7KYmGqqg/640)




要从理论上（数学上）实现可逆，那么要求 z 和 x 的维度一样。假设 f,g 的形式都知道了，那么通过 (1) 算 q(x) 相当于是对 q(z) 做一个积分变换 z=f(x)。即本来是：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3NF4ws9icfznBSJhECRHXdy3XciaGHn78mRM2ia3Aw5OU8HXaYXZWdJdGQ/640)




的标准高斯分布（D 是 z 的维度），现在要做一个变换 z=f(x)。注意概率密度函数的变量代换并不是简单地将 z 替换为 f(x) 就行了，还多出了一个**“雅可比行列式”的绝对值**，也就是：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3K6ZfaXw0tGKJ9MvuTc2aic7WajRqctlZ96aQtzEaFcI5KDv7ZRIo2Eg/640)




这样，对 f 我们就有两个要求：




1. 可逆，并且易于求逆函数（它的逆 g 就是我们希望的生成模型）；




2. 对应的雅可比行列式容易计算。




这样一来：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3WGAjd3eI1vIMjWHJ668CibWDiaMVsBicgBc0faiahcPlc9aNlsWkkl0w8A/640)




这个优化目标是可以求解的。并且由于 f 容易求逆，因此一旦训练完成，我们就可以随机采样一个 z，然后通过 f 的逆来生成一个样本![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3xjRg69SJszqCO26cL5Op9vVv9iaMI0K0iaBgajjdfkTiaP3sqqPBdiaRYw/640)，这就得到了生成模型。




# flow




前面我们已经介绍了 flow 模型的特点和难点，下面我们来详细展示 flow 模型是如何针对难点来解决问题的。因为本文主要是介绍第一篇文章 ***NICE: Non-linear Independent Components Estimation*** 的工作，因此本文的模型也专称为 NICE。




**分块耦合层**




相对而言，行列式的计算要比函数求逆要困难，所以我们从“要求 2”出发思考。熟悉线性代数的朋友会知道，三角阵的行列式最容易计算：**三角阵的行列式等于对角线元素之积**。所以我们应该要想办法使得变换f的雅可比矩阵为三角阵。NICE 的做法很精巧，它将 D 维的 x 分为两部分 x1,x2，然后取下述变换：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3ANASCiaeKtm2mcsy5nMHOrDQLsS2ohmJ2SWa32IT8rO8UraeibmjBsBQ/640)




其中 x1,x2 是 x 的某种划分，m 是 x1 的任意函数。也就是说，将 x 分为两部分，然后按照上述公式进行变换，得到新的变量 h，这个我们称为“**加性耦合层**”（Additive Coupling）。不失一般性，可以将 x 各个维度进行重排，使得 x1=x1:d 为前 d 个元素，x2=xd+1:D 为 d+1∼D 个元素。 




不难看出，这个变换的雅可比矩阵![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3sk5BicaaETJ7gaYLSbpgrxiaYh9EIeTIX5WSuoIP39UL0dZLrPwzoZ8g/640)是一个三角阵，而且对角线全部为 1，用分块矩阵表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3OWgpRPU01hNsq5dicqc5DtITehxswSJt3V5xjDv8c6nEMAa5wXIBaXw/640)




这样一来，这个变换的雅可比行列式为 1，其对数为 0，这样就解决了行列式的计算问题。




同时，(7) 式的变换也是可逆的，其逆变换为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3jaRTK8WYXN7QgcSibBqMhibM0ltWvVoQGqvAzYF4RKbIAtUW7vIwGR1w/640)




**细水长flow**




上面的变换让人十分惊喜：**可逆，而且逆变换也很简单，并没有增加额外的计算量**。尽管如此，我们可以留意到，变换 (7) 的第一部分是平凡的（恒等变换），因此单个变换不能达到非常强的非线性，所以我们需要多个简单变换的复合，以达到强非线性，增强拟合能力。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3InFmycicUcRv8ovtHjZs5CibZA9Dof6uHawoftuVUbc3edmKjKbwoGcA/640)




其中每个变换都是加性耦合层。这就好比流水一般，积少成多，细水长流，所以这样的一个流程成为一个“流（flow）”。也就是说，一个 flow 是多个加性耦合层的耦合。




由链式法则：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3RtpOxHKv2zVLibiaIJKjxv69M0EicGGiaN58FqQhUzkibpIRecJQlWwWalw/640)




因为“矩阵的乘积的行列式等于矩阵的行列式的乘积”,而每一层都是加性耦合层，因此每一层的行列式为 1，所以结果就是：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3Fz3cU5b6ibqBUoeQPabNjwMZeJ76w60v1ghAOn8FFXVrgZia6qoGn5Qw/640)




考虑到下面的错位，行列式可能变为 -1，但绝对值依然为 1，所以我们依然不用考虑行列式。




**交错中前进**




要注意，如果耦合的顺序一直保持不变，即：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3lbIHKLv7fjcLwOQ6805dibqlTuolwDFgJiafLyvhN9zZoZXyg6RRHhDw/640)




那么最后还是 z1=x1，第一部分依然是平凡的，如下图：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3VqvHdibDd8KvL1Z1VFKbO1pWLcR26Yn2HoFbn5IlFdS14IPq9Zo1Dag/640)

▲ 简单的耦合使得其中一部分仍然保持恒等，信息没有充分混合




为了得到不平凡的变换，我们可以考虑在每次进行加性耦合前，打乱或反转输入的各个维度的顺序，或者简单地直接交换这两部分的位置，使得信息可以充分混合，比如：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU33CwksVIsCcVibC7756n64EorrzesfMiaPLibZWDjnst6nzwdYStcJ6N6g/640)




如下图：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3mAh04YwkR3P1tSN15Y1jM7saxT2uOAfVHIwDnKYt2dPv3rCOe7EAdA/640)

▲ 通过交叉耦合，充分混合信息，达到更强的非线性




**尺度变换层**




在文章的前半部分我们已经指出过，flow 是基于可逆变换的，所以当模型训练完成之后，我们同时得到了一个生成模型和一个编码模型。但也正是因为可逆变换，随机变量 z 和输入样本 x 具有同一大小。




当我们指定 z 为高斯分布时，它是遍布整个 D 维空间的，D 也就是输入 x 的尺寸。但虽然 x 具有 D 维，但它未必就真正能遍布整个 D 维空间，比如 MNIST 图像虽然有 784 个像素，但有些像素不管在训练集还是测试集，都一直保持为 0，这说明它远远没有 784 维那么大。 




也就是说，**flow 这种基于可逆变换的模型，天生就存在比较严重的维度浪费问题**：输入数据明明都不是 D 维流形，但却要编码为一个 D 维流形，这可行吗？




为了解决这个情况，NICE 引入了一个尺度变换层，它对最后编码出来的每个维度的特征都做了个尺度变换，也就是![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3SjVI2f3GEFoUZ9Q7eMLZKs8qWbhZZRr0xjHD4WT83c50cELM0f2f9w/640)这样的形式，其中 s=(s1,s2,…,sD) 也是一个要优化的参数向量（各个元素非负）。这个 s 向量能识别该维度的重要程度（越小越重要，越大说明这个维度越不重要，接近可以忽略），起到压缩流形的作用。




注意这个尺度变换层的雅可比行列式就不再是 1 了，可以算得它的雅可比矩阵为对角阵：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3LZssvLdCcLnxD2Sb9uYibOjmauHibliaicnicV3icGibicqFRIKbsPeQmyvQ9Q/640)




所以它的行列式为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3GNBFFv2EUTZU7ctDD2swhPk6icShiaH6MzPo87WvGicLKaZ9GljDsCbVA/640)。于是根据 (6) 式，我们有对数似然：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3Okx7oVpricY9icSm9hqUHWp3JYkB5KrzDicMJPgibdrib1QK0ZWE5qFxYZw/640)




为什么这个尺度变换能识别特征的重要程度呢？其实这个尺度变换层可以换一种更加清晰的方式描述：我们开始设 z 的先验分布为标准正态分布，也就是各个方差都为 1。




事实上，我们可以将先验分布的方差也作为训练参数，这样训练完成后方差有大有小，方差越小，说明该特征的“弥散”越小，如果方差为 0，那么该特征就恒为均值 0，该维度的分布坍缩为一个点，于是这意味着流形减少了一维。




不同于 (4) 式，我们写出带方差的正态分布：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3yr2jYn3TC9TZKYafkiaFicnJLWmbCcLQSXA5ia7KIRUPIwgW6oKzxPLdA/640)




将流模型 z=f(x) 代入上式，然后取对数，类似 (6) 式，我们得到：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3aEyicTg3yp2VFzMVeRgHmtpIicbK4nwkeq8ic867xMEP5RDD5eAFknBEg/640)




对比 (15) 式，其实就有 si=1/σi。**所以尺度变换层等价于将先验分布的方差（标准差）也作为训练参数，如果方差足够小，我们就可以认为该维度所表示的流形坍缩为一个点，从而总体流形的维度减 1，暗含了降维的可能。**




**特征解耦**




当我们将先验分布选为各分量独立的高斯分布时，除了采样上的方便，还能带来什么好处呢？ 




在 flow 模型中，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU30q8NCic9GwZgIOrrvdGP4ibticDwQVzVm707EKHX7Np00KoEicGMDicjpCw/640)是生成模型，可以用来随机生成样本，那么 f 就是编码器。但是不同于普通神经网络中的自编码器“强迫低维重建高维来提取有效信息”的做法，flow 模型是完全可逆的，那么就不存在信息损失的问题，那么这个编码器还有什么价值呢？ 




这就涉及到了“什么是好的特征”的问题了。在现实生活中，我们经常抽象出一些维度来描述事物，比如“高矮”、“肥瘦”、“美丑”、“贫富”等，这些维度的特点是：“当我们说一个人高时，他不是必然会肥或会瘦，也不是必然会有钱或没钱”，也就是说这些特征之间没有多少必然联系，不然这些特征就有冗余了。**所以，一个好的特征，理想情况下各个维度之间应该是相互独立的，这样实现了特征的解耦，使得每个维度都有自己独立的含义。 **




这样，我们就能理解“先验分布为各分量独立的高斯分布”的好处了，由于各分量的独立性，我们有理由说当我们用f对原始特征进行编码时，输出的编码特征 z=f(x) 的各个维度是解耦的。




NICE 的全称 Non-linear Independent Components Estimation，翻译为“非线性独立成分估计”，就是这个含义。反过来，由于 z 的每个维度的独立性，理论上我们控制改变单个维度时，就可以看出生成图像是如何随着该维度的改变而改变，从而发现该维度的含义。 




类似地，我们也可以对两幅图像的编码进行插值（加权平均），得到过渡自然的生成样本，这些在后面发展起来的 Glow 模型中体现得很充分。不过，我们后面只做了 MNIST 实验，所以本文中就没有特别体现这一点。




# 实验




这里我们用 Keras 重现 NICE 一文中的 MNIST 实验。




**模型细节**




先来把 NICE 模型的各个部分汇总一下。NICE 模型是 flow 模型的一种，由多个加性耦合层组成，每个加性耦合层如 (7)，它的逆是 (9)。在耦合之前，需要反转输入的维度，使得信息充分混合。最后一层需要加个尺度变换层，最后的 loss 是 (15) 式的相反数。 




加性耦合层需要将输入分为两部分，NICE 采用交错分区，即下标为偶数的作为第一部分，下标为奇数的作为第二部分，而每个 m(x) 则简单地用多层全连接（5 个隐藏层，每个层 1000 节点，relu 激活）。在 NICE 中一共耦合了 4 个加性耦合层。




**参考代码 **




这里是我用 Keras 实现的参考代码：

https://github.com/bojone/flow/blob/master/nice.py 




在我的实验中，20 个 epoch 内可以跑到最优，11s 一个 epoch（GTX1070 环境），最终的 loss 约为 -2200。 




相比于原论文的实现，这里做了一些改动。对于加性耦合层，我用了 (9) 式作为前向，(7) 式作为其逆向。因为 m(x) 用 relu 激活，我们知道 relu 是非负的，因此两种选择是有点差别的。因为正向是编码器，而逆向是生成器，选用 (7) 式作为逆向，那么生成模型更倾向于生成正数，这跟我们要生成的图像是吻合的，因为我们需要生成的是像素值为 0～1 的图像。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3icEk33DxsbQ3RPNGJcIn4GtSo2ibzibrxibKuzu2e65umoHoCpBAaK3qpg/640)

▲ NICE模型生成的数字样本




**退火参数**




虽然我们最终希望从标准正态分布中采样随机数来生成样本，但实际上对于训练好的模型，理想的采样方差并不一定是 1，而是在 1 上下波动，一般比 1 稍小。最终采样的正态分布的标准差，我们称之为退火参数。比如上面的参考实现中，我们的退火参数选为 0.8，目测在这时候生成模型的质量最优。




# 总结




NICE 的模型还是比较庞大的，按照上述模型，模型的参数量约为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglmO52YDDeJfeFk6ic0mwvU3y2GmgDCH1jGAQGPvwWS1RtQMRaTLv66LibcPEjEBiaWtVryrWFuW5IZQ/640)，也就是两千万的参数只为训练一个 MNIST 生成模型，也是夸张。




NICE 整体还是比较简单粗暴的，首先加性耦合本身比较简单，其次模型 m 部分只是简单地用到了庞大的全连接层，还没有结合卷积等玩法，因此探索空间还有很大，Real NVP 和 Glow 就是它们的两个改进版本，它们的故事我们后面再谈。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看作者其他文章：**




- 
[从无监督构建词库看「最小熵原理」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488802&idx=1&sn=eb35229374ee283d5c54d58ae277b9f0&chksm=96e9caa2a19e43b4f624eac3d56532cb9dc7ca017c9e0eaf96387e20e5f985e37da833fbddfd&scene=21#wechat_redirect)

- 
[基于CNN的阅读理解式问答模型：DGCNN](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488480&idx=1&sn=7bb9a4dd60680df5013670287a9e0cc2&chksm=96e9cc60a19e457618b2ffdea2a7e13ba172ea3fdfbfda07de53aae57126047f1b71a6969d76&scene=21#wechat_redirect)

- 
[再谈最小熵原理：飞象过河之句模版和语言结构](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489564&idx=1&sn=a5d2191dd978e7917f06f66022ce8358&chksm=96e9c79ca19e4e8ac874f0fa5808b99b7323149d42a78fade690c6d60c0337ecabe043f616d1&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)


- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)

- 
[全新视角：用变分推断统一理解生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)





**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 查看作者博客





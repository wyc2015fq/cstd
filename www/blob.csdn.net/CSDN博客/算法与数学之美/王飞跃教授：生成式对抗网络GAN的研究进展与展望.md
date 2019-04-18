# 王飞跃教授：生成式对抗网络GAN的研究进展与展望 - 算法与数学之美 - CSDN博客
2019年02月16日 22:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：326
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkyYNr2mLLFgV8rAcf8TKrhVVxPz4sJSdjDum3ia43TqSj6sPicZVQ5XqpfDhulibnGRdeLqAhyHf8TWw/640?wx_fmt=gif)
> 
***本次汇报的主要内容包括GAN的提出背景、GAN的理论与实现模型、发展以及我们所做的工作，即GAN与平行智能。 ***
生成式对抗网络GAN
GAN是Goodfellow在2014年提出来的一种思想，是一种比较直接的思路。例如，想象我们有两张图片，一张是真的，一张是假的。那么对人类而言，如何去判断这幅画究竟是伪造的还是真的？我们又为什么可以这样认为呢？比如图1中，这个伪造图的问题在于“画中人”画的不对，不是人，而是一只兔子，所以可以认为它是假的。而对于伪造者，他会去想：这里是该画人的地方画的不对，以后在这个地方改进一点，就可以画出更真实的画。第二次，等他画出之后，大家可能又会发现这里那里又有另外一个问题。这样循环迭代，不断去改进，我们就可以提升我们的生成器，也就是这个故事中制造伪画的这个人的水平。这同时我们也提高了侦探的水平。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a7pZGSpPFT9eDlnMl9q2FlO0icbgfOZia5WVuPnEqzOmzz8lvIKDKqxHQ/640?wx_fmt=png)
图1
事实上，GAN提出后很长一段时间，并没有得到很好的发展。图中是在Arxiv上搜索GAN关键词得到的发表结果，可以看到，2016年NIPS会议之前，GAN的文章发表并不是很多，但是16年之后就发展了很多，目前呈一个指数增长的趋势。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aSgiaXjIcIr4RjjMCbn76617HDhOL8JEmDs0EbicSyAWBe5WT62KAdDsQ/640?wx_fmt=png)
![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aZl05n1fLAGJHKWFgbwFI1DGh9XtStkbuvF83moKiaURvBVsHUES2WlQ/640?)
发表GAN文章的作者中，排在第一位的是Bengio，然后有Goodfellow以及国内的余勇教授。他们所研究的主要内容还是计算机视觉，也可以说这两年的研究热点主要还是集中在图像生成这些方面。除此之外，我们可以看到，GAN还涉及许多其他内容，例如关于加密与安全、机器人、甚至包括天体物理的。由此可知，GAN的发展方向正在不断扩大，从传统的计算机视觉向其他方向扩展出去。
LeCun的访谈中提到对于深度学习，10年内最值得关注的一个idea就是GAN，后面我们会讲到这是为什么。首先我们先来看一下已经有的、大家比较熟知的关于GAN的应用。图2中，是使用GAN生成视频中下一帧，相比于MSE方法可能造成下一帧人像没有耳朵，但是使用GAN方法我们可以清晰看到人像是有耳朵的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aiagTtRa3jZu18xmVKahfSC69aF0MXj78y2zRg9N33sX60zI6ZZRcGmA/640?wx_fmt=png)
图2
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3awh2cmLiaWJqRZ9NMMAj1fnSzq590cMHXGLPUQicreSW3G0lib5CnxSPuQ/640?wx_fmt=png)
图3
图3中是使用GAN生成自动驾驶的图像，即使用GAN生成图像，再利用图像去做自动驾驶的训练。此外，在以下组图中，可以看到使用GAN生成超分辨图像，也是计算机视觉领域常用的一种工作。而Adobe的这个工作超出了传统的GAN所进行的工作，即通过人手绘制的线条，自动生成山、草地这类的场景。也有研究做了类似的工作，为了给人添加头发，只需进行简单的涂鸦，就能靠GAN自动生成。如图，是一个3D模型的生成，跟以往相比这是一个比较少见的工作，通过一副图片，或者通过噪音，或添加一些隐藏变量，就可以生成新的3D模型。而图中这个工作是图像变换，即输入一个图像，我们得到一个抽象的图像、或是素描、照片，这样的一个工作也是以往的GAN所没有涉及到的。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aaFQk39O1IVYIqxJ4yZtdIRFeRTB27Gib5PlI8yiansqOaeocPTwXviaibA/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aibPdLvr6icwpwicFHZHSBreib85lveibechN0bPl3Qke9w5GqfYXuHlLFvw/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aCUwOA0oZZtXG764H8H4PfXEpM4QzIo0WFkXgkzJribJ22bRfZNibIAVQ/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aTL1iaIiccB8xDRTuE5jBB5UcIodbsK3ZDR1zDyn1MuSvNHSZZHmuxx7A/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aLB0JOBhM5W8b3g067KrsmRR9zrakLqLchml0EAVsVbGlBiafqRUJrnA/640?wx_fmt=png)
GAN的提出背景
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a37T0sKTbQKlwlhu5ITWlVg1kcQT2hEWSaKPaxXCjJumHL1f8dT6Ticg/640?wx_fmt=png)
**人工智能的研究层次 **
人工智能一开始是从感知开始，渐渐向认知发展。最开始，我们希望机器像人一样去判别什么东西在哪里，但最后我们希望它能够完成一些创造性的工作，即它知道这个是什么，它可以去完成什么。可以看到，在机器学习发展以后，传统的识别问题以已经得到了很大的突破。那么人工智能要向下一步发展，也就是做创造性和应用性的工作。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aovt62dPhFyC6clrNy1t8JibG6CRHD7qR3qZ51oic6avYpic0QsehQbQmw/640?wx_fmt=png)
Feynman说：“what I cannot create，I do not understand”。一个东西我们无法创造，我们就无法真正的理解。我们也知道，Goodfellow在2014年12月发表的关于对抗样本的研究时说到，神经网络的图像分类器，虽然它看起来可以知道它真正分类的是什么，它可以分类出猫、狗、各种东西，比人类高的正确率，然而事实上，它并不真正理解这是什么东西。为什么呢？因为通过加入一些很小的噪音就可以去欺骗它。这说明神经网络并不真正理解它分类的是什么，他只是通过数据建立一种模式，这种模式与真实的世界是有很大差距的。如果我们为它加入一个创造的功能，会有助于它提高理解能力。当说到GAN，其实它的发展也有很长的历史沿革。
早在GAN之前，就有许多关于生成模型的工作。生成模型一般是指随机生成观测数据的模型，那么它有别于一般我们所说的判别模型。生成模型是所有变量的全概率模型，判别模型是在给定观测变量值的前提下，求目标变量的概率。
比如说给定一张图片，它是猫的概率多大？那么这是一个判定模型，是条件概率模型。而这种模型不是生成模型，它几乎没有给信息或者给一个随机的信息，它希望得到一个真实的图片。比如这个常见的例子，即猫的模型，使用GAN生成猫的图像。传统的工作，我们可能只是去判别这是猫还是狗，而不是说让它去生成怎么样。对于GAN模型我们有很多的用处，比如说可以对高维分布的一个表示和处理，包括它可以嵌入强化学习中，生成供agent去学习的数据，或者使用它进行半监督学习，还有很多其他问题都会用到。 
一般来说，人工智能的研究出发点有两个部分，一类就是从人类理解数据的角度去研究生成的东西。对于人类理解数据，我们知道人类的经验是非常有限的，我们必须从某种数学或者现实中能够感受到的直观的例子去学习。那么常见的，在以前的时候我们做生成模型，我会先设定一个分布，比如说高斯分布，那么我们假设我们的图像符合这个分布，只是我们还不知道参数的分布是什么，那我们可以通过数据去拟合这个分布。但是还有另外一种可能性，我们可以用机器或模型直接去理解这个数据。也就是我们不进行任何的假设，我们仅仅是从一个模型让它去生成数据，然后我们再判断这个数据究竟是对的还是错的，是像真实数据一样还是说和真实数据差的太远，那么我们将会根据这个判断反复去修正这个模型，这是理解机器数据的一个角度。可以看到在以往的生成模型研究主要还是在以人类理解数据的角度，我们总希望去使用一个显式的分布假设去设计我们的模型。比如说我们可能会设计一个高斯分布，或者像VAB里面做的，对数据加一个某种分布的噪音，让我们的拟合函数去估计它的分布。但是GAN可以说是第一个比较广泛为人所知的从数据角度出发拟合数据的模型。我们不再拘泥于任何的假设、不在给它任何的数据模分布，我们直接去学习。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a37T0sKTbQKlwlhu5ITWlVg1kcQT2hEWSaKPaxXCjJumHL1f8dT6Ticg/640?wx_fmt=png)
**神经网络与深度学习**
当然，这样一个工作显然离不开现有技术的发展，也就是众所周知的深度学习的发展。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a8lILzaOvwWjiaObEyzkWVyGriaxAU89NqLHqPOocpoSGHXPKQS58DDKQ/640?wx_fmt=png)
我们知道人工智能的发展有三次热潮，已经经历了两次低谷，第三次低谷会在什么时间我们还不知道。在最开始的时候，AI主要进行了定理证明和一些跳棋程序。那么可以看到这里面主要是使用一种规则手段，我们人工去设计它的规则。但是他有这么几个问题，第一个这个规则好像不太适用，我们人类经验有限，我们所设计的规则实际上受制于我们个人经验来说，它不可能包括所有的，那么这会导致我们这个系统没有办法去适应新的数据，同时由于规则使用需要人工去设计方法，它会导致更新也不够快，不能适应大量的数据情况。那么，第二次热潮其实也还是解决这个问题。当时的想法是说如果人工不好，那么我们使用专家系统，将现有的领域专家请过来处理这些问题会不会好一些。这个时候其实神经网络已经开始发展了，但是，大家都知道Minsky证明了传统感知机没法解决亦或问题，这个影响很大，导致进入了第二次低谷期。当第二个低谷期过了之后进入第三次热潮，首先是1991年深蓝战胜了象棋冠军，是IBM的一个工作，实际上这个工作还是基于规则式的模型，但是它使用了一个很强大的算力。我们可以看到第三次要开始的时候计算人工智能发展不仅仅和算法的设计有联系，同时更多的和我们硬件有很大的关系。那么在今天的AI爆发的时候，主要的工作就是云计算、大数据、深度学习这三个方面。可以看到云计算和大数据都是基于互联网的发展和我们现有的数据库设计，使得我们收集数据的能力得到前所未有的加强。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aA73G4EkV5lXf8DfgeAw2O5T8mdZCcA8W6vvNK5wGvGK1VaXicByBz2w/640?wx_fmt=png)
在前两次，我们的数据需要人工收集，耗费了大量的时间，但是进入第三次AI爆发的时候，我们已经有了自动化收集数据的能力。在这个基础上深度学习被提出来去处理这些大规模的数据，它同样也得益于GPU的发展使得以往我们要花很长时间训练的神经网络现在可以很快的训练，最出名的就是去年AlphaGo战胜李世石和今年战胜柯洁。那么在这时候，人工智能，或是深度学习就为大家所知。图中所示是一个深度学习发展的的简史，我们可以看到最早是从1958年开始，有神学家和心理学家对这个东西进行了研究，就是对神经网络这个问题进行研究。但是很快Minsky就开始反对这样的研究，从这里开始就停滞了。直到2007年往后，应到11年12年的时候，ICNet在ImageNet数据集上取得超越传统方法将近10%的正确率的胜利，那么在之后，大公司都推动这样一个新技术，都用它发展。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a37T0sKTbQKlwlhu5ITWlVg1kcQT2hEWSaKPaxXCjJumHL1f8dT6Ticg/640?wx_fmt=png)
**对抗思想**
最后就是对抗思想，在传统的时候我们可能认为神经网络训练时使用的目标函数是需要人工去设计的。我们可能去设计一个MSE或是说其他的方法，我们需要有监督的对它进行学习。但是越来越多的人发现其实我们不需要人工去指定很明确的问题，我们可以用一个很大的反馈去控制它，也就是像AlphaGo这里面说的，我们用两个网络互相博弈，我们训练一个差不多的网络，然后我们两个网络谁输了，那么去调整它。另一方面就是我刚才说的对抗问题。虽然和GAN并没有很明确的关系，虽然他们做的是一样的。我个人认为我们在对抗样本这个案例中可以很明显的看出，神经网络实际上是存在一个本质的缺陷，就是它没有办法真正理解我们所看到的这种。基于这个问题会推动我们工作的下一步发展。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3agR2YwktEPfcEbiax1PZHiaK7DibLdB1PDy8K8NhJj52m28ef0xutcBbtQ/640?wx_fmt=png)
可以看到以上这些内容是对抗思想，将神经网络、以及我们现在的深度模型以及现在的GPU发展结合起来。因此，我们的研究工作就很自然进入下一个阶段，就是应该使用深度学习的模型去进行生成模型研究。 
GAN理论与实现模型
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a37T0sKTbQKlwlhu5ITWlVg1kcQT2hEWSaKPaxXCjJumHL1f8dT6Ticg/640?wx_fmt=png)
**GAN框架**
鉴于上面提到的局限，Goodfellow提出了GAN的思想。他的基本思想是设计这样一个游戏，包括两个玩家，其中一个就是生成器Generator，他的工作就是生成图片，并且使得这个图片看上去就是来自于训练样本，那么另外一个玩家就是判决器Descrimator，那么它的目标当然就是判决输入图片是否真的是训练样本，而不是生成的。它的整个框架大概是这样子，一个随机变量，隐空间的随机变量——它可能采样自一个高斯分布，它也可能是具有某种信息意义上的一个隐变量，他的维度可能在我们真实样本、真实空间的维度以下。就那么将这个东西输入我们的生成器，这是一个可微的函数G(z)。我们会将G(z)与真实样本G都放入我们的Discriminator里面，它尽量判决我们的生成图像是一个假的图像。而G会尽量让我们的判断器误以为这个图像是来自于真实的图像。但是这个D判决是否正确，它会将它的梯度回传给G和D，这也就是为什么我们要强调G和D都是可微函数。因为如果它不可微，这时候我们的误差是无法回传的。那么通过这样一个方式，我们就设计了这样一个原始的GAN结构。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aia48osIe6dMye845pTvLQJx8McKysN9lvjNiaXxb3BCCOjNfPm7yPAMg/640?wx_fmt=png)
当然有很多改进的模型，原始的GAN他的判别器和生成器都是一个全连接网络，用于生成图像。现在主要是CNN也就是使用卷积神经网络去设计我们的输入输出图像。
![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aZmzq40dY4bZxnf2ialPE5nCQ5I2tUwDlw1aHiaKiaAQypUPngkGJftfwg/640?)
对于自然语言理解方面的工作，比如说如果我们有一段文字。我们知道文字不是一个连续的数据，我们不可能像图像一样使用RGB值对它进行训练。那么有很多解决办法，SeqGAN是一个比较常见和大家常说到的一个模型，那么它实际上是将原始的使用增强学习生成的语言模型进行改进，使得它适合可以和我们的GAN结合在一起。
![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3ajLlR4OJlibjF9juaAJag4VUKYqPxhvkXboibkHWYhtjIXfnLKNGhs70A/640?)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a37T0sKTbQKlwlhu5ITWlVg1kcQT2hEWSaKPaxXCjJumHL1f8dT6Ticg/640?wx_fmt=png)
**损失函数 **
GAN的优点，它相比传统模型的优点，是它生成的数据的复杂度和我们的维度是线性相关的。也就是说如果你要生成一个更大的图像，你并不会像传统模型一样面临指数上升的计算量，它只是一个神经网络线性增大的过程。第二个是它的先验假设非常少，那就是相比传统模型最大的一个优点。也就是说，我们这里不对数据进行任何假设，没有假设它服从任何的分布。第三个是它可以生成更高质量的样本。但是这个原因是为什么，目前还没有很好的一个说明。当然，相比传统模型，它有优点自然也有缺点。传统判别模型一般来说也是一个优化函数，我们知道对于凸优化而言，是有最优解的，我们肯定能达到的。但是GAN实际上是在一个双人游戏中去寻找一个纳什均衡点。像纳什均衡点如果对于一个确定的策略，比如神经网络，他是一个确定策略，我们输入一个量肯定是得到确定的一个输出。这时候你不一定确定你能找到一个纳什均衡点。对于GAN寻找和优化纳什均衡点的研究目前来说还是不够的，这个工作实际上是很困难的。 
那么我们具体说一下，刚才我们在上一个模型中，我们说到判断D是否正确，然后将梯度进行回传，那么我们具体怎么判断正不正确？在一篇论文中，提出了这样一个原始的判别器性能函数。它实际上是一个普通二分类问题的交叉熵损失函数。区别在于，他的训练数据分为两部分，一部分来自于真实数据集，一部分来自于生成器。前半部分目标是确保真实数据分类正确，第二部分是希望它能够有效判别出来数据是不是假的。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3axlCRrrIddDajfPonBKmS901ZPo72QoU7Y33l53gQGqIODj8GTsfZgw/640?wx_fmt=png)
前面说到了判别器的损失函数，我们现在说一下生成器的损失函数。GAN在设计中是一个双人零和游戏。也就是生成器损失就是判别器的获得。那么很自然的从这里出发，我们就可以设置我们生成器的损失函数就是判别器的损失函数的负数。在这个设置下，我们又进一步证明了GAN的真实分布和生成分布之前的J-S度量。但是事实上这里有一个问题，就是如果以一个很高的置信度去拒绝G的时候，我们公式的梯度会趋近于零。那么我们看到这个数据的主要来源都会来源于第一项，对于第二项，也就是根据每一个相关的地方来说，那么它的梯度其实就是很小的，在这种情况下你的训练就可能会非常慢。那么这时候提出一个折中的解决方案，也是我们对G的损失函数不再考虑前面一项，也就不再考虑判别器判别真实样本准不准，而只考虑它判别生成的样本。也就是使用这样一个指数函数，公式的第二项，那么我们生成器的最大化目标就是我们判别器的错误率。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aB7uWBiasfF3ofV8iaqW3gBibwdaKMDrMux9uKt4ibuEWIfsG80aeicJfReQ/640?wx_fmt=png)
进一步，我们从概率角度上去考虑这个问题，也就是我们最小化生成模型和真实数据之间的K-L散度。最小化公式中的散度就等于最小化模型到数据的K-L散度。我们不会把生成器训练到最优再去使用，实际使用中一般使用f-divergence。就是在求和符号这里换成f函数。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a37T0sKTbQKlwlhu5ITWlVg1kcQT2hEWSaKPaxXCjJumHL1f8dT6Ticg/640?wx_fmt=png)
**GAN的训练算法 **
那么结合完这些东西我们就可以具体说一下GAN算法。主要是这里面有有几个问题，第一个问题是采样的时候，我们对生成数据采样和优化的时候，一般使用的是mini-batch的方法。也就是说我们会对它批量去处理。为了避免如果每次训练一个样本，会导致比较大的波动。第二个就是原始论文中的算法，实际上现在新的算法有改进，包括对梯度进行裁剪以及其他的事情。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3ajJUbd3eQ8epmpk2ZJaKNRZI8Z8cKbJq8Fk3BHkFx7oQDiciaPcYqrJEQ/640?wx_fmt=png)
GAN的发展与应用
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a37T0sKTbQKlwlhu5ITWlVg1kcQT2hEWSaKPaxXCjJumHL1f8dT6Ticg/640?wx_fmt=png)
**GAN的收敛问题 **
我们看一下现代GAN的发展究竟还有哪些问题。最主要的一个例子就是GAN的不收敛问题，如果大家有试验过可能会知道，GAN的收敛是很困难的。往往就是需要设置很多的参数，比如学习率，比如说你的网络结构，你去很好的去提出G和D的能力，才能使得它最终达到收敛。最主要的原因来源于两部分。第一，就是梯度消失的问题，也就是我们所说的当你、优化的时候，对于公式里我们的生成器、判别器的损失函数会存在梯度消失的问题，那么我们需要设计一些更好的损失函数，使得梯度消失问题得到解决。第二个就是模式发现问题，也就是说我们的生成器可能生成同样的数据而不是多样的数据。例如猫，它可能生成的都是黑猫、或者生成MNIST的数据全都是零。那么这个问题主要原因是来源于，因为我们的梯度下降实际上不区分min-max和max-min。那么会导致我们的生成器会希望多生成一些重复但是很安全的样本。比如说我生了一些黑猫或生成一些0，甚至可能到这样一个情况就是它生成的接真实的就完全一样，但它只适合这样一个情况。那么这是我们不希望，我们希望生成器会生成多样样本，尽量与真实样本多样化一致。
那么我们怎么办呢？这个问题就留在后面我们具体解决，我们这里就简单提一下。
首先，对于第一个问题，我们现在可以使用一些改进的目标函数。包括在16年的一个文章，那么就使用reference和virtual batch normalization。他的基本思想就是从不只是对一个样本，而是对一批样本进行梯度下降。还有LSGAN，即最小二乘GAN和WGAN，还有Loss-SensitiveGAN，这些都是使用不同的损失函数。就是对原始的损失函数进行处理，使得它平滑，不容易梯度消失。还有一种思路比较直接，就是对GAN提供一些额外信息。包括class-conditionGAN，将样本输入给GAN，希望它生成的东西也能被判别器判别为同样的样本。还有就是infoGAN。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a37T0sKTbQKlwlhu5ITWlVg1kcQT2hEWSaKPaxXCjJumHL1f8dT6Ticg/640?wx_fmt=png)
**增强学习与模仿学习 **
除了GAN的理论问题，我们还有GAN的应用问题。那么我这里主要讲GAN和增强学习的应用。16年有一篇文章讲到了GAN和Actor-Critic相似在什么地方。如这张图所表示的，我们可以看到GAN的网络结构就是类似于左边这个，而Actor-Critic作为增强学习的一种方法，它的结构是右边这种。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3auX4UJB4s3Tr7S1dibyHRqk3j1sUu5ZNvVA4YYlkAszeIry6GHeuxNfw/640?wx_fmt=png)
对于GAN来说，z输入给G，然后由G的输出给进去判别究竟是对还是不对。而对于Actor-Critic，我们会将环境中的状态输入给π，然后由π 来输出给Q，也就是对于我们的Cridit评判者去评判我们的策略。我们看到增强学习和GAN的区别就在于这个z，因为我们知道这个环境的状态实际上具有一定的随机性，而在GAN中，这两个被分离开来。我们看到这两个东西的结构上具有一致性，但是它们是不是训练方法具有一致性呢？实际上是确实是这样，在Actor-Critic中可以用的大部分方法，都有人研究如何把增强学习中的性能稳定的方法移植到GAN中。那么另一方面就是GAN可以给增强学习提供一些数据。对于增强学习，数据是很稀缺的，我们可能很久也得不到这样一个数据，或者说我们的数据采样很昂贵，我们需要很高的代价去采集它，那我们可以设计一个这样的框架，enhancedGAN，我们将GAN生成的数据给增强学习去处理。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aC0pBPHticichUnQ9FUM9Ixj0cBvAICI8XeZSpMBYmyrsByR3qejAhj0Q/640?wx_fmt=png)
对于模仿学习，16年的这篇文章，需要我们从专家样本中学习到一个价值函数reward function，再用这个reward function去训练我们的reinforcement learning agent。在篇文章中，方法很直接，我们不直接学习reward function，而是直接学习状态到行为的映射(st, at) 这里面的st 相当于GAN中的G，而at 相当于GAN中的G(z)，我们通过这样的结构直接将专家的行为定义为映射，而不需要经过中间的步骤。
以上就是到目前为止传统的GAN的进展，那么后面我们会介绍一下GAN与我们做这些工作的进展。 
GAN与平行智能
![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a37T0sKTbQKlwlhu5ITWlVg1kcQT2hEWSaKPaxXCjJumHL1f8dT6Ticg/640?)
**赛博崛起：智能与时代**
大家都知道AlphaGo，其实我认为AlphaGo的成功并不是来源于深度学习，而是来源于对抗，与GAN一样的思路。
计算机发展到今天靠的是图灵，这是一个假设，是无法证明的。但冯·诺伊曼提出这个假设，所有可以计算的数都可以用图灵理论计算，这个东西是无法证明的，而且图灵只能算有限的数，而实际上能算的数应该是无限的。但是根据这个假设，从冯·诺伊曼，发展到今天的计算机行业和计算机信息行业。AlphaGo产生的效果，就是类似于Church-Turing Thesis一样的效应。
![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aK56q6nmM6aJkTEMfZ8cv8MqvSrVyTUo5MCNFS4USzJuPicgRq6G1OyQ/640?)
AlphaGo给了一条路，这条路是从牛顿到默顿，从小数据到大数据再到小知识的过程，是智能技术和智能产业时代的到来。AlphaGo之后，IT不再是信息技术，是智能技术（Intelligent Technology）。200年前IT是工业技术的意思，这跟波普尔的三个世界理论有密切相关，我只知道物理世界、心理世界，但是波普尔告诉我们还有第三世界，人工世界，所以今天是人工智能时代。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a7xicxbRMH6efxf2EiaibsibhncwJVJGZY3xYTZTkdjictnrJLFNZiaWsRY2g/640?wx_fmt=png)
围绕着物理技术发展了从农业技术到工业基础，围绕着心理世界发展了信息技术。走到今天，下面要开发的是第三世界，是人工世界，大数据变成资源，人口、智力都变成了资源，这就是导致IT变成新IT。从工业时代到信息时代，再到智能时代，这是平行的世界。
这个社会的基础结构就是这么来的，整个社会基础结构最初就是一张交通网。2000年有了全世界第一个高速公路网，直到后来工业时代到来，我们才有了电力网、能源网、电话网，发展到今天的互联网，现在开始做物联网。但是我觉得物联网是一个过渡，很快就应该到了智联网。今天已经不需要我来解释什么是智能产业，这个智能产业需要智能的基础设施，这个基础设施就是CPSS，各种各样的5.0，把社会空间和物理空间、信息空间、虚拟空间结合在一起的时候才能实现人机结合，才能做到哲学上的知行合一，你掌握的知识和采取的行动是一致的，最后达到虚实一体的境界。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aPEwb0nyzU7TEzNIic2dNZbK5WfVoaKdxHbiaYeSvcwCcS93O0DhrzbPg/640?wx_fmt=png)
31年前，Minsky的《心智社会》，给人工智能提出了深刻变革。它和卡尔·波普尔的三个世界有什么联系？这五张网把三个世界圈在一起。我们围绕着物理世界建交通网，围绕着心理世界建立信息网，现在叫做虚拟世界、人工世界，叫智联网。在这中间有过渡，有网络2.0，能源网，有网络4.0，物联网。
现在很自然迈向工业5.0，1.0是蒸汽机，所以大学中有机械系，后来又来了电动机，大学又有了电机系，现在大学里还有几个研究电机的，然后有了计算机，大学开始有了计算机系，现在研究算法、计算结构的也变成少数。物联网，路由器的兴起，很多学校已经开始物联网学院，但是我觉得只是一个过渡，已经到了平衡时代，也就是智能时代。
这不是要和德国工业4.0对抗，工业4.0是很好的理念、口号、营销策略，它的实质就是ICT+CPS。我们对这两个词的理解不一样，什么是ICT？什么是CPS？4.0是过去上的理解，是信息与通讯技术，我们把他认为智能联通技术，把CPS认为信息物理，我们认为信息物理社会系统，导致的后果就是不同的理解，不同的时代。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3ayK1MK3QAEJsor0ea0MnFnXll32ibZ9MUAia4pVXfvHYj9PkeTBT8lwLw/640?wx_fmt=png)
这就带来了一个新时代。卡尔·雅斯贝斯和马克思是同一个名字，而且马克思死的那年他生了，中华人民共和国诞生的那一年他写了《历史的起源与目标》，他在书中提到“轴心时代”的概念，公元800年到公元600年期间，世界人性大崛起，中国出了老子、孟子、孔子、庄子、孙子，古希腊出了苏格拉底、伯拉图、亚里士多德，还有犹太教、印度教、中东教，是第一轴心时代，是哲学上的突破。在轴心时代的末尾，西汉开始了丝绸之路，第一次全球化，可惜那时候技术不支持，后来来了第二次轴心时代，这是我个人的说法，因为很多人认为第二次轴心时代刚刚开始，是宗教上的变革。我认为文艺复兴带来的从哥白尼到伽利略、牛顿、爱因斯坦，这就是理性大崛起，带来了科学。在这个时代开头的时候，也是600年，从1400至1900，在这个时代的开始，中国又开始第二次全球化，郑和下西洋，结果什么都没带回来，结果有了哥伦布大发现，发现美洲，实现了真正的第一次全球化，我叫它复合全球化，因为殖民，拿着枪指着你。
在这个时代之末，二战结束之后，美国开始马歇尔计划，全球开始了自由贸易开始，零和，只卖给你东西，不给你技术，你必须老老实实待在原来的位置，你和平崛起他都不认。但是现在这个时代开始了，从哥德尔的不完备定理，就是因为哥德尔的不完备定理所以大家才往人工智能和计算机上发展，这是智性的大崛起带来技术的突破，就是今天的智能技术。
第三轴心开始，中国开始“一带一路”，这个轴心开始是正和，是包容。中华做人工智能第一人是王皓，王皓以前在西南联大，和冯友兰、金岳霖学哲学，后来到哈佛学哲学和逻辑。60年代，最简单的计算机把罗素和怀特海在数学原理中上百个定义，在十几分钟就证明出来了，是定理证明第一人。我们这个领域很有名的赫伯恩奖第一个就给了他，后来吴文俊也得了这个奖。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a6hT1IsSBSAvjqZ4PT9lIGvN3v4IUEHChekFBIqpQZshAia0xnINMN6A/640?wx_fmt=png)
他晚年和哥德尔研究哥德尔定义，成立了世界上第一个哥德尔学会，他任首任会长。他写了两本书，他说哥德尔的思想最后是要证明他的定理不太适合于数学方式，在人类社会上也适合，我把它广义化。广义的哥德尔定理，算法所表达的智能永远小于人类用语言能表达的智能，能用语言表达出来的智能永远小于大脑中存在的智能，这就是我为什么从来不相信奇点理论，永远不相信人工智能超越人类。
老子《道德经》的前两句话是“道可道非常道”，实际上就是广义的哥德尔定理。哥德尔后来就是在研究怎么把他的定理推广到社会领域中去。
为什么给大家讲这个？是因为现在进入了一个新的时代，我称之为第三轴心时代。我们现在就处于一个爆炸，就是智能的爆炸，理性已经到头了，必须要靠人类的智性。到了第三世界，从理性认可变成了数据认可，这便是默顿定理。
所以我认为，像GAN这类算法只是开了个头。我当然希望大家能好好掌握这类方法，就应该开创出自己的方法。将小数据导成大数据，将大数据导成小知识。实现从牛顿系统到默顿系统的跨越。它的基础设施基础结构就是虚实互动的平行。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a37T0sKTbQKlwlhu5ITWlVg1kcQT2hEWSaKPaxXCjJumHL1f8dT6Ticg/640?wx_fmt=png)
**平行系统与平行学习**
所以我们这里做的，就是人工组织，就是个模型。不仅有物理的模型，还要把行为模型加进去。怎么导成大数据？就是靠计算实验，好多实验，物理实验做不了，因为受到成本、法律、道德、科学上无可行性的约束。然后通过把模型的行为和实际的行为在CPS上进行大的反馈，因此就可以做平行执行。
所以以后所有的系统都应当是实际加人工。可以一对一、多对一、一对多、多对多。它与传统的并行的区别在于并行是分而治之，而平行是扩而治之。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a7HeDibFK01znu1IQUicyic81Non5JvXsjesNCZgH6vVJcpCMQrAnCDicSA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3ay0KO9FH9hribPXJTwv3SEmjJDVgbs1Nd0A1dQtvmeZeOlHncr9BTyUQ/640?wx_fmt=png)
牛顿系统与默顿系统的区别在于，对于牛顿系统，你的分析不改变它的行为，而默顿系统中，你的分析一定会改变它的行为。模型与实际行为会产生一个巨大的鸿沟。这个鸿沟一是靠数据填补，二是靠平行加持。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aULodFjl7r0SibFqgJy4aw9hKT0jldRTyWA8ZZj6ZSMC2SUibIAjrUDDg/640?wx_fmt=png)
其实GAN就是一个最简单的平行系统，它两边都不全。它用一个判别器实现物理的系统，利用生成器产生人工系统。对我而言，这就是未来的平行机。它打通了3个世界：物理世界、人工世界、赛博空间。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3aR5NB1O9nqpZKwhnOicrdFhMibn19tibicORs2iaPt9SWLdxmS4fspsSMeCw/640?wx_fmt=png)
所以以后一切都应当是平行的，驾驶是平行的，视觉是平行的、平行数据、平行材料等等。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/47ibaBJyUH46C7H3tg8Mg07tLuA4O1q3a4m7DNIVl4q0cibfTfJkibfEgFe9H8a3IicCDib8YhcjsiaPnoqINwDtvHkw/640?wx_fmt=png)
所以我希望大家可以在课上完了之后，沿着平行的思路再想一想，把这一类方式变成牛顿到默顿的特例。我也很愿意在这个方向支持大家。谢谢。
**————**
编辑 ∑ Gemini
来源：自动化学报
微信公众号“算法数学之美”，由算法与数学之美团队打造的另一个公众号，欢迎大家扫码关注！
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyjQVC3BFiaFKQ4DJqz2xhrwkzeCPbjQdnnG8678fRf1sxc2ZQtvtVib2dqWUkeopYtmgckINoOoGoQ/640?wx_fmt=jpeg)
**更多精彩：**
☞[如何向5岁小孩解释什么是支持向量机（SVM）？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483726&idx=1&sn=e5e008fb68a7d837546d0ac5b5438042&chksm=ebe9cbf3dc9e42e5d625b2da6b9b3866dff9f08d442d8106f4cbf035d8602e1fdda86eec6476&scene=21#wechat_redirect)
☞[自然底数e的意义是什么？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483726&idx=2&sn=18272b7eaa172794b51c30d0a2dd9c48&chksm=ebe9cbf3dc9e42e5ddf9a189822a2fa099543a631ad63a1d6ed0158b51c76212eb65ebbfe71b&scene=21#wechat_redirect)
[☞费马大定理，集惊险与武侠于一体](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483715&idx=1&sn=9069dadf4bbce2aa34bd64b85a69dcee&chksm=ebe9cbfedc9e42e81c27d72da15c0dbf848e505946f231051b8b4033d0941bc6f51cef32790e&scene=21#wechat_redirect)
☞[简单的解释，让你秒懂“最优化” 问题](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483715&idx=2&sn=55a16f37c89b27994b263e0dc9837561&chksm=ebe9cbfedc9e42e842deb581ea62b750cedd839abd58c2db3261bf9fbcd172a2cf18512e4d2d&scene=21#wechat_redirect)
[☞一分钟看懂一维空间到十维空间](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483676&idx=1&sn=2366a39bca3ef42a6e868e91ea718813&chksm=ebe9cba1dc9e42b70c7e147b9e43828c1a7c68401f442890471a06e5cf0704437f9813ca0e0f&scene=21#wechat_redirect)
☞ [本科、硕士和博士到底有什么区别？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483661&idx=1&sn=d822666a054ba70b37dfb06d14c60f3a&chksm=ebe9cbb0dc9e42a6c476f7f81095b772aa45d960bf516f60c5b2e1155c9093696222cea0a83d&scene=21#wechat_redirect)
☞[小波变换通俗解释](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=1&sn=7d0d05c78cd01df91495f1d14609cbce&chksm=ebe9cbbbdc9e42add13cfe99f3383745fa5c059df705a3a9e28644d073dff804569af94970e3&scene=21#wechat_redirect)
☞[微积分必背公式](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=4&sn=ce88086b650c601bdbf57ecfe5a490a1&chksm=ebe9cbbbdc9e42adfaf0e4ee644d254835c830ef47663315b70a39a2b47e6a7cf10d0826b88d&scene=21#wechat_redirect)
☞[影响计算机算法世界的十位大师](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=3&sn=ceaec6043bb0e8a851033482f8f572bf&chksm=ebe9cbbbdc9e42ad30fd38383cf1caa609ac6e81964da17277f8e2a7f17a933cd11e0f3840c8&scene=21#wechat_redirect)
☞[数据挖掘之七种常用的方法](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=2&sn=bf439d56bc7d42083708fa76434a6025&chksm=ebe9cbbbdc9e42ad9e5f2b3c1952e620e0e3d4452aae25b611e7e54be8678b0d80e002e7be6d&scene=21#wechat_redirect)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com

# 样本增广自动化-AutoAugment论文解读 - 知乎
# 

## **样本增广自动化-AutoAugment论文解读**

本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
- [书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
- [书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)

**PDF全文链接：**[样本增广自动化-AutoAugment论文解读](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_144.html)

Google DeepMind (2018)

本文深入研究了图像的数据增强方法，提出了一种简单的基于搜索的数据增强方法AutoAugment. 作者的主要思想是创建数据增强策略的搜索空间，并直接在一些数据集上评估特定策略的质量。 在本文的实验中，作者设计了一个搜索空间，每一种策略由许多子策略组成，在每一个batch中都会为每一张图像随机选择一种子策略。子策略包含两个操作，每个操作都是一种图像处理方法，如平移，旋转或剪切等，对于每一个操作都有一组概率和幅度来表征这个操作的使用性质。本文使用搜索算法, 搜索使神经网络在目标数据集上获得了最高的验证集精度的最佳策略, 在没有额外数据的条件下实现了CIFAR-10，CIFAR-100，SVHN和ImageNet上state-of-the-art的精度。在ImageNet上，AutoAugment获得了83.54％的top 1精度。在CIFAR-10上，AutoAugment实现了仅1.48％的错误率，比之前state-of-the-art的结果提高了0.65％。在reduced CIFAR-10数据集上, AutoAugment在不使用任何未标记数据的情况下做到了与半监督方法相同的性能。最后，从一个数据集中学习的策略可以直接转移到其他类似的数据集上并且表现的同样好。例如，在ImageNet上学到的策略无需使用新的数据对预训练的模型进行微调便可以做到在细粒度的视觉分类数据集Stanford Cars上实现state-of-the-art的精确度.

## **一.概述**

在有海量数据支持的情况下, 深度神经网络可以说是机器学习系统里强大的一个分支。数据增强是一种通过随机“扩充”来增加数据量和多样性的策略[1–3]; 在图像领域，常见的增强包括将图像平移几个像素，或者水平翻转图像。 直观地说，数据增强用于”教会”模型数据域中的不变性：如对图像进行分类通常对水平翻转或图像变换不敏感。 一些设计出来的神经网络可以被用来硬编码(通过某种精巧的设计来获取)不变性[3–6]:如卷积神经网络拥有平移不变性，而另一些物理模型也具有自身不变性:如平移，旋转和置换的不变性等等[7–12]。 但是使用数据增强来获得潜在的不变性要比直接将不变性硬编码到模型体系结构中容易的多。

然而，机器学习和计算机视觉领域的绝大多数工作是在设计更好的网络架构[13-21], 而较少的关注寻找包含更多不变性的更好的数据增强方法。 例如，在ImageNet比赛中，2012年推出的数据增强方法仍然是目前最通用的方法(只有很小的变化)。 即使一些特定的数据集已经找到了用于自身的独特数据增强方法，但它们通常也不能有效地转移到其他数据集上去。 例如，由于不同数据集中存在不同的对称性要求，因此在训练阶段水平翻转图像是CIFAR-10上的有效数据增强方法，但不是MNIST上的有效数据增强方法。 因而, 让网络自主的寻找或选择数据增强方法逐渐成为一种无论是学术或者工程上都不可或缺的需求

在本文中，作者致力于把为目标数据集寻找最有效的数据增强策略这一过程自动化。 在本文的实现中(附录)，每个策略包含了可能的增强操作的若干选择和顺序，其中每个操作是一种图像处理功能（例如，平移，旋转或颜色标准化），并包含执行操作的概率和幅度。本文使用搜索算法来找到这些操作的最佳选择和顺序，使得训练神经网络产生最佳的验证集精度。 在本文的实验中，作者使用强化学习[23]作为搜索算法，但作者相信如果使用更好的搜做算法[21,24]，结果可以进一步改善。

本文的方法在很多数据集上实现了没有附加数据的情况下state-of-the-art的精度(如CIFAR-10，reduced CIFAR-10，CIFAR-100，SVHN，reduced SVHN和ImageNet)。在CIFAR-10上，AutoAugment实现了仅1.48％的错误率，比之前state-of-the-art 的方法[21]又提升了0.65％。在SVHN上，AutoAugment将state-of-the-art的错误率从1.30％[25]提高到1.02％。在一些reduced数据集上，本文的方法在不使用任何未标记数据的情况下实现了与半监督方法相当的性能。在ImageNet上，AutoAugment实现了前所未有的83.54％的精度。最后，本文阐明了在一个任务上搜寻到的策略可以很好地移植到不同的模型和数据集。例如，ImageNet上的策略可以显著改善一系列FGVC数据集。即使对于某些使用ImageNet上的pretrain模型去做fine-tuning没有显著提升的数据集[26]，例如Stanford Cars [27]和FGVC Aircraft [28], 采用ImageNet策略进行数据增强和训练，测试集误差也可以分别降低1.16％和1.76％。该结果表明，迁移数据增强策略为迁移学习提供了另一种途径。

## **二.相关工作**

在图像识别领域, 常见的数据增强方法都已经被手工设计并广泛应用，不同的数据集需要使用不同的增强策略来获得最佳的效果。 例如在MNIST上，大多数排名靠前的模型使用弹性形变，缩放，平移和旋转[2,29-31]来进行数据增强。 而在自然图像数据集上(如CIFAR-10和ImageNet)，随机裁剪，图像镜像和色彩变换/白化更为常见[3]。 由于这些方法是手动设计的，所以不同的数据集可能需要某些特定领域的专业知识和设计时间. 而本文通过数据学习数据增强的策略理论上可以用于任意数据集.

本文介绍了一种从数据中寻找数据增强策略的自动化方法: 本文的方法受到Architecture search最新进展的启发(使用基于强化学习和基于进化的算法(evolution)从数据[19,21,23,32-40]中发现模型架构)。 虽然这些方法已经在人工设计的架构上得到了改进，但仅使用架构搜索在CIFAR-10数据集上仍存在着2％错误率的瓶颈。

很多工作都曾尝试学习数据增强的方法，包括 : 通过合并来自同一类的两个或更多个样本来自动生成增强数据的网络[41], Tran等人使用贝叶斯方法从训练集中学习数据的分布来生成更多数据[42], DeVries和Taylor在学习的特征空间中使用简单的变换来增加数据[43].

生成对抗网络也可以被用于生成附加数据[44-48]: 本文的方法和生成模型之间的关键区别在于本文的方法生成符号转换操作，而生成模型（如GAN）直接生成扩充数据。 (一个例外是Ratner等人的工作，他使用GAN生成描述数据增强策略的序列[49]。)

## **三.AutoAugment**

本文将寻找最佳增强策略的问题形式化为离散搜索问题: 在本文的搜索空间中，一个策略由5个子策略组成，每个子策略由两个按顺序执行的图像操作组成，每个操作还与两个超参数相关联：1）执行操作的概率 2）执行操作的规模。

![](https://pic2.zhimg.com/v2-68e80c4f4f7488cfd81f72015972b9cd_b.png)图1

图1：展示在SVHN上找到的策略之一，以及如何在给定用于训练神经网络的原始图像上生成增强数据。该策略有5个子政策。对于一个mini-batch中的每个图像，作者随机均匀地选择子策略以生成变换图像以训练神经网络。每个子策略由2个操作组成，每个操作与两个数值相关联：调用操作的概率和操作的大小。因为调用操作存在概率，因此有一定可能不会在该mini-batch中应用该操作。但是如果应用，则操作固定大小。作者通过展示如何在不同批次的mini-batch中对同一个示例图像进行不同的转换来强调应用子策略的随机性，即使使用相同的子策略也是如此。正如文本中所解释的，在SVHN上，AutoAugment会更频繁地选择几何变换, 同样也可以看出为什么Invert是对SVHN的常用操作，因为一些图像中的数字对于该变换是不变的。

图1展示了本文的搜索空间中包含5个子策略的策略示例。 第一个子策略指定了对ShearX 和Invert的执行，其中调用操作ShearX的概率为0.9，当执行时操作幅度为7，之后是执行Invert，概率为0.8。反转操作不使用幅度信息. 作者强调这些操作是按指定的顺序执行的。

Search space of operations:

本文实验中使用的操作均来自PIL(一种流行的Python图像库). 为了一般性，作者考虑了PIL中接受图像作为输入输出的所有函数, 除此之外本文还使用了另外两种最新的增强技术：Cutout [25]和SamplePairing [50]。 所以本文搜索的操作包括ShearX/Y, TranslateX/Y, Rotate, AutoContrast, Invert, Equalize, Solarize, Posterize, Contrast, Color, Brightness, Sharpness, Cutout [25], Sample Pairing [50] 。总的来说，本文在搜索空间中有16个操作, 每个操作还带有默认的幅度范围(将在第4部分中详细介绍)。作者将幅度范围离散为10个值（均匀间距）以便使用离散搜索算法来查找它们, 同样本文也将将该操作应用的概率离散化为11个值（均匀间距）。因此寻找每个对应子策略成为了（16×10×11）2种可能的空间中的搜索问题。本文的目标是同时找到5个这样的子策略(以增加多样性), 具有5个子策略的搜索空间则具有大致（16×10×11）10≈2.9×1032种可能。

本文使用的16个操作及其默认值范围可以在附录的表1中查看。 值得注意的是本文的搜索空间中没有明确的"Identity"操作; 此操作是隐式的:可以通过调用概率设置为0的操作来实现。

Search algorithm details:

作者在实验中使用了基于强化学习的搜索算法(灵感来自[19,23,32,51])。 搜索算法有两个组成部分：一部分是控制器(controller,一个递归神经网络)，另一部分则是训练算法PPO(Proximal Policy Optimization)算法[52]。 每一步操作中，控制器对softmax输出的结果预测产生决策(特征向量); 然后将决策作为下一步操作的嵌入向量。总的来说，控制器拥有30个softmax来分别预测5个子策略的决策，每个子策略又具有2个操作，而每个操作又需要操作类型，幅度和概率。

控制器通过奖励信号进行训练，奖励信号定义为某个策略对于原始模型(“child model” : 作为搜索过程其中一部分而训练的神经网络)泛化能力的优化程度。在本文的实验中，我们预留了一个验证集来衡量子模型的泛化能力, 通过在训练集上应用5个子策略（不包含验证集）生成增强的数据来训练子模型, 并对于mini-batch中的每个样本随机选择5个子策略中的一个进行增强, 然后在验证集上评估子模型的精度，评测的结果被用作递归神经网络控制器的奖励信号。在每个数据集上，控制器对大约15,000个策略进行采样。本文遵循[19]中的训练和超参数寻优方法。在搜索结束时，作者将来自5个最佳策略的子策略合并为单个策略（具有25个子策略）, 这个包含25个子策略的最终策略组被用于训练所有数据集对应的模型。

上述搜索算法只是可用于查找最佳策略的众多可能搜索算法之一,不同的离散搜索算法(如遗传算法等)均有可能被用来实现本文的算法并实现对作者结果的进一步提高.

## **四.实验**

在这一部分，作者经验性的研究了AutoAugment在CIFAR-10 [53]，CIFAR-100 [53]，SVHN [54]和ImageNet [55]数据集上的表现。作者不仅描述了用于查找数据增强策略的实验, 同时还解释了如何使用这些策略来对模型进行训练并在这些数据集上获得state-of-the-art的结果。

CIFAR-10 and CIFAR-100 Results:

虽然CIFAR-10有50,000个训练样例，但本文的实验包含在一个较小的数据规模上搜索最佳策略(将其称为“reduced CIFAR-10”) 以节省在搜索过程中训练子模型的时间，减少后的数据集仅包含4,000个随机选择的样本。 作者发现在相同时间内，让模型在较少数据上训练更多epoch的结果比在较多数据上训练较少epoch的结果好得多。对于child model，作者选择了小型Wide-ResNet-40-2（40层 - 扩展因子2）模型[56]，并训练120个epoch。因为每个子模型都是从头开始训练来计算控制器的梯度更新, 因而使用小型Wide-ResNet可以提高计算效率。作者使用10-4的权重衰减，0.01的学习率和一个退火周期的余弦学习衰减[57]。

在搜索reduced CIFAR-10数据中找到的策略之后被用于训练CIFAR-10，reduced CIFAR-10和CIFAR-100的最终模型。 正如之前所说，本文将来自5个最佳策略的子策略连接起来，形成具有25个子策略的单个策略，该策略用于CIFAR数据集上的所有AutoAugment实验。

基线模型的预处理使用state-of-the-art的CIFAR-10模型的惯例：标准化数据，使用50％概率的水平翻转，零填充和随机裁剪，最后切割16x16像素[19,21,58,59]。 作者在标准基线的预处理之上应用AutoAugment策略：在一个图像上，首先使用现有基线方法提供的增强策略，之后应用AutoAugment策略，最后应用Cutout。 本文没有优化Cutout区域大小，并使用默认的16像素值[25]。值得注意的是: 由于Cutout是搜索空间中的操作，因此Cutout可能会在同一图像上使用两次：第一次使用学习后的区域大小，第二次使用固定区域大小。 但实际上，因为第一种可能中Cutout操作的概率很小，因此Cutout通常在给定图像上只使用一次。

在CIFAR-10数据集上，AutoAugment主要选择的其实是基于颜色的转换,如:CIFAR-10上最常采用的转换是Equalize，AutoContrast，Color和Brightness, 而像ShearX和ShearY这样的几何变换很少在较优的策略中出现, Invert则几乎从未在策略中使用过。附录中包含了作者通过实验在CIFAR-10上搜索到的最优策略。接下来，作者使用在reduced CIFAR-10搜索到的策略对于完整CIFAR数据集进行实验, 以下所有的实验结果都是实验5次后取得平均值.

CIFAR-10 Results:

在表1中展示了不同神经网络架构上的测试集精度。本文在TensorFlow [60]中实现Wide-ResNet-28-10 [56]，Shake-Shake [58]和ShakeDrop [59]模型，并为使用基线增强的模型找到了令其验证集精度最优的权重衰减和学习率超参数, 其他超参数与模型论文中默认值的相同[56,58,59], 本文最终使用相同的模型和超参数来评估AutoAugment的测试集精度。对于AmoebaNets，本文在基线和AutoAugment增强模型中使用与[21]相同的超参数。从表中可以看出的，本文使用ShakeDrop [59]模型实现了1.48％的错误率，比state-of-the-art的模型[21]提升了0.65％。 值得注意的是，AutoAugment带来的增益远大于AmoebaNet-B对ShakeDrop（+ 0.18％）以及ShakeDrop对Shake-Shake（+ 0.25％）的增益。而本文提出的方法也是可以打破2%瓶颈的第一种方法。

![](https://pic4.zhimg.com/v2-b92d857d1ffa07c174b214755eee34d7_b.png)表1


表1：CIFAR-10上的测试集错误率（％）。 本文的实验中复现了基线模型和具有Cutout的基线模型的所有结果，并与之前的结果相匹配[25,56,58,59]。 一个例外是Shake-Shake（26 2x112d），它具有比[58]中最大模型更多的filter(112vs96)，因而没有对比结果。 值得注意的是，最佳策略是在reduced CIFAR-10上找到的。

CIFAR-100 Results:

本文还使用与CIFAR-10相同的AutoAugment策略训练CIFAR-100上的模型; 结果显示在表2中。同样，本文在该数据集上获得了state-of-the-art的结果，打败了由ShakeDrop正规化[12]实现的12.19％误差率的最高记录。

![](https://pic1.zhimg.com/v2-564dd2a4c49e2c5a3ddc02c7376d2114_b.png)表2

表2：CIFAR-100上的测试集错误率（％）。 Wide-ResNet和ShakeDrop的基线和Cutout结果在本文的实验中被复现，并与之前的结果相匹配[25,59]。 同样，最佳策略是在reduced CIFAR-10上找到的。

A comparison against Semi-Supervised Learning:

最后，作者使用相同的AutoAugment策略来训练reduced CIFAR-10的模型(与用于查找最佳策略相同的4,000样本的训练集), 该实验方法与半监督学习领域[61-65]广泛采用的方法类似。目前半监督学习领域现有的错误率从一致正则化（Π-M）[64]的12.36％到虚拟对抗训练和熵最小化[62]的10.55％，所有这些实验都引入了另外46,000个未标记的样本。 本文使用Wide-ResNet-28-10 [56]实现了表3中显示的结果, 仅略低于半监督方法中的state-of-the-art错误率（如[65]所总结），而后者在训练中使用了额外的46,000个未标记的样本。当作者使用Shake-Shake模型时，在不使用任何未标记的样本的条件下显示出了优于所有之前的半监督学习方法的性能.

我们注意到，与完整数据集精度的提升相比，AutoAugment使reduced CIFAR-10数据集精度的提升结果更显得重要。 随着训练集的大小和多样性的增加，可以预料的是数据增强的效果将会降低。 但是在接下来的部分中，作者将展示即使对于像SVHN和ImageNet这样的大型数据集，AutoAugment仍然可以从一定程度上提高泛化精度。

SVHN Results:

作者在SVHN数据集[54](包含73,257个训练样本（也称为“核心训练集”）和531,131个额外的训练样本)上也进行了实验,其中测试集有26,032个样本。 为了减少搜索时间，作者建立了一个从核心训练集中随机抽样1,000个样本的简化SVHN数据集。 作者使用AutoAugment来查找最佳策略, Child model的模型架构和训练过程与CIFAR-10实验相同。

![](https://pic2.zhimg.com/v2-289a7890b224d7cba96af65f9ebaef91_b.png)表3

表3：reduced CIFAR-10的测试集错误率（％），选择了与用于查找最佳策略的训练集相同的4,000个训练样本。本文的结果与10.55％的半监督学习中的state-of-the-art相当，且不使用额外的数据。

在SVHN上选择的策略与在CIFAR-10上选择的不同, 如SVHN上最常采用的操作是Invert，Equalize，ShearX / Y和Rotate, 但之前的实验中我们知道Invert操作几乎从未在CIFAR-10上使用过. 直观地说这是有一定道理的，因为数字的特定颜色不如数字与背景的相对颜色重要。 此外几何变换ShearX / Y是SVHN上最常见的两种变换,这也可以通过SVHN中图像的一般属性来理解：门牌号通常在数据集中自然倾斜的，因此通过数据增强来学习这种变换的不变性是有帮助的。 图1可视化了SVHN样本中的五个最优子策略。

在搜索结束后，作者把5个最优策略链接起来，并将它们应用于在SVHN上表现良好的使用标准数据增强策略的训练模型上。对于完整数据的训练过程，作者遵循Wide-ResNet论文[56]中提到的使用核心训练集和额外数据的通用方法. 通过预留训练集的最后7325个样本来构建验证集, 并根据验证集性能来调整权重衰减和学习率, 而其他超参数和训练细节与模型论文中的相同[56,58]。但由于完整的SVHN数据集规模较大，作者仅对Shake-Shake模型训练了160个epoch（而不是论文中的1,800个）。遵循[25]中描述的方法, 基线预处理策略包括了标准化数据和应用区域大小为20x20像素的Cutout。AutoAugment结果实际上是将基线预处理与在SVHN上学习到的策略进行结合得到的结果. 另外作者没有在reduced SVHN上使用Cutout，因为它会显著降低精度。表4是这些实验结果的总结,从表中我们可以看出，本文使用的两种模型均实现了state-of-the-art的精度。

![](https://pic4.zhimg.com/v2-386b55a0f88eae1cb56ddf066daf1ddf_b.png)表4

表4：测试集错误率（％）。 其中AA指的是AutoAugment。 Reduced SVHN数据集指的是：用于策略搜索的数据集上的结果。 在本文的实验中复现了reduced SVHN的Wide-ResNet基线结果，并大致匹配了其论文的精度（本文的结果高出0.38％，可能是由于超参数调整的不同）[65]。 SVHN数据集指的是：使用了基线增强策略[56]和Cutout [25]的Wide-ResNet结果(本文的实验中复现并匹配了其论文中的结果)。值得注意的是，AutoAugment策略是在reduced SVHN上搜索得到的。

本文还测试了在reduced SVHN上使用最佳策略的结果（与用于策略搜索的相同的1,000个样本）。 AutoAugment在reduced数据集上的结果再次与state-of-the-art的半监督方法相当(5.42％~3.86％)[62]。同样我们也可以看到AutoAugment在reduced数据集上了实现了比完整数据集更显着的提升。

ImageNet Results:

与之前实验类似，本文使用reduced ImageNet训练集(包含120个类（随机选择）和6,000个样本)进行策略搜索。作者使用余弦衰减对Wide-ResNet 40-2训练了200个epoch(10-5的weight decay和0.1的学习率)。通过训练ImageNet得到的最佳策略与CIFAR-10上的最佳策略相似，都侧重于对颜色的转换, 但在ImageNet的最优策略中几何变换rotate出现的次数要更多。 图2是通过ImageNet训练集得到的最好的策略之一:


![](https://pic1.zhimg.com/v2-01b4cef9a9cda2213ae5561388d81598_b.png)图2

图2：ImageNet上的最优策略之一。如文中所述，ImageNet上的大多数策略都使用了基于颜色的转换。

这里同样的将5个最佳策略(含25个子策略)结合，为ImageNet训练构建了最终的策略。作者在ResNet-50和ResNet-200上使用完整的ImageNet数据集并使用此策略从头开始训练了270个epoch, 其中batch-size为4096, 学习率为1.6, 并在90,180和240个epoch将学习率降低到原来的1/10。对于基线的数据增强，作者使用标准的Inception-style预处理方法，包括将像素值缩放到[-1,1]，50％ 概率水平翻转和颜色的随机扭曲[14,16]。作者使用基线预处理和ImageNet上学到的策略进行AutoAugment模型的训练, 同时作者也发现去除颜色的随机失真不会改变AutoAugment的结果。


![](https://pic1.zhimg.com/v2-efc356dd922bac453178eed0f0b49204_b.png)表5

表5：ImageNet验证集的Top-1 / Top-5错误率（％）。使用基线增强的ResNet-50结果来自[15]。 AmoebaNet-B，C结果与Inception-style在本文的实验中被复现，并与其论文的结果相匹配[21]。除此之外, 虽然存在有14.6％Top-1错误率[67]的结果，但他们的方法使用了大量弱标记的额外数据。

Fine Grained Visual Classification Datasets

为了评估在ImageNet上搜索到策略的可移植性，作者在五个图像大小与ImageNet相似的FGVC数据集上使用了原本在ImageNet上学习到的策略(表5). 这些数据集都很具有挑战性，原因是它们的训练数据相对较少但同时却具有大量类别。

![](https://pic4.zhimg.com/v2-42430f7885915c983fdaf9e274135c6b_b.png)表6

表6：FGVC数据集上的测试集Top-1错误率（％）。 AutoAugment的结果使用了在ImageNet上的策略, 并从头训练了Inception v4。

对于表6中列出的所有数据集(图像大小设置为448x448像素)，本文使用具有一个退火周期的余弦学习速率衰减对Inception v4[16]训练了1,000个epoch, 并根据验证集性能筛选学习率和权重衰减。作者将训练集和验证集合并, 使用所选的超参数再次进行了训练。实验表明, 应用ImageNet上获得的策略可以显著提高所有FGVC数据集的泛化精度。对于Stanford Cars数据集, 之前的state-of-the-art的结果微调了在ImageNet上pretrain的模型, 并使用深层聚合(deep layer aggregation)达到了5.9％的错误率[71], 而本文的实验在没有依赖任何预训练网络的情况下实现了该数据集上最低的错误率(5.19%).

## **五.讨论**

A comparison against Semi-Supervised Learning:

本文工作的一个重要方面是在训练期间随机应用子策略。在每个mini-batch中, 每个图像仅使用许多子策略中的一个来增强，而这个过程本身也具有更大的随机性(因为每个操作具有与其相关联的执行概率)。作者发现这种随机性使得每个子策略都要求一定数量的epoch来使AutoAugment算法变得有效。 由于每个child model都会训练相应的5个子策略，所以模型往往需要进行80-100个epoch的训练才能使所有子策略都可以有效的对结果发挥正面效果, 而这也是作者选择将child model训练120个epoch的原因。在策略被学习到之后，整个模型的训练时间更长（例如，CIFAR-10上的Shake-Shake的1800个epoch，ImageNet上的ResNet-50的270个epoch），这使得模型可以使用更多的子策略组合。

Importance of Diversity in AutoAugment Policies:

作者做过如下假设: 随着子策略的数量的增加，神经网络使用更具多样性的增强方法进行训练应该可以对模型泛化能力有更大程度的提升。为了验证这一假设，作者探究了CIFAR-10上训练的Wide-ResNet-28-10模型的平均验证集准确度，从而构建训练中使用的子策略数量的函数。作者从500个较优子策略池中随机选择子策略集，并使用每个子策略集对Wide-ResNet-28-10模型进行200个epoch的训练。 为了获得更好的统计结果, 作者在每个集合大小下都对子策略进行了五次不同的采样. 模型训练细节与之前在CIFAR-10上训练的Wide-ResNet-28-10相同。图3展示了平均验证集精度与训练中使用的子策略数量的函数关系，从实验结果我们可以确定验证集精度随着子策略数量的增加而不断增加(直到子策略数量达到20)

![](https://pic4.zhimg.com/v2-cecded910bceb113e79fa721e16a68cb_b.png)图3

图3：在CIFAR-10上训练的Wide-ResNet-28-10的验证集错误率（五次实验的平均结果），将其看作随机在500个郊优子策略池中选择子策略数量的函数。数线表示每个子策略数量下的验证错误范围。


Transferability across datasets and architectures:

值得注意的是，上述策略可以很好地移植到不同的模型结构和数据集上。例如，在Wide-ResNet-40-2和reduced CIFAR-10上学到的策略对在完整CIFAR-10和CIFAR-100上训练的所有其他模型都有一定的优化效果。同样, 在Wide-ResNet-40-2和reduced ImageNet上学习的策略也可以在具有不同数据分布和类分布的FGVC数据集对Inception v4进行改进。即使模型学习和应用在不同的数据集上进行，也从未发现AutoAugment产生过损害模型精度的情况。作者在附录中展示了ImageNet和SVHN数据集的最优策略，这有望帮助相关领域的研究人员去提高相应图像分类任务的泛化精度。

虽然本文的实验展示了AutoAugment算法的可移植性, 但是作者发现在最接近目标的数据分布上学习的策略具有最佳的性能：在SVHN训练时，使用reduced CIFAR-10上学习的最佳策略与基线数据增强相比确实略微提高了泛化精度，但跟使用SVHN本身数据集学习的策略相比还是比较有限。

## **六.附录**

![](https://pic1.zhimg.com/v2-43fca128fbbe205dac37a1896d45edb4_b.png)表7

表7：控制器在搜索期间可以选择的所有图像变换的列表。 第三列展示了控制器搜索每个操作的幅度值范围（图像尺寸331x331）, 其中某些变换不使用幅度信息（例如反转和均衡）。

表 8: 在reduced CIFAR-10上使用的AutoAugment策略

![](https://pic3.zhimg.com/v2-de3b52c3ce6f71455d2effdcb312df16_b.png)

表9：在reduced SVHN上使用的AutoAugment策略

![](https://pic4.zhimg.com/v2-1b0372cd99771de96e28635050fdbdbf_b.png)

表10：在reduced ImageNet上使用的AutoAugment策略

![](https://pic3.zhimg.com/v2-745b8487af06c9157737460bcc8988a2_b.png)

References:

[1] Henry S Baird. Document image defect models. In Structured Document Image Analysis, pages 546–556. Springer, 1992. 

[2] Patrice Y Simard, David Steinkraus, John C Platt, et al. Best practices for convolutional neural networks applied to visual document analysis. In Proceedings of International Conference on Document Analysis and Recognition, 2003. 

[3] Alex Krizhevsky, Ilya Sutskever, and Geoffrey E. Hinton. Imagenet classification with deep convolutional neural networks. In Advances in Neural Information Processing Systems, 2012. 

[4] Kunihiko Fukushima and Sei Miyake. Neocognitron: A self-organizing neural network model for a mechanism of visual pattern recognition. In Competition and cooperation in neural nets, pages 267–285. Springer, 1982. 

[5] Yann LeCun, Léon Bottou, Yoshua Bengio, and Patrick Haffner. Gradient-based learning applied to document recognition. Proceedings of the IEEE, 86(11):2278–2324, 1998. 

[6] Kevin Jarrett, Koray Kavukcuoglu, Yann LeCun, et al. What is the best multi-stage architecture for object recognition? In Proceedings of IEEE Conference on Computer Vision and Pattern Recognition, pages 2146–2153. IEEE, 2009. 

[7] Jörg Behler and Michele Parrinello. Generalized neural-network representation of high-dimensional potential-energy surfaces. Physical review letters, 98(14):146401, 2007. 

[8] Kristof Schütt, Pieter-Jan Kindermans, Huziel Enoc Sauceda Felix, Stefan Chmiela, Alexandre Tkatchenko, and Klaus-Robert Müller. Schnet: A continuous-filter convolutional neural network for modeling quantum interactions. In Advances in Neural Information Processing Systems, pages 992–1002, 2017. 

[9] Ekin D Cubuk, Brad D Malone, Berk Onat, Amos Waterland, and Efthimios Kaxiras. Representations in neural network based empirical potentials. The Journal of chemical physics, 147(2):024104, 2017. 

[10] Justin Gilmer, Samuel S Schoenholz, Patrick F Riley, Oriol Vinyals, and George E Dahl. Neural message passing for quantum chemistry. arXiv preprint arXiv:1704.01212, 2017. 

[11] Samuel S Schoenholz, Ekin D Cubuk, Efthimios Kaxiras, and Andrea J Liu. Relationship between local structure and relaxation in out-of-equilibrium glassy systems. Proceedings of the National Academy of Sciences, 114(2):263–267, 2017. 

[12] Ekin D Cubuk, Samuel S Schoenholz, Efthimios Kaxiras, and Andrea J Liu. Structural properties of defects in glassy liquids. The Journal of Physical Chemistry B, 120(26):6139–6146, 2016. 

[13] Karen Simonyan and Andrew Zisserman. Very deep convolutional networks for large-scale image recognition. Advances in Neural Information Processing Systems, 2015. 

[14] Christian Szegedy, Wei Liu, Yangqing Jia, Pierre Sermanet, Scott Reed, Dragomir Anguelov, Dumitru Erhan, Vincent Vanhoucke, Andrew Rabinovich, et al. Going deeper with convolutions. In Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition (CVPR), 2015. 

[15] Kaiming He, Xiangyu Zhang, Shaoqing Ren, and Jian Sun. Deep residual learning for image recognition. In Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition (CVPR), pages 770–778, 2016. 

[16] Christian Szegedy, Sergey Ioffe, Vincent Vanhoucke, and Alexander A Alemi. Inception-v4, inceptionresnet and the impact of residual connections on learning. In AAAI, 2017. 9 

[17] Saining Xie, Ross Girshick, Piotr Dollár, Zhuowen Tu, and Kaiming He. Aggregated residual transformations for deep neural networks. In Proceedings of IEEE Conference on Computer Vision and Pattern Recognition (CVPR), pages 5987–5995, 2017. 

[18] Dongyoon Han, Jiwhan Kim, and Junmo Kim. Deep pyramidal residual networks. In Proceedings of IEEE Conference on Computer Vision and Pattern Recognition (CVPR), pages 6307–6315. IEEE, 2017. 

[19] Barret Zoph, Vijay Vasudevan, Jonathon Shlens, and Quoc V Le. Learning transferable architectures for scalable image recognition. In Proceedings of IEEE Conference on Computer Vision and Pattern Recognition, 2017. 

[20] Jie Hu, Li Shen, and Gang Sun. Squeeze-and-excitation networks. arXiv preprint arXiv:1709.01507, 2017. 

[21] Esteban Real, Alok Aggarwal, Yanping Huang, and Quoc V Le. Regularized evolution for image classifier architecture search. arXiv preprint arXiv:1802.01548, 2018. 

[22] Ilya Sutskever, John Schulman, Tim Salimans, and Durk Kingma. Requests For Research 2.0. https: [//blog.openai.com/requests-for-research-2](https://link.zhihu.com/?target=https%3A//blog.openai.com/requests-for-research-2), 2018. 

[23] Barret Zoph and Quoc V Le. Neural architecture search with reinforcement learning. In International Conference on Learning Representations, 2017. 

[24] Horia Mania, Aurelia Guy, and Benjamin Recht. Simple random search provides a competitive approach to reinforcement learning. arXiv preprint arXiv:1803.07055, 2018. 

[25] Terrance DeVries and Graham W Taylor. Improved regularization of convolutional neural networks with cutout. arXiv preprint arXiv:1708.04552, 2017. 

[26] Simon Kornblith, Jonathon Shlens, and Quoc V Le. Do better imagenet models transfer better? arXiv preprint arXiv:1805.08974, 2018. 

[27] Jonathan Krause, Jia Deng, Michael Stark, and Li Fei-Fei. Collecting a large-scale dataset of fine-grained cars. In Second Workshop on Fine-Grained Visual Categorization, 2013. 

[28] Subhransu Maji, Esa Rahtu, Juho Kannala, Matthew Blaschko, and Andrea Vedaldi. Fine-grained visual classification of aircraft. arXiv preprint arXiv:1306.5151, 2013. 

[29] Dan Ciregan, Ueli Meier, and Jürgen Schmidhuber. Multi-column deep neural networks for image classification. In Proceedings of IEEE Conference on Computer Vision and Pattern Recognition, pages 3642–3649. IEEE, 2012. 

[30] Li Wan, Matthew Zeiler, Sixin Zhang, Yann Le Cun, and Rob Fergus. Regularization of neural networks using dropconnect. In International Conference on Machine Learning, pages 1058–1066, 2013. 

[31] Ikuro Sato, Hiroki Nishimura, and Kensuke Yokoi. Apac: Augmented pattern classification with neural networks. arXiv preprint arXiv:1505.03229, 2015. 

[32] Bowen Baker, Otkrist Gupta, Nikhil Naik, and Ramesh Raskar. Designing neural network architectures using reinforcement learning. In International Conference on Learning Representations, 2017. 

[33] Andrew Brock, Theodore Lim, James M Ritchie, and Nick Weston. Smash: one-shot model architecture search through hypernetworks. In International Conference on Learning Representations, 2017. 

[34] Hanxiao Liu, Karen Simonyan, Oriol Vinyals, Chrisantha Fernando, and Koray Kavukcuoglu. Hierarchical representations for efficient architecture search. In International Conference on Learning Representations, 2018. 

[35] Thomas Elsken, Jan-Hendrik Metzen, and Frank Hutter. Simple and efficient architecture search for convolutional neural networks. arXiv preprint arXiv:1711.04528, 2017. 

[36] Chenxi Liu, Barret Zoph, Jonathon S 

[39] Lingxi Xie and Alan Yuille. Genetic CNN. In Proceedings of IEEE Conference on Computer Vision and Pattern Recognition, 2017. 

[40] Ekin D Cubuk, Barret Zoph, Samuel S Schoenholz, and Quoc V Le. Intriguing properties of adversarial examples. arXiv preprint arXiv:1711.02846, 2017. 

[41] Joseph Lemley, Shabab Bazrafkan, and Peter Corcoran. Smart augmentation learning an optimal data augmentation strategy. IEEE Access, 5:5858–5869, 2017. 

[42] Toan Tran, Trung Pham, Gustavo Carneiro, Lyle Palmer, and Ian Reid. A bayesian data augmentation approach for learning deep models. In Advances in Neural Information Processing Systems, pages 2794–2803, 2017. 

[43] Terrance DeVries and Graham W Taylor. Dataset augmentation in feature space. arXiv preprint arXiv:1702.05538, 2017. 

[44] Luis Perez and Jason Wang. The effectiveness of data augmentation in image classification using deep learning. arXiv preprint arXiv:1712.04621, 2017. 

[45] Seongkyu Mun, Sangwook Park, David K Han, and Hanseok Ko. Generative adversarial network based acoustic scene training set augmentation and selection using svm hyper-plane. In Detection and Classification of Acoustic Scenes and Events Workshop, 2017. 

[46] Xinyue Zhu, Yifan Liu, Zengchang Qin, and Jiahong Li. Data augmentation in emotion classification using generative adversarial networks. arXiv preprint arXiv:1711.00648, 2017. 

[47] Antreas Antoniou, Amos Storkey, and Harrison Edwards. Data augmentation generative adversarial networks. arXiv preprint arXiv:1711.04340, 2017. 

[48] Leon Sixt, Benjamin Wild, and Tim Landgraf. Rendergan: Generating realistic labeled data. arXiv preprint arXiv:1611.01331, 2016. 

[49] Alexander J Ratner, Henry Ehrenberg, Zeshan Hussain, Jared Dunnmon, and Christopher Ré. Learning to compose domain-specific transformations for data augmentation. In Advances in Neural Information Processing Systems, pages 3239–3249, 2017. 

[50] Hiroshi Inoue. Data augmentation by pairing samples for images classification. arXiv preprint arXiv:1801.02929, 2018. 

[51] Irwan Bello, Barret Zoph, Vijay Vasudevan, and Quoc V Le. Neural optimizer search with reinforcement learning. In International Conference on Machine Learning, 2017. 

[52] John Schulman, Filip Wolski, Prafulla Dhariwal, Alec Radford, and Oleg Klimov. Proximal policy optimization algorithms. arXiv preprint arXiv:1707.06347, 2017. 

[53] Alex Krizhevsky and Geoffrey Hinton. Learning multiple layers of features from tiny images. Technical report, University of Toronto, 2009. 

[54] Yuval Netzer, Tao Wang, Adam Coates, Alessandro Bissacco, Bo Wu, and Andrew Y Ng. Reading digits in natural images with unsupervised feature learning. In NIPS Workshop on Deep Learning and Unsupervised Feature Learning, 2011. 

[55] Jia Deng, Wei Dong, Richard Socher, Li-Jia Li, Kai Li, and Li Fei-Fei. Imagenet: A large-scale hierarchical image database. In Proceedings of IEEE Conference on Computer Vision and Pattern Recognition (CVPR), 2009. 

[56] Sergey Zagoruyko and Nikos Komodakis. Wide residual networks. In British Machine Vision Conference, 2016. 

[57] Ilya Loshchilov and Frank Hutter. SGDR: Stochastic gradient descent with warm restarts. arXiv preprint arXiv:1608.03983, 2016. 

[58] Xavier Gastaldi. Shake-shake regularization. arXiv preprint arXiv:1705.07485, 2017. 

[59] Yoshihiro Yamada, Masakazu Iwamura, and Koichi Kise. Shakedrop regularization. arXiv preprint arXiv:1802.02375, 2018. 11 

[60] Martín Abadi, Paul Barham, Jianmin Chen, Zhifeng Chen, Andy Davis, Jeffrey Dean, Matthieu Devin, Sanjay Ghemawat, Geoffrey Irving, Michael Isard, Manjunath Kudlur, Josh Levenberg, Rajat Monga, Sherry Moore, Derek G. Murray, Benoit Steiner, Paul Tucker, Vijay Vasudevan, Pete Warden, Martin Wicke, Yuan Yu, and Xiaoqiang Zheng. Tensorflow: A system for large-scale machine learning. In Proceedings of the 12th USENIX Conference on Operating Systems Design and Implementation, OSDI’16, pages 265–283, Berkeley, CA, USA, 2016. USENIX Association. 

[61] Antti Tarvainen and Harri Valpola. Mean teachers are better role models: Weight-averaged consistency targets improve semi-supervised deep learning results. In Advances in neural information processing systems, pages 1195–1204, 2017. 

[62] Takeru Miyato, Shin-ichi Maeda, Masanori Koyama, and Shin Ishii. Virtual adversarial training: a regularization method for supervised and semi-supervised learning. In International Conference on Learning Representations, 2016. 

[63] Mehdi Sajjadi, Mehran Javanmardi, and Tolga Tasdizen. Regularization with stochastic transformations and perturbations for deep semi-supervised learning. In Advances in Neural Information Processing Systems, pages 1163–1171, 2016. 

[64] Samuli Laine and Timo Aila. Temporal ensembling for semi-supervised learning. arXiv preprint arXiv:1610.02242, 2016. 

[65] Avital Oliver, Augustus Odena, Colin Raffel, Ekin D Cubuk, and Ian J Goodfellow. Realistic evaluation of deep semi-supervised learning algorithms. arXiv preprint arXiv:1804.09170, 2018. 

[66] Andrew G Howard. Some improvements on deep convolutional neural network based image classification. arXiv preprint arXiv:1312.5402, 2013. 

[67] Dhruv Mahajan, Ross Girshick, Vignesh Ramanathan, Kaiming He, Manohar Paluri, Yixuan Li, Ashwin Bharambe, and Laurens van der Maaten. Exploring the limits of weakly supervised pretraining. arXiv preprint arXiv:1805.00932, 2018. 

[68] Maria-Elena Nilsback and Andrew Zisserman. Automated flower classification over a large number of classes. In Computer Vision, Graphics & Image Processing, 2008. ICVGIP’08. Sixth Indian Conference on, pages 722–729. IEEE, 2008. 

[69] Li Fei-Fei, Rob Fergus, and Pietro Perona. Learning generative visual models from few training examples: An incremental bayesian approach tested on 101 object categories. Computer vision and Image understanding, 106(1):59–70, 2007. 

[70] Yan Em, Feng Gag, Yihang Lou, Shiqi Wang, Tiejun Huang, and Ling-Yu Duan. Incorporating intra-class variance to fine-grained visual recognition. In Multimedia and Expo (ICME), 2017 IEEE International Conference on, pages 1452–1457. IEEE, 2017. 

[71] Fisher Yu, Dequan Wang, and Trevor Darrell. Deep layer aggregation. arXiv preprint arXiv:1707.06484, 2017. 

[72] Zhun Zhong, Liang Zheng, Guoliang Kang, Shaozi Li, and Yi Yang. Random erasing data augmentation. arXiv preprint arXiv:1708.04896, 2017. 

[73] Hongyi Zhang, Moustapha Cisse, Yann N Dauphin, and David Lopez-Paz. mixup: Beyond empirical risk minimization. arXiv preprint arXiv:1710.09412, 2017.


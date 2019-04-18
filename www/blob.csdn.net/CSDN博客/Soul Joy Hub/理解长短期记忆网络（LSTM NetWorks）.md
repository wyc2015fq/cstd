# 理解长短期记忆网络（LSTM NetWorks） - Soul Joy Hub - CSDN博客

2017年06月13日 23:27:47[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1035


**摘要：**作者早前提到了人们使用RNNs取得的显著成效，基本上这些都是使用了LSTMs。对于大多数任务，它们真的可以达到更好的效果！写了一堆方程式，LSTMs看起来很吓人。希望通过这篇文章中一步一步的剖析，能更好理解它们。              

**递归神经网络**

人类并不是每时每刻都从头开始思考。正如你阅读这篇文章的时候，你是在理解前面词语的基础上来理解每个词。你不会丢弃所有已知的信息而从头开始思考。你的思想具有持续性。

传统的神经网络不能做到这点，而且这似乎也是它的主要缺陷。比如，你想对电影中每个点发生的事件类型进行分类。目前还不清楚传统神经网络如何利用之前事件的推理来得出后来事件。

递归神经网络能够解决这一问题。这些网络中具有循环结构，能够使信息持续保存。

![](http://img.ptcms.csdn.net/article/201511/25/5655a0dc4811c.jpg)

递归神经网络具有循环结构

在上图中，一组神经网络A，接收参数，输出，循环A可以使信息从网络的某个步骤中传递到下一个步骤。

这些循环使得递归神经网络有些不可思议。不过，如果你进一步思考，可以发现，它们与常规神经网络并非完全不同。可以将递归神经网络想象成是有多层相同网络的神经网络，每一层将信息传递给下一层。如果我们展开循环，就是这样：

![](http://img.ptcms.csdn.net/article/201511/25/5655a1123a85d.jpg)

展开的递归神经网络

这种链状的性质表明，递归神经网络与序列和列表密切相关。这是处理这种数据所使用的神经网络的自然结构。

人们当然使用了它们！在过去的几年里，在许多问题上使用RNNs已经取得了难以置信的成功，比如语音识别，语言建模，翻译，图像字幕，这样的例子不胜枚举。不过我不将讨论RNNs的惊人壮举，Andrej Karpathy对此写了篇非常棒的博客，递归神经网络不可思议的有效性（[The Unreasonable Effectiveness of Recurrent Neural Networks](http://karpathy.github.io/2015/05/21/rnn-effectiveness/)）。它们真是太了不起了。

成功的关键是使用了“LSTMs”，一种特殊的递归神经网络，在许多任务中，它的表现要比标准递归神经网络出色许多。几乎所有基于递归神经网络令人振奋的结果都是由它们实现的。这篇文章就将探讨这些LSTMs。

**长期依赖关系问题**

RNNs呼吁的一点就是，它们可能将前期信息与当前任务连接，比如使用前面的视频帧可能得出对当前帧的理解。如果RNNs能够做到这点，它们会非常有用。但是它们能吗？这得看情况。

有些时候，在执行当前任务时，我们只需要查看最近的信息。比如，考虑一个语言模型，试图根据之前单词预测下一个。如果我们想要预测“the clouds are in the sky”中最后一个单词，我们不需要更多的上下文——很明显下一个单词会是“sky”。在这种情况下，如果相关信息与预测位置的间隔比较小，RNNs可以学会使用之前的信息。

![](http://img.ptcms.csdn.net/article/201511/25/5655a12a03a93.jpg)

但我们也有需要更多上下文的情况。考虑试图预测“I grew up in France… I speak fluent French.”中最后一个词。最近信息显示下一个词可能是一门语言的名字，但是如果我们想要缩小选择范围，我们需要包含“法国”的那段上下文，从前面的信息推断后面的单词。相关信息与预测位置的间隔很大是完全有可能的。

不幸的是，随着这种间隔的拉长，RNNs就会无法学习连接信息。

![](http://img.ptcms.csdn.net/article/201511/25/5655a14ec9f39.jpg)

从理论上讲，RNNs绝对能够处理这样的“长期依赖关系”。一个人可以仔细挑选参数来解决这种简单的问题。不幸的是，实际上RNNs不能够学习它们。这种问题被[Hochreiter （1991）【德国】](http://people.idsia.ch/~juergen/SeppHochreiter1991ThesisAdvisorSchmidhuber.pdf)和[Bengio等人](http://www-dsi.ing.unifi.it/~paolo/ps/tnn-94-gradient.pdf)深入探讨过，他们发现了造成这种可能的一些非常基本的原因。

值得庆幸的是，LSTMs没有这个问题！

**LSTM网络**

长短期记忆网络——通常简称“LSTMs”——是一种特殊的RNN，能够学习长期依赖关系。它们由[Hochreiter和Schmidhuber （1997）](http://deeplearning.cs.cmu.edu/pdfs/Hochreiter97_lstm.pdf)提出，在后期工作中又由许多人进行了调整和普及（除了原始作者之外，许多人为现代LSTM做出了贡献，不完全统计：Felix Gers， Fred Cummins， Santiago Fernandez， Justin Bayer， Daan Wierstra， Julian Togelius， Faustian Gomez， Matteo Gagliolo 和 Alex Graves）。它们在大量问题上效果异常出色，现在正在广泛使用。

LSTMs明确设计成能够避免长期依赖关系问题。记住信息很长一段时间几乎是它们固有的行为，而不是努力去学习！

所有的递归神经网络都具有一连串重复神经网络模块的形式。在标准的RNNs中，这种重复模块有一种非常简单的结构，比如单个tanh层。

![](http://img.ptcms.csdn.net/article/201511/25/5655a173cb1e7.jpg)

标准RNN中的重复模块包含单个层

LSTMs同样也有这种链状的结构，但是重复模块有着不同的结构。它有四层神经网络层以特殊的方式相互作用，而不是单个神经网络层。

![](http://img.ptcms.csdn.net/article/201511/25/5655a235588a8.jpg)

LSTM中的重复模块包含四个相互作用的神经网络层

先别急着想问细节。我们之后会一步一步讲解LSTM图。现在，我们先来熟悉下我们将要使用到的符号。

在上面的图中，每条线表示一个完整向量，从一个节点的输出到其他节点的输入。粉红色圆圈代表逐点操作，比如向量加法，而黄色框框表示的是已学习的神经网络层。线条合并表示串联，线条分叉表示内容复制并输入到不同地方。

**LSTMs核心理念**

LSTMs的关键点是单元状态，就是穿过图中的水平线。

单元状态有点像是个传送带。它贯穿整个链条，只有一些小的线性相互作用。这很容易让信息以不变的方式向下流动。

![](http://img.ptcms.csdn.net/article/201511/25/5655a1c09fba2_middle.jpg?_=38321)

LSTM有能力向单元状态中移除或添加信息，通过结构来仔细管理称为门限。

门限是有选择地让信息通过。它们由一个sigmoid神经网络层和逐点乘法运算组成。

![](http://img.ptcms.csdn.net/article/201511/25/5655a1e052cb4.jpg)

sigmoid层输出0到1之间的数字，描述了每个成分应该通过门限的程度。0表示“不让任何成分通过”，而1表示“让所有成分通过！”。

LSTM有三种这样的门限，来保护和控制单元状态。

**一步一步剖析LSTM**

LSTM中第一步是决定哪些信息需要从单元状态中抛弃。这项决策是由一个称为“遗忘门限层”的sigmoid层决定的。它接收和，然后为单元状态中的每个数字计算一个0到1之间的数字。1表示“完全保留”，而0则表示“完全抛弃”。

我们来回顾一下那个语言模型的例子，试图根据前面所有的词语来预测下一个词。在这种问题中，单元状态可能包含当前主语的性别，所以可以使用正确的代词。当碰到一个新的主语时，我们希望它能够忘记旧主语的性别。

![](http://img.ptcms.csdn.net/article/201511/25/5655a201d61f2.jpg)

接下来我们需要决定在单元状态中需要存储哪些新信息。这分为两个部分。首先，一个叫做“输入门限层”的sigmoid层决定哪些值需要更新。接下来，一个tanh层创建一个向量，包含新候选值，这些值可以添加到这个状态中。下一步我们将会结合这两者来创建一个状态更新。

在语言模型的例子中，我们希望在单元状态中添加新主语的性别，来替换我们忘记的旧主语性别。

![](http://img.ptcms.csdn.net/article/201511/25/5655a21e6a118.jpg)

现在来更新旧单元状态了，输入到新单元状态。之前的步骤已经决定了需要做哪些事情，我们只需要实现这些事情就行了。

我们在旧状态上乘以，忘记之前决定需要忘记的。然后我们加上，这就是新的候选值，它的规模取决于我们决定每个状态值需要更新多少。

在语言模型的例子中，这里就是我们实际丢弃旧主语性别信息，根据之前步骤添加新信息的地方。

![](http://img.ptcms.csdn.net/article/201511/25/5655a2399979f.jpg)

最后，我们需要决定需要输出什么。这个输出将会建立在单元状态的基础上，但是个过滤版本。首先，我们运行一个sigmoid层来决定单元状态中哪些部分需要输出。然后我们将单元状态输入到tanh函数（将值转换成-1到1之间）中，然后乘以输出的sigmoid门限值，所以我们只输出了我们想要输出的那部分。

对于语言模型例子来说，因为它只看到了一个主语，它可能想输出与动词相关的信息，为接下来出现的词做准备。比如，它可能输出主语是单数还是复数，那么我们知道接下来修饰动词的应该成对。

![](http://img.ptcms.csdn.net/article/201511/25/5655a25bb7fdf.jpg)

**长短期记忆变体**

我目前所讲述的还是非常常规的LSTM。但并不是所有的LSTMs都与上述的LSTM一样。实际上，几乎所有关于LSTMs的论文都稍有不同。虽然差异很小但也值得一谈。

一种流行的LSTM变种，由[Gers和Schmidhuber （2000）](ftp://ftp.idsia.ch/pub/juergen/TimeCount-IJCNN2000.pdf)提出，加入了“窥视孔连接”（peephole connections）。这意味着门限层也将单元状态作为输入。

![](http://img.ptcms.csdn.net/article/201511/25/5655a3263bef0.jpg)

上图中，所有的门限中都加入了窥视孔，但是许多论文都只使用部分窥视孔。

另一个变种就是使用耦合遗忘和输入门限。我们不单独决定遗忘哪些、添加哪些新信息，而是一起做出决定。在输入的时候才进行遗忘。在遗忘某些旧信息时才将新值添加到状态中。

![](http://img.ptcms.csdn.net/article/201511/25/5655a34610f8e.jpg)

稍微有戏剧性的LSTM变种是门限递归单元或GRU，由[Cho等人（2014）](http://arxiv.org/pdf/1406.1078v3.pdf)提出。它将遗忘和输入门限结合输入到单个“更新门限”中。同样还将单元状态和隐藏状态合并，并做出一些其他变化。所得模型比标准LSTM模型要简单，这种做法越来越流行。

![](http://img.ptcms.csdn.net/article/201511/25/5655a2c503375.jpg)

提出的深度门限RNNs（Depth Gated RNNs ）。当然还有其他解决长短期依赖关系问题的方法，比如[Koutnik等人（2014）](http://arxiv.org/pdf/1402.3511v1.pdf)提出的Clockwork RNNs 。

这些变种哪个是最好的？这些差异重要吗？[Greff等人（2015）](http://arxiv.org/pdf/1503.04069.pdf)针对流行的变种做了良好对比，发现它们其实都一样。[Jozefowicz等人（2015）](http://jmlr.org/proceedings/papers/v37/jozefowicz15.pdf)对超过1万种RNN架构做了测试，发现其中某些在特定任务上效果比LSTMs要好。

**结论**

早些时候，我提到了人们使用RNNs取得的显著成效。基本上这些都是使用了LSTMs。对于大多数任务，它们真的可以达到更好的效果！

写了一堆方程式，LSTMs看起来很吓人。希望通过这篇文章中一步一步的剖析，能更好理解它们。

LSTMs是我们使用RNNs的一个飞跃。那么很自然地会想：还有其他飞跃吗？研究人员之间共同的想法是：“有的！还有其他飞跃，还有关注点（attention）！”。这个想法是，让RNN中每一步都从更大的信息集中挑选信息作为输入。比如，你使用RNN来创建一个标题来描述图像，它可能会挑选图像的一部分作为输入，然后输出信息。事实上，[Xu等人（2015）](http://arxiv.org/pdf/1502.03044v2.pdf)这么做了——如果你想探索关注点这可能是个有趣的出发点！使用关注点还有一些其他令人兴奋的结果，而且似乎还有其他的效果还没被发现……

关注点并不是RNN研究中唯一令人振奋的地方。比如，[Kalchbrenner等人（2015）](http://arxiv.org/pdf/1507.01526v1.pdf)提出的Grid LSTMs似乎非常有前途。在生产模型——比如[Gregor等人（2015）](http://arxiv.org/pdf/1502.04623.pdf)，[Chung等人（2015）](http://arxiv.org/pdf/1506.02216v3.pdf)和[Bayer，Osendorfer（2015）](http://arxiv.org/pdf/1411.7610v3.pdf)中使用RNNs似乎同样有趣。过去的几年对递归神经网络来说是激动人心的时期，而且今后更会如此！

**致谢**

我要感谢帮助我理解LSTMs的一群人，他们对网络模型的结构图进行了评论，并对这篇文章进行了反馈。

我非常感谢我在谷歌的同事提供的反馈，特别是[Oriol Vinyals](http://research.google.com/pubs/OriolVinyals.html)，[Greg Corrado](http://research.google.com/pubs/GregCorrado.html)，[Jon Shlens](http://research.google.com/pubs/JonathonShlens.html)，[Luke Vilnis](http://people.cs.umass.edu/~luke/)，和[Ilya Sutskever](http://www.cs.toronto.edu/~ilya/)。同样感谢那些百忙之中给予我帮助的朋友和同事，[Dario Amodei](https://www.linkedin.com/pub/dario-amodei/4/493/393)，和[Jacob Steinhardt](http://cs.stanford.edu/~jsteinhardt/)。我特别要感谢[Kyunghyun Cho](http://www.kyunghyuncho.me/)，对我的图表给出了非常周到的对应关系。

在写这篇文章之前，我在我教的两个神经网络研讨会上练习过解释LSTMs。感谢参与其中每个人对我的耐心和给出的反馈。

**原文链接：**[Understanding LSTM Networks](http://colah.github.io/posts/2015-08-Understanding-LSTMs/)（译者/刘翔宇 审校/赵屹华 责编/周建丁 原创、翻译投稿请联系：zhoujd@csdn.net，微信号：jianding_zhou）

**译者介绍：**[刘翔宇](http://blog.csdn.net/lxytsos)，中通软开发工程师，关注机器学习、神经网络、模式识别。 

本文为CSDN编译整理，未经允许不得转载，如需转载请联系market#csdn.net(#换成@)



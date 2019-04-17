# 从CNN视角看在自然语言处理上的应用 - Paper weekly - CSDN博客





2017年10月23日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：490









作者丨卞书青

学校丨中国人民大学在读硕士

研究方向丨信息抽取、深度学习



本文经授权转载自知乎专栏「RUC智能情报站」。

**1. 前言**





卷积神经网络（Convolutional Neural Network）最早是应用在计算机视觉当中，而如今 CNN 也早已应用于自然语言处理（Natural Language Processing）的各种任务。



**本文主要以 CMU CS 11-747 (Neural Networks for NLP) [1] 课程中 Convolutional Networks for Text 这一章节的内容作为主线进行讲解。**本文主要包括了对如下几块内容的讲解，第一部分是对于常见的语言模型在进行文本表示时遇到的问题以及引入卷积神经网络的意义，第二部分是对于卷积神经网络模块的介绍，第三部分主要是介绍一些卷积神经网络应用于自然语言处理中的论文，第四部分主要是对这一篇综述进行总结。




**2. 引例**




我们首先来看这么一个问题，假设我们需要对句子做情感上的分类。




![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8XIkZtLvDicx2zAPjrbict6vibgtGRntdI4eLvCU5E5icYPrTbZDBEicuPlA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)






传统的词袋模型或者连续词袋模型都可以通过构建一个全连接的神经网络对句子进行情感标签的分类。但是这样存在一个问题，我们通过激活函数可以让某些结点激活（例如一个句子里“not”，”hate”这样的较强的特征词），但在这样网络构建里，句子中词语的顺序被忽略，也许同样两个句子都出现了
 not 和 hate，但是一个句子（I do not hate this movie）表示的是 good 的情感，另一个句子（I hate this movie and will not choose it）表示的是 bad 的情感。




其实很重要的一点是在刚才上述模型中我们无法捕获像 not hate 这样由连续两个词所构成的关键特征的词的含义。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8Ria2rueNwW3YRA5mBg3iclS6IHYuFPkAxiaNtxBH6scwOoKnqurF4mCRg/0?wx_fmt=jpeg)




在语言模型里 n-gram 模型是可以用来解决，想法其实就是将连续的两个词作为一个整体纳入到模型中，这样确实能够解决我们刚才提出的问题。加入 bi-gram，tri-gram 可以让我们捕捉到例如“don’t love”，“not the best”。




但是问题又来了，如果我们使用多元模型，实际训练时的参数是一个非常大的问题，因为假设你有 20000 个词，加入 bi-gram 实际上你就要有 400000000 个词，这样参数训练显然是爆炸的。另外一点，相似的词语在这样的模型中不能共享例如参数权重等，这样就会导致相似词无法获得交互信息。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8A8D2ViaY5s1vicUt2ozAFL0a4yA0W91LB0cibpj2gZt7oC08Lf4x08OFA/0?wx_fmt=jpeg)




**3. 卷积神经网络结构的认识**



利用卷积神经网络实际上是可以解决上述的两个问题。在讲卷积神经网络前，我们先来看两个简单的例子。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8b3P3FYN5A5puW7Qe7bsQric6t16ial4ZE80BmMY4SyCkLEoTFAMINbWw/0?wx_fmt=jpeg)




假设我去识别出左边这个方框里的猫，实际上在一张图片中猫所处的位置并不重要，它在左边，在右边，还是在底部，其实对于猫来说，它的特征是不变的。我需要在这一部分位置学习的特征也能用在另一部分位置上，所以对于这个图像上的所有位置，我们都能使用同样的学习特征。




而在右边的例子中，假设一句话中是谈论猫咪的，猫咪这个词的意义是否会随它在第一句话还是第二句话而发生改变呢，大部分情况是不变的，所以我们当我们使用一个文本网络时，网络能够学习到什么是猫咪并且可以重复使用，而不是每一次见到它就要重新学习。




接下来我们先来介绍卷积神经网络中各个重要的环节。




**3.1 卷积**




所以这里我们首先去理解卷积神经网络中卷积的运算。这里我们以图像作为输入。比较容易理解卷积的方法是把卷积想象成作用于矩阵的一个滑动窗口函数。如下面这张图的表示。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8cZpV1Fh9W0rYgMvX9MAk95e17OkwMibfwweQ1nl3mvyqyv0V3icBnE0w/0?wx_fmt=jpeg)




滑动窗口又称作卷积核、滤波器或是特征检测器。图中使用 3x3 的卷积核，将卷积核与矩阵对应的部分逐元素相乘，然后求和。对于卷积的运算可以看下面这幅图的解释。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8yqEcfIbtk1jQH1bIolPNnyAFCdKU8H0C2yhLDdKzOvUIq2bO1hOUOw/0?wx_fmt=jpeg)




在不改变卷积核权重的情况下，就像拿着一只刷子一样对整个图水平垂直滑动进行卷积运算，这样输出就是经过卷积运算后的输出层。这里有一个对卷积操作的动画演示
**[2]**，可以加深对其的理解。




**3.2 什么是卷积神经网络**




卷积神经网络其实就是多层卷积运算，然后对每层的卷积输出用非线性激活函数做转换（后面会讲到）。卷积过程中每块局部的输入区域与输出的一个神经元相连接。对每一层应用不同的卷积核，每一种卷积核其实可以理解为对图片的一种特征进行提取，然后将多种特征进行汇总。




以下面这幅图为例，原始的 input 为一幅图片，第一层卷积过后输出层变为 6@28*28，所以这里的卷积核实际上用了 6 个，6 个卷积核代表了对这一张原始图片的六种不同角度的特征提取（例如提取图片左上方的边缘线条，右下方的边缘线条等等）。




feature map 实际上的含义就是特征通道（或者理解为一个图片的不同特征），也可以说就是输出层的深度，这里就是 6，然后后面每一次做卷积操作是都是要对所有的特征通道进行卷积操作以便提取出更高级的特征。这里也涉及到池化层，在下一小节进行讲解。在训练阶段，卷积神经网络会基于你想完成的任务自动学习卷积核的权重值。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8mcwVmz81IGSOOgab18j3O4rKGLbeWyibPSk1bic98EE4ekuAzS599SDA/0?wx_fmt=jpeg)




例如，在上面这幅图中，第一层 CNN 模型也许学会从原始像素点中检测到一些边缘线条，然后根据边缘线条在第二层检测出一些简单的形状（例如横线条，左弯曲线条，竖线条等），然后基于这些形状检测出更高级的特征，比如一个A字母的上半部分等。最后一层则是利用这些组合的高级特征进行分类。 




**卷积神经网络中的卷积计算实际上体现了：位置不变性和组合性。**位置不变性是因为卷积核是在全图范围内平移，所以并不用关心猫究竟在图片的什么位置。组合性是指每个卷积核对一小块局部区域的低级特征组合形成更高级的特征表示。当然这两点对于句子的建模也是很多的帮助，我们会在后面的例子中提到。 



**3.3 卷积是如何应用到自然语言处理中 **




在图像中卷积核通常是对图像的一小块区域进行计算，而在文本中，一句话所构成的词向量作为输入。每一行代表一个词的词向量，所以在处理文本时，卷积核通常覆盖上下几行的词，所以此时卷积核的宽度与输入的宽度相同，通过这样的方式，我们就能够捕捉到多个连续词之间的特征，并且能够在同一类特征计算时中共享权重。下面这张图很好地诠释了刚才的讲解。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8fGz95LVOlynIaUicCmicKquku7a0UsckYca2sr2Ahdtp3sRicZu0B3TXA/0?wx_fmt=jpeg)




**3.4 池化层 **




卷积神经网络的一个重要概念就是池化层，一般是在卷积层之后。池化层对输入做降采样。池化的过程实际上是对卷积层分区域求最大值或者对每个卷积层求最大值。例如，下图就是 2x2 窗口的最大值池化（在自然语言处理中，我们通常对整个输出做池化，每个卷积层只有一个输出值）。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8S2cEXVmLe1bxWpniamSHZiafRziaKGyxmCJoE9vxwMW8yPxn2UhxAYApQ/0?wx_fmt=jpeg)




**为什么要进行池化操作？**




池化首先是可以输出一个固定大小的矩阵，这对于自然语言处理当中输入句子的长度不一有非常大的作用。例如，如果你用了 200 个卷积核，并对每个输出使用最大池化，那么无论卷积核的尺寸是多大，也无论输入数据的维度或者单词个数如何变化，你都将得到一个 200 维的输出。这让你可以应对不同长度的句子和不同大小的卷积核，但总是得到一个相同维度的输出结果，用作最后的分类。




另外池化层在降低数据维度的同时还能够保留显著的特征。每一种卷积核都是用来检测一种特定的特征。在以句子分类中，每一种卷积核可以用来检测某一种含义的词组，如果这种类型的含义的词语出现了，该卷积核的输出值就会非常大，通过池化过程就能够尽可能地将该信息保留下来。




关于池化层几种池化方式会在下面的内容里讲解。






**3.5 激活函数 **




有关激活函数很多细节的讲述在最后的总结会提到。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8ARls9napyhFCbrLEf1ZmFIcy8hmafxFiaaZwevDdicPcam8jF3zdFNLw/0?wx_fmt=jpeg)




**4. 卷积神经网络结构在自然语言处理的应用**




首先我们来介绍第一篇论文 ***Natural Language Processing (almost) from Scratch***，该论文主要是针对原来那种
 man-made 的输入特征和人工特征，利用神经网络的方法自动抽取出文本句子更高级的特征用来处理自然语言处理里的各项任务。




例如本文中输入是一个句子序列，输出是对句子中各个词的词性的预测。该文提出了两种方法，一种是滑动窗口的方法（window approach），另一种就是将整个句子作为输入（sentence approach）的方法，两种方法就分别对应着局部和全局的特征。模型结构如下图所示：




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8iajyMgfZ8F2QU95c0hKHJYCibGmOvTsfibnosANiaoKLcCHvxRKozR8Yuw/0?wx_fmt=jpeg)

window approach




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ80gChC0vicIlLFkGhYF7jJXP4gYtGmnfHNU5JpMXgS9gibza348qdheMA/0?wx_fmt=jpeg)

sentence approach




window approach 是根据某一个单词以及其附近固定长度范围内的单词对应的词向量来为单词预测标签。需要注意的是，当处理到一个句子的开始或者结尾的单词的时候，其前后窗口或许不包含单词，这时候我们需要填充技术，为前面或者后面填充象征开始或者结束的符号。 




实际上基于窗口的方法已经可以解决很多常见的任务，但是如果一个单词如果非常依赖上下文的单词，且当时这个单词并不在窗口中，这时就需要 sentence approach，这里所使用的卷积操作与卷积神经网络中的卷积操作基本相同。这里需要对句子中的每一个单词进行一次卷积操作，这里池化过程选择最大池化，这里认为句子中大部分的词语对该单词的意义不会有影响。 




刚才这篇论文实际上是在池化层中直接选择了最大池化，接下来的这篇论文 ***A Convolutional Neural Network for Modelling Sentences ***对句子级别特征的池化过程进行了改进并且提出了 DCNN 动态卷积网络（Dynamic Convolutional Neural Network），在介绍该论文前首先先来介绍一下常见的几种池化方式。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8cPEBkbHsjO1michDF64yFUUWYSkxJAnWnIwIHUN3ABagnzjJWMVoHiaA/0?wx_fmt=jpeg)




Max-pooling 最为常见，最大池化是取整个区域的最大值作为特征，在自然语言处理中常用于分类问题，希望观察到的特征是强特征，以便可以区分出是哪一个类别。Average-pooling 通常是用于主题模型，常常是一个句子不止一个主题标签，如果是使用 Max-pooling 的话信息过少，所以使用 Average 的话可以广泛反映这个区域的特征。




最后两个 K-max pooling 是选取一个区域的前 k 个大的特征。Dynamic pooling 是根据网络结构动态调整取特征的个数。最后两个的组合选取，就是该篇论文的亮点。 




该论文的亮点首先对句子语义建模，在底层通过组合邻近的词语信息，逐步向上传递，上层则又组合新的语义信息，从而使得句子中相离较远的词语也有交互行为（或者某种语义联系）。从直观上来看，这个模型能够通过词语的组合，再通过池化层提取出句子中重要的语义信息。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8t7iaM2fczZpiaWHg6CAYD5ia4RQuEXWaqibDTIDMuQdicwf42MIticafJkJw/0?wx_fmt=jpeg)




另一个亮点就是在池化过程中，该模型采用动态 k-Max 池化，这里池化的结果不是返回一个最大值，而是返回k组最大值，这些最大值是原输入的一个子序列。池化中的参数k可以是一个动态函数，具体的值依赖于输入或者网络的其他参数。该模型的网络结构如下图所示：




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8InFudqd2Fu5VfkWmt4Jt6saZPbHyLQaCFatg0IFFcngoDx2ibMswJdA/0?wx_fmt=jpeg)




这里重点介 绍k-max 池化和动态 k-max 池化。K-max 的好处在于，既提取除了句子中不止一个重要信息，同时保留了它们的顺序。同时，这里取 k 的个数是动态变化的，具体的动态函数如下。




![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8jdTTrQ5ZGona92KibIvazrlgibUmFYgLVYGwn6LEvOyibe8zfof0P5qFQ/0?wx_fmt=png)




这里需要注意的是 s 代表的是句子长度， L 代表总的卷积层的个数， l 代表的是当前是在几个卷积层，所以可以看出这里的 k 是随着句子的长度和网络深度而改变，我们的直观的感受也能看出初始的句子层提取较多的特征，而到后面提取的特征将会逐渐变少，同时由于 k_top 代表最顶层的卷积层需要提取的个数。 




这里的网络结构大多与通常的卷积网络层，但需要注意的是这里有一个 Folding 层（折叠操作层）。这里考虑相邻的两行之间的某种联系，将两行的词向量相加。 




该模型亮点很多，总结如下，首先它保留了句子中词序和词语之间的相对位置，同时考虑了句子中相隔较远的词语之间的语义信息，通过动态 k-max pooling 较好地保留句子中多个重要信息且根据句子长度动态变化特征抽取的个数。 




刚才这篇论文是对池化过程进行改进，接下来的两篇论文是对卷积层进行了改进。第三篇论文是 ***Neural Machine Translation in Linear Time***，该论文提出了扩张卷积神经网络（Dilated Convolution）应用于机器翻译领域。Dilated convolution 实际上要解决的问题是池化层的池化会损失很多信息（无论该信息是有用还是无用）。




Dilated convolution 的主要贡献就是，如何在去掉池化操作的同时，而不降低网络的感受野。下图理解起来更加容易，卷积的输入像素的间距由 1-2-4-8，虽然没有池化层，但是随着层数越深覆盖的原始输入信息依旧在增加。也就是我们通常卷积核与输入的一个区域的维度大小保持一致，但是去掉池化层后，我们随着深度增加，卷积核的所能覆盖的输入区域扩展一倍。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8px4VHw5yDjFgRPEaicuXHtpQTjylnlV4Z6EtWNdm4hrhic2c2L45YOkw/0?wx_fmt=jpeg)




在该模型中，句子建模时输入是以句子的字符级别开始的，之后随着卷积核所能覆盖的范围扩展，不断地去交互信息，同时还能够保证原始的输入信息不被丢失。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8EKoC5TDRjkZAicicFoyBEd35nDklpGNy55YteJdrusJN2Yg7B587ho7g/0?wx_fmt=jpeg)




之前的论文中主要是对卷积层和池化层从本身结构上进行改造，下面的这篇论文主要考虑到了本身句子已有依存句法树信息，将其融入到句子的建模中来。




论文 ***Dependency-based Convolutional Neural Networks for Sentence Embedding ***便是提出这一想法，模型的想法是，不仅仅是利用句子中相邻的词信息作为特征信息，一个依存句法树的实际上将句子的语义信息关系真正地提取出来，由于整个卷积的过程，句子的语序关系仍然会丢失，通过将依存句法树中**父子节点的语序信息**和**兄弟语序信息**一起作为输入，可以更加有效地抽取句子的特征。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8mr6BjYpJgMibBoRiaxsLYxiahQQWSSSkR9ju5ricjmD2ibgjJR0DdGqprFw/0?wx_fmt=jpeg)




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ86iahoib4HeTp1V5dzOdN4kyZSbuo0wUsCRbYPyQ5HEkyVA6Ty2VpO6iaQ/0?wx_fmt=jpeg)




最后要介绍的一篇论文是有关于句子匹配（Sentence Matching）的问题，基础问题仍然是句子建模。首先，文中提出了一种基于 CNN 的句子建模网络，卷积的作用是从句子中提取出局部的语义组合信息，而多个 Feature Map 则是从多种角度进行提取，也就是保证提取的语义组合的多样性。分别单独地对两个句子进行建模（使用上文中的句子模型），从而得到两个相同且固定长度的向量，然后，将这两个向量作为一个多层感知机（MLP）的输入，最后计算匹配的分数。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8CPGicmpmReWicPQMNUw1qrY4cZia8diaXJDVSaggEJShXmPoiaQ6r5XkibibA/0?wx_fmt=jpeg)




这个模型比较简单，但是有一个较大的缺点，两个句子在建模过程中是完全独立的，没有任何交互行为，一直到最后生成抽象的向量表示后才有交互行为，这样做使得句子在抽象建模的过程中会丧失很多语义细节，因此，推出了第二种模型结构。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8WgxL1Z3wPyKxkKm7JsBDYYUFibPcWb57dl6q4KVaSXw0MGzeoFFcYmw/0?wx_fmt=jpeg)




这种结构提前了两个句子间的交互行为,第一层中，首先取一个固定的卷积窗口 K1 ，然后遍历 Sx 和 Sy 中所有组合的二维矩阵进行卷积，每一个二维矩阵输出一个值，构成 Layer-2，然后进行 2×2 的 Max-pooling，后续的卷积层均是传统的二维卷积操作。




**5.总结/Q&A**




本篇综述中具体介绍了卷积神经网络的结构以及应用于自然语言处理中的场景，最后再做一个简单的归纳总结。




![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkAia6NcOG7yaNvcUEicRTcQ8s1T6UcNOYERCD6Kunls2UVhPmFwrsPjJOX7HIGlTHVABzUIqfdF6kA/0?wx_fmt=jpeg)




还有一些有关卷积神经网络细节上的问题与答案，与大家分享。 



**Q: 卷积层和池化层有什么区别？ **



**A: **首先可以从结构上可以看出，卷积之后输出层的维度减小，深度变深。但池化层深度不变。同时池化可以把很多数据用最大值或者平均值代替。目的是降低数据量。降低训练的参数。对于输入层，当其中像素在邻域发生微小位移时，池化层的输出是不变的，从而能提升鲁棒性。而卷积则是把数据通过一个卷积核变化成特征，便于后面的分离。 




**Q: 采用宽卷积的好处有什么？ **




**A: **通过将输入边角的值纳入到滑窗中心进行计算，以便损失更少的信息。 




**Q: 卷积输出的深度与哪个部件的个数相同？**




**A: **输出深度（通道）与卷积核（过滤器）的个数相等。



****Q: **激活函数通常放在卷积神经网络的那个操作之后？**




**A: **通常放在卷积层之后。 



****Q: **为什么激活函数通常都是采用非线性的函数？ **




**A: **如果网络中都采用线性函数的组合，那么线性的组合还是线性，那么使用多次线性组合就等同于使用了一次线性函数。因此采用非线性函数可以来逼近任意函数。 




**Q: 非线性激活函数中 sigmod 函数存在哪些不足？ **




**A: **Sigmod 函数存在饱和状态，尤其是值过大时，当进入饱和状态时，进行梯度下降计算时，很容易出现梯度消失的情况，求导的精确值不能保证。 




**Q: ReLU 和 SoftPlus 激活函数有哪些优势？ **




**A: **与 sigmod 相比，不存在指数计算，求导计算量变小，同时缓解了过拟合的情况，一部分输出为 0，减少了参数的相互依存。




**6. 参考资料**




****[1] **CMU CS 11-747 (Neural Networks for NLP) **

http://phontron.com/class/nn4nlp2017/schedule.html#

**[2] 卷积操作动画演示**

http://cs231n.github.io/convolutional-networks/

**[3] 宽/窄卷积动画演示**

http://deeplearning.net/software/theano_versions/dev/tutorial/conv_arithmetic.html

**[4] Udacity Deep Learning 课程**


https://cn.udacity.com/course/deep-learning--ud730

**[5] 深度学习入门教程/代码**

https://github.com/CreatCodeBuild/TensorFlow-and-DeepLearning-Tutorial




**7. 参考文献**




**• Neural Machine Translation in Linear Time阅读笔记**

https://zhuanlan.zhihu.com/p/23795111

**• 卷积神经网络(CNN)在句子建模上的应用**

http://www.jeyzhang.com/cnn-apply-on-modelling-sentence.html

**• 卷积神经网络在自然语言处理的应用**

http://www.csdn.net/article/2015-11-11/2826192




**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。






![0?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/0?wx_fmt=gif)

▽ 点击 | 阅读原文|
 访问作者知乎专栏





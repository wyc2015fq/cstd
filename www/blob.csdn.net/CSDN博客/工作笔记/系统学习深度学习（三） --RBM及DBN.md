# 系统学习深度学习（三） --RBM及DBN - 工作笔记 - CSDN博客





2017年01月10日 13:41:40[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：22458








转自：http://blog.csdn.net/yt7589/article/details/52487505

另外，有一篇文章《RBM原理理解》写的很详细，每个过程都有详细推导，也就通俗化的理解。我这里也有做根据文章对博文做了相关补充。

我们目前的讨论的神经网络，虽然学习[算法](http://lib.csdn.net/base/datastructure)不同，但基本上[架构](http://lib.csdn.net/base/architecture)还是相同的，就是都是分层网络，即神经元按层进行组织，层内神经元无连接，层间神经元间有连接。我们在这篇博文中，将讨论一种非常不同的神经网络，这类神经网络是由没层次关系的神经元全连接网络进化而来，采用有别于梯度下降算法进行学习的网络。

这类网络首先起源于Holpfield网络，这是一种全联接的网络，神经元之间进行全连接，我们可以给这个网络定义一个能量函数，神经网络的学习任务就是使能量函数达到最小值。这类网络典型的成功应用是担货郞问题，即有N个地点，每个地点间都有道路相通，担货郞必须把货物送到每个地点，通过Holpfield网络，可以有效地找到最佳路径。但是即使是对于二值（神经元只能处在0或1状态），全联接网络的状态也2的N次方个状态，要从这些状态中找到找到能量函数的最小值，难度相当大，大家一定还记得国际象棋发明者，向国王讨赏的典故吧，一个64个方格的棋盘，连全世界总粮食产量都填不满，可见这个问题的复杂性。

与此同时，根植于统计力学模型的波尔兹曼机（BM）也开始流行起来。在这种网络中，神经元的输出只有激活和未激活两种状态，用0或1来表示，各个神经元的输出值由概率纺计模型给出。典型的波尔兹曼机（BM）的网络模型如下图所示：

![](https://img-blog.csdn.net/20160909163832127?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

由实践来看，波尔兹曼机（BM）具有强大的非监督学习能力，可以发现数据中潜在规则，理论上来讲，非常适合于数据挖掘领域应用。便是由于是全连接网络，导致这种网络的训练时间非常长，没有高效的学习算法，直接制约了这种网络的应用。

后来Smolensky引入了限制性波尔兹曼机（RBM）模型，其主要思想就是去掉了波尔兹曼机中层内连接。限制性波尔兹曼机（RBM）具有一个可见层，一个隐藏层，层内神经元间无连接，层间神经元全连接，如下图所示：

![](https://img-blog.csdn.net/20160909164043571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

限制性波尔兹曼机（RBM）中，输入信号通过可见层输入到网络中，此时传播到隐藏层后，各隐藏层神经元的激活是互相独立的，同理在给定隐藏层信号后，反向传播到可见层时，可见层神经元的激活也具有独立性。可以从理论上证明，这种网络结构，只要隐藏层神经元节点足够多，限制性波尔兹曼机（RBM）可以拟合任意离散分布。虽然在理论上RBM很好，但是一直由于没有高效的学习算法，限制性波尔兹曼机（RBM）并没有得到广泛应用。但是[深度学习](http://lib.csdn.net/base/deeplearning)之父Hinton在2002年提出了对比散度（CD）算法，使限制性波尔兹曼机（RBM）具备了快速学习的能力。从此，RBM得到了广泛的应用，出现了各种对比散度算法的变种，使得算法收敛性更高。与此同时，波尔兹曼机（RBM）在分类、回归、降噪、高维时间序列分析、图像特征提取、协同过滤等方面，得到了广泛的应用，在年初Science子刊上发表了利用限制性波尔兹曼机（RBM）分析非结构化病例信息，从其中进行医学诊断知识，并成功应用于癌症早期筛查，表明RBM在非结构化数据处理方面，也有实用价值。另外，Hinton在2006年提出，将限制性波尔兹曼机（RBM）堆叠起来，形成深度信念网络（DBN），通过逐层训练RBM网络，将训练好的RBM网络堆叠成深度学习网络，可以得到非常好的初始参数值，有效地解决了大型神经网络训练速度慢的问题，是当前的研究热点之一。

在介绍了这么多限制性波尔兹曼机（RBM）的基本情况之后，下面我们来具体介绍一下RBM的理论基础。

如上图所示，RBM的可见层用v表示，用于接收输入信号，隐藏层由h表示，可以视为是输入信号的特征提取器。我们在前面讨论过，制约神经网络大规模应用的一个瓶颈之一，就是很难为研究问题找到合适的特征，而RBM则是通过无监督学习方式，自动找到研究问题的最佳特征，因此对于研究者们而言，具有非常大的吸引力，这也是为什么RBM在近些年来如此火的原因。我们设定可见层神经元为二值变量，即，隐藏层单元同样为二值变量，即，假定可见层有m个神经元，用下标i代表第j个神经元，隐藏层有n个神经元，用下标j表示第j个神经元。

我们可以定义网络的能量函数为：

![](https://img-blog.csdn.net/20160918190204856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式1

上式中为网络参数，均为实数，Wij为可见层神经元i到隐藏层神经元j的连接权值，bj为可见层第j个神经元的偏置，ci为隐藏层第i个神经元的偏置。

补充：

这里说一下 RBM 的能量模型，这里关系到 RBM 的理解 能量模型是个什么样的东西呢？直观上的理解就是，把一个表面粗糙又不太圆的小球， 放到一个表面也比较粗糙的碗里，就随便往里面一扔，看看小球停在碗的哪个地方。一般来 说停在碗底的可能性比较大，停在靠近碗底的其他地方也可能，甚至运气好还会停在碗口附 近（这个碗是比较浅的一个碗）；能量模型把小球停在哪个地方定义为一种状态，每种状态都对应着一个能量E，这个能量由能量函数来定义，小球处在某种状态的概率（如停在碗底的概率跟停在碗口的概率当然不一样）可以通过这种状态下小球具有的能量来定义（换个说 法，如小球停在了碗口附近，这是一种状态，这个状态对应着一个能量E ，而发生“小球停 在碗口附近”这种状态的概率 p ，可以用E 来表示，表示成p=f(E)，其中 f 是能量函数， 其实还有一个简单的理解，球在碗底的能量一般小于在碗边缘的，比如重力势能这，显然碗 底的状态稳定些，并且概率大些。 也就是说，RBM采用能量模型来表示系统稳态的一种测度。这里可以注意到 RBM 是一种随机网络，描述一个随机网络，主要有以下 2 点 ：

1. 概率分布函数。各个节点的取值状态是概率的、随机的，这里用了 3 种概率分布来描述 整个 RBM 网络，有联合概率密度，条件概率密度和边缘概率密度。

2. 能量函数。随机神经网络的基础是统计力学，差不多思想是热力学来的，能量函数是描 述整个系统状态的一种测度。系统越有序或者概率分布越集中（比如小球在碗底的情况）， 系统的能量越小，反之，系统越无序并且概率分布发散（比如平均分布） ，则系统的能量 越大，能量函数的最小值，对应着整个系统最稳定的状态。 这里跟之前提到的最大熵模型思路是一样的。


RBM 能量模型的作用是什么呢？为什么要弄清楚能量模型的作用呢？ 第一、 RBM 网络是一种无监督学习的方法，无监督学习的目的自然就是最大限度的拟 合输入数据和输出数据。 第二、 对于一组输入数据来说，如果不知道它的分布，那是非常难对这个数据进行学 习的。例如：如果我们实现写出了高斯函数，就可以写出似然函数，那么就可 以进行求解，就知道大致的参数，所以实现如果不知道分布是非常痛苦的一件 事情，但是，没关系啊，统计力学的一项研究成果表明，任何概率分布都可以 转变成基于能量的模型，即使这个概率分布是未知的，我们仍然可以将这个分 布改写成能量函数。 第三、 能量函数能够为无监督学习方法提供 2 个特殊的东西 a）目标函数。 b）目标解， 换句话说，使用能量模型使得学习一个数据的变得容易可行了。 能否把最优解的求解嵌入能量模型中至关重要，决定着我们具体问题求解的好坏。能量模型要捕获变量之间的

相关性，变量之间的相关程度决定了能量的高低。把变量的相关关系用图表示 出来，并引入概率测度方式就构成了概率图（为什么是概率图？前面一句说了， RBM 是一个图，以概率为测度，所以是概率图）模型的能量模型。


由上面所说，RBM是一种概率图模型，既然引入了概率，那么就可以通过采样技术来求解，在 CD（contrastive divergence）算法中采样部分扮演着模拟求解梯度的角色。

接上文：



接下来我们定义关于可见层和隐藏层的联合概率分布：

![](https://img-blog.csdn.net/20160918190620986?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式2

Z为归一化因子，其定义为：

![](https://img-blog.csdn.net/20160918190809581?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式3

补充：

为什么要搞能量函数？前面指出未知分布不好求解但是可以通过能量函数来表示，那么 能量函数的概率模型很大程度上可以得到未知分布的概率模型，这样大致就知道了未知分布 的分布 既然知道了一个 RBM 网络 hidden 和 visible 整个框架的能量函数，那么可以定义这个能 量函数（能量）出现的概率，很显然这个能量的出现与 hidden 和 visible 的每个节点的取值都有关系，那么这个能量出现的概率就是v和h 的联合概率密。也是一个特殊的Gibbs分布。

接上文：

熟悉[机器学习](http://lib.csdn.net/base/machinelearning)算法的都知道，我们需要知道的实际上是输入样本的概率分布，也就是式2的P的边际分布，也称为似然函数，定义如下所示：

![](https://img-blog.csdn.net/20160911143253111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式4

补充：

在知道了联合概率密度后，边缘概率，条件概率都可以计算出来。

因为需要计算归一化因子，而这需要![](https://img-blog.csdn.net/20160911144111880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)次运算，对于高维问题，即使我们可以通过算法得出网络的参数，但是由于运算量过大，这个公式在实际应用过程中，也不能直接应用。

但是，由于限制性波尔兹曼机（RBM）具有层间全连接，层内无连接，当我们将输入信号输入到可见层时，可见层将决定隐藏层各神经元的状态，而且由层内无连接，此时隐藏层神经元的激活状态是条件独立的，隐藏层第j个神经元激活状态的概率为：

![](https://img-blog.csdn.net/20160918191448880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式5

与此同理，如果隐藏层状态完全确定时，可见层第i个神经元激活状态也是条件独立的，其公式为：

![](https://img-blog.csdn.net/20160918192751721?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式6

对于RBM学习算法，我们会用到对数似然函数，其定义如下所示：

![](https://img-blog.csdn.net/20160919193337340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式7

补充：

上面的内容已经得到了 Gibbs 分布的各种概率密度函数，现在回到最初的目的，即求解目标------让 RBM 网络表示的 Gibbs 分布最大可能的拟合输入数据，或者换一种说法，求解 的目标可以认为是让 RBM 网络表示的 Gibbs 分布与输入样本的分布尽可能的接近。 现在的小问题是“最大可能的拟合输入数据”这句话怎么定义：

假设S表示样本空间，即里面含有很多个不同的 X ，q 是输入样本的分布，q（X）表 示训练样本的概率，再假设 p 是 RBM 网络表示的 Gibbs 分布的v 的边缘分布，即p(v)，可以理解成每种不同情况的 X 都对应着一个概率。那么样本真实的分布和 RBM 网络表示的边缘分布的 KL 距离就是 2 者之间的差异性 ，样本的真实分布与 RBM 网络表示 的边缘分布的 KL 距离如下所示 :

![](https://img-blog.csdn.net/20170111105212828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) 

KL 的全称是 Kullback-Leibler 差异，也叫做 Relative Entropy，它衡量的是相同事件空间里 的两个概率分布的差异情况，它的物理意义是：在相同事件空间里，概率分布 Px 的事 件空间，若用概率分布  Qx 进行编码，平均每个事件（符号）的编码长度增加了多少比 特。我们用 D(P||Q)表示 KL 距离，计算公式如下：![](https://img-blog.csdn.net/20170111105423314?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



如果输入样本表示的分布与 RBM 表示的 Gibbs 分布完全符合，这个 KL 距离就是 0，否 则是一个大于 0 的数 由附录对熵的定义可知，上面KL计算的第一项是输入样本的熵，

这个是一个固定的数，输入样本固定了，熵就固定了，第二项明显无法直接求。由 KL 的性 质可知，KL 是一定大于 0 的，那么当第二项最大的时候，整个 KL 最小，我们本来的目的也是求 KL 最小。注意到第二项中的qx当样本固定的时候，也是固定的，而 ln 函数是递增的，即当lnp(x) 最大即可。在实际应用中，我们采用的是 所有样本的极大似然估计。
结论就是求解输入样本的极大似然，就能让 RBM 网络表示的 Gibbs 分布 和样本本身表 示的分布最接近，也就是p(v)的极大似然估计。也就是上面的式7.其中已经将相关概率带入极大似然估计计算中。

接上文：

在式7的推导中，我们代入了式4的内容，同时把对数内除法转化为了对数的减法。

我们需要的是对数似然函数对于参数的导数（梯度下降），如下所示：

![](https://img-blog.csdn.net/20160921111453306?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式8

由于式8比较复杂，我们需要分别对第一项和第二项进行求导，考虑到大家对数学公式的熟悉程度，我们将一步一步进行推导，中间用到的数学公式，我们会在文中列出。

对于式8的第一项来说，我们首先需要应用链式求导法则，如下所示：

![](https://img-blog.csdn.net/20160921112506853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式9

我们在高数中，常用函数的求导公式可知：

![](https://img-blog.csdn.net/20160921113009404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式10

指数函数求导公式：

![](https://img-blog.csdn.net/20160921113241684?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式11

将式10和式11代入式9，可以得到如下结果：

![](https://img-blog.csdn.net/20160921113448737?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式12

在这时我们要注意，我们不能将第一项和第二项消去，而是应该将第一项视为一个常数，将其代入第二项的叠加符号内。

根据隐藏层在可见层条件下的概率密度函数定义，我们可以得到如下结果：

![](https://img-blog.csdn.net/20160921113914446?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式13

在这个推导中会用到概率论中的贝叶斯公式，即条件概率定义：

![](https://img-blog.csdn.net/20160921114452046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式14

将式13代入式12，可以得到如下结果：

![](https://img-blog.csdn.net/20160921114636206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式15

到此为止，我们成功求出了式8的第一项，下面我们还是应用链式求导法则，来化简式8的第二项，结果如下所示：

![](https://img-blog.csdn.net/20160921114944052?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式16

推导方法与第一项类似，只不过在推导过程中，用到了式3和式4的定义。因此对数似然函数对参数的求导公式最后可以化简为如下形式：

![](https://img-blog.csdn.net/20160921115345022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式17

对于限制性波尔兹曼机（RBM）而言，参数是权值矩阵W、可见层偏置c，隐藏层偏置b，下面我们根据式17分别求对于他们的导数，我们首先来求对权值的导数，如下所示：

![](https://img-blog.csdn.net/20160921124811968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式18

由于式18比较复杂，下面我们分别对第一项和第二项进行求解，对于第一项，我们根据能量函数E的定义（式1），其对wij求导的结果为hivj，结果如下所示：

![](https://img-blog.csdn.net/20160921132538407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式19

接着我们来求式18的第二项：

![](https://img-blog.csdn.net/20160921134624368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式20

因此式18的最终结果为：

![](https://img-blog.csdn.net/20160921134858212?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式21

如果我们采取在线学习模式，即每个训练样本均更新神经网络的参数，式21即可作为权值调整值来使用，但是由于样本的随机性，这样的参数调整效率会比较低，因此实际中使用更多的是批量学习模式。假设设置批量学习样本为：

![](https://img-blog.csdn.net/20160921135456758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式22

则式21的导数可以表示为该批次内对所有训练样本求导的平均值，如下所示：

![](https://img-blog.csdn.net/20160921141138264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式22

式22中，q代表经验分布，通常我们有如下结果：

![](https://img-blog.csdn.net/20160921141703756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式23

与求导对数似然函数对权值的导数方法类似，我们同样可以求出对可见层偏移量bj的导数，最后结果为：

![](https://img-blog.csdn.net/20160921142546619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式24

同理对数似然函数对隐藏层偏移量ci求导数的结果为：

![](https://img-blog.csdn.net/20160921142659916?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)式25

式21、式24、式25中的第二项（即都含有对所有v的遍历），可以通过Gibbs采样得到，但是这需要运行受限波尔兹曼机（RBM）很长时间，即可见层到隐藏层，再从隐藏层到可见层，反复运行，最后达到静止点，这就是马可夫链蒙特卡罗方法。但是这种方法运算量非常大，在2006年以前，受限于此，RBM很少应用在实际问题中。在2006年，深度学习之父Hinton提出了CD-k算法，而且通常k=1，可以很好的解决定一问题，才使RBM具有了应用价值，并且直接促使深度信念网络（DBN）的广泛使用。

补充：

1.蒙特卡洛采样：

最早的蒙特卡罗方法，是由物理学家发明的，旨在通过随机化的方法来计算积分。

假定给定一个函数h（x），我们想要计算区间[a, b]上面的积分，但是我们并 不知道h(x)在[a, b]上面的全部取值，这时，我们可以将h(x)分解为某个函数 fx和一 个定义在[a, b]上的概率密度函数的乘积。这样整个积分就可以写成：

![](https://img-blog.csdn.net/20170111131929879?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这样一来，原积分就等同于fx在px这个分布上面的期望。这时，我们若从分布 px上采集大量的样本Xn，这些样本的分布符合px，即 
![](https://img-blog.csdn.net/20170111132042318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) 

那么我们可以通过这个样本来逼近这个均值：

![](https://img-blog.csdn.net/20170111132113382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)  

因此，上面的遍历部分的Gibbs采样，具体的处理过程是对于每个训练样本 X ，都用某种抽样方法抽取一个对应的Y ，这个 Y 是符合 RBM 网络所表示的 Gibbs 分布的。那么对于整个训练集X来说，就 得到一组对应的符合 RBM 网络表示的 Gibbs 分布的样本集Y，然后拿这个样本去估算第二项，从而计算出梯度。如下公式(这个就是W，C,B的更行规则，还有下面条件概率计算)：

![](https://img-blog.csdn.net/20170111132409605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170111132419179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上面的式子中 Xk表示第k 个训练样本，Yk是Xk 所对应的符合 RBM 网络表示的 Gibbs分布的样本，在式子中用 yv表示。梯度求出来了，就可以求解了，最后不断迭代就可以得

到 w,c,b.

另外：

![](https://img-blog.csdn.net/20170111132652843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170111132728015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

推到过程略，有兴趣的可以参考《RBM原理理解》。

![](https://img-blog.csdn.net/20170111132702436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

需要注意的是，上面只是条件概率计算，采样还要根据概率值采样成伯努利分布方式。因为RBM是二值分布图。

2.对比散度算法

Gibbs 抽样是一种基于 Markov Chain Monte Carlo 策略的抽样方法。具体来说对于一个d 维的随机向量X=（x1,x2,...,xd），我们无法求的 X 的概率分布P（x），但我们知道给

定的 X 的其他分量是其第i个分量xi 的条件分布即 p(xi|xi-),xi-=(x1,x2,…xi-1,xi+1…xd)。那么，我们可以从 x 的一个任意状态（如(x1(0),x2(0),…,xd(0))）开始，利用条件分布 p(xi|xi-)， 迭代地对这状态的每个分量进行抽样，随着抽样次数 n 的增加，随机变量(x1(n),x2(n),…,xd(n)) 的概率分布将以 n 的几何级数的速度收敛与 x 的联合概率分布 p(v)。 Gibbs 抽样其实就是可以让我们可以在未知联合概率分布 p(v)的情况下对其进行抽样。 基于 RBM 模型的对称结构，以及其中节点的条件独立性，我们可以使用 Gibbs 抽样方 法得到服从 RBM 定义的分布的随机样本。在 RBM 中进行 k 步 Gibbs 抽样的具体算法为：

用 一个训练样本（或者可视节点的一个随机初始状态）初始化可视节点的状态 v0，交替进行下 面的抽样：

![](https://img-blog.csdn.net/20170111133320980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在抽样步数 n 足够大的情况下，就可以得到 RBM 所定义的分布的样本（即符合参数确 定的 Gibbs 分布的样本）了，得到这些样本我们就可以拿去计算梯度的第二项了。 可以看到，上面进行了 k 步的抽样，这个 k 一般还要比较大，所以是比较费时间的，尤其是在训练样本的特征数比较多（可视节点数大）的时候，所以 hinton 教授就弄一个简化的 版本，叫做 CD-k，也就对比散度。 对比散度是英文 ContrastiveDivergence（CD）的中文翻译。与 Gibbs 抽样不同，hinton 教 授指出当使用训练样本初始化 v0 的时候，仅需要较少的抽样步数（就是 k 的值，一般就一 步）就可以得到足够好的近似了。

在 CD 算法一开始，可见单元的状态就被设置为一个训练样本，并用上面的几个条件概 率来对隐藏节点的每个单元都从{0,1}中抽取到相应的值，然后再利用隐藏节点的值来对可视节点的每个 单元都从{0,1}中抽取相应的值，这样就得到了 v1 了，一般 v1 就够了，就可以拿来估算梯度 了。

下面给出 RBM 的基于 CD-k 的快速学习的主要步骤。

![](https://img-blog.csdn.net/20170111133458887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中，之所以第二项没有了那个 1/l，就是因为这个梯度会对所有样本进行累加（极大 似然是所有训练样本的梯度的和），最终加和的结果跟现在这样算是相等的（刚好是 l 个样 本，l 个第二项相加后，最终结果刚好等于每一个第二项的那个累加符号后面的那一项，而 这里所有结果加和后，也能得到相同的值）。 


首先 RBM 假设有一个 hidden 和一个 visible，这与前面的假设是完全一致的，给定一个 visible，我们想算出对应的 hidden 层的概率（注意这里说的 hidden 层是一个整体，包含每 个 hidden 节点），其实对应的 hidden 层的概率已经算出来了，就是前面的边缘概率密度， 但是只有概率密度是不够的，需要的是样本，这个 gibbs sampling 就是获取样本的，我们知 道p(h=1|v)，这样就可以算出 hidden 层各个节点的取值。 打个比方，有个 Gibbs Sampling 的算法，比如一个样本是 3 维，姑且认为是（g,u,b），一般来 说我们无法得到样本的分布，但是我们一般知道条件概率p（g|u,b）,p(u|g,b),p(b|g,u)：

 Step1: 给 g ，u ，b  赋初始值：  g0,u0,b0，这个初始值还不知道是不是随机赋的，还是有什么规则，现在将第一步产生的当作初始样本。 

Step2: 利用 p (g | u0, b0) 产生 g1 

Step3: 利用 p (u | g1, b0) 产生 u1 

Step4: 利用 p (b | g1, u1) 产生 b1

Step5: 重复 step2~step5 这样我们就可以得到一个 markov chain {a1,a2 … ai, ai+1,… at} 一般最后面的就是我们需要的样本，比如说取最后面的 15 个之类的。








转自：http://blog.csdn.net/zouxy09/article/details/8781396

**9.3、Restricted Boltzmann Machine (RBM)限制波尔兹曼机**

       假设有一个二值图（节点服从伯努利分布的无向图），每一层的节点之间没有链接，一层是可视层，即输入数据层（v)，一层是隐藏层(h)，如果假设所有的节点都是随机二值变量节点（只能取0或者1值），同时假设全概率分布p(v,h)满足Boltzmann 分布，我们称这个模型是Restricted BoltzmannMachine (RBM)。

![](https://img-my.csdn.net/uploads/201304/10/1365561323_7932.jpg)

       下面我们来看看为什么它是Deep Learning方法。首先，这个模型因为是二部图，所以在已知v的情况下，所有的隐藏节点之间是条件独立的（因为节点之间不存在连接），即p(h|v)=p(h1|v)…p(hn|v)。同理，在已知隐藏层h的情况下，所有的可视节点都是条件独立的。同时又由于所有的v和h满足Boltzmann 分布，因此，当输入v的时候，通过p(h|v) 可以得到隐藏层h，而得到隐藏层h之后，通过p(v|h)又能得到可视层，通过调整参数，我们就是要使得从隐藏层得到的可视层v1与原来的可视层v如果一样，那么得到的隐藏层就是可视层另外一种表达，因此隐藏层可以作为可视层输入数据的特征，所以它就是一种Deep Learning方法。

![](https://img-my.csdn.net/uploads/201304/10/1365561362_2427.jpg)

       如何训练呢？也就是可视层节点和隐节点间的权值怎么确定呢？我们需要做一些数学分析。也就是模型了。

![](https://img-my.csdn.net/uploads/201304/10/1365561384_7276.jpg)

        联合组态（jointconfiguration）的能量可以表示为：

![](https://img-my.csdn.net/uploads/201304/10/1365561400_3303.jpg)

        而某个组态的联合概率分布可以通过Boltzmann 分布（和这个组态的能量）来确定：

![](https://img-my.csdn.net/uploads/201304/10/1365561427_1491.jpg)

      因为隐藏节点之间是条件独立的（因为节点之间不存在连接），即：

![](https://img-my.csdn.net/uploads/201304/10/1365561464_9047.jpg)

      然后我们可以比较容易（对上式进行因子分解Factorizes）得到在给定可视层v的基础上，隐层第j个节点为1或者为0的概率：

![](https://img-my.csdn.net/uploads/201304/10/1365561492_4178.jpg)

       同理，在给定隐层h的基础上，可视层第i个节点为1或者为0的概率也可以容易得到：

![](https://img-my.csdn.net/uploads/201304/10/1365561523_2700.jpg)

       给定一个满足独立同分布的样本集：D={**v**(1),** v**(2),…,** v**(N)}，我们需要学习参数θ={W,a,b}。

       我们最大化以下对数似然函数（最大似然估计：对于某个概率模型，我们需要选择一个参数，让我们当前的观测样本的概率最大）：

![](https://img-my.csdn.net/uploads/201304/10/1365561551_7458.jpg)

        也就是对最大对数似然函数求导，就可以得到L最大时对应的参数W了。

![](https://img-my.csdn.net/uploads/201304/10/1365561570_5666.jpg)

** 上面是算是对整个RBM推导的核心过程的简述，可以对着开头的内容阅读。**

       如果，我们把隐藏层的层数增加，我们可以得到Deep Boltzmann Machine(DBM)；如果我们在靠近可视层的部分使用贝叶斯信念网络（即有向图模型，当然这里依然限制层中节点之间没有链接），而在最远离可视层的部分使用Restricted Boltzmann Machine，我们可以得到DeepBelief Net（DBN）。

![](https://img-my.csdn.net/uploads/201304/10/1365561611_3496.jpg)



**9.4、Deep Belief Networks深信度网络**

        DBNs是一个概率生成模型，与传统的判别模型的神经网络相对，生成模型是建立一个观察数据和标签之间的联合分布，对P(Observation|Label)和 P(Label|Observation)都做了评估，而判别模型仅仅而已评估了后者，也就是P(Label|Observation)。对于在深度神经网络应用传统的BP[算法](http://lib.csdn.net/base/datastructure)的时候，DBNs遇到了以下问题：

（1）需要为训练提供一个有标签的样本集；

（2）学习过程较慢；

（3）不适当的参数选择会导致学习收敛于局部最优解。

![](https://img-my.csdn.net/uploads/201304/10/1365561636_9432.jpg)

       DBNs由多个限制玻尔兹曼机（Restricted Boltzmann Machines）层组成，一个典型的神经网络类型如图三所示。这些网络被“限制”为一个可视层和一个隐层，层间存在连接，但层内的单元间不存在连接。隐层单元被训练去捕捉在可视层表现出来的高阶数据的相关性。

       首先，先不考虑最顶构成一个联想记忆（associative memory）的两层，一个DBN的连接是通过自顶向下的生成权值来指导确定的，RBMs就像一个建筑块一样，相比传统和深度分层的sigmoid信念网络，它能易于连接权值的学习。

       最开始的时候，通过一个非监督贪婪逐层方法去预训练获得生成模型的权值，非监督贪婪逐层方法被Hinton证明是有效的，并被其称为对比分歧（contrastive divergence）。

       在这个训练阶段，在可视层会产生一个向量v，通过它将值传递到隐层。反过来，可视层的输入会被随机的选择，以尝试去重构原始的输入信号。最后，这些新的可视的神经元激活单元将前向传递重构隐层激活单元，获得h（在训练过程中，首先将可视向量值映射给隐单元；然后可视单元由隐层单元重建；这些新可视单元再次映射给隐单元，这样就获取新的隐单元。执行这种反复步骤叫做吉布斯采样）。这些后退和前进的步骤就是我们熟悉的Gibbs采样（这个理解不对，这种编码解码的目的只是无监督自动特征提取的误差逼近方式，但在隐单元，或者可视单元，依据概率取值才是Gibbs采样，例如有p(h|v),采样h，或者由p（v|h）采样v），而隐层激活单元和可视层输入之间的相关性差别就作为权值更新的主要依据。

       训练时间会显著的减少，因为只需要单个步骤就可以接近最大似然学习。增加进网络的每一层都会改进训练数据的对数概率，我们可以理解为越来越接近能量的真实表达。这个有意义的拓展，和无标签数据的使用，是任何一个深度学习应用的决定性的因素。

![](https://img-my.csdn.net/uploads/201304/10/1365561659_1933.jpg)

       在最高两层，权值被连接到一起，这样更低层的输出将会提供一个参考的线索或者关联给顶层，这样顶层就会将其联系到它的记忆内容。而我们最关心的，最后想得到的就是判别性能，例如分类任务里面。

       在预训练后，DBN可以通过利用带标签数据用BP算法去对判别性能做调整。在这里，一个标签集将被附加到顶层（推广联想记忆），通过一个自下向上的，学习到的识别权值获得一个网络的分类面。这个性能会比单纯的BP算法训练的网络好。这可以很直观的解释，DBNs的BP算法只需要对权值参数空间进行一个局部的搜索，这相比前向神经网络来说，训练是要快的，而且收敛的时间也少。

       DBNs的灵活性使得它的拓展比较容易。一个拓展就是卷积DBNs（Convolutional Deep Belief Networks(CDBNs)）。DBNs并没有考虑到图像的2维结构信息，因为输入是简单的从一个图像矩阵一维向量化的。而CDBNs就是考虑到了这个问题，它利用邻域像素的空域关系，通过一个称为卷积RBMs的模型区达到生成模型的变换不变性，而且可以容易得变换到高维图像。DBNs并没有明确地处理对观察变量的时间联系的学习上，虽然目前已经有这方面的研究，例如堆叠时间RBMs，以此为推广，有序列学习的dubbed temporal convolutionmachines，这种序列学习的应用，给语音信号处理问题带来了一个让人激动的未来研究方向。

       目前，和DBNs有关的研究包括堆叠自动编码器，它是通过用堆叠自动编码器来替换传统DBNs里面的RBMs。这就使得可以通过同样的规则来训练产生深度多层神经网络[架构](http://lib.csdn.net/base/architecture)，但它缺少层的参数化的严格要求。与DBNs不同，自动编码器使用判别模型，这样这个结构就很难采样输入采样空间，这就使得网络更难捕捉它的内部表达。但是，降噪自动编码器却能很好的避免这个问题，并且比传统的DBNs更优。它通过在训练过程添加随机的污染并堆叠产生场泛化性能。训练单一的降噪自动编码器的过程和RBMs训练生成模型的过程一样。




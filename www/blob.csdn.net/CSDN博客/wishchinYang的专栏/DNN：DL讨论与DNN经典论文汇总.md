# DNN：DL讨论与DNN经典论文汇总 - wishchinYang的专栏 - CSDN博客
2013年12月31日 15:04:06[wishchin](https://me.csdn.net/wishchin)阅读数：2613
引言：
       达尔文的进化哲学：适者生存！其原因不是因为适者生存，而是因为不适者都会死去；
       没有免费的午餐定理：没有普适性的优越算法，如果一个算法对一个或一些应用相对其他算法表现极大的优越性，那么在特定领域必然也体验足够的弱势；
       纯科学的存在价值：不能转化为应用的科学是没有价值的，纯科学的存在价值，就是维持整个科学体系的严谨性；
       理想与现实：在实践多年以后，计算机科学家会发现其毕生的热爱只是对未来美好的憧憬的期待，并去努力地弥补那些差距；
       奥卡姆剃刀原理：如果我们可以以一种确定的方式解决问题，就不用模糊的方法；如果我们能以一种简单的方法解决问题，就不使用复杂方法；
Tips:[http://www.qiuxe.com/?p=26](http://www.qiuxe.com/?p=26)  作者：丘宪恩
       两个小朋友学习加减法，聪明的小朋友掌握了加减法的规律，例如进位制等，很快学会了。来了个新的题目，他按照那些规律很快就算出来了。但是笨一点的小朋友可能没掌握到规律，就死记硬背，把所以可能的两个数相加的结果都背了下来，来了个新的题目后发现这个东西见过，于是也很快给出来答案（但是他背得很累）。当然聪明的小朋友也是要记住一些东西的，但是他记住的东西非常少，只有一些简单的规则和规律。
        与之类似，     聪明的算法和笨算法之间的最本质的区别之一就是它们要记住的东西不一样，不需要记任何东西的算法是不存在的。聪明的算法，只需要记住少量的东西，如贝叶斯只需要记住各个特征的概率分布，线性回归只需要记住各个特征的权重；而笨算法如神经网络就要记住30亿个参数！
       这只是作者和大家开的一个玩笑，并且下面也做了相当的解释；贝叶斯和logistic回归可以称之为算法时适当的，而DNN简单的归结为算法就不适当了，DNN更像是一个数据与结构（算法）相结合的大的模型。
       简单bayes网络和logistic分类器只记住少量参数，其特性基于人给出的先验知识：例如假设特征独立，问题线性可分等。这些都是经过人的智能进行抽象提取的高层知识总结，可以应用于特定领域你，那么泛化能力必然降低。
1. 关于DNN训练：
       为什么神经网络特别是深度网络结构有着比较好的识别效果。神经网络特别是深度网络的特点之一就是层次多，节点多，其结果就是参数多。举个例子，就以Hinton 2006年发表在Science上的那篇文章Fig.1里面所用的网络结构为例。在网络在降维阶段就有4层，节点数量分别是2000， 1000， 500， 30，层与层之间两两全连接，总共的参数数量为2000*1000*500*30= 3,000,000,000, 多达30亿个参数！假设有10万个训练样本，每一个训练样本相当于一个方程，10万个样本相当于10万个方程（姑且不论方程相关性问题），但是总共有30亿个参数！如果你比较熟悉方程理论的话，就会知道这很明显就是个不定方程（参数数量比方程数量多）！不定方程是有无穷个解的，更别提通过训练得到一些近似解了。所以用这么多的参数有那么好的识别效果一点也不足为奇。
       但是还没完，也许会有人争辩说，虽然这是个不定方程问题，但是如何解释泛化能力的问题呢？问得好！其实不定方程带来的问题恰恰就是过拟合问题，也就是泛化能力不足。这一点早在上个世纪神经网络开始流行的时候就已经被人诟病。所以才有了后来的各种各样的正则化来解决过拟合的问题。
       除了正则化技术，还可以通过用足够多的训练样本进行大规模训练来解决过拟合的问题。在现在数据越来越多，计算能力越来越强的大数据背景下，这种大规模训练变得可能，这也就是为什么神经网络现在能复兴的原因之一（以前没有那么多数据也没有那么强的计算能力）。
       但是且慢，当你的训练样本足够多的时候，多到几乎穷举了所有样本空间的时候，对新来的样本正确识别就更加不足为奇了，因为所有可能样本你都见过了，为什么会识别不出来呢？大规模训练样本加上大规模参数集，说的不好听一点就是用超级野蛮超级暴力的方法强行记住了所有东西， 死记硬背，来了个新东西就去查找记忆，这还是智能么？
       ...... 说到底，对生物本身的智能认识不如我们对我们自己创造出来的DNN的认识，对生物智能的 “聪明”为何物，现在谁都给不了一个确切的定义。对AI的最好评定便是图灵先生的 图灵测试，所见即所得是我们还未曾见到本质之前对事物理解的最靠谱的方式，因为表象与本质具有本质的联系。
       由此，可见DNN比bayesian网络与logistic分类器聪明，因为我们可以看到了聪明的分类结果，而bayesian和logistic更像是一个执行工具。DNN近似于特征的重提取，表现出一定的智能。
       DNN更像是一个数据存储类型转换器，它把大量的训练样本以参数的形式存储在DNN里面；每一个参数模型对应了一个样本集合模型，在巨大的模型映射里面，如果建立的映射足够多，那么对模型的描述就足够稠密，对于其应用的领域便足够精确。
2.关于局部最优解：
       ANN会陷入局部最优解，这并不代表ANN就不是智能；人脑的知识模型是一个不断增长的模型，建立，扩增，再完备；在整个模型完备之前，在这个知识框架之内，对于某一个问题，得到的必然是局部最优解。
**一：其中有些转自于DL网站和其他网站**：[http://deeplearning.net](http://deeplearning.net/tutorial/)
查看：深度学习是ML研究的一个较新的领域，其在结构上使ML更接近AI的原始目标.
[1.  a brief introduction to Machine Learning for AI](http://www.iro.umontreal.ca/~pift6266/H10/notes/mlintro.html)  ：这个是简介页..
### [Table Of Contents](http://www.iro.umontreal.ca/~pift6266/H10/notes/contents.html)
- [Very Brief Introduction to Machine Learning for AI](http://www.iro.umontreal.ca/~pift6266/H10/notes/mlintro.html)- [Intelligence](http://www.iro.umontreal.ca/~pift6266/H10/notes/mlintro.html#intelligence)
- [Artificial Intelligence](http://www.iro.umontreal.ca/~pift6266/H10/notes/mlintro.html#artificial-intelligence)
- [Machine Learning](http://www.iro.umontreal.ca/~pift6266/H10/notes/mlintro.html#machine-learning)
- [Formalization of Learning](http://www.iro.umontreal.ca/~pift6266/H10/notes/mlintro.html#formalization-of-learning)
- [Supervised Learning](http://www.iro.umontreal.ca/~pift6266/H10/notes/mlintro.html#supervised-learning)
- [Unsupervised Learning](http://www.iro.umontreal.ca/~pift6266/H10/notes/mlintro.html#unsupervised-learning)
- [Local Generalization](http://www.iro.umontreal.ca/~pift6266/H10/notes/mlintro.html#local-generalization)
- [Distributed versus Local Representation and Non-Local
 Generalization](http://www.iro.umontreal.ca/~pift6266/H10/notes/mlintro.html#distributed-versus-local-representation-and-non-local-generalization)
2. AI的子领域迅速崛起，并自立门户，剩下一些执着的科学家还在寻找真正可以通晓整个逻辑的东西。
和 ：
[3.  an introduction to Deep Learning algorithms](http://www.iro.umontreal.ca/~pift6266/H10/notes/deepintro.html).：最好是把这个简介看完...
经典文献：[Yoshua Bengio, Learning Deep Architectures for AI, Foundations and Trendsin Machine Learning, 2(1),
 2009](http://www.iro.umontreal.ca/~lisa/publications2/index.php/publications/show/239)
[](http://www.iro.umontreal.ca/~lisa/publications2/index.php/publications/show/239)
4. 深度学习是关于学习多个表示和抽象层次，这些层次帮助解释数据，例如图像，声音和文本。对于更多的关于深度学习算法的知识，查看：
- The monograph or review paper [Learning Deep Architectures for AI](http://www.iro.umontreal.ca/~lisa/publications2/index.php/publications/show/239) (Foundations & Trends in Machine Learning, 2009).
- The ICML 2009 Workshop on Learning Feature Hierarchies [webpage](http://www.cs.toronto.edu/~rsalakhu/deeplearning/index.html) has a [list of references](http://www.cs.toronto.edu/~rsalakhu/deeplearning/references.html). 
- The LISA [public wiki](http://www.iro.umontreal.ca/~lisa/twiki/bin/view.cgi/Public/WebHome) has a [reading list](http://www.iro.umontreal.ca/~lisa/twiki/bin/view.cgi/Public/ReadingOnDeepNetworks) and a [bibliography](http://www.iro.umontreal.ca/~lisa/twiki/bin/view.cgi/Public/DeepNetworksBibliography). 
- Geoff Hinton has [readings](http://www.cs.toronto.edu/~hinton/deeprefs.html) from last year’s [NIPS tutorial](http://videolectures.net/jul09_hinton_deeplearn/).
5. 这个算法的综述有一些先决条件：
        Python给了科研工作者一个可以摆脱Matlab的产品级程序工具，并提供了优越特性.
       应该首先知道一个关于python的知识，并熟悉numpy。
       由于这个综述是关于如何使用Theano，你应该首先阅读[Theano basic tutorial](http://deeplearning.net/software/theano/tutorial)。
       一旦你完成这些，阅读我们的[*Getting Started*](http://deeplearning.net/tutorial/gettingstarted.html#gettingstarted)章节---它将介绍概念定义，数据集，和利用随机梯度下降来优化模型的方法。
纯有监督学习算法可以按照以下顺序阅读：
- [*Logistic Regression*](http://deeplearning.net/tutorial/logreg.html#logreg) - using Theano for something simple
- [*Multilayer perceptron*](http://deeplearning.net/tutorial/mlp.html#mlp) - introduction to layers
- [*Deep Convolutional Network*](http://deeplearning.net/tutorial/lenet.html#lenet) - a simplified version of LeNet5
无监督和半监督学习算法可以用任意顺序阅读(auto-encoders可以被独立于RBM/DBM地阅读)：
- [*Auto Encoders, Denoising Autoencoders*](http://deeplearning.net/tutorial/dA.html#daa) - description of autoencoders
- [*Stacked Denoising Auto-Encoders*](http://deeplearning.net/tutorial/SdA.html#sda) - easy steps into unsupervised pre-training for deep nets
- [*Restricted Boltzmann Machines*](http://deeplearning.net/tutorial/rbm.html#rbm) - single layer generative RBM model
- [*Deep Belief Networks*](http://deeplearning.net/tutorial/DBN.html#dbn) - unsupervised generative pre-training of stacked RBMs followed by supervised fine-tuning
关于mcRBM模型，我们有一篇新的关于从能量模型中抽样的综述：
- [*HMC Sampling*](http://deeplearning.net/tutorial/hmc.html#hmc) - hybrid (aka Hamiltonian) Monte-Carlo sampling with scan()
**二：算法简介： **
查看最新论文[：Yoshua Bengio, Learning Deep Architectures for AI, Foundations and Trends in Machine Learning, 2(1), 2009](http://www.iro.umontreal.ca/~lisa/publications2/index.php/publications/show/239)
## 深度(Depth)    
## 此段原始链接：[an introduction to Deep Learning](http://www.iro.umontreal.ca/~pift6266/H10/notes/deepintro.html)
       从一个输入中产生一个输出所涉及的计算可以通过一个流向图(** flow graph**)来表示：流向图是一种能够表示计算的图，在这种图中每一个节点表示一个基本的计算并且一个计算的值(计算的结果被应用到这个节点的孩子节点的值)。考虑这样一个计算集合，它可以被允许在每一个节点和可能的图结构中，并定义了一个函数族。输入节点没有孩子，输出节点没有父亲。
       对于表达![1](http://images.cnblogs.com/cnblogs_com/ysjxw/201110/201110081042193171.png)
 的流向图，可以通过一个有两个输入节点![2](http://images.cnblogs.com/cnblogs_com/ysjxw/201110/201110081042242152.png)
 和![3](http://images.cnblogs.com/cnblogs_com/ysjxw/201110/201110081042251464.png)
 的图表示，其中一个节点通过使用![2](http://images.cnblogs.com/cnblogs_com/ysjxw/201110/201110081042279380.png)和![3](http://images.cnblogs.com/cnblogs_com/ysjxw/201110/201110081042315014.png)
 作为输入(例如作为孩子)来表示![4](http://images.cnblogs.com/cnblogs_com/ysjxw/201110/201110081042322341.png)
 ；一个节点仅使用![2](http://images.cnblogs.com/cnblogs_com/ysjxw/201110/201110081042349700.png)
 作为输入来表示平方；一个节点使用![6](http://images.cnblogs.com/cnblogs_com/ysjxw/201110/201110081042355980.png)
 和![7](http://images.cnblogs.com/cnblogs_com/ysjxw/201110/201110081042365848.png)
 作为输入来表示加法项(其值为![5](http://images.cnblogs.com/cnblogs_com/ysjxw/201110/201110081042381713.png)
 )；最后一个输出节点利用一个单独的来自于加法节点的输入计算SIN。
这种流向图的一个特别属性是深度(depth)：从一个输入到一个输出的最长路径的长度。
       传统的前馈神经网络能够被看做拥有等于层数的深度(比如对于输出层为隐层数加1)。SVMs有深度2(一个对应于核输出或者特征空间，另一个对应于所产生输出的线性混合)。
## 深度架构的动机
学习基于深度架构的学习算法的主要动机是：
       不充分的深度是有害的；
       大脑有一个深度架构；
       认知过程是深度的；
##### 不充分的深度是有害的
       在许多情形中深度2就足够(比如logical gates, formal [threshold] neurons, sigmoid-neurons, Radial Basis Function [RBF] units like in SVMs)表示任何一个带有给定目标精度的函数。但是其代价是：图中所需要的节点数(比如计算和参数数量)可能变的非常大。理论结果证实那些事实上所需要的节点数随着输入的大小指数增长的函数族是存在的。这一点已经在logical gates, formal [threshold]
 neurons 和rbf单元中得到证实。在后者中Hastad说明了但深度是d时，函数族可以被有效地(紧地)使用O(n)个节点(对于n个输入)来表示，但是如果深度被限制为d-1，则需要指数数量的节点数O(2^n)。
        我们可以**将深度架构看做一种因子分解**。大部分随机选择的函数不能被有效地表示，无论是用深地或者浅的架构。但是许多能够有效地被深度架构表示的却不能被用浅的架构高效表示(see the polynomials example in the[Bengio
 survey paper](http://www.iro.umontreal.ca/~lisa/publications2/index.php/publications/show/239))。一个紧的和深度的表示的存在意味着在潜在的可被表示的函数中存在某种结构。如果不存在任何结构，那将不可能很好地泛化。
##### 大脑有一个深度架构
       例如，视觉皮质得到了很好的研究，并显示出一系列的区域，在每一个这种区域中包含一个输入的表示和从一个到另一个的信号流(这里忽略了在一些层次并行路径上的关联，因此更复杂)。这个特征层次的每一层表示在一个不同的抽象层上的输入，并在层次的更上层有着更多的抽象特征，他们根据低层特征定义。
       需要注意的是大脑中的表示是在中间紧密分布并且纯局部：他们是稀疏的：1%的神经元是同时活动的。给定大量的神经元，任然有一个非常高效地(指数级高效)表示。
##### 认知过程看起来是深度的
- 人类层次化地组织思想和概念；
- 人类首先学习简单的概念，然后用他们去表示更抽象的；
- 工程师将任务分解成多个抽象层次去处理；
       学习/发现这些概念(知识工程由于没有反省而失败？)是很美好的。对语言可表达的概念的反省也建议我们一个稀疏的表示：仅所有可能单词/概念中的一个小的部分是可被应用到一个特别的输入(一个视觉场景)。
## 学习深度架构的突破
2006年前，尝试训练深度架构都失败了：训练一个深度有监督前馈神经网络趋向于产生坏的结果(同时在训练和测试误差中)，然后将其变浅为1(1或者2个隐层)。
2006年的3篇论文改变了这种状况，由Hinton的革命性的在深度信念网(Deep Belief Networks, DBNs)上的工作所引领：
- Hinton, G. E., Osindero, S. and Teh, Y., [A fast learning algorithm for deep belief nets](http://www.cs.toronto.edu/~hinton/absps/fastnc.pdf) .Neural Computation 18:1527-1554, 2006
- Yoshua Bengio, Pascal Lamblin, Dan Popovici and Hugo Larochelle, [Greedy Layer-Wise Training of Deep Networks](http://www.iro.umontreal.ca/~lisa/publications2/index.php/publications/show/190), in J. Platt et al. (Eds), Advances in Neural Information Processing Systems 19 (NIPS 2006), pp. 153-160, MIT Press, 2007
- Marc’Aurelio Ranzato, Christopher Poultney, Sumit Chopra and Yann LeCun [Efficient Learning of Sparse Representations with an Energy-Based Model](http://yann.lecun.com/exdb/publis/pdf/ranzato-06.pdf), in J. Platt et al. (Eds), Advances in Neural Information Processing Systems (NIPS 2006), MIT Press, 2007
在这三篇论文中以下主要原理被发现：
- 表示的无监督学习被用于(预)训练每一层；
- 在一个时间里的一个层次的无监督训练，接着之前训练的层次。在每一层学习到的表示作为下一层的输入；
- 用无监督训练来调整所有层(加上一个或者更多的用于产生预测的附加层)；
       DBNs在每一层中利用用于表示的无监督学习RBMs。Bengio et al paper 探讨和对比了RBMs和auto-encoders(通过一个表示的瓶颈内在层预测输入的神经网络)。Ranzato et al paper在一个convolutional架构的上下文中使用稀疏auto-encoders(类似于稀疏编码)。Auto-encoders和convolutional架构将在以后的课程中讲解。
**三：原始经典文献：**
（1）：Hinton 06年发表在Science上面的奠基性著作：
               G. E. Hinton, R. R. Salakhutdinov, Reducing the Dimensionality of Data with Neural Networks, Science, vol. 313, no. 5786, pp. 504-507, 2006
![](https://img-blog.csdn.net/20131231154103156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20131231154121093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20131231154139921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20131231154258187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（2）：我想说的是，脑认知模型或许是最后的路径...不过还是看的不够远啊
**四：[Andrew Ng谈Deep Learning](www.csdn.net/article/2014-08-01/2821007)**
### **Deep Learning与AI**
**《程序员》：**
**         Amara法则说，“我们倾向于高估科技的短期影响力，而又低估其长期影响力。”在你看来，Deep Learning的短期和长期影响分别是什么？历史上，我们曾对实现人工智能有过错误估计，对于Deep Learning的前景，人们是否过于乐观？**
Andrew Ng：
        我对Deep Learning的前景很乐观，它的价值在过去几年已得到印证，未来我们还会沿着这个方向继续努力。语音识别、计算机视觉都将获得长足进步，数据与科技的碰撞，会让这一切变得更具价值。在短期，我们会看到身边的产品变得更好；而长期，它有潜力改变我们与计算机的交互方式，并凭借它创造新的产品和服务。
        不过围绕Deep Learning，我也看到存在着某种程度的夸大，这是一种不健康的氛围。它不单出现于媒体的字里行间，也存在于一些研究者之中。将Deep Learning描绘成对人脑的模拟，这种说法颇具吸引力，但却是过于简化的模仿，它距离真正的AI或人们所谓的“奇点”还相当遥远。目前这项技术主要是从海量数据当中学习，理解数据，这也是现今有关Deep Learning技术研究和产品发展的驱动力。而具备与人的能力相匹配的AI需要无所不包，例如人类拥有丰富的感情、不同的动机，以及同感能力。这些都是当下Deep
 Learning研究尚未涉及的。
.............................
**《程序员》：**
**        有一种说法是，对人脑机制理解的缺乏是我们开发智能机器的最大限制之一，在这个存在许多假设和未知的前沿领域进行研究，你怎样判断自己研究的方向和做出的各种选择是否正确？**
Andrew Ng：
       诚然，神经科学尚未揭开人脑的运作机制，是对这项研究的一种制约。但如今我们尝试的算法，大多只是粗略地基于神经科学研究的统计阐释，这些研究启迪我们的灵感，鼓励我们尝试新算法。但现实中，我们更主要地依据算法真实的运行效果进行评判，假如一味追求模拟神经的运作方式，不一定能带来最优的结果。有时我们偏重神经科学原理，例如某些模拟大脑局部的算法；但更多时候，性能是准绳。若按比例划分，前者大约只占2%，后者则占据98%。
因为我们不知道何种算法最优，所以才不断尝试，衡量是否取得进步的方式之一（并非唯一方式），是观察新算法能否在应用中表现得更好，例如Web和图像搜索结果是否更准确，或者语音识别的正确率更高。假如回望五年，你就会发现，那时我们曾认为颇有前景的算法，如今已然被抛弃。这些年，我们有规律地，甚至偶然地发现一些新算法，推动着这个领域持续前进。
.............................
**《程序员》：**
**       关于Deep Learning的原理，已有许多人知晓。为了做出一流研究和应用，对于研究者来说，决胜的关键因素是什么？为何如今只有少数几人，成为这个领域的顶尖科学家？**
Andrew Ng：
       关于决定因素，我认为有三点最为关键。
       首先是数据，对于解决某些领域的问题，获取数据并非轻而易举；其次是计算基础架构工具，包括计算机硬件和软件；最后是这个领域的工程师培养，无论在斯坦福还是百度，我都对如何快速训练工程师从事Deep Learning研究，成为这个领域的专家思索了很长时间。幸运的是，我从Coursera和大学的教学经历中获益良多。创新往往来自多个观念的整合，源于一整支研究团队，而非单独一个人。
       从事Deep Learning研究的一个不利因素在于，这还是一个技术快速发展的年轻领域，许多知识并非依靠阅读论文便能获得。那些关键知识，往往只存在于顶尖科学家的头脑中，这些专家彼此相识，信息相互共享，却不为外人所知。另外一些时候，这群顶尖科学家也不能确定自己的灵感源于何处，如何向其他人解释。但我相信，越来越多的知识会传递给普通开发者；在百度，我也正努力寻找方法，将自己的灵感和直觉高效地传授给其他研究者。尽管我们已有这方面的教程，但需要改进之处仍有很多。
此外，许多顶尖实验室的迭代速度都非常快，而Deep Learning算法复杂，计算代价很高，这些实验室都拥有优秀的工具与之配合，从快速迭代中学习进步。
............................
**《程序员》：**
**       十年前，Jeff Hawkins在《On Intelligence》中已经向普通人描述了机器与智能之间的关系，这些描述与我们现在看到的Deep Learning似乎非常相似。在这最近的十年中，我们新学到了什么？**
Andrew Ng：
       包括我在内，Jeff Hawkins的作品启发了许多AI研究者，多年以前，我个人还曾是Hawkins这家公司的技术顾问之一。但在现实中，每个人的实现细节和算法迥异，与这本书其实颇有不同。例如在书中，Hawkins极为强调与时间相关的临时数据的重要性，而在Deep Learning中，虽然用到了临时数据，但远没有那重要，另外网络的架构也大不相同。在最近十年中，我们认识到了可扩展的重要性，另外我们还找到了进行非监督式学习更好的方式。
............................
**《程序员》：**
**       为了开发智能机器，许多年前，Daniel Hillis和他的Thinking Machines曾尝试突破von Neumann架构，你觉得当今的硬件是否是实现智能机器的最好选择？如果不是，当前的计算机架构有哪些限制，我们需要做哪方面的突破？**
Andrew Ng：
       这是一个有趣的话题。我们尚不知道怎样的硬件架构是智能机器的最佳选择，因而更需要拥有灵活性，快速尝试不同的算法。在这方面，GPU相对易于编程，因而可以高效地尝试不同的算法。作为对比，ASIC（专用集成电路）的运行速度比GPU更快，但开发适合Deep Learning的ASIC难度高、周期长，在漫长的研发过程中，很可能我们早已发现了新算法。
GPU与CPU结合是目前的首选硬件平台，不过随着技术的进一步成熟，这种现状有可能发生改观。例如，目前已有几家初创公司正在研发专门用于Deep Learning的硬件系统。
**后记：**
              一把利剑好用与否还要看用剑者的水准,DNN也是如此.........................................................

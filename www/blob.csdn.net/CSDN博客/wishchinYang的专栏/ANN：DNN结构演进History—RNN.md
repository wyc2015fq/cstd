# ANN：DNN结构演进History—RNN - wishchinYang的专栏 - CSDN博客
2015年08月24日 11:56:15[wishchin](https://me.csdn.net/wishchin)阅读数：1903
**前言废话：**
       CNN在图像处理领域的极大成功源于CNN的二维递进映射结构，通过训练多层卷积核来进行特征提取函数训练，在二维图像的稀疏表达和语义关联分析方面有天生的结构优势。而涉及时序问题的逻辑序列分析—边长序列分析，需要引入适合解决其问题的方法。
       引入RNN：在深度学习领域，传统的前馈神经网络（feed-forward neural net，简称FNN）具有出色的表现，取得了许多成功，它曾在许多不同的任务上——包括手写数字识别和目标分类上创造了记录。甚至到了今天，FNN在解决分类任务上始终都比其他方法要略胜一筹。
       尽管如此，大多数专家还是会达成共识：FNN可以实现的功能仍然相当有限。 究其原因，人类的大脑有着惊人的计算功能，而 “分类” 任务仅仅是其中很小的一个组成部分。我们不仅能够识别个体案例，**更能分析输入信息之间的整体逻辑序列**，即时序模式信息。这些信息序列富含有大量的内容，信息彼此间有着复杂的时间关联性，并且信息长度各种各样。例如视觉、开车、演讲还有理解能力，这些都需要我们同时处理更高维度的多种输入信息，因为它们时时都在变化，而这是FNN在建模时就极为匮乏的。
       现在的问题在于如何学习信息的逻辑顺序，解决这一问题有一个相当靠谱的途径，那就是递归神经网络（Recurrent Neural Net，简称RNN）。递归神经网络无需在层面之间构建，同时引入**定向循环**，能够更好地处理高维度信息的整体逻辑顺序。
       原文链接：[A Deep Dive into Recurrent Neural Nets](http://nikhilbuduma.com/2015/01/11/a-deep-dive-into-recurrent-neural-networks/) （编译/孙薇 校对/周建丁）；原文链接：[http://www.csdn.net/article/2015-01-28/2823747](http://www.csdn.net/article/2015-01-28/2823747)   ；此外还有RNN的一篇详细介绍，值得一看：[递归神经网络(RNN, Recurrent Neural Networks)介绍](http://blog.csdn.net/heyongluoyao8/article/details/48636251)
        强烈推荐，这一个即经典又理解深刻且能深入浅出：[循环神经网络（RNN）-karpathy.github.io/2015](http://karpathy.github.io/2015/05/21/rnn-effectiveness/)；建议详细研读此文，经典深刻、层层深入，且有代码做出了详细演示。
       RNN通过引入神经元定向循环用于处理边变长问题，由此被称为递归网络；再通过其他神经元（如果有自我连接则包括自身）的输入和当前值的输入，进行加权求和（logit）之后重新计算出新的行为，保存之前记忆。通过时间轴展开成类似于FNN的新构架，因此可以使用BP算法进行网络训练；而根据时间展开长序列会产生极深FNN，容易产生梯度的消失与爆炸问题，因此引入了LSTM-长短期记忆，保持一个常数误差流，以此保证梯度的不会爆炸消失；用于恒稳误差，通常使用一个门单元进行误差流控制。
## Recurrent Neural Networks
**Sequences**.
          Depending on your background you might be wondering: *What makes Recurrent Networks so special*? A glaring limitation of Vanilla Neural Networks (and also Convolutional Networks) is that their API is too constrained: they accept a fixed-sized vector as input (e.g. an image) and produce a fixed-sized vector as output (e.g. probabilities of different classes). Not only that: These models perform this mapping using a fixed amount of computational steps (e.g. the number of layers in the model). The core reason that recurrent nets are more exciting is that they allow us to operate over *sequences* of vectors: Sequences in the input, the output, or in the most general case both. A few examples may make this more concrete:
![](http://karpathy.github.io/assets/rnn/diags.jpeg)
            Each rectangle is a vector and arrows represent functions (e.g. matrix multiply). Input vectors are in red, output vectors are in blue and green vectors hold the RNN's state (more on this soon). From left to right: **(1)** Vanilla mode of processing without RNN, from fixed-sized input to fixed-sized output (e.g. image classification). **(2)** Sequence output (e.g. image captioning takes an image and outputs a sentence of words). **(3)** Sequence input (e.g. sentiment analysis where a given sentence is classified as expressing positive or negative sentiment). **(4)** Sequence input and sequence output (e.g. Machine Translation: an RNN reads a sentence in English and then outputs a sentence in French). **(5)** Synced sequence input and output (e.g. video classification where we wish to label each frame of the video). Notice that in every case are no pre-specified constraints on the lengths sequences because the recurrent transformation (green) is fixed and can be applied as many times as we like.
           上述一段主要描述几种简单的RNN网络：根据输入输出的维度，划分为1对1，可以用于单输出的时序单维度变长序列模式识别，如手势识别；1对多，输出多维度的变长序列模式识别，如固定编码器；多对1，可以用于单输出的时序多维度变长序列模式识别，如步态识别；延迟多对多，比如翻译网络；实时多对多，如视频分析模式识别。
**一、RNN是什么/类别？**
         维基百科：[https://en.wikipedia.org/wiki/RNN](https://en.wikipedia.org/wiki/RNN)
- [1Architectures](https://en.wikipedia.org/wiki/Recurrent_neural_network#Architectures)- [1.1Fully recurrent network    全连接递归神经网络](https://en.wikipedia.org/wiki/Recurrent_neural_network#Fully_recurrent_network)
- [1.2Hopfield network              霍普菲尔德神经网络](https://en.wikipedia.org/wiki/Recurrent_neural_network#Hopfield_network)
- [1.3Elman networks and Jordan networks](https://en.wikipedia.org/wiki/Recurrent_neural_network#Elman_networks_and_Jordan_networks)
- [1.4Echo state network          回声状态网络](https://en.wikipedia.org/wiki/Recurrent_neural_network#Echo_state_network)
- [1.5Long short term memory network                  长短期网络](https://en.wikipedia.org/wiki/Recurrent_neural_network#Long_short_term_memory_network)
- [1.6Bi-directional RNN                                         双向网络](https://en.wikipedia.org/wiki/Recurrent_neural_network#Bi-directional_RNN)
- [1.7Continuous-time RNN                                    CTRNN连续时间网络](https://en.wikipedia.org/wiki/Recurrent_neural_network#Continuous-time_RNN)
- [1.8Hierarchical RNN                                           分层RNN](https://en.wikipedia.org/wiki/Recurrent_neural_network#Hierarchical_RNN)
- [1.9Recurrent multilayer perceptron](https://en.wikipedia.org/wiki/Recurrent_neural_network#Recurrent_multilayer_perceptron)[递归多层网络](https://en.wikipedia.org/wiki/Recurrent_neural_network#Hierarchical_RNN)
- [1.10Second Order Recurrent Neural Network    二阶递归神经网络](https://en.wikipedia.org/wiki/Recurrent_neural_network#Second_Order_Recurrent_Neural_Network)
- [1.11Multiple Timescales Recurrent Neural Network (MTRNN) Model     多时间瓷都递归网络模型](https://en.wikipedia.org/wiki/Recurrent_neural_network#Multiple_Timescales_Recurrent_Neural_Network_.28MTRNN.29_Model)
- [1.12Pollack’s sequential cascaded networks      序列级联网络](https://en.wikipedia.org/wiki/Recurrent_neural_network#Pollack.E2.80.99s_sequential_cascaded_networks)
- [1.13Neural Turing Machines                                神经图灵机](https://en.wikipedia.org/wiki/Recurrent_neural_network#Neural_Turing_Machines)
- [1.14Bidirectional Associative Memory (BAM)      双向联想记忆（网络）](https://en.wikipedia.org/wiki/Recurrent_neural_network#Bidirectional_Associative_Memory_.28BAM.29)
- 
- [2Training](https://en.wikipedia.org/wiki/Recurrent_neural_network#Training)- [2.1Gradient descent                                   梯度下降法](https://en.wikipedia.org/wiki/Recurrent_neural_network#Gradient_descent)
- [2.2Global optimization methods                 全局优化方法](https://en.wikipedia.org/wiki/Recurrent_neural_network#Global_optimization_methods)
- [3Related fields and models](https://en.wikipedia.org/wiki/Recurrent_neural_network#Related_fields_and_models)
         RNN建立在与FNN相同的计算单元上，两者之间区别在于：组成这些神经元相互关联的架构有所不同。FNN是建立在层面之上，其中信息从输入单元向输出单元单向流动，在这些连通模式中并不存在不定向的循环。尽管大脑的神经元确实在层面之间的连接上包含有不定向循环，我们还是加入了这些限制条件，以牺牲计算的功能性为代价来简化这一训练过程。因此，为了创建更为强大的计算系统，我们允许RNN打破这些人为设定强加性质的规定：**RNN无需在层面之间构建，同时定向循环也会出现**。事实上，神经元在实际中是允许彼此相连的。
![](http://img.ptcms.csdn.net/article/201501/29/54c985da2f578.jpg)
RNN例图，包含直接循环和内部连通
         RNN包含输入单元（input units）群，我们将其标记为u1，u2直到uK，而输出单元（output units）群则被标记为y1，y2直到yL。RNN还包含隐藏单元（hidden units），我们将其标记为x1，x2直到xN，这些隐藏单元完成了最为有意思的工作。
         你会发现，在例图中：有一条单向流动的信息流是从输入单元到达隐藏单元的，与此同时另一条单向流动的信息流从隐藏单元到达输出单元。在某些情况下，RNN会打破后者的限制，**引导信息从输出单元返回隐藏单元**，这些被称为“**backprojections-反向影响**”，不让RNN分析更加复杂。我们在这里讨论的技术同样适用于包含backprojections的RNN。
        训练RNN存在很多相当具有挑战性的难题，而这仍是一个非常活跃的研究领域。了解概念之后，本文将带您深入探析RNN的原理、训练和优化等各方面的内容，以及RNN已经获取的一些成就。
**模拟RNN-变长时间分析**
         现在我们了解到RNN的结构了，可以讨论一下RNN模拟一系列事件的方式。举个简单的例子，下文中的这个RNN的运作方式类似一个计时器模块，这是由Herbert Jaeger设计的一个经典案例（他的原稿请点击[这里](http://www.pdx.edu/sites/www.pdx.edu.sysc/files/Jaeger_TrainingRNNsTutorial.2005.pdf)查看）。
![](http://img.ptcms.csdn.net/article/201501/29/54c98609d5804.jpg)
简单案例：一个完美的RNN如何模拟计时器
       在这个例子中，我们有两个输入单元，输入单元u1相当于一个二进制开关，峰值时数值为1（在RNN开始计时的时候）；输入单元u2是一个离散变量，其取值范围在0.1到1.0之间变化，指的是计时器如果在那一瞬间开启，则输出值相应开启的长度。在RNN的规范中，要求它将输出结果持续在1000 u2的区间里开启。最终，训练案例中的输出结果会在0（关闭）与0.5（开启）之间来回拨动。
       但是，一个神经网络究竟是如何完成这个计算的呢？首先，RNN的所有隐藏层行为会被初始化成为一些预设的状态，然后在每个时间步长中（时间 t =1，2，……），所有的隐藏单元通过外部连接发送其当前的行为，再通过其他神经元（如果有自我连接则包括自身）的输入和当前值的输入，**进行加权求和（logit）之后重新计算出新的行为**，然后将其写入神经元的特定功能中（**简单的复制操作，可调函数，soft-max**等等）。
        因为之前的行为矢量被用在计算每个时间步长的行为矢量中，RNN可以**保留之前的事件记忆**并使用该记忆来做决定。
        显然一个神经网络不大可能完全根据规范而构建，但是可以想象一下，在RNN的训练进行过数百次或数千次之后，其输出结果（橙色）会非常接近客观数据（蓝色）。下文中我们会对RNN训练的方式进行更多讨论。
![](http://img.ptcms.csdn.net/article/201501/29/54c9861f61537.jpg)
经过良好的训练后，RNN在实验案例中接近输出测试用例
       此时此刻，你可能觉得这相当酷，但是有相当多的案例都很不自然。实践中运用RNN的策略是什么呢？我们调查了**真实的系统以及随着时间流逝它们对于刺激物的回应行为**。举例来说，你可以教会一个RNN通过建立一个数据组将声频转化为文字（在某种意义上，在训练组中观察人类的听觉系统对于输入内容的回应）。你还可以使用一个训练过的神经网络来模拟一个系统在异常刺激下的反映。
![](http://img.ptcms.csdn.net/article/201501/29/54c9864c84953.jpg)
RNN在实践中如何运用
       但是如果你富有创意的话，可以通过更为惊人方式来使用RNN，比如一种专门的RNN——LSTM（**Long Short-Term Memory**），就已经被用来实现规模巨大的数据压缩比率了，尽管目前基于RNN的压缩方法还需要花费大量的时间。（后文将有详细解释。）
**RNN-BP算法训练（**通过时间进行（BackPropagation）**）**
        我们一开始又是如何对RNN进行训练，让它来完成所有这些惊人的功能呢？尤其我们是如何确定每个连接的强度（或称权值）呢？我们又是如何从所有隐藏单元中选择初始行为的呢？我们的第一反应可能是直接使用BP算法，毕竟这种算法在FNN中使用过而且效果良好。
        这里使用BP算法的问题在于我们有着**周期性的依赖关系**。在FNN中，我们在针对一个层面中的权值来计算误差衍生时，可以根据上一层面中的误差衍生对其进行完全表达。RNN中并没有这种漂亮的分层，因为神经元并未组成有向非循环图。在RNN中使用BP算法可能会迫使我们根据它自身来进行误差衍生的表达，这样会使分析复杂化。
       因此，使用BP算法需要执行一个机智的转化：**将RNN转化为一个新的架构**，这个新架构在本质上来说就是一个FNN，我们将这个策略称为**通过时间“展开”RNN**。下面图表中有一个样例（为了简化起见，每个时间步长中只有一个输入/输出）：
![](http://img.ptcms.csdn.net/article/201501/29/54c9866eb9197.jpg)
通过时间将RNN “展开” 以使用BP算法
        具体步骤非常简单，但对我们分析神经网络的能力有着深远的影响。我们将 RNN输入、输出、隐藏单元和复制 分别看作时间步长。在我们的新FNN中，这些分别对应着不同的层面，然后我们将隐藏单元按照下面的方式连接起来，在原始的RNN中，w表示从神经元i到神经元j的连接强度（即权值），而在新的FNN中，我们画出一个连接w的图，分别连接每个tk层中的神经元i和每个tk+1层中的神经元j。
       这样一来，为了训练RNN，我们取随机的初始化权值，将其“展开”到FNN中，然后通过BP算法以确定最佳权值。为了确定时间0时隐藏状态的赋初值，我们可以**将初始行为视作注入FNN底层的参数**，然后通过BP算法以确定其最佳值。
       但是这里我们遇到了一个问题：在使用过每一批训练案例之后，我们需要基于计算出的误差衍生来修正权值。在FNN中，我们有一套连接关系，全部与原始的RNN中同样的连接关系相对应。但是，根据权值计算出的误差衍生却无法保证是相同的，也就是说我们可能会用不同的数量来修正误差。我们当然不想变成这样！
       对于这一问题，我们的解决办法是：**将同一组中所有连接的误差衍生求平均值（或者求和）**。也就是说每一批训练之后，我们会用相同的数量来修正相应连接，因此如果它们的赋初值相同，则最终值也会相同，这很好地解决了我们的问题。
**深层BP算法的问题**
       不同于传统的FNN，由展开RNN而产生的FNN可能会有非常多层。这就产生了一个严重的现实问题：通过时间手段使用BP算法来进行训练可能会变得非常困难。让我们退回一步，研究一下原因。
       我们试着训练RNN做一个非常简单的工作：先给RNN的一个隐藏单元赋予一个偏差值，然后把它与自身还有一个单一的输出接口相连接。我们希望这个神经网络在50步以后输出一个固定目标值，这个值我们设定为0.7。这样我们在将要在第50步的时候，使用输出的均方误差（squared error）作为误差函数，就能通过权值和偏差值画出一个曲面：
![](http://img.ptcms.csdn.net/article/201501/29/54c9868380825.jpg)
一个简单的RNN的问题误差曲面（图片出自：[Pascanu et al.](http://arxiv.org/pdf/1211.5063v2.pdf)）
       现在，假定我们从红色五角星处开始（使用一个随机的权值赋初值），你会注意到：在使用梯度下降（gradient descent）的时候，图线会越来越趋近于曲面的局部最小值。但是突然间，在稍微越过低谷并触及峭壁的时候，在**相反方向出现一个巨大的梯度**，这就使得我们向反方向反弹并远离局部最小值。
       一旦我们进入了虚空中，就会很快发现：梯度几近消失，想要再次接近则需要近乎无穷的时间。这个问题被称做“**梯度的爆发与消失**”（exploding and vanishing gradients），可以想象一下：我们可以通过改变梯度值，让它永远不要超过最大值来控制这个问题（请看撞到峭壁后点线的路径），但是这个方法仍然不是非常有效，尤其是在更复杂的RNN中。（想要查看这个问题的数学处理方法，请查看这篇[论文](http://arxiv.org/pdf/1211.5063v2.pdf)。）
**长短时记忆**
       为了解决这些问题，研究人员提出了RNN的修正架构，以协助在强制的输入、适当的回应与防止梯度爆发之间建立一个长期的时滞。这个架构强制其在特殊记忆单元中的内部状态保持一个**常数误差流**（constant error flow），**这样一来图线既不会爆发也不会消失**。这个长短时记忆（LSTM）架构利用了下面结构中显示的单元：
![](http://img.ptcms.csdn.net/article/201501/29/54c986b33e192.jpg)
基本的LSTM单元架构
        LSTM单元包含一个尝试将信息储存较久的存储单元。这个记忆单元的入口被一些特殊的门神经元（gate neurons）所保护，被保护的功能包括保存、写入和读取操作。这些门神经元都是logistic units，它们不会将自己的行为作为输入值发送给其他神经元，而是负责在神经网络的其它部分与记忆单元连接的边缘处设定权值。这个记忆单元是一个线型的神经元，有自体内部连接。当保存门被打开时（如1中的行为），自体连接权值为1，记忆单元将内容写入自身。当保存门输出为0时，记忆单元会清除之前的内容。写入门允许在输出值为1的时候，神经网络的其它部分将内容写入记忆单元，而读取门则允许在输出值为1的时候，神经网络的其它部分读取记忆单元。
       因此，这个操作究竟是如何保持一个时间的常数误差流，从而在局部防止爆发与消失梯度的产生呢？为了更加形象化，我们将LSTM单元按照时间展开：
![](http://img.ptcms.csdn.net/article/201501/29/54c986ce3d8ec.jpg)
通过时间区域展开LSTM单元
       首先，保存门被设定为0，写入门被设定为1，并在记忆单元中存入4.2这个值。在随后保存值为1的时候，4.2这个值一直被保存在记忆单元中，拒绝值为0时的读取或写入，最终这个单元在被读取后清除。现在，让我们试着从4.2这个值被载入记忆单元的那一刻起进行BP算法，直到从记忆单元中读出4.2的那一刻并随之将其清除为止。我们发现，根据记忆神经的线型本质，我们从读取点到写入点接收到的BP误差派生的变化完全可以忽略，因为通过所有时间层连接记忆单元的连接权值加权后约等于1。因此，我们可以在几百步中对这个误差派生值进行局部保存，而无需担心梯度的爆发或消失。
      LSTM RNN非常有效，而且同样适用于很多其他领域。我们早些时候讨论过，LSTM RNN的深层架构被用于实现了相当惊人的数据压缩率。（如果对LSTM RNN的特定应用有兴趣，可以查看这篇[论文](http://arxiv.org/pdf/1308.0850.pdf)做更深入的了解。）
**结论**
      有效进行神经网络训练的相关方法仍旧是一个活跃的研究领域，并引出了大量置换手段，目前尚无哪一种表现出明显的优势。LSTM RNN架构就是这样一个提高RNN训练的方法。还有一个方法就是使用更好的最优化控制器来对付梯度的爆炸与消失。Hessian-free优化器尝试通过小型梯度探测方向，甚至带来了更小的曲度，这使得它比单纯的梯度下降表现更佳。三分之一的方法涉及到权值谨慎的赋初值，期望借此避免头一个梯度爆发和消失的问题（例如：反射状态网络，基于动量的方法）。

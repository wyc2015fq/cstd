# 系统学习深度学习（四） --CNN原理，推导及实现源码分析 - 工作笔记 - CSDN博客





2017年01月12日 09:10:44[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：37100








之前看机器学习中，多层感知器部分，提到可以在设计多层感知器时，对NN的结构设计优化，例如结构化设计和权重共享，当时还没了解深度学习，现在看到CNN，原来CNN就是这方面的一个代表。CNN由纽约大学的Yann LeCun于1998年提出。CNN本质上是一个多层感知机，其成功的原因关键在于它所采用的局部连接和共享权值的方式，一方面减少了的权值的数量使得网络易于优化，另一方面降低了过拟合的风险。

下面内容可能有点重复啰嗦，主要是因为学习过程中， 总在不同文章看到一些新的，未知的东西，但同时又有很多重复。（最重要的是因为自己懒，，，）

另外：http://cs.stanford.edu/people/karpathy/convnetjs/demo/mnist.html CNN动态演示。

转自：http://blog.csdn.net/zouxy09/article/details/8781543/

1.Convolutional Neural Networks卷积神经网络

       卷积神经网络是人工神经网络的一种，已成为当前语音分析和图像识别领域的研究热点。它的权值共享网络结构使之更类似于生物神经网络，降低了网络模型的复杂度，减少了权值的数量。该优点在网络的输入是多维图像时表现的更为明显，使图像可以直接作为网络的输入，避免了传统识别[算法](http://lib.csdn.net/base/datastructure)中复杂的特征提取和数据重建过程。在二维图像处理上有众多优势，如网络能自行抽取图像特征包括颜色、纹理、形状及图像的拓扑结构；在处理二维图像问题上，特别是识别位移、缩放及其它形式扭曲不变性的应用上具有良好的鲁棒性和运算效率等。卷积网络是为识别二维形状而特殊设计的一个多层感知器，这种网络结构对平移、比例缩放、倾斜或者共他形式的变形具有高度不变性。

       CNNs是受早期的延时神经网络（TDNN）的影响。延时神经网络通过在时间维度上共享权值降低学习复杂度，适用于语音和时间序列信号的处理。

       CNNs是第一个真正成功训练多层网络结构的学习算法。它利用空间关系减少需要学习的参数数目以提高一般前向BP算法的训练性能。CNNs作为一个深度学习[架构](http://lib.csdn.net/base/architecture)提出是为了最小化数据的预处理要求。在CNN中，图像的一小部分（局部感受区域）作为层级结构的最低层的输入，信息再依次传输到不同的层，每层通过一个数字滤波器去获得观测数据的最显著的特征。这个方法能够获取对平移、缩放和旋转不变的观测数据的显著特征，因为图像的局部感受区域允许神经元或者处理单元可以访问到最基础的特征，例如定向边缘或者角点。

CNN具有一些传统技术所没有的优点：良好的容错能力、并行处理能力和自学习能力，可处理环境信息复杂，背景知识不清楚，推理规则不明确情况下的问题，允许样品有较大的缺损、畸变，运行速度快，自适应性能好，具有较高的分辨率。它是通过结构重组和减少权值将特征抽取功能融合进多层感知器，省略识别前复杂的图像特征抽取过程。

         CNN的泛化能力要显著优于其它方法，卷积神经网络已被应用于模式分类，物体检测和物体识别等方面。利用卷积神经网络建立模式分类器，将卷积神经网络作为通用的模式分类器，直接用于灰度图像。

         CNN是一个前溃式神经网络，能从一个二维图像中提取其拓扑结构，采用反向传播算法来优化网络结构，求解网络中的未知参数。

CNN是一类特别设计用来处理二维数据的多层神经网络。CNN被认为是第一个真正成功的采用多层层次结构网络的具有鲁棒性的[深度学习](http://lib.csdn.net/base/deeplearning)方法。CNN通过挖掘数据中的空间上的相关性，来减少网络中的可训练参数的数量，达到改进前向传播网络的反向传播算法效率，因为CNN需要非常少的数据预处理工作，所以也被认为是一种深度学习的方法。在CNN中，图像中的小块区域（也叫做“局部感知区域”）被当做层次结构中的底层的输入数据，信息通过前向传播经过网络中的各个层，在每一层中都由过滤器构成，以便能够获得观测数据的一些显著特征。因为局部感知区域能够获得一些基础的特征，比如图像中的边界和角落等，这种方法能够提供一定程度对位移、拉伸和旋转的相对不变性。

         CNN中层次之间的紧密联系和空间信息使得其特别适用于图像的处理和理解，并且能够自动的从图像抽取出丰富的相关特性。

         CNN通过结合局部感知区域、共享权重、空间或者时间上的降采样来充分利用数据本身包含的局部性等特征，优化网络结构，并且保证一定程度上的位移和变形的不变性。

CNN受视觉神经机制的启发而设计,是为识别二维或三维信号而设计的一个多层感知器,这种网络结构对平移、缩放、倾斜等变形具有高度不变性。

         CNN可以用来识别位移、缩放及其它形式扭曲不变性的二维或三维图像。CNN的特征提取层参数是通过训练数据学习得到的，所以其避免了人工特征提取，而是从训练数据中进行学习；其次同一特征图的神经元共享权值，减少了网络参数，这也是卷积网络相对于全连接网络的一大优势。共享局部权值这一特殊结构更接近于真实的生物神经网络使CNN在图像处理、语音识别领域有着独特的优越性，另一方面权值共享同时降低了网络的复杂性，且多维输入信号（语音、图像）可以直接输入网络的特点避免了特征提取和分类过程中数据重排的过程。

CNN是一种特殊的深层的神经网络模型，它的特殊性体现在两个方面，一方面它的神经元的连接是非全连接的，另一方面同一层中某些神经元之间的连接的权重是共享的（即相同的）。它的非全连接和权值共享的网络结构使之更类似于生物神经网络，降低了网络模型的复杂度（对于很难学习的深层结构来说，这是非常重要的），减少了权值的数量。

CNN是一种深度的监督学习下的[机器学习](http://lib.csdn.net/base/machinelearning)模型，具有极强的适应性，善于挖掘数据局部特征，提取全局训练特征和分类，它的权值共享结构网络使之更类似于生物神经网络，在模式识别各个领域都取得了很好的成果。

稀疏连接：在BP神经网络中，每一层的神经元节点是一个线性一维排列结构，层与层各神经元节点之间是全连接的。卷积神经网络中，层与层之间的神经元节点不再是全连接形式，利用层间局部空间相关性将相邻每一层的神经元节点只与和它相近的上层神经元节点连接，即局部连接。这样大大降低了神经网络[架构](http://lib.csdn.net/base/architecture)的参数规模。

权重共享：在卷积神经网络中，卷积层的每一个卷积滤波器重复的作用于整个感受野中，对输入图像进行卷积，卷积结果构成了输入图像的特征图，提取出图像的局部特征。每一个卷积滤波器共享相同的参数，包括相同的权重矩阵和偏置项。共享权重的好处是在对图像进行特征提取时不用考虑局部特征的位置。而且权重共享提供了一种有效的方式，使要学习的卷积神经网络模型参数数量大大降低。

最大池采样：它是一种非线性降采样方法。在通过卷积获取图像特征之后是利用这些特征进行分类。可以用所有提取到的特征数据进行分类器的训练，但这通常会产生极大的计算量。所以在获取图像的卷积特征后，要通过最大池采样方法对卷积特征进行降维。将卷积特征划分为数个n*n的不相交区域，用这些区域的最大(或平均)特征来表示降维后的卷积特征。这些降维后的特征更容易进行分类。

最大池采样在计算机视觉中的价值体现在两个方面：(1)、它减小了来自上层隐藏层的计算复杂度；(2)、这些池化单元具有平移不变性，即使图像有小的位移，提取到的特征依然会保持不变。由于增强了对位移的鲁棒性，最大池采样方法是一个高效的降低数据维度的采样方法。

Softmax回归：它是在逻辑回归的基础上扩张而来，它的目的是为了解决多分类问题。在这类问题中，训练样本的种类一般在两个以上。Softmax回归是有监督学习算法，它也可以与深度学习或无监督学习方法结合使用。

CNN是一种带有卷积结构的深度神经网络，通常至少有两个非线性可训练的卷积层，两个非线性的固定卷积层（又叫Pooling Laye）和一个全连接层，一共至少5个隐含层。

卷积神经网络中，输入就是一幅幅的图像，权值W就是卷积模板，一般是卷积层和下采样层交替，最后是全连接的神经网络。

局部区域感知能够发现数据的一些局部特征，比如图片上的一个角，一段弧，这些基本特征是构成动物视觉的基础。

CNN中每一层的由多个map组成，每个map由多个神经单元组成，同一个map的所有神经单元共用一个卷积核（即权重），卷积核往往代表一个特征，比如某个卷积核代表一段弧，那么把这个卷积核在整个图片上滚一下，卷积值较大的区域就很有可能是一段弧。注意卷积核其实就是权重，我们并不需要单独去计算一个卷积，而是一个固定大小的权重矩阵去图像上匹配时，这个操作与卷积类似，因此我们称为卷积神经网络，实际上，BP也可以看作一种特殊的卷积神经网络，只是这个卷积核就是某层的所有权重，即感知区域是整个图像。权重共享策略减少了需要训练的参数，使得训练出来的模型的泛华能力更强。

CNN一般采用卷积层与采样层交替设置，即一层卷积层接一层采样层，采样层后接一层卷积...这样卷积层提取出特征，再进行组合形成更抽象的特征，最后形成对图片对象的描述特征，CNN后面还可以跟全连接层，全连接层跟BP一样。

CNN的最大特点就是稀疏连接（局部感受）和权值共享。稀疏连接和权值共享可以减少所要训练的参数，减少计算复杂度。

卷积神经网络是一个多层的神经网络，每层由多个二维平面组成，而每个平面由多个独立神经元组成。

卷积网络在本质上是一种输入到输出的映射，它能够学习大量的输入与输出之间的映射关系，而不需要任何输入和输出之间的精确的数学表达式，只要用已知的模式对卷积网络加以训练，网络就具有输入输出对之间的映射能力。卷积网络执行的是有导师训练，所以其样本集是由形如：（输入向量，理想输出向量）的向量对构成的。所有这些向量对，都应该是来源于网络即将模拟的系统的实际“运行”结果。它们可以是从实际运行系统中采集来的。在开始训练前，所有的权都应该用一些不同的小随机数进行初始化。“小随机数”用来保证网络不会因权值过大而进入饱和状态，从而导致训练失败；“不同”用来保证网络可以正常地学习。实际上，如果用相同的数去初始化权矩阵，则网络无能力学习。训练算法与传统的BP算法差不多。

1）卷积神经网络的历史

       1962年Hubel和Wiesel通过对猫视觉皮层细胞的研究，提出了感受野(receptive field)的概念，1984年日本学者Fukushima基于感受野概念提出的神经认知机(neocognitron)可以看作是卷积神经网络的第一个实现网络，也是感受野概念在人工神经网络领域的首次应用。神经认知机将一个视觉模式分解成许多子模式（特征），然后进入分层递阶式相连的特征平面进行处理，它试图将视觉系统模型化，使其能够在即使物体有位移或轻微变形的时候，也能完成识别。

       通常神经认知机包含两类神经元，即承担特征抽取的S-元和抗变形的C-元。S-元中涉及两个重要参数，即感受野与阈值参数，前者确定输入连接的数目，后者则控制对特征子模式的反应程度。许多学者一直致力于提高神经认知机的性能的研究：在传统的神经认知机中，每个S-元的感光区中由C-元带来的视觉模糊量呈正态分布。如果感光区的边缘所产生的模糊效果要比中央来得大，S-元将会接受这种非正态模糊所导致的更大的变形容忍性。我们希望得到的是，训练模式与变形刺激模式在感受野的边缘与其中心所产生的效果之间的差异变得越来越大。为了有效地形成这种非正态模糊，Fukushima提出了带双C-元层的改进型神经认知机。

       Van Ooyen和Niehuis为提高神经认知机的区别能力引入了一个新的参数。事实上，该参数作为一种抑制信号，抑制了神经元对重复激励特征的激励。多数神经网络在权值中记忆训练信息。根据Hebb学习规则，某种特征训练的次数越多，在以后的识别过程中就越容易被检测。也有学者将进化计算理论与神经认知机结合，通过减弱对重复性激励特征的训练学习，而使得网络注意那些不同的特征以助于提高区分能力。上述都是神经认知机的发展过程，而卷积神经网络可看作是神经认知机的推广形式，神经认知机是卷积神经网络的一种特例。

2）卷积神经网络的网络结构

      卷积神经网络是一个多层的神经网络，每层由多个二维平面组成，而每个平面由多个独立神经元组成。

![](https://img-my.csdn.net/uploads/201304/10/1365562094_2229.jpg)

       图：卷积神经网络的概念示范：输入图像通过和三个可训练的滤波器和可加偏置进行卷积，滤波过程如图一，卷积后在C1层产生三个特征映射图，然后特征映射图中每组的四个像素再进行求和，加权值，加偏置，通过一个Sigmoid函数得到三个S2层的特征映射图。这些映射图再进过滤波得到C3层。这个层级结构再和S2一样产生S4。最终，这些像素值被光栅化，并连接成一个向量输入到传统的神经网络，得到输出。

       一般地，C层为特征提取层，每个神经元的输入与前一层的局部感受野相连，并提取该局部的特征，一旦该局部特征被提取后，它与其他特征间的位置关系也随之确定下来；S层是特征映射层，网络的每个计算层由多个特征映射组成，每个特征映射为一个平面，平面上所有神经元的权值相等。特征映射结构采用影响函数核小的sigmoid函数作为卷积网络的激活函数，使得特征映射具有位移不变性。

       此外，由于一个映射面上的神经元共享权值，因而减少了网络自由参数的个数，降低了网络参数选择的复杂度。卷积神经网络中的每一个特征提取层（C-层）都紧跟着一个用来求局部平均与二次提取的计算层（S-层），这种特有的两次特征提取结构使网络在识别时对输入样本有较高的畸变容忍能力。

3）关于参数减少与权值共享

      上面聊到，好像CNN一个牛逼的地方就在于通过感受野和权值共享减少了神经网络需要训练的参数的个数。那究竟是啥的呢？

       下图左：如果我们有1000x1000像素的图像，有1百万个隐层神经元，那么他们全连接的话（每个隐层神经元都连接图像的每一个像素点），就有1000x1000x1000000=10^12个连接，也就是10^12个权值参数。然而图像的空间联系是局部的，就像人是通过一个局部的感受野去感受外界图像一样，每一个神经元都不需要对全局图像做感受，每个神经元只感受局部的图像区域，然后在更高层，将这些感受不同局部的神经元综合起来就可以得到全局的信息了。这样，我们就可以减少连接的数目，也就是减少神经网络需要训练的权值参数的个数了。如下图右：假如局部感受野是10x10，隐层每个感受野只需要和这10x10的局部图像相连接，所以1百万个隐层神经元就只有一亿个连接，即10^8个参数。比原来减少了四个0（数量级），这样训练起来就没那么费力了，但还是感觉很多的啊，那还有啥办法没？

![](https://img-my.csdn.net/uploads/201304/10/1365562155_9356.jpg)

       我们知道，隐含层的每一个神经元都连接10x10个图像区域，也就是说每一个神经元存在10x10=100个连接权值参数。那如果我们每个神经元这100个参数是相同的呢？也就是说每个神经元用的是同一个卷积核去卷积图像。这样我们就只有多少个参数？？只有100个参数啊！！！亲！不管你隐层的神经元个数有多少，两层间的连接我只有100个参数啊！亲！这就是权值共享啊！亲！这就是卷积神经网络的主打卖点啊！亲！（有点烦了，呵呵）也许你会问，这样做靠谱吗？为什么可行呢？这个……共同学习。

       好了，你就会想，这样提取特征也忒不靠谱吧，这样你只提取了一种特征啊？对了，真聪明，我们需要提取多种特征对不？假如一种滤波器，也就是一种卷积核就是提出图像的一种特征，例如某个方向的边缘。那么我们需要提取不同的特征，怎么办，加多几种滤波器不就行了吗？对了。所以假设我们加到100种滤波器，每种滤波器的参数不一样，表示它提出输入图像的不同特征，例如不同的边缘。这样每种滤波器去卷积图像就得到对图像的不同特征的放映，我们称之为Feature Map。所以100种卷积核就有100个Feature Map。这100个Feature Map就组成了一层神经元。到这个时候明了了吧。我们这一层有多少个参数了？100种卷积核x每种卷积核共享100个参数=100x100=10K，也就是1万个参数。才1万个参数啊！亲！（又来了，受不了了！）见下图右：不同的颜色表达不同的滤波器。

![](https://img-my.csdn.net/uploads/201304/10/1365562217_2880.jpg)

       嘿哟，遗漏一个问题了。刚才说隐层的参数个数和隐层的神经元个数无关，只和滤波器的大小和滤波器种类的多少有关。那么隐层的神经元个数怎么确定呢？它和原图像，也就是输入的大小（神经元个数）、滤波器的大小和滤波器在图像中的滑动步长都有关！例如，我的图像是1000x1000像素，而滤波器大小是10x10，假设滤波器没有重叠，也就是步长为10，这样隐层的神经元个数就是(1000x1000 )/ (10x10)=100x100个神经元了，假设步长是8，也就是卷积核会重叠两个像素，那么……我就不算了，思想懂了就好。注意了，这只是一种滤波器，也就是一个Feature Map的神经元个数哦，如果100个Feature Map就是100倍了。由此可见，图像越大，神经元个数和需要训练的权值参数个数的贫富差距就越大。

![](https://img-my.csdn.net/uploads/201304/10/1365562245_8389.jpg)

      需要注意的一点是，上面的讨论都没有考虑每个神经元的偏置部分。所以权值个数需要加1 。这个也是同一种滤波器共享的。

      总之，卷积网络的核心思想是将：局部感受野、权值共享（或者权值复制）以及时间或空间亚采样这三种结构思想结合起来获得了某种程度的位移、尺度、形变不变性。

4）一个典型的例子说明

       一种典型的用来识别数字的卷积网络是LeNet-5（[效果和paper等见这](http://yann.lecun.com/exdb/lenet/index.html)）。当年美国大多数银行就是用它来识别支票上面的手写数字的。能够达到这种商用的地步，它的准确性可想而知。毕竟目前学术界和工业界的结合是最受争议的。

![](https://img-my.csdn.net/uploads/201304/10/1365562301_9590.jpg)

      那下面咱们也用这个例子来说明下。

![](https://img-my.csdn.net/uploads/201304/10/1365562321_2400.jpg)

        LeNet-5共有7层，不包含输入，每层都包含可训练参数（连接权重）。输入图像为32*32大小。这要比[Mnist数据库](http://yann.lecun.com/exdb/mnist/)（一个公认的手写[数据库](http://lib.csdn.net/base/mysql)）中最大的字母还大。这样做的原因是希望潜在的明显特征如笔画断电或角点能够出现在最高层特征监测子感受野的中心。

        我们先要明确一点：每个层有多个Feature Map，每个Feature Map通过一种卷积滤波器提取输入的一种特征，然后每个Feature Map有多个神经元。

        C1层是一个卷积层（为什么是卷积？卷积运算一个重要的特点就是，通过卷积运算，可以使原信号特征增强，并且降低噪音），由6个特征图Feature Map构成。特征图中每个神经元与输入中5*5的邻域相连。特征图的大小为28*28，这样能防止输入的连接掉到边界之外（是为了BP反馈时的计算，不致梯度损失，个人见解）。C1有156个可训练参数（每个滤波器5*5=25个unit参数和一个bias参数，一共6个滤波器，共(5*5+1)*6=156个参数），共156*(28*28)=122,304个连接。

       S2层是一个下采样层（为什么是下采样？利用图像局部相关性的原理，对图像进行子抽样，可以减少数据处理量同时保留有用信息），有6个14*14的特征图。特征图中的每个单元与C1中相对应特征图的2*2邻域相连接。S2层每个单元的4个输入相加，乘以一个可训练参数，再加上一个可训练偏置。结果通过sigmoid函数计算。可训练系数和偏置控制着sigmoid函数的非线性程度。如果系数比较小，那么运算近似于线性运算，亚采样相当于模糊图像。如果系数比较大，根据偏置的大小亚采样可以被看成是有噪声的“或”运算或者有噪声的“与”运算。每个单元的2*2感受野并不重叠，因此S2中每个特征图的大小是C1中特征图大小的1/4（行和列各1/2）。S2层有12个可训练参数和5880个连接。

![](https://img-my.csdn.net/uploads/201304/10/1365562371_3221.jpg)

图：卷积和子采样过程：卷积过程包括：用一个可训练的滤波器fx去卷积一个输入的图像（第一阶段是输入的图像，后面的阶段就是卷积特征map了），然后加一个偏置bx，得到卷积层Cx。子采样过程包括：每邻域四个像素求和变为一个像素，然后通过标量Wx+1加权，再增加偏置bx+1，然后通过一个sigmoid激活函数，产生一个大概缩小四倍的特征映射图Sx+1。

       所以从一个平面到下一个平面的映射可以看作是作卷积运算，S-层可看作是模糊滤波器，起到二次特征提取的作用。隐层与隐层之间空间分辨率递减，而每层所含的平面数递增，这样可用于检测更多的特征信息。

       C3层也是一个卷积层，它同样通过5x5的卷积核去卷积层S2，然后得到的特征map就只有10x10个神经元，但是它有16种不同的卷积核，所以就存在16个特征map了。这里需要注意的一点是：C3中的每个特征map是连接到S2中的所有6个或者几个特征map的，表示本层的特征map是上一层提取到的特征map的不同组合（这个做法也并不是唯一的）。（看到没有，这里是组合，就像之前聊到的人的视觉系统一样，底层的结构构成上层更抽象的结构，例如边缘构成形状或者目标的部分）。

       刚才说C3中每个特征图由S2中所有6个或者几个特征map组合而成。为什么不把S2中的每个特征图连接到每个C3的特征图呢？原因有2点。第一，不完全的连接机制将连接的数量保持在合理的范围内。第二，也是最重要的，其破坏了网络的对称性。由于不同的特征图有不同的输入，所以迫使他们抽取不同的特征（希望是互补的）。

      例如，存在的一个方式是：C3的前6个特征图以S2中3个相邻的特征图子集为输入。接下来6个特征图以S2中4个相邻特征图子集为输入。然后的3个以不相邻的4个特征图子集为输入。最后一个将S2中所有特征图为输入。这样C3层有1516个可训练参数和151600个连接。

       S4层是一个下采样层，由16个5*5大小的特征图构成。特征图中的每个单元与C3中相应特征图的2*2邻域相连接，跟C1和S2之间的连接一样。S4层有32个可训练参数（每个特征图1个因子和一个偏置）和2000个连接。

       C5层是一个卷积层，有120个特征图。每个单元与S4层的全部16个单元的5*5邻域相连。由于S4层特征图的大小也为5*5（同滤波器一样），故C5特征图的大小为1*1：这构成了S4和C5之间的全连接。之所以仍将C5标示为卷积层而非全相联层，是因为如果LeNet-5的输入变大，而其他的保持不变，那么此时特征图的维数就会比1*1大。C5层有48120个可训练连接。

        F6层有84个单元（之所以选这个数字的原因来自于输出层的设计），与C5层全相连。有10164个可训练参数。如同经典神经网络，F6层计算输入向量和权重向量之间的点积，再加上一个偏置。然后将其传递给sigmoid函数产生单元i的一个状态。

      最后，输出层由欧式径向基函数（Euclidean Radial Basis Function）单元组成，每类一个单元，每个有84个输入。换句话说，每个输出RBF单元计算输入向量和参数向量之间的欧式距离。输入离参数向量越远，RBF输出的越大。一个RBF输出可以被理解为衡量输入模式和与RBF相关联类的一个模型的匹配程度的惩罚项。用概率术语来说，RBF输出可以被理解为F6层配置空间的高斯分布的负log-likelihood。给定一个输入模式，损失函数应能使得F6的配置与RBF参数向量（即模式的期望分类）足够接近。这些单元的参数是人工选取并保持固定的（至少初始时候如此）。这些参数向量的成分被设为-1或1。虽然这些参数可以以-1和1等概率的方式任选，或者构成一个纠错码，但是被设计成一个相应字符类的7*12大小（即84）的格式化图片。这种表示对识别单独的数字不是很有用，但是对识别可打印ASCII集中的字符串很有用。

      使用这种分布编码而非更常用的“1 of N”编码用于产生输出的另一个原因是，当类别比较大的时候，非分布编码的效果比较差。原因是大多数时间非分布编码的输出必须为0。这使得用sigmoid单元很难实现。另一个原因是分类器不仅用于识别字母，也用于拒绝非字母。使用分布编码的RBF更适合该目标。因为与sigmoid不同，他们在输入空间的较好限制的区域内兴奋，而非典型模式更容易落到外边。

        RBF参数向量起着F6层目标向量的角色。需要指出这些向量的成分是+1或-1，这正好在F6 sigmoid的范围内，因此可以防止sigmoid函数饱和。实际上，+1和-1是sigmoid函数的最大弯曲的点处。这使得F6单元运行在最大非线性范围内。必须避免sigmoid函数的饱和，因为这将会导致损失函数较慢的收敛和病态问题。

5）训练过程

        神经网络用于模式识别的主流是有指导学习网络，无指导学习网络更多的是用于聚类分析。对于有指导的模式识别，由于任一样本的类别是已知的，样本在空间的分布不再是依据其自然分布倾向来划分，而是要根据同类样本在空间的分布及不同类样本之间的分离程度找一种适当的空间划分方法，或者找到一个分类边界，使得不同类样本分别位于不同的区域内。这就需要一个长时间且复杂的学习过程，不断调整用以划分样本空间的分类边界的位置，使尽可能少的样本被划分到非同类区域中。

       卷积网络在本质上是一种输入到输出的映射，它能够学习大量的输入与输出之间的映射关系，而不需要任何输入和输出之间的精确的数学表达式，只要用已知的模式对卷积网络加以训练，网络就具有输入输出对之间的映射能力。卷积网络执行的是有导师训练，所以其样本集是由形如：（输入向量，理想输出向量）的向量对构成的。所有这些向量对，都应该是来源于网络即将模拟的系统的实际“运行”结果。它们可以是从实际运行系统中采集来的。在开始训练前，所有的权都应该用一些不同的小随机数进行初始化。“小随机数”用来保证网络不会因权值过大而进入饱和状态，从而导致训练失败；“不同”用来保证网络可以正常地学习。实际上，如果用相同的数去初始化权矩阵，则网络无能力学习。

       训练算法与传统的BP算法差不多。主要包括4步，这4步被分为两个阶段：

第一阶段，向前传播阶段：

a）从样本集中取一个样本(X,Yp)，将X输入网络；

b）计算相应的实际输出Op。

      在此阶段，信息从输入层经过逐级的变换，传送到输出层。这个过程也是网络在完成训练后正常运行时执行的过程。在此过程中，网络执行的是计算（实际上就是输入与每层的权值矩阵相点乘，得到最后的输出结果）：

          Op=Fn（…（F2（F1（XpW（1））W（2））…）W（n））

第二阶段，向后传播阶段

a）算实际输出Op与相应的理想输出Yp的差；

b）按极小化误差的方法反向传播调整权矩阵。

6）卷积神经网络的优点

        卷积神经网络CNN主要用来识别位移、缩放及其他形式扭曲不变性的二维图形。由于CNN的特征检测层通过训练数据进行学习，所以在使用CNN时，避免了显式的特征抽取，而隐式地从训练数据中进行学习；再者由于同一特征映射面上的神经元权值相同，所以网络可以并行学习，这也是卷积网络相对于神经元彼此相连网络的一大优势。卷积神经网络以其局部权值共享的特殊结构在语音识别和图像处理方面有着独特的优越性，其布局更接近于实际的生物神经网络，权值共享降低了网络的复杂性，特别是多维输入向量的图像可以直接输入网络这一特点避免了特征提取和分类过程中数据重建的复杂度。

        流的分类方式几乎都是基于统计特征的，这就意味着在进行分辨前必须提取某些特征。然而，显式的特征提取并不容易，在一些应用问题中也并非总是可靠的。卷积神经网络，它避免了显式的特征取样，隐式地从训练数据中进行学习。这使得卷积神经网络明显有别于其他基于神经网络的分类器，通过结构重组和减少权值将特征提取功能融合进多层感知器。它可以直接处理灰度图片，能够直接用于处理基于图像的分类。

       卷积网络较一般神经网络在图像处理方面有如下优点： a）输入图像和网络的拓扑结构能很好的吻合；b）特征提取和模式分类同时进行，并同时在训练中产生；c）权重共享可以减少网络的训练参数，使神经网络结构变得更简单，适应性更强。

**下面是推导和实现**

         这个文档讨论的是CNNs的推导和实现。CNN[架构](http://lib.csdn.net/base/architecture)的连接比权值要多很多，这实际上就隐含着实现了某种形式的规则化。这种特别的网络假定了我们希望通过数据驱动的方式学习到一些滤波器，作为提取输入的特征的一种方法。

         本文中，我们先对训练全连接网络的经典BP[算法](http://lib.csdn.net/base/datastructure)做一个描述，然后推导2D CNN网络的卷积层和子采样层的BP权值更新方法。在推导过程中，我们更强调实现的效率，所以会给出一些Matlab代码。最后，我们转向讨论如何自动地学习组合前一层的特征maps，特别地，我们还学习特征maps的稀疏组合。

一、全连接的反向传播算法

         典型的CNN中，开始几层都是卷积和下采样的交替，然后在最后一些层（靠近输出层的），都是全连接的一维网络。这时候我们已经将所有两维2D的特征maps转化为全连接的一维网络的输入。这样，当你准备好将最终的2D特征maps输入到1D网络中时，一个非常方便的方法就是把所有输出的特征maps连接成一个长的输入向量。然后我们回到BP算法的讨论。（更详细的基础推导可以参考UFLDL中“[反向传导算法](http://deeplearning.stanford.edu/wiki/index.php/%E5%8F%8D%E5%90%91%E4%BC%A0%E5%AF%BC%E7%AE%97%E6%B3%95)”）。

2.1、Feedforward Pass前向传播

         在下面的推导中，我们采用平方误差代价函数。我们讨论的是多类问题，共c类，共N个训练样本。

![](https://img-blog.csdn.net/20130816002706875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

         这里![](https://img-blog.csdn.net/20130816002717187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表示第n个样本对应的标签的第k维。![](https://img-blog.csdn.net/20130816002727875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表示第n个样本对应的网络输出的第k个输出。对于多类问题，输出一般组织为“one-of-c”的形式，也就是只有该输入对应的类的输出节点输出为正，其他类的位或者节点为0或者负数，这个取决于你输出层的激活函数。sigmoid就是0，tanh就是-1.

         因为在全部训练集上的误差只是每个训练样本的误差的总和，所以这里我们先考虑对于一个样本的BP。对于第n个样本的误差，表示为：

![](https://img-blog.csdn.net/20130816002745906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       传统的全连接神经网络中，我们需要根据BP规则计算代价函数E关于网络每一个权值的偏导数。我们用l来表示当前层，那么当前层的输出可以表示为：

![](https://img-blog.csdn.net/20130816002804250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       输出激活函数f(.)可以有很多种，一般是sigmoid函数或者双曲线正切函数。sigmoid将输出压缩到[0, 1]，所以最后的输出平均值一般趋于0 。所以如果将我们的训练数据归一化为零均值和方差为1，可以在梯度下降的过程中增加收敛性。对于归一化的数据集来说，双曲线正切函数也是不错的选择。

2.2、Backpropagation Pass反向传播

         反向传播回来的误差可以看做是每个神经元的基的灵敏度sensitivities（灵敏度的意思就是我们的基b变化多少，误差会变化多少，也就是误差对基的变化率，也就是导数了），定义如下：（第二个等号是根据求导的链式法则得到的）

![](https://img-blog.csdn.net/20130816002850203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

         因为∂u/∂b=1，所以∂E/∂b=∂E/∂u=δ，也就是说bias基的灵敏度∂E/∂b=δ和误差E对一个节点全部输入u的导数∂E/∂u是相等的。这个导数就是让高层误差反向传播到底层的神来之笔。反向传播就是用下面这条关系式：（下面这条式子表达的就是第l层的灵敏度，就是）

![](https://img-blog.csdn.net/20130816002937765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)公式（1）

         这里的“◦”表示每个元素相乘。输出层的神经元的灵敏度是不一样的：

![](https://img-blog.csdn.net/20130816002950875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

         最后，对每个神经元运用delta（即δ）规则进行权值更新。具体来说就是，对一个给定的神经元，得到它的输入，然后用这个神经元的delta（即δ）来进行缩放。用向量的形式表述就是，对于第l层，误差对于该层每一个权值（组合为矩阵）的导数是该层的输入（等于上一层的输出）与该层的灵敏度（该层每个神经元的δ组合成一个向量的形式）的叉乘。然后得到的偏导数乘以一个负学习率就是该层的神经元的权值的更新了：

![](https://img-blog.csdn.net/20130816003002437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)公式（2）

         对于bias基的更新表达式差不多。实际上，对于每一个权值(W)ij都有一个特定的学习率ηIj。

二、Convolutional Neural Networks 卷积神经网络

2.1、Convolution Layers 卷积层

         我们现在关注网络中卷积层的BP更新。在一个卷积层，上一层的特征maps被一个可学习的卷积核进行卷积，然后通过一个激活函数，就可以得到输出特征map。每一个输出map可能是组合卷积多个输入maps的值：

![](https://img-blog.csdn.net/20130816003036562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       这里Mj表示选择的输入maps的集合，那么到底选择哪些输入maps呢？有选择一对的或者三个的。但下面我们会讨论如何去自动选择需要组合的特征maps。每一个输出map会给一个额外的偏置b，但是对于一个特定的输出map，卷积每个输入maps的卷积核是不一样的。也就是说，如果输出特征map j和输出特征map k都是从输入map i中卷积求和得到，那么对应的卷积核是不一样的。

2.1.1、Computing the Gradients梯度计算

         我们假定每个卷积层l都会接一个下采样层l+1 。对于BP来说，根据上文我们知道，要想求得层l的每个神经元对应的权值的权值更新，就需要先求层l的每一个神经节点的灵敏度δ（也就是权值更新的公式（2））。为了求这个灵敏度我们就需要先对下一层的节点（连接到当前层l的感兴趣节点的第l+1层的节点）的灵敏度求和（得到δl+1），然后乘以这些连接对应的权值（连接第l层感兴趣节点和第l+1层节点的权值）W。再乘以当前层l的该神经元节点的输入u的激活函数f的导数值（也就是那个灵敏度反向传播的公式（1）的δl的求解），这样就可以得到当前层l每个神经节点对应的灵敏度δl了。

      然而，因为下采样的存在，采样层的一个像素（神经元节点）对应的灵敏度δ对应于卷积层（上一层）的输出map的一块像素（采样窗口大小）。因此，层l中的一个map的每个节点只与l+1层中相应map的一个节点连接。

     为了有效计算层l的灵敏度，我们需要上采样upsample 这个下采样downsample层对应的灵敏度map（特征map中每个像素对应一个灵敏度，所以也组成一个map），这样才使得这个灵敏度map大小与卷积层的map大小一致，然后再将层l的map的激活值的偏导数与从第l+1层的上采样得到的灵敏度map逐元素相乘（也就是公式（1））。

        在下采样层map的权值都取一个相同值β，而且是一个常数。所以我们只需要将上一个步骤得到的结果乘以一个β就可以完成第l层灵敏度δ的计算。

       我们可以对卷积层中每一个特征map j重复相同的计算过程。但很明显需要匹配相应的子采样层的map（参考公式（1））：

![](https://img-blog.csdn.net/20130816003129515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

        up(.)表示一个上采样操作。如果下采样的采样因子是n的话，它简单的将每个像素水平和垂直方向上拷贝n次。这样就可以恢复原来的大小了。实际上，这个函数可以用Kronecker乘积来实现：

![](https://img-blog.csdn.net/20130816003155078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       好，到这里，对于一个给定的map，我们就可以计算得到其灵敏度map了。然后我们就可以通过简单的对层l中的灵敏度map中所有节点进行求和快速的计算bias基的梯度了：

![](https://img-blog.csdn.net/20130816003207265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)公式（3）

       最后，对卷积核的权值的梯度就可以用BP算法来计算了（公式（2））。另外，很多连接的权值是共享的，因此，对于一个给定的权值，我们需要对所有与该权值有联系（权值共享的连接）的连接对该点求梯度，然后对这些梯度进行求和，就像上面对bias基的梯度计算一样：

![](https://img-blog.csdn.net/20130816003232171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       这里，![](https://img-blog.csdn.net/20130816003313265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是![](https://img-blog.csdn.net/20130816003318046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)中的在卷积的时候与![](https://img-blog.csdn.net/20130816003328953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)逐元素相乘的patch，输出卷积map的(u, v)位置的值是由上一层的(u, v)位置的patch与卷积核k_ij逐元素相乘的结果。

      咋一看，好像我们需要煞费苦心地记住输出map（和对应的灵敏度map）每个像素对应于输入map的哪个patch。但实际上，在Matlab中，可以通过一个代码就实现。对于上面的公式，可以用Matlab的卷积函数来实现：

![](https://img-blog.csdn.net/20130816003352281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       我们先对delta灵敏度map进行旋转，这样就可以进行互相关计算，而不是卷积（在卷积的数学定义中，特征矩阵（卷积核）在传递给conv2时需要先翻转（flipped）一下。也就是颠倒下特征矩阵的行和列）。然后把输出反旋转回来，这样我们在前向传播进行卷积的时候，卷积核才是我们想要的方向。



3.2、Sub-sampling Layers 子采样层

         对于子采样层来说，有N个输入maps，就有N个输出maps，只是每个输出map都变小了。

![](https://img-blog.csdn.net/20130816003432812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

        down(.)表示一个下采样函数。典型的操作一般是对输入图像的不同nxn的块的所有像素进行求和。这样输出图像在两个维度上都缩小了n倍。每个输出map都对应一个属于自己的乘性偏置β和一个加性偏置b。



3.2.1、Computing the Gradients 梯度计算

         这里最困难的是计算灵敏度map。一旦我们得到这个了，那我们唯一需要更新的偏置参数β和b就可以轻而易举了（公式（3））。如果下一个卷积层与这个子采样层是全连接的，那么就可以通过BP来计算子采样层的灵敏度maps。

         我们需要计算卷积核的梯度，所以我们必须找到输入map中哪个patch对应输出map的哪个像素。这里，就是必须找到当前层的灵敏度map中哪个patch对应与下一层的灵敏度map的给定像素，这样才可以利用公式（1）那样的δ递推，也就是灵敏度反向传播回来。另外，需要乘以输入patch与输出像素之间连接的权值，这个权值实际上就是卷积核的权值（已旋转的）。

![](https://img-blog.csdn.net/20130816003501218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

      在这之前，我们需要先将核旋转一下，让卷积函数可以实施互相关计算。另外，我们需要对卷积边界进行处理，但在Matlab里面，就比较容易处理。Matlab中全卷积会对缺少的输入像素补0 。

      到这里，我们就可以对b和β计算梯度了。首先，加性基b的计算和上面卷积层的一样，对灵敏度map中所有元素加起来就可以了：

![](https://img-blog.csdn.net/20130816003539109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

       而对于乘性偏置β，因为涉及到了在前向传播过程中下采样map的计算，所以我们最好在前向的过程中保存好这些maps，这样在反向的计算中就不用重新计算了。我们定义：

![](https://img-blog.csdn.net/20130816003553468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这样，对β的梯度就可以用下面的方式计算：

![](https://img-blog.csdn.net/20130816003605718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



2.3、Learning Combinations of Feature Maps 学习特征map的组合

         大部分时候，通过卷积多个输入maps，然后再对这些卷积值求和得到一个输出map，这样的效果往往是比较好的。在一些文献中，一般是人工选择哪些输入maps去组合得到一个输出map。但我们这里尝试去让CNN在训练的过程中学习这些组合，也就是让网络自己学习挑选哪些输入maps来计算得到输出map才是最好的。我们用αij表示在得到第j个输出map的其中第i个输入map的权值或者贡献。这样，第j个输出map可以表示为：

![](https://img-blog.csdn.net/20130816003621765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

         需要满足约束：

![](https://img-blog.csdn.net/20130816003641828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

         这些对变量αij的约束可以通过将变量αij表示为一个组无约束的隐含权值cij的softmax函数来加强。（因为softmax的因变量是自变量的指数函数，他们的变化率会不同）。

![](https://img-blog.csdn.net/20130816003652625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

         因为对于一个固定的j来说，每组权值cij都是和其他组的权值独立的，所以为了方面描述，我们把下标j去掉，只考虑一个map的更新，其他map的更新是一样的过程，只是map的索引j不同而已。

         Softmax函数的导数表示为：

![](https://img-blog.csdn.net/20130816003718375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

        这里的δ是Kronecker delta。对于误差对于第l层变量αi的导数为：

![](https://img-blog.csdn.net/20130816003726703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

         最后就可以通过链式规则去求得代价函数关于权值ci的偏导数了：

![](https://img-blog.csdn.net/20130816003743953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



2.3.1、Enforcing Sparse Combinations 加强稀疏性组合

         为了限制αi是稀疏的，也就是限制一个输出map只与某些而不是全部的输入maps相连。我们在整体代价函数里增加稀疏约束项Ω(α)。对于单个样本，重写代价函数为：

![](https://img-blog.csdn.net/20130816003835515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后寻找这个规则化约束项对权值ci求导的贡献。规则化项Ω(α)对αi求导是：

![](https://img-blog.csdn.net/20130816003848406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

         然后，通过链式法则，对ci的求导是：

![](https://img-blog.csdn.net/20130816003914937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

         所以，权值ci最后的梯度是：

![](https://img-blog.csdn.net/20130816003929468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**CNN的源码实现：**

转自：[http://blog.csdn.net/tostq/article/details/51786265](http://blog.csdn.net/tostq/article/details/51786265)蓝色部分是我的注释。

卷积神经网络是深度学习的基础，但是学习CNN却不是那么简单，虽然网络上关于CNN的相关代码很多，比较经典的是tiny_cnn（C++）、DeepLearnToolbox（Matlab）等等，但通过C语言来编写CNN的却比较少，本人因为想在多核DSP下运行CNN，所以便尝试通过C语言来编写，主要参考的代码是DeepLearnToolbox的内容，DeepLearnToolbox是用Matlab脚本编写，是我看过的最为简单的CNN代码，代码清晰，阅读方便，非常适合新手入门学习。

       本文的CNN代码是一个最基本的卷积网络，主要用于手写数字的识别，选择的训练测试是数据库是Minst手写数字库，主要是包括了一个基本的多层卷积网络框架、卷积层、Pooling层、及全连接的单层神经网络输出层，不过CNN其他重要的概念如Dropout、ReLu等暂时没有涉及，但是个人对于新手，学习卷积网络的基本结构及其误差反向传播方法是完全足够。工程用代码，可以参考各个主流深度学习开源框架。

       这里要注意的是，本文的方法并不是深度学习之父Yann. LeCun在1998年就已经提出的成熟算法LeNet-5卷积网络，而只是DeepLearnToolbox内的cnn代码的c语言实现，不过我们会比较二者之间的区别，因为二者的基本原理是相似的。另外，为了不使博客篇幅过长，所以博客中贴的代码并不完整，完整代码请见附件。

       这篇博客总共分为四节：

       第一节：前言，介绍项目结构及Minst数据集测试训练数据集

       第二节：主要介绍CNN的网络结构、相关数据结构

       第三节：重点介绍CNN学习训练过程的误差反向传播方法，采用的是在线训练方式

       第四节：CNN的学习及测试结果的比较



**论文参考文献：**

Y. LeCun, L. Bottou, Y. Bengio and P. Haffner: Gradient-Based Learning Applied to Document Recognition, Proceedings of the IEEE, 86(11):2278-2324, November 1998



**一、代码结构**

       本文的CNN代码是通过标准C编写，也不需要调用任何三方库文件，附件共享的文件是通过VS2010编译的项目文件（这里虽然是.cpp文件，但实际上完全是用C编写的，直接改成.c文件是完全可以使用的），当然也可以直接将相关的源文件导入到其他IDE上，也是能够运行的。

       文件结构：

       cnn.cpp cnn.h 存在关于CNN网络的函数、网络结构等

       minst.cpp minst.h 处理Minst数据库的函数、数据结构等

       mat.cpp mat.h 一些关于矩阵的函数，如卷积操作、180度翻转操作等

       main.cpp 主函数和测试函数


 项目代码地址：[https://github.com/tostq/DeepLearningC/tree/master/CNN](https://github.com/tostq/DeepLearningC/tree/master/CNN)



**二、MINST数据库**

       MINST数据库是由Yann提供的手写数字数据库文件，其官方下载地址[http://yann.lecun.com/exdb/mnist/](http://yann.lecun.com/exdb/mnist/)

       这个里面还包含了对这个数据库进行识别的各类算法的结果比较及相关算法的论文

![](https://img-blog.csdn.net/20160629220713526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       数据库的里的图像都是28*28大小的灰度图像，每个像素的是一个八位字节（0~255）

![](https://img-blog.csdn.net/20160629220759683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       这个数据库主要包含了60000张的训练图像和10000张的测试图像，主要是下面的四个文件

![](https://img-blog.csdn.net/20160629220853418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       上述四个文件直接解压就可以使用了，虽然数据未压缩，但是我们还是需要将图像提取出来，方便我们进行操作

（1）存储图像数据的相关数据结构：

       单张图像结构及保存图像的链表

![](https://img-blog.csdn.net/20160629220914480?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       图像实际数字标号（0~9），数据库里是用一个八位字节来表示，不过为了方便学习，需要用10位来表示。

       这里的10位表示网络输出层的10个神经元，某位为1表示数字标号即为该位。

![](https://img-blog.csdn.net/20160629221001084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（2）读入图像数据的相关函数

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
ImgArr read_Img(const char* filename) // 读入图像
{
    FILE  *fp=NULL;
    fp=fopen(filename,"rb");
    if(fp==NULL)
        printf("open file failed\n");
    assert(fp);

    int magic_number = 0;  
    int number_of_images = 0;  
    int n_rows = 0;  
    int n_cols = 0;  
    //从文件中读取sizeof(magic_number) 个字符到 &magic_number  
    fread((char*)&magic_number,sizeof(magic_number),1,fp); 
    magic_number = ReverseInt(magic_number);  
    //获取训练或测试image的个数number_of_images 
    fread((char*)&number_of_images,sizeof(number_of_images),1,fp);  
    number_of_images = ReverseInt(number_of_images);    
    //获取训练或测试图像的高度Heigh  
    fread((char*)&n_rows,sizeof(n_rows),1,fp); 
    n_rows = ReverseInt(n_rows);                  
    //获取训练或测试图像的宽度Width  
    fread((char*)&n_cols,sizeof(n_cols),1,fp); 
    n_cols = ReverseInt(n_cols);  
    //获取第i幅图像，保存到vec中 
    int i,r,c;

    // 图像数组的初始化
    ImgArr imgarr=(ImgArr)malloc(sizeof(MinstImgArr));
    imgarr->ImgNum=number_of_images;
    imgarr->ImgPtr=(MinstImg*)malloc(number_of_images*sizeof(MinstImg));

    for(i = 0; i < number_of_images; ++i)  
    {  
        imgarr->ImgPtr[i].r=n_rows;
        imgarr->ImgPtr[i].c=n_cols;
        imgarr->ImgPtr[i].ImgData=(float**)malloc(n_rows*sizeof(float*));
        for(r = 0; r < n_rows; ++r)      
        {
            imgarr->ImgPtr[i].ImgData[r]=(float*)malloc(n_cols*sizeof(float));
            for(c = 0; c < n_cols; ++c)
            { 
                // 因为神经网络用float型计算更为精确，这里我们将图像像素转为浮点型
                unsigned char temp = 0;  
                fread((char*) &temp, sizeof(temp),1,fp); 
                imgarr->ImgPtr[i].ImgData[r][c]=(float)temp/255.0;
            }  
        }    
    }

    fclose(fp);
    return imgarr;
}
```



（3）读入图像数据标号

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
LabelArr read_Lable(const char* filename)// 读入图像
{
    FILE  *fp=NULL;
    fp=fopen(filename,"rb");
    if(fp==NULL)
        printf("open file failed\n");
    assert(fp);

    int magic_number = 0;  
    int number_of_labels = 0; 
    int label_long = 10;

    //从文件中读取sizeof(magic_number) 个字符到 &magic_number  
    fread((char*)&magic_number,sizeof(magic_number),1,fp); 
    magic_number = ReverseInt(magic_number);  
    //获取训练或测试image的个数number_of_images 
    fread((char*)&number_of_labels,sizeof(number_of_labels),1,fp);  
    number_of_labels = ReverseInt(number_of_labels);    

    int i,l;

    // 图像标记数组的初始化
    LabelArr labarr=(LabelArr)malloc(sizeof(MinstLabelArr));
    labarr->LabelNum=number_of_labels;
    labarr->LabelPtr=(MinstLabel*)malloc(number_of_labels*sizeof(MinstLabel));

    for(i = 0; i < number_of_labels; ++i)  
    {
        // 数据库内的图像标记是一位，这里将图像标记变成10位，10位中只有唯一一位为1，为1位即是图像标记  
        labarr->LabelPtr[i].l=10;
        labarr->LabelPtr[i].LabelData=(float*)calloc(label_long,sizeof(float));
        unsigned char temp = 0;  
        fread((char*) &temp, sizeof(temp),1,fp); 
        labarr->LabelPtr[i].LabelData[(int)temp]=1.0;    
    }

    fclose(fp);
    return labarr;    
}
```

上一节我们总体介绍项目并说明Minst手写数字数据库的使用，这一节我们将重点介绍CNN网络总体结构。

![](https://img-blog.csdn.net/20160629221535623?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      上图我们已经非常熟悉，其为Yann在1998年介绍的LeNet-5网络的结构，其刚被提出，就在学术和工业领域上得到广泛应用，而本文的CNN卷积网络却是如下图所示（博主自己画的，画这个图还是挺麻烦的：L，不清晰请原谅），和LeNet-5相比主要有以下三点不同：

      （1）LeNet-5给输入图像增加了一圈黑边，使输入图像大小变成了32x32，这样的目的是为了在下层卷积过程中保留更多原图的信息。

      （2）LeNet-5的卷积层C3只有16个模板，得到16个输出，而本文的卷积层C3由于是全连接，所以有6*12个模板，得到12个输出图像

      （3）LeNet-5多了两种，分别是C5到F6的全连接神经网络层，和F6到OUTPUT高斯连接网络层。而本文的直接由采样层S4直接经过一层全连接神经网络层到OUTPUT。

      下面我们将重点介绍各层的结构及数据的前向传播。

![](https://img-blog.csdn.net/20160629221649031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**一、各层的解释**

（1）卷积层C1

      输入为28x28的灰度图像，灰度图像分别同6个5x5的模板进行卷积操作，分别得到了6个24x24的卷积图像，图像里的每个像素加上一个权重，并经过一个激活函数，得到该层的输出。

      所以该层的相关参数为：6个5x5的模板参数w，6个模板对应的权重参数b，共6x5x5+6个参数

**Tips:**

      关于激活函数：激活函数我们在学习神经网络时就已经接触过了，其主要有两个目的，第一是将数据钳制在一定范围内（如Sigmoid函数将数据压缩在-1到1之间），不太高也不太低，第二是用来加入非线性因素的，因为线性模型的表达能力不够。传统神经网络中最常用的两个激活函数Sigmoid系和Tanh系，而Sigmoid系（Logistic-Sigmoid、Tanh-Sigmoid）被视为神经网络的核心所在。本文的例子就是Sigmoid系。

![](https://img-blog.csdn.net/20160629221801438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

近年来，在深度学习领域中效果最好，应用更为广泛的是ReLu激活函数，其相较于Sigmoid系，主要变化有三点：①单侧抑制 ②相对宽阔的兴奋边界 ③稀疏激活性。特别是在神经科学方面，除了新的激活频率函数之外，神经科学家还发现了的稀疏激活性广泛存在于大脑的神经元，神经元编码工作方式具有稀疏性和分布性。大脑同时被激活的神经元只有1~4%。 从信号方面来看，即神经元同时只对输入信号的少部分选择性响应，大量信号被刻意的屏蔽了，这样可以提高学习的精度，更好更快地提取稀疏特征。而在经验规则的初始化W之后，传统的Sigmoid系函数同时近乎有一半的神经元被激活，这不符合神经科学的研究，而且会给深度网络训练带来巨大问题。Softplus照顾到了新模型的前两点，却没有稀疏激活性。因而，校正函数max(0,x)即ReLu函数成了最大赢家。

![](https://img-blog.csdn.net/20160629221824720?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（2）采样层S2及S4（Pooling层）

      采样层S又名Pooling层，Pooling主要是为了减少数据处理的维度，常见的pooling方法有max pooling和average pooling等。

      max pooling 就是选择当前块内最大像素值来表示当前局部块

      average pooling 就是选择当前块的像素值平均值来代替

      本文的选择Pooling方法是average pooling，而使用广泛效果较好的方法却是max pooling。（看到这里，你可能会吐槽，为什么不用效果好，因为平均计算相比而言，有那么一丢丢简单！）

（3）卷积层C3

      这里的卷积层是一个全连接的卷积层。输出的卷积公式如下，这里I表示图像，W表示卷积模板，b表示偏重，φ表示激活函数，i表示输入图像序号（i=1~6），j表示该层输出图像序号（j=1~12）

![](https://img-blog.csdn.net/20160629221938126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      由此可以看到在卷积层C3中输入为6个12x12的图像，输出为12个8x8的图像

      所需要训练的参数有6x12个5x5的卷积模板w和12个偏重b（每个模板对应的偏重都是相同的）

      而实际上由于神经网络的稀疏结构和减少训练时间的需要，该卷积层一般不是利用全连接的，就比如前面介绍LeNet-5网络，只需要利用16个卷积模板就可以了，而不是全连接的6x12个，其连接方法如下，其最终得到16个输出图像。

![](https://img-blog.csdn.net/20160629222010503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      这里X表示选择卷积，比如第0张输出图像是由第0、1、2张输入图像分别同第0个卷积模板卷积相加，再加上偏重，经过激活函数得到的。而第15张图像是由第0、1、2、3、4、5张输入图像分别同第15个卷积模板卷积相加得到的。

（4）输出层O5：

      采样层S4后，我们将得到12张4*4的图像，将所有图像展开成一维，就得到了12*4*4=192位的向量。

输出层是由输入192位，输出10位的全连接单层神经网络，共有10个神经元构成，每个神经元都同192位输入相连，即都有192位的输入和1位输出，其处理公式如下，这里j表示输出神经元的序号，i表示输入的序号。

![](https://img-blog.csdn.net/20160629222040144?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      所以该层参数共有192*10个权重w，和10个偏重b



**二、卷积神经网络的相关数据结构**

      这个卷积网络主要有五层网络，主要结构是卷积层、采样层（Pooling）、卷积层、采样层（Pooling）和全连接的单层神经网络层（输出层），所以我们建立了三个基本层的结构及一个总的卷积网络结构。

      这里结构内除了必要的权重参数，而需要记录该层输入输出数据y，及需要传递到下一层的局部梯度d。

（1）卷积层

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
// 卷积层
typedef struct convolutional_layer{
    int inputWidth;   //输入图像的宽
    int inputHeight;  //输入图像的长
    int mapSize;      //特征模板的大小，模板一般都是正方形

    int inChannels;   //输入图像的数目
    int outChannels;  //输出图像的数目

    // 关于特征模板的权重分布，这里是一个四维数组
    // 其大小为inChannels*outChannels*mapSize*mapSize大小
    // 这里用四维数组，主要是为了表现全连接的形式，实际上卷积层并没有用到全连接的形式
    // 这里的例子是DeapLearningToolboox里的CNN例子，其用到就是全连接
    float**** mapData;     //存放特征模块的数据
    float**** dmapData;    //存放特征模块的数据的局部梯度

    float* basicData;   //偏置，偏置的大小，为outChannels
    bool isFullConnect; //是否为全连接
    bool* connectModel; //连接模式（默认为全连接）

    // 下面三者的大小同输出的维度相同
    float*** v; // 进入激活函数的输入值
    float*** y; // 激活函数后神经元的输出

    // 输出像素的局部梯度
    float*** d; // 网络的局部梯度,δ值  
}CovLayer;
```



（2）采样层

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
// 采样层 pooling
typedef struct pooling_layer{
    int inputWidth;   //输入图像的宽
    int inputHeight;  //输入图像的长
    int mapSize;      //特征模板的大小

    int inChannels;   //输入图像的数目
    int outChannels;  //输出图像的数目

    int poolType;     //Pooling的方法
    float* basicData;   //偏置

    float*** y; // 采样函数后神经元的输出,无激活函数
    float*** d; // 网络的局部梯度,δ值
}PoolLayer;
```



（3）全连接的单层神经网络

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
// 输出层 全连接的神经网络
typedef struct nn_layer{
    int inputNum;   //输入数据的数目
    int outputNum;  //输出数据的数目

    float** wData; // 权重数据，为一个inputNum*outputNum大小
    float* basicData;   //偏置，大小为outputNum大小

    // 下面三者的大小同输出的维度相同
    float* v; // 进入激活函数的输入值
    float* y; // 激活函数后神经元的输出
    float* d; // 网络的局部梯度,δ值

    bool isFullConnect; //是否为全连接
}OutLayer;
```



（4）各层共同组成一个完整的卷积网络

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
typedef struct cnn_network{
    int layerNum;
    CovLayer* C1;
    PoolLayer* S2;
    CovLayer* C3;
    PoolLayer* S4;
    OutLayer* O5;

    float* e; // 训练误差
    float* L; // 瞬时误差能量
}CNN;
```



（5）另外还有一个用于存放训练参量的结构

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
typedef struct train_opts{
    int numepochs; // 训练的迭代次数
    float alpha; // 学习速率
}CNNOpts;
```



**三、卷积神经网络的初始化**

       卷积神经网络的初始化主要包含了各数据的空间初始化及权重的随机赋值，没有什么复杂，按照结构分配空间就可以了，这里不再详细赘述了，可以直接参考代码内cnnsetup()函数



**四、卷积神经网络的前向传播过程**

       前向传播过程实际上就是指输入图像数据，得到输出结果的过程，而后向传播过程就是将输出结果的误差由后向前传递给各层，各层依次调整权重的过程。所以前向传播过程相比而是比较直观，而且简单的。

前向传播过程在项目中主要是由cnnff函数完成，下面我们将按层介绍其过程

（1）卷积层C1

       卷积层C1共有6个卷积模板，每个模板同输入图像卷积将会得到一个输出，即共6个输出，以下是图像的卷积公式：

![](https://img-blog.csdn.net/20160629222351784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       C1层的相关代码，这里cov函数是卷积函数，在mat.cpp是具体的定义，activation_Sigma是激活函数

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
int outSizeW=cnn->S2->inputWidth;
    int outSizeH=cnn->S2->inputHeight;
    // 第一层的传播
    int i,j,r,c;
    // 第一层输出数据
    nSize mapSize={cnn->C1->mapSize,cnn->C1->mapSize};
    nSize inSize={cnn->C1->inputWidth,cnn->C1->inputHeight};
    nSize outSize={cnn->S2->inputWidth,cnn->S2->inputHeight};
    for(i=0;i<(cnn->C1->outChannels);i++){
        for(j=0;j<(cnn->C1->inChannels);j++){
            float** mapout=cov(cnn->C1->mapData[j][i],mapSize,inputData,inSize,valid);
            addmat(cnn->C1->v[i],cnn->C1->v[i],outSize,mapout,outSize);
            for(r=0;r<outSize.r;r++)
                free(mapout[r]);
            free(mapout);
        }
        for(r=0;r<outSize.r;r++)
            for(c=0;c<outSize.c;c++)
                cnn->C1->y[i][r][c]=activation_Sigma(cnn->C1->v[i][r][c],cnn->C1->basicData[i]);
    }
```

（2）采样层S2，avgPooling是平均Pooling函数

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
// 第二层的输出传播S2，采样层
    outSize.c=cnn->C3->inputWidth;
    outSize.r=cnn->C3->inputHeight;
    inSize.c=cnn->S2->inputWidth;
    inSize.r=cnn->S2->inputHeight;
    for(i=0;i<(cnn->S2->outChannels);i++){
        if(cnn->S2->poolType==AvePool)
            avgPooling(cnn->S2->y[i],outSize,cnn->C1->y[i],inSize,cnn->S2->mapSize);
    }
```



（3）卷积层C3，同C1很类似   

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
// 第三层输出传播,这里是全连接
    outSize.c=cnn->S4->inputWidth;
    outSize.r=cnn->S4->inputHeight;
    inSize.c=cnn->C3->inputWidth;
    inSize.r=cnn->C3->inputHeight;
    mapSize.c=cnn->C3->mapSize;
    mapSize.r=cnn->C3->mapSize;
    for(i=0;i<(cnn->C3->outChannels);i++){
        for(j=0;j<(cnn->C3->inChannels);j++){
            float** mapout=cov(cnn->C3->mapData[j][i],mapSize,cnn->S2->y[j],inSize,valid);
            addmat(cnn->C3->v[i],cnn->C3->v[i],outSize,mapout,outSize);
            for(r=0;r<outSize.r;r++)
                free(mapout[r]);
            free(mapout);
        }
        for(r=0;r<outSize.r;r++)
            for(c=0;c<outSize.c;c++)
                cnn->C3->y[i][r][c]=activation_Sigma(cnn->C3->v[i][r][c],cnn->C3->basicData[i]);
    }
```

（4）采样层S4，同S2很类似 

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
// 第四层的输出传播
    inSize.c=cnn->S4->inputWidth;
    inSize.r=cnn->S4->inputHeight;
    outSize.c=inSize.c/cnn->S4->mapSize;
    outSize.r=inSize.r/cnn->S4->mapSize;
    for(i=0;i<(cnn->S4->outChannels);i++){
        if(cnn->S4->poolType==AvePool)
            avgPooling(cnn->S4->y[i],outSize,cnn->C3->y[i],inSize,cnn->S4->mapSize);
    }
```

（5）输出层O5

**[cpp]**[view plain](http://blog.csdn.net/tostq/article/details/51786315#)[copy](http://blog.csdn.net/tostq/article/details/51786315#)[print](http://blog.csdn.net/tostq/article/details/51786315#)[?](http://blog.csdn.net/tostq/article/details/51786315#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
-     // 输出层O5的处理  
-     // 首先需要将前面的多维输出展开成一维向量  
-     float* O5inData=(float*)malloc((cnn->O5->inputNum)*sizeof(float));   
-     for(i=0;i<(cnn->S4->outChannels);i++)  
-         for(r=0;r<outSize.r;r++)  
-             for(c=0;c<outSize.c;c++)  
-                 O5inData[i*outSize.r*outSize.c+r*outSize.c+c]=cnn->S4->y[i][r][c];  
- 
-     nSize nnSize={cnn->O5->inputNum,cnn->O5->outputNum};  
-     nnff(cnn->O5->v,O5inData,cnn->O5->wData,cnn->O5->basicData,nnSize);  
-     for(i=0;i<cnn->O5->outputNum;i++)  
-         cnn->O5->y[i]=activation_Sigma(cnn->O5->v[i],cnn->O5->basicData[i]);  
-     free(O5inData);  
- }  ﻿﻿

上一节我们介绍了卷积神经网络的前向传播过程，这一节我们重点介绍反向传播过程，反向传播过程反映神经网络的学习训练过程。

误差反向传播方法是神经网络学习的基础，网络上已经有许多相关的内容了，不过关于卷积网络的误差反向传递的公式推导却比较少，而且也不是很清晰，本文将会详细推导这个过程，虽然内容很复杂，但却值得学习.

       首先我们需要知道的是误差反向传播的学习方法，实际是梯度下降法求最小误差的权重过程。当然我们的目的是求误差能量关于参数（权重）的导数.

       梯度下降法更新权重公式如下所示：

![](https://img-blog.csdn.net/20160630101738046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       这里W表示权重，E表示误差能量，n表示第n轮更新迭代，η表示学习参数，Y表示输出，δ表示局域梯度。

       而另一方面误差能量关于参数（权重）的导数同当前层输入是相关的，所以我们需要一个更好地将当前层误差传递给下一层的量，因为这个δ同当前层的输出无关，其只是反映了当前层的固定结构，所以我们可以将这个固有性质δ反向传递给下一层，其定义为：

![](https://img-blog.csdn.net/20160630101800497?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       接下来我们分层分析整个网络的反向传播过程。在本文的卷积神经网络中主要有以下四种情况：



**一、输出层（单层神经网络层）**

       （1）误差能量定义为实际输出与理想输出的误差

       这里的d是理想预期输出，y指实际输出，i指输出位，本文的网络输出为10位，所以N=10.

![](https://img-blog.csdn.net/20160630101830156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       （2）误差能量关于参数（权重）的导数。

       这一层是比较简单的

![](https://img-blog.csdn.net/20160630101854356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       由于本文是采用Sigmoid系数的激活函数，所以其导数可以求出为：

![](https://img-blog.csdn.net/20160630102023112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       其局域梯度δ表示为：

![](https://img-blog.csdn.net/20160630102041113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**二、后接输出层的采样层S4**

       后接输出层的采样层向多层感知器的隐藏神经元的反向传播是类似的。

       由于这一层没有权重，所以不需要进行权重更新，但是我们也需要将误差能量传递给下一层，所以需要计算局域梯度δ，其定义如下，这里j指输出图像中的像素序号，S4层共有12*4*4=192个输出像素，所以j=1~192。

![](https://img-blog.csdn.net/20160630102125973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       另外输出层O5的局域梯度δ也已经计算过了：

![](https://img-blog.csdn.net/20160630102209459?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       由于采样层没有激活函数，所以φ的导数为1，则最终可以得到

![](https://img-blog.csdn.net/20160630102232030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       通过上式，我们就可以算出由输出层O5传递到S4层的局域梯度δ值。可以看出传递到采样层各输出像素j的局域梯度δ值，实际是相当于与其相连的下层输出的局域梯度δ值乘上相连权重的总和。



**三、后接采样层的卷积层C1、C3**

       前面为了方便计算，S4层和O5层的输出都被展开成了一维，所以像素都是以i和j作为标号的，到了C3层往前，我们以像素的坐标m(x，y)来标号，m(x，y)表示第m张输出模板的(x,y)位置的像素。局域梯度δ值定义为：

![](https://img-blog.csdn.net/20160630102302342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       传递到该像素的误差能量等于所有与其相连的像素误差能量和，这里的i指的m(x,y)采样邻域Θ内的所有像素

![](https://img-blog.csdn.net/20160630102323171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       因为本文采用的是平均Pooling方法，S4的输出就是该像素邻域内的所有像素的平均值，这里的S指邻域Θ内的所有像素的总数，本文采用的是2*2的采样块，所以S=4。

![](https://img-blog.csdn.net/20160630102341273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（1）因此由S4传递到C3层的局域梯度δ值为：

![](https://img-blog.csdn.net/20160630102358102?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       接下来我们依据局域梯度δ值，来计算C3层的权重更新值。

（2）C3层的权重更新值。

       C3层共有6*12个5*5的模板，我们首先定义n=1~6,m=1~12表示模板的标号，s,t表示模板中参数的位置

![](https://img-blog.csdn.net/20160630102421587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（3）C1层的权重更新公式和局域梯度δ值

       同理，我们也可以得到C1层的权重更新公式，这里的M=6,N=1，而y是指输入图像

![](https://img-blog.csdn.net/20160630102520603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**四、后接卷积层的采样层S2**

       这里的n为当前S2层的输出图像序号（n=1~6），n为当前C3层的输出图像序号（m=1~12）。

![](https://img-blog.csdn.net/20160630102547745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       因此第n块图像的局域梯度δ值为

![](https://img-blog.csdn.net/20160630102609187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**五、误差反向传播过程的代码展示**

**[cpp]**[view plain](http://blog.csdn.net/tostq/article/details/51788093#)[copy](http://blog.csdn.net/tostq/article/details/51788093#)[print](http://blog.csdn.net/tostq/article/details/51788093#)[?](http://blog.csdn.net/tostq/article/details/51788093#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- void cnnbp(CNN* cnn,float* outputData) // 网络的后向传播  
- {  
-     int i,j,c,r; // 将误差保存到网络中  
-     for(i=0;i<cnn->O5->outputNum;i++)  
-         cnn->e[i]=cnn->O5->y[i]-outputData[i];  
- 
-     /*从后向前反向计算*/  
-     // 输出层O5  
-     for(i=0;i<cnn->O5->outputNum;i++)  
-         cnn->O5->d[i]=cnn->e[i]*sigma_derivation(cnn->O5->y[i]);  
- 
-     // S4层，传递到S4层的误差  
-     // 这里没有激活函数  
-     nSize outSize={cnn->S4->inputWidth/cnn->S4->mapSize,cnn->S4->inputHeight/cnn->S4->mapSize};  
-     for(i=0;i<cnn->S4->outChannels;i++)  
-         for(r=0;r<outSize.r;r++)  
-             for(c=0;c<outSize.c;c++)  
-                 for(j=0;j<cnn->O5->outputNum;j++){  
-                     int wInt=i*outSize.c*outSize.r+r*outSize.c+c;  
-                     cnn->S4->d[i][r][c]=cnn->S4->d[i][r][c]+cnn->O5->d[j]*cnn->O5->wData[j][wInt];  
-                 }  
- 
-     // C3层  
-     // 由S4层传递的各反向误差,这里只是在S4的梯度上扩充一倍  
-     int mapdata=cnn->S4->mapSize;  
-     nSize S4dSize={cnn->S4->inputWidth/cnn->S4->mapSize,cnn->S4->inputHeight/cnn->S4->mapSize};  
-     // 这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化  
-     for(i=0;i<cnn->C3->outChannels;i++){  
-         float** C3e=UpSample(cnn->S4->d[i],S4dSize,cnn->S4->mapSize,cnn->S4->mapSize);  
-         for(r=0;r<cnn->S4->inputHeight;r++)  
-             for(c=0;c<cnn->S4->inputWidth;c++)  
-                 cnn->C3->d[i][r][c]=C3e[r][c]*sigma_derivation(cnn->C3->y[i][r][c])/(float)(cnn->S4->mapSize*cnn->S4->mapSize);  
-         for(r=0;r<cnn->S4->inputHeight;r++)  
-             free(C3e[r]);  
-         free(C3e);  
-     }  
- 
-     // S2层，S2层没有激活函数，这里只有卷积层有激活函数部分  
-     // 由卷积层传递给采样层的误差梯度，这里卷积层共有6*12个卷积模板  
-     outSize.c=cnn->C3->inputWidth;  
-     outSize.r=cnn->C3->inputHeight;  
-     nSize inSize={cnn->S4->inputWidth,cnn->S4->inputHeight};  
-     nSize mapSize={cnn->C3->mapSize,cnn->C3->mapSize};  
-     for(i=0;i<cnn->S2->outChannels;i++){  
-         for(j=0;j<cnn->C3->outChannels;j++){  
-             float** corr=correlation(cnn->C3->mapData[i][j],mapSize,cnn->C3->d[j],inSize,full);  
-             addmat(cnn->S2->d[i],cnn->S2->d[i],outSize,corr,outSize);  
-             for(r=0;r<outSize.r;r++)  
-                 free(corr[r]);  
-             free(corr);  
-         }  
-         /* 
-         for(r=0;r<cnn->C3->inputHeight;r++) 
-             for(c=0;c<cnn->C3->inputWidth;c++) 
-                 // 这里本来用于采样的激活 
-         */  
-     }  
- 
-     // C1层，卷积层  
-     mapdata=cnn->S2->mapSize;  
-     nSize S2dSize={cnn->S2->inputWidth/cnn->S2->mapSize,cnn->S2->inputHeight/cnn->S2->mapSize};  
-     // 这里的Pooling是求平均，所以反向传递到下一神经元的误差梯度没有变化  
-     for(i=0;i<cnn->C1->outChannels;i++){  
-         float** C1e=UpSample(cnn->S2->d[i],S2dSize,cnn->S2->mapSize,cnn->S2->mapSize);  
-         for(r=0;r<cnn->S2->inputHeight;r++)  
-             for(c=0;c<cnn->S2->inputWidth;c++)  
-                 cnn->C1->d[i][r][c]=C1e[r][c]*sigma_derivation(cnn->C1->y[i][r][c])/(float)(cnn->S2->mapSize*cnn->S2->mapSize);  
-         for(r=0;r<cnn->S2->inputHeight;r++)  
-             free(C1e[r]);  
-         free(C1e);  
-     }      
- }  

**Tips:**

      关于在线学习和批量学习

**      批量学习方法：**突触权值的调整在训练样本集合的所有N个例子都出现后进行，这个N个例子构成训练的一个回合，每个回合的训练样本的样例都是随机选择的，而权值的调整是靠所有N个例子的总体平均来实现。

批量学习方法的优点在于能够精确估计当前梯度向量（即代价函数对权值向量W的导数），因此，保证能最快下降到局部极小点的收敛性，另外能满足并行计算的要求，当前也增加存储压力。

**      在线学习方法：**即样例是一个接一个输入网络中，只有当前一样本输入完成权值调整后，后一样本才能进入网络。在线学习方法相比于批量学习方法，其更容易执行，而且也不易陷入局部极值点，另外比批量学习需要更少的存储空间。

本文这个版本的CNN采用是在线学习方法，DeepLearnToolbox的版本默认是采用的是批量学习，可以将批量数改为1也就成了在线学习方法。



      前面几节我们重点介绍CNN的结构和学习方法，这一节我们观察这个网络的实验结果。

      首先前面也说了，虽然称C语言版本的CNN，但博主为了方便调试还是用了VS2010来编写。



**一、CNN的学习过程及误差曲线**

      这里要提前说明三点：

      1、学习过程的要花费很长的时间，笔者的电脑跑了两个多小时，DeepLearnToolbox的Matlab版本大概只要20分钟左右，这主要是由于Matlab并行运算的缘故。

      2、程序对于内存空间要求很大，虽然博主尽量仔细处理分配的空间，但还是需要2G多内存才能完成整个学习过程，另外在程序运行前，最好设置一个较大的堆。

      3、为了方便测试与观察结果，博主将程序产生相关数据保存起来，放到了Matlab内观察。

      最终结果：本文的C版本同DeepLearnToolbox的Matlab版本的训练结果几乎是一致的。训练误差图如下所示，可以看到最终的误差大概都在0.02左右。

![](https://img-blog.csdn.net/20160630211545389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**二、CNN权重的保存**

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
void savecnn(CNN* cnn, const char* filename)
{
    FILE  *fp=NULL;
    fp=fopen(filename,"wb");
    if(fp==NULL)
        printf("write file failed\n");

    int i,j,r;
    // C1的数据
    for(i=0;i<cnn->C1->inChannels;i++)
        for(j=0;j<cnn->C1->outChannels;j++)
            for(r=0;r<cnn->C1->mapSize;r++)
                fwrite(cnn->C1->mapData[i][j][r],sizeof(float),cnn->C1->mapSize,fp);

    fwrite(cnn->C1->basicData,sizeof(float),cnn->C1->outChannels,fp);

    // C3网络
    for(i=0;i<cnn->C3->inChannels;i++)
        for(j=0;j<cnn->C3->outChannels;j++)
            for(r=0;r<cnn->C3->mapSize;r++)
                fwrite(cnn->C3->mapData[i][j][r],sizeof(float),cnn->C3->mapSize,fp);

    fwrite(cnn->C3->basicData,sizeof(float),cnn->C3->outChannels,fp);

    // O5输出层
    for(i=0;i<cnn->O5->outputNum;i++)
        fwrite(cnn->O5->wData[i],sizeof(float),cnn->O5->inputNum,fp);
    fwrite(cnn->O5->basicData,sizeof(float),cnn->O5->outputNum,fp);

    fclose(fp);
}
```



**三、测试正确识别率**

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
int vecmaxIndex(float* vec, int veclength)// 返回向量最大数的序号
{
    int i;
    float maxnum=-1.0;
    int maxIndex=0;
    for(i=0;i<veclength;i++){
        if(maxnum<vec[i]){
            maxnum=vec[i];
            maxIndex=i;
        }
    }
    return maxIndex;
}

// 测试cnn函数
float cnntest(CNN* cnn, ImgArr inputData,LabelArr outputData,int testNum)
{
    int n=0;
    int incorrectnum=0;  //错误预测的数目
    for(n=0;n<testNum;n++){
        cnnff(cnn,inputData->ImgPtr[n].ImgData);
        if(vecmaxIndex(cnn->O5->y,cnn->O5->outputNum)!=vecmaxIndex(outputData->LabelPtr[n].LabelData,cnn->O5->outputNum))
            incorrectnum++;
        cnnclear(cnn);
    }
    return (float)incorrectnum/(float)testNum;
}
```



      经过测试函数，本文的最终错误识别率为3%左右



五、卷积神经网络注意事项

1.        数据集的大小和分块

数据驱动的模型一般依赖于数据集的大小，CNN和其它经验模型一样，能适用于任意大小的数据集，但用于训练的数据集应该足够大，能够覆盖问题域中所有已知可能出现的问题。设计CNN的时候，数据集中应该包含三个子集：训练集、[测试](http://lib.csdn.net/base/softwaretest)集、验证集。训练集应该包含问题域中的所有数据，并在训练阶段用来调整网络权值。测试集用来在训练过程中测试网络对于训练集中未出现的数据的分类性能。根据网络在测试集上的性能情况，网络的结构可能需要做出调整，或者增加训练循环的次数。验证集中的数据同样应该包含在测试集合训练集中没有出现过的数据，用于在确定网络结构后能够更加好的测试和衡量网络的性能。Looney等人建议，数据集中的65%用于训练，25%用于测试，剩余的10%用于验证。

2.      数据预处理

为了加速训练算法的收敛速度，一般都会采用一些数据预处理技术，这其中包括：去除噪声、输入数据降维、删除无关数据等。数据的平衡化在分类问题中异常重要，一般认为训练集中的数据应该相对于标签类别近似于平均分布，也就是每一个类别标签所对应的数据量在训练集中是基本相等的，以避免网络过于倾向于表现某些分类的特点。为了平衡数据集，应该移除一些过度富余的分类中的数据，并相应的补充一些相对样例稀少的分类中的数据。还有一个办法就是复制一部分这些样例稀少分类中的数据，并在这些输入数据中加入随机噪声。

3.       数据规则化

将数据规则化到一个统一的区间（如[0,1]）中具有很重要的优点：防止数据中存在较大数值的数据造成数值较小的数据对于训练效果减弱甚至无效化。一个常用的方法是将输入和输出数据按比例调整到一个和激活函数（sigmoid函数等）相对应的区间。

4.       网络权值初始化

CNN的初始化主要是初始化卷积层和输出层的卷积核（权重）和偏置。

网络权值初始化就是将网络中的所有连接权值（包括阈值）赋予一个初始值。如果初始权值向量处在误差曲面的一个相对平缓的区域的时候，网络训练的收敛速度可能会异常缓慢。一般情况下，网络的连接权值和阈值被初始化在一个具有0均值的相对小的区间内均匀分布，比如[-0.30, +0.30]这样的区间内。

5.       BP算法的学习速率

如果学习速率n选取的比较大则会在训练过程中较大幅度的调整权值w，从而加快网络训练的速度，但这会造成网络在误差曲面上搜索过程中频繁抖动且有可能使得训练过程不能收敛，而且可能越过一些接近优化w。同样，比较小的学习速率能够稳定的使得网络逼近于全局最优点，但也有可能陷入一些局部最优区域。对于不同的学习速率设定都有各自的优缺点，而且还有一种自适应的学习速率方法，即n随着训练算法的运行过程而自行调整。

6.       收敛条件

有几个条件可以作为停止训练的判定条件，训练误差、误差梯度和交叉验证。一般来说，训练集的误差会随着网络训练的进行而逐步降低。

7.       训练方式

训练样例可以有两种基本的方式提供给网络训练使用，也可以是两者的结合：逐个样例训练(EET)、批量样例训练(BT)。在EET中，先将第一个样例提供给网络，然后开始应用BP算法训练网络，直到训练误差降低到一个可以接受的范围，或者进行了指定步骤的训练次数。然后再将第二个样例提供给网络训练。EET的优点是相对于BT只需要很少的存储空间，并且有更好的随机搜索能力，防止训练过程陷入局部最小区域。EET的缺点是如果网络接收到的第一个样例就是劣质（有可能是噪音数据或者特征不明显）的数据，可能使得网络训练过程朝着全局误差最小化的反方向进行搜索。相对的，BT方法是在所有训练样例都经过网络传播后才更新一次权值，因此每一次学习周期就包含了所有的训练样例数据。BT方法的缺点也很明显，需要大量的存储空间，而且相比EET更容易陷入局部最小区域。而随机训练（ST）则是相对于EET和BT一种折衷的方法，ST和EET一样也是一次只接受一个训练样例，但只进行一次BP算法并更新权值，然后接受下一个样例重复同样的步骤计算并更新权值，并且在接受训练集最后一个样例后，重新回到第一个样例进行计算。ST和EET相比，保留了随机搜索的能力，同时又避免了训练样例中最开始几个样例如果出现劣质数据对训练过程的过度不良影响。﻿﻿

﻿﻿

通道特征计算：

对于单通道图像，若利用10个卷积核进行卷积计算，可以得到10个特征图；若输入为多通道图像，则输出特征图的个数依然是卷积核的个数（10个）。

1.单通道多个卷积核卷积计算

![](http://i.imgur.com/tO5fMwT.png)

一个卷积核得到的特征提取是不充分的，我们可以添加多个卷积核，比如32个卷积核，从而可以学习32种特征。

2.多通道多个卷积核卷积计算

图片：假设图片的宽度为width:W，高度为height:H，图片的通道数为D，一般目前都用RGB三通道D=3，为了通用性，通道数用D表示；

卷积核：卷积核大小为K*K，由于处理的图片是D通道的，因此卷积核其实也就是K*K*D大小的，因此，对于RGB三通道图像，在指定kernel_size的前提下，真正的卷积核大小是kernel_size*kernel_size*3。

对于D通道图像的各通道而言，是在每个通道上分别执行二维卷积，然后将D个通道加起来，得到该位置的二维卷积输出，对于RGB三通道图像而言，就是在R，G，B三个通道上分别使用对应的每个通道上的kernel_size*kernel_size大小的核去卷积每个通道上的W*H的图像，然后将三个通道卷积得到的输出相加，得到二维卷积输出结果。因此，若有M个卷积核，可得到M个二维卷积输出结果，在有padding的情况下，能保持输出图片大小和原来的一样，因此是output(W,H,M)。

![](http://i.imgur.com/ilEPEvZ.jpg)

下面的图动态形象地展示了三通道图像卷积层的计算过程：

![](http://i.imgur.com/j8kLBKs.gif)



﻿﻿

﻿﻿

﻿﻿




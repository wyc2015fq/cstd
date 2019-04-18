# ANN:神经网络堆叠/进化故事（ 从感知机到DRBN ） - wishchinYang的专栏 - CSDN博客
2015年04月16日 10:36:59[wishchin](https://me.csdn.net/wishchin)阅读数：4427
**前言：   **
        几乎每一次神经网络的再流行，都会出现：推进人工智能的梦想之说。不过感觉，神经网络的成功是对人的已有经验的覆盖。自然状态是一个DFA，而总结规律的过程则是根据经验的正确性把几何状态转化为抽象代数运算，这就是规则。ANN以我们难以理解的网络参数来表示规则，是个不明所以的黑箱。
        DNN的流行因其多层模型，以其强大的表征能力可拟合形状诡异的样本流形分布，可以使用多层少量参数的组合来代替浅层网络的海量参数。
        本文有大量修改，如有疑问，请拜访原文， 原文链接：[http://www.cnblogs.com/xiaowanyer/p/3701944.html](http://www.cnblogs.com/xiaowanyer/p/3701944.html)
        关于DNN的对比介绍：[最接近人脑的智能学习器-深度学习及并行化实现](http://www.52ml.net/17069.html)
        另外参考：[DL深度学习原理与实现](http://www.chinakdd.com/article-5l34674Q821C41J.html)    DL原理步进：[http://blog.csdn.net/zouxy09/article/details/8781396](http://blog.csdn.net/zouxy09/article/details/8781396)
**目录：**
1.感知器：线性分类器
2.多层网：前馈神经网络
3.解决方案：逐层初始化—自编码器
4.随机可视化：全连接玻尔兹曼机
5.自学习：深度信念网络
**一. 感知器       **
       智慧分解为元素是识别和决策，代表人对环境的输入和输出。而识别和决策可以坍塌为一种逻辑表示：分类，即感知器对输入的分类和触发器对思维结果的分类。其中最简单的是两类问题，即布尔逻辑运算问题。
## 感知机结构
　　感知机是监督式训练算法，是神经网络构建的基础。神经网络的学习过程最终是发生在神经元之间的突触部位，突触的联结强度随着突触前后神经元的活动而变化，变化的量与两个神经元的活性之和成正比。 1957年，Frank Rosenblatt用数学的方法描述这个过程，即为感知器模型。    
       此处参考资料：[神经网络从被人忽悠好忽悠人](http://www.cnblogs.com/GarfieldEr007/p/5517387.html)
![](https://img-blog.csdn.net/20171010160324522?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171010101228429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       数学模型：
![](https://img-blog.csdn.net/20171010160700572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**应用场景例子分解**：
      假如平面中存在 *n *个点，并被分别标记为“0”和“1”。此时加入一个新的点，如果我们想知道这个点的标记是什么（和之前提到的小狗图片的辨别同理），我们要怎么做呢？
　　一种很简单的方法是查找离这个点最近的点是什么，然后返回和这个点一样的标记。而一种稍微“智能”的办法则是去找出平面上的一条线来将不同标记的数据点分开，并用这条线作为“分类器”来区分新数据点的标记。
![线性分类器](http://images.cnitblog.com/i/629033/201404/301954151118120.png)
　　在本例中，每一个输入数据都可以表示为一个向量 **x** = (*x_1, x_2*) ，而我们的函数则是要实现“如果线以下，输出0；线以上，输出1”。这归结为一个简单的线性分类问题。
**数学描述**：用数学方法表示，定义一个表示权重的向量 **w** 和一个垂直偏移量 *b*。然后，我们将输入、权重和偏移结合可以得到如下传递函数：
![映射变换函数](http://images.cnitblog.com/i/629033/201404/302002326425318.png)
                或者离散描述这种形式：
![](https://img-blog.csdn.net/20171010101101895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
　　这个传递函数的结果将被输入到一个激活函数中以产生标记。在上面的例子中，我们的**激活函数**是一个门限截止函数（即大于某个阈值后输出1）：
![](http://images.cnitblog.com/i/629033/201404/302016242201737.png)
　　整个步骤连接起来就是一个感知器模型，由此实现线性分类。 
**模型识别过程代码实现：**
```cpp
bool slp_calculate_output( const double * inputs, const double * weights, int nInputs,int & output )  
{  
    if( NULL ==inputs || NULL == weights )  
        return false;  
    //1.权值相加，累计
    double sum =0.0;  
    for (int i = 0 ; i < nInputs ; ++i){  
        sum += (weights[i] * inputs[i] );  
    }  
    //2.激活函数：这里我们对乘积和的处理：如果大于0，则输出值为1；其他情况，输出值为-1  
    if(sum >0.0)  
        output = 1;  
    else  
        output = -1;  
    return true;
}
```
简单模型代码：感知器模型实现了一个简单的感知器算法，此外可以生成一个感知器结构，用以规整化输入输出，对应结构图中的描述。
### 学习/训练过程
　　感知机的训练包括多训练样本的输入及计算每个样本的输出。在每一次计算以后，权重 *w* 都要调整以最小化输出误差，这个误差由输入样本的标记值与实际计算得出值的差得出。还有其它的误差计算方法，如[均方差](http://zh.wikipedia.org/wiki/%E5%9D%87%E6%96%B9%E5%B7%AE)等，但基本的原则是一样的。
      感知器学习算法步骤如下：
- 对权系数置初值。
- 输入一样(X1…Xn)本以及它的期望输出d。
期望输出值d在样本的类属不同时取值不同。如果是A类，则取d＝1,如果是B类，则取-1。期望输出d也即是教师信号。
- 计算实际输出值。
![](https://img-blog.csdn.net/20171010160700572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 根据实际输出求误差e。
 E = d-Y(t)
- 用误差e去修改权系数。
![](https://img-blog.csdn.net/20171010161227043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 转到第2点，一直执行到一切样本均稳定为止。
### 学习有效性
        模型的可收敛性能：通过上述的学习算法，模型一定收敛到一个稳定模型吗？
        此处参考：[Rosenblatt感知器详解](http://www.cnblogs.com/lanix/p/5003521.html)      详细看原文章，下面转载两个图，感知器算法模型训练收敛性证明的上界和下界。
![](https://img-blog.csdn.net/20171010162054976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171010162101302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                        上界证明过程                                                                                         下界证明过程
### 感知器算法缺陷
　　这种简单的感知机有一个明显缺陷：只能对线性可分样本集合分类。
       这个缺陷重要吗？比如 XOR，这么简单的函数，都不能被线性分类器分类（如下图所示，分隔两类点失败）：
![](http://images.cnitblog.com/i/629033/201404/302026267526714.png)
       为了解决这个问题，我们要使用一种多层感知机，也就是——多层前馈神经网络：事实上，我们将要组合一群这样的感知机来创建出一个更强大的学习机器。多层分段线性函数的组合可以模拟任意复杂的非线性函数，也即使理论上多层感知机网络可以 对任意分类面复杂的 样本集合进行正确分类。
## 二. 多层前馈神经网络
　　神经网络实际上就是将大量之前讲到的感知机进行组合，用不同的方法进行连接并作用在不同的激活函数上。
![前馈神经网络示意图](http://images.cnitblog.com/i/629033/201404/302034370173392.png)
　　我们简单介绍下前向神经网络，其具有以下属性：
- 一个输入层，一个输出层，一个或多个隐含层。上图所示的神经网络中有一个三神经元的输入层、一个四神经元的隐含层、一个二神经元的输出层。
- 每一个神经元都是一个上文提到的感知机。
- 输入层的神经元作为隐含层的输入，同时隐含层的神经元也是输出层神经元的输入。
- 每条建立在神经元之间的连接都有一个权重 *w* （与感知机中提到的权重类似）。
- 在 *t* 层的每个神经元通常与前一层（ *t - 1层*）中的每个神经元都有连接（但你可以通过将这条连接的权重设为0来断开这条连接）。
- 为了处理输入数据，将输入向量赋到输入层中。在上例中，这个网络可以计算一个3维输入向量（由于只有3个输入层神经元）。假如输入向量是 [7, 1, 2]，你将第一个输入神经元输入7，中间的输入1，第三个输入2。这些值将被传播到隐含层，通过加权传递函数传给每一个隐含层神经元（这就是前向传播），隐含层神经元再计算输出（激活函数）。
- 输出层和隐含层一样进行计算，输出层的计算结果就是整个神经网络的输出。
### 非线性
　　如果每一个感知机都只能使用一个线性激活函数会怎么样？整个网络的最终输出也仍然是将输入数据通过一些线性函数计算过一遍，只是用一些在网络中收集的不同权值调整了一下。换名话说，再多线性函数的组合还是线性函数。如果我们限定只能使用线性激活函数的话，前馈神经网络其实比一个感知机强大不到哪里去，无论网络有多少层。
　　正是这个原因，大多数神经网络都是使用的非线性激活函数，如对数函数、双曲正切函数、阶跃函数、整流函数等。不用这些非线性函数的神经网络只能学习输入数据的线性组合。
### 训练过程
大多数常见的应用在多层感知机的监督式训练的算法都是反向传播算法。基本的流程如下：
　　1、将训练样本通过神经网络进行前向传播计算。
　　2、计算输出误差，常用均方差：
![](http://images.cnitblog.com/i/629033/201404/302128239083193.png)
　　　其中 *t* 是目标值， *y* 是实际的神经网络计算输出。其它的误差计算方法也可以，但MSE（均方差）通常是一种较好的选择。
![](http://images.cnitblog.com/i/629033/201404/302136275333710.png)
　　3、网络误差通过 [随机梯度下降](http://en.wikipedia.org/wiki/Stochastic_gradient_descent)/梯度下降法 的方法来最小化。
　　梯度下降很常用，但在神经网络中，输入参数是一个训练误差的曲线。每个权重的最佳值应该是误差曲线中的全局最小值（上图中的 *global minimum*）。在训练过程中，权重以非常小的步幅改变（在每个样本或每小组样本训练完成后）以找到全局最小值，但这可不容易，训练通常会结束在局部最小值上（上图中的local minima）。如例子中的，如果当前权重值为0.6，那么要向0.4方向移动。
　　这个图表示的是最简单的情况，误差只依赖于单个参数。但是，网络误差依赖于每一个网络权重，误差函数非常、非常复杂。
　　好消息是反向传播算法提供了一种通过利用输出误差来修正两个神经元之间权重的方法。关系本身十分复杂，但对于一个给定结点的权重修正按如下方法（简单）：
![](http://images.cnitblog.com/i/629033/201404/302159245645034.png)
　　其中 *E* 是输出误差， *w_i* 是输入 *i* 的权重。
　　实质上这么做的目的是利用权重 *i *来修正梯度的方向。关键的地方在于误差的导数的使用，这可不一定好计算：你怎么样能给一个大型网络中随机一个结点中的随机一个权重求导数呢？
　　答案是：通过反向传播。误差的首次计算很简单（只要对预期值和实际值做差即可），然后通过一种巧妙的方法反向传回网络，让我们有效的在训练过程中修正权重并（期望）达到一个最小值。
### 隐含层
　　隐含层十分有趣。根据[普适逼近原理](http://en.wikipedia.org/wiki/Universal_approximation_theorem)，一个具有有限数目神经元的隐含层可以被训练成可逼近任意随机函数。换句话说，一层隐含层就强大到可以学习任何函数了。这说明我们在多隐含层（如深度网络）的实践中可以得到更好的结果。
　　隐含层存储了训练数据的内在抽象表示，和人类大脑（简化的类比）保存有对真实世界的抽象一样。接下来，我们将用各种方法来搞一下这个隐含层。
### 一个网络的例子
　　可以看一下这个通过 *testMLPSigmoidBP* 方法用Java实现的简单(4-2-3)前馈神经网络，它将 [IRIS](http://archive.ics.uci.edu/ml/datasets/Iris) 数据集进行了分类。这个数据集中包含了三类鸢尾属植物，特征包括花萼长度，花瓣长度等等。每一类提供50个样本给这个神经网络训练。特征被赋给输入神经元，每一个输出神经元代表一类数据集（“1/0/0”
 表示这个植物是Setosa，“0/1/0”表示 Versicolour，而“0/0/1”表示 Virginica）。分类的错误率是2/150（即每分类150个，错2个）。
### 大规模网络中的难题
　　神经网络中可以有多个隐含层：这样，在更高的隐含层里可以对其之前的隐含层构建新的抽象。而且像之前也提到的，这样可以更好的学习大规模网络。增加隐含层的层数通常会导致两个问题：
　　　　1、**梯度消失**：随着我们添加越来越多的隐含层，反向传播传递给较低层的信息会越来越少。实际上，由于信息向前反馈，不同层次间的梯度开始消失，对网络中权重的影响也会变小。
　　　　2、**过度拟合**：也许这是机器学习的核心难题。简要来说，过度拟合指的是对训练数据有着过于好的识别效果，这时导至模型非常复杂。这样的结果会导致对训练数据有非常好的识别较果，而对真实样本的识别效果非常差。
下面我们来看看一些深度学习的算法是如何面对这些难题的。
## 三. 自编码器--逐层哈希
大多数的机器学习入门课程都会让你放弃前馈神经网络。但是实际上这里面大有可为——请接着看。
　　自编码器就是一个典型的前馈神经网络，它的目标就是学习一种对数据集的压缩且分布式的表示方法（编码思想）。
![](http://images.cnitblog.com/i/629033/201405/042101350337185.png)
　　从概念上讲，神经网络的目的是要训练去“重新建立”输入数据，好像输入和目标输出数据是一样的。换句话说：你正在让神经网络的输出与输入是同一样东西，只是经过了压缩（或者说是所谓的**降维**.....）。这还是不好理解，先来看一个例子。
### 压缩输入数据：灰度图像
　　这里有一个由28x28像素的灰度图像组成的训练集，且每一个像素的值都作为一个输入层神经元的输入（这时输入层就会有784个神经元）。输出层神经元要有相同的数目（784），且每一个输出神经元的输出值和输入图像的对应像素灰度值相同。
　　在这样的算法架构背后，~~神经网络学习到的实际上并不是一个训练数据到标记的“映射”，而是去学习数据本身的内在结构和特征（也正是因为这，隐含层也被称作特征探测器(feature detector)~~）。通常隐含层中的神经元数目要比输入/输入层的少，这是为了使神经网络只去学习最重要的特征并实现特征的降维。
　　我们想在中间层用很少的结点去在概念层上学习数据、产生一个紧致的表示方法。
### 流行感冒例子
　　为了更好的描述自编码器，再看一个应用。
　　这次我们使用一个简单的数据集，其中包括一些感冒的症状。如果感兴趣，这个例子的源码发布在[这里](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/test/java/com/github/neuralnetworks/test/AETest.java)。
　　数据结构如下：
- - 输入数据一共六个二进制位
- 前三位是病的证状。例如，*1 0 0 0 0 0* 代表病人发烧；*0 1 0 0 0 0* 代表咳嗽；*1 1 0 0 0 0* 代表即咳嗽又发烧等等。
- 后三位表示抵抗能力，如果一个病人有这个，代表他/她不太可能患此病。例如，*0 0 0 1 0 0* 代表病人接种过流感疫苗。一个可能的组合是：*0 1 0 1 0 0* ，这代表着一个接种过流感疫苗的咳嗽病人，等等。
　　当一个病人同时拥用前三位中的两位时，我们认为他生病了；如果至少拥用后三位中的两位，那么他是健康的，如：
- - *111000, 101000, 110000, 011000, 011100* = 生病
- *000111, 001110, 000101, 000011, 000110* = 健康
　　我们来训练一个自编码器（使用反向传播），六个输入、六个输出神经元，而只有两个隐含神经元。
　　在经过几百次迭代以后，我们发现，每当一个“生病”的样本输入时，两个隐含层神经元中的一个（对于生病的样本总是这个）总是显示出更高的激活值。而如果输入一个“健康”样本时，另一个隐含层则会显示更高的激活值。
### 再看学习
　　本质上来说，这两个隐含神经元从数据集中学习到了流感症状的一种紧致表示方法。为了检验它是不是真的实现了学习，我们再看下过度拟合的问题。通过训练我们的神经网络学习到的是一个紧致的简单的，而不是一个高度复杂且对数据集过度拟合的表示方法。
　　某种程度上来讲，与其说在找一种简单的表示方法，我们更是在尝试从“感觉”上去学习数据。
## 四. 受限波尔兹曼机
　　下一步来看下受限波尔兹曼机（[Restricted Boltzmann machines](http://en.wikipedia.org/wiki/Restricted_Boltzmann_machine) RBM），一种可以在输入数据集上学习概率分布的生成随机神经网络。
![](http://images.cnitblog.com/i/629033/201405/042157554391186.png)
　　RBM由隐含层、可见层、偏置层组成。和前馈神经网络不同，可见层和隐含层之间的连接是无方向性（值可以从可见层->隐含层或隐含层->可见层任意传输）且全连接的（每一个当前层的神经元与下一层的每个神经元都有连接——如果允许任意层的任意神经元连接到任意层去，我们就得到了一个波尔兹曼机（非受限的））。
　　标准的RBM中，隐含和可见层的神经元都是二态的（即神经元的激活值只能是服从[伯努力分布](http://en.wikipedia.org/wiki/Bernoulli_distribution)的0或1），不过也存在其它非线性的变种。
　　虽然学者们已经研究RBM很长时间了，最近出现的对比差异无监督训练算法使这个领域复兴。
### 对比差异
　　单步对比差异算法原理：
　　1、正向过程：
- - 输入样本 *v* 输入至输入层中。
- *v* 通过一种与前馈网络相似的方法传播到隐含层中，隐含层的激活值为 *h*。
　　2、反向过程：
- - 将 *h* 传回可见层得到 *v’* （可见层和隐含层的连接是无方向的，可以这样传）。
- 再将 *v’* 传到隐含层中，得到 *h’*。
　　3、权重更新：
![](http://images.cnitblog.com/i/629033/201405/042224050481209.png)
　　　　        其中 *a* 是学习速率， *v*, *v’*, *h*, *h’* 和 *w *都是向量。
　　算法的思想就是在正向过程中影响了网络的内部对于真实数据的表示。同时，反向过程中尝试通过这个被影响过的表示方法重建数据。主要目的是可以使生成的数据与原数据尽可能相似，这个差异影响了权重更新。
　　换句话说，这样的网络具有了感知对输入数据表示的程度的能力，而且尝试通过这个感知能力重建数据。如果重建出来的数据与原数据差异很大，那么进行调整并再次重建。
### 再看流行感冒的例子
　　为了说明对比差异，我们使用与上例相同的流感症状的数据集。测试网络是一个包含6个可见层神经元、2个隐含层神经元的RBM。我们用对比差异的方法对网络进行训练，将症状 *v* 赋到可见层中。在测试中，这些症状值被重新传到可见层；然后再被传到隐含层。隐含层的神经元表示健康/生病的状态，与自编码器相似。
　　在进行过几百次迭代后，我们得到了与自编码器相同的结果：输入一个生病样本，其中一个隐含层神经元具有更高激活值；输入健康的样本，则另一个神经元更兴奋。
　　例子的代码在[这里](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/test/java/com/github/neuralnetworks/test/RBMTest.java)。
## 深度网络
　　到现在为止，我们已经学习了隐含层中强大的特征探测器——自编码器和RBM，~~但现在还没有办法有效的去利用这些功能~~。实际上，上面所用到的这些数据集都是特定的。而我们要找到一些方法来间接的使用这些探测出的特征。
　　好消息是，已经发现这些结构可以通过栈式叠加来实现深度网络。这些网络可以通过贪心法的思想训练，每次训练一层，以克服之前提到在反向传播中梯度消失及过度拟合的问题。
　　这样的算法架构十分强大，可以产生很好的结果。如Google著名的[“猫”识别](http://static.googleusercontent.com/media/research.google.com/en/us/pubs/archive/38115.pdf)，在实验中通过使用特定的**深度自编码器**，在无标记的图片库中学习到人和猫脸的识别。
　　下面我们将更深入。
### 栈式自编码器
　　和名字一样，这种网络由多个栈式结合的自编码器组成。
![](http://images.cnitblog.com/i/629033/201405/052136291049785.png)
　　自编码器的隐含层 *t* 会作为 *t + 1 *层的输入层。第一个输入层就是整个网络的输入层。利用贪心法训练每一层的步骤如下：
　　　　1、通过反向传播的方法利用所有数据对第一层的自编码器进行训练（*t=1*，上图中的红色连接部分）。
　　　　2、训练第二层的自编码器 *t=2* （绿色连接部分）。由于 *t=2* 的输入层是 *t=1 *的隐含层，我们已经不再关心 *t=1* 的输入层，可以从整个网络中移除。整个训练开始于将输入样本数据赋到 *t=1 *的输入层，通过前向传播至 t = 2 的输出层。下面t = 2的权重（输入->隐含和隐含->输出）使用反向传播的方法进行更新。t = 2的层和 t=1 的层一样，都要通过所有样本的训练。
　　　　3、对所有层重复步骤1-2（即移除前面自编码器的输出层，用另一个自编码器替代，再用反向传播进行训练）。
　　　　4、步骤1-3被称为预训练，这将网络里的权重值初始化至一个合适的位置。但是通过这个训练并没有得到一个输入数据到输出标记的映射。例如，一个网络的目标是被训练用来识别手写数字，经过这样的训练后还不能将最后的特征探测器的输出（即隐含层中最后的自编码器）对应到图片的标记上去。这样，一个通常的办法是在网络的最后一层（即蓝色连接部分）后面再加一个或多个全连接层。整个网络可以被看作是一个多层的感知机，并使用反向传播的方法进行训练（这步也被称为微调）。
　　栈式自编码器，提供了一种有效的预训练方法来初始化网络的权重，这样你得到了一个可以用来训练的复杂、多层的感知机。
**DBN与RBM**：
全部用RBM形成的深层模型为深度玻尔兹曼机（Deep Boltzmann Machine，DBM）。如果将靠近输入层的部分替换为贝叶斯信念网络，即有向图模型，而在远离输入层的部分仍然使用RBM，则称为深度信念网络 （Deep Belief Networks，DBN）。
**插入：**
**RBM-受限波尔兹曼机**
假设有一个二部图，每一层的节点之间没有链接，一层是可视层，即输入数据层（v)，一层是隐藏层(h)，如果假设所有的节点都是随机二值变量节点（只能取0或者1值），同时假设全概率分布p(v,h)满足Boltzmann 分布，我们称这个模型是Restricted BoltzmannMachine (RBM)。
![](https://img-my.csdn.net/uploads/201304/10/1365561323_7932.jpg)
下面我们来看看为什么它是Deep Learning方法。首先，这个模型因为是二部图，所以在已知v的情况下，所有的隐藏节点之间是条件独立的（因为节点之间不存在连接），即p(h|v)=p(h1|v)…p(hn|v)。同理，在已知隐藏层h的情况下，所有的可视节点都是条件独立的。同时又由于所有的v和h满足Boltzmann 分布，因此，当输入v的时候，通过p(h|v) 可以得到隐藏层h，而得到隐藏层h之后，通过p(v|h)又能得到可视层，通过调整参数，我们就是要使得从隐藏层得到的可视层v1与原来的可视层v如果一样，那么得到的隐藏层就是可视层另外一种表达，因此隐藏层可以作为可视层输入数据的特征，所以它就是一种Deep
 Learning方法。
![](https://img-my.csdn.net/uploads/201304/10/1365561362_2427.jpg)
如何训练呢？也就是可视层节点和隐节点间的权值怎么确定呢？我们需要做一些数学分析。也就是模型了。
![](https://img-my.csdn.net/uploads/201304/10/1365561384_7276.jpg)
**能量最小化**
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
       如果，我们把隐藏层的层数增加，我们可以得到Deep Boltzmann Machine(DBM)；如果我们在靠近可视层的部分使用贝叶斯信念网络（即有向图模型，当然这里依然限制层中节点之间没有链接），而在最远离可视层的部分使用Restricted Boltzmann Machine，我们可以得到DeepBelief Net（DBN）。
![](https://img-my.csdn.net/uploads/201304/10/1365561611_3496.jpg)
**训练过程和代码：**
       对于二者都使用同一个算法来训练，看起来毫无区别，但是DBM有一个优势，由于RBM是无向的，这就决定了无论给定可视节点还是隐藏节点，各个节点都是独立的，可由图模型的马尔科夫性看出。
       作为无向图的DBM天生具有一些优秀的基因，比如当人看到一个外观性质，知道它是什么物体，同样你告诉他物体名字，他可以知道物体的外观应该是什么样子。这种**互相推理的关系正好可以用无向图**来表示。这种优势也顺理成章的延伸出了autoencoder（大家所谓的自编码神经网络）和栈式神经网络，最终输出的少量节点是可以推理（重建）出原来样本，也起到了降维的作用，无形中也找到了特征（编码），autoencoder的效果如图二所示。但是DBN中有些层是有向的，就不具有这种优势。
![](http://www.chinakdd.com/include/ueditor/jsp/upload/20131118/71401384738400891.jpg)
二者逐层预训练后，结合样本标签，使用BP算法进行权重微调，说白了就是**在预训练后的权重基础上使用BP算法进行训**练，这样得出的权重更好些。。。
下面贴出部分DBN代码，大家可以看出总体思路是按照构建DBN网络（刚构建后的每层的权重是随机生成的，从代码也能看出），贪心层层预训练，权重微调，预测（识别）这个步骤来的。
      另外代码中**softmax其实是多变量的逻辑回归函数**，注意我发的下面的代码中权重微调使用的是逻辑回归，不是BP：
      多维逻辑回归使用的优化方法依然是最小二乘，二不是BP.
```cpp
#include <iostream>  
#include <math.h>  
#include "HiddenLayer.h"  
#include "RBM.h"  
#include "LogisticRegression.h"  
#include "DBN.h"  
#include "FirstRBM.h"
using namespace std;  
double uniform(double min, double max) {  
	return rand() / (RAND_MAX 1.0) * (max - min) min;  
}  
int binomial(int n, double p) {  
	if(p < 0 || p > 1) return 0;  
	int c = 0;  
	double r;  
	for(int i=0; i<n; i ) {  
		r = rand() / (RAND_MAX 1.0);  
		if (r < p) c ;  
	}  
	return c;  
}  
double sigmoid(double x) {  
	return 1.0 / (1.0 exp(-x));  
}  
// DBN  
DBN::DBN(int size, int n_i, int *hls, int n_o, int n_l) {  
	int input_size;  
	N = size;  
	n_ins = n_i;  
	hidden_layer_sizes = hls;  
	n_outs = n_o;  
	n_layers = n_l;  
	sigmoid_layers = new HiddenLayer*[n_layers];  
	rbm_layers = new RBM*[n_layers];  
	// construct multi-layer  
	for(int i=0; i<n_layers; i ) {  
		if(i == 0) {  
			input_size = n_ins;  
		} else {  
			input_size = hidden_layer_sizes[i-1];  
		}  
		// construct sigmoid_layer  
		sigmoid_layers[i] = new HiddenLayer(N, input_size, hidden_layer_sizes[i], NULL, NULL);  
		// construct rbm_layer  
		rbm_layers[i] = new RBM(N, input_size, hidden_layer_sizes[i],\  
			sigmoid_layers[i]->W, sigmoid_layers[i]->b, NULL);  
	}  
	// layer for output using LogisticRegression  
	log_layer = new LogisticRegression(N, hidden_layer_sizes[n_layers-1], n_outs);  
}  
DBN::~DBN() {  
	delete log_layer;  
	for(int i=0; i<n_layers; i ) {  
		delete sigmoid_layers[i];  
		delete rbm_layers[i];  
	}  
	delete[] sigmoid_layers;  
	delete[] rbm_layers;  
}  
void DBN::pretrain(int *input, double lr, int k, int epochs) {  
	int *layer_input;  
	int prev_layer_input_size;  
	int *prev_layer_input;  
	int *train_X = new int[n_ins];  
	for(int i=0; i<n_layers; i ) {  // layer-wise  
		for(int epoch=0; epoch<epochs; epoch ) {  // training epochs  
			for(int n=0; n<N; n ) { // input x1...xN  
				// initial input  
				for(int m=0; m<n_ins; m ) train_X[m] = input[n * n_ins m];  
				// layer input  
				for(int l=0; l<=i; l ) {  
					if(l == 0) {  
						layer_input = new int[n_ins];  
						for(int j=0; j<n_ins; j ) layer_input[j] = train_X[j];  
					} else {  
						if(l == 1) prev_layer_input_size = n_ins;  
						else prev_layer_input_size = hidden_layer_sizes[l-2];  
						prev_layer_input = new int[prev_layer_input_size];  
						for(int j=0; j<prev_layer_input_size; j ) prev_layer_input[j] = layer_input[j];  
						delete[] layer_input;  
						layer_input = new int[hidden_layer_sizes[l-1]];  
						sigmoid_layers[l-1]->sample_h_given_v(prev_layer_input, layer_input);  
						delete[] prev_layer_input;  
					}  
				}  
				rbm_layers[i]->contrastive_divergence(layer_input, lr, k);  
			}  
		}  
	}  
	delete[] train_X;  
	delete[] layer_input;  
}  
void DBN::finetune(int *input, int *label, double lr, int epochs) {  
	int *layer_input;  
	// int prev_layer_input_size;  
	int *prev_layer_input;  
	int *train_X = new int[n_ins];  
	int *train_Y = new int[n_outs];  
	for(int epoch=0; epoch<epochs; epoch ) {  
		for(int n=0; n<N; n ) { // input x1...xN  
			// initial input  
			for(int m=0; m<n_ins; m )  train_X[m] = input[n * n_ins m];  
			for(int m=0; m<n_outs; m ) train_Y[m] = label[n * n_outs m];  
			// layer input  
			for(int i=0; i<n_layers; i ) {  
				if(i == 0) {  
					prev_layer_input = new int[n_ins];  
					for(int j=0; j<n_ins; j ) prev_layer_input[j] = train_X[j];  
				} else {  
					prev_layer_input = new int[hidden_layer_sizes[i-1]];  
					for(int j=0; j<hidden_layer_sizes[i-1]; j ) prev_layer_input[j] = layer_input[j];  
					delete[] layer_input;  
				}  
				layer_input = new int[hidden_layer_sizes[i]];  
				sigmoid_layers[i]->sample_h_given_v(prev_layer_input, layer_input);  
				delete[] prev_layer_input;  
			}  
			log_layer->train(layer_input, train_Y, lr);  
		}  
		// lr *= 0.95;  
	}  
	delete[] layer_input;  
	delete[] train_X;  
	delete[] train_Y;  
}  
void DBN::predict(int *x, double *y) {  
	double *layer_input;  
	// int prev_layer_input_size;  
	double *prev_layer_input;  
	double linear_output;  
	prev_layer_input = new double[n_ins];  
	for(int j=0; j<n_ins; j ) prev_layer_input[j] = x[j];  
	// layer activation  
	for(int i=0; i<n_layers; i ) {  
		layer_input = new double[sigmoid_layers[i]->n_out];  
		for(int k=0; k<sigmoid_layers[i]->n_out; k ) {  
			// linear_output = 0.0;  //原代码中删除此句  
			for(int j=0; j<sigmoid_layers[i]->n_in; j ) {  
				linear_output = 0.0;   //原代码中添加此句  
				linear_output = sigmoid_layers[i]->W[k][j] * prev_layer_input[j];  
			}  
			linear_output = sigmoid_layers[i]->b[k];  
			layer_input[k] = sigmoid(linear_output);  
		}  
		delete[] prev_layer_input;  
		if(i < n_layers-1) {  
			prev_layer_input = new double[sigmoid_layers[i]->n_out];  
			for(int j=0; j<sigmoid_layers[i]->n_out; j ) prev_layer_input[j] = layer_input[j];  
			delete[] layer_input;  
		}  
	}  
	for(int i=0; i<log_layer->n_out; i ) {  
		y[i] = 0;  
		for(int j=0; j<log_layer->n_in; j ) {  
			y[i] = log_layer->W[i][j] * layer_input[j];  
		}  
		y[i] = log_layer->b[i];  
	}  
	log_layer->softmax(y);  
	delete[] layer_input;  
}  
// HiddenLayer  
HiddenLayer::HiddenLayer(int size, int in, int out, double **w, double *bp) {  
	N = size;  
	n_in = in;  
	n_out = out;  
	if(w == NULL) {  
		W = new double*[n_out];  
		for(int i=0; i<n_out; i ) W[i] = new double[n_in];  
		double a = 1.0 / n_in;  
		for(int i=0; i<n_out; i ) {  
			for(int j=0; j<n_in; j ) {  
				W[i][j] = uniform(-a, a);  
			}  
		}  
	} else {  
		W = w;  
	}  
	if(bp == NULL) {  
		b = new double[n_out];  
	} else {  
		b = bp;  
	}  
}  
HiddenLayer::~HiddenLayer() {  
	for(int i=0; i<n_out; i ) delete W[i];  
	delete[] W;  
	delete[] b;  
}  
double HiddenLayer::output(int *input, double *w, double b) {  
	double linear_output = 0.0;  
	for(int j=0; j<n_in; j ) {  
		linear_output = w[j] * input[j];  
	}  
	linear_output = b;  
	return sigmoid(linear_output);  
}  
void HiddenLayer::sample_h_given_v(int *input, int *sample) {  
	for(int i=0; i<n_out; i ) {  
		sample[i] = binomial(1, output(input, W[i], b[i]));  
	}  
}  
// RBM  
RBM::RBM(){}
RBM::RBM(int size, int n_v, int n_h, double **w, double *hb, double *vb) {  
	N = size;  
	n_visible = n_v;  
	n_hidden = n_h;  
	if(w == NULL) {  
		W = new double*[n_hidden];  
		for(int i=0; i<n_hidden; i ) W[i] = new double[n_visible];  
		double a = 1.0 / n_visible;  
		for(int i=0; i<n_hidden; i ) {  
			for(int j=0; j<n_visible; j ) {  
				W[i][j] = uniform(-a, a);  
			}  
		}  
	} else {  
		W = w;  
	}  
	if(hb == NULL) {  
		hbias = new double[n_hidden];  
		for(int i=0; i<n_hidden; i ) hbias[i] = 0;  
	} else {  
		hbias = hb;  
	}  
	if(vb == NULL) {  
		vbias = new double[n_visible];  
		for(int i=0; i<n_visible; i ) vbias[i] = 0;  
	} else {  
		vbias = vb;  
	}  
}  
RBM::~RBM() {  
	// for(int i=0; i<n_hidden; i ) delete[] W[i];  
	// delete[] W;  
	// delete[] hbias;  
	delete[] vbias;  
}  
void RBM::contrastive_divergence(int *input, double lr, int k) {  
	double *ph_mean = new double[n_hidden];  
	int *ph_sample = new int[n_hidden];  
	double *nv_means = new double[n_visible];  
	int *nv_samples = new int[n_visible];  
	double *nh_means = new double[n_hidden];  
	int *nh_samples = new int[n_hidden];  
	/* CD-k */  
	sample_h_given_v(input, ph_mean, ph_sample);  
	for(int step=0; step<k; step ) {  
		if(step == 0) {  
			gibbs_hvh(ph_sample, nv_means, nv_samples, nh_means, nh_samples);  
		} else {  
			gibbs_hvh(nh_samples, nv_means, nv_samples, nh_means, nh_samples);  
		}  
	}  
	for(int i=0; i<n_hidden; i ) {  
		for(int j=0; j<n_visible; j ) {  
			W[i][j] = lr * (ph_sample[i] * input[j] - nh_means[i] * nv_samples[j]) / N;  
		}  
		hbias[i] = lr * (ph_sample[i] - nh_means[i]) / N;  
	}  
	for(int i=0; i<n_visible; i ) {  
		vbias[i] = lr * (input[i] - nv_samples[i]) / N;  
	}  
	delete[] ph_mean;  
	delete[] ph_sample;  
	delete[] nv_means;  
	delete[] nv_samples;  
	delete[] nh_means;  
	delete[] nh_samples;  
}  
void RBM::sample_h_given_v(int *v0_sample, double *mean, int *sample) {  
	for(int i=0; i<n_hidden; i ) {  
		mean[i] = propup(v0_sample, W[i], hbias[i]);  
		sample[i] = binomial(1, mean[i]);  
	}  
}  
void RBM::sample_v_given_h(int *h0_sample, double *mean, int *sample) {  
	for(int i=0; i<n_visible; i ) {  
		mean[i] = propdown(h0_sample, i, vbias[i]);  
		sample[i] = binomial(1, mean[i]);  
	}  
}  
double RBM::propup(int *v, double *w, double b) {  
	double pre_sigmoid_activation = 0.0;  
	for(int j=0; j<n_visible; j ) {  
		pre_sigmoid_activation = w[j] * v[j];  
	}  
	pre_sigmoid_activation = b;  
	return sigmoid(pre_sigmoid_activation);  
}  
double RBM::propdown(int *h, int i, double b) {  
	double pre_sigmoid_activation = 0.0;  
	for(int j=0; j<n_hidden; j ) {  
		pre_sigmoid_activation = W[j][i] * h[j];  
	}  
	pre_sigmoid_activation = b;  
	return sigmoid(pre_sigmoid_activation);  
}  
void RBM::gibbs_hvh(
	int *h0_sample, double *nv_means, int *nv_samples,
	double *nh_means, int *nh_samples) {  
		sample_v_given_h(h0_sample, nv_means, nv_samples);  
		sample_h_given_v(nv_samples, nh_means, nh_samples);  
}  
void RBM::reconstruct(int *v, double *reconstructed_v) {  
	double *h = new double[n_hidden];  
	double pre_sigmoid_activation;  
	for(int i=0; i<n_hidden; i ) {  
		h[i] = propup(v, W[i], hbias[i]);  
	}  
	for(int i=0; i<n_visible; i ) {  
		pre_sigmoid_activation = 0.0;  
		for(int j=0; j<n_hidden; j ) {  
			pre_sigmoid_activation = W[j][i] * h[j];  
		}  
		pre_sigmoid_activation = vbias[i];  
		reconstructed_v[i] = sigmoid(pre_sigmoid_activation);  
	}  
	delete[] h;  
}  
// LogisticRegression  
LogisticRegression::LogisticRegression(int size, int in, int out) {  
	N = size;  
	n_in = in;  
	n_out = out;  
	W = new double*[n_out];  
	for(int i=0; i<n_out; i ) W[i] = new double[n_in];  
	b = new double[n_out];  
	for(int i=0; i<n_out; i ) {  
		for(int j=0; j<n_in; j ) {  
			W[i][j] = 0;  
		}  
		b[i] = 0;  
	}  
}  
LogisticRegression::~LogisticRegression() {  
	for(int i=0; i<n_out; i ) delete[] W[i];  
	delete[] W;  
	delete[] b;  
}  
void LogisticRegression::train(int *x, int *y, double lr) {  
	double *p_y_given_x = new double[n_out];  
	double *dy = new double[n_out];  
	for(int i=0; i<n_out; i ) {  
		p_y_given_x[i] = 0;  
		for(int j=0; j<n_in; j ) {  
			p_y_given_x[i] = W[i][j] * x[j];  
		}  
		p_y_given_x[i] = b[i];  
	}  
	softmax(p_y_given_x);  
	for(int i=0; i<n_out; i ) {  
		dy[i] = y[i] - p_y_given_x[i];  
		for(int j=0; j<n_in; j ) {  
			W[i][j] = lr * dy[i] * x[j] / N;  
		}  
		b[i] = lr * dy[i] / N;  
	}  
	delete[] p_y_given_x;  
	delete[] dy;  
}  
void LogisticRegression::softmax(double *x) {  
	double max = 0.0;  
	double sum = 0.0;  
	for(int i=0; i<n_out; i ) if(max < x[i]) max = x[i];  
	for(int i=0; i<n_out; i ) {  
		x[i] = exp(x[i] - max);  
		sum = x[i];  
	}  
	for(int i=0; i<n_out; i ) x[i] /= sum;  
}  
void LogisticRegression::predict(int *x, double *y) {  
	for(int i=0; i<n_out; i ) {  
		y[i] = 0;  
		for(int j=0; j<n_in; j ) {  
			y[i] = W[i][j] * x[j];  
		}  
		y[i] = b[i];  
	}  
	softmax(y);  
}  
void test_dbn() {  
	srand(0);  
	double pretrain_lr = 0.1;  
	int pretraining_epochs = 1000;  
	int k = 1;  
	double finetune_lr = 0.1;  
	int finetune_epochs = 500;  
	int train_N = 6;  
	int test_N = 3;  
	int n_ins = 6;  
	int n_outs = 2;  
	int hidden_layer_sizes[] = {3, 3};  
	int n_layers = sizeof(hidden_layer_sizes) / sizeof(hidden_layer_sizes[0]);  
	// training data  
	int train_X[6][6] = {  
		{1, 1, 1, 0, 0, 0},  
		{1, 0, 1, 0, 0, 0},  
		{1, 1, 1, 0, 0, 0},  
		{0, 0, 1, 1, 1, 0},  
		{0, 0, 1, 1, 0, 0},  
		{0, 0, 1, 1, 1, 0}  
	};  
	int train_Y[6][2] = {  
		{1, 0},  
		{1, 0},  
		{1, 0},  
		{0, 1},  
		{0, 1},  
		{0, 1}  
	};  
	// construct DBN  
	DBN dbn(train_N, n_ins, hidden_layer_sizes, n_outs, n_layers);  
	// pretrain  
	dbn.pretrain(*train_X, pretrain_lr, k, pretraining_epochs);  
	// finetune  
	dbn.finetune(*train_X, *train_Y, finetune_lr, finetune_epochs);  
	// test data  
	int test_X[3][6] = {  
		{1, 1, 0, 0, 0, 0},  
		{0, 0, 0, 1, 1, 0},  
		{1, 1, 1, 1, 1, 0}  
	};  
	double test_Y[3][2];  
	// test  
	for(int i=0; i<test_N; i ) {  
		dbn.predict(test_X[i], test_Y[i]);  
		for(int j=0; j<n_outs; j ) {  
			cout << test_Y[i][j] << " ";  
		}  
		cout << endl;  
	}  
}  
int main() {  
	test_dbn();  
	return 0;  
}
```
程序输出：
```cpp
<pre></pre>  
    <p>程序运行结果，是个二维的回归值：</p>  
    <p>0.493724  0.5062760.493724  0.5062760.493724  0.506276</p>
```
### 深度念网络
　　和自编码器一样，我也可以将波尔兹曼机进行栈式叠加来构建深度信度网络（DBN）。
![](http://images.cnitblog.com/i/629033/201405/052153574959807.png)
　　在本例中，隐含层 RBM *t *可以看作是 RBM *t+1 *的可见层。第一个RBM的输入层即是整个网络的输入层，层间贪心式的预训练的工作模式如下：
　　　　1. 通过对比差异法对所有训练样本训练第一个RBM *t=1*
　　　　2. 训练第二个RBM *t=1*。由于 *t=2* 的可见层是 *t=1 *的隐含层，训练开始于将数据赋至 *t=1 *的可见层，通过前向传播的方法传至 *t=1* 的隐含层。然后作为 *t=2 *的对比差异训练的初始数据。
　　　　3. 对所有层重复前面的过程。
　　　　4. 和栈式自编码器一样，通过预训练后，网络可以通过连接到一个或多个层间全连接的 RBM 隐含层进行扩展。这构成了一个可以通过反向传僠进行微调的多层感知机。
　　本过程和栈式自编码器很相似，只是用RBM将自编码器进行替换，并用对比差异算法将反向传播进行替换。
*　　(注: 例中的源码可以从 [此处](https://github.com/ivan-vasilev/neuralnetworks/blob/d2bbc296eca926d07d09b860b29c5a5a3f632f63/nn-core/src/test/java/com/github/neuralnetworks/test/DNNTest.java)获得.)*
**五. Deep Belief Networks-深度信念网络**
DBNs是一个概率生成模型，与传统的判别模型的神经网络相对，生成模型是建立一个观察数据和标签之间的联合分布，对P(Observation|Label)和 P(Label|Observation)都做了评估，而判别模型仅仅而已评估了后者，也就是P(Label|Observation)。对于在深度神经网络应用传统的BP算法的时候，DBNs遇到了以下问题：
（1）需要为训练提供一个有标签的样本集；
（2）学习过程较慢；
（3）不适当的参数选择会导致学习收敛于局部最优解。
![](https://img-my.csdn.net/uploads/201304/10/1365561636_9432.jpg)
       DBNs由多个限制玻尔兹曼机（Restricted Boltzmann Machines）层组成，一个典型的神经网络类型如图三所示。这些网络被“限制”为一个可视层和一个隐层，层间存在连接，但层内的单元间不存在连接。隐层单元被训练去捕捉在可视层表现出来的高阶数据的相关性。
**训练过程**
       首先，先不考虑最顶构成一个联想记忆（associative memory）的两层，一个DBN的连接是通过自顶向下的生成权值来指导确定的，RBMs就像一个建筑块一样，相比传统和深度分层的sigmoid信念网络，它能易于连接权值的学习。
       最开始的时候，通过一个非监督贪婪逐层方法去预训练获得生成模型的权值，非监督贪婪逐层方法被Hinton证明是有效的，并被其称为对比分歧（contrastive divergence）。
       在这个训练阶段，在可视层会产生一个向量v，通过它将值传递到隐层。反过来，可视层的输入会被随机的选择，以尝试去重构原始的输入信号。最后，这些新的可视的神经元激活单元将前向传递重构隐层激活单元，获得 h（在训练过程中，首先将 可视向量值 映射给隐单元；然后 可视单元 由隐层单元重建； 这些新可视单元 再次映射给隐单元，这样就 获取新的隐单元。执行这种反复步骤叫做**吉布斯采样**）。这些后退和前进的步骤就是我们熟悉的**Gibbs采样**，而隐层激活单元和可视层输入之间的相关性差别就作为权值更新的主要依据。
       训练时间会显著的减少，因为只需要单个步骤就可以接近最大似然学习。增加进网络的每一层都会改进训练数据的对数概率，我们可以理解为越来越接近能量的真实表达。这个有意义的拓展，和无标签数据的使用，是任何一个深度学习应用的决定性的因素。
![](https://img-my.csdn.net/uploads/201304/10/1365561659_1933.jpg)
在最高两层，权值被连接到一起，这样更低层的输出将会提供一个参考的线索或者关联给顶层，这样顶层就会将其联系到它的记忆内容。而我们最关心的，最后想得到的就是判别性能，例如分类任务里面。
       在预训练后，DBN可以通过利用带标签数据用BP算法去对判别性能做调整。在这里，一个标签集将被附加到顶层（推广联想记忆），通过一个自下向上的，学习到的识别权值获得一个网络的分类面。这个性能会比单纯的BP算法训练的网络好。这可以很直观的解释，DBNs的BP算法只需要对权值参数空间进行一个局部的搜索，这相比前向神经网络来说，训练是要快的，而且收敛的时间也少。
       DBNs的灵活性使得它的拓展比较容易。一个拓展就是卷积DBNs（Convolutional Deep Belief Networks(CDBNs)）。DBNs并没有考虑到图像的2维结构信息，因为输入是简单的从一个图像矩阵一维向量化的。而** CDBNs** 就是考虑到了这个问题，它利用邻域像素的空域关系，通过一个称为卷积RBMs的模型区达到生成模型的变换不变性，而且可以容易得变换到高维图像。DBNs并没有明确地处理对观察变量的时间联系的学习上，虽然目前已经有这方面的研究，例如堆叠时间RBMs，以此为推广，有序列学习的dubbed
 temporal convolutionmachines，这种序列学习的应用，给语音信号处理问题带来了一个让人激动的未来研究方向。
       目前，和DBNs有关的研究包括堆叠自动编码器，它是通过用堆叠自动编码器来替换传统DBNs里面的RBMs。这就使得可以通过同样的规则来训练产生深度多层神经网络架构，但它缺少层的参数化的严格要求。与DBNs不同，自动编码器使用判别模型，这样这个结构就很难采样输入采样空间，这就使得网络更难捕捉它的内部表达。但是，降噪自动编码器却能很好的避免这个问题，并且比传统的DBNs更优。它通过在训练过程添加随机的污染并堆叠产生场泛化性能。训练单一的降噪自动编码器的过程和RBMs训练生成模型的过程一样。
## 卷积神经网络（图像识别）
　　 这个是本文最后一个软件架构——卷积网络，一类特殊的对图像识别非常有效的前馈网络。
![](http://images.cnitblog.com/i/629033/201405/052154359799665.png)
　　在我们深入看实际的卷积网络之臆，我们先定义一个图像滤波器，或者称为一个赋有相关权重的方阵。一个滤波器可以应用到整个图片上，通常可以应用多个滤波器。比如，你可以应用四个6x6的滤波器在一张图片上。然后，输出中坐标（1,1）的像素值就是输入图像左上角一个6x6区域的加权和，其它像素也是如此。
　　有了上面的基础，我们来介绍定义出卷积网络的属性：
- **卷积层**  对输入数据应用若干滤波器。比如图像的第一卷积层使用4个6x6滤波器。对图像应用一个滤波器之后的得到的结果被称为特征图谱（feature map, FM），特征图谱的数目和滤波器的数目相等。如果前驱层也是一个卷积层，那么滤波器应用在FM上，相当于输入一个FM，输出另外一个FM。从直觉上来讲，如果将一个权重分布到整个图像上后，那么这个特征就和位置无关了，同时多个滤波器可以分别探测出不同的特征。
- **下采样层** 缩减输入数据的规模。例如输入一个32x32的图像，并且通过一个2x2的下采样，那么可以得到一个16x16的输出图像，这意味着原图像上的四个像素合并成为输出图像中的一个像素。实现下采样的方法有很多种，最常见的是最大值合并、平均值合并以及随机合并。
- 最后一个下采样层（或卷积层）通常连接到一个或多个全连层，全连层的输出就是最终的输出。
- 训练过程通过改进的反向传播实现，将下采样层作为考虑的因素并基于所有值来更新卷积滤波器的权重。
　　可以[在这](https://github.com/ivan-vasilev/neuralnetworks/blob/9e569aa7c9a4d724cf3c1aed8a8036af272ec58f/nn-samples/src/test/java/com/github/neuralnetworks/samples/test/MnistTest.java)看几个应用在 [MNIST](http://en.wikipedia.org/wiki/MNIST_database) 数据集上的卷积网络的例子，[在这](http://cs.stanford.edu/people/karpathy/convnetjs/demo/mnist.html)还有一个用JavaScript实现的一个可视的类似网络。
............................................
## 实现
　　目前为止，我们已经学会了常见神经网络中最主要的元素了，但是我只写了很少的在实现过程中所遇到的挑战。
　　概括来讲，我的目标是实现一个[深度学习的库](https://github.com/ivan-vasilev/neuralnetworks)，即一个基于神经网络且满足如下条件的框架：　　　　
- - 一个可以表示多种模型的通用架构（比如所有上文提到的神经网络中的元素）
- 可以使用多种训练算法（反向传播，对比差异等等）。
- 体面的性能
　　为了满足这些要求，我在软件的设计中使用了分层的思想。
### 结构
　　我们从如下的基础部分开始：
- - [NeuralNetworkImpl](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/architecture/NeuralNetworkImpl.java) 是所有神经网络模型实现的基类。
- 每个网络都包含有一个 [layer](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/architecture/Layer.java) 的集合。
- 每一层中有一个 [connections](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/architecture/ConnectionsImpl.java) 的链表， connection 指的是两个层之间的连接，将整个网络构成一个有向无环图。
　　这个结构对于经典的反馈网络、[RBM](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/architecture/types/RBM.java) 及更复杂的如 [ImageNet](http://www.cs.toronto.edu/~hinton/absps/imagenet.pdf) 都已经足够灵活。
　　这个结构也允许一个 layer 成为多个网络的元素。比如，在 [Deep Belief Network](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/architecture/types/DBN.java)（深度信度网络）中的layer也可以用在其 RBM
 中。
　　另外，通过这个架构可以将DBN的预训练阶段显示为一个栈式RBM的列表，微调阶段显示为一个前馈网络，这些都非常直观而且程序实现的很好。
### 数据流
下个部分介绍网络中的数据流，一个两步过程:
- 定义出层间的序列。例如，为了得到一个多层感知机的结果，输入数据被赋到输入层（因此，这也是首先被计算的层），然后再将数据通过不同的方法流向输出层。为了在反向传播中更新权重，输出的误差通过广度优先的方法从输出层传回每一层。这部分通过 *[LayerOrderStrategy](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/calculation/LayerOrderStrategy.java)*进行实现，应用到了网络图结构的优势，使用了不同的图遍历方法。其中一些样例包含了 [广度优先策略](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/calculation/BreadthFirstOrderStrategy.java) 和 [定位到一个指定的层](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/calculation/TargetLayerOrderStrategy.java)。层的序列实际上由层间的连接进行决定，所以策略部分都是返回一个连接的有序列表。
- 计算激活值。每一层都有一个关联的 *[ConnectionCalculator](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/calculation/ConnectionCalculator.java)*，包含有连接的列表（从上一步得来）和输入值（从其它层得到）并计算得到结果的激活值。例如，在一个简单的S形前馈网络中，隐含层的  *ConnectionCalculator* 接受输入层和偏置层的值（分别为输入值和一个值全为1的数组）和神经元之间的权重值（如果是全连接层，权重值实际上以一个矩阵的形式存储在一个 [*FullyConnected*](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/architecture/FullyConnected.java) 结构中，计算加权和，然后将结果传给S函数。*ConnectionCalculator* 中实现了一些转移函数（如加权求和、卷积）和激活函数（如对应多层感知机的对数函数和双曲正切函数，对应RBM的二态函数）。其中的大部分都可以通过 [Aparapi](https://code.google.com/p/aparapi/) 在GPU上进行计算，可以利用迷你批次训练。
#### 通过 Aparapi 进行 GPU 计算
　　像我之前提到的，神经网络在近些年复兴的一个重要原因是其训练的方法可以高度并行化，允许我们通过GPGPU高效的加速训练。本文中，我选择 [Aparapi](https://code.google.com/p/aparapi/) 库来进行GPU的支持。
　　Aparapi 在连接计算上强加了一些重要的限制：
- 只允许使用原始数据类型的一维数组（变量）。
- 在GPU上运行的程序只能调用 Aparapi *Kernel* 类本身的成员函数。
　　这样，大部分的数据（权重、输入和输出数据）都要保存在 [*Matrix*](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/architecture/Matrix.java) 实例里面，其内部是一个一维浮点数组。所有Aparapi 连接计算都是使用 [*AparapiWeightedSum*](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/calculation/neuronfunctions/AparapiWeightedSum.java) （应用在全连接层和加权求和函数上）、 [*AparapiSubsampling2D*](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/calculation/neuronfunctions/AparapiSubsampling2D.java) （应用在下采样层）或 [*AparapiConv2D*](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/calculation/neuronfunctions/AparapiConv2D.java) （应用在卷积层）。这些限制可以通过 [Heterogeneous
 System Architecture](http://developer.amd.com/resources/heterogeneous-computing/what-is-heterogeneous-system-architecture-hsa/)　里介绍的内容解决一些。而且Aparapi 允许相同的代码运行在CPU和GPU上。
### 训练
[training](https://github.com/ivan-vasilev/neuralnetworks/tree/master/nn-core/src/main/java/com/github/neuralnetworks/training) 的模块实现了多种训练算法。这个模块依赖于上文提到的两个模块。比如，[*BackPropagationTrainer*](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/training/backpropagation/BackPropagationTrainer.java) （所有的训练算法都以 [*Trainer*](https://github.com/ivan-vasilev/neuralnetworks/blob/master/nn-core/src/main/java/com/github/neuralnetworks/training/Trainer.java) 为基类）在前馈阶段使用前馈层计算，在误差传播和权重更新时使用特殊的广度优先层计算。
　　我最新的工作是在Java8环境下开发，其它一些更新的功能可以在这个[branch](https://github.com/ivan-vasilev/neuralnetworks/tree/java8) 下获得，这部分的工作很快会merge到主干上。
## 结论
　　本文的目标是提供一个深度学习算法领域的一个简明介绍，由最基本的组成元素开始（感知机）并逐渐深入到多种当前流行且有效的架构上，比如受限波尔兹曼机。
　　神经网络的思想已经出现了很长时间，但是今天，你如果身处机器学习领域而不知道深度学习或其它相关知识是不应该的。不应该过度宣传，但不可否认随着GPGPU提供的计算能力、包括Geoffrey Hinton, Yoshua Bengio, Yann LeCun and Andrew Ng在内的研究学者们提出的高效算法，这个领域已经表现出了很大的希望。现在正是最佳的时间深入这些方面的学习。
## 附录：相关资源
　　如果你想更深入的学习，下面的这些资源在我的工作当中都起过重要的作用：
- [DeepLearning.net](http://deeplearning.net/): 深度学习所有方面知识的一个门户。里面有完善的[手册](http://deeplearning.net/reading-list/tutorials/)、[软件库](http://deeplearning.net/software_links/) 和一个非常好的 [阅读列表](http://deeplearning.net/reading-list/)。
- 活跃的 [Google+](https://plus.google.com/u/0/communities/112866381580457264725)社区.
- 两个很好的课程: [Machine Learning](https://www.coursera.org/course/ml) and [Neural Networks for Machine Learning](https://www.coursera.org/course/neuralnets), 都在Coursera上。
- The [Stanford neural networks tutorial](http://www.toptal.com/machine-learning/ufldl.stanford.edu/wiki/index.php/UFLDL_Tutorial)，斯坦福神经网络指南。
- 
**后记：**
       很期待作者的库，在这里转载一下，省的作者有一天删除了........

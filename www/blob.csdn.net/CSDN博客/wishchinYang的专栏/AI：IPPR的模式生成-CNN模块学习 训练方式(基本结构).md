# AI：IPPR的模式生成-CNN模块学习/训练方式(基本结构) - wishchinYang的专栏 - CSDN博客
置顶2017年07月23日 22:02:14[wishchin](https://me.csdn.net/wishchin)阅读数：3939
**前言：**
        一个完备的模式识别系统，必然包含一个模式识别模型，此外还要附加一个评价模型，以及为了构建识别模型而构建的学习模型，并选择在学习模型中使用的学习方法。
**       表示（Representation）　**：    一个分类器必须用计算机可以处理的某种形式语言来表示。反过来讲，为模式识别模型选择一种表示，就意味选择一个特定的分类器集合。学习器可能学出的分类器只能在这个集合中。这个集合被称为学习器的假设空间（hypothesis space）。如果某个分类器不在该空间中，它就不可能被该学习器学到。与此相关的一个问题是如何表示输入，即使用哪些特征，本文稍后介绍。
**       评价（Evaluation）**：        我们需要一个评价函数（亦称为目标函数或打分函数）来判断分类器的优劣。机器学习算法内部使用的评价函数和我们希望分类器进行优化的外部评价函数有所不同。这是为了便于优化和最终评价。
**      学习/训练模型（Learning）**：       可表示的模式识别模型，若想得到最优结果，也应该是可学习的。我们需要一个构建模式识别模型的方法，即学习/训练模型。用于构建学习出模式识别模型。
**      优化（Optimization）-学习方法   **：      最后，我们需要一个搜索方法，能够在假设空间中找到评价函数得分最高的那个分类器。
      机器学习模型不仅要求模型是可表示的，而且是可学习的。传统方法要求可以通过明确的函数范畴的子集合来表示模型，且一定能根据函数本身获得一般的函数参数优化方式，即学习方法。
      对于预先选择的一个模式识别表示方法，会有配套的整个模式识别系统。一旦选定某个分类器，则应选用对应的训练模型，使用适合的学习方法，并使用相应的评价方法，逐步优化模型到最优。参考文章：[PAC机器学习理论：机器学习那些事](http://blog.csdn.net/wishchin/article/details/53634396)
      参考文章：[模式识别两种方法：知识和数据](http://write.blog.csdn.net/postedit/22654357)
**模式识别的专家系统**
        模式识别的传统意义的专家系统，是由人类专家构建知识，使用谓词逻辑，构建的超大型知识图谱，并由已知图谱推导直至遍历路径和节点。专家问题的既然是人类专家获取的知识，必然有其粒度划分范围的限制，在某些情况下，推演可能会导致矛盾：不同的路径会产生矛盾-相反的结果。或者粒度较大时，问题空间不能被遍历，导致无法解析。
        风热和风寒的症状区别是什么？专家根据个人经验列出一个表格，给出什么症状是风热什么是风寒，以至于以后什么症状出现，根据推理规则推理出风热还是风寒，规则是明确的，准确度取决于粒度。
        专家系统出问题的根源在于粒度，若不到原子层，定义域集合元素的粒度再小规则也是一个非确定性规则，即概率规则。这就引出了，使用概率规则的模式识别方法—机器学习方法。
**模式识别的机器学习方法**
        机器学习系统是一个专家系统，与传统专家系统的区别在于规则构建的方式。传统机器学习由专家构建规则，而机器学习系统由人类专家设置特定边界或等价形式，由数据来构建规则。比如，一个医疗专家系统给出了整个表象的陈述（知识节点），并构建出详细逻辑关系（推理路径-因果关系），而机器学习系统给出知识的边界，设定或不设定映射结构，由数据来构建出逻辑关系。
        风热和风寒的区别是什么？机器学习会找很多人给标注一下，那些症状是风热，那些是风寒，然后设定一个样本边界或者映射限制，使用预测-反馈的方法，反复调整推理，构建推理路径的具体参数。准确度取决于函数的参数密度。
**机器学习方法的判别模型**
       ML方法划分出示例学习和非示例学习，对应了监督学习和非监督学习。示例学习需要给出示例，即人工标记，根据已知关系得到规则，叫做归纳，而根据已知关系推到未知关系，叫做演绎。示例学习根据规则的表述形式不同，又划分为弱学习模型和强学习模型。弱学习模型一般直接对应了近邻方法，相似即相近，此种方法一般进行系统判别时需要一定程度的遍历，此种方法有KNN,决策树模型等；强学习模型把系统描述为可遍历系统的等价形式，以避免判别时必须的遍历过程。
**机器学习中的归纳方式**
        根据粒度进行划分，划分的规则即为归纳方式。
        ..........................
**模式映射的形成-监督训练**
       参数化的函数映射，必须有一个复合条件的最优形式，在固定模型里面表现为最优参数，模型参数调优使用监督训练的方法。
**Bayes方法**
        贝叶斯方法是所见即所得的直观描述形式，统计样本的后验概率，根据条件概率来推到先验概率的一般形式，bayes方法的错误率是泛用的，是专家归纳知识的错误极限，没有任何实际意义。
        Markov方法是一种多层贝叶斯网络的方法..............
**参数模型方法的训练方式——残差反馈/反向传播**
        归纳方法的规则终点表述为最优映射，描述为最优参数。通用方法是计算预测标签 f(x)与 标定标签Yi的差值 ,获取差值，反馈调整模型参数。常用的ML算法的残差反馈—模型优化方法如下图，为一层反向传播的各种表现形式。
![](https://img-blog.csdn.net/20170723143037874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        支持向量机把残差问题再次转化为近邻查找问题，使用[二维搜索](http://blog.csdn.net/wishchin/article/details/52861709)寻找最优支持向量之间的超平面，其中SMO方法直接使用了轮换柱搜索方法，并用支持向量表示模式函数。逻辑斯特递归等线性判别方法把函数优化问题转化为一个残差最小化问题，即最小二乘问题，可通过解析方程的方法得到解析最优参数。
[贝叶斯网络和CRF方法](http://blog.csdn.net/wishchin/article/details/50351616)的训练方法为使用遍历搜索的方法，寻找最大概率路径，有有名的前向后向算法、维比特算法。
        感知器算法作为一类简单的线性分类器，使用了近似的反向传播算法。以训练样本被错分的程度为目标函数，训练中每次出现错误时，便使权系数朝着目标函数相对于权系数负梯度的方向更新，直到目标函数取得极小值即没有训练样本被错分，即梯度下降法的核心思想。
**反向传播**：预测函数与标签差值反馈到权系数，对权系数进行更新，是为反向传播。不断调整参数，直至预测函数误差最小或者达到学习准则约束，误差不再反传。
### **CNN网络参数反传**
         CNN方法使用CNN结构，使用卷积核的概念，使用广泛的数据调整卷积核的参数，训练得到卷积核，来完成适用于特定数据集的特征提取。理论上效果应该超过广泛使用的特征提取方法比如SIFT和HOG特征，而事实上对于大量数据也是如此。那么CNN的进行逐层训练，也有相应的残差反向传播的方法。
          ........................
**一、SoftMax函数层反传参数更新**
        学习页面：[http://ufldl.stanford.edu/wiki/index.php/Softmax](http://ufldl.stanford.edu/wiki/index.php/Softmax%E5%9B%9E%E5%BD%92)
        参考文章：[详解SoftMax函数极其求导过程](https://zhuanlan.zhihu.com/p/25723112)
        softmax函数作为最后一层输出层，也是第一层反传层，若使用交叉熵作为损失函数，有较为**巧妙的**参数更新方式。
        计算模型：
![](https://img-blog.csdn.net/20170813212016781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       反传模型：
![](https://img-blog.csdn.net/20170813213055722?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         对任一标记样本（Xi，yi），若模式函数映射为（Xi，f(x)'）
        假设我们有一个数组，V，Vi表示V中的第i个元素，那么这个元素的softmax值就是    ![](https://pic4.zhimg.com/v2-65035de6fdfd8b2f13b930191e9a548b_b.png)
        选用损失函数为交叉熵函数形式如下：![Loss = -\sum_{i}^{}{y_{i}lna_{i}  }](http://www.zhihu.com/equation?tex=Loss+%3D+-%5Csum_%7Bi%7D%5E%7B%7D%7By_%7Bi%7Dlna_%7Bi%7D++%7D+)，交叉熵是用来衡量我们的预测的低效性。
        其中y代表我们的真实值，a代表我们softmax求出的值。j是w的函数，所以整个代价函数是w的函数，要想求最小就要对w求偏导，利用链式求导法则[推导过程看这里](https://math.stackexchange.com/questions/945871/derivative-of-softmax-loss-function)求导。i代表的是输出结点的标号！
**        这里认为训练数据的真实输出为第j个为1，其它均为0！**那么Loss就变成了![Loss = -y_{j}lna_{j}](http://www.zhihu.com/equation?tex=Loss+%3D+-y_%7Bj%7Dlna_%7Bj%7D++),累和已经去掉**。**
        其中![y_{j} =1](http://www.zhihu.com/equation?tex=y_%7Bj%7D+%3D1),那么形式变为![Loss = -lna_{j}](http://www.zhihu.com/equation?tex=Loss+%3D+-lna_%7Bj%7D++)
**最后更新规则**就是：
               当上层节点p=类别节点j时， w=w−1
               否则 ， w=w
        这样，**每个神经的权值向量的更新要么减1，要么不变**，就不用受激活函数的制约了。
**        举个例子，**通过若干层的计算，最后得到的某个训练样本的向量的分数是[ 2, 3, 4 ], 那么经过softmax函数作用后概率分别就是=[![\frac{e^{2} }{e^{2}+e^{3}+e^{4}}](http://www.zhihu.com/equation?tex=%5Cfrac%7Be%5E%7B2%7D+%7D%7Be%5E%7B2%7D%2Be%5E%7B3%7D%2Be%5E%7B4%7D%7D+),![\frac{e^{3} }{e^{2}+e^{3}+e^{4}}](http://www.zhihu.com/equation?tex=%5Cfrac%7Be%5E%7B3%7D+%7D%7Be%5E%7B2%7D%2Be%5E%7B3%7D%2Be%5E%7B4%7D%7D+),![\frac{e^{4} }{e^{2}+e^{3}+e^{4}}](http://www.zhihu.com/equation?tex=%5Cfrac%7Be%5E%7B4%7D+%7D%7Be%5E%7B2%7D%2Be%5E%7B3%7D%2Be%5E%7B4%7D%7D+)]  =  [0.0903,0.2447,0.665], 如果这个样本正确的分类是第二个的话，那么计算出来的偏导就是[0.0903,0.2447-1,0.665]=[0.0903,-0.7553,0.665]，是不是非常简单！！然后再根据这个进行back propagation就可以了。
**二、ANN的[多层反向传播](http://blog.csdn.net/wishchin/article/details/45067177)——全连接层BP算法**
        ANN一般使用非线性激活函数构建多层感知器模型，进而可以理论上拟合任意的非线性映射。
       多层感知机的监督式训练的算法为反向传播算法。基本的流程如下：
　　1、将训练样本通过神经网络进行前向传播计算。
　　2、计算输出误差，常用均方差：
![](http://images.cnitblog.com/i/629033/201404/302128239083193.png)
　　　其中 *t* 是目标值， *y* 是实际的神经网络计算输出。其它的误差计算方法也可以，但MSE（均方差）通常是一种较好的选择。
![](http://images.cnitblog.com/i/629033/201404/302136275333710.png)
　　3、网络误差通过 [随机梯度下降](http://en.wikipedia.org/wiki/Stochastic_gradient_descent)/梯度下降法 的方法来寻找最小值。
　　梯度下降很常用，但在神经网络中，输入参数是一个训练误差的曲线。每个权重的最佳值应该对应误差曲线中的全局最小值（上图中的 *global minimum*）。在训练过程中，权重以特定的步幅改变（在每个样本或每小组样本训练完成后）以找到全局最小值，但这可不容易，训练通常会结束在局部最小值上（上图中的local minima）。如例子中的，如果当前权重值为0.6，那么要向0.4方向移动。
　　 这个图表示的是最简单的情况，误差只依赖于单个参数。但是，网络误差依赖于每一个网络权重，误差函数非常、非常复杂。
　　 好消息是反向传播算法提供了一种通过利用输出误差来修正两个神经元之间权重的方法。关系本身十分复杂，但对于一个给定结点的权重修正按如下方法（简单）：
![](http://images.cnitblog.com/i/629033/201404/302159245645034.png)
　　 其中 *E* 是输出误差， *w_i* 是输入 *i* 的权重。
　　 实质上这么做的目的是利用权重 *i*来修正梯度的方向。关键的地方在于误差的导数的使用，这可不一定好计算：你怎么样能给一个大型网络中随机一个结点中的随机一个权重求导数呢？
　　 答案是：通过反向传播。误差的首次计算很简单（只要对预期值和实际值做差即可），然后通过一种巧妙的方法反向传回网络，让我们有效的在训练过程中修正权重并（期望）达到一个最小值。
       4.** 反传函数**：起初的ANN普遍使用非线性激活函数sigmod函数，
![](http://images.cnitblog.com/i/629033/201404/302159245645034.png)
         从∂/wi 里面可以分离出sigmod函数的导数∂f(x)，即 △wi = -a*∂f(x)*K，用于前一层的参数更新。
         参考：[聊一聊深度学习的激活函数](https://zhuanlan.zhihu.com/p/25110450)
![](https://img-blog.csdn.net/20170724110248512?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         图片来之知乎：  [DNN activation function的具体作用是什么](https://www.zhihu.com/question/29021768)？
         对激活函数的导数∂f(x)反传，查看sigmod函数可以看出，在输入为X>一特定值时，其偏导数接近于0，这便是饱和特性，梯度丢失导致残差无法反传用于上一层的参数更新。而ReLU函数没有饱和特性。且ReLU函数在计算量上小于sigmod函数。目前的2016年2017年，ReLU由于其理论和实验优势，一般被用作默认激活函数。
**三、[CNN卷积-池化层误差反传——旋转卷积核](http://blog.csdn.net/zy3381/article/details/44409535)**
        此段请拜访上述标题中的原文链接...........
        特意提取从卷基层到池化层.......卷积层是CNN特征提取哈希层保持旋转不变性的根源，通过**调整卷积核的参数**来实现对图像块主方向的确定，反传时要旋转卷积核![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)。
        注意：此处反传计算输入的是误差矩阵；
**CNN由卷积层到池化层**
        有一个非常关键的的步骤就是将某个卷积(Convolve)层的误差传到前一层的池化(Pool)层上，方法是：**反向传播的时候要把卷积核旋转180°，并采用full的形式来进行卷积运算**。
       因为在CNN中是2D反传，与传统神经网络中的1D反传有点细节上的不同，下面通过一个简单的例子来详细分解一下这个反传步骤。
       假设在一个CNN网络中，P代表某个池化层，K代表卷积核，C代表卷基层，首先来看一下前向(feed forward)计算，从一个池化层经过与卷积核(Kernel)的运算得到卷积层:
![](https://img-blog.csdn.net/20150318140845781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenkzMzgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       将前向计算的步骤进行分解，可以得到以下公式：
![](https://img-blog.csdn.net/20150318140931232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenkzMzgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       下面根据这个前向计算的步骤来分解出反向传播的步骤：
![](https://img-blog.csdn.net/20150318141056642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenkzMzgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       首先要确定误差传播的目的地，从deltaC到deltaP，所以先从deltaP1开始分析
       从前面的前向计算过程中可以找出P1参与了C中哪些元素的计算，并且可以根据对应的前向计算得出反向传播的计算公式：
![](https://img-blog.csdn.net/20150318141526686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenkzMzgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       依次类推，还有如下公式：
       对于P2
![](https://img-blog.csdn.net/20150318141523389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenkzMzgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       对于P3
![](https://img-blog.csdn.net/20150318141530502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenkzMzgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       对于P4
![](https://img-blog.csdn.net/20150318141538021?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenkzMzgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       对于P5
![](https://img-blog.csdn.net/20150318140911340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenkzMzgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       一直可以推到P9.
       总结这9个反向传播的公式到一起：
![](https://img-blog.csdn.net/20150318141746724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenkzMzgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       进一步可以发现，**这9个公式可以用如下的卷积过程来实现**：
![](https://img-blog.csdn.net/20150318141719205?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenkzMzgx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        至此，从计算的细节上解释了为什么**反向传播的时候要把卷积核旋转180°，并采用full的形式来进行卷积运算**。
        (注：上文所说的“卷积”被认为是一种不会180°旋转第二个因子的的计算过程，实际上matlab中的的conv2(a,b)会自动180°旋转b，换句话说，在matlab中实现这个步骤的时候不用提前旋转，留给conv2函数自行旋转即可)
**四、综述**：[**DL论文笔记之四-CNN推导和实现**](http://blog.csdn.net/zouxy09/article/details/9993371)
**        以下为全部摘抄，此段请拜访上述原文链接..........**
**        应该参考：****[DL-CNN的反向传导和练习](http://www.cnblogs.com/tornadomeet/p/3468450.html)**
**        还有这个大杂烩：[DL-DL模型之CNN总结](http://blog.csdn.net/u010555688/article/details/38780807)**
**全连接的反向传播算法**
         典型的CNN中，开始几层都是卷积和下采样的交替，然后在最后一些层（靠近输出层的），都是全连接的一维网络。这时候我们已经将所有两维2D的特征maps转化为全连接的一维网络的输入。这样，当你准备好将最终的2D特征maps输入到1D网络中时，一个非常方便的方法就是把所有输出的特征maps连接成一个长的输入向量。然后我们回到BP算法的讨论。（更详细的基础推导可以参考UFLDL中“[反向传导算法](http://deeplearning.stanford.edu/wiki/index.php/%E5%8F%8D%E5%90%91%E4%BC%A0%E5%AF%BC%E7%AE%97%E6%B3%95)”）。
**2.1、Feedforward Pass前向传播**
         在下面的推导中，我们采用平方误差代价函数。我们讨论的是多类问题，共c类，共N个训练样本。
![](https://img-blog.csdn.net/20130816002706875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         这里![](https://img-blog.csdn.net/20130816002717187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表示第n个样本对应的标签的第k维。![](https://img-blog.csdn.net/20130816002727875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)表示第n个样本对应的网络输出的第k个输出。对于多类问题，输出一般组织为“one-of-c”的形式，也就是只有该输入对应的类的输出节点输出为正，其他类的位或者节点为0或者负数，这个取决于你输出层的激活函数。sigmoid就是0，tanh就是-1.
         因为在全部训练集上的误差只是每个训练样本的误差的总和，所以这里我们先考虑对于一个样本的BP。对于第n个样本的误差，表示为：
![](https://img-blog.csdn.net/20130816002745906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        传统的全连接神经网络中，我们需要根据BP规则计算代价函数E关于网络每一个权值的偏导数。我们用l来表示当前层，那么当前层的输出可以表示为：
![](https://img-blog.csdn.net/20130816002804250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         输出激活函数f(.)可以有很多种，一般是sigmoid函数或者双曲线正切函数。sigmoid将输出压缩到[0, 1]，所以最后的输出平均值一般趋于0。所以如果将我们的训练数据归一化为零均值和方差为1，可以在梯度下降的过程中增加收敛性。对于归一化的数据集来说，双曲线正切函数也是不错的选择。
**2.2、Backpropagation Pass反向传播**
         反向传播回来的误差可以看做是每个神经元的基的**灵敏度sensitivities**（ 灵敏度的意思就是我们的基b变化多少，误差会变化多少，也就是误差对基的变化率，也就是导数了），定义如下：（第二个等号是根据求导的链式法则得到的）
![](https://img-blog.csdn.net/20130816002850203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         因为∂u/∂b=1，所以∂E/∂b=∂E/∂u=δ，也就是说bias基的灵敏度∂E/∂b=δ和误差E对一个节点全部输入u的导数∂E/∂u是相等的。这个导数就是让高层误差反向传播到底层的神来之笔。反向传播就是用下面这条关系式：（下面这条式子表达的就是第l层的灵敏度，就是）
![](https://img-blog.csdn.net/20130816002937765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)公式（1）
         这里的“◦”表示每个元素相乘。输出层的神经元的灵敏度是不一样的：
![](https://img-blog.csdn.net/20130816002950875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)** 公式1.X**
         最后，对每个神经元运用delta（即δ）规则进行权值更新。具体来说就是，对一个给定的神经元，得到它的输入，然后用这个神经元的delta（即δ）来进行缩放。用向量的形式表述就是，对于第l层，误差对于该层每一个权值（组合为矩阵）的导数是该层的输入（等于上一层的输出）与该层的灵敏度（该层每个神经元的δ组合成一个向量的形式）的叉乘。然后得到的偏导数乘以一个负学习率就是该层的神经元的权值的更新了：
![](https://img-blog.csdn.net/20130816003002437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)** 公式（****2）**
         对于bias基的更新表达式差不多。实际上，对于每一个权值(W)ij都有一个特定的学习率ηIj。
**3、Convolutional Neural Networks 卷积神经网络**
**3.1、Convolution Layers卷积层**
        我们现在关注**网络中卷积层的BP更新**。在一个卷积层，上一层的特征maps被一个可学习的卷积核进行卷积，然后通过一个**激活函数**，就可以得到输出特征map。每一个输出map可能是组合卷积多个输入maps的值：
![](https://img-blog.csdn.net/20130816003036562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这里Mj表示选择的输入maps的集合，那么到底选择哪些输入maps呢？有选择一对的或者三个的。但下面我们会讨论如何去自动选择需要组合的特征maps。每一个输出map会给一个额外的偏置b，但是对于一个特定的输出map，卷积每个输入maps的卷积核是不一样的。
也就是说，如果输出特征map j和输出特征map k都是从输入map i中卷积求和得到，那么对应的卷积核是不一样的。
**3.1.1、Computing the Gradients梯度计算**
         我们假定每个卷积层l都会接一个下采样层l+1。对于BP来说，根据上文我们知道，要想求得层l的每个神经元对应的权值的权值更新，就需要先求层l的每一个神经节点的灵敏度δ（也就是权值更新的公式（2））。为了求这个灵敏度我们就需要先对下一层的节点（连接到当前层l的感兴趣节点的第l+1层的节点）的灵敏度求和（得到δl+1），然后乘以这些连接对应的权值（连接第l层感兴趣节点和第l+1层节点的权值）W。再乘以当前层l的该神经元节点的输入u的激活函数f的导数值（也就是那个灵敏度反向传播的公式（1）的δl的求解），这样就可以得到当前层l每个神经节点对应的灵敏度δl了。
         然而，因为下采样的存在，采样层的一个像素（神经元节点）对应的灵敏度δ对应于卷积层（上一层）的输出map的一块像素（采样窗口大小）。因此，层l中的一个map的每个节点只与l+1层中相应map的一个节点连接。
         为了有效计算层l的灵敏度，我们需要上采样upsample这个下采样downsample层对应的灵敏度map（特征map中每个像素对应一个灵敏度，所以也组成一个map），这样才使得这个灵敏度map大小与卷积层的map大小一致，然后再将层l的map的激活值的偏导数与从第l+1层的上采样得到的灵敏度map逐元素相乘（也就是公式（1））。
         在下采样层map的权值都取一个相同值β，而且是一个常数。所以我们只需要将上一个步骤得到的结果乘以一个β就可以完成第l层灵敏度δ的计算。
        我们可以对卷积层中每一个特征map j重复相同的计算过程。但很明显需要匹配相应的子采样层的map（参考公式（1））：
![](https://img-blog.csdn.net/20130816003129515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         up(.)表示一个上采样操作。如果下采样的采样因子是n的话，它简单的将每个像素水平和垂直方向上拷贝n次。这样就可以恢复原来的大小了。实际上，这个函数可以用**Kronecker乘积**来实现：
![](https://img-blog.csdn.net/20130816003155078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
          好，到这里，对于一个给定的map，我们就可以计算得到其灵敏度map了。然后我们就可以通过简单的对层l中的灵敏度map中所有节点进行求和快速的计算bias基的梯度了：
![](https://img-blog.csdn.net/20130816003207265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**公式（3）**
         最后，对**卷积核的权值的梯度就可以用BP算法**来计算了（公式（2））。另外，很多连接的权值是共享的，因此，对于一个给定的权值，我们需要对所有与该权值有联系（权值共享的连接）的连接对该点求梯度，然后对这些梯度进行求和，就像上面对bias基的梯度计算一样：
![](https://img-blog.csdn.net/20130816003232171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这里，![](https://img-blog.csdn.net/20130816003313265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)是![](https://img-blog.csdn.net/20130816003318046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)中的在卷积的时候与![](https://img-blog.csdn.net/20130816003328953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)逐元素相乘的patch，输出卷积map的(u, v)位置的值是由上一层的(u, v)位置的patch与卷积核k_ij逐元素相乘的结果。
        咋一看，好像我们需要煞费苦心地记住输出map（和对应的灵敏度map）每个像素对应于输入map的哪个patch。但实际上，在Matlab中，可以通过一个代码就实现。对于上面的公式，可以用Matlab的卷积函数来实现：
![](https://img-blog.csdn.net/20130816003352281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        我们先对delta灵敏度map进行旋转，这样就可以进行**互相关计算**，而不是卷积（在卷积的数学定义中，特征矩阵（卷积核）在传递给conv2时需要先翻转（flipped）一下。也就是颠倒下特征矩阵的行和列）。然后把输出反旋转回来，这样我们在前向传播进行卷积的时候，卷积核才是我们想要的方向。
**3.2、Sub-sampling Layers子采样层**
         对于子采样层来说，有N个输入maps，就有N个输出maps，只是每个输出map都变小了。
![](https://img-blog.csdn.net/20130816003432812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        down(.)表示一个下采样函数。典型的操作一般是对输入图像的不同nxn的块的所有像素进行求和。这样输出图像在两个维度上都缩小了n倍。每个输出map都对应一个属于自己的乘性偏置β和一个加性偏置b。
**3.2.1、Computing the Gradients梯度计算**
         这里最困难的是计算灵敏度map。一旦我们得到这个了，那我们唯一需要更新的偏置参数β和b就可以轻而易举了（公式（3））。如果下一个卷积层与这个子采样层是全连接的，那么就可以通过BP来计算子采样层的灵敏度maps。
         我们需要计算卷积核的梯度，所以我们必须找到输入map中哪个patch对应输出map的哪个像素。这里，就是必须找到当前层的灵敏度map中哪个patch对应与下一层的灵敏度map的给定像素，这样才可以利用公式（1）那样的δ递推，也就是灵敏度反向传播回来。另外，需要乘以输入patch与输出像素之间连接的权值，这个权值实际上就是卷积核的权值（已旋转的）。
![](https://img-blog.csdn.net/20130816003501218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
       在这之前，我们需要先将核旋转一下，让卷积函数可以实施互相关计算。另外，我们需要对卷积边界进行处理，但在Matlab里面，就比较容易处理。Matlab中全卷积会对缺少的输入像素补0。
       到这里，我们就可以对b和β计算梯度了。首先，加性基b的计算和上面卷积层的一样，对灵敏度map中所有元素加起来就可以了：
![](https://img-blog.csdn.net/20130816003539109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        而对于乘性偏置β，因为涉及到了在前向传播过程中下采样map的计算，所以我们最好在前向的过程中保存好这些maps，这样在反向的计算中就不用重新计算了。我们定义：
![](https://img-blog.csdn.net/20130816003553468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样，对β的梯度就可以用下面的方式计算：
![](https://img-blog.csdn.net/20130816003605718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**3.3、Learning Combinations of Feature Maps学习特征map的组合**
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
**3.3.1、Enforcing Sparse Combinations加强稀疏性组合**
          为了限制αi是稀疏的，也就是限制一个输出map只与某些而不是全部的输入maps相连。我们在整体代价函数里增加稀疏约束项Ω(α)。对于单个样本，重写代价函数为：
![](https://img-blog.csdn.net/20130816003835515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
       然后寻找这个规则化约束项对权值ci求导的贡献。规则化项Ω(α)对αi求导是：
![](https://img-blog.csdn.net/20130816003848406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         然后，通过链式法则，对ci的求导是：
![](https://img-blog.csdn.net/20130816003914937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
         所以，权值ci最后的梯度是：
![](https://img-blog.csdn.net/20130816003929468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**3.4、Making it Fast with MATLAB**
        CNN的训练主要是在**卷积层和子采样层的交互上**，其主要的计算瓶颈是：
1）前向传播过程：下采样每个卷积层的maps；
2）反向传播过程：上采样高层子采样层的灵敏度map，以匹配底层的卷积层输出maps的大小；
3）sigmoid的运用和求导。
         对于第一和第二个问题，我们考虑的是如何用Matlab内置的图像处理函数去实现上采样和下采样的操作。对于上采样，imresize函数可以搞定，但需要很大的开销。一个比较快速的版本是使用Kronecker乘积函数kron。通过一个全一矩阵ones来和我们需要上采样的矩阵进行Kronecker乘积，就可以实现上采样的效果。对于前向传播过程中的下采样，imresize并没有提供在缩小图像的过程中还计算nxn块内像素的和的功能，所以没法用。一个比较好和快速的方法是用一个全一的卷积核来卷积图像，然后简单的通过标准的索引方法来采样最后卷积结果。例如，如果下采样的域是2x2的，那么我们可以用2x2的元素全是1的卷积核来卷积图像。然后再卷积后的图像中，我们每个2个点采集一次数据，y=x(1:2:end,1:2:end)，这样就可以得到了两倍下采样，同时执行求和的效果。
         对于第三个问题，实际上有些人以为Matlab中对sigmoid函数进行inline的定义会更快，其实不然，Matlab与C/C++等等语言不一样，Matlab的inline反而比普通的函数定义更费时间。所以，我们可以直接在代码中使用计算sigmoid函数及其导数的真实代码。
**总结**
        此文主要描述了基本CNN的反向传播过程。包括作为分类器的SoftMax层和FCN层，以及作为特征提取的卷积层和Pooling层的参数更新。

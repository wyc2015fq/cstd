# VC维与DNN的Boundary - wishchinYang的专栏 - CSDN博客
2015年04月23日 23:19:34[wishchin](https://me.csdn.net/wishchin)阅读数：1743
原文链接：[解读机器学习基础概念：VC维来去](http://dataunion.org/14581.html)
        作者：[vincentyao](http://www.flickering.cn/author/vincentyao/)
**目录：**
- 说说历史
- Hoeffding不等式
- Connection to Learning
- 学习可行的两个核心条件
- Effective Number of Hypotheses
- Growth Function
- Break Point与Shatter
- VC Bound
- VC dimension
- 深度学习与VC维
- 小结
- 参考文献
        VC维在机器学习领域是一个很基础的概念，它给诸多机器学习方法的可学习性提供了坚实的理论基础，但有时候，特别是对我们工程师而言，SVM，LR，深度学习等可能都已经用到线上了，但却不理解VC维。
        这里，在台湾大学[机器学习基石](https://www.coursera.org/course/ntumlone)课程的基础上，我们简单聊聊“VC维的来龙去脉”。我们将解决以下问题：为什么某机器学习方法是可学习的？为什么会有过拟合？拿什么来衡量机器学习模型的复杂度？深度学习与VC维的关系？
## 说说历史
        在讲VC维之前，我们不妨来说说VC维的历史。而说起VC维的历史，又不得不提起神经网络，一方面是因为神经网络与VC维的发明过程是交织在一起的，另一方面是由于神经网络乏善可陈的泛化控制方法，深度学习在理论基础上一直被怀疑，甚至神经网络和VC维的代表SVM还一起争风吃醋过好多年。
       1943年，模拟神经网络由麦卡洛可（McCulloch）和皮茨（Pitts)提出，他们分析了理想化的人工神经元网络，并且指出了它们进行简单逻辑运算的机制。
        1957年，康奈尔大学的实验心理学家弗兰克·罗森布拉特(Rosenblatt)在一台IBM–704计算机上模拟实现了一种他发明的叫作“感知机”（Perceptron）的神经网络模型。神经网络与支持向量机都源自于感知机（Perceptron）。
       1962年，罗森布拉特著作：《神经动力学原理：感知机和大脑机制的理论》（Principles of Neurodynamics: Perceptrons and the Theory of Brain Mechanisms）。
        1969年，明斯基和麻省理工学院的另一位教授佩普特合作著作：《感知机：计算几何学》（Perceptrons: An Introduction to Computational Geometry)。在书中，明斯基和佩普特证明单层神经网络不能解决XOR（异或）问题。
        1971年，V. Vapnik and A. Chervonenkis在论文“On the uniform convergence of relative frequencies of events to their probabilities”中提出**VC维**的概念。
        1974年，V. Vapnik提出了结构风险最小化原则。
        1974年，沃波斯（Werbos）的博士论文证明了在神经网络多加一层，并且利用“后向传播”（Back-propagation）学习方法，可以解决XOR问题。那时正是神经网络研究的低谷，文章不合时宜。
       1982年，在加州理工担任生物物理教授的霍普菲尔德，提出了一种新的神经网络，可以解决一大类模式识别问题，还可以给出一类组合优化问题的近似解。这种神经网络模型后被称为霍普菲尔德网络。
      1986年，Rummelhart与McClelland发明了神经网络的学习算法Back Propagation。
      1993年，Corinna Cortes和Vapnik等人提出了支持向量机(support vector machine)。神经网络是多层的非线性模型，支持向量机利用核技巧把非线性问题转换成线性问题。
      1992~2005年，SVM与Neural network之争，但被互联网风潮掩盖住了。
       2006年，Hinton提出神经网络的Deep Learning算法。Deep Learning假设神经网络是多层的，首先用**Restricted Boltzmann Machine（非监督学习）学习网络的结构**，然后再**通过Back Propagation（监督学习）学习网络的权值**。
        现在，deep learning的应用越来越广泛，甚至已经有超越SVM的趋势。一方面以Hinton，Lecun为首的深度学习派坚信其有效实用性，另一方面Vapnik等统计机器学习理论专家又坚持着理论阵地，怀疑deep learning的泛化界。
......................................
..............................................
..............................................
## 深度学习与VC维
对于神经网络，其VC维的公式为：
dVC = O(VD)，
其中V表示神经网络中神经元的个数，D表示weight的个数，也就是神经元之间连接的数目。(注意：此式是一个较粗略的估计，深度神经网络目前没有明确的vc bound)
![neural_network_vc_dimension](http://dataunion.org/wp-content/uploads/2015/04/neural_network_vc_dimension.png)
举例来说，一个普通的三层全连接神经网络：input layer是1000维，hidden layer有1000个nodes，output layer为1个node，则它的VC维大约为O(1000*1000*1000)。
可以看到，**神经网络的VC维相对较高，因而它的表达能力非常强**，可以用来处理任何复杂的分类问题。根据上一节的结论，要充分训练该神经网络，所需样本量为10倍的VC维。如此大的训练数据量，是不可能达到的。所以在20世纪，复杂神经网络模型在out of sample的表现不是很好，容易overfit。
但现在为什么深度学习的表现越来越好。原因是多方面的，主要体现在：
- 通过修改神经网络模型的结构，以及提出新的regularization方法，使得神经网络模型的VC维相对减小了。例如卷积神经网络，通过修改模型结构(局部感受野和权值共享)，减少了参数个数，降低了VC维。  2012年的AlexNet，8层网络，参数个数只有60M；而2014年的[GoogLeNet](http://www.cs.unc.edu/~wliu/papers/GoogLeNet.pdf)，22层网络，参数个数只有7M。再例如dropout，drop connect，denosing等regularization方法的提出，也一定程度上增加了神经网络的泛化能力。
- 训练数据变多了。随着互联网的越来越普及，相比于以前，训练数据的获取容易程度以及量和质都大大提升了。训练数据越多，Ein越容易接近于Eout。而且目前训练神经网络，还会用到很多data augmentation方法，例如在图像上，剪裁，平移，旋转，调亮度，调饱和度，调对比度等都使用上了。
- 除此外，pre-training方法的提出，GPU的利用，都促进了深度学习。
但即便这样，深度学习的VC维和VC Bound依旧很大，其泛化控制方法依然没有强理论支撑。但是实践又一次次证明，深度学习是好用的。所以VC维对深度学习的指导意义，目前不好表述，有一种思想建议，深度学习应该抛弃对VC维之类概念的迷信，尝试从其他方面来解释其可学习型，例如使用泛函空间（如[Banach Space](http://en.wikipedia.org/wiki/Banach_space)）中的概率论。
更多细节请参考下面链接：
- [VC Dimension of Multilayer Neural Networks](http://ttic.uchicago.edu/~tewari/lectures/lecture12.pdf)，该文章给出了多层神经网络的VC bound的相关证明。
- [Lecun: What is the relationship between Deep Learning and Support Vector Machines / Statistical Learning Theory?](http://www.kdnuggets.com/2014/02/exclusive-yann-lecun-deep-learning-facebook-ai-lab.html)Vapnik really believes in his bounds. He worried that neural nets didn’t have similarly good ways to do capacity control (although neural nets do have generalization bounds, since they have finite VC dimension).Lecun’s counter argument was that the ability to do capacity control was somewhat secondary to the ability to compute highly complex function with a limited amount of computation.
## 小结
        上面仔细分析了VC维的来龙去脉，讲述了VC维在机器学习理论中的指导意义。考虑到VC维在机器学习领域虽是基础，却也是大坑，所以难免有理解不深或不当之处，敬请谅解。若希望获得更深理解，请参考下面的参考文献。
## 参考文献
- [VC dimension Tutorial Slides by Andrew Moore](http://www.autonlab.org/tutorials/vcdim.html)
- [机器学习基石](https://www.coursera.org/course/ntumlone)[笔记](http://www.douban.com/doulist/3381853/) (上文的截图均出自于该课程的讲义)
- [vc-dimension in svms](http://www.svms.org/vc-dimension/)
- [机器学习简史](http://www.36dsj.com/archives/21236)
- [Vapnik–Chervonenkis theory](http://en.wikipedia.org/wiki/Vapnik%E2%80%93Chervonenkis_theory)
- [Deep Learning Tutorial](http://www.cs.nyu.edu/~yann/talks/lecun-ranzato-icml2013.pdf)
- [深度学习的研究领域是否有被过度夸大](http://www.zhihu.com/question/27434103)
- [VC Theory: Vapnik–Chervonenkis Dimension](http://freemind.pluskid.org/slt/vc-theory-vapnik-chervonenkis-dimension)
**后记：**
** 不管是深度学习还是浅层组合学习，其本质上是人力抽取规则的堆积固化，只是DL的弹性和复用性更强，且融合消融较小，且可以貌似无边界扩充。不可迷恋VC维，系统的完备性不能以底层的参数来表述，因为可操控性极差，远离直观是科学最危险的表达方式。**

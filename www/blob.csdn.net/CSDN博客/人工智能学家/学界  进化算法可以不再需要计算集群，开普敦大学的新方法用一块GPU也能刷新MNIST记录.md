# 学界 | 进化算法可以不再需要计算集群，开普敦大学的新方法用一块GPU也能刷新MNIST记录 - 人工智能学家 - CSDN博客
2017年10月08日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：151
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmjQySWiciaiav8cBy5ahp02drhUlS8wR8dNmOBgZfzsWeGjxiaCZdOWqx9ydmjjsRLtSw47AFuRZsrICw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
*来源：AI科技评论*
*概要：最近这项来自南非开普敦大学的研究就带来了新的发现，只用单块GPU的资源就进化出了理想的网络结构，还刷新了三项测试的结果。*
作者按：进化算法和生成式对抗性网络GANs类似，提出时大家都觉得是很好的想法，可以帮人类极大地拓展行为和想象空间，然而找到好的、可控的实现方法却没那么简单。GANs方面现在已经有了许多的成果，但是进化算法仍然停留在较为初期的状态，无法生成大规模、复杂的网络，需要的计算资源也是在计算集群的级别上。
不过，进化算法的研究也是一步步在“进化”的，最近这项来自南非开普敦大学的研究就带来了新的发现，只用单块GPU的资源就进化出了理想的网络结构，还刷新了三项测试的结果。作者把背景和这篇论文的成果简单介绍如下。
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmjQySWiciaiav8cBy5ahp02drhzYN2so3cwHYl9NOjVQGGcGKWicm4JvGDMwIEQDLlicnM1CDg7yuMGzgg/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
深度神经网络是强大但是反直觉的野兽，要有丰富的经验、反复试错，才能驾驭它们得到好的表现。也有许多例子证明，神经网络的表现会随着深度的增加不断提高。这些特性加上深度学习在各个领域内越来越强的影响力都表明，能自动找到最优或者接近最优的网络结构和超参数的方法越来越重要。
不过显然，网络中每一层的特性、层与层的顺序、激活函数、全连接层里的单元数量、卷积层里卷积核的数量等等建立深度学习网络需要的参数，挑选的时候每一项都不算容易。不过现在有一个好处是，我们有丰富的计算资源，一般来说都可以尝试许多种不同的组合，找到效果比较好的。那有没有可能只用非常有限的计算资源，比如一块GPU，就取得不错的结果呢？
这篇论文就讨论了一种思路，通过进化算法，发展出优秀的神经网络。这样的神经进化算法并不是新鲜事物，大概三十年前就有人提出过，最开始是在一项跟神经网络中的连接权重相关的研究中。
### **神经进化算法的近期成果**
下面先介绍一下神经进化算法方面的近期相关工作，跟这篇论文中的成果相比，它们都用到了很多的计算资源。E. Real 等在“Large-scale evolution of image classifiers”（arXiv:1703.01041, 2017）中提出了一种神经进化的方法，优化用于图像分类的神经网络，其中用到了含有250台计算机的并行计算集群，而在CIFAR图像数据集任务中取得了可观的成果。B.
 Zoph 和 Q. V. Le 则在“Neural architecture search with reinforcement learning”（arXiv:1611.01578, 2016）中用了一个综合了强化学习的循环神经网络，用来学习优秀的架构，他们一共在800个GPU上训练了800个网络。
R. Miikkulainen 等人提出了CoDeepNEAT（“Evolving deep neural networks”，arXiv:1703.00548, 2017），其中产生了一大批模型和蓝图。这些蓝图是由数个节点组成的，节点指向表征了神经网络的特定模块。这样一来，他们所提的方法就让重复结构的进化变得可能，因为蓝图可以重复使用进化后的模块。T. Desell
 提出了 EXACT（“Large scale evolution of convolutional neural networks using volunteer computing” ，arXiv:1703.05422, 2017），这是一种用来部署在分布式计算集群上的神经进化算法，他们当时使用了4500台志愿参与者的计算机，进化出了12万个网络用来搞定MNIST数据集。他们的方法中没有使用池化层，也仅限于使用2维的输入和滤波器。
作者们还发现，近期有研究中只用一个GPU就可以进化出能够准确判断出监督学习任务中需要的是回归模型还是分类模型的深度神经网络，在一系列不同的任务中获得了96%的平均准确率。（E. Dufourq 和 B. A. Bassett, “Automated problem identification: Regression vs classification via
 evolutionary deep networks”，Annual Conference of the South African Institute of Computer Scientists and Information Technologists, ACM, 2017）这项成果是作者们的这篇论文的直接先驱，而且只要有足够的计算资源就可以无缝衔接到这篇论文中的网络优化过程中来。
### **基因算法**
基因算法（genetic algorithm，GA）是一种可以用来解决优化问题的进化算法。首先初始化一群染色体，每个染色体都表征了优化问题的一个解决方案。然后通过一个匹配度函数评价每个染色体，以确定哪个染色体可以解决这个问题。在迭代进化模型中，基因算法会迭代很多次，可以称作“世代”（generations），一直迭代到预定义的条件达成为止（比如最大世代数目）。每一个染色体由很多个基因组成，这些基因就可以用基因操作器进行替换。经过基因操作器操作之后的染色体，就可以称为原来染色体的子代（offspring）。根据染色体群落数目的不同，可以产生多个子代。每个世代中，子代染色体都会代替现有的染色体群落。
这篇论文中，作者们使用了传统的基因算法。他们还额外增加了世代数目和网络训练中epoch的数目，用来探索最佳的epoch的数目。所用的基因算法如下图。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmjQySWiciaiav8cBy5ahp02drhpN86EMJZMwjDTFw6Iod2kibIhTwX284ZLjrbOUnMJNiaqibLoVWLBpONQ/640?wx_fmt=jpeg)
作者们采用基因算法的原因是，染色体的复杂程度可以根据编码的基因数目增大或者降低。基因算法相比其它进化算法还有一些好处：它们可以流畅地处理离散（比如层类型）和连续（比如学习速率）的连续空间，进行神经进化研究的时候这就是理想的选择。
### **EDEN**
在这篇论文中，作者们提出了进化性深度网络（Evolutionary DEep Networks，EDEN），这算一种结合了基因算法和深度神经网络的的神经进化算法，用来探索神经网络架构、相关的超参数以及epoch数目的搜索空间。在这项研究中，作者们探索了例如嵌入层的优化这类的额外特征，并且提高了现有研究的复杂度。作者们希望通过EDEN解答两个问题：1，能否通过进化的方法得出总体来说较为优秀的架构和超参数用来解决许多不同的问题（并不仅限于图像识别）？2，与之前研究中使用的大规模计算集群相反，他们能否用单个GPU就达到这一目标？
作者们将EDEN接入了TensorFlow，这样新的层、函数以及其它的特征都可以轻松地被EDEN集合、应用，因为这些特征的表征函数都可以作为TensorFlow函数调用。而且，EDEN也不局限于TensorFlow，其它的现代深度神经网络框架也可以用来实现EDEN。下图就是一个EDEN的染色体在编码后得到的神经网络结构的例子。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmjQySWiciaiav8cBy5ahp02drh5pBPkdspoLZTGUHvg4sWibYI6ncp4dXC56cM8mTJAIbCnmEHLibCWnlQ/640?wx_fmt=jpeg)
每个EDEN染色体包含两个基因，编码学习速率和一个神经网络。图中展现的是一个用EDEN进化出的用于情绪分析的神经网络。EDEN创造了一个输出维度为120的嵌入层，其后有3个一维卷积层。EDEN进化出了滤波器的数目、每个滤波器的维度，以及每个滤波器响应的激活函数。对于最后一个层，EDEN决定使用的激活函数是sigmoid。这个染色体上的学习速率是0.0023。
下面这个动图展示了EDEN执行的过程中染色体进化的过程，要解决的问题是一个MNIST图像分类问题。这个过程中神经元群落向着一个高效的解决方案收敛，最终主要由二维卷积层组成。
![0?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/vJe7ErxcLmjQySWiciaiav8cBy5ahp02drht2tTNTYee8QOPUY1H58sLsoLd1U3paKjhaSZXiaicM5SngvgzA0MK47g/0?wx_fmt=gif)
### **实验结果**
运用EDEN，在10个世代和13个训练epoch后，作者们在多项测试中分别得到的网络结果如下。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmjQySWiciaiav8cBy5ahp02drhKN2s1libAmS8S8ZuJEgSlNRnE6deg94oGWiangqia7BhuibsebNOiamIStw/640?wx_fmt=jpeg)
对于不同的任务，网络中进化出的学习速率和参数数目也不一样。其中EMNIST-balanced、EMNIST-digits 和 Fashion-MNIST 三项任务中取得了最好的结果，刷新了目前的最好成绩。对于CIFAR-10中低很多的表现，作者们认为原因是，由于他们这项研究关注效率更多，所以对模型的深度做出了一定限制，也就限制了CIFAR-10任务中的表现。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/vJe7ErxcLmjQySWiciaiav8cBy5ahp02drh0TBRemfmUflGYMMEH9P8ibPpwCtbmDfG867J7ZKmZXLHibaf1VL59cEw/640?wx_fmt=jpeg)
这张图是在MNIST任务中，网络进化到不同世代的平均匹配度变化图。其中的竖线标出的是整个群落中5%到95%的匹配度值。在进化刚开始时，群落中的匹配度有很大的不同，然后随着解决方案变好、群落收敛，匹配度的变化幅度开始减小。图中也标出了自进化开始之后，最初、中间、最终三个世代时的网络结构以及相关的超参数。
作者们表示，他们实验中只使用了一块GTX1070 GPU，训练时间6到24小时，就可以取得这样的成果，他们觉得非常满意。他们的研究也首次尝试了把神经进化用在一维卷积网络的创造中，用来解决情感分析、包括嵌入层的优化问题。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVpctSUruZ7CVyvfdgiam9Kn1RRqiadHFWR5AXGa25afyXp46CsOLkNiatSrj39XcsQicHI1Ead3aIzhg/0?wx_fmt=png)

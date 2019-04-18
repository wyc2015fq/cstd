# Reducing the Dimensionality of Data with Neural Networks：神经网络用于降维 - wishchinYang的专栏 - CSDN博客
2015年04月26日 20:18:39[wishchin](https://me.csdn.net/wishchin)阅读数：9816
      原文链接：[http://www.ncbi.nlm.nih.gov/pubmed/16873662/](http://www.ncbi.nlm.nih.gov/pubmed/16873662/)
     G. E. Hinton* and R. R. Salakhutdinov .   [Science.](http://www.ncbi.nlm.nih.gov/pubmed/16873662/#) 2006 Jul 28;313(5786):504-7.
### Abstract
        High-dimensional data can be converted to low-dimensional codes by training a multilayer neural network with a small central layer to reconstruct high-dimensional input vectors. Gradient descent can be used for fine-tuning the weights in such "autoencoder" networks, but this works well only if the initial weights are close to a good solution. We describe an effective way of initializing the weights that allows deep autoencoder networks to learn low-dimensional codes that work much better than principal components analysis as a tool to reduce the dimensionality of data.
### Comment in
- [Computer science. New life for neural networks.](http://www.ncbi.nlm.nih.gov/pubmed/16873635) [Science. 2006]
摘要：
        高维数据通过使用一个小中心层（神经元个数小于维数）的多层神经网络（稀疏编码？）可以转换为低维数据 ，并可以通过此网络重建输入向量。梯度下降法可以用于在这个“自编码网络“  调整参数 ，只是**这种机理 只在 初始权值 接近最优参数 时有效** 。 我们设计出一个初始化权值的 有效方法，允许sleep 自编码器网络可以 学习低维表示 ，并且在降维的效果上好于主成分分析。
       特征降维广泛应用于分类、可视化、通信和高纬数据存储过程。一个简单和广泛应用的降维方法是主成分分析PCA，他通过寻找数据集合最大偏差方向 并 映射数据集到此方向，获得主成分。我们描述了一个非线性PCA产生方法，使用动态的多层编码器网络 转化高纬数据到低维数据，并且可以使用一个 类似的自编码网络 可以**从 低维数据恢复出元数据**。（使用压缩表示，用数据预训练——获取近似最优权值，用以生成可以刻画样本隐含性质的压缩结构）
![](https://img-blog.csdn.net/20150426120425159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Fig. 1. Pretraining consists of learning a stack of restricted Boltzmann machines (RBMs), each having only one layer of feature detectors. The learned feature activations of one** RBM**are used as the ‘‘data’’ for training the next RBM in the stack. After the pretraining, the RBMs are ‘‘unrolled’’ to create a deep autoencoder, which is then fine-tuned using backpropagation of error derivatives.
        预处理包含了 学习 一个 受限玻尔兹曼机栈（ 堆叠的RBM），每一个只有一层特征监测子。一层RBM学习过的数据作为下一层RBM的输入，然后使用BP算法调整参数。
        初始化：随机产生两个网络的权值，他们可以 同时 使用 最小化偏差（原数据和重建数据）方法进行训练。**梯度**可以 由**先通过解码网络再经过编码网络**反传导数误差链获得。整个系统叫做”自编码器“，在图表1中 阐述。
       优化有多层网络的 非线性自编码器 是非常困难的(2–4)，使用大量的初始大参数，自编码器容易陷入局部最优；使用小的初始化参数，前几层的梯度 较小，使自编码器 的多层训练变得不可行。 若初始化权值接近一个最优方案，梯度衰减可以正常运行，但是寻找这一个近邻最优权值 需要一个特别的算法， 可以一次性训练一个特征提取层。 我们为二进制数据引入一个”预处理“过程，并推广它到 实数数据，结果显示此过程适用于广泛的数据集。
       一个二进制向量的组合（比如 一个图片）可以使用 ”受限玻尔兹曼机RBM“ 建模，使用对称加权连接  随机二进制特征 到随机二进制特征探测器。**像素点值对应于RBM的“可见”元**，因为 他们是可见的；**特征探测子/描述子 对应”隐藏“单元**。一个 可见和隐藏单元 生成 联合分布/配置（v,h）能量：
![](https://img-blog.csdn.net/20150426142106421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
where vi and hj are the binary states of pixel i and feature j, biand bjare their biases, and wij is the weight between them.
 这里vi 和 hj 为点i 和特征j 的二进制状态， bi和 bj为他们的基，wij 为他们的权值。
       网络通过这个方程 分配一个概率到每一个可能的图像，正如（8）解释的。这个训练图像的概率 可以 通过调整权值和基 以降低图片的能量 产生，并生成相似的、”虚构/构建“ 的图片（更偏向于真实数据的网络）能量。
       特征探测子 j 的二进制状态 Hj 设定为1以 F(bj +Add(Vi*Wij) ) 的概率，在此F()是逻辑斯特函数（1/(1+exp(-x))）,bj 是j的偏差，Vi是i的状态，Wij是i和j之间的权值。
       一旦二进制状态被选定到隐藏元，一个 虚构/构建 产生 通过设定每一个Vi为1 以F(bj + sigma(Vi*Wij)  )的概率，此时bi为i的偏差。隐藏元的权值被更新 以使他们可以 表示"虚构/构建" 的特征，**权值更新**通过公式
![](https://img-blog.csdn.net/20150426154236873?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)得出，
       这里emuxilong 是学习率，<ViHi>data为分裂的次数（点i 和 特征检测子j ）当特征检测子受数据驱动，<ViHi>recon为”虚拟/构建“ 的响应速率。同样的简化版本 的学习规则可用于偏差。这个学习过程工作良好即使它并不事实沿着训练数据(6)的逻辑概率 的梯度方向。
![](https://img-blog.csdn.net/20150426144507433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Fig. 2. (A) Top to bottom: Random samples of curves from the test data set; reconstructions produced by the six-dimensional deep autoencoder; reconstructions
by  ‘‘logistic PCA’’ (8) using six components; reconstructions by logistic PCA and standard PCA using 18 components. The average squared error per image for the last four rows is 1.44, 7.64, 2.45, 5.90. 
由顶至下：来自于测试集的随机采样曲线；由六维深度自编码器重建，使用Logistic PCA方法重建(8)使用6个主元；使用Logistic PCA方法重建使用18个主元.每个图像最后四行 平均方差是 1.44, 7.64, 2.45, 5.90.
(B) Top to bottom: A random test image from each class; reconstructions by the 30-dimensional autoencoder; reconstructions by 30- dimensional logistic PCA and standard PCA. The average squared errors for the last three rows are 3.00, 8.01, and 13.87.
自顶向下：每一类的 一个随机测试图像；由30维自编码器重建；使用30维 Logistic PCA和标准PCA方法重建；最下三行的平均方差 是3.00, 8.01, and 13.87.
(C) Top to bottom: Random samples from the test data set; reconstructions by the 30- dimensional autoencoder; reconstructions by 30-dimensional PCA. The average squared errors are 126 and 135.
自顶向下：每一类的 一个随机测试样本；由30维自编码器重建；使用30维  PCA重建；均方差是126 and 135.
       一个单层二进制特征提取层并非是 为一个图像集合 建立模型最好的方法。训练完一层网络之后，我们可以使它激活，当这一层被数据驱动——进而**生成数据**作为学习第二层特征的输入。第一层的特征探测子成为第二层RBM的可见元。这种一层接一层/逐层的学习 当必要时可以重复多次/反复进行。
       可以看出：**增加一个额外的层一般可以促进更低的 （这个模型指定给训练数据的）逻辑概率边界**，此时应假定每层特征检测子的数目不减少并且他们的初始权值被正确初始化(9)。这个约束不能应用于当一个更高的层具有更少的特征探测子的情况，但 即使如此 逐层学习算法 依然是一个有效去预训练深度编码器的权值的方法。每一层的特征捕获 底层网络单元中  更稳定/强 的更高阶的联系。对于更广泛的数据集，这是渐进地揭示低维数据结构的 有效结构 的一个有效方法。
![](https://img-blog.csdn.net/20150426161953137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20150426162601423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        预训练多层网络之后，模型 对于产生（使用相同权值初始化的）编码和解码器 网络 是 ”开放“的（图表1）。全局参数调整过程随后  用确定性的实值概率和 使用 通过对整个自编码器反传 调整权值 取代（stochastic activities）随机激活去优化结构。
       对于连续数据，第一层RBM隐藏元保持二进制，但是可见元使用高斯噪音处理过的线性单元取代(10)。若噪音单元变量，隐藏元的随机更新规则 保持相同，这个可见元i的更新规则来自于 使用 单元方差和均值(bj + Add(H j +W ij) )。
        在我们的试验中，每一个RBM和实值激活 的可见元，逻辑元变化范围为[0,1]。当训练更高层的RBM，可见元内设定为激活概率（来自于前一层的 隐藏元——除了顶层RBM的随机二进制值）。顶层的隐藏元的权值 为随机实数值 从 一个单变量高斯 提取（其均值 由RBM的 逻辑可见元 决定 ）。这 就使得 低维编码 可以更好地利用连续数据 并且 方便与PCA  对比。预训练和参数调整 可以参考（8）。
        为证实我们的预训练算法可以使我们有效地调节参数，我们训练一个特定的深度编码器在一个合成的数据集上（数据集 包含由两个维度的三个随机选择点 生成的 ”曲线“ 图片  ）。对于这个数据集，**这个本质/真实 维度是已知的**，这个映射从 点的强度到 六个参数 是高度非线性的。点强度范围为[0,1] 并且是非高斯的，因此我们使用逻辑斯特输出单元在自编码器，学习的 参数微调阶段使用 最小化 交叉熵损失[- Add(Pi*  log M(Pi)  ) - Add( (1-Pi )* log(1-Pi )  )  ] , Pi 为点i的强度，M(Pi)    为  此点的重建。
       自编码器包含了一个尺寸为（28* 28）的编码器400-200-100-50-25-6和一个合成解码器。这 在编码层的六个单元是线性的，且所有的其他单元是 Logistic。此网络由20000个图片训练并使用10000个测试。这个自编码器 发现 如何转化784个点图像 到6个实数（图表2.A : 可实现完美重建），PCA给出的效果坏的多（有损压缩？）。若没有预训练过程，特定的自编码器 即使 延长 参数微调(8)，也只能恢复出平均数据。只有一层的浅层自编码器 可以不经预训练学习，但是**经过预训练可以很大地降低训练时间(8)**。当参数相同时，深度自编码器 可以比浅层模型 产生更低的重建误差，但是这种 优势会随着参数增长而递减。
       下一步，我们使用一个 784-1000-500-250-30 自编码器 提取所有的MINIST训练集的 编码。我们使用的训练和调参Matlab代码在[ 8 ] 。然后，除 编码层   的30个线性单元 之外，所有单元是 Logistic单元。 在训练600000 个样本之后，模型在10000个样本上测试，取得了比PCA更好的重建结果（图表2.B）。一个二维 自编码器 可视化效果 超过 PCA 两主元分析(图表3)。
         我们使用 一个625-2000-1000-500-30自编码器  利用线性输入元 为灰度图像块（Olivetti face data set） 产生30维编码。自编码器 明确显示出PCA(图表2C).
       当训练用于文档，自编码器产生 允许快速检索的 编码。我们 把 804 414 个 每个新闻故事作为一个向量(13)， 每个向量对应 文档特定概率 ——2000个通用词根之中的。我们训练 一个 2000-500-250-125-10 自编码器，使用一半的故事，使用多类交叉熵[- Add(Pi*  log M(Pi)  )] 用于参数微调。10个码为线性 ，其余的隐藏元 为Logistic。两个编码的 余弦用于相似性度量，自编码器(8)依然 效果好于 局部线性嵌入LLE，一个最近 发现的非线性降维方法。
       逐层/一层又一层预训练 也可以用于 分类和回归。在广泛使用的MNIST手写数字识别任务上，随机初始化的BP网络最好的错误率为1.6% ,支持向量机最好为1.4%。在逐层预训练 的784-500-500-2000-10 网络，使用梯度下降法和小学习速率 的反传网络达到了1.2%。预处理 帮助/促进泛化 是因为权值产生的大多数信息来自于图像本身的建模。只有特定的受限制的带标签的训练信息用于调整权值。
        从1980年之后，已证明若在计算速度较快、数据集足够大，初始权值接近最优权值，反传算法用于深度自编码将是一个 非常有效 的方法。现在所有三个条件都已经满足。不像非参数模型（15,16）,**自编码器给出了方向和范围两方面的映射**，并且可以用于大数据集合 ，因为 预处理和 参数微调 都在线性时间和空间 内执行。
参考文献：
1. D. C. Plaut, G. E. Hinton, Comput. Speech Lang. 2, 35(1987).
2. D. DeMers, G. Cottrell, Advances in Neural Information Processing Systems 5 (Morgan Kaufmann, San Mateo, CA,1993), pp. 580–587.
3. R. Hecht-Nielsen, Science 269, 1860 (1995).
4. N. Kambhatla, T. Leen, Neural Comput. 9, 1493(1997).
5. P. Smolensky, Parallel Distributed Processing: Volume 1:Foundations, D. E. Rumelhart, J. L. McClelland, Eds. (MITPress, Cambridge, 1986), pp. 194–281.
6. G. E. Hinton, Neural Comput. 14, 1711 (2002).
7. J. J. Hopfield, Proc. Natl. Acad. Sci. U.S.A. 79, 2554(1982).
8. See supporting material on Science Online.
9. G. E. Hinton, S. Osindero, Y. W. Teh, Neural Comput. 18,1527 (2006).
10. M. Welling, M. Rosen-Zvi, G. Hinton, Advances in Neural Information Processing Systems 17 (MIT Press, Cambridge,MA, 2005), pp. 1481–1488.
11. The MNIST data set is available at http://yann.lecun.com/exdb/mnist/index.html.
12. The Olivetti face data set is available at www.cs.toronto.edu/ roweis/data.html.
13. The Reuter Corpus Volume 2 is available at http://trec.nist.gov/data/reuters/reuters.html.
14. S. C. Deerwester, S. T. Dumais, T. K. Landauer, G. W.Furnas, R. A. Harshman, J. Am. Soc. Inf. Sci. 41, 391(1990).
15. S. T. Roweis, L. K. Saul, Science 290, 2323 (2000).
16. J. A. Tenenbaum, V. J. de Silva, J. C. Langford, Science
290, 2319 (2000).
17. We thank D. Rumelhart, M. Welling, S. Osindero, and S. Roweis for helpful discussions, and the Natural Sciences and Engineering Research Council of Canada for
funding. G.E.H. is a fellow of the Canadian Institute for Advanced Research.
Supporting Online Material
www.sciencemag.org/cgi/content/full/313/5786/504/DC1
Materials and Methods
Figs. S1 to S5
Matlab Code
20 March 2006; accepted 1 June 2006
10.1126/science.1127647
**后记：**
        有一天，把感知和决策 划归到 触发器 分类的范畴，把多类分类 划归为 维度压缩，把任意 物体 压缩到特征空间 、把特征集合 压缩到任意 一维 的离散个点上，这就是特征提取和分类器设计。特征提取 和 分类器设计？ 都是降维？
        这样不好吧？
        参考一下链接：[http://blog.csdn.net/yihaizhiyan/article/details/8189731](http://blog.csdn.net/yihaizhiyan/article/details/8189731)
      代码下载：[http://www.cs.toronto.edu/~hinton/MatlabForSciencePaper.html](http://www.cs.toronto.edu/~hinton/MatlabForSciencePaper.html)  （一个需要注意的地方，就是数据文件：要使用**gunzip**train-images-idx3-ubyte.gz 进行解压~若使用winzip解压后，会损坏相关数据文件~）
**总结：**
        参考链接：[http://blog.csdn.net/zouxy09/article/details/8781543/](http://blog.csdn.net/zouxy09/article/details/8781543/)
**工程角度理解：**
**训练方法：**
1）首先逐层构建单层神经元，这样每次都是训练一个单层网络。
2）当所有层训练完后，Hinton使用wake-sleep算法进行调优。
        将除最顶层的其它层间的权重变为双向的，这样**最顶层仍然是一个单层神经网络，而其它层则变为了图模型**。向上的权重用于“认知”，向下的权重用于“生成”。然后使用**Wake-Sleep算法**调整所有的权重。让认知和生成达成一致，也就是保证生成的最顶层表示能够尽可能正确的复原底层的结点。
       比如顶层的一个结点表示人脸，那么所有人脸的图像应该激活这个结点，并且这个结果向下生成的图像应该能够表现为一个大概的人脸图像。Wake-Sleep算法分为醒（wake）和睡（sleep）两个部分。
**1）wake阶段**：认知过程，通过外界的特征和向上的权重（认知权重）产生每一层的抽象表示（结点状态），并且使用梯度下降修改层间的下行权重（生成权重）。~~也就是“如果现实跟我想象的不一样，改变我的权重使得我想象的东西就是这样的”。~~
**2）sleep阶段**：生成过程，通过顶层表示（醒时学得的概念）和向下权重，生成底层的状态，同时修改层间向上的权重。~~也就是“如果梦中的景象不是我脑中的相应概念，改变我的认知权重使得这种景象在我看来就是这个概念”。~~
**DL具体训练过程：**
1）使用自下上升非监督学习：
     （就是从底层开始，一层一层的往顶层训练）采用无标定数据（有标定数据也可）分层训练各层参数，这一步可以看作是一个无监督训练过程，是和传统神经网络区别最大的部分（这个过程可以看作是**feature learning过程**）：
       具体的，先用无标定数据训练第一层，训练时先学习第一层的参数（这一层可以看作是得到一个使得输出和输入差别最小的三层神经网络的隐层），由于模型capacity的限制以及稀疏性约束，使得得到的模型能够学习到数据本身的结构，从而得到比输入更具有表示能力的特征；在学习得到第n-1层后，将n-1层的输出作为第n层的输入，训练第n层，由此分别得到各层的参数；
2）自顶向下的监督学习：
      （就是通过带标签的数据去训练，误差自顶向下传输，对网络进行微调）基于第一步得到的各层参数进一步fine-tune整个多层模型的参数，这一步是一个有监督训练过程；第一步类似神经网络的随机初始化初值过程，由于DL的第一步不是随机初始化，而是通过学习输入数据的结构得到的，因而这**个初值更接近全局最优**，从而能够取得更好的效果；所以deep learning效果好很大程度上归功于第一步的feature learning过程。

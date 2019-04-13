
# 理解主成分分析 (PCA) - SIGAI_CSDN的博客 - CSDN博客
# [SIGAI_CSDN的博客](https://blog.csdn.net/sigai_csdn)


[博客首页](https://blog.csdn.net/SIGAI_CSDN)
[关于我们](https://me.csdn.net/SIGAI_CSDN)

2018年06月12日 13:32:57[SIGAI_csdn](https://me.csdn.net/SIGAI_CSDN)阅读数：3150


原创声明：本文为SIGAI原创文章，仅供个人学习使用，未经允许，不得转载，不能用于商业目的。


### 导言
主成分分析法(PCA)是一种常用的数据分析手段。对于一组不同维度之间可能存在线性相关关系的数据，PCA能够把这组数据通过正交变换变成各个维度之间线性无关的数据。经过PCA处理的数据中的各个样本之间的关系往往更直观，所以它是一种非常常用的数据分析和预处理工具。PCA处理之后的数据各个维度之间是线性无关的，通过剔除方差较小的那些维度上的数据我们可以达到数据降维的目的。在本文中，我们将介绍PCA的原理、应用以及缺陷。
### 为什么要有
### PCA
如果数据之中的某些维度之间存在较强的线性相关关系，那么样本在这两个维度上提供的信息有就会一定的重复，所以我们希望数据各个维度之间是不相关的(也就是正交的)。此外，出于降低处理数据的计算量或去除噪声等目的，我们也希望能够将数据集中一些不那么重要(方差小)的维度剔除掉。例如在下图中，数据在x轴和y轴两个维度上存在着明显的相关性，当我们知道数据的x值时也能大致确定y值的分布。但是如果我们不是探究数据的x坐标和y坐标之间的关系，那么数据的x值和y值提供的信息就有较大的重复。在绿色箭头标注的方向上数据的方差较大，而在蓝色箭头方向上数据的方差较小。这时候我们可以考虑利用蓝色和绿色的箭头表示的单位向量来作为新的基底，在新的坐标系中原来不同维度间线性相关的数据变成了线性不相关的。由于在蓝色箭头方向上数据的方差较小，在需要降低数据维度的时候我们可以将这一维度上的数据丢弃并且不会损失较多的信息。如果把丢弃这一维度之后的数据重新变化回原来的坐标系，得到的数据与原来的数据之间的误差不大。这被称为重建误差最小化。PCA就是进行这种从原坐标系到新的坐标系的变换的。
![](https://img-blog.csdn.net/20180612113207191?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图1:示意图

### 如何计算
### PCA
数据经过PCA变换之后的各个维度被称为主成分，各个维度之间是线性无关的。为了使变换后的数据各个维度提供的信息量从大到小排列，变换后的数据的各个维度的方差也应该是从大到小排列的。数据经过PCA变换之后方差最大的那个维度被称为第一主成分。
我们先来考虑如何计算第一主成分。假设每一条原始数据是一个m维行向量，数据集中有n条数据。这样原始数据就可以看作一个n行m列的矩阵。我们将其称为X，用![](https://img-blog.csdn.net/20180612133541207?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)代表数据集中的第i条数据(也就是X的第i和行向量)。这里为了方便起见，我们认为原始数据的各个维度的均值都是0。当原始数据的一些维度的均值不为0时我们首先让这一维上的数据分别减去这一维的均值，这样各个维度的均值就都变成了0。为了使X变化到另一个坐标系，我们需要让X乘以一个m × m的正交变换矩阵W。W视为由列向量![](https://img-blog.csdn.net/20180612133622413?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)组成。我们让X和W进行矩阵相乘之后就可以原始数据变换到新的坐标系中。
T = XW
为了使变换不改变数据的大小，我们让W中的每个列向量wi的长度都为1，也就是![](https://img-blog.csdn.net/20180612133710497?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)。T中的各个列向量为![](https://img-blog.csdn.net/20180612133817758?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)。为了使第一主成分(t1)的方差最大，
![](https://img-blog.csdn.net/20180612113308435?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上述最优化问题中w1的长度被限制为1，为了求解w1，我们将其变成如下的形式:
![](https://img-blog.csdn.net/20180612113324842?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因为当C是一个不为零的常数时，
![](https://img-blog.csdn.net/20180612113337956?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这时候求解出的是 w1 的方向。我们只要在这个方向上长度取长度为 1的向量就得到了结果。![](https://img-blog.csdn.net/20180612133940300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)是一个非常常见的瑞利熵，其更一般的形式是
![](https://img-blog.csdn.net/20180612113509369?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这里的M是一个厄米特矩阵(Hermitian Matrix)，在本文中我们可以将其认为是一个实对称矩阵;x是一个长度不为零的列向量。求解瑞利熵的最值需要对实对称矩阵的对角化有一定的了解。这里的![](https://img-blog.csdn.net/20180612134013409?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)很显然是一个实对称矩阵。对一个实对称矩阵进行特征值分解，我们可以得到:
![](https://img-blog.csdn.net/20180612113527302?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这里的D是一个对角矩阵，对角线上的元素是特征值;P =< p1, p2, ..., pn>，每个pi都是一个长度为1的特征向量，不同的特征向量之间正交。我们将特征值分解的结果带回瑞利熵中可以得到
![](https://img-blog.csdn.net/20180612113550977?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这里的![](https://img-blog.csdn.net/201806121341313?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)这时有![](https://img-blog.csdn.net/20180612134207486?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)。这样![](https://img-blog.csdn.net/20180612134239312?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)就构成了一个一维凸包。根据凸包的性质我们可以知道，当最大的λi对应的αi= 1时整个式子有最大值。所以当x的为最大的特征值对应的特征向量时瑞利熵有最大值，这个最大值就是最大的特征值。根据这个结论我们就可以知道w1就是XTX的最大的特征值对应的特征向量，第一主成分t1= Xw1。这样我们就得到了计算第一主成分的方法。接下来我们继续考虑如何计算其他的主成分。因为W是一个正交矩阵，所以
![](https://img-blog.csdn.net/20180612131656171?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因为wk和w1, w2, ..wk−1正交，
![](https://img-blog.csdn.net/20180612131915939?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
为了使第k个主成分在与前k - 1个主成分线性无关的条件下的方差最大，那么wk应该是第k大的特征值对应的特征向量。经过这些分析我们就能发现变换矩阵W中的每个列向量就是![](https://img-blog.csdn.net/20180612134325749?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的各个特征向量按照特征值的大小从左到右排列得到的。
接下来我们对如何计算PCA做一个总结:
1、把每一条数据当一个行向量，让数据集中的各个行向量堆叠成一个矩阵。
2、将数据集的每一个维度上的数据减去这个维度的均值，使数据集每个维度的均值都变成0，得到矩阵X。
3、计算方阵![](https://img-blog.csdn.net/20180612134325749?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的特征值和特征向量，将特征向量按照特征值由大到小的顺序从左到右组合成一个变化矩阵W。为了降低数据维度，我们可以将特征值较小的特征向量丢弃
4、计算T = XW，这里的T就是经过PCA之后的数据矩阵。
除了这种方法之外，我们还可以使用奇异值分解的方法来对数据进行PCA处理，这里不再详细介绍。

#### PCA
#### 的应用
首先我们来看一下PCA在数据降维方面的应用。我们在MNIST数据集上进行了测试。我们对MNIST的测试集中的每一幅28×28的图片的变成一个784维的行向量，然后把各幅图片拼接成的行向量堆叠一个784×10000的数据矩阵。对这个数据矩阵进行PCA处理。处理得到的特征值的分布如下图。通过图片我们可以看出前面一小部分的特征值比较大，后面的特征值
![](https://img-blog.csdn.net/20180612132413760?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图2: MNIST数据集特征值的分布
都比较接近于零。接下来我们取前200，300个主成分对数据进行重建。我们发现使用前200个主成分重建的图像已经能够大致分辨出每个数字，使用前300个主成分重建的图像已经比较清晰。根据实验我们可以发现PCA能够在丢失较少的信息的情况下对数据进行降维。
PCA在自然语言处理方面也有比较多的应用，其中之一就是用来计算词向量。word2vec是Google在2013年提出了一个自然语言处理工具包，
![](https://img-blog.csdn.net/20180612132434713?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图3:原始图像
![](https://img-blog.csdn.net/20180612132450545?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图4:使用前200个主成分重建的图像
![](https://img-blog.csdn.net/20180612132507790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图5:使用前300个主成分重建的图像
其思想是用一个向量来表示单词，意思和词性相近的单词对应的向量之间的距离比较小，反之则单词之间的距离比较大。word2vec原本是使用神经网络计算出来的，本文中的PCA也可以被用于计算词向量。具体的做法为:构建一个单词共生矩阵，然后对这个矩阵进行PCA降维，将降维得到的数据作为词向量。使用这种方法构造出的词向量在单独使用时效果虽然不如使用神经网络计算出的词向量，但是将神经网络构造出来的词向量和使用PCA降维得到的词向量相加之后得到的词向量在表示词语意思时的效果要好于单独使用神经网络计算出来的词向量。
![](https://img-blog.csdn.net/20180612132522930?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图6:一个共生矩阵的例子，
图片来自于斯坦福大学公开课cs224n课件

#### PCA
#### 的缺陷
虽然PCA是一种强大的数据分析工具，但是它也存在一定的缺陷。一方面，PCA只能对数据进行线性变换，这对于一些线性不可分的数据是不利的。为了解决PCA只能进行线性变换的问题，Schölkopf, Bernhard在1998年提出了Kernel PCA。Kernel PCA在计算M =![](https://img-blog.csdn.net/20180612134325749?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)的时候不是直接进行相乘，而是使![](https://img-blog.csdn.net/20180612132814548?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)。这里的K(xi,xj)是一个与支持向量机中类似的核函数。这样就能够对数据进行非线性变换。另一方面，PCA的结果容易受到每一维数据的大小的影响，如果我们对每一维数据乘以一个不同的权重因子之后再进行PCA降维，得到的结果可能与直接进行PCA降维得到的结果相差比较大。对于这个问题，Leznik等人在论文Estimating Invariant Principal Components Using Diagonal Regression中给出了一种解决方案。除此之外，PCA要求数每一维的均值都是0，在将原始数据的每一维的均值都变成0时可能会丢失掉一些信息。虽然PCA有这些缺陷，但是如果合理的利用，PCA仍然不失为一种优秀的数据分析和降维的手段。

参考文献
Pearson, K. (1901). ”On Lines and Planes of Closest Fit to Systems of Points in Space”.http://stat.smmu.edu.cn/history/pearson1901.pdf. Philosophical Magazine. 2 (11):559–572.
Principal component analysis(主成分分析). https://en.wikipedia.org/wiki/Principal_component
Rayleigh quotient(瑞利熵). https://en.wikipedia.org/wiki/Rayleigh_quotient.Wikipedia.
Hermitian matrix(厄米特矩阵). https://en.wikipedia.org/wiki/Hermitian_matrix.Wikipedia.
Yann LeCun. [MNIST数据集] (http://yann.lecun.com/exdb/mnist/).
Tomas Mikolov, Ilya Sutskever, Kai Chen, Greg Corrado, Je rey Dean.(2013)”Distributed Representations of Words and Phrases and their Compositionality”.https://arxiv.org/pdf/1310.45 arxiv.org.
Schölkopf, Bernhard (1998). ”Nonlinear Component Analysis as a Kernel Eigenvalue Problem”. Neural Computation. 10: 1299–1319. doi:10.1162/089976698300017467.
Leznik, M; Tofallis, C. 2005 [uhra.herts.ac.uk/bitstream/handle/2299/715/S56.pdf Estimating Invariant Principal Components Using Diagonal Regres-
sion.]
原创声明：本文为SIGAI原创文章，仅供个人学习使用，未经允许，不得转载，不能用于商业目的。

推荐阅读
[1]机器学习-波澜壮阔40年SIGAI 2018.4.13.
[2]学好机器学习需要哪些数学知识？SIGAI 2018.4.17.
[3]人脸识别算法演化史SIGAI 2018.4.20.
[4]基于深度学习的目标检测算法综述SIGAI 2018.4.24.
[5]卷积神经网络为什么能够称霸计算机视觉领域？SIGAI 2018.4.26.
[6]用一张图理解SVM的脉络SIGAI 2018.4.28.
[7]人脸检测算法综述SIGAI 2018.5.3.
[8]理解神经网络的激活函数SIGAI 2018.5.5.
[9]深度卷积神经网络演化历史及结构改进脉络-40页长文全面解读SIGAI2018.5.8.
[10]理解梯度下降法SIGAI 2018.5.11
[11]循环神经网络综述—语音识别与自然语言处理的利器SIGAI 2018.5.15
[12]理解凸优化SIGAI 2018.5.18
[13]【实验】理解SVM的核函数和参数SIGAI 2018.5.22
[14]【SIGAI综述】行人检测算法SIGAI 2018.5.25
[15]机器学习在自动驾驶中的应用—以百度阿波罗平台为例（上）SIGAI 2018.5.29
[16]理解牛顿法SIGAI 2018.5.31
[17]【群话题精华】5月集锦—机器学习和深度学习中一些值得思考的问题SIGAI 2018.6.1
[18]大话Adaboost算法SIGAI 2018.6.1
[19]FlowNet到FlowNet2.0：基于卷积神经网络的光流预测算法SIGAI 2018.6.4

![](https://img-blog.csdn.net/20180612112124163?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NJR0FJX0NTRE4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


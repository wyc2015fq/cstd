# 数据降维算法-从PCA到LargeVis - 知乎
# 

本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
- [书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
- [书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)

**PDF全文链接：**[数据降维算法-从PCA到LargeVis](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_143.html)

## **数据降维算法-从PCA到LargeVis**


数据降维算法是机器学习算法中的大家族，与分类、回归、聚类等算法不同，它的目标是将向量投影到低维空间，以达到某种目的如可视化，或是做分类。本文对数据降维算法做了较为详细的总结，是对《机器学习与应用》，清华大学出版社，雷明著一书第1版的补充，该书自2019年1月份出版以来，已经重印4次，最近一次重印修正了之前已经发现的所有笔误与印刷错误，优化后的第2版将在上半年出版。本文所列的大部分内容都将在第2版中详细讲述。对于流形学习，之前SIGAI已经写过一篇简单的文章“流形学习概述”，由于当时的时间仓促，还不够全面，本文更为全面。限于篇幅，对每种算法不做具体的推导，详细的推导和证明可以阅读《机器学习与应用》的第1版或第2版。


## **什么是降维问题**

数据降维从字面看很容易理解，就是把向量变换到低维空间中，即降低向量的维数。初学者可能会有一个疑问：为什么要这个事情？下面我们来看一个例子。大家都知道MNIST手写数字数据集，它们是这样的黑白图像

![](https://pic2.zhimg.com/v2-211e7fe0b5eadff50bb35d560f77f041_b.png)

该数据集包括0-9这10个阿拉伯数字，每张图像为28x28像素。如果将这些像素拼接起来，则为784维的向量。你是否想过，在784维的空间中，这些样本是如何分布的？。按道理来说，同样的字，如“1”，应该有规律的分布在784维空间中的某一或几个区域中。单我们只能“想象”，因为人能直观看到的空间，最多为3维。通过数据降维，可以将这些数字投影到3维空间中并进行可视化，下图是投影后的效果

![](https://pic3.zhimg.com/v2-05ea7d40f0930c2ed0142078e3db7ef6_b.png)

在低维空间中，相同的数字往往分布在一起，如果这种投影保持了高维空间中的数据分布特性，则我们可认为在高维空间中相同的数字也分布在某一或多个区域内。

人脸图像也是高维空间中的向量，它在空间中的分布受人脸的身份（不同的人的脸有不同的外观），光照，角度等因素的影响。通过对人脸数据进行降维，可以观察这些因素对人脸在空间中的分布所产生的影响，下图形象了说明了这一做法

![](https://pic2.zhimg.com/v2-cbef72bcec28dd3534d8a1fb491c9769_b.png)

这里的横轴是人脸相对于成像平面左右旋转的角度，纵轴为上下旋转的角度， 另外哈加上了光照方向信息。从这张图中我们可以清晰的看到这些因素是如何影响人脸在高维空间中分布的。

再举一个例子。Google DeepMind公司当年提出的DQN（用深度强化学习打Atari游戏）开深度强化学习之先河，为了验证CNN学习到的特征的有效性，论文里用t-SNE对各种游戏画面下CNN提取的特征进行了可视化。结果发现，需要采取相同动作的游戏画面经过CNN映射之后，其特征图像用t-SNE投影后聚集在一起，这说明CNN确实学习得到了有用的游戏画面信息。下图是他们的实验结果

![](https://pic2.zhimg.com/v2-01c15420a0ed471adcfd816bac98a281_b.png)

除了便于可视化，数据降维算法还可以提升分类、聚类算的精度，避免维数灾难问题。抽象来看，数据降维就是寻找一个映射函数f，将高维向量x映射成低维向量y

![f : \mathbf{x} \rightarrow \mathbf{y} ](https://www.zhihu.com/equation?tex=f+%3A+%5Cmathbf%7Bx%7D+%5Crightarrow+%5Cmathbf%7By%7D+)

如何确定这个映射函数，是各降维算法核心，它们往往根据不同的准则进行构造。


## **降维算法的分类**

目前已经存在大量的数据降维算法，可以从另个不同的维度对它们进行分类。按照是否有使用样本的标签值，可以将降维算法分为有监督降维和无监督降维；按照降维算法使用的映射函数，可以将算法分为线性降维与非线性降维。

无监督降维算法不使用样本标签值，因此是一种无监督学习算法，其典型代表是PCA。有监督的降维算法则使用了样本标签值，是一种有监督学习算法，其典型代表是LDA。

线性降维算根据样本集构造出线性函数完成向低维空间的映射。一般通过对向量x进行线性变换即左乘一个投影矩阵W而得到结果向量y

![\mathbf{y}=\mathbf{W} \mathbf{x}](https://www.zhihu.com/equation?tex=%5Cmathbf%7By%7D%3D%5Cmathbf%7BW%7D+%5Cmathbf%7Bx%7D)

非线性降维算法则构造一个非线性映射完成数据的降维。很多时候数据是非线性的，因此需要使用非线性降维算法以取得更好的效果。


## **PCA**

PCA[1]是最基础的无监督降维算法，其目标是向数据变化最大的方向投影，或者说向重构误差最小化的方向投影。如果要将向量投影到$$d^{'}$$维的空间，每个向量可以表示成

![\mathbf{x}=\mathbf{m}+\sum_{i=1}^{d^{\prime}} a_{i} \mathbf{e}_{i}](https://www.zhihu.com/equation?tex=%5Cmathbf%7Bx%7D%3D%5Cmathbf%7Bm%7D%2B%5Csum_%7Bi%3D1%7D%5E%7Bd%5E%7B%5Cprime%7D%7D+a_%7Bi%7D+%5Cmathbf%7Be%7D_%7Bi%7D)

在这里 ![e_{i}](https://www.zhihu.com/equation?tex=e_%7Bi%7D) 都是单位向量，并且相互正交，是PCA的投影方向。重构误差函数为

![\sum_{i=1}^{n}\left\|\mathbf{m}+\sum_{j=1}^{d^{\prime}} a_{i j} \mathbf{e}_{j}-\mathbf{x}_{i}\right\|^{2}](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Cleft%5C%7C%5Cmathbf%7Bm%7D%2B%5Csum_%7Bj%3D1%7D%5E%7Bd%5E%7B%5Cprime%7D%7D+a_%7Bi+j%7D+%5Cmathbf%7Be%7D_%7Bj%7D-%5Cmathbf%7Bx%7D_%7Bi%7D%5Cright%5C%7C%5E%7B2%7D)

使得该函数取最小值的 ![e_{j}](https://www.zhihu.com/equation?tex=e_%7Bj%7D) 为散度矩阵最大的 ![d^{'}](https://www.zhihu.com/equation?tex=d%5E%7B%27%7D) 个特征值对应的单位长度特征向量。通过拉格朗日乘数法可以证明，最小化重构误差等价于求解下面的特征值问题

![\begin{array}{l}{\min _{\mathrm{w}}-\operatorname{tr}\left(\mathbf{w}^{\mathrm{T}} \mathbf{S W}\right)} \\ {\mathbf{W}^{\mathrm{T}} \mathbf{W}=\mathbf{I}}\end{array}](https://www.zhihu.com/equation?tex=%5Cbegin%7Barray%7D%7Bl%7D%7B%5Cmin+_%7B%5Cmathrm%7Bw%7D%7D-%5Coperatorname%7Btr%7D%5Cleft%28%5Cmathbf%7Bw%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7BS+W%7D%5Cright%29%7D+%5C%5C+%7B%5Cmathbf%7BW%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7BW%7D%3D%5Cmathbf%7BI%7D%7D%5Cend%7Barray%7D)

其中tr为矩阵的迹，I为单位矩阵，S是样本的协方差矩阵。等式约束保证投影基向量是标准正交基。矩阵W的列 ![e_{j}](https://www.zhihu.com/equation?tex=e_%7Bj%7D) 是要求解的基向量。在得到投影矩阵W之后，先将x减掉均值向量m，然后左乘矩阵W即可完成降维。

下图是用PCA对MNIST数据集投影后的结果

![](https://pic1.zhimg.com/v2-ffcb7d01899230a03af48e7b8e3951ec_b.png)

## **LDA**

LDA[2-3]的目标是向最大化内间差异，最小化类内差异的方向投影，以利于分类等任务即将不同类的样本有效的分开。这归结为求解下面的优化问题

![\max _{\mathbf{w}} \frac{\operatorname{tr}\left(\mathbf{w}^{\mathrm{T}} \mathbf{S}_{s} \mathbf{w}\right)}{\operatorname{tr}\left(\mathbf{w}^{\mathrm{T}} \mathbf{S}_{w} \mathbf{w}\right)}](https://www.zhihu.com/equation?tex=%5Cmax+_%7B%5Cmathbf%7Bw%7D%7D+%5Cfrac%7B%5Coperatorname%7Btr%7D%5Cleft%28%5Cmathbf%7Bw%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7BS%7D_%7Bs%7D+%5Cmathbf%7Bw%7D%5Cright%29%7D%7B%5Coperatorname%7Btr%7D%5Cleft%28%5Cmathbf%7Bw%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7BS%7D_%7Bw%7D+%5Cmathbf%7Bw%7D%5Cright%29%7D)

其中tr为矩阵的迹， ![S_{B}](https://www.zhihu.com/equation?tex=S_%7BB%7D) 为类间散度矩阵， ![S_{W}](https://www.zhihu.com/equation?tex=S_%7BW%7D) 为类内散度矩阵，W为投影矩阵。分子反映了类间差异，需要最大化；分母反映了类内差异，需要最小化。这个问题存在冗余，加上约束条件消掉冗余，等价于优化下面的问题

![\begin{array}{l}{\max _{\mathbf{w}} \operatorname{tr}\left(\mathbf{W}^{\mathrm{T}} \mathbf{S}_{B} \mathbf{W}\right)} \\ {\mathbf{W}^{\mathrm{T}} \mathbf{S}_{W} \mathbf{W}=\mathbf{I}}\end{array}](https://www.zhihu.com/equation?tex=%5Cbegin%7Barray%7D%7Bl%7D%7B%5Cmax+_%7B%5Cmathbf%7Bw%7D%7D+%5Coperatorname%7Btr%7D%5Cleft%28%5Cmathbf%7BW%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7BS%7D_%7BB%7D+%5Cmathbf%7BW%7D%5Cright%29%7D+%5C%5C+%7B%5Cmathbf%7BW%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7BS%7D_%7BW%7D+%5Cmathbf%7BW%7D%3D%5Cmathbf%7BI%7D%7D%5Cend%7Barray%7D)

使该目标函数最大的W的列w必须满足

![\mathbf{S}_{B} \mathbf{w}=\lambda \mathbf{S}_{W} \mathbf{w}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BS%7D_%7BB%7D+%5Cmathbf%7Bw%7D%3D%5Clambda+%5Cmathbf%7BS%7D_%7BW%7D+%5Cmathbf%7Bw%7D)

通过拉格朗日乘数法可以证明，最优解是矩阵 ![\mathbf{S}_{w}^{-1} \mathbf{S}_{B}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BS%7D_%7Bw%7D%5E%7B-1%7D+%5Cmathbf%7BS%7D_%7BB%7D) 的特征值和特征向量。矩阵 ![\mathbf{S}_{w}^{-1} \mathbf{S}_{B}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BS%7D_%7Bw%7D%5E%7B-1%7D+%5Cmathbf%7BS%7D_%7BB%7D) 可能有d个特征值和特征向量，要将向量投影到c-1维，为此挑选出最大的c-1个特征值以及它们对应的特征向量，组成投影矩阵W。

下图是将MNIST数据集投影到3维空间后的结果。

![](https://pic4.zhimg.com/v2-0400b6141a5a58583ca9ff561241c14f_b.png)

LDA是一种有监督的线性降维算法，因为在计算散度矩阵的时候使用了样本标签值。


## **MDS**

MDS（multidimensional scaling）[4]通过计算任意两个样本点之间的距离，使得投影到低维空间之后能够保持这种相对距离而实现投影。

首选计算任意两个样本 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 以及 ![x_{j}](https://www.zhihu.com/equation?tex=x_%7Bj%7D) 之间的距离，然后通过求解下面的最优化问题得到投影结果y

![\min _{\mathbf{y}} \sum_{i=1}^{N} \sum_{j=1}^{N}\left(d_{i j}-\left\|\mathbf{y}_{i}-\mathbf{y}_{j}\right\|\right)^{2}](https://www.zhihu.com/equation?tex=%5Cmin+_%7B%5Cmathbf%7By%7D%7D+%5Csum_%7Bi%3D1%7D%5E%7BN%7D+%5Csum_%7Bj%3D1%7D%5E%7BN%7D%5Cleft%28d_%7Bi+j%7D-%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bi%7D-%5Cmathbf%7By%7D_%7Bj%7D%5Cright%5C%7C%5Cright%29%5E%7B2%7D)

上面目标函数的含义是，投影到低维空间之后，要保证样本之间的距离 ![\left\|\mathbf{y}_{i}-\mathbf{y}_{j}\right\|](https://www.zhihu.com/equation?tex=%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bi%7D-%5Cmathbf%7By%7D_%7Bj%7D%5Cright%5C%7C) 与在原始空间中它们之间的距离 ![d_{ij}](https://www.zhihu.com/equation?tex=d_%7Bij%7D)尽量接近，否则会产生一个大的损失。这就保证了距离较远的样本投影之后依然距离较远，距离较近的样本投影之后距离依然较近。

相比之下非线性降维算法的家族更为庞大。可以分为使用核（kernel）技术的方法，基于流形的降维算法，以及使用神经网络的降维算法。这里介绍前面两类方法，基于神经网络的算法在本文中则不做介绍。


## **基于核技术的方法**

核是机器学习中一种技术，通过对原始数据进行非线性的映射，使得原本线性不可解的问题在另外一个空间中变得可能会线性可解。典型的代表是支持向量机中的核函数，它将原本线性不可分的问题变得可能会线性可分。下图是用高斯核函数的支持向量机所拟合出来的分类边界线，为曲线
![](https://pic2.zhimg.com/v2-6a0ae968247c0dbacff3e2c7d90df58d_b.png)

将核技术用于降维算法，可以使得PCA与LDA这样的线性降维算法能够处理非线性降维问题。

直接用一个函数将x映射为另一个空间中的向量y不易构造，核函数巧妙的避开了此问题，通过用核函数K对两个向量进行映射，等价于先对两个向量进行映射后再做内积

![K\left(\mathbf{x}_{i}, \mathbf{x}_{j}\right)=\phi\left(\mathbf{x}_{i}\right)^{\mathrm{T}} \phi\left(\mathbf{x}_{j}\right)](https://www.zhihu.com/equation?tex=K%5Cleft%28%5Cmathbf%7Bx%7D_%7Bi%7D%2C+%5Cmathbf%7Bx%7D_%7Bj%7D%5Cright%29%3D%5Cphi%5Cleft%28%5Cmathbf%7Bx%7D_%7Bi%7D%5Cright%29%5E%7B%5Cmathrm%7BT%7D%7D+%5Cphi%5Cleft%28%5Cmathbf%7Bx%7D_%7Bj%7D%5Cright%29)

对于核函数以及支持向量机的介绍，可以阅读SIGAI之前的公众号文章“用一张图理解支持向量机的脉络”或阅读《机器学习与应用》一书，相信这是市面上已知的对SVM最清晰透彻的阐述。

## **KPCA**

KPCA（kernel PCA）[5]是核技术与PCA结合的产物。主要的改进在于计算协方差矩阵时使用了核函数，即是经过核函数映射之后的协方差矩阵。与支持向量机类似，可以采用高斯核，多项式核。由于整体原理与PCA类似，在这里不做详细介绍。


## **KLDA**

与KPCA类似，KLDA（kernel PCA）[6-7]是核技术与PCA结合的产物。相对于LDA的主要改进是计算类内散度矩阵与类间散度矩阵的时候使用了核函数。由于整体原理与LDA类似，在这里不做详细介绍。


## **流形学习**

流形是几何中的一个概念，是高维空间中的某种几何结构即空间中的点构成的集合，可以简单的将流形理解成二维空间的曲线，三维空间的曲面等几何体在更高维空间的推广。下图是三维空间中的一个流形

![](https://pic3.zhimg.com/v2-4868cdc28039766024d9bf4087a694aa_b.png)

这是一个卷曲的面。我们相信很多应用问题的数据在高维空间中的分布具有某种几何形状，即位于一个低维的流形附近。同一个人的人脸图像向量在高维空间中可能是一个复杂的形状。流形学习假设原始数据在高维空间的分布位于某一更低维的流形上，基于这个假设来进行数据的分析。对于降维，要保证降维之后的数据同样满足与高维空间流形有关的几何约束关系，由此演化出各种流形降维算法。

## **LLE**

局部线性嵌入[8]（locally linear embedding，简称LLE）将高维数据投影到低维空间中，保持数据点之间的局部线性关系。核心思想是每个点可以由与它相邻的多个点的线性组合而近似重构，投影到低维空间之后要保持这种线性重构关系，即有相同的重构系数。

假设数据集由n个D维向量 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 组成，它们分布于D维空间中的某一流形附近。每个数据点和它的邻居位于或者接近于流形的一个局部线性片段上，即可以用邻居点的线性组合近似重构

![\mathbf{x}_{i} \approx \sum_{j} w_{i j} \mathbf{x}_{j}](https://www.zhihu.com/equation?tex=%5Cmathbf%7Bx%7D_%7Bi%7D+%5Capprox+%5Csum_%7Bj%7D+w_%7Bi+j%7D+%5Cmathbf%7Bx%7D_%7Bj%7D)

权重 ![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D) 为第j个数据点对第i个点的组合权重。权重系数通过最小化下面的重构误差确定

![\min _{w_{i j}} \sum_{i=1}^{n}\left\|\mathbf{x}_{i}-\sum_{j=1}^{n} w_{i j} \mathbf{x}_{j}\right\|^{2}](https://www.zhihu.com/equation?tex=%5Cmin+_%7Bw_%7Bi+j%7D%7D+%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Cleft%5C%7C%5Cmathbf%7Bx%7D_%7Bi%7D-%5Csum_%7Bj%3D1%7D%5E%7Bn%7D+w_%7Bi+j%7D+%5Cmathbf%7Bx%7D_%7Bj%7D%5Cright%5C%7C%5E%7B2%7D)

这里附加了两个约束条件：每个点只由它的邻居来重构，如果 ![x_{j}](https://www.zhihu.com/equation?tex=x_%7Bj%7D) 不在 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 的邻居集合里则权重值为0。另外权重矩阵的每一行元素之和为1，即

![\sum_{j} w_{i j}=1](https://www.zhihu.com/equation?tex=%5Csum_%7Bj%7D+w_%7Bi+j%7D%3D1)

求解该问题可以得到权重系数。

假设算法将向量从D维空间的x映射为d维空间的y。每个点在d维空间中的坐标由下面的最优化问题确定

![\min _{\mathbf{y}_{i}} \sum_{i=1}^{n}\left\|\mathbf{y}_{i}-\sum_{j=1}^{n} w_{i j} \mathbf{y}_{j}\right\|^{2}](https://www.zhihu.com/equation?tex=%5Cmin+_%7B%5Cmathbf%7By%7D_%7Bi%7D%7D+%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bi%7D-%5Csum_%7Bj%3D1%7D%5E%7Bn%7D+w_%7Bi+j%7D+%5Cmathbf%7By%7D_%7Bj%7D%5Cright%5C%7C%5E%7B2%7D)

这里的权重和上一个优化问题的值相同，在前面已经得到，是已知量。这里优化的目标是 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) ，此优化问题等价于求解稀疏矩阵的特征值问题。得到y之后，即完成了降维。

下图是用LLE对MNIST数据集降维后的结果。

![](https://pic4.zhimg.com/v2-827e0f9d8af35ec7ffb35a5b8413350f_b.png)


## **拉普拉斯特征映射**

拉普拉斯特征映[9]射通过对图的拉普拉斯矩阵进行特征值分解而完成数据降维。它为样本集构造相似度图，然后计算拉普拉斯矩阵。如果图的邻接矩阵为W，加权度矩阵为D，拉普拉斯矩阵定义为

![\mathbf{L}=\mathbf{D}-\mathbf{W}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BL%7D%3D%5Cmathbf%7BD%7D-%5Cmathbf%7BW%7D)

算法为样本点构造加权图，图的节点是每一个样本点，边为每个节点与它的邻居节点之间的相似度，每个节点只和它的邻居有连接关系。降维的目标是投影之后保持在高维空间中的距离关系，假设投影后到低维空间后的坐标为 y，它通过最小化如下目标函数实现

![\min _{\mathbf{y}_{i}} \sum_{i=1}^{n} \sum_{j=1}^{n}\left\|\mathbf{y}_{i}-\mathbf{y}_{j}\right\|^{2} w_{i j}](https://www.zhihu.com/equation?tex=%5Cmin+_%7B%5Cmathbf%7By%7D_%7Bi%7D%7D+%5Csum_%7Bi%3D1%7D%5E%7Bn%7D+%5Csum_%7Bj%3D1%7D%5E%7Bn%7D%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bi%7D-%5Cmathbf%7By%7D_%7Bj%7D%5Cright%5C%7C%5E%7B2%7D+w_%7Bi+j%7D)

此函数的含义是如果样本 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 和 ![x_{j}](https://www.zhihu.com/equation?tex=x_%7Bj%7D) 的相似度很高即在高维空间中距离很近，则它们之间的边的权重 ![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D)很大，因此投影到低维空间中后两个点要离得很近，即 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 和 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 要很接近，否则会产生一大个的损失值。求解该目标函数等价于下面的优化问题

![\begin{array}{l}{\min _{\mathrm{y}} \operatorname{tr}\left(\mathbf{Y}^{\mathrm{T}} \mathbf{L} \mathbf{Y}\right)} \\ {\mathbf{Y}^{\mathrm{T}} \mathbf{D Y}=\mathbf{I}}\end{array}](https://www.zhihu.com/equation?tex=%5Cbegin%7Barray%7D%7Bl%7D%7B%5Cmin+_%7B%5Cmathrm%7By%7D%7D+%5Coperatorname%7Btr%7D%5Cleft%28%5Cmathbf%7BY%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7BL%7D+%5Cmathbf%7BY%7D%5Cright%29%7D+%5C%5C+%7B%5Cmathbf%7BY%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7BD+Y%7D%3D%5Cmathbf%7BI%7D%7D%5Cend%7Barray%7D)

其中

![\mathbf{Y}=\left[ \begin{array}{llll}{\mathbf{y}_{1}} & {\mathbf{y}_{2}} & {\dots} & {\mathbf{y}_{d}}\end{array}\right]](https://www.zhihu.com/equation?tex=%5Cmathbf%7BY%7D%3D%5Cleft%5B+%5Cbegin%7Barray%7D%7Bllll%7D%7B%5Cmathbf%7By%7D_%7B1%7D%7D+%26+%7B%5Cmathbf%7By%7D_%7B2%7D%7D+%26+%7B%5Cdots%7D+%26+%7B%5Cmathbf%7By%7D_%7Bd%7D%7D%5Cend%7Barray%7D%5Cright%5D)

为投影后的坐标按列构成的矩阵，这里加上了等式约束条件以消掉y的冗余，选用矩阵D来构造等式约束是因为其主对角线元素即节点的加权度反映了图的每个节点的重要性。通过拉格朗日乘数法可以证明，这个问题的最优解是如下广义值问题的解

![\mathbf{L} \mathbf{f}=\lambda \mathbf{D} \mathbf{f}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BL%7D+%5Cmathbf%7Bf%7D%3D%5Clambda+%5Cmathbf%7BD%7D+%5Cmathbf%7Bf%7D)

这个广义特征值问题的所有特征值非负。最优解为这个广义特征值问题除去0之外的最小的d个广义特征值对应的特征向量，这些向量按照列构成矩阵Y，即为投影结果。


## **局部保持投影**

局部保持投影（Locality preserving projections，简称LPP）[10]通过最好的保持一个数据集的邻居结构信息来构造投影映射，其思路和拉普拉斯特征映射类似，区别在于不是直接得到投影结果而是求解投影矩阵。

假设有样本集 ![\mathbf{X}_{1}, \dots, \mathbf{X}_{m}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BX%7D_%7B1%7D%2C+%5Cdots%2C+%5Cmathbf%7BX%7D_%7Bm%7D) 。这里的目标是寻找一个变换矩阵A，将这些样本点映射到更低维的空间，得到向量 ![\mathbf{y}_{1}, \dots, \mathbf{y}_{m}](https://www.zhihu.com/equation?tex=%5Cmathbf%7By%7D_%7B1%7D%2C+%5Cdots%2C+%5Cmathbf%7By%7D_%7Bm%7D) ，使得 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 能够代表 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D)


![\mathbf{y}_{i}=\mathbf{A}^{\mathrm{T}} \mathbf{x}_{i}](https://www.zhihu.com/equation?tex=%5Cmathbf%7By%7D_%7Bi%7D%3D%5Cmathbf%7BA%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7Bx%7D_%7Bi%7D)


目标函数与拉普拉斯特征映射相同，定义为

![\frac{1}{2} \sum_{i=1}^{m} \sum_{j=1}^{m}\left\|\mathbf{y}_{i}-\mathbf{y}_{j}\right\|^{2} w_{i j}=\frac{1}{2} \sum_{i=1}^{m} \sum_{j=1}^{m}\left\|\mathbf{A}^{\mathrm{T}} \mathbf{x}_{i}-\mathbf{A}^{\mathrm{T}} \mathbf{x}_{j}\right\|^{2} w_{i j}](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7B2%7D+%5Csum_%7Bi%3D1%7D%5E%7Bm%7D+%5Csum_%7Bj%3D1%7D%5E%7Bm%7D%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bi%7D-%5Cmathbf%7By%7D_%7Bj%7D%5Cright%5C%7C%5E%7B2%7D+w_%7Bi+j%7D%3D%5Cfrac%7B1%7D%7B2%7D+%5Csum_%7Bi%3D1%7D%5E%7Bm%7D+%5Csum_%7Bj%3D1%7D%5E%7Bm%7D%5Cleft%5C%7C%5Cmathbf%7BA%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7Bx%7D_%7Bi%7D-%5Cmathbf%7BA%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7Bx%7D_%7Bj%7D%5Cright%5C%7C%5E%7B2%7D+w_%7Bi+j%7D)

所有矩阵的定义与拉普拉斯特征映射相同。投影变换矩阵为

![\mathbf{A}=\left[ \begin{array}{llll}{\mathbf{a}_{1}} & {\mathbf{a}_{2}} & {\dots} & {\mathbf{a}_{d}}\end{array}\right]](https://www.zhihu.com/equation?tex=%5Cmathbf%7BA%7D%3D%5Cleft%5B+%5Cbegin%7Barray%7D%7Bllll%7D%7B%5Cmathbf%7Ba%7D_%7B1%7D%7D+%26+%7B%5Cmathbf%7Ba%7D_%7B2%7D%7D+%26+%7B%5Cdots%7D+%26+%7B%5Cmathbf%7Ba%7D_%7Bd%7D%7D%5Cend%7Barray%7D%5Cright%5D)

即

![\mathbf{y}=\mathbf{A}^{\mathrm{T}} \mathbf{x}](https://www.zhihu.com/equation?tex=%5Cmathbf%7By%7D%3D%5Cmathbf%7BA%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7Bx%7D)

假设矩阵x为所有样本按照列构成的矩阵。上面的最优化问题等价于求解下面的问题

![\begin{array}{c}{\min _{\mathbf{a}} \mathbf{a}^{\mathrm{T}} \mathbf{X} \mathbf{L} \mathbf{X}^{\mathrm{T}} \mathbf{a}} \\ {\mathbf{a}^{\mathrm{T}} \mathbf{X} \mathbf{D} \mathbf{X}^{\mathrm{T}} \mathbf{a}=1}\end{array}](https://www.zhihu.com/equation?tex=%5Cbegin%7Barray%7D%7Bc%7D%7B%5Cmin+_%7B%5Cmathbf%7Ba%7D%7D+%5Cmathbf%7Ba%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7BX%7D+%5Cmathbf%7BL%7D+%5Cmathbf%7BX%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7Ba%7D%7D+%5C%5C+%7B%5Cmathbf%7Ba%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7BX%7D+%5Cmathbf%7BD%7D+%5Cmathbf%7BX%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7Ba%7D%3D1%7D%5Cend%7Barray%7D)

通过拉格朗日乘数法可以证明，此问题的最优解是下面广义特征值问题的解

![\mathbf{X} \mathbf{L} \mathbf{X}^{\mathrm{T}} \mathbf{a}=\lambda \mathbf{X} \mathbf{D} \mathbf{X}^{\mathrm{T}} \mathbf{a}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BX%7D+%5Cmathbf%7BL%7D+%5Cmathbf%7BX%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7Ba%7D%3D%5Clambda+%5Cmathbf%7BX%7D+%5Cmathbf%7BD%7D+%5Cmathbf%7BX%7D%5E%7B%5Cmathrm%7BT%7D%7D+%5Cmathbf%7Ba%7D)

该问题的解即为投影方向。

## **等距映射**

等距映射（Isomap）[11]使用了微分几何中测地线的概念，它希望数据在向低维空间映射之后能够保持流形上的测地线距离。测地线源自于大地测量学，是地球上任意两点之间在球面上的最短路径。在三维空间中两点之间的最短距离是它们之间线段的长度，但如果要沿着地球表面走，最短距离就是测地线的长度，因为不能从地球内部穿过去。这里的测地线就是球面上两点之间大圆上劣弧的长度。算法计算任意两个样本之间的测地距离，然后根据这个距离构造距离矩阵。最后通过距离矩阵求解优化问题完成数据的降维，降维之后的数据保留了原始数据点之间的距离信息。

同样的需要先为样本集构造图，然后计算任意两个节点之间的测地距离，这通过经典的Dijkstra算法实现。假设最短路径长度为$$d\_{G}(i,j)$$，由它构造如下矩阵：

![\mathbf{D}_{G}=\left\{d_{G}(i, j)\right\}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BD%7D_%7BG%7D%3D%5Cleft%5C%7Bd_%7BG%7D%28i%2C+j%29%5Cright%5C%7D)

其元素是所有节点对之间的最短路径长度。然后根据矩阵$$D\_{G}$$构造 d维嵌入y，这通过求解如下最优化问题实现

![\min _{\mathbf{y}} \sum_{i=1}^{N} \sum_{j=1}^{N}\left(d_{G}(i, j)-\left\|\mathbf{y}_{i}-\mathbf{y}_{j}\right\|\right)^{2}](https://www.zhihu.com/equation?tex=%5Cmin+_%7B%5Cmathbf%7By%7D%7D+%5Csum_%7Bi%3D1%7D%5E%7BN%7D+%5Csum_%7Bj%3D1%7D%5E%7BN%7D%5Cleft%28d_%7BG%7D%28i%2C+j%29-%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bi%7D-%5Cmathbf%7By%7D_%7Bj%7D%5Cright%5C%7C%5Cright%29%5E%7B2%7D)

这个问题的解y即为降维之后的向量。可以看到，这个目标函数与MDS及其类似，不同的是用测地距离替代了常用的距离。这个目标函数的意义是向量降维之后任意两点之间的距离要尽量的接近在原始空间中这两点之间的最短路径长度，因此可以认为降维尽量保留了数据点之间的测地距离信息。

下图是用等距映射对MNIST数据集降维后的结果。

![](https://pic3.zhimg.com/v2-26549fb42eb3defcb422f4612d88c1ca_b.png)

## **SNE**

随机近邻嵌入（stochastic neighbor embedding，简称SNE）[12]是一种基于概率的算法，基于如下思想：在高维空间中距离很近的点投影到低维空间中之后也要保持这种近邻关系，在这里距离通过概率体现。假设在高维空间中有两个点样本点 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 和 ![x_{j}](https://www.zhihu.com/equation?tex=x_%7Bj%7D) ， ![x_{j}](https://www.zhihu.com/equation?tex=x_%7Bj%7D) 以![P_{j | i}](https://www.zhihu.com/equation?tex=P_%7Bj+%7C+i%7D) 的概率作为 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 的邻居，将样本之间的欧氏距离转化成概率值，借助于正态分布，此概率的计算公式为

![p_{j \mid  k}=\frac{\exp \left(-\left\|\mathbf{x}_{i}-\mathbf{x}_{j}\right\|^{2} / 2 \sigma_{i}^{2}\right)}{\sum_{k\neq i} \exp \left(-\left\|\mathbf{x}_{i}-\mathbf{x}_{k}\right\|^{2} / 2 \sigma_{i}^{2}\right)}](https://www.zhihu.com/equation?tex=p_%7Bj+%5Cmid++k%7D%3D%5Cfrac%7B%5Cexp+%5Cleft%28-%5Cleft%5C%7C%5Cmathbf%7Bx%7D_%7Bi%7D-%5Cmathbf%7Bx%7D_%7Bj%7D%5Cright%5C%7C%5E%7B2%7D+%2F+2+%5Csigma_%7Bi%7D%5E%7B2%7D%5Cright%29%7D%7B%5Csum_%7Bk%5Cneq+i%7D+%5Cexp+%5Cleft%28-%5Cleft%5C%7C%5Cmathbf%7Bx%7D_%7Bi%7D-%5Cmathbf%7Bx%7D_%7Bk%7D%5Cright%5C%7C%5E%7B2%7D+%2F+2+%5Csigma_%7Bi%7D%5E%7B2%7D%5Cright%29%7D)

假设 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 和 ![x_{j}](https://www.zhihu.com/equation?tex=x_%7Bj%7D) 投影之后对应的点为 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 和 ![y_{j}](https://www.zhihu.com/equation?tex=y_%7Bj%7D) ，在低维空间中对应的近邻概率记为 ![q_{j | i}](https://www.zhihu.com/equation?tex=q_%7Bj+%7C+i%7D) ，计算公式与上面类似，即

![q_{j | i}=\frac{\exp \left(-\left\|\mathbf{y}_{i}-\mathbf{y}_{j}\right\|^{2}\right)}{\sum_{k \neq i} \exp \left(-\left\|\mathbf{y}_{i}-\mathbf{y}_{k}\right\|^{2}\right)}](https://www.zhihu.com/equation?tex=q_%7Bj+%7C+i%7D%3D%5Cfrac%7B%5Cexp+%5Cleft%28-%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bi%7D-%5Cmathbf%7By%7D_%7Bj%7D%5Cright%5C%7C%5E%7B2%7D%5Cright%29%7D%7B%5Csum_%7Bk+%5Cneq+i%7D+%5Cexp+%5Cleft%28-%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bi%7D-%5Cmathbf%7By%7D_%7Bk%7D%5Cright%5C%7C%5E%7B2%7D%5Cright%29%7D)

上面定义的是一个点与它的一个邻居点的概率关系，如果考虑所有其他点，这些概率值构成一个离散型概率分布![P_{i}](https://www.zhihu.com/equation?tex=P_%7Bi%7D) ，是所有样本点成为 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 的邻居的概率。在低维空间中对应的概率分布为 ![Q_{i}](https://www.zhihu.com/equation?tex=Q_%7Bi%7D) ，投影的目标是这两个概率分布尽可能接近，因此需要衡量两个概率分布之间的相似度或距离。在机器学习中一般用KL（Kullback-Leibler）散度衡量两个概率分布之间的距离。

由此得到投影的目标为最小化如下函数

![L\left(\mathbf{y}_{i}\right)=\sum_{i=1}^{l} K L\left(P_{i} | Q_{i}\right)=\sum_{i=1}^{l} \sum_{j=1}^{l} p_{j| } \log \frac{p_{j|i}}{q_{j | i}}](https://www.zhihu.com/equation?tex=L%5Cleft%28%5Cmathbf%7By%7D_%7Bi%7D%5Cright%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bl%7D+K+L%5Cleft%28P_%7Bi%7D+%7C+Q_%7Bi%7D%5Cright%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bl%7D+%5Csum_%7Bj%3D1%7D%5E%7Bl%7D+p_%7Bj%7C+%7D+%5Clog+%5Cfrac%7Bp_%7Bj%7Ci%7D%7D%7Bq_%7Bj+%7C+i%7D%7D)

这里对所有样本点的KL散度求和，l为样本数。把概率的计算公式代入KL散度，可以将目标函数写成所有 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 的函数。得到的最优 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 值即为 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 投影后的结果。

## **t-SNE**

虽然SNE有较好的效果，但训练时难以优化，且容易导致拥挤问题（crowding problem）。t分布随机近邻嵌入（t-distributed Stochastic Neighbor Embedding，简称t-SNE）[13]是对SNE的改进。t-SNE采用了对称的概率计算公式，另外在低维空间中计算样本点之间的概率时使用t分布代替了正态分布。

在SNE中$$p\_{i | j}$$和$$p\_{j | i}$$是不相等的，因此概率值不对称。可以用两个样本点的联合概率替代它们之间的条件概率解决此问题。在高维空间中两个样本点的联合概率定义为

![p_{i j}=\frac{\exp \left(-| | \mathbf{x}_{i}-\mathbf{x}_{j}\left\|^{2} / 2 \sigma^{2}\right)\right.}{\sum_{k\neq l} \exp \left(-\left\|\mathbf{x}_{k}-\mathbf{x}_{l}\right\|^{2} / 2 \sigma^{2}\right)}](https://www.zhihu.com/equation?tex=p_%7Bi+j%7D%3D%5Cfrac%7B%5Cexp+%5Cleft%28-%7C+%7C+%5Cmathbf%7Bx%7D_%7Bi%7D-%5Cmathbf%7Bx%7D_%7Bj%7D%5Cleft%5C%7C%5E%7B2%7D+%2F+2+%5Csigma%5E%7B2%7D%5Cright%29%5Cright.%7D%7B%5Csum_%7Bk%5Cneq+l%7D+%5Cexp+%5Cleft%28-%5Cleft%5C%7C%5Cmathbf%7Bx%7D_%7Bk%7D-%5Cmathbf%7Bx%7D_%7Bl%7D%5Cright%5C%7C%5E%7B2%7D+%2F+2+%5Csigma%5E%7B2%7D%5Cright%29%7D)

显然这个定义是对称的，即 ![p_{i j}=p_{j i}](https://www.zhihu.com/equation?tex=p_%7Bi+j%7D%3Dp_%7Bj+i%7D) 。同样的，低维空间中两个点的联合概率为

![q_{i j}=\frac{\exp \left(- \| \mathbf{y}_{i}-\mathbf{y}_{j}\left\|^{2}\right)\right.}{\sum_{k \neq i} \exp \left(-\left\|\mathbf{y}_{k}-\mathbf{y}_{t}\right\|^{2}\right)}](https://www.zhihu.com/equation?tex=q_%7Bi+j%7D%3D%5Cfrac%7B%5Cexp+%5Cleft%28-+%5C%7C+%5Cmathbf%7By%7D_%7Bi%7D-%5Cmathbf%7By%7D_%7Bj%7D%5Cleft%5C%7C%5E%7B2%7D%5Cright%29%5Cright.%7D%7B%5Csum_%7Bk+%5Cneq+i%7D+%5Cexp+%5Cleft%28-%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bk%7D-%5Cmathbf%7By%7D_%7Bt%7D%5Cright%5C%7C%5E%7B2%7D%5Cright%29%7D)

目标函数采用KL散度，定义为

![L\left(\mathbf{y}_{i}\right)=D_{\mathrm{KL}}(P | Q)=\sum_{i=1}^{l} \sum_{j=1}^{l} p_{i j} \log \frac{p_{i j}}{q_{i j}}](https://www.zhihu.com/equation?tex=L%5Cleft%28%5Cmathbf%7By%7D_%7Bi%7D%5Cright%29%3DD_%7B%5Cmathrm%7BKL%7D%7D%28P+%7C+Q%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bl%7D+%5Csum_%7Bj%3D1%7D%5E%7Bl%7D+p_%7Bi+j%7D+%5Clog+%5Cfrac%7Bp_%7Bi+j%7D%7D%7Bq_%7Bi+j%7D%7D)

但这样定义联合概率会存在异常值问题。如果某一个样本 是异常点即离其他点很远，则所有的 ![\left\|\mathbf{x}_{i}-\mathbf{x}_{j}\right\|^{2}](https://www.zhihu.com/equation?tex=%5Cleft%5C%7C%5Cmathbf%7Bx%7D_%7Bi%7D-%5Cmathbf%7Bx%7D_%7Bj%7D%5Cright%5C%7C%5E%7B2%7D) 都很大，因此与![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 有关的 ![p_{ij}](https://www.zhihu.com/equation?tex=p_%7Bij%7D) 很小，从而导致低维空间中的 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D)对目标函数影响很小。解决方法是重新定义高维空间中的联合概率，具体为

![p_{i j}=\frac{p_{j|i}+p_{i | j}}{2 n}](https://www.zhihu.com/equation?tex=p_%7Bi+j%7D%3D%5Cfrac%7Bp_%7Bj%7Ci%7D%2Bp_%7Bi+%7C+j%7D%7D%7B2+n%7D)

这样能确保对所有的 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) 有 ![\sum_{j} p_{i j}>\frac{1}{2 n}](https://www.zhihu.com/equation?tex=%5Csum_%7Bj%7D+p_%7Bi+j%7D%3E%5Cfrac%7B1%7D%7B2+n%7D) ，因此每个样本点都对目标函数有显著的贡献。这种方法称为对称SNE。

对称SNE虽然对SNE做了改进，但还存在拥挤问题，各类样本降维后聚集在一起而缺乏区分度。解决方法是用t分布替代高斯分布，计算低维空间中的概率值。相比于正态分布，t分布更长尾。如果在低维空间中使用t分布，则在高维空间中距离近的点，在低维空间中距离也要近；但在高维空间中距离远的点，在低维空间中距离要更远。因此可以有效的拉大各个类之间的距离。使用t分布之后，低维空间中的概率计算公式为

![q_{i j}=\frac{\left(1+\left\|\mathbf{y}_{i}-\mathbf{y}_{j}\right\|^{2}\right)^{-1}}{\sum_{k \neq l}\left(1+\left\|\mathbf{y}_{k}-\mathbf{y}_{l}\right\|^{2}\right)^{-1}}](https://www.zhihu.com/equation?tex=q_%7Bi+j%7D%3D%5Cfrac%7B%5Cleft%281%2B%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bi%7D-%5Cmathbf%7By%7D_%7Bj%7D%5Cright%5C%7C%5E%7B2%7D%5Cright%29%5E%7B-1%7D%7D%7B%5Csum_%7Bk+%5Cneq+l%7D%5Cleft%281%2B%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bk%7D-%5Cmathbf%7By%7D_%7Bl%7D%5Cright%5C%7C%5E%7B2%7D%5Cright%29%5E%7B-1%7D%7D)

目标函数同样采用KL散度。下图是t-SNE对MNIST数据集投影的结果。

![](https://pic4.zhimg.com/v2-d4401b83b88597aff95dc01cf7d0b607_b.png)

## **LargeVis**

t-SNE虽然效果好，但有计算量大的问题。从名字就可以看出，LargeVis[14]的目标是大规模数据集的可视化，是对t-SNE的改进。主要改进点是高效的构建kNN图的算法，以及低维空间的概率计算公式，目标函数。

借助于随机投影树，LargeVis可以高效的计算kNN图，以此加速样本点概率值的计算速度。LargeVis在原始空间中计算样本概率分布的方法与t-SNE相同，但计算低维空间中概率分布时做了改进，两个点之间有边连接的概率为

![p\left(e_{i j}=1\right)=f\left(\left\|\mathbf{y}_{i}-\mathbf{y}_{j}\right\|\right)](https://www.zhihu.com/equation?tex=p%5Cleft%28e_%7Bi+j%7D%3D1%5Cright%29%3Df%5Cleft%28%5Cleft%5C%7C%5Cmathbf%7By%7D_%7Bi%7D-%5Cmathbf%7By%7D_%7Bj%7D%5Cright%5C%7C%5Cright%29)

其中

![f(x)=\frac{1}{1+\exp \left(x^{2}\right)}](https://www.zhihu.com/equation?tex=f%28x%29%3D%5Cfrac%7B1%7D%7B1%2B%5Cexp+%5Cleft%28x%5E%7B2%7D%5Cright%29%7D)

目标函数定义为

![\sum_{(i, j) \in E} p_{i j} \log p\left(e_{i j}=1\right)+\sum_{(i, j) \in E} \gamma \log \left(1-p\left(e_{i j}=1\right)\right)](https://www.zhihu.com/equation?tex=%5Csum_%7B%28i%2C+j%29+%5Cin+E%7D+p_%7Bi+j%7D+%5Clog+p%5Cleft%28e_%7Bi+j%7D%3D1%5Cright%29%2B%5Csum_%7B%28i%2C+j%29+%5Cin+E%7D+%5Cgamma+%5Clog+%5Cleft%281-p%5Cleft%28e_%7Bi+j%7D%3D1%5Cright%29%5Cright%29)

其中E为图的边的集合， ![\overline{E}](https://www.zhihu.com/equation?tex=%5Coverline%7BE%7D) 为其补集。下图是LargeVis与t-SNE的效果比较，左侧为t-SNE的降维结果，右侧为LargeVis的降维结果。

![](https://pic3.zhimg.com/v2-a6891a23004bf50232bb3ee1c489fc3e_b.png)

参考文献

[1] Ian T. Jolliffe. Principal Component Analysis. Springer Verlag, New York, 1986.

[2] Ronald A. Fisher. The use of multiple measurements in taxonomic problems. Annals of Eugenics, 7 Part 2: 179-188, 1936.

[3] Geoffrey J. McLachlan. Discriminant Analysis and Statistical Pattern Recognition. Wiley, New York, 1992.

[4] Torgerson, Warren S. (1958). Theory & Methods of Scaling. New York: Wiley. ISBN 978-0-89874-722-5.

[5] Scholkopf, B.,Smola,A.,Mulller,K.-P. Nonlinear component analysis as a kernel eigenvalue problem. Neural Computation, 10(5), 1299-1319, 1998.

[6] Mika, S; Rätsch, G.; Weston, J.; Schölkopf, B.; Müller, KR (1999). Fisher discriminant analysis with kernels. Neural Networks for Signal Processing. IX. pp. 41–48. 

[7] Baudat, G.; Anouar, F. (2000). Generalized discriminant analysis using a kernel approach. Neural Computation. 12 (10): 2385–2404. 

[8] Roweis, Sam T and Saul, Lawrence K. Nonlinear dimensionality reduction by locally linear embedding. Science, 290(5500). 2000: 2323-2326.

[9] Belkin, Mikhail and Niyogi, Partha. Laplacian eigenmaps for dimensionality reduction and data representation. Neural computation. 15(6). 2003:1373-1396.

[10] He Xiaofei and Niyogi, Partha. Locality preserving projections. NIPS. 2003:234-241.

[11] Tenenbaum, Joshua B and De Silva, Vin and Langford, John C. A global geometric framework for nonlinear dimensionality reduction. Science, 290(5500). 2000: 2319-2323.

[12] Geoffrey E Hinton, Sam T Roweis. Stochastic Neighbor Embedding.  neural information processing systems, 2002.

[13] Maaten, Laurens van der, and Geoffrey Hinton. Visualizing data using t-SNE. Journal of machine learning research 9.Nov (2008): 2579-2605.

[14] Visualizing Large-scale and High-dimensional Data. Jian Tang, Jingzhou Liu, Ming Zhang, Qiaozhu Mei. 2016, the web conference.


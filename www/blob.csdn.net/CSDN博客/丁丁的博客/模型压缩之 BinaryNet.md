
# 模型压缩之 BinaryNet - 丁丁的博客 - CSDN博客


2016年12月03日 16:13:02[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：9997标签：[模型压缩																](https://so.csdn.net/so/search/s.do?q=模型压缩&t=blog)[BinaryNet																](https://so.csdn.net/so/search/s.do?q=BinaryNet&t=blog)[二值化																](https://so.csdn.net/so/search/s.do?q=二值化&t=blog)[二值网络																](https://so.csdn.net/so/search/s.do?q=二值网络&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=二值化&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=BinaryNet&t=blog)个人分类：[原创																](https://blog.csdn.net/JackyTintin/article/category/1194487)[人工智能																](https://blog.csdn.net/JackyTintin/article/category/6788737)[深度学习																](https://blog.csdn.net/JackyTintin/article/category/6788745)[
							](https://blog.csdn.net/JackyTintin/article/category/6788737)
[
																								](https://blog.csdn.net/JackyTintin/article/category/1194487)

[
		](https://so.csdn.net/so/search/s.do?q=BinaryNet&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=模型压缩&t=blog)

# 1. 动机
深度学习在图像、语音、文本等领域都取得了巨大的成功，推动了一系列智能产品的落地。但深度模型存在着参数众多，训练和 inference 计算量大的不足。目前，基于深度学习的产品大多依靠服务器端运算能力的驱动，非常依赖良好的网络环境。
很多时候，出于响应时间、服务稳定性和隐私方面的考虑，我们更希望将模型部署在本地（如智能手机上）。为此，我们需要解决模型压缩的问题——将模型大小、内存占用、功耗等降低到本地设备能够承受的范围之内。
# 2. 方法
神经网络具有分布式的特点——特征表征和计算都分散于各个层、各个参数。因此，神经网络在结构上天然具有冗余的特点。**冗余**是神经网络进行压缩的前提。
压缩模型一般可以有几种常见的方法：
## 2.1 使用小模型
### 设计小模型
可以直接将模型大小做为约束，在模型结构设计和选择时便加以考虑。对于全连接，使用 bottleneck 是一个有效的手段（如[LSTMP](https://arxiv.org/abs/1402.1128)）。[Highway](https://arxiv.org/abs/1512.04280)，[ResNet](https://arxiv.org/abs/1610.05256)，[DenseNet](https://arxiv.org/abs/1611.09326)等带有 skip connection 结构的模型也被用来设计窄而深的网络，从而减少模型整体参数量和计算量。对 CNN 网络，[SqueezeNet](https://arxiv.org/abs/1602.07360)通过引入1 x 1的小卷积核、减少 feature map 数量等方法，在分类精度与 AlexNet 相当的前提下，将模型大小压缩在 1M 以内，而模型大小仅是 Alexnet 的**50分之一**。更新的还有 MobileNet、ShuffleNet 等。
### 模型小型化
一般而言，相比于小模型，大模型更容易通过训练得到更优的性能。那么，能否用一个较小的模型，“提炼”出训练好的大模型的知识能力，从而使得小模型在特定任务上，达到或接近大模型的精度？Knowledge Distilling(e.g.[1](https://arxiv.org/abs/1503.02531)、[2](https://arxiv.org/abs/1412.6550))便尝试解决这一问题。knowledge distilling 将大模型的输出做为 soft target 来训练小模型，达到知识“凝练“的效果。实验表明，distilling 方法在 MNIST 及声学建模等任务上有着很好的表现。
## 2.2 利用稀疏性
我们也可以通过在模型结构上引入稀疏性，从而达到减少模型参数量的效果。
### 裁剪已有模型
将训练好的模型进行裁剪的方法，至少可以追溯到90年代。[Optimal Brain Damage](http://yann.lecun.com/exdb/publis/pdf/lecun-90b.pdf)和[Optimal Brain Surgeon](http://ee.caltech.edu/Babak/pubs/conferences/00298572.pdf)通过一阶或二阶的梯度信息，删除不对性能影响不显著的连接，从而压缩模型规模。
### 学习稀疏结构
稀疏性也可以通过训练获得。更近的一系列工作（Deep compression:[a](https://arxiv.org/abs/1506.02626)、[b](https://arxiv.org/abs/1510.00149)、[c](https://arxiv.org/abs/1602.01528)及[HashedNets](https://arxiv.org/abs/1504.04788)）在控制模型性能的前提下，学习稀疏的模型结构，从而极大的压缩模型规模。
## 2.3 降低运算精度
不同传统的高性能计算，神经网络对计算精度的要求不高。目前，基本上所有神经网络都采用单精度浮点数进行训练（这在很大程度上决定着 GPU 的架构设计）。已经发布的[NVIDIA Pascal](https://devblogs.nvidia.com/parallelforall/mixed-precision-programming-cuda-8/)架构的最大特色便是原生的支持半精度（half float）运算。在服务端，**FPGA**等特殊硬件在许多数据中心得到广泛[应用](https://www.nextplatform.com/2016/11/24/fpgas-give-microsoft-von-neumann-tax-break/)，多采用低精度（8 bit）的定点运算。
### 参数量化
除了使用低精度浮点运算（float32, float16）外，量化参数是另一种利用简化模型的有效方法。
将参数量化有如下二个优势：
* 减少模型大——将 32 或 16 位浮点数量化为 8 位甚至更少位的定点数，能够极大减少模型占用的空间；
* 加速运算——相比于复杂的浮点运算，量化后的定点运算更容易利用特殊硬件（FPGA，ASIC）进行加速。
上面提到的 Deep Compression 使用不同的位数量化网络。[Lin 等](http://jmlr.org/proceedings/papers/v48/linb16.html)的工作，在理论上讨论上，在不损失性能的前提下，CNN 的最优量化策略。此外，还有量化[CNN](https://arxiv.org/abs/1605.04711)和[RNN](https://arxiv.org/abs/1608.06902)权值的相关工作。
### 参数二值化
量化的极限是二值化，即每一个参数只占用一个 bIt。本文讨论的正是这个种压缩模型的方法。
# 3. BinaryNet
BinaryNet [1] 研究对象是前馈网络（全连接结构或卷积结构）（这方法在 RNN 上并不成功 [4]）。这里，我们更关心权值的二值化对 inference 的精度和速度的影响，而不关心模型的训练速度（量化梯度以加速模型训练的工作可以参见 [3]）。
前馈模型（卷积可以看成是一种特殊的全连接）可以用如下公式表示：

$$
x^k =  \sigma(W^k \cdot x^{k-1})
$$
其中，$x^k$为第$k$层的输入，$W^k$为第$k$层的权值矩阵，$\sigma(\cdot)$为非线性激活函数。由于[Batch Normalizaiton](https://arxiv.org/abs/1502.03167)的引入，偏置项$b$成为冗余项，不再考虑。
## 3.1. 二值化权值和激活
首先，我们定义取符号操作：

$$
sign(x) = \{
 \begin{array}
 +1\ if\ x \ge 0, \\
 -1\ 其他
 \end{array}
$$
在 BinaryNet 中，网络权值为 +1 或 -1，即可以用 1bit 表示，这一点与[BinaryConnect](https://github.com/MatthieuCourbariaux/BinaryConnect)相同。更进一步，BinaryNet 使用了输出为二值的激活函数，即：

$$
\sigma(x) = sign(x)
$$
这样，除了第一层的输入为浮点数或多位定点数外，其他各层的输入都为 1 bit。
## 3.2. 训练
BinaryNet 二值化权值和激活的思路很容易理解，但关键在于，如何有效地训练网络这样一个二值网络。
**[1] 提出的解决方案是：权值和梯度在训练过程中保持全精度（full precison），也即，训练过程中，权重依然为浮点数，训练完成后，再将权值二值化，以用于 inference。**
### 权值
在训练过程中，权值为 32 位的浮点数，且取值值限制在 [-1, 1] 之间，以保持网络的稳定性。为此，训练过程中，每次权值更新后，需要对权值$W$的大小进行检查，$W=max(min(1, W), -1)$。
### 前向
前向运算时，我们首先得到二值化的权值：$W_b^k = sign(W^k), k=1,\cdots ,n$
然后，用$W^k_b$代替$W^k$：

$$
x^k = \sigma (BN(W_b^k \cdot x^{k-1}) = sign(BN(W_b^k \cdot x^{k-1}))
$$
其中，$BN(\cdot)$为 Batch Normalization 操作。
### 后向
根据误差反传算法（Backpropagation，BP），由于$sign(\cdot)$的导数（几乎）处处为零，因此，$W$通过 BP 得到的误差$\Delta W$为零 ，因此不能直接用来更新权值。为解决这个问题，[1] 采用**straight-through estimator**（Section 1.3） 的方法，用$\Delta W_b$代替$\Delta W$。这样，BinaryNet 就可以和序普通的实值网络一样，使用梯度下降法进行优化。
另外，如果激活值（二值化之前）的绝对值大于1，相应的梯度也要置零，否则会影响训练效果。
## 3.4. 性能
### 模型精度
BinaryNet 在 MNIST (MLP)  ，CIFAR10、SVHN（CNN）上取得了不错的结果（表1第二列）。
|数据集|论文结果|squared hinge loss (同论文)|xent loss|
|---|---|---|---|
|MNIST (MLP)|0.96%|1.06%|1.02%|
|CIFAR10 (CNN)|11.40%|11.92%|11.91%|
|SVHN (CNN)|2.80%|2.94%|2.82%|
*表 1*不同数据集上错误率
### 压缩效果
二值化网络在运算速度、内存占用、能耗上的优势是显而易见的，这也是我们对二值化感兴趣的原因。[1] 中给出了这方面的一些分析，具体可以参见 [1]（Section 3），此处不再赘述。
# 4. Source Code
## 4.1 训练
BinaryNet[1]的作者给出了[theano](https://github.com/MatthieuCourbariaux/BinaryNet)和[torch](https://github.com/itayhubara/BinaryNet)两个版本，两者之间略有不同。theano 采用确定性（deterministic）的二值化方式，而 torch 是随机化（stochastic）的二值化，并且 torch 版对 Batch Normalization 操作也进行了离散化处理。具体差异可以参见论文。
根据文章 theano 版本的实现，我们有基于[Keras 的实现](https://github.com/DingKe/nn_playground/tree/master/binarynet)。这个版本利用了一个[trick](https://github.com/DingKe/BinaryNet/blob/master/binary_ops.py#L8)，实现了梯度的 straight-through estimator。
> 理想情况下，theano 和 tensorflow 在做 Graph 优化时，应该能够优化掉这个 trick 带来的性能开销，但对于MLP， tensorflow 的后端明显比 theano 慢（~235s vs. ~195s），但不清楚是否是两者对 Graph 优化能力差异造成的。
在 MNIST、CIFAR10 和 SVHN 数据集上，基本复现了文章的结果（见表1. 三、四列）
> 与文章声称的不同，目标函数分别选择交叉熵（crossentropy, xent）与合叶损失（L2-SVM）时，在三个数据集上的性能几乎没有判别，甚至交叉熵还要略好一些。
另外，感兴趣的读者可以参考基于 pytorch[的实现](https://github.com/DingKe/pytorch_workplace/tree/master/binary)。
## 4.2 Inference
正如上面介绍的，BinaryNet 的最大优点是可以 XNOR-计数 运算替代复杂的乘法-加法操作。[1] 给出了二值网络 inference 的基于 CUDA 的 GPU[参考实现](https://github.com/MatthieuCourbariaux/BinaryNet/tree/master/Run-time)。另外，还有基于[CPU 的实现](https://github.com/codekansas/tinier-nn/tree/master/eval)（其基于 tensorflow 的训练代码有些小问题）。[2] 报告了基于**FPGA**的实现及加速效果。
# 5. 结语
在小型任务上，BinaryNet 完全有希望满足精度要求。目前手里没有真实应用的数据（如语音的静音检测），不能进一步验证可行性。
至于 BinaryNet 在大型任务上的性能，从 [5][6] 报告的 ImageNet 准确率来看， 性能损失还是比较大的。更高的量化精度似是必须的[4][7]。
此外，根据实验经验，BinaryNet 的训练不太稳定，需要较小的学习率，收敛速度明显慢于实值网络。
---

# References
Courbariaux et al.[Binarized Neural Networks: Training Neural Networks with Weights and Activations Constrained to +1 or −1](https://arxiv.org/abs/1602.02830).
Umuroglu et al.[FINN: A Framework for Fast, Scalable Binarized Neural Network Inference](http://www.idi.ntnu.no/~yamanu/2017-fpga-finn-preprint.pdf).
Lin et al.[Neural Networks with Few Multiplications](https://arxiv.org/abs/1510.03009).
Ott et al.[Recurrent Neural Networks With Limited Numerical Precision](https://arxiv.org/abs/1608.06902).
Zhou et al.[DoReFa-Net: Training Low Bitwidth Convolutional Neural Networks with Low Bitwidth Gradients](https://arxiv.org/abs/1606.06160).
Rastegari et al.[XNOR-Net: ImageNet Classification Using Binary Convolutional Neural Networks](https://arxiv.org/abs/1603.05279).
Hubara et al.[Quantized Neural Networks: Training Neural Networks with Low Precision Weights and Activations](https://arxiv.org/abs/1609.07061).
# Further Readings
Anderson et al.[The High-Dimensional Geometry of Binary Neural Networks](https://arxiv.org/abs/1705.07199).
Yang et al.[BMXNet: An Open-Source Binary Neural Network Implementation Based on MXNet](https://arxiv.org/abs/1705.09864).
Zhang et al.[ShuffleNet: An Extremely Efficient Convolutional Neural Network for Mobile Devices](https://arxiv.org/abs/1707.01083.).
Howard et al.[MobileNets: Efficient Convolutional Neural Networks for Mobile Vision Applications](https://arxiv.org/abs/1704.04861).
Lu et al.[The Expressive Power of Neural Networks: A View from the Width](https://arxiv.org/abs/1709.02540).
Lin et al.[Deep Gradient Compression: Reducing the Communication Bandwidth for Distributed Training](https://arxiv.org/abs/1712.01887).
Sakr et al.[TRUE GRADIENT-BASED TRAINING OF DEEP BINARY ACTIVATED NEURAL NETWORKS VIA CONTINUOUS BINARIZATION]().
Liu et al.[Bi-Real Net: Enhancing the Performance of 1-bit CNNs With Improved Representational Capability and Advanced Training Algorithm](https://arxiv.org/abs/1808.00278).
Jacob et al.[Quantization and Training of Neural Networks for Efficient Integer-Arithmetic-Only Inference](https://arxiv.org/abs/1712.05877).
Peters et al.[Probabilistic Binary Neural Networks](https://arxiv.org/abs/1809.03368).



# 深度学习在CTR预估中的应用 | CTR深度模型大盘点 - Paper weekly - CSDN博客


2018年04月11日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：620


作者丨辛俊波
单位丨腾讯
研究方向丨CTR预估，推荐系统

# 前言

深度学习凭借其强大的表达能力和灵活的网络结构在 NLP、图像、语音等众多领域取得了重大突破。在广告领域，预测用户点击率（Click Through Rate，简称 CTR）领域近年也有大量关于深度学习方面的研究，仅这两年就出现了不少于二十多种方法。

**本文就近几年 CTR 预估领域中学术界的经典方法进行探究，并比较各自之间模型设计的初衷和各自优缺点**。通过十种不同 CTR 深度模型的比较，不同的模型本质上都可以由基础的底层组件组成。

**本文中出现的变量定义：**

n: 特征个数，所有特征 one-hot 后连接起来的整体规模大小

f: 特征 field 个数，表示特征类别有多少个

k: embedding 层维度，在 FM 中是隐向量维度

H1: 深度网络中第一个隐层节点个数，第二层 H2，以此类推

# 深度学习模型

**1. Factorization-machine (FM)**

FM 模型可以看成是线性部分的 LR，还有非线性的特征组合 xixj 交叉而成，表示如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIUC3OdaMcarRjF5AIAe0jaUbNG5pyicy0c4oZxs9AajuiaUXeT92icTk9g/640)

其中 vi 是第 i 维特征的隐向量，长度 k<<n，包含 k 个描述特征的因子。参数个数为 k*n。所有包含 xi 的非零组合特征都可以用来训练 vi，缓解数据稀疏问题。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIKNPiagR5f26wJJg1gU2Mc6DgzGuiar51vWAblOsDLGcb1y3Ac3WwMb5Q/640)
**▲**图1：FM模型结构

图 1 是从神经网络的角度表示 FM， 可以看成底层为特征维度为 n 的离散输入，经过 embedding 层后，对 embedding 层线性部分（LR）和非线性部分（特征交叉部分）累加后输出。

FM 等价于 FM + embedding，待学习的参数如下：

LR 部分：1+n

embedding 部分：n*k

FM 下文中将作为各种网络模型的基础组件。

**2. Deep Neural Network (DNN)**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIzicz5MJPm9fU3CMsQKqt5wN6dl4pHaHsvkc9ls0seULQK5Zr2X0fK5g/640)
**▲**图2：DNN模型结构

图 2 是经典的 DNN 网络， 结构上看就是传统的多层感知机（MultiLayer Perceptron，简称 MLP）。

在 MLP 网络中，输入是原始的特征 n 维特征空间，假设第一层隐层节点数为 H1，第二层为 H2，以此类推。在第一层网络中，需要学习的参数就是 n*H1。

对于大多数 CTR 模型来说，特征体系都极其庞大而且稀疏，典型的特征数量级 n 从百万级到千万级到亿级甚至更高，这么大规模的 n 作为网络输入在 CTR 预估的工业界场景中是不可接受的。

下面要讲到的大多数深度学习 CTR 网络结构，都围绕着如何将 DNN 的高维离散输入，通过 embedding 层变成低维稠密的输入工作来展开。

DNN 待学习参数：

n*H1+H1*H2+H2*H3+H3*o

o 为输出层大小，在 CTR 预估中为 1。

DNN（后文称 MLP）也将作为下文各种模型的基础组件之一。

**3. Factorization-machine supported Neural Networks (FNN)**

在上述的 DNN 中，网络的原始输入是全部原始特征，维度为 n，通常都是百万级以上。然而特征维度 n 虽然空间巨大，但如果归属到每个特征所属的 field（维度为 f），通常 f 维度会小很多。

**如果有办法将每个特征用其所属的 field 来表示，原始输入将大大减少不少**。Factorisation-machine Supported Neural Networks，简称 FNN 就是基于这种思想提出来的。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHI7ib6BrI0shUMQ3tJXkAQz5U8Zx0YnMrXUlU9k4sqPkgYUuUxxsWANAQ/640)
**▲**图3：FNN模型结构

FNN 假设每个 field 有且只有一个值为 1，其他均为 0。x 为原始输入的特征，它是大规模离散稀疏的。它可以分成 n 个 field，每一个 field 中，只有一个值为 1，其余都为 0（即 one hot）。

field i 的输入可以表示成 x[start_i: end_i]，Wi 为field i 的 embedding 矩阵。z为 embedding 后的向量，是一个 k 维的向量，它由一次项 wi ，二次项 vi=(vi1,vi2,…vik) 组成，其中 k 是 FM 中二次项的向量的维度。而后面的 l1，l2 则为神经网络的全连接层的表示。

除此之外，**FNN 还具有以下几个特点**：

**FM 参数需要预训练**

FM 部分的 embedding 需要预先进行训练，所以 FNN 不是一个 end-to-end 模型。在其他论文中，有试过不用 FM 初始化 embedding，而用随机初始化的方法，要么收敛速度很慢，要么无法收敛。有兴趣的同学可以实验验证下。

**无法拟合低阶特征**

FM 得到的 embedding 向量直接 concat 连接之后作为 MLP 的输入去学习高阶特征表达，最终的 DNN 输出作为 CTR 预估值。因此，FNN 对低阶信息的表达比较有限。

**每个 field 只有一个非零值的强假设**

FNN 假设每个 fileld 只有一个值为非零值，如果是稠密原始输入，则 FNN 失去意义。对于一个 fileld 有几个非零值的情况，例如用户标签可能有多个，一般可以做 average/sum/max 等处理。

本质上讲，FNN = LR+DEEP = LR + embedding + MLP，参数如下：

LR 部分： 1+n

embedding 部分： n*k

MLP 部分： f*k*H1+H1*H2+H2

可以看到，对比 DNN，在进入 MLP 部分之前，网络的输入由 n 降到了 f*k（f 为 field 个数，几十到几百之间，k 为隐向量维度，一般 0~100）。

**4. Product-based Neural Network (PNN)**

FNN 的 embedding 层直接 concat 连接后输出到 MLP 中去学习高阶特征。

PNN，全称为 Product-based Neural Network，认为在 embedding 输入到 MLP 之后学习的交叉特征表达并不充分，提出了一种 product layer 的思想，既基于乘法的运算来体现体征交叉的 DNN 网络结构，如图 4 所示。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIqBIV1Vic24T75d98Iw0KphnxYJ8HHKLGJb1Hib5qATTLCGLaFBSdiaF4Q/640)
**▲**图4：PNN模型结构

对比 FNN 网络，PNN 的区别在于中间多了一层 Product Layer 层。Product Layer 层由两部分组成，左边 z 为 embedding 层的线性部分，右边为 embedding 层的特征交叉部分。

**除了 Product Layer 不同，PNN 和 FNN 的 MLP 结构是一样的**。这种 product 思想来源于，在 CTR 预估中，认为特征之间的关系更多是一种 and“且”的关系，而非 add"加”的关系。例如，性别为男且喜欢游戏的人群，比起性别男和喜欢游戏的人群，前者的组合比后者更能体现特征交叉的意义。

根据 product 的方式不同，可以分为 inner product (IPNN) 和 outer product (OPNN)，如图 5 所示。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHI0gibCpegCw4gGXgiaIaPwfqOHmW8jQyuIZVcB29HdTf1S3woibQ5YFZoQ/640)
**▲**图5：PNN (左图 IPNN，右图 OPNN)

Product Layer 的输出为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIXqpjK4mzoaWad8LhtMbFUcxNj1oJIic8e85UOokiajEbWNdvg89JcswQ/640)

**Inner Product-based Neural Network**

IPNN 的叉项使用了内积 g(fi, fj) = <fi, fj>。f 个 filed，两两求内积共计交叉项 p 部分的参数共 f*(f-1)/2（f 为特征的 field 个数，原始论文里用的 N）个，线性部分 z 部分参数共 f*k 个。需要学习的参数为：

FM 部分：1+ n + n*k

product部分：(f*k + f*(f-1)/2)*H1

MLP 部分：H1*H2+H2*1

**Outer Product-based Neural Network**

OPNN 用矩阵乘法来表示特征的交叉，g(fi, fj)=fifit。f 个 field 两两求矩阵乘法，交叉项 p 共 f*(f-1)/2*k*k 个参数。线性部分 z 部分参数共 f*k 个。需要学习的参数为：

FM 部分： 1+ n + n*k

product 部分：(f*k + f*(f-1)/2*k*k)*H1

MLP 部分：H1*H2+H2*1


**5. Wide & Deep Learning (Wide & Deep)**

前面介绍的两种变体 DNN 结构 FNN 和 PNN，都在 embedding 层对输入做处理后输入 MLP，让神经网络充分学习特征的高阶表达，deep 部分是有了，对高阶的特征学习表达较强，但 wide 部分的表达是缺失的，模型对于低阶特征的表达却比较有限。

Google 在 2016 年提出了大名鼎鼎的 Wide & Deep 结构正是解决了这样的问题。Wide & Deep 结合了 Wide 模型的优点和 Deep 模型的优点，网络结构如图 6 所示，Wide 部分是 LR 模型，Deep 部分是 DNN 模型。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIr5qJWL1UYFS2HYT5kel50lQSCOpicklw9SDmLdZ2Hq0gk700Z5IsNibg/640)
**▲**图6：Wide & Deep 模型结构

在这个经典的Wide & Deep模型中，Google 提出了两个概念，Generalization（泛化性）和 Memory（记忆性）。

**Memory（记忆性）**

Wide 部分长处在于学习样本中的高频部分，优点是模型的记忆性好，对于样本中出现过的高频低阶特征能够用少量参数学习；缺点是模型的泛化能力差，例如对于没有见过的 ID 类特征，模型学习能力较差。

**Generalization（泛化性）**

Deep 部分长处在于学习样本中的长尾部分，优点是泛化能力强，对于少量出现过的样本甚至没有出现过的样本都能做出预测（非零的 embedding 向量）;缺点是模型对于低阶特征的学习需要用较多参才能等同 Wide 部分效果，而且泛化能力强某种程度上也可能导致过拟合出现 bad case。

除此之外，**Wide & Deep 模型还有如下特点**：

**人工特征工程**

LR 部分的特征，仍然需要人工设计才能保证一个不错的效果。因为 LR 部分是直接作为最终预测的一部分，如果作为 Wide 部分的 LR 特征工程做的不够完善，将影响整个 Wide & Deep 的模型精度。

**联合训练**

模型是 end-to-end 结构，Wide 部分和 Deep 部分是联合训练的。

**Embedding 层 Deep 部分单独占有**

LR 部分直接作为最后输出，因此 embedding 层是 Deep 部分独有的。

Wide & Deep 等价于 LR + embedding + MLP，需要学习的网络参数有：

LR: 1+n

embedding 部分：n*k

MLP 部分：f*k*H1 + H1*H2 + H2*1

**6. Factorization-Machine based Neural Network (DeepFM)**

Google 提出的 Wide & Deep 框架固然强大，但由于 Wide 部分是个 LR 模型，仍然需要人工特征工程。

Wide & Deep给整个学术界和工业界提供了一种框架思想。基于这种思想，华为诺亚方舟团队结合 FM 相比 LR 的特征交叉的功能，将Wide & Deep部分的 LR 部分替换成 FM 来避免人工特征工程，于是有了 DeepFM，网络结构如图 7 所示。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIicXmXb8ue4UhdY5kqff2ssKJ61ib2PYXdKsBib3Jg8Nc68fqDPD303KEg/640)
**▲**图7：DeepFM 模型结构

比起 Wide & Deep 的 LR 部分，DeepFM 采用 FM 作为 Wide 部分的输出，FM 部分如图 8 所示。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIwIicDvhqQ6MV6afibbWeh9HaRbjCkicQPtqricKpPHGLAiaibpILLlEBUv3A/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHI6bV4d6p3U4Ql4GCtTSCIu0gh8ic2iclMf17MvIaMygpnb71kY9hDDLicg/640)

**▲**图8：DeepFM 模型中的 FM 部分结构

除此之外，**DeepFM 还有如下特点**：

**低阶特征表达**

Wide 部分取代 WDL 的 LR，比 FNN 和 PNN 更能捕捉低阶特征信息。

**Embedding 层共享**

Wide & Deep 部分的 embedding 层得需要针对 Deep 部分单独设计；而在 DeepFM 中，FM 和 Deep 部分共享 embedding 层，FM 训练得到的参数既作为 Wide 部分的输出，也作为 DNN 部分的输入。

**end-end训练**

embedding 和网络权重联合训练，无需预训练和单独训练。

DeepFM 等价于 FM + embedding + DNN：

FM 部分：1+n

embedding 部分：n*k

DNN 部分：f*k*H1 + H1*H2+H1

通过 embedding 层后，FM 部分直接输出没有参数需要学习，进入 DNN 部分的参数维度从原始 n 维降到 f*k 维。

**7. Neural Factorization Machines (NFM)**

前面的 DeepFM 在 embedding 层后把 FM 部分直接 concat 起来（f*k 维，f 个 field，每个 filed 是 k 维向量）作为 DNN 的输入。

**Neural Factorization Machines，简称 NFM，提出了一种更加简单粗暴的方法**，在 embedding 层后，做了一个叫做 BI-interaction 的操作，让各个 field 做 element-wise 后 sum 起来去做特征交叉，MLP 的输入规模直接压缩到 k 维，和特征的原始维度 ｎ 和特征 field 维度 f 没有任何关系。

网络结构如图 9 所示：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHINQCDibxtgypVhiafLSrDUwsVTBbkOhMtdicz98ehTTrd6GibOnALSdjS4Q/640)
**▲**图9：NFM 模型结构

这里论文只画出了其中的 Deep 部分，Wide 部分在这里省略没有画出来。

Bi-interaction 听名字很高大上，其实操作很简单：就是让 f 个 field 两两 element-wise 相乘后，得到 f*(f-1)/2 个向量，然后直接 sum 起来，最后得到一个 k 维的向量。所以该层没有任何参数需要学习。

NFM 等价于 FM + embedding + MLP，需要学习的参数有：

FM部分：1+n

embedding部分：n*k

MLP部分：k*H1 + H1*H2+…+Hl*1

NFM 在 embedding 做了 bi-interaction 操作来做特征的交叉处理，优点是网络参数从 n 直接压缩到 k（比 FNN 和 DeepFM 的 f*k 还少），降低了网络复杂度，能够加速网络的训练得到模型；但同时这种方法也可能带来较大的信息损失。

**8. Attention Neural Factorization Machines (AFM)**

前面提到的各种网络结构中的 FM 在做特征交叉时，让不同特征的向量直接做交叉，基于的假设是各个特征交叉对 CTR 结果预估的贡献度是一样的。这种假设其实是不合理的，不同特征在做交叉时，对 CTR 预估结果的贡献度是不一样的。

Attention Neural Factorization Machines，简称 NFM 模型，利用了近年来在图像、NLP、语音等领域大获成功的 attention 机制，**在前面讲到的 NFM 基础上，引入了 attention 机制来解决这个问题**。

AFM 的网络结构如图 10 所示。和 NFM 一样，这里也省略了 Wide 部分，只画出了 Deep 部分结构。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHICF3p5TIQiav7HmiaNL9V114iaX8YnTd3Npk7dhBLAoVVic1jwZTniaLBf2A/640)
**▲**图10：AFM 模型结构

AFM 的 embedding 层后和 NFM 一样，先让 f 个 field 的特征做了 element-wise product 后，得到 f*(f-1)/2 个交叉项。

和 NFM 直接把这些交叉项 sum 起来不同，AFM 引入了一个 Attention Net，认为这些交叉特征项每个对结果的贡献是不同的，例如 xi 和 xj 的权重重要度，用 aij 来表示。

**从这个角度来看，其实 AFM 就是个加权累加的过程**。Attention Net 部分的权重 aij 不是直接学习，而是通过如下公式表示：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHI0Mj97KVUb6zNE1SHICFNhHIicUkquJHicQcic0JbCzrSr5k3KKeuaqywQ/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIovTxwibL2jTmQbDAyF20p5k2DhjIU39eDgpyRia66ppGpuTA2bAic6OwA/640)

这里 t 表示 attention net 中的隐层维度，k 和前面一样，为 embedding 层的维度。所以这里需要学习的参数有 3 个，W, b, h，参数个数共 t*k+2*t 个。

得到 aij 权重后，对各个特征两两点积加权累加后，得到一个 k 维的向量，引入一个简单的参数向量 pT，维度为 k 进行学习，和 Wide 部分一起得到最终的 AFM 输出。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHI0UgkXkJZcPoKG5WvUMc7LVskjtTicNicdNvVVWL4QEMXrrggnlZ3bqxQ/640)

**总结 AFM 的网络结构来说，有如下特点**：

**Attention Network**

AFM 的亮点所在，通过一个 Attention Net 生成一个关于特征交叉项的权重，然后将 FM 原来的二次项直接累加，变成加权累加。**本质上是一个加权平均**，学习 xjxj 的交叉特征重要性。

**Deep Network**

没有 Deep，卒。

Attention Net 学习得到的交叉项直接学些个 pt 参数就输出了，少了 DNN 部分的表达，对高阶特征部分的进一步学习可能存在瓶颈。另外，FFM 其实也引入了 field 的概念去学习 filed 和 featrue 之间的权重。

没有了 Deep 部分的 AFM，和优化的 FFM 上限应该比较接近。

AFM 等价于 FM + embedding + attention + MLP（一层），需要学习的参数有：

FM部分参数：1+n

Embedding部分参数：n*k

Attention Network部分参数：k*t + t*2

MLP部分参数：k*1

**9. Deep&Cross Network (DCN)**

在 CTR 预估中，特征交叉是很重要的一步，但目前的网络结构，最多都只学到二级交叉。

LR 模型采用原始人工交叉特征，FM 自动学习 xi 和 xj 的二阶交叉特征，而 PNN 用 product 方式做二阶交叉，NFM 和 AFM 也都采用了 Bi-interaction 的方式学习特征的二阶交叉。对于更高阶的特征交叉，只有让 Deep 去学习了。

为解决这个问题，Google 在 2017 年提出了**Deep & Cross Network**，简称 DCN 的模型，可以任意组合特征，而且不增加网络参数。图 11 为 DCN 的结构。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIibTVUeQQBsUIyEkRdBJSG2icXfDQBQP3nXL3Ujj2OYlJWWfCAibOia66SA/640)
**▲**图11：DCN 模型结构

整个网络由 4 部分组成：

**Embedding and Stacking Layer**

之所以不把 embedding 和 stacking 分开来看，是因为很多时候，embedding 和 stacking 过程是分不开的。

前面讲到的各种 XX-based FM 网络结构，利用 FM 学到的 v 向量可以很好地作为 embedding。

而在很多实际的业务结构，可能已经有了提取到的 embedding 特征信息，例如图像的特征 embedding，text 的特征 embedding，item 的 embedding 等，还有其他连续值信息，例如年龄，收入水平等，这些 embedding 向量 stack 在一起后，一起作为后续网络结构的输入。

当然，这部分也可以用前面讲到的 FM 来做 embedding。为了和原始论文保持一致，这里我们假设 X0 向量维度为 d（上文的网络结构中为 k），这一层的做法就是简单地把各种 embedding 向量 concat 起来。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIe80pl3ic2F83y91ujeibUKjznkSBSaIHPqwNLzxkhuYMzDvu7L0mNvTg/640)

**Deep Layer Network**

在 Embedding and Stacking Layer 之后，网络分成了两路，一路是传统的 DNN 结构。表示如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHId6SPUREUABbJJqmibRteQX8ic830ARiaPzbkOw6g2QbbJLcqr3lJRIPNg/640)

为简化理解，假设每一层网络的参数有 m 个，一共有 Ld 层，输入层由于和上一层连接，有 d*m 个参数（d 为 x0 向量维度），后续的 Ld-1 层，每层需要 m*(m+1) 个参数，所以一共需要学习的参数有 d*m+m*(m+1)*(Ld-1)。最后的输出也是个 m 维向量 Hl2。

**Cross Layer Network**

Embedding and Stacking Layer 输入后的另一路就是 DCN 的重点工作了。假设网络有 L1 层，每一层和前一层的关系可以用如下关系表示：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIC8icsB4cWhX72HfynP250UxhnLTtXxqNz5E1Az8QCDs6rZlzicRXicREA/640)

可以看到 f 是待拟合的函数，xl 即为上一层的网络输入。需要学习的参数为 wl 和 bl，因为 xl 维度为 d， 当前层网络输入 xl+1 也为 d 维，待学习的参数 wl 和 bl 也都是 d 维度向量。

因此，每一层都有 2*d 的参数（w 和 b）需要学习，网络结构如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIQHqaY5w0ff3OKM6DIJMBf9G6hXOxrN5q5NMqMzBcKE3cG8MibZ4aibiaw/640)

经过 Lc 层的 Cross Layer Network 后，在该 layer 最后一层 Lc 层的输出为 Lc2 的 d 维向量。

**Combination Output Layer**

经过 Cross Network 的输出 XL1（d 维）和 Deep Network 之后的向量输入（m 维）直接做 concat，变为一个 d+m 的向量，最后套一个 LR 模型，需要学习参数为 1+d+m。

总结起来，DCN 引入的 Cross Network 理论上可以表达任意高阶组合，同时每一层保留低阶组合，参数的向量化也控制了模型的复杂度。

DCN 等价于 embedding + cross + deep + LR：

embedding 部分参数：根据情况而定

cross 部分参数：2*d*Lc（Lc 为 cross 网路层数）

deep 部分参数：d*(m+1)+m*(m+1)*(Ld-1)，Ld 为深度网络层数，m 为每层网络参数

LR 部分参数：1+d+m

**10. Deep Interest Network (DIN)**

最后介绍阿里在 2017 年提出的**Deep Interest Network**，简称 DIN 模型。与上面的 FNN，PNN 等引入低阶代数范式不同，DIN 的核心是基于数据的内在特点，引入了更高阶的学习范式。

用户的兴趣是多种多样的，从数学的角度来看，用户的兴趣在兴趣空间是一个多峰分布。在预测 CTR 时，用户 embedding 表示的兴趣维度，很多是和当前 item 是否点击无关的，只和用户兴趣中的局部信息有关。

因此，受 attention 机制启发，DIN 在 embedding 层后做了一个 action unit 的操作，对用户的兴趣分布进行学习后再输入到 DNN 中去，网络结构如图 12所示：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHI6Iu1UYKwCgeuib8M6kQiaql1a4h7SV960BxkiaUPqfk9YKiabqgrKDAXmQ/640)
**▲**图12：DIN 模型结构

DIN 把用户特征、用户历史行为特征进行 embedding 操作，视为对用户兴趣的表示，之后通过 attention network，对每个兴趣表示赋予不同的权值。

Vu：表示用户最终向量

Vi：表示用户兴趣向量（shop_id, good_id..）

Va：表示广告表示向量

Wi: 对于候选广告，attention 机制中该兴趣的权重


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIEHGo4C8IOl4BgKMMNz3VoN65t8hHgRPvPicWRfXaB2ibIcO7ibOXBB6Aw/640)

可以看到，对于用户的每个兴趣向量 Vi，都会通过学习该兴趣的权重 Vi, 来作为最终的用户表示。

# 写在最后

前面介绍了 10 种深度学习模型的网络结构，总结起来如下所表示：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIwdFyNiboPYgkgDWUYsV58LUeVM9nafwVgHf61HMlFnWjsibOaQvZlgYQ/640)

各种 CTR 深度模型看似结构各异，其实大多数可以用如下的通用范式来表达：

**input->embedding**：把大规模的稀疏特征 ID 用 embedding 操作映射为低维稠密的 embedding 向量。

**embedding 层向量**：concat, sum, average pooling 等操作，大部分 CTR 模型在该层做改造。

**embedding->output**：通用的 DNN 全连接框架，输入规模从 n 维降为 k*f 维度甚至更低。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIQiccVpfBvkKRJeLNia12eMLTKuh3usj6Yr29otBfKdLTf1hns9KiarYtg/640)
**▲**图13：通用深度学习模型结构

其中，embedding vector 这层的融合是深度学习模型改造最多的地方，该层是进入深度学习模型的输入层，embedding 融合的质量将影响 DNN 模型学习的好坏。

个人总结大体有以下 4 种操作，当然后续可能会有越来越多其他的变形结构。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglALhF8BN58iaeJ6my9LUdHIPf11J4CgibGjKppaAqj0icbLGmibThTsaDzesib7KhXFI7ib5P9RcvTsMBA/640)
**▲**图14：embedding 层融合方式

另外，DNN 部分，业界也有很多或 state-of-art 或很 tricky 的方法，都可以在里面进行尝试，例如 dropout，在 NFM 的 Bi-interaction 中可以尝试以一定概率 dropout 掉交叉特征增前模型的泛化能力等。

**结语**

CTR 预估领域不像图像、语音等领域具有连续、稠密的数据以及空间、时间等的良好局部相关性，CTR 预估中的大多数输入都是离散而且高维的，特征也分散在少量不同的 field 上。

要解决这样的一个深度学习模型，面临的第一个问题是怎么把输入向量用一个 embedding 层降维策划那个稠密连续的向量，如本文介绍的用 FM 去做预训练，或者和模型一起联合训练，或者其他数据源提取的 embedding 特征向量去做 concat。

其次，在宽和深的大战中，在 Google 提出了 Wide & Deep 模型框架后，这套体系基本已成为业内的基本框架。无论 Wide 部分或者 Deep 怎么改造，其实本质上还是一些常见组件的结合，或者改造 Wide，或者改造 Weep，或者在 Wide 和 Deep 的结合过程中进行改造。

CTR 预估领域方法变化层出不穷，但万变不离其宗，各种模型本质上还是基础组件的组合，如何结合自己的业务、数据、应用场景去挑选合适的模型应用，可能才是真正的难点所在。

# 参考文献

[1] Factorization Machines
[2] Wide & Deep Learning for Recommender Systems
[3] Deep Learning over Multi-Field Categorical Data: A Case Study on User Response Prediction
[4] Product-based Neural Networks for User Response Prediction
[5] DeepFM: A Factorization-Machine based Neural Network for CTR Prediction
[6] Neural Factorization Machines for Sparse Predictive Analytics
[7] Attentional Factorization Machines: Learning the Weight of Feature Interactions via Attention Networks
[8] Deep & Cross Network for Ad Click Predictions
[9] Deep Interest Network for Click-Through Rate Prediction


![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgmsvubgibQtWV5t7M3ETKt3bbXiaAothCErMicibic9QCUBpxkuibuht62MGcCTcLyAxqGrsUXbv254InDA/640?)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****作 者 招 募****\#**

**[让你的文字被很多很多人看到，喜欢我们不如加入我们](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)**

我是彩蛋
**解锁新功能：热门职位推荐！**

PaperWeekly小程序升级啦

**今日arXiv√猜你喜欢√****热门职位****√**

找全职找实习都不是问题
**解锁方式**
1. 识别下方二维码打开小程序
2. 用PaperWeekly社区账号进行登陆
3. 登陆后即可解锁所有功能

**职位发布**
请添加小助手微信（**pwbot02**）进行咨询

**长按识别二维码，使用小程序**
账号注册![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/b96CibCt70iaajvl7fD4ZCicMcjhXMp1v6UYhfOp924bYQUqYCVgoT9tY9zibIyUB4m4udOibRHibe4ZiboyZXLGXa16A/640?)paperweek.ly

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/640?)



**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 进入作者知乎专栏



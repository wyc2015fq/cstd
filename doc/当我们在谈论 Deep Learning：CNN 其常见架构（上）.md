# 当我们在谈论 Deep Learning：CNN 其常见架构（上）

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

50 人赞同了该文章

**本系列意在长期连载分享，内容上可能也会有所增删改减；**

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

## **引言**

CNN（Convolution Neural Network，卷积神经网络）作为一种经典的 DNN 结构，上世纪90年代就已经被提出了，却沉寂近20年。有趣的是，在2012年 AlexNet 大放异彩后，CNN 在随后几年近乎卷席了整个图像处理领域，很有点戏剧性。近些年，CNN 在语音处理、NLP 领域也发展迅速。本文会介绍 CNN 的相关概念，并具体介绍一些有趣的 CNN 架构。

## **CNN**

CNN 其实可以看作 DNN 的一种特殊形式。它跟传统 DNN 标志性的区别在于两点，Convolution Kernel 以及 Pooling。

## Convolution Kernel

**卷积**

说起 Convolution Kernel，首先要解释一下 Convolution（卷积）。一般我们接触过的都是一维信号的卷积，也就是



在信号处理中，![x[n]](https://www.zhihu.com/equation?tex=x%5Bn%5D)是输入信号，![h[n]](https://www.zhihu.com/equation?tex=h%5Bn%5D)是单位响应。于是输出信号![y[n]](https://www.zhihu.com/equation?tex=y%5Bn%5D)就是输入信号![x[n]](https://www.zhihu.com/equation?tex=x%5Bn%5D)响应的延迟叠加。这也就是一维卷积本质：加权叠加/积分。

那么对于二维信号，比如图像，卷积的公式就成了



假设现在 Convolution Kernel 大小是![3\times 3](https://www.zhihu.com/equation?tex=3%5Ctimes+3)，我们就可以化简上式为

![img](https://pic1.zhimg.com/80/v2-0770096e7d37a636be715ed9ddeeb63c_hd.png)

看公式很繁琐，我们画个图看看，假如 Convolution Kernel 如下图

![img](https://pic4.zhimg.com/80/v2-d2b2c461b55e439e3d0ee0dc974e963f_hd.png)

那么，从 Input Image 到 Output Image 的变化如下

![img](https://pic3.zhimg.com/80/v2-90aa7dc35c5385b928a14ee8a464fa1a_hd.png)

可以看出，其实二维卷积一样也是加权叠加/积分。需要注意的是，其中 Convolution Kernel 进行了水平和竖直方向的翻转。

**Convolution Kernel 的意义**

Convolution Kernel 其实在图像处理中并不是新事物，Sobel 算子等滤波算子，一直都在被用于边缘检测等工作中，只是以前被称为 Filter。图像处理的同学应该有印象。

Convolution Kernel 的一个属性就是局部性。即它只关注局部特征，局部的程度取决于 Convolution Kernel 的大小。比如用 Sobel 算子进行边缘检测，本质就是比较图像邻近像素的相似性。

也可以从另外一个角度理解 Convolution Kernel 的意义。学过信号处理的同学应该记得，时域卷积对应频域相乘。所以原图像与 Convolution Kernel 的卷积，其实对应频域中对图像频段进行选择。比如，图像中的边缘和轮廓属于是高频信息，图像中区域强度的综合考量属于低频信息。在传统图像处理里，这些物理意义是指导设计 Convolution Kernel 的一个重要方面。

**CNN 的 Convolution Kernel**

CNN 中的 Convolution Kernel 跟传统的 Convolution Kernel 本质没有什么不同。仍然以图像为例，Convolution Kernel 依次与 Input 不同位置的图像块做卷积，得到 Output，如下图。

![img](https://pic3.zhimg.com/80/v2-90aa7dc35c5385b928a14ee8a464fa1a_hd.png)

同时，CNN 有一些它独特的地方，比如各种定义，以及它属于 DNN 的那些属性：

1. CNN 可以看作是 DNN 的一种简化形式，Input 和 Output 是 DNN 中的 Layer，Convolution Kernel 则是这两层连线对应的![w](https://www.zhihu.com/equation?tex=w)，且与 DNN 一样，会加一个参数 Bias ![b](https://www.zhihu.com/equation?tex=b)
2. 一个 Convolution Kernel 在与 Input 不同区域做卷积时，它的参数是固定不变的。放在 DNN 的框架中理解，就是对 Output Layer 中的神经元而言，它们的![w](https://www.zhihu.com/equation?tex=w)和![b](https://www.zhihu.com/equation?tex=b)是相同的，只是与 Input Layer 中连接的节点在改变。在 CNN 里，这叫做 Shared Weights and Biases
3. 在 CNN 中，Convolution Kernel 可能是高维的。假如输入是![m \times n \times k](https://www.zhihu.com/equation?tex=m+%5Ctimes+n+%5Ctimes+k)维的，那么一般 Convolution Kernel 就会选择为![d \times d \times k](https://www.zhihu.com/equation?tex=d+%5Ctimes+d+%5Ctimes+k)维，也就是与输入的 Depth 一致
4. 最重要的一点，在 CNN 中，Convolution Kernel 的权值其实就是![w](https://www.zhihu.com/equation?tex=w)，因此不需要提前设计，而是跟 DNN 一样利用 GD 来优化
5. 如上面所说，Convolution Kernel 卷积后得到的会是原图的某些特征（如边缘信息），所以在 CNN 中，Convolution Kernel 卷积得到的 Layer 称作 Feature Map
6. 一般 CNN 中一层会含有多个 Convolution Kernel，目的是学习出 Input 的不同特征，对应得到多个 Feature Map。又由于 Convolution Kernel 中的参数是通过 GD 优化得到而非设定的，于是![w](https://www.zhihu.com/equation?tex=w)的初始化就显得格外重要了

以上内容中关于 CNN 的 GD 推导，我并没有细讲。有兴趣的同学自行 Google 即可。

（由于有同学感兴趣，CNN 的 GD 推导已补充在“[当我们在谈论 Deep Learning：CNN 其常见架构（下）](https://zhuanlan.zhihu.com/p/27235732)”）

## Pooling

Pooling 的本质，其实是采样。Pooling 对于输入的 Feature Map，选择某种方式对其进行压缩。如下图，表示的就是对 Feature Map ![2 \times 2](https://www.zhihu.com/equation?tex=2+%5Ctimes+2)邻域内的值，选择最大值输出到下一层，这叫做 Max-Pooling。于是一个![2N \times 2N](https://www.zhihu.com/equation?tex=2N+%5Ctimes+2N)的 Feature Map 被压缩到了![N \times N](https://www.zhihu.com/equation?tex=N+%5Ctimes+N)。

![img](https://pic2.zhimg.com/80/v2-bbcea7d03f0ebd97c7d07c5d133fab5d_hd.png)

除此之外，还有Mean-Pooling，Stochastic-Pooling 等。它们的具体实现如名称所示，具体选择哪一个则取决于具体的任务。

Pooling 的意义，主要有两点：

第一个显而易见，就是减少参数。通过对 Feature Map 降维，有效减少后续层需要的参数

另一个则是 Translation Invariance。它表示对于 Input，当其中像素在邻域发生微小位移时，Pooling Layer 的输出是不变的。这就增强了网络的鲁棒性，有一定抗扰动的作用。

## 小结

从上面我们可以看出来，CNN 里结构大都对应着传统图像处理某种操作。区别在于，以前是我们利用专家知识设计好每个操作的细节，而现在是利用训练样本+优化算法学习出网络的参数。在实际工程中，我们也必须根据实际物理含义对 CNN 结构进行取舍。

但是，随着 Convolution 的堆叠，Feature Map 变得越来越抽象，人类已经很难去理解了。为了攻克这个黑箱，现在大家也都在尝试各种不同的方式来对 CNN 中的细节进行理解，因为如果没有足够深的理解，或许很难发挥出 CNN 更多的能力。不过，这就是另外一个很大的课题了。

接下来的部分，会介绍 CNN 一路发展以来，一些有趣架构。

## **CNN 架构发展八卦**

CNN 的架构发展可以参考刘昕博士总结的图

![img](https://pic1.zhimg.com/80/v2-1430e3c733985b462e29c096fc940924_hd.png)

这里简单八卦一下 CNN 架构的变迁史。

LeNet-5 是最经典的 CNN 架构，上面介绍的 Convolutional Kernel 和 Pooling 它都有了。它在1998年由 LeCun 提出，用于对“Mnist手写数字数据集”进行分类。不过效果并不比当时手工设计的特征有明显提升，因此并没有太大反响。

十来年后，Alex Krizhevsky 在2012年提出 AlexNet，凭借它在 ILSVRC2012 的 ImageNet 图像分类项目中获得冠军，且错误率比上一年冠军下降十多个百分点。举圈震惊。

随后几年，CNN 卷席整个图像处理领域，ILSVRC 每年被刷榜，也就出现了我们常见的这张错误率下降趋势的图。当然，也伴随着如图所示的 Layer 数的激增。

![img](https://pic1.zhimg.com/80/v2-fcb9e82c6b63d99406a17cbe56b4a5ac_hd.png)

下面，我们对一些有趣的架构进行详细介绍。

## **LeNet-5**

LeNet5 是 LeCun 在1998年“Gradient-Based Learning Applied to Document. Recognition”中提出的网络架构。用于对“Mnist手写数字数据集”进行分类。其具体的结构如下图

![img](https://pic1.zhimg.com/80/v2-6e1dd62f1da1ebe4dc35926910b730e0_hd.png)

- Input 是一个手写图像。
- C1 层由6个 Feature Map 组成，其中每个神经元与 Input 的![5 \times 5](https://www.zhihu.com/equation?tex=5+%5Ctimes+5)的邻域相连。这些连接对应的![w](https://www.zhihu.com/equation?tex=w)就是上面所说的 Convolution Kernel。当然，与 DNN 一样，这里还有一个参数 Bias ![b](https://www.zhihu.com/equation?tex=b)。所以，此层有![(5*5+1)*6=156](https://www.zhihu.com/equation?tex=%285%2A5%2B1%29%2A6%3D156)个参数。而如果不使用共享权重，则会需要![(5x5+1)x(28x28)x6 = 122304](https://www.zhihu.com/equation?tex=%285x5%2B1%29x%2828x28%29x6+%3D+122304)个参数。
- S2 层即下采样层，对应着现在的 Pooling 操作后的层。其依然是6个 Channel，每个神经元与上一层![2 \times 2](https://www.zhihu.com/equation?tex=2+%5Ctimes+2)的邻域相连。作者在此层进行的操作是加权平均，再加上 bias，并将结果输入到激活函数 Sigmoid 函数中。因此，此层共有![2*6=12](https://www.zhihu.com/equation?tex=2%2A6%3D12)个参数。
- C3 层由16个 Feature Map 组成，其中每个神经元与 S2 的![5 \times 5](https://www.zhihu.com/equation?tex=5+%5Ctimes+5)的邻域相连。不过，当时作者对连接进行了一些设计，让每个 Feature Map 仅与上一层部分 Channel 相连。只是这种 trick 现在也很少用到，这里就不细讲了。
- S4 层也是下采样层，与 S2 是意义一样。由于有16个 Channel，因此共![2*16=32](https://www.zhihu.com/equation?tex=2%2A16%3D32)个参数。
- C5 层由120个 Feature Map 组成，其中每个神经元与 S4 每个 Channel 的![5 \times 5](https://www.zhihu.com/equation?tex=5+%5Ctimes+5)的邻域相连。恰巧，这里 C3 的每一个 Feature Map 都是![1 \times 1](https://www.zhihu.com/equation?tex=1+%5Ctimes+1)，显得好像跟 S4 是全连接，不过这仅仅是恰巧罢了。
- F6 由84个神经元组成，与 C5 全连接。并将结果输入到激活函数，得到每个神经元的状态。这里激活函数使用的是双曲正切函数（tanh）。此层共![(120+1)*84=10164](https://www.zhihu.com/equation?tex=%28120%2B1%29%2A84%3D10164)个参数。而之所以使用84个神经元，是因为有人曾将 ASCII 码绘制于![12 \times 7](https://www.zhihu.com/equation?tex=12+%5Ctimes+7)的 bit-map 上，如下图，作者希望此层的输出有类似的效果。

![img](https://pic4.zhimg.com/80/v2-7275b1bcf926639d0b6c44dff2a9a207_hd.png)

- Output 是10个神经元，分别代表十个数字。作者还使用欧式径向基函数（Euclidean Radial Basis Function）对数据进行了一些变换，有兴趣的同学可以自行参考原文。

网络结构设计好之后，选择 Loss Function，并利用 GD 学习网络参数，这些都是套路了。最后，LeNet-5 每一层的输出可以用以下这张图来辅助理解。

![img](https://pic3.zhimg.com/80/v2-0d995bd71fa2444d1e6fe06b939a9e7e_hd.png)

## **尾巴**

戏谑地说，CNN 最值得学习的地方应该是起名字的能力。原本平常的操作，在 CNN 都换上了高逼格的马甲，令人感觉专业满满。比如你可以跟人介绍你的室友是“彼格-耶勒”，大家可能会以为是个什么海外华侨，直到你牵出你家大黄。

抛开玩笑话，CNN 也已不止局限在传统图像和语音处理领域，正快速往其他领域扩散，如 NLP。现在借助 Tensorflow 或者 Keras，搭建 CNN 已经是比较简单的事情了，大家都可以试一试。

## **本系列其他文章：**

Supervised Learning：

当我们在谈论 Deep Learning：DNN 与 Backpropagation

当我们在谈论 Deep Learning：DNN 与它的参数们（壹）

当我们在谈论 Deep Learning：DNN 与它的参数们（贰）

当我们在谈论 Deep Learning：DNN 与它的参数们（叁）

当我们在谈论 Deep Learning：CNN 其常见架构（上）

Unsupervised Learning：
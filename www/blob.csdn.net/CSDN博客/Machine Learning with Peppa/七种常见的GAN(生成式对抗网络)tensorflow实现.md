# 七种常见的GAN(生成式对抗网络)tensorflow实现 - Machine Learning with Peppa - CSDN博客





2018年12月01日 17:30:16[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：469








代码在我的GITHUB：[https://github.com/Y1ran/GAN-Network-Library-Tensorflow](https://github.com/Y1ran/GAN-Network-Library-Tensorflow)

下载使用前记得star哦~ 除了GAN以外还包括几种变分自编码器的tensorflow实现，版本为Python3

## 一、GAN

生成式对抗网络（[GAN](https://baike.baidu.com/item/GAN/22181905), Generative Adversarial Networks ）是一种[深度学习](https://baike.baidu.com/item/%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0/3729729)模型，是近年来复杂分布上[无监督学习](https://baike.baidu.com/item/%E6%97%A0%E7%9B%91%E7%9D%A3%E5%AD%A6%E4%B9%A0/810193)最具前景的方法之一。模型通过框架中（至少）两个模块：生成模型（Generative Model）和判别模型（Discriminative Model）的互相[博弈](https://baike.baidu.com/item/%E5%8D%9A%E5%BC%88/4669968)学习产生相当好的输出。原始 GAN 理论中，并不要求 G 和 D 都是神经网络，只需要是能拟合相应生成和判别的函数即可。但实用中一般均使用深度神经网络作为 G 和 D 。一个优秀的GAN应用需要有良好的训练方法，否则可能由于神经网络模型的自由性而导致输出不理想。

GAN，Generative Adversarial Network是目前非常火也是非常有潜力的一个发展方向，原始的GAN模型存在着无约束、不可控、噪声信号z很难解释等问题，近年来，在原始GAN模型的基础上衍生出了很多种模型，如：条件——CGAN、卷积——DCGAN等等

## 二、InfoGAN

2014年，Ian J. Goodfellow提出了生成对抗网络：Generative Adversarial Networks，通过generator和discriminator的对抗学习，最终可以得到一个与real data分布一致的fake data，但是由于generator的输入z是一个连续的噪声信号，并且没有任何约束，导致GAN无法利用这个z，并将z的具体维度与数据的语义特征对应起来，并不是一个Interpretable（可解释） Representation，而这正好是InfoGAN的出发点，它试图利用z，寻找一个可解释的表达，于是它将z进行了拆解，一是不可压缩的噪声z，二是可解释的 隐变量c，称作为latent code，而我们希望通过约束c与生成数据之间的关系，可以使得c里面包含有对数据的可解释的信息，如对MNIST数据，c可以分为categorical latent code代 来表数字种类信息（0~9），以及continuous latent code来表示倾斜度、笔画粗细等等。


## 三、CGAN

条件生成式对抗网络（CGAN）是对原始GAN的一个扩展，生成器和判别器都增加额外信息 yy为条件, yy 可以使任意信息,例如类别信息,或者其他模态的数据。如 Figure 1 所示，通过将额外信息 yy 输送给判别模型和生成模型,作为输入层的一部分,从而实现条件GAN。在生成模型中,先验输入噪声 p(z)p(z) 和条件信息 yy 联合组成了联合隐层表征。对抗训练框架在隐层表征的组成方式方面相当地灵活。类似地，条件 GAN 的目标函数是带有条件概率的二人极小极大值博弈（two-player minimax game ）

### 四、DCGAN

DCGAN是继GAN之后比较好的改进，其主要的改进主要是在网络结构上，到目前为止，DCGAN的网络结构还是被广泛的使用，DCGAN极大的提升了GAN训练的稳定性以及生成结果质量。

论文的主要贡献是：

◆  为GAN的训练提供了一个很好的网络拓扑结构。

◆  表明生成的特征具有向量的计算特性。


## 五、WGAN

与DCGAN不同，WGAN主要从损失函数的角度对GAN做了改进，损失函数改进之后的WGAN即使在全链接层上也能得到很好的表现结果，WGAN对GAN的改进主要有：

◆  判别器最后一层去掉sigmoid

◆  生成器和判别器的loss不取log

◆  对更新后的权重强制截断到一定范围内，比如[-0.01，0.01]，以满足论文中提到的lipschitz连续性条件。

◆  论文中也推荐使用SGD， RMSprop等优化器，不要基于使用动量的优化算法，比如adam，但是就我目前来说，训练GAN时，我还是adam用的多一些。


## 六、LSGAN

其实原理部分可以一句话概括，即使用了最小二乘损失函数代替了GAN的损失函数。但是就这样的改变，缓解了GAN训练不稳定和生成图像质量差多样性不足的问题。

事实上，作者认为使用JS散度并不能拉近真实分布和生成分布之间的距离，使用最小二乘可以将图像的分布尽可能的接近决策边界



## 七、BEGAN

BEGAN的主要贡献：

◆  提出了一种新的简单强大GAN，使用标准的训练方式，不加训练trick也能很快且稳定的收敛

◆  对于GAN中G，D的能力的平衡提出了一种均衡的概念（GAN的理论基础就是goodfellow理论上证明了GAN均衡点的存在，但是一直没有一个准确的衡量指标说明GAN的均衡程度）

◆  提出了一种收敛程度的估计，这个机制只在WGAN中出现过。作者在论文中也提到，他们的灵感来自于WGAN，在此之前只有wgan做到了

◆  提出了一种收敛程度的估计，这个机制只在WGAN中出现过。作者在论文中也提到，他们的灵感来自于WGAN







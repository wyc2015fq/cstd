# 生成式对抗网络（Generative Adversarial Networks，GANs） - Joe的博客 - CSDN博客





2018年01月04日 15:40:00[Joe-Han](https://me.csdn.net/u010089444)阅读数：21060








# **1. 简介**

首先简要介绍一下生成模型（Generative model）与判别模型（Discriminative mode）的概念：
- 生成模型：对联合概率进行建模，从统计的角度表示数据的分布情况，刻画数据是如何生成的，收敛速度快，例如朴素贝叶斯，GDA，HMM等。
- 判别模型：对条件概率$P(Y|X)$进行建模，不关心数据如何生成，主要是寻找不同类别之间的最优分类面，例如LR，SVM等。

判别模型在深度学习乃至机器学习领域取得了巨大成功，其本质是将样本的特征向量映射成对应的label；而生成模型由于需要大量的先验知识去对真实世界进行建模，且先验分布的选择直接影响模型的性能，因此此前人们更多关注于判别模型方法。生成式对抗网络（Generative Adversarial Networks，GANs）是蒙特利尔大学的Goodfellow Ian于2014年提出的一种生成模型， 在之后引起了业内人士的广泛关注与研究。

GANs中包含了两个模型，一个是生成模型$G$，另一个是判别模型$D$，下面通过一个生成图片的例子来解释两个模型的作用：
- 生成模型$G$：不断学习训练集中真实数据的概率分布，目标是将输入的随机噪声转化为可以以假乱真的图片（生成的图片与训练集中的图片越相似越好）
- 判别模型$D$：判断一个图片是否是真实的图片，目标是将生成模型$G$产生的“假”图片与训练集中的“真”图片分辨开。

GANs的实现方法是让$D$和$G$进行博弈，训练过程中通过相互竞争让这两个模型同时得到增强。由于判别模型$D$的存在，使得 $G$ 在没有大量先验知识以及先验分布的前提下也能很好的去学习逼近真实数据，并最终让模型生成的数据达到以假乱真的效果（即$D$无法区分$G$生成的图片与真实图片，从而$ G $和$D $达到某种纳什均衡）。

# **2. 生成对抗网络**

GANs中生成模型和判别模型的选择没有强制限制，在Ian的论文中，判别模型$D$和生成模型$G$均采用多层感知机。GANs定义了一个噪声$p_z(x)$ 作为先验，用于学习生成模型$G$ 在训练数据$x$上的概率分布$p_g$，$G(z)$表示将输入的噪声$z$映射成数据（例如生成图片）。$D(x)$代表$x$ 来自于真实数据分布$p_{data}$而不是$p_g$的概率。据此，优化的目标函数定义如下minmax的形式： 

![图片名称](https://img-blog.csdn.net/20180103224338513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

原论文在参数的更新过程，是对$D$更新$k$次后，才对$G$更新1次。上式中的minmax可理解为当更新$D$时，需要最大化上式，而当更新$G$时，需要最小化上式，详细解释如下：

- 
在对判别模型$D$的参数进行更新时：对于来自真实分布$p_{data}$的样本$x$而言，我们希望$D(x)$的输出越接近于1越好，即$logD(x)$越大越好；对于通过噪声$z$生成的数据$G(z)$而言，我们希望$D(G(z))$尽量接近于0（即$D$能够区分出真假数据），因此$log(1-D(G(z)))$也是越大越好，所以需要$max D$。

- 
在对生成模型$G$的参数进行更新时：我们希望$G(z)$尽可能和真实数据一样，即$p_g=p_{data}$。因此我们希望$D(G(z))$尽量接近于1，即$log(1-D(G(z)))$越小越好，所以需要$min G$。需要说明的是，$logD(x)$是与$G(z)$无关的项，在求导时直接为0。


原论文中对GANs理论上的有效性进行了分析，即当固定$G$更新$D$时，最优解为$D^{*}(x)=\frac{p_{data}(x)}{p_{data}(x)+p_{g}(x)}$；而在更新$G$时，目标函数取到全局最小值当且仅当$p_g=p_{data}$。最后两个模型博弈的结果是$G$可以生成以假乱真的数据$G(z)$。而$D$难以判定$G$生成的数据是否真实，即$D(G(z)) = 0.5$。

下面这张图解释了这一博弈过程： 

![图片名称](https://img-blog.csdn.net/20180104115745382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


第一阶段只有判别模型$D$参与。将训练集中的样本$x$作为$D$的输入，输出0-1之间的某个值，数值越大意味着样本$x$为真实数据的可能性越大。在这个过程中，我们希望$D$尽可能使输出的值逼近1。第二阶段中，判别模型$D$和生成模型$G$都参与，首先将噪声$z$输入$G$，$G$从真实数据集里学习概率分布并产生假的样本，然后将假的样本输入判别模型$D$，这一次$D$将尽可能输入数值0。所以在这个过程中，判别模型$D$相当于一个监督情况下的二分类器，数据要么归为1，要么归为0。

下图给出了一个更为直观的解释： 

![图片名称](https://img-blog.csdn.net/20180104145938525?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其中，蓝色虚线为判别模型$D$的分布，黑色虚线为真实数据的分布$p_{data}$，绿色实线为生成模型$G$学习的分布$p_g$。下方的水平线为均匀采样噪声$z$的区域，上方的水平线为数据$x$的区域。朝上的箭头表示将随机噪声转化成数据，即$x=G(z)$。从图（a）到图（b）给出了一个GANs的收敛过程。图(a)中$p_g$与$p_{data}$存在相似性，但还未完全收敛，$D$是个部分准确的分类器。图(b)中，固定$G$更新$D$，收敛到 $D^{*}(x)= \frac{p_{data}(x)}{p_{data}(x)+p_{g}(x)}$ 。图(c)中对$G$进行了1次更新，$D$的梯度引导$G(z)$移向更可能分类为真实数据的区域。图(d)中，训练若干步后，若$G$和$D$均有足够的capacity，它们接近某个稳定点，此时$p_g=p_{data}$。判别模型将无法区分真实数据分布和生成数据分布，即$D(x)=0.5$。

论文中给出的算法流程如下： 

![图片名称](https://img-blog.csdn.net/20180104152407335?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# **3. GANs的应用**

对抗网络的一个主要缺点是训练过程不稳定，为了提高训练的稳定性，文献[2]提出了 Conditional Generative Adversarial Nets （CGAN），通过把无监督的 GAN 变成半监督或者有监督的模型，从而为 GAN 的训练加上一点目标，其优化的目标函数为： 

![图片名称](https://img-blog.csdn.net/20180105131059790?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


CGAN在生成模型$G$和判别模型$D$的建模中均引入了条件变量 y，这里y可以是label或者其他数据形态，将y和GAN原有的输入合并成一个向量作为CGAN的输入。这个简单直接的改进被证明很有效，并广泛用于后续的相关工作中。CGAN模型的示意图如下所示： 

![图片名称](https://img-blog.csdn.net/20180105132108339?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


文献[3]提出了一种Deep Convolutional Generative Adversarial Networks（DCGAN），将对抗网络与卷积神经网络相结合进行图片生成，DCGAN模型的结构如下： 

![图片名称](https://img-blog.csdn.net/20180105161213241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


DCGANs的基本架构就是使用几层“反卷积”（Deconvolution）。传统的CNN是将图像的尺寸压缩，变得越来越小，而反卷积是将初始输入的小数据（噪声）变得越来越大（但反卷积并不是CNN的逆向操作），例如在上面这张图中，从输入层的100维noise，到最后输出层64x64x3的图片，从小维度产生出大的维度。反卷积的示意图如下所示，2x2的输入图片，经过3x3 的卷积核，可产生4x4的feature map： 

![图片名称](https://img-blog.csdn.net/20180105171408184?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


由于反卷积存在于卷积的反向传播中。其中反向传播的卷积核矩阵是前向传播的转置，所以其又可称为transport convolution。只不过我们把反向传播的操作拿到了前向传播来做，就产生了所谓的反卷积一说。但是transport convolution只能还原信号的大小，不能还原其值，因此不是真正的逆操作。

DCGAN的另一个改进是对生成模型中池化层的处理，传统CNN使用池化层（max-pooling或mean-pooling）来压缩数据的尺寸。在反卷积过程中，数据的尺寸会变得越来越大，而max-pooling的过程并不可逆，所以DCGAN的论文里并没有采用池化的逆向操作，而只是让反卷积的滑动步长设定为2或更大值，从而让尺寸按我们的需求增大。另外，DCGAN模型在$G$和$D$上均使用了batch normalization，这使得训练过程更加稳定和可控。

文献[3]将GANs应用于文本转图像（Text to Image），从而可根据特定输入文本所描述的内容来产生特定图像。因此，生成模型里除了输入随机噪声之外，还有一些特定的自然语言信息。所以判别模型不仅要区分样本是否是真实的，还要判定其是否与输入的语句信息相符。网络结构如下图所示：


![图片名称](https://img-blog.csdn.net/20180105170111576?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

以下是实验的结果，同时可看到模型仍然存在Mode Collapse的问题： 
![图片名称](https://img-blog.csdn.net/20180105171006864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# **4. 文献**
- Goodfellow I J, Pouget-Abadie J, Mirza M, et al. Generative adversarial nets[C]// International Conference on Neural Information Processing Systems. MIT Press, 2014:2672-2680.
- Mirza M, Osindero S. Conditional generative adversarial nets[J]. arXiv preprint arXiv:1411.1784, 2014.
- Radford A, Metz L, Chintala S. Unsupervised representation learning with deep convolutional generative adversarial networks[J]. arXiv preprint arXiv:1511.06434, 2015.
- Reed S, Akata Z, Yan X, et al. Generative adversarial text to image synthesis[J]. arXiv preprint arXiv:1605.05396, 2016.




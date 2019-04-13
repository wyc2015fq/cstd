
# 自编码器（auto-encoder）介绍 - 郭云飞的专栏 - CSDN博客


2017年10月17日 11:55:23[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：909


**一、自编码器简介**
自编码器可以理解为一个试图还原原始输入的系统，如下图：
![](https://img-blog.csdn.net/20171017115715158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上图中，虚线蓝色框内的部分就是一个自编码器模型，它由编码器（Encoder）和解码器（Decoder）组成。本质上是对输入信号做某种变换。编码器将输入信号x变换成**编码信号**y，而解码器将编码信号y转换成输出信号![](https://img-blog.csdn.net/20171017120216831?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)：
![](https://img-blog.csdn.net/20171017120620614?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
自编码器的目的是让输出仅可能的复现输入。有人会问，如果f和g是恒等映射，那输入不就永远等于输出了吗？的确如此，但这样有卵用！这里强调一点：对于自编码器，我们不关心输出是啥，而是中间层的编码，或者说对输入数据的映射。就是说将输入数据转换到另一种形式，这就是**特征提取**呀！
**二、自编码器与神经网络**
![](https://img-blog.csdn.net/20171017125847390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
神经网络把输入层数据x∈Rn转换到中间层h∈Rp，再转换到输出层y∈Rm。两层之间的变换都是**线性变换 + 非线性激活**。公式为：
![](https://img-blog.csdn.net/20171017131706870?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
神经网络主要用于分类。定义一个目标函数来衡量当前的输出和真实结果的差异（损失函数），利用梯度下降逐步调整参数，使得整个网络尽可能的拟合训练数据。如果有正则约束的话，还能要求模型尽量简单（防止过拟合）。
对于自编码器，由于输入和输出相等。我们利用数据训练网络后，这个网络就学习出了x-->h-->y的能力。此处的h是至关重要的，因为它是在尽量不损失信息量的情况下，对原始数据的另一种表达。
为了尽量学到有意义的表达，我们会给隐层加入一定的约束。从数据维度来看，常见以下两种情况：

n > p，即隐层维度小于输入数据维度。也就是说从x→h的变换是一种降维的操作，网络试图以更小的维度去描述原始数据而尽量不损失数据信息。实际上，当每两层之间的变换均为线性，且监督训练的误差是二次型误差时，该网络等价于PCA！
n < p，即隐层维度大于输入数据维度。这又有什么用呢？其实不好说，但比如我们同时约束h的表达尽量稀疏（有大量维度为0，未被激活），此时的编码器便是大名鼎鼎的“稀疏自编码器”。
**三、堆叠自编码器（Stacked Auto Encoder，SAE）**
![](https://img-blog.csdn.net/20171017134411357?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**四、自编码器的变种**
![](https://img-blog.csdn.net/20171017134613876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)














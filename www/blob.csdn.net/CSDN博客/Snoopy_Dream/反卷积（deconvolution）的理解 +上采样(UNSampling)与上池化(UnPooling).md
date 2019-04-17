# 反卷积（deconvolution）的理解 +上采样(UNSampling)与上池化(UnPooling) - Snoopy_Dream - CSDN博客





2018年11月30日 20:11:20[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：357








像往常一样，举个梯子：

[https://blog.csdn.net/A_a_ron/article/details/79181108](https://blog.csdn.net/A_a_ron/article/details/79181108) 上采样( (UNSampling)与上池化(UnPooling)

[https://www.zhihu.com/question/43609045/answer/132235276](https://www.zhihu.com/question/43609045/answer/132235276)  反卷积 deconvolution networks



在网络解码器结构中**有的时候使用反卷积、而有的时候使用unpooling或或者unsampling，**三者还是有不同的。这里记录一下。

**目录**

[上采样与上池化](#%E4%B8%8A%E9%87%87%E6%A0%B7%E4%B8%8E%E4%B8%8A%E6%B1%A0%E5%8C%96)

[反卷积](#%E5%8F%8D%E5%8D%B7%E7%A7%AF)

[基础](#%E5%9F%BA%E7%A1%80)

[用法](#%E7%94%A8%E6%B3%95)

## 上采样与上池化



图示理解

使用三张图进行说明： 

![](https://img-blog.csdn.net/20180127154813206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQV9hX3Jvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图（a）表示UnPooling的过程，特点是在Maxpooling的时候保留最大值的位置信息，之后在unPooling阶段使用该信息扩充Feature Map，除最大值位置以外，其余补0。与之相对的是图（b），两者的区别在于UnSampling阶段没有使用MaxPooling时的位置信息，而是直接将内容复制来扩充Feature Map。从图中即可看到两者结果的不同。图（c）为反卷积的过程，反卷积是卷积的逆过程，又称作转置卷积。最大的区别在于反卷积过程是有参数要进行学习的（类似卷积过程），理论是反卷积可以实现UnPooling和unSampling，只要卷积核的参数设置的合理。

有关反卷积的详细信息，可以参考这篇博客。

**反卷积与UnPooling的可视化**

对网络层进行可视化的结果： 

![](https://img-blog.csdn.net/20180127155824063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQV9hX3Jvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图（a）是输入层；图（b）是14*14反卷积的结果；图（c）是28*28的UnPooling结果；图（d）是28*28的反卷积结果；图（e）是56*56的Unpooling结果；图（f）是56*56反卷积的结果；图（g）是112*112 UnPooling的结果；图（h）是112*112的反卷积的结果；图（i）和图（j）分别是224*224的UnPooling和反卷积的结果。两者各有特点。

## 反卷积

### 基础

一句话解释：**逆卷积**相对于卷积在神经网络结构的**正向和反向传播中做相反的运算。**

逆卷积(Deconvolution)比较容易引起误会，转置卷积(Transposed Convolution)是一个更为合适的叫法.

举个栗子：

4x4的输入，卷积Kernel为3x3, 没有Padding / Stride, 则输出为2x2。

![](https://pic2.zhimg.com/80/3a18904d07e27a12c6bebdfba6d263f1_hd.png)

普通的conv层：

输入矩阵可展开为16维向量，记作![x](https://www.zhihu.com/equation?tex=x)

输出矩阵可展开为4维向量，记作![y](https://www.zhihu.com/equation?tex=y)

卷积运算可表示为![y = Cx](https://www.zhihu.com/equation?tex=y+%3D+Cx)



不难想象![C](https://www.zhihu.com/equation?tex=C)其实就是如下的稀疏阵:我们再把4×4的输入特征展成[16,1]的矩阵 XX，那么 Y=CXY=CX 则是一个[4,1]的输出特征矩阵，把它重新排列2×2的输出特征就得到最终的结果，

![](https://pic2.zhimg.com/80/fcf85c4e66326ad5279563b480a80ae1_hd.png)



平时神经网络中的正向传播就是转换成了如上矩阵运算。

那么当反向传播时又会如何呢？首先我们已经有从更深层的网络中得到的![\frac{\partial Loss}{\partial y}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+Loss%7D%7B%5Cpartial+y%7D).

![\frac{\partial Loss}{\partial x_j} = \sum_i \frac{\partial Loss}{\partial y_i} \frac{\partial y_i}{\partial x_j} = \sum_i \frac{\partial Loss}{\partial y_i} C_{i,j}  = \frac{\partial Loss}{\partial y} \cdot C_{*,j} = C_{*,j}^T \frac{\partial Loss}{\partial y}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+Loss%7D%7B%5Cpartial+x_j%7D+%3D%0A%5Csum_i+%5Cfrac%7B%5Cpartial+Loss%7D%7B%5Cpartial+y_i%7D+%5Cfrac%7B%5Cpartial+y_i%7D%7B%5Cpartial+x_j%7D+%3D+%5Csum_i+%5Cfrac%7B%5Cpartial+Loss%7D%7B%5Cpartial+y_i%7D+C_%7Bi%2Cj%7D++%3D+%5Cfrac%7B%5Cpartial+Loss%7D%7B%5Cpartial+y%7D+%5Ccdot+C_%7B%2A%2Cj%7D+%3D+C_%7B%2A%2Cj%7D%5ET+%5Cfrac%7B%5Cpartial+Loss%7D%7B%5Cpartial+y%7D)

回想第一句话，你猜的没错，所谓逆卷积其实就是正向时左乘![C^T](https://www.zhihu.com/equation?tex=C%5ET)，而反向时左乘![(C^T)^T](https://www.zhihu.com/equation?tex=%28C%5ET%29%5ET)，即![C](https://www.zhihu.com/equation?tex=C)的运算。

逆卷积的一个很有趣的应用是GAN(Generative Adversarial Network)里用来生成图片：[Generative Models](https://link.zhihu.com/?target=https%3A//openai.com/blog/generative-models/)

![](https://pic4.zhimg.com/80/6ec2bc4c838aff682072ffd13db3d8d7_hd.png)----
[[1603.07285] A guide to convolution arithmetic for deep learning](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1603.07285)
[GitHub - vdumoulin/conv_arithmetic: A technical report on convolution arithmetic in the context of deep learning](https://link.zhihu.com/?target=https%3A//github.com/vdumoulin/conv_arithmetic)

### 用法

到**visualization/pixel-wise prediction/unsupervised learning/image generation**都会用到deconv的结构。比如**Deconvolutional ****Network**[1][2]做图片的unsupervised feature learning，**ZF-Net**论文中的卷积网络可视化[3]，**FCN**网络中的upsampling[4]，**GAN**中的Generative图片生成[5]。



Deconvolution大致可以分为以下几个方面：

（1）unsupervised learning，其实就是covolutional sparse coding[1][2]：这里的deconv只是观念上和传统的conv反向，传统的conv是从图片生成feature map，而deconv是用unsupervised的方法找到一组kernel和feature map，让它们重建图片。

（2）CNN可视化[3]：通过deconv将CNN中conv得到的feature map还原到像素空间，以观察特定的feature map对哪些pattern的图片敏感，这里的deconv其实不是conv的可逆运算，只是conv的transpose，所以tensorflow里一般取名叫transpose_conv。

（3）**upsampling[4][5]：**在pixel-wise prediction比如image segmentation[4]以及image generation[5]中，由于**需要做原始图片尺寸空间的预测，**而卷积由于stride往往会降低图片size， 所以**往往需要通过upsampling的方法来还原到原始图片尺寸**，deconv就充当了一个upsampling的角色

1. 不解释了

2. CNN可视化

deconv第二个方面是用来做CNN的可视化。ZF-Net[3]中用到了deconv来做可视化，它是将CNN学习到的feature map用得到这些feature map的卷积核，取转置，将图片特征从feature map空间转化到pixel空间，以发现是哪些pixel激活了特定的feature map，达到分析理解CNN的目的。
**3. upsampling**

分别简单介绍两篇文章，**FCN和DCAN**。FCN[4]主要用来做pixel-wise的image segmentation预测，先用传统的CNN结构得到feature map，同时将传统的full connected转换成了对应参数的卷积层，比如传统pool5层的尺寸是7×7×512，fc6的尺寸是4096，传统的**full connected** weight是7×7×512×4096这样多的参数，**将它转成卷积核**，**kernel size为7×7，input channel为512，output channel为4096**，则将传统的分别带有卷积和全连接的网络转成了全卷积网络(fully convolutional network, FCN)。FCN的一个好处是输入图片尺寸大小可以任意，不受传统网络全连接层尺寸限制，传统的方法还要用类似SPP结构来避免这个问题。**FCN中为了得到pixel-wise的prediction，也要把feature map通过deconv转化到像素空间**。论文中还有一些具体的feature融合，详情可参见论文。


DCGAN[5]中使用deconv就更自然了，本身GAN就需要generative model，需要通过deconv从特定分布的输入数据中生成图片。GAN这种模式被Yann LeCun特别看好，认为是unsupervised learning的一个未来。




[1] Zeiler M D, Krishnan D, Taylor G W, et

al. Deconvolutional networks[C]. Computer Vision and Pattern Recognition, 2010.

[2] Zeiler M D, Taylor G W, Fergus R, et

al. Adaptive deconvolutional networks for mid and high level feature

learning[C]. International Conference on Computer Vision, 2011.

[3] Zeiler M D, Fergus R. Visualizing and

Understanding Convolutional Networks[C]. European Conference on Computer

Vision, 2013.

[4] Long J, Shelhamer E, Darrell T, et al.

Fully convolutional networks for semantic segmentation[C]. Computer Vision and

Pattern Recognition, 2015.

[5] Unsupervised Representation Learning

with Deep Convolutional Generative Adversarial Networks

[6] [Sparse Coding - Ufldl](https://link.zhihu.com/?target=http%3A//ufldl.stanford.edu/wiki/index.php/Sparse_Coding)

[7] [Denoising Autoencoders (dA)](https://link.zhihu.com/?target=http%3A//www.deeplearning.net/tutorial/dA.html)

[8] [Convolution arithmetic tutorial](https://link.zhihu.com/?target=http%3A//deeplearning.net/software/theano_versions/dev/tutorial/conv_arithmetic.html%23transposed-convolution-arithmetic)




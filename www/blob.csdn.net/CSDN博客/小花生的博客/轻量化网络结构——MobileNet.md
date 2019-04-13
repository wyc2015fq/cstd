
# 轻量化网络结构——MobileNet - 小花生的博客 - CSDN博客


2018年09月12日 23:39:16[Peanut_范](https://me.csdn.net/u013841196)阅读数：476



###### 《MobileNets: Efficient Convolutional Neural Networks for Mobile Vision Applications》
Google
**Google在2017年提出的适用于手机端的神经网络模型**
**1.主要采用了深度可分离卷积Depthwise separable convolution的卷积方式**
达到：**1）减少参数数量 2）提升运算速度**
Depthwise separable convolution不是MobileNet提出来的，也是借鉴。
Depthwise separable convolution是一个卷积核负责一部分feature map，每个feature map只被一个卷积核卷积。
MobileNet的精华在于卷积方式–Depthwise separable convolution；采用深度可分离卷积会涉及到两个超参数，来减少参数量和计算量。
**1）宽度乘数（width multiplier）**：[减少输入和输出的channels]
**2）分辨率乘数（resolution multiplier）**：[减少输入和输出的feature map大小]
**2.深度可分离卷积（Depthwise separable convolution）**
将一个标准卷积分成两步：
**1）Depthwise convolution（深度卷积）**，即逐通道的卷积，一个卷积核负责一个通道，一个通道只被一个卷积核滤波。
**2）Pointwise convolution（逐点卷积）**，将Depthwise convolution得到的feature map再串起来，通过一个1x1的卷积来线性结合起来，组成新的feature map。
![这里写图片描述](https://img-blog.csdn.net/20180912230731406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180912230731406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**注：**输出的每一个feature map要包含输入层所有feature map的信息。然而仅采用depthwise convolution 是没有办法做到这一点的，因此需要pointwise convolution的辅助。
[
](https://img-blog.csdn.net/20180912230731406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**2.1 标准卷积**
标准的卷积层是将维度为$D_{F}*D_{F}*M$的输入层转化为维度是$D_{G}*D_{G}*M$
其中：
$D_{F}$是输入feature map的长和宽，M是输入的通道数（channels）
$D_{G}$是输出feature map的长和宽，N是输出的通道数
假设卷积核filter的大小是$D_{K}*D_{K}$，则标准卷积的计算量为：
$D_{K}*D_{K}*M*N*D_{F}*D_{F}$
标准卷积，即不管当前pixel有多少channels，卷积之后就是一个channel。
![](https://img-blog.csdn.net/20180912231332264?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**2.2 深度可分离卷积**
分为两个步骤：
1）第一步**深度卷积**：在每一个channel上进行卷积操作，卷积核的大小是$D_{K}*D_{K}*1*M$,
所以总的计算量是：$D_{K}*D_{K}*M*D_{F}*D_{F}$
![](https://img-blog.csdn.net/20180912231451281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180912231451281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2）第二步**逐点卷积**，将其结合起来，卷积核的大小是$1*1*M*N$, 所以总的计算量是：$M*N*D_{F}*D_{F}$
![](https://img-blog.csdn.net/20180912231629938?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180912231629938?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所以和标准卷积相比计算比率为：
![](https://img-blog.csdn.net/20180912231701908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180912231701908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
MobileNet使用的是3x3的卷积核，所以计算量可以减少**8-9倍。**
[
](https://img-blog.csdn.net/20180912231701908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**3.神经网络结构**
MobileNet共28层，可以发现这里的下采样方式没有采用池化层，而是利用depthwise convolution的时候将步长设置为2，达到下采样的目的。
![这里写图片描述](https://img-blog.csdn.net/20180912234101904?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**总的结构图：**
![这里写图片描述](https://img-blog.csdn.net/20180912231947945?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**4.宽度乘数（Width Multiplier）**
引入超参数α，目的是使模型变瘦
即输入层的channels个数M，变成αM，输出层的channels个数N变成了αN。
所以引入宽度乘数后的计算量是：
![这里写图片描述](https://img-blog.csdn.net/20180912232150757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180912232150757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
一般α取（0，1]，常取的值是1，0.75，0.5，0.25
大约可以减少参数量和计算量的。
**5.分辨率乘数（Resolution Multiplier）**
引入超参数ρ，目的是降低图片的分辨率
即作用在输入的feature map上
所以再引入分辨率乘数后总的计算量是：
![这里写图片描述](https://img-blog.csdn.net/20180912232227412?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180912232227412?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
一般输入图片的分辨率是224，192，160，128
大约可以减少计算量ρ的平方。
**6.实验结果**
![这里写图片描述](https://img-blog.csdn.net/20180912232341856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180912232341856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
1.0 MobileNet与GoogLeNet及VGG16做比较，可以发现，相较于GoogLeNet，虽然参数差不多，都是一个量级，但在运算量上却小于GoogLeNet一个量级，这就得益于depthwise convolution。
[
](https://img-blog.csdn.net/20180912232341856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180912232455846?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180912232455846?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
SqueezeNet虽然可以减少内存，但计算量严重大于MobileNet。
[
](https://img-blog.csdn.net/20180912232455846?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**MobileNet小结：**
核心思想是采用Depthwise separable convolution操作，在相同的权值参数数量的情况下，相较于standard convolution操作，可以减少数倍的计算量，从而达到提升运算速度的目的。
参考：[https://blog.csdn.net/u011995719/article/details/79100582](https://blog.csdn.net/u011995719/article/details/79100582)
---

###### 注：博众家之所长，集群英之荟萃。

[
](https://img-blog.csdn.net/20180912232455846?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

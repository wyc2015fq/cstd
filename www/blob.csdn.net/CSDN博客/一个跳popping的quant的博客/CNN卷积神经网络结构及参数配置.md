# CNN卷积神经网络结构及参数配置 - 一个跳popping的quant的博客 - CSDN博客





2018年07月14日 10:00:27[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1731








来源：机器学习算法与自然语言处理

作者：白雪峰

转载于：https://blog.csdn.net/np4rHI455vg29y2/article/details/78958121




# 1、CNN例子



Yann LeCun提出的最基本和开始的CNN结构图



![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYO6748PvibAe5VMadJdXE4bokhnVm5CJAnpiauj5A6qpLIZAS2lFwzZibw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)

![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYMKibVERr1cIZLaMfuxlfDpM7ZnfbibstQob4ibfL7DUbicBGmevOTJs5bA/640?wx_fmt=jpeg)

# 2、什么是CNN

### 2.1、什么是卷积

连续和离散卷积的定义：


![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYAaibogFjmxt6ba8nZfSdsffFESsJklT6iaxnQplp82bPG1KZQKD73jCA/640?wx_fmt=jpeg)


特点：


![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYXPcp8U3ibibdoxFS9sEkgwhPMriaibkPITA7aTGmibwGyPPJOYgZwiaGGqAQ/640?wx_fmt=jpeg)

### 2.2、离散卷积的例子

丢骰子时加起来要等于4的概率是多少？



![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYEbjJrUGd3ibnhQPiamMMibePa5e1JfPGJGRl4W0tG2WLicib3XJXpQS4wqA/640?wx_fmt=jpeg)


二维离散的卷积

![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYjJCC6zicvAw08E8ibhu3LeurdcRBGlPzzEoXWKYPNHwN0efE22ZsRWcA/640?wx_fmt=jpeg)


计算的动图如下

![0?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYBE6kwEOiadmorXjMJrhLWPGesKRLLic9dR7j6rjhcPUgQD3RoNPtJk6A/0?wx_fmt=gif)

### 2.3、用到二维图像上

关于卷积中常用到的一些概念：神经网络的卷积是对应位相乘，现在是信号相乘。

在图像处理中，卷积通常被称作为filtering，现在也有很多著名的filtering/convolution kernels都是在图像中抓取直观特征的。


下面移动的小矩阵有两种叫法：一种叫做滤波器filter，一种叫做卷积核kernel，是相同东西的两种不同叫法。




![0?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYqvsoOgfKMlRQaShOkLtyF6iaZzicXQz88dtb3BiaNCjmNzVFcHoNGiaYoA/0?wx_fmt=gif)

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaY1DHebiaibyLOfJyolo6V5ziaWMy3ao6zDGibtIicu53PJ4ZCRzUlk7xF1JA/640?wx_fmt=jpeg)

### 2.4、用到神经网络中

我们其实需要学习的就是里面的线上面对应的权值，比如下面绿色的代表3*1的卷积核大小，只是这里用神经网络的结构表示出来了。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaY6Afe6sNAYMNytbyV49Dse5hIvA9CNtHvBRvJCmjeJvFHOJHicGPHXBA/640?wx_fmt=jpeg)

### 2.5、卷积的细节
- 
#### filter/kernel size, number


假设神经网络的输入是6*6的图像。



![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYGQKkZmu0nd3sUzbEbPSdWXBlVMgLYC9aarERH0pF9x5Jr6fTvlpRPw/640?wx_fmt=jpeg)
其中每个卷积核代表提取不同的特征，多个卷积核提取的特征然后进行组合(这样更强大)，一同送入到后续的结构。

下面来一个更通俗的解释：



![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYRfVpbvQKzEyKD0C2KMbKOibt4qNBLTNMj63K74lTelXux41bKnR9h8A/640?wx_fmt=jpeg)

每个人代表一个卷积核来进行提取不同的特征，一个人是弱小的，但是一组人就强大了，他们有着不同的知识（权重），这样类比来理解，就会好理解很多。
- 
#### stride


使用filter扫描图片的步的大小。



![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYvbP6LVQtHh3ibBlafCH0vXRwqQlb25Kwp8ewnOqecPykh23AFvTSwXA/640?wx_fmt=jpeg)
- 
#### zero-padding


一种不忽略边界模式的方法。

新图像要比原图像更小。



![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaY1uAb9VKN7IS0w42ia6gA08W0QFORd24h6iboX1k2MnUDjWSLWldabNUw/640?wx_fmt=jpeg)
- 
#### channel




![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYiaW2ZDH9W4qM5mLibzzJuRgAoicX3Wr8RmSiaia0gXAcSyShhl7tcq1RHeA/640?wx_fmt=jpeg)

### 2.6、池化（pooling）

池化层从它们的输入中子取样。

1）、Spatial pooling（也称作subsampling或者downsampling）减少每个特征映射的维度。

2）、保持最重要的信息。

max pooling例子(average pooling etc)



![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYWGo8Aib4lZHzkpibwt6qBNHMdJcFFtKemLCKGFVhVnQtQSYco2Zl4zeA/640?wx_fmt=jpeg)
- Pooling具有局部不变性。
- 图像往左或者往右移动，pooling的结果是不变的。



![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYscChibuvHMDvA7qBFIZjftkkkhobxicZmNEibrXzM3h90IoDYoebhAicibQ/640?wx_fmt=jpeg)

### 2.7、flatten



![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYicKrg1NSCEWtXic6cvFeyjWKmgpaje9bd0szsz53ZR6wphQRHRh7Uvuw/640?wx_fmt=jpeg)

### 2.8、Convolution VS Fully Connected



![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYzshffoEMwegsXmib0P0ICU0t8ibehgzvUeMfOl1jW3j9uKyS0anMicgow/640?wx_fmt=jpeg)

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYyU1KOdpXujucAKQ8oZvS9jTP9cCtiaTyRYj2E6ia1mQbjY7u0eEfib9EQ/640?wx_fmt=jpeg)

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYQIQasywUQRvRY0VzA7WJrkIApW4eVnSNRqAWia66qsJLB2ZxTPta2Rg/640?wx_fmt=jpeg)

### 2.9、CNN的整体



![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYMj2S6y9B4rh1kTzynNUCbSYXSdib66oA56CmB2lSIM5U53wxfCUnCSA/640?wx_fmt=jpeg)

所以从整体来看，CNN的组成：
- Convolution(卷积层)
- Nolinearity:e.g ReLu(非线性转化)
- Pooling(池化)
- FC Layers(全连接层)

![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYsRtcGVSjYBmRdqAicNpugelLeibQ784Sm61TIVzibqhC5JtMbOoGtmJvA/640?wx_fmt=jpeg)

# 3、为什么CNN有效

### 3.1、一些模式是比整幅图像更小的图像    



![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYicUiaqbF0ZuvrIsaC52PGb7L6raiacrkA3ovayic9WpTkDrj9icSe1pV3Eg/640?wx_fmt=jpeg)

### 3.2、一些相同的模式出现在不同的区域中



![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYlibtWQcTjBr0t7IaKwiasoNyrvMQibBJS2INwn8iabyJ4aibNVibbqyDZjHQ/640?wx_fmt=jpeg)

### 3.3、子采样像素不会改变对象



![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYGtn1UmxhibjeswZMoZ4RpIqdWX9RjTGmKrhVbrAyhCuk6QA31btszJw/640?wx_fmt=jpeg)

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYf16q7IF314Me0xWqreooNzG3b85n06HISib4ZXnGsHf6tcy4gdz0Jvg/640?wx_fmt=jpeg)

# 4、对CNN的一些其他理解

### 4.1、关于接受域（receptive field）



称在底层中影响上层输出单元s的单元合集为s的接受域（receptive field）。






![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYKWHU1NxQ4xummNhyDaK6Alx6FFxuWdxmp2ibW6UXrtOibqlV3V5ic2icNQ/640?wx_fmt=jpeg)

处于卷积网络更深的层中的单元，它们的接受域要比处在浅层的单元的接受的域更大。如果网络还包含类似步幅卷积或者池化之类的结构特征，这种效应会加强。这意味着在卷积网络中尽管直接连接都是很稀疏的，但处在更深的层中的单元可以间接地链接到全部或者大部分输入图像。（表现性能）




![640?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/heS6wRSHVMn7nLwdz2xxAfTru2GG9QaYpttvX4mNRXzic1TYSDcXPUTGicXibKG9jLXKh3obGQic6nbRKUqFBTYYMA/640?wx_fmt=jpeg)

### 4.2、卷积和池化作为一种无限强的先验

首先，弱先验具有较高的熵值，因此自由性较强，强先验具有较低的熵值，这样的先验在决定参数最终取值时可以起着非常积极的作用。

把卷积网络类比成全连接网络，但对于网络的权重具有无限强的先验。
- 
所有隐藏单元的权重是共享的。

- 
除了一些连续的小单元的权重外，其他的权重都是0.

- 
池化也是一个无限强的先验：每个单元都具有对少量平移的不变性。


卷积和池化可能导致欠拟合！任何其他先验类似，卷积和池化只有当先验的夹着合理且正确时才有用。如果一项任务依赖于保存精确的空间信息，那么在所有的特征上使用池化将会增大训练误差。

根据实际需求选取先验。









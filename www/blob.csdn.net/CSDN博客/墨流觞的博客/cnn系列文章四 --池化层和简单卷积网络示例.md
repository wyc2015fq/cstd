# cnn系列文章四  --池化层和简单卷积网络示例 - 墨流觞的博客 - CSDN博客





2018年09月12日 12:00:19[墨氲](https://me.csdn.net/dss_dssssd)阅读数：357








[cnn系列文章三  –padding和strides详解](https://blog.csdn.net/dss_dssssd/article/details/82659316)

出去吃了个饭，突然感觉好没胃口，好落寞。。。。 

哎，继续吧 

一句歌词继续中:
> 
《霜雪千年》 

  苔绿青石板街 

  斑驳了流水般岁月 

  小酌三盏两杯 

  理不清缠绕的情结
## 典型的卷积神经网络
- convolution (Conv) 卷积层
- pooling   (pool)  池化层
- fully connected (FC) 全连接层

## 池化层

### 1. max pooling(最大池化)

简单的说就是使用过滤器对图像进行区域分割，然后输出每个区域的最大值。

比如下图使用$2\times 2$的过滤器，选择步长为2，则然后输出$\frac{4 -2}{2} + 1 = 2$即$2\times2$的图像，每个像素点的值为原图像中过滤器覆盖区域的最大值 
![](./17.png)![这里写图片描述](https://img-blog.csdn.net/20180912115453294?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**最大池化的直观理解：**

还记得垂直边缘检测的例子吗，在边缘处，输出图像的值很大。也就是说，当输出图像的值很大时，意味着提取了某些特征。

而最大池化的操作就是，只要在任何一个象限中提取到某个特征，就都会保留到最大池化的输出结果中。如果提取到某个特征，则保留其最大值，比如上图中左上角的9，而如果为提取到特征，则最大池化后的输出仍很小，比如右上角的2.

在实际工作中，加入最大池化后，通常网络表现的会很好。

注意当有多个通道时：是对每一通道分别进行最大池化操作，不会改变图像的通道数目。 
![](./21.png)![这里写图片描述](https://img-blog.csdn.net/2018091211552151?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 2.average pooling (平均池化)

顾名思义，就是在每个区域上求平均值，作为平均池化的输出值 
![](./22.png)![这里写图片描述](https://img-blog.csdn.net/2018091211560661?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
平均池化用的很少

### 总结
- 
池化操作不需要训练参数，只有两个超参数：
- $f: filter size$
- $s: stride$

常见的超参数有：

$f = 2, s=2$将图像的高度和宽度减半

$f=3, s=2$

- 
很少用padding,即$p=0$

- 
input: $n_H \times n_W \times n_C$

output: $\lfloor \frac{n_H - f}{s} + 1 \rfloor \times \lfloor \frac{n_W - f}{s} + 1 \rfloor \times n_C$


### 卷积神经网络示例

参数设计收LeNet-5启发。我的建议还是在纸上推导一下呢。

![](./23.png)![这里写图片描述](https://img-blog.csdn.net/20180912115549350?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
接下来看一下网络结构和参数：

![](./24.png)![这里写图片描述](https://img-blog.csdn.net/20180912115622485?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
来计算几个参数： 

我发现208这个参数数量有点没太搞懂

不是$5 \times 5 \times 3 \times 8 + 8 = 608$吗？
- 参数大多集中在全连接层
- 随着网络层深度增加，激活数量在减小，但是数量不能下降的太快，否则会影响网络的表现性能。

## 卷积神经网络的优势

![](./27.png)![这里写图片描述](https://img-blog.csdn.net/20180912115643813?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
若是全连接层需要的参数大概是14 million。
- 参数共享 
![](./25.png)![这里写图片描述](https://img-blog.csdn.net/20180912115655854?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

某一个特征检测器可以在所有图像上使用，大量减少参数数量- 稀疏连接 
![](./26.png)![这里写图片描述](https://img-blog.csdn.net/20180912115710639?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

每一个输出像素值紧依赖于输入很小的输入值，而非全连接层的所有输入值。更容易发现局部特征
通过以上两种方式，cnn结构大量减少参数数量。

[接下来讲解tensorflow卷积和池化两个函数。](https://blog.csdn.net/dss_dssssd/article/details/82659501)















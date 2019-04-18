# TensorFlow实战——CNN - Soul Joy Hub - CSDN博客

2017年06月03日 22:59:10[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：774


全连接神经网络存在以下三个问题：
- 参数太多（每个节点都要和下一层的所有节点连接）
- 容易过拟合
- 不能很好的抽取局部的特征（如一张有两只猫的图片，它偏向于抽取整张图的特征，而不是图中部分区域的特征）

鉴于以上的问题，我们介绍卷积神经网络（CNN）。先介绍CNN中的池化层和卷积层。我们根据下图来讲解：

![这里写图片描述](https://img-blog.csdn.net/20170603180724998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 过滤器

池化层和卷积层都会有过滤器，过滤器会有个**窗口**（如上图中，黄色的框框），它会把上一层的数据过滤到一下层（如上图中，将左边的矩阵过滤生成右边的矩阵。）此外，过滤器能设置一下层的**深度**。（后续会讲解）

为了让image长宽不变，我们可以在边缘添加值为0的一些行与列，从而使得过滤后的行与列不变。

若不填充，那么

$\large \color{blue}{ 下层的长度 =  CEILING((上层长度-窗口长方向步长+1)/2)}$
$\large  \color{blue}{ 下层的宽度 =  CEILING((上层宽度-窗口宽方向步长+1)/2)}$

# 卷积层

上次其实就是卷积层中某一个深度上的过滤。我们设想数据是一张`5*5*3`的RGB图片，该卷积层的过滤器，窗口为3*3，深度为10。设$W^i_j$为将上一层中深度$i$的image过滤到一下层中深度$j$上的image的参数矩阵，假设（如上图黄色框框的红色数字）：

$\large  \color{blue}{ W^0_0 =  \begin{array}{ccc} 1 & 0 & 1\\ 0& 1 & 0\\ 1 & 0 & 1 \end{array}}$

$\large  \color{blue}{ 右边矩阵得到的第一个值 = 1*1 + 1*0 + 1*1 + 0*0 + 1*1 + 1*0 +0*1 + 0*0 + 1*1 = 4}$

后面的类似计算。我们得到右边的矩阵$R^0_0$。通用的，我们可以利用$W^1_0$和$W^2_0$，计算出$R^1_0$和$R^2_0$。

而下一层深度0上的Image的就是：

$\large  \color{blue}{ Image_0 = R^0_0 + R^1_0 + R^2_0 + B_0}$

上式中$B_0$是下一层深度0上偏置矩阵，要注意的是该矩阵的值都是相同的。所以，**偏置参数的个数为下一层的深度数**。

# 池化层

理解的卷积层，池化层就很容易理解了。池化层有两点不同。
- 没有W参数矩阵。也就是没有黄色框框的红色数字部分。那它怎么过滤呢？很简单，选框框中最大的数字，或者求个平均值等方式，就求出右边矩阵中的值了。
- 池化层不改变前后层的深度。也就是说，上一层第 $i$ 深度的image直接过滤出下一层第 $i$ 深度的image。

# 卷积神经网络

卷积神经网络的架构为：

$\large  \color{blue}{ 输入层 -> (卷积层Conv+ -> 池化层Pool？)+ -> 全连接层FC+}$

`+`表示`1~n`，`?`表示`0~n`

## VGGNet模型

![这里写图片描述](https://img-blog.csdn.net/20170603224740285?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## LeNet模型

![这里写图片描述](https://img-blog.csdn.net/20170603225145055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Inception-v3 模型

![这里写图片描述](https://img-blog.csdn.net/20170604211435923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170603225406428?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


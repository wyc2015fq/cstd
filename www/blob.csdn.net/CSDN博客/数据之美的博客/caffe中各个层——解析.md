# caffe中各个层——解析 - 数据之美的博客 - CSDN博客
2017年03月19日 14:38:50[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：387
转自：http://www.cnblogs.com/denny402/p/5071126.html
所有的层都具有的参数，如name, type, bottom, top和transform_param请参看我的前一篇文章：[Caffe学习系列(2)：数据层及参数](http://www.cnblogs.com/denny402/p/5070928.html)
本文只讲解视觉层（Vision Layers)的参数，视觉层包括Convolution, Pooling, Local Response Normalization (LRN), im2col等层。
1、Convolution层：
就是卷积层，是卷积神经网络（CNN）的核心层。
层类型：Convolution
**　lr_mult: 学习率的系数，最终的学习率是这个数乘以solver.prototxt配置文件中的base_lr。如果有两个lr_mult, 则第一个表示权值的学习率，第二个表示偏置项的学习率。一般偏置项的学习率是权值学习率的两倍。**
在后面的convolution_param中，我们可以设定卷积层的特有参数。
必须设置的参数：
  　　num_output: 卷积核（filter)的个数
  　　kernel_size: 卷积核的大小。如果卷积核的长和宽不等，需要用kernel_h和kernel_w分别设定
其它参数：
  　　 stride: 卷积核的步长，默认为1。也可以用stride_h和stride_w来设置。
  　　 pad: 扩充边缘，默认为0，不扩充。 扩充的时候是左右、上下对称的，比如卷积核的大小为5*5，那么pad设置为2，则四个边缘都扩充2个像素，即宽度和高度都扩充了4个像素,这样卷积运算之后的特征图就不会变小。也可以通过pad_h和pad_w来分别设定。
    　　weight_filler: 权值初始化。 默认为“constant",值全为0，很多时候我们用"xavier"算法来进行初始化，也可以设置为”gaussian"
    　　bias_filler: 偏置项的初始化。一般设置为"constant",值全为0。
   　　 bias_term: 是否开启偏置项，默认为true, 开启
   　　 group: 分组，默认为1组。如果大于1，我们限制卷积的连接操作在一个子集内。如果我们根据图像的通道来分组，那么第i个输出分组只能与第i个输入分组进行连接。
输入：n*c0*w0*h0
输出：n*c1*w1*h1
其中，c1就是参数中的num_output，生成的特征图个数
 w1=(w0+2*pad-kernel_size)/stride+1;
 h1=(h0+2*pad-kernel_size)/stride+1;
如果设置stride为1，前后两次卷积部分存在重叠。如果设置pad=(kernel_size-1)/2,则运算后，宽度和高度不变。
示例：
```
layer {
  name: "conv1"
  type: "Convolution"
  bottom: "data"
  top: "conv1"
  param {
    lr_mult: 1
  }
  param {
    lr_mult: 2
  }
  convolution_param {
    num_output: 20
    kernel_size: 5
    stride: 1
    weight_filler {
      type: "xavier"
    }
    bias_filler {
      type: "constant"
    }
  }
}
```
  2、Pooling层
也叫池化层，为了减少运算量和数据维度而设置的一种层。
层类型：Pooling
必须设置的参数：
  　　 kernel_size: 池化的核大小。也可以用kernel_h和kernel_w分别设定。
其它参数：
  　pool: 池化方法，默认为MAX。目前可用的方法有MAX, AVE, 或STOCHASTIC
　　pad: 和卷积层的pad的一样，进行边缘扩充。默认为0
　　stride: 池化的步长，默认为1。一般我们设置为2，即不重叠。也可以用stride_h和stride_w来设置。
 示例：
```
layer {
  name: "pool1"
  type: "Pooling"
  bottom: "conv1"
  top: "pool1"
  pooling_param {
    pool: MAX
    kernel_size: 3
    stride: 2
  }
}
```
pooling层的运算方法基本是和卷积层是一样的。
输入：n*c*w0*h0
输出：n*c*w1*h1
和卷积层的区别就是其中的c保持不变
 w1=(w0+2*pad-kernel_size)/stride+1;
 h1=(h0+2*pad-kernel_size)/stride+1;
如果设置stride为2，前后两次卷积部分不重叠。100*100的特征图池化后，变成50*50.
3、Local Response Normalization (LRN)层
此层是对一个输入的局部区域进行归一化，达到“侧抑制”的效果。可去搜索AlexNet或GoogLenet，里面就用到了这个功能
 层类型：LRN
参数：全部为可选，没有必须
　　local_size: 默认为5。如果是跨通道LRN，则表示求和的通道数；如果是在通道内LRN，则表示求和的正方形区域长度。
　　alpha: 默认为1，归一化公式中的参数。
　　beta: 默认为5，归一化公式中的参数。
　　norm_region: 默认为ACROSS_CHANNELS。有两个选择，ACROSS_CHANNELS表示在相邻的通道间求和归一化。WITHIN_CHANNEL表示在一个通道内部特定的区域内进行求和归一化。与前面的local_size参数对应。
归一化公式：对于每一个输入, 去除以![](http://images2015.cnblogs.com/blog/140867/201512/140867-20151224102027640-2114104644.png)，得到归一化后的输出
示例：
```
layers {
  name: "norm1"
  type: LRN
  bottom: "pool1"
  top: "norm1"
  lrn_param {
    local_size: 5
    alpha: 0.0001
    beta: 0.75
  }
}
```
4、im2col层
如果对matlab比较熟悉的话，就应该知道im2col是什么意思。它先将一个大矩阵，重叠地划分为多个子矩阵，对每个子矩阵序列化成向量，最后得到另外一个矩阵。
看一看图就知道了：
![](http://images2015.cnblogs.com/blog/140867/201512/140867-20151224103800781-1819163043.png)
在caffe中，卷积运算就是先对数据进行im2col操作，再进行内积运算（inner product)。这样做，比原始的卷积操作速度更快。
看看两种卷积操作的异同：
![](http://images2015.cnblogs.com/blog/140867/201512/140867-20151224103937640-1245969956.jpg)

# 【用Python学习Caffe】0. 前言及介绍 - tostq的专栏 - CSDN博客





置顶2017年06月22日 21:50:15[tostq](https://me.csdn.net/tostq)阅读数：8461
所属专栏：[用Python学习Caffe](https://blog.csdn.net/column/details/16089.html)









# 0.前言及介绍

老实说现在的Caffe已经不够流行了（说到这里，我有点无力了，近年来深度学习发展实在是太快了，完全跟不上学习脚步了，刚刚Caffe有点了解后，马上就要跟不上时代了=_=||）。

如果对于现在的我来说，我更愿意推荐去学习Tensorflow或者是Pytorch，甚至是Caffe2。因为这些框架都有大公司参与开发，相关的学习资料更全，前段时间刚上手Tensorflow，个人感觉开发起来要比Caffe方便得多了。

我是从去年开始接触Caffe，刚开始也没有人带，再加个Caffe的学习资料比较少，所以一直以来都学得不够系统。现在终于要毕业，一方面为了给未来的学弟学妹留下点资料，避免他们以后少走些弯路，另一方面也给自己的Caffe学习过程作一个总结，因此写了这一系列博客，希望对大家有些许帮助，博客中的所有例子都有代码（github.com/tostq/）。

Caffe支持命令行、Python和Matlab三种开发方式。而为什么选择用Python开发，主要原因有以下几点：
- Python做Caffe的源代码和资源比较丰富
- Python有许多机器学习库包，可以同Caffe协同开发
- Python可以比较方便的整合Caffe
- 另外就是个人的原因，未来要干机器学习算法工程师，所以学点Python是必须的^_^

## 0.1 博客目录
- [0.前言部分：介绍Caffe平台的搭建，结构与开发流程](http://blog.csdn.net/tostq/article/details/73611437)
- [1.使用Caffe进行图像分类](http://blog.csdn.net/tostq/article/details/73611529)
- [2.使用Caffe完成图像目标检测](http://blog.csdn.net/tostq/article/details/73611590)
- [3.图像训练测试数据集LMDB的生成](http://blog.csdn.net/tostq/article/details/73611632)
- [4.设计自己的网络结构](http://blog.csdn.net/tostq/article/details/73611658)
- [5.生成训练器solver文件](http://blog.csdn.net/tostq/article/details/73611671)
- [6.权重预设、预训练及微调](http://blog.csdn.net/tostq/article/details/73611688)
- [7.网络修剪](http://blog.csdn.net/tostq/article/details/73611719)
- [8.网络权重共享量化](http://blog.csdn.net/tostq/article/details/73611746)

## 0.2 Caffe平台的搭建

Caffe最早是在Linux系统下运行的，而现在早就可以在Windows7平台下开发，微软已经给出了官方版本，各种安装及使用都非常成熟，另一方面因为个人不太习惯Linux系统，所以就选择了Caffe的Windows7版本，总体感觉安装更简单方便，没有Ubuntu下安装那么多奇怪的问题。

> 
Tips：关于是否使用GPU模式：GPU主要用于加速的，本博客的教程并不一定需要GPU，如果实在没有GPU的话，完全没有关系。当然如果有条件的，建议使用GPU模型，因为Windows下安装CUDN非常简单，而且进行GPU和CPU模型切换也只需要一两行代码就可以搞定了^_^。


Caffe的Windows版本的安装，因为网络上有许多非常好的教程，这里并不赘述了，但必须记得编译Pycaffe项目，将Pycaffe所生成caffe库包移到Python的第三方库包中（`site-packages`）。

还需要提一下的是，由于本系列教程会有介绍图像目标检测的例子，文中用到了SSD网络，因此必须安装windows-ssd版本的Caffe（百度上就可以搜到相关版本的安装），当然也可以直接安装我个人的Caffe版本，本系列博客的代码也在我的Caffe仓库中`python/learn`可以找到。如果已经安装了官方Caffe版本，又不想重新安装其他版本，也没关系，因此只有图像目标检测的例子用到了SSD，其他的例子都是可以在官方版本下运行的。

> 
Tips：实际上SSD只是在官方Caffe版本上增加了一些层结构，可以自行添加把新增层文件添加到项目中，重新编译就可以了^_^


Python的安装版本我选择的是Anaconda2 （Python 2.7）。另外Python开发还是需要一款IDE，个人推荐PyCharm。另外由于实验需要展示图像，还需要安装OpenCV。

因此Caffe平台（Windows 7平台）搭建需要如下条件：
- Caffe windows版本及VS2013 （必要）
- cuda 及 cuDNN （GPU模式必要）
- Anaconda2 (Python2.7 必要，可选其他版本）
- PyCharm (Python IDE 必要，可选其他版本）
- OpenCV Python版（ 必要）

## 0.3 Caffe的结构与开发流程

安装Pycaffe后，我们可以通过`import caffe`来查看是否成功安装，通过编译Pycaffe项目所得到Caffe的Python接口包，实际上并不是Caffe的纯Python实现，而将Caffe用Python进行包装，运行中调用的是Caffe的C++代码，但我们仍可以通过Python来查看Caffe网络在运行中的数据。

本节主要介绍Caffe的结构，主要包含了三个部分：Caffe的网络模型结构组成、Caffe的数据结构及Caffe的Python包结构。通过这三个部分的介绍，最后将总结如何用Python进行Caffe的开发。

### 0.3.1 Caffe的模型结构

对于Caffe深度学习系统中，数据和模型是最核心的两个部分，网络模型结构组成是指构建一个网络模型的要素。通常来说，一个深度学习模型是由三个部分组成：
- 
**网络结构配置文件**：主要描述网络各层Layer（卷积层、全连接层、池化层等）的配置参数及各层之间的连接结构。这里的层之间的输入输出都Blob数据结构，所以层之间的连接都是Blob数据结构为媒介的，但在网络结构配置文件中并没有描述Blob结构，而只是描述层的配置，因为层的输入输出Blob可以由层layer的配置参数所确定。一般来说，网络结构配置文件在网络运行中是不可修改的，其大体上分为三类：训练网络结构`train.prototxt`、测试网络结构`test.prototxt`及实施网络结构`deploy.prototxt`。其都是以`*.prototxt`的文件格式存储在磁盘中的。

> 
对于深度学习网络来说，一般都会涉及到三种网络配置，训练时网络（train.prototxt）、测试时网络及实施时网络。这三者的不同之处：
- 输入数据层不一样，训练时网络用的是训练数据，而测试时网络用的是测试数据，而实施时网络，并没有预先设定数据。
- 处理批量不一样，训练时网络的批量batch是一个预先设定的非常重要的超参数，而测试时网络的批量可以在内存范围内任意设置，批量设置越大，总测试时间越少，而实施时网络的批量不重要，一般设为1（即指单次处理一张图）。
- 最后的层不一样，训练时网络最后必须有一个损失层，而测试时网络最后要有一个匹配层（判断检测是否正确），而实施时网络直接输出最终结果。
- 内部结构可能不一样，许多网络在训练时需要应用特定措施，比如dropout，而往往要在测试时网络和实施时网络中移除这些结构。


- 
**网络权重参数文件**：该文件用于保存网络各层的权重值，是以`*.caffemodel`格式存储的文件。而在运行中，其将以Blob数据形式存入内存中。

- 
**训练超参数文件**：用来控制网络训练及测试阶段的超参数，比如测试网络结构配置文件，梯度下降法中的批量、学习率、遗忘因子等参数，测试的间隔迭代次数等等，其同样是用`*.prototxt`的文件格式（比如`solver.prototxt`）


Python下通过上述模型文件调用网络的方法：
- 通过训练超参数文件进行网络训练：
`solver = caffe.SGDSolver('solver.prototxt') # 调用训练器`
`solver.solve() # 直接训练模型`- 通过网络权重参数及网络结构配置文件调用网络：


```
net = caffe.Net('deploy.prototxt', '*.caffemodel',caffe.TEST) 

 

3. 通过训练超参数文件及网络权重参数文件进行权重预设的网络训练： 

    `solver = caffe.SGDSolver('solver.prototxt') # 调用训练器`
`solver.net.copy_from(caffemodel) # 预设权重`
`solver.solve() # 直接训练模型`
`train_net = solver.net # 训练网络`
`test_net = solver.test_nets[0] # 测试网络`


### 0.3.2 Caffe的数据结构

在Caffe中，用Net结构来表示一个深度学习网络，而一个Net由多个层（Layer）拼接而成，而层的参数（比如Conv层的卷积核参数，当然池化层就没有参数）及各层的输入及输出都是Blob数据结构。

因此如果我们将深度学习网络看成是一个原材料（原始图像）加工成某个产品（比如一组反映分类结果的向量）的过程，那么Net就是加工厂，Layer就是流水线上的车间，而原材料或者中间半成品都是以Blob对象形式包装，从而在各个车间中流动的。

#### 1. Blob

Caffe的所有数据（各层的参数、及各层输入及输出）都是以Blob数据结构形式存在于内存中的，其是Caffe的基本存储单元，这同Torch、Theano和Tensorflow中的Tensor结构是一致的。

Blob实际上表示为4维数组，其维度从低到高分别表示为宽、高、通道数及数量。对于深度网络，一般都是通过梯度下降法训练的，因此在Blob结构中除了包含一个存储数据或者权重值的4维数组外，而需要包含一个存储权重增量（diff）的4维数组。

Caffe源代码中，

#### 2. Layer

层（Layer）是Caffe的基本计算单元，Layer是将输入Blob（Bottom）计算得到输出Blob（Top），而其后的Layer会继续将上一层的输出Blob作为输入Blob，从而得到该层的输出Blob，而网络Net就是通过这种方式，将Blob数据一层接一层利用Layer结构计算，从而得到最终结构。

对于层（Layer），其至少会有一个输入Blob或者输出Blob，当然也可以有多个输入或输出Blob。一些层的内部还会有权重值（Weight）和偏置值（Bias）两个Blob结构。

层作为基本计算单元，其主要进行两类运算：前向传播（forward）和后向传播（backward）。在前向传播中，Layer主要是将输入Blob进行某种处理（比如卷积操作）得到输出Blob，而在后向传播中，主要是对输出Blob的diff进行处理得到输入Blob的diff（这也就是损失梯度的反向传播过程），对于需要进行参数更新的层，还会计算权重值Blob及偏置Blob的diff，并用于参数更新（对于批量训练法，会在根据一个批量的累积diff来进行参数更新，而并不是在每次反向传播中都进行参数更新）。

各类层的定义及声明可以参见caffe源代码中的Layer文件夹。

Python下通过操作Layer结构的例子：

`net = caffe.Net('deploy.prototxt', '*.caffemodel',caffe.TEST) # 打开网络`
`conv1_W = net.params['conv1'][0].data # net中的params保存各层的参数，conv1权重值`
`conv1_b = net.params['conv1'][1].data # net中的params保存各层的参数，conv1偏置值`



#### 3. Net

Net在Caffe中代表一个完整的CNN模型，其由若干Layer所组成，我们可以通过`*.prototxt`来定义网络Net的结构。

Net中包含了两个部分结构：Layer结构和Blob结构。其中Blob对象主要用于存放每个Layer层的输入及输出的中间结果，而Layer结构即为对Blob对象进行计算处理的层（比如卷积、全连接、池化、softmax等）。注意的是Blob对象和Layer对象可能重名，但两者是完全不一样的。

![](http://i.imgur.com/VY1DmxG.png)

Net作为全局图纸，其记录网络中的所有结构，以下几个对象是比较的重要的：
- layers_：记录Net中所有层Layer
- blobs_：记录Net中的所有Blob结构
- params_：记录Net可训练的层的权重参数

Python下通过操作Net结构的例子：

`net = caffe.Net('deploy.prototxt', '*.caffemodel',caffe.TEST) # 打开网络`
`# 通过构造训练器得到网络`
`solver = caffe.SGDSolver('solver.prototxt') # 调用训练器`
`train_net = solver.net # 训练网络`
`test_net = solver.test_nets[0] # 测试网络`



### 0.3.3 Caffe Python包结构

我们通过VS2013编译Pycaffe项目会得到一个Caffe的Python包，其位于`pycaffe/caffe`，我们就caffe文件夹移入到`Anaconda2/Lib/site-packages`下，就可以使用Caffe的Python接口了。

在caffe文件夹中，主要需要关注以下几个文件：
- 
`model_libs.py`：主要包含一些生成常用网络模型结构的函数，如带BN的卷积层、ResNet网络主体结构、Inception结构、VGG网络主体结构、带标签的数据层结构等，这些结构都可以由caffe的基础层拼接而成，但并没有Caffe C++源代码所实现。我们也编写属于自己的拼接层结构，之后的教程将给大家介绍如何组织生成自己的网络层结构。`examples/pycaffe/caffenet.py`也给出了自行组织网络结构的例子。

- 
`pycaffe.py`：该文件定义Caffe的python接口，说明了网络能使用的相关Python函数和数据，比如前向传递forward函数、后向传递backward函数、网络参数params、网络数据blobs、网络名称names等等。

- 
`io.py`：包含blobproto和numpy数组之间转换函数，图像数据的变换函数，以及图像导入导出、预处理的相关函数。

- 
`coord_map.py`：处理一些层（Convolution、Pooling等）的输入输出坐标映射。

- 
`net_spec.py`：网络、层、参数结构的基础性声明

- 
`_caffe.lib`：其是由`_caffe.cpp`所编译而来的，打开_caffe.cpp，我们可以看出，这个文件主要用于声明caffe结构到Python结构的名称转换。

- 
Python的其他一些小工具
- 
`classifier.py`：里面有一个分类预测函数

- 
`detector.py`：里面主要是用于目标检测的相关函数

- 
`draw.py`：一些画图命令



一些未安装的Python工具:
- `python\train.py`：train、time及solver的Python运行函数
- `python\draw_net.py`：根据网络配置文件画出网络结构图的函数
- `tools\extra\extract_seconds.py`
- `tools\extra\parse_log.py`
- `tools\extra\resize_and_crop_images.py`
- `tools\extra\summarize.py`
- `tools\extra\plot_training_log.py`

Pycaffe的一些例子`examples\pycaffe`给出一些pycaffe应用例子
- `caffenet.py`：自行生成网络模型的例子
- `.\layer`：生成层结构的例子

#### 如何查看Caffe的Python接口命令

在`net_spec.py`及`pycaffe.py`这两个文件中，我们可以找到对于net、solver等数据结构的声明，但是一些层的声明如何查找呢？

一方面我们可以从`model_libs.py`及`caffenet.py`等文件找到如何使用层结构的例子。另外可能参考[这个教程](https://github.com/abhi-kumar/Caffe-Python-Basic-Tutorial)，里面给出caffe的所有层的Python函数使用例子。

另一方面我们可以参照[caffe的C++类定义](http://caffe.berkeleyvision.org/doxygen/annotated.html)。Python直接将层类定义转换成一个Python函数，函数名称为类名，函数输入参数，可以打开caffe.proto文件，该文件定义params结构，举个例子比如对于pool层来说，该层类名为PoolingLayer，其层名（其Python函数名）为该类的返回类型（type()函数返回）为Pooling。其输入参数，可以在caffe.proto文件中查找PoolingParameter的定义：

```
message PoolingParameter {
  enum PoolMethod {
    MAX = 0;
    AVE = 1;
    STOCHASTIC = 2;
  }
  optional PoolMethod pool = 1 [default = MAX]; // The pooling method
  // Pad, kernel size, and stride are all given as a single value for equal
  // dimensions in height and width or as Y, X pairs.
  optional uint32 pad = 4 [default = 0]; // The padding size (equal in Y, X)
  optional uint32 pad_h = 9 [default = 0]; // The padding height
  optional uint32 pad_w = 10 [default = 0]; // The padding width
  optional uint32 kernel_size = 2; // The kernel size (square)
  optional uint32 kernel_h = 5; // The kernel height
  optional uint32 kernel_w = 6; // The kernel width
  optional uint32 stride = 3 [default = 1]; // The stride (equal in Y, X)
  optional uint32 stride_h = 7; // The stride height
  optional uint32 stride_w = 8; // The stride width
  enum Engine {
    DEFAULT = 0;
    CAFFE = 1;
    CUDNN = 2;
  }
  optional Engine engine = 11 [default = DEFAULT];
  // If global_pooling then it will pool over the size of the bottom by doing
  // kernel_h = bottom->height and kernel_w = bottom->width
  optional bool global_pooling = 12 [default = false];
}
```


因此pooling层函数可以按如下方法使用from caffe import layers as L
L.Pooling(bottom, pool=P.Pooling.MAX, kernel_size=ks, stride=stride, pad = pad)
```






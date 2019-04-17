# 贾扬清等人撰文详解Caffe2：从强大新能力到上手教程 - 知乎
# 



昨天，[Facebook 推出了 Caffe2](https://link.zhihu.com/?target=http%3A//www.jiqizhixin.com/article/2674)，一个兼具表现力、速度和模块性的开源深度学习框架。它沿袭了大量的 Caffe 设计，可解决多年来在 Caffe 的使用和部署之中发现的瓶颈问题。最终，Caffe2 打开了算法实验和新产品的大门。通过在内部用于各种深度学习和增强现实任务，Caffe2 已经在 Facebook 对于规模和性能的需求上得到了锻造。同时，它为移动端应用提供了令人印象深刻的新功能，例如高级相机和即时通讯功能。英伟达作为 Caffe2 的开发合作者，计划对 Caffe2 的深度学习应用推出一系列博客文章。本文即是该系列博文的第一篇，将介绍 Caffe2 的深度学习基础知识，证明其灵活性和速度；本文还将介绍为什么你想要使用 Caffe2、是什么使 Caffe2 区别于 Caffe，最后还会通过一个预训练的目标分类模型给出一个 Caffe2 使用案例。


**一次编码，任意运行**


在保有扩展性和高性能的同时，Caffe2 也强调了便携性。「便携性」通常使人想起 overhead——它如何在诸多不同的平台上工作？overhead 如何影响扩展的能力？Caffe2 当然已把这些考虑在内，其从一开始就以性能、扩展、移动端部署作为主要设计目标。Caffe2 的核心 C++ 库能提供速度和便携性，而其 Python 和 C++ API 使你可以轻松地在 Linux、Windows、iOS、Android 甚至 Raspberry Pi 和 NVIDIA Tegra 上进行原型设计、训练和部署。也许你会问：物联网呢？Caffe2 将适用于大量设备。尽管你并不想在物联网设备上训练网络，但你可以在其上面部署训练好的模型。


当 GPU 可用时，Caffe2 也不会错失这个良机。在 Facebook 和英伟达的合作下，Caffe2 已经可以充分利用英伟达 GPU 深度学习平台。Caffe2 可使用最新的英伟达深度学习 SDK 库——cuDNN、cuBLAS 和 NCCL——来实现高性能、多 GPU 加速训练和推理。


绝大多数内置函数都可根据运行状态在 CPU 模式和 GPU 模式之间无缝切换。这意味着无需额外编程即可享用深度学习超级加速的便利。这引出了 Caffe2 激动人心的另一个方面：多 GPU 和多主机处理。Caffe2 使并行化网络训练变得简单，现在实验和扩展对你而言也非常简单。


最近的 ImageNet 训练基准使用了 64 块最新的英伟达 GPU 和 ResNet-50 神经网络架构。Facebook 工程师实现的 Caffe2 的 data_parallel_model（[https://github.com/caffe2/caffe2/blob/master/caffe2/python/data_parallel_model.py](https://link.zhihu.com/?target=https%3A//github.com/caffe2/caffe2/blob/master/caffe2/python/data_parallel_model.py)）能够在 Facebook 的 8 个 Big Basin 人工智能服务器（每个服务器配有 8 个英伟达 Tesla P100 GPU 加速器，8 个服务器共有 64 块 GPU）上进行分布式神经网络训练。图 1 是这些系统的扩展结果：近乎直线的深度学习训练扩展，带有 57 倍的吞吐量加速。


*![](https://pic2.zhimg.com/v2-84032418bd1a74990c7f92b3bc1ab241_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='492' height='492'></svg>)图 1: 通过 Resnet-50 模型在多达 64 个英伟达 Tesla P100 GPU 加速器上训练的 Caffe2 扩展系数*



**Caffe2 的新功能**


你也许记得在 Caffe 中一切都表征为一个「网」（Net），它由「层」组成，这些层是以神经网络中心化方式来定义计算。然而，这创建了一种非常刚性的计算模式，并带来了很多硬编码例程，尤其在深度神经网络训练方面。


Caffe2 采用了更现代的计算图（computation graph）来表征神经网络或者包括集群通信和数据压缩在内的其它计算。这一计算图采用「算子」（operator）的概念：在给定输入的适当数量和类型以及参数的情况下，每个算子都包含了计算输所必需的逻辑。尽管 Caffe 中的层总是采用张量（矩阵或多维数组），但 Caffe2 中的算子可采用并产生包含随意对象的「blob」，这一设计使得很多过去在 Caffe 中不可实现的事情成为可能：

- 
CNN 分布式训练可由单个计算图表征，不管是在一个或多个 GPU 还是在多台机器上训练。这对 Facebook 规模的深度学习应用很关键。

- 
在专业硬件上轻松进行异构计算。例如，在 iOS 上，Caffe2 计算图可从 CPU 获取图像，将其转化为 Metal GPU 缓存对象，并将计算完全保留在 GPU 上，以获得最大吞吐量。

- 
更好地管理运行时间资源，比如使用 memonger 优化静态内存，或者预打包训练网络以获得最佳性能。

- 
float、float16、int8 混合精度和其他量化模型的计算。



Caffe2 有超过 400 个算子，具备广泛的功能。你可以浏览算子目录（[Operators Catalogue](https://link.zhihu.com/?target=http%3A//caffe2.ai/docs/operators-catalogue.html)）、查看稀疏操作（[Sparse Operations](https://link.zhihu.com/?target=http%3A//caffe2.ai/docs/sparse-operations.html)）并学习如何编写自定义算子（[Custom Operators](https://link.zhihu.com/?target=http%3A//caffe2.ai/docs/custom-operators.html)）。


**安装与设置**


你要做的第一件事就是查看 Caffe2 的 GitHub 主页，clone 或 fork 该项目的 Github repo。

`git clone https://github.com/caffe2/caffe2.git`

如果你安装不了 Caffe2，你可以查看以下安装指南：[Install](https://link.zhihu.com/?target=http%3A//caffe2.ai/docs/getting-started.html)，尝试这个 Docker 镜像：[https://hub.docker.com/r/caffe2ai/caffe2/](https://link.zhihu.com/?target=https%3A//hub.docker.com/r/caffe2ai/caffe2/)，或者在你选择的云提供商上运行。其文档也为每个选项提供了指令。然而，我们建议你通过建立 GPU 支持的云实例，来验证 GPU 的处理速度。以下是使用 Docker 建立 GPU 支持的 Caffe2 的快速方式：

`nvidia-docker run -it caffe2ai/caffe2 python -m caffe2.python.operator_test.relu_op_test`

**尝试一个预训练的模型**


现在让我们实际上手试一试！在这第一个教程中，我会教你如何轻松使用 Caffe2 的 Model Zoo 和模型下载器（model downloader），以帮你自己动手实验一些其它模型。Model Zoo 链接：[Caffe2 Model Zoo](https://link.zhihu.com/?target=http%3A//caffe2.ai/docs/zoo.html)


**使用 Caffe2 的模型下载器**


这是一个下载模块（[https://github.com/caffe2/caffe2/blob/master/caffe2/python/models/download.py](https://link.zhihu.com/?target=https%3A//github.com/caffe2/caffe2/blob/master/caffe2/python/models/download.py)），你可以使用它来获取预训练好的网络。你可以将该模块整合到你的脚本中，或者在命令行中使用它：

`python -m caffe2.python.models.download <model name>`

比如，这行命令可以下载 squeezenet 预训练模型：

`python -m caffe2.python.models.download squeezenet`

下载了 squeezenet 之后，你可以加载它。这个模型下载器模块有一个安装（install）选项，你可以使用 -i 开启。否则你就需要在下载文件后自己移动它。一旦安装完成，你也可以直接将这些模型导入到你的 Python 脚本：

`python -m caffe2.python.models.download -i squeezenet`

**运行一个预训练模型：目标分类**


让我们试试用 Caffe2 做一次目标分类。如果你已经下载了一个预训练模型，这做起来就很简单。如果你还没有下载 squeezenet，你可以使用上述方法下载，也可从 S3 下载 init_net.pb 和 predict_net.pb 文件。


> 
*init_net.pb：[https://s3.amazonaws.com/caffe2/models/squeezenet/init_net.pb](https://link.zhihu.com/?target=https%3A//s3.amazonaws.com/caffe2/models/squeezenet/init_net.pb)*

*predict_net.pb：[https://s3.amazonaws.com/caffe2/models/squeezenet/predict_net.pb](https://link.zhihu.com/?target=https%3A//s3.amazonaws.com/caffe2/models/squeezenet/predict_net.pb)*



将下载好的文件放到 $PYTHONPATH/caffe2/python/models/squeezenet 文件夹。你的 Python 代码需要 Caffe2 的 workspace 来存放该模型的 protobuf 负载和权重，并将它们加载到 blob、init_net 和 predict_net 中。你将需要 workspace.Predictor 来接收这两个 protobuf，然后剩下的就交给 Caffe2 处理了。Caffe2 有一个简单的 run 函数，可以输入图像并进行分析，然后返回一个带有结果的张量。


```
# load up the caffe2 workspace
from caffe2.python import workspace
# choose your model here (use the downloader first)
from caffe2.python.models import squeezenet as mynet
# helper image processing functions
import caffe2.python.tutorials.helpers as helpers

# load the pre-trained model
init_net = mynet.init_net
predict_net = mynet.predict_net
# you must name it something
predict_net.name = "squeezenet_predict"
workspace.RunNetOnce(init_net)
workspace.CreateNet(predict_net)
p = workspace.Predictor(init_net.SerializeToString(), predict_net.SerializeToString())

# use whatever image you want (local files or urls)
img = “https://upload.wikimedia.org/wikipedia/commons/thumb/7/7b/Orange-Whole-%26-Split.jpg/1200px-Orange-Whole-%26-Split.jpg”
img = "https://upload.wikimedia.org/wikipedia/commons/a/ac/Pretzel.jpg"
img = "https://cdn.pixabay.com/photo/2015/02/10/21/28/flower-631765_1280.jpg"
# average mean to subtract from the image
mean = 128
# the size of images that the model was trained with
input_size = 227

# use the image helper to load the image and convert it to NCHW
img = helpers.loadToNCHW(img, mean, input_size)

# submit the image to net and get a tensor of results
results = p.run([img])
response = helpers.parseResults(results)
# and lookup our result from the list
print response
```


该结果是一个概率的张量（一个多维数组）。本质上来看，每一行都表示了目标与神经网络所识别出的内容相匹配的几率。


注意，当该 workspace 的 Predictor 函数被调用来加载该预训练的模型时，下一步就是调用 .run 并给该函数传递一个图像数组。


```
p = workspace.Predictor(init_net, predict_net)
results = p.run([img])
```


**图像预处理**


为了更快的处理速度和传统上的原因，图像在被送入 Caffe2 之前还需要经过两步转换：


1. 将颜色从 RGB 转换成 BGR

2. 将图像封装成像素数组，并提供批（batch）中图像的数量（在这案例中是 1）、通道（按 BGR 排列的像素）的数量、高度和宽度，它们分别被称为：NCHW for Number、Channels、Height 和 Width。


这些图像预处理函数由一个助手模块（helper module）进行处理，所以你可以仅关注特定于 Caffe2 的交互。要更深入地了解图像预处理，请参阅相关 IPython 笔记：[caffe2/caffe2](https://link.zhihu.com/?target=https%3A//github.com/caffe2/caffe2/blob/master/caffe2/python/tutorials/Image_Pre-Processing_Pipeline.ipynb)


**获得结果**


当模型完成图像数组的处理时，你会获得一个多维数组，其形式为（1, 1, 1000, 1, 1）


```
results = np.asarray(results)
print "results shape: ", results.shape

results shape:  (1, 1, 1000, 1, 1)
```


看到 results.shape 中的那个 1000 了吗？如果该批中不止一张图片，那么这个数组就会更大，但在中间仍然有 1000 个单元。其存放了该预训练模型中每个类别的概率。所以当你查看结果时，就好像是在说：「计算机，这是一个铍（Beryllium）球的概率是多少？」还是说这是一只毒蜥或其它 998 种该模型被训练用来识别的类别。


这是一个从 1000 长度的张量中提取出的前 3 个结果，已经过收缩和排序。这些结果按照匹配的概率进行了排序，0.98222 (98%) 是最高的。


> 
*[array([985.0, 0.9822268486022949], dtype=object), array([309.0, 0.011943698860704899], dtype=object), array([946.0, 0.004810151644051075], dtype=object)]*


这是按照概率排列的前三个类别，说明了被检测的目标属于某个类别的概率。你可以使用这个 gist 来查看结果：[https://gist.githubusercontent.com/aaronmarkham/cd3a6b6ac071eca6f7b4a6e40e6038aa/raw/9edb4038a37da6b5a44c3b5bc52e448ff09bfe5b/alexnet_codes](https://link.zhihu.com/?target=https%3A//gist.githubusercontent.com/aaronmarkham/cd3a6b6ac071eca6f7b4a6e40e6038aa/raw/9edb4038a37da6b5a44c3b5bc52e448ff09bfe5b/alexnet_codes)。每次你运行这个示例时，你都会得到有轻微差别的结果。运行一张有一些雏菊的图片，该模型会得到：![](https://pic3.zhimg.com/v2-322465c8702322eee420fc858a072d96_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1284' height='392'></svg>)


除了最高概率的正确答案之外，第二和第三分别是蜜蜂和刺棘蓟，鉴于蜜蜂常常和花出现在同一张照片中，所以这样的结果也有点道理。![](https://pic3.zhimg.com/v2-71eea047c8c3a189cd3438cee727dca2_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='310' height='148'></svg>)


输入一张切开的橘子照片，可以得到：橘子（95.3%）、柠檬（4.6%）、草莓（0.006%）。



**Caffe2 的合作与共享**


Caffe2 发展的基石是对深度学习感兴趣并使用过 Caffe 以及其它开源机器学习工具的开发者、研究人员和公司组成的社区。通过在 Caffe2 上的开源以及在 Model Zoo 上的创新协作，我们希望能够推进人工智能科学的进步，并促进各个产业的收益。Caffe2 开源项目的成员能直接在列举所有模型的 Caffe2 Github Wiki 页面做贡献：[caffe2/caffe2](https://link.zhihu.com/?target=https%3A//github.com/caffe2/caffe2/wiki)。


我们也邀请了开发者、研究人员以及任何对创造或精调模型感兴趣的人在 Caffe2 GitHub 的「issue」页面进行分享：[caffe2/caffe2](https://link.zhihu.com/?target=https%3A//github.com/caffe2/caffe2/issues)，也可以要求把问题添加到 Zoo 中。此外，Github 的「issue」部分不只是面向 Caffe2 的开发者。如果你创建 Caffe2 模型、改进预训练模型，甚至只是使用预训练模型，你也能在该部分对此资源和 Model Zoo 进行输入、建议与贡献。关于 Caffe2 和 Model Zoo 合作（[Caffe2 Model Zoo](https://link.zhihu.com/?target=http%3A//caffe2.ai/docs/zoo.html)）的进一步信息请移步 [Caffe 2](https://link.zhihu.com/?target=http%3A//caffe2.ai)。


**在 GTC 大会上学习 Caffe2**


这部分包含了我们第一篇 Parallel Forall 博客。接下来的一篇博客中我们将会深解如何使用 Caffe2 训练 ImageNet，在文章内我们会展示一些有关分布式训练的激动人心的新基准、在优化 Caffe 2 训练上的小建议以及如何使用 Caffe2 的数据并行模型的细节。


此外 5 月 8 日 - 11 日在圣何塞市举办的 GTC 大会是每年一度的人工智能与 GPU 开发者大会，机器之心作为受邀媒体将会对此大会的精彩内容进行报道。

选自[Devblogs.nvidia](https://link.zhihu.com/?target=https%3A//devblogs.nvidia.com/parallelforall/caffe2-deep-learning-framework-facebook/)**机器之心编译**



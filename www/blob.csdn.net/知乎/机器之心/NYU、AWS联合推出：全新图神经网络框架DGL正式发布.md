# NYU、AWS联合推出：全新图神经网络框架DGL正式发布 - 知乎
# 



机器之心发布，作者：DGL Team。

深度神经网络在近年来的发展和成功有目共睹。这些网络架构成功的一个重要原因在于其能有效地捕捉到了数据内在的相关性，并在不同的抽象级别构建表示 (representation)。比如 CNN 刻画了图片中相邻像素点间的空间不变性；RNN 抓住了文本数据的有序线性结构。

CNN 和 RNN 的成功让人思考——我们是否能将此思想拓展到其他结构的数据上呢？事实上，人们一直在探索如何将深度学习应用于更广泛的结构数据中。早在 2014 年，Kai Sheng Tai 等人就研究了能在文本语法树上训练的树神经网络模型 TreeLSTM。这个工作在一定程度上冲击了大家用 RNN 处理文本的范式，并且用树型结构看待文本数据开创了很多新的研究可能。从链表到树，从树到图：近年来，对于图神经网络（Graph Neural Network）的研究热潮使得神经网络的应用领域大大增加。
![](https://pic4.zhimg.com/v2-633083209113cfeafb5a1a939bf95573_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='933' height='350'></svg>)
小到分子结构图（[Gilmer et al., 2017], [Jin et al., 2018]），大到知识图谱（[Schlichtkrull et al., 2017]）、社交网络和推荐系统（[Ying et al., 2018]），图神经网络都展现了强大的应用潜力。

广义上说，没有一个点的特征表达是孤立的；丰富特征的一个重要手段必然是融合该点和其他邻居点的互动。从这个意义上来说，CNN 和 RNN 分别把空间和时间上的相邻点看作互动对象是非常粗糙的。把图和深度学习结合的意义正在于突破这个局限。

由于结构数据的普适性，Google, Deepmind, Pinterest 等公司也纷纷在这一领域发力。除了数据，深度神经网络模型的发展也有着更加动态和更加稀疏的趋势。这源于目前深度神经网络对于算力的需求已经达到了一个惊人的地步。同时，我们观察到现在还没有一个框架既高效，又好用，能帮助开发新的模型。

（比如目前火热的 BERT 模型在一台 8 块 V100 的服务器上预计训练时间是 42 天。）

于是一个很现实的问题是如何设计「既快又好」的深度神经网络？也许更加动态和稀疏的模型会是答案所在。可见，不论是数据还是模型，「图」应该成为一个核心概念。

基于这些思考，我们开发了 Deep Graph Library（DGL），一款面向图神经网络以及图机器学习的全新框架。

在设计上，DGL 秉承三项原则：
- DGL 必须和目前的主流的深度学习框架（PyTorch、MXNet、TensorFlow 等）无缝衔接。从而实现从传统的 tensor 运算到图运算的自由转换。
- DGL 应该提供最少的 API 以降低用户的学习门槛。
- 在保证以上两点的基础之上，DGL 能高效并透明地并行图上的计算，以及能很方便地扩展到巨图上。

这里，我们简单介绍 DGL 是如何实现以上目标的。

**设计一：DGL 是一个「框架上的框架」**
![](https://pic3.zhimg.com/v2-a92615bbf3148e1db924586960463362_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='619' height='493'></svg>)
神经网络计算的重要核心是稠密张量计算和自动求导。当前神经网络框架对此都已经有了非常良好的支持。此外，由于神经网络模型的日益模块化，用户对于这些深度学习的框架的黏性度也越来越高。

为了避免重复造轮子（build a wheel again），DGL 在设计上采取了类似 Keras 的做法——基于主流框架之上进行开发。但又不同于 Keras，DGL 并不限制用户必须使用自己的语法。相反，DGL 鼓励用户在两者间灵活地使用。比如用户可以使用他们偏爱的框架编写常见的卷积层和注意力层，而当遇到图相关的计算时则可以切换为 DGL。用户通过 DGL 调用的计算，经过系统优化，仍然调用底层框架的运算和自动求导等功能，因此开发和调试都能更快上手。用户和 DGL 的交互主要通过自定义函数 UDF（user-defined function）。在下一节编程模型我们将继续展开。目前 DGL 支持 Pytorch 以及 MXNet/Gluon 作为系统后端。

**设计二：基于「消息传递」（message passing）编程模型**

消息传递是图计算的经典编程模型。原因在于图上的计算往往可以表示成两步：
- 发送节点根据自身的特征（feature）计算需要向外分发的消息。
- 接受节点对收到的消息进行累和并更新自己的特征。

比如常见的卷积图神经网络 GCN（Graph Convolutional Network）可以用下图中的消息传递模式进行表示（图中 h 表示节点各自的特征）。用户可以定制化消息函数（message function），以及节点的累和更新函数（reduce function）来构造新的模型。事实上，在 Google 的 2017 年的论文中 [Gilmer et al. 2017] 将很多图神经网络都归纳到了这一体系内。
![](https://pic2.zhimg.com/v2-bac28a4d0d3885fc7af721c265e97bd9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='498' height='365'></svg>)
DGL 的编程模型正基于此。以下是图卷积网络在 DGL 中的实现（使用 Pytorch 后端）：
![](https://pic2.zhimg.com/v2-6fc8e2aff69b6b008825d78a19b3d415_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='752' height='630'></svg>)
可以看到，用户可以在整个程序中灵活地使用 Pytorch 运算。而 DGL 则只是辅助地提供了诸如 mailbox、send、recv 等形象的消息传递 API 来帮助用户完成图上的计算。在我们内部的项目预演中，我们发现熟悉深度学习框架的用户可以很快上手 DGL 并着手开发新模型。




**设计三：DGL 的自动批处理（auto-batching）**

好的系统设计应该是简单透明的。在提供用户最大的自由度的同时，将系统优化最大程度地隐藏起来。图计算的主要难点在于并行。

我们根据模型特点将问题划分为三类。首先是处理单一静态图的模型（比如 citation graph），其重点是如何并行计算多个节点或者多条边。DGL 通过分析图结构能够高效地将可以并行的节点分组，然后调用用户自定义函数进行批处理。相比于现有的解决方案（比如 Dynet 和 TensorflowFold），DGL 能大大降低自动批处理的开销从而大幅提高性能。

第二类是处理许多图的模型（比如 module graph），其重点是如何并行不同图间的计算。DGL 的解决方案是将多张图合并为一张大图的多个连通分量，从而将该类模型转化为了第一类。

第三类是巨图模型（比如 knowledge graph），对此，DGL 提供了高效的图采样接口，将巨图变为小图样本，从而转化为第一类问题。

目前 DGL 提供了 10 个示例模型，涵盖了以上三种类别。其中除了 TreeLSTM，其余都是 2017 年以后新鲜出炉的图神经网络，其中包括几个逻辑上相当复杂的生成模型（DGMG、JTNN）我们也尝试用图计算的方式重写传统模型比如 Capsue 和 Universal Transformer，让模型简单易懂，帮助进一步扩展思路。

我们也对 DGL 的性能进行了测试：
![](https://pic4.zhimg.com/v2-65c6db73de0b42b6f0405ffc6123ada7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='866' height='378'></svg>)
可以看到，DGL 能在这些模型上都取得了相当好的效果。我们也对 DGL 在巨图上的性能进行了测试。在使用 MXNet/Gluon 作为后端时，DGL 能支持在千万级规模的图上进行神经网络训练。

DGL 现已开源。
- 主页地址：[http://dgl.ai](https://link.zhihu.com/?target=http%3A//dgl.ai)
- 项目地址：[https://github.com/jermainewang/dgl](https://link.zhihu.com/?target=https%3A//github.com/jermainewang/dgl)
- 初学者教程：[https://docs.dgl.ai/tutorials/basics/index.html](https://link.zhihu.com/?target=https%3A//docs.dgl.ai/tutorials/basics/index.html)
- 所有示例模型的详细从零教程：[https://docs.dgl.ai/tutorials/models/index.html](https://link.zhihu.com/?target=https%3A//docs.dgl.ai/tutorials/models/index.html)

我们欢迎任何来自社区的贡献，同时也希望能和大家一起学习进步，推动结构化深度学习这一个新方向。

DGL 项目由纽约大学、纽约大学上海分校、AWS 上海研究院以及 AWS MXNet Science Team 共同开发（详见 [https://www.dgl.ai/ack](https://link.zhihu.com/?target=https%3A//www.dgl.ai/ack)）。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*






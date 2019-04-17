# TensorFlow 源代码初读感受 - LeftNotEasy - 博客园







# [TensorFlow 源代码初读感受](https://www.cnblogs.com/LeftNotEasy/p/rough-understanding-of-tensorflow-sourcecode.html)





把自己微博发的文章：[http://www.weibo.com/1804230372/En7PdlgLb?from=page_1005051804230372_profile&wvr=6&mod=weibotime&type=comment](http://www.weibo.com/1804230372/En7PdlgLb?from=page_1005051804230372_profile&wvr=6&mod=weibotime&type=comment) 也转到博客来，算是自己写的很短的一篇博客了。

花了一些时间看了一下TensorFlow 核心(core)部分的源代码，主要是C++的部分。除了C++ 11的代码实在有些烧脑以外，感叹一下TensorFlow的架构设计得真不错：

首先TensorFlow core要解决的问题全部集中在了图上：每个节点(node)就是一个操作(op)，比如说矩阵乘法，每条边(edge)就是操作的输入也就是数据或者依赖关系（称为control edge)。每个op对应了不同的实现，也被称为kernel，比如说用GPU去做矩阵运算的实现，或者FPGA去做加法的实现等等。

然后就是如何优化了：

1) 首先是一个图的变换操作：用户输入的图根据op、kernel和输入/中间数据，被分布到不同的机器的硬件（device）上。这样把一个大图分拆成为了几个子图（每个device上一个？）。

2) 然后对分拆之后的子图再进行进一步的优化，比如说把那些无法访问的节点(dead node)删掉，把那些输入、输出、操作都一样的子图找出来防止重复计算 (Common Subexpression Elimination)等等

3）最后就是一些下层模块的优化，比如说RPC怎么做更快，Kernel怎么实现更优化等等。

这样的架构设计基本上能保证

1）不同层级之间的耦合很小，不至于一个改动牵一而发动全身。

2）项目的扩展性很好，有很少的瓶颈。比如说完全可以有一个小团队专门进行RPC的实现（RDMA），或者说用FPGA来实现不同op的kernel等等。这样可以完全让一个上百人的团队来并行的开发kernel和周边的东西。

目前读下来唯一觉得可能会导致扩展性的地方是Session还有图优化部分的实现，很难让大团队能够在这个部分并行工作，不过这部分代码也不是劳动密集型的改动，所以看起来也没什么大问题













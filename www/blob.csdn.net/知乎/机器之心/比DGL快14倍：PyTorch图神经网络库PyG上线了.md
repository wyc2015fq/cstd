# 比DGL快14倍：PyTorch图神经网络库PyG上线了 - 知乎
# 



> [图神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650758310%26idx%3D1%26sn%3D64655e53fddb1f9492b8b6a1fe3a382b%26chksm%3D871a98d8b06d11ce8292442045b293b2819f5057b726e594a1896ea7c2ae0355949e9df7bd03%26token%3D1236375972%26lang%3Dzh_CN)是最近 AI 领域最热门的方向之一，很多图神经网络框架如 [graph_nets](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650750321%26idx%3D2%26sn%3Df4346bbe52a63d7b0f6a8a326c74aca2%26chksm%3D871af90fb06d70198e57661233f6865f1742bc8065e45f96698ad5c901105ea40320177a9072%26scene%3D21%23wechat_redirect) 和 [DGL](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650753378%26idx%3D2%26sn%3D66cd0204e55439476745b751da8ecd61%26chksm%3D871a8d1cb06d040a59581226fabbbfcdc2b70e8adcdfa33ee91662dbd99112cda94408858cc7%26scene%3D21%23wechat_redirect)已经上线。但看起来这些工具还有很多可以改进的空间。近日，来自德国多特蒙德工业大学的研究者们提出了 PyTorch Geometric，该项目一经上线便在 GitHub 上获得 1500 多个 star，并得到了 Yann LeCun 的点赞。现在，创建新的 GNN 层更加容易了。

项目链接：[https://github.com/rusty1s/pytorch_geometric](https://link.zhihu.com/?target=https%3A//github.com/rusty1s/pytorch_geometric)
![](https://pic2.zhimg.com/v2-7ba5f2c75bf4c359e241b715baa08ab5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='750'></svg>)
LeCun评价：一个快速且漂亮的几何深度学习库（适用于图神经网络与其他不规则结构）

PyTorch Geometric 主要是现有模型快速重新实现的集合（具有定制化的稀疏操作），如果你想尝试一下特定的已发布模型，它会是一个很不错的选择；如果想实现更为复杂的结构，其自定义稀疏/分散操作也非常方便。

PyTorch Geometric是基于PyTorch构建的深度学习库，用于处理不规则结构化输入数据（如图、点云、流形）。除了一般的图形数据结构和处理方法外，它还包含从关系学习到3D数据处理等领域中最新发布的多种方法。通过利用稀疏 GPU 加速、提供专用的 CUDA 内核以及为不同大小的输入样本引入高效的小批量处理，PyTorch Geometric 实现了很高的数据吞吐量。

PyTorch Geometric（PyG）库包含易用的小批量加载器（mini-batch loader）、多GPU支持、大量常见基准数据集和有用的变换，适用于任意图像、三维网格（3D mesh）和点云。

其作者Matthias Fey 和 Jan E. Lenssen 来自德国多特蒙德工业大学，他们通过实验证明了该库已实现方法在图分类、点云分类、半监督节点分类等任务上的性能。此外，PyG 速度奇快，大大超过其它深度图神经网络库，如 DGL。

**简单易用**

PyTorch Geometric大大简化了实现图卷积网络的过程。比如，它可以用以下几行代码实现一个层（如edge convolution layer）：
![](https://pic2.zhimg.com/v2-9de85222f831945ed880d46819d7f2d5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='765' height='427'></svg>)
**速度快**

PyTorch Geometric 速度非常快。下图展示了这一工具和其它图神经网络库的训练速度对比情况：
![](https://pic3.zhimg.com/v2-d77811785d4ed2c37316f2d2d771116e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='729' height='480'></svg>)
最高比 DGL 快 14 倍！

**已实现方法多**

PyTorch Geometric 目前已实现以下方法，所有实现方法均支持 CPU 和 GPU 计算：
![](https://pic2.zhimg.com/v2-4af38b0f1d10c1d23a1508d219ab4a4d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='644'></svg>)![](https://pic2.zhimg.com/v2-5def3d24d5e0ceff4fbd98c82e9f54ad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='983' height='389'></svg>)
**PyG 概览**

图神经网络（GNN）最近成为在图、点云和流形上进行表征学习的强大方法。与规则域中常用的卷积层和池化层概念类似，GNN通过传递、变换和聚合信息来 （层级化地）提取局部嵌入。

但是，实现GNN并不容易，因为它需要在不同大小的高度稀疏与不规则数据上实现较高的GPU吞吐量。PyTorch Geometric (PyG) 是基于Pytorch构建的几何深度学习扩展库。它可以利用专门的CUDA内核实现高性能。在简单的消息传递API之后，它将大多数近期提出的卷积层和池化层捆绑成一个统一的框架。所有的实现方法都支持 CPU 和 GPU 计算，并遵循不变的数据流范式，这种范式可以随着时间的推移动态改变图结构。PyG已在MIT许可证下开源，它具备完备的文档，且提供了附带教程和示例。

PyG 用节点特征矩阵 X ∈ ℝ^(N×F) 和稀疏邻接元组（I,E）来表示图 G = (X, (I, E))，其中 I ∈ ℕ^(2×E) 以坐标（COO）格式编码边索引，E ∈ ℝ^(E×D)（可选地）保留D维边特征。所有面向用户的API（如数据加载路径、多GPU支持、数据增强或模型实例化）都受到了PyTorch的极大启发，以让用户尽可能地熟悉它们。

**邻域聚合（neighborhood aggregation）。**将卷积运算泛化至不规则域通常表示为邻域聚合或消息传递方案（Gilmer et al., 2017）
![](https://pic2.zhimg.com/v2-a4f86e2fae64e8c947c8b43dfb1e0891_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='729' height='83'></svg>)
其中 ⬚ 表示可微置换不变函数（permutation invariant function），如求和、均值或最大值，r 和 
![](https://pic2.zhimg.com/v2-d29c13ab57f3b4d03250f26e3ef89db5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='89' height='111'></svg>)
表示可微函数 ，如MLP。实践中，r 和 
![](https://pic2.zhimg.com/v2-d29c13ab57f3b4d03250f26e3ef89db5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='89' height='111'></svg>)
的逐元素计算可以通过收集和散射节点特征、利用广播来实现，如图1所示。尽管该方案处理的是不规则结构化输入，但它依然可以通过GPU实现大幅加速。
![](https://pic2.zhimg.com/v2-20e96dd25f7d48d69fb3a47cc8713f89_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='822' height='250'></svg>)
图 1：GNN 层计算方法。利用基于边索引 I 的收集和散射方法，从而在节点并行空间（parallel space）和边并行空间之间进行交替。 

PyG为用户提供通用的MessagePassing接口，以便对新的研究想法进行快速干净的原型制作。此外，几乎所有近期提出的邻域聚合函数都适用于此接口，其中包括PyG已经集成的方法。

**全局池化。**PyG提供多种readout函数（如global add、mean 或 max pooling），从而支持图级别输出，而非节点级别输出。PyG还提供更加复杂的方法，如 set-to-set (Vinyals et al., 2016)、sort pooling (Zhang et al., 2018) 和全局软注意力层 (Li et al., 2016)。

**层级池化（Hierarchical Pooling）。**为进一步提取层级信息和使用更深层的GNN模型，需要以空间或数据依赖的方式使用多种池化方法。PyG目前提供Graclus、voxel grid pooling、迭代最远点采样算法（iterative farthest point sampling algorithm）的实现示例，以及可微池化机制（如DiffPool和top_k pooling）。

**小批量处理。**PyG 可自动创建单个（稀疏）分块对角邻接矩阵（block-diagonal adjacency matrix），并在节点维度中将特征矩阵级联起来，从而支持对多个（不同大小）图实例的小批量处理。正因如此，PyG可在不经修改的情况下应用邻域聚合方法，因为不相连的图之间不会出现信息交流。此外，自动生成的 assignment 向量可确保节点级信息不会跨图聚合，比如当执行全局聚合运算时。

**处理数据集。**PyG提供统一的数据格式和易用的接口，方便使用者创建和处理数据集，大型数据集和训练期间可保存在内存中的数据集皆可适用。要想创建新数据集，用户只需读取／下载数据，并转换为PyG数据格式即可。此外，用户可以使用变换（transform，即访问单独的多个图并对其进行变换）方法来修改数据集，比如数据增强、使用合成结构化图属性来增强节点特征等，从而基于点云自动生成图，或者从网格中自动采样点云。

PyG目前支持大量常见基准数据集，它们均可在第一次初始化时自动下载和处理。具体来讲，PyG提供60多个 graph kernel 基准数据集 (Kersting et al., 2016)，如 PROTEINS 或 IMDB-BINARY、引用网络数据集 Cora、CiteSeer、PubMed 和 Cora-Full (Sen et al., 2008; Bojchevski & Günnemann, 2018)、Coauthor CS/Physics 和 Amazon Computers/Photo 数据集 (Shchur et al. (2018)、分子数据集 QM7b (Montavon et al., 2013) 和 QM9 (Ramakrishnan et al., 2014)，以及Hamilton 等人 (2017) 创建的蛋白质相互作用图。此外，PyG还提供嵌入式数据集，如MNIST超像素 (Monti et al., 2017)、FAUST (Bogo et al., 2014)、ModelNet10/40 (Wu et al., 2015)、ShapeNet (Chang et al., 2015)、COMA (Ranjan et al., 2018)，以及 PCPNet 数据集 (Guerrero et al., 2018).

## **实证评估**

**半监督节点分类**
![](https://pic2.zhimg.com/v2-3ac21580ace5dca83d94e28f97f2a019_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='753' height='214'></svg>)
表 1：多个模型使用固定分割和随机分割的半监督节点分类结果。

**图分类**
![](https://pic2.zhimg.com/v2-36d9bc22c9af361fe42b68fd3c2f04c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='781' height='343'></svg>)
表 2：图分类。

**点云分类**
![](https://pic1.zhimg.com/v2-26474832ec781eb37de6a785cafb3948_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='729' height='465'></svg>)
表3：点云分类。
看起来，图神经网络框架的竞争正愈发激烈起来，PyTorch Geometric 也引起了 DGL 创作者的注意，来自 AWS 上海 AI 研究院的 Ye Zihao 对此评论道：「目前 DGL 的速度比 PyG 慢，这是因为它 PyTorch spmm 的后端速度较慢（相比于 PyG 中的收集+散射）。在 DGL 的下一个版本（0.2）中，我们将报告新的模型训练速度数据，并提供基准测试脚本。我们还将提供定制内核支持以加速 GAT，敬请期待！」
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)

论文链接：[https://arxiv.org/abs/1903.02428](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1903.02428)



# 新的PyTorch图神经网络库，比前辈快14倍：LeCun盛赞，GitHub 2200星 - 知乎
# 



> 方栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-21f9e5e73994b432552a8a49c2bb38b4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='910' height='260'></svg>)
“CNN已老，GNN当立！”

当科学家们发现，**图神经网络** (GNN) 能搞定传统CNN处理不了的非欧数据，从前深度学习解不开的许多问题都找到了钥匙。

如今，有个图网络PyTorch库，已在GitHub摘下**2200多星**，还被CNN的爸爸**Yann LeCun**翻了牌：
![](https://pic4.zhimg.com/v2-031c438cc2380c535fce20fd59634883_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='826'></svg>)
它叫**PyTorch Geometric**，简称PyG，聚集了**26项**图网络研究的代码实现。

这个库还很**快**，比起前辈DGL图网络库，PyG最高可以达到它的15倍速度。

## **应有尽有的库**

要跑结构不规则的数据，就用PyG吧。不管是**图形** (Graphs)，**点云** (Point Clouds) 还是**流形** (Manifolds) 。
![](https://pic4.zhimg.com/v2-63e1726f0a01167db1e0152951964973_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='796' height='339'></svg>)△ 右边是不规则的，非欧空间
这是一个丰盛的库：许多模型的**PyTorch实现**，各种有用的**转换** (Transforms) ，以及大量常见的**benchmark数据集**，应有尽有。

说到实现，包括Kipf等人的图卷积网络 (**GCN**) 和Bengio实验室的图注意力网络 (**GAT**) 在内，2017-2019年各大顶会的 (至少) 26项图网络研究，这里都能找到快速实现。

到底能多快？PyG的两位作者用英伟达GTX 1080Ti做了实验。

对手**DGL**，也是图网络库：
![](https://pic1.zhimg.com/v2-aa7123698378e6080add709885c112f4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1012' height='706'></svg>)
在四个数据集里，PyG全部比DGL跑得快。最悬殊的一场比赛，是在Cora数据集上运行GAT模型：跑200个epoch，对手耗时**33.4秒**，PyG只要**2.2秒**，相当于对方速度的15倍。

每个算法的实现，都支持了CPU计算和GPU计算。

## **食用方法**

库的作者，是两位德国少年，来自多特蒙德工业大学。
![](https://pic4.zhimg.com/v2-b6a89be6e25d25e8bef00aa9deb8d3cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='506' height='625'></svg>)△ 其中一位
他们说，有了PyG，做起图网络就像一阵微风。

你看，实现一个边缘卷积层 (Edge Convolution Layer) 只要这样而已：


```python
import torch
  from torch.nn import Sequential as Seq, Linear as Lin, ReLU
  from torch_geometric.nn import MessagePassing
 
  class EdgeConv(MessagePassing):
      def __init__(self, F_in, F_out):
          super(EdgeConv, self).__init__()
          self.mlp = Seq(Lin( * F_in, F_out), ReLU(), Lin(F_out, F_out))
  
     def forward(self, x, edge_index):
         # x has shape [N, F_in]
         # edge_index has shape [2, E]
         return self.propagate(aggr='max', edge_index=edge_index, x=x)  # shape [N, F_out]
 
     def message(self, x_i, x_j):
         # x_i has shape [E, F_in]
         # x_j has shape [E, F_in]
         edge_features = torch.cat([x_i, x_j - x_i], dim=)  # shape [E, 2 * F_in]
         return self.mlp(edge_features)  # shape [E, F_out]
```


安装之前确认一下，至少要有PyTorch 1.0.0；再确认一下cuda/bin在$PATH里，cuda/include在$CPATH里：


```python
$ python -c "import torch; print(torch.__version__)"
>>> 1.0.

$ echo $PATH
>>> /usr/local/cuda/bin:...

$ echo $CPATH
>>> /usr/local/cuda/include:...
```


然后，就开始各种pip install吧。

PyG项目传送门：
[https://github.com/rusty1s/pytorch_geometric](https://link.zhihu.com/?target=https%3A//github.com/rusty1s/pytorch_geometric)

PyG主页传送门：
[https://rusty1s.github.io/pytorch_geometric/build/html/index.html](https://link.zhihu.com/?target=https%3A//rusty1s.github.io/pytorch_geometric/build/html/index.html)

PyG论文传送门：
[https://arxiv.org/pdf/1903.02428.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1903.02428.pdf)

—**完**—

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤



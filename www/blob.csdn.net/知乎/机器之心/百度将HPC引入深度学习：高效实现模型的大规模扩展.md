# 百度将HPC引入深度学习：高效实现模型的大规模扩展 - 知乎
# 



神经网络在过去几年中规模不断扩大，训练需要大量的数据和计算资源。为了提供所需的计算能力，我们可以使用高性能计算（HPC）中常见的技术将模型扩展到几十个 GPU，但该技术在深度学习中未被充分利用。这项技术，Ring Allreduce，还能减少不同 GPU 之间的通信时间，从而允许将更多时间用在有用计算上。在百度的硅谷人工智能实验室（SVAIL），我们已经成功地使用这些技术训练了当前最先进的语音识别模型。我们很高兴以库和 TensorFlow 软件补丁的形式推出 Ring Allreduce 的实现。我们也希望通过发布这些库可以使深度学习社区更有效地扩展他们的模型。

**引言**


在过去的几年中，神经网络已被证明是解决各种问题的非常有效的工具，并在规模和计算需求上快速增长。在用两个 GPU 运行一周并调节了 6000 万参数之后，用于图像识别的 SuperVision 卷积网络在物体识别方面取得了巨大成功 [1]。在 2016 年，对一个有超过 10 亿个参数的网络在 32 个 GPU 上训练了 3 周之后，研究人员在语言建模方面取得了突破性进展 [2]。在 SVAIL，2014 年我们的 Deep Speech 语音识别系统的第一次迭代约有 1100 万个参数 [5]，而一年后的下一次迭代已经增长到 1 亿个参数 [3]。


随着参数数量以及神经网络计算需求的不断增长，在多节点、多 GPU 上进行高效并行的神经网络训练已经变得越发重要，因为等待几个月时间训练大型网络会减慢试验进程，限制进一步开发。在这篇博文中，我们提出了一种来自高性能计算（HPC）领域的技术，并演示如何将其应用于深度学习以在神经网络训练中取得显著的表现。


**通信问题**


当在多个 GPU 上并行训练一个神经网络，你必须选择如何将不同的运算分布到不同的 GPU 上。本文中，我们将介绍一种被称为数据并行随机梯度下降（data parallel stochastic gradient descent）的技术。在标准随机梯度下降（SGD）中，梯度下降通过使用数据的子集（minibatch）来完成，它们通过进行多次迭代来遍历整个数据集。然而，在数据并行训练中，每个 GPU 都有一个完整的神经网络模型的副本，并且每一次迭代只会被分配 minibatch 样本中的一个子集。对于每次迭代，每个 GPU 在自己处理的数据上将神经网络向前传播，随后再进行误差反向传播（error backpropagation）来计算相对于神经网络参数的损失的梯度。


最后，GPU 通过相互通信来平均不同 GPU 计算的梯度，将平均梯度应用于权重来获取新权重。GPU 在锁步（lock-step）中都进行迭代，并且一旦一个 GPU 完成了自己的迭代，它必须要等待其它所有 GPU 都完成，这样以保证权重可以被适当地更新。这等价于在单块 GPU 上处理 SGD，但是我们通过把数据分配给多个 GPU 来并行运算，从而获得了计算速度的提升。


当你仅仅只有两块 GPU 和数以兆字节（MB）的参数时，这些 GPU 如何通信可能看上去没什么影响。但是，当你的模型有数十亿个参数时，梯度就会占用千兆字（GB）节的空间（因为每个参数都有一个梯度值），并且你还在同时协调几十个 GPU，那么此时 GPU 之间的通信机制就显得非常重要了。


例如，我们考虑一下可能的最直接的通信机制。每个 GPU 都在 minibatch 上的一个子集里计算一个梯度。然后，每个 GPU 都将该子集的梯度发送给同一个 GPU，让这个 GPU 来计算所有梯度的平均值，最后它会将平均值发送回给其它 GPU。


*![](https://pic4.zhimg.com/v2-fa0f7b4badc003eb28fdb9ea000c4607_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='411'></svg>)与单个 reducer GPU 之间的数据传输*


如果存在越多需要被发送的数据，那么发送的时间就越长；每个通信信道都有一个最大吞吐量（带宽）。例如，一个好的网络连接可以提供 15MB/s 的带宽，一个千兆以太网连接能提供 125MB/s 的带宽。搭载在高性能计算集群（HPC cluster）上的专业网络硬件（比如 InfiniBand）可以在结点之间提供高达数 GB/s 的带宽。


在数据于单个 GPU 上传输的直接机制（straight-forward mechanism）中，这个 GPU 必须接收来自所有其它 GPU 的所有参数，并且它还要将所有参数发回给所有 GPU。于是，系统中存在的 GPU 越多，通信成本就越大。


现在，让我们来评估一下这种通信机制在真实模型上的能力，例如，有一个基于百度语音识别系统 Deep Speech 2 开发的语音识别网络 [3]，它有 3 亿个可训练参数，每个参数占 4 字节（Byte），也就是大概 1.2 GB 的数据量。让我们假设你系统上的网络硬件能够支持 1GB/s 的带宽，那也就是说，如此将你的系统在如上所述的两块 GPU 上并行训练，将会让每次迭代都变慢 1.2 秒。如果在 10 个 GPU 上并行训练，将会让每次迭代都变慢 10.8 秒。随着 GPU 数量的增加，处理每次迭代的时间都会线性增长。即便每个迭代会花个几秒钟，这种在通信成本中的快速线性增长也会使接下来的并行处理变得不现实，它大大降低了你训练的效率。


一种替代办法就是放弃训练算法的同步性质，去除所有 GPU 在锁步中遍历梯度下降迭代的约束。然而，尽管这可以使得你模型的并行处理更加简便，去除了这些限制的算法（各种异步随机梯度下降）还是会很难调试，因为有些模型会收敛到欠佳的结果上。不过由于这篇博文意不在此，我们就不在这里考虑它了。


不过，我们可以通过使用来自高性能计算领域的分布式简约算法（distributed reduction algorithms）并利用带宽优化环衰减（bandwidth-optimal ring allreduce）来解决通信问题。


**Ring Allreduce**


上述简单通信策略的主要问题是通信成本随系统中的 GPU 数量线性增长。相反，ring allreduce 是这样一种算法——其通信成本是恒定的，与系统中的 GPU 的数量无关，并且仅由系统中的 GPU 之间的最慢连接来确定。事实上，如果在通信成本上你只考虑带宽这一因素（并忽略延迟），那么 ring allreduce 就是一个最佳的通信算法 [4]（当你的模型较大时，这是一个很好的通信成本估算，你需要在较少的次数内发送大量数据）。


Ring Allreduce 中的 GPU 被布置在一个逻辑环路（logical ring）之中。每个 GPU 左右两个各有一个 GPU，并且只从左边的 GPU 接收数据，再把数据发送至右边的 GPU。


*![](https://pic3.zhimg.com/v2-22f5e1067be77a713b3ef64e1b1a6392_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='745' height='587'></svg>)被布置在逻辑环中的 GPU*



算法的进行分两步：第一步，scatter-reduce；第二步，allgather。在第一步中，GPU 将交换数据，使得每个 GPU 最终都有一个最终结果的数据块。在第二步中，GPU 将交换那些块，使得所有 GPU 最终得到完整的最后结果。


**Scatter-Reduce**


为了简单起见，让我们假设目标是以元素方式求和浮点数的单个大数组的所有元素。在系统中有 N 个 GPU, 其中每个 GPU 有一个相同大小的数组。在 allreduce 的最后，每个 GPU 都应该有一个同样大小的包含了原数组中数值的和的数组。


一开始，GPU 把数组分割成 N 个较小的块（其中 N 是 GPU 在环中的数量）。

![](https://pic3.zhimg.com/v2-87b138323a3db7d16c6f39c3409dd82a_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='832' height='499'></svg>)

接着，GPU 会执行 N-1 次迭代 scatter-reduce。在每一次迭代中，GPU 将发送其中一个块到右边的 GPU，并从左边的 GPU 接收一个块，把数据累积进该块。在每一次迭代中，被发送的块和被接收的块是不同的。第 n 个 GPU 以发送块 n 和接收块 n – 1 开始，并从那儿接着向后运行。每次迭代发送的块即是上次迭代所接收的块。


例如，在第一次迭代中，上图表中的 5 个 GPU 将会发送和接收以下的块：

- 
GPU 发送 接收

- 
0 Chunk 0 Chunk 4

- 
1 Chunk 1 Chunk 0

- 
2 Chunk 2 Chunk 1

- 
3 Chunk 3 Chunk 2

- 
4 Chunk 4 Chunk 3



*![](https://pic3.zhimg.com/v2-b3831bb78546b246115bc7e9f5728a9a_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='856' height='597'></svg>)在 scatter-reduce 的第一次迭代中的数据传输*



在第一次的发送和接收完成之后，每个 GPU 会有一个由两个不同 GPU 中的相同块的总和组成的块。例如，第二个 GPU 上的第一块将是来自第二个 GPU 和第一个 GPU 的那个块中的值的和。


*![](https://pic3.zhimg.com/v2-c5afe045e615cda1b57b7b0693f11b46_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='832' height='499'></svg>)scatter-reduce 的第一次迭代完成之后的中间和*



在下一次迭代中，进程继续，直到最后，每个 GPU 会有一个块包含所有 GPU 中的那块的所有值的和。下面的图像演示了所有的数据传输和中间结果，从第一次迭代开始，一直持续到 scatter-reduce 结束。


*![](https://pic3.zhimg.com/v2-b3831bb78546b246115bc7e9f5728a9a_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='856' height='597'></svg>)scatter-reduce 数据传输（迭代 1）*



![](https://pic1.zhimg.com/v2-50898c65f367d4564d378490cdaf5810_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='850' height='612'></svg>)scatter-reduce 数据传输（迭代 2）



*![](https://pic2.zhimg.com/v2-19119fa77fcd2aa34730e8d2be464c1d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='850' height='597'></svg>)scatter-reduce 数据传输（迭代 3）*



*![](https://pic3.zhimg.com/v2-a7137288dc6e083b6da0fa872d93d262_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='855' height='597'></svg>)scatter-reduce 数据传输（迭代 4）*



*![](https://pic1.zhimg.com/v2-965dd11a8fb21e3fa1f4ee5d0407ff48_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='833' height='469'></svg>)所有 scatter-reduce 传输结束之后的最后状态*



**Allgather**


scatter-reduce 这一步完成之后，每个 GPU 有一个值的数组，其中这些值（每个 GPU 一个块）中的一些是包含来自所有 GPU 贡献的最后值。为了完成 allreduce，GPU 必须交换这些块，从而所有的 GPU 获得所有的必需值。


该环路的 allgather 的执行等同于 scatter-reduce（通过 N-1 次发送和接收的迭代），除了不是累加 GPU 接收的值，而是简单地重写块。第 n 个 GPU 通过发送第 n+1 个块和接收第 n 个块开始，并在未来的迭代中一直发送它刚接收的块。


例如，在我们的 5 个 GPU 设置中的第一次迭代，GPU 会发送和接收以下的块:

- 
GPU 发送 接收

- 
0 Chunk 1 Chunk 0

- 
1 Chunk 2 Chunk 1

- 
2 Chunk 3 Chunk 2

- 
3 Chunk 4 Chunk 3

- 
4 Chunk 0 Chunk 4



*![](https://pic4.zhimg.com/v2-10db2c290c40c02d5cb42b9a26d1f847_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='855' height='601'></svg>)allgather 的第一次迭代中的数据传输*



在首次迭代完成之后，每个 GPU 将有最终数组（final array) 的两个块。


在接下来的迭代中，该进程会继续运行，一直到最后每个 GPU 都会有整个数组的全部累计值。下面的图像演示了从第一次迭代到 allgather 完成的所有数据传输和中间结果。


*![](https://pic4.zhimg.com/v2-10db2c290c40c02d5cb42b9a26d1f847_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='855' height='601'></svg>)Allgather 数据传输（第 1 次迭代）*



*![](https://pic4.zhimg.com/v2-d6c539ba4253dae9698f34b8a7e8b477_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='858' height='602'></svg>)Allgather 数据传输（第 2 次迭代）*



*![](https://pic3.zhimg.com/v2-d0bbfe457331320b46aebe5bcffcdf76_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='854' height='601'></svg>)Allgather 数据传输（第 3 次迭代）*



*![](https://pic2.zhimg.com/v2-412ca3c34dd1bc8a0261e648063b17ed_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='853' height='601'></svg>)Allgather 数据传输（第 4 次迭代）*



*![](https://pic1.zhimg.com/v2-6c6a88c69419251e470b71fb5b573c9c_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='834' height='469'></svg>)所有 allgather 完成后的最终状态*



**Allreduce 通信成本**


回到引言中描述的简单的通信算法，通信成本（communication cost）会随 GPU 的数量而线性增长。allreduce 效果良好的主要原因是情况已经发生了变化。


在我们描述的系统中，每 N 个 GPU 都会因为 scatter-reduce 而接收 N-1 次值，还为 allgather 接收 N-1 次值。每一次，GPU 都会发送 K/N 个值，其中 K 是指数组中值的总数量，这是在不同 GPU 上相加得到的。因此，每个 GPU 的传入和传出数据总量为：

- 
被传输的数据=2(N−1)N⋅K



其独立于 GPU 的数量，这是很关键的。


因为在离散的迭代中，所有的传输都是同时发生的，所以 allreduce 的速度受限于该环路中相邻 GPU 之间的最慢（最低带宽）的连接。为每个 GPU 选择合适的邻居，该算法能在所有 allreduce 上做到带宽最优并且可能是最快的算法（假设相比于带宽，其延迟成本可以忽略不计）[4]。总的来说，如果在一个节点上的所有 GPU 都临近该环路中的其它 GPU，那么该算法可以得到最好的效果；这能最小化网络连接的数量，从而显著增加这种 GPU-GPU 连接的有效带宽。


**将该 Allreduce 应用于深度学习**


ring allreduce 是高性能计算领域内一个众所周知的算法，但在深度学习领域内的应用相对较少。在我们的实验室中，我们已经成功地将这种工具用作我们所有的数据并行训练的基础，让我们可以将训练有效地扩展到十余个 GPU。


为了最小化通信负载，我们可以利用神经网络的结构。在每一次迭代中，每一个 GPU 都会运行前向传播来计算错误，然后再运行反向传播来为该神经网络的每一个参数计算梯度。反向传播是从输出层开始计算梯度，然后向输入层移动，这意味着输出层参数的梯度在更早的层的梯度之前是显著可用的。因为该 allreduce 能一次操作该网络的参数的一个子集，所以我们可以在其输出层参数上开始 allreduce，同时还能计算其它梯度。这样做使得该计算可以和反向传播步骤中的其它计算一起进行，所以可以减少每个 GPU 用于等待通信完成的总时间。


比如，假设有一个类似于 [2] 中的语言模型，但带有大约 3 亿个可学习的参数（因此总梯度大小为 1.2 GB）。使用 allreduce，每个 GPU 必须发送和接收大约 2.4 GB 的数据。使用一种 CUDA-aware MPI 实现（比如 OpenMPI），我们可以使用 GPUDirect RDMA 在 GPU 之间传输数据，带宽大约为 10 GB/s；但是，在我们的集群中的节点之间的连接更慢——Infiniband 提供了大约 6 GB/s 的带宽。因为 Infiniband 连接是这里的限制因素，那么单次迭代就需要大约

- 
每秒 2.4 GB/6.0 GB ≈ 每次迭代 400 ms



因为更深入到网络中的层一开始就有可用的梯度，所以我们可以在整个反向传播通过完成之前就开始进行数据传输，所以其真正的开销可能会少于 400 ms；通信和计算之间的这种重叠可能会随被优化的神经网络的本质而发生改变。


我们实现了之前提到的语言模型，并且在我们从单个 GPU（没有通信开销）扩展到 40 个 GPU 时测试了每次迭代所用的时间。这 40 个 GPU 被布置成了 5 个节点，每个节点 8 个 GPU，它们之间通过 Infiniband 连接。我们以 32 的批大小对该语言模型运行了 300 迭代，然后计算了其每秒所处理的样本的数量。


*![](https://pic3.zhimg.com/v2-031b88cb217253328d0e13d47f0558c2_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='600'></svg>)对于一个有 3 亿个参数的语言模型，每秒所处理的样本的数量会随同时进行同步训练的 GPU 的数量而线性增长。*


如你所见，整个系统的吞吐量会随 GPU 的数量线性扩展；但超过一个特定的点之后，增加更多 GPU 并不会导致每次迭代的显著减速。在 40 个 GPU 上运行该模型的时间是每次迭代 650-700 ms，而在单个 GPU 上该数字为大约 370 ms。因为据我们估计通信大约需要 400 ms，所以我们通过将反向传播和数据传输重叠起来进行而为每次迭代节省了 70-120 ms 的时间。


**总结**


来自于高性能计算领域的 Ring Allreduce 技术让我们可以高效地在跨多设备和多节点的神经网络上对梯度进行平均。通过在训练过程中使用这种带宽优化算法，你可以极大地减少通信负载并扩展到远远更多的设备，同时仍能保留同步随机梯度下降的确定性和可预测的收敛性。该算法并不特定于任何网络架构和深度学习框架，能够为数据并行训练的效率提供显著的和直接的好处，同时其部署实现也是相当直接和容易的。


为了让你能更轻松地使用这些技术，今天我们也发布了一个演示该 allreduce 算法的 C 语言库：，你可以将其嵌入到任何使用 MPI 的应用中。此外，我们也已经将该 allreduce 整合到 TensorFlow 中（可在 tensorflow.contrib.mpi 模块获取文档）。

- 
C 语言 baidu-allreduce：[GitHub - baidu-research/baidu-allreduce](https://link.zhihu.com/?target=https%3A//github.com/baidu-research/baidu-allreduce)

- 
TensorFlow-allreduce：[GitHub - baidu-research/tensorflow-allreduce](https://link.zhihu.com/?target=https%3A//github.com/baidu-research/tensorflow-allreduce)



我们希望其它深度学习框架也能在合适的地方使用类似的技术；通过使用这些工具，你可以轻松有效地将你的神经网络模型扩展到许多机器，而且不论你选择的是什么框架。


**参考文献**


*1.Krizhevsky, Alex, Ilya Sutskever, and Geoffrey E. Hinton.「ImageNet classification with deep convolutional neural networks.」Advances in neural information processing systems. 2012.*

*2.Jozefowicz, Rafal, et al.「Exploring the limits of language modeling.」arXiv preprint arXiv:1602.02410 (2016).*

*3.Amodei, Dario, et al.「Deep speech 2: End-to-end speech recognition in english and mandarin.」arXiv preprint arXiv:1512.02595 (2015).*

*4.Patarasuk, Pitch, and Xin Yuan.「Bandwidth optimal all-reduce algorithms for clusters of workstations.」Journal of Parallel and Distributed Computing 69.2 (2009): 117-124.*

*5.Hannun, Awni, et al.「Deep speech: Scaling up end-to-end speech recognition.」arXiv preprint arXiv:1412.5567 (2014).*

选自[Baidu Research](https://link.zhihu.com/?target=http%3A//research.baidu.com/bringing-hpc-techniques-deep-learning/)**机器之心编译**



# 声学模型学习笔记（四） dnn speedup - xmdxcsj的专栏 - CSDN博客





2016年10月08日 19:56:00[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1213








## training

### minibatch

一般设为256-1024.太小会导致更新参数频繁，降低GPU的计算效率；太大会导致训练需要更多的训练迭代数。

### piplined bp

将模型按层分割，然后分配到不同的GPU机器上面，实现训练的并行。例如下图 
![这里写图片描述](https://img-blog.csdn.net/20161008195535481)

一帧数据依次经过三个GPU的前向计算，然后在依次进行后向传播。图中的时刻： 

- GPU1处理n的前向计算和n-5的bp计算 

- GPU2处理n-1的前向计算和n-4的bp计算 

- GPU3处理n-2的前向计算和n-3的bp计算
考虑到参数更新的时延性，GPU上面先进行后向更新，在进行前向计算。参数更新越在lower layer，时延越长，最后一层没有时延。 

如果GPU的数量少于层数，可以将多层分到一个GPU，此时需要考虑到计算量的balance问题，因为一般最后一层（输出维度较高）计算量比其它层要大。

### Asynchronous SGD

首先在CPU集群上面提出的 

- parameter server pool（多个机器，每个机器有一部分的模型参数）作为master，master将模型参数发送给slaves 

- 多个slave，每个slave包含多个机器（每个机器有一部分的模型参数，一个slave包含完整的模型数据），每个slave对应训练集的一个subset，计算minibatch的梯度然后发送给master
ASGD也可以用在GPU机器上面，主机上的CPU作为master，GPU作为slave。由于GPU的使用以及GPU和CPU之间通信（PCIe bus）的优势，这种训练方式比CPU集群高效。同时GPU和CPU之间的通信也会成为瓶颈，所以GPU上面可以累计多个minibatch和CPU通信一次，此时需要降低学习率。

### ALMs

对于多GPU（大于4）的情况，由于延时更新的问题，ASGD或者pipelined BP都有一定的局限性，可以考虑使用ALMs方法（Augmented Lagrangian Methods）。

### reduce model size

使用low-rank factorization降低模型参数，例如SVD。

## decoding

### parallel computation
- CPU支持SIMD指令级并行，SSE2/SSSE3/SSE4
- batch处理，一次输入多帧特征，向量*矩阵可以转化为矩阵*矩阵，获得更好的优化
- lazy evaluation，每一帧数据的25%-35%的输出会有用，所以可以lazy计算最后一层的输出。

### sparse network

实际DNN的参数中，有很大一部分的数值很小，比如有人统计70%的权重小于0.1。所以可以将这些权重小的边砍掉，获得一个稀疏网络。 

网络被稀疏以后还需要重新进行训练

### low-rank approximation

对于lower layer，使用low-rank对结果影响比较大。同样，参数矩阵做过SVD以后，还需要重新训练。 

实验表示，保留30%的模型大小，基本对性能没有影响。

### teach small DNN with large DNN

使用large DNN的输出作为标注，训练small DNN，以期望small DNN和large DNN的输出一致，达到同样的性能。

### multiframe DNN

解码的时候计算dnn得分可以复制使用上一帧的得分，以减少计算，称为frame-asynchronous DNN。 

还有一种方式称为MFDNN（multiframe DNN），不再是简单的复制得分，而是一个DNN对应多个输出的softmax输出层，这样就可以减少隐层的计算，一帧的输入，一次前向就能得到多帧的输出结果。 

MFDNN的性能比asynchronous DNN好。
## 参考

《automatic speech recognition a deep learning approach》 chapter 7







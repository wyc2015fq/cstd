# 【python】pytorch中如何使用DataLoader对数据集进行批处理 - zkq_1986的博客 - CSDN博客





2018年12月25日 15:53:36[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1077








第一步：

我们要创建torch能够识别的数据集类型（pytorch中也有很多现成的数据集类型，以后再说）。

首先我们建立两个向量X和Y，一个作为输入的数据，一个作为正确的结果：

![](https://img-blog.csdnimg.cn/20181225155337136)

随后我们需要把X和Y组成一个完整的数据集，并转化为pytorch能识别的数据集类型：

![](https://img-blog.csdnimg.cn/20181225155337152)

我们来看一下这些数据的数据类型：

![](https://img-blog.csdnimg.cn/20181225155337172)

可以看出我们把X和Y通过Data.TensorDataset() 这个函数拼装成了一个数据集，数据集的类型是【TensorDataset】。

好了，第一步结束了





第二步：

就是把上一步做成的数据集放入Data.DataLoader中，可以生成一个迭代器，从而我们可以方便的进行批处理。

![](https://img-blog.csdnimg.cn/20181225155337190)

DataLoader中也有很多其他参数：

![复制代码](https://img-blog.csdnimg.cn/20181225155337206)

```
dataset：Dataset类型，从其中加载数据 
batch_size：int，可选。每个batch加载多少样本 
shuffle：bool，可选。为True时表示每个epoch都对数据进行洗牌 
sampler：Sampler，可选。从数据集中采样样本的方法。 
num_workers：int，可选。加载数据时使用多少子进程。默认值为0，表示在主进程中加载数据。 
collate_fn：callable，可选。 
pin_memory：bool，可选 
drop_last：bool，可选。True表示如果最后剩下不完全的batch,丢弃。False表示不丢弃。
```

![复制代码](https://img-blog.csdnimg.cn/20181225155337206)

好了，第二步结束了，





第三步：

好啦，现在我们就可以愉快的用我们上面定义好的迭代器进行训练啦。

在这里我们利用print来模拟我们的训练过程，即我们在这里对搭建好的网络进行喂入。

![](https://img-blog.csdnimg.cn/20181225155337240)

输出的结果是：

![](https://img-blog.csdnimg.cn/20181225155337266)

可以看到，我们一共训练了所有的数据训练了5次。数据中一共10组，我们设置的mini-batch是3，即每一次我们训练网络的时候喂入3组数据，到了最后一次我们只有1组数据了，比mini-batch小，我们就仅输出这一个。

此外，还可以利用python中的enumerate()，是对所有可以迭代的数据类型（含有很多东西的list等等）进行取操作的函数，用法如下：

![](https://img-blog.csdnimg.cn/20181225155337301)



好啦，结束。

转载自：[https://www.cnblogs.com/JeasonIsCoding/p/10168753.html](https://www.cnblogs.com/JeasonIsCoding/p/10168753.html)




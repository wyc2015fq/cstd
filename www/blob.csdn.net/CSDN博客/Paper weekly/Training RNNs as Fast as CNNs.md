
# Training RNNs as Fast as CNNs - Paper weekly - CSDN博客


2017年09月18日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：443



**本期论文**


**Training RNNs as Fast as CNNs**

本文通过有意简化状态计算并展现更多的并行性而提出了一个替代性的 RNN 实现，这一循环单元的运算和卷积层一样快，并且比 cuDNN 优化的 LSTM 快 5-10x。该实现在诸如分类、问题回答、语言建模上证明了其有效性，并已在 PyTorch 和 CNTK1 中开源。

**论文链接：**https://arxiv.org/abs/1709.02755

**社区讨论精选**

❶

**soloice**

几个观点：

1. 不就是先前 Quasi-RNN 的工作换了个壳……只不过这篇文章里实验做的比较多，更详实一些。但是实验部分只和标准的 LSTM 等结构作比较，没有和 Quasi-RNN 比，固然可能是因为实现一下 Quasi-RNN 工作量太大，但我还是感觉有些避重就轻了，避开了最相关的前人工作。

2. 训练速度提升没有疑问，解除了各个 gate 的时序依赖，但是我谨怀疑推断速度会变慢。在机器翻译那个任务里，LSTM 只要两三层，它的方法要堆六到十层才能获得同样的效果；因为在解码时只能一步一步解码，解除时序依赖不会带来加速（在某一步的时候还不知道未来会输入啥呢），更多的层数可能会带来 latency 变大。

❷

**countback**

论文从实验的角度上来说，确实提供了新的思路，但是论文中提出的新的 SRU implementation，实际上在 16 年 6 月份开始陆陆续续有很多论文在做类似的事情，模型设计上并没有什么新鲜感。

随便找了几篇论文展示类似的idea：
Quasi-RNN: https://arxiv.org/pdf/1611.01576.pdf
PRU模型：https://arxiv.org/pdf/1611.06530.pdf
MGU模型: https://arxiv.org/pdf/1603.09420.pdf

❸

**liujshi**

下图是 GRU 的计算单元，GRU 每一步计算都会引入上一步的隐藏变量：

![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk6YN2XLNnzoKgyE1GaSXmGAH5B5B4yiaXMJwNCWDdWOMuvHRv3hmcPicwTs5NZBYBZQ55icNmtcnh3A/0.png)

下图是 SRU 的计算单元，SRU 计算过程对依赖进行了解耦，使得前三部计算不需要依赖前面的过程，前三部就可以用矩阵拼接的方法对整个句子进行全局计算，起到了加速左右，而后面两个方程仅仅虽然依赖前向过程，但是涉及到的计算仅仅是矩阵的按位乘法，速度很快。

![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk6YN2XLNnzoKgyE1GaSXmGV67mN7sHbw9SVrqicOsRrRTcewGibCiboIicj4jqd8kwpkKSud1nG8evEA/0.png)

但是 SRU 显然对 C~t-1~ 的信息利用很低，所以导致其效果并不好，需要多叠堆层数才能有 GRU 一样的效果。

❹
**dalegebit**

有个疑问，虽然解除了 h_t 的相关性，减少了那些矩阵乘法，但是 c_t 的还是没解除，ct 还是需要等待 c{t-1} 生成之后才能生成，还是有时间上的依赖。而且如果没有时间上的依赖那也不叫 RNN 了。

**Soloice：**对的。后面的时间步当然还要等，只是减少了等待时间。本来是 gate 和 hidden state 都要等前面的时间步算完，现在只有 hidden state 需要等，各个时间步的 gate 可以同步算。但是，这种加速技巧是 Quasi-RNN 提出来的啊，这篇文章把 Quasi-RNN 的 recurrent unit 还简化了一下，不过把输出搞复杂了一点（输出 = hidden 和 input 的加权，相当于 highway network）。

**参与讨论**

点击**「阅读原文」**即可申请注册 PaperWeekly 社区，快来发现你感兴趣的论文吧。

**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。
![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmUEtia3RCJ5eZHIskGRIYc1Uen4885tjUqeiaS963f1BQ5PdHHibppPMuHbfbpVsbicYsU00j6RwOA2w/640?)



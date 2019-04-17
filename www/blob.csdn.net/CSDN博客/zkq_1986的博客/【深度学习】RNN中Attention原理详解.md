# 【深度学习】RNN中Attention原理详解 - zkq_1986的博客 - CSDN博客





2018年06月08日 14:23:38[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：554








### Attention原理

在输出序列中，当前的输出与当前状态有关。

当前状态与前一步状态、前一步输出和当前所看到的上下文有关

当前所看到的上下文来自其所看到的输入的信息(对输入加工得到有用信息)的加权，这个加权就是所谓的Attention机制。也就是说之前所看到的信息需要区别对待。

输入信息的加权值，通过当前输入信息与前一步状态通过softmax计算得到。这个加权值的计算也是需要通过神经网络训练出来，因为它所属的分布很难得知，通过神经网络，可以避开这个问题。

![](https://img-blog.csdn.net/20180608142321373?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180608142525814?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


参考文献：Bahdanau D, Cho K, Bengio Y. Neural Machine Translation by Jointly Learning to Align and Translate[J]. Computer Science, 2014.




# 英特尔宣布推出“Nervana”神经网络处理器 - 人工智能学家 - CSDN博客
2017年10月25日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：175
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBV71Cqibib4fRh5MK70xH2SLISxh6NjZsbLDhaVBKLUm1M96xkaEI242KaVPyiaLQY9eibfQAcpUOWX3A/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
原作者：Ryan Whitwam
译者：彭婷
概要：Nervana
 NNP就是基于这种计算设计的。这也是所谓的专用集成电路（ASIC），所以它无法应用于一般的计算任务。
科幻小说家和现代科技大型公司一致认为AI可以开辟出一条康庄大道。因而无论是Google的员工还是Facebook的员工，每个人都致力于设计人工神经网络以解决譬如计算机视觉和语音合成等大问题。其中，大多数项目使用的是现有的计算机硬件，但英特尔最近却在筹划一件大事。该芯片制造商宣布要推出首款专用神经网络处理器，即英特尔Nervana神经网络处理器（NNP）。
他们的设计目的在于，神经网络能够以一种更像大脑的方式处理数据并解决问题。神经网络是由人造神经元层组成，而人造神经元层负责处理输入，并将数据沿线下传到网络中的下一个神经元。最后，在网络应用的转换与通知下，你就得到了输出，这比强力计算效率更高。 这些系统可以通过大量的数据培训随时随地学习。 也正是通过这种方式，Google改进AlphaGo，成功打败了世界上最厉害的围棋选手。
Nervana NNP就是基于这种计算设计的。这也是所谓的专用集成电路（ASIC），所以它无法应用于一般的计算任务。然而，如果你以此运行或训练神经网络的话，Nervana可能比现有硬件快许多倍。因为它擅长于神经网络中的矩阵乘法，卷积和其他数学运算。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBV71Cqibib4fRh5MK70xH2SLIk2rE1krpmnianvK2Z5N5MNicN3YRKRhsfbVY7k2PQyexJ5KedkLNNXFw/0?wx_fmt=png)
有趣的是，就像你在CPU上看到的一样，芯片上无法找到的缓存。 相反，Nervana使用的是一个软件定义的内存管理系统，该系统可以根据神经网络的需要调整性能。此外，英特尔还实现了自己的数字格式，并将其称之为Flexpoint。 它不如常规整数数学精确，但英特尔说这对神经网络没有影响。噪音对它来说，毫无作用，在某些情况下，数据中的噪音甚至有助于训练神经元。较低的精度还有助于芯片进行并行计算，因而整个网络的带宽会更高，而延迟会降低。
英特尔并不是唯一一个致力于加速神经网络的公司。Google已开发了名为Tensor Processing Units的云计算芯片，Nvidia正在推动其GPU以解决神经网络处理问题。Facebook也已与英特尔合作，并为其硬件设计做出了贡献。英特尔表示，NervanaNNP将于2017年年底之前出售。
本文系网易新闻 · 网易号“各有态度”特色内容。

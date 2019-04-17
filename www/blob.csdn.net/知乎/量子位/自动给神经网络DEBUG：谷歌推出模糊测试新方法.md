# 自动给神经网络DEBUG：谷歌推出模糊测试新方法 - 知乎
# 



> 夏乙 发自 凹非寺
量子位 报道 | 公众号 QbitAI




DEBUG，是程序员永无止境的日常。给神经网络捉虫，更是比普通程序难得多：

绝大部分bug都不会导致神经网络崩溃、报错，只能让它训练了没效果，默默地不收敛。

能不能把炼丹师们从无休止无希望的debug工作中拯救出来？

两位谷歌大脑研究员Augustus Odena和Ian Goodfellow说，好像能。
![](https://pic4.zhimg.com/v2-e7fc1f97b40fb4fe06bcbece4a800f6f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='400'></svg>)
他们推出了一种自动为神经网络做软件测试的方法：**TensorFuzz**。它非常擅长自动发现那些只有少数某些输入会引发的错误。

比如说，它能在已训练的神经网络里发现数值误差，生成神经网络和其量化版本之间的分歧，发现字符级语言模型中的不良行为。

这种方法说新也不新，它来源于传统编程技艺中的一种测试手段：覆盖引导的模糊测试，英语叫coverage-guided fuzzing，简称CGF。

模糊测试，就是通过向目标系统提供非预期的输入并监视异常结果，来发现软件漏洞。也就是说，用随机坏数据（也称做 fuzz）攻击一个程序，然后等着观察哪里遭到了破坏。

而所谓“覆盖引导”的模糊测试，是指在进行模糊测试时，尽量最大化程序的代码覆盖率，测试尽可能多的代码分支。

AFL、libFuzzer都是比较常见的模糊测试工具。

当然传统的方法，不能直接用于神经网络。把CGF搬到神经网络上，不能像在传统的测试中那样简单地去计算分支覆盖率。为此，深度学习的研究者们提出了不少计算覆盖率的新方法，来计算神经元覆盖率、神经元边界覆盖率等等。

两位谷歌大牛提出，应该存储与输入相关联的激活，通过用近似最近邻算法来检查这些输入有没有导致覆盖率的增加，

粗略地说，TensorFuzz是通过查看计算图的“激活”来测量覆盖率。整体架构如下图所示，左边是模糊测试的程序图，标明了数据如何流动；右边以算法形式描述了模糊测试过程的主循环。
![](https://pic1.zhimg.com/v2-0719b300b084ede18fb24c6fe88aa960_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='562'></svg>)
说一下实验结果。

1、CGF可以在训练好的神经网络中有效发现数值误差

由于神经网络使用浮点数学，因此无论是在训练还是评估时，都很容易受到数值问题的影像。众所周知，这些问题很难调试。而CGF可以帮助我们专注于找到导致NaN值的输入。

2、CGF揭示了模型和其量化版本之间的分歧

量化是一种存储神经网络权重的方法，本质上是使用较少的数值表示来执行神经网络的计算。量化是降低神经网络计算成本或尺寸的流行方法。但是一定要避免量化之后，显著降低模型的准确性。

CGF可以在数据周围的小区域内快速找到许多错误，在测试的案例中，70%的情况下模糊器能够产生分歧。另外，在给定相同突变的情况下，随机搜索没有找到新的错误。

3、CGF揭示了字符级语言模型中的不良行为

作者运行了TensorFuzz和随机搜索进行测试，测试目的有两个，一是模型不应该连续多次重复相同的词，而是不应该输出黑名单上的词。

测试进行24小时后，TensorFuzz和随机搜索都生成了连续重复的词。此外，TensorFuzz生成了十个黑名单词汇中的六个，而随机搜索只有一个。

大概就酱。

总之，作者为神经网络引入CGF的概念，并且在论文中描述了如何实现。上面的三个实验结果，证明了TensorFuzz的实际适用性。
![](https://pic2.zhimg.com/v2-9ed17a049742a8bd4367b0c96b5e362d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='711'></svg>)
Goodfellow说，他希望TensorFuzz能成为包括机器学习在内复杂软件的基础回归测试。例如在新版本发布前，模糊搜索新旧版本之间的差异。

论文传送门在此：
[Debugging Neural Networks with Coverage-Guided Fuzzing​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1807.10875)


![](https://pic1.zhimg.com/v2-5fd4df4a2489ac4afcc15afb4447b2d4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='381'></svg>)
Abstract：

Machine learning models are notoriously difficult to interpret and debug. This is particularly true of neural networks. In this work, we introduce automated software testing techniques for neural networks that are well-suited to discovering errors which occur only for rare inputs. Specifically, we develop coverage-guided fuzzing (CGF) methods for neural networks. In CGF, random mutations of inputs to a neural network are guided by a coverage metric toward the goal of satisfying user-specified constraints. We describe how fast approximate nearest neighbor algorithms can provide this coverage metric. We then discuss the application of CGF to the following goals: finding numerical errors in trained neural networks, generating disagreements between neural networks and quantized versions of those networks, and surfacing undesirable behavior in character level language models. Finally, we release an open source library called TensorFuzz that implements the described techniques.

作者还说了，TensorFuzz是一个开源库。 不过嘛，目前两位研究人员还没放出地址，为期应该不远了~

— **完** —
欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)
诚挚招聘
量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。
[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者
վ'ᴗ' ի 追踪AI技术和产品新动态



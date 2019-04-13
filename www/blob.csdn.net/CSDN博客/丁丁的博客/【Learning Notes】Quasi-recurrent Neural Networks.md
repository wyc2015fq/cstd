
# 【Learning Notes】Quasi-recurrent Neural Networks - 丁丁的博客 - CSDN博客


2017年09月12日 20:02:14[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：2719


[QRNN](https://arxiv.org/abs/1611.01576)[1] 是 Salesforce Research 团队（Update:[官方pytorch实现](https://github.com/salesforce/pytorch-qrnn)）提出的一种使用卷积操作替代传统的循环结构（vanilla RNN, LSTM, GRU）的新网络结构。QRNN 可以视为介于 RNN 和 CNN 之间的特殊结构。由于卷积操作没有循环结构时间上的依赖性，因此，QRNN 的计算并行度高；在训练时，卷积结构也要比循环结构更稳定。因此 ，QRNN 是一种潜在有用的网络，可以 drop-in 地替代各种 RNN。
笔者之前基于 Keras 写一个原型实现（见[github](https://github.com/DingKe/nn_playground/tree/master/qrnn)）。但不论学界还是工业界都对 QRNN 关注不多（大家倒是对 CNN 替代 LSTM/GRU 非常上心），最近发现一些比较详细的正向结果 [4]，因此，笔者觉得有必要更认真的对待 QRNN。
# 1. 动机
RNN 在计算时，有时间的依赖性，并行度受限；而 CNN 受制于有限的 receptive field，因此，信息传递太慢。QRNN 希望能综合RNN和CNN的优点，尽量避免各自的缺陷。
# 2. 算法
仿照 LSTM 的计算公式，QRNN 的各个门（gate）的计算公式如下：

$$
\mathbf{Z} = \tanh(\mathbf{W}_z * \mathbf{X})
$$

$$
\mathbf{F} = \sigma(\mathbf{W}_f * \mathbf{X})
$$

$$
\mathbf{O} = \sigma(\mathbf{W}_o * \mathbf{X})
$$
其中$*$表示1维卷积操作。显然，这一步操作是没有时序上的依赖的。
为建模时序关系，不同时刻 memory 更新如下：

$$
\mathbf{c}_t = \mathbf{f}_t \odot  \mathbf{c}_{t-1} + (1 - \mathbf{f}_t) \odot \mathbf{z}_t
$$

$$
\mathbf{h}_t = \mathbf{o}_t \odot  \mathbf{c}_{t}
$$
这一步是有时序依赖的，但是计算量较小，只用到 element-wise的操作。
此外，文章还提出使用 dropout 的一种方案：

$$
\mathbf{F} = 1 - dropout(1 - \sigma(\mathbf{W}_f * \mathbf{X}))
$$

# 3. 性能及加速
1维卷积在实现上，可以转化为普通的矩阵运算。而 QRNN 在运算量最大矩阵乘操作上不存在时序依赖，因此，良好实现的 QRNN 相对于 LSTM/GRU 一定是有速度上的优势。具体加速情况可以参考论文，特别是[5]，使用 cuDNN 的 LSTM 作为基线，加速效果是有实际参考意义的。
另一方面，QRNN 主体是卷积操作，因此，相对于循环理应更容易训练。
## 3.1 论文
原文[1]在三个任务上实验了QRNN：
情感分类（序列分类）
Penn Treebank 上语言模型（many-to-many）
基于 character 的机器翻译（seq2seq with attention）
在这些序列任务上，在有加速收益的前提下，都取得了相当的性能表现。
## 3.2 第三方结果
目前，follow QRNN 的工作报告的不多。
### 韵律预测
文章 [2，3] 利用 QRNN 做 TTS 系统前端的韵律预测，这是一个序列标注问题。
### SRU（Simple Recurrent Unit）
文章[4] 提出 SRU（Simple Recurrent Unit）。这实质上是 QRNN window 宽度取1的特殊情况（即使用普通的线性变换）。不同的是，SRU 将 highway 连接作为模型的一部分（QRNNs当然可以加残差或 highway 连接）。文章实验非常充分：
在六个不同数据上实验了序列分类问题；
在 SQuAD 数据集上实验了问题任务（序列生成问题）；
在 Penn Treebank 上实验了语言模型；
在 WMT’14 English→German 任务上实验了神经机器翻译；
在 switchboard 上实验了语音识别任务。
虽然实验结果并不算非常公平（SRU自带了 highway 连接），但使用SRU的模型确实都取得了非常好的性能。
# 讨论
在使用卷积结构替代循环结构上，QRNN 没有纯 CNN 模型（e.g. WaveNet）那么激进，依然保留了一些循环结构。个人经验发现，使用纯 CNN 模型超过 RNN 并不是一件容易的事情。QRNN 则可以逐层替代原有的 RNN，使用起来门槛要低很多，而且加速效果有保证。虽然目前应用案例不多，还是可以期待一下。
---

# References
Bradbury et al.[Quasi-recurrent Neural Networks](https://arxiv.org/abs/1611.01576)， ICLR‘17.
Arik et al.[Deep Voice: Real-time Neural Text-to-Speech](https://arxiv.org/abs/1702.07825).
Arik et al.[Deep Voice 2: Multi-Speaker Neural Text-to-Speech](https://arxiv.org/abs/1705.08947).
Lei et al.[Training RNNs as Fast as CNNs](https://arxiv.org/abs/1709.02755).


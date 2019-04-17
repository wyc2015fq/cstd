# ICASSP 2018 | 阿里巴巴论文提出Advanced LSTM：关于更优时间依赖性刻画在情感识别方面的应用 - 知乎
# 



选自arXiv，作者：陶菲/Fei Tao、刘刚/Gang Liu，机器之心编译。




**论文：《高级长短期记忆网络：关于更优时间依赖性刻画在情感识别方面的应用》（Advanced LSTM: A Study about Better Time Dependency Modeling in Emotion Recognition）**
![](https://pic2.zhimg.com/v2-c4ae7410dfe5e070843b3585c971711d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='241'></svg>)
论文地址：[https://arxiv.org/pdf/1710.10197.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1710.10197.pdf)




摘要：长短期记忆网络（LSTM）隐含了这样一个假设，本层的现时状态依赖于前一时刻的状态。这种「一步」的时间依赖性，可能会限制 LSTM 对于序列信号动态特性的建模。在这篇论文里，针对这样的一个问题，我们提出了高级长短期记忆网络（advanced LSTM (A-LSTM)），利用线性组合，将若干时间点的本层状态都结合起来，以打破传统 LSTM 的这种局限性。在这篇文章中，我们将 A-LSTM 应用于情感识别中。实验结果显示，与应用传统 LSTM 的系统相比，应用了 A-LSTM 的系统能相对提高 5.5% 的识别率。




**研究背景**

LSTM 现在被广泛的应用在 RNN 中。它促进了 RNN 在对序列信号建模的应用当中。LSTM 有两个输入，一个来源于前一层，还有一个来源于本层的前一个时刻。因此，LSTM 隐含了这样一个假设，本层的现时状态依赖于前一时刻的状态。这种「一步」的时间依赖性，可能会限制 LSTM 对于序列信号动态特性的建模（尤其对一些时间依赖性在时间轴上跨度比较大的任务）。在这篇论文里，针对这样的一个问题，我们提出了 advanced LSTM (A-LSTM)，以期打破传统 LSTM 的这种局限性。A-LSTM 利用线性组合，将若干时间点的本层状态都结合起来，因此不仅可以看到」一步「以前的状态，还可以看到更远以前的历史状态。

在这篇文章中，我们把 A-LSTM 应用到整句话层级（utterance level) 上的情感识别任务中。传统的情感识别依赖于在整句话上提取底端特征（low level descriptors) 的统计数据，比如平均值，方差等等。由于实际应用中，整句话中可能会有一些长静音，或者是一些非语音的声音，这种统计数据就可能不准确。在这篇论文中，我们使用基于注意力模型（attention model) 的加权池化 (weighted pooling) 递归神经网络 (recurrent neural network) 来更有效的提取整句话层级上的特征。




**高级长短期记忆网络**

A-LSTM 利用线性组合，将若干时间点的本层状态都结合起来。这其中的线性组合是利用与注意力模型 (attention model) 类似的机制进行计算的。具体公式如下：
![](https://pic1.zhimg.com/v2-40916f11d01ada6ac210dc8c558bf844_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='534' height='154'></svg>)
Fig 1 中 C'(t) 即为前面若干时间状态的线性组合。这个线性组合以后的时间状态将被输入下一时间点进行更新。可以想象，每次的更新都不只是针对前一时刻，而是对若干时刻的组合进行更新。由于这种组合的权重是有注意力模型控制，A-LSTM 可以通过学习来自动调节各时间点之间的权重占比。如果依赖性在时间跨度上比较大，则更远以前的历史状态可能会占相对大的比重；反之，比较近的历史状态会占相对大的比重。
![](https://pic4.zhimg.com/v2-27bd0cd0ac0d17aeb0abd24f78835b3f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='998' height='382'></svg>)Fig 1 The unrolled A-LSTM
**加权池化递归神经网络**
![](https://pic3.zhimg.com/v2-9220aa89a6da23c8f8a2849381a08c9e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1004' height='496'></svg>)Fig 2 The attention based weighted pooling RNN
在这篇论文中，我们使用基于注意力模型的加权池化递归神经网络来进行情感识别（见 Fig 2)。这一神经网络的输入是序列声学信号。利用注意力模型，我们的神经网络可以自动调整各个时间点上的权重，然后将各个时间点上的输出进行加权平均（加权池化）。加权平均的结果是一个能够表征这一整串序列的表达。由于注意力模型的存在，这一表达的提取可以包含有效信息，规避无用信息（比如输入序列中中的一些长时间的静音部分）。这就比简单的计算一整个序列的统计数值要更好（比如有 opensmile 提取的一些底端特征）。为了更好的训练模型，我们在情感识别任务之外还添加了两个辅助任务，说话人识别和性别识别。我们在这个模型当中使用了 A-LSTM 来提升系统性能。




**实验**

在实验阶段，我们使用 IEMOCAP 数据集中的四类数据（高兴，愤怒，悲伤和普通）。这其中一共有 4490 句语音文件。我们随机选取 1 位男性和 1 位女性说话人的数据作为测试数据。其余的数据用来训练（其中的 10% 的数据用来做验证数据）。我们采用三个衡量指标，分别为无权重平均 F-score（MAF），无权重平均精密度（MAP），以及准确率（accuracy)。

我们提取了 MECC, 信号过零率（zero crossing rate), 能量，能量熵，频谱矩心 (spectral centroid)，频谱流量 (spectral flux)，频谱滚边（spectral rolloff),12 维彩度向量（chroma vector), 色度偏差 (chroma deviation), 谐波比（harmonic ratior) 以及语音基频，一共 36 维特征。对这些序列特征进行整句话层级上的归一化后，将其送入系统进行训练或测试。

在这个实验中，我们的系统有两层神经元层，第一层位全连接层（fully connected layer)，共有 256 个精馏线性神经元组成（rectified linear unit)。第二层位双向长短期记忆网络（bidirectional LSTM (BLST))。两个方向一共有 256 个神经元。之后即为基于注意力模型的加权池化层。最上方为三个柔性最大值传输函数层，分别对应三个任务。我们给三个任务分配了不同的权重，其中情感识别权重为 1，说话人识别权重为 0.3，性别识别为 0.6。如果是应用 A-LSTM，我们就将第二层的 BLSTM 替换成双向的 A-LSTM，其他的所有参数都不变。这里的 A-LSTM 选取三个时间点的状态作线性组合，分别为 5 个时间点前（t-5)，3 个时间点前 (t-3)，以及 1 个时间点前 (t-1)。实验结果如下：
![](https://pic3.zhimg.com/v2-d871ed544fb614989f9d4f4ccd08d796_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='696' height='212'></svg>)
其中的 mean LSTM 与 A-LSTM 比较类似，唯一区别是，当我们为选取的几个时间点的状态作线性组合的时候，不是采用注意力模型，而是简单的做算术平均。




**结论**

与应用传统 LSTM 的系统相比，应用了 A-LSTM 的系统显示出了更好的识别率。由于加权池化过程是将所有时间点上的输出进行加权平均，因此系统性能的提升只可能是来源于 A-LSTM 更加灵活的时间依赖性模型，而非其他因素，例如高层看到更多时间点等等。并且，这一提升的代价只会增加了数百个参数。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)






# RNN--长短期记忆(Long Short Term Memory, LSTM) - wydbyxr的博客 - CSDN博客
2018年10月31日 10:33:43[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：353
个人分类：[RNN](https://blog.csdn.net/wydbyxr/article/category/8286658)
# 长短期记忆(Long Short Term Memory, LSTM)
  是一种 RNN 特殊的类型，可以学习长期依赖信息。记住长期的信息在实践中是 LSTM 的默认行为，而非需要付出很大代价才能获得的能力！
## LSTM 单元和普通 RNN 单元的区别
  在标准的 RNN 中，这个重复的模块只有一个非常简单的结构，例如一个 tanh 层。LSTM 同样是这样的结构，但是重复的模块拥有一个不同的结构，重复模块包含四个交互的gate。
  主要大的区别是，采用一个叫 “细胞状态（state）” 的通道贯穿了整个时间序列。通过精心设计的称作 “门” 的结构来去除或增加信息到细胞状态的能力。
## 组成部分
  1）" 忘记门”；
  2）“输入门” 的打开关闭也是由当前输入和上一个时间点的输出决定的。
  3）“输出门”，控制输出多少，最终仅仅会输出确定输出的那部分。
  4）状态门：让几个 “门” 的输入数据除了正常的输入数据和上一个时刻的输出以外，再接受 “细胞状态” 的输入。
## 各种各样的LSTM
  为了得到更好的建模能力，一种流行的做法是将 LSTM 层堆叠起来。但带有太多 vanilla LSTM 层的 LSTM-RNN 非常难以训练，而且如果网络太深，还会有梯度消失问题。这个问题可以使用 highway LSTM 或 residual LSTM 解决。
  1）在 highway LSTM中，相邻层的记忆单元通过门控的直接链路连接在一起，这为信息提供了一种在层之间更直接且不衰减地流动的路径。
  2）residual LSTM在 LSTM 层之间使用了捷径连接（shortcut connection），因此也提供了一种缓解梯度消失问题的方法。
  3）二维时频 LSTM（2-D, time-frequency (TF) LSTM），可以在时间和频率轴上对语音输入进行联合扫描，以对频谱时间扭曲（spectro-temporal warping）建模，然后再将其输出的激活（activation）用作传统的时间 LSTM 的输入。这种时间-频率联合建模能为上层的时间 LSTM 提供更好的规范化的特征。
  4）网格 LSTM（Grid LSTM）是一种将 LSTM 记忆单元排布成多维网格的通用 LSTM，可以被看作是一种将 LSTM 用于时间、频谱和空间计算的统一方法。
  5）尽管双向 LSTM（BLSTM）：通过使用过去和未来的语境信息能得到比单向 LSTM 更好的表现，但它们并不适合实时系统，因为这需要在观察到整个话语之后才能进行识别。因为这个原因，延迟受控 BLSTM（LC-BLSTM）[29] 和行卷积 BLSTM（RC-BLSTM）等模型被提了出来，这些模型构建了单向 LSTM 和 BLSTM 之间的桥梁。在这些模型中，前向 LSTM 还是保持原样。但反向 LSTM 会被替代——要么被带有最多 N 帧前瞻量的反向 LSTM（如 LC-BLSTM 的情况）替代，要么被集成了 N 帧前瞻量中的信息的行卷积替代。

# RNN--GRU，Gated Recurrent Unit - wydbyxr的博客 - CSDN博客
2018年11月01日 10:38:31[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：140
# GRU，Gated Recurrent Unit
  Gated Recurrent Unit (GRU)，这是由 Cho, et al. (2014) 提出。它将忘记门和输入门合成了一个单一的更新门(故它只有3个门)。同样还混合了细胞状态和隐藏状态，和其他一些改动。最终的模型比标准的 LSTM 模型要简单，也是非常流行的变体。
  GRU是一般的RNNs的改良版本，主要是从以下两个方面进行改进：
  1）序列中不同的位置处的单词(已单词举例)对当前的隐藏层的状态的影响不同，越前面的影响越小，即每个前面状态对当前的影响进行了距离加权，距离越远，权值越小。
  2）在产生误差error时，误差可能是由某一个或者几个单词而引发的，所以应当仅仅对对应的单词weight进行更新。

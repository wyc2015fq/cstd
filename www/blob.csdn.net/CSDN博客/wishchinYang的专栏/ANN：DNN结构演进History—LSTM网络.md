# ANN：DNN结构演进History—LSTM网络 - wishchinYang的专栏 - CSDN博客
2015年08月24日 13:19:18[wishchin](https://me.csdn.net/wishchin)阅读数：622
        为了保持文章系列的连贯性，参考这个文章：
        此前的文章： [DNN结构演进History—LSTM_NN](http://blog.csdn.net/wishchin/article/details/42425087)  ，这个**文章的分析更加全面清**晰：[Understanding LSTM NetWork](http://colah.github.io/posts/2015-08-Understanding-LSTMs/) 分析了各种LSTM网络，LSTM小品文 ：[谷歌语音转录背后的神经网络](http://www.csdn.net/article/2015-08-15/2825468)。
摘要：
        LSTM使用一个控制门控制参数是否进行梯度计算，以此避免梯度消失或者爆炸。
**LSTM的优势与不足**
**LSTM的不足**
- 
LSTM的高效截断版本并不能很轻松的解决“**强延迟异或**”类的问题。
- 
LSTM的每个存储单元块需要一个输入门和一个输出门，而这在其他的循环方法中并不是必需的。
- 
常数误差流通过存储单元内部的“Constant Error Carrousels”，能够跟传统的前馈架构一样，产生与整个输入串相同的效果。
- 
LSTM与其他前馈方法一样，在“regency”的概念上有缺陷。 如果需要精密时间步长计数，可能还需要额外的计数机制。
**LSTM的优势**
- 
内存单元中反向传播的常数误差，赋予该架构桥接长时滞的算法的能力。
- 
LSTM可近似于噪声问题域、分布式表示和连续值。
- 
LSTM概述了要考虑的问题域。这一点很重要，因为一些任务对于已经建立的循环网络来说很棘手。
- 
在问题域上不需要微调网络参数。
- 
在每个权重和时间步长更新的复杂性方面，LSTM基本上等同于BPTT。
- 
LSTM在机器翻译等领域取得了当前最先进的结果，显示出强大的能力。
参考：[LSTM、GRU与NTM这些最先进的循环神经网络，是如何进行深度学习任](https://www.yidianzixun.com/article/0GzYShOd?title_sn/0&s=9&appid=yidian&ver=4.5.5.3&utk=ag0j9ca9)务的

# tensorflow，神经网络，spark - 找工作_用户5706034127_新浪博客
|||
记录学习 tensorflow，神经网络，spark的过程。
显示学习tensorflow技术栈，框架，
------------------------------------------------------------------------------------------------------------
然后看到了介绍ILSVRC比赛历年的冠军网络，，然后就学习了卷积、循环、残差神经网络。
，ILSVRC比赛历年冠军网络深度也是越来越深的。
从12年的冠军  AlexNet： ReLU
Nonlinearity**， **Local
Response Normalization ， Dropout，  Data
Augmentation。
14年的冠军GoogLeNet：paper见Going
Deeper with Convolutions.相关工作主要包括LeNet-5、Gabor
filters、Network-in-Network.Network-in-Network改进了传统的CNN网络，采用少量的参数就轻松地击败了AlexNet网络，使用Network-in-Network的模型最后大小约为29MNetwork-in-Network
caffe
model.GoogLeNet借鉴了Network-in-Network的思想，下面会详细讲述下。
ILSVRC2015冠军Deep Residual
Network：
------------------------------------------------------------------------------------------------------------
卷积神经网络中的，卷基层，也叫过滤层，比如一个检测直线的卷基层，只有图片的直线部分才会输出较大信号，其它部分输出信号很小，所以也可以当做是过滤。
------------------------------------------------------------------------------------------------------------
循环神经网络：[LSTMs(Long Short-Term Memory，长短时记忆模型)模型](https://en.wikipedia.org/wiki/Long_short_term_memory)
RNN for Machine
Translation. [Image
Source](http://cs224d.stanford.edu/lectures/CS224d-Lecture8.pdf)
  下面是关于RNNs中机器翻译研究的三篇文章：
- [A Recursive Recurrent Neural Network for Statistical Machine
Translation](http://www.aclweb.org/anthology/P14-1140.pdf)
- [Sequence to Sequence Learning with Neural Networks](http://papers.nips.cc/paper/5346-sequence-to-sequence-learning-with-neural-networks.pdf)
- [Joint Language and Translation Modeling with Recurrent Neural
Networks](http://research.microsoft.com/en-us/um/people/gzweig/Pubs/EMNLP2013RNNMT.pdf)
#### 语音识别(Speech Recognition)
[语音识别](http://lib.csdn.net/base/vras)是指给一段声波的声音信号，预测该声波对应的某种指定源语言的语句以及该语句的概率值。 
  RNNs中的语音识别研究论文：
- [Towards End-to-End Speech Recognition with Recurrent Neural
Networks](http://www.jmlr.org/proceedings/papers/v32/graves14.pdf)
最好动手实践一下吧。


# RNN - 郭云飞的专栏 - CSDN博客


2017年10月10日 15:05:48[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1946


**RNN（Recurrent Neural Network）——循环神经网络**，已经在自然语言处理*(Natural Language Processing, NLP*)中取得了巨大成功以及广泛应用。
不同于传统的*FNN*(Feed-forward Neural Network，前向反馈神经网络)，RNN引入了定向循环，能够处理那些输入之间前后关联的问题。定向循环结构如下图所示：
![](https://img-blog.csdn.net/20171010150922824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
RNN的目的是用来处理序列数据。
传统的神经网络模型，是从输入层到隐含层再到输出层，层与层之间是全连接的，每层之间的节点是无连接的。
这种传统的神经网络对于很多问题却无能无力的。例如，你要预测句子的下一个单词是什么，一般需要用到前面的单词，因为一个句子中前后单词并不是独立的。RNN之所以称为循环神经网路，即**一个序列当前的输出与前面的输出也有关**。具体的表现形式为网络会对前面的信息进行记忆并应用于当前输出的计算中，即**隐藏层之间的节点不再无连接而是有连接的**，并且隐藏层的输入不仅包括输入层的输出还包括上一时刻隐藏层的输出。理论上，RNN能够对任何长度的序列数据进行处理。但是在实践中，为了降低复杂性往往假设当前的状态只与前面的几个状态相关，下图便是一个典型的RNN：
![](https://img-blog.csdn.net/20171010151559821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**RNN能干什么？**
RNN已经被在实践中证明对自然语言处理（*NLP*）是非常成功的。如词向量表达、语句合法性检查、词性标注等。在RNNs中，目前使用最广泛最成功的模型便是**LSTM（Long Short-Term Memory，长短时记忆模型）**模型，该模型通常比vanilla RNNs能够更好地对长短时依赖进行表达，该模型相对于一般的RNNs，只是在隐藏层做了手脚。下面对RNNs在*NLP*中的应用进行简单的介绍。
**RNN应用1——机器翻译(Machine Translation)**
下面是关于RNN在机器翻译研究中的几篇文章：
A Recursive Recurrent Neural Network for Statistical Machine Translation
Sequence to Sequence Learning with Neural Networks
Joint Language and Translation Modeling with Recurrent Neural Networks
**RNN应用2——语音识别（speech recognition）**

Towards End-to-End Speech Recognition with Recurrent Neural Networks
RNN应用3——**图像描述生成 (Generating Image Descriptions)**
和卷积神经网络(convolutional Neural Networks, CNNs)一样，RNNs已经在对无标图像描述自动生成中得到应用。将CNNs与RNNs结合进行图像描述自动生成。这是一个非常神奇的研究与应用。该组合模型能够根据图像的特征生成描述。
**如何训练RNNs**
对于RNN的训练和对传统的ANN训练一样。同样使用BP误差反向传播算法，不过有一点区别。如果将RNNs进行网络展开，那么参数W,U,V是共享的，而传统神经网络却不是的。并且在使用梯度下降算法中，每一步的输出不仅依赖当前步的网络，并且还以来前面若干步网络的状态。比如，在t=4时，我们还需要向后传递三步，已经后面的三步都需要加上各种的梯度。该学习算法称为*Backpropagation Through Time (BPTT)*。需要意识到的是，在vanilla RNNs训练中，BPTT无法解决长时依赖问题(即当前的输出与前面很长的一段序列有关，一般超过十步就无能为力了)，因为BPTT会带来所谓的梯度消失或梯度爆炸问题(the
 vanishing/exploding gradient problem)。当然，有很多方法去解决这个问题，如*LSTM*便是专门应对这种问题的。
**常见的一些RNNs模型**
模型1——*Simple RNNs(SRNs)*
SRNs是RNNs的一种特例，它是一个三层网络，并且在隐藏层增加了上下文单元，下图中的y便是隐藏层，u便是上下文单元。上下文单元节点与隐藏层中的节点的连接是固定(谁与谁连接)的，并且权值也是固定的(值是多少)，其实是一个上下文节点与隐藏层节点一一对应，并且值是确定的。在每一步中，使用标准的前向反馈进行传播，然后使用学习算法进行学习。上下文每一个节点保存其连接的隐藏层节点的上一步的输出，即保存上文，并作用于当前步对应的隐藏层节点的状态，即隐藏层的输入由输入层的输出与上一步的自己的状态所决定的。因此SRNs能够解决标准的多层感知机(MLP)无法解决的对序列数据进行预测的任务。
![](https://img-blog.csdn.net/20171010153457950?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
模型2——Bidirectional RNNs
Bidirectional RNNs(双向网络)的改进之处便是，假设当前的输出(第t步的输出)不仅仅与前面的序列有关，并且还与后面的序列有关。例如：预测一个语句中缺失的词语那么就需要根据上下文来进行预测。Bidirectional RNNs是一个相对较简单的RNNs，是由两个RNNs上下叠加在一起组成的。输出由这两个RNNs的隐藏层的状态决定的。
模型3——Deep(Bidirectional)RNNs
Deep(Bidirectional)RNNs与Bidirectional RNNs相似，只是对于每一步的输入有多层网络。这样，该网络便有更强大的表达与学习能力，但是复杂性也提高了，同时需要更多的训练数据。
模型4——Echo State Networks
ESNs(回声状态网络)虽然也是一种RNNs，但是它与传统的RNNs相差很大。ESNs具有三个特点：
它的核心结构时一个随机生成、且保持不变的储备池(Reservoir)，储备池是大规模的、随机生成的、稀疏连接(SD通常保持1%～5%，SD表示储备池中互相连接的神经元占总的神经元个数N的比例)的循环结构；
其储备池到输出层的权值矩阵是唯一需要调整的部分；
简单的线性回归就可完成网络的训练。
模型5——Gated Recurrent Unit Recurrent Neural Networks
GRUs也是一般的RNNs的改良版本，主要是从以下两个方面进行改进。一是，序列中不同的位置处的单词(已单词举例)对当前的隐藏层的状态的影响不同，越前面的影响越小，即每个前面状态对当前的影响进行了距离加权，距离越远，权值越小。二是，在产生误差error时，误差可能是由某一个或者几个单词而引发的，所以应当仅仅对对应的单词weight进行更新。GRUs的结构如下图所示。GRUs首先根据当前输入单词向量word vector已经前一个隐藏层的状态hidden state计算出update gate和reset
 gate。再根据reset gate、当前word vector以及前一个hidden state计算新的记忆单元内容(new memory content)。当reset gate为1的时候，new memory content忽略之前的所有memory content，最终的memory是之前的hidden state与new memory content的结合。
模型6——LSTM Netwoorks
LSTMs与GRUs类似，目前非常流行。它与一般的RNNs结构本质上并没有什么不同，只是使用了不同的函数去去计算隐藏层的状态。在LSTMs中，i结构被称为cells，可以把cells看作是黑盒用以保存当前输入xt之前的保存的状态ht−1，这些cells更加一定的条件决定哪些cell抑制哪些cell兴奋。它们结合前面的状态、当前的记忆与当前的输入。已经证明，该网络结构在对长序列依赖问题中非常有效。
**以上模型的参考文献**
Hinton G E. Learning Distributed Representations of Concepts[C]. Proceedings of the 8th Annual Conference of the Cognitive Science Society. 1986, 1: 12.
Elman, J. L. Finding structure in time. CRL Technical Report 8801, Center for Research in Language, University
of California, San Diego, 1988.
Schuster M, Paliwal K K. Bidirectional recurrent neural networks[J]. Signal Processing, IEEE Transactions on, 1997, 45(11): 2673-2681.
Graves A, Mohamed A R, Hinton G. Speech Recognition with Deep Recurrent Neural Networks[J]. Acoustics Speech & Signal Processing . icassp. international Conference on, 2013:6645 - 6649.
Jaeger H, Haas H. Harnessing nonlinearity: Predicting chaotic systems and saving energy in wireless communication[J]. Science, 2004, 304(5667): 78-80.
Cho K, Van Merrienboer B, Gulcehre C, et al. Learning Phrase Representations using RNN Encoder-Decoder for Statistical Machine Translation[J]. Eprint Arxiv, 2014.
Hochreiter S, Schmidhuber J. Long short-term memory.[J]. Neural Computation, 1997, 9(8):1735-1780.
Chung J, Gulcehre C, Cho K H, et al. Empirical evaluation of gated recurrent neural networks on sequence modeling[J]. arXiv preprint arXiv:1412.3555, 2014.
Jan Koutnik, Klaus Greff, Faustino Gomez, Juergen Schmidhuber. A Clockwork RNN[J]. Proceedings of The 31st International Conference on Machine Learning, pp. 1863–1871, 2014.
Sutskever, Ilya, Martens, James, Dahl, George E., and Hinton, Geoffrey E. On the importance of initialization and momentum in deep learning. In Dasgupta, Sanjoy and Mcallester, David (eds.), Proceedings of the 30th International Conference on Machine Learning (ICML-13), volume 28, pp. 1139–1147.



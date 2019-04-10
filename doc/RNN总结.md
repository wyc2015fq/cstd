# RNN总结



# 一、overview

2015年10月05日 00:15:15 [xmucas](https://me.csdn.net/xmdxcsj) 阅读数：1312



想学习神经网络，由于最近有语义理解和断句的工作需求，正好以RNN为切入点，开始神经网络方面的学习。

一些RNN学习的资源：



<https://en.wikipedia.org/wiki/Recurrent_neural_network>

<http://www.zhihu.com/question/29411132>



# 神经网络基础

## Neural Networks



![img](https://img-blog.csdn.net/20151005000944026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

神经网络中的神经元结构如上图

## Feedforward Neural Networks

![img](https://img-blog.csdn.net/20151005000955831?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

所有节点之间不存在环，所以所有的节点可以归类到某一层，一层的输出基于上一层的输出，输入X作为最底层的输入。

前馈网络的局限性在于需要保证输入数据的独立性，对于视频、音乐或者句子这些在时间维度相关的输入数据来说，应用前馈网络就有很大的局限性。

# RNN

![img](https://img-blog.csdn.net/20151005001023001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20151005001205762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20151005001249991?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

时刻t隐层的输入不仅跟时刻t输入x(t)有关，还跟t-1时刻的隐层h(t-1)有关。所以最后将导致，时刻t-1的输入x(t-1)将会影响t时刻的输出y(t)。

## Training

由于RNN在时间维度的高依赖性，导致RNN的训练相比于DNN更为复杂。主要的两大难点在于vanishing和exploding。

Truncated backpropagationthrough time (TBPTT)是一种有效的训练方法。

# Modern RNNs

两个最成功的RNN结构是LSTM和BRNN。

### LSTM

![img](https://img-blog.csdn.net/20151005001344179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20151005001350985?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20151005001357290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Input node：g，跟上一时刻的隐层和当前时刻的输入x有关，激活函数是tanh。

Input gate：i，跟上一时刻的隐层和当前时刻的输入x有关

Internal state：s，有一条自旋的边称为constant error carousel，保证误差随着时间的传播没有vanishing

Forget gate：f

Output gate：o

前向使用input gate和output gate控制是否通过internal state，后向使用constant error carousel避免误差的vanishing和exploding。

### BRNN 

Bidirectional RecurrentNeural Networks包含两个隐层，第一个隐层跟过去有关，第二个隐层跟未来有关。



![img](https://img-blog.csdn.net/20151005001425447?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

BRNN的局限性在于其需要未来的信息，不适合应用线上 环境（因为没有未来的信息），但可以用在自然语言处理方面，比如文本标注问题，跟上下文都有关系。

 ![img](https://img-blog.csdn.net/20151005001435468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 ![img](https://img-blog.csdn.net/20151005001443839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

参考文献

A Critical Review of Recurrent Neural Networksfor Sequence Learning




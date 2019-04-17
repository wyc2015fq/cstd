# 【深度学习】RNN（循环神经网络）之LSTM（长短时记忆） - zkq_1986的博客 - CSDN博客





2016年11月14日 16:54:07[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：3346








## 1 循环神经网络（RNN）概念

循环神经网络，是一种线性序列结构，前一个神经元的结果会传递给后一个神经元。 

递归神经网络，是一种层次结构，前一个神经元会影响后一个神经元，后一个神经元同样也会影响前一个神经元。循环神经网络是递归神经网络中的一种特殊情况，因为线性结构是层次结构的一种特例。

## 2 RNN结构

### 2.1 误差传播为什么容易消失或放大

1）如果采用线性激活函数，误差会被放大； 

2）如果采用sigmoid、双曲正切tanh，误差经过多层之后消失。

数学公式推导，有待学习。

### 2.2

![这里写图片描述](https://img-blog.csdn.net/20170313141101197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemtxXzE5ODY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170313141122885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemtxXzE5ODY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170313141136729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemtxXzE5ODY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 3 小知识

### 3.1  循环神经网络

![这里写图片描述](https://img-blog.csdn.net/20170313141205198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemtxXzE5ODY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

循环神经网络是指，存在这样一种结构，即神经元的输出同时作为下一次该神经元的输入，形成循环。





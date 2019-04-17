# lstm(一) 演化之路 - xmdxcsj的专栏 - CSDN博客





2016年09月13日 16:14:28[xmucas](https://me.csdn.net/xmdxcsj)阅读数：7311标签：[神经网络](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)
个人分类：[神经网络](https://blog.csdn.net/xmdxcsj/article/category/5855803)









递归神经网络引入了时序的反馈机制，在语音、音乐等时序信号的分析上有重要的意义。 

Hochreiter(应该是Schmidhuber的弟子)在1991年**分析了bptt带来的梯度爆炸和消失问题**，给学习算法带来了梯度 

震荡和学习困难等问题； 

Hochreater和Schmidhuber在1997年**提出了LSTM的网络结构**，引入CEC单元解决bptt的梯度爆炸和消失问题； 

Felix Gers(Schmidhuber是指导人之一)2001年的博士论文进一步改进了lstm的网络结构，**增加了forget gate和peephole**； 

Alex Graves(Schmidhuber的弟子)2006年提出了**lstm的ctc训练准则**。
## 第一步：RNN->基本lstm

参考文献[1]和[2]

### 问题

#### 问题一：gradient

BPTT学习算法存在梯度爆炸和消失问题(gradient blow up or vanish)，简单通过local error flow分析如下： 

对RNN的隐层进行[unfolding](http://blog.csdn.net/xmdxcsj/article/details/50088967)后，可以得到如下的递推关系： 


$\vartheta_j(t)=f'_j(net_j(t))\sum_iw_{ij}\vartheta_i(t+1)$

可以理解为t+1时刻的error通过$w_{ij}$传递到t时刻的j节点。 

对于t时刻的节点u，通过bptt可以传递到t-q时刻的节点u，不难得到递推关系如下： 


$\frac{\partial{\vartheta_v (t-q)}}{\partial{\vartheta_u(t)}} =  \begin{cases}    f'_v(net_v(t-1))w_{uv}       & \quad q=1\\    f'_{v}(net_v(t-q))\sum_{l_{q-1}=1}^{n}\frac{\partial{\vartheta_{l_{q-1}} (t-q+1)}}{\partial{\vartheta_u(t)}} w_{l_{q-1}v} & \quad q>1\\  \end{cases}$

由此可以递推： 


$\frac{\partial{\vartheta_{l_{q-1}} (t-q+1)}}{\partial{\vartheta_u(t)}}=f'_{l_{q-1}}(net_{l_{q-1}}(t-q+1))\sum_{l_{q-2}=1}^{n}\frac{\partial{\vartheta_{l_{q-2}} (t-q+2)}}{\partial{\vartheta_u(t)}} w_{{l_{q-2}}l_{q-1}}$


$... ...$

设$l_q=v$以及$l_0=u$最后可得： 


$\frac{\partial{\vartheta_v (t-q)}}{\partial{\vartheta_u(t)}} =\sum_{l_{1}=1}^{n}...\sum_{l_{q-1}=1}^{n}\prod_{m=1}^{q}f'_{l_m}(net_{l_{m}}(t-m))w_{{l_m}{l_{m-1}}}$

所以后面的联乘公式对于梯度的传播影响很大，如果 


$|f'_{l_m}(net_{l_{m}}(t-m))w_{{l_m}{l_{m-1}}}|>1.0$

会有梯度爆炸问题，反之，有梯度消失问题。 
**直观上理解，error向后传播的时候，每向前传递一步都需要乘以对应的系数W，系数W的大小会导致梯度的异常。**
#### 问题二：conflict
- input weight conflict 

假设$w_{ji}$表示输入层到隐层之间的连接，对于有些输入希望尽可能通过，也就是$w_{ji}$比较大，但是另外一些无关的输入可能希望尽可能屏蔽掉，也就是$w_{ji}$尽可能为0。而实际网络中的$w_{ji}$参数是跟输入无关，对于所有的输入，它的大小是一致的。由于缺少这种自动调节功能，从而导致学习比较困难。
- output weight conflict 

同理，隐层到输出层之间也存在放行和屏蔽的conflict。

### 解决

![这里写图片描述](https://img-blog.csdn.net/20160913161049567)

1997年Hochreiter和Schmidhuber首先提出了LSTM的网络结构，解决了传统RNN的上面两个问题。
#### 问题一的solution

lstm通过引入**CEC(constant error carrousel)**单元解决了梯度沿时间尺度unfolding带来的问题。 

首先梯度的递推关系如下： 


$\vartheta_j(t)=f'_j(net_j(t))\sum_iw_{ij}\vartheta_i(t+1)$

要想梯度传播没有异常的话，最容易的想法是： 


$f'_j(net_j(t))w_{ij}=1$

这里进一步简化这个问题，可以如下约定： 


$f_{j}(x)=x$


$w_{jj}=1$


$w_{ij}=0(i\neq j)$

相比之前的RNN结构，做了如下改变： 

1. 矩阵$W_{hh}$简化为对角矩阵，也就是只允许节点的自旋，不允许隐层的其他节点连接到本节点 

2. 激活函数sigmoid替换为了线性函数$f(x)=x$
**以上两点保证了error可以无损由t时刻传递到t-1时刻，如上图中的$s_{c_{j}}(t)=s_{c_{j}}(t-1)+gy^{in_{j}}$，CEC是lstm的核心部件**

#### 问题二的solution

针对问题二，lstm引入了两个gate：input gate（对应图中的$in_j$）可以控制某些输入进入cell（对应图中的$c_j$）更新原来存储的信息，或者屏蔽输入以保持cell存储的信息不变；output gate（对应图中的$out_j$）以控制cell的信息对输出产生多大程度的影响。 

以output gate为例，直观上可以理解为（**个人理解，欢迎讨论**）： 

传统RNN隐层到输出层的连接是由参数$w_{kj}$控制的，由于对所有的隐层输出，$w_{kj}$都是一视同仁的，这对于传统的DNN模型来讲是没有问题，因为DNN模型不会考虑历史信息，DNN可以理解为一个简单的函数，一个输入有对应的输出就可以了，没有必要使用gate加以限制。 

但是对于RNN来讲，这种一视同仁则是不合理的，因为它需要存储历史信息，而且该历史信息会对当前的输出产生影响（有些时候是至关重要的影响），比如说”I grew up in France… I speak fluent French”，此时的France对French的影响将会是决定性的。所以lstm引入了gate来改善这种一视同仁的不良现状，为什么gate会改善这种不良现状呢？ 

因为此时的输出结果不再只由$w_{kj}$来控制了，还会受到output gate开门或者关门的影响，而output gate开门或者关门的控制权是由$w_{out_ji}$参数以及输入x等参数控制的，所以此时的输出多了一条watchdog，参数增多了控制的将会更精确。
## 第二步：lstm + forget gate

参考文献[3]

### 问题

传统的lstm存在一个问题：随着时间序列的增多，lstm网络没有重置的机制（比如两句话合成一句话作为输入的话，希望是在第一句话结束的时候进行reset），从而导致cell state容易发生饱和，进一步会导致cell state的输出h（趋近于1）的梯度很小（sigmoid函数在x值很大的时候梯度趋向于0），阻碍了error的传入；另一方面输出h趋近于1，导致cell的输出近似等于output gate的输出，意味着网络丧失了memory的功能。

### 解决

![这里写图片描述](https://img-blog.csdn.net/20160913161121078)

在传统lstm的基础之上，引入了forget gate。使用这种结构可以让网络自动学习什么时候应该reset。具体做法即为使用$y^\varphi$替换原来的CEC的常量1.0，定义如下; 


$net_{\varphi_j}(t)=\sum_{m}w_{\varphi_jm}y^{m}(t-1)$


$y^{\varphi_j}(t)=f_{\varphi_j}(net_{\varphi_j}(t))$

forget gate的引入可以解决需要内部切割（hierarchical decomposition）但又不知道切割位置的序列问题。
## 第三步：lstm+peephole

参考文献[3]

### 问题

lstm的gate的输入包含两个部分，网络输入和上一时刻（t-1）网络的输出。 

此时如果output gate关闭（值接近0）的话，网络的输出（t时刻）将为0，下一时刻（t+1）网络gate将完全跟网络输入有关，就会丢失历史信息。

### 解决

![这里写图片描述](https://img-blog.csdn.net/20160913161405243)

增加CEC到各个gate之间的连线，使得CEC(const error carrousels)和gate之间存在双向的关联，CEC收到当前时刻gate的限制，同时又会影响下一时刻的gate。 

- input gate和forget gate的输入增加一项$s(t-1)$

- output gate的输入增加一项$s(t)$
peephole使得网络可以记录更多的时序上的关联性，有助于提取相关事件准确周期的相关信息，可以应用于音乐韵律的分析等工作。

## 第四步：CTC训练准则

[ctc训练](http://blog.csdn.net/xmdxcsj/article/details/51763868)

## 参考

[1]《Untersuchungen zu dynamischen neuronalen Netzen》 Hochreiter（德文的，人家的硕士论文） 

[2]《Long Short-Term Memory》  Hochreiter, Sepp; Schmidhuber 

[3]《Long Short-Term Memory in Recurrent Neural Networks》 Felix Gers 

[4]《Supervised Sequence Labelling with Recurrent Neural Networks》 Alex Graves 

[5] [http://colah.github.io/posts/2015-08-Understanding-LSTMs/](http://colah.github.io/posts/2015-08-Understanding-LSTMs/)











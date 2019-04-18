# LSTM—GRU—indRNN 避免与解决梯度弥散与爆炸--Batch Normalization - wsp_1138886114的博客 - CSDN博客





2018年08月09日 15:18:20[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1953
所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)











- - - - - [1. LSTM避免RNN的梯度消失（gradient vanishing）](#1-lstm避免rnn的梯度消失gradient-vanishing)
- [2. LSTM避免RNN的梯度爆炸](#2-lstm避免rnn的梯度爆炸)
- [3.  GRU 与 LSTM 比较](#3-gru-与-lstm-比较)
- [4 Batch Normalization 到 Group Normalization](#4-batch-normalization-到-group-normalization)- [Batch Normalization](#batch-normalization)
- [Group Normalization](#group-normalization)









##### 1. LSTM避免RNN的梯度消失（gradient vanishing）

RNN的本质是在网络内部维护了一个状态 $S_t$，其中 $t$ 表示时间且 $S_t$ 可递归计算。  
- 传统的RNN总是用“覆写”的方式计算状态：$S_t = f(S_{t-1},x_t)$， 其中$f(·)$表示仿射变换外面在套一个Sigmoid， $S_t$ 表示输入序列在时刻 $t$ 的值。根据求导的链式法则，这种形式直接导致梯度被表示为连成积的形式，以致于造成梯度消失——粗略的说, 很多个小于1的项连乘就很快的逼近零。 
- 
现代的RNN（包括但不限于使用LSTM单元的RNN）使用“累加”的形式计算状态：  
$S_t = \sum_{\tau =1}^{t}\Delta S_\tau $，其中的 $\Delta$ 显示依赖序列输入$x_t$ 稍加推导即可发现，这种累加形式导致导数也是累加形式，因此避免了梯度消失。 

- 
如长短期记忆（LSTM）与门控已经提出复发单元（GRU）来解决梯度问题。然而，双曲正切的使用Sigmoid函数作为激活函数这些变体导致在层上的梯度衰减。因此，深LSTM或GRU的建设与训练基于RNN的网络实际上是困难的。相比之下，IndRNN 使用非饱和激活函数等CNNsRelu可以被堆叠到非常深的网络中。使用基本卷积层和100层以上的层


##### 2. LSTM避免RNN的梯度爆炸
- 设定阈值和使用正则化减少权重 

爆炸梯度问题相对容易。

通过**简单地收缩其规范超过的梯度**来处理阈值，一种被称为梯度裁剪的技术。 

如果一个巨大的因素减少了梯度，就会受到影响。过于频繁的裁剪是非常有效的。

##### 3.  GRU 与 LSTM 比较
- GRU (Gated Recurrent Unit)
GRU与LSTM单元相似，GRU具有调节信息流动的门单元，但是，没有一个单独的记忆单元（memory cells）。 

GRU在时刻 $t$ 的 **激励 $h_t^j$ 是一个线性的修改**：$h_t^j = (1-z_t^j)h_{t-1}^j+z_t^j\tilde{h}_t^j$更新门的计算为 : $z_t^j = \sigma (W_zx_t + U_z h_{t-1})^j$这个过程是把现在的状态和新的状态求和，和LSTM的单元类似。**GRU没有采取任何机制去控制那个状态暴露出来，而是每次所有状态都会暴露。**- LSTM ( Long Short-Term Memory Unit)
一般的循环单元只是简单的计算输入信号的权重和一个非线性函数。而LSTM每一个在t时刻的LSTM单元 $j$ 有一个记忆 $c_t^j$ ,LSTM的输出或者激励是: $h_t^j = o_t^j tanh(c_t^j)$
GRC把LSTM的 input gate 和 forget gate 整合成一个update gate，也是通过gate机制来控制梯度。 

除语言建模外，GRU（门控重复单元）在所有任务上都优于LSTM  
![这里写图片描述](https://img-blog.csdn.net/20180809151500510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- MUT1与GRU在语言建模方面的表现相匹配，在所有其他任务上均优于GRU
- 允许丢失时，LSTM在PTB上的性能明显优于其他体系结构
- 增加大忘记栅极偏置可大大提高LSTM性能
- LSTM的遗忘门是最重要的，而输出门相对不重要 一般初始化bais=0.5

##### 4 Batch Normalization 到 Group Normalization

现在我们已经知道：
- 激活函数对梯度也有很大的影响，大部分激活函数只在某个区域内梯度比较好。
- 在后向传播的时候，我们需要进行链式求导，如果网络层很深，激活函数如果权重很小，会导致梯度消失；如果权重很大，又会导致梯度爆炸。

那么解决梯度消失可以从这几方面入手： 

1）换激活函数；2）调整激活函数的输入；3）调整网络结构

事实上，我们有一个好东西可以解决梯度问题，叫做Normalization，就是从第二方面入手同时解决梯度消失和爆炸，而且也可以加快训练速度。

###### Batch Normalization

假设对于一个batch内某个维度的特征 $ {{x_1, x_2, ..., x_m}}$， 

BN需要将其转化成 $ {{y_1, y_2, ..., y_m}}$， 

首先对节点的线性组合值进行归一化，使其均值是0，方差是1。（也就是，对节点的输入进行归一化，而不是对输出进行归一化）（归一化后使梯度稳定,快速收敛）


$x_i'=\frac{x_i-\mu}{\sqrt{\sigma^{2}+\varepsilon}}$

其中 $ \mu $ 是均值，$ \sigma^{2}$ 是标准差，$ \varepsilon$ 是用来控制分母为正。

但是数据本来不是这样子的啊！我们强行对数据进行缩放，可能是有问题的，所以BN又加了一个scale的操作，使得数据有可能会恢复回原来的样子：



$y_i = \gamma x_i'+\beta$

加了scale可以提升模型的容纳能力。

既然是Batch归一化，那么BN就会受到batch size的影响： 
- 如果size太小，算出的均值和方差就会不准确，影响归一化，导致性能下降，
- 如果太大，内存可能不够用。 
- 优点：
（1）**可以使用更高的学习率，BN有快速收敛的特性。**在没有加Batch Normalization的网络中我们要慢慢的调整学习率时，甚至在网络训练到一半的时候，还需要想着学习率进一步调小的比例选择多少比较合适。现在，我们在网络中加入Batch Normalization时，可以采用初始化很大的学习率，然后学习率衰减速度也很大，因此这个算法收敛很快。
（2）**模型中BN可以代替dropout或者使用较低的dropout**。dropout是经常用来防止过拟合的方法，但是模型中加入BN减少dropout，可以大大提高模型训练速度，提高网络泛化性能。

（3）**减少L2权重衰减系数**。用了Batch Normalization后，可以把L2权重衰减系数降低，论文中降低为原来的5倍。

（4）**取消Loacl Response Normalization层**。（局部响应归一化是Alexnet网络用到的方法），因为BN本身就是一个归一化网络层。

（5）**BN本质上解决了反向传播过程中梯度消失的问题**。BN算法对Sigmoid激活函数的提升非常明显，解决了困扰学术界十几年的sigmoid过饱和造成梯度消失的问题。在深度神经网络中，靠近输入的网络层在梯度下降的时候，得到梯度值太小，导致深层神经网络只有靠近输出层的那几层网络在学习。因为数据使用BN后，归一化的数据仅使用了sigmoid线性的部分。

（6）**可以把训练数据彻底打乱**。防止了每批训练的时候，某一个样本经常被挑选到。论文中指出这个操作可以提高1%的精度。

###### Group Normalization

因此上个月提出的GN，就是为了避免batch size带来的影响。乍一看标题以为做了啥大改革，BN要退出舞台了，其实只是归一化的方向不一样，不再沿batch方向归一化，他们的不同点就在于归一化的方向不一样： 
![这里写图片描述](https://img-blog.csdn.net/20180809153342240?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

BN：批量归一化，往batch方向做归一化，归一化维度是[N，H，W] 

LN：层次归一化，往channel方向做归一化，归一化维度为[C，H，W] 

IN：实例归一化，只在一个channel内做归一化，归一化维度为[H，W] 

GN：介于LN和IN之间，在channel方向分group来做归一化，归一化的维度为[C//G , H, W]






# YJango的循环神经网络——实现LSTM - 知乎
# 

## **介绍**

描述最常用的RNN实现方式：Long-Short Term Memory（LSTM）

## 梯度消失和梯度爆炸

**网络回忆：**在[《循环神经网络——介绍》](https://link.zhihu.com/?target=https%3A//yjango.gitbooks.io/superorganism/content/%25E9%2580%2592%25E5%25BD%2592%25E7%25A5%259E%25E7%25BB%258F%25E7%25BD%2591%25E7%25BB%259C.html)中提到循环神经网络用相同的方式处理每个时刻的数据。
- **动态图：**
![](https://pic3.zhimg.com/v2-db52470df88f53271d8e06722da39122_b.gif)

- **数学公式**：![h_t= \phi(W_{xh} \cdot x_t + W_{hh} \cdot h_{t-1} + {b})](https://www.zhihu.com/equation?tex=h_t%3D+%5Cphi%28W_%7Bxh%7D+%5Ccdot+x_t+%2B+W_%7Bhh%7D+%5Ccdot+h_%7Bt-1%7D+%2B+%7Bb%7D%29)

**设计目的：**我们希望循环神经网络可以将过去时刻发生的状态信息传递给当前时刻的计算中。

**实际问题：**但普通的RNN结构却难以传递相隔较远的信息。
- **考虑：**若只看上图蓝色箭头线的、隐藏状态的传递过程，不考虑非线性部分，那么就会得到一个简化的式子(1)：
- (1) ![h_t= W_{hh} \cdot h_{t-1}](https://www.zhihu.com/equation?tex=h_t%3D+W_%7Bhh%7D+%5Ccdot+h_%7Bt-1%7D)
- 如果将起始时刻的隐藏状态信息![h_0](https://www.zhihu.com/equation?tex=h_0)向第![t](https://www.zhihu.com/equation?tex=t)时刻传递，会得到式子(2)
- (2) ![h_t= (W_{hh})^t \cdot h_{0}](https://www.zhihu.com/equation?tex=h_t%3D+%28W_%7Bhh%7D%29%5Et+%5Ccdot+h_%7B0%7D)
- ![W_{hh}](https://www.zhihu.com/equation?tex=W_%7Bhh%7D)会被乘以多次，若允许矩阵![W_{hh}](https://www.zhihu.com/equation?tex=W_%7Bhh%7D)进行特征分解
- (3) ![h_t= (W_{hh})^t \cdot h_{0}](https://www.zhihu.com/equation?tex=h_t%3D+%28W_%7Bhh%7D%29%5Et+%5Ccdot+h_%7B0%7D)
- 式子(2)会变成(4)
- (4) ![h_t= Q \cdot \Lambda ^t \cdot Q^T \cdot h_{0}](https://www.zhihu.com/equation?tex=h_t%3D+Q+%5Ccdot+%5CLambda+%5Et+%5Ccdot+Q%5ET+%5Ccdot+h_%7B0%7D)

当特征值小于1时，不断相乘的结果是特征值的![t](https://www.zhihu.com/equation?tex=t)次方向 ![0](https://www.zhihu.com/equation?tex=0) 衰减； 当特征值大于1时，不断相乘的结果是特征值的t次方向 ![\infty](https://www.zhihu.com/equation?tex=%5Cinfty)扩增。 这时想要传递的![h_0](https://www.zhihu.com/equation?tex=h_0)中的信息会被掩盖掉，无法传递到![h_t](https://www.zhihu.com/equation?tex=h_t)。

![](https://pic3.zhimg.com/v2-f1f179149020fd496170de1bb25fcbee_b.png)- **类比：**设想y![y=a^t*x](https://www.zhihu.com/equation?tex=y%3Da%5Et%2Ax)，如果![a](https://www.zhihu.com/equation?tex=a)等于0.1，![x](https://www.zhihu.com/equation?tex=x)在被不断乘以0.1一百次后会变成多小？如果![a](https://www.zhihu.com/equation?tex=a)等于5，![x](https://www.zhihu.com/equation?tex=x)在被不断乘以5一百次后会变得多大？若想要![x](https://www.zhihu.com/equation?tex=x)所包含的信息既不消失，又不爆炸，就需要尽可能的将![a](https://www.zhihu.com/equation?tex=a)的值保持在1。
- 注：更多内容请参阅[Deep Learning by Ian Goodfellow中第十章](https://link.zhihu.com/?target=http%3A//www.deeplearningbook.org/contents/rnn.html)。

## **Long Short Term Memory (LSTM)**

上面的现象可能并不意味着无法学习，但是即便可以，也会非常非常的慢。为了有效的利用梯度下降法学习，我们希望使不断相乘的梯度的积(the product of derivatives)保持在接近1的数值。

一种实现方式是建立线性自连接单元(linear self-connections)和在自连接部分数值接近1的权重，叫做leaky units。但Leaky units的线性自连接权重是手动设置或设为参数，而目前最有效的方式gated RNNs是通过gates的调控，**允许线性自连接的权重在每一步都可以自我变化调节。**LSTM就是gated RNNs中的一个实现。

## LSTM的初步理解

LSTM(或者其他gated RNNs)是在标准RNN （![h_t= \phi(W_{xh} \cdot x_t + W_{hh} \cdot h_{t-1} + {b})](https://www.zhihu.com/equation?tex=h_t%3D+%5Cphi%28W_%7Bxh%7D+%5Ccdot+x_t+%2B+W_%7Bhh%7D+%5Ccdot+h_%7Bt-1%7D+%2B+%7Bb%7D%29)）的基础上装备了若干个控制数级(magnitude)的gates。可以理解成神经网络(RNN整体)中加入其他神经网络(gates)，而这些gates只是控制数级，控制信息的流动量。

**数学公式：**这里贴出基本LSTM的数学公式，看一眼就好，仅仅是为了让大家先留一个印象，不需要记住，不需要理解。

![](https://pic1.zhimg.com/v2-eb4824dc842bb2f73be2b8d297c51944_b.png)
尽管式子不算复杂，却包含很多知识，接下来就是逐步分析这些式子以及背后的道理。 比如![\odot](https://www.zhihu.com/equation?tex=%5Codot)的意义和使用原因，sigmoid的使用原因。

## **门(gate)的理解**

理解Gated RNNs的第一步就是明白gate到底起到什么作用。
- **物理意义**：gate本身可看成是十分有物理意义的一个神经网络。
- 输入：gate的输入是控制依据；
- 输出：gate的输出是值域为![(0,1)](https://www.zhihu.com/equation?tex=%280%2C1%29)的数值，表示该如何调节其他数据的数级的控制方式。
- **使用方式**：gate所产生的输出会用于控制其他数据的数级，相当于过滤器的作用。
- 类比图：可以把信息想象成水流，而gate就是控制多少水流可以流过。
![](https://pic1.zhimg.com/v2-5b968059df4831d80b6c049579c5ea18_b.jpg)- 例如：当用gate来控制向量![\left[\begin{matrix}20 & 5& 7 & 8 \\\end{matrix}\right]](https://www.zhihu.com/equation?tex=%5Cleft%5B%5Cbegin%7Bmatrix%7D20+%26+5%26+7+%26+8+%5C%5C%5Cend%7Bmatrix%7D%5Cright%5D)时，
- 若gate的输出为![\left[\begin{matrix}0.1 & 0.2& 0.9 & 0.5 \\\end{matrix}\right]](https://www.zhihu.com/equation?tex=%5Cleft%5B%5Cbegin%7Bmatrix%7D0.1+%26+0.2%26+0.9+%26+0.5+%5C%5C%5Cend%7Bmatrix%7D%5Cright%5D)时，原来的向量就会被对应元素相乘(element-wise)后变成：

![\left[\begin{matrix}20 & 5& 7 & 8 \\\end{matrix}\right]\odot \left[\begin{matrix}0.1 & 0.2& 0.9 & 0.5 \\\end{matrix}\right]](https://www.zhihu.com/equation?tex=%5Cleft%5B%5Cbegin%7Bmatrix%7D20+%26+5%26+7+%26+8+%5C%5C%5Cend%7Bmatrix%7D%5Cright%5D%5Codot+%5Cleft%5B%5Cbegin%7Bmatrix%7D0.1+%26+0.2%26+0.9+%26+0.5+%5C%5C%5Cend%7Bmatrix%7D%5Cright%5D)=![\left[\begin{matrix}20*0.1 & 5*0.2& 7*0.9 & 8*0.5 \\\end{matrix}\right]=\left[\begin{matrix}2 & 1& 6.3 & 4 \\\end{matrix}\right]](https://www.zhihu.com/equation?tex=%5Cleft%5B%5Cbegin%7Bmatrix%7D20%2A0.1+%26+5%2A0.2%26+7%2A0.9+%26+8%2A0.5+%5C%5C%5Cend%7Bmatrix%7D%5Cright%5D%3D%5Cleft%5B%5Cbegin%7Bmatrix%7D2+%26+1%26+6.3+%26+4+%5C%5C%5Cend%7Bmatrix%7D%5Cright%5D)
- 若gate的输出为![\left[\begin{matrix}0.5 & 0.5& 0.5 & 0.5 \\\end{matrix}\right]](https://www.zhihu.com/equation?tex=%5Cleft%5B%5Cbegin%7Bmatrix%7D0.5+%26+0.5%26+0.5+%26+0.5+%5C%5C%5Cend%7Bmatrix%7D%5Cright%5D)时，原来的向量就会被对应元素相乘(element-wise)后变成：

![\left[\begin{matrix}20 & 5& 7 & 8 \\\end{matrix}\right]\odot \left[\begin{matrix}0.5 & 0.5& 0.5 & 0.5 \\\end{matrix}\right]=\left[\begin{matrix}10 & 2.5& 3.5 & 4 \\\end{matrix}\right]](https://www.zhihu.com/equation?tex=%5Cleft%5B%5Cbegin%7Bmatrix%7D20+%26+5%26+7+%26+8+%5C%5C%5Cend%7Bmatrix%7D%5Cright%5D%5Codot+%5Cleft%5B%5Cbegin%7Bmatrix%7D0.5+%26+0.5%26+0.5+%26+0.5+%5C%5C%5Cend%7Bmatrix%7D%5Cright%5D%3D%5Cleft%5B%5Cbegin%7Bmatrix%7D10+%26+2.5%26+3.5+%26+4+%5C%5C%5Cend%7Bmatrix%7D%5Cright%5D)

- **控制依据**：明白了gate的输出后，剩下要确定以什么信息为控制依据，也就是什么是gate的输入。
- 例如：即便是LSTM也有很多个变种。一个变种方式是调控门的输入。例如下面两种gate：
- ![g= sigmoid(W_{xg} \cdot x_t + W_{hg} \cdot h_{t-1} + {b})](https://www.zhihu.com/equation?tex=g%3D+sigmoid%28W_%7Bxg%7D+%5Ccdot+x_t+%2B+W_%7Bhg%7D+%5Ccdot+h_%7Bt-1%7D+%2B+%7Bb%7D%29)：
- 这种gate的输入有当前的输入![x_t](https://www.zhihu.com/equation?tex=x_t)和上一时刻的隐藏状态![h_{t-1}](https://www.zhihu.com/equation?tex=h_%7Bt-1%7D)， 表示gate是将这两个信息流作为控制依据而产生输出的。
- ![g= sigmoid(W_{xg} \cdot x_t + W_{hg} \cdot h_{t-1} +W_{cg} \cdot c_{t-1}+ {b})](https://www.zhihu.com/equation?tex=g%3D+sigmoid%28W_%7Bxg%7D+%5Ccdot+x_t+%2B+W_%7Bhg%7D+%5Ccdot+h_%7Bt-1%7D+%2BW_%7Bcg%7D+%5Ccdot+c_%7Bt-1%7D%2B+%7Bb%7D%29)：
- 这种gate的输入有当前的输入![x_t ](https://www.zhihu.com/equation?tex=x_t+)和上一时刻的隐藏状态![h_{t-1}](https://www.zhihu.com/equation?tex=h_%7Bt-1%7D)，以及上一时刻的cell状态![c_{t-1}](https://www.zhihu.com/equation?tex=c_%7Bt-1%7D)， 表示gate是将这三个信息流作为控制依据而产生输出的。这种方式的LSTM叫做peephole connections。

## **LSTM的再次理解**

明白了gate之后再回过头来看LSTM的数学公式

**数学公式**：
![](https://pic4.zhimg.com/v2-e45fc2149e49856df73c73fc9b635db3_b.png)

- **gates：**先将前半部分的三个式子![i_t,f_t,o_t](https://www.zhihu.com/equation?tex=i_t%2Cf_t%2Co_t)统一理解。在LSTM中，网络首先构建了3个gates来控制信息的流通量。
- 注： 虽然gates的式子构成方式一样，但是注意3个gates式子![W](https://www.zhihu.com/equation?tex=W)和![b](https://www.zhihu.com/equation?tex=b)的下角标并不相同。它们有各自的物理意义，在网络学习过程中会产生不同的权重。
- 有了这3个gates后，接下来要考虑的就是如何用它们装备在普通的RNN上来控制信息流，而根据它们所用于控制信息流通的地点不同，它们又被分为：
- 输入门![i_t](https://www.zhihu.com/equation?tex=i_t)：控制有多少信息可以流入memory cell（第四个式子![c_t](https://www.zhihu.com/equation?tex=c_t)）。
- 遗忘门![f_t](https://www.zhihu.com/equation?tex=f_t)：控制有多少上一时刻的memory cell中的信息可以累积到当前时刻的memory cell中。
- 输出门![o_t](https://www.zhihu.com/equation?tex=o_t)：控制有多少当前时刻的memory cell中的信息可以流入当前隐藏状态![h_t](https://www.zhihu.com/equation?tex=h_t)中。
- 注：gates并不提供额外信息，gates只是起到限制信息的量的作用。因为gates起到的是过滤器作用，所以所用的激活函数是sigmoid而不是tanh。

- **信息流：**信息流的来源只有三处，当前的输入![x_t](https://www.zhihu.com/equation?tex=x_t)，上一时刻的隐藏状态![h_{t-1}](https://www.zhihu.com/equation?tex=h_%7Bt-1%7D)，上一时刻的cell状态![c_{t-1}](https://www.zhihu.com/equation?tex=c_%7Bt-1%7D)，其中![c_{t-1}](https://www.zhihu.com/equation?tex=c_%7Bt-1%7D)是额外制造出来、可线性自连接的单元（请回想起leaky units）。真正的信息流来源可以说只有当前的输入![x_t](https://www.zhihu.com/equation?tex=x_t)，上一时刻的隐藏状态![h_{t-1}](https://www.zhihu.com/equation?tex=h_%7Bt-1%7D)两处。三个gates的控制依据，以及数据的更新都是来源于这两处。
- 分析了gates和信息流后，再分析剩下的两个等式，来看LSTM是如何累积历史信息和计算隐藏状态![h](https://www.zhihu.com/equation?tex=h)的。
- **历史信息累积：**
- 式子：![c _t = f_t \odot c_{t - 1} + i_t \odot tanh(W_{xc} x_t + W_{hc}h_{t-1} + b_c)](https://www.zhihu.com/equation?tex=c+_t+%3D+f_t+%5Codot+c_%7Bt+-+1%7D+%2B+i_t+%5Codot+tanh%28W_%7Bxc%7D+x_t+%2B+W_%7Bhc%7Dh_%7Bt-1%7D+%2B+b_c%29)
- 其中![new=tanh(W_{xc} x_t + W_{hc}h_{t-1} + b_c)](https://www.zhihu.com/equation?tex=new%3Dtanh%28W_%7Bxc%7D+x_t+%2B+W_%7Bhc%7Dh_%7Bt-1%7D+%2B+b_c%29)是本次要累积的信息来源。
- 改写：![c _t = f_t \odot c_{t - 1} + i_t \odot new](https://www.zhihu.com/equation?tex=c+_t+%3D+f_t+%5Codot+c_%7Bt+-+1%7D+%2B+i_t+%5Codot+new)

所以历史信息的累积是并不是靠隐藏状态![h](https://www.zhihu.com/equation?tex=h)自身，而是依靠memory cell这个自连接来累积。 在累积时，靠遗忘门来限制上一时刻的memory cell的信息，并靠输入门来限制新信息。并且真的达到了leaky units的思想，memory cell的自连接是线性的累积。

- **当前隐藏状态的计算：**如此大费周章的最终任然是同普通RNN一样要计算当前隐藏状态。
- 式子：![h_t = o_t \odot tanh(c_t)](https://www.zhihu.com/equation?tex=h_t+%3D+o_t+%5Codot+tanh%28c_t%29)
- 当前隐藏状态![h_t](https://www.zhihu.com/equation?tex=h_t)是从![c_t](https://www.zhihu.com/equation?tex=c_t)计算得来的，因为![c_t](https://www.zhihu.com/equation?tex=c_t)是以线性的方式自我更新的，所以先将其加入带有非线性功能的![tanh(c_t)](https://www.zhihu.com/equation?tex=tanh%28c_t%29)。 随后再靠输出门![o_t](https://www.zhihu.com/equation?tex=o_t)的过滤来得到当前隐藏状态![h_t](https://www.zhihu.com/equation?tex=h_t)。

## **普通RNN与LSTM的比较**

下面为了加深理解循环神经网络的核心，再来和YJango一起比较一下普通RNN和LSTM的区别。
- **比较公式：**最大的区别是多了三个神经网络(gates)来控制数据的流通。
- **普通RNN：**![h_t= tanh(W_{xh} \cdot x_t + W_{hh} \cdot h_{t-1} + {b})](https://www.zhihu.com/equation?tex=h_t%3D+tanh%28W_%7Bxh%7D+%5Ccdot+x_t+%2B+W_%7Bhh%7D+%5Ccdot+h_%7Bt-1%7D+%2B+%7Bb%7D%29)
- **LSTM：**![h _t = o_t \odot tanh(f_t \odot c_{t - 1} + i_t \odot tanh(W_{xc} x_t + W_{hc}h_{t-1} + b_c))](https://www.zhihu.com/equation?tex=h+_t+%3D+o_t+%5Codot+tanh%28f_t+%5Codot+c_%7Bt+-+1%7D+%2B+i_t+%5Codot+tanh%28W_%7Bxc%7D+x_t+%2B+W_%7Bhc%7Dh_%7Bt-1%7D+%2B+b_c%29%29)
- **比较：**二者的信息来源都是![tanh(W_{xh} \cdot x_t + W_{hh} \cdot h_{t-1} + {b})](https://www.zhihu.com/equation?tex=tanh%28W_%7Bxh%7D+%5Ccdot+x_t+%2B+W_%7Bhh%7D+%5Ccdot+h_%7Bt-1%7D+%2B+%7Bb%7D%29)，** 不同的是LSTM靠3个gates将信息的积累建立在线性自连接的memory cell之上，并靠其作为中间物来计算当前![h_t ](https://www.zhihu.com/equation?tex=h_t+)。**
- **示图比较：**图片来自[Understanding LSTM](https://link.zhihu.com/?target=http%3A//colah.github.io/posts/2015-08-Understanding-LSTMs/)，强烈建议一并阅读。
- **普通RNN：**
**![](https://pic2.zhimg.com/v2-bce0d99e8bc969fa3c2ffa99f93935c5_b.png)**

- **LSTM：**加号圆圈表示线性相加，乘号圆圈表示用gate来过滤信息。
![](https://pic2.zhimg.com/v2-703dffbefaddcf8dcdb39ccf589312e9_b.jpg)

- **比较：**新信息从黄色的tanh处，线性累积到memory cell之中后，又从红色的tanh处加入非线性并返回到了隐藏状态![h_t](https://www.zhihu.com/equation?tex=h_t)的计算中。

> **LSTM靠3个gates将信息的积累建立在线性自连接的权重接近1的memory cell之上，并靠其作为中间物来计算当前![h_t](https://www.zhihu.com/equation?tex=h_t)。**

## **LSTM的类比**

对于用LSTM来实现RNN的记忆，可以类比我们所用的手机（仅仅是为了方便记忆，并非一一对应）。
![](https://pic3.zhimg.com/v2-3cbda7436b42c7b05c8a11868794e2b2_b.jpg)

普通RNN好比是手机屏幕，而LSTM-RNN好比是手机膜。

大量非线性累积历史信息会造成梯度消失(梯度爆炸)好比是不断使用后容易使屏幕刮花。

而LSTM将信息的积累建立在线性自连接的memory cell之上，并靠其作为中间物来计算当前![h_t](https://www.zhihu.com/equation?tex=h_t)好比是用手机屏幕膜作为中间物来观察手机屏幕。

输入门、遗忘门、输出门的过滤作用好比是手机屏幕膜的反射率、吸收率、透射率三种性质。

## Gated RNNs的变种

需要再次明确的是，神经网络之所以被称之为网络是因为它可以非常自由的创建合理的连接。而上面所介绍的LSTM也只是最基本的LSTM。只要遵守几个关键点，读者可以根据需求设计自己的Gated RNNs，而至于在不同任务上的效果需要通过实验去验证。下面就简单介绍YJango所理解的几个Gated RNNs的变种的设计方向。
- **信息流**：标准的RNN的信息流有两处：input输入和hidden state隐藏状态。

但往往信息流并非只有两处，即便是有两处，也可以拆分成多处，并通过明确多处信息流之间的结构关系来加入先验知识，减少训练所需数据量，从而提高网络效果。

例如：[Tree-LSTM](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1503.00075.pdf)在具有此种结构的自然语言处理任务中的应用。
![](https://pic2.zhimg.com/v2-b6cc00b99f001d39d97be0932bd52575_b.jpg)

- **gates的控制方式**：与LSTM一样有名的是Gated Recurrent Unit (GRU)，而GRU使用gate的方式就与LSTM的不同，GRU只用了两个gates，将LSTM中的输入门和遗忘门合并成了更新门。并且并不把线性自更新建立在额外的memory cell上，而是直接线性累积建立在隐藏状态上，并靠gates来调控。
![](https://pic1.zhimg.com/v2-eb21a394a6bc08394a26b60707471c98_b.png)

- **gates的控制依据**：上文所介绍的LSTM中的三个gates所使用的控制依据都是![W x_t + Wh_{t-1}](https://www.zhihu.com/equation?tex=W+x_t+%2B+Wh_%7Bt-1%7D)，但是可以通过与memory cell的连接来增加控制依据或者删除某个gate的![W x_t](https://www.zhihu.com/equation?tex=W+x_t)或![Wh_{t-1}](https://www.zhihu.com/equation?tex=Wh_%7Bt-1%7D)来缩减控制依据。比如去掉上图中![z_t=sigmoid(W_z\cdot [h_{t-1},x_t])](https://www.zhihu.com/equation?tex=z_t%3Dsigmoid%28W_z%5Ccdot+%5Bh_%7Bt-1%7D%2Cx_t%5D%29)中的![h_{t-1}](https://www.zhihu.com/equation?tex=h_%7Bt-1%7D)从而变成![z_t=sigmoid(W_z\cdot h_{t-1})](https://www.zhihu.com/equation?tex=z_t%3Dsigmoid%28W_z%5Ccdot+h_%7Bt-1%7D%29)

介绍完[《循环神经网络——实现LSTM》](https://link.zhihu.com/?target=https%3A//yjango.gitbooks.io/superorganism/content/lstmgru.html)后， 接下来的第三篇[《循环神经网络——代码》](https://link.zhihu.com/?target=https%3A//yjango.gitbooks.io/superorganism/content/%25E4%25BB%25A3%25E7%25A0%2581%25E6%25BC%2594%25E7%25A4%25BA.html)就是用tensorflow从头来实现网络内容。


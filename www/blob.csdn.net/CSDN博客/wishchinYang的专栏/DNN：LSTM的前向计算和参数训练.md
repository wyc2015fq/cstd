# DNN：LSTM的前向计算和参数训练 - wishchinYang的专栏 - CSDN博客
2018年11月29日 17:26:55[wishchin](https://me.csdn.net/wishchin)阅读数：432
原文-LSTM的反向传播：[深度学习(6)-长短期网路](https://zybuluo.com/hanbingtao/note/581764)；此处仅摘抄一小段，建议拜访全文。
LSTM的参数训练：[https://www.jianshu.com/p/dcec3f07d3b5](https://www.jianshu.com/p/dcec3f07d3b5)；LSTM的参数训练和前向计算比RNNs还是稍微复杂一些。
## 长短时记忆网络的前向计算
前面描述的开关是怎样在算法中实现的呢？这就用到了**门（gate）**的概念。门实际上就是一层**全连接层**，它的输入是一个向量，输出是一个0到1之间的实数向量。假设W是门的权重向量，**b**是偏置项，那么门可以表示为：
![](https://img-blog.csdnimg.cn/20181129164448964.png)
门的使用，就是用门的输出向量按元素乘以我们需要控制的那个向量。因为门的输出是0到1之间的实数向量，那么，当门输出为0时，任何向量与之相乘都会得到0向量，这就相当于啥都不能通过；输出为1时，任何向量与之相乘都不会有任何改变，这就相当于啥都可以通过。因为![](https://img-blog.csdnimg.cn/20181129164551518.png)（也就是sigmoid函数）的值域是(0,1)，所以门的状态都是半开半闭的。
LSTM用两个门来控制单元状态c的内容，一个是**遗忘门（forget gate）**，它决定了上一时刻的单元状态![](https://img-blog.csdnimg.cn/20181129164611781.png)有多少保留到当前时刻![](https://img-blog.csdnimg.cn/20181129164623941.png)；另一个是**输入门（input gate）**，它决定了当前时刻网络的输入有多少保存到单元状态![](https://img-blog.csdnimg.cn/20181129164631815.png)。LSTM用**输出门（output gate）**来控制单元状态![](https://img-blog.csdnimg.cn/20181129164637489.png)有多少输出到LSTM的当前输出值![](https://img-blog.csdnimg.cn/20181129164651986.png)。
我们先来看一下遗忘门：
![](https://img-blog.csdnimg.cn/2018112916470691.png)
上式中，![](https://img-blog.csdnimg.cn/20181129164729218.png)是遗忘门的权重矩阵，![](https://img-blog.csdnimg.cn/2018112916475482.png)表示把两个向量连接成一个更长的向量，![](https://img-blog.csdnimg.cn/20181129164806195.png)是遗忘门的偏置项，![](https://img-blog.csdnimg.cn/20181129164814412.png)是sigmoid函数。如果输入的维度是![](https://img-blog.csdnimg.cn/20181129164831248.png)，隐藏层的维度是![](https://img-blog.csdnimg.cn/20181129164841875.png)，单元状态的维度是![](https://img-blog.csdnimg.cn/20181129164854766.png)（通常![](https://img-blog.csdnimg.cn/20181129164904880.png)），则遗忘门的权重矩阵![](https://img-blog.csdnimg.cn/20181129164915180.png)维度是![](https://img-blog.csdnimg.cn/20181129164927411.png)。事实上，权重矩阵![](https://img-blog.csdnimg.cn/20181129164915180.png)都是两个矩阵拼接而成的：一个是![](https://img-blog.csdnimg.cn/20181129165029985.png)，它对应着输入项![](https://img-blog.csdnimg.cn/20181129165041236.png)，其维度为![](https://img-blog.csdnimg.cn/20181129165101894.png)；一个是![](https://img-blog.csdnimg.cn/20181129165114231.png)，它对应着输入项![](https://img-blog.csdnimg.cn/20181129165125361.png)，其维度为![](https://img-blog.csdnimg.cn/20181129165134749.png)。可以写为：
![](https://img-blog.csdnimg.cn/20181129165153356.png)
下图显示了遗忘门的计算：
![](http://upload-images.jianshu.io/upload_images/2256672-c7f7ca0aa64b562f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
接下来看看输入门：
![](https://img-blog.csdnimg.cn/20181129165220514.png)
上式中，![](https://img-blog.csdnimg.cn/20181129165248473.png)是输入门的权重矩阵，![](https://img-blog.csdnimg.cn/20181129165259179.png)是输入门的偏置项。下图表示了输入门的计算：
![](http://upload-images.jianshu.io/upload_images/2256672-89529fa23d9c8a7d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
接下来，我们计算用于描述当前输入的单元状态![](https://img-blog.csdnimg.cn/20181129165318580.png)，它是根据上一次的输出和本次输入来计算的：
![](https://img-blog.csdnimg.cn/20181129165330542.png)
下图是的计算：
![](http://upload-images.jianshu.io/upload_images/2256672-73a0246cafc1d10d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
现在，我们计算当前时刻的单元状态![](https://img-blog.csdnimg.cn/20181129165416797.png)。它是由上一次的单元状态![](https://img-blog.csdnimg.cn/20181129165427225.png)按元素乘以遗忘门![](https://img-blog.csdnimg.cn/20181129165436393.png)，再用当前输入的单元状态![](https://img-blog.csdnimg.cn/20181129165451399.png)按元素乘以输入门![](https://img-blog.csdnimg.cn/20181129165503621.png)，再将两个积加和产生的：
![](https://img-blog.csdnimg.cn/20181129165515365.png)
符号o表示**按元素乘**。下图是![](https://img-blog.csdnimg.cn/20181129165535859.png)的计算：
![](http://upload-images.jianshu.io/upload_images/2256672-5c766f3d734334b1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这样，我们就把LSTM关于当前的记忆和长期的记忆组合在一起，形成了新的单元状态。由于遗忘门的控制，它可以保存很久很久之前的信息，由于输入门的控制，它又可以避免当前无关紧要的内容进入记忆。下面，我们要看看输出门，它控制了长期记忆对当前输出的影响：
![](https://img-blog.csdnimg.cn/20181129165552476.png)
下图表示输出门的计算：
![](http://upload-images.jianshu.io/upload_images/2256672-fd4d91d1b68b3759.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
LSTM最终的输出，是由输出门和单元状态共同确定的：
![](https://img-blog.csdnimg.cn/20181129165843984.png)
下图表示LSTM最终输出的计算：
![](http://upload-images.jianshu.io/upload_images/2256672-7ea82e4f1ac6cd75.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**式1**到**式6**就是LSTM前向计算的全部公式。至此，我们就把LSTM前向计算讲完了。
## 长短时记忆网络的训练
熟悉我们这个系列文章的同学都清楚，训练部分往往比前向计算部分复杂多了。LSTM的前向计算都这么复杂，那么，可想而知，它的训练算法一定是非常非常复杂的。现在只有做几次深呼吸，再一头扎进公式海洋吧。
### LSTM训练算法框架
LSTM的训练算法仍然是反向传播算法，对于这个算法，我们已经非常熟悉了。主要有下面三个步骤：
- 前向计算每个神经元的输出值，对于LSTM来说，即![](https://img-blog.csdnimg.cn/20181129170152703.png)五个向量的值。计算方法已经在上一节中描述过了。
- 反向计算每个神经元的**误差项**![](https://img-blog.csdnimg.cn/20181129170216788.png)值。与**循环神经网络**一样，LSTM误差项的反向传播也是包括两个方向：一个是沿时间的反向传播，即从当前t时刻开始，计算每个时刻的误差项；一个是将误差项向上一层传播。
- 根据相应的误差项，计算每个权重的梯度。
### 关于公式和符号的说明
首先，我们对推导中用到的一些公式、符号做一下必要的说明。
接下来的推导中，我们设定gate的激活函数为sigmoid函数，输出的激活函数为tanh函数。他们的导数分别为：
![](https://img-blog.csdnimg.cn/20181129170252821.png)
从上面可以看出，sigmoid和tanh函数的导数都是原函数的函数。这样，我们一旦计算原函数的值，就可以用它来计算出导数的值。
LSTM需要学习的参数共有8组，分别是：遗忘门的权重矩阵![](https://img-blog.csdnimg.cn/20181129170326559.png)和偏置项![](https://img-blog.csdnimg.cn/2018112917034051.png)、输入门的权重矩阵![](https://img-blog.csdnimg.cn/20181129170350166.png)和偏置项![](https://img-blog.csdnimg.cn/20181129170400471.png)、输出门的权重矩阵![](https://img-blog.csdnimg.cn/20181129170411973.png)和偏置项![](https://img-blog.csdnimg.cn/20181129170420939.png)，以及计算单元状态的权重矩阵![](https://img-blog.csdnimg.cn/20181129170429571.png)和偏置项![](https://img-blog.csdnimg.cn/20181129170440618.png)。因为权重矩阵的两部分在反向传播中使用不同的公式，因此在后续的推导中，权重矩阵![](https://img-blog.csdnimg.cn/20181129170500218.png)都将被写为分开的两个矩阵：![](https://img-blog.csdnimg.cn/20181129170510534.png)、![](https://img-blog.csdnimg.cn/20181129170521211.png)。
我们解释一下按元素乘符号o。当o作用于两个**向量**时，运算如下：
![](https://img-blog.csdnimg.cn/20181129170609857.png)
当o作用于一个**向量**和一个**矩阵**时，运算如下：
![](https://img-blog.csdnimg.cn/20181129170643129.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
当o作用于两个**矩阵**时，两个矩阵对应位置的元素相乘。按元素乘可以在某些情况下简化矩阵和向量运算。例如，当一个对角矩阵右乘一个矩阵时，相当于用对角矩阵的对角线组成的向量按元素乘那个矩阵：
![](https://img-blog.csdnimg.cn/20181129170707695.png)
当一个行向量右乘一个对角矩阵时，相当于这个行向量按元素乘那个矩阵对角线组成的向量：
![](https://img-blog.csdnimg.cn/20181129170732211.png)
上面这两点，在我们后续推导中会多次用到。
在t时刻，LSTM的输出值为![](https://img-blog.csdnimg.cn/20181129170755492.png)。我们定义t时刻的误差项![](https://img-blog.csdnimg.cn/20181129170807660.png)为：![](https://img-blog.csdnimg.cn/20181129170816655.png)
注意，和前面几篇文章不同，我们这里假设误差项是损失函数对输出值的导数，而不是对加权输入![](https://img-blog.csdnimg.cn/20181129170852250.png)的导数。因为LSTM有四个加权输入，分别对应![](https://img-blog.csdnimg.cn/20181129170839353.png)，我们希望往上一层传递一个误差项而不是四个。但我们仍然需要定义出这四个加权输入，以及他们对应的误差项。
![](https://img-blog.csdnimg.cn/20181129170914248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
### 误差项沿时间的反向传递
沿时间反向传递误差项，就是要计算出t-1时刻的误差项![](https://img-blog.csdnimg.cn/20181129170944625.png)。
![](https://img-blog.csdnimg.cn/20181129170956387.png)
我们知道，![](https://img-blog.csdnimg.cn/20181129171020835.png)是一个Jacobian矩阵。如果隐藏层h的维度是N的话，那么它就是一个![](https://img-blog.csdnimg.cn/20181129171031701.png)矩阵。为了求出它，我们列出![](https://img-blog.csdnimg.cn/20181129171043845.png)的计算公式，即前面的**式6**和**式4**：
![](https://img-blog.csdnimg.cn/20181129171101425.png)
显然，![](https://img-blog.csdnimg.cn/20181129171139463.png)都是![](https://img-blog.csdnimg.cn/20181129171128765.png)的函数，那么，利用全导数公式可得：
![](https://img-blog.csdnimg.cn/20181129171156490.png)
下面，我们要把**式7**中的每个偏导数都求出来。根据**式6**，我们可以求出：
![](https://img-blog.csdnimg.cn/2018112917121620.png)
根据**式4**，我们可以求出：
![](https://img-blog.csdnimg.cn/20181129171253406.png)
因为：
![](https://img-blog.csdnimg.cn/20181129171314119.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
我们很容易得出：
![](https://img-blog.csdnimg.cn/2018112917133636.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
将上述偏导数带入到**式7**，我们得到：
![](https://img-blog.csdnimg.cn/20181129171541826.png)
根据![](https://img-blog.csdnimg.cn/20181129171614875.png)的定义，可知：
![](https://img-blog.csdnimg.cn/20181129171601318.png)
**式8**到**式12**就是将误差沿时间反向传播一个时刻的公式。有了它，我们可以写出将误差项向前传递到任意k时刻的公式：
![](https://img-blog.csdnimg.cn/20181129171635210.png)
### 将误差项传递到上一层
我们假设当前为第l层，定义l-1层的误差项是误差函数对l-1层**加权输入**的导数，即：![](https://img-blog.csdnimg.cn/20181129171701724.png)
本次LSTM的输入![](https://img-blog.csdnimg.cn/20181129171729403.png)由下面的公式计算：
![](https://img-blog.csdnimg.cn/20181129171737761.png)
上式中，![](https://img-blog.csdnimg.cn/20181129171802698.png)表示第l-1层的**激活函数**。
因为![](https://img-blog.csdnimg.cn/20181129171824920.png)都是![](https://img-blog.csdnimg.cn/20181129171836270.png)的函数，![](https://img-blog.csdnimg.cn/20181129171837914.png)又是![](https://img-blog.csdnimg.cn/20181129171846788.png)的函数，因此，要求出E对![](https://img-blog.csdnimg.cn/20181129171852842.png)的导数，就需要使用全导数公式：
![](https://img-blog.csdnimg.cn/20181129171905866.png)
**式14**就是将误差传递到上一层的公式。
### 权重梯度的计算
对于![](https://img-blog.csdnimg.cn/20181129172014114.png)的权重梯度，我们知道它的梯度是各个时刻梯度之和（证明过程请参考文章[零基础入门深度学习(5) - 循环神经网络](https://zybuluo.com/hanbingtao/note/541458)），我们首先求出它们在t时刻的梯度，然后再求出他们最终的梯度。
我们已经求得了误差项![](https://img-blog.csdnimg.cn/20181129172029947.png)，很容易求出t时刻的![](https://img-blog.csdnimg.cn/2018112917204738.png)：
![](https://img-blog.csdnimg.cn/20181129172100417.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
将各个时刻的梯度加在一起，就能得到最终的梯度：
![](https://img-blog.csdnimg.cn/20181129172120189.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
对于偏置项![](https://img-blog.csdnimg.cn/20181129172136369.png)的梯度，也是将各个时刻的梯度加在一起。下面是各个时刻的偏置项梯度：
![](https://img-blog.csdnimg.cn/20181129172201839.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
下面是最终的偏置项梯度，即将各个时刻的偏置项梯度加在一起：
![](https://img-blog.csdnimg.cn/20181129172223882.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
对于![](https://img-blog.csdnimg.cn/20181129172254485.png)的权重梯度，只需要根据相应的误差项直接计算即可：
![](https://img-blog.csdnimg.cn/20181129172305187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
以上就是LSTM的训练算法的全部公式。因为这里面存在很多重复的模式，仔细看看，会发觉并不是太复杂。
当然，LSTM存在着相当多的变体，读者可以在互联网上找到很多资料。因为大家已经熟悉了基本LSTM的算法，因此理解这些变体比较容易，因此本文就不再赘述了。
## 长短时记忆网络的实现
> 
完整代码请参考GitHub: [https://github.com/hanbt/learn_dl/blob/master/lstm.py](https://github.com/hanbt/learn_dl/blob/master/lstm.py) (python2.7)
在下面的实现中，LSTMLayer的参数包括输入维度、输出维度、隐藏层维度，单元状态维度等于隐藏层维度。gate的激活函数为sigmoid函数，输出的激活函数为tanh。
### 激活函数的实现
我们先实现两个激活函数：sigmoid和tanh。
```python
class SigmoidActivator(object):
    def forward(self, weighted_input):
        return 1.0 / (1.0 + np.exp(-weighted_input))
    def backward(self, output):
        return output * (1 - output)
class TanhActivator(object):
    def forward(self, weighted_input):
        return 2.0 / (1.0 + np.exp(-2 * weighted_input)) - 1.0
    def backward(self, output):
        return 1 - output * output
```
LSTM初始化、前向计算、反向传播（误差项、梯度项）、梯度下降法、梯度检查Python代码的实现；
请继续参考原文：
[https://zybuluo.com/hanbingtao/note/581764](https://zybuluo.com/hanbingtao/note/581764)

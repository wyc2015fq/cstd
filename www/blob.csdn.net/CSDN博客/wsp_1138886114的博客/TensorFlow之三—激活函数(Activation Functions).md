# TensorFlow之三—激活函数(Activation Functions) - wsp_1138886114的博客 - CSDN博客





2019年01月25日 21:52:03[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：63标签：[TensorFlow之三—激活函数](https://so.csdn.net/so/search/s.do?q=TensorFlow之三—激活函数&t=blog)
个人分类：[深度学习](https://blog.csdn.net/wsp_1138886114/article/category/7729523)












### 文章目录
- [1. sigmoid函数](#1_sigmoid_4)
- [2. tanh 双曲正切函数](#2_tanh__8)
- [3. relu 函数](#3_relu__14)
- [4. ELU函数](#4_ELU_21)
- [5. PReLU函数](#5_PReLU_25)
- [6. 其它函数](#6__30)



在TensorFlow的训练中，为了更好的模拟或者学习更多的特征，在线性函数中引入了非线性，又名激活函数。如果你对激活函数不甚了解，你可以点击[]()。这一章我们主要将一些常用的激活函数。[详情请点击](https://en.wikipedia.org/wiki/Activation_function)

一些常用的激活函数如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190119202408530.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)


#### 1. sigmoid函数

由上图第一个可知，导数从 0 开始很快就又趋近于 0 了，易造成“梯度消失”现象

调用函数：`tf.sigmoid(x, name = None) == tf.nn.sigmoid(x, name = None)`

#### 2. tanh 双曲正切函数

tanh函数和sigmod函数的曲线是比较相近的，由上图。首先相同的是，这两个函数在输入很大或是很小的时候，输出都几乎平滑，梯度很小，不利于权重更新；不同的是输出区间，tanh的输出区间是在(-1,1)之间，而且整个函数是以0为中心的，这个特点比sigmod的好。它把输入映射到 (-1, 1) 区间，但仍然会导致梯度消失问题 !

调用函数：`tf.tanh(x, name = None) == tf.nn.tanh(x, name = None)`

公式：$$f(x) = \frac{e^x-e^{-x}}{e^x+e^{-x}}$$

#### 3. relu 函数

比 sigmoid/tanh 收敛的更快，其导数在其权重和(z) 大于 0 的时候为 1，从而误差可以很好的传播，权重可以正常更新。

但是，其导数在其权重和(z) 小于 0 的时候为 0，会导致梯度值为0（神经元死亡），从而权重无法正常更新输出具有偏移现象，即输出均值恒大于零。

调用函数：`tf.nn.relu(features, name=None)`

公式：y = max(x, 0)
#### 4. ELU函数

函数图如上图。

ELU函数是针对ReLU函数的一个改进型，相比于ReLU函数，在输入为负数的情况下，是有一定的输出的，而且这部分输出还具有一定的抗干扰能力。这样可以消除ReLU死掉的问题，不过还是有梯度饱和和指数运算的问题。

#### 5. PReLU函数

函数图如上图。

PReLU也是针对ReLU的一个改进型，在负数区域内，PReLU有一个很小的斜率，这样也可以避免ReLU死掉的问题。相比于ELU，PReLU在负数区域内是线性运算，斜率虽然小，但是不会趋于0，这算是一定的优势吧。

我们看PReLU的公式，里面的参数α一般是取0~1之间的数，而且一般还是比较小的，如零点零几。当α=0.01时，我们叫**PReLU**为 **Leaky ReLU**，算是PReLU的一种特殊情况吧。

#### 6. 其它函数

`tf.nn.relu6(features, name = None)`

公式：$\min ( \max(features, 0), 6 )$

`tf.nn.softplus(features, name = None)`

公式： $\log ( \exp( features ) + 1)$

`tf.nn.bias_add(value, bias, name = None)` 偏执项





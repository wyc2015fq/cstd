# 9987 用Theano实现Nesterov momentum的正确姿势 - 知乎
# 

　　这篇文章跟语言学无关，它讲的是我这周在科研中实现双向递归神经网络（bidirectional recurrent neural networks）的一点经验。

## 1. 神经网络简介

　　深层神经网络（deep neural networks，DNN）是近年来机器学习领域的天之骄子，从分类到回归，它无所不能。它的结构如下图所示：

![](https://pic1.zhimg.com/139722b952565e6b37959662e5b0123c_b.jpg)上图中，每个方框代表一个向量，![x](https://www.zhihu.com/equation?tex=x)是输入，![y](https://www.zhihu.com/equation?tex=y)是输出，各个![h_i](https://www.zhihu.com/equation?tex=h_i)是隐藏层。箭头是信息传递的方向，每个箭头上带有一个权重矩阵，它们是网络的参数。除输入外，每个方框按照如下规则来计算自己的值：
　　　　　　・![h_i = \sigma(W_ih_{i-1}+b_i)](https://www.zhihu.com/equation?tex=h_i+%3D+%5Csigma%28W_ih_%7Bi-1%7D%2Bb_i%29)
其中![b_i](https://www.zhihu.com/equation?tex=b_i)是![h_i](https://www.zhihu.com/equation?tex=h_i)的偏置向量，也是网络参数；![\sigma](https://www.zhihu.com/equation?tex=%5Csigma)是一个非线性函数（常用的有[sigmoid](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Sigmoid_function)、[tanh](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Hyperbolic_function)、[ReLU](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Rectifier_%28neural_networks%29)等等），正是这里的非线性让神经网络无比强大。

　　上面这种结构的神经网络称为前馈神经网络（feed-forward neural networks），它能够处理单个的向量。但有些时候，我们需要处理向量序列，这就要用到递归神经网络（recurrent neural networks，RNN）。RNN具有如下的二维结构，纵向是层次，横向是时间：

![](https://pic1.zhimg.com/bb8e6e663683866877d142e3ede6fdb4_b.jpg)与前馈神经网络相比，RNN的每个隐藏层要从左、下两个方向接收信息，其中“左”代表过去。各个隐藏层需要初始化（最左边的![h_{0i}](https://www.zhihu.com/equation?tex=h_%7B0i%7D)），这些向量也是网络参数。

　　上面这种RNN是单向（unidirectional）的，因为信息只能从过去流向未来，![x_3](https://www.zhihu.com/equation?tex=x_3)包含的信息不能用于计算![y_2](https://www.zhihu.com/equation?tex=y_2)。而很多时候，未来的信息有助于当前输出的计算，例如在语音识别中，我们常常听到下一秒才知道这一秒说了什么。为了允许信息的逆向流动，人们发明了双向递归神经网络（bidirectional RNN），其结构如下：

![](https://pic1.zhimg.com/20e48e6cc81192f94b8dbd8efdc8213c_b.jpg)单向RNN中的所有隐藏层被复制了一份，沿时间方向的箭头被反了过来。不仅如此，相邻层之间的正向单元（黄色）和反向单元（浅蓝色）之间也建立了连接。好乱有木有！

## 2. 梯度下降法与Nesterov momentum

　　别看神经网络结构那么吓人，它说白了就是一个函数![y=f(x)](https://www.zhihu.com/equation?tex=y%3Df%28x%29)。在前馈神经网络中，![x](https://www.zhihu.com/equation?tex=x)和![y](https://www.zhihu.com/equation?tex=y)都是向量；在递归神经网络中，它们是矩阵。函数![y=f(x)](https://www.zhihu.com/equation?tex=y%3Df%28x%29)中包含了许多参数，包络所有箭头上的权重矩阵、各个隐藏层和输出层的偏置向量、各隐藏层的初始化向量。我们把它们合起来记为![w](https://www.zhihu.com/equation?tex=w)。

　　神经网络的训练，就是要找到一组比较好的![w](https://www.zhihu.com/equation?tex=w)，让神经网络的输出与我们期望的输出相近。把训练数据的输入![x](https://www.zhihu.com/equation?tex=x)喂给网络，可以得到输入![y](https://www.zhihu.com/equation?tex=y)；把它与训练数据中的标准输出比较，可以求出误差。这个误差是网络参数![w](https://www.zhihu.com/equation?tex=w)的函数，记作![L(w)](https://www.zhihu.com/equation?tex=L%28w%29)。训练神经网络，就是要找到![L(w)](https://www.zhihu.com/equation?tex=L%28w%29)的最小值，实在不行找个极小值也行。

.　　![L(w)](https://www.zhihu.com/equation?tex=L%28w%29)的形式当然很复杂，不过它有一个好，就是可导。求一个可导函数的极小值，常常用梯度下降法。![L(w)](https://www.zhihu.com/equation?tex=L%28w%29)的梯度![\nabla L(w)](https://www.zhihu.com/equation?tex=%5Cnabla+L%28w%29)是一个向量，它的方向表示了函数值上升最快的方向，大小是该方向上![L(w)](https://www.zhihu.com/equation?tex=L%28w%29)的方向导数。梯度下降法就是从任意的参数初值![w_0](https://www.zhihu.com/equation?tex=w_0)出发，每次沿梯度的反方向走一步，步子的大小梯度大小成正比。写成数学表达式，就是![w_{t+1} = w_t - \lambda \nabla L(w_t)](https://www.zhihu.com/equation?tex=w_%7Bt%2B1%7D+%3D+w_t+-+%5Clambda+%5Cnabla+L%28w_t%29)，其中比例系数![\lambda](https://www.zhihu.com/equation?tex=%5Clambda)称为学习速率（learning rate）。

　　下面是梯度下降法的示意图。不同的颜色代表函数值的大小，颜色的分界线是等高线，梯度与等高线垂直。
![](https://pic2.zhimg.com/9afc32917f3550c9398a39054514f075_b.jpg)　　有人觉得，在![L(w)](https://www.zhihu.com/equation?tex=L%28w%29)的地形并不很崎岖的情况下，相邻步的梯度方向应该差不多，因此在走每一步的时候，除了考虑当前位置梯度的方向以外，还可以沿上一步的方向多走一段距离。形象的理解，就是每一步都保留着上一步的“冲劲”——这正是这种方法的英文名momentum的来源，而中文名我觉得叫做“惯性”比较好。带惯性的梯度下降法（下文简称“惯性法”）的数学表达式为：

　　　　　　・![\delta_{t+1} = \mu \delta_t - \lambda \nabla L(w_t)](https://www.zhihu.com/equation?tex=%5Cdelta_%7Bt%2B1%7D+%3D+%5Cmu+%5Cdelta_t+-+%5Clambda+%5Cnabla+L%28w_t%29)
　　　　　　・![w_{t+1} = w_t + \delta_{t+1}](https://www.zhihu.com/equation?tex=w_%7Bt%2B1%7D+%3D+w_t+%2B+%5Cdelta_%7Bt%2B1%7D)
这里，![\delta_t](https://www.zhihu.com/equation?tex=%5Cdelta_t)是第![t](https://www.zhihu.com/equation?tex=t)步的位移，初始值取![\delta_0 = 0](https://www.zhihu.com/equation?tex=%5Cdelta_0+%3D+0)；![\mu](https://www.zhihu.com/equation?tex=%5Cmu)可以称为“惯性系数”，它位于0和1之间，控制了“冲劲”的衰减快慢。惯性法的示意图如下：

![](https://pic4.zhimg.com/b16c833a772d3743784cc02e6a2ae37b_b.jpg)　　上面的惯性法是学术界很常用的做法。但有一个叫Nesterov的人提出：在走第![t+1](https://www.zhihu.com/equation?tex=t%2B1)步的时候，既然已经知道要先走一步![\mu\delta_t](https://www.zhihu.com/equation?tex=%5Cmu%5Cdelta_t)了，为什么还要“刻舟求剑”式地在原来的![w_t](https://www.zhihu.com/equation?tex=w_t)处求梯度呢？在走了这一步之后的![w_t + \mu\delta_t](https://www.zhihu.com/equation?tex=w_t+%2B+%5Cmu%5Cdelta_t)处求梯度不是更好吗？于是就有了Nesterov惯性法[1]：
　　　　　　・![\delta_{t+1} = \mu \delta_t - \lambda \nabla L(w_t + \mu\delta_t)](https://www.zhihu.com/equation?tex=%5Cdelta_%7Bt%2B1%7D+%3D+%5Cmu+%5Cdelta_t+-+%5Clambda+%5Cnabla+L%28w_t+%2B+%5Cmu%5Cdelta_t%29)
　　　　　　・![w_{t+1} = w_t + \delta_{t+1}](https://www.zhihu.com/equation?tex=w_%7Bt%2B1%7D+%3D+w_t+%2B+%5Cdelta_%7Bt%2B1%7D)

其示意图如下：
![](https://pic1.zhimg.com/04be2e523af6af82fd177b36e9469f98_b.jpg)
我是在[Coursera](https://link.zhihu.com/?target=https%3A//class.coursera.org/neuralnets-2012-001/lecture/63)上学到这个方法的，从1年前开始，我训练神经网络时一直用的都是这种方法。

## 3. 用Theano训练神经网络的基本步骤

　　Theano是用来训练神经网络的几大流行工具包之一，是基于Python语言的。Theano本身其实并不包含神经网络的功能，它本身的功能是用图形处理单元（GPU）进行高效的矩阵运算，但由于这正是神经网络训练过程的主要操作，所以Theano就被拿来用于神经网络的训练了。

　　用Theano训练神经网络的基本步骤是这样的。首先，要搭建一个纯符号的运算图，来表示神经网络的结构。运算图的输入有两种，一种是“符号变量”（symbolic variables），表示神经网络的输入；另一种是“共享内存“（shared memory），用来存储神经网络的参数。用这二者进行层层运算，可以得到神经网络的输出以及误差函数；利用Theano强大的**符号求导**功能，可以直接求得误差函数的梯度。运算图搭建好了之后，Theano可以把它编译成一个Python函数。在编译过程中，Theano会对运算图进行各种优化以提高其运行效率。编译好的函数可以在GPU上执行，它接收训练数据，据此计算出神经网络的输出以及误差函数的梯度，并更新网络的参数。

　　一直以来，我都是用如下代码实现Nesterov惯性法的：

```
import numpy, theano, theano.tensor as T

x = T.fmatrix()     % 网络输入，是一个符号向量或矩阵
t = T.fmatrix()     % 标准输出，也是一个符号向量或矩阵
lr = T.fscalar()    % 学习速率，是一个符号标量
mu = T.fscalar()    % 惯性系数，也是一个符号标量

params = [theano.shared(...), theano.shared(...), ...]
    % 网络参数，都是共享内存
delta = [theano.shared(numpy.zeros(...)), ...]
    % 惯性法中的delta，也是共享内存，初始值为0

y = ...x...params...        % 用符号运算搭建出网络的输出
L = ...y...t...             % 把网络输出与标准输出比较，搭建出误差函数
grad = T.grad(L, params)    % 自动求导！

updates = []
for w, d, g in zip(params, delta, grad):
  new_delta = mu * d - lr * theano.clone(g, replace = {w: w + mu * d})
    % 新的delta，要用theano.clone来把梯度中的w代换成w + mu * delta
  updates.append((d, new_delta))        % 更新delta
  updates.append((w, w + new_delta))    % 更新网络参数

train = theano.function(inputs = [x, t, lr, mu], updates = updates)
    % 把运算图编译成函数
```

　　在“搭建网络输出”的这一步中，如果网络是递归的，则需要用一个“扫描”（scan）运算符来实现沿时间轴的递归计算，这是网络中最复杂的运算符。

## 4. 用Theano实现Nesterov momentum的正确姿势

　　上面的代码一直都没有问题，直到我这周开始实现双向递归神经网络。我发现把运算图编译成函数这一步奇慢无比。一个1层的双向RNN，可以在三四分钟内编译完毕，这是正常速度。但一个2层的双向RNN，编译就需要将近1个小时；一个3层的双向RNN，编译竟然需要3个小时！

　　我在网上找到了一个神经网络工具包[Lasagne](https://link.zhihu.com/?target=http%3A//lasagne.readthedocs.org/en/latest/)（这是加菲猫最爱吃的千层面，也许是用来比喻神经网络的多层结构？），用它实现了一个3层的双向RNN。果然，它只需要几分钟就能编译完毕。看来，的确是我的实现有问题。

　　我通过一些黑科技，让Theano在编译过程中输出运算图中扫描运算符的个数。我发现，用Lasagne实现的3层双向RNN，其运算图中含有12个扫描运算符（我猜想是3层 × (正向+逆向) × (原函数+导函数)）；而我自己实现的3层双向RNN，其运算图中竟然含有110个扫描运算符。通过一行一行地删除代码，我最终定位到了问题的根源——theano.clone。为了把![w_t](https://www.zhihu.com/equation?tex=w_t)代换成![w_t + \mu \delta_t](https://www.zhihu.com/equation?tex=w_t+%2B+%5Cmu+%5Cdelta_t)，Theano要把整个运算图复制一次，这就使得运算图变得庞大无比，扫描运算符的数量也成倍增加。

　　事实上，我上面的实现不仅编译慢，逻辑也是错的。网络参数![w_t](https://www.zhihu.com/equation?tex=w_t)实际上是由许多个矩阵组成的，在求梯度时，应当把误差函数对每一个矩阵的梯度中的所有![w_t](https://www.zhihu.com/equation?tex=w_t)都替换成相应的![w_t + \mu \delta_t](https://www.zhihu.com/equation?tex=w_t+%2B+%5Cmu+%5Cdelta_t)，而我却只替换了每个梯度中相应的矩阵。逻辑正确的实现方式，应该是这样的：

```python
replace = dict((w, w + mu * d) for w, d in zip(params, delta))
for w, d, g in zip(params, delta, grad):
  new_delta = mu * d - lr * theano.clone(g, replace = replace)
  updates.append((d, new_delta))
  updates.append((w, w + new_delta))
```

　　但这并不是本文的重点。本文的重点是，我从Lasagne的源代码中，学到了一种可以**完全避免在梯度中进行变量代换**的方法。回顾Nesterov惯性法的示意图可以看到，在从![w_t](https://www.zhihu.com/equation?tex=w_t)向![w_{t+1}](https://www.zhihu.com/equation?tex=w_%7Bt%2B1%7D)前进的过程中，经过了![w'_t = w_t + \mu \delta_t](https://www.zhihu.com/equation?tex=w%27_t+%3D+w_t+%2B+%5Cmu+%5Cdelta_t)。如果把这个过程拆成![w_t \rightarrow w'_t \rightarrow w_{t+1}](https://www.zhihu.com/equation?tex=w_t+%5Crightarrow+w%27_t+%5Crightarrow+w_%7Bt%2B1%7D)这样的两步，我们会发现，在Nesterov惯性法的执行过程中，我们存储了偶数步的参数值，却是在奇数步处求梯度。事实上，偶数步的参数值![w_t](https://www.zhihu.com/equation?tex=w_t)与奇数步的参数值![w'_t](https://www.zhihu.com/equation?tex=w%27_t)在地位上并没有区别，我们完全可以放弃存储![w_t](https://www.zhihu.com/equation?tex=w_t)，转而存储![w'_t](https://www.zhihu.com/equation?tex=w%27_t)。这样，我们就可以在存储的参数值处求梯度，从而避免变量代换。
　　用![w_t](https://www.zhihu.com/equation?tex=w_t)表达的Nesterov惯性法的公式为：

　　　　　　・![\delta_{t+1} = \mu \delta_t - \lambda \nabla L(w_t + \mu\delta_t)](https://www.zhihu.com/equation?tex=%5Cdelta_%7Bt%2B1%7D+%3D+%5Cmu+%5Cdelta_t+-+%5Clambda+%5Cnabla+L%28w_t+%2B+%5Cmu%5Cdelta_t%29)
　　　　　　・![w_{t+1} = w_t + \delta_{t+1}](https://www.zhihu.com/equation?tex=w_%7Bt%2B1%7D+%3D+w_t+%2B+%5Cdelta_%7Bt%2B1%7D)
通过简单的变量代换，可以得到用![w'_t](https://www.zhihu.com/equation?tex=w%27_t)表达的公式[2]：

　　　　　　・![\delta_{t+1} = \mu \delta_t - \lambda \nabla L(w'_t)](https://www.zhihu.com/equation?tex=%5Cdelta_%7Bt%2B1%7D+%3D+%5Cmu+%5Cdelta_t+-+%5Clambda+%5Cnabla+L%28w%27_t%29)
　　　　　　・![w'_{t+1} = w'_t + \mu^2 \delta_{t} - (1+\mu) \lambda \nabla L(w'_t)](https://www.zhihu.com/equation?tex=w%27_%7Bt%2B1%7D+%3D+w%27_t+%2B+%5Cmu%5E2+%5Cdelta_%7Bt%7D+-+%281%2B%5Cmu%29+%5Clambda+%5Cnabla+L%28w%27_t%29)
相应的Theano代码为：

```python
for w, d, g in zip(params, delta, grad):
  updates.append((d, mu * d - lr * g))
  updates.append((w, w + mu * mu * d - ( + mu) * lr * g))
```

做了这个修改后，我的程序果然也可以在几分钟之内编译完毕了。

## 后记

　　其实我想写的就是最后的两个公式，但是前面铺垫居然写了一天= =

## 参考文献

[1] Yurii Nesterov, "A method of solving a convex programming
problem with convergence rate O(1/sqr(k))", *SovietMathematics Doklady*, 27:372–376, 1983.

[2] [https://github.com/lisa-lab/pylearn2/pull/136#issuecomment-10381617](https://link.zhihu.com/?target=https%3A//github.com/lisa-lab/pylearn2/pull/136%23issuecomment-10381617)


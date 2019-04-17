# pytorch系列 --11 pytorch loss function： MSELoss BCELoss CrossEntropyLoss及one_hot 格式求 cross_entropy - 墨流觞的博客 - CSDN博客





2018年11月13日 22:41:05[墨氲](https://me.csdn.net/dss_dssssd)阅读数：1205
所属专栏：[python3 pytorch0.4系列教程](https://blog.csdn.net/column/details/29714.html)









本文主要包括：
- pytorch 实现的损失函数

### pytorch实现的loss function

神经网络主要实现**分类以及回归预测**两类问题，对于回归问题，主要讲述**均方损失函数**，而对于一些回归问题，需要根据特殊情况**自定义损失函数**。对于分类，主要讲述**二分类交叉熵和多分类交叉熵函数**

在讲解函数之前，提前说一下：
- 所有的loss的基类是Module，所以使用loss的方法是：

```python
# 1. 创建损失函数对象,并指定返回结果，默认为：平均值 以MSE为例
criterion = MSELoss(reduction='...')
# 2.  定义input x, traget y
x = torch.tensor(...) 
y = torch.tensor(...)
# 计算损失函数
loss = criterion(x, y)
```
- 在pytorch 0.4中，参数`size_average`和`reduce`已被舍弃，使用`reduction`参数控制损失函数的输出行为。
- reduction (string, optional)
- ‘none’: 不进行数据降维，输出为向量
- ‘elementwise_mean’： 将向量中的数累加求和，然后除以元素数量，返回**误差的平均值**
- ‘sum’： 返回向量的**误差值的和**


#### 1. `class torch.nn.MSELoss(size_average=None, reduce=None, reduction='elementwise_mean')`

计算输入x和标签y，n个元素平均平方误差（mean square error），x和y具有相同的Size

损失函数如下定义：
$$\ell(x, y) = L = \{l_1,\dots,l_N\}^\top, \quad        l_n = ( x_n - y_n )^2$$

如果reduction != ‘none’:
$$\ell(x, y) = \begin{cases}            \operatorname{mean}(L), &amp; \text{if}\; \text{reduction} = \text{&#x27;elementwise\_mean&#x27;},\\            \operatorname{sum}(L),  &amp; \text{if}\; \text{reduction} = \text{&#x27;sum&#x27;}.        \end{cases}$$
shape: N 为一批数据的数量
- input： $(N, *)$, * 意味着任何数量的附加维度
- Target: (N,∗), shape与input相同

通过代码来看一基本用法，以及reduction参数对返回结果的影响：

```python
import torch

from torch import nn

criterion_none = nn.MSELoss( reduction='none')
criterion_elementwise_mean = nn.MSELoss(reduction='elementwise_mean')
criterion_sum = nn.MSELoss(reduction='sum')

x = torch.randn(3, 2, requires_grad=True)
y = torch.randn(3, 2)

loss_none = criterion_none(x, y)

loss_elementwise_mean = criterion_elementwise_mean(x, y)

loss_sum = criterion_sum(x, y )

print('reduction={}:   {}'.format('none', loss_none.detach().numpy()))
print('reduction={}:   {}'.format('elementwise_mean', loss_elementwise_mean.item()))
print('reduction={}:   {}'.format('sum', loss_sum.item()))
```

out:

> 
reduction=none:

[[0.02320575 0.30483633]

[0.04768182 0.4319028 ]

[3.11864    7.9872203 ]]

reduction=elementwise_mean:   1.9855811595916748   # 1.9 * 6 = 11.4

reduction=sum:   11.913487434387207
#### 2. 交叉熵损失函数

该博客讲述了交叉熵的定义以及为何使用交叉熵，对交叉熵不是很了解的可以看一下：
[http://jackon.me/posts/why-use-cross-entropy-error-for-loss-function/](http://jackon.me/posts/why-use-cross-entropy-error-for-loss-function/)

这篇博客讲了如何求交叉熵损失函数的导数，有兴趣的可以看一下：
[https://zhuanlan.zhihu.com/p/35709485](https://zhuanlan.zhihu.com/p/35709485)
##### 1.  `class torch.nn.BCELoss(weight=None, size_average=None, reduce=None, reduction='elementwise_mean')`

二分类的交叉熵函数。**使用该函数之前先计算Sigmoid值**。[https://pytorch.org/docs/stable/nn.html#torch.nn.Sigmoid](https://pytorch.org/docs/stable/nn.html#torch.nn.Sigmoid)

损失函数表达式如下：
$$\ell(x, y) = L = \{l_1,\dots,l_N\}^\top, \quad        l_n = - w_n \left[ y_n \cdot \log x_n + (1 - y_n) \cdot \log (1 - x_n) \right],$$
如果reduction != ‘none’:
$$\ell(x, y) = \begin{cases}            \operatorname{mean}(L), &amp; \text{if}\; \text{reduction } = \text{&#x27;elementwise\_mean&#x27;},\\            \operatorname{sum}(L),  &amp; \text{if}\; \text{reduction } = \text{&#x27;sum&#x27;}.        \end{cases}$$

shape: N 为一批数据的数量
- input： $(N, *)$, * 意味着任何数量的附加维度
- Target: (N,∗), shape与input相同

random_：[https://pytorch.org/docs/master/tensors.html#torch.Tensor.random_](https://pytorch.org/docs/master/tensors.html#torch.Tensor.random_)

[https://pytorch.org/docs/master/torch.html#in-place-random-sampling](https://pytorch.org/docs/master/torch.html#in-place-random-sampling)

```python
import torch

from torch import nn

m = nn.Sigmoid()
criterion = nn.BCELoss()
x = torch.randn(3, requires_grad=True)
# random_(from=0, to): 按均匀分布从[from, to -1]内去出离散整数值
# 将y取0或1
y = torch.empty(3).random_(2)
# 在计算前线计算x的sigmoid值
loss = criterion(m(x), y)

print(loss.item())
```

out:

> 
0.8146645426750183


##### 2. `class torch.nn.CrossEntropyLoss(weight=None, size_average=None, ignore_index=-100, reduce=None, reduction='elementwise_mean')`

[https://pytorch.org/docs/stable/nn.html#torch.nn.CrossEntropyLoss](https://pytorch.org/docs/stable/nn.html#torch.nn.CrossEntropyLoss)

**在使用该函数前不需要经过softmax计算， target不是one_hot编码格式**

shape: N是一批数据的数量
- input: (N, C): C是类的数量

(N,C,d1,d2,…,dK) with K≥2 in the case of K-dimensional loss.
- Target： (N),   0≤targets[i]≤C−1,为每个样本的类别。

(N,d1,d2,…,dK) with K≥2 in the case of K-dimensional loss
- Output: (N), (N, d1,d2,…,dK)


loss表达式：
$$\text{loss}(x, class) = -\log\left(\frac{\exp(x[class])}{\sum_j \exp(x[j])}\right)                       = -x[class] + \log\left(\sum_j \exp(x[j])\right)$$

```python
import torch

from torch import nn


criterion = nn.CrossEntropyLoss()
x = torch.randn(3, 5, requires_grad=True)
# random_(from=0, to): 按均匀分布从[from, to -1]内去出离散整数值
# 将y取0或1
y = torch.empty(3, dtype=torch.long).random_(5)
# 在计算前线计算x的sigmoid值
loss = criterion(x, y)

print(loss.item())
```

out:

> 
1.887318730354309


如果传给target是one_hot编码格式呢？
- 将target one_hot的编码格式转换为每个样本的类别，再传给CrossEntropyLoss

代码中主要涉及了
- 如何变为`one_hot`形式，使用`scatter_`:

参考文章：
- [https://discuss.pytorch.org/t/convert-int-into-one-hot-format/507/4](https://discuss.pytorch.org/t/convert-int-into-one-hot-format/507/4)
- [https://blog.csdn.net/u012436149/article/details/77017832](https://blog.csdn.net/u012436149/article/details/77017832)

- max函数， 简单的说返回元组$(最大值， 在指定dim上的位置)$

```python
import torch

from torch import nn
from torch.nn import functional as F
# 编码one_hot
def one_hot(y):
    '''
    y: (N)的一维tensor，值为每个样本的类别
    out: 
        y_onehot: 转换为one_hot 编码格式 
    '''
    y = y.view(-1, 1)
    y_onehot = torch.FloatTensor(3, 5)
    
    # In your for loop
    y_onehot.zero_()
    y_onehot.scatter_(1, y, 1)
    return y_onehot


def cross_entropy_one_hot(input_, target):
	# 解码 
    _, labels = target.max(dim=1)
    # 调用cross_entropy
    return F.cross_entropy(input_, labels)

# 调用计算loss： loss_1 = cross_entropy_one_hot(x, one_hot(y))
```
- 自己根据CrossEntropyLoss的定义重写

多类的crossentropy:

[0.1, 0.2, 0.7] (prediction) ------------------ [1.0, 0.0, 0.0] (target)

则损失函数为： - (1.0 * log(0.1) + 0.0 * log(0.2) + 0.0 * log(0.7))

```python
def cross_entropy(input_, target, reduction='elementwise_mean'):
    """ Cross entropy that accepts soft targets
    Args:
         pred: predictions for neural network
         targets: targets, can be soft
         size_average: if false, sum is returned instead of mean

    Examples::

        input = torch.FloatTensor([[1.1, 2.8, 1.3], [1.1, 2.1, 4.8]])
        input = torch.autograd.Variable(out, requires_grad=True)

        target = torch.FloatTensor([[0.05, 0.9, 0.05], [0.05, 0.05, 0.9]])
        target = torch.autograd.Variable(y1)
        loss = cross_entropy(input, target)
        loss.backward()
    """
    logsoftmax = nn.LogSoftmax(dim=1)
    res  =-target * logsoftmax(input_)
    if reduction == 'elementwise_mean':
        return torch.mean(torch.sum(res, dim=1))
    elif reduction == 'sum':
        return torch.sum(torch.sum(res, dim=1))
    else:
        return res
```

最后附上三种求loss的的代码：

```python
import torch

from torch import nn
from torch.nn import functional as F


x = torch.randn(3, 5, requires_grad=True)
# random_(from=0, to): 按均匀分布从[from, to -1]内去出离散整数值
# 将y取0或1
y = torch.empty(3, dtype=torch.long).random_(5)
def one_hot(y):
    '''
    y: (N)的一维tensor，值为每个样本的类别
    out: 
        y_onehot: 转换为one_hot 编码格式 
    '''
    y = y.view(-1, 1)
    y_onehot = torch.FloatTensor(3, 5)
    
    # In your for loop
    y_onehot.zero_()
    y_onehot.scatter_(1, y, 1)
    return y_onehot

# 1. 自带的函数
criterion = nn.CrossEntropyLoss()

# 2.将one_hot解码后使用pytorch自带的cross_entropy
def cross_entropy_one_hot(input, target):
    _, labels = target.max(dim=1)
    return F.cross_entropy(input, labels)  
    # 也可以使用以下语句
    # return nn.CrossEntropyLoss()(input, labels)


# 3. 自定义
def cross_entropy(input_, target, reduction='elementwise_mean'):
    """ Cross entropy that accepts soft targets
    Args:
         pred: predictions for neural network
         targets: targets, can be soft
         size_average: if false, sum is returned instead of mean

    Examples::

        input = torch.FloatTensor([[1.1, 2.8, 1.3], [1.1, 2.1, 4.8]])
        input = torch.autograd.Variable(out, requires_grad=True)

        target = torch.FloatTensor([[0.05, 0.9, 0.05], [0.05, 0.05, 0.9]])
        target = torch.autograd.Variable(y1)
        loss = cross_entropy(input, target)
        loss.backward()
    """
    logsoftmax = nn.LogSoftmax(dim=1)
    res  =-target * logsoftmax(input_)
    if reduction == 'elementwise_mean':
        return torch.mean(torch.sum(res, dim=1))
    elif reduction == 'sum':
        return torch.sum(torch.sum(res, dim=1))
    else:
        return res

loss = criterion(x, y)
print("loss",loss.item())
loss_1 = cross_entropy_one_hot(x, one_hot(y))
print("loss_one_hot", loss_1.item())
loss_2 = cross_entropy(x, one_hot(y))
print("loss_custom", loss_2.item())
```

out:

> 
loss 3.0437448024749756

loss_one_hot 3.0437448024749756

loss_custom 3.0437448024749756
参考：
[https://discuss.pytorch.org/t/cross-entropy-with-one-hot-targets/13580/3](https://discuss.pytorch.org/t/cross-entropy-with-one-hot-targets/13580/3)











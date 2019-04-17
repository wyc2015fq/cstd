# Pytorch之深入backward - qq_37385726的博客 - CSDN博客





2018年08月15日 16:58:27[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：285
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









# **目录**

[解释](#%E8%A7%A3%E9%87%8A)



> 
对于backward，我们一般都是一个tensor标量来调用的，那如果是一个variable.data不是标量呢，那还可以调用吗？

可以，加上参数就行。

参数决定的是variable.data每一元素的影响因子（权值）

【看后面的解释就明白了】




**torch.autograd.backward(variables, grad_variables=None, retain_graph=None, create_graph=False)**

给定图的叶子节点`variables`, 计算图中变量的梯度和。 计算图可以通过链式法则求导。如果`variables`中的任何一个`variable`是 非标量(`non-scalar`)的，且`requires_grad=True`。那么此函数需要指定`grad_variables`，它的长度应该和`variables`的长度匹配，里面保存了相关`variable`的梯度(对于不需要`gradient tensor`的`variable`，`None`是可取的)。

此函数累积`leaf variables`计算的梯度。你可能需要在调用此函数之前将`leaf variable`的梯度置零。

**参数：**
- `variables`（变量的序列） - 被求微分的叶子节点，即 `ys` 。
- `grad_variables`（（张量，变量）的序列或无） - 对应`variable`的梯度。仅当`variable`不是标量且需要求梯度的时候使用。
- `retain_graph`（`bool`，可选） - 如果为`False`，则用于释放计算`grad`的图。请注意，在几乎所有情况下，没有必要将此选项设置为`True`，通常可以以更有效的方式解决。默认值为`create_graph`的值。
- `create_graph`（`bool`，可选） - 如果为`True`，则将构造派生图，允许计算更高阶的派生产品。默认为`False`。

我这里举一个官方的例子

```python
import torch
from torch.autograd import Variable
x = Variable(torch.ones(2, 2), requires_grad=True)
y = x + 2
z = y * y * 3
out = z.mean()
out.backward()#这里是默认情况，相当于out.backward(torch.Tensor([1.0]))
print(x.grad)
```



输出结果是

Variable containing:

4.5000 4.5000

4.5000 4.5000

[torch.FloatTensor of size 2x2]



接着我们继续

```python
x = torch.randn(3)
x = Variable(x, requires_grad=True)

y = x * 2
while y.data.norm() < 1000:
    y = y * 2

gradients = torch.FloatTensor([0.1, 1.0, 0.0001])
y.backward(gradients)
print(x.grad)
```

输出结果是

Variable containing:

204.8000

2048.0000

0.2048

[torch.FloatTensor of size 3]



# 解释

这里这个gradients为什么要是[0.1, 1.0, 0.0001]？

如果输出的多个loss权重不同的话，例如有三个loss，一个是y1 loss，一个是y2 loss，一个是y3 loss。那么很明显的不可能所有loss对结果影响程度都一样，他们之间应该有一个比例。那么比例这里指的就是[0.1, 1.0, 0.0001]，这个问题中的loss对应的就是上面说的y，那么这里的输出就很好理解了dy/dx=0.1*dy1/dx+1.0*dy2/dx+0.0001*dy3/dx。



> 
**这个在考虑不同因素的影响因子时给人启发，但是同时也可以再加上一个或一层神经元得到一个标量的结果**





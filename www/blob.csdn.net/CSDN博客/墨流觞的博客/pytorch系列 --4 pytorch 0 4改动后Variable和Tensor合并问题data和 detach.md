# pytorch系列 --4  pytorch 0.4改动后Variable和Tensor合并问题data和.detach - 墨流觞的博客 - CSDN博客





2018年11月07日 15:03:21[墨氲](https://me.csdn.net/dss_dssssd)阅读数：1310
所属专栏：[pytorch系列教程](https://blog.csdn.net/column/details/29549.html)[python3 pytorch0.4系列教程](https://blog.csdn.net/column/details/29714.html)









本文主要讲述pytorch0.4更新后相关的代码迁移问题

## Tensor和Variable合并

torch.Tensor 和torch.autograd.Variable现在是同一个类。torch.Tensor 能够像之前的Variable一样追踪历史和反向传播。Variable仍能够正常工作，但是返回的是Tensor。所以在0.4的代码中，不需要使用Variable了。

可以看出使用Variable封装Tensor之后返回的也是Tensor。

```python
import torch
from torch.autograd import Variable

x_tensor = torch.Tensor(3,4)
x_var = Variable(x_tensor,requires_grad=True)

print(x_var)
```

out:

> 
tensor([[1.7753e+28, 4.5601e+30, 1.8465e+25, 1.0901e+27],

[1.8289e+34, 7.4103e+28, 1.6443e+19, 4.9172e+33],

[9.2246e-39, 6.8383e-43, 1.4013e-45, 0.0000e+00]], requires_grad=True)
接下来看一下，合并Tensor和Variable之后`autograd`是如何实现历史追踪和反向传播的

作为能否`autograd`的标签，`requires_grad`现在是Tensor的属性，所以，只要当一个操作(operation)的**任何**输入`Tensor`具有`requires_grad = True`的属性，`autograd`就可以自动追踪历史和反向传播了。

官方的实例代码：

```python
# 默认创建requires_grad = False的Tensor
 x = torch.ones(1)   # create a tensor with requires_grad=False (default)
 x.requires_grad
 # out: False
 
 # 创建另一个Tensor，同样requires_grad = False
 y = torch.ones(1)  # another tensor with requires_grad=False
 # both inputs have requires_grad=False. so does the output
 z = x + y
 # 因为两个Tensor x,y，requires_grad=False.都无法实现自动微分，
 # 所以操作（operation）z=x+y后的z也是无法自动微分，requires_grad=False
 z.requires_grad
 # out: False
 
 # then autograd won't track this computation. let's verify!
 # 因而无法autograd，程序报错
 z.backward()
 # out：程序报错：RuntimeError: element 0 of tensors does not require grad and does not have a grad_fn

# now create a tensor with requires_grad=True
 w = torch.ones(1, requires_grad=True)
 w.requires_grad
 # out: True
 
 # add to the previous result that has require_grad=False
 # 因为total的操作中输入Tensor w的requires_grad=True，因而操作可以进行反向传播和自动求导。
 total = w + z
# the total sum now requires grad!
total.requires_grad
# out: True
# autograd can compute the gradients as well
total.backward()
w.grad
#out: tensor([ 1.])

# and no computation is wasted to compute gradients for x, y and z, which don't require grad
# 由于z，x，y的requires_grad=False,所以并没有计算三者的梯度
z.grad == x.grad == y.grad == None
# True
```

那接下来看一下如何在已经创建的tensor上添加requires_grad的属性呢？

相比于在创建tensor是就设置`requires_grad`的值，可以 使用`my_tensor=requires_grad_()`实现`in-place`的操作来改变`requires_grad`的属性

```python
existing_tensor.requires_grad_()
existing_tensor.requires_grad
# out：True
```

也可以在创建时就设置属性值

```python
my_tensor = torch.zeros(3, 4, requires_grad=True)
my_tensor.requires_grad
# out: True
```

**重点介绍一下.data和detach()函数的区别**

推荐在经网络网训练或测试时访问tensor的数据时使用.detach()方法。

简单的说就是使用y=x.data属性来访问数据时，pytorch不会记录数据是否改变，此时改变了y的值，意味着也要改变x的值，而在自动求导时会使用更改后的值，这回导致错误求导结果。

```python
a = torch.tensor([1,2,3.], requires_grad = True)
out = a.sigmoid()
c = out.data
c.zero_()
#out: tensor([ 0.,  0.,  0.])

out  # out  was modified by c.zero_()
#out: tensor([ 0.,  0.,  0.])

out.sum().backward()
a.grad  # The result is very, very wrong because `out` changed!
#out: tensor([ 0.,  0.,  0.])
```

而使用y=x.detach()时，如果了y值，也意味着改变了x值，此时调用x.backword()会报错。也就是说.detach()方法会记录数据的变化状态。
**所以，推荐使用x.detach()来访问数据，更加安全。**

```python
a = torch.tensor([1,2,3.], requires_grad = True)
out = a.sigmoid()
c = out.detach()
 c.zero_()
 #out: tensor([ 0.,  0.,  0.])

out  # modified by c.zero_() !!
#out:  tensor([ 0.,  0.,  0.])

out.sum().backward()  # Requires the original value of out, but that was overwritten by c.zero_()
RuntimeError: one of the variables needed for gradient computation has been modified by an
```

### Tensor的type()方法

`type(tensor)`将不在返回tensor的数据类型，返回的是class，使用`isinstance()`或`tensor.type()`来得到tensor的数据类型.

```python
x = torch.DoubleTensor([1, 1, 1])
 print(type(x))  # was torch.DoubleTensor
# out: "<class 'torch.Tensor'>"

 print(x.type())  # OK: 'torch.DoubleTensor'
# out: 'torch.DoubleTensor'

print(isinstance(x, torch.DoubleTensor))  # OK: True
# out: True
```

### 支持0维向量（标量）

在0.4之前，索引1维的Tensor但会python number， 而索引Variable返回的是size为(1,)的向量；两者在行为上不一致，同样的对于一些降维函数，比如Tensor.sum(),返回的也是Python number，而variable.sum()返回的是size为(1,)的向量。

在0.4的版本release中，pytorch支持标量（0-维tensor).可以使用torch.tensor创建标量。而sum()，loss()等返回一个数的降维函数也将返回标量(0-维 tensor)

对于0-维 tensor， .item()方法，返回Python number。

```python
torch.tensor(3.1416)         # create a scalar directly
# out： tensor(3.1416)

torch.tensor(3.1416).size()  # scalar is 0-dimensional
#out： torch.Size([])

torch.tensor([3]).size()     # compare to a vector of size 1
#out： torch.Size([1])
ector = torch.arange(2, 6)  # this is a vector
vector
#out： tensor([ 2.,  3.,  4.,  5.])

vector.size()
#out： torch.Size([4])

vector[3]                    # indexing into a vector gives a scalar
#out: tensor(5.)

vector[3].item()             # .item() gives the value as a Python number
#out: 5.0

mysum = torch.tensor([2, 3]).sum() # sum（）返回一个数，在此返回标量
mysum
#out: tensor(5)

mysum.size()
#out: torch.Size([])
 
mysum[0] # 使用此会出现警告
#out: User Warning: invalid index of a 0-dim tensor. This will be an error in PyTorch 0.5. Use tensor.item() to convert a 0-dim tensor to a Python number
```

### losses

在0.4之前，广泛的loss使用是：`total_loss += loss.data[0]`, 这是因为loss是variable，其中size为（1，），而在0.4之后，loss是一个0-维的向量（标量）。可以使用loss.item()从标量中获得Python number。

**注意**，当不转换为Python number来计算loss的累加值时，程序将会有大量的内存使用，这是因为`total_loss += loss.data[0]`式子的右边不是Python float而变成了0维tensor。这杨，总损失就会包含loss和梯度历史，这些梯度历史会在大的autograd graph中保存更长时间，带来大量的内存消耗。

**上述loss积累的式子可以变为：**
`total_loss += loss.item()`

在本系列的代码中，将遵循新版本语言规范。






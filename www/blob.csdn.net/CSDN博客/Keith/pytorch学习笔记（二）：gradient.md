# pytorch学习笔记（二）：gradient - Keith - CSDN博客





2017年01月21日 11:15:45[ke1th](https://me.csdn.net/u012436149)阅读数：25762
所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)









在`BP`的时候，`pytorch`是将`Variable`的梯度放在`Variable`对象中的，我们随时都可以使用`Variable.grad`得到对应`Variable`的`grad`。刚创建`Variable`的时候，它的`grad`属性是初始化为`0.0`的（0.2 版本已经是 打印的结果是 None。）。

```python
import torch
from torch.autograd import Variable
w1 = Variable(torch.Tensor([1.0,2.0,3.0]),requires_grad=True)#需要求导的话，requires_grad=True属性是必须的。
w2 = Variable(torch.Tensor([1.0,2.0,3.0]),requires_grad=True)
print(w1.grad) # 0.2 版本打印的是 None
print(w2.grad) # 0.2 版本打印的是 None
```

```
Variable containing:
 0
 0
 0
[torch.FloatTensor of size 3]

Variable containing:
 0
 0
 0
[torch.FloatTensor of size 3]
```

**从下面这两段代码可以看出，使用`d.backward()`求`Variable`的梯度的时候，`Variable.grad`是累加的即: `Variable.grad=Variable.grad+new_grad`**

```python
d = torch.mean(w1)
d.backward()
w1.grad
```

```
Variable containing:
 0.3333
 0.3333
 0.3333
[torch.FloatTensor of size 3]
```

```python
d.backward()
w1.grad
```

```
Variable containing:
 0.6667
 0.6667
 0.6667
[torch.FloatTensor of size 3]
```

**既然累加的话，那我们如何置零呢？**

```python
w1.grad.data.zero_()
w1.grad
```

```
Variable containing:
 0
 0
 0
[torch.FloatTensor of size 3]
```

通过上面的方法，就可以将`grad`置零。通过打印出来的信息可以看出，`w1.grad`其实是`Variable`。现在可以更清楚的理解一下`Variable`与`Tensor`之间的关系，上篇博客已经说过，`Variable`是`Tensor`的一个`wrapper`，那么到底是什么样的`wrapper`呢？从目前的掌握的知识来看，一个是保存`weights`的`Tensor`，一个是保存`grad`的`Variable`。`Variable`的一些运算，实际上就是里面的`Tensor`的运算。 
`pytorch`中的所有运算都是基于`Tensor`的，`Variable`只是一个`Wrapper`，`Variable`的计算的实质就是里面的`Tensor`在计算。`Variable`默认代表的是里面存储的`Tensor`（`weights`）。理解到这，我们就可以对`grad`进行随意操作了。

```python
# 获得梯度后，如何更新
learning_rate = 0.1
#w1.data -= learning_rate * w1.grad.data 与下面式子等价
w1.data.sub_(learning_rate*w1.grad.data)# w1.data是获取保存weights的Tensor
```

这里更新的时候为什么要用`Tensor`更新，为什么不直接用`Variable`？ 
`Variable`更多是用在`feedforward`中的，因为`feedforward`是需要记住各个`Tensor`之间联系的，这样，才能正确的`bp`。`Tensor`不会记录路径。而且，如果使用`Variable`操作的话，就会造成循环图了（猜测）。

## torch.optim

如果每个参数的更新都要`w1.data.sub_(learning_rate*w1.grad.data)`，那就比较头疼了。还好，`pytorch`为我们提供了`torch.optim`包，这个包可以简化我们更新参数的操作。

```python
import torch.optim as optim
# create your optimizer
optimizer = optim.SGD(net.parameters(), lr = 0.01)

# in your training loop:
for i in range(steps):
  optimizer.zero_grad() # zero the gradient buffers，必须要置零
  output = net(input)
  loss = criterion(output, target)
  loss.backward()
  optimizer.step() # Does the update
```

注意：`torch.optim`只用于更新参数，不care梯度的计算。

## 关于 backward()

`backward(gradient=None, retain_variables=False)`

参数： 
`gradient (Tensor) – Gradient of the differentiated function w.r.t. the data. Required only if the data has more than one element`
```python
z.backward(gradient=grads)
```

上面代码应该怎么解释呢？



$\frac{\partial obj}{\partial z}\frac{\partial z}{\partial w}=grads*\frac{\partial z}{\partial w}$

对于 `retain_variables`:

```python
import torch
from torch.autograd import Variable
w1 = Variable(torch.Tensor([1.0,2.0,3.0]),requires_grad=True)#需要求导的话，requires_grad=True属性是必须的。
w2 = Variable(torch.Tensor([1.0,2.0,3.0]),requires_grad=True)

z = w1*w2+w1 # 第二次BP出现问题就在这，不知道第一次BP之后销毁了啥。
res = torch.mean(z)
res.backward() #第一次求导没问题
res.backward() #第二次BP会报错,但使用 retain_variables=True，就好了。
# Trying to backward through the graph second time, but the buffers have already been 
#freed. Please specify retain_variables=True when calling backward for the first time
```
- 这里也可以看出，`backward` 这个方法也是 **释放一些资源的** 的一个标志，如果不需要 `backward` 的话，一定要记得设置 网络 为 `eval`。

## 其他

这里来测试一下只使用部分 `Variable` 求出来的 `loss`对于原`Variable`求导得到的梯度是什么样的。

```python
import torch
import torch.cuda as cuda
from torch.autograd import Variable
w1 = Variable(cuda.FloatTensor(2,3), requires_grad=True)
res = torch.mean(w1[1])# 只用了variable的第二行参数
res.backward()
print(w1.grad)
```

```
Variable containing:
 0.0000  0.0000  0.0000
 0.3333  0.3333  0.3333
[torch.cuda.FloatTensor of size 2x3 (GPU 0)]
```

看结果和直觉是一样的。





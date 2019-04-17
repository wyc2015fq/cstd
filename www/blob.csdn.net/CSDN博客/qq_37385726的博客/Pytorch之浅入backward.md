# Pytorch之浅入backward - qq_37385726的博客 - CSDN博客





2018年08月15日 15:53:17[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：187
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)



> 
原地 ( in-place )操作都要加_ ，对于variable的grad属性，由于其是累加的，所以更新的时候需要清零，可以使用原地操作，例如x.grad.data.zero_()


# 代码

```python
import torch
from torch.autograd import Variable

x = Variable(torch.ones(2,2),requires_grad = True)

y = x + 2
print(y)
y = y.mean()  #求平均数
print(y)

y.backward()  #反向传递函数，用于求y对前面的变量（x）的梯度
print(x.grad) # d(y)/d(x)

x.grad.data.zero_()  #grad是累加的，所以再次求需要清零

y.backward()
print(x.grad)
```

# 输出

> 
**自己创建的variable的grad_fn是none，但对于由计算得到的variable，例如 y，的grad_fn是不为none的，可以调用backward函数求梯度**


tensor([[3., 3.],

        [3., 3.]], grad_fn=<AddBackward>)



tensor(3., grad_fn=<MeanBackward1>)



tensor([[0.2500, 0.2500],

        [0.2500, 0.2500]])



tensor([[0.2500, 0.2500],

        [0.2500, 0.2500]])




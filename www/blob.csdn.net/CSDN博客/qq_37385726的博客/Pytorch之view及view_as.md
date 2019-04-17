# Pytorch之view及view_as - qq_37385726的博客 - CSDN博客





2018年08月16日 11:36:11[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：4064
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









# **目录**

[很重要的一点](#%E5%BE%88%E9%87%8D%E8%A6%81%E7%9A%84%E4%B8%80%E7%82%B9)

[view函数与Pytorch0.4中新增的reshape的区别](#view%E5%87%BD%E6%95%B0%E4%B8%8EPytorch0.4%E4%B8%AD%E6%96%B0%E5%A2%9E%E7%9A%84reshape%E7%9A%84%E5%8C%BA%E5%88%AB)

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)

[view_as](#view_as)

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA%C2%A0)



> 
**view()函数**是在torch.Tensor.view()下的一个函数，可以有tensor调用，也可以有variable调用。

其作用在于返回和原tensor数据个数相同，但size不同的tensor

【Numpy中的size是元素个数，但是在Pytorch中size等价为Numpy中的shape】


> 
**view函数的-1参数的作用在于基于另一参数，自动计算该维度的大小**








# **很重要的一点**

> 
**view函数只能由于contiguous的张量上**，具体而言，就是在内存中连续存储的张量。

具体而言，可以参看[https://blog.csdn.net/qq_37385726/article/details/81811254](https://blog.csdn.net/qq_37385726/article/details/81811254)

所以，当tensor之前调用了transpose, permute函数就会是tensor内存中变得不再连续，就不能调用view函数。

**所以，应该提前做tensor.contiguous()的操作**


# **view函数与Pytorch0.4中新增的reshape的区别**

> 
**reshape函数调用是不依赖于tensor在内存中是不是连续的。**

**即**

**reshape ≈ tensor.contiguous().view**








# 代码

```python
import numpy as np
import torch
from torch.autograd import Variable

x = torch.Tensor(2,2,2)
print(x)

y = x.view(1,8)
print(y)

z = x.view(-1,4)  # the size -1 is inferred from other dimensions
print(z)

t = x.view(8)
print(t)
```

# 输出

tensor([[[1.3712e-14, 6.4069e+02],

         [4.3066e+21, 1.1824e+22]],

        [[4.3066e+21, 6.3828e+28],

         [3.8016e-39, 0.0000e+00]]])

**#x.view(1,8)生成的是[1,8]的张量**

tensor([[1.3712e-14, 6.4069e+02, 4.3066e+21, 1.1824e+22, 4.3066e+21, 6.3828e+28,

         3.8016e-39, 0.0000e+00]])

**#x.view(-1,4)其中-1是在4下的另一个维度的大小，也就是8/4=2，所以生成的是[2,4]的张量**

tensor([[1.3712e-14, 6.4069e+02, 4.3066e+21, 1.1824e+22],

        [4.3066e+21, 6.3828e+28, 3.8016e-39, 0.0000e+00]])

**x.view(8)生成的是[8,]的张量，是个数组**

tensor([1.3712e-14, 6.4069e+02, 4.3066e+21, 1.1824e+22, 4.3066e+21, 6.3828e+28,

        3.8016e-39, 0.0000e+00])

# view_as

返回被视作与给定的tensor相同大小的原tensor。 等效于：
`self.view(tensor.size())`
具体用法为：

### 代码

```python
a = torch.Tensor(2, 4)
b = a.view_as(torch.Tensor(4, 2))
print (b)
```

### 输出 

tensor([[1.3712e-14, 6.4069e+02],

        [4.3066e+21, 1.1824e+22],

        [4.3066e+21, 6.3828e+28],

        [3.8016e-39, 0.0000e+00]])




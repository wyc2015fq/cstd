# Pytorch之Variable - qq_37385726的博客 - CSDN博客





2018年08月15日 16:41:04[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：177
所属专栏：[Pytorch](https://blog.csdn.net/column/details/26172.html)









# **目录**

[1.简介](#1.%E7%AE%80%E4%BB%8B)

[代码1](#%E4%BB%A3%E7%A0%811)

[输出1](#%E8%BE%93%E5%87%BA1)

[代码2](#%E4%BB%A3%E7%A0%812)

[输出2](#%E8%BE%93%E5%87%BA2)

[代码3](#%E4%BB%A3%E7%A0%813)

[输出3](#%E8%BE%93%E5%87%BA3)

[代码4](#%E4%BB%A3%E7%A0%814)

[输出4](#%E8%BE%93%E5%87%BA4)

[2.grad属性](#2.grad%E5%B1%9E%E6%80%A7)

# 1.简介

torch.autograd.Variable是Autograd的核心类，它封装了Tensor，并整合了反向传播的相关实现

> 
**Variable和tensor的区别和联系**

Variable是篮子，而tensor是鸡蛋，鸡蛋应该放在篮子里才能方便拿走（定义variable时一个参数就是tensor）

Variable这个篮子里除了装了tensor外还有requires_grad参数，表示是否需要对其求导，默认为False

Variable这个篮子呢，自身有一些属性

比如grad，梯度variable.grad是d(y)/d(variable)保存的是变量y对variable变量的梯度值，如果requires_grad参数为False，所以variable.grad返回值为None，如果为True，返回值就为对variable的梯度值

比如grad_fn，对于用户自己创建的变量（Variable()）grad_fn是为none的，也就是不能调用backward函数，但对于由计算生成的变量，如果存在一个生成中间变量的requires_grad为true，那其的grad_fn不为none，反则为none

比如data，这个就很简单，这个属性就是装的鸡蛋（tensor）


Varibale包含三个属性：

data：存储了Tensor，是本体的数据

grad：保存了data的梯度，本事是个Variable而非Tensor，与data形状一致

grad_fn：指向Function对象，用于反向传播的梯度计算之用



### 代码1

```python
import numpy as np
import torch
from torch.autograd import Variable

x = Variable(torch.ones(2,2),requires_grad = False)
temp = Variable(torch.zeros(2,2),requires_grad = True)

y = x + temp + 2
y = y.mean()  #求平均数

y.backward()  #反向传递函数，用于求y对前面的变量（x）的梯度
print(x.grad) # d(y)/d(x)
```

### 输出1

none

(因为requires_grad=False)



### 代码2

```python
import numpy as np
import torch
from torch.autograd import Variable

x = Variable(torch.ones(2,2),requires_grad = False)
temp = Variable(torch.zeros(2,2),requires_grad = True)


y = x + temp + 2
y = y.mean()  #求平均数

y.backward()  #反向传递函数，用于求y对前面的变量（x）的梯度
print(temp.grad)  # d(y)/d(temp)
```

### 输出2

tensor([[0.2500, 0.2500],

        [0.2500, 0.2500]])



### 代码3

```python
import numpy as np
import torch
from torch.autograd import Variable

x = Variable(torch.ones(2,2),requires_grad = False)
temp = Variable(torch.zeros(2,2),requires_grad = True)


y = x + 2
y = y.mean()  #求平均数

y.backward()  #反向传递函数，用于求y对前面的变量（x）的梯度
print(x.grad)  # d(y)/d(x)
```

### 输出3

Traceback (most recent call last):

  File "path", line 12, in <module>

    y.backward() 

(报错了，因为生成变量y的中间变量只有x，而x的requires_grad是False，所以y的grad_fn是none)

### 代码4

```python
import numpy as np
import torch
from torch.autograd import Variable

x = Variable(torch.ones(2,2),requires_grad = False)
temp = Variable(torch.zeros(2,2),requires_grad = True)


y = x + 2
y = y.mean()  #求平均数

#y.backward()  #反向传递函数，用于求y对前面的变量（x）的梯度
print(y.grad_fn)  # d(y)/d(x)
```

### 输出4

none



# 2.grad属性

在每次backward后，grad值是会累加的，所以利用BP算法，每次迭代是需要将grad清零的。

> 
**x.grad.data.zero_()**

**(in-place操作需要加上_，即zero_)**





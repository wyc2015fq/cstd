# PyTorch基本用法(二)——Variable - YZXnuaa的博客 - CSDN博客
2018年06月01日 20:04:09[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：3705
本文主要是PyTorch中Variable变量的一些用法。
```python
import torch
from torch.autograd import Variable
tensor = torch.FloatTensor([[1, 2], [3, 4]])
# 定义Variable, requires_grad用来指定是否需要计算梯度
variable = Variable(tensor, requires_grad = True)
print tensor
print variable
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
```
1  2
 3  4
[torch.FloatTensor of size 2x2]
Variable containing:
 1  2
 3  4
[torch.FloatTensor of size 2x2]
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
```python
# 计算x^2的均值
tensor_mean = torch.mean(tensor * tensor)
variable_mean = torch.mean(variable * variable)
print tensor_mean
print variable_mean
```
- 1
- 2
- 3
- 4
- 5
```
7.5
Variable containing:
 7.5000
[torch.FloatTensor of size 1]
```
- 1
- 2
- 3
- 4
- 5
```python
# variable进行反向传播
# 梯度计算如下：
# variable_mean = 1/4 * sum(variable * variable)
# d(variable_mean)/d(variable) = 1/4 * 2 * variable = 1/2 * variable
variable_mean.backward()
# 输出variable中的梯度
print variable.grad
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
```
Variable containing:
 0.5000  1.0000
 1.5000  2.0000
[torch.FloatTensor of size 2x2]
```
- 1
- 2
- 3
- 4
- 5
```python
# *表示逐元素点乘,不是矩阵乘法
print tensor * tensor
print variable * variable
```
- 1
- 2
- 3
```
1   4
  9  16
[torch.FloatTensor of size 2x2]
Variable containing:
  1   4
  9  16
[torch.FloatTensor of size 2x2]
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
```python
# 输出variable中的data, data是tensor
print variable.data
```
- 1
- 2
```
1  2
 3  4
[torch.FloatTensor of size 2x2]
```

# Pytorch学习笔记（一） - YZXnuaa的博客 - CSDN博客
2018年06月01日 18:40:40[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：129
本篇博客主要向大家介绍Pytorch中view()、squeeze()、unsqueeze()、torch.max()函数，这些函数虽然简单，但是在 
神经网络编程总却经常用到，希望大家看了这篇博文能够把这些函数的作用弄清楚。
```python
import torch
```
- 1
```python
a=torch.Tensor(2,3)
a
```
- 1
- 2
```
3.8686e+25  9.1836e-39  1.2771e-40
 9.0079e+15  1.6751e-37  2.9775e-41
 [torch.FloatTensor of size 2x3]
```
- 1
- 2
- 3
- 4
view()函数作用是将一个多行的Tensor,拼接成一行。
```python
a.view(1,-1)
```
- 1
```
3.8686e+25  9.1836e-39  1.2771e-40  9.0079e+15  1.6751e-37  2.9775e-41
 [torch.FloatTensor of size 1x6]
```
- 1
- 2
- 3
下面是torch中squeeze()和unsqueeze()两个函数。
```python
b=torch.Tensor(1,3)
b
```
- 1
- 2
```
3.3447e+30  6.1237e-43  5.9179e+32
 [torch.FloatTensor of size 1x3]
```
- 1
- 2
- 3
```python
b.squeeze(0)
```
- 1
```
3.3447e+30
 6.1237e-43
 5.9179e+32
[torch.FloatTensor of size 3]
```
- 1
- 2
- 3
- 4
- 5
```python
b.squeeze(1)
```
- 1
```
3.3447e+30  6.1237e-43  5.9179e+32
[torch.FloatTensor of size 1x3]
```
- 1
- 2
- 3
squeeze中的参数0、1分别代表第一、第二维度，squeeze(0)表示如果第一维度值为1，则去掉，否则不变。故b的维度(1,3),可去掉1成(3),但不可去掉3。
```python
c=torch.Tensor(3)
c
```
- 1
- 2
```
7.5589e+28
 5.2839e-11
 1.8888e+31
[torch.FloatTensor of size 3]
```
- 1
- 2
- 3
- 4
- 5
```python
c.unsqueeze(0)
```
- 1
```
7.5589e+28  5.2839e-11  1.8888e+31
[torch.FloatTensor of size 1x3]
```
- 1
- 2
- 3
```python
c.unsqueeze(1)
```
- 1
```
7.5589e+28
 5.2839e-11
 1.8888e+31
[torch.FloatTensor of size 3x1]
```
- 1
- 2
- 3
- 4
- 5
unsqueeze()与squeeze()作用相反。参数代表的意思相同。
```python
d=torch.Tensor([[1,3],[2,4]])
d
```
- 1
- 2
```
1  3
 2  4
[torch.FloatTensor of size 2x2]
```
- 1
- 2
- 3
- 4
```python
torch.max(d,0)
```
- 1
```
(
  2  4
 [torch.FloatTensor of size 1x2], 
  1  1
 [torch.LongTensor of size 1x2])
```
- 1
- 2
- 3
- 4
- 5
- 6
torch.max()返回两个结果，第一个是最大值，第二个是对应的索引值；第二个参数 0 代表按列取最大值并返回对应的行索引值，1 代表按行取最大值并返回对应的列索引值。
```python
torch.max(d,1)
```
- 1
```
(
  3
  4
 [torch.FloatTensor of size 2x1], 
  1
  1
 [torch.LongTensor of size 2x1])
```

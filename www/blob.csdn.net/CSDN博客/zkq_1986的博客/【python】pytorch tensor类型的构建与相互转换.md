# 【python】pytorch: tensor类型的构建与相互转换 - zkq_1986的博客 - CSDN博客





2018年12月27日 10:21:20[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：192








Summary

主要包括以下三种途径：

使用独立的函数；

使用torch.type()函数；

使用type_as(tesnor)将张量转换为给定类型的张量。

使用独立函数

import torch

tensor = torch.randn(3, 5)

print(tensor)

# torch.long() 将tensor投射为long类型

long_tensor = tensor.long()

print(long_tensor)

# torch.half()将tensor投射为半精度浮点类型

half_tensor = tensor.half()

print(half_tensor)

# torch.int()将该tensor投射为int类型

int_tensor = tensor.int()

print(int_tensor)

# torch.double()将该tensor投射为double类型

double_tensor = tensor.double()

print(double_tensor)

# torch.float()将该tensor投射为float类型

float_tensor = tensor.float()

print(float_tensor)

# torch.char()将该tensor投射为char类型

char_tensor = tensor.char()

print(char_tensor)

# torch.byte()将该tensor投射为byte类型

byte_tensor = tensor.byte()

print(byte_tensor)

# torch.short()将该tensor投射为short类型

short_tensor = tensor.short()

print(short_tensor)


-0.5841 -1.6370  0.1353  0.6334 -3.0761

-0.2628  0.1245  0.8626  0.4095 -0.3633

 1.3605  0.5055 -2.0090  0.8933 -0.6267

[torch.FloatTensor of size 3x5]



 0 -1  0  0 -3

 0  0  0  0  0

 1  0 -2  0  0

[torch.LongTensor of size 3x5]



-0.5840 -1.6367  0.1353  0.6333 -3.0762

-0.2627  0.1245  0.8628  0.4094 -0.3633

 1.3604  0.5054 -2.0098  0.8936 -0.6265

[torch.HalfTensor of size 3x5]



 0 -1  0  0 -3

 0  0  0  0  0

 1  0 -2  0  0

[torch.IntTensor of size 3x5]



-0.5841 -1.6370  0.1353  0.6334 -3.0761

-0.2628  0.1245  0.8626  0.4095 -0.3633

 1.3605  0.5055 -2.0090  0.8933 -0.6267

[torch.DoubleTensor of size 3x5]



-0.5841 -1.6370  0.1353  0.6334 -3.0761

-0.2628  0.1245  0.8626  0.4095 -0.3633

 1.3605  0.5055 -2.0090  0.8933 -0.6267

[torch.FloatTensor of size 3x5]



 0 -1  0  0 -3

 0  0  0  0  0

 1  0 -2  0  0

[torch.CharTensor of size 3x5]



   0  255    0    0  253

   0    0    0    0    0

   1    0  254    0    0

[torch.ByteTensor of size 3x5]



 0 -1  0  0 -3

 0  0  0  0  0

 1  0 -2  0  0

[torch.ShortTensor of size 3x5]


其中，torch.Tensor、torch.rand、torch.randn 均默认生成 torch.FloatTensor型 ：

import torch

tensor = torch.Tensor(3, 5)

assert isinstance(tensor, torch.FloatTensor)

tensor = torch.rand(3, 5)

assert isinstance(tensor, torch.FloatTensor)

tensor = torch.randn(3, 5)

assert isinstance(tensor, torch.FloatTensor)


使用torch.type()函数

type(new_type=None, async=False)

import torch

tensor = torch.randn(3, 5)

print(tensor)

int_tensor = tensor.type(torch.IntTensor)

print(int_tensor)


-0.4449  0.0332  0.5187  0.1271  2.2303

 1.3961 -0.1542  0.8498 -0.3438 -0.2834

-0.5554  0.1684  1.5216  2.4527  0.0379

[torch.FloatTensor of size 3x5]



 0  0  0  0  2

 1  0  0  0  0

 0  0  1  2  0

[torch.IntTensor of size 3x5]


使用type_as(tesnor)将张量转换为给定类型的张量

import torch

tensor_1 = torch.FloatTensor(5)

tensor_2 = torch.IntTensor([10, 20])

tensor_1 = tensor_1.type_as(tensor_2)

assert isinstance(tensor_1, torch.IntTensor)


作者：JNingWei 

来源：CSDN 

原文：https://blog.csdn.net/jningwei/article/details/79849600 




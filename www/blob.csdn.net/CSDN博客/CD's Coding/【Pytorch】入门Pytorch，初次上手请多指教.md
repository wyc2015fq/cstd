# 【Pytorch】入门Pytorch，初次上手请多指教 - CD's Coding - CSDN博客





2017年09月27日 08:32:02[糖果天王](https://me.csdn.net/okcd00)阅读数：883








## 前言

本篇为在`.ipynb`页面上的自学尝试记录， 

可以在本人的个人主页上查看或下载自行测试，关于Windows10如何配置Pytorch请移步前篇文章即可~
- [传送到](https://okcd00.github.io/articles/2017-09/%E3%80%90Pytorch%E3%80%91Torch_Basic_Learning.html) 个人主页 (Github Pages) 
- [传送到](https://okcd00.oschina.io/articles/2017-09/%E3%80%90Pytorch%E3%80%91Torch_Basic_Learning.html) 个人主页 (国内镜像源，加载速度稍快) 
- [传送到](https://github.com/okcd00/okcd00.github.io/blob/master/wlog/backup/Torch_Basic_Learning.ipynb) Ipynb的查看或下载

## Pytorch
- Tensor computation (like numpy) with strong GPU acceleration
- PyTorch is an optimized tensor library for deep learning using GPUs and CPUs.
- It has a CUDA counterpart, that enables you to run your tensor computations on an NVIDIA GPU with compute capability >= 3.0.

```python
from __future__ import print_function
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torch.autograd as autograd
from torch.autograd import Variable

import numpy as np
print("Import success.\nTorch Version:{}".format(torch.__version__))
```

```
Import success.
Torch Version:0.2.1+a4fc05a
```
|Package|Description|
|----|----|
|torch|a Tensor library like NumPy, with strong GPU support|
|torch.autograd|a tape based automatic differentiation library that supports all differentiable Tensor operations in torch|
|torch.nn|a neural networks library deeply integrated with autograd designed for maximum flexibility|
|torch.optim|an optimization package to be used with torch.nn with standard optimization methods such as SGD, RMSProp, LBFGS, Adam etc.|
|torch.multiprocessing|python multiprocessing, but with magical memory sharing of torch Tensors across processes. Useful for data loading and hogwild training.|
|torch.utils|DataLoader, Trainer and other utility functions for convenience|
|torch.legacy(.nn/.optim)|legacy code that has been ported over from torch for backward compatibility reasons|

### Brief view in pytorch

```python
torch.set_printoptions(
    precision=None, # Number of digits of precision for floating point output (default 8).
    threshold=None, # Total number of array elements which trigger summarization rather than full repr (default 1000).
    edgeitems=None, # Number of array items in summary at beginning and end of each dimension (default 3).
    linewidth=None, # The number of characters per line for the purpose of inserting line breaks (default 80). 
                    # Thresholded matricies will ignore this parameter.
    profile=None,   # Sane defaults for pretty printing. Can override with any of the above options. (default, short, full) 
)
```

```python
np_mat = np.random.randn(1,2,3,4)
tc_mat = torch.randn(1,2,3,4)
print("Numpy Matrix:\n", np_mat)
print("\nTorch Matrix:", tc_mat)

torch.set_printoptions(precision=5) # change print options, just link numpy
print("\nTorch Matrix:", tc_mat)
torch.set_printoptions(profile='default') # back to default_pretty_printing
# print("\nTorch Matrix:", tc_mat)
```

```
Numpy Matrix:
 [[[[-1.26951553 -0.5783194  -0.2817905  -1.91081633]
   [-1.03255087  0.09356858  0.87141257  0.24527875]
   [-1.57634325 -0.95006681 -0.54418479 -0.81721992]]

  [[ 1.52620998  1.19114026  0.12362855  0.30176001]
   [-0.18951867  0.33470665 -1.13370011  1.17999206]
   [ 0.17942397 -0.15441461  0.53326092 -0.22955392]]]]

Torch Matrix: 
(0 ,0 ,.,.) = 
  1.6238  0.4029 -1.6028  0.4393
  0.0485 -0.5608 -1.3842  1.6696
  0.9837  0.7565 -0.8661 -0.6509

(0 ,1 ,.,.) = 
 -0.3699 -0.8046  0.3459  0.3806
  0.0225 -0.0216  0.5001  0.4924
  0.2519 -1.1100 -1.5480 -0.0549
[torch.FloatTensor of size 1x2x3x4]


Torch Matrix: 
(0 ,0 ,.,.) = 
 1.62381 0.40293 -1.60275 0.43934
 0.04847 -0.56078 -1.38419 1.66964
 0.98369 0.75650 -0.86610 -0.65095

(0 ,1 ,.,.) = 
 -0.36994 -0.80465 0.34593 0.38057
 0.02247 -0.02161 0.50013 0.49238
 0.25189 -1.10998 -1.54797 -0.05489
[torch.FloatTensor of size 1x2x3x4]
```

### Easy defination and assignment with numpy
- torch.from_numpy(< type numpy.array >) creates a Tensor from a numpy.ndarray.

> 
The returned tensor and ndarray **share the same memory**. Modifications to the tensor will be reflected in the ndarray and vice versa.  

  The returned tensor is **not resizable**.


```python
tc_mat = torch.from_numpy(np_mat)
print("Numpy Matrix:\n", np_mat)
print("\nTorch matrix from Numpy:", tc_mat)
```

```
Numpy Matrix:
 [[[[ 0.28134406 -0.34748676  0.83155334  0.85618986]
   [-0.95961146 -0.68374939 -1.67800331  0.84947823]
   [-0.72478517  0.71535117 -2.02988345 -0.1911564 ]]

  [[ 1.04884339  0.35382358 -0.69535152  0.27244267]
   [-0.18157492 -0.02892887 -0.54348221  1.49079913]
   [ 0.6273026   0.86512992 -1.02024843 -0.58441433]]]]
Torch matrix from Numpy:

(0 ,0 ,.,.) = 
  0.2813 -0.3475  0.8316  0.8562
 -0.9596 -0.6837 -1.6780  0.8495
 -0.7248  0.7154 -2.0299 -0.1912

(0 ,1 ,.,.) = 
  1.0488  0.3538 -0.6954  0.2724
 -0.1816 -0.0289 -0.5435  1.4908
  0.6273  0.8651 -1.0202 -0.5844
[torch.DoubleTensor of size 1x2x3x4]
```

### Data preprocessing
- torch.clamp(x, min, max)
- torch.cat((x, x, …, x), 0)

```python
a = torch.randn(3,4)
print('Original Matrix a:', a)
print('\nAfter torch.clamp:', torch.clamp(a, min=-0.5, max=0.5))

cat0, cat1 = torch.cat((a, a, a), 0), torch.cat((a, a), 1)
print('\nAfter torch.cat((a,a), axis=1):', cat1)
print('\nAfter torch.cat((a,a,a), axis=0):', cat0)
```

```
Original Matrix a: 
-0.0708  0.7695  0.4194 -0.2349
-1.6515  0.4604  0.7691  0.3826
-0.7259 -0.0289 -0.7962  0.5046
[torch.FloatTensor of size 3x4]


After torch.clamp: 
-0.0708  0.5000  0.4194 -0.2349
-0.5000  0.4604  0.5000  0.3826
-0.5000 -0.0289 -0.5000  0.5000
[torch.FloatTensor of size 3x4]


After torch.cat((a,a), axis=1): 
-0.0708  0.7695  0.4194 -0.2349 -0.0708  0.7695  0.4194 -0.2349
-1.6515  0.4604  0.7691  0.3826 -1.6515  0.4604  0.7691  0.3826
-0.7259 -0.0289 -0.7962  0.5046 -0.7259 -0.0289 -0.7962  0.5046
[torch.FloatTensor of size 3x8]


After torch.cat((a,a,a), axis=0): 
-0.0708  0.7695  0.4194 -0.2349
-1.6515  0.4604  0.7691  0.3826
-0.7259 -0.0289 -0.7962  0.5046
-0.0708  0.7695  0.4194 -0.2349
-1.6515  0.4604  0.7691  0.3826
-0.7259 -0.0289 -0.7962  0.5046
-0.0708  0.7695  0.4194 -0.2349
-1.6515  0.4604  0.7691  0.3826
-0.7259 -0.0289 -0.7962  0.5046
[torch.FloatTensor of size 9x4]
```

### Data Select
- torch.masked_select
- torch.index_select 

> 
  Returned Tensor does not use the same storage as the original Tensor 

  and `masked_select returns a 1-dim tensor`

```python
select = torch.LongTensor([0, 2])
sel0 = torch.index_select(a, 0, select)
sel1 = torch.index_select(a, 1, select)
print('Original Matrix a:', a)

mask = a.gt(0.5) # if element is greater than 0.5
print('\nMask for greater_than_0.5:', mask)
print('\nAfter torch.masked_select(a, mask):', torch.masked_select(a, mask))

print('We select index: select = torch.LongTensor([0, 2])')
print('\nAfter torch.index_select(a, axis=0, select):', sel0)
print('\nAfter torch.index_select(a, axis=1, select):', sel1)
```

```
Original Matrix a: 
-0.0708  0.7695  0.4194 -0.2349
-1.6515  0.4604  0.7691  0.3826
-0.7259 -0.0289 -0.7962  0.5046
[torch.FloatTensor of size 3x4]


Mask for greater_than_0.5: 
 0  1  0  0
 0  0  1  0
 0  0  0  1
[torch.ByteTensor of size 3x4]


After torch.masked_select(a, mask): 
 0.7695
 0.7691
 0.5046
[torch.FloatTensor of size 3]

We select index: select = torch.LongTensor([0, 2])

After torch.index_select(a, axis=0, select): 
-0.0708  0.7695  0.4194 -0.2349
-0.7259 -0.0289 -0.7962  0.5046
[torch.FloatTensor of size 2x4]


After torch.index_select(a, axis=1, select): 
-0.0708  0.4194
-1.6515  0.7691
-0.7259 -0.7962
[torch.FloatTensor of size 3x2]
```

### Data Squeeze
- torch.squeeze

> 
You can select which axis to be squeezed


```python
z = torch.zeros(4,5,1,2)
print("Size of Z:                 \t ", z.size())
print("Size of Z after squeeze(z):\t ", torch.squeeze(z).size())
print("Size of Z after squeeze(z, 1):\t ", torch.squeeze(z, 1).size())
print("Size of Z after squeeze(z, 2):\t ", torch.squeeze(z, 2).size())
```

```
Size of Z:                    torch.Size([4, 5, 1, 2])
Size of Z after squeeze(z):   torch.Size([4, 5, 2])
Size of Z after squeeze(z, 1):    torch.Size([4, 5, 1, 2])
Size of Z after squeeze(z, 2):    torch.Size([4, 5, 2])
```
- [Serialization](http://pytorch.org/docs/master/notes/serialization.html#recommend-saving-models)
- [Parallelism](http://pytorch.org/docs/master/torch.html#parallelism)
- torch.`set_num_threads(int)`
- torch.`get_num_threads()` → int

- Check Matrix Comparison 
- torch.`eq()`/`ge()`/`gt()` -> Tensor
- torch.`equal()` -> Bool


### In-place random sampling

There are a few more in-place random sampling functions defined on Tensors as well. Click through to refer to their documentation:
|Distribution|description|
|----|----|
|`torch.Tensor.bernoulli_()`|in-place version of torch.bernoulli()|
|`torch.Tensor.cauchy_()`|numbers drawn from the Cauchy distribution|
|`torch.Tensor.exponential_()`|numbers drawn from the exponential distribution|
|`torch.Tensor.geometric_()`|elements drawn from the geometric distribution|
|`torch.Tensor.log_normal_()`|samples from the log-normal distribution|
|`torch.Tensor.normal_()`|in-place version of torch.normal()|
|`torch.Tensor.random_()`|numbers sampled from the discrete uniform distribution|
|`torch.Tensor.uniform_()`|numbers sampled from the uniform distribution|

### Variable for requires_grad or not

```python
from torch.autograd import Variable
# http://pytorch.org/docs/master/autograd.html#torch.autograd.Function
x = Variable(torch.randn(5, 5))
y = Variable(torch.randn(5, 5))
z = Variable(torch.randn(5, 5), requires_grad=True)
a = x + y
b = a + z
print("Variable a=x+y requires grad: ", a.requires_grad)
print("Variable b=a+z requires grad: ", b.requires_grad)
```

```
Variable a=x+y requires grad:  False
Variable b=a+z requires grad:  True
```

### Finished Basic

I can’t wait for Machine learning! 

Let’s start new learning into [torch.nn](http://pytorch.org/docs/master/nn.html)~

![dynamic_graph](http://pytorch.org/static/img/dynamic_graph.gif)





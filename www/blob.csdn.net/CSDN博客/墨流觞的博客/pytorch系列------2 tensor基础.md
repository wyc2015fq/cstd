# pytorch系列------2 tensor基础 - 墨流觞的博客 - CSDN博客





2018年11月05日 17:22:25[墨氲](https://me.csdn.net/dss_dssssd)阅读数：440
所属专栏：[pytorch系列教程](https://blog.csdn.net/column/details/29549.html)[python3 pytorch0.4系列教程](https://blog.csdn.net/column/details/29714.html)









## pytorch Tensor的基本用法
- Tensor的创建
- 索引，合并，切片
- 初始化
- 数学操作

### 1. Tensor的创建

#### 1) 随机数字

`torch.rand(*sizes, out=None) → Tensor`

返回一个张量，包含了从区间[0, 1)的均匀分布中抽取的一组随机数。张量的形状由参数sizes定义

```python
# torch.rand(sizes) -> [0,1)之间的均匀分布
x = torch.rand(2,3)
x
```

out:

> 
0.9161  0.7135  0.8969

0.7552  0.8760  0.2236

[torch.FloatTensor of size 2x3]
`torch.randn(*sizes, out=None) → Tensor`

返回一个张量，包含了从标准正态分布（均值为0，方差为1，即高斯白噪声）中抽取的一组随机数。张量的形状由参数sizes定义

```python
# torch.randn(sizes) -> Z(0,1)
x = torch.randn(2,3)
x
```

out:

> 
-1.9352  0.9779  1.7401

2.0432 -0.1962 -0.1177

[torch.FloatTensor of size 2x3]
`torch.randperm(n)`

返回张量， 0~n之间的一个全排列

```python
# torch.randperm(n) -> permutation of 0~n
x = torch.randperm(5)
x
```

out:

> 
3

1

4

0

2

[torch.LongTensor of size 5]
#### 2） zeros, ones, eye, arrange

`torch.zeros(*sizes, out=None, dtype=None, layout=torch.strided, device=None, requires_grad=False) → Tensor`

返回张量，用0填充，size有参数sizes确定

```
# torch.zeros(2,3) -> [[0,0,0],[0,0,0]]
x = torch.zeros(2,3)
x
```

out:

> 
0  0  0

0  0  0

[torch.FloatTensor of size 2x3]
`torch.ones(*sizes, out=None, dtype=None, layout=torch.strided, device=None, requires_grad=False) → Tensor`

返回张量，用1填充，size有参数sizes确定

```python
# torch.ones(2,3) -> [[0,0,0],[0,0,0]]
x = torch.ones(2,3)
x
```

out:

> 
1  1  1

1  1  1

[torch.FloatTensor of size 2x3]
`torch.arange(start=0, end, step=1, out=None, dtype=None, layout=torch.strided, device=None, requires_grad=False) → Tensor`

返回一维张量，在[start, end)区间内，按step为间隔返回一个等差数列，
$$out_{i+1} = out_i + step$$
```
# torch.arange(start,end,step=1) -> [start,end) with step

y = torch.arange(1, 4)
x = torch.arange(0,3,step=0.5)
y
x
```

out:

> 
tensor([ 1,  2,  3])

0.0000

0.5000

1.0000

1.5000

2.0000

2.5000

[torch.FloatTensor of size 6]
`torch.eye(n, m=None, out=None, dtype=None, layout=torch.strided, device=None, requires_grad=False) → Tensor`

返回二维对角矩阵张量，对角线上为1，其余为0

```python
torch.eye(3)
```

out:

> 
tensor([[ 1.,  0.,  0.],

[ 0.,  1.,  0.],

[ 0.,  0.,  1.]])
### 3) Tensor 数据类型

所有的数据类型都在这里
[https://pytorch.org/docs/stable/tensors.html#torch-tensor](https://pytorch.org/docs/stable/tensors.html#torch-tensor)

创建size为[2,3]的浮点数张量
```python
# torch.FloatTensor(size or list)
x = torch.FloatTensor(2,3)
x
```

out:

> 
-7.4341e-17  4.5629e-41 -7.4341e-17

4.5629e-41 -7.4342e-17  2.9427e-44

[torch.FloatTensor of size 2x3]
创建数据为2,3的浮点数张量

```python
# torch.FloatTensor(size or list)
x = torch.FloatTensor([2,3])
x
```

out:

> 
2

3

[torch.FloatTensor of size 2]
转换为整型张量

```python
# tensor.type_as(tensor_type)
x = x.type_as(torch.IntTensor())
x
```

out:

> 
2

3

[torch.IntTensor of size 2]
#### 4) Tensor 和 numpy的相互转换

从numpy转换为Tensor

```python
import numpy as np

# torch.from_numpy(ndarray) -> tensor

x1 = np.ndarray(shape=(2,3), dtype=int,buffer=np.array([1,2,3,4,5,6]))
x2 = torch.from_numpy(x1)

x2
```

out:

> 
1  2  3

4  5  6

[torch.LongTensor of size 2x3]
从张量转换为numpy数组

```python
# tensor.numpy() -> ndarray
x3 = x2.numpy()
x3
```

out:>

array([[1, 2, 3],

[4, 5, 6]])
#### 5) GPU和CPU上的Tensor

创建Tensor

```python
x = torch.FloatTensor([[1,2,3],[4,5,6]])
x
```

out:

> 
1  2  3

4  5  6

[torch.FloatTensor of size 2x3]
转换为GPU上：

```python
x_gpu = x.cuda()
x_gpu
```

out:

> 
1  2  3

4  5  6

[torch.cuda.FloatTensor of size 2x3 (GPU 0)]
从GPU格式转换为CPU

```python
x_cpu = x_gpu.cpu()
x_cpu
```

out:

> 
1  2  3

4  5  6

[torch.FloatTensor of size 2x3]
#### 6) tensor的size

也就是tensor的形状

创建维度为[10, 12, 3,3]的浮点数张量

```python
# tensor.size() -> indexing also possible

x = torch.FloatTensor(10,12,3,3)

x.size()[:]
```

out:

> 
torch.Size([10, 12, 3, 3])


### 2. 索引（indexing）,切片(slicing), 合并(joining), 更改形状(reshaping)

#### 1) indexing

`torch.index_select(input, dim, index)`

在dim的维度上，按index索引

比如下列就是在行上，索引第0和3行

```python
# torch.index_select(input, dim, index)

x = torch.rand(4,3)
out = torch.index_select(x,0,torch.LongTensor([0,3]))

x,out
```

out:

> 
(

0.9495  0.8593  0.2616

0.2279  0.0857  0.7603

0.4439  0.2024  0.8021

0.3477  0.6363  0.5002

[torch.FloatTensor of size 4x3],

0.9495  0.8593  0.2616

0.3477  0.6363  0.5002

[torch.FloatTensor of size 2x3])
也支持python和numpy的索引方式

```python
x[:,0],x[0,:],x[0:2,0:2]
```

out:

> 
(

0.9495

0.2279

0.4439

0.3477

[torch.FloatTensor of size 4],

0.9495

0.8593

0.2616

[torch.FloatTensor of size 3],

0.9495  0.8593

0.2279  0.0857

[torch.FloatTensor of size 2x2])
`torch.masked_select(input, mask, out=None) → Tensor`

返回一维张量，mask为一个ByteTensor，返回mask值为1的位置对应input的值，

mask和input的size不一定相同，但要满足广播机制

```python
# torch.masked_select(input, mask)

x = torch.randn(2,3)
mask = torch.ByteTensor([[0,0,1],[0,1,0]])
out = torch.masked_select(x,mask)

x, mask, out
```

out:

> 
(

-2.0702 -0.3064 -0.8124

0.2870  0.2648  0.4418

[torch.FloatTensor of size 2x3],

0  0  1

0  1  0

[torch.ByteTensor of size 2x3],

-0.8124

0.2648

[torch.FloatTensor of size 2])
来看一个更有意义的场景，

返回x中大于0.5的值：

```python
x = torch.randn(3, 4)
x
'''
tensor([[ 0.3552, -2.3825, -0.8297,  0.3477],
        [-1.2035,  1.2252,  0.5002,  0.6248],
        [ 0.1307, -2.0608,  0.1244,  2.0139]])
 '''
 # 值大于0.5为1，否则为0
mask = x.ge(0.5)
mask
'''
tensor([[ 0,  0,  0,  0],
        [ 0,  1,  1,  1],
        [ 0,  0,  0,  1]], dtype=torch.uint8)
 '''
 torch.masked_select(x, mask)

'''
tensor([ 1.2252,  0.5002,  0.6248,  2.0139])
'''
```

#### 2) joining

`torch.cat(seq, dim=0) -> concatenate tensor along dim`

最常见的是二维数据， dim为0是按行排列，dim为1是按列排列。

而对于二维数组而言dim=-1和dim=1完全等价，因为dim=-1表示size的最后一个维度。
```python
# torch.cat(seq, dim=0) -> concatenate tensor along dim

x = torch.FloatTensor([[1,2,3],[4,5,6]])
y = torch.FloatTensor([[-1,-2,-3],[-4,-5,-6]])
z1 = torch.cat([x,y],dim=0)
z2 = torch.cat([x,y],dim=1)

x,y,z1,z2
```

out:

> 
(

1  2  3

4  5  6

[torch.FloatTensor of size 2x3],

-1 -2 -3

-4 -5 -6

[torch.FloatTensor of size 2x3],

1  2  3

4  5  6

-1 -2 -3

-4 -5 -6

[torch.FloatTensor of size 4x3],

1  2  3 -1 -2 -3

4  5  6 -4 -5 -6

[torch.FloatTensor of size 2x6])
#### 3) slicing 切片

`torch.chunk(tensor, chunks, dim=0) → List of Tensors`

将tensor在维度dim上分割为特定数量的块（chunks）

例如下列代码第一行将z1在行上分割为两部分，第二行将z1在列上分为3部分

```
# torch.chunk(tensor, chunks, dim=0) -> tensor into num chunks

x_1, x_2 = torch.chunk(z1,2,dim=0)
y_1, y_2, y_3 = torch.chunk(z1,3,dim=1)

z1,x_1,x_2,z1,y_1,y_2,y_3
```

out:

> 
(

1  2  3

4  5  6

-1 -2 -3

-4 -5 -6

[torch.FloatTensor of size 4x3],

1  2  3

4  5  6

[torch.FloatTensor of size 2x3],

-1 -2 -3

-4 -5 -6

[torch.FloatTensor of size 2x3],

1  2  3

4  5  6

-1 -2 -3

-4 -5 -6

[torch.FloatTensor of size 4x3],

1

4

-1

-4

[torch.FloatTensor of size 4x1],

2

5

-2

-5

[torch.FloatTensor of size 4x1],

3

6

-3

-6

[torch.FloatTensor of size 4x1])
和chunk的作用一样

```python
# torch.split(tensor,split_size,dim=0) -> split into specific size

x1,x2 = torch.split(z1,2,dim=0)
y1 = torch.split(z1,2,dim=1) 

z1,x1,x2,y1
```

out:

> 
(

1  2  3

4  5  6

-1 -2 -3

-4 -5 -6

[torch.FloatTensor of size 4x3],

1  2  3

4  5  6

[torch.FloatTensor of size 2x3],

-1 -2 -3

-4 -5 -6

[torch.FloatTensor of size 2x3], (

1  2

4  5

-1 -2

-4 -5

[torch.FloatTensor of size 4x2],

3

6

-3

-6

[torch.FloatTensor of size 4x1]))
#### 4) squeezing

`torch.squeeze(input,dim=None) -> reduce dim by 1`

去掉张量中维度大小为1的dim

创建size为[10, 1,3,1,4]的张量，通过squeeze函数，去掉1的dim，变为[10,3,4]
```python
# torch.squeeze(input,dim=None) -> reduce dim by 1

x1 = torch.FloatTensor(10,1,3,1,4)
x2 = torch.squeeze(x1)

x1.size(),x2.size()
```

out:

> 
(torch.Size([10, 1, 3, 1, 4]), torch.Size([10, 3, 4]))


`torch.unsqueeze(input,dim=None) -> add dim by 1`

在指定的dim上添加大小为1维度

比如下列代码，在dim=0的维度上增加纬度值为1 的维度，所以张量size变为[1, 10, 3, 4]
```python
# torch.unsqueeze(input,dim=None) -> add dim by 1

x1 = torch.FloatTensor(10,3,4)
x2 = torch.unsqueeze(x1,dim=0)

x1.size(),x2.size()
```

out:

> 
(torch.Size([10, 3, 4]), torch.Size([1, 10, 3, 4]))


#### 5) reshaping

`tensor.view(size)`:改变张量的形状size

经常在最后一层卷积层，第一层全连接层之前使用，展开为[batch, -1]的形状

```python
# tensor.view(size)

x1 = torch.FloatTensor(10,3,4)
# 展开为1维
x2 = x1.view(-1)
x3 = x1.view(5,-1)
x4 = x1.view(3,10,-1)

x1.size(), x2.size(), x3.size(), x4.size()
```

out:>

(torch.Size([10, 3, 4]),

torch.Size([120]),

torch.Size([5, 24]),

torch.Size([3, 10, 4]))
### 3. 初始化

```python
import torch.nn.init as init
# 均匀分布
x1 = init.uniform(torch.FloatTensor(3,4),a=0,b=9) 
# 正太分布
x2 = init.normal(torch.FloatTensor(3,4),std=0.2)
# 初始化为常数
x3 = init.constant(torch.FloatTensor(3,4),3.1415)

x1,x2,x3
```

out:

> 
(

7.8079  3.7811  4.0829  8.0698

8.4885  5.2119  2.5769  1.4455

3.0500  3.9946  5.6066  4.9679

[torch.FloatTensor of size 3x4],

-0.4025 -0.2582 -0.0780 -0.2003

0.1577 -0.0933 -0.2640 -0.3402

0.1431 -0.1946 -0.1485  0.0026

[torch.FloatTensor of size 3x4],

3.1415  3.1415  3.1415  3.1415

3.1415  3.1415  3.1415  3.1415

3.1415  3.1415  3.1415  3.1415

[torch.FloatTensor of size 3x4])
### 4. 数学操作

##### 1） 算数操作 Arithmetic operations

```python
# torch.add()

x1 = torch.FloatTensor([[1,2,3],[4,5,6]])
x2 = torch.FloatTensor([[1,2,3],[4,5,6]])
add = torch.add(x1,x2)

x1,x2,add,x1+x2,x1-x2
```

out:

> 
(

1  2  3

4  5  6

[torch.FloatTensor of size 2x3],

1  2  3

4  5  6

[torch.FloatTensor of size 2x3],

2   4   6

8  10  12

[torch.FloatTensor of size 2x3],

2   4   6

8  10  12

[torch.FloatTensor of size 2x3],

0  0  0

0  0  0

[torch.FloatTensor of size 2x3])
支持广播机制

```python
# torch.add() broadcasting

x1 = torch.FloatTensor([[1,2,3],[4,5,6]])
x2 = torch.add(x1,10)

x1,x2,x1+10,x2-10
```

(

1  2  3

4  5  6

[torch.FloatTensor of size 2x3],

11  12  13

14  15  16

[torch.FloatTensor of size 2x3],

11  12  13

14  15  16

[torch.FloatTensor of size 2x3],

1  2  3

4  5  6

[torch.FloatTensor of size 2x3])
元素级乘法：

```python
# torch.mul() -> size better match

x1 = torch.FloatTensor([[1,2,3],[4,5,6]])
x2 = torch.FloatTensor([[1,2,3],[4,5,6]])
x3 = torch.mul(x1,x2)

x3
```

out:

> 
1   4   9

16  25  36

[torch.FloatTensor of size 2x3]
也支持广播：

```python
# torch.mul() -> broadcasting

x1 = torch.FloatTensor([[1,2,3],[4,5,6]])
x2 = x1*10

x2
```

out:

> 
10  20  30

40  50  60

[torch.FloatTensor of size 2x3]
元素级除法

```python
# torch.div() -> size better match

x1 = torch.FloatTensor([[1,2,3],[4,5,6]])
x2 = torch.FloatTensor([[1,2,3],[4,5,6]])
x3 = torch.div(x1,x2)

x3
```

out:

> 
1  1  1

1  1  1

[torch.FloatTensor of size 2x3]
```python
# torch.div() -> broadcasting

x1 = torch.FloatTensor([[1,2,3],[4,5,6]])

x1/5
```

out:

> 
0.2000  0.4000  0.6000

0.8000  1.0000  1.2000

[torch.FloatTensor of size 2x3]
#### 2)其他的数学操作

pow 幂操作

求每一个元素的平方

```python
# torch.pow(input,exponent)

x1 = torch.FloatTensor(3,4)
torch.pow(x1,2),x1**2
```

out:

> 
(

1.00000e-33 *

5.5267  0.0000  5.5267  0.0000

0.0000  0.0000  5.5267  0.0000

0.0000  0.0000  0.0000  0.0000

[torch.FloatTensor of size 3x4],

1.00000e-33 *

5.5267  0.0000  5.5267  0.0000

0.0000  0.0000  5.5267  0.0000

0.0000  0.0000  0.0000  0.0000

[torch.FloatTensor of size 3x4])
指数操作

```python
# torch.exp(tensor,out=None) 

x1 = torch.FloatTensor(3,4)
torch.exp(x1)
```

out:

> 
1   1   1   1

inf   1 inf   1

1   1   1   1

[torch.FloatTensor of size 3x4]
对数操作

```python
# torch.log(input, out=None) -> natural logarithm

x1 = torch.FloatTensor(3,4)
torch.log(x1)
```

out:

> 
nan -92.8880 -86.5151     -inf

-inf     -inf     -inf     -inf

-inf     -inf     -inf     -inf

[torch.FloatTensor of size 3x4]
#### 3) 矩阵操作

矩阵乘法

```python
# torch.mm(mat1, mat2) -> matrix multiplication

x1 = torch.FloatTensor(3,4)
x2 = torch.FloatTensor(4,5)

torch.mm(x1,x2)
```

out:

> 
-9.6088e-12  5.8988e-36  5.5267e-33  0.0000e+00 -9.6628e-12

2.4562e-18  6.9592e+27  2.4562e-18 -2.4818e+01  8.6738e-02

1.2129e-32  5.8988e-36  5.5267e-33  0.0000e+00 -9.6628e-12

[torch.FloatTensor of size 3x5]
批矩阵乘法

```python
# torch.bmm(batch1, batch2) -> batch matrix multiplication

x1 = torch.FloatTensor(10,3,4)
x2 = torch.FloatTensor(10,4,5)

torch.bmm(x1,x2).size()
```

out:

> 
torch.Size([10, 3, 5])


点乘：

```python
# torch.dot(tensor1,tensor2) -> dot product of two tensor

x1 = torch.FloatTensor(3,4)
x2 = torch.FloatTensor(3,4)

torch.dot(x1,x2)
```

out:

> 
5.5267435454184226e-33


转置

```python
# torch.t(matrix) -> transposed matrix

x1 = torch.FloatTensor(3,4)

x1,x1.t()
```

out:

> 
(

1.00000e-17 *

-7.4341  0.0000 -7.4341  0.0000

0.0000  0.0000  0.0000  0.0000

-7.4342  0.0000 -7.4342  0.0000

[torch.FloatTensor of size 3x4],

1.00000e-17 *

-7.4341  0.0000 -7.4342

0.0000  0.0000  0.0000

-7.4341  0.0000 -7.4342

0.0000  0.0000  0.0000

[torch.FloatTensor of size 4x3])
按两个指定的维度调换位置

```python
# torch.transpose(input,dim0,dim1) -> transposed matrix

x1 = torch.FloatTensor(10,3,4)

x1.size(), torch.transpose(x1,1,2).size(), x1.transpose(1,2).size()
```

out:

> 
(torch.Size([10, 3, 4]), torch.Size([10, 4, 3]), torch.Size([10, 4, 3]))











































































# tensorflow学习笔记（二）：tensor 变换 - Keith - CSDN博客





2016年10月20日 13:35:36[ke1th](https://me.csdn.net/u012436149)阅读数：41333
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









矩阵操作

```python
#对于2-D
#所有的reduce_...，如果不加axis的话，都是对整个矩阵进行运算
tf.reduce_sum(a, 1） #对axis1
tf.reduce_mean(a,0) #每列均值
```

第二个参数是axis，如果为0的话，$res[i] = \sum_{j} a[j,i]$即（$res[i] = \sum a[:,i]$）， 如果是1的话，$res[i] = \sum_{j} a[i,j]$

NOTE:返回的都是行向量,（axis等于几，就是对那维操作,i.e.:沿着那维操作, 其它维度保留）

```python
#关于concat，可以用来进行降维 3D->2D , 2D->1D
tf.concat(concat_dim, data)
#arr = np.zeros([2,3,4,5,6])
In [6]: arr2.shape
Out[6]: (2, 3, 4, 5)
In [7]: np.concatenate(arr2, 0).shape
Out[7]: (6, 4, 5)   :(2*3, 4, 5)
In [9]: np.concatenate(arr2, 1).shape
Out[9]: (3, 8, 5)   :(3, 2*4, 5)
#tf.concat()
t1 = [[1, 2, 3], [4, 5, 6]]
t2 = [[7, 8, 9], [10, 11, 12]]
# 将t1, t2进行concat，axis为0，等价于将shape=[2, 2, 3]的Tensor concat成
#shape=[4, 3]的tensor。在新生成的Tensor中tensor[:2,:]代表之前的t1
#tensor[2:,:]是之前的t2
tf.concat(0, [t1, t2]) ==> [[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]

# 将t1, t2进行concat，axis为1，等价于将shape=[2, 2, 3]的Tensor concat成
#shape=[2, 6]的tensor。在新生成的Tensor中tensor[:,:3]代表之前的t1
#tensor[:,3:]是之前的t2
tf.concat(1, [t1, t2]) ==> [[1, 2, 3, 7, 8, 9], [4, 5, 6, 10, 11, 12]]
```

concat是将list中的向量给连接起来，axis表示将那维的数据连接起来，而其他维的结构保持不变

```python
#squeeze 降维 维度为1的降掉
tf.squeeze(arr, [])
降维， 将维度为1 的降掉
arr = tf.Variable(tf.truncated_normal([3,4,1,6,1], stddev=0.1))
arr2 = tf.squeeze(arr, [2,4])
arr3 = tf.squeeze(arr) #降掉所以是1的维

#split
tf.split(split_dim, num_split, value, name='split')
# 'value' is a tensor with shape [5, 30]
# Split 'value' into 3 tensors along dimension 1
split0, split1, split2 = tf.split(1, 3, value)
tf.shape(split0) ==> [5, 10]

#embedding
mat = np.array([1,2,3,4,5,6,7,8,9]).reshape((3,-1))
ids = [[1,2], [0,1]]
res = tf.nn.embedding_lookup(mat, ids)
res.eval()
array([[[4, 5, 6],
        [7, 8, 9]],

       [[1, 2, 3],
        [4, 5, 6]]])

#扩展维度，如果想用广播特性的话，经常会用到这个函数
# 't' is a tensor of shape [2]
#一次扩展一维
shape(tf.expand_dims(t, 0)) ==> [1, 2]
shape(tf.expand_dims(t, 1)) ==> [2, 1]
shape(tf.expand_dims(t, -1)) ==> [2, 1]
# 't2' is a tensor of shape [2, 3, 5]
shape(tf.expand_dims(t2, 0)) ==> [1, 2, 3, 5]
shape(tf.expand_dims(t2, 2)) ==> [2, 3, 1, 5]
shape(tf.expand_dims(t2, 3)) ==> [2, 3, 5, 1]
```

## tf.slice()

`tf.slice(input_, begin, size, name=None)`

先看例子

```python
import tensorflow as tf
import numpy as np
sess = tf.InteractiveSession()
a = np.array([[1,2,3,4,5],[4,5,6,7,8],[9,10,11,12,13]])
tf.slice(a,[1,2],[-1,2]).eval()

#array([[ 6,  7],
#       [11, 12]])
```

理解tf.slice()最好是从返回值上去理解，现在假设input的shape是[a1, a2, a3], `begin`的值是[b1, b2, b3],`size`的值是[s1, s2, s3],那么`tf.slice()`返回的值就是 `input[b1:b1+s1, b2:b2+s2, b3:b3+s3]`。 

如果 $s_i=-1$ ，那么 返回值就是 `input[b1:b1+s1,..., bi: ,...]`

**注意：input[1:2] 取不到input[2]**

## tf.stack()

tf.stack(values, axis=0, name=’stack’)

将 a list of R 维的`Tensor`堆成 `R+1`维的`Tensor`。 

 Given a list of length N of tensors of shape (A, B, C); 

if axis == 0 then the output tensor will have the shape (N, A, B, C)
```python
这时 res[i,:,:,:] 就是原 list中的第 i 个 tensor
```

. if axis == 1 then the output tensor will have the shape (A, N, B, C).

```python
这时 res[:,i,:,:] 就是原list中的第 i 个 tensor
```

Etc.

```python
# 'x' is [1, 4]
# 'y' is [2, 5]
# 'z' is [3, 6]
stack([x, y, z]) => [[1, 4], [2, 5], [3, 6]]  # Pack along first dim.
stack([x, y, z], axis=1) => [[1, 2, 3], [4, 5, 6]]
```

## tf.gather()

`tf.gather(params, indices, validate_indices=None, name=None)`

`indices must be an integer tensor of any dimension (usually 0-D or 1-D). Produces an output tensor with shape indices.shape + params.shape[1:]`

```python
# Scalar indices, 会降维
output[:, ..., :] = params[indices, :, ... :]

# Vector indices
output[i, :, ..., :] = params[indices[i], :, ... :]

# Higher rank indices，会升维
output[i, ..., j, :, ... :] = params[indices[i, ..., j], :, ..., :]
```

## tf.pad

```python
tf.pad(tensor, paddings, mode="CONSTANT", name=None)
```
- `tensor`: 任意`shape`的`tensor`，维度 `Dn`
- `paddings`: `[Dn, 2]` 的 `Tensor`, `Padding`后`tensor`的某维上的长度变为`padding[D,0]+tensor.dim_size(D)+padding[D,1]`
- `mode`: `CONSTANT`表示填`0`, `REFLECT`表示反射填充,`SYMMETRIC`表示对称填充。

## tf.scatter_nd

```python
scatter_nd(
    indices,
    updates,
    shape,
    name=None
)

# 文档中废话那么多，可以总结成两个式子
# res[*indice[i,j,..,z], ...] = updates[i,j,..,z,...]
# len([i,j,..,z]) = indice.rank-2
# 在这里 rank 表示 几维， a=[1,2,3], a.rank=1,  b = [[1,2], [2,3], [3,4]], b.rank=2
```





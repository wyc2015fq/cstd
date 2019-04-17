# RPN 预备编程知识（一） :  Numpy VS Pytorch （ Anchor_target_layer ） - Snoopy_Dream - CSDN博客





2018年11月03日 12:12:59[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：129
所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)









**目录**

[1.  np.where(x>4)](#1.%20np.where(x%3E4))[)](#1.%20np.where(x%3E4))[知道返回[0] [1]](#1.%20np.where(x%3E4))

[2. np.meshgrid 构造xy轴](#2.np.meshgrid)

[3.  np.ravel( ) VS np.flatten() 拉成一维](#3.%20np.ravel(%20)%20VS%20np.flatten())

[4.  x.reshape( )  了解填充方式](#4.%20x.reshape(%20))

[5.  np.squeeze(a,axis=none) & b.squeeze(0) [ b是torch.data ]](#5.%20np.squeeze(a%2Caxis%3Dnone)%20%26%20b.squeeze(0)%20%5B%20b%E6%98%AFtorch.data%20%5D)

[6.  np.view & tensor.view 区别很大](#6.%20np.view%20%26%20tensor.view)

[7.  np.argmax(a,axis = none) & torch.max(a,1) 注意np中有关键字](#7.%20np.argmax(a%2Caxis%20%3D%20none)%20%26%20torch.max(a%2C1))

[8.  np.shape VS tensor.size() 一个属性 一个方法](#8.%20np.shape%20VS%20tensor.size())

[9.  vstack( ) take 1 position argument but 2 were given[an error] 需要的是一个元组](#9.%20Vstack(%20)%20take%201%20position%20argument%20but%202%20were%20given%5Ban%20error%5D)

[10. np.empty([2,2]) & np.empty(2) & np.empty(2,) VS tensor.new(2,3)](#10.%20np.empty(%5B2%2C2%5D)%20%26%20np.empty(2)%20%26%20np.empty(2%2C))

[11. np.fill & np.full 默认值 VS tensor.fill_(-1)](#11.%20np.fill%20%26%20np.full)

[12. np.linspace(2,3,5)从2到3有5个数](#12.%20np.linspace(2%2C3%2C5)%E4%BB%8E2%E5%88%B03%E6%9C%895%E4%B8%AA%E6%95%B0)

[13. nplogspace(2,3,4,base = 2)](#13.%20nplogspace(2%2C3%2C4%2Cbase%20%3D%202))

[14. np.full_like(a,fill_value) & np.empty_like(a) & np.zeros_like(a) 类似维度的数组](#14.%20np.full_like(a%2Cfill_value)%20%26%20np.empty_like(a)%20%26%20np.zeros_like(a))

[15.  np.ascontiguousarray VS tensor.contiguous() → Tensor  地址连续的需要](#15.%C2%A0%20%20np.ascontiguousarray%20VS%20tensor.contiguous()%20%E2%86%92%20Tensor)

[16. 广播（重要）](#16.%20%E5%B9%BF%E6%92%AD%EF%BC%88%E9%87%8D%E8%A6%81%EF%BC%89)

17. torch.stack((a,b,c),dim)

18. expand_as(*tensor*)

19. torch.equal(a,b) & a.eq(b)

20. dx = deltas[:, 0::4] #！4表示步长，每隔4个选一个（proposal layer）

21.  pred_boxes = np.zeros(deltas.shape, dtype=deltas.dtype)  VS

       pred_boxes = deltas.clone() 返回一个新的与某一个输出同样类型大小的变量。（proposal layer）

由于我自己之前编程很少利用np，这个算一个补习吧。如果想要尝试理解一个np函数什么时候用，可以看anchor_target_layer解析里面的应用。还有就是  numpy&pytorch 总是有时候晕，你比方  np.squeeze(a,axis=none) & a.squeeze() [ a是torch.data ],首先np中没有unsqueeze,pytorch有；  再有就是np.view()和torch中的x.view(x.size[0],-1);还有torch中直接x.size()类似与x.shape（注意后者没有()），但是np没有np.size()。



---------------------  

### 1. np.where(x>4)

返回一个行向量，一个列向量。

所以大多后面加一个[0] np.where(x>4)[0]

---------------------  

### 2.np.meshgrid

类似于**坐标轴**一样的感觉,所以rpn生成anchor平移的时候，就可以用到坐标轴，也就是np.meshgrid。

z,s=np.meshgrid(x,y)  z就是x轴，**z的每一列就是x轴=-2,1,0,1**。**s的每一行y轴=0,1,2**

```python
x=np.array([-2,1,0,1])  4
y=np.array([0,1,2])   3
z,s=np.meshgrid(x,y)   3*4
              列数，行数
>>> z=array([[-2,  1,  0,  1],
             [-2,  1,  0,  1],
             [-2,  1,  0,  1]])
>>> s= array([[0, 0, 0, 0],
              [1, 1, 1, 1],
              [2, 2, 2, 2]])
```

[`meshgrid`](https://docs.scipy.org/doc/numpy/reference/generated/numpy.meshgrid.html#numpy.meshgrid) is very useful to evaluate functions on a grid.下面是caffe中的Anchor_target_layer的shift实现过程。

```python
#算出box的偏移量
# 1. 构造原图上的一个grid，看成xy轴
shift_x = np.arange(0, width) * self._feat_stride          
shift_y = np.arange(0, height) * self._feat_stride 
# shift_x 变成 height行np.arange(0, width)
# shift_y 变成 width列np.arange(0, height)两者用ravel展开的时候就是在图片上一行一行读取。
shift_x, shift_y = np.meshgrid(shift_x, shift_y) 
# 用来算偏移量, 原始的anchor只要+shifts就是了，一行一行读取。
shifts = np.vstack((shift_x.ravel(), shift_y.ravel(),  
                    shift_x.ravel(), shift_y.ravel())).transpose()
```

**shifts.ndim=(H*W,4),注意我写的是H*W,而不是W*H。因为它是逐行添加的，具体可看下面x对应w，y对应h，填充y=0第一行，所有列，然后填充第二行。**

---------------------  

### 3. np.ravel( ) VS np.flatten() 变成1维处理节省空间

相同点都是返回一维度。

np.ravel( )  返回view(视图)，共享内存。之前的变了，他也变。更节省！

np.flatten() 返回copy，开辟内存。

--------------------- 

### 4. x.reshape( )

注意：返回的是view，没有copy。

```python
c = a.reshape(1,3)
>>> a = array([[1],[2],[3]])
>>> a[2] = 4
>>> a
array([[1],[2],[4]])
>>> c
array([[1, 2, 4]])
```

首先看几个常用的 ：

    x.reshape( -1) 变成维度为1的数组。ndim = (n,)

    x.reshape( 1,-1) _>ndim = (1,n) ，-1 可以用到增加维度上。也可以将ndim = (n,)的数组，变成ndim = (1,n) or(n,1)的向量时。

然后通过下面的3维的数组看一下，reshape填充的顺序，从最后一个维度开始赋值。

```python
c = np.arange(8).reshape(2,2,2)
>>> c[0,:,:]
>>> array([[0, 1],
          [2, 3]])
>>> c[0,0,0]= 0  c[0,0,1]= 1  c[0,1,0]= 2  c[0,1,1]= 3
```

### --------------------- 

### 5. np.squeeze(a,axis=none) & b.squeeze(0) [ b是torch.data ]

首先np中没有unsqueeze。 squeeze的作用，是把axis轴的维度压缩，去掉，前提是axis的维度为 1，如果不是就报错。

```python
a=array([[[1],[2],[3]]])
a.shape
>>>(1,3,1)
np.squeeze(a,axis=0)
>>>array([[1],[2],[3]])
np.squeeze(a,axis=1)
>>>报错
np.squeeze(a,axis=2)
array([[1,2,3]])
```

但是在pytorch中，有unsqueeze，相当于增加维度。squeeze相当于压缩维度。

tensor.squeeze(0)把第0个轴压缩，如果维度是1则压缩，不是不压缩，也不报错！

--------------------- 

### 6. np.view & tensor.view

np.view是一个视图，但是在torch中你经常看到[x = x.*view*(x.size(0), -1)](https://blog.csdn.net/whut_ldz/article/details/78882532)作用是改变维度[n,c,h,w]_>[n,c*h*w]

当需要对输入图像**三个通道进行相同的处理时**，**使用cv2.split和cv2.merge是相当浪费资源的**，因为任何一个通道的数据对处理来说都是一样的，我们可以**用view来将其转换为一维矩阵后再做处理，这要不需要额外的内存开销和时间开销。**

代码如下：

x.view(x.size(0), -1)

--------------------- 

### 7. np.argmax(a,axis= none) & torch.max(a,1)

np.argmax返回的索引 如果**axis=1，二维表示在每一行里面找列最大的**,只返回索引。**高维度的可以这样理解：在axis轴对应的维数里寻找最大。你比如：a.shape=(2,3,4), np.argmax(a,axis=2),按4个数4个数的找最大，返回(2,3)。**

torch.max(a,1)[0]  返回最大值

torch.max(a,1)[1]   返回索引

---------------------

### 8. np.shape VS tensor.size()

首先torch中也有tensor.shape类似于tensor.size()，但是shape是一个属性，而size是一个方法，所以size有()需要参数。

np中没有size()的方法！

---------------------

### 9. Vstack( ) take 1 position argument but 2 were given[an error]

例：np.row_stack(a,b)

改为：np.row_stack(**（**a,b**）**) -----别忘了括号，里面需要的是一个元组。

---------------------

### 10. np.empty([2,2]) & np.empty(2) & np.empty(2,) VS tensor.new(2,3)

首先后两个是一样的，其次np.empty([2,2]) 出来每次都一样。然后出来的是无限接近于0的数。

tensor.new()随机建立和tensor一种类型的数据

---------------------

### 11. np.fill & np.full VS tensor.new().fill_()

创造一列的，有默认值的那种，np.fill & np.full这两个没分清，感觉差不多，之后再说吧。

```
# label: 1 is positive, 0 is negative, -1 is dont care 

labels = np.empty((len(inds_inside), ), dtype=np.float32)#默认float64 
labels.fill(-1)
```



```python
#没有找到new函数，创建和gt_boxes同一类型的new
        labels = gt_boxes.new(batch_size, inds_inside.size(0)).fill_(-1)
```

---------------------

### 12. np.linspace(2,3,5)从2到3有5个数

---------------------

### 13. nplogspace(2,3,4,base = 2)

默认，base = 10 ，4 ...8从2**2到2***3有四个数

---------------------

### 14. np.full_like(a,*fill_value*) & np.empty_like(a) & np.zeros_like(a)

创造一个类似于a的但填充了其他value的array

---------------------

### 15.  np.ascontiguousarray VS tensor.contiguous() → Tensor

主要用在vstack **transpose **这种，还有 anchors = all_anchors[inds_inside:]经过索引返回部分结果。打乱了数据内存的情况。而之后要用到torch.view()转换维度 这种需要数据内存连续，才不报错，或者不copy节省空间



判断是否contiguous用torch.Tensor.is_contiguous()函数。

```python
import torch
x = torch.ones(10, 10)
x.is_contiguous()  # True
x.transpose(0, 1).is_contiguous()  # False
x.transpose(0, 1).contiguous().is_contiguous()  # True
```

另外，在pytorch的最新版本0.4版本中，增加了**torch.reshape()**, 这与 numpy.reshape 的功能类似。它大致相当于 **tensor.contiguous().view()**

```python
#bbox_overlaps里有vastck
overlaps = bbox_overlaps( #返回大小连续的overlaps，等同于排序
            np.ascontiguousarray(anchors, dtype=np.float),  
            np.ascontiguousarray(gt_boxes, dtype=np.float))
```

```
#返回一个内存连续的有相同数据的tensor，如果原tensor内存连续则返回原tensor
#shifts.view need 内存连续
#type_as(tesnor)将tensor投射为参数给定tensor类型并返回。 如果tensor已经是正确的类型则不会执行操作。等效于：self.type(tensor.type())
#shifts int_>float

shifts = shifts.contiguous().type_as(rpn_cls_score).float()
A = self._num_anchors
K = shifts.size(0)
#int_tensor_>float_tensor
self._anchors = self._anchors.type_as(gt_boxes) # move to specific gpu.
#board
all_anchors = self._anchors.view(1, A, 4) + shifts.view(K, 1, 4)
#k,a,4_>k*a,4
all_anchors = all_anchors.view(K * A, 4)
```

---------------------

### 16. 广播（重要）

广播的原则：如果两个数组的后缘维度（trailing dimension，即从末尾开始算起的维度）的轴长度相符，或其中的一方的长度为1，则认为它们是广播兼容的。广播会在缺失和（或）长度为1的维度上进行。(3,2)(2,)两者的后缘维度是相同的。

更加通俗的说法：

**规则1: **如果两个数组在维度的数量上有差异，那么维度较少的数组的形状就会被用1填充在它的前导(左)边。
**规则2:** 如果两个数组的形状在任何维度上都不匹配，但等于1，那么在这个维度中，形状为1的数组将被拉伸以匹配另一个形状。**规则3: **如果在任何维度上，大小都不一致，且两者都不等于1，就会出现错误。

下面看例子：

In[1]: import numpy as np

In[2]: M = np.ones((2, 3))

       a = np.arange(3)

In [3]: M.shape

Out[3]: (2, 3)

In [4]: a.shape

Out[4]: (3,)



根据规则1，数组a的维数更少，所以我们用1填充在它的左边。

M.shape -> (2, 3)

a.shape -> (1, 3)

根据规则2，我们现在看到第一个维度不匹配，所以我们扩展这个维度来匹配。

M.shape -> (2, 3)

a.shape -> (2, 3)

现在就可以相加了，而且最终的shape是(2, 3)。我们验证一下：

In [5]: (M + a).shape

Out[5]: (2, 3)



下面看一个满足规则3的例子：

In [15]： M = np.ones((3, 2))

In [16]： a = np.arange(3)

In [16]: M.shape

Out[16]: (3, 2)

In [17]: a.shape

Out[17]: (3,)

根据规则1，

M.shape -> (3, 2)

a.shape -> (1, 3)

根据规则2,a的第一个维度被拉伸到与M匹配:

M.shape -> (3, 2)

a.shape -> (3, 3)

现在根据规则3，如果我们进行加的操作，会出现错误，验证下：

In [18]: M + a

Traceback (most recent call last):

  File "<ipython-input-18-8cac1d547906>", line 1, in <module>

    M + a

ValueError: operands could not be broadcast together with shapes (3,2) (3,)

注意这里可能出现的混淆:你可以想象出a和M的兼容性，比方说，用1在右边而不是左边填充a。但这不是广播规则的工作方式!还要注意的是，虽然我们一直在关注+运算符，但是这些广播规则适用于任何二元ufunc。

---------------------

### 17. torch.stack((a,b,c),dim)

沿着一个新维度对输入张量序列进行连接。 序列中所有的张量都应该为相同形状。

注意这个函数和concatenate是不同的，torch的concatenate函数是torch.cat，是在已有的维度上拼接，而stack是建立一个新的维度，然后再在该纬度上进行拼接。

参数:
- sqequence (Sequence) – 待连接的张量序列
- dim (int) – 插入的维度。必须介于 0 与 待连接的张量序列数之间。要看合成后的

类似的维度的操作，可以看我上面reshape的说明，必须要理解填充的顺序。从后缘维度开始填充。

你比如：a=[[1,2,3],[4,5,9],[7,8,9]] b=[[1,2,3],[4,5,9],[7,8,9]] ,c=-[[1,2,3],[4,5,9],[7,8,9]] ，

torch.stack((a,b,c),0)0是最后一个填充的，从最大层次也就是最外面的括号看

>>>【[[1,2,3],[4,5,9],[7,8,9]],[[1,2,3],[4,5,9],[7,8,9]],-[[1,2,3],[4,5,9],[7,8,9]] 】

torch.stack((a,b,c),1)从中间的括号开始看

>>> [[[1,2,3],[1,2,3],-[1,2,3]],[4,5,9],[4,5,9],-[4,5,9]...]

torch.stack((a,b,c),2)从最里面的一个元素一个元素竖着开始看

>>> [[[1,1,-1],[2,2,-2],[3,3,-3]],[....]

在比如一维度数组stack

a=([1,2,3]) b=([1,2,3])

torch.stack((a,b),0)

>>>[[1,2,3],[1,2,3]]

torch.stack((a,b),1)

>>>[[1,1][2,2],[3,3]]

---------------------

### 18. expand_as(*tensor*)

torch版本的bbox_transform_batch有用。将tensor扩展为参数tensor的大小。类似于**人为广播操作。但必须只有一个维度是不相等的。**

```python
>>> a=torch.rand(2,1,4)
>>> b=torch.rand(2,4,4)
>>> a
tensor([[[ 0.1828,  0.9803,  0.4630,  0.7553]],

        [[ 0.3949,  0.8661,  0.8884,  0.3982]]])
>>> b
tensor([[[ 0.5678,  0.7193,  0.6781,  0.9207],
         [ 0.4690,  0.4737,  0.2638,  0.9247],
         [ 0.9315,  0.8202,  0.5217,  0.8444],
         [ 0.5279,  0.5673,  0.6443,  0.4649]],

        [[ 0.3587,  0.8476,  0.6263,  0.1106],
         [ 0.4531,  0.0974,  0.9504,  0.6684],
         [ 0.5585,  0.9721,  0.1965,  0.3138],
         [ 0.6662,  0.2503,  0.9760,  0.0737]]])
>>> a.expand_as(b)
tensor([[[ 0.1828,  0.9803,  0.4630,  0.7553],
         [ 0.1828,  0.9803,  0.4630,  0.7553],
         [ 0.1828,  0.9803,  0.4630,  0.7553],
         [ 0.1828,  0.9803,  0.4630,  0.7553]],

        [[ 0.3949,  0.8661,  0.8884,  0.3982],
         [ 0.3949,  0.8661,  0.8884,  0.3982],
         [ 0.3949,  0.8661,  0.8884,  0.3982],
         [ 0.3949,  0.8661,  0.8884,  0.3982]]])
```

### 19.torch.equal(a,b) & a.eq(b)

**a.eq(b)**

```
torch.eq(input, other, out=None) → Tensor
```

比较元素相等性。第二个参数可为一个数或与第一个参数同类型形状的张量。

参数：
- input (Tensor) – 待比较张量
- other (Tensor or float) – 比较张量或数
- out (Tensor, optional) – 输出张量，须为 ByteTensor类型 or 与`input`同类型

返回值： 一个 `torch.ByteTensor` 张量，包含了每个位置的比较结果(相等为1，不等为0 )

返回类型： Tensor

例子：

```
>>> torch.eq(torch.Tensor([[1, 2], [3, 4]]), torch.Tensor([[1, 1], [4, 4]]))
1  0
0  1
[torch.ByteTensor of size 2x2]
```

**torch.equal**

```
torch.equal(tensor1, tensor2) → bool
```

如果两个张量有相同的形状和元素值，则返回`True` ，否则 `False`。

例子：

```
>>> torch.equal(torch.Tensor([1, 2]), torch.Tensor([1, 2]))
True
```




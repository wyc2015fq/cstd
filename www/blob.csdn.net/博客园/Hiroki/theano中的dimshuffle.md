# theano中的dimshuffle - Hiroki - 博客园




## [theano中的dimshuffle](https://www.cnblogs.com/wacc/p/5342479.html)


theano中的dimshuffle函数用于对张量的维度进行操作，可以增加维度，也可以交换维度，删除维度。

注意的是只有shared才能调用dimshuffle()

'x'表示增加一维，从0d scalar到1d vector

(0, 1)表示一个与原先相同的2D向量

(1, 0)表示将2D向量的两维交换

(‘x’, 0) 表示将一个1d vector变为一个1xN矩阵

(0, ‘x’)将一个1d vector变为一个Nx1矩阵

(2, 0, 1) -> AxBxC to CxAxB （2表示第三维也就是C，0表示第一维A，1表示第二维B）

(0, ‘x’, 1) -> AxB to Ax1xB 表示A，B顺序不变在中间增加一维

(1, ‘x’, 0) -> AxB to Bx1xA 同理自己理解一下

(1,) -> 删除维度0，(1xA to A)

写了个小程序来验证猜想

```
from __future__ import print_function
import theano
import numpy as np
def print_hline(file):
    print('------------------------------------------',file=file,end='\r\n')
write_file=open('G:\data\dimshuffle_output.txt','wb')
v = theano.shared(np.arange(3))
# v.shape is a symbol expression, need theano.function or eval to compile it
print_hline(write_file)
v_disp = v.dimshuffle(0)
print('v.dimshuffle(0):',v_disp.eval(),file=write_file,end='\r\n')
print('v.dimshuffle(0).shape:',v_disp.shape.eval(),file=write_file,end='\r\n')
print_hline(write_file)
v_disp = v.dimshuffle('x', 0)
print("v.dimshuffle('x',0):",v_disp.eval(),file=write_file,end='\r\n')
print("v.dimshuffle('x',0).shape:",v_disp.shape.eval(),file=write_file,end='\r\n')
print_hline(write_file)
v_disp = v.dimshuffle(0,'x')
print("v.dimshuffle(0,'x'):",v_disp.eval(),file=write_file,end='\r\n')
print("v.dimshuffle(0,'x').shape:",v_disp.shape.eval(),file=write_file,end='\r\n')
print_hline(write_file)
v_disp = v.dimshuffle(0,'x','x')
print("v.dimshuffle(0,'x','x'):",v_disp.eval(),file=write_file,end='\r\n')
print("v.dimshuffle(0,'x','x').shape:",v_disp.shape.eval(),file=write_file,end='\r\n')
print_hline(write_file)
v_disp = v.dimshuffle('x',0,'x')
print("v.dimshuffle('x',0,'x'):",v_disp.eval(),file=write_file,end='\r\n')
print("v.dimshuffle('x',0,'x').shape:",v_disp.shape.eval(),file=write_file,end='\r\n')
print_hline(write_file)
v_disp = v.dimshuffle('x','x',0)
print("v.dimshuffle('x','x',0):",v_disp.eval(),file=write_file,end='\r\n')
print("v.dimshuffle('x','x',0).shape:",v_disp.shape.eval(),file=write_file,end='\r\n')
print_hline(write_file)
m = theano.shared(np.arange(6).reshape(2,3))
print("m:",m.eval(),file=write_file,end='\r\n')
print("m.shape:",m.shape.eval(),file=write_file,end='\r\n')
print_hline(write_file)
m_disp = m.dimshuffle(0,'x',1)
print("m.dimshuffle(0,'x',1):",m_disp.eval(),file=write_file,end='\r\n')
print("m.dimshuffle(0,'x',1).shape:",m_disp.shape.eval(),file=write_file,end='\r\n')
print_hline(write_file)
m_disp = m.dimshuffle('x',0,1)
print("m.dimshuffle('x',0,1):",m_disp.eval(),file=write_file,end='\r\n')
print("m.dimshuffle('x',0,1).shape:",m_disp.shape.eval(),file=write_file,end='\r\n')
print_hline(write_file)
m_disp = m.dimshuffle(0,1,'x')
print("m.dimshuffle(0,1,'x'):",m_disp.eval(),file=write_file,end='\r\n')
print("m.dimshuffle(0,1,'x').shape:",m_disp.shape.eval(),file=write_file,end='\r\n')
print_hline(write_file)
# amount to transpose
m_disp = m.dimshuffle(1,'x',0)
print("m.dimshuffle(1,'x',0):",m_disp.eval(),file=write_file,end='\r\n')
print("m.dimshuffle(1,'x',0).shape:",m_disp.shape.eval(),file=write_file,end='\r\n')
write_file.close()
```

首先定义了一个[0 1 2]的1D vector：v，v.dimshuffle(0)中的0表示第一维：3，也只有一维，所以不变。因为是1D的，所以shape只有（3，）

```
v.dimshuffle(0): [0 1 2]
v.dimshuffle(0).shape: [3]
```

v.dimshuffle('x',0)表示在第一维前加入一维，只要记住加了'x'就加了一维，所以大小变成了1x3

```
v.dimshuffle('x',0): [[0 1 2]]
v.dimshuffle('x',0).shape: [1 3]
```

剩下的同理可理解

```
v.dimshuffle(0,'x'): [[0]
 [1]
 [2]]
v.dimshuffle(0,'x').shape: [3 1]
```

```
v.dimshuffle(0,'x','x'): [[[0]]

 [[1]]

 [[2]]]
v.dimshuffle(0,'x','x').shape: [3 1 1]
```

```
v.dimshuffle('x',0,'x'): [[[0]
  [1]
  [2]]]
v.dimshuffle('x',0,'x').shape: [1 3 1]
```

```
v.dimshuffle('x','x',0): [[[0 1 2]]]
v.dimshuffle('x','x',0).shape: [1 1 3]
```

第二个例子，m是一个2x3矩阵

```
m: [[0 1 2]
 [3 4 5]]
m.shape: [2 3]
```

先确定0,'x',1的维数,0对应第一维（2），1表示第二维（3），'x'表示新加入的维度（1）

所以结果维度是2x1x3

加括号的顺序按照从左到右（外->内)的顺序

1.先加最内层3，3表示括号内有3个数，因此是[0 1 2]和[3 4 5]

2.再加中间层1，1表示括号内只有一个匹配的"[]"，因此是[[0 1 2]],[[3 4 5]]

3.最后加最外层2，2表示括号内有两个匹配的"[]"（只算最外层的匹配），于是最后结果是

[[[0 1 2]]

[[3 4 5]]]

```
m.dimshuffle(0,'x',1): [[[0 1 2]]

 [[3 4 5]]]
m.dimshuffle(0,'x',1).shape: [2 1 3]
```

剩下的同理可以理解

```
m.dimshuffle('x',0,1): [[[0 1 2]
  [3 4 5]]]
m.dimshuffle('x',0,1).shape: [1 2 3]
```

```
m.dimshuffle(0,1,'x'): [[[0]
  [1]
  [2]]

 [[3]
  [4]
  [5]]]
m.dimshuffle(0,1,'x').shape: [2 3 1]
```

```
m.dimshuffle(1,'x',0): [[[0 3]]

 [[1 4]]

 [[2 5]]]
m.dimshuffle(1,'x',0).shape: [3 1 2]
```









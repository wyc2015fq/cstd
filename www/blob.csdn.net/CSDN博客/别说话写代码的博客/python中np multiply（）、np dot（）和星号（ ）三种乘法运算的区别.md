# python中np.multiply（）、np.dot（）和星号（*）三种乘法运算的区别 - 别说话写代码的博客 - CSDN博客





2018年07月09日 14:39:23[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：52标签：[python中numpy矩阵乘法																[np.multiply与np.dot区别																[np.dot与*区别																[np.multiply与*区别																[python中3种乘法运算](https://so.csdn.net/so/search/s.do?q=python中3种乘法运算&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)





本文转自：https://blog.csdn.net/zenghaitao0128/article/details/78715140

为了区分三种乘法运算的规则，具体分析如下：

```python
import numpy as np
```
- 1

### 1. np.multiply()函数

函数作用

> 
数组和矩阵对应位置相乘，输出与相乘数组/矩阵的大小一致


#### 1.1数组场景

```python
A = np.arange(1,5).reshape(2,2)
A
```
- 1
- 2

```
array([[1, 2],
       [3, 4]])
```
- 1
- 2
- 3

```python
B = np.arange(0,4).reshape(2,2)
B
```
- 1
- 2

```
array([[0, 1],
       [2, 3]])
```
- 1
- 2
- 3

```python
np.multiply(A,B)       #数组对应元素位置相乘
```
- 1

```
array([[ 0,  2],
       [ 6, 12]])
```
- 1
- 2
- 3

#### 1.2 矩阵场景

```python
np.multiply(np.mat(A),np.mat(B))     #矩阵对应元素位置相乘，利用np.mat()将数组转换为矩阵
```
- 1

```
matrix([[ 0,  2],
        [ 6, 12]])
```
- 1
- 2
- 3

```python
np.sum(np.multiply(np.mat(A),np.mat(B)))    #输出为标量
```
- 1

```
20
```
- 1
- 2

### 2. np.dot()函数

函数作用

> 
对于秩为1的数组，执行对应位置相乘，然后再相加；

对于秩不为1的二维数组，执行矩阵乘法运算；超过二维的可以参考numpy库介绍。


### 2.1 数组场景

### 2.1.1 数组秩不为1的场景

```python
A = np.arange(1,5).reshape(2,2)
A
```
- 1
- 2

```
array([[1, 2],
       [3, 4]])
```
- 1
- 2
- 3

```python
B = np.arange(0,4).reshape(2,2)
B
```
- 1
- 2

```
array([[0, 1],
       [2, 3]])
```
- 1
- 2
- 3

```python
np.dot(A,B)    #对数组执行矩阵相乘运算
```
- 1

```
array([[ 4,  7],
       [ 8, 15]])
```
- 1
- 2
- 3

### 2.1.2 数组秩为1的场景

```python
C = np.arange(1,4)
C
```
- 1
- 2

```
array([1, 2, 3])
```
- 1
- 2

```python
D = np.arange(0,3)
D
```
- 1
- 2

```
array([0, 1, 2])
```
- 1
- 2

```python
np.dot(C,D)   #对应位置相乘，再求和
```
- 1

```
8
```
- 1
- 2

### 2.2 矩阵场景

```python
np.dot(np.mat(A),np.mat(B))   #执行矩阵乘法运算
```
- 1

```
matrix([[ 4,  7],
        [ 8, 15]])
```
- 1
- 2
- 3

### 3. 星号（*）乘法运算

作用

> 
对数组执行对应位置相乘

对矩阵执行矩阵乘法运算


#### 3.1 数组场景

```python
A = np.arange(1,5).reshape(2,2)
A
```
- 1
- 2

```
array([[1, 2],
       [3, 4]])
```
- 1
- 2
- 3

```python
B = np.arange(0,4).reshape(2,2)
B
```
- 1
- 2

```
array([[0, 1],
       [2, 3]])
```
- 1
- 2
- 3

```python
A*B  #对应位置点乘
```
- 1

```
array([[ 0,  2],
       [ 6, 12]])
```
- 1
- 2
- 3

### 3.2矩阵场景

```python
(np.mat(A))*(np.mat(B))  #执行矩阵运算
```
- 1

```
matrix([[ 4,  7],
        [ 8, 15]])
```](https://so.csdn.net/so/search/s.do?q=np.multiply与*区别&t=blog)](https://so.csdn.net/so/search/s.do?q=np.dot与*区别&t=blog)](https://so.csdn.net/so/search/s.do?q=np.multiply与np.dot区别&t=blog)](https://so.csdn.net/so/search/s.do?q=python中numpy矩阵乘法&t=blog)





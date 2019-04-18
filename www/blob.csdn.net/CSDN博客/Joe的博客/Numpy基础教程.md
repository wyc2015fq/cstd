# Numpy基础教程 - Joe的博客 - CSDN博客





2018年04月02日 23:57:42[Joe-Han](https://me.csdn.net/u010089444)阅读数：1116








引入numpy 模块：
`import numpy as np`
# 1. 创建array

#### 1.1 列表转换为数组，查看属性：

```
array = np.array([[1,2,3],[4,5,6]], dtype = np.float)

print(array)
print(array.dtype)
print('number of dim:',array.ndim) 
print('shape:',array.shape)
print('size:',array.size)
```

> 
输出： 

  [[ 1.  2.  3.] 

  [ 4.  5.  6.]] 

  float64 

  (‘number of dim:’, 2) 

  (‘shape:’, (2L, 3L)) 

  (‘size:’, 6)
#### 1.2 创建全为0的数组：

```
a = np.zeros(shape=(2,2))
print(a)
```

> 
输出： 

  [[ 0.  0.] 

  [ 0.  0.]]
#### 1.3 创建全为1的数组：

```
a = np.ones(shape=(3,3),dtype = np.int32)
print(a)
```

> 
输出： 

  [[1 1 1] 

   [1 1 1] 

   [1 1 1]]
#### 1.4 创建单位矩阵：

```
a = np.eye(2,2)
print(a)
```

> 
输出： 

  [[ 1.  0.] 

   [ 0.  1.]]
#### 1.5 创建有序数组：

```
a = np.arange(10, 20, 2)
print(a)
```

> 
输出 

  [10 12 14 16 18]


#### 1.6 创建等差数组

```
a = np.linspace(0, 10, 5)
print(a)
```

> 
输出 

  [  0.    2.5   5.    7.5  10. ]


# 2. array基本操作

#### 2.1 改变数组大小

```
a = np.arange(12).reshape((3,4))
print(a)
```

> 
输出 

  [[ 0  1  2  3] 

   [ 4  5  6  7] 

   [ 8  9 10 11]]
#### 2.2 插入新维度

```php
a = np.array([1,1,1])
print a[:, np.newaxis]
```

> 
输出 

  [[1] 

   [1] 

   [1]]
#### 2.3 判断元素值

```
print a > 25
print a == b
print a == 10
```

> 
输出 

  [False False  True  True] 

  [False False False False] 

  [ True False False False]
#### 2.4 排序

```
a = np.array([[4, 3, 2], [2, 1, 4]])
print np.sort(a, axis=1)
```

> 
输出 

  [[2 3 4] 

   [1 2 4]]
```
dtype = [('Name', 'S10'), ('Height', float), ('Age', int)]
values = [('Li', 1.8, 41), ('Wang', 1.9, 38), ('Duan', 1.7, 38)]
a = np.array(values, dtype=dtype)
print np.sort(a, order=['Age', 'Height'])
```

> 
输出 

  [(‘Duan’, 1.7, 38) (‘Wang’, 1.9, 38) (‘Li’, 1.8, 41)]


#### 2.5 修剪array

小于min或大于max的元素直接置为min或max

```
c = np.array([[1.0, 2.0], [3.0, 4.0]])
print np.clip(c, 1, 2)
```

> 
输出 

  [[ 1.  2.] 

   [ 2.  2.]]
#### 2.6 多维转一维

```
c = np.array(np.arange(12)).reshape((3,4))
print c.flatten()
```

> 
输出 

  [ 0  1  2  3  4  5  6  7  8  9 10 11]


#### 2.7 索引与切片

```php
a = np.array([10, 20, 30, 40])
b = np.array(np.arange(12)).reshape((3,4))

print a[a >20]
print b[:2, :]
print b[:, 2:]
```

> 
输出 

  [30 40] 

  [[0 1 2 3] 

   [4 5 6 7]] 

  [[ 2  3] 

   [ 6  7] 

   [10 11]]
# 3. 基本运算

首先创建两个数组：

```
a = np.array([10, 20, 30, 40])
b = np.arange(1, 5)
```

#### 3.1 加法操作
`print a + b`
> 
输出 

  [11 22 33 44]


#### 3.2 除法操作
`print a / b`
> 
输出 

  [10 10 10 10]


#### 3.3 幂运算
`print b**3`
> 
输出 

  [ 1  8 27 64]


#### 3.4 三角函数运算

```
print np.sin(a)
print np.tan(b)
```

> 
输出 

  [-0.54402111  0.91294525 -0.98803162  0.74511316] 

  [ 1.55740772 -2.18503986 -0.14254654  1.15782128]
#### 3.5 array类矩阵运算

转置

```
c = np.arange(9).reshape((3,3)) 
print np.transpose(c) # a.T, c.transpose()
```

> 
输出 

  [[0 3 6] 

   [1 4 7] 

   [2 5 8]]
矩阵相乘
`print np.dot(a,np.transpose(b))  # a.dot(b.T)`
> 
输出 

  300


矩阵逆

```
c = np.array([[1.0, 2.0], [3.0, 4.0]])
print np.linalg.inv(c)
```

> 
输出 

  [[-2.   1. ] 

   [ 1.5 -0.5]]
#### 3.6 array最值、求和、均值

```
c = np.array([[1.0, 2.0], [3.0, 4.0]])

print c.min(axis=0)
print c.max(axis=1)
print c.sum()
print c.mean()
```

> 
输出 

  [ 1.  2.] 

  [ 2.  4.] 

  10.0 

  2.5
#### 3.7 array最值索引

```
print np.argmax(a)
print np.argmin(a)
```

> 
输出 

  3 

  0
#### 3.8 累加与累差
``print np.cumsum(a)
print np.diff(a)
> 

输出 

  [ 10  30  60 100] 

  [10 10 10]3.9 查找非零元素``c = np.array([[0.0, 2.0], [3.0, 4.0]])
print np.nonzero(c)
> 


输出 

  (array([0, 1, 1], dtype=int64), array([1, 0, 1], dtype=int64))4. array分割与合并4.1 数组分割``a = np.arange(12).reshape((3, 4))
print np.split(a, 2, axis=1)
> 


输出 

  [array([[0, 1], 

         [4, 5], 

         [8, 9]]), array([[ 2,  3], 

         [ 6,  7], 

         [10, 11]])]4.2 纵向分割``a = np.arange(12).reshape((3, 4))
print np.vsplit(a, 3)
> 


输出 

  [array([[0, 1, 2, 3]]), array([[4, 5, 6, 7]]), array([[ 8,  9, 10, 11]])]4.3 横向分割``a = np.arange(12).reshape((3, 4))
print np.hsplit(a, 2)
> 


输出 

  [array([[0, 1], 

         [4, 5], 

         [8, 9]]), array([[ 2,  3], 

         [ 6,  7], 

         [10, 11]])]4.4 数组合并``a = np.array([1,2,3])
b = np.array([4,5,6])

print np.concatenate((a, b))
print np.concatenate((a[:,np.newaxis], b[:,np.newaxis]), axis=1)
> 


输出 

  [1 2 3 4 5 6] 

  [[1 4] 

   [2 5] 

   [3 6]]4.5 纵向合并
```php

```
a = np.array([1, 1, 1])
b = np.array([2, 2, 2])
print np.vstack((a,b))
> 


输出 

  [[1 1 1] 

   [2 2 2]]4.6 横向合并
```php

```
a = np.array([1, 1, 1])
b = np.array([2, 2, 2])
print np.hstack((a,b))
> 


输出 

  [1 1 1 2 2 2]5. random模块

numpy.random模块分为四个部分：- 
简单随机数： 产生简单的随机数据，可以是任何维度 - 
排列：将所给对象随机排列 - 
分布：产生指定分布的数据，如高斯分布等 - 
生成器：种随机数种子，根据同一种子产生的随机数是相同的 5.1 简单随机数函数名函数功能参数说明rand(d0, d1, …, dn)产生均匀分布的随机数dn为第n维数据的维度randn(d0, d1, …, dn)产生标准正态分布随机数dn为第n维数据的维度randint(low[, high, size, dtype])产生随机整数low：最小值；high：最大值；size：数据个数random_sample([size])在[0,1）内产生随机数size：随机数的shape``print np.random.randint(0,10,5)
print np.random.randn(2,2)
> 


输出 

  [[ 0.24565135 -0.70674316] 

   [-0.57555779  0.43117502]]5.2 排列函数名函数功能参数说明shuffle(x)打乱对象x（多维矩阵按照第一维打乱）矩阵或者列表permutation(x)打乱并返回该对象（多维矩阵按照第一维打乱）整数或者矩阵``a = np.array([10, 20, 30, 40])
np.random.shuffle(a)
print a
> 


输出 

  [30 40 20 10]5.3 分布函数名函数功能beta(a, b[, size])贝塔分布样本，在 [0, 1]内binomial(n, p[, size])二项分布的样本exponential([scale, size])指数分布logistic([loc, scale, size])Logistic分布样本lognormal([mean, sigma, size])对数正态分布multinomial(n, pvals[, size])多项分布multivariate_normal(mean, cov[, size])多元正态分布normal([loc, scale, size])正态(高斯)分布poisson([lam, size])泊松分布uniform([low, high, size])均匀分布``print np.random.normal(1, 5, 10)
print np.random.uniform([0, 1, 2])
> 


输出 

  [-4.38667406 -2.65368275  8.44676789  2.89982949  5.66674056] 

  [ 0.61799423  0.44157577]5.4 生成器- 
随机数是由随机种子根据一定的计算方法计算出来的数值。所以，只要计算方法一定，随机种子一定，那么产生的随机数就不会变。 - 
只要用户不设置随机种子，那么在默认情况下随机种子来自系统时钟（即定时/计数器的值）    - 
随机数产生的算法与系统有关，Windows和Linux是不同的，也就是说，即便是随机种子一样，不同系统产生的随机数也不一样。函数名函数功能参数说明RandomState定义种子类RandomState是一个种子类，提供了各种种子方法，最常用seedseed([seed])定义全局种子参数为整数或者矩阵``np.random.seed(1234)


































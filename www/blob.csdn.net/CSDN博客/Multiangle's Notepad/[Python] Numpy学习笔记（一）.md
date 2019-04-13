
# [Python] Numpy学习笔记（一） - Multiangle's Notepad - CSDN博客


2015年11月06日 19:21:51[multiangle](https://me.csdn.net/u014595019)阅读数：7677


最近开始学习numpy和pandas的一些东西，顺手做了些笔记，跟大家分享，也方便我以后查阅
---
**Ndarray: 一种多维数组对象**
```python
array
```
```python
.shape
```
```python
array
```
```python
的规格
```
```python
array
```
```python
.ndim
```
```python
array
```
```python
.dtype
```
```python
array
```
```python
的数据规格
numpy.zeros(dim1,dim2)              创建dim1*dim2的零矩阵
numpy.arange
numpy.eye(n) /numpy.identity(n)     创建n*n单位矩阵
numpy.
```
```python
array
```
```python
([…data…], dtype=float64 )
```
```python
array
```
```python
.astype(numpy.float64)         更换矩阵的数据形式
```
```python
array
```
```python
.astype(float)                 更换矩阵的数据形式
```
```python
array
```
```python
*
```
```python
array
```
```python
矩阵点乘
```
```python
array
```
```python
[a:b]                          切片
```
```python
array
```
```python
.
```
```python
copy
```
```python
()                        得到ndarray的副本，而不是视图
```
```python
array
```
```python
[a] [b]=
```
```python
array
```
```python
[ a, b ]        两者等价
name=np.
```
```python
array
```
```python
([
```
```python
'bob'
```
```python
,
```
```python
'joe'
```
```python
,
```
```python
'will'
```
```python
]) res=name==’bob’ res=
```
```python
array
```
```python
([
```
```python
True
```
```python
,
```
```python
False
```
```python
,
```
```python
False
```
```python
], dtype=bool)
data[
```
```python
True
```
```python
,
```
```python
False
```
```python
,…..]                索引，只索取为
```
```python
True
```
```python
的部分，去掉
```
```python
False
```
```python
部分
通过布尔型索引选取数组中的数据，将总是创建数据的副本。
data[ [
```
```python
4
```
```python
,
```
```python
3
```
```python
,
```
```python
0
```
```python
,
```
```python
6
```
```python
] ]                   索引，将第
```
```python
4
```
```python
,
```
```python
3
```
```python
,
```
```python
0
```
```python
,
```
```python
6
```
```python
行摘取出来，组成新数组
data[-
```
```python
1
```
```python
]=data[data.__len__()-
```
```python
1
```
```python
]
numpy.reshape(a,b)                  将a*b的一维数组排列为a*b的形式
```
```python
array
```
```python
([a,b,c,d],[d,e,f,g])          返回一维数组，分别为[a,d],[b,e],[c,f],[d,g]
```
```python
array
```
```python
[ [a,b,c,d] ][:,[e,f,g,h] ]=
```
```python
array
```
```python
[ numpy.ix_( [a,b,c,d],[e,f,g,h] ) ]
```
```python
array
```
```python
.T
```
```python
array
```
```python
的转置
numpy.random.randn(a,b)             生成a*b的随机数组
numpy.dot(matrix_1,matrix_2)        矩阵乘法
```
```python
array
```
```python
.transpose( (
```
```python
1
```
```python
,
```
```python
0
```
```python
,
```
```python
2
```
```python
,etc.) )     对于高维数组，转置需要一个由轴编号组成的元组
```
**ufunc：通用函数，简单函数的矢量化包装**
```python
一元ufunc
numpy.
```
```python
sqrt
```
```python
(
```
```python
array
```
```python
)                   平方根函数   
numpy.
```
```python
exp
```
```python
(
```
```python
array
```
```python
)                    e^
```
```python
array
```
```python
[i]的数组
numpy.
```
```python
abs
```
```python
/
```
```python
fabs
```
```python
(
```
```python
array
```
```python
)               计算绝对值
numpy.square(
```
```python
array
```
```python
)                 计算各元素的平方 等于
```
```python
array
```
```python
**
```
```python
2
```
```python
numpy.
```
```python
log
```
```python
/
```
```python
log10
```
```python
/log2(
```
```python
array
```
```python
)         计算各元素的各种对数
numpy.sign(
```
```python
array
```
```python
)                   计算各元素正负号
numpy.isnan(
```
```python
array
```
```python
)                  计算各元素是否为NaN
numpy.
```
```python
cos
```
```python
/
```
```python
cosh
```
```python
/
```
```python
sin
```
```python
/
```
```python
sinh
```
```python
/
```
```python
tan
```
```python
/
```
```python
tanh
```
```python
(
```
```python
array
```
```python
) 三角函数
numpy.
```
```python
modf
```
```python
(
```
```python
array
```
```python
)                   将
```
```python
array
```
```python
中值得整数和小数分离，作两个数组返回
二元ufunc
numpy.add(array1,array2)            元素级加法
numpy.subtract(array1,array2)       元素级减法
numpy.multiply(array1,array2)       元素级乘法
numpy.divide(array1,array2)         元素级除法 array1./array2
numpy.power(array1,array2)          元素级指数 array1.^array2
numpy.maximum/minimum(array1,aray2) 元素级最大值
numpy.fmax/fmin(array1,array2)      元素级最大值，忽略NaN
numpy.mod(array1,array2)            元素级求模
numpy.copysign(array1,array2)       将第二个数组中值得符号复制给第一个数组中值
numpy.greater/greater_equal/less/less_equal/equal/not_equal (array1,array2)
元素级比较运算，产生布尔数组
numpy.logical_end/logical_or/logic_xor(array1,array2)元素级的真值逻辑运算
```



# TensorFlow：tf.matmul和tf.einsum实现矩阵相乘 - 小花生的博客 - CSDN博客


2018年07月21日 08:12:51[Peanut_范](https://me.csdn.net/u013841196)阅读数：1958


TensorFlow：tf.matmul和tf.einsum实现矩阵相乘
## 1.TensorFlow：tf.matmul函数：
```python
函数：tf.matmul
matmul(
```
```python
a
```
```python
,
    b,
    transpose_a=
```
```python
False
```
```python
,
    transpose_b=
```
```python
False
```
```python
,
    adjoint_a=
```
```python
False
```
```python
,
    adjoint_b=
```
```python
False
```
```python
,
```
```python
a_is
```
```python
_sparse=
```
```python
False
```
```python
,
    b_is_sparse=
```
```python
False
```
```python
,
    name=None
)
```
将矩阵 a 乘以矩阵 b，生成a * b
输入必须在任何转换之后是 rank> = 2 的张量，其中内部 2 维度指定有效的矩阵乘法参数，并且任何其他外部维度匹配。
两个矩阵必须是相同类型。支持的类型有：float16，float32，float64，int32，complex64，complex128。
通过将相应的标志之一设置为 True，矩阵可以被转置或 adjointed(共轭和转置)。默认情况下，这些都是 False。
如果一个或两个矩阵包含很多的零，则可以通过将相应的 a_is_sparse 或 b_is_sparse 标志设置为 True 来使用更有效的乘法算法，默认为 false。这个优化仅适用于具有数据类型为bfloat16 或 float32 的纯矩阵（rank 为2的张量）。
**参数：**
a：类型为 float16，float32，float64，int32，complex64，complex128 和 rank > 1的张量。
b：与 a 具有相同类型和 rank。
transpose_a：如果 True，a 在乘法之前转置。
transpose_b：如果 True，b 在乘法之前转置。
adjoint_a：如果 True，a 在乘法之前共轭和转置。
adjoint_b：如果 True，b 在乘法之前共轭和转置。
a_is_sparse：如果 True，a 被视为稀疏矩阵。
b_is_sparse：如果 True，b 被视为稀疏矩阵。
name：操作名称（可选）。
**返回：**
该函数返回与 a 和 b 具有相同类型的张量，其中每个最内矩阵是 a 和 b 中对应矩阵的乘积，例如，如果所有转置或伴随的属性为 False：
output[…, i, j] = sum_k (a[…, i, k] * b[…, k, j]), for all indices i, j
Note：这是矩阵乘积，而不是元素的乘积。
**可能引发的异常：**
ValueError：如果 transpose_a 和 adjoint_a，或者 transpose_b 和 adjoint_b 都设置为 True。
## 2.TensorFlow如何实现张量收缩：
```python
tf.einsum：
```
```python
einsum
```
```python
(
    equation,
    *inputs
)
```
任意维度张量之间的广义收缩。
这个函数返回一个张量，其元素其元素是由等式定义的，这是由爱因斯坦求和公式所启发的速写形式定义的。作为示例，考虑将两个矩阵 A 和 B 相乘以形成矩阵C。C的元素由下式给出：
C[i,k] = sum_j A[i,j] * B[j,k]
相应的等式是：
ij,jk->ik
一般来说, 方程是从较熟悉的元素方程得到：
删除变量名称、括号和逗号；
用 “*” 替换 “，”；
删除总和标志；
将输出移到右侧，并将 “=” 替换为 “->>”。
许多常见操作可以用这种方式来表示。例如:
```python
# Matrix multiplication
```
```python
>>>
```
```python
einsum(
```
```python
'ij,jk->ik'
```
```python
, m0, m1)
```
```python
# output[i,k] = sum_j m0[i,j] * m1[j, k]
```
```python
# Dot product
```
```python
>>>
```
```python
einsum(
```
```python
'i,i->'
```
```python
, u, v)
```
```python
# output = sum_i u[i]*v[i]
```
```python
# Outer product
```
```python
>>>
```
```python
einsum(
```
```python
'i,j->ij'
```
```python
, u, v)
```
```python
# output[i,j] = u[i]*v[j]
```
```python
# Transpose
```
```python
>>>
```
```python
einsum(
```
```python
'ij->ji'
```
```python
, m)
```
```python
# output[j,i] = m[i,j]
```
```python
# Batch matrix multiplication
```
```python
>>>
```
```python
einsum(
```
```python
'aij,ajk->aik'
```
```python
, s, t)
```
```python
# out[a,i,k] = sum_j s[a,i,j] * t[a, j, k]
```
此函数的行为类似于 numpy.einsum，但不支持：
椭圆（下标像：ij…,jk…->ik…）
一个轴在单个输入上出现多次的下标（例如 ijj、k->> ik）。
在多个输入之间求和的下标（例如 ij、ij、jk->> ik）。
**ARGS：**
equation：一种描述收缩的 str，与 numpy.einsum 格式相同。
* inputs：合同的输入（每个张量），其形状应与 equation 一致。
**返回：**
返回收缩的张量，形状由 equation 决定。
**注意：**
ValueError：如果 equation 格式不正确，equation 隐含的输入数与 len(inputs) 不匹配，一个轴出现在输出下标中，但不显示在任何输入中，输入的维数与其下标中的索引数不同，或者输入形状沿特定轴线不一致。
---
**代码实现三维矩阵相乘：**
```python
# -*- coding: utf-
```
```python
8
```
```python
-*-
import tensorflow as tf
import numpy as np
m1 = tf.constant(
```
```python
[[[1.,2,3],[1,2,1]]
```
```python
,
```
```python
[[1,2,3],[1,2,1]]
```
```python
])
m2 = tf.constant(
```
```python
[[[1.,3],[2,1],[3,3]]
```
```python
,
```
```python
[[1,1],[1,2],[2,4]]
```
```python
])
# 现已将tf.batch_matmul替换成tf.matmul
r1 = tf.matmul(m1, m2) #tf.batch_matmul(m1,m2)
r2 = tf.einsum(
```
```python
'jmk,jkn->jmn'
```
```python
,m1,m2)
sess = tf.Session()
```
```python
print
```
```python
(sess.run(tf.shape(m1))) #[
```
```python
2
```
```python
2
```
```python
3
```
```python
]
```
```python
print
```
```python
(sess.run(tf.shape(m2))) #[
```
```python
2
```
```python
3
```
```python
2
```
```python
]
```
```python
print
```
```python
(sess.run(tf.shape(r1))) #[
```
```python
2
```
```python
2
```
```python
2
```
```python
]
```
```python
print
```
```python
(sess.run(r1))
#
```
```python
[[[ 14.  14.]
#  [  8.   8.]]
```
```python
#
#
```
```python
[[  9.  17.]
#  [  5.   9.]]
```
```python
]
```
```python
print
```
```python
(sess.run(tf.shape(r2))) #[
```
```python
2
```
```python
2
```
```python
2
```
```python
]
```
```python
print
```
```python
(sess.run(r2))
#
```
```python
[[[ 14.  14.]
#  [  8.   8.]]
```
```python
#
#
```
```python
[[  9.  17.]
#  [  5.   9.]]
```
```python
]
```
参考：[https://www.w3cschool.cn/tensorflow_python/](https://www.w3cschool.cn/tensorflow_python/)


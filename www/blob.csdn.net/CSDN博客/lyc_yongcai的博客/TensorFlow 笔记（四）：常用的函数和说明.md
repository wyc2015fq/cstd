
# TensorFlow 笔记（四）：常用的函数和说明 - lyc_yongcai的博客 - CSDN博客


2017年06月15日 10:08:51[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：251个人分类：[TensorFlow																](https://blog.csdn.net/lyc_yongcai/article/category/6970987)



# 1.矩阵操作
## 1.1矩阵生成
这部分主要将如何生成矩阵，包括全０矩阵，全１矩阵，随机数矩阵，常数矩阵等
### tf.ones | tf.zeros
**tf.ones(shape,type=tf.float32,name=None)**
**tf.zeros([2, 3], int32)**
用法类似，都是产生尺寸为shape的张量(tensor)
```python
sess = tf.InteractiveSession()
x = tf.ones([
```
```python
2
```
```python
,
```
```python
3
```
```python
], int32)
```
```python
print
```
```python
(sess.run(x))
#
```
```python
[[1 1 1],
# [1 1 1]]
```
1
2
3
4
5
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
### tf.ones_like | tf.zeros_like
**tf.ones_like(tensor,dype=None,name=None)**
**tf.zeros_like(tensor,dype=None,name=None)**
新建一个与给定的tensor类型大小一致的tensor，其所有元素为1和0
```python
tensor=
```
```python
[[1, 2, 3], [4, 5, 6]]
```
```python
x = tf.ones_like(tensor)
```
```python
print
```
```python
(sess.run(x))
#
```
```python
[[1 1 1],
# [1 1 1]]
```
1
2
3
4
5
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
### tf.fill
**tf.fill(shape,value,name=None)**
创建一个形状大小为shape的tensor，其初始值为value
```python
print
```
```python
(sess.run(tf.fill([
```
```python
2
```
```python
,
```
```python
3
```
```python
],
```
```python
2
```
```python
)))
#
```
```python
[[2 2 2],
# [2 2 2]]
```
1
2
3
![](http://static.blog.csdn.net/images/save_snippets_01.png)
1
2
3
### tf.constant
**tf.constant(value,dtype=None,shape=None,name=’Const’)**
创建一个常量tensor，按照给出value来赋值，可以用shape来指定其形状。value可以是一个数，也可以是一个list。
如果是一个数，那么这个常亮中所有值的按该数来赋值。
如果是list,那么len(value)一定要小于等于shape展开后的长度。赋值时，先将value中的值逐个存入。不够的部分，则全部存入value的最后一个值。
```python
a = tf.constant(
```
```python
2
```
```python
,shape=[
```
```python
2
```
```python
])
b = tf.constant(
```
```python
2
```
```python
,shape=[
```
```python
2
```
```python
,
```
```python
2
```
```python
])
c = tf.constant([
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
],shape=[
```
```python
6
```
```python
])
d = tf.constant([
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
],shape=[
```
```python
3
```
```python
,
```
```python
2
```
```python
])
sess = tf.InteractiveSession()
```
```python
print
```
```python
(sess.run(a))
#[
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
(sess.run(b))
#
```
```python
[[2 2]
# [2 2]]
```
```python
print
```
```python
(sess.run(c))
#[
```
```python
1
```
```python
2
```
```python
3
```
```python
3
```
```python
3
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
(sess.run(d))
#
```
```python
[[1 2]
# [3 3]
# [3 3]]
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
### tf.random_normal | tf.truncated_normal | tf.random_uniform
**tf.random_normal(shape,mean=0.0,stddev=1.0,dtype=tf.float32,seed=None,name=None)**
**tf.truncated_normal(shape, mean=0.0, stddev=1.0, dtype=tf.float32, seed=None, name=None)**
**tf.random_uniform(shape,minval=0,maxval=None,dtype=tf.float32,seed=None,name=None)**
这几个都是用于生成随机数tensor的。尺寸是shape
random_normal: 正太分布随机数，均值mean,标准差stddev
truncated_normal:截断正态分布随机数，均值mean,标准差stddev,不**过只保留[mean-2*stddev,mean+2*stddev]范围内的随机数**
random_uniform:均匀分布随机数，范围为[minval,maxval]
```python
sess = tf.InteractiveSession()
x = tf.random_normal(shape=[
```
```python
1
```
```python
,
```
```python
5
```
```python
],mean=
```
```python
0.0
```
```python
,stddev=
```
```python
1.0
```
```python
,dtype=tf.float32,seed=None,name=None)
```
```python
print
```
```python
(sess.run(x))
#===>
```
```python
[[-0.36128798  0.58550537 -0.88363433 -0.2677258   1.05080092]]
```
1
2
3
4
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
### tf.get_variable
```python
get_variable(name, shape=
```
```python
None
```
```python
, dtype=dtypes.float32, initializer=
```
```python
None
```
```python
,
                 regularizer=
```
```python
None
```
```python
, trainable=
```
```python
True
```
```python
, collections=
```
```python
None
```
```python
,
                 caching_device=
```
```python
None
```
```python
, partitioner=
```
```python
None
```
```python
, validate_shape=
```
```python
True
```
```python
,
                 custom_getter=
```
```python
None
```
```python
):
```
1
2
3
4
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
如果在该命名域中之前已经有名字=name的变量，则调用那个变量；如果没有，则根据输入的参数重新创建一个名字为name的变量。在众多的输入参数中，有几个是我已经比较了解的，下面来一一讲一下
name: 这个不用说了，变量的名字
shape: 变量的形状，[]表示一个数，[3]表示长为3的向量，[2,3]表示矩阵或者张量(tensor)
dtype: 变量的数据格式，主要有tf.int32, tf.float32, tf.float64等等
**initializer: 初始化工具，有tf.zero_initializer, tf.ones_initializer, tf.constant_initializer, tf.random_uniform_initializer, tf.random_normal_initializer, tf.truncated_normal_initializer等**
## 1.2 矩阵变换
### tf.shape
**tf.shape(Tensor)**
Returns the shape of a tensor.返回张量的形状。但是注意，**tf.shape函数本身也是返回一个张量**。而在tf中，张量是需要用`sess.run(Tensor)`来得到具体的值的。
```python
labels = [
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
]
shape = tf
```
```python
.shape
```
```python
(labels)
print(shape)
sess = tf
```
```python
.InteractiveSession
```
```python
()
print(sess
```
```python
.run
```
```python
(shape))
```
```python
# >>>Tensor("Shape:0", shape=(1,), dtype=int32)
```
```python
# >>>[3]
```
1
2
3
4
5
6
7
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
### tf.expand_dims
**tf.expand_dims(Tensor, dim)**
为张量+1维。官网的例子：’t’ is a tensor of shape [2]
shape(expand_dims(t, 0)) ==> [1, 2]
shape(expand_dims(t, 1)) ==> [2, 1]
shape(expand_dims(t, -1)) ==> [2, 1]
```python
sess = tf.InteractiveSession()
labels = [
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
]
x = tf.expand_dims(labels,
```
```python
0
```
```python
)
```
```python
print
```
```python
(sess.run(x))
x = tf.expand_dims(labels,
```
```python
1
```
```python
)
```
```python
print
```
```python
(sess.run(x))
#>>>
```
```python
[[1 2 3]]
```
```python
#>>>
```
```python
[[1]
#    [2]
#    [3]]
```
1
2
3
4
5
6
7
8
9
10
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
### tf.pack
**tf.pack(values, axis=0, name=”pack”)**
Packs a list of rank-`R`tensors into one rank-`(R+1)`tensor
将一个R维张量列表沿着axis轴组合成一个R+1维的张量。
```python
#
```
```python
'x'
```
```python
is [
```
```python
1
```
```python
,
```
```python
4
```
```python
]
  #
```
```python
'y'
```
```python
is [
```
```python
2
```
```python
,
```
```python
5
```
```python
]
  #
```
```python
'z'
```
```python
is [
```
```python
3
```
```python
,
```
```python
6
```
```python
]
  pack([x, y, z]) =>
```
```python
[[1, 4], [2, 5], [3, 6]]
```
```python
# Pack along first dim.
  pack([x, y, z], axis=
```
```python
1
```
```python
) =>
```
```python
[[1, 2, 3], [4, 5, 6]]
```
1
2
3
4
5
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
### tf.concat
**tf.concat(concat_dim, values, name=”concat”)**
Concatenates tensors along one dimension.
将张量沿着指定维数拼接起来。个人感觉跟前面的pack用法类似
```python
t1 =
```
```python
[[1, 2, 3], [4, 5, 6]]
```
```python
t2 =
```
```python
[[7, 8, 9], [10, 11, 12]]
```
```python
tf.concat(
```
```python
0
```
```python
, [t1, t2]) 
#==>
```
```python
[[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]
```
```python
tf.concat(
```
```python
1
```
```python
, [t1, t2]) 
#==>
```
```python
[[1, 2, 3, 7, 8, 9], [4, 5, 6, 10, 11, 12]]
```
1
2
3
4
5
6
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
### tf.sparse_to_dense
稀疏矩阵转密集矩阵
定义为：
```python
def
```
```python
sparse_to_dense
```
```python
(sparse_indices,
                    output_shape,
                    sparse_values,
                    default_value=
```
```python
0
```
```python
,
                    validate_indices=True,
                    name=None)
```
```python
:
```
1
2
3
4
5
6
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
几个参数的含义：
sparse_indices: 元素的坐标[[0,0],[1,2]] 表示(0,0)，和(1,2)处有值
output_shape: 得到的密集矩阵的shape
sparse_values: sparse_indices坐标表示的点的值，可以是0D或者1D张量。若0D，则所有稀疏值都一样。若是1D，则len(sparse_values)应该等于len(sparse_indices)
default_values: 缺省点的默认值
### tf.random_shuffle
**tf.random_shuffle(value,seed=None,name=None)**
沿着value的第一维进行随机重新排列
```python
sess = tf.InteractiveSession()
a=
```
```python
[[1,2],[3,4],[5,6]]
```
```python
x = tf.random_shuffle(a)
```
```python
print
```
```python
(sess.run(x))
#===>
```
```python
[[3 4],[5 6],[1 2]]
```
1
2
3
4
5
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
### tf.argmax | tf.argmin
tf.argmax(input=tensor,dimention=axis)
找到给定的张量tensor中在指定轴axis上的最大值/最小值的位置。
```python
a=tf
```
```python
.get
```
```python
_variable(name=
```
```python
'a'
```
```python
,
                  shape=[
```
```python
3
```
```python
,
```
```python
4
```
```python
],
                  dtype=tf
```
```python
.float
```
```python
32,
                  initializer=tf
```
```python
.random
```
```python
_uniform_initializer(minval=-
```
```python
1
```
```python
,maxval=
```
```python
1
```
```python
))
b=tf
```
```python
.argmax
```
```python
(input=a,dimension=
```
```python
0
```
```python
)
c=tf
```
```python
.argmax
```
```python
(input=a,dimension=
```
```python
1
```
```python
)
sess = tf
```
```python
.InteractiveSession
```
```python
()
sess
```
```python
.run
```
```python
(tf
```
```python
.initialize
```
```python
_all_variables())
print(sess
```
```python
.run
```
```python
(a))
```
```python
#[[ 0.04261756 -0.34297419 -0.87816691 -0.15430689]
```
```python
# [ 0.18663144  0.86972666 -0.06103253  0.38307118]
```
```python
# [ 0.84588599 -0.45432305 -0.39736366  0.38526249]]
```
```python
print(sess
```
```python
.run
```
```python
(b))
```
```python
#[2 1 1 2]
```
```python
print(sess
```
```python
.run
```
```python
(c))
```
```python
#[0 1 0]
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
### tf.equal
tf.equal(x, y, name=None):
判断两个tensor是否每个元素都相等。返回一个格式为bool的tensor
### tf.cast
cast(x, dtype, name=None)
将x的数据格式转化成dtype.例如，原来x的数据格式是bool，那么将其转化成float以后，就能够将其转化成0和1的序列。反之也可以
```python
a = tf
```
```python
.Variable
```
```python
([
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
0
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
])
b = tf
```
```python
.cast
```
```python
(a,dtype=tf
```
```python
.bool
```
```python
)
sess = tf
```
```python
.InteractiveSession
```
```python
()
sess
```
```python
.run
```
```python
(tf
```
```python
.initialize
```
```python
_all_variables())
print(sess
```
```python
.run
```
```python
(b))
```
```python
#[ True False False  True  True]
```
1
2
3
4
5
6
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
### tf.matmul
用来做矩阵乘法。若a为l*m的矩阵，b为m*n的矩阵，那么通过tf.matmul(a,b) 结果就会得到一个l*n的矩阵
不过这个函数还提供了很多额外的功能。我们来看下函数的定义：
```python
matmul(
```
```python
a
```
```python
, b,
           transpose_a=
```
```python
False
```
```python
, transpose_b=
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
, b_is_sparse=
```
```python
False
```
```python
,
```
```python
name=None):
```
1
2
3
4
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
可以看到还提供了transpose和is_sparse的选项。
如果对应的transpose项为True，例如transpose_a=True,那么a在参与运算之前就会先转置一下。
而如果a_is_sparse=True,那么a会被当做稀疏矩阵来参与运算。
### tf.reshape
reshape(tensor, shape, name=None)
顾名思义，就是将tensor按照新的shape重新排列。一般来说，shape有三种用法：
如果 shape=[-1], 表示要将tensor展开成一个list
如果 shape=[a,b,c,…] 其中每个a,b,c,..均>0，那么就是常规用法
如果 shape=[a,-1,c,…] 此时b=-1，a,c,..依然>0。这表示tf会根据tensor的原尺寸，自动计算b的值。
官方给的例子已经很详细了，我就不写示例代码了
```python
# tensor
```
```python
't'
```
```python
is [
```
```python
1
```
```python
,
```
```python
2
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
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
,
```
```python
7
```
```python
,
```
```python
8
```
```python
,
```
```python
9
```
```python
]
# tensor
```
```python
't'
```
```python
has shape [
```
```python
9
```
```python
]
reshape(t, [
```
```python
3
```
```python
,
```
```python
3
```
```python
]) ==>
```
```python
[[1, 2, 3],
                        [4, 5, 6],
                        [7, 8, 9]]
```
```python
# tensor
```
```python
't'
```
```python
is
```
```python
[[[1, 1], [2, 2]]
```
```python
,
#
```
```python
[[3, 3], [4, 4]]
```
```python
]
# tensor
```
```python
't'
```
```python
has shape [
```
```python
2
```
```python
,
```
```python
2
```
```python
,
```
```python
2
```
```python
]
reshape(t, [
```
```python
2
```
```python
,
```
```python
4
```
```python
]) ==>
```
```python
[[1, 1, 2, 2],
                        [3, 3, 4, 4]]
```
```python
# tensor
```
```python
't'
```
```python
is
```
```python
[[[1, 1, 1],
#                 [2, 2, 2]]
```
```python
,
#
```
```python
[[3, 3, 3],
#                 [4, 4, 4]]
```
```python
,
#
```
```python
[[5, 5, 5],
#                 [6, 6, 6]]
```
```python
]
# tensor
```
```python
't'
```
```python
has shape [
```
```python
3
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
]
# pass
```
```python
'[-1]'
```
```python
to flatten
```
```python
't'
```
```python
reshape(t, [-
```
```python
1
```
```python
]) ==> [
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
2
```
```python
,
```
```python
2
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
3
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
4
```
```python
,
```
```python
4
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
5
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
,
```
```python
6
```
```python
,
```
```python
6
```
```python
]
# -
```
```python
1
```
```python
can also be used to infer the shape
# -
```
```python
1
```
```python
is inferred to be
```
```python
9
```
```python
:
reshape(t, [
```
```python
2
```
```python
, -
```
```python
1
```
```python
]) ==>
```
```python
[[1, 1, 1, 2, 2, 2, 3, 3, 3],
                         [4, 4, 4, 5, 5, 5, 6, 6, 6]]
```
```python
# -
```
```python
1
```
```python
is inferred to be
```
```python
2
```
```python
:
reshape(t, [-
```
```python
1
```
```python
,
```
```python
9
```
```python
]) ==>
```
```python
[[1, 1, 1, 2, 2, 2, 3, 3, 3],
                         [4, 4, 4, 5, 5, 5, 6, 6, 6]]
```
```python
# -
```
```python
1
```
```python
is inferred to be
```
```python
3
```
```python
:
reshape(t, [
```
```python
2
```
```python
, -
```
```python
1
```
```python
,
```
```python
3
```
```python
]) ==>
```
```python
[[[1, 1, 1],
                              [2, 2, 2],
                              [3, 3, 3]]
```
```python
,
```
```python
[[4, 4, 4],
                              [5, 5, 5],
                              [6, 6, 6]]
```
```python
]
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
# 2. 神经网络相关操作
### tf.nn.embedding_lookup
embedding_lookup(params, ids, partition_strategy=”mod”, name=None,
validate_indices=True):
简单的来讲，就是将一个数字序列ids转化成embedding序列表示。
假设params.shape=[v,h], ids.shape=[m], 那么该函数会返回一个shape=[m,h]的张量。用数学来表示，就是

那么这个有什么用呢？如果你了解[word2vec](http://blog.csdn.net/u014595019/article/details/51884529)的话，就知道我们可以根据文档来对每个单词生成向量。单词向量可以进一步用来测量单词的相似度等等。那么假设我们现在已经获得了每个单词的向量，都存在param中。那么根据单词id序列ids,就可以通过embedding_lookup来获得embedding表示的序列。
### tf.trainable_variables
**返回所有可训练的变量**。
在创造变量(tf.Variable, tf.get_variable 等操作)时，都会有一个trainable的选项，表示该变量是否可训练。这个函数会返回图中所有trainable=True的变量。
**tf.get_variable(…), tf.Variable(…)的默认选项是True, 而 tf.constant(…)只能是False**
```python
import tensorflow as tf
from pprint import pprint
a = tf.get_variable(
```
```python
'a'
```
```python
,shape=[
```
```python
5
```
```python
,
```
```python
2
```
```python
])    # 默认 trainable=True
b = tf.get_variable(
```
```python
'b'
```
```python
,shape=[
```
```python
2
```
```python
,
```
```python
5
```
```python
],trainable=False)
c = tf.constant([
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
],dtype=tf.int32,shape=[
```
```python
8
```
```python
],name=
```
```python
'c'
```
```python
) # 因为是常量，所以trainable=False
d = tf.Variable(tf.random_uniform(shape=[
```
```python
3
```
```python
,
```
```python
3
```
```python
]),name=
```
```python
'd'
```
```python
)
tvar = tf.trainable_variables()
tvar_name = [x.name
```
```python
for
```
```python
x
```
```python
in
```
```python
tvar]
```
```python
print
```
```python
(tvar)
# [<tensorflow.python.ops.variables.Variable object at
```
```python
0x7f9c8db8ca20
```
```python
>, <tensorflow.python.ops.variables.Variable object at
```
```python
0x7f9c8db8c9b0
```
```python
>]
```
```python
print
```
```python
(tvar_name)
# [
```
```python
'a:0'
```
```python
,
```
```python
'd:0'
```
```python
]
sess = tf.InteractiveSession()
sess.run(tf.initialize_all_variables())
pprint(sess.run(tvar))
#[array(
```
```python
[[ 0.27307487, -0.66074866],
#       [ 0.56380701,  0.62759042],
#       [ 0.50012994,  0.42331111],
#       [ 0.29258847, -0.09185416],
#       [-0.35913971,  0.3228929 ]]
```
```python
, dtype=float32),
# array(
```
```python
[[ 0.85308731,  0.73948073,  0.63190091],
#       [ 0.5821209 ,  0.74533939,  0.69830012],
#       [ 0.61058474,  0.76497936,  0.10329771]]
```
```python
, dtype=float32)]
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
### tf.gradients
用来计算导数。该函数的定义如下所示
```python
def
```
```python
gradients
```
```python
(ys,
              xs,
              grad_ys=None,
              name=
```
```python
"gradients"
```
```python
,
              colocate_gradients_with_ops=False,
              gate_gradients=False,
              aggregation_method=None)
```
```python
:
```
1
2
3
4
5
6
7
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
虽然可选参数很多，但是最常使用的还是ys和xs。根据说明得知，ys和xs都可以是一个tensor或者tensor列表。而计算完成以后，该函数会返回一个长为len(xs)的tensor列表，列表中的每个tensor是ys中每个值对xs[i]求导之和。如果用数学公式表示的话，那么`g = tf.gradients(y,x)`可以表示成


### tf.clip_by_global_norm
修正梯度值，用于**控制梯度爆炸的问题**。梯度爆炸和梯度弥散的原因一样，都是因为链式法则求导的关系，导致梯度的指数级衰减。为了避免梯度爆炸，需要对梯度进行修剪。
先来看这个函数的定义：
```python
def
```
```python
clip_by_global_norm
```
```python
(t_list, clip_norm, use_norm=None, name=None)
```
```python
:
```
1
![](http://static.blog.csdn.net/images/save_snippets.png)
1
**输入参数**中：t_list为待修剪的张量, clip_norm 表示修剪比例(clipping ratio).
函数**返回2个参数**： list_clipped，修剪后的张量，以及global_norm，一个中间计算量。当然如果你之前已经计算出了global_norm值，你可以在use_norm选项直接指定global_norm的值。
那么具体**如何计算**呢？根据源码中的说明，可以得到
list_clipped[i]=t_list[i] * clip_norm / max(global_norm, clip_norm),其中
global_norm = sqrt(sum([l2norm(t)**2 for t in t_list]))
如果你更熟悉数学公式，则可以写作

其中，
和代表t_list[i]和list_clipped[i]，
和代表clip_norm
 和 global_norm的值。
其实也可以看到其实就是t_list的L2模。上式也可以进一步写作

也就是说，当t_list的L2模大于指定的时，就会对t_list做等比例缩放
### tf.nn.dropout
dropout(x, keep_prob, noise_shape=None, seed=None, name=None)
**按概率来将x中的一些元素值置零，并将其他的值放大**。用于进行dropout操作，一定程度上可以防止过拟合
x是一个张量，而keep_prob是一个（0,1]之间的值。x中的各个元素清零的概率互相独立，为1-keep_prob,而没有清零的元素，则会统一乘以1/keep_prob, 目的是为了保持x的整体期望值不变。
```python
sess = tf.InteractiveSession()
a = tf.get_variable(
```
```python
'a'
```
```python
,shape=[
```
```python
2
```
```python
,
```
```python
5
```
```python
])
b = a
a_drop = tf.nn.dropout(a,
```
```python
0.8
```
```python
)
sess.run(tf.initialize_all_variables())
```
```python
print
```
```python
(sess.run(b))
#
```
```python
[[ 0.28667903 -0.66874665 -1.14635754  0.88610041 -0.55590457]
# [-0.29704338 -0.01958954  0.80359757  0.75945008  0.74934876]]
```
```python
print
```
```python
(sess.run(a_drop))
#
```
```python
[[ 0.35834879 -0.83593333 -1.43294692  1.10762548 -0.        ]
# [-0.37130421 -0.          0.          0.94931257  0.93668592]]
```
1
2
3
4
5
6
7
8
9
10
11
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
11
# 3.普通操作
### tf.linspace | tf.range
**tf.linspace(start,stop,num,name=None)**
**tf.range(start,limit=None,delta=1,name=’range’)**
这两个放到一起说，是因为他们都用于产生等差数列，不过具体用法不太一样。
tf.linspace在[start,stop]范围内产生num个数的等差数列。不过注意，**start和stop要用浮点数表示，不然会报错**
tf.range在[start,limit)范围内以步进值delta产生等差数列。注意是不包括limit在内的。
```python
sess = tf
```
```python
.InteractiveSession
```
```python
()
```
```python
x
```
```python
= tf
```
```python
.linspace
```
```python
(start=
```
```python
1.0
```
```python
,stop=
```
```python
5.0
```
```python
,num=
```
```python
5
```
```python
,name=None)
```
```python
# 注意1.0和5.0
```
```python
y
```
```python
= tf
```
```python
.range
```
```python
(start=
```
```python
1
```
```python
,limit=
```
```python
5
```
```python
,delta=
```
```python
1
```
```python
)
print(sess
```
```python
.run
```
```python
(
```
```python
x
```
```python
))
print(sess
```
```python
.run
```
```python
(
```
```python
y
```
```python
))
```
```python
#===>[ 1.  2.  3.  4.  5.]
```
```python
#===>[1 2 3 4]
```
1
2
3
4
5
6
7
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
### tf.assign
**assign(ref, value, validate_shape=None, use_locking=None, name=None)**
tf.assign是用来更新模型中变量的值的。ref是待赋值的变量，value是要更新的值。即效果等同于`ref = value`
简单的实例代码见下
```python
sess = tf
```
```python
.InteractiveSession
```
```python
()
a = tf
```
```python
.Variable
```
```python
(
```
```python
0.0
```
```python
)
b = tf
```
```python
.placeholder
```
```python
(dtype=tf
```
```python
.float
```
```python
32,shape=[])
op = tf
```
```python
.assign
```
```python
(a,b)
sess
```
```python
.run
```
```python
(tf
```
```python
.initialize
```
```python
_all_variables())
print(sess
```
```python
.run
```
```python
(a))
```
```python
# 0.0
```
```python
sess
```
```python
.run
```
```python
(op,feed_dict={b:
```
```python
5.
```
```python
})
print(sess
```
```python
.run
```
```python
(a))
```
```python
# 5.0
```
1
2
3
4
5
6
7
8
9
10
11
12
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
5
6
7
8
9
10
11
12
# 4.规范化
### tf.variable_scope
简单的来讲，就是为变量添加命名域
```python
with
```
```python
tf.variable_scope(
```
```python
"foo"
```
```python
):
```
```python
with
```
```python
tf.variable_scope(
```
```python
"bar"
```
```python
):
          v = tf.get_variable(
```
```python
"v"
```
```python
, [
```
```python
1
```
```python
])
```
```python
assert
```
```python
v.name ==
```
```python
"foo/bar/v:0"
```
1
2
3
4
![](http://static.blog.csdn.net/images/save_snippets.png)
1
2
3
4
函数的定义为
```python
def
```
```python
variable_scope
```
```python
(name_or_scope, reuse=None, initializer=None,
                   regularizer=None, caching_device=None, partitioner=None,
                   custom_getter=None)
```
```python
:
```
1
2
3
![](http://static.blog.csdn.net/images/save_snippets_01.png)
1
2
3
各变量的含义如下：
name_or_scope:`string`or`VariableScope`: the scope to open.
reuse:`True`or`None`; if`True`, we[
Go](http://lib.csdn.net/base/go)into reuse mode for this scope as well as all sub-scopes; if`None`, we just inherit the parent scope reuse. 如果reuse=True, 那么就是使用之前定义过的name_scope和其中的变量，
initializer: default initializer for variables within this scope.
regularizer: default regularizer for variables within this scope.
caching_device: default caching device for variables within this scope.
partitioner: default partitioner for variables within this scope.
custom_getter: default custom getter for variables within this scope.
### tf.get_variable_scope
返回当前变量的命名域，返回一个tensorflow.[Python](http://lib.csdn.net/base/python).ops.variable_scope.VariableScope变量。



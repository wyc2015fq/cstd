# 【tensorflow】tensorflow常用函数 - zkq_1986的博客 - CSDN博客





2018年10月10日 17:23:15[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：91
个人分类：[tensorflow](https://blog.csdn.net/zkq_1986/article/category/7167521)









# 1.矩阵操作

## 1.1矩阵生成

这部分主要将如何生成矩阵，包括全０矩阵，全１矩阵，随机数矩阵，常数矩阵等

### tf.ones | tf.zeros

**tf.ones(shape,type=tf.float32,name=None)**
**tf.zeros([2, 3], int32)**

用法类似，都是产生尺寸为shape的张量(tensor)

```
sess = tf.InteractiveSession()
x = tf.ones([2, 3], int32)
print(sess.run(x))
#[[1 1 1],
# [1 1 1]]
```

### tf.ones_like | tf.zeros_like

**tf.ones_like(tensor,dype=None,name=None)**
**tf.zeros_like(tensor,dype=None,name=None)**

新建一个与给定的tensor类型大小一致的tensor，其所有元素为1和0

```
tensor=[[1, 2, 3], [4, 5, 6]] 
x = tf.ones_like(tensor) 
print(sess.run(x))
#[[1 1 1],
# [1 1 1]]
```
- 

### tf.fill

**tf.fill(shape,value,name=None)**

创建一个形状大小为shape的tensor，其初始值为value

```
print(sess.run(tf.fill([2,3],2)))
#[[2 2 2],
# [2 2 2]]
```

### tf.constant

**tf.constant(value,dtype=None,shape=None,name=’Const’)**

创建一个常量tensor，按照给出value来赋值，可以用shape来指定其形状。value可以是一个数，也可以是一个list。

如果是一个数，那么这个常亮中所有值的按该数来赋值。

如果是list,那么len(value)一定要小于等于shape展开后的长度。赋值时，先将value中的值逐个存入。不够的部分，则全部存入value的最后一个值。

```
a = tf.constant(2,shape=[2])
b = tf.constant(2,shape=[2,2])
c = tf.constant([1,2,3],shape=[6])
d = tf.constant([1,2,3],shape=[3,2])

sess = tf.InteractiveSession()
print(sess.run(a))
#[2 2]
print(sess.run(b))
#[[2 2]
# [2 2]]
print(sess.run(c))
#[1 2 3 3 3 3]
print(sess.run(d))
#[[1 2]
# [3 3]
# [3 3]]
```

### tf.random_normal | tf.truncated_normal | tf.random_uniform

**tf.random_normal(shape,mean=0.0,stddev=1.0,dtype=tf.float32,seed=None,name=None)**
**tf.truncated_normal(shape, mean=0.0, stddev=1.0, dtype=tf.float32, seed=None, name=None)**
**tf.random_uniform(shape,minval=0,maxval=None,dtype=tf.float32,seed=None,name=None)**

这几个都是用于生成随机数tensor的。尺寸是shape

random_normal: 正太分布随机数，均值mean,标准差stddev

truncated_normal:截断正态分布随机数，均值mean,标准差stddev,不**过只保留[mean-2*stddev,mean+2*stddev]范围内的随机数**

random_uniform:均匀分布随机数，范围为[minval,maxval]

```
sess = tf.InteractiveSession()
x = tf.random_normal(shape=[1,5],mean=0.0,stddev=1.0,dtype=tf.float32,seed=None,name=None)
print(sess.run(x))
#===>[[-0.36128798  0.58550537 -0.88363433 -0.2677258   1.05080092]]
```

### tf.get_variable

```
get_variable(name, shape=None, dtype=dtypes.float32, initializer=None,
                 regularizer=None, trainable=True, collections=None,
                 caching_device=None, partitioner=None, validate_shape=True,
                 custom_getter=None):
```

如果在该命名域中之前已经有名字=name的变量，则调用那个变量；如果没有，则根据输入的参数重新创建一个名字为name的变量。在众多的输入参数中，有几个是我已经比较了解的，下面来一一讲一下

name: 这个不用说了，变量的名字

shape: 变量的形状，[]表示一个数，[3]表示长为3的向量，[2,3]表示矩阵或者张量(tensor)

dtype: 变量的数据格式，主要有tf.int32, tf.float32, tf.float64等等
**initializer: 初始化工具，有tf.zero_initializer, tf.ones_initializer, tf.constant_initializer, tf.random_uniform_initializer, tf.random_normal_initializer, tf.truncated_normal_initializer等**

## 1.2 矩阵变换

### tf.shape

**tf.shape(Tensor)**

Returns the shape of a tensor.返回张量的形状。但是注意，**tf.shape函数本身也是返回一个张量**。而在tf中，张量是需要用`sess.run(Tensor)`来得到具体的值的。

```
labels = [1,2,3]
shape = tf.shape(labels)
print(shape)
sess = tf.InteractiveSession()
print(sess.run(shape))
# >>>Tensor("Shape:0", shape=(1,), dtype=int32)
# >>>[3]
```

### tf.expand_dims

**tf.expand_dims(Tensor, dim)**

为张量+1维。官网的例子：’t’ is a tensor of shape [2]

shape(expand_dims(t, 0)) ==> [1, 2]

shape(expand_dims(t, 1)) ==> [2, 1]

shape(expand_dims(t, -1)) ==> [2, 1]

```
sess = tf.InteractiveSession()
labels = [1,2,3]
x = tf.expand_dims(labels, 0)
print(sess.run(x))
x = tf.expand_dims(labels, 1)
print(sess.run(x))
#>>>[[1 2 3]]
#>>>[[1]
#    [2]
#    [3]]
```
- 

### tf.pack

**tf.pack(values, axis=0, name=”pack”)**

Packs a list of rank-`R` tensors into one rank-`(R+1)` tensor

将一个R维张量列表沿着axis轴组合成一个R+1维的张量。

```
# 'x' is [1, 4]
  # 'y' is [2, 5]
  # 'z' is [3, 6]
  pack([x, y, z]) => [[1, 4], [2, 5], [3, 6]]  # Pack along first dim.
  pack([x, y, z], axis=1) => [[1, 2, 3], [4, 5, 6]]
```

### tf.concat

**tf.concat(concat_dim, values, name=”concat”)**

Concatenates tensors along one dimension.

将张量沿着指定维数拼接起来。个人感觉跟前面的pack用法类似

```
t1 = [[1, 2, 3], [4, 5, 6]]
t2 = [[7, 8, 9], [10, 11, 12]]
tf.concat(0, [t1, t2]) 
#==> [[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12]]
tf.concat(1, [t1, t2]) 
#==> [[1, 2, 3, 7, 8, 9], [4, 5, 6, 10, 11, 12]]
```

### tf.sparse_to_dense

稀疏矩阵转密集矩阵

定义为：

```
def sparse_to_dense(sparse_indices,
                    output_shape,
                    sparse_values,
                    default_value=0,
                    validate_indices=True,
                    name=None):
```

几个参数的含义：

sparse_indices: 元素的坐标[[0,0],[1,2]] 表示(0,0)，和(1,2)处有值

output_shape: 得到的密集矩阵的shape

sparse_values: sparse_indices坐标表示的点的值，可以是0D或者1D张量。若0D，则所有稀疏值都一样。若是1D，则len(sparse_values)应该等于len(sparse_indices)

default_values: 缺省点的默认值

### tf.random_shuffle

**tf.random_shuffle(value,seed=None,name=None)**

沿着value的第一维进行随机重新排列

```
sess = tf.InteractiveSession()
a=[[1,2],[3,4],[5,6]]
x = tf.random_shuffle(a)
print(sess.run(x))
#===>[[3 4],[5 6],[1 2]]
```

### tf.argmax | tf.argmin

tf.argmax(input=tensor,dimention=axis)

找到给定的张量tensor中在指定轴axis上的最大值/最小值的位置。

```
a=tf.get_variable(name='a',
                  shape=[3,4],
                  dtype=tf.float32,
                  initializer=tf.random_uniform_initializer(minval=-1,maxval=1))
b=tf.argmax(input=a,dimension=0)
c=tf.argmax(input=a,dimension=1)
sess = tf.InteractiveSession()
sess.run(tf.initialize_all_variables())
print(sess.run(a))
#[[ 0.04261756 -0.34297419 -0.87816691 -0.15430689]
# [ 0.18663144  0.86972666 -0.06103253  0.38307118]
# [ 0.84588599 -0.45432305 -0.39736366  0.38526249]]
print(sess.run(b))
#[2 1 1 2]
print(sess.run(c))
#[0 1 0]
```

### tf.equal

tf.equal(x, y, name=None):

判断两个tensor是否每个元素都相等。返回一个格式为bool的tensor

### tf.cast

cast(x, dtype, name=None)

将x的数据格式转化成dtype.例如，原来x的数据格式是bool，那么将其转化成float以后，就能够将其转化成0和1的序列。反之也可以

```
a = tf.Variable([1,0,0,1,1])
b = tf.cast(a,dtype=tf.bool)
sess = tf.InteractiveSession()
sess.run(tf.initialize_all_variables())
print(sess.run(b))
#[ True False False  True  True]
```

### tf.matmul

用来做矩阵乘法。若a为l*m的矩阵，b为m*n的矩阵，那么通过tf.matmul(a,b) 结果就会得到一个l*n的矩阵

不过这个函数还提供了很多额外的功能。我们来看下函数的定义：

```
matmul(a, b,
           transpose_a=False, transpose_b=False,
           a_is_sparse=False, b_is_sparse=False,
           name=None):
```

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

```
# tensor 't' is [1, 2, 3, 4, 5, 6, 7, 8, 9]
# tensor 't' has shape [9]
reshape(t, [3, 3]) ==> [[1, 2, 3],
                        [4, 5, 6],
                        [7, 8, 9]]

# tensor 't' is [[[1, 1], [2, 2]],
#                [[3, 3], [4, 4]]]
# tensor 't' has shape [2, 2, 2]
reshape(t, [2, 4]) ==> [[1, 1, 2, 2],
                        [3, 3, 4, 4]]

# tensor 't' is [[[1, 1, 1],
#                 [2, 2, 2]],
#                [[3, 3, 3],
#                 [4, 4, 4]],
#                [[5, 5, 5],
#                 [6, 6, 6]]]
# tensor 't' has shape [3, 2, 3]
# pass '[-1]' to flatten 't'
reshape(t, [-1]) ==> [1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6]

# -1 can also be used to infer the shape
# -1 is inferred to be 9:
reshape(t, [2, -1]) ==> [[1, 1, 1, 2, 2, 2, 3, 3, 3],
                         [4, 4, 4, 5, 5, 5, 6, 6, 6]]

# -1 is inferred to be 2:
reshape(t, [-1, 9]) ==> [[1, 1, 1, 2, 2, 2, 3, 3, 3],
                         [4, 4, 4, 5, 5, 5, 6, 6, 6]]

# -1 is inferred to be 3:
reshape(t, [ 2, -1, 3]) ==> [[[1, 1, 1],
                              [2, 2, 2],
                              [3, 3, 3]],
                             [[4, 4, 4],
                              [5, 5, 5],
                              [6, 6, 6]]]
```

# 2. 神经网络相关操作

### tf.nn.embedding_lookup

embedding_lookup(params, ids, partition_strategy=”mod”, name=None,

validate_indices=True):

简单的来讲，就是将一个数字序列ids转化成embedding序列表示。

假设params.shape=[v,h], ids.shape=[m], 那么该函数会返回一个shape=[m,h]的张量。用数学来表示，就是

ids=[i1,i2,…,im]params=⎡⎣⎢⎢⎢⎢⎢w11,w21,⋯,wh1w12,w22,⋯,wh2⋮w1v,w2v,⋯,whv⎤⎦⎥⎥⎥⎥⎥res=⎡⎣⎢⎢⎢⎢⎢⎢w1i1,w2i1,…,whi1w1i2,w2i2,…,whi2⋮w1im,w2im,…,whim⎤⎦⎥⎥⎥⎥⎥⎥ids=[i1,i2,…,im]params=[w11,w12,⋯,w1hw21,w22,⋯,w2h⋮wv1,wv2,⋯,wvh]res=[wi11,wi12,…,wi1hwi21,wi22,…,wi2h⋮wim1,wim2,…,wimh]



那么这个有什么用呢？如果你了解[word2vec](http://blog.csdn.net/u014595019/article/details/51884529)的话，就知道我们可以根据文档来对每个单词生成向量。单词向量可以进一步用来测量单词的相似度等等。那么假设我们现在已经获得了每个单词的向量，都存在param中。那么根据单词id序列ids,就可以通过embedding_lookup来获得embedding表示的序列。



### tf.trainable_variables

**返回所有可训练的变量**。

在创造变量(tf.Variable, tf.get_variable 等操作)时，都会有一个trainable的选项，表示该变量是否可训练。这个函数会返回图中所有trainable=True的变量。
**tf.get_variable(…), tf.Variable(…)的默认选项是True, 而 tf.constant(…)只能是False**

```
import tensorflow as tf
from pprint import pprint

a = tf.get_variable('a',shape=[5,2])    # 默认 trainable=True
b = tf.get_variable('b',shape=[2,5],trainable=False)
c = tf.constant([1,2,3],dtype=tf.int32,shape=[8],name='c') # 因为是常量，所以trainable=False
d = tf.Variable(tf.random_uniform(shape=[3,3]),name='d')
tvar = tf.trainable_variables()
tvar_name = [x.name for x in tvar]
print(tvar)
# [<tensorflow.python.ops.variables.Variable object at 0x7f9c8db8ca20>, <tensorflow.python.ops.variables.Variable object at 0x7f9c8db8c9b0>]
print(tvar_name)
# ['a:0', 'd:0']

sess = tf.InteractiveSession()
sess.run(tf.initialize_all_variables())
pprint(sess.run(tvar))
#[array([[ 0.27307487, -0.66074866],
#       [ 0.56380701,  0.62759042],
#       [ 0.50012994,  0.42331111],
#       [ 0.29258847, -0.09185416],
#       [-0.35913971,  0.3228929 ]], dtype=float32),
# array([[ 0.85308731,  0.73948073,  0.63190091],
#       [ 0.5821209 ,  0.74533939,  0.69830012],
#       [ 0.61058474,  0.76497936,  0.10329771]], dtype=float32)]
```

### tf.gradients

用来计算导数。该函数的定义如下所示

```
def gradients(ys,
              xs,
              grad_ys=None,
              name="gradients",
              colocate_gradients_with_ops=False,
              gate_gradients=False,
              aggregation_method=None):
```

虽然可选参数很多，但是最常使用的还是ys和xs。根据说明得知，ys和xs都可以是一个tensor或者tensor列表。而计算完成以后，该函数会返回一个长为len(xs)的tensor列表，列表中的每个tensor是ys中每个值对xs[i]求导之和。如果用数学公式表示的话，那么 `g = tf.gradients(y,x)`可以表示成

gi=∑j=0len(y)∂yj∂xig=[g0,g1,...,glen(x)]gi=∑j=0len(y)∂yj∂xig=[g0,g1,...,glen(x)]



### tf.clip_by_global_norm

修正梯度值，用于**控制梯度爆炸的问题**。梯度爆炸和梯度弥散的原因一样，都是因为链式法则求导的关系，导致梯度的指数级衰减。为了避免梯度爆炸，需要对梯度进行修剪。

先来看这个函数的定义：
`def clip_by_global_norm(t_list, clip_norm, use_norm=None, name=None):`- 1

**输入参数**中：t_list为待修剪的张量, clip_norm 表示修剪比例(clipping ratio).

函数**返回2个参数**： list_clipped，修剪后的张量，以及global_norm，一个中间计算量。当然如果你之前已经计算出了global_norm值，你可以在use_norm选项直接指定global_norm的值。

那么具体**如何计算**呢？根据源码中的说明，可以得到

list_clipped[i]=t_list[i] * clip_norm / max(global_norm, clip_norm),其中

global_norm = sqrt(sum([l2norm(t)**2 for t in t_list]))

如果你更熟悉数学公式，则可以写作

Lic=Lit∗Ncmax(Nc,Ng)Ng=∑i(Lit)2−−−−−−−√Lci=Lti∗Ncmax(Nc,Ng)Ng=∑i(Lti)2



其中，

LicLci和LigLgi代表t_list[i]和list_clipped[i]，

NcNc和NgNg代表clip_norm 和 global_norm的值。

其实也可以看到其实NgNg就是t_list的L2模。上式也可以进一步写作

Lic={Lit,(Ng<=Nc)Lit∗NcNg,(Ng>Nc)Ng=∑i(Lit)2−−−−−−−√Lci={Lti,(Ng<=Nc)Lti∗NcNg,(Ng>Nc)Ng=∑i(Lti)2



也就是说，当t_list的L2模大于指定的NcNc时，就会对t_list做等比例缩放



### tf.nn.dropout

dropout(x, keep_prob, noise_shape=None, seed=None, name=None)
**按概率来将x中的一些元素值置零，并将其他的值放大**。用于进行dropout操作，一定程度上可以防止过拟合

x是一个张量，而keep_prob是一个（0,1]之间的值。x中的各个元素清零的概率互相独立，为1-keep_prob,而没有清零的元素，则会统一乘以1/keep_prob, 目的是为了保持x的整体期望值不变。

# 3.普通操作

### tf.linspace | tf.range

**tf.linspace(start,stop,num,name=None)**
**tf.range(start,limit=None,delta=1,name=’range’)**

这两个放到一起说，是因为他们都用于产生等差数列，不过具体用法不太一样。

tf.linspace在[start,stop]范围内产生num个数的等差数列。不过注意，**start和stop要用浮点数表示，不然会报错**

tf.range在[start,limit)范围内以步进值delta产生等差数列。注意是不包括limit在内的。

```
sess = tf.InteractiveSession()
x = tf.linspace(start=1.0,stop=5.0,num=5,name=None)  # 注意1.0和5.0
y = tf.range(start=1,limit=5,delta=1)
print(sess.run(x))
print(sess.run(y))
#===>[ 1.  2.  3.  4.  5.]
#===>[1 2 3 4]
```

### tf.assign

**assign(ref, value, validate_shape=None, use_locking=None, name=None)**

tf.assign是用来更新模型中变量的值的。ref是待赋值的变量，value是要更新的值。即效果等同于 `ref = value`

简单的实例代码见下

```
sess = tf.InteractiveSession()

a = tf.Variable(0.0)
b = tf.placeholder(dtype=tf.float32,shape=[])
op = tf.assign(a,b)

sess.run(tf.initialize_all_variables())
print(sess.run(a))
# 0.0
sess.run(op,feed_dict={b:5.})
print(sess.run(a))
# 5.0
```

# 4.规范化

### tf.variable_scope

简单的来讲，就是为变量添加命名域

```
with tf.variable_scope("foo"):
      with tf.variable_scope("bar"):
          v = tf.get_variable("v", [1])
          assert v.name == "foo/bar/v:0"
```

函数的定义为

```
def variable_scope(name_or_scope, reuse=None, initializer=None,
                   regularizer=None, caching_device=None, partitioner=None,
                   custom_getter=None):
```

各变量的含义如下：

name_or_scope: `string` or `VariableScope`: the scope to open.

reuse: `True` or `None`; if `True`, we go into reuse mode for this scope as well as all sub-scopes; if `None`, we just inherit the parent scope reuse. 如果reuse=True, 那么就是使用之前定义过的name_scope和其中的变量，

initializer: default initializer for variables within this scope.

regularizer: default regularizer for variables within this scope.

caching_device: default caching device for variables within this scope.

partitioner: default partitioner for variables within this scope.

custom_getter: default custom getter for variables within this scope.

### tf.get_variable_scope

返回当前变量的命名域，返回一个tensorflow.python.ops.variable_scope.VariableScope变量



## 基本概念

TensorFlow程序一般可以分为两个阶段，
- 构造部分，使用tensor表示数据，使用graph来表示计算任务
- 执行部分，在被称为Session的context里执行图中的计算（使用feed和fetch可以为任意的op赋值或从中获取数据）

### 计算图

TensorFlow中每一个计算都是计算图上的一个节点。TensorFlow会自动生成一个默认的计算图，如果没有特殊指定，运算会自动加入这个计算图中。

### 张量

在TensorFlow中，张量并没有真正保存数字，它只是对TensorFlow中运算结果的引用。一个张量中主要保存了三个属性：名字（name）、维度（shape）和类型（type）。张量的命名是通过`“node:src_output”`的形式来给出，`node`为节点的名称，`src_ouput`表示当前张量来自节点的第几个输出（编号从0开始）。当计算图构造完成后，可以通过会话（session）来得到张量的计算结果（或者借助eval()），如`tf.Session().run(result)`。

### 会话

TensorFlow中的会话（session）用来执行定义好的运算。TensorFlow不会自动生成默认的会话，需要手动指定。

### placeholder机制

TensorFlow提供了placeholder机制用于提供输入数据：placeholder相当于定义了一个位置，这个位置的数据在程序运行时再使用feed_dict来指定。

为什么会出现这个？若每轮迭代选取的数据都要通过常量来表示，计算图的节点就会非常多。（那为什么不适用variable呢？我现在还没想清楚）

## collection集合

```
tf.add_to_collection('losses', mse_loss)
loss = tf.get_collection('losses')
```

在一个计算图中，可以通过集合（collection）来管理不同类别的资源（可以是张量、变量或者运行Tensorflow程序所需要的队列资源等）。比如通过`tf.add_to_collection`函数可以将资源加入一个或多个集合中，然后通过`tf.get_collection`获取一个集合里面的所有资源。

TensorFlow中也自动管理了一些最常用的集合，如：tf.GraphKeys.TRAINABLE_VARIABLES是可学习变量的集合，可通过`tf.trainable_variables()`获得。

见《TensorFlow实战Google深度学习框架》P42。

## tf.constant

`a = tf.constant([1.0, 2.0], name='a')`
`tf.constant`是一个计算，这个计算的结果为一个张量，保存在变量a中。

张量主要的三个属性：名字（name）、维度（shape）和类型（type）。

见《TensorFlow实战Google深度学习框架》P43。

## tf.matmul

`a=tf.matmul(x, w1)`
`tf.matmul`实现了矩阵乘法的功能。

## tf.Variable

```
weights = tf.Variable(tf.random_normal([2, 3], stddev=2))
w2=tf.Variable(weights.initiallized_value())
```

变量`tf.Variable`的作用就是保存和更新神经网络中的参数，也支持通过其他变量的初始值来初始化新的变量。`tf.Variable`是一个类。

tensorflow随机数生成函数：`tf.random_normal`、`tf.truncated_normal`、`tf.random_uniform`、`tf.random_gamma`

tensorflow常数生成函数：`tf.zeros`、`tf.ones`、`tf.fill`、`tf.constant`

见《TensorFlow实战Google深度学习框架》P43。

## tf.get_variable() 和 tf.variable_scope()

和tf.Variable不同的一点是，`tf.get_variable` 必须包含一个指定变量名称的参数，它会根据这个名字去创建或者获取变量。如果需要通过`tf.get_variable`获取一个已经创建的变量，需要通过`tf.variable_scope`函数生成一个上下文管理器。具体说来，当`tf.variable_scope`函数使用参数reuse=True生成上下文管理器时，这个上下文管理器内所有的`tf.get_variable`函数会直接获取已经创建的变量，如果变量不存在则`tf.get_variable`函数将报错；相反，如果tf.variable_scope函数使用参数reuse=False生成上下文管理器时，`tf.get_variable`操作将创建新的变量，如果同名的变量已经存在，则`tf.get_variable`函数将报错。说起来有点绕，可以看下书籍《TensorFlow实战Google深度学习框架》5.3节 变量管理。

想一下，为什么这么做呢？答：这种变量管理的方式就不需要将所有变量都作为参数传递到不同的函数中了，并且大大提高程序的可读性。

除此之外，`tf.variable_scope`函数也会创建一个命名空间，在命名空间内创建的变量名称都会带上这个命名空间名作为前缀。`tf.name_scope`也会创建一个命名空间，但和前者的区别在于此函数将会自动忽略`tf.get_variable` 创建的变量。

参见：《TensorFlow实战Google深度学习框架》5.3节 变量管理 ；[What’s the difference of name scope and a variable scope in tensorflow?](http://stackoverflow.com/questions/35919020/whats-the-difference-of-name-scope-and-a-variable-scope-in-tensorflow)

## tf.assign

`assign(ref, value, validate_shape=None, use_locking=None, name=None)`

tf.assign用来更新模型中变量的值，效果等同于 ref = value。

## tf.train.ExponentialMovingAverage滑动平均

```
ema = tf.train.ExponentialMovingAverage(decay, num_updates)  # 定义了一个滑动平均类的对象，初始化时给定了衰减率deay和控制衰减率的变量num_updates
maintain_averages_op = ema.apply([v1])  #定义一个更新变量滑动平均的操作，每次执行这个操作时参数中的变量都会被更新
sess.run(maintain_averages_op)  # 更新v1的滑动平均值
v1_shadow = ema.average(v1) # 获取滑动平均之后变量的取值
```

滑动平均不会改变变量本身的取值，而是会维护一个影子变量来记录其滑动平均值。所以当需要使用这个滑动平均值时，需要明确调用average函数。

参见《TensorFlow实战Google深度学习框架》P90

## tf.contrib.layer.l2_regularizer 正则化

TensorFlow提供了`tf.contrib.layer.l2_regularizer` 函数，它可以返回一个函数，这个函数可以计算一个给定参数的L2正则化的值。
`loss_regular = tf.contrib.layer.l2_regularizer(lambda)(weights)  # lambda为正则化项的权重，weights为需要正则化的值`
参见《TensorFlow实战Google深度学习框架》P88

## tf.train.exponential_decay 指数衰减学习率

```
learning_rate = tf.train.exponential_decay(LEARNING_RATE_BASE, global_step, mnist.train.num_examples/BATCH_SIZE, LEARNING_RATE_DECAY)  # (基础学习率，当前迭代轮数，衰减速度（通常为过完一次所有训练数据需要的迭代次数），学习率衰减系数)
train_step = tf.train.GradientDescentOptimizer(learning_rate).minimize(my_loss, global_step=global_step) # 使用指数衰减的学习率。在minimize函数中传入global_step将自动更新global_step参数，从而使得学习率也得到相应更新。
```

衰减公式：learning_rate=learning_rate_base∗decay_rate(global_step/decay_steps)learning_rate=learning_rate_base∗decay_rate(global_step/decay_steps)

参见《TensorFlow实战Google深度学习框架》P85

## placeholder机制

为什么会出现这个？若每轮迭代选取的数据都要通过常量来表示，计算图的节点就会非常多。为了避免这个问题，TensorFlow提供了placeholder机制用于提供输入数据：placeholder相当于定义了一个位置，这个位置的数据在程序运行时再使用feed_dict来指定。

```
x = tf.placeholder(tf.float32, shape=(3, 2), name="input")
sess.run(y, feed_dict={x:[[0.7, 0.9], [0.1, 0.4], [0.5, 0.8]]})
```

## tf.control_dependencies控制依赖

```
with g.control_dependencies([a, b, c]): 
# `d` 和 `e` 将在 `a`, `b`, 和`c`执行完之后运行 
d = … 
e = …
```

在训练神经网络模型时，每过一遍数据既要通过反向传播来更新神经网络中的参数，又要更新每一个参数的滑动平均值。为了一次完成多个操作，TensorFlow提供了tf.control_dependencies和tf.group两种机制。下面两行程序和`train_op = tf.group(train_step, variables_averages_op)`是等价的。

```
with tf.control_dependencies([train_step, variables_averages_op]):
    train_op = tf.no_op(name='train')
```

## tf.nn.sparse_softmax_cross_entropy_with_logits交叉熵函数
`cross_entropy = tf.nn.sparse_softmax_cross_entropy_with_logits(y, tf.argmax(y_, 1))`
当分类问题只有一个正确答案时，可以使用这个函数来加速交叉熵的计算。这个函数的第一个参数时神经网络不包括softmax层的前向传播结果，第二个是训练数据的正确答案。因为标准答案是一个长度为10的一维数组，而该函数需要提供的是一个正确答案的数字，所以需要使用tf.argmax函数来得到正确答案对应的类别编号。

## tf.train.GradientDescentOptimizer优化算法
`train_step = tf.train.GradientDescentOptimizer(learning_rate).minimize(loss, global_step=global_step)`- 1

使用其来优化损失函数，注意在这个函数中会把`global_step`的值加1。

## tf.train.Saver()保存加载模型

保存模型：

```
saver = tf.train.Saver()
saver.save(sess, path_to_save, global_step=global_step)
```

通过变量重命名的方式加载部分模型：

```
variable_averages = tf.train.ExponentialMovingAverage(mnist_train.MOVING_AVERAGE_DECAY) 
variables_to_restore = variable_averages.variables_to_restore() # Returns a map of names to Variables to restore
saver = tf.train.Saver(variables_to_restore) #参数可以是一个dict(即这里的用法)也可以是一个list
saver.restore(sess, path_saved)
```

参考：[变量:创建、初始化、保存和加载](http://wiki.jikexueyuan.com/project/tensorflow-zh/how_tos/variables.html)；[variables_to_restore(moving_avg_variables=None)](https://www.tensorflow.org/api_docs/python/tf/train/ExponentialMovingAverage#variables_to_restore)

## tf.gfile.FastGFile

一个文件读写的封装类。

tf.gfile.FastGFile(image_path, ‘rb’).read() # Returns the contents of a file as a string.

参考：[tf.gfile.FastGFile](https://www.tensorflow.org/api_docs/python/tf/gfile/FastGFile#read)

## tf.argmax

返回沿着某个维度最大值的位置。

```
a = np.arange(6).reshape(2,3)
a.argmax()
#输出：5
a.argmax(0)
#输出：array([1, 1, 1])
a.argmax(1)
#输出：array([2, 2])
```

## GPU配置

```
gpu_options = tf.GPUOptions(visible_device_list="0,1", allow_growth=True) # 指定使用的GPU设备号；允许运行过程中tf根据需求增长扩大显存的使用
gpu_options = tf.GPUOptions(visible_device_list="0", per_process_gpu_memory_fraction=0.4)    # 指定使用的GPU设备号；指定每个可用GPU上的显存分配比率
session_config = tf.ConfigProto(gpu_options=gpu_options, allow_soft_placement=True, log_device_placement=True)    # gpu配置；如果指定的设备不存在允许tf自动分配设备；打印设备分配日志
sess = tf.Session(config=session_config)
```

转载自：

--------------------- 作者：NodYoung 来源：CSDN 原文：https://blog.csdn.net/NNNNNNNNNNNNY/article/details/70177509?utm_source=copy 

--------------------- 作者：multiangle 来源：CSDN 原文：https://blog.csdn.net/u014595019/article/details/52805444?utm_source=copy






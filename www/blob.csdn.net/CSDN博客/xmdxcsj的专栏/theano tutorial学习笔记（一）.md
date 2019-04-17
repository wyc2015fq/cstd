# theano tutorial学习笔记（一） - xmdxcsj的专栏 - CSDN博客





2015年10月06日 21:48:09[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1540








# algebra

## 标量求和

**>>> importtheano.tensorasT****>>> fromtheanoimport** function**>>> **x = T.dscalar('x')**>>> **y = T.dscalar('y')**>>> **z = x + y**>>> **f = function([x, y], z)

x不是dscalar的实例，而是TensorVariable的实例。x的type字段是dscalar

## 矩阵求和

**>>> **x = T.dmatrix('x')**>>> **y = T.dmatrix('y')**>>> **z = x + y**>>> **f = function([x, y], z)

## logistic function


**>>> importtheano**

**>>> importtheano.tensorasT**

**>>> **x 
= T.dmatrix('x')

**>>> **s 
=1/ (1+ T.exp(-x))

**>>> **logistic 
= theano.function([x], s)

**>>> **logistic([[0,1], [-1,-2]])

array([[ 0.5       , 0.73105858],

      [ 0.26894142,  0.11920292]])


## 设置默认参数

**>>> fromtheanoimport** Param**>>> fromtheanoimport** function**>>> **x, y = T.dscalars('x', 'y')**>>> **z = x + y**>>> **f = function([x, Param(y, default=1)], z)**>>> **f(33)array(34.0)



**>>> **x, y, w = T.dscalars('x', 'y', 'w')**>>> **z = (x + y) * w**>>> **f = function([x, Param(y, default=1), Param(w, default=2, name='w_by_name')], z)**>>> **f(33)

## 使用sharedvariables

**>>> fromtheanoimport** shared**>>> **state = shared(0)**>>> **inc = T.iscalar('inc')**>>> **accumulator = function([inc], state, updates=[(state, state+inc)])

Shared变量类似于static变量；updates参数形式 (shared-variable,new expression)，每次使用新表达式的值代替原来的shared变量。

given参数设置指定参数的值，shared变量的值不受影响

## Using random numbers

**fromtheano.tensor.shared_randomstreamsimport** RandomStreams**fromtheanoimport** functionsrng = RandomStreams(seed=234)rv_u = srng.uniform((2,2))rv_n = srng.normal((2,2))f = function([], rv_u)g = function([], rv_n, no_default_updates=True)    *#Not updating rv_n.rng*nearly_zeros = function([], rv_u + rv_u -2* rv_u)

uniform表示均匀分布，normal表示正态分布，no_default_updates表示每次调用g输出的结果一样。

# Theano Graphs

Theano的图结构包括三种节点：variable、op和apply

**importtheano.tensorasT**x = T.dmatrix('x')y = T.dmatrix('y')z = x + y

![](https://img-blog.csdn.net/20151006214611090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**>>> importtheano****>>> **x = theano.tensor.dmatrix('x')**>>> **y = x *2.**>>> **y.owner.op.name'Elemwise{mul,no_inplace}'**>>> **y.owner.inputs[0]x**>>> **y.owner.inputs[1]DimShuffle{x,x}.0

使用图结构，theano可以对表达式进行优化，比如减少重复计算

使用theano.printing.pydotprint可以画出表达式的图结构。

## Printing theano

theano.printing.pprinttheano.printing.debugprinttheano.printing.pydotprint

## Derivatives

### 计算梯度

**>>> **x = T.dscalar('x')**>>> **y = x **2**>>> **gy = T.grad(y, x)

打印求导以后的表达式

**>>> **pp(gy)  *# print out the gradient prior to optimization*

pp(f.maker.fgraph.outputs[0])



### 计算雅克比

**>>> importtheano****>>> importtheano.tensorasT****>>> **x = T.dvector('x')**>>> **y = x **2**>>> **J, updates = theano.scan(**lambda** i, y,x : T.grad(y[i], x), sequences=T.arange(y.shape[0]), non_sequences=[y,x])**>>> **f = theano.function([x], J, updates=updates)**>>> **f([4, 4])array([[ 8.,  0.],       [ 0.,  8.]])

### 计算Hessian矩阵

**>>> **x = T.dvector('x')**>>> **y = x **2**>>> **cost = y.sum()**>>> **gy = T.grad(cost, x)**>>> **H, updates = theano.scan(**lambda** i, gy,x : T.grad(gy[i], x), sequences=T.arange(gy.shape[0]), non_sequences=[gy, x])**>>> **f = theano.function([x], H, updates=updates)**>>> **f([4, 4])array([[ 2.,  0.],       [ 0.,  2.]])

# Configuration and modes

## Configuaration

有三种方式修改theano的配置，优先顺序如下：

1.      an assignment totheano.config.<property>

2.      an assignment in [THEANO_FLAGS](http://deeplearning.net/software/theano/library/config.html#envvar-THEANO_FLAGS)

3.      an assignment in the .theanorcfile (or the file indicated in [THEANORC](http://deeplearning.net/software/theano/library/config.html#envvar-THEANORC))

## modes

function函数的图结构优化有四种模式可以选择
|**short name**|**What does it do?**|
|----|----|
|FAST_COMPILE|Python implementations only, quick and cheap graph transformations|
|FAST_RUN|C implementations where available, all available graph transformations.|
|DebugMode|Both implementations where available, all available graph transformations.|
|ProfileMode|Deprecated. C implementations where available, all available graph transformations, print profile information.|

## Linker

使用config.linker控制使用的连接器
|linker|gc [[1]](http://deeplearning.net/software/theano/tutorial/modes.html#gc)|Raise error by op|Overhead|Definition|
|----|----|----|----|----|
|cvm|yes|yes|“++”|As c|py, but the runtime algo to execute the code is in c|
|cvm_nogc|no|yes|“+”|As cvm, but without gc|
|c|py [[2]](http://deeplearning.net/software/theano/tutorial/modes.html#cpy1)|yes|yes|“+++”|Try C code. If none exists for an op, use Python|
|c|py_nogc|no|yes|“++”|As c|py, but without gc|
|c|no|yes|“+”|Use only C code (if none available for an op, raise an error)|
|py|yes|yes|“+++”|Use only Python code|
|ProfileMode|no|no|“++++”|(Deprecated) Compute some extra profiling info|
|DebugMode|no|yes|VERY HIGH|Make many checks on what Theano computes|

## Debugmode

x = T.dvector('x')f = theano.function([x], 10* x, mode='DebugMode')f([5])f([0])f([7])

运行速度特别慢。

## Profilemode

创建instance

**>>> fromtheanoimport** ProfileMode**>>> **profmode = theano.ProfileMode(optimizer='fast_run', linker=theano.gof.OpWiseCLinker())

编译graph

**>>> **v1, v2 = T.vectors(2)**>>> **o = v1 + v2**>>> **f = theano.function([v1,v2],[o], mode=profmode)

Profile

profmode.print_summary()

# loading and saving

使用pickle模块进行序列化和反序列化，cPickle基于c代码，比pickle模块更快

序列化

**>>> **f =file('objects.save', 'wb')**>>> for** obj **in** [obj1, obj2, obj3]:**... **    cPickle.dump(obj, f, protocol=cPickle.HIGHEST_PROTOCOL)**>>> **f.close()

反序列化

**>>> **f =file('objects.save', 'rb')**>>> **loaded_objects = []**>>> for** i **in**range(3):**... **    loaded_objects.append(cPickle.load(f))**>>> **f.close()

### Short-term序列化

可以控制对象的哪些部分序列化

**def**__getstate__(self):    state =dict(self.__dict__)**del** state['training_set']**return** state**def**__setstate__(self, d):self.__dict__.update(d)self.training_set = cPickle.load(file(self.training_set_file, 'rb'))

## Robust序列化

来自于ndarray和CudaNdarray的对象将会被单独保存为NPY文件。反序列化不需要theano。

## Long-term序列化

序列化的对象不稳定的时候，比如成员重命名或者函数创建删除，自定义需要保存的属性。

**def**__getstate__(self):**return** (self.W, self.b)**def**__setstate__(self, state):    W, b = stateself.W = Wself.b = b

# Conditions

**fromtheanoimport** tensor **as** T**fromtheano.ifelseimport** ifelse**importtheano**,**time**,**numpy**a,b = T.scalars('a', 'b')x,y = T.matrices('x', 'y')z_switch = T.switch(T.lt(a, b), T.mean(x), T.mean(y))z_lazy = ifelse(T.lt(a, b), T.mean(x), T.mean(y))

switch是elementsize，相比ifelse应用多一些

switch计算所有的变量，ifesle属于lazy的运算

# loop

使用scan

# Sparse

Theano使用的稀疏矩阵有两种：csc和csr。csc是基于列的（shape[0]<shape[1]的时候使用），csr是基于行的。可以利用更高效的算法来进行线性代数计算，缺点是不能更改矩阵的结构（比如添加元素）。

## 成员

data：一维数组，包含所有的非零元素

indices和indptr：存储矩阵中数据的位置信息

shape：跟dense矩阵的shape含义一样

稀疏矩阵无法直接获取成员，需要使用csm_properties函数获取，可以根据获得的成员对稀疏矩阵进行重构。

**>>> **x = sparse.csc_matrix(name='x', dtype='int64')**>>> **data, indices, indptr, shape = sparse.csm_properties(x)**>>> **y = sparse.CSR(data, indices, indptr, shape)**>>> **f = theano.function([x], y)**>>> **a = sp.csc_matrix(np.asarray([[0, 1, 1], [0, 0, 0], [1, 0, 0]]))**>>> print** a.toarray()[[0 1 1] [0 0 0] [1 0 0]]**>>> print** f(a).toarray()[[0 0 1] [1 0 0] [1 0 0]

## 运算

稀疏矩阵使用运算称为structured（对于矩阵中的0元素，不做任何计算）。

**>>> **x = sparse.csc_matrix(name='x', dtype='float32')**>>> **y = sparse.structured_add(x, 2)**>>> **f = theano.function([x], y)**>>> **a = sp.csc_matrix(np.asarray([[0, 0, -1], [0, -2, 1], [3, 0, 0]], dtype='float32'))**>>> print** a.toarray()[[ 0.  0. -1.] [ 0. -2.  1.] [ 3.  0.  0.]]**>>> print** f(a).toarray()[[ 0.  0.  1.] [ 0.  0.  3.] [ 5.  0.  0.]]






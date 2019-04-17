# CS 20SI|Lecture 2  TensorFlow Ops - 浅梦的博客 - CSDN博客





2017年02月03日 01:09:25[浅梦s](https://me.csdn.net/u012151283)阅读数：923








# Agenda
- Basic operations
- Tensor types
- Project speed dating
- Placeholders and feeding inputs
- Lazy loading

# Using TensorBoard

## 使用TensorBoard的5个步骤

```
#1. 定义需要记录的tensor
w2_hist = tf.summary.histogram("weights2",W2)
cost_sum = tf.summary.scalar("cost",cost)
#2. 合并所有记录
summary = tf.summary.merge_all()
#3. 创建writer并添加图
wirter = tf.summary.FileWriter('./logs')
writer.add_graph(sess.graph)
#4. 运行汇总op，添加结果
s,_ = sess.run([summary,optimizer],feed_dict=feed_dict)
writer.add_summary(s,global_step=global_step)
```

## 本地启动

`tensorboard --logdir=./logs`

## 远端启动

```bash
ssh -L local_port:127.0.0.1:remote_port username@server.com

local> $ ssh -L 7007:121.0.0.0:6006 test@server.com
server> $ tensorboard -logdir=./logs
```

在构建完计算图之后开始训练之前加入以下代码， 
`writer ​=​ tf​.​summary​.​FileWriter​(​logs_dir​,​ sess​.​graph)`

```python
import tensorflow as tf
a = tf.constant(2)
b = tf.constant(3)
x = tf.add(a, b)
with tf.Session() as sess:
    # add this line to use TensorBoard. 
    # Create the summary writer after graph definition and before running your session
    writer = tf.summary.FileWriter('./graphs', sess.graph) # Where you want to keep your event files
    print(sess.run(x))
writer.close() # close the writer when you’re done using it
```

在当前目录下打开终端控制台中输入

```
$ python [filename].py
$ tensorboard --logdir="./graphs" --port 6006
```

打开浏览器 [http://localhost:6006/](http://localhost:6006/)查看tensorboard 
![这里写图片描述](https://img-blog.csdn.net/20170203010634607?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

显式命名变量
```python
import tensorflow as tf
a = tf.constant(2, name="a")
b = tf.constant(3, name="b")
x = tf.add(a, b, name="add")
writer = tf.summary.FileWriter("./graphs", sess.graph)
with tf.Session() as sess:
print sess.run(x) # >> 5
```

# 常数 More constants


```
tf.constant(value, dtype=None, shape=None, name='Const', verify_shape=False) 

 

类似`numpy.array`创建多维数组一样
```python
# constant of 1d tensor (vector) 
a ​=​ tf​.​constant​([​2​,​ ​2​],​ name​=​"vector") 
# constant of 2x2 tensor (matrix) 
b ​=​ tf​.​constant​([[​0​,​ ​1​],​ ​[​2​,​ ​3​]],​ name​=​"b")
```

## Tensors filled with a specific value

类似于`numpy.zeros,numpy.zeros_like,numpy.ones,numpy.ones_like`创建特定值的`tensor`

```python
tf​.​zeros​(​shape​,​ dtype​=​tf​.​float32​,​ name​=​None) 
tf​.​zeros_like​(​input_tensor​,​ dtype​=​None​,​ name​=​None​,​ optimize​=​True)
tf​.​ones​(​shape​,​ dtype​=​tf​.​float32​,​ name​=​None) 
tf​.​ones_like​(​input_tensor​,​ dtype​=​None​,​ name​=​None​,​ optimize​=​True) 
tf​.​fill​(​dims​,​ value​,​ name​=​None​)
```

## Constants as sequences

`tf​.​linspace​(​start​,​ stop​,​ num​,​ name​=​None)`

创建一个区间范围为[start,stop]间隔为(stop-start)/(num -1)的数字序列 
`tf​.​range​(​start​,​ limit​=​None​,​ delta​=​1​,​ dtype​=​None​,​ name​=​'range')`

创建一个区间范围为[start,limit)间隔为delta的数字序列 

与numpy序列不同的是,**tensorflow的序列是不可迭代对象**
```python
for​ _ ​in​ np​.​linspace​(​0​,​ ​10​,​ ​4​):​ ​# OK 
for​ _ ​in​ tf​.​linspace​(​0​,​ ​10​,​ ​4​):​ ​# TypeError("'Tensor' object is not iterable.") 
for​ _ ​in​ range​(​4​):​ ​# OK 
for​ _ ​in​ tf​.​range​(​4​):​ ​# TypeError("'Tensor' object is not iterable.")
```

## Randomly Generated Constants

设置随机数种子`tf.set_random_seed(seed)`

```python
tf.random_normal(shape, mean=0.0, stddev=1.0, dtype=tf.float32, seed=None, name=None) 
tf.truncated_normal(shape, mean=0.0, stddev=1.0, dtype=tf.float32, seed=None, name=None) tf.random_uniform(shape, minval=0, maxval=None, dtype=tf.float32, seed=None, name=None) 
tf.random_shuffle(value, seed=None, name=None) 
tf.random_crop(value, size, seed=None, name=None)
tf.multinomial(logits, num_samples, seed=None, name=None)
tf.random_gamma(shape, alpha, beta=None, dtype=tf.float32, seed=None, name=None)
```

# 数学运算 Math Operations

TensorFlow 数学运算类似于Numpy的操作。官方API链接文档 [https://www.tensorflow.org/api_guides/python/math_ops#Arithmetic_Operators](https://www.tensorflow.org/api_guides/python/math_ops#Arithmetic_Operators)

> 
图片来自于Fundamental of Deep Learning 
![这里写图片描述](https://img-blog.csdn.net/20170204174249290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 归约 Reduction

规约可对k维的张量的指定维度进行操作从而达到降维的目的。 
[https://www.tensorflow.org/api_guides/python/math_ops#Reduction](https://www.tensorflow.org/api_guides/python/math_ops#Reduction)

以最常用的`tf.reduce_sum`为例，其API如下
```python
reduce_sum(
    input_tensor,
    axis=None,
    keep_dims=False,
    name=None,
    reduction_indices=None
)
```

官网给了如下例子

```python
# 'x' is [[1, 1, 1]
#         [1, 1, 1]]
tf.reduce_sum(x) ==> 6
tf.reduce_sum(x, 0) ==> [2, 2, 2]
tf.reduce_sum(x, 1) ==> [3, 3]
tf.reduce_sum(x, 1, keep_dims=True) ==> [[3], [3]]
tf.reduce_sum(x, [0, 1]) ==> 6
```

简单来说，当`axis=None`时，操作会沿着所有维度进行。 

当`axis=0`时，操作沿着0轴进行，对于2维的张量，竖直方向为0轴，水平方向为1轴。 

当`keep_dims=True`，返回的维度和原来保持一致（通常归约操作是导致维度减少的）。 

当`axis=[0,1]`时，表明规约操作沿着多个轴进行。 

对于k维的张量，我们只要记住最里面那一层的元素所在维度为`k-1`，相应地，往外的维度继续减1. 

现在考虑一个三维3*2*2的张量，对应k=3，现在对他进行一些规约操作
```python
a = [[[ 1,  2],
        [ 3,  4]],

       [[ 5,  6],
        [ 7,  8]],

       [[ 9, 10],
        [11, 12]]]
tf.reduce_sum(x,2,keep_dims=True)#这里axis=k-1，那么是对最里面一层的元素进行规约操作，最里面一层的维度对应的长度应该变为1,也就是说结果应该是一个3*2*1的张量ans_1,如果不保持维度的话，就会变成3*2的张量ans_2
ans_1 =[[ [3],  [7]],
       [[11], [15]],
       [[19], [23]]]
ans_2 = [[ 3,  7],
       [11, 15],
       [19, 23]]
tf.reduce_sum(x, 2) #axis=k-2,那么是对倒数第二层的元素进行规约求和，分别对应于1+3,2+4,6+7...结果如下
ans = [[ 4,  6],
       [12, 14],
       [20, 22]]
tf.reduce_sum(x, 0) #axis = k-3，这是对倒数第三层元素进行求和，分别对应于1+5+9，2+6+10，。。。结果如下
ans = [[15, 18],
       [21, 24]]
```

**最后，对于k维的形状为(k1,k2,ki,…,kn)的张量，若规约操作沿着ki进行，那么得到结果形状为(k1,k2,1,…,kn)，将长度为1的维度删去就达到了规约降低张量维度的目的。**

# 数据类型 Data Types

下图是TensorFlow的数据类型 
![这里写图片描述](https://img-blog.csdn.net/20170204174813235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 重要说明
- TF的数据类型和Numpy之间是可以相互转换的，如`tf.int32 == np.int32`该表达式的值为True
- 当执行`tf.Session.run(fetchs)`时，如果请求的fetches是`tensor`，那么该函数的返回值将是一个Numpy的`ndarray`
- 尽量不要使用Python原生类型的数据，如Python的整数类型只有`int`而TF分为`tf.int8`,`tf.int16`,`tf.int32`等，对于原生类型的Python数据结构，TF需要进行类型推断，影响效率
- 虽然Numpy数组和TF的tensor之间可以相互转换，但是Numpy并不提供创建tensor的函数，也不支持自动求导和GPU计算。**所以尽量使用TensorFlow的数据类型**。
- TensorFlow的`constants`数存储在图定义中的，这意味着当`constants`很大的时，每次加载计算图都会很慢，也会消耗大量的内存。 
**Print out the graph def**

```python
import tensorflow as tf
my_const = tf.constant([1.0, 2.0], name="my_const")
with tf.Session() as sess:
print sess.graph.as_graph_def()
# you will see value of my_const stored in the graph’s definition
```

只有对于基本类型使用`constants`，对于需要大量内存的数据尽量使用`Variable`或`readers`

# 变量 Variables

使用`tf.Variable(initial_value=None, validate_shape=True, name=None,  dtype=None, )`创建变量 

注意不是`tf.variable`因为`tf.Varible`是一个类，而之前的`tf.constant`是一个op。 
`tf.Variable`包含了若干ops:
```python
x = tf.Variable(...)  
x.initializer # init  x.value() # read op  x.assign(...) # write op  x.assign_add(...)  
# and more
```

## 变量初始化

声明好变量后，在使用之前需要对变量进行初始化操作 

1.全局变量初始化 `tf.global_variables_initializer()`

```python
init ​=​ tf​.​global_variables_initializer​() 
with​ tf​.​Session​()​ ​as​ sess: 
    tf​.​run​(​init)
```

2.初始化指定变量 `tf.variables_initializer()`

```python
init_ab ​=​ tf​.​variables_initializer​([​a​,​ b​],​ name​=​"init_ab") 
with​ tf​.​Session​()​ ​as​ sess: 
    tf​.​run​(​init_ab)
```

3.变量单独初始化`tf.Variable.initializer`

```python
# create variable W as 784 x 10 tensor, filled with zeros W ​=​ tf​.​Variable​(​tf​.​zeros​([​784​,​10​])) with​ tf​.​Session​()​ ​as​ sess: tf​.​run​(​W​.​initializer)
```

4.restore from a save file

## 变量求值

变量初始化后，直接进行print会得到如下的结果

```python
W ​=​ tf.Variable(tf.ones([2,2],)) 
with​ tf​.​Session​()​ ​as​ sess: 
    sess​.​run​(​W​.​initializer) 
    print​(W) 
>>​ ​Tensor​(​"Variable/read:0"​,​ shape=(2, 2), dtype=float32)
```

要想获得变量的值，对变量调用`eval()`函数

```python
W ​=​ tf.Variable(tf.ones([2,2],))  
with​ tf​.​Session​()​ ​as​ sess: 
    sess​.​run​(​W​.​initializer) 
    print​(W​.​eval​())
>> [[ 1.  1.]
 [ 1.  1.]]
```

## 变量赋值

使用`tf.Variable.assign()`函数为变量赋值。

```python
W ​=​ tf​.​Variable​(​10​) 
W​.​assign​(​100​) 
with​ tf​.​Session​()​ ​as​ sess: 
    sess​.​run​(​W​.​initializer) 
    print​ (W​.​eval​())
>> 10
```

上面这段代码并不会输入我们赋值的变量10，这是为什么呢？ 

注意到，在Lecture1中提到过，所有的op都要在`Session`中被执行，而上面我们虽然声明了一个`assign`op，但是该op并没有被真正的执行，所以变量的值仍然是10。做如下的修改：

```python
W ​=​ tf​.​Variable​(​10​) 
assign_op = W​.​assign​(​100​) 
with​ tf​.​Session​()​ ​as​ sess: 
    sess​.​run​(​assign_op) 
    print​ (W​.​eval​()) 
>> 100
```

注意到，上述代码中并没有执行初始化的op,这是因为`assign`op进行了初始化的操作，事实上，`initializer` op执行的操作就是将初始值`assign`给变量。

### assign_add和assign_sub

使用`tf.Variable.assign_add()`和 `tf.Variable.assign_sub()`可以对变量进行增加或减少指定数值，但是需要进行初始化。

```python
W = tf.Variable(10) 
with tf.Session() as sess:     
    sess.run(W.initializer) 
    print (sess.run(W.assign_add(10))) # >> 20 
    print (sess.run(W.assign_sub(2))) # >> 18
```

### 不同的Session会维护同一个变量的不同副本

```python
W = tf.Variable(10) 
sess1 = tf.Session() 
sess2 = tf.Session() 
sess1.run(W.initializer) 
sess2.run(W.initializer) 

print (sess1.run(W.assign_add(10))) # >> 20 
print (sess2.run(W.assign_sub(2))) # >> 8 

print (sess1.run(W.assign_add(100))) # >> 120 print (sess2.run(W.assign_sub(50))) # >> -42 

sess1.close() 
sess2.close()
```

# 交互式会话 InteractiveSession

使用`InteractiveSession`代替`Session`，`InteractiveSession`将自身设置为默认的`Session`，这样就可以直接调用`run()`和`eval()`等方法而不需要显式地指定一个`Session`。这在交互式编程环境中用起来会很方便，但是如果你有多个`Session`需要运行的话，就不太方便了。

```python
sess ​=​ tf​.​InteractiveSession​() 
a ​=​ tf​.​constant​(​5.0) 
b ​=​ tf​.​constant​(​6.0) 
c ​=​ a ​*​ b # We can just use 'c.eval()' without passing 'sess' 
print​(​c​.​eval​()) 
sess​.​close​()
```

`tf.InteractiveSession.close()`会关闭一个 InteractiveSession. 

`tf.get_default_session()` 返回当前线程的默认`session`。返回的`session`将称为`Session`或`Session.as_default()`上下文进入的`session`。

# 控制依赖性 Control Dependencies

当我们有若干个互相独立的op a,b,c,d时，若想要让ab先执行，de后执行，可以使用`tf.Graph.control_dependencies(control_inputs)`方法

```python
# your graph g have 5 ops: a, b, c, d, 
with​ g​.​control_dependencies​([​a​,​ b​,​​]):   ​# `c` and `d` will only run after `a`, `b` have executed.   
c ​=​ ​...   
d ​=​ …
```

# 占位符 Placeholders and feed_dict

事实上，我们可以先定义出计算图的结构，而不具体提供需要的数据，等到需要真正执行计算的时候，再将数据传入。可以先定义一个承载数据的占位符 
`tf​.​placeholder​(​dtype​,​ shape​=​None​,​ name​=​None)`

我们需要指定数据类型`dtype`，对于`shape`来说，尽量清晰的指定出来，这在之后的debug中会比较方便。 

在需要真实数据进行计算时，将数据作`feed_dict`的参数传递到`sess.run()`方法中。 

我们还可以喂给非`placeholder`的`tensor`数据，只要`tensor`是`feedable`的。通过`tf.Graph.is_feedable(tensor)`检查tensor的该性质。 

在测试模型时，feed_dict会很方便，我们可以直接向某个中间步喂给数据，而不需要进行所有的的计算。
# The trap of lazy loading

大概的意思是尽量在session外就定义好各种需要的op,不要在run中再写op，否则有可能造成计算图消耗不必要的资源。
```






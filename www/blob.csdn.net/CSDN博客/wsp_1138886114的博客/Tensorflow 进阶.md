# Tensorflow 进阶 - wsp_1138886114的博客 - CSDN博客





2018年06月14日 10:59:24[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：163










- - [TensorFlow 代码入门](#tensorflow-代码入门)- [一、 graph 与 session](#一-graph-与-session)- - - [举例 定义一个数据流图：](#举例-定义一个数据流图)



- [二、Tensor（rank,shape,data types）](#二tensorrankshapedata-types)- - - [2.1 rank](#21-rank)
- [2.2 shape](#22-shape)
- [2.3 data type（数据类型）](#23-data-type数据类型)



- [三、Variables](#三variables)- - [3.1 初识  Variables](#31-初识-variables)
- [3.2  共享模型变量](#32-共享模型变量)
- [3.3 Variables 与constant的区别](#33-variables-与constant的区别)
- [3.4 variables 与 get_variables 的区别](#34-variables-与-getvariables-的区别)
- [3.5 Variables常用运算](#35-variables常用运算)


- [四、  placeholders 与 feed_dict](#四-placeholders-与-feeddict)
- [五、constant 、variable及 placeholder的异同](#五constant-variable及-placeholder的异同)- - [5.1 constant](#51-constant)
- [5.2 variable](#52-variable)
- [5.3 placeholder](#53-placeholder)


- [六、常用概念](#六常用概念)






## TensorFlow 代码入门

```
编写 tensorflow 可以总结为两步：
     （1）组装一个graph;
     （2）使用session去执行graph中的operation。
```

### 一、 graph 与 session

Tensorflow 是基于计算图的框架，因此理解 graph 与 session 显得尤为重要。不过在讲解 graph 与 session 之前首先介绍下什么是计算图。假设我们有这样一个需要计算的表达式。该表达式包括了两个加法与一个乘法，为了更好讲述引入中间变量c与d。由此该表达式可以表示为： 
![这里写图片描述](https://img-blog.csdn.net/20180613104136410?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

当需要计算e时就需要计算c与d，而计算c就需要依赖输入值a与b，计算d需要依赖输入值a与b。这样就形成了依赖关系。这种有向无环图就叫做计算图，因为对于图中的每一个节点其微分都很容易得出，因此应用链式法则求得一个复杂的表达式的导数就成为可能，所以它会应用在类似tensorflow这种需要应用反向传播算法的框架中。
```
graph , session , operation , tensor 四个概念的简介。
    1 Tensor：   类型化的多维数组，图的边；边（edge）对应于向操作（operation）传入和从operation
                 传出的实际数值，通常以箭头表示。
    2 Operation：执行计算的单元，图的节点，节点（node）通常以圆圈、椭圆和方框等来表示，代表了对数据
                 所做的运算或某种操作，在上图中，“add”、“mul”为运算节点。
    3 Graph：    一张有边与点的图，其表示了需要进行计算的任务；
    4 Session:   称之为会话的上下文，用于执行图。
```

###### 举例 定义一个数据流图：

```
import  tensorflow as tf                       # 导入tensorflow
graph  = tf.Graph()                            # 定义一个graph类
with  graph.as_default():
    a = tf.Variable(3,name='input_a')
    b = tf.Variable(5,name='input_b')
    d = tf.add(a,b,name='add_d')
    initialize =  tf.global_variables_initializer()
    print(d)
>>>
ensor("add_d:0", shape=(), dtype=int32)
```

### 二、Tensor（rank,shape,data types）

###### 2.1 rank

```
张量(Tensor)为n维矩阵，而Rank一般是指张量的维度：
    0D（0维）张量称为标量；
    1D（1维）张量等价于向量；
    2D（2维）等价于矩阵；
对于更高维数的张量，可称为N维张量或N阶张量。对示例数据流图进行修改，使其变为使用张量：
```

![这里写图片描述](https://img-blog.csdn.net/20180613111719366?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改之前的代码。将分离的节点a、b替换为统一的输入节点a，而a为1D张量。当然运算符需要调整，由add改为reduce_sum,具体如下：
```
a= tf.Variable([5,3],name='input_a')
c=tf.reduce_sum(a,name='sum_c')
```

###### 2.2 shape

Shape指tensor每个维度数据的个数，可以用python的list/tuple表示。 
`Rank 与 shape`的对应关系：
|**Rank**|**Shape**|**Dim**|**Example**|
|----|----|----|----|
|0|[ ]|0-D|一个 0维张量. 一个纯量.|
|1|[D0]|1-D|一个1维张量的形式[5].|
|2|[D0, D1]|2-D|一个2维张量的形式[3, 4].|
|3|[D0, D1, D2]|3-D|一个3维张量的形式 [1, 4, 3].|
|n|[D0, D1, … Dn]|n-D|一个n维张量的形式 [D0, D1, …   Dn].|

###### 2.3 data type（数据类型）
|数据类型|Python 类型|描述|
|----|----|----|
|DT_FLOAT|tf.float32|32 位浮点数.|
|DT_DOUBLE|tf.float64|64 位浮点数.|
|DT_INT64|tf.int64|64 位有符号整型.|
|DT_INT32|tf.int32|32 位有符号整型.|
|DT_INT16|tf.int16|16 位有符号整型.|
|DT_INT8|tf.int8|8 位有符号整型.|
|DT_UINT8|tf.uint8|8 位无符号整型.|
|DT_STRING|tf.string|可变长度的字节数组.每一个张量元素都是一个字节数组.|
|DT_BOOL|tf.bool|布尔型.|
|DT_COMPLEX64|tf.complex64|由两个32位浮点数组成的复数:实数和虚数.|
|DT_QINT32|tf.qint32|用于量化Ops的32位有符号整型.|
|DT_QINT8|tf.qint8|用于量化Ops的8位有符号整型.|
|DT_QUINT8|tf.quint8|用于量化Ops的8位无符号整型.|

### 三、Variables

##### 3.1 初识  Variables

```
流程为：
--▶创建变量 --▶初始化变量 --▶保存变量 --▶恢复变量 --▶共享变量。

当训练模型后的参数拥有Variables的指向就不会在session中丢失。
Variables可以通过Variables初始化。
tf.random_normal：   随机生成一个正态分布的tensor，
stddev：             标准差。
tf.zeros：           生成一个全零的tensor。
将这个tensor的值赋值给Variable。
```

```
◆ 1 创建模型的权重及偏置
import tensorflow as tf

weights = tf.Variable(tf.random_normal([784, 200], stddev=0.35), name="weights")
biases = tf.Variable(tf.zeros([200]), name="biases")
```

```
◆ 2 初始化变量
实际在其初始化过程中做了很多的操作，比如初始化空间，赋初值（等价于tf.assign），
并把 Variable 添加到 graph 中等操作。注意在计算前需要初始化所有的Variable。
一般会在定义 graph 时定义global_variables_initializer，其会在session运算时初始化所有变量。
直接调用 global_variables_initializer 会初始化所有的Variable，如果仅想初始化部分Variable
可以调用tf.variables_initializer。 

init_op= tf.global_variables_initializer()
sess=tf.Session()
sess.run(init_op)
```

```
◆ 3 保存模型变量
保存模型由三个文件组成 model.data， model.index， model.meta

saver = tf.train.Saver()
saver.save(sess, './tmp/model/',global_step=100)
```

```bash
◆ 4 恢复模型变量
先加载 meta graph并恢复权重变量

saver = tf.train.import_meta_graph('./tmp/model/-100.meta')
saver.restore(sess,tf.train.latest_checkpoint('./tmp/model/'))
```

```
◆ 5 查看恢复后的变量 

print(sess.run('biases:0'))
>>>
[ 0.  0.  0.  0.  0.  0.  0.  0.  0.  0...................  0.]
```

##### 3.2  共享模型变量

在复杂的深度学习模型中，存在大量的模型变量，并且期望能够一次性地初始化这些变量。TensorFlow提供了   `tf.variable_scope`和`tf.get_variable`两个API，实现了共享模型变量。

```
tf.get_variable(name, shape, initializer)：表示创建或返回指定名称的模型变量，

name           变量名称，
shape          变量的维度信息，
initializer    变量的初始化方法。
tf.variable_scope(<scope_name>)：表示变量所在的命名空间，
scope_name    表示命名空间的名称。
```

共享模型变量使用示例如下：

```python
#定义卷积神经网络运算规则，其中weights和biases为共享变量

def conv_relu(input, kernel_shape, bias_shape):
    weights = tf.get_variable("weights", kernel_shape, initializer=tf.random_normal_initializer())
    biases = tf.get_variable("biases", bias_shape, initializer=tf.constant_initializer(0.0))
    conv = tf.nn.conv2d(input, weights, strides=[1, 1, 1, 1], padding='SAME')
    return tf.nn.relu(conv + biases)

#定义卷积层
with tf.variable_scope("conv1"):   # 创建变量 "conv1/weights", "conv1/biases".
    relu1 = conv_relu(input_images, [5, 5, 32, 32], [32])
with tf.variable_scope("conv2"):   # 创建变量 "conv2/weights", "conv2/biases".
    relu1 = conv_relu(relu1, [5, 5, 32, 32], [32])
```

##### 3.3 `Variables` 与`constant`的区别

```
▶ Constant一般是常量，可以被赋值给Variables，constant保存在graph中，如果graph重复载入那么
   constant也会重复载入，其非常浪费资源，如非必要尽量不使用其保存大量数据。
▶ Variables在每个session中都是单独保存的，甚至可以单独存在一个参数服务器上。
  可以通过代码观察到constant实际是保存在graph中，具体如下。
```

##### 3.4 `variables` 与 `get_variables` 的区别

```
（1）语法格式：
    tf.Variable的参数列表为：
    tf.Variable(name=None, initial_value, validate_shape=True, trainable=True, \
                collections=None)，返回一个由initial_value创建的变量

    tf.get_variable的参数列表为：
    tf.get_variable(name,shape=None,initializer=None, dtype=tf.float32,trainable=True,\
                    collections=None)，如果已存在参数定义相同的变量，就返回已存在的变量，
                    否则创建由参数定义的新变量。

（2）使用tf.Variable时，如果检测到命名冲突，系统会自己处理。
    使用tf.get_variable()时，系统不会处理冲突，而会报错。
```

```python
import tensorflow as tf            #导入模块

w_1 = tf.Variable(3,name="w_1")
w_2 = tf.Variable(1,name="w_1")
print( w_1.name)
print( w_2.name)
输出：
w_1:0
w_1_1:0
--------------------------------------------------
w_1 = tf.get_variable(name="w_1",initializer=1)
w_2 = tf.get_variable(name="w_1",initializer=2)
输出：
alueError          Traceback (most recent call last)
<ipython-input-2-4ad8806618f8> in <module>()
      2 
      3 w_1 = tf.get_variable(name="w_1",initializer=1)
  --> 4 w_2 = tf.get_variable(name="w_1",initializer=2)
```

```
（3）两者的本质区别
    tf.get_variable创建变量时，会进行变量检查，
    设置共享变量时(通过scope.reuse_variables()或tf.get_variable_scope().reuse_variables())
    检查到第二个拥有相同名字的变量，就返回已创建的相同的变量；
    若没有设置共享变量，则会报[ValueError: Variable varx alreadly exists, disallowed.]的错误。

    tf.Variable()创建变量时，name属性值允许重复，
    检查到相同名字的变量时，由自动别名机制创建不同的变量。
```

```python
import tensorflow as tf

tf.reset_default_graph()

with tf.variable_scope("scope1"):
    w1 = tf.get_variable("w1", shape=[])
    w2 = tf.Variable(0.0, name="w2")
with tf.variable_scope("scope1", reuse=True):
    w1_p = tf.get_variable("w1", shape=[])
    w2_p = tf.Variable(1.0, name="w2")

print(w1 is w1_p, w2 is w2_p)
print(w1_p,w2_p)
输出：
True False
<tf.Variable 'scope1/w1:0' shape=() dtype=float32_ref> \
<tf.Variable 'scope1_1/w2:0' shape=() dtype=float32_ref>
```

```
由于tf.Variable() 每次都在创建新对象， 所有reuse=True 和它并没有什么关系。
对于get_variable()，若已经创建的变量对象，返回那个对象，若没有创建变量对象的话，就创建一个新的。

附加：尽量每一个变量都明确的命名，这样易于管理命令空间
```

##### 3.5 `Variables`常用运算
|类         型|示                 例|
|----|----|
|数学运算|add、sub、multiply、Div、Exp、Log、Greater(>)、Less(<)、Equal(=)|
|向量运算|reduce_prod、reduce_sum、Rank、Shape、Shuffle(打乱)|
|矩阵运算|Matmul(矩阵相乘)、MatrixInverse(矩阵求逆)、MatrixDeterminant(矩阵行列式)|
|带状态的运算|Variable、Assign、AssignAdd|
|神经网络组件|SoftMax、Sigmoid、ReLU、Convolution2D、MaxPooling|
|存储、恢复|Save、Restore|
|队列及同步运算|Enqueue、Dequeue、MutexAcquire、MutexRelease|
|控制张量流动|Merge、Switch、Enter、Leave、NextIteration|

### 四、  placeholders 与 feed_dict

当我们定义一张graph时，有时候并不知道需要计算的值，比如模型的输入数据，其只有在训练与预测时才会有值。这时就需要placeholder与feed_dict的帮助。 

定义一个placeholder，

```
可以使用 tf.placeholder(dtype,shape=None,name=None)函数。
    a = tf.placeholder(tf.int32,shape=[1],name='input_a')
    b = tf.constant(2,name='input_b')
    d = tf.add(a,b,name='add_d')
    with tf.Session() as sess:
    print(sess.run(d))
```

在上面的代码中，会抛出错误（InvalidArgumentError (see above for traceback): You must feed a value for placeholder tensor ‘input_a_1’ with dtype int32 and shape [1]），因为计算d需要a的具体值，而在代码中并没有给出。这时候需要将实际值赋给a。最后一行修改如下: 

    print(sess.run(d,{a:[5]})) 

其中最后的dict就是一个feed_dict，一般会使用python读入一些值后传入，当使用minbatch的情况下，每次输入的值都不同。
### 五、constant 、variable及 placeholder的异同

> 
##### 5.1 constant

```
constant()是一个函数，作用是创建一个常量tensor，其格式为：

   tf.constant(value,dtype=None,shape=None,name=’Const’,verify_shape=False)
value: 一个dtype类型（如果指定了）的常量值（列表）。要注意的是，要是value是一个列表的话，
       那么列表的长度不能够超过形状参数指定的大小（如果指定了）。要是列表长度小于指定的，
       那么多余的由列表的最后一个元素来填充。 
dtype: 返回tensor的类型 
shape: 返回的tensor形状。 
name: tensor的名字 
verify_shape: Boolean that enables verification of a shape of values。
```

```
示例：
import tensorflow as tf

a=tf.constant(1.,name="a")           #build graph
b=tf.constant(1.,shape=[2,2],name="b")

sess=tf.Session()                    #construct session

result_a=sess.run([a,b])             #run in session
print("result_a:",result_a[0])
print("result_b:",result_a[1])
输出：
result_a: 1.0
result_b: [[ 1.  1.] [ 1.  1.]]
```

##### 5.2 `variable`

```
通过Variable()构造一个变量（variable），创建一个新的变量，
初始值为initial_value构造函数需要初始值，初始值为initial_value，
初始值可以是一个任何类型任何形状的Tensor。初始值的形状和类型决定了这个变量的形状和类型。
构造之后，这个变量的形状和类型就固定了，他的值可以通过assign()函数来改变。
如果你想要在之后改变变量的形状，你就需要assign()函数同时变量的validate_shape=False。
和任何的Tensor一样，通过Variable（）创造的变量能够作为图中其他操作的输入使用。

◆ 一般的tensor张量在tensorflow运行过程中只是在计算图中流过，并不会保存下来。
◆ varibale用来保存tensorflow构建的一些结构中的参数，得到一个模型。

比如神经网络中的权重和bias等，在训练过后，用到Variable将这些参数能够保存下来。
注意，所有和varible有关的操作在计算的时候都要使用session会话来控制，包括计算，打印等等。
```

```python
格式为：
tf.Variable(initial_value=None, trainable=True, collections=None, \
validate_shape=True, caching_device=None, name=None, variable_def=None, dtype=None, \
expected_shape=None, import_scope=None, constraint=None)

各参数说明：
initial_value:  一个Tensor类型或者是能够转化为Tensor的python对象类型。是这个变量的初始值。
               这个初始值必须指定形状信息，不然后面的参数validate_shape需要设置为false。
               当然也能够传入一个无参数可调用并且返回制定初始值的对象，在这种情况下，dtype必须指定。 
trainable:  如果为True（默认也为Ture），这个变量就会被添加到图的集合
            GraphKeys.TRAINABLE_VARIABLES.中去 ，这个collection被作为优化器类的默认列表。 
collections:图的collection 键列表，新的变量被添加到这些collection中去。默认是
            [GraphKeys.GLOBAL_VARIABLES]. 
validate_shape:   如果是False的话，就允许变量能够被一个形状未知的值初始化，默认是True，表示必须知道形状。 
caching_device:   可选，描述设备的字符串，表示哪个设备用来为读取缓存。默认是变量的device， 
name:         可选，变量的名称 
variable_def: VariableDef protocol buffer. If not None, recreates the Variable object \
              with its contents. variable_def and the other arguments are mutually exclusive. 
dtype:          如果被设置，初始化的值就会按照这里的类型来定。 
expected_shape: TensorShape类型.要是设置了，那么初始的值会是这种形状 
import_scope:   Optional string. Name scope to add to the Variable. 
                Only used when initializing from protocol buffer.
```

```
示例：
import numpy as np
import tensorflow as tf
       #create a Variable
w=tf.Variable(initial_value=[[1,2],[3,4]],dtype=tf.float32)      
x=tf.Variable(initial_value=[[1,1],[1,1]],dtype=tf.float32)
x=x.assign(x*2)
print(x)
y=tf.matmul(w,x)
z=tf.sigmoid(y)
print(z)
init=tf.global_variables_initializer()

with tf.Session() as session:
    session.run(init)
    z=session.run(z)
    print(z)
输出：
Tensor("Assign_1:0", shape=(2, 2), dtype=float32_ref)
Tensor("Sigmoid_2:0", shape=(2, 2), dtype=float32)
[[ 0.99752742  0.99752742]
 [ 0.99999917  0.99999917]]
```

##### 5.3 `placeholder`

```
placeholder的作用可以理解为占个位置，我并不知道这里将会是什么值，但是知道类型和形状等等一些信息， 
先把这些信息填进去占个位置，然后以后用feed的方式来把这些数据“填”进去。返回的就是一个用来用来处理
feeding一个值的tensor。 
那么feed的时候一般就会在你之后session的run（）方法中用到feed_dict这个参数了。这个参数的内容就是 
你要“喂”给那个placeholder的内容。

它是tensorflow中又一保存数据的利器，它在使用的时候和前面的variable不同的是在session运行阶段，需要 
给placeholder提供数据，利用feed_dict的字典结构给placeholdr变量“喂数据”。
```


```
格式：
tf.placeholder(dtype, shape=None, name=None)
参数说明：
dtype: 将要被fed的元素类型 
shape:（可选） 将要被fed的tensor的形状，要是不指定的话，你能够fed进任何形状的tensor。 
name:（可选）这个操作的名字
示例：
x = tf.placeholder(tf.float32, shape=(2, 3))
y=tf.reshape(x,[3,2])
z= tf.matmul(x, y)
print(z)
>
with tf.Session() as sess:
  #print(sess.run(y))  #不注释将报错，因没有给y输入具体数据.
  rand_array_x = np.random.rand(2, 3)
  rand_array_y = np.random.rand(3, 2)
  print(sess.run(z, feed_dict={x: rand_array_x,y: rand_array_y}))  #这句成功
运行结果：
Tensor("MatMul_9:0", shape=(2, 2), dtype=float32)
[[ 0.2707203   0.68843865]
 [ 0.42275223  0.73435611]]
```

### 六、常用概念

TensorFlow架构中，主要的一些概念总结如下。
|**类型**|**描述**|**用途**|
|----|----|----|
|Session|会话|图必须在称之为“会话”的上下文中执行。会话将图的op分发到诸如CPU或者GPU上计算|
|Graph|描述计算过程|必须在Session中启动|
|tensor|数据或张量|数据类型之一,代表多维数组|
|op|操作|图中的节点被称之为op，一个op获得0或者多个Tensor，执行计算，产生0或者多个Tensor|
|Variable|变量|数据类型之一,运行过程中可以被改变,用于维护状态|
|feed|赋值|为op的tensor赋值|
|fetch|取值|从op的tensor中取值|
|Constant|常量|数据类型之一,不可变|
|placeholder|占位符|预先不知道其值，使用或运算过程中赋值。|







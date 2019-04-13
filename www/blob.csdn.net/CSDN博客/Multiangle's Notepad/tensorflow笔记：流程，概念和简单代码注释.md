
# tensorflow笔记：流程，概念和简单代码注释 - Multiangle's Notepad - CSDN博客


2016年09月27日 08:13:18[multiangle](https://me.csdn.net/u014595019)阅读数：62776所属专栏：[深度学习&tensorflow笔记](https://blog.csdn.net/column/details/13414.html)



tensorflow是google在2015年开源的深度学习框架，可以很方便的检验算法效果。这两天看了看官方的tutorial，极客学院的文档，以及综合tensorflow的源码，把自己的心得整理了一下，作为自己的备忘录。
tensorflow笔记系列：
（一）[ tensorflow笔记：流程，概念和简单代码注释](http://blog.csdn.net/u014595019/article/details/52677412)
（二）[tensorflow笔记：多层CNN代码分析](http://blog.csdn.net/u014595019/article/details/52728886)
（三）[tensorflow笔记：多层LSTM代码分析](http://blog.csdn.net/u014595019/article/details/52759104)
（四）[tensorflow笔记：常用函数说明](http://blog.csdn.net/u014595019/article/details/52805444)
（五）[tensorflow笔记：模型的保存与训练过程可视化](http://blog.csdn.net/u014595019/article/details/53912710)
（六）[tensorflow笔记：使用tf来实现word2vec](http://blog.csdn.net/u014595019/article/details/54093161)
---

# 1.tensorflow的运行流程
tensorflow的运行流程主要有2步，分别是**构造模型**和**训练**。
在构造模型阶段，我们需要构建一个图(Graph)来描述我们的模型。所谓图，也可以理解为流程图，就是将数据的输入->中间处理->输出的过程表示出来，就像下面这样。
![20160601101108139.png-26.2kB](http://static.zybuluo.com/multiangle/w5z7qki50wm9kddzshko8meo/20160601101108139.png)[  ](http://static.zybuluo.com/multiangle/w5z7qki50wm9kddzshko8meo/20160601101108139.png)
注意此时是不会发生实际运算的。而在模型构建完毕以后，会进入训练步骤。此时才会有实际的数据输入，梯度计算等操作。那么，**如何构建抽象的模型呢？**这里就要提到tensorflow中的几个概念:**Tensor,Variable,placeholder**,而在训练阶段，则需要介绍**Session**。下面先解释一些上面的几个概念
## 1.1概念描述
---

### 1.1.1 Tensor
Tensor的意思是张量，不过按我的理解，其实就是指矩阵。也可以理解为tensorflow中矩阵的表示形式。Tensor的生成方式有很多种，最简单的就如
```python
import
```
```python
tensorflow
```
```python
as
```
```python
tf
```
```python
# 在下面所有代码中，都去掉了这一行，默认已经导入
```
```python
a = tf.zeros(shape=[
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
])
```
不过要注意，因为在训练开始前，所有的数据都是抽象的概念，也就是说，此时a只是表示这应该是个1*5的零矩阵，而没有实际赋值，也没有分配空间，所以如果此时print,就会出现如下情况:
```python
print
```
```python
(a)
#===>Tensor(
```
```python
"zeros:0"
```
```python
, shape=
```
```python
(1
```
```python
,
```
```python
2
```
```python
), dtype=
```
```python
float32
```
```python
)
```
只有在训练过程开始后，才能获得a的实际值
```python
sess = tf.InteractiveSession()
```
```python
print
```
```python
(sess.run(a))
#===>
```
```python
[[ 0.  0.]]
```
这边设计到Session概念，后面会提到
---

### 1.1.2 Variable
故名思议，是变量的意思。一般**用来表示图中的各计算参数**，包括矩阵，向量等。例如，我要表示上图中的模型，那表达式就是

$$
y=Relu(Wx+b)
$$
（relu是一种激活函数，具体可见[这里](http://blog.csdn.net/u014595019/article/details/52562159)）这里$W$和$b$是我要用来训练的参数，那么此时这两个值就可以用Variable来表示。Variable的初始函数有很多其他选项，这里先不提，只输入一个Tensor也是可以的
```python
W = tf
```
```python
.Variable
```
```python
(tf
```
```python
.zeros
```
```python
(shape=[
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
]))
```
注意，此时W一样是一个抽象的概念，而且**与Tensor不同，Variable必须初始化以后才有具体的值**。
```python
tensor = tf.zeros(shape=[
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
])
variable = tf.Variable(tensor)
sess = tf.InteractiveSession()
#
```
```python
print
```
```python
(sess.run(variable))  # 会报错
sess.run(tf.initialize_all_variables()) # 对variable进行初始化
```
```python
print
```
```python
(sess.run(variable))
#===>
```
```python
[[ 0.  0.]]
```
---

### 1.1.3 placeholder
又叫占位符，同样是一个抽象的概念。用于表示输入输出数据的格式。告诉系统：这里有一个值/向量/矩阵，现在我没法给你具体数值，不过我正式运行的时候会补上的！例如上式中的x和y。因为没有具体数值，所以只要指定尺寸即可
```python
x = tf.placeholder(tf.
```
```python
float32
```
```python
,
```
```python
[1
```
```python
,
```
```python
5
```
```python
],name=
```
```python
'input'
```
```python
)
y = tf.placeholder(tf.
```
```python
float32
```
```python
,[None,
```
```python
5
```
```python
],name=
```
```python
'input'
```
```python
)
```
上面有两种形式，第一种x，表示输入是一个[1,5]的横向量。
而第二种形式，表示输入是一个[?,5]的矩阵。那么什么情况下会这么用呢?就是需要输入一批[1,5]的数据的时候。比如我有一批共10个数据，那我可以表示成[10,5]的矩阵。如果是一批5个，那就是[5,5]的矩阵。tensorflow会自动进行批处理
---

### 1.1.4 Session
session，也就是会话。我的理解是，**session是抽象模型的实现者**。为什么之前的代码多处要用到session？因为模型是抽象的嘛，只有实现了模型以后，才能够得到具体的值。同样，具体的**参数训练，预测，甚至变量的实际值查询，都要用到session**,看后面就知道了
---

## 1.2 模型构建
这里我们使用官方tutorial中的mnist数据集的分类代码，公式可以写作

$$
\begin{align}
&z=Wx+b\\
&a=softmax(z)
\end{align}
$$
那么该模型的代码描述为
```python
# 建立抽象模型
```
```python
x
```
```python
= tf
```
```python
.placeholder
```
```python
(tf
```
```python
.float
```
```python
32, [None,
```
```python
784
```
```python
])
```
```python
# 输入占位符
```
```python
y
```
```python
= tf
```
```python
.placeholder
```
```python
(tf
```
```python
.float
```
```python
32, [None,
```
```python
10
```
```python
])
```
```python
# 输出占位符（预期输出）
```
```python
W = tf
```
```python
.Variable
```
```python
(tf
```
```python
.zeros
```
```python
([
```
```python
784
```
```python
,
```
```python
10
```
```python
]))        
b = tf
```
```python
.Variable
```
```python
(tf
```
```python
.zeros
```
```python
([
```
```python
10
```
```python
]))
a = tf
```
```python
.nn
```
```python
.softmax
```
```python
(tf
```
```python
.matmul
```
```python
(
```
```python
x
```
```python
, W) + b)
```
```python
# a表示模型的实际输出
```
```python
# 定义损失函数和训练方法
```
```python
cross_entropy = tf
```
```python
.reduce
```
```python
_mean(-tf
```
```python
.reduce
```
```python
_sum(
```
```python
y
```
```python
* tf
```
```python
.log
```
```python
(a), reduction_indices=[
```
```python
1
```
```python
]))
```
```python
# 损失函数为交叉熵
```
```python
optimizer = tf
```
```python
.train
```
```python
.GradientDescentOptimizer
```
```python
(
```
```python
0.5
```
```python
)
```
```python
# 梯度下降法，学习速率为0.5
```
```python
train = optimizer
```
```python
.minimize
```
```python
(cross_entropy)
```
```python
# 训练目标：最小化损失函数
```
可以看到这样以来，模型中的所有元素(图结构，损失函数，下降方法和训练目标)都已经包括在train里面。我们可以把train叫做**训练模型**。那么我们还需要**测试模型**
```python
correct_prediction = tf
```
```python
.equal
```
```python
(tf
```
```python
.argmax
```
```python
(a,
```
```python
1
```
```python
), tf
```
```python
.argmax
```
```python
(
```
```python
y
```
```python
,
```
```python
1
```
```python
))
accuracy = tf
```
```python
.reduce
```
```python
_mean(tf
```
```python
.cast
```
```python
(correct_prediction, tf
```
```python
.float
```
```python
32))
```
上述两行代码，tf.argmax表示找到最大值的位置(也就是预测的分类和实际的分类)，然后看看他们是否一致，是就返回true,不是就返回false,这样得到一个boolean数组。tf.cast将boolean数组转成int数组，最后求平均值，得到分类的准确率(怎么样，是不是很巧妙)
---

## 1.3 实际训练
有了训练模型和测试模型以后，我们就可以开始进行实际的训练了
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
# 建立交互式会话
```
```python
tf
```
```python
.initialize
```
```python
_all_variables()
```
```python
.run
```
```python
()
```
```python
# 所有变量初始化
```
```python
for i
```
```python
in
```
```python
range(
```
```python
1000
```
```python
):
    batch_xs, batch_ys = mnist
```
```python
.train
```
```python
.next
```
```python
_batch(
```
```python
100
```
```python
)
```
```python
# 获得一批100个数据
```
```python
train
```
```python
.run
```
```python
({
```
```python
x
```
```python
: batch_xs,
```
```python
y
```
```python
: batch_ys})
```
```python
# 给训练模型提供输入和输出
```
```python
print(sess
```
```python
.run
```
```python
(accuracy,feed_dict={
```
```python
x
```
```python
:mnist
```
```python
.test
```
```python
.images
```
```python
,
```
```python
y
```
```python
:mnist
```
```python
.test
```
```python
.labels
```
```python
}))
```
可以看到，在模型搭建完以后，我们只要为模型提供输入和输出，模型就能够自己进行训练和测试了。中间的求导，求梯度，反向传播等等繁杂的事情，tensorflow都会帮你自动完成。
---

# 2. 实际代码
实际操作中，还包括了获取数据的代码
```python
"""A very simple MNIST classifier.
See extensive documentation at
http://tensorflow.org/tutorials/mnist/beginners/index.md
"""
```
```python
from
```
```python
__future__
```
```python
import
```
```python
absolute_import
```
```python
from
```
```python
__future__
```
```python
import
```
```python
division
```
```python
from
```
```python
__future__
```
```python
import
```
```python
print_function
```
```python
# Import data
```
```python
from
```
```python
tensorflow.examples.tutorials.mnist
```
```python
import
```
```python
input_data
```
```python
import
```
```python
tensorflow
```
```python
as
```
```python
tf
flags = tf.app.flags
FLAGS = flags.FLAGS
flags.DEFINE_string(
```
```python
'data_dir'
```
```python
,
```
```python
'/tmp/data/'
```
```python
,
```
```python
'Directory for storing data'
```
```python
)
```
```python
# 把数据放在/tmp/data文件夹中
```
```python
mnist = input_data.read_data_sets(FLAGS.data_dir, one_hot=
```
```python
True
```
```python
)
```
```python
# 读取数据集
```
```python
# 建立抽象模型
```
```python
x = tf.placeholder(tf.float32, [
```
```python
None
```
```python
,
```
```python
784
```
```python
])
```
```python
# 占位符
```
```python
y = tf.placeholder(tf.float32, [
```
```python
None
```
```python
,
```
```python
10
```
```python
])
W = tf.Variable(tf.zeros([
```
```python
784
```
```python
,
```
```python
10
```
```python
]))
b = tf.Variable(tf.zeros([
```
```python
10
```
```python
]))
a = tf.nn.softmax(tf.matmul(x, W) + b)
```
```python
# 定义损失函数和训练方法
```
```python
cross_entropy = tf.reduce_mean(-tf.reduce_sum(y * tf.log(a), reduction_indices=[
```
```python
1
```
```python
]))
```
```python
# 损失函数为交叉熵
```
```python
optimizer = tf.train.GradientDescentOptimizer(
```
```python
0.5
```
```python
)
```
```python
# 梯度下降法，学习速率为0.5
```
```python
train = optimizer.minimize(cross_entropy)
```
```python
# 训练目标：最小化损失函数
```
```python
# Test trained model
```
```python
correct_prediction = tf.equal(tf.argmax(a,
```
```python
1
```
```python
), tf.argmax(y,
```
```python
1
```
```python
))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
```
```python
# Train
```
```python
sess = tf.InteractiveSession()
```
```python
# 建立交互式会话
```
```python
tf.initialize_all_variables().run()
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
1000
```
```python
):
    batch_xs, batch_ys = mnist.train.next_batch(
```
```python
100
```
```python
)
    train.run({x: batch_xs, y: batch_ys})
print(sess.run(accuracy,feed_dict={x:mnist.test.images,y:mnist.test.labels}))
```
得到的分类准确率在91%左右


# TensorFlow 高级之一（实现单层神经网络） - wsp_1138886114的博客 - CSDN博客





2018年08月05日 13:08:41[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：513








### 单层神经网络（一个神经元）

```
实现环境：TensorFlow1.3，Python3.6，Jupyter notebook
```



- - - [单层神经网络（一个神经元）](#单层神经网络一个神经元)
- [1 第一个 tensorflow](#1-第一个-tensorflow)
- [2 定义一个神经元](#2-定义一个神经元)
- [3 TensorBoard可视化你的图](#3-tensorboard可视化你的图)
- [4 训练神经元](#4-训练神经元)
- [5 可视化及训练全过程](#5-可视化及训练全过程)
- [实践](#实践)






### 1 第一个 tensorflow

```python
import tensorflow as tf               # 导入tensorflow

graph = tf.get_default_graph()        # 定义一个图
input_value = tf.constant(1.0)        # 定义一个常数 

sess = tf.Session()                   #定义session
weight = tf.Variable(0.8)             #初始权重
output_value = weight * input_value   #让权重与输入相乘，得到输出 

init=tf.global_variables_initializer()#全局初始化
sess.run(init)
sess.run(output_value)                #输出:0.8
```

```
不像Python变量一样，定义后立即可以看到其值，对于TensorFlow定义 
的这个常数，为了执行input_value这句话，并给出这个数字的值，我们需要创造一个“会话”（session）。 
让图里的计算在其中执行并明确地要执行input_value并给出结果（会话会默认地去找那个默认图）

“执行”一个常量可能会让人觉得有点怪。但是这与在Python里执行一个表达式类似。 
这就是TensorFlow管理它,自己的对象空间（计算图）和它自己的执行方式。
```

### 2 定义一个神经元

```
接下来我们定义一个神经元，假设我们有一个训练集，输入为1，输出为0.8（正确输出是0）。
假设我们要预测的函数为：y=w*x,其中w为权重，x为输入，y为输出或预测值。
他们间的关系如下图所示：
```


![这里写图片描述](https://img-blog.csdn.net/20180614173428627?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


```
现在已经有了一个会话，其中有一个简单的图。 
下面让我们构建仅有一个参数的神经元，或者叫权重。 
通常即使是简单的神经元也都会有偏置项和非一致的启动函数或激活函数。
神经元的权重不应该是常量，我们会期望这个值能改变，从而学习训练数据里的输入和输出。
这里我们定义权重是一个TensorFlow的变量，
让权重与输入相乘，得到输出
output_value = weight * input_value

tf.global_variables_initializer()函数生成了一个运算，来初始化所有的变量（我们的情况是只有一个变量）。 

init=tf.global_variables_initializer()
sess.run(init) 

tf.global_variables_initializer()的结果会包括现在图里所有变量的初始化器。 
所以如果你后续加入了新的变量，你就需要再次使用tf.global_variables_initializer()。 
一个旧的init是不会包括新的变量的。
现在我们已经准备好运行output_value运算了。

0.8 * 1.0是一个32位的浮点数，而32位浮点数一般不会是0.8。0.80000001是系统可以获得的一个近似值。
```

### 3 TensorBoard可视化你的图

![这里写图片描述](https://img-blog.csdn.net/20180614174807212?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 4 训练神经元

我们已经有了一个神经元，但如何才能让它学习？假定我们让输入为1，而正确的输出应该是0。也就是说我们有了一个仅有一条记录且记录只有一个特征（值为1）和一个结果（值为0）的训练数据集。我们现在希望这个神经元能学习这个1->0的函数。 

目前的这个系统是输入1而输出0.8。但不是我们想要的。我们需要一个方法来测量系统误差是多少。我们把对误差的测量称为“损失”，并把损失最小化设定为系统的目标。损失是可以为负值的，而对负值进行最小化是毫无意思的。所以我们用实际输出和期望输出之差的平方来作为损失的测量值。

```
y_ = tf.constant(0.0)
loss = (y - y_)**2
```

对此，现有的图还不能做什么事情。所以我们需要一个优化器。这里我们使用梯度下降优化器来基于损失值的导数去更新权重。这个优化器采用一个学习率来调整每一步更新的大小。这里我们设为0.025。

```
optim = tf.train.GradientDescentOptimizer(learning_rate=0.025)
```

这个优化器很聪明。它自动地运行，并在整个网络里恰当地设定梯度，完成后向的学习过程。让我们看看我们的简单例子里的梯度是什么样子的。

```
grads_and_vars = optim.compute_gradients(loss)
```

那么compute_gradients可能会返回（None，v），即部分变量没有对应的梯度，在下一步的时候NoneType会导致错误。因此，需要将有梯度的变量提取出来，记为grads_vars。

```
grads_vars = [v for (g,v) in grads_and_vars if g is not None]
```

之后，对grads_vars再一次计算梯度，得到了gradient。

```
gradient = optim.compute_gradients(loss, grads_vars)
sess.run(tf.global_variables_initializer())
sess.run(gradient)
输出：[(1.6, 0.80000001)]
```

为什么梯度值是1.6？我们的损失函数是错误的平方，因此它的导数就是这个错误乘2。现在系统的输出是0.8而不是0，所以这个错误就是0.8，乘2就是1.6。优化器是对的! 

对于更复杂的系统，TensorFlow可以自动地计算并应用这些梯度值。 

让我们运用这个梯度来完成反向传播。 
```
sess.run(tf.global_variables_initializer())
sess.run(optim.apply_gradients(grads_and_vars))
sess.run(w)
输出：0.75999999，约为0.76
```

现在权重减少了0.04，这是因为优化器减去了梯度乘以学习比例（1.6*0.025），即：

```
w_2=w_1-r Δy/Δx=0.8-0.025*1.6
```

权重向着正确的方向在变化。 

其实我们不必像这样调用优化器。我们可以形成一个运算，自动地计算和使用梯度：train_step。

```
train_step = tf.train.GradientDescentOptimizer(0.025).minimize(loss)
for i in range(100):
    sess.run(train_step)
sess.run(y)
输出：0.0047364226
```

通过100次运行训练步骤后，权重和输出值已经非常接近0了。这个神经元已经学会了！

### 5 可视化及训练全过程

```
# 目标: 输入1 --> 输出0
import tensorflow as tf

x = tf.constant(1.0, name='input')
w = tf.Variable(0.8, name='weight')
y = tf.multiply(w, x, name='output')
y_ = tf.constant(0.0, name='correct_value')
loss = tf.pow(y - y_, 2, name='loss')
train_step = tf.train.GradientDescentOptimizer(0.025).minimize(loss)

for value in [x, w, y, y_, loss]:
    tf.summary.scalar(value.op.name, value)

summaries = tf.summary.merge_all()

sess = tf.Session()
summary_writer = tf.summary.FileWriter('./log/simple_stats', sess.graph)
sess.run(tf.global_variables_initializer())

for i in range(100):
    summary_str = sess.run(summaries)
    summary_writer.add_summary(summary_str, i)
    sess.run(train_step)
    if i%20 ==0 or i==99:
        print("训练{}次结果y为：{}，损失函数：{}".format(i+1,sess.run(y),sess.run(loss)))

输出：
训练1次结果y为：0.7599999904632568，损失函数：0.5776000022888184
训练21次结果y为：0.272449254989624，损失函数：0.07422859966754913
训练41次结果y为：0.09766922146081924，损失函数：0.009539276361465454
训练61次结果y为：0.03501303866505623，损失函数：0.0012259128270670772
训练81次结果y为：0.012551683932542801，损失函数：0.00015754476771689951
训练100次结果y为：0.004736422561109066，损失函数：2.2433698177337646e-05
```

### 实践

> 
```
动手实践：用单个神经元训练一个函数：y=2x+1
预测值y=wx+b
根据以下训练集，训练参数w，b
输入数据集：
x:  [0.1,0.2,0.5,1,1.5,1.6]
y_:[1.1,1.5,2.0,3,4,4.3]
```

```python
import tensorflow as tf

x = tf.placeholder(tf.float32,shape=[None])
w = tf.Variable([1.0])
b = tf.Variable([0.2])
y = tf.multiply(w, x)
y=tf.add(y,b)
y_ = tf.placeholder(tf.float32,shape=[None])
loss = tf.reduce_mean(tf.square((y - y_)))

train_step = tf.train.GradientDescentOptimizer(0.001).minimize(loss)

sess = tf.Session()
sess.run(tf.global_variables_initializer())

for i in range(2000):
    w_,b_,loss_=sess.run([w,b,loss],
                feed_dict={x:[0.1,0.2,0.5,1,1.5,1.6],y_:[1.1,1.5,2.0,3,4,4.3]})  
    sess.run([y,train_step],
                feed_dict={x:[0.1,0.2,0.5,1,1.5,1.6],y_:[1.1,1.5,2.0,3,4,4.3]})
    if i%200==0:
        print('w=', w_, 'b=', b_)  
输出：
w= [ 1.] b= [ 0.2]
w= [ 1.48553479] b= [ 0.66175097]
w= [ 1.72320616] b= [ 0.879767]
w= [ 1.84133148] b= [ 0.98082918]
w= [ 1.90166068] b= [ 1.02590919]
w= [ 1.93392718] b= [ 1.04431856]
w= [ 1.95245183] b= [ 1.05014014]
w= [ 1.96414256] b= [ 1.05013025]
w= [ 1.97233987] b= [ 1.04752338]
w= [ 1.97866631] b= [ 1.0438596]
```






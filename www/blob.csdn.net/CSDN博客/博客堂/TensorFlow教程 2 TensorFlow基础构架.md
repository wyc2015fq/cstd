# TensorFlow教程 2 TensorFlow基础构架 - 博客堂 - CSDN博客





2017年07月14日 11:16:04[最小森林](https://me.csdn.net/u012052268)阅读数：708标签：[session																[神经网络																[tensorflow																[激活](https://so.csdn.net/so/search/s.do?q=激活&t=blog)
个人分类：[深度学习](https://blog.csdn.net/u012052268/article/category/6969928)





# 2 TensorFlow基础构架


- [TensorFlow基础构架](#2-tensorflow基础构架)- [1 处理结构](#21-处理结构)
- [2 例子2](#22-例子2)
- [3 Session 会话控制](#23-session-会话控制)
- [4 Variable 变量](#24-variable-变量)
- [5 Placeholder 传入值](#25-placeholder-传入值)
- [6 激励函数 Activation Function](#26-激励函数-activation-function)




## 2.1 处理结构

Tensorflow的运算步骤是：
- 首先要定义神经网络的结构。
- 然后再把数据放入结构当中去运算和 training.

如图： 
![image](https://morvanzhou.github.io/static/results/tensorflow/1_4_1.png)
其中：Gradients是它的计算模块，上面的SGD是误差计算。

> 
什么是TensorFlow？


TensorFlow是采用数据流图（data　flow　graphs）来计算, 所以首先我们得创建一个数据流流图, 然后再将我们的数据（数据以张量(tensor)的形式存在）放在数据流图中计算. 节点（Nodes）在图中表示数学操作,图中的线（edges）则表示在节点间相互联系的多维数据数组, 即张量（tensor). 训练模型时tensor会不断的从数据流图中的一个节点flow到另一节点, 这就是TensorFlow名字的由来.

```
张量（tensor):
` 张量有多种. 零阶张量为 纯量或标量 (scalar) 也就是一个数值. 比如 [1]
` 一阶张量为 向量 (vector), 比如 一维的 [1, 2, 3]
` 二阶张量为 矩阵 (matrix), 比如 二维的 [[1, 2, 3],[4, 5, 6],[7, 8, 9]]
` 以此类推, 还有 三阶 三维的 …
```

## 2.2 例子2

```python
import tensorflow as tf
import numpy as np

## 1.构造实验数据
x_data = np.random.rand(100).astype(np.float32)
y_data = x_data * 0.1 + 0.3 # y与x的关系：权重 偏置

## 2.创建tensorflow的结构
# 定义 权重变量（随机生成的）
Weights = tf.Variable(tf.random_uniform([1],-1.0,1.0))
# 定义 偏置
biases = tf.Variable(tf.zeros([1]))
# 定义 模型输出
y = x_data * Weights +biases

# 接着就是计算 y 和 y_data 的误差:
loss = tf.reduce_mean(tf.square(y - y_data))

# 定义优化器 optimizer：
optimizer = tf.train.GradientDescentOptimizer(0.5) # 我们使用的误差传递方法是梯度下降法
train = optimizer.minimize(loss)

## 3.激活刚才定义的神经网络结构
init = tf.global_variables_initializer()  # 激活器 初始化所有之前定义的Variable

sess = tf.Session() # 创建会话 Session
sess.run(init) # 用 Session 来 run 激活前面的变量.

## 4.训练模型
for step in range(201):
    sess.run(train)
    if step % 20 == 0:
        print("第",step,"次训练，参数为 ： ",sess.run(Weights),sess.run(biases))
```

## 2.3 Session 会话控制

Session 是 Tensorflow 为了控制,和输出文件的执行的语句. 

运行 session.run() 可以获得你要得知的运算结果, 或者是你所要运算的部分.

> 
例子：建立两个 matrix ,输出两个 matrix 矩阵相乘的结果。


```python
import tensorflow as tf

# create two matrixes

matrix1 = tf.constant([[3,3]])
matrix2 = tf.constant([[2],
                       [2]])
product = tf.matmul(matrix1,matrix2)
```

因为 product 不是直接计算的步骤, 所以我们会要使用 Session 来激活 product 并的到计算结果. 有两种形式使用会话控制 Session 。

```python
# method 1
sess = tf.Session()
result = sess.run(product)
print(result)
sess.close()
# [[12]]

# method 2
with tf.Session() as sess:
    result2 = sess.run(product)
    print(result2)
# [[12]]
```

其中with的方法会帮助我们在必要的时候关闭session

## 2.4 Variable 变量

在 Tensorflow 中，用 ==定义语法： state = tf.Variable()== 定义了某字符串是变量，它才是变量，这一点是与 Python 所不同的。

```python
import tensorflow as tf

# 这里定义变量！赋初值，名字
state = tf.Variable(0, name='counter')

# 定义常量 one
one = tf.constant(1)

# 定义加法步骤 (注: 此步并没有直接计算)
new_value = tf.add(state, one)

# 将 State 更新成 new_value，这一步也并没有真正计算
update = tf.assign(state, new_value)
```

如果你在 Tensorflow 中设定了变量，那么初始化变量是最重要的！！所以定义了变量以后, 一定要定义 ==init = tf.initialize_all_variables()==

到这里变量还是没有被激活，需要再在 sess 里, sess.run(init) , 激活 init 这一步.

```python
# 如果定义 Variable, 就一定要 initialize
# init = tf.initialize_all_variables() # tf 马上就要废弃这种写法
init = tf.global_variables_initializer()  # 替换成这样就好

# 使用 Session
with tf.Session() as sess:
    sess.run(init)
    for _ in range(3):
        sess.run(update)
        print(sess.run(state))
```

> 
注意：直接 print(state) 不起作用！！


一定要把 sess 的指针指向 state 再进行 print 才能得到想要的结果！

## 2.5 Placeholder 传入值

placeholder 是 Tensorflow 中的占位符，暂时储存变量.

Tensorflow 如果想要从外部传入data, 那就需要用到 tf.placeholder(), 然后以这种形式传输数据 sess.run(***, feed_dict={input: **}).

示例：

```python
import tensorflow as tf

#在 Tensorflow 中需要定义 placeholder 的 type ，一般为 float32 形式
input1 = tf.placeholder(tf.float32)
input2 = tf.placeholder(tf.float32)

# mul = multiply 是将input1和input2 做乘法运算，并输出为 output 
ouput = tf.multiply(input1, input2)
```

接下来, 传值的工作交给了 sess.run() , 需要传入的值放在了字典 feed_dict={} 中  

并一一对应每一个 input. 

placeholder 与 feed_dict={} 是绑定在一起出现的。

```python
with tf.Session() as sess:
    print(sess.run(ouput, feed_dict={input1: [7.], input2: [2.]}))
# [ 14.]
```

## 2.6 激励函数 Activation Function

激励函数运行时激活神经网络中某一部分神经元，将激活信息向后传入下一层的神经系统。激励函数的实质是非线性方程。 

Tensorflow 的神经网络 里面处理较为复杂的问题时都会需要运用激励函数 activation function 。](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)





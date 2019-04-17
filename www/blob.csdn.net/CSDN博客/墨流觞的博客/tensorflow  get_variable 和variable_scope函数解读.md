# tensorflow   get_variable 和variable_scope函数解读 - 墨流觞的博客 - CSDN博客





2018年09月09日 23:54:31[墨氲](https://me.csdn.net/dss_dssssd)阅读数：187








# tensorflow 存储训练数据和构建tensorboard中的可视化网络结构

## 操作变量所属的域和变量分享

主要涉及两个函数 

* tf.get_variable(name, shape, initializer)

```
>如果要创建变量的名字存在，则返回已经创建的变量，否则使用参数shape和initializer来创建新的函数
```

* tf.variable_scope()

> 
管理域名，并且确定变量所在的域。 

     不同作用域下，变量不共享，所以变量名可以相同。


推荐使用tf.variable_scope来给每一层网络创建一个作用域，并且使用tf.get_variable代替tf.Variable来创建tensorflow变量

### 例子
- 
不使用上面两个函数：

```python
def my_network(input):
    W_1 = tf.Variable(tf.random_uniform([784, 100], -1, 1),
    name="W_1")
    b_1 = tf.Variable(tf.zeros([100]), name="biases_1")
    output_1 = tf.matmul(input, W_1) + b_1
    W_2 = tf.Variable(tf.random_uniform([100, 50], -1, 1),
    name="W_2")
    b_2 = tf.Variable(tf.zeros([50]), name="biases_2")
    output_2 = tf.matmul(output_1, W_2) + b_2
    W_3 = tf.Variable(tf.random_uniform([50, 10], -1, 1),
    name="W_3")
    b_3 = tf.Variable(tf.zeros([10]), name="biases_3")
    output_3 = tf.matmul(output_2, W_3) + b_3
    # printing names
    print ("Printing names of weight parameters")
    print (W_1.name, W_2.name, W_3.name)
    print ("Printing names of bias parameters")
    print (b_1.name, b_2.name, b_3.name)
    return output_3
```

执行代码输出：

```python
i_1 = tf.placeholder(tf.float32,[1000, 784],name="i_1")
    my_network(i_1)
    i_2 = tf.placeholder(tf.float32, [1000, 784],name="i_2")
    my_network(i_2)
'''
output:
Printing names of weight parameters
W_1:0 W_2:0 W_3:0
Printing names of bias parameters
biases_1:0 biases_2:0 biases_3:0

W_1_1:0 W_2_1:0 W_3_1:0
Printing names of bias parameters
biases_1_1:0 biases_2_1:0 biases_3_1:0
'''
```

可以看出在第二个网络创建时，将变量名通过添加后缀的形式来创建新的变量。这种tensorflow默认的创建方式会由于变量名的的混乱而会增加可视化工作的难度。

- 
使用刚开始两个函数修改此网络

```python
def layer(input, weight_shape, bias_shape):
        # 变量初始化器， 将作为get_variable的参数
        weight_init = tf.random_uniform_initializer(minval=-1,
        maxval=1)
        bias_init = tf.constant_initializer(value=0)
        # 使用initializer指定上面创建的初始化器
        W = tf.get_variable("W", weight_shape,
        initializer=weight_init)
        b = tf.get_variable("b", bias_shape,
        initializer=bias_init)
        return tf.matmul(input, W) + b

    def my_network(input):
        # 为每一层网路创建一个特有的域
        with tf.variable_scope("layer_1"):
        output_1 = layer(input, [784, 100], [100])
        with tf.variable_scope("layer_2"):
        output_2 = layer(output_1, [100, 50], [50])
        with tf.variable_scope("layer_3"):
        output_3 = layer(output_2, [50, 10], [10])
        return output_3
```

将会在最后的代码中看到这种书写风格的巨大优势，注意tensorflow中的域名规则和python中的域名规则相同，一个作用域内可以访问其子作用域中的所有变量，类似于集合的文恩图。

下面看一个例子：

```python
i_1 = tf.placeholder(tf.float32, [1000, 784], name="i_1")
 my_network(i_1)

 i_2 = tf.placeholder(tf.float32, [1000, 784],name="i_2")
 my_network(i_2)
 ''output
 <tensorflow.python.framework.ops.Tensor ...>
 ValueError: Over-sharing: Variable layer_1/W already exists...
 '''
```

发现第二个网络输出报错了，因为在创建第二个网络时，作用域的名字和第一个作用域相同，也就是说和第一网络使用同一个作用域，这就导致名字冲突，报错。而这是tensorflow为了安全问题而规定的变量名不可共享。

可以通过函数强制共享：

```python
with tf.variable_scope("shared_variables") as scope:
        i_1 = tf.placeholder(tf.float32, [1000, 784], name="i_1")
        my_network(i_1)
        # 强制共享
        scope.reuse_variables() 

        i_2 = tf.placeholder(tf.float32, [1000, 784], name="i_2")
        my_network(i_2)
```

下面我们介绍存储历史保存数据的tensorflow内置函数，最终是在tensorboard展示的





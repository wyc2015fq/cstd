# TensorFlow学习笔记 —— tf.train.Optimizer - Hxyue2018's Blog - CSDN博客





2018年12月13日 21:42:36[Hxyue2018](https://me.csdn.net/Super_Json)阅读数：62








反正是要学一些API的，不如直接从例子里面学习怎么使用API，这样同时可以复习一下一些基本的机器学习知识。但是一开始开始和以前一样，先直接讲类和常用函数用法，然后举例子。 

这里主要是各种优化器，以及使用。因为大多数机器学习任务就是最小化损失，在损失定义的情况下，后面的工作就交给优化器啦。 

因为深度学习常见的是对于梯度的优化，也就是说，优化器最后其实就是各种对于梯度下降算法的优化。 

理论部分可以见斯坦福深度学习的课程。同时这里推荐一个博客，总结了这些优化器的原理以及性能，写的挺好的：An overview of gradient descent optimazation algorithms 

从其中讲几个比较常用的，其他的可以自己去看文档。官方文档：Training

Optimizer 

GradientDescentOptimizer 

AdagradOptimizer 

AdagradDAOptimizer 

MomentumOptimizer 

AdamOptimizer 

FtrlOptimizer 

RMSPropOptimizer

二.常用的optimizer类

Ⅰ.class tf.train.Optimizer

优化器（optimizers）类的基类。这个类定义了在训练模型的时候添加一个操作的API。你基本上不会直接使用这个类，但是你会用到他的子类比如GradientDescentOptimizer, AdagradOptimizer, MomentumOptimizer.等等这些。 

后面讲的时候会详细讲一下GradientDescentOptimizer 这个类的一些函数，然后其他的类只会讲构造函数，因为类中剩下的函数都是大同小异的

Ⅱ.class tf.train.GradientDescentOptimizer

这个类是实现梯度下降算法的优化器。(结合理论可以看到，这个构造函数需要的一个学习率就行了)

__init__(learning_rate, use_locking=False,name=’GradientDescent’)

作用：创建一个梯度下降优化器对象 

参数： 

learning_rate: A Tensor or a floating point value. 要使用的学习率 

use_locking: 要是True的话，就对于更新操作（update operations.）使用锁 

name: 名字，可选，默认是”GradientDescent”.

compute_gradients(loss,var_list=None,gate_gradients=GATE_OP,aggregation_method=None,colocate_gradients_with_ops=False,grad_loss=None)

作用：对于在变量列表（var_list）中的变量计算对于损失函数的梯度,这个函数返回一个（梯度，变量）对的列表，其中梯度就是相对应变量的梯度了。这是minimize()函数的第一个部分， 

参数： 

loss: 待减小的值 

var_list: 默认是在GraphKey.TRAINABLE_VARIABLES. 

gate_gradients: How to gate the computation of gradients. Can be GATE_NONE, GATE_OP, or GATE_GRAPH. 

aggregation_method: Specifies the method used to combine gradient terms. Valid values are defined in the class AggregationMethod. 

colocate_gradients_with_ops: If True, try colocating gradients with the corresponding op. 

grad_loss: Optional. A Tensor holding the gradient computed for loss.

apply_gradients(grads_and_vars,global_step=None,name=None)

作用：把梯度“应用”（Apply）到变量上面去。其实就是按照梯度下降的方式加到上面去。这是minimize（）函数的第二个步骤。 返回一个应用的操作。 

参数: 

grads_and_vars: compute_gradients()函数返回的(gradient, variable)对的列表 

global_step: Optional Variable to increment by one after the variables have been updated. 

name: 可选，名字

get_name()

minimize(loss,global_step=None,var_list=None,gate_gradients=GATE_OP,aggregation_method=None,colocate_gradients_with_ops=False,name=None,grad_loss=None)

作用：非常常用的一个函数 

通过更新var_list来减小loss，这个函数就是前面compute_gradients() 和apply_gradients().的结合

Ⅲ.class tf.train.AdadeltaOptimizer

实现了 Adadelta算法的优化器，可以算是下面的Adagrad算法改进版本

构造函数： 

tf.train.AdadeltaOptimizer.init(learning_rate=0.001, rho=0.95, epsilon=1e-08, use_locking=False, name=’Adadelta’)

作用：构造一个使用Adadelta算法的优化器 

参数： 

learning_rate: tensor或者浮点数，学习率 

rho: tensor或者浮点数. The decay rate. 

epsilon: A Tensor or a floating point value. A constant epsilon used to better conditioning the grad update. 

use_locking: If True use locks for update operations. 

name: 【可选】这个操作的名字，默认是”Adadelta”

IV.class tf.train.AdagradOptimizer

Optimizer that implements the Adagrad algorithm.

See this paper. 

tf.train.AdagradOptimizer.__init__(learning_rate, initial_accumulator_value=0.1, use_locking=False, name=’Adagrad’)

Construct a new Adagrad optimizer. 

Args:

learning_rate: A Tensor or a floating point value. The learning rate.

initial_accumulator_value: A floating point value. Starting value for the accumulators, must be positive.

use_locking: If True use locks for update operations.

name: Optional name prefix for the operations created when applying gradients. Defaults to "Adagrad".

1

2

3

4

Raises:

ValueError: If the initial_accumulator_value is invalid.

1

The Optimizer base class provides methods to compute gradients for a loss and apply gradients to variables. A collection of subclasses implement classic optimization algorithms such as GradientDescent and Adagrad.

You never instantiate the Optimizer class itself, but instead instantiate one of the subclasses.

Ⅴ.class tf.train.MomentumOptimizer

Optimizer that implements the Momentum algorithm.

tf.train.MomentumOptimizer.__init__(learning_rate, momentum, use_locking=False, name=’Momentum’, use_nesterov=False)

Construct a new Momentum optimizer.

Args:

learning_rate: A Tensor or a floating point value. The learning rate. 

momentum: A Tensor or a floating point value. The momentum. 

use_locking: If True use locks for update operations. 

name: Optional name prefix for the operations created when applying gradients. Defaults to “Momentum”.

Ⅵ.class tf.train.AdamOptimizer

实现了Adam算法的优化器 

构造函数： 

tf.train.AdamOptimizer.__init__(learning_rate=0.001, beta1=0.9, beta2=0.999, epsilon=1e-08, use_locking=False, name=’Adam’)

Construct a new Adam optimizer.

Initialization:

m_0 <- 0 (Initialize initial 1st moment vector) 

v_0 <- 0 (Initialize initial 2nd moment vector) 

t <- 0 (Initialize timestep) 

The update rule for variable with gradient g uses an optimization described at the end of section2 of the paper:

t <- t + 1 

lr_t <- learning_rate * sqrt(1 - beta2^t) / (1 - beta1^t)

m_t <- beta1 * m_{t-1} + (1 - beta1) * g 

v_t <- beta2 * v_{t-1} + (1 - beta2) * g * g 

variable <- variable - lr_t * m_t / (sqrt(v_t) + epsilon) 

The default value of 1e-8 for epsilon might not be a good default in general. For example, when training an Inception network on ImageNet a current good choice is 1.0 or 0.1.

Note that in dense implement of this algorithm, m_t, v_t and variable will update even if g is zero, but in sparse implement, m_t, v_t and variable will not update in iterations g is zero.

Args:

learning_rate: A Tensor or a floating point value. The learning rate. 

beta1: A float value or a constant float tensor. The exponential decay rate for the 1st moment estimates. 

beta2: A float value or a constant float tensor. The exponential decay rate for the 2nd moment estimates. 

epsilon: A small constant for numerical stability. 

use_locking: If True use locks for update operations. 

name: Optional name for the operations created when applying gradients. Defaults to “Adam”.

三.例子

I.线性回归

要是有不知道线性回归的理论知识的，请到 

http://blog.csdn.net/xierhacker/article/details/53257748 

http://blog.csdn.net/xierhacker/article/details/53261008 

熟悉的直接跳过。 

直接上代码:

import tensorflow as tf

import numpy as np

import matplotlib.pyplot as plt

# Prepare train data

train_X = np.linspace(-1, 1, 100)

train_Y = 2 * train_X + np.random.randn(*train_X.shape) * 0.33 + 10

# Define the model

X = tf.placeholder("float")

Y = tf.placeholder("float")

w = tf.Variable(0.0, name="weight")

b = tf.Variable(0.0, name="bias")

loss = tf.square(Y - X*w - b)

train_op = tf.train.GradientDescentOptimizer(0.01).minimize(loss)

# Create session to run

with tf.Session() as sess:

    sess.run(tf.initialize_all_variables())

    epoch = 1

    for i in range(10):

        for (x, y) in zip(train_X, train_Y):

            _, w_value, b_value = sess.run([train_op, w, b],feed_dict={X: x,Y: y})

        print("Epoch: {}, w: {}, b: {}".format(epoch, w_value, b_value))

        epoch += 1



#draw

plt.plot(train_X,train_Y,"+")

plt.plot(train_X,train_X.dot(w_value)+b_value)

plt.show()

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

结果： 





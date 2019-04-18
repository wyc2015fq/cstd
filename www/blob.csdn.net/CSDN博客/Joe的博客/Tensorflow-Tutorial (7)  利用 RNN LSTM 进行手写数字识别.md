# Tensorflow - Tutorial (7) : 利用 RNN/LSTM 进行手写数字识别 - Joe的博客 - CSDN博客





2017年03月10日 15:34:31[Joe-Han](https://me.csdn.net/u010089444)阅读数：19454








# **1. 常用类**

## **class tf.contrib.rnn.BasicLSTMCell**

**BasicLSTMCell** 是最简单的一个LSTM类，没有实现clipping，projection layer，peep-hole等一些LSTM的高级变种，仅作为一个基本的basicline结构存在，如果要使用这些高级变种，需用**class tf.contrib.rnn.LSTMCell**这个类。

**使用方式：**
`lstm = rnn.BasicLSTMCell(lstm_size, forget_bias=1.0, state_is_tuple=True)`
> 
Args:
- 
num_units: int, The number of units in the LSTM cell. 

- 
forget_bias: float, The bias added to forget gates. 

- 
state_is_tuple: If True, accepted and returned states are 2-tuples of the c_state and m_state. If False, they are concatenated along the column axis. The latter behavior will soon be deprecated.

- 
activation: Activation function of the inner states.



**说明：**
- num_units 是指一个Cell中神经元的个数，并不是循环层的Cell个数。这里有人可能会疑问：循环层的Cell数目怎么表示？答案是通过如下代码中的 time_step_size确定（X_split 中划分出的arrays数量为循环层的Cell个数）：
`    X_split = tf.split(XR, time_step_size, 0)`- 在任意时刻 $t$ ，LSTM Cell会产生两个内部状态 $c_{t}$和$h_{t}$ （**关于RNN与LSTM的介绍可参考：**[**循环神经网络与LSTM**](http://blog.csdn.net/u010089444/article/details/55211653)）。当state_is_tuple=True时，上面讲到的状态$c_{t}$和$h_{t}$ 就是分开记录，放在一个二元tuple中返回，如果这个参数没有设定或设置成False，两个状态就按列连接起来返回。官方说这种形式马上就要被deprecated了，所有我们在使用LSTM的时候要加上state_is_tuple=True。

## **class tf.contrib.rnn.DropoutWrapper**

RNN中的dropout和cnn不同，在RNN中，时间序列方向不进行dropout，也就是说从t-1时刻的状态传递到t时刻进行计算时，这个中间不进行memory的dropout；如下图所示，Dropout仅应用于虚线方向的输入，即仅针对于上一层的输出做Dropout。


![图 1](https://img-blog.csdn.net/20170309214740269?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因此，我们在代码中定义完Cell之后，在Cell外部包裹上dropout，这个类叫DropoutWrapper，这样我们的Cell就有了dropout功能！

`lstm = tf.nn.rnn_cell.DropoutWrapper(lstm, output_keep_prob=keep_prob)`
> 
Args:
- 
cell: an RNNCell, a projection to output_size is added to it.

- 
input_keep_prob: unit Tensor or float between 0 and 1, input keep probability; if it is float and 1, no input dropout will be added.

- 
output_keep_prob: unit Tensor or float between 0 and 1, output keep probability; if it is float and 1, no output dropout will be added.

- 
seed: (optional) integer, the randomness seed.



## **class tf.contrib.rnn.MultiRNNCell**

如果希望整个网络的层数更多（例如上图表示一个两层的RNN，第一层Cell的output还要作为下一层Cell的输入），应该堆叠多个LSTM Cell，tensorflow给我们提供了MultiRNNCell，因此堆叠多层网络只生成这个类即可：
`lstm = tf.nn.rnn_cell.MultiRNNCell([lstm] * num_layers, state_is_tuple=True)`
# **2. 代码**

MNIST数据集的格式与数据预处理代码 input_data.py的讲解请参考 :[Tutorial (2)](http://blog.csdn.net/u010089444/article/details/52495254)

```
# -*- coding: utf-8 -*-
import tensorflow as tf
from tensorflow.contrib import rnn

import numpy as np
import input_data

# configuration
#                        O * W + b -> 10 labels for each image, O[? 28], W[28 10], B[10]
#                       ^ (O: output 28 vec from 28 vec input)
#                       |
#      +-+  +-+       +--+
#      |1|->|2|-> ... |28| time_step_size = 28
#      +-+  +-+       +--+
#       ^    ^    ...  ^
#       |    |         |
# img1:[28] [28]  ... [28]
# img2:[28] [28]  ... [28]
# img3:[28] [28]  ... [28]
# ...
# img128 or img256 (batch_size or test_size 256)
#      each input size = input_vec_size=lstm_size=28

# configuration variables
input_vec_size = lstm_size = 28 # 输入向量的维度
time_step_size = 28 # 循环层长度

batch_size = 128
test_size = 256

def init_weights(shape):
    return tf.Variable(tf.random_normal(shape, stddev=0.01))


def model(X, W, B, lstm_size):
    # X, input shape: (batch_size, time_step_size, input_vec_size)
    # XT shape: (time_step_size, batch_size, input_vec_size)
    XT = tf.transpose(X, [1, 0, 2])  # permute time_step_size and batch_size,[28, 128, 28]

    # XR shape: (time_step_size * batch_size, input_vec_size)
    XR = tf.reshape(XT, [-1, lstm_size]) # each row has input for each lstm cell (lstm_size=input_vec_size)

    # Each array shape: (batch_size, input_vec_size)
    X_split = tf.split(XR, time_step_size, 0) # split them to time_step_size (28 arrays),shape = [(128, 28),(128, 28)...]


    # Make lstm with lstm_size (each input vector size). num_units=lstm_size; forget_bias=1.0
    lstm = rnn.BasicLSTMCell(lstm_size, forget_bias=1.0, state_is_tuple=True)

    # Get lstm cell output, time_step_size (28) arrays with lstm_size output: (batch_size, lstm_size)
    # rnn..static_rnn()的输出对应于每一个timestep，如果只关心最后一步的输出，取outputs[-1]即可
    outputs, _states = rnn.static_rnn(lstm, X_split, dtype=tf.float32)  # 时间序列上每个Cell的输出:[... shape=(128, 28)..]

    # Linear activation
    # Get the last output
    return tf.matmul(outputs[-1], W) + B, lstm.state_size # State size to initialize the stat

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True) # 读取数据

# mnist.train.images是一个55000 * 784维的矩阵, mnist.train.labels是一个55000 * 10维的矩阵
trX, trY, teX, teY = mnist.train.images, mnist.train.labels, mnist.test.images, mnist.test.labels

# 将每张图用一个28x28的矩阵表示,(55000,28,28,1)
trX = trX.reshape(-1, 28, 28) 
teX = teX.reshape(-1, 28, 28) 

X = tf.placeholder("float", [None, 28, 28])
Y = tf.placeholder("float", [None, 10])

# get lstm_size and output 10 labels
W = init_weights([lstm_size, 10])  # 输出层权重矩阵28×10
B = init_weights([10])  # 输出层bais

py_x, state_size = model(X, W, B, lstm_size)

cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=py_x, labels=Y))
train_op = tf.train.RMSPropOptimizer(0.001, 0.9).minimize(cost)
predict_op = tf.argmax(py_x, 1)

session_conf = tf.ConfigProto()
session_conf.gpu_options.allow_growth = True

# Launch the graph in a session
with tf.Session(config=session_conf) as sess:
    # you need to initialize all variables
    tf.global_variables_initializer().run()

    for i in range(100):
        for start, end in zip(range(0, len(trX), batch_size), range(batch_size, len(trX)+1, batch_size)):
            sess.run(train_op, feed_dict={X: trX[start:end], Y: trY[start:end]})

        test_indices = np.arange(len(teX))  # Get A Test Batch
        np.random.shuffle(test_indices)
        test_indices = test_indices[0:test_size]

        print(i, np.mean(np.argmax(teY[test_indices], axis=1) ==
                         sess.run(predict_op, feed_dict={X: teX[test_indices]})))
```


![图 1](https://img-blog.csdn.net/20170310145732849?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# **3. 参考资料**
- [https://medium.com/@erikhallstrm/hello-world-rnn-83cd7105b767](https://medium.com/@erikhallstrm/hello-world-rnn-83cd7105b767)
- [http://www.cnblogs.com/wuzhitj/p/6434171.html](http://www.cnblogs.com/wuzhitj/p/6434171.html)
- [http://blog.csdn.net/mydear_11000/article/details/52414342](http://blog.csdn.net/mydear_11000/article/details/52414342)




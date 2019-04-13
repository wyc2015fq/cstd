
# 如何让TensorFlow模型运行提速36.8% - lyc_yongcai的博客 - CSDN博客


2017年06月14日 15:39:38[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：6274


在训练TensorFlow模型的时候，我们传统的做法是在每个Epoch将数据通过feed_dict导入到session中，即不断地从Python到C++之间来回切换，这种做法十分不高效。而且，训练操作与导入数据操作都是属于同一个主线程，它们属于同步操作，训练操作必须等待导入数据操作完成以后才开始执行。这种做法带来的就是运行效率底下，我们应该赶紧抛弃，取而代之的是TensorFlow中的线程和队列。下面，我将通过实例代码来说明如何使用线程和队列以及其如何为程序效率提高36%。
为了比较，我写了一段传统的基于TensorFlow训练神经网络的代码。构建了一个隐含层神经元个数为128的**双向动态LSTM网络**（BLSTM），输入数据是**400**个样本，每个样本的维度是**[128, 512]**，即时间长度为128，特征长度为512，而对应标签是将这个序列进行二分类，得到0或1。
x和y是标准的输入输出，网络的预测输出则是在BLSTM顶端加了一个线性MLP，最后通过sigmoid层并输出交叉熵误差。在这个程序里，数据都是通过feed_dict导入的，代码如下：

```python
#coding:utf-8
import time
import tensorflow as tf
from tensorflow.contrib.rnn import LSTMCell
'''
构建了一个隐含层神经元个数为128的双向动态LSTM网络（BLSTM），
输入数据是400个样本，每个样本的维度是[128, 512]，即时间长度为128，特征长度为512，
而对应标签是将这个序列进行二分类，得到0或1。
'''
time_length = 128
batch_size = 400
feature_size = 512
hidden_size = 128
# 随机产生以均值为0 方差为1 的[time_length, batch_size, feature_size]数据
x = tf.random_normal([time_length,
    batch_size, feature_size], mean=0, stddev=1)
y = tf.reduce_mean(tf.reduce_sum(x, axis=0), axis=1, keep_dims=True)
y = tf.cast(tf.greater(y, 0), tf.int32)
inputs = tf.placeholder(tf.float32,
     shape=[time_length, batch_size, feature_size])
labels = tf.placeholder(tf.int32, shape=[batch_size, 1])
sequence_length = tf.Variable([time_length]*batch_size, dtype=tf.int32)
cell_fw = LSTMCell(num_units=hidden_size)
cell_bw = LSTMCell(num_units=hidden_size)
outputs, state = tf.nn.bidirectional_dynamic_rnn(
      cell_fw=cell_fw,
      cell_bw=cell_bw,
      inputs=inputs,
      sequence_length=sequence_length,
      dtype=tf.float32,
      time_major=True)
outputs_fw, outputs_bw = outputs
outputs = tf.concat([outputs_fw, outputs_bw], axis=2)
outputs = tf.reduce_mean(outputs, axis=0)
outputs = tf.contrib.layers.fully_connected(
            inputs=outputs,
            num_outputs=1,
            activation_fn=None)

losses_op = tf.nn.sigmoid_cross_entropy_with_logits(None, tf.cast(labels, tf.float32), outputs)
losses_op = tf.reduce_mean(losses_op)
y_pred = tf.cast(tf.greater(outputs, 0), tf.int32)
accuracy = tf.reduce_mean(tf.cast(tf.equal(y_pred, y), tf.float32))
# adam = tf.train.AdamOptimizer(0.001)
train_op = tf.train.AdamOptimizer(0.001).minimize(losses_op, name="train_op")
t1 = time.time()
with tf.Session() as sess:
  sess.run(tf.global_variables_initializer())
  for i in range(50):
    data_x, data_y = sess.run([x, y])
    _, losses, acc = sess.run([train_op, losses_op, accuracy],
                              feed_dict={inputs: data_x, labels: data_y})
    print('epoch:%d, loss: %f,   accuracy: %f' % (i, losses, acc))
print('time:', (time.time()-t1))
```
将这段代码在我电脑的单一GPU上执行（GPU为TITAN X系列），运行结果如下所示，可以看到，整个程序花费了58.94秒。![](http://mmbiz.qpic.cn/mmbiz_png/sfAgOD009UXm8TfbVVgoMYRJcs8EkK6zo0VXtTZxOKq0vSaqqDDx6xRCic0Vfl1CDbuGpW3crcnKdbwcJbTia1og/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
接下来我使用TensorFlow的queue来优化这个程序，和TensorFlow中的其他对象一样，queue也只是TensorFlow中的一个节点，其可以将feed_dict操作转化为异步计算。从tensorflow.python.ops.data_flow_ops的源码中可以看到，所有的Queue类都是继承自QueueBase父类，继承自QueueBase类的子类有：
RandomShuffleQueue 随机队列
FIFOQueue 先入先出
PaddingFIFOQueue 可以自带padding
PriorityQueue 优先队列
并且每个类都有以下几个常用的方法：
enqueue() 即向queue中压入数据
dequeue() 即从queue中弹出数据
enqueue_many() 即向queue中压入多个数据
dequeue_many() 即从queue中弹出多个数据
当然，单个这个Queue类的方法好像并没有什么吸引人之处，因为就它自身而言，如果我们启动了一个queue，它是生产者消费者模型，如果只是在单一线程下面工作，那仍然是无济于事的，就好比很多个厨师一起做菜，然而却只有一个灶台可以利用。因此，要想提高运行效率，我们必须要让enqueue和dequeue分别处在不同线程上，这个时候就需要用到QueueRunner类和Coordinator类了。
QueueRunner类和Coordinator类的作用是处理队列的操作、保持同步，并且这些操作都是在不同的线程上面。根据tensorflow.python.training中的源码可知，QueueRunner需要一个队列和入队的操作（可以是很多个操作），然后根据session即可创造出很多个执行入队操作的线程，然后调用tf.train.add_queue_runner方法即可将queue_runner添加到TensorFlow QUEUE_RUNNERS集合中去，再调用tf.train.start_queue_runner方法即可启动所有线程。这样，就可以调用上层API
 Coordinate来执行线程了。
Coordinator类可以让多个线程停止，它主要有三个方法：tf.train.Coordinator.should_stop 确认线程是否应该停止
tf.train.Coordinator.request_stop 要求线程停止
tf.train.Coordinator.join 要求等待线程结束
代码如下：

```python
import time
import tensorflow as tf
from tensorflow.contrib.rnn import LSTMCell
time_length = 128
batch_size = 400
feature_size = 512
hidden_size = 128
## prepare data
x = tf.random_normal([time_length, batch_size, feature_size], mean=0, stddev=1)
q = tf.FIFOQueue(capacity=4, dtypes=tf.float32)
enqueue_op = q.enqueue(x)
num_threads = 1
qr = tf.train.QueueRunner(q, [enqueue_op] * num_threads)
tf.train.add_queue_runner(qr)
inputs = q.dequeue()
inputs.set_shape(x.get_shape())
y = tf.reduce_mean(tf.reduce_sum(inputs, axis=0), axis=1, keep_dims=True)
labels = tf.cast(tf.greater(y, 0), tf.int32)
## build model
sequence_length = tf.Variable([time_length]*batch_size, dtype=tf.int32)
cell_fw = LSTMCell(num_units=hidden_size)
cell_bw = LSTMCell(num_units=hidden_size)
outputs, state = tf.nn.bidirectional_dynamic_rnn(
      cell_fw=cell_fw,
      cell_bw=cell_bw,
      inputs=inputs,
      sequence_length=sequence_length,
      dtype=tf.float32,
      time_major=True)
outputs_fw, outputs_bw = outputs
outputs = tf.concat([outputs_fw, outputs_bw], axis=2)
outputs = tf.reduce_mean(outputs, axis=0)
outputs = tf.contrib.layers.fully_connected(
            inputs=outputs,
            num_outputs=1,
            activation_fn=None)
losses_op = tf.nn.sigmoid_cross_entropy_with_logits(None, tf.cast(labels, tf.float32), outputs)
losses_op = tf.reduce_mean(losses_op)
y_pred = tf.cast(tf.greater(outputs, 0), tf.int32)
accuracy = tf.reduce_mean(tf.cast(tf.equal(y_pred, labels), tf.float32))
# adam = tf.train.AdamOptimizer(0.001)
train_op = tf.train.AdamOptimizer(0.001).minimize(losses_op, name="train_op")
t1 = time.time()
with tf.Session() as sess:
  sess.run(tf.global_variables_initializer())
  coord = tf.train.Coordinator()
  threads = tf.train.start_queue_runners(coord=coord)
  for i in range(50):
    _, losses, acc = sess.run([train_op, losses_op, accuracy])
    print('epoch:%d, loss: %f' % (i, losses))
  coord.request_stop()
  coord.join(threads)
  print("Time taken: %f" % (time.time() - t1))
```
运行结果如下图所示：![](http://mmbiz.qpic.cn/mmbiz_png/sfAgOD009UXm8TfbVVgoMYRJcs8EkK6zArtfiaaVpN5uM9THSYnjtN1mHdrc1JayIylCBj4jUibqfwibDPwoTqxiaQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)

**可以看到这里我们同样的程序，同样的数据，同样的GPU，只花费了37.25秒，由此可计算出该方法竟然比传统的feed_dict做法提速了36.8%!**



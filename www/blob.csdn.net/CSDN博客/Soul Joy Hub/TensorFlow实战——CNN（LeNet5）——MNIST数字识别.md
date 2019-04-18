# TensorFlow实战——CNN（LeNet5）——MNIST数字识别 - Soul Joy Hub - CSDN博客

2017年06月04日 21:20:21[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：4763


本文地址：
[http://blog.csdn.net/u011239443/article/details/72861591](http://blog.csdn.net/u011239443/article/details/72861591)

我们来实现下不标准的LeNet模型：

train：[https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter6/mnist_train_6_4_1.py](https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter6/mnist_train_6_4_1.py)

inference：[https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter6/mnist_inference_6_4_1.py](https://github.com/xiaoyesoso/TensorFlowinAction/blob/master/InActionB1/chapter6/mnist_inference_6_4_1.py)

# train

train部分和[《TensorFlow实战——DNN——MNIST数字识别 》](http://blog.csdn.net/u011239443/article/details/71173351)中没太多的不同。
- 首先，原始学习率要降低：

```python
LEARNING_RATE_BASE = 0.01
```
- 第二点，x是一个四维的矩阵：

```python
x = tf.placeholder(tf.float32,[BATCH_SIZE,mnist_inference_6_4_1.IMAGE_SIZE,mnist_inference_6_4_1.IMAGE_SIZE,mnist_inference_6_4_1.NUM_CHANNELS],name='x-input')
```

`mnist_inference_6_4_1.NUM_CHANNELS`为图片的深度。
- xs也要换成四维矩阵：

```python
xs,ys = mnist.train.next_batch(BATCH_SIZE)
            reshaped_xs = np.reshape(xs,(BATCH_SIZE,mnist_inference_6_4_1.IMAGE_SIZE,mnist_inference_6_4_1.IMAGE_SIZE,mnist_inference_6_4_1.NUM_CHANNELS))
            _,loss_value,step = sess.run([train_op,loss,global_step],feed_dict={x:reshaped_xs,y_:ys})
```

# inference

## layer1

```python
with tf.variable_scope('layer1-conv1'):
        conv1_weights = tf.get_variable("weight",[CONV1_SIZE,CONV1_SIZE,NUM_CHANNELS,CONV1_DEEP],initializer=tf.truncated_normal_initializer(stddev=0.1))
        conv1_biases = tf.get_variable("bias",[CONV1_DEEP],initializer=tf.constant_initializer(0.0))

        conv1 = tf.nn.conv2d(input_tensor,conv1_weights,strides=[1,1,1,1],padding='SAME')
        relu1 = tf.nn.relu(tf.nn.bias_add(conv1,conv1_biases))
```
- 首先我们看下`strides`参数：

> 
strides: A list of `ints`.

1-D of length 4.  The stride of the sliding window for each dimension

of `input`. Must be in the same order as the dimension specified with format.

`strides`代表着移动的步长，它的顺序必须和`input_tensor`一样，及`[BATCH_SIZE,mnist_inference_6_4_1.IMAGE_SIZE,mnist_inference_6_4_1.IMAGE_SIZE,mnist_inference_6_4_1.NUM_CHANNELS]`。由于`BATCH_SIZE`和`mnist_inference_6_4_1.NUM_CHANNELS`上肯定是一步一步的移动的，所以数组的第一个值和最后一个值肯定为1。
- `padding='SAME'`，表示填充0，不改变`Image`的大小。
- 注意`tf.nn.bias_add(conv1,conv1_biases)`，并不是`conv1`与`conv1_biases`直接相加。

## layer2

```python
with tf.name_scope('layer2-pool1'):
        pool1 = tf.nn.max_pool(relu1,ksize=[1,2,2,1],strides=[1,2,2,1],padding='SAME')
```

`max_pool`表示是取最大值的池化层。

我们来看下参数`ksize`：

> 
ksize: A list of ints that has length >= 4.  The size of the window for

each dimension of the input tensor.

窗口各个维度多大小。由于池化层窗口只在当前数据中的当前深度做，所以数组的第一个值和最后一个值肯定为1。

## layer5

`layer3`和`layer4`前面的类似，我们跳过它们来看`layer5`：

```python
pool_shape = pool2.get_shape().as_list()
    nodes = pool_shape[1] * pool_shape[2] * pool_shape[3]
    reshaped = tf.reshape(pool2,[pool_shape[0],nodes])

    with tf.variable_scope('layer5-fc1'):
        fc1_weights = tf.get_variable("weight", [nodes,FC_SIZE],
                                        initializer=tf.truncated_normal_initializer(stddev=0.1))
        if regularizer != None:
            tf.add_to_collection('losses',regularizer(fc1_weights))

        fc1_biases = tf.get_variable("bias", [FC_SIZE], initializer=tf.constant_initializer(0.1))
        fc1 = tf.nn.relu(tf.matmul(reshaped,fc1_weights) + fc1_biases)
        if train:
            fc1 = tf.nn.dropout(fc1,0.5)
```
- `get_shape().as_list()`能得到`pool2`的`size`。
$pool\_shape[1] * pool\_shape[2] * pool\_shape[3] = 长×宽×深$，这相当把长方体拉成一条直线。`pool_shape[0]`为`BATCH_SIZE`
- `dropout`为了赋值过拟合，可以似的一定比例的输出变为0。

其余部分就是全连接神经网络了，`layer6`也和`layer5`类似。

结果：

```
After 1 training step(s), loss is 6.06818
After 101 training step(s), loss is 2.24668
After 201 training step(s), loss is 1.65929
After 301 training step(s), loss is 1.30799
After 401 training step(s), loss is 1.3613
After 501 training step(s), loss is 0.960646
After 601 training step(s), loss is 0.954722
After 701 training step(s), loss is 0.883449
After 801 training step(s), loss is 0.870421
After 901 training step(s), loss is 0.905906
After 1001 training step(s), loss is 0.932337
```

![这里写图片描述](https://img-blog.csdn.net/20170625203225853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# cnn系列文章六  --deeplearning.ai  andrew ng课上网络结构实现 - 墨流觞的博客 - CSDN博客





2018年09月12日 12:08:53[墨氲](https://me.csdn.net/dss_dssssd)阅读数：79标签：[python																[DeepLearning																[CNN																[tensorflow](https://so.csdn.net/so/search/s.do?q=tensorflow&t=blog)
个人分类：[python深度学习](https://blog.csdn.net/dss_dssssd/article/category/8032217)





[cnn系列文章五 -tf.nn.conv2d 和tf.nn.max_pool讲解](https://blog.csdn.net/dss_dssssd/article/details/82659501)

惯例, 分享一句歌词哦：

> 
《天涯》 

  为君清宵歌一曲 

  一曲天涯梦魂远
### 参考

[softmax](http://www.tensorfly.cn/tfdoc/api_docs/python/nn.html#softmax)

[flatten](https://blog.csdn.net/qq_28224015/article/details/80875665)

### 基本函数封装

```python
def conv_2d(input_data, filter_shape, bias_shape, strides=None, padding='VALID', activation_function=0):
    '''

    :param input_data: [batch, in_height, in_weight, in_channels]
    :param filter_shape: [height, width, in_channels, out_channels]
    :param bias_shape: bias的长度与filter_shape最后一个数相同，
    :param strides: a list of ints, 默认[1, 1,1,1]
    :param padding:
    :param activation_function: 0 ->relu 1->sigmoid 2->tanh 3->无激励函数
    :return: [batch, out_height, out_width, out_channels]
    '''
    # 1. 执行滤波操作 tf.nn.conv2d
    # 2. 利用python的broadcast(广播机制)加上bias
    # 3. 激活函数（activation function） 默认不经过激活函数，


    # 默认 strides = [1,1,1,1]
    if strides is None:
        strides = [1, 1, 1,1]

    # 根据论文，初始参数的方差与输入到神经元个数关系： variance = 2 / num
    in_data_num = filter_shape[0] * filter_shape[1] * filter_shape[1]
    fliter_init = tf.random_normal_initializer(stddev=(2.0 / in_data_num) ** 0.5)

    fliter = tf.get_variable('filter', filter_shape, initializer=fliter_init)
    bias_init = tf.constant_initializer(value=0)
    b = tf.get_variable('bias', bias_shape, initializer=bias_init)

    conv_out = tf.nn.conv2d(input_data,fliter,strides=strides, padding=padding)

    add_bias = tf.nn.bias_add(conv_out, b)

    if activation_function == 2:
        return tf.nn.tanh(add_bias)
    elif activation_function == 1:
        return tf.nn.sigmoid(add_bias)     
    elif activation_function == 0:
        return tf.nn.relu(add_bias)
    elif activation_function == 3:
        return add_bias



def max_pool(input_data, k_size=2, k_strides=2, padding='VALID'):
    '''

    :param input_data: [batch, in_height, in_weight, channels]
    :param k_size: 池化区域的大小
    :param k_strides: 在每个维度上的步长， 一般在batch和channels上的步长为 1
    :param padding: 'VALID' p=0 ; 'SAME'  输入输出维度相同
    :return: [batch, out_height, ot_weight, channels]
    '''
    return tf.nn.max_pool(input_data,ksize=[1, k_size, k_size, 1],
                   strides=[1, k_strides, k_strides, 1], padding=padding)


def layer(input_data, weight_shape, bias_shape, activation_function=0):
    '''

    :param input_data: [batch, in_neurons]
    :param weight_shape: [in_neurons, out_neurons]
    :param bias_shape: [out_neurons]
    :param activation_function  0 ->relu 1->sigmoid 2->tanh 3->无激励函数
    :return: [batch, out_neurons]
    '''
    weight_stddev = (2.0 / weight_shape[0]) ** 0.5
    w_init = tf.random_normal_initializer(stddev=weight_stddev)
    bias_init = tf.constant_initializer(value=0.0)

    W = tf.get_variable('W', weight_shape, initializer=w_init)
    b = tf.get_variable('b', bias_shape, initializer=bias_init)

    out = tf.matmul(input_data, W) + b
    if activation_function == 2:
        return tf.nn.tanh(out)
    elif activation_function == 1:
        return tf.nn.sigmoid(out)
    elif activation_function == 0:
        return tf.nn.relu(out)
    elif activation_function == 3:
        return out
```

### 课程网络结构实现：

结构如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20180912120724540?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
import tensorflow as tf

def cnn_architecture(x, keep_prob=0.5):
    x = tf.reshape(x, shape=[-1, 32, 32, 3])


    with tf.variable_scope("layer_1"):
        # TODO: conv_1: input:[ batch, 32, 32, 3] -> output [batch, 28, 28, 6]
        conv_1 = conv_2d(x,filter_shape=[5, 5, 3, 6], bias_shape=[6], activation_function=0)
        # TODO: max_pool_1: input: [batch, 28, 28, 6] -> output [batch, 14, 14, 6]
        pool_1 = max_pool(conv_1)

    with tf.variable_scope("layer_2"):
        # TODO: conv_2: input: [batch, 14, 14, 6] ->output[ batch, 10, 10, 16]
        conv_2 = conv_2d(pool_1, filter_shape=[5, 5, 6, 16], bias_shape=[16], activation_function=0)
        #TODO: max_pool_2: input:[batch, 10, 10 ,16] -> output [batch, 5, 5, 16]
        pool_2 = max_pool(conv_2)

    with tf.variable_scope('FC3'):
        # TODO : flatten pool_2  [batch, 5, 5,  16] -> [batch, 400]
        _flatten = tf.contrib.layers.flatten(pool_2)
        # TODO: fully connection  [batch, 400] -> [batch, 120]
        fc_3 = layer(_flatten, [400, 120], [120], activation_function=0)

        # apply dropout to avoid overfitting
        fc_3_drop = tf.nn.dropout(fc_3, keep_prob=keep_prob)

    with tf.variable_scope('FC4'):
        # TODO:fully connection  [batch, 120] -> [batch, 84]
        fc_4 = layer(fc_3_drop, [120, 84], [84], activation_function=0)

        # apply dropout to avoid overfitting
        fc_4_drop = tf.nn.dropout(fc_4, keep_prob=keep_prob)

    with tf.variable_scope('Softmax'):
        # TODO: softmax classfier layer [batch, 84] -> [batch, 10]
        out = layer(fc_4_drop, [84, 10], [10], activation_function=3)
        softmax_out = tf.nn.softmax(out)
    return softmax_out
```](https://so.csdn.net/so/search/s.do?q=CNN&t=blog)](https://so.csdn.net/so/search/s.do?q=DeepLearning&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)





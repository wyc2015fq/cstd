# cnn系列文章五   --tf.nn.conv2d 和 tf.nn.max_pool详解 - 墨流觞的博客 - CSDN博客





2018年09月12日 12:05:56[墨氲](https://me.csdn.net/dss_dssssd)阅读数：153








[cnn系列文章四 –池化层和简单卷积网络示例](https://blog.csdn.net/dss_dssssd/article/details/82659440)

你懂得，分享一句歌词哦：

> 
《故梦》 

  谁踩过枯枝轻响，萤火绘着画屏香 

  为谁拢一袖芬芳，红叶的信笺情意绵长
接下来主要讲解tensorflow有关cnn构建的问题了。
- 
tf.nn.conv2d(input, filter, padding, use_cundnn_on_gpu=True, data_format=’NHWC’, name=None)
- input  [batch, in_height, in_width, in_channels]
- filter [height, width, in_channels, out_channels]
- padding:  
- ’VALID’ : p=0
- ‘SAME’: 输入输出相等

- strides: 各个维度上([batch, height, width, channels])的跨度,一般设置为：[1, strides, strides, 1]

output: [batch, out_height, out_width, out_channels]



$out_{height} = \lfloor \frac {in_{height} + 2 \times p - fliter_{height}}{strides_{height}}  +1\rfloor$


$out_{width} = \lfloor \frac {in_{width} + 2 \times p -fliter_{width}}{strides_{width}}  +1\rfloor$
- 
tf.nn.max_pool(input, ksize, strides, padding, data_format=”NHWC”,name=None)
- input: [batch, in_height, in_width, in_channels]
- ksize: 一般为[1, height, width, 1], 不在batch和channels上做池化，故设为1
- strides: 在每个维度上滑动的步长，一般[1, strides, strides, 1]
- padding: 一般为’VALID’,即p=0;也可以’SAME’

output: [batch, out_height, out_width, in_channels]



$out_{height} = \lfloor \frac {in_{height} + 2 \times p - ksize_{height}}{strides_{height}}  +1\rfloor$


$out_{width} = \lfloor \frac {in_{width} + 2 \times p -ksizer_{width}}{strides_{width}}  +1\rfloor$


```python
import tensorflow as tf


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
    # 根据论文，初始参数的方差与输入到神经元个数关系： variance = 2 / num
    in_data_num = filter_shape[0] * filter_shape[1] * filter_shape[1]
    fliter_init = tf.random_normal_initializer(stddev=(2.0 / in_data_num) ** 0.5)

    fliter = tf.get_variable('filter', filter_shape, initializer=fliter_init)
    bias_init = tf.constant_initializer(value=0)
    b = tf.get_variable('bias', bias_shape, initializer=bias_init)

    conv_out = tf.nn.conv2d(input_data,fliter,strides=strides, padding=padding)

    add_bias = tf.nn.bias_add(conv_out, b)

    if activation_function == 3:
        return tf.nn.tanh(add_bias)
    elif activation_function == 2:
        return tf.nn.sigmoid(add_bias)     
    elif activation_function == 1:
        return tf.nn.relu(add_bias)
    else:
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
```

[接下来我们将使用上面两个函数构建课程中基于lenet-5的网络结构](https://blog.csdn.net/dss_dssssd/article/details/82659523)







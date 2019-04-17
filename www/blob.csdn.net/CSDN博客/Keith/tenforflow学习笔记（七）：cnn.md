# tenforflow学习笔记（七）：cnn - Keith - CSDN博客





2016年10月22日 10:57:59[ke1th](https://me.csdn.net/u012436149)阅读数：5690
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









### 卷积函数

1.tf.nn.conv2d(input, filter, strides, padding, use_cudnn_on_gpu=None, data_format=None, name=None) 

2.tf.nn.depthwise_conv2d(input, filter, strides, padding, name=None) 

3.tf.nn.separable_conv2d(input, depthwise_filter, pointwise_filter, strides, padding, name=None) 

4.tf.nn.atrous_conv2d(value, filters, rate, padding, name=None)
#### tf.nn.conv2d(input, filter, strides, padding, use_cudnn_on_gpu=None, data_format=None, name=None)

```python
def conv2d(input, filter, strides, padding, use_cudnn_on_gpu=None, data_format=None, name=None):
#input:shape [batch_size, in_height, in_width, in_channels].channels的意思是就是，如果你输入的是RGB图像，channel就是3
#filter:shape [filter_height, filter_width, in_channels, out_channels], out_channels表示输出几张feature map
#strides:A list of ints that has length >= 4. The stride of the sliding window for each dimension of the input tensor.
#stride:stride=[1,h_stride,v_stride,1] ，strides[0]=strides[3]=1!!!!!!第0个是batch中的样本，第三个是channel
#padding:两种形式"VALID"和"SAME","VALID"不会去补0,"SAME"和"VALID"一样运算，不够的时候，会补0，不知为啥tensorflow没有"FULL"
#输出:[batch_size, out_height, out_width, out_channels]
```

![conv2d的输入输出](https://img-blog.csdn.net/20161022105911841)
图一：conv2d的输入输出
**每三个filter看作一组，每组中的权值不是共享的，组之间也不是共享的**


```python
def depthwise_conv2d(input, filter, strides, padding, name=None):
#input:shape [batch_size, in_height, in_width, in_channels]
#filter:shape [filter_height, filter_width, in_channels, channel_multiplier]
#strides:同上
#padding:同上
#return:shape [batch, out_height, out_width, in_channels * channel_multiplier]
```

![depthwise_conv2d的输入输出](https://img-blog.csdn.net/20161022110028343)
图二：depthwise_conv2d的输入输出
**各filter之间权值不共享**
**剩下两个函数暂时还没研究**
**tf.nn.bias_add(input, biases). 求完卷积后，用这个加上bias**


```python
def bias_add(input, biases):
"""Adds `bias` to `value`.
  This is (mostly) a special case of `tf.add` where `bias` is restricted to 1-D.
  Broadcasting is supported, so `value` may have any number of dimensions.
  Unlike `tf.add`, the type of `bias` is allowed to differ from `value` in the
  case where both types are quantized.
  Args:
    value: A `Tensor` with type `float`, `double`, `int64`, `int32`, `uint8`,
      `int16`, `int8`, `complex64`, or `complex128`.
    bias: A 1-D `Tensor` with size matching the last dimension of `value`.
      Must be the same type as `value` unless `value` is a quantized type,
      in which case a different quantized type may be used.
    data_format: A string. 'NHWC' and 'NCHW' are supported.
    name: A name for the operation (optional).
```

### tf.nn.atrous_conv2d(value, filters, rate, padding, name=None)
- rate 等价于 dilation，什么叫dilation 
[详见conv-gif](https://github.com/vdumoulin/conv_arithmetic/blob/master/README.md)

### POOLING函数

1.tf.nn.avg_pool(value, ksize, strides, padding, data_format=’NHWC’, name=None) 

2.tf.nn.max_pool(value, ksize, strides, padding, data_format=’NHWC’, name=None) 

3.tf.nn.max_pool_with_argmax(input, ksize, strides, padding, Targmax=None, name=None)
```python
def avg_pool(value, ksize, strides, padding, data_format='NHWC', name=None):
#value:shape [batch, height, width, channels]
#ksize:A list of ints that has length >= 4. The size of the window for each dimension of the input tensor.
#strides:A list of ints that has length >= 4. The stride of the sliding window for each dimension of the input tensor.一般为[1, h_stride, v_stride, 1]
#return: [batch, out__height, out_width, out_channels]

#max_pool与avg_pool相似
```

## SAME 卷积 tf 的是如何pad 的

左和上： `需要 pad 的值 // 2`

右和下： 减去上面的值

## SAME 与 VALID输出size计算
- 
`SAME`:  


$o\_h=ceil(\frac{i\_h}{stride[1]}), o\_w=ceil(\frac{i\_w}{stride[2]})$

- 
`VALID`: 


$o\_h = floor(\frac{i\_h-k\_h}{stride[1]}), o\_w=floor(\frac{i\_w-k\_w}{stride[2]})$









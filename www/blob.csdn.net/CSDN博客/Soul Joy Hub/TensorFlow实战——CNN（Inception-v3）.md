# TensorFlow实战——CNN（Inception-v3） - Soul Joy Hub - CSDN博客

2017年06月10日 21:05:31[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：5078


本文：[http://blog.csdn.net/u011239443/article/details/73008131](http://blog.csdn.net/u011239443/article/details/73008131)

![这里写图片描述](https://img-blog.csdn.net/20170610194512901?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们之前介绍的神经网络层与层之间都是一对一的，而`Inception-v3`模型存在一对多和多对一。
- 一对多：用不同或者相同的过滤器过滤出多个层
- 多对一：将多层合并，即深度会相加。

tensorflow github 中有`Inception-v3`模型的实现：[https://github.com/tensorflow/tensorflow/blob/fc1567c78b3746b44aa50373489a767afbb95d2b/tensorflow/contrib/slim/python/slim/nets/inception_v3.py](https://github.com/tensorflow/tensorflow/blob/fc1567c78b3746b44aa50373489a767afbb95d2b/tensorflow/contrib/slim/python/slim/nets/inception_v3.py)

由于大部分代码雷同，所以我们就讲解最后一个`Mixed`的代码：

![这里写图片描述](https://img-blog.csdn.net/20170610195438564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

首先我们来介绍下一个可以来帮我们创建卷积神经网络层的工具：`layers.conv2d`。它可以很简洁的帮我们创建卷积神经网络卷积层，它本质是`convolution`函数，我们可以看下`convolution`函数：

```python
def convolution(inputs,
                num_outputs,
                kernel_size,
                stride=1,
                padding='SAME',
                data_format=None,
                rate=1,
                activation_fn=nn.relu,
                normalizer_fn=None,
                normalizer_params=None,
                weights_initializer=initializers.xavier_initializer(),
                weights_regularizer=None,
                biases_initializer=init_ops.zeros_initializer(),
                biases_regularizer=None,
                reuse=None,
                variables_collections=None,
                outputs_collections=None,
                trainable=True,
                scope=None):

  """
  Args:
    inputs: a Tensor of rank N+2 of shape
      `[batch_size] + input_spatial_shape + [in_channels]` if data_format does
      not start with "NC" (default), or
      `[batch_size, in_channels] + input_spatial_shape` if data_format starts
      with "NC".
    num_outputs: integer, the number of output filters.
    kernel_size: a sequence of N positive integers specifying the spatial
      dimensions of of the filters.  Can be a single integer to specify the same
      value for all spatial dimensions.
    stride: a sequence of N positive integers specifying the stride at which to
     ......
    scope: Optional scope for `variable_scope`.
'''
```

我们可以看到参数：
- inputs：输入数据
- num_outputs：输出深度
- kernel_size：窗口尺寸
- stride=1：步长，默认为1
- padding=‘SAME’：是否填充0，默认填充

接下来我们来看下这部分的代码：
- Branch_0：

![这里写图片描述](https://img-blog.csdn.net/20170610203756459?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从`net`，用深度为320，1×1的过滤器，过滤出`branch_0`。

```python
end_point = 'Mixed_7b'
      with variable_scope.variable_scope(end_point):
        with variable_scope.variable_scope('Branch_0'):
          branch_0 = layers.conv2d(
              net, depth(320), [1, 1], scope='Conv2d_0a_1x1')
```
- Branch_1：

![这里写图片描述](https://img-blog.csdn.net/20170610204008689?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

先从`net`，用深度为384，1×1的过滤器，过滤出`branch_1`。

再从`branch_1`，分别用一个深度为384，1×3的过滤器和深度为384，3×1的过滤器，过滤出两层。

最后，将这两层合并，赋值给`branch_1`，`concat`中参数3，代表在是在第三个维度，即深度上，对两者进行合并。

```python
with variable_scope.variable_scope('Branch_1'):
          branch_1 = layers.conv2d(
              net, depth(384), [1, 1], scope='Conv2d_0a_1x1')
          branch_1 = array_ops.concat(
              [
                  layers.conv2d(
                      branch_1, depth(384), [1, 3], scope='Conv2d_0b_1x3'),
                  layers.conv2d(
                      branch_1, depth(384), [3, 1], scope='Conv2d_0b_3x1')
              ],
              3)
```
- Branch_2 & Branch_3

![这里写图片描述](https://img-blog.csdn.net/20170610205519084?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

前面两个操作类似，可以参考图片自己理解代码：

```python
with variable_scope.variable_scope('Branch_2'):
          branch_2 = layers.conv2d(
              net, depth(448), [1, 1], scope='Conv2d_0a_1x1')
          branch_2 = layers.conv2d(
              branch_2, depth(384), [3, 3], scope='Conv2d_0b_3x3')
          branch_2 = array_ops.concat(
              [
                  layers.conv2d(
                      branch_2, depth(384), [1, 3], scope='Conv2d_0c_1x3'),
                  layers.conv2d(
                      branch_2, depth(384), [3, 1], scope='Conv2d_0d_3x1')
              ],
              3)
        with variable_scope.variable_scope('Branch_3'):
          branch_3 = layers_lib.avg_pool2d(net, [3, 3], scope='AvgPool_0a_3x3')
          branch_3 = layers.conv2d(
              branch_3, depth(192), [1, 1], scope='Conv2d_0b_1x1')
```

最后，将`branch_0, branch_1, branch_2, branch_3`进行合并：

```
net = array_ops.concat([branch_0, branch_1, branch_2, branch_3], 3)
      end_points[end_point] = net
      if end_point == final_endpoint:
return net, end_points
```

![这里写图片描述](https://img-blog.csdn.net/20170611105123627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


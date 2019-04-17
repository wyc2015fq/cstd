# 【Tensorflow slim 实战】写Inception-V4 Inception-ResNet-v2结构 - zj360202的专栏 - CSDN博客





2017年11月24日 18:29:47[zj360202](https://me.csdn.net/zj360202)阅读数：3501








inception的paper地址：[https://arxiv.org/pdf/1602.07261.pdf](https://arxiv.org/pdf/1602.07261.pdf)

Inception-V4 Inception-ResNet-v2结构结构图如下：

![](https://img-blog.csdn.net/20171124182455305?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemozNjAyMDI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


实现如下



```python
# -*- coding:utf-8 -*-
import tensorflow as tf
import tensorflow.contrib.slim as slim

#v4.default_image_size = 229
def v4(inputs,
       sc='Inception-ResNet-v2'):
    '''
    Inception-V4 Inception-ResNet-v2 结构
    net structs
    --------------------------------------
    input            | 229 x 229 x 3
    3x3 conv / s2    | 149 x 149 x 32
    3x3 conv / s1    | 147 x 147 x 32
    3x3 conv / s1    | 147 x 147 x 64
    3x3 conv / s1    | 147 x 147 x 32
    3x3 conv / s1    | 147 x 147 x 64
    --------------------------------------
    3x3 maxpool / s2 | 73 x 73 x 64
                     +
    3x3 conv / s2    | 73 x 73 x 96
    --------------------------------------
    concat           | 73 x 73 x 160
    --------------------------------------
    1x1 conv / s1    | 73 x 73 x 64          #1x1就是为了降维(或是说成将上一层输出的深度压缩)的，将上面的160维降到64维
    3x3 conv / s1    | 71 x 71 x 96
                     +
    1x1 conv / s1    | 73 x 73 x 64
    7x1 conv / s1    | 73 x 73 x 64
    1x7 conv / s1    | 73 x 73 x 64
    3x3 conv / s1    | 71 x 71 x 96
    --------------------------------------
    concat           | 71 x 71 x 192
    --------------------------------------
    3x3 maxpool / s2 | 35 x 35 x 192
                     +
    3x3 conv / s2    | 35 x 35 x 192
    --------------------------------------
    concat           | 35 x 35 x 384
    --------------------------------------
    '''
    end_points = {}
    with tf.variable_scope(sc):
        with slim.arg_scope([slim.conv2d, slim.max_pool2d],stride=1,padding='SAME'):
            net = slim.conv2d(inputs, 32, [3, 3], stride=2, scope='conv_1')
            end_points['conv_1'] = net
            net = slim.conv2d(net, 32, [3, 3], padding='VALID', name='conv_2')
            end_points['conv_2'] = net
            net = slim.conv2d(net, 64, [3, 3], name='conv_3')
            end_points['conv_3'] = net
            with tf.variable_scope('mixed_1'):
                with tf.variable_scope('branch_0'):
                    branch_0 = slim.max_pool2d(net, [3, 3], stride=2, name='branch_0_mp')
                with tf.variable_scope('branch_1'):
                    branch_1 = slim.conv2d(net, 96, [3, 3], stride=2, name='branch_1_conv')
                net = tf.concat([branch_0, branch_1], 3)
            end_points['mixed_1'] = net
            with tf.variable_scope('mixed_2'):
                with tf.variable_scope('branch_0'):
                    branch_0 = slim.conv2d(net, 64, [1, 1], name='branch_0_conv1')
                    branch_0 = slim.conv2d(branch_0, 96, [3, 3], padding='VALID', name='branch_0_conv2')
                with tf.variable_scope('branch_1'):
                    branch_1 = slim.conv2d(net, 64, [1, 1], name='branch_1_conv1')
                    branch_1 = slim.conv2d(branch_1, 64, [7, 1], name='branch_1_conv1')
                    branch_1 = slim.conv2d(branch_1, 64, [1, 7], name='branch_1_conv1')
                    branch_1 = slim.conv2d(branch_1, 96, [3, 3], padding='VALID', name='branch_1_conv1')
                net = tf.concat([branch_0, branch_1], 3)
            end_points['mixed_2'] = net
            with tf.variable_scope('mixed_3'):
                with tf.variable_scope('branch_0'):
                    branch_0 = slim.max_pool2d(net, [3, 3], stride=2, name='branch_0_mp')
                with tf.variable_scope('branch_1'):
                    branch_1 = slim.conv2d(net, 192, [3, 3], stride=2, name='branch_1_conv')
                net = tf.concat([branch_0, branch_1], 3)
            end_points['mixed_3'] = net
    end_points['net'] = net
    return net, end_points
```






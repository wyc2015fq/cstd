# 【Tensorflow slim 实战】写MobileNet - zj360202的专栏 - CSDN博客





2017年11月24日 13:17:33[zj360202](https://me.csdn.net/zj360202)阅读数：6467








按https://arxiv.org/pdf/1704.04861.pdf

通过和https://github.com/Zehaos/MobileNet/blob/master/nets/mobilenet.py 的对比

复写了一遍MobileNet



```python
# -*- coding:utf-8 -*-
import tensorflow as tf
import tensorflow.contrib.slim as slim

def mobile_net(inputs,
               num_classes=1000,
               is_training=True,
               width_multiplier=1,
               scope='MobileNet'):
    '''
    net structs
    --------------------------------------------------
    conv / s2         | 3x3x3x32         | 224x224x3  
    conv dw / s1      | 3x3x1x32 dw      | 112x112x32
    conv / s1         | 1x1x32x64        | 112x112x32  
    conv dw / s2      | 3x3x1x64 dw      | 112x112x64
    conv / s1         | 1x1x64x128       | 56x56x64  
    conv dw / s1      | 3x3x1x128 dw     | 56x56x128
    conv / s1         | 1x1x128x128      | 56x56x128 
    conv dw / s2      | 3x3x1x128 dw     | 56x56x128
    conv / s1         | 1x1x128x256      | 28x28x128  
    conv dw / s1      | 3x3x1x256 dw     | 28x28x256
    conv / s1         | 1x1x256x256      | 28x28x256  
    conv dw / s2      | 3x3x1x256 dw     | 28x28x256
    conv / s1         | 1x1x256x512      | 14x14x256
    --------------------------------------------------
    5x  
       conv dw / s1   | 3x3x1x512 dw     | 14x14x512
       conv / s1      | 1x1x512x512      | 14x14x512  
    --------------------------------------------------
    conv dw / s2      | 3x3x1x512 dw     | 14x14x512
    conv / s1         | 1x1x512x1024     | 7x7x512  
    conv dw / s2      | 3x3x1x1024 dw    | 7x7x1024
    conv / s1         | 1x1x1024x1024    | 7x7x1024
    Avg Pool / s1     | Pool 7x7         | 7x7x1024  
    FC / s1           | 1204x1000        | 1x1x1024
    Softmax / s1      | Classifier       | 1x1x1000
    --------------------------------------------------
    '''
    def _depthwise_separable_conv(inputs,
                                  num_pwc_filters,
                                  width_multiplier,
                                  downsimple=False,
                                  scope_name='Depthwise_Conv'):
        '''
        net structs  == conv dw / sx  + conv /s1   x=downsimple取值
        -------------------------
        3x3 Depthwise Conv
        BN
        ReLU
        1X1 Conv
        BN
        ReLU
        -------------------------
        inputs
        '''
        ds = 2 if downsimple else 1
        dwc = slim.separable_conv2d(inputs,
                                    num_outputs=None,
                                    kernel_size=[3, 3],
                                    stride = ds,
                                    depth_multiplier=1,
                                    activation_fn=None,
                                    scope=scope_name+'/depthwise_conv')
        bn = slim.batch_norm(dwc, scope=scope_name+'/dw_bn')
        relu = tf.nn.relu(bn, name=scope_name+'/dw_relu')
        conv = slim.conv2d(relu,
                       num_pwc_filters,
                       kernel_size=[1, 1],
                       activation_fn=None,
                       scope=scope_name+'/conv2d')
        bn = slim.batch_norm(conv, activation_fn=tf.nn.relu, scope=scope_name+'/conv_bn')
        #relu = tf.nn.relu(bn, name=scope_name+'/conv_relu')
        return bn

    def _standard_conv(inputs,
                       num_outputs,
                       scope_name="std_conv"):
        '''
        net structs
        -------------------------
        3x3 conv
        BN
        ReLU
        -------------------------

        '''
        conv = slim.conv2d(inputs,
                       num_outputs=num_outputs,
                       kernel_size=[3,3],
                       activation_fn=None,
                       scope=scope_name+'/conv')
        bn = slim.batch_norm(conv, activation_fn=tf.nn.relu, scope=scope_name+"/bn")
        return bn

    with tf.variable_scope(scope) as sc:
        end_points_collection = sc+'_end_points'
        with slim.arg_scope([slim.conv2d, slim.separable_conv2d],
                            activation_fn=None,
                            outputs_collections=[end_points_collection]):
            with slim.arg_scope([slim.batch_norm],
                                is_training=is_training,
                                activation_fn=tf.nn.relu,
                                fused=True)
                net = slim.conv2d(inputs, round(32 * width_multiplier), [3, 3], stride=2, padding='SAME', scope='conv_1')
                net = slim.batch_norm(net, scope='conv_1/bn')
                net = _depthwise_separable_conv(net, 64, width_multiplier, scope_name='conv_ds_2')
                net = _depthwise_separable_conv(net, 128, width_multiplier, downsimple=True, scope_name='conv_ds_3')
                net = _depthwise_separable_conv(net, 128, width_multiplier, scope_name='conv_ds_4')
                net = _depthwise_separable_conv(net, 256, width_multiplier, downsimple=True, scope_name='conv_ds_5')
                net = _depthwise_separable_conv(net, 256, width_multiplier, scope_name='conv_ds_6')
                net = _depthwise_separable_conv(net, 512, width_multiplier, downsimple=True, scope_name='conv_ds_7')
                net = _depthwise_separable_conv(net, 512, width_multiplier, scope_name='conv_ds_8')
                net = _depthwise_separable_conv(net, 512, width_multiplier, scope_name='conv_ds_9')
                net = _depthwise_separable_conv(net, 512, width_multiplier, scope_name='conv_ds_10')
                net = _depthwise_separable_conv(net, 512, width_multiplier, scope_name='conv_ds_11')
                net = _depthwise_separable_conv(net, 512, width_multiplier, scope_name='conv_ds_12')
                net = _depthwise_separable_conv(net, 1024, width_multiplier, downsimple=True, scope_name='conv_ds_13')
                net = _depthwise_separable_conv(net, 1024, width_multiplier, downsimple=True, scope_name='conv_ds_14')
                net = slim.avg_pool2d(net, [7, 7], scope='avg_pool_15')
            end_points = slim.utils.convert_collection_to_dict(end_points_collection)
            net = tf.squeeze(net, [1, 2], name='SpatialSqueeze')
            end_points['squeeze'] = net
            net = slim.fully_connected(net, num_classes, activation_fn=None, scope='fc_16')
            predictions = slim.softmax(logits, scope='Predictions')

            end_points['Logits'] = logits
            end_points['Predictions'] = predictions
        return logits, end_points
```







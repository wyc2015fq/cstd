# 【tensorflow】tensorflow中使用gpu方法 - zkq_1986的博客 - CSDN博客





2018年11月14日 10:12:14[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：68








在tensorflow中，我们可以使用 tf.device() 指定模型运行的具体设备，可以指定运行在GPU还是CUP上，以及哪块GPU上。

设置使用GPU

使用 tf.device('/gpu:1') 指定Session在第二块GPU上运行：

import tensorflow as tf


with tf.device('/gpu:1'):

    v1 = tf.constant([1.0, 2.0, 3.0], shape=[3], name='v1')

    v2 = tf.constant([1.0, 2.0, 3.0], shape=[3], name='v2')

    sumV12 = v1 + v2


    with tf.Session(config=tf.ConfigProto(log_device_placement=True)) as sess:

        print sess.run(sumV12)

ConfigProto() 中参数 log_device_placement=True  会打印出执行操作所用的设备，以上输出：

如果安装的是GPU版本的tensorflow，机器上有支持的GPU，也正确安装了显卡驱动、CUDA和cuDNN，默认情况下，Session会在GPU上运行：

import tensorflow as tf


v1 = tf.constant([1.0, 2.0, 3.0], shape=[3], name='v1')

v2 = tf.constant([1.0, 2.0, 3.0], shape=[3], name='v2')

sumV12 = v1 + v2


with tf.Session(config=tf.ConfigProto(log_device_placement=True)) as sess:

    print sess.run(sumV12)



默认在GPU:0上执行：



设置使用cpu

tensorflow中不同的GPU使用/gpu:0和/gpu:1区分，而CPU不区分设备号，统一使用 /cpu:0

import tensorflow as tf


with tf.device('/cpu:0'):

    v1 = tf.constant([1.0, 2.0, 3.0], shape=[3], name='v1')

    v2 = tf.constant([1.0, 2.0, 3.0], shape=[3], name='v2')

    sumV12 = v1 + v2


    with tf.Session(config=tf.ConfigProto(log_device_placement=True)) as sess:

        print sess.run(sumV12)



转载自：

--------------------- 

作者：-牧野- 

来源：CSDN 

原文：https://blog.csdn.net/dcrmg/article/details/79747882 





# Keras以及Tensorflow强制使用CPU - 站在巨人的肩膀上coding - CSDN博客





2018年04月19日 22:46:34[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：802








Keras如果是使用Theano后端的话，应该是自动不使用ＧＰＵ只是用ＣＰＵ的，启动ＧＰＵ使用Theano内部命令即可。 
对于Tensorflow后端的Keras以及Tensorflow会自动使用可见的ＧＰＵ，而我需要其必须只运行在ＣＰＵ上。网上查到三种方法，最后一种方法对我有用，但也对三种都做如下记录：
- 使用tensorflow的 `with tf.device('/cpu:0'):`函数。简单操作就是把所有命令都放在前面所述的域里面。
- 使用tensorflow声明Session时的参数： 
关于tensorflow中Session中的部分参数设置，以及Keras如何设置其调用的Tensorflow的Session，可以参见[Keras设定GPU使用内存大小(Tensorflow backend)](http://blog.csdn.net/silent56_th/article/details/60154637)。 
对于Tensorflow，声明Session的时候加入`device_count={'gpu':0}`即可，代码如下：

```python
import tensorflow as tf 

sess = tf.Session(config=tf.ConfigProto(device_count={'gpu':0}))
```
- 1
- 2
- 3

```
对于Keras,则调用后端函数，设置其使用如上定义的Session即可，代码如下：
```
- 1
- 2

```python
import tensorflow as tf
import keras.backend.tensorflow_backend as KTF

KTF.set_session(tf.Session(config=tf.ConfigProto(device_count={'gpu':0})))
```
- 1
- 2
- 3
- 4

对于多线程以及ＧＰＵ内存设置等可以参见[Keras设定GPU使用内存大小(Tensorflow backend)](http://blog.csdn.net/silent56_th/article/details/60154637)；更多详细内容请见[tensorflow官网](https://blog.csdn.net/silent56_th/article/details/tensorflow.org)。

3 . 第三种是使用CUDA_VISIBLE_DEVICES命令行参数，代码如下：
`CUDA_VISIBLE_DEVICES="" python3 train.py`




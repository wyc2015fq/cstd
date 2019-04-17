# tensorflow学习笔记(二十一):tensorflow可视化 - Keith - CSDN博客





2016年11月16日 11:30:56[ke1th](https://me.csdn.net/u012436149)阅读数：28802
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









`tensorflow`的可视化是使用`summary`和`tensorboard`合作完成的.

## 基本用法

首先明确一点,`summary`也是`op`.

**输出网络结构**

```python
with tf.Session() as sess:
  writer = tf.summary.FileWriter(your_dir, sess.graph)
```

命令行运行`tensorboard --logdir your_dir`,然后浏览器输入`127.0.1.1:6006`**注：tf1.1.0 版本的tensorboard端口换了(0.0.0.0:6006)**

这样你就可以在`tensorboard`中看到你的网络结构图了

## 可视化参数

```python
#ops
loss = ...
tf.summary.scalar("loss", loss)
merged_summary = tf.summary.merge_all()

init = tf.global_variable_initializer()
with tf.Session() as sess:
  writer = tf.summary.FileWriter(your_dir, sess.graph)
  sess.run(init)
  for i in xrange(100):
    _,summary = sess.run([train_op,merged_summary], feed_dict)
    writer.add_summary(summary, i)
```

这时,打开`tensorboard`,在`EVENTS`可以看到`loss`随着`i`的变化了，如果看不到的话，可以在代码最后加上`writer.flush()`试一下，原因后面说明。

## 函数介绍
- 
`tf.summary.merge_all`: 将之前定义的所有`summary op`整合到一起

- 
`FileWriter`: 创建一个`file writer`用来向硬盘写`summary`数据,

- 
`tf.summary.scalar(summary_tags, Tensor/variable, collections=None)`: 用于标量的 `summary`

- 
`tf.summary.image(tag, tensor, max_images=3, collections=None, name=None)`:tensor,必须4维,形状[batch_size, height, width, channels],`max_images`(最多只能生成3张图片的`summary`),觉着这个用在卷积中的`kernel`可视化很好用.`max_images`确定了生成的图片是[-max_images: ,height, width, channels]，还有一点就是，`TensorBord`中看到的`image summary`永远是最后一个`global step`的

- 
`tf.summary.histogram(tag, values, collections=None, name=None)`:values,任意形状的`tensor`,生成直方图`summary`

- 
`tf.summary.audio(tag, tensor, sample_rate, max_outputs=3, collections=None, name=None)`


**解释collections参数：它是一个`list`,如果不指定collections, 那么此summary会被添加到`f.GraphKeys.SUMMARIES`中，如果指定了，就会放在的`collections`中。**

## FileWriter

**注意**:`add_summary`仅仅是向`FileWriter`对象的缓存中存放`event data`。而向`disk`上写数据是由`FileWrite对象`控制的。下面通过`FileWriter`的构造函数来介绍这一点！！！

```python
tf.summary.FileWriter.__init__(logdir, graph=None, max_queue=10, flush_secs=120, graph_def=None)

Creates a FileWriter and an event file.
# max_queue: 在向disk写数据之前，最大能够缓存event的个数
# flush_secs: 每多少秒像disk中写数据，并清空对象缓存
```

## 注意
- 
如果使用`writer.add_summary(summary，global_step)`时没有传`global_step`参数,会使`scarlar_summary`变成一条直线。

- 
只要是在计算图上的`Summary op`，都会被`merge_all`捕捉到， 不需要考虑变量生命周期问题！

- 如果执行一次，`disk`上没有保存`Summary`数据的话，可以尝试下`file_writer.flush()`

## 小技巧

如果想要生成的summary有层次的话，记得在`summary`外面加一个`name_scope`

```python
with tf.name_scope("summary_gradients"):
    tf.summary.histgram("name", gradients)
```

这样，`tensorboard`在显示的时候，就会有一个`sumary_gradients`一级目录。

## tensorboard中distribution图的意义

[https://stackoverflow.com/questions/35567132/meaning-of-histogram-on-tensorboard](https://stackoverflow.com/questions/35567132/meaning-of-histogram-on-tensorboard)

**参考资料**
[https://www.tensorflow.org/api_docs/python/summary/generation_of_summaries_#FileWriter.**init**](https://www.tensorflow.org/api_docs/python/summary/generation_of_summaries_#FileWriter.__init__)




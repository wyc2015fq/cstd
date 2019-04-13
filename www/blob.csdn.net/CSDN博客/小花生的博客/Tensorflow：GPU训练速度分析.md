
# Tensorflow：GPU训练速度分析 - 小花生的博客 - CSDN博客


2018年09月06日 21:32:32[Peanut_范](https://me.csdn.net/u013841196)阅读数：1551



## GPU训练速度分析：
常见模型会从磁盘中抽取数据，进行预处理，然后通过网络发送数据。例如，处理JPEG图片的模型会有下面的流程：从磁盘加载图片，将JPEG解码成一个tensor，进行裁减（crop）和补齐（pad），可能还会进行翻转（flip）和扭曲（distort），然后再batch。该流程被称为input pipeline。随着GPUs和其它硬件加速器越来越快，**数据预处理可能是个瓶颈**。
## 验证该问题：
生成一个timeline，然后观察那些空白（等待状态）的大块(large blocks)。
**参考代码：**
```python
import tensorflow
```
```python
as
```
```python
tf
```
```python
from
```
```python
tensorflow.python.client import timeline
a = tf.random_normal([
```
```python
2000
```
```python
,
```
```python
5000
```
```python
])
b = tf.random_normal([
```
```python
5000
```
```python
,
```
```python
1000
```
```python
])
res = tf.matmul(a, b)
```
```python
with
```
```python
tf.Session()
```
```python
as
```
```python
sess:
    #
```
```python
add
```
```python
additional options
```
```python
to
```
```python
trace the session execution
    options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
    run_metadata = tf.RunMetadata()
    sess.run(res, options=options, run_metadata=run_metadata)
    #
```
```python
Create
```
```python
the Timeline object,
```
```python
and
```
```python
write
```
```python
it
```
```python
to
```
```python
a json file
    fetched_timeline = timeline.Timeline(run_metadata.step_stats)
    chrome_trace = fetched_timeline.generate_chrome_trace_format()
```
```python
with
```
```python
open(
```
```python
'timeline_01.json'
```
```python
,
```
```python
'w'
```
```python
)
```
```python
as
```
```python
f:
        f.
```
```python
write
```
```python
(chrome_trace)
```
应该注意到给session运行时添加的额外的options和run_metadata，这个脚本应该在CPU和GPU上运行，在执行之后我们会获得timeline_01.json和性能剖析的数据，这些都以Chrome的trace文件形式存储。
要查看存储的数据，我们应该使用Chrome浏览器（不幸的是据我所知，只有它支持自己的跟踪格式）。转到chrome://tracing跟踪页面。在左上角，您会看到加载按钮。按下它并加载我们的JSON文件。
**点击load按钮，将生成的.json进行load即可：**
![这里写图片描述](https://img-blog.csdn.net/20180906210318179?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180906210318179?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
由上图可知：数据预处理是CPU和GPU都占用，耗时比较长。
将队列操作全部指定在CPU下：**with tf.device(‘/cpu:0’):**
**重新生成timeline：**
![这里写图片描述](https://img-blog.csdn.net/20180906210437891?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180906210437891?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可见：数据预处理速度明显减少，GPU使用效率明显提升。
[
](https://img-blog.csdn.net/20180906210437891?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## GPU利用率优化：
[
](https://img-blog.csdn.net/20180906210437891?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**1.CPU预处理**
在CPU上放置input pipeline操作，可以极大提升性能。对于input pipeline，使用CPU可以充分释放GPU，让它更聚焦于训练上。为了确保预处理过程发生在CPU上，需要按以下方式包装预处理操作：
with tf.device(‘/cpu:0’):
\# function to get and process images or data.
distorted_inputs = load_and_distort_images()
如果使用tf.estimator.Estimator，input function会被自动放置在CPU上。
**2使用Dataset API**
对于构建input pipeline，推荐使用Dataset API来替代queue_runner。该API在tensorFlow 1.2中作为contrib的一部分被添加进去，并在后续版本会移至core包中。Dataset API会使用C++的多线程机制，会比基于python的queue_runner（受限于python的多线程低性能）的开销更低。
当使用一个feed_dict来feeding数据时，会提供更高的灵活性，使用feed_dict的大多数实例不可以进行合适的扩展。然而，在只有一个GPU的实例中，这种差异是微不足道的。我们仍推荐使用Dataset API。避免使用以下的情况：
**feed_dict often results in suboptimal performance when using large inputs.**
sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys})
**3.Fused batch norm**
Fused batch norm 会结合多种op，来将batch归一化（normalization）到单个kernel中。对于一些模型，Batch norm是一个开销昂贵的处理，会占据大量的操作时间。使用fused batch norm可以进行12%-30%的加速。
有两个常用的batch norm，它们同时支持fusing。核心的tf.layers.batch_normalization在TensorFlow 1.3中被添加进去。
bn = tf.layers.batch_normalization(input_layer, fused=True, data_format=’NCHW’)
自TensorFlow 1.0后，tf.contrib.layers.batch_norm方法具有fused操作。
bn = tf.contrib.layers.batch_norm(input_layer, fused=True, data_format=’NCHW’)
**4.其他还有很多优化方法，参考：**
[https://walsvid.github.io/2017/03/25/profiletensorflow/](https://walsvid.github.io/2017/03/25/profiletensorflow/)
[http://d0evi1.com/tensorflow/performance/](http://d0evi1.com/tensorflow/performance/)
---

###### 注：博众家之所长，集群英之荟萃。


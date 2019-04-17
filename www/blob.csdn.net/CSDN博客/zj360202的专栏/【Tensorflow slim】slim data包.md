# 【Tensorflow slim】slim.data包 - zj360202的专栏 - CSDN博客





2017年11月21日 12:35:58[zj360202](https://me.csdn.net/zj360202)阅读数：2180













TensorFlow是谷歌基于DistBelief进行研发的第二代[人工智能](https://baike.so.com/doc/2952526-3114987.html)[学习系统](https://baike.so.com/doc/10020706-10368631.html)，其命名来源于本身的运行原理。Tensor(张量)意味着N维数组，Flow(流)意味着基于数据流图的计算，TensorFlow为张量从流图的一端流动到另一端计算过程。TensorFlow是将复杂的数据结构传输至人工智能神经网中进行分析和处理过程的系统。


TensorFlow可被用于[语音识别](https://baike.so.com/doc/1662577-1757537.html)或[图像识别](https://baike.so.com/doc/6527187-6740919.html)等多项机器深度学习领域，对2011年开发的深度学习基础架构DistBelief进行了各方面的改进，它可在小到一部智能手机、大到数千台数据中心服务器的各种设备上运行。TensorFlow将完全开源，任何人都可以用。

原生接口文章


- [【Tensorflow】tf.placeholder函数](http://blog.csdn.net/zj360202/article/details/70243127)
- [【TensorFlow】tf.nn.conv2d是怎样实现卷积的](http://blog.csdn.net/zj360202/article/details/70243424)
- [【TensorFlow】tf.nn.max_pool实现池化操作](http://blog.csdn.net/zj360202/article/details/70243836)
- [【Tensorflow】tf.nn.relu函数](http://blog.csdn.net/zj360202/article/details/70256545)
- [【Tensorflow】tf.reshape
 函数](http://blog.csdn.net/zj360202/article/details/70256835)
- [【Tensorflow】tf.nn.dropout函数](http://blog.csdn.net/zj360202/article/details/70257048)
- [【Tensorflow】tf.argmax函数](http://blog.csdn.net/zj360202/article/details/70259999)
- [【Tensorflow】tf.cast
 类型转换 函数](http://blog.csdn.net/zj360202/article/details/70260265)
- [【Tensorflow】tf.train.AdamOptimizer函数](http://blog.csdn.net/zj360202/article/details/70263110)
- [【Tensorflow】tf.Graph()函数](http://blog.csdn.net/zj360202/article/details/78539464)
- [【TensorFlow】tf.nn.softmax_cross_entropy_with_logits的用法](http://blog.csdn.net/zj360202/article/details/78582895)

- [【Tensorflow】tf.dynamic_partition
 函数 分拆数组](http://blog.csdn.net/zj360202/article/details/78642340)

原生接口实例



- [【Tensorflow】实现简单的卷积神经网络CNN实际代码](http://blog.csdn.net/zj360202/article/details/70265414)
- [【Tensorflow
 实战】实现欧式距离](http://blog.csdn.net/zj360202/article/details/78623609)

slim接口文章



- [【Tensorflow】tensorflow.contrib.slim
 包](http://blog.csdn.net/zj360202/article/details/78585197)

- [【Tensorflow
 slim】 slim.arg_scope的用法](http://blog.csdn.net/zj360202/article/details/78590285)

- [【Tensorflow
 slim】slim.data包](http://blog.csdn.net/zj360202/article/details/78591068)

- [【Tensorflow
 slim】slim evaluation 函数](http://blog.csdn.net/zj360202/article/details/78593236)

- [【Tensorflow
 slim】slim layers包](http://blog.csdn.net/zj360202/article/details/78593626)

- [【Tensorflow
 slim】slim learning包](http://blog.csdn.net/zj360202/article/details/78594089)
- [【Tensorflow
 slim】slim losses包](http://blog.csdn.net/zj360202/article/details/78594359)

- [【Tensorflow
 slim】slim nets包](http://blog.csdn.net/zj360202/article/details/78595102)

- [【Tensorflow
 slim】slim variables包](http://blog.csdn.net/zj360202/article/details/78595220)

- [【Tensorflow
 slim】slim metrics包](http://blog.csdn.net/zj360202/article/details/78595315)
slim
 实例



- [【Tensorflow
 slim 实战】写MobileNet](http://blog.csdn.net/zj360202/article/details/78623567)

- [【Tensorflow
 slim 实战】写Inception-V4 Inception-ResNet-v2结构](http://blog.csdn.net/zj360202/article/details/78627217)
kera
 接口文章



- [【Tensorflow
 keras】Keras:基于Theano和TensorFlow的深度学习库](http://blog.csdn.net/zj360202/article/details/78645039)

- [【Tensorflow
 keras】轻量级深度学习框架 Keras简介](http://blog.csdn.net/zj360202/article/details/78646969)

tensorflow使用过程中的辅助接口或通过tensorflow实现的批量操作接口



- [将非RGB图片转换为RGB图片](http://blog.csdn.net/zj360202/article/details/78539040)

- [【opencv】python3
 将图片生成视频文件](http://blog.csdn.net/zj360202/article/details/71561925)

- [【opencv】selective_search函数](http://blog.csdn.net/zj360202/article/details/70847144)





=========================================================================





slim.data包下面有很多对数据的操作方法；在slim.__init__种有如下几种：



```python
from tensorflow.contrib.slim.python.slim.data import data_decoder
from tensorflow.contrib.slim.python.slim.data import data_provider
from tensorflow.contrib.slim.python.slim.data import dataset
from tensorflow.contrib.slim.python.slim.data import dataset_data_provider
from tensorflow.contrib.slim.python.slim.data import parallel_reader
from tensorflow.contrib.slim.python.slim.data import prefetch_queue
from tensorflow.contrib.slim.python.slim.data import tfexample_decoder
```

## Data Decoder

```python
# Load the data
my_encoded_data = ...
data_decoder = slim.DataDecoder()

# 解析输入和标签:
decoded_input, decoded_labels = data_decoder.Decode(data, ['input', 'labels'])

# 只解析输入:
decoded_input = data_decoder.Decode(data, ['input'])

# Check which items a data decoder knows how to decode:
for item in data_decoder.list_items():
  print(item)
```

## Data Provider

```python
provider = slim.DataProvider(...)
image, sentence, clazz = provider.get(['image', 'sentence', 'class'])
```

## DataSet

```python
dataset = slim.dataset.Dataset(
            data_sources='./pascal_train_*.tfrecord',
            reader=tf.TFRecordReader,
            num_samples = 3, # 手动生成了三个样本集
            decoder=decoder,
            items_to_descriptions = {},
            num_classes=21)
```

## Data Data Provider

shuffle=Flase，主要的话，每一次取的数据是一样的



```python
pascal_voc_data_provider = slim.DatasetDataProvider(
      slim.datasets.pascal_voc.get_split('train'),
      shuffle=False)
  images, labels = pascal_voc_data_provider.get(['images', 'labels'])
```


shuffle=True，每一次取的数据会进行洗牌，取的结果和上传会不一样的





```python
pascal_voc_data_provider = slim.DatasetDataProvider(
      slim.datasets.pascal_voc.Dataset(),
      num_readers=10,
      shuffle=True)
  images, labels = pascal_voc_data_provider.get(['images', 'labels'])
```
分开取元素





```python
[images] = pascal_voc_data_provider.get(['images'])
[labels] = pascal_voc_data_provider.get(['labels'])
```



## Parallel Reader

通过队列和可选洗牌实现并行数据读取器



```python
common_queue = tf.RandomShuffleQueue(
          capacity=256,
          min_after_dequeue=128,
          dtypes=[tf.string, tf.string])
p_reader = slim.ParallelReader(tf.TFRecordReader, common_queue)
common_queue = tf.FIFOQueue(
          capacity=256,
          dtypes=[tf.string, tf.string])
p_reader = slim.ParallelReader(readers, common_queue, num_readers=2)
```

## Prefetch Queue

预取队列


```python
images, labels = tf.train.batch([image, label], batch_size=32, num_threads=4)
batch_queue = slim.prefetch_queue([images, labels])
images, labels = batch_queue.dequeue()
logits = Net(images)
loss = Loss(logits, labels)
```










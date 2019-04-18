# 编写基于TensorFlow的应用之构建数据pipelin - 知乎
# 

本文及其它机器学习、深度学习算法的全面系统讲解可以阅读《机器学习与应用》，清华大学出版社，雷明著，由SIGAI公众号作者倾力打造，自2019年1月出版以来已重印3次。
- [书的购买链接](https://link.zhihu.com/?target=https%3A//item.jd.com/12504554.html)
- [书的勘误，优化，源代码资源](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_78.html)

本文主要以MNIST数据集为例介绍TFRecords文件如何制作以及加载使用。所讲内容可以在SIGAI 在线编程功能中的sharedata/intro_to_tf文件夹中可以免费获取。此项功能对所有注册用户免费开放.

> 官网地址：www.sigai.cn,
推荐使用chrome浏览器

 在线编程功能使用指南见SIGAI官网->知识库->在线编程使用说明小视频
![](https://pic1.zhimg.com/v2-ecee7fbfcd752792b2fd960a9f393f38_b.png)图1 典型的基于TensorFlow 的应用的workflow

 通常情况下，一个基于TensorFlow 的应用训练过程中所 

采用的workflow 如图1 所示。针对与原始数据的格式，首先采用不同的转换方式在运行过程中生成Tensor格式的数据，然后将其送到TensorFlowGraph中运行，根据设定的目标函数，不断的在训练数据上迭代并周期性地保存checkpoint到文件中，checkpoint文件可以用于后续的模型持久化操作。TensorFlow框架下训练输入pipeline是一个标准的ETL过程：

1. 提取数据(Extract): 从存储空间内部读取原始数据

2. 数据转换(Transform): 使用CPU解析原始数据并执行一些预处理的操作: 文本数据转换为数组，图片大小变换，图片数据增强操作等等

3. 数据加载(Load): 加载转换后的数据并传给GPU，FPGA，ASIC等加速芯片进行计算

在TensorFlow框架之下，使用 tf.dataset API 可以完成上述过程中所需的所有操作，其过程如下图所示：
![](https://pic1.zhimg.com/v2-f28e7064aa7a23a2571255269842b7bc_b.png)图2 TensorFlow中的ETL过程
相较于TFRecords文件，文本文件，numpy数组,csv文件等文件格式更为常见。接下来，本文将以常用的MNIST数据集为例简要介绍TFRecord文件如何生成以及如何从TFrecord构建数据pipeline。

Record文件简介

TFRecord文件是基于Google Protocol Buffers的一种保存数据的格式，我们推荐在数据预处理过程中尽可能使用这种方式将训练数据保存成这种格式。Protocol Buffers 是一种简洁高效的序列化格式化的方法，其采用了语言无关，平台无关且可扩展的机制。 采用这种方式的优势在于： 

1. 采用二进制格式存储，减少存储空间，提高读取效率

2. 针对TensorFlow框架进行优化，支持合并多个数据源，并且支持TensorFlow内置的其他数据预处理方式

3. 支持序列化数据的存储(时序数据或者词向量)
![](https://pic2.zhimg.com/v2-ba6bd9f10bf6fa7fcde6f7fc81afd4c1_b.png)图3 TFRecord文件中存储内容结构
TFRecords中存储的层级如图3所示，从图中可以看到：

‣ 一个TFRecord文件中包含了多个tf.train.Example, 每个tf.train.Example是一个Protocol Buffer 

‣ 每个tf.train.Example包含了tf.train.Features

‣ 每个tf.train.Features是由多个feature 构成的feature set

## 以MNIST为例生成TFRecord文件
![](https://pic2.zhimg.com/v2-47ae36478bfd4204edd54c216803ed25_b.png)图4 TFRecord文件制作和加载过程
从原始文件生成TFRecord的过程如图4所示：

1. 从文件中读取数据信息，如果是类别，长度，高度等数值型数据就转换成Int64List, FloatList格式的特征，如果是图片等raw data，则直接读取其二进制编码内容，再转换成BytesList即可

2. 将多个特征合并为 tf.train.Features，并传递到tf.train.Example中

3. 最后使用TFRecordWriter写入到文件中 

对于MNIST文件，从[http://yann.lecun.com/exdb/mnist/](https://link.zhihu.com/?target=http%3A//yann.lecun.com/exdb/mnist/)网站下载下来的是以二进制方式存储的数据集，本文略过下载并读取MNIST为numpy 数组的过程，有兴趣的读者可以查看mnist_data.py中的read_mnist函数。接下来我们重要讲解从一个numpy 数组到tfrecord文件需要执行的主要步骤：

1. 对于整个数组，需要遍历整个数组并依次将其转换成一个tf.train.Example(protocol buffer)

def feature_to_example(img, label):
  """convert numpy array to a `tf.train.example`

  Args:
    img : An `np.ndarray`. Img in numpy array format
    label : An `np.int32`. label of the image
  """
  # convert raw data corresponding to the numpy array in memory into pytho bytes
  img = img.tostring()
  return tf.train.Example(
      features=tf.train.Features(
          feature={
              'img': bytes_feature(img),
              'label': int_feature(label)
          }
      )
  )
这其中使用到的bytes_feature和int_feature分别是用来将图片和标签转换成二进制的feature和int列表的特征的函数

def int_feature(value):
  return tf.train.Feature(int64_list=tf.train.Int64List(value=[value]))

def bytes_feature(value):
  return tf.train.Feature(bytes_list=tf.train.BytesList(value=[value]))

3. 在使用SerializeToString函数将protocol buffer中的内容序列化之后， 将其内容写入到文件中

至此，MNIST的tfrecord文件就制作完成了。以上步骤各位读者可以在sharedat/intro_to_tf路径下的tfrecords.ipynb 文件中进行实验。下载MNIST数据集过程需要消耗一定时间，请各位耐心等待。[[1]](https://zhuanlan.zhihu.com/p/50107666/edit#_msocom_1) 由于MNIST中涉及到的特征仅有数组和标签两类内容，对于读者在使用TensorFlow过程中可能会遇到的其他数据格式，建议参考 [https://github.com/tensorflow/models/blob/master/research/object_detection/dataset_tools/create_pascal_tf_record.py](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/models/blob/master/research/object_detection/dataset_tools/create_pascal_tf_record.py) 文件编写适合自己数据集内容的函数

加载TFRecord文件并构建数据pipeline

从图4中，可以看到加载一个TFRrecord文件需要执行的步骤，其过程中使用了TensorFlow dataset类提供的函数：

1. shuffle:打乱输入数据的顺序

2. repeat: 重复数据集内容若干次

3. map: 对数据集中的每个数据使用map函数中传入的方法进行变换，这个过程中可以包含解析tf.train.Example内容，数据归一化以及以及data augmentation等其他操作

4. batch: 根据需要设置每次训练采用多少数据

5. prefetch:提前加载n个数据，保证每个session运行之前数据是可以立即使用的

在mnist_tfrecords.py文件中有两个不同的加载数据的方式，我们建议使用第二种优化过的加载方式，其优点在于：

1. shuffle_and_repeat可以保证加载数据的速度以及确保数据之间的顺序正确

2. map_and_batch 整合了map和batch 过程，提高了加载效率

经过优化过的加载TFRecord文件函数如下：

 def load_data_optimized(cache_dir='data/cache',
                        split='train',
                        batch_size=64,
                        epochs_between_evals=3):
  tfrecord_file = os.path.join(cache_dir,
 'mnist_{}.tfrecord'.format(split))

 # load the tfrecord data
  dataset = tf.data.TFRecordDataset(tfrecord_file)

 # shuffle and repeat data
 if split == 'train':
    dataset = dataset.apply(shuffle_and_repeat(60000, epochs_between_evals))
 else:
    dataset = dataset.apply(shuffle_and_repeat(10000, epochs_between_evals))

 # fuse map and batch
  dataset = dataset.apply(map_and_batch(parse_example,
                                        batch_size=batch_size,
                                        drop_remainder=True,
                                        num_parallel_calls=8))

  dataset = dataset.prefetch(1)

return dataset

在SIGAI提供的实验过程中，验证读取数据的内容如下图所示：
![](https://pic2.zhimg.com/v2-23c4d3af2e93a674e49817351cdc309d_b.png)
图5 验证加载MNIST数据集

本文主要介绍了TFRecord文件，然后以MNIST数据集为例讲解了如何制作MNIST数据集的TFRecord文件，接着讲述了如何加载文件并构建数据 pipeline。大家在做实验过程中使用了Eager模式，我们将在下一篇文章中介绍Eager 模式的使用。


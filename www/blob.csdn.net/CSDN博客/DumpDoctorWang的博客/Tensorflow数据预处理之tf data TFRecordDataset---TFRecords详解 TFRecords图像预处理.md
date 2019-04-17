# Tensorflow数据预处理之tf.data.TFRecordDataset---TFRecords详解\TFRecords图像预处理 - DumpDoctorWang的博客 - CSDN博客





2018年12月05日 11:32:36[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：208











### 目录
- [1、概述](#1_2)
- [2、预处理数据](#2_17)
- [2.1、常量定义](#21_33)
- [2.2、导入库](#22_89)
- [2.3、从train.txt文件中读取图片-标签对](#23traintxt_106)
- [2.4、预处理图片并保存](#24_122)
- [2.5、调用main函数](#25main_205)
- [3、读取预处理后的数据](#3_214)
- [3.1、导入库](#31_218)
- [3.2、定义TFRecordDataset](#32TFRecordDataset_236)
- [3.3、验证是否成功读取了数据](#33_294)
- [3.3、在tensorflow模型中使用](#33tensorflow_324)
- [3.5、在Keras中使用](#35Keras_361)




# 1、概述

在Tensoflow中，预处理数据除了使用tf.data.Dataset以外，还可以使用TFRecords。和tf.data.Dataset相比，优缺点如下（个人总结）：
- **在训练时节省数据预处理的计算资源。** 使用TFRecords时，是将原始数据处理之后，以一种特定的格式保存为TFRecords文件，训练是只是简单的将数据取出来训练，在训练时可以节省相当多的计算资源。
- **预处理数据的逻辑可以非常复杂。** 使用TFRecords时，数据预处理可以使用任意python代码完成，而不必拘泥于Tensorflow预定义的操作，为预处理数据提供了相当的灵活性，预处理数据的逻辑可以非常复杂。
- **训练时占用的内存更小。** 这点可能时因为不需要复杂的数据预处理，所以占用的内存更小。
- **处理之后的数据可能比原始数据大好几倍。** 这点是针对图像数据的，图像因为有压缩，所以图像原文件都比较小。预处理完成以后，像素值以浮点数或者整数的形式保存。所以和图像原始图像数据相比，处理之后的数据要比原始数据大好几倍。

本文将从一个实际的例子出发，讲解TFRecords如何使用，分为如下几部分：第二节讲如何预处理数据（以处理图像为例）和将数据保存为TFRecords文件；第三部分讲解如何读取TFRecords文件，并如何在Tensoflow和Keras中使用这些数据。

参考链接（多看官方链接才是王道）：
- [Using TFRecords and TF Examples](https://www.tensorflow.org/tutorials/load_data/tf-records)。
- [导入数据](https://www.tensorflow.org/guide/datasets)。
- [tf.data.TFRecordDataset](https://www.tensorflow.org/api_docs/python/tf/data/TFRecordDataset)。

# 2、预处理数据

在我的项目中，数据预处理的需求是：首先从一个文本文件中读取图像路径和其标签（属于哪一类）；然后根据图片路径读出图片，把像素值的范围从 [0, 255] 缩放到 [-1.0, 1.0] ；然后把处理后的数据和对应的标签保存到TFRecords文件里面。上述的文本文件的名字为train.txt，一行代表一个图片样本，由图片路径和其对应的标签组成，部分几行如下：

```
data/M-PIE/test/001/001_01_01_051_09.png 0
data/M-PIE/test/001/001_01_01_051_10.png 0
data/M-PIE/test/002/002_01_01_051_19.png 1
data/M-PIE/test/002/002_01_01_051_09.png 1
data/M-PIE/test/003/003_01_01_051_14.png 2
data/M-PIE/test/003/003_01_01_051_03.png 2
data/M-PIE/test/004/004_01_01_051_05.png 3
data/M-PIE/test/004/004_01_01_051_06.png 3
...
```

## 2.1、常量定义

此部分主要把一些常量定义在一个globals.py文件当中，免得到处都是常量，也便于将来修改。

```python
# coding=utf-8
# 兼容python3
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os
import random
import numpy as np
import tensorflow as tf

# -----------------------常量区--------------------------
# 设置种子，随便定义的
SEED = 1213

# 分类网络的类别数目，也是网络最后一层的单元数目
NUM_CLASSES = 285  

# 预处理后图像的大小
IMAGE_SHAPE = (227, 227, 3)
# 预处理后图像像素值的个数
IMAGE_SIZE = IMAGE_SHAPE[0] * IMAGE_SHAPE[1] * IMAGE_SHAPE[2]

# 未处理的图像的像素值最大值
IMAGE_DEPTH = 255

# 训练多少轮
NUM_TRAIN_EPOCH = 400
# 训练的batch size
TRAIN_BATCH_SIZE = 128

# 训练集的图片-标签对文本文件路径
TRAIN_LIST = 'data/train.txt'
# 预处理后的训练集的保存路径
TRAIN_TFRECORDS = 'data/train.tfrecords'

# 验证集的图片-标签对文本文件路径
VAL_LIST = 'data/test.txt'
# 预处理后的验证集的保存路径
VAL_TFRECORDS = 'data/test.tfrecords'

# ------------------------------------------------------

def set_seed():
    """
    固定随机数的种子，避免每次随机过程结果不一样，得到可复现的结果。
    """
    os.environ['PYTHONHASHSEED'] = str(SEED)
    np.random.seed(seed=SEED)
    tf.set_random_seed(seed=SEED)
    random.seed(SEED)
```

## 2.2、导入库

从这里开始，代码均在preprocess.py中实现，全部代码不到100行。

```python
# coding=utf-8
# 前三行是为了python2的代码兼容python3
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import numpy as np
import tensorflow as tf
import cv2

import globals as _g
# 设置固定的种子
_g.set_seed()
```

## 2.3、从train.txt文件中读取图片-标签对

这部分代码非常简单，使用numpy的一个函数就可以完成，还不会出错。

```python
def main(list_name, record_file_name):
    """
    预处理图片和把预处理的数据保存到tfrecords文件里面
    :param list_name: 含有图片-标签对的文本文件路径
    :param record_file_name: tfrecords的文件路径
    """
    # 读取图片-标签对，读取出来的格式为((path1，label1),(path2, label2), ...)
    lists_and_labels = np.loadtxt(list_name, dtype=str).tolist()
    # 以图片-标签对为单位，打乱数据集
    np.random.shuffle(lists_and_labels)
```

## 2.4、预处理图片并保存

本部分代码是main函数的一部分。非常简单，先上代码：

```python
# 定义一个TFRecordWriter，用来写TFRecords文件
    writer = tf.python_io.TFRecordWriter(record_file_name)

    for file_name, label in lists_and_labels:
        # 使用read_image函数读取并预处理图片，得到一个numpy数组
        img = read_image(file_name)
        # 把img的shape从_g.IMAGE_SHAPE调整为[_g.IMAGE_SIZE, ]
        img_reshape = np.reshape(img, [_g.IMAGE_SIZE, ])
        print(file_name, img.shape, img_reshape.shape)

        # 创建特征的字典，这里我们只需要标签和图像原始数据，如果要
        # 保存图片路径，再创建一个特征就行。
        feature = {
            'label': tf.train.Feature(int64_list=tf.train.Int64List(value=[int(label)])),
            'image_raw': tf.train.Feature(float_list=tf.train.FloatList(value=img_reshape.tolist()))
        }

        # 定义一个Example
        example = tf.train.Example(features=tf.train.Features(feature=feature))
		 # 把example写入到文件中
        writer.write(example.SerializeToString())

    writer.close()
```

**关于read_image**

上面代码中用到的使用OpenCV库来读取图片，然后使用numpy来完成数据类型转换，把值的范围从[0, 255]缩放到[-1.0, 1.0]，代码如下：

```python
def read_image(file_name):
    """
    读取并预处理图片。
    :param file_name: 图片的路径
    :return: numpy数组，shape为_g.IMAGE_SHAPE
    """
    # 读取图片，img为numpy数组，dtype=np.uint8
    img = cv2.imread(file_name, cv2.IMREAD_UNCHANGED)
    # 调整img的大小，只需要指定行数和列数
    img = cv2.resize(img, _g.IMAGE_SHAPE[0:2])
    # 转换img的数据类型
    img = img.astype(dtype=np.float32)
    # 把像素值的范围从[0, 255]缩放到[-1.0, 1.0]
    img -= _g.IMAGE_DEPTH / 2
    img /= _g.IMAGE_DEPTH / 2
    return img
```

**关于tf.train.Feature**

一个tf.Example由许多tf.train.Feature组成（可以这样理解）。tf.train.Feature可以接收以下三种类型的数据，其他类型的数据基本都可以转换为这三种数据：
- bytes_list（string，byte）
- float_list （float32，float64）
- int64_list（bool，enum，int32，	uint32，int64，	uint64）

为了将标准类型转换为tf.train.Feature兼容，可以使用如下函数：

```python
def _bytes_feature(value):
  """Returns a bytes_list from a string / byte."""
  return tf.train.Feature(bytes_list=tf.train.BytesList(value=[value]))

def _float_feature(value):
  """Returns a float_list from a float / double."""
  return tf.train.Feature(float_list=tf.train.FloatList(value=[value]))

def _int64_feature(value):
  """Returns an int64_list from a bool / enum / int / uint."""
  return tf.train.Feature(int64_list=tf.train.Int64List(value=[value]))
```

**需要说明的是，tf.train.*****List的value参数是一个list。** 使用上述函数的一些例子：

```python
print(_bytes_feature('test_string'))
print(_bytes_feature(bytes('test_bytes')))
print(_float_feature(np.exp(1)))
print(_int64_feature(True))
print(_int64_feature(1))
```

在文中，我保存label是使用int64_list；保存图像数据是使用float_list，之所以使用float_list，一个是为了能在读取时的代码更加简单，另外一个就是节省读取时的CPU资源，缺点是文件占用的空间比较大。

## 2.5、调用main函数

对训练集和验证集调用main函数，完成数据预处理：

```python
if __name__ == '__main__':
    main(_g.TRAIN_LIST, _g.TRAIN_TFRECORDS)
    main(_g.VAL_LIST, _g.VAL_TFRECORDS)
```

至此，数据的预处理就完成了。

# 3、读取预处理后的数据

此部分代码主要演示如何使用tf.data.TFRecordDataset读取tfrecords文件。使用TFRecordDataset即可以作为tensorflow代码所写的模型的输入，也可以作为keras模型的输入，简直美滋滋。还有其他读取tfrecords文件的代码，就不多说了。此部分的代码实现于inputs_tfrecords.py中。

## 3.1、导入库

```python
# coding=utf-8
# 兼容python3
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import numpy as np
import multiprocessing as mt
import tensorflow as tf
import cv2
import globals as _g

_g.set_seed()
```

## 3.2、定义TFRecordDataset

TFRecordDataset和tf.data.Dataset非常相似，在这里不做过多的阐述，函数的说明可参见我另外一篇博客[tf.data.Dataset图像预处理详解](https://blog.csdn.net/DumpDoctorWang/article/details/84028957)的第2部分。

```python
def prepare_dataset(record_name, list_name):
    """
    从record_name指定的TFRecords文件，初始化一个dataset
    :param record_name: TFRecords文件路径
    :param list_name: 与record_name相对应的图片-标签对文件路径
    """
    # 定义TFRecordDataset
    dataset = tf.data.TFRecordDataset([record_name])
    # 对每个dataset的每个样本调用_parse_function来读取TFRecords数据
    dataset = dataset.map(_parse_function, mt.cpu_count())
    # 定义batch size大小，非常重要。
    dataset = dataset.batch(_g.TRAIN_BATCH_SIZE)
    # 无限重复数据集
    dataset = dataset.repeat()
    # 返回dataset和训练一轮需要的步数
    return dataset, compute_steps(list_name)
```

**关于_parse_function**

_parse_function是解析TFRecords的函数，实现如下：

```python
def _parse_function(record):
    # 定义一个特征词典，和写TFRecords时的特征词典相对应
    features = {
        'label': tf.FixedLenFeature([], tf.int64, default_value=0),
        'image_raw': tf.FixedLenFeature([_g.IMAGE_SIZE, ], tf.float32,)
    }

    # 根据上面的特征解析单个数据（保存时的单个Example）
    example = tf.parse_single_example(record, features)

    # 把image的shape从[_g.IMAGE_SIZE, ]调整回_g.IMAGE_SHAPE
    image = tf.reshape(example['image_raw'], _g.IMAGE_SHAPE)

    # 如果使用dataset作为keras中，model.fit函数等的参数，则需要使用one_hot编码
    # 在tensorflow中，基本是不需要的，可以直接返回example['label']。
    one_hot_label = tf.one_hot(example['label'], _g.NUM_CLASSES)

    return image, one_hot_label
```

tf.FixedLenFeature的第一个参数为特征的长度（元素的个数），如果只有一个整数，直接传[]，后面的default_value可设置为0；如果是一个list，有很多数，那么需要指定第一个参数为该特征的长度（这个长度要和2.4节保存的数据的个数相同），default_value建议不设置。第二个参数为特征的类型。

**关于compute_steps**

compute_steps的作用就是训练一轮（one epoch）需要多少步（steps）。计算steps很简单，record_name对应的那个list_name包含多少个样本（一行一个样本，就是有多少行），然后除以batch size并向上取整就可以得到steps：

```python
def compute_steps(list_name):
    # 读取所有的图片-标签对
    lists_and_labels = np.loadtxt(list_name, dtype=str).tolist()
	# 除以batch size并向上取整
    return np.ceil(len(list(lists_and_labels)) / _g.TRAIN_BATCH_SIZE).astype(np.int32)
```

## 3.3、验证是否成功读取了数据

验证预处理的是否正确的方式比较简单，总体思路是从dataset获取图像和标签，然后保存图像，看看对不对。

```python
def save_image(file_name, image):
    """
    保存image到file_name指定的位置
    """
    # 把图像的值范围从[-1.0, 1.0] 缩放回 [0, 255]
    image *= _g.IMAGE_DEPTH / 2
    image += _g.IMAGE_DEPTH / 2
    # 转换类型
    image = image.astype(dtype=np.uint8)
    # 保存图片
    cv2.imwrite(file_name, image)
    
def inputs_test():
    dataset, steps = prepare_dataset(_g.TRAIN_TFRECORDS, _g.TRAIN_LIST)

    print('shapes:', dataset.output_shapes)
    print('types:', dataset.output_types)
    print('steps: ', steps)

    next_op = dataset.make_one_shot_iterator().get_next()
    with tf.Session() as sess:
        for i in range(10):
            image, label = sess.run(next_op)
            print(image.shape, label.shape)
            save_image('logs/%d.png' % i, image[0])
```

## 3.3、在tensorflow模型中使用

思路为从dataset中取出数据，然后作为sess.run的feed_dict的参数：

```python
import inputs_tfrecords
...

def train():
    # 训练集
    dataset, steps = inputs_tfrecords.prepare_dataset(_g.TRAIN_TFRECORDS, _g.TRAIN_LIST)
    # 验证集
    val_dataset, val_steps = inputs_tfrecords.prepare_dataset(_g.VAL_TFRECORDS, _g.VAL_LIST)

    print('shapes:', dataset.output_shapes)
    print('types:', dataset.output_types)
    print('steps: ', steps)

    # 计算shape
    shape = _g.IMAGE_SHAPE[:]
    shape.insert(0, _g.TRAIN_BATCH_SIZE)
    # 定义placeholder
    img = tf.placeholder(shape=shape, name='image')
    lab = tf.placeholder(shape=[_g.TRAIN_BATCH_SIZE, ], name='label')
    # 定义训练操作
    train_op = ...


    # 训练
    next_op = dataset.make_one_shot_iterator().get_next()
    with tf.Session() as sess:
        for i in range(steps):
            image, label = sess.run(next_op)
            print(image.shape, label.shape)
            sess.run([train_op], feed_dict={'image': image, 'label': label})
            ...
```

## 3.5、在Keras中使用

那真的是非常简单了：

```python
import inputs_tfrecords
...

def train():
    # 训练集
    dataset, steps = inputs_tfrecords.prepare_dataset(_g.TRAIN_TFRECORDS, _g.TRAIN_LIST)
    # 验证集
    val_dataset, val_steps = inputs_tfrecords.prepare_dataset(_g.VAL_TFRECORDS, _g.VAL_LIST)

    print('shapes:', dataset.output_shapes)
    print('types:', dataset.output_types)
    print('steps: ', steps)

    # 得到模型
    model = tf.keras.Sequential()
    ...
    # 训练
    model.fit(train_dataset, epochs=_g.NUM_TRAIN_EPOCH, steps_per_epoch=train_steps,
              validation_data=val_dataset, validation_steps=val_steps)
```

关于tensorflow中如何使用keras，可参考：
- [Tensorflow keras入门教程](https://blog.csdn.net/DumpDoctorWang/article/details/83718054)
- [基于Keras mnist手写数字识别—Keras卷积神经网络入门教程](https://blog.csdn.net/DumpDoctorWang/article/details/83831185)




# tf.data.Dataset图像预处理详解 - DumpDoctorWang的博客 - CSDN博客





2018年11月13日 22:02:12[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1476











### 目录
- [1、tf.data.Dataset](#1tfdataDataset_1)
- [2、Dataset常用函数](#2Dataset_8)
- [3、图像预处理的第一种方式](#3_50)
- [3.1、导入依赖库](#31_66)
- [3.2、定义常量](#32_80)
- [3.3、读取文本中的图片标签对](#33_93)
- [3.4、实例化Dataset并完成图像预处理](#34Dataset_106)
- [3.5、从Dataset中获取数据](#35Dataset_135)
- [3.6、处理需要预测的样本](#36_165)
- [4、使用tf.py_func进行图片预处理](#4tfpy_func_191)
- [4.1、来个例子](#41_243)
- [4.2、导入依赖库](#42_271)
- [4.3、定义常量](#43_287)
- [4.4、定义Dataset](#44Dataset_304)
- [4.5、读取物体模型列表](#45_332)
- [4.5、图片的预处理操作](#45_352)
- [4.6、从Dataset读取数据](#46Dataset_381)
- [4.7、获取预测样本](#47_400)
- [5、两种方法的完整代码](#5_420)
- [5.1、第一种方法](#51_422)
- [5.2、第二种方法](#52_522)




# 1、tf.data.Dataset

当训练集的样本特别大时， 比较适合tf.data.Dataset作为数据输入管线，相当方便。然而真正在使用tf.data.Dataset时，还是有许多坑，在这里写出来，当作参考。由于我只涉及图像处理，本文专注于图像预处理相关内容。当然，本文对了解tf.data.Dataset也有很好的参考意义。

本文的第二部分主要讲一些讲一些Dataset的常用函数；第三部分讲了使用Tensorflow原生API来完成图片预处理的方法；第四部分是使用tf.py_func来完成任意逻辑的预处理；第五部分是例子的完整代码。实际上，还有另外一种预处理数据的方法，就是先用不涉及tensorflow的纯python代码来完成预处理，然后把处理后的数据（比如Numpy数组）存到硬盘上，然后再使用tf.py_func使用相同的逻辑来读取处理后的数据，这样就不用每次训练都预处理数据了。

除了使用tf.data.Dataset以外，还可以使用TFRecords进行数据预处理，可参考博客：[TFRecords详解\TFRecords图像预处理](https://blog.csdn.net/DumpDoctorWang/article/details/84798594) 。

参考链接：- [导入数据 | TensorFlow](https://www.tensorflow.org/programmers_guide/datasets?hl=zh-cn)。

# 2、Dataset常用函数

先来看一个例子

```python
# 读取filename指定的图像，并调整其大小。label是其对应的标签
def _parse_function(filename, label):
    image_string = tf.read_file(filename)
    # 读取图片
    image_decoded = tf.image.decode_image(image_string)
    # 调整大小
    image_resized = tf.image.resize_images(image_decoded, [28, 28])
    return image_resized, label

# 图像名称组成的常量tensor
filenames = tf.constant(["data/image1.jpg", "data/image2.jpg", ...])
# 图像标签。`labels[i]`-->`filenames[i].
labels = tf.constant([0, 37, ...])
# 定义一个Dataset实例
dataset = tf.data.Dataset.from_tensor_slices((filenames, labels))
# 对dataset中的每一对（filename， label）调用_parse_function进行处理
dataset = dataset.map(_parse_function)
# 设置每批次的大小
dataset = dataset.batch(batch_size=32)
# 无限重复数据集
dataset = dataset.repeat()
```
- **tf.data.Dataset.from_tensor_slices((data, labels))**

创建一个Dataset实例。如果函数的参数是NumPy数组，并且未启用Eager Execution，则值将作为一个或多个tf.constant操作嵌入到graph中。 对于大型数据集（> 1 GB），这可能会浪费内存并超过graph序列化（保存模型的时候需要序列化）的字节限制。 如果函数的参数包含一个或多个大型NumPy阵列，请参考[替代方案](https://www.tensorflow.org/programmers_guide/datasets?hl=zh-cn#consuming_numpy_arrays)。
- **tf.data.Dataset.map(f, num_parallel_calls)**

Dataset.map 转换通过将函数 f 应用于输入数据集的每对元素（data， label）来生成新数据集。比如在上面的例子中，就是把（filename， label）中filename指定的图像读取出来并调整大小。

num_parallel_calls指定使用多少个线程来进行map操作。可以设置为CPU的最大核心数目（=multiprocessing.cpu_count()）。如果不指定的话，只使用一个线程处理数据。- **tf.data.Dataset.batch(batch_size)**
**这个函数特别重要。** 假如输入图像大小为（227，227，3），模型的输入shape为（None，227，227，3），其中None是batch_size。如果不调用这个函数，那么从dataset获取一批数据时，返回的数据shape为（227，227，3），输入到模型维度肯定匹配不上，就会出现如下类似的错误：
`Index out of range using input dim 4; input has only 4 dims`

或者
`Error when checking target: expected softmax to have 2 dimensions, but got array with shape (250,)`

如果调用了这个函数，再从dataset获取一批数据时，返回的数据shape为（batch_size，227，227，3），就能和模型的输入shape匹配上了。- **tf.data.Dataset.repeat(count)**

重复这个数据集多少次。如果不传count这个参数，默认会无限重复这个数据集。加入count=1，那么当你训练完一轮之后，就会报错`tensorflow.python.framework.errors_impl.OutOfRangeError: End of sequence`。在实际使用中，基本可以不传count参数，无限重复这个数据集。

常用函数还有tf.data.Dataset.shuffle()，是用来打乱数据集的。另外还需要注意的是：**这些函数都是返回调用该操作之后的一个Dataset实例，并没有在本身上应用该操作。**

# 3、图像预处理的第一种方式

首先说下需求，主要是需要训练一个分类模型。训练集放在一个txt文本中，每一行是由图片和标签组成，一部分如下

```
data/test/001/001_01_01_051_09.png 0
data/test/001/001_01_01_051_10.png 0
data/test/002/002_01_01_051_19.png 1
data/test/002/002_01_01_051_09.png 1
data/test/003/003_01_01_051_14.png 2
data/test/003/003_01_01_051_03.png 2
data/test/004/004_01_01_051_05.png 3
data/test/004/004_01_01_051_06.png 3
...
```

现在需要把文本中的图片路径和标签读入到一个Dataset里面。然后使用Dataset.map调用预处理函数，读取图片，并完成预处理。

## 3.1、导入依赖库

```python
# coding=utf-8
# 兼容python3
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import multiprocessing as mt
import numpy as np
import tensorflow as tf
from tensorflow import keras
```

## 3.2、定义常量

```python
# 分类问题，总共有250个类
NUM_CLASSES = 250
# 训练批次大小
TRAIN_BATCH_SIZE = 128
# 图像每个像素的每个通道的最大值，对于8位图像，就是255
IMAGE_DEPTH=255
# 包含训练集的文本
TRAIN_LIST = 'data/train.txt'
```

## 3.3、读取文本中的图片标签对

```python
# 读取由path指定的文本文件，并返回由很多(图片路径,标签)组成的列表
lists_and_labels = np.loadtxt(path, dtype=str).tolist()
# 打乱下lists_and_labels
np.random.shuffle(lists_and_labels)
# 把图片路径和标签分开
list_files, labels = zip(*[(l[0], int(l[1])) for l in lists_and_labels])
# 如果使用keras构建模型，还需要对标签进行one_hot编码，如果使用tensorflow构建的模型，则不需要。
one_shot_labels = keras.utils.to_categorical(labels, NUM_CLASSES).astype(dtype=np.int32)
```

## 3.4、实例化Dataset并完成图像预处理

```python
# 定义数据集实例
dataset = tf.data.Dataset.from_tensor_slices((tf.constant(list_files), tf.constant(one_shot_labels)))
# 对每一对 (image, label)调用_parse_image，完成图像的预处理
dataset = dataset.map(_parse_image, num_parallel_calls=mt.cpu_count())
# 设置训练批次大小。非常重要！！！
dataset = dataset.batch(TRAIN_BATCH_SIZE)
# 无限重复数据集
dataset = dataset.repeat()
# 计算遍历一遍数据集需要多少步
steps_per_epoch = np.ceil(len(labels) / TRAIN_BATCH_SIZE).astype(np.int32)
return dataset, steps_per_epoch
```

_parse_image函数需要实现的是：读取图片，调整大小，并将图像像素值的范围从[0, 255]缩放到[-0.5, 0.5]。_parse_image不能直接调用其他库来实现功能，只能使用tensorflow中预定的操作来完成所需要的功能。实现如下：

```python
def _parse_image(filename, label):
    # 读取并解码图片
    image_string = tf.read_file(filename)
    image_decoded = tf.image.decode_image(image_string)
    # 一定要在这里转换类型！！！
    image_converted = tf.cast(image_decoded, tf.float32)
    # 缩放范围
    image_scaled = tf.divide(tf.subtract(image_converted, IMAGE_DEPTH/2), IMAGE_DEPTH)
    return image_scaled, label
```

至此dataset就可以作为model.fit和model.evaluate（keras中）的参数了。

## 3.5、从Dataset中获取数据

在使用Dataset作为使用tensorflow编写的模型的输入时，需要把数据取出来，作为feed_dict的参数的数据。**另外，在使用Dataset作为模型输入时，需要看看数据预处理的结果对不对，把数据取出来，看看实际的数据是否符合预期。**

```python
# 打印dataset的相关信息
print('shapes:', dataset.output_shapes)
print('types:', dataset.output_types)
print('steps:', steps)
# 获取一个用来迭代数据的iterator
data_it = dataset.make_one_shot_iterator()
# 定义个获取下一组数据的操作(operator)
next_data = data_it.get_next()
# 新建Session
with tf.Session() as sess:
    # 获取前10批数据
    for i in range(10):
        # 获取一批图片和对应的标签
        data, label = sess.run(next_data)
        # 打印数据的长度，标签的长度，数据的shape，数据的最大值和最小值
        print(len(data), len(label), data.shape, np.min(data), np.max(data))
```

运行上面的程序，输出类似于

```python
128 128 (128, 227, 227, 3) -0.5 0.5
128 128 (128, 227, 227, 3) -0.49607846 0.5
128 128 (128, 227, 227, 3) -0.5 0.5
128 128 (128, 227, 227, 3) -0.49607846 0.5
...
```

## 3.6、处理需要预测的样本

预测（predict）样本时，在预处理图片时，**预处理的操作一定要和训练时的相同**，否则评估或者预测的结果是不对的。在上面的方法中，预处理的代码为：

```python
def read_image(filename):
    with tf.Session() as sess:
        read_op = _parse_image(tf.constant(filename, dtype=tf.string), tf.constant(0))
        image, label = sess.run(read_op)
        return image
        
image = read_image('data/test/001/001_01_01_051_09.png')
print('shape: ', image.shape)
```

在使用model.predict（keras）时，还需要扩展image的维度为四维，代码如下

```python
# 读图片
image = read_image('data/train/022/022_01_01_051_00.png')
# 扩展维度为 (1, 227, 227, 3)
image = image[np.newaxis, :]
print(image.shape)
....
# 预测
model = ...
softmax = model.predict(image, 1)
print(np.argmax(softmax))
```

# 4、使用tf.py_func进行图片预处理

有时候，需要完成特别复杂的预处理的时候，无法使用tensorflow内置的操作完成预处理，就可以使用tf.py_func来完成任意逻辑的预处理。先来个例子：

```python
# coding=utf-8
import cv2
import tensorflow as tf

# 使用OpenCV代码来完成读取图片，在这个函数里，你可以使用任意的python库来完成任意操作
def _read_py_function(filename, label):
    image_decoded = cv2.imread(filename.decode(), cv2.IMREAD_UNCHANGED)
    return image_decoded, label

# 读取图片
def _read_image_caller(filename, label):
    return tf.py_func(_read_py_function, [filename, label], [tf.uint8, label.dtype])
    
# 使用标准TensorFlow操作来调整图片大小
def _resize_function(image_decoded, label):
    # 由于无法从image_decoded推断shape，所以要先手动设定
    image_decoded.set_shape([None, None, None])
    # 调整大小
    image_resized = tf.image.resize_images(image_decoded, [28, 28])
    return image_resized, label

filenames = ["data/train/001/001_01_01_051_04.png", "data/train/001/001_01_01_051_05.png", ]
labels = [0, 37, ]

# 定义dataset对象
dataset = tf.data.Dataset.from_tensor_slices((filenames, labels))

# 调用map，完成图片读取
dataset = dataset.map(_read_image_caller)

# 再次调用map，完成图片的调整大小的操作
dataset = dataset.map(_resize_function)

# 定义获取数据的tensorflow操作
next_op = dataset.make_one_shot_iterator().get_next()

with tf.Session() as sess:
    for _ in range(len(labels)):
        # 获取下一组image,label图像对
        image, label = sess.run(next_op)
        print image.shape, label
```

tf.py_func的作用是把一个普通的python函数包装（wrap）为tensorflow操作（类似于tf.read_file之类的），主要参数如下
- func: 指定要包裹的普通python函数F。
- inp: F所需要的参数组成的列表。
- Tout: 指定F返回值的类型。

## 4.1、来个例子

假如需要训练物体分类模型，每个物体有12张图片。模型输入的shape为（None，12， 227，227，3），其中的None是批次的大小，12是一个物体模型有12张图片，（227，227，3）是一张图像的大小。所以预处理的要求是，把一个物体的12张图片读进来，完成调整大小，缩放像素值的范围到[-0.5, 0.5]，并叠在一起（shape为（12，227，227，3））。物体模型的列表和标签train.txt如下：

```
data/train/001/list.txt 0
data/train/002/list.txt 1
data/train/003/list.txt 2
data/train/004/list.txt 3
data/train/005/list.txt 4
...
```

每一行的一个list.txt指定了一个物体模型的12张图片，其中的一个如下：

```
data/train/001/001_01_01_051_14.png
data/train/001/001_01_01_051_19.png
data/train/001/001_01_01_051_18.png
data/train/001/001_01_01_051_10.png
data/train/001/001_01_01_051_07.png
data/train/001/001_01_01_051_16.png
data/train/001/001_01_01_051_04.png
data/train/001/001_01_01_051_17.png
data/train/001/001_01_01_051_13.png
data/train/001/001_01_01_051_15.png
data/train/001/001_01_01_051_11.png
data/train/001/001_01_01_051_05.png
```

## 4.2、导入依赖库

```python
# coding=utf-8
# 兼容python3
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import cv2
import numpy as np
import tensorflow as tf
import multiprocessing as mt
from tensorflow import keras
```

## 4.3、定义常量

```python
# 分类问题，总共有40个类
NUM_CLASSES = 40
# 训练批次大小
TRAIN_BATCH_SIZE = 1
# 图像每个像素的每个通道的最大值，对于8位图像，就是255
IMAGE_DEPTH=255
# 包含训练集的文本
TRAIN_LIST = 'data/train.txt'
# 一个物体有12张图片
NUM_VIEWS = 12
# 一张图的大小
IMAGE_SHAPE = (227, 227, 3)
```

## 4.4、定义Dataset

这里我这给出定义Dataset的函数

```python
def prepare_dataset(path=''):
    # 读取物体模型列表
    lists_and_labels = np.loadtxt(path, dtype=str).tolist()
    # 打乱数据
    np.random.shuffle(lists_and_labels)
    # 分为模型列表和标签
    list_files, labels = zip(*[(l[0], int(l[1])) for l in lists_and_labels])
    # 对标签进行one_hot编码
    one_shot_labels = keras.utils.to_categorical(labels, NUM_CLASSES).astype(dtype=np.int32)
    # 定义数据集
    dataset = tf.data.Dataset.from_tensor_slices((tf.constant(list_files), tf.constant(one_shot_labels)))
    # 读取每个模型的12张图片的路径
    dataset = dataset.map(read_object_caller, num_parallel_calls=mt.cpu_count())
    # 调整每张图片的大小，转换图片的数据类型为float32,并将12张图片堆叠到一起
    dataset = dataset.map(read_resize_concat, num_parallel_calls=mt.cpu_count())
    # 非常重要，记得要设置批次大小
    dataset = dataset.batch(TRAIN_BATCH_SIZE)
    # 无限重复
    dataset = dataset.repeat()
    # 计算每次迭代需要多少步
    steps_per_epoch = np.ceil(len(labels)/TRAIN_BATCH_SIZE).astype(np.int32)
    return dataset, steps_per_epoch
```

## 4.5、读取物体模型列表

我在写代码的时候，读取一个物体的12张图片的路径列表花了很久很久，就是因为不知道tf.py_func这个神器，接下来的代码，就是如何读取一个物体模型的列表。

```python
def read_object_caller(filename, label):
    # 使用tf.py_func调用一个普通python函数来读取一个物体的12张图片路径
    # 注意返回值的类型是[tf.string, label.dtype]。
    return tf.py_func(read_object_list, [filename, label], [tf.string, label.dtype])

def read_object_list(filename, label):
    # 读取一个物体模型的列表
    image_lists = np.loadtxt(filename.decode(), dtype=str)
    # 截取前NUM_VIEWS个图片路径
    image_lists = image_lists[:NUM_VIEWS]
    # 如果图片路径没有NUM_VIEWS个，抛出错误
    if len(image_lists) != NUM_VIEWS:
        raise ValueError('There haven\'t %d views in %s ' % (NUM_VIEWS, filename))
    # 返回图片列表与标签
    return image_lists, label
```

## 4.5、图片的预处理操作

```python
def read_resize_concat(image_list, label):
    # image_list是物体模型的12张图片路径的列表
    # 下面这个函数就是处理列表中的每一个图像的函数
    def process_one_image(image):
        # 读取图片并解码
        image_string = tf.read_file(image)
        image_decoded = tf.image.decode_image(image_string)
        # 由于无法从image_decoded推断shape，所以要先手动设定，否则resize_images会报错
        image_decoded.set_shape([None, None, None])
        # 调整大小
        image_resized = tf.image.resize_images(image_decoded, IMAGE_SHAPE[0:2])
        # 转换图像像素类型
        image_converted = tf.cast(image_resized, tf.float32)
        # 把像素值的范围从[0, 255]缩放到[-0.5, 0.5]
        image_scaled = tf.divide(tf.subtract(image_converted, IMAGE_DEPTH / 2), IMAGE_DEPTH)
        return image_scaled

    # 调用tf.map_fn对image_list的每个元素，也就是一张图片的路径，调用process_one_image函数，完成
    # 对一张图片的预处理，并返回一个处理后的list
    image_prepocessed_list = tf.map_fn(process_one_image, image_list, dtype=tf.float32)
    # 把12个处理后图片在维度0上堆叠起来，一张图片的shape为（227, 227, 3），堆叠后的shape为（12, 227,227,3）
    concat = tf.concat(image_prepocessed_list, axis=0)
    return concat, label
```

注意：tf.image.decode_image返回的image_decoded没有shape，如果直接对image_decoded调用tf.image.resize_images，会出现如下错误`ValueError: 'images' contains no shape.`

## 4.6、从Dataset读取数据

```python
def inputs_test():
    dataset, steps = prepare_dataset(TRAIN_LIST)
    print('shapes:', dataset.output_shapes)
    print('types:', dataset.output_types)
    print('steps:', steps)
    next_op = dataset.make_one_shot_iterator().get_next()

    with tf.Session() as sess:
        for i in range(5):
            data, label = sess.run(next_op)
            print(len(data), len(label), data.shape, np.min(data), np.max(data))

if __name__ == '__main__':
    inputs_test()
```

## 4.7、获取预测样本

同样，在预测时，样本数据需要经过和训练数据同样的预处理，代码如下：

```python
def process_one_sample(path):
    label = 0
    # 读取图片列表
    image_list, _ = read_object_list(path, label)
    # 定义处理操作
    process_op = read_resize_concat(tf.constant(image_list), tf.constant(label))
    # 处理
    with tf.Session() as sess:
        concat_image, _ = sess.run(process_op)
        return concat_image

if __name__ == '__main__':
    concat_image = process_one_sample('data/train/004/list.txt')
    print(concat_image.shape)
```

# 5、两种方法的完整代码

数据我就不提供了，自行准备吧。

## 5.1、第一种方法

```python
# coding=utf-8
# 兼容python3
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import multiprocessing as mt

import numpy as np
import tensorflow as tf
from tensorflow import keras

# 分类问题，总共有250个类
NUM_CLASSES = 250
# 训练批次大小
TRAIN_BATCH_SIZE = 128
# 图像每个像素的每个通道的最大值，对于8位图像，就是255
IMAGE_DEPTH = 255
# 包含训练集的文本
TRAIN_LIST = 'data/train.txt'


def prepare_dataset(path=''):
    """
    prepaer dataset using tf.data.Dataset
    :param path: the list file like data/train_lists_demo.txt
    and data/val_lists_demo.txt
    :return: a Dataset object
    """
    # read image list files name and labels
    lists_and_labels = np.loadtxt(path, dtype=str).tolist()
    # shuffle dataset
    np.random.shuffle(lists_and_labels)
    # split lists an labels
    list_files, labels = zip(*[(l[0], int(l[1])) for l in lists_and_labels])
    # one_shot encoding on labels
    one_shot_labels = keras.utils.to_categorical(labels, NUM_CLASSES).astype(dtype=np.int32)
    # make data set
    dataset = tf.data.Dataset.from_tensor_slices((tf.constant(list_files), tf.constant(one_shot_labels)))
    # perform function parse_image on each pair of (data, label)
    dataset = dataset.map(_parse_image, num_parallel_calls=mt.cpu_count())
    # set the batch size, Very important function!
    dataset = dataset.batch(TRAIN_BATCH_SIZE)
    # repeat forever
    dataset = dataset.repeat()
    # compute steps_per_epoch
    steps_per_epoch = np.ceil(len(labels) / TRAIN_BATCH_SIZE).astype(np.int32)
    return dataset, steps_per_epoch


def _parse_image(filename, label):
    """
    read and pre-process image
    """
    image_string = tf.read_file(filename)
    image_decoded = tf.image.decode_image(image_string)
    # must convert dtype here !!!
    image_converted = tf.cast(image_decoded, tf.float32)
    image_scaled = tf.divide(tf.subtract(image_converted, IMAGE_DEPTH / 2), IMAGE_DEPTH)
    return image_scaled, label


def read_image(filename):
    """
    read image defined by filename
    :param filename: the path of a image
    :return: a numpy array
    """
    with tf.Session() as sess:
        read_op = _parse_image(tf.constant(filename, dtype=tf.string), tf.constant(0))
        image, label = sess.run(read_op)
        return image


def inputs_test():
    """
    test function prepare_dataset
    """
    dataset, steps = prepare_dataset(TRAIN_LIST)
    print('shapes:', dataset.output_shapes)
    print('types:', dataset.output_types)
    print('steps:', steps)
    data_it = dataset.make_one_shot_iterator()
    next_data = data_it.get_next()

    with tf.Session() as sess:
        for i in range(10):
            data, label = sess.run(next_data)
            print(len(data), len(label), data.shape, np.min(data), np.max(data))


if __name__ == '__main__':
    inputs_test()
    read_image('data/test/001/001_01_01_051_09.png')
```

## 5.2、第二种方法

```python
# coding=utf-8
# 兼容python3
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import cv2
import numpy as np
import tensorflow as tf
import multiprocessing as mt
from tensorflow import keras

# 分类问题，总共有40个类
NUM_CLASSES = 40
# 训练批次大小
TRAIN_BATCH_SIZE = 1
# 图像每个像素的每个通道的最大值，对于8位图像，就是255
IMAGE_DEPTH=255
# 包含训练集的文本
TRAIN_LIST = 'data/train.txt'
# 一个物体有12张图片
NUM_VIEWS = 12
# 一张图的大小
IMAGE_SHAPE = (227, 227, 3)


def prepare_dataset(path=''):
    # 读取物体模型列表
    lists_and_labels = np.loadtxt(path, dtype=str).tolist()
    # 打乱数据
    np.random.shuffle(lists_and_labels)
    # 分为模型列表和标签
    list_files, labels = zip(*[(l[0], int(l[1])) for l in lists_and_labels])
    # 对标签进行one_hot编码
    one_shot_labels = keras.utils.to_categorical(labels, NUM_CLASSES).astype(dtype=np.int32)
    # 定义数据集
    dataset = tf.data.Dataset.from_tensor_slices((tf.constant(list_files), tf.constant(one_shot_labels)))
    # 读取每个模型的12张图片的路径
    dataset = dataset.map(read_object_caller, num_parallel_calls=mt.cpu_count())
    # 调整每张图片的大小，转换图片的数据类型为float32,并将12张图片堆叠到一起
    dataset = dataset.map(read_resize_concat, num_parallel_calls=mt.cpu_count())
    # 非常重要，记得要设置批次大小
    dataset = dataset.batch(TRAIN_BATCH_SIZE)
    # 无限重复
    dataset = dataset.repeat()
    # 计算每次迭代需要多少步
    steps_per_epoch = np.ceil(len(labels)/TRAIN_BATCH_SIZE).astype(np.int32)
    return dataset, steps_per_epoch


def read_object_caller(filename, label):
    # 使用tf.py_func调用一个普通python函数来读取一个物体的12张图片路径
    # 注意返回值的类型是[tf.string, label.dtype]。
    return tf.py_func(read_object_list, [filename, label], [tf.string, label.dtype])


def read_object_list(filename, label):
    # 读取一个物体模型的列表
    image_lists = np.loadtxt(filename.decode(), dtype=str)
    # 截取前NUM_VIEWS个图片路径
    image_lists = image_lists[:NUM_VIEWS]
    # 如果图片路径没有NUM_VIEWS个，抛出错误
    if len(image_lists) != NUM_VIEWS:
        raise ValueError('There haven\'t %d views in %s ' % (NUM_VIEWS, filename))
    # 返回图片列表与标签
    return image_lists, label


def read_resize_concat(image_list, label):
    # image_list是物体模型的12张图片路径的列表
    # 下面这个函数就是处理列表中的每一个图像的函数
    def process_one_image(image):
        # 读取图片并解码
        image_string = tf.read_file(image)
        image_decoded = tf.image.decode_image(image_string)
        # 由于无法从image_decoded推断shape，所以要先手动设定，否则resize_images会报错
        image_decoded.set_shape([None, None, None])
        # 调整大小
        image_resized = tf.image.resize_images(image_decoded, IMAGE_SHAPE[0:2])
        # 转换图像像素类型
        image_converted = tf.cast(image_resized, tf.float32)
        # 把像素值的范围从[0, 255]缩放到[-0.5, 0.5]
        image_scaled = tf.divide(tf.subtract(image_converted, IMAGE_DEPTH / 2), IMAGE_DEPTH)
        return image_scaled

    # 调用tf.map_fn对image_list的每个元素，也就是一张图片的路径，调用process_one_image函数，完成
    # 对一张图片的预处理，并返回一个处理后的list
    image_prepocessed_list = tf.map_fn(process_one_image, image_list, dtype=tf.float32)
    # 把12个处理后图片在维度0上堆叠起来，一张图片的shape为（227, 227, 3），堆叠后的shape为（12, 227,227,3）
    concat = tf.concat(image_prepocessed_list, axis=0)
    return concat, label


def inputs_test():
    """
    test function prepare_dataset
    """
    dataset, steps = prepare_dataset(TRAIN_LIST)
    print('shapes:', dataset.output_shapes)
    print('types:', dataset.output_types)
    print('steps:', steps)
    next_op = dataset.make_one_shot_iterator().get_next()

    with tf.Session() as sess:
        for i in range(5):
            data, label = sess.run(next_op)
            print(len(data), len(label), data.shape, np.min(data), np.max(data))


def process_one_sample(path):
    label = 0
    # 读取图片列表
    image_list, _ = read_object_list(path, label)
    # 定义处理操作
    process_op = read_resize_concat(tf.constant(image_list), tf.constant(label))
    # 处理
    with tf.Session() as sess:
        concat_image, _ = sess.run(process_op)
        return concat_image


if __name__ == '__main__':
    inputs_test()
    concat_image = process_one_sample('data/train/004/list.txt')
    print(concat_image.shape)
```







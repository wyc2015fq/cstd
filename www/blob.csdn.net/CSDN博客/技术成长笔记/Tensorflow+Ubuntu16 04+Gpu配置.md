# Tensorflow+Ubuntu16.04+Gpu配置 - 技术成长笔记 - CSDN博客





2017年02月26日 22:03:21[zhixuhao](https://me.csdn.net/u012931582)阅读数：1894








## 显卡驱动安装

对于还没有安装N卡驱动的同学,建议通过系统自带程序进行安装,搜索附加驱动: 
![这里写图片描述](https://img-blog.csdn.net/20170226214729736?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

选择367.57 
![这里写图片描述](https://img-blog.csdn.net/20170226214742170?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击应用更改,之后重新启动,应用生效,打开终端输入命令nvidia-smi,如果出现如下界面,则说明安装成功:

![这里写图片描述](https://img-blog.csdn.net/20170226214855051?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## cuda安装

首先下载cuda8.0,注意一定要**选择runfile安装方式**!!!!!

也可以在我的云盘上[下载](https://pan.baidu.com/s/1pKP9gfx)  密码: c3bp

下载之后,执行安装, 

注意在安装过程中会提醒是否安装显卡驱动,一定要选择否,其他的都选则是,即可.

之后测试一下cuda是否安装成功,打开/usr/local/cuda/samples/1_Utilities/deviceQuery, make之后执行./deviceQuery,如果输出如下则说明安装成功:

![这里写图片描述](https://img-blog.csdn.net/20170226220425354?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## cudnn安装

首先下载cudnn5.1,可以在我的云盘上[下载](https://pan.baidu.com/s/1i59g9xj)密码: si9e

解压之后,执行以下命令:

解压之后复制文件

```
sudo cp cuda/include/cudnn.h /usr/local/cuda/include/
sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64/
```

然后更改权限:

```
sudo chmod a+r /usr/local/cuda/include/cudnn.h
sudo chmod a+r /usr/local/cuda/lib64/libcudnn*
```

之后需要配置环境变量,

```
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/cuda/lib64:/usr/local/cuda/extras/CUPTI/lib64"
export CUDA_HOME=/usr/local/cuda
```

加到~./bashrc 文件末尾就好了

## tensorflow安装

这一步非常简单,可以参考github官网  

建议采用pip安装,没有pip的同学可以安装一下, sudo   apt-get install python-pip

然后pip install tensorflow-gpu 就完成了tensorflow的安装,非常简单.

我在这里提供几个测试程序,这是线性回归的程序:

```
import tensorflow as tf
import numpy as np

x_data = np.float32(np.random.rand(2, 100))
y_data = np.dot([0.100, 0.200], x_data) + 0.300

b = tf.Variable(tf.zeros([1]))
W = tf.Variable(tf.random_uniform([1, 2], -1.0, 1.0))
y = tf.matmul(W, x_data) + b

loss = tf.reduce_mean(tf.square(y - y_data))
optimizer = tf.train.GradientDescentOptimizer(0.5)
train = optimizer.minimize(loss)

init = tf.initialize_all_variables()
sess = tf.Session()
sess.run(init)

for step in xrange(0, 201):
    sess.run(train)
    if step % 20 == 0:
        print step, sess.run(W), sess.run(b)
```

输出应该如下:

![这里写图片描述](https://img-blog.csdn.net/20170226221558074?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这是用cnn进行图片分类的程序:

```
import tensorflow as tf  
import sys  
import input_data

def weight_variable(shape):  
  initial = tf.truncated_normal(shape, stddev=0.1)  
  return tf.Variable(initial)  

def bias_variable(shape):  
  initial = tf.constant(0.1, shape=shape)  
  return tf.Variable(initial)  

def conv2d(x, W):  
  return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')  

def max_pool_2x2(x):  
  return tf.nn.max_pool(x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')  

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)  

sess = tf.InteractiveSession()  

x = tf.placeholder("float", shape=[None, 784])  
y_ = tf.placeholder("float", shape=[None, 10])  

W = tf.Variable(tf.zeros([784,10]))  
b = tf.Variable(tf.zeros([10]))  

W_conv1 = weight_variable([5, 5, 1, 32])  
b_conv1 = bias_variable([32])  

x_image = tf.reshape(x, [-1, 28, 28, 1])  

h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)  
h_pool1 = max_pool_2x2(h_conv1)  

W_conv2 = weight_variable([5, 5, 32, 64])  
b_conv2 = bias_variable([64])  

h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)  
h_pool2 = max_pool_2x2(h_conv2)  

# Now image size is reduced to 7*7  
W_fc1 = weight_variable([7 * 7 * 64, 1024])  
b_fc1 = bias_variable([1024])  

h_pool2_flat = tf.reshape(h_pool2, [-1, 7*7*64])  
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)  

keep_prob = tf.placeholder("float")  
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)  

W_fc2 = weight_variable([1024, 10])  
b_fc2 = bias_variable([10])  
y_conv=tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)  


cross_entropy = -tf.reduce_sum(y_*tf.log(y_conv))  
train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)  
correct_prediction = tf.equal(tf.argmax(y_conv,1), tf.argmax(y_,1))  
accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))  
sess.run(tf.initialize_all_variables())  

for i in range(2000):  
  batch = mnist.train.next_batch(50)  
  if i%100 == 0:  
    train_accuracy = accuracy.eval(feed_dict={  
        x:batch[0], y_: batch[1], keep_prob: 1.0})  
    print "step %d, training accuracy %.3f"%(i, train_accuracy)  
  train_step.run(feed_dict={x: batch[0], y_: batch[1], keep_prob: 0.5})  

print "Training finished"  

##### out of memory, so test 10 batch  
#print "test accuracy %g"%accuracy.eval(feed_dict={
#    x: mnist.test.images, y_: mnist.test.labels, keep_prob: 1.0})
for i in xrange(10):
    testSet = mnist.test.next_batch(50)
    print("test accuracy %g"%accuracy.eval(feed_dict={ x: testSet[0], y_: testSet[1], keep_prob: 1.0}))
```

输出应该如下,注意,我验证结果是分批验证的,这是因为如果所有测试数据同时验证的时候,我的内存不够,就会报错,这和配置以及程序无关,主要就是看自己显存:

![这里写图片描述](https://img-blog.csdn.net/20170226221711559?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中的input_data模块如下:

```
# Copyright 2015 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================
"""Functions for downloading and reading MNIST data."""
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import gzip
import os
import tensorflow.python.platform
import numpy
from six.moves import urllib
from six.moves import xrange  # pylint: disable=redefined-builtin
import tensorflow as tf
SOURCE_URL = 'http://yann.lecun.com/exdb/mnist/'
def maybe_download(filename, work_directory):
  """Download the data from Yann's website, unless it's already here."""
  if not os.path.exists(work_directory):
    os.mkdir(work_directory)
  filepath = os.path.join(work_directory, filename)
  if not os.path.exists(filepath):
    filepath, _ = urllib.request.urlretrieve(SOURCE_URL + filename, filepath)
    statinfo = os.stat(filepath)
    print('Successfully downloaded', filename, statinfo.st_size, 'bytes.')
  return filepath
def _read32(bytestream):
  dt = numpy.dtype(numpy.uint32).newbyteorder('>')
  return numpy.frombuffer(bytestream.read(4), dtype=dt)[0]
def extract_images(filename):
  """Extract the images into a 4D uint8 numpy array [index, y, x, depth]."""
  print('Extracting', filename)
  with gzip.open(filename) as bytestream:
    magic = _read32(bytestream)
    if magic != 2051:
      raise ValueError(
          'Invalid magic number %d in MNIST image file: %s' %
          (magic, filename))
    num_images = _read32(bytestream)
    rows = _read32(bytestream)
    cols = _read32(bytestream)
    buf = bytestream.read(rows * cols * num_images)
    data = numpy.frombuffer(buf, dtype=numpy.uint8)
    data = data.reshape(num_images, rows, cols, 1)
    return data
def dense_to_one_hot(labels_dense, num_classes=10):
  """Convert class labels from scalars to one-hot vectors."""
  num_labels = labels_dense.shape[0]
  index_offset = numpy.arange(num_labels) * num_classes
  labels_one_hot = numpy.zeros((num_labels, num_classes))
  labels_one_hot.flat[index_offset + labels_dense.ravel()] = 1
  return labels_one_hot
def extract_labels(filename, one_hot=False):
  """Extract the labels into a 1D uint8 numpy array [index]."""
  print('Extracting', filename)
  with gzip.open(filename) as bytestream:
    magic = _read32(bytestream)
    if magic != 2049:
      raise ValueError(
          'Invalid magic number %d in MNIST label file: %s' %
          (magic, filename))
    num_items = _read32(bytestream)
    buf = bytestream.read(num_items)
    labels = numpy.frombuffer(buf, dtype=numpy.uint8)
    if one_hot:
      return dense_to_one_hot(labels)
    return labels
class DataSet(object):
  def __init__(self, images, labels, fake_data=False, one_hot=False,
               dtype=tf.float32):
    """Construct a DataSet.
    one_hot arg is used only if fake_data is true.  `dtype` can be either
    `uint8` to leave the input as `[0, 255]`, or `float32` to rescale into
    `[0, 1]`.
    """
    dtype = tf.as_dtype(dtype).base_dtype
    if dtype not in (tf.uint8, tf.float32):
      raise TypeError('Invalid image dtype %r, expected uint8 or float32' %
                      dtype)
    if fake_data:
      self._num_examples = 10000
      self.one_hot = one_hot
    else:
      assert images.shape[0] == labels.shape[0], (
          'images.shape: %s labels.shape: %s' % (images.shape,
                                                 labels.shape))
      self._num_examples = images.shape[0]
      # Convert shape from [num examples, rows, columns, depth]
      # to [num examples, rows*columns] (assuming depth == 1)
      assert images.shape[3] == 1
      images = images.reshape(images.shape[0],
                              images.shape[1] * images.shape[2])
      if dtype == tf.float32:
        # Convert from [0, 255] -> [0.0, 1.0].
        images = images.astype(numpy.float32)
        images = numpy.multiply(images, 1.0 / 255.0)
    self._images = images
    self._labels = labels
    self._epochs_completed = 0
    self._index_in_epoch = 0
  @property
  def images(self):
    return self._images
  @property
  def labels(self):
    return self._labels
  @property
  def num_examples(self):
    return self._num_examples
  @property
  def epochs_completed(self):
    return self._epochs_completed
  def next_batch(self, batch_size, fake_data=False):
    """Return the next `batch_size` examples from this data set."""
    if fake_data:
      fake_image = [1] * 784
      if self.one_hot:
        fake_label = [1] + [0] * 9
      else:
        fake_label = 0
      return [fake_image for _ in xrange(batch_size)], [
          fake_label for _ in xrange(batch_size)]
    start = self._index_in_epoch
    self._index_in_epoch += batch_size
    if self._index_in_epoch > self._num_examples:
      # Finished epoch
      self._epochs_completed += 1
      # Shuffle the data
      perm = numpy.arange(self._num_examples)
      numpy.random.shuffle(perm)
      self._images = self._images[perm]
      self._labels = self._labels[perm]
      # Start next epoch
      start = 0
      self._index_in_epoch = batch_size
      assert batch_size <= self._num_examples
    end = self._index_in_epoch
    return self._images[start:end], self._labels[start:end]
def read_data_sets(train_dir, fake_data=False, one_hot=False, dtype=tf.float32):
  class DataSets(object):
    pass
  data_sets = DataSets()
  if fake_data:
    def fake():
      return DataSet([], [], fake_data=True, one_hot=one_hot, dtype=dtype)
    data_sets.train = fake()
    data_sets.validation = fake()
    data_sets.test = fake()
    return data_sets
  TRAIN_IMAGES = 'train-images-idx3-ubyte.gz'
  TRAIN_LABELS = 'train-labels-idx1-ubyte.gz'
  TEST_IMAGES = 't10k-images-idx3-ubyte.gz'
  TEST_LABELS = 't10k-labels-idx1-ubyte.gz'
  VALIDATION_SIZE = 5000
  local_file = maybe_download(TRAIN_IMAGES, train_dir)
  train_images = extract_images(local_file)
  local_file = maybe_download(TRAIN_LABELS, train_dir)
  train_labels = extract_labels(local_file, one_hot=one_hot)
  local_file = maybe_download(TEST_IMAGES, train_dir)
  test_images = extract_images(local_file)
  local_file = maybe_download(TEST_LABELS, train_dir)
  test_labels = extract_labels(local_file, one_hot=one_hot)
  validation_images = train_images[:VALIDATION_SIZE]
  validation_labels = train_labels[:VALIDATION_SIZE]
  train_images = train_images[VALIDATION_SIZE:]
  train_labels = train_labels[VALIDATION_SIZE:]
  data_sets.train = DataSet(train_images, train_labels, dtype=dtype)
  data_sets.validation = DataSet(validation_images, validation_labels,
                                 dtype=dtype)
  data_sets.test = DataSet(test_images, test_labels, dtype=dtype)
  return data_sets
```





# 【Tensorflow slim】slim nets包 - zj360202的专栏 - CSDN博客





2017年11月21日 17:49:12[zj360202](https://me.csdn.net/zj360202)阅读数：3964








slim nets中包含几种常用的net网络



```python
from tensorflow.contrib.slim.python.slim.nets import alexnet
from tensorflow.contrib.slim.python.slim.nets import inception
from tensorflow.contrib.slim.python.slim.nets import overfeat
from tensorflow.contrib.slim.python.slim.nets import resnet_utils
from tensorflow.contrib.slim.python.slim.nets import resnet_v1
from tensorflow.contrib.slim.python.slim.nets import resnet_v2
from tensorflow.contrib.slim.python.slim.nets import vgg
from tensorflow.python.util.all_util import make_all
```


```python
import tensorflow as tf
vgg = tf.contrib.slim.nets.vgg

# Load the images and labels.
images, labels = ...

# Create the model.
predictions, _ = vgg.vgg_16(images)

# Define the loss functions and get the total loss.
loss = slim.losses.softmax_cross_entropy(predictions, labels)
```






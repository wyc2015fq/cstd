# keras系列︱Application中五款已训练模型、VGG16框架（Sequential式、Model式）解读（二） - 素质云笔记/Recorder... - CSDN博客





2017年06月04日 17:19:57[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：31965








> 
不得不说，这深度学习框架更新太快了尤其到了Keras2.0版本，快到Keras中文版好多都是错的，快到官方文档也有旧的没更新，前路坑太多。  

  到发文为止，已经有theano/tensorflow/CNTK支持keras,虽然说tensorflow造势很多，但是笔者认为接下来Keras才是正道。 

  笔者先学的caffe，从使用来看，比caffe简单超级多，非常好用，特别是重新训练一个模型，但是呢，在fine-tuning的时候，遇到了很多问题，对新手比较棘手。
中文文档：[http://keras-cn.readthedocs.io/en/latest/](http://keras-cn.readthedocs.io/en/latest/)

官方文档：[https://keras.io/](https://keras.io/)

文档主要是以keras2.0。 

.
.

### Keras系列：

1、[keras系列︱Sequential与Model模型、keras基本结构功能（一）](http://blog.csdn.net/sinat_26917383/article/details/72857454)

2、[keras系列︱Application中五款已训练模型、VGG16框架（Sequential式、Model式）解读（二）](http://blog.csdn.net/sinat_26917383/article/details/72859145)

3、[keras系列︱图像多分类训练与利用bottleneck features进行微调（三）](http://blog.csdn.net/sinat_26917383/article/details/72861152)

 4、[keras系列︱人脸表情分类与识别：opencv人脸检测+Keras情绪分类（四）](http://blog.csdn.net/sinat_26917383/article/details/72885715)

 5、[keras系列︱迁移学习：利用InceptionV3进行fine-tuning及预测、完整案例（五）](http://blog.csdn.net/sinat_26917383/article/details/72982230)
# 一、Application的五款已训练模型 + H5py简述

Kera的应用模块Application提供了带有预训练权重的Keras模型，这些模型可以用来进行预测、特征提取和finetune。 

后续还有对以下几个模型的参数介绍：
- Xception
- VGG16
- VGG19
- ResNet50
- InceptionV3

所有的这些模型(除了Xception)都兼容Theano和Tensorflow，并会自动基于~/.keras/keras.json的Keras的图像维度进行自动设置。例如，如果你设置data_format=”channel_last”，则加载的模型将按照TensorFlow的维度顺序来构造，即“Width-Height-Depth”的顺序。

模型的官方下载路径：[https://github.com/fchollet/deep-learning-models/releases](https://github.com/fchollet/deep-learning-models/releases)

其中： 

.

## 1、**th**与**tf**的区别

==================

**Keras提供了两套后端，Theano和Tensorflow，**

th和tf的大部分功能都被backend统一包装起来了，但二者还是存在不小的冲突，有时候你需要特别注意Keras是运行在哪种后端之上，它们的主要冲突有：

dim_ordering，也就是维度顺序。比方说一张224*224的彩色图片，theano的维度顺序是(3，224，224)，即通道维在前。而tf的维度顺序是(224，224，3)，即通道维在后。 

卷积层权重的shape：从无到有训练一个网络，不会有任何问题。但是如果你想把一个th训练出来的卷积层权重载入风格为tf的卷积层……说多了都是泪。我一直觉得这个是个bug，数据的dim_ordering有问题就罢了，为啥卷积层权重的shape还需要变换咧？我迟早要提个PR把这个bug修掉！ 

然后是卷积层kernel的翻转不翻转问题，这个我们说过很多次了，就不再多提。 

数据格式的区别，channels_last”对应原本的“tf”，“channels_first”对应原本的“th”。 

以128x128的RGB图像为例，“channels_first”应将数据组织为（3,128,128），而“channels_last”应将数据组织为（128,128,3）。 

譬如： 

vgg16_weights_th_dim_ordering_th_kernels_notop.h5 

vgg16_weights_tf_dim_ordering_tf_kernels_notop.h5 

.
## 2、notop模型是指什么？

==============

是否包含最后的3个全连接层（whether to include the 3 fully-connected layers at the top of the network）。用来做fine-tuning专用，专门开源了这类模型。 

.

## 3、H5py简述

========

keras的已训练模型是H5PY格式的，不是caffe的.caffemodel 

h5py.File类似Python的词典对象，因此我们可以查看所有的键值： 

读入`file=h5py.File('.../notop.h5','r')`
```python
f.attrs['nb_layers'],代表f的属性，其中有一个属性为'nb_layers'
```

```python
>>> f.keys()
[u'block1_conv1', u'block1_conv2', u'block1_pool', u'block2_conv1', u'block2_conv2', u'block2_pool', u'block3_conv1', u'block3_conv2', u'block3_conv3', u'block3_pool', u'block4_conv1', u'block4_conv2', u'block4_conv3', u'block4_pool', u'block5_conv1', u'block5_conv2', u'block5_conv3', u'block5_pool']
```

可以看到f中各个层内有些什么。

```python
for name in f:
    print(name)
    # 类似f.keys()
```

.

## 4、官方案例——利用ResNet50网络进行ImageNet分类

================================

```
rom keras.applications.resnet50 import ResNet50
from keras.preprocessing import image
from keras.applications.resnet50 import preprocess_input, decode_predictions
import numpy as np

model = ResNet50(weights='imagenet')

img_path = 'elephant.jpg'
img = image.load_img(img_path, target_size=(224, 224))
x = image.img_to_array(img)
x = np.expand_dims(x, axis=0)
x = preprocess_input(x)

preds = model.predict(x)
print('Predicted:', decode_predictions(preds, top=3)[0])
# Predicted: [(u'n02504013', u'Indian_elephant', 0.82658225), (u'n01871265', u'tusker', 0.1122357), (u'n02504458', u'African_elephant', 0.061040461)]
```

还有的案例可见[Keras官方文档](http://keras-cn.readthedocs.io/en/latest/other/application/)

> 
利用VGG16提取特征、从VGG19的任意中间层中抽取特征、在定制的输入tensor上构建InceptionV3


.

## 5、调用参数解释

========

**以下几类，因为调用好像都是从网站下载权重，所以可以自己修改一下源码，让其可以读取本地H5文件。**

### Xception模型

ImageNet上,该模型取得了验证集top1 0.790和top5 0.945的正确率; 

,该模型目前仅能以TensorFlow为后端使用,由于它依赖于”SeparableConvolution”层,目前该模型只支持channels_last的维度顺序(width, height, channels)

默认输入图片大小为299x299

```python
keras.applications.xception.Xception(include_top=True, weights='imagenet',
                                    input_tensor=None, input_shape=None,
                                    pooling=None, classes=1000)
```

### VGG16模型

VGG16模型,权重由ImageNet训练而来

该模型再Theano和TensorFlow后端均可使用,并接受channels_first和channels_last两种输入维度顺序

模型的默认输入尺寸时224x224

```python
keras.applications.vgg16.VGG16(include_top=True, weights='imagenet',
                                input_tensor=None, input_shape=None,
                                pooling=None,
                                classes=1000)
```

### VGG19模型

VGG19模型,权重由ImageNet训练而来

该模型在Theano和TensorFlow后端均可使用,并接受channels_first和channels_last两种输入维度顺序

模型的默认输入尺寸时224x224

```python
keras.applications.vgg19.VGG19(include_top=True, weights='imagenet',
                                input_tensor=None, input_shape=None,
                                pooling=None,
                                classes=1000)
```

### ResNet50模型

50层残差网络模型,权重训练自ImageNet

该模型在Theano和TensorFlow后端均可使用,并接受channels_first和channels_last两种输入维度顺序

模型的默认输入尺寸时224x224

```python
keras.applications.resnet50.ResNet50(include_top=True, weights='imagenet',
                                input_tensor=None, input_shape=None,
                                pooling=None,
                                classes=1000)
```

### InceptionV3模型

InceptionV3网络,权重训练自ImageNet

该模型在Theano和TensorFlow后端均可使用,并接受channels_first和channels_last两种输入维度顺序

模型的默认输入尺寸时299x299

```python
keras.applications.inception_v3.InceptionV3(include_top=True,
                                            weights='imagenet',
                                            input_tensor=None,
                                            input_shape=None,
                                            pooling=None,
                                            classes=1000)
```

.

# 二、 keras-applications-VGG16解读——函数式

.py文件来源于：[https://github.com/fchollet/deep-learning-models/blob/master/vgg16.py](https://github.com/fchollet/deep-learning-models/blob/master/vgg16.py)

VGG16默认的输入数据格式应该是：channels_last

```python
# -*- coding: utf-8 -*-
'''VGG16 model for Keras.
# Reference:
- [Very Deep Convolutional Networks for Large-Scale Image Recognition](https://arxiv.org/abs/1409.1556)
'''
from __future__ import print_function

import numpy as np
import warnings

from keras.models import Model
from keras.layers import Flatten
from keras.layers import Dense
from keras.layers import Input
from keras.layers import Conv2D
from keras.layers import MaxPooling2D
from keras.layers import GlobalMaxPooling2D
from keras.layers import GlobalAveragePooling2D
from keras.preprocessing import image
from keras.utils import layer_utils
from keras.utils.data_utils import get_file
from keras import backend as K
from keras.applications.imagenet_utils import decode_predictions
# decode_predictions 输出5个最高概率：(类名, 语义概念, 预测概率) decode_predictions(y_pred)
from keras.applications.imagenet_utils import preprocess_input
#  预处理 图像编码服从规定，譬如,RGB，GBR这一类的，preprocess_input(x)  
from keras.applications.imagenet_utils import _obtain_input_shape
# 确定适当的输入形状，相当于opencv中的read.img，将图像变为数组
from keras.engine.topology import get_source_inputs

WEIGHTS_PATH = 'https://github.com/fchollet/deep-learning-models/releases/download/v0.1/vgg16_weights_tf_dim_ordering_tf_kernels.h5'
WEIGHTS_PATH_NO_TOP = 'https://github.com/fchollet/deep-learning-models/releases/download/v0.1/vgg16_weights_tf_dim_ordering_tf_kernels_notop.h5'

def VGG16(include_top=True, weights='imagenet',
          input_tensor=None, input_shape=None,
          pooling=None,
          classes=1000):
    # 检查weight与分类设置是否正确
    if weights not in {'imagenet', None}:
        raise ValueError('The `weights` argument should be either '
                         '`None` (random initialization) or `imagenet` '
                         '(pre-training on ImageNet).')

    if weights == 'imagenet' and include_top and classes != 1000:
        raise ValueError('If using `weights` as imagenet with `include_top`'
                         ' as true, `classes` should be 1000')

    # 设置图像尺寸，类似caffe中的transform
    # Determine proper input shape
    input_shape = _obtain_input_shape(input_shape,
                                      default_size=224,
                                      min_size=48,
                                      # 模型所能接受的最小长宽
                                      data_format=K.image_data_format(),
                                      # 数据的使用格式
                                      include_top=include_top)
                                      #是否通过一个Flatten层再连接到分类器

    # 数据简单处理，resize
    if input_tensor is None:
        img_input = Input(shape=input_shape)
        # 这里的Input是keras的格式，可以用于转换
    else:
        if not K.is_keras_tensor(input_tensor):
            img_input = Input(tensor=input_tensor, shape=input_shape)
        else:
            img_input = input_tensor
        # 如果是tensor的数据格式，需要两步走：
        # 先判断是否是keras指定的数据类型，is_keras_tensor
        # 然后get_source_inputs(input_tensor)

    # 编写网络结构，prototxt
    # Block 1
    x = Conv2D(64, (3, 3), activation='relu', padding='same', name='block1_conv1')(img_input)
    x = Conv2D(64, (3, 3), activation='relu', padding='same', name='block1_conv2')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2), name='block1_pool')(x)

    # Block 2
    x = Conv2D(128, (3, 3), activation='relu', padding='same', name='block2_conv1')(x)
    x = Conv2D(128, (3, 3), activation='relu', padding='same', name='block2_conv2')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2), name='block2_pool')(x)

    # Block 3
    x = Conv2D(256, (3, 3), activation='relu', padding='same', name='block3_conv1')(x)
    x = Conv2D(256, (3, 3), activation='relu', padding='same', name='block3_conv2')(x)
    x = Conv2D(256, (3, 3), activation='relu', padding='same', name='block3_conv3')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2), name='block3_pool')(x)

    # Block 4
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block4_conv1')(x)
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block4_conv2')(x)
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block4_conv3')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2), name='block4_pool')(x)

    # Block 5
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block5_conv1')(x)
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block5_conv2')(x)
    x = Conv2D(512, (3, 3), activation='relu', padding='same', name='block5_conv3')(x)
    x = MaxPooling2D((2, 2), strides=(2, 2), name='block5_pool')(x)

    if include_top:
        # Classification block
        x = Flatten(name='flatten')(x)
        x = Dense(4096, activation='relu', name='fc1')(x)
        x = Dense(4096, activation='relu', name='fc2')(x)
        x = Dense(classes, activation='softmax', name='predictions')(x)
    else:
        if pooling == 'avg':
            x = GlobalAveragePooling2D()(x)
        elif pooling == 'max':
            x = GlobalMaxPooling2D()(x)

    # 调整数据      
    # Ensure that the model takes into account
    # any potential predecessors of `input_tensor`.
    if input_tensor is not None:
        inputs = get_source_inputs(input_tensor)
        # get_source_inputs 返回计算需要的数据列表，List of input tensors.
        # 如果是tensor的数据格式，需要两步走：
        # 先判断是否是keras指定的数据类型，is_keras_tensor
        # 然后get_source_inputs(input_tensor)
    else:
        inputs = img_input

    # 创建模型
    # Create model.
    model = Model(inputs, x, name='vgg16')

    # 加载权重
    # load weights
    if weights == 'imagenet':
        if include_top:
            weights_path = get_file('vgg16_weights_tf_dim_ordering_tf_kernels.h5',
                                    WEIGHTS_PATH,
                                    cache_subdir='models')
        else:
            weights_path = get_file('vgg16_weights_tf_dim_ordering_tf_kernels_notop.h5',
                                    WEIGHTS_PATH_NO_TOP,
                                    cache_subdir='models')
        model.load_weights(weights_path)

        if K.backend() == 'theano':
            layer_utils.convert_all_kernels_in_model(model)

        if K.image_data_format() == 'channels_first':
            if include_top:
                maxpool = model.get_layer(name='block5_pool')
                shape = maxpool.output_shape[1:]
                dense = model.get_layer(name='fc1')
                layer_utils.convert_dense_weights_data_format(dense, shape, 'channels_first')

            if K.backend() == 'tensorflow':
                warnings.warn('You are using the TensorFlow backend, yet you '
                              'are using the Theano '
                              'image data format convention '
                              '(`image_data_format="channels_first"`). '
                              'For best performance, set '
                              '`image_data_format="channels_last"` in '
                              'your Keras config '
                              'at ~/.keras/keras.json.')
    return model

if __name__ == '__main__':
    model = VGG16(include_top=True, weights='imagenet')

    img_path = 'elephant.jpg'
    img = image.load_img(img_path, target_size=(224, 224))
    x = image.img_to_array(img)
    x = np.expand_dims(x, axis=0)
    x = preprocess_input(x)
    print('Input image shape:', x.shape)

    preds = model.predict(x)
    print('Predicted:', decode_predictions(preds))
    # decode_predictions 输出5个最高概率：(类名, 语义概念, 预测概率)
```

其中： 

.

## 1、如何已经把模型下载到本地

==============

模型已经下载，不再每次从网站进行加载，可以修改以下内容。

```bash
WEIGHTS_PATH = 'https://github.com/fchollet/deep-learning-models/releases/download/v0.1/vgg16_weights_tf_dim_ordering_tf_kernels.h5'
WEIGHTS_PATH_NO_TOP = 'https://github.com/fchollet/deep-learning-models/releases/download/v0.1/vgg16_weights_tf_dim_ordering_tf_kernels_notop.h5'

  weights_path = get_file('vgg16_weights_tf_dim_ordering_tf_kernels.h5',
                          WEIGHTS_PATH,
                          cache_subdir='models')
            weights_path = get_file('vgg16_weights_tf_dim_ordering_tf_kernels_notop.h5',
                                    WEIGHTS_PATH_NO_TOP,
                                    cache_subdir='models')
```

.

## 2、几个layer中的新用法

==============

```
from keras.applications.imagenet_utils import decode_predictions
decode_predictions 输出5个最高概率：(类名, 语义概念, 预测概率) decode_predictions(y_pred)

from keras.applications.imagenet_utils import preprocess_input
预处理 图像编码服从规定，譬如,RGB，GBR这一类的，preprocess_input(x)  

from keras.applications.imagenet_utils import _obtain_input_shape 
确定适当的输入形状，相当于opencv中的read.img，将图像变为数组
```

（1）decode_predictions用在最后输出结果上，比较好用【print(‘Predicted:’, decode_predictions(preds))】； 

（2）preprocess_input，改变编码，【preprocess_input(x)】； 

（3）_obtain_input_shape 

 相当于caffe中的transform，在预测的时候，需要对预测的图片进行一定的预处理。
```
input_shape = _obtain_input_shape(input_shape,
                                      default_size=224,
                                      min_size=48,
                                      # 模型所能接受的最小长宽
                                      data_format=K.image_data_format(),
                                      # 数据的使用格式
                                      include_top=include_top)
```

.

## 3、当include_top=True时

====================

```
fc_model = VGG16(include_top=True)
notop_model = VGG16(include_top=False)
```

之前提到过用VGG16做fine-tuning的时候，得到的notop_model就是没有全连接层的模型。然后再去添加自己的层。 

当时健全的网络结构的时候，fc_model需要添加以下的内容以补全网络结构：

```
x = Flatten(name='flatten')(x)
x = Dense(4096, activation='relu', name='fc1')(x)
x = Dense(4096, activation='relu', name='fc2')(x)
x = Dense(classes, activation='softmax', name='predictions')(x)
```

pool层之后接一个flatten层，修改数据格式，然后接两个dense层，最后有softmax的Dense层。 

.

## 4、如果输入的数据格式是channels_first？

===========================

如果input的格式是’channels_first’，fc_model还需要修改一下格式，因为VGG16源码是以’channels_last’定义的，所以需要转换一下输出格式。

```bash
maxpool = model.get_layer(name='block5_pool')
 # model.get_layer()依据层名或下标获得层对象
 shape = maxpool.output_shape[1:]
 # 获取block5_pool层输出的数据格式
 dense = model.get_layer(name='fc1')
 layer_utils.convert_dense_weights_data_format(dense, shape, 'channels_first')
```

其中layer_utils.convert_dense_weights_data_format的作用很特殊，官方文档中没有说明，本质用来修改数据格式，因为层中有Flatter层把数据格式换了，所以需要再修改一下。 

原文：

> 
When porting the weights of a convnet from one data format to the other,if the convnet includes a `Flatten` layer (applied to the last convolutional feature map) followed by a `Dense` layer, the weights of that `Dense` layer should be updated to reflect the new dimension ordering.


.

# 三、keras-Sequential-VGG16源码解读：序列式

本节节选自Keras中文文档[《CNN眼中的世界：利用Keras解释CNN的滤波器》](http://keras-cn.readthedocs.io/en/latest/blog/cnn_see_world/)

已训练好VGG16和VGG19模型的权重： 

国外：[https://gist.github.com/baraldilorenzo/07d7802847aaad0a35d3](https://gist.github.com/baraldilorenzo/07d7802847aaad0a35d3)

国内：[http://files.heuritech.com/weights/vgg16_weights.h5](http://files.heuritech.com/weights/vgg16_weights.h5)
前面是VGG16架构的函数式模型的结构，那么在官方文档这个案例中，也有VGG16架构的序列式，都拿来比对一下比较好。 

.

## 1、VGG16的Sequential-网络结构

首先，我们在Keras中定义VGG网络的结构：

```python
from keras.models import Sequential
from keras.layers import Convolution2D, ZeroPadding2D, MaxPooling2D

img_width, img_height = 128, 128

# build the VGG16 network
model = Sequential()
model.add(ZeroPadding2D((1, 1), batch_input_shape=(1, 3, img_width, img_height)))
first_layer = model.layers[-1]
# this is a placeholder tensor that will contain our generated images
input_img = first_layer.input

# build the rest of the network
model.add(Convolution2D(64, 3, 3, activation='relu', name='conv1_1'))
model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(64, 3, 3, activation='relu', name='conv1_2'))
model.add(MaxPooling2D((2, 2), strides=(2, 2)))

model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(128, 3, 3, activation='relu', name='conv2_1'))
model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(128, 3, 3, activation='relu', name='conv2_2'))
model.add(MaxPooling2D((2, 2), strides=(2, 2)))

model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(256, 3, 3, activation='relu', name='conv3_1'))
model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(256, 3, 3, activation='relu', name='conv3_2'))
model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(256, 3, 3, activation='relu', name='conv3_3'))
model.add(MaxPooling2D((2, 2), strides=(2, 2)))

model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(512, 3, 3, activation='relu', name='conv4_1'))
model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(512, 3, 3, activation='relu', name='conv4_2'))
model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(512, 3, 3, activation='relu', name='conv4_3'))
model.add(MaxPooling2D((2, 2), strides=(2, 2)))

model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(512, 3, 3, activation='relu', name='conv5_1'))
model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(512, 3, 3, activation='relu', name='conv5_2'))
model.add(ZeroPadding2D((1, 1)))
model.add(Convolution2D(512, 3, 3, activation='relu', name='conv5_3'))
model.add(MaxPooling2D((2, 2), strides=(2, 2)))

# get the symbolic outputs of each "key" layer (we gave them unique names).
layer_dict = dict([(layer.name, layer) for layer in model.layers])
```

从使用Convolution2D来看，是比较早的版本写的。 

.

## 2、Sequential模型如何部分layer载入权重

下面，我们将预训练好的权重载入模型，一般而言我们可以通过model.load_weights()载入，但这种办法是载入全部的权重，并不适用。 

之前所看到的No_top模型就是用来应付此时的。 

这里我们只载入一部分参数，用的是set_weights()函数，所以我们需要手工载入：
```python
import h5py

weights_path = '.../vgg16_weights.h5'

f = h5py.File(weights_path)
for k in range(f.attrs['nb_layers']):
    if k >= len(model.layers):
        break
    g = f['layer_{}'.format(k)]
    weights = [g['param_{}'.format(p)] for p in range(g.attrs['nb_params'])]
    model.layers[k].set_weights(weights)
f.close()
print('Model loaded.')
```

笔者在实践过程中，并没有实践出来，因为我载入的.h5，不知道为什么没有属性f.attrs[‘nb_layers’]也没有属性g.attrs[‘nb_params’]) 

在寻找答案的过程中，看到有前人也跟我一样的问题，可见（[[keras]猫狗大战的总结](http://blog.csdn.net/smuevian/article/details/60333974)）：
- Q1.f.attrs[‘nb_layers’]是什么意思？我看h5py中没有’nb_layers’的属性啊？attrs是指向f中的属性，点击右键可以看见这个属性（在HDF5-viewer）
- Q2.g= f[‘layer_{}’.format(k)]的含义,.format的作用 

format是格式化的意思，输出g就是format（k）填充到{}上
- Q3.weights = [g[‘param_{}’.format(p)] for p 

inrange(g.attrs[‘nb_params’])]的含义 得到的是layer下param_0、param_1等 

这里用到的是set_weights(weights),weights设置的大小应与该层网络大小一致，否则会报错。













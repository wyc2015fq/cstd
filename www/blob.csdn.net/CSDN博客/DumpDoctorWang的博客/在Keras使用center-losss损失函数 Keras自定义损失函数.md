# 在Keras使用center-losss损失函数\Keras自定义损失函数 - DumpDoctorWang的博客 - CSDN博客





2018年11月19日 10:21:20[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：580











### 目录
- [1、站在巨人的肩膀上](#1_2)
- [2、Keras的损失函数](#2Keras_7)
- [3、在Keras实现center-loss损失函数](#3Kerascenterloss_29)
- [3.1、导入库和定义常量](#31_34)
- [3.2、实现多元分类softmax损失函数](#32softmax_60)
- [3.3、实现center-loss损失函数](#33centerloss_76)
- [3.4、把softmax-loss和center-loss相加](#34softmaxlosscenterloss_124)
- [4、在模型中使用新的损失函数](#4_172)
- [5、完整代码](#5_231)




# 1、站在巨人的肩膀上
- [TensorFlow实现center loss](https://blog.csdn.net/encodets/article/details/54648015)。
- [损失函数改进之Center Loss](https://blog.csdn.net/u014380165/article/details/76946339)。

# 2、Keras的损失函数

想必大家都知道编译（model.compile）模型时，需要指定loss这个参数，也就是损失函数。实际上，Keras已经实现了很多损失函数，比如：

```python
# 多元分类（分好多个类）损失函数
def categorical_crossentropy(y_true, y_pred):
    return K.categorical_crossentropy(y_true, y_pred)
# 稀疏多元分类损失函数
def sparse_categorical_crossentropy(y_true, y_pred):
    return K.sparse_categorical_crossentropy(y_true, y_pred)
# 二元分类损失函数
def binary_crossentropy(y_true, y_pred):
    return K.mean(K.binary_crossentropy(y_true, y_pred), axis=-1)
```

观察上面的损失函数，参数都是y_true和y_pred。y_true是使用one_hot编码之后的标签，shape为(batch_size，NUM_CLASSES)；y_pred是模型最后一层（一般为softmax层）的输出，shape为(batch_size，NUM_CLASSES)；损失函数返回的shape为(batch_size，)。其中NUM_CLASSES是在多元分类问题中类别的数量。

因此，想要自定义损失函数，也非常简单，只要定义一个函数类似于：

```python
def my_loss(y_true, y_pred):
    m_loss=...
    return m_loss
```

# 3、在Keras实现center-loss损失函数

三步走！第一步是去掉原模型最后的一层softmax层，直接获取最后一层fc层的输出，因为center-loss需要获取fc层的输出作为输入。第二步是实现多元分类softmax损失函数。第三步就是实现center-loss损失函数。还有附加的一步，就是把第二部的softmax损失加上第三步的center-loss损失。

因为我的keras是使用tensorflow作为后端，所以可以使用tensorflow的代码来实现损失函数。**另外，为了实现代码的移植性，推荐使用keras.backend里面的方法来实现损失函数。** 第3部分的代码在cl.py中实现。

## 3.1、导入库和定义常量

去掉模型的softmax层很简单，就不多说了。接下来是导入库和定义常量：

```python
# coding=utf-8
# cl.py
# 兼容python3
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from tensorflow import keras
from tensorflow.python.keras import backend as K
import tensorflow as tf
import numpy as np
import random

# 分类问题的类数，fc层的输出单元个数
NUM_CLASSES = 240
# 更新中心的学习率
ALPHA = 0.6
# center-loss的系数
LAMBDA = 0.0005
```

ALPHA是更新中心的学习率，一般设置为0.6或者0.5。LAMBDA是文中center-loss项的系数，这个参数需要仔细的调教，才能取得很好的效果。

## 3.2、实现多元分类softmax损失函数

这里说的多元分类softmax损失函数就是categorical_crossentropy（多类别分类的损失函数，标签需要使用one_hot编码）。非常简单，如下：

```python
def softmax_loss(labels, features):
    """
    计算softmax-loss
    :param labels: 等同于y_true，使用了one_hot编码，shape应为(batch_size, NUM_CLASSES)
    :param features: 等同于y_pred，模型的最后一个fc层(不是softmax层)的输出，shape应为(batch_size, NUM_CLASSES)
    :return: 多云分类的softmax-loss损失，shape为(batch_size, )
    """
    return K.categorical_crossentropy(labels, K.softmax(features, axis=-1))
```

这里为什么要重新实现而不直接使用keras.loss.categorical_crossentropy呢？因为去掉了模型的softmax层，这里要对模型最后一层的输出，也就是features，调用softmax处理一遍，再作为K.categorical_crossentropy的参数。

## 3.3、实现center-loss损失函数

我在这里借鉴了另外一位大佬为tensorflow实现的center-loss代码，因为我的keras使用tensorflow作为后端，所以按道理是可以直接用的。先给出计算center-loss的代码：

```python
def center_loss(labels, features, alpha=_g.ALPHA, num_classes=_g.NUM_CLASSES):
    """
    获取center loss及更新样本的center
    :param labels: Tensor,表征样本label,非one-hot编码,shape应为(batch_size,).
    :param features: Tensor,表征样本特征,最后一个fc层的输出,shape应该为(batch_size, num_classes).
    :param alpha: 0-1之间的数字,控制样本类别中心的学习率,细节参考原文.
    :param num_classes: 整数,表明总共有多少个类别,网络分类输出有多少个神经元这里就取多少.
    :return: Tensor, center-loss， shape因为(batch_size,)
    """
    # 获取特征的维数，例如256维
    len_features = features.get_shape()[1]
    # 建立一个Variable,shape为[num_classes, len_features]，用于存储整个网络的样本中心，
    # 设置trainable=False是因为样本中心不是由梯度进行更新的
    centers = tf.get_variable('centers', [num_classes, len_features], dtype=tf.float32,
                              initializer=tf.constant_initializer(0), trainable=False)
    # 将label展开为一维的，如果labels已经是一维的，则该动作其实无必要
    labels = tf.reshape(labels, [-1])

    # 根据样本label,获取mini-batch中每一个样本对应的中心值
    centers_batch = tf.gather(centers, labels)

    # 当前mini-batch的特征值与它们对应的中心值之间的差
    diff = centers_batch - features

    # 获取mini-batch中同一类别样本出现的次数,了解原理请参考原文公式(4)
    unique_label, unique_idx, unique_count = tf.unique_with_counts(labels)
    appear_times = tf.gather(unique_count, unique_idx)
    appear_times = tf.reshape(appear_times, [-1, 1])

    diff = diff / tf.cast((1 + appear_times), tf.float32)
    diff = alpha * diff

    # 更新centers
    centers_update_op = tf.scatter_sub(centers, labels, diff)

    # 这里使用tf.control_dependencies更新centers
    with tf.control_dependencies([centers_update_op]):
        # 计算center-loss
        c_loss = tf.nn.l2_loss(features - centers_batch)

    return c_loss
```

这里说一下我当时遇到的难题，因为原代码中，是返回c_loss、centers和centers_update_op，然后使用sess.run来更新centers，但损失函数只能返回损失项c_loss，并不能返回centers_update_op，也就不能更新centers。最后想到的办法是使用tf.control_dependencies强行在计算c_loss之前更新centers，这样，centers就会更新了。

## 3.4、把softmax-loss和center-loss相加

```python
def loss(labels, features):
    """
    使用这个函数来作为损失函数，计算softmax-loss加上一定比例的center-loss
    :param labels: Tensor，等同于y_true，使用了one_hot编码，shape应为(batch_size, NUM_CLASSES)
    :param features: Tensor， 等同于y_pred, 模型的最后一个fc层(不是softmax层)的输出，shape应为(batch_size, NUM_CLASSES)
    :return: softmax-loss加上一定比例的center-loss
    """
    labels = K.cast(labels, dtype=tf.float32)
    # 计算softmax-loss
    sf_loss = softmax_loss(labels, features)
    # 计算center-loss，因为labels使用了one_hot来编码，所以这里要使用argmax还原到原来的标签
    c_loss = center_loss(K.argmax(labels, axis=-1), features)
    return sf_loss + LAMBDA * c_loss
```

LAMBDA这个是一个需要仔细调教的参数，对于我的模型，不同的大小对模型训练的效果影响比较大。测试新的softmax-loss with center-loss：

```python
if __name__ == '__main__':
    # 测试label和测试用的features
    test_features = np.random.randn(32, NUM_CLASSES).astype(dtype=np.float32)
    test_labels = np.array(random.sample(range(0, NUM_CLASSES - 1), 32))
    test_labels[0] = 0
    # one_hot编码
    test_labels = keras.utils.to_categorical(test_labels, NUM_CLASSES)

    print(test_features.shape, test_labels.shape)

    # 新建tensor
    test_features = tf.constant(test_features)
    test_labels = tf.constant(test_labels)
    # 得到计算损失的op
    loss_op = loss(test_labels, test_features)
    with tf.Session() as sess:
        # 初始化变量
        sess.run(tf.global_variables_initializer())
        # 计算损失
        result = sess.run(loss_op)
        print(result.shape)
        print(result)
        # 把centers取出来，看看有没有更新
        centers = sess.graph.get_tensor_by_name('centers:0')
        print(centers.eval().shape)
        print(centers.eval())
```

# 4、在模型中使用新的损失函数

未使用新的损失函数前，模型的最后一层是softmax层，编译模型和训练模型的部分代码如下：

```python
# 创建训练集，train_dataset是tf.data.Dataset实例，train_steps是训练一遍需要的步数
    train_dataset, train_steps = ...
    # 创建验证集
    val_dataset, val_steps = ...
    # 获取模型
    model = ...
    # 编译模型
    model.compile(optimizer=keras.optimizers.Adam(lr=0.0001, decay=1e-5),
                  loss=keras.losses.categorical_crossentropy, # 损失函数
                  metrics=[keras.metrics.categorical_accuracy]) # 指标
    # 训练模型
    model.fit(train_dataset, epochs=10, steps_per_epoch=train_steps,
              validation_data=val_dataset, validation_steps=val_steps)
```

使用center-loss之后，编译和训练模型的代码如下：

```python
import cl
    
    # 创建训练集，train_dataset是tf.data.Dataset实例，train_steps是训练一遍需要的步数
    train_dataset, train_steps = ...
    # 创建验证集
    val_dataset, val_steps = ...
    # 获取模型
    model = ...
    # 编译模型
    model.compile(optimizer=keras.optimizers.Adam(lr=0.0001, decay=1e-5),
                  loss=cl.loss, # 损失函数
                  metrics=[cl.categorical_accuracy]) # 指标
    # 在这里初始化变量
    sess = K.get_session()
    sess.run(tf.global_variables_initializer())
    # 训练模型
    model.fit(train_dataset, epochs=10, steps_per_epoch=train_steps,
              validation_data=val_dataset, validation_steps=val_steps)
```

特别需要注意的是，**一定要在编译（compile）模型之后，训练（fit）或者评估（evaluate）之前，使用tf.global_variables_initializer初始化center-loss函数中定义的centers**，否则会报错说试图使用未初始化的变量。

观察上面的代码，指标后面的函数cl.categorical_accuracy也是重新实现在cl.py中的：

```python
def categorical_accuracy(y_true, y_pred):
    """
    重写categorical_accuracy函数，以适应去掉softmax层的模型
    :param y_true: 等同于labels，
    :param y_pred: 等同于features。
    :return: 准确率
    """
    # 计算y_pred的softmax值
    sm_y_pred = K.softmax(y_pred, axis=-1)
    # 返回准确率
    return K.cast(K.equal(K.argmax(y_true, axis=-1), K.argmax(sm_y_pred, axis=-1)), K.floatx())
```

和原来keras.metrics.categorical_accuracy相比，因为去掉了模型最后的softmax层，所以需要softmax函数处理一下y_pred。

# 5、完整代码

为方便大家，贴上完整代码：

```python
# coding=utf-8
# 兼容python3
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

from tensorflow import keras
from tensorflow.python.keras import backend as K
import tensorflow as tf
import numpy as np
import random

# 分类问题的类数，fc层的输出单元个数
NUM_CLASSES = 240
# 更新中心的学习率
ALPHA = 0.6
# center-loss的系数
LAMBDA = 0.0005


def center_loss(labels, features, alpha=ALPHA, num_classes=NUM_CLASSES):
    """
    获取center loss及更新样本的center
    :param labels: Tensor,表征样本label,非one-hot编码,shape应为(batch_size,).
    :param features: Tensor,表征样本特征,最后一个fc层的输出,shape应该为(batch_size, num_classes).
    :param alpha: 0-1之间的数字,控制样本类别中心的学习率,细节参考原文.
    :param num_classes: 整数,表明总共有多少个类别,网络分类输出有多少个神经元这里就取多少.
    :return: Tensor, center-loss， shape因为(batch_size,)
    """
    # 获取特征的维数，例如256维
    len_features = features.get_shape()[1]
    # 建立一个Variable,shape为[num_classes, len_features]，用于存储整个网络的样本中心，
    # 设置trainable=False是因为样本中心不是由梯度进行更新的
    centers = tf.get_variable('centers', [num_classes, len_features], dtype=tf.float32,
                              initializer=tf.constant_initializer(0), trainable=False)
    # 将label展开为一维的，如果labels已经是一维的，则该动作其实无必要
    labels = tf.reshape(labels, [-1])

    # 根据样本label,获取mini-batch中每一个样本对应的中心值
    centers_batch = tf.gather(centers, labels)

    # 当前mini-batch的特征值与它们对应的中心值之间的差
    diff = centers_batch - features

    # 获取mini-batch中同一类别样本出现的次数,了解原理请参考原文公式(4)
    unique_label, unique_idx, unique_count = tf.unique_with_counts(labels)
    appear_times = tf.gather(unique_count, unique_idx)
    appear_times = tf.reshape(appear_times, [-1, 1])

    diff = diff / tf.cast((1 + appear_times), tf.float32)
    diff = alpha * diff

    # 更新centers
    centers_update_op = tf.scatter_sub(centers, labels, diff)

    # 这里使用tf.control_dependencies更新centers
    with tf.control_dependencies([centers_update_op]):
        # 计算center-loss
        c_loss = tf.nn.l2_loss(features - centers_batch)

    return c_loss


def softmax_loss(labels, features):
    """
    计算softmax-loss
    :param labels: 等同于y_true，使用了one_hot编码，shape应为(batch_size, NUM_CLASSES)
    :param features: 等同于y_pred，模型的最后一个FC层(不是softmax层)的输出，shape应为(batch_size, NUM_CLASSES)
    :return: 多云分类的softmax-loss损失，shape为(batch_size, )
    """
    return K.categorical_crossentropy(labels, K.softmax(features, axis=-1))


def loss(labels, features):
    """
    使用这个函数来作为损失函数，计算softmax-loss加上一定比例的center-loss
    :param labels: Tensor，等同于y_true，使用了one_hot编码，shape应为(batch_size, NUM_CLASSES)
    :param features: Tensor， 等同于y_pred, 模型的最后一个fc层(不是softmax层)的输出，shape应为(batch_size, NUM_CLASSES)
    :return: softmax-loss加上一定比例的center-loss
    """
    labels = K.cast(labels, dtype=tf.float32)
    # 计算softmax-loss
    sf_loss = softmax_loss(labels, features)
    # 计算center-loss，因为labels使用了one_hot来编码，所以这里要使用argmax还原到原来的标签
    c_loss = center_loss(K.argmax(labels, axis=-1), features)
    return sf_loss + LAMBDA * c_loss


def categorical_accuracy(y_true, y_pred):
    """
    重写categorical_accuracy函数，以适应去掉softmax层的模型
    :param y_true: 等同于labels，
    :param y_pred: 等同于features。
    :return: 准确率
    """
    # 计算y_pred的softmax值
    sm_y_pred = K.softmax(y_pred, axis=-1)
    # 返回准确率
    return K.cast(K.equal(K.argmax(y_true, axis=-1), K.argmax(sm_y_pred, axis=-1)), K.floatx())


if __name__ == '__main__':
    # 测试label和测试用的features
    test_features = np.random.randn(32, NUM_CLASSES).astype(dtype=np.float32)
    test_labels = np.array(random.sample(range(0, NUM_CLASSES - 1), 32))
    test_labels[0] = 0
    # one_hot编码
    test_labels = keras.utils.to_categorical(test_labels, NUM_CLASSES)

    print(test_features.shape, test_labels.shape)

    # 新建tensor
    test_features = tf.constant(test_features)
    test_labels = tf.constant(test_labels)
    # 得到计算损失的op
    loss_op = loss(test_labels, test_features)
    with tf.Session() as sess:
        # 初始化变量
        sess.run(tf.global_variables_initializer())
        # 计算损失
        result = sess.run(loss_op)
        print(result.shape)
        print(result)
        # 把centers取出来，看看有没有更新
        updated_centers = sess.graph.get_tensor_by_name('centers:0')
        print(updated_centers.eval().shape)
        print(updated_centers.eval())
```




# TensorFlow 高级之二 （卷积神经网络手写字体识别） - wsp_1138886114的博客 - CSDN博客





2018年09月22日 17:45:51[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：569











### 文章目录
- [一、数据集获取](#_1)
- [二、数据感知与处理](#_22)
- [2.1 导包](#21__23)
- [2.2 导入数据](#22__38)
- [2.3 把标签转换为one-hot格式](#23_onehot_69)
- [2.4.  数据维度](#24___90)
- [2.5. 打印部分样例图片](#25__98)
- [三、创建神经网络](#_129)
- [3.1 定义网络参数—初始化（权重-偏值项）](#31__130)
- [3.2 定义卷积层](#32__148)
- [3.3 定义展平层](#33__191)
- [3.4 定义全连接层](#34__207)
- [3.5 定义占位符](#35__225)
- [3.6 连接神经网络（卷积层--展平层--连接层--预测类别--代价函数--优化方法--模型性能度量）](#36__238)
- [3.7 定义session 并执行](#37_session__282)




### 一、数据集获取

前言
- 在梯度下降和最优化部分用传统的神经网络在MNIST数据集上得到了90%左右的准确率。结果其实并不太理想。
- 接下来，我们将使用卷积神经网络来得到一个准确率更高的模型，接近99%。卷积神经网络使用共享的卷积核对图像进行卷积操作，以提取图像深层特征。

这些深层特征然后组合成特征向量输入全连接的神经网络中，再使用类似传统神经网络的方法进行分类。

数据集概述
- 这里以MNIST为数据集，MNIST是一个手写数字0-9的数据集，它有60000个训练样本集和10000个测试样本集它是NIST数据库的一个子集。
- MNIST数据库官方网址为：[http://yann.lecun.com/exdb/mnist/](http://yann.lecun.com/exdb/mnist/)

或者博客下载地址：[https://download.csdn.net/download/wsp_1138886114/10602153](https://download.csdn.net/download/wsp_1138886114/10602153)
- 在windows下直接下载:

train-images-idx3-ubyte.gz / train-labels-idx1-ubyte.gz

t10k-labels-idx1-ubyte.gz / t10k-images-idx3-ubyte.gz

四个文件，图像数据都保存在二进制文件中。每个样本图像的宽高为28*28。
#####1.1 网络架构图
![这里写图片描述](https://img-blog.csdn.net/2018061407202559?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 二、数据感知与处理

##### 2.1 导包

```python
```python
import os
import struct
import tensorflow as tf          
import matplotlib.pyplot as plt  
import numpy as np             
from sklearn.metrics import confusion_matrix    # 混淆矩阵，分析模型误差

import time                                     # 计时
from datetime import timedelta
import math

%matplotlib inline                              #使用notebook在线画图
```
```

##### 2.2 导入数据

```python
```python
def load_mnist(path, kind='train'):            #读取数据函数
    #Load MNIST data from path
    labels_path = os.path.join(path, '%s-labels.idx1-ubyte' % kind)
    images_path = os.path.join(path, '%s-images.idx3-ubyte' % kind)

    with open(labels_path, 'rb') as lbpath:
        magic, n = struct.unpack('>II',lbpath.read(8))
        labels = np.fromfile(lbpath, dtype=np.uint8)

    with open(images_path, 'rb') as imgpath:
        magic, num, rows, cols = struct.unpack(">IIII",imgpath.read(16))
        images = np.fromfile(imgpath, dtype=np.uint8).reshape(len(labels), 784)

    return images, labels

#导入数据
X_train, y_train = load_mnist('./data/mnist', kind='train')
print('Rows: %d, columns: %d' % (X_train.shape[0], X_train.shape[1]))

X_test, y_test = load_mnist('./data/mnist', kind='t10k')
print('Rows: %d, columns: %d' % (X_test.shape[0], X_test.shape[1]))
print(y_train[:5],y_test[:5]) 

输出： -------------------------------------------------
	Rows: 60000, columns: 784
	Rows: 10000, columns: 784
	[5 0 4 1 9] [7 2 1 0 4]
```
```

##### 2.3 把标签转换为one-hot格式

```python
```python
def dense_to_one_hot(labels_dense, num_classes=10):
  """把类标签转换 one-hot向量."""
  num_labels = labels_dense.shape[0]
  index_offset = np.arange(num_labels) * num_classes
  labels_one_hot = np.zeros((num_labels, num_classes))
  labels_one_hot.flat[index_offset + labels_dense.ravel()] = 1
  return labels_one_hot

y_train01=dense_to_one_hot(y_train, num_classes=10)
y_test01=dense_to_one_hot(y_test, num_classes=10)

print(y_train01[:5])
print(y_test01[:5])

test=np.argmax(y_test01, axis=1)
print(test[:5])
print("样本维度：", X_train.shape)
print("标签维度：", y_train01.shape)
```
```

##### 2.4.  数据维度

```python
```python
img_size = 28                          # 图片的高度和宽度
img_size_flat = img_size * img_size    # 展平为向量的尺寸
img_shape = (img_size, img_size)       # 图片的二维尺寸
num_channels = 1                       # 输入为单通道灰度图像
num_classes = 10                       # 类别数目
```
```

##### 2.5. 打印部分样例图片

```python
```python
def plot_images(images, cls_true, cls_pred=None):
    """
     绘制图像，输出真实标签与预测标签
     images: 图像（9 张）
     cls_true: 真实类别
     cls_pred: 预测类别
    """
    assert len(images) == len(cls_true) == 9    # 保证存在 9 张图片
    fig, axes = plt.subplots(3, 3)              # 创建 3x3 个子图的画布
    fig.subplots_adjust(hspace=0.3, wspace=0.3) # 调整每张图之间的间隔
    for i, ax in enumerate(axes.flat):
        # 绘图，将一维向量变为二维矩阵，黑白二值图像使用 binary
        ax.imshow(images[i].reshape(img_shape), cmap='binary')
        if cls_pred is None:                  # 如果未传入预测类别
            xlabel = "True: {0}".format(cls_true[i])
        else:
            xlabel = "True: {0}, Pred: {1}".format(cls_true[i], cls_pred[i])
        ax.set_xlabel(xlabel)
        ax.set_xticks([])                     # 删除坐标信息
        ax.set_yticks([])
    plt.show()
    
indices = np.arange(len(test))
np.random.shuffle(indices)                   # 打乱数据 
indices = indices[:9]                        # 随机取 9 张图片
images = X_test[indices]
cls_true = test[indices]
plot_images(images, cls_true)
```
```

### 三、创建神经网络

##### 3.1 定义网络参数—初始化（权重-偏值项）

```python
```python
# 卷积层 1 参数
filter_size1 = 5          # 5 x 5 卷积核
num_filters1 = 16         # 共 16 个卷积核

# 卷积层 2 参数
filter_size2 = 5          # 5 x 5 卷积核
num_filters2 = 36         # 共 36 个卷积核

# 全连接层 参数
fc_size = 128             # 全连接层神经元个数.

def new_weights(shape):
    return tf.Variable(tf.truncated_normal(shape, stddev=0.05)) # 初始化为随机值
def new_biases(length):
    return tf.Variable(tf.constant(0.05, shape=[length]))      # 初始化为常数
```
```

##### 3.2 定义卷积层

```python
```python
def new_conv_layer(input,                   # 前一层.
    num_input_channels,                     # 前一层通道数
    filter_size,                            # 卷积核尺寸
    num_filters,                            # 卷积核数目
    use_pooling=True):                      # 使用 2x2 max-pooling.
    # 卷积核权重的形状，由 TensorFlow API 决定
    shape = [filter_size, filter_size, num_input_channels, num_filters]
    
    weights = new_weights(shape=shape)      # 根据跟定形状创建权重
    biases = new_biases(length=num_filters) # 创建新的偏置，每个卷积核一个偏置
    """
        创建卷积层。注意 stride 全设置为 1。
        第1个和第4个必须是1，因为第1个是图像的数目，第4个是图像的通道。
        第2和第3指定和左右、上下的步长。
        padding设置为'SAME' 意味着给图像补零，以保证前后像素相同。
    """
    layer = tf.nn.conv2d(input=input,
                        filter=weights,
                        strides=[1, 1, 1, 1],
                        padding='SAME')

    layer += biases     # 给卷积层的输出添加一个偏置，每个卷积通道一个偏置值

    
    if use_pooling:    # 是否使用pooling
        # 这是 2x2 max-pooling, 表明使用 2x2 的窗口，选择每一窗口的最大值作为该窗口的像素，
        # 然后移动2格到下一窗口。
        layer = tf.nn.max_pool(value=layer,
                              ksize=[1, 2, 2, 1],
                              strides=[1, 2, 2, 1],
                              padding='SAME')

    # R(ReLU). 计算 max(x, 0)，把负数的像素值变为0.为原输出添加了一定的非线性特性
    layer = tf.nn.relu(layer)
    """
        注意 relu 通常在pooling前执行，但是由于 relu(max_pool(x)) == max_pool(relu(x))，
        我们可以通过先max_pooling再relu省去75%的计算。
        返回结果层和权重，结果层用于下一层输入，权重用于显式输出
    """
    return layer, weights
```
```

##### 3.3 定义展平层

```python
```python
def flatten_layer(layer):
    # 获取输入层的形状，layer_shape == [num_images, img_height, img_width, num_channels]
    layer_shape = layer.get_shape()

    # 特征数量: img_height * img_width * num_channels
    num_features = layer_shape[1:4].num_elements()
    """
        将形状重塑为 [num_images, num_features].
        注意只设定了第二个维度的尺寸为num_filters，第一个维度为-1，保证第一个维度num_images不变
        展平后的层的形状为:   [num_images, img_height * img_width * num_channels]
    """
    layer_flat = tf.reshape(layer, [-1, num_features])
    return layer_flat, num_features
```
```

##### 3.4 定义全连接层

```python
```python
def new_fc_layer(input,         # 前一层.
                num_inputs,     # 前一层输入维度
                num_outputs,    # 输出维度
                use_relu=True): # 是否使用RELU

    # 更新——权重和偏置.
    weights = new_weights(shape=[num_inputs, num_outputs])
    biases = new_biases(length=num_outputs)

    # 计算 y = wx + b
    layer = tf.matmul(input, weights) + biases
    
    if use_relu:              # 是否使用RELU
        layer = tf.nn.relu(layer)
    return layer
```
```

##### 3.5 定义占位符

```python
```python
x = tf.placeholder(tf.float32, shape=[None, img_size_flat], name='x')          

 # 转换为2维图像
x_image = tf.reshape(x, [-1, img_size, img_size, num_channels])                 

 # 原始输出
y_true = tf.placeholder(tf.float32, shape=[None, num_classes], name='y_true') 

 # 转换为真实类别，与之前的使用placeholder不同
y_true_cls = tf.argmax(y_true, axis=1)
```
```

##### 3.6 连接神经网络（卷积层–展平层–连接层–预测类别–代价函数–优化方法–模型性能度量）

```python
```python
layer_conv1, weights_conv1 = \
new_conv_layer(input=x_image,                        # 输入图像
              num_input_channels=num_channels,       # 输入通道数
              filter_size=filter_size1,              # 卷积核尺寸
              num_filters=num_filters1,              # 卷积核数目
              use_pooling=True)
print(layer_conv1) 

layer_conv2, weights_conv2 = \
new_conv_layer(input=layer_conv1,
               num_input_channels=num_filters1,
               filter_size=filter_size2,
               num_filters=num_filters2,
               use_pooling=True)
print(layer_conv2) 

layer_flat, num_features = flatten_layer(layer_conv2)
print(layer_flat)

layer_fc1 = new_fc_layer(input=layer_flat,          # 展平层输出
                         num_inputs=num_features,   # 输入特征维度
                         num_outputs=fc_size,       # 输出特征维度
                         use_relu=True)
print(layer_fc1)

layer_fc2 = new_fc_layer(input=layer_fc1,           # 上一全连接层
                         num_inputs=fc_size,        # 输入特征维度
                         num_outputs=num_classes,   # 输出类别数
                         use_relu=False)
print(layer_fc2)

y_pred = tf.nn.softmax(layer_fc2)                   # softmax归一化
y_pred_cls = tf.argmax(y_pred, axis=1)              # 真实类别

cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=layer_fc2,labels=y_true)
cost = tf.reduce_mean(cross_entropy)

optimizer = tf.train.AdamOptimizer(learning_rate=1e-4).minimize(cost)  #优化器

correct_prediction = tf.equal(y_pred_cls, y_true_cls)
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
```
```

##### 3.7 定义session 并执行

```python
```python
session = tf.Session()                           # 创建session
session.run(tf.global_variables_initializer())   # 变量初始化

def next_batch(num, data, labels):
    '''
    Return a total of `num` random samples and labels. 
    '''
    idx = np.arange(0 , len(data))
    np.random.shuffle(idx)                     #随机获取数据
    idx = idx[:num]
    data_shuffle = [data[i] for i in idx]
    labels_shuffle = [labels[i] for i in idx]

    return np.asarray(data_shuffle), np.asarray(labels_shuffle)  

train_batch_size = 64
total_iterations = 0           # 统计目前总迭代次数

def optimize(num_iterations):

    global total_iterations    # 迭代次数.
    start_time = time.time()   # 统计用时.

    for i in range(total_iterations, total_iterations + num_iterations):
        # 获取一批数据，放入dict
        #x_batch, y_true_batch = data.train.next_batch(train_batch_size)
        x_batch, y_true_batch = next_batch(train_batch_size, X_train, y_train01)
        #x_batch = X_train.next_batch(train_batch_size)
        #y_true_batch = y_test01.next_batch(train_batch_size)
        feed_dict_train = {x: x_batch,
                           y_true: y_true_batch}
        session.run(optimizer, feed_dict=feed_dict_train)           # 运行优化器

        if i % 100 == 0 or i==899:                                  # 每100轮迭代输出状态
            acc = session.run(accuracy, feed_dict=feed_dict_train)  # 计算训练集准确率.
            msg = "迭代轮次: {0:>6}, 训练准确率: {1:>6.1%}"
            print(msg.format(i + 1, acc))

    total_iterations += num_iterations
    end_time = time.time()
    time_dif = end_time - start_time

    # 输出用时.
    print("用时: " + str(timedelta(seconds=int(round(time_dif))))) 

def plot_example_errors(cls_pred, correct):
    # 计算错误情况
    incorrect = (correct == False)
    images = X_test[incorrect]
    cls_pred = cls_pred[incorrect]
    cls_true = test[incorrect]

    # 随机挑选9个
    indices = np.arange(len(images))
    np.random.shuffle(indices)
    indices = indices[:9] 

    plot_images(images[indices], cls_true[indices], cls_pred[indices])

def plot_confusion_matrix(cls_pred):
    cls_true = test          # 真实类别  

    # 使用scikit-learn的confusion_matrix来计算混淆矩阵
    cm = confusion_matrix(y_true=cls_true, y_pred=cls_pred)

    print(cm)                # 打印混淆矩阵

    # 将混淆矩阵输出为图像
    plt.imshow(cm, interpolation='nearest', cmap=plt.cm.Blues)
    plt.tight_layout()      # 调整图像
    plt.colorbar()
    tick_marks = np.arange(num_classes)
    plt.xticks(tick_marks, range(num_classes))
    plt.yticks(tick_marks, range(num_classes))
    plt.xlabel('Predicted')
    plt.ylabel('True')
    plt.show()

test_batch_size = 256                                  # 将测试集分成更小的批次   

def print_test_accuracy(show_example_errors=False,
                        show_confusion_matrix=False):
    num_test = len(X_test)                             # 测试集图像数量.

    cls_pred = np.zeros(shape=num_test, dtype=np.int)  # 为预测结果申请一个数组.

    i = 0                                              # 数据集的起始id为0
    while i < num_test:
        j = min(i + test_batch_size, num_test)         # j为下一批次的截止id
        images = X_test[i:j, :]                        # 获取i，j之间的图像
        labels = y_test01[i:j, :]                      # 获取相应标签.

        feed_dict = {x: images,   y_true: labels}      # 创建feed_dict

        # 计算预测结果
        cls_pred[i:j] = session.run(y_pred_cls, feed_dict=feed_dict)
        i = j                                          # 设定为下一批次起始值.

    cls_true = test
    correct = (cls_true == cls_pred)                   # 正确的分类
    correct_sum = correct.sum()                        # 正确分类的数量

    acc = float(correct_sum) / num_test                # 分类准确率
    msg = "测试集准确率: {0:.1%}({1}/{2})"              # 打印准确率.
    print(msg.format(acc, correct_sum, num_test))

    if show_example_errors:                            # 打印部分错误样例.
        print("Example errors:")
        plot_example_errors(cls_pred=cls_pred, correct=correct)

    if show_confusion_matrix:                          # 打印混淆矩阵.
        print("Confusion Matrix:")
        plot_confusion_matrix(cls_pred=cls_pred)

optimize(num_iterations=900)
print_test_accuracy(show_example_errors=True)
```
```






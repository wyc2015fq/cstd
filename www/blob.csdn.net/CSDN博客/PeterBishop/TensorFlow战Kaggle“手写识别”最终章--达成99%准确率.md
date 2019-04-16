# TensorFlow战Kaggle“手写识别”最终章--达成99%准确率 - PeterBishop - CSDN博客





2018年12月01日 16:37:58[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：58








> 
这是一个TensorFlow的系列文章，本文是第三篇，在这个系列中，你讲了解到机器学习的一些基本概念、TensorFlow的使用，并能实际完成手写数字识别、图像分类、风格迁移等实战项目。


上一篇文章，我们用CNN实现了手写识别

![](https://upload-images.jianshu.io/upload_images/280127-74b02a656a789723.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/600/format/webp)

CNN网络模型举例

### 一、改进方案
- 1 使用Keras框架，而不是用TensorFlow自己实现，提升编程效率
- 2 增加网络的深度，这里增加到了20层
- 3 每次卷积完之后，加入规范层
- 4 使用最新的SELU激活函数，这是Sepp Hochreiter最新发表在arXiv上的激活函数，Sepp是当年和Jürgen Schmidhuber 一起发明 LSTM 的神级人物。介绍参考这个链接：[「自归一化神经网络」提出新型激活函数SELU](https://link.jianshu.com?t=http://example.net/)

### 二、理论知识补充(最小知识集)

1）BatchNormalization 的计算原理，优缺点

为了避免对数据的感应不铭感，会对数据做处理，使得数据的变化范围不会太大，这个处理叫normalization预处理。Batch normalization 的 batch 是批数据, 把数据分成小批小批进行随机梯度下降. 而且在每批数据进行前向传递 forward propagation 的时候, 对每一层都进行 normalization 的处理。 优点是可以避免数据对激活函数的饱和从而收敛到更好的结果，并且能减少训练时长。

2）rmsprop 优化器的原理，优缺点

RMSprop 是 Geoff Hinton 提出的一种以梯度的平方来自适应调节学习率的优化方法。

优点是，对于常见参数更新较小，自动更新，不用手动调节学习率

缺点是，因为公式中分母上会累加梯度平方，这样在训练中持续增大的话，会使学习率非常小，甚至趋近无穷小

3）池化层的计算原理

池化层是在几个数据中挑选出最能代表这个区域的，把一个区域简化成一个数值，可以取最大值、最小值、平均值。这样可以降低计算量，降低过拟合。

4）softmax与 sigmoid 激活函数的区别

sigmoid将一个real value映射到（0,1）的区间（当然也可以是（-1,1）），这样可以用来做二分类。

而softmax把一个k维的real value向量（a1,a2,a3,a4….）映射成一个（b1,b2,b3,b4….）其中bi是一个0-1的数值，b1到bn累加为1，然后可以根据bi的大小来进行多分类的任务，如取权重最大的一维。

5）categorical_crossentropy 的计算原理，为什么选择交叉熵而不是平均平方误差（MSE）

交叉熵计算原理是−[ylna+(1−y)ln(1−a)]。不选择MSE的原因是，使用MSE在Y取较大值时，权重和偏置的更新速度会很慢，而用交叉熵作为损失函数可以克服这个问题

### 三、代码实现

![](https://upload-images.jianshu.io/upload_images/280127-0dd9f45f8617b58a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/745/format/webp)

1 加载数据集，对数据集进行处理，把输入和结果进行分开

![](https://upload-images.jianshu.io/upload_images/280127-2cb1b64d704a29c7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/897/format/webp)

2 设相关参数

![](https://upload-images.jianshu.io/upload_images/280127-077abfc8bf88a76c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

3 定义模型

![](https://upload-images.jianshu.io/upload_images/280127-c011fcc1b7fec653.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/953/format/webp)

4 查看模型结构，可视化模型

![](https://upload-images.jianshu.io/upload_images/280127-e7465d3467befdd3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

5 训练模型，保存和载入模型

![](https://upload-images.jianshu.io/upload_images/280127-12b51a98bca6a7bb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

6 生成预测结果

### 四、上传Kaggle看结果

请注意，训练是有一定随机性的，如果一次训练的不好，可以反复尝试训练。**尽量准确度在99.2%以上，再生成提交结果。**

![](https://upload-images.jianshu.io/upload_images/280127-e11e0061cd83617a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/957/format/webp)

kaggle结果

虽然手写识别已经有无数人做过了，但提交到Kaggle的只有1600多人，我们的结果在200多位，进入了前20%，对于新手来错，应该还可以了。

```python
import numpy as np
import tensorflow as tf
import pandas as pd
from keras.models import Sequential
from keras.layers import Dense, Activation, Conv2D
from keras.layers import MaxPool2D, Flatten, Dropout, ZeroPadding2D, BatchNormalization
from keras.utils import np_utils
import keras
from keras.models import save_model, load_model
from keras.models import Model

# 1 加载数据集，对数据集进行处理，把输入和结果分开
df = pd.read_csv('train.csv')
data = df.as_matrix()  # data = np.array(df)

df = None

# 打乱顺序
np.random.shuffle(data)

x_train = data[:, 1:]

# 把训练的图片数据转化成28*28的图片
x_train = x_train.reshape(data.shape[0], 28, 28, 1).astype('float32')
x_train = x_train / 255
# 把训练的图片进行one-hot编码
y_train = np_utils.to_categorical(data[:, 0], 10).astype('float32')

# 2 设相关参数
# 设置对训练集的批次大小
batch_size = 64
# 设置卷积滤镜个数
n_filter = 32
# 设置最大池化，池化核大小
pool_size = (2, 2)

# 3 定义网络，安照zeropadding,巻积层，规范层，池化层进行设置
# 这里用了relu激活函数
cnn_net = Sequential()

cnn_net.add(Conv2D(32, kernel_size=(3, 3), strides=(1, 1), input_shape=(28, 28, 1)))
cnn_net.add(Activation('relu'))
cnn_net.add(BatchNormalization(epsilon=1e-6, axis=1))
cnn_net.add(MaxPool2D(pool_size=pool_size))

cnn_net.add(ZeroPadding2D((1, 1)))
cnn_net.add(Conv2D(48, kernel_size=(3, 3)))
cnn_net.add(Activation('relu'))
cnn_net.add(BatchNormalization(epsilon=1e-6, axis=1))
cnn_net.add(MaxPool2D(pool_size=pool_size))

cnn_net.add(ZeroPadding2D((1, 1)))
cnn_net.add(Conv2D(64, kernel_size=(2, 2)))
cnn_net.add(Activation('relu'))
cnn_net.add(BatchNormalization(epsilon=1e-6, axis=1))
cnn_net.add(MaxPool2D(pool_size=pool_size))

cnn_net.add(Dropout(0.25))
cnn_net.add(Flatten())

cnn_net.add(Dense(3168))
cnn_net.add(Activation('relu'))

cnn_net.add(Dense(10))
cnn_net.add(Activation('softmax'))

# 4 查看网络结构
cnn_net.summary()

# from keras.utils.vis_utils import plot_model, model_to_dot
# from Ipython.display import Image, SVG
##可视化模型
# SVG(model_to_dot(cnn_net).create(prog='dot',format='svg'))

# 5 训练模型，保存模型
cnn_net.compile(optimizer='rmsprop',
                loss='binary_crossentropy',
                metrics=['accuracy'])
print('开始训练')
cnn_net.fit(x_train, y_train, batch_size=batch_size, epochs=50, verbose=1, validation_split=0.2)
print('训练结束')
cnn_net.save('cnn_net_model.h5')

# 6 加载模型
cnn_net = load_model('cnn_net_model.h5')

# 7 生成提交预测结果
df = pd.read_csv('test.csv')
x_valid = df.values.astype('float32')
n_valid = x_valid.shape[0]
x_valid = x_valid.reshape(n_valid, 28, 28, 1)
x_valid = x_valid / 255

y_pred = cnn_net.predict_classes(x_valid, batch_size=32, verbose=1)
np.savetxt('DeepConvNN1.csv', np.c_[range(1, len(y_pred) + 1), y_pred], delimiter=',' \
           , header='ImageId,Label', comments='', fmt='%d')
```






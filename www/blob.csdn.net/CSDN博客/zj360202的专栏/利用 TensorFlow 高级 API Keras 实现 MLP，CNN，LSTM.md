# 利用 TensorFlow 高级 API Keras 实现 MLP，CNN，LSTM - zj360202的专栏 - CSDN博客





2018年01月17日 17:21:18[zj360202](https://me.csdn.net/zj360202)阅读数：834









TensorFlow 代码长，不好读，不好理解，这可能是很多初学者的痛。在一些开发者努力下基于 TF 构建了更高级的 API，无需再用冗长难记的底层 API 构建模型。在众多高级 API 中，Keras 和 TFLearn 较为流行。


我们前面文章《[利用
 TFLearn 快速搭建经典深度学习模型](http://mp.weixin.qq.com/s?__biz=MzI2MzYwNzUyNg==&mid=2247483810&idx=1&sn=a3c5fba3d2550a5a44696022665dd6fe&chksm=eab8042bddcf8d3d79c94cd1c0a75edae81ea0dd73f76d27824781002ef78957c43a67df30cb&scene=21#wechat_redirect)》介绍了 TFLearn 高级 API，今天内容可以作为对照进行学习。 





> 

Keras 是一个高级神经网络库，用 Python 语言写成，可以运行在 TensorFlow 或者 Theano 之上。它关注快速试验和原型设计。“以最短的时间将想法转换为结果是做好研究的关键”。


> 

如果你需要一个这样的深度学习库：
- 

允许简单快速的原型设计（完全模块化，最小化，可扩展）；

- 

支持 CNN 和 RNN，以及二者的结合物；

- 

支持任意连接图（多输入、多输出训练）；

- 

在 CPU 和 GPU 上无缝运行；



那你选 Keras 就选对了。


更多文档请访问：https://keras.io/









安装 Keras 很简单，只需在一台安装了 TensorFlow 的机器上，先下载 Keras 源码：




$ git clone https://github.com/fchollet/keras.git --recursive

$ cd keras/

$ sudo python setup.py install





最后一步会根据当前环境情况下载、安装一些依赖包，像 numpy, scipy, pyyaml, HDF5 和 h5py 等。安装结束后，就能直接运行例程了。例程位于 keras/examples/ 目录下。


先看一个 MLP 的例子（mnist_mlp.py）：





# 加载 Keras 模型相关的 Python 模块


from keras.models import Sequential


from keras.layers.core import Dense, Dropout, Activation


from keras.optimizers import RMSprop





# 构建 MLP 网络


model = Sequential()    


model.add(Dense(512, input_shape=(784,))) # 全连接层


model.add(Activation('relu')) # ReLU


model.add(Dropout(0.2)) # Dropout


model.add(Dense(512)) # 全连接层


model.add(Activation('relu')) # ReLU


model.add(Dropout(0.2)) # Dropout


model.add(Dense(10)) # 分类层


model.add(Activation('softmax')) # Softmax


model.summary() # 打印模型


model.compile(loss='categorical_crossentropy',


              optimizer=RMSprop(),


              metrics=['accuracy']) # 生成模型





学过 Torch 的同学会意识到，Keras 的 API 与 Torch 构建模型的 API 比较相似。





运行例程：


$ python mnist_mlp.py


运行输出能显示网络的结构：

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAFbyurrIJribn9C9liaP4W8ZAeX6Tic04TMdVDxicEP8lgWr7NaNr1FiaNgMo0KGEezh1Y6ZicXwAKZQqBw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)





经过 20 个 epoch，分类准确率达到 98.48%

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAFbyurrIJribn9C9liaP4W8ZAaTFEPCFd6LakInjic9mKF7NK0H9FVsZPEO8RiaE1jls70SaseAwibrVNQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)








下面是一个 CNN 例子（mnist_cnn.py）：





# 加载 Keras 模型相关的 Python 模块


from keras.models import Sequential


from keras.layers import Dense, Dropout, Activation, Flatten


from keras.layers import Convolution2D, MaxPooling2D


# 构建 CNN 网络


model = Sequential()


model.add(Convolution2D(nb_filters, kernel_size[0], kernel_size[1],


                        border_mode='valid',


                        input_shape=input_shape)) # 卷积层


model.add(Activation('relu')) # ReLU


model.add(Convolution2D(nb_filters, kernel_size[0], kernel_size[1])) # 卷积层


model.add(Activation('relu')) # ReLU


model.add(MaxPooling2D(pool_size=pool_size)) # Maxpooling


model.add(Dropout(0.25)) # Dropout


model.add(Flatten()) # 将响应转换为一维向量


model.add(Dense(128)) # 全连接层


model.add(Activation('relu')) # ReLU


model.add(Dropout(0.5)) # Dropout


model.add(Dense(nb_classes)) # 分类层


model.add(Activation('softmax')) # Softmax


model.compile(loss='categorical_crossentropy',


              optimizer='adadelta',


              metrics=['accuracy']) # 生成模型


运行例程：





$ python mnist_cnn.py






如下图显示了 CNN 模型的结构（如果没有显示，需要在代码中model.compile 前面增加一行 model.summary() ）：

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAFbyurrIJribn9C9liaP4W8ZAD200iaWbk8qxV0u90yvLP5JQRibwVIThR4xg9C1SlQOfm0rH59cOVZfQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


最后显示分类准确率：

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAFbyurrIJribn9C9liaP4W8ZA2PALqdziaX1TISDVia3uzoMPuibfZwmVrc14koQbPhaYGDgUjG7r77crQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


准确率提升到 99%。






接着看一个 LSTM 例子（imdb_lstm.py），完成 IMDB 上句子分类任务：





# 加载 Keras 模型相关的 Python 模块


from keras.models import Sequential


from keras.layers import Dense, Activation, Embedding


from keras.layers import LSTM


# 构建 LSTM 模型


model = Sequential()


model.add(Embedding(max_features, 128, dropout=0.2)) # 词嵌入


model.add(LSTM(128, dropout_W=0.2, dropout_U=0.2))  # LSTM 层


model.add(Dense(1))    # 二分类层


model.add(Activation('sigmoid')) # Sigmoid 


model.summary()  # 打印模型



model.compile(loss='binary_crossentropy',


              optimizer='adam',


              metrics=['accuracy']) # 生成模型





运行这个例子：





$ python imdb_lstm.py





查看网络结构：

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAFbyurrIJribn9C9liaP4W8ZAnYg3KhtVeWgBopGlvwO8JeTANicmJCJibv4hibPvue1hOMflGVT9pkic7g/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

![](http://mmsns.qpic.cn/mmsns/a9UoojghtAFbyurrIJribn9C9liaP4W8ZAbpo4s4ibtNH7HpRsNe34rNQ/0?wx_lazy=1)





最终实现了 81.9% 的分类准确率。





通过本文内容，可以发现利用高级 API  Keras 能很快搭建各种模型，无需人工对底层运算符进行连接和调试，节省了开发时间。




小技巧 1：如何让 Keras 后端从 TensorFlow 切换为 Theano？


# cat ~/.keras/keras.json


{


    "image_dim_ordering": "tf",


    "epsilon": 1e-07,


    "floatx": "float32",


    "backend": "tensorflow"


}


只需将 "backend" 后面对应的值改为 "theano"，保存，下次运行 Keras 时会生效。生效后输出会有提示：

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAFbyurrIJribn9C9liaP4W8ZALY9iaCbhPk8Je4KZiapPFMCicH3hSH5vKV9ibB96iaVAZDCNTT7nuaE2A5Q/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


实测用同样的模型 mnist_mlp.py， backend 采用 Theano 相比 TensorFlow 慢 10 倍以上。如果希望提速，需要对 Theano 做一些参数配置。




小技巧 2：下载数据集太慢怎么破？


关注本公众号，回复“20170219”，获得本文用到的数据集下载地址。例如 MNIST 例程，只需将 mnist.pkl.gz 放在 ~/.keras/datasets/ 目录下，运行例程即可跳过下载数据过程。




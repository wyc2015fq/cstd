# Keras入门（1）——Keras的安装与运行 - 刘炫320的博客 - CSDN博客
2017年04月14日 15:13:50[刘炫320](https://me.csdn.net/qq_35082030)阅读数：5668
# 0. 写在前面
从几天开始，我就新跳入了一个坑里，这个坑就是大家基本上竞相跳的深度学习-Keras。但是入手并不顺利啊，虽然它已经是深度学习中，封装的十分棒的框架了，但是就和武器一样，越是厉害的武器，限制条件越多。下面看看我的坎坷之路。
# 1. Keras安装
## 1.1安装前准备
咱属于个人开发，要啥没啥，环境只能是笔记本+windows+anaconda+python3.5+pycharm。
那装Keras需要装什么，大家可以先看一下这里：[Keras的设备推荐](http://keras-cn.readthedocs.io/en/latest/for_beginners/keras_windows/)
说是要装很多，基本上使用anaconda+pycharm的话，直接在pycharm里直接安装就好了。
## 2.2安装后填坑
这要填的第一个坑，就是它的计算框架，默认的是TensorFlow，虽然它支持Theano，但是你想要调换的话，还需要参考：[Keras切换后端](http://blog.csdn.net/tangwenbo124/article/details/52627774?locationNum=7)，不然它默认的是TensorFlow。
缺什么咱们再补什么呗，于是就从Pycharm里的库包下载那边去找tensorflow，别说还真有，但是安装，额，安装失败。
我百思不得其解，看人家网上教程都装的挺6的，虽然人家是linux下的吧，但也不至于装不上吧。后来忽然想起了一个重要问题，我这是在中国。
对，tensorflow是谷歌的，于是需要翻墙。我又气喘吁吁的翻了墙，把Tensorflow安装了上去，我是笔记本，自己的，烧坏了显卡多不好，我就没装GPU版本的。
至此，基本上都能运行了。在我尝试运行了一个Test以后，发现又有很多问题,原来看的是[Keras中文文档](http://keras-cn.readthedocs.io/en/latest/getting_started/sequential_model/)。觉得不错啊，有中文学起来快多了，后来才发现，开什么玩笑，一个都没法运行，总是报各种各样的错。后来我才去找了[Keras英文文档](https://keras.io/)，这才发现什么是差距啊，英文文档写的很好，而且还带连接，有问题了直接就能链接过去，哎，链接过去，链接，链接不过去。。。原来是又被强了，只好又翻墙，到了[TensorFlow的官方windos安装帮助](https://www.tensorflow.org/install/install_windows)，才彻底解决。
反正中间也遇到了不少困难，报错了，看一下报的错，然后多去Stackoverflow那边看看，英语的描述确实能恰到好处的解决。最后也历经坎坷，能跑了一个测试样例：
```python
from keras.models import Sequential
from keras.layers import LSTM, Dense
import numpy as np
data_dim = 16
timesteps = 8
num_classes = 10
# expected input data shape: (batch_size, timesteps, data_dim)
model = Sequential()
model.add(LSTM(32, return_sequences=True,
               input_shape=(timesteps, data_dim)))  # returns a sequence of vectors of dimension 32
model.add(LSTM(32, return_sequences=True))  # returns a sequence of vectors of dimension 32
model.add(LSTM(32))  # return a single vector of dimension 32
model.add(Dense(10, activation='softmax'))
model.compile(loss='categorical_crossentropy',
              optimizer='sgd',
              metrics=['accuracy'])
# Generate dummy training data
x_train = np.random.random((1000, timesteps, data_dim))
y_train = np.random.random((1000, num_classes))
# Generate dummy validation data
x_val = np.random.random((100, timesteps, data_dim))
y_val = np.random.random((100, num_classes))
model.fit(x_train, y_train,
          batch_size=64, epochs=1,
          validation_data=(x_val, y_val))
```
另外说一点，在英文的Keras指导手册中，给了一个[Github的Examples](https://github.com/liuxuan320/keras)，Examples文件夹里都是它文档中提到的例子，可是那些非常诱人的例子，例如使用多层感知机的路透社的新闻标题分类（2017NLPCC Shared Task也有类似的任务），结果数据集需要从网上下，然而一如既往的被强了，费了九牛二虎之力下完之后，才发现什么不能解压，是什么鬼。暂时没搞定，等我搞定了再来解释。
#2. 绘制Keras模型图
我是刚刚入门深度学习，不太知道这些是啥意思，只听过CNN、RNN、LSTM什么的，根本没有一个直观的概念，还有什么序贯模型和函数模型，不太懂，也没有图能解释，等等！图，在Keras中文指南里，有这样一幅图：
![LSTM](https://img-blog.csdn.net/20170414145437150?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUwODIwMzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我就想如果能可视化多好了，才发现，确实可以，这个就用模型可视化来解决。具体的参考[Keras的模型可视化](http://blog.csdn.net/lk7688535/article/details/52882408)。但是它有几个问题，首先，我们使用的是集成开发环境，还用了Pycharm，因此我们可以去pycharm库包那里下载，不用那么繁琐，但是要下载的有这么几个模块：
graphviz,pydot,pydot_ng
但是graphviz使用包安装以后还是不行，还是需要去官网安装，并且配置环境变量。另外，最重要的一点是，当配置完环境变量以后，一定要重启pycharm，这样才行。
另外一点就是，现在的可视化模块不叫做：keras.utils.visualize_util 了，改成了keras.utils.vis_utils了，因此引入的时候要特别注意。
等所有错误都不报的时候，我们就可以完成了绘图的过程：
```python
from keras.models import Sequential
from keras.layers import LSTM, Dense
#这一行新加的，用于导入绘图包
from keras.utils.vis_utils import plot_model
import numpy as np
data_dim = 16
timesteps = 8
num_classes = 10
# expected input data shape: (batch_size, timesteps, data_dim)
model = Sequential()
model.add(LSTM(32, return_sequences=True,
               input_shape=(timesteps, data_dim)))  # returns a sequence of vectors of dimension 32
model.add(LSTM(32, return_sequences=True))  # returns a sequence of vectors of dimension 32
model.add(LSTM(32))  # return a single vector of dimension 32
model.add(Dense(10, activation='softmax'))
model.compile(loss='categorical_crossentropy',
              optimizer='sgd',
              metrics=['accuracy'])
# Generate dummy training data
x_train = np.random.random((1000, timesteps, data_dim))
y_train = np.random.random((1000, num_classes))
# Generate dummy validation data
x_val = np.random.random((100, timesteps, data_dim))
y_val = np.random.random((100, num_classes))
model.fit(x_train, y_train,
          batch_size=64, epochs=1,
          validation_data=(x_val, y_val))
#这一行新加的，用于绘图
plot_model(model, to_file='model1.png',show_shapes=True)
```
# 3. 基本的运行过程
其实我也不太懂，只是刚开始的时候来对整个模型有一个浅显的认识。总共做深度学习就这么几个步骤：
- 载入训练数据和测试数据
- 训练数据和测试数据预处理，比如向量化等。
- 构造学习模型，也就是那个Add（）那个。
- 声明模型的各个类型，也就是Compile（）那个。
- 然后就是fit()了，
- 最后就是验证模型、保存模型等等之类的。
更细节的讲解，我们需要在以后学习的过程中讲解。

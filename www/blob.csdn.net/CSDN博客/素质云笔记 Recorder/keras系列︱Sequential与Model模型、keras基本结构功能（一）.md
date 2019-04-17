# keras系列︱Sequential与Model模型、keras基本结构功能（一） - 素质云笔记/Recorder... - CSDN博客





2017年06月04日 11:23:08[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：37346








> 
不得不说，这深度学习框架更新太快了尤其到了Keras2.0版本，快到Keras中文版好多都是错的，快到官方文档也有旧的没更新，前路坑太多。

到发文为止，已经有theano/tensorflow/CNTK支持keras,虽然说tensorflow造势很多，但是笔者认为接下来Keras才是正道。

笔者先学的caffe，从使用来看，比caffe简单超级多，非常好用，特别是重新训练一个模型，但是呢，在fine-tuning的时候，遇到了很多问题，对新手比较棘手。
中文文档：[http://keras-cn.readthedocs.io/en/latest/](http://keras-cn.readthedocs.io/en/latest/)

官方文档：[https://keras.io/](https://keras.io/)

文档主要是以keras2.0。
.

### Keras系列：

1、[keras系列︱Sequential与Model模型、keras基本结构功能（一）](http://blog.csdn.net/sinat_26917383/article/details/72857454)

2、[keras系列︱Application中五款已训练模型、VGG16框架（Sequential式、Model式）解读（二）](http://blog.csdn.net/sinat_26917383/article/details/72859145)

3、[keras系列︱图像多分类训练与利用bottleneck features进行微调（三）](http://blog.csdn.net/sinat_26917383/article/details/72861152)

4、[keras系列︱人脸表情分类与识别：opencv人脸检测+Keras情绪分类（四）](http://blog.csdn.net/sinat_26917383/article/details/72885715)

5、[keras系列︱迁移学习：利用InceptionV3进行fine-tuning及预测、完整案例（五）](http://blog.csdn.net/sinat_26917383/article/details/72982230)
## 零、keras介绍与基本的模型保存

写成了思维导图，便于观察与理解。

### 1.keras网络结构

![这里写图片描述](https://img-blog.csdn.net/20170604101316305?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

###2.keras网络配置
![这里写图片描述](https://img-blog.csdn.net/20170604101328219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中回调函数callbacks应该是keras的精髓~

###3.keras预处理功能
![这里写图片描述](https://img-blog.csdn.net/20170604101335306?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
###4、模型的节点信息提取

```python
# 节点信息提取
config = model.get_config()  # 把model中的信息，solver.prototxt和train.prototxt信息提取出来
model = Model.from_config(config)  # 还回去
# or, for Sequential:
model = Sequential.from_config(config) # 重构一个新的Model模型，用去其他训练，fine-tuning比较好用
```

###5、 模型概况查询（包括权重查询）

```python
# 1、模型概括打印
model.summary()

# 2、返回代表模型的JSON字符串，仅包含网络结构，不包含权值。可以从JSON字符串中重构原模型：
from models import model_from_json

json_string = model.to_json()
model = model_from_json(json_string)

# 3、model.to_yaml：与model.to_json类似，同样可以从产生的YAML字符串中重构模型
from models import model_from_yaml

yaml_string = model.to_yaml()
model = model_from_yaml(yaml_string)

# 4、权重获取
model.get_layer()      #依据层名或下标获得层对象
model.get_weights()    #返回模型权重张量的列表，类型为numpy array
model.set_weights()    #从numpy array里将权重载入给模型，要求数组具有与model.get_weights()相同的形状。

# 查看model中Layer的信息
model.layers 查看layer信息
```

###6、模型保存与加载

```python
model.save_weights(filepath)
# 将模型权重保存到指定路径，文件类型是HDF5（后缀是.h5）

model.load_weights(filepath, by_name=False)
# 从HDF5文件中加载权重到当前模型中, 默认情况下模型的结构将保持不变。
# 如果想将权重载入不同的模型（有些层相同）中，则设置by_name=True，只有名字匹配的层才会载入权重
```

.

### 7、如何在keras中设定GPU使用的大小

本节来源于：[深度学习theano/tensorflow多显卡多人使用问题集](https://zhuanlan.zhihu.com/p/23250782)（参见：[Limit the resource usage for tensorflow backend · Issue #1538 · fchollet/keras · GitHub](http://link.zhihu.com/?target=https://github.com/fchollet/keras/issues/1538)）

在使用keras时候会出现总是占满GPU显存的情况，可以通过重设backend的GPU占用情况来进行调节。

```
import tensorflow as tf
from keras.backend.tensorflow_backend import set_session
config = tf.ConfigProto()
config.gpu_options.per_process_gpu_memory_fraction = 0.3
set_session(tf.Session(config=config))
```

需要注意的是，虽然代码或配置层面设置了对显存占用百分比阈值，但在实际运行中如果达到了这个阈值，程序有需要的话还是会突破这个阈值。换而言之如果跑在一个大数据集上还是会用到更多的显存。以上的显存限制仅仅为了在跑小数据集时避免对显存的浪费而已。（2017年2月20日补充）

### 8.更科学地模型训练与模型保存

```
filepath = 'model-ep{epoch:03d}-loss{loss:.3f}-val_loss{val_loss:.3f}.h5'
checkpoint = ModelCheckpoint(filepath, monitor='val_loss', verbose=1, save_best_only=True, mode='min')
# fit model
model.fit(x, y, epochs=20, verbose=2, callbacks=[checkpoint], validation_data=(x, y))
```

save_best_only打开之后，会如下：

```
ETA: 3s - loss: 0.5820Epoch 00017: val_loss did not improve
```

如果val_loss 提高了就会保存，没有提高就不会保存。

### 9.如何在keras中使用tensorboard

```
RUN = RUN + 1 if 'RUN' in locals() else 1   # locals() 函数会以字典类型返回当前位置的全部局部变量。

    LOG_DIR = model_save_path + '/training_logs/run{}'.format(RUN)
    LOG_FILE_PATH = LOG_DIR + '/checkpoint-{epoch:02d}-{val_loss:.4f}.hdf5'   # 模型Log文件以及.h5模型文件存放地址

    tensorboard = TensorBoard(log_dir=LOG_DIR, write_images=True)
    checkpoint = ModelCheckpoint(filepath=LOG_FILE_PATH, monitor='val_loss', verbose=1, save_best_only=True)
    early_stopping = EarlyStopping(monitor='val_loss', patience=5, verbose=1)

    history = model.fit_generator(generator=gen.generate(True), steps_per_epoch=int(gen.train_batches / 4),
                                  validation_data=gen.generate(False), validation_steps=int(gen.val_batches / 4),
                                  epochs=EPOCHS, verbose=1, callbacks=[tensorboard, checkpoint, early_stopping])
```

都是在回调函数中起作用：
- 
EarlyStopping patience：当early

（1）stop被激活（如发现loss相比上一个epoch训练没有下降），则经过patience个epoch后停止训练。

（2）mode：‘auto’，‘min’，‘max’之一，在min模式下，如果检测值停止下降则中止训练。在max模式下，当检测值不再上升则停止训练。- 
模型检查点ModelCheckpoint

（1）save_best_only：当设置为True时，将只保存在验证集上性能最好的模型

（2） mode：‘auto’，‘min’，‘max’之一，在save_best_only=True时决定性能最佳模型的评判准则，例如，当监测值为val_acc时，模式应为max，当检测值为val_loss时，模式应为min。在auto模式下，评价准则由被监测值的名字自动推断。

（3）save_weights_only：若设置为True，则只保存模型权重，否则将保存整个模型（包括模型结构，配置信息等）

（4）period：CheckPoint之间的间隔的epoch数- 
可视化tensorboard write_images: 是否将模型权重以图片的形式可视化


其他内容可参考[keras中文文档](https://keras-cn.readthedocs.io/en/latest/other/callbacks/#tensorboard)

.

## 一、Sequential 序贯模型

序贯模型是函数式模型的简略版，为最简单的线性、从头到尾的结构顺序，不分叉。

### Sequential模型的基本组件

一般需要：
- 1、model.add，添加层；
- 2、model.compile,模型训练的BP模式设置；
- 3、model.fit，模型训练参数设置 + 训练；
- 4、模型评估
- 5、模型预测

#### 1. add：添加层——train_val.prototxt

```python
add(self, layer)

# 譬如：
model.add(Dense(32, activation='relu', input_dim=100))
model.add(Dropout(0.25))
```

add里面只有层layer的内容，当然在序贯式里面，也可以model.add（other_model）加载另外模型，在函数式里面就不太一样，详见函数式。

#### 2、compile 训练模式——solver.prototxt文件

```python
compile(self, optimizer, loss, metrics=None, sample_weight_mode=None)
```

其中：

optimizer：   字符串（预定义优化器名）或优化器对象，参考优化器

loss：        字符串（预定义损失函数名）或目标函数，参考损失函数

metrics：     列表，包含评估模型在训练和测试时的网络性能的指标，典型用法是metrics=[‘accuracy’]

sample_weight_mode：如果你需要按时间步为样本赋权（2D权矩阵），将该值设为“temporal”。

默认为“None”，代表按样本赋权（1D权）。在下面fit函数的解释中有相关的参考内容。

kwargs：       使用TensorFlow作为后端请忽略该参数，若使用Theano作为后端，kwargs的值将会传递给 K.function
注意：

模型在使用前必须编译，否则在调用fit或evaluate时会抛出异常。

#### 3、fit 模型训练参数+训练——train.sh+soler.prototxt（部分）

```python
fit(self, x, y, batch_size=32, epochs=10, verbose=1, callbacks=None, validation_split=0.0, validation_data=None, shuffle=True, class_weight=None, sample_weight=None, initial_epoch=0)
```

本函数将模型训练nb_epoch轮，其参数有：
- x：输入数据。如果模型只有一个输入，那么x的类型是numpy

array，如果模型有多个输入，那么x的类型应当为list，list的元素是对应于各个输入的numpy array
- y：标签，numpy array
- batch_size：整数，指定进行梯度下降时每个batch包含的样本数。训练时一个batch的样本会被计算一次梯度下降，使目标函数优化一步。
- epochs：整数，训练的轮数，每个epoch会把训练集轮一遍。
- verbose：日志显示，0为不在标准输出流输出日志信息，1为输出进度条记录，2为每个epoch输出一行记录
- callbacks：list，其中的元素是keras.callbacks.Callback的对象。这个list中的回调函数将会在训练过程中的适当时机被调用，参考回调函数
- validation_split：0~1之间的浮点数，用来指定训练集的一定比例数据作为验证集。验证集将不参与训练，并在每个epoch结束后测试的模型的指标，如损失函数、精确度等。注意，validation_split的划分在shuffle之前，因此如果你的数据本身是有序的，需要先手工打乱再指定validation_split，否则可能会出现验证集样本不均匀。
- validation_data：形式为（X，y）的tuple，是指定的验证集。此参数将覆盖validation_spilt。
- shuffle：布尔值或字符串，一般为布尔值，表示是否在训练过程中随机打乱输入样本的顺序。若为字符串“batch”，则是用来处理HDF5数据的特殊情况，它将在batch内部将数据打乱。
- class_weight：字典，将不同的类别映射为不同的权值，该参数用来在训练过程中调整损失函数（只能用于训练）
- sample_weight：权值的numpy

array，用于在训练时调整损失函数（仅用于训练）。可以传递一个1D的与样本等长的向量用于对样本进行1对1的加权，或者在面对时序数据时，传递一个的形式为（samples，sequence_length）的矩阵来为每个时间步上的样本赋不同的权。这种情况下请确定在编译模型时添加了sample_weight_mode=‘temporal’。
- initial_epoch: 从该参数指定的epoch开始训练，在继续之前的训练时有用。

fit函数返回一个History的对象，其History.history属性记录了损失函数和其他指标的数值随epoch变化的情况，如果有验证集的话，也包含了验证集的这些指标变化情况
**注意：**

要与之后的fit_generator做区别，两者输入x/y不同。
#### 4.evaluate 模型评估

```python
evaluate(self, x, y, batch_size=32, verbose=1, sample_weight=None)
```

本函数按batch计算在某些输入数据上模型的误差，其参数有：
- x：输入数据，与fit一样，是numpy array或numpy array的list
- y：标签，numpy array
- batch_size：整数，含义同fit的同名参数
- verbose：含义同fit的同名参数，但只能取0或1
- sample_weight：numpy array，含义同fit的同名参数

本函数返回一个测试误差的标量值（如果模型没有其他评价指标），或一个标量的list（如果模型还有其他的评价指标）。model.metrics_names将给出list中各个值的含义。

如果没有特殊说明，以下函数的参数均保持与fit的同名参数相同的含义

如果没有特殊说明，以下函数的verbose参数（如果有）均只能取0或1

#### 5 predict 模型评估

```python
predict(self, x, batch_size=32, verbose=0)
predict_classes(self, x, batch_size=32, verbose=1)
predict_proba(self, x, batch_size=32, verbose=1)
```

本函数按batch获得输入数据对应的输出，其参数有：

函数的返回值是预测值的numpy array

predict_classes：本函数按batch产生输入数据的类别预测结果；

predict_proba：本函数按batch产生输入数据属于各个类别的概率
#### 6 on_batch 、batch的结果，检查

```python
train_on_batch(self, x, y, class_weight=None, sample_weight=None)
test_on_batch(self, x, y, sample_weight=None)
predict_on_batch(self, x)
```
- train_on_batch：本函数在一个batch的数据上进行一次参数更新，函数返回训练误差的标量值或标量值的list，与evaluate的情形相同。
- test_on_batch：本函数在一个batch的样本上对模型进行评估，函数的返回与evaluate的情形相同
- predict_on_batch：本函数在一个batch的样本上对模型进行测试，函数返回模型在一个batch上的预测结果

#### 7 fit_generator

```python
#利用Python的生成器，逐个生成数据的batch并进行训练。
#生成器与模型将并行执行以提高效率。
#例如，该函数允许我们在CPU上进行实时的数据提升，同时在GPU上进行模型训练
# 参考链接：http://keras-cn.readthedocs.io/en/latest/models/sequential/
```

有了该函数，图像分类训练任务变得很简单。

```python
fit_generator(self, generator, steps_per_epoch, epochs=1, verbose=1, callbacks=None, validation_data=None, validation_steps=None, class_weight=None, max_q_size=10, workers=1, pickle_safe=False, initial_epoch=0)

# 案例：
def generate_arrays_from_file(path):
    while 1:
            f = open(path)
            for line in f:
                # create Numpy arrays of input data
                # and labels, from each line in the file
                x, y = process_line(line)
                yield (x, y)
        f.close()

model.fit_generator(generate_arrays_from_file('/my_file.txt'),
        samples_per_epoch=10000, epochs=10)
```

其他的两个辅助的内容：

```python
evaluate_generator(self, generator, steps, max_q_size=10, workers=1, pickle_safe=False)
predict_generator(self, generator, steps, max_q_size=10, workers=1, pickle_safe=False, verbose=0)
```

evaluate_generator：本函数使用一个生成器作为数据源评估模型，生成器应返回与test_on_batch的输入数据相同类型的数据。该函数的参数与fit_generator同名参数含义相同，steps是生成器要返回数据的轮数。

predcit_generator：本函数使用一个生成器作为数据源预测模型，生成器应返回与test_on_batch的输入数据相同类型的数据。该函数的参数与fit_generator同名参数含义相同，steps是生成器要返回数据的轮数。

### 案例一：简单的2分类

For a single-input model with 2 classes (binary classification):

```python
from keras.models import Sequential
from keras.layers import Dense, Activation
```

```python
#模型搭建阶段
model= Sequential()
model.add(Dense(32, activation='relu', input_dim=100))
# Dense(32) is a fully-connected layer with 32 hidden units.
model.add(Dense(1, activation='sigmoid'))
model.compile(optimizer='rmsprop',
              loss='binary_crossentropy',
              metrics=['accuracy'])
```

其中：

Sequential()代表类的初始化；

Dense代表全连接层，此时有32个全连接层，最后接relu，输入的是100维度

model.add，添加新的全连接层，

compile，跟prototxt一样，一些训练参数,solver.prototxt
```python
# Generate dummy data
import numpy as np
data = np.random.random((1000, 100))
labels = np.random.randint(2, size=(1000, 1))

# Train the model, iterating on the data in batches of 32 samples
model.fit(data, labels, nb_epoch =10, batch_size=32)
```

之前报过这样的错误，是因为版本的问题。 版本1.2里面是nb_epoch ，而keras2.0是epochs = 10

```
error:
    TypeError: Received unknown keyword arguments: {'epochs': 10}
```

其中：

epoch=batch_size * iteration,10次epoch代表训练十次训练集

### 案例二:多分类-VGG的卷积神经网络

```python
import numpy as np
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.optimizers import SGD
from keras.utils import np_utils

# Generate dummy data
x_train = np.random.random((100, 100, 100, 3))
# 100张图片，每张100*100*3
y_train = keras.utils.to_categorical(np.random.randint(10, size=(100, 1)), num_classes=10)
# 100*10
x_test = np.random.random((20, 100, 100, 3))
y_test = keras.utils.to_categorical(np.random.randint(10, size=(20, 1)), num_classes=10)
# 20*100

model = Sequential()
# input: 100x100 images with 3 channels -> (100, 100, 3) tensors.
# this applies 32 convolution filters of size 3x3 each.
model.add(Conv2D(32, (3, 3), activation='relu', input_shape=(100, 100, 3)))
model.add(Conv2D(32, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))

model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(Conv2D(64, (3, 3), activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))

model.add(Flatten())
model.add(Dense(256, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(10, activation='softmax'))

sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
model.compile(loss='categorical_crossentropy', optimizer=sgd)

model.fit(x_train, y_train, batch_size=32, epochs=10)
score = model.evaluate(x_test, y_test, batch_size=32)
```

标准序贯网络，标签的训练模式
**注意：**

这里非常重要的一点，对于我这样的新手，这一步的作用？
```
keras.utils.to_categorical
```

特别是多分类时候，我之前以为输入的就是一列（100，），但是keras在多分类任务中是不认得这个的，所以需要再加上这一步，让其转化为Keras认得的数据格式。

### 案例三：使用LSTM的序列分类

```python
from keras.models import Sequential
from keras.layers import Dense, Dropout
from keras.layers import Embedding
from keras.layers import LSTM

model = Sequential()
model.add(Embedding(max_, output_dim=256))
model.add(LSTM(128))
model.add(Dropout(0.5))
model.add(Dense(1, activation='sigmoid'))

model.compile(loss='binary_crossentropy',
              optimizer='rmsprop',
              metrics=['accuracy'])

model.fit(x_train, y_train, batch_size=16, epochs=10)
score = model.evaluate(x_test, y_test, batch_size=16)
```

.

## 三、Model式模型

来自keras中文文档：[http://keras-cn.readthedocs.io/en/latest/](http://keras-cn.readthedocs.io/en/latest/)

比序贯模型要复杂，但是效果很好，可以同时/分阶段输入变量，分阶段输出想要的模型；

一句话，只要你的模型不是类似VGG一样一条路走到黑的模型，或者你的模型需要多于一个的输出，那么你总应该选择函数式模型。
不同之处：

书写结构完全不一致

## 函数式模型基本属性与训练流程

一般需要：

1、model.layers，添加层信息；

2、model.compile,模型训练的BP模式设置；

3、model.fit，模型训练参数设置 + 训练；

4、evaluate，模型评估；

5、predict 模型预测
### 1 常用Model属性

```python
model.layers：组成模型图的各个层
model.inputs：模型的输入张量列表
model.outputs：模型的输出张量列表
```

### 2 compile 训练模式设置——solver.prototxt

```python
compile(self, optimizer, loss, metrics=None, loss_weights=None, sample_weight_mode=None)
```

本函数编译模型以供训练，参数有

optimizer：优化器，为预定义优化器名或优化器对象，参考优化器

loss：损失函数，为预定义损失函数名或一个目标函数，参考损失函数

metrics：列表，包含评估模型在训练和测试时的性能的指标，典型用法是metrics=[‘accuracy’]如果要在多输出模型中为不同的输出指定不同的指标，可像该参数传递一个字典，例如metrics={‘ouput_a’: ‘accuracy’}

sample_weight_mode：如果你需要按时间步为样本赋权（2D权矩阵），将该值设为“temporal”。默认为“None”，代表按样本赋权（1D权）。

如果模型有多个输出，可以向该参数传入指定sample_weight_mode的字典或列表。在下面fit函数的解释中有相关的参考内容。
【Tips】如果你只是载入模型并利用其predict，可以不用进行compile。在Keras中，compile主要完成损失函数和优化器的一些配置，是为训练服务的。predict会在内部进行符号函数的编译工作（通过调用_make_predict_function生成函数）

### 3  fit 模型训练参数设置 + 训练

```python
fit(self, x=None, y=None, batch_size=32, epochs=1, verbose=1, callbacks=None, validation_split=0.0, validation_data=None, shuffle=True, class_weight=None, sample_weight=None, initial_epoch=0)
```

本函数用以训练模型，参数有：
- x：输入数据。如果模型只有一个输入，那么x的类型是numpy

array，如果模型有多个输入，那么x的类型应当为list，list的元素是对应于各个输入的numpy

array。如果模型的每个输入都有名字，则可以传入一个字典，将输入名与其输入数据对应起来。- 
y：标签，numpy array。如果模型有多个输出，可以传入一个numpy

array的list。如果模型的输出拥有名字，则可以传入一个字典，将输出名与其标签对应起来。- 
batch_size：整数，指定进行梯度下降时每个batch包含的样本数。训练时一个batch的样本会被计算一次梯度下降，使目标函数优化一步。- 
nb_epoch：整数，训练的轮数，训练数据将会被遍历nb_epoch次。Keras中nb开头的变量均为"number of"的意思- 
verbose：日志显示，0为不在标准输出流输出日志信息，1为输出进度条记录，2为每个epoch输出一行记录- 
callbacks：list，其中的元素是keras.callbacks.Callback的对象。这个list中的回调函数将会在训练过程中的适当时机被调用，参考回调函数- 
validation_split：0~1之间的浮点数，用来指定训练集的一定比例数据作为验证集。验证集将不参与训练，并在每个epoch结束后测试的模型的指标，如损失函数、精确度等。注意，validation_split的划分在shuffle之后，因此如果你的数据本身是有序的，需要先手工打乱再指定validation_split，否则可能会出现验证集样本不均匀。- 
validation_data：形式为（X，y）或（X，y，sample_weights）的tuple，是指定的验证集。此参数将覆盖validation_spilt。- 
shuffle：布尔值，表示是否在训练过程中每个epoch前随机打乱输入样本的顺序。- 
class_weight：字典，将不同的类别映射为不同的权值，该参数用来在训练过程中调整损失函数（只能用于训练）。该参数在处理非平衡的训练数据（某些类的训练样本数很少）时，可以使得损失函数对样本数不足的数据更加关注。- 
sample_weight：权值的numpy

array，用于在训练时调整损失函数（仅用于训练）。可以传递一个1D的与样本等长的向量用于对样本进行1对1的加权，或者在面对时序数据时，传递一个的形式为（samples，sequence_length）的矩阵来为每个时间步上的样本赋不同的权。这种情况下请确定在编译模型时添加了sample_weight_mode=‘temporal’。- 
initial_epoch: 从该参数指定的epoch开始训练，在继续之前的训练时有用。

输入数据与规定数据不匹配时会抛出错误

fit函数返回一个History的对象，其History.history属性记录了损失函数和其他指标的数值随epoch变化的情况，如果有验证集的话，也包含了验证集的这些指标变化情况4.evaluate，模型评估
```python

```
evaluate(self, x, y, batch_size=32, verbose=1, sample_weight=None)

本函数按batch计算在某些输入数据上模型的误差，其参数有：- 
x：输入数据，与fit一样，是numpy array或numpy array的list- 
y：标签，numpy array- 
batch_size：整数，含义同fit的同名参数- 
verbose：含义同fit的同名参数，但只能取0或1- 
sample_weight：numpy array，含义同fit的同名参数

本函数返回一个测试误差的标量值（如果模型没有其他评价指标），或一个标量的list（如果模型还有其他的评价指标）。model.metrics_names将给出list中各个值的含义。

如果没有特殊说明，以下函数的参数均保持与fit的同名参数相同的含义

如果没有特殊说明，以下函数的verbose参数（如果有）均只能取0或15.predict 模型预测
```python

```
predict(self, x, batch_size=32, verbose=0)

本函数按batch获得输入数据对应的输出，其参数有：

函数的返回值是预测值的numpy array模型检查 on_batch
```python

```
train_on_batch(self, x, y, class_weight=None, sample_weight=None)
test_on_batch(self, x, y, sample_weight=None)
predict_on_batch(self, x)

train_on_batch：本函数在一个batch的数据上进行一次参数更新，函数返回训练误差的标量值或标量值的list，与evaluate的情形相同。

test_on_batch：本函数在一个batch的样本上对模型进行评估，函数的返回与evaluate的情形相同；

predict_on_batch：本函数在一个batch的样本上对模型进行测试，函数返回模型在一个batch上的预测结果_generator
```python

```
fit_generator(self, generator, steps_per_epoch, epochs=1, verbose=1, callbacks=None, validation_data=None, validation_steps=None, class_weight=None, max_q_size=10, workers=1, pickle_safe=False, initial_epoch=0)
evaluate_generator(self, generator, steps, max_q_size=10, workers=1, pickle_safe=False)案例一：简单的单层-全连接网络
```python

```
from keras.layers import Input, Dense
from keras.models import Model

# This returns a tensor
inputs = Input(shape=(784,))# a layer instance is callable on a tensor, and returns a tensor
x = Dense(64, activation='relu')(inputs)# 输入inputs，输出x# (inputs)代表输入
x = Dense(64, activation='relu')(x)# 输入x，输出x
predictions = Dense(10, activation='softmax')(x)# 输入x，输出分类# This creates a model that includes# the Input layer and three Dense layers
model = Model(inputs=inputs, outputs=predictions)
model.compile(optimizer='rmsprop',
              loss='categorical_crossentropy',
              metrics=['accuracy'])
model.fit(data, labels)# starts training

其中：

可以看到结构与序贯模型完全不一样，其中x = Dense(64, activation=‘relu’)(inputs)中：(input)代表输入；x代表输出

model = Model(inputs=inputs, outputs=predictions)；该句是函数式模型的经典，可以同时输入两个input，然后输出output两个模型案例二：视频处理
```python

```
x = Input(shape=(784,))# This works, and returns the 10-way softmax we defined above.
y = model(x)# model里面存着权重，然后输入x，输出结果，用来作fine-tuning# 分类->视频、实时处理from keras.layers import TimeDistributed

# Input tensor for sequences of 20 timesteps,# each containing a 784-dimensional vector
input_sequences = Input(shape=(20,784))# 20个时间间隔，输入784维度的数据# This applies our previous model to every timestep in the input sequences.# the output of the previous model was a 10-way softmax,# so the output of the layer below will be a sequence of 20 vectors of size 10.
processed_sequences = TimeDistributed(model)(input_sequences)# Model是已经训练好的

其中：

Model是已经训练好的，现在用来做迁移学习；

其中还可以通过TimeDistributed来进行实时预测；

TimeDistributed(model)(input_sequences)，input_sequences代表序列输入；model代表已训练的模型案例三：双输入、双模型输出：LSTM 时序预测

本案例很好，可以了解到Model的精髓在于他的任意性，给编译者很多的便利。

输入：

新闻语料；新闻语料对应的时间

输出：

新闻语料的预测模型；新闻语料+对应时间的预测模型
![这里写图片描述](http://keras-cn.readthedocs.io/en/latest/images/multi-input-multi-output-graph.png)模型一：只针对新闻语料的LSTM模型
```python

```
from keras.layers import Input, Embedding, LSTM, Dense
from keras.models import Model

# Headline input: meant to receive sequences of 100 integers, between 1 and 10000.# Note that we can name any layer by passing it a "name" argument.
main_input = Input(shape=(100,), dtype='int32', name='main_input')# 一个100词的BOW序列# This embedding layer will encode the input sequence# into a sequence of dense 512-dimensional vectors.
x = Embedding(output_dim=512, input_dim=10000, input_length=100)(main_input)# Embedding层，把100维度再encode成512的句向量，10000指的是词典单词总数# A LSTM will transform the vector sequence into a single vector,# containing information about the entire sequence
lstm_out = LSTM(32)(x)# ？ 32什么意思？？？？？？？？？？？？？？？？？？？？？#然后，我们插入一个额外的损失，使得即使在主损失很高的情况下，LSTM和Embedding层也可以平滑的训练。

auxiliary_output = Dense(1, activation='sigmoid', name='aux_output')(lstm_out)#再然后，我们将LSTM与额外的输入数据串联起来组成输入，送入模型中：# 模型一：只针对以上的序列做的预测模型组合模型：新闻语料+时序
```python

```
# 模型二：组合模型
auxiliary_input = Input(shape=(5,), name='aux_input')# 新加入的一个Input,5维度
x = keras.layers.concatenate([lstm_out, auxiliary_input])# 组合起来，对应起来# We stack a deep densely-connected network on top# 组合模型的形式
x = Dense(64, activation='relu')(x)
x = Dense(64, activation='relu')(x)
x = Dense(64, activation='relu')(x)# And finally we add the main logistic regression layer
main_output = Dense(1, activation='sigmoid', name='main_output')(x)#最后，我们定义整个2输入，2输出的模型：
model = Model(inputs=[main_input, auxiliary_input], outputs=[main_output, auxiliary_output])#模型定义完毕，下一步编译模型。#我们给额外的损失赋0.2的权重。我们可以通过关键字参数loss_weights或loss来为不同的输出设置不同的损失函数或权值。#这两个参数均可为Python的列表或字典。这里我们给loss传递单个损失函数，这个损失函数会被应用于所有输出上。

其中：Model(inputs=[main_input, auxiliary_input], outputs=[main_output, auxiliary_output])是核心，

Input两个内容，outputs两个模型
```python

```
# 训练方式一：两个模型一个loss
model.compile(optimizer='rmsprop', loss='binary_crossentropy',
              loss_weights=[1.,0.2])#编译完成后，我们通过传递训练数据和目标值训练该模型：

model.fit([headline_data, additional_data],[labels, labels],
          epochs=50, batch_size=32)# 训练方式二：两个模型,两个Loss#因为我们输入和输出是被命名过的（在定义时传递了“name”参数），我们也可以用下面的方式编译和训练模型：
model.compile(optimizer='rmsprop',
              loss={'main_output':'binary_crossentropy','aux_output':'binary_crossentropy'},
              loss_weights={'main_output':1.,'aux_output':0.2})# And trained it via:
model.fit({'main_input': headline_data,'aux_input': additional_data},{'main_output': labels,'aux_output': labels},
          epochs=50, batch_size=32)

因为输入两个，输出两个模型，所以可以分为设置不同的模型训练参数案例四：共享层：对应关系、相似性

一个节点，分成两个分支出去
```python

```
import keras
from keras.layers import Input, LSTM, Dense
from keras.models import Model

tweet_a = Input(shape=(140,256))
tweet_b = Input(shape=(140,256))#若要对不同的输入共享同一层，就初始化该层一次，然后多次调用它# 140个单词，每个单词256维度，词向量# # This layer can take as input a matrix# and will return a vector of size 64
shared_lstm = LSTM(64)# 返回一个64规模的向量# When we reuse the same layer instance# multiple times, the weights of the layer# are also being reused# (it is effectively *the same* layer)
encoded_a = shared_lstm(tweet_a)
encoded_b = shared_lstm(tweet_b)# We can then concatenate the two vectors:# 连接两个结果# axis=-1？？？？？
merged_vector = keras.layers.concatenate([encoded_a, encoded_b], axis=-1)# And add a logistic regression on top
predictions = Dense(1, activation='sigmoid')(merged_vector)# 其中的1 代表什么？？？？# We define a trainable model linking the# tweet inputs to the predictions
model = Model(inputs=[tweet_a, tweet_b], outputs=predictions)

model.compile(optimizer='rmsprop',
              loss='binary_crossentropy',
              metrics=['accuracy'])
model.fit([data_a, data_b], labels, epochs=10)# 训练模型，然后预测案例五：抽取层节点内容
```python

```
# 1、单节点
a = Input(shape=(140,256))
lstm = LSTM(32)
encoded_a = lstm(a)assert lstm.output == encoded_a
# 抽取获得encoded_a的输出张量# 2、多节点
a = Input(shape=(140,256))
b = Input(shape=(140,256))

lstm = LSTM(32)
encoded_a = lstm(a)
encoded_b = lstm(b)assert lstm.get_output_at(0)== encoded_a
assert lstm.get_output_at(1)== encoded_b

# 3、图像层节点# 对于input_shape和output_shape也是一样，如果一个层只有一个节点，#或所有的节点都有相同的输入或输出shape，#那么input_shape和output_shape都是没有歧义的，并也只返回一个值。#但是，例如你把一个相同的Conv2D应用于一个大小为(3,32,32)的数据，#然后又将其应用于一个(3,64,64)的数据，那么此时该层就具有了多个输入和输出的shape，#你就需要显式的指定节点的下标，来表明你想取的是哪个了
a = Input(shape=(3,32,32))
b = Input(shape=(3,64,64))

conv = Conv2D(16,(3,3), padding='same')
conved_a = conv(a)# Only one input so far, the following will work:assert conv.input_shape ==(None,3,32,32)

conved_b = conv(b)# now the `.input_shape` property wouldn't work, but this does:assert conv.get_input_shape_at(0)==(None,3,32,32)assert conv.get_input_shape_at(1)==(None,3,64,64)案例六：视觉问答模型
```python

```
#这个模型将自然语言的问题和图片分别映射为特征向量，#将二者合并后训练一个logistic回归层，从一系列可能的回答中挑选一个。from keras.layers import Conv2D, MaxPooling2D, Flatten
from keras.layers import Input, LSTM, Embedding, Dense
from keras.models import Model, Sequential

# First, let's define a vision model using a Sequential model.# This model will encode an image into a vector.
vision_model = Sequential()
vision_model.add(Conv2D(64,(3,3) activation='relu', padding='same', input_shape=(3,224,224)))
vision_model.add(Conv2D(64,(3,3), activation='relu'))
vision_model.add(MaxPooling2D((2,2)))
vision_model.add(Conv2D(128,(3,3), activation='relu', padding='same'))
vision_model.add(Conv2D(128,(3,3), activation='relu'))
vision_model.add(MaxPooling2D((2,2)))
vision_model.add(Conv2D(256,(3,3), activation='relu', padding='same'))
vision_model.add(Conv2D(256,(3,3), activation='relu'))
vision_model.add(Conv2D(256,(3,3), activation='relu'))
vision_model.add(MaxPooling2D((2,2)))
vision_model.add(Flatten())# Now let's get a tensor with the output of our vision model:
image_input = Input(shape=(3,224,224))
encoded_image = vision_model(image_input)# Next, let's define a language model to encode the question into a vector.# Each question will be at most 100 word long,# and we will index words as integers from 1 to 9999.
question_input = Input(shape=(100,), dtype='int32')
embedded_question = Embedding(input_dim=10000, output_dim=256, input_length=100)(question_input)
encoded_question = LSTM(256)(embedded_question)# Let's concatenate the question vector and the image vector:
merged = keras.layers.concatenate([encoded_question, encoded_image])# And let's train a logistic regression over 1000 words on top:
output = Dense(1000, activation='softmax')(merged)# This is our final model:
vqa_model = Model(inputs=[image_input, question_input], outputs=output)# The next stage would be training this model on actual data.

.延伸一：fine-tuning时如何加载No_top的权重

如果你需要加载权重到不同的网络结构（有些层一样）中，例如fine-tune或transfer-learning，你可以通过层名字来加载模型：

model.load_weights(‘my_model_weights.h5’, by_name=True)

例如：

假如原模型为：``    model = Sequential()
    model.add(Dense(2, input_dim=3, name="dense_1"))
    model.add(Dense(3, name="dense_2"))
    ...
    model.save_weights(fname)
``# new model
model = Sequential()
model.add(Dense(2, input_dim=3, name="dense_1"))  # will be loaded
model.add(Dense(10, name="new_dense"))  # will not be loaded

# load weights from first model; will only affect the first layer, dense_1.
model.load_weights(fname, by_name=True)
延伸二：应对不均衡样本的情况

使用：class_weight,sample_weight

两者的区别为：- 


class_weight—主要针对的上数据不均衡问题，比如：异常检测的二项分类问题，异常数据仅占1%，正常数据占99%;   此时就要设置不同类对loss的影响。- 


sample_weight—主要解决的是样本质量不同的问题，比如前1000个样本的可信度，那么它的权重就要高，后1000个样本可能有错、不可信，那么权重就要调低。

class_weight的使用：``cw = {0: 1, 1: 50}
model.fit(x_train, y_train,batch_size=batch_size,epochs=epochs,verbose=1,callbacks=cbks,validation_data=(x_test, y_test), shuffle=True,class_weight=cw)


sample_weight的使用：

来源：[](https://www.kaggle.com/c/jigsaw-toxic-comment-classification-challenge/discussion/46673)https://www.kaggle.com/c/jigsaw-toxic-comment-classification-challenge/discussion/46673``from sklearn.utils import class_weight

list_classes = ["toxic", "severe_toxic", "obscene", "threat", "insult", "identity_hate"]
y = train[list_classes].values
sample_weights = class_weight.compute_sample_weight('balanced', y)

model.fit(X_t, y, batch_size=batch_size, epochs=epochs,validation_split=0.1,sample_weight=sample_weights, callbacks=callbacks_list)






















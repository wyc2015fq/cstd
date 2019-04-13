
# keras + tensorflow —— 函数式 API编程 - Zhang's Wikipedia - CSDN博客


2018年11月03日 12:18:41[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：327



## 1. 实现简单的逻辑回归
`from keras import Input
from keras import layers
from keras.models import Model
x = Input(shape=(32, ))
	# TensorShape([Dimension(None), Dimension(32)])
y = layers.Dense(16, activation='softmax')(x)
model = Model(x, y) 
	# model.output_shape
	# model.summary()`
## 2. 函数式编程的模型构建
序列化模型构建
seq_model = Sequential()
seq_model.add(layers.Dense(32, activation='relu', input_shape=(64, )))
	\# 首层必须指定 input_shape 或 batch_input_shape
seq_model.add(layers.Dense(32, activation='relu'))
seq_model.add(layers.Dense(10, activation='softmax'))
函数式API 下的模型构建
input_tensor = Input(shape=(64, ))
z = layers.Dense(32, activation='relu')(input_tensor)
z = layers.Dense(32, activation='relu')(z)
y = layers.Dense(10, activation='softmax')(z)
from keras.models import Model
model = Model(input_tensor, y)
两种方式对比：
seq_model.summary()
model.summary()
## 3. layers.concatenate 与 layers.add
layers.add：执行 tensor 的相加操作，要求输入必须同维度（或者经过 broadcast 之后是同维度）；
x_1 = Input(shape=(32, ))
x_2 = Input(shape=(32, ))
>> layers.add([x_1, x_2])
<tf.Tensor 'add_3/add:0' shape=(?, 32) dtype=float32>
x_1 = Input(shape=(32, 64))
x_2 = Input(shape=(64,))
>> layers.add([x_1, x_2])
<tf.Tensor 'add_4/add:0' shape=(?, 32, 64) dtype=float32>
layers.concatente：执行的是 tensor 的拼接操作（维度会拉长）
x_1 = Input(shape=(32, ))
x_2 = Input(shape=(64, ))
>> layers.concatenate([x_1, x_2])
<tf.Tensor 'concatenate_3/concat:0' shape=(?, 96) dtype=float32>


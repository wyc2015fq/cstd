# Keras多GPU训练以及载入权重无效的问题 - DumpDoctorWang的博客 - CSDN博客





2018年11月15日 11:09:55[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：454











### 目录
- [1、数据并行](#1_3)
- [1.1、单GPU或者无GPU训练的代码](#11GPUGPU_4)
- [1.2、数据并行的多GPU 训练](#12GPU__37)
- [2、设备并行](#2_78)
- [参考链接](#_109)



本文讲简单的探讨Keras中使用多GPU训练的方法以及需要注意的地方。有两种方法可在多个 GPU 上运行单个模型：数据并行和设备并行（Keras官方建议使用 TensorFlow 后端）。第一部分讲如何使用数据并行的方式使用多GPU加速；第二部分给出一个设备并行的例子。


# 1、数据并行

## 1.1、单GPU或者无GPU训练的代码

```python
# 定义数据集
train_dataset, train_steps = ...
val_dataset, val_steps = ...
# 定义一个模型
model = ...
model.summary()

# 编译模型。因为是多类分类问题，所以loss是categorical_crossentropy
# lr一定不要太高，不然训练的精度有时能上去，有时上不去
model.compile(optimizer=keras.optimizers.Adam(lr=0.0001, decay=1e-5),
              loss=keras.losses.categorical_crossentropy,
              metrics=[keras.metrics.categorical_accuracy])

# 设置回调
callbacks = [
    # 把TensorBoard日志写到'logs'里面
    keras.callbacks.TensorBoard(log_dir='./logs'),
    # 当categorical_accuracy，也就是分类精度在10个epoh之内都没提升时，降低learning rate
    keras.callbacks.ReduceLROnPlateau(monitor='categorical_accuracy', patience=10, verbose=2),
    # 当categorical_accuracy在15个epoch内没有提升的时候，停止训练
    keras.callbacks.EarlyStopping(monitor='categorical_accuracy', patience=15, verbose=2)
]

# 开始训练
model.fit(train_dataset, epochs=NUM_TRAIN_EPOCH, steps_per_epoch=train_steps,
          validation_data=val_dataset, validation_steps=val_steps, callbacks=callbacks)

# 保存模型的权重
model.save_weights('model/latest.weights.h5', save_format='h5')
```

## 1.2、数据并行的多GPU 训练

数据并行包括在每个设备上复制一次目标模型，并使用每个模型副本处理不同部分的输入数据。Keras 有一个内置的实用函数 keras.utils.multi_gpu_model，它可以生成任何模型的数据并行版本，在多达 8 个 GPU 上实现准线性加速。数据并行的意思是，把模型复制，然后把一批（Batch）数据分为同等大小的几个子批在不同的GPU上训练，然后再在CPU上合并结果。

下面给出1.1中对应的使用多GPU训练的代码：

```python
# 定义数据集
train_dataset, train_steps = ...
val_dataset, val_steps = ...
# 定义一个模型
model = ...
model.summary()

# 获取数据并行的模型，其中2是GPU的数量。
parallel_model = keras.utils.multi_gpu_model(model, 2)

# 编译模型。因为是多类分类问题，所以loss是categorical_crossentropy
# lr一定不要太高，不然训练的精度有时能上去，有时上不去
parallel_model.compile(optimizer=keras.optimizers.Adam(lr=0.0001, decay=1e-5),
              loss=keras.losses.categorical_crossentropy,
              metrics=[keras.metrics.categorical_accuracy])

# 设置回调
callbacks = [
    # 把TensorBoard日志写到'logs'里面
    keras.callbacks.TensorBoard(log_dir='./logs'),
    # 当categorical_accuracy，也就是分类精度在10个epoh之内都没提升时，降低learning rate
    keras.callbacks.ReduceLROnPlateau(monitor='categorical_accuracy', patience=10, verbose=2),
    # 当categorical_accuracy在15个epoch内没有提升的时候，停止训练
    keras.callbacks.EarlyStopping(monitor='categorical_accuracy', patience=15, verbose=2)
]

# 开始训练
parallel_model.fit(train_dataset, epochs=NUM_TRAIN_EPOCH, steps_per_epoch=train_steps,
          validation_data=val_dataset, validation_steps=val_steps, callbacks=callbacks)

# 保存模型的权重
model.save_weights('model/latest.weights.h5', save_format='h5')
```

**注意：保存权重时，一定要使用model，也就是不是并行的模型，来保存权重。** 如果使用parallel_model.save_weights保存权重，在使用model.load_weights载入权重时，会出现类似于如下错误：
`ValueError: You are trying to load a weight file containing 1 layers into a model with 4 layers.`

在model.load_weights加上by_name=True时，倒是不会报错，**但是载入的权重是无效的**，可以自行测试。
# 2、设备并行

设备并行性包括在不同设备上运行同一模型的不同部分。对于具有并行体系结构的模型，例如有两个分支的模型，这种方式很合适。这种并行可以通过使用 tf.device来实现。一个简单的例子：

```python
# 模型中共享的 LSTM 用于并行编码两个不同的序列
input_a = keras.Input(shape=(140, 256))
input_b = keras.Input(shape=(140, 256))

shared_lstm = keras.layers.LSTM(64)

# 在一个 GPU 上处理第一个序列
with tf.device('/device:GPU:0'):
    encoded_a = shared_lstm(input_a)
# 在另一个 GPU上 处理下一个序列
with tf.device('/device:GPU:1'):
    encoded_b = shared_lstm(input_b)

# 在 CPU 上连接结果
with tf.device('/cpu:0'):
    merged_vector = keras.layers.concatenate([encoded_a, encoded_b], axis=-1)

# 整个全连接层
fc1 = keras.layers.Dense(50, activation='softmax')(merged_vector)

# 新建模型
model = keras.Model(inputs=[input_a, input_b], outputs=fc1)
# 其他代码
...
```

# 参考链接
- [如何在多 GPU 上运行 Keras 模型?](https://keras.io/zh/getting-started/faq/#how-can-i-run-a-keras-model-on-multiple-gpus)
- [Using GPUs](https://www.tensorflow.org/guide/using_gpu)





# keras上手系列之: 模型的保存 - happyhorizon的算法天空 - CSDN博客
2017年11月21日 11:20:54[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：3668
如何将训练好的网络进行保存以便以后使用, 进行后续的研究呢? 
首先,定义一个简单的LSTM模型:
```python
from keras.models import Sequential
from keras.layers import LSTM, Dense
model = Sequential()
model.add(LSTM(4,input_shape=(1,8)))
model.add(Dense(1))
```
### 整体保存模型及参数
首先,安装python的h5py包. 
`sudo pip3 install h5py`
之后调用model.save(filepath)将Keras模型和权重保存在一个HDF5文件中，该文件将包含： 
- 模型的结构，以便重构该模型 
- 模型的权重 
- 训练配置（损失函数，优化器等） 
- 优化器的状态，以便于从上次训练中断的地方开始 
使用`keras.models.load_model(filepath)`来重新实例化之前训练好的模型，如果文件中存储了训练配置的话，该函数还会同时完成模型的编译
```python
from keras.models import load_model
model.save('my_model.h5')  # creates a HDF5 file 'my_model.h5'
del model  # deletes the existing model
# returns a compiled model identical to the previous one
model = load_model('my_model.h5')
```
### 只保存模型的结构
可以用model.to_jason()将模型序列化保存为json文件.
```
# save as JSON
json_string = model.to_json()
```
例如上面LSTM网络的json_string就是:
```
json_string
Out[10]: '{"class_name": "Sequential", "config": [{"class_name": "LSTM", "config": {"name": "lstm_1", "trainable": true, "batch_input_shape": [null, 1, 8], "dtype": "float32", "return_sequences": false, "return_state": false, "go_backwards": false, "stateful": false, "unroll": false, "implementation": 0, "units": 4, "activation": "tanh", "recurrent_activation": "hard_sigmoid", "use_bias": true, "kernel_initializer": {"class_name": "VarianceScaling", "config": {"scale": 1.0, "mode": "fan_avg", "distribution": "uniform", "seed": null}}, "recurrent_initializer": {"class_name": "Orthogonal", "config": {"gain": 1.0, "seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "unit_forget_bias": true, "kernel_regularizer": null, "recurrent_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "recurrent_constraint": null, "bias_constraint": null, "dropout": 0.0, "recurrent_dropout": 0.0}}, {"class_name": "Dense", "config": {"name": "dense_1", "trainable": true, "units": 1, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "VarianceScaling", "config": {"scale": 1.0, "mode": "fan_avg", "distribution": "uniform", "seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}], "keras_version": "2.0.8", "backend": "tensorflow"}'
```
里面记录了网络的整体结构, 各个层的参数设置等信息.  将json字符串保存到文件.
`open('my_model_architecture.json','w').write(json_string)`
当然，你也可以从保存好的json文件或yaml文件中载入模型：
```
# 读取json文件
from keras.models import model_from_json
json_string = open('my_model_architecture.json').read()
model = model_from_json(json_string)
```
除了json格式,还可以保存为yaml格式的字符串:
```python
# save as YAML
yaml_string = model.to_yaml()
# 类似地,读取yaml文件
from keras.models import model_from_yaml
model = model_from_yaml(yaml_string)
```
### 保存模型权重等配置信息
经过调参后网络的输出精度比较满意后,可以将训练好的网络权重参数保存下来. 
可通过下面的代码利用HDF5进行保存 
model.save_weights(‘my_model_weights.h5’)
以后用的时候可以像这样加载模型: 
model.load_weights(‘my_model_weights.h5’)
如果你需要加载权重到不同的网络结构（有些层一样）中，例如fine-tune或transfer-learning，你可以通过层名字来加载模型： 
`model.load_weights('my_model_weights.h5', by_name=True)`
首先在建模时,最好对每一层都指定名字, 例如：
```
# 定义模型
model = Sequential()
model.add(LSTM(4, input_shape=(1, 8), name="lstm_old"))
model.add(Dense(1, name="dense_old"))
...
model.save_weights('my_model_weights.h5')
# 新模型, 重载了前一个模型训练好的LSTM层
model_new = Sequential()
model_new.add(LSTM(4, input_shape=(1, 8), name="lstm_old"))  # will be loaded
model_new.add(Dense(10, name="dense_new"))  # will not be loaded
# 载入LSTM层训练好的参数
model.load_weights('my_model_weights.h5', by_name=True)
```

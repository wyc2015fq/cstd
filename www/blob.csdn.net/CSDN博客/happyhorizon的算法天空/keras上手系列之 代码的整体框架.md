# keras上手系列之: 代码的整体框架 - happyhorizon的算法天空 - CSDN博客
2017年11月22日 17:59:40[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：1161
keras的名字来源于希腊史诗<奥德赛>里的牛角之门(Gate of Horn), 是追梦者之路,是梦想实现之门. Those that come through the Ivory Gate cheat us with empty promises that never see fullfillment. Those that come through the Gate of Horn inform the dreamer of the truth.  
不得不说keras的作者真的太有文化了. 
当然,keras正如名字所寓意的一样,非常好用. 在最新的tensorflow发布版本中, 已经将keras列入官方支持的高级API之一.  
keras支持python语言,后台支持CNTK, theono和tensorflow三种深度学习框架. keras在设计时采用了高度模块化的方法, 强调快速建模, 实战中也的确如此. 下面就对keras开发的大致流程做一介绍. 
**1、载入数据**
keras支持numpy的float数据类型的array. 通常可以用pandas的load_csv读取数据, 并转化为支持的格式.
```
import pandas
dataset = pandas.load_csv(filename, some_conditions)
dataset = dataset.values()
dataset = dataset.astype('float32')
```
**2、归一化**
数据归一化是神经网络模型非常重要的一步. 可以调用sklearn.preprocessing中的MinMaxScaler将数据区间转化到[0,1]. 
```bash
from sklearn.preprocessing import MinMaxScaler
scaler = MinMaxScaler(feature_range=(0,1))
dataset = scaler.fit_transform(dataset)
```
之后调用`scaler.inverse_transform(dataset)` 就可以将数据恢复到原区间. 这里需要注意的是, dataset的维度不能发生改变. 也就是fit_transform()和 inverse_transform()处理的数据矩阵是同样大小的.
**3、将数据分为训练数据和验证数据，便于交叉验证**
```
train_size = int(len(dataset)*ratio)
test_size = len(dataset) - train_size
train_data, test_data = dataset[0:train_size,:], dataset[train_size:len(dataset), :]
```
**4、 将数据规范化，生成LSTM网络适合的数据**
keras的LSTM网络要求的数据的维度依次为： 
data[samples, timesteps, data_dim_at_each_timestep] 
或者说是 
data[samples, time steps, features]
**5、构建神经网络**
keras提供了两种方式构建神经网络, 一种是序贯模型, 一种是函数式模型. 
序贯模型: `from keras.models import Sequential`
函数式模型: `from keras.models import Model`
通常, 序贯模型比函数式模型建模更为简单, 但是函数式建模则更加灵活, 可以构建相对复杂的神经网络模型. 
以下是一个序贯模型的方式编写的简单LSTM网络: 
```python
from keras.models import Sequential
from keras.layers import LSTM, Dense
model = Sequential()
model.add(LSTM(layer_output_dim, input_shape=(timesteps, features)))
model.add(Dense(output_dim))
model.compile(loss='mse', optimizer='adam')
```
查看网络的结构:
```
model_config = model.config()
print('model config: ', model_config)
```
或者用summary列表的方式显示模型的整体结构:
```
model_summary = model.summary()
print(model_summary)
```
**6, 训练网络**
`model.fit(trainX, trainY, epochs=100, batch_size=1, verbose=0/1/2) # verbose显示迭代步骤，0是不显示`
**7. 模型输出预测**
`predict_y = model.predict(predict_X)`

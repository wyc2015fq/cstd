# keras上手系列之:序列到序列预测问题 - happyhorizon的算法天空 - CSDN博客
2017年11月26日 12:38:48[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：1159
# LSTM序列到序列模型种类
LSTM 序列到序列(seq to seq)问题建模, 根据问题和数据本身的特点,可以分为几种不同: 
 - 一对一(one to one) 
 - 多对一(many to one) 
 - 一对多(one to many) 
 - 多对多(many to many)
# 一个时间步预测
**(1) 一对一(one to one)**
这种模型是根据过去的一个时间点上的数据,预测下一个时间点的数据. 典型的问题结构是: 
y(t+1) = f(x(t)) 
对应的LSTM网络结构是: 
![这里写图片描述](https://img-blog.csdn.net/20171126123159415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
按照timestep展开后, 网络结构如下: 
![这里写图片描述](https://img-blog.csdn.net/20171126123236840?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
一对一模型用keras建模实现如下:
```
model = Sequential()
model.add(LSTM(..., input_shape=(1, features)))
model.add(Dense(1))
```
值得注意的是, 数据本身可以是一个向量, 也就是说, 在一个时间点上, 对应的数据本身是多维的. 
整个网络的输入是1个长度为features的向量， 输出是标量。 
**(2) 多对一(many to one)**
此时,网络输入的时间步是大于1的, 但是输出的预测时间步只是1.  
y(t+1) = f(x(t-n), x(t-n+1), …, x(t)) 
此时的网络结构为: 
![这里写图片描述](https://img-blog.csdn.net/20171126123420107?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
model = Sequential()
features = 10
vectors = 3
model.add(LSTM(8, input_shape=(vectors, features)))
model.add(Dense(1))
print(model.summary())
```
可以得到：
```
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
lstm_27 (LSTM)               (None, 8)                 608       
_________________________________________________________________
dense_22 (Dense)             (None, 1)                 9         
=================================================================
Total params: 617
Trainable params: 617
Non-trainable params: 0
_________________________________________________________________
None
```
# 多个时间步预测
**TimeDistributed()包装器**
要输出对未来多个时间步上的预测, 就必须使用TimeDistributed()包装器, 将输出上一层网络的输出在时间轴上展开. 关于TimeDistributed()包装器, F. Chollet(Keras的作者)解释说： 
TimeDistributedDense将同样的密集(全连接)操作应用到3D张量的每一个时间间隔上。 
TimeDistributedDense applies a same Dense (fully-connected) operation to every timestep of a 3D tensor.
这一点可以用一个简单的例子说明. 例如如下的单层LSTM网络:
```
model = Sequential()
model.add(LSTM(1, input_shape=(10,1)))
print(model.summary())
```
得到:
```
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
lstm_15 (LSTM)               (None, 1)                 12        
=================================================================
Total params: 12
Trainable params: 12
Non-trainable params: 0
_________________________________________________________________
None
```
另外一个单层LSTM网络外面还增加了TimeDistributed()包装器.
```python
model = Sequential()
model.add(TimeDistributed(LSTM(1, input_shape=(None, 10, 1))))
print(model.summary())
```
得到:
```
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
time_distributed_14 (TimeDis (None, None, 1)           12        
=================================================================
Total params: 12
Trainable params: 12
Non-trainable params: 0
_________________________________________________________________
None
```
得到的网络参数都是12. 可见, TimeDistributed()包装器没有引入新的参数, 而是将同样的网络分别在batch中的每个样本上都应用了.正如这个包装器的命名，是将训练数据的batch视为在时间轴上的展开. 这一点上可以从Output Shape中看出来。 对于第一个单层LSTM网络，输出的数据维度就是[batch, 1], 第二个LSTM网络的输出是[batch, batch, 1]. 
有了对TimeDistributed()包装器的基本认识，我们就可以理解下面的多时间步预测问题。
# 多时间步上的预测
**(3) 一对多(one to many)**
这种问题的典型结构是: 
y(t+1), y(t+2) = f(x(t)) 
![这里写图片描述](https://img-blog.csdn.net/20171126123549984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
也就是说, 根据过去一步的数据, 预测未来两步的数据. 用keras建模应该是:
```
model = Sequential()
features = 8
model.add(LSTM(out_dim, input_shape(1, features), return_sequences=True))
model.add(TimeDistributed(Dense(1)))
```
这里TimeDistributed() 包装器将第一层LSTM的输出序列按照时间顺序排列好. 例如上面LSTM网络的输入数据是[batch, 1, features], 那么输出是: [batch, 1, 20] 
在Dense层，增加了TimeDistributed()包装器，可以得到输出的维数是：[batch, 1, 1] 
这个模型的结构和输出数据的维数是：
```
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
lstm_18 (LSTM)               (None, 1, 20)             2320      
_________________________________________________________________
time_distributed_17 (TimeDis (None, 1, 1)              21        
=================================================================
Total params: 2,341
Trainable params: 2,341
Non-trainable params: 0
_________________________________________________________________
None
output_shape is : (None, 1, 1)
```
网络的输入是1个长度是8维的向量，输出是标量。
**(4) 多对多(many to many)**
多对多问题的典型结构是： 
y(t+1), y(t+2), …, y(t+n) = f(y(t-n), y(t-n+1), …, y(t)) 
![这里写图片描述](https://img-blog.csdn.net/20171126123714031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
用keras建模：
```python
from keras.models import Sequential
from keras.layers import LSTM, Dense, TimeDistributed
model = Sequential()
in_features = 8
n_in = 10
out_dim = 6
out_features = 12
model.add(LSTM(out_dim, input_shape=(n_in, in_features), return_sequences=True))
model.add(TimeDistributed(Dense(out_features)))
print(model.summary())
```
得到：
```
Using TensorFlow backend.
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
lstm_1 (LSTM)                (None, 10, 6)             360       
_________________________________________________________________
time_distributed_1 (TimeDist (None, 10, 12)            84        
=================================================================
Total params: 444
Trainable params: 444
Non-trainable params: 0
_________________________________________________________________
None
```
网络的输入是10个长度为8的向量，输出是长度是12的向量。
如果输入序列的长度与输出序列不一致, 那么需要在网络中增加RepeatVector()层.
```
from keras.layers import RepeatVector
model = Sequential()
n_in = 120
n_out = 60
model.add(LSTM(60, input_shape=(n_in, 1)))
model.add(RepeatVector(n_out))
model.add(LSTM(20, return_sequences=True))
model.add(TimeDistributed(Dense(1)))
model.compile(loss='mse',  optimizer='adam')
print(model.summary())
```
得到:
```
_________________________________________________________________
Layer (type)                 Output Shape              Param #   
=================================================================
lstm_4 (LSTM)                (None, 60)                14880     
_________________________________________________________________
repeat_vector_1 (RepeatVecto (None, 60, 60)            0         
_________________________________________________________________
lstm_5 (LSTM)                (None, 60, 20)            6480      
_________________________________________________________________
time_distributed_2 (TimeDist (None, 60, 1)             21        
=================================================================
Total params: 21,381
Trainable params: 21,381
Non-trainable params: 0
_________________________________________________________________
None
```

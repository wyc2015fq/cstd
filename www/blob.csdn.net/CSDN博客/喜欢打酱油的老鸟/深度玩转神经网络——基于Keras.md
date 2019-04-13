
# 深度玩转神经网络——基于Keras - 喜欢打酱油的老鸟 - CSDN博客


2019年01月07日 08:00:56[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：70


[https://www.toutiao.com/a6641550966929752579/](https://www.toutiao.com/a6641550966929752579/)
2019-01-01 23:29:46
随着AI人工智能的发展，深度学习也被越来越多的量化研究员应用到量化投资领域中，不管是简单粗暴地随着AI人工智能的发展，深度学习也被越来越多的量化研究员应用到量化投资领域中，不管是简单粗暴地直接预测股票价格的变化，或者是预测未来的财务指标、新闻舆情分析等，深度学习都在发挥着它举足轻重的作用，越来越多的量化私募在路演的时候也都会介绍自己的深度学习策略，在各个论坛上，关于深度学习在量化投资上的应用的讨论也十分热烈。
作为公众号2019年的第一篇推文，我们就来展示一下各种可以应用于量化投资的神经网络模型，也祝各位读者元旦快乐，19年净值翻倍。
Keras中文文档： https://keras.io/zh/
# Example1
首先是一个很常规的3层神经网络，我们可以输入股票的基本面的选股因子，输出未来某一段时间股票获得正alpha的概率。
```python
import pandas as pd 
import numpy as np 
from keras import layers,Model
X = np.random.rand(10000,30) # 10000个样本,30个特征
y = np.random.choice([0,1],size=10000)
Input = layers.Input(shape=(30,),name='Example1_input')
dense1 = layers.Dense(60,activation='relu',name='Example1_dense1')(Input)
dense2 = layers.Dense(60,activation='relu',name='Example1_dense2')(dense1)
Output = layers.Dense(1,activation='sigmoid',name='Example1_output')(dense2)
model = Model(inputs=Input,outputs=Output)
model.compile(optimizer='sgd',loss='binary_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
print(model.summary())
```
![深度玩转神经网络——基于Keras](http://p1.pstatp.com/large/pgc-image/d1aaab73864f48e4892615e43a4fb8fc)
**dense1**: 60个节点，每个节点接受30个特征的输入以及自身的偏置值，故有60*(30+1)=1860个参数;
**dense2**: 60个节点，每个节点接受dense1的60个输出以及自身的偏置值，故有60*(60+1)=3660个参数;
**output**: 1个节点，接受dense2的60个输出以及自身的偏置值，故有1*(60+1)=61个参数。
# Example2
这是一个简单的RNN，很多人把循环神经网络看成是非线性的arima，其实也没错，但是我们使用arima的时候，大多数都是处理单个时间序列，而RNN本质上跟其他机器学习一样，也是用来处理回归和分类问题。例如在情感分析中，输入一段文本，输出文本的情感分类，而训练数据是很多条文本以及文本的标签。在量化投资中，可以输入过去n天各种技术指标的时间序列数据，例如MACD,KDJ,RSI等，输出未来某一段时间股票获得正alpha的概率。
```python
X = np.random.rand(10000,60,30) # 10000个样本,60个时间点,30个特征
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input = layers.Input(shape=(60,30,),name='Example2_input') # 输入60天的30个技术指标
rnn1 = layers.SimpleRNN(50,return_sequences=True,name='Example2_rnn1')(Input)
rnn2 = layers.SimpleRNN(50,name='Example2_rnn2')(rnn1)
Output = layers.Dense(3,activation='softmax',name='Example2_output')(rnn2)
model = Model(inputs=Input,outputs=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
print(model.summary())
```
![深度玩转神经网络——基于Keras](http://p99.pstatp.com/large/pgc-image/9f62d76c902e438b8802ef3ef60dd7e2)
**rnn1**: 50个节点，接受输入层30个特征、自身t-1隐层的输出，以及自身的偏置值，故有(30+50+1)*50=4050个参数
**rnn2**: 50个节点，接受rnn1的50个隐层的输出、自身t-1隐层的输出，以及自身的偏置值，故有(50+50+1)*50=5050个参数
**output**: 3个节点，接受rnn2的50个输出以及自身的偏置值，故有3*(50+1)=153个参数。
# Example3
如果我们认为基本面和技术面不能简单地混合一起用，我们将Example1和Example2结合起来，基本面的特征使用普通的全连接网络，技术面的特征使用时序模型RNN，然后再将它们的输出合并起来，再输入到输出层。
```python
X1 = np.random.rand(10000,50) # 10000个样本,50个特征
X2 = np.random.rand(10000,60,30) # 10000个样本,60个时间点,30个特征
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input1 = layers.Input(shape=(50,),name='Example3_input1')
dense1 = layers.Dense(100,activation='relu',name='Example3_dense1')(Input1)
dense2 = layers.Dense(100,activation='relu',name='Example3_dense2')(dense1)
Input2 = layers.Input(shape=(60,30,),name='Example3_input2')
rnn1 = layers.SimpleRNN(60,return_sequences=True,name='Example3_rnn1')(Input2)
rnn2 = layers.SimpleRNN(60,name='Example3_rnn2')(rnn1)
concat = layers.Concatenate(axis=-1,name='Example3_concat')([dense2,rnn2])
Output = layers.Dense(3,activation='softmax',name='output')(concat)
model = Model(inputs=[Input1,Input2],outputs=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit([X1,X2],y,batch_size=200,epochs=10)
```
网络的结构如下：
![深度玩转神经网络——基于Keras](http://p99.pstatp.com/large/pgc-image/8f5ffab5150640f6832f7ed804d83a64)
![深度玩转神经网络——基于Keras](http://p99.pstatp.com/large/pgc-image/c75f10f0e005423f82ba8b0069bc29d2)
**dense1**: 100个节点，接受input1的50个特征，以及自身的偏置值，故有(50+1)*100=5100个参数;
**rnn1**: 50个节点，接受input2的30个特征，以及自身t-1时刻的50个隐层输出，故有(30+50+1)*50=4050个参数;
**dense2**: 100个节点，接受dense1的100个隐层输出，以及自身的偏置值，故有(100+1)*100=10100个参数;
**rnn2**: 50个节点，接受rnn1的50个隐层的输出、自身t-1隐层的输出，以及自身的偏置值，故有(50+50+1)*50=5050个参数;
**output**: 3个节点，接受dense2的100个隐层的输出、rnn2的50个隐层的输出，以及自身的偏置值，故有(100+50+1)*3=453个参数。
# Example4
事实上我们较少使用的简单的RNN，使用LSTM和GRU比较多。另外在搭建网络时，我们一般在每一层网络的输入上做BatchNormalization和Dropout，Example4为Example2的改进。
关于BatchNormalization和Dropout可以看我们的之前的推文：
深度学习科普及其在多因子选股中的应用
```python
X = np.random.rand(10000,60,30) # 10000个样本,60个时间点,30个特征
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input = layers.Input(shape=(60,30,),name='Example4_input')
BN_Input = layers.BatchNormalization(name='Example4_BN_input')(Input)
Dropout_Input = layers.Dropout(0.5,name='Example4_Dropout_input')(BN_Input)
lstm1 = layers.LSTM(50,dropout=0.3,return_sequences=True,name='Example4_lstm1')(Dropout_Input)
BN_lstm1 = layers.BatchNormalization(name='Example4_BN_lstm1')(lstm1)
Dropout_lstm1 = layers.Dropout(0.5,name='Example4_Dropout_lstm1')(BN_lstm1)
lstm2 = layers.LSTM(50,dropout=0.3,name='Example4_lstm2')(Dropout_lstm1)
BN_lstm2 = layers.BatchNormalization(name='Example4_BN_lstm2')(lstm2)
Dropout_lstm2 = layers.Dropout(0.5,name='Example4_Dropout_lstm2')(BN_lstm2)
Output = layers.Dense(3,activation='softmax',name='Example4_output')(Dropout_lstm2)
model = Model(inputs=Input,outputs=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
print(model.summary())
```
![深度玩转神经网络——基于Keras](http://p99.pstatp.com/large/pgc-image/77ca1dd0f0e045468ad60b10ff42beeb)
**BN_input**: 接受30个特征输入，每一个特征都需要求出样本的均值，方差，然后要给出γ和β，故有30*2=60个无需训练的参数和30*2=60个需要训练的参数。总共120个参数；
**lstm1**: 50个隐层节点，每一个节点接受30个特征输入和50个t-1隐层输出以及偏置值，每一个隐层节点，都对应着输入门，遗忘门和输出门，它们也都有30个特征输入和50个t-1隐层输出以及偏置值，跟rnn相比，多出了三个门的参数，参数的个数变成了原来的4倍。故参数的个数总共有(30+50+1)*50*4=16200;
**BN_lstm1**: 接受lstm1的50个隐层节点的输出，故有50*2=100个无需训练的参数和50*2=100个需要训练的参数。总共200个参数;
**lstm2**: 50个隐层节点，还有对应的输入门，遗忘门，输出门，都接受lstm1的50个隐层输出、自身50个t-1隐层输出以及偏置值，故参数的个数总共有(50+50+1)*50*4=20200;
**BN_lstm2**: 接受lstm2的50个隐层节点的输出，故有50*2=100个无需训练的参数和50*2=100个需要训练的参数。总共200个参数;
**output**: 3个节点，接受lstm2的50个隐层节点的输出以及自身的偏置值，故有(50+1)*3=153个参数。
# Example5
将Example4中BatchNormalization和Dropout应用到Example3中，再将Example3中SimpleRNN换成LSTM。
```python
X1 = np.random.rand(10000,50) # 10000个样本,50个特征
X2 = np.random.rand(10000,60,30) # 10000个样本,60个时间点,30个特征
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input1 = layers.Input(shape=(50,),name='Example5_input1')
BN_Input1 = layers.BatchNormalization(name='Example5_BN_input1')(Input1)
Dropout_Input1 = layers.Dropout(0.5,name='Example5_Dropout_input1')(BN_Input1)
dense1 = layers.Dense(100,activation='relu',name='Example5_dense1')(Dropout_Input1)
BN_dense1 = layers.BatchNormalization(name='Example5_BN_dense1')(dense1)
Dropout_dense1 = layers.Dropout(0.5,name='Example5_Dropout_dense1')(BN_dense1)
dense2 = layers.Dense(100,activation='relu',name='Example5_dense2')(Dropout_dense1)
Input2 = layers.Input(shape=(60,30,),name='Example5_input2')
BN_Input2 = layers.BatchNormalization(name='Example5_BN_input2')(Input2)
Dropout_Input2 = layers.Dropout(0.5,name='Example5_Dropout_input2')(BN_Input2)
rnn1 = layers.SimpleRNN(60,dropout=0.3,return_sequences=True,name='Example5_rnn1')(Dropout_Input2)
BN_rnn1 = layers.BatchNormalization(name='Example5_BN_rnn1')(rnn1)
Dropout_rnn1 = layers.Dropout(0.5,name='Example5_Dropout_rnn1')(BN_rnn1)
rnn2 = layers.SimpleRNN(60,name='Example5_rnn2')(Dropout_rnn1)
concat = layers.Concatenate(axis=-1,name='Example5_concat')([dense2,rnn2])
BN_concat = layers.BatchNormalization(name='Example5_BN_concat')(concat)
Dropout_concat = layers.Dropout(0.5,name='Example5_Dropout_concat')(BN_concat)
Output = layers.Dense(3,activation='softmax',name='output')(Dropout_concat)
model = Model(inputs=[Input1,Input2],outputs=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit([X1,X2],y,batch_size=200,epochs=10)
print(model.summary())
```
参考Example3和Example4就可以将参数数清楚，这里不再累赘。
每一个神经网络模型，理论上都应该使用BatchNormalization，Dropout，下面的例子为了代码的简洁，将不再出现
BatchNormalization，Dropout。
# Example6
一般我们使用全接连网络训练基本面因子，使用LSTM训练技术面指标；如果你想输入最近几个交易日的分钟行情数据，交易日的数据是属于时间序列数据，而在一个交易日中，分钟行情数据又是时间序列数据，那么我们使用如下的网络进行训练。
其中TimeDistributed给与了模型一对一，多对多的能力，增加了模型的维度。
```python
X = np.random.rand(10000,10,48,5) #10000个样本,10个交易,48根5分钟k线,5个特征
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input = layers.Input(shape=(10,48,5,))
TD1 = layers.TimeDistributed(layers.LSTM(8,return_sequences=True),name='Example6_TD1')(Input)
TD2 = layers.TimeDistributed(layers.LSTM(8),name='Example6_TD2')(TD1)
lstm1 = layers.LSTM(12,return_sequences=True,name='Example6_lstm1')(TD2)
lstm2 = layers.LSTM(6,name='Example6_lstm2')(lstm1)
Output = layers.Dense(3,activation='softmax',name='Example_output')(lstm2)
model = Model(inputs=Input,outputs=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
print(model.summary())
```
![深度玩转神经网络——基于Keras](http://p3.pstatp.com/large/pgc-image/578a48d293274134ab093dabca0d4f34)
**TD1**: 10个交易日均使用同一个lstm，这个lstm有8个隐层节点，输入的时间长度是48，接受5个特征以及t-1隐层的输出，还有一个偏置值，参数的个数是(5+8+1)*8*4=448;
**TD2**: 8个隐层节点，TD1中lstm的8个隐层节点的输出，以及自身t-1隐层的输出，还有一个偏置值，参数的个数是(8+8+1)*8*4=544;
**lstm1**: 12个节点， 10个交易的TD2均有8个输出，以及自身t-1隐层的输出，还有一个偏置值，故有(8+12+1)*12*4=1008;
**lstm2**: 6个节点， lstm1均有12个输出，以及自身t-1隐层的输出，还有一个偏置值，故有(12+6+1)*6*4=456个参数;
TD1和TD2中的lstm是应用在时间长度为48的5分钟k线上的，而lstm1和lstm2是应用在10个交易日上的。
# Example7
Example7是使用神经网络进行排序学习，假设我们有一组股票比另一组股票更值得买入，那么可以尝试使用排序学习，训练出一个打分模型，帮助我们挑选出更值得买入的股票。这里展示排序学习主要是展示共享网络层的用法。
```python
# 假设X1的样本相对于X2对应位置的样本更值得买入
X1 = np.random.rand(10000,30)
X2 = np.random.rand(10000,30)
y = np.ones(10000) # y永远为1
# h1和h2是没有给定Input的网络层
h1 = layers.Dense(50,activation='relu',name='Example7_h1')
h2 = layers.Dense(1,activation='relu',name='Example7_h2')
# 定义两个Input层，接收X1和X2
Input1 = layers.Input(shape=(30,),name='Example7_input1')
Input2 = layers.Input(shape=(30,),name='Example7_input2')
#从Input1和Input2进来的数据流经过h1和h2之后，得到Score1和Score2
Score1 = h2(h1(Input1))
Score2 = h2(h1(Input2))
# 将Score1和Score2相减，得到得分差
Subtract = layers.Subtract(name='Example7_Subtract')([Score1,Score2])
# 每一个Score1都尽可能要大于Score2，故Output应尽可能输出1
Output = layers.Dense(1,activation='sigmoid',name='Example7_output')(Subtract)
model = Model(inputs=[Input1,Input2],outputs=Output)
model.compile(optimizer='sgd',loss='binary_crossentropy')
model.fit([X1,X2],y,batch_size=200,epochs=10)
from keras import backend
#定义Input1到Score1的数据流路径，这里使用Input1到Score1也是一样的
get_score = backend.function([Input1], [Score1]) #定义Input1到Score1的数据流路径，这里
X = np.vstack((X1,X2))
Score = get_score([X])[0] #Score进行选股即可
```
# Example8
Example8为多目标学习，假设你想使用基本面因子对股票做未来一个季度或者一个月的收益预测，然后使用技术面因子对股票做未来5个交易日或者10个交易日的收益预测，那么可以使用多目标学习，即可以输入多个X，输出多个Y。
```python
X1 = np.random.rand(10000,30) #假设为基本面因子
X2 = np.random.rand(10000,50) #假设为技术面因子
# 假设y1为分类任务，y2为回归任务
y1 = np.random.choice([0,1,2],size=10000)
y1 = np.eye(3,dtype=int)[y1]
y2 = np.random.randn(10000)
Input1 = layers.Input(shape=(30,),name='Example8_input1')
dense1 = layers.Dense(60,activation='relu',name='Example8_dense1')(Input1)
dense2 = layers.Dense(20,activation='relu',name='Example8_dense2')(dense1)
Output1 = layers.Dense(3,activation='softmax',name='Example8_output1')(dense2)
Input2 = layers.Input(shape=(50,),name='Example8_input2')
Concat = layers.Concatenate(axis=-1,name='Example8_concat')([Input2,dense2])
dense3 = layers.Dense(80,activation='relu',name='Example8_dense3')(Concat)
dense4 = layers.Dense(50,activation='relu',name='Example8_dense4')(dense3)
Output2 =layers.Dense(1,name='Example8_output2')(dense4)
model = Model(inputs=[Input1,Input2],outputs=[Output1,Output2])
# 分类任务用交叉熵损失函数，回归任务用均方误差损失函数，回归和分类的损失权重假设为2:1
model.compile(optimizer='sgd',
 loss={'Example8_output1':'categorical_crossentropy','Example8_output2':'mse'},
 loss_weights = {'Example8_output1':2,'Example8_output2':1})
model.fit([X1,X2],[y1,y2],batch_size=200,epochs=10)
```
网络结构如下：
![深度玩转神经网络——基于Keras](http://p99.pstatp.com/large/pgc-image/139f0a915f24473e87d02d79db9ed33b)

# Example9
下面再展示CNN的例子，CNN一般是用来做图像识别的，这里我们利用CNN来捕捉价格突变的信息，但是在逻辑上能使用1维卷积。这里先示范一个简单的CNN的例子，我们输入一天中的分钟价格序列，输出某一个我们自定义的标签。
```python
X = np.random.rand(10000,240,1) # 10000个样本，240根分钟k线的价格
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input = layers.Input(shape=(240,1,),name='Example9_input')
Conv1 = layers.Conv1D(filters=5,kernel_size=4,strides=1,padding='same',name='Example_9_Conv1')(Input)
maxpool = layers.MaxPool1D(pool_size=4,strides=1,padding='same',name='Example9_maxpool')(Conv1)
Conv2 = layers.Conv1D(filters=5,kernel_size=4,strides=1,padding='valid',name='Example_9_Conv2')(maxpool)
avgpool = layers.AveragePooling1D(pool_size=4,strides=1,padding='valid',name='Example9_avgpool')(Conv2)
flatten = layers.Flatten(name='Example9_flatten')(avgpool)
Output = layers.Dense(3,activation='softmax',name='Example9_output')(flatten)
model = Model(inputs=Input,output=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
print(model.summary())
```
![深度玩转神经网络——基于Keras](http://p3.pstatp.com/large/pgc-image/b20a7247347b4592a87293022ed409d0)
**Conv1**: filters个数为5, kernel_size为4，可以认为输入的数据filters为1，还有一个偏置值，故参数的个数为{ (kernel_size * 输入数据的filters + 偏置值个数) * filters个数 }=(4*1+1)*5=25，因为padding指定为'same'，所以shape为(240,5)；
**maxpool**: padding='same'，shape不变;
**Conv2**: filters个数为5，kernel_size为4，因为上一层的filters为5，故参数的个数为(4*5+1)*5=105，因为padding指定为'valid'，故大小将减小(kernel_size-1)，shape由(240,5)变成了(237,5)；
**avgpool**: padding='valid'，shape由(237,5)变成(234,5);
**output**: 接受经过flatten的234*5=1170个输出，以及偏置值，故有(1170+1)*3=3513个参数。
# Example10
将Example9的CNN替换Example6中分钟数据的lstm，分钟数据采用CNN处理，然后每个交易日都得到输出之后，再输入到LSTM。
```python
X = np.random.rand(10000,10,240,1) #10000个样本，10个交易日,240根分钟k线价格数据
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input = layers.Input(shape=(10,240,1,),name='Example10_input')
TD_Conv1 = layers.TimeDistributed(layers.Conv1D(filters=5,kernel_size=4,strides=1,padding='same'),name='Example10_TD_Conv1')(Input)
TD_maxpool = layers.TimeDistributed(layers.MaxPool1D(pool_size=4,strides=1,padding='same'),name='Example10_TD_maxpool')(TD_Conv1)
TD_Conv2 = layers.TimeDistributed(layers.Conv1D(filters=5,kernel_size=4,strides=1),name='Example10_TD_Conv2')(TD_maxpool)
TD_avgpool = layers.TimeDistributed(layers.AveragePooling1D(pool_size=4,strides=1),name='Example10_TD_avgpool')(TD_Conv2)
TD_flatten = layers.TimeDistributed(layers.Flatten(),name='Example10_TD_flatten')(TD_avgpool)
lstm1 = layers.LSTM(50,name='Exammple10_lstm1',return_sequences=True)(TD_flatten)
lstm2 = layers.LSTM(30,name='Exammple10_lstm2')(lstm1)
Output = layers.Dense(3,activation='softmax',name='Example10_output')(lstm2)
model = Model(inputs=Input,output=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
```
![深度玩转神经网络——基于Keras](http://p1.pstatp.com/large/pgc-image/2cba434a0a334a57a60af76c3d67d1b8)
结合Example6和Example9即可将参数的个数以及每一层的Output shape算清楚。
总结
Keras是基于tensorflow的高级神经网络API，使用Keras也可以搭建各种各样的神经网络。跟其他机器学习算法相比，神经网络可以轻松地实现多输入多输出，可以实现一些比较复杂的传统机器学习模型难以解决的任务。最后，需要提同时在时间和截面上的建模，并且支持醒大家的是，尽管神经网络功能十分强大，但是我们不能因为拿着锤子，然后看到什么东西都觉得是根钉子，研究量化策略模型，其核心还是需要基于对市场的理解，模型是对市场理解的刻画；另外，**根据奥卡姆剃刀原理**，在模型精度相同的情况下，应该选择简单的模型。因此，有时候我们无需追求复杂的模型，复杂的模型并不一定能带来令人满意的效果；相反，有时简单的模型却可以做出很好的效果。直接预测股票价格的变化，或者是预测未来的财务指标、新闻舆情分析等，深度学习都在发挥着它举足轻重的作用，越来越多的量化私募在路演的时候也都会介绍自己的深度学习策略，在各个论坛上，关于深度学习在量化投资上的应用的讨论也十分热烈。
Keras中文文档： https://keras.io/zh/
Example1
首先是一个很常规的3层神经网络，我们可以输入股票的基本面的选股因子，输出未来某一段时间股票获得正alpha的概率。
```python
import pandas as pd 
import numpy as np 
from keras import layers,Model
X = np.random.rand(10000,30) # 10000个样本,30个特征
y = np.random.choice([0,1],size=10000)
Input = layers.Input(shape=(30,),name='Example1_input')
dense1 = layers.Dense(60,activation='relu',name='Example1_dense1')(Input)
dense2 = layers.Dense(60,activation='relu',name='Example1_dense2')(dense1)
Output = layers.Dense(1,activation='sigmoid',name='Example1_output')(dense2)
model = Model(inputs=Input,outputs=Output)
model.compile(optimizer='sgd',loss='binary_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
print(model.summary())
```
![深度玩转神经网络——基于Keras](http://p3.pstatp.com/large/pgc-image/92df4f5700454feca467b3a543cf65b0)
**dense1**: 60个节点，每个节点接受30个特征的输入以及自身的偏置值，故有60*(30+1)=1860个参数;
**dense2**: 60个节点，每个节点接受dense1的60个输出以及自身的偏置值，故有60*(60+1)=3660个参数;
**output**: 1个节点，接受dense2的60个输出以及自身的偏置值，故有1*(60+1)=61个参数。
Example2
这是一个简单的RNN，很多人把循环神经网络看成是非线性的arima，其实也没错，但是我们使用arima的时候，大多数都是处理单个时间序列，而RNN本质上跟其他机器学习一样，也是用来处理回归和分类问题。例如在情感分析中，输入一段文本，输出文本的情感分类，而训练数据是很多条文本以及文本的标签。在量化投资中，可以输入过去n天各种技术指标的时间序列数据，例如MACD,KDJ,RSI等，输出未来某一段时间股票获得正alpha的概率。
```python
X = np.random.rand(10000,60,30) # 10000个样本,60个时间点,30个特征
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input = layers.Input(shape=(60,30,),name='Example2_input') # 输入60天的30个技术指标
rnn1 = layers.SimpleRNN(50,return_sequences=True,name='Example2_rnn1')(Input)
rnn2 = layers.SimpleRNN(50,name='Example2_rnn2')(rnn1)
Output = layers.Dense(3,activation='softmax',name='Example2_output')(rnn2)
model = Model(inputs=Input,outputs=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
print(model.summary())
```
![深度玩转神经网络——基于Keras](http://p3.pstatp.com/large/pgc-image/217e3218225e4b76a1f8ffd9c9c1e1b9)
**rnn1**: 50个节点，接受输入层30个特征、自身t-1隐层的输出，以及自身的偏置值，故有(30+50+1)*50=4050个参数
**rnn2**: 50个节点，接受rnn1的50个隐层的输出、自身t-1隐层的输出，以及自身的偏置值，故有(50+50+1)*50=5050个参数
**output**: 3个节点，接受rnn2的50个输出以及自身的偏置值，故有3*(50+1)=153个参数。
Example3
如果我们认为基本面和技术面不能简单地混合一起用，我们将Example1和Example2结合起来，基本面的特征使用普通的全连接网络，技术面的特征使用时序模型RNN，然后再将它们的输出合并起来，再输入到输出层。
```python
X1 = np.random.rand(10000,50) # 10000个样本,50个特征
X2 = np.random.rand(10000,60,30) # 10000个样本,60个时间点,30个特征
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input1 = layers.Input(shape=(50,),name='Example3_input1')
dense1 = layers.Dense(100,activation='relu',name='Example3_dense1')(Input1)
dense2 = layers.Dense(100,activation='relu',name='Example3_dense2')(dense1)
Input2 = layers.Input(shape=(60,30,),name='Example3_input2')
rnn1 = layers.SimpleRNN(60,return_sequences=True,name='Example3_rnn1')(Input2)
rnn2 = layers.SimpleRNN(60,name='Example3_rnn2')(rnn1)
concat = layers.Concatenate(axis=-1,name='Example3_concat')([dense2,rnn2])
Output = layers.Dense(3,activation='softmax',name='output')(concat)
model = Model(inputs=[Input1,Input2],outputs=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit([X1,X2],y,batch_size=200,epochs=10)
```
网络的结构如下：
![深度玩转神经网络——基于Keras](http://p1.pstatp.com/large/pgc-image/29bcb9fada71488ba5f8a16d4e99f281)
![深度玩转神经网络——基于Keras](http://p99.pstatp.com/large/pgc-image/e30320627da64cf8b6514d0c782f4e6f)
**dense1**: 100个节点，接受input1的50个特征，以及自身的偏置值，故有(50+1)*100=5100个参数;
**rnn1**: 50个节点，接受input2的30个特征，以及自身t-1时刻的50个隐层输出，故有(30+50+1)*50=4050个参数;
**dense2**: 100个节点，接受dense1的100个隐层输出，以及自身的偏置值，故有(100+1)*100=10100个参数;
**rnn2**: 50个节点，接受rnn1的50个隐层的输出、自身t-1隐层的输出，以及自身的偏置值，故有(50+50+1)*50=5050个参数;
**output**: 3个节点，接受dense2的100个隐层的输出、rnn2的50个隐层的输出，以及自身的偏置值，故有(100+50+1)*3=453个参数。
Example4
事实上我们较少使用的简单的RNN，使用LSTM和GRU比较多。另外在搭建网络时，我们一般在每一层网络的输入上做BatchNormalization和Dropout，Example4为Example2的改进。
关于BatchNormalization和Dropout可以看我们的之前的推文：
深度学习科普及其在多因子选股中的应用
```python
X = np.random.rand(10000,60,30) # 10000个样本,60个时间点,30个特征
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input = layers.Input(shape=(60,30,),name='Example4_input')
BN_Input = layers.BatchNormalization(name='Example4_BN_input')(Input)
Dropout_Input = layers.Dropout(0.5,name='Example4_Dropout_input')(BN_Input)
lstm1 = layers.LSTM(50,dropout=0.3,return_sequences=True,name='Example4_lstm1')(Dropout_Input)
BN_lstm1 = layers.BatchNormalization(name='Example4_BN_lstm1')(lstm1)
Dropout_lstm1 = layers.Dropout(0.5,name='Example4_Dropout_lstm1')(BN_lstm1)
lstm2 = layers.LSTM(50,dropout=0.3,name='Example4_lstm2')(Dropout_lstm1)
BN_lstm2 = layers.BatchNormalization(name='Example4_BN_lstm2')(lstm2)
Dropout_lstm2 = layers.Dropout(0.5,name='Example4_Dropout_lstm2')(BN_lstm2)
Output = layers.Dense(3,activation='softmax',name='Example4_output')(Dropout_lstm2)
model = Model(inputs=Input,outputs=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
print(model.summary())
```
![深度玩转神经网络——基于Keras](http://p99.pstatp.com/large/pgc-image/6761d23d141040f58f1d519e71660363)
**BN_input**: 接受30个特征输入，每一个特征都需要求出样本的均值，方差，然后要给出γ和β，故有30*2=60个无需训练的参数和30*2=60个需要训练的参数。总共120个参数；
**lstm1**: 50个隐层节点，每一个节点接受30个特征输入和50个t-1隐层输出以及偏置值，每一个隐层节点，都对应着输入门，遗忘门和输出门，它们也都有30个特征输入和50个t-1隐层输出以及偏置值，跟rnn相比，多出了三个门的参数，参数的个数变成了原来的4倍。故参数的个数总共有(30+50+1)*50*4=16200;
**BN_lstm1**: 接受lstm1的50个隐层节点的输出，故有50*2=100个无需训练的参数和50*2=100个需要训练的参数。总共200个参数;
**lstm2**: 50个隐层节点，还有对应的输入门，遗忘门，输出门，都接受lstm1的50个隐层输出、自身50个t-1隐层输出以及偏置值，故参数的个数总共有(50+50+1)*50*4=20200;
**BN_lstm2**: 接受lstm2的50个隐层节点的输出，故有50*2=100个无需训练的参数和50*2=100个需要训练的参数。总共200个参数;
**output**: 3个节点，接受lstm2的50个隐层节点的输出以及自身的偏置值，故有(50+1)*3=153个参数。
Example5
将Example4中BatchNormalization和Dropout应用到Example3中，再将Example3中SimpleRNN换成LSTM。
```python
X1 = np.random.rand(10000,50) # 10000个样本,50个特征
X2 = np.random.rand(10000,60,30) # 10000个样本,60个时间点,30个特征
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input1 = layers.Input(shape=(50,),name='Example5_input1')
BN_Input1 = layers.BatchNormalization(name='Example5_BN_input1')(Input1)
Dropout_Input1 = layers.Dropout(0.5,name='Example5_Dropout_input1')(BN_Input1)
dense1 = layers.Dense(100,activation='relu',name='Example5_dense1')(Dropout_Input1)
BN_dense1 = layers.BatchNormalization(name='Example5_BN_dense1')(dense1)
Dropout_dense1 = layers.Dropout(0.5,name='Example5_Dropout_dense1')(BN_dense1)
dense2 = layers.Dense(100,activation='relu',name='Example5_dense2')(Dropout_dense1)
Input2 = layers.Input(shape=(60,30,),name='Example5_input2')
BN_Input2 = layers.BatchNormalization(name='Example5_BN_input2')(Input2)
Dropout_Input2 = layers.Dropout(0.5,name='Example5_Dropout_input2')(BN_Input2)
rnn1 = layers.SimpleRNN(60,dropout=0.3,return_sequences=True,name='Example5_rnn1')(Dropout_Input2)
BN_rnn1 = layers.BatchNormalization(name='Example5_BN_rnn1')(rnn1)
Dropout_rnn1 = layers.Dropout(0.5,name='Example5_Dropout_rnn1')(BN_rnn1)
rnn2 = layers.SimpleRNN(60,name='Example5_rnn2')(Dropout_rnn1)
concat = layers.Concatenate(axis=-1,name='Example5_concat')([dense2,rnn2])
BN_concat = layers.BatchNormalization(name='Example5_BN_concat')(concat)
Dropout_concat = layers.Dropout(0.5,name='Example5_Dropout_concat')(BN_concat)
Output = layers.Dense(3,activation='softmax',name='output')(Dropout_concat)
model = Model(inputs=[Input1,Input2],outputs=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit([X1,X2],y,batch_size=200,epochs=10)
print(model.summary())
```
参考Example3和Example4就可以将参数数清楚，这里不再累赘。
每一个神经网络模型，理论上都应该使用BatchNormalization，Dropout，下面的例子为了代码的简洁，将不再出现
BatchNormalization，Dropout。
Example6
一般我们使用全接连网络训练基本面因子，使用LSTM训练技术面指标；如果你想输入最近几个交易日的分钟行情数据，交易日的数据是属于时间序列数据，而在一个交易日中，分钟行情数据又是时间序列数据，那么我们使用如下的网络进行训练。
其中TimeDistributed给与了模型一对一，多对多的能力，增加了模型的维度。
```python
X = np.random.rand(10000,10,48,5) #10000个样本,10个交易,48根5分钟k线,5个特征
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input = layers.Input(shape=(10,48,5,))
TD1 = layers.TimeDistributed(layers.LSTM(8,return_sequences=True),name='Example6_TD1')(Input)
TD2 = layers.TimeDistributed(layers.LSTM(8),name='Example6_TD2')(TD1)
lstm1 = layers.LSTM(12,return_sequences=True,name='Example6_lstm1')(TD2)
lstm2 = layers.LSTM(6,name='Example6_lstm2')(lstm1)
Output = layers.Dense(3,activation='softmax',name='Example_output')(lstm2)
model = Model(inputs=Input,outputs=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
print(model.summary())
```
![深度玩转神经网络——基于Keras](http://p99.pstatp.com/large/pgc-image/d2882cea68bf4221a2d5f22d85f69d45)
**TD1**: 10个交易日均使用同一个lstm，这个lstm有8个隐层节点，输入的时间长度是48，接受5个特征以及t-1隐层的输出，还有一个偏置值，参数的个数是(5+8+1)*8*4=448;
**TD2**: 8个隐层节点，TD1中lstm的8个隐层节点的输出，以及自身t-1隐层的输出，还有一个偏置值，参数的个数是(8+8+1)*8*4=544;
**lstm1**: 12个节点， 10个交易的TD2均有8个输出，以及自身t-1隐层的输出，还有一个偏置值，故有(8+12+1)*12*4=1008;
**lstm2**: 6个节点， lstm1均有12个输出，以及自身t-1隐层的输出，还有一个偏置值，故有(12+6+1)*6*4=456个参数;
TD1和TD2中的lstm是应用在时间长度为48的5分钟k线上的，而lstm1和lstm2是应用在10个交易日上的。
Example7
Example7是使用神经网络进行排序学习，假设我们有一组股票比另一组股票更值得买入，那么可以尝试使用排序学习，训练出一个打分模型，帮助我们挑选出更值得买入的股票。这里展示排序学习主要是展示共享网络层的用法。
```python
# 假设X1的样本相对于X2对应位置的样本更值得买入
X1 = np.random.rand(10000,30)
X2 = np.random.rand(10000,30)
y = np.ones(10000) # y永远为1
# h1和h2是没有给定Input的网络层
h1 = layers.Dense(50,activation='relu',name='Example7_h1')
h2 = layers.Dense(1,activation='relu',name='Example7_h2')
# 定义两个Input层，接收X1和X2
Input1 = layers.Input(shape=(30,),name='Example7_input1')
Input2 = layers.Input(shape=(30,),name='Example7_input2')
#从Input1和Input2进来的数据流经过h1和h2之后，得到Score1和Score2
Score1 = h2(h1(Input1))
Score2 = h2(h1(Input2))
# 将Score1和Score2相减，得到得分差
Subtract = layers.Subtract(name='Example7_Subtract')([Score1,Score2])
# 每一个Score1都尽可能要大于Score2，故Output应尽可能输出1
Output = layers.Dense(1,activation='sigmoid',name='Example7_output')(Subtract)
model = Model(inputs=[Input1,Input2],outputs=Output)
model.compile(optimizer='sgd',loss='binary_crossentropy')
model.fit([X1,X2],y,batch_size=200,epochs=10)
from keras import backend
#定义Input1到Score1的数据流路径，这里使用Input1到Score1也是一样的
get_score = backend.function([Input1], [Score1]) #定义Input1到Score1的数据流路径，这里
X = np.vstack((X1,X2))
Score = get_score([X])[0] #Score进行选股即可
```
Example8
Example8为多目标学习，假设你想使用基本面因子对股票做未来一个季度或者一个月的收益预测，然后使用技术面因子对股票做未来5个交易日或者10个交易日的收益预测，那么可以使用多目标学习，即可以输入多个X，输出多个Y。
```python
X1 = np.random.rand(10000,30) #假设为基本面因子
X2 = np.random.rand(10000,50) #假设为技术面因子
# 假设y1为分类任务，y2为回归任务
y1 = np.random.choice([0,1,2],size=10000)
y1 = np.eye(3,dtype=int)[y1]
y2 = np.random.randn(10000)
Input1 = layers.Input(shape=(30,),name='Example8_input1')
dense1 = layers.Dense(60,activation='relu',name='Example8_dense1')(Input1)
dense2 = layers.Dense(20,activation='relu',name='Example8_dense2')(dense1)
Output1 = layers.Dense(3,activation='softmax',name='Example8_output1')(dense2)
Input2 = layers.Input(shape=(50,),name='Example8_input2')
Concat = layers.Concatenate(axis=-1,name='Example8_concat')([Input2,dense2])
dense3 = layers.Dense(80,activation='relu',name='Example8_dense3')(Concat)
dense4 = layers.Dense(50,activation='relu',name='Example8_dense4')(dense3)
Output2 =layers.Dense(1,name='Example8_output2')(dense4)
model = Model(inputs=[Input1,Input2],outputs=[Output1,Output2])
# 分类任务用交叉熵损失函数，回归任务用均方误差损失函数，回归和分类的损失权重假设为2:1
model.compile(optimizer='sgd',
 loss={'Example8_output1':'categorical_crossentropy','Example8_output2':'mse'},
 loss_weights = {'Example8_output1':2,'Example8_output2':1})
model.fit([X1,X2],[y1,y2],batch_size=200,epochs=10)
```
网络结构如下：
![深度玩转神经网络——基于Keras](http://p99.pstatp.com/large/pgc-image/bc9a2011f20a46bd9a585139928f7e12)
Example9
下面再展示CNN的例子，CNN一般是用来做图像识别的，这里我们利用CNN来捕捉价格突变的信息，但是在逻辑上能使用1维卷积。这里先示范一个简单的CNN的例子，我们输入一天中的分钟价格序列，输出某一个我们自定义的标签。
```python
X = np.random.rand(10000,240,1) # 10000个样本，240根分钟k线的价格
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input = layers.Input(shape=(240,1,),name='Example9_input')
Conv1 = layers.Conv1D(filters=5,kernel_size=4,strides=1,padding='same',name='Example_9_Conv1')(Input)
maxpool = layers.MaxPool1D(pool_size=4,strides=1,padding='same',name='Example9_maxpool')(Conv1)
Conv2 = layers.Conv1D(filters=5,kernel_size=4,strides=1,padding='valid',name='Example_9_Conv2')(maxpool)
avgpool = layers.AveragePooling1D(pool_size=4,strides=1,padding='valid',name='Example9_avgpool')(Conv2)
flatten = layers.Flatten(name='Example9_flatten')(avgpool)
Output = layers.Dense(3,activation='softmax',name='Example9_output')(flatten)
model = Model(inputs=Input,output=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
print(model.summary())
```
![深度玩转神经网络——基于Keras](http://p3.pstatp.com/large/pgc-image/08b5ab08ffc54b6787fabc5999261401)
**Conv1**: filters个数为5, kernel_size为4，可以认为输入的数据filters为1，还有一个偏置值，故参数的个数为{ (kernel_size * 输入数据的filters + 偏置值个数) * filters个数 }=(4*1+1)*5=25，因为padding指定为'same'，所以shape为(240,5)；
**maxpool**: padding='same'，shape不变;
**Conv2**: filters个数为5，kernel_size为4，因为上一层的filters为5，故参数的个数为(4*5+1)*5=105，因为padding指定为'valid'，故大小将减小(kernel_size-1)，shape由(240,5)变成了(237,5)；
**avgpool**: padding='valid'，shape由(237,5)变成(234,5);
**output**: 接受经过flatten的234*5=1170个输出，以及偏置值，故有(1170+1)*3=3513个参数。
# Example10
将Example9的CNN替换Example6中分钟数据的lstm，分钟数据采用CNN处理，然后每个交易日都得到输出之后，再输入到LSTM。
```python
X = np.random.rand(10000,10,240,1) #10000个样本，10个交易日,240根分钟k线价格数据
y = np.random.choice([0,1,2],size=10000)
y = np.eye(3,dtype=int)[y]
Input = layers.Input(shape=(10,240,1,),name='Example10_input')
TD_Conv1 = layers.TimeDistributed(layers.Conv1D(filters=5,kernel_size=4,strides=1,padding='same'),name='Example10_TD_Conv1')(Input)
TD_maxpool = layers.TimeDistributed(layers.MaxPool1D(pool_size=4,strides=1,padding='same'),name='Example10_TD_maxpool')(TD_Conv1)
TD_Conv2 = layers.TimeDistributed(layers.Conv1D(filters=5,kernel_size=4,strides=1),name='Example10_TD_Conv2')(TD_maxpool)
TD_avgpool = layers.TimeDistributed(layers.AveragePooling1D(pool_size=4,strides=1),name='Example10_TD_avgpool')(TD_Conv2)
TD_flatten = layers.TimeDistributed(layers.Flatten(),name='Example10_TD_flatten')(TD_avgpool)
lstm1 = layers.LSTM(50,name='Exammple10_lstm1',return_sequences=True)(TD_flatten)
lstm2 = layers.LSTM(30,name='Exammple10_lstm2')(lstm1)
Output = layers.Dense(3,activation='softmax',name='Example10_output')(lstm2)
model = Model(inputs=Input,output=Output)
model.compile(optimizer='sgd',loss='categorical_crossentropy')
model.fit(X,y,batch_size=200,epochs=10)
```
![深度玩转神经网络——基于Keras](http://p3.pstatp.com/large/pgc-image/70e8fdc42b344139b2c09a011bb5ded6)
结合Example6和Example9即可将参数的个数以及每一层的Output shape算清楚。
# 总结
Keras是基于tensorflow的高级神经网络API，使用Keras也可以搭建各种各样的神经网络。跟其他机器学习算法相比，神经网络可以轻松地实现多输入多输出，可以实现一些比较复杂的传统机器学习模型难以解决的任务。最后，需要提同时在时间和截面上的建模，并且支持醒大家的是，尽管神经网络功能十分强大，但是我们不能因为拿着锤子，然后看到什么东西都觉得是根钉子，研究量化策略模型，其核心还是需要基于对市场的理解，模型是对市场理解的刻画；另外，**根据奥卡姆剃刀原理**，在模型精度相同的情况下，应该选择简单的模型。因此，有时候我们无需追求复杂的模型，复杂的模型并不一定能带来令人满意的效果；相反，有时简单的模型却可以做出很好的效果。


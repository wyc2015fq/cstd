# 循环神经网络RNN——利用LSTM对脑电波信号进行分类，Keras实现 - shine302的博客 - CSDN博客
2018年04月08日 16:42:37[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：1487

本文对脑电波信号进行分类。人类大脑在接受不同的刺激（光照、声音）时，脑电波的形状是不一样的。
因而，想通过对脑电波进行学习，从而判断大脑接受了哪种刺激。
**我们在人的大脑上安装了32个芯片，芯片的分布如下图所示。这些芯片每隔一定的秒数就会测得信号、这些信号就是脑电波信号。显然，脑电波数据是时间序列。**
![](https://img-blog.csdn.net/20180408165742888?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
脑电波的时间序列如下：
![](https://img-blog.csdn.net/20180408170208176?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
每一条脑电波都对应了一种刺激（共6种），结构如下：
![](https://img-blog.csdn.net/20180408170325721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**本文构建了LSTM模型来对脑电波数据进行分类，深度学习平台采用的keras。核心代码如下：**
**# code for building an LSTM with 100 neurons and dropout. Runs for 50 epochs**
**model = Sequential()**
**model.add(LSTM(100, return_sequences=False, input_shape=(time_steps, n_features)))**
**参数解释：**
**1）每个脑电波信号都由time_steps个单元组成，而每个单元用n_features维的[向量](http://blog.csdn.net/jiangpeng59/article/details/77533309)表示。**
**2）return_sequences：布尔值，默认False，控制返回类型。若为True则返回整个序列，否则仅返回输出序列的最后一个输出 **
**model.add(Dropout(0.5))model.add(Dense(1, activation='sigmoid'))model.compile(loss='binary_crossentropy',              optimizer='rmsprop',              metrics=['accuracy'])model.fit(X_train, y_train, batch_size=16, epochs=50)**
**score = model.evaluate(X_test, y_test, batch_size=16)**
**模型核心结构如下图所示：**
![](https://img-blog.csdn.net/201804081715099?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NoaW5lMzAy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
(1)V1为32维的向量
(2)通过第一个LSTM输出为维度100，而return_sequences=True，我们可以获得time_steps个100维的向量,最后因为return_sequences=False，所以只输出了最后一个红色的向量
（3）为输入数据施加Dropout。Dropout将在训练过程中每次更新参数时随机断开一定百分比（p）的输入神经元连接，Dropout层用于防止过拟合。
```python
keras.layers.core.Dropout(p)
```
- p：0~1的浮点数，控制需要断开的链接的比例
(4)100维度的红色的向量输入到Dense层（全连接层），激活函数是sigmod
(5)全连接层的输出是类标向量

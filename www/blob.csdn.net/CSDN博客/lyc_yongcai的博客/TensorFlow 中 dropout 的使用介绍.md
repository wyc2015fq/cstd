
# TensorFlow 中 dropout 的使用介绍 - lyc_yongcai的博客 - CSDN博客


2017年06月19日 11:24:48[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：2210


dropout 主要作用就是防止过拟合。
dropout 一般都是用在全连接中，在卷积部分不会用到 dropout ，输出层也不会用到，一般用在输入层与输出层之间。
在 tensorflow 中有两种形式：
（1）tf.nn.dropout(x, keep_prob, noise_shape = None, seed = None, name = None)
（2）tf.contrib.rnn.DropoutWrapper(rnn_cell, input_keep_prob)

一般情况下的 dropout :

```python
def dropout(x, keep_prob, noise_shape=None, seed=None, name=None)
#x: 输入
#keep_prob: 名字代表的意思
#return：包装了dropout的x。训练的时候用，test的时候就不需要dropout了
#例：
w = tf.get_variable("w1",shape=[size, out_size])
x = tf.placeholder(tf.float32, shape=[batch_size, size])
x = tf.nn.dropout(x, keep_prob=0.5)
y = tf.matmul(x,w)
```
RNN 中的 dropout :

```python
# 定义 LSTM 结构
lstm = tf.contrib.rnn.BasicLSTMCell(lstm_hidden_size)
# 使用 DropoutWrapper 类来实现 dropout 功能，input_keep_prob 控制输出的 dropout 概率
dropout_lstm = tf.contrib.rnn.DropoutWrapper(lstm, input_keep_prob=0.5)
stacked_lstm = tf.contrib.rnn.MultiRNNCell([dropout_lstm] * number_of_layers)
```









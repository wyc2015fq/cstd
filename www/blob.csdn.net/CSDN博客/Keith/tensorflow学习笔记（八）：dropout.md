# tensorflow学习笔记（八）：dropout - Keith - CSDN博客





2016年10月22日 11:28:47[ke1th](https://me.csdn.net/u012436149)阅读数：20223
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









**我们都知道dropout对于防止过拟合效果不错**
**dropout一般用在全连接的部分，卷积部分一般不会用到dropout,输出曾也不会使用dropout，适用范围[输入，输出)**
- tf.nn.dropout(x, keep_prob, noise_shape=None, seed=None, name=None)
- tf.nn.rnn_cell.DropoutWrapper(rnn_cell, input_keep_prob=1.0, output_keep_prob=1.0)

## 普通dropout

```python
def dropout(x, keep_prob, noise_shape=None, seed=None, name=None)
#x: 输入
#keep_prob: 名字代表的意思, keep_prob 参数可以为 tensor，意味着，训练时候 feed 为0.5，测试时候 feed 为 1.0 就 OK。 
#return：包装了dropout的x。训练的时候用，test的时候就不需要dropout了
#例：
w = tf.get_variable("w1",shape=[size, out_size])
x = tf.placeholder(tf.float32, shape=[batch_size, size])
x = tf.nn.dropout(x, keep_prob=0.5)
y = tf.matmul(x,w)
```

## rnn中的dropout

```python
def rnn_cell.DropoutWrapper(rnn_cell, input_keep_prob=1.0, output_keep_prob=1.0):
#例
lstm_cell = tf.nn.rnn_cell.BasicLSTMCell(size, forget_bias=0.0, state_is_tuple=True)
lstm_cell = tf.nn.rnn_cell.DropoutWrapper(lstm_cell, output_keep_prob=0.5)
#经过dropout包装的lstm_cell就出来了
```





# TensorFlow问题：Variable rnn-basic_lstm_cell-kernel already exists, disallowed - manduner的博客 - CSDN博客


2018年08月31日 19:32:22[Manduner_TJU](https://me.csdn.net/manduner)阅读数：482


# 1，前言
首先陈述下我遇到的问题，在我写的程序中需要同时用到两个RNN模型，而且这两个RNN模型需要同时参与训练。结果出现了报错：Variable rnn/basic_lstm_cell/kernel already exists, disallowed....
再者表达下我从网上查找这类错误的经历，在google中快搜上述错误，确实会搜到一些解决办法，主流办法主要有两种：
（1）通过*with tf.variable_scope('scope',reuse=True)*，给不同的RNN模型定义不同的作用域
（2）在代码中写上*tf.reset_default_graph()*
下面陈述在测试这两种方法时遇到的问题
# 2，网上方法遇到的问题陈述
## （1）第一种方法
## with tf.variable_scope('scope',reuse=True)
在网上看到这种方法的时候，我都不知道这行代码应该写在工程代码中的那一块（我是tensorflow菜鸟一枚，着实不知道在哪儿写，亲测后才知道）。这行代码应该写在RNN模型开始执行的前边，以下代码段给出正确的书写位置
```python
def RNN_u():
	pass
def RNN_i():
	pass
		
with tf.variable_scope('user'):
	self.h_pool_flat_u = RNN_u(self.embedded_user,weights,biases) #RNN模型开始执行的代码
with tf.variable_scope('item',reuse = True):
	self.h_pool_flat_i = RNN_i(self.embedded_item,weights,biases) #RNN模型开始执行的代码
```
位置写正确了，开始运行整个程序吧，还是报错、、、无奈，只能试第二种办法
## （2）第二种方法
## tf.reset_default_graph()
这种方法，真真是不知道写在代码的程序中的哪个位置，我试试了写在两个模型的前边、两个模型的中间，运行程序还是会报错
# 3，解决方案（亲测，程序运行成功）
经过测试，能让程序正常运行的方法是第一种方法的改良版，就是将*reuse=True*去掉，代码如下：
```python
def RNN_u():
	pass
def RNN_i():
	pass
		
with tf.variable_scope('user'):
	self.h_pool_flat_u = RNN_u(self.embedded_user,weights,biases) #RNN模型开始执行的代码
with tf.variable_scope('item'):
	self.h_pool_flat_i = RNN_i(self.embedded_item,weights,biases) #RNN模型开始执行的代码
```
# 4，总结
适合自己的问题的方法才是有用的方法，希望以上解答能够帮助到大家


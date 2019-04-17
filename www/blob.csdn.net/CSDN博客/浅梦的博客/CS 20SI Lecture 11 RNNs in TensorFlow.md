# CS 20SI|Lecture 11 RNNs in TensorFlow - 浅梦的博客 - CSDN博客





2017年03月17日 15:27:05[浅梦s](https://me.csdn.net/u012151283)阅读数：616








# Agenda
- All about RNNs
- Implementation tricks & treats
- Live demo of Language Modeling

# 从前馈网络到RNN

![这里写图片描述](https://img-blog.csdn.net/20170317141525127?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- RNN可以提取序列信息
- 有向环
- 所有步骤共享权值减少模型参数
- NLP的基石
- 也可用在图像上

## Simple Recurrent Neural Network(SRNN)

![这里写图片描述](https://img-blog.csdn.net/20170317141758258?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170317141847712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## The rise of LSTMs

RNN并不擅长捕捉长时间的依赖关系。



- 可以控制采用多少当前的输入，忘记多少之前的隐藏状态
- 与人类处理信息类似
- 最早在1997年由Hochreiter和Schmidhuber发表论文 




> 
*Hochreiter, Sepp, and Jürgen Schmidhuber. “Long short-term memory.” Neural computation 9.8 (1997): 1735-1780.
   
![这里写图片描述](https://img-blog.csdn.net/20170317142509625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170317142746583?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从以上图片可以清晰的看出典型LSTM的结构，它只是对普通RNN单元结构的内部做了改变。输入门，遗忘门，和输出门都对当前输入和之前隐藏状态做了相同的操作(sigmoid激活)，只是权重不同。新记忆单元使用双曲正切函数tanh作为激活函数，其输入仍然是当前输入和之前隐藏状态。最终记忆单元又输出门结果和前一次的最终隐藏单元逐元素乘法再加上输入门结果和新记忆单元逐元素乘法构成。实现了选择性地保留当前输入和遗忘之前的输入。隐藏状态由输出门结果和经过tanh激活的最终记忆单元做逐元素乘法得出。

## GRUs

人们发现LSTM表现不错，但是过于复杂，于是引入了GRU（门限重复单元 Gated Recurrent Units） 
![这里写图片描述](https://img-blog.csdn.net/20170317144342365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


- 计算代价小
- 性能和LSTM差不多 




> 
*Chung, Junyoung, et al. “Empirical evaluation of gated recurrent neural networks on sequence modeling.” arXiv preprint arXiv:1412.3555 (2014)
  


## RNN用途
- 字符级语言模型
- 机器翻译
- 文本总结
- 图片说明

# RNNs in TensorFlow

## 单元支持(tf.nn.rnn_cell)
- BasicRNNCell: The most basic RNN cell.  
- RNNCell: Abstract object representing an RNN cell. 
- BasicLSTMCell: Basic LSTM recurrent network cell.
- LSTMCell: LSTM recurrent network cell. 
- GRUCell: Gated Recurrent Unit cell 

## 构建单元(tf.nn.rnn_cell)

`cell = tf.nn.rnn_cell.GRUCell(hidden_size)`

## 堆叠多个单元

```python
cell = tf.nn.rnn_cell.GRUCell(hidden_size)
rnn_cells = tf.nn.rnn_cell.MultiRNNCell([cell] * num_layers)
```

构建循环神经网络
- `tf.nn.dynamic_rnn`: uses a tf.While loop to dynamically construct the graph when it is executed. Graph creation is faster and you can feed batches of variable size. 
- `tf.nn.bidirectional_dynamic_rnn`: dynamic_rnn with bidirectional

```python
cell = tf.nn.rnn_cell.GRUCell(hidden_size)
rnn_cells = tf.nn.rnn_cell.MultiRNNCell([cell] * num_layers)
output, out_state = tf.nn.dynamic_rnn(cell, seq, length, initial_state)#大多数序列并不是等长的！
```

## 处理可变序列长度

用零向量填充所有序列和标签使得具有相同的长度 

大部分当前模型不能处理长度大于120个tokens的序列。所以通常有一个固定的`max_length`然后我们修正序列到那个长度。 

存在的问题：填充的标签改变了总的损失，影响了梯度求解。
## 填充/修正序列长度

### 方法1：
- 维护一个掩码(真实tokensTrue,填充tokensFalse)
- 在真实/填充tokens上运行模型（模型也会预测填充tokens的标签）
- 只考虑由真实元素引起的损失

### 方法2：
- 让你的模型直到真实的序列这样它会只预测真实的tokens

```python
cell = tf.nn.rnn_cell.GRUCell(hidden_size) rnn_cells = tf.nn.rnn_cell.MultiRNNCell([cell] * num_layers) tf.reduce_sum(tf.reduce_max(tf.sign(seq), 2), 1) output, out_state = tf.nn.dynamic_rnn(cell, seq, length, initial_state)
```

# 如何处理训练RNN的常见问题

## 梯度消失

使用不同的激活单元：
- tf.nn.relu
- tf.nn.relu6
- tf.nn.crelu
- 
tf.nn.elu 

另外：

- 
tf.nn.softplus

- tf.nn.softsign
- tf.nn.bias_add
- tf.sigmoid
- tf.tanh

## 梯度爆炸

使用`tf.clip_by_global_norm`裁剪梯度。就是当梯度的变化超过某一定值的时，同比缩放所有的梯度变化量。

```python
gradients = tf.gradients(cost, tf.trainable_variables()) # take gradients of cosst w.r.t. all trainable variables
clipped_gradients, _ = tf.clip_by_global_norm(gradients, max_grad_norm) # clip the gradients by a pre-defined max norm
optimizer = tf.train.AdamOptimizer(learning_rate) train_op = optimizer.apply_gradients(zip(gradients, trainables)) # add the clipped gradients to the optimizer
```

## 学习率退火（衰减学习率）

Optimizers accept both scalars and tensors as learning rate

```python
learning_rate = tf.train.exponential_decay(init_lr,global_step,decay_steps,decay_rate,   staircase=True) 
optimizer = tf.train.AdamOptimizer(learning_rate)
```

## 过拟合

使用随机失活通过`tf.nn.dropout`或 `DropoutWrapper` for cells
- tf.nn.dropout

```python
hidden_layer = tf.nn.dropout(hidden_layer,keep_prob)
```
- DropoutWrapper 

```python
cell = tf.nn.rnn_cell.GRUCell(hidden_size)
cell = tf.nn.rnn_cell.DropoutWrapper(cell,                                      output_keep_prob=keep_prob)
```

# 语言模型

## 神经语言模型
- 允许衡量一个句子的可能性（像不像正常句子）
- 机器翻译的重要输入
- 生成新文本

## 主要方法
- 词语级n元语言模型
- 字符级
- 子词语级，介于以上两者之间

## N-grams
- 传统方法
- 训练一个根据前面n个词预测下一个词的模型
- 巨大的词汇表
- 不能泛化到词汇表之外
- 需要大量内存

## 字符级
- 2010s引入
- 输入和输出都是字符
- 优点 
- 小词汇表
- 不需要词嵌入
- 快速训练

- 缺点 
- 流畅性低（很多词语都是没有意义的乱语）


## 混合
- 默认词语级，预见不知道的tokens转换成字符级

## Subword-level

![这里写图片描述](https://img-blog.csdn.net/20170317153850315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)









# tensorflow学习笔记（六）：LSTM 与 GRU - Keith - CSDN博客





2016年10月21日 21:20:25[ke1th](https://me.csdn.net/u012436149)阅读数：41715
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









[10.26.2016 更新]

[新增内容：今天写代码的时候，如果使用state_is_tuple=True,  那么

```python
initial_state = lstm_cell.zero_state(batch_size, dtype=tf.float32)
...
with tf.Session() as sess:
	tf.initialize_all_variables().run()
	state = initial_state.eval()   #state_is_tuple=True的话，这里会报错，不用eval也会报错
	sess.run([train_op, state],    #所以我使用了state_is_tuple=False,不知0.11.0会不会正常
		feed_dict={
		initial_state:state
		})
```

]

# LSTM & GRU

## 基本LSTM

**tensorflow提供了LSTM实现的一个basic版本，不包含lstm的一些高级扩展，同时也提供了一个标准接口，其中包含了lstm的扩展。分别为：tf.nn.rnn_cell.BasicLSTMCell(), tf.nn.rnn_cell.LSTMCell()**

### LSTM的结构

盗用一下[Understanding LSTM Networks](http://colah.github.io/posts/2015-08-Understanding-LSTMs/)上的图
![图一](http://upload-images.jianshu.io/upload_images/42741-b9a16a53d58ca2b9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)图一
**tensorflow中的BasicLSTMCell()是完全按照这个结构进行设计的，BasicLSTMCell只构建了一个时间步的计算图，下面会说到，在tf中有两种方式进行时间步展开。**

```python
#tf.nn.rnn_cell.BasicLSTMCell(num_units, forget_bias, input_size, state_is_tupe=Flase, activation=tanh)
cell = tf.nn.rnn_cell.BasicLSTMCell(num_units, forget_bias=1.0, input_size=None, state_is_tupe=Flase, activation=tanh)
#num_units:图一中ht的维数，如果num_units=10,那么ht就是10维行向量
#forget_bias：遗忘门的初始化偏置
#input_size:[batch_size, max_time, size]。假设要输入一句话，这句话的长度是不固定的，max_time就代表最长的那句话是多长，size表示你打算用多长的向量代表一个word，即embedding_size（embedding_size和size的值不一定要一样）
#state_is_tuple:true的话，返回的状态是一个tuple:(c=array([[]]), h=array([[]]):其中c代表Ct的最后时间的输出，h代表Ht最后时间的输出，h是等于最后一个时间的output的
#图三向上指的ht称为output
#此函数返回一个lstm_cell，即图一中的一个A
```

**如果想要设计一个多层的LSTM网络，你就会用到tf.nn.rnn_cell.MultiRNNCell(cells, state_is_tuple=False),这里多层的意思上向上堆叠，而不是按时间展开**

```python
lstm_cell = tf.nn.rnn_cell.MultiRNNCells(cells, state_is_tuple=False)
#cells:一个cell列表，将列表中的cell一个个堆叠起来，如果使用cells=[cell]*4的话，就是四曾，每层cell输入输出结构相同
#如果state_is_tuple:则返回的是 n-tuple，其中n=len(cells): tuple:(c=[batch_size, num_units], h=[batch_size,num_units])
```

**这是，网络已经搭好了，tensorflow提供了一个非常方便的方法来生成初始化网络的state**

```python
initial_state = lstm_cell.zero_state(batch_size, dtype=)
#返回[batch_size, 2*len(cells)],或者[batch_size, s]
#这个函数只是用来生成初始化值的
```

**现在进行时间展开，有两种方法：**
**法一：**

使用现成的接口：
```python
tf.nn.dynamic_rnn(cell, inputs, sequence_length=None, initial_state=None,dtype=None,time_major=False)
#此函数会通过，inputs中的max_time将网络按时间展开
#cell:将上面的lstm_cell传入就可以
#inputs:[batch_size, max_time, size]如果time_major=Flase. [max_time, batch_size, size]如果time_major=True
#sequence_length:是一个list，如果你要输入三句话，且三句话的长度分别是5,10,25,那么sequence_length=[5,10,25]
#返回：（outputs, states）:output，[batch_size, max_time, num_units]如果time_major=False。 [max_time,batch_size,num_units]如果time_major=True。states:[batch_size, 2*len(cells)]或[batch_size,s]
#outputs输出的是最上面一层的输出，states保存的是最后一个时间输出的states
```

**法二**

```python
outputs = []
states = initial_states
with tf.variable_scope("RNN"):
	for time_step in range(max_time):
    	if time_step>0:tf.get_variable_scope().reuse_variables()#LSTM同一曾参数共享，
        (cell_out, state) = lstm_cell(inputs[:,time_step,:], state)
        outputs.append(cell_out)
```

**已经得到输出了，就可以计算loss了,根据你自己的训练目的确定loss函数**

## GRU

**GRU结构图**

来自[Understanding LSTM Networks](http://colah.github.io/posts/2015-08-Understanding-LSTMs/)
![图四](http://upload-images.jianshu.io/upload_images/42741-dd3d241fa44a71c0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)图二
**tenforflow提供了tf.nn.rnn_cell.GRUCell()构建一个GRU单元**
```python
cell = tenforflow提供了tf.nn.rnn_cell.GRUCell(num_units, input_size=None, activation=tanh)
#参考lstm cell 使用
```

            






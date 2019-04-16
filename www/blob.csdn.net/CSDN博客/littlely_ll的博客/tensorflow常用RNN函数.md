# tensorflow常用RNN函数 - littlely_ll的博客 - CSDN博客





2018年03月23日 18:53:17[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：947








# tensorlfow常用RNN函数

`tf.nn.rnn_cell.BasicLSTMCell`：

_*init_*(

num_units,

forget_bias=1.0,

state_is_tuple=True,

activation=None,

reuse=None,

name=None

)
> - num_units:int类型，LSTM 中单元个数(LSTM中包含memory blocks，也就是我们看到的图示的一个小长方形，memory block中有cell和gate，标准LSTM中一个memory block只有一个cell和三个gate，但可以包含多个cell及相应的gate，num_units就是一个memory block包含多少个cell及其相应的gate)
- forget_bias:float，0.0或1.0（默认），
- state_is_tuple:bool，默认True，即得到（cell，hidden_state）二元组。False的话是把（cell，hidden_state）连接起来，不过要deprecated了
- activation:内部状态的激活函数，默认tanh
- reuse; name


`tf.nn.rnn_cell.LSTMCell`:

_*init_*(

num_units,

use_peepholes=False,

cell_clip=None,

initializer=None,

num_proj=None,

proj_clip=None,

num_unit_shards=None,

num_proj_shards=None,

forget_bias=1.0,

state_is_tuple=True,

activation=None,

reuse=None,

name=None

)
> - num_units:int, LSTM cell中单元个数
- use_peepholes: bool, 如果为True，LSTM内部的cells与gates的连接以掌握精确的输出时机
- cell_clip: float, 可选，如果cell state超过这个值，则在cell输出到激活之前被截断
- initializer:权重（weights）及映射（projection）矩阵的初始化
- num_proj:（可选），int，映射矩阵的输出维度
- proj_clip:（可选），float，如果num_proj大于0，并且提供了cell_clip，则映射值截断于[-proj_clip, proj_clip]
- num_unit_shards;num_proj_shards: deprecated
- forget_bias:默认1.0
- state_is_tuple；activation；reuse；name:如上


具体的peephole connections,projection layer和cell clipping可看如下文章：
[Learning Preise Timing with LSTM Reurrent Networks](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.12.2001&rep=rep1&type=pdf)
[Long Short-Term Memory

Based Recurrent Neural Network Architectures for Large Vocabulary

Speech Recognition
`tf.nn.rnn_cell.MultiRNNCell`：

由多个简单cell顺序组成的RNN

_*init*_(

cells,

state_is_tuple=True

)
> - cells：RNN cell的一个列表，由这个顺序组成RNN
- state_is_tuple: bool，如果为True（默认），接受并返回一个n-元组的状态，n=len(cells)，为False的时候已经deprecated了。


`tf.nn.rnn_cell.GRUCell`：

参数：

> - num_units: GRU cell中单元个数
- activation:非线性函数，默认tanh
- reuse
- kernel_initializer:（可选），用于权重和映射矩阵的初始化
- biase_initializer:（可选）,用于偏置项的初始化
- name：层的名字，相同的名称共享相同的权重，为避免错误，需要reuse=True


参考文献：[Learning Phrase Representations using RNN Encoder-Decoder for Statistical Machine Translation](https://arxiv.org/pdf/1406.1078.pdf)

`tf.nn.rnn_cell.DropoutWrapper`:

对给定cell的输入输出加上dropout

_*init*_(

cell,

input_keep_prob=1.0,

output_keep_prob=1.0,

state_keep_prob=1.0,

variational_recurrent=False,

input_size=None,

dtype=None,

seed=None,

dropout_state_filter_visitor=None

)
> - cell:一个RNNcell
- input_keep_prob：0-1之间的float值，如果为1，对输入不添加dropout
- output_keep_prob: 0-1之间的float值，如果为1，对输出不添加dropout
- state_keep_prob: 0-1之间的float值，如果为1，想要dropout其中的cell，还需要设置dropout_state_filter_visitor（默认的是不会dropout cell的）
- variation_recurrent:如果True，相同的dropout模式应用于所有的时间步，如果设置了这个参数，那么input_size也必须设置
- input_size:嵌套的tensorshape，只有variation_recurrent为True并且input_keep_prob < 1才能用
- dtype; seed可选
- dropout_state_filter_visitor:默认除了cell外可以dropout任何项


默认地，dropout应用层与层之间，variation_recurrent设置为True的时候不单可以应用于层层之间，还可用于时间步之间，对于variation_recurrent的详细理解可以参考下面的文献：
[A Theoretically Grounded Application of Dropout in Recurrent Neural Networks](https://arxiv.org/pdf/1512.05287.pdf)
`tf.nn.dynamic_rnn`:

dynamic_rnn(

cell,

inputs,

sequence_length=None,

initial_state=None,

dtype=None,

parallel_iterations=None,

swap_memory=False,

time_major=False,

scope=None

)
> - cell：一个RNNcell
- inputs:输入，每一个batch有相同的输入长度大小，不同的batch可以不同的输入大小
- sequence_length：列表，一个batch中序列的真实长度


下面是Stack Overflow中回答关于dynamic_rnn的输出问题：[Analysis of the output from tf.nn.dynamic_rnn tensorflow function](https://stackoverflow.com/questions/44162432/analysis-of-the-output-from-tf-nn-dynamic-rnn-tensorflow-function)

tf.nn.dynamic_rnn provides two outputs, **outputs** and **state**.

**outputs** contains the output of the RNN cell at every time instant. Assuming the default time_major == False, let’s say you have an input composed of 10 examples with 7 time steps each and a feature vector of size 5 for every time step. Then your input would be 10x7x5 (batch_sizexmax_timexfeatures). Now you give this as an input to a RNN cell with output size 15. Conceptually, each time step of each example is input to the RNN, and you would get a 15-long vector for each of those. So that is what outputs contains, a tensor in this case of size 10x7x15 (batch_sizexmax_timexcell.output_size) with the output of the RNN cell at each time step. If you are only interested in the last output of the cell, you can just slice the time dimension to pick just the last element (e.g. outputs[:, -1, :]).
**state** contains the state of the RNN after processing all the inputs. Note that, unlike outputs, this doesn’t contain information about every time step, but only about the last one (that is, the state after the last one). Depending on your case, the state may or may not be useful. For example, if you have very long sequences, you may not want/be able to processes them in a single batch, and you may need to split them into several subsequences. If you ignore the state, then whenever you give a new subsequence it will be as if you are beginning a new one; if you remember the state, however (e.g. outputting it or storing it in a variable), you can feed it back later (through the initial_state parameter of tf.nn.dynamic_rnn) in order to correctly keep track of the state of the RNN, and only reset it to the initial state (generally all zeros) after you have completed the whole sequences. The shape of state can vary depending on the RNN cell that you are using, but, in general, you have some state for each of the examples (one or more tensors with size batch_sizexstate_size, where state_size depends on the cell type and size).

在他按时间步分割的时候即outputs中最后一句话不妥，因为在取最后一个时间步输出的时候，有的sentence可能比较短，所以outputs[:,-1,:]的输出可能为0，因此，如果取最后的输出，应该用state.h。
```
import tensorflow as tf
import numpy as np
x = np.random.randn(2,5,3)
x[1,4:] = 0
x_len = [5,4]
cell = tf.nn.rnn_cell.BasicLSTMCell(num_units=1, state_is_tuple=True) #num_units可以自己设置多个
outputs, last_states = tf.nn.dynamic_rnn(cell=cell,inputs=x,dtype=tf.float64,sequence_length=x_len)
result = tf.contrib.learn.run_n({"outputs": outputs, "last_states": last_states},n=1,feed_dict=None)
print(result[0])
```
[{'outputs': array([[[-0.02468451],
        [-0.09230891],
        [-0.00885939],
        [-0.08525897],
        [-0.25602909]],

       [[-0.05933624],
        [ 0.12028753],
        [ 0.02201308],
        [ 0.10565564],
        [ 0.        ]]]), 
        'last_states':LSTMStateTuple(c=array([[-0.54874369],
       [ 0.21746937]]), h=array([[-0.25602909],
       [ 0.10565564]]))}]

`tf.nn.bidirectional_dynamic_rnn`:

tf.nn.bidirectional_dynamic_rnn(

cell_fw,

cell_bw,

inputs,

sequence_length=None,

initial_state_fw=None,

initial_state_bw=None,

dtype=None,

parallel_iterations=None,

swap_memory=False,

time_major=False,

scope=None

)
> - cell_fw, cell_bw: 一个RNN Cell的实例
- inputs: RNN的输入
- sequence_length: 输入序列的真实长度


返回一个（outputs, output_states）二元组，默认地，outputs为一个 (output_fw, output_bw)二元组，output_fw或output_bw的tensor的形式为[batch_size, max_time, output_size]。output_states为 一个(output_state_fw, output_state_bw)元组，为双向RNN最终的输出状态。

`tf.layers.dense`:

dense(

inputs,

units,

activation=None,

use_bias=True,

kernel_initializer=None,

bias_initializer=tf.zeros_initializer(),

kernel_regularizer=None,

bias_regularizer=None,

activity_regularizer=None,

kernel_constraint=None,

bias_constraint=None,

trainable=True,

name=None,

reuse=None

)
> - inputs: 一个tensor
- units:int或long输出维度


`tf.layers.dropout`：

dropout(

inputs,

rate=0.5,

noise_shape=None,

seed=None,

training=False,

name=None

)
> - inputs:一个tensor
- rate: dropout比例，例如rate=0.1，则会去掉10%的输入节点，剩下的结点将会1/(1-rate)缩放


`tf.contrib.rnn.AttentionCellWrapper`：

_*init*_(

cell,

attn_length,

attn_size=None,

attn_vec_size=None,

input_size=None,

state_is_tuple=True,

reuse=None

)

添加attention
> - cell: 要添加attention的RNNcell
- attn_length:integer，attention窗口的大小
- attn_size:integer，一个attention vector的大小，默认等于cell.output_size](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.713.6391&rep=rep1&type=pdf)






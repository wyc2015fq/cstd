# tensrflow lstm - Keith - CSDN博客





2016年10月13日 20:48:42[ke1th](https://me.csdn.net/u012436149)阅读数：2381








```python
tf.nn.rnn_cell.LSTMCell.__init__(num_units, input_size=None, use_peepholes=False,
 cell_clip=None, initializer=None, num_proj=None, num_unit_shards=1, num_proj_shards=1,
 forget_bias=1.0, state_is_tuple=False, activation=tanh)
```


- num_units: cell输出的维数（一个lstm的一个单元称为cell）

- state_is_tuple: 设置为true的话，返回（c_state, m_state）， false的话不知道返回啥
- 官网上说 不赞成使用inputsize这个参数， 

- 函数的输出是一个cell




```python
tf.nn.rnn_cell.DropoutWrapper.__init__(cell, input_keep_prob=1.0,
 output_keep_prob=1.0, seed=None)
```


- cell:就用上面代码返回的cell
- input_keep_prob / output_keep_prob :保留连接的比例
- 使用此函数可以使cell 后面接一个dropout
- 函数输出 后接dropout的cell




```python
tf.nn.rnn_cell.MultiRNNCell.__init__(cells, state_is_tuple=False)
```



- 此函数用来搭建 多层的lstm网络
- cells： 一般用 [cell]*n.   n为你想搭的层数
- state_is_tuple: 同上
- 叠lstm的层数
- 返回的也是cell（大的）




```python
tf.nn.dynamic_rnn(cell, inputs, sequence_length=None, initial_state=None, dtype=None,
 parallel_iterations=None, swap_memory=False, time_major=False, scope=None)
```



- cell：搭好的网络， 即上个函数的返回值（是cell就可以）
- inputs：The RNN inputs.If time_major == False (default), this must be a tensor of shape:`[batch_size, max_time, input_size]`.If time_major == True, this must be a tensor of shape:`[max_time, batch_size, input_size]`.  batch_size: ....max_time指的是：
 输入句子中单词的最大数量。input_size:每个单词向量的长度
- 网络是通过 max_time 的值进行按时间展开的。 max_time是 最长句子中单词的个数。
- sequence_size: 是一个向量：向量的每个值代表每个batch的 output的个数（假设batch1，有6个单词， 而max_time是10， 就可以将sequence_size[batch1]=6）
- 即：每个batch的句子中单词的数量（句子的长度）

- 一个batch代表一个句子
- 函数输出：outputs：维数[batch_size, max_time, cell.output_size] , 

- 函数输出：state是每行最后一个state的集合（tensor）， 假设n层 （n, cell.output_size）

![](https://img-blog.csdn.net/20161013204426036?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

参考 ：[点击打开链接](http://http//lan2720.github.io/2016/07/16/%E8%A7%A3%E8%AF%BBtensorflow%E4%B9%8Brnn/)





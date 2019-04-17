# 【Tensorflow】自主实现包含全节点Cell的LSTM层（Cell-Holding LSTM Layer） - CD's Coding - CSDN博客





2018年06月28日 17:17:30[糖果天王](https://me.csdn.net/okcd00)阅读数：186








## 0x00 前言

常用的LSTM，或是双向LSTM，输出的结果通常是以下两个： 

1） outputs，包括所有节点的hidden 

2） 末节点的state，包括末节点的hidden和cell 

大部分任务有这些就足够了，state是随着节点间信息的传递依次变化并容纳更多信息， 

所以通常末状态的cell就囊括了所有信息，不需要中间每个节点的cell信息， 

但如果我们的研究过程中需要用到这些cell该如何是好呢？
近期的任务中，需要每个节点的前后节点cell信息来做某种判断， 

所以属于一个较为特殊的任务，自主实现了一下这个同样也会反馈cell的LSTM， 

哦顺带一提Cell-Holding，是强行为了简称成CHD取的名字（笑）
## 0x01 分析与设计

首先分析源码，看一下通常LSTM层调用使用 `dynamic_rnn` 的实现逻辑， 

原逻辑大概是这样的：

```
outputs = []
state = Cell.zero_state(N, tf.float32)  # state = (hidden, cell)
for input in inputs:
    output, state = Cell(input, state)  # hidden, (hidden, cell) = Cell()
    outputs.append(output)  # outputs.append(hidden)
return outputs, state  # outputs := a list of (hidden)
```

那么其实……我们只需要重新实现一个简化的版本，让cell留下来即可。 

此处使用的逻辑大概是这样的：

```bash
states_case = []
state = Cell.zero_state(N, tf.float32)  # state = (hidden, cell)
for input in inputs:
    output, state = Cell(input, state)  # hidden, (hidden, cell) = Cell()
    outputs.append(output)  # states_case.append((hidden, cell))
return states_case  # states_case := list of (hidden, cell)
```

为了实现这些，就需要做到以下几件事情： 

1） 获取或共享已有LSTM层的BasicLSTMCell 

2） 编写Cell相关计算，保留LSTM计算途中的信息，可自定义获取输出的格式 

3） 采用设计的输出格式使用这些节点信息，以完成其他任务
## 0x02 Source Code

### Advanced LSTM Layer

> 
**[LstmLayer]** in tf_layers 

  首先要在不影响功能的情况下改写原有的LSTM Layer，令其支持获取BasicCell的操作


```python
class LstmLayer(object):
    # based on LSTM Layer, thanks for @lhw446
    def __init__(self, input_dim, num_units, sequence_length=None, bidirection=False, name="lstm"):
        self.input_dim = input_dim
        self.num_units = num_units
        self.bidirection = bidirection
        self.sequence_length = sequence_length
        self.name = name

        # `with ... as...` remains assignment work.
        self.lstm_fw_cell = None
        self.lstm_bw_cell = None

        with tf.name_scope('%s_def' % (self.name)):
            self.lstm_fw_cell = tf.nn.rnn_cell.BasicLSTMCell(self.num_units, state_is_tuple=True)
            if self.bidirection:
                self.lstm_bw_cell = tf.nn.rnn_cell.BasicLSTMCell(self.num_units, state_is_tuple=True)


    def __call__(self, inputs, sequence_length=None, time_major=False,
                 initial_state_fw=None, initial_state_bw=None):
        inputs_shape = tf.shape(inputs)
        inputs = tf.reshape(inputs, [-1, inputs_shape[-2], self.input_dim])
        sequence_length = self.sequence_length if sequence_length is None \
            else tf.reshape(sequence_length, [-1])

        if initial_state_fw is not None:
            initial_state_fw = tf.nn.rnn_cell.LSTMStateTuple(
                tf.reshape(initial_state_fw[0], [-1, self.num_units]),
                tf.reshape(initial_state_fw[1], [-1, self.num_units]))
        if initial_state_bw is not None:
            initial_state_bw = tf.nn.rnn_cell.LSTMStateTuple(
                tf.reshape(initial_state_bw[0], [-1, self.num_units]),
                tf.reshape(initial_state_bw[1], [-1, self.num_units]))

        resh_1 = lambda tensors: tf.reshape(
            tensors, tf.concat([inputs_shape[:-1], [tf.shape(tensors)[-1]]], 0))
        resh_2 = lambda tensors: tf.reshape(
            tensors, tf.concat([inputs_shape[:-2], [tf.shape(tensors)[-1]]], 0))

        with tf.variable_scope('%s_cal' % (self.name)):
            if self.bidirection:
                outputs, output_states = tf.nn.bidirectional_dynamic_rnn(
                    self.lstm_fw_cell, self.lstm_bw_cell, inputs,
                    sequence_length=sequence_length,
                    initial_state_fw=initial_state_fw,
                    initial_state_bw=initial_state_bw,
                    time_major=time_major, dtype=tf.float32)
                # (fw_outputs, bw_outputs)
                outputs = tf.nn.rnn_cell.LSTMStateTuple(resh_1(outputs[0]), resh_1(outputs[1]))
                # ((fw_c_states, fw_m_states), (bw_c_states, bw_m_states))
                output_states = tf.nn.rnn_cell.LSTMStateTuple(
                    tf.nn.rnn_cell.LSTMStateTuple(resh_2(output_states[0][0]), resh_2(output_states[0][1])),
                    tf.nn.rnn_cell.LSTMStateTuple(resh_2(output_states[1][0]), resh_2(output_states[1][1])))
            else:
                outputs, output_states = tf.nn.dynamic_rnn(
                    self.lstm_fw_cell, inputs, sequence_length=sequence_length,
                    initial_state=initial_state_fw,
                    time_major=time_major, dtype=tf.float32)
                outputs = resh_1(outputs)  # (outputs)
                # (c_states, m_states)
                output_states = tf.nn.rnn_cell.LSTMStateTuple(
                    resh_2(output_states[0]), resh_2(output_states[1]))

            return outputs, output_states
```

### Cell-HolDing Layer

> 
**chd_lstm_layer** in network 

  然后基于目标LSTM层，构建使用相同基本单元的scope，设定初始零状态，逐层计算 

  （此处仅剪枝了所有的padding位，没有特意做加速，用了简单的python-like的for循环） 

  （且为了本次实验需要，没有将hidden和cell区分开来，而是直接保存了state整体，可自行修改）
```python
def chd_lstm_layer(self, inputs, target_layer):
    cell = target_layer.lstm_fw_cell

    with tf.variable_scope('%s_cal' % (target_layer.name)):
        # generate initial states for current inputs
        states_case = []
        for batch_idx in range(self.batch_size):
            batch_state_case = []
            state = cell.zero_state(1, tf.float32)
            for time_step in range(self.seg_len[batch_idx]):
                tf_input = inputs[batch_idx, time_step]
                output, _state = cell(
                    tf.reshape(tf_input, [1, -1]), state)
                batch_state_case.append(_state)
                state = _state
            states_case.append(batch_state_case)
        # a nested list of states [batch_size, seg_len]
        return states_case, cell
```

上述是任务需要， 

主要演示了可以简单的循环调用给定LSTM层的Cell进行计算， 

在对齐的情况下还可以通过stack等操作拼成一个tf的矩阵使用。 

其中用作循环迭代次数的参数 `self.batch_size``self.seg_len`等， 

不可以是tf.placeholder，因为range内必须为一个固定的数值而不能为一个占位符（tf.loop不知道能不能做到） 

所以在`feed_dict`前，我做了如下的操作，将这些固定数值作为 `instance_variables` 传给网络以供使用。
```python
def gen_infer_inputs(self, data):
    # data = merge_by_batch_size(batch_data_generate(data))
    self.batch_size = data['cell_lens'].shape[0]
    self.seg_len = data['cell_lens']
    self.can_len = data['candi_mask'].sum(-1)
    return {
        self.input_data: data['input_data'],
        self.cell_lens: data['cell_lens'],
        self.candidates: data['candidates'],
        self.candi_mask: data['candi_mask'],
        self.keep_prob: 1.0,
    }
```

### Further usage on states_case

> 
**others_layer** in network 

  获取了states_case之后，可以用于各个位置的使用 

  下文中给出一个使用案例，此处用于计算相同LSTM序列中，替换其中任意节点为其他节点的输出。
```python
def replace_layer(self, forward_emb, candidate_emb):
    backward_emb = self.get_reverse(forward_emb, rev_length=self.cell_lens + 2)

    fw_states, fw_cell = self.chd_lstm_layer(
        forward_emb, self.forward_lstm)
    bw_states, bw_cell = self.chd_lstm_layer(
        backward_emb, self.backward_lstm)

    hidden_case = []
    for batch_idx in range(self.batch_size):
        batch_case = []
        for time_step in range(self.seg_len[batch_idx]):
            time_case = []
            for candidate_idx in range(self.can_len[batch_idx, time_step]):
                tf_input = candidate_emb[batch_idx, time_step, candidate_idx]
                fw_hidden, _ = fw_cell(
                    tf.reshape(tf_input, [1, -1]),
                    fw_states[batch_idx][time_step])
                bw_hidden, _ = bw_cell(
                    tf.reshape(tf_input, [1, -1]),
                    bw_states[batch_idx][-time_step])
                hidden = tf.concat([fw_hidden, bw_hidden], -1)
                time_case.append(hidden)
            batch_case.append(time_case)
        hidden_case.append(batch_case)
    return hidden_case  # a nested list.
```

## 0x03 后记

`cell`因其持续更新且后者包含前者信息的特性通常不被保存， 

但是 `LSTMCell``RNNCell` 的调用却需要完整的state（包括`hidden`和`cell`）， 

在我们对已经计算完毕的LSTM序列中内部的某些节点有所想法时，就很难回溯了， 

所以说不定这种`layer`也是有一定价值的，目前tensorflow里还没有整合成类似的层， 

所以自行手写了一个，虽说不是太复杂，不过提供了这样一种想法，记录一下~ 

（说不定以后就加了这个层呢~ 到时候我可以指着这篇文章说我早就想到咯^_^）












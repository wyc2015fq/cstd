# 【Reverse】Tensorflow中屏蔽Padding处理变长序列的双向LSTM - CD's Coding - CSDN博客





2018年04月19日 20:11:46[糖果天王](https://me.csdn.net/okcd00)阅读数：1935








## 0x00 前言

通常我们处理双向LSTM的时候，序列长短不一参差不齐，就不可避免的需要进行padding， 

实际上句子长度可能如下所示，为[2, 4, 5]，就需要被padding成这样才能作为矩阵传入计算：

```
array([[1, 2, 0, 0, 0],
       [2, 1, 4, 3, 0],
       [3, 2, 1, 5, 4]], dtype=int32)
```

对于padding的数据做双向LSTM的时候，反向的部分会将`0`也算入， 

虽说对于大多任务中，把padding也算作一个词，做一个padding的embedding， 

也是可以获得不错的效果的，但是存在过长的句子时， 

大多较短句子后面过多的padding会把末状态过于稀释， 

诸如此类，还有各种各样的情况都希望可以双向LSTM可以每个方向都只计算有效长度
## 0x01 简要解释

这里的方法是采用单向LSTM，配以cell_len来控制终止位置， 

正向LSTM正常调用即可，反向LSTM需要对非Padding位进行reverse， 

（即有效长度部分逆转，Padding部分不变） 

这里和通常直接调用的双向LSTM不同，自定义的成分比较多，例如下面实际场景运用中， 

正反向错位合并，普通的LSTM应该不会那么轻松（有点说大话了，至少……看起来没那么明显？）
## 0x02 函数介绍

```python
# Tensorflow.Reverse()
sess = tf.Session()

inp = tf.placeholder(tf.int32, [None, 5])
cell_lens = tf.placeholder(tf.int32, [None])

rseq = tf.reverse_sequence(
            inp, cell_lens,  # 输入矩阵 与 需要作reverse操作的长度
            seq_axis=None,
            batch_axis=None,
            name='reverse_data',  # 取名，方便在计算图中定位
            seq_dim=1, batch_dim=0  # 需要作reverse操作的维度和作为batch的维度
        )

sess.run(rseq, {
        inp: [range(5), range(5), range(5)], 
        cell_lens: [2, 3, 4]
})

"""
# 分别前2、3、4维进行reverse操作
array([[1, 0, 2, 3, 4],
       [2, 1, 0, 3, 4],
       [3, 2, 1, 0, 4]], dtype=int32)
"""
```

## 0x03 实际场景运用节选

> 
这是在一处NLP相关使用场景中运用的案例 

  由于研究相关及保密需求，此处省略数据流部分，仅展示与本文讨论相关要点


```python
class LstmLayer(object):
""" LSTM layer class """
    def __init__(self, num_units, bidirection=False, sequence_length=None, name="lstm"):
        self.num_units = num_units
        self.bidirection = bidirection
        self.sequence_length = tf.reshape(sequence_length, [-1])
        self.name = name

    def __call__(self, inputs, time_major=False):
        with tf.name_scope('{}_cal'.format(self.name)):
            with tf.variable_scope(self.name, reuse=tf.AUTO_REUSE):
                if self.bidirection:
                    lstm_fw_cell = tf.nn.rnn_cell.BasicLSTMCell(self.num_units, state_is_tuple=True)
                    lstm_bw_cell = tf.nn.rnn_cell.BasicLSTMCell(self.num_units, state_is_tuple=True)
                    outputs, output_states = tf.nn.bidirectional_dynamic_rnn(
                        lstm_fw_cell, lstm_bw_cell, inputs,
                        sequence_length=self.sequence_length,
                        time_major=time_major, dtype=tf.float32)
                else:
                    lstm_fw_cell = tf.nn.rnn_cell.BasicLSTMCell(self.num_units, state_is_tuple=True)
                    outputs, output_states = tf.nn.dynamic_rnn(
                        lstm_fw_cell, inputs, sequence_length=self.sequence_length,
                        time_major=time_major, dtype=tf.float32)

            return outputs, output_states


class Network(object):
""" Network for model testing """
    def __init__(self, options=feed_options, is_training=True):
        self.options = options
        self.is_training = is_training

        # LSTM Layer
        self.forward_lstm = LstmLayer(num_units=options.get('lstm_dim'),
                                      sequence_length=self.cell_lens,
                                      name="forward_lstm")

        self.backward_lstm = LstmLayer(num_units=options.get('lstm_dim'),
                                       sequence_length=self.cell_lens,
                                       name="backward_lstm")

    def get_reverse(self, input, seq_dim=1, batch_dim=0):
        """
        # Given this:
        batch_dim = 2
        seq_dim = 0
        input.dims = (8, ?, 4, ...)
        seq_lengths = [7, 2, 3, 5]

        # then slices of input are reversed on seq_dim, but only up to seq_lengths:
        output[0:7, :, 0, :, ...] = input[7:0:-1, :, 0, :, ...]
        output[0:2, :, 1, :, ...] = input[2:0:-1, :, 1, :, ...]
        output[0:3, :, 2, :, ...] = input[3:0:-1, :, 2, :, ...]
        output[0:5, :, 3, :, ...] = input[5:0:-1, :, 3, :, ...]

        # while entries past seq_lens are copied through:
        output[7:, :, 0, :, ...] = input[7:, :, 0, :, ...]
        output[2:, :, 1, :, ...] = input[2:, :, 1, :, ...]
        output[3:, :, 2, :, ...] = input[3:, :, 2, :, ...]
        output[2:, :, 3, :, ...] = input[2:, :, 3, :, ...]
        """

        tf.reverse_sequence(
            input, self.cell_lens,
            name='reverse_data',
            seq_dim=seq_dim,
            batch_dim=batch_dim,
        )

    def lstm_layer(self, forward_emb):
        # [batch, seg_len + 2, emb_dim] -> [batch, seg_len + 2, emb_dim]
        backward_emb = self.get_reverse(forward_emb)

        # [batch, seg_len + 2, emb_dim] x 2 -> [batch, seg_len, lstm_dim] x 2
        _, (c1, f_hidden) = self.forward_lstm(forward_emb)
        _, (c2, b_hidden) = self.backward_lstm(backward_emb)

        # [batch, seg_len, lstm_dim] x 2 -> [batch, seg_len, lstm_dim * 2]
        return tf.concat([f_hidden, self.get_reverse(b_hidden)], -1)
```

此处应用场景中还有卖弄了一个反向错位相加的小伎俩，使用了`tensorflow.reverse()`， 

拿到需求就想出可以通过前后加`<BOS>`、`<EOS>`后，正反向各少两个节点做LSTM隐向量拼接这一点， 

我自己还是有点小骄傲的~~






# 【tensorflow】tf.contrib.rnn.BasicLSTMCell解析 - zkq_1986的博客 - CSDN博客





2018年12月05日 14:09:35[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：147








BasicLSTMCell 是最简单的LSTMCell，源码位于：/tensorflow/contrib/rnn/python/ops/core_rnn_cell_impl.py。
BasicLSTMCell 继承了RNNCell，源码位于：/tensorflow/python/ops/rnn_cell_impl.py
def __init__(self, num_units, forget_bias=1.0, input_size=None,
               state_is_tuple=True, activation=tanh):
Args:
- `num_units`: int, The number of units in the LSTM cell.神经元数量
- `forget_bias`: float, The bias added to forget gates (see above). Must set to `0.0` manually when restoring from CudnnLSTM-trained checkpoints.  遗忘的偏置是0-1的数，1全记得，0全忘记
- `state_is_tuple`: If True, accepted and returned states are 2-tuples of the `c_state` and `m_state`. If False, they are concatenated along the column axis. The latter behavior will soon be deprecated.最好是true,返回元祖。
- `activation`: Activation function of the inner states. Default: `tanh`.    激活函数，默认tanh
- input_size: Deprecated and unused.




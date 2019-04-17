# tensorflow学习笔记（三十七）：如何自定义LSTM的initial state - Keith - CSDN博客





2017年03月21日 17:11:37[ke1th](https://me.csdn.net/u012436149)阅读数：8125
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









# 如何初始化LSTM的state

`LSTM` 需要 `initial state`。一般情况下，我们都会使用 `lstm_cell.zero_state()`来获取 `initial state`。但有些时候，我们想要给 `lstm_cell` 的 `initial state` 赋予我们想要的值，而不是简单的用 `0` 来初始化，那么，应该怎么做呢？ 

当然，当我们设置了`state_is_tuple=False`的时候，是很简单的，当`state_is_tuple=True`的时候，应该怎么做呢？ 

需要用到`LSTMStateTuple`
## LSTMStateTuple(c ,h)

可以把 `LSTMStateTuple()` 看做一个`op`

```python
from tensorflow.contrib.rnn.python.ops.core_rnn_cell_impl import LSTMStateTuple

...
c_state = ...
h_state = ...
# c_state , h_state 都为Tensor
initial_state = LSTMStateTuple(c_state, h_state)
```

当然，`GRU`就没有这么麻烦了，因为`GRU`没有两个`state`。





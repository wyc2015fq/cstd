# 【深度学习】RNN之LSTM原理 - zkq_1986的博客 - CSDN博客





2018年05月18日 14:48:04[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：131








LSTM，long short term memory，长短时记忆元

与GRU相比，它是3个门，GRU是2个门。GRU更简单，易于大规模应用；LSTM准确性比GRU更好。到底用哪个，需要权衡。

![](https://img-blog.csdn.net/20180518144742601)

![](https://img-blog.csdn.net/20180518144755977)

基于Tensorflow的python代码片段：

```python
def unit(x, hidden_memory_tm1):
                previous_hidden_state, c_prev = tf.unstack(hidden_memory_tm1)

                # Input Gate
                i = tf.sigmoid(
                    tf.matmul(x, self.Wi) +
                    tf.matmul(previous_hidden_state, self.Ui) + self.bi
                )

                # Forget Gate
                f = tf.sigmoid(
                    tf.matmul(x, self.Wf) +
                    tf.matmul(previous_hidden_state, self.Uf) + self.bf
                )

                # Output Gate
                o = tf.sigmoid(
                    tf.matmul(x, self.Wog) +
                    tf.matmul(previous_hidden_state, self.Uog) + self.bog
                )

                # New Memory Cell
                c_ = tf.nn.tanh(
                    tf.matmul(x, self.Wc) +
                    tf.matmul(previous_hidden_state, self.Uc) + self.bc
                )

                # Final Memory cell
                c = f * c_prev + i * c_

                # Current Hidden state
                current_hidden_state = o * tf.nn.tanh(c)

                return tf.stack([current_hidden_state, c])
```






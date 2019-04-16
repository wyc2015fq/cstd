# tensorflow实现rnn之tf.nn.dynamic_rnn参数shape介绍 - 一个跳popping的quant的博客 - CSDN博客





2018年04月16日 12:47:13[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：4803
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









使用tensorflow实现rnn或者lstm很方便，只需创建rnn或者lstm神经单元，然后创建网络就可以了，但是rnn或者lstm不同于常规的nn神经网络，因为它是处理时间序列的，所以在进行batch训练时，对数据格式的要求也不一样。

下面举一个预测价格的例子：


### 创建网络

先看一下rnn的最基本结构：

![](https://img-blog.csdn.net/20180415213041908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


代码如下：

```python
def _build_net(self):
        self.x = tf.placeholder(tf.float32, [None, 4])
        self.y = tf.placeholder(tf.float32, [None, 1])

        w = tf.Variable(tf.truncated_normal([self.hidden_size, 1], stddev=0.1))
        b = tf.Variable(tf.constant(0.1, shape=[1]))

     
        input_data = tf.reshape(self.x, [-1, self.stateNum, 1])
        lstm_cell = tf.nn.rnn_cell.BasicLSTMCell(self.hidden_size)
        init_state = lstm_cell.zero_state(self.batchSize, tf.float32)
       
        outputs_rnn, final_state = tf.nn.dynamic_rnn(lstm_cell, input_data, dtype=tf.float32, initial_state=init_state)
        output = tf.reshape(outputs_rnn,[-1,self.hidden_size])
        
        self.prediction = tf.matmul(final_state[1], w) + b

        self.loss = tf.reduce_mean(tf.square(self.y - self.prediction))
        self.train = tf.train.AdamOptimizer(0.001).minimize(self.loss)
```



接受的训练数据是一个[batchSize, 4]的shape，就是用前4个数据预测后一个。

hidden_size是一个lstm或者rnn单元的神经元的个数，也就是结构图中的一个方框A，可以想象其中有这么多个神经元。

**接下来是重点！！！**

为什么要reshape成这种结构？

首先reshape的这个结构是作为tf.nn.dynamic_rnn的参数传入的，我们先看一下这个函数的参数介绍：

```python
cell: An instance of RNNCell.
    inputs: The RNN inputs.
      If `time_major == False` (default), this must be a `Tensor` of shape:
        `[batch_size, max_time, ...]`, or a nested tuple of such
        elements.
      If `time_major == True`, this must be a `Tensor` of shape:
        `[max_time, batch_size, ...]`, or a nested tuple of such
        elements.
      This may also be a (possibly nested) tuple of Tensors satisfying
      this property.  The first two dimensions must match across all the inputs,
      but otherwise the ranks and other shape components may differ.
      In this case, input to `cell` at each time-step will replicate the
      structure of these tuples, except for the time dimension (from which the
      time is taken).
      The input to `cell` at each time step will be a `Tensor` or (possibly
      nested) tuple of Tensors each with dimensions `[batch_size, ...]`.
    sequence_length: (optional) An int32/int64 vector sized `[batch_size]`.
      Used to copy-through state and zero-out outputs when past a batch
      element's sequence length.  So it's more for correctness than performance.
    initial_state: (optional) An initial state for the RNN.
      If `cell.state_size` is an integer, this must be
      a `Tensor` of appropriate type and shape `[batch_size, cell.state_size]`.
      If `cell.state_size` is a tuple, this should be a tuple of
      tensors having shapes `[batch_size, s] for s in cell.state_size`.
    dtype: (optional) The data type for the initial state and expected output.
      Required if initial_state is not provided or RNN state has a heterogeneous
      dtype.
    parallel_iterations: (Default: 32).  The number of iterations to run in
      parallel.  Those operations which do not have any temporal dependency
      and can be run in parallel, will be.  This parameter trades off
      time for space.  Values >> 1 use more memory but take less time,
      while smaller values use less memory but computations take longer.
    swap_memory: Transparently swap the tensors produced in forward inference
      but needed for back prop from GPU to CPU.  This allows training RNNs
      which would typically not fit on a single GPU, with very minimal (or no)
      performance penalty.
    time_major: The shape format of the `inputs` and `outputs` Tensors.
      If true, these `Tensors` must be shaped `[max_time, batch_size, depth]`.
      If false, these `Tensors` must be shaped `[batch_size, max_time, depth]`.
      Using `time_major = True` is a bit more efficient because it avoids
      transposes at the beginning and end of the RNN calculation.  However,
      most TensorFlow data is batch-major, so by default this function
      accepts input and emits output in batch-major form.
    scope: VariableScope for the created subgraph; defaults to "rnn".
```

参数介绍有点长，先看一下关于inputs的介绍，其中说了inputs的格式是[batch_size, max_time, .....]这里的max_time的意思是这个rnn网络在展开的时候有多长，就是图中所示t这么长。然后这个.....的意思就是每次的输入这个x的维度了。这样的话reshape为[-1, 4, 1]就好解释了-1就是不用管这个维度，如果我们的batch_size是50的话，-1这里就被计算为50*4/4/1=50了，也就是分50次输入rnn网络，每次输入长度是4，就是有4个方框，每个方框接受的数据是1维的。

用rnn来做28*28像素的图像识别时可能会reshape为[-1, 28, 28]，也就是每次输入rnn时有28个rnn方框，每个方框接受28个也就是一行图像，这样就把这整个图像输入进来了。




弄个明白了输入，现在再搞一下输出。

```python
Returns:
    A pair (outputs, state) where:

    outputs: The RNN output `Tensor`.

      If time_major == False (default), this will be a `Tensor` shaped:
        `[batch_size, max_time, cell.output_size]`.

      If time_major == True, this will be a `Tensor` shaped:
        `[max_time, batch_size, cell.output_size]`.

      Note, if `cell.output_size` is a (possibly nested) tuple of integers
      or `TensorShape` objects, then `outputs` will be a tuple having the
      same structure as `cell.output_size`, containing Tensors having shapes
      corresponding to the shape data in `cell.output_size`.

    state: The final state.  If `cell.state_size` is an int, this
      will be shaped `[batch_size, cell.state_size]`.  If it is a
      `TensorShape`, this will be shaped `[batch_size] + cell.state_size`.
      If it is a (possibly nested) tuple of ints or `TensorShape`, this will
      be a tuple having the corresponding shapes. If cells are `LSTMCells`
      `state` will be a tuple containing a `LSTMStateTuple` for each cell.
```

输出是两个结果，一个是outputs，一个是state。

outputs输出是一个[batch_size, max_time, cell.out_size]shape的输出。对于我们设计的网络，相对应的就是一个shape[50, 4, 100]其实就是这一个batch中50组输入的数据通过这4个单元每个单元100个神经元的输出。用常规的NN来比较，正常的NN输出应该是一个类似[50, 100]的输出，但是rnn通过展开后，得到的就是[50, 4, 100]的一个结果。

理解了outputs后，再理解state就好说了。这个state就是final state，也就是batch中一个组数据输入后，最后一个单元的神经元的输出，因为设置了100个神经元，所以总的输出就是[50, 100]。在rnn模型图中就是对应最后一个A方框的隐层输出结果。

**对应代码部分：**

代码中因为用了BasicLSTMCells，所以返回结果是一个tuple shape like (cell_state[50, 100], hidden_state[50, 100])，第一个是cell的state输出结果，后面的是隐层的输出结果，都是[50, 100]。之所以又一个cell_state可以参考lstm的结构。

如果是用了BasicRNNCells的话，返回结果是一个[50, 100]的结果，也就是最后一个单元中隐层神经元的输出。











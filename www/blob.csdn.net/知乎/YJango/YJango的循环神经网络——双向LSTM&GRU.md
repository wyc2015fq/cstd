# YJango的循环神经网络——双向LSTM&GRU - 知乎
# 

## **介绍**

双向LSTM和GRU用scan的方式实现

## 任务描述：

本次的代码LV2是紧接着[代码LV1](https://zhuanlan.zhihu.com/p/25821063)的升级版，所学习的内容与先前的一样，不同的是：
- 简单梳理调整了代码结构，方便使用
- 将所有gate的计算并在一个大矩阵乘法下完成提高GPU的利用率
- 除了LSTM（Long-Short Term Memory）以外的cell，还提供了GRU（gate recurrent unit） cell模块
- 双向RNN（可选择任意cell组合）
- 该代码可被用于练习结构改造或实际建模任务

## 定义LSTMcell类
- 目的：LSTMcell包含所有学习所需要的parameters以及每一时刻所要运行的step方法
- 代码：

```
class LSTMcell(object):
  def __init__(self, incoming, D_input, D_cell, initializer,
               f_bias=1.0, L2=False, h_act=tf.tanh, 
               init_h=None, init_c=None):
      # 属性
      self.incoming = incoming # 输入数据
      self.D_input = D_input
      self.D_cell = D_cell
      self.initializer = initializer # 初始化方法
      self.f_bias = f_bias # 遗忘门的初始偏移量
      self.h_act = h_act # 这里可以选择LSTM的hidden state的激活函数
      self.type = 'lstm' # 区分gru
      # 如果没有提供最初的hidden state和memory cell，会全部初始为0
      if init_h is None and init_c is None:
          # If init_h and init_c are not provided, initialize them
          # the shape of init_h and init_c is [n_samples, D_cell]
          self.init_h = tf.matmul(self.incoming[0,:,:], tf.zeros([self.D_input, self.D_cell]))
          self.init_c = self.init_h
          self.previous = tf.stack([self.init_h, self.init_c])
      # LSTM所有需要学习的参数
      # 每个都是[W_x, W_h, b_f]的tuple
      self.igate = self.Gate()
      self.fgate = self.Gate(bias = f_bias)
      self.ogate = self.Gate()
      self.cell = self.Gate()
      # 因为所有的gate都会乘以当前的输入和上一时刻的hidden state
      # 将矩阵concat在一起，计算后再逐一分离，加快运行速度
      # W_x的形状是[D_input, 4*D_cell]
      self.W_x = tf.concat(values=[self.igate[0], self.fgate[0], self.ogate[0], self.cell[0]], axis=1)
      self.W_h = tf.concat(values=[self.igate[1], self.fgate[1], self.ogate[1], self.cell[1]], axis=1)
      self.b = tf.concat(values=[self.igate[2], self.fgate[2], self.ogate[2], self.cell[2]], axis=0)
      # 对LSTM的权重进行L2 regularization
      if L2:
          self.L2_loss = tf.nn.l2_loss(self.W_x) + tf.nn.l2_loss(self.W_h)
  # 初始化gate的函数        
  def Gate(self, bias = 0.001):
      # Since we will use gate multiple times, let's code a class for reusing
      Wx = self.initializer([self.D_input, self.D_cell])
      Wh = self.initializer([self.D_cell, self.D_cell])
      b  = tf.Variable(tf.constant(bias, shape=[self.D_cell]),trainable=True) 
      return Wx, Wh, b
  # 大矩阵乘法运算完毕后，方便用于分离各个gate
  def Slice_W(self, x, n):
      # split W's after computing
      return x[:, n*self.D_cell:(n+1)*self.D_cell]  
  # 每个time step需要运行的步骤
  def Step(self, previous_h_c_tuple, current_x):
      # 分离上一时刻的hidden state和memory cell
      prev_h, prev_c = tf.unstack(previous_h_c_tuple)
      # 统一在concat成的大矩阵中一次完成所有的gates计算
      gates = tf.matmul(current_x, self.W_x) + tf.matmul(prev_h, self.W_h) + self.b
      # 分离输入门
      i = tf.sigmoid(self.Slice_W(gates, 0))
      # 分离遗忘门
      f = tf.sigmoid(self.Slice_W(gates, 1))
      # 分离输出门
      o = tf.sigmoid(self.Slice_W(gates, 2))
      # 分离新的更新信息
      c = tf.tanh(self.Slice_W(gates, 3))
      # 利用gates进行当前memory cell的计算
      current_c = f*prev_c + i*c
      # 利用gates进行当前hidden state的计算
      current_h = o*self.h_act(current_c)
      return tf.stack([current_h, current_c])
```

## 定义GRUcell类
- 代码：

```
class GRUcell(object):
  def __init__(self, incoming, D_input, D_cell, initializer, L2=False, init_h=None):
      # 属性
      self.incoming = incoming
      self.D_input = D_input
      self.D_cell = D_cell
      self.initializer = initializer
      self.type = 'gru'
      # 如果没有提供最初的hidden state，会初始为0
      # 注意GRU中并没有LSTM中的memory cell，其功能是由hidden state完成的
      if init_h is None:
          # If init_h is not provided, initialize it
          # the shape of init_h is [n_samples, D_cell]
          self.init_h = tf.matmul(self.incoming[0,:,:], tf.zeros([self.D_input, self.D_cell]))
          self.previous = self.init_h
      # 如果没有提供最初的hidden state，会初始为0
      # 注意GRU中并没有LSTM中的memory cell，其功能是由hidden state完成的
      self.rgate = self.Gate()
      self.ugate = self.Gate()
      self.cell = self.Gate()
      # 因为所有的gate都会乘以当前的输入和上一时刻的hidden state
      # 将矩阵concat在一起，计算后再逐一分离，加快运行速度
      # W_x的形状是[D_input, 3*D_cell]
      self.W_x = tf.concat(values=[self.rgate[0], self.ugate[0], self.cell[0]], axis=1)
      self.W_h = tf.concat(values=[self.rgate[1], self.ugate[1], self.cell[1]], axis=1)
      self.b = tf.concat(values=[self.rgate[2], self.ugate[2], self.cell[2]], axis=0)
      # 对LSTM的权重进行L2 regularization
      if L2:
          self.L2_loss = tf.nn.l2_loss(self.W_x) + tf.nn.l2_loss(self.W_h)
  # 初始化gate的函数   
  def Gate(self, bias = 0.001):
      # Since we will use gate multiple times, let's code a class for reusing
      Wx = self.initializer([self.D_input, self.D_cell])
      Wh = self.initializer([self.D_cell, self.D_cell])
      b  = tf.Variable(tf.constant(bias, shape=[self.D_cell]),trainable=True) 
      return Wx, Wh, b
  # 大矩阵乘法运算完毕后，方便用于分离各个gate
  def Slice_W(self, x, n):
      # split W's after computing
      return x[:, n*self.D_cell:(n+1)*self.D_cell]  
  # 每个time step需要运行的步骤
  def Step(self, prev_h, current_x):
      # 分两次，统一在concat成的大矩阵中完成gates所需要的计算
      Wx = tf.matmul(current_x, self.W_x) + self.b
      Wh = tf.matmul(prev_h, self.W_h)
      # 分离和组合reset gate
      r = tf.sigmoid(self.Slice_W(Wx, 0) + self.Slice_W(Wh, 0))
      # 分离和组合update gate
      u = tf.sigmoid(self.Slice_W(Wx, 1) + self.Slice_W(Wh, 1))
      # 分离和组合新的更新信息
      # 注意GRU中，在这一步就已经有reset gate的干涉了
      c = tf.tanh(self.Slice_W(Wx, 2) + r*self.Slice_W(Wh, 2))
      # 计算当前hidden state，GRU将LSTM中的input gate和output gate的合设成1，
      # 用update gate完成两者的工作
      current_h = (1-u)*prev_h + u*c
      return current_h
```

## 定义RNN函数
- 目的：用于接受cell的实例，并用scan计算所有time steps的hidden states
- 代码：

```
def RNN(cell, cell_b=None, merge='sum'):
  """
  该函数接受的数据需要是[n_steps, n_sample, D_output],
  函数的输出也是[n_steps, n_sample, D_output].
  如果输入数据不是[n_steps, n_sample, D_input],
  使用'inputs_T = tf.transpose(inputs, perm=[1,0,2])'.
  """
  # 正向rnn的计算
  hstates = tf.scan(fn = cell.Step,
                  elems = cell.incoming,
                  initializer = cell.previous,
                  name = 'hstates')
  # lstm的step经过scan计算后会返回4维tensor，
  # 其中[:,0,:,:]表示hidden state，
  # [:,1,:,:]表示memory cell，这里只需要hidden state
  if cell.type == 'lstm':
      hstates = hstates[:,0,:,:]
  # 如果提供了第二个cell，将进行反向rnn的计算
  if cell_b is not None:
      # 将输入数据变为反向
      incoming_b = tf.reverse(cell.incoming, axis=[0])
      # scan计算反向rnn
      b_hstates_rev = tf.scan(fn = cell_b.Step,
                  elems = incoming_b,
                  initializer = cell_b.previous, # 每个cell自带的初始值
                  name = 'b_hstates')
      if cell_b.type == 'lstm':
          b_hstates_rev = b_hstates_rev[:,0,:,:]
      # 用scan计算好的反向rnn需要再反向回来与正向rnn所计算的数据进行合并
      b_hstates = tf.reverse(b_hstates_rev, axis=[0])
      # 合并方式可以选择直接相加，也可以选择concat
      if merge == 'sum':
          hstates = hstates + b_hstates
      else:
          hstates = tf.concat(values=[hstates, b_hstates], axis=2)
  return hstates
```

- 解释：可以使用两个GRU cell进行双向rnn的就算，也可以混搭网络构建
- 目的：这里演示的是两层relu feedforward layers后，接一层双向GRU-RNN，最后再接两层relu feedforward layers。
- 效果图：
![](https://pic4.zhimg.com/v2-338d014ff709c0f35ece69610a9b31fb_b.jpg)- 代码：

```python
D_input = 
D_label = 
learning_rate = 7e-5
num_units=
L2_penalty = 1e-4
inputs = tf.placeholder(tf.float32, [None, None, D_input], name="inputs")
labels = tf.placeholder(tf.float32, [None, D_label], name="labels")
# 保持多少节点不被dropout掉
drop_keep_rate = tf.placeholder(tf.float32, name="dropout_keep")
# 用于reshape
n_steps = tf.shape(inputs)[]
n_samples = tf.shape(inputs)[]
# 将输入数据从[n_samples, n_steps, D_input]，reshape成[n_samples*n_steps, D_input]
# 用于feedforward layer的使用
re1 = tf.reshape(inputs, [-, D_input])
# 第一层
Wf0 = weight_init([D_input, num_units])
bf0 = bias_init([num_units])
h1 = tf.nn.relu(tf.matmul(re1, Wf0) + bf0)
# dropout
h1d = tf.nn.dropout(h1, drop_keep_rate)
# 第二层
Wf1 = weight_init([num_units, num_units])
bf1 = bias_init([num_units])
h2 = tf.nn.relu(tf.matmul(h1d, Wf1) + bf1)
# dropout
h2d = tf.nn.dropout(h2, drop_keep_rate)
# 将输入数据从[n_samples*n_steps, D_input]，reshape成[n_samples, n_steps, D_input]
# 用于双向rnn layer的使用
re2 = tf.reshape(h2d, [n_samples,n_steps, num_units])
# 将数据从[n_samples, n_steps, D_input]，转换成[n_steps, n_samples, D_input]
inputs_T = tf.transpose(re2, perm=[,,])
# 实例rnn的正向cell，这里使用的是GRUcell
rnn_fcell = GRUcell(inputs_T, num_units, num_units, orthogonal_initializer)
# 实例rnn的反向cell
rnn_bcell = GRUcell(inputs_T, num_units, num_units, orthogonal_initializer)
# 将两个cell送给scan里计算，并使用sum的方式合并两个方向所计算的数据
rnn0 = RNN(rnn_fcell, rnn_bcell)
# 将输入数据从[n_samples, n_steps, D_input]，reshape成[n_samples*n_steps, D_input]
# 用于feedforward layer的使用
rnn1 = tf.reshape(rnn0, [-, num_units])
# dropout
rnn2 = tf.nn.dropout(rnn1, drop_keep_rate)
# 第三层
W0 = weight_init([num_units, num_units])
b0 = bias_init([num_units])
rnn3 = tf.nn.relu(tf.matmul(rnn2, W0) + b0)
rnn4 = tf.nn.dropout(rnn3, drop_keep_rate)
# 第四层
W1 = weight_init([num_units, num_units])
b1 = bias_init([num_units])
rnn5 = tf.nn.relu(tf.matmul(rnn4, W1) + b1)
rnn6 = tf.nn.dropout(rnn5, drop_keep_rate)
# 输出层
W = weight_init([num_units, D_label])
b = bias_init([D_label])
output = tf.matmul(rnn6, W) + b
# loss
loss=tf.reduce_mean((output-labels)**)
L2_total = tf.nn.l2_loss(Wf0) + tf.nn.l2_loss(Wf1)+ tf.nn.l2_loss(W0) + tf.nn.l2_loss(W1) + tf.nn.l2_loss(W)#+ rnn_fcell.L2_loss + rnn_bcell.L2_loss 
# 训练所需的
train_step = tf.train.AdamOptimizer(learning_rate).minimize(loss + L2_penalty*L2_total)
```

## 训练

剩下的代码就和[代码LV1](https://link.zhihu.com/?target=https%3A//yjango.gitbooks.io/superorganism/content/%25E4%25BB%25A3%25E7%25A0%2581%25E6%25BC%2594%25E7%25A4%25BA.html)的相同了， 大家可以结合tensorboard来记录和分析所学习的权重矩阵和loss的下降等。使用方式请参考[代码演示LV3](https://link.zhihu.com/?target=https%3A//yjango.gitbooks.io/superorganism/content/%25E4%25BB%25A3%25E7%25A0%2581%25E6%25BC%2594%25E7%25A4%25BAlv3.html)，也可以和[代码演示LV3](https://link.zhihu.com/?target=https%3A//yjango.gitbooks.io/superorganism/content/%25E4%25BB%25A3%25E7%25A0%2581%25E6%25BC%2594%25E7%25A4%25BAlv3.html)中的代码结合着使用，根据自己的需要注意reshape和transpose即可。完整代码在我的[github](https://link.zhihu.com/?target=https%3A//github.com/YJango/tensorflow/blob/master/RNN/BGRU_BLSTM_scan_LV2.ipynb)上。

## 效果
- loss：训练集的loss在0.022，验证集的loss在0.222，比feedforward要好很多
- 效果图：另外预测的轨迹也十分的平滑
![](https://pic2.zhimg.com/v2-8eeb0cba9259ea5d9083bb41dd0651bd_b.jpg)![](https://pic2.zhimg.com/v2-feb75672c765888a371f7decf8ac2f11_b.jpg)

## 其他
- 速度：将所有gates的参数并在一起处理再分离可以节省很多时间，代价自然是更多的memory
- rnn的dropout：每个gate其实也是一个再cell内部的具有物理意义的神经网络，那么同样也是可以利用dropout来防止gate在拟合物理意义时过拟合。
- 多层rnn：可以rnn层之后以相同的方式再来一层双向或单向rnn，比如：

```
# 第一层双向rnn
rnn_fcell = GRUcell(inputs_T, num_units, num_units, orthogonal_initializer)
rnn_bcell = GRUcell(inputs_T, num_units, num_units, orthogonal_initializer)
rnn0 = RNN(rnn_fcell, rnn_bcell)
# 第二层双向rnn
rnn_fcell2 = GRUcell(inputs_T, num_units, num_units, orthogonal_initializer)
rnn_bcell2 = GRUcell(inputs_T, num_units, num_units, orthogonal_initializer)
rnn02 = RNN(rnn_fcell2, rnn_bcell2)
# 后续处理
rnn1 = tf.reshape(rnn02, [-1, num_units])
```


# YJango的循环神经网络——scan实现LSTM - 知乎
# 

## **介绍**

上一节在《[循环神经网络——实现LSTM](https://link.zhihu.com/?target=https%3A//yjango.gitbooks.io/superorganism/content/lstmgru.html)》中介绍了循环神经网络目前最流行的实现方法LSTM和GRU，这一节就演示如何利用Tensorflow来搭建LSTM网络。
代码LV1是指本次的演示是最核心的code，并没有多余的功能。
为了更深刻的理解LSTM的结构，这次所用的并非是tensorflow自带的rnn_cell类，而是从新编写，并且用scan来实现graph里的loop (动态RNN)。

## **任务描述：**

这次所要学习的模型依然是[代码演示LV3](https://link.zhihu.com/?target=https%3A//yjango.gitbooks.io/superorganism/content/%25E4%25BB%25A3%25E7%25A0%2581%25E6%25BC%2594%25E7%25A4%25BAlv3.html)中的用声音来预测口腔移动，没有阅读的朋友请先阅读链接中的章节对于任务的描述。同时拿链接中的前馈神经网络与循环神经网络进行比较。

## **处理训练数据**
- 目的：减掉每句数据的平均值，除以每句数据的标准差，降低模型拟合难度。
- 代码：

```
# 所需库包
import tensorflow as tf
import numpy as np
import time
import matplotlib.pyplot as plt
%matplotlib inline
# 直接使用在代码演示LV3中定义的function
def Standardize(seq):
  #subtract mean
  centerized=seq-np.mean(seq, axis = 0)
  #divide standard deviation
  normalized=centerized/np.std(centerized, axis = 0)
  return normalized
# 读取输入和输出数据
mfc=np.load('X.npy')
art=np.load('Y.npy')
totalsamples=len(mfc)
# 20%的数据作为validation set
vali_size=0.2
# 将每个样本的输入和输出数据合成list，再将所有的样本合成list
# 其中输入数据的形状是[n_samples, n_steps, D_input]
# 其中输出数据的形状是[n_samples, D_output]
def data_prer(X, Y):
  D_input=X[0].shape[1]
  data=[]
  for x,y in zip(X,Y):
      data.append([Standardize(x).reshape((1,-1,D_input)).astype("float32"),
                   Standardize(y).astype("float32")])
  return data
# 处理数据
data=data_prer(mfc, art)
# 分训练集与验证集
train=data[int(totalsamples*vali_size):]
test=data[:int(totalsamples*vali_size)]
```

- 示意图：1，2，3，4，5表示list中的每个元素，而每个元素又是一个长度为2的list。
![](https://pic3.zhimg.com/v2-2ffadd794aef7201d3a1a2e598a490ce_b.jpg)

- 解释：比如全部数据有100个序列，如果设定每个input的形状就是[1, n_steps, D_input]，那么处理后的list的长度就是100，这样的数据使用的是SGD的更新方式。而如果想要使用mini-batch GD，将batch size(也就是n_samples)的个数为2，那么处理后的list的长度就会是50，每次网络训练时就会同时计算2个样本的梯度并用均值来更新权重。 因为每句语音数据的时间长短都不相同，如果使用3维tensor，需要大量的zero padding，所以将n_samples设成1。但是这样处理的缺点是：只能使用SGD，无法使用mini-batch GD。如果想使用mini-batch GD，需要几个n_steps长度相同的样本并在一起形成3维tensor（不等长时需要zero padding，如下图）。
- 演示图：v表示一个维度为39的向量，序列1的n_steps的长度为3，序列2的为7，如果想把这三个序列并成3维tensor，就需要选择最大的长度作为n_steps的长度，将不足该长度的序列补零（都是0的39维的向量）。最后会形成shape为[3,7,39]的一个3维tensor。
![](https://pic3.zhimg.com/v2-918f052df1c52710ee2d26ffe5af441a_b.jpg)

权重初始化方法
- 目的：合理的初始化权重，可以降低网络在学习时卡在鞍点或极小值的损害，增加学习速度和效果
- 代码：

```
def weight_init(shape):
  initial = tf.random_uniform(shape,minval=-np.sqrt(5)*np.sqrt(1.0/shape[0]), maxval=np.sqrt(5)*np.sqrt(1.0/shape[0]))
  return tf.Variable(initial,trainable=True)
# 全部初始化成0
def zero_init(shape):
  initial = tf.Variable(tf.zeros(shape))
  return tf.Variable(initial,trainable=True)
# 正交矩阵初始化
def orthogonal_initializer(shape,scale = 1.0):
  #https://github.com/Lasagne/Lasagne/blob/master/lasagne/init.py
  scale = 1.0
  flat_shape = (shape[0], np.prod(shape[1:]))
  a = np.random.normal(0.0, 1.0, flat_shape)
  u, _, v = np.linalg.svd(a, full_matrices=False)
  q = u if u.shape == flat_shape else v
  q = q.reshape(shape) #this needs to be corrected to float32
  return tf.Variable(scale * q[:shape[0], :shape[1]],trainable=True, dtype=tf.float32)
def bias_init(shape):
  initial = tf.constant(0.01, shape=shape)
  return tf.Variable(initial)
# 洗牌
def shufflelists(data):
  ri=np.random.permutation(len(data))
  data=[data[i] for i in ri]
  return data
```

- 解释：其中shufflelists是用于洗牌重新排序list的。正交矩阵初始化是有利于gated_rnn的学习的方法。

## 定义LSTM类
- 属性：使用class类来定义是因为LSTM中有大量的参数，定义成属性方便管理。
- 代码：在init中就将所有需要学习的权重全部定义成属性

```
class LSTMcell(object):
  def __init__(self, incoming, D_input, D_cell, initializer, f_bias=1.0):

      # var
      # incoming是用来接收输入数据的，其形状为[n_samples, n_steps, D_input]
      self.incoming = incoming
      # 输入的维度
      self.D_input = D_input
      # LSTM的hidden state的维度，同时也是memory cell的维度
      self.D_cell = D_cell
      # parameters
        # 输入门的 三个参数
        # igate = W_xi.* x + W_hi.* h + b_i
      self.W_xi = initializer([self.D_input, self.D_cell])
      self.W_hi = initializer([self.D_cell, self.D_cell])
      self.b_i  = tf.Variable(tf.zeros([self.D_cell])) 
        # 遗忘门的 三个参数 
        # fgate = W_xf.* x + W_hf.* h + b_f
      self.W_xf = initializer([self.D_input, self.D_cell])
      self.W_hf = initializer([self.D_cell, self.D_cell])
      self.b_f  = tf.Variable(tf.constant(f_bias, shape=[self.D_cell])) 
        # 输出门的 三个参数
        # ogate = W_xo.* x + W_ho.* h + b_o
      self.W_xo = initializer([self.D_input, self.D_cell])
      self.W_ho = initializer([self.D_cell, self.D_cell])
      self.b_o  = tf.Variable(tf.zeros([self.D_cell])) 
        # 计算新信息的三个参数
        # cell = W_xc.* x + W_hc.* h + b_c
      self.W_xc = initializer([self.D_input, self.D_cell])
      self.W_hc = initializer([self.D_cell, self.D_cell])
      self.b_c  = tf.Variable(tf.zeros([self.D_cell]))  

      # 最初时的hidden state和memory cell的值，二者的形状都是[n_samples, D_cell]
      # 如果没有特殊指定，这里直接设成全部为0
      init_for_both = tf.matmul(self.incoming[:,0,:], tf.zeros([self.D_input, self.D_cell]))
      self.hid_init = init_for_both
      self.cell_init = init_for_both
      # 所以要将hidden state和memory并在一起。
      self.previous_h_c_tuple = tf.stack([self.hid_init, self.cell_init])
      # 需要将数据由[n_samples, n_steps, D_cell]的形状变成[n_steps, n_samples, D_cell]的形状
      self.incoming = tf.transpose(self.incoming, perm=[1,0,2])
```

- 解释：将hidden state和memory并在一起，以及将输入的形状变成[n_steps, n_samples, D_cell]是为了满足tensorflow中的scan的特点，后面会提到。
- 每步计算方法：定义一个function，用于制定每一个step的计算。
- 代码：

```
def one_step(self, previous_h_c_tuple, current_x):

      # 再将hidden state和memory cell拆分开
      prev_h, prev_c = tf.unstack(previous_h_c_tuple)
      # 这时，current_x是当前的输入，
      # prev_h是上一个时刻的hidden state
      # prev_c是上一个时刻的memory cell

      # 计算输入门
      i = tf.sigmoid(
          tf.matmul(current_x, self.W_xi) + 
          tf.matmul(prev_h, self.W_hi) + 
          self.b_i)
      # 计算遗忘门
      f = tf.sigmoid(
          tf.matmul(current_x, self.W_xf) + 
          tf.matmul(prev_h, self.W_hf) + 
          self.b_f)
      # 计算输出门
      o = tf.sigmoid(
          tf.matmul(current_x, self.W_xo) + 
          tf.matmul(prev_h, self.W_ho) + 
          self.b_o)
      # 计算新的数据来源
      c = tf.tanh(
          tf.matmul(current_x, self.W_xc) + 
          tf.matmul(prev_h, self.W_hc) + 
          self.b_c)
      # 计算当前时刻的memory cell 
      current_c = f*prev_c + i*c
      # 计算当前时刻的hidden state
      current_h = o*tf.tanh(current_c)
      # 再次将当前的hidden state和memory cell并在一起返回
      return tf.stack([current_h, current_c])
```

- 解释：将上一时刻的hidden state和memory拆开，用于计算后，所出现的新的当前时刻的hidden state和memory会再次并在一起作为该function的返回值，同样是为了满足scan的特点。定义该function后，LSTM就已经完成了。one_step方法会使用LSTM类中所定义的parameters与当前时刻的输入和上一时刻的hidden state与memory cell计算当前时刻的hidden state和memory cell。
- scan：使用scan逐次迭代计算所有timesteps，最后得出所有的hidden states进行后续的处理。
- 代码：  

```
def all_steps(self):
      # 输出形状 : [n_steps, n_sample, D_cell]
      hstates = tf.scan(fn = self.one_step,
                        elems = self.incoming, #形状为[n_steps, n_sample, D_input]
                        initializer = self.previous_h_c_tuple,
                        name = 'hstates')[:,0,:,:] 
      return hstates
```

- 解释：scan接受的fn, elems, initializer有以下要求：
- fn：第一个输入是上一时刻的输出（需要与fn的返回值保持一致），第二个输入是当前时刻的输入。
- elems：scan方法每一步都会沿着所要处理的tensor的第一个维进行一次一次取值，所以要将数据由[n_samples, n_steps, D_cell]的形状变成[n_steps, n_samples, D_cell]的形状。
- initializer：初始值，需要与fn的第一个输入和返回值保持一致。
- scan的返回值在上例中是[n_steps, 2, n_samples, D_cell]，其中第二个维度的2是由hidden state和memory cell组成的。

## 构建网络
- 代码：

```
D_input = 39
D_label = 24
learning_rate = 7e-5
num_units=1024
# 样本的输入和标签
inputs = tf.placeholder(tf.float32, [None, None, D_input], name="inputs")
labels = tf.placeholder(tf.float32, [None, D_label], name="labels")
# 实例LSTM类
rnn_cell = LSTMcell(inputs, D_input, num_units, orthogonal_initializer)
# 调用scan计算所有hidden states
rnn0 = rnn_cell.all_steps()
# 将3维tensor [n_steps, n_samples, D_cell]转成 矩阵[n_steps*n_samples, D_cell]
# 用于计算outputs
rnn = tf.reshape(rnn0, [-1, num_units])
# 输出层的学习参数
W = weight_init([num_units, D_label])
b = bias_init([D_label])
output = tf.matmul(rnn, W) + b
# 损失
loss=tf.reduce_mean((output-labels)**2)
# 训练所需
train_step = tf.train.AdamOptimizer(learning_rate).minimize(loss)
```

- 解释：以hard coding的方式直接构建一个网络，输入是39维，第一个隐藏层也就是RNN-LSTM，1024维，而输出层又将1024维的LSTM的输出变换到24维与label对应。
- 注： 这个网络并不仅仅取序列的最后一个值，而是要用所有timestep的值与实际轨迹进行比较计算loss

## 训练网络
- 代码：

```
# 建立session并实际初始化所有参数
sess = tf.InteractiveSession()
tf.global_variables_initializer().run()
# 训练并记录
def train_epoch(EPOCH):
  for k in range(EPOCH):
      train0=shufflelists(train)
      for i in range(len(train)):
          sess.run(train_step,feed_dict={inputs:train0[i][0],labels:train0[i][1]})
      tl=0
      dl=0
      for i in range(len(test)):
          dl+=sess.run(loss,feed_dict={inputs:test[i][0],labels:test[i][1]})
      for i in range(len(train)):
          tl+=sess.run(loss,feed_dict={inputs:train[i][0],labels:train[i][1]})
      print(k,'train:',round(tl/83,3),'test:',round(dl/20,3))
t0 = time.time()
train_epoch(10)
t1 = time.time()
print(" %f seconds" % round((t1 - t0),2))
# 训练10次后的输出和时间
(0, 'train:', 0.662, 'test:', 0.691)
(1, 'train:', 0.558, 'test:', 0.614)
(2, 'train:', 0.473, 'test:', 0.557)
(3, 'train:', 0.417, 'test:', 0.53)
(4, 'train:', 0.361, 'test:', 0.504)
(5, 'train:', 0.327, 'test:', 0.494)
(6, 'train:', 0.294, 'test:', 0.476)
(7, 'train:', 0.269, 'test:', 0.468)
(8, 'train:', 0.244, 'test:', 0.452)
(9, 'train:', 0.226, 'test:', 0.453)
563.110000 seconds
```

- 解释：由于上文的LSTM是非常直接的编写方式，并不高效，在实际使用中会花费较长时间。

## **预测效果**
- 代码：pY=sess.run(output,feed_dict={inputs:test[10][0]})
plt.plot(pY[:,8])
plt.plot(test[10][1][:,8])
plt.title('test')
plt.legend(['predicted','real'])
- 解释：plot出一个样本中的维度的预测效果与真是轨迹进行对比
- 效果图：
![](https://pic3.zhimg.com/v2-f6a81764fd65b894514dfcb4137f248a_b.jpg)![](https://pic2.zhimg.com/v2-6f015b68033e1158702857617109679d_b.jpg)


## 总结说明

该文是尽可能只展示LSTM最核心的部分（只训练了10次，有兴趣的朋友可以自己多训练几次），帮助大家理解其工作方式而已，完整代码可以从我的github中[LSTM_lV1](https://link.zhihu.com/?target=https%3A//github.com/YJango/tensorflow/blob/master/RNN/LSTM_scan_LV1.ipynb)中找到。
该LSTM由于运行效率并不高，下一篇会稍微进行改动加快运行速度，并整理结构方便使用GRU以及多层RNN的堆叠以及双向RNN，同时加入其他功能。


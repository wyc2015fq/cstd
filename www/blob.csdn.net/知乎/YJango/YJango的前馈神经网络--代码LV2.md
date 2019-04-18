# YJango的前馈神经网络--代码LV2 - 知乎
# 

## **介绍**

依旧是与[YJango的前馈神经网络--代码LV1](https://zhuanlan.zhihu.com/p/27853521)相同的任务，但是这次要将程序增加一些“枝叶”，好用于记录分析。全部代码在[github](https://link.zhihu.com/?target=https%3A//github.com/YJango/tensorflow/blob/master/XOR_gate.ipynb)上。 

其实真正使用的时候并不需要像下面那样自己编写。可以选择利用[TensorLayer](https://link.zhihu.com/?target=http%3A//tensorlayercn.readthedocs.io/zh/latest/)、[TFLearn](https://link.zhihu.com/?target=http%3A//tflearn.org/)这样写在TensorFlow之上的工具包。下面的操作也就是TensorLayer等所做的。

但使用该便利工具的前提是已了解底层内容，在随后的使用中可以自由更改网络底层结构，而无需依赖于TensorLayer等工具。原因在于神经网络仍在不断发展，每天都会有很多新的方法和结构被提出。这些都需要能够改造网络结构的能力。

更重要的是，对底层的了解可以允许我们迅速发现训练过程中的问题，并及时做出调整。

## **构建模块**
- **计算结构图**：这次加入了L2和dropout。最终的loss是由L2_loss和Sum of Squared Error（就是上篇的loss）相加组成的。
![](https://pic2.zhimg.com/v2-9422a6c1b57a034965581d14bbbce8e9_b.png)

- **step 0**: 为方便以后使用，将所有操作都建到一个类中。并且建立初始化函数：

```
class FNN(object):
  """Build a general FeedForward neural network
  Parameters
  ----------
  learning_rate : float
  drop_out : float
  Layers : list
      The number of layers
  N_hidden : list
      The numbers of nodes in layers
  D_input : int
      Input dimension
  D_label : int
      Label dimension
  Task_type : string
      'regression' or 'classification'
  L2_lambda : float

  """
  def __init__(self, learning_rate, drop_keep, Layers, N_hidden,
               D_input, D_label, Task_type='regression', L2_lambda=0.0):

      # 全部共有属性
      self.learning_rate = learning_rate
      self.drop_keep = drop_keep
      self.Layers = Layers
      self.N_hidden = N_hidden
      self.D_input = D_input
      self.D_label = D_label
      # 类型控制loss函数的选择
      self.Task_type = Task_type
      # L2 regularization的惩罚强弱，过高会使得输出都拉向0
      self.L2_lambda = L2_lambda
      # 用于存放所累积的每层l2 regularization
      self.l2_penalty = tf.constant(0.0)
```

- **解释 0**：该类的目的是建立一个通用的feedforward神经网络生成类。在以后的使用中只需要调节参数就可以改变网络内容，而无需重新编写。
- **step 1**: 建立 输入 ![x](https://www.zhihu.com/equation?tex=x) 和输出 ![y](https://www.zhihu.com/equation?tex=y) 的容器，但是这次加入name_scope，为的是像上图一样生成一个结构图：

```
# 用于生成tensorflow缩放图的,括号里起名字
      with tf.name_scope('Input'):
          self.inputs = tf.placeholder(tf.float32, [None, D_input], name="inputs")
      with tf.name_scope('Label'):
          self.labels = tf.placeholder(tf.float32, [None, D_label], name="labels")
      with tf.name_scope('keep_rate'):
          self.drop_keep_rate = tf.placeholder(tf.float32, name="dropout_keep")

      # 初始化的时候直接生成，build方法是后面会建立的
      self.build('F')
```

- **解释 1**：“with tf.name_scope('keep_rate'):”下的内容都会被表示成一个方框。可以打开查看详细内容。
- **step 2**: 由于所有层都需要该初始化，所以建函数方便以后直接调用。

```
def weight_init(self,shape):
      # shape : list [in_dim, out_dim]
      # can change initialization here
      initial = tf.truncated_normal(shape, stddev=0.1)
      return tf.Variable(initial)

  def bias_init(self,shape):
      # can change initialization here
      initial = tf.constant(0.1, shape=shape)
      return tf.Variable(initial)
```

- **解释 2**： 这里的初始化方式是固定的。可以在函数中另加入一个input来控制初始化方式。上面的shape接受的都是list。
- **step 3**: 用于记录和统计训练中的数据.

```
def variable_summaries(self, var, name):
      with tf.name_scope(name+'_summaries'):
          mean = tf.reduce_mean(var)
          tf.scalar_summary('mean/' + name, mean)
      with tf.name_scope(name+'_stddev'):
          stddev = tf.sqrt(tf.reduce_mean(tf.square(var - mean)))
      # 记录每次训练后变量的数值变化
      tf.scalar_summary('_stddev/' + name, stddev)
      tf.scalar_summary('_max/' + name, tf.reduce_max(var))
      tf.scalar_summary('_min/' + name, tf.reduce_min(var))
      tf.histogram_summary(name, var)
```

- **解释 3**： 上面的函数就是随着训练记录一个变量的最大值、最小值、方差、的变化，以及直方图。
- **step 4**: 构建用于建立每层的函数：

```
def layer(self,in_tensor, in_dim, out_dim, layer_name, act=tf.nn.relu):
      with tf.name_scope(layer_name):
          with tf.name_scope(layer_name+'_weights'):
              # 用所建立的weight_init函数进行初始化。
              weights = self.weight_init([in_dim, out_dim])
              # 存放着每一个权重W
              self.W.append(weights)
              # 对权重进行统计
              self.variable_summaries(weights, layer_name + '/weights')
          with tf.name_scope(layer_name+'_biases'):
              biases = self.bias_init([out_dim])
              self.variable_summaries(biases, layer_name + '/biases')
          with tf.name_scope(layer_name+'_Wx_plus_b'):
              # 计算Wx+b
              pre_activate = tf.matmul(in_tensor, weights) + biases
              # 记录直方图
              tf.histogram_summary(layer_name + '/pre_activations', pre_activate)
          # 计算a(Wx+b)
          activations = act(pre_activate, name='activation')
          tf.histogram_summary(layer_name + '/activations', activations)
      # 最终返回该层的输出，以及权重W的L2
      return activations, tf.nn.l2_loss(weights)
```

- **解释 4**：该函数接受输出tensor，以及该层的信息，返回处理后的输出。而该输出又可以做为下一层的输入，以此不断反复来叠加层数。这里的self.W.append(weights)是我随意添加的，用于输出每一层的权重。self.W本身会在使用该函数前声明。（selb.b同理）
- **step 5**: 创建dropout层的函数：

```
def drop_layer(self,in_tensor):
          #tf.scalar_summary('dropout_keep', self.drop_keep_rate)
      dropped = tf.nn.dropout(in_tensor, self.drop_keep_rate)
      return dropped
```

- **解释 5**：将该内容额外编写出来是想让读者知道，尽管这个称为“层”，但“层”的操作也可以很简单。叫做层的操作一般都是那些可以反复叠加利用的tensor处理方式。
- **step 6**: 构建网络，看起来内容很多，实际上核心内容就是上一节所描述的：

```
def build(self, prefix):
      # 建立网络 
      # incoming也代表当前tensor的流动位置
      incoming = self.inputs
      # 如果没有隐藏层
      if self.Layers!=0:
          layer_nodes = [self.D_input] + self.N_hidden
      else:
          layer_nodes = [self.D_input]

      # hid_layers用于存储所有隐藏层的输出
      self.hid_layers=[]
      # W用于存储所有层的权重
      self.W=[]
      # b用于存储所有层的偏移
      self.b=[]
      # total_l2用于存储所有层的L2
      self.total_l2=[]
      # drop存储dropout后的输出
      self.drop=[]

      # 开始叠加隐藏层。这跟千层饼没什么区别。
      for l in range(self.Layers):
          # 使用刚才编写的函数来建立层，并更新incoming的位置
          incoming,l2_loss= self.layer(incoming,layer_nodes[l],layer_nodes[l+1],prefix+'_hid_'+str(l+1),act=tf.nn.relu)
          # 累计l2
          self.total_l2.append(l2_loss)
          # 输出一些信息，让我们知道网络在建造中做了什么
          print('Add dense layer: relu with drop_keep:%s' %self.drop_keep)
          print('    %sD --> %sD' %(layer_nodes[l],layer_nodes[l+1]))
          # 存储所有隐藏层的输出
          self.hid_layers.append(incoming)
          # 加入dropout层
          incoming = self.drop_layer(incoming)
          # 存储所有dropout后的输出
          self.drop.append(incoming)

      # 输出层的建立。输出层需要特别对待的原因是输出层的activation function要根据任务来变。
      # 回归任务的话，下面用的是tf.identity，也就是没有activation function
      if self.Task_type=='regression':
          out_act=tf.identity
      else:
          # 分类任务使用softmax来拟合概率
          out_act=tf.nn.softmax
      self.output,l2_loss= self.layer(incoming,layer_nodes[-1],self.D_label, layer_name='output',act=out_act)
      self.total_l2.append(l2_loss)
      print('Add output layer: linear')
      print('    %sD --> %sD' %(layer_nodes[-1],self.D_label))

      # l2 loss的缩放图
      with tf.name_scope('total_l2'):
          for l2 in self.total_l2:
              self.l2_penalty+=l2
          tf.scalar_summary('l2_penalty', self.l2_penalty)

      # 不同任务的loss
      # 若为回归，则loss是用于判断所有预测值和实际值差别的函数。
      if self.Task_type=='regression':
          with tf.name_scope('SSE'):
              self.loss=tf.reduce_mean((self.output-self.labels)**2)
              tf.scalar_summary('loss', self.loss)
      else:
          # 若为分类，cross entropy的loss function
          entropy = tf.nn.softmax_cross_entropy_with_logits(self.output, self.labels)
          with tf.name_scope('cross entropy'):
              self.loss = tf.reduce_mean(entropy)
              tf.scalar_summary('loss', self.loss)
          with tf.name_scope('accuracy'):
              correct_prediction = tf.equal(tf.argmax(self.output, 1), tf.argmax(self.labels, 1))
              self.accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
              tf.scalar_summary('accuracy', self.accuracy)
      # 整合所有loss，形成最终loss
      with tf.name_scope('total_loss'):
          self.total_loss=self.loss + self.l2_penalty*self.L2_lambda
          tf.scalar_summary('total_loss', self.total_loss)

      # 训练操作
      with tf.name_scope('train'):
          self.train_step = tf.train.AdamOptimizer(self.learning_rate).minimize(self.total_loss)
```

## **代码学习**

这是关于如何记住上面的内容的说明。

不同的**层**和所搭建的**网络**的本质都是**变体**，可以根据任务需求随意变化。**学习**是要把这些**变体**拆分成（因素+关联）的方式。
- **实例**：上述的结构中，关系如下图：
![](https://pic3.zhimg.com/v2-7094587d54abd8a254fcb3c4c87dcd6a_b.png)

- **因素**：weight_init、bias_init、variable_summaries、loss、train、parameters
- 绿色：代表被编写成了function。
- 黄色：代表直接以固定的形式写成代码。
- **变体**：最终的网络结构是这些因素有结构性的迭代组合：layer是由绿色方块组成，而build的组成因素又有layer。
- **关联**：这些黑线才是读者应该学习的对象：**关联**。

> **用变体训练，但不要学习变体，学习的是关联。**

到此请**再次理解**深层神经网络的两大固有先验知识：**并行**、**迭代**。 因为weight_init和bias_init的工作相互独立、并不干扰，我们才可以把他们拆分成因素。而这里的weight_init是truncated_normal方式的初始化，如果我想构建一个用其他初始化方法的层，那么被固定的weight_init就无法被使用。我们不得不重新写一个初始化方法。所以**深层的前提**是下一级的内容可由上一级构成。

**注**：我画的网络是**固定**的，神经网络并不一样。神经网络可以在训练时自由调整，会逐渐形成满足迭代先验的结构。自然数据几乎都可以从迭代先验中获益。

## **使用模块**
- **step 0**：数据准备

```
inputs=[[0,0],[0,1],[1,0],[1,1]]
outputs=[0,1,1,0]
X=np.array(inputs).reshape((4,1,2)).astype('int16')
Y=np.array(outputs).reshape((4,1,1)).astype('int16')
```

- **解释 0**：不一样的地方是reshape((4,1,1))一部分，YJango刻意要用这种方式表达的原因是因为在TensorFlow的使用中，常会遇到很多关于tensor的shape不匹配的问题。所以要尽早熟悉reshape的指令和作用。reshape((4,1,2))表达的意思是，4个有序的1 by 2矩阵（向量）。4表示训练数量。2表示维度。
- **step 1**：生成网络实例 ff（可随意取名字）

```
# 生成网络实例
ff=FNN(learning_rate=1e-3, drop_keep=1.0, Layers=1, N_hidden=[2], D_input=2, D_label=1, Task_type='regression', L2_lambda=1e-2)
# 下面是实际输出内容
#
Add dense layer: relu with drop_keep:1.0
  2D --> 2D
Add output layer: linear
  2D --> 1D
```

- **解释 1**：上面是生成一个初始学习速率为0.001，没有dropout（1.0表示全部保留，一个不扔），一个隐藏层（Layers表示隐藏层的个数），输入维度是2，目标维度是1，回归任务，L2的惩罚强度为0.01。生成后，程序会按照事先编写的格式输出一些内容。随后我们就可以用[http://ff.xxx](https://link.zhihu.com/?target=http%3A//ff.xxx)的形式来获取ff内的所有属性。
- **step 2**：加载会话Session

```
sess = tf.InteractiveSession()
tf.initialize_all_variables().run()
# 用于将所有summary合成一个ops
merged = tf.merge_all_summaries()
# 用于记录训练中内容。前一个参数是记录地址，后一个参数是记录的graph。
train_writer = tf.train.SummaryWriter('log' + '/train',sess.graph)
```

- **解释 2**：相比代码演示，这次的新内容只有后两句。用于记录所有统计内容。

## **训练前**
- **step 3**：训练前观察一下**权重W**的值# 获得输出

```
# 获得输出
W0= sess.run(ff.W[0])
W1= sess.run(ff.W[1])
# 显示
print('W_0:\n%s' %sess.run(ff.W[0]))
print('W_1:\n%s' %sess.run(ff.W[1]))
# 实际输出为
W_0:
[[-0.17334478 -0.0750991 ]
[-0.02134527  0.03925243]]
W_1:
[[ 0.00257381]
[ 0.0365728 ]]
```

- **解释 3**：ff.W[i]表示不同层的W，i是从0开始计数。为了有个更直观的认识，这里画出了权重和节点间的关系图。YJango为节省篇幅，还请读者从[github](https://link.zhihu.com/?target=https%3A//github.com/YJango/tensorflow/blob/master/XOR_gate.ipynb)中找源码，并且结合先前讲的[线性代数](https://link.zhihu.com/?target=https%3A//yjango.gitbooks.io/superorganism/content/xian_xing_dai_shu.html)中的矩阵乘法，找到**每个权重对应的两个节点**（在tensorflow中，矩阵乘法是x⋅Wx⋅W的形式进行的）。红色表示输入层，绿色是隐藏层，而蓝色是输出层。线的粗细表示权重的绝对值大小。
![](https://pic4.zhimg.com/v2-5af7a92c62e675541487c7ef71cf78c3_b.png)

- **step 4**：训练前的输出值（ff.output中）

```
# 训练前的输出
pY=sess.run(ff.output,feed_dict={ff.inputs:X.reshape((4,2)),ff.drop_keep_rate:1.0})
print(pY)
# 画图（4个数据循序用红、绿、蓝、黑表示）
plt.scatter([0,1,2,3],pY,color=['red','green','blue','black'],s=25,alpha=0.4,marker='o')
# 实际输出结果：
[[ 0.10391466]
[ 0.10529529]
[ 0.1009107 ]
[ 0.10234627]]
```

- **实际图**：可以看到并不是我们想要的结果。
![](https://pic2.zhimg.com/v2-f9996df86fd5bcb251f6250fd987c4cd_b.png)

- **step 5**：训练前的隐藏层的输出值（ff.hid_layers[i]中，i对应第一个隐藏层）

```
# 训练前隐藏层的输出
pY=sess.run(ff.hid_layers[0],feed_dict={ff.inputs:X.reshape((4,2)),ff.drop_keep_rate:1.0})
print(pY)
plt.scatter(pY[:,0],pY[:,1],color=['red','green','blue','black'],s=25,alpha=0.4,marker='o')
# 实际输出值：
[[ 0.1         0.1       ]
[ 0.07865474  0.13925242]
[ 0.          0.02490091]
[ 0.          0.06415333]]
```

- **实际图**：4个不同值
![](https://pic2.zhimg.com/v2-edfa92d20da191ecddfb03398c2b1571_b.png)

- **Dropout**：会随机扔掉节点，其余的节点会被乘以1/keep_rate。原理以后另讲。这里可以先通过调节keep_rate来感受隐藏层的输出变化。

```
sess.run(ff.drop[0],feed_dict={ff.inputs:X.reshape((4,2)),ff.labels:Y.reshape((4,1)),ff.drop_keep_rate:0.5})
# 当keep_rate：1时，保持所有节点（就是隐藏层的原有输出）
[[ 0.1        0.1       ]
[ 0.07865474  0.13925242]
[ 0.          0.02490091]
[ 0.          0.06415333]]
# 当keep_rate：0.5时，每个数据的节点都有一个被扔掉了。剩下的那个节点被乘以1/0.5
[[ 0.2        0.        ]
[ 0.          0.27850484]
[ 0.          0.04980182]
[ 0.          0.        ]]
```

## **训练**
- **step 6**：训练，并且记录所有统计内容

```
# 训练并记录
# k表示训练了多少次
k=0.0
# i每增加1，就表示所有的训练数据都被训练完了一次。叫做epoch
for i in range(10000):
  k+=1
  # summary是merged得出的值，即所有统计内容
  summary, _ = sess.run([merged,ff.train_step],feed_dict={ff.inputs:X.reshape((4,2)),ff.labels:Y.reshape((4,1)),ff.drop_keep_rate:1.0})
  # 将统计内容写入指定log文件中
  train_writer.add_summary(summary, k)
```

- **解释 6**：这里默认训练10000次。sess.run([merged,ff.train_step],feed....)中执行了两个操作。ff.train_step是训练操作。由于是ff的属性，记得加ff.

## **训练后**
- **权重**：

```
W_0:
[[-0.82895017  0.82891428]
[ 0.82915729 -0.82918972]]
W_1:
[[ 1.17231631]
[ 1.1722393 ]]
```

- **权重图**：
![](https://pic3.zhimg.com/v2-cc98b4f4f32177c7b9f0f1efdfd84776_b.png)

- **输出**：

```
[[  0.00000000e+00]
[  9.72034633e-01]
[  9.71685886e-01]
[  2.42817347e-04]]
```

- **输出图**：
![](https://pic1.zhimg.com/v2-e5e25e45712b158a3f0b334b3e028288_b.png)

- **隐藏层输出**

```
[[  0.00000000e+00   0.00000000e+00]
[  8.29157293e-01   0.00000000e+00]
[  0.00000000e+00   8.28914285e-01]
[  2.07126141e-04   0.00000000e+00]]
```

- **隐藏层输出**：注意这里只有**3个变体**。说明了学习也就是消除熵，消除变体的过程。
![](https://pic1.zhimg.com/v2-9d334bb900b4fe942579e351ab81e0e0_b.png)

## **权重的读存**

Tensorflow提供了[读存的API](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/versions/r0.11/how_tos/variables/index.html%23saving-and-restoring)，实际应用中十分方便。但这里YJango想让读者能更深的体会TensorFlow的操作（ops）的特点，直接用更新权重的方式读训练好的权重。可以作为练习，想想如果要用这种方式pre-train的话可以怎么做。详细内容看[github](https://link.zhihu.com/?target=https%3A//github.com/YJango/tensorflow/blob/master/XOR_gate.ipynb)。

```
# 该操作可以用于读取已经训练好的权重W和b
# 每层W想要读取的值
W_0=np.array([[-0.82895017,0.82891428],[ 0.82915729,-0.82918972]],dtype='float32')
W_1=np.array([[ 1.17231631],[ 1.1722393 ]],dtype='float32')
# 每层b想要读取的值
b_0=np.array([0,0],dtype='float32')
b_1=np.array([0],dtype='float32')
# 读取ops
reload1=tf.assign(ff.W[0],W_0)
reload2=tf.assign(ff.W[1],W_1)
reload3=tf.assign(ff.b[0],b_0)
reload4=tf.assign(ff.b[1],b_1)
# 执行ops
sess.run([reload1,reload2,reload3,reload4])
```

## **分析**

先前的代码中加入统计内容的目的就是为了在分析训练过程中的问题。
- **tensorboard**：
- 加载：tensorboard --logdir=（等号后面是log存放的地址）
- 进入：随后进入本地的[http://127.0.1.1:6006](https://link.zhihu.com/?target=http%3A//127.0.1.1%3A6006/) 就可以查看所有统计内容。包括最开始生成的graph图。
- **events**: 下面是各个变量最大值随训练的变化
![](https://pic2.zhimg.com/v2-dbdc14b20600659979dfafc9b957874d_b.png)

- **histograms**: 从这里可以看到随着训练（从上到下），隐藏层的各个变量在不同数据间直方图的变化。可以看到随着训练，隐藏层的变体的分布的变化过程。从最初叠加在一起逐渐变得分明。
![](https://pic4.zhimg.com/v2-56c4043c28088f2c56391310adc60387_b.png)

YJango花这么大的篇幅来描述的并不是XOR gate问题本身，而是利用这种容易控制的任务让读者尽可能多的体会神经网络的特点、熟悉神经网络的核心要素：权重 ![W](https://www.zhihu.com/equation?tex=W) ，偏移 ![b](https://www.zhihu.com/equation?tex=b) ，隐藏层输出 ![h](https://www.zhihu.com/equation?tex=h) 。随后的卷积神经网络、递归神经网络、所有的神经网络变体的本质就是代码演示中的内容。

并且看过《超智能体》前面内容的朋友，YJango希望你们将线性代数，生物进化，熵增，神经网络的所有内容都联想起来，不要分割。内容若不相关，我是不会特别放在《超智能体》中去描述的。


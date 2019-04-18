# YJango的前馈神经网络--代码LV1 - 知乎
# 

## **介绍**

用[TensorFlow](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/)来演示XOR门问题（可以换成任何问题，并用相同方法来解决）。

下面的代码只是没有任何枝叶的**核心内容**。全部代码在[github](https://link.zhihu.com/?target=https%3A//github.com/YJango/tensorflow/blob/master/core_code.ipynb)上。

## **准备工作**
- ubuntu系统
- [安装](https://link.zhihu.com/?target=http%3A//blog.csdn.net/u010751535/article/details/50806073)工具

## **模块分布**

训练神经网络主要就三大模块。掌握模块核心任务**后**再学习分支内容，中途可任意增添，并且以后还会不断发展。我不希望读者在该篇展开过深。只要对神经网络有一个大概的感觉即可。如下图所示，先**抓住三大模块**。越靠右边的内容，初学者就越不要上来就展开。
![](https://pic2.zhimg.com/v2-22f1881b7b1241b774d3dbca20b3ced5_b.png)

## **一、网络说明**
- 打开jupyter notebook（安装完anaconda直接就有）
- new->python
- 引入库：

```
# tensorflow引入，并重命名为tf
import tensorflow as tf

# 矩阵操作库
import numpy as np
```

- **网络结构**：2维输入  ![\rightarrow](https://www.zhihu.com/equation?tex=%5Crightarrow)![\rightarrow](https://www.zhihu.com/equation?tex=%5Crightarrow)1维输出
![](https://pic1.zhimg.com/v2-7ede84b0e78751d92e323a486ba90d3c_b.png)

- **结构表达式**：
- 正向传递： ![y=M(x)=relu(W_{h} \cdot relu(W_{x} \cdot x+b_{x})+b_{h})](https://www.zhihu.com/equation?tex=y%3DM%28x%29%3Drelu%28W_%7Bh%7D+%5Ccdot+relu%28W_%7Bx%7D+%5Ccdot+x%2Bb_%7Bx%7D%29%2Bb_%7Bh%7D%29) (1)
- ![y](https://www.zhihu.com/equation?tex=y) 用于表达随机变量 ![Y](https://www.zhihu.com/equation?tex=Y) 的值， ![x](https://www.zhihu.com/equation?tex=x) 表示随机变量 ![X](https://www.zhihu.com/equation?tex=X) 的值， ![M(x)](https://www.zhihu.com/equation?tex=M%28x%29) 是我们的神经网络模型，等号右侧是具体的表达。
- 损失函数： ![loss=1/2\cdot \sum\limits_i (y_i-t_i)^2](https://www.zhihu.com/equation?tex=loss%3D1%2F2%5Ccdot+%5Csum%5Climits_i+%28y_i-t_i%29%5E2)
- 该loss就是比较 ![y](https://www.zhihu.com/equation?tex=y) 和 ![t](https://www.zhihu.com/equation?tex=t) 中所有值的差别。

## **二、核心功能**

## **计算图说明：**

请先阅读：[TensorFlow整体把握](https://zhuanlan.zhihu.com/p/23932714)，这里解释了：
- 为什么使用tensorflow、theano等这样的工具。
- 计算图是什么
- TensorFlow基本用法

TensorFlow所建立的只是一个网络框架。在编程时，并不会有任何实际值出现在框架中。所有权重和偏移都是框架中的一部分，初始时至少给定初始值才能形成框架。因此需要initialization初始化。

## **实例说明：**

以下是利用[tensorboard](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/versions/r0.11/how_tos/summaries_and_tensorboard/index.html)生成的我们想要建立的学习XOR gate的网络可视化结构图。
- **整体结构**：左侧的图表示网络结构。绿色方框表示操作，也叫作层（layer）。该结构中，输入 ![x](https://www.zhihu.com/equation?tex=x) 经过hid_layer算出隐藏层的值 ![h](https://www.zhihu.com/equation?tex=h) ，再传递给out_layer，计算出预测值 ![y](https://www.zhihu.com/equation?tex=y) ，随后与真实值 ![t](https://www.zhihu.com/equation?tex=t) 进行比较，算出损失 ![loss](https://www.zhihu.com/equation?tex=loss) ，再从反向求导得出梯度后对每一层的 ![W](https://www.zhihu.com/equation?tex=W) 和 ![b](https://www.zhihu.com/equation?tex=b) 进行更新。
![](https://pic3.zhimg.com/v2-e3227327a2daa56a5394053ba0d88b0a_b.png)

- **正向传递**：如果放大hid_layer内部，从下向上，会看到 ![W_h](https://www.zhihu.com/equation?tex=W_h) 先用truncated_normal的方法进行了初始化，随后与输入 ![x](https://www.zhihu.com/equation?tex=x) 进行矩阵相乘，加上 ![b_h](https://www.zhihu.com/equation?tex=b_h) ，又经过了activation后，送给了用于计算 ![y](https://www.zhihu.com/equation?tex=y) 的out_layer中。而 ![y](https://www.zhihu.com/equation?tex=y) 的计算方式和 ![h](https://www.zhihu.com/equation?tex=h) 完全一致，但用的是不同的权重 ![W_o](https://www.zhihu.com/equation?tex=W_o) 和偏移 ![b_o](https://www.zhihu.com/equation?tex=b_o) 。最后将算出的预测值 ![y](https://www.zhihu.com/equation?tex=y) 与真实值 ![t](https://www.zhihu.com/equation?tex=t) 一同求出 ![loss](https://www.zhihu.com/equation?tex=loss)
![](https://pic3.zhimg.com/v2-7eb77efa6bf6d1326f11f9c158d7571a_b.png)

- **反向传递**：如果放大train的内部，再放大内部中的gradients，就可以看到框架是从 ![loss](https://www.zhihu.com/equation?tex=loss) 开始一步步反向求得各个层中 ![W](https://www.zhihu.com/equation?tex=W) 和 ![b](https://www.zhihu.com/equation?tex=b) 的梯度的。
![](https://pic4.zhimg.com/v2-344e704fb95d85ba9a5efddf120e1337_b.png)

- **权重更新**：求出的各个层 ![W](https://www.zhihu.com/equation?tex=W) 和 ![b](https://www.zhihu.com/equation?tex=b) 的梯度，将会被用于更新对应的 ![W](https://www.zhihu.com/equation?tex=W) 和 ![b](https://www.zhihu.com/equation?tex=b) ，并用learning rate控制一次更新多大。（beta1_power和beta2_power是[Adam更新](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1412.6980)方法中的参数，目前只需要知道权重更新的核心是各自对应的梯度。）
![](https://pic4.zhimg.com/v2-17ef57321a563e408c3f890c02f15647_b.png)

## **三、代码实现**

## **网络模块**
- 变量定义：定义节点数和学习速率

```
# 网络结构：2维输入 --> 2维隐藏层 --> 1维输出
# 学习速率（learning rate）：0.0001

D_input  = 2
D_label  = 1
D_hidden = 2
lr       = 1e-4
```

**正向传递：**到预测值为止的网络框架。在预测时，只拿的输出。
- **容器**：用tf.placeholder创建输入值和真实值的容器，编程过程中始终是个空的，只有加载到sess中才会放入具体值。这种容器便是存放tensor的数据类型。

```
x = tf.placeholder(tf.float32, [None, D_input], name="x")
t = tf.placeholder(tf.float32, [None, D_label], name="t")
```

- **精度**： 如果是用GPU训练，浮点精度要低于32bit，由第一个参数tf.float32定义。
- **矩阵形状**： 输入输出的容器都是矩阵。为的是可以进行mini-batch一次算多个样本的平均梯度来训练。None意味着样本数可随意改变。
- **命名**：控制tensorboard生成图中的名字，也会方便debug。
- **隐藏层**：

```
# 初始化W
W_h1 = tf.Variable(tf.truncated_normal([D_input, D_hidden], stddev=0.1), name="W_h")
# 初始化b
b_h1 = tf.Variable(tf.constant(0.1, shape=[D_hidden]), name="b_h")
# 计算Wx+b
pre_act_h1 = tf.matmul(x, W_h1) + b_h1
# 计算a(Wx+b)
act_h1 = tf.nn.relu(pre_act_h1, name='act_h')
```

- **变量**：tensorflow中的变量tf.Variable是用于定义在训练过程中可以更新的值。权重W和偏移b正符合该特点。
- **初始化**：合理的初始化会给网络一个比较好的训练起点，帮助逃脱局部极小值（or 鞍点）。详细请回顾[梯度下降训练法](https://link.zhihu.com/?target=https%3A//yjango.gitbooks.io/superorganism/content/ti_du_xia_jiang_xun_lian_fa.html)。tf.truncated_normal([D_input, D_hidden], stddev=0.1)是初始化的一种方法（还有很多种），其中[imcoing_dim, outputing_dim]是矩阵的形状，前后参数的意义是进入该层的维度（节点个数）和输出该层的维度。stddev=是用于初始化的标准差。
- **矩阵乘法**：tf.matmul(x, W_h1)用于计算矩阵乘法
- **激活函数**：除了tf.nn.relu()还有tf.nn.tanh()，tf.nn.sigmoid()
- **输出层**：同隐藏层的计算方式一致，但输出层的“输入”是隐藏层的“输出”。

```
W_o = tf.Variable(tf.truncated_normal([D_hidden, D_label], stddev=0.1), name="W_o")
b_o = tf.Variable(tf.constant(0.1, shape=[D_label]), name="b_o")
pre_act_o = tf.matmul(act_h1, W_o) + b_o
y = tf.nn.relu(pre_act_o, name='act_y')
```

**反向传递：**计算误差值来更新网络权重的结构。
- **损失函数**：定义想要不断缩小的损失函数。

`loss = tf.reduce_mean((self.output-self.labels)**2)`
- **更新方法**：选择想要用于更新权重的训练方法和每次更新步伐（lr），除tf.train.AdamOptimizer外还有tf.train.RMSPropOptimizer等。默认推荐AdamOptimizer。

`train_step = tf.train.AdamOptimizer(lr).minimize(loss)`
- **学习速率**：AdamOptimizer(lr)的括号内放入学习速率。
- **优化目标**：minimize(loss)的括号内放入想要缩小的值。

**准备数据：**用numpy.array格式准备XOR的输入和输出，即训练数据

```
#X和Y是4个数据的矩阵，X[i]和Y[i]的值始终对应。
X=[[0,0],[0,1],[1,0],[1,1]]
Y=[[0],[1],[1],[0]]
X=np.array(X).astype('int16')
Y=np.array(Y).astype('int16')
```

- **数据类型**：用python使用tensorflow时，输入到网络中的训练数据需要以np.array的类型存在。并且要限制dtype为32bit以下。变量后跟着“.astype('float32')”总可以满足要求。

**加载训练：**将建好的网络加载到session中执行操作。

```
#创建session
sess = tf.InteractiveSession()
#初始化权重
tf.initialize_all_variables().run()
```

- **创建方式**：sess = tf.InteractiveSession()是比较方便的创建方法。也有sess = tf.Session()方式，但该方式无法使用tensor.eval()快速取值等功能。
- **初始化**：虽然在先前定义权重 ![W](https://www.zhihu.com/equation?tex=W) 时选择了初始化方式，但要实际执行该操作需要将操作（op）加载到session中。
- **训练网络**：

```
T=10000 #训练几次
for i in range(T):
  sess.run(train_step,feed_dict={x:X,t:Y})
```

- **说明**：sess.run(,)接受的两个参数。前一个参数的类型是list，表示所有想要执行的操作（op）或者想要获取的值。而后一个参数会需要执行第一个参数时相关placeholder的值。并且以feed_dict=字典的方式赋值。{x:X,t:Y}中的x是key名，对应着placeholder，而冒号后的是计算时的具体输入值。
- **GD（Gradient Descent）**：X和Y是4组不同的训练数据。上面将所有数据输入到网络，算出平均梯度来更新一次网络的方法叫做GD。效率很低，也容易卡在局部极小值，但更新方向稳定。
- **SGD（Gradient Descent）**：一次只输入一个训练数据到网络，算出梯度来更新一次网络的方法叫做SGD。效率高，适合大规模学习任务，容易挣脱局部极小值（或鞍点），但更新方向不稳定。代码如下：

```
T=10000 #训练几epoch
for i in range(T):
    for j in range(X.shape[0]): #X.shape[0]表示样本个数
        sess.run(train_step,feed_dict={x:X[j],t:Y[j]})
```

- **batch-GD**：这是上面两个方法的折中方式。每次计算部分数据的平均梯度来更新权重。部分数据的数量大小叫做batch_size，对训练效果有影响。一般10个以下的也叫mini-batch-GD。代码如下：

```
T=10000 #训练几epoch
b_idx=0 #batch计数
b_size=2 #batch大小
for i in range(T):
    while batch_idx<=X.shape[0]:
        sess.run(train_step,feed_dict={x:X[b_idx:b_idx+b_size],t:Y[b_idx:b_idx+b_size]})
        b_idx+=b_size #更新batch计数
```

- **shuffle**：SGD和batch-GD由于只用到了部分数据。若数据都以相同顺序进入网络会使得随后的epoch影响很小。shuffle是用于打乱数据在矩阵中的排列顺序，提高后续epoch的训练效果。代码如下：

```
#shuffle函数
def shufflelists(lists): #多个序列以相同顺序打乱
    ri=np.random.permutation(len(lists[1]))
    out=[]
    for l in lists:
        out.append(l[ri])
    return out
#训练网络
T=10000 #训练几epoch
b_idx=0 #batch计数
b_size=2 #batch大小
for i in range(T):
#每次epoch都打乱顺序
    X,Y = shufflelists([X,Y])
    while batch_idx<=X.shape[0]:
        sess.run(train_step,feed_dict={x:X[b_idx:b_idx+b_size],t:Y[b_idx:b_idx+b_size]})
        b_idx+=b_size #更新batch计数
```

- **预测**：

```
#计算预测值
sess.run(y,feed_dict={x:X})
#输出：已训练100000次
array([[ 0.],
       [ 1.],
       [ 1.],
       [ 0.]], dtype=float32)
```

- **说明**：预测时与目标值 ![t](https://www.zhihu.com/equation?tex=t) 无关，只需要将输入 ![x](https://www.zhihu.com/equation?tex=x) 到网络中即可预测该 ![x](https://www.zhihu.com/equation?tex=x) 对应哪个 ![y](https://www.zhihu.com/equation?tex=y) ，而预测好坏取决于训练的网络本身。
- **隐藏层**：

```
#查看隐藏层的输出
sess.run(act_h1,feed_dict={x:X})
#输出：已训练100000次
array([[ 1.10531139,  1.00508392],
       [ 0.55236477,  0.        ],
       [ 0.55236477,  0.        ],
       [ 0.        ,  0.        ]], dtype=float32)
```

- **说明**：act_h1是隐藏层的输出。可以看到隐藏层前有4个变体，而经过隐藏层后，只有3个变体。
- **查看其他值**：用户可以用 sess.run()获取网络框架中的任何值。比如查看隐藏层的权重 ![W_h](https://www.zhihu.com/equation?tex=W_h) 和 ![W_o](https://www.zhihu.com/equation?tex=W_o) ：

`sess.run([W_h,W_o])`
- **多值输出**：可将多个想要的值放入一个list中传递给 sess.run()实现多值输出。
- **变量无需容器**： ![W_h](https://www.zhihu.com/equation?tex=W_h) 和 ![W_o](https://www.zhihu.com/equation?tex=W_o) 的获取并未需要输入placeholder。因为训练好后，它们的值与placeholder的值无关，所以不需要输出。

上面已经演示了最基本，也是核心代码内容。虽然简陋，但完全可以完成任务。

深层神经网络就是简单的在此基础上增加更多的隐藏层。深层神经网络的实现非常简单，并没有人们想想的那么高大上。

然而深层学习的难点不在网络实现上，而在对数据的分析和反馈、当遇到无法拟合的情况该如何处理。为了能够记录和分析结果，我们会想知道误差下降的过程，网络在某时训练的如何，比如下图：可以看出来训练在第30000次后的误差就几乎为0了。但上述的代码可以让我们完成训练却没有这些分析功能。下章是关于记录、分析、代码重用性等功能所需要的“枝叶”。

![](https://pic4.zhimg.com/v2-268975a1b44b226daaa0db768be10fc3_b.png)


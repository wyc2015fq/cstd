# TensorFlow之四—正则化 - wsp_1138886114的博客 - CSDN博客





2019年01月26日 19:37:16[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：64标签：[TensorFlow之四—正则化](https://so.csdn.net/so/search/s.do?q=TensorFlow之四—正则化&t=blog)
个人分类：[深度学习](https://blog.csdn.net/wsp_1138886114/article/category/7729523)












### 文章目录
- [一、常见正则化方法](#_3)
- [二、L2正则化 -- tf.contrib.layers.l2_regularizer()](#L2__tfcontriblayersl2_regularizer_18)
- [三、L1正则化 -- tf.contrib.layers.l1_regularizer()](#L1__tfcontriblayersl1_regularizer_52)
- [四、混合正则 tf.contrib.layers.sum_regularizer()](#_tfcontriblayerssum_regularizer_84)



正则化：避免过拟合的方法之一。在我们使用模型训练数据时，会遇到欠拟合和过拟合的问题，[关于欠拟合和过拟合详情请点击](https://blog.csdn.net/wsp_1138886114/article/details/81084934)。我们今天主要讲使用正则化来防止过拟合。


### 一、常见正则化方法

在线性回归等问题中，我们定义了损失函数:
$$J(\theta;X,y )=\frac{1}{2}* \sum_{i=1}^{n}(y-y^{'})^{2}$$
- 注：这里的θ是指所有的参数，包括权重w等

我们发现，这样学习的模型虽然在测试集上比较好，但是泛化能力一般，于是就有了参数惩罚的思路，直接给J()后面加个惩罚项（拖住学习的节奏），也就是正则化项，损失函数变成了：

$$J(\theta;X,y )=\frac{1}{2}* \sum_{i=1}^{n}(y-y^{'})^{2}+\alpha \Omega (\theta ) $$

其中 $α∈[0,∞)$ 是惩罚系数，那么这个惩罚项 $Ω(θ)$ 是什么呢？在神经网络中又是怎样使用呢？

在神经网络中，参数包括每一层仿射变换的权重和偏置，我们**通常只对权重做惩罚而不对偏置做正则惩罚**。精确拟合偏置所需的数据通常比拟合权重少得多，且每个权重会指定两个变量如何相互作用。我们需要在各种条件下观察这两个变量才能良好地拟合权重。而每个偏置仅控制一个单变量。这意味着，我们不对其进行正则化也不会导致太大的方差。另外，正则化偏置参数可能会导致明显的欠拟合。因此，我们使用向量 $w$ 表示所有应受范数惩罚影响的权重，而向量 $θ$ 表示所有参数 (包括 $w$ 和无需正则化的参数)。

在神经网络的情况下，有时希望对网络的每一层使用单独的惩罚，并分配不同 的 $α$ 系数。寻找合适的多个超参数的代价很大，因此为了减少搜索空间，我们会在所有层使用相同的权重衰减(L2正则化)。

### 二、L2正则化 – tf.contrib.layers.l2_regularizer()

说起L2正则化，它指权值向量w中各个元素的平方和然后再求平方根（可以看到Ridge回归的L2正则化项有平方符号），通常表示为 $||w||^{2}$ 数学表示为：
$$\Omega (\theta )=\frac{1}{2}||w||^{2}=\frac{1}{2}\sum_{i=1}^{n}||w_{i}||^{2}$$

其目的是使权重更加接近原点1。在其他学术圈， L2 也被称为岭回归或 Tikhonov 正则。那它是怎么实现权重衰减的呢？我们用线性代数表示一下损失函数：

$$\hat{J}(w;X,y)=\alpha w^{T}w+J(w;X,y)$$

我们对权重求取梯度得到:

$$w\leftarrow w-\beta (\alpha w+\bigtriangledown _{w}J(w;X,y))$$

β为梯度下降的步长，最终我们得到了：

$$w\leftarrow (1-\beta \alpha )w-\bigtriangledown _{w}J(w;X,y)$$

我们可以看到，加入权重衰减后会引起学习规则的修改，即在每步执行通常的梯度更 新之前先收缩权重向量（将权重向量乘以一个常数因子）。其实L2的本质是：**L2正则化能让学习算法 “感知” 到具有较高方差的输入 x，因此与输出目标的协方差较小（相对增加方差）的特征的权重将会收缩。**

```python
import tensorlfow as tf
tf.contrib.layers.l2_regularizer(
    scale,            # 该值为 0.0 时，禁用正则化
    scope=None        # 可选作用域名称。
)
"""
该函数返回一个可用于对权重使用l2正则化的函数。一个小的l2值有助于防止过度拟合训练数据。
"""

# 实现方式
def get_weight(shape):
    return tf.Variable(tf.random_normal(shape),dtype=tf.float32)
def get_loss(shape,lambda):
    var = get_weight(shape)
    loss = tf.reduce_mean(tf.square(y_ - cur_layer))+tf.contrib.layers.l2_regularizer(lambda)(var))
    return loss
```

### 三、L1正则化 – tf.contrib.layers.l1_regularizer()

L1正则化，它和L2之间区别很小，是指权值向量w中各个元素的绝对值之和，通常表示为$||w||_{1}$，表示为：
$$\Omega (\theta )=\frac{1}{2}||w||=\frac{1}{2}\sum_{i=1}^{n}||w_{i}|| $$

那么损失函数就变成了：

$$\hat{J}(w;X,y)=\alpha||w||+J(w;X,y)$$

求解梯度后就和L2之间有点区别了，它的梯度为：

$$\bigtriangledown _{w}\hat{J}(w;X,y)=\alpha sign(w)-\bigtriangledown _{w}J(w;X,y)$$

我们可以看到正则化对梯度的影响不再是线性地缩放每个 $w_i$；而是添加了一项与 $w_i$ 同号的常数。使用这种形式的梯度之后，我们不一定能得到 $J(X;y;w)$ 二次近似的直接算术解（L2正则化时可以）。

比较特征就是 L2正则化不会使参数变得稀疏，而L1正则化有可能通过足够大的α实现稀疏。由 L1正则化导出的稀疏性质已经被广泛地用于特征选择（feature selection）机制。 特征选择从可用的特征子集选择出有意义的特征，化简机器学习问题。

同样的道理，L1正则化的tensorlfow代码只是使用了不同的接口而已，L2换成L1即可

```python
import tensorlfow as tf
tf.contrib.layers.l1_regularizer(
    scale,
    scope=None
)
 
# 具体的实现方式
def get_weight(shape):
    return tf.Variable(tf.random_normal(shape),dtype=tf.float32)
def get_loss(shape,lambda):
    var = get_weight(shape)
    loss = tf.reduce_mean(tf.square(y_ - cur_layer))+tf.contrib.layers.l1_regularizer(lambda)(var))
    return loss
```

### 四、混合正则 tf.contrib.layers.sum_regularizer()

`tf.contrib.layers.sum_regularizer(regularizer_list, scope=None)`

返回一个可以执行多种(个)正则化的函数。意思是，创建一个正则化方法,这个方法是多个正则化方法的混合体。

参数：regularizer_list：regulizer的列表

应用正则化方法到参数上
`tf.contrib.layers.apply_regularization(regularizer, weights_list=None)`
- 参数:

`regularizer`：就是我们上一步创建的正则化方法
`weights_list`：想要执行正则化方法的参数列表,如果为None的话，就取GraphKeys.WEIGHTS中的weights.

函数返回一个标量Tensor,同时,这个标量Tensor也会保存到`GraphKeys.REGULARIZATION_LOSSES`中。这个Tensor保存了计算正则项损失的方法.

tensorflow中的Tensor是保存了计算这个值的路径(方法),当我们run的时候,tensorflow后端就通过路径计算出Tensor对应的值

现在,我们只需将这个正则项损失加到我们的损失函数上就可以了.

如果是自己手动定义weight的话，需要手动将weight保存到GraphKeys.WEIGHTS中，但是如果使用layer的话,就不用这么麻烦了,别人已经帮你考虑好了.(最好自己验证一下tf.GraphKeys.WEIGHTS中是否包含了所有的weights,防止被坑)

其它

在使用`tf.get_variable()`和`tf.variable_scope()`的时候，你会发现，它们俩中有regularizer形参。如果传入这个参数的话，那么variable_scope内的weights的正则化损失，或者weights的正则化损失就会被添加到`GraphKeys.REGULARIZATION_LOSSES`中.

示例:
```python
import tensorflow as tf
from tensorflow.contrib import layers

regularizer = layers.l1_regularizer(0.1)
with tf.variable_scope('var', 
                       initializer=tf.random_normal_initializer(),
                       regularizer=regularizer):
    weight = tf.get_variable('weight', shape=[8], initializer=tf.ones_initializer())

with tf.variable_scope('var2', 
                       initializer=tf.random_normal_initializer(),
                       regularizer=regularizer):
    weight2 = tf.get_variable('weight', shape=[8], initializer=tf.ones_initializer())

regularization_loss = tf.reduce_sum(tf.get_collection(tf.GraphKeys.REGULARIZATION_LOSSES))
```

最后来看一个小案例

```python
import tensorflow as tf
from numpy.random import RandomState


def get_weights1(lambd):
    var1 = tf.Variable(tf.truncated_normal([2, 6], seed=1), dtype=tf.float32)
    tf.add_to_collection('losses', tf.contrib.layers.l2_regularizer(lambd)(var1))
    return var1

def get_weights2(lambd):
    var2 = tf.Variable(tf.truncated_normal([6, 1], seed=1), dtype=tf.float32)
    tf.add_to_collection('losses', tf.contrib.layers.l2_regularizer(lambd)(var2))
    return var2
"""
这里关于w1,w2,x初始化的数据形状（维度）多说一句：（矩阵相乘：前一矩阵的行数与后一矩阵的列数相同）
也就是w1[2, 6]与x[None, 2]这两个维度要对应
"""
w1 = get_weights1([2, 6])
w2 = get_weights2([6, 1])
x = tf.placeholder(dtype=tf.float32, shape=[None, 2])
y_real = tf.placeholder(dtype=tf.float32, shape=[None, 1])

a = tf.nn.relu(tf.matmul(x, w1))       # 激活函数为 relu
y_pre = tf.nn.relu(tf.matmul(a, w2))

sample_size = 20000                    # 训练样本总数
batch = 500                            # 使用mini-batch（批梯度下降）。

rds = RandomState(0)
X = rds.rand(sample_size, 2)
Y = [[int(20 * x1 + 30 * x2)] + rds.rand(1) for (x1, x2) in X]

global_step = tf.Variable(0)

# --------------学习速率的设置（学习速率呈指数下降）----- # 将 global_step/decay_steps 强制转换为整数
learning_rate = tf.train.exponential_decay(1e-2, global_step, decay_steps=sample_size/batch,
                                           decay_rate=0.98,staircase=True)
MSE = tf.reduce_mean(tf.square(y_real - y_pre))       # 使用均方误差（MSE）作为损失函数
train_step = tf.train.GradientDescentOptimizer(learning_rate).minimize(MSE, global_step=global_step)

step = 20000                         # 训练的总次数
start = 0
end = batch


with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    for i in range(step):
        sess.run(train_step, feed_dict={x: X[start:end], y_real: Y[start:end]})
        if not i % 20:
            H = sess.run(MSE, feed_dict={x: X[start:end], y_real: Y[start:end]})
            lr = sess.run(learning_rate)
            if i%2000 ==0:
                print("MSE: ", H, "\t", "learning_rate: ", lr)
            if H < 1e-1:               # 采用stop early 的方法防止过拟合，节省训练时间。
                break
        strat = end if end < sample_size else 0
        end = start + batch
    y1 = sess.run(y_pre, feed_dict={x: X[start:end]})
    y2 = Y[start:end]

# 训练结果的部分展示
for i in range(100):
    if i % 20 == 0:
        print(y1[i], y2[i])
        
输出：
MSE:  543.05066 	 learning_rate:  0.01
MSE:  0.15368746 	 learning_rate:  0.0036417004
MSE:  0.1533455 	 learning_rate:  0.0013261981
MSE:  0.15326522 	 learning_rate:  0.0004829616
MSE:  0.15323871 	 learning_rate:  0.00017588015
MSE:  0.15322933 	 learning_rate:  6.405028e-05
MSE:  0.15322606 	 learning_rate:  2.3325192e-05
MSE:  0.1532248 	 learning_rate:  8.494337e-06
MSE:  0.15322448 	 learning_rate:  3.0933827e-06
MSE:  0.15322448 	 learning_rate:  1.1265173e-06
[32.428265] [32.36925632]
[20.29843] [20.56019416]
[18.785416] [18.48722409]
[29.536114] [29.6998297]
[27.546238] [27.39447417]
```

在网络训练中我们可以收集正则化损失

收集正则化损失 `tf.get_collection(tf.GraphKeys.REGULARIZATION_LOSSES)`





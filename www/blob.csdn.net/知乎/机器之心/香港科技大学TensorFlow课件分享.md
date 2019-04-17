# 香港科技大学TensorFlow课件分享 - 知乎
# 



> *这是一套香港科技大学发布的极简 TensorFlow 入门教程，三天全套幻灯片教程已被分享到 Google Drive。机器之心将简要介绍该教程并借此梳理 TensorFlow 的入门概念与实现。*




该教程第一天先介绍了深度学习和机器学习的潜力与基本概念，而后便开始探讨深度学习框架 TensorFlow。首先我们将学到如何安装 TensorFlow，其实我们感觉 TensorFlow 环境配置还是相当便捷的，基本上按照官网的教程就能完成安装。随后就从「Hello TensorFlow」开始依次讲解计算图、占位符、张量等基本概念。

当然我们真正地理解 TensorFlow 还需要从实战出发一点点学习那些最基本的概念，因此第一天重点讲解了线性回归、Logistic 回归、Softmax 分类和神经网络。每一个模型都从最基本的概念出发先推导运行过程，然后再结合 TensorFlow 讲解张量、计算图等真正的意义。神经网络这一部分讲解得十分详细，我们将从最基本的感知机原理开始进而使用多层感知机解决异或问题（XOR），重点是该课程详细推导了前向传播与反向传播的数学过程并配以 TensorFlow 实现。

教程第二天详细地讨论了卷积神经网络，它从 TensorFlow 的训练与构建技巧开始，解释了应用于神经网络的各种权重初始化方法、激活函数、损失函数、正则化和各种优化方法等。在教程随后论述 CNN 原理的部分，我们可以看到大多是根据斯坦福 CS231n 课程来解释的。第二天最后一部分就是使用 TensorFlow 实现前面的理论，该教程使用单独的代码块解释了 CNN 各个部分的概念，比如说 2 维卷积层和最大池化层等。

教程第三天详解了循环神经网络，其从时序数据开始先讲解了 RNN 的基本概念与原理，包括编码器-解码器模式、注意力机制和门控循环单元等非常先进与高效的机制。该教程后一部分使用了大量的实现代码来解释前面我们所了解的循环神经网络基本概念，包括 TensorFlow 中单个循环单元的构建、批量输入与循环层的构建、RNN 序列损失函数的构建、训练计算图等。

下面机器之心将根据该教程资料简要介绍 TensorFlow 基本概念和 TensorFlow 机器学习入门实现。更详细的内容请查看香港科技大学三日 TensorFlow 速成课程资料




三日速成课程 Google Drive 资料地址：[https://drive.google.com/drive/folders/0B41Zbb4c8HVyY1F5Ml94Z2hodkE](https://link.zhihu.com/?target=https%3A//drive.google.com/drive/folders/0B41Zbb4c8HVyY1F5Ml94Z2hodkE)

三日速成课程百度云盘资料地址：[http://pan.baidu.com/s/1boGGzeR](https://link.zhihu.com/?target=http%3A//pan.baidu.com/s/1boGGzeR)




## **TensorFlow 基础**

本小节将从张量与图、常数与变量还有占位符等基本概念出发简要介绍 TensorFlow。需要进一步了解 TensorFlow 的读者可以阅读谷歌 TensorFlow 的文档，当然也可以阅读其他中文教程或书籍，例如《TensorFlow：实战 Google 深度学习框架》和《TensorFlow 实战》等。

TensorFlow 文档地址：[https://www.tensorflow.org/get_started/](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/get_started/)




## **1. 图**

TensorFlow 是一种采用数据流图（data flow graphs），用于数值计算的开源软件库。其中 Tensor 代表传递的数据为张量（多维数组），Flow 代表使用计算图进行运算。数据流图用「结点」（nodes）和「边」（edges）组成的有向图来描述数学运算。「结点」一般用来表示施加的数学操作，但也可以表示数据输入的起点和输出的终点，或者是读取/写入持久变量（persistent variable）的终点。边表示结点之间的输入/输出关系。这些数据边可以传送维度可动态调整的多维数据数组，即张量（tensor）。
![](https://pic4.zhimg.com/v2-4a67e12961d71d510c83c2aa35a8febb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='252' height='448'></svg>)
在 Tensorflow 中，所有不同的变量和运算都是储存在计算图。所以在我们构建完模型所需要的图之后，还需要打开一个会话（Session）来运行整个计算图。在会话中，我们可以将所有计算分配到可用的 CPU 和 GPU 资源中。 
![](https://pic4.zhimg.com/v2-7346237ff3379d3ee3161864aa13606b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='726' height='516'></svg>)
如上所示我们构建了一个加法运算的计算图，第二个代码块并不会输出计算结果，因为我们只是定义了一张图，而没有运行它。第三个代码块才会输出计算结果，因为我们需要创建一个会话（Session）才能管理 TensorFlow 运行时的所有资源。但计算完毕后需要关闭会话来帮助系统回收资源，不然就会出现资源泄漏的问题。

TensorFlow 中最基本的单位是常量（Constant）、变量（Variable）和占位符（Placeholder）。常量定义后值和维度不可变，变量定义后值可变而维度不可变。在神经网络中，变量一般可作为储存权重和其他信息的矩阵，而常量可作为储存超参数或其他结构信息的变量。在上面的计算图中，结点 1 和结点 2 都是定义的常量 tf.constant()。我们可以分别声明不同的常量（tf.constant()）和变量（tf.Variable()），其中 tf.float 和 [http://tf.int](https://link.zhihu.com/?target=http%3A//tf.int) 分别声明了不同的浮点型和整数型数据。




## **2. 占位符和 feed_dict**

TensorFlow 同样还支持占位符，占位符并没有初始值，它只会分配必要的内存。在会话中，占位符可以使用 feed_dict 馈送数据。

feed_dict 是一个字典，在字典中需要给出每一个用到的占位符的取值。在训练神经网络时需要每次提供一个批量的训练样本，如果每次迭代选取的数据要通过常量表示，那么 TensorFlow 的计算图会非常大。因为每增加一个常量，TensorFlow 都会在计算图中增加一个结点。所以说拥有几百万次迭代的神经网络会拥有极其庞大的计算图，而占位符却可以解决这一点，它只会拥有占位符这一个结点。
![](https://pic3.zhimg.com/v2-864fa7500da2b3b05ed4b08012f9f8fa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='774' height='333'></svg>)
## **3. 张量**

在 TensorFlow 中，张量是计算图执行运算的基本载体，我们需要计算的数据都以张量的形式储存或声明。如下所示，该教程给出了各阶张量的意义。
![](https://pic3.zhimg.com/v2-d8a71cabf42889643b6c5204e57f526a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1092' height='305'></svg>)



零阶张量就是我们熟悉的标量数字，它仅仅只表达了量的大小或性质而没有其它的描述。一阶张量即我们熟悉的向量，它不仅表达了线段量的大小，同时还表达了方向。一般来说二维向量可以表示平面中线段的量和方向，三维向量和表示空间中线段的量和方向。二阶张量即矩阵，我们可以看作是填满数字的一个表格，矩阵运算即一个表格和另外一个表格进行运算。当然理论上我们可以产生任意阶的张量，但在实际的机器学习算法运算中，我们使用得最多的还是一阶张量（向量）和二阶张量（矩阵）。
![](https://pic4.zhimg.com/v2-1cdaed3e8d9bbf40004492738c01c7af_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='591' height='349'></svg>)
一般来说，张量中每个元素的数据类型有以上几种，即浮点型和整数型，一般在神经网络中比较常用的是 32 位浮点型。




## **4. TensorFlow 机器**

在整个教程中，下面一张示意图将反复出现，这基本上是所有 TensorFlow 机器学习模型所遵循的构建流程，即构建计算图、馈送输入张量、更新权重并返回输出值。
![](https://pic3.zhimg.com/v2-b4070f6384abd7d946aab57126fdf7ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1232' height='716'></svg>)
在第一步使用 TensorFlow 构建计算图中，我们需要构建整个模型的架构。例如在神经网络模型中，我们需要从输入层开始构建整个神经网络的架构，包括隐藏层的数量、每一层神经元的数量、层级之间连接的情况与权重、整个网络每个神经元使用的激活函数等内容。此外，我们还需要配置整个训练、验证与测试的过程。例如在神经网络中，定义整个正向传播的过程与参数并设定学习率、正则化率和批量大小等各类训练超参数。第二步需要将训练数据或测试数据等馈送到模型中，TensorFlow 在这一步中一般需要打开一个会话（Session）来执行参数初始化和馈送数据等任务。例如在计算机视觉中，我们需要随机初始化整个模型参数数值，并将图像成批（图像数等于批量大小）地馈送到定义好的卷积神经网络中。第三步即更新权重并获取返回值，这个一般是控制训练过程与获得最终的预测结果。




## **TensorFlow 模型实战**




**TensorFlow 线性回归**




该教程前面介绍了很多线性回归的基本概念，包括直线拟合、损失函数、梯度下降等基础内容。我们一直认为线性回归是理解机器学习最好的入门模型，因为他的原理和概念十分简单，但又基本涉及到了机器学习的各个过程。总的来说，线性回归模型可以用下图概括：



![](https://pic3.zhimg.com/v2-afad7e6f3a4fa277fde60b7eafd7d366_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='743' height='482'></svg>)



其中「×」为数据点，我们需要找到一条直线以最好地拟合这些数据点。该直线和这些数据点之间的距离即损失函数，所以我们希望找到一条能令损失函数最小的直线。以下是使用 TensorFlow 构建线性回归的简单案例。




1. 构建目标函数（即「直线」）

目标函数即 H(x)=Wx+b，其中 x 为特征向量、W 为特征向量中每个元素对应的权重、b 为偏置项。
![](https://pic1.zhimg.com/v2-6df065c6bf89dc948c09914b999c3200_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='864' height='264'></svg>)
如上所示，我们定义了 y=wx+b 的运算，即我们需要拟合的一条直线。




2. 构建损失函数

下面我们需要构建整个模型的损失函数，即各数据点到该直线的距离，这里我们构建的损失函数为均方误差函数：
![](https://pic1.zhimg.com/v2-5061b1b5c10a2d59d9a428c7de412f74_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='570' height='104'></svg>)



该函数表明根据数据点预测的值和该数据点真实值之间的距离，我们可以使用以下代码实现：

> # cost/loss **function**
**cost** = **tf**.**reduce_mean**(tf.square(hypothesis - y_train))

其中 tf.square() 为取某个数的平方，而 tf.reduce_mean() 为取均值。




3. 采用梯度下降更新权重

> # Minimize
optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.01)
train = optimizer.minimize(cost)

为了寻找能拟合数据的最好直线，我们需要最小化损失函数，即数据与直线之间的距离，因此我们可以采用梯度下降算法：




4. 运行计算图执行训练

> # Launch the graph **in** a session.
sess = tf.Session()
# Initializes global variables **in** the graph.
sess.run(tf.global_variables_initializer())

# Fit the line
**for** step **in** range(2001):
   sess.run(train)
**if** step % 20 == 0:
       print(step, sess.run(cost), sess.run(W), sess.run(b))

上面的代码打开了一个会话并执行变量初始化和馈送数据。




最后，该课程给出了完整的实现代码，刚入门的读者可以尝试实现这一简单的线性回归模型：

> **import** tensorflow **as** tf
W = tf.Variable(tf.random_normal([1]), name='weight')
b = tf.Variable(tf.random_normal([1]), name='bias')

X = tf.placeholder(tf.float32, shape=[None])
Y = tf.placeholder(tf.float32, shape=[None])

# Our hypothesis XW+b
hypothesis = X * W + b
# cost/loss **function****cost** = **tf**.**reduce_mean**(tf.square(hypothesis - Y))
# **Minimize****optimizer** = **tf**.**train**.**GradientDescentOptimizer**(learning_rate=0.01)
**train** = **optimizer**.**minimize**(cost)

# **Launch****the****graph****in****a****session**.
**sess** = **tf**.**Session**()
# **Initializes****global****variables****in****the****graph**.
**sess**.**run**(tf.global_variables_initializer())

# **Fit****the****line****for****step****in****range**(2001):
**cost_val**, **W_val**, **b_val**, **_** = **sess**.**run**([cost, W, b, train],
                feed_dict={X: [1, 2, 3], Y: [1, 2, 3]})
**if****step** % 20 == 0:
**print**(step, cost_val, W_val, b_val)

下面让我们概览该课程更多的内容：




**Logistic 回归**




该课程照例先简介了 Logistic 回归的基本概念，如下展示了目标函数、损失函数和权重更新过程。
![](https://pic2.zhimg.com/v2-219bcf533866618ed63ca363321799fd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1284' height='764'></svg>)



后面展示了 Logistic 回归的实现代码：

> xy = np.loadtxt('data-03-diabetes.csv', delimiter=',', dtype=np.float32)
x_data = xy[:, 0:-1]
y_data = xy[:, [-1]]

# placeholders **for** a tensor that will be always fed.
X = tf.placeholder(tf.float32, shape=[None, 8])
Y = tf.placeholder(tf.float32, shape=[None, 1])

W = tf.Variable(tf.random_normal([8, 1]), name='weight')
b = tf.Variable(tf.random_normal([1]), name='bias')

# Hypothesis using sigmoid: tf.div(1., 1. + tf.exp(tf.matmul(X, W)))
hypothesis = tf.sigmoid(tf.matmul(X, W) + b)
# cost/loss **function****cost** = -**tf**.**reduce_mean**(Y * tf.log(hypothesis) + (1 - Y) * **tf**.**log**(1 - hypothesis))
**train** = **tf**.**train**.**GradientDescentOptimizer**(learning_rate=0.01).**minimize**(cost)

# **Accuracy****computation**
# **True****if****hypothesis**>0.5 **else****False****predicted** = **tf**.**cast**(hypothesis > 0.5, dtype=tf.float32)
**accuracy** = **tf**.**reduce_mean**(tf.cast(tf.equal(predicted, Y), **dtype**=**tf**.**float32**))

# **Launch****graph****with****tf**.**Session**() **as****sess**:
**sess**.**run**(tf.global_variables_initializer())

**feed** = {X: x_data, Y: y_data}
**for** step **in** range(10001):
       sess.run(train, feed_dict=feed)
**if** step % 200 == 0:
           print(step, sess.run(cost, feed_dict=feed))

   # Accuracy report
   h, c, a = sess.run([hypothesis, predicted, accuracy], feed_dict=feed)
   print("\nHypothesis: ", h, "\nCorrect (Y): ", c, "\nAccuracy: ", a)

**Softmax 分类**

下图展示了 Softmax 的基本方法，它可以产生和为 1 的类别概率。
![](https://pic3.zhimg.com/v2-e27622ce907d62a631981fb22d65b932_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='918' height='562'></svg>)
以下代码为 Softmax 分类器处理 MNIST 数据集：




> # weights & bias **for** nn layers
W = tf.Variable(tf.random_normal([784, 10]))
b = tf.Variable(tf.random_normal([10]))
hypothesis = tf.matmul(X, W) + b
# define cost/loss & optimizer
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=hypothesis, labels=Y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)
# initialize
sess = tf.Session()
sess.run(tf.global_variables_initializer())

# train my model
**for** epoch **in** range(training_epochs):
   avg_cost = 0
   total_batch = int(mnist.train.num_examples / batch_size)

**for** i **in** range(total_batch):
       batch_xs, batch_ys = mnist.train.next_batch(batch_size)
       feed_dict = {X: batch_xs, Y: batch_ys}
       c, _ = sess.run([cost, optimizer], feed_dict=feed_dict)
       avg_cost += c / total_batch

   print('Epoch:', '%04d' % (epoch + 1), 'cost =', '{:.9f}'.format(avg_cost))

print('Learning Finished!')

# Test model and check accuracy
correct_prediction = tf.equal(tf.argmax(hypothesis, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print('Accuracy:', sess.run(accuracy, feed_dict={X: mnist.test.images, Y: mnist.test.labels}))




**神经网络**

下图简要介绍了神经网络的运算过程，这一部分十分详细，对于初学者来说是难得的资料：



![](https://pic4.zhimg.com/v2-39ed53eb63f24979e94421ab4f4cb2ef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1330' height='416'></svg>)



下面是该教程采用神经网络解决异或问题的代码，异或问题是十分经典的任务，我们可以从该问题中理解神经网络的强大之处：

> x_data = np.array([[0, 0], [0, 1], [1, 0], [1, 1]], dtype=np.float32)
y_data = np.array([[0], [1], [1], [0]], dtype=np.float32)
X = tf.placeholder(tf.float32)
Y = tf.placeholder(tf.float32)

W1 = tf.Variable(tf.random_normal([2, 2]), name='weight1')
b1 = tf.Variable(tf.random_normal([2]), name='bias1')
layer1 = tf.sigmoid(tf.matmul(X, W1) + b1)

W2 = tf.Variable(tf.random_normal([2, 1]), name='weight2')
b2 = tf.Variable(tf.random_normal([1]), name='bias2')
hypothesis = tf.sigmoid(tf.matmul(layer1, W2) + b2)

# cost/loss **function****cost** = -**tf**.**reduce_mean**(Y * tf.log(hypothesis) + (1 - Y) * **tf**.**log**(1 - hypothesis))
**train** = **tf**.**train**.**GradientDescentOptimizer**(learning_rate=0.1).**minimize**(cost)
# **Accuracy****computation**
# **True****if****hypothesis**>0.5 **else****False****predicted** = **tf**.**cast**(hypothesis > 0.5, dtype=tf.float32)
**accuracy** = **tf**.**reduce_mean**(tf.cast(tf.equal(predicted, Y), **dtype**=**tf**.**float32**))
# **Launch****graph****with****tf**.**Session**() **as****sess**:
   # **Initialize****TensorFlow****variables****sess**.**run**(tf.global_variables_initializer())
**for****step****in****range**(10001):
**sess**.**run**(train, feed_dict={X: x_data, Y: y_data})
**if****step** % 100 == 0:
**print**(step, sess.run(cost, feed_dict={X: x_data, Y: y_data}), **sess**.**run**([W1, W2]))

   # **Accuracy****report****h**, **c**, **a** = **sess**.**run**([hypothesis, predicted, accuracy],
                      feed_dict={X: x_data, Y: y_data})
**print**("\nHypothesis: ", h, "\nCorrect: ", c, "\nAccuracy: ", a)







## **卷积神经网络**




第二天教程就正式进入卷积神经网络，我们只能用下图展示卷积神经网络大概的架构，更多资料请查看原课程课件：
![](https://pic4.zhimg.com/v2-c1840d239ed15979b926089ab9e25b5f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1171' height='482'></svg>)
该教程同样提供了很多卷积网络的实现代码，下面我们简要介绍一个简单的卷积神经网络实现过程，该卷积神经网络的架构如下： 
![](https://pic4.zhimg.com/v2-0f40574a2ffa88b4faf836b2334e9d3f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='999' height='535'></svg>)
下面的代码穿件了第一个卷积层，即上图卷积层 1 和池化层 1：

> # input placeholders
X = tf.placeholder(tf.float32, [None, 784])
X_img = tf.reshape(X, [-1, 28, 28, 1])   # img 28x28x1 (black/white)
Y = tf.placeholder(tf.float32, [None, 10])

# L1 ImgIn shape=(?, 28, 28, 1)
W1 = tf.Variable(tf.random_normal([3, 3, 1, 32], stddev=0.01))
#    Conv     -> (?, 28, 28, 32)
#    Pool     -> (?, 14, 14, 32)
L1 = tf.nn.conv2d(X_img, W1, strides=[1, 1, 1, 1], padding='SAME')
L1 = tf.nn.relu(L1)
L1 = tf.nn.max_pool(L1, ksize=[1, 2, 2, 1],
         strides=[1, 2, 2, 1], padding='SAME')
'''
Tensor("Conv2D:0", shape=(?, 28, 28, 32), dtype=float32)
Tensor("Relu:0", shape=(?, 28, 28, 32), dtype=float32)
Tensor("MaxPool:0", shape=(?, 14, 14, 32), dtype=float32)
'''

后面的代码构建了第二个卷积层，即上图中的卷积层 2 和池化层 2：

> '''
Tensor("Conv2D:0", shape=(?, 28, 28, 32), dtype=float32)
Tensor("Relu:0", shape=(?, 28, 28, 32), dtype=float32)
Tensor("MaxPool:0", shape=(?, 14, 14, 32), dtype=float32)
'''

# L2 ImgIn shape=(?, 14, 14, 32)
W2 = tf.Variable(tf.random_normal([3, 3, 32, 64], stddev=0.01))
#    Conv      ->(?, 14, 14, 64)
#    Pool      ->(?, 7, 7, 64)
L2 = tf.nn.conv2d(L1, W2, strides=[1, 1, 1, 1], padding='SAME')
L2 = tf.nn.relu(L2)
L2 = tf.nn.max_pool(L2, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
L2 = tf.reshape(L2, [-1, 7 * 7 * 64])
'''
Tensor("Conv2D_1:0", shape=(?, 14, 14, 64), dtype=float32)
Tensor("Relu_1:0", shape=(?, 14, 14, 64), dtype=float32)
Tensor("MaxPool_1:0", shape=(?, 7, 7, 64), dtype=float32)
Tensor("Reshape_1:0", shape=(?, 3136), dtype=float32)

最后我们只需要构建一个全连接层就完成了整个 CNN 架构的搭建，即用以下代码构建上图最后紫色的全连接层：

> '''
Tensor("Conv2D_1:0", shape=(?, 14, 14, 64), dtype=float32)
Tensor("Relu_1:0", shape=(?, 14, 14, 64), dtype=float32)
Tensor("MaxPool_1:0", shape=(?, 7, 7, 64), dtype=float32)
Tensor("Reshape_1:0", shape=(?, 3136), dtype=float32)
'''
L2 = tf.reshape(L2, [-1, 7 * 7 * 64])

# Final FC 7x7x64 inputs -> 10 outputs
W3 = tf.get_variable("W3", shape=[7 * 7 * 64, 10], initializer=tf.contrib.layers.xavier_initializer())
b = tf.Variable(tf.random_normal([10]))
hypothesis = tf.matmul(L2, W3) + b

# define cost/loss & optimizer
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=hypothesis, labels=Y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)

最后我们只需要训练该 CNN 就完成了整个模型：

> # initialize
sess = tf.Session()
sess.run(tf.global_variables_initializer())

# train my model
print('Learning stared. It takes sometime.')
**for** epoch **in** range(training_epochs):
   avg_cost = 0
   total_batch = int(mnist.train.num_examples / batch_size)   
**for** i **in** range(total_batch):
       batch_xs, batch_ys = mnist.train.next_batch(batch_size)       
       feed_dict = {X: batch_xs, Y: batch_ys}
       c, _, = sess.run([cost, optimizer], feed_dict=feed_dict)       
       avg_cost += c / total_batch
   print('Epoch:', '%04d' % (epoch + 1), 'cost =', '{:.9f}'.format(avg_cost))

print('Learning Finished!')

# Test model and check accuracy
correct_prediction = tf.equal(tf.argmax(hypothesis, 1), tf.argmax(Y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print('Accuracy:', sess.run(accuracy, feed_dict={X: mnist.test.images, Y: mnist.test.labels}))




**循环神经网络**




该教程第三天讲述了循环神经网络，下图展示了循环单元的展开，循环单元是处理时序数据的核心。更详细的资料请查看该课程课件。
![](https://pic1.zhimg.com/v2-5e3617847b23393d99cc5b7ed834c86c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1127' height='469'></svg>)






以下 TensorFlow 代码定义了简单的循环单元：

> # One cell RNN input_dim (4) -> output_dim (2)
hidden_size = 2
cell = tf.contrib.rnn.BasicRNNCell(num_units=hidden_size)  

x_data = np.array([[[1,0,0,0]]], dtype=np.float32) 
outputs, _states = tf.nn.dynamic_rnn(cell, x_data, dtype=tf.float32)  

sess.run(tf.global_variables_initializer())
pp.pprint(outputs.eval())

下面该课程展示了一个简单的卷积神经网络案例，如下所示，该案例训练一个 RNN 以输出「hihello」。
![](https://pic1.zhimg.com/v2-7968c19bea6396590eddcefbb35c7d28_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1315' height='498'></svg>)



1. 创建 RNN 单元

如下可知 TensorFlow 中一般可以创建 3 种 RNN 单元，即 RNN 单元、LSTM 单元和 GRU 单元

> # RNN model
rnn_cell = rnn_cell.BasicRNNCell(rnn_size)


rnn_cell = rnn_cell. BasicLSTMCell(rnn_size)
rnn_cell = rnn_cell. GRUCell(rnn_size)




2. 执行 RNN

> # RNN model
rnn_cell = rnn_cell.BasicRNNCell(rnn_size)

outputs, _states = tf.nn.dynamic_rnn(
                        rnn_cell, 
                        X,
                        initial_state=initial_state,     
                        dtype=tf.float32)

3. 设定 RNN 的参数

> hidden_size = 5      # output **from** the LSTM
input_dim = 5        # one-hot size
batch_size = 1       # one sentence
sequence_length = 6  # |ihello| == 6

4. 创建数据

> idx2char = ['h', 'i', 'e', 'l', 'o'] # h=0, i=1, e=2, l=3, o=4
x_data = [[0, 1, 0, 2, 3, 3]]        # hihell
x_one_hot = [[[1, 0, 0, 0, 0],       # h 0
              [0, 1, 0, 0, 0],       # i 1
              [1, 0, 0, 0, 0],       # h 0
              [0, 0, 1, 0, 0],       # e 2
              [0, 0, 0, 1, 0],       # l 3
              [0, 0, 0, 1, 0]]]      # l 3

y_data = [[1, 0, 2, 3, 3, 4]]        # ihello
X = tf.placeholder(tf.float32, 
[None, sequence_length, input_dim]) # X one-hot
Y = tf.placeholder(tf.int32, [None, sequence_length])  # Y label

5. 将数据馈送到 RNN 中

> X = tf.placeholder(
   tf.float32, [None, sequence_length, hidden_size])  # X one-hot
Y = tf.placeholder(tf.int32, [None, sequence_length])  # Y label

cell = tf.contrib.rnn.BasicLSTMCell(num_units=hidden_size, state_is_tuple=True)
initial_state = cell.zero_state(batch_size, tf.float32)
outputs, _states = tf.nn.dynamic_rnn(
   cell, X, initial_state=initial_state, dtype=tf.float32)

6. 创建序列损失函数

> outputs, _states = tf.nn.dynamic_rnn(
   cell, X, initial_state=initial_state, dtype=tf.float32)
weights = tf.ones([batch_size, sequence_length])

sequence_loss = tf.contrib.seq2seq.sequence_loss(
                       logits=outputs, targets=Y, weights=weights)
loss = tf.reduce_mean(sequence_loss)
train = tf.train.AdamOptimizer(learning_rate=0.1).minimize(loss)

7. 训练 RNN

这是最后一步，我们将打开一个 TensorFlow 会话完成模型的训练。

> prediction = tf.argmax(outputs, axis=2)

**with** tf.Session() **as** sess:
   sess.run(tf.global_variables_initializer())
**for** i **in** range(2000):
       l, _ = sess.run([loss, train], feed_dict={X: x_one_hot, Y: y_data})
       result = sess.run(prediction, feed_dict={X: x_one_hot})
       print(i, "loss:", l, "prediction: ", result, "true Y: ", y_data)

       # print char using dic
       result_str = [idx2char[c] **for** c **in** np.squeeze(result)]
       print("\tPrediction str: ", ''.join(result_str))




***声明：本文由机器之心原创出品，版权归作者所有，转载请查看要求，机器之心对于违规侵权者保有法律追诉权。***







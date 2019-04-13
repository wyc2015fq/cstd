
# CNN和VGGNet-16背后的架构 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月25日 08:18:46[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：88


[https://www.toutiao.com/a6646310796697534989/](https://www.toutiao.com/a6646310796697534989/)
2019-01-14 19:20:20
在机器学习方面，人工神经网络表现非常好。由于人工神经网络是非线性函数，它可以应用于求解问题中许多模式的运算。我们将人工神经网络(ANN)用于图像、音频、单词等各种分类任务中，也可用于时间序列等各种回归分析中。不同的人工神经网络有不同的用途，例如为了找到句子中的下一个序列词，我们使用了循环神经网络(RNN) LSTM，对于图像分类，我们使用卷积神经网络(CNN)。
但是在深入研究卷积神经网络之前让我们看一下神经网络的基本组成部分。
![CNN和VGGNet-16背后的架构](http://p1.pstatp.com/large/pgc-image/8ce042edb8d040e2abbbaa6ed50507ff)
神经网络基本上由三层组成：
输入层 ：它是我们为机器学习模型提供输入的层。该层中的神经元数量等于我们数据中的特征总数（图像的像素数）。
隐藏层:来自输入层的输入被输入到隐藏层。根据我们的模型和数据大小，可以有许多隐藏层。每个隐藏层可以有不同数量的神经元，这些神经元的数量通常大于特征的数量。每一层的输出由上一层的输出与该层的可学习权重的矩阵乘法来计算，然后加上可学习的偏差，再加上使网络非线性的激活函数。
输出层：隐藏层的输出然后被输入逻辑函数，如sigmoid或softmax，它将每个类的输出转换为每个类的概率分数。
![CNN和VGGNet-16背后的架构](http://p3.pstatp.com/large/pgc-image/4b6eec02c21741f498a9145e99862f5e)
FeedForward和BackPropagation
具有2个隐藏层的神经网络的基本python代码如下：
```python
activation_function = lambda x: 1.0/(1.0 + np.exp(-x))
input = np.random.randn(3,1)
W1 = np.random.randn(None, 1)
W2 = np.random.randn(None,1)
W3 = np.random.randn(None,1)
b1 = np.zeros(1)
b2 = np.zeros(1)
b3 = np.zeros(1)
hidden_1 = activation(np.dot(W1, input) + b1)
hidden_2 = activation(np.dot(W2, W1) + b2)
output = np.dot(W3, W2) + b3
```
![CNN和VGGNet-16背后的架构](http://p1.pstatp.com/large/pgc-image/b5402d4276d24f31bb3255cbe52c20c0)
W1，W2，W3，b1，b2，b3是使用Gradient Descent的可学习参数
# 卷积神经网络
卷积神经网络或covnets是共享它们的参数的神经网络。想象你有一个图像。它可以表示为一个长方体，有它的长度、宽度(图像的尺寸)和高度(图像通常有红色、绿色和蓝色通道)。
![CNN和VGGNet-16背后的架构](http://p3.pstatp.com/large/pgc-image/041bead3ee8c47d1aebfaf3e35cf5a08)
![CNN和VGGNet-16背后的架构](http://p1.pstatp.com/large/pgc-image/bab0f8d2213a4fc9b6f2ecf5cc4dcd1f)
现在想象一下这个图像的一小部分（patch）并在其上运行一个小的神经网络。现在将神经网络滑过整个图像，结果，我们将得到另一个具有不同宽度，高度和深度的图像。现在我们有更多的通道，但宽度和高度更小，而不仅仅是R，G和B通道。他的运算叫做卷积。如果patch大小与图像大小相同，则它将是常规神经网络。由于这个小patch，我们有更少的权重。
现在让我们谈谈整个卷积过程中涉及的一些数学。
卷积层由一组可学习的filters 组成（上图中的patch）。每个filter都具有较小的宽度和高度，深度与输入volume 相同（如果输入层是图像输入，则为3）。例如，如果我们必须在尺寸为34x34x3的图像上运行卷积。filters 的可能尺寸可以是axax3，其中'a'可以是3,5,7等（优选是奇数）但是与图像尺寸相比较小。
在forward pass间，我们逐步在整个输入volume 上滑动每个filter，其中每个步骤称为stride （对于高维图像可以具有值2或3或甚至4），并计算filters 的权重与输入volume的patch之间的点积。
当我们滑动filters 时，我们将为每个filter获得2-D输出，我们将它们堆叠在一起，因此，我们将得到深度等于filters 数量的输出volume 量。网络将学习所有filters 。
# 用于构建ConvNet的层
让我们通过运行尺寸为32 x 32 x 3的图像的covnets来举个例子。
输入层：此层保存图像的原始输入，宽度为32，高度为32，深度为3。
卷积层：此层通过计算所有filters 和图像patch之间的点积来计算输出volume 。假设我们对此图层使用总共12个filters ，我们将获得尺寸为32 x 32 x 12的输出volume 。
激活函数层：该层将元素激活函数应用于卷积层的输出。一些常见的激活函数是RELU：max（0，x），Sigmoid：1 /（1 + e ^ -x），Tanh，Leaky RELU等。volume保持不变，因此输出volume将具有32 x 32 x 12的尺寸。
池化层：该层周期性地插入到covnets中，其主要功能是减小volume的大小，使计算快速减少内存并防止过度拟合。两种常见类型的池化层是最大池化和平均池化。如果我们使用具有2 x 2 filters 和stride 2的最大池，则生成的volume将为16x16x12。
![CNN和VGGNet-16背后的架构](http://p1.pstatp.com/large/pgc-image/3e2ddc32979a49fd89b0c628a6a5aeaa)
降维公式
![CNN和VGGNet-16背后的架构](http://p1.pstatp.com/large/pgc-image/fd44c70c3acb4f398a2fe2b65da89b38)
全连接层：该层是常规神经网络层，它从前一层获取输入并计算类得分并输出大小等于类数的1-D数组。
![CNN和VGGNet-16背后的架构](http://p3.pstatp.com/large/pgc-image/6f7ea20a88e443fdb5f8c1e0f865812c)

# VGGNet
![CNN和VGGNet-16背后的架构](http://p1.pstatp.com/large/pgc-image/1666acab088c4a438b7e4fee7f30f21b)
VGGNet的结构
现在VGGNet是经典的卷积神经网络之一。为了尝试一些实验，我创建了一个与VGGNet在架构上几乎相似的模型。现在，因为我已经在CIFAR-10数据集上进行了训练，因此我删除了卷积的最后一层，因此我的模型只比原始VGGNet-16模型少了一个卷积层。
```python
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
cifar_data = tf.keras.datasets.cifar10
(x_train, y_train), (x_test, y_test) = cifar_data.load_data() 
print(x_train.shape)
keep_rate = 0.8
keep_prob = tf.placeholder(tf.float32)
n_classes = 10
batch_sz = 1000
x = tf.placeholder("float", [None, 3072])
y = tf.placeholder("float", [None, n_classes])
def oneHotencode(Y):
 one_hot_ = np.zeros([Y.shape[0], n_classes])
 for i in range(Y.shape[0]):
 one_hot_[i, Y[i]] = 1.
 
 return one_hot_
def conv2d(x, W):
 return tf.nn.conv2d(x, W, strides = [1,1,1,1], padding = 'SAME')
def maxpool2d(x):
 return tf.nn.max_pool(x, ksize = [1,2,2,1], strides = [1,2,2,1], padding = 'SAME')
def convolutional_neural_network(x):
 weights = {'W_conv1' : tf.Variable(tf.random_normal([3,3,3,64])),
 'W_conv2' : tf.Variable(tf.random_normal([3,3,64,64])),
 'W_conv3' : tf.Variable(tf.random_normal([3,3,64,128])),
 'W_conv4' : tf.Variable(tf.random_normal([3,3,128,128])),
 'W_conv5': tf.Variable(tf.random_normal([3,3,128,256])),
 'W_conv6' : tf.Variable(tf.random_normal([3,3,256,256])),
 'W_conv7' : tf.Variable(tf.random_normal([3,3,256,256])),
 'W_conv8' : tf.Variable(tf.random_normal([3,3,256,512])),
 'W_conv9' : tf.Variable(tf.random_normal([3,3,512,512])),
 'W_conv10' : tf.Variable(tf.random_normal([3,3,512,512])),
 'W_fc1' : tf.Variable(tf.random_normal([2*2*512, 1024])),
 'W_fc2' : tf.Variable(tf.random_normal([1024, 1024])),
 'W_fc3' : tf.Variable(tf.random_normal([1024, n_classes]))}
 
 biases = {'b_conv1' : tf.Variable(tf.random_normal([64])),
 'b_conv2' : tf.Variable(tf.random_normal([64])),
 'b_conv3' : tf.Variable(tf.random_normal([128])),
 'b_conv4' : tf.Variable(tf.random_normal([128])),
 'b_conv5' : tf.Variable(tf.random_normal([256])),
 'b_conv6' : tf.Variable(tf.random_normal([256])),
 'b_conv7' : tf.Variable(tf.random_normal([256])),
 'b_conv8' : tf.Variable(tf.random_normal([512])),
 'b_conv9' : tf.Variable(tf.random_normal([512])),
 'b_conv10' : tf.Variable(tf.random_normal([512])),
 'b_fc1' : tf.Variable(tf.random_normal([1024])),
 'b_fc2' : tf.Variable(tf.random_normal([1024])),
 'b_fc3' : tf.Variable(tf.random_normal([n_classes]))}
 
 x = tf.reshape(x, [-1, 32, 32, 3])
 
 # Conv1
 conv1 = tf.nn.relu(conv2d(x, weights['W_conv1']) + biases['b_conv1'])
 
 # Conv2
 conv2 = tf.nn.relu(conv2d(conv1, weights['W_conv2']) + biases['b_conv2'])
 
 # Max pool
 conv2 = maxpool2d(conv2)
 
 # Conv3
 conv3 = tf.nn.relu(conv2d(conv2, weights['W_conv3']) + biases['b_conv3'])
 
 # Conv4
 
 conv4 = tf.nn.relu(conv2d(conv3, weights['W_conv4']) + biases['b_conv4'])
 
 # maxpool
 
 conv4 = maxpool2d(conv4)
 
 # Conv5
 conv5 = tf.nn.relu(conv2d(conv4, weights['W_conv5']) + biases['b_conv5'])
 
 #Conv6
 conv6 = tf.nn.relu(conv2d(conv5, weights['W_conv6']) + biases['b_conv6'])
 
 #Conv 7
 conv7 = tf.nn.relu(conv2d(conv6, weights['W_conv7']) + biases['b_conv7'])
 
 # max pool 
 conv7 = maxpool2d(conv7)
 
 #Conv 8
 conv8 = tf.nn.relu(conv2d(conv7, weights['W_conv8']) + biases['b_conv8'])
 
 #Conv9
 
 conv9 = tf.nn.relu(conv2d(conv8, weights['W_conv9']) + biases['b_conv9'])
 
 #Conv10
 conv10 = tf.nn.relu(conv2d(conv9, weights['W_conv10']) + biases['b_conv10'])
 
 # max pool
 conv10 = maxpool2d(conv10)
 
 #fc1 
 fc1 = tf.reshape(conv10, [-1, 2*2*512])
 fc1 = tf.nn.relu(tf.matmul(fc1, weights['W_fc1']) + biases['b_fc1'])
 fc1 = tf.nn.dropout(fc1, keep_rate)
 
 #fc2 
 fc2 = tf.nn.relu(tf.matmul(fc1, weights['W_fc2']) + biases['b_fc2'])
 fc2 = tf.nn.dropout(fc2, keep_rate)
 
 #Output Layer
 output = tf.nn.softmax(tf.matmul(fc2, weights['W_fc3']) + biases['b_fc3'])
 
 return output
def CNN(trainX, trainY, testX, testY):
 
 prediction = convolutional_neural_network(x)
 cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(logits= prediction,
 labels = y))
 optimizer = tf.train.AdamOptimizer().minimize(cost)
 
 n_epochs = 5
 n_batches = trainX.shape[0] // batch_sz
 
 train_cost = []
 
 config = tf.ConfigProto()
 config.gpu_options.allocator_type = 'BFC'
 
 sess = tf.InteractiveSession(config=config)
 sess.run(tf.global_variables_initializer())
 
 for i in range(n_epochs):
 epoch_loss = 0
 print("Epoch : {}".format(i))
 for j in range(n_batches):
 batchX, batchY = trainX[j * batch_sz : (j+1) * batch_sz], trainY[j * batch_sz : (j+1) * batch_sz]
 
 _,c = sess.run([optimizer, cost], feed_dict = {x: batchX, y: batchY})
 
 epoch_loss += c
 
 if j % batch_sz == 0:
 print("Batch : {} , Loss : {:.3f}".format(j, c))
 
 print("Epoch : {} / {} , Overall loss: {:.3f}".format(i, n_epochs, epoch_loss))
 train_cost.append(epoch_loss)
 
 correct = tf.equal(tf.argmax(prediction, 1), tf.argmax(y, 1))
 accuracy = tf.reduce_mean(tf.cast(correct, 'float'))
 
 print("Accuracy: {:.2f} %".format(accuracy.eval({x: testX, y: testY})))
 
 
 plt.title("Training Cost")
 plt.plot(train_cost)
 plt.show()
if __name__ == '__main__':
 
 y_train_one_hot = oneHotencode(y_train)
 y_test_one_hot = oneHotencode(y_test)
 CNN(x_train.reshape([-1, 3072]), y_train_one_hot, x_test.reshape([-1, 3072]), y_test_one_h
```
![CNN和VGGNet-16背后的架构](http://p9.pstatp.com/large/pgc-image/e2472788d0f4496f938d9f877167a9f7)
![CNN和VGGNet-16背后的架构](http://p1.pstatp.com/large/pgc-image/4bf1281853bc4df0b5a9dcc672836340)
![CNN和VGGNet-16背后的架构](http://p1.pstatp.com/large/pgc-image/92b2c2ebc77c47cca672ddbde731e04d)
![CNN和VGGNet-16背后的架构](http://p3.pstatp.com/large/pgc-image/82917f8a0e2542d68df9a74dd88412c0)


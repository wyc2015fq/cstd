# TensorFlow实现卷积神经网络 - 数据之美的博客 - CSDN博客
2017年06月12日 17:43:09[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：313
卷积神经网络能够自动从图像中提取有用的特征，并在图像分类领域取得了巨大的成功，关于卷积神经网络的理论网上已经有很多，这里主要是关于其实现部分。虽然以前使用Theano框架实现过，但因为最近项目需要使用TensorFlow框架，这里则采用tensorflow实现该模型。总的来说，同Theano类似，TensorFlow也采用的是张量流图的形式编程，确定出输入变量和输出变量的关系后，在Session中输入数据集并运行即可。本文对编程过程中遇到的问题进行了记录，以便后来翻看。
系统环境：TensorFlow0.8.0，cuda7.5， cudnn7.0，RedHat系统；
硬件配置：一块Tesla k20c显卡
代码：
**[python]**[view
 plain](http://blog.csdn.net/diligent_321/article/details/52955881#)[copy](http://blog.csdn.net/diligent_321/article/details/52955881#)
- import tensorflow as tf  
- import numpy as np  
- from tensorflow.examples.tutorials.mnist import input_data  
- 
- def weight_variable(shape):  
- return tf.Variable(initial_value=tf.truncated_normal(shape=shape, stddev=0.1), name='weight')  
- 
- def bias_variable(shape):  
- return tf.Variable(initial_value=tf.constant(value=0.1, shape=shape), name='bias')  
- 
- def conv_2d(x, w):  
- return tf.nn.conv2d(input=x, filter=w, strides=[1, 1, 1, 1], padding="SAME")  
- 
- def max_pool_2x2(x):  
- return tf.nn.max_pool(value=x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME') #!!!!!!
- def evaluate(y, y_):  
-     y = tf.arg_max(input=y, dimension=1)  
-     y_ = tf.arg_max(input=y_, dimension=1)  
- return tf.reduce_mean(input_tensor=tf.cast(tf.equal(y, y_), tf.float32))      
- 
- def test_cnn(batch_size=50, lr=0.0001, num_iter=20000):   
-     dataset = input_data.read_data_sets(train_dir='MNIST_data/', one_hot=True)  
- 
-     x = tf.placeholder(dtype=tf.float32, shape=[None, 784], name='images') #后面的卷积操作输入参数必须为‘float32’或者‘float64’
-     y = tf.placeholder(dtype=tf.float32, shape=[None, 10], name='labels')  
- 
-     w_conv1 = weight_variable(shape=[5, 5, 1, 32])  
-     b_conv1 = bias_variable(shape=[32])  
-     reshape_x = tf.reshape(x, shape=[-1, 28, 28, 1])  #省略的形式区别于占位符!!!!!!
-     conv1_out = tf.nn.relu(conv_2d(reshape_x, w_conv1)+b_conv1)      
-     pool1_out = max_pool_2x2(conv1_out)  
- 
-     w_conv2 = weight_variable(shape=[5, 5, 32, 64])  
-     b_conv2 = bias_variable(shape=[64])  
-     conv2_out = tf.nn.relu(conv_2d(pool1_out, w_conv2)+b_conv2)  
-     pool2_out = max_pool_2x2(conv2_out)  
- 
-     full_connected_in = tf.reshape(pool2_out, shape=[-1, 7*7*64])  
-     w_full_connected = weight_variable(shape=[7*7*64, 1024])  
-     b_full_connected = bias_variable(shape=[1024])  
-     full_connected_out1 = tf.nn.relu(tf.matmul(full_connected_in, w_full_connected)+b_full_connected)  
-     dropout_prob = tf.placeholder(dtype=tf.float32, name='dropout_probability')  
-     full_connected_out = tf.nn.dropout(x=full_connected_out1, keep_prob=dropout_prob) #drop out防止过拟合
- 
-     w_softmax = weight_variable(shape=[1024, 10])  
-     b_softmax = bias_variable(shape=[10])      
-     softmax_in = tf.matmul(full_connected_out, w_softmax)+b_softmax  
-     softmax_out = tf.nn.softmax(logits=softmax_in, name='softmax_layer')  
-     Loss = tf.nn.softmax_cross_entropy_with_logits(logits=softmax_in, labels=y)  
-     Step_train = tf.train.AdamOptimizer(learning_rate=lr).minimize(loss=Loss)  
-     accuracy = evaluate(y, softmax_out) #在测试数据集上评估算法的准确率    
-     initialized_variables = tf.initialize_all_variables()  
- 
- print('Start to train the convolutional neural network......')  
-     sess = tf.Session()  
-     sess.run(fetches=initialized_variables)  
- for iter in range(num_iter):  
-         batch = dataset.train.next_batch(batch_size=batch_size)  
-         sess.run(fetches=Step_train, feed_dict={x:batch[0], y:batch[1], dropout_prob:0.5})  
- if (iter+1)%100==0:  #计算在当前训练块上的准确率
-             Accuracy = sess.run(fetches=accuracy, feed_dict={x:batch[0], y:batch[1], dropout_prob:1})  
- print('Iter num %d ,the train accuracy is %.3f' % (iter+1, Accuracy))  
- 
-     Accuracy = sess.run(fetches=accuracy, feed_dict={x:dataset.test.images, y:dataset.test.labels, dropout_prob:1})  
-     sess.close()  
- print('Train process finished, the best accuracy is %.3f' % Accuracy)  
- 
- if __name__ == '__main__':  
-     test_cnn()  
**对代码中内容的解释：**
这段代码创建了一个卷积神经网络，用于解决MNIST数据集的分类问题，这也是一个经典的分类问题。整个网络的结构为：输入层+卷积层+池化层+卷积层+池化层+全连接层+softmax分类器。每迭代100次，输出参数在当前训练块上的准确率。为了防止过拟合，在全连接层的输出采用了drop out（一种类似于集成学习的思想，详细解释可自行百科）的策略。
**编程值得注意的细节：**
（1） 因为在训练和[测试](http://lib.csdn.net/base/softwaretest)阶段的dropout_prob取值不一样，因此这里定义成占位符，而不是常量输入；
（2）tf.placeholder()函数中的shape参数可省略，或者使用None关键字省略其中的某一维度大小，而其他维度可以用于校验输入数据集的大小格式；tf.reshape()函数中的shape参数不可以省略，仅仅可以省略第一维度，与tf.placeholder()函数不同的是，此时采用-1表示第一维度，其真实值可以自动计算出来，且至多省略一个维度；
**部分TensorFlow自带函数的详细解释：**
（1）conv2d(input, filter, strides, padding,use_cudnn_on_gpu=None, data_format=None, name=None)
> 
input: A Tensor. Must be one of the following types: float32, float64.
> 
filter: A Tensor. Must have the same type as input.
 （滤波器也即权重参数，而权值通常为小数，故占位符输入x也要定义成浮点数据类型）
strides: A list of ints.1-D
 of length 4. The stride of the sliding window for each dimension of input. Must be in the same order as the dimension
 specified with format.（表示进行卷积操作时，模板窗口沿各个维度滑动的步长）
padding: A string from: “SAME”,
 “VALID”. The type of padding algorithm to use. padding="SAME"表示卷积时在输入图像四周进行填充，使卷积后图像的尺寸不变；padding="VALID"则与此相反；
use_cudnn_on_gpu: An optional bool. Defaults to True. （cudnn是NVIDIA公司开发的[深度学习](http://lib.csdn.net/base/deeplearning)库，介于cuda层和tensorflow层之间）
（2）max_pool(value, ksize, strides, padding,data_format='NHWC', name=None)
> 
ksize: A list of ints that has length >= 4. The size of the window for each dimension of the input tensor. （表示各个维度池化窗口的大小，若某个维度取值为1，则表示在该维度上不进行池化）strides: 表示进行池化操作时，模板窗口沿各个维度滑动的步长。值得一提的是，strides大小和ksize的大小相同，其效果等价于只对单个“feature map”进行缩放；其他参数：同conv2d中的解释；
程序运行结果：
Extracting MNIST_data/train-images-idx3-ubyte.gz
Extracting MNIST_data/train-labels-idx1-ubyte.gz
Extracting MNIST_data/t10k-images-idx3-ubyte.gz
Extracting MNIST_data/t10k-labels-idx1-ubyte.gz
Start to train the convolutional neural network......
Iter num 100 ,the train accuracy is 0.920
Iter num 200 ,the train accuracy is 0.840
Iter num 300 ,the train accuracy is 0.960
Iter num 400 ,the train accuracy is 0.920
Iter num 500 ,the train accuracy is 0.920
Iter num 600 ,the train accuracy is 0.940
、、、、、、
Iter num 19500 ,the train accuracy is 1.000
Iter num 19600 ,the train accuracy is 1.000
Iter num 19700 ,the train accuracy is 1.000
Iter num 19800 ,the train accuracy is 1.000
Iter num 19900 ,the train accuracy is 1.000
Iter num 20000 ,the train accuracy is 1.000
Train process finished, the best accuracy is 0.992
参考资料：http://www.tensorfly.cn/tfdoc/tutorials/mnist_pros.html

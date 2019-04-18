# 自编码器及其tensorflow实现 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:09:57[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：1884

# 自编码器理论
自编码器（AutoEncoder）顾名思义，就是可以用自身的高阶特征编码自己。自编码器实际上也是一种神经网络，它的输入和输入的维度是一样的。借助稀疏编码的思想，目标是使用稀疏的一些高阶特征重新组合来重构自己。
早年在关于稀疏编码（Sparse Coding）的研究中，通过对大量黑白风景照片提取16*16的图像碎片分析，研究发现几乎所有的图像碎片都可以由64种正交的边组合得到，并且组合出一张图像碎片需要的边的数量是很少的，也就是稀疏的。声音也有同样的情况，大量未标注的音频中可以得到20种基本结构，绝大多数声音都可以由这些基本的结构线性组合得到。这就是特征的稀疏表达，通过少量的基本特征组合、拼装得到更高层抽象的特征。
说到自编码器，就必须要提到Hinton在science上发表的文章Reducing the dimensionality of data with neural networks，这篇文章系统讲解了使用自编码器对数据进行降维的方法，随后发展出了DBN（Deep Belief Networks, DBN），为训练很深的网络提供了一个可行的方案。也就是用无监督的逐层训练提取特征，将网络的权重初始化到一个比较好的状态这种无监督的逐层训练，和自编码器是非常相似的。
基本的自编码器模型是一个简单的三层神经网络结构：一个输入层、一个隐藏层和一个输出层。其中输出层和输入层具有相同的维数。
![1240](https://upload-images.jianshu.io/upload_images/4685306-43db2b39f47c75fd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
自编码器包含两部分：编码器（encoder）和解码器（decoder）。从输入层到隐藏层是编码的过程，从隐藏层到输出是解码的过程。
![1240](https://upload-images.jianshu.io/upload_images/4685306-b03c78f07c2a46bb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
如果仅仅是简单地将输入复制到输出，这样的建模是没有意义的。因此，自编码器的网络在设计上，经常采用欠完备（undercomplete）的形式，限制隐藏层h的维度比输入x小。学习过程可以定义为最小化损失函数：
![png.latex?L(%5Ctextbf%7Bx%7D,g(f(%5Ctextbf%7Bx%7D)))](http://latex.codecogs.com/png.latex?L(%5Ctextbf%7Bx%7D,g(f(%5Ctextbf%7Bx%7D))))
损失函数可以定义为：
![1240](https://upload-images.jianshu.io/upload_images/4685306-ed27d7d4e45289fa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
当损失函数取均方误差是，欠完备的自编码器会学习出PCA相同的子空间。
此外，还有一类自编码器，叫做正则自编码器，正则自编码器通过定义合适的损失函数使得模型具有对输入特性的学习能力，并不限制隐层维度和编码器自身的深度。
正则自编码器有很多形式，例如稀疏自编码器，去噪编码器等。稀疏自编码器可以认为是带有潜变量的生成模型的近似最大似然训练，去噪自编码器将输入中添加了噪声，训练网络隐式地去学习数据的分布特征。
# 自编码器的tensorflow实现
下面将给出自编码器的tensorflow实现。
首先导入库和数据。
```
import numpy as np
import sklearn.preprocessing as prep
import tensorflow as tf
from tensorflow.examples.tutorials.mnist import input_data
```
自编码器中用到了xavier initialization初始化参数：
```
# define xavier initialization function, mean distribution or gauss distribution
# mean = 0, var = 2/(n_in+n_out) 
def xavier_init(fan_in, fan_out, constant=1):
    """xavier initialization function
    fan_in: input node number
    fan_out: output node number"""
    low = -constant*np.sqrt(6/(fan_in+fan_out))
    high = constant*np.sqrt(6/(fan_in+fan_out))
    return tf.random_uniform((fan_in, fan_out), minval=low, maxval=high, dtype=tf.float32)
```
定义一个去噪自编码器的类，包括一个构建函数**init**(),完成一些成员变量输出化和网络构建，定义权重初始化函数_initialize_weights()，定义损失函数cost和单步训练函数partial_fit().
```
class AdditiveGaussianNoiseAutoencoder(object):   
    # define construct function
    def __init__(self, n_input, n_hidden, transfer_function=tf.nn.softplus, 
                 optimizer=tf.train.AdamOptimizer(), scale = 0.1):
        self.n_input = n_input
        self.n_hidden = n_hidden
        self.transfer = transfer_function
        self.scale = tf.placeholder(tf.float32)
        self.training_scale = scale
        network_weights = self._initialize_weights()
        self.weights = network_weights
       # define auto-encoder net structure
        with tf.name_scope('input'):
            self.x = tf.placeholder(tf.float32, [None, self.n_input])
        with tf.name_scope('hidden_layr'):
            self.hidden = self.transfer(tf.add(tf.matmul(self.x+scale*tf.random_normal((n_input,)),
                                                     self.weights['w1']), self.weights['b1']))
            tf.summary.histogram('hidden',self.hidden)
           # tf.summary.image('hidden_image',self.hidden)
        with tf.name_scope('output_layr'):
            self.reconstruction = tf.add(tf.matmul(self.hidden,self.weights['w2']), self.weights['b2'])
        # define loss function
        with tf.name_scope('loss_func'):
            self.cost = 0.5*tf.reduce_mean(tf.pow(tf.subtract(self.reconstruction,self.x),2.0))
        self.optimizer = optimizer.minimize(self.cost)
        # initialize all variables
        init = tf.global_variables_initializer()
        self.sess = tf.Session()
        self.sess.run(init)
        self.merged = tf.summary.merge_all()
        # parameter initialize function
    def _initialize_weights(self):
        all_weights = dict()
        all_weights['w1'] = tf.Variable(xavier_init(self.n_input,self.n_hidden))
        all_weights['b1'] = tf.Variable(tf.zeros([self.n_hidden],dtype=tf.float32))
        all_weights['w2'] = tf.Variable(tf.zeros([self.n_hidden, self.n_input],dtype=tf.float32))
        all_weights['b2'] = tf.Variable(tf.zeros([self.n_input],dtype=tf.float32))
        return all_weights
    # 1 step train function
    def partial_fit(self, X):
        cost, opt, merged = self.sess.run((self.cost, self.optimizer, self.merged),feed_dict={self.x:X, self.scale:self.training_scale})
        return cost, merged
    # loss function
    def calc_total_cost(self,X):
        return self.sess.run(self.cost, feed_dict={self.x:X, self.scale:self.training_scale})
```
之后在主函数中对训练数据进行预处理，训练网络：
```
if __name__  == '__main__':
    mnist = input_data.read_data_sets('MNIST_DATA', one_hot=True)
    logdir = './auto_encoder_logdir'
    summary_writer = tf.summary.FileWriter(logdir)
    with tf.Graph().as_default():
        # define standard scale fucntion
        def standard_scale(X_train, X_test):
            preprocessor = prep.StandardScaler().fit(X_train)
            X_train = preprocessor.transform(X_train)
            X_test = preprocessor.transform(X_test)
            return X_train, X_test
        # define get random block function
        def get_random_block_from_data(data, batch_size):
            start_index = np.random.randint(0, len(data)-batch_size)
            return data[start_index:(start_index+batch_size)]
        X_train, X_test = standard_scale(mnist.train.images, mnist.test.images)
        n_samples = int(mnist.train.num_examples)        
        training_epochs = 20
        batch_size = 128
        display_step = 2
        autoencoder = AdditiveGaussianNoiseAutoencoder(n_input = 784, 
                                                       n_hidden = 200,
                                                       transfer_function=tf.nn.softplus,
                                                       optimizer = tf.train.AdamOptimizer(learning_rate=0.001),
                                                       scale = 0.01
                                                       )
        # training process
        for epoch in range(training_epochs):
            avg_cost = 0
            total_batch = int(n_samples/batch_size)
            for i in range(total_batch):
                batch_xs = get_random_block_from_data(X_train, batch_size)
                #cost = autoencoder.partial_fit(batch_xs)
                cost, merged = autoencoder.partial_fit(batch_xs)
                summary_writer.add_summary(merged, i)
                avg_cost += cost/n_samples*batch_size 
                if epoch%display_step == 0:
                    print('Epoch:','%04d'%(epoch+1), 'cost=','{:.9f}'.format(avg_cost))
            print('Total cost:'+str(autoencoder.calc_total_cost(X_test)))
        summary_writer.close()
```
网络训练完成后，但是tensorboard可视化的效果并不好，无法显示网络结构。待修改。另外还有一个问题就是，如何可视化特征提取的结果？
未完待续……
参考：
[http://blog.csdn.net/pi9nc/article/details/27711441](http://blog.csdn.net/pi9nc/article/details/27711441)
《深度学习》
《tensorflow实战》

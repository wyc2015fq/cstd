# 使用DCGAN实现人脸图像生成-流浪的鱼-51CTO博客
### DCGAN介绍
原始的GAN网络在训练过程中生成者生成图像质量不太稳定，无法得到高质量的生成者网络，导致这个问题的主要原因是生成者与判别者使用相同的反向传播网络，对生成者网络的改进就是用卷积神经网络替代原理的MLP实现稳定生成者网络，生成高质量的图像。这个就是Deep Convolutional Generative Adversarial Network (DCGAN)的由来。相比GAN，DCGAN把原来使用MLP的地方都改成了CNN，同时去掉了池化层，改变如下：
- 判别器使用正常卷积，最后一层使用全连接层做预测判别
- 生成器根据输入的随机噪声，通过卷积神经网络生成一张图像
- 无论是生成器还是判别器都在卷积层后面有BN层
- 生成器与判别器分别使用relu与leaky relu作为激活函数， 除了生成器的最后一层
- 生成器使用转置/分步卷积、判别器使用正常卷积。
最终DCGAN的网络模型如下：
![使用DCGAN实现人脸图像生成](https://s1.51cto.com/images/blog/201901/31/d4f214f40e9e680c0b829622dadbd31b.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
其中基于卷积神经网络的生成器模型如下：![使用DCGAN实现人脸图像生成](https://s1.51cto.com/images/blog/201901/31/acf0f155d4e8b3cbcb451e6ddedb0ef5.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
判别器模型如下：![使用DCGAN实现人脸图像生成](https://s1.51cto.com/images/blog/201901/31/b5f067cb9c0fe282534824c3cbe58aef.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
### 代码实现：
生成器：
```
class Generator:
    def __init__(self, depths=[1024, 512, 256, 128], s_size=4):
        self.depths = depths + [3]
        self.s_size = s_size
        self.reuse = False
    def __call__(self, inputs, training=False):
        inputs = tf.convert_to_tensor(inputs)
        with tf.variable_scope('g', reuse=self.reuse):
            # reshape from inputs
            with tf.variable_scope('reshape'):
                outputs = tf.layers.dense(inputs, self.depths[0] * self.s_size * self.s_size)
                outputs = tf.reshape(outputs, [-1, self.s_size, self.s_size, self.depths[0]])
                outputs = tf.nn.relu(tf.layers.batch_normalization(outputs, training=training), name='outputs')
            # deconvolution (transpose of convolution) x 4
            with tf.variable_scope('deconv1'):
                outputs = tf.layers.conv2d_transpose(outputs, self.depths[1], [5, 5], strides=(2, 2), padding='SAME')
                outputs = tf.nn.relu(tf.layers.batch_normalization(outputs, training=training), name='outputs')
            with tf.variable_scope('deconv2'):
                outputs = tf.layers.conv2d_transpose(outputs, self.depths[2], [5, 5], strides=(2, 2), padding='SAME')
                outputs = tf.nn.relu(tf.layers.batch_normalization(outputs, training=training), name='outputs')
            with tf.variable_scope('deconv3'):
                outputs = tf.layers.conv2d_transpose(outputs, self.depths[3], [5, 5], strides=(2, 2), padding='SAME')
                outputs = tf.nn.relu(tf.layers.batch_normalization(outputs, training=training), name='outputs')
            with tf.variable_scope('deconv4'):
                outputs = tf.layers.conv2d_transpose(outputs, self.depths[4], [5, 5], strides=(2, 2), padding='SAME')
            # output images
            with tf.variable_scope('tanh'):
                outputs = tf.tanh(outputs, name='outputs')
        self.reuse = True
        self.variables = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES, scope='g')
        return outputs
```
判别器：
```
class Discriminator:
    def __init__(self, depths=[64, 128, 256, 512]):
        self.depths = [3] + depths
        self.reuse = False
    def __call__(self, inputs, training=False, name=''):
        def leaky_relu(x, leak=0.2, name=''):
            return tf.maximum(x, x * leak, name=name)
        outputs = tf.convert_to_tensor(inputs)
        with tf.name_scope('d' + name), tf.variable_scope('d', reuse=self.reuse):
            # convolution x 4
            with tf.variable_scope('conv1'):
                outputs = tf.layers.conv2d(outputs, self.depths[1], [5, 5], strides=(2, 2), padding='SAME')
                outputs = leaky_relu(tf.layers.batch_normalization(outputs, training=training), name='outputs')
            with tf.variable_scope('conv2'):
                outputs = tf.layers.conv2d(outputs, self.depths[2], [5, 5], strides=(2, 2), padding='SAME')
                outputs = leaky_relu(tf.layers.batch_normalization(outputs, training=training), name='outputs')
            with tf.variable_scope('conv3'):
                outputs = tf.layers.conv2d(outputs, self.depths[3], [5, 5], strides=(2, 2), padding='SAME')
                outputs = leaky_relu(tf.layers.batch_normalization(outputs, training=training), name='outputs')
            with tf.variable_scope('conv4'):
                outputs = tf.layers.conv2d(outputs, self.depths[4], [5, 5], strides=(2, 2), padding='SAME')
                outputs = leaky_relu(tf.layers.batch_normalization(outputs, training=training), name='outputs')
            with tf.variable_scope('classify'):
                batch_size = outputs.get_shape()[0].value
                reshape = tf.reshape(outputs, [batch_size, -1])
                outputs = tf.layers.dense(reshape, 2, name='outputs')
        self.reuse = True
        self.variables = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES, scope='d')
        return outputs
```
损失函数与训练
```
def loss(self, traindata):
    """build models, calculate losses.
    Args:
        traindata: 4-D Tensor of shape `[batch, height, width, channels]`.
    Returns:
        dict of each models' losses.
    """
    generated = self.g(self.z, training=True)
    g_outputs = self.d(generated, training=True, name='g')
    t_outputs = self.d(traindata, training=True, name='t')
    # add each losses to collection
    tf.add_to_collection(
        'g_losses',
        tf.reduce_mean(
            tf.nn.sparse_softmax_cross_entropy_with_logits(
                labels=tf.ones([self.batch_size], dtype=tf.int64),
                logits=g_outputs)))
    tf.add_to_collection(
        'd_losses',
        tf.reduce_mean(
            tf.nn.sparse_softmax_cross_entropy_with_logits(
                labels=tf.ones([self.batch_size], dtype=tf.int64),
                logits=t_outputs)))
    tf.add_to_collection(
        'd_losses',
        tf.reduce_mean(
            tf.nn.sparse_softmax_cross_entropy_with_logits(
                labels=tf.zeros([self.batch_size], dtype=tf.int64),
                logits=g_outputs)))
    return {
        self.g: tf.add_n(tf.get_collection('g_losses'), name='total_g_loss'),
        self.d: tf.add_n(tf.get_collection('d_losses'), name='total_d_loss'),
    }
def train(self, losses, learning_rate=0.0002, beta1=0.5):
    """
    Args:
        losses dict.
    Returns:
        train op.
    """
    g_opt = tf.train.AdamOptimizer(learning_rate=learning_rate, beta1=beta1)
    d_opt = tf.train.AdamOptimizer(learning_rate=learning_rate, beta1=beta1)
    g_opt_op = g_opt.minimize(losses[self.g], var_list=self.g.variables)
    d_opt_op = d_opt.minimize(losses[self.d], var_list=self.d.variables)
    with tf.control_dependencies([g_opt_op, d_opt_op]):
        return tf.no_op(name='train')
```
### 训练与运行
![使用DCGAN实现人脸图像生成](https://s1.51cto.com/images/blog/201901/31/dc0494b41854cb16ac0393659073eac7.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
开始
![使用DCGAN实现人脸图像生成](https://s1.51cto.com/images/blog/201901/31/da0ddd5d07ed61798a41c5034a9447b7.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2个epoch之后
![使用DCGAN实现人脸图像生成](https://s1.51cto.com/images/blog/201901/31/1be4819204450c629c9e60819421fc55.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
5个epoch之后
![使用DCGAN实现人脸图像生成](https://s1.51cto.com/images/blog/201901/31/a6f034c769228021201396a17d09f4d4.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
OpenCV+tensorflow系统化学习路线图，推荐视频教程：
[计算机视觉从入门到实战](http://edu.51cto.com/topic/1817.html)


# 机器学习中的DCGAN-Tensorflow：用于更稳定的训练 - 喜欢打酱油的老鸟 - CSDN博客


2019年03月15日 11:37:34[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：109


[https://www.toutiao.com/a6666031263536644621/](https://www.toutiao.com/a6666031263536644621/)
自从Ian Goodfellow的论文以来，GAN已应用于许多领域，但其不稳定性一直存在问题。GAN必须解决极小极大（鞍点）问题，因此这个问题是固有的。
![机器学习中的DCGAN-Tensorflow：用于更稳定的训练](http://p1.pstatp.com/large/pgc-image/4156278411f44317abe2007039c30462)
马鞍点的滑稽表示
许多研究人员试图通过各种方法解决GAN的这些困境。其中DCGAN已经取得了显著的成果。DCGAN提出了稳定的GAN网络结构。如果您根据本文的指导设计模型，可以看出模型训练是稳定的。
> 稳定的深度卷积GANs的体系结构指南

> •用 strided convolutions（判别器）和 fractional-strided convolutions （生成器）替换任何池化层 。

> • 在生成器和判别器中使用
> batchnorm
> 。

> •
> 删除完全连接的
> 隐藏层以获得更深层次的架构。

> • 除了使用
> Tanh
> 之外， 对
> 生成器
> 中的所有层使用
> ReLU激活
> 。

> • 在
> 判别器
> 中对所有层使用
> LeakyReLU激活
> 。
**生成器**
![机器学习中的DCGAN-Tensorflow：用于更稳定的训练](http://p9.pstatp.com/large/pgc-image/2dedeff5dfed4dffaa9d7a2f87a7f97c)
DCGAN生成器的结构
首先，生成器项目和重塑噪声分布（100-dim阵列）到*4x4x1024*特征映射。我们使用*matmul*和*reshape*函数来实现它。然后，我们使用一系列四个fractionally-strided函数卷积（conv2d_transpose）来逐步创建64x64图像。如上所述，batchnorm放置在每层的末尾，并且除输出之外，relu作为激活函数。我们使用tanh作为输出，所以输出的像素范围是[-1,1]。
```python
def generator(self, input, reuse = False):
 with tf.variable_scope("generator") as scope:
 if reuse:
 scope.reuse_variables()
 G_FW1 = tf.get_variable('G_FW1', [self.n_noise, self.n_hidden], initializer = tf.random_normal_initializer(stddev=0.01))
 G_Fb1 = tf.get_variable('G_Fb1', [self.n_hidden], initializer = tf.constant_initializer(0))
 G_W1 = tf.get_variable('G_W1', [5,5,self.n_W2, self.n_W1], initializer = tf.truncated_normal_initializer(stddev=0.02))
 G_W2 = tf.get_variable('G_W2', [5,5,self.n_W3, self.n_W2], initializer = tf.truncated_normal_initializer(stddev=0.02))
 G_W3 = tf.get_variable('G_W3', [5,5,self.n_W4, self.n_W3], initializer = tf.truncated_normal_initializer(stddev=0.02))
 G_W4 = tf.get_variable('G_W4', [5,5,self.image_channels, self.n_W4], initializer = tf.truncated_normal_initializer(stddev=0.02))
 hidden = tf.nn.relu(
 tf.matmul(input, G_FW1) + G_Fb1)
 hidden = tf.reshape(hidden, [self.batch_size, 4,4,self.n_W1]) 
 dconv1 = tf.nn.conv2d_transpose(hidden, G_W1, [self.batch_size, 8, 8, self.n_W2], [1, 2, 2, 1])
 dconv1 = tf.nn.relu(tf.contrib.layers.batch_norm(dconv1,decay=0.9, epsilon=1e-5))
 dconv2 = tf.nn.conv2d_transpose(dconv1, G_W2, [self.batch_size, 16, 16, self.n_W3], [1, 2, 2, 1])
 dconv2 = tf.nn.relu(tf.contrib.layers.batch_norm(dconv2,decay=0.9, epsilon=1e-5))
 dconv3 = tf.nn.conv2d_transpose(dconv2, G_W3, [self.batch_size, 32, 32, self.n_W4], [1, 2, 2, 1])
 dconv3 = tf.nn.relu(tf.contrib.layers.batch_norm(dconv3,decay=0.9, epsilon=1e-5))
 dconv4 = tf.nn.conv2d_transpose(dconv3, G_W4, [self.batch_size, 64, 64, self.image_channels], [1, 2, 2, 1])
 #dconv4 = tf.nn.relu(tf.layers.batch_normalization(dconv3, training = 'True'))
 output = tf.nn.tanh(dconv4)
 return output
```
![机器学习中的DCGAN-Tensorflow：用于更稳定的训练](http://p1.pstatp.com/large/pgc-image/1e88b6d9397147799488d2446d62d041)
**判别器**
本文中使用的判别器与生成器具有对称结构。与生成器相反，它通过减小图像大小来训练特征映像。所以我们使用了步幅大小为2的*conv2d*。与生成器相同，batchnorm放置在每层的末尾。但是leaky relu被用作激活函数。
```python
def discriminator(self, input, reuse = False):
 with tf.variable_scope("discriminator") as scope:
 if reuse:
 scope.reuse_variables()
 D_W1 = tf.get_variable('D_W1', [5,5,self.image_channels, self.n_W5], initializer = tf.truncated_normal_initializer(stddev=0.02))
 D_W2 = tf.get_variable('D_W2', [5,5,self.n_W5, self.n_W4], initializer = tf.truncated_normal_initializer(stddev=0.02))
 D_W3 = tf.get_variable('D_W3', [5,5,self.n_W4, self.n_W3], initializer = tf.truncated_normal_initializer(stddev=0.02))
 D_W4 = tf.get_variable('D_W4', [5,5,self.n_W3, self.n_W2], initializer = tf.truncated_normal_initializer(stddev=0.02)) 
 D_FW1 = tf.get_variable('D_FW1', [4*4*self.n_W2, 1], initializer = tf.random_normal_initializer(stddev=0.01))
 D_Fb1 = tf.get_variable('D_Fb1', [1], initializer = tf.constant_initializer(0))
 conv1 = tf.nn.conv2d(input, D_W1, strides = [1, 2, 2, 1], padding='SAME')
 conv1 = tf.nn.leaky_relu(conv1, alpha = 0.2)
 conv2 = tf.nn.conv2d(conv1, D_W2, strides = [1, 2, 2, 1], padding='SAME')
 conv2 = tf.nn.leaky_relu(tf.contrib.layers.batch_norm(conv2, decay=0.9, epsilon=1e-5), alpha = 0.2)
 conv3 = tf.nn.conv2d(conv2, D_W3, strides = [1, 2, 2, 1], padding='SAME')
 conv3 = tf.nn.leaky_relu(tf.contrib.layers.batch_norm(conv3, decay=0.9, epsilon=1e-5), alpha = 0.2)
 conv4 = tf.nn.conv2d(conv3, D_W4, strides = [1, 2, 2, 1], padding='SAME')
 conv4 = tf.nn.leaky_relu(tf.contrib.layers.batch_norm(conv4, decay=0.9, epsilon=1e-5), alpha = 0.2)
 hidden = tf.reshape(conv4, [self.batch_size, 4*4*self.n_W2]) 
 output = tf.nn.sigmoid(
 tf.matmul(hidden, D_FW1) + D_Fb1)
 return output
```
![机器学习中的DCGAN-Tensorflow：用于更稳定的训练](http://p1.pstatp.com/large/pgc-image/4acb1d3928464c13a151c7a6f99e771f)
**损失函数和优化器**
损失函数和优化器与基本GAN相同。然而，根据该DCGAN论文的（对抗性训练的细节）中，我们设置*优化器的学习率*是*0.0002*和*bata1为0.5*。
```python
def loss(self, X, Z):
 g_out = self.generator(Z)
 d_fake = self.discriminator(g_out, reuse = False)
 d_real = self.discriminator(X, reuse = True)
 d_loss = tf.reduce_mean(tf.log(d_real) + tf.log(1 - d_fake))
 g_loss = tf.reduce_mean(tf.log(d_fake))
 return d_loss, g_loss
 
 def optimizer(self, d_loss, g_loss, learning_rate):
 d_var_list = tf.get_collection(tf.GraphKeys.GLOBAL_VARIABLES, scope='discriminator')
 g_var_list = tf.get_collection(tf.GraphKeys.GLOBAL_VARIABLES, scope='generator')
 #print('G_var_list:', len(G_var_list))
 #print('D_var_list:', len(D_var_list))
 d_opt = tf.train.AdamOptimizer(learning_rate, beta1 = 0.5).minimize(-d_loss,
 var_list=d_var_list)
 g_opt = tf.train.AdamOptimizer(learning_rate, beta1 = 0.5).minimize(-g_loss,
 var_list=g_var_list)
 return d_opt, g_opt
```
![机器学习中的DCGAN-Tensorflow：用于更稳定的训练](http://p9.pstatp.com/large/pgc-image/91987dc1983b405c9eb5e21a2cbe5963)
**结果**
数据集是从kaggle下载的。数据集下载地址（https://www.kaggle.com/scolianni/mnistasjpg）
![机器学习中的DCGAN-Tensorflow：用于更稳定的训练](http://p3.pstatp.com/large/pgc-image/7fb81c1a8cc44185b3d1f16c92ae5790)
DCGAN
![机器学习中的DCGAN-Tensorflow：用于更稳定的训练](http://p3.pstatp.com/large/pgc-image/3369bdd4cefc433299f8ae34301eec62)
BasicGAN
使用mnist数据集训练了DCGAN和basicGAN模型。DCGAN生成比basicGAN更干净的图像。当然，上述结果似乎是合理的，因为DCGAN具有比BasicGAN更复杂的结构并且具有更多参数。但是DCGAN中引入的方法甚至可以稳定地学习复杂的模型。这是DCGAN的贡献。
此外，我还使用Celeb_A数据集训练了两个模型，其中有大约200k肖像照片。数据集下载地址（https://www.kaggle.com/jessicali9530/celeba-dataset）
![机器学习中的DCGAN-Tensorflow：用于更稳定的训练](http://p3.pstatp.com/large/pgc-image/45fccc1196de4ab59689a5ba455ca586)
DCGAN
![机器学习中的DCGAN-Tensorflow：用于更稳定的训练](http://p1.pstatp.com/large/pgc-image/3adab16875d2428a8fd637fa8af39697)
BasicGAN
我们为DCGAN和BasicGAN模型训练了9个周期。DCGAN不仅可以产生清晰的图像，还可以表现出各种特征，如眼镜，妆容和胡须。但它仍然看起来不自然。
这就是利用深度学习来实现人脸的创造。其他深度学习模型（如**变分自编码器**和**自回归模型）**也是生成模型的好示例。


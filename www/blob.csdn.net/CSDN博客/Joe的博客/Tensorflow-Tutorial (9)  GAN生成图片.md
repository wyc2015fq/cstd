# Tensorflow - Tutorial (9) : GAN生成图片 - Joe的博客 - CSDN博客





2018年01月08日 16:35:29[Joe-Han](https://me.csdn.net/u010089444)阅读数：2606








# **1. 介绍 **

本文利用Tensorflow实现生成式对抗网络GAN，关于GAN的详细介绍可参考：[生成式对抗网络（Generative Adversarial Networks，GANs）](http://blog.csdn.net/u010089444/article/details/78946039)。训练所使用的MNIST数据集包含了各种手写数字图片，图片的格式与数据预处理代码input_data.py的介绍详见：[Tutorial (2)](http://blog.csdn.net/u010089444/article/details/52495254)。本文在GAN中使用的生成模型$G$和判别模型$D$均为多层感知机。生成模型的定义如下：输入是一个200维的噪声，隐含层的维度是600，输出是一个28*28维的图片，模型中的参数用字典存储。

```python
# 生成模型G
z_size = 200
g_w1_size = 600
g_out_size = 28 * 28

# 定义网络G的参数
g_weights = {
    'w1': tf.Variable(xavier_initializer(shape=(z_size, g_w1_size))),
    'b1': tf.Variable(tf.zeros(shape=[g_w1_size])),
    'out': tf.Variable(xavier_initializer(shape=(g_w1_size, g_out_size))),
    'b2': tf.Variable(tf.zeros(shape=[g_out_size])),
}

def G(z, w=g_weights):
    h1 = tf.tanh(tf.matmul(z, w['w1']) + w['b1']) #隐含层向量
    return tf.sigmoid(tf.matmul(h1, w['out']) + w['b2'])
```

判别模型的定义如下，输入为一张28*28=784维的图片，隐含层的维度是600，模型的输出为一个实数值，并通过sigmoid函数映射到$[0, 1]$区间（sigmoid函数的映射在sigmoid_cross_entropy_with_logits中实现）。

```python
# 判别模型D
x_size = 28 * 28
d_w1_size = 600
d_out_size = 1

# 定义网络D的参数
d_weights ={
    'w1': tf.Variable(xavier_initializer(shape=(x_size, d_w1_size))),
    'b1': tf.Variable(tf.zeros(shape=[d_w1_size])),
    'out': tf.Variable(xavier_initializer(shape=(d_w1_size, d_out_size))),
    'b2': tf.Variable(tf.zeros(shape=[d_out_size])),
}

def D(x, w=d_weights):
    h1 = tf.tanh(tf.matmul(x, w['w1']) + w['b1']) #隐含层向量 
    return tf.matmul(h1, w['out']) + w['b2']
```

判别模型对两类图片的输出如下：

```python
dout_real = D(X) # 判别模型对"真"图片的输出
dout_fake = D(G(z)) # 判别模型对"假"图片的输出
```

生成模型的目标函数，即最小化-$logD(G(z))$

```python
# 生成模型的目标函数
G_obj = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=dout_fake, labels=tf.ones_like(dout_fake)))
```

判别模型的目标函数，第1行为$-logD(x)$，第2行为$-log[1-D(G(z))]$，需最小化D_obj

```python
D_obj_real = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=dout_real, labels=(tf.ones_like(dout_real)))) 
D_obj_fake = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=dout_fake, labels=tf.zeros_like(dout_fake))) 
D_obj = D_obj_real + D_obj_fake # 判别模型的目标函数
```

# **2. 代码**

完整代码如下：

```python
# -*- coding: utf-8 -*-
import tensorflow as tf
import numpy as np
import input_data
from PIL import Image

#读取数据
mnist = input_data.read_data_sets("MNIST_data/") 
images = mnist.train.images

def xavier_initializer(shape):
    return tf.random_normal(shape=shape, stddev=1.0/shape[0])

# 生成模型G
z_size = 200
g_w1_size = 600
g_out_size = 28 * 28

# 判别模型D
x_size = 28 * 28
d_w1_size = 600
d_out_size = 1

z = tf.placeholder('float', shape=(None, z_size))
X = tf.placeholder('float', shape=(None, x_size))

# 定义网络G的参数
g_weights = {
    'w1': tf.Variable(xavier_initializer(shape=(z_size, g_w1_size))),
    'b1': tf.Variable(tf.zeros(shape=[g_w1_size])),
    'out': tf.Variable(xavier_initializer(shape=(g_w1_size, g_out_size))),
    'b2': tf.Variable(tf.zeros(shape=[g_out_size])),
}

# 定义网络D的参数
d_weights ={
    'w1': tf.Variable(xavier_initializer(shape=(x_size, d_w1_size))),
    'b1': tf.Variable(tf.zeros(shape=[d_w1_size])),
    'out': tf.Variable(xavier_initializer(shape=(d_w1_size, d_out_size))),
    'b2': tf.Variable(tf.zeros(shape=[d_out_size])),
}

def G(z, w=g_weights):
    h1 = tf.tanh(tf.matmul(z, w['w1']) + w['b1']) # 隐含层向量
    return tf.sigmoid(tf.matmul(h1, w['out']) + w['b2'])

def D(x, w=d_weights):
    h1 = tf.tanh(tf.matmul(x, w['w1']) + w['b1']) #隐含层向量 
    return tf.matmul(h1, w['out']) + w['b2']

# 生成服从均匀分布的噪声
def generate_z(n=1):
    return np.random.rand(n,z_size)*10

dout_real = D(X) # 判别模型对"真"图片的输出
dout_fake = D(G(z)) # 判别模型对"假"图片的输出

# 生成模型的目标函数
G_obj = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=dout_fake, labels=tf.ones_like(dout_fake)))
D_obj_real = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=dout_real, labels=(tf.ones_like(dout_real)))) 
D_obj_fake = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=dout_fake, labels=tf.zeros_like(dout_fake))) 
D_obj = D_obj_real + D_obj_fake # 判别模型的目标函数

G_opt = tf.train.AdamOptimizer().minimize(G_obj, var_list=g_weights.values())
D_opt = tf.train.AdamOptimizer().minimize(D_obj, var_list=d_weights.values())

# 使用mini-batch进行训练
batch_size = 128
with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    for i in range(50):
        # 训练判别模型D
        sess.run(D_opt, feed_dict={
            X: images[np.random.choice(range(len(images)), batch_size)].reshape(batch_size, x_size),
            z: generate_z(batch_size),
        })
        # 训练生成模型G
        sess.run(G_opt, feed_dict={
            z: generate_z(batch_size)
        })

        g_cost = sess.run(G_obj, feed_dict={z: generate_z(batch_size)})
        d_cost = sess.run(D_obj, feed_dict={
            X: images[np.random.choice(range(len(images)), batch_size)].reshape(batch_size, x_size),
            z: generate_z(batch_size),
        })
        image = sess.run(G(z), feed_dict={z:generate_z()})[0].reshape([28, 28]) * 255 # 像素的范围为[0, 255]
        Image.fromarray(image).convert('RGB').save('./'+str(i)+'.jpg') # 在每轮训练结束后，保存G生成的图片
        df = sess.run(tf.sigmoid(dout_fake), feed_dict={z:generate_z()})    
        # 迭代次数，判别模型的cost，生成模型的cost，max G(z)，D(G(z))
        print (i, g_cost, d_cost, image.max(), df[0][0]) 
        # 这里设定了训练停止条件，即D(G(z)接近0.5时训练停止
        if df[0][0] >0.45 and df[0][0] <0.55 and i >8:
            break
    # 显示图片
    image = sess.run(G(z), feed_dict={z:generate_z()})
    image1 = image[0].reshape([28, 28]) * 255
    im = Image.fromarray(image1)
    im.show()
```

# **3. 运行结果**

代码运行结果如下，总共训练15轮： 

![](https://img-blog.csdn.net/20180108162754719?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

每轮训练结束后，生成模型$G$所产生的图片如下： 
![图片名称](https://img-blog.csdn.net/20180108163024231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





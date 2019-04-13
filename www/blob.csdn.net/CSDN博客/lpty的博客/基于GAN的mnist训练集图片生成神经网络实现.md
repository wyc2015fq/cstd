
# 基于GAN的mnist训练集图片生成神经网络实现 - lpty的博客 - CSDN博客

2017年08月31日 19:15:53[lpty](https://me.csdn.net/sinat_33741547)阅读数：5224



### 一、前言
### 1、什么是GAN？
GAN主要包括了两个部分，即生成器generator与判别器discriminator。
生成器主要用来学习真实图像分布从而让自身生成的图像更加真实，以骗过判别器。判别器则需要对接收的图片进行真假判别。
在整个过程中，生成器努力地让生成的图像更加真实，而判别器则努力地去识别出图像的真假，这个过程相当于一个二人博弈，随着时间的推移，生成器和判别器在不断地进行对抗，最终两个网络达到了一个动态均衡：生成器生成的图像接近于真实图像分布，而判别器识别不出真假图像，对于给定图像的预测为真的概率基本接近0.5（相当于随机猜测类别）

### 二、实战
**1、参数定义**

```python
class MnistModel:
    def __init__(self):
        # mnist测试集
        self.mnist = input_data.read_data_sets('mnist/', one_hot=True)
        # 图片大小
        self.img_size = self.mnist.train.images[0].shape[0]
        # 每步训练使用图片数量
        self.batch_size = 64
        # 图片分块数量
        self.chunk_size = self.mnist.train.num_examples // self.batch_size
        # 训练循环次数
        self.epoch_size = 300
        # 抽取样本数
        self.sample_size = 25
        # 生成器判别器隐含层数量
        self.units_size = 128
        # 学习率
        self.learning_rate = 0.001
        # 平滑参数
        self.smooth = 0.1
```
采用mnist训练集作为训练数据
**2、真实图片与混淆图片**

```python
# 真实图片与混淆图片
        # 不确定输入图片数量 用None
        real_imgs = tf.placeholder(tf.float32, [None, self.img_size], name='real_imgs')
        fake_imgs = tf.placeholder(tf.float32, [None, self.img_size], name='fake_imgs')
```
真实图片为mnist的训练图片，这里的size应该是784,。fake_img接受噪声输入经由生成器转化后，传入判别器判断。
**3、生成器**

```python
def generator_graph(fake_imgs, units_size, out_size, alpha=0.01):
        # 生成器与判别器属于两个网络 定义不同scope
        with tf.variable_scope('generator'):
            # 构建一个全连接层
            layer = tf.layers.dense(fake_imgs, units_size)
            # leaky ReLU 激活函数
            relu = tf.maximum(alpha * layer, layer)
            # dropout 防止过拟合
            drop = tf.layers.dropout(relu, rate=0.2)
            # logits
            # out_size应为真实图片size大小
            logits = tf.layers.dense(drop, out_size)
            # 激活函数 将向量值限定在某个区间 与 真实图片向量类似
            # 这里tanh的效果比sigmoid好一些
            # 输出范围(-1, 1) 采用sigmoid则为[0, 1]
            outputs = tf.tanh(logits)
            return logits, outputs
```
**4、判别器**

```python
def discriminator_graph(imgs, units_size, alpha=0.01, reuse=False):
        with tf.variable_scope('discriminator', reuse=reuse):
            # 构建全连接层
            layer = tf.layers.dense(imgs, units_size)
            # leaky ReLU 激活函数
            relu = tf.maximum(alpha * layer, layer)
            # logits
            # 判断图片真假 out_size直接限定为1
            logits = tf.layers.dense(relu, 1)
            # 激活函数
            outputs = tf.sigmoid(logits)
            return logits, outputs
```
**5、损失**

```python
def loss_graph(real_logits, fake_logits, smooth):
        # 生成器图片loss
        # 生成器希望判别器判断出来的标签为1
        gen_loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=fake_logits, labels=tf.ones_like(fake_logits) * (1 - smooth)))
        # 判别器识别生成器图片loss
        # 判别器希望识别出来的标签为0
        fake_loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=fake_logits, labels=tf.zeros_like(fake_logits)))
        # 判别器识别真实图片loss
        # 判别器希望识别出来的标签为1
        real_loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=real_logits, labels=tf.ones_like(real_logits) * (1 - smooth)))
        # 判别器总loss
        dis_loss = tf.add(fake_loss, real_loss)
        return gen_loss, fake_loss, real_loss, dis_loss
```
**6、优化**
```python
def optimizer_graph(gen_loss, dis_loss, learning_rate):
        # 所有定义变量
        train_vars = tf.trainable_variables()
        # 生成器变量
        gen_vars = [var for var in train_vars if var.name.startswith('generator')]
        # 判别器变量
        dis_vars = [var for var in train_vars if var.name.startswith('discriminator')]
        # optimizer
        # 生成器与判别器作为两个网络需要分别优化
        gen_optimizer = tf.train.AdamOptimizer(learning_rate).minimize(gen_loss, var_list=gen_vars)
        dis_optimizer = tf.train.AdamOptimizer(learning_rate).minimize(dis_loss, var_list=dis_vars)
        return gen_optimizer, dis_optimizer
```
**7、开始训练**

```python
# 开始训练
        saver = tf.train.Saver()
        step = 0
        # 指定占用GPU比例
        # tensorflow默认占用全部GPU显存 防止在机器显存被其他程序占用过多时可能在启动时报错
        gpu_options = tf.GPUOptions(per_process_gpu_memory_fraction=0.8)
        with tf.Session(config=tf.ConfigProto(gpu_options=gpu_options)) as sess:
            sess.run(tf.global_variables_initializer())
            for epoch in range(self.epoch_size):
                for _ in range(self.chunk_size):
                    batch_imgs, _ = self.mnist.train.next_batch(self.batch_size)
                    batch_imgs = batch_imgs * 2 - 1
                    # generator的输入噪声
                    noise_imgs = np.random.uniform(-1, 1, size=(self.batch_size, self.img_size))
                    # 优化
                    _ = sess.run(gen_optimizer, feed_dict={fake_imgs: noise_imgs})
                    _ = sess.run(dis_optimizer, feed_dict={real_imgs: batch_imgs, fake_imgs: noise_imgs})
                    step += 1
```
**8、结果**
![](https://img-blog.csdn.net/20170831191115272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
到这里可以看到，我们随机输入的噪声向量经生成器转化后，已经和原本的训练集图片有一定的相似之处了。
## 三、其他
1、完整代码可以在我的github上找到：[https://github.com/lpty/tensorflow_tutorial](https://github.com/lpty/tensorflow_tutorial)
2、参考文章：[https://zhuanlan.zhihu.com/p/28057434](https://zhuanlan.zhihu.com/p/28057434)




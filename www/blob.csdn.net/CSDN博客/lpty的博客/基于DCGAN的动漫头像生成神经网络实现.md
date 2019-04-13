
# 基于DCGAN的动漫头像生成神经网络实现 - lpty的博客 - CSDN博客

2017年09月06日 21:08:39[lpty](https://me.csdn.net/sinat_33741547)阅读数：7890标签：[GAN																](https://so.csdn.net/so/search/s.do?q=GAN&t=blog)[cnn																](https://so.csdn.net/so/search/s.do?q=cnn&t=blog)[DCGAN																](https://so.csdn.net/so/search/s.do?q=DCGAN&t=blog)[神经网络																](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)[tenforflow																](https://so.csdn.net/so/search/s.do?q=tenforflow&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=DCGAN&t=blog)个人分类：[图像处理																](https://blog.csdn.net/sinat_33741547/article/category/7312908)[深度学习																](https://blog.csdn.net/sinat_33741547/article/category/7028998)[
							](https://blog.csdn.net/sinat_33741547/article/category/7312908)
[
				](https://so.csdn.net/so/search/s.do?q=DCGAN&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=DCGAN&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=cnn&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=GAN&t=blog)

## 一、前言
1、[什么是DCGAN？](https://zhuanlan.zhihu.com/p/24767059)
2、[DCGAN的TensorFlow实现](https://github.com/carpedm20/DCGAN-tensorflow)
3、[什么是转置卷积？](https://www.zhihu.com/question/43609045?sort=created)
4、[转置卷积的Tensorflow实现](http://blog.csdn.net/mao_xiao_feng/article/details/71713358)
5、[Batch Normalization解读](http://blog.csdn.net/elaine_bao/article/details/50890491)
本文假设读者已经了解GAN及CNN的基本原理实现，如不清楚可参考以下文章：
[基于GAN的的mnist训练集图片生成神经网络实现](http://blog.csdn.net/sinat_33741547/article/details/77751035)
[基于CNN的验证码识别神经网络实现](http://blog.csdn.net/sinat_33741547/article/details/77510656)
## 二、实战
**1、训练数据处理**
(1)数据源：[百度云盘](https://pan.baidu.com/s/1eSifHcA)提取码：g5qa
(2)创建一个生成器

```python
class Avatar:
    def __init__(self):
        self.data_name = 'faces'
        self.source_shape = (96, 96, 3)
        self.resize_shape = (48, 48, 3)
        self.crop = True
        self.img_shape = self.source_shape if not self.crop else self.resize_shape
        self.img_list = self._get_img_list()
        self.batch_size = 64
        self.batch_shape = (self.batch_size, ) + self.img_shape
        self.chunk_size = len(self.img_list) // self.batch_size
    def _get_img_list(self):
        path = os.path.join(os.getcwd(), self.data_name, '*.jpg')
        return glob(path)
    def _get_img(self, name):
        assert name in self.img_list
        img = scipy.misc.imread(name).astype(np.float32)
        assert img.shape == self.source_shape
        return self._resize(img) if self.crop else img
    def _resize(self, img):
        h, w = img.shape[:2]
        resize_h, resize_w = self.resize_shape[:2]
        crop_h, crop_w = self.source_shape[:2]
        j = int(round((h - crop_h) / 2.))
        i = int(round((w - crop_w) / 2.))
        cropped_image = scipy.misc.imresize(img[j:j + crop_h, i:i + crop_w], [resize_h, resize_w])
        return np.array(cropped_image) / 127.5 - 1.
    @staticmethod
    def save_img(image, path):
        scipy.misc.imsave(path, image)
        return True
    def batches(self):
        start = 0
        end = self.batch_size
        for _ in range(self.chunk_size):
            name_list = self.img_list[start:end]
            imgs = [self._get_img(name) for name in name_list]
            batches = np.zeros(self.batch_shape)
            batches[::] = imgs
            yield batches
            start += self.batch_size
            end += self.batch_size
```
读取本地图片数据并创建一个生成器，作为后续模型数据源
**2.模型参数定义**

```python
def __init__(self):
        self.avatar = Avatar()
        # 真实图片shape (height, width, depth)
        self.img_shape = self.avatar.img_shape
        # 一个batch的图片向量shape (batch, height, width, depth)
        self.batch_shape = self.avatar.batch_shape
        # 一个batch包含图片数量
        self.batch_size = self.avatar.batch_size
        # batch数量
        self.chunk_size = self.avatar.chunk_size
        # 噪音图片size
        self.noise_img_size = 100
        # 卷积转置输出通道数量
        self.gf_size = 64
        # 卷积输出通道数量
        self.df_size = 64
        # 训练循环次数
        self.epoch_size = 50
        # 学习率
        self.learning_rate = 0.0002
        # 优化指数衰减率
        self.beta1 = 0.5
        # 生成图片数量
        self.sample_size = 64
```
**3、输入定义**

```python
# 真实图片
        real_imgs = tf.placeholder(tf.float32, self.batch_shape, name='real_images')
        # 噪声图片
        noise_imgs = tf.placeholder(tf.float32, [None, self.noise_img_size], name='noise_images')
```
我们利用随机的噪音输入来生成图片
**4、生成器**

```python
def generator(self, noise_imgs, train=True):
        with tf.variable_scope('generator'):
            # 分别对应每个layer的height, width
            s_h, s_w, _ = self.img_shape
            s_h2, s_w2 = self.conv_out_size_same(s_h, 2), self.conv_out_size_same(s_w, 2)
            s_h4, s_w4 = self.conv_out_size_same(s_h2, 2), self.conv_out_size_same(s_w2, 2)
            s_h8, s_w8 = self.conv_out_size_same(s_h4, 2), self.conv_out_size_same(s_w4, 2)
            s_h16, s_w16 = self.conv_out_size_same(s_h8, 2), self.conv_out_size_same(s_w8, 2)
            # layer 0
            # 对输入噪音图片进行线性变换
            z, h0_w, h0_b = self.linear(noise_imgs, self.gf_size*8*s_h16*s_w16)
            # reshape为合适的输入层格式
            h0 = tf.reshape(z, [-1, s_h16, s_w16, self.gf_size * 8])
            # 对数据进行归一化处理 加快收敛速度
            h0 = self.batch_normalizer(h0, train=train, name='g_bn0')
            # 激活函数
            h0 = tf.nn.relu(h0)
            # layer 1
            # 卷积转置进行上采样
            h1, h1_w, h1_b = self.deconv2d(h0, [self.batch_size, s_h8, s_w8, self.gf_size*4], name='g_h1')
            h1 = self.batch_normalizer(h1, train=train, name='g_bn1')
            h1 = tf.nn.relu(h1)
            # layer 2
            h2, h2_w, h2_b = self.deconv2d(h1, [self.batch_size, s_h4, s_w4, self.gf_size*2], name='g_h2')
            h2 = self.batch_normalizer(h2, train=train, name='g_bn2')
            h2 = tf.nn.relu(h2)
            # layer 3
            h3, h3_w, h3_b = self.deconv2d(h2, [self.batch_size, s_h2, s_w2, self.gf_size*1], name='g_h3')
            h3 = self.batch_normalizer(h3, train=train, name='g_bn3')
            h3 = tf.nn.relu(h3)
            # layer 4
            h4, h4_w, h4_b = self.deconv2d(h3, self.batch_shape, name='g_h4')
            return tf.nn.tanh(h4)
```
DCGAN的生成器为卷积网络，使用转置卷积进行上采样，去除pooling层，利用batch normalization加快收敛速度。
**5、判别器**

```python
def discriminator(self, real_imgs, reuse=False):
        with tf.variable_scope("discriminator", reuse=reuse):
            # layer 0
            # 卷积操作
            h0 = self.conv2d(real_imgs, self.df_size, name='d_h0_conv')
            # 激活函数
            h0 = self.lrelu(h0)
            # layer 1
            h1 = self.conv2d(h0, self.df_size*2, name='d_h1_conv')
            h1 = self.batch_normalizer(h1, name='d_bn1')
            h1 = self.lrelu(h1)
            # layer 2
            h2 = self.conv2d(h1, self.df_size*4, name='d_h2_conv')
            h2 = self.batch_normalizer(h2, name='d_bn2')
            h2 = self.lrelu(h2)
            # layer 3
            h3 = self.conv2d(h2, self.df_size*8, name='d_h3_conv')
            h3 = self.batch_normalizer(h3, name='d_bn3')
            h3 = self.lrelu(h3)
            # layer 4
            h4, _, _ = self.linear(tf.reshape(h3, [self.batch_size, -1]), 1, name='d_h4_lin')
            return tf.nn.sigmoid(h4), h4
```
DCGAN的判别器为卷积网络，这里使用卷积操作对图像进行特征提取识别。
**6、损失和优化**

```python
@staticmethod
    def loss_graph(real_logits, fake_logits):
        # 生成器图片loss
        # 生成器希望判别器判断出来的标签为1
        gen_loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=fake_logits, labels=tf.ones_like(fake_logits)))
        # 判别器识别生成器图片loss
        # 判别器希望识别出来的标签为0
        fake_loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=fake_logits, labels=tf.zeros_like(fake_logits)))
        # 判别器识别真实图片loss
        # 判别器希望识别出来的标签为1
        real_loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=real_logits, labels=tf.ones_like(real_logits)))
        # 判别器总loss
        dis_loss = tf.add(fake_loss, real_loss)
        return gen_loss, fake_loss, real_loss, dis_loss
    @staticmethod
    def optimizer_graph(gen_loss, dis_loss, learning_rate, beta1):
        # 所有定义变量
        train_vars = tf.trainable_variables()
        # 生成器变量
        gen_vars = [var for var in train_vars if var.name.startswith('generator')]
        # 判别器变量
        dis_vars = [var for var in train_vars if var.name.startswith('discriminator')]
        # optimizer
        # 生成器与判别器作为两个网络需要分别优化
        gen_optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate, beta1=beta1).minimize(gen_loss, var_list=gen_vars)
        dis_optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate, beta1=beta1).minimize(dis_loss, var_list=dis_vars)
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
                batches = self.avatar.batches()
                for batch_imgs in batches:
                    # generator的输入噪声
                    noises = np.random.uniform(-1, 1, size=(self.batch_size, self.noise_img_size)).astype(np.float32)
                    # 优化
                    _ = sess.run(dis_optimizer, feed_dict={real_imgs: batch_imgs, noise_imgs: noises})
                    _ = sess.run(gen_optimizer, feed_dict={noise_imgs: noises})
                    _ = sess.run(gen_optimizer, feed_dict={noise_imgs: noises})
                    step += 1
                    print(datetime.now().strftime('%c'), epoch, step)
```
**8、结果**
![](https://img-blog.csdn.net/20170907004212703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
跑了50个循环大概用了5个小时，笔者GPU比较一般，就不继续训练了。可以看到，到这里已经生成了不错的效果。
## 三、其他
具体代码可以在我的github上找到：[https://github.com/lpty/tensorflow_tutorial](https://github.com/lpty/tensorflow_tutorial)








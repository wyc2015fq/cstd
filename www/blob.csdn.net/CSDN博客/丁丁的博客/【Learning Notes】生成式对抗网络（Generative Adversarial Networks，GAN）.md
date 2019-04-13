
# 【Learning Notes】生成式对抗网络（Generative Adversarial Networks，GAN） - 丁丁的博客 - CSDN博客


2017年03月14日 10:18:13[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：7341


在学习VariationalAuto-Encoder时，同时注意到了GAN研究的火热。但当时觉得GAN非常不成熟（训练不稳定，依赖各种说不清的tricks；没有有效的监控指标，需要大量的人工判断，因此难以扩展到图像之外的高维数据）。在读了Goodfellow的tutorial后[2]，开始黑转路人，觉得GAN虽然缺点不少，但优点也很明显。WGAN[5,6]等工作出现后，开始逐渐路人转粉，对GAN产生了兴趣。
这里，我们仅仅从直观上讨论GAN框架及相关变种，将理论留待将来讨论。
# 1. Basic GAN
本质上，GAN是一种训练模式，而非一种待定的网络结构[1]。
![](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)[ ](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)
**图****1****.****G****A****N****基****本****框****架****【****s****r****c****】**
[
](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)GAN的基本思想是，生成器和判别器玩一场“道高一尺，魔高一丈”的游戏：判别器要练就“火眼金睛”，尽量区分出真实的样本（如真实的图片）和由生成器生成的假样本；生成器要学着“以假乱真”，生成出使判别器判别为真实的“假样本”。
[
](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)竞争的理想怦是双方都不断进步——（理想情况下）判别器的眼睛越发“雪亮”，生成器的欺骗能力也不断提高。**对****抗****的****胜****负****无****关****紧****要****，****重****要****的****是****，****最****后****生****成****器****的****欺****骗****能****力****足****够****好****，****能****够****生****成****与****真****实****样****本****足****够****相****似****的****样****本**——直观而言，生成的样本看起来像是训练集（如图片）的样本；形式化的，生成器生成样本的分布，应该与训练集样本分布接近。
[
](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)理论上可以，在理想条件下，生成器是可以通过这种对抗得到目标分布的（即生成足够真实的样本）。
[
](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)假设要训练数据为灰度MNIST（归一化为[0,1]之间），生成器（generator）可以为任意输入为隐变量维度，输出为1x28x28的模型。一个示例模型定义如下：
[

](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)
```python
def
```
```python
build_generator
```
```python
(latent_size)
```
```python
:
```
```python
model = Sequential()
    model.add(Dense(
```
```python
1024
```
```python
, input_dim=latent_size, activation=
```
```python
'relu'
```
```python
))
    model.add(Dense(
```
```python
28
```
```python
*
```
```python
28
```
```python
, activation=
```
```python
'tanh'
```
```python
))
    model.add(Reshape((
```
```python
1
```
```python
,
```
```python
28
```
```python
,
```
```python
28
```
```python
)))
```
```python
return
```
```python
model
```
[
](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)判别器（discriminator）可以为任意输入1x28x28，输出为1维且在[0,1]之间（经过sigmoid激活）的模型。一个示例模型定义如下：
[

](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)
```python
def
```
```python
build_discriminator
```
```python
()
```
```python
:
```
```python
model = Sequential()
    model.add(Flatten(input_shape=(
```
```python
1
```
```python
,
```
```python
28
```
```python
,
```
```python
28
```
```python
)))
    model.add(Dense(
```
```python
256
```
```python
, activation=
```
```python
'relu'
```
```python
))
    model.add(Dense(
```
```python
128
```
```python
, activation=
```
```python
'relu'
```
```python
))
    model.add(Dense(
```
```python
1
```
```python
)， activation=
```
```python
'sigmoid'
```
```python
)
```
```python
return
```
```python
model
```
[
](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)输出值表示判别器判别输入样本为真的概率。即输出值越接近1，判别器越确信样本为真；输出值越接近0，判别器越确信样本为假。
[

](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)
## 判别器
[
](https://blog.slinuxer.com/wp-content/uploads/2016/10/gan.jpg)
$$
L_D = -\Sigma_i \log(D(\textbf{x}_i)) -\Sigma_i \log(1-D(G(\textbf{z}_i)))
$$
判别器的训练的目标为：对于真实样本，输出尽量接近1；对于生成器生成的假样本，输出尽量接近0。
也即训练判别器时，真实样本的标签为1，生成样本的标签为0。
## 生成器

$$
L_G = \Sigma_i \log(1-D(G(\textbf{z}_i)))
$$
判别器的训练的目标为生成的假样本，使判别器的输出尽量接近1，即尽量以假乱真。
为了解决训练过程中，梯度消失的问题，一般使用如下损失函数(Trick2)：

$$
L_G = -\Sigma_i \log(D(G(\textbf{z}_i)))
$$
为使用这个损失函数，只需要将生成样本的标签为1，同时使用变通的交叉熵损失函数。
GAN的训练流程如下[1]：
![这里写图片描述](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

$$
\nabla\theta\frac{1}{m}\Sigma_{i=1}^{m}-\log(D(G(\textbf{z}^{(i)})))
$$
[
](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)GAN足够简单，也有理论上的保证。但在实践中，需要许多技巧和运气才能正常把“游戏玩下去”。这里，我们不考虑理论，而是关注不要GAN变种在损失函数设计的差异。
[

](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 2. Least Squares GAN
[
](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)我们以[4]中Eq(9)为例来介绍LSGAN。其中判别器的定义如下：
[

](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
def
```
```python
build_discriminator
```
```python
()
```
```python
:
```
```python
model = Sequential()
    model.add(Flatten(input_shape=(
```
```python
1
```
```python
,
```
```python
28
```
```python
,
```
```python
28
```
```python
)))
    model.add(Dense(
```
```python
256
```
```python
, activation=
```
```python
'relu'
```
```python
))
    model.add(Dense(
```
```python
128
```
```python
, activation=
```
```python
'relu'
```
```python
))
    model.add(Dense(
```
```python
1
```
```python
)， activation=
```
```python
'linear'
```
```python
)
```
```python
## change 1
```
```python
return
```
```python
model
```
[
](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)与basicGAN唯一不同在判别器的最后输出不使用sigmoid激活，而是使用了线性函数（也即不使用激活）（第6行change1）。
[
](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)有了生成器和判别器的定义，我们来实际构造两者以用于训练：
[

](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
# 构造判别器
```
```python
disc = build_discriminator()
disc.compile(optimizer=Adam(lr=lr),loss=
```
```python
'mse'
```
```python
)
```
```python
# 构建生成器
```
```python
generator = build_generator(latent_size)
latent = Input(shape=(latent_size,))
```
```python
# 生成假图片
```
```python
fake = generator(latent)
```
```python
# 我们要训练生成器，因此固定判别的权值不变
```
```python
disc.trainable =
```
```python
False
```
```python
fake = disc(fake)
combined = Model(input=latent, output=fake)
combined.compile(optimizer=Adam(lr=lr), loss=
```
```python
'mse'
```
```python
)
```
[
](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)不同于basicGAN,LSGAN的训练损失函数由交叉熵改为MSE(MeanSquaredError)。
[

](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
for
```
```python
epoch
```
```python
in
```
```python
range(nb_epochs):
```
```python
for
```
```python
index
```
```python
in
```
```python
range(nb_batches):
```
```python
## 1) 训练判别器
```
```python
# 1.1采样隐变量并生成假样本
```
```python
noise = np.random.uniform(-
```
```python
1
```
```python
,
```
```python
1
```
```python
, (batch_size, latent_size))
        generated_images = generator.predict(noise, verbose=
```
```python
0
```
```python
)
```
```python
# 1.2 从训练中采样真实样本
```
```python
image_batch = X_train[index * batch_size:(index +
```
```python
1
```
```python
) * batch_size]
        label_batch = y_train[index * batch_size:(index +
```
```python
1
```
```python
) * batch_size]
```
```python
# 利用真假数据进行训练
```
```python
X = np.concatenate((image_batch, generated_images))
```
```python
# 设定真假数据的损失，a == 0, b == 1
```
```python
y = np.array([
```
```python
1
```
```python
] * len(image_batch) + [
```
```python
0
```
```python
] * batch_size)
        disc.train_on_batch(X, y)
```
```python
## 2）训练生成器
```
```python
# 采样隐变量
```
```python
noise = np.random.uniform(-
```
```python
1
```
```python
,
```
```python
1
```
```python
, (batch_size, latent_size))
        target = np.ones(batch_size)
```
```python
# 设定生成样本的损失 c == b == 1
```
```python
combined.train_on_batch(noise, target)
```
[
](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)图2是训练过程中，由生成器采样的几张示例图片。完整的示例可以参见repo。
[
](https://img-blog.csdn.net/20170313154339495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![LS-GAN](https://img-blog.csdn.net/20170314081856037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170314081856037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图****2****.****L****S****G****A****N****随****机****采****样****生****成****的****图****片****（****E****p****o****c****h****:****4****4****3****）**
[
](https://img-blog.csdn.net/20170314081856037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 由
> 于
> 仅
> 作
> 为
> 示
> 例
> 以
> 及
> 时
> 间
> 和
> 计
> 算
> 资
> 源
> 的
> 限
> 制
> ，
> 从
> 模
> 型
> 结
> 构
> 到
> 优
> 化
> 器
> 的
> 参
> 数
> 都
> 没
> 有
> 经
> 过
> 任
> 何
> 调
> 优
> 。
> 因
> 此
> ，
> 这
> 里
> 生
> 成
> 的
> 图
> 片
> 的
> 质
> 量
> 不
> 应
> 该
> 做
> 为
> 算
> 法
> 优
> 劣
> 的
> 依
> 据
> （
> 下
> 同
> ）
> 。
[

](https://img-blog.csdn.net/20170314081856037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 3. Wasserstein GAN(WGAN)
[
](https://img-blog.csdn.net/20170314081856037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)WGAN采用线性的损失函数，为此我们定义：
[

](https://img-blog.csdn.net/20170314081856037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
def
```
```python
dummy_loss
```
```python
(loss_to_backprop, y_pred)
```
```python
:
```
```python
return
```
```python
K.mean(loss_to_backprop * y_pred)
```
```python
# delta == loss_to_backprop
```
```python
disc.compile(optimizer=Adam(lr=lr),loss=dummy_loss)
combined.compile(optimizer=Adam(lr=lr), loss=dummy_loss)
```
[
](https://img-blog.csdn.net/20170314081856037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)为应用这个损失函数，代码更改如下（第12和18行，change2、3）。
[

](https://img-blog.csdn.net/20170314081856037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
for
```
```python
epoch
```
```python
in
```
```python
range(nb_epochs):
```
```python
for
```
```python
index
```
```python
in
```
```python
range(nb_batches):
```
```python
## 1) 训练判别器
```
```python
# 1.1采样隐变量并生成假样本
```
```python
noise = np.random.uniform(-
```
```python
1
```
```python
,
```
```python
1
```
```python
, (batch_size, latent_size))
        generated_images = generator.predict(noise, verbose=
```
```python
0
```
```python
)
```
```python
# 1.2 从训练中采样真实样本
```
```python
image_batch = X_train[index * batch_size:(index +
```
```python
1
```
```python
) * batch_size]
        label_batch = y_train[index * batch_size:(index +
```
```python
1
```
```python
) * batch_size]
```
```python
# 利用真假数据进行训练
```
```python
X = np.concatenate((image_batch, generated_images))        
        y = np.array([-
```
```python
1
```
```python
] * len(image_batch) + [
```
```python
1
```
```python
] * batch_size)
```
```python
## change 2
```
```python
disc.train_on_batch(X, y)
```
```python
## 2）训练生成器
```
```python
# 采样隐变量
```
```python
noise = np.random.uniform(-
```
```python
1
```
```python
,
```
```python
1
```
```python
, (batch_size, latent_size))
        target = -np.ones(batch_size)
```
```python
## change 3
```
```python
combined.train_on_batch(noise, target)
```
[
](https://img-blog.csdn.net/20170314081856037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)WGAN有如下突出优点[6]：
*训练稳定，不需要平稳生成器和判别器。
*loss值与生成样本质量相关，可以用来监督训练进程，不需要人工判断干预。
完整的示例可以参见repo。读者可以自行验证，D_loss及生成图像的质量变化。
# 4. GLSGAN
[7]提出了LossSensitiveGAN，并随后发现，可以和WGAN在统一的框架下研究，即generalizedLSGAN（图3）。
![这里写图片描述](https://img-blog.csdn.net/20170314175357846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170314175357846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图****3****.****【****s****r****c****】**
[
](https://img-blog.csdn.net/20170314175357846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)GLSGAN使用LeakyReLU作用激活，其中$s \in （-\infty, 1]$。

$$
LeakyReLU(x) = \{
 \begin{array}
 xx,  \ if\ x \ge 0, \\
 s\cdot x, \ x < 0.
 \end{array}
$$

```python
def
```
```python
build_discriminator
```
```python
()
```
```python
:
```
```python
model = Sequential()
    model.add(Flatten(input_shape=(
```
```python
1
```
```python
,
```
```python
28
```
```python
,
```
```python
28
```
```python
)))
    model.add(Dense(
```
```python
256
```
```python
, activation=
```
```python
'relu'
```
```python
))
    model.add(Dense(
```
```python
128
```
```python
, activation=
```
```python
'relu'
```
```python
))
    model.add(Dense(
```
```python
1
```
```python
)， activation=
```
```python
'linear'
```
```python
)
    model.add(LeakyReLU(slope))
```
```python
##
```
```python
return
```
```python
model
```
下面是不同s下，训练的模型生成的示例图片。
![slope==1， WGAN](https://img-blog.csdn.net/20170313184548123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170313184548123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图****4****.****S****l****o****p****e****:****1****(****W****G****A****N****)****,****E****p****o****c****h****:****1****7****0**
[
](https://img-blog.csdn.net/20170313184548123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![slope==0, LS-GAN](https://img-blog.csdn.net/20170313184509592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170313184509592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图****5****.****S****l****o****p****e****:****0****(****L****o****s****s****S****e****n****s****i****t****i****v****e****G****A****N****)****,****E****p****o****c****h****:****1****8****9**
[
](https://img-blog.csdn.net/20170313184509592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![slope==-1, L1](https://img-blog.csdn.net/20170314081939914?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170314081939914?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图****6****.****S****l****o****p****e****:****-****1****(****L****1****L****o****s****s****)****,****E****p****o****c****h****:****3****9****9**
[

](https://img-blog.csdn.net/20170314081939914?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 非线性损失
[
](https://img-blog.csdn.net/20170314081939914?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)GLSGAN并不限定损失函数为（分段）线性。这里使用ExponentialLinearUnit（ELU）。

$$
ELU(x) = \{
 \begin{array}
 xx,  \ if\ x \ge 0, \\
 \alpha\cdot(\exp(x)-1), \ x < 0.
 \end{array}
$$
![](https://ai2-s2-public.s3.amazonaws.com/figures/2016-11-08/0373b97580cdfd0b69f165e1a946bae62da95dce/1-Figure2-1.png)[ ](https://ai2-s2-public.s3.amazonaws.com/figures/2016-11-08/0373b97580cdfd0b69f165e1a946bae62da95dce/1-Figure2-1.png)
**图****7****.****E****x****p****o****n****e****n****t****i****a****l****L****i****n****e****a****r****U****n****i****t****v****s****.****R****e****L****U****【****s****r****c****】**
[

](https://ai2-s2-public.s3.amazonaws.com/figures/2016-11-08/0373b97580cdfd0b69f165e1a946bae62da95dce/1-Figure2-1.png)
```python
def
```
```python
build_discriminator
```
```python
()
```
```python
:
```
```python
# Other Code goes here...
```
```python
model.add(ELU)
```
```python
# Exponential Linear Unit
```
```python
return
```
```python
model
```
[
](https://ai2-s2-public.s3.amazonaws.com/figures/2016-11-08/0373b97580cdfd0b69f165e1a946bae62da95dce/1-Figure2-1.png)![ELU](https://img-blog.csdn.net/20170314082044960?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170314082044960?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图****8****.****E****L****U****,****E****p****o****c****h****:****3****6****7**
[
](https://img-blog.csdn.net/20170314082044960?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)一个完整的示例见repo。基于torch的officialrepo。
[

](https://img-blog.csdn.net/20170314082044960?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 5. 讨论
[

](https://img-blog.csdn.net/20170314082044960?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 损失函数
[
](https://img-blog.csdn.net/20170314082044960?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)损失函数唯一重要的地方在于，不断驱动两个网络的竞争。直观上，判别网络将真实样本和生成样本，向坐标轴上的两个不同的区域移动。
*对于basicGAN，这两个区域分别分别是0（生成）和1（真实），使用的损失函数是对数函数（$f(x)=-\log(x)$）（即交叉熵）[1]。
*对于LeastSquaredGAN，这两个区域分别是a和b（a<b），使用的损失函数是二次函数（$f(x)= x^2$）[4]。
*对于WGAN，这两个区域分别$+\infty$（真实样本）和$-\infty$（生成样本），使用的是线性损失函数（$f(x)=x$）[6]。
*对于Loss-SensitiveGAN，这两个区域分别是$+\infty$（真实样本）和$(-\infty, 0]$[7]，使用的是ReLu损失函数。
*对于GeneralizedLSGAN（$0 < \gamma < 1$），这两个区域分别是$+\infty$（真实样本）和$(-\infty, 0]$[7]，使用的是LeakyReLu损失函数。
*对于GeneralizedLSGAN（$\gamma < 0$），这两个区域分别是$+\infty$（真实样本）和$0$[7]，使用的是分段线性的损失函数。
> 从
> 损
> 失
> 函
> 数
> 的
> 角
> 度
> ，
> B
> a
> s
> i
> c
> G
> A
> N
> 几
> 乎
> 选
> 择
> 了
> 一
> 个
> 最
> 差
> 的
> 方
> 案
> —
> —
> 经
> 过
> s
> i
> g
> m
> o
> i
> d
> 激
> 活
> 后
> ，
> 损
> 失
> 函
> 数
> 在
> 0
> -
> 1
> 两
> 端
> 都
> 存
> 在
> 饱
> 和
> 区
> 。

## 关于GLSGAN
当$\gamma < 0$时，从形式上，GLSGAN其实已经不能叫做LossSensitive了。因为此时GLSGAN的行为更向是LeastSquaresGAN——将生成样本向某个点推（零点）。不过GLSGAN对于真实样本更激进，它会不断将真实样本向$+\infty$推。另一个不同是，GLSGAN使用线性的函数，而LSGAN使用二次函数。
> T
> O
> D
> O
> 此
> 处
> 有
> 一
> 个
> 疑
> 问
> 待
> 解
> 决
> ：
> 文
> 章
> 中
> 说
> L
> e
> a
> s
> t
> S
> q
> u
> a
> r
> e
> s
> G
> A
> N
> 也
> 存
> 在
> 梯
> 度
> 消
> 失
> 的
> 问
> 题
> 。
> 从
> 形
> 式
> 上
> 看
> ，
> 虽
> 然
> 一
> 次
> 函
> 数
> 在
> 极
> 值
> 附
> 近
> 梯
> 度
> 接
> 近
> 0
> ，
> 但
> 由
> 于
> 正
> 负
> 样
> 本
> 的
> 损
> 失
> 函
> 数
> 的
> 极
> 值
> 点
> 不
> 同
> ，
> 因
> 此
> ，
> 直
> 觉
> 上
> ，
> 在
> 对
> 抗
> 训
> 练
> 过
> 程
> 中
> 应
> 该
> 不
> 会
> 出
> 现
> 梯
> 度
> 消
> 失
> 的
> 现
> 象
> 。
> 看
> 到
> 需
> 要
> 进
> 一
> 步
> 提
> 高
> 理
> 论
> 修
> 养
> 。

## Regularities
这里我们没有关注正则性约束，但WGAN，GLSGAN要求判别器是Lipschitz（相对于模型参数）。直观上，Lipschitz保证训练过程中，不会因为参数更新引起模型的跳跃性变化，确保训练过程平稳。
# 6. 结语
形式上，各种方法仅仅是损失函数不太一样，但损失函数的选择并不trivial。basicGAN训练困难已经表明了GAN对抗的训练方式对损失函数的非常的敏感。没有严谨的理论支撑，随意的损失函数并不能保证训练如预期进行（收敛且稳定）。
鲁棒的GAN训练方法对于GAN在广阔领域的应用将是非常大的推动力（如最近的压缩感知应用）。
对不同损失函数（不同GAN）的性质，目前还缺少系统性的比较研究，期待更新的研究结果。
# References
IanGoodfellowetal.(2014).GenerativeAdversarialNetworks.
IanGoodfellow.(2016).NIPS2016Tutorial:GenerativeAdversarialNetworks.
Nowozinetal.(2016).f-GAN:TrainingGenerativeNeuralSamplersusingVariationalDivergenceMinimization.
Maoetal.(2016).LeastSquaresGenerativeAdversarialNetworks.
Arjovskyetal.(2016).TowardsPrincipledMethodsforTrainingGenerativeAdversarialNetworks.
Arjovskyetal.(2017).WassersteinGAN.
Qi.(2017).Loss-SensitiveGenerativeAdversarialNetworksonLipschitzDensities.
AnIncompleteMapoftheGANmodels.
LS-GAN：把GAN建立在Lipschitz密度上.
广义LS-GAN（GLS-GAN).
# Further Reading
Aroraetal.(2017).GeneralizationandEquilibriuminGenerativeAdversarialNets(GANs).
Aroraetal.(2017).DoGANsactuallylearnthedistribution?Anempiricalstudy.


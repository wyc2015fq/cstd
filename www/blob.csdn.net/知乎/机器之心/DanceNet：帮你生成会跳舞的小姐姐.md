# DanceNet：帮你生成会跳舞的小姐姐 - 知乎
# 



**选自GitHub，机器之心整理，参与：思源、张倩。**

> 最近有开发者尝试构建能自动生成舞蹈动作的深度网络，他们结合了变分自编码器、LSTM 与混合密度网络，并将这一深度网络命名为 DanceNet。该网络的主要思想即使用 VAE 生成单张舞蹈图片，并根据 LSTM 将这些舞蹈图片组合成一系列完整的动作，最后联合训练就能生成非常逼真的舞蹈动作。机器之心也尝试使用了该项目，并能生成还不错的舞蹈视频，感兴趣的读者也可以使用并完善该项目。

项目地址：[https://github.com/jsn5/dancenet](https://link.zhihu.com/?target=https%3A//github.com/jsn5/dancenet)

**主要模块**

DanceNet 中最主要的三个模块是变分自编码器、LSTM 与 MDN。其中变分自编码器（VAE）是最常见的生成模型之一，它能以无监督的方式学习复杂的分布，因此常被用来生成图像数据。VAE 非常优秀的属性是可以使用深度神经网络和随机梯度下降进行训练，并且中间的隐藏编码还表示了图像的某些属性。

如下变分自编码器中的编码器使用三个卷积层和一个全连接层，以生成隐藏编码 z 分布的均值与方差。因为 z 服从于高斯分布，因此确定方差与均值后 z 的分布就完全确定了，随后解码器从 z 分布中采样隐藏编码就能解码为与输出图像相近的输出。


```
input_img = Input(shape=(120,208,1))
x = Conv2D(filters=128,kernel_size=3, activation='relu', padding='same')(input_img)
x = MaxPooling2D(pool_size=2)(x)
x = Conv2D(filters=64,kernel_size=3, activation='relu', padding='same')(x)
x = MaxPooling2D(pool_size=2)(x)
x = Conv2D(filters=32,kernel_size=3, activation='relu', padding='same')(x)
x = MaxPooling2D(pool_size=2)(x)
shape = K.int_shape(x)
x = Flatten()(x)
x = Dense(128,kernel_initializer='glorot_uniform')(x)

z_mean = Dense(latent_dim)(x)
z_log_var = Dense(latent_dim)(x)
z = Lambda(sampling, output_shape=(latent_dim,), name="z")([z_mean,z_log_var])

encoder = Model(input_img, [z_mean, z_log_var,z], name="encoder")
```


z_mean 和 z_log_var 为编码器编码的均值与方差，z 为从它们确定的分布中所采样的隐藏编码，编码器最后输出这三个变量。以下展示了 Jaison 所采用解码器的架构，其首通过全连接层对隐藏编码 z 执行仿射变换，再交叉通过 4 个卷积层与 3 个上采样层以将隐藏编码恢复为原始输入图像大小。


```
latent_inputs = Input(shape=(latent_dim,), name='z_sampling')
x = Dense(shape[1] * shape[2] * shape[3], kernel_initializer='glorot_uniform',activation='relu')(latent_inputs)
x = Reshape((shape[1],shape[2],shape[3]))(x)
x = Dense(128,kernel_initializer='glorot_uniform')(x)
x = Conv2D(filters=32, kernel_size=3, activation='relu', padding='same')(x)
x = UpSampling2D(size=(2,2))(x)
x = Conv2D(filters=64,kernel_size=3, activation='relu', padding='same')(x)
x = UpSampling2D(size=(2,2))(x)
x = Conv2D(filters=128,kernel_size=3, activation='relu', padding='same')(x)
x = UpSampling2D(size=(2,2))(x)
x = Conv2D(filters=1,kernel_size=3, activation='sigmoid', padding='same')(x)

decoder = Model(latent_inputs,x,name='decoder')
```


在训练 VAE 后，我们从高斯分布中采样一个隐藏编码 z，并将其馈送到解码器，那么模型就能生成新的图像。因此我们可以设想给定不同的隐藏编码 z，解码器最终能生成不同的舞姿图像。

最后，我们还需要长短期记忆网络（LSTM）和混合密度层以将这些舞姿图像连接在一起，并生成真正的舞蹈动作。如下 Jaison 堆叠了三个 LSTM 层，且每一个 LSTM 层后面都采用 Dropout 操作以实现更好的鲁棒性。将循环层的输出接入全连接层与混合密度网络层，最后输出一套舞蹈动作每一个时间步的图像应该是怎样的。


```
inputs = Input(shape=(128,))
x = Reshape((1,128))(inputs)
x = LSTM(512, return_sequences=True,input_shape=(1,128))(x)
x = Dropout(0.40)(x)
x = LSTM(512, return_sequences=True)(x)
x = Dropout(0.40)(x)
x = LSTM(512)(x)
x = Dropout(0.40)(x)
x = Dense(1000,activation='relu')(x)
outputs = mdn.MDN(outputDim, numComponents)(x)
model = Model(inputs=inputs,outputs=outputs)
```


以下是实践该项目的环境与过程，机器之心也尝试使用了预训练模型，并生成了效果还不错的视频。此外，根据试验结果，VAE 中的编码器参数数量约 172 万，解码器约为 174 万，但 LSTM+MDN 却有 1219 万参数。最后我们生成了一个 16 秒的舞蹈视频：
[dancenet生成的舞蹈视频_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/s1346hkpr0a.html)
**要求：**
- Python = 3.5.2

**工具包：**
- keras==2.2.0
- sklearn==0.19.1
- numpy==1.14.3
- opencv-python==3.4.1

如下展示了用于训练的数据集视频：
[Dancenet数据集_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/k0752rcnsiq.html)



**本地实现已训练模型：**
- 下载预训练权重
- 提取到 dancenet 目录
- 运行 dancegen.ipynb
- 预训练权重下载地址：[https://drive.google.com/file/d/1LWtERyPAzYeZjL816gBoLyQdC2MDK961/view?usp=sharing](https://link.zhihu.com/?target=https%3A//drive.google.com/file/d/1LWtERyPAzYeZjL816gBoLyQdC2MDK961/view%3Fusp%3Dsharing)

**如何在浏览器上运行：**
- 打开 FloydHub 工作区
- 训练的权重数据集将自动与环境相连
- 运行 dancegen.ipynb
- FloydHu 工作区：[https://floydhub.com/run](https://link.zhihu.com/?target=https%3A//floydhub.com/run)
- 预训练权重：[https://www.floydhub.com/whatrocks/datasets/dancenet-weights](https://link.zhihu.com/?target=https%3A//www.floydhub.com/whatrocks/datasets/dancenet-weights)




**从头开始训练：**
- 在 imgs/ 文件夹下补充标签为 1.jpg, 2.jpg ... 的舞蹈序列图像
- 运行 model.py
- 运行 gen_lv.py 以编码图像
- 运行 video_from_lv.py 以测试解码视频
- 运行 jupyter notebook 中的 dancegen.ipynb 以训练 dancenet 并生成新视频




**参考资料：**
- Cary Huang：[https://www.youtube.com/watch?v=Sc7RiNgHHaE&t=9s](https://link.zhihu.com/?target=https%3A//www.youtube.com/watch%3Fv%3DSc7RiNgHHaE%26t%3D9s)
- Francois Chollet：[https://blog.keras.io/building-autoencoders-in-keras.html](https://link.zhihu.com/?target=https%3A//blog.keras.io/building-autoencoders-in-keras.html)
- 通过 Python 和 Keras 使用 LSTM 循环神经网络构建时序预测模型：[https://machinelearningmastery.com/time-series-prediction-lstm-recurrent-neural-networks-python-keras/](https://link.zhihu.com/?target=https%3A//machinelearningmastery.com/time-series-prediction-lstm-recurrent-neural-networks-python-keras/)
- 使用深度学习的生成编舞：[https://arxiv.org/abs/1605.06921](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1605.06921)
- David Ha：[http://blog.otoro.net/2015/06/14/mixture-density-networks/](https://link.zhihu.com/?target=http%3A//blog.otoro.net/2015/06/14/mixture-density-networks/)
- Charles Martin：[https://github.com/cpmpercussion/keras-mdn-layer](https://link.zhihu.com/?target=https%3A//github.com/cpmpercussion/keras-mdn-layer)







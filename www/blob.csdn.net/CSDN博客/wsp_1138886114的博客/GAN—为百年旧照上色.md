# GAN—为百年旧照上色 - wsp_1138886114的博客 - CSDN博客





2018年11月06日 19:44:50[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：642








### 前言

Attention GAN 该项目的目的是为旧照片着色并将其修复。带自注意力机制的生成对抗网络。生成器是一个预训练 Unet，我将它修改为具有光谱归一化和自注意力。这是一个非常简单的转换过程。首先，先来看一下 DeOldify 对旧照片的修复效果！
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181106193141444.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)- 
受 GAN Progressive Growing 的启发（参见《Progressive Growing of GANs for Improved Quality, Stability, and Variation》）设计出的训练结构。不同之处在于层数保持不变——我只是不断改变输入的尺寸并调整学习率，以确保尺寸之间的转换顺利进行。似乎基本最终结果是相同的——训练更快、更稳定，且泛化效果更好。

- 
两个时间尺度上的更新规则（参见《GANs Trained by a Two Time-Scale Update Rule Converge to a Local Nash Equilibrium》）。这个也非常简单，只是一个一对一生成器/critic 迭代及较高级别的 critic 学习率。

- 
生成器损失分为两个部分：
- 一部分是基于 VGG16 的基本感知损失（或特征损失）——这只是偏向生成器模型来复制输入图像。
- 另一部分是来自 critic 的损失分数。感知损失本身不足以产生好的结果，只会带来大量的棕色/绿色/蓝色填充——测试的时候作弊可是神经网络的强项！这里要理解的重点是：GAN 本质上是在为你学习损失函数——这实际上是朝着我们在机器学习中追求的理想迈进了一大步。当然，让机器学习你以前手工编码的东西通常会得到更好的结果。在这个例子中就是这样。


该项目基于 [Fast.AI](http://Fast.AI) 库构建。不过，我使用的是旧版本库，即将更新为新版本。构建该项目的先决条件是：
- 旧版本 [Fast.AI](http://Fast.AI) 库。埋头该项目两个月后，我有点疑惑，因为标注为「old」的旧版本 [Fast.AI](http://Fast.AI) 库和我用的库不一样。因此建议使用这里的库：[https://github.com/jantic/fastai。](https://github.com/jantic/fastai%E3%80%82)
- 不管 [Fast.AI](http://Fast.AI) 的依赖项是什么，总有方便的 requirements.txt 和 environment.yml。
- Pytorch 0.4.1（需要 spectral_norm，因此需要最新的稳定版本）。
- Jupyter Lab
- Tensorboard（即安装 Tensorflow）和 TensorboardX ([https://github.com/lanpa/tensorboardX](https://github.com/lanpa/tensorboardX))。
- ImageNet：训练数据集。
- 强大的 GPU：我想要比 GeForce 1080TI (11GB) 更大内存的 GPU。内存越少花费时间就会越多。Unet 和 Critic 非常大，但是它们规模越大，获得的结果也会越好。

至于想要立刻开始转换自己的图像的人：如果要立刻开始使用自己的图像而不训练模型的话，你需要我上传预训练权重。我正在做这件事。做好之后，你可以在可视化 notebook 中看到。我将使用 ColorizationVisualization.ipynb。你需要设置 `colorizer_path = IMAGENET.parent/('bwc_rc_gen_192.h5')`。我上传的权重文件用于生成器（着色器）。

然后你就可以把想转换的任意图像放在/test_images/文件夹，然后在 notebook 内部看到结果：

```python
vis.plot_transformed_image("test_images/derp.jpg", \
                            netG, md.val_ds, tfms=x_tfms, sz=500)
```

我把图像大小设置为 500px 左右，你需要在足够内存的 GPU 上运行（比如 11 GB GeForce 1080Ti）。如果内存过少，你需要将图像调小或者尝试在 CPU 上运行模型。

项目地址：[https://github.com/jantic/DeOldify](https://github.com/jantic/DeOldify)





# 用自注意力GAN为百年旧照上色：效果惊艳，多图预警！ - 知乎
# 



选自GitHub，作者：Jason Antic，机器之心编译，参与：路、张倩。

> 本文介绍了一个旧照片着色&修复神器DeOldify，包括修复效果、技术细节等。

项目地址：[https://github.com/jantic/DeOldify/blob/master/README.md](https://link.zhihu.com/?target=https%3A//github.com/jantic/DeOldify/blob/master/README.md)

该项目的目的是为旧照片着色并将其修复。本文将简要介绍技术细节。首先，先来看一下 DeOldify 对旧照片的修复效果！（大部分原图像来自 r/TheWayWeWere subreddit。）
![](https://pic4.zhimg.com/v2-8ce4ef72236b3e8cbc38765261ff7dab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='407'></svg>)和人体骨骼一起拍照的医学生（约 1890 年）![](https://pic1.zhimg.com/v2-fc45d5859931fc44e1e56c402644f690_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1058' height='582'></svg>)Whirling Horse（1898）![](https://pic4.zhimg.com/v2-2c8a42210a39f70a4966d5c89bd80063_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='449'></svg>)19 世纪 80 年代的巴黎![](https://pic3.zhimg.com/v2-1bc1a310df317277250a6575ed10c9be_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='591'></svg>)20 世纪 20 年代的爱丁堡俯瞰![](https://pic1.zhimg.com/v2-dcf8a00e9daf91e9e3bcacff325e54fc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='366'></svg>)伦敦滑铁卢车站人们第一次看到电视机（1936）![](https://pic4.zhimg.com/v2-23181f57985a28e7ad09944fca3d58ff_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='355'></svg>)抽鸦片的大清子民（1880）
非常旧和质量差的照片也能修复得很好：
![](https://pic4.zhimg.com/v2-c95c350b09e4fc3694f661bc7f5e532b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='318'></svg>)南达科他州枯木镇（1877）![](https://pic1.zhimg.com/v2-a5e4d2350b0fdd743d1e196da4b121e4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='320'></svg>)家庭合照（1877）
但，模型效果并不总是完美的。下图中红手让人抓狂：
![](https://pic3.zhimg.com/v2-bc435cebd81f9f88269aa0e701160ff2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='740'></svg>)塞内卡原住民（1908）
该模型还能对黑白线稿进行着色：
![](https://pic3.zhimg.com/v2-09dfb9f55b85ed7efa19bf4fd564d35a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='319'></svg>)
**技术细节**

这是一个基于深度学习的模型。具体来说，我所做的是将以下方法组合在一起：
- 带自注意力机制的生成对抗网络。生成器是一个预训练 Unet，我将它修改为具有光谱归一化和自注意力。这是一个非常简单的转换过程。不过，之前我拼命地尝试用 Wasserstein GAN，但效果并不好，直到用上这个版本，一切都变了。我喜欢 Wasserstein GAN 的理念，但它在实践中并没有成功。我爱上了带自注意力的 GAN。
- 受 GAN Progressive Growing 的启发（参见《Progressive Growing of GANs for Improved Quality, Stability, and Variation》）设计出的训练结构。不同之处在于层数保持不变——我只是不断改变输入的尺寸并调整学习率，以确保尺寸之间的转换顺利进行。似乎基本最终结果是相同的——训练更快、更稳定，且泛化效果更好。
- 两个时间尺度上的更新规则（参见《GANs Trained by a Two Time-Scale Update Rule Converge to a Local Nash Equilibrium》）。这个也非常简单，只是一个一对一生成器/critic 迭代及较高级别的 critic 学习率。
- 生成器损失分为两个部分：一部分是基于 VGG16 的基本感知损失（或特征损失）——这只是偏向生成器模型来复制输入图像。另一部分是来自 critic 的损失分数。感知损失本身不足以产生好的结果，只会带来大量的棕色/绿色/蓝色填充——测试的时候作弊可是神经网络的强项！这里要理解的重点是：GAN 本质上是在为你学习损失函数——这实际上是朝着我们在机器学习中追求的理想迈进了一大步。当然，让机器学习你以前手工编码的东西通常会得到更好的结果。在这个例子中就是这样。

该模型的美妙之处在于，它可能适用于所有类型的图像修复，效果也应该很好。上文展示了很多模型结果，但这只是我想开发的流程的一个组件。

接下来，我想做的是「defade」模型。我已经做了一些初步努力，截至本文写作时，它还在训练阶段。大体上，是训练同样的模型用于重建使用过分的对比度／亮度调整后的图像。我已经看到了一些还不错的试验结果：
![](https://pic4.zhimg.com/v2-ee842a9f501748c6bbcbc2e7a400a647_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='488'></svg>)
**关于该项目**

该项目的重点是：我想利用 GAN 使旧照片变得好看，更重要的是，做一个有用的项目。我对视频很感兴趣，但是首先我需要先解决如何处理模型和内存的关系。如果该模型在 1080TI 上的训练时间不用花费两三天就太棒了（然而 GAN 通常需要这么长时间……）我将积极更新和改进代码，并尝试使该项目对用户友好。

**开始操作**

该项目基于 Fast.AI 库构建。不过，我使用的是旧版本库，即将更新为新版本。构建该项目的先决条件是：
- 旧版本 Fast.AI 库。埋头该项目两个月后，我有点疑惑，因为标注为「old」的旧版本 Fast.AI 库和我用的库不一样。因此建议使用这里的库：[https://github.com/jantic/fastai](https://link.zhihu.com/?target=https%3A//github.com/jantic/fastai)。
- 不管 Fast.AI 的依赖项是什么，总有方便的 requirements.txt 和 environment.yml。
- Pytorch 0.4.1（需要 spectral_norm，因此需要最新的稳定版本）。
- Jupyter Lab
- Tensorboard（即安装 Tensorflow）和 TensorboardX ([https://github.com/lanpa/tensorboardX](https://link.zhihu.com/?target=https%3A//github.com/lanpa/tensorboardX))。
- ImageNet：很棒的训练数据集。
- 强大的 GPU：我想要比 GeForce 1080TI (11GB) 更大内存的 GPU。内存越少花费时间就会越多。Unet 和 Critic 非常大，但是它们规模越大，获得的结果也会越好。

至于想要立刻开始转换自己的图像的人：如果要立刻开始使用自己的图像而不训练模型的话，你需要我上传预训练权重。我正在做这件事。做好之后，你可以在可视化 notebook 中看到。我将使用 ColorizationVisualization.ipynb。你需要设置 colorizer_path = IMAGENET.parent/('bwc_rc_gen_192.h5')。我上传的权重文件用于生成器（着色器）。

然后你就可以把想转换的任意图像放在/test_images/文件夹，然后在 notebook 内部看到结果：

`vis.plot_transformed_image("test_images/derp.jpg", netG, md.val_ds, tfms=x_tfms, sz=500)`

我把图像大小设置为 500px 左右，你需要在足够内存的 GPU 上运行（比如 11 GB GeForce 1080Ti）。如果内存过少，你需要将图像调小或者尝试在 CPU 上运行模型。我试过后一种方法，非常慢……我还没有研究具体原因。 
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*





